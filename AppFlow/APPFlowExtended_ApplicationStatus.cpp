/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       APPFlowExtended_ApplicationStatus.cpp
* 
* @class      APPFLOWEXTENDED_APPLICATIONSTATUS
* @brief      Application Flow Extended Application Status class
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

#include "APPFlowExtended_ApplicationStatus.h"

#include "VersionFrameWork.h"

#include "XFactory.h"
#include "XSystem.h"
#include "XDateTime.h"
#include "XSleep.h"
#include "XThread.h"

#include "APPFlowCheckResourcesHardware.h"
#include "APPFlowCFG.h"
#include "APPFlowConsole.h"
#include "APPFlowExtended.h"

#include "XTranslation_GEN.h"
#include "XTranslation.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         APPFLOWEXTENDED_APPLICATIONSTATUS::APPFLOWEXTENDED_APPLICATIONSTATUS(APPFLOWCFG* cfg) : DIOWEBSERVER_RESOLVEENDPOINT()
* @brief      Constructor of class
* @ingroup    APPFLOW
* 
* 
* @param[in]  cfg : Application flow configuration.
* 
* --------------------------------------------------------------------------------------------------------------------*/
APPFLOWEXTENDED_APPLICATIONSTATUS::APPFLOWEXTENDED_APPLICATIONSTATUS(APPFLOWCFG* cfg) : DIOWEBSERVER_RESOLVEENDPOINT()
{
  Clean();

  this->cfg = cfg;

  GEN_XSYSTEM.GetOperativeSystemID(osversion);

  XDWORD app_version      = 0;
  XDWORD app_subversion   = 0;
  XDWORD app_versionerror = 0;

  GEN_VERSION.GetAppVersions(app_version, app_subversion, app_versionerror);
  GEN_VERSION.GetAppVersion(app_version, app_subversion, app_versionerror, appversion);

  xtimeroperatingtime = GEN_XFACTORY.CreateTimer();
  checkresourceshardware = GEN_NEW APPFLOWCHECKRESOURCESHARDWARE();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         APPFLOWEXTENDED_APPLICATIONSTATUS::~APPFLOWEXTENDED_APPLICATIONSTATUS()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    APPFLOW
* 
* --------------------------------------------------------------------------------------------------------------------*/
APPFLOWEXTENDED_APPLICATIONSTATUS::~APPFLOWEXTENDED_APPLICATIONSTATUS()
{  
  if(xtimeroperatingtime)
    {
      GEN_XFACTORY.DeleteTimer(xtimeroperatingtime);
    }

  if(checkresourceshardware)
    {
      checkresourceshardware->End();

      GEN_DELETE checkresourceshardware;
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         APPFLOWCHECKRESOURCESHARDWARE* APPFLOWEXTENDED_APPLICATIONSTATUS::GetCheckResourcesHardware()
* @brief      Get check resources hardware
* @ingroup    APPFLOW
* 
* @return     APPFLOWCHECKRESOURCESHARDWARE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
APPFLOWCHECKRESOURCESHARDWARE* APPFLOWEXTENDED_APPLICATIONSTATUS::GetCheckResourcesHardware()
{
  return checkresourceshardware; 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* APPFLOWEXTENDED_APPLICATIONSTATUS::GetOSVersion()
* @brief      Get OS version
* @ingroup    APPFLOW
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* APPFLOWEXTENDED_APPLICATIONSTATUS::GetOSVersion()
{
  return &osversion;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* APPFLOWEXTENDED_APPLICATIONSTATUS::GetAppVersion()
* @brief      Get app version
* @ingroup    APPFLOW
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* APPFLOWEXTENDED_APPLICATIONSTATUS::GetAppVersion()
{
  return &appversion;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD APPFLOWEXTENDED_APPLICATIONSTATUS::GetMemoryTotal()
* @brief      Get memory total
* @ingroup    APPFLOW
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD APPFLOWEXTENDED_APPLICATIONSTATUS::GetMemoryTotal()
{
  return memory_total;
}
    

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void APPFLOWEXTENDED_APPLICATIONSTATUS::SetMemoryTotal(XDWORD memory_total)
* @brief      Set memory total
* @ingroup    APPFLOW
* 
* @param[in]  memory_total : Memory total value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void APPFLOWEXTENDED_APPLICATIONSTATUS::SetMemoryTotal(XDWORD memory_total)
{
  this->memory_total = memory_total;
}
    

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD APPFLOWEXTENDED_APPLICATIONSTATUS::GetMemoryFree()
* @brief      Get memory free
* @ingroup    APPFLOW
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD APPFLOWEXTENDED_APPLICATIONSTATUS::GetMemoryFree()
{
  return memory_free;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void APPFLOWEXTENDED_APPLICATIONSTATUS::SetMemoryFree(XDWORD memory_free)
* @brief      Set memory free
* @ingroup    APPFLOW
* 
* @param[in]  memory_free : Memory free value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void APPFLOWEXTENDED_APPLICATIONSTATUS::SetMemoryFree(XDWORD memory_free)
{
  this->memory_free = memory_free;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD APPFLOWEXTENDED_APPLICATIONSTATUS::GetMemoryFreePercent()
* @brief      Get memory free percent
* @ingroup    APPFLOW
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD APPFLOWEXTENDED_APPLICATIONSTATUS::GetMemoryFreePercent()
{
  return memory_freepercent;
}
    

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void APPFLOWEXTENDED_APPLICATIONSTATUS::SetMemoryFreePercent(XDWORD memory_freepercent)
* @brief      Set memory free percent
* @ingroup    APPFLOW
* 
* @param[in]  memory_freepercent : Memory freepercent value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void APPFLOWEXTENDED_APPLICATIONSTATUS::SetMemoryFreePercent(XDWORD memory_freepercent)
{
  this->memory_freepercent = memory_freepercent;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* APPFLOWEXTENDED_APPLICATIONSTATUS::GetAverange()
* @brief      Get averange
* @ingroup    APPFLOW
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* APPFLOWEXTENDED_APPLICATIONSTATUS::GetAverange()
{
  return &averange;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* APPFLOWEXTENDED_APPLICATIONSTATUS::GetCurrentDate()
* @brief      Get current date
* @ingroup    APPFLOW
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* APPFLOWEXTENDED_APPLICATIONSTATUS::GetCurrentDate()
{
  return &currentdate;
}
    

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* APPFLOWEXTENDED_APPLICATIONSTATUS::GetOperatingTime()
* @brief      Get operating time
* @ingroup    APPFLOW
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* APPFLOWEXTENDED_APPLICATIONSTATUS::GetOperatingTime()
{
  return &operatingtime;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWEXTENDED_APPLICATIONSTATUS::Update()
* @brief      Update
* @ingroup    APPFLOW
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWEXTENDED_APPLICATIONSTATUS::Update()
{    
  if(checkresourceshardware && cfg)
    { 
      static bool iniappcheck = false;           

      if(!iniappcheck)
        {
          checkresourceshardware->Ini(cfg);
          iniappcheck = true;
        }

      bool ischecktotalCPUusage = cfg->CheckResourcesHardware_GetTotalCPUUsageCheckCadence()?true:false;
      bool ischeckappCPUusage   = cfg->CheckResourcesHardware_GetAppCPUUsageCheckCadence()?true:false;

      if(ischecktotalCPUusage || ischeckappCPUusage)
        {  
          averange.Empty();
  
          if(ischecktotalCPUusage)
            {
              averange.AddFormat(XT_L(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_CPUTOTALAVG), checkresourceshardware->GetCPUTotalCPUUsageAverange(), checkresourceshardware->GetCPUTotalCPUUsageMax());  
            }

          if(ischeckappCPUusage)
            {
              if(ischecktotalCPUusage)
                {
                  averange.AddFormat(_L(", "));
                }

              if(ischeckappCPUusage)
                {         
                  averange.AddFormat(XT_L(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_CPUAPPAVG), cfg->CheckResourcesHardware_GetAppCPUUsageProcessName()->Get(), checkresourceshardware->GetCPUAppCPUUsageAverange(), checkresourceshardware->GetCPUAppCPUUsageMax());
                }
            } 
        }         
    }

  GEN_XSYSTEM.GetMemoryInfo(memory_total, memory_free);
  memory_freepercent = GEN_XSYSTEM.GetFreeMemoryPercent();

  XDATETIME* datetime = GEN_XFACTORY.CreateDateTime();
  if(datetime)
    { 
      datetime->Read();
      datetime->GetDateTimeToString(XDATETIME_FORMAT_STANDARD | XDATETIME_FORMAT_TEXTMONTH | XDATETIME_FORMAT_ADDDAYOFWEEK, currentdate);

      GEN_XFACTORY.DeleteDateTime(datetime);
    }
  
  if(xtimeroperatingtime)
    {
      xtimeroperatingtime->GetMeasureString(operatingtime, true);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWEXTENDED_APPLICATIONSTATUS::Show(XCONSOLE* console)
* @brief      Show
* @ingroup    APPFLOW
* 
* @param[in]  console : Console pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWEXTENDED_APPLICATIONSTATUS::Show(XCONSOLE* console)
{
  XSTRING string;
  XSTRING string2;

  if(!APPFLOW_EXTENDED.GetConsole())
    { 
      return false;
    }

  string  = XT_L(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_OSVERSION);
  string2 = GetOSVersion()->Get();
  APPFLOW_EXTENDED.GetConsole()->Show_Line(string, string2);

  // string  = _L("Application Version");
  // string2 = GetAppVersion()->Get();
  // APPFLOW_EXTENDED.ShowLine(console, string, string2);
  
  string  = XT_L(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_CPUMEMORY);

  XDWORD      memorytotal       = GetMemoryTotal();
  XDWORD      memoryfree        = GetMemoryFree();
  double      memorydivisor     = 1.0;
  const XCHAR* memoryunit       = _L("KB");

  if(memorytotal >= (1024U * 1024U * 1024U))
    {
      memorydivisor = 1024.0 * 1024.0 * 1024.0;
      memoryunit    = _L("TB");
    }
  else if(memorytotal >= (1024U * 1024U))
    {
      memorydivisor = 1024.0 * 1024.0;
      memoryunit    = _L("GB");
    }
  else if(memorytotal >= 1024U)
    {
      memorydivisor = 1024.0;
      memoryunit    = _L("MB");
    }

  string2.Format(XT_L(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_MEMORYFREE), (double)memorytotal / memorydivisor, memoryunit, (double)memoryfree / memorydivisor, memoryunit, GetMemoryFreePercent());

  APPFLOW_EXTENDED.GetConsole()->Show_Line(string, string2);

  string  = XT_L(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_AVERANGE);
  string2 = GetAverange()->Get();
  APPFLOW_EXTENDED.GetConsole()->Show_Line(string, string2);

  string  = XT_L(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_CURRENTDATE);
  string2 = GetCurrentDate()->Get();
  APPFLOW_EXTENDED.GetConsole()->Show_Line(string, string2);

  string  = XT_L(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_OPERATINGTIME);
  string2 = GetOperatingTime()->Get();
  APPFLOW_EXTENDED.GetConsole()->Show_Line(string, string2);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWEXTENDED_APPLICATIONSTATUS::Serialize()
* @brief      Serialize
* @ingroup    APPFLOW
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWEXTENDED_APPLICATIONSTATUS::Serialize()
{
  Primitive_Add<XSTRING*>(&osversion      , _L("osversion"));
  Primitive_Add<XSTRING*>(&appversion     , _L("appversion"));

  Primitive_Add<int>(memory_total         , _L("memorytotal"));
  Primitive_Add<int>(memory_free          , _L("memoryfree"));
  Primitive_Add<int>(memory_freepercent   , _L("memoryfreepercent"));

  Primitive_Add<XSTRING*>(&averange       , _L("averange"));

  Primitive_Add<XSTRING*>(&currentdate    , _L("currentdate"));
  Primitive_Add<XSTRING*>(&operatingtime  , _L("operatingtime"));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWEXTENDED_APPLICATIONSTATUS::Deserialize()
* @brief      Deserialize
* @ingroup    APPFLOW
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWEXTENDED_APPLICATIONSTATUS::Deserialize()
{ 
  Primitive_Extract<XSTRING>(osversion           , _L("osversion"));  
  Primitive_Extract<XSTRING>(appversion          , _L("appversion"));

  Primitive_Extract<XDWORD>(memory_total         , _L("memorytotal"));
  Primitive_Extract<XDWORD>(memory_free          , _L("memoryfree"));
  Primitive_Extract<XDWORD>(memory_freepercent   , _L("memoryfreepercent"));  

  Primitive_Extract<XSTRING>(averange            , _L("averange"));

  Primitive_Extract<XSTRING>(currentdate         , _L("currentdate"));
  Primitive_Extract<XSTRING>(operatingtime       , _L("operatingtime"));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void APPFLOWEXTENDED_APPLICATIONSTATUS::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    APPFLOW
* 
* --------------------------------------------------------------------------------------------------------------------*/
void APPFLOWEXTENDED_APPLICATIONSTATUS::Clean()
{
  cfg                         = NULL;
  xtimeroperatingtime         = NULL;
  checkresourceshardware      = NULL;
 
  osversion.Empty();
  appversion.Empty();

  memory_total                = 0;
  memory_free                 = 0;
  memory_freepercent          = 0;

  averange.Empty();

  currentdate.Empty();
  operatingtime.Empty();
}


