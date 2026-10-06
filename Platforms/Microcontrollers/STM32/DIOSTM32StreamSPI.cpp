/**-------------------------------------------------------------------------------------------------------------------
*
* @file       DIOSTM32StreamSPI.cpp
*
* @class      DIOSTM32STREAMSPI
* @brief      Data Input/Output STM32 Stream SPI class
* @ingroup    PLATFORM_STM32
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

#if defined(DIO_ACTIVE) && defined(DIO_STREAMSPI_ACTIVE)

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XFactory.h"
#include "XThreadCollected.h"

#include "DIOStream_XEvent.h"
#include "DIOStreamSPIConfig.h"

#include "DIOSTM32StreamSPI.h"

#include "XMemory_Control.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

DIOSTM32STREAMSPIPORT*    DIOSTM32STREAMSPI::ports[DIOSTM32STREAMSPI_MAXPORTS]     = { NULL, NULL };
DIOSTM32STREAMSPI*        DIOSTM32STREAMSPI::handles[DIOSTM32STREAMSPI_MAXHANDLES] = { NULL, NULL };

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------------------------*/
/*  DIOSTM32STREAMSPIPORT                                                                                             */
/*--------------------------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTM32STREAMSPIPORT::DIOSTM32STREAMSPIPORT()
* @brief      Constructor of class
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTM32STREAMSPIPORT::DIOSTM32STREAMSPIPORT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTM32STREAMSPIPORT::~DIOSTM32STREAMSPIPORT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTM32STREAMSPIPORT::~DIOSTM32STREAMSPIPORT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOSTM32STREAMSPIPORT::GetCounterRef()
* @brief      Get counter ref
* @ingroup    PLATFORM_STM32
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTM32STREAMSPIPORT::GetCounterRef()
{
  return counterref;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTM32STREAMSPIPORT::SetCounterRef(XDWORD counterref)
* @brief      Set counter ref
* @ingroup    PLATFORM_STM32
* 
* @param[in]  counterref : Counterref value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTM32STREAMSPIPORT::SetCounterRef(XDWORD counterref)
{
  this->counterref = counterref;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SPI_HandleTypeDef* DIOSTM32STREAMSPIPORT::GetHandleSPI()
* @brief      Get handle SPI
* @ingroup    PLATFORM_STM32
* 
* @return     SPI_HandleTypeDef* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
SPI_HandleTypeDef* DIOSTM32STREAMSPIPORT::GetHandleSPI()
{
  return hspi;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTM32STREAMSPIPORT::SetHandleSPI(SPI_HandleTypeDef* hspi)
* @brief      Set handle SPI
* @ingroup    PLATFORM_STM32
* 
* @param[in]  hspi : Hspi pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTM32STREAMSPIPORT::SetHandleSPI(SPI_HandleTypeDef* hspi)
{
  this->hspi = hspi;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTM32STREAMSPIPORT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTM32STREAMSPIPORT::Clean()
{
  counterref        = 0;
  hspi              = NULL;
}




/*--------------------------------------------------------------------------------------------------------------------*/
/*  DIOSTM32STREAMSPI                                                                                                 */
/*--------------------------------------------------------------------------------------------------------------------*/



/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTM32STREAMSPI::DIOSTM32STREAMSPI() : DIOSTREAMSPI()
* @brief      Constructor of class
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTM32STREAMSPI::DIOSTM32STREAMSPI() : DIOSTREAMSPI()
{
  Clean();

  AddState( DIOSTREAMSPI_FSMSTATE_NONE            ,
            DIOSTREAMSPI_FSMEVENT_CONNECTED       , DIOSTREAMSPI_FSMSTATE_CONNECTED        ,
            DIOSTREAMSPI_FSMEVENT_DISCONNECTING   , DIOSTREAMSPI_FSMSTATE_DISCONNECTING    ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOSTREAMSPI_FSMSTATE_CONNECTED       ,
            DIOSTREAMSPI_FSMEVENT_WAITINGTOREAD   , DIOSTREAMSPI_FSMSTATE_WAITINGTOREAD    ,
            DIOSTREAMSPI_FSMEVENT_DISCONNECTING   , DIOSTREAMSPI_FSMSTATE_DISCONNECTING    ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOSTREAMSPI_FSMSTATE_WAITINGTOREAD   ,
            DIOSTREAMSPI_FSMEVENT_CONNECTED       , DIOSTREAMSPI_FSMSTATE_CONNECTED        ,
            DIOSTREAMSPI_FSMEVENT_DISCONNECTING   , DIOSTREAMSPI_FSMSTATE_DISCONNECTING    ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOSTREAMSPI_FSMSTATE_DISCONNECTING   ,
            DIOSTREAMSPI_FSMEVENT_CONNECTED       , DIOSTREAMSPI_FSMSTATE_CONNECTED        ,
            DIOSTREAMSPI_FSMEVENT_WAITINGTOREAD   , DIOSTREAMSPI_FSMSTATE_WAITINGTOREAD    ,
            XFSMACHINESTATE_EVENTDEFEND);

  threadconnection = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMSPI, _L("DIOSTM32STREAMSPI::DIOSTM32STREAMSPI"), ThreadConnection, (void*)this);
}



