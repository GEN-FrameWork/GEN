/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOScraperWebPublicIP.h
* 
* @class      DIOSCRAPERWEBPUBLICIP
* @brief      Typed Public IP scraper filled by script (not XML)
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

#ifdef DIO_SCRAPERWEB_PUBLICIP_ACTIVE

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOIP.h"
#include "DIOScraperWebCache.h"
#include "DIOScraperScript.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOSCRAPERWEBPUBLICIP_SCRIPTPATH    _L("publicip.g")
#define DIOSCRAPERWEBPUBLICIP_CACHEASK      _L("public IP ID")
#define DIOSCRAPERWEBPUBLICIP_MAXTIMEOUT    DIOSCRAPERSCRIPT_DEFAULT_TIMEOUT

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XMUTEX;

class DIOPUBLICIP_RESULT :  public DIOSCRAPERWEBCACHE_RESULT
{
  public:
                              DIOPUBLICIP_RESULT                ();
    virtual                  ~DIOPUBLICIP_RESULT                ();

    DIOIP*                    Get                               ();

  private:

    void                      Clean                             ();

    DIOIP                     IP;
};


class DIOSCRAPERWEBPUBLICIP
{
  public:
                              DIOSCRAPERWEBPUBLICIP             ();
    virtual                  ~DIOSCRAPERWEBPUBLICIP             ();

    bool                      Get                               (DIOIP& IP, int timeoutforurl = DIOSCRAPERWEBPUBLICIP_MAXTIMEOUT, XSTRING* localIP = NULL, bool usecache = true);

    bool                      SetScriptPath                     (XCHAR* relativescriptpath);
    XCHAR*                    GetScriptPath                     ();

  private:

    void                      Clean                             ();

    DIOSCRAPERWEBCACHE*       cache;
    XMUTEX*                   xmutexdo;
    XSTRING                   scriptpath;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/

#endif

