/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       INPLINUXDeviceMouseX11.cpp
* 
* @class      INPLINUXDEVICEMOUSEX11
* @brief      LINUX Input X11 device mouse class
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



#ifdef LINUX_X11_ACTIVE


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "INPLINUXDeviceMouseX11.h"

#include <unistd.h>
#include <locale.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>

#include "GRPLINUXScreenX11.h"

#include "INPButton.h"
#include "INPCursor.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPLINUXDEVICEMOUSEX11::INPLINUXDEVICEMOUSEX11() : INPDEVICE()
* @brief      Constructor of class
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPLINUXDEVICEMOUSEX11::INPLINUXDEVICEMOUSEX11() : INPDEVICE()
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
* @fn         INPLINUXDEVICEMOUSEX11::~INPLINUXDEVICEMOUSEX11()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPLINUXDEVICEMOUSEX11::~INPLINUXDEVICEMOUSEX11()
{  
  // Window root = DefaultRootWindow(grpscreenx11->GetDisplay());
  // XUngrabButton(grpscreenx11->GetDisplay(), AnyButton, AnyModifier, root);
 
  SetEnabled(false);
  created = false;

  DeleteAllButtons();
  DeleteAllCursors();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPLINUXDEVICEMOUSEX11::Update()
* @brief      Update
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPLINUXDEVICEMOUSEX11::Update()
{
  if(!created || !enabled) 
    {
      return false;
    }

  if(!grpscreenx11)               
    {
      return false;
    }

  // Cursor position is always polled via XQueryPointer even without keyboard
  // focus so that the custom-chrome hit-test and auto-hide logic work on KDE
  // native X11, where the WM may not grant keyboard focus to a borderless
  // window until the user explicitly clicks it. XQueryPointer is a direct
  // server round-trip that does not depend on any event-mask or focus state.
  Display*     display = grpscreenx11->GetDisplay();
  Window       root;
  Window       child;
  int          rootx;
  int          rooty;
  int          mousex  = -1;
  int          mousey  = -1;
  unsigned int mask;

  XQueryPointer(display, (*grpscreenx11->GetWindow()), &root, &child, &rootx, &rooty, &mousex, &mousey, &mask);

  INPCURSOR* cursor = GetCursor(INPCURSOR_ID_MOUSE);
  if(cursor)
    {
      if((mousex>=0)&&(mousey>=0)&&(mousex < grpscreenx11->GetWidth()) && (mousey < grpscreenx11->GetHeight()))
        {
          cursor->SetIsChanged(false);

          // The cursor stores Y INVERTED (bottom-up); compare in that same space. The previous code compared
          // the stored (inverted) Y against the RAW (top-down) Y: two different coordinate spaces. That made
          // the change-test lie in both directions -- and in the worst case (raw y mirroring the stored y
          // around the window's vertical center, constantly crossed while dragging the window vertically
          // under a quiet cursor) the Y update was skipped with the cursor genuinely moved, freezing all
          // vertical caption-drag motion while horizontal kept working.
          int invmousey = (grpscreenx11->GetHeight() - mousey);

          if((cursor->GetX() != mousex) || (cursor->GetY() != invmousey))
            {
              cursor->Set(mousex, invmousey);
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

  // Button events are processed regardless of keyboard focus: XCheckWindowEvent below only ever dequeues
  // ButtonPress/ButtonRelease that landed on THIS window (the XGrabButton in SetScreen() is on this window,
  // not on root), so there is no risk of reacting to clicks made elsewhere. Gating them on HasFocus() broke
  // caption-dragging on native KDE: KWin does not always grant *keyboard* focus to a borderless
  // (custom-chromes) window, so with the gate in place the very clicks meant to drag the window were the
  // ones being thrown away. Keyboard input stays focus-gated in its own device (INPLINUXDEVICEKEYBOARDX11).

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
    
  XEvent event; 

  // XCheckWindowEvent only dequeues events matching this mask for this
  // window, leaving everything else (ConfigureNotify, ClientMessage, etc.)
  // untouched in the queue for its own handler. The previous XNextEvent call
  // dequeued whatever was next regardless of type, silently dropping it if it
  // wasn't a button event.
  while(XCheckWindowEvent(display, (*grpscreenx11->GetWindow()), ButtonPressMask | ButtonReleaseMask, &event))
    {
      switch(event.type)
        {
          case ButtonPress   : {  INPBUTTON* button = GetButtonByCode(event.xbutton.button);
                                  if(button)
                                    {
                                      switch (button->GetState())
                                        {
                                          case INPBUTTON_STATE_UP       : button->SetState(INPBUTTON_STATE_PRESSED);  break;
                                          case INPBUTTON_STATE_HOLD     : button->SetState(INPBUTTON_STATE_HOLD);     break;
                                          case INPBUTTON_STATE_PRESSED  : button->SetState(INPBUTTON_STATE_HOLD);     break;
                                          case INPBUTTON_STATE_RELEASED : button->SetState(INPBUTTON_STATE_PRESSED);  break;
                                                              default   : break; 
                                        }

                                      button->SetPressed(true);
                                    }
                                }
                                break;

          case ButtonRelease : {  INPBUTTON* button = GetButtonByCode(event.xbutton.button);
                                  if(button)
                                    {
                                      switch (button->GetState())
                                        {
                                          case INPBUTTON_STATE_UP       : button->SetState(INPBUTTON_STATE_UP);       break;
                                          case INPBUTTON_STATE_HOLD     : button->SetState(INPBUTTON_STATE_RELEASED); break;
                                          case INPBUTTON_STATE_PRESSED  : button->SetState(INPBUTTON_STATE_RELEASED); break;
                                          case INPBUTTON_STATE_RELEASED : button->SetState(INPBUTTON_STATE_RELEASED); break;
                                                           default      : break; 
                                        }

                                      button->SetPressed(false);
                                    }
                                }
                                break;

               default      : break; 
        }
    }


  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPLINUXDEVICEMOUSEX11::SetScreen(void* screenhandle)
* @brief      Set screen
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  screenhandle : handle to screen
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPLINUXDEVICEMOUSEX11::SetScreen(void* screenhandle)
{
  grpscreenx11 = (GRPLINUXSCREENX11*)screenhandle;
  if(!grpscreenx11) 
    {
      return false;
    }
    
  XSelectInput(grpscreenx11->GetDisplay(), (*grpscreenx11->GetWindow()), KeyPressMask | KeyReleaseMask);

  // Grab on our own window, not on the X11 root window. Grabbing on root
  // intercepts every button click on the whole display, including clicks on
  // the window manager's own decorations (minimize/maximize/close), so they
  // never reach the WM. Grabbing on our window only captures clicks that
  // land on our client area.
  XGrabButton(grpscreenx11->GetDisplay(), AnyButton, AnyModifier, (*grpscreenx11->GetWindow()), False, ButtonPressMask | ButtonReleaseMask, GrabModeAsync, GrabModeAsync, None, None);  
   
  XFlush(grpscreenx11->GetDisplay());

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void INPLINUXDEVICEMOUSEX11::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
void INPLINUXDEVICEMOUSEX11::Clean()
{ 
  grpscreenx11  = NULL;  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPLINUXDEVICEMOUSEX11::CreateAllButtons()
* @brief      Create all buttons
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPLINUXDEVICEMOUSEX11::CreateAllButtons()
{
  INPBUTTON::CreateButton(&buttons, Button3, INPBUTTON_ID_MOUSE_RIGHT, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, Button1, INPBUTTON_ID_MOUSE_LEFT, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, Button2, INPBUTTON_ID_MOUSE_MIDDLE, __C('\x0'));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPLINUXDEVICEMOUSEX11::CreateAllCursors()
* @brief      Create all cursors
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPLINUXDEVICEMOUSEX11::CreateAllCursors()
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

