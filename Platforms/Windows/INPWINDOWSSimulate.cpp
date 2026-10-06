/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       INPWindowsSimulate.cpp
* 
* @class      INPWINDOWSSIMULATE
* @brief      WINDOWS Input Simulate class
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

#include "INPWINDOWSSimulate.h"

#include <Windows.h>

#include "XString.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/




/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPWINDOWSSIMULATE::INPWINDOWSSIMULATE()
* @brief      Constructor of class
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPWINDOWSSIMULATE::INPWINDOWSSIMULATE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPWINDOWSSIMULATE::~INPWINDOWSSIMULATE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPWINDOWSSIMULATE::~INPWINDOWSSIMULATE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPWINDOWSSIMULATE::Key_Press(XBYTE code)
* @brief      Key press
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  code : Code value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPWINDOWSSIMULATE::Key_Press(XBYTE code)
{
  INPUT input;

  memset(&input, 0, sizeof(INPUT));
  input.type       = INPUT_KEYBOARD;
  input.ki.wVk     = code;
  input.ki.wScan   = (WORD)MapVirtualKey((UINT)code, MAPVK_VK_TO_VSC);
  input.ki.dwFlags = 0;

  return (SendInput(1, &input, sizeof(INPUT)) == 1);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPWINDOWSSIMULATE::Key_UnPress(XBYTE code)
* @brief      Key un press
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  code : Code value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPWINDOWSSIMULATE::Key_UnPress(XBYTE code)
{
  INPUT input;

  memset(&input, 0, sizeof(INPUT));
  input.type       = INPUT_KEYBOARD;
  input.ki.wVk     = code;
  input.ki.wScan   = (WORD)MapVirtualKey((UINT)code, MAPVK_VK_TO_VSC);
  input.ki.dwFlags = KEYEVENTF_KEYUP;

  return (SendInput(1, &input, sizeof(INPUT)) == 1);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPWINDOWSSIMULATE::Key_Click(XBYTE code, int pressuretime)
* @brief      Key click
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  code : Code value.
* @param[in]  pressuretime : Pressuretime value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPWINDOWSSIMULATE::Key_Click(XBYTE code, int pressuretime)
{
  Key_Press(code);    
    
  Sleep(pressuretime);
  
  Key_UnPress(code);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPWINDOWSSIMULATE::Key_PressByLiteral(XCHAR* literal)
* @brief      Key press by literal
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  literal : Literal pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPWINDOWSSIMULATE::Key_PressByLiteral(XCHAR* literal)
{
  ALTERNATIVE_KEY altkey  = ALTERNATIVE_KEY_NONE;
  XBYTE           code    = GetKDBCodeByLiteral(literal, altkey);

  if(code)
    {    
      switch(altkey)
        {
          case ALTERNATIVE_KEY_NONE   : break;
          case ALTERNATIVE_KEY_ALTGR  : Key_PressByLiteral(_L("Right ALT"));  break;
          case ALTERNATIVE_KEY_SHIFT  : Key_PressByLiteral(_L("SHIFT"));      break;
        }
      
      bool status = Key_Press(code);  
      return status;   
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPWINDOWSSIMULATE::Key_UnPressByLiteral(XCHAR* literal)
* @brief      Key un press by literal
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  literal : Literal pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPWINDOWSSIMULATE::Key_UnPressByLiteral(XCHAR* literal)
{
  ALTERNATIVE_KEY altkey  = ALTERNATIVE_KEY_NONE;
  XBYTE           code    = GetKDBCodeByLiteral(literal, altkey);

  if(code)
    {    
      bool status = Key_UnPress(code);  

      switch(altkey)
        {
          case ALTERNATIVE_KEY_NONE   : break;
          case ALTERNATIVE_KEY_ALTGR  : Key_UnPressByLiteral(_L("Right ALT"));  break;
          case ALTERNATIVE_KEY_SHIFT  : Key_UnPressByLiteral(_L("SHIFT"));      break;
        }
      
      return status;   
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPWINDOWSSIMULATE::Key_ClickByLiteral(XCHAR* literal, int pressuretime)
* @brief      Key click by literal
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  literal : Literal pointer to use.
* @param[in]  pressuretime : Pressuretime value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPWINDOWSSIMULATE::Key_ClickByLiteral(XCHAR* literal, int pressuretime)
{
  ALTERNATIVE_KEY altkey  = ALTERNATIVE_KEY_NONE;
  XBYTE           code    = GetKDBCodeByLiteral(literal, altkey);

  if(code)
    {    
      switch(altkey)
        {
          case ALTERNATIVE_KEY_NONE   : break;
          case ALTERNATIVE_KEY_ALTGR  : Key_PressByLiteral(_L("Right ALT"));  break;
          case ALTERNATIVE_KEY_SHIFT  : Key_PressByLiteral(_L("SHIFT"));      break;
        }

      bool status = Key_Click(code, pressuretime);  

      switch(altkey)
        {
          case ALTERNATIVE_KEY_NONE   : break;
          case ALTERNATIVE_KEY_ALTGR  : Key_UnPressByLiteral(_L("Right ALT"));  break;
          case ALTERNATIVE_KEY_SHIFT  : Key_UnPressByLiteral(_L("SHIFT"));      break;
        }

      return status;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPWINDOWSSIMULATE::Key_ClickByText(XCHAR* text, int pressuretimeinterval)
* @brief      Key click by text
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  text : Text to use.
* @param[in]  pressuretimeinterval : Pressuretimeinterval value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPWINDOWSSIMULATE::Key_ClickByText(XCHAR* text, int pressuretimeinterval)
{
  XSTRING _text;
  XSTRING literal;
  bool    status = true;

  if(!text)
    {
      return false;
    }

  _text = text;

  if(_text.IsEmpty())
    {
      return true;
    }

  for(XDWORD c=0; c<_text.GetSize(); c++)
    {
      bool handled = true;

      literal.Empty();
      literal.Add(_text.Get()[c]);

      switch(_text.Get()[c])
        {
          case _C('A')   : 
          case _C('B')   : 
          case _C('C')   : 
          case _C('D')   : 
          case _C('E')   : 
          case _C('F')   : 
          case _C('G')   : 
          case _C('H')   : 
          case _C('I')   : 
          case _C('J')   : 
          case _C('K')   : 
          case _C('L')   : 
          case _C('M')   : 
          case _C('N')   : 
          case _C('O')   : 
          case _C('P')   : 
          case _C('Q')   : 
          case _C('R')   : 
          case _C('S')   : 
          case _C('T')   : 
          case _C('U')   : 
          case _C('V')   : 
          case _C('W')   : 
          case _C('X')   : 
          case _C('Y')   : 
          case _C('Z')   : { bool changecapslock = false;      
                              
                              if(!IsCapsLockActive())
                                {
                                  if(!Key_ClickByLiteral(_L("CAPS LOCK"), pressuretimeinterval)) status = false;
                                  changecapslock = true;      
                                }

                              if(!Key_ClickByLiteral(literal.Get(), pressuretimeinterval)) status = false;

                              if(changecapslock)
                                {
                                  if(!Key_ClickByLiteral(_L("CAPS LOCK"), pressuretimeinterval)) status = false;
                                }    
                            }
                            break;

          case _C(' ')   : if(!Key_ClickByLiteral(_L("SPACEBAR"), pressuretimeinterval)) status = false;
                            break;

          case _C('a')   : 
          case _C('b')   : 
          case _C('c')   : 
          case _C('d')   : 
          case _C('e')   : 
          case _C('f')   : 
          case _C('g')   : 
          case _C('h')   : 
          case _C('i')   : 
          case _C('j')   : 
          case _C('k')   : 
          case _C('l')   : 
          case _C('m')   : 
          case _C('n')   : 
          case _C('o')   : 
          case _C('p')   : 
          case _C('q')   : 
          case _C('r')   : 
          case _C('s')   : 
          case _C('t')   : 
          case _C('u')   : 
          case _C('v')   : 
          case _C('w')   : 
          case _C('x')   : 
          case _C('y')   : 

          case _C('z')   : { bool changecapslock = false;      
                              
                              if(IsCapsLockActive())
                                {
                                  if(!Key_ClickByLiteral(_L("CAPS LOCK"), pressuretimeinterval)) status = false;
                                  changecapslock = true;      
                                }

                              if(!Key_ClickByLiteral(literal.Get(), pressuretimeinterval)) status = false;

                              if(changecapslock)
                                {
                                  if(!Key_ClickByLiteral(_L("CAPS LOCK"), pressuretimeinterval)) status = false;
                                }    
                            }
                            break;
                                                                                          
          case _C('1')   : 
          case _C('2')   : 
          case _C('3')   : 
          case _C('4')   : 
          case _C('5')   : 
          case _C('6')   : 
          case _C('7')   : 
          case _C('8')   : 
          case _C('9')   : 
          case _C('0')   : 

          case _C('!')   : 
          case _C('@')   : 
          case _C('#')   : 
          case _C('$')   : 
          case _C('%')   : 
          case _C('^')   : 
          case _C('&')   : 
          case _C('*')   : 
          case _C('(')   : 
          case _C(')')   : 
          case _C('_')   : 
          case _C('+')   : 
          case _C('-')   : 
          case _C('=')   : 
          case _C('[')   : 
          case _C(']')   : 
          case _C('{')   : 
          case _C('}')   : 
          case _C('|')   : 
          case _C(';')   : 
          case _C(':')   : 
          case _C('\'')  : 
          case _C(',')   : 
          case _C('.')   : 
          case _C('<')   :    
          case _C('?')   : 
          case _C('/')   : 
          case _C('\\')  : 
          case _C('"')   :       

          case 0xBF       : // _C('')   : 
          case 0xA1       : // _C('')   : 
          case 0xF1       : // _C('?')   : 
          case 0xD1       : //_C('_')    :    
          case 0xB7       : //_C('')    :
                            if(!Key_ClickByLiteral(literal.Get(), pressuretimeinterval)) status = false;
                            break;

          default         : handled = false;
                            break;
        }

      if(!handled)
        {
          status = false;
        }
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPWINDOWSSIMULATE::Mouse_SetPos(int x, int y)
* @brief      Mouse set pos
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  x : X coordinate.
* @param[in]  y : Y coordinate.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPWINDOWSSIMULATE::Mouse_SetPos(int x, int y)
{
  SetCursorPos(x, y);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPWINDOWSSIMULATE::Mouse_Click(int x, int y)
* @brief      Mouse click
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  x : X coordinate.
* @param[in]  y : Y coordinate.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPWINDOWSSIMULATE::Mouse_Click(int x, int y)
{
  // Prefer mouse_event over SendInput for UI hit-testing apps (e.g. GEN UI_System /
  // ANGLE): SendInput can report success while the target never sees a usable click.
  SetCursorPos(x, y);
  Sleep(5);
  mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
  Sleep(15);
  mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPWINDOWSSIMULATE::IsCapsLockActive()
* @brief      Is caps lock active
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPWINDOWSSIMULATE::IsCapsLockActive()
{
  short capslockstate = GetKeyState(VK_CAPITAL);

  bool iscapslockon = (capslockstate & 0x0001) != 0;

  return iscapslockon;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPWINDOWSSIMULATE::IsNumLockActive()
* @brief      Is num lock active
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPWINDOWSSIMULATE::IsNumLockActive()
{
  short numlockstate = GetKeyState(VK_NUMLOCK);

  bool isnumlockon = (numlockstate & 0x0001) != 0;

  return isnumlockon;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPWINDOWSSIMULATE::IsScrollLockActive()
* @brief      Is scroll lock active
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPWINDOWSSIMULATE::IsScrollLockActive()
{
  short scrolllockstate = GetKeyState(VK_SCROLL);

  bool isscrolllockon = (scrolllockstate & 0x0001) != 0;

  return isscrolllockon;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void INPWINDOWSSIMULATE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void INPWINDOWSSIMULATE::Clean()
{
  
}




