/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       Script_Lib_Process.cpp
* 
* @class      SCRIPT_LIB_PROCESS
* @brief      Script Lib Process
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

#include "Script_Lib_Process.h"

#include "XPath.h"
#include "XProcessManager.h"
#include "XVariant.h"

#include "APPFlowBase.h"
#include "APPFlowMain.h"

#include "Script.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/




/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SCRIPT_LIB_PROCESS::SCRIPT_LIB_PROCESS()
* @brief      Constructor of class
* @ingroup    SCRIPT
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
SCRIPT_LIB_PROCESS::SCRIPT_LIB_PROCESS() : SCRIPT_LIB(SCRIPT_LIB_NAME_PROCESS)
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SCRIPT_LIB_PROCESS::~SCRIPT_LIB_PROCESS()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
SCRIPT_LIB_PROCESS::~SCRIPT_LIB_PROCESS()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT_LIB_PROCESS::AddLibraryFunctions(SCRIPT* script)
* @brief      Add library functions
* @ingroup    SCRIPT
* 
* @param[in]  script : Script pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT_LIB_PROCESS::AddLibraryFunctions(SCRIPT* script)
{
  if(!script) return false;

  this->script = script;

  script->AddLibraryFunction(this, __L("OpenURL"), Call_OpenURL);
  script->AddLibraryFunction(this, __L("ExecApplication"), Call_ExecApplication);
  script->AddLibraryFunction(this, __L("MakeCommand"), Call_MakeCommand);
  script->AddLibraryFunction(this, __L("TerminateAplication"), Call_TerminateApplication);
  script->AddLibraryFunction(this, __L("TerminateAplicationWithWindow"), Call_TerminateApplicationWithWindow);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SCRIPT_LIB_PROCESS::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SCRIPT_LIB_PROCESS::Clean()
{

}


/*---- LIBRARY FUNCTIONS ---------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_OpenURL(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      call  open URl
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_OpenURL(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params)  || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();

  if(!params->GetSize())
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  XSTRING* string1 = (XSTRING*)params->Get(0)->GetData();
  bool     status  = false;
  
  if(string1)
    {      
      status = GEN_XPROCESSMANAGER.OpenURL(string1->Get());      
    }

  (*returnvalue) = status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_MakeCommand(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call  make command
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_MakeCommand(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
   if((!library) || (!script) || (!params)  || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();

  if(!params->GetSize())
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  XSTRING* command    = NULL;
  bool     status     = false;
  
  if(params->Get(0))
    {
      command = (XSTRING*)params->Get(0)->GetData();
    }
  
  if(command)
    {      
      status = GEN_XPROCESSMANAGER.MakeCommand(command->Get());                       
    }

  (*returnvalue) = status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_ExecApplication(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_ExecApplication
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_ExecApplication(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params)  || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();

  if(!params->GetSize())
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  XSTRING* app_path   = NULL;
  XSTRING* app_params = NULL;
  bool     status     = false;
  
  if(params->Get(0))
    {
      app_path = (XSTRING*)params->Get(0)->GetData();
    }

  if(params->Get(1))
    {
      app_params = (XSTRING*)params->Get(1)->GetData();
    }
  
  if(app_path)
    {    
      int returncode = 0;

      // Do not capture stdin/stdout: that path waits for process exit (and used CREATE_NO_WINDOW).
      // ExecApplication is meant to launch interactive GUI apps and return while they stay open.
      status = GEN_XPROCESSMANAGER.Application_Execute(app_path->Get(), app_params?app_params->Get():__L(""), (XBUFFER*)NULL, (XBUFFER*)NULL, &returncode);    
      (*returnvalue) = status;  
      return;    
    }

  (*returnvalue) = false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_TerminateApplication(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_TerminateApplication
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_TerminateApplication(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{  
  if((!library) || (!script) || (!params)  || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();

  if(!params->GetSize())
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  XSTRING* string  = NULL;
  bool     status  = false;
  
  if(params->Get(0))
    {
      string = (XSTRING*)params->Get(0)->GetData();
    }
  
  if(string)
    {      
      status = GEN_XPROCESSMANAGER.Application_Terminate(string->Get());                       
    }

  (*returnvalue) = status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_TerminateApplicationWithWindow(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_TerminateApplicationWithWindow
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_TerminateApplicationWithWindow(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{   
  if((!library) || (!script) || (!params)  || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();

  if(params->GetSize()<2)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }
 
  XVECTOR<XPROCESS*>  applist;
  XSTRING             appname       = (*params->Get(0));
  XSTRING             windowstitle  = (*params->Get(1));
  int                 windowsposx   = 0; 
  bool                status        = false;

  if(GEN_XPROCESSMANAGER.Application_GetRunningList(applist, true))
    {
      for(XDWORD c=0; c<applist.GetSize(); c++)
        {                              
          if(applist.Get(c)->GetName()->Find(appname, true)!= XSTRING_NOTFOUND) 
            {  
              if(applist.Get(c)->GetWindowTitle()->Find(windowstitle, false) != XSTRING_NOTFOUND)
                {
                  status = GEN_XPROCESSMANAGER.Application_Terminate((*applist.Get(c)));                     
                  break;
                }
            }
        }
    }
    
  applist.DeleteContents();
  applist.DeleteAll();
   
  (*returnvalue) = status;
}



