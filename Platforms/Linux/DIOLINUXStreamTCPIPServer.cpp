/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOLINUXStreamTCPIPServer.cpp
* 
* @class      DIOLINUXSTREAMTCPIPSERVER
* @brief      LINUX Data Input/Output Stream TCP/IP Server class
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

#include "DIOLINUXStreamTCPIPServer.h"

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
* @fn         DIOLINUXSTREAMTCPIPSERVER::DIOLINUXSTREAMTCPIPSERVER() : DIOSTREAMTCPIPSERVER() , XFSMACHINE(0)
* @brief      Constructor of class
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOLINUXSTREAMTCPIPSERVER::DIOLINUXSTREAMTCPIPSERVER() : DIOSTREAMTCPIPSERVER() , XFSMACHINE(0)
{
  Clean();

  AddState( DIOLINUXTCPIPSERVERFSMSTATE_NONE                ,
            DIOLINUXTCPIPSERVERFSMEVENT_GETTINGCONNECTION   , DIOLINUXTCPIPSERVERFSMSTATE_GETTINGCONNECTION ,
            DIOLINUXTCPIPSERVERFSMEVENT_CONNECTED           , DIOLINUXTCPIPSERVERFSMSTATE_CONNECTED         ,
            DIOLINUXTCPIPSERVERFSMEVENT_DISCONNECTING       , DIOLINUXTCPIPSERVERFSMSTATE_DISCONNECTING     ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOLINUXTCPIPSERVERFSMSTATE_GETTINGCONNECTION   ,
            DIOLINUXTCPIPSERVERFSMEVENT_CONNECTED           , DIOLINUXTCPIPSERVERFSMSTATE_CONNECTED         ,
            DIOLINUXTCPIPSERVERFSMEVENT_WAITINGTOREAD       , DIOLINUXTCPIPSERVERFSMSTATE_WAITINGTOREAD     ,
            DIOLINUXTCPIPSERVERFSMEVENT_SENDINGDATA         , DIOLINUXTCPIPSERVERFSMSTATE_SENDINGDATA       ,
            DIOLINUXTCPIPSERVERFSMEVENT_DISCONNECTING       , DIOLINUXTCPIPSERVERFSMSTATE_DISCONNECTING     ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOLINUXTCPIPSERVERFSMSTATE_CONNECTED           ,
            DIOLINUXTCPIPSERVERFSMEVENT_GETTINGCONNECTION   , DIOLINUXTCPIPSERVERFSMSTATE_GETTINGCONNECTION ,
            DIOLINUXTCPIPSERVERFSMEVENT_WAITINGTOREAD       , DIOLINUXTCPIPSERVERFSMSTATE_WAITINGTOREAD     ,
            DIOLINUXTCPIPSERVERFSMEVENT_SENDINGDATA         , DIOLINUXTCPIPSERVERFSMSTATE_SENDINGDATA       ,
            DIOLINUXTCPIPSERVERFSMEVENT_DISCONNECTING       , DIOLINUXTCPIPSERVERFSMSTATE_DISCONNECTING     ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOLINUXTCPIPSERVERFSMSTATE_WAITINGTOREAD       ,
            DIOLINUXTCPIPSERVERFSMEVENT_GETTINGCONNECTION   , DIOLINUXTCPIPSERVERFSMSTATE_GETTINGCONNECTION ,
            DIOLINUXTCPIPSERVERFSMEVENT_CONNECTED           , DIOLINUXTCPIPSERVERFSMSTATE_CONNECTED         ,
            DIOLINUXTCPIPSERVERFSMEVENT_SENDINGDATA         , DIOLINUXTCPIPSERVERFSMSTATE_SENDINGDATA       ,
            DIOLINUXTCPIPSERVERFSMEVENT_DISCONNECTING       , DIOLINUXTCPIPSERVERFSMSTATE_DISCONNECTING     ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOLINUXTCPIPSERVERFSMSTATE_DISCONNECTING       ,
            DIOLINUXTCPIPSERVERFSMEVENT_GETTINGCONNECTION   , DIOLINUXTCPIPSERVERFSMSTATE_GETTINGCONNECTION ,
            DIOLINUXTCPIPSERVERFSMEVENT_CONNECTED           , DIOLINUXTCPIPSERVERFSMSTATE_CONNECTED         ,
            DIOLINUXTCPIPSERVERFSMEVENT_WAITINGTOREAD       , DIOLINUXTCPIPSERVERFSMSTATE_WAITINGTOREAD     ,
            DIOLINUXTCPIPSERVERFSMEVENT_SENDINGDATA         , DIOLINUXTCPIPSERVERFSMSTATE_SENDINGDATA       ,
            XFSMACHINESTATE_EVENTDEFEND);

  threadconnection = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMTCPIP, _L("DIOLINUXSTREAMTCPIPSERVER::DIOLINUXSTREAMTCPIPSERVER"), ThreadConnection, (void*)this);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOLINUXSTREAMTCPIPSERVER::~DIOLINUXSTREAMTCPIPSERVER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOLINUXSTREAMTCPIPSERVER::~DIOLINUXSTREAMTCPIPSERVER()
{
  if(threadconnection)
    {
      DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMTCPIP, threadconnection);
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXSTREAMTCPIPSERVER::Open()
* @brief      Open
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXSTREAMTCPIPSERVER::Open()
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
* @fn         bool DIOLINUXSTREAMTCPIPSERVER::Disconnect()
* @brief      Disconnect
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXSTREAMTCPIPSERVER::Disconnect()
{
  for(XDWORD c=0; c<GetMultiSocketStreams()->GetSize(); c++)
    {
      DIOLINUXSTREAMTCPIP* diostream = (DIOLINUXSTREAMTCPIP*)GetMultiSocketStreams()->Get(c);
      if(diostream)
        {
          if(diostream->GetStatus()!=DIOSTREAMSTATUS_DISCONNECTED)
            {
              diostream->SetEvent(DIOLINUXTCPIPSERVERFSMEVENT_DISCONNECTING);
              GEN_XSLEEP.MilliSeconds(10);          
            }             
        }
    }
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXSTREAMTCPIPSERVER::Close()
* @brief      Close
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXSTREAMTCPIPSERVER::Close()
{
  if(!threadconnection)  
    {
      return false;
    }

  threadconnection->End();

  Disconnect();

  DeleteAllStream();

  int handleserver = (int)config->GetHandleMultiServer();
  if(handleserver != -1)
    {          
      close(handleserver);

      if(config->GetXMutexMultiServer())
        {
          config->GetXMutexMultiServer()->Lock();
          config->SetHandleMultiServer((int)-1);
          config->GetXMutexMultiServer()->UnLock();
        }
    }
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOLINUXSTREAMTCPIPSERVER::Accept(int handlesocket, void* addr, void* addrlen, XDWORD usec)
* @brief      Accept
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  handlesocket : Handlesocket value.
* @param[in]  addr : Addr pointer to use.
* @param[in]  addrlen : Addrlen pointer to use.
* @param[in]  usec : Usec value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOLINUXSTREAMTCPIPSERVER::Accept(int handlesocket, void* addr, void* addrlen, XDWORD usec)
{
  fd_set         fds;
  struct timeval time_out;
  int            status;

  if(handlesocket==-1) 
    {
      return -1;
    }

  FD_ZERO(&fds);
  FD_SET((unsigned int)handlesocket, &fds);

  time_out.tv_sec  = 0;
  time_out.tv_usec = usec;

  status = (int)select((int)(handlesocket)+1, &fds, NULL, NULL, &time_out);
  if(status>0)
    {
      status = accept(handlesocket, (struct sockaddr*)addr, (socklen_t*)addrlen);
    }
   else 
    {
      status = -1;
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOLINUXSTREAMTCPIPSERVER::IsReadyConnect(int handlesocket)
* @brief      Is ready connect
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  handlesocket : Handlesocket value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOLINUXSTREAMTCPIPSERVER::IsReadyConnect(int handlesocket)
{
  if(handlesocket==-1) 
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
  if(rc == -1) return -1;

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
* @fn         DIOSTREAM* DIOLINUXSTREAMTCPIPSERVER::CreateStream()
* @brief      Create stream
* @ingroup    PLATFORM_LINUX
* 
* @return     DIOSTREAM* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAM* DIOLINUXSTREAMTCPIPSERVER::CreateStream()
{
  DIOSTREAMTCPIPCONFIG* _config = NULL;

  _config = GEN_NEW DIOSTREAMTCPIPCONFIG();
  if(_config)
    {
      _config->SetMode(DIOSTREAMMODE_SERVER);
      _config->SetRemotePort(config->GetRemotePort());
    }  

  DIOLINUXSTREAMTCPIP* diostream =  GEN_NEW DIOLINUXSTREAMTCPIP();
  if(diostream)
    {      
      diostream->SetConfig(_config);

      diostream->SetEvent(DIOLINUXTCPIPSERVERFSMEVENT_GETTINGCONNECTION);
      diostream->SetStatus(DIOSTREAMSTATUS_GETTINGCONNECTION);

      diostream->ResetXBuffers();
      diostream->ResetConnectionStatistics(); 
     
      multisocketstreams.Add(diostream);     
    }

  return diostream;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXSTREAMTCPIPSERVER::DeleteAllStreamDisconnected()
* @brief      Delete all stream disconnected
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXSTREAMTCPIPSERVER::DeleteAllStreamDisconnected()
{
  int index = 0;

  while(index < GetMultiSocketStreams()->GetSize())
    {
      DIOLINUXSTREAMTCPIP* diostream = (DIOLINUXSTREAMTCPIP*)GetMultiSocketStreams()->Get(index);
      if(diostream)
        {
          if(diostream->GetStatus() == DIOSTREAMSTATUS_DISCONNECTED)
            {
              if(diostream->GetConfig())
                {
                  GEN_DELETE diostream->GetConfig();
                }

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
* @fn         bool DIOLINUXSTREAMTCPIPSERVER::DeleteAllStream()
* @brief      Delete all stream
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXSTREAMTCPIPSERVER::DeleteAllStream()
{
  int index = 0;

  while(index < GetMultiSocketStreams()->GetSize())
    {
      DIOLINUXSTREAMTCPIP* diostream = (DIOLINUXSTREAMTCPIP*)GetMultiSocketStreams()->Get(index);
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
* @fn         bool DIOLINUXSTREAMTCPIPSERVER::GetHandleServer(DIOSTREAMTCPIP* diostream)
* @brief      Get handle server
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  diostream : Diostream pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXSTREAMTCPIPSERVER::GetHandleServer(DIOSTREAMTCPIP* diostream)
{
  int                 handleserver = (int)config->GetHandleMultiServer();
  struct sockaddr_in  loc_addr     = { 0 };
  struct sockaddr_in  rem_addr     = { 0 };
  XSTRING             IPstring;

  // ------------------------------------------------------------------------------------------  

  memset(&loc_addr, 0, sizeof(struct sockaddr_in));
  memset(&rem_addr, 0, sizeof(struct sockaddr_in));

  if(!diostream)
    {
      return false;
    }

  // ------------------------------------------------------------------------------------------  

  if(handleserver < 0)
    {
      config->GetXMutexMultiServer()->Lock();

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

  socklen_t sizeaddr     = sizeof(struct sockaddr_in);
  int       handlesocket = Accept(handleserver, (struct sockaddr*)&rem_addr, &sizeaddr, 100/*50*1000*/);
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

  DIOLINUXSTREAMTCPIP* diowindowsstreamtcpip = (DIOLINUXSTREAMTCPIP*)diostream;
  if(diowindowsstreamtcpip)
    {
      diowindowsstreamtcpip->SetHandleSocket(handlesocket);
    }
  
  // ------------------------------------------------------------------------------------------  

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOLINUXSTREAMTCPIPSERVER::ThreadConnection(void* data)
* @brief      Thread connection
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  data : Data buffer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOLINUXSTREAMTCPIPSERVER::ThreadConnection(void* data)
{
  DIOLINUXSTREAMTCPIPSERVER* diostreamserver = (DIOLINUXSTREAMTCPIPSERVER*)data;
  if(!diostreamserver) 
    {
      return;
    }

  diostreamserver->DeleteAllStreamDisconnected();

  for(XDWORD c=0; c<diostreamserver->GetMultiSocketStreams()->GetSize(); c++)
    {
      DIOLINUXSTREAMTCPIP* diostream = (DIOLINUXSTREAMTCPIP*)diostreamserver->GetMultiSocketStreams()->Get(c);
      if(diostream) 
        {
          if(diostream->GetEvent()==DIOLINUXTCPIPFSMEVENT_NONE) // No hay nuevos Eventos
            {
              switch(diostream->GetCurrentState())
                {
                  case DIOLINUXTCPIPSERVERFSMSTATE_NONE               : break;

                  case DIOLINUXTCPIPSERVERFSMSTATE_GETTINGCONNECTION  : if(diostream->GetHandleSocket() == -1)
                                                                          {
                                                                            diostreamserver->GetHandleServer(diostream);                                                                              
                                                                          }
                                                                         else
                                                                          {
                                                                            switch(diostreamserver->IsReadyConnect(diostream->GetHandleSocket()))
                                                                              {
                                                                                case -1:  diostream->SetEvent(DIOLINUXTCPIPFSMEVENT_DISCONNECTING);
                                                                                          break;

                                                                                case  1:  diostream->SetEvent(DIOLINUXTCPIPFSMEVENT_CONNECTED);
                                                                                          diostream->SetStatus(DIOSTREAMSTATUS_CONNECTED);
                                                                                          break;

                                                                                default:  diostream->SetStatus(DIOSTREAMSTATUS_GETTINGCONNECTION);
                                                                                          break;
                                                                              }
                                                                          }
                                                                        break;

                  case DIOLINUXTCPIPSERVERFSMSTATE_CONNECTED          : break;

                  case DIOLINUXTCPIPSERVERFSMSTATE_WAITINGTOREAD      : { struct timeval  waitd;
                                                                          fd_set          read_flags;
                                                                          fd_set          write_flags;

                                                                          if(diostream->GetHandleSocket() == -1)
                                                                            {
                                                                              diostream->SetEvent(DIOLINUXTCPIPFSMEVENT_DISCONNECTING);
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
                                                                          if(error == -1)   //Socket ready for reading
                                                                            {
                                                                              diostream->SetEvent(DIOLINUXTCPIPFSMEVENT_DISCONNECTING);
                                                                              break;
                                                                            }

                                                                          if(FD_ISSET(diostream->GetHandleSocket(), &read_flags))  //Socket ready for reading
                                                                            {
                                                                              XBYTE buffer[DIOSTREAM_MAXBUFFER];

                                                                              FD_CLR(diostream->GetHandleSocket(), &read_flags);

                                                                              XDWORD size = recv(diostream->GetHandleSocket(), (char*)buffer, DIOSTREAM_MAXBUFFER, 0);
                                                                              if(size==-1)
                                                                                {
                                                                                  diostream->SetEvent(DIOLINUXTCPIPFSMEVENT_DISCONNECTING);
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
                                                                                  diostream->SetEvent(DIOLINUXTCPIPFSMEVENT_DISCONNECTING);
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

                                                                                  if(size == -1)
                                                                                    {
                                                                                      diostream->SetEvent(DIOLINUXTCPIPFSMEVENT_DISCONNECTING);
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

                  case DIOLINUXTCPIPSERVERFSMSTATE_SENDINGDATA        : break;

                  case DIOLINUXTCPIPSERVERFSMSTATE_DISCONNECTING      : break;

                }
            }
            else
            {
              if(diostream->GetEvent()<DIOLINUXTCPIP_LASTEVENT)
                {
                  diostream->CheckTransition();

                  switch(diostream->GetCurrentState())
                    {
                      case DIOLINUXTCPIPSERVERFSMSTATE_NONE                 : break;

                      case DIOLINUXTCPIPSERVERFSMSTATE_GETTINGCONNECTION    : if(diostreamserver->enumservers)
                                                                                {
                                                                                  if(diostreamserver->enumservers->GetXTimer()) 
                                                                                    {
                                                                                      diostreamserver->enumservers->GetXTimer()->AddSeconds(DIOSTREAMTCPIP_TIMEOUTENUMSERVERSIGNAL);
                                                                                    }
                                                                                }
                                                                              break;

                      case DIOLINUXTCPIPSERVERFSMSTATE_CONNECTED            : { DIOSTREAM_XEVENT xevent(diostream, DIOSTREAM_XEVENT_TYPE_CONNECTED);
                                                                                xevent.SetDIOStream(diostream);
                                                                                diostreamserver->PostEvent(&xevent);

                                                                                diostream->SetEvent(DIOLINUXTCPIPFSMEVENT_WAITINGTOREAD);

                                                                                diostreamserver->CreateStream(); 
                                                                              }
                                                                              break;

                      case DIOLINUXTCPIPSERVERFSMSTATE_WAITINGTOREAD        : break;

                      case DIOLINUXTCPIPSERVERFSMSTATE_SENDINGDATA          : break;

                      case DIOLINUXTCPIPSERVERFSMSTATE_DISCONNECTING        : { DIOSTREAM_XEVENT xevent(diostream, DIOSTREAM_XEVENT_TYPE_DISCONNECTED);
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
* @fn         void DIOLINUXSTREAMTCPIPSERVER::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOLINUXSTREAMTCPIPSERVER::Clean()
{
  threadconnection   = NULL;  
}






