/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XThread.cpp
* 
* @class      XTHREAD
* @brief      eXtended Utils Thread class
* @ingroup    XUTILS
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

#include "XThread.h"

#include <stdio.h>
#include <string.h>

#include "XFactory.h"
#include "XTimer.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XMUTEX::XMUTEX()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XMUTEX::XMUTEX()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XMUTEX::~XMUTEX()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XMUTEX::~XMUTEX()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XMUTEX::IsLock()
* @brief      Is lock
* @ingroup    XUTILS
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XMUTEX::IsLock()
{
  return islock;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XMUTEX::Lock()
* @brief      Lock
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XMUTEX::Lock()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XMUTEX::UnLock()
* @brief      Un lock
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XMUTEX::UnLock()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XMUTEX::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XMUTEX::Clean()
{
  islock = false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTHREAD::XTHREAD(XTHREADGROUPID groupID, XCHAR* ID, XTHREADFUNCTION function, void* param)
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* @param[in]  groupID : Group ID value.
* @param[in]  ID : Identifier to use.
* @param[in]  function : Function value.
* @param[in]  param : Param pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTHREAD::XTHREAD(XTHREADGROUPID groupID, XCHAR* ID, XTHREADFUNCTION function, void* param)
{
  Clean();

  statusfunc = XTHREADSTATUS_NONE;

  this->groupID     = groupID;
  this->ID          = ID;
  this->function    = function;
  this->param       = param;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTHREAD::~XTHREAD()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTHREAD::~XTHREAD()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTHREAD::Ini(bool run)
* @brief      Initialize the object
* @ingroup    XUTILS
* 
* @param[in]  run : Run value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTHREAD::Ini(bool run)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTHREAD::Wait(int miliseconds)
* @brief      Wait
* @ingroup    XUTILS
* 
* @param[in]  miliseconds : Miliseconds value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTHREAD::Wait(int miliseconds)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTHREAD::End()
* @brief      End the object
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTHREAD::End()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTHREAD::IsRunning()
* @brief      Is running
* @ingroup    XUTILS
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTHREAD::IsRunning()
{
  if(statusfunc == XTHREADSTATUS_RUN) return true;
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTHREAD::Run(bool activate)
* @brief      Run operation
* @ingroup    XUTILS
* 
* @param[in]  activate : Activate value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTHREAD::Run(bool activate)
{
  if(activate)
    {
      if(IsRunning()) return false;
      statusfunc = XTHREADSTATUS_RUN;
    }
    else
    {
      if(!IsRunning()) return false;
      statusfunc = XTHREADSTATUS_STOP;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTHREAD::WaitToEnd(XDWORD timeout)
* @brief      Wait to end
* @ingroup    XUTILS
* 
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTHREAD::WaitToEnd(XDWORD timeout)
{
  if(!gotofunction)
    {
      statusfunc = XTHREADSTATUS_END;
      return true;
    }

  bool statustimeout = false;

  XTIMER* xtimerout=GEN_XFACTORY.CreateTimer();
  if(!xtimerout) return false;
 
  while(statusfunc != XTHREADSTATUS_END)
    {
      Wait();

      if(xtimerout->GetMeasureSeconds() >= timeout)
        {
          statustimeout = true;
          break;
        }
    }

  GEN_XFACTORY.DeleteTimer(xtimerout);

  return (!statustimeout);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XTHREAD::GetWaitYield()
* @brief      Get wait yield
* @ingroup    XUTILS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XTHREAD::GetWaitYield()
{
  return waityield;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTHREAD::SetWaitYield(XDWORD waityield)
* @brief      Set wait yield
* @ingroup    XUTILS
* 
* @param[in]  waityield : Waityield value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTHREAD::SetWaitYield(XDWORD waityield)
{
  this->waityield = waityield;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTHREAD::Exit()
* @brief      Exit
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTHREAD::Exit()
{
  statusfunc = XTHREADSTATUS_EXIT;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTHREADGROUPID XTHREAD::GetGroupID()
* @brief      Get group ID
* @ingroup    XUTILS
* 
* @return     XTHREADGROUPID : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTHREADGROUPID XTHREAD::GetGroupID()
{
  return groupID;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTHREAD::SetGroupID(XTHREADGROUPID groupID)
* @brief      Set group ID
* @ingroup    XUTILS
* 
* @param[in]  groupID : Group ID value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTHREAD::SetGroupID(XTHREADGROUPID groupID)
{
  this->groupID = groupID;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* XTHREAD::GetID()
* @brief      Get ID
* @ingroup    XUTILS
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* XTHREAD::GetID()
{
  return &ID;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTHREADFUNCTION XTHREAD::GetFunction()
* @brief      Get function
* @ingroup    XUTILS
* 
* @return     XTHREADFUNCTION : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTHREADFUNCTION XTHREAD::GetFunction()
{
  return function;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void* XTHREAD::GetParam()
* @brief      Get param
* @ingroup    XUTILS
* 
* @return     void* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void* XTHREAD::GetParam()
{
  return param;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTHREADPRIORITY XTHREAD::GetPriority()
* @brief      Get priority
* @ingroup    XUTILS
* 
* @return     XTHREADPRIORITY : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTHREADPRIORITY XTHREAD::GetPriority()
{
  return priority;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTHREAD::SetPriority(XTHREADPRIORITY priority)
* @brief      Set priority
* @ingroup    XUTILS
* 
* @param[in]  priority : Priority value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTHREAD::SetPriority(XTHREADPRIORITY priority)
{
  this->priority = priority;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XTHREAD::GetStackSize()
* @brief      Get stack size
* @ingroup    XUTILS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XTHREAD::GetStackSize()
{
  return stacksize;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTHREAD::SetStackSize(XDWORD stacksize)
* @brief      Set stack size
* @ingroup    XUTILS
* 
* @param[in]  stacksize : Stacksize value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTHREAD::SetStackSize(XDWORD stacksize)
{
  this->stacksize = stacksize;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTHREAD::IsInFunction()
* @brief      Is in function
* @ingroup    XUTILS
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTHREAD::IsInFunction()
{
  return isinfunction;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTHREADSTATUS XTHREAD::GetStatusFunc()
* @brief      Get status func
* @ingroup    XUTILS
* 
* @return     XTHREADSTATUS : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTHREADSTATUS XTHREAD::GetStatusFunc()
{
  return statusfunc;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTHREAD::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTHREAD::Clean()
{
  groupID         = XTHREADGROUPID_UNKNOWN;
  ID.Empty();
  function        = NULL;
  param           = NULL;

  waityield       = XTHREAD_DEFAULTWAITYIELD;

  priority        = XTHREADPRIORITY_UNKNOWN;
  stacksize       = 0;

  gotofunction    = false;
  isinfunction    = false;
  statusfunc      = XTHREADSTATUS_NONE;
}





