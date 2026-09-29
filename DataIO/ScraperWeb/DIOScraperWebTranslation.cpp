/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOScraperWebTranslation.cpp
* 
* @class      DIOSCRAPERWEBTRANSLATION
* @brief      Typed Translation scraper filled by script (not XML)
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

#ifdef DIO_SCRAPERWEB_TRANSLATION_ACTIVE

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOScraperWebTranslation.h"

#include "XFactory.h"
#include "XThread.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/




/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOTRANSLATION_RESULT::DIOTRANSLATION_RESULT()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOTRANSLATION_RESULT::DIOTRANSLATION_RESULT(): DIOSCRAPERWEBCACHE_RESULT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOTRANSLATION_RESULT::~DIOTRANSLATION_RESULT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOTRANSLATION_RESULT::~DIOTRANSLATION_RESULT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* DIOTRANSLATION_RESULT::GetSourceLanguage()
* @brief      Get source language
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* DIOTRANSLATION_RESULT::GetSourceLanguage()
{
  return sourcelanguage.Get();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* DIOTRANSLATION_RESULT::GetTargetLanguage()
* @brief      Get target language
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* DIOTRANSLATION_RESULT::GetTargetLanguage()
{
  return targetlanguage.Get();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* DIOTRANSLATION_RESULT::GetTranslation()
* @brief      Get translation
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* DIOTRANSLATION_RESULT::GetTranslation()
{
  return translation.Get();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOTRANSLATION_RESULT::IsEmpty()
* @brief      Is empty
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOTRANSLATION_RESULT::IsEmpty()
{
  if(translation.IsEmpty()) return true;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOTRANSLATION_RESULT::CopyFrom(DIOSCRAPERWEBCACHE_RESULT* result)
* @brief      Copy from
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOTRANSLATION_RESULT::CopyFrom(DIOSCRAPERWEBCACHE_RESULT* result)
{
  if(!result) return false;

  DIOTRANSLATION_RESULT* translationresult = (DIOTRANSLATION_RESULT*)result;

  if(translationresult->IsEmpty()) return false;

  sourcelanguage = translationresult->sourcelanguage;
  targetlanguage = translationresult->targetlanguage;
  translation    = translationresult->translation;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOTRANSLATION_RESULT::CopyTo(DIOSCRAPERWEBCACHE_RESULT* result)
* @brief      Copy to
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOTRANSLATION_RESULT::CopyTo(DIOSCRAPERWEBCACHE_RESULT* result)
{
  if(!result)   return false;
  if(IsEmpty()) return false;

  DIOTRANSLATION_RESULT* translationresult = (DIOTRANSLATION_RESULT*)result;

  translationresult->sourcelanguage = sourcelanguage;
  translationresult->targetlanguage = targetlanguage;
  translationresult->translation    = translation;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOTRANSLATION_RESULT::Set(XSTRING& sourcelanguage, XSTRING& targetlanguage, XSTRING& translation)
* @brief      Set value
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOTRANSLATION_RESULT::Set(XSTRING& sourcelanguage, XSTRING& targetlanguage, XSTRING& translation)
{
  return Set(sourcelanguage.Get(), targetlanguage.Get(), translation.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOTRANSLATION_RESULT::Set(XCHAR* sourcelanguage, XCHAR* targetlanguage, XCHAR* translation)
* @brief      Set value
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOTRANSLATION_RESULT::Set(XCHAR* sourcelanguage, XCHAR* targetlanguage, XCHAR* translation)
{
  if(sourcelanguage) this->sourcelanguage = sourcelanguage;
  if(targetlanguage) this->targetlanguage = targetlanguage;
  if(translation)    this->translation    = translation;

  XCHAR character[3] = { 0x09, 0x0A, 0x0D };

  for(int c=0;c<3;c++)
    {
      this->sourcelanguage.DeleteCharacter(character[c], XSTRINGCONTEXT_ALLSTRING);
      this->targetlanguage.DeleteCharacter(character[c], XSTRINGCONTEXT_ALLSTRING);
      this->translation.DeleteCharacter(character[c], XSTRINGCONTEXT_ALLSTRING);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOTRANSLATION_RESULT::Clean()
* @brief      Clean
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOTRANSLATION_RESULT::Clean()
{

}




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSCRAPERWEBTRANSLATION::DIOSCRAPERWEBTRANSLATION()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSCRAPERWEBTRANSLATION::DIOSCRAPERWEBTRANSLATION()
{
  Clean();

  cache      = GEN_NEW DIOSCRAPERWEBCACHE();
  xmutexdo   = GEN_XFACTORY.Create_Mutex();
  scriptpath = DIOSCRAPERWEBTRANSLATION_SCRIPTPATH;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSCRAPERWEBTRANSLATION::~DIOSCRAPERWEBTRANSLATION()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSCRAPERWEBTRANSLATION::~DIOSCRAPERWEBTRANSLATION()
{
  if(cache)
    {
      cache->DeleteAll();
      GEN_DELETE cache;
    }

  if(xmutexdo)
    {
      GEN_XFACTORY.Delete_Mutex(xmutexdo);
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEBTRANSLATION::Get(XCHAR* text, XCHAR* sourcelanguage, XCHAR* targetlanguage, DIOTRANSLATION_RESULT& result, ...)
* @brief      Get translation via scraper script
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEBTRANSLATION::Get(XCHAR* text, XCHAR* sourcelanguage, XCHAR* targetlanguage, DIOTRANSLATION_RESULT& result, int timeoutforurl, XSTRING* localIP, bool usecache)
{
  bool    status = false;
  XSTRING cacheask;
  XSTRING sl;
  XSTRING tl;

  if((!text) || (!text[0]))           return false;
  if((!targetlanguage) || (!targetlanguage[0])) return false;

  sl = (sourcelanguage && sourcelanguage[0]) ? sourcelanguage : __L("auto");
  tl = targetlanguage;

  if(xmutexdo) xmutexdo->Lock();

  cacheask  = text;
  cacheask += __L("|");
  cacheask += sl;
  cacheask += __L("|");
  cacheask += tl;

  if(usecache && cache)
    {
      DIOTRANSLATION_RESULT* cached = (DIOTRANSLATION_RESULT*)cache->Get(cacheask);
      if(cached)
        {
          result.CopyFrom(cached);

          if(xmutexdo) xmutexdo->UnLock();
          return true;
        }
    }

  DIOSCRAPERSCRIPT runner;

  runner.SetArg(__L("text"), text);
  runner.SetArg(__L("sl"), sl);
  runner.SetArg(__L("tl"), tl);
  runner.SetArgInt(__L("timeout"), timeoutforurl);
  if(localIP && (!localIP->IsEmpty()))
    {
      runner.SetArg(__L("localIP"), (*localIP));
    }

  if(runner.Run(scriptpath.Get()))
    {
      XSTRING ok;
      XSTRING translation;
      XSTRING detected;

      runner.GetResult(__L("ok"), ok);
      runner.GetResult(__L("translation"), translation);
      runner.GetResult(__L("detected"), detected);

      if((ok.Compare(__L("1")) == 0) && (!translation.IsEmpty()))
        {
          if(detected.IsEmpty()) detected = sl;

          result.Set(detected, tl, translation);

          if(!result.IsEmpty())
            {
              if(usecache && cache)
                {
                  DIOTRANSLATION_RESULT* entry = GEN_NEW DIOTRANSLATION_RESULT();
                  if(entry)
                    {
                      entry->CopyFrom(&result);
                      cache->Add(cacheask, entry);
                    }
                }

              status = true;
            }
        }
    }

  if(xmutexdo) xmutexdo->UnLock();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEBTRANSLATION::Get(XSTRING& text, XSTRING& sourcelanguage, XSTRING& targetlanguage, DIOTRANSLATION_RESULT& result, ...)
* @brief      Get translation via scraper script
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEBTRANSLATION::Get(XSTRING& text, XSTRING& sourcelanguage, XSTRING& targetlanguage, DIOTRANSLATION_RESULT& result, int timeoutforurl, XSTRING* localIP, bool usecache)
{
  return Get(text.Get(), sourcelanguage.Get(), targetlanguage.Get(), result, timeoutforurl, localIP, usecache);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEBTRANSLATION::Get(XCHAR* text, XCHAR* sourcelanguage, XCHAR* targetlanguage, XSTRING& translation, ...)
* @brief      Legacy-compatible out-param Get
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEBTRANSLATION::Get(XCHAR* text, XCHAR* sourcelanguage, XCHAR* targetlanguage, XSTRING& translation, int timeoutforurl, XSTRING* localIP, bool usecache)
{
  DIOTRANSLATION_RESULT result;

  if(!Get(text, sourcelanguage, targetlanguage, result, timeoutforurl, localIP, usecache)) return false;

  translation = result.GetTranslation();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEBTRANSLATION::Get(XSTRING& text, XSTRING& sourcelanguage, XSTRING& targetlanguage, XSTRING& translation, ...)
* @brief      Legacy-compatible out-param Get
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEBTRANSLATION::Get(XSTRING& text, XSTRING& sourcelanguage, XSTRING& targetlanguage, XSTRING& translation, int timeoutforurl, XSTRING* localIP, bool usecache)
{
  return Get(text.Get(), sourcelanguage.Get(), targetlanguage.Get(), translation, timeoutforurl, localIP, usecache);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEBTRANSLATION::SetScriptPath(XCHAR* relativescriptpath)
* @brief      SetScriptPath
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEBTRANSLATION::SetScriptPath(XCHAR* relativescriptpath)
{
  if((!relativescriptpath) || (!relativescriptpath[0])) return false;

  scriptpath = relativescriptpath;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* DIOSCRAPERWEBTRANSLATION::GetScriptPath()
* @brief      GetScriptPath
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* DIOSCRAPERWEBTRANSLATION::GetScriptPath()
{
  return scriptpath.Get();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSCRAPERWEBTRANSLATION::Clean()
* @brief      Clean
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSCRAPERWEBTRANSLATION::Clean()
{
  cache    = NULL;
  xmutexdo = NULL;
}

#endif
