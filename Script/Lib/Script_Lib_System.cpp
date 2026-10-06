/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       Script_Lib_System.cpp
* 
* @class      SCRIPT_LIB_SYSTEM
* @brief      Script Library System
* @ingroup    SCRIPT
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

#include "Script_Lib_System.h"

#include <stdio.h>
#include <stdlib.h>
#include <cmath>

#include "XFactory.h"
#include "XPath.h"
#include "XSystem.h"

#include "Script.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/




/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SCRIPT_LIB_SYSTEM::SCRIPT_LIB_SYSTEM()
* @brief      Constructor of class
* @ingroup    SCRIPT
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
SCRIPT_LIB_SYSTEM::SCRIPT_LIB_SYSTEM() : SCRIPT_LIB(SCRIPT_LIB_SYSTEM_NAME)
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SCRIPT_LIB_SYSTEM::~SCRIPT_LIB_SYSTEM()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
SCRIPT_LIB_SYSTEM::~SCRIPT_LIB_SYSTEM()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT_LIB_SYSTEM::AddLibraryFunctions(SCRIPT* script)
* @brief      Add library functions
* @ingroup    SCRIPT
* 
* @param[in]  script : Script pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT_LIB_SYSTEM::AddLibraryFunctions(SCRIPT* script)
{
  if(!script) return false;

  this->script = script;

  script->AddLibraryFunction(this, _L("System_GetType"), Call_System_GetType);
  script->AddLibraryFunction(this, _L("System_GetOperativeSystemID"), Call_System_GetOperativeSystemID);
  script->AddLibraryFunction(this, _L("System_GetHardwareType"), Call_System_GetHardwareType);
  script->AddLibraryFunction(this, _L("System_IsWindows"), Call_System_IsWindows);
  script->AddLibraryFunction(this, _L("System_IsLinux"), Call_System_IsLinux);
  script->AddLibraryFunction(this, _L("System_IsAndroid"), Call_System_IsAndroid);
  script->AddLibraryFunction(this, _L("System_GetLanguageSO"), Call_System_GetLanguageSO);
  script->AddLibraryFunction(this, _L("System_GetUser"), Call_System_GetUser);
  script->AddLibraryFunction(this, _L("System_GetDomain"), Call_System_GetDomain);
  script->AddLibraryFunction(this, _L("System_GetFreeMemoryPercent"), Call_System_GetFreeMemoryPercent);
  script->AddLibraryFunction(this, _L("System_GetPathExecApplication"), Call_System_GetPathExecApplication);

  script->AddLibraryFunction(this, _L("System_Reboot"), Call_System_Reboot);
  script->AddLibraryFunction(this, _L("System_PowerOff"), Call_System_PowerOff);
  script->AddLibraryFunction(this, _L("System_Logout"), Call_System_Logout);
  script->AddLibraryFunction(this, _L("System_GetEnviromentVar"), Call_System_GetEnviromentVar);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SCRIPT_LIB_SYSTEM::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SCRIPT_LIB_SYSTEM::Clean()
{

}




/*---- LIBRARY FUNCTIONS ---------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_System_GetType(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_System_GetType
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_System_GetType(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  switch(GEN_XSYSTEM.GetPlatform())
    {
      case XSYSTEM_PLATFORM_UNKNOWN         :
                      default               : (*returnvalue) = "";                    break;
      case XSYSTEM_PLATFORM_WINDOWS         : (*returnvalue) = "Windows";             break; 
      case XSYSTEM_PLATFORM_LINUX           : (*returnvalue) = "Linux";               break;
      case XSYSTEM_PLATFORM_LINUX_EMBEDDED  : (*returnvalue) = "LinuxEmbedded";       break;
      case XSYSTEM_PLATFORM_ANDROID         : (*returnvalue) = "Android";             break;
      case XSYSTEM_PLATFORM_STM32           : (*returnvalue) = "STM32";               break;
      case XSYSTEM_PLATFORM_ESP32           : (*returnvalue) = "ESP32";               break;
      case XSYSTEM_PLATFORM_SAMD5XE5X       : (*returnvalue) = "SAMD5xE5x";           break;
    };
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_System_GetOperativeSystemID(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_System_GetOperativeSystemID
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_System_GetOperativeSystemID(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  XSTRING osid;

  if(GEN_XSYSTEM.GetOperativeSystemID(osid))
    {
      (*returnvalue) = osid.Get();
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_System_GetHardwareType(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_System_GetHardwareType
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_System_GetHardwareType(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  switch(GEN_XSYSTEM.GetTypeHardware())
    {
      case XSYSTEM_HARDWARETYPE_UNKNOWN         :
                      default                   : (*returnvalue) = "Unknown";         break;
      case XSYSTEM_HARDWARETYPE_PC              : (*returnvalue) = "PC";              break;
      case XSYSTEM_HARDWARETYPE_LEX3V700A       : (*returnvalue) = "LEX3V700A";       break;
      case XSYSTEM_HARDWARETYPE_ARMGENERIC      : (*returnvalue) = "ARMGeneric";      break;
      case XSYSTEM_HARDWARETYPE_ARTILA500       : (*returnvalue) = "Artila500";       break;
      case XSYSTEM_HARDWARETYPE_GUMSTIXCONNEX   : (*returnvalue) = "GumstixConnex";   break;
      case XSYSTEM_HARDWARETYPE_RASPBERRYPI     : (*returnvalue) = "RaspberryPi";     break;
      case XSYSTEM_HARDWARETYPE_BEAGLEBONE      : (*returnvalue) = "BeagleBone";      break;
      case XSYSTEM_HARDWARETYPE_NXP_IMX6        : (*returnvalue) = "NXP_iMX6";        break;
      case XSYSTEM_HARDWARETYPE_NXP_IMX8        : (*returnvalue) = "NXP_iMX8";        break;
      case XSYSTEM_HARDWARETYPE_MICRO_STM32     : (*returnvalue) = "STM32";           break;
      case XSYSTEM_HARDWARETYPE_MICRO_ESP32     : (*returnvalue) = "ESP32";           break;
      case XSYSTEM_HARDWARETYPE_MICRO_SAMD5XE5X : (*returnvalue) = "SAMD5xE5x";       break;
    };
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_System_IsWindows(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_System_IsWindows
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_System_IsWindows(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  (*returnvalue) = (bool)(GEN_XSYSTEM.GetPlatform() == XSYSTEM_PLATFORM_WINDOWS);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_System_IsLinux(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_System_IsLinux
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_System_IsLinux(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  XSYSTEM_PLATFORM platform = GEN_XSYSTEM.GetPlatform();

  (*returnvalue) = (bool)((platform == XSYSTEM_PLATFORM_LINUX) || (platform == XSYSTEM_PLATFORM_LINUX_EMBEDDED));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_System_IsAndroid(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_System_IsAndroid
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_System_IsAndroid(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  (*returnvalue) = (bool)(GEN_XSYSTEM.GetPlatform() == XSYSTEM_PLATFORM_ANDROID);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_System_GetLanguageSO(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_System_GetLanguageSO
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_System_GetLanguageSO(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  (*returnvalue) = (int)GEN_XSYSTEM.GetLanguageSO();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_System_GetUser(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_System_GetUser
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_System_GetUser(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  XSTRING user;
  XSTRING domain;

  if(GEN_XSYSTEM.GetUserAndDomain(user, domain))
    {
      (*returnvalue) = user.Get();
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_System_GetDomain(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_System_GetDomain
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_System_GetDomain(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  XSTRING user;
  XSTRING domain;

  if(GEN_XSYSTEM.GetUserAndDomain(user, domain))
    {
      (*returnvalue) = domain.Get();
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_System_GetFreeMemoryPercent(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_System_GetFreeMemoryPercent
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_System_GetFreeMemoryPercent(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  (*returnvalue) = GEN_XSYSTEM.GetFreeMemoryPercent();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_System_GetPathExecApplication(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_System_GetPathExecApplication
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_System_GetPathExecApplication(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  if(params->GetSize() < 1)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  XVARIANT* variant = params->Get(0);
  if(!variant)
    {
      return;
    }

  XSTRING appname = (*variant);
  XPATH   apppath;

  if(GEN_XSYSTEM.GetPathExecApplication(appname.Get(), apppath))
    {
      (*returnvalue) = apppath.Get();
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_System_Reboot(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_System_Reboot
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_System_Reboot(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  (*returnvalue) = GEN_XSYSTEM.ShutDown(XSYSTEM_CHANGESTATUSTYPE_REBOOT);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_System_PowerOff(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_System_PowerOff
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_System_PowerOff(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();
  
  (*returnvalue) = GEN_XSYSTEM.ShutDown(XSYSTEM_CHANGESTATUSTYPE_POWEROFF);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_System_Logout(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_System_Logout
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_System_Logout(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();
  
  (*returnvalue) = GEN_XSYSTEM.ShutDown(XSYSTEM_CHANGESTATUSTYPE_SESSION_LOGOFF);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_System_GetEnviromentVar(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_System_GetEnviromentVar
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_System_GetEnviromentVar(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  if(!params->GetSize())
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  XSTRING* namevar = (XSTRING*)params->Get(0)->GetData();    
  if(!namevar)  return;

  XCHAR* variable = GEN_XSYSTEM.GetEnviromentVariable(namevar->Get());
  if(variable)
    {
      (*returnvalue) = variable;
    }   
}
