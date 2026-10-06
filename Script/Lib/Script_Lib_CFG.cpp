/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       Script_Lib_CFG.cpp
* 
* @class      SCRIPT_LIB_CFG
* @brief      Script Library CFG
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

#include "Script_Lib_CFG.h"

#include "XFileCFG.h"

#include "Script.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/




/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SCRIPT_LIB_CFG::SCRIPT_LIB_CFG()
* @brief      Constructor of class
* @ingroup    SCRIPT
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
SCRIPT_LIB_CFG::SCRIPT_LIB_CFG() : SCRIPT_LIB(SCRIPT_LIB_NAME_CFG)
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SCRIPT_LIB_CFG::~SCRIPT_LIB_CFG()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
SCRIPT_LIB_CFG::~SCRIPT_LIB_CFG()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT_LIB_CFG::AddLibraryFunctions(SCRIPT* script)
* @brief      Add library functions
* @ingroup    SCRIPT
* 
* @param[in]  script : Script pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT_LIB_CFG::AddLibraryFunctions(SCRIPT* script)
{
  if(!script) return false;

  this->script = script;

  script->AddLibraryFunction(this, _L("GetFileCFGValue"), Call_GetFileCFGValue);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XFILECFG* SCRIPT_LIB_CFG::GetXFileCFG()
* @brief      Get X file CFG
* @ingroup    SCRIPT
* 
* @return     XFILECFG* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XFILECFG* SCRIPT_LIB_CFG::GetXFileCFG()
{
  return xfileCFG;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SCRIPT_LIB_CFG::SetXFileCFG(XFILECFG* xfileCFG)
* @brief      Set X file CFG
* @ingroup    SCRIPT
* 
* @param[in]  xfileCFG : Xfile configuration pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SCRIPT_LIB_CFG::SetXFileCFG(XFILECFG* xfileCFG)
{
  this->xfileCFG = xfileCFG;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SCRIPT_LIB_CFG::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SCRIPT_LIB_CFG::Clean()
{
  xfileCFG = NULL;
}




/*---- LIBRARY FUNCTIONS ---------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_GetFileCFGValue(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_GetFileCFGValue
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_GetFileCFGValue(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  if(params->GetSize() < 2)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }
  
  XSTRING          datakey;
  SCRIPT_LIB_CFG*  libraryCFG = (SCRIPT_LIB_CFG*)library;

  if(libraryCFG->GetXFileCFG())
    {
      XSTRING* param1 = (XSTRING*)params->Get(0)->GetData();
      XSTRING* param2 = (XSTRING*)params->Get(1)->GetData();

      XVARIANT* variable = libraryCFG->GetXFileCFG()->GetValue(param1->Get(), param2->Get());
      if(variable) 
        {
          variable->ToString(datakey);        
          GEN_DELETE variable;
        }

    }

  (*returnvalue) = datakey.Get();
}


