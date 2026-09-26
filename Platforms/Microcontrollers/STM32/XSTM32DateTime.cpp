/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XSTM32DateTime.cpp
*
* @class      XSTM32DATETIME
* @brief      eXtended Utils STM32 date time class
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

#include "XSTM32DateTime.h"

#include "XMemory_Control.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTM32DATETIME::XSTM32DATETIME()
* @brief      Constructor of class
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTM32DATETIME::XSTM32DATETIME()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTM32DATETIME::~XSTM32DATETIME()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTM32DATETIME::~XSTM32DATETIME()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32DATETIME::Read()
* @brief      Read
* @ingroup    PLATFORM_STM32
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32DATETIME::Read()
{
  //GetActualDateTime(this);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32DATETIME::Write()
* @brief      Write
* @ingroup    PLATFORM_STM32
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32DATETIME::Write()
{
  //SetActualDateTime(this);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XSTM32DATETIME::GetMeridianDifference()
* @brief      Get meridian difference
* @ingroup    PLATFORM_STM32
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XSTM32DATETIME::GetMeridianDifference()
{
  return (int)0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32DATETIME::IsDayLigthSavingTime(int* bias)
* @brief      Is day ligth saving time
* @ingroup    PLATFORM_STM32
* 
* @param[in]  bias : Bias pointer to use.
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32DATETIME::IsDayLigthSavingTime(int* bias)
{
  return false;
}



