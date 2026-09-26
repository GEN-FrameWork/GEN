/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOWINDOWSStreamUSB.cpp
* 
* @class      DIOWINDOWSSTREAMUSB
* @brief      WINDOWS Data Input/Output Stream USB class
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



#if defined(DIO_ACTIVE) && defined(DIO_STREAMUSB_ACTIVE)


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOWINDOWSStreamUSB.h"

#include "XFactory.h"
#include "XString.h"
#include "XBuffer.h"

#include "DIOStream_XEvent.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWINDOWSSTREAMUSB::DIOWINDOWSSTREAMUSB() : DIOSTREAMUSB() , XFSMACHINE(0)
* @brief      Constructor of class
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWINDOWSSTREAMUSB::DIOWINDOWSSTREAMUSB() : DIOSTREAMUSB() , XFSMACHINE(0)
{
  Clean();

  AddState( DIOWINDOWSUSBFSMSTATE_NONE                ,
            DIOWINDOWSUSBFSMEVENT_CONNECTED           , DIOWINDOWSUSBFSMSTATE_CONNECTED        ,
            DIOWINDOWSUSBFSMEVENT_DISCONNECTING       , DIOWINDOWSUSBFSMSTATE_DISCONNECTING    ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOWINDOWSUSBFSMSTATE_CONNECTED           ,
            DIOWINDOWSUSBFSMEVENT_WAITINGTOREAD       , DIOWINDOWSUSBFSMSTATE_WAITINGTOREAD    ,
            DIOWINDOWSUSBFSMEVENT_SENDINGDATA         , DIOWINDOWSUSBFSMSTATE_SENDINGDATA      ,
            DIOWINDOWSUSBFSMEVENT_DISCONNECTING       , DIOWINDOWSUSBFSMSTATE_DISCONNECTING    ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOWINDOWSUSBFSMSTATE_WAITINGTOREAD       ,
            DIOWINDOWSUSBFSMEVENT_CONNECTED           , DIOWINDOWSUSBFSMSTATE_CONNECTED        ,
            DIOWINDOWSUSBFSMEVENT_SENDINGDATA         , DIOWINDOWSUSBFSMSTATE_SENDINGDATA      ,
            DIOWINDOWSUSBFSMEVENT_DISCONNECTING       , DIOWINDOWSUSBFSMSTATE_DISCONNECTING    ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOWINDOWSUSBFSMSTATE_DISCONNECTING       ,
            DIOWINDOWSUSBFSMEVENT_CONNECTED           , DIOWINDOWSUSBFSMSTATE_CONNECTED        ,
            DIOWINDOWSUSBFSMEVENT_WAITINGTOREAD       , DIOWINDOWSUSBFSMSTATE_WAITINGTOREAD    ,
            DIOWINDOWSUSBFSMEVENT_SENDINGDATA         , DIOWINDOWSUSBFSMSTATE_SENDINGDATA      ,
            XFSMACHINESTATE_EVENTDEFEND);

  threadconnection = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMUSB, __L("DIOWINDOWSSTREAMUSB::DIOWINDOWSSTREAMUSB"), ThreadConnection, (void*)this);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWINDOWSSTREAMUSB::~DIOWINDOWSSTREAMUSB()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWINDOWSSTREAMUSB::~DIOWINDOWSSTREAMUSB()

