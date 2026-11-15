/**-------------------------------------------------------------------------------------------------------------------
*
* @file       DIOScraperWebStaticMap.h
*
* @class      DIOSCRAPERWEBSTATICMAP
* @brief      Typed Static Map scraper filled by script (not XML)
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

#ifdef DIO_SCRAPERWEB_STATICMAP_ACTIVE

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XMap.h"
#include "XString.h"
#include "XPath.h"

#include "DIOScraperWebCache.h"
#include "DIOScraperScript.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOSCRAPERWEBSTATICMAP_SCRIPTPATH       NL("staticmap.g")
#define DIOSCRAPERWEBSTATICMAP_MAXTIMEOUT       DIOSCRAPERSCRIPT_DEFAULT_TIMEOUT
#define DIOSCRAPERWEBSTATICMAP_DEFAULT_WIDTH    240
#define DIOSCRAPERWEBSTATICMAP_DEFAULT_HEIGHT   140
#define DIOSCRAPERWEBSTATICMAP_DEFAULT_ZOOM     11

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XMUTEX;

class DIOSTATICMAP_RESULT : public DIOSCRAPERWEBCACHE_RESULT
{
  public:

                            DIOSTATICMAP_RESULT               ();
    virtual                ~DIOSTATICMAP_RESULT               ();

    XCHAR*                  GetPath                           ();
    XCHAR*                  GetProvider                       ();

    bool                    IsEmpty                           ();

    virtual bool            CopyFrom                          (DIOSCRAPERWEBCACHE_RESULT* result);
    virtual bool            CopyTo                            (DIOSCRAPERWEBCACHE_RESULT* result);

    bool                    Set                               (XSTRING& path, XSTRING& provider);
    bool                    Set                               (XCHAR* path, XCHAR* provider = NULL);

  private:

    void                    Clean                             ();

    XSTRING                 path;
    XSTRING                 provider;
};


class DIOSCRAPERWEBSTATICMAP
{
  public:
                            DIOSCRAPERWEBSTATICMAP            ();
    virtual                ~DIOSCRAPERWEBSTATICMAP            ();

    bool                    Get                               (float latitude, float longitude, XPATH& outpath, DIOSTATICMAP_RESULT& result, int width = DIOSCRAPERWEBSTATICMAP_DEFAULT_WIDTH, int height = DIOSCRAPERWEBSTATICMAP_DEFAULT_HEIGHT, int zoom = DIOSCRAPERWEBSTATICMAP_DEFAULT_ZOOM, XCHAR* address = NULL, int timeoutforurl = DIOSCRAPERWEBSTATICMAP_MAXTIMEOUT, bool usecache = true);
    bool                    Get                               (float latitude, float longitude, XSTRING& outpath, DIOSTATICMAP_RESULT& result, int width = DIOSCRAPERWEBSTATICMAP_DEFAULT_WIDTH, int height = DIOSCRAPERWEBSTATICMAP_DEFAULT_HEIGHT, int zoom = DIOSCRAPERWEBSTATICMAP_DEFAULT_ZOOM, XCHAR* address = NULL, int timeoutforurl = DIOSCRAPERWEBSTATICMAP_MAXTIMEOUT, bool usecache = true);

    bool                    SetScriptPath                     (XCHAR* relativescriptpath);
    XCHAR*                  GetScriptPath                     ();

  private:

    void                    Clean                             ();
    bool                    BuildCacheAsk                     (float latitude, float longitude, int width, int height, int zoom, XCHAR* address, XSTRING& ask);

    DIOSCRAPERWEBCACHE*     cache;
    XMUTEX*                 xmutexdo;
    XSTRING                 scriptpath;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/

#endif
