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
  INPBUTTON::CreateButton(&buttons, KEYCODE_BACKSPACE, INPBUTTON_ID_BACK_SPACE, __C('\b'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_TAB, INPBUTTON_ID_TAB, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_ENTER, INPBUTTON_ID_RETURN, __C('\n'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_ESCAPE, INPBUTTON_ID_ESCAPE, __C('\x0'));
//INPBUTTON::CreateButton( &buttons, VK_CAPITAL                 , INPBUTTON_ID_CAPS_LOCK          , __C('\x0'));

  INPBUTTON::CreateButton(&buttons, KEYCODE_SHIFT_RIGHT, INPBUTTON_ID_SHIFT_RIGHT, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_SHIFT_LEFT, INPBUTTON_ID_SHIFT_LEFT, __C('\x0'));
//INPBUTTON::CreateButton( &buttons, VK_RCONTROL                , INPBUTTON_ID_CONTROL_RIGHT      , __C('\x0'));
//INPBUTTON::CreateButton( &buttons, VK_LCONTROL                , INPBUTTON_ID_CONTROL_LEFT       , __C('\x0'));
//INPBUTTON::CreateButton( &buttons, VK_RMENU                   , INPBUTTON_ID_ALT_RIGHT          , __C('\x0'));
//INPBUTTON::CreateButton( &buttons, VK_LMENU                   , INPBUTTON_ID_ALT_LEFT           , __C('\x0'));

//INPBUTTON::CreateButton( &buttons, OPEN_BRANCH                , INPBUTTON_ID_OPEN_BRANCH        , __C('('  ));
//INPBUTTON::CreateButton( &buttons, CLOSE_BRANCH               , INPBUTTON_ID_CLOSE_BRANCH       , __C(')'  ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_COMMA, INPBUTTON_ID_COMMA, __C(',' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_MINUS, INPBUTTON_ID_MINUS, __C('-' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_PERIOD, INPBUTTON_ID_POINT, __C('.' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_SLASH, INPBUTTON_ID_SLASH, __C('/' ));
//INPBUTTON::CreateButton( &buttons, KEYCODE_BACK_QUOTE         , INPBUTTON_ID_BACK_QUOTE         , __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_LEFT_BRACKET, INPBUTTON_ID_OPEN_BRACKET, __C('[' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_RIGHT_BRACKET, INPBUTTON_ID_CLOSE_BRACKET, __C(']' ));
//INPBUTTON::CreateButton( &buttons, KEYCODE_QUOTE              , INPBUTTON_ID_QUOTE              , __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_SPACE, INPBUTTON_ID_SPACE, __C(' ' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_PAGE_UP, INPBUTTON_ID_PAGE_UP, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_PAGE_DOWN, INPBUTTON_ID_PAGE_DOWN, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_MOVE_END, INPBUTTON_ID_END, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_MOVE_HOME, INPBUTTON_ID_HOME, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_DPAD_LEFT, INPBUTTON_ID_LEFT, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_DPAD_UP, INPBUTTON_ID_UP, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_DPAD_RIGHT, INPBUTTON_ID_RIGHT, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_DPAD_DOWN, INPBUTTON_ID_DOWN, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_INSERT, INPBUTTON_ID_INSERT, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_FORWARD_DEL, INPBUTTON_ID_DELETE, __C('\x0'));

  INPBUTTON::CreateButton(&buttons, KEYCODE_0, INPBUTTON_ID_0, __C('0' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_1, INPBUTTON_ID_1, __C('1' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_2, INPBUTTON_ID_2, __C('2' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_3, INPBUTTON_ID_3, __C('3' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_4, INPBUTTON_ID_4, __C('4' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_5, INPBUTTON_ID_5, __C('5' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_6, INPBUTTON_ID_6, __C('6' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_7, INPBUTTON_ID_7, __C('7' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_8, INPBUTTON_ID_8, __C('8' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_9, INPBUTTON_ID_9, __C('9' ));

  INPBUTTON::CreateButton(&buttons, KEYCODE_A, INPBUTTON_ID_A, __C('A' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_B, INPBUTTON_ID_B, __C('B' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_C, INPBUTTON_ID_C, __C('C' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_D, INPBUTTON_ID_D, __C('D' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_E, INPBUTTON_ID_E, __C('E' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_F, INPBUTTON_ID_F, __C('F' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_G, INPBUTTON_ID_G, __C('G' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_H, INPBUTTON_ID_H, __C('H' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_I, INPBUTTON_ID_I, __C('I' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_J, INPBUTTON_ID_J, __C('J' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_K, INPBUTTON_ID_K, __C('K' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_L, INPBUTTON_ID_L, __C('L' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_M, INPBUTTON_ID_M, __C('M' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_N, INPBUTTON_ID_N, __C('N' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_O, INPBUTTON_ID_O, __C('O' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_P, INPBUTTON_ID_P, __C('P' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_Q, INPBUTTON_ID_Q, __C('Q' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_R, INPBUTTON_ID_R, __C('R' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_S, INPBUTTON_ID_S, __C('S' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_T, INPBUTTON_ID_T, __C('T' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_U, INPBUTTON_ID_U, __C('U' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_V, INPBUTTON_ID_V, __C('V' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_W, INPBUTTON_ID_W, __C('W' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_X, INPBUTTON_ID_X, __C('X' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_Y, INPBUTTON_ID_Y, __C('Y' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_Z, INPBUTTON_ID_Z, __C('Z' ));

  INPBUTTON::CreateButton(&buttons, KEYCODE_NUM_LOCK, INPBUTTON_ID_NUMLOCK, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_NUMPAD_0, INPBUTTON_ID_NUMPAD0, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_NUMPAD_1, INPBUTTON_ID_NUMPAD1, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_NUMPAD_2, INPBUTTON_ID_NUMPAD2, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_NUMPAD_3, INPBUTTON_ID_NUMPAD3, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_NUMPAD_4, INPBUTTON_ID_NUMPAD4, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_NUMPAD_5, INPBUTTON_ID_NUMPAD5, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_NUMPAD_6, INPBUTTON_ID_NUMPAD6, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_NUMPAD_7, INPBUTTON_ID_NUMPAD7, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_NUMPAD_8, INPBUTTON_ID_NUMPAD8, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_NUMPAD_9, INPBUTTON_ID_NUMPAD9, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_NUMPAD_MULTIPLY, INPBUTTON_ID_MULTIPLY, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_NUMPAD_ADD, INPBUTTON_ID_ADD, __C('+' ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_NUMPAD_SUBTRACT, INPBUTTON_ID_SUBTRACT, __C('-' ));
//INPBUTTON::CreateButton( &buttons, VK_DECIMAL                 , INPBUTTON_ID_DECIMAL            , __C(','  ));
  INPBUTTON::CreateButton(&buttons, KEYCODE_NUMPAD_DIVIDE, INPBUTTON_ID_DIVIDE, __C('\\' ));

  INPBUTTON::CreateButton(&buttons, KEYCODE_F1, INPBUTTON_ID_F1, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_F2, INPBUTTON_ID_F2, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_F3, INPBUTTON_ID_F3, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_F4, INPBUTTON_ID_F4, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_F5, INPBUTTON_ID_F5, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_F6, INPBUTTON_ID_F6, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_F7, INPBUTTON_ID_F7, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_F8, INPBUTTON_ID_F8, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_F9, INPBUTTON_ID_F9, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_F10, INPBUTTON_ID_F10, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_F11, INPBUTTON_ID_F11, __C('\x0'));
  INPBUTTON::CreateButton(&buttons, KEYCODE_F12, INPBUTTON_ID_F12, __C('\x0'));

//INPBUTTON::CreateButton( &buttons, VK_SNAPSHOT                , INPBUTTON_ID_PRINTSCREEN        , __C('\x0'));
//INPBUTTON::CreateButton( &buttons, VK_SCROLL                  , INPBUTTON_ID_SCROLL_LOCK        , __C('\x0'));
//INPBUTTON::CreateButton( &buttons, VK_PAUSE                   , INPBUTTON_ID_PAUSE              , __C('\x0'));

  return true;
}



