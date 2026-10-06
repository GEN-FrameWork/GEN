/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XWINDOWSProcessManager.cpp
* 
* @class      XWINDOWSPROCESSMANAGER
* @brief      eXtended WINDOWS process manager class
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

#include "XWINDOWSProcessManager.h"

#include <windows.h>

#include <tlhelp32.h>
#include <psapi.h> 
#include <tchar.h>
#include <process.h>



#include <wtsapi32.h>
#include <userenv.h>

#include "XFactory.h"
#include "XFile.h"
#include "XSleep.h"
#include "XTrace.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/


#pragma comment(lib, "wtsapi32.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "userenv.lib")




/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSPROCESSMANAGER::XWINDOWSPROCESSMANAGER() : XPROCESSMANAGER()
* @brief      Constructor of class
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSPROCESSMANAGER::XWINDOWSPROCESSMANAGER() : XPROCESSMANAGER()
{
  Clean();

  xmutexgetsrunninglist = GEN_XFACTORY.Create_Mutex();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSPROCESSMANAGER::~XWINDOWSPROCESSMANAGER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSPROCESSMANAGER::~XWINDOWSPROCESSMANAGER()
{
  if(xmutexgetsrunninglist)
    {
       GEN_XFACTORY.Delete_Mutex(xmutexgetsrunninglist);
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSPROCESSMANAGER::MakeSystemCommand(XCHAR* command)
* @brief      Make system command
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  command : Command pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSPROCESSMANAGER::MakeSystemCommand(XCHAR* command)
{
  return (_wsystem(command) == -1)?false:true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSPROCESSMANAGER::MakeCommand(XCHAR* command, XBUFFER* out, int* returncode)
* @brief      Make command
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  command : Command pointer to use.
* @param[in]  out : Output out.
* @param[in]  returncode : Returncode pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSPROCESSMANAGER::MakeCommand(XCHAR* command, XBUFFER* out, int* returncode)
{
  XBYTE  buffer[_MAXBUFFER];
  FILE*  pipe;

  if((pipe = _wpopen(command, _L("rt") )) == NULL) return false;

  if(out)
    {
      memset(buffer, 0, 256);
          
      while(fgets((char*)buffer, _MAXBUFFER, pipe))
        {          
          out->Add((XBYTE*)buffer, SizeBufferASCII(buffer, _MAXBUFFER));
          memset(buffer, 0, _MAXBUFFER);
        }
    }

  if(feof(pipe))
    {
      if(returncode) (*returncode) =  _pclose(pipe);
    }
   else
    {
      return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSPROCESSMANAGER::OpenURL(XCHAR* url)
* @brief      Open URL
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  url : URL to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSPROCESSMANAGER::OpenURL(XCHAR* url)
{
  //handleINSTANCE handle = ShellExecute(NULL, L"open", url ,NULL, NULL, SW_SHOWNORMAL); 
  //if((int)handle <= 32) return false;

  ShellExecute(NULL, L"open", url, NULL, NULL, SW_SHOWNORMAL); 

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSPROCESSMANAGER::Application_Execute(XCHAR* applicationpath, XCHAR* params, XBUFFER* in, XBUFFER* out, int* returncode)
* @brief      Application execute
* @ingroup    PLATFORM_WINDOWS
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
bool XWINDOWSPROCESSMANAGER::Application_Execute(XCHAR* applicationpath, XCHAR* params, XBUFFER* in, XBUFFER* out, int* returncode)
{
  if(!applicationpath)
    {
      return false;
    }

  if(!applicationpath[0])
    {
      return false;
    }

  if(returncode) 
    {
      (*returncode) = 0;
    }

  //#define OUTBUF_SIZE   (10*1024*1024)

  #define OUTBUF_SIZE   (4*1024)
  
  PROCESS_INFORMATION processinfo;
  STARTUPINFOW        startupinfo;
  SECURITY_ATTRIBUTES saattr; 
  XBYTE*              outbuftmp           = NULL;
  DWORD               bytes_read;
  DWORD               exitcode            = (DWORD)-1; // sentinel: GetExitCodeProcess() failing must not read as a successful (0) exit
  HANDLE              stdhandle_out_read  = NULL;
  HANDLE              stdhandle_out_write = NULL;
  HANDLE              stdhandle_in_read   = NULL;
  HANDLE              stdhandle_in_write  = NULL;
  XSTRING             command;
  bool                status              = false;
  bool                capturermode        = (out != NULL) || ((in != NULL) && in->GetSize());

  command.Format(_L("\"%s\""), applicationpath);
      
  if(params)
    {
      command.AddFormat(_L(" %s"), params);          
    }        

  ZeroMemory(&processinfo, sizeof(PROCESS_INFORMATION));
  ZeroMemory(&startupinfo, sizeof(STARTUPINFO));
  startupinfo.cb = sizeof(startupinfo);

  if(capturermode)
    {
      outbuftmp = GEN_NEW XBYTE[OUTBUF_SIZE];
      if(!outbuftmp)
        {
          return false;
        }

      memset(&saattr, 0, sizeof(saattr));
      saattr.nLength              = sizeof(SECURITY_ATTRIBUTES);
      saattr.bInheritHandle       = TRUE;
      saattr.lpSecurityDescriptor = NULL;

      if(!CreatePipe(&stdhandle_out_read, &stdhandle_out_write, &saattr, 0))
        {
          GEN_DELETE_ARRAY outbuftmp;
          return false;
        }
      if(!SetHandleInformation(stdhandle_out_read, HANDLE_FLAG_INHERIT, 0))
        {
          CloseHandle(stdhandle_out_read);
          CloseHandle(stdhandle_out_write);
          GEN_DELETE_ARRAY outbuftmp;
          return false;
        }

      if(!CreatePipe(&stdhandle_in_read, &stdhandle_in_write, &saattr, 0))
        {
          CloseHandle(stdhandle_out_read);
          CloseHandle(stdhandle_out_write);
          GEN_DELETE_ARRAY outbuftmp;
          return false;
        }
      if(!SetHandleInformation(stdhandle_in_write, HANDLE_FLAG_INHERIT, 0))
        {
          CloseHandle(stdhandle_out_read);
          CloseHandle(stdhandle_out_write);
          CloseHandle(stdhandle_in_read);
          CloseHandle(stdhandle_in_write);
          GEN_DELETE_ARRAY outbuftmp;
          return false;
        }

      startupinfo.hStdError  = stdhandle_out_write;
      startupinfo.hStdOutput = stdhandle_out_write;
      startupinfo.hStdInput  = stdhandle_in_read;
      startupinfo.dwFlags   |= STARTF_USESTDHANDLES;
    }
  else
    {
      // Interactive GUI launch (ExecApplication): show window, do not redirect std handles.
      startupinfo.dwFlags    |= STARTF_USESHOWWINDOW;
      startupinfo.wShowWindow = SW_SHOWNORMAL;
    }

  if(CreateProcessW(NULL, (LPWSTR)command.Get(), NULL, NULL, capturermode?TRUE:FALSE, capturermode?CREATE_NO_WINDOW:0, NULL, NULL, &startupinfo, &processinfo))
    {
      if(capturermode)
        {
          CloseHandle(stdhandle_out_write);
          stdhandle_out_write = NULL;
          
          if(in)
            {
              if(in->GetSize())
                {
                  DWORD bytes_write;

                  WriteFile(stdhandle_in_write, in->Get(), in->GetSize(), &bytes_write, NULL);
                  
                  FlushFileBuffers(stdhandle_in_write);
                }
            }

          CloseHandle(stdhandle_in_write);
          stdhandle_in_write = NULL;

          if(out)
            {             
              memset(outbuftmp, 0, OUTBUF_SIZE);

              for(;;)
                {
                  if(!ReadFile(stdhandle_out_read, outbuftmp, OUTBUF_SIZE, &bytes_read, NULL))
                    {
                      break;
                    }
          
                  if(bytes_read > 0)
                    {                    
                      out->Add(outbuftmp, bytes_read);
                    }
                }             
            }
        }

      DWORD waitms     = capturermode ? INFINITE : 500;
      DWORD waitresult = WaitForSingleObject(processinfo.hProcess, waitms);

      if(waitresult == WAIT_OBJECT_0)
        {
          // FIXED: exit code retrieval and handle cleanup must happen whenever the wait itself
          // succeeds, not only when the caller also asked to capture stdout (out != NULL) --
          // otherwise returncode was left at its initial 0 and the process/thread handles leaked
          // whenever out was NULL. Mirrors XLINUXPROCESSMANAGER::Application_Execute(), which
          // always retrieves the child's real exit status from waitpid() regardless of whether
          // stdout is being captured.
          if(GetExitCodeProcess(processinfo.hProcess, &exitcode))
            {
              if(returncode) (*returncode) = exitcode;
            }

          CloseHandle(processinfo.hProcess);
          CloseHandle(processinfo.hThread);

          // FIXED: status must reflect whether the child actually exited successfully (exit
          // code 0), exactly like XLINUXPROCESSMANAGER::Application_Execute() does
          // ("WIFEXITED(returnstatus) && !WEXITSTATUS(returnstatus)") -- not just whether the
          // wait for its termination succeeded.
          status = (exitcode == 0);
        }
      else if((waitresult == WAIT_TIMEOUT) && (!capturermode))
        {
          // Fire-and-forget GUI launch: process still running after short settle wait.
          CloseHandle(processinfo.hProcess);
          CloseHandle(processinfo.hThread);
          status = true;
        }
      else
        {
          CloseHandle(processinfo.hProcess);
          CloseHandle(processinfo.hThread);
        }
    }

  if(stdhandle_out_read)  { CloseHandle(stdhandle_out_read);  }
  if(stdhandle_in_read)   { CloseHandle(stdhandle_in_read);   }
  if(stdhandle_out_write) { CloseHandle(stdhandle_out_write); }
  if(stdhandle_in_write)  { CloseHandle(stdhandle_in_write);  }

  if(outbuftmp)
    {
      GEN_DELETE_ARRAY outbuftmp;
    }
  
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSPROCESSMANAGER::Application_ExecuteElevated(XCHAR* applicationpath, XCHAR* params, XBUFFER* in, XBUFFER* out, int* returncode)
* @brief      application  execute elevated
* @ingroup    PLATFORM_WINDOWS
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
bool XWINDOWSPROCESSMANAGER::Application_ExecuteElevated(XCHAR* applicationpath, XCHAR* params, XBUFFER* in, XBUFFER* out, int* returncode)
{  
  DWORD sessionID = 0;
  if(!GetActiveSessionId(&sessionID))
    {
      return FALSE;
    }
    
  EnablePrivilege(SE_ASSIGNPRIMARYTOKEN_NAME);
  EnablePrivilege(SE_INCREASE_QUOTA_NAME);

  HANDLE handleusertoken = NULL;
  if(!WTSQueryUserToken(sessionID, &handleusertoken))
    {
      int error = GetLastError();
      return FALSE;
    }

    
  HANDLE handleprimary = NULL;
  if(!DuplicateToPrimaryToken(handleusertoken, &handleprimary)) 
    {
      CloseHandle(handleusertoken);
      return FALSE;
    }

  CloseHandle(handleusertoken);


  HANDLE handleelevprimary = NULL;
  HANDLE hTokenToUse = handleprimary;
  if(TryGetLinkedElevatedPrimary(handleprimary, &handleelevprimary)) 
    {
      hTokenToUse = handleelevprimary;
    }

    
  LPVOID ptrenv = NULL;
  if(!CreateEnvironmentBlock(&ptrenv, hTokenToUse, FALSE)) 
    {
      ptrenv = NULL;
    }

  LPWSTR cmdLine = NULL;
  if(!BuildMutableCommandLine(applicationpath, params, &cmdLine)) 
    {
      if(ptrenv) 
        {
          DestroyEnvironmentBlock(ptrenv);
        }

      if(handleelevprimary) 
        {
          CloseHandle(handleelevprimary);
        }
        
      CloseHandle(handleprimary);

      return FALSE;
    }

  bool status = Application_Execute(applicationpath, params, in, out, returncode);

  if(cmdLine) 
    {
      HeapFree(GetProcessHeap(), 0, cmdLine);
    }

  if(ptrenv) 
    {
      DestroyEnvironmentBlock(ptrenv);
    }

  if(handleelevprimary) 
    {
      CloseHandle(handleelevprimary);
    }

  CloseHandle(handleprimary);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSPROCESSMANAGER::Application_IsRunning(XCHAR* applicationname, XDWORD* ID)
* @brief      Application is running
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  applicationname : application name
* @param[in]  ID : ID of the application by SO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSPROCESSMANAGER::Application_IsRunning(XCHAR* applicationname, XDWORD* ID)
{
   if(!applicationname)
    {
      return false;
    }

  if(!applicationname[0])
    {
      return false;
    }

  XVECTOR<XPROCESS*>  applist;
  bool                status = GEN_XPROCESSMANAGER.Application_GetRunningList(applist);
  bool                exists = false;

  if(status)
    {
      for(XDWORD c=0; c<applist.GetSize(); c++)
        {
          XSTRING nameapp;
          XSTRING _applicationname = applicationname;

          nameapp  = applist.Get(c)->GetName()->Get();

          if(_applicationname.Find(nameapp, true) != XSTRING_NOTFOUND)
            {
              if(ID) (*ID) = applist.Get(c)->GetID();
              exists = true;
            }

        }
    }
    
  applist.DeleteContents();
  applist.DeleteAll();

  return exists;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSPROCESSMANAGER::Application_GetRunningList(XVECTOR<XPROCESS*>& applist, bool onlywithvalidwindow)
* @brief      Application get running list
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  applist : Applist pointer to use.
* @param[in]  onlywithvalidwindow : Onlywithvalidwindow value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSPROCESSMANAGER::Application_GetRunningList(XVECTOR<XPROCESS*>& applist, bool onlywithvalidwindow)
{
  PROCESSENTRY32 processentry;
  HANDLE         snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
  
  if(snapshot == INVALID_HANDLE_VALUE) 
    {
      return false;
    }

  if(xmutexgetsrunninglist)
    {   
      xmutexgetsrunninglist->Lock();
    }
  
  applist.DeleteContents();
  applist.DeleteAll();

  memset(&processentry, 0, sizeof(PROCESSENTRY32));
  processentry.dwSize = sizeof(PROCESSENTRY32);

  if(Process32First(snapshot, &processentry))
    {      
      do{ XPROCESS* xprocess = GEN_NEW XPROCESS();
          if(xprocess)
            {
              HANDLE  processhandle = NULL;
              TCHAR   pathfilename[MAX_PATH];

              xprocess->SetID((XDWORD)processentry.th32ProcessID); 
              xprocess->GetName()->Set(processentry.szExeFile); 
              
              processhandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processentry.th32ProcessID);
              if(processhandle != NULL) 
                {
                  GetModuleFileNameEx(processhandle, NULL, pathfilename, MAX_PATH);

                  xprocess->GetPath()->Set(pathfilename);

                  CloseHandle(processhandle);  
                }
              
              applist.Add(xprocess);               
            }

          memset(&processentry, 0, sizeof(PROCESSENTRY32));
          processentry.dwSize = sizeof(PROCESSENTRY32);

        } while(Process32Next(snapshot, &processentry));
    }  

  CloseHandle(snapshot);  

  GetChildProcesses(applist);

  MAPWINPROCESS mapofwinprocess;
  GetMapOfWinProcess(mapofwinprocess);

  
  XDWORD c = 0;   

  do{ XPROCESS* xprocess = applist.Get(c);
      if(xprocess)
        {
          if(!xprocess->GetWindowHandle())      
            {              
              for(XDWORD d=0; d<mapofwinprocess.GetSize(); d++)
                {
                  HWND    hwnd      = mapofwinprocess.GetKey(d);
                  XDWORD  processID = mapofwinprocess.GetElement(d);
                  
                  for(XDWORD e=0; e<xprocess->GetProcessIDs()->GetSize(); e++)
                    {                   
                      if(processID == xprocess->GetProcessIDs()->Get(e))
                        {                       
                          XSTRING     title;
                          GRPRECTINT  rect;                          
         
                          GetWindowPropertys(hwnd, title, rect);                          

                          if(!xprocess->GetWindowHandle())                                  
                            {                                                           
                              xprocess->SetWindowHandle((void*)hwnd);         
                              xprocess->GetWindowTitle()->Set(title.Get());
                              xprocess->GetWindowRect()->CopyFrom(&rect);
                              xprocess->SetWindowTitleHeight(GetWindowTitleHeight(hwnd));
                              xprocess->SetWindowBorderWidth(GetWindowBorderWidth(hwnd));                              
                            }
                           else
                            {                                                                    
                              XPROCESS* newxprocess = GEN_NEW XPROCESS();
                              if(newxprocess)
                                {
                                  newxprocess->CopyFrom((*xprocess));  
                                                              
                                  newxprocess->SetWindowHandle((void*)hwnd);         
                                  newxprocess->GetWindowTitle()->Set(title.Get());
                                  newxprocess->GetWindowRect()->CopyFrom(&rect);
                                  newxprocess->SetWindowTitleHeight(GetWindowTitleHeight(hwnd));
                                  newxprocess->SetWindowBorderWidth(GetWindowBorderWidth(hwnd));

                                  applist.Add(newxprocess);                                  
                                }                                                                                                                                                  
                            } 

                          break; 
                        } 
                    }                  
                }
            }
        }
      
      c++;

    } while(c<applist.GetSize());          

  mapofwinprocess.DeleteAll();

  //------------------------------------------------------------------------

  if(onlywithvalidwindow)
    {
      XDWORD c = 0;

      do{ XPROCESS* xprocess = applist.Get(c);
          if(xprocess)
            {
              if(!xprocess->GetWindowHandle()           || 
                  xprocess->GetWindowTitle()->IsEmpty() ||          
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
           else
            {
              break;
            }

        } while(c<applist.GetSize());          
    }

  //------------------------------------------------------------------------
  /*
  #ifdef XTRACE_ACTIVE
  for(XDWORD c = 0; c < applist.GetSize(); c++)
    {
      XPROCESS* process = applist.Get(c);
      if(process)
        {
          XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("ID: %08X, %s, %s, %08X, %s, [%d,%d,%d,%d]") , process->GetID()
                                                                                                , process->GetName()->Get()
                                                                                                , process->GetPath()->Get()                                                                                                
                                                                                                , process->GetWindowHandle()
                                                                                                , process->GetWindowTitle()->Get()
                                                                                                , process->GetWindowRect()->x1
                                                                                                , process->GetWindowRect()->y1
                                                                                                , process->GetWindowRect()->x2
                                                                                                , process->GetWindowRect()->y2
                                                                                                );
        }
    }  
  #endif  
  */
  //------------------------------------------------------------------------

  if(xmutexgetsrunninglist)
    {   
      xmutexgetsrunninglist->UnLock();
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSPROCESSMANAGER::Application_Terminate(XDWORD processID, XDWORD exitcode)
* @brief      Application terminate
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  processID : Process ID value.
* @param[in]  exitcode : Exitcode value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSPROCESSMANAGER::Application_Terminate(XDWORD processID, XDWORD exitcode)
{
  DWORD   desiredaccess   = PROCESS_TERMINATE;
  bool    inherithandle   = FALSE;
  HANDLE  hprocess        = NULL;

  hprocess = OpenProcess(desiredaccess, inherithandle, processID);
  if(hprocess == NULL) return false;

  BOOL result = TerminateProcess(hprocess, exitcode);

  CloseHandle(hprocess);

  return (result?true:false);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         BOOL XWINDOWSPROCESSMANAGER::EnablePrivilege(LPCWSTR privname)
* @brief      enable privilege
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  privname : Privname value.
* 
* @return     BOOL : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
BOOL XWINDOWSPROCESSMANAGER::EnablePrivilege(LPCWSTR privname)
{
  HANDLE hToken = NULL;
  if(!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
    {
      return FALSE;
    }

  LUID luid;
  if(!LookupPrivilegeValueW(NULL, privname, &luid)) 
    {
      CloseHandle(hToken);
      return FALSE;
    }

  TOKEN_PRIVILEGES tp;
  tp.PrivilegeCount = 1;
  tp.Privileges[0].Luid = luid;
  tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

  if(!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL)) 
    {
      CloseHandle(hToken);
      return FALSE;
    }

  DWORD err = GetLastError();
  CloseHandle(hToken);

  return (err == ERROR_SUCCESS);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         BOOL XWINDOWSPROCESSMANAGER::GetActiveSessionId(DWORD* ptrsessionID)
* @brief      get active session id
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  ptrsessionID : Ptrsession ID pointer to use.
* 
* @return     BOOL : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
BOOL XWINDOWSPROCESSMANAGER::GetActiveSessionId(DWORD* ptrsessionID)
{
  if(!ptrsessionID) 
    {
      return FALSE;
    }

  DWORD sid = WTSGetActiveConsoleSessionId();

  if(sid == 0xFFFFFFFF) 
    {
      return FALSE;
    }

  *ptrsessionID = sid;

  return TRUE;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         BOOL XWINDOWSPROCESSMANAGER::DuplicateToPrimaryToken(HANDLE handleIN, HANDLE* phandleprimaryout)
* @brief      duplicate to primary token
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  handleIN : Handle IN value.
* @param[in]  phandleprimaryout : Phandleprimaryout pointer to use.
* 
* @return     BOOL : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
BOOL XWINDOWSPROCESSMANAGER::DuplicateToPrimaryToken(HANDLE handleIN, HANDLE* phandleprimaryout)
{
  if(!phandleprimaryout) 
    {
      return FALSE;
    }
    
  *phandleprimaryout = NULL;

  SECURITY_ATTRIBUTES sa;
  
  ZeroMemory(&sa, sizeof(sa));
  sa.nLength = sizeof(sa);

  return DuplicateTokenEx(handleIN, TOKEN_ALL_ACCESS, &sa, SecurityImpersonation, TokenPrimary, phandleprimaryout);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         BOOL XWINDOWSPROCESSMANAGER::TryGetLinkedElevatedPrimary(HANDLE handleprimary, HANDLE* phandleelevprimaryout)
* @brief      try get linked elevated primary
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  handleprimary : Handleprimary value.
* @param[in]  phandleelevprimaryout : Phandleelevprimaryout pointer to use.
* 
* @return     BOOL : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
BOOL XWINDOWSPROCESSMANAGER::TryGetLinkedElevatedPrimary(HANDLE handleprimary, HANDLE* phandleelevprimaryout)
{
  if(!phandleelevprimaryout) 
    {
      return FALSE;
    }
  
  *phandleelevprimaryout = NULL;

  TOKEN_ELEVATION_TYPE  elevType = TokenElevationTypeDefault;
  DWORD                 retLen   = 0;

  if(!GetTokenInformation(handleprimary, TokenElevationType, &elevType, sizeof(elevType), &retLen))
    {
      return FALSE;
    }

  if(elevType != TokenElevationTypeLimited)
    {
      return FALSE;
    }

  TOKEN_LINKED_TOKEN linked;
  
  ZeroMemory(&linked, sizeof(linked));
  retLen = 0;

  if(!GetTokenInformation(handleprimary, TokenLinkedToken, &linked, sizeof(linked), &retLen))
    {
      return FALSE;
    }

  HANDLE  hLinkedPrimary = NULL;
  BOOL    ok             = DuplicateToPrimaryToken(linked.LinkedToken, &hLinkedPrimary);

  CloseHandle(linked.LinkedToken);

  if(!ok) 
    {
      return FALSE;
    }

  *phandleelevprimaryout = hLinkedPrimary;

  return TRUE;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         BOOL XWINDOWSPROCESSMANAGER::BuildMutableCommandLine(LPCWSTR exepath, LPCWSTR args, LPWSTR* ptrcmdline)
* @brief      build mutable command line
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  exepath : Exepath value.
* @param[in]  args : Args value.
* @param[in]  ptrcmdline : Ptrcmdline pointer to use.
* 
* @return     BOOL : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
BOOL XWINDOWSPROCESSMANAGER::BuildMutableCommandLine(LPCWSTR exepath, LPCWSTR args, LPWSTR* ptrcmdline)
{
  if(!exepath || !ptrcmdline) 
    {
      return FALSE;
    }
  
  *ptrcmdline = NULL;

  SIZE_T exeLen   = lstrlenW(exepath);
  SIZE_T argsLen  = (args && args[0]) ? lstrlenW(args) : 0;    
  SIZE_T total    = (SIZE_T)2 + exeLen + (argsLen ? (SIZE_T)2 + argsLen : (SIZE_T)0) + (SIZE_T)1;

  LPWSTR buf      = (LPWSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, total * sizeof(WCHAR));
  if(!buf) 
    {
      return FALSE;
    }

    
  buf[0] = L'"';
  CopyMemory(buf + 1, exepath, exeLen * sizeof(WCHAR));
  buf[1 + exeLen] = L'"';

  SIZE_T pos = 2 + exeLen; 
  if(argsLen) 
    {
      buf[pos - 0] = L' ';
      CopyMemory(buf + pos, args, argsLen * sizeof(WCHAR));
      pos += argsLen + 1; // +1 por el espacio ya puesto
    }
    
  *ptrcmdline = buf;

  return TRUE;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         BOOL XWINDOWSPROCESSMANAGER::CreateInteractiveProcess_AttemptElevated(LPCWSTR exepath, LPCWSTR args, LPCWSTR workingdir, BOOL waitforexit, DWORD* ptrexitcode)
* @brief      create interactive process  attempt elevated
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  exepath : Exepath value.
* @param[in]  args : Args value.
* @param[in]  workingdir : Workingdir value.
* @param[in]  waitforexit : Waitforexit value.
* @param[in]  ptrexitcode : Ptrexitcode pointer to use.
* 
* @return     BOOL : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
BOOL XWINDOWSPROCESSMANAGER::CreateInteractiveProcess_AttemptElevated(LPCWSTR exepath, LPCWSTR args, LPCWSTR workingdir, BOOL waitforexit, DWORD* ptrexitcode)
{
  if(ptrexitcode) 
    {
      *ptrexitcode = 0;
    }

  if(!exepath || !exepath[0]) 
    {
      return FALSE;
    }

  DWORD sessionId = 0;
  if(!GetActiveSessionId(&sessionId))
    {
      return FALSE;
    }

    
  EnablePrivilege(SE_ASSIGNPRIMARYTOKEN_NAME);
  EnablePrivilege(SE_INCREASE_QUOTA_NAME);

  HANDLE handleusertoken = NULL;
  if(!WTSQueryUserToken(sessionId, &handleusertoken))
    {
      return FALSE;
    }

    
  HANDLE handleprimary = NULL;
  if(!DuplicateToPrimaryToken(handleusertoken, &handleprimary)) 
    {
      CloseHandle(handleusertoken);
      return FALSE;
    }

  CloseHandle(handleusertoken);


  HANDLE handleelevprimary = NULL;
  HANDLE hTokenToUse = handleprimary;
  if(TryGetLinkedElevatedPrimary(handleprimary, &handleelevprimary)) 
    {
      hTokenToUse = handleelevprimary;
    }

    
  LPVOID ptrenv = NULL;
  if(!CreateEnvironmentBlock(&ptrenv, hTokenToUse, FALSE)) 
    {
      ptrenv = NULL;
    }

  LPWSTR cmdLine = NULL;
  if(!BuildMutableCommandLine(exepath, args, &cmdLine)) 
    {
      if(ptrenv) 
        {
          DestroyEnvironmentBlock(ptrenv);
        }

      if(handleelevprimary) 
        {
          CloseHandle(handleelevprimary);
        }
        
      CloseHandle(handleprimary);

      return FALSE;
    }

  STARTUPINFOW si;
  ZeroMemory(&si, sizeof(si));

  si.cb           = sizeof(si);
  si.lpDesktop    = (LPWSTR)L"winsta0\\default";
  si.dwFlags      = STARTF_USESHOWWINDOW;
  si.wShowWindow  = SW_SHOWNORMAL;

  PROCESS_INFORMATION pi;
  ZeroMemory(&pi, sizeof(pi));

  DWORD flags = CREATE_UNICODE_ENVIRONMENT;

  BOOL ok = CreateProcessAsUserW(hTokenToUse, NULL, cmdLine, NULL, NULL, FALSE, flags, ptrenv, workingdir, &si, &pi);
    
  if(cmdLine) 
    {
      HeapFree(GetProcessHeap(), 0, cmdLine);
    }

  if(ptrenv) 
    {
      DestroyEnvironmentBlock(ptrenv);
    }

  if(handleelevprimary) 
    {
      CloseHandle(handleelevprimary);
    }

  CloseHandle(handleprimary);

  if(!ok) 
    {
      XDWORD error = GetLastError();
      return FALSE;
    }

  if(waitforexit) 
    {
      WaitForSingleObject(pi.hProcess, INFINITE);
      if(ptrexitcode) 
        {
          DWORD ec = 0;
          if(GetExitCodeProcess(pi.hProcess, &ec)) 
            {
              *ptrexitcode = ec;
            }
        }
    }

  CloseHandle(pi.hThread);
  CloseHandle(pi.hProcess);

  return TRUE;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSPROCESSMANAGER::GetWindowPropertys(HWND hwnd, XSTRING& title, GRPRECTINT& rect)
* @brief      get window propertys
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  hwnd : Hwnd value.
* @param[in]  title : Title text.
* @param[in]  rect : Rectangle to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSPROCESSMANAGER::GetWindowPropertys(HWND hwnd, XSTRING& title, GRPRECTINT& rect)
{           
  title.Empty(); 

  int sizetitlewindow = GetWindowTextLength(hwnd);                                   
  if(!sizetitlewindow)
    { 
      return false;
    }
                  
  title.AdjustSize(sizetitlewindow+1);
  int size = GetWindowText(hwnd, title.Get(), sizetitlewindow + 1);
  title.AdjustSize();

  if(!size) 
    {
      title.Empty(); 
      return false;
    }

  RECT wrect;

  if(GetWindowRect(hwnd, &wrect))
    {
      rect.x1 = wrect.left;
      rect.y1 = wrect.top;
      rect.x2 = wrect.right;
      rect.y2 = wrect.bottom;
    }
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XWINDOWSPROCESSMANAGER::GetWindowTitleHeight(HWND hwnd)
* @brief      Get window title height
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  hwnd : Hwnd value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XWINDOWSPROCESSMANAGER::GetWindowTitleHeight(HWND hwnd) 
{
  RECT window_rectangle; 
  RECT client_rectangle;
  int  height;
  int  width;
    
  GetWindowRect(hwnd, &window_rectangle);
  GetClientRect(hwnd, &client_rectangle);

  height = (window_rectangle.bottom - window_rectangle.top) - (client_rectangle.bottom - client_rectangle.top);
  width  = (window_rectangle.right - window_rectangle.left) - (client_rectangle.right - client_rectangle.left);
    
  return height - (width/2);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XWINDOWSPROCESSMANAGER::GetWindowBorderWidth(HWND hwnd)
* @brief      Get window border width
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  hwnd : Hwnd value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XWINDOWSPROCESSMANAGER::GetWindowBorderWidth(HWND hwnd) 
{
  RECT window_rectangle; 
  RECT client_rectangle;

  GetWindowRect(hwnd, &window_rectangle);
  GetClientRect(hwnd, &client_rectangle);

  return ((window_rectangle.right - window_rectangle.left) - client_rectangle.right) / 2; 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSPROCESSMANAGER::GetChildProcesses(XVECTOR<XPROCESS*>& applist)
* @brief      Get child processes
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  applist : Applist pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSPROCESSMANAGER::GetChildProcesses(XVECTOR<XPROCESS*>& applist) 
{
  HANDLE threadSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
  if(threadSnapshot == INVALID_HANDLE_VALUE) 
    {
      return false;
    }

  THREADENTRY32 threadEntry;
  threadEntry.dwSize = sizeof(THREADENTRY32);

  if(!Thread32First(threadSnapshot, &threadEntry)) 
    {
      CloseHandle(threadSnapshot);
      return false;
    }

  do { XPROCESS* mainprocess = Application_GetProcessByID(threadEntry.th32OwnerProcessID, applist);
       if(mainprocess) 
         {
           if(mainprocess->GetProcessIDs()->IsEmpty())
             {
               mainprocess->GetProcessIDs()->Add(threadEntry.th32OwnerProcessID); 
             }

           mainprocess->GetProcessIDs()->Add(threadEntry.th32ThreadID);
         }

      } while (Thread32Next(threadSnapshot, &threadEntry));

  CloseHandle(threadSnapshot);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         BOOL CALLBACK XWINDOWSPROCESSMANAGER::EnumWindowsProc(HWND hwnd, LPARAM lparam)
* @brief      Enum windows proc
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  hwnd : Hwnd value.
* @param[in]  lparam : Lparam value.
* 
* @return     BOOL CALLBACK : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
BOOL CALLBACK XWINDOWSPROCESSMANAGER::EnumWindowsProc(HWND hwnd, LPARAM lparam)
{
  MAPWINPROCESS* mapofwinprocess = (MAPWINPROCESS*)lparam;
  if(!mapofwinprocess)
    {
      return FALSE;
    }
    
  DWORD processID;  

  GetWindowThreadProcessId(hwnd, &processID);

  if(processID)
    {
      mapofwinprocess->Add(hwnd, processID);
    }
    
  return TRUE;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSPROCESSMANAGER::GetMapOfWinProcess(MAPWINPROCESS& mapofwinprocess)
* @brief      Get map of win process
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  mapofwinprocess : Mapofwinprocess value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSPROCESSMANAGER::GetMapOfWinProcess(MAPWINPROCESS& mapofwinprocess)
{  
  mapofwinprocess.DeleteAll();

  BOOL result = EnumWindows(EnumWindowsProc, (LPARAM)(&mapofwinprocess));
  
  return result?true:false;
 }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XWINDOWSPROCESSMANAGER::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XWINDOWSPROCESSMANAGER::Clean()
{
  xmutexgetsrunninglist = NULL;
}

