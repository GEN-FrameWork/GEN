/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XScheduler.cpp
* 
* @class      XSCHEDULER
* @brief      eXtended Utils Scheduler class
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

#include "XScheduler.h"

#include "XFactory.h"
#include "XSleep.h"
#include "XThread.h"
#include "XDateTime.h"
#include "XTimer.h"
#include "XTrace.h"

#include "XScheduler_XEvent.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSCHEDULERTASK::XSCHEDULERTASK(XSCHEDULER* xscheduler)
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* @param[in]  xscheduler : Xscheduler pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSCHEDULERTASK::XSCHEDULERTASK(XSCHEDULER* xscheduler)
{
  Clean();

  this->xscheduler  = xscheduler;

  xtimer=GEN_XFACTORY.CreateTimer();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSCHEDULERTASK::~XSCHEDULERTASK()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSCHEDULERTASK::~XSCHEDULERTASK()
{
  if(xtimer)
    {
      GEN_XFACTORY.DeleteTimer(xtimer);
      xtimer = NULL;
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSCHEDULERTASK::IsActive()
* @brief      Is active
* @ingroup    XUTILS
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSCHEDULERTASK::IsActive()
{
  return active;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSCHEDULERTASK::SetIsActive(bool active)
* @brief      Set is active
* @ingroup    XUTILS
* 
* @param[in]  active : Active value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSCHEDULERTASK::SetIsActive(bool active)
{
  this->active  = active;
  if(xtimer) xtimer->Reset();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XSCHEDULERTASK::GetID()
* @brief      Get ID
* @ingroup    XUTILS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XSCHEDULERTASK::GetID()
{
  return ID;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XSCHEDULERTASK::SetID(XDWORD ID)
* @brief      Set ID
* @ingroup    XUTILS
* 
* @param[in]  ID : Identifier to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XSCHEDULERTASK::SetID(XDWORD ID)
{
  this->ID = ID;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTIMER* XSCHEDULERTASK::GetXTimer()
* @brief      Get X timer
* @ingroup    XUTILS
* 
* @return     XTIMER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTIMER* XSCHEDULERTASK::GetXTimer()
{
  return xtimer;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSCHEDULERTASK::IsInValidTimeLimit()
* @brief      Is in valid time limit
* @ingroup    XUTILS
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSCHEDULERTASK::IsInValidTimeLimit()
{
  return isinvalidtimelimit;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDATETIME* XSCHEDULERTASK::GetTimeLimitStart()
* @brief      Get time limit start
* @ingroup    XUTILS
* 
* @return     XDATETIME* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDATETIME* XSCHEDULERTASK::GetTimeLimitStart()
{
  return &xdatetimelimitstart;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDATETIME* XSCHEDULERTASK::GetTimeLimitEnd()
* @brief      Get time limit end
* @ingroup    XUTILS
* 
* @return     XDATETIME* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDATETIME* XSCHEDULERTASK::GetTimeLimitEnd()
{
  return &xdatetimelimitend;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSCHEDULERTASK::IsCyclic()
* @brief      Is cyclic
* @ingroup    XUTILS
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSCHEDULERTASK::IsCyclic()
{
  return (!ncyclestodo)?false:true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XSCHEDULERTASK::GetNCyclesTodo()
* @brief      Get N cycles todo
* @ingroup    XUTILS
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XSCHEDULERTASK::GetNCyclesTodo()
{
  return ncyclestodo;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSCHEDULERTASK::SetNCycles(int ncyclestodo, XDATETIME* xdatetimecadence)
* @brief      Set N cycles
* @ingroup    XUTILS
* 
* @param[in]  ncyclestodo : Ncyclestodo value.
* @param[in]  xdatetimecadence : Xdatetimecadence pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSCHEDULERTASK::SetNCycles(int ncyclestodo, XDATETIME* xdatetimecadence)
{
  this->ncyclestodo = ncyclestodo;
  if(this->ncyclestodo<0) this->ncyclestodo = XSCHEDULER_CYCLEFOREVER;

  this->ncyclesactual = this->ncyclestodo;

  if(xdatetimecadence) this->xdatetimecadence.CopyFrom(xdatetimecadence);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSCHEDULERTASK::SetNCycles(int ncyclestodo, XQWORD cadenceinseconds)
* @brief      Set N cycles
* @ingroup    XUTILS
* 
* @param[in]  ncyclestodo : Ncyclestodo value.
* @param[in]  cadenceinseconds : Cadenceinseconds value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSCHEDULERTASK::SetNCycles(int ncyclestodo, XQWORD cadenceinseconds)
{
  XDATETIME xdatetimecadence;
  XTIMER    xtimercadence;

  xdatetimecadence.SetToZero();

  xtimercadence.Reset();
  xtimercadence.AddSeconds(cadenceinseconds);

  xtimercadence.GetMeasureToDate(&xdatetimecadence);

  return SetNCycles(ncyclestodo, &xdatetimecadence);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSCHEDULERTASK::SetNCycles(int ncyclestodo, int cadenceinseconds)
* @brief      Set N cycles
* @ingroup    XUTILS
* 
* @param[in]  ncyclestodo : Ncyclestodo value.
* @param[in]  cadenceinseconds : Cadenceinseconds value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSCHEDULERTASK::SetNCycles(int ncyclestodo, int cadenceinseconds)
{
  return SetNCycles(ncyclestodo, (XQWORD)cadenceinseconds);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSCHEDULERTASK::IsStartImmediatelyCycles()
* @brief      Is start immediately cycles
* @ingroup    XUTILS
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSCHEDULERTASK::IsStartImmediatelyCycles()
{
  return isstartimmediatelycycles;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XSCHEDULERTASK::SetIsStartImmediatelyCycles(bool isstartimmediatelycycles)
* @brief      Set is start immediately cycles
* @ingroup    XUTILS
* 
* @param[in]  isstartimmediatelycycles : Isstartimmediatelycycles value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XSCHEDULERTASK::SetIsStartImmediatelyCycles(bool isstartimmediatelycycles)
{
  this->isstartimmediatelycycles = isstartimmediatelycycles;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSCHEDULERTASK::SetTimeLimits(XDATETIME* xdatetimelimitstart, XDATETIME* xdatetimelimitend)
* @brief      Set time limits
* @ingroup    XUTILS
* 
* @param[in]  xdatetimelimitstart : Xdatetimelimitstart pointer to use.
* @param[in]  xdatetimelimitend : Xdatetimelimitend pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSCHEDULERTASK::SetTimeLimits(XDATETIME* xdatetimelimitstart, XDATETIME* xdatetimelimitend)
{
  if(xdatetimelimitstart) this->xdatetimelimitstart.CopyFrom(xdatetimelimitstart);
  if(xdatetimelimitend)   this->xdatetimelimitend.CopyFrom(xdatetimelimitend);

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XSCHEDULERTASK::SetConditionDayWeek(XBYTE mask)
* @brief      Set condition day week
* @ingroup    XUTILS
* 
* @param[in]  mask : Mask value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XSCHEDULERTASK::SetConditionDayWeek(XBYTE mask)
{
  conditiondayweekmask =  mask;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSCHEDULERTASK::ResetCondition()
* @brief      Reset condition
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSCHEDULERTASK::ResetCondition()
{
  if(!xtimer)
    {
      return false;      
    }

  xtimer->Reset();

  ncyclesmade = 0;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSCHEDULERTASK::StartConditionImmediately()
* @brief      Start condition immediately
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSCHEDULERTASK::StartConditionImmediately()
{
  if(!xtimer)
    {
      return false;      
    }

  xtimer->AddSeconds(xdatetimecadence.GetSeconsFromDate() + 1);
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSCHEDULERTASK::CheckCondition(XDATETIME* xdatetimeactual, XTIMER* xtimeractual)
* @brief      Check condition
* @ingroup    XUTILS
* 
* @param[in]  xdatetimeactual : Xdatetimeactual pointer to use.
* @param[in]  xtimeractual : Xtimeractual pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSCHEDULERTASK::CheckCondition(XDATETIME* xdatetimeactual, XTIMER* xtimeractual)
{
  if(!xdatetimeactual) return false;

  XQWORD timeactualsec   = xdatetimeactual->GetSeconsFromDate();

  if(xdatetimelimitstart.IsValidDate())
    {
      if(timeactualsec <  xdatetimelimitstart.GetSeconsFromDate())
        {
          isinvalidtimelimit = false;

          xtimeractual->Reset();
          return false;
        }
       else isinvalidtimelimit = true;
    }

  if(xdatetimelimitend.IsValidDate())
    {
      if(timeactualsec >  xdatetimelimitend.GetSeconsFromDate())
        {
          isinvalidtimelimit = false;
          return false;

        } else isinvalidtimelimit = true;
    }

  if(conditiondayweekmask != XSCHEDULER_DAYWEEK_NONE)
    {
      if(!(GetDayOfWeekMask(xdatetimeactual) & conditiondayweekmask))
        {
          xtimeractual->Reset();
          return false;
        }
    }

  if(!ncyclesactual)  return false;

  bool checkcadence = true;
  if(isstartimmediatelycycles)
    {
      if(!ncyclesmade)  checkcadence = false;
    }

  if(checkcadence)
    {
      if(xdatetimecadence.IsValidDate())
        {
          if(xtimeractual->GetMeasureSeconds() < xdatetimecadence.GetSeconsFromDate()) return false;
        }
    }

  if(ncyclesactual>0)  ncyclesactual--;

  xtimeractual->Reset();

  ncyclesmade++;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE XSCHEDULERTASK::GetDayOfWeekMask(XDATETIME* xtimeactual)
* @brief      Get day of week mask
* @ingroup    XUTILS
* 
* @param[in]  xtimeactual : Xtimeactual pointer to use.
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE XSCHEDULERTASK::GetDayOfWeekMask(XDATETIME* xtimeactual)
{
  if(!xtimeactual) return XSCHEDULER_DAYWEEK_NONE;

  XBYTE mask = XSCHEDULER_DAYWEEK_NONE;

  switch(xtimeactual->GetDayOfWeek())
    {
      case XDATETIME_DAYWEEK_SUNDAY     : mask = XSCHEDULER_DAYWEEK_ISSUNDAY;     break;
      case XDATETIME_DAYWEEK_MONDAY     : mask = XSCHEDULER_DAYWEEK_ISMONDAY;     break;
      case XDATETIME_DAYWEEK_TUESDAY    : mask = XSCHEDULER_DAYWEEK_ISTUESDAY;    break;
      case XDATETIME_DAYWEEK_WEDNESDAY  : mask = XSCHEDULER_DAYWEEK_ISWEDNESDAY;  break;
      case XDATETIME_DAYWEEK_THURSDAY   : mask = XSCHEDULER_DAYWEEK_ISTHURSDAY;   break;
      case XDATETIME_DAYWEEK_FRIDAY     : mask = XSCHEDULER_DAYWEEK_ISFRIDAY;     break;
      case XDATETIME_DAYWEEK_SATURDAY   : mask = XSCHEDULER_DAYWEEK_ISSATURDAY;   break;
    }

  return mask;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XSCHEDULERTASK::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XSCHEDULERTASK::Clean()
{
  xscheduler                = NULL;
  xtimer                    = NULL;

  ID                        = 0;

  active                    = false;

  isstartimmediatelycycles  = false;
  ncyclesmade               = 0;
  ncyclestodo               = 0;
  ncyclesactual             = 0;

  isinvalidtimelimit        = false;

  conditiondayweekmask      = XSCHEDULER_DAYWEEK_NONE;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSCHEDULER::XSCHEDULER()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSCHEDULER::XSCHEDULER()
{
  Clean();

  RegisterEvent(XEVENT_TYPE_SCHEDULER);

  xtimerwait=GEN_XFACTORY.CreateTimer();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSCHEDULER::~XSCHEDULER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSCHEDULER::~XSCHEDULER()
{
  DeRegisterEvent(XEVENT_TYPE_SCHEDULER);

  End();

  if(xtimerwait) 
    {
      GEN_XFACTORY.DeleteTimer(xtimerwait);
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSCHEDULER::Ini()
* @brief      Initialize the object
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSCHEDULER::Ini()
{
  xmutexscheduler=GEN_XFACTORY.Create_Mutex();
  if(!xmutexscheduler) return false;

  xdatetimeactual=GEN_XFACTORY.CreateDateTime();
  if(!xdatetimeactual) return false;

  xthreadscheduler = CREATEXTHREAD(XTHREADGROUPID_SCHEDULER, __L("XSCHEDULER::Ini"), ThreadScheduler, (void*)this);
  if(!xthreadscheduler) return false;

  if(!xthreadscheduler->Ini()) return false;

  isactive = true;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSCHEDULER::IsActive()
* @brief      Is active
* @ingroup    XUTILS
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSCHEDULER::IsActive()
{
  return isactive;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XSCHEDULER::Activate(bool on)
* @brief      Activate
* @ingroup    XUTILS
* 
* @param[in]  on : On value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XSCHEDULER::Activate(bool on)
{
  this->isactive = on;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XMUTEX* XSCHEDULER::GetMutexScheduler()
* @brief      Get mutex scheduler
* @ingroup    XUTILS
* 
* @return     XMUTEX* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XMUTEX* XSCHEDULER::GetMutexScheduler()
{
  return xmutexscheduler;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDATETIME* XSCHEDULER::GetDateTimeActual()
* @brief      Get date time actual
* @ingroup    XUTILS
* 
* @return     XDATETIME* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDATETIME* XSCHEDULER::GetDateTimeActual()
{
  return xdatetimeactual;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSCHEDULER::Task_Add(XSCHEDULERTASK* task)
* @brief      Task add
* @ingroup    XUTILS
* 
* @param[in]  task : Task pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSCHEDULER::Task_Add(XSCHEDULERTASK* task)
{
  if(!task) return false;

  if(xmutexscheduler) xmutexscheduler->Lock();

  if(task->GetXTimer()) task->GetXTimer()->Reset();

  tasks.Add(task);

  if(xmutexscheduler) xmutexscheduler->UnLock();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSCHEDULERTASK* XSCHEDULER::Task_Get(int index)
* @brief      Task get
* @ingroup    XUTILS
* 
* @param[in]  index : Index value.
* 
* @return     XSCHEDULERTASK* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSCHEDULERTASK* XSCHEDULER::Task_Get(int index)
{
  if(tasks.IsEmpty()) return NULL;

  return (XSCHEDULERTASK*)tasks.Get(index);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSCHEDULERTASK* XSCHEDULER::Task_GetForID(XDWORD ID)
* @brief      Task get for ID
* @ingroup    XUTILS
* 
* @param[in]  ID : Identifier to use.
* 
* @return     XSCHEDULERTASK* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSCHEDULERTASK* XSCHEDULER::Task_GetForID(XDWORD ID)
{
  if(!xmutexscheduler) return NULL;
  if(tasks.IsEmpty())  return NULL;

  xmutexscheduler->Lock();

  for(XDWORD c=0;c<tasks.GetSize();c++)
    {
      XSCHEDULERTASK* task = tasks.Get(c);
      if(task)
        {
          if(task->GetID()==ID)
            {
              xmutexscheduler->UnLock();
              return task;
            }
        }
    }

  xmutexscheduler->UnLock();

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSCHEDULER::Task_Del(int index)
* @brief      Task del
* @ingroup    XUTILS
* 
* @param[in]  index : Index value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSCHEDULER::Task_Del(int index)
{
  if(!xmutexscheduler) return false;
  if(tasks.IsEmpty())  return false;

  bool status = false;

  xmutexscheduler->Lock();

  XSCHEDULERTASK* task = Task_Get(index);
  if(task)
    {
      status = tasks.Delete(task);
      GEN_DELETE task;
    }

  xmutexscheduler->UnLock();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSCHEDULER::Task_DelForID(XDWORD ID)
* @brief      Task del for ID
* @ingroup    XUTILS
* 
* @param[in]  ID : Identifier to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSCHEDULER::Task_DelForID(XDWORD ID)
{
  if(!xmutexscheduler) return false;
  if(tasks.IsEmpty())  return false;

  xmutexscheduler->Lock();

  bool status = false;

  for(XDWORD c=0;c<tasks.GetSize();c++)
    {
      XSCHEDULERTASK* task = tasks.Get(c);
      if(task)
        {
          if(task->GetID()==ID)
            {
              status = tasks.Delete(task);
              GEN_DELETE task;
              break;
            }
        }
    }

  xmutexscheduler->UnLock();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSCHEDULER::Task_DelAll()
* @brief      Task del all
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSCHEDULER::Task_DelAll()
{
  if(tasks.IsEmpty())  return false;

  if(xmutexscheduler) xmutexscheduler->Lock();

  tasks.DeleteContents();
  tasks.DeleteAll();

  if(xmutexscheduler) xmutexscheduler->UnLock();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XSCHEDULER::End()
* @brief      End the object
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XSCHEDULER::End()
{
  isactive = false;

  if(xthreadscheduler)
    {
      xthreadscheduler->End();
      DELETEXTHREAD(XTHREADGROUPID_SCHEDULER, xthreadscheduler);

      xthreadscheduler = NULL;
    }

  Task_DelAll();

  if(xmutexscheduler)
    {
      GEN_XFACTORY.Delete_Mutex(xmutexscheduler);
      xmutexscheduler = NULL;
    }

  if(xdatetimeactual)
    {
      GEN_XFACTORY.DeleteDateTime(xdatetimeactual);
      xdatetimeactual = NULL;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XSCHEDULER::ThreadScheduler(void* data)
* @brief      Thread scheduler
* @ingroup    XUTILS
* 
* @param[in]  data : Data buffer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XSCHEDULER::ThreadScheduler(void* data)
{
  XSCHEDULER* xscheduler = (XSCHEDULER*)data;
  if(!xscheduler)                      
    {
      return;
    }

  if(!xscheduler->isactive) 
    {
      return;
    }

  if(!xscheduler->xmutexscheduler)     
    {
      return;
    }

  if(!xscheduler->xdatetimeactual)     
    {
      return;
    }

  //GEN_XSLEEP.MilliSeconds(10);

  xscheduler->xmutexscheduler->Lock();

  XSCHEDULERTASK* task;

  task = xscheduler->Task_Get(xscheduler->indextask);
  if(!task)
    {
      xscheduler->xmutexscheduler->UnLock();
      xscheduler->indextask = 0;
      return;
    }

  if(xscheduler->xdatetimeactual) 
    {
      xscheduler->xdatetimeactual->Read();
    }

  if(task->IsActive())
    {
      if(task->CheckCondition(xscheduler->xdatetimeactual, task->GetXTimer()))
        {          
          XSCHEDULER_XEVENT xevent(xscheduler, XEVENT_TYPE_SCHEDULER);

          xevent.SetScheduler(xscheduler);
          xevent.SetTask(task);
          xevent.SetDateTime(xscheduler->xdatetimeactual);

          xscheduler->PostEvent(&xevent);           
        }
    }
   
  xscheduler->indextask++;
  xscheduler->xmutexscheduler->UnLock();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XSCHEDULER::Clean ()
* @brief      Clean internal state
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XSCHEDULER::Clean ()
{
  xmutexscheduler     = NULL;
  xthreadscheduler    = NULL;
  xtimerwait          = NULL;

  isactive            = false;

  xdatetimeactual     = NULL;
  indextask           = 0;
}





