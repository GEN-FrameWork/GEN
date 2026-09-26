/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       APPFlowExtended.cpp
* 
* @class      APPFLOWEXTENDED
* @brief      Application Flow Extended class
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

#include "APPFlowExtended.h"

#include "XFactory.h"
#include "XLog.h"
#include "XSystem.h"
#include "XConsole.h"
#include "XSleep.h"
#include "XThread.h"
#include "XTranslation_GEN.h"
#include "XTranslation.h"
#include "XLanguage_ISO_639_3.h"

#include "VersionFrameWork.h"

#include "APPFlowBase.h"
#include "APPFlowCFG.h"
#include "APPFlowLog.h"
#include "APPFlowConsole.h"
#ifdef APPFLOW_EXTENDED_APPLICATIONSTATUS_ACTIVE
#include "APPFlowExtended_ApplicationStatus.h"
#endif
#ifdef APPFLOW_EXTENDED_INTERNETSTATUS_ACTIVE
#include "APPFlowExtended_InternetStatus.h"
#endif



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

APPFLOWEXTENDED* APPFLOWEXTENDED::instance = NULL;



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWEXTENDED::GetIsInstanced()
* @brief      Get is instanced
* @ingroup    APPFLOW
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWEXTENDED::GetIsInstanced()
{
  return instance != NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         APPFLOWEXTENDED& APPFLOWEXTENDED::GetInstance()
* @brief      Get instance
* @ingroup    APPFLOW
* 
* @return     APPFLOWEXTENDED& : Reference to the requested object.
* 
* --------------------------------------------------------------------------------------------------------------------*/
APPFLOWEXTENDED& APPFLOWEXTENDED::GetInstance()
{
  if(!instance) instance = GEN_NEW APPFLOWEXTENDED();

  return (*instance);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWEXTENDED::DelInstance()
* @brief      Del instance
* @ingroup    APPFLOW
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWEXTENDED::DelInstance()
{
  if(instance)
    {      
      GEN_DELETE instance;
      instance = NULL;

      return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWEXTENDED::APPStart(APPFLOWCFG* appcfg, APPFLOWCONSOLE* appconsole)
* @brief      APP start
* @ingroup    APPFLOW
* 
* @param[in]  appcfg : Appconfiguration pointer to use.
* @param[in]  appconsole : Appconsole pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWEXTENDED::APPStart(APPFLOWCFG* appcfg, APPFLOWCONSOLE* appconsole)
{  
  XSTRING   string;
  XSTRING   stringresult;
  bool      status;

  if(!appcfg)
    { 
      return false;
    }

  if(!appcfg->Log_IsActive())
    {
      return false;
    }  

 //---------------------------------------------------------------------------------------------------------------------------------

  updatemutex = GEN_XFACTORY.Create_Mutex();  
  if(!updatemutex)
    {
      return false;
    }
      
  updatethread = GEN_XFACTORY.CreateThread(XTHREADGROUPID_APPFLOWEXTENDED, __L("APPFLOWEXTENDED::APPFLOWEXTENDED"), ThreadFunction_Update, this);
  if(!updatethread) 
    {
      return false;
    }

  updatethread->Ini();   

  //---------------------------------------------------------------------------------------------------------------------------------

  this->appcfg      = appcfg;
  this->appconsole  = appconsole;

  XSTRING SO_ID;
  status = GEN_XSYSTEM.GetOperativeSystemID(SO_ID);

  XTRACE_PRINT(__L(" "), NULL);

  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("%s"), GEN_VERSION.GetAppVersion()->Get()); 
  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("%s: %s"), XT_L(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPROOTPATH), GEN_XPATHSMANAGER.GetPathSection(XPATHSMANAGERSECTIONTYPE_ROOT)->xpath->Get()); 

  XTRACE_PRINT(__L(" "), NULL);
  
  XTRACE_PRINTMSGSTATUS(XT_L(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPVERSION), GEN_VERSION.GetAppVersion()->Get()); 
  XTRACE_PRINTMSGSTATUS(XT_L(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_SOVERSION), SO_ID.Get()); 

  status = APPFLOW_LOG.Ini(appcfg, GEN_VERSION.GetAppExecName()->Get()); 

  string.Format(APPFLOWCONSOLE_DEFAULT_MESSAGEMASK, XT_L(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INILOG));  
  stringresult.Format(__L("%s."), (status)?XT_L(XTRANSLATION_GEN_ID_OK):XT_L(XTRANSLATION_GEN_ID_ERROR));

  if(status)
    { 
      XDWORD total = 0;
      XDWORD free  = 0;

      GEN_XSYSTEM.GetMemoryInfo(total, free);

      APPFLOW_LOG_ENTRY(XLOGLEVEL_INFO, APPFLOW_CFG_LOG_SECTIONID_INITIATION, false, __L("-------------------------------------------------------------------------------------------------------------"));    
      APPFLOW_LOG_ENTRY(XLOGLEVEL_INFO, APPFLOW_CFG_LOG_SECTIONID_INITIATION, false, __L("%s: %s"), XT_L(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_SOVERSION), SO_ID.Get());
      APPFLOW_LOG_ENTRY(XLOGLEVEL_INFO, APPFLOW_CFG_LOG_SECTIONID_INITIATION, false, XT_L(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_TOTALMEMORY), total, free, GEN_XSYSTEM.GetFreeMemoryPercent());

      APPFLOW_LOG_ENTRY(((status)?XLOGLEVEL_INFO:XLOGLEVEL_ERROR), APPFLOW_CFG_LOG_SECTIONID_INITIATION, false, __L("%s: %s") , string.Get(), stringresult.Get());       
    }
  
  #ifdef APPFLOW_CONSOLE_ACTIVE
  if(appconsole)
    {     
      appconsole->GetConsole()->Clear();

      #ifdef APPFLOW_EXTENDED_APPLICATIONHEADER_ACTIVE
      appconsole->Show_Header(true);
      #endif

      appconsole->GetConsole()->PrintMessage(string.Get(), 1, true, false);
      appconsole->GetConsole()->PrintMessage(stringresult.Get(), 0, false, true);      
    }
  #endif 


  #ifdef APPFLOW_EXTENDED_APPLICATIONSTATUS_ACTIVE
  applicationstatus = GEN_NEW APPFLOWEXTENDED_APPLICATIONSTATUS(appcfg);
  if(!applicationstatus)
    {
      return false;
    }
   else 
    {
      status = true;
    }

  string.Format(APPFLOWCONSOLE_DEFAULT_MESSAGEMASK, XT_L(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INIAPPSTATUS));  
  stringresult.Format(__L("%s."), (status)?XT_L(XTRANSLATION_GEN_ID_OK):XT_L(XTRANSLATION_GEN_ID_ERROR));

  APPFLOW_LOG_ENTRY(((status)?XLOGLEVEL_INFO:XLOGLEVEL_ERROR), APPFLOW_CFG_LOG_SECTIONID_INITIATION, false, __L("%s: %s") , string.Get(), stringresult.Get());       

    #ifdef APPFLOW_CONSOLE_ACTIVE
    if(appconsole)
      {          
        appconsole->GetConsole()->PrintMessage(string.Get(), 1, true, false);
        appconsole->GetConsole()->PrintMessage(stringresult.Get(), 0, false, true);      
      }
    #endif  

  #endif


  #ifdef APPFLOW_EXTENDED_INTERNETSTATUS_ACTIVE
  internetstatus = GEN_NEW APPFLOWEXTENDED_INTERNETSTATUS(appcfg);
  if(!internetstatus)
    {
      return false;
    }
   else 
    {
      status = true;
    }

  string.Format(APPFLOWCONSOLE_DEFAULT_MESSAGEMASK, XT_L(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INIINTERNETSTATUS));  
  stringresult.Format(__L("%s."), (status)?XT_L(XTRANSLATION_GEN_ID_OK):XT_L(XTRANSLATION_GEN_ID_ERROR));

  APPFLOW_LOG_ENTRY(((status)?XLOGLEVEL_INFO:XLOGLEVEL_ERROR), APPFLOW_CFG_LOG_SECTIONID_INITIATION, false, __L("%s: %s") , string.Get(), stringresult.Get());       

    #ifdef APPFLOW_CONSOLE_ACTIVE
    if(appconsole)
      {          
        appconsole->GetConsole()->PrintMessage(string.Get(), 1, true, false);
        appconsole->GetConsole()->PrintMessage(stringresult.Get(), 0, false, true);      
      }
    #endif  

  #endif

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWEXTENDED::APPEnd()
* @brief      APP end
* @ingroup    APPFLOW
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWEXTENDED::APPEnd()
{
  XSTRING   string;
  XSTRING   stringresult;
  
  exitincurse = true;   
   
  if(!appcfg)
    { 
      return false;
    }

  if(!appcfg->Log_IsActive())
    {
      return false;
    } 

  if(updatemutex) 
    {
      updatemutex->Lock();
    }

  #ifdef APPFLOW_EXTENDED_INTERNETSTATUS_ACTIVE
  if(internetstatus)
    {
      GEN_DELETE internetstatus;
      internetstatus = NULL;  
    
      string.Format(APPFLOWCONSOLE_DEFAULT_MESSAGEMASK, XT_L(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_ENDINTERNETSTATUS));
      stringresult.Format(__L("%s."), XT_L(XTRANSLATION_GEN_ID_OK));    

      APPFLOW_LOG_ENTRY(XLOGLEVEL_INFO, APPFLOW_CFG_LOG_SECTIONID_ENDING, false, __L("%s: %s") , string.Get(), stringresult.Get()); 

      #ifdef APPFLOW_CONSOLE_ACTIVE
      if(appconsole)
        {        
          appconsole->GetConsole()->PrintMessage(string.Get(), 1, true, false);
          appconsole->GetConsole()->PrintMessage(stringresult.Get() , 0, false, true);     
        }
      #endif
    }
  #endif 


  #ifdef APPFLOW_EXTENDED_APPLICATIONSTATUS_ACTIVE
  if(applicationstatus)
    {
      GEN_DELETE applicationstatus;
      applicationstatus = NULL;  

      string.Format(APPFLOWCONSOLE_DEFAULT_MESSAGEMASK, XT_L(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_ENDAPPSTATUS));
      stringresult.Format(__L("%s."), XT_L(XTRANSLATION_GEN_ID_OK));    

      APPFLOW_LOG_ENTRY(XLOGLEVEL_INFO, APPFLOW_CFG_LOG_SECTIONID_ENDING, false, __L("%s: %s") , string.Get(), stringresult.Get()); 

      #ifdef APPFLOW_CONSOLE_ACTIVE
      if(appconsole)
        {        
          appconsole->GetConsole()->PrintMessage(string.Get(), 1, true, false);
          appconsole->GetConsole()->PrintMessage(stringresult.Get() , 0, false, true);     
        }
      #endif
    }
  #endif

     
  string.Format(APPFLOWCONSOLE_DEFAULT_MESSAGEMASK, XT_L(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_ENDLOG));
  stringresult.Format(__L("%s."), XT_L(XTRANSLATION_GEN_ID_OK));
  
  APPFLOW_LOG_ENTRY(XLOGLEVEL_INFO, APPFLOW_CFG_LOG_SECTIONID_ENDING, false, __L("%s: %s") , string.Get(), stringresult.Get());       
  APPFLOW_LOG_ENTRY(XLOGLEVEL_INFO, APPFLOW_CFG_LOG_SECTIONID_ENDING, false, __L("-------------------------------------------------------------------------------------------------------------"));

  APPFLOW_LOG.DelInstance();

  #ifdef APPFLOW_CONSOLE_ACTIVE
  if(appconsole)
    {     
      appconsole->GetConsole()->PrintMessage(string.Get(), 1, true, false);
      appconsole->GetConsole()->PrintMessage(stringresult.Get() , 0, false, true);     
    }
  #endif

  if(updatemutex) 
    {
      updatemutex->UnLock();
    }

  if(updatethread)
    {         
      updatethread->End();
      GEN_XFACTORY.DeleteThread(XTHREADGROUPID_APPFLOWEXTENDED, updatethread);

      updatethread = NULL;
    }

  if(updatemutex)
    {
      GEN_XFACTORY.Delete_Mutex(updatemutex);
      updatemutex = NULL;
    }
 
  appcfg      = NULL;
  appconsole  = NULL;
      
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         APPFLOWCFG* APPFLOWEXTENDED::GetCFG()
* @brief      Get CFG
* @ingroup    APPFLOW
* 
* @return     APPFLOWCFG* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
APPFLOWCFG* APPFLOWEXTENDED::GetCFG()
{
  return appcfg;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         APPFLOWCONSOLE* APPFLOWEXTENDED::GetConsole()
* @brief      Get console
* @ingroup    APPFLOW
* 
* @return     APPFLOWCONSOLE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
APPFLOWCONSOLE* APPFLOWEXTENDED::GetConsole()
{
  return appconsole;
}


#ifdef APPFLOW_EXTENDED_APPLICATIONSTATUS_ACTIVE
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         APPFLOWEXTENDED_APPLICATIONSTATUS* APPFLOWEXTENDED::GetApplicationStatus()
* @brief      Get application status
* @ingroup    APPFLOW
* 
* @return     APPFLOWEXTENDED_APPLICATIONSTATUS* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
APPFLOWEXTENDED_APPLICATIONSTATUS* APPFLOWEXTENDED::GetApplicationStatus()
{
  return applicationstatus;
}
#endif


#ifdef APPFLOW_EXTENDED_INTERNETSTATUS_ACTIVE
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         APPFLOWEXTENDED_INTERNETSTATUS* APPFLOWEXTENDED::GetInternetStatus()
* @brief      Get internet status
* @ingroup    APPFLOW
* 
* @return     APPFLOWEXTENDED_INTERNETSTATUS* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
APPFLOWEXTENDED_INTERNETSTATUS* APPFLOWEXTENDED::GetInternetStatus()
{
  return internetstatus;
}    
#endif


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWEXTENDED::ShowAll()
* @brief      Show all
* @ingroup    APPFLOW
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWEXTENDED::ShowAll()
{
  #ifdef APPFLOW_CONSOLE_ACTIVE

  if(!appconsole)
    { 
      return false;
    }

  if(updatemutex) 
    {
      updatemutex->Lock();
    }

  XCONSOLE* console = appconsole->GetConsole();
  if(!console)
    {
      return false;
    }

  console->Clear();

  #ifdef APPFLOW_EXTENDED_APPLICATIONHEADER_ACTIVE
  if(appconsole->Show_Header(false))      
    {
      console->PrintMessage(__L(""), 0, false, true);
    }
  #endif

  #ifdef APPFLOW_EXTENDED_APPLICATIONSTATUS_ACTIVE
  if(APPFLOW_EXTENDED.GetApplicationStatus()->Show(console))
    {
      console->PrintMessage(__L(""), 0, false, true);
    }
  #endif

  #ifdef APPFLOW_EXTENDED_INTERNETSTATUS_ACTIVE
  if(APPFLOW_EXTENDED.GetInternetStatus()->Show(console))
    {
      console->PrintMessage(__L(""), 0, false, true);
    }
  #endif
 
  if(updatemutex) 
    {
      updatemutex->UnLock();
    }

  #endif

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         APPFLOWEXTENDED::APPFLOWEXTENDED()
* @brief      Constructor of class
* @ingroup    APPFLOW
* 
* --------------------------------------------------------------------------------------------------------------------*/
APPFLOWEXTENDED::APPFLOWEXTENDED()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         APPFLOWEXTENDED::~APPFLOWEXTENDED()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    APPFLOW
* 
* --------------------------------------------------------------------------------------------------------------------*/
APPFLOWEXTENDED::~APPFLOWEXTENDED()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void APPFLOWEXTENDED::ThreadFunction_Update(void* param)
* @brief      Thread function update
* @ingroup    APPFLOW
* 
* @param[in]  param : Param pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void APPFLOWEXTENDED::ThreadFunction_Update(void* param)
{
  APPFLOWEXTENDED* appextended = (APPFLOWEXTENDED*)param;
  if(!appextended)
    {
      return;
    }

  if(appextended->exitincurse)
    {
      return;
    }

  if(appextended->updatemutex) 
    {
      appextended->updatemutex->Lock();
    }

  #ifdef APPFLOW_EXTENDED_APPLICATIONSTATUS_ACTIVE
  if(appextended->GetApplicationStatus())
    {
      appextended->GetApplicationStatus()->Update();
    }  
  #endif

  #ifdef APPFLOW_EXTENDED_INTERNETSTATUS_ACTIVE
  if(appextended->GetInternetStatus())
    {
      appextended->GetInternetStatus()->Update();
    }  
  #endif  

  if(appextended->updatemutex) 
    {
      appextended->updatemutex->UnLock();
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void APPFLOWEXTENDED::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    APPFLOW
* 
* --------------------------------------------------------------------------------------------------------------------*/
void APPFLOWEXTENDED::Clean()
{
  appcfg            = NULL;
  appconsole        = NULL;

  updatemutex       = NULL;
  updatethread      = NULL;  
  exitincurse       = false;
  
  #ifdef APPFLOW_EXTENDED_APPLICATIONSTATUS_ACTIVE
  applicationstatus = NULL;
  #endif

  #ifdef APPFLOW_EXTENDED_INTERNETSTATUS_ACTIVE
  internetstatus    = NULL;
  #endif
}


