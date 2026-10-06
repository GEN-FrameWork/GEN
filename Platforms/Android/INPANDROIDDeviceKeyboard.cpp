/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       INPANDROIDDeviceKeyboard.cpp
* 
* @class      INPANDROIDDEVICEKEYBOARD
* @brief      ANDROID Input device keyboard class
* @ingroup    PLATFORM_ANDROID
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

#include "INPANDROIDDeviceKeyboard.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPANDROIDDEVICEKEYBOARD::INPANDROIDDEVICEKEYBOARD(): INPDEVICE()
* @brief      Constructor of class
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPANDROIDDEVICEKEYBOARD::INPANDROIDDEVICEKEYBOARD(): INPDEVICE()
{
  Clean();

  created = true;

  SetType(INPDEVICE_TYPE_KEYBOARD);

  SetEnabled(CreateAllButtons());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPANDROIDDEVICEKEYBOARD::~INPANDROIDDEVICEKEYBOARD()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPANDROIDDEVICEKEYBOARD::~INPANDROIDDEVICEKEYBOARD()
{
  DeleteAllButtons();

  SetEnabled(false);
  created = false;

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPANDROIDDEVICEKEYBOARD::Update()
* @brief      Update
* @ingroup    PLATFORM_ANDROID
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPANDROIDDEVICEKEYBOARD::Update()
{
  if((!created)||(!enabled)) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPANDROIDDEVICEKEYBOARD::SetScreen(void* screenpointer)
* @brief      Set screen
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  screenpointer : Screenpointer pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPANDROIDDEVICEKEYBOARD::SetScreen(void* screenpointer)
{
    return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void INPANDROIDDEVICEKEYBOARD::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
void INPANDROIDDEVICEKEYBOARD::Clean()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPANDROIDDEVICEKEYBOARD::CreateAllButtons()
* @brief      Create all buttons
* @ingroup    PLATFORM_ANDROID
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPANDROIDDEVICEKEYBOARD::CreateAllButtons()
{
  INPBUTTON::CreateButton(&buttons, KEYCODE_BACKSPACE, INPBUTTON_ID_BACK_SPACE, _C('\b'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_TAB, INPBUTTON_ID_TAB, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_ENTER, INPBUTTON_ID_RETURN, _C('\n'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_ESCAPE, INPBUTTON_ID_ESCAPE, _C('\x0'));
//INPBUTTON::CreateButton( &buttons, VK_CAPITAL                 , INPBUTTON_ID_CAPS_LOCK          , _C('\x0'));

  INPBUTTON::CreateButton(&buttons, KEYCODE_SHIFT_RIGHT, INPBUTTON_ID_SHIFT_RIGHT, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_SHIFT_LEFT, INPBUTTON_ID_SHIFT_LEFT, _C('\x0'));
//INPBUTTON::CreateButton( &buttons, VK_RCONTROL                , INPBUTTON_ID_CONTROL_RIGHT      , _C('\x0'));
//INPBUTTON::CreateButton( &buttons, VK_LCONTROL                , INPBUTTON_ID_CONTROL_LEFT       , _C('\x0'));
//INPBUTTON::CreateButton( &buttons, VK_RMENU                   , INPBUTTON_ID_ALT_RIGHT          , _C('\x0'));
//INPBUTTON::CreateButton( &buttons, VK_LMENU                   , INPBUTTON_ID_ALT_LEFT           , _C('\x0'));

//INPBUTTON::CreateButton( &buttons, OPEN_BRANCH                , INPBUTTON_ID_OPEN_BRANCH        , _C('('  ));
//INPBUTTON::CreateButton( &buttons, CLOSE_BRANCH               , INPBUTTON_ID_CLOSE_BRANCH       , _C(')'  ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_COMMA, INPBUTTON_ID_COMMA, _C(',' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_MINUS, INPBUTTON_ID_MINUS, _C('-' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_PERIOD, INPBUTTON_ID_POINT, _C('.' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_SLASH, INPBUTTON_ID_SLASH, _C('/' ));
//INPBUTTON::CreateButton( &buttons, KEYCODE_BACK_QUOTE         , INPBUTTON_ID_BACK_QUOTE         , _C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_LEFT_BRACKET, INPBUTTON_ID_OPEN_BRACKET, _C('[' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_RIGHT_BRACKET, INPBUTTON_ID_CLOSE_BRACKET, _C(']' ));
//INPBUTTON::CreateButton( &buttons, KEYCODE_QUOTE              , INPBUTTON_ID_QUOTE              , _C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_SPACE, INPBUTTON_ID_SPACE, _C(' ' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_PAGE_UP, INPBUTTON_ID_PAGE_UP, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_PAGE_DOWN, INPBUTTON_ID_PAGE_DOWN, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_MOVE_END, INPBUTTON_ID_END, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_MOVE_HOME, INPBUTTON_ID_HOME, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_DPAD_LEFT, INPBUTTON_ID_LEFT, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_DPAD_UP, INPBUTTON_ID_UP, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_DPAD_RIGHT, INPBUTTON_ID_RIGHT, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_DPAD_DOWN, INPBUTTON_ID_DOWN, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_INSERT, INPBUTTON_ID_INSERT, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_FORWARD_DEL, INPBUTTON_ID_DELETE, _C('\x0'));

  INPBUTTON::CreateButton(&buttons, KEYCODE_0, INPBUTTON_ID_0, _C('0' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_1, INPBUTTON_ID_1, _C('1' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_2, INPBUTTON_ID_2, _C('2' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_3, INPBUTTON_ID_3, _C('3' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_4, INPBUTTON_ID_4, _C('4' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_5, INPBUTTON_ID_5, _C('5' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_6, INPBUTTON_ID_6, _C('6' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_7, INPBUTTON_ID_7, _C('7' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_8, INPBUTTON_ID_8, _C('8' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_9, INPBUTTON_ID_9, _C('9' ));

  INPBUTTON::CreateButton(&buttons, KEYCODE_A, INPBUTTON_ID_A, _C('A' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_B, INPBUTTON_ID_B, _C('B' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_C, INPBUTTON_ID_C, _C('C' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_D, INPBUTTON_ID_D, _C('D' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_E, INPBUTTON_ID_E, _C('E' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_F, INPBUTTON_ID_F, _C('F' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_G, INPBUTTON_ID_G, _C('G' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_H, INPBUTTON_ID_H, _C('H' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_I, INPBUTTON_ID_I, _C('I' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_J, INPBUTTON_ID_J, _C('J' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_K, INPBUTTON_ID_K, _C('K' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_L, INPBUTTON_ID_L, _C('L' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_M, INPBUTTON_ID_M, _C('M' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_N, INPBUTTON_ID_N, _C('N' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_O, INPBUTTON_ID_O, _C('O' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_P, INPBUTTON_ID_P, _C('P' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_Q, INPBUTTON_ID_Q, _C('Q' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_R, INPBUTTON_ID_R, _C('R' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_S, INPBUTTON_ID_S, _C('S' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_T, INPBUTTON_ID_T, _C('T' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_U, INPBUTTON_ID_U, _C('U' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_V, INPBUTTON_ID_V, _C('V' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_W, INPBUTTON_ID_W, _C('W' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_X, INPBUTTON_ID_X, _C('X' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_Y, INPBUTTON_ID_Y, _C('Y' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_Z, INPBUTTON_ID_Z, _C('Z' ));

  INPBUTTON::CreateButton(&buttons, KEYCODE_NUM_LOCK, INPBUTTON_ID_NUMLOCK, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_NUMPAD_0, INPBUTTON_ID_NUMPAD0, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_NUMPAD_1, INPBUTTON_ID_NUMPAD1, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_NUMPAD_2, INPBUTTON_ID_NUMPAD2, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_NUMPAD_3, INPBUTTON_ID_NUMPAD3, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_NUMPAD_4, INPBUTTON_ID_NUMPAD4, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_NUMPAD_5, INPBUTTON_ID_NUMPAD5, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_NUMPAD_6, INPBUTTON_ID_NUMPAD6, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_NUMPAD_7, INPBUTTON_ID_NUMPAD7, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_NUMPAD_8, INPBUTTON_ID_NUMPAD8, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_NUMPAD_9, INPBUTTON_ID_NUMPAD9, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_NUMPAD_MULTIPLY, INPBUTTON_ID_MULTIPLY, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_NUMPAD_ADD, INPBUTTON_ID_ADD, _C('+' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_NUMPAD_SUBTRACT, INPBUTTON_ID_SUBTRACT, _C('-' ));
//INPBUTTON::CreateButton( &buttons, VK_DECIMAL                 , INPBUTTON_ID_DECIMAL            , _C(','  ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_NUMPAD_DIVIDE, INPBUTTON_ID_DIVIDE, _C('\\' ));

  INPBUTTON::CreateButton(&buttons, KEYCODE_F1, INPBUTTON_ID_F1, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_F2, INPBUTTON_ID_F2, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_F3, INPBUTTON_ID_F3, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_F4, INPBUTTON_ID_F4, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_F5, INPBUTTON_ID_F5, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_F6, INPBUTTON_ID_F6, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_F7, INPBUTTON_ID_F7, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_F8, INPBUTTON_ID_F8, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_F9, INPBUTTON_ID_F9, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_F10, INPBUTTON_ID_F10, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_F11, INPBUTTON_ID_F11, _C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_F12, INPBUTTON_ID_F12, _C('\x0'));

//INPBUTTON::CreateButton( &buttons, VK_SNAPSHOT                , INPBUTTON_ID_PRINTSCREEN        , _C('\x0'));
//INPBUTTON::CreateButton( &buttons, VK_SCROLL                  , INPBUTTON_ID_SCROLL_LOCK        , _C('\x0'));
//INPBUTTON::CreateButton( &buttons, VK_PAUSE                   , INPBUTTON_ID_PAUSE              , _C('\x0'));

  return true;
}