/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTM32STREAMSPI::~DIOSTM32STREAMSPI()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTM32STREAMSPI::~DIOSTM32STREAMSPI()
{
  Close();

  if(threadconnection) DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMSPI, threadconnection);

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMSTATUS DIOSTM32STREAMSPI::GetStatus()
* @brief      Get status
* @ingroup    PLATFORM_STM32
* 
* @return     DIOSTREAMSTATUS : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMSTATUS DIOSTM32STREAMSPI::GetStatus()
{
  if(!config) return DIOSTREAMSTATUS_DISCONNECTED;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTM32STREAMSPI::Open()
* @brief      Open
* @ingroup    PLATFORM_STM32
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTM32STREAMSPI::Open()
{
  if(!threadconnection)  return false;
  if(!config)           return false;

 for(int c=0; c<DIOSTM32STREAMSPI_MAXHANDLES; c++)
    {
      if(DIOSTM32STREAMSPI::handles[c] == this) return false;

      if(!DIOSTM32STREAMSPI::handles[c])
        {
          indexhandle = c;
          DIOSTM32STREAMSPI::handles[indexhandle] = this;
          break;
        }
    }

  if(indexhandle == -1) return false;

  indexport = config->GetPort()-1;

  if(!DIOSTM32STREAMSPI::ports[indexport])
    {
      DIOSTM32STREAMSPIPORT* port = GEN_NEW DIOSTM32STREAMSPIPORT();
      if(!port) return false;

      hspi = GEN_NEW SPI_HandleTypeDef();
      if(!hspi)
        {
          GEN_DELETE port;
          return false;
        }

      switch(config->GetPort())
        {
          case  1 : hspi->Instance = SPI1;       break;
          case  2 : hspi->Instance = SPI2;       break;
          default : return false;
        }

     

      hspi->Init.Mode               = SPI_MODE_MASTER;
      hspi->Init.Direction          = SPI_DIRECTION_2LINES;
      hspi->Init.DataSize           = SPI_DATASIZE_8BIT;
      hspi->Init.CLKPolarity        = SPI_POLARITY_LOW;
      hspi->Init.CLKPhase           = SPI_PHASE_1EDGE;
      hspi->Init.NSS                = SPI_NSS_HARD_OUTPUT;
      hspi->Init.BaudRatePrescaler  = SPI_BAUDRATEPRESCALER_2;
      hspi->Init.FirstBit           = SPI_FIRSTBIT_MSB;
      hspi->Init.TIMode             = SPI_TIMODE_DISABLE;
      hspi->Init.CRCCalculation     = SPI_CRCCALCULATION_DISABLE;
      hspi->Init.CRCPolynomial      = 7;
      hspi->Init.CRCLength          = SPI_CRC_LENGTH_DATASIZE;
      hspi->Init.NSSPMode           = SPI_NSS_PULSE_ENABLE;

      if(HAL_SPI_Init(hspi) != HAL_OK)
        {
          return false;
        }

      port->SetHandleSPI(hspi);
      DIOSTM32STREAMSPI::ports[indexport] = port;
    }
   else
    {
      hspi = DIOSTM32STREAMSPI::ports[indexport]->GetHandleSPI();
      DIOSTM32STREAMSPI::ports[indexport]->SetCounterRef(DIOSTM32STREAMSPI::ports[indexport]->GetCounterRef()+1);
    }

  SetEvent(DIOSTREAMSPI_FSMEVENT_CONNECTED);

  status = DIOSTREAMSTATUS_CONNECTED;

  memset(bufferdata, 0, DIOSTREAM_MAXBUFFER);

  ResetXBuffers();
  ResetConnectionStatistics();

  return threadconnection->Ini();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTM32STREAMSPI::WaitToFilledReadingBuffer(int filledto, int timeout)
* @brief      Wait to filled reading buffer
* @ingroup    PLATFORM_STM32
* 
* @param[in]  filledto : Filledto value.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTM32STREAMSPI::WaitToFilledReadingBuffer(int filledto, int timeout)
{
  if(filledto == DIOSTREAM_SOMETHINGTOREAD) sizebufferdata = 1; else sizebufferdata = filledto;

  switch(config->GetMode())
    {
      case DIOSTREAMMODE_MASTER     :
      case DIOSTREAMMODE_SEMIMASTER :
      case DIOSTREAMMODE_SLAVE      : HAL_SPI_Transmit_IT(hspi, (uint8_t*)bufferdata, sizebufferdata);
                                      break;
                           default  : break;
    }

  return DIOSTREAM::WaitToFilledReadingBuffer(sizebufferdata, timeout);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOSTM32STREAMSPI::ReadDirect(XBYTE* buffer, XDWORD size)
* @brief      Read direct
* @ingroup    PLATFORM_STM32
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTM32STREAMSPI::ReadDirect(XBYTE* buffer, XDWORD size)
{
  if(!config)                                          return 0;
  if(GetStatus()==DIOSTREAMSTATUS_DISCONNECTED) return 0;

  XDWORD br = 0;

  switch(config->GetMode())
    {
      case DIOSTREAMMODE_MASTER     :
      case DIOSTREAMMODE_SEMIMASTER :
      case DIOSTREAMMODE_SLAVE      : if(HAL_SPI_Receive(hspi, buffer, size, 100) == HAL_OK) br = size;
                                      break;
                           default  : break;
    }

  return br;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOSTM32STREAMSPI::WriteDirect(XBYTE* buffer, XDWORD size)
* @brief      Write direct
* @ingroup    PLATFORM_STM32
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTM32STREAMSPI::WriteDirect(XBYTE* buffer, XDWORD size)
{
  if(!config)                                          return 0;
  if(GetStatus()==DIOSTREAMSTATUS_DISCONNECTED) return 0;

  XDWORD bw = 0;

  if(!size) return 0;

   switch(config->GetMode())
    {
      case DIOSTREAMMODE_MASTER       :
      case DIOSTREAMMODE_SEMIMASTER   :
      case DIOSTREAMMODE_SLAVE        : if(HAL_SPI_Transmit(hspi, buffer, size, 100) == HAL_OK) bw = size;
                                        break;
                             default  : break;
    }

  return bw;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOSTM32STREAMSPI::Write(XBYTE* buffer, XDWORD size)
* @brief      Write
* @ingroup    PLATFORM_STM32
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTM32STREAMSPI::Write(XBYTE* buffer, XDWORD size)
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
* @fn         bool DIOSTM32STREAMSPI::Disconnect()
* @brief      Disconnect
* @ingroup    PLATFORM_STM32
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTM32STREAMSPI::Disconnect()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTM32STREAMSPI::Close()
* @brief      Close
* @ingroup    PLATFORM_STM32
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTM32STREAMSPI::Close()
{
  if(!threadconnection) return false;

  threadconnection->End();

  if(GetStatus()==DIOSTREAMSTATUS_DISCONNECTED) return false;

  if(!DIOSTM32STREAMSPI::handles[indexhandle]) return false;

  DIOSTM32STREAMSPI::handles[indexhandle] = NULL;

  if(!DIOSTM32STREAMSPI::ports[indexport]->GetCounterRef())
    {
      HAL_SPI_DeInit(DIOSTM32STREAMSPI::ports[indexport]->GetHandleSPI());

      GEN_DELETE DIOSTM32STREAMSPI::ports[indexport]->GetHandleSPI();
      GEN_DELETE DIOSTM32STREAMSPI::ports[indexport];

      DIOSTM32STREAMSPI::ports[indexport] = NULL;
    }
   else
   {
     DIOSTM32STREAMSPI::ports[indexport]->SetCounterRef(DIOSTM32STREAMSPI::ports[indexport]->GetCounterRef()-1);
   }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTM32STREAMSPI::CleanBuffers()
* @brief      Clean buffers
* @note       INTERNAL
* @ingroup    PLATFORM_STM32
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTM32STREAMSPI::CleanBuffers()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void HAL_SPI_MasterRxCpltCallback(SPI_HandleTypeDef* hspi)
* @brief      AL_SPI_MasterRxCpltCallback
* @ingroup    PLATFORM_STM32
* 
* @param[in]  hspi : Hspi pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void HAL_SPI_MasterRxCpltCallback(SPI_HandleTypeDef* hspi)
{
  int port = -1;

  if(hspi->Instance == SPI1) port = 0;
  if(hspi->Instance == SPI2) port = 1;

  for(int c=0; c<DIOSTM32STREAMSPI_MAXHANDLES; c++)
    {
      if(DIOSTM32STREAMSPI::handles[c])
        {
          if(DIOSTM32STREAMSPI::handles[c]->indexport  == port)
            {
              if(DIOSTM32STREAMSPI::handles[c]->GetStatus() == DIOSTREAMSTATUS_DISCONNECTED) return;

              if(!DIOSTM32STREAMSPI::handles[c]->IsBlockRead())
                {
                   DIOSTM32STREAMSPI::handles[c]->inbuffer->Add(DIOSTM32STREAMSPI::handles[c]->bufferdata, DIOSTM32STREAMSPI::handles[c]->sizebufferdata);
                }

              DIOSTM32STREAMSPI::handles[c]->sizebufferdata = 0;
            }
        }
    }
}



/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void HAL_SPI_SlaveRxCpltCallback(SPI_HandleTypeDef* hspi)
* @brief      AL_SPI_SlaveRxCpltCallback
* @ingroup    PLATFORM_STM32
* 
* @param[in]  hspi : Hspi pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void HAL_SPI_SlaveRxCpltCallback(SPI_HandleTypeDef* hspi)
{
  HAL_SPI_MasterRxCpltCallback(hspi);
}



/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
* @brief      AL_SPI_ErrorCallback
* @ingroup    PLATFORM_STM32
* 
* @param[in]  hspi : Hspi pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
  XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L("SPI comunication error [%d]"), hspi->ErrorCode);
}



/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTM32STREAMSPI::ThreadConnection(void* param)
* @brief      Thread connection
* @ingroup    PLATFORM_STM32
* 
* @param[in]  param : Param pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTM32STREAMSPI::ThreadConnection(void* param)
{
  DIOSTM32STREAMSPI* diostream = (DIOSTM32STREAMSPI*)param;
  if(!diostream) return;

  //HAL_NVIC_DisableIRQ(TIM3_IRQn);

  if(diostream->GetEvent()==DIOSTREAMSPI_FSMEVENT_NONE) // No hay nuevos Eventos
    {
      switch(diostream->GetCurrentState())
        {
          case DIOSTREAMSPI_FSMSTATE_NONE                   : break;

          case DIOSTREAMSPI_FSMSTATE_CONNECTED              : break;

          case DIOSTREAMSPI_FSMSTATE_WAITINGTOREAD          : if(!diostream->IsBlockRead())
                                                                {
                                                                  if(!diostream->inbuffer->IsBlocked())
                                                                    {
                                                                      if(diostream->readcache[diostream->indexport].GetSize())
                                                                        {
                                                                          diostream->inbuffer->Add(diostream->readcache[diostream->indexport]);
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
                                                                          diostream->outbuffer->Extract(diostream->writebuffer, 0, esize);
                                                                          diostream->WriteDirect(diostream->writebuffer, esize);
                                                                        }
                                                                    }
                                                                }
                                                              break;

          case DIOSTREAMSPI_FSMSTATE_DISCONNECTING          : break;

        }
    }
   else
    {
      if(diostream->GetEvent()<DIOSTREAMSPI_LASTEVENT)
        {
          diostream->CheckTransition();

          switch(diostream->GetCurrentState())
            {
              case DIOSTREAMSPI_FSMSTATE_NONE               : break;

              case DIOSTREAMSPI_FSMSTATE_CONNECTED          : { DIOSTREAM_XEVENT xevent(diostream, DIOSTREAM_XEVENT_TYPE_CONNECTED);
                                                                xevent.SetDIOStream(diostream);
                                                                diostream->PostEvent(&xevent);

                                                                diostream->SetEvent(DIOSTREAMSPI_FSMEVENT_WAITINGTOREAD);
                                                              }
                                                              break;

              case DIOSTREAMSPI_FSMSTATE_WAITINGTOREAD      : break;

              case DIOSTREAMSPI_FSMSTATE_DISCONNECTING      : { DIOSTREAM_XEVENT xevent(diostream, DIOSTREAM_XEVENT_TYPE_DISCONNECTED);
                                                                xevent.SetDIOStream(diostream);
                                                                diostream->PostEvent(&xevent);

                                                                diostream->status = DIOSTREAMSTATUS_DISCONNECTED;
                                                              }
                                                              break;
            }
        }
    }

  //HAL_NVIC_EnableIRQ(TIM3_IRQn);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTM32STREAMSPI::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTM32STREAMSPI::Clean()
{
  hspi              = NULL;
  localaddress      = 0;
  remoteaddress     = 0;
  threadconnection   = NULL;
  indexhandle       = -1;
  indexport         = -1;
  sizebufferdata    = 0;
}





#endif
