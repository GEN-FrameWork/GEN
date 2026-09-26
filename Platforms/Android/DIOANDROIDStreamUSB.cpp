/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOANDROIDStreamUSB.cpp
* 
* @class      DIOANDROIDSTREAMUSB
* @brief      ANDROID Data Input/Output Stream USB class
* @ingroup    PLATFORM_ANDROID
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

#include "DIOANDROIDStreamUSB.h"

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <errno.h>
#include <termios.h>

#include "XFactory.h"
#include "XThread.h"
#include "XBase.h"
#include "XString.h"
#include "XTrace.h"

#include "DIOStream_XEvent.h"
#include "DIOStreamUSBConfig.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOANDROIDSTREAMUSB::DIOANDROIDSTREAMUSB() : DIOSTREAMUSB(), XFSMACHINE(0)
* @brief      Constructor of class
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOANDROIDSTREAMUSB::DIOANDROIDSTREAMUSB() : DIOSTREAMUSB(), XFSMACHINE(0)
{
  Clean();

  AddState( DIOANDROIDUSBFSMSTATE_NONE              ,
            DIOANDROIDUSBFSMEVENT_CONNECTED       , DIOANDROIDUSBFSMSTATE_CONNECTED        ,
            DIOANDROIDUSBFSMEVENT_DISCONNECTING   , DIOANDROIDUSBFSMSTATE_DISCONNECTING    ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOANDROIDUSBFSMSTATE_CONNECTED       ,
            DIOANDROIDUSBFSMEVENT_WAITINGTOREAD   , DIOANDROIDUSBFSMSTATE_WAITINGTOREAD    ,
            DIOANDROIDUSBFSMEVENT_SENDINGDATA     , DIOANDROIDUSBFSMSTATE_SENDINGDATA      ,
            DIOANDROIDUSBFSMEVENT_DISCONNECTING   , DIOANDROIDUSBFSMSTATE_DISCONNECTING    ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOANDROIDUSBFSMSTATE_WAITINGTOREAD   ,
            DIOANDROIDUSBFSMEVENT_CONNECTED       , DIOANDROIDUSBFSMSTATE_CONNECTED        ,
            DIOANDROIDUSBFSMEVENT_SENDINGDATA     , DIOANDROIDUSBFSMSTATE_SENDINGDATA      ,
            DIOANDROIDUSBFSMEVENT_DISCONNECTING   , DIOANDROIDUSBFSMSTATE_DISCONNECTING    ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOANDROIDUSBFSMSTATE_DISCONNECTING   ,
            DIOANDROIDUSBFSMEVENT_CONNECTED       , DIOANDROIDUSBFSMSTATE_CONNECTED        ,
            DIOANDROIDUSBFSMEVENT_WAITINGTOREAD   , DIOANDROIDUSBFSMSTATE_WAITINGTOREAD    ,
            DIOANDROIDUSBFSMEVENT_SENDINGDATA     , DIOANDROIDUSBFSMSTATE_SENDINGDATA      ,
            XFSMACHINESTATE_EVENTDEFEND);

  threadconnection = CREATEXTHREAD(__L("DIOANDROIDSTREAMUSB::DIOANDROIDSTREAMUSB"), ThreadConnection, (void*)this);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOANDROIDSTREAMUSB::~DIOANDROIDSTREAMUSB()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOANDROIDSTREAMUSB::~DIOANDROIDSTREAMUSB()
{
  Close();

  if(threadconnection) DELETEXTHREAD(threadconnection);

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMSTATUS DIOANDROIDSTREAMUSB::GetStatus()
* @brief      Get status
* @ingroup    PLATFORM_ANDROID
* 
* @return     DIOSTREAMSTATUS : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMSTATUS DIOANDROIDSTREAMUSB::GetStatus()
{
  if(fd<0)    return DIOSTREAMSTATUS_DISCONNECTED;
  if(!config) return DIOSTREAMSTATUS_DISCONNECTED;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOANDROIDSTREAMUSB::Open()
* @brief      Open
* @ingroup    PLATFORM_ANDROID
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOANDROIDSTREAMUSB::Open()
{
  if(!config) return false;

  XBUFFER charstr;

  (*config->GetDeviceID()).ConvertToASCII(charstr); 
  fd= open(charstr.GetPtrChar(), O_RDWR | O_NOCTTY | O_NONBLOCK);  
  if(fd<0) return false;

  //fcntl(fd, F_SETFL, FNDELAY);
  //fcntl(fd, F_SETFL, 0);

  SetEvent(DIOANDROIDUSBFSMEVENT_CONNECTED);

  status = DIOSTREAMSTATUS_CONNECTED;

  ResetXBuffers();

  ResetConnectionStatistics();

  return threadconnection->Ini();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOANDROIDSTREAMUSB::Disconnect()
* @brief      Disconnect
* @ingroup    PLATFORM_ANDROID
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOANDROIDSTREAMUSB::Disconnect()                                
{ 
  return false; 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOANDROIDSTREAMUSB::Close()
* @brief      Close
* @ingroup    PLATFORM_ANDROID
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOANDROIDSTREAMUSB::Close()
{
  if(!threadconnection) return false;

  threadconnection->End();

  if(GetStatus()==DIOSTREAMSTATUS_DISCONNECTED) return false;

  if(fd>=0)
    {
      close(fd);
      fd = -1;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOANDROIDSTREAMUSB::CleanBuffers()
* @brief      Clean buffers
* @note       INTERNAL
* @ingroup    PLATFORM_ANDROID
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOANDROIDSTREAMUSB::CleanBuffers()
{
  if(GetStatus()==DIOSTREAMSTATUS_DISCONNECTED) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOANDROIDSTREAMUSB::ThreadConnection(void* data)
* @brief      Thread connection
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  data : Data buffer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOANDROIDSTREAMUSB::ThreadConnection(void* data)
{
  DIOANDROIDSTREAMUSB* diostream = (DIOANDROIDSTREAMUSB*)data;
  if(!diostream) return;

  if(diostream->GetEvent()==DIOANDROIDUSBFSMEVENT_NONE) // No hay nuevos Eventos
    {
      switch(diostream->GetCurrentState())
        {
          case DIOANDROIDUSBFSMSTATE_NONE                   : break;

          case DIOANDROIDUSBFSMSTATE_CONNECTED                : break;

          case DIOANDROIDUSBFSMSTATE_WAITINGTOREAD            : {

                                                                  if(!diostream->IsBlockRead())
                                                                    {
                                                                      XBYTE buffer;

                                                                      XDWORD br = diostream->ReadBuffer(buffer, DIOSTREAMUSB_MAXBUFFER);
                                                                      if(br)
                                                                        {
                                                                          //XTRACE_PRINTDATABLOCKCOLOR(XTRACE_COLOR_BLUE, (XBYTE*)diostream->buffer, (int)br);
                                                                          diostream->inbuffer->Add(buffer, br);
                                                                        }
                                                                    }

                                                                  if(!diostream->IsBlockWrite())
                                                                    {
                                                                      int esize = diostream->outbuffer->GetSize();
                                                                      if(esize)
                                                                        {
                                                                          if(esize>DIOSTREAMUSB_MAXBUFFER) esize = DIOSTREAMUSB_MAXBUFFER;

                                                                          diostream->outbuffer->SetBlocked(true);
                                                                          XDWORD bw = diostream->WriteBuffer(diostream->outbuffer->Get(), esize);
                                                                          diostream->outbuffer->SetBlocked(false);

                                                                          if(bw)
                                                                            {
                                                                              //XTRACE_PRINTDATABLOCKCOLOR(XTRACE_COLOR_RED, (XBYTE*)diostream->buffer, (int)bw);
                                                                              diostream->outbuffer->Extract(NULL, 0, bw);
                                                                            }
                                                                           else
                                                                            {
                                                                              //XTRACE_PRINTDATABLOCKCOLOR(XTRACE_COLOR_PURPLE, (XBYTE*)diostream->buffer, (int)bw);
                                                                            }
                                                                        }
                                                                    }
                                                              }
                                                              break;


          case DIOANDROIDUSBFSMSTATE_SENDINGDATA              : break;

          case DIOANDROIDUSBFSMSTATE_DISCONNECTING            : break;

        }
    }
   else
    {
      if(diostream->GetEvent()<DIOANDROIDUSB_LASTEVENT)
        {
          diostream->CheckTransition();

          switch(diostream->GetCurrentState())
            {
              case DIOANDROIDUSBFSMSTATE_NONE               : break;

              case DIOANDROIDUSBFSMSTATE_CONNECTED            : { DIOSTREAM_XEVENT xevent(diostream, DIOSTREAM_XEVENT_TYPE_CONNECTED);
                                                                xevent.SetDIOStream(diostream);
                                                                if(diostream->PostEvent(&xevent, true)) xevent.WaitForCompleted(5);

                                                                diostream->SetEvent(DIOANDROIDUSBFSMEVENT_WAITINGTOREAD);
                                                              }
                                                              break;

              case DIOANDROIDUSBFSMSTATE_WAITINGTOREAD        : break;

              case DIOANDROIDUSBFSMSTATE_SENDINGDATA          : break;

              case DIOANDROIDUSBFSMSTATE_DISCONNECTING        : { DIOSTREAM_XEVENT xevent(diostream, DIOSTREAM_XEVENT_TYPE_DISCONNECTED);
                                                                xevent.SetDIOStream(diostream);
                                                                if(diostream->PostEvent(&xevent, true)) xevent.WaitForCompleted(5);

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
* @fn         XDWORD DIOANDROIDSTREAMUSB::ReadBuffer(XBYTE* buffer, XDWORD size)
* @brief      Read buffer
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOANDROIDSTREAMUSB::ReadBuffer(XBYTE* buffer, XDWORD size)
{
  if(GetStatus()==DIOSTREAMSTATUS_DISCONNECTED) return 0;

  fd_set         fds;
  struct timeval tv;

  tv.tv_sec  = 0;
  tv.tv_usec = readtimeout;

  FD_ZERO(&fds);
  FD_SET(fd, &fds);

  int br = 0;

  if(select(fd+1, &fds, NULL, NULL, &tv) > 0)
    {
      br = read(fd, buffer, size);

      if(br< 0)
        {
          return 0;
        }
    }

  return (XDWORD)br;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOANDROIDSTREAMUSB::WriteBuffer(XBYTE* buffer, XDWORD size)
* @brief      Write buffer
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOANDROIDSTREAMUSB::WriteBuffer(XBYTE* buffer, XDWORD size)
{
  if(GetStatus()==DIOSTREAMSTATUS_DISCONNECTED) return 0;

  fd_set         fds;
  struct timeval tv;

  tv.tv_sec  = 0;
  tv.tv_usec = writetimeout;

  FD_ZERO(&fds);
  FD_SET(fd, &fds);

  int bw = 0;

  if(select(fd+1, NULL , &fds, NULL, &tv) > 0)
    {
      bw = write(fd, buffer, size);
      if(bw<0) return 0;
    }

  return (XDWORD)bw;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOANDROIDSTREAMUSB::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOANDROIDSTREAMUSB::Clean()
{
  fd            = -1;
  readtimeout   = 3000;
  writetimeout  = 3000;
}




#endif

