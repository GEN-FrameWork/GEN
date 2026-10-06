/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOLINUXStreamTCPIP.cpp
* 
* @class      DIOLINUXSTREAMTCPIP
* @brief      LINUX Data Input/Output Stream TCP/IP class
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

#include "DIOLINUXStreamTCPIP.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <malloc.h>
#include <syslog.h>
#include <signal.h>

#include <sys/socket.h>
#include <sys/un.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/ioctl.h>
#ifdef LINUX
#include <sys/fcntl.h>
#endif
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <net/if.h>
#include <netdb.h>
#include <linux/if_ether.h>

#include "XFactory.h"
#include "XTimer.h"
#include "XBuffer.h"
#include "XThread.h"
#include "XTrace.h"
#include "XString.h"
#include "XTrace.h"

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
* @fn         DIOLINUXSTREAMTCPIP::DIOLINUXSTREAMTCPIP() : DIOSTREAMTCPIP() , XFSMACHINE(0)
* @brief      Constructor of class
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOLINUXSTREAMTCPIP::DIOLINUXSTREAMTCPIP() : DIOSTREAMTCPIP() , XFSMACHINE(0)
{
  Clean();

  AddState( DIOLINUXTCPIPFSMSTATE_NONE                ,
            DIOLINUXTCPIPFSMEVENT_GETTINGCONNECTION   , DIOLINUXTCPIPFSMSTATE_GETTINGCONNECTION ,
            DIOLINUXTCPIPFSMEVENT_CONNECTED           , DIOLINUXTCPIPFSMSTATE_CONNECTED        ,
            DIOLINUXTCPIPFSMEVENT_DISCONNECTING       , DIOLINUXTCPIPFSMSTATE_DISCONNECTING    ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOLINUXTCPIPFSMSTATE_GETTINGCONNECTION   ,
            DIOLINUXTCPIPFSMEVENT_CONNECTED           , DIOLINUXTCPIPFSMSTATE_CONNECTED        ,
            DIOLINUXTCPIPFSMEVENT_WAITINGTOREAD       , DIOLINUXTCPIPFSMSTATE_WAITINGTOREAD    ,
            DIOLINUXTCPIPFSMEVENT_SENDINGDATA         , DIOLINUXTCPIPFSMSTATE_SENDINGDATA      ,
            DIOLINUXTCPIPFSMEVENT_DISCONNECTING       , DIOLINUXTCPIPFSMSTATE_DISCONNECTING    ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOLINUXTCPIPFSMSTATE_CONNECTED           ,
            DIOLINUXTCPIPFSMEVENT_GETTINGCONNECTION   , DIOLINUXTCPIPFSMSTATE_GETTINGCONNECTION ,
            DIOLINUXTCPIPFSMEVENT_WAITINGTOREAD       , DIOLINUXTCPIPFSMSTATE_WAITINGTOREAD    ,
            DIOLINUXTCPIPFSMEVENT_SENDINGDATA         , DIOLINUXTCPIPFSMSTATE_SENDINGDATA      ,
            DIOLINUXTCPIPFSMEVENT_DISCONNECTING       , DIOLINUXTCPIPFSMSTATE_DISCONNECTING    ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOLINUXTCPIPFSMSTATE_WAITINGTOREAD       ,
            DIOLINUXTCPIPFSMEVENT_GETTINGCONNECTION   , DIOLINUXTCPIPFSMSTATE_GETTINGCONNECTION ,
            DIOLINUXTCPIPFSMEVENT_CONNECTED           , DIOLINUXTCPIPFSMSTATE_CONNECTED        ,
            DIOLINUXTCPIPFSMEVENT_SENDINGDATA         , DIOLINUXTCPIPFSMSTATE_SENDINGDATA      ,
            DIOLINUXTCPIPFSMEVENT_DISCONNECTING       , DIOLINUXTCPIPFSMSTATE_DISCONNECTING    ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOLINUXTCPIPFSMSTATE_DISCONNECTING       ,
            DIOLINUXTCPIPFSMEVENT_GETTINGCONNECTION   , DIOLINUXTCPIPFSMSTATE_GETTINGCONNECTION ,
            DIOLINUXTCPIPFSMEVENT_CONNECTED           , DIOLINUXTCPIPFSMSTATE_CONNECTED        ,
            DIOLINUXTCPIPFSMEVENT_WAITINGTOREAD       , DIOLINUXTCPIPFSMSTATE_WAITINGTOREAD    ,
            DIOLINUXTCPIPFSMEVENT_SENDINGDATA         , DIOLINUXTCPIPFSMSTATE_SENDINGDATA      ,
            XFSMACHINESTATE_EVENTDEFEND);

  threadconnection = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMTCPIP, _L("DIOLINUXSTREAMTCPIP::DIOLINUXSTREAMTCPIP"), ThreadConnection, (void*)this);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOLINUXSTREAMTCPIP::~DIOLINUXSTREAMTCPIP()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOLINUXSTREAMTCPIP::~DIOLINUXSTREAMTCPIP()
{
  if(threadconnection)
    {
      DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMTCPIP, threadconnection);
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXSTREAMTCPIP::Open()
* @brief      Open
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXSTREAMTCPIP::Open()
{
  if(!threadconnection)  return false;

  if(!inbuffer)         return false;
  if(!outbuffer)        return false;

  SetEvent(DIOLINUXTCPIPFSMEVENT_GETTINGCONNECTION);
  status = DIOSTREAMSTATUS_GETTINGCONNECTION;

  ResetXBuffers();
  ResetConnectionStatistics();

  return threadconnection->Ini();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXSTREAMTCPIP::Disconnect()
* @brief      Disconnect
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXSTREAMTCPIP::Disconnect()
{
  XTIMER* timerout=GEN_XFACTORY.CreateTimer();
  if(!timerout) return false;

  if((GetStatus() == DIOSTREAMSTATUS_CONNECTED)           ||
     (GetStatus() == DIOSTREAMSTATUS_GETTINGCONNECTION))
     {
       while(GetStatus() != DIOSTREAMSTATUS_DISCONNECTED)
        {
          SetEvent(DIOLINUXTCPIPFSMEVENT_DISCONNECTING);
          usleep(10000);

          if(timerout->GetMeasureSeconds() > 3)
            {
              break;
            }

          if(threadconnection)
            {
              if(!threadconnection->IsRunning())
                {
                  break;
                }
            }
        }
     }

  GEN_XFACTORY.DeleteTimer(timerout);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXSTREAMTCPIP::Close()
* @brief      Close
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXSTREAMTCPIP::Close()
{
  if(!threadconnection)  return false;

  Disconnect();

  threadconnection->End();

  if(config->IsServer())
    {
      int handleserver = (int)config->GetHandleMultiServer();

      if(handleserver != -1)
        {
          config->SetCounterMultiServer(config->GetCounterMultiServer()-1);

          if(config->GetCounterMultiServer() <= 0)
            {
              close(handleserver);

              if(config->GetXMutexMultiServer())
                {
                  config->GetXMutexMultiServer()->Lock();
                  config->SetHandleMultiServer(-1);
                  config->GetXMutexMultiServer()->UnLock();

                  //XTRACE_PRINTCOLOR(2, _L("TCPIP: Delete Handle BASE %d Port %d"), handleserver, config->GetRemotePort());
                }
            }
        }
    }

  if(handlesocket != -1)
    {
      close(handlesocket);
      handlesocket  = -1;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOLINUXSTREAMTCPIP::GetHandleSocket()
* @brief      Get handle socket
* @ingroup    PLATFORM_LINUX
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOLINUXSTREAMTCPIP::GetHandleSocket()
{
  return handlesocket;
}

 
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOLINUXSTREAMTCPIP::SetHandleSocket(int handlesocket)
* @brief      Set handle socket
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  handlesocket : Handlesocket value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOLINUXSTREAMTCPIP::SetHandleSocket(int handlesocket)
{
  this->handlesocket = handlesocket;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOLINUXSTREAMTCPIP::Accept(int socket, void* addr, void* addrlen, XDWORD usec)
* @brief      Accept
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  socket : Socket value.
* @param[in]  addr : Addr pointer to use.
* @param[in]  addrlen : Addrlen pointer to use.
* @param[in]  usec : Usec value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOLINUXSTREAMTCPIP::Accept(int socket, void* addr, void* addrlen, XDWORD usec)
{
  fd_set         fds;
  struct timeval time_out;
  int            status;

  if(socket==-1) return -1;

  FD_ZERO(&fds);
  FD_SET((unsigned int)socket, &fds);

  time_out.tv_sec  = 0;
  time_out.tv_usec = usec;

  status = (int)select((int)(socket)+1, &fds, NULL, NULL, &time_out);
  if(status>0)
         status = accept(socket, (struct sockaddr*)addr, (socklen_t*)addrlen);
    else status = -1;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOLINUXSTREAMTCPIP::IsReadyConnect(int socket)
* @brief      Is ready connect
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  socket : Socket value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOLINUXSTREAMTCPIP::IsReadyConnect(int socket)
{
  if(socket==-1)
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

  FD_SET((unsigned int)socket, &fdr);
  FD_SET((unsigned int)socket, &fdw);
  FD_SET((unsigned int)socket, &fds);

  tv.tv_sec  = 0;
  tv.tv_usec = 100;

  rc = select((int)(socket)+1, &fdr, &fdw, &fds, &tv);
  if(rc == -1)
    {
      SetLastDIOError(DIOSTREAMERROR_TCPCONNECTION);
      return -1;
    }

  int status1 = FD_ISSET(socket, &fdr) ? 1 : 0;
  int status2 = FD_ISSET(socket, &fdw) ? 1 : 0;
  int status3 = FD_ISSET(socket, &fds) ? 1 : 0;

  if(config->IsServer())
    {
      if(status1 || status2 )   return  1;
    }
   else
    {
      if(status1 || status2)
        {
          int       optval;
          socklen_t optlen = sizeof(optval);

          if(getsockopt(socket, SOL_SOCKET, SO_ERROR, &optval, &optlen) < 0 )
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
* @fn         bool DIOLINUXSTREAMTCPIP::GetHandleServer()
* @brief      Get handle server
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXSTREAMTCPIP::GetHandleServer()
{
  int                 handleserver = (int)config->GetHandleMultiServer();
  struct sockaddr_in  loc_addr     = { 0 };
  struct sockaddr_in  rem_addr     = { 0 };
  XSTRING             IPstring;

  if(!config->GetXMutexMultiServer())
    {
      return false;
    }

  config->GetXMutexMultiServer()->Lock();

  if(handleserver < 0)
    {
      handleserver = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
      if(handleserver < 0)
        {
          SetEvent(DIOLINUXTCPIPFSMEVENT_DISCONNECTING);
          SetLastDIOError(DIOSTREAMERROR_UNKNOWN);
          config->GetXMutexMultiServer()->UnLock();

          return false;
        }

      int opt = 1;
      if(setsockopt(handleserver, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        {
          SetEvent(DIOLINUXTCPIPFSMEVENT_DISCONNECTING);
          SetLastDIOError(DIOSTREAMERROR_UNKNOWN);
          config->GetXMutexMultiServer()->UnLock();

          return false;
        }

      #ifdef SO_REUSEPORT
      opt = 1;
      if(setsockopt(handleserver, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) < 0)
        {
          SetEvent(DIOLINUXTCPIPFSMEVENT_DISCONNECTING);
          SetLastDIOError(DIOSTREAMERROR_UNKNOWN);
          config->GetXMutexMultiServer()->UnLock();

          return false;
        }
      #endif

      config->GetLocalIP()->GetXString(IPstring);

      loc_addr.sin_family = AF_INET;
      if(!config->GetLocalIP()->IsEmpty())
        {          
          XBUFFER charstr;
          
          IPstring.ConvertToASCII(charstr);
          loc_addr.sin_addr.s_addr  = inet_addr(charstr.GetPtrChar());
          
        } else loc_addr.sin_addr.s_addr = htonl(INADDR_ANY);

      loc_addr.sin_port = htons(config->GetRemotePort());

      if(bind(handleserver, (struct sockaddr *)&loc_addr, sizeof(loc_addr)) < 0)
        {
          if(errno == EADDRINUSE)
            {
              SetLastDIOError(DIOSTREAMERROR_ADDRINUSE);

            } else SetLastDIOError(DIOSTREAMERROR_UNKNOWN);

          SetEvent(DIOLINUXTCPIPFSMEVENT_DISCONNECTING);

          config->GetXMutexMultiServer()->UnLock();

          return false;
        }

      if(listen(handleserver, DIOSTREAMTCPIP_MAXLISTEN) < 0)
        {
          SetEvent(DIOLINUXTCPIPFSMEVENT_DISCONNECTING);
          SetLastDIOError(DIOSTREAMERROR_UNKNOWN);
          config->GetXMutexMultiServer()->UnLock();

          return false;
        }

      fcntl(handleserver, F_SETFL, fcntl(handleserver, F_GETFL, 0) | O_NONBLOCK);

      config->SetHandleMultiServer(handleserver);
    }

  config->GetXMutexMultiServer()->UnLock();

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

  socklen_t sizeaddr = sizeof(struct sockaddr_in);
  handlesocket = Accept(handleserver, (struct sockaddr*)&rem_addr, &sizeaddr, 100/*50*1000*/);
  if(handlesocket >= 0)
    {
      int opt = 1;
      if(setsockopt(handlesocket, SOL_SOCKET, SO_KEEPALIVE , &opt, sizeof(opt)) < 0)
        {
          SetEvent(DIOLINUXTCPIPFSMEVENT_DISCONNECTING);
          SetLastDIOError(DIOSTREAMERROR_UNKNOWN);

          return false;
        }

      int flags = fcntl(handlesocket, F_GETFL, 0);
      fcntl(handlesocket, F_SETFL, flags | O_NONBLOCK);

      clientIP.Set(inet_ntoa(rem_addr.sin_addr));
    }
   else
    {
      if(errno == EADDRINUSE)
        {
          SetEvent(DIOLINUXTCPIPFSMEVENT_DISCONNECTING);
          SetLastDIOError(DIOSTREAMERROR_ADDRINUSE);

          return false;
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXSTREAMTCPIP::GetHandleClient()
* @brief      Get handle client
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXSTREAMTCPIP::GetHandleClient()
{
  if(config->GetConnectionURL()->IsEmpty())
    {
      SetEvent(DIOLINUXTCPIPFSMEVENT_DISCONNECTING);
      SetLastDIOError(DIOSTREAMERROR_URLNOTVALID);
      return false;
    }

  handlesocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(handlesocket == -1)
    {
      SetEvent(DIOLINUXTCPIPFSMEVENT_DISCONNECTING);
      SetLastDIOError(DIOSTREAMERROR_TCPCONNECTION);
      return false;
    }

  if(!config->GetLocalIP()->IsEmpty())
    {
      struct sockaddr_in loc_addr;
      XSTRING            IPstring;

      config->GetLocalIP()->GetXString(IPstring);

      memset(&loc_addr, 0, sizeof(struct sockaddr_in));

      loc_addr.sin_family      = AF_INET;

      XBUFFER charstr;
      
      IPstring.ConvertToASCII(charstr);      
      loc_addr.sin_addr.s_addr = inet_addr(charstr.GetPtrChar());      
      loc_addr.sin_port        = 0; //htons(diostream->config->GetRemotePort());

      if(bind(handlesocket, (struct sockaddr *)&loc_addr, sizeof(loc_addr)) < 0 )
        {
          SetEvent(DIOLINUXTCPIPFSMEVENT_DISCONNECTING);
          SetLastDIOError(DIOSTREAMERROR_TCPCONNECTION);
          return false;
        }
    }

  struct sockaddr_in  rem_addr;
  DIOIP               remoteIP;
  XSTRING             IPstring;

  memset(&rem_addr, 0, sizeof(struct sockaddr_in));

  if(!config->GetConnectionURL()->ResolveURL(remoteIP))
    {
      SetEvent(DIOLINUXTCPIPFSMEVENT_DISCONNECTING);
      SetLastDIOError(DIOSTREAMERROR_DNS);
      return false;
    }

  remoteIP.GetXString(IPstring);

  rem_addr.sin_family       = AF_INET;
  
  XBUFFER charstr;
  
  IPstring.ConvertToASCII(charstr);  
  rem_addr.sin_addr.s_addr  = inet_addr(charstr.GetPtrChar());
  clientIP.Set(charstr.GetPtrChar());  
  rem_addr.sin_port         = htons(config->GetConnectionPort());

  int opt = 1;
  if(setsockopt(handlesocket, SOL_SOCKET, SO_KEEPALIVE , &opt, sizeof(opt)) < 0)
    {
      SetEvent(DIOLINUXTCPIPFSMEVENT_DISCONNECTING);
      SetLastDIOError(DIOSTREAMERROR_TCPCONNECTION);
      return false;
    }

  int flags = fcntl(handlesocket, F_GETFL, 0);
  fcntl(handlesocket, F_SETFL, flags | O_NONBLOCK);

  if(connect(handlesocket, (struct sockaddr *)&rem_addr, sizeof(rem_addr)) < 0)
    {
      if((errno != EINPROGRESS) && (errno != EWOULDBLOCK) && (errno != EALREADY))
        {
          SetEvent(DIOLINUXTCPIPFSMEVENT_DISCONNECTING);
          SetLastDIOError((errno == EADDRINUSE)?DIOSTREAMERROR_ADDRINUSE:DIOSTREAMERROR_TCPCONNECTION);
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
* @fn         void DIOLINUXSTREAMTCPIP::ThreadConnection(void* data)
* @brief      Thread connection
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  data : Data buffer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOLINUXSTREAMTCPIP::ThreadConnection(void* data)
{
  DIOLINUXSTREAMTCPIP* diostream = (DIOLINUXSTREAMTCPIP*)data;
  if(!diostream) return;

  if(diostream->GetEvent()==DIOLINUXTCPIPFSMEVENT_NONE) // No hay nuevos Eventos
    {
      switch(diostream->GetCurrentState())
        {
          case DIOLINUXTCPIPFSMSTATE_NONE               : break;

          case DIOLINUXTCPIPFSMSTATE_GETTINGCONNECTION   : if(diostream->handlesocket == -1)
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
                                                                    case -1:  diostream->SetEvent(DIOLINUXTCPIPFSMEVENT_DISCONNECTING);
                                                                              break;

                                                                    case  1:  diostream->SetEvent(DIOLINUXTCPIPFSMEVENT_CONNECTED);
                                                                              diostream->status = DIOSTREAMSTATUS_CONNECTED;
                                                                              break;

                                                                    default:  diostream->status = DIOSTREAMSTATUS_GETTINGCONNECTION;
                                                                              break;
                                                                  }
                                                              }
                                                            break;

          case DIOLINUXTCPIPFSMSTATE_CONNECTED          : break;

          case DIOLINUXTCPIPFSMSTATE_WAITINGTOREAD      : { struct timeval  waitd;
                                                            fd_set          read_flags;
                                                            fd_set          write_flags;

                                                            if(diostream->handlesocket==-1)
                                                              {
                                                                diostream->SetEvent(DIOLINUXTCPIPFSMEVENT_DISCONNECTING);
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
                                                            if(error == -1)   //Socket ready for reading
                                                              {
                                                                diostream->SetEvent(DIOLINUXTCPIPFSMEVENT_DISCONNECTING);
                                                                break;
                                                              }

                                                            if(FD_ISSET(diostream->handlesocket, &read_flags))  //Socket ready for reading
                                                              {
                                                                XBYTE buffer[DIOSTREAM_MAXBUFFER];

                                                                FD_CLR(diostream->handlesocket, &read_flags);

                                                                XDWORD size = recv(diostream->handlesocket, (char*)buffer, DIOSTREAM_MAXBUFFER, 0);
                                                                if(size==-1)
                                                                  {
                                                                    diostream->SetEvent(DIOLINUXTCPIPFSMEVENT_DISCONNECTING);
                                                                    break;
                                                                  }

                                                                if(size)
                                                                  {
                                                                    diostream->inbuffer->Add(buffer, size);
                                                                  }
                                                                 else
                                                                  {
                                                                    diostream->SetEvent(DIOLINUXTCPIPFSMEVENT_DISCONNECTING);
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

                                                                    if(size == -1)
                                                                      {
                                                                        diostream->SetEvent(DIOLINUXTCPIPFSMEVENT_DISCONNECTING);
                                                                        break;
                                                                      }

                                                                    if(size) diostream->outbuffer->Extract(NULL, 0, size);
                                                                  }
                                                              }
                                                          }
                                                          break;

          case DIOLINUXTCPIPFSMSTATE_SENDINGDATA        : break;

          case DIOLINUXTCPIPFSMSTATE_DISCONNECTING      : break;

        }
    }
   else
    {
      if(diostream->GetEvent()<DIOLINUXTCPIP_LASTEVENT)
        {
          diostream->CheckTransition();

          switch(diostream->GetCurrentState())
            {
              case DIOLINUXTCPIPFSMSTATE_NONE               : break;

              case DIOLINUXTCPIPFSMSTATE_GETTINGCONNECTION   : if(diostream->enumservers)
                                                                {
                                                                  if(diostream->enumservers->GetXTimer()) diostream->enumservers->GetXTimer()->AddSeconds(DIOSTREAMTCPIP_TIMEOUTENUMSERVERSIGNAL);
                                                                }

                                                              if(diostream->config->IsServer())
                                                                {
                                                                  diostream->config->SetCounterMultiServer(diostream->config->GetCounterMultiServer()+1);
                                                                }
                                                              break;

              case DIOLINUXTCPIPFSMSTATE_CONNECTED          : { DIOSTREAM_XEVENT xevent(diostream, DIOSTREAM_XEVENT_TYPE_CONNECTED);
                                                                xevent.SetDIOStream(diostream);
                                                                diostream->PostEvent(&xevent);

                                                                diostream->SetEvent(DIOLINUXTCPIPFSMEVENT_WAITINGTOREAD);
                                                              }
                                                              break;

              case DIOLINUXTCPIPFSMSTATE_WAITINGTOREAD      : break;

              case DIOLINUXTCPIPFSMSTATE_SENDINGDATA        : break;

              case DIOLINUXTCPIPFSMSTATE_DISCONNECTING      : { DIOSTREAM_XEVENT xevent(diostream, DIOSTREAM_XEVENT_TYPE_DISCONNECTED);
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
* @fn         void DIOLINUXSTREAMTCPIP::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOLINUXSTREAMTCPIP::Clean()
{
  threadconnection   = NULL;
  handlesocket      = -1;
}





