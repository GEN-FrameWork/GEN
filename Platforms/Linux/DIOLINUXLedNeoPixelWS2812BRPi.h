/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOLINUXLedNeoPixelWS2812BRPi.h
* 
* @class      DIOLINUXLEDNEOPIXELWS2812BRPI
* @brief      LINUX Data Input/Output Led Neopixel WS2812B Raspberry Pi class
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

//#include "XLINUXIDRPi.h"

#include "ws2811.h"

#include "DIOLedNeoPixelWS2812B.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

// defaults 
#define NEOPIXELWS2812BRPI_DEFAULT_TARGET_FREQ             WS2811_TARGET_FREQ
#define NEOPIXELWS2812BRPI_DEFAULT_GPIO_PIN                18
#define NEOPIXELWS2812BRPI_DEFAULT_DMA                     10
//#define NEOPIXELWS2812BRPI_DEFAULT_STRIP_TYPE            WS2811_STRIP_RGB		// WS2812/SK6812RGB integrated chip+leds
#define NEOPIXELWS2812BRPI_DEFAULT_STRIP_TYPE              WS2811_STRIP_GBR		// WS2812/SK6812RGB integrated chip+leds
//#define NEOPIXELWS2812BRPI_DEFAULT_STRIP_TYPE            SK6812_STRIP_RGBW	// SK6812RGBW (NOT SK6812RGB)

#define NEOPIXELWS2812BRPI_DEFAULT_WIDTH                   8
#define NEOPIXELWS2812BRPI_DEFAULT_HEIGHT                  8
#define NEOPIXELWS2812BRPI_DEFAULT_LED_COUNT               (NEOPIXELWS2812BRPI_DEFAULT_WIDTH * NEOPIXELWS2812BRPI_DEFAULT_HEIGHT)



/*---- CLASS ---------------------------------------------------------------------------------------------------------*/


class DIOLINUXLEDNEOPIXELWS2812BRPI : public DIOLEDNEOPIXELWS2812B
{
  public:
                              DIOLINUXLEDNEOPIXELWS2812BRPI     ();
    virtual                  ~DIOLINUXLEDNEOPIXELWS2812BRPI     ();

    virtual bool              Ini                               (XDWORD nled);   
    virtual bool              Send                              ();    
    virtual bool              End                               ();
    
  private:

    void                      Clean                             ();
   
    ws2811_t                  config;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





