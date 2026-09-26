/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOWINDOWSStreamI2C.cpp
* 
* @class      DIOWINDOWSSTREAMI2C
* @brief      WINDOWS Data Input/Output Stream I2C class
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



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOWINDOWSStreamI2C.h"

#include "XFactory.h"
#include "XBuffer.h"

#include "DIOFactory.h"
#include "DIOStream_XEvent.h"
#include "DIOStreamI2CConfig.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/





/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWINDOWSSTREAMI2C::DIOWINDOWSSTREAMI2C() : DIOSTREAMI2C()
* @brief      Constructor of class
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWINDOWSSTREAMI2C::DIOWINDOWSSTREAMI2C() : DIOSTREAMI2C()
{
  threadconnection = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMI2C, __L("DIOWINDOWSSTREAMI2C::DIOWINDOWSSTREAMI2C"), ThreadConnection, (void*)this);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWINDOWSSTREAMI2C::~DIOWINDOWSSTREAMI2C()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWINDOWSSTREAMI2C::~DIOWINDOWSSTREAMI2C()
{
  if(threadconnection)
    {
      threadconnection->End();
      DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMI2C, threadconnection);
      threadconnection = NULL;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMI2C::Open()
* @brief      Open
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMI2C::Open()
{
  if(!config) return false;

  if(!threadconnection)  return false;

  ResetXBuffers();

  ResetConnectionStatistics();

  return threadconnection->Ini();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMI2C::Close()
* @brief      Close
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMI2C::Close()
{
  if(!threadconnection) return false;

  threadconnection->End();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWINDOWSSTREAMI2C::ThreadConnection(void* data)
* @brief      Thread connection
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  data : Data buffer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWINDOWSSTREAMI2C::ThreadConnection(void* data)
{
  DIOWINDOWSSTREAMI2C* diostream = (DIOWINDOWSSTREAMI2C*)data;
  if(!diostream) return;

  if(diostream->GetEvent()==DIOSTREAMI2C_FSMEVENT_NONE) // No hay nuevos Eventos
    {
      switch(diostream->GetCurrentState())
        {
          case DIOSTREAMI2C_FSMSTATE_NONE                     : break;

          case DIOSTREAMI2C_FSMSTATE_CONNECTED                : break;

          case DIOSTREAMI2C_FSMSTATE_WAITINGTOREAD            : /*
                                                                    { if(diostream->outbuffer->IsBlocked()) break;

                                                                    while(1)
                                                                      {
                                                                        XDWORD size  = diostream->outbuffer->GetSize();

                                                                        if(!size) break;
                                                                        if(size>(XDWORD)DIOWINDOWSSTREAMI2C_MAXSIZEBUFFER) size = DIOWINDOWSSTREAMI2C_MAXSIZEBUFFER;

                                                                        memset(diostream->bufferread  , 0 , DIOWINDOWSSTREAMI2C_MAXSIZEBUFFER);
                                                                        memset(diostream->bufferwrite , 0 , DIOWINDOWSSTREAMI2C_MAXSIZEBUFFER);

                                                                        diostream->outbuffer->Extract(diostream->bufferwrite,0,size);
                                                                        diostream->TransferBuffer(diostream->bufferread,diostream->bufferwrite,size);

                                                                        if(!(diostream->config->IsOnlyWrite())) diostream->inbuffer->Add(diostream->bufferread,size);
                                                                      }
                                                                  }
                                                                  */
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

