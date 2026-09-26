/**-------------------------------------------------------------------------------------------------------------------
*
* @file       DIOESP32StreamUART.cpp
*
* @class      DIOESP32STREAMUART
* @brief      Data Input/Output ESP32 Stream UART class
* @ingroup    PLATFORM_ESP32
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

/*---- PRECOMPILATION CONTROL ----------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"

#if defined(DIO_ACTIVE) && defined(DIO_STREAMUART_ACTIVE)

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XFactory.h"
#include "XThreadCollected.h"

#include "DIOStream_XEvent.h"
#include "DIOStreamUARTConfig.h"

#include "DIOESP32StreamUART.h"

#include "XMemory_Control.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

DIOESP32STREAMUART*   diostreamuartptrhandle[DIOESP32STREAMIUART_MAXHANDLES] = { NULL, NULL, NULL, NULL };

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/



/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOESP32STREAMUART::DIOESP32STREAMUART() : DIOSTREAMUART(), XFSMACHINE(0)
* @brief      Constructor of class
* @ingroup    PLATFORM_ESP32
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOESP32STREAMUART::DIOESP32STREAMUART() : DIOSTREAMUART(), XFSMACHINE(0)
{
  Clean();

  AddState( DIOESP32UARTFSMSTATE_NONE             ,
            DIOESP32UARTFSMEVENT_CONNECTED        , DIOESP32UARTFSMSTATE_CONNECTED         ,
            DIOESP32UARTFSMEVENT_DISCONNECTING    , DIOESP32UARTFSMSTATE_DISCONNECTING     ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOESP32UARTFSMSTATE_CONNECTED        ,
            DIOESP32UARTFSMEVENT_WAITINGTOREAD    , DIOESP32UARTFSMSTATE_WAITINGTOREAD     ,
            DIOESP32UARTFSMEVENT_DISCONNECTING    , DIOESP32UARTFSMSTATE_DISCONNECTING     ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOESP32UARTFSMSTATE_WAITINGTOREAD    ,
            DIOESP32UARTFSMEVENT_CONNECTED        , DIOESP32UARTFSMSTATE_CONNECTED         ,
            DIOESP32UARTFSMEVENT_DISCONNECTING    , DIOESP32UARTFSMSTATE_DISCONNECTING     ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOESP32UARTFSMSTATE_DISCONNECTING    ,
            DIOESP32UARTFSMEVENT_CONNECTED        , DIOESP32UARTFSMSTATE_CONNECTED         ,
            DIOESP32UARTFSMEVENT_WAITINGTOREAD    , DIOESP32UARTFSMSTATE_WAITINGTOREAD     ,
            XFSMACHINESTATE_EVENTDEFEND);

  threadconnection = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMUART, __L("DIOESP32STREAMUART::DIOESP32STREAMUART"), ThreadConnection, (void*)this);
}



/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOESP32STREAMUART::~DIOESP32STREAMUART()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_ESP32
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOESP32STREAMUART::~DIOESP32STREAMUART()
{
  Close();

  if(threadconnection) DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMUART, threadconnection);

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMSTATUS DIOESP32STREAMUART::GetStatus()
* @brief      Get status
* @ingroup    PLATFORM_ESP32
* 
* @return     DIOSTREAMSTATUS : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMSTATUS DIOESP32STREAMUART::GetStatus()
{
  if(!config) return DIOSTREAMSTATUS_DISCONNECTED;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOESP32STREAMUART::Open()
* @brief      Open
* @ingroup    PLATFORM_ESP32
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOESP32STREAMUART::Open()
{
  if(!threadconnection)  return false;

  switch(config->GetPort())
    {
      case  1 : huart.Instance = USART1;      break;
			
/*			
      case  2 : huart.Instance = USART2;      break;
      #ifndef HW_ESP32F4XX
      case  3 : huart.Instance = USART3;      break;
      #endif
      default : return false;
*/			
    }

  indexport = (config->GetPort()-1);
  if(indexport < 0) return false;

  if(!Config(DIOSTREAMUARTMASK_ALL)) return false;

  if(HAL_UART_Init(&huart) != HAL_OK) return false;

  diostreamuartptrhandle[indexport] = this;

  __HAL_UART_ENABLE_IT(&huart, UART_IT_RXNE);

   switch(config->GetPort())
    {
      case  1 : HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
                HAL_NVIC_EnableIRQ(USART1_IRQn);
                break;
/*
      case  2 : HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
                HAL_NVIC_EnableIRQ(USART2_IRQn);
                break;

      #ifndef HW_ESP32F4XX
      case  3 :
                #ifdef HW_ESP32F0XX
                HAL_NVIC_SetPriority(USART3_4_IRQn, 0, 0);
                HAL_NVIC_EnableIRQ(USART3_4_IRQn);
                #else
                HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
                HAL_NVIC_EnableIRQ(USART3_IRQn);
                #endif

                break;			
      #endif
*/			

      default : return false;
    }

  HAL_UART_Receive_IT(&huart, (uint8_t*)&receiveddata, 1);

  SetEvent(DIOESP32UARTFSMEVENT_CONNECTED);

  status = DIOSTREAMSTATUS_CONNECTED;

  ResetXBuffers();
  ResetConnectionStatistics();

  return threadconnection->Ini();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOESP32STREAMUART::Config(XWORD mask)
