/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOSPIOLEDDisplaySSD1331.h
* 
* @class      DIOSPIOLEDDISPLAYSSD1331
* @brief      Data Input/Output SPI Display OLED SSD 1331 class
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

#include "DIODeviceSPI.h"
#include "DIODisplayDevice.h"
#include "DIOGPIO.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


#define DIOSPIOLEDDISPLAYSSD1331_CMD_FILLRECTDISABLE               0x00
#define DIOSPIOLEDDISPLAYSSD1331_CMD_FILLRECTENABLE                0x01

#define DIOSPIOLEDDISPLAYSSD1331_CMD_HSCROLLDISABLE                0x00
#define DIOSPIOLEDDISPLAYSSD1331_CMD_HSCROLLENABLE                 0x01

#define DIOSPIOLEDDISPLAYSSD1331_CMD_VSCROLLDISABLE                0x00
#define DIOSPIOLEDDISPLAYSSD1331_CMD_VSCROLLENABLE                 0x01

#define DIOSPIOLEDDISPLAYSSD1331_CMD_LOCKMODEDISABLE               0x12
#define DIOSPIOLEDDISPLAYSSD1331_CMD_LOCKMODEENABLE                0x16

#define DIOSPIOLEDDISPLAYSSD1331_CMD_SETCOLUMNADDRESS              0x15
#define DIOSPIOLEDDISPLAYSSD1331_CMD_SETROWADDRESS                 0x75

#define DIOSPIOLEDDISPLAYSSD1331_CMD_DRAWLINE                      0x21
#define DIOSPIOLEDDISPLAYSSD1331_CMD_DRAWRECT                      0x22
#define DIOSPIOLEDDISPLAYSSD1331_CMD_CLEARWINDOW                   0x25
#define DIOSPIOLEDDISPLAYSSD1331_CMD_FILLRECT                      0x26

#define DIOSPIOLEDDISPLAYSSD1331_CMD_CONTINUOUSSCROLLINGSETUP      0x27
#define DIOSPIOLEDDISPLAYSSD1331_CMD_DEACTIVESCROLLING             0x2E
#define DIOSPIOLEDDISPLAYSSD1331_CMD_ACTIVESCROLLING               0x2F

#define DIOSPIOLEDDISPLAYSSD1331_CMD_SETCONTRASTA                  0x81
#define DIOSPIOLEDDISPLAYSSD1331_CMD_SETCONTRASTB                  0x82
#define DIOSPIOLEDDISPLAYSSD1331_CMD_SETCONTRASTC                  0x83

#define DIOSPIOLEDDISPLAYSSD1331_CMD_MASTERCURRENTCONTROL          0x87

#define DIOSPIOLEDDISPLAYSSD1331_CMD_SETPRECHARGESPEEDA            0x8A
#define DIOSPIOLEDDISPLAYSSD1331_CMD_SETPRECHARGESPEEDB            0x8B
#define DIOSPIOLEDDISPLAYSSD1331_CMD_SETPRECHARGESPEEDC            0x8C

#define DIOSPIOLEDDISPLAYSSD1331_CMD_SETREMAP                      0xA0
#define DIOSPIOLEDDISPLAYSSD1331_CMD_SETDISPLAYSTARTLINE           0xA1
#define DIOSPIOLEDDISPLAYSSD1331_CMD_SETDISPLAYOFFSET              0xA2
#define DIOSPIOLEDDISPLAYSSD1331_CMD_NORMALDISPLAY                 0xA4
#define DIOSPIOLEDDISPLAYSSD1331_CMD_ENTIREDISPLAY_ON              0xA5
#define DIOSPIOLEDDISPLAYSSD1331_CMD_ENTIREDISPLAY_OFF             0xA6
#define DIOSPIOLEDDISPLAYSSD1331_CMD_INVERSEDISPLAY                0xA7
#define DIOSPIOLEDDISPLAYSSD1331_CMD_SETMULTIPLEXRATIO             0xA8
#define DIOSPIOLEDDISPLAYSSD1331_CMD_DISPLAYONDIM                  0xAC
#define DIOSPIOLEDDISPLAYSSD1331_CMD_SETMASTERCONFIGURE            0xAD
#define DIOSPIOLEDDISPLAYSSD1331_CMD_DISPLAYOFF                    0xAE
#define DIOSPIOLEDDISPLAYSSD1331_CMD_DISPLAYON                     0xAF
#define DIOSPIOLEDDISPLAYSSD1331_CMD_POWERSAVEMODE                 0xB0
#define DIOSPIOLEDDISPLAYSSD1331_CMD_PHASE12PERIOD                 0xB1
#define DIOSPIOLEDDISPLAYSSD1331_CMD_CLOCKDIVIDER                  0xB3
#define DIOSPIOLEDDISPLAYSSD1331_CMD_SETPRECHARGEVOLTAGE           0xBB
#define DIOSPIOLEDDISPLAYSSD1331_CMD_SETVVOLTAGE                   0xBE

