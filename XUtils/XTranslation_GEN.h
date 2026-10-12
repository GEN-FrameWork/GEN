/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XTranslation_GEN.h
* 
* @class      XTRANSLATION_GEN
* @brief      eXtension Translation of GEN class (sencences within the framework)
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

#include "XString.h"

#include "XTranslation_GEN_ID.h"


#define GEN_TL(ID)                        XTRANSLATION_GEN::GetInstance().GetSentence(ID)


/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XTRANSLATION_GEN_SENTENCE
{
  public:

                                          XTRANSLATION_GEN_SENTENCE          ();
    virtual                              ~XTRANSLATION_GEN_SENTENCE          ();

    void                                  Clean                              ();

    XDWORD                                ID;
    XDWORD                                codelanguage;
    XCHAR*                                sentence;
    XDWORD                                fixed;
};

class XTRANSLATION_GEN
{
  public:

    static bool                           GetIsInstanced                      ();
    static XTRANSLATION_GEN&              GetInstance                         ();
    static bool                           DelInstance                         ();

    bool                                  IsLanguageAvailable                 (XDWORD code);

    XDWORD                                GetActual                           ();
    bool                                  SetActual                           ();
    bool                                  SetActual                           (XDWORD codelanguage);

    XCHAR*                                GetSentence                         (XDWORD ID);

    bool                                  Sentence_Add                        (XDWORD ID, XDWORD codelanguage, XCHAR* sentence, XDWORD fixed = 0);
    XTRANSLATION_GEN_SENTENCE*            Sentence_Get                        (XDWORD ID, XDWORD codelanguage);
    XTRANSLATION_GEN_SENTENCE*            Sentence_FindByText                 (XCHAR* sentence, bool ignorecase = false);

    bool                                  Sentences_AddAll                    ();
    bool                                  Sentences_DeleteAll                 ();

  private:
                                          XTRANSLATION_GEN                    ();
                                          XTRANSLATION_GEN                    (XTRANSLATION_GEN const&);
    virtual                              ~XTRANSLATION_GEN                    ();

    void                                  operator =                          (XTRANSLATION_GEN const&);

    void                                  Clean                               ();

    static XTRANSLATION_GEN*              instance;

    XDWORD                                codelanguage;
    XSTRING                               emptysentence;
    XVECTOR<XDWORD>                       languageavailables;
    XVECTOR<XTRANSLATION_GEN_SENTENCE*>   sentences;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/






