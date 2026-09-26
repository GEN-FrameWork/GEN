/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XWINDOWSTrace.cpp
* 
* @class      XWINDOWSTRACE
* @brief      WINDOWS eXtended Utils debug trace class
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



#ifdef XTRACE_ACTIVE


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XWINDOWSTrace.h"

#include "XBuffer.h"
#include "XString.h"
#include "XWINDOWSSystem.h"
#include "XWINDOWSDateTime.h"



/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSTRACE::XWINDOWSTRACE()
* @brief      Constructor of class
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSTRACE::XWINDOWSTRACE()
{
  Clean();

  XTRACE::instance = this;

  mutexhandle = (XQWORD)CreateMutex( NULL, FALSE, NULL);
  if(!mutexhandle) return;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSTRACE::~XWINDOWSTRACE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSTRACE::~XWINDOWSTRACE()
{
  if(mutexhandle)
    {
      CloseHandle((HANDLE)mutexhandle);
      mutexhandle = NULL;
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
* @fn         void XWINDOWSTRACE::PrintSpecial(XTRACE_TARGET* target, XBYTE level, XCHAR* string)
* @brief      Print special
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  target : target to print the debug
* @param[in]  level : level of debug
* @param[in]  string : string with the line
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XWINDOWSTRACE::PrintSpecial(XTRACE_TARGET* target, XBYTE level, XCHAR* string)
{
  if(!target)              return;
  if(!target->GetAim()[0]) return;

  Lock();

  HWND           hwnd;
  COPYDATASTRUCT cs;

  hwnd = FindWindow(NULL, target->GetAim());

  if(hwnd == NULL)
    {
      #ifdef _MSC_VER

      OutputDebugString(string);
      OutputDebugString(__L("\n"));
      #endif
    }
   else
    {

      XDWORD             publicIP = 0;
      XDWORD             localIP  = 0x01010101;
      XBUFFER            xbufferpacket(false);
      XWINDOWSDATETIME   xdatetime;

      xdatetime.Read();

      SetTraceTextToXBuffer(publicIP, localIP, level, sequence, &xdatetime, string, xbufferpacket);

      cs.dwData  = 0;
      cs.lpData  = (PVOID)xbufferpacket.Get();
      cs.cbData  = (XDWORD)xbufferpacket.GetSize();

      SendMessage(hwnd, WM_COPYDATA, (WPARAM)NULL, (LPARAM)&cs);
    }

  target->AddNSendings();

  UnLock();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XWINDOWSTRACE::PrintFile(XTRACE_TARGET* target, XBYTE level, XCHAR* string)
* @brief      Print file
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  target : target to print the debug
* @param[in]  level : level of debug
* @param[in]  string : string with the line
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XWINDOWSTRACE::PrintFile(XTRACE_TARGET* target, XBYTE level, XCHAR* string)
{
  if(!target)              return;
  if(!target->GetAim()[0]) return;

  XSTRING line;
  FILE*   file;

  Lock();

  int error = 0;

  #ifdef BUILDER
  file = _wfopen(target->GetAim(), __L("at"));
  if(!file)
    {
      UnLock();
      return;
    }
  #else
  error = _wfopen_s(&file, target->GetAim(), __L("at"));
  #endif

  if(error)
    {
      UnLock();
      return;
    }

  // --- Check File size --------------

  XDWORD sizefile;

  fflush(file);

  int position = ftell(file);
  fseek(file, 0, SEEK_END);
  sizefile = ftell(file);
  fseek(file, position, SEEK_SET);

  if(sizefile+(XDWORD)line.GetSize()>sizelimit)
    {
      fclose(file);

      DeleteFile(target->GetAim());

      #ifdef BUILDER
      file = _wfopen(target->GetAim(), __L("at"));
      if(!file)
        {
          UnLock();
          return;
        }
      #else
      error = _wfopen_s(&file, target->GetAim(), __L("at"));
      #endif

      if(error)
        {
          UnLock();
          return;
        };
    }

  // --- Check File size --------------

  line = string;

  if(file)
    {
      
      XBUFFER charstr;
      
      line.ConvertToASCII(charstr); 
      fwrite(charstr.Get(), 1, line.GetSize(), file);      
      fwrite(__L("\n\r"), 1, 1, file);

      fclose(file);
    }

  target->AddNSendings();

  UnLock();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XWINDOWSTRACE::PrintNet(XTRACE_TARGET* target, XBYTE level, XCHAR* string)
* @brief      Print net
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  target : target to print the debug
* @param[in]  level : level of debug
* @param[in]  string : string with the line
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XWINDOWSTRACE::PrintNet(XTRACE_TARGET* target, XBYTE level, XCHAR* string)
{
  if(!target)  return;

  #if(defined(DIO_ACTIVE) && defined(DIO_STREAMUDP_ACTIVE))

  Lock();

  XBUFFER xbufferpacket(false);

  xdatetime.Read();

  #ifdef XTRACE_NOINTERNET
  XDWORD publicIP = 0;
  #endif

  SetTraceTextToXBuffer(publicIP, localIP, level, sequence, &xdatetime, string, xbufferpacket);

  SOCKET handle = (SOCKET)target->GetNETHandle();
  send(handle, (const char*)xbufferpacket.Get(), xbufferpacket.GetSize(), 0);

  target->AddNSendings();

  UnLock();  

  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSTRACE::GetHandleNet(XTRACE_TARGET* target)
* @brief      Get handle net
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  target : target of debug
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSTRACE::GetHandleNet(XTRACE_TARGET* target)
{
  #if(defined(DIO_ACTIVE) && defined(DIO_STREAMUDP_ACTIVE))

  SOCKADDR_IN  addr;
  SOCKET       handle;

  memset(&addr, 0, sizeof(SOCKADDR_IN));

  handle = socket(AF_INET, SOCK_DGRAM, 0);
  if(handle == INVALID_SOCKET)  return false;

  if(!target->GetIP()[0]) return false;


  addr.sin_family       = AF_INET;
  addr.sin_port         = htons(target->GetPort());

  if(target->GetIP()[0] == '*')
    {
      addr.sin_addr.s_addr  =  htonl(INADDR_BROADCAST);
    }
   else
    {  
      #ifndef BUILDER
      inet_pton(addr.sin_family, target->GetIP(), &addr.sin_addr.s_addr);
      #else
      addr.sin_addr.s_addr  = inet_addr(target->GetIP());
      #endif
    }

  connect(handle, (LPSOCKADDR)&addr, sizeof(SOCKADDR_IN));

  XQWORD NEThandle = (XQWORD)handle;

  target->SetNETHandle(NEThandle);

  #endif

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSTRACE::CloseHandleNet(XTRACE_TARGET* target)
* @brief      Close handle net
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  target : target of debug
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSTRACE::CloseHandleNet(XTRACE_TARGET* target)
{
  #if(defined(DIO_ACTIVE) && defined(DIO_STREAMUDP_ACTIVE))

  SOCKET handle = (SOCKET)target->GetNETHandle();
  if(!handle) return false;

  closesocket(handle);
  target->SetNETHandle(0);

  #endif

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSTRACE::Lock()
* @brief      Lock
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSTRACE::Lock()
{
  if(!mutexhandle) return false;
  if(WaitForSingleObject((HANDLE)mutexhandle, INFINITE) == WAIT_FAILED) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSTRACE::UnLock()
* @brief      Un lock
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSTRACE::UnLock()
{
  if(!mutexhandle)                       return false;
  if(!ReleaseMutex((HANDLE)mutexhandle)) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XWINDOWSTRACE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XWINDOWSTRACE::Clean()
{
  mutexhandle  = NULL;
}




#endif


