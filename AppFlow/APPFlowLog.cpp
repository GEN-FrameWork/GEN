/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       APPFlowLog.cpp
* 
* @class      APPFLOWLOG
* @brief      Application Flow Log class
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

#include "APPFlowLog.h"

#include "XLog.h"
#include "XSystem.h"
#include "XTranslation.h"
#include "XTranslation_GEN.h"
#include "XTranslation.h"
#include "XLanguage_ISO_639_3.h"

#include "APPFlowBase.h"
#include "APPFlowCFG.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

APPFLOWLOG* APPFLOWLOG::instance = NULL;



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWLOG::GetIsInstanced()
* @brief      Get is instanced
* @ingroup    APPFLOW
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWLOG::GetIsInstanced()
{
  return instance != NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         APPFLOWLOG& APPFLOWLOG::GetInstance()
* @brief      Get instance
* @ingroup    APPFLOW
* 
* @return     APPFLOWLOG& : Reference to the requested object.
* 
* --------------------------------------------------------------------------------------------------------------------*/
APPFLOWLOG& APPFLOWLOG::GetInstance()
{
  if(!instance) instance = GEN_NEW APPFLOWLOG();

  return (*instance);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWLOG::DelInstance()
* @brief      Del instance
* @ingroup    APPFLOW
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWLOG::DelInstance()
{
  if(instance)
    {
      instance->End();

      GEN_DELETE instance;
      instance = NULL;

      return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWLOG::Ini(APPFLOWCFG* cfg, XCHAR* applicationname)
* @brief      Initialize the object
* @ingroup    APPFLOW
* 
* @param[in]  cfg : Configuration object to use.
* @param[in]  applicationname : Application name.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWLOG::Ini(APPFLOWCFG* cfg, XCHAR* applicationname)
{
  if(!cfg) 
    {
      return false;
    }

  XPATH xpathsection;
  XPATH xpath;
  bool  status;

  this->cfg = cfg;

  GEN_XPATHSMANAGER.GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpathsection);
  xpath.Create(3 , xpathsection.Get(), applicationname, __L(".log"));

  status = GEN_XLOG.Ini(xpath, applicationname, true);
  if(status)
    {     
      GEN_XLOG.SetLimit(XLOGTYPELIMIT_SIZE, cfg->Log_MaxSize()*1000, cfg->Log_ReductionPercent());
      GEN_XLOG.SetFilters(cfg->Log_ActiveSectionsID()->Get(), cfg->Log_LevelMask());
      GEN_XLOG.SetBackup(cfg->Log_Backup_IsActive(), cfg->Log_Backup_GetMaxFiles(), cfg->Log_Backup_IsCompress());           
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool APPFLOWLOG::End()
* @brief      End the object
* @ingroup    APPFLOW
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool APPFLOWLOG::End()
{
  GEN_XLOG.End();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         APPFLOWLOG::APPFLOWLOG()
* @brief      Constructor of class
* @ingroup    APPFLOW
* 
* --------------------------------------------------------------------------------------------------------------------*/
APPFLOWLOG::APPFLOWLOG()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         APPFLOWLOG::~APPFLOWLOG()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    APPFLOW
* 
* --------------------------------------------------------------------------------------------------------------------*/
APPFLOWLOG::~APPFLOWLOG()
{
  End();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void APPFLOWLOG::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    APPFLOW
* 
* --------------------------------------------------------------------------------------------------------------------*/
void APPFLOWLOG::Clean()
{
  cfg = NULL;
}


