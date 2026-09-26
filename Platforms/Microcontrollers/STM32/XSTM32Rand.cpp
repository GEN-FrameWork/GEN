/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XSTM32Rand.cpp
*
* @class      XSTM32RAND
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
#include "XTrace.h"

#include "XSTM32_HAL.h"
#include "XSTM32Rand.h"

#include "XMemory_Control.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTM32RAND::XSTM32RAND()
* @brief      Constructor of class
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTM32RAND::XSTM32RAND()
{
  Clean();

  Ini();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTM32RAND::~XSTM32RAND()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTM32RAND::~XSTM32RAND()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32RAND::Ini()
* @brief      Initialize the object
* @ingroup    PLATFORM_STM32
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32RAND::Ini()
{  
  const int prime1 = 42;
  const int prime2 = 89;
  
  initialvalue = HAL_GetTick();

  initialvalue += (initialvalue * prime1) + prime2;

  srand(initialvalue);
  
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32RAND::IsCryptographicallySecure()
* @brief      Report whether this platform provider is cryptographically secure
* @ingroup    PLATFORM_STM32
* 
* @return     bool : false until a board-specific HAL hardware RNG provider is supplied.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32RAND::IsCryptographicallySecure()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XSTM32RAND::MaxElements(int max)
* @brief      Max elements
* @ingroup    PLATFORM_STM32
* 
* @param[in]  max : Max value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XSTM32RAND::MaxElements(int max)
{
  return GETRANDOM(0, max-1);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XSTM32RAND::Max(int max)
* @brief      Max
* @ingroup    PLATFORM_STM32
* 
* @param[in]  max : Max value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XSTM32RAND::Max(int max)
{
  return GETRANDOM(0, max);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XSTM32RAND::Between(int min, int max)
* @brief      Between
* @ingroup    PLATFORM_STM32
* 
* @param[in]  min : Min value.
* @param[in]  max : Max value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XSTM32RAND::Between(int min, int max)
{
  return GETRANDOM(min, max);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XSTM32RAND::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XSTM32RAND::Clean()
{
  initialvalue = 0;
}



