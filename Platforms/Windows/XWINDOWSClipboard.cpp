/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XWINDOWSClipboard.cpp
* 
* @class      XWINDOWSCLIPBOARD
* @brief      WINDOWS eXtended Utils Clipboard class
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

#include "XWINDOWSClipboard.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSCLIPBOARD::XWINDOWSCLIPBOARD()
* @brief      Constructor of class
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSCLIPBOARD::XWINDOWSCLIPBOARD()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSCLIPBOARD::~XWINDOWSCLIPBOARD()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSCLIPBOARD::~XWINDOWSCLIPBOARD()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSCLIPBOARD::SetText(XSTRING& text)
* @brief      Set text into the Windows clipboard (CF_UNICODETEXT)
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  text : Text to place on the clipboard.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSCLIPBOARD::SetText(XSTRING& text)
{
  if(!OpenClipboard(NULL)) return false;

  EmptyClipboard();

  XDWORD chars = text.GetSize() + 1;
  SIZE_T bytes = (SIZE_T)chars * sizeof(wchar_t);
  HGLOBAL mem  = GlobalAlloc(GMEM_MOVEABLE, bytes);
  if(!mem)
    {
      CloseClipboard();
      return false;
    }

  wchar_t* dst = (wchar_t*)GlobalLock(mem);
  if(!dst)
    {
      GlobalFree(mem);
      CloseClipboard();
      return false;
    }

  if(text.GetSize() > 0 && text.Get())
    {
      memcpy(dst, text.Get(), text.GetSize() * sizeof(wchar_t));
    }

  dst[text.GetSize()] = 0;
  GlobalUnlock(mem);

  if(!SetClipboardData(CF_UNICODETEXT, mem))
    {
      GlobalFree(mem);
      CloseClipboard();
      return false;
    }

  CloseClipboard();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSCLIPBOARD::GetText(XSTRING& text)
* @brief      Get text from the Windows clipboard (CF_UNICODETEXT)
* @ingroup    PLATFORM_WINDOWS
* 
* @param[out] text : Clipboard text (cleared on failure).
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSCLIPBOARD::GetText(XSTRING& text)
{
  text.Empty();

  if(!IsClipboardFormatAvailable(CF_UNICODETEXT)) return false;
  if(!OpenClipboard(NULL)) return false;

  HGLOBAL mem = GetClipboardData(CF_UNICODETEXT);
  if(!mem)
    {
      CloseClipboard();
      return false;
    }

  wchar_t* src = (wchar_t*)GlobalLock(mem);
  if(!src)
    {
      CloseClipboard();
      return false;
    }

  text.Set(src);
  GlobalUnlock(mem);
  CloseClipboard();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSCLIPBOARD::Clear()
* @brief      Clear the Windows clipboard
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSCLIPBOARD::Clear()
{
  if(!OpenClipboard(NULL)) return false;

  bool status = EmptyClipboard()?true:false;
  CloseClipboard();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSCLIPBOARD::HasText()
* @brief      Has Unicode text available on the Windows clipboard
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if clipboard holds Unicode text; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSCLIPBOARD::HasText()
{
  return IsClipboardFormatAvailable(CF_UNICODETEXT)?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XWINDOWSCLIPBOARD::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XWINDOWSCLIPBOARD::Clean()
{

}



