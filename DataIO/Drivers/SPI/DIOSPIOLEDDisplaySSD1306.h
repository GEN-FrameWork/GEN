/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOSPIOLEDDisplaySSD1306.h
* 
* @class      DIOSPIOLEDDISPLAYSSD1306
* @brief      Data Input/Output SPI Display OLED SSD 1306 class
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

#include "DIOGPIO.h"
#include "DIODeviceSPI.h"
#include "DIODisplayDevice.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum DIOSPIOLEDDISPLAYSSD1306_DISPLAYSIZE
{
  DIOSPIOLEDDISPLAYSSD1306_DISPLAYSIZE_128x32   = 1 ,
  DIOSPIOLEDDISPLAYSSD1306_DISPLAYSIZE_128x64       ,
  DIOSPIOLEDDISPLAYSSD1306_DISPLAYSIZE_132x64       ,
  DIOSPIOLEDDISPLAYSSD1306_DISPLAYSIZE_64x32
};

#define DIOSPIOLEDDISPLAYSSD1306_RW              0
#define DIOSPIOLEDDISPLAYSSD1306_SA0             1
#define DIOSPIOLEDDISPLAYSSD1306_DC              6
#define DIOSPIOLEDDISPLAYSSD1306_CO              7

// Fundamental Command Table
#define DIOSPIOLEDDISPLAYSSD1306_COMMAND_DISPLAY_ON                          0xAF
#define DIOSPIOLEDDISPLAYSSD1306_COMMAND_DISPLAY_OFF                         0xAE
#define DIOSPIOLEDDISPLAYSSD1306_COMMAND_CONSTRAST_SET                       0x81
#define DIOSPIOLEDDISPLAYSSD1306_COMMAND_DISPLAY_ALL_ON_RESUME               0xA4
#define DIOSPIOLEDDISPLAYSSD1306_COMMAND_DISPLAY_ALL_ON                      0xA5
#define DIOSPIOLEDDISPLAYSSD1306_COMMAND_DISPLAY_NORMAL                      0xA6
#define DIOSPIOLEDDISPLAYSSD1306_COMMAND_DISPLAY_INVERSE                     0xA7

// Scrolling Command Table
#define DIOSPIOLEDDISPLAYSSD1306_COMMAND_SCROLL_ACTIVATE                     0x2F
#define DIOSPIOLEDDISPLAYSSD1306_COMMAND_SCROLL_DEACTIVATE                   0x2E
#define DIOSPIOLEDDISPLAYSSD1306_COMMAND_SCROLL_LEFT                         0x27
#define DIOSPIOLEDDISPLAYSSD1306_COMMAND_SCROLL_RIGHT                        0x26
#define DIOSPIOLEDDISPLAYSSD1306_COMMAND_SCROLL_VERTICAL_LEFT                0x2A
#define DIOSPIOLEDDISPLAYSSD1306_COMMAND_SCROLL_VERTICAL_RIGHT               0x29
#define DIOSPIOLEDDISPLAYSSD1306_COMMAND_SCROLL_VERTICAL_AREA_SET            0xA3

// Addressing Setting Command Table
#define DIOSPIOLEDDISPLAYSSD1306_COMMAND_PAGE_ADDRESSING_COLUMN_LOWER_SET    0x00
#define DIOSPIOLEDDISPLAYSSD1306_COMMAND_PAGE_ADDRESSING_COLUMN_HIGHER_SET   0x10
#define DIOSPIOLEDDISPLAYSSD1306_COMMAND_PAGE_ADDRESSING_PAGE_START_SET      0xB0
#define DIOSPIOLEDDISPLAYSSD1306_COMMAND_PAGE_ADDRESS_SET                    0x22
#define DIOSPIOLEDDISPLAYSSD1306_COMMAND_MEMORY_ADDRESSING_SET               0x20
#define DIOSPIOLEDDISPLAYSSD1306_COMMAND_COLUMN_ADDRESS_SET                  0x21