#define DIOSPIOLEDDISPLAYSSD1331_CMD_LOCKMODE                      0xFD

#define DIOSPIOLEDDISPLAYSSD1331_COLOR_BLACK                       0x0000      //   0,   0,   0
#define DIOSPIOLEDDISPLAYSSD1331_COLOR_LIGHTGREY                   0xC618      // 192, 192, 192
#define DIOSPIOLEDDISPLAYSSD1331_COLOR_DARKGREY                    0x7BEF      // 128, 128, 128
#define DIOSPIOLEDDISPLAYSSD1331_COLOR_RED                         0xF800      // 255,   0,   0
#define DIOSPIOLEDDISPLAYSSD1331_COLOR_GREEN                       0x07E0      //   0, 255,   0
#define DIOSPIOLEDDISPLAYSSD1331_COLOR_CYAN                        0x07FF      //   0, 255, 255
#define DIOSPIOLEDDISPLAYSSD1331_COLOR_BLUE                        0x001F      //   0,   0, 255
#define DIOSPIOLEDDISPLAYSSD1331_COLOR_MAGENTA                     0xF81F      // 255,   0, 255
#define DIOSPIOLEDDISPLAYSSD1331_COLOR_YELLOW                      0xFFE0      // 255, 255,   0
#define DIOSPIOLEDDISPLAYSSD1331_COLOR_WHITE                       0xFFFF      // 255, 255, 255

#define DIOSPIOLEDDISPLAYSSD1331_DEFAULTTIMEOUT                    3

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOSPIOLEDDISPLAYSSD1331 : public DIODISPLAYDEVICE, public DIODEVICESPI
{
  public:
                            DIOSPIOLEDDISPLAYSSD1331        ();
    virtual                ~DIOSPIOLEDDISPLAYSSD1331        ();

    bool                    IniDevice                       ();

    bool                    Clear                           (XWORD color = DIOSPIOLEDDISPLAYSSD1331_COLOR_BLACK);
    bool                    Update                          (XBYTE* buffer);
    bool                    PutPixel                        (XWORD x, XWORD y, XWORD color);

    bool                    End                             ();

  private:
    
    bool                    Initializate                    ();

    bool                    Reset                           ();
  
    bool                    Active                          (bool on);
    bool                    EnableFillMode                  (bool on);
    bool                    EnableLockMode                  (bool on);
    bool                    EnableScrollMode                (bool on);
    bool                    SetScrollMode                   (XWORD horizontal, XWORD vertical);
    bool                    ClearDisplay                    ();
    bool                    MaxWindow                       ();
    bool                    FillRect                        (XBYTE x0, XBYTE y0, XBYTE x1, XBYTE y1, XWORD color, XWORD background = DIOSPIOLEDDISPLAYSSD1331_COLOR_BLACK);

    bool                    SendCommand                     (XBYTE command, int timeout = DIOSPIOLEDDISPLAYSSD1331_DEFAULTTIMEOUT);
    bool                    SendCommand                     (XBYTE* command, XDWORD size, int timeout = DIOSPIOLEDDISPLAYSSD1331_DEFAULTTIMEOUT);

    bool                    SendData                        (XBYTE data, int timeout = DIOSPIOLEDDISPLAYSSD1331_DEFAULTTIMEOUT);
    bool                    SendData                        (XBYTE* data, XDWORD size, int timeout = DIOSPIOLEDDISPLAYSSD1331_DEFAULTTIMEOUT);

    void                    Clean                           ();  
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





