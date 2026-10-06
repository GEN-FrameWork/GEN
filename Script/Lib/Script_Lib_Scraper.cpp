/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       Script_Lib_Scraper.cpp
* 
* @class      SCRIPT_LIB_SCRAPER
* @brief      Script Library Scraper
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

#include "Script_Lib_Scraper.h"

#include "DIOScraperScript.h"

#include "Script.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/




/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SCRIPT_LIB_SCRAPER::SCRIPT_LIB_SCRAPER()
* @brief      Constructor of class
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
SCRIPT_LIB_SCRAPER::SCRIPT_LIB_SCRAPER() : SCRIPT_LIB(SCRIPT_LIB_NAME_SCRAPER)
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SCRIPT_LIB_SCRAPER::~SCRIPT_LIB_SCRAPER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
SCRIPT_LIB_SCRAPER::~SCRIPT_LIB_SCRAPER()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT_LIB_SCRAPER::AddLibraryFunctions(SCRIPT* script)
* @brief      Add library functions
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT_LIB_SCRAPER::AddLibraryFunctions(SCRIPT* script)
{
  if(!script)
    {
      return false;
    }

  this->script = script;

  script->AddLibraryFunction(this, _L("Scraper_GetArg")   , Call_Scraper_GetArg);
  script->AddLibraryFunction(this, _L("Scraper_GetArgInt"), Call_Scraper_GetArgInt);
  script->AddLibraryFunction(this, _L("Scraper_SetResult"), Call_Scraper_SetResult);
  script->AddLibraryFunction(this, _L("Scraper_GetResult"), Call_Scraper_GetResult);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SCRIPT_LIB_SCRAPER::SetContext(DIOSCRAPERSCRIPT* context)
* @brief      SetContext
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SCRIPT_LIB_SCRAPER::SetContext(DIOSCRAPERSCRIPT* context)
{
  this->context = context;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSCRAPERSCRIPT* SCRIPT_LIB_SCRAPER::GetContext()
* @brief      GetContext
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSCRAPERSCRIPT* SCRIPT_LIB_SCRAPER::GetContext()
{
  return context;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SCRIPT_LIB_SCRAPER::Clean()
* @brief      Clean
* @note       INTERNAL
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SCRIPT_LIB_SCRAPER::Clean()
{
  context = NULL;
}




/*---- LIBRARY FUNCTIONS ---------------------------------------------------------------------------------------------*/


static SCRIPT_LIB_SCRAPER* ScriptLibScraper_AsLib(SCRIPT_LIB* library)
{
  return (SCRIPT_LIB_SCRAPER*)library;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_Scraper_GetArg(...)
* @brief      Scraper_GetArg(name) → string
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_Scraper_GetArg(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params) || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();

  if(params->GetSize() < 1)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  SCRIPT_LIB_SCRAPER* scraperlib = ScriptLibScraper_AsLib(library);
  DIOSCRAPERSCRIPT*   context    = scraperlib ? scraperlib->GetContext() : NULL;
  XSTRING             name;
  XSTRING             value;

  if(!library->GetParamConverted(params->Get(0), name) || (!context))
    {
      (*returnvalue) = value;
      return;
    }

  context->GetArg(name.Get(), value);
  (*returnvalue) = value;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_Scraper_GetArgInt(...)
* @brief      Scraper_GetArgInt(name) → int (0 if missing/invalid)
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_Scraper_GetArgInt(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params) || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();
  (*returnvalue) = 0;

  if(params->GetSize() < 1)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  SCRIPT_LIB_SCRAPER* scraperlib = ScriptLibScraper_AsLib(library);
  DIOSCRAPERSCRIPT*   context    = scraperlib ? scraperlib->GetContext() : NULL;
  XSTRING             name;
  int                 value = 0;

  if(!library->GetParamConverted(params->Get(0), name) || (!context))
    {
      return;
    }

  context->GetArgInt(name.Get(), value);
  (*returnvalue) = value;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_Scraper_SetResult(...)
* @brief      Scraper_SetResult(name, value) → bool
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_Scraper_SetResult(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params) || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();
  (*returnvalue) = false;

  if(params->GetSize() < 2)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  SCRIPT_LIB_SCRAPER* scraperlib = ScriptLibScraper_AsLib(library);
  DIOSCRAPERSCRIPT*   context    = scraperlib ? scraperlib->GetContext() : NULL;
  XSTRING             name;
  XSTRING             value;

  if(!context)
    {
      return;
    }

  if(!library->GetParamConverted(params->Get(0), name))
    {
      return;
    }

  if(!library->GetParamConverted(params->Get(1), value))
    {
      return;
    }

  (*returnvalue) = context->SetResult(name.Get(), value);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_Scraper_GetResult(...)
* @brief      Scraper_GetResult(name) → string
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_Scraper_GetResult(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params) || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();

  if(params->GetSize() < 1)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  SCRIPT_LIB_SCRAPER* scraperlib = ScriptLibScraper_AsLib(library);
  DIOSCRAPERSCRIPT*   context    = scraperlib ? scraperlib->GetContext() : NULL;
  XSTRING             name;
  XSTRING             value;

  if(!library->GetParamConverted(params->Get(0), name) || (!context))
    {
      (*returnvalue) = value;
      return;
    }

  context->GetResult(name.Get(), value);
  (*returnvalue) = value;
}
