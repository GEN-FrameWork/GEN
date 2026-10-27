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
#include <stdint.h>

#ifdef LINUX_X11_ACTIVE
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#endif

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


#ifdef LINUX_X11_ACTIVE

static int XLINUXConsole_XErrorIgnore(Display* /*display*/, XErrorEvent* /*event*/)
{
  return 0;
}


static bool XLINUXConsole_ActivateWindow(Display* display, Window window)
{
  if(!display || !window) return false;

  XRaiseWindow(display, window);

  Atom netactive = XInternAtom(display, "_NET_ACTIVE_WINDOW", False);
  if(netactive != None)
    {
      XEvent event;
      memset(&event, 0, sizeof(event));
      event.xclient.type         = ClientMessage;
      event.xclient.window       = window;
      event.xclient.message_type = netactive;
      event.xclient.format       = 32;
      event.xclient.data.l[0]    = 1; // application
      event.xclient.data.l[1]    = CurrentTime;
      event.xclient.data.l[2]    = 0;
      event.xclient.data.l[3]    = 0;
      event.xclient.data.l[4]    = 0;
      XSendEvent(display, DefaultRootWindow(display), False, SubstructureRedirectMask | SubstructureNotifyMask, &event);
    }

  XSetInputFocus(display, window, RevertToParent, CurrentTime);
  XFlush(display);
  return true;
}


static bool XLINUXConsole_GetWindowPid(Display* display, Window window, pid_t& outpid)
{
  outpid = 0;
  if(!display || !window) return false;

  Atom           atom = XInternAtom(display, "_NET_WM_PID", True);
  Atom           actualtype = None;
  int            actualformat = 0;
  unsigned long  nitems = 0;
  unsigned long  bytesafter = 0;
  unsigned char* prop = NULL;

  if(atom == None) return false;
  if(XGetWindowProperty(display, window, atom, 0, 1, False, XA_CARDINAL, &actualtype, &actualformat, &nitems, &bytesafter, &prop) != Success)
    {
      return false;
    }

  bool ok = false;
  if(prop && (actualformat == 32) && (nitems >= 1))
    {
      outpid = (pid_t)(*((unsigned long*)prop));
      ok = (outpid != 0);
    }

  if(prop) XFree(prop);
  return ok;
}


static Window XLINUXConsole_FindWindowByPid(Display* display, Window window, pid_t target, int depth)
{
  if(!display || !window || (depth > 8) || (target <= 0)) return None;

  pid_t wpid = 0;
  if(XLINUXConsole_GetWindowPid(display, window, wpid) && (wpid == target))
    {
      XWindowAttributes attr;
      memset(&attr, 0, sizeof(attr));
      if(XGetWindowAttributes(display, window, &attr) && (attr.map_state == IsViewable) && !attr.override_redirect)
        {
          return window;
        }
    }

  Window root_ret = None;
  Window parent = None;
  Window* children = NULL;
  unsigned int nchildren = 0;
  if(!XQueryTree(display, window, &root_ret, &parent, &children, &nchildren)) return None;

  Window found = None;
  for(unsigned int c=0; c<nchildren && found == None; c++)
    {
      found = XLINUXConsole_FindWindowByPid(display, children[c], target, depth + 1);
    }

  if(children) XFree(children);
  return found;
}


static pid_t XLINUXConsole_GetPpid(pid_t pid)
{
  if(pid <= 1) return 0;

  char path[64];
  snprintf(path, sizeof(path), "/proc/%d/stat", (int)pid);
  FILE* fp = fopen(path, "r");
  if(!fp) return 0;

  // /proc/pid/stat: pid (comm) state ppid ...
  int  ppid = 0;
  char state = 0;
  char comm[256];
  if(fscanf(fp, "%*d %255s %c %d", comm, &state, &ppid) != 3)
    {
      fclose(fp);
      return 0;
    }
  fclose(fp);
  return (pid_t)ppid;
}


static Window XLINUXConsole_ResolveTerminalWindow(Display* display)
{
  if(!display) return None;

  const char* envwid = getenv("WINDOWID");
  if(envwid && envwid[0])
    {
      char* end = NULL;
      unsigned long wid = strtoul(envwid, &end, 10);
      if(wid && end && (*end == '\0'))
        {
          return (Window)wid;
        }
    }

  pid_t pid = getpid();
  for(int depth=0; depth<10 && pid > 1; depth++)
    {
      Window found = XLINUXConsole_FindWindowByPid(display, DefaultRootWindow(display), pid, 0);
      if(found != None) return found;
      pid = XLINUXConsole_GetPpid(pid);
    }

  return None;
}

#endif // LINUX_X11_ACTIVE


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XLINUXCONSOLE::Get_InputFocus(void*& handle)
* @brief      Capture current X11 input-focus window.
* @ingroup    PLATFORM_LINUX
*
* @param[out] handle : X11 Window cast to void*, or NULL.
*
* @return     bool : true if a focus window was captured.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXCONSOLE::Get_InputFocus(void*& handle)
{
  handle = NULL;

  #ifdef LINUX_X11_ACTIVE
  Display* display = XOpenDisplay(NULL);
  if(!display) return false;

  XErrorHandler previous = XSetErrorHandler(XLINUXConsole_XErrorIgnore);

  Window focus = None;
  int    revert = RevertToParent;
  XGetInputFocus(display, &focus, &revert);

  XSetErrorHandler(previous);
  XCloseDisplay(display);

  if(!focus || (focus == PointerRoot) || (focus == None)) return false;

  handle = (void*)(uintptr_t)focus;
  return true;
  #else
  return false;
  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XLINUXCONSOLE::Set_InputFocus(void* handle)
* @brief      Restore X11 input focus to a previously captured window.
* @ingroup    PLATFORM_LINUX
*
* @param[in]  handle : X11 Window from Get_InputFocus.
*
* @return     bool : true if focus was requested.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXCONSOLE::Set_InputFocus(void* handle)
{
  if(!handle) return false;

  #ifdef LINUX_X11_ACTIVE
  Display* display = XOpenDisplay(NULL);
  if(!display) return false;

  XErrorHandler previous = XSetErrorHandler(XLINUXConsole_XErrorIgnore);
  bool ok = XLINUXConsole_ActivateWindow(display, (Window)(uintptr_t)handle);
  XSetErrorHandler(previous);
  XCloseDisplay(display);
  return ok;
  #else
  return false;
  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XLINUXCONSOLE::Set_Focus()
* @brief      Raise/focus the terminal emulator window that hosts this process.
* @ingroup    PLATFORM_LINUX
*
* @return     bool : true if a terminal window was found and focused.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXCONSOLE::Set_Focus()
{
  #ifdef LINUX_X11_ACTIVE
  Display* display = XOpenDisplay(NULL);
  if(!display) return false;

  XErrorHandler previous = XSetErrorHandler(XLINUXConsole_XErrorIgnore);
  Window terminal = XLINUXConsole_ResolveTerminalWindow(display);
  bool ok = (terminal != None) && XLINUXConsole_ActivateWindow(display, terminal);
  XSetErrorHandler(previous);
  XCloseDisplay(display);
  return ok;
  #else
  return false;
  #endif
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




