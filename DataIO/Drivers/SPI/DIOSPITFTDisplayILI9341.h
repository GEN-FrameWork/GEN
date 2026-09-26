/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOSPITFTDisplayILI9341.h
* 
* @class      DIOSPITFTDISPLAYILI9341
* @brief      Data Input/Output SPI Display TFT ILI9341 (Ilitek) class
* @ingroup    DATAIO
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

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdarg.h>

#include "DIOGPIO.h"
#include "DIODeviceSPI.h"
#include "DIODisplayDevice.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOSPITFTDISPLAYILI9341_DCHIGH                   GEN_DIOGPIO.SetValue(GetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID_DC), true); 
#define DIOSPITFTDISPLAYILI9341_DCLOW                    GEN_DIOGPIO.SetValue(GetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID_DC), false);

#define DIOSPITFTDISPLAYILI9341_COLOR_BLACK              0x0000
#define DIOSPITFTDISPLAYILI9341_COLOR_BLUE               0x001F
#define DIOSPITFTDISPLAYILI9341_COLOR_RED                0xF800
#define DIOSPITFTDISPLAYILI9341_COLOR_GREEN              0x07E0
#define DIOSPITFTDISPLAYILI9341_COLOR_CYAN               0x07FF
#define DIOSPITFTDISPLAYILI9341_COLOR_MAGENTA            0xF81F
#define DIOSPITFTDISPLAYILI9341_COLOR_YELLOW             0xFFE0
#define DIOSPITFTDISPLAYILI9341_COLOR_WHITE              0xFFFF

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOSPITFTDISPLAYILI9341 : public DIODISPLAYDEVICE, public DIODEVICESPI
{
  public:
                            DIOSPITFTDISPLAYILI9341         ();
    virtual                ~DIOSPITFTDISPLAYILI9341         ();

    bool                    IniDevice                       ();

    bool                    Clear                           (XWORD color = DIOSPITFTDISPLAYILI9341_COLOR_BLACK);
    bool                    Update                          (XBYTE* buffer);
    bool                    PutPixel                        (XWORD x, XWORD y, XWORD color);

    bool                    End                             ();

  private:

    void                    Clean                           ();

    bool                    TFT_Init                        ();
    bool                    TFT_Reset                       ();
    bool                    TFT_RotateRGB                   (int rotate, XBYTE bgr);
    bool                    TFT_SetWindow                   (int xs, int ys, int xe, int ye);

    bool                    SendCommand                     (XBYTE command);
    bool                    SendData                        ();

    bool                    WriteRegister                   (XBYTE reg, int ndata, ...);

    XBUFFER                 databuffer; 
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





