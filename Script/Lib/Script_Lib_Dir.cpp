/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       Script_Lib_Dir.cpp
* 
* @class      SCRIPT_LIB_DIR
* @brief      Script Library Dir
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

#include "Script_Lib_Dir.h"

#include "XFactory.h"
#include "XDir.h"

#include "Script.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/




/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SCRIPT_LIB_DIR::SCRIPT_LIB_DIR()
* @brief      Constructor of class
* @ingroup    SCRIPT
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
SCRIPT_LIB_DIR::SCRIPT_LIB_DIR() : SCRIPT_LIB(SCRIPT_LIB_NAME_DIR)
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SCRIPT_LIB_DIR::~SCRIPT_LIB_DIR()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
SCRIPT_LIB_DIR::~SCRIPT_LIB_DIR()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT_LIB_DIR::AddLibraryFunctions(SCRIPT* script)
* @brief      Add library functions
* @ingroup    SCRIPT
* 
* @param[in]  script : Script pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT_LIB_DIR::AddLibraryFunctions(SCRIPT* script)
{
  if(!script) return false;

  this->script = script;

  script->AddLibraryFunction(this, _L("IsItExists"), Call_IsItExists);
  script->AddLibraryFunction(this, _L("ChangeDir"), Call_ChangeDir);
  script->AddLibraryFunction(this, _L("RemoveDir"), Call_RemoveDir);
  script->AddLibraryFunction(this, _L("MakeDir"), Call_MakeDir);
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SCRIPT_LIB_DIR::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SCRIPT_LIB_DIR::Clean()
{
  
}


/*---- LIBRARY FUNCTIONS ---------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_IsItExists(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_IsItExists
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_IsItExists(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  XDIR* dir     = NULL;
  bool  status  = false;
  
  dir = GEN_XFACTORY.Create_Dir();
  if(dir)
    {     
      if(!params->GetSize())
        {
          script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
          return;
        }

      XSTRING* string = (XSTRING*)params->Get(0)->GetData();  
      if(string)
        {
          if(!string->IsEmpty())
            {
              status = dir->Exist(string->Get());
            }        
        }
    
      GEN_XFACTORY.Delete_Dir(dir);
    }

  (*returnvalue) = status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_ChangeDir(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_ChangeDir
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_ChangeDir(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  XDIR* dir     = NULL;
  bool  status  = false;
  
  dir = GEN_XFACTORY.Create_Dir();
  if(dir)
    {     
      if(!params->GetSize())
        {
          script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
          return;
        }

      XSTRING* string = (XSTRING*)params->Get(0)->GetData();  
      if(string)
        {
          if(!string->IsEmpty())
            {
              status = dir->ChangeTo(string->Get());
            }        
        }
    
      GEN_XFACTORY.Delete_Dir(dir);
    }

  (*returnvalue) = status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_RemoveDir(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      all_RemoveDir
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_RemoveDir(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  XDIR* dir     = NULL;
  bool  status  = false;
  
  dir = GEN_XFACTORY.Create_Dir();
  if(dir)
    {     
      if(!params->GetSize())
        {
          script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
          return;
        }

      XSTRING* string = (XSTRING*)params->Get(0)->GetData();  
      if(string)
        {
          if(!string->IsEmpty())
            {
              status = dir->Delete(string->Get(), true);
            }        
        }
    
      GEN_XFACTORY.Delete_Dir(dir);
    }

  (*returnvalue) = status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_MakeDir(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      all_MakeDir
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_MakeDir(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  XDIR* dir     = NULL;
  bool  status  = false;
  
  dir = GEN_XFACTORY.Create_Dir();
  if(dir)
    {     
      if(!params->GetSize())
        {
          script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
          return;
        }

      XSTRING* string = (XSTRING*)params->Get(0)->GetData();  
      if(string)
        {
          if(!string->IsEmpty())
            {
              status = dir->Make(string->Get(), true);
            }        
        }
    
      GEN_XFACTORY.Delete_Dir(dir);
    }

  (*returnvalue) = status;
}



