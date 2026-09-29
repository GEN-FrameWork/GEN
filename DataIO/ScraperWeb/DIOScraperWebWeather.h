/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOScraperWebWeather.h
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

#pragma once

#ifdef DIO_SCRAPERWEB_WEATHER_ACTIVE

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XString.h"

#include "DIOScraperWebCache.h"
#include "DIOScraperScript.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOSCRAPERWEBWEATHER_SCRIPTPATH    __L("weather.g")
#define DIOSCRAPERWEBWEATHER_MAXTIMEOUT    DIOSCRAPERSCRIPT_DEFAULT_TIMEOUT

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XMUTEX;

class DIOWEATHER_RESULT : public DIOSCRAPERWEBCACHE_RESULT
{
  public:

                            DIOWEATHER_RESULT                 ();
    virtual                ~DIOWEATHER_RESULT                 ();

    XCHAR*                  GetCondition                      ();
    float                   GetTemperature                    ();
    float                   GetHumidity                       ();

    bool                    IsEmpty                           ();

    virtual bool            CopyFrom                          (DIOSCRAPERWEBCACHE_RESULT* result);
    virtual bool            CopyTo                            (DIOSCRAPERWEBCACHE_RESULT* result);

    bool                    Set                               (XSTRING& condition, float temperature, float humidity);
    bool                    Set                               (XCHAR* condition, float temperature, float humidity);

  private:

    void                    Clean                             ();

    XSTRING                 condition;
    float                   temperature;
    float                   humidity;
};


class DIOSCRAPERWEBWEATHER
{
  public:
                            DIOSCRAPERWEBWEATHER              ();
    virtual                ~DIOSCRAPERWEBWEATHER              ();

    bool                    Get                               (XCHAR* location, bool iscelsius, DIOWEATHER_RESULT& weather, int timeoutforurl = DIOSCRAPERWEBWEATHER_MAXTIMEOUT, XSTRING* localIP = NULL, bool usecache = true);
    bool                    Get                               (XSTRING& location, bool iscelsius, DIOWEATHER_RESULT& weather, int timeoutforurl = DIOSCRAPERWEBWEATHER_MAXTIMEOUT, XSTRING* localIP = NULL, bool usecache = true);
    bool                    Get                               (XCHAR* location, bool iscelsius, XSTRING& condition, float& temperature, float& humidity, int timeoutforurl = DIOSCRAPERWEBWEATHER_MAXTIMEOUT, XSTRING* localIP = NULL, bool usecache = true);
    bool                    Get                               (XSTRING& location, bool iscelsius, XSTRING& condition, float& temperature, float& humidity, int timeoutforurl = DIOSCRAPERWEBWEATHER_MAXTIMEOUT, XSTRING* localIP = NULL, bool usecache = true);

    bool                    SetScriptPath                     (XCHAR* relativescriptpath);
    XCHAR*                  GetScriptPath                     ();

  private:

    void                    Clean                             ();

    DIOSCRAPERWEBCACHE*     cache;
    XMUTEX*                 xmutexdo;
    XSTRING                 scriptpath;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/

#endif
