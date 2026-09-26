/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XSTM32Sleep.cpp
*
* @class      XSTM32SLEEP
* @brief      eXtended utils STM32 rand class
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


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XSTM32_HAL.h"

#include "XSTM32Sleep.h"

#include "XMemory_Control.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTM32SLEEP::XSTM32SLEEP()
* @brief      Constructor of class
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTM32SLEEP::XSTM32SLEEP()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTM32SLEEP::~XSTM32SLEEP()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTM32SLEEP::~XSTM32SLEEP()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XSTM32SLEEP::Seconds(int seconds)
* @brief      Seconds
* @ingroup    PLATFORM_STM32
* 
* @param[in]  seconds : Seconds value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XSTM32SLEEP::Seconds(int seconds)
{
  for(int c=0; c<seconds; c++)
    {
      HAL_Delay(1000);
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XSTM32SLEEP::MilliSeconds(int milliseconds)
* @brief      Milli seconds
* @ingroup    PLATFORM_STM32
* 
* @param[in]  milliseconds : Milliseconds value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XSTM32SLEEP::MilliSeconds(int milliseconds)
{
  HAL_Delay(milliseconds);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XSTM32SLEEP::MicroSeconds(int microseconds)
* @brief      Micro seconds
* @ingroup    PLATFORM_STM32
* 
* @param[in]  microseconds : Microseconds value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XSTM32SLEEP::MicroSeconds(int microseconds)
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XSTM32SLEEP::NanoSeconds(int nanoseconds)
* @brief      Nano seconds
* @ingroup    PLATFORM_STM32
* 
* @param[in]  nanoseconds : Nanoseconds value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XSTM32SLEEP::NanoSeconds(int nanoseconds)
{

}




