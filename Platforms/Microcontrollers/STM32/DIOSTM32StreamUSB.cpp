/**-------------------------------------------------------------------------------------------------------------------
*
* @file       DIOSTM32StreamUSB.cpp
*
* @class      DIOSTM32STREAMUSB
* @brief      Data Input/Output STM32 Stream USB class
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

#if defined(DIO_ACTIVE) && defined(DIO_STREAMUSB_ACTIVE)

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XSTM32_HAL.h"
#include "usbd_cdc_if.h"
#include "usb_device.h"

#include "XFactory.h"
#include "XThreadCollected.h"

#include "DIOStream_XEvent.h"
#include "DIOStreamUSBConfig.h"

#include "DIOSTM32StreamUSB.h"

#include "XMemory_Control.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

DIOSTM32STREAMUSB*      diostreamusbptrhandle =  NULL;

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/



/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTM32STREAMUSB::DIOSTM32STREAMUSB() : DIOSTREAMUSB(), XFSMACHINE(0)
* @brief      Constructor of class
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTM32STREAMUSB::DIOSTM32STREAMUSB() : DIOSTREAMUSB(), XFSMACHINE(0)
{
  Clean();

  AddState( DIOSTM32USBFSMSTATE_NONE              ,
            DIOSTM32USBFSMEVENT_CONNECTED       , DIOSTM32USBFSMSTATE_CONNECTED        ,
            DIOSTM32USBFSMEVENT_DISCONNECTING   , DIOSTM32USBFSMSTATE_DISCONNECTING    ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOSTM32USBFSMSTATE_CONNECTED       ,
            DIOSTM32USBFSMEVENT_WAITINGTOREAD   , DIOSTM32USBFSMSTATE_WAITINGTOREAD    ,
            DIOSTM32USBFSMEVENT_DISCONNECTING   , DIOSTM32USBFSMSTATE_DISCONNECTING    ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOSTM32USBFSMSTATE_WAITINGTOREAD   ,
            DIOSTM32USBFSMEVENT_CONNECTED       , DIOSTM32USBFSMSTATE_CONNECTED        ,
            DIOSTM32USBFSMEVENT_DISCONNECTING   , DIOSTM32USBFSMSTATE_DISCONNECTING    ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOSTM32USBFSMSTATE_DISCONNECTING   ,
            DIOSTM32USBFSMEVENT_CONNECTED       , DIOSTM32USBFSMSTATE_CONNECTED        ,
            DIOSTM32USBFSMEVENT_WAITINGTOREAD   , DIOSTM32USBFSMSTATE_WAITINGTOREAD    ,
            XFSMACHINESTATE_EVENTDEFEND);

  threadconnection = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMUSB, _L("DIOSTM32STREAMUSB::DIOSTM32STREAMUSB"), ThreadConnection, (void*)this);
}



/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTM32STREAMUSB::~DIOSTM32STREAMUSB()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTM32STREAMUSB::~DIOSTM32STREAMUSB()
{
  Close();

  if(threadconnection) DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMUSB, threadconnection);

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMSTATUS DIOSTM32STREAMUSB::GetStatus()
* @brief      Get status
* @ingroup    PLATFORM_STM32
* 
* @return     DIOSTREAMSTATUS : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMSTATUS DIOSTM32STREAMUSB::GetStatus()
{
  if(!config) return DIOSTREAMSTATUS_DISCONNECTED;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTM32STREAMUSB::Open()
* @brief      Open
* @ingroup    PLATFORM_STM32
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTM32STREAMUSB::Open()
{
  if(!threadconnection)  return false;

  MX_USB_DEVICE_Init();

  diostreamusbptrhandle = this;

  SetEvent(DIOSTM32USBFSMEVENT_CONNECTED);

  status = DIOSTREAMSTATUS_CONNECTED;

  ResetXBuffers();
  ResetConnectionStatistics();

  return threadconnection->Ini();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOSTM32STREAMUSB::WriteDirect(XBYTE* buffer, XDWORD size)
* @brief      Write direct
* @ingroup    PLATFORM_STM32
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTM32STREAMUSB::WriteDirect(XBYTE* buffer, XDWORD size)
{
  if(GetStatus()==DIOSTREAMSTATUS_DISCONNECTED) return 0;

  if(CDC_Transmit_FS((uint8_t*)buffer, (XWORD)size) == USBD_OK) return size;

  return (XDWORD)0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOSTM32STREAMUSB::Write(XBYTE* buffer, XDWORD size)
* @brief      Write
* @ingroup    PLATFORM_STM32
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTM32STREAMUSB::Write(XBYTE* buffer, XDWORD size)
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
* @fn         bool DIOSTM32STREAMUSB::Disconnect()
* @brief      Disconnect
* @ingroup    PLATFORM_STM32
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTM32STREAMUSB::Disconnect()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTM32STREAMUSB::Close()
* @brief      Close
* @ingroup    PLATFORM_STM32
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTM32STREAMUSB::Close()
{
  if(!threadconnection) return false;

  threadconnection->End();

  if(GetStatus()==DIOSTREAMSTATUS_DISCONNECTED) return false;

  diostreamusbptrhandle = NULL;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTM32STREAMUSB::CleanBuffers()
* @brief      Clean buffers
* @note       INTERNAL
* @ingroup    PLATFORM_STM32
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTM32STREAMUSB::CleanBuffers()
{

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTM32STREAMUSB::HAL_USB_RxCpltCallback(uint8_t* buffer, uint32_t *len)
* @brief      HALUSB rx cplt callback
* @ingroup    PLATFORM_STM32
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  len : Len pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTM32STREAMUSB::HAL_USB_RxCpltCallback(uint8_t* buffer, uint32_t *len)
{
  if(GetStatus() == DIOSTREAMSTATUS_DISCONNECTED) return;
  if(IsBlockRead())                                      return;

  if(!inbuffer->IsBlocked())
    {
      if(readcache.GetSize())
        {
          inbuffer->Add(readcache);
          readcache.Delete();
        }

      inbuffer->Add((XBYTE*)buffer, (XDWORD)(*len));
    }
   else
    {
      readcache.Add((XBYTE*)buffer, (XDWORD)(*len));
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTM32STREAMUSB::ThreadConnection(void* param)
* @brief      Thread connection
* @ingroup    PLATFORM_STM32
* 
* @param[in]  param : Param pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTM32STREAMUSB::ThreadConnection(void* param)
{
  DIOSTM32STREAMUSB* diostream = (DIOSTM32STREAMUSB*)param;
  if(!diostream) return;

  if(diostream->GetEvent()==DIOSTM32USBFSMEVENT_NONE) // No hay nuevos Eventos
    {
      switch(diostream->GetCurrentState())
        {
          case DIOSTM32USBFSMSTATE_NONE                     : break;

          case DIOSTM32USBFSMSTATE_CONNECTED                : break;

          case DIOSTM32USBFSMSTATE_WAITINGTOREAD            : { if(!diostream->IsBlockRead())
                                                                      {
                                                                        if(!diostream->inbuffer->IsBlocked())
                                                                          {
                                                                            if(diostream->readcache.GetSize())
                                                                              {
                                                                                diostream->inbuffer->Add(diostream->readcache);
                                                                                diostream->readcache.Delete();
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

          case DIOSTM32USBFSMSTATE_DISCONNECTING            : break;

        }
    }
   else
    {
      if(diostream->GetEvent()<DIOSTM32USB_LASTEVENT)
        {
          diostream->CheckTransition();

          switch(diostream->GetCurrentState())
            {
              case DIOSTM32USBFSMSTATE_NONE               : break;

              case DIOSTM32USBFSMSTATE_CONNECTED            : { DIOSTREAM_XEVENT xevent(diostream, DIOSTREAM_XEVENT_TYPE_CONNECTED);
                                                                    xevent.SetDIOStream(diostream);
                                                                    diostream->PostEvent(&xevent);

                                                                    diostream->SetEvent(DIOSTM32USBFSMEVENT_WAITINGTOREAD);
                                                                  }
                                                                  break;

              case DIOSTM32USBFSMSTATE_WAITINGTOREAD        : break;

              case DIOSTM32USBFSMSTATE_DISCONNECTING        : { DIOSTREAM_XEVENT xevent(diostream, DIOSTREAM_XEVENT_TYPE_DISCONNECTED);
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
* @fn         void DIOSTM32STREAMUSB::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTM32STREAMUSB::Clean()
{
  threadconnection   = NULL;
}


#endif



