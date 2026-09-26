/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOLINUXStreamICMP.cpp
* 
* @class      DIOLINUXSTREAMICMP
* @brief      LINUX Data Input/Output Stream ICMP class
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


#if defined(DIO_ACTIVE) && defined(DIO_STREAMICMP_ACTIVE)

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOLINUXStreamICMP.h"

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
#include <net/if.h>
#include <netdb.h>
#include <linux/if_ether.h>

#include "XFactory.h"
#include "XBuffer.h"
#include "XTrace.h"

#include "DIOIP.h"
#include "DIOURL.h"
#include "DIOStream_XEvent.h"
#include "DIOStreamICMPConfig.h"
#include "DIOStreamICMP.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOLINUXSTREAMICMP::DIOLINUXSTREAMICMP() : DIOSTREAMICMP() , XFSMACHINE(0)
* @brief      Constructor of class
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOLINUXSTREAMICMP::DIOLINUXSTREAMICMP() : DIOSTREAMICMP() , XFSMACHINE(0)
{
  Clean();

  AddState( DIOLINUXICMPFSMSTATE_NONE                 ,
            DIOLINUXICMPFSMEVENT_GETTINGCONNECTION    , DIOLINUXICMPFSMSTATE_GETTINGCONNECTION  ,
            DIOLINUXICMPFSMEVENT_CONNECTED            , DIOLINUXICMPFSMSTATE_CONNECTED         ,
            DIOLINUXICMPFSMEVENT_DISCONNECTING        , DIOLINUXICMPFSMSTATE_DISCONNECTING     ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOLINUXICMPFSMSTATE_GETTINGCONNECTION    ,
            DIOLINUXICMPFSMEVENT_CONNECTED            , DIOLINUXICMPFSMSTATE_CONNECTED         ,
            DIOLINUXICMPFSMEVENT_WAITINGTOREAD        , DIOLINUXICMPFSMSTATE_WAITINGTOREAD     ,
            DIOLINUXICMPFSMEVENT_SENDINGDATA          , DIOLINUXICMPFSMSTATE_SENDINGDATA       ,
            DIOLINUXICMPFSMEVENT_DISCONNECTING        , DIOLINUXICMPFSMSTATE_DISCONNECTING     ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOLINUXICMPFSMSTATE_CONNECTED            ,
            DIOLINUXICMPFSMEVENT_GETTINGCONNECTION    , DIOLINUXICMPFSMSTATE_GETTINGCONNECTION,
            DIOLINUXICMPFSMEVENT_WAITINGTOREAD        , DIOLINUXICMPFSMSTATE_WAITINGTOREAD     ,
            DIOLINUXICMPFSMEVENT_SENDINGDATA          , DIOLINUXICMPFSMSTATE_SENDINGDATA       ,
            DIOLINUXICMPFSMEVENT_DISCONNECTING        , DIOLINUXICMPFSMSTATE_DISCONNECTING     ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOLINUXICMPFSMSTATE_WAITINGTOREAD        ,
            DIOLINUXICMPFSMEVENT_GETTINGCONNECTION    , DIOLINUXICMPFSMSTATE_GETTINGCONNECTION  ,
            DIOLINUXICMPFSMEVENT_CONNECTED            , DIOLINUXICMPFSMSTATE_CONNECTED         ,
            DIOLINUXICMPFSMEVENT_SENDINGDATA          , DIOLINUXICMPFSMSTATE_SENDINGDATA       ,
            DIOLINUXICMPFSMEVENT_DISCONNECTING        , DIOLINUXICMPFSMSTATE_DISCONNECTING     ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOLINUXICMPFSMSTATE_DISCONNECTING        ,
            DIOLINUXICMPFSMEVENT_GETTINGCONNECTION    , DIOLINUXICMPFSMSTATE_GETTINGCONNECTION  ,
            DIOLINUXICMPFSMEVENT_CONNECTED            , DIOLINUXICMPFSMSTATE_CONNECTED         ,
            DIOLINUXICMPFSMEVENT_WAITINGTOREAD        , DIOLINUXICMPFSMSTATE_WAITINGTOREAD     ,
            DIOLINUXICMPFSMEVENT_SENDINGDATA          , DIOLINUXICMPFSMSTATE_SENDINGDATA       ,
            XFSMACHINESTATE_EVENTDEFEND);

  threadconnection = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMICMP, __L("DIOLINUXSTREAMICMP::DIOLINUXSTREAMICMP"), ThreadRunFunction, (void*)this);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOLINUXSTREAMICMP::~DIOLINUXSTREAMICMP()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOLINUXSTREAMICMP::~DIOLINUXSTREAMICMP()
{
  if(threadconnection)
    {
      DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMICMP, threadconnection);
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXSTREAMICMP::Open()
* @brief      Open
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXSTREAMICMP::Open()
{
  if(!threadconnection)   return false;

  if(!inbuffer)           return false;
  if(!outbuffer)          return false;

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

  SetEvent(DIOLINUXICMPFSMEVENT_GETTINGCONNECTION);

  status = DIOSTREAMSTATUS_GETTINGCONNECTION;

  ResetXBuffers();

  ResetConnectionStatistics();

  threadconnection->SetWaitYield(config->GetThreadWaitYield());
  threadconnection->SetPriority(config->GetThreadPriority());

  return threadconnection->Ini();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXSTREAMICMP::Disconnect()
* @brief      Disconnect
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXSTREAMICMP::Disconnect()
{
  if((GetStatus()==DIOSTREAMSTATUS_GETTINGCONNECTION)||
     (GetStatus()==DIOSTREAMSTATUS_CONNECTED))
     {
       SetEvent(DIOLINUXICMPFSMEVENT_DISCONNECTING);

       while(GetStatus()!=DIOSTREAMSTATUS_DISCONNECTED)
        {
          usleep(10000);
        }
     }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXSTREAMICMP::Close()
* @brief      Close
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXSTREAMICMP::Close()
{
  if(!threadconnection) return false;

  threadconnection->End();

  if(handle>=0)
    {
      shutdown(handle, SHUT_RDWR);
      close(handle);
      handle  = -1;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOLINUXSTREAMICMP::IsReadyConnect(int socket)
* @brief      Is ready connect
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  socket : Socket value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOLINUXSTREAMICMP::IsReadyConnect(int socket)
{
  struct timeval  tv;
  int             rc;
  fd_set          fdr;
  fd_set          fdw;
  fd_set          fds;

  if(socket==-1) return -1;

  FD_ZERO(&fdr);
  FD_ZERO(&fdw);
  FD_ZERO(&fds);

  FD_SET(socket, &fdr);
  FD_SET(socket, &fdw);
  FD_SET(socket, &fds);

  tv.tv_sec  = 0;
  tv.tv_usec = 100;

  rc = select(socket+1, &fdr, &fdw, &fds, &tv);
  if(rc==-1) return -1;

  int status1 = FD_ISSET(socket, &fdr) ? 1 : 0;
  int status2 = FD_ISSET(socket, &fdw) ? 1 : 0;
  int status3 = FD_ISSET(socket, &fds) ? 1 : 0;

  if(config->IsServer())
    {
      if(status1 || status2 ) return  1;
      if(status3)             return -1;
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
* @fn         void DIOLINUXSTREAMICMP::ThreadRunFunction(void* thread)
* @brief      Thread run function
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  thread : Thread pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOLINUXSTREAMICMP::ThreadRunFunction(void* thread)
{
  DIOLINUXSTREAMICMP* diostream = (DIOLINUXSTREAMICMP*)thread;
  if(!diostream) return;

  if(diostream->GetEvent()==DIOLINUXICMPFSMEVENT_NONE)
    {
      switch(diostream->GetCurrentState())
        {
          case DIOLINUXICMPFSMSTATE_NONE                : break;

          case DIOLINUXICMPFSMSTATE_GETTINGCONNECTION  : switch(diostream->IsReadyConnect(diostream->handle))
                                                          {
                                                            case -1:  diostream->SetEvent(DIOLINUXICMPFSMEVENT_DISCONNECTING);
                                                                      break;

                                                            case  1:  diostream->SetEvent(DIOLINUXICMPFSMEVENT_CONNECTED);
                                                                      diostream->status = DIOSTREAMSTATUS_CONNECTED;
                                                                      break;

                                                            default:  diostream->status = DIOSTREAMSTATUS_GETTINGCONNECTION;
                                                                      break;
                                                          }
                                                        break;

          case DIOLINUXICMPFSMSTATE_CONNECTED         : break;

          case DIOLINUXICMPFSMSTATE_WAITINGTOREAD     : { struct timeval  waitd;
                                                          fd_set          read_flags;
                                                          fd_set          write_flags;

                                                          if(diostream->handle == -1)
                                                            {
                                                              diostream->SetEvent(DIOLINUXICMPFSMEVENT_DISCONNECTING);
                                                              break;
                                                            }

                                                          waitd.tv_sec  = 0;
                                                          waitd.tv_usec = 100;

                                                          FD_ZERO(&read_flags);
                                                          if(diostream->GetFirstDatagram(true) != DIOSTREAMICMP_NOTFOUND)  FD_ZERO(&write_flags);

                                                          FD_SET(diostream->handle, &read_flags);
                                                          FD_SET(diostream->handle, &write_flags);

                                                          int error = select(diostream->handle+1, &read_flags, &write_flags, NULL, &waitd);
                                                          if(error==-1)
                                                            {
                                                              diostream->SetEvent(DIOLINUXICMPFSMEVENT_DISCONNECTING);
                                                              break;
                                                            }

                                                          if(FD_ISSET(diostream->handle, &read_flags))  //Socket ready for reading
                                                            {
                                                              FD_CLR(diostream->handle, &read_flags);

                                                              if(!diostream->IsBlockRead())
                                                                {
                                                                  XBYTE buffer[DIOSTREAM_MAXBUFFER];

                                                                  struct sockaddr_in origin_addr;
                                                                  socklen_t          size_addr   = sizeof(sockaddr_in);

                                                                  memset(&origin_addr, 0, size_addr);

                                                                  XDWORD size = recvfrom(diostream->handle, buffer, DIOSTREAM_MAXBUFFER, 0, (struct sockaddr*)&origin_addr, &size_addr);
                                                                  if(size < 0)
                                                                    {
                                                                      diostream->SetEvent(DIOLINUXICMPFSMEVENT_DISCONNECTING);
                                                                      break;
                                                                    }

                                                                    if(size != 0)
                                                                      {
                                                                        XSTRING address;

                                                                        int _address[4] = { 0, 0, 0, 0 };

                                                                        sscanf(inet_ntoa(origin_addr.sin_addr), "%d.%d.%d.%d", &_address[0], &_address[1], &_address[2], &_address[3]);

                                                                        address.Format(__L("%d.%d.%d.%d"), _address[0], _address[1], _address[2], _address[3]);

                                                                        //XTRACE_PRINTCOLOR(1, __L("Read UDP from [%s] (%d)"), address.Get(), size);

                                                                        diostream->AddDatagram(false, address.Get(), (XBYTE*)buffer, size);
                                                                        diostream->inbuffer->Add(buffer, size);
                                                                      }
                                                                     else
                                                                      {
                                                                        diostream->SetEvent(DIOLINUXICMPFSMEVENT_DISCONNECTING);
                                                                        break;
                                                                      }
                                                                  }
                                                            }

                                                          if(FD_ISSET(diostream->handle, &write_flags))  //Socket ready for writing
                                                            {
                                                              FD_CLR(diostream->handle, &write_flags);

                                                              if(!diostream->IsBlockWrite())
                                                                {
                                                                  int indexdatagram  = diostream->GetFirstDatagram(true);
                                                                  if(indexdatagram != DIOSTREAMICMP_NOTFOUND)
                                                                    {
                                                                      DIOSTREAMICMPDATAGRAM* datagram = (DIOSTREAMICMPDATAGRAM*)diostream->GetDatagram(indexdatagram);
                                                                      if(datagram)
                                                                        {
                                                                          struct sockaddr_in  target_addr;
                                                                          int                 size_addr = sizeof(struct sockaddr_in);
                                                                          int                 size      = 0;
                                                                          XSTRING             tmpremoteaddress;

                                                                          memset(&target_addr, 0, size_addr);

                                                                          target_addr.sin_family = AF_INET;

                                                                          if(datagram->GetAddress()->IsEmpty())
                                                                                  tmpremoteaddress = diostream->remoteaddress.Get();
                                                                            else  tmpremoteaddress = datagram->GetAddress()->Get();


                                                                          XBUFFER charstr;
                                                                          
                                                                          tmpremoteaddress.ConvertToASCII(charstr);                                                                          
                                                                          target_addr.sin_addr.s_addr = inet_addr(charstr.GetPtrChar());                                                                          
                                                                          target_addr.sin_port        = 0;

                                                                          size = sendto(diostream->handle, (char*)datagram->GetData()->Get(), datagram->GetData()->GetSize(), 0, (sockaddr*)&target_addr, size_addr);

                                                                          if(size < 0)
                                                                            {
                                                                              //XTRACE_PRINTCOLOR(4, __L("Write ICMP to [%s] (%d) ERROR!"), tmpremoteaddress.Get(), size);

                                                                              diostream->SetEvent(DIOLINUXICMPFSMEVENT_DISCONNECTING);
                                                                              break;
                                                                            }

                                                                          if(size)
                                                                            {
                                                                              //XTRACE_PRINTCOLOR(1, __L("Write ICMP to [%s] (%d)"), tmpremoteaddress.Get(), size);

                                                                              diostream->outbuffer->Extract(NULL, 0 , datagram->GetData()->GetSize());
                                                                              diostream->DeleteDatagram(indexdatagram);
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                        break;

          case DIOLINUXICMPFSMSTATE_SENDINGDATA       : break;

          case DIOLINUXICMPFSMSTATE_DISCONNECTING     : break;
        }
    }
   else
    {
      if(diostream->GetEvent()<DIOLINUXICMP_LASTEVENT)
        {
          diostream->CheckTransition();

          switch(diostream->GetCurrentState())
            {
              case DIOLINUXICMPFSMSTATE_NONE                : break;

              case DIOLINUXICMPFSMSTATE_GETTINGCONNECTION  : { struct sockaddr_in loc_addr = { 0 };

                                                              diostream->handle = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
                                                              if(diostream->handle < 0)
                                                                {
                                                                  diostream->SetEvent(DIOLINUXICMPFSMEVENT_DISCONNECTING);
                                                                  break;
                                                                }


                                                              if(diostream->config->IsServer() || (!diostream->config->GetLocalIP()->IsEmpty()))
                                                                {
                                                                  XSTRING IPstring;

                                                                  diostream->config->GetLocalIP()->GetXString(IPstring);

                                                                  loc_addr.sin_family = AF_INET;

                                                                  if(!diostream->config->GetLocalIP()->IsEmpty())
                                                                    {
                                                                      XBUFFER charstr;

                                                                      IPstring.ConvertToASCII(charstr);                                                                     
                                                                      loc_addr.sin_addr.s_addr      = inet_addr(charstr.GetPtrChar());                                                                      
                                                                    } else loc_addr.sin_addr.s_addr = htonl(INADDR_ANY);

                                                                  loc_addr.sin_port = 0;

                                                                  if(bind(diostream->handle, (struct sockaddr *)&loc_addr, sizeof(loc_addr)) == -1)
                                                                    {
                                                                      diostream->SetEvent(DIOLINUXICMPFSMEVENT_DISCONNECTING);
                                                                      break;
                                                                    }
                                                                }

                                                              if(!diostream->config->IsServer())
                                                                {
                                                                  diostream->config->GetRemoteURL()->ResolveURL(diostream->remoteaddress);
                                                                }

                                                              fcntl(diostream->handle, F_SETFL, fcntl(diostream->handle, F_GETFL, 0) | O_NONBLOCK);

                                                            }
                                                            break;


              case DIOLINUXICMPFSMSTATE_CONNECTED         : { DIOSTREAM_XEVENT xevent(diostream, DIOSTREAM_XEVENT_TYPE_CONNECTED);
                                                              xevent.SetDIOStream(diostream);
                                                              diostream->PostEvent(&xevent);

                                                              diostream->SetEvent(DIOLINUXICMPFSMEVENT_WAITINGTOREAD);
                                                            }
                                                            break;

              case DIOLINUXICMPFSMSTATE_WAITINGTOREAD     : break;

              case DIOLINUXICMPFSMSTATE_SENDINGDATA       : break;

              case DIOLINUXICMPFSMSTATE_DISCONNECTING     : { DIOSTREAM_XEVENT xevent(diostream, DIOSTREAM_XEVENT_TYPE_DISCONNECTED);
                                                              xevent.SetDIOStream(diostream);
                                                              diostream->PostEvent(&xevent);

                                                              diostream->threadconnection->Run(false);
                                                              diostream->status        = DIOSTREAMSTATUS_DISCONNECTED;
                                                            }
                                                            break;

            }
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOLINUXSTREAMICMP::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOLINUXSTREAMICMP::Clean()
{
  threadconnection   = NULL;
  status            = DIOSTREAMSTATUS_DISCONNECTED;
  handle            = -1;
}




#endif

