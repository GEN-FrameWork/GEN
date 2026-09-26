/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XLINUXTrace.cpp
* 
* @class      XLINUXTRACE
* @brief      LINUX eXtended Utils debug trace class
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



#ifdef XTRACE_ACTIVE


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XLINUXTrace.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>

#include "XBase.h"
#include "XBuffer.h"

#include "XLINUXSystem.h"
#include "XLINUXDateTime.h"

#include "DIOURL.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XLINUXTRACE::XLINUXTRACE()
* @brief      Constructor of class
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
XLINUXTRACE::XLINUXTRACE()
{
  Clean();

  XTRACE::instance = this;

  pthread_mutexattr_init(&attr);
  if(!pthread_mutex_init(&mutex, &attr)) mutexhandle = 1;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XLINUXTRACE::~XLINUXTRACE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
XLINUXTRACE::~XLINUXTRACE()
{
  if(mutexhandle)
    {
      pthread_mutex_destroy(&mutex);
      mutexhandle = 0;
    }

  for(int c=0; c<XTRACE_MAXNTARGETS; c++)
    {
      if(targets[c].GetType() == XTRACE_TYPE_NET)
        {
          CloseHandleNet(&targets[c]);
        }
    }

  Clean();

  XTRACE::instance = NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XLINUXTRACE::PrintSpecial(XTRACE_TARGET* target, XBYTE level, XCHAR* string)
* @brief      Print special
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  target : target to print the debug
* @param[in]  level : level of debug
* @param[in]  string : string with the line
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XLINUXTRACE::PrintSpecial(XTRACE_TARGET* target, XBYTE level, XCHAR* string)
{
  if(!target)              return;
  if(!target->GetAim()[0]) return;

  Lock();

  XSTRING  _aim;
  XBYTE    aim[_MAXPATH];
  int      filehdl = -1;
  int      size    = 0;

  memset(aim, 0, _MAXPATH);

  _aim.Set(target->GetAim());
  _aim.ConvertToUTF8(aim, size);

  signal(SIGPIPE, SIG_IGN);

  filehdl = open((char*)aim, O_RDWR | O_NONBLOCK);
  if(filehdl == -1)
    {
      if(mkfifo((char*)aim, 0777 /*S_IRUSR | S_IWUSR*/)!=0) return;

      filehdl = open((char*)aim, O_RDWR | O_NONBLOCK);
      if(filehdl == -1)
        {
          UnLock();
          return;
        }
    }

  // Not Limit of Size ...

  XSTRING line;

  line = string;
  
  XBUFFER charstr;
  
  line.ConvertToASCII(charstr);

  write(filehdl, charstr.Get(), line.GetSize());  
  write(filehdl, __L("\n\r"), 2);
  
  fsync(filehdl);

  close(filehdl);

  UnLock();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XLINUXTRACE::PrintFile(XTRACE_TARGET* target, XBYTE level, XCHAR* string)
* @brief      Print file
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  target : target to print the debug
* @param[in]  level : level of debug
* @param[in]  string : string with the line
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XLINUXTRACE::PrintFile(XTRACE_TARGET* target, XBYTE level, XCHAR* string)
{
  if(!target)              return;
  if(!target->GetAim()[0]) return;

  Lock();

  XSTRING  _aim;
  XBYTE    aim[_MAXPATH];
  XSTRING  _mode;
  XBYTE    mode[32];
  XSTRING  line;
  FILE*    file;
  int      size = 0;

  _aim.Set(target->GetAim());
  _aim.ConvertToUTF8(aim, size);

  _mode.Set(__L("at"));
  _mode.ConvertToUTF8(mode, size);

  file=fopen((char*)aim, (char*)mode);
  if(!file)
    {
      UnLock();
      return;
    }

  // --- Check File size --------------

  XDWORD sizefile = 0;

  fflush(file);

  int position = ftell(file);
  fseek(file, 0, SEEK_END);
  sizefile = ftell(file);
  fseek(file, position, SEEK_SET);

  if((sizefile+(int)line.GetSize())>sizelimit)
    {
      fclose(file);

      if(unlink((char*)target)==-1)
        {
          int a=0;
          a++;
        }

      file=fopen((char*)target, (char*)mode);
      if(!file)
        {
          UnLock();
          return;
        }
    }

  line = string;

  XBUFFER charstr;
  
  line.ConvertToASCII(charstr);
  fwrite(charstr.Get(), 1, line.GetSize(), file);  
  fwrite(__L("\n\r"), 1, 1, file);

  fclose(file);

  UnLock();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XLINUXTRACE::PrintNet(XTRACE_TARGET* target, XBYTE level, XCHAR* string)
* @brief      Print net
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  target : target to print the debug
* @param[in]  level : level of debug
* @param[in]  string : string with the line
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XLINUXTRACE::PrintNet(XTRACE_TARGET* target, XBYTE level, XCHAR* string)
{
  if(!target) return;

  #if(defined(DIO_ACTIVE) && defined(DIO_STREAMUDP_ACTIVE))

  Lock();

  XBUFFER xbufferpacket(false);

  xdatetime.Read();

  #ifdef XTRACE_NOINTERNET
  XDWORD publicIP = 0;
  #endif

  SetTraceTextToXBuffer(publicIP, localIP, level, sequence, &xdatetime, string, xbufferpacket);
  
  int handle = (int)target->GetNETHandle();

  send(handle, (const char*)xbufferpacket.Get(), xbufferpacket.GetSize(), 0);

  target->AddNSendings();
 
  UnLock();

  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXTRACE::GetHandleNet(XTRACE_TARGET* target)
* @brief      Get handle net
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  target : target of debug
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXTRACE::GetHandleNet(XTRACE_TARGET* target)
{
  #if(defined(DIO_ACTIVE) && defined(DIO_STREAMUDP_ACTIVE))

  struct sockaddr_in  addr;
  int                 handle;

  memset(&addr, 0, sizeof(struct sockaddr));

  handle = socket(AF_INET, SOCK_DGRAM, 0);
  if(handle < 0)  return false;

  if(!target->GetIP()[0]) return false;

  addr.sin_family       = AF_INET;  
  addr.sin_port         = htons(target->GetPort());

  if(target->GetIP()[0] == '*')
    {
      addr.sin_addr.s_addr =  htonl(INADDR_BROADCAST);

      int yes = 1;
      setsockopt(handle, SOL_SOCKET, SO_BROADCAST, (char*)&yes, sizeof(yes));
    }
   else
    {    
      addr.sin_addr.s_addr  = inet_addr(target->GetIP());
    }

  connect(handle, (struct sockaddr*)&addr, sizeof(struct sockaddr));

  XQWORD NEThandle = (XQWORD)handle;

  target->SetNETHandle(NEThandle);

  #endif

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXTRACE::CloseHandleNet(XTRACE_TARGET* target)
* @brief      Close handle net
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  target : target of debug
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXTRACE::CloseHandleNet(XTRACE_TARGET* target)
{
  #if(defined(DIO_ACTIVE) && defined(DIO_STREAMUDP_ACTIVE))

  int handle = (int)target->GetNETHandle();
  if(!handle) return false;

  close(handle);
  target->SetNETHandle(0);

  #endif

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXTRACE::Lock()
* @brief      Lock
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXTRACE::Lock()
{
  if(!mutexhandle)               return false;
  if(pthread_mutex_lock(&mutex)) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLINUXTRACE::UnLock()
* @brief      Un lock
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLINUXTRACE::UnLock()
{
  if(!mutexhandle)                   return false;
  if(pthread_mutex_unlock(&mutex))   return false;

  return true;
}




#endif


