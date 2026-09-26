/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOWINDOWSStreamTCPIP.cpp
* 
* @class      DIOWINDOWSSTREAMTCPIP
* @brief      WINDOWS Data Input/Output Stream TCP/IP class
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

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <windows.h>

#include "XFactory.h"
#include "XBuffer.h"
#include "XThread.h"
#include "XTrace.h"
#include "XString.h"
#include "XTrace.h"

#include "DIOWINDOWSStreamTCPIP.h"

#include "DIOIP.h"
#include "DIOURL.h"
#include "DIOStream_XEvent.h"
#include "DIOStreamEnumServers.h"
#include "DIOStreamTCPIPConfig.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWINDOWSSTREAMTCPIP::DIOWINDOWSSTREAMTCPIP() : DIOSTREAMTCPIP() , XFSMACHINE(0)
* @brief      Constructor of class
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWINDOWSSTREAMTCPIP::DIOWINDOWSSTREAMTCPIP() : DIOSTREAMTCPIP() , XFSMACHINE(0)
{
  Clean();

  AddState( DIOWINDOWSTCPIPFSMSTATE_NONE              ,
            DIOWINDOWSTCPIPFSMEVENT_GETTINGCONNECTION , DIOWINDOWSTCPIPFSMSTATE_GETTINGCONNECTION ,
            DIOWINDOWSTCPIPFSMEVENT_CONNECTED         , DIOWINDOWSTCPIPFSMSTATE_CONNECTED         ,
            DIOWINDOWSTCPIPFSMEVENT_DISCONNECTING     , DIOWINDOWSTCPIPFSMSTATE_DISCONNECTING     ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOWINDOWSTCPIPFSMSTATE_GETTINGCONNECTION ,
            DIOWINDOWSTCPIPFSMEVENT_CONNECTED         , DIOWINDOWSTCPIPFSMSTATE_CONNECTED         ,
            DIOWINDOWSTCPIPFSMEVENT_WAITINGTOREAD     , DIOWINDOWSTCPIPFSMSTATE_WAITINGTOREAD     ,
            DIOWINDOWSTCPIPFSMEVENT_SENDINGDATA       , DIOWINDOWSTCPIPFSMSTATE_SENDINGDATA       ,
            DIOWINDOWSTCPIPFSMEVENT_DISCONNECTING     , DIOWINDOWSTCPIPFSMSTATE_DISCONNECTING     ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOWINDOWSTCPIPFSMSTATE_CONNECTED         ,
            DIOWINDOWSTCPIPFSMEVENT_GETTINGCONNECTION , DIOWINDOWSTCPIPFSMSTATE_GETTINGCONNECTION ,
            DIOWINDOWSTCPIPFSMEVENT_WAITINGTOREAD     , DIOWINDOWSTCPIPFSMSTATE_WAITINGTOREAD     ,
            DIOWINDOWSTCPIPFSMEVENT_SENDINGDATA       , DIOWINDOWSTCPIPFSMSTATE_SENDINGDATA       ,
            DIOWINDOWSTCPIPFSMEVENT_DISCONNECTING     , DIOWINDOWSTCPIPFSMSTATE_DISCONNECTING     ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOWINDOWSTCPIPFSMSTATE_WAITINGTOREAD     ,
            DIOWINDOWSTCPIPFSMEVENT_GETTINGCONNECTION , DIOWINDOWSTCPIPFSMSTATE_GETTINGCONNECTION ,
            DIOWINDOWSTCPIPFSMEVENT_CONNECTED         , DIOWINDOWSTCPIPFSMSTATE_CONNECTED         ,
            DIOWINDOWSTCPIPFSMEVENT_SENDINGDATA       , DIOWINDOWSTCPIPFSMSTATE_SENDINGDATA       ,
            DIOWINDOWSTCPIPFSMEVENT_DISCONNECTING     , DIOWINDOWSTCPIPFSMSTATE_DISCONNECTING     ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOWINDOWSTCPIPFSMSTATE_DISCONNECTING     ,
            DIOWINDOWSTCPIPFSMEVENT_GETTINGCONNECTION , DIOWINDOWSTCPIPFSMSTATE_GETTINGCONNECTION ,
            DIOWINDOWSTCPIPFSMEVENT_CONNECTED         , DIOWINDOWSTCPIPFSMSTATE_CONNECTED         ,
            DIOWINDOWSTCPIPFSMEVENT_WAITINGTOREAD     , DIOWINDOWSTCPIPFSMSTATE_WAITINGTOREAD     ,
            DIOWINDOWSTCPIPFSMEVENT_SENDINGDATA       , DIOWINDOWSTCPIPFSMSTATE_SENDINGDATA       ,
            XFSMACHINESTATE_EVENTDEFEND);

  threadconnection = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMTCPIP, __L("DIOWINDOWSSTREAMTCPIP::DIOWINDOWSSTREAMTCPIP"), ThreadConnection, (void*)this);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWINDOWSSTREAMTCPIP::~DIOWINDOWSSTREAMTCPIP()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWINDOWSSTREAMTCPIP::~DIOWINDOWSSTREAMTCPIP()
{
  if(threadconnection)
    {
      DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMTCPIP, threadconnection);
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMTCPIP::Open()
* @brief      Open
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMTCPIP::Open()
{
  if(!threadconnection)  return false;
  if(!inbuffer)         return false;
  if(!outbuffer)        return false;

  SetEvent(DIOWINDOWSTCPIPFSMEVENT_GETTINGCONNECTION);

  status = DIOSTREAMSTATUS_GETTINGCONNECTION;

  ResetXBuffers();

  ResetConnectionStatistics();

  return threadconnection->Ini();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMTCPIP::Disconnect()
* @brief      Disconnect
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMTCPIP::Disconnect()
{
  if((GetStatus() == DIOSTREAMSTATUS_CONNECTED)          ||
     (GetStatus() == DIOSTREAMSTATUS_GETTINGCONNECTION))
     {
       while(GetStatus()!=DIOSTREAMSTATUS_DISCONNECTED)
        {
          SetEvent(DIOWINDOWSTCPIPFSMEVENT_DISCONNECTING);
          Sleep(10);

          if(threadconnection)
            {
              if(!threadconnection->IsRunning())
                {
                  return true;
                }
            }
        }
     }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMTCPIP::Close()
* @brief      Close
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMTCPIP::Close()
{
  if(!threadconnection)  return false;

  Disconnect();

  threadconnection->End();

  if(config->IsServer())
    {
      SOCKET handleserver = (SOCKET)config->GetHandleMultiServer();

      if(handleserver != INVALID_SOCKET)
        {
          config->SetCounterMultiServer(config->GetCounterMultiServer()-1);

          if(config->GetCounterMultiServer() <=0)
            {
              closesocket(handleserver);

              if(config->GetXMutexMultiServer())
                {
                  config->GetXMutexMultiServer()->Lock();
                  config->SetHandleMultiServer((int)INVALID_SOCKET);
                  config->GetXMutexMultiServer()->UnLock();

                  //XTRACE_PRINTCOLOR(1, __L("TCPIP: Delete Handle BASE %d Port %d"), handleserver, config->GetRemotePort());
                }
            }
        }
    }

  if(handlesocket != INVALID_SOCKET)
    {
      closesocket(handlesocket);
      handlesocket  = INVALID_SOCKET;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SOCKET DIOWINDOWSSTREAMTCPIP::GetHandleSocket()
* @brief      Get handle socket
* @ingroup    PLATFORM_WINDOWS
* 
* @return     SOCKET : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
SOCKET DIOWINDOWSSTREAMTCPIP::GetHandleSocket()
{
  return handlesocket;
}

 
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWINDOWSSTREAMTCPIP::SetHandleSocket(SOCKET handlesocket)
* @brief      Set handle socket
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  handlesocket : Handlesocket value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWINDOWSSTREAMTCPIP::SetHandleSocket(SOCKET handlesocket)
{
  this->handlesocket = handlesocket;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SOCKET DIOWINDOWSSTREAMTCPIP::Accept(SOCKET handlesocket, void* addr, void* addrlen, XDWORD usec)
* @brief      Accept
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  handlesocket : Handlesocket value.
* @param[in]  addr : Addr pointer to use.
* @param[in]  addrlen : Addrlen pointer to use.
* @param[in]  usec : Usec value.
* 
* @return     SOCKET : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
SOCKET DIOWINDOWSSTREAMTCPIP::Accept(SOCKET handlesocket, void* addr, void* addrlen, XDWORD usec)
{
  fd_set         fds;
  struct timeval time_out;
  SOCKET         status;

  if(handlesocket==INVALID_SOCKET) return INVALID_SOCKET;

  FD_ZERO(&fds);
  FD_SET((unsigned int)handlesocket, &fds);

  time_out.tv_sec  = 0;
  time_out.tv_usec = usec;

  status = (SOCKET)select((int)(handlesocket)+1, &fds, NULL, NULL, &time_out);
  if(status>0)
    {
      status = accept(handlesocket, (SOCKADDR*)addr, (int*)addrlen);
    }
   else 
    {
      status = INVALID_SOCKET;
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOWINDOWSSTREAMTCPIP::IsReadyConnect(SOCKET handlesocket)
* @brief      Is ready connect
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  handlesocket : Handlesocket value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOWINDOWSSTREAMTCPIP::IsReadyConnect(SOCKET handlesocket)
{
  if(handlesocket==INVALID_SOCKET) 
    {
      SetLastDIOError(DIOSTREAMERROR_TCPCONNECTION);
      return -1;
    }

  struct timeval  tv;
  int             rc;
  fd_set          fdr;
  fd_set          fdw;
  fd_set          fds;

  FD_ZERO(&fdr);
  FD_ZERO(&fdw);
  FD_ZERO(&fds);

  FD_SET((unsigned int)handlesocket, &fdr);
  FD_SET((unsigned int)handlesocket, &fdw);
  FD_SET((unsigned int)handlesocket, &fds);

  tv.tv_sec  = 0;
  tv.tv_usec = 100;

  rc = select((int)(handlesocket)+1, &fdr, &fdw, &fds, &tv);
  if(rc == SOCKET_ERROR) 
    {
      SetLastDIOError(DIOSTREAMERROR_TCPCONNECTION);
      return -1;
    }

  int status1 = FD_ISSET(handlesocket, &fdr) ? 1 : 0;
  int status2 = FD_ISSET(handlesocket, &fdw) ? 1 : 0;
  int status3 = FD_ISSET(handlesocket, &fds) ? 1 : 0;

  if(config->IsServer())
    {
      if(status1 || status2 )   
        {
          return  1;
        }
    }
   else
    {
      if(status1 || status2)
        {
          int optval;
          int optlen = sizeof(int);

          if(getsockopt(handlesocket, SOL_SOCKET, SO_ERROR, (char*)&optval, &optlen) < 0) 
            {
              SetLastDIOError(DIOSTREAMERROR_TCPCONNECTION);
              return -1;
            }

          if(optval)
            {
              SetLastDIOError(DIOSTREAMERROR_TCPCONNECTION);
              return -1;
            }

          return  1;
        }
    }

  if(status3)
    {
      SetLastDIOError(DIOSTREAMERROR_TCPCONNECTION);
      return -1;
    }

  return 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMTCPIP::GetHandleServer()
* @brief      Get handle server
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMTCPIP::GetHandleServer()
{
  SOCKET       handleserver = (SOCKET)config->GetHandleMultiServer();
  SOCKADDR_IN  loc_addr;
  SOCKADDR_IN  rem_addr;
  XSTRING      IPstring;

  memset(&loc_addr, 0, sizeof(SOCKADDR_IN));
  memset(&rem_addr, 0, sizeof(SOCKADDR_IN));

  if(handleserver == INVALID_SOCKET)
    {
      config->GetXMutexMultiServer()->Lock();

      handleserver = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
      if(handleserver== INVALID_SOCKET)
        {

        }

      int opt = 1;
      if(setsockopt(handleserver, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) == SOCKET_ERROR)
        {
          SetEvent(DIOWINDOWSTCPIPFSMEVENT_DISCONNECTING);
          SetLastDIOError(DIOSTREAMERROR_UNKNOWN);

          config->GetXMutexMultiServer()->UnLock();
          return false;
        }

      config->GetLocalIP()->GetXString(IPstring);

      loc_addr.sin_family = AF_INET;
      if(!config->GetLocalIP()->IsEmpty())
        {
          XBUFFER charstr;
          
          IPstring.ConvertToASCII(charstr);
                    
          #ifndef BUILDER
          inet_pton(loc_addr.sin_family, charstr.GetPtrChar(), &loc_addr.sin_addr.s_addr);
          #else
          loc_addr.sin_addr.s_addr  = inet_addr(charstr.GetPtrChar());
          #endif
          
        } else loc_addr.sin_addr.s_addr = htonl(INADDR_ANY);

      loc_addr.sin_port = htons(config->GetRemotePort());

      if(bind(handleserver, (LPSOCKADDR)&loc_addr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
        {
          int errortype = WSAGetLastError();
          if(errortype == WSAEADDRINUSE)
            {
              SetLastDIOError(DIOSTREAMERROR_ADDRINUSE);
            }
           else SetLastDIOError(DIOSTREAMERROR_UNKNOWN);

          SetEvent(DIOWINDOWSTCPIPFSMEVENT_DISCONNECTING);

          config->GetXMutexMultiServer()->UnLock();
          return false;
        }

      if(listen(handleserver, DIOSTREAMTCPIP_MAXLISTEN) == SOCKET_ERROR)
        {
          SetEvent(DIOWINDOWSTCPIPFSMEVENT_DISCONNECTING);
          SetLastDIOError(DIOSTREAMERROR_UNKNOWN);

          config->GetXMutexMultiServer()->UnLock();
          return false;
        }

      DWORD nonblock = 1;
      ioctlsocket(handleserver, FIONBIO, &nonblock);

      config->SetHandleMultiServer((int)handleserver);

      config->GetXMutexMultiServer()->UnLock();

      //XTRACE_PRINTCOLOR(1, __L("TCPIP: Create Handle BASE %d Port %d"), handleserver, config->GetRemotePort());
    }

  if(enumservers)
    {
      if(enumservers->GetXTimer())
        {
          if(enumservers->GetXTimer()->GetMeasureSeconds() >= DIOSTREAMTCPIP_TIMEOUTENUMSERVERSIGNAL)
            {
              enumservers->SendSignal();
              enumservers->GetXTimer()->Reset();
            }
        }
    }

  XDWORD sizeaddr   = sizeof(SOCKADDR_IN);
  handlesocket = Accept(handleserver, (SOCKADDR*)&rem_addr, &sizeaddr, 50*1000);
  if(handlesocket != INVALID_SOCKET)
    {
      int opt = 1;
      if(setsockopt(handlesocket, SOL_SOCKET, SO_KEEPALIVE, (char*)&opt, sizeof(opt)) == SOCKET_ERROR)
        {
          SetEvent(DIOWINDOWSTCPIPFSMEVENT_DISCONNECTING);
          SetLastDIOError(DIOSTREAMERROR_UNKNOWN);
          return false;
        }

      DWORD nonblock = 1;
      ioctlsocket(handlesocket, FIONBIO, &nonblock);

      #ifndef BUILDER
      char IPcharstring[_MAXSTR];
      inet_ntop(AF_INET, &(rem_addr.sin_addr), IPcharstring, _MAXSTR);
      clientIP.Set(IPcharstring);
      #else
      clientIP.Set(inet_ntoa(rem_addr.sin_addr));
      #endif
    }
   else
    {
      int errortype = WSAGetLastError();
      // consider what's better, check for individual error codes or just anything now WSAEWOULDBLOCK
      if(errortype == WSAEADDRINUSE)
        {
          SetEvent(DIOWINDOWSTCPIPFSMEVENT_DISCONNECTING);
          SetLastDIOError(DIOSTREAMERROR_ADDRINUSE);
          return false;
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMTCPIP::GetHandleClient()
* @brief      Get handle client
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMTCPIP::GetHandleClient()
{
  if(config->GetConnectionURL()->IsEmpty())
    {
      SetEvent(DIOWINDOWSTCPIPFSMEVENT_DISCONNECTING);
      SetLastDIOError(DIOSTREAMERROR_URLNOTVALID);
      return false;
    }

  handlesocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(handlesocket == INVALID_SOCKET)
    {
      SetEvent(DIOWINDOWSTCPIPFSMEVENT_DISCONNECTING);
      SetLastDIOError(DIOSTREAMERROR_TCPCONNECTION);
      return false;
    }

  if(!config->GetLocalIP()->IsEmpty())
    {
      SOCKADDR_IN  loc_addr;
      XSTRING      IPstring;

      config->GetLocalIP()->GetXString(IPstring);

      memset(&loc_addr, 0, sizeof(SOCKADDR_IN));

      loc_addr.sin_family      = AF_INET;

      XBUFFER charstr;
      
      IPstring.ConvertToASCII(charstr);
 
      #ifndef BUILDER
      inet_pton(loc_addr.sin_family, charstr.GetPtrChar(), &loc_addr.sin_addr.s_addr);
      #else
      loc_addr.sin_addr.s_addr = inet_addr(charstr.GetPtrChar());
      #endif
      
      loc_addr.sin_port        = 0; //htons(diostream->config->GetRemotePort());

      if(bind(handlesocket, (LPSOCKADDR)&loc_addr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
        {
          SetEvent(DIOWINDOWSTCPIPFSMEVENT_DISCONNECTING);
          SetLastDIOError(DIOSTREAMERROR_TCPCONNECTION);
          return false;
        }
    }

  SOCKADDR_IN rem_addr;
  DIOIP       remoteIP;
  XSTRING     IPstring;

  memset(&rem_addr, 0, sizeof(SOCKADDR_IN));

  if(!config->GetConnectionURL()->ResolveURL(remoteIP))
    {
      SetEvent(DIOWINDOWSTCPIPFSMEVENT_DISCONNECTING);
      SetLastDIOError(DIOSTREAMERROR_DNS);

      return false;
    }

  remoteIP.GetXString(IPstring);

  rem_addr.sin_family       = AF_INET;
  
  XBUFFER charstr;

  IPstring.ConvertToASCII(charstr);
  
  #ifndef BUILDER
  inet_pton(rem_addr.sin_family, charstr.GetPtrChar(), &rem_addr.sin_addr.s_addr);
  clientIP.Set(charstr.GetPtrChar());
  #else
  rem_addr.sin_addr.s_addr  = inet_addr(charstr.GetPtrChar());
  clientIP.Set(charstr.GetPtrChar());
  #endif
  
  rem_addr.sin_port         = htons(config->GetConnectionPort());

  int opt = 1;
  if(setsockopt(handlesocket, SOL_SOCKET, SO_KEEPALIVE, (char*)&opt, sizeof(opt)) == SOCKET_ERROR)
    {
      SetEvent(DIOWINDOWSTCPIPFSMEVENT_DISCONNECTING);
      SetLastDIOError(DIOSTREAMERROR_TCPCONNECTION);

      return false;
    }

  DWORD nonblock = 1;
  ioctlsocket(handlesocket, FIONBIO, &nonblock);

  if(connect(handlesocket, (LPSOCKADDR)&rem_addr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
    {
      int errortype = WSAGetLastError();
      if((errortype != WSAEWOULDBLOCK) && (errortype != WSAEINPROGRESS) && (errortype != WSAEALREADY))
        {
          SetEvent(DIOWINDOWSTCPIPFSMEVENT_DISCONNECTING);
          SetLastDIOError((errortype == WSAEADDRINUSE)?DIOSTREAMERROR_ADDRINUSE:DIOSTREAMERROR_TCPCONNECTION);
          return false;
        }
    }

  DIOSTREAM_XEVENT xevent(this, DIOSTREAM_XEVENT_TYPE_GETTINGCONNECTION);
  xevent.SetDIOStream(this);
  PostEvent(&xevent);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWINDOWSSTREAMTCPIP::ThreadConnection(void* data)
* @brief      Thread connection
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  data : Data buffer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWINDOWSSTREAMTCPIP::ThreadConnection(void* data)
{
  DIOWINDOWSSTREAMTCPIP* diostream = (DIOWINDOWSSTREAMTCPIP*)data;
  if(!diostream) return;

  if(diostream->GetEvent()==DIOWINDOWSTCPIPFSMEVENT_NONE) // No hay nuevos Eventos
    {
      switch(diostream->GetCurrentState())
        {
          case DIOWINDOWSTCPIPFSMSTATE_NONE               : break;

          case DIOWINDOWSTCPIPFSMSTATE_GETTINGCONNECTION   : if(diostream->handlesocket == INVALID_SOCKET)
                                                              {
                                                                if(diostream->config->IsServer())
                                                                  {
                                                                    diostream->GetHandleServer();
                                                                  }
                                                                 else
                                                                  {
                                                                    diostream->GetHandleClient();
                                                                  }
                                                              }
                                                             else
                                                              {
                                                                switch(diostream->IsReadyConnect(diostream->handlesocket))
                                                                  {
                                                                    case -1:  diostream->SetEvent(DIOWINDOWSTCPIPFSMEVENT_DISCONNECTING);
                                                                              break;

                                                                    case  1:  diostream->SetEvent(DIOWINDOWSTCPIPFSMEVENT_CONNECTED);
                                                                              diostream->status = DIOSTREAMSTATUS_CONNECTED;
                                                                              break;

                                                                    default:  diostream->status = DIOSTREAMSTATUS_GETTINGCONNECTION;
                                                                              break;
                                                                  }
                                                              }
                                                            break;

          case DIOWINDOWSTCPIPFSMSTATE_CONNECTED          : break;

          case DIOWINDOWSTCPIPFSMSTATE_WAITINGTOREAD      : { struct timeval  waitd;
                                                              fd_set          read_flags;
                                                              fd_set          write_flags;
                                                              
                                                              if(diostream->handlesocket==INVALID_SOCKET)
                                                                {
                                                                  diostream->SetEvent(DIOWINDOWSTCPIPFSMEVENT_DISCONNECTING);
                                                                  break;
                                                                }

                                                              waitd.tv_sec  = 0;
                                                              waitd.tv_usec = 100;

                                                              FD_ZERO(&read_flags);
                                                              FD_ZERO(&write_flags);

                                                              if(!diostream->IsBlockRead()) FD_SET(diostream->handlesocket, &read_flags);
                                                              if(!diostream->IsBlockWrite())
                                                                {
                                                                  if(diostream->outbuffer->GetSize()) FD_SET(diostream->handlesocket, &write_flags);
                                                                }

                                                              int error = select((int)(diostream->handlesocket)+1, &read_flags, &write_flags, (fd_set*)NULL, &waitd);
                                                              if(error == SOCKET_ERROR)   //Socket ready for reading
                                                                {
                                                                  diostream->SetEvent(DIOWINDOWSTCPIPFSMEVENT_DISCONNECTING);
                                                                  break;
                                                                }

                                                              if(FD_ISSET(diostream->handlesocket, &read_flags))  //Socket ready for reading
                                                                {
                                                                  XBYTE buffer[DIOSTREAM_MAXBUFFER];

                                                                  FD_CLR(diostream->handlesocket, &read_flags);

                                                                  memset(buffer, 0, DIOSTREAM_MAXBUFFER);

                                                                  XDWORD size = recv(diostream->handlesocket, (char*)buffer, DIOSTREAM_MAXBUFFER, 0);
                                                                  if(size==SOCKET_ERROR)
                                                                    {
                                                                      diostream->SetEvent(DIOWINDOWSTCPIPFSMEVENT_DISCONNECTING);
                                                                      break;
                                                                    }

                                                                  if(size)
                                                                    {
                                                                      diostream->inbuffer->Add(buffer, size);
                                                                    }
                                                                   else
                                                                    {
                                                                      diostream->SetEvent(DIOWINDOWSTCPIPFSMEVENT_DISCONNECTING);
                                                                      break;
                                                                    }
                                                                }

                                                              if(FD_ISSET(diostream->handlesocket, &write_flags))  //Socket ready for writing
                                                                {
                                                                  FD_CLR(diostream->handlesocket, &write_flags);

                                                                  int esize = diostream->outbuffer->GetSize();
                                                                  if(esize>DIOSTREAM_MAXBUFFER) esize = DIOSTREAM_MAXBUFFER;

                                                                  if(esize)
                                                                    {
                                                                      diostream->outbuffer->SetBlocked(true);
                                                                      XDWORD size = send(diostream->handlesocket, (char*)diostream->outbuffer->Get(), esize, 0);
                                                                      diostream->outbuffer->SetBlocked(false);
                                                                      if(size==SOCKET_ERROR)
                                                                        {
                                                                          diostream->SetEvent(DIOWINDOWSTCPIPFSMEVENT_DISCONNECTING);
                                                                          break;
                                                                        }

                                                                      if(size)
                                                                        {
                                                                          diostream->outbuffer->Extract(NULL, 0, size);
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                            break;

          case DIOWINDOWSTCPIPFSMSTATE_SENDINGDATA        : break                                                                                   ;

          case DIOWINDOWSTCPIPFSMSTATE_DISCONNECTING      : break;

        }
    }
   else
    {
      if(diostream->GetEvent()<DIOWINDOWSTCPIP_LASTEVENT)
        {
          diostream->CheckTransition();

          switch(diostream->GetCurrentState())
            {
              case DIOWINDOWSTCPIPFSMSTATE_NONE               : break;

              case DIOWINDOWSTCPIPFSMSTATE_GETTINGCONNECTION   : if(diostream->enumservers)
                                                                  {
                                                                    if(diostream->enumservers->GetXTimer()) diostream->enumservers->GetXTimer()->AddSeconds(DIOSTREAMTCPIP_TIMEOUTENUMSERVERSIGNAL);
                                                                  }

                                                                if(diostream->config->IsServer())
                                                                  {
                                                                    diostream->config->SetCounterMultiServer(diostream->config->GetCounterMultiServer()+1);
                                                                  }
                                                                break;

              case DIOWINDOWSTCPIPFSMSTATE_CONNECTED          : { DIOSTREAM_XEVENT xevent(diostream, DIOSTREAM_XEVENT_TYPE_CONNECTED);
                                                                  xevent.SetDIOStream(diostream);
                                                                  diostream->PostEvent(&xevent);

                                                                  diostream->SetEvent(DIOWINDOWSTCPIPFSMEVENT_WAITINGTOREAD);
                                                                }
                                                                break;

              case DIOWINDOWSTCPIPFSMSTATE_WAITINGTOREAD      : break;

              case DIOWINDOWSTCPIPFSMSTATE_SENDINGDATA        : break;

              case DIOWINDOWSTCPIPFSMSTATE_DISCONNECTING      : { DIOSTREAM_XEVENT xevent(diostream, DIOSTREAM_XEVENT_TYPE_DISCONNECTED);
                                                                  xevent.SetDIOStream(diostream);
                                                                  diostream->PostEvent(&xevent);

                                                                  diostream->threadconnection->Run(false);
                                                                  diostream->status       = DIOSTREAMSTATUS_DISCONNECTED;
                                                                }
                                                                break;

            }
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWINDOWSSTREAMTCPIP::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void  DIOWINDOWSSTREAMTCPIP::Clean()
{
  threadconnection  = NULL;
  handlesocket      = INVALID_SOCKET;
}
