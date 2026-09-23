/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       Script_Lib_InputSimulate.cpp
* 
* @class      SCRIPT_LIB_INPUTSIMULATE
* @brief      Script Lib Input Simulate class
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

#include "Script_Lib_InputSimulate.h"

#include "XVariant.h"
#include "XProcessManager.h"

#include "APPFlowBase.h"
#include "APPFlowMain.h"

#include "INPFactory.h"
#include "INPSimulate.h"

#include "Script.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/
			


/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SCRIPT_LIB_INPUTSIMULATE::SCRIPT_LIB_INPUTSIMULATE()
* @brief      Constructor
* @ingroup    SCRIPT
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
SCRIPT_LIB_INPUTSIMULATE::SCRIPT_LIB_INPUTSIMULATE() : SCRIPT_LIB(SCRIPT_LIB_NAME_INPUTSIMULATE )
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SCRIPT_LIB_INPUTSIMULATE::~SCRIPT_LIB_INPUTSIMULATE()
* @brief      Destructor
* @note       VIRTUAL
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
SCRIPT_LIB_INPUTSIMULATE::~SCRIPT_LIB_INPUTSIMULATE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT_LIB_INPUTSIMULATE::AddLibraryFunctions(SCRIPT* script)
* @brief      Add library functions
* @ingroup    SCRIPT
* 
* @param[in]  script : Script pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT_LIB_INPUTSIMULATE::AddLibraryFunctions(SCRIPT* script)
{
  if(!script) return false;

  this->script = script;

  script->AddLibraryFunction(this, __L("InpSim_Key_Press")             , Call_Key_Press);
  script->AddLibraryFunction(this, __L("InpSim_Key_UnPress")           , Call_Key_UnPress);
  script->AddLibraryFunction(this, __L("InpSim_Key_Click")             , Call_Key_Click);
  script->AddLibraryFunction(this, __L("InpSim_Key_ClickByLiteral")    , Call_Key_ClickByLiteral);
  script->AddLibraryFunction(this, __L("InpSim_Key_ClickByText")       , Call_Key_ClickByText);
  script->AddLibraryFunction(this, __L("InpSim_Mouse_SetPos")          , Call_Mouse_SetPos);
  script->AddLibraryFunction(this, __L("InpSim_Mouse_Click")           , Call_Mouse_Click);
      
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SCRIPT_LIB_INPUTSIMULATE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SCRIPT_LIB_INPUTSIMULATE::Clean()
{

}


/*---- LIBRARY FUNCTIONS ---------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_Key_Press(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      all  key  press
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_Key_Press(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params)  || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();

  bool status  = false;

  if(!params->GetSize())
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  int key = 0;
  
  library->GetParamConverted(params->Get(0), key);
  
  INPSIMULATE* inpsimulate = GEN_INPFACTORY.CreateSimulator();
  if(!inpsimulate)
    {
      (*returnvalue) = status;
      return;
    }
  
  status = inpsimulate->Key_Press((XBYTE)key);

  GEN_INPFACTORY.DeleteSimulator(inpsimulate);

  (*returnvalue) = status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_Key_UnPress(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call  key  un press
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_Key_UnPress(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params)  || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();

  bool status  = false;

  if(!params->GetSize())
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  int key = 0;
  
  library->GetParamConverted(params->Get(0), key);
  
  INPSIMULATE* inpsimulate = GEN_INPFACTORY.CreateSimulator();
  if(!inpsimulate)
    {
      (*returnvalue) = status;
      return;
    }
  
  status = inpsimulate->Key_UnPress((XBYTE)key);

  GEN_INPFACTORY.DeleteSimulator(inpsimulate);

  (*returnvalue) = status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_Key_Click(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_Key_Click
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_Key_Click(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params)  || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();

  bool status  = false;

  if(params->GetSize() < 2)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  int key          = 0;
  int pressuretime = 0;

  library->GetParamConverted(params->Get(0), key);
  library->GetParamConverted(params->Get(1), pressuretime);
  
  INPSIMULATE* inpsimulate = GEN_INPFACTORY.CreateSimulator();
  if(!inpsimulate)
    {
      (*returnvalue) = status;
      return;
    }
  
  status = inpsimulate->Key_Click((XBYTE)key,pressuretime);

  GEN_INPFACTORY.DeleteSimulator(inpsimulate);

  (*returnvalue) = status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_Key_ClickByLiteral(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_Key_ClickByLiteral
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_Key_ClickByLiteral(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params)  || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();

  bool status  = false;

  if(params->GetSize() < 2)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  XVARIANT* variant;
  variant = params->Get(0);

  XSTRING literal      = (*variant);
  int     pressuretime = 0;

  library->GetParamConverted(params->Get(1), pressuretime);
    
  INPSIMULATE* inpsimulate = GEN_INPFACTORY.CreateSimulator();
  if(!inpsimulate)
    {
      (*returnvalue) = status;
      return;
    }
  
  status = inpsimulate->Key_ClickByLiteral(literal.Get(), pressuretime);

  GEN_INPFACTORY.DeleteSimulator(inpsimulate);

  (*returnvalue) = status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_Key_ClickByText(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_Key_ClickByText
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_Key_ClickByText(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params)  || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();

  bool status  = false;

  if(params->GetSize() < 2)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  XVARIANT* variant;
  variant = params->Get(0);

  XSTRING text         = (*variant);
  int     pressuretime = 0;

  library->GetParamConverted(params->Get(1), pressuretime);
    
  INPSIMULATE* inpsimulate = GEN_INPFACTORY.CreateSimulator();
  if(!inpsimulate)
    {
      (*returnvalue) = status;
      return;
    }
  
  status = inpsimulate->Key_ClickByText(text.Get(), pressuretime);

  GEN_INPFACTORY.DeleteSimulator(inpsimulate);

  (*returnvalue) = status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_Mouse_SetPos(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_Mouse_SetPos
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_Mouse_SetPos(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params)  || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();

  bool status  = false;

  if(params->GetSize()<2)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  int x = 0;
  int y = 0;

  library->GetParamConverted(params->Get(0), x);
  library->GetParamConverted(params->Get(1), y);
  
  INPSIMULATE* inpsimulate = GEN_INPFACTORY.CreateSimulator();
  if(!inpsimulate)
    {
      (*returnvalue) = status;
      return;
    }
  
  status = inpsimulate->Mouse_SetPos(x, y);

  GEN_INPFACTORY.DeleteSimulator(inpsimulate);
  
  (*returnvalue) = status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_Mouse_Click(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_Mouse_Click
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_Mouse_Click(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params)  || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();

  bool status  = false;

  if(params->GetSize()<2)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  int x = 0;
  int y = 0;

  library->GetParamConverted(params->Get(0), x);
  library->GetParamConverted(params->Get(1), y);

  INPSIMULATE* inpsimulate = GEN_INPFACTORY.CreateSimulator();
  if(!inpsimulate)
    {
      (*returnvalue) = status;
      return;
    }
  
  status = inpsimulate->Mouse_Click(x, y);

  GEN_INPFACTORY.DeleteSimulator(inpsimulate);
  
  (*returnvalue) = status;
}




