/**-------------------------------------------------------------------------------------------------------------------
*
* @file       DIOSTM32StreamSPI.h
*
* @class      DIOSTM32STREAMSPI
* @brief      Data Input/Output STM32 Stream SPI class
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

#if defined(DIO_ACTIVE) && defined(DIO_STREAMSPI_ACTIVE)

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XBuffer.h"
#include "XFSMachine.h"

#include "XSTM32_HAL.h"

#include "DIOStreamSPI.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOSTM32STREAMSPI_MAXHANDLES   2
#define DIOSTM32STREAMSPI_MAXPORTS     2

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XFACTORY;
class XPUBLISHER;
class DIOFACTORY;
class DIOSTREAMCONFIG;
class XTHREADCOLLECTED;


class DIOSTM32STREAMSPIPORT : public DIOSTREAMSPI
{
  public:
                                            DIOSTM32STREAMSPIPORT               ();
    virtual                                ~DIOSTM32STREAMSPIPORT               ();

    XDWORD                                  GetCounterRef                       ();
    void                                    SetCounterRef                       (XDWORD counterref);

    SPI_HandleTypeDef*                      GetHandleSPI                        ();
    void                                    SetHandleSPI                        (SPI_HandleTypeDef* hspi);

  private:

    void                                    Clean                               ();

    XDWORD                                  counterref;
    SPI_HandleTypeDef*                      hspi;
};

class DIOSTM32STREAMSPI : public DIOSTREAMSPI
{
  public:
                                            DIOSTM32STREAMSPI                   ();
    virtual                                ~DIOSTM32STREAMSPI                   ();

    DIOSTREAMSTATUS                         GetStatus                    ();

    bool                                    Open                                ();

    bool                                    WaitToFilledReadingBuffer           (int filledto = DIOSTREAM_SOMETHINGTOREAD, int timeout = XTIMER_INFINITE);

    XDWORD                                  ReadDirect                          (XBYTE* buffer, XDWORD size);
    XDWORD                                  WriteDirect                         (XBYTE* buffer, XDWORD size);

    XDWORD                                  Write                               (XBYTE* buffer, XDWORD size);

    bool                                    Disconnect                          ();
    bool                                    Close                               ();

    bool                                    CleanBuffers                        ();

    SPI_HandleTypeDef*                      hspi;
    XWORD                                   localaddress;
    XWORD                                   remoteaddress;
    XBYTE                                   bufferdata[DIOSTREAM_MAXBUFFER];
    XWORD                                   sizebufferdata;
    XBUFFER                                 readcache[DIOSTM32STREAMSPI_MAXPORTS];

    static DIOSTM32STREAMSPIPORT*           ports[DIOSTM32STREAMSPI_MAXPORTS];
    static DIOSTM32STREAMSPI*               handles[DIOSTM32STREAMSPI_MAXHANDLES];

    int                                     indexhandle;
    int                                     indexport;

  protected:


    static void                             ThreadConnection                    (void* param);
    void                                    Clean                               ();

    XTHREADCOLLECTED*                       threadconnection;
    XBYTE                                   writebuffer[DIOSTREAM_MAXBUFFER];

};

/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/

#endif







