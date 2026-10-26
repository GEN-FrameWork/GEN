/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XLINUXProcessManager.cpp
* 
* @class      XLINUXPROCESSMANAGER
* @brief      LINUX eXtended Utils process management class
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#include "XLINUXProcessManager.h"

#include "XLINUXFactory.h"

#include "XBuffer.h"
#include "XFile.h"
#include "XTrace.h"
#include "XString.h"
#include "XPath.h"

#include <limits.h>

#ifdef LINUX_X11_ACTIVE
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <stdint.h>
#endif



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



#ifdef LINUX_X11_ACTIVE

static int XLINUXProcessManager_XErrorIgnore(Display* /*display*/, XErrorEvent* /*event*/)
{
  return 0;
}


static bool XLINUXProcessManager_GetWindowPid(Display* display, Window window, XDWORD& outpid)
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
      outpid = (XDWORD)(*((unsigned long*)prop));
      ok = (outpid != 0);
    }

  if(prop) XFree(prop);
  return ok;
}


static bool XLINUXProcessManager_GetWindowTitle(Display* display, Window window, XSTRING& title)
{
  title.Empty();
  if(!display || !window) return false;

  Atom           utf8 = XInternAtom(display, "UTF8_STRING", True);
  Atom           netname = XInternAtom(display, "_NET_WM_NAME", True);
  Atom           actualtype = None;
  int            actualformat = 0;
  unsigned long  nitems = 0;
  unsigned long  bytesafter = 0;
  unsigned char* prop = NULL;

  if((netname != None) && (utf8 != None))
    {
      if(XGetWindowProperty(display, window, netname, 0, 1024, False, utf8, &actualtype, &actualformat, &nitems, &bytesafter, &prop) == Success)
        {
          if(prop && (nitems > 0))
            {
              title.Set((char*)prop);
              XFree(prop);
              if(!title.IsEmpty()) return true;
            }
          if(prop) XFree(prop);
          prop = NULL;
        }
    }

  XTextProperty textprop;
  memset(&textprop, 0, sizeof(textprop));
  if(XGetWMName(display, window, &textprop) && textprop.value)
    {
      title.Set((char*)textprop.value);
      if(textprop.value) XFree(textprop.value);
      return !title.IsEmpty();
    }

  return false;
}


static bool XLINUXProcessManager_GetWindowRect(Display* display, Window window, GRPRECTINT& rect)
{
  rect.x1 = 0; rect.y1 = 0; rect.x2 = 0; rect.y2 = 0;
  if(!display || !window) return false;

  XWindowAttributes attr;
  memset(&attr, 0, sizeof(attr));
  if(!XGetWindowAttributes(display, window, &attr)) return false;
  if((attr.width <= 0) || (attr.height <= 0)) return false;

  Window child = None;
  int    rootx = 0;
  int    rooty = 0;
  if(!XTranslateCoordinates(display, window, DefaultRootWindow(display), 0, 0, &rootx, &rooty, &child))
    {
      rootx = attr.x;
      rooty = attr.y;
    }

  rect.x1 = rootx;
  rect.y1 = rooty;
  rect.x2 = rootx + attr.width;
  rect.y2 = rooty + attr.height;
  return !rect.IsEmpty();
}


static void XLINUXProcessManager_CollectWindows(Display* display, Window window, XVECTOR<Window>& windows, int depth)
{
  if(!display || !window || (depth > 6)) return;

  XWindowAttributes attr;
  memset(&attr, 0, sizeof(attr));
  if(!XGetWindowAttributes(display, window, &attr)) return;
  if(attr.map_state != IsViewable) return;

  XDWORD pid = 0;
  XSTRING title;
  bool haspid = XLINUXProcessManager_GetWindowPid(display, window, pid);
  bool hastitle = XLINUXProcessManager_GetWindowTitle(display, window, title);

  if(!attr.override_redirect && (haspid || hastitle))
    {
      bool already = false;
      for(XDWORD c=0; c<windows.GetSize(); c++)
        {
          if(windows.Get(c) == window) { already = true; break; }
        }
      if(!already) windows.Add(window);
    }

  Window root_ret = None;
  Window parent = None;
  Window* children = NULL;
  unsigned int nchildren = 0;
  if(!XQueryTree(display, window, &root_ret, &parent, &children, &nchildren)) return;

  for(unsigned int c=0; c<nchildren; c++)
    {
      XLINUXProcessManager_CollectWindows(display, children[c], windows, depth + 1);
    }

  if(children) XFree(children);
}


