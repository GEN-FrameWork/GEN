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
  keybd_event(code, 0, 0, 0);    
    
  return true;
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
  keybd_event(code, 0, KEYEVENTF_KEYUP, 0);

  return true;
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
          case ALTERNATIVE_KEY_ALTGR  : Key_PressByLiteral(__L("Right ALT"));  break;
          case ALTERNATIVE_KEY_SHIFT  : Key_PressByLiteral(__L("SHIFT"));      break;
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
          case ALTERNATIVE_KEY_ALTGR  : Key_UnPressByLiteral(__L("Right ALT"));  break;
          case ALTERNATIVE_KEY_SHIFT  : Key_UnPressByLiteral(__L("SHIFT"));      break;
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
          case ALTERNATIVE_KEY_ALTGR  : Key_PressByLiteral(__L("Right ALT"));  break;
          case ALTERNATIVE_KEY_SHIFT  : Key_PressByLiteral(__L("SHIFT"));      break;
        }

      bool status = Key_Click(code, pressuretime);  

      switch(altkey)
        {
          case ALTERNATIVE_KEY_NONE   : break;
          case ALTERNATIVE_KEY_ALTGR  : Key_UnPressByLiteral(__L("Right ALT"));  break;
          case ALTERNATIVE_KEY_SHIFT  : Key_UnPressByLiteral(__L("SHIFT"));      break;
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

  _text = text;

  for(XDWORD c=0; c<_text.GetSize(); c++)
    {
      literal.Empty();
      literal.Add(_text.Get()[c]);

      switch(_text.Get()[c])
        {
          case __C('A')   : 
          case __C('B')   : 
          case __C('C')   : 
          case __C('D')   : 
          case __C('E')   : 
          case __C('F')   : 
          case __C('G')   : 
          case __C('H')   : 
          case __C('I')   : 
          case __C('J')   : 
          case __C('K')   : 
          case __C('L')   : 
          case __C('M')   : 
          case __C('N')   : 
          case __C('O')   : 
          case __C('P')   : 
          case __C('Q')   : 
          case __C('R')   : 
          case __C('S')   : 
          case __C('T')   : 
          case __C('U')   : 
          case __C('V')   : 
          case __C('W')   : 
          case __C('X')   : 
          case __C('Y')   : 
          case __C('Z')   : { bool changecapslock = false;      
                              
                              if(!IsCapsLockActive())
                                {
                                  Key_ClickByLiteral(__L("CAPS LOCK"), pressuretimeinterval);
                                  changecapslock = true;      
                                }

                              Key_ClickByLiteral(literal.Get(), pressuretimeinterval);

                              if(changecapslock)
                                {
                                  Key_ClickByLiteral(__L("CAPS LOCK"), pressuretimeinterval);                                  
                                }    
                            }
                            break;

          case __C(' ')   : Key_ClickByLiteral(__L("SPACEBAR"), pressuretimeinterval);                            
                            break;

          case __C('a')   : 
          case __C('b')   : 
          case __C('c')   : 
          case __C('d')   : 
          case __C('e')   : 
          case __C('f')   : 
          case __C('g')   : 
          case __C('h')   : 
          case __C('i')   : 
          case __C('j')   : 
          case __C('k')   : 
          case __C('l')   : 
          case __C('m')   : 
          case __C('n')   : 
          case __C('o')   : 
          case __C('p')   : 
          case __C('q')   : 
          case __C('r')   : 
          case __C('s')   : 
          case __C('t')   : 
          case __C('u')   : 
          case __C('v')   : 
          case __C('w')   : 
          case __C('x')   : 
          case __C('y')   : 

          case __C('z')   : { bool changecapslock = false;      
                              
                              if(IsCapsLockActive())
                                {
                                  Key_ClickByLiteral(__L("CAPS LOCK"), pressuretimeinterval);
                                  changecapslock = true;      
                                }

                              Key_ClickByLiteral(literal.Get(), pressuretimeinterval);

                              if(changecapslock)
                                {
                                  Key_ClickByLiteral(__L("CAPS LOCK"), pressuretimeinterval);                                  
                                }    
                            }
                            break;
                                                                                          
          case __C('1')   : 
          case __C('2')   : 
          case __C('3')   : 
          case __C('4')   : 
          case __C('5')   : 
          case __C('6')   : 
          case __C('7')   : 
          case __C('8')   : 
          case __C('9')   : 
          case __C('0')   : 

          case __C('!')   : 
          case __C('@')   : 
          case __C('#')   : 
          case __C('$')   : 
          case __C('%')   : 
          case __C('^')   : 
          case __C('&')   : 
          case __C('*')   : 
          case __C('(')   : 
          case __C(')')   : 
          case __C('_')   : 
          case __C('+')   : 
          case __C('-')   : 
          case __C('=')   : 
          case __C('[')   : 
          case __C(']')   : 
          case __C('{')   : 
          case __C('}')   : 
          case __C('|')   : 
          case __C(';')   : 
          case __C(':')   : 
          case __C('\'')  : 
          case __C(',')   : 
          case __C('.')   : 
          case __C('<')   :    
          case __C('?')   : 
          case __C('/')   : 
          case __C('\\')  : 
          case __C('"')   :       

          case 0xBF       : // __C('')   : 
          case 0xA1       : // __C('')   : 
          case 0xF1       : // __C('?')   : 
          case 0xD1       : //__C('_')    :    
          case 0xB7       : //__C('')    :
                            Key_ClickByLiteral(literal.Get(), pressuretimeinterval);                            
                            break;                           
        }
    }

  return false;
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
  SetCursorPos(x, y);
  
  mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
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




