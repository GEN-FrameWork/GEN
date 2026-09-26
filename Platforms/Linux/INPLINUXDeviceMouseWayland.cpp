/**-------------------------------------------------------------------------------------------------------------------
*
* @file       INPLINUXDeviceMouseWayland.cpp
*
* @class      INPLINUXDEVICEMOUSEWAYLAND
* @brief      LINUX Input Wayland device mouse class
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



#ifdef LINUX_WAYLAND_ACTIVE


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "INPLINUXDeviceMouseWayland.h"

#include <unistd.h>
#include <linux/input.h>

#include "GRPLINUXScreenWayland.h"

#include "INPButton.h"
#include "INPCursor.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         INPLINUXDEVICEMOUSEWAYLAND::INPLINUXDEVICEMOUSEWAYLAND() : INPDEVICE()
* @brief      Constructor of class
* @ingroup    PLATFORM_LINUX
*
* --------------------------------------------------------------------------------------------------------------------*/
INPLINUXDEVICEMOUSEWAYLAND::INPLINUXDEVICEMOUSEWAYLAND() : INPDEVICE()
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
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         INPLINUXDEVICEMOUSEWAYLAND::~INPLINUXDEVICEMOUSEWAYLAND()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_LINUX
*
* --------------------------------------------------------------------------------------------------------------------*/
INPLINUXDEVICEMOUSEWAYLAND::~INPLINUXDEVICEMOUSEWAYLAND()
{
  SetEnabled(false);
  created = false;

  DeleteAllButtons();
  DeleteAllCursors();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool INPLINUXDEVICEMOUSEWAYLAND::Update()
* @brief      Update
* @ingroup    PLATFORM_LINUX
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool INPLINUXDEVICEMOUSEWAYLAND::Update()
{
  if(!created || !enabled)
    {
      return false;
    }

  if(!grpscreenwayland)
    {
      return false;
    }

  if(!grpscreenwayland->HasFocus())
    {
      return false;
    }

  // GRPLINUXSCREENWAYLAND owns the wl_pointer listener (see the rationale in
  // GRPLINUXScreenWayland.h / GRPLINUXSCREENWAYLAND_EVENTQUEUE_SIZE) and pumps the connection
  // itself; a second DispatchEvents() call here is a cheap no-op if this frame's screen Update()
  // already ran it, same as the keyboard device's Update().
  grpscreenwayland->DispatchEvents();

  int  mousex = -1;
  int  mousey = -1;

  bool haspointer = grpscreenwayland->GetPointerPosition(mousex, mousey);

  INPCURSOR* cursor = GetCursor(INPCURSOR_ID_MOUSE);
  if(cursor)
    {
      if(haspointer && (mousex>=0) && (mousey>=0) && (mousex < grpscreenwayland->GetWidth()) && (mousey < grpscreenwayland->GetHeight()))
        {
          cursor->SetIsChanged(false);

          if((cursor->GetX() != mousex) || (cursor->GetY() != mousey))
            {
              // wl_pointer::motion reports surface-local coordinates top-down, the same convention
              // XQueryPointer() uses on X11 -- so INPLINUXDEVICEMOUSEX11's Y-flip (into GEN's
              // bottom-up cursor space) applies here unchanged.
              mousey = (grpscreenwayland->GetHeight()-mousey);
              cursor->Set(mousex, mousey);
              cursor->SetIsChanged(true);
            }
        }
       else
        {
          mousex = -1;
          mousey = -1;

          cursor->Set(mousex, mousey);
          cursor->SetIsChanged(true);
        }

      INPBUTTON* button = GetButton(INPBUTTON_ID_MOUSE_LEFT);
      if(button) cursor->AddPointToMotion(button->IsPressed());
    }

  //-------------------------------------------------------------
  // Read Buttons

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
              if(button->GetState() == INPBUTTON_STATE_PRESSED)
                {
                  button->SetState(INPBUTTON_STATE_HOLD);
                }
            }
        }
    }

  XDWORD code     = 0;
  bool   pressed  = false;

  // PopButtonEvent() drains the FIFO GRPLINUXSCREENWAYLAND::Pointer_Button() fills -- the push
  // equivalent of XCheckWindowEvent(display, window, ButtonPressMask | ButtonReleaseMask, &event)
  // in INPLINUXDEVICEMOUSEX11::Update(). 'code' is the raw Linux evdev BTN_* value; see
  // CreateAllButtons() below for why that is exactly what this table is keyed on.
  while(grpscreenwayland->PopButtonEvent(code, pressed))
    {
      INPBUTTON* button = GetButtonByCode(code);
      if(button)
        {
          if(pressed)
            {
              switch (button->GetState())
                {
                  case INPBUTTON_STATE_UP       : button->SetState(INPBUTTON_STATE_PRESSED);  break;
                  case INPBUTTON_STATE_HOLD     : button->SetState(INPBUTTON_STATE_HOLD);      break;
                  case INPBUTTON_STATE_PRESSED  : button->SetState(INPBUTTON_STATE_HOLD);      break;
                  case INPBUTTON_STATE_RELEASED : button->SetState(INPBUTTON_STATE_PRESSED);   break;
                                      default   : break;
                }

              button->SetPressed(true);
            }
           else
            {
              switch (button->GetState())
                {
                  case INPBUTTON_STATE_UP       : button->SetState(INPBUTTON_STATE_UP);        break;
                  case INPBUTTON_STATE_HOLD     : button->SetState(INPBUTTON_STATE_RELEASED);   break;
                  case INPBUTTON_STATE_PRESSED  : button->SetState(INPBUTTON_STATE_RELEASED);   break;
                  case INPBUTTON_STATE_RELEASED : button->SetState(INPBUTTON_STATE_RELEASED);   break;
                                   default      : break;
                }

              button->SetPressed(false);
            }
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool INPLINUXDEVICEMOUSEWAYLAND::SetScreen(void* screenhandle)
* @brief      Set screen
* @ingroup    PLATFORM_LINUX
*
* @param[in]  screenhandle : handle to screen
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool INPLINUXDEVICEMOUSEWAYLAND::SetScreen(void* screenhandle)
{
  grpscreenwayland = (GRPLINUXSCREENWAYLAND*)screenhandle;
  if(!grpscreenwayland)
    {
      return false;
    }

  // No XSelectInput()/XGrabButton() equivalent needed: GRPLINUXSCREENWAYLAND already installed
  // the wl_pointer listener (and requested WL_SEAT_CAPABILITY_POINTER) as soon as the seat
  // announced that capability, in Seat_Capabilities(). Wayland also has no concept of grabbing
  // input from the whole display the way XGrabButton(..., root, ...) could -- a client only ever
  // receives pointer events for surfaces it owns and currently has focus on, which is exactly the
  // scoped behaviour INPLINUXDEVICEMOUSEX11::SetScreen() had to grab explicitly to get on X11.
  // This class only registers itself as a (non-owning) consumer of the resulting event queue.

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void INPLINUXDEVICEMOUSEWAYLAND::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* --------------------------------------------------------------------------------------------------------------------*/
void INPLINUXDEVICEMOUSEWAYLAND::Clean()
{
  grpscreenwayland  = NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool INPLINUXDEVICEMOUSEWAYLAND::CreateAllButtons()
* @brief      Create all buttons
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool INPLINUXDEVICEMOUSEWAYLAND::CreateAllButtons()
{
  // Keyed on Linux evdev BTN_* codes (from <linux/input.h>), not X11's Button1/2/3 index -- see
  // the Pointer_Button() comment in GRPLINUXScreenWayland.cpp. BTN_LEFT/BTN_RIGHT/BTN_MIDDLE are
  // the same physical buttons INPLINUXDEVICEMOUSEX11::CreateAllButtons() maps via Button1/3/2.
  INPBUTTON::CreateButton(&buttons, BTN_RIGHT, INPBUTTON_ID_MOUSE_RIGHT, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, BTN_LEFT, INPBUTTON_ID_MOUSE_LEFT, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, BTN_MIDDLE, INPBUTTON_ID_MOUSE_MIDDLE, __C('\x0'));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool INPLINUXDEVICEMOUSEWAYLAND::CreateAllCursors()
* @brief      Create all cursors
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool INPLINUXDEVICEMOUSEWAYLAND::CreateAllCursors()
{
  INPCURSOR* cursor;

  cursor = GEN_NEW INPCURSOR();
  if(!cursor) return false;

  cursor->SetID(INPCURSOR_ID_MOUSE);

  cursor->SetHavePreSelect(true);

  cursors.Add(cursor);

  return true;
}




#endif

