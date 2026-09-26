/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XESP32Factory.cpp
* 
* @class      XESP32FACTORY
* @brief      eXtended utils ESP32 platform factory class
* @ingroup    PLATFORM_ESP32
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XESP32Factory.h"

#include "XThreadCollected.h"

#include "XESP32DateTime.h"
#include "XESP32Timer.h"
#include "XESP32Rand.h"
#include "XESP32File.h"
#include "XESP32Dir.h"
#include "XESP32System.h"
#include "XESP32Thread.h"

#include "XFactoryMacros.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/




/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


CREATEFUNC(XESP32FACTORY  , XTIMER          , XESP32TIMER            , CreateTimer)
DELETEFUNC(XESP32FACTORY  , XTIMER          , XESP32TIMER            , DeleteTimer)

CREATEFUNC(XESP32FACTORY  , XDATETIME       , XESP32DATETIME         , CreateDateTime)
DELETEFUNC(XESP32FACTORY  , XDATETIME       , XESP32DATETIME         , DeleteDateTime)

CREATEFUNC(XESP32FACTORY  , XRAND           , XESP32RAND             , CreateRand)
DELETEFUNC(XESP32FACTORY  , XRAND           , XESP32RAND             , DeleteRand)

CREATEFUNC(XESP32FACTORY  , XFILE           , XESP32FILE             , Create_File)
DELETEFUNC(XESP32FACTORY  , XFILE           , XESP32FILE             , Delete_File)

CREATEFUNC(XESP32FACTORY  , XDIR            , XESP32DIR              , Create_Dir)
DELETEFUNC(XESP32FACTORY  , XDIR            , XESP32DIR              , Delete_Dir)

CREATEFUNC(XESP32FACTORY  , XSYSTEM         , XESP32SYSTEM           , CreateSystem)
DELETEFUNC(XESP32FACTORY  , XSYSTEM         , XESP32SYSTEM           , DeleteSystem)

CREATEFUNC(XESP32FACTORY  , XMUTEX          , XESP32MUTEX            , Create_Mutex)
DELETEFUNC(XESP32FACTORY  , XMUTEX          , XESP32MUTEX            , Delete_Mutex)


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTHREAD* XESP32FACTORY::CreateThread(XTHREADGROUPID groupID, XCHAR* ID, XTHREADFUNCTION function, void* data)
* @brief      Create thread
* @ingroup    PLATFORM_ESP32
* 
* @param[in]  groupID : Group ID value.
* @param[in]  ID : Identifier to use.
* @param[in]  function : Function value.
* @param[in]  data : Data buffer to use.
* 
* @return     XTHREAD* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTHREAD* XESP32FACTORY::CreateThread(XTHREADGROUPID groupID, XCHAR* ID, XTHREADFUNCTION function, void* data)
{
  XESP32THREAD* _class =  GEN_NEW XESP32THREAD(groupID, ID, function, data);
  return (XTHREAD*)_class;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XESP32FACTORY::DeleteThread(XTHREADGROUPID groupID, XTHREAD* xthread)
* @brief      Delete thread
* @ingroup    PLATFORM_ESP32
* 
* @param[in]  groupID : Group ID value.
* @param[in]  xthread : Xthread pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XESP32FACTORY::DeleteThread(XTHREADGROUPID groupID, XTHREAD* xthread)
{
  if(!xthread) return false;

  XESP32THREAD* _xthread = (XESP32THREAD*)xthread;
  GEN_DELETE _xthread;

  return true;
};















