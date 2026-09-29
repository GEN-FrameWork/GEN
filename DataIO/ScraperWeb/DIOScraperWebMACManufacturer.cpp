/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOScraperWebMACManufacturer.cpp
* 
* @class      DIOSCRAPERWEBMACMANUFACTURER
* @brief      Typed MAC Manufacturer scraper filled by script (not XML)
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

#ifdef DIO_SCRAPERWEB_MACMANUFACTURER_ACTIVE

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOScraperWebMACManufacturer.h"

#include "XFactory.h"
#include "XThread.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/




/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOMACMANUFACTURED_RESULT::DIOMACMANUFACTURED_RESULT()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOMACMANUFACTURED_RESULT::DIOMACMANUFACTURED_RESULT(): DIOSCRAPERWEBCACHE_RESULT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOMACMANUFACTURED_RESULT::~DIOMACMANUFACTURED_RESULT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOMACMANUFACTURED_RESULT::~DIOMACMANUFACTURED_RESULT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* DIOMACMANUFACTURED_RESULT::GetManufacturer()
* @brief      Get manufacturer
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* DIOMACMANUFACTURED_RESULT::GetManufacturer()
{
  return manufacturer.Get();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOMACMANUFACTURED_RESULT::Get()
* @brief      Get manufacturer string (legacy)
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOMACMANUFACTURED_RESULT::Get()
{
  return &manufacturer;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOMACMANUFACTURED_RESULT::IsEmpty()
* @brief      Is empty
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOMACMANUFACTURED_RESULT::IsEmpty()
{
  if(manufacturer.IsEmpty()) return true;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOMACMANUFACTURED_RESULT::CopyFrom(DIOSCRAPERWEBCACHE_RESULT* result)
* @brief      Copy from
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOMACMANUFACTURED_RESULT::CopyFrom(DIOSCRAPERWEBCACHE_RESULT* result)
{
  if(!result) return false;

  DIOMACMANUFACTURED_RESULT* macresult = (DIOMACMANUFACTURED_RESULT*)result;

  if(macresult->IsEmpty()) return false;

  manufacturer = macresult->manufacturer;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOMACMANUFACTURED_RESULT::CopyTo(DIOSCRAPERWEBCACHE_RESULT* result)
* @brief      Copy to
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOMACMANUFACTURED_RESULT::CopyTo(DIOSCRAPERWEBCACHE_RESULT* result)
{
  if(!result)   return false;
  if(IsEmpty()) return false;

  DIOMACMANUFACTURED_RESULT* macresult = (DIOMACMANUFACTURED_RESULT*)result;

  macresult->manufacturer = manufacturer;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOMACMANUFACTURED_RESULT::Set(XSTRING& manufacturer)
* @brief      Set value
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOMACMANUFACTURED_RESULT::Set(XSTRING& manufacturer)
{
  return Set(manufacturer.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOMACMANUFACTURED_RESULT::Set(XCHAR* manufacturer)
* @brief      Set value
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOMACMANUFACTURED_RESULT::Set(XCHAR* manufacturer)
{
  if(manufacturer) this->manufacturer = manufacturer;

  XCHAR character[3] = { 0x09, 0x0A, 0x0D };

  for(int c=0;c<3;c++)
    {
      this->manufacturer.DeleteCharacter(character[c], XSTRINGCONTEXT_ALLSTRING);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOMACMANUFACTURED_RESULT::Clean()
* @brief      Clean
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOMACMANUFACTURED_RESULT::Clean()
{

}




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSCRAPERWEBMACMANUFACTURER::DIOSCRAPERWEBMACMANUFACTURER()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSCRAPERWEBMACMANUFACTURER::DIOSCRAPERWEBMACMANUFACTURER()
{
  Clean();

  cache      = GEN_NEW DIOSCRAPERWEBCACHE();
  xmutexdo   = GEN_XFACTORY.Create_Mutex();
  scriptpath = DIOSCRAPERWEBMACMANUFACTURER_SCRIPTPATH;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSCRAPERWEBMACMANUFACTURER::~DIOSCRAPERWEBMACMANUFACTURER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSCRAPERWEBMACMANUFACTURER::~DIOSCRAPERWEBMACMANUFACTURER()
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
* @fn         bool DIOSCRAPERWEBMACMANUFACTURER::Get(DIOMAC& MAC, DIOMACMANUFACTURED_RESULT& result, ...)
* @brief      Get manufacturer via scraper script
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEBMACMANUFACTURER::Get(DIOMAC& MAC, DIOMACMANUFACTURED_RESULT& result, int timeoutforurl, XSTRING* localIP, bool usecache)
{
  XSTRING macstring;

  MAC.GetXString(macstring);

  return Get(macstring.Get(), result, timeoutforurl, localIP, usecache);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEBMACMANUFACTURER::Get(XSTRING& MAC, DIOMACMANUFACTURED_RESULT& result, ...)
* @brief      Get manufacturer via scraper script
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEBMACMANUFACTURER::Get(XSTRING& MAC, DIOMACMANUFACTURED_RESULT& result, int timeoutforurl, XSTRING* localIP, bool usecache)
{
  return Get(MAC.Get(), result, timeoutforurl, localIP, usecache);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEBMACMANUFACTURER::Get(XCHAR* MAC, DIOMACMANUFACTURED_RESULT& result, ...)
* @brief      Get manufacturer via scraper script
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEBMACMANUFACTURER::Get(XCHAR* MAC, DIOMACMANUFACTURED_RESULT& result, int timeoutforurl, XSTRING* localIP, bool usecache)
{
  bool    status = false;
  XSTRING cacheask;

  if((!MAC) || (!MAC[0])) return false;

  if(xmutexdo) xmutexdo->Lock();

  cacheask = MAC;

  if(usecache && cache)
    {
      DIOMACMANUFACTURED_RESULT* cached = (DIOMACMANUFACTURED_RESULT*)cache->Get(cacheask);
      if(cached)
        {
          result.CopyFrom(cached);

          if(xmutexdo) xmutexdo->UnLock();
          return true;
        }
    }

  DIOSCRAPERSCRIPT runner;

  runner.SetArg(__L("mac"), MAC);
  runner.SetArgInt(__L("timeout"), timeoutforurl);
  if(localIP && (!localIP->IsEmpty()))
    {
      runner.SetArg(__L("localIP"), (*localIP));
    }

  if(runner.Run(scriptpath.Get()))
    {
      XSTRING ok;
      XSTRING manufacturer;

      runner.GetResult(__L("ok"), ok);
      runner.GetResult(__L("manufacturer"), manufacturer);

      if((ok.Compare(__L("1")) == 0) && (!manufacturer.IsEmpty()))
        {
          result.Set(manufacturer);

          if(!result.IsEmpty())
            {
              if(usecache && cache)
                {
                  DIOMACMANUFACTURED_RESULT* entry = GEN_NEW DIOMACMANUFACTURED_RESULT();
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
* @fn         bool DIOSCRAPERWEBMACMANUFACTURER::Get(DIOMAC& MAC, XSTRING& manufactured, ...)
* @brief      Legacy out-param Get
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEBMACMANUFACTURER::Get(DIOMAC& MAC, XSTRING& manufactured, int timeoutforurl, XSTRING* localIP, bool usecache)
{
  DIOMACMANUFACTURED_RESULT result;

  if(!Get(MAC, result, timeoutforurl, localIP, usecache)) return false;

  manufactured = result.GetManufacturer();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEBMACMANUFACTURER::SetScriptPath(XCHAR* relativescriptpath)
* @brief      SetScriptPath
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEBMACMANUFACTURER::SetScriptPath(XCHAR* relativescriptpath)
{
  if((!relativescriptpath) || (!relativescriptpath[0])) return false;

  scriptpath = relativescriptpath;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* DIOSCRAPERWEBMACMANUFACTURER::GetScriptPath()
* @brief      GetScriptPath
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* DIOSCRAPERWEBMACMANUFACTURER::GetScriptPath()
{
  return scriptpath.Get();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSCRAPERWEBMACMANUFACTURER::Clean()
* @brief      Clean
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSCRAPERWEBMACMANUFACTURER::Clean()
{
  cache    = NULL;
  xmutexdo = NULL;
}

#endif
