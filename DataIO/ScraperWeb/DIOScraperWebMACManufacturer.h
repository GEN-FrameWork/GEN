/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOScraperWebMACManufacturer.h
* 
* @class      DIOSCRAPERWEBMACMANUFACTURER
* @brief      Data Input/Output Scraper Web MAC Manufacturer class
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

#include "DIOMAC.h"
#include "DIOURL.h"
#include "DIOScraperWeb.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOSCRAPERWEBMACMANUFACTURER_NAMEFILE     __L("ws_publicip.xml")
#define DIOSCRAPERWEBMACMANUFACTURER_NAMESERVICE  __L("MACMANUFACTURED")

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOMACMANUFACTURED_RESULT :  public DIOSCRAPERWEBCACHE_RESULT
{
  public:
                              DIOMACMANUFACTURED_RESULT         ();
    virtual                  ~DIOMACMANUFACTURED_RESULT         ();

    XSTRING*                  Get                               ();

  private:

    void                      Clean                             ();

    XSTRING                   manufactured;
};


class DIOSCRAPERWEBMACMANUFACTURER : public DIOSCRAPERWEB
{
  public:
                              DIOSCRAPERWEBMACMANUFACTURER      ();
    virtual                  ~DIOSCRAPERWEBMACMANUFACTURER      ();

    bool                      ChangeURL                         (XCHAR* maskurl, DIOURL& url);

    bool                      Get                               (DIOMAC& MAC, XSTRING& manufactured, int timeoutforurl = DIOSCRAPERWEB_MAXTIMEOUTFORURL, XSTRING* localIP = NULL, bool usecache = true);

  private:

    void                      Clean                             ();

    DIOMAC                    MAC;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/







