/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XSTM32System.cpp
*
* @class      XSTM32SYSTEM
* @brief      eXtended utils STM32 system class
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

#include <stdio.h>

#include "XString.h"

#include "XSTM32_HAL.h"

#include "XSTM32System.h"

#include "XMemory_Control.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/



/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTM32SYSTEM::XSTM32SYSTEM() : XSYSTEM()
* @brief      Constructor of class
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTM32SYSTEM::XSTM32SYSTEM() : XSYSTEM()
{
  Clean();
}



/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTM32SYSTEM::~XSTM32SYSTEM()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTM32SYSTEM::~XSTM32SYSTEM()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSYSTEM_HARDWARETYPE XSTM32SYSTEM::GetTypeHardware(int* revision)
* @brief      Get type hardware
* @ingroup    PLATFORM_STM32
* 
* @param[in]  revision : Revision pointer to use.
* 
* @return     XSYSTEM_HARDWARETYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSYSTEM_HARDWARETYPE XSTM32SYSTEM::GetTypeHardware(int* revision)
{
  return XSYSTEM_HARDWARETYPE_MICRO_STM32;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSYSTEM_PLATFORM XSTM32SYSTEM::GetPlatform(XSTRING* namestring)
* @brief      Get platform
* @ingroup    PLATFORM_STM32
* 
* @param[in]  namestring : Namestring pointer to use.
* 
* @return     XSYSTEM_PLATFORM : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSYSTEM_PLATFORM XSTM32SYSTEM::GetPlatform(XSTRING* namestring)
{
  if(namestring)  namestring->Set(__L("STM32"));
    
  #if defined(STM32F072xB)
  if(namestring)  namestring->Set(__L("STM32F072xB"));
  #endif

  #if defined(STM32F303xC) 
  if(namestring)  namestring->Set(__L("STM32F303xC"));
  #endif
  
  #if defined(STM32F303xE)
  if(namestring)  namestring->Set(__L("STM32F303xE"));
  #endif

  #if defined(STM32F407xx) 
  if(namestring)  namestring->Set(__L("STM32F407xx"));
  #endif
  
  #if defined(STM32F411xE)  
  if(namestring)  namestring->Set(__L("STM32F411xE"));  
  #endif
      
  #if defined(STM32F446xx)
  if(namestring)  namestring->Set(__L("STM32F446xx"));
  #endif

  #if defined(STM32WB55xx)
  if(namestring)  namestring->Set(__L("STM32WB55xx"));
  #endif
  
  #if defined(STM32WB5Mxx)
  if(namestring)  namestring->Set(__L("STM32WB5Mxx"));  
  #endif
    
  #if defined(STM32WB35xx)
  if(namestring)  namestring->Set(__L("STM32WB35xx"));
  #endif

  return XSYSTEM_PLATFORM_STM32;  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32SYSTEM::GetMemoryInfo(XDWORD& total, XDWORD& free)
* @brief      Get memory info
* @ingroup    PLATFORM_STM32
* 
* @param[in]  total : Total value.
* @param[in]  free : Free value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32SYSTEM::GetMemoryInfo(XDWORD& total, XDWORD& free)
{
  total = 0;
  free  = 0;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32SYSTEM::ShutDown(XSYSTEM_CHANGESTATUSTYPE type)
* @brief      Shut down
* @ingroup    PLATFORM_STM32
* 
* @param[in]  type : Type value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32SYSTEM::ShutDown(XSYSTEM_CHANGESTATUSTYPE type)
{
  switch(type)
    {
      case XSYSTEM_CHANGESTATUSTYPE_REBOOT              :
                                                          #if defined(STM32F407xx) || defined(STM32F411xE) || defined(STM32F446x)
                                                          { __DSB();                                                                                                                     // Ensure all outstanding memory accesses included buffered write are completed before reset
                                                            SCB->AIRCR  = ((0x5FA << SCB_AIRCR_VECTKEY_Pos) | (SCB->AIRCR & SCB_AIRCR_PRIGROUP_Msk) | SCB_AIRCR_SYSRESETREQ_Msk);        // Keep priority group unchanged
                                                            __DSB();                                                                                                                     // Ensure completion of memory access
                                                            while(1);                                                                                                                    // Wait until reset
                                                          }
                                                          #endif
                                                          break;

      case XSYSTEM_CHANGESTATUSTYPE_SESSION_LOGOFF    :  break;
      case XSYSTEM_CHANGESTATUSTYPE_POWEROFF          :  break;
                                              default :  break;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XSTM32SYSTEM::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XSTM32SYSTEM::Clean()
{

}

