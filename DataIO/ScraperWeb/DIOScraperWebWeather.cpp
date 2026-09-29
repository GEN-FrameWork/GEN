/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOScraperWebWeather.cpp
* 
* @class      DIOSCRAPERWEBWEATHER
* @brief      Typed Weather scraper filled by script (not XML)
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

#ifdef DIO_SCRAPERWEB_WEATHER_ACTIVE

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOScraperWebWeather.h"

#include "XFactory.h"
#include "XThread.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/




/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEATHER_RESULT::DIOWEATHER_RESULT()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEATHER_RESULT::DIOWEATHER_RESULT(): DIOSCRAPERWEBCACHE_RESULT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEATHER_RESULT::~DIOWEATHER_RESULT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEATHER_RESULT::~DIOWEATHER_RESULT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* DIOWEATHER_RESULT::GetCondition()
* @brief      Get condition
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* DIOWEATHER_RESULT::GetCondition()
{
  return condition.Get();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         float DIOWEATHER_RESULT::GetTemperature()
* @brief      Get temperature
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
float DIOWEATHER_RESULT::GetTemperature()
{
  return temperature;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         float DIOWEATHER_RESULT::GetHumidity()
* @brief      Get humidity
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
float DIOWEATHER_RESULT::GetHumidity()
{
  return humidity;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEATHER_RESULT::IsEmpty()
* @brief      Is empty
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEATHER_RESULT::IsEmpty()
{
  if(condition.IsEmpty()) return true;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEATHER_RESULT::CopyFrom(DIOSCRAPERWEBCACHE_RESULT* result)
* @brief      Copy from
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEATHER_RESULT::CopyFrom(DIOSCRAPERWEBCACHE_RESULT* result)
{
  if(!result) return false;

  DIOWEATHER_RESULT* weather = (DIOWEATHER_RESULT*)result;

  if(weather->IsEmpty()) return false;

  condition   = weather->condition;
  temperature = weather->temperature;
  humidity    = weather->humidity;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEATHER_RESULT::CopyTo(DIOSCRAPERWEBCACHE_RESULT* result)
* @brief      Copy to
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEATHER_RESULT::CopyTo(DIOSCRAPERWEBCACHE_RESULT* result)
{
  if(!result)   return false;
  if(IsEmpty()) return false;

  DIOWEATHER_RESULT* weather = (DIOWEATHER_RESULT*)result;

  weather->condition   = condition;
  weather->temperature = temperature;
  weather->humidity    = humidity;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEATHER_RESULT::Set(XSTRING& condition, float temperature, float humidity)
* @brief      Set value
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEATHER_RESULT::Set(XSTRING& condition, float temperature, float humidity)
{
  return Set(condition.Get(), temperature, humidity);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEATHER_RESULT::Set(XCHAR* condition, float temperature, float humidity)
* @brief      Set value
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEATHER_RESULT::Set(XCHAR* condition, float temperature, float humidity)
{
  if(condition) this->condition = condition;

  this->temperature = temperature;
  this->humidity    = humidity;

  XCHAR character[3] = { 0x09, 0x0A, 0x0D };

  for(int c=0;c<3;c++)
    {
      this->condition.DeleteCharacter(character[c], XSTRINGCONTEXT_ALLSTRING);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEATHER_RESULT::Clean()
* @brief      Clean
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEATHER_RESULT::Clean()
{
  temperature = 0.0f;
  humidity    = 0.0f;
}




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSCRAPERWEBWEATHER::DIOSCRAPERWEBWEATHER()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSCRAPERWEBWEATHER::DIOSCRAPERWEBWEATHER()
{
  Clean();

  cache      = GEN_NEW DIOSCRAPERWEBCACHE();
  xmutexdo   = GEN_XFACTORY.Create_Mutex();
  scriptpath = DIOSCRAPERWEBWEATHER_SCRIPTPATH;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSCRAPERWEBWEATHER::~DIOSCRAPERWEBWEATHER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSCRAPERWEBWEATHER::~DIOSCRAPERWEBWEATHER()
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
* @fn         bool DIOSCRAPERWEBWEATHER::Get(XCHAR* location, bool iscelsius, DIOWEATHER_RESULT& weather, ...)
* @brief      Get weather via scraper script
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEBWEATHER::Get(XCHAR* location, bool iscelsius, DIOWEATHER_RESULT& weather, int timeoutforurl, XSTRING* localIP, bool usecache)
{
  bool    status = false;
  XSTRING cacheask;

  if((!location) || (!location[0])) return false;

  if(xmutexdo) xmutexdo->Lock();

  cacheask = location;
  cacheask += __L("|");
  cacheask += iscelsius ? __L("C") : __L("F");

  if(usecache && cache)
    {
      DIOWEATHER_RESULT* cached = (DIOWEATHER_RESULT*)cache->Get(cacheask);
      if(cached)
        {
          weather.CopyFrom(cached);

          if(xmutexdo) xmutexdo->UnLock();
          return true;
        }
    }

  DIOSCRAPERSCRIPT runner;

  runner.SetArg(__L("location"), location);
  runner.SetArgInt(__L("celsius"), iscelsius ? 1 : 0);
  runner.SetArgInt(__L("timeout"), timeoutforurl);
  if(localIP && (!localIP->IsEmpty()))
    {
      runner.SetArg(__L("localIP"), (*localIP));
    }

  if(runner.Run(scriptpath.Get()))
    {
      XSTRING ok;
      XSTRING condition;
      XSTRING temperature;
      XSTRING humidity;
      float   temp = 0.0f;
      float   hum  = 0.0f;

      runner.GetResult(__L("ok"), ok);
      runner.GetResult(__L("condition"), condition);
      runner.GetResult(__L("temperature"), temperature);
      runner.GetResult(__L("humidity"), humidity);

      if(!temperature.IsEmpty()) temperature.UnFormat(__L("%f"), &temp);
      if(!humidity.IsEmpty())    humidity.UnFormat(__L("%f"), &hum);

      if((ok.Compare(__L("1")) == 0) && (!temperature.IsEmpty()) && (!humidity.IsEmpty()))
        {
          weather.Set(condition, temp, hum);

          if(!weather.IsEmpty())
            {
              if(usecache && cache)
                {
                  DIOWEATHER_RESULT* entry = GEN_NEW DIOWEATHER_RESULT();
                  if(entry)
                    {
                      entry->CopyFrom(&weather);
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
* @fn         bool DIOSCRAPERWEBWEATHER::Get(XSTRING& location, bool iscelsius, DIOWEATHER_RESULT& weather, ...)
* @brief      Get weather via scraper script
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEBWEATHER::Get(XSTRING& location, bool iscelsius, DIOWEATHER_RESULT& weather, int timeoutforurl, XSTRING* localIP, bool usecache)
{
  return Get(location.Get(), iscelsius, weather, timeoutforurl, localIP, usecache);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEBWEATHER::Get(XCHAR* location, bool iscelsius, XSTRING& condition, float& temperature, float& humidity, ...)
* @brief      Legacy-compatible out-params Get
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEBWEATHER::Get(XCHAR* location, bool iscelsius, XSTRING& condition, float& temperature, float& humidity, int timeoutforurl, XSTRING* localIP, bool usecache)
{
  DIOWEATHER_RESULT weather;

  if(!Get(location, iscelsius, weather, timeoutforurl, localIP, usecache)) return false;

  condition   = weather.GetCondition();
  temperature = weather.GetTemperature();
  humidity    = weather.GetHumidity();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEBWEATHER::Get(XSTRING& location, bool iscelsius, XSTRING& condition, float& temperature, float& humidity, ...)
* @brief      Legacy-compatible out-params Get
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEBWEATHER::Get(XSTRING& location, bool iscelsius, XSTRING& condition, float& temperature, float& humidity, int timeoutforurl, XSTRING* localIP, bool usecache)
{
  return Get(location.Get(), iscelsius, condition, temperature, humidity, timeoutforurl, localIP, usecache);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEBWEATHER::SetScriptPath(XCHAR* relativescriptpath)
* @brief      SetScriptPath
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEBWEATHER::SetScriptPath(XCHAR* relativescriptpath)
{
  if((!relativescriptpath) || (!relativescriptpath[0])) return false;

  scriptpath = relativescriptpath;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* DIOSCRAPERWEBWEATHER::GetScriptPath()
* @brief      GetScriptPath
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* DIOSCRAPERWEBWEATHER::GetScriptPath()
{
  return scriptpath.Get();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSCRAPERWEBWEATHER::Clean()
* @brief      Clean
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSCRAPERWEBWEATHER::Clean()
{
  cache    = NULL;
  xmutexdo = NULL;
}

#endif
