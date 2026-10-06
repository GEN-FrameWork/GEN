/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOWINDOWSStreamTCPIPServer.cpp
* 
* @class      DIOWINDOWSSTREAMTCPIPSERVER
* @brief      WINDOWS Data Input/Output Stream TCP/IP Server class
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

#include "DIOWINDOWSStreamTCPIPServer.h"

#include "DIOWINDOWSStreamTCPIP.h"

#include "DIOFactory.h"
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
* @fn         DIOWINDOWSSTREAMTCPIPSERVER::DIOWINDOWSSTREAMTCPIPSERVER() : DIOSTREAMTCPIPSERVER(), XFSMACHINE(0)
* @brief      Constructor of class
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWINDOWSSTREAMTCPIPSERVER::DIOWINDOWSSTREAMTCPIPSERVER() : DIOSTREAMTCPIPSERVER(), XFSMACHINE(0)
{
  Clean();

  AddState( DIOWINDOWSTCPIPSERVERFSMSTATE_NONE                ,
            DIOWINDOWSTCPIPSERVERFSMEVENT_GETTINGCONNECTION   , DIOWINDOWSTCPIPSERVERFSMSTATE_GETTINGCONNECTION ,
            DIOWINDOWSTCPIPSERVERFSMEVENT_CONNECTED           , DIOWINDOWSTCPIPSERVERFSMSTATE_CONNECTED         ,
            DIOWINDOWSTCPIPSERVERFSMEVENT_DISCONNECTING       , DIOWINDOWSTCPIPSERVERFSMSTATE_DISCONNECTING     ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOWINDOWSTCPIPSERVERFSMSTATE_GETTINGCONNECTION   ,
            DIOWINDOWSTCPIPSERVERFSMEVENT_CONNECTED           , DIOWINDOWSTCPIPSERVERFSMSTATE_CONNECTED         ,
            DIOWINDOWSTCPIPSERVERFSMEVENT_WAITINGTOREAD       , DIOWINDOWSTCPIPSERVERFSMSTATE_WAITINGTOREAD     ,
            DIOWINDOWSTCPIPSERVERFSMEVENT_SENDINGDATA         , DIOWINDOWSTCPIPSERVERFSMSTATE_SENDINGDATA       ,
            DIOWINDOWSTCPIPSERVERFSMEVENT_DISCONNECTING       , DIOWINDOWSTCPIPSERVERFSMSTATE_DISCONNECTING     ,

            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOWINDOWSTCPIPSERVERFSMSTATE_CONNECTED           ,
            DIOWINDOWSTCPIPSERVERFSMEVENT_GETTINGCONNECTION   , DIOWINDOWSTCPIPSERVERFSMSTATE_GETTINGCONNECTION ,
            DIOWINDOWSTCPIPSERVERFSMEVENT_WAITINGTOREAD       , DIOWINDOWSTCPIPSERVERFSMSTATE_WAITINGTOREAD     ,
            DIOWINDOWSTCPIPSERVERFSMEVENT_SENDINGDATA         , DIOWINDOWSTCPIPSERVERFSMSTATE_SENDINGDATA       ,
            DIOWINDOWSTCPIPSERVERFSMEVENT_DISCONNECTING       , DIOWINDOWSTCPIPSERVERFSMSTATE_DISCONNECTING     ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOWINDOWSTCPIPSERVERFSMSTATE_WAITINGTOREAD       ,
            DIOWINDOWSTCPIPSERVERFSMEVENT_GETTINGCONNECTION   , DIOWINDOWSTCPIPSERVERFSMSTATE_GETTINGCONNECTION ,
            DIOWINDOWSTCPIPSERVERFSMEVENT_CONNECTED           , DIOWINDOWSTCPIPSERVERFSMSTATE_CONNECTED        ,
            DIOWINDOWSTCPIPSERVERFSMEVENT_SENDINGDATA         , DIOWINDOWSTCPIPSERVERFSMSTATE_SENDINGDATA      ,
            DIOWINDOWSTCPIPSERVERFSMEVENT_DISCONNECTING       , DIOWINDOWSTCPIPSERVERFSMSTATE_DISCONNECTING    ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOWINDOWSTCPIPSERVERFSMSTATE_DISCONNECTING       ,
            DIOWINDOWSTCPIPSERVERFSMEVENT_GETTINGCONNECTION   , DIOWINDOWSTCPIPSERVERFSMSTATE_GETTINGCONNECTION ,
            DIOWINDOWSTCPIPSERVERFSMEVENT_CONNECTED           , DIOWINDOWSTCPIPSERVERFSMSTATE_CONNECTED        ,
            DIOWINDOWSTCPIPSERVERFSMEVENT_WAITINGTOREAD       , DIOWINDOWSTCPIPSERVERFSMSTATE_WAITINGTOREAD    ,
            DIOWINDOWSTCPIPSERVERFSMEVENT_SENDINGDATA         , DIOWINDOWSTCPIPSERVERFSMSTATE_SENDINGDATA      ,
            XFSMACHINESTATE_EVENTDEFEND);

  threadconnection = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMTCPIP, _L("DIOWINDOWSSTREAMTCPIPSERVER::DIOWINDOWSSTREAMTCPIPSERVER"), ThreadConnection, (void*)this);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWINDOWSSTREAMTCPIPSERVER::~DIOWINDOWSSTREAMTCPIPSERVER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWINDOWSSTREAMTCPIPSERVER::~DIOWINDOWSSTREAMTCPIPSERVER()
{
  if(threadconnection)
    {
      DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMTCPIP, threadconnection);
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMTCPIPSERVER::Open()
* @brief      Open
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMTCPIPSERVER::Open()
{  
  if(!threadconnection)  
   {
      return false;
   }

  if(!CreateStream())
    {
      return false;
    }

  return threadconnection->Ini();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMTCPIPSERVER::Disconnect()
* @brief      Disconnect
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMTCPIPSERVER::Disconnect()
{
  for(XDWORD c=0; c<GetMultiSocketStreams()->GetSize(); c++)
    {
      DIOWINDOWSSTREAMTCPIP* diostream = (DIOWINDOWSSTREAMTCPIP*)GetMultiSocketStreams()->Get(c);
      if(diostream)
        {
          if(diostream->GetStatus()!=DIOSTREAMSTATUS_DISCONNECTED)
            {
              diostream->SetEvent(DIOWINDOWSTCPIPSERVERFSMEVENT_DISCONNECTING);
              GEN_XSLEEP.MilliSeconds(10);          
            }             
        }
    }
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMTCPIPSERVER::Close()
* @brief      Close
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMTCPIPSERVER::Close()
{
  if(!threadconnection)  
    {
      return false;
    }

  threadconnection->End();

  Disconnect();

  DeleteAllStream();

  SOCKET handleserver = (SOCKET)config->GetHandleMultiServer();
  if(handleserver != INVALID_SOCKET)
    {          
      closesocket(handleserver);

      if(config->GetXMutexMultiServer())
        {
          config->GetXMutexMultiServer()->Lock();
          config->SetHandleMultiServer((int)INVALID_SOCKET);
          config->GetXMutexMultiServer()->UnLock();
        }
    }
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SOCKET DIOWINDOWSSTREAMTCPIPSERVER::Accept(SOCKET handlesocket, void* addr, void* addrlen, XDWORD usec)
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
SOCKET DIOWINDOWSSTREAMTCPIPSERVER::Accept(SOCKET handlesocket, void* addr, void* addrlen, XDWORD usec)
{
  fd_set         fds;
  struct timeval time_out;
  SOCKET         status;

  if(handlesocket==INVALID_SOCKET) 
    {
      return INVALID_SOCKET;
    }

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
* @fn         int DIOWINDOWSSTREAMTCPIPSERVER::IsReadyConnect(SOCKET handlesocket)
* @brief      Is ready connect
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  handlesocket : Handlesocket value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOWINDOWSSTREAMTCPIPSERVER::IsReadyConnect(SOCKET handlesocket)
{
  if(handlesocket==INVALID_SOCKET) 
    {
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
      return -1;
    }

  int status1 = FD_ISSET(handlesocket, &fdr) ? 1 : 0;
  int status2 = FD_ISSET(handlesocket, &fdw) ? 1 : 0;
  int status3 = FD_ISSET(handlesocket, &fds) ? 1 : 0;

  if(status1 || status2 )  
    {
      return  1;
    }
  
  if(status3) 
    {
      return -1;
    }

  return 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAM* DIOWINDOWSSTREAMTCPIPSERVER::CreateStream()
* @brief      Create stream
* @ingroup    PLATFORM_WINDOWS
* 
* @return     DIOSTREAM* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAM* DIOWINDOWSSTREAMTCPIPSERVER::CreateStream()
{
  DIOSTREAMTCPIPCONFIG* _config = NULL;

  _config = GEN_NEW DIOSTREAMTCPIPCONFIG();
  if(_config)
    {
      _config->SetMode(DIOSTREAMMODE_SERVER);
      _config->SetRemotePort(config->GetRemotePort());
    }  

  DIOWINDOWSSTREAMTCPIP* diostream =  GEN_NEW DIOWINDOWSSTREAMTCPIP();
  if(diostream)
    {      
      diostream->SetConfig(_config);

      diostream->SetEvent(DIOWINDOWSTCPIPSERVERFSMEVENT_GETTINGCONNECTION);
      diostream->SetStatus(DIOSTREAMSTATUS_GETTINGCONNECTION);

      diostream->ResetXBuffers();
      diostream->ResetConnectionStatistics(); 
     
      multisocketstreams.Add(diostream);     
    }

  return diostream;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMTCPIPSERVER::DeleteAllStreamDisconnected()
* @brief      Delete all stream disconnected
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMTCPIPSERVER::DeleteAllStreamDisconnected()
{
  XDWORD index = 0;

  while(index < GetMultiSocketStreams()->GetSize())
    {
      DIOWINDOWSSTREAMTCPIP* diostream = (DIOWINDOWSSTREAMTCPIP*)GetMultiSocketStreams()->Get(index);
      if(diostream)
        {
          if(diostream->GetStatus() == DIOSTREAMSTATUS_DISCONNECTED)
            {
              if(diostream->GetConfig())
                {
                  GEN_DELETE diostream->GetConfig();
                }

              diostream->Close();
              GEN_DELETE diostream;
              GetMultiSocketStreams()->Delete(diostream);    

              continue;
            }
        }

      index++;
    }  

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMTCPIPSERVER::DeleteAllStream()
* @brief      Delete all stream
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMTCPIPSERVER::DeleteAllStream()
{
  XDWORD index = 0;

  while(index < GetMultiSocketStreams()->GetSize())
    {
      DIOWINDOWSSTREAMTCPIP* diostream = (DIOWINDOWSSTREAMTCPIP*)GetMultiSocketStreams()->Get(index);
      if(diostream)
        {
          if(diostream->GetConfig())
            {
              GEN_DELETE diostream->GetConfig();
            }

          GEN_DELETE diostream;
          GetMultiSocketStreams()->Delete(diostream);    

          continue;          
        }

      index++;
    }  

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMTCPIPSERVER::GetHandleServer(DIOSTREAMTCPIP* diostream)
* @brief      Get handle server
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  diostream : Diostream pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMTCPIPSERVER::GetHandleServer(DIOSTREAMTCPIP* diostream)
{  
  SOCKET       handleserver = (SOCKET)config->GetHandleMultiServer();
  SOCKADDR_IN  loc_addr;
  SOCKADDR_IN  rem_addr;
  XSTRING      IPstring;

  // ------------------------------------------------------------------------------------------  

  memset(&loc_addr, 0, sizeof(SOCKADDR_IN));
  memset(&rem_addr, 0, sizeof(SOCKADDR_IN));

  if(!diostream)
    {
      return false;
    }

  // ------------------------------------------------------------------------------------------  

  if(handleserver == INVALID_SOCKET)
    {
      config->GetXMutexMultiServer()->Lock();

      handleserver = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
      if(handleserver == INVALID_SOCKET)
        {
          return false;
        }

      int opt = 1;
      if(setsockopt(handleserver, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) == SOCKET_ERROR)
        {
          SetEvent(DIOWINDOWSTCPIPSERVERFSMEVENT_DISCONNECTING);
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

          SetEvent(DIOWINDOWSTCPIPSERVERFSMEVENT_DISCONNECTING);

          config->GetXMutexMultiServer()->UnLock();
          return false;
        }

      if(listen(handleserver, DIOSTREAMTCPIP_MAXLISTEN) == SOCKET_ERROR)
        {
          SetEvent(DIOWINDOWSTCPIPSERVERFSMEVENT_DISCONNECTING);
          SetLastDIOError(DIOSTREAMERROR_UNKNOWN);

          config->GetXMutexMultiServer()->UnLock();
          return false;
        }

      DWORD nonblock = 1;
      ioctlsocket(handleserver, FIONBIO, &nonblock);

      config->SetHandleMultiServer((int)handleserver);

      config->GetXMutexMultiServer()->UnLock();
    }

  // ------------------------------------------------------------------------------------------  

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

  // ------------------------------------------------------------------------------------------  

  
  XDWORD sizeaddr     = sizeof(SOCKADDR_IN);
  SOCKET handlesocket = Accept(handleserver, (SOCKADDR*)&rem_addr, &sizeaddr, 50*1000);
  if(handlesocket != INVALID_SOCKET)
    {
      int opt = 1;
      if(setsockopt(handlesocket, SOL_SOCKET, SO_KEEPALIVE, (char*)&opt, sizeof(opt)) == SOCKET_ERROR)
        {
          SetEvent(DIOWINDOWSTCPIPSERVERFSMEVENT_DISCONNECTING);
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
          SetEvent(DIOWINDOWSTCPIPSERVERFSMEVENT_DISCONNECTING);
          SetLastDIOError(DIOSTREAMERROR_ADDRINUSE);
          return false;
        }
    }
  
  DIOWINDOWSSTREAMTCPIP* diowindowsstreamtcpip = (DIOWINDOWSSTREAMTCPIP*)diostream;
  if(diowindowsstreamtcpip)
    {
      diowindowsstreamtcpip->SetHandleSocket(handlesocket);
    }

  // ------------------------------------------------------------------------------------------  

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWINDOWSSTREAMTCPIPSERVER::ThreadConnection(void* data)
* @brief      Thread connection
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  data : Data buffer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWINDOWSSTREAMTCPIPSERVER::ThreadConnection(void* data)
{  
  DIOWINDOWSSTREAMTCPIPSERVER* diostreamserver = (DIOWINDOWSSTREAMTCPIPSERVER*)data;
  if(!diostreamserver) 
    {
      return;
    }

  diostreamserver->DeleteAllStreamDisconnected();

  for(XDWORD c=0; c<diostreamserver->GetMultiSocketStreams()->GetSize(); c++)
    {
      DIOWINDOWSSTREAMTCPIP* diostream = (DIOWINDOWSSTREAMTCPIP*)diostreamserver->GetMultiSocketStreams()->Get(c);
      if(diostream) 
        {
          if(diostream->GetEvent()==DIOWINDOWSTCPIPSERVERFSMEVENT_NONE) // No hay nuevos Eventos
            {
              switch(diostream->GetCurrentState())
                {
                  case DIOWINDOWSTCPIPSERVERFSMSTATE_NONE                 : break;

                  case DIOWINDOWSTCPIPSERVERFSMSTATE_GETTINGCONNECTION    : if(diostream->GetHandleSocket() == INVALID_SOCKET)
                                                                              {
                                                                                 diostreamserver->GetHandleServer(diostream);                                                                               
                                                                              }
                                                                             else
                                                                              {
                                                                                switch(diostreamserver->IsReadyConnect(diostream->GetHandleSocket()))
                                                                                  {
                                                                                    case -1:  diostream->SetEvent(DIOWINDOWSTCPIPSERVERFSMEVENT_DISCONNECTING);
                                                                                              break;

                                                                                    case  1:  diostream->SetEvent(DIOWINDOWSTCPIPSERVERFSMEVENT_CONNECTED);
                                                                                              diostream->SetStatus(DIOSTREAMSTATUS_CONNECTED);
                                                                                              break;

                                                                                    default:  diostream->SetStatus(DIOSTREAMSTATUS_GETTINGCONNECTION);
                                                                                              break;
                                                                                  }
                                                                              }
                                                                            break;

                  case DIOWINDOWSTCPIPSERVERFSMSTATE_CONNECTED          : break;

                  case DIOWINDOWSTCPIPSERVERFSMSTATE_WAITINGTOREAD      : { struct timeval  waitd;
                                                                            fd_set          read_flags;
                                                                            fd_set          write_flags;
                                                              
                                                                            if(diostream->GetHandleSocket() == INVALID_SOCKET)
                                                                              {
                                                                                diostream->SetEvent(DIOWINDOWSTCPIPSERVERFSMEVENT_DISCONNECTING);
                                                                                break;
                                                                              }

                                                                            waitd.tv_sec  = 0;
                                                                            waitd.tv_usec = 100;

                                                                            FD_ZERO(&read_flags);
                                                                            FD_ZERO(&write_flags);

                                                                            if(!diostream->IsBlockRead()) FD_SET(diostream->GetHandleSocket(), &read_flags);
                                                                            if(!diostream->IsBlockWrite())
                                                                              {
                                                                                if(diostream->outbuffer->GetSize()) FD_SET(diostream->GetHandleSocket(), &write_flags);
                                                                              }

                                                                            int error = select((int)(diostream->GetHandleSocket())+1, &read_flags, &write_flags, (fd_set*)NULL, &waitd);
                                                                            if(error == SOCKET_ERROR)   //Socket ready for reading
                                                                              {
                                                                                diostream->SetEvent(DIOWINDOWSTCPIPSERVERFSMEVENT_DISCONNECTING);
                                                                                break;
                                                                              }

                                                                            if(FD_ISSET(diostream->GetHandleSocket(), &read_flags))  //Socket ready for reading
                                                                              {
                                                                                XBYTE buffer[DIOSTREAM_MAXBUFFER];

                                                                                FD_CLR(diostream->GetHandleSocket(), &read_flags);

                                                                                memset(buffer, 0, DIOSTREAM_MAXBUFFER);

                                                                                XDWORD size = recv(diostream->GetHandleSocket(), (char*)buffer, DIOSTREAM_MAXBUFFER, 0);
                                                                                if(size==SOCKET_ERROR)
                                                                                  {
                                                                                    diostream->SetEvent(DIOWINDOWSTCPIPSERVERFSMEVENT_DISCONNECTING);
                                                                                    break;
                                                                                  }

                                                                                if(size)
                                                                                  {
                                                                                    diostream->inbuffer->Add(buffer, size);

                                                                                    diostream->AddNBytesRead(size);
                                                                                    diostreamserver->nbytesread += size;
                                                                                  }
                                                                                 else
                                                                                  {
                                                                                    diostream->SetEvent(DIOWINDOWSTCPIPSERVERFSMEVENT_DISCONNECTING);
                                                                                    break;
                                                                                  }
                                                                              }

                                                                            if(FD_ISSET(diostream->GetHandleSocket(), &write_flags))  //Socket ready for writing
                                                                              {
                                                                                FD_CLR(diostream->GetHandleSocket(), &write_flags);

                                                                                int esize = diostream->outbuffer->GetSize();
                                                                                if(esize>DIOSTREAM_MAXBUFFER) esize = DIOSTREAM_MAXBUFFER;

                                                                                if(esize)
                                                                                  {
                                                                                    diostream->outbuffer->SetBlocked(true);
                                                                                    XDWORD size = send(diostream->GetHandleSocket(), (char*)diostream->outbuffer->Get(), esize, 0);
                                                                                    diostream->outbuffer->SetBlocked(false);
                                                                                    if(size==SOCKET_ERROR)
                                                                                      {
                                                                                        diostream->SetEvent(DIOWINDOWSTCPIPSERVERFSMEVENT_DISCONNECTING);
                                                                                        break;
                                                                                      }

                                                                                    if(size)
                                                                                      {
                                                                                        diostream->outbuffer->Extract(NULL, 0, size);

                                                                                        diostream->AddNBytesWrite(size);
                                                                                        diostreamserver->nbyteswrite += size;
                                                                                      }
                                                                                  }
                                                                              }
                                                                          }
                                                                          break;

                  case DIOWINDOWSTCPIPSERVERFSMSTATE_SENDINGDATA        : break;

                  case DIOWINDOWSTCPIPSERVERFSMSTATE_DISCONNECTING      : break;

                }
            }
           else
            {
              if(diostream->GetEvent()<DIOWINDOWSTCPIPSERVER_LASTEVENT)
                {
                  diostream->CheckTransition();

                  switch(diostream->GetCurrentState())
                    {
                      case DIOWINDOWSTCPIPSERVERFSMSTATE_NONE                 : break;

                      case DIOWINDOWSTCPIPSERVERFSMSTATE_GETTINGCONNECTION    : if(diostreamserver->enumservers)
                                                                                  {
                                                                                    if(diostreamserver->enumservers->GetXTimer()) 
                                                                                      {
                                                                                        diostreamserver->enumservers->GetXTimer()->AddSeconds(DIOSTREAMTCPIP_TIMEOUTENUMSERVERSIGNAL);
                                                                                      }
                                                                                  }                                                                                
                                                                                break;

                      case DIOWINDOWSTCPIPSERVERFSMSTATE_CONNECTED            : { DIOSTREAM_XEVENT xevent(diostream, DIOSTREAM_XEVENT_TYPE_CONNECTED);
                                                                                  xevent.SetDIOStream(diostream);
                                                                                  diostreamserver->PostEvent(&xevent);

                                                                                  diostream->SetEvent(DIOWINDOWSTCPIPSERVERFSMEVENT_WAITINGTOREAD);

                                                                                  diostreamserver->CreateStream();                                                                              
                                                                                }
                                                                                break;

                      case DIOWINDOWSTCPIPSERVERFSMSTATE_WAITINGTOREAD        : break;

                      case DIOWINDOWSTCPIPSERVERFSMSTATE_SENDINGDATA          : break;

                      case DIOWINDOWSTCPIPSERVERFSMSTATE_DISCONNECTING        : { DIOSTREAM_XEVENT xevent(diostream, DIOSTREAM_XEVENT_TYPE_DISCONNECTED);
                                                                                  xevent.SetDIOStream(diostream);
                                                                                  diostreamserver->PostEvent(&xevent);
                                                                                  
                                                                                  diostream->SetStatus(DIOSTREAMSTATUS_DISCONNECTED);
                                                                                }
                                                                                break;

                    }
                }
            }
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWINDOWSSTREAMTCPIPSERVER::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWINDOWSSTREAMTCPIPSERVER::Clean()
{
  threadconnection   = NULL;                                
}



