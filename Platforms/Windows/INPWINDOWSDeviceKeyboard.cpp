/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       INPWINDOWSDeviceKeyboard.cpp
* 
* @class      INPWINDOWSDEVICEKEYBOARD
* @brief      WINDOWS Input Device Keyboard class
* @ingroup    PLATFORM_WINDOWS
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

#include "INPWINDOWSDeviceKeyboard.h"

#include "GRPWINDOWSScreen.h"

#include "INPButton.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPWINDOWSDEVICEKEYBOARD::INPWINDOWSDEVICEKEYBOARD() : INPDEVICE()
* @brief      Constructor of class
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPWINDOWSDEVICEKEYBOARD::INPWINDOWSDEVICEKEYBOARD() : INPDEVICE()
{
  Clean();

  created = true;

  SetType(INPDEVICE_TYPE_KEYBOARD);

  SetEnabled(CreateAllButtons());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPWINDOWSDEVICEKEYBOARD::~INPWINDOWSDEVICEKEYBOARD()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPWINDOWSDEVICEKEYBOARD::~INPWINDOWSDEVICEKEYBOARD()
{
  DeleteAllButtons();

  SetEnabled(false);
  created = false;

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPWINDOWSDEVICEKEYBOARD::Update()
* @brief      Update
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPWINDOWSDEVICEKEYBOARD::Update()
{
  if((!created)||(!enabled)) return false;

  if(grpscreen)
    {
      if(grpscreen->GetHandle()!=GetForegroundWindow()) 
        {
          return false;
        }
    }

  for(int c=0;c<(int)buttons.GetSize();c++)
    {
      INPBUTTON* button = (INPBUTTON*)buttons.Get(c);

      if(button)
        {
          if(GetKeyState(button->GetKeyCode()) & 0x80)
            {
              button->SetPressed(true);
            }
           else
            {
              button->SetPressed(false);
            }
        }
    }

  int n=buttons.GetSize();
  for(int c=0;c<n;c++)
    {
      INPBUTTON* button = (INPBUTTON*)buttons.FastGet(c);
      if(button)
        {
          SHORT state = GetAsyncKeyState(button->GetKeyCode());
          if(state & 0x8000) //key down
            {
              if(state & 0x01) //changed
                {
                  if (button->GetState() != INPBUTTON_STATE_HOLD) button->SetState(INPBUTTON_STATE_PRESSED);

                } else button->SetState(INPBUTTON_STATE_HOLD);
            }
           else
            {
              if(button->GetState() == INPBUTTON_STATE_HOLD || button->GetState() == INPBUTTON_STATE_PRESSED)
                {
                  button->SetState(INPBUTTON_STATE_RELEASED);

                } else button->SetState(INPBUTTON_STATE_UP);
            }
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void INPWINDOWSDEVICEKEYBOARD::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void INPWINDOWSDEVICEKEYBOARD::Clean()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPWINDOWSDEVICEKEYBOARD::CreateAllButtons()
* @brief      Create all buttons
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPWINDOWSDEVICEKEYBOARD::CreateAllButtons()
{
  INPBUTTON::CreateButton(&buttons, VK_BACK, INPBUTTON_ID_BACK_SPACE, __C('\b'));
  INPBUTTON::CreateButton(&buttons, VK_TAB, INPBUTTON_ID_TAB, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, VK_RETURN, INPBUTTON_ID_RETURN, __C('\n'));
  INPBUTTON::CreateButton(&buttons, VK_ESCAPE, INPBUTTON_ID_ESCAPE, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, VK_CAPITAL, INPBUTTON_ID_CAPS_LOCK, __C('\x0'));

  INPBUTTON::CreateButton(&buttons, VK_RSHIFT, INPBUTTON_ID_SHIFT_RIGHT, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, VK_LSHIFT, INPBUTTON_ID_SHIFT_LEFT, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, VK_RCONTROL, INPBUTTON_ID_CONTROL_RIGHT, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, VK_LCONTROL, INPBUTTON_ID_CONTROL_LEFT, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, VK_RMENU, INPBUTTON_ID_ALT_RIGHT, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, VK_LMENU, INPBUTTON_ID_ALT_LEFT, __C('\x0'));

//INPBUTTON::CreateButton( &buttons, OPEN_BRANCH        , INPBUTTON_ID_OPEN_BRANCH        , __C('('  ));
//INPBUTTON::CreateButton( &buttons, CLOSE_BRANCH       , INPBUTTON_ID_CLOSE_BRANCH       , __C(')'  ));
  INPBUTTON::CreateButton(&buttons, VK_OEM_COMMA, INPBUTTON_ID_COMMA, __C(',' ));
  INPBUTTON::CreateButton(&buttons, VK_OEM_MINUS, INPBUTTON_ID_MINUS, __C('-' ));
  INPBUTTON::CreateButton(&buttons, VK_OEM_PERIOD, INPBUTTON_ID_POINT, __C('.' ));
//INPBUTTON::CreateButton( &buttons,  SLASH             , INPBUTTON_ID_SLASH              , __C('/'  ));
//INPBUTTON::CreateButton( &buttons, BACK_QUOTE         , INPBUTTON_ID_BACK_QUOTE         , __C('\x0'));
//INPBUTTON::CreateButton( &buttons, OPEN_BRACKET       , INPBUTTON_ID_OPEN_BRACKET       , __C('['  ));
//INPBUTTON::CreateButton( &buttons, CLOSE_BRACKET      , INPBUTTON_ID_CLOSE_BRACKET      , __C(']'  ));
//INPBUTTON::CreateButton( &buttons, QUOTE              , INPBUTTON_ID_QUOTE              , __C('\x0'));
  INPBUTTON::CreateButton(&buttons, VK_SPACE, INPBUTTON_ID_SPACE, __C(' ' ));
  INPBUTTON::CreateButton(&buttons, VK_PRIOR, INPBUTTON_ID_PAGE_UP, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, VK_NEXT, INPBUTTON_ID_PAGE_DOWN, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, VK_END, INPBUTTON_ID_END, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, VK_HOME, INPBUTTON_ID_HOME, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, VK_LEFT, INPBUTTON_ID_LEFT, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, VK_UP, INPBUTTON_ID_UP, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, VK_RIGHT, INPBUTTON_ID_RIGHT, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, VK_DOWN, INPBUTTON_ID_DOWN, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, VK_INSERT, INPBUTTON_ID_INSERT, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, VK_DELETE, INPBUTTON_ID_DELETE, __C('\x0'));

  INPBUTTON::CreateButton(&buttons, 0x30, INPBUTTON_ID_0, __C('0' ));
  INPBUTTON::CreateButton(&buttons, 0x31, INPBUTTON_ID_1, __C('1' ));
  INPBUTTON::CreateButton(&buttons, 0x32, INPBUTTON_ID_2, __C('2' ));
  INPBUTTON::CreateButton(&buttons, 0x33, INPBUTTON_ID_3, __C('3' ));
  INPBUTTON::CreateButton(&buttons, 0x34, INPBUTTON_ID_4, __C('4' ));
  INPBUTTON::CreateButton(&buttons, 0x35, INPBUTTON_ID_5, __C('5' ));
  INPBUTTON::CreateButton(&buttons, 0x36, INPBUTTON_ID_6, __C('6' ));
  INPBUTTON::CreateButton(&buttons, 0x37, INPBUTTON_ID_7, __C('7' ));
  INPBUTTON::CreateButton(&buttons, 0x38, INPBUTTON_ID_8, __C('8' ));
  INPBUTTON::CreateButton(&buttons, 0x39, INPBUTTON_ID_9, __C('9' ));

  INPBUTTON::CreateButton(&buttons, 0x41, INPBUTTON_ID_A, __C('A' ));
  INPBUTTON::CreateButton(&buttons, 0x42, INPBUTTON_ID_B, __C('B' ));
  INPBUTTON::CreateButton(&buttons, 0x43, INPBUTTON_ID_C, __C('C' ));
  INPBUTTON::CreateButton(&buttons, 0x44, INPBUTTON_ID_D, __C('D' ));
  INPBUTTON::CreateButton(&buttons, 0x45, INPBUTTON_ID_E, __C('E' ));
  INPBUTTON::CreateButton(&buttons, 0x46, INPBUTTON_ID_F, __C('F' ));
  INPBUTTON::CreateButton(&buttons, 0x47, INPBUTTON_ID_G, __C('G' ));
  INPBUTTON::CreateButton(&buttons, 0x48, INPBUTTON_ID_H, __C('H' ));
  INPBUTTON::CreateButton(&buttons, 0x49, INPBUTTON_ID_I, __C('I' ));
  INPBUTTON::CreateButton(&buttons, 0x4A, INPBUTTON_ID_J, __C('J' ));
  INPBUTTON::CreateButton(&buttons, 0x4B, INPBUTTON_ID_K, __C('K' ));
  INPBUTTON::CreateButton(&buttons, 0x4C, INPBUTTON_ID_L, __C('L' ));
  INPBUTTON::CreateButton(&buttons, 0x4D, INPBUTTON_ID_M, __C('M' ));
  INPBUTTON::CreateButton(&buttons, 0x4E, INPBUTTON_ID_N, __C('N' ));
  INPBUTTON::CreateButton(&buttons, 0x4F, INPBUTTON_ID_O, __C('O' ));
  INPBUTTON::CreateButton(&buttons, 0x50, INPBUTTON_ID_P, __C('P' ));
  INPBUTTON::CreateButton(&buttons, 0x51, INPBUTTON_ID_Q, __C('Q' ));
  INPBUTTON::CreateButton(&buttons, 0x52, INPBUTTON_ID_R, __C('R' ));
  INPBUTTON::CreateButton(&buttons, 0x53, INPBUTTON_ID_S, __C('S' ));
  INPBUTTON::CreateButton(&buttons, 0x54, INPBUTTON_ID_T, __C('T' ));
  INPBUTTON::CreateButton(&buttons, 0x55, INPBUTTON_ID_U, __C('U' ));
  INPBUTTON::CreateButton(&buttons, 0x56, INPBUTTON_ID_V, __C('V' ));
  INPBUTTON::CreateButton(&buttons, 0x57, INPBUTTON_ID_W, __C('W' ));
  INPBUTTON::CreateButton(&buttons, 0x58, INPBUTTON_ID_X, __C('X' ));
  INPBUTTON::CreateButton(&buttons, 0x59, INPBUTTON_ID_Y, __C('Y' ));
  INPBUTTON::CreateButton(&buttons, 0x5A, INPBUTTON_ID_Z, __C('Z' ));

  INPBUTTON::CreateButton(&buttons, VK_NUMLOCK, INPBUTTON_ID_NUMLOCK, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, VK_NUMPAD0, INPBUTTON_ID_NUMPAD0, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, VK_NUMPAD1, INPBUTTON_ID_NUMPAD1, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, VK_NUMPAD2, INPBUTTON_ID_NUMPAD2, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, VK_NUMPAD3, INPBUTTON_ID_NUMPAD3, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, VK_NUMPAD4, INPBUTTON_ID_NUMPAD4, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, VK_NUMPAD5, INPBUTTON_ID_NUMPAD5, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, VK_NUMPAD6, INPBUTTON_ID_NUMPAD6, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, VK_NUMPAD7, INPBUTTON_ID_NUMPAD7, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, VK_NUMPAD8, INPBUTTON_ID_NUMPAD8, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, VK_NUMPAD9, INPBUTTON_ID_NUMPAD9, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, VK_MULTIPLY, INPBUTTON_ID_MULTIPLY, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, VK_ADD, INPBUTTON_ID_ADD, __C('+' ));
  INPBUTTON::CreateButton(&buttons, VK_SUBTRACT, INPBUTTON_ID_SUBTRACT, __C('-' ));
  INPBUTTON::CreateButton(&buttons, VK_DECIMAL, INPBUTTON_ID_DECIMAL, __C(',' ));
  INPBUTTON::CreateButton(&buttons, VK_DIVIDE, INPBUTTON_ID_DIVIDE, __C('\\' ));

  INPBUTTON::CreateButton(&buttons, VK_F1, INPBUTTON_ID_F1, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, VK_F2, INPBUTTON_ID_F2, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, VK_F3, INPBUTTON_ID_F3, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, VK_F4, INPBUTTON_ID_F4, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, VK_F5, INPBUTTON_ID_F5, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, VK_F6, INPBUTTON_ID_F6, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, VK_F7, INPBUTTON_ID_F7, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, VK_F8, INPBUTTON_ID_F8, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, VK_F9, INPBUTTON_ID_F9, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, VK_F10, INPBUTTON_ID_F10, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, VK_F11, INPBUTTON_ID_F11, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, VK_F12, INPBUTTON_ID_F12, __C('\x0'));

  INPBUTTON::CreateButton(&buttons, VK_SNAPSHOT, INPBUTTON_ID_PRINTSCREEN, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, VK_SCROLL, INPBUTTON_ID_SCROLL_LOCK, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, VK_PAUSE, INPBUTTON_ID_PAUSE, __C('\x0'));

  return true;
}



