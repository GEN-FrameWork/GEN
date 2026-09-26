/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XLINUXConsole.cpp
* 
* @class      XLINUXCONSOLE
* @brief      LINUX eXtended Utils Console class
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



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XLINUXConsole.h"

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <locale.h>
#include <langinfo.h>
#include <string.h>
#include <strings.h>

#include "XTrace.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XLINUXCONSOLE::XLINUXCONSOLE(): XCONSOLE()
* @brief      Constructor of class
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
XLINUXCONSOLE::XLINUXCONSOLE(): XCONSOLE()
{
  RawModeEnable();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XLINUXCONSOLE::~XLINUXCONSOLE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
XLINUXCONSOLE::~XLINUXCONSOLE()
{
  RawModeDisable();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXCONSOLE::GetSizeText(int& columns, int& rows)
* @brief      Get size text
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  columns : Columns value.
* @param[in]  rows : Rows value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXCONSOLE::GetSizeText(int& columns, int& rows)
{
  #ifndef APPFLOW_GRAPHICS_NOTCONSOLE_ACTIVE

  struct winsize console_size;

  //ioctl(0, TIOCGWINSZ, &console_size);
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &console_size);

  columns = console_size.ws_col;
  rows    = console_size.ws_row;

  #endif

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XCONSOLE_SYMBOLSUSED XLINUXCONSOLE::GetSymbolsUsed()
* @brief      Get symbols used
* @ingroup    PLATFORM_LINUX
*
* @note       LINUX has no per-handle "console code page" the way Windows does (GetConsoleCP() /
*             GetConsoleOutputCP()). The nearest equivalents are: (1) whether stdin/stdout are
*             really attached to a TTY at all -- isatty() -- which mirrors Windows reporting
*             in_cp == 0 when there is no console attached, and (2) which character set the
*             terminal is actually rendering, which on LINUX is not a property of the terminal
*             itself but of the process' locale -- nl_langinfo(CODESET) after setlocale(LC_CTYPE, "").
*
* @return     XCONSOLE_SYMBOLSUSED : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
XCONSOLE_SYMBOLSUSED XLINUXCONSOLE::GetSymbolsUsed()
{
  XCONSOLE_SYMBOLSUSED symbol_cp = XCONSOLE_SYMBOLSUSED_UNKNWON;

  #ifndef APPFLOW_GRAPHICS_NOTCONSOLE_ACTIVE

  bool isinputtty  = (isatty(STDIN_FILENO)  != 0);
  bool isoutputtty = (isatty(STDOUT_FILENO) != 0);

  // Mirrors XWINDOWSCONSOLE::GetSymbolsUsed()'s "if(in_cp != out_cp) return symbol_cp;": input and
  // output are attached in an inconsistent way (e.g. redirected stdin but a real stdout terminal,
  // or the reverse), so there is no single, reliable answer.
  if(isinputtty != isoutputtty)
    {
      return symbol_cp; // XCONSOLE_SYMBOLSUSED_UNKNWON
    }

  // Neither side is a real terminal (stdout/stdin redirected to a file or a pipe, or the process
  // has no controlling terminal at all) -- equivalent to Windows' in_cp == 0 case.
  if(!isinputtty)
    {
      return XCONSOLE_SYMBOLSUSED_NOTCONSOLE;
    }

  // We are attached to a real TTY on both ends: ask the current locale which charset it renders.
  // setlocale(LC_CTYPE, NULL) returns a pointer into internal static storage, so copy it out before
  // calling setlocale() again to change it.
  char        previouslocale[256] = { 0 };
  const char* currentlocale       = setlocale(LC_CTYPE, NULL);

  if(currentlocale) strncpy(previouslocale, currentlocale, sizeof(previouslocale) - 1);

  setlocale(LC_CTYPE, "");

  const char* codeset = nl_langinfo(CODESET);

  if(codeset)
    {
      if     (!strcasecmp(codeset, "UTF-8"))      symbol_cp = XCONSOLE_SYMBOLSUSED_UNICODE_UTF8;

      else if(!strcasecmp(codeset, "ISO-8859-1")) symbol_cp = XCONSOLE_SYMBOLSUSED_ISO_8859_1;
      else if(!strcasecmp(codeset, "ISO-8859-2")) symbol_cp = XCONSOLE_SYMBOLSUSED_ISO_8859_2;
      else if(!strcasecmp(codeset, "ISO-8859-3")) symbol_cp = XCONSOLE_SYMBOLSUSED_ISO_8859_3;
      else if(!strcasecmp(codeset, "ISO-8859-4")) symbol_cp = XCONSOLE_SYMBOLSUSED_ISO_8859_4;

      else if(!strcasecmp(codeset, "CP1250"))     symbol_cp = XCONSOLE_SYMBOLSUSED_WINDOWS_1250;
      else if(!strcasecmp(codeset, "CP1251"))     symbol_cp = XCONSOLE_SYMBOLSUSED_WINDOWS_1251;
      else if(!strcasecmp(codeset, "CP1252"))     symbol_cp = XCONSOLE_SYMBOLSUSED_WINDOWS_1252;
      else if(!strcasecmp(codeset, "CP1253"))     symbol_cp = XCONSOLE_SYMBOLSUSED_WINDOWS_1253;
      else if(!strcasecmp(codeset, "CP1254"))     symbol_cp = XCONSOLE_SYMBOLSUSED_WINDOWS_1254;

      else if(!strcasecmp(codeset, "CP437"))      symbol_cp = XCONSOLE_SYMBOLSUSED_CODEPAGE_437;
      else if(!strcasecmp(codeset, "CP850"))      symbol_cp = XCONSOLE_SYMBOLSUSED_CODEPAGE_850;
      else if(!strcasecmp(codeset, "CP852"))      symbol_cp = XCONSOLE_SYMBOLSUSED_CODEPAGE_852;
      else if(!strcasecmp(codeset, "CP866"))      symbol_cp = XCONSOLE_SYMBOLSUSED_CODEPAGE_866;

      else                                         symbol_cp = XCONSOLE_SYMBOLSUSED_NOTSUPPORTED;
    }

  setlocale(LC_CTYPE, previouslocale[0] ? previouslocale : "C");

  #endif

  return symbol_cp;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XLINUXCONSOLE::Maximize()
* @brief      Maximize
* @ingroup    PLATFORM_LINUX
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXCONSOLE::Maximize()
{
  #ifndef APPFLOW_GRAPHICS_NOTCONSOLE_ACTIVE
  setvbuf(stdout, NULL, _IONBF, 0);
  #endif

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXCONSOLE::Minimize()
* @brief      Minimize
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXCONSOLE::Minimize()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXCONSOLE::Hide()
* @brief      Hide
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXCONSOLE::Hide()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXCONSOLE::IsHide()
* @brief      Is hide
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXCONSOLE::IsHide()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXCONSOLE::Print(XCHAR* string)
* @brief      Print
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  string : string to print
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXCONSOLE::Print(XCHAR* string)
{
  #ifndef APPFLOW_GRAPHICS_NOTCONSOLE_ACTIVE

  if(!string)     return false;
  if(!string[0])  return false;

  XSTRING _string;

  _string = string;

  XBUFFER charstr;
  
  _string.ConvertToASCII(charstr);  
  printf("%s", charstr.GetPtrChar());
  
  fflush(stdout);

  #endif

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXCONSOLE::Clear(bool fill)
* @brief      Clear
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  fill : Fill value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXCONSOLE::Clear(bool fill)
{
  #ifndef APPFLOW_GRAPHICS_NOTCONSOLE_ACTIVE

  printf("\033[2J");
  printf("\033[H");


  //printf("\033[2J\033[H");    
  //printf("\e[3J");

  //printf("\033c");    // KDE
 
  //system("clear");

  #endif

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXCONSOLE::KBHit(void)
* @brief      KB hit
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXCONSOLE::KBHit(void)
{

  #ifndef APPFLOW_GRAPHICS_NOTCONSOLE_ACTIVE
  /*
  struct termios oldt;
  struct termios newt;
  int            ch;
  int            oldf;

  tcgetattr(STDIN_FILENO, &oldt);

  newt          = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);

  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);

  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if(ch != EOF)
    {
      ungetc(ch, stdin);
      return true;
    }

  return false;
  */

  int byteswaiting;
  ioctl(0, FIONREAD, &byteswaiting);

  return byteswaiting > 0;

  #else

  return false;

  #endif


}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XLINUXCONSOLE::GetChar()
* @brief      Get char
* @ingroup    PLATFORM_LINUX
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XLINUXCONSOLE::GetChar()
{
  #ifndef APPFLOW_GRAPHICS_NOTCONSOLE_ACTIVE

  struct termios oldt, newt;
  int ch;

  tcgetattr( STDIN_FILENO, &oldt );               /* Obtenemos atributos del terminal */
  newt = oldt;
  newt.c_lflag &= ~ICANON;
  newt.c_lflag &= ~ECHO;                          /* Eliminamos el Echo */

  tcsetattr( STDIN_FILENO, TCSANOW, &newt );      /* Definimos los nuevos atributos al terminal */

  ch = getchar();

  tcsetattr( STDIN_FILENO, TCSANOW, &oldt );      /* Ponemos los atributos como estaban al principio */

  return ch;

  #else

  return 0;

  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XLINUXCONSOLE::RawModeEnable()
* @brief      Raw mode enable
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XLINUXCONSOLE::RawModeEnable()
{
  #ifndef APPFLOW_GRAPHICS_NOTCONSOLE_ACTIVE

  termios term;

  tcgetattr(0, &term);
  term.c_lflag &= ~(ICANON | ECHO); // Disable echo as well
  tcsetattr(0, TCSANOW, &term);

  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XLINUXCONSOLE::RawModeDisable()
* @brief      Raw mode disable
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XLINUXCONSOLE::RawModeDisable()
{
  #ifndef APPFLOW_GRAPHICS_NOTCONSOLE_ACTIVE

  termios term;
  
  tcgetattr(0, &term);
  term.c_lflag |= ICANON | ECHO;
  tcsetattr(0, TCSANOW, &term);

  #endif
}




