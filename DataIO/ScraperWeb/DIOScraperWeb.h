/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOScraperWeb.h
* 
* @class      DIOSCRAPERWEB
* @brief      Data Input/Output Scraper Web class (LEGACY XML engine — prefer script facades)
*
* @note       LEGACY: do not add new providers here. Use DIOSCRAPERSCRIPT + typed facades
*             (see Resources/scraperweb/Scraper_Script_Model.md). PublicIP and GeolocationIP
*             no longer inherit this class.
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

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XVector.h"
#include "XMap.h"

#include "DIOWebClient.h"
#include "DIOScraperWebCache.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOSCRAPERWEB_MAXTIMEOUTFORURL         10
#define DIOSCRAPERWEB_NAMEFILE                 __L("scraperweb.xml")
#define DIOSCRAPERWEB_DEFAULTUSERAGENT         DIOWEBCLIENT_DEFAULTUSERAGENT

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XDATETIME;
class XTIMER;
class XRAND;
class XBUFFER;
class XFILEXML;
class XPATHS;
class DIOCHECKINTERNETCONNECTION;

class DIOSCRAPERWEB
{
  public:
                                    DIOSCRAPERWEB                         ();
    virtual                        ~DIOSCRAPERWEB                         ();

    bool                            Load                                  (XCHAR* namefile);
    bool                            Load                                  (XPATH& namefile);

    DIOURL*                         GetURLDownload                        ();

    bool                            Download                              (DIOURL& URL, XPATH& xpath);

    bool                            Do                                    (XCHAR* namewebservice, int timeoutforurl = DIOSCRAPERWEB_MAXTIMEOUTFORURL, XSTRING* localIP = NULL);
    bool                            Do                                    (XSTRING& namewebservice, int timeoutforurl = DIOSCRAPERWEB_MAXTIMEOUTFORURL, XSTRING* localIP = NULL);

    virtual bool                    ChangeURL                             (XCHAR* maskurl, DIOURL& url);

    bool                            AddValue                              (XCHAR* name, XCHAR* value);
    bool                            AddValue                              (XCHAR* name, XSTRING& value);
    bool                            AddValue                              (XSTRING& name, XCHAR* value);
    bool                            AddValue                              (XSTRING& name, XSTRING& value);
    XCHAR*                          GetValue                              (XCHAR* namevalue);
    XCHAR*                          GetValue                              (XSTRING& namevalue);
    bool                            DeleteAllValues                       ();

    DIOWEBCLIENT*                   GetWebClient                          ();

    bool                            ExtractString                         (XCHAR* searchini, XCHAR* searchend, XBUFFER* xbuffer, bool usebufferpos, XSTRING& result);
    bool                            ExtractString                         (XCHAR* searchini, XSTRING& searchend, XBUFFER* xbuffer, bool usebufferpos, XSTRING& result);
    bool                            ExtractString                         (XSTRING& searchini, XCHAR* searchend, XBUFFER* xbuffer, bool usebufferpos, XSTRING& result);
    bool                            ExtractString                         (XSTRING& searchini, XSTRING& searchend, XBUFFER* xbuffer, bool usebufferpos, XSTRING& result);

  protected:

    XMUTEX*                         xmutexdo;
    DIOWEBCLIENT*                   webclient;
    DIOSCRAPERWEBCACHE*             cache;

  private:

    void                            HandleEvent                           (XEVENT* xevent);

    void                            Clean                                 ();

    DIOURL*                         URLdownload;
    XFILEXML*                       xml;
    XMAP<XSTRING*,XSTRING*>         values;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





