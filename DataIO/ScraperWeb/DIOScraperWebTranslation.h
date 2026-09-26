/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOScraperWebTranslation.h
* 
* @class      DIOSCRAPERWEBTRANSLATION
* @brief      Data Input/Output Scraper Web Translation class
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

#include "XTranslation.h"

#include "DIOIP.h"
#include "DIOURL.h"
#include "DIOScraperWeb.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOSCRAPERWEBTRANSLATION_NAMEFILE       __L("ws_translation.xml")
#define DIOSCRAPERWEBTRANSLATION_NAMESERVICE    __L("TRANSLATION")

#define XLANGUAGE_CODE                          XLANGUAGE_ISO_639_3_CODE


/*---- CLASS ---------------------------------------------------------------------------------------------------------*/


class DIOTRANSLATION_RESULT :  public DIOSCRAPERWEBCACHE_RESULT
{
  public:
                            DIOTRANSLATION_RESULT           ();
    virtual                ~DIOTRANSLATION_RESULT           ();

    XLANGUAGE_CODE          GetLanguageTextCode             ();
    bool                    SetLanguageTextCode             (XLANGUAGE_CODE code);

    XLANGUAGE_CODE          GetLanguageTranslationCode      ();
    bool                    SetLanguageTranslationCode      (XLANGUAGE_CODE code);

    XSTRING*                GetTranslation                  ();

  private:

    void                    Clean                           ();

    XLANGUAGE_CODE          languagetextcode;
    XLANGUAGE_CODE          languagetranslationcode;
    XSTRING                 translation;
};


class DIOSCRAPERWEBTRANSLATION : public DIOSCRAPERWEB
{
  public:
                            DIOSCRAPERWEBTRANSLATION        ();
    virtual                ~DIOSCRAPERWEBTRANSLATION        ();

    bool                    ChangeURL                       (XCHAR* maskurl, DIOURL& url);

    bool                    Get                             (XCHAR* text, XLANGUAGE_CODE languagetextcode, XLANGUAGE_CODE languagetranslationcode, XSTRING& translation, int timeoutforurl = DIOSCRAPERWEB_MAXTIMEOUTFORURL, XSTRING* localIP = NULL, bool usecache = true);
    bool                    Get                             (XSTRING& text, XLANGUAGE_CODE languagetextcode, XLANGUAGE_CODE languagetranslationcode, XSTRING& translation, int timeoutforurl = DIOSCRAPERWEB_MAXTIMEOUTFORURL, XSTRING* localIP = NULL, bool usecache = true);

  private:

    bool                    GoogleLanguageCode              (XLANGUAGE_CODE languagecode, XSTRING& googlecode);

    void                    Clean                           ();

    XSTRING                 text;
    XLANGUAGE_CODE          languagetextcode;
    XLANGUAGE_CODE          languagetranslationcode;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





