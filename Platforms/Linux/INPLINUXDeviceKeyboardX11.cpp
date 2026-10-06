/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       INPLINUXDeviceKeyboardX11.cpp
* 
* @class      INPLINUXDEVICEKEYBOARDX11
* @brief      LINUX X11 input device keyboard class
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

#include "INPLINUXDeviceKeyboardX11.h"

#include <unistd.h>
#include <locale.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>
#include <X11/keysym.h>
#include <X11/keysymdef.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "XFactory.h"
#include "XFileTXT.h"

#include "GRPLINUXScreenX11.h"

#include "INPButton.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPLINUXDEVICEKEYBOARDX11::INPLINUXDEVICEKEYBOARDX11(): INPDEVICE()
* @brief      Constructor of class
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPLINUXDEVICEKEYBOARDX11::INPLINUXDEVICEKEYBOARDX11(): INPDEVICE()
{
  Clean();

  created = true;

  SetType(INPDEVICE_TYPE_KEYBOARD);

  SetEnabled(CreateAllButtons());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPLINUXDEVICEKEYBOARDX11::~INPLINUXDEVICEKEYBOARDX11()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPLINUXDEVICEKEYBOARDX11::~INPLINUXDEVICEKEYBOARDX11()
{
  DeleteAllButtons();

  SetEnabled(false);
  created = false;

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPLINUXDEVICEKEYBOARDX11::Update()
* @brief      Update
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPLINUXDEVICEKEYBOARDX11::Update()
{
  if((!created)||(!enabled))     return false;

  if(!grpscreenx11)              return false;
  if(!grpscreenx11->HasFocus())  return false;

  XEvent event;

  for(int e=0; e<this->buttons.GetSize(); e++)
    {
      INPBUTTON* button = buttons.FastGet(e);

      if(button->GetState() == INPBUTTON_STATE_RELEASED) button->SetState(INPBUTTON_STATE_UP);
      if(button->GetState() == INPBUTTON_STATE_PRESSED)  button->SetState(INPBUTTON_STATE_HOLD);
    }

  while(XCheckWindowEvent(grpscreenx11->GetDisplay(), (*grpscreenx11->GetWindow()), KeyPressMask | KeyReleaseMask, &event))
    {
      XKeyEvent* keyevent = (XKeyEvent *)&event;

      switch(event.type)
        {
          case KeyPress   : { long       keysym   = XLookupKeysym (keyevent, 0);
                              INPBUTTON* button   = GetButtonByCode(keysym);

                              if(!button)
                                {
                                  KeySym low    = 0;
                                  KeySym upper  = 0;

                                  XConvertCase(keysym, &low, &upper);

                                  button        = GetButtonByCode(upper);
                                }

                              if(!button) break;

                              if(button->GetState() == INPBUTTON_STATE_UP) button->SetState(INPBUTTON_STATE_PRESSED);

                              button->SetPressed(true);
                            }
                            break;

          case KeyRelease : { long       keysym   = XLookupKeysym (keyevent, 0);
                              INPBUTTON* button   = GetButtonByCode(keysym);

                              if(!button)
                                {
                                  KeySym low    = 0;
                                  KeySym upper  = 0;

                                  XConvertCase(keysym, &low, &upper);

                                  button        = GetButtonByCode(upper);
                                }

                              if(!button) break;

                              button->SetState(INPBUTTON_STATE_RELEASED);

                              button->SetPressed(false);
                            }
                            break;
        }

    }
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPLINUXDEVICEKEYBOARDX11::SetScreen(void* screenhandle)
* @brief      Set screen
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  screenhandle : handle to screen
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPLINUXDEVICEKEYBOARDX11::SetScreen(void* screenhandle)
{
  grpscreenx11 = (GRPLINUXSCREENX11*)screenhandle;
  if(!grpscreenx11) return false;

  XSelectInput(grpscreenx11->GetDisplay(), (*grpscreenx11->GetWindow()), KeyPressMask | KeyReleaseMask);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void INPLINUXDEVICEKEYBOARDX11::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
void INPLINUXDEVICEKEYBOARDX11::Clean()
{
  grpscreenx11 = NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPLINUXDEVICEKEYBOARDX11::CreateAllButtons()
* @brief      Create all buttons
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPLINUXDEVICEKEYBOARDX11::CreateAllButtons()
{
  INPBUTTON::CreateButton(&buttons, XK_BackSpace, INPBUTTON_ID_BACK_SPACE, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XK_Tab, INPBUTTON_ID_TAB, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XK_Return, INPBUTTON_ID_RETURN, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XK_Escape, INPBUTTON_ID_ESCAPE, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XK_Caps_Lock, INPBUTTON_ID_CAPS_LOCK, _C('\x0'));

  INPBUTTON::CreateButton(&buttons, XK_Shift_R, INPBUTTON_ID_SHIFT_RIGHT, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XK_Shift_L, INPBUTTON_ID_SHIFT_LEFT, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XK_Control_R, INPBUTTON_ID_CONTROL_RIGHT, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XK_Control_L, INPBUTTON_ID_CONTROL_LEFT, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XK_Alt_R, INPBUTTON_ID_ALT_RIGHT, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XK_Alt_L, INPBUTTON_ID_ALT_LEFT, _C('\x0'));

  INPBUTTON::CreateButton(&buttons, XK_parenleft, INPBUTTON_ID_OPEN_BRANCH, _C('(' ));
  INPBUTTON::CreateButton(&buttons, XK_parenright, INPBUTTON_ID_CLOSE_BRANCH, _C(')' ));
  INPBUTTON::CreateButton(&buttons, XK_comma, INPBUTTON_ID_COMMA, _C(',' ));
  INPBUTTON::CreateButton(&buttons, XK_minus, INPBUTTON_ID_MINUS, _C('-' ));
  INPBUTTON::CreateButton(&buttons, XK_periodcentered, INPBUTTON_ID_POINT, _C('.' ));
  INPBUTTON::CreateButton(&buttons, XK_slash, INPBUTTON_ID_SLASH, _C('/' ));

//INPBUTTON::CreateButton( &buttons, OPEN_BRACKET       , INPBUTTON_ID_OPEN_BRACKET      , _C('['   ));
//INPBUTTON::CreateButton( &buttons, CLOSE_BRACKET      , INPBUTTON_ID_CLOSE_BRACKET     , _C(']'  ));

  INPBUTTON::CreateButton(&buttons, XK_space, INPBUTTON_ID_SPACE, _C(' ' ));
  INPBUTTON::CreateButton(&buttons, XK_Page_Up, INPBUTTON_ID_PAGE_UP, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XK_Page_Down, INPBUTTON_ID_PAGE_DOWN, _C('\x0'));


  INPBUTTON::CreateButton(&buttons, XK_End, INPBUTTON_ID_END, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XK_Home, INPBUTTON_ID_HOME, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XK_Left, INPBUTTON_ID_LEFT, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XK_Up, INPBUTTON_ID_UP, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XK_Right, INPBUTTON_ID_RIGHT, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XK_Down, INPBUTTON_ID_DOWN, _C('\x0'));

  INPBUTTON::CreateButton(&buttons, XK_Insert, INPBUTTON_ID_INSERT, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XK_Delete, INPBUTTON_ID_DELETE, _C('\x0'));

  INPBUTTON::CreateButton(&buttons, XK_0, INPBUTTON_ID_0, _C('0' ));
  INPBUTTON::CreateButton(&buttons, XK_1, INPBUTTON_ID_1, _C('1' ));
  INPBUTTON::CreateButton(&buttons, XK_2, INPBUTTON_ID_2, _C('2' ));
  INPBUTTON::CreateButton(&buttons, XK_3, INPBUTTON_ID_3, _C('3' ));
  INPBUTTON::CreateButton(&buttons, XK_4, INPBUTTON_ID_4, _C('4' ));
  INPBUTTON::CreateButton(&buttons, XK_5, INPBUTTON_ID_5, _C('5' ));
  INPBUTTON::CreateButton(&buttons, XK_6, INPBUTTON_ID_6, _C('6' ));
  INPBUTTON::CreateButton(&buttons, XK_7, INPBUTTON_ID_7, _C('7' ));
  INPBUTTON::CreateButton(&buttons, XK_8, INPBUTTON_ID_8, _C('8' ));
  INPBUTTON::CreateButton(&buttons, XK_9, INPBUTTON_ID_9, _C('9' ));

  INPBUTTON::CreateButton(&buttons, XK_A, INPBUTTON_ID_A, _C('A' ));
  INPBUTTON::CreateButton(&buttons, XK_B, INPBUTTON_ID_B, _C('B' ));
  INPBUTTON::CreateButton(&buttons, XK_C, INPBUTTON_ID_C, _C('C' ));
  INPBUTTON::CreateButton(&buttons, XK_D, INPBUTTON_ID_D, _C('D' ));
  INPBUTTON::CreateButton(&buttons, XK_E, INPBUTTON_ID_E, _C('E' ));
  INPBUTTON::CreateButton(&buttons, XK_F, INPBUTTON_ID_F, _C('F' ));
  INPBUTTON::CreateButton(&buttons, XK_G, INPBUTTON_ID_G, _C('G' ));
  INPBUTTON::CreateButton(&buttons, XK_H, INPBUTTON_ID_H, _C('H' ));
  INPBUTTON::CreateButton(&buttons, XK_I, INPBUTTON_ID_I, _C('I' ));
  INPBUTTON::CreateButton(&buttons, XK_J, INPBUTTON_ID_J, _C('J' ));
  INPBUTTON::CreateButton(&buttons, XK_K, INPBUTTON_ID_K, _C('K' ));
  INPBUTTON::CreateButton(&buttons, XK_L, INPBUTTON_ID_L, _C('L' ));
  INPBUTTON::CreateButton(&buttons, XK_M, INPBUTTON_ID_M, _C('M' ));
  INPBUTTON::CreateButton(&buttons, XK_N, INPBUTTON_ID_N, _C('N' ));
  INPBUTTON::CreateButton(&buttons, XK_O, INPBUTTON_ID_O, _C('O' ));
  INPBUTTON::CreateButton(&buttons, XK_P, INPBUTTON_ID_P, _C('P' ));
  INPBUTTON::CreateButton(&buttons, XK_Q, INPBUTTON_ID_Q, _C('Q' ));
  INPBUTTON::CreateButton(&buttons, XK_R, INPBUTTON_ID_R, _C('R' ));
  INPBUTTON::CreateButton(&buttons, XK_S, INPBUTTON_ID_S, _C('S' ));
  INPBUTTON::CreateButton(&buttons, XK_T, INPBUTTON_ID_T, _C('T' ));
  INPBUTTON::CreateButton(&buttons, XK_U, INPBUTTON_ID_U, _C('U' ));
  INPBUTTON::CreateButton(&buttons, XK_V, INPBUTTON_ID_V, _C('V' ));
  INPBUTTON::CreateButton(&buttons, XK_W, INPBUTTON_ID_W, _C('W' ));
  INPBUTTON::CreateButton(&buttons, XK_X, INPBUTTON_ID_X, _C('X' ));
  INPBUTTON::CreateButton(&buttons, XK_Y, INPBUTTON_ID_Y, _C('Y' ));
  INPBUTTON::CreateButton(&buttons, XK_Z, INPBUTTON_ID_Z, _C('Z' ));

  INPBUTTON::CreateButton(&buttons, XK_Num_Lock, INPBUTTON_ID_NUMLOCK, _C('\x0'));

  INPBUTTON::CreateButton(&buttons, XK_F1, INPBUTTON_ID_F1, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XK_F2, INPBUTTON_ID_F2, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XK_F3, INPBUTTON_ID_F3, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XK_F4, INPBUTTON_ID_F4, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XK_F5, INPBUTTON_ID_F5, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XK_F6, INPBUTTON_ID_F6, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XK_F7, INPBUTTON_ID_F7, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XK_F8, INPBUTTON_ID_F8, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XK_F9, INPBUTTON_ID_F9, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XK_F10, INPBUTTON_ID_F10, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XK_F11, INPBUTTON_ID_F11, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XK_F12, INPBUTTON_ID_F12, _C('\x0'));

  INPBUTTON::CreateButton(&buttons, XK_Print, INPBUTTON_ID_PRINTSCREEN, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XK_Scroll_Lock, INPBUTTON_ID_SCROLL_LOCK, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, XK_Pause, INPBUTTON_ID_PAUSE, _C('\x0'));

  return true;
}




#endif


