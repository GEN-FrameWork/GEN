/**-------------------------------------------------------------------------------------------------------------------
*
* @file       DIOESP32StreamUSB.cpp
*
* @class      DIOESP32STREAMUSB
* @brief      Data Input/Output ESP32 Stream USB class
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

#if defined(DIO_ACTIVE) && defined(DIO_STREAMUSB_ACTIVE)

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XESP32_HAL.h"
#include "usbd_cdc_if.h"
#include "usb_device.h"

#include "XFactory.h"
#include "XThreadCollected.h"

#include "DIOStream_XEvent.h"
#include "DIOStreamUSBConfig.h"

#include "DIOESP32StreamUSB.h"

#include "XMemory_Control.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

DIOESP32STREAMUSB*      diostreamusbptrhandle =  NULL;

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/



/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOESP32STREAMUSB::DIOESP32STREAMUSB() : DIOSTREAMUSB(), XFSMACHINE(0)
* @brief      Constructor of class
* @ingroup    PLATFORM_ESP32
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOESP32STREAMUSB::DIOESP32STREAMUSB() : DIOSTREAMUSB(), XFSMACHINE(0)
{
  Clean();

  AddState( DIOESP32USBFSMSTATE_NONE              ,
            DIOESP32USBFSMEVENT_CONNECTED       , DIOESP32USBFSMSTATE_CONNECTED        ,
            DIOESP32USBFSMEVENT_DISCONNECTING   , DIOESP32USBFSMSTATE_DISCONNECTING    ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOESP32USBFSMSTATE_CONNECTED       ,
            DIOESP32USBFSMEVENT_WAITINGTOREAD   , DIOESP32USBFSMSTATE_WAITINGTOREAD    ,
            DIOESP32USBFSMEVENT_DISCONNECTING   , DIOESP32USBFSMSTATE_DISCONNECTING    ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOESP32USBFSMSTATE_WAITINGTOREAD   ,
            DIOESP32USBFSMEVENT_CONNECTED       , DIOESP32USBFSMSTATE_CONNECTED        ,
            DIOESP32USBFSMEVENT_DISCONNECTING   , DIOESP32USBFSMSTATE_DISCONNECTING    ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOESP32USBFSMSTATE_DISCONNECTING   ,
            DIOESP32USBFSMEVENT_CONNECTED       , DIOESP32USBFSMSTATE_CONNECTED        ,
            DIOESP32USBFSMEVENT_WAITINGTOREAD   , DIOESP32USBFSMSTATE_WAITINGTOREAD    ,
            XFSMACHINESTATE_EVENTDEFEND);

  threadconnection = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMUSB, _L("DIOESP32STREAMUSB::DIOESP32STREAMUSB"), ThreadConnection, (void*)this);
}



/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOESP32STREAMUSB::~DIOESP32STREAMUSB()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_ESP32
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOESP32STREAMUSB::~DIOESP32STREAMUSB()
{
  Close();

  if(threadconnection) DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMUSB, threadconnection);

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMSTATUS DIOESP32STREAMUSB::GetStatus()
* @brief      Get status
* @ingroup    PLATFORM_ESP32
* 
* @return     DIOSTREAMSTATUS : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMSTATUS DIOESP32STREAMUSB::GetStatus()
{
  if(!config) return DIOSTREAMSTATUS_DISCONNECTED;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOESP32STREAMUSB::Open()
* @brief      Open
* @ingroup    PLATFORM_ESP32
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOESP32STREAMUSB::Open()
{
  if(!threadconnection)  return false;

  MX_USB_DEVICE_Init();

  diostreamusbptrhandle = this;

  SetEvent(DIOESP32USBFSMEVENT_CONNECTED);

  status = DIOSTREAMSTATUS_CONNECTED;

  ResetXBuffers();
  ResetConnectionStatistics();

  return threadconnection->Ini();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOESP32STREAMUSB::WriteDirect(XBYTE* buffer, XDWORD size)
* @brief      Write direct
* @ingroup    PLATFORM_ESP32
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOESP32STREAMUSB::WriteDirect(XBYTE* buffer, XDWORD size)
{
  if(GetStatus()==DIOSTREAMSTATUS_DISCONNECTED) return 0;

  if(CDC_Transmit_FS((uint8_t*)buffer, (XWORD)size) == USBD_OK) return size;

  return (XDWORD)0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOESP32STREAMUSB::Write(XBYTE* buffer, XDWORD size)
* @brief      Write
* @ingroup    PLATFORM_ESP32
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOESP32STREAMUSB::Write(XBYTE* buffer, XDWORD size)
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
* @fn         bool DIOESP32STREAMUSB::Disconnect()
* @brief      Disconnect
* @ingroup    PLATFORM_ESP32
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOESP32STREAMUSB::Disconnect()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOESP32STREAMUSB::Close()
* @brief      Close
* @ingroup    PLATFORM_ESP32
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOESP32STREAMUSB::Close()
{
  if(!threadconnection) return false;

  threadconnection->End();

  if(GetStatus()==DIOSTREAMSTATUS_DISCONNECTED) return false;

  diostreamusbptrhandle = NULL;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOESP32STREAMUSB::CleanBuffers()
* @brief      Clean buffers
* @note       INTERNAL
* @ingroup    PLATFORM_ESP32
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOESP32STREAMUSB::CleanBuffers()
{

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOESP32STREAMUSB::HAL_USB_RxCpltCallback(uint8_t* buffer, uint32_t *len)
* @brief      HALUSB rx cplt callback
* @ingroup    PLATFORM_ESP32
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  len : Len pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOESP32STREAMUSB::HAL_USB_RxCpltCallback(uint8_t* buffer, uint32_t *len)
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
* @fn         void DIOESP32STREAMUSB::ThreadConnection(void* param)
* @brief      Thread connection
* @ingroup    PLATFORM_ESP32
* 
* @param[in]  param : Param pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOESP32STREAMUSB::ThreadConnection(void* param)
{
  DIOESP32STREAMUSB* diostream = (DIOESP32STREAMUSB*)param;
  if(!diostream) return;

  if(diostream->GetEvent()==DIOESP32USBFSMEVENT_NONE) // No hay nuevos Eventos
    {
      switch(diostream->GetCurrentState())
        {
          case DIOESP32USBFSMSTATE_NONE                     : break;

          case DIOESP32USBFSMSTATE_CONNECTED                : break;

          case DIOESP32USBFSMSTATE_WAITINGTOREAD            : { if(!diostream->IsBlockRead())
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

          case DIOESP32USBFSMSTATE_DISCONNECTING            : break;

        }
    }
   else
    {
      if(diostream->GetEvent()<DIOESP32USB_LASTEVENT)
        {
          diostream->CheckTransition();

          switch(diostream->GetCurrentState())
            {
              case DIOESP32USBFSMSTATE_NONE               : break;

              case DIOESP32USBFSMSTATE_CONNECTED            : { DIOSTREAM_XEVENT xevent(diostream, DIOSTREAM_XEVENT_TYPE_CONNECTED);
                                                                    xevent.SetDIOStream(diostream);
                                                                    diostream->PostEvent(&xevent);

                                                                    diostream->SetEvent(DIOESP32USBFSMEVENT_WAITINGTOREAD);
                                                                  }
                                                                  break;

              case DIOESP32USBFSMSTATE_WAITINGTOREAD        : break;

              case DIOESP32USBFSMSTATE_DISCONNECTING        : { DIOSTREAM_XEVENT xevent(diostream, DIOSTREAM_XEVENT_TYPE_DISCONNECTED);
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
* @fn         void DIOESP32STREAMUSB::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_ESP32
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOESP32STREAMUSB::Clean()
{
  threadconnection   = NULL;
}


#endif



