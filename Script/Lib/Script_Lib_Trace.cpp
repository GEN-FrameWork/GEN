/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       Script_Lib_Trace.cpp
* 
* @class      SCRIPT_LIB_Trace
* @brief      Script Library Trace
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

#include "Script_Lib_Trace.h"

#include "XFactory.h"
#include "XTrace.h"
#include "XLog.h"
#include "XConsole.h"

#include "Script.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/




/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SCRIPT_LIB_TRACE::SCRIPT_LIB_TRACE()
* @brief      Constructor of class
* @ingroup    SCRIPT
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
SCRIPT_LIB_TRACE::SCRIPT_LIB_TRACE() : SCRIPT_LIB(SCRIPT_LIB_NAME_TRACE)
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SCRIPT_LIB_TRACE::~SCRIPT_LIB_TRACE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
SCRIPT_LIB_TRACE::~SCRIPT_LIB_TRACE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT_LIB_TRACE::AddLibraryFunctions(SCRIPT* script)
* @brief      Add library functions
* @ingroup    SCRIPT
* 
* @param[in]  script : Script pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT_LIB_TRACE::AddLibraryFunctions(SCRIPT* script)
{
  if(!script) return false;

  this->script = script;

  script->AddLibraryFunction(this, _L("TraceClearScreen")         , Call_TraceClearScreen);
  script->AddLibraryFunction(this, _L("TraceClearMsgsStatus")     , Call_TraceClearMsgsStatus);
  script->AddLibraryFunction(this, _L("TracePrintColor")          , Call_TracePrintColor);
  script->AddLibraryFunction(this, _L("TracePrintMsgTests")       , Call_TracePrintMsgTests);
  script->AddLibraryFunction(this, _L("TraceTests_Load")          , Call_TraceTests_Load);
  script->AddLibraryFunction(this, _L("TraceTests_Exists")        , Call_TraceTests_Exists);
  script->AddLibraryFunction(this, _L("TraceTests_GetDescription"), Call_TraceTests_GetDescription);
  script->AddLibraryFunction(this, _L("TraceTests_DeleteAll")     , Call_TraceTests_DeleteAll);

  // Compatibility: eliminate in a future
  script->AddLibraryFunction(this, _L("XTRACE_PRINTCOLOR"), Call_TracePrintColor);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SCRIPT_LIB_TRACE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SCRIPT_LIB_TRACE::Clean()
{
 
}




/*---- LIBRARY FUNCTIONS ---------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_TraceClearScreen(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      all_TraceClearScreen
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_TraceClearScreen(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
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

  bool  recursive  = false;
  library->GetParamConverted(params->Get(0), recursive);

  if(recursive)
    {
      XTRACE_CLEARALLSCREENS;
    }
   else
    {
      XTRACE_CLEARSCREEN;
    }
  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_TraceClearMsgsStatus(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      all_TraceClearMsgsStatus
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_TraceClearMsgsStatus(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
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

  bool  recursive  = false;
  library->GetParamConverted(params->Get(0), recursive);

  if(recursive)
    {
      XTRACE_CLEARALLMSGSSTATUS;
    }
   else
    {
      XTRACE_CLEARMSGSSTATUS;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_TracePrintColor(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      Call_TracePrintColor
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_TracePrintColor(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
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

  XDWORD    color       = 0;
  library->GetParamConverted(params->Get(0), color);

  XVARIANT  variantmask = (*params->Get(1));
  XCHAR*    mask = variantmask;
  XSTRING   outstring;
  XSTRING   string;

  int paramindex = 2;
  int c          = 0;

  if(!mask)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  while(mask[c])
    {
      switch(mask[c])
        {
          case '%' : {
                        #define MAXTEMPOSTR 32

                        XCHAR param[MAXTEMPOSTR];

                        int  nparam = 1;
                        bool end    = false;

                        memset(param, 0, MAXTEMPOSTR*sizeof(XCHAR));
                        param[0] = '%';

                        c++;

                        do{ string.Empty();

                            param[nparam] = mask[c];
                            nparam++;

                            switch(mask[c])
                              {
                                case _C('c')   :
                                case _C('C')   :
                                case _C('d')   :
                                case _C('i')   :
                                case _C('o')   :
                                case _C('u')   :
                                case _C('x')   :
                                case _C('X')   : { int value = 0;
                                                    library->GetParamConverted(params->Get(paramindex), value);
                                                    string.Format(param, value);
                                                    paramindex++;
                                                    end  = true;
                                                  }
                                                  break;

                                case _C('f')   : { float value = 0.0f;
                                                    library->GetParamConverted(params->Get(paramindex), value);
                                                    string.Format(param, value);
                                                    paramindex++;
                                                    end  = true;
                                                  }
                                                  break;

                                case _C('g')   :
                                case _C('G')   :

                                case _C('e')   :
                                case _C('E')   :

                                case _C('n')   :
                                case _C('p')   : end = true;
                                                  break;

                                case _C('s')   :
                                case _C('S')   : { XVARIANT variantparam = (*params->Get(paramindex));
                                                    paramindex++;
                                                    // Pass data as a string value — do not re-parse '%' inside it.
                                                    string = (XCHAR*)variantparam;
                                                    end = true;
                                                  }
                                                  break;

                                case _C('%')   : string = _L("%");
                                                  end = true;
                                                  break;

                                case _C('\0')  : end = true;
                                                  break;

                                      default   : break;
                              }

                            c++;

                          } while(!end);
                      }
                      break;

            default : string.Set(mask[c]);
                      c++;
                      break;
        }

      outstring += string;
    }

 XTRACE_PRINTCOLOR(color, outstring.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_TracePrintMsgTests(...)
* @brief      TracePrintMsgTests(id, error) → bool
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_TracePrintMsgTests(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  (*returnvalue) = false;

  if(params->GetSize() < 2)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  XDWORD id    = 0;
  int    error = 0;

  library->GetParamConverted(params->Get(0), id);
  library->GetParamConverted(params->Get(1), error);

  #ifdef XTRACE_ACTIVE
  if(XTRACE::instance)
    {
      (*returnvalue) = XTRACE::instance->PrintMsgTests(id, error);
    }
  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_TraceTests_Load(...)
* @brief      TraceTests_Load(path) → bool
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_TraceTests_Load(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  (*returnvalue) = false;

  if(!params->GetSize())
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  XSTRING path;

  if(!library->GetParamConverted(params->Get(0), path) || path.IsEmpty())
    {
      return;
    }

  #ifdef XTRACE_ACTIVE
  if(XTRACE::instance)
    {
      (*returnvalue) = XTRACE::instance->Tests_Load(path);
    }
  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_TraceTests_Exists(...)
* @brief      TraceTests_Exists(id) → bool
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_TraceTests_Exists(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  (*returnvalue) = false;

  if(!params->GetSize())
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  XDWORD id = 0;

  library->GetParamConverted(params->Get(0), id);

  #ifdef XTRACE_ACTIVE
  if(XTRACE::instance)
    {
      (*returnvalue) = XTRACE::instance->Tests_Exists(id);
    }
  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_TraceTests_GetDescription(...)
* @brief      TraceTests_GetDescription(id) → string (empty if missing)
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_TraceTests_GetDescription(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
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

  XDWORD  id = 0;
  XSTRING description;

  library->GetParamConverted(params->Get(0), id);

  #ifdef XTRACE_ACTIVE
  if(XTRACE::instance)
    {
      if(XTRACE::instance->Tests_GetDescription(id, description))
        {
          (*returnvalue) = description.Get();
        }
    }
  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_TraceTests_DeleteAll(...)
* @brief      TraceTests_DeleteAll() → bool — free loaded tests catalog from memory
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_TraceTests_DeleteAll(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  (*returnvalue) = false;

  #ifdef XTRACE_ACTIVE
  if(XTRACE::instance)
    {
      (*returnvalue) = XTRACE::instance->Tests_DeleteAll();
    }
  #endif
}

