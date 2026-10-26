/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOLINUXLedNeoPixelWS2812BRPi.cpp
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOLINUXLedNeoPixelWS2812BRPi.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>
#include <stdarg.h>
#include <getopt.h>

#include "clk.h"
#include "gpio.h"
#include "dma.h"
#include "pwm.h"
#include "version.h"

#include "XTrace.h"
#include "XSleep.h"
#include "XBuffer.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

//#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/




/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOLINUXLEDNEOPIXELWS2812BRPI::DIOLINUXLEDNEOPIXELWS2812BRPI()
* @brief      Constructor of class
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOLINUXLEDNEOPIXELWS2812BRPI::DIOLINUXLEDNEOPIXELWS2812BRPI()
{
  Clean();

  //XLINUXIDRPI IDpi;  
  //model = IDpi.GetModel();

  memset(&config, 0, sizeof(ws2811_t));

  config.freq                  = NEOPIXELWS2812BRPI_DEFAULT_TARGET_FREQ;
  config.dmanum                = NEOPIXELWS2812BRPI_DEFAULT_DMA;
  
  config.channel[0].gpionum    = NEOPIXELWS2812BRPI_DEFAULT_GPIO_PIN;
  config.channel[0].count      = NEOPIXELWS2812BRPI_DEFAULT_LED_COUNT;
  config.channel[0].invert     = 0;
  config.channel[0].brightness = brightnesslevel;
  config.channel[0].strip_type = NEOPIXELWS2812BRPI_DEFAULT_STRIP_TYPE;

  // RPI_PWM_CHANNELS is 1 on the Pi 5 / CM5 build. channel[1] is past the end of ws2811_t.
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOLINUXLEDNEOPIXELWS2812BRPI::~DIOLINUXLEDNEOPIXELWS2812BRPI()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOLINUXLEDNEOPIXELWS2812BRPI::~DIOLINUXLEDNEOPIXELWS2812BRPI()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXLEDNEOPIXELWS2812BRPI::Ini(XDWORD nled)
* @brief      Initialize the object
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  nled : Nled value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXLEDNEOPIXELWS2812BRPI::Ini(XDWORD nled)
{  
  config.channel[0].count = nled;

  databuffer.Resize(nled * 3);

  ws2811_return_t returnvalue = ws2811_init(&config);
  if(returnvalue != WS2811_SUCCESS) 
    {
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, NL("[NeoPixel] ws2811_init: %s"), ws2811_get_return_t_str(returnvalue));
      return false;
    }

  initialized = true;

  return true;  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXLEDNEOPIXELWS2812BRPI::Send()
* @brief      Send
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXLEDNEOPIXELWS2812BRPI::Send()
{
  if(!initialized || !config.device || !config.channel[0].leds)
    {
      return false;
    }

  XBYTE* ptrdata  = databuffer.Get();
  XDWORD data     = 0;
  int    index    = 0;

  for(int c=0; c<config.channel[0].count; c++)
    {
      config.channel[0].leds[c] = 0;

      data = ptrdata[index];      
      config.channel[0].leds[c] |= data;           
      index++;

      data = ptrdata[index];      
      data <<= 8;
      config.channel[0].leds[c] |= data;           
      index++;

      data = ptrdata[index];      
      data <<= 16;
      config.channel[0].leds[c] |= data;           
      index++;

      data = 0xFF;      
      data <<= 24;
      config.channel[0].leds[c] |= data;                
    }

  config.channel[0].brightness = brightnesslevel;

  ws2811_return_t returnvalue = ws2811_render(&config);
  if(returnvalue != WS2811_SUCCESS) 
    { 
      return false;
    }

  return true;
   
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXLEDNEOPIXELWS2812BRPI::End()
* @brief      End the object
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXLEDNEOPIXELWS2812BRPI::End()
{  
  if(!initialized || !config.device)
    {
      return false;
    }

  //databuffer.FillBuffer();
  SetBrightnessLevel(0);
  Send();

  ws2811_fini(&config);
  initialized = false;
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOLINUXLEDNEOPIXELWS2812BRPI::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOLINUXLEDNEOPIXELWS2812BRPI::Clean()
{
  initialized = false;
}



