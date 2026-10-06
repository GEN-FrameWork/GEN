/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       INPLINUXDeviceMouseDirect.cpp
* 
* @class      INPLINUXDEVICEMOUSEDIRECT
* @brief      LINUX direct input device mouse class
* @ingroup    PLATFORM_LINUX
* 
* @copyright  EndoraSoft. All rights reserved.
* 
* @cond
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
* documentation files(the "Software"), to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense, and/ or sell copies of the Software,
* and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of
* the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
* THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
* @endcond
* 
* --------------------------------------------------------------------------------------------------------------------*/

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "INPLINUXDeviceMouseDirect.h"

#include <unistd.h>
#include <locale.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>
#include <cerrno>

#include "XTimer.h"
#include "XFileTXT.h"
#include "XTrace.h"

#include "GRPScreen.h"

#include "INPLINUXFactory.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPLINUXDEVICEMOUSEDIRECT::INPLINUXDEVICEMOUSEDIRECT(): INPDEVICE()
* @brief      Constructor of class
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPLINUXDEVICEMOUSEDIRECT::INPLINUXDEVICEMOUSEDIRECT(): INPDEVICE()
{
  Clean();

  SetType(INPDEVICE_TYPE_MOUSE);

  created = true;

  if(CreateAllButtons())
    {
      if(CreateAllCursors())
        {
          SetEnabled(true);
        }
    }

  UpdateMouseData();

  OpenAllDevicesID();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPLINUXDEVICEMOUSEDIRECT::~INPLINUXDEVICEMOUSEDIRECT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPLINUXDEVICEMOUSEDIRECT::~INPLINUXDEVICEMOUSEDIRECT()
{
  CloseAllDevicesID();

  mice.DeleteContents();
  mice.DeleteAll();

  DeleteAllButtons();
  DeleteAllCursors();

  SetEnabled(false);
  created = false;

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPLINUXDEVICEMOUSEDIRECT::Update()
* @brief      Update
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPLINUXDEVICEMOUSEDIRECT::Update()
{
  if((!created)||(!enabled)) return false;

  int                 eventsize = sizeof(struct input_event);
  int                 bytesread = 0;

  struct input_event  event;

  static int          delta     = 0;
  static int          hdelta    = 0;

  //--------------- clean up old states

  for(XDWORD c=0; c<buttons.GetSize(); c++)
    {
      INPBUTTON* button=buttons.Get(c);
      if(button)
        {
          if(button->GetState() == INPBUTTON_STATE_RELEASED)
            {
              button->SetState(INPBUTTON_STATE_UP);
            }
           else
            {
              if(button->GetState() == INPBUTTON_STATE_PRESSED) button->SetState(INPBUTTON_STATE_HOLD);
           }
        }
    }

  //--------------- process

  if(mice.GetSize() == 0)
    {
      OpenAllDevicesID();
    }

  for(XDWORD e=0; e<mice.GetSize(); e++)
    {
      while(true)
        {
          bytesread=0;

          memset(&event, 0, sizeof(event));

          errno = 0;
          bytesread = read(mice.Get(e)->GetFileDescriptor(), &event, sizeof(event)/*eventsize*/);

          // if device is diconnected we receive a ENODEV message (happens for example every time a touchscreen is poweredoff)

          if(bytesread == -1 && errno == ENODEV)
            {
              // we lost the handle to the event file, need to reclaim it
              mice.DeleteContents();
              OpenAllDevicesID();
            }

          if(bytesread==-1 && errno!=EAGAIN) break;

          if((bytesread < sizeof(struct input_event)) || (bytesread == -1))  break;

          if(bytesread == sizeof(event)/*eventsize*/)
            {
              switch(event.type)
                {
                  case EV_ABS : switch(event.code)
                                  {
                                    case ABS_X      : { mousex  = event.value;
                                                        float x = ((float)mousex / (float)maxx);

                                                        mousex  = (int)(x * grpscreen->GetWidth());
                                                      }
                                                      break;

                                    case ABS_Y      : { mousey  = event.value;
                                                        float y = ((float)mousey / (float)maxy);

                                                        mousey  = (int)(y * grpscreen->GetHeight());
                                                        mousey  = (grpscreen->GetHeight()-mousey);

                                                      }
                                                      break;

                                    case ABS_Z      : mousez  = event.value;  break;
                                    case REL_HWHEEL : hdelta  = event.value;  break;
                                    case REL_WHEEL  : delta   = event.value;  break;
                                  }
                                break;

                  case EV_REL : switch(event.code)
                                  {
                                    case REL_X      : mousex  += event.value; break;
                                    case REL_Y      : mousey  -= event.value; break;
                                    case REL_Z      : mousez  += event.value; break;
                                    case REL_HWHEEL : hdelta  += event.value; break;
                                    case REL_WHEEL  : delta   += event.value; break;
                                  }
                                break;


                  case EV_KEY : { INPBUTTON* button = GetButtonByCode(event.code);
                                  if(button)
                                    {
                                      switch(button->GetState())
                                        {
                                          case INPBUTTON_STATE_UP       : (event.value==0) ? button->SetState(INPBUTTON_STATE_UP)       : button->SetState(INPBUTTON_STATE_PRESSED);  break;
                                          case INPBUTTON_STATE_PRESSED  : (event.value==0) ? button->SetState(INPBUTTON_STATE_RELEASED) : button->SetState(INPBUTTON_STATE_HOLD);     break;
                                          case INPBUTTON_STATE_RELEASED :
                                          case INPBUTTON_STATE_HOLD     : (event.value==0) ? button->SetState(INPBUTTON_STATE_RELEASED) : button->SetState(INPBUTTON_STATE_PRESSED);  break;
                                                            default     : break;
                                        }

                                      button->SetPressed((event.value==1)?true:false);
                                    }

                                  // emulate mouse button
                                  if(event.code == BTN_TOUCH)
                                    {
                                      button = GetButtonByCode(BTN_LEFT);
                                      if(button)
                                        {
                                          switch(button->GetState())
                                            {
                                              case INPBUTTON_STATE_UP       : (event.value==0) ? button->SetState(INPBUTTON_STATE_UP)       : button->SetState(INPBUTTON_STATE_PRESSED);  break;
                                              case INPBUTTON_STATE_PRESSED  : (event.value==0) ? button->SetState(INPBUTTON_STATE_RELEASED) : button->SetState(INPBUTTON_STATE_HOLD);     break;
                                              case INPBUTTON_STATE_RELEASED :
                                              case INPBUTTON_STATE_HOLD     : (event.value==0) ? button->SetState(INPBUTTON_STATE_RELEASED) : button->SetState(INPBUTTON_STATE_PRESSED);  break;
                                                                default     : break;
                                            }

                                          button->SetPressed((event.value==1)?true:false);
                                        }
                                    }
                                }
                                break;

                  #ifdef EV_SYN
                  case EV_SYN :  //  UpdateMouseData();
                                break;
                  #endif

                      default : continue;
                }

            } else break;
        }
    }

  UpdateMouseData();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPLINUXDEVICEMOUSEDIRECT::CreateAllButtons()
* @brief      Create all buttons
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPLINUXDEVICEMOUSEDIRECT::CreateAllButtons()
{
  INPBUTTON::CreateButton(&buttons, BTN_RIGHT, INPBUTTON_ID_MOUSE_RIGHT, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, BTN_LEFT, INPBUTTON_ID_MOUSE_LEFT, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, BTN_MIDDLE, INPBUTTON_ID_MOUSE_MIDDLE, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, BTN_TOUCH, INPBUTTON_ID_TOUCHSCREEN, _C('\x0'));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPLINUXDEVICEMOUSEDIRECT::CreateAllCursors()
* @brief      Create all cursors
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPLINUXDEVICEMOUSEDIRECT::CreateAllCursors()
{
  INPCURSOR* cursor = GEN_NEW INPCURSOR();
  if(!cursor) return false;

  cursor->SetID(INPCURSOR_ID_MOUSE);

  cursor->SetHavePreSelect(true);

  cursors.Add(cursor);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPLINUXDEVICEMOUSEDIRECT::OpenAllDevicesID()
* @brief      Open all devices ID
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPLINUXDEVICEMOUSEDIRECT::OpenAllDevicesID()
{
  INPLINUXFACTORY*     inpdevices = (INPLINUXFACTORY*)(&INPFACTORY::GetInstance());
  XVECTOR<INPLINUXDEVICEID*>  devicesID;
      
  if(inpdevices) inpdevices->GetDeviceHandlers(INPDEVICE_TYPE_MOUSE, devicesID);

  for(XDWORD c=0; c<devicesID.GetSize(); c++)
    {
      INPLINUXDEVICEID* deviceID = devicesID.Get(c);
      if(deviceID)
        {
          XSTRING event;
          int     filedescriptor = INPLINUXDEVICEID_INVALID;
     
          event.Format(_L("/dev/input/event%d"), deviceID->GetEventIndex());
                     
          XBUFFER charstr;
          
          event.ConvertToASCII(charstr);          
          filedescriptor = open(charstr.GetPtrChar(), O_RDONLY | O_NONBLOCK);
          
          if(filedescriptor != -1)
            {
              char    mouseOEMname[_MAXSTR];
              XSTRING namehardware;

              // we get the keyboard name
              memset(mouseOEMname, 0, _MAXSTR);
  
              ioctl(filedescriptor, EVIOCGNAME(_MAXSTR), mouseOEMname);
              namehardware.Set(mouseOEMname);
                      
              deviceID->SetFileDescriptor(filedescriptor);
              //deviceID->GetName()->Set(namehardware);

      
              // get max/min values on each axis
              int abs[6] = {0};
              ioctl(filedescriptor, EVIOCGABS(ABS_X), abs);
              minx = abs[1];
              maxx = abs[2];

              ioctl(filedescriptor, EVIOCGABS(ABS_Y), abs);
              miny = abs[1];
              maxy = abs[2];

              ioctl(filedescriptor, EVIOCGABS(ABS_Z), abs);
              minz = abs[1];
              maxz = abs[2];

              mice.Add(deviceID);
           }
        }
    }

  devicesID.DeleteAll();

  return mice.GetSize()?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPLINUXDEVICEMOUSEDIRECT::CloseAllDevicesID()
* @brief      Close all devices ID
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPLINUXDEVICEMOUSEDIRECT::CloseAllDevicesID()
{
  if(mice.IsEmpty()) return false;

  for(XDWORD c=0; c<mice.GetSize(); c++)
    {
      INPLINUXDEVICEID* deviceID = mice.Get(c);
      if(deviceID)
        {
          close(deviceID->GetFileDescriptor());
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPLINUXDEVICEMOUSEDIRECT::UpdateMouseData()
* @brief      Update mouse data
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPLINUXDEVICEMOUSEDIRECT::UpdateMouseData()
{
  if(!grpscreen) return false;

  for(int e=0;e<GetCursors()->GetSize();e++)
    {
      INPCURSOR* cursor = GetCursors()->Get(e);
      if(!cursor) continue;

      cursor->SetIsChanged(false);

      if((cursor->GetX() != mousex) || (cursor->GetY() != mousey))
        {
          cursor->Set(mousex, mousey);
          cursor->SetIsChanged(true);
        }
    }

  INPBUTTON* button = GetButton(INPBUTTON_ID_MOUSE_LEFT);
  if(button)
    {
      if(GetCursors()->Get(0)) GetCursors()->Get(0)->AddPointToMotion(button->IsPressed());
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void INPLINUXDEVICEMOUSEDIRECT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
void INPLINUXDEVICEMOUSEDIRECT::Clean()
{
  mousex    = 0;
  minx      = 0;
  maxx      = 0;

  mousey    = 0;
  miny      = 0;
  maxy      = 0;

  mousez    = 0;
  minz      = 0;
  maxz      = 0;
}









