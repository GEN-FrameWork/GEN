/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOScraperWebTranslation.h
* 
* @class      DIOSCRAPERWEBTRANSLATION
* @brief      Typed Translation scraper filled by script (not XML)
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

#ifdef DIO_SCRAPERWEB_TRANSLATION_ACTIVE

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XString.h"

#include "DIOScraperWebCache.h"
#include "DIOScraperScript.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOSCRAPERWEBTRANSLATION_SCRIPTPATH    _L("translation.g")
#define DIOSCRAPERWEBTRANSLATION_MAXTIMEOUT    DIOSCRAPERSCRIPT_DEFAULT_TIMEOUT

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XMUTEX;

class DIOTRANSLATION_RESULT : public DIOSCRAPERWEBCACHE_RESULT
{
  public:

                            DIOTRANSLATION_RESULT             ();
    virtual                ~DIOTRANSLATION_RESULT             ();

    XCHAR*                  GetSourceLanguage                 ();
    XCHAR*                  GetTargetLanguage                 ();
    XCHAR*                  GetTranslation                    ();

    bool                    IsEmpty                           ();

    virtual bool            CopyFrom                          (DIOSCRAPERWEBCACHE_RESULT* result);
    virtual bool            CopyTo                            (DIOSCRAPERWEBCACHE_RESULT* result);

    bool                    Set                               (XSTRING& sourcelanguage, XSTRING& targetlanguage, XSTRING& translation);
    bool                    Set                               (XCHAR* sourcelanguage, XCHAR* targetlanguage, XCHAR* translation);

  private:

    void                    Clean                             ();

    XSTRING                 sourcelanguage;
    XSTRING                 targetlanguage;
    XSTRING                 translation;
};


class DIOSCRAPERWEBTRANSLATION
{
  public:
                            DIOSCRAPERWEBTRANSLATION          ();
    virtual                ~DIOSCRAPERWEBTRANSLATION          ();

    bool                    Get                               (XCHAR* text, XCHAR* sourcelanguage, XCHAR* targetlanguage, DIOTRANSLATION_RESULT& result, int timeoutforurl = DIOSCRAPERWEBTRANSLATION_MAXTIMEOUT, XSTRING* localIP = NULL, bool usecache = true);
    bool                    Get                               (XSTRING& text, XSTRING& sourcelanguage, XSTRING& targetlanguage, DIOTRANSLATION_RESULT& result, int timeoutforurl = DIOSCRAPERWEBTRANSLATION_MAXTIMEOUT, XSTRING* localIP = NULL, bool usecache = true);
    bool                    Get                               (XCHAR* text, XCHAR* sourcelanguage, XCHAR* targetlanguage, XSTRING& translation, int timeoutforurl = DIOSCRAPERWEBTRANSLATION_MAXTIMEOUT, XSTRING* localIP = NULL, bool usecache = true);
    bool                    Get                               (XSTRING& text, XSTRING& sourcelanguage, XSTRING& targetlanguage, XSTRING& translation, int timeoutforurl = DIOSCRAPERWEBTRANSLATION_MAXTIMEOUT, XSTRING* localIP = NULL, bool usecache = true);

    bool                    SetScriptPath                     (XCHAR* relativescriptpath);
    XCHAR*                  GetScriptPath                     ();

  private:

    bool                    DecodeTranslationText             (XSTRING& text);
    void                    Clean                             ();

    DIOSCRAPERWEBCACHE*     cache;
    XMUTEX*                 xmutexdo;
    XSTRING                 scriptpath;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/

#endif
