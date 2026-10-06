/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOWINDOWSStreamICMP.cpp
* 
* @class      DIOWINDOWSSTREAMICMP
* @brief      WINDOWS Data Input/Output Stream ICMP class
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


#if defined(DIO_ACTIVE) && defined(DIO_STREAMICMP_ACTIVE)

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOWINDOWSStreamICMP.h"

#include "XFactory.h"
#include "XBuffer.h"
#include "XTrace.h"

#include "DIOIP.h"
#include "DIOURL.h"
#include "DIOStream_XEvent.h"
#include "DIOStreamICMPConfig.h"

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/



/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWINDOWSSTREAMICMP::DIOWINDOWSSTREAMICMP() : DIOSTREAMICMP(), XFSMACHINE(0)
* @brief      Constructor of class
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWINDOWSSTREAMICMP::DIOWINDOWSSTREAMICMP() : DIOSTREAMICMP(), XFSMACHINE(0)
{
  Clean();

  AddState( DIOWINDOWSICMPFSMSTATE_NONE                 ,
            DIOWINDOWSICMPFSMEVENT_GETTINGCONNECTION     , DIOWINDOWSICMPFSMSTATE_GETTINGCONNECTION   ,
            DIOWINDOWSICMPFSMEVENT_CONNECTED            , DIOWINDOWSICMPFSMSTATE_CONNECTED          ,
            DIOWINDOWSICMPFSMEVENT_DISCONNECTING        , DIOWINDOWSICMPFSMSTATE_DISCONNECTING      ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOWINDOWSICMPFSMSTATE_GETTINGCONNECTION     ,
            DIOWINDOWSICMPFSMEVENT_CONNECTED            , DIOWINDOWSICMPFSMSTATE_CONNECTED          ,
            DIOWINDOWSICMPFSMEVENT_WAITINGTOREAD        , DIOWINDOWSICMPFSMSTATE_WAITINGTOREAD      ,
            DIOWINDOWSICMPFSMEVENT_SENDINGDATA          , DIOWINDOWSICMPFSMSTATE_SENDINGDATA        ,
            DIOWINDOWSICMPFSMEVENT_DISCONNECTING        , DIOWINDOWSICMPFSMSTATE_DISCONNECTING      ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOWINDOWSICMPFSMSTATE_CONNECTED            ,
            DIOWINDOWSICMPFSMEVENT_GETTINGCONNECTION     , DIOWINDOWSICMPFSMSTATE_GETTINGCONNECTION   ,
            DIOWINDOWSICMPFSMEVENT_WAITINGTOREAD        , DIOWINDOWSICMPFSMSTATE_WAITINGTOREAD      ,
            DIOWINDOWSICMPFSMEVENT_SENDINGDATA          , DIOWINDOWSICMPFSMSTATE_SENDINGDATA        ,
            DIOWINDOWSICMPFSMEVENT_DISCONNECTING        , DIOWINDOWSICMPFSMSTATE_DISCONNECTING      ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOWINDOWSICMPFSMSTATE_WAITINGTOREAD        ,
            DIOWINDOWSICMPFSMEVENT_GETTINGCONNECTION     , DIOWINDOWSICMPFSMSTATE_GETTINGCONNECTION   ,
            DIOWINDOWSICMPFSMEVENT_CONNECTED            , DIOWINDOWSICMPFSMSTATE_CONNECTED          ,
            DIOWINDOWSICMPFSMEVENT_SENDINGDATA          , DIOWINDOWSICMPFSMSTATE_SENDINGDATA        ,
            DIOWINDOWSICMPFSMEVENT_DISCONNECTING        , DIOWINDOWSICMPFSMSTATE_DISCONNECTING      ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOWINDOWSICMPFSMSTATE_DISCONNECTING        ,
            DIOWINDOWSICMPFSMEVENT_GETTINGCONNECTION     , DIOWINDOWSICMPFSMSTATE_GETTINGCONNECTION   ,
            DIOWINDOWSICMPFSMEVENT_CONNECTED            , DIOWINDOWSICMPFSMSTATE_CONNECTED          ,
            DIOWINDOWSICMPFSMEVENT_WAITINGTOREAD        , DIOWINDOWSICMPFSMSTATE_WAITINGTOREAD      ,
            DIOWINDOWSICMPFSMEVENT_SENDINGDATA          , DIOWINDOWSICMPFSMSTATE_SENDINGDATA        ,
            XFSMACHINESTATE_EVENTDEFEND);

  threadconnection = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMICMP, _L("DIOWINDOWSSTREAMICMP::DIOWINDOWSSTREAMICMP"), ThreadConnection, (void*)this);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWINDOWSSTREAMICMP::~DIOWINDOWSSTREAMICMP()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWINDOWSSTREAMICMP::~DIOWINDOWSSTREAMICMP()
{
  if(threadconnection) DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMICMP, threadconnection);

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMICMP::Open()
* @brief      Open
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMICMP::Open()
{
  if(!threadconnection)  return false;

  if(!inbuffer)         return false;
  if(!outbuffer)        return false;

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

  SetEvent(DIOWINDOWSICMPFSMEVENT_GETTINGCONNECTION);

  status = DIOSTREAMSTATUS_GETTINGCONNECTION;

  ResetXBuffers();

  ResetConnectionStatistics();

  threadconnection->SetWaitYield(config->GetThreadWaitYield());
  threadconnection->SetPriority(config->GetThreadPriority());

  // Windows Sleep() defaults to the system scheduler tick (~15.6 ms): a Sleep(1) can
  // really sleep ~15 ms unless the process timer resolution is raised. Only requested
  // when the caller (e.g. DIOPING, for accurate RTT) asked for a tight cadence via
  // DIOSTREAMCONFIG::SetThreadWaitYield(); scoped to the lifetime of this stream, and
  // restored in Close(). Other DIOSTREAMICMP consumers keep today's behavior.
  if(config->GetThreadWaitYield() < DIOSTREAM_TIMEINWAITFUNCTIONS)
    {
      hastimeperiodactive = (timeBeginPeriod(1) == TIMERR_NOERROR);
    }

  return threadconnection->Ini();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMICMP::Disconnect()
* @brief      Disconnect
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMICMP::Disconnect()
{
  if((GetStatus()==DIOSTREAMSTATUS_GETTINGCONNECTION)||
     (GetStatus()==DIOSTREAMSTATUS_CONNECTED))
     {
       SetEvent(DIOWINDOWSICMPFSMEVENT_DISCONNECTING);

       while(GetStatus()!=DIOSTREAMSTATUS_DISCONNECTED)
        {
          Sleep(10);
        }
     }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMICMP::Close()
* @brief      Close
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMICMP::Close()
{
  if(!threadconnection) return false;

  if(hastimeperiodactive)
    {
      timeEndPeriod(1);
      hastimeperiodactive = false;
    }

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
* @fn         int DIOWINDOWSSTREAMICMP::IsReadyConnect(SOCKET socket)
* @brief      Is ready connect
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  socket : Socket value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOWINDOWSSTREAMICMP::IsReadyConnect(SOCKET socket)
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
* @fn         void DIOWINDOWSSTREAMICMP::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWINDOWSSTREAMICMP::Clean()
{
  threadconnection     = NULL;
  status               = DIOSTREAMSTATUS_DISCONNECTED;
  handle               = INVALID_SOCKET;

  hastimeperiodactive  = false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWINDOWSSTREAMICMP::ThreadConnection(void* data)
* @brief      Thread connection
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  data : Data buffer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWINDOWSSTREAMICMP::ThreadConnection(void* data)
{
  DIOWINDOWSSTREAMICMP* diostream = (DIOWINDOWSSTREAMICMP*)data;
  if(!diostream) return;

  if(diostream->GetEvent()==DIOWINDOWSICMPFSMEVENT_NONE) // No hay nuevos Eventos
    {
      switch(diostream->GetCurrentState())
        {
          case DIOWINDOWSICMPFSMSTATE_NONE              : break;

          case DIOWINDOWSICMPFSMSTATE_GETTINGCONNECTION : switch(diostream->IsReadyConnect(diostream->handle))
                                                            {
                                                              case -1:  diostream->SetEvent(DIOWINDOWSICMPFSMEVENT_DISCONNECTING);
                                                                        break;

                                                              case  1:  diostream->SetEvent(DIOWINDOWSICMPFSMEVENT_CONNECTED);
                                                                        diostream->status = DIOSTREAMSTATUS_CONNECTED;
                                                                        break;

                                                              default:  diostream->status = DIOSTREAMSTATUS_GETTINGCONNECTION;
                                                                        break;
                                                            }
                                                          break;

          case DIOWINDOWSICMPFSMSTATE_CONNECTED        :  break;

          case DIOWINDOWSICMPFSMSTATE_WAITINGTOREAD    :  { struct timeval  waitd;
                                                            fd_set          read_flags;
                                                            fd_set          write_flags;

                                                            if(diostream->handle==INVALID_SOCKET)
                                                              {
                                                                diostream->SetEvent(DIOWINDOWSICMPFSMEVENT_DISCONNECTING);
                                                                break;
                                                              }

                                                            waitd.tv_sec  = 0;
                                                            waitd.tv_usec = 100;

                                                            FD_ZERO(&read_flags);
                                                            FD_ZERO(&write_flags);

                                                            FD_SET(diostream->handle, &read_flags);
                                                            if(diostream->GetFirstDatagram(true) != DIOSTREAMICMP_NOTFOUND) FD_SET(diostream->handle, &write_flags);

                                                            int error = select((int)(diostream->handle)+1, &read_flags, &write_flags, (fd_set*)0, &waitd);
                                                            if(error==SOCKET_ERROR)
                                                              {
                                                                diostream->SetEvent(DIOWINDOWSICMPFSMEVENT_DISCONNECTING);
                                                                break;
                                                              }

                                                            if(FD_ISSET(diostream->handle, &read_flags))  //Socket ready for reading
                                                              {
                                                                FD_CLR(diostream->handle, &read_flags);

                                                                if(!diostream->IsBlockRead())
                                                                  {
                                                                    XBYTE buffer[DIOSTREAM_MAXBUFFER];

                                                                    SOCKADDR_IN  origin_addr;
                                                                    int          size_addr = sizeof(SOCKADDR_IN);

                                                                    memset(&origin_addr, 0, size_addr);

                                                                    XDWORD size = recvfrom(diostream->handle, (char*)buffer, DIOSTREAM_MAXBUFFER, 0, (sockaddr*)&origin_addr, &size_addr);

                                                                    if(size == SOCKET_ERROR)
                                                                      {
                                                                        diostream->SetEvent(DIOWINDOWSICMPFSMEVENT_DISCONNECTING);
                                                                        break;
                                                                      }

                                                                    if(size)
                                                                      {
                                                                        XSTRING address;

                                                                        address.Format(_L("%d.%d.%d.%d"), origin_addr.sin_addr.S_un.S_un_b.s_b1, origin_addr.sin_addr.S_un.S_un_b.s_b2, origin_addr.sin_addr.S_un.S_un_b.s_b3, origin_addr.sin_addr.S_un.S_un_b.s_b4);

                                                                        diostream->AddDatagram(false, address.Get(), (XBYTE*)buffer, size);
                                                                        diostream->inbuffer->Add(buffer, size);

                                                                      }
                                                                     else
                                                                      {
                                                                        diostream->SetEvent(DIOWINDOWSICMPFSMEVENT_DISCONNECTING);
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
                                                                    if(indexdatagram!=DIOSTREAMICMP_NOTFOUND)
                                                                      {
                                                                        DIOSTREAMICMPDATAGRAM* datagram = (DIOSTREAMICMPDATAGRAM*)diostream->GetDatagram(indexdatagram);
                                                                        if(datagram)
                                                                          {
                                                                            SOCKADDR_IN  target_addr;
                                                                            int          size_addr = sizeof(SOCKADDR_IN);
                                                                            int          size;                                                                            

                                                                            memset(&target_addr, 0, size_addr);

                                                                            target_addr.sin_family = AF_INET;

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
                                                                            
                                                                            target_addr.sin_port  = 0;

                                                                            size = sendto(diostream->handle, (char*)datagram->GetData()->Get(), datagram->GetData()->GetSize(), 0, (sockaddr*)&target_addr, size_addr);

                                                                            if(size == SOCKET_ERROR)
                                                                              {
                                                                                //XTRACE_PRINTCOLOR(4, _L("Write ICMP to [%s] (%d) ERROR!"), tmpremoteaddress.Get(), size);

                                                                                diostream->SetEvent(DIOWINDOWSICMPFSMEVENT_DISCONNECTING);
                                                                                break;
                                                                              }

                                                                            if(size)
                                                                              {
                                                                                //XTRACE_PRINTCOLOR(1, _L("Write ICMP to [%s] (%d)"), tmpremoteaddress.Get(), size);

                                                                                diostream->outbuffer->Extract(NULL, 0 , datagram->GetData()->GetSize());
                                                                                diostream->DeleteDatagram(indexdatagram);
                                                                              }
                                                                          }
                                                                      }
                                                                  }
                                                              }
                                                          }
                                                          break;

          case DIOWINDOWSICMPFSMSTATE_SENDINGDATA      :  break;

          case DIOWINDOWSICMPFSMSTATE_DISCONNECTING    :  break;

        }
    }
   else
    {
      if(diostream->GetEvent()<DIOWINDOWSICMP_LASTEVENT)
        {
          diostream->CheckTransition();

          switch(diostream->GetCurrentState())
            {
              case DIOWINDOWSICMPFSMSTATE_NONE                : break;

              case DIOWINDOWSICMPFSMSTATE_GETTINGCONNECTION   : { SOCKADDR_IN loc_addr;

                                                                  memset(&loc_addr, 0, sizeof(SOCKADDR_IN));

                                                                  diostream->handle = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
                                                                  if(diostream->handle == INVALID_SOCKET)
                                                                    {
                                                                      diostream->SetEvent(DIOWINDOWSICMPFSMEVENT_DISCONNECTING);
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
                                                                          #ifndef BUILDER
                                                                          inet_pton(loc_addr.sin_family, charstr.GetPtrChar(), &loc_addr.sin_addr.s_addr);
                                                                          #else
                                                                          loc_addr.sin_addr.s_addr  = inet_addr(charstr.GetPtrChar());
                                                                          #endif                                                                          

                                                                        } else loc_addr.sin_addr.s_addr = htonl(INADDR_ANY);

                                                                      loc_addr.sin_port = 0;

                                                                      if(bind(diostream->handle, (LPSOCKADDR)&loc_addr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
                                                                        {
                                                                          diostream->SetEvent(DIOWINDOWSICMPFSMEVENT_DISCONNECTING);
                                                                          break;
                                                                        }
                                                                    }                                                                 

                                                                  /*
                                                                  int opt = 1;
                                                                  if(setsockopt(diostream->handle, IPPROTO_IP, IP_HDRINCL, (char*)&opt, sizeof(opt)) == SOCKET_ERROR)
                                                                    {
                                                                      diostream->SetEvent(DIOWINDOWSICMPFSMEVENT_DISCONNECTING);
                                                                      break;
                                                                    }
                                                                  */

                                                                  DWORD nonblock = 1;
                                                                  if (ioctlsocket(diostream->handle, FIONBIO, &nonblock) == SOCKET_ERROR)
                                                                    {
                                                                      diostream->SetEvent(DIOWINDOWSICMPFSMEVENT_DISCONNECTING);
                                                                      break;
                                                                    }
                                                                }
                                                                break;

              case DIOWINDOWSICMPFSMSTATE_CONNECTED         : { DIOSTREAM_XEVENT xevent(diostream, DIOSTREAM_XEVENT_TYPE_CONNECTED);
                                                                xevent.SetDIOStream(diostream);

                                                                diostream->PostEvent(&xevent);

                                                                diostream->SetEvent(DIOWINDOWSICMPFSMEVENT_WAITINGTOREAD);
                                                              }
                                                              break;

              case DIOWINDOWSICMPFSMSTATE_WAITINGTOREAD     : break;

              case DIOWINDOWSICMPFSMSTATE_SENDINGDATA       : break;

              case DIOWINDOWSICMPFSMSTATE_DISCONNECTING     : { DIOSTREAM_XEVENT xevent(diostream, DIOSTREAM_XEVENT_TYPE_DISCONNECTED);
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





#endif

