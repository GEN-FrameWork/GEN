/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOSPITFTDisplayST7789.h
* 
* @class      DIOSPITFTDISPLAYST7789
* @brief      Data Input/Output SPI Display TFT ST7789 class
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


#define DIOSPITFTDISPLAYST7789_DC_DATA                   GEN_DIOGPIO.SetValue(GetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID_DC), true);
#define DIOSPITFTDISPLAYST7789_DC_CMD                    GEN_DIOGPIO.SetValue(GetGPIOEntryID(DIODISPLAYDEVICE_INDEX_GPIOENTRYID_DC), false);

#define DIOSPITFTDISPLAYST7789_TFTWIDTH                  240
#define DIOSPITFTDISPLAYST7789_TFTHEIGHT                 240

#define DIOSPITFTDISPLAYST7789_240x240_XSTART            0
#define DIOSPITFTDISPLAYST7789_240x240_YSTART            0

#define DIOSPITFTDISPLAYST7789_CMD_ST7789_NOP            0x00
#define DIOSPITFTDISPLAYST7789_CMD_ST7789_SWRESET        0x01
#define DIOSPITFTDISPLAYST7789_CMD_ST7789_RDDID          0x04
#define DIOSPITFTDISPLAYST7789_CMD_ST7789_RDDST          0x09

#define DIOSPITFTDISPLAYST7789_CMD_ST7789_SLPIN          0x10
#define DIOSPITFTDISPLAYST7789_CMD_ST7789_SLPOUT         0x11
#define DIOSPITFTDISPLAYST7789_CMD_ST7789_PTLON          0x12
#define DIOSPITFTDISPLAYST7789_CMD_ST7789_NORON          0x13

#define DIOSPITFTDISPLAYST7789_CMD_ST7789_INVOFF         0x20
#define DIOSPITFTDISPLAYST7789_CMD_ST7789_INVON          0x21
#define DIOSPITFTDISPLAYST7789_CMD_ST7789_DISPOFF        0x28
#define DIOSPITFTDISPLAYST7789_CMD_ST7789_DISPON         0x29
#define DIOSPITFTDISPLAYST7789_CMD_ST7789_CASET          0x2A
#define DIOSPITFTDISPLAYST7789_CMD_ST7789_RASET          0x2B
#define DIOSPITFTDISPLAYST7789_CMD_ST7789_RAMWR          0x2C
#define DIOSPITFTDISPLAYST7789_CMD_ST7789_RAMRD          0x2E

#define DIOSPITFTDISPLAYST7789_CMD_ST7789_PTLAR          0x30
#define DIOSPITFTDISPLAYST7789_CMD_ST7789_COLMOD         0x3A
#define DIOSPITFTDISPLAYST7789_CMD_ST7789_MADCTL         0x36

#define DIOSPITFTDISPLAYST7789_CMD_ST7789_MADCTL_MY      0x80
#define DIOSPITFTDISPLAYST7789_CMD_ST7789_MADCTL_MX      0x40
#define DIOSPITFTDISPLAYST7789_CMD_ST7789_MADCTL_MV      0x20
#define DIOSPITFTDISPLAYST7789_CMD_ST7789_MADCTL_ML      0x10
#define DIOSPITFTDISPLAYST7789_CMD_ST7789_MADCTL_RGB     0x00

#define DIOSPITFTDISPLAYST7789_CMD_ST7789_RDID1          0xDA
#define DIOSPITFTDISPLAYST7789_CMD_ST7789_RDID2          0xDB
#define DIOSPITFTDISPLAYST7789_CMD_ST7789_RDID3          0xDC
#define DIOSPITFTDISPLAYST7789_CMD_ST7789_RDID4          0xDD

#define DIOSPITFTDISPLAYST7789_COLOR_BLACK               0x0000
#define DIOSPITFTDISPLAYST7789_COLOR_BLUE                0x001F
#define DIOSPITFTDISPLAYST7789_COLOR_RED                 0xF800
#define DIOSPITFTDISPLAYST7789_COLOR_GREEN               0x07E0
#define DIOSPITFTDISPLAYST7789_COLOR_CYAN                0x07FF
#define DIOSPITFTDISPLAYST7789_COLOR_MAGENTA             0xF81F
#define DIOSPITFTDISPLAYST7789_COLOR_YELLOW              0xFFE0
#define DIOSPITFTDISPLAYST7789_COLOR_WHITE               0xFFFF

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOSPITFTDISPLAYST7789 : public DIODISPLAYDEVICE, public DIODEVICESPI
{
  public:
                            DIOSPITFTDISPLAYST7789          (XDWORD width, XDWORD height);
    virtual                ~DIOSPITFTDISPLAYST7789          ();

    bool                    IniDevice                       ();

    bool                    Clear                           (XWORD color = DIOSPITFTDISPLAYST7789_COLOR_BLACK);
    bool                    Update                          (XBYTE* buffer);
    bool                    PutPixel                        (XWORD x, XWORD y, XWORD color);

    bool                    End                             ();

    bool                    SetActiveBlackLight             (bool active);

  private:

    void                    Clean                           ();

    bool                    TFT_Init                        ();
    bool                    TFT_Reset                       ();
    bool                    TFT_SetWindow                   (int xs, int ys, int xe, int ye);
    void                    TFT_SetRotation                 (XBYTE mode);

    bool                    SendCommand                     (XBYTE command);
    bool                    SendCommandParams               (XBYTE command, int ndata, ...);
    bool                    SendData                        ();
    bool                    SendData                        (XBYTE data);
    bool                    SendData                        (XWORD data);

    XBYTE                   _colstart;
    XBYTE                   _rowstart;
    XBYTE                   _xstart;
    XBYTE                   _ystart;
    
    XBYTE                   rotation;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/






