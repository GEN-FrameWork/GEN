/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XSTM32_HAL.h
* 
* @class      XSTM32_HAL
* @brief      STM32 HAL includes
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

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#if defined(STM32F072xB)
#define HW_STM32F0XX
#include "stm32f0xx.h"
#include "stm32f0xx_hal.h"
#endif

#if defined(STM32F303xC) || defined(STM32F303xE)
#define HW_STM32F3XX
#include "stm32f3xx.h"
#include "stm32f3xx_hal.h"
#endif

#if defined(STM32F407xx)|| defined(STM32F411xE) || defined(STM32F446xx)
#define HW_STM32F4XX
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#endif

#if defined(STM32WB55xx) || defined (STM32WB5Mxx) || defined(STM32WB35xx)
#define HW_STM32WB55XX
#include "stm32wbxx.h"
#include "stm32wbxx_hal.h"
#endif




/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#ifndef TRUE
#define TRUE                  1
#endif

#ifndef FALSE
#define FALSE                 0
#endif

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/


#ifdef __cplusplus
extern "C"
{
#endif
    
/* ------ Declarations copied from arm\src\lib\<dlib\heap>\dlmalloc.c ----*/
/* ------ MAKE SURE to copy correct/current content from dlmalloc.c ------*/

#define MALLINFO_FIELD_TYPE size_t

#define STRUCT_MALLINFO_DECLARED 1
struct mallinfo {
  MALLINFO_FIELD_TYPE arena;
  MALLINFO_FIELD_TYPE ordblks;
  MALLINFO_FIELD_TYPE smblks;
  MALLINFO_FIELD_TYPE hblks;
  MALLINFO_FIELD_TYPE hblkhd;
  MALLINFO_FIELD_TYPE usmblks;
  MALLINFO_FIELD_TYPE fsmblks;
  MALLINFO_FIELD_TYPE uordblks;
  MALLINFO_FIELD_TYPE fordblks;
  MALLINFO_FIELD_TYPE keepcost;
};
/* ------------------------------------------------------------------------*/

struct mallinfo __iar_dlmallinfo(void);
void __iar_dlmalloc_stats(void);

#ifdef __cplusplus
}
#endif






