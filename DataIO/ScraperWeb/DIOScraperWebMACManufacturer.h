/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOScraperWebMACManufacturer.h
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

#pragma once

#ifdef DIO_SCRAPERWEB_MACMANUFACTURER_ACTIVE

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XString.h"

#include "DIOMAC.h"
#include "DIOScraperWebCache.h"
#include "DIOScraperScript.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOSCRAPERWEBMACMANUFACTURER_SCRIPTPATH    __L("macmanufacturer.g")
#define DIOSCRAPERWEBMACMANUFACTURER_MAXTIMEOUT    DIOSCRAPERSCRIPT_DEFAULT_TIMEOUT

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XMUTEX;

class DIOMACMANUFACTURED_RESULT : public DIOSCRAPERWEBCACHE_RESULT
{
  public:

                              DIOMACMANUFACTURED_RESULT         ();
    virtual                  ~DIOMACMANUFACTURED_RESULT         ();

    XCHAR*                    GetManufacturer                   ();
    XSTRING*                  Get                               ();

    bool                      IsEmpty                           ();

    virtual bool              CopyFrom                          (DIOSCRAPERWEBCACHE_RESULT* result);
    virtual bool              CopyTo                            (DIOSCRAPERWEBCACHE_RESULT* result);

    bool                      Set                               (XSTRING& manufacturer);
    bool                      Set                               (XCHAR* manufacturer);

  private:

    void                      Clean                             ();

    XSTRING                   manufacturer;
};


class DIOSCRAPERWEBMACMANUFACTURER
{
  public:
                              DIOSCRAPERWEBMACMANUFACTURER      ();
    virtual                  ~DIOSCRAPERWEBMACMANUFACTURER      ();

    bool                      Get                               (DIOMAC& MAC, DIOMACMANUFACTURED_RESULT& result, int timeoutforurl = DIOSCRAPERWEBMACMANUFACTURER_MAXTIMEOUT, XSTRING* localIP = NULL, bool usecache = true);
    bool                      Get                               (XCHAR* MAC, DIOMACMANUFACTURED_RESULT& result, int timeoutforurl = DIOSCRAPERWEBMACMANUFACTURER_MAXTIMEOUT, XSTRING* localIP = NULL, bool usecache = true);
    bool                      Get                               (XSTRING& MAC, DIOMACMANUFACTURED_RESULT& result, int timeoutforurl = DIOSCRAPERWEBMACMANUFACTURER_MAXTIMEOUT, XSTRING* localIP = NULL, bool usecache = true);
    bool                      Get                               (DIOMAC& MAC, XSTRING& manufactured, int timeoutforurl = DIOSCRAPERWEBMACMANUFACTURER_MAXTIMEOUT, XSTRING* localIP = NULL, bool usecache = true);

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
