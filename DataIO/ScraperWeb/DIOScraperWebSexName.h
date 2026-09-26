/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOScraperWebSexName.h
* 
* @class      DIOSCRAPERWEBSEXNAME
* @brief      Data Input/Output Scraper Web Sex Name class
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

enum DIOSCRAPERWEBSEXNAMETYPE
{
  DIOSCRAPERWEBSEXNAMETYPE_NOTOBTAINED      = 0 ,
  DIOSCRAPERWEBSEXNAMETYPE_NONAME               ,
  DIOSCRAPERWEBSEXNAMETYPE_MALE                 ,
  DIOSCRAPERWEBSEXNAMETYPE_FEMALE               ,
};

#define DIOSCRAPERWEBSEXNAME_NAMEFILE       __L("ws_sexname.xml")
#define DIOSCRAPERWEBSEXNAME_NAMESERVICE    __L("SEXNAME")




/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOSEXNAME_RESULT :  public DIOSCRAPERWEBCACHE_RESULT
{
  public:
                                DIOSEXNAME_RESULT                 ();
    virtual                    ~DIOSEXNAME_RESULT                 ();

    DIOSCRAPERWEBSEXNAMETYPE    Get                               ();
    bool                        Set                               (DIOSCRAPERWEBSEXNAMETYPE sex);

    XSTRING*                    GetNameClear                      ();

  private:

    void                        Clean                             ();

    DIOSCRAPERWEBSEXNAMETYPE    sex;
    XSTRING                     nameclear;
};


class DIOSCRAPERWEBSEXNAME : public DIOSCRAPERWEB
{
  public:
                                DIOSCRAPERWEBSEXNAME              ();
    virtual                    ~DIOSCRAPERWEBSEXNAME              ();

    bool                        ChangeURL                         (XCHAR* maskurl, DIOURL& url);

    bool                        Get                               (XCHAR* name, DIOSCRAPERWEBSEXNAMETYPE& sexnametype, XSTRING* nameclear = NULL, int timeoutforurl = DIOSCRAPERWEB_MAXTIMEOUTFORURL, XSTRING* localIP = NULL, bool usecache = true);
    bool                        Get                               (XSTRING& name, DIOSCRAPERWEBSEXNAMETYPE& sexnametype, XSTRING* nameclear = NULL, int timeoutforurl = DIOSCRAPERWEB_MAXTIMEOUTFORURL, XSTRING* localIP = NULL, bool usecache = true);

  private:

    void                        Clean                             ();

    XSTRING                     name;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





