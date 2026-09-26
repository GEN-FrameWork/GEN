/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOScraperWebUserAgentID.h
* 
* @class      DIOSCRAPERWEBUSERAGENTID
* @brief      Data Input/Output Scraper Web User Agent ID class
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

#include "DIOURL.h"
#include "DIOScraperWeb.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOSCRAPERWEBUSERAGENTID_NAMEFILE       __L("ws_useragentid.xml")
#define DIOSCRAPERWEBUSERAGENTID_NAMESERVICE    __L("USERAGENTID")

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOUSERAGENTID_RESULT :  public DIOSCRAPERWEBCACHE_RESULT
{
  public:
                              DIOUSERAGENTID_RESULT           ();
    virtual                  ~DIOUSERAGENTID_RESULT           ();

    XSTRING*                  GetBrowser                      ();
    XSTRING*                  GetSO                           ();

  private:

    void                      Clean                           ();

    XSTRING                   browser;
    XSTRING                   systemoperative;
};


class DIOSCRAPERWEBUSERAGENTID : public DIOSCRAPERWEB
{
  public:
                              DIOSCRAPERWEBUSERAGENTID        ();
    virtual                  ~DIOSCRAPERWEBUSERAGENTID        ();

    bool                      ChangeURL                       (XCHAR* maskurl, DIOURL& url);

    bool                      Get                             (XCHAR* useragent, XSTRING& browser, XSTRING& systemoperative, int timeoutforurl = DIOSCRAPERWEB_MAXTIMEOUTFORURL, XSTRING* localIP = NULL, bool usecache = true);
    bool                      Get                             (XSTRING& useragent, XSTRING& browser, XSTRING& systemoperative, int timeoutforurl = DIOSCRAPERWEB_MAXTIMEOUTFORURL, XSTRING* localIP = NULL, bool usecache = true);

  private:

    void                      Clean                           ();

    XSTRING                   useragent;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/






