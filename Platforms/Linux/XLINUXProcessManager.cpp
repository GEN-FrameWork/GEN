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



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



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
  XSTRING command = __L("xdg-open ");
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

  pid_t   pID;
  bool    exist  = false;
  bool    status = false;
  int     stdinpipe[2]  = { 0 , 0 };
  int     stdoutpipe[2] = { 0 , 0 };
  int     nresult = 0;  
  XSTRING _params;

  if(pipe(stdinpipe) < 0)
    {
      return -1;
    }

  if(pipe(stdoutpipe) < 0)
    {
      close(stdinpipe[PIPE_READ]);
      close(stdinpipe[PIPE_WRITE]);

      return -1;
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

  cmd  = __L("chmod 775 ");
  cmd += applicationpath;


  XBUFFER charstr;
  
  cmd.ConvertToASCII(charstr);
  status = system(charstr.GetPtrChar());
  
  //if(status == -1) return false;

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
      int       found = _params.Find(__L(" "), true, start);

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
      // redirect stdin
      if(dup2(stdinpipe[PIPE_READ], STDIN_FILENO)    == -1)
        {
          exit(errno);
        }

        // redirect stdout
      if(dup2(stdoutpipe[PIPE_WRITE], STDOUT_FILENO) == -1)
        {
          exit(errno);
        }

      // redirect stderr
      if(dup2(stdoutpipe[PIPE_WRITE], STDERR_FILENO) == -1)
        {
          exit(errno);
        }

      // all these are for use by parent only
      close(stdinpipe[PIPE_READ]);
      close(stdinpipe[PIPE_WRITE]);
      close(stdoutpipe[PIPE_READ]);
      close(stdoutpipe[PIPE_WRITE]);

      // run child process image
      // replace this with any exec* function find easier to use ("man exec")

      // Child
   
      XBUFFER charstr;
   
      cmd.ConvertToASCII(charstr);                                
      execl(charstr.GetPtrChar(), param[ 0], param[ 1], param[ 2], param[ 3], param[ 4], param[ 5], param[ 6], param[ 7], param[ 8], param[ 9], param[10], param[11], param[12], param[13], param[14], param[15], param[16], param[17], param[18], param[19], NULL);   
      _exit(127);
    } 
   else 
    {
      if(pID > 0) 
        {
          // parent
                    
          XBYTE nchar[_MAXBUFFER];  
          int   returnstatus = 0; 

          // close unused file descriptors, these are for child only
          close(stdinpipe[PIPE_READ]);
          close(stdoutpipe[PIPE_WRITE]);     

          if(in)
            {
              if(in->GetSize())
                {                  
                  write(stdinpipe[PIPE_WRITE], (char*)in->Get(), (int)in->GetSize());
                }
            }

          if(waitpid(pID, &returnstatus, 0) > 0) 
            {
              if(WIFEXITED(returnstatus) && !WEXITSTATUS(returnstatus)) 
                {
                  status = true;
                  if(returncode) (*returncode) = WEXITSTATUS(returnstatus);
                } 
               else 
                {
                  if(WIFEXITED(returnstatus) && WEXITSTATUS(returnstatus)) 
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

                  if(nread)
                    {
                      out->Add((XBYTE*)nchar, nread);
                    }

                } while(nread == _MAXBUFFER);
                                
            } 

          close(stdinpipe[PIPE_WRITE]);
          close(stdoutpipe[PIPE_READ]);
                           
        } 
       else 
        {
          // Error fork 

          close(stdinpipe[PIPE_READ]);
          close(stdinpipe[PIPE_WRITE]);

          close(stdoutpipe[PIPE_READ]);
          close(stdoutpipe[PIPE_WRITE]);
          
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
              nameall.DeleteCharacter(__C(' ') , XSTRINGCONTEXT_FROM_FIRST);
              nameall.DeleteCharacter(__C('\t'), XSTRINGCONTEXT_FROM_FIRST);
              nameall.DeleteCharacter(__C('\n'), XSTRINGCONTEXT_TO_END);
              nameall.DeleteCharacter(__C('\r'), XSTRINGCONTEXT_TO_END);

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
              nameall.DeleteCharacter(__C(' ') , XSTRINGCONTEXT_FROM_FIRST);
              nameall.DeleteCharacter(__C('\t'), XSTRINGCONTEXT_FROM_FIRST);
              nameall.DeleteCharacter(__C('\n'), XSTRINGCONTEXT_TO_END);
              nameall.DeleteCharacter(__C('\r'), XSTRINGCONTEXT_TO_END);

              XPROCESS* xprocess = GEN_NEW XPROCESS();
              if(xprocess)
                {                  
                  xprocess->SetID((XDWORD)lpid); 
                  xprocess->GetPath()->Set(__L(""));
                  xprocess->GetName()->Set(nameall); 
                                
                  applist.Add(xprocess); 
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


