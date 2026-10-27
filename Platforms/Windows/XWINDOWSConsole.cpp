/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XWINDOWSConsole.cpp
* 
* @class      XWINDOWSCONSOLE
* @brief      WINDOWS eXtended Utils Console class
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

#include "XWINDOWSConsole.h"

#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <fcntl.h> 
#include <io.h>

#include "XString.h"
#include "XBuffer.h"
#include "XFileTXT.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/


unsigned long XWINDOWSCONSOLE::cursororiginsize    = 0;
bool          XWINDOWSCONSOLE::cursororiginvisible = true;
bool          XWINDOWSCONSOLE::cursororiginsaved   = false;



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSCONSOLE::XWINDOWSCONSOLE(): XCONSOLE()
* @brief      Constructor of class
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSCONSOLE::XWINDOWSCONSOLE(): XCONSOLE()
{
  Clean();

  #ifndef APPFLOW_GRAPHICS_NOTCONSOLE_ACTIVE

  HANDLE              hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO cursorinfo;

  if(hconsole && (hconsole != INVALID_HANDLE_VALUE) && GetConsoleCursorInfo(hconsole, &cursorinfo))
    {
      cursororiginsize    = cursorinfo.dwSize;
      cursororiginvisible = (cursorinfo.bVisible != FALSE);
      cursororiginsaved   = true;

      cursorinfo.bVisible = FALSE;
      SetConsoleCursorInfo(hconsole, &cursorinfo);
    }

  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSCONSOLE::~XWINDOWSCONSOLE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSCONSOLE::~XWINDOWSCONSOLE()
{
  RestoreCursorOrigin();
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XWINDOWSCONSOLE::Clean()
* @brief      Clean the attributes of the class: Default initialize.
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
*
*---------------------------------------------------------------------------------------------------------------------*/
void XWINDOWSCONSOLE::Clean()
{
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XWINDOWSCONSOLE::RestoreCursor()
* @brief      Show the console cursor again (Ctrl-C/Z safe; no instance required).
* @ingroup    PLATFORM_WINDOWS
*
* @return     bool : true if the cursor was restored.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSCONSOLE::RestoreCursor()
{
  return RestoreCursorOrigin();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XWINDOWSCONSOLE::RestoreCursorOrigin()
* @brief      Restore the console cursor state saved when this instance hid it.
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSCONSOLE::RestoreCursorOrigin()
{
  #ifndef APPFLOW_GRAPHICS_NOTCONSOLE_ACTIVE

  HANDLE              hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO cursorinfo;

  if(!hconsole || (hconsole == INVALID_HANDLE_VALUE)) return false;

  if(!GetConsoleCursorInfo(hconsole, &cursorinfo))
    {
      cursorinfo.dwSize   = 25;
      cursorinfo.bVisible = TRUE;
    }

  if(cursororiginsaved)
    {
      if(cursororiginsize > 0) cursorinfo.dwSize = (DWORD)cursororiginsize;
      cursorinfo.bVisible = cursororiginvisible ? TRUE : FALSE;
    }
   else
    {
      // Abrupt Ctrl-C / exit may skip the save path: force a visible cursor for the parent CMD.
      if(cursorinfo.dwSize < 1) cursorinfo.dwSize = 25;
      cursorinfo.bVisible = TRUE;
    }

  if(!SetConsoleCursorInfo(hconsole, &cursorinfo)) return false;

  cursororiginsaved = false;

  return true;

  #else

  return false;

  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSCONSOLE::GetSize(int& width, int& height)
* @brief      Get size
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  width : Width value.
* @param[in]  height : Height value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSCONSOLE::GetSize(int& width, int& height)
{
  #ifndef APPFLOW_GRAPHICS_NOTCONSOLE_ACTIVE

  HWND consolehandle = GetConsoleWindow();
  RECT rect;

  width  = 0;
  height = 0;

  if(GetWindowRect(consolehandle, &rect) == FALSE) return false;

  width  = rect.right  - rect.left;
  height = rect.bottom - rect.top;

  #endif

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSCONSOLE::SetSize(int width, int height)
* @brief      Set size
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  width : Width value.
* @param[in]  height : Height value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSCONSOLE::SetSize(int width, int height)
{
  #ifndef APPFLOW_GRAPHICS_NOTCONSOLE_ACTIVE

  HWND consolehandle = GetConsoleWindow();
  RECT rect;
  RECT rectdesktop;

  if(GetWindowRect(GetDesktopWindow(), &rectdesktop) == FALSE) return false;

  if(GetWindowRect(consolehandle, &rect) == FALSE) return false;

  rect.left   = (rectdesktop.right-width)/2;
  rect.top    = (rectdesktop.bottom-height)/2;

  if(MoveWindow(consolehandle, rect.left, rect.top, width, height, TRUE) == FALSE) return false;

  #endif

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSCONSOLE::GetSizeText(int& columns, int& rows)
* @brief      Get size text
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  columns : Columns value.
* @param[in]  rows : Rows value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSCONSOLE::GetSizeText(int& columns, int& rows)
{
  #ifndef APPFLOW_GRAPHICS_NOTCONSOLE_ACTIVE

  CONSOLE_SCREEN_BUFFER_INFO csbi;

  if(!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) return false;

  columns = csbi.srWindow.Right  - csbi.srWindow.Left + 1;
  rows    = csbi.srWindow.Bottom - csbi.srWindow.Top  + 1;

  #endif

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCONSOLE_SYMBOLSUSED XWINDOWSCONSOLE::GetSymbolsUsed()
* @brief      get symbols used
* @ingroup    PLATFORM_WINDOWS
* 
* @return     XCONSOLE_SYMBOLSUSED : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCONSOLE_SYMBOLSUSED XWINDOWSCONSOLE::GetSymbolsUsed()
{
  UINT                    in_cp  = GetConsoleCP();        
  UINT                    out_cp = GetConsoleOutputCP();  
  XCONSOLE_SYMBOLSUSED    symbol_cp = XCONSOLE_SYMBOLSUSED_UNKNWON;
  
  if(in_cp != out_cp)
    {
      return symbol_cp;
    }

  switch(in_cp)
    {
      case      0 : symbol_cp =  XCONSOLE_SYMBOLSUSED_NOTCONSOLE;         break;  
      case    437 : symbol_cp =  XCONSOLE_SYMBOLSUSED_CODEPAGE_437;       break;  // OEM USA (original DOS)
      case    850	: symbol_cp =  XCONSOLE_SYMBOLSUSED_CODEPAGE_850;       break;  // OEM Western Europe
      case    852	: symbol_cp =  XCONSOLE_SYMBOLSUSED_CODEPAGE_852;       break;  // OEM Central Europe
      case    855	: symbol_cp =  XCONSOLE_SYMBOLSUSED_NOTSUPPORTED;       break;  // OEM Cyrillic
      case    857	: symbol_cp =  XCONSOLE_SYMBOLSUSED_NOTSUPPORTED;       break;  // OEM Turkish
      case    858	: symbol_cp =  XCONSOLE_SYMBOLSUSED_NOTSUPPORTED;       break;  // OEM Multilingual Latin I
      case    860	: symbol_cp =  XCONSOLE_SYMBOLSUSED_NOTSUPPORTED;       break;  // OEM Portuguese
      case    861	: symbol_cp =  XCONSOLE_SYMBOLSUSED_NOTSUPPORTED;       break;  // OEM Icelandic
      case    862	: symbol_cp =  XCONSOLE_SYMBOLSUSED_NOTSUPPORTED;       break;  // OEM Hebrew
      case    863	: symbol_cp =  XCONSOLE_SYMBOLSUSED_NOTSUPPORTED;       break;  // OEM Canadian French
      case    865	: symbol_cp =  XCONSOLE_SYMBOLSUSED_NOTSUPPORTED;       break;  // OEM Nordic
      case    866	: symbol_cp =  XCONSOLE_SYMBOLSUSED_CODEPAGE_866;       break;  // OEM Russian
      case    874	: symbol_cp =  XCONSOLE_SYMBOLSUSED_NOTSUPPORTED;       break;  // OEM Thai
      case    932	: symbol_cp =  XCONSOLE_SYMBOLSUSED_NOTSUPPORTED;       break;  // OEM Japanese (Shift-JIS)
      case    936	: symbol_cp =  XCONSOLE_SYMBOLSUSED_NOTSUPPORTED;       break;  // OEM Simplified Chinese (GBK)
      case    949	: symbol_cp =  XCONSOLE_SYMBOLSUSED_NOTSUPPORTED;       break;  // OEM Korean
      case    950	: symbol_cp =  XCONSOLE_SYMBOLSUSED_NOTSUPPORTED;       break;  // OEM Traditional Chinese

      case   1250	: symbol_cp =  XCONSOLE_SYMBOLSUSED_WINDOWS_1250;       break;  // ANSI Central Europe
      case   1251	: symbol_cp =  XCONSOLE_SYMBOLSUSED_WINDOWS_1251;       break;  // ANSI Cyrillic
      case   1252	: symbol_cp =  XCONSOLE_SYMBOLSUSED_WINDOWS_1252;       break;  // ANSI Western Europe
      case   1253	: symbol_cp =  XCONSOLE_SYMBOLSUSED_WINDOWS_1253;       break;  // ANSI Greek
      case   1254	: symbol_cp =  XCONSOLE_SYMBOLSUSED_WINDOWS_1254;       break;  // ANSI Turkish
      case   1255	: symbol_cp =  XCONSOLE_SYMBOLSUSED_NOTSUPPORTED;       break;  // ANSI Hebrew
      case   1256	: symbol_cp =  XCONSOLE_SYMBOLSUSED_NOTSUPPORTED;       break;  // ANSI Arabic
      case   1257	: symbol_cp =  XCONSOLE_SYMBOLSUSED_NOTSUPPORTED;       break;  // ANSI Baltic
      case   1258	: symbol_cp =  XCONSOLE_SYMBOLSUSED_NOTSUPPORTED;       break;  // ANSI Vietnamese
     
      case   1200	: symbol_cp =  XCONSOLE_SYMBOLSUSED_UNICODE_UTF16;      break;  // UTF-16 LE
      case   1201	: symbol_cp =  XCONSOLE_SYMBOLSUSED_UNICODE_UTF16;      break;  // UTF-16 BE 
      case  65001 : symbol_cp =  XCONSOLE_SYMBOLSUSED_UNICODE_UTF8;       break;  // UTF-8

      case  20932	: symbol_cp =  XCONSOLE_SYMBOLSUSED_NOTSUPPORTED;       break;  // EUC-JP
      case  51932	: symbol_cp =  XCONSOLE_SYMBOLSUSED_NOTSUPPORTED;       break;  // EUC-JP (ext)
      case  54936	: symbol_cp =  XCONSOLE_SYMBOLSUSED_NOTSUPPORTED;       break;  // GB18030

          default : symbol_cp =  XCONSOLE_SYMBOLSUSED_NOTSUPPORTED;       break;
    }  

  return symbol_cp;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSCONSOLE::Maximize()
* @brief      Maximize
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSCONSOLE::Maximize()
{
  #ifndef APPFLOW_GRAPHICS_NOTCONSOLE_ACTIVE

  HANDLE                     hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
  HWND                       hwindow  = GetConsoleWindow();
  CONSOLE_SCREEN_BUFFER_INFO csbi;

  if(!hconsole) return false;
  if(!hwindow)  return false;

  GetConsoleScreenBufferInfo(hconsole , &csbi);

  csbi.dwSize.X = 150;
  csbi.dwSize.Y = 60;

  SetConsoleScreenBufferSize(hconsole, csbi.dwSize);

  ShowWindow(hwindow, SW_MAXIMIZE);

  #endif

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XWINDOWSCONSOLE::Get_InputFocus(void*& handle)
* @brief      Capture current foreground window handle.
* @ingroup    PLATFORM_WINDOWS
*
* @param[out] handle : HWND cast to void*, or NULL.
*
* @return     bool : true if a focus window was captured.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSCONSOLE::Get_InputFocus(void*& handle)
{
  handle = NULL;

  #ifndef APPFLOW_GRAPHICS_NOTCONSOLE_ACTIVE
  HWND foreground = GetForegroundWindow();
  if(!foreground) return false;
  handle = (void*)foreground;
  return true;
  #else
  return false;
  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XWINDOWSCONSOLE::Set_InputFocus(void* handle)
* @brief      Restore foreground focus to a previously captured HWND.
* @ingroup    PLATFORM_WINDOWS
*
* @param[in]  handle : HWND from Get_InputFocus.
*
* @return     bool : true if focus was requested.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSCONSOLE::Set_InputFocus(void* handle)
{
  if(!handle) return false;

  #ifndef APPFLOW_GRAPHICS_NOTCONSOLE_ACTIVE
  HWND hwnd = (HWND)handle;
  if(!IsWindow(hwnd)) return false;

  DWORD pidtarget = 0;
  GetWindowThreadProcessId(hwnd, &pidtarget);
  if(pidtarget) AllowSetForegroundWindow(pidtarget);

  ShowWindow(hwnd, SW_RESTORE);
  return SetForegroundWindow(hwnd) ? true : false;
  #else
  return false;
  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XWINDOWSCONSOLE::Set_Focus()
* @brief      Bring this process console window to the foreground.
* @ingroup    PLATFORM_WINDOWS
*
* @return     bool : true if focus was requested.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSCONSOLE::Set_Focus()
{
  #ifndef APPFLOW_GRAPHICS_NOTCONSOLE_ACTIVE
  HWND hwindow = GetConsoleWindow();
  if(!hwindow) return false;
  return Set_InputFocus((void*)hwindow);
  #else
  return false;
  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSCONSOLE::Minimize()
* @brief      Minimize
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSCONSOLE::Minimize()
{
  #ifndef APPFLOW_GRAPHICS_NOTCONSOLE_ACTIVE

  HANDLE                     hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
  HWND                       hwindow  = GetConsoleWindow();
  CONSOLE_SCREEN_BUFFER_INFO csbi;

  if(!hconsole) return false;
  if(!hwindow)  return false;

  GetConsoleScreenBufferInfo(hconsole , &csbi);

  csbi.dwSize.X = 120;
  csbi.dwSize.Y = 50;

  SetConsoleScreenBufferSize(hconsole, csbi.dwSize);

  ShowWindow(hwindow, SW_MINIMIZE);

  #endif

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSCONSOLE::Hide()
* @brief      Hide
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSCONSOLE::Hide()
{
  #ifndef APPFLOW_GRAPHICS_NOTCONSOLE_ACTIVE

  HWND hwindow  = GetConsoleWindow();
  if(!hwindow)  return false;

  ShowWindow(hwindow, SW_HIDE);

  #endif

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSCONSOLE::IsHide()
* @brief      Is hide
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSCONSOLE::IsHide()
{
  #ifndef APPFLOW_GRAPHICS_NOTCONSOLE_ACTIVE  

  HWND hwindow  = GetConsoleWindow();
  if(!hwindow)  return false;

  return (IsWindowVisible(hwindow))?false:true;

  #else

  return false;

  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSCONSOLE::UnHide()
* @brief      Un hide
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSCONSOLE::UnHide()
{
  #ifndef APPFLOW_GRAPHICS_NOTCONSOLE_ACTIVE

  HWND hwindow  = GetConsoleWindow();
  if(!hwindow)  return false;

  ShowWindow(hwindow, SW_SHOW);

  #endif

  return true;

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSCONSOLE::Print(XCHAR* string)
* @brief      Print
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  string : String to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSCONSOLE::Print(XCHAR* string)
{
  #ifndef APPFLOW_GRAPHICS_NOTCONSOLE_ACTIVE

  if(!string)     return false;
  if(!string[0])  return false;

  fflush(stdout);

  HANDLE consoleoutput = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD  consolemode   = 0;

  if((consoleoutput != NULL) && (consoleoutput != INVALID_HANDLE_VALUE) && GetConsoleMode(consoleoutput, &consolemode))
    {
      const XCHAR* pending   = string;
      XDWORD       remaining = XSTRING::GetSize(string);

      while(remaining)
        {
          DWORD chunk   = (DWORD)remaining;
          DWORD written = 0;

          if(chunk > 40000) chunk = 40000;

          if(!WriteConsoleW(consoleoutput, pending, chunk, &written, NULL)) return false;
          if(!written)                                                      return false;

          pending   += written;
          remaining -= written;
        }

      return true;
    }

  XSTRING _string;
  XBUFFER charstr;

  _string = string;

  if(!_string.ConvertToUTF8(charstr)) return false;

  printf("%s", charstr.GetPtrChar());
  fflush(stdout);

  #endif

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSCONSOLE::Clear(bool fill)
* @brief      Clear
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  fill : Fill value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSCONSOLE::Clear(bool fill)
{
  #ifndef APPFLOW_GRAPHICS_NOTCONSOLE_ACTIVE

  system("cls");

  #endif

  return true;  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSCONSOLE::KBHit(void)
* @brief      KB hit
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSCONSOLE::KBHit(void)
{
  #ifndef APPFLOW_GRAPHICS_NOTCONSOLE_ACTIVE

  #ifdef _MSC_VER
  if(_kbhit()) return true;
  #endif

  #endif

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XWINDOWSCONSOLE::GetChar()
* @brief      Get char
* @ingroup    PLATFORM_WINDOWS
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XWINDOWSCONSOLE::GetChar()
{
  #ifndef APPFLOW_GRAPHICS_NOTCONSOLE_ACTIVE
  
  //if(!KBHit()) return 0;

  return _getch();

  #else

  return 0;

  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSCONSOLE::Generate()
* @brief      Generate
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSCONSOLE::Generate()
{
  //#ifndef APPFLOW_GRAPHICS_NOTCONSOLE_ACTIVE

  #ifndef BUILDER
  if(!AttachConsole(ATTACH_PARENT_PROCESS))
    {
      // allocate a console for this app
      if(!AllocConsole())
        {
          return false;
        }
    }
  #endif

  freopen("CON", "w", stdout);
  freopen("CON", "w", stderr);
  freopen("CON", "r", stdin);

  /*
  HANDLE  consoleoutput = GetStdHandle(STD_OUTPUT_HANDLE);
  int     SystemOutput  = _open_osfhandle(intptr_t(consoleoutput), _O_TEXT);

  // check if output is a console and not redirected to a file
  if(!isatty(SystemOutput))
    {
      return false;
    }

  FILE*   coutputhandle = _fdopen(SystemOutput, "w");

  // Get STDERR handle
  HANDLE  consoleerror  = GetStdHandle(STD_ERROR_HANDLE);
  int     systemerror   = _open_osfhandle(intptr_t(consoleerror), _O_TEXT);
  FILE*   cerrorhandle  = _fdopen(systemerror, "w");

  // Get STDIN handle
  HANDLE  consoleinput  = GetStdHandle(STD_INPUT_HANDLE);
  int     systeminput   = _open_osfhandle(intptr_t(consoleinput), _O_TEXT);
  FILE*   cinputhandle  = _fdopen(systeminput, "r");

  //make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog point to console as well
  std::ios::sync_with_stdio(true);

  // Redirect the CRT standard input, output, and error handles to the console
  freopen_s(&cinputhandle , "CONIN$"  , "r", stdin);
  freopen_s(&coutputhandle, "CONOUT$" , "w", stdout);
  freopen_s(&cerrorhandle , "CONOUT$" , "w", stderr);
  */

  //Clear the error state for each of the C++ standard stream objects.
  /*std::wcout.clear();
  std::cout.clear();
  std::wcerr.clear();
  std::cerr.clear();
  std::wcin.clear();
  std::cin.clear(); */

  //#endif

  return true;   
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSCONSOLE::Close()
* @brief      Close
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSCONSOLE::Close()
{
  //#ifndef APPFLOW_GRAPHICS_NOTCONSOLE_ACTIVE

  #ifndef APPFLOW_GRAPHICS_NOTCONSOLE_ACTIVE

  // Close() is static: show the cursor again before FreeConsole detaches the parent CMD.
  RestoreCursor();

  #endif

  SendEnterKey();

  return FreeConsole();

  //#else

  //return false;

  //#endif

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XWINDOWSCONSOLE::SendEnterKey()
* @brief      Send enter key
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XWINDOWSCONSOLE::SendEnterKey() 
{
  #ifndef APPFLOW_GRAPHICS_NOTCONSOLE_ACTIVE

  INPUT ip;
  // Set up a generic keyboard event.
  ip.type = INPUT_KEYBOARD;
  ip.ki.wScan = 0; // hardware scan code for key
  ip.ki.time = 0;
  ip.ki.dwExtraInfo = 0;

  // Send the "Enter" key
  ip.ki.wVk = 0x0D; // virtual-key code for the "Enter" key
  ip.ki.dwFlags = 0; // 0 for key press
  SendInput(1, &ip, sizeof(INPUT));

  // Release the "Enter" key
  ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
  SendInput(1, &ip, sizeof(INPUT));

  #endif
}




