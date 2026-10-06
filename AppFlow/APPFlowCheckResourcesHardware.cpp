/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       APPFlowCheckResourcesHardware.cpp
* 
* @class      APPFLOWCHECKRESOURCESHARDWARE
* @brief      Application Flow Check Resources Hardware class
* @ingroup    APPFLOW
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

#include "APPFlowCheckResourcesHardware.h"

#include "XLog.h"
#include "XScheduler.h"
#include "XScheduler_XEvent.h"
#include "XSystem.h"

#include "APPFlowCFG.h"

#include "APPFlowCheckResourcesHardware_XEvent.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         APPFLOWCHECKRESOURCESHARDWARE::APPFLOWCHECKRESOURCESHARDWARE()
* @brief      Constructor of class
* @ingroup    APPFLOW
* 
* --------------------------------------------------------------------------------------------------------------------*/
APPFLOWCHECKRESOURCESHARDWARE::APPFLOWCHECKRESOURCESHARDWARE()
{
  Clean();

  RegisterEvent(APPFLOWCHECKRESOURCESHARDWARE_XEVENT_TYPE_MEMFREELIMIT);
  RegisterEvent(APPFLOWCHECKRESOURCESHARDWARE_XEVENT_TYPE_TOTALCPUUSAGELIMIT);
  RegisterEvent(APPFLOWCHECKRESOURCESHARDWARE_XEVENT_TYPE_APPCPUUSAGELIMIT);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         APPFLOWCHECKRESOURCESHARDWARE::~APPFLOWCHECKRESOURCESHARDWARE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    APPFLOW
* 
* --------------------------------------------------------------------------------------------------------------------*/
APPFLOWCHECKRESOURCESHARDWARE::~APPFLOWCHECKRESOURCESHARDWARE()
{
  End();

  DeRegisterEvent(APPFLOWCHECKRESOURCESHARDWARE_XEVENT_TYPE_MEMFREELIMIT);
  DeRegisterEvent(APPFLOWCHECKRESOURCESHARDWARE_XEVENT_TYPE_TOTALCPUUSAGELIMIT);
  DeRegisterEvent(APPFLOWCHECKRESOURCESHARDWARE_XEVENT_TYPE_APPCPUUSAGELIMIT);

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWCHECKRESOURCESHARDWARE::Ini(APPFLOWCFG* cfg)
* @brief      Initialize the object
* @ingroup    APPFLOW
* 
* @param[in]  cfg : Configuration object to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWCHECKRESOURCESHARDWARE::Ini(APPFLOWCFG* cfg)
{
  if(!cfg) 
    {
      return false;
    }

  this->cfg = cfg;

  xscheduler = GEN_NEW XSCHEDULER();
  if(!xscheduler) return false;

  XSCHEDULERTASK* xtask;

  //---------------------------------------------------------

  if(cfg->CheckResourcesHardware_GetMemStatusCheckCadence())
    {
      xtask = GEN_NEW XSCHEDULERTASK(xscheduler);
      if(!xtask) return false;

      xtask->SetNCycles(XSCHEDULER_CYCLEFOREVER, cfg->CheckResourcesHardware_GetMemStatusCheckCadence());
      xtask->SetID(APPFLOWCHECKRESOURCESHARDWARE_TASKID_CHECKMEMORY);
      xtask->SetIsStartImmediatelyCycles(true);
      xtask->SetIsActive(true);

      xscheduler->Task_Add(xtask);
    }

  //---------------------------------------------------------
  
  /*
  for(int c=0; c<100; c++)
    {
      CheckTotalCPUUsageStatus();   
      CheckAppCPUUsageStatus();   
      GEN_XSLEEP.MilliSeconds(10);  
    }
  */
  
  if(cfg->CheckResourcesHardware_GetTotalCPUUsageCheckCadence())
    {
      xtask = GEN_NEW XSCHEDULERTASK(xscheduler);
      if(!xtask) return false;

      xtask->SetNCycles(XSCHEDULER_CYCLEFOREVER, cfg->CheckResourcesHardware_GetTotalCPUUsageCheckCadence());
      xtask->SetID(APPFLOWCHECKRESOURCESHARDWARE_TASKID_TOTALCPUUSAGE);
      xtask->SetIsStartImmediatelyCycles(true);
      xtask->SetIsActive(true);

      xscheduler->Task_Add(xtask);
    }

  if(cfg->CheckResourcesHardware_GetAppCPUUsageCheckCadence())
    {
      xtask = GEN_NEW XSCHEDULERTASK(xscheduler);
      if(!xtask) return false;

      xtask->SetNCycles(XSCHEDULER_CYCLEFOREVER, cfg->CheckResourcesHardware_GetAppCPUUsageCheckCadence());
      xtask->SetID(APPFLOWCHECKRESOURCESHARDWARE_TASKID_APPCPUUSAGE);
      xtask->SetIsStartImmediatelyCycles(true);
      xtask->SetIsActive(true);

      xscheduler->Task_Add(xtask);
    }

  //---------------------------------------------------------

  SubscribeEvent(XEVENT_TYPE_SCHEDULER, xscheduler);

  if(!xscheduler->Ini()) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD APPFLOWCHECKRESOURCESHARDWARE::GetCPUTotalMemory()
* @brief      Get CPU total memory
* @ingroup    APPFLOW
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD APPFLOWCHECKRESOURCESHARDWARE::GetCPUTotalMemory()
{
  return mem_total;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD APPFLOWCHECKRESOURCESHARDWARE::GetCPUFreeMemory()
* @brief      Get CPU free memory
* @ingroup    APPFLOW
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD APPFLOWCHECKRESOURCESHARDWARE::GetCPUFreeMemory()
{
  return mem_free;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int APPFLOWCHECKRESOURCESHARDWARE::GetCPUFreePercentMemory()
* @brief      Get CPU free percent memory
* @ingroup    APPFLOW
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int APPFLOWCHECKRESOURCESHARDWARE::GetCPUFreePercentMemory()
{
  return mem_freepercent;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int APPFLOWCHECKRESOURCESHARDWARE::GetCPUTotalCPUUsageAverange()
* @brief      Get CPU total CPU usage averange
* @ingroup    APPFLOW
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int APPFLOWCHECKRESOURCESHARDWARE::GetCPUTotalCPUUsageAverange()
{
  if(!ntotalCPUusage) return 0;

  return (ntotalCPUusage / ntotalCPUsamples);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int APPFLOWCHECKRESOURCESHARDWARE::GetCPUTotalCPUUsageMax()
* @brief      Get CPU total CPU usage max
* @ingroup    APPFLOW
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int APPFLOWCHECKRESOURCESHARDWARE::GetCPUTotalCPUUsageMax()
{
  return maxtotalCPUusage;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int APPFLOWCHECKRESOURCESHARDWARE::GetCPUAppCPUUsageAverange()
* @brief      Get CPU app CPU usage averange
* @ingroup    APPFLOW
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int APPFLOWCHECKRESOURCESHARDWARE::GetCPUAppCPUUsageAverange()
{
  if(!nappCPUusage) return 0;

  return (nappCPUusage / nappCPUsamples);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int APPFLOWCHECKRESOURCESHARDWARE::GetCPUAppCPUUsageMax()
* @brief      Get CPU app CPU usage max
* @ingroup    APPFLOW
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int APPFLOWCHECKRESOURCESHARDWARE::GetCPUAppCPUUsageMax()
{
  return maxappCPUusage;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWCHECKRESOURCESHARDWARE::End()
* @brief      End the object
* @ingroup    APPFLOW
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWCHECKRESOURCESHARDWARE::End()
{
  inexit =  true;

  if(xscheduler)
    {
      GEN_DELETE xscheduler;
      xscheduler = NULL;
    } 

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWCHECKRESOURCESHARDWARE::CheckMemoryStatus()
* @brief      Check memory status
* @ingroup    APPFLOW
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWCHECKRESOURCESHARDWARE::CheckMemoryStatus()
{
  if(!cfg) 
    {
      return false;
    }

  GEN_XSYSTEM.GetMemoryInfo(mem_total, mem_free);

  mem_freepercent = GEN_XSYSTEM.GetFreeMemoryPercent();

  if(mem_freepercent < cfg->CheckResourcesHardware_GetMemStatusLimitPercent())
    {
      GEN_XSYSTEM.FreeCacheMemory();

      //bool status =  GEN_XSYSTEM.FreeCacheMemory();
      //GEN_XLOG.AddEntry((status?XLOGLEVEL_INFO:XLOGLEVEL_WARNING), APPFLOW_CFG_LOG_SECTIONID_STATUSAPP, false, _L("Low free memory: %d Kb (%d%%). Cache memory released: %s"), free,  freepercent, (status?_L("Ok."):_L("Error!")));      
     
      APPFLOWCHECKRESOURCESHARDWARE_XEVENT  xevent(this, APPFLOWCHECKRESOURCESHARDWARE_XEVENT_TYPE_MEMFREELIMIT);      
      xevent.SetActualMemFree(mem_free, GEN_XSYSTEM.GetFreeMemoryPercent());      
      PostEvent(&xevent);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWCHECKRESOURCESHARDWARE::CheckTotalCPUUsageStatus()
* @brief      Check total CPU usage status
* @ingroup    APPFLOW
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWCHECKRESOURCESHARDWARE::CheckTotalCPUUsageStatus()
{
  if(!cfg) 
    {
      return false;
    }

  int cpuusage = GEN_XSYSTEM.GetCPUUsageTotal();

  if(cpuusage != XSYSTEM_CPUUSAGE_ERROR)
    {
      if(cpuusage > maxtotalCPUusage) 
        {
          maxtotalCPUusage = cpuusage;
        }
  
      ntotalCPUusage  += cpuusage;
      ntotalCPUsamples++;
    }
   
  if(ntotalCPUsamples > 5)
    {
      if(GetCPUTotalCPUUsageAverange() > (XDWORD)cfg->CheckResourcesHardware_GetTotalCPUUsageLimitPercent())
        {      
          APPFLOWCHECKRESOURCESHARDWARE_XEVENT  xevent(this, APPFLOWCHECKRESOURCESHARDWARE_XEVENT_TYPE_TOTALCPUUSAGELIMIT);        
          xevent.SetActualTotalCPUUsage(cpuusage);      
          PostEvent(&xevent);
        } 
    }
    
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWCHECKRESOURCESHARDWARE::CheckAppCPUUsageStatus()
* @brief      Check app CPU usage status
* @ingroup    APPFLOW
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWCHECKRESOURCESHARDWARE::CheckAppCPUUsageStatus()
{
  if(!cfg) 
    {
      return false;
    }

  int cpuusage = 0;

  if(!cfg->CheckResourcesHardware_GetAppCPUUsageProcessName()->IsEmpty())
    {
      cpuusage = GEN_XSYSTEM.GetCPUUsageForProcessName(cfg->CheckResourcesHardware_GetAppCPUUsageProcessName()->Get());  

      if(cpuusage != XSYSTEM_CPUUSAGE_ERROR)
        {
          if(cpuusage > maxappCPUusage) 
            {
              maxappCPUusage = cpuusage;
            } 

          nappCPUusage  += cpuusage;
          nappCPUsamples++;
        }
    }

  if(nappCPUsamples > 5)
    {
      if(GetCPUAppCPUUsageAverange() > (XDWORD)cfg->CheckResourcesHardware_GetAppCPUUsageLimitPercent())
        {      
          APPFLOWCHECKRESOURCESHARDWARE_XEVENT  xevent(this, APPFLOWCHECKRESOURCESHARDWARE_XEVENT_TYPE_APPCPUUSAGELIMIT);        
          xevent.SetActualAppCPUUsage(cpuusage);      
          PostEvent(&xevent);
        } 
    }
    
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void APPFLOWCHECKRESOURCESHARDWARE::HandleEvent_Scheduler(XSCHEDULER_XEVENT* event)
* @brief      Handle event scheduler
* @note       INTERNAL
* @ingroup    APPFLOW
* 
* @param[in]  event : Event to process.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void APPFLOWCHECKRESOURCESHARDWARE::HandleEvent_Scheduler(XSCHEDULER_XEVENT* event)
{
  if(inexit) 
    {
      return;
    }   

  switch(event->GetTask()->GetID())
    {
      case APPFLOWCHECKRESOURCESHARDWARE_TASKID_CHECKMEMORY       : CheckMemoryStatus();            break;

      case APPFLOWCHECKRESOURCESHARDWARE_TASKID_TOTALCPUUSAGE     : CheckTotalCPUUsageStatus();     break;
      case APPFLOWCHECKRESOURCESHARDWARE_TASKID_APPCPUUSAGE       : CheckAppCPUUsageStatus();       break;
    } 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void APPFLOWCHECKRESOURCESHARDWARE::HandleEvent(XEVENT* xevent)
* @brief      Handle event
* @note       INTERNAL
* @ingroup    APPFLOW
* 
* @param[in]  xevent : Xevent pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void APPFLOWCHECKRESOURCESHARDWARE::HandleEvent(XEVENT* xevent)
{
  if(!xevent) 
    {
      return; 
    }

  switch(xevent->GetEventFamily())
    {
      case XEVENT_TYPE_SCHEDULER      : { XSCHEDULER_XEVENT* event = (XSCHEDULER_XEVENT*)xevent;
                                          if(!event) return;

                                          HandleEvent_Scheduler(event);
                                        }
                                        break;
    } 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void APPFLOWCHECKRESOURCESHARDWARE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    APPFLOW
* 
* --------------------------------------------------------------------------------------------------------------------*/
void APPFLOWCHECKRESOURCESHARDWARE::Clean()
{
  cfg                     = NULL;

  inexit                  = false;
  xscheduler              = NULL;

  ntotalCPUusage          = 0;
  ntotalCPUsamples        = 0;
  maxtotalCPUusage        = 0;

  nappCPUusage            = 0;
  nappCPUsamples          = 0;
  maxappCPUusage          = 0;
}


