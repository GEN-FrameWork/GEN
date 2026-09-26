/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOANDROIDStreamI2C.cpp
* 
* @class      DIOANDROIDSTREAMI2C
* @brief      ANDROID Data Input/Output Stream I2C class
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



#if defined(DIO_ACTIVE) && defined(DIO_STREAMI2C_ACTIVE)


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOANDROIDStreamI2C.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <getopt.h>
#include <linux/types.h>


#include "XFactory.h"
#include "XBuffer.h"
#include "XThreadCollected.h"
#include "XSystem.h"
#include "XTrace.h"

#include "DIOFactory.h"
#include "DIOStream_XEvent.h"
#include "DIOStreamI2CConfig.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOANDROIDSTREAMI2C::DIOANDROIDSTREAMI2C() : DIOSTREAMI2C()
* @brief      Constructor of class
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOANDROIDSTREAMI2C::DIOANDROIDSTREAMI2C() : DIOSTREAMI2C()
{
  Clean();

  threadconnection = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMI2C, __L("DIOANDROIDSTREAMI2C::DIOANDROIDSTREAMI2C"), ThreadConnection, (void*)this);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOANDROIDSTREAMI2C::~DIOANDROIDSTREAMI2C()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOANDROIDSTREAMI2C::~DIOANDROIDSTREAMI2C()
{
  if(threadconnection)
    {
      threadconnection->End();
      DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMI2C, threadconnection);
      threadconnection = NULL;
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOANDROIDSTREAMI2C::Open()
* @brief      Open
* @ingroup    PLATFORM_ANDROID
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOANDROIDSTREAMI2C::Open()
{
  if(!config) return false;

  if(!threadconnection)  return false;
  
  XBUFFER charstr;
  
  (*config->GetLocalDeviceName()).ConvertToASCII(charstr);
  handle = open(charstr.GetPtrChar(), O_RDWR);
  if(handle<0) return false;

  if(ioctl(handle, DIOANDROIDSTREAMI2C_SLAVE, config->GetRemoteAddress()) < 0) return false;

  SetEvent(DIOSTREAMI2C_FSMEVENT_CONNECTED);

  status = DIOSTREAMSTATUS_CONNECTED;

  ResetXBuffers();
  ResetConnectionStatistics();

  return threadconnection->Ini();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOANDROIDSTREAMI2C::WaitToFilledReadingBuffer(int filledto, int timeout)
* @brief      Wait to filled reading buffer
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  filledto : Filledto value.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOANDROIDSTREAMI2C::WaitToFilledReadingBuffer(int filledto, int timeout)
{
  if(filledto == DIOSTREAM_SOMETHINGTOREAD) sizeread = 1; else sizeread = filledto;

  bool status = DIOSTREAM::WaitToFilledReadingBuffer(filledto, timeout);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOANDROIDSTREAMI2C::Close()
* @brief      Close
* @ingroup    PLATFORM_ANDROID
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOANDROIDSTREAMI2C::Close()
{
  if(!threadconnection) return false;

  threadconnection->End();

  if(handle>=0)
    {
      close(handle);
      handle  = -1;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOANDROIDSTREAMI2C::ThreadConnection(void* data)
* @brief      Thread connection
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  data : Data buffer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOANDROIDSTREAMI2C::ThreadConnection(void* data)
{
  DIOANDROIDSTREAMI2C* diostream = (DIOANDROIDSTREAMI2C*)data;
  if(!diostream) return;

  if(diostream->GetEvent()==DIOSTREAMI2C_FSMEVENT_NONE) // No hay nuevos Eventos
    {
      switch(diostream->GetCurrentState())
        {
          case DIOSTREAMI2C_FSMSTATE_NONE                     : break;

          case DIOSTREAMI2C_FSMSTATE_CONNECTED                : break;

          case DIOSTREAMI2C_FSMSTATE_WAITINGTOREAD            : { if(diostream->outbuffer->IsBlocked()) break;

                                                                  if(!diostream->IsBlockWrite())
                                                                    {
                                                                      XDWORD size  = diostream->outbuffer->GetSize();
                                                                      if(size)
                                                                        {
                                                                          diostream->outbuffer->SetBlocked(true);
                                                                          XDWORD bw = diostream->I2C_Write(diostream->config->GetRemoteAddress(), diostream->outbuffer->Get(), size);
                                                                          diostream->outbuffer->SetBlocked(false);

                                                                          if(bw)
                                                                            {
                                                                              diostream->outbuffer->Extract(NULL, 0 , size);
                                                                            }
                                                                        }
                                                                    }


                                                                  if(!diostream->IsBlockRead())
                                                                    {
                                                                      if(diostream->sizeread)
                                                                        {
                                                                          XBYTE* _data = GEN_NEW XBYTE[diostream->sizeread];

                                                                          if(_data)
                                                                            {
                                                                              if(diostream->I2C_Read(diostream->config->GetRemoteAddress(), _data, diostream->sizeread))
                                                                                {
                                                                                  diostream->inbuffer->Add(_data, diostream->sizeread);
                                                                                  //XTRACE_PRINTCOLOR(3, __L("(Addr %04X, size:%d)"), diostream->config->GetRemoteAddress(), diostream->inbuffer->GetSize());
                                                                                  //XTRACE_PRINTDATABLOCKCOLOR(XTRACE_COLOR_BLUE, (*diostream->inbuffer));

                                                                                  diostream->sizeread = 0;
                                                                                }

                                                                              GEN_DELETE_ARRAY _data;
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                                break;


          case DIOSTREAMI2C_FSMSTATE_SENDINGDATA              : break;

          case DIOSTREAMI2C_FSMSTATE_DISCONNECTING            : break;

        }
    }
   else
    {
      if(diostream->GetEvent()<DIOSTREAMI2C_LASTEVENT)
        {
          diostream->CheckTransition();

          switch(diostream->GetCurrentState())
            {
              case DIOSTREAMI2C_FSMSTATE_NONE                 : break;

              case DIOSTREAMI2C_FSMSTATE_CONNECTED            : { DIOSTREAM_XEVENT xevent(diostream, DIOSTREAM_XEVENT_TYPE_CONNECTED);
                                                                  xevent.SetDIOStream(diostream);

                                                                  diostream->PostEvent(&xevent);

                                                                  diostream->SetEvent(DIOSTREAMI2C_FSMEVENT_WAITINGTOREAD);
                                                                }
                                                                break;

              case DIOSTREAMI2C_FSMSTATE_WAITINGTOREAD        : break;

              case DIOSTREAMI2C_FSMSTATE_SENDINGDATA          : break;

              case DIOSTREAMI2C_FSMSTATE_DISCONNECTING        : { DIOSTREAM_XEVENT xevent(diostream, DIOSTREAM_XEVENT_TYPE_DISCONNECTED);
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
* @fn         bool DIOANDROIDSTREAMI2C::I2C_Read(XWORD address, XBYTE* buffer, XWORD size)
* @brief      I2C read
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  address : Address value.
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOANDROIDSTREAMI2C::I2C_Read(XWORD address, XBYTE* buffer, XWORD size)
{
  /*
  int _size = read(handle, buffer, size);

  XTRACE_PRINTCOLOR(3, __L("(Read size: %d)"), _size);

  return false;
  */

  DIOANDROIDSTREAMI2C_RDWR_IOCTL_TICKET   msg_rdwr;
  DIOANDROIDSTREAMI2C_MSG               msg;

  msg.addr        = address;
  msg.flags       = DIOANDROIDSTREAMI2C_MSG_RD;
  msg.buffer      = buffer;
  msg.size        = size;

  msg_rdwr.msgs   = &msg;
  msg_rdwr.nmsgs  = 1;

  //XTRACE_PRINTCOLOR(3, __L("[Addr %04X, size:%d]"), address, size);

  if(ioctl(handle, DIOANDROIDSTREAMI2C_RDWR, &msg_rdwr) < 0) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOANDROIDSTREAMI2C::I2C_Write(XWORD address, XBYTE* buffer, XWORD size)
* @brief      I2C write
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  address : Address value.
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOANDROIDSTREAMI2C::I2C_Write(XWORD address, XBYTE* buffer, XWORD size)
{
  /*
  if(write(handle, buffer, size) != size) return false;

  return true;
  */

  DIOANDROIDSTREAMI2C_RDWR_IOCTL_TICKET   msg_rdwr;
  DIOANDROIDSTREAMI2C_MSG               msg;

  msg.addr        = address;
  msg.flags       = 0;
  msg.buffer      = buffer;
  msg.size        = size;

  msg_rdwr.msgs   = &msg;
  msg_rdwr.nmsgs  = 1;

  if(ioctl(handle, DIOANDROIDSTREAMI2C_RDWR, &msg_rdwr) < 0)
    {
      XTRACE_PRINTCOLOR(4, __L("IOCTL ERROR : %d"), errno);
      return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOANDROIDSTREAMI2C::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOANDROIDSTREAMI2C::Clean()
{
  handle   = -1;
  sizeread = 0;
}




#endif