// Hardware Configuration
#define DIOSPIOLEDDISPLAYSSD1306_COMMAND_DISPLAY_START_LINE_SET              0x40
#define DIOSPIOLEDDISPLAYSSD1306_COMMAND_DISPLAY_OFFSET_SET                  0xD3
#define DIOSPIOLEDDISPLAYSSD1306_COMMAND_SEGMENT_REMAP_SET                   0xA0
#define DIOSPIOLEDDISPLAYSSD1306_COMMAND_MULTIPLEX_RADIO_SET                 0xA8
#define DIOSPIOLEDDISPLAYSSD1306_COMMAND_COM_OUTPUT_SCAN_INC                 0xC0
#define DIOSPIOLEDDISPLAYSSD1306_COMMAND_COM_OUTPUT_SCAN_DEC                 0xC8
#define DIOSPIOLEDDISPLAYSSD1306_COMMAND_COM_PINS_SET                        0xDA

// Timing and Driving Scheme Setting Command Table
#define DIOSPIOLEDDISPLAYSSD1306_COMMAND_DISPLAY_CLOCK_DIV_RATIO_SET         0xD5
#define DIOSPIOLEDDISPLAYSSD1306_COMMAND_DISPLAY_OSCILLATOR_FREQUENCY_SET    0xD5
#define DIOSPIOLEDDISPLAYSSD1306_COMMAND_PRECHARGE_PERIOD_SET                0xD9
#define DIOSPIOLEDDISPLAYSSD1306_COMMAND_VCOMH_DESELECT_LEVEL_SET            0xDB
#define DIOSPIOLEDDISPLAYSSD1306_COMMAND_NOP                                 0xE3

// Charge Pump Command Table
#define DIOSPIOLEDDISPLAYSSD1306_COMMAND_CHARGE_PUMP_SET                     0x8D

#define DIOSPIOLEDDISPLAYSSD1306_DISPLAY_LINE_HEIGHT                         8
#define DIOSPIOLEDDISPLAYSSD1306_DISPLAY_LINES                               (width / DIOSPIOLEDDISPLAYSSD1306_DISPLAY_LINE_HEIGHT)

#define DIOSPIOLEDDISPLAYSSD1306_POINTER(x, y)                               (x + ((y / DIOSPIOLEDDISPLAYSSD1306_DISPLAY_LINE_HEIGHT) * width))

#define DIOSPIOLEDDISPLAYSSD1306_BITSET(x, y)                                (x |=  (1UL<<y))
#define DIOSPIOLEDDISPLAYSSD1306_BITCLEAR(x, y)                              (x &= (~(1UL<<y)))
#define DIOSPIOLEDDISPLAYSSD1306_BITTOGGLE(x, y)                             (x ^=  (1UL<<y))
#define DIOSPIOLEDDISPLAYSSD1306_BITCHECK(x, y)                              (x & (1UL<<y) ? 1 : 0)

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOSPIOLEDDISPLAYSSD1306 : public DIODISPLAYDEVICE, public DIODEVICESPI
{
  public:

                              DIOSPIOLEDDISPLAYSSD1306      (XDWORD width, XDWORD height);
    virtual                  ~DIOSPIOLEDDISPLAYSSD1306      ();

    bool                      IniDevice                     ();

    bool                      Clear                         (XBYTE color = 0);
    bool                      Update                        (XBYTE* buffer);
    bool                      PutPixel                      (XWORD x, XWORD y, bool color = true);

    bool                      End                           ();


  private:

    bool                      Initializate                  ();

    bool                      Reset                         ();

    bool                      Activate                      (bool active);
    void                      SetContrast                   (XBYTE contrast);

    XBYTE                     GetX                          ();
    XBYTE                     GetY                          ();

    bool                      SetX                          (XBYTE x);
    bool                      SetY                          (XBYTE y);
    bool                      SetXY                         (XBYTE x, XBYTE y);

    bool                      SendCommand                   (XBYTE command);
    bool                      SendData                      (XBYTE* data, int size);
    bool                      Send                          (XBYTE* data, int sizedata);

    void                      Clean                         ();

    XBYTE                     x;
    XBYTE                     y;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/






