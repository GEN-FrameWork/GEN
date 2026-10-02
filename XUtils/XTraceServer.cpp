/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XTraceServer.cpp
* 
* @class      XTRACESERVER
* @brief      XTRACE receive server (UDP/UART) with bounded thread-safe queue
* @ingroup    XUTILS
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

#include "XTraceServer.h"

#if defined(DIO_ACTIVE) && (defined(DIO_STREAMUDP_ACTIVE) || defined(DIO_STREAMUART_ACTIVE))

#include "XFactory.h"
#include "XSleep.h"
#include "XTimer.h"
#include "XThreadCollected.h"
#include "XTrace.h"
#include "XBuffer.h"

#include "DIOFactory.h"

#if defined(DIO_STREAMUDP_ACTIVE)
#include "DIOIP.h"
#include "DIOStreamUDPConfig.h"
#include "DIOStreamUDP.h"
#endif

#if defined(DIO_STREAMUART_ACTIVE)
#include "DIOStreamUARTConfig.h"
#include "DIOStreamUART.h"
#endif



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTRACESERVER_MSG::XTRACESERVER_MSG()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTRACESERVER_MSG::XTRACESERVER_MSG()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTRACESERVER_MSG::~XTRACESERVER_MSG()
* @brief      Destructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTRACESERVER_MSG::~XTRACESERVER_MSG()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACESERVER_MSG::CopyFrom(XTRACESERVER_MSG* origin)
* @brief      Copy from
* @ingroup    XUTILS
* 
* @param[in]  origin : Origin message.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACESERVER_MSG::CopyFrom(XTRACESERVER_MSG* origin)
{
  if(!origin) return false;

  return CopyFrom(*origin);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACESERVER_MSG::CopyFrom(XTRACESERVER_MSG& origin)
* @brief      Copy from
* @ingroup    XUTILS
* 
* @param[in]  origin : Origin message.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACESERVER_MSG::CopyFrom(XTRACESERVER_MSG& origin)
{
  publicIP  = origin.publicIP;
  localIP   = origin.localIP;
  level     = origin.level;
  sequence  = origin.sequence;

  time.CopyFrom(origin.time);
  text = origin.text;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTRACESERVER_MSG::Clean()
* @brief      Clean
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTRACESERVER_MSG::Clean()
{
  publicIP  = 0;
  localIP   = 0;
  level     = 0;
  sequence  = 0;

  time.SetToZero();
  text.Empty();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTRACESERVER::XTRACESERVER()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTRACESERVER::XTRACESERVER()
{
  Clean();

  xmutexmessages = GEN_XFACTORY.Create_Mutex();

#if defined(DIO_STREAMUDP_ACTIVE)
  xthreadreadUDP = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAM, __L("XTRACESERVER::ReadUDP"), ThreadReadUDPFunction, (void*)this);
  if(xthreadreadUDP)
    {
      xthreadreadUDP->Ini(false);
    }
#endif

#if defined(DIO_STREAMUART_ACTIVE)
  xthreadreadUART = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAM, __L("XTRACESERVER::ReadUART"), ThreadReadUARTFunction, (void*)this);
  if(xthreadreadUART)
    {
      xthreadreadUART->Ini(false);
    }
#endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTRACESERVER::~XTRACESERVER()
* @brief      Destructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTRACESERVER::~XTRACESERVER()
{
  End();

#if defined(DIO_STREAMUDP_ACTIVE)
  if(xthreadreadUDP)
    {
      xthreadreadUDP->End();
      DELETEXTHREAD(XTHREADGROUPID_DIOSTREAM, xthreadreadUDP);
      xthreadreadUDP = NULL;
    }
#endif

#if defined(DIO_STREAMUART_ACTIVE)
  if(xthreadreadUART)
    {
      xthreadreadUART->End();
      DELETEXTHREAD(XTHREADGROUPID_DIOSTREAM, xthreadreadUART);
      xthreadreadUART = NULL;
    }
#endif

  if(xmutexmessages)
    {
      GEN_XFACTORY.Delete_Mutex(xmutexmessages);
      xmutexmessages = NULL;
    }

  Clean();
}


#if defined(DIO_STREAMUDP_ACTIVE)

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACESERVER::Ini(XWORD port)
* @brief      Ini
* @ingroup    XUTILS
* 
* @param[in]  port : UDP listen port.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACESERVER::Ini(XWORD port)
{
  XSTRING config;

  config.Format(__L("*:%d"), port);

  return Ini(&config);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACESERVER::Ini(XCHAR* config)
* @brief      Ini
* @ingroup    XUTILS
* 
* @param[in]  config : DIOSTREAMUDPCONFIG string (host:port).
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACESERVER::Ini(XCHAR* config)
{
  if(!config) return false;

  XSTRING string(config);

  return Ini(&string);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACESERVER::Ini(XSTRING& config)
* @brief      Ini
* @ingroup    XUTILS
* 
* @param[in]  config : DIOSTREAMUDPCONFIG string (host:port).
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACESERVER::Ini(XSTRING& config)
{
  return Ini(&config);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACESERVER::Ini(XSTRING* config)
* @brief      Ini
* @ingroup    XUTILS
* 
* @param[in]  config : DIOSTREAMUDPCONFIG string (host:port).
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACESERVER::Ini(XSTRING* config)
{
  return OpenUDP(config);
}

#endif


#if defined(DIO_STREAMUART_ACTIVE)

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACESERVER::IniUART(XCHAR* config)
* @brief      Ini UART
* @ingroup    XUTILS
* 
* @param[in]  config : DIOSTREAMUARTCONFIG string.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACESERVER::IniUART(XCHAR* config)
{
  if(!config) return false;

  XSTRING string(config);

  return IniUART(&string);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACESERVER::IniUART(XSTRING& config)
* @brief      Ini UART
* @ingroup    XUTILS
* 
* @param[in]  config : DIOSTREAMUARTCONFIG string.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACESERVER::IniUART(XSTRING& config)
{
  return IniUART(&config);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACESERVER::IniUART(XSTRING* config)
* @brief      Ini UART
* @ingroup    XUTILS
* 
* @param[in]  config : DIOSTREAMUARTCONFIG string.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACESERVER::IniUART(XSTRING* config)
{
  return OpenUART(config);
}

#endif


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACESERVER::End()
* @brief      End
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACESERVER::End()
{
#if defined(DIO_STREAMUDP_ACTIVE)
  CloseUDP();
#endif

#if defined(DIO_STREAMUART_ACTIVE)
  CloseUART();
#endif

  Clear();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACESERVER::IsOpen()
* @brief      Is open
* @ingroup    XUTILS
* 
* @return     bool : true if any transport is open.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACESERVER::IsOpen()
{
  bool open = false;

#if defined(DIO_STREAMUDP_ACTIVE)
  if(isopenudp) open = true;
#endif

#if defined(DIO_STREAMUART_ACTIVE)
  if(isopenuart) open = true;
#endif

  return open;
}


#if defined(DIO_STREAMUDP_ACTIVE)

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACESERVER::IsOpenUDP()
* @brief      Is open UDP
* @ingroup    XUTILS
* 
* @return     bool : true if UDP is open.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACESERVER::IsOpenUDP()
{
  return isopenudp;
}

#endif


#if defined(DIO_STREAMUART_ACTIVE)

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACESERVER::IsOpenUART()
* @brief      Is open UART
* @ingroup    XUTILS
* 
* @return     bool : true if UART is open.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACESERVER::IsOpenUART()
{
  return isopenuart;
}

#endif


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XTRACESERVER::GetMaxMessages()
* @brief      Get max messages
* @ingroup    XUTILS
* 
* @return     XDWORD : Max queued messages.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XTRACESERVER::GetMaxMessages()
{
  return maxmessages;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTRACESERVER::SetMaxMessages(XDWORD maxmessages)
* @brief      Set max messages
* @ingroup    XUTILS
* 
* @param[in]  maxmessages : Max queued messages (0 keeps current).
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTRACESERVER::SetMaxMessages(XDWORD maxmessages)
{
  if(!maxmessages) return;

  this->maxmessages = maxmessages;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XTRACESERVER::GetCount()
* @brief      Get count
* @ingroup    XUTILS
* 
* @return     XDWORD : Number of queued messages.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XTRACESERVER::GetCount()
{
  XDWORD count = 0;

  if(xmutexmessages) xmutexmessages->Lock();

  count = messages.GetSize();

  if(xmutexmessages) xmutexmessages->UnLock();

  return count;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XTRACESERVER::GetDroppedCount()
* @brief      Get dropped count
* @ingroup    XUTILS
* 
* @return     XDWORD : Number of dropped (oldest) messages.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XTRACESERVER::GetDroppedCount()
{
  XDWORD count = 0;

  if(xmutexmessages) xmutexmessages->Lock();

  count = droppedcount;

  if(xmutexmessages) xmutexmessages->UnLock();

  return count;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACESERVER::Clear()
* @brief      Clear
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACESERVER::Clear()
{
  if(xmutexmessages) xmutexmessages->Lock();

  messages.DeleteContentsInstanced();
  messages.DeleteAll();

  if(xmutexmessages) xmutexmessages->UnLock();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACESERVER::Pop(XTRACESERVER_MSG& msg)
* @brief      Pop
* @ingroup    XUTILS
* 
* @param[out] msg : Destination message.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACESERVER::Pop(XTRACESERVER_MSG& msg)
{
  bool status = false;

  if(xmutexmessages) xmutexmessages->Lock();

  if(messages.GetSize())
    {
      XTRACESERVER_MSG* first = messages.Get(0);
      if(first)
        {
          status = msg.CopyFrom(first);
          delete first;
        }

      messages.DeleteIndex(0);
    }

  if(xmutexmessages) xmutexmessages->UnLock();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACESERVER::Peek(XDWORD index, XTRACESERVER_MSG& msg)
* @brief      Peek
* @ingroup    XUTILS
* 
* @param[in]  index : Queue index (0 = oldest).
* @param[out] msg : Destination message.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACESERVER::Peek(XDWORD index, XTRACESERVER_MSG& msg)
{
  bool status = false;

  if(xmutexmessages) xmutexmessages->Lock();

  XTRACESERVER_MSG* entry = messages.Get(index);
  if(entry)
    {
      status = msg.CopyFrom(entry);
    }

  if(xmutexmessages) xmutexmessages->UnLock();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACESERVER::WaitPop(XTRACESERVER_MSG& msg, int timeout_ms)
* @brief      Wait pop
* @ingroup    XUTILS
* 
* @param[out] msg : Destination message.
* @param[in]  timeout_ms : Timeout in ms (-1 = infinite, 0 = non-blocking).
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACESERVER::WaitPop(XTRACESERVER_MSG& msg, int timeout_ms)
{
  if(timeout_ms == 0)
    {
      return Pop(msg);
    }

  XTIMER* xtimer = NULL;

  if(timeout_ms > 0)
    {
      xtimer = GEN_XFACTORY.CreateTimer();
      if(xtimer) xtimer->Reset();
    }

  bool status = false;

  for(;;)
    {
      if(Pop(msg))
        {
          status = true;
          break;
        }

      if(!IsOpen()) break;

      if(xtimer)
        {
          if((int)xtimer->GetMeasureMilliSeconds() >= timeout_ms) break;
        }

      GEN_XSLEEP.MilliSeconds(1);
    }

  if(xtimer)
    {
      GEN_XFACTORY.DeleteTimer(xtimer);
    }

  return status;
}


#if defined(DIO_STREAMUDP_ACTIVE)

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACESERVER::OpenUDP(XSTRING* config)
* @brief      Open UDP
* @ingroup    XUTILS
* 
* @param[in]  config : DIOSTREAMUDPCONFIG string (host:port).
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACESERVER::OpenUDP(XSTRING* config)
{
  if(!config) return false;
  if(config->IsEmpty()) return false;

  if(isopenudp)
    {
      CloseUDP();
    }

  if(isopenudp) return false;

  if(!diostreamudpcfg) diostreamudpcfg = new DIOSTREAMUDPCONFIG();
  if(!diostreamudpcfg) return false;

  diostreamudpcfg->SetFromString(config);
  diostreamudpcfg->SetMode(DIOSTREAMMODE_SERVER);
  diostreamudpcfg->SetIsUsedDatagrams(true);
  diostreamudpcfg->SetSizeBufferSO(10000*1024);
  diostreamudpcfg->SetThreadPriority(XTHREADPRIORITY_REALTIME);
  diostreamudpcfg->SetThreadWaitYield(0);

  diostreamudp = (DIOSTREAMUDP*)GEN_DIOFACTORY.CreateStreamIO(diostreamudpcfg);
  if(diostreamudp)
    {
      if(diostreamudp->Open())
        {
          isopenudp = true;

          if(xthreadreadUDP)
            {
              xthreadreadUDP->Run(true);
            }
        }
    }

  if(!isopenudp)
    {
      CloseUDP();
    }

  return isopenudp;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACESERVER::CloseUDP()
* @brief      Close UDP
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACESERVER::CloseUDP()
{
  isopenudp = false;

  if(xthreadreadUDP) xthreadreadUDP->Run(false);

  if(diostreamudp)
    {
      diostreamudp->Disconnect();
      diostreamudp->WaitToDisconnected(10);
      diostreamudp->Close();

      GEN_DIOFACTORY.DeleteStreamIO(diostreamudp);
      diostreamudp = NULL;
    }

  if(diostreamudpcfg)
    {
      delete diostreamudpcfg;
      diostreamudpcfg = NULL;
    }

  return true;
}

#endif


#if defined(DIO_STREAMUART_ACTIVE)

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACESERVER::OpenUART(XSTRING* config)
* @brief      Open UART
* @ingroup    XUTILS
* 
* @param[in]  config : DIOSTREAMUARTCONFIG string.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACESERVER::OpenUART(XSTRING* config)
{
  if(!config) return false;
  if(config->IsEmpty()) return false;

  if(isopenuart)
    {
      CloseUART();
    }

  if(isopenuart) return false;

  if(!diostreamuartcfg) diostreamuartcfg = new DIOSTREAMUARTCONFIG();
  if(!diostreamuartcfg) return false;

  if(!diostreamuartcfg->SetFromString(config))
    {
      CloseUART();
      return false;
    }

  diostreamuart = (DIOSTREAMUART*)GEN_DIOFACTORY.CreateStreamIO(diostreamuartcfg);
  if(diostreamuart)
    {
      if(diostreamuart->Open())
        {
          isopenuart = true;

          if(xthreadreadUART)
            {
              xthreadreadUART->Run(true);
            }
        }
    }

  if(!isopenuart)
    {
      CloseUART();
    }

  return isopenuart;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACESERVER::CloseUART()
* @brief      Close UART
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACESERVER::CloseUART()
{
  isopenuart = false;

  if(xthreadreadUART) xthreadreadUART->Run(false);

  if(diostreamuart)
    {
      diostreamuart->Close();

      GEN_DIOFACTORY.DeleteStreamIO(diostreamuart);
      diostreamuart = NULL;
    }

  if(diostreamuartcfg)
    {
      delete diostreamuartcfg;
      diostreamuartcfg = NULL;
    }

  return true;
}

#endif


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACESERVER::PushParsedPacket(XBUFFER& packet, XDWORD publicIP, XDWORD localIP)
* @brief      Push parsed packet
* @ingroup    XUTILS
* 
* @param[in]  packet : Raw XTRACE packet buffer (consumed on success).
* @param[in]  publicIP : Public IP stored in the queued message.
* @param[in]  localIP : Local IP stored in the queued message.
* 
* @return     bool : true if a message was queued; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACESERVER::PushParsedPacket(XBUFFER& packet, XDWORD publicIP, XDWORD localIP)
{
  if(!XTRACE::instance) return false;

  XDWORD     packetpublicIP = 0;
  XDWORD     packetlocalIP  = 0;
  XBYTE      level          = 0;
  XDWORD     sequence       = 0;
  XDATETIME  xtime;
  XBUFFER    data;
  XSTRING    string;

  xtime.SetToZero();

  if(XTRACE::instance->GetTraceFromXBuffer(packet, packetpublicIP, packetlocalIP, level, sequence, &xtime, data))
    {
      return false;
    }

  XTRACE::instance->SetTraceDataToText(data, string);

  return PushMessage(publicIP, localIP, level, sequence, xtime, string);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACESERVER::PushMessage(XDWORD publicIP, XDWORD localIP, XBYTE level, XDWORD sequence, XDATETIME& xtime, XSTRING& text)
* @brief      Push message
* @ingroup    XUTILS
* 
* @param[in]  publicIP : Public IP packed.
* @param[in]  localIP : Local IP packed.
* @param[in]  level : Trace level.
* @param[in]  sequence : Sequence.
* @param[in]  xtime : Trace time.
* @param[in]  text : Trace text.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACESERVER::PushMessage(XDWORD publicIP, XDWORD localIP, XBYTE level, XDWORD sequence, XDATETIME& xtime, XSTRING& text)
{
  XTRACESERVER_MSG* msg = new XTRACESERVER_MSG();
  if(!msg) return false;

  msg->publicIP = publicIP;
  msg->localIP  = localIP;
  msg->level    = level;
  msg->sequence = sequence;
  msg->time.CopyFrom(xtime);
  msg->text     = text;

  if(xmutexmessages) xmutexmessages->Lock();

  while(maxmessages && (messages.GetSize() >= maxmessages))
    {
      XTRACESERVER_MSG* oldest = messages.Get(0);
      if(oldest) delete oldest;

      messages.DeleteIndex(0);
      droppedcount++;
    }

  messages.Add(msg);

  if(xmutexmessages) xmutexmessages->UnLock();

  return true;
}


#if defined(DIO_STREAMUDP_ACTIVE)

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTRACESERVER::ThreadReadUDPFunction(void* param)
* @brief      Thread read UDP function
* @ingroup    XUTILS
* 
* @param[in]  param : XTRACESERVER instance.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTRACESERVER::ThreadReadUDPFunction(void* param)
{
  XTRACESERVER* server = (XTRACESERVER*)param;
  if(!server) return;
  if(!server->isopenudp) return;
  if(!server->diostreamudp) return;
  if(!XTRACE::instance) return;

  bool addmessage = true;

  while(addmessage)
    {
      XDWORD     publicIP  = 0;
      XDWORD     localIP   = 0;
      XBYTE      level     = 0;
      XDWORD     sequence  = 0;
      XDATETIME  xtime;
      XBUFFER    data;
      XSTRING    string;
      XSTRING    address;
      XWORD      port      = 0;

      if(!server->isopenudp)
        {
          break;
        }

      xtime.SetToZero();
      string.Empty();

      server->diostreamudp->ReadDatagram(address, port, data);

      XDWORD error = XTRACE::instance->GetTraceFromXBuffer(data, publicIP, localIP, level, sequence, &xtime, data);
      if(!error)
        {
          if((!publicIP) && (!address.IsEmpty()))
            {
              DIOIP pIP;

              pIP.Set(address);
              if(!pIP.IsLocal() && !pIP.IsAPIPA())
                {
                  publicIP = (pIP.Get()[0]<<24) | (pIP.Get()[1]<<16) | (pIP.Get()[2]<<8) | (pIP.Get()[3]);

                  if(publicIP == localIP)
                    {
                      publicIP = 0;
                    }
                }
            }

          XTRACE::instance->SetTraceDataToText(data, string);
          server->PushMessage(publicIP, localIP, level, sequence, xtime, string);
        }
       else
        {
          addmessage = false;
        }
    }
}

#endif


#if defined(DIO_STREAMUART_ACTIVE)

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTRACESERVER::ThreadReadUARTFunction(void* param)
* @brief      Thread read UART function
* @ingroup    XUTILS
* 
* @param[in]  param : XTRACESERVER instance.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTRACESERVER::ThreadReadUARTFunction(void* param)
{
  XTRACESERVER* server = (XTRACESERVER*)param;
  if(!server) return;
  if(!server->isopenuart) return;
  if(!server->diostreamuart) return;
  if(!XTRACE::instance) return;

  bool addmessage = true;

  while(addmessage)
    {
      if(!server->isopenuart)
        {
          break;
        }

      XBUFFER* inbuffer = server->diostreamuart->GetInXBuffer();
      if(!inbuffer)
        {
          break;
        }

      if(!server->PushParsedPacket((*inbuffer), 0xFFFFFFFF, 0xFFFFFFFF))
        {
          addmessage = false;
        }
    }
}

#endif


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTRACESERVER::Clean()
* @brief      Clean
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTRACESERVER::Clean()
{
  maxmessages       = XTRACESERVER_DEFAULT_MAXMESSAGES;
  droppedcount      = 0;
  xmutexmessages    = NULL;

#if defined(DIO_STREAMUDP_ACTIVE)
  isopenudp         = false;
  diostreamudpcfg   = NULL;
  diostreamudp      = NULL;
  xthreadreadUDP    = NULL;
#endif

#if defined(DIO_STREAMUART_ACTIVE)
  isopenuart        = false;
  diostreamuartcfg  = NULL;
  diostreamuart     = NULL;
  xthreadreadUART   = NULL;
#endif
}


#endif
