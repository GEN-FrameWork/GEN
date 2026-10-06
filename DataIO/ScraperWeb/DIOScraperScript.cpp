/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOScraperScript.cpp
* @class      DIOSCRAPERSCRIPT
* @brief      Scraper script runner
* @ingroup    DATAIO
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

#include "DIOScraperScript.h"

#include "Script.h"
#include "Script_Lib_Scraper.h"
#include "Script_ErrorCode.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/




/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSCRAPERSCRIPT::DIOSCRAPERSCRIPT()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSCRAPERSCRIPT::DIOSCRAPERSCRIPT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSCRAPERSCRIPT::~DIOSCRAPERSCRIPT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSCRAPERSCRIPT::~DIOSCRAPERSCRIPT()
{
  ClearArgs();
  ClearResults();
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERSCRIPT::ClearArgs()
* @brief      ClearArgs
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERSCRIPT::ClearArgs()
{
  return DeleteMapContents(args);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERSCRIPT::ClearResults()
* @brief      ClearResults
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERSCRIPT::ClearResults()
{
  return DeleteMapContents(results);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERSCRIPT::SetArg(XCHAR* name, XCHAR* value)
* @brief      SetArg
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERSCRIPT::SetArg(XCHAR* name, XCHAR* value)
{
  XSTRING stringvalue;

  if(value) stringvalue = value;
  return SetArg(name, stringvalue);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERSCRIPT::SetArg(XCHAR* name, XSTRING& value)
* @brief      SetArg
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERSCRIPT::SetArg(XCHAR* name, XSTRING& value)
{
  if((!name) || (!name[0])) return false;

  XSTRING* existing = NULL;
  XSTRING  key      = name;

  for(XDWORD c=0; c<args.GetSize(); c++)
    {
      XSTRING* k = args.GetKey(c);
      if(k && (k->Compare(name) == 0))
        {
          existing = args.GetElement(c);
          if(existing)
            {
              (*existing) = value;
              return true;
            }
        }
    }

  XSTRING* newkey   = GEN_NEW XSTRING(name);
  XSTRING* newvalue = GEN_NEW XSTRING(value.Get());

  if((!newkey) || (!newvalue))
    {
      GEN_DELETE newkey;
      GEN_DELETE newvalue;
      return false;
    }

  return args.Add(newkey, newvalue);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERSCRIPT::SetArgInt(XCHAR* name, int value)
* @brief      SetArgInt
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERSCRIPT::SetArgInt(XCHAR* name, int value)
{
  XSTRING stringvalue;

  stringvalue.Format(_L("%d"), value);
  return SetArg(name, stringvalue);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERSCRIPT::GetArg(XCHAR* name, XSTRING& value)
* @brief      GetArg
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERSCRIPT::GetArg(XCHAR* name, XSTRING& value)
{
  value.Empty();
  if((!name) || (!name[0])) return false;

  for(XDWORD c=0; c<args.GetSize(); c++)
    {
      XSTRING* k = args.GetKey(c);
      if(k && (k->Compare(name) == 0))
        {
          XSTRING* v = args.GetElement(c);
          if(v)
            {
              value = (*v);
              return true;
            }
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERSCRIPT::GetArgInt(XCHAR* name, int& value)
* @brief      GetArgInt
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERSCRIPT::GetArgInt(XCHAR* name, int& value)
{
  XSTRING stringvalue;

  value = 0;
  if(!GetArg(name, stringvalue)) return false;

  value = stringvalue.ConvertToInt();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERSCRIPT::SetResult(XCHAR* name, XCHAR* value)
* @brief      SetResult
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERSCRIPT::SetResult(XCHAR* name, XCHAR* value)
{
  XSTRING stringvalue;

  if(value) stringvalue = value;
  return SetResult(name, stringvalue);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERSCRIPT::SetResult(XCHAR* name, XSTRING& value)
* @brief      SetResult
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERSCRIPT::SetResult(XCHAR* name, XSTRING& value)
{
  if((!name) || (!name[0])) return false;

  for(XDWORD c=0; c<results.GetSize(); c++)
    {
      XSTRING* k = results.GetKey(c);
      if(k && (k->Compare(name) == 0))
        {
          XSTRING* existing = results.GetElement(c);
          if(existing)
            {
              (*existing) = value;
              return true;
            }
        }
    }

  XSTRING* newkey   = GEN_NEW XSTRING(name);
  XSTRING* newvalue = GEN_NEW XSTRING(value.Get());

  if((!newkey) || (!newvalue))
    {
      GEN_DELETE newkey;
      GEN_DELETE newvalue;
      return false;
    }

  return results.Add(newkey, newvalue);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERSCRIPT::GetResult(XCHAR* name, XSTRING& value)
* @brief      GetResult
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERSCRIPT::GetResult(XCHAR* name, XSTRING& value)
{
  value.Empty();
  if((!name) || (!name[0])) return false;

  for(XDWORD c=0; c<results.GetSize(); c++)
    {
      XSTRING* k = results.GetKey(c);
      if(k && (k->Compare(name) == 0))
        {
          XSTRING* v = results.GetElement(c);
          if(v)
            {
              value = (*v);
              return true;
            }
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERSCRIPT::HaveResult(XCHAR* name)
* @brief      HaveResult
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERSCRIPT::HaveResult(XCHAR* name)
{
  XSTRING unused;

  return GetResult(name, unused);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERSCRIPT::Run(XCHAR* relativescriptpath)
* @brief      Run
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERSCRIPT::Run(XCHAR* relativescriptpath)
{
  #ifndef SCRIPT_ACTIVE
  return false;
  #else

  if((!relativescriptpath) || (!relativescriptpath[0])) return false;

  XPATH scriptpath;

  if(!SCRIPT::ResolvePathInScriptsRoot(relativescriptpath, scriptpath))
    {
      return false;
    }

  return RunPath(scriptpath);

  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERSCRIPT::RunPath(XPATH& scriptpath)
* @brief      RunPath
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERSCRIPT::RunPath(XPATH& scriptpath)
{
  #ifndef SCRIPT_ACTIVE
  return false;
  #else

  #if defined(SCRIPT_LIB_SANDBOX_ACTIVE)
  return false;
  #endif

  if(scriptpath.IsEmpty()) return false;

  ClearResults();

  SCRIPT* script = SCRIPT::Create(scriptpath.Get());
  if(!script) return false;

  SCRIPT_LIB_SCRAPER* scraperlibridge = GEN_NEW SCRIPT_LIB_SCRAPER();
  if(!scraperlibridge)
    {
      GEN_DELETE script;
      return false;
    }

  scraperlibridge->SetContext(this);
  if(!script->AddLibrary(scraperlibridge))
    {
      GEN_DELETE scraperlibridge;
      GEN_DELETE script;
      return false;
    }

  if(!script->Load(scriptpath))
    {
      GEN_DELETE script;
      return false;
    }

  int returnvalue = 0;
  int errorcode   = script->Run(&returnvalue);

  GEN_DELETE script;

  return (errorcode == SCRIPT_ERRORCODE_NONE);

  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERSCRIPT::DeleteMapContents(XMAP<XSTRING*, XSTRING*>& map)
* @brief      DeleteMapContents
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERSCRIPT::DeleteMapContents(XMAP<XSTRING*, XSTRING*>& map)
{
  for(XDWORD c=0; c<map.GetSize(); c++)
    {
      XSTRING* key   = map.GetKey(c);
      XSTRING* value = map.GetElement(c);

      GEN_DELETE key;
      GEN_DELETE value;
    }

  map.DeleteAll();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSCRAPERSCRIPT::Clean()
* @brief      Clean
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSCRAPERSCRIPT::Clean()
{

}