static bool XLINUXProcessManager_FillFromWindow(Display* display, Window window, XPROCESS& process)
{
  XDWORD  pid = 0;
  XSTRING title;
  GRPRECTINT rect;

  XLINUXProcessManager_GetWindowPid(display, window, pid);
  XLINUXProcessManager_GetWindowTitle(display, window, title);
  if(title.IsEmpty()) return false;
  if(!XLINUXProcessManager_GetWindowRect(display, window, rect)) return false;
  // PID optional on some compositors (WSLg); keep 0 and still expose title/handle/rect.

  process.SetID(pid);
  process.SetWindowHandle((void*)(uintptr_t)window);
  process.GetWindowTitle()->Set(title.Get());
  process.GetWindowRect()->CopyFrom(&rect);

  // CaptureContent/GetPosXY operate on the client drawable. Compositor frame extents
  // (_NET_FRAME_EXTENTS / WSLg DWM chrome) sit OUTSIDE that drawable — adding them here
  // double-offsets bitmap hits (Windows title/border metrics describe Win32 non-client
  // area of the same HWND used for capture). Keep 0 for client-space parity.
  process.SetWindowTitleHeight(0);
  process.SetWindowBorderWidth(0);

  char linkpath[64];
  char exepath[PATH_MAX];
  snprintf(linkpath, sizeof(linkpath), "/proc/%u/exe", (unsigned)pid);
  ssize_t n = readlink(linkpath, exepath, sizeof(exepath) - 1);
  if(n > 0)
    {
      exepath[n] = 0;
      process.GetPath()->Set(exepath);
      XSTRING namefileext;
      if(process.GetPath()->GetNamefileExt(namefileext) && !namefileext.IsEmpty())
        {
          process.GetName()->Set(namefileext.Get());
        }
    }

  if(process.GetName()->IsEmpty() && (pid != 0))
    {
      char statuspath[64];
      char line[512];
      snprintf(statuspath, sizeof(statuspath), "/proc/%u/status", (unsigned)pid);
      FILE* fp = fopen(statuspath, "r");
      if(fp)
        {
          while(fgets(line, sizeof(line), fp))
            {
              if(strncmp(line, "Name:", 5) == 0)
                {
                  char* p = line + 5;
                  while((*p == ' ') || (*p == '\t')) p++;
                  char* end = p + strlen(p);
                  while((end > p) && ((end[-1] == '\n') || (end[-1] == '\r') || (end[-1] == ' ') || (end[-1] == '\t'))) end--;
                  *end = 0;
                  process.GetName()->Set(p);
                  break;
                }
            }
          fclose(fp);
        }
    }

  if(process.GetName()->IsEmpty())
    {
      // WM_CLASS instance name (fallback when PID/exe unavailable).
      XClassHint hint;
      memset(&hint, 0, sizeof(hint));
      if(XGetClassHint(display, window, &hint))
        {
          if(hint.res_name) process.GetName()->Set(hint.res_name);
          if(hint.res_name) XFree(hint.res_name);
          if(hint.res_class) XFree(hint.res_class);
        }
    }

  // Still usable for title-only WaitWindow if caller matches by title with a known app substring.
  if(process.GetName()->IsEmpty() && !title.IsEmpty())
    {
      process.GetName()->Set(title.Get());
    }

  return !process.GetName()->IsEmpty() && (process.GetWindowHandle() != NULL);
}

