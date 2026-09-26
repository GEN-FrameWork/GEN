/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XSTM32EEpromMemoryManager.cpp
*
* @class      XSTM32EEPROMMEMORYMANAGER
* @brief      eXtended utils STM32 Flash Memory class
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
#include <string.h>

#include "XSTM32EEPROMMemoryManager.h"

#include "XMemory_Control.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTM32EEPROMMEMORYMANAGER::XSTM32EEPROMMEMORYMANAGER()
* @brief      Constructor of class
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTM32EEPROMMEMORYMANAGER::XSTM32EEPROMMEMORYMANAGER()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTM32EEPROMMEMORYMANAGER::~XSTM32EEPROMMEMORYMANAGER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTM32EEPROMMEMORYMANAGER::~XSTM32EEPROMMEMORYMANAGER()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32EEPROMMEMORYMANAGER::Ini()
* @brief      Initialize the object
* @ingroup    PLATFORM_STM32
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32EEPROMMEMORYMANAGER::Ini()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32EEPROMMEMORYMANAGER::End()
* @brief      End the object
* @ingroup    PLATFORM_STM32
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32EEPROMMEMORYMANAGER::End()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32EEPROMMEMORYMANAGER::Read(XDWORD offset, XBYTE* data, XDWORD size)
* @brief      Read
* @ingroup    PLATFORM_STM32
* 
* @param[in]  offset : Offset value.
* @param[in]  data : Data buffer to use.
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32EEPROMMEMORYMANAGER::Read(XDWORD offset, XBYTE* data, XDWORD size)
{
  if((offset + size) > XSTM32EEPROMMEMORYMANAGER_MAXSIZE) return false;

  #if defined(STM32F407xx) || defined(STM32F446xx)

  for(int c=0; c<size; c++)
    {
      (*(data+c)) = (*(__IO uint8_t *)(XSTM32EEPROMMEMORYMANAGER_BASE+offset+c));
    }

  #endif

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32EEPROMMEMORYMANAGER::Write(XDWORD offset, XBYTE* data, XDWORD size)
* @brief      Write
* @ingroup    PLATFORM_STM32
* 
* @param[in]  offset : Offset value.
* @param[in]  data : Data buffer to use.
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32EEPROMMEMORYMANAGER::Write(XDWORD offset, XBYTE* data, XDWORD size)
{
  if((offset + size) > XSTM32EEPROMMEMORYMANAGER_MAXSIZE) return false;

  #if defined(STM32F407xx) || defined(STM32F446xx)

  for(int c=0; c<size; c++)
    {
      (*(__IO uint8_t *)(XSTM32EEPROMMEMORYMANAGER_BASE+offset+c)) = (*(data+c));
    }

  #endif

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32EEPROMMEMORYMANAGER::EraseAll()
* @brief      Erase all
* @ingroup    PLATFORM_STM32
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32EEPROMMEMORYMANAGER::EraseAll()
{
  #if defined(STM32F407xx) || defined(STM32F446xx)

  for(int c=0; c<XSTM32EEPROMMEMORYMANAGER_MAXSIZE; c++)
    {
      (*(__IO uint8_t *)(XSTM32EEPROMMEMORYMANAGER_BASE+c))= 0;
    }

  #endif

  return true;
}






