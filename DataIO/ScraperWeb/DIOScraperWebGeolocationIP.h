/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOScraperWebGeolocationIP.h
* 
* @class      DIOSCRAPERWEBGEOLOCATIONIP
* @brief      Typed Geolocation IP scraper filled by script (not XML)
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

#ifdef DIO_SCRAPERWEB_GEOLOCATIONIP_ACTIVE

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XMap.h"

#include "DIOIP.h"
#include "DIOURL.h"

#include "DIOScraperWebCache.h"
#include "DIOScraperScript.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOSCRAPERWEBGEOLOCATIONIP_SCRIPTPATH    _L("geolocationip.g")
#define DIOSCRAPERWEBGEOLOCATIONIP_MAXTIMEOUT    DIOSCRAPERSCRIPT_DEFAULT_TIMEOUT

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XMUTEX;

class DIOGEOLOCATIONIP_RESULT : public DIOSCRAPERWEBCACHE_RESULT
{
  public:

                            DIOGEOLOCATIONIP_RESULT           ();
    virtual                ~DIOGEOLOCATIONIP_RESULT           ();

    XCHAR*                  GetCountry                        ();
    XCHAR*                  GetState                          ();
    XCHAR*                  GetCity                           ();
    XCHAR*                  GetISP                            ();
    XCHAR*                  GetOrganization                   ();
    float                   GetLatitude                       ();
    float                   GetLongitude                      ();

    bool                    IsEmpty                           ();

    virtual bool            CopyFrom                          (DIOSCRAPERWEBCACHE_RESULT* result);
    virtual bool            CopyTo                            (DIOSCRAPERWEBCACHE_RESULT* result);

    bool                    Set                               (XSTRING& country, XSTRING& state, XSTRING& city, XSTRING& ISP, XSTRING& organization);
    bool                    Set                               (XCHAR* country, XCHAR* state = NULL, XCHAR* city = NULL, XCHAR* ISP = NULL, XCHAR* organization = NULL);
    bool                    Set                               (float latitude, float longitude);

  private:

    void                    Clean                             ();

    XSTRING                 country;
    XSTRING                 state;
    XSTRING                 city;
    XSTRING                 ISP;
    XSTRING                 organization;
    float                   latitude;
    float                   longitude;
};


class DIOSCRAPERWEBGEOLOCATIONIP
{
  public:
                            DIOSCRAPERWEBGEOLOCATIONIP        ();
    virtual                ~DIOSCRAPERWEBGEOLOCATIONIP        ();

    bool                    Get                               (XCHAR* IP, DIOGEOLOCATIONIP_RESULT& geolocationIP, int timeoutforurl = DIOSCRAPERWEBGEOLOCATIONIP_MAXTIMEOUT, XSTRING* localIP = NULL, bool usecache = true);
    bool                    Get                               (XSTRING& IP, DIOGEOLOCATIONIP_RESULT& geolocationIP, int timeoutforurl = DIOSCRAPERWEBGEOLOCATIONIP_MAXTIMEOUT, XSTRING* localIP = NULL, bool usecache = true);
    bool                    Get                               (DIOIP& IP, DIOGEOLOCATIONIP_RESULT& geolocationIP, int timeoutforurl = DIOSCRAPERWEBGEOLOCATIONIP_MAXTIMEOUT, XSTRING* localIP = NULL, bool usecache = true);

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

