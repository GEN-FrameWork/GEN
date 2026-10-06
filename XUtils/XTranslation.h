/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XTranslation.h
* 
* @class      XTRANSLATION
* @brief      eXtended Utils Language class
* @ingroup    XUTILS
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
#include "XString.h"
#include "XPath.h"

#include "XLanguage_ISO_639_3.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define XTRANSLATION_NAMEFILEEXT         _L(".lng")

#define XTRANSLATION_GENRESERVE          500
#ifndef MICROCONTROLLER
#define XTRANSLATION_MAXSENTENCES        10000
#else
#define XTRANSLATION_MAXSENTENCES        100
#endif

#define XT_L(ID)                         GEN_XTRANSLATION.Translate_GetSentence(ID)

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XPATH;

class XTRANSLATION
{
  public:

    static bool                       GetIsInstanced                      ();
    static XTRANSLATION&              GetInstance                         ();
    static bool                       DelInstance                         ();

    #ifdef XFILE_ACTIVE
    bool                              Ini                                 (XPATH& xpath);
    #else
    bool                              Ini                                 ();
    #endif

    XDWORD                            GetActual                           ();
    virtual bool                      SetActual                           (XDWORD code);

    bool                              Translate_Add                       (XDWORD ID, XCHAR* sentence, XDWORD fixed);
    XCHAR*                            Translate_GetSentence               (XDWORD ID);

    bool                              End                                 ();

  private:
                                      XTRANSLATION                        ();
                                      XTRANSLATION                        (XTRANSLATION const&);
    virtual                          ~XTRANSLATION                        ();

    void                              operator =                          (XTRANSLATION const&);

    bool                              LoadLanguageAvailable               ();
    bool                              IsLanguageAvailable                 (XDWORD code);
    bool                              AddLanguageAvailable                (XDWORD code);
    int                               GetIndexLanguageByCode              (XDWORD code);

    bool                              Translate_Load                      ();
    bool                              Translate_Delete                    ();

    void                              Clean                               ();

    static XTRANSLATION*              instance;

    #ifdef XFILE_ACTIVE
    XPATH                             xpath;
    #endif

    XDWORD                            code;

    XLANGUAGE_ISO_639_3               ISO_639_3;
    XVECTOR<XDWORD>                   languageavailables;
    XSTRING                           emptysentence;
    XCHAR*                            sentences[XTRANSLATION_MAXSENTENCES];
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





