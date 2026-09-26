/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOScraperWebPublicIP.h
* 
* @class      DIOSCRAPERWEBPUBLICIP
* @brief      Data Input/Output Scraper Web Public IP class
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

#include "DIOIP.h"
#include "DIOScraperWeb.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOSCRAPERWEBPUBLICIP_NAMEFILE      __L("ws_publicip.xml")
#define DIOSCRAPERWEBPUBLICIP_NAMESERVICE   __L("PUBLICIP")

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

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


class DIOSCRAPERWEBPUBLICIP : public DIOSCRAPERWEB
{
  public:
                              DIOSCRAPERWEBPUBLICIP             ();
    virtual                  ~DIOSCRAPERWEBPUBLICIP             ();

    bool                      Get                               (DIOIP& IP, int timeoutforurl = DIOSCRAPERWEB_MAXTIMEOUTFORURL, XSTRING* localIP = NULL, bool usecache = true);

  private:

    void                      Clean                             ();

    DIOIP                     IP;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





