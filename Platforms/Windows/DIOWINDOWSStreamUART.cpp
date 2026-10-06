/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOWINDOWSStreamUART.cpp
* 
* @class      DIOWINDOWSSTREAMUART
* @brief      WINDOWS Data Input/Output Stream UART class
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



#if defined(DIO_ACTIVE) && defined(DIO_STREAMUART_ACTIVE)


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOWINDOWSStreamUART.h"

#include "XFactory.h"
#include "XTimer.h"
#include "XString.h"
#include "XBuffer.h"
#include "XTrace.h"

#include "DIOStream_XEvent.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWINDOWSSTREAMUART::DIOWINDOWSSTREAMUART() : DIOSTREAMUART(), XFSMACHINE(0)
* @brief      Constructor of class
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWINDOWSSTREAMUART::DIOWINDOWSSTREAMUART() : DIOSTREAMUART(), XFSMACHINE(0)
{
  Clean();

  AddState( DIOWINDOWSUARTFSMSTATE_NONE               ,
            DIOWINDOWSUARTFSMEVENT_CONNECTED            , DIOWINDOWSUARTFSMSTATE_CONNECTED         ,
            DIOWINDOWSUARTFSMEVENT_DISCONNECTING        , DIOWINDOWSUARTFSMSTATE_DISCONNECTING     ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOWINDOWSUARTFSMSTATE_CONNECTED            ,
            DIOWINDOWSUARTFSMEVENT_WAITINGTOREAD        , DIOWINDOWSUARTFSMSTATE_WAITINGTOREAD     ,
            DIOWINDOWSUARTFSMEVENT_DISCONNECTING        , DIOWINDOWSUARTFSMSTATE_DISCONNECTING     ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOWINDOWSUARTFSMSTATE_WAITINGTOREAD        ,
            DIOWINDOWSUARTFSMEVENT_CONNECTED            , DIOWINDOWSUARTFSMSTATE_CONNECTED         ,
            DIOWINDOWSUARTFSMEVENT_DISCONNECTING        , DIOWINDOWSUARTFSMSTATE_DISCONNECTING     ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOWINDOWSUARTFSMSTATE_DISCONNECTING        ,
            DIOWINDOWSUARTFSMEVENT_CONNECTED            , DIOWINDOWSUARTFSMSTATE_CONNECTED         ,
            DIOWINDOWSUARTFSMEVENT_WAITINGTOREAD        , DIOWINDOWSUARTFSMSTATE_WAITINGTOREAD     ,
            XFSMACHINESTATE_EVENTDEFEND);

  threadconnection = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMUART, _L("DIOWINDOWSSTREAMUART::DIOWINDOWSSTREAMUART"), ThreadConnection, (void*)this);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWINDOWSSTREAMUART::~DIOWINDOWSSTREAMUART()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWINDOWSSTREAMUART::~DIOWINDOWSSTREAMUART()
{
  Close();

  if(threadconnection) DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMUART, threadconnection);

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMSTATUS DIOWINDOWSSTREAMUART::GetStatus()
* @brief      Get status
* @ingroup    PLATFORM_WINDOWS
* 
* @return     DIOSTREAMSTATUS : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMSTATUS DIOWINDOWSSTREAMUART::GetStatus()
{
  if(hcom==INVALID_HANDLE_VALUE)  return DIOSTREAMSTATUS_DISCONNECTED;
  if(!config)                     return DIOSTREAMSTATUS_DISCONNECTED;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMUART::Open()
* @brief      Open
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMUART::Open()
{
  if(!threadconnection)
    {
      return false;
    }

  if(!inbuffer)         
    {
      return false;
    }

  if(!outbuffer)        
    {
      return false;
    }

  if(!config)           
    {
      return false;
    }

  XSTRING comport;
  XSTRING cfg;

  comport.Empty();

  if(!config->GetLocalDeviceName()->IsEmpty())
    {
      comport = config->GetLocalDeviceName()->Get();
    }
   else
    {
      comport.Format(_L("\\\\.\\COM%d"), config->GetPort());
    }

  hcom = CreateFile(comport.Get(),
                    GENERIC_READ|GENERIC_WRITE,
                    0,
                    0,
                    OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL,  // FILE_FLAG_OVERLAPPED
                    0);

  if(hcom==INVALID_HANDLE_VALUE) return false;

  if(!SetTimeouts())
    {
      Close();
      return false;
    }

// EV_ERR|EV_BREAK|EV_CTS|EV_DSR|EV_RING|EV_RLSD|EV_RXCHAR|EV_RXFLAG|EV_TXEMPTY

  if(!SetMask(EV_ERR|EV_RXCHAR|EV_TXEMPTY))
    {
      Close();
      return false;
    }

  if(!SetupComm(hcom, DIOSTREAM_MAXBUFFER, DIOSTREAM_MAXBUFFER))
    {
      Close();
      return false;
    }

  SetRTS(false);
  SetDTR(false);


  Config();

  memset(&ovi, 0, sizeof(ovi));

  SetEvent(DIOWINDOWSUARTFSMEVENT_CONNECTED);

  status = DIOSTREAMSTATUS_CONNECTED;

  ResetXBuffers();

  ResetConnectionStatistics();

  return threadconnection->Ini();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMUART::Config(XWORD mask)
* @brief      Config
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  mask : Mask value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMUART::Config(XWORD mask)
{
  if(!config) return false;
  // if(GetStatus()==DIOSTREAMSTATUS_DISCONNECTED) return false;

  DCB dcb;

  if(!GetCommState(hcom, &dcb)) return false;

  if((mask&DIOSTREAMUARTMASK_BAUDRATE) == DIOSTREAMUARTMASK_BAUDRATE)  dcb.BaudRate = config->GetBaudRate();

  if((mask&DIOSTREAMUARTMASK_DATABITS ) == DIOSTREAMUARTMASK_DATABITS)
    {
      switch(config->GetDataBits())
        {
          case DIOSTREAMUARTDATABIT_5             :  dcb.ByteSize = 5;          break;
          case DIOSTREAMUARTDATABIT_6             :  dcb.ByteSize = 6;          break;
          case DIOSTREAMUARTDATABIT_7             :  dcb.ByteSize = 7;          break;
          case DIOSTREAMUARTDATABIT_8             :  dcb.ByteSize = 8;          break;
                                        default   :  break;
        }
    }

  if((mask&DIOSTREAMUARTMASK_PARITY) == DIOSTREAMUARTMASK_PARITY)
    {
      dcb.fParity = TRUE;

      switch(config->GetParity())
        {
          case DIOSTREAMUARTPARITY_NONE           :  dcb.Parity   = NOPARITY;
                                                     dcb.fParity  = FALSE;
                                                     break;
          case DIOSTREAMUARTPARITY_ODD            :  dcb.Parity   = ODDPARITY;    break;
          case DIOSTREAMUARTPARITY_EVEN           :  dcb.Parity   = EVENPARITY;   break;
          case DIOSTREAMUARTPARITY_MARK           :  dcb.Parity   = MARKPARITY;   break;
          case DIOSTREAMUARTPARITY_SPACE          :  dcb.Parity   = SPACEPARITY;  break;
        }
    }


  if((mask&DIOSTREAMUARTMASK_STOPBITS ) == DIOSTREAMUARTMASK_STOPBITS)
    {
      switch(config->GetStopBits())
        {
          case DIOSTREAMUARTSTOPBITS_ONE          :  dcb.StopBits =  ONESTOPBIT;   break;
          case DIOSTREAMUARTSTOPBITS_ONEANDAHALF  :
          case DIOSTREAMUARTSTOPBITS_TWO          :  dcb.StopBits =  TWOSTOPBITS;  break;
        }
    }

  if((mask&DIOSTREAMUARTMASK_FLOWCONTROL) == DIOSTREAMUARTMASK_FLOWCONTROL)
    {
      switch(config->GetFlowControl())
        {
          case DIOSTREAMUARTFLOWCONTROL_NONE      : dcb.fDtrControl       = DTR_CONTROL_DISABLE;
                                                    dcb.fRtsControl       = RTS_CONTROL_DISABLE;
                                                    break;

          case DIOSTREAMUARTFLOWCONTROL_HARD      : dcb.fDtrControl       = DTR_CONTROL_ENABLE;
                                                    dcb.fRtsControl       = RTS_CONTROL_ENABLE;
                                                    break;

          case DIOSTREAMUARTFLOWCONTROL_SOFT      : dcb.fDtrControl       = DTR_CONTROL_DISABLE;
                                                    dcb.fRtsControl       = RTS_CONTROL_DISABLE;
                                                    break;
        }
    }

  dcb.XonChar  = 1;
  dcb.XoffChar = 0;

  if(!SetCommState(hcom, &dcb)) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOWINDOWSSTREAMUART::ReadDirect(XBYTE* buffer, XDWORD size)
* @brief      Read direct
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOWINDOWSSTREAMUART::ReadDirect(XBYTE* buffer, XDWORD size)
{
  if(GetStatus()==DIOSTREAMSTATUS_DISCONNECTED) return 0;
  if(size == 0)                                        return 0;

  DWORD br    = 0;
  bool status = true;

  if(!hevent)
    {
      hevent = CreateEvent(0, TRUE, FALSE, 0);
      if(!hevent) return 0;

      ovi.hEvent = hevent;

      if(!ReadFile(hcom, buffer, size, &br, &ovi))
        {
          if(GetLastError()!=ERROR_IO_PENDING)
            {
              status = false;
            }
           else return ReadDirect(buffer, size);
        }
    }
   else
    {
      switch(WaitForSingleObject(ovi.hEvent, readtimeout))
        {
          case WAIT_OBJECT_0: if(!GetOverlappedResult(hcom, &ovi, &br, FALSE))
                                     status = false;
                                else status = true;
                              break;

          case WAIT_TIMEOUT : CancelIo(hcom);
                              status=false;
                              break;

                   default  : status=false;
                              break;

         }
     }

  memset(&ovi, 0, sizeof(ovi));

  if(hevent)
    {
      CloseHandle(hevent);
      hevent = NULL;
    }

  if(!status) return 0;

  return (XDWORD)br;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOWINDOWSSTREAMUART::WriteDirect(XBYTE* buffer, XDWORD size)
* @brief      Write direct
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOWINDOWSSTREAMUART::WriteDirect(XBYTE* buffer, XDWORD size)
{
  if(GetStatus()==DIOSTREAMSTATUS_DISCONNECTED) return 0;

  if(size == 0)    return 0;

  DWORD      bw         = 0;
  OVERLAPPED oviwrite   = { 0 };
  bool       status;

  oviwrite.hEvent = CreateEvent(0, TRUE, FALSE, 0);
  if(!oviwrite.hEvent) return bw;

  if(!WriteFile(hcom, buffer, size, &bw, &oviwrite))
    {
      if(GetLastError()!=ERROR_IO_PENDING)
        {
          status = false;
        }
       else
        {
          switch(WaitForSingleObject(oviwrite.hEvent, writetimeout))
            {
              case WAIT_OBJECT_0 :  if(!GetOverlappedResult(hcom, &oviwrite, &bw, TRUE))
                                           status = false;
                                      else status = true;
                                    break;

              case WAIT_TIMEOUT  : CancelIo(hcom);
                                   status=false;
                                   break;

                       default   : status=false;
                                   break;

            }
        }

    } else status = true;

  CloseHandle(oviwrite.hEvent);

  if(!status) return 0;

  return (XDWORD)bw;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMUART::Disconnect()
* @brief      disconnect
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMUART::Disconnect() 
{
  SetEvent(DIOWINDOWSUARTFSMEVENT_DISCONNECTING);

  return true; 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMUART::Close()
* @brief      Close
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMUART::Close()
{
  if(threadconnection) threadconnection->End();
  //// if(GetStatus()==DIOSTREAMSTATUS_DISCONNECTED) return false;

  if(hcom != INVALID_HANDLE_VALUE)
    {
      CloseHandle(hcom);
      hcom = INVALID_HANDLE_VALUE;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMUART::GetCTS()
* @brief      Get CTS
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMUART::GetCTS()
{
  // if(GetStatus()==DIOSTREAMSTATUS_DISCONNECTED) return false;

  DWORD status = 0;
  if(!GetCommModemStatus(hcom, &status))
    {
      Close();
      return false;
    }

  return (status&MS_CTS_ON)!= 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMUART::GetDSR()
* @brief      Get DSR
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMUART::GetDSR()
{
  // if(GetStatus()==DIOSTREAMSTATUS_DISCONNECTED) return false;

  DWORD status = 0;
  if(!GetCommModemStatus(hcom, &status))
    {
      Close();
      return false;
    }

  return (status&MS_DSR_ON)!= 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMUART::GetRing()
* @brief      Get ring
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMUART::GetRing()
{
  // if(GetStatus()==DIOSTREAMSTATUS_DISCONNECTED) return false;

  DWORD status = 0;
  if(!GetCommModemStatus(hcom, &status))
    {
      Close();
      return false;
    }

  return (status&MS_RING_ON)!= 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMUART::GetRLSD()
* @brief      Get RLSD
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMUART::GetRLSD()
{
  // if(GetStatus()==DIOSTREAMSTATUS_DISCONNECTED) return false;

  DWORD status = 0;
  if(!GetCommModemStatus(hcom, &status))
    {
      Close();
      return false;
    }

  return (status&MS_RLSD_ON)!= 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMUART::SetRTS(bool on)
* @brief      Set RTS
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  on : On value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMUART::SetRTS(bool on)
{
  // if(GetStatus()==DIOSTREAMSTATUS_DISCONNECTED) return false;

  if(!EscapeCommFunction(hcom, on?SETRTS:CLRRTS)) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMUART::SetDTR(bool on)
* @brief      Set DTR
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  on : On value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMUART::SetDTR(bool on)
{
  // if(GetStatus()==DIOSTREAMSTATUS_DISCONNECTED) return false;

  if(!EscapeCommFunction(hcom, on?SETDTR:CLRDTR)) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMUART::CleanBuffers()
* @brief      Clean buffers
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMUART::CleanBuffers()
{
  // if(GetStatus()==DIOSTREAMSTATUS_DISCONNECTED) return false;

  if(!PurgeComm(hcom, PURGE_TXCLEAR|PURGE_RXCLEAR)) return false;

  DWORD error;

  if(!ClearCommError(hcom, &error, NULL)) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMUART::SetMask(XDWORD mask)
* @brief      Set mask
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  mask : Mask value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMUART::SetMask(XDWORD mask)
{
  // if(GetStatus()==DIOSTREAMSTATUS_DISCONNECTED) return false;

  if(!SetCommMask(hcom, mask))
    {
      Close();
      return false;
    }

  this->mask = mask;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMUART::SetTimeouts()
* @brief      Set timeouts
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMUART::SetTimeouts()
{
  // if(GetStatus()==DIOSTREAMSTATUS_DISCONNECTED) return false;

  COMMTIMEOUTS to;

  to.ReadIntervalTimeout          = MAXDWORD;
  to.ReadTotalTimeoutMultiplier   = 0;
  to.ReadTotalTimeoutConstant     = 0;
  to.WriteTotalTimeoutMultiplier  = 0;
  to.WriteTotalTimeoutConstant    = 0;

  /*
  to.ReadIntervalTimeout         = 20;
  to.ReadTotalTimeoutMultiplier  = 0;
  to.ReadTotalTimeoutConstant    = 2000;
  to.WriteTotalTimeoutMultiplier = 1;
  to.WriteTotalTimeoutConstant   = 2000;
  */

  if(!SetCommTimeouts(hcom, &to))
    {
      Close();
      return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWINDOWSSTREAMUART::ThreadConnection(void* data)
* @brief      Thread connection
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  data : Data buffer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWINDOWSSTREAMUART::ThreadConnection(void* data)
{
  DIOWINDOWSSTREAMUART* diostream = (DIOWINDOWSSTREAMUART*)data;
  if(!diostream) return;

  if(diostream->GetEvent()==DIOWINDOWSUARTFSMEVENT_NONE) // No hay nuevos Eventos
    {
      switch(diostream->GetCurrentState())
        {
          case DIOWINDOWSUARTFSMSTATE_NONE                  : break;

          case DIOWINDOWSUARTFSMSTATE_CONNECTED             : break;

          case DIOWINDOWSUARTFSMSTATE_WAITINGTOREAD         : { if(!diostream->IsBlockRead())
                                                                  {
                                                                    XBYTE buffer[DIOSTREAM_MAXBUFFER];

                                                                    XDWORD br = diostream->ReadDirect(buffer, DIOSTREAM_MAXBUFFER);
                                                                    if(br)
                                                                      {
                                                                        //XTRACE_PRINTDATABLOCKCOLOR(XTRACE_COLOR_GREEN, (XBYTE*)diostream->buffer, (int)br);
                                                                        diostream->inbuffer->Add(buffer, br);
                                                                      }
                                                                  }

                                                                if(!diostream->IsBlockWrite())
                                                                  {
                                                                    int esize = diostream->outbuffer->GetSize();
                                                                    if(esize)
                                                                      {
                                                                        diostream->outbuffer->SetBlocked(true);
                                                                        XDWORD bw = diostream->WriteDirect(diostream->outbuffer->Get(), esize);
                                                                        diostream->outbuffer->SetBlocked(false);

                                                                        if(bw) diostream->outbuffer->Extract(NULL, 0, bw);

                                                                      }
                                                                  }
                                                              }
                                                              break;

          case DIOWINDOWSUARTFSMSTATE_DISCONNECTING         : break;

        }
    }
   else
    {
      if(diostream->GetEvent()<DIOWINDOWSUART_LASTEVENT)
        {
          diostream->CheckTransition();

          switch(diostream->GetCurrentState())
            {
              case DIOWINDOWSUARTFSMSTATE_NONE              : break;

              case DIOWINDOWSUARTFSMSTATE_CONNECTED         : { DIOSTREAM_XEVENT xevent(diostream, DIOSTREAM_XEVENT_TYPE_CONNECTED);
                                                                xevent.SetDIOStream(diostream);
                                                                diostream->PostEvent(&xevent);

                                                                diostream->SetEvent(DIOWINDOWSUARTFSMEVENT_WAITINGTOREAD);
                                                              }
                                                              break;

              case DIOWINDOWSUARTFSMSTATE_WAITINGTOREAD     : break;

              case DIOWINDOWSUARTFSMSTATE_DISCONNECTING     : { DIOSTREAM_XEVENT xevent(diostream, DIOSTREAM_XEVENT_TYPE_DISCONNECTED);
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
* @fn         void DIOWINDOWSSTREAMUART::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWINDOWSSTREAMUART::Clean()
{
  threadconnection = NULL;

  hcom            = INVALID_HANDLE_VALUE;
  hevent          = NULL;

  memset(&ovi, 0, sizeof(ovi));

  readtimeout  = 3000;
  writetimeout = 3000;
}



#endif


