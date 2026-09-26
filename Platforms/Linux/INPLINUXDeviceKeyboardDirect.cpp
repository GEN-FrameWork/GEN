/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       INPLINUXDeviceKeyboardDirect.cpp
* 
* @class      INPLINUXDEVICEKEYBOARDDIRECT
* @brief      LINUX direct input device keyboard class
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

#include "INPLINUXDeviceKeyboardDirect.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <locale.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/kd.h>
#include <linux/input.h>
#include <cerrno>

#include "XFactory.h"
#include "XBase.h"
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
* @fn         INPLINUXDEVICEKEYBOARDDIRECT::INPLINUXDEVICEKEYBOARDDIRECT(): INPDEVICE()
* @brief      Constructor of class
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPLINUXDEVICEKEYBOARDDIRECT::INPLINUXDEVICEKEYBOARDDIRECT(): INPDEVICE()
{
  Clean();

  created = true;

  SetType(INPDEVICE_TYPE_KEYBOARD);

  if(!CreateDevices())
    {
      Old_CreateDevices();
    }

  SetEnabled(CreateAllButtons());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPLINUXDEVICEKEYBOARDDIRECT::~INPLINUXDEVICEKEYBOARDDIRECT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPLINUXDEVICEKEYBOARDDIRECT::~INPLINUXDEVICEKEYBOARDDIRECT()
{
  Old_DeleteTerminalConfig();

  keyboards.DeleteContents();
  keyboards.DeleteAll();

  DeleteAllButtons();

  SetEnabled(false);

  created = false;

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPLINUXDEVICEKEYBOARDDIRECT::Update()
* @brief      Update
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPLINUXDEVICEKEYBOARDDIRECT::Update()
{
  if((!created)||(!enabled)) return false;

  int bytesread = 0;

  if(old_fd != -1)
    {
      XBYTE buffer[INPLINUXDEVICEKEYBOARDDIRECT_OLDMAXBUFFER];

      for(XDWORD e=0; e<keyboards.GetSize(); e++)
        {
          errno = 0;
          bytesread = read(keyboards.Get(e)->GetFileDescriptor(), buffer, sizeof(buffer));
          if(bytesread == -1 && errno == ENODEV)
            {
               // we lost the handle to the event file, need to reclaim it
               keyboards.DeleteContents();
               Old_CreateDevices();
            }

          if(bytesread)
            {
              for(int c=0; c<bytesread; c++)
                {
                  INPBUTTON* button = GetButtonByCode((buffer[c] & 0x7f));
                  if(button)
                    {
                      button->SetPressed((buffer[c] & 0x80) != 0x80);

                      if(button->IsPressed())
                        {
                          if(button->GetState() == INPBUTTON_STATE_PRESSED)
                            {
                              button->SetState(INPBUTTON_STATE_HOLD);
                            }
                           else
                            {
                              button->SetState(INPBUTTON_STATE_PRESSED);
                            }
                        }
                       else
                        {
                          button->SetState(INPBUTTON_STATE_RELEASED);
                        }

                      //XTRACE_PRINTCOLOR(1,__L("Keypres : code (%d) on : %s "), (buffer[c] & 0x7f), (button->IsPressed()?__L("Press"):__L("Release")));
                    }
                }
            }
        }
    }
   else
    {
      int                   eventsize = sizeof(struct input_event);
      struct  input_event   event;

      for(XDWORD e=0; e<keyboards.GetSize(); e++)
        {
          errno = 0;
          do{ bytesread = read(keyboards.Get(e)->GetFileDescriptor(), &event, eventsize);
              if(bytesread == -1 && errno == ENODEV)
                {
                  // we lost the handle to the event file, need to reclaim it
                  keyboards.DeleteContents();
                  CreateDevices();
                }

              if(bytesread == eventsize)
                {
                  if(EV_KEY == event.type)
                    {
                      INPBUTTON* button = GetButtonByCode(event.code);
                      if(button)
                        {
                          button->SetPressed(event.value!=0);
                          //XTRACE_PRINTCOLOR(1,__L("Keypres : code (%d) on : %s "), event.code, (button->IsPressed()?__L("Press"):__L("Release")));

                        } else continue;

                      switch(event.value)
                        {
                          case 0: button->SetState(INPBUTTON_STATE_RELEASED);
                                  break;

                          case 1: if(button->GetState() == INPBUTTON_STATE_PRESSED)
                                    {
                                      button->SetState(INPBUTTON_STATE_HOLD);
                                    }
                                   else
                                    {
                                      button->SetState(INPBUTTON_STATE_PRESSED);
                                    }
                                  break;

                          case 2: button->SetState(INPBUTTON_STATE_HOLD);
                                  break;
                        }
                    }
                }

            } while(bytesread == eventsize);
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void INPLINUXDEVICEKEYBOARDDIRECT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
void INPLINUXDEVICEKEYBOARDDIRECT::Clean()
{
  old_fd = -1;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPLINUXDEVICEKEYBOARDDIRECT::CreateAllButtons()
* @brief      Create all buttons
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPLINUXDEVICEKEYBOARDDIRECT::CreateAllButtons()
{
  INPBUTTON::CreateButton(&buttons, KEY_BACKSPACE, INPBUTTON_ID_BACK_SPACE, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEY_TAB, INPBUTTON_ID_TAB, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEY_ENTER, INPBUTTON_ID_RETURN, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEY_ESC, INPBUTTON_ID_ESCAPE, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEY_CAPSLOCK, INPBUTTON_ID_CAPS_LOCK, __C('\x0'));

  INPBUTTON::CreateButton(&buttons, KEY_RIGHTSHIFT, INPBUTTON_ID_SHIFT_RIGHT, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEY_LEFTSHIFT, INPBUTTON_ID_SHIFT_LEFT, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEY_RIGHTCTRL, INPBUTTON_ID_CONTROL_RIGHT, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEY_LEFTCTRL, INPBUTTON_ID_CONTROL_LEFT, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEY_RIGHTALT, INPBUTTON_ID_ALT_RIGHT, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEY_LEFTALT, INPBUTTON_ID_ALT_LEFT, __C('\x0'));

  INPBUTTON::CreateButton(&buttons, KEY_KPLEFTPAREN, INPBUTTON_ID_OPEN_BRANCH, __C('(' ));
  INPBUTTON::CreateButton(&buttons, KEY_KPRIGHTPAREN, INPBUTTON_ID_CLOSE_BRANCH, __C(')' ));
  INPBUTTON::CreateButton(&buttons, KEY_COMMA, INPBUTTON_ID_COMMA, __C(',' ));
  INPBUTTON::CreateButton(&buttons, KEY_MINUS, INPBUTTON_ID_MINUS, __C('-' ));
  INPBUTTON::CreateButton(&buttons, KEY_DOT, INPBUTTON_ID_POINT, __C('.' ));
  INPBUTTON::CreateButton(&buttons, KEY_SLASH, INPBUTTON_ID_SLASH, __C('/' ));

  INPBUTTON::CreateButton(&buttons, KEY_LEFTBRACE, INPBUTTON_ID_OPEN_BRACKET, __C('[' ));
  INPBUTTON::CreateButton(&buttons, KEY_RIGHTBRACE, INPBUTTON_ID_CLOSE_BRACKET, __C(']' ));

  INPBUTTON::CreateButton(&buttons, KEY_SPACE, INPBUTTON_ID_SPACE, __C(' ' ));
  INPBUTTON::CreateButton(&buttons, KEY_PAGEUP, INPBUTTON_ID_PAGE_UP, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEY_PAGEDOWN, INPBUTTON_ID_PAGE_DOWN, __C('\x0'));

  INPBUTTON::CreateButton(&buttons, KEY_END, INPBUTTON_ID_END, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEY_HOME, INPBUTTON_ID_HOME, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEY_LEFT, INPBUTTON_ID_LEFT, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEY_UP, INPBUTTON_ID_UP, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEY_RIGHT, INPBUTTON_ID_RIGHT, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEY_DOWN, INPBUTTON_ID_DOWN, __C('\x0'));

  INPBUTTON::CreateButton(&buttons, KEY_INSERT, INPBUTTON_ID_INSERT, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEY_DELETE, INPBUTTON_ID_DELETE, __C('\x0'));

  INPBUTTON::CreateButton(&buttons, KEY_A, INPBUTTON_ID_A, __C('A' ));
  INPBUTTON::CreateButton(&buttons, KEY_B, INPBUTTON_ID_B, __C('B' ));
  INPBUTTON::CreateButton(&buttons, KEY_C, INPBUTTON_ID_C, __C('C' ));
  INPBUTTON::CreateButton(&buttons, KEY_D, INPBUTTON_ID_D, __C('D' ));
  INPBUTTON::CreateButton(&buttons, KEY_E, INPBUTTON_ID_E, __C('E' ));
  INPBUTTON::CreateButton(&buttons, KEY_F, INPBUTTON_ID_F, __C('F' ));
  INPBUTTON::CreateButton(&buttons, KEY_G, INPBUTTON_ID_G, __C('G' ));
  INPBUTTON::CreateButton(&buttons, KEY_H, INPBUTTON_ID_H, __C('H' ));
  INPBUTTON::CreateButton(&buttons, KEY_I, INPBUTTON_ID_I, __C('I' ));
  INPBUTTON::CreateButton(&buttons, KEY_J, INPBUTTON_ID_J, __C('J' ));
  INPBUTTON::CreateButton(&buttons, KEY_K, INPBUTTON_ID_K, __C('K' ));
  INPBUTTON::CreateButton(&buttons, KEY_L, INPBUTTON_ID_L, __C('L' ));
  INPBUTTON::CreateButton(&buttons, KEY_M, INPBUTTON_ID_M, __C('M' ));
  INPBUTTON::CreateButton(&buttons, KEY_N, INPBUTTON_ID_N, __C('N' ));
  INPBUTTON::CreateButton(&buttons, KEY_O, INPBUTTON_ID_O, __C('O' ));
  INPBUTTON::CreateButton(&buttons, KEY_P, INPBUTTON_ID_P, __C('P' ));
  INPBUTTON::CreateButton(&buttons, KEY_Q, INPBUTTON_ID_Q, __C('Q' ));
  INPBUTTON::CreateButton(&buttons, KEY_R, INPBUTTON_ID_R, __C('R' ));
  INPBUTTON::CreateButton(&buttons, KEY_S, INPBUTTON_ID_S, __C('S' ));
  INPBUTTON::CreateButton(&buttons, KEY_T, INPBUTTON_ID_T, __C('T' ));
  INPBUTTON::CreateButton(&buttons, KEY_U, INPBUTTON_ID_U, __C('U' ));
  INPBUTTON::CreateButton(&buttons, KEY_V, INPBUTTON_ID_V, __C('V' ));
  INPBUTTON::CreateButton(&buttons, KEY_W, INPBUTTON_ID_W, __C('W' ));
  INPBUTTON::CreateButton(&buttons, KEY_X, INPBUTTON_ID_X, __C('X' ));
  INPBUTTON::CreateButton(&buttons, KEY_Y, INPBUTTON_ID_Y, __C('Y' ));
  INPBUTTON::CreateButton(&buttons, KEY_Z, INPBUTTON_ID_Z, __C('Z' ));

  INPBUTTON::CreateButton(&buttons, KEY_1, INPBUTTON_ID_1, __C('1' ));
  INPBUTTON::CreateButton(&buttons, KEY_2, INPBUTTON_ID_2, __C('2' ));
  INPBUTTON::CreateButton(&buttons, KEY_3, INPBUTTON_ID_3, __C('3' ));
  INPBUTTON::CreateButton(&buttons, KEY_4, INPBUTTON_ID_4, __C('4' ));
  INPBUTTON::CreateButton(&buttons, KEY_5, INPBUTTON_ID_5, __C('5' ));
  INPBUTTON::CreateButton(&buttons, KEY_6, INPBUTTON_ID_6, __C('6' ));
  INPBUTTON::CreateButton(&buttons, KEY_7, INPBUTTON_ID_7, __C('7' ));
  INPBUTTON::CreateButton(&buttons, KEY_8, INPBUTTON_ID_8, __C('8' ));
  INPBUTTON::CreateButton(&buttons, KEY_9, INPBUTTON_ID_9, __C('9' ));
  INPBUTTON::CreateButton(&buttons, KEY_0, INPBUTTON_ID_0, __C('0' ));

  INPBUTTON::CreateButton(&buttons, KEY_NUMLOCK, INPBUTTON_ID_NUMLOCK, __C('\x0'));

  INPBUTTON::CreateButton(&buttons, KEY_F1, INPBUTTON_ID_F1, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEY_F2, INPBUTTON_ID_F2, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEY_F3, INPBUTTON_ID_F3, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEY_F4, INPBUTTON_ID_F4, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEY_F5, INPBUTTON_ID_F5, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEY_F6, INPBUTTON_ID_F6, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEY_F7, INPBUTTON_ID_F7, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEY_F8, INPBUTTON_ID_F8, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEY_F9, INPBUTTON_ID_F9, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEY_F10, INPBUTTON_ID_F10, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEY_F11, INPBUTTON_ID_F11, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEY_F12, INPBUTTON_ID_F12, __C('\x0'));

  #ifdef KEY_SCREEN
  INPBUTTON::CreateButton(&buttons, KEY_SCREEN, INPBUTTON_ID_PRINTSCREEN, __C('\x0'));
  #endif

  INPBUTTON::CreateButton(&buttons, KEY_SCROLLLOCK, INPBUTTON_ID_SCROLL_LOCK, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEY_PAUSE, INPBUTTON_ID_PAUSE, __C('\x0'));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPLINUXDEVICEKEYBOARDDIRECT::CreateDevices()
* @brief      Create devices
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPLINUXDEVICEKEYBOARDDIRECT::CreateDevices()
{
  INPLINUXFACTORY*     inpdevices = (INPLINUXFACTORY*)(&INPFACTORY::GetInstance());
  XVECTOR<INPLINUXDEVICEID*>  devicesID;
      
  if(inpdevices) inpdevices->GetDeviceHandlers(INPDEVICE_TYPE_KEYBOARD, devicesID);

  for(XDWORD c=0; c<devicesID.GetSize(); c++)
    {
      INPLINUXDEVICEID* deviceID = devicesID.Get(c);
      if(deviceID)
        {
          XSTRING event;
          int     filedescriptor = INPLINUXDEVICEID_INVALID;
     
          event.Format(__L("/dev/input/event%d"), deviceID->GetEventIndex());

          XBUFFER charstr;
          
          event.ConvertToASCII(charstr); 
          filedescriptor = open(charstr.GetPtrChar(), O_RDONLY | O_NONBLOCK);
          
          if(filedescriptor != -1)
            {
              char    keyboardOEMname[_MAXSTR];
              XSTRING namehardware;

              // we get the keyboard name
              memset(keyboardOEMname, 0, _MAXSTR);
  
              ioctl(filedescriptor, EVIOCGNAME(_MAXSTR), keyboardOEMname);
              namehardware.Set(keyboardOEMname);
                      
              deviceID->SetFileDescriptor(filedescriptor);
              //deviceID->GetName()->Set(namehardware);

              keyboards.Add(deviceID);
           }
        }
    }

  devicesID.DeleteAll();
  
  return keyboards.GetSize()?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPLINUXDEVICEKEYBOARDDIRECT::Old_CreateDevices()
* @brief      Old create devices
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPLINUXDEVICEKEYBOARDDIRECT::Old_CreateDevices()
{
  struct termios newsettings;

  old_fd = Old_GetFD(NULL);

  if(ioctl(old_fd, KDGKBMODE, &old_keyboardmode))          return false;

  if(tcgetattr(old_fd, &old_termiosinitialsettings) == -1) return false;
  if(tcgetattr(old_fd, &newsettings) == -1)                return false;

  newsettings.c_lflag     &= ~(ICANON | ECHO | ISIG);
  newsettings.c_iflag      = 0;
  newsettings.c_cc[VMIN]   = INPLINUXDEVICEKEYBOARDDIRECT_OLDMAXBUFFER;
  newsettings.c_cc[VTIME]  = 1; /* 0.1 sec intercharacter timeout */

  if(tcsetattr(old_fd, TCSAFLUSH, &newsettings) == -1) return false;

  if(ioctl(old_fd, KDSKBMODE, K_MEDIUMRAW)) return false;

  if(old_fd != -1)
    {
      INPLINUXDEVICEID* deviceID = NULL;

      deviceID = GEN_NEW INPLINUXDEVICEID();
      if(!deviceID) return false;

      deviceID->SetFileDescriptor(old_fd);
      keyboards.Add(deviceID);

      //XTRACE_PRINTCOLOR((deviceID?1:4),__L("Keyboard handle %d"), old_fd);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPLINUXDEVICEKEYBOARDDIRECT::Old_DeleteTerminalConfig()
* @brief      Old GEN_DELETE terminal config
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPLINUXDEVICEKEYBOARDDIRECT::Old_DeleteTerminalConfig()
{
  if(old_fd == -1) return false;

  if(ioctl(old_fd, KDSKBMODE, old_keyboardmode))  return false;

  if(tcsetattr(old_fd, 0, &old_termiosinitialsettings) == -1) return false;

  close(old_fd);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int INPLINUXDEVICEKEYBOARDDIRECT::Old_IsConsole(int fd)
* @brief      Old is console
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  fd : Fd value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int INPLINUXDEVICEKEYBOARDDIRECT::Old_IsConsole(int fd)
{
  char arg;

  arg = 0;

  return (ioctl(fd, KDGKBTYPE, &arg) == 0 && ((arg == KB_101) || (arg == KB_84)));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int INPLINUXDEVICEKEYBOARDDIRECT::Old_OpenConsole(const char *fnam)
* @brief      Old open console
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  fnam : Fnam value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int INPLINUXDEVICEKEYBOARDDIRECT::Old_OpenConsole(const char *fnam)
{
  int fd;

  /*
   * For ioctl purposes we only need some fd and permissions
   * do not matter. But setfont:activatemap() does a write.
   */
  fd = open(fnam, O_RDWR | O_NONBLOCK);
  if(fd < 0 && errno == EACCES) fd = open(fnam, O_WRONLY | O_NONBLOCK);
    if (fd < 0 && errno == EACCES) fd = open(fnam, O_RDONLY | O_NONBLOCK);
      if (fd < 0) return -1;

  if(!Old_IsConsole(fd))
    {
      close(fd);
      return -1;
    }

  return fd;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int INPLINUXDEVICEKEYBOARDDIRECT::Old_GetFD(const char *fnam)
* @brief      Old get FD
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  fnam : Fnam value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int INPLINUXDEVICEKEYBOARDDIRECT::Old_GetFD(const char *fnam)
{
  int fd;

  if (fnam)
    {
      fd = Old_OpenConsole(fnam);
      if (fd >= 0) return fd;

      return -1;
    }

  fd = Old_OpenConsole("/dev/tty");
  if (fd >= 0) return fd;

  fd = Old_OpenConsole("/dev/tty0");
  if (fd >= 0) return fd;

  fd = Old_OpenConsole("/dev/vc/0");
  if (fd >= 0) return fd;

  fd = Old_OpenConsole("/dev/console");
  if(fd >= 0) return fd;
    {
      for(fd = 0; fd < 3; fd++)
        {
          if (Old_IsConsole(fd))  return fd;
        }
    }

  return -1;
}



