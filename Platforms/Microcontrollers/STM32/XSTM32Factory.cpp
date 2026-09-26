/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XSTM32Factory.cpp
*
* @class      XSTM32FACTORY
* @brief      eXtended utils STM32 platform factory class
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

#include "XThreadCollected.h"

#include "XSTM32DateTime.h"
#include "XSTM32Timer.h"
#include "XSTM32Rand.h"
#include "XSTM32File.h"
#include "XSTM32Dir.h"
#include "XSTM32System.h"
#include "XSTM32Thread.h"

#include "XFactoryMacros.h"

#include "XSTM32Factory.h"

#include "XMemory_Control.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/



CREATEFUNC(XSTM32FACTORY  , XTIMER          , XSTM32TIMER            , CreateTimer)
DELETEFUNC(XSTM32FACTORY  , XTIMER          , XSTM32TIMER            , DeleteTimer)

CREATEFUNC(XSTM32FACTORY  , XDATETIME       , XSTM32DATETIME         , CreateDateTime)
DELETEFUNC(XSTM32FACTORY  , XDATETIME       , XSTM32DATETIME         , DeleteDateTime)

CREATEFUNC(XSTM32FACTORY  , XRAND           , XSTM32RAND             , CreateRand)
DELETEFUNC(XSTM32FACTORY  , XRAND           , XSTM32RAND             , DeleteRand)

CREATEFUNC(XSTM32FACTORY  , XFILE           , XSTM32FILE             , Create_File)
DELETEFUNC(XSTM32FACTORY  , XFILE           , XSTM32FILE             , Delete_File)

CREATEFUNC(XSTM32FACTORY  , XDIR            , XSTM32DIR              , Create_Dir)
DELETEFUNC(XSTM32FACTORY  , XDIR            , XSTM32DIR              , Delete_Dir)

CREATEFUNC(XSTM32FACTORY  , XSYSTEM         , XSTM32SYSTEM           , CreateSystem)
DELETEFUNC(XSTM32FACTORY  , XSYSTEM         , XSTM32SYSTEM           , DeleteSystem)

CREATEFUNC(XSTM32FACTORY  , XMUTEX          , XSTM32MUTEX            , Create_Mutex)
DELETEFUNC(XSTM32FACTORY  , XMUTEX          , XSTM32MUTEX            , Delete_Mutex)




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTHREAD* XSTM32FACTORY::CreateThread(XTHREADGROUPID groupID, XCHAR* ID, XTHREADFUNCTION function, void* data)
* @brief      Create thread
* @ingroup    PLATFORM_STM32
* 
* @param[in]  groupID : Group ID value.
* @param[in]  ID : Identifier to use.
* @param[in]  function : Function value.
* @param[in]  data : Data buffer to use.
* 
* @return     XTHREAD* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTHREAD* XSTM32FACTORY::CreateThread(XTHREADGROUPID groupID, XCHAR* ID, XTHREADFUNCTION function, void* data)
{
  XSTM32THREAD* _class =  GEN_NEW XSTM32THREAD(groupID, ID, function, data);
  return (XTHREAD*)_class;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSTM32FACTORY::DeleteThread(XTHREADGROUPID groupID, XTHREAD* xthread)
* @brief      Delete thread
* @ingroup    PLATFORM_STM32
* 
* @param[in]  groupID : Group ID value.
* @param[in]  xthread : Xthread pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSTM32FACTORY::DeleteThread(XTHREADGROUPID groupID, XTHREAD* xthread)
{
  if(!xthread) return false;

  XSTM32THREAD* _xthread = (XSTM32THREAD*)xthread;
  GEN_DELETE _xthread;

  return true;
};



