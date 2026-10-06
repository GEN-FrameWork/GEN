/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       INPWINDOWSCapture.cpp
* 
* @class      INPWINDOWSCAPTURE
* @brief      WINDOWS Input Capture class
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

#include "INPWINDOWSCapture.h"

#include <shlwapi.h>

#include "XFactory.h"
#include "XBuffer.h"
#include "XPathsManager.h"
#include "XPath.h"
#include "XSystem.h"
#include "XTrace.h"

#include "INPCapture_XEvent.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

INPWINDOWSCAPTURE* INPWINDOWSCAPTURE::capture = NULL;



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPWINDOWSCAPTURE::INPWINDOWSCAPTURE()
* @brief      Constructor of class
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPWINDOWSCAPTURE::INPWINDOWSCAPTURE()
{  
  Clean();

  capture = this;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPWINDOWSCAPTURE::~INPWINDOWSCAPTURE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPWINDOWSCAPTURE::~INPWINDOWSCAPTURE()
{
  Deactivate();
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPWINDOWSCAPTURE::Activate()
* @brief      Activate
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPWINDOWSCAPTURE::Activate()
{
  if(keyhook) return false;
  
  RegisterEvent(INPCAPTURE_XEVENT_TYPE_PRESSKEY);
  RegisterEvent(INPCAPTURE_XEVENT_TYPE_UNPRESSKEY); 

  // Set up the hook

  HINSTANCE hinst = (HINSTANCE)applicationhandle;
  if(!hinst) return false;

  keyhook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, hinst, 0);
  if(keyhook == NULL)
    {
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L("[Windows Hook Keyboard] Error: Could not hook keyboard. Another program might be interfering."));
      return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPWINDOWSCAPTURE::Deactivate()
* @brief      Deactivate
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPWINDOWSCAPTURE::Deactivate()
{
  if(!keyhook) return false;

  DeRegisterEvent(INPCAPTURE_XEVENT_TYPE_PRESSKEY);
  DeRegisterEvent(INPCAPTURE_XEVENT_TYPE_UNPRESSKEY); 

  //Remove keyboard hook
  if(UnhookWindowsHookEx(keyhook) == 0)
    {
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L("[Windows Hook Keyboard] Error: Could not unhook keyboard."));
      return false;
    }

  //Success
  keyhook = NULL;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         LRESULT CALLBACK INPWINDOWSCAPTURE::LowLevelKeyboardProc(int ncode, WPARAM wparam, LPARAM lparam)
* @brief      Low level keyboard proc
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  ncode : Ncode value.
* @param[in]  wparam : Wparam value.
* @param[in]  lparam : Lparam value.
* 
* @return     LRESULT CALLBACK : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
LRESULT CALLBACK INPWINDOWSCAPTURE::LowLevelKeyboardProc(int ncode, WPARAM wparam, LPARAM lparam)
{
  bool keydown = false;

  if(ncode == HC_ACTION) 
    {
      if(wparam == WM_KEYDOWN || wparam == WM_SYSKEYDOWN) keydown = true;
      if(wparam == WM_KEYUP   || wparam == WM_SYSKEYUP)   keydown = false;
    }

  KBDLLHOOKSTRUCT* keydata = (KBDLLHOOKSTRUCT*)lparam;

  //XTRACE_PRINTCOLOR(XTRACE_COLOR_PURPLE, _L("[Windows Hook keyboard] vkCode %04X, scanCode %08X, flags %08X [%c]"), keydata->vkCode, keydata->scanCode, keydata->flags, keydata->vkCode);

  if(capture)
    { 
      INPCAPTURE_XEVENT xevent(capture, (keydown?INPCAPTURE_XEVENT_TYPE_PRESSKEY:INPCAPTURE_XEVENT_TYPE_UNPRESSKEY));
      
      if(!keydown)
        {      
          XCHAR character = (XCHAR)keydata->vkCode;

          capture->SetNKeys(capture->GetNKeys()); 

          if(capture->GetLimit())
            {
              if(capture->GetLimit() >= capture->GetNKeys())
                {
                  capture->GetString()->Empty();
                  capture->GetBuffer()->Empty();
                }
            }

          if(((character >= 0x20) && (character < 0x80)) || (character == 0x13) || (character == 0x10) || (character == 0x09))
            {
              capture->GetString()->Add(character);
            }

          capture->GetBuffer()->Add((XDWORD)keydata->vkCode);   
        }
        
      xevent.SetVKCode((XDWORD)keydata->vkCode); 
      xevent.SetScanCode((XWORD)keydata->scanCode);
      xevent.SetFlags((XDWORD)keydata->flags);

      xevent.SetNKeys(capture->GetNKeys());
      xevent.SetLimit(capture->GetLimit());
      xevent.GetString()->Set(capture->GetString()->Get());
      xevent.GetBuffer()->Add(capture->GetBuffer()->Get(), capture->GetBuffer()->GetSize());

      capture->PostEvent(&xevent);
      if(xevent.IsKeyLocked())
        {
          return 1;
        }
       else
        {
          return 0;
        }
    }
    
  return CallNextHookEx(NULL, ncode, wparam, lparam);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void INPWINDOWSCAPTURE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void INPWINDOWSCAPTURE::Clean()
{
  keyhook             = NULL;
  capture             = NULL;
}