#endif // LINUX_X11_ACTIVE



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XLINUXPROCESSMANAGER::XLINUXPROCESSMANAGER(): XPROCESSMANAGER()
* @brief      Constructor of class
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
XLINUXPROCESSMANAGER::XLINUXPROCESSMANAGER(): XPROCESSMANAGER()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XLINUXPROCESSMANAGER::~XLINUXPROCESSMANAGER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
XLINUXPROCESSMANAGER::~XLINUXPROCESSMANAGER()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXPROCESSMANAGER::MakeSystemCommand(XCHAR* command)
* @brief      Make system command
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  command : Command pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXPROCESSMANAGER::MakeSystemCommand(XCHAR* command)
{
  XSTRING   _command;
  bool      status = false;

  _command = command;
  
  XBUFFER cmdchar;
  
  _command.ConvertToASCII(cmdchar);  
  //printf(" -- %s\n", cmdchar.GetPtrChar());
  if(system(cmdchar.GetPtrChar()) == -1) status = false; else status = true;
    
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXPROCESSMANAGER::MakeCommand(XCHAR* command, XBUFFER* out, int* returncode)
* @brief      Make command
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  command : Command pointer to use.
* @param[in]  out : Output out.
* @param[in]  returncode : Returncode pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXPROCESSMANAGER::MakeCommand(XCHAR* command, XBUFFER* out, int* returncode)
{
  XSTRING   _command;
  XBYTE   buffer[_MAXBUFFER];
  FILE*   pipe;
  bool    status = true;

  _command = command;

  XBUFFER cmdchar;
  
  _command.ConvertToASCII(cmdchar);   
  pipe = popen(cmdchar.GetPtrChar(), "r");
  if(pipe == NULL) status = false;
  
  if(!status) return false;

  memset(buffer, 0, _MAXBUFFER);
  while(fgets((char*)buffer, _MAXBUFFER, pipe))
    {
      if(out) out->Add((XBYTE*)buffer, SizeBufferASCII(buffer, _MAXBUFFER));
      memset(buffer, 0, _MAXBUFFER);
    }

  int result = pclose(pipe);
  if(returncode) (*returncode) = result;

  return result != -1;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXPROCESSMANAGER::OpenURL(XCHAR* url)
* @brief      Open URL
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  url : URL to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXPROCESSMANAGER::OpenURL(XCHAR* url)
{
  XSTRING command = NL("xdg-open ");
  int     result  = -1;
 
  command.Add(url);

  result = MakeSystemCommand(command.Get());

  if(result == -1) 
    {
      return false;
    }
        
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXPROCESSMANAGER::Application_Execute(XCHAR* applicationpath, XCHAR* params, XBUFFER* in, XBUFFER* out, int* returncode)
* @brief      Application execute
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  applicationpath : Applicationpath pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  in : In pointer to use.
* @param[in]  out : Output out.
* @param[in]  returncode : Returncode pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXPROCESSMANAGER::Application_Execute(XCHAR* applicationpath, XCHAR* params, XBUFFER* in, XBUFFER* out, int* returncode)
{ 
  #define PIPE_READ   0
  #define PIPE_WRITE  1  
  #define MAXNPARAMS  20

  if(!applicationpath) return false;

  // Parity with Windows Application_Execute: only capture/wait when stdin/stdout buffers are used.
  // GUI launches (ExecApplication) pass NULL buffers and must return while the child stays open.
  bool    capturermode = (out != NULL) || ((in != NULL) && in->GetSize());
  pid_t   pID;
  bool    exist  = false;
  bool    status = false;
  int     stdinpipe[2]  = { -1 , -1 };
  int     stdoutpipe[2] = { -1 , -1 };
  XSTRING _params;

  if(capturermode)
    {
      if(pipe(stdinpipe) < 0) return false;
      if(pipe(stdoutpipe) < 0)
        {
          close(stdinpipe[PIPE_READ]);
          close(stdinpipe[PIPE_WRITE]);
          return false;
        }
    }

  XFILE* xfile=GEN_XFACTORY.Create_File();
  if(xfile)
    {
      exist = xfile->Open(applicationpath);
      xfile->Close();
    }

  GEN_XFACTORY.Delete_File(xfile);

  if(!exist) return false;

  //-----------------------------------------
  // exec attributtes

  XSTRING cmd;

  cmd  = NL("chmod 775 ");
  cmd += applicationpath;


  XBUFFER charstr;
  
  cmd.ConvertToASCII(charstr);
  system(charstr.GetPtrChar());

  //------------------------------------------

  cmd = applicationpath;
  
  _params.Add(cmd);
  if(params)
    {
      _params.Add(" ");
      _params.Add(params);
    }
 
  char*     param[MAXNPARAMS] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                                  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
                                };
  int       start             = 0;
  bool      endfound          = false;
  
  for(int c=0; c<MAXNPARAMS; c++)
    {
      XSTRING   string;
      XBUFFER   xbuffer;
      int       found = _params.Find(NL(" "), true, start);

      if(found == XSTRING_NOTFOUND)
        {
          _params.Copy(start, string);
          endfound = true;
        }
       else
        {
          _params.Copy(start, found, string);
          start = found+1;
        }

      string.ConvertToASCII(xbuffer);
         
      param[c] = GEN_NEW char[xbuffer.GetSize()+1];
      if(param[c])
        {
          memset(param[c], 0, xbuffer.GetSize()+1);
          memcpy(param[c], xbuffer.Get(), xbuffer.GetSize());         
        }
                 
      if(endfound) break;
    }

  pID = fork();
  
  if(pID == 0) 
    {
      // Child
      if(capturermode)
        {
          if(dup2(stdinpipe[PIPE_READ], STDIN_FILENO)    == -1) _exit(errno);
          if(dup2(stdoutpipe[PIPE_WRITE], STDOUT_FILENO) == -1) _exit(errno);
          if(dup2(stdoutpipe[PIPE_WRITE], STDERR_FILENO) == -1) _exit(errno);

          close(stdinpipe[PIPE_READ]);
          close(stdinpipe[PIPE_WRITE]);
          close(stdoutpipe[PIPE_READ]);
          close(stdoutpipe[PIPE_WRITE]);
        }
       else
        {
          // Detach from parent's session so SIGHUP on ASQA exit does not kill the GUI.
          setsid();
        }

      // Prefer project assets/ next to CMake/Build when present (Examples/.../assets);
      // otherwise CWD = executable directory.
      {
        XPATH apppath(applicationpath);
        XSTRING appdir;
        if(apppath.GetDriveAndPath(appdir) && !appdir.IsEmpty())
          {
            // Build/<OS>/<arch>/exe → ../../../../assets (UI_System/Examples layout).
            XPATH assetsdir(appdir);
            assetsdir.Add(NL("../../../../assets"));
            XBUFFER assetsascii;
            assetsdir.ConvertToASCII(assetsascii);
            XBUFFER dirascii;
            appdir.ConvertToASCII(dirascii);

            if(assetsascii.GetPtrChar() && (access(assetsascii.GetPtrChar(), F_OK) == 0))
              {
                chdir(assetsascii.GetPtrChar());
              }
             else if(dirascii.GetPtrChar())
              {
                chdir(dirascii.GetPtrChar());
              }
          }
      }

      XBUFFER execlpath;
      cmd.ConvertToASCII(execlpath);
      execl(execlpath.GetPtrChar(), param[ 0], param[ 1], param[ 2], param[ 3], param[ 4], param[ 5], param[ 6], param[ 7], param[ 8], param[ 9], param[10], param[11], param[12], param[13], param[14], param[15], param[16], param[17], param[18], param[19], NULL);   
      _exit(127);
    } 
   else 
    {
      if(pID > 0) 
        {
          int returnstatus = 0;

          if(capturermode)
            {
              XBYTE nchar[_MAXBUFFER];

              close(stdinpipe[PIPE_READ]);
              close(stdoutpipe[PIPE_WRITE]);

              if(in && in->GetSize())
                {
                  write(stdinpipe[PIPE_WRITE], (char*)in->Get(), (int)in->GetSize());
                }

              if(waitpid(pID, &returnstatus, 0) > 0)
                {
                  if(WIFEXITED(returnstatus) && !WEXITSTATUS(returnstatus))
                    {
                      status = true;
                      if(returncode) (*returncode) = WEXITSTATUS(returnstatus);
                    }
                   else if(WIFEXITED(returnstatus) && WEXITSTATUS(returnstatus))
                    {
                      status = false;
                      if(returncode) (*returncode) = WEXITSTATUS(returnstatus);
                    }
                   else
                    {
                      status = true;
                      if(returncode) (*returncode) = WEXITSTATUS(returnstatus);
                    }
                }
               else
                {
                  status = false;
                  if(returncode) (*returncode) = 1;
                }

              if(out)
                {
                  int nread;
                  do{ nread = read(stdoutpipe[PIPE_READ], nchar, _MAXBUFFER);
                      if(nread > 0) out->Add((XBYTE*)nchar, nread);
                    } while(nread == _MAXBUFFER);
                }

              close(stdinpipe[PIPE_WRITE]);
              close(stdoutpipe[PIPE_READ]);
            }
           else
            {
              // GUI / fire-and-forget: brief poll like Windows WaitForSingleObject(..., 500).
              usleep(500000);
              pid_t waited = waitpid(pID, &returnstatus, WNOHANG);
              if(waited == 0)
                {
                  // Still running → success (same as Windows WAIT_TIMEOUT path).
                  status = true;
                  if(returncode) (*returncode) = 0;
                }
               else if(waited == pID)
                {
                  status = (WIFEXITED(returnstatus) && (WEXITSTATUS(returnstatus) == 0));
                  if(returncode) (*returncode) = WEXITSTATUS(returnstatus);
                }
               else
                {
                  status = false;
                  if(returncode) (*returncode) = 1;
                }
            }
        } 
       else 
        {
          if(capturermode)
            {
              close(stdinpipe[PIPE_READ]);
              close(stdinpipe[PIPE_WRITE]);
              close(stdoutpipe[PIPE_READ]);
              close(stdoutpipe[PIPE_WRITE]);
            }
          for(int c=0; c<MAXNPARAMS; c++) GEN_DELETE_ARRAY param[c];
          return false;
        }
    }

   for(int c=0; c<MAXNPARAMS; c++)
    {
      GEN_DELETE_ARRAY param[c];
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXPROCESSMANAGER::Application_IsRunning(XCHAR* applicationname, XDWORD* ID)
* @brief      Application is running
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  applicationname : application name
* @param[in]  ID : ID of the application in the SO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXPROCESSMANAGER::Application_IsRunning(XCHAR* applicationname, XDWORD* ID)
{
  DIR*            dir;
  struct dirent*  ent;
  char*           endptr;
  char            buf[512];

  if(!(dir = opendir("/proc"))) return -1;

  while((ent = readdir(dir)) != NULL)
    {
      // if endptr is not a null character, the directory is not entirely numeric, so ignore it
      long lpid = strtol(ent->d_name, &endptr, 10);
      if(*endptr != '\0')
        {
          continue;
        }

      // try to open the cmdline file
      snprintf(buf, sizeof(buf), "/proc/%ld/status", lpid);
      FILE* fp = fopen(buf, "r");
      if(fp)
        {
          if(fgets(buf, sizeof(buf), fp) != NULL)
            {
              // check the first token in the file, the program name
              char* first = strtok(buf, " ");

              XPATH nameall;

              nameall = first;
              nameall.SetOnlyNamefile();
              nameall.DeleteCharacter(_C(' ') , XSTRINGCONTEXT_FROM_FIRST);
              nameall.DeleteCharacter(_C('\t'), XSTRINGCONTEXT_FROM_FIRST);
              nameall.DeleteCharacter(_C('\n'), XSTRINGCONTEXT_TO_END);
              nameall.DeleteCharacter(_C('\r'), XSTRINGCONTEXT_TO_END);

              if(!nameall.Compare(applicationname, false))
                {
                  if(ID) (*ID) = (XDWORD)lpid;

                  fclose(fp);
                  closedir(dir);

                  return true;

                }
            }

          fclose(fp);
        }
    }

  closedir(dir);

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXPROCESSMANAGER::Application_FindWindowAtPoint(int screenx, int screeny, XPROCESS& outprocess)
* @brief      Application find window at point (Linux stub until foreign-toplevel hit-test exists).
* @ingroup    PLATFORM_LINUX
*
* @param[in]  screenx : Desktop X coordinate.
* @param[in]  screeny : Desktop Y coordinate.
* @param[out] outprocess : Filled process/window snapshot on success.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXPROCESSMANAGER::Application_FindWindowAtPoint(int screenx, int screeny, XPROCESS& outprocess)
{
  #ifndef LINUX_X11_ACTIVE
  return false;
  #else
  Display* display = XOpenDisplay(NULL);
  if(!display) return false;

  XErrorHandler previous = XSetErrorHandler(XLINUXProcessManager_XErrorIgnore);

  XVECTOR<Window> windows;
  XLINUXProcessManager_CollectWindows(display, DefaultRootWindow(display), windows, 0);

  Window best = None;
  int    bestarea = 0;

  for(XDWORD c=0; c<windows.GetSize(); c++)
    {
      Window      window = windows.Get(c);
      GRPRECTINT  rect;
      if(!XLINUXProcessManager_GetWindowRect(display, window, rect)) continue;
      if(!rect.IsHit(screenx, screeny)) continue;

      int area = (int)rect.GetWidth() * (int)rect.GetHeight();
      if(area <= 0) continue;

      // Prefer smallest containing window (same heuristic as former ActionScriptQA hit-test).
      if((best == None) || (area < bestarea))
        {
          best     = window;
          bestarea = area;
        }
    }

  bool status = false;
  if(best != None)
    {
      XPROCESS found;
      if(XLINUXProcessManager_FillFromWindow(display, best, found))
        {
          status = outprocess.CopyFrom(found);
        }
    }

  XSetErrorHandler(previous);
  XCloseDisplay(display);
  return status;
  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXPROCESSMANAGER::Application_GetRunningList(XVECTOR<XPROCESS*>& applist, bool onlywithvalidwindow)
* @brief      Application get running list
* @ingroup    PLATFORM_LINUX
*
* @param[in]  applist : Applist pointer to use.
* @param[in]  onlywithvalidwindow : Onlywithvalidwindow value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXPROCESSMANAGER::Application_GetRunningList(XVECTOR<XPROCESS*>& applist, bool onlywithvalidwindow)
{
  DIR*            dir;
  struct dirent*  ent;
  char*           endptr;
  char            buf[512];

  applist.DeleteContents();
  applist.DeleteAll();

  if(!(dir = opendir("/proc"))) return false;

  while((ent = readdir(dir)) != NULL)
    {
      long lpid = strtol(ent->d_name, &endptr, 10);
      if(*endptr != '\0') continue;

      snprintf(buf, sizeof(buf), "/proc/%ld/status", lpid);
      FILE* fp = fopen(buf, "r");
      if(!fp) continue;

      char line[512];
      XSTRING procname;
      while(fgets(line, sizeof(line), fp))
        {
          if(strncmp(line, "Name:", 5) == 0)
            {
              char* p = line + 5;
              while((*p == ' ') || (*p == '\t')) p++;
              char* end = p + strlen(p);
              while((end > p) && ((end[-1] == '\n') || (end[-1] == '\r') || (end[-1] == ' ') || (end[-1] == '\t'))) end--;
              *end = 0;
              procname.Set(p);
              break;
            }
        }
      fclose(fp);

      if(procname.IsEmpty()) continue;

      XPROCESS* xprocess = GEN_NEW XPROCESS();
      if(!xprocess) continue;

      xprocess->SetID((XDWORD)lpid);
      xprocess->GetName()->Set(procname.Get());

      char linkpath[64];
      char exepath[PATH_MAX];
      snprintf(linkpath, sizeof(linkpath), "/proc/%ld/exe", lpid);
      ssize_t n = readlink(linkpath, exepath, sizeof(exepath) - 1);
      if(n > 0)
        {
          exepath[n] = 0;
          xprocess->GetPath()->Set(exepath);
          XSTRING namefileext;
          if(xprocess->GetPath()->GetNamefileExt(namefileext) && !namefileext.IsEmpty())
            {
              // Prefer executable basename (may include .exe-less name already).
              xprocess->GetName()->Set(namefileext.Get());
            }
        }

      applist.Add(xprocess);
    }

  closedir(dir);

  #ifdef LINUX_X11_ACTIVE
  Display* display = XOpenDisplay(NULL);
  if(display)
    {
      XErrorHandler previous = XSetErrorHandler(XLINUXProcessManager_XErrorIgnore);

      XVECTOR<Window> windows;
      XLINUXProcessManager_CollectWindows(display, DefaultRootWindow(display), windows, 0);

      // Prefer _NET_CLIENT_LIST when the WM exposes it (stacking order).
      Atom           listatom = XInternAtom(display, "_NET_CLIENT_LIST", True);
      Atom           actualtype = None;
      int            actualformat = 0;
      unsigned long  nitems = 0;
      unsigned long  bytesafter = 0;
      unsigned char* prop = NULL;
      if((listatom != None) &&
         (XGetWindowProperty(display, DefaultRootWindow(display), listatom, 0, 1024, False, XA_WINDOW,
                             &actualtype, &actualformat, &nitems, &bytesafter, &prop) == Success) &&
         prop && (nitems > 0))
        {
          windows.DeleteAll();
          Window* list = (Window*)prop;
          for(unsigned long c=0; c<nitems; c++) windows.Add(list[c]);
        }
      if(prop) XFree(prop);

      for(XDWORD w=0; w<windows.GetSize(); w++)
        {
          XPROCESS filled;
          if(!XLINUXProcessManager_FillFromWindow(display, windows.Get(w), filled)) continue;

          XPROCESS* existing = (filled.GetID() != 0) ? Application_GetProcessByID(filled.GetID(), applist) : NULL;
          if(!existing)
            {
              XPROCESS* created = GEN_NEW XPROCESS();
              if(created)
                {
                  created->CopyFrom(filled);
                  applist.Add(created);
                }
              continue;
            }

          if(!existing->GetWindowHandle())
            {
              existing->SetWindowHandle(filled.GetWindowHandle());
              existing->GetWindowTitle()->Set(filled.GetWindowTitle()->Get());
              existing->GetWindowRect()->CopyFrom(filled.GetWindowRect());
              existing->SetWindowTitleHeight(filled.GetWindowTitleHeight());
              existing->SetWindowBorderWidth(filled.GetWindowBorderWidth());
              if(existing->GetPath()->IsEmpty() && filled.GetPath() && !filled.GetPath()->IsEmpty())
                {
                  existing->GetPath()->Set(filled.GetPath()->Get());
                }
              if(filled.GetName() && !filled.GetName()->IsEmpty())
                {
                  existing->GetName()->Set(filled.GetName()->Get());
                }
            }
           else
            {
              // Extra top-level window for same PID → duplicate entry (Windows parity).
              XPROCESS* cloned = GEN_NEW XPROCESS();
              if(cloned)
                {
                  cloned->CopyFrom((*existing));
                  cloned->SetWindowHandle(filled.GetWindowHandle());
                  cloned->GetWindowTitle()->Set(filled.GetWindowTitle()->Get());
                  cloned->GetWindowRect()->CopyFrom(filled.GetWindowRect());
                  cloned->SetWindowTitleHeight(filled.GetWindowTitleHeight());
                  cloned->SetWindowBorderWidth(filled.GetWindowBorderWidth());
                  applist.Add(cloned);
                }
            }
        }

      XSetErrorHandler(previous);
      XCloseDisplay(display);
    }
  #endif

  if(onlywithvalidwindow)
    {
      XDWORD c = 0;
      while(c < applist.GetSize())
        {
          XPROCESS* xprocess = applist.Get(c);
          if(!xprocess ||
             !xprocess->GetWindowHandle() ||
             !xprocess->GetWindowTitle() ||
              xprocess->GetWindowTitle()->IsEmpty() ||
             !xprocess->GetWindowRect() ||
              xprocess->GetWindowRect()->IsEmpty())
            {
              applist.Delete(xprocess);
              GEN_DELETE xprocess;
            }
           else
            {
              c++;
            }
        }
    }

  return true;
} 


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXPROCESSMANAGER::Application_Terminate(XDWORD processID, XDWORD exitcode)
* @brief      Application terminate
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  processID : Process ID value.
* @param[in]  exitcode : Exitcode value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXPROCESSMANAGER::Application_Terminate(XDWORD processID, XDWORD exitcode)
{
  pid_t pid = (pid_t)processID;

    // Send SIGTERM signal to terminate the process gracefully
  if(kill(pid, SIGTERM) == 0) 
    {
      return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XLINUXPROCESSMANAGER::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XLINUXPROCESSMANAGER::Clean()
{

}


