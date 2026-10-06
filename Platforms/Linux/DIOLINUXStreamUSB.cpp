/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOLINUXStreamUSB.cpp
* 
* @class      DIOLINUXSTREAMUSB
* @brief      LINUX Data Input/Output Stream USB class
* @ingroup    PLATFORM_LINUX
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

#include "DIOLINUXStreamUSB.h"

#include "XFactory.h"
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
* @fn         DIOLINUXSTREAMUSB::DIOLINUXSTREAMUSB() : DIOSTREAMUSB(), XFSMACHINE(0)
* @brief      Constructor of class
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOLINUXSTREAMUSB::DIOLINUXSTREAMUSB() : DIOSTREAMUSB(), XFSMACHINE(0)
{
  Clean();

  AddState( DIOLINUXUSBFSMSTATE_NONE              ,
            DIOLINUXUSBFSMEVENT_CONNECTED       , DIOLINUXUSBFSMSTATE_CONNECTED        ,
            DIOLINUXUSBFSMEVENT_DISCONNECTING   , DIOLINUXUSBFSMSTATE_DISCONNECTING    ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOLINUXUSBFSMSTATE_CONNECTED       ,
            DIOLINUXUSBFSMEVENT_WAITINGTOREAD   , DIOLINUXUSBFSMSTATE_WAITINGTOREAD    ,
            DIOLINUXUSBFSMEVENT_SENDINGDATA     , DIOLINUXUSBFSMSTATE_SENDINGDATA      ,
            DIOLINUXUSBFSMEVENT_DISCONNECTING   , DIOLINUXUSBFSMSTATE_DISCONNECTING    ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOLINUXUSBFSMSTATE_WAITINGTOREAD   ,
            DIOLINUXUSBFSMEVENT_CONNECTED       , DIOLINUXUSBFSMSTATE_CONNECTED        ,
            DIOLINUXUSBFSMEVENT_SENDINGDATA     , DIOLINUXUSBFSMSTATE_SENDINGDATA      ,
            DIOLINUXUSBFSMEVENT_DISCONNECTING   , DIOLINUXUSBFSMSTATE_DISCONNECTING    ,
            XFSMACHINESTATE_EVENTDEFEND);

  AddState( DIOLINUXUSBFSMSTATE_DISCONNECTING   ,
            DIOLINUXUSBFSMEVENT_CONNECTED       , DIOLINUXUSBFSMSTATE_CONNECTED        ,
            DIOLINUXUSBFSMEVENT_WAITINGTOREAD   , DIOLINUXUSBFSMSTATE_WAITINGTOREAD    ,
            DIOLINUXUSBFSMEVENT_SENDINGDATA     , DIOLINUXUSBFSMSTATE_SENDINGDATA      ,
            XFSMACHINESTATE_EVENTDEFEND);

  threadconnection = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMUSB, _L("DIOLINUXSTREAMUSB::DIOLINUXSTREAMUSB"), ThreadConnection, (void*)this);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOLINUXSTREAMUSB::~DIOLINUXSTREAMUSB()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOLINUXSTREAMUSB::~DIOLINUXSTREAMUSB()
{
  Close();

  if(threadconnection) DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMUSB, threadconnection);

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMSTATUS DIOLINUXSTREAMUSB::GetStatus()
* @brief      Get status
* @ingroup    PLATFORM_LINUX
* 
* @return     DIOSTREAMSTATUS : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMSTATUS DIOLINUXSTREAMUSB::GetStatus()
{
  if(fd<0)    return DIOSTREAMSTATUS_DISCONNECTED;
  if(!config) return DIOSTREAMSTATUS_DISCONNECTED;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXSTREAMUSB::Open()
* @brief      Open
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXSTREAMUSB::Open()
{
  if(!config) return false;

  XBUFFER charstr;
  
  (*config->GetResource()).ConvertToASCII(charstr);  
  fd= open(charstr.GetPtrChar(), O_RDWR | O_NOCTTY | O_NONBLOCK);  
  if(fd<0) return false;

  //fcntl(fd, F_SETFL, FNDELAY);
  //fcntl(fd, F_SETFL, 0);

  SetEvent(DIOLINUXUSBFSMEVENT_CONNECTED);

  status = DIOSTREAMSTATUS_CONNECTED;

  ResetXBuffers();

  ResetConnectionStatistics();

  return threadconnection->Ini();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXSTREAMUSB::Disconnect()
* @brief      Disconnect
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXSTREAMUSB::Disconnect()                                
{ 
  return false; 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXSTREAMUSB::Close()
* @brief      Close
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXSTREAMUSB::Close()
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
* @fn         bool DIOLINUXSTREAMUSB::CleanBuffers()
* @brief      Clean buffers
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXSTREAMUSB::CleanBuffers()
{
  if(GetStatus()==DIOSTREAMSTATUS_DISCONNECTED) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOLINUXSTREAMUSB::ThreadConnection(void* data)
* @brief      Thread connection
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  data : Data buffer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOLINUXSTREAMUSB::ThreadConnection(void* data)
{
  DIOLINUXSTREAMUSB* diostream = (DIOLINUXSTREAMUSB*)data;
  if(!diostream) return;

  if(diostream->GetEvent()==DIOLINUXUSBFSMEVENT_NONE) // No hay nuevos Eventos
    {
      switch(diostream->GetCurrentState())
        {
          case DIOLINUXUSBFSMSTATE_NONE                   : break;

          case DIOLINUXUSBFSMSTATE_CONNECTED                : break;

          case DIOLINUXUSBFSMSTATE_WAITINGTOREAD            : {
                                                                if(!diostream->IsBlockRead())
                                                                  {
                                                                    XBYTE buffer[DIOSTREAM_MAXBUFFER];

                                                                    XDWORD br = diostream->ReadBuffer(buffer, DIOSTREAM_MAXBUFFER);
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
                                                                        if(esize > DIOSTREAM_MAXBUFFER) esize = DIOSTREAM_MAXBUFFER;

                                                                        diostream->outbuffer->SetBlocked(true);
                                                                        XDWORD bw = diostream->WriteBuffer(diostream->outbuffer->Get(), esize);
                                                                        diostream->outbuffer->SetBlocked(false);

                                                                        if(bw)
                                                                          {
                                                                            //XTRACE_PRINTDATABLOCKCOLOR(XTRACE_COLOR_RED, (XBYTE*)diostream->buffer, (int)bw);
                                                                            diostream->outbuffer->Extract(NULL , 0, bw);
                                                                          }
                                                                         else
                                                                          {
                                                                            //XTRACE_PRINTDATABLOCKCOLOR(XTRACE_COLOR_PURPLE, (XBYTE*)diostream->buffer, (int)bw);
                                                                          }
                                                                      }
                                                                  }
                                                              }
                                                              break;


          case DIOLINUXUSBFSMSTATE_SENDINGDATA              : break;

          case DIOLINUXUSBFSMSTATE_DISCONNECTING            : break;

        }
    }
   else
    {
      if(diostream->GetEvent()<DIOLINUXUSB_LASTEVENT)
        {
          diostream->CheckTransition();

          switch(diostream->GetCurrentState())
            {
              case DIOLINUXUSBFSMSTATE_NONE               : break;

              case DIOLINUXUSBFSMSTATE_CONNECTED            : { DIOSTREAM_XEVENT xevent(diostream, DIOSTREAM_XEVENT_TYPE_CONNECTED);
                                                                xevent.SetDIOStream(diostream);
                                                                diostream->PostEvent(&xevent);

                                                                diostream->SetEvent(DIOLINUXUSBFSMEVENT_WAITINGTOREAD);
                                                              }
                                                              break;

              case DIOLINUXUSBFSMSTATE_WAITINGTOREAD        : break;

              case DIOLINUXUSBFSMSTATE_SENDINGDATA          : break;

              case DIOLINUXUSBFSMSTATE_DISCONNECTING        : { DIOSTREAM_XEVENT xevent(diostream, DIOSTREAM_XEVENT_TYPE_DISCONNECTED);
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
* @fn         XDWORD DIOLINUXSTREAMUSB::ReadBuffer(XBYTE* buffer, XDWORD size)
* @brief      Read buffer
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOLINUXSTREAMUSB::ReadBuffer(XBYTE* buffer, XDWORD size)
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
* @fn         XDWORD DIOLINUXSTREAMUSB::WriteBuffer(XBYTE* buffer, XDWORD size)
* @brief      Write buffer
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOLINUXSTREAMUSB::WriteBuffer(XBYTE* buffer, XDWORD size)
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
* @fn         void DIOLINUXSTREAMUSB::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOLINUXSTREAMUSB::Clean()
{
  fd            = -1;
  readtimeout   = 3000;
  writetimeout  = 3000;
}




#endif



