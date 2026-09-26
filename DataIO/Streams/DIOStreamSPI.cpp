/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOStreamSPI.cpp
* 
* @class      DIOSTREAMSPI
* @brief      Data Input/Output Stream SPI class
* @ingroup    DATAIO
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

#include "DIOStreamSPI.h"

#include "XFactory.h"
#include "XBuffer.h"
#include "XThreadCollected.h"
#include "XTrace.h"

#include "DIOFactory.h"
#include "DIOGPIO.h"
#include "DIOStream_XEvent.h"

#include "DIOStreamSPIConfig.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMSPI::DIOSTREAMSPI() : DIOSTREAM(), XFSMACHINE(0)
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMSPI::DIOSTREAMSPI() : DIOSTREAM(), XFSMACHINE(0)
{
  Clean();

  AddState( DIOSTREAMSPI_FSMSTATE_NONE              ,
            DIOSTREAMSPI_FSMEVENT_CONNECTED       , DIOSTREAMSPI_FSMSTATE_CONNECTED        ,
            DIOSTREAMSPI_FSMEVENT_DISCONNECTING   , DIOSTREAMSPI_FSMSTATE_DISCONNECTING    ,
            XFSMACHINESTATE_EVENTDEFEND);


  AddState( DIOSTREAMSPI_FSMSTATE_CONNECTED       ,
            DIOSTREAMSPI_FSMEVENT_WAITINGTOREAD   , DIOSTREAMSPI_FSMSTATE_WAITINGTOREAD    ,
            DIOSTREAMSPI_FSMEVENT_SENDINGDATA     , DIOSTREAMSPI_FSMSTATE_SENDINGDATA      ,
            DIOSTREAMSPI_FSMEVENT_DISCONNECTING   , DIOSTREAMSPI_FSMSTATE_DISCONNECTING    ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOSTREAMSPI_FSMSTATE_WAITINGTOREAD   ,
            DIOSTREAMSPI_FSMEVENT_CONNECTED       , DIOSTREAMSPI_FSMSTATE_CONNECTED        ,
            DIOSTREAMSPI_FSMEVENT_SENDINGDATA     , DIOSTREAMSPI_FSMSTATE_SENDINGDATA      ,
            DIOSTREAMSPI_FSMEVENT_DISCONNECTING   , DIOSTREAMSPI_FSMSTATE_DISCONNECTING    ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOSTREAMSPI_FSMSTATE_DISCONNECTING   ,
            DIOSTREAMSPI_FSMEVENT_CONNECTED       , DIOSTREAMSPI_FSMSTATE_CONNECTED        ,
            DIOSTREAMSPI_FSMEVENT_WAITINGTOREAD   , DIOSTREAMSPI_FSMSTATE_WAITINGTOREAD    ,
            DIOSTREAMSPI_FSMEVENT_SENDINGDATA     , DIOSTREAMSPI_FSMSTATE_SENDINGDATA      ,
            XFSMACHINESTATE_EVENTDEFEND);


  threadconnection = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMSPI, __L("DIOSTREAMSPI::DIOSTREAMSPI"), ThreadConnection, (void*)this);
  threadconnection->SetPriority(XTHREADPRIORITY_HIGH);

  xtimer=GEN_XFACTORY.CreateTimer();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMSPI::~DIOSTREAMSPI()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMSPI::~DIOSTREAMSPI()
{
  Close();

  if(threadconnection)  DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMSPI, threadconnection);
  if(xtimer)            GEN_XFACTORY.DeleteTimer(xtimer);

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMCONFIG* DIOSTREAMSPI::GetConfig()
* @brief      Get config
* @ingroup    DATAIO
* 
* @return     DIOSTREAMCONFIG* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMCONFIG* DIOSTREAMSPI::GetConfig()
{
  return (DIOSTREAMCONFIG*)config;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMSPI::SetConfig(DIOSTREAMCONFIG* config)
* @brief      Set config
* @ingroup    DATAIO
* 
* @param[in]  config : Configuration object to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMSPI::SetConfig(DIOSTREAMCONFIG* config)
{
  if(!config) return false;
  this->config = (DIOSTREAMSPICONFIG*)config;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMSPI::Open()
* @brief      Open
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMSPI::Open()
{
  if(!config) return false;
  if(!xtimer) return false;

  if(!threadconnection)  return false;

  if(!inbuffer)         return false;
  if(!outbuffer)        return false;

  if(config->IsDirectAccess())
    {      
      GPIO_Entry[DIOSTREAMSPI_GPIO_CLK]     = GEN_DIOGPIO.GPIOEntry_GetByID(config->GetPinsDirectAccess()[DIOSTREAMSPI_GPIO_CLK]); 
      GPIO_Entry[DIOSTREAMSPI_GPIO_MISO]    = GEN_DIOGPIO.GPIOEntry_GetByID(config->GetPinsDirectAccess()[DIOSTREAMSPI_GPIO_MISO]);
      GPIO_Entry[DIOSTREAMSPI_GPIO_MOSI]    = GEN_DIOGPIO.GPIOEntry_GetByID(config->GetPinsDirectAccess()[DIOSTREAMSPI_GPIO_MOSI]);
      GPIO_Entry[DIOSTREAMSPI_GPIO_CS]      = GEN_DIOGPIO.GPIOEntry_GetByID(config->GetPinsDirectAccess()[DIOSTREAMSPI_GPIO_CS]);


      if(GPIO_Entry[DIOSTREAMSPI_GPIO_CLK])   GEN_DIOGPIO.SetMode(GPIO_Entry[DIOSTREAMSPI_GPIO_CLK], DIOGPIO_MODE_INPUT);
      if(GPIO_Entry[DIOSTREAMSPI_GPIO_MISO])  GEN_DIOGPIO.SetMode(GPIO_Entry[DIOSTREAMSPI_GPIO_MISO], DIOGPIO_MODE_OUTPUT);
      if(GPIO_Entry[DIOSTREAMSPI_GPIO_MOSI])  GEN_DIOGPIO.SetMode(GPIO_Entry[DIOSTREAMSPI_GPIO_MOSI], DIOGPIO_MODE_INPUT);
      if(GPIO_Entry[DIOSTREAMSPI_GPIO_CS])    GEN_DIOGPIO.SetMode(GPIO_Entry[DIOSTREAMSPI_GPIO_CS], DIOGPIO_MODE_INPUT);     
    }

  bufferread   = GEN_NEW XBYTE[DIOSTREAMSPI_MAXSIZEBUFFER];

  mode              = config->GetMode();
  nbitsword         = config->GetNBitsWord();
  timedelay         = config->GetDelay();
  timedelaydouble  *= timedelay;
  CSline            = ((mode & DIOSTREAMSPI_CS_HIGH)   == DIOSTREAMSPI_CS_HIGH);
  lbsfirst          = ((mode & DIOSTREAMSPI_LSB_FIRST) == DIOSTREAMSPI_LSB_FIRST);

  if(config->IsDirectAccess())
    {
      switch(mode)
        {
          case DIOSTREAMSPI_MODE_0:
          case DIOSTREAMSPI_MODE_1: GEN_DIOGPIO.SetValue(GPIO_Entry[DIOSTREAMSPI_GPIO_CLK], false);
                                    break;

          case DIOSTREAMSPI_MODE_2:
          case DIOSTREAMSPI_MODE_3: GEN_DIOGPIO.SetValue(GPIO_Entry[DIOSTREAMSPI_GPIO_CLK], true);
                                    break;
        }

      GEN_DIOGPIO.SetValue(GPIO_Entry[DIOSTREAMSPI_GPIO_CS], CSline);
    }

  SetEvent(DIOSTREAMSPI_FSMEVENT_CONNECTED);

  status = DIOSTREAMSTATUS_CONNECTED;

  ResetXBuffers();

  ResetConnectionStatistics();

  return threadconnection->Ini();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMSPI::Close()
* @brief      Close
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMSPI::Close()
{
  if(config->IsDirectAccess()) GEN_DIOGPIO.SetValue(GPIO_Entry[DIOSTREAMSPI_GPIO_CS], !CSline); 

  if(!threadconnection) return false;

  threadconnection->End();

  if(bufferread)
    {
      GEN_DELETE_ARRAY bufferread;
      bufferread = NULL;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMSPI::TransferBuffer(XBYTE* bufferread, XBYTE* bufferwrite, XDWORD size)
* @brief      Transfer buffer
* @ingroup    DATAIO
* 
* @param[in]  bufferread : Bufferread pointer to use.
* @param[in]  bufferwrite : Bufferwrite pointer to use.
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMSPI::TransferBuffer(XBYTE* bufferread, XBYTE* bufferwrite, XDWORD size)
{
  if(!config)     return false;
  if(!xtimer)     return false;

  if(!GPIO_Entry[DIOSTREAMSPI_GPIO_CLK])   return false;
  if(!GPIO_Entry[DIOSTREAMSPI_GPIO_MOSI])  return false;
  if(!GPIO_Entry[DIOSTREAMSPI_GPIO_CS])    return false;

  if(config->GetNBitsWord()<=8)
    {
      for(XDWORD c=0;c<size;c++)
        {
          XWORD readdata;

          TransferOneData(bufferwrite[c], readdata);

          bufferread[c] = (XBYTE)readdata;
        }
    }
   else
    {
      for(XDWORD c=0; c<size; c+=2)
        {
          XWORD readdata;
          //XWORD writedata = bufferwrite[c];

          //writedata<<=8;
          //writedata |= bufferwrite[c];

          TransferOneData(bufferwrite[c], readdata);

          bufferread[c]   = (XBYTE)(readdata>>8);
          bufferread[c+1] = (XBYTE)(readdata&0xFF);
        }

    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMSPI::Sleep(int count)
* @brief      Sleep
* @ingroup    DATAIO
* 
* @param[in]  count : Count value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMSPI::Sleep(int count)
{
  /*
  int i = 0;
  for(i=0;i<count;i++)
    {
      #ifdef LINUX
      asm("nop");
      #endif

      #ifdef _MSC_VER
      __nop();
//      __asm nop;
      #endif

    }
  */
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMSPI::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMSPI::Clean()
{
  config            = NULL;
  xtimer            = NULL;

  bufferread        = NULL;

  threadconnection  = NULL;

  for(int c=0; c<DIOSTREAMSPI_GPIO_MAX; c++)
    {
      GPIO_Entry[c] = NULL;
    }

  mode              = DIOSTREAMSPI_MODE_0;
  nbitsword         = 0;
  timedelay         = 0;
  timedelaydouble   = 0;
  CSline            = false;
  lbsfirst          = false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMSPI::TransferOneData(XWORD writedata, XWORD& readdata)
* @brief      Transfer one data
* @ingroup    DATAIO
* 
* @param[in]  writedata : Writedata value.
* @param[in]  readdata : Readdata value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMSPI::TransferOneData(XWORD writedata, XWORD& readdata)
{
  if(!config->IsDirectAccess()) return false;

  XWORD mask = 0x0001;

  if(!lbsfirst) mask<<=(nbitsword-1);

  readdata = 0;

  for(int c=0;c<nbitsword;c++)
    {
      switch(mode)
        {
          case DIOSTREAMSPI_MODE_0: GEN_DIOGPIO.SetValue(GPIO_Entry[DIOSTREAMSPI_GPIO_MOSI] , (writedata&mask)?true:false);
                                    if(GEN_DIOGPIO.GetValue(GPIO_Entry[DIOSTREAMSPI_GPIO_MISO])) readdata |=  mask;
                                    DIOSTREAMSPI_SLEEP;
                                    GEN_DIOGPIO.SetValue(GPIO_Entry[DIOSTREAMSPI_GPIO_CLK], true);
                                    DIOSTREAMSPI_SLEEPDOUBLE;
                                    GEN_DIOGPIO.SetValue(GPIO_Entry[DIOSTREAMSPI_GPIO_CLK], false);
                                    break;

          case DIOSTREAMSPI_MODE_1:
          case DIOSTREAMSPI_MODE_2:
                                    GEN_DIOGPIO.SetValue(GPIO_Entry[DIOSTREAMSPI_GPIO_CLK], true);
                                    GEN_DIOGPIO.SetValue(GPIO_Entry[DIOSTREAMSPI_GPIO_MOSI], (writedata&mask)?true:false);
                                    if(GEN_DIOGPIO.GetValue(GPIO_Entry[DIOSTREAMSPI_GPIO_MISO])) readdata |=  mask;
                                    DIOSTREAMSPI_SLEEP;
                                    GEN_DIOGPIO.SetValue(GPIO_Entry[DIOSTREAMSPI_GPIO_CLK], false);
                                    DIOSTREAMSPI_SLEEPDOUBLE;
                                    break;

          case DIOSTREAMSPI_MODE_3:
                                    GEN_DIOGPIO.SetValue(GPIO_Entry[DIOSTREAMSPI_GPIO_CLK], false);
                                    GEN_DIOGPIO.SetValue(GPIO_Entry[DIOSTREAMSPI_GPIO_MOSI], (writedata&mask)?true:false);
                                    if(GEN_DIOGPIO.GetValue(GPIO_Entry[DIOSTREAMSPI_GPIO_MISO])) readdata |=  mask;
                                    DIOSTREAMSPI_SLEEP;
                                    GEN_DIOGPIO.SetValue(GPIO_Entry[DIOSTREAMSPI_GPIO_CLK], true);
                                    DIOSTREAMSPI_SLEEPDOUBLE;
                                    break;
        }

      if(lbsfirst) mask<<=1; else mask>>=1;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMSPI::ThreadConnection(void* data)
* @brief      Thread connection
* @ingroup    DATAIO
* 
* @param[in]  data : Data buffer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMSPI::ThreadConnection(void* data)
{
  DIOSTREAMSPI* diostream = (DIOSTREAMSPI*)data;
  if(!diostream) return;

  if(diostream->GetEvent()==DIOSTREAMSPI_FSMEVENT_NONE) // No hay nuevos Eventos
    {
      switch(diostream->GetCurrentState())
        {
          case DIOSTREAMSPI_FSMSTATE_NONE                   : break;

          case DIOSTREAMSPI_FSMSTATE_CONNECTED                : break;

          case DIOSTREAMSPI_FSMSTATE_WAITINGTOREAD            : { if(diostream->outbuffer->IsBlocked()) break;

                                                                  while(1)
                                                                    {
                                                                      XDWORD size  = diostream->outbuffer->GetSize();

                                                                      if(!size) break;
                                                                      if(size>(XDWORD)DIOSTREAMSPI_MAXSIZEBUFFER) size = DIOSTREAMSPI_MAXSIZEBUFFER;

                                                                      memset(diostream->bufferread, 0 , DIOSTREAMSPI_MAXSIZEBUFFER);

                                                                      if(diostream->TransferBuffer(diostream->bufferread, diostream->outbuffer->Get(), size))
                                                                        {
                                                                          if(!diostream->config->IsOnlyWrite())
                                                                            {
                                                                              diostream->inbuffer->Add(diostream->bufferread, size);
                                                                            }

                                                                          diostream->outbuffer->Extract(NULL, 0, size);
                                                                        }
                                                                    }
                                                                }
                                                                break;


          case DIOSTREAMSPI_FSMSTATE_SENDINGDATA              : break;

          case DIOSTREAMSPI_FSMSTATE_DISCONNECTING            : break;

        }
    }
   else
    {
      if(diostream->GetEvent()<DIOSTREAMSPI_LASTEVENT)
        {
          diostream->CheckTransition();

          switch(diostream->GetCurrentState())
            {
              case DIOSTREAMSPI_FSMSTATE_NONE                 : break;

              case DIOSTREAMSPI_FSMSTATE_CONNECTED            : { DIOSTREAM_XEVENT xevent(diostream, DIOSTREAM_XEVENT_TYPE_CONNECTED);
                                                                  xevent.SetDIOStream(diostream);
                                                                  diostream->PostEvent(&xevent);

                                                                  diostream->SetEvent(DIOSTREAMSPI_FSMEVENT_WAITINGTOREAD);
                                                                }
                                                                break;

              case DIOSTREAMSPI_FSMSTATE_WAITINGTOREAD        : break;

              case DIOSTREAMSPI_FSMSTATE_SENDINGDATA          : break;

              case DIOSTREAMSPI_FSMSTATE_DISCONNECTING        : { DIOSTREAM_XEVENT xevent(diostream, DIOSTREAM_XEVENT_TYPE_DISCONNECTED);
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



