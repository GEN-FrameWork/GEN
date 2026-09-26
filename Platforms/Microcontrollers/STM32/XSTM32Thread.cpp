/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XSTM32Thread.cpp
*
* @class      XSTM32THREAD
* @brief      eXtended utils STM32 mutex & thread class
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

#include "XSleep.h"
#include "XTrace.h"
#include "XThreadListNonPreemptive.h"
#include "XSTM32_HAL.h"

#include "XSTM32Thread.h"

#include "XMemory_Control.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------------*/
/*  XSTM32MUTEX                                                                                                       */
/*--------------------------------------------------------------------------------------------------------------------*/



/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTM32MUTEX::XSTM32MUTEX()
* @brief      Constructor of class
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTM32MUTEX::XSTM32MUTEX()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTM32MUTEX::~XSTM32MUTEX()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTM32MUTEX::~XSTM32MUTEX()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32MUTEX::Lock()
* @brief      Lock
* @ingroup    PLATFORM_STM32
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32MUTEX::Lock()
{
  counterlock++;
  if(counterlock) islock = true;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32MUTEX::UnLock()
* @brief      Un lock
* @ingroup    PLATFORM_STM32
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32MUTEX::UnLock()
{
  if(!counterlock) return false;

  counterlock--;
  if(!counterlock)  islock = false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XSTM32MUTEX::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XSTM32MUTEX::Clean()
{
  counterlock  = 0;
}



/*--------------------------------------------------------------------------------------------------------------------*/
/*  XSTM32THREAD                                                                                                      */
/*--------------------------------------------------------------------------------------------------------------------*/



/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTM32THREAD::XSTM32THREAD(XTHREADGROUPID groupID, XCHAR* ID, XTHREADFUNCTION function, void* data) : XTHREAD(groupID, ID, function, data)
* @brief      Constructor of class
* @ingroup    PLATFORM_STM32
* 
* @param[in]  groupID : Group ID value.
* @param[in]  ID : Identifier to use.
* @param[in]  function : Function value.
* @param[in]  data : Data buffer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTM32THREAD::XSTM32THREAD(XTHREADGROUPID groupID, XCHAR* ID, XTHREADFUNCTION function, void* data) : XTHREAD(groupID, ID, function, data)
{
  Clean();


}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTM32THREAD::~XSTM32THREAD()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTM32THREAD::~XSTM32THREAD()
{
  End();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32THREAD::Ini(bool run)
* @brief      Initialize the object
* @ingroup    PLATFORM_STM32
* 
* @param[in]  run : Run value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32THREAD::Ini(bool run)
{
  if(run)
    {
      if(!Run(true)) return false;
    }
   else statusfunc = XTHREADSTATUS_STOP;

  return XTHREADLISTNONPREEMPTIVE::GetInstance().AddThread(this);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32THREAD::Wait(int miliseconds)
* @brief      Wait
* @ingroup    PLATFORM_STM32
* 
* @param[in]  miliseconds : Miliseconds value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32THREAD::Wait(int miliseconds)
{
  GEN_XSLEEP.MilliSeconds(miliseconds);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32THREAD::End()
* @brief      End the object
* @ingroup    PLATFORM_STM32
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32THREAD::End()
{
  if(statusfunc == XTHREADSTATUS_END) return false;

  statusfunc = XTHREADSTATUS_EXIT;

  //WaitToEnd();

  return XTHREADLISTNONPREEMPTIVE::GetInstance().DeleteThread(this);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XSTM32THREAD::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_STM32
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XSTM32THREAD::Clean()
{

}