* @brief      Config
* @ingroup    PLATFORM_ESP32
* 
* @param[in]  mask : Mask value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOESP32STREAMUART::Config(XWORD mask)
{
  if((mask&DIOSTREAMUARTMASK_BAUDRATE) == DIOSTREAMUARTMASK_BAUDRATE)
    {
      switch(this->config->GetBaudRate())
        {
          case 50     : huart.Init.BaudRate = 50;     break;
          case 300    : huart.Init.BaudRate = 300;    break;
          case 600    : huart.Init.BaudRate = 600;    break;
          case 1200   : huart.Init.BaudRate = 1200;   break;
          case 4800   : huart.Init.BaudRate = 4800;   break;
          case 9600   : huart.Init.BaudRate = 9600;   break;
          case 19200  : huart.Init.BaudRate = 19200;  break;
          case 38400  : huart.Init.BaudRate = 38400;  break;
          case 57600  : huart.Init.BaudRate = 57600;  break;
          case 115200 : huart.Init.BaudRate = 115200; break;
        }
    }

  if((mask&DIOSTREAMUARTMASK_DATABITS ) == DIOSTREAMUARTMASK_DATABITS)
    {
      switch(this->config->GetDataBits())
        {
          case DIOSTREAMUARTDATABIT_5             : break;
          case DIOSTREAMUARTDATABIT_6             : break;
          case DIOSTREAMUARTDATABIT_7             : break;
          case DIOSTREAMUARTDATABIT_8             : huart.Init.WordLength = UART_WORDLENGTH_8B;     break;
          case DIOSTREAMUARTDATABIT_9             : huart.Init.WordLength = UART_WORDLENGTH_9B;     break;
        }
    }

  if((mask&DIOSTREAMUARTMASK_PARITY) == DIOSTREAMUARTMASK_PARITY)
    {
      switch(this->config->GetParity())
        {
          case DIOSTREAMUARTPARITY_NONE           : huart.Init.Parity = UART_PARITY_NONE;           break;
          case DIOSTREAMUARTPARITY_MARK           :                                                 break;
          case DIOSTREAMUARTPARITY_ODD            : huart.Init.Parity = UART_PARITY_ODD;            break;
          case DIOSTREAMUARTPARITY_SPACE          :                                                 break;
          case DIOSTREAMUARTPARITY_EVEN           : huart.Init.Parity = UART_PARITY_EVEN;           break;
        }
    }

  if((mask&DIOSTREAMUARTMASK_STOPBITS ) == DIOSTREAMUARTMASK_STOPBITS)
    {
      switch(this->config->GetStopBits())
        {
          case DIOSTREAMUARTSTOPBITS_ONE          : huart.Init.StopBits = UART_STOPBITS_1;          break;
          #if defined(HW_ESP32F0XX) || defined(HW_ESP32F3XX) || defined(HW_ESP32WB55XX)
          case DIOSTREAMUARTSTOPBITS_ONEANDAHALF  : huart.Init.StopBits = UART_STOPBITS_1_5;        break;
          #endif
          case DIOSTREAMUARTSTOPBITS_TWO          : huart.Init.StopBits = UART_STOPBITS_2;          break;
        }
    }

  if((mask&DIOSTREAMUARTMASK_FLOWCONTROL) == DIOSTREAMUARTMASK_FLOWCONTROL)
    {
      switch(config->GetFlowControl())
        {
          case DIOSTREAMUARTFLOWCONTROL_NONE      : huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;     break;
          case DIOSTREAMUARTFLOWCONTROL_HARD      : huart.Init.HwFlowCtl = UART_HWCONTROL_RTS_CTS;  break;
          case DIOSTREAMUARTFLOWCONTROL_SOFT      : break;
        }
    }

  huart.Init.Mode                   = UART_MODE_TX_RX;
  huart.Init.OverSampling           = UART_OVERSAMPLING_16;
  #if defined(HW_ESP32F0XX) || defined(HW_ESP32F3XX)
  //huart.Init.OneBitSampling         = UART_ONEBIT_SAMPLING_DISABLED;
  huart.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  #endif

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOESP32STREAMUART::ReadDirect(XBYTE* buffer, XDWORD size)
* @brief      Read direct
* @ingroup    PLATFORM_ESP32
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOESP32STREAMUART::ReadDirect(XBYTE* buffer, XDWORD size)
{
  if(GetStatus()==DIOSTREAMSTATUS_DISCONNECTED) return 0;

  int br = 0;

  if(HAL_UART_Receive(&diostreamuartptrhandle[indexport]->huart, (uint8_t*)buffer, size, 1000) == HAL_OK)
    {
      br = size;
    }

  return (XDWORD)br;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOESP32STREAMUART::WriteDirect(XBYTE* buffer, XDWORD size)
* @brief      Write direct
* @ingroup    PLATFORM_ESP32
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOESP32STREAMUART::WriteDirect(XBYTE* buffer, XDWORD size)
{
  if(GetStatus()==DIOSTREAMSTATUS_DISCONNECTED) return 0;

  if(HAL_UART_Transmit(&huart, (uint8_t*)buffer , (uint16_t)size, 1000) == HAL_OK) return size;

  return (XDWORD)0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOESP32STREAMUART::Write(XBYTE* buffer, XDWORD size)
* @brief      Write
* @ingroup    PLATFORM_ESP32
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOESP32STREAMUART::Write(XBYTE* buffer, XDWORD size)
{
  if(!outbuffer) return 0;

  while(outbuffer->IsBlocked())
    {

    }

  if(!outbuffer->Add(buffer, size))  return 0;

  nbyteswrite+=size;
  if(xtimernotactivity) xtimernotactivity->Reset();

  outbuffer->SetBlocked(false);

  return size;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOESP32STREAMUART::Disconnect()
* @brief      Disconnect
* @ingroup    PLATFORM_ESP32
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOESP32STREAMUART::Disconnect()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOESP32STREAMUART::Close()
* @brief      Close
* @ingroup    PLATFORM_ESP32
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOESP32STREAMUART::Close()
{
  if(!threadconnection) return false;

  threadconnection->End();

  if(GetStatus()==DIOSTREAMSTATUS_DISCONNECTED) return false;

  diostreamuartptrhandle[indexport] = NULL;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOESP32STREAMUART::GetCTS()
* @brief      Get CTS
* @ingroup    PLATFORM_ESP32
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOESP32STREAMUART::GetCTS()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOESP32STREAMUART::GetDSR()
* @brief      Get DSR
* @ingroup    PLATFORM_ESP32
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOESP32STREAMUART::GetDSR()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOESP32STREAMUART::GetRing()
* @brief      Get ring
* @ingroup    PLATFORM_ESP32
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOESP32STREAMUART::GetRing()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOESP32STREAMUART::GetRLSD()
* @brief      Get RLSD
* @ingroup    PLATFORM_ESP32
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOESP32STREAMUART::GetRLSD()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOESP32STREAMUART::SetRTS(bool on)
* @brief      Set RTS
* @ingroup    PLATFORM_ESP32
* 
* @param[in]  on : On value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOESP32STREAMUART::SetRTS(bool on)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOESP32STREAMUART::SetDTR(bool on)
* @brief      Set DTR
* @ingroup    PLATFORM_ESP32
* 
* @param[in]  on : On value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOESP32STREAMUART::SetDTR(bool on)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOESP32STREAMUART::CleanBuffers()
* @brief      Clean buffers
* @note       INTERNAL
* @ingroup    PLATFORM_ESP32
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOESP32STREAMUART::CleanBuffers()
{

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UART_HandleTypeDef* DIOESP32STREAMUART::GetHuart()
* @brief      Get huart
* @ingroup    PLATFORM_ESP32
* 
* @return     UART_HandleTypeDef* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UART_HandleTypeDef* DIOESP32STREAMUART::GetHuart()
{
  return &huart;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOESP32STREAMUART::SetHuart(UART_HandleTypeDef* huart)
* @brief      Set huart
* @ingroup    PLATFORM_ESP32
* 
* @param[in]  huart : Huart pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOESP32STREAMUART::SetHuart(UART_HandleTypeDef* huart)
{
	if(!huart) return;
	
  this->huart = (*huart);
}




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
* @brief      AL_UART_RxCpltCallback
* @ingroup    PLATFORM_ESP32
* 
* @param[in]  huart : Huart pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  int indexport = -1;

  if(huart->Instance == USART1) indexport = 0;
	
  //if(huart->Instance == USART2) indexport = 1;
  //#ifndef HW_ESP32F4XX
  //if(huart->Instance == USART3) indexport = 2;
  //#endif
	

  if(indexport == -1)                                                                       return;
  if(!diostreamuartptrhandle[indexport])                                                    return;
  if(diostreamuartptrhandle[indexport]->GetStatus() == DIOSTREAMSTATUS_DISCONNECTED) return;

  if(!diostreamuartptrhandle[indexport]->IsBlockRead())
    {
      if(!diostreamuartptrhandle[indexport]->inbuffer->IsBlocked())
        {
          if(diostreamuartptrhandle[indexport]->readcache[indexport].GetSize())
            {
              diostreamuartptrhandle[indexport]->inbuffer->Add(diostreamuartptrhandle[indexport]->readcache);
              diostreamuartptrhandle[indexport]->readcache[indexport].Delete();
            }

          diostreamuartptrhandle[indexport]->inbuffer->Add(diostreamuartptrhandle[indexport]->receiveddata);
        }
       else
        {
          diostreamuartptrhandle[indexport]->readcache[indexport].Add(diostreamuartptrhandle[indexport]->receiveddata);
        }
    }

  HAL_UART_Receive_IT(&diostreamuartptrhandle[indexport]->huart, (uint8_t*)&diostreamuartptrhandle[indexport]->receiveddata, 1);
}



/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOESP32STREAMUART::ThreadConnection(void* param)
* @brief      Thread connection
* @ingroup    PLATFORM_ESP32
* 
* @param[in]  param : Param pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOESP32STREAMUART::ThreadConnection(void* param)
{
  DIOESP32STREAMUART* diostream = (DIOESP32STREAMUART*)param;
  if(!diostream) return;

  if(diostream->GetEvent()==DIOESP32UARTFSMEVENT_NONE) // No hay nuevos Eventos
    {
      switch(diostream->GetCurrentState())
        {
          case DIOESP32UARTFSMSTATE_NONE                    : break;

          case DIOESP32UARTFSMSTATE_CONNECTED               : break;

          case DIOESP32UARTFSMSTATE_WAITINGTOREAD           : { if(!diostream->IsBlockRead())
                                                                      {
                                                                        if(!diostream->inbuffer->IsBlocked())
                                                                          {
                                                                            if(diostream->readcache[diostream->indexport].GetSize())
                                                                              {
                                                                                diostream->inbuffer->Add(diostream->readcache);
                                                                                diostream->readcache[diostream->indexport].Delete();
                                                                              }
                                                                          }
                                                                      }

                                                                    if(!diostream->IsBlockWrite())
                                                                      {
                                                                        if(!diostream->outbuffer->IsBlocked())
                                                                         {
                                                                            int esize = diostream->outbuffer->GetSize();
                                                                            if(esize>DIOSTREAM_MAXBUFFER) esize = DIOSTREAM_MAXBUFFER;

                                                                            if(esize)
                                                                              {
                                                                                diostream->outbuffer->Get(diostream->writebuffer, esize, 0);
                                                                                if(diostream->WriteDirect(diostream->writebuffer, esize))
                                                                                  {
                                                                                    diostream->outbuffer->Extract(NULL, 0, esize);
                                                                                  }
                                                                              }
                                                                          }
                                                                      }
                                                                  }
                                                                  break;

          case DIOESP32UARTFSMSTATE_DISCONNECTING           : break;

        }
    }
   else
    {
      if(diostream->GetEvent()<DIOESP32UART_LASTEVENT)
        {
          diostream->CheckTransition();

          switch(diostream->GetCurrentState())
            {
              case DIOESP32UARTFSMSTATE_NONE                : break;

              case DIOESP32UARTFSMSTATE_CONNECTED           : { DIOSTREAM_XEVENT xevent(diostream, DIOSTREAM_XEVENT_TYPE_CONNECTED);
                                                                    xevent.SetDIOStream(diostream);
                                                                    diostream->PostEvent(&xevent);

                                                                    diostream->SetEvent(DIOESP32UARTFSMEVENT_WAITINGTOREAD);
                                                                  }
                                                                  break;

              case DIOESP32UARTFSMSTATE_WAITINGTOREAD       : break;

              case DIOESP32UARTFSMSTATE_DISCONNECTING       : { DIOSTREAM_XEVENT xevent(diostream, DIOSTREAM_XEVENT_TYPE_DISCONNECTED);
                                                                    xevent.SetDIOStream(diostream);
                                                                    diostream->PostEvent(&xevent);

                                                                    diostream->status = DIOSTREAMSTATUS_DISCONNECTED;
                                                                  }
                                                                  break;
            }
        }
    }

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOESP32STREAMUART::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_ESP32
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOESP32STREAMUART::Clean()
{
  threadconnection   = NULL;
  indexport         = -1;
}




#endif
