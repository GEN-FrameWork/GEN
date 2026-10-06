/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOWINDOWSStreamUDP.cpp
* 
* @class      DIOWINDOWSSTREAMUDP
* @brief      WINDOWS Data Input/Output Stream UDP class
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

#include "DIOWINDOWSStreamUDP.h"

#include "XFactory.h"
#include "XBuffer.h"
#include "XTrace.h"

#include "DIOIP.h"
#include "DIOURL.h"
#include "DIOStream_XEvent.h"
#include "DIOStreamUDPConfig.h"


/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/




/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWINDOWSSTREAMUDP::DIOWINDOWSSTREAMUDP() : DIOSTREAMUDP() , XFSMACHINE(0)
* @brief      Constructor of class
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWINDOWSSTREAMUDP::DIOWINDOWSSTREAMUDP() : DIOSTREAMUDP() , XFSMACHINE(0)
{
  Clean();

  AddState( DIOWINDOWSUDPFSMSTATE_NONE                ,
            DIOWINDOWSUDPFSMEVENT_GETTINGCONNECTION   , DIOWINDOWSUDPFSMSTATE_GETTINGCONNECTION ,
            DIOWINDOWSUDPFSMEVENT_CONNECTED           , DIOWINDOWSUDPFSMSTATE_CONNECTED         ,
            DIOWINDOWSUDPFSMEVENT_DISCONNECTING       , DIOWINDOWSUDPFSMSTATE_DISCONNECTING     ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOWINDOWSUDPFSMSTATE_GETTINGCONNECTION   ,
            DIOWINDOWSUDPFSMEVENT_CONNECTED           , DIOWINDOWSUDPFSMSTATE_CONNECTED         ,
            DIOWINDOWSUDPFSMEVENT_WAITINGTOREAD       , DIOWINDOWSUDPFSMSTATE_WAITINGTOREAD     ,
            DIOWINDOWSUDPFSMEVENT_SENDINGDATA         , DIOWINDOWSUDPFSMSTATE_SENDINGDATA       ,
            DIOWINDOWSUDPFSMEVENT_DISCONNECTING       , DIOWINDOWSUDPFSMSTATE_DISCONNECTING     ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOWINDOWSUDPFSMSTATE_CONNECTED           ,
            DIOWINDOWSUDPFSMEVENT_GETTINGCONNECTION   , DIOWINDOWSUDPFSMSTATE_GETTINGCONNECTION ,
            DIOWINDOWSUDPFSMEVENT_WAITINGTOREAD       , DIOWINDOWSUDPFSMSTATE_WAITINGTOREAD     ,
            DIOWINDOWSUDPFSMEVENT_SENDINGDATA         , DIOWINDOWSUDPFSMSTATE_SENDINGDATA       ,
            DIOWINDOWSUDPFSMEVENT_DISCONNECTING       , DIOWINDOWSUDPFSMSTATE_DISCONNECTING     ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOWINDOWSUDPFSMSTATE_WAITINGTOREAD       ,
            DIOWINDOWSUDPFSMEVENT_GETTINGCONNECTION   , DIOWINDOWSUDPFSMSTATE_GETTINGCONNECTION ,
            DIOWINDOWSUDPFSMEVENT_CONNECTED           , DIOWINDOWSUDPFSMSTATE_CONNECTED         ,
            DIOWINDOWSUDPFSMEVENT_SENDINGDATA         , DIOWINDOWSUDPFSMSTATE_SENDINGDATA       ,
            DIOWINDOWSUDPFSMEVENT_DISCONNECTING       , DIOWINDOWSUDPFSMSTATE_DISCONNECTING     ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOWINDOWSUDPFSMSTATE_DISCONNECTING       ,
            DIOWINDOWSUDPFSMEVENT_GETTINGCONNECTION   , DIOWINDOWSUDPFSMSTATE_GETTINGCONNECTION ,
            DIOWINDOWSUDPFSMEVENT_CONNECTED           , DIOWINDOWSUDPFSMSTATE_CONNECTED         ,
            DIOWINDOWSUDPFSMEVENT_WAITINGTOREAD       , DIOWINDOWSUDPFSMSTATE_WAITINGTOREAD     ,
            DIOWINDOWSUDPFSMEVENT_SENDINGDATA         , DIOWINDOWSUDPFSMSTATE_SENDINGDATA       ,
            XFSMACHINESTATE_EVENTDEFEND);

  threadconnection = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMUDP, _L("DIOWINDOWSSTREAMUDP::DIOWINDOWSSTREAMUDP"), ThreadConnection, (void*)this);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWINDOWSSTREAMUDP::~DIOWINDOWSSTREAMUDP()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWINDOWSSTREAMUDP::~DIOWINDOWSSTREAMUDP()
{
  if(threadconnection)
    {
      DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMUDP, threadconnection);
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMUDP::Open()
* @brief      Open
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMUDP::Open()
{
  if(!threadconnection)  return false;

  if(!inbuffer)         return false;
  if(!outbuffer)        return false;
  if(!config)           return true;

  if(!config->IsServer())
    {
      if(config->GetRemoteURL()->IsEmpty()) 
        {
          return false;   
        }

      if(config->GetRemoteURL()->IsAURL())
        {
          config->GetRemoteURL()->ResolveURL((*config->GetResolvedRemoteURL()));
        }
       else 
        {
          config->GetResolvedRemoteURL()->Set(config->GetRemoteURL()->Get());      
        }
    }
  
  SetEvent(DIOWINDOWSUDPFSMEVENT_GETTINGCONNECTION);

  status = DIOSTREAMSTATUS_GETTINGCONNECTION;

  ResetXBuffers();

  ResetConnectionStatistics();

  threadconnection->SetWaitYield(config->GetThreadWaitYield());
  threadconnection->SetPriority(config->GetThreadPriority());

  return threadconnection->Ini();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMUDP::Disconnect()
* @brief      Disconnect
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMUDP::Disconnect()
{
  if((GetStatus()==DIOSTREAMSTATUS_GETTINGCONNECTION)||
     (GetStatus()==DIOSTREAMSTATUS_CONNECTED))
    {
      SetEvent(DIOWINDOWSUDPFSMEVENT_DISCONNECTING);

      XTIMER* timerout=GEN_XFACTORY.CreateTimer();
      if(timerout)
        {            
          while(GetStatus()!=DIOSTREAMSTATUS_DISCONNECTED)
            {
              if(timerout->GetMeasureSeconds() > 3) break;

              Sleep(1000);
            }

          GEN_XFACTORY.DeleteTimer(timerout);
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMUDP::Close()
* @brief      Close
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMUDP::Close()
{
  if(!threadconnection) return false;

  threadconnection->End();

  if(handle!=INVALID_SOCKET)
    {
      shutdown(handle, SD_BOTH);
      closesocket(handle);
      handle  = INVALID_SOCKET;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOWINDOWSSTREAMUDP::IsReadyConnect(SOCKET socket)
* @brief      Is ready connect
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  socket : Socket value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOWINDOWSSTREAMUDP::IsReadyConnect(SOCKET socket)
{
  struct timeval  tv;
  int             rc;
  fd_set          fdr;
  fd_set          fdw;
  fd_set          fds;

  if(socket == INVALID_SOCKET) return -1;

  FD_ZERO(&fdr);
  FD_ZERO(&fdw);
  FD_ZERO(&fds);

  FD_SET((unsigned int)socket, &fdr);
  FD_SET((unsigned int)socket, &fdw);
  FD_SET((unsigned int)socket, &fds);

  tv.tv_sec  = 0;
  tv.tv_usec = 100;

  rc = select((int)(socket)+1, &fdr, &fdw, &fds, &tv);
  if(rc == SOCKET_ERROR) return -1;

  int status1 = FD_ISSET(socket, &fdr) ? 1 : 0;
  int status2 = FD_ISSET(socket, &fdw) ? 1 : 0;
  int status3 = FD_ISSET(socket, &fds) ? 1 : 0;

  //XTRACE_PRINTCOLOR(1, _L("UDP stream: r: %d, w: %d, s: %d"), status1, status2, status3);

  if(config->IsServer())
    {
      if(status1 || status2 )   return  1;
      if(status3)               return -1;
    }
   else
    {
      if((!status1) && status2) return  1;
      if(status3)               return -1;
    }

  return 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWINDOWSSTREAMUDP::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWINDOWSSTREAMUDP::Clean()
{
  threadconnection  = NULL;
  status            = DIOSTREAMSTATUS_DISCONNECTED;
  handle            = INVALID_SOCKET;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWINDOWSSTREAMUDP::ThreadConnection(void* data)
* @brief      Thread connection
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  data : Data buffer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWINDOWSSTREAMUDP::ThreadConnection(void* data)
{
  DIOWINDOWSSTREAMUDP* diostream = (DIOWINDOWSSTREAMUDP*)data;
  if(!diostream) return;

  if(diostream->GetEvent()==DIOWINDOWSUDPFSMEVENT_NONE) // No hay nuevos Eventos
    {
      switch(diostream->GetCurrentState())
        {
          case DIOWINDOWSUDPFSMSTATE_NONE                 : break;

          case DIOWINDOWSUDPFSMSTATE_GETTINGCONNECTION    : switch(diostream->IsReadyConnect(diostream->handle))
                                                              {
                                                                case -1:  diostream->SetEvent(DIOWINDOWSUDPFSMEVENT_DISCONNECTING);
                                                                          break;

                                                                case  1:  diostream->SetEvent(DIOWINDOWSUDPFSMEVENT_CONNECTED);
                                                                          diostream->status = DIOSTREAMSTATUS_CONNECTED;
                                                                          break;

                                                                default:  diostream->status = DIOSTREAMSTATUS_GETTINGCONNECTION;
                                                                          break;
                                                              }
                                                            break;

          case DIOWINDOWSUDPFSMSTATE_CONNECTED            : break;

          case DIOWINDOWSUDPFSMSTATE_WAITINGTOREAD        : { struct timeval  waitd;
                                                              fd_set          read_flags;
                                                              fd_set          write_flags;

                                                              if(diostream->handle == INVALID_SOCKET)
                                                                {
                                                                  diostream->SetEvent(DIOWINDOWSUDPFSMEVENT_DISCONNECTING);
                                                                  break;
                                                                }

                                                              waitd.tv_sec  = 0;
                                                              waitd.tv_usec = 100;

                                                              FD_ZERO(&read_flags);
                                                              FD_ZERO(&write_flags);

                                                              if(!diostream->IsBlockRead()) FD_SET(diostream->handle, &read_flags);
                                                              if(!diostream->IsBlockWrite())
                                                                {
                                                                  if(diostream->config->IsUsedDatagrams())
                                                                    {
                                                                      if(diostream->GetFirstDatagram(true) != DIOSTREAMUDP_NOTFOUND) 
                                                                        {
                                                                          FD_SET(diostream->handle, &write_flags);
                                                                        }
                                                                    }
                                                                   else
                                                                    {
                                                                      if(diostream->outbuffer->GetSize()) 
                                                                        {
                                                                          FD_SET(diostream->handle, &write_flags);
                                                                        }
                                                                    }
                                                                }

                                                              int error = select((int)(diostream->handle)+1, &read_flags, &write_flags, (fd_set*)0, &waitd);
                                                              if(error == SOCKET_ERROR)
                                                                {
                                                                  diostream->SetEvent(DIOWINDOWSUDPFSMEVENT_DISCONNECTING);
                                                                  break;
                                                                }

                                                              if(FD_ISSET(diostream->handle, &read_flags))  //Socket ready for reading
                                                                {
                                                                  XBYTE buffer[DIOSTREAM_MAXBUFFER];

                                                                  FD_CLR(diostream->handle, &read_flags);

                                                                  memset(buffer, 0, DIOSTREAM_MAXBUFFER);

                                                                  SOCKADDR_IN  origin_addr;
                                                                  int          size_addr = sizeof(SOCKADDR_IN);

                                                                  memset(&origin_addr, 0, size_addr);

                                                                  XDWORD size = recvfrom(diostream->handle, (char*)buffer, DIOSTREAM_MAXBUFFER, 0, (sockaddr*)&origin_addr, &size_addr);

                                                                  if(size == SOCKET_ERROR)
                                                                    {
                                                                      //int error = WSAGetLastError();

                                                                      diostream->SetEvent(DIOWINDOWSUDPFSMEVENT_DISCONNECTING);
                                                                      break;
                                                                    }

                                                                  if(size)
                                                                    {
                                                                      XSTRING address;
                                                                      XWORD   port;

                                                                      address.Format(_L("%d.%d.%d.%d"), origin_addr.sin_addr.S_un.S_un_b.s_b1, origin_addr.sin_addr.S_un.S_un_b.s_b2, origin_addr.sin_addr.S_un.S_un_b.s_b3, origin_addr.sin_addr.S_un.S_un_b.s_b4);

                                                                     // XTRACE_PRINTCOLOR(((size == 100)?XTRACE_COLOR_PURPLE:XTRACE_COLOR_BLUE) , _L("Read UDP from [%s] (%d)"), address.Get(), size);

                                                                      port =  ntohs(origin_addr.sin_port);

                                                                      if(diostream->config->IsUsedDatagrams())
                                                                        {
                                                                          diostream->AddDatagram(false, address.Get(), port, (XBYTE*)buffer, size);
                                                                        }
                                                                       else 
                                                                        {
                                                                          diostream->inbuffer->Add(buffer, size);
                                                                        }
                                                                    }
                                                                   else
                                                                    {
                                                                      diostream->SetEvent(DIOWINDOWSUDPFSMEVENT_DISCONNECTING);
                                                                      break;
                                                                    }
                                                                }


                                                              if(FD_ISSET(diostream->handle, &write_flags))  //Socket ready for writing
                                                                {
                                                                  FD_CLR(diostream->handle, &write_flags);

                                                                  if(diostream->config->IsUsedDatagrams())
                                                                    {
                                                                      int indexdatagram  = diostream->GetFirstDatagram(true);
                                                                      if(indexdatagram != DIOSTREAMUDP_NOTFOUND)
                                                                        {
                                                                          DIOSTREAMUDPDATAGRAM* datagram = (DIOSTREAMUDPDATAGRAM*)diostream->GetDatagram(indexdatagram);
                                                                          if(datagram)
                                                                            {
                                                                              SOCKADDR_IN  target_addr;
                                                                              int          size_addr = sizeof(SOCKADDR_IN);
                                                                              int          size;
                                                                             
                                                                              memset(&target_addr, 0, size_addr);

                                                                              target_addr.sin_family = AF_INET;

                                                                              if(diostream->config->IsBroadcastModeActive())
                                                                                {
                                                                                  target_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
                                                                                }
                                                                               else
                                                                                {
                                                                                  if(!datagram->GetAddress()->IsEmpty())
                                                                                    {
                                                                                      diostream->config->GetResolvedRemoteURL()->Set(datagram->GetAddress()->Get());
                                                                                    }

                                                                                  XBUFFER charstr;
                                                                                  
                                                                                  diostream->config->GetResolvedRemoteURL()->ConvertToASCII(charstr);
                                                                                  
                                                                                  #ifndef BUILDER
                                                                                  inet_pton(target_addr.sin_family, charstr.GetPtrChar(), &target_addr.sin_addr.s_addr);
                                                                                  #else
                                                                                  target_addr.sin_addr.s_addr   = inet_addr(charstr.GetPtrChar());
                                                                                  #endif                                                                                  
                                                                                }

                                                                              target_addr.sin_port  = htons(datagram->GetPort()?datagram->GetPort():diostream->config->GetRemotePort());

                                                                              size = sendto(diostream->handle, (char*)datagram->GetData()->Get(), datagram->GetData()->GetSize(), 0, (sockaddr*)&target_addr, size_addr);                                                                              
                                                                              if(size == SOCKET_ERROR)
                                                                                {
                                                                                  diostream->SetEvent(DIOWINDOWSUDPFSMEVENT_DISCONNECTING);
                                                                                  break;
                                                                                }

                                                                              if(size)
                                                                                {
                                                                                  if(diostream->config->IsUsedDatagrams())
                                                                                    {
                                                                                      diostream->DeleteDatagram(indexdatagram);
                                                                                    }
                                                                                   else 
                                                                                    {
                                                                                      diostream->outbuffer->Extract(NULL, 0 , datagram->GetData()->GetSize());
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                   else
                                                                    {                                                                      
                                                                      int esize = diostream->outbuffer->GetSize();
                                                                      if(esize>DIOSTREAM_MAXBUFFER) esize = DIOSTREAM_MAXBUFFER;

                                                                      if(esize)
                                                                        {
                                                                          SOCKADDR_IN  target_addr;
                                                                          int          size_addr = sizeof(SOCKADDR_IN);
                                                                          int          size;
                                                                          
                                                                          memset(&target_addr, 0, size_addr);

                                                                          target_addr.sin_family = AF_INET;

                                                                          if(diostream->config->IsBroadcastModeActive())
                                                                            {
                                                                              target_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
                                                                              target_addr.sin_port        = htons(diostream->config->GetRemotePort());

                                                                              diostream->outbuffer->SetBlocked(true);
                                                                              size = sendto(diostream->handle, (char*)diostream->outbuffer->Get(), esize, 0, (sockaddr*)&target_addr, size_addr);
                                                                              diostream->outbuffer->SetBlocked(false);
                                                                            }
                                                                           else
                                                                            {                                                                                                                                                            
                                                                              if(!diostream->config->GetResolvedRemoteURL()->IsEmpty())
                                                                                {
                                                                                  XBUFFER charstr;
                                                                              
                                                                                  diostream->config->GetResolvedRemoteURL()->ConvertToASCII(charstr);
                                                                                                                                                            
                                                                                  #ifndef BUILDER
                                                                                  inet_pton(target_addr.sin_family, charstr.GetPtrChar(), &target_addr.sin_addr.s_addr);
                                                                                  #else
                                                                                  target_addr.sin_addr.s_addr   = inet_addr(charstr.GetPtrChar());
                                                                                  #endif                                                                              

                                                                                  target_addr.sin_port  = htons(diostream->config->GetRemotePort());

                                                                                  diostream->outbuffer->SetBlocked(true);
                                                                                  size = sendto(diostream->handle, (char*)diostream->outbuffer->Get(), esize, 0, (sockaddr*)&target_addr, size_addr);
                                                                                  diostream->outbuffer->SetBlocked(false);
                                                                                }
                                                                               else
                                                                                {
                                                                                  diostream->outbuffer->SetBlocked(true);
                                                                                  size = send(diostream->handle, (char*)diostream->outbuffer->Get(), esize, 0);
                                                                                  diostream->outbuffer->SetBlocked(true);
                                                                                }
                                                                            }
                                                                          
                                                                          if(esize)
                                                                            {
                                                                              diostream->outbuffer->Extract(NULL, 0 , esize);
                                                                            }

                                                                          if(size == SOCKET_ERROR)
                                                                            {
                                                                              diostream->SetEvent(DIOWINDOWSUDPFSMEVENT_DISCONNECTING);
                                                                              break;
                                                                            }

                                                                        }
                                                                    }
                                                                }
                                                            }
                                                            break;

          case DIOWINDOWSUDPFSMSTATE_SENDINGDATA          : break;

          case DIOWINDOWSUDPFSMSTATE_DISCONNECTING        : break;

        }
    }
   else
    {
      if(diostream->GetEvent()<DIOWINDOWSUDP_LASTEVENT)
        {
          diostream->CheckTransition();

          switch(diostream->GetCurrentState())
            {
              case DIOWINDOWSUDPFSMSTATE_NONE               : break;

              case DIOWINDOWSUDPFSMSTATE_GETTINGCONNECTION  : { SOCKADDR_IN loc_addr;

                                                                memset(&loc_addr, 0, sizeof(SOCKADDR_IN));

                                                                diostream->handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
                                                                if(diostream->handle == INVALID_SOCKET)
                                                                  {
                                                                    diostream->SetEvent(DIOWINDOWSUDPFSMEVENT_DISCONNECTING);
                                                                    break;
                                                                  }

                                                                if(diostream->config->IsServer())
                                                                  {
                                                                    XSTRING IPstring;

                                                                    diostream->config->GetLocalIP()->GetXString(IPstring);

                                                                    loc_addr.sin_family = AF_INET;

                                                                    if(!diostream->config->GetLocalIP()->IsEmpty())
                                                                      {
                                                                        XBUFFER charstr;
 
                                                                        IPstring.ConvertToASCII(charstr);
                                                                                                                                               
                                                                        #ifndef BUILDER
                                                                        inet_pton(loc_addr.sin_family, charstr.GetPtrChar(), &loc_addr.sin_addr.s_addr);
                                                                        #else
                                                                        loc_addr.sin_addr.s_addr  = inet_addr(charstr.GetPtrChar());
                                                                        #endif                                                                        
                                                                      }
                                                                     else
                                                                      {
                                                                        loc_addr.sin_addr.s_addr  = htonl(INADDR_ANY);
                                                                      }

                                                                    loc_addr.sin_port = htons(diostream->config->GetRemotePort());
                                                                    
                                                                    const int enable = 1;
                                                                    if(setsockopt(diostream->handle, SOL_SOCKET, SO_REUSEADDR, (const char*)&enable, sizeof(int)) < 0)
                                                                      {
                                                                        //int error = WSAGetLastError();
                                                                      }

                                                                    if(bind(diostream->handle, (LPSOCKADDR)&loc_addr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
                                                                      {
                                                                        //int error = WSAGetLastError();

                                                                        diostream->SetEvent(DIOWINDOWSUDPFSMEVENT_DISCONNECTING);
                                                                        break;
                                                                      }
                                                                  }

                                                                if(!diostream->config->IsServer())
                                                                  {
                                                                    if(diostream->config->IsBroadcastModeActive())
                                                                      {
                                                                        int yes = 1;
                                                                        setsockopt(diostream->handle, SOL_SOCKET, SO_BROADCAST, (char*)&yes, sizeof(yes));
                                                                      }                                                                     
                                                                  }

                                                                if(diostream->config->GetSizeBufferSO())
                                                                  {
                                                                    XDWORD sizebuffer = diostream->config->GetSizeBufferSO();
                                                                    if(setsockopt(diostream->handle, SOL_SOCKET, SO_RCVBUF, (const char*)&sizebuffer, sizeof(XDWORD)) == -1)
                                                                      {
                                                                        //fprintf(stderr, "Error setting socket opts: %s\n", strerror(errno));
                                                                      }
                                                                  }

                                                                DWORD nonblock = 1;
                                                                if (ioctlsocket(diostream->handle, FIONBIO, &nonblock) == SOCKET_ERROR)
                                                                  {
                                                                    diostream->SetEvent(DIOWINDOWSUDPFSMEVENT_DISCONNECTING);
                                                                    break;
                                                                  }
                                                              }
                                                              break;

              case DIOWINDOWSUDPFSMSTATE_CONNECTED          : { DIOSTREAM_XEVENT xevent(diostream, DIOSTREAM_XEVENT_TYPE_CONNECTED);
                                                                xevent.SetDIOStream(diostream);

                                                                diostream->PostEvent(&xevent);

                                                                diostream->SetEvent(DIOWINDOWSUDPFSMEVENT_WAITINGTOREAD);
                                                              }
                                                              break;

              case DIOWINDOWSUDPFSMSTATE_WAITINGTOREAD      : break;

              case DIOWINDOWSUDPFSMSTATE_SENDINGDATA        : break;

              case DIOWINDOWSUDPFSMSTATE_DISCONNECTING      : { DIOSTREAM_XEVENT xevent(diostream, DIOSTREAM_XEVENT_TYPE_DISCONNECTED);
                                                                xevent.SetDIOStream(diostream);

                                                                diostream->PostEvent(&xevent);

                                                                diostream->threadconnection->Run(false);
                                                                diostream->status = DIOSTREAMSTATUS_DISCONNECTED;

                                                                diostream->config->GetResolvedRemoteURL()->Empty();
                                                              }
                                                              break;

            }
        }
    }
}

