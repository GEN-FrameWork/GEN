/**-------------------------------------------------------------------------------------------------------------------
*
* @file       DIOSTM32LedNeoPixelWS2812B.cpp
*
* @class      DIOSTM32LEDNEOPIXELWS2812B
* @brief      Data Input/Output STM32 Led Neopixel WS2812B
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

#include "XTrace.h"
#include "XSleep.h"
#include "XBuffer.h"

#include "DIOSTM32LedNeoPixelWS2812B.h"

#include "XMemory_Control.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/



/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTM32LEDNEOPIXELWS2812B::DIOSTM32LEDNEOPIXELWS2812B()
* @brief      Constructor of class
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTM32LEDNEOPIXELWS2812B::DIOSTM32LEDNEOPIXELWS2812B()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTM32LEDNEOPIXELWS2812B::~DIOSTM32LEDNEOPIXELWS2812B()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTM32LEDNEOPIXELWS2812B::~DIOSTM32LEDNEOPIXELWS2812B()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTM32LEDNEOPIXELWS2812B::Send()
* @brief      Send
* @ingroup    PLATFORM_STM32
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTM32LEDNEOPIXELWS2812B::Send()
{   
	DIOGPIO_ENTRY* entry = GEN_DIOGPIO.GPIOEntry_GetByID(GPIOentryID);
  if(!entry) return false;
  
  GPIO_TypeDef* stm32_group = NULL;  
  switch(entry->GetGroup())
    {     
      case DIOGPIO_PINSGROUP_NONE : break;
      case DIOGPIO_PINSGROUP_A    : stm32_group = ((GPIO_TypeDef *) GPIOA_BASE);   break;
      case DIOGPIO_PINSGROUP_B    : stm32_group = ((GPIO_TypeDef *) GPIOB_BASE);   break;
      case DIOGPIO_PINSGROUP_C    : stm32_group = ((GPIO_TypeDef *) GPIOC_BASE);   break;
      case DIOGPIO_PINSGROUP_D    : stm32_group = ((GPIO_TypeDef *) GPIOD_BASE);   break; 
      case DIOGPIO_PINSGROUP_E    : stm32_group = ((GPIO_TypeDef *) GPIOE_BASE);   break;
      #if defined(HW_STM32F4XX)
      case DIOGPIO_PINSGROUP_F    : stm32_group = ((GPIO_TypeDef *) GPIOF_BASE);   break; 
      #else
      case DIOGPIO_PINSGROUP_F    : break;
      #endif
    }  
  
  if(!stm32_group) return false; 

  XWORD stm32_pin = (XWORD)entry->GetPin();
  if(!stm32_pin)  return false;
    
  XBYTE* RGB  = databuffer.Get();
  if(!RGB)  return false ;  
  
  
  static XBYTE 	c = 0;
	static XBYTE 	d = 0; 
	static XBYTE 	data;	

	__disable_irq();	
	
	stm32_group->BRR  = stm32_pin;			
	
  bool bit  = false;
	d         = 0;
	c         = 0;
	
	while(d<(3*nled))
		{
			data = RGB[d];
      			
			while(c<8)
				{	
          bit = (data&0x80)?true:false;    
          
					if(bit) 
					  {	
							stm32_group->BSRR = stm32_pin;	
              
							__asm("nop");		
							__asm("nop");																
							__asm("nop");		
							__asm("nop");																
							__asm("nop");		
							__asm("nop");																							
							__asm("nop");							
							__asm("nop");		
							__asm("nop");																
							__asm("nop");		
							__asm("nop");																
							__asm("nop");		
							__asm("nop");																							
							__asm("nop");			
              /*
							__asm("nop");		
							__asm("nop");																
							__asm("nop");		
							__asm("nop");																
							__asm("nop");		
							__asm("nop");																							
							__asm("nop");							
							__asm("nop");		
							__asm("nop");																
							__asm("nop");		
							__asm("nop");																
							__asm("nop");		
							__asm("nop");																							
							__asm("nop");              
              */
						}
					 else
						{							
							stm32_group->BSRR  = stm32_pin;	  
              
							__asm("nop");		
							__asm("nop");																
							__asm("nop");		
							__asm("nop");			
              /*
							__asm("nop");		
							__asm("nop");																
							__asm("nop");		
							__asm("nop");		              
              */
						}	
						
					
          stm32_group->BRR  = stm32_pin;	 
          
						                   	
					data <<= 1;								
					c++;	          
				}
			
			c = 0;
			d++;
		}
	
	stm32_group->BRR  = stm32_pin;

	__enable_irq();		
 
  
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTM32LEDNEOPIXELWS2812B::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTM32LEDNEOPIXELWS2812B::Clean()
{
  
}
