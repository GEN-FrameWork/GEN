/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       Script_Lib_Path.cpp
* 
* @class      SCRIPT_LIB_PATH
* @brief      Script Library Path
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

#include "Script_Lib_Path.h"

#include "Script.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/




/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SCRIPT_LIB_PATH::SCRIPT_LIB_PATH()
* @brief      Constructor of class
* @ingroup    SCRIPT
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
SCRIPT_LIB_PATH::SCRIPT_LIB_PATH() : SCRIPT_LIB(SCRIPT_LIB_NAME_PATH)
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SCRIPT_LIB_PATH::~SCRIPT_LIB_PATH()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
SCRIPT_LIB_PATH::~SCRIPT_LIB_PATH()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT_LIB_PATH::AddLibraryFunctions(SCRIPT* script)
* @brief      Add library functions
* @ingroup    SCRIPT
* 
* @param[in]  script : Script pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT_LIB_PATH::AddLibraryFunctions(SCRIPT* script)
{
  if(!script) return false;

  this->script = script;

  script->AddLibraryFunction(this, _L("GetPathScript"), Call_GetPathScript);
  script->AddLibraryFunction(this, _L("GetNameScript"), Call_GetNameScript);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SCRIPT_LIB_PATH::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SCRIPT_LIB_PATH::Clean()
{

}




/*---- LIBRARY FUNCTIONS ---------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_GetPathScript(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      all_GetPathScript
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_GetPathScript(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  XSTRING path;

  script->GetPath()->GetDriveAndPath(path);
  
  (*returnvalue)  = path.Get();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_GetNameScript(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      all_GetNameScript
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_GetNameScript(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  XSTRING namefile;

  script->GetPath()->GetNamefile(namefile);
  
  (*returnvalue)  = namefile.Get();
}




