/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOScraperWebUserAgentID.cpp
* 
* @class      DIOSCRAPERWEBUSERAGENTID
* @brief      Typed User-Agent scraper filled by script (not XML)
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

#ifdef DIO_SCRAPERWEB_USERAGENTID_ACTIVE

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOScraperWebUserAgentID.h"

#include "XFactory.h"
#include "XThread.h"

#include "DIOURL.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/




/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOUSERAGENTID_RESULT::DIOUSERAGENTID_RESULT()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOUSERAGENTID_RESULT::DIOUSERAGENTID_RESULT(): DIOSCRAPERWEBCACHE_RESULT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOUSERAGENTID_RESULT::~DIOUSERAGENTID_RESULT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOUSERAGENTID_RESULT::~DIOUSERAGENTID_RESULT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* DIOUSERAGENTID_RESULT::GetBrowser()
* @brief      Get browser
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* DIOUSERAGENTID_RESULT::GetBrowser()
{
  return browser.Get();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* DIOUSERAGENTID_RESULT::GetBrowserVersion()
* @brief      Get browser version
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* DIOUSERAGENTID_RESULT::GetBrowserVersion()
{
  return browserversion.Get();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* DIOUSERAGENTID_RESULT::GetBrowserType()
* @brief      Get browser type
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* DIOUSERAGENTID_RESULT::GetBrowserType()
{
  return browsertype.Get();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* DIOUSERAGENTID_RESULT::GetSO()
* @brief      Get operative system (legacy name)
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* DIOUSERAGENTID_RESULT::GetSO()
{
  return systemoperative.Get();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* DIOUSERAGENTID_RESULT::GetOSType()
* @brief      Get OS type
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* DIOUSERAGENTID_RESULT::GetOSType()
{
  return ostype.Get();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* DIOUSERAGENTID_RESULT::GetOSVersion()
* @brief      Get OS version
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* DIOUSERAGENTID_RESULT::GetOSVersion()
{
  return osversion.Get();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* DIOUSERAGENTID_RESULT::GetLanguage()
* @brief      Get language
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* DIOUSERAGENTID_RESULT::GetLanguage()
{
  return language.Get();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* DIOUSERAGENTID_RESULT::GetLanguageTag()
* @brief      Get language tag
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* DIOUSERAGENTID_RESULT::GetLanguageTag()
{
  return languagetag.Get();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOUSERAGENTID_RESULT::IsEmpty()
* @brief      Is empty
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOUSERAGENTID_RESULT::IsEmpty()
{
  if(browser.IsEmpty()) return true;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOUSERAGENTID_RESULT::CopyFrom(DIOSCRAPERWEBCACHE_RESULT* result)
* @brief      Copy from
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOUSERAGENTID_RESULT::CopyFrom(DIOSCRAPERWEBCACHE_RESULT* result)
{
  if(!result) return false;

  DIOUSERAGENTID_RESULT* uaresult = (DIOUSERAGENTID_RESULT*)result;

  if(uaresult->IsEmpty()) return false;

  browser          = uaresult->browser;
  browserversion   = uaresult->browserversion;
  browsertype      = uaresult->browsertype;
  systemoperative  = uaresult->systemoperative;
  ostype           = uaresult->ostype;
  osversion        = uaresult->osversion;
  language         = uaresult->language;
  languagetag      = uaresult->languagetag;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOUSERAGENTID_RESULT::CopyTo(DIOSCRAPERWEBCACHE_RESULT* result)
* @brief      Copy to
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOUSERAGENTID_RESULT::CopyTo(DIOSCRAPERWEBCACHE_RESULT* result)
{
  if(!result)   return false;
  if(IsEmpty()) return false;

  DIOUSERAGENTID_RESULT* uaresult = (DIOUSERAGENTID_RESULT*)result;

  uaresult->browser          = browser;
  uaresult->browserversion   = browserversion;
  uaresult->browsertype      = browsertype;
  uaresult->systemoperative  = systemoperative;
  uaresult->ostype           = ostype;
  uaresult->osversion        = osversion;
  uaresult->language         = language;
  uaresult->languagetag      = languagetag;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOUSERAGENTID_RESULT::Set(...)
* @brief      Set value
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOUSERAGENTID_RESULT::Set(XCHAR* browser, XCHAR* browserversion, XCHAR* browsertype, XCHAR* so, XCHAR* ostype, XCHAR* osversion, XCHAR* language, XCHAR* languagetag)
{
  if(browser)        this->browser         = browser;
  if(browserversion) this->browserversion  = browserversion;
  if(browsertype)    this->browsertype     = browsertype;
  if(so)             this->systemoperative = so;
  if(ostype)         this->ostype          = ostype;
  if(osversion)      this->osversion       = osversion;
  if(language)       this->language        = language;
  if(languagetag)    this->languagetag     = languagetag;

  Sanitize(this->browser);
  Sanitize(this->browserversion);
  Sanitize(this->browsertype);
  Sanitize(this->systemoperative);
  Sanitize(this->ostype);
  Sanitize(this->osversion);
  Sanitize(this->language);
  Sanitize(this->languagetag);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOUSERAGENTID_RESULT::Sanitize(XSTRING& value)
* @brief      Sanitize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOUSERAGENTID_RESULT::Sanitize(XSTRING& value)
{
  XCHAR character[3] = { 0x09, 0x0A, 0x0D };

  for(int c=0;c<3;c++)
    {
      value.DeleteCharacter(character[c], XSTRINGCONTEXT_ALLSTRING);
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOUSERAGENTID_RESULT::Clean()
* @brief      Clean
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOUSERAGENTID_RESULT::Clean()
{

}




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSCRAPERWEBUSERAGENTID::DIOSCRAPERWEBUSERAGENTID()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSCRAPERWEBUSERAGENTID::DIOSCRAPERWEBUSERAGENTID()
{
  Clean();

  cache      = GEN_NEW DIOSCRAPERWEBCACHE();
  xmutexdo   = GEN_XFACTORY.Create_Mutex();
  scriptpath = DIOSCRAPERWEBUSERAGENTID_SCRIPTPATH;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSCRAPERWEBUSERAGENTID::~DIOSCRAPERWEBUSERAGENTID()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSCRAPERWEBUSERAGENTID::~DIOSCRAPERWEBUSERAGENTID()
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
* @fn         bool DIOSCRAPERWEBUSERAGENTID::Get(XSTRING& useragent, DIOUSERAGENTID_RESULT& result, ...)
* @brief      Get via scraper script
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEBUSERAGENTID::Get(XSTRING& useragent, DIOUSERAGENTID_RESULT& result, int timeoutforurl, XSTRING* localIP, bool usecache)
{
  return Get(useragent.Get(), result, timeoutforurl, localIP, usecache);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEBUSERAGENTID::Get(XCHAR* useragent, DIOUSERAGENTID_RESULT& result, ...)
* @brief      Get via scraper script
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEBUSERAGENTID::Get(XCHAR* useragent, DIOUSERAGENTID_RESULT& result, int timeoutforurl, XSTRING* localIP, bool usecache)
{
  bool    status = false;
  XSTRING cacheask;
  DIOURL  uaencoded;

  if((!useragent) || (!useragent[0])) return false;

  if(xmutexdo) xmutexdo->Lock();

  cacheask = useragent;

  if(usecache && cache)
    {
      DIOUSERAGENTID_RESULT* cached = (DIOUSERAGENTID_RESULT*)cache->Get(cacheask);
      if(cached)
        {
          result.CopyFrom(cached);

          if(xmutexdo) xmutexdo->UnLock();
          return true;
        }
    }

  uaencoded.EncodeUnsafeCharsFromString(useragent);

  DIOSCRAPERSCRIPT runner;

  runner.SetArg(_L("ua"), uaencoded.Get());
  runner.SetArgInt(_L("timeout"), timeoutforurl);
  if(localIP && (!localIP->IsEmpty()))
    {
      runner.SetArg(_L("localIP"), (*localIP));
    }

  if(runner.Run(scriptpath.Get()))
    {
      XSTRING ok;
      XSTRING browser;
      XSTRING browserversion;
      XSTRING browsertype;
      XSTRING so;
      XSTRING ostype;
      XSTRING osversion;
      XSTRING language;
      XSTRING languagetag;

      runner.GetResult(_L("ok"), ok);
      runner.GetResult(_L("browser"), browser);
      runner.GetResult(_L("browser_version"), browserversion);
      runner.GetResult(_L("browser_type"), browsertype);
      runner.GetResult(_L("os"), so);
      runner.GetResult(_L("os_type"), ostype);
      runner.GetResult(_L("os_version"), osversion);
      runner.GetResult(_L("language"), language);
      runner.GetResult(_L("language_tag"), languagetag);

      if((ok.Compare(_L("1")) == 0) && (!browser.IsEmpty()))
        {
          result.Set(browser.Get(),
                     browserversion.Get(),
                     browsertype.Get(),
                     so.Get(),
                     ostype.Get(),
                     osversion.Get(),
                     language.Get(),
                     languagetag.Get());

          if(!result.IsEmpty())
            {
              if(usecache && cache)
                {
                  DIOUSERAGENTID_RESULT* entry = GEN_NEW DIOUSERAGENTID_RESULT();
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
* @fn         bool DIOSCRAPERWEBUSERAGENTID::Get(XCHAR* useragent, XSTRING& browser, XSTRING& systemoperative, ...)
* @brief      Legacy out-param Get
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEBUSERAGENTID::Get(XCHAR* useragent, XSTRING& browser, XSTRING& systemoperative, int timeoutforurl, XSTRING* localIP, bool usecache)
{
  DIOUSERAGENTID_RESULT result;

  if(!Get(useragent, result, timeoutforurl, localIP, usecache)) return false;

  browser         = result.GetBrowser();
  systemoperative = result.GetSO();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEBUSERAGENTID::Get(XSTRING& useragent, XSTRING& browser, XSTRING& systemoperative, ...)
* @brief      Legacy out-param Get
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEBUSERAGENTID::Get(XSTRING& useragent, XSTRING& browser, XSTRING& systemoperative, int timeoutforurl, XSTRING* localIP, bool usecache)
{
  return Get(useragent.Get(), browser, systemoperative, timeoutforurl, localIP, usecache);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEBUSERAGENTID::SetScriptPath(XCHAR* relativescriptpath)
* @brief      SetScriptPath
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEBUSERAGENTID::SetScriptPath(XCHAR* relativescriptpath)
{
  if((!relativescriptpath) || (!relativescriptpath[0])) return false;

  scriptpath = relativescriptpath;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* DIOSCRAPERWEBUSERAGENTID::GetScriptPath()
* @brief      GetScriptPath
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* DIOSCRAPERWEBUSERAGENTID::GetScriptPath()
{
  return scriptpath.Get();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSCRAPERWEBUSERAGENTID::Clean()
* @brief      Clean
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSCRAPERWEBUSERAGENTID::Clean()
{
  cache    = NULL;
  xmutexdo = NULL;
}

#endif