{
  Close();

  if(threadconnection) DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMUSB, threadconnection);

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMSTATUS DIOWINDOWSSTREAMUSB::GetStatus()
* @brief      Get status
* @ingroup    PLATFORM_WINDOWS
* 
* @return     DIOSTREAMSTATUS : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMSTATUS DIOWINDOWSSTREAMUSB::GetStatus()
{
  if(handle==INVALID_HANDLE_VALUE)  return DIOSTREAMSTATUS_DISCONNECTED;
  if(!config)                       return DIOSTREAMSTATUS_DISCONNECTED;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMUSB::Open()
* @brief      Open
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMUSB::Open()
{
  if(!threadconnection)  return false;

  if(!inbuffer)         return false;
  if(!outbuffer)        return false;

  if(!config)           return false;

  XSTRING comport;
  XSTRING cfg;


  handle = CreateFile(config->GetResource()->Get(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);


  if(handle==INVALID_HANDLE_VALUE) return false;

  memset(&ovi, 0, sizeof(ovi));

  SetEvent(DIOWINDOWSUSBFSMEVENT_CONNECTED);

  status = DIOSTREAMSTATUS_CONNECTED;

  ResetXBuffers();

  ResetConnectionStatistics();

  return threadconnection->Ini();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMUSB::Close()
* @brief      Close
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMUSB::Close()
{
  if(!threadconnection) return false;

  threadconnection->End();

  if(GetStatus()==DIOSTREAMSTATUS_DISCONNECTED) return false;

  CloseHandle(handle);

  handle=INVALID_HANDLE_VALUE;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMUSB::CleanBuffers()
* @brief      Clean buffers
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMUSB::CleanBuffers()
{
  if(GetStatus()==DIOSTREAMSTATUS_DISCONNECTED) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWINDOWSSTREAMUSB::ThreadConnection(void* data)
* @brief      Thread connection
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  data : Data buffer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWINDOWSSTREAMUSB::ThreadConnection(void* data)
{
  DIOWINDOWSSTREAMUSB* diostream = (DIOWINDOWSSTREAMUSB*)data;
  if(!diostream) return;

  if(diostream->GetEvent()==DIOWINDOWSUSBFSMEVENT_NONE) // No hay nuevos Eventos
    {
      switch(diostream->GetCurrentState())
        {
          case DIOWINDOWSUSBFSMSTATE_NONE                 : break;

          case DIOWINDOWSUSBFSMSTATE_CONNECTED              : break;

          case DIOWINDOWSUSBFSMSTATE_WAITINGTOREAD          : { if(!diostream->IsBlockRead())
                                                                  {
                                                                    XBYTE buffer[DIOSTREAM_MAXBUFFER];

                                                                    XDWORD br = diostream->ReadBuffer(buffer, DIOSTREAM_MAXBUFFER);
                                                                    if(br)
                                                                      {
                                                                        diostream->inbuffer->Add(buffer, br);
                                                                      }
                                                                  }

                                                                if(::GetLastError() == 5)
                                                                  {
                                                                    diostream->SetEvent(DIOWINDOWSUSBFSMEVENT_DISCONNECTING);
                                                                    break;
                                                                  }

                                                                if(!diostream->IsBlockWrite())
                                                                  {
                                                                    int esize = diostream->outbuffer->GetSize();
                                                                    if(esize)
                                                                      {
                                                                        diostream->outbuffer->SetBlocked(true);
                                                                        XDWORD bw = diostream->WriteBuffer(diostream->outbuffer->Get(), esize);
                                                                        diostream->outbuffer->SetBlocked(false);
                                                                        if(bw) diostream->outbuffer->Extract(NULL, 0, bw);
                                                                      }
                                                                  }
                                                              }
                                                              break;

          case DIOWINDOWSUSBFSMSTATE_SENDINGDATA            : break;

          case DIOWINDOWSUSBFSMSTATE_DISCONNECTING          : break;

        }
    }
   else
    {
      if(diostream->GetEvent()<DIOWINDOWSUSB_LASTEVENT)
        {
          diostream->CheckTransition();

          switch(diostream->GetCurrentState())
            {
              case DIOWINDOWSUSBFSMSTATE_NONE             : break;

              case DIOWINDOWSUSBFSMSTATE_CONNECTED          : { DIOSTREAM_XEVENT xevent(diostream, DIOSTREAM_XEVENT_TYPE_CONNECTED);
                                                                xevent.SetDIOStream(diostream);

                                                                diostream->PostEvent(&xevent);

                                                                diostream->SetEvent(DIOWINDOWSUSBFSMEVENT_WAITINGTOREAD);
                                                              }
                                                              break;

              case DIOWINDOWSUSBFSMSTATE_WAITINGTOREAD      : break;

              case DIOWINDOWSUSBFSMSTATE_SENDINGDATA        : break;

              case DIOWINDOWSUSBFSMSTATE_DISCONNECTING      : { DIOSTREAM_XEVENT xevent(diostream, DIOSTREAM_XEVENT_TYPE_DISCONNECTED);
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
* @fn         XDWORD DIOWINDOWSSTREAMUSB::ReadBuffer(XBYTE* buffer, XDWORD size)
* @brief      Read buffer
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOWINDOWSSTREAMUSB::ReadBuffer(XBYTE* buffer, XDWORD size)
{
  if(GetStatus()==DIOSTREAMSTATUS_DISCONNECTED) return 0;
  if(size == 0)                                        return 0;

  DWORD br    = 0;
  bool status = true;

  status = ReadFile(handle, buffer, size, &br, NULL)?true:false;

  if(!status) return 0;

  return (XDWORD)br;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOWINDOWSSTREAMUSB::WriteBuffer(XBYTE* buffer, XDWORD size)
* @brief      Write buffer
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOWINDOWSSTREAMUSB::WriteBuffer(XBYTE* buffer, XDWORD size)
{
  if(GetStatus()==DIOSTREAMSTATUS_DISCONNECTED) return 0;

  if(size == 0)    return 0;

  DWORD      bw         = 0;
  OVERLAPPED oviwrite   = { 0 };
  bool       status;

  oviwrite.hEvent = CreateEvent(0, TRUE, FALSE, 0);
  if(!oviwrite.hEvent) return bw;

  if(!WriteFile(handle, buffer, size, &bw, &oviwrite))
    {
      if(GetLastError()!=ERROR_IO_PENDING)
        {
          status = false;
        }
       else
        {
          switch(WaitForSingleObject(oviwrite.hEvent, writetimeout))
            {
              case WAIT_OBJECT_0 :  if(!GetOverlappedResult(handle, &oviwrite, &bw, TRUE))
                                           status = false;
                                      else status = true;
                                    break;

              case WAIT_TIMEOUT  : CancelIo(handle);
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
* @fn         void DIOWINDOWSSTREAMUSB::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWINDOWSSTREAMUSB::Clean()
{
  threadconnection = NULL;

  handle           = INVALID_HANDLE_VALUE;
  hevent           = NULL;

  memset(&ovi, 0, sizeof(ovi));

  readtimeout      = 3000;
  writetimeout     = 3000;
}




#endif

