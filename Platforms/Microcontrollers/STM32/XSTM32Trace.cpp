/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XSTM32Trace.cpp
*
* @class      XSTM32TRACE
* @brief      eXtended utils STM32 timer class
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

/*---- PRECOMPILATION CONTROL ----------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"


#ifdef XTRACE_ACTIVE

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XBase.h"
#include "XBuffer.h"
#include "XSleep.h"
#include "XTrace.h"

#include "XSTM32DateTime.h"

#include "XSTM32Trace.h"

#include "XMemory_Control.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTM32TRACE::XSTM32TRACE()
* @brief      Constructor of class
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTM32TRACE::XSTM32TRACE()
{
  Clean();

  XTRACE::instance = this;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTM32TRACE::~XSTM32TRACE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTM32TRACE::~XSTM32TRACE()
{
  Clean();

  XTRACE::instance = NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XSTM32TRACE::PrintSpecial(XTRACE_TARGET* target, XBYTE level, XCHAR* string)
* @brief      Print special
* @ingroup    PLATFORM_STM32
* 
* @param[in]  target : target to print the debug
* @param[in]  level : level of debug
* @param[in]  string : string with the line
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XSTM32TRACE::PrintSpecial(XTRACE_TARGET* target, XBYTE level, XCHAR* string)
{
  XSTM32DATETIME   xdatetime;
  static bool      reenter = false;

  if(!huart) return;

  if(reenter)
    {
      XBUFFER         xbufferpackettmp;
      static XBUFFER   xbuffercache;
      XSTM32DATETIME   xdatetime;

      SetTraceTextToXBuffer(0xFFFFFFFF, 0xFFFFFFFF, level, sequence, &xdatetime, string, xbufferpackettmp);      
      
      xbuffercache.Add(xbufferpackettmp);
    }
   else
    {
      reenter = true;

      if(xbuffercache.GetSize())
        {
          HAL_UART_Transmit(huart, (uint8_t*)xbuffercache.Get(), (uint16_t)xbuffercache.GetSize(), 1000);
          xbuffercache.Delete();
        }

      XBUFFER          xbufferpacket;
      XSTM32DATETIME   xdatetime;

      SetTraceTextToXBuffer(0xFFFFFFFF, 0xFFFFFFFF, level, sequence, &xdatetime, string, xbufferpacket);
      HAL_UART_Transmit(huart, (uint8_t*)xbufferpacket.Get(), (uint16_t)xbufferpacket.GetSize(), 1000);

      reenter = false;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         UART_HandleTypeDef* XSTM32TRACE::GetHuart()
* @brief      Get huart
* @ingroup    PLATFORM_STM32
* 
* @return     UART_HandleTypeDef* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
UART_HandleTypeDef* XSTM32TRACE::GetHuart()
{
  return huart;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XSTM32TRACE::SetHuart(UART_HandleTypeDef* huart)
* @brief      Set huart
* @ingroup    PLATFORM_STM32
* 
* @param[in]  huart : Huart pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XSTM32TRACE::SetHuart(UART_HandleTypeDef* huart)
{
  this->huart = huart;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XSTM32TRACE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XSTM32TRACE::Clean()
{
  huart = NULL;
}


#endif
