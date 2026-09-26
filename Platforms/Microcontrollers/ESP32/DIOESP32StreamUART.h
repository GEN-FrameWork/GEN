/**-------------------------------------------------------------------------------------------------------------------
*
* @file       DIOESP32StreamUART.h
*
* @class      DIOESP32STREAMUART
* @brief      Data Input/Output ESP32 Stream UART class
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

#pragma once


#if defined(DIO_ACTIVE) && defined(DIO_STREAMUART_ACTIVE)

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XBuffer.h"
#include "XFSMachine.h"

#include "XESP32_HAL.h"

#include "DIOStreamUART.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum DIOESP32UARTFSMEVENTS
{
  DIOESP32UARTFSMEVENT_NONE              = 0  ,
  DIOESP32UARTFSMEVENT_CONNECTED              ,
  DIOESP32UARTFSMEVENT_WAITINGTOREAD          ,
  DIOESP32UARTFSMEVENT_DISCONNECTING          ,

  DIOESP32UART_LASTEVENT

};


enum DIOESP32UARTFSMSTATES
{
  DIOESP32UARTFSMSTATE_NONE              = 0  ,

  DIOESP32UARTFSMSTATE_CONNECTED              ,
  DIOESP32UARTFSMSTATE_WAITINGTOREAD          ,
  DIOESP32UARTFSMSTATE_DISCONNECTING          ,

  DIOESP32UART_LASTSTATE
};

#define DIOESP32STREAMIUART_MAXHANDLES   8

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XFACTORY;
class XPUBLISHER;
class DIOFACTORY;
class DIOSTREAMCONFIG;
class XTHREADCOLLECTED;

class DIOESP32STREAMUART : public DIOSTREAMUART, public XFSMACHINE
{
  public:
                                DIOESP32STREAMUART        		();
    virtual                    ~DIOESP32STREAMUART        		();

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

extern DIOESP32STREAMUART*  diostreamuartptrhandle[DIOESP32STREAMIUART_MAXHANDLES];

#endif






