/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XANDROIDFactory.cpp
* 
* @class      XANDROIDFACTORY
* @brief      ANDROID eXtended platform Factory
* @ingroup    PLATFORM_ANDROID
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

#include "XANDROIDFactory.h"

#include "XANDROIDDateTime.h"
#include "XANDROIDTimer.h"
#include "XANDROIDRand.h"
#include "XANDROIDFile.h"
#include "XANDROIDDir.h"
#include "XANDROIDSystem.h"
#include "XANDROIDConsole.h"
#include "XANDROIDThread.h"

#include "XFactoryMacros.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


CREATEFUNC(XANDROIDFACTORY, XTIMER     , XANDROIDTIMER      , CreateTimer)
DELETEFUNC(XANDROIDFACTORY, XTIMER     , XANDROIDTIMER      , DeleteTimer)

CREATEFUNC(XANDROIDFACTORY, XDATETIME  , XANDROIDDATETIME   , CreateDateTime)
DELETEFUNC(XANDROIDFACTORY, XDATETIME  , XANDROIDDATETIME   , DeleteDateTime)

CREATEFUNC(XANDROIDFACTORY, XRAND      , XANDROIDRAND       , CreateRand)
DELETEFUNC(XANDROIDFACTORY, XRAND      , XANDROIDRAND       , DeleteRand)

CREATEFUNC(XANDROIDFACTORY, XFILE      , XANDROIDFILE       , Create_File)
DELETEFUNC(XANDROIDFACTORY, XFILE      , XANDROIDFILE       , Delete_File)

CREATEFUNC(XANDROIDFACTORY, XDIR       , XANDROIDDIR        , Create_Dir)
DELETEFUNC(XANDROIDFACTORY, XDIR       , XANDROIDDIR        , Delete_Dir)

CREATEFUNC(XANDROIDFACTORY, XCONSOLE   , XANDROIDCONSOLE    , CreateConsole)
DELETEFUNC(XANDROIDFACTORY, XCONSOLE   , XANDROIDCONSOLE    , DeleteConsole)

CREATEFUNC(XANDROIDFACTORY, XMUTEX     , XANDROIDMUTEX      , Create_Mutex)
DELETEFUNC(XANDROIDFACTORY, XMUTEX     , XANDROIDMUTEX      , Delete_Mutex)


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTHREAD* XANDROIDFACTORY::CreateThread(XTHREADGROUPID groupID, XCHAR* ID, XTHREADFUNCTION function, void* param)
* @brief      Create thread
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  groupID : group ID of threads
* @param[in]  ID : ID for thread
* @param[in]  function : callback to thread
* @param[in]  param : param to callback
* 
* @return     XTHREAD* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTHREAD* XANDROIDFACTORY::CreateThread(XTHREADGROUPID groupID, XCHAR* ID, XTHREADFUNCTION function, void* param)
{
  XANDROIDTHREAD* _class =  GEN_NEW XANDROIDTHREAD(groupID, ID, function, param);
  return (XTHREAD*)_class;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XANDROIDFACTORY::DeleteThread(XTHREADGROUPID groupID, XTHREAD* xthread)
* @brief      Delete thread
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  groupID : group ID of threads
* @param[in]  xthread : thread to delete
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XANDROIDFACTORY::DeleteThread(XTHREADGROUPID groupID, XTHREAD* xthread)
{
  if(!xthread) return false;

  XANDROIDTHREAD* _xthread = (XANDROIDTHREAD*)xthread;
  GEN_DELETE _xthread;

  return true;
};














