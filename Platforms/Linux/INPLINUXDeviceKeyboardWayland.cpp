/**-------------------------------------------------------------------------------------------------------------------
*
* @file       INPLINUXDeviceKeyboardWayland.cpp
*
* @class      INPLINUXDEVICEKEYBOARDWAYLAND
* @brief      LINUX Wayland input device keyboard class
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

#include "INPLINUXDeviceKeyboardWayland.h"

#include <xkbcommon/xkbcommon.h>
#include <xkbcommon/xkbcommon-keysyms.h>

#include "XFactory.h"

#include "GRPLINUXScreenWayland.h"

#include "INPButton.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         INPLINUXDEVICEKEYBOARDWAYLAND::INPLINUXDEVICEKEYBOARDWAYLAND(): INPDEVICE()
* @brief      Constructor of class
* @ingroup    PLATFORM_LINUX
*
* --------------------------------------------------------------------------------------------------------------------*/
INPLINUXDEVICEKEYBOARDWAYLAND::INPLINUXDEVICEKEYBOARDWAYLAND(): INPDEVICE()
{
  Clean();

  created = true;

  SetType(INPDEVICE_TYPE_KEYBOARD);

  SetEnabled(CreateAllButtons());
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         INPLINUXDEVICEKEYBOARDWAYLAND::~INPLINUXDEVICEKEYBOARDWAYLAND()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_LINUX
*
* --------------------------------------------------------------------------------------------------------------------*/
INPLINUXDEVICEKEYBOARDWAYLAND::~INPLINUXDEVICEKEYBOARDWAYLAND()
{
  DeleteAllButtons();

  SetEnabled(false);
  created = false;

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool INPLINUXDEVICEKEYBOARDWAYLAND::Update()
* @brief      Update
* @ingroup    PLATFORM_LINUX
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool INPLINUXDEVICEKEYBOARDWAYLAND::Update()
{
  if((!created)||(!enabled))         return false;

  if(!grpscreenwayland)              return false;
  if(!grpscreenwayland->HasFocus())  return false;

  // Unlike X11's XCheckWindowEvent() (polled here, by this class, directly against Display/Window),
  // GRPLINUXSCREENWAYLAND is the sole owner/installer of the wl_keyboard listener (a wl_proxy only
  // accepts one) and pumps the connection itself -- this class only drains the small FIFO that
  // listener fills. Calling DispatchEvents() again here is a harmless, cheap no-op if the screen's
  // own Update() already pumped this frame; see GRPLINUXSCREENWAYLAND_EVENTQUEUE_SIZE in
  // GRPLINUXScreenWayland.h for the full rationale.
  grpscreenwayland->DispatchEvents();

  for(int e=0; e<this->buttons.GetSize(); e++)
    {
      INPBUTTON* button = buttons.FastGet(e);

      if(button->GetState() == INPBUTTON_STATE_RELEASED) button->SetState(INPBUTTON_STATE_UP);
      if(button->GetState() == INPBUTTON_STATE_PRESSED)  button->SetState(INPBUTTON_STATE_HOLD);
    }

  XDWORD keysym   = 0;
  bool   pressed  = false;

  while(grpscreenwayland->PopKeyEvent(keysym, pressed))
    {
      INPBUTTON* button = GetButtonByCode(keysym);

      if(!button)
        {
          // xkb_state_key_get_one_sym() already resolves the modifier-correct keysym (unlike
          // XLookupKeysym(keyevent, 0), which always returns the unshifted/level-1 form and forced
          // GRPLINUXDEVICEKEYBOARDX11 to fall back to XConvertCase()). The one case CreateAllButtons()
          // below still needs help with is plain letters: buttons are registered under the uppercase
          // XKB_KEY_A..XKB_KEY_Z codes (to match INPLINUXDEVICEKEYBOARDX11's table exactly), but an
          // unshifted letter press resolves to the lowercase XKB_KEY_a..XKB_KEY_z keysym. Both keysym
          // spaces mirror plain ASCII for the Latin letter range, so the fold is a fixed offset.
          if((keysym >= XKB_KEY_a) && (keysym <= XKB_KEY_z))
            {
              button = GetButtonByCode(keysym - (XKB_KEY_a - XKB_KEY_A));
            }
        }

      if(!button) continue;

      if(pressed)
        {
          if(button->GetState() == INPBUTTON_STATE_UP) button->SetState(INPBUTTON_STATE_PRESSED);

          button->SetPressed(true);
        }
       else
        {
          button->SetState(INPBUTTON_STATE_RELEASED);

          button->SetPressed(false);
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool INPLINUXDEVICEKEYBOARDWAYLAND::SetScreen(void* screenhandle)
* @brief      Set screen
* @ingroup    PLATFORM_LINUX
*
* @param[in]  screenhandle : handle to screen
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool INPLINUXDEVICEKEYBOARDWAYLAND::SetScreen(void* screenhandle)
{
  grpscreenwayland = (GRPLINUXSCREENWAYLAND*)screenhandle;
  if(!grpscreenwayland) return false;

  // No equivalent of X11's XSelectInput() needed: GRPLINUXSCREENWAYLAND already installed the
  // wl_keyboard listener (and requested WL_SEAT_CAPABILITY_KEYBOARD) as soon as the seat announced
  // that capability, in Seat_Capabilities(). This class only registers itself as a (non-owning)
  // consumer of the resulting event queue.
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void INPLINUXDEVICEKEYBOARDWAYLAND::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* --------------------------------------------------------------------------------------------------------------------*/
void INPLINUXDEVICEKEYBOARDWAYLAND::Clean()
{
  grpscreenwayland = NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool INPLINUXDEVICEKEYBOARDWAYLAND::CreateAllButtons()
* @brief      Create all buttons
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool INPLINUXDEVICEKEYBOARDWAYLAND::CreateAllButtons()
{
  // Table kept identical (same INPBUTTON_ID_* codes, same key set, same order) to
  // INPLINUXDEVICEKEYBOARDX11::CreateAllButtons() -- only the keysym constant source changes,
  // from X11/keysymdef.h's XK_* to xkbcommon-keysyms.h's XKB_KEY_*, which are numerically
  // identical (xkbcommon-keysyms.h is generated from the same X keysym registry).

  INPBUTTON::CreateButton(&buttons, XKB_KEY_BackSpace, INPBUTTON_ID_BACK_SPACE, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_Tab, INPBUTTON_ID_TAB, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_Return, INPBUTTON_ID_RETURN, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_Escape, INPBUTTON_ID_ESCAPE, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_Caps_Lock, INPBUTTON_ID_CAPS_LOCK, _C('\x0'));

  INPBUTTON::CreateButton(&buttons, XKB_KEY_Shift_R, INPBUTTON_ID_SHIFT_RIGHT, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_Shift_L, INPBUTTON_ID_SHIFT_LEFT, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_Control_R, INPBUTTON_ID_CONTROL_RIGHT, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_Control_L, INPBUTTON_ID_CONTROL_LEFT, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_Alt_R, INPBUTTON_ID_ALT_RIGHT, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_Alt_L, INPBUTTON_ID_ALT_LEFT, _C('\x0'));

  INPBUTTON::CreateButton(&buttons, XKB_KEY_parenleft, INPBUTTON_ID_OPEN_BRANCH, _C('(' ));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_parenright, INPBUTTON_ID_CLOSE_BRANCH, _C(')' ));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_comma, INPBUTTON_ID_COMMA, _C(',' ));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_minus, INPBUTTON_ID_MINUS, _C('-' ));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_periodcentered, INPBUTTON_ID_POINT, _C('.' ));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_slash, INPBUTTON_ID_SLASH, _C('/' ));

//INPBUTTON::CreateButton( &buttons, OPEN_BRACKET            , INPBUTTON_ID_OPEN_BRACKET      , _C('['   ));
//INPBUTTON::CreateButton( &buttons, CLOSE_BRACKET           , INPBUTTON_ID_CLOSE_BRACKET     , _C(']'  ));

  INPBUTTON::CreateButton(&buttons, XKB_KEY_space, INPBUTTON_ID_SPACE, _C(' ' ));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_Page_Up, INPBUTTON_ID_PAGE_UP, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_Page_Down, INPBUTTON_ID_PAGE_DOWN, _C('\x0'));


  INPBUTTON::CreateButton(&buttons, XKB_KEY_End, INPBUTTON_ID_END, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_Home, INPBUTTON_ID_HOME, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_Left, INPBUTTON_ID_LEFT, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_Up, INPBUTTON_ID_UP, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_Right, INPBUTTON_ID_RIGHT, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_Down, INPBUTTON_ID_DOWN, _C('\x0'));

  INPBUTTON::CreateButton(&buttons, XKB_KEY_Insert, INPBUTTON_ID_INSERT, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_Delete, INPBUTTON_ID_DELETE, _C('\x0'));

  INPBUTTON::CreateButton(&buttons, XKB_KEY_0, INPBUTTON_ID_0, _C('0' ));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_1, INPBUTTON_ID_1, _C('1' ));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_2, INPBUTTON_ID_2, _C('2' ));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_3, INPBUTTON_ID_3, _C('3' ));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_4, INPBUTTON_ID_4, _C('4' ));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_5, INPBUTTON_ID_5, _C('5' ));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_6, INPBUTTON_ID_6, _C('6' ));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_7, INPBUTTON_ID_7, _C('7' ));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_8, INPBUTTON_ID_8, _C('8' ));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_9, INPBUTTON_ID_9, _C('9' ));

  INPBUTTON::CreateButton(&buttons, XKB_KEY_A, INPBUTTON_ID_A, _C('A' ));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_B, INPBUTTON_ID_B, _C('B' ));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_C, INPBUTTON_ID_C, _C('C' ));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_D, INPBUTTON_ID_D, _C('D' ));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_E, INPBUTTON_ID_E, _C('E' ));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_F, INPBUTTON_ID_F, _C('F' ));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_G, INPBUTTON_ID_G, _C('G' ));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_H, INPBUTTON_ID_H, _C('H' ));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_I, INPBUTTON_ID_I, _C('I' ));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_J, INPBUTTON_ID_J, _C('J' ));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_K, INPBUTTON_ID_K, _C('K' ));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_L, INPBUTTON_ID_L, _C('L' ));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_M, INPBUTTON_ID_M, _C('M' ));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_N, INPBUTTON_ID_N, _C('N' ));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_O, INPBUTTON_ID_O, _C('O' ));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_P, INPBUTTON_ID_P, _C('P' ));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_Q, INPBUTTON_ID_Q, _C('Q' ));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_R, INPBUTTON_ID_R, _C('R' ));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_S, INPBUTTON_ID_S, _C('S' ));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_T, INPBUTTON_ID_T, _C('T' ));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_U, INPBUTTON_ID_U, _C('U' ));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_V, INPBUTTON_ID_V, _C('V' ));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_W, INPBUTTON_ID_W, _C('W' ));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_X, INPBUTTON_ID_X, _C('X' ));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_Y, INPBUTTON_ID_Y, _C('Y' ));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_Z, INPBUTTON_ID_Z, _C('Z' ));

  INPBUTTON::CreateButton(&buttons, XKB_KEY_Num_Lock, INPBUTTON_ID_NUMLOCK, _C('\x0'));

  INPBUTTON::CreateButton(&buttons, XKB_KEY_F1, INPBUTTON_ID_F1, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_F2, INPBUTTON_ID_F2, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_F3, INPBUTTON_ID_F3, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_F4, INPBUTTON_ID_F4, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_F5, INPBUTTON_ID_F5, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_F6, INPBUTTON_ID_F6, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_F7, INPBUTTON_ID_F7, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_F8, INPBUTTON_ID_F8, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_F9, INPBUTTON_ID_F9, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_F10, INPBUTTON_ID_F10, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_F11, INPBUTTON_ID_F11, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_F12, INPBUTTON_ID_F12, _C('\x0'));

  INPBUTTON::CreateButton(&buttons, XKB_KEY_Print, INPBUTTON_ID_PRINTSCREEN, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_Scroll_Lock, INPBUTTON_ID_SCROLL_LOCK, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XKB_KEY_Pause, INPBUTTON_ID_PAUSE, _C('\x0'));

  return true;
}




#endif

