/**-------------------------------------------------------------------------------------------------------------------
*
* @file       DIOSTM32StreamUART.h
*
* @class      DIOSTM32STREAMUART
* @brief      Data Input/Output STM32 Stream UART class
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

#pragma once


#if defined(DIO_ACTIVE) && defined(DIO_STREAMUART_ACTIVE)

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XBuffer.h"
#include "XFSMachine.h"

#include "XSTM32_HAL.h"

#include "DIOStreamUART.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum DIOSTM32UARTFSMEVENTS
{
  DIOSTM32UARTFSMEVENT_NONE              = 0  ,
  DIOSTM32UARTFSMEVENT_CONNECTED              ,
  DIOSTM32UARTFSMEVENT_WAITINGTOREAD          ,
  DIOSTM32UARTFSMEVENT_DISCONNECTING          ,

  DIOSTM32UART_LASTEVENT

};


enum DIOSTM32UARTFSMSTATES
{
  DIOSTM32UARTFSMSTATE_NONE              = 0  ,

  DIOSTM32UARTFSMSTATE_CONNECTED              ,
  DIOSTM32UARTFSMSTATE_WAITINGTOREAD          ,
  DIOSTM32UARTFSMSTATE_DISCONNECTING          ,

  DIOSTM32UART_LASTSTATE
};

#define DIOSTM32STREAMIUART_MAXHANDLES   8

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XFACTORY;
class XPUBLISHER;
class DIOFACTORY;
class DIOSTREAMCONFIG;
class XTHREADCOLLECTED;

class DIOSTM32STREAMUART : public DIOSTREAMUART, public XFSMACHINE
{
  public:
                                DIOSTM32STREAMUART        		();
    virtual                    ~DIOSTM32STREAMUART        		();

    virtual DIOSTREAMSTATUS     GetStatus              ();

    virtual bool          	    Open                          ();

    virtual bool                Config                        (XWORD mask = DIOSTREAMUARTMASK_ALL);

    virtual XDWORD              ReadDirect                    (XBYTE* buffer, XDWORD size);
    virtual XDWORD              WriteDirect                   (XBYTE* buffer, XDWORD size);

    virtual XDWORD              Write                         (XBYTE* buffer, XDWORD size);

    virtual bool                Disconnect                    ();
    virtual bool                Close                         ();

    virtual bool                GetCTS                        ();
    virtual bool                GetDSR                        ();
    virtual bool                GetRing                       ();
    virtual bool                GetRLSD                       ();

    virtual bool                SetRTS                        (bool on = true);
    virtual bool                SetDTR                        (bool on = true);

    virtual bool            	  CleanBuffers                  ();

    UART_HandleTypeDef*         GetHuart                      ();
    void                        SetHuart                      (UART_HandleTypeDef* huart);


    UART_HandleTypeDef          huart;
    XBYTE                       receiveddata;
    XBUFFER                     readcache[3];

  protected:

    void                        Clean                         ();

    static void                 ThreadConnection              (void* param);

    XTHREADCOLLECTED*           threadconnection;
    int                         indexport;
    XBYTE                       writebuffer[DIOSTREAM_MAXBUFFER];
};


/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/

extern DIOSTM32STREAMUART*  diostreamuartptrhandle[DIOSTM32STREAMIUART_MAXHANDLES];

#endif






