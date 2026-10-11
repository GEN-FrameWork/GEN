/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XTranslation_GEN.cpp
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XTranslation_GEN.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "XLanguage_ISO_639_3.h"

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

XTRANSLATION_GEN* XTRANSLATION_GEN::instance  = NULL;

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTRANSLATION_GEN_SENTENCE::XTRANSLATION_GEN_SENTENCE()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTRANSLATION_GEN_SENTENCE::XTRANSLATION_GEN_SENTENCE()
{
  Clean();
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTRANSLATION_GEN_SENTENCE::~XTRANSLATION_GEN_SENTENCE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTRANSLATION_GEN_SENTENCE::~XTRANSLATION_GEN_SENTENCE()
{
  if(sentence) GEN_DELETE_ARRAY sentence;

  Clean();
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTRANSLATION_GEN_SENTENCE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTRANSLATION_GEN_SENTENCE::Clean()
{
  ID            = 0;
  codelanguage  = 0;
  sentence      = NULL;
  fixed         = 0;
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRANSLATION_GEN::GetIsInstanced()
* @brief      Get is instanced
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRANSLATION_GEN::GetIsInstanced()
{
  return instance!=NULL;
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTRANSLATION_GEN& XTRANSLATION_GEN::GetInstance()
* @brief      Get instance
* @ingroup    XUTILS
* 
* @return     XTRANSLATION_GEN& : Reference to the requested object.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTRANSLATION_GEN& XTRANSLATION_GEN::GetInstance()
{
  if(!instance) instance = GEN_NEW XTRANSLATION_GEN();

  return (*instance);
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRANSLATION_GEN::DelInstance()
* @brief      Del instance
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRANSLATION_GEN::DelInstance()
{
  if(instance)
    {
      GEN_DELETE instance;
      instance = NULL;

      return true;
    }

  return false;
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTRANSLATION_GEN::XTRANSLATION_GEN()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTRANSLATION_GEN::XTRANSLATION_GEN()
{
  Clean();

  Sentences_AddAll();
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTRANSLATION_GEN::~XTRANSLATION_GEN()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTRANSLATION_GEN::~XTRANSLATION_GEN()
{
  Sentences_DeleteAll();

  Clean();
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRANSLATION_GEN::IsLanguageAvailable(XDWORD code)
* @brief      Is language available
* @ingroup    XUTILS
* 
* @param[in]  code : Code value.
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRANSLATION_GEN::IsLanguageAvailable(XDWORD code)
{
  for(XDWORD c=0; c<languageavailables.GetSize(); c++)
    {
      if(code == languageavailables.Get(c)) return true;
    }

  return false;
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XTRANSLATION_GEN::GetActual()
* @brief      Get actual
* @ingroup    XUTILS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XTRANSLATION_GEN::GetActual()
{
  return codelanguage;
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRANSLATION_GEN::SetActual(XDWORD codelanguage)
* @brief      Set actual
* @ingroup    XUTILS
* 
* @param[in]  codelanguage : Code language value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRANSLATION_GEN::SetActual(XDWORD codelanguage)
{
  if(this->codelanguage == codelanguage) return false;

  if(!IsLanguageAvailable(codelanguage)) return false;

  this->codelanguage = codelanguage;

  return true;
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* XTRANSLATION_GEN::GetSentence(XDWORD ID)
* @brief      Get sentence
* @ingroup    XUTILS
* 
* @param[in]  ID : Identifier to use.
* 
* @return     XCHAR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* XTRANSLATION_GEN::GetSentence(XDWORD ID)
{
  XTRANSLATION_GEN_SENTENCE* sentence = Sentence_Get(ID, codelanguage);
  if(!sentence || !sentence->sentence)
    {
      return emptysentence.Get();
    }

  return sentence->sentence;
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRANSLATION_GEN::Sentence_Add(XDWORD ID, XDWORD codelanguage, XCHAR* sentence, XDWORD fixed)
* @brief      Sentence add
* @ingroup    XUTILS
* 
* @param[in]  ID : Identifier to use.
* @param[in]  codelanguage : Codelanguage value.
* @param[in]  sentence : Sentence pointer to use.
* @param[in]  fixed : Fixed value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRANSLATION_GEN::Sentence_Add(XDWORD ID, XDWORD codelanguage, XCHAR* sentence, XDWORD fixed)
{
  if(!ID)           return false;
  if(!codelanguage) return false;
  if(!sentence)     return false;

  if(Sentence_Get(ID, codelanguage)) return false;

  XTRANSLATION_GEN_SENTENCE* sentenceGEN = GEN_NEW XTRANSLATION_GEN_SENTENCE();
  if(!sentenceGEN) return false;

  int sizesentence = XSTRING::GetSize(sentence);

  sentenceGEN->ID           = ID;
  sentenceGEN->codelanguage = codelanguage;
  sentenceGEN->sentence     = GEN_NEW XCHAR[sizesentence+1];
  sentenceGEN->fixed        = fixed;
  if(!sentenceGEN->sentence)
    {
      GEN_DELETE sentenceGEN;
      return false;
    }

  memset(sentenceGEN->sentence, 0, (sizesentence+1) * sizeof(XCHAR));
  memcpy(sentenceGEN->sentence, sentence, sizesentence     * sizeof(XCHAR));

  sentences.Add(sentenceGEN);

  return true;
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTRANSLATION_GEN_SENTENCE* XTRANSLATION_GEN::Sentence_Get(XDWORD ID, XDWORD codelanguage)
* @brief      Sentence get
* @ingroup    XUTILS
* 
* @param[in]  ID : Identifier to use.
* @param[in]  codelanguage : Codelanguage value.
* 
* @return     XTRANSLATION_GEN_SENTENCE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTRANSLATION_GEN_SENTENCE* XTRANSLATION_GEN::Sentence_Get(XDWORD ID, XDWORD codelanguage)
{
  for(XDWORD c=0; c<sentences.GetSize(); c++)
    {
      XTRANSLATION_GEN_SENTENCE* sentence = sentences.Get(c);
      if(sentence)
        {
          if((sentence->ID == ID) && (sentence->codelanguage == codelanguage)) return sentence;
        }
    }

  return NULL;
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTRANSLATION_GEN_SENTENCE* XTRANSLATION_GEN::Sentence_FindByText(XCHAR* sentence, bool ignorecase)
* @brief      Find the first GEN internal sentence whose text matches
* @ingroup    XUTILS
* 
* @param[in]  sentence : Sentence text to look for.
* @param[in]  ignorecase : Ignorecase value.
* 
* @return     XTRANSLATION_GEN_SENTENCE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTRANSLATION_GEN_SENTENCE* XTRANSLATION_GEN::Sentence_FindByText(XCHAR* sentence, bool ignorecase)
{
  if(!sentence)
    {
      return NULL;
    }

  XSTRING needle(sentence);
  if(needle.IsEmpty())
    {
      return NULL;
    }

  for(XDWORD c=0; c<sentences.GetSize(); c++)
    {
      XTRANSLATION_GEN_SENTENCE* candidate = sentences.Get(c);
      if(!candidate || !candidate->sentence)
        {
          continue;
        }

      XSTRING haystack(candidate->sentence);
      if(!haystack.Compare(needle, ignorecase))
        {
          return candidate;
        }
    }

  return NULL;
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRANSLATION_GEN::Sentences_AddAll()
* @brief      Sentences add all
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRANSLATION_GEN::Sentences_AddAll()
{
  languageavailables.DeleteAll();

  languageavailables.Add(XLANGUAGE_ISO_639_3_CODE_ENG);
  languageavailables.Add(XLANGUAGE_ISO_639_3_CODE_SPA);
  languageavailables.Add(XLANGUAGE_ISO_639_3_CODE_FRE);
  languageavailables.Add(XLANGUAGE_ISO_639_3_CODE_GER);
  languageavailables.Add(XLANGUAGE_ISO_639_3_CODE_CHI);

  #ifdef ANONYMOUS_MODE

  #else

  //--- GENERIC  -----------------------------------------------------------------------------------------------------

  Sentence_Add(XTRANSLATION_GEN_ID_OK, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Ok"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_OK, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Correcto"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_OK, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Correct"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_OK, XLANGUAGE_ISO_639_3_CODE_GER, NL("Korrekt"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_OK, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u6b63\u786e"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_ERROR, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Error"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_ERROR, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Error"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_ERROR, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Erreur"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_ERROR, XLANGUAGE_ISO_639_3_CODE_GER, NL("Fehler"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_ERROR, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u9519\u8bef"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_YES, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Yes"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_YES, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Si"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_YES, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Oui"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_YES, XLANGUAGE_ISO_639_3_CODE_GER, NL("Ja"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_YES, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u662f"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_NO, XLANGUAGE_ISO_639_3_CODE_ENG, NL("No"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_NO, XLANGUAGE_ISO_639_3_CODE_SPA, NL("No"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_NO, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Non"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_NO, XLANGUAGE_ISO_639_3_CODE_GER, NL("Nein"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_NO, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u5426"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_TRUE, XLANGUAGE_ISO_639_3_CODE_ENG, NL("True"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_TRUE, XLANGUAGE_ISO_639_3_CODE_SPA, NL("true"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_TRUE, XLANGUAGE_ISO_639_3_CODE_FRE, NL("vrai"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_TRUE, XLANGUAGE_ISO_639_3_CODE_GER, NL("wahr"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_TRUE, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u771f"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_FALSE, XLANGUAGE_ISO_639_3_CODE_ENG, NL("False"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_FALSE, XLANGUAGE_ISO_639_3_CODE_SPA, NL("false"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_FALSE, XLANGUAGE_ISO_639_3_CODE_FRE, NL("faux"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_FALSE, XLANGUAGE_ISO_639_3_CODE_GER, NL("falsch"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_FALSE, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u5047"), 0);

  //--- DATETIME -----------------------------------------------------------------------------------------------------

  // MONTHS

  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_JANUARY, XLANGUAGE_ISO_639_3_CODE_ENG, NL("January"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_JANUARY, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Enero"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_JANUARY, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Janvier"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_JANUARY, XLANGUAGE_ISO_639_3_CODE_GER, NL("Januar"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_JANUARY, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u4e00\u6708"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_FEBRUARY, XLANGUAGE_ISO_639_3_CODE_ENG, NL("February"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_FEBRUARY, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Febrero"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_FEBRUARY, XLANGUAGE_ISO_639_3_CODE_FRE, NL("F\u00e9vrier"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_FEBRUARY, XLANGUAGE_ISO_639_3_CODE_GER, NL("Februar"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_FEBRUARY, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u4e8c\u6708"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_MARCH, XLANGUAGE_ISO_639_3_CODE_ENG, NL("March"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_MARCH, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Marzo"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_MARCH, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Mars"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_MARCH, XLANGUAGE_ISO_639_3_CODE_GER, NL("M\u00e4rz"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_MARCH, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u4e09\u6708"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_APRIL, XLANGUAGE_ISO_639_3_CODE_ENG, NL("April"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_APRIL, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Abril"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_APRIL, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Avril"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_APRIL, XLANGUAGE_ISO_639_3_CODE_GER, NL("April"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_APRIL, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u56db\u6708"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_MAY, XLANGUAGE_ISO_639_3_CODE_ENG, NL("May"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_MAY, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Mayo"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_MAY, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Mai"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_MAY, XLANGUAGE_ISO_639_3_CODE_GER, NL("Mai"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_MAY, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u4e94\u6708"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_JUNE, XLANGUAGE_ISO_639_3_CODE_ENG, NL("June"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_JUNE, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Junio"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_JUNE, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Juin"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_JUNE, XLANGUAGE_ISO_639_3_CODE_GER, NL("Juni"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_JUNE, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u516d\u6708"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_JULY, XLANGUAGE_ISO_639_3_CODE_ENG, NL("July"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_JULY, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Julio"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_JULY, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Juillet"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_JULY, XLANGUAGE_ISO_639_3_CODE_GER, NL("Juli"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_JULY, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u4e03\u6708"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_AUGUST, XLANGUAGE_ISO_639_3_CODE_ENG, NL("August"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_AUGUST, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Agosto"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_AUGUST, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Ao\u00fbt"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_AUGUST, XLANGUAGE_ISO_639_3_CODE_GER, NL("August"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_AUGUST, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u516b\u6708"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_SEPTEMBER, XLANGUAGE_ISO_639_3_CODE_ENG, NL("September"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_SEPTEMBER, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Septiembre"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_SEPTEMBER, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Septembre"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_SEPTEMBER, XLANGUAGE_ISO_639_3_CODE_GER, NL("September"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_SEPTEMBER, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u4e5d\u6708"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_OCTOBER, XLANGUAGE_ISO_639_3_CODE_ENG, NL("October"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_OCTOBER, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Octubre"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_OCTOBER, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Octobre"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_OCTOBER, XLANGUAGE_ISO_639_3_CODE_GER, NL("Oktober"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_OCTOBER, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u5341\u6708"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_NOVEMBER, XLANGUAGE_ISO_639_3_CODE_ENG, NL("November"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_NOVEMBER, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Noviembre"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_NOVEMBER, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Novembre"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_NOVEMBER, XLANGUAGE_ISO_639_3_CODE_GER, NL("November"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_NOVEMBER, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u5341\u4e00\u6708"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_DECEMBER, XLANGUAGE_ISO_639_3_CODE_ENG, NL("December"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_DECEMBER, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Diciembre"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_DECEMBER, XLANGUAGE_ISO_639_3_CODE_FRE, NL("D\u00e9cembre"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_DECEMBER, XLANGUAGE_ISO_639_3_CODE_GER, NL("Dezember"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_DECEMBER, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u5341\u4e8c\u6708"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_PRESEPARATOR, XLANGUAGE_ISO_639_3_CODE_ENG, NL("of"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_PRESEPARATOR, XLANGUAGE_ISO_639_3_CODE_SPA, NL("de"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_PRESEPARATOR, XLANGUAGE_ISO_639_3_CODE_FRE, NL("de"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_PRESEPARATOR, XLANGUAGE_ISO_639_3_CODE_GER, NL("von"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_PRESEPARATOR, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u00b7"), 0);

  // DAYOFWEEK

  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_SUNDAY, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Sunday"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_SUNDAY, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Domingo"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_SUNDAY, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Dimanche"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_SUNDAY, XLANGUAGE_ISO_639_3_CODE_GER, NL("Sonntag"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_SUNDAY, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u661f\u671f\u65e5"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_MONDAY, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Monday"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_MONDAY, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Lunes"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_MONDAY, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Lundi"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_MONDAY, XLANGUAGE_ISO_639_3_CODE_GER, NL("Montag"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_MONDAY, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u661f\u671f\u4e00"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_TUESDAY, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Tuesday"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_TUESDAY, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Martes"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_TUESDAY, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Mardi"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_TUESDAY, XLANGUAGE_ISO_639_3_CODE_GER, NL("Dienstag"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_TUESDAY, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u661f\u671f\u4e8c"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_WEDNESDAY, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Wednesday"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_WEDNESDAY, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Miercoles"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_WEDNESDAY, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Mercredi"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_WEDNESDAY, XLANGUAGE_ISO_639_3_CODE_GER, NL("Mittwoch"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_WEDNESDAY, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u661f\u671f\u4e09"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_THURSDAY, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Thursday"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_THURSDAY, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Jueves"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_THURSDAY, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Jeudi"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_THURSDAY, XLANGUAGE_ISO_639_3_CODE_GER, NL("Donnerstag"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_THURSDAY, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u661f\u671f\u56db"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_FRIDAY, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Friday"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_FRIDAY, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Viernes"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_FRIDAY, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Vendredi"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_FRIDAY, XLANGUAGE_ISO_639_3_CODE_GER, NL("Freitag"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_FRIDAY, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u661f\u671f\u4e94"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_SATURDAY, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Saturday"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_SATURDAY, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Sabado"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_SATURDAY, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Samedi"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_SATURDAY, XLANGUAGE_ISO_639_3_CODE_GER, NL("Samstag"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_SATURDAY, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u661f\u671f\u516d"), 0);

  //--- XTIMER -------------------------------------------------------------------------------------------------------

  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_XX_YEARS, XLANGUAGE_ISO_639_3_CODE_ENG, NL("%d years"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_XX_YEARS, XLANGUAGE_ISO_639_3_CODE_SPA, NL("%d a\xf1os"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_XX_YEARS, XLANGUAGE_ISO_639_3_CODE_FRE, NL("%d ans"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_XX_YEARS, XLANGUAGE_ISO_639_3_CODE_GER, NL("%d Jahre"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_XX_YEARS, XLANGUAGE_ISO_639_3_CODE_CHI, NL("%d\u5e74"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ONE_YEAR, XLANGUAGE_ISO_639_3_CODE_ENG, NL("one year"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ONE_YEAR, XLANGUAGE_ISO_639_3_CODE_SPA, NL("un a\xf1o"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ONE_YEAR, XLANGUAGE_ISO_639_3_CODE_FRE, NL("un an"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ONE_YEAR, XLANGUAGE_ISO_639_3_CODE_GER, NL("ein Jahr"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ONE_YEAR, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u4e00\u5e74"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_XX_MONTHS, XLANGUAGE_ISO_639_3_CODE_ENG, NL("%d months"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_XX_MONTHS, XLANGUAGE_ISO_639_3_CODE_SPA, NL("%d meses"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_XX_MONTHS, XLANGUAGE_ISO_639_3_CODE_FRE, NL("%d mois"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_XX_MONTHS, XLANGUAGE_ISO_639_3_CODE_GER, NL("%d Monate"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_XX_MONTHS, XLANGUAGE_ISO_639_3_CODE_CHI, NL("%d\u4e2a\u6708"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ONE_MONTH, XLANGUAGE_ISO_639_3_CODE_ENG, NL("one month"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ONE_MONTH, XLANGUAGE_ISO_639_3_CODE_SPA, NL("un mes"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ONE_MONTH, XLANGUAGE_ISO_639_3_CODE_FRE, NL("un mois"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ONE_MONTH, XLANGUAGE_ISO_639_3_CODE_GER, NL("ein Monat"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ONE_MONTH, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u4e00\u4e2a\u6708"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_XX_DAYS, XLANGUAGE_ISO_639_3_CODE_ENG, NL("%d days"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_XX_DAYS, XLANGUAGE_ISO_639_3_CODE_SPA, NL("%d dias"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_XX_DAYS, XLANGUAGE_ISO_639_3_CODE_FRE, NL("%d jours"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_XX_DAYS, XLANGUAGE_ISO_639_3_CODE_GER, NL("%d Tage"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_XX_DAYS, XLANGUAGE_ISO_639_3_CODE_CHI, NL("%d\u5929"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ONE_DAY, XLANGUAGE_ISO_639_3_CODE_ENG, NL("one day"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ONE_DAY, XLANGUAGE_ISO_639_3_CODE_SPA, NL("un dia"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ONE_DAY, XLANGUAGE_ISO_639_3_CODE_FRE, NL("un jour"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ONE_DAY, XLANGUAGE_ISO_639_3_CODE_GER, NL("ein Tag"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ONE_DAY, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u4e00\u5929"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_XX_HOURS, XLANGUAGE_ISO_639_3_CODE_ENG, NL("%d hours"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_XX_HOURS, XLANGUAGE_ISO_639_3_CODE_SPA, NL("%d horas"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_XX_HOURS, XLANGUAGE_ISO_639_3_CODE_FRE, NL("%d heures"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_XX_HOURS, XLANGUAGE_ISO_639_3_CODE_GER, NL("%d Stunden"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_XX_HOURS, XLANGUAGE_ISO_639_3_CODE_CHI, NL("%d\u5c0f\u65f6"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ONE_HOUR, XLANGUAGE_ISO_639_3_CODE_ENG, NL("one hour"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ONE_HOUR, XLANGUAGE_ISO_639_3_CODE_SPA, NL("una hora"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ONE_HOUR, XLANGUAGE_ISO_639_3_CODE_FRE, NL("une heure"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ONE_HOUR, XLANGUAGE_ISO_639_3_CODE_GER, NL("eine Stunde"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ONE_HOUR, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u4e00\u5c0f\u65f6"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_XX_MINUTES, XLANGUAGE_ISO_639_3_CODE_ENG, NL("%d minutes"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_XX_MINUTES, XLANGUAGE_ISO_639_3_CODE_SPA, NL("%d minutos"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_XX_MINUTES, XLANGUAGE_ISO_639_3_CODE_FRE, NL("%d minutes"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_XX_MINUTES, XLANGUAGE_ISO_639_3_CODE_GER, NL("%d Minuten"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_XX_MINUTES, XLANGUAGE_ISO_639_3_CODE_CHI, NL("%d\u5206\u949f"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ONE_MINUTE, XLANGUAGE_ISO_639_3_CODE_ENG, NL("one minute"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ONE_MINUTE, XLANGUAGE_ISO_639_3_CODE_SPA, NL("un minuto"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ONE_MINUTE, XLANGUAGE_ISO_639_3_CODE_FRE, NL("une minute"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ONE_MINUTE, XLANGUAGE_ISO_639_3_CODE_GER, NL("eine Minute"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ONE_MINUTE, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u4e00\u5206\u949f"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_XX_SECONDS, XLANGUAGE_ISO_639_3_CODE_ENG, NL("%d seconds"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_XX_SECONDS, XLANGUAGE_ISO_639_3_CODE_SPA, NL("%d segundos"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_XX_SECONDS, XLANGUAGE_ISO_639_3_CODE_FRE, NL("%d secondes"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_XX_SECONDS, XLANGUAGE_ISO_639_3_CODE_GER, NL("%d Sekunden"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_XX_SECONDS, XLANGUAGE_ISO_639_3_CODE_CHI, NL("%d\u79d2"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ONE_SECOND, XLANGUAGE_ISO_639_3_CODE_ENG, NL("one second"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ONE_SECOND, XLANGUAGE_ISO_639_3_CODE_SPA, NL("un segundo"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ONE_SECOND, XLANGUAGE_ISO_639_3_CODE_FRE, NL("une seconde"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ONE_SECOND, XLANGUAGE_ISO_639_3_CODE_GER, NL("eine Sekunde"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ONE_SECOND, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u4e00\u79d2"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ZERO_SECONDS, XLANGUAGE_ISO_639_3_CODE_ENG, NL("zero seconds"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ZERO_SECONDS, XLANGUAGE_ISO_639_3_CODE_SPA, NL("cero segundos"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ZERO_SECONDS, XLANGUAGE_ISO_639_3_CODE_FRE, NL("z\u00e9ro secondes"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ZERO_SECONDS, XLANGUAGE_ISO_639_3_CODE_GER, NL("null Sekunden"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ZERO_SECONDS, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u96f6\u79d2"), 0);

  //--- APPLICATION LOG ----------------------------------------------------------------------------------------------

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INILOG, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Activating LOG system"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INILOG, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Activando sistema de LOG"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INILOG, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Activation du syst\u00e8me LOG"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INILOG, XLANGUAGE_ISO_639_3_CODE_GER, NL("LOG-System aktivieren"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INILOG, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u6fc0\u6d3b\u65e5\u5fd7\u7cfb\u7edf"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_ENDLOG, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Disabling LOG system"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_ENDLOG, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Desactivando sistema de LOG"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_ENDLOG, XLANGUAGE_ISO_639_3_CODE_FRE, NL("D\u00e9sactivation du syst\u00e8me LOG"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_ENDLOG, XLANGUAGE_ISO_639_3_CODE_GER, NL("LOG-System deaktivieren"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_ENDLOG, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u5173\u95ed\u65e5\u5fd7\u7cfb\u7edf"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INIAPPSTATUS, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Activating App status"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INIAPPSTATUS, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Activando estado aplicacion"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INIAPPSTATUS, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Activation de l'\u00e9tat de l'application"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INIAPPSTATUS, XLANGUAGE_ISO_639_3_CODE_GER, NL("Anwendungsstatus aktivieren"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INIAPPSTATUS, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u6fc0\u6d3b\u5e94\u7528\u72b6\u6001"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_ENDAPPSTATUS, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Disabling App status"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_ENDAPPSTATUS, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Desactivando estado aplicacion"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_ENDAPPSTATUS, XLANGUAGE_ISO_639_3_CODE_FRE, NL("D\u00e9sactivation de l'\u00e9tat de l'application"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_ENDAPPSTATUS, XLANGUAGE_ISO_639_3_CODE_GER, NL("Anwendungsstatus deaktivieren"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_ENDAPPSTATUS, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u5173\u95ed\u5e94\u7528\u72b6\u6001"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INIINTERNETSTATUS, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Activating Internet status"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INIINTERNETSTATUS, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Activando estado Internet"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INIINTERNETSTATUS, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Activation de l'\u00e9tat Internet"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INIINTERNETSTATUS, XLANGUAGE_ISO_639_3_CODE_GER, NL("Internetstatus aktivieren"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INIINTERNETSTATUS, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u6fc0\u6d3b\u4e92\u8054\u7f51\u72b6\u6001"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_ENDINTERNETSTATUS, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Disabling Internet status"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_ENDINTERNETSTATUS, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Desactivando estado Internet"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_ENDINTERNETSTATUS, XLANGUAGE_ISO_639_3_CODE_FRE, NL("D\u00e9sactivation de l'\u00e9tat Internet"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_ENDINTERNETSTATUS, XLANGUAGE_ISO_639_3_CODE_GER, NL("Internetstatus deaktivieren"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_ENDINTERNETSTATUS, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u5173\u95ed\u4e92\u8054\u7f51\u72b6\u6001"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPROOTPATH, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Application Root Path"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPROOTPATH, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Camino raiz de application"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPROOTPATH, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Chemin racine de l'application"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPROOTPATH, XLANGUAGE_ISO_639_3_CODE_GER, NL("Anwendungsstammverzeichnis"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPROOTPATH, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u5e94\u7528\u6839\u8def\u5f84"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPVERSION, XLANGUAGE_ISO_639_3_CODE_ENG, NL("APP version"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPVERSION, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Version APP"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPVERSION, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Version APP"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPVERSION, XLANGUAGE_ISO_639_3_CODE_GER, NL("APP-Version"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPVERSION, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u5e94\u7528\u7248\u672c"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_SOVERSION, XLANGUAGE_ISO_639_3_CODE_ENG, NL("S.O. version"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_SOVERSION, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Version S.O."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_SOVERSION, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Version S.O."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_SOVERSION, XLANGUAGE_ISO_639_3_CODE_GER, NL("BS-Version"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_SOVERSION, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u7cfb\u7edf\u7248\u672c"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_TOTALMEMORY, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Total memory %d Kb, free %d Kb (%d%%)."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_TOTALMEMORY, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Memoria total %d Kb, libre %d Kb (%d%%)."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_TOTALMEMORY, XLANGUAGE_ISO_639_3_CODE_FRE, NL("M\u00e9moire totale %d Kb, libre %d Kb (%d%%)."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_TOTALMEMORY, XLANGUAGE_ISO_639_3_CODE_GER, NL("Gesamtspeicher %d Kb, frei %d Kb (%d%%)."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_TOTALMEMORY, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u603b\u5185\u5b58 %d Kb\uff0c\u7a7a\u95f2 %d Kb (%d%%)\u3002"), 0);

  //--- APPLICATION CONSOLE EXIT CODE --------------------------------------------------------------------------------

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_NOTINFO, XLANGUAGE_ISO_639_3_CODE_ENG, NL("No closing information"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_NOTINFO, XLANGUAGE_ISO_639_3_CODE_SPA, NL("No hay informacion de cierre"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_NOTINFO, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Aucune information de fermeture"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_NOTINFO, XLANGUAGE_ISO_639_3_CODE_GER, NL("Keine Informationen zum Schliessen"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_NOTINFO, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u6ca1\u6709\u5173\u95ed\u4fe1\u606f"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_SERIOUSERROR, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Application closed due to a serious error"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_SERIOUSERROR, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Aplicacion cerrada debido a un error grave"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_SERIOUSERROR, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Application ferm\u00e9e en raison d'une erreur grave"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_SERIOUSERROR, XLANGUAGE_ISO_639_3_CODE_GER, NL("Anwendung wegen eines schweren Fehlers geschlossen"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_SERIOUSERROR, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u5e94\u7528\u56e0\u4e25\u91cd\u9519\u8bef\u800c\u5173\u95ed"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_UPDATE, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Application closed by update"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_UPDATE, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Aplicacion cerrada por actualizacion"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_UPDATE, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Application ferm\u00e9e par mise \u00e0 jour"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_UPDATE, XLANGUAGE_ISO_639_3_CODE_GER, NL("Anwendung durch Aktualisierung geschlossen"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_UPDATE, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u5e94\u7528\u56e0\u66f4\u65b0\u800c\u5173\u95ed"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_APPLICATION, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Execution of the application concluded"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_APPLICATION, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Ejecucion de la aplicacion concluida"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_APPLICATION, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Ex\u00e9cution de l'application termin\u00e9e"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_APPLICATION, XLANGUAGE_ISO_639_3_CODE_GER, NL("Ausf\u00fchrung der Anwendung beendet"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_APPLICATION, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u5e94\u7528\u6267\u884c\u5df2\u7ed3\u675f"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_USER, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Application closed by the user"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_USER, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Aplicacion cerrada por el usuario"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_USER, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Application ferm\u00e9e par l'utilisateur"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_USER, XLANGUAGE_ISO_639_3_CODE_GER, NL("Anwendung vom Benutzer geschlossen"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_USER, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u5e94\u7528\u7531\u7528\u6237\u5173\u95ed"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_SHUTDOWN, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Application closed by shutdown of the operating system"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_SHUTDOWN, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Aplicacion cerrada por apagado del sistema operativo"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_SHUTDOWN, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Application ferm\u00e9e par arr\u00eat du syst\u00e8me"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_SHUTDOWN, XLANGUAGE_ISO_639_3_CODE_GER, NL("Anwendung durch Herunterfahren des Systems geschlossen"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_SHUTDOWN, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u5e94\u7528\u56e0\u7cfb\u7edf\u5173\u673a\u800c\u5173\u95ed"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_INVALIDLICENSE, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Application closed by invalid license"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_INVALIDLICENSE, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Aplication cerrada por licencia invalida"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_INVALIDLICENSE, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Application ferm\u00e9e pour licence invalide"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_INVALIDLICENSE, XLANGUAGE_ISO_639_3_CODE_GER, NL("Anwendung wegen ung\u00fcltiger Lizenz geschlossen"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_INVALIDLICENSE, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u5e94\u7528\u56e0\u8bb8\u53ef\u8bc1\u65e0\u6548\u800c\u5173\u95ed"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_EXPIREDLICENSE, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Closed application for expired license"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_EXPIREDLICENSE, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Aplication cerrada por licencia vencida"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_EXPIREDLICENSE, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Application ferm\u00e9e pour licence expir\u00e9e"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_EXPIREDLICENSE, XLANGUAGE_ISO_639_3_CODE_GER, NL("Anwendung wegen abgelaufener Lizenz geschlossen"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_EXPIREDLICENSE, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u5e94\u7528\u56e0\u8bb8\u53ef\u8bc1\u8fc7\u671f\u800c\u5173\u95ed"), 0);

  //--- APPLICATION STATUS SHOW --------------------------------------------------------------------------------------

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_OSVERSION, XLANGUAGE_ISO_639_3_CODE_ENG, NL("O.S Version"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_OSVERSION, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Version S.O."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_OSVERSION, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Version S.O."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_OSVERSION, XLANGUAGE_ISO_639_3_CODE_GER, NL("BS-Version"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_OSVERSION, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u7cfb\u7edf\u7248\u672c"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_CPUMEMORY, XLANGUAGE_ISO_639_3_CODE_ENG, NL("CPU Memory"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_CPUMEMORY, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Memoria CPU"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_CPUMEMORY, XLANGUAGE_ISO_639_3_CODE_FRE, NL("M\u00e9moire CPU"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_CPUMEMORY, XLANGUAGE_ISO_639_3_CODE_GER, NL("CPU-Speicher"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_CPUMEMORY, XLANGUAGE_ISO_639_3_CODE_CHI, NL("CPU\u5185\u5b58"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_AVERANGE, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Averange"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_AVERANGE, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Media"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_AVERANGE, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Moyenne"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_AVERANGE, XLANGUAGE_ISO_639_3_CODE_GER, NL("Mittelwert"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_AVERANGE, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u5e73\u5747\u503c"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_CURRENTDATE, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Current date"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_CURRENTDATE, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Fecha actual"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_CURRENTDATE, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Date actuelle"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_CURRENTDATE, XLANGUAGE_ISO_639_3_CODE_GER, NL("Aktuelles Datum"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_CURRENTDATE, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u5f53\u524d\u65e5\u671f"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_OPERATINGTIME, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Operating time"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_OPERATINGTIME, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Tiempo de funcionamiento"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_OPERATINGTIME, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Temps de fonctionnement"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_OPERATINGTIME, XLANGUAGE_ISO_639_3_CODE_GER, NL("Betriebszeit"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_OPERATINGTIME, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u8fd0\u884c\u65f6\u95f4"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_MEMORYFREE, XLANGUAGE_ISO_639_3_CODE_ENG, NL("%.1f %s, free %.1f %s (%d%%)"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_MEMORYFREE, XLANGUAGE_ISO_639_3_CODE_SPA, NL("%.1f %s, libre %.1f %s (%d%%)"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_MEMORYFREE, XLANGUAGE_ISO_639_3_CODE_FRE, NL("%.1f %s, libre %.1f %s (%d%%)"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_MEMORYFREE, XLANGUAGE_ISO_639_3_CODE_GER, NL("%.1f %s, frei %.1f %s (%d%%)"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_MEMORYFREE, XLANGUAGE_ISO_639_3_CODE_CHI, NL("%.1f %s\uff0c\u7a7a\u95f2 %.1f %s (%d%%)"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_CPUTOTALAVG, XLANGUAGE_ISO_639_3_CODE_ENG, NL("avg. %d%% (max. %d%%)"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_CPUTOTALAVG, XLANGUAGE_ISO_639_3_CODE_SPA, NL("media %d%% (max. %d%%)"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_CPUTOTALAVG, XLANGUAGE_ISO_639_3_CODE_FRE, NL("moy. %d%% (max. %d%%)"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_CPUTOTALAVG, XLANGUAGE_ISO_639_3_CODE_GER, NL("Mittel %d%% (max. %d%%)"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_CPUTOTALAVG, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u5e73\u5747 %d%% (\u6700\u5927 %d%%)"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_CPUAPPAVG, XLANGUAGE_ISO_639_3_CODE_ENG, NL("app %s avg. %d%% (max. %d%%)"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_CPUAPPAVG, XLANGUAGE_ISO_639_3_CODE_SPA, NL("app %s media %d%% (max. %d%%)"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_CPUAPPAVG, XLANGUAGE_ISO_639_3_CODE_FRE, NL("app %s moy. %d%% (max. %d%%)"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_CPUAPPAVG, XLANGUAGE_ISO_639_3_CODE_GER, NL("App %s Mittel %d%% (max. %d%%)"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_CPUAPPAVG, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u5e94\u7528 %s \u5e73\u5747 %d%% (\u6700\u5927 %d%%)"), 0);

  //--- INTERNET STATUS SHOW -----------------------------------------------------------------------------------------

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INTERNETSTATUS_LOCALIP, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Local IP"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INTERNETSTATUS_LOCALIP, XLANGUAGE_ISO_639_3_CODE_SPA, NL("IP local"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INTERNETSTATUS_LOCALIP, XLANGUAGE_ISO_639_3_CODE_FRE, NL("IP locale"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INTERNETSTATUS_LOCALIP, XLANGUAGE_ISO_639_3_CODE_GER, NL("Lokale IP"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INTERNETSTATUS_LOCALIP, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u672c\u5730 IP"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INTERNETSTATUS_CONNECTION, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Internet Connection"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INTERNETSTATUS_CONNECTION, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Conexion a Internet"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INTERNETSTATUS_CONNECTION, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Connexion Internet"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INTERNETSTATUS_CONNECTION, XLANGUAGE_ISO_639_3_CODE_GER, NL("Internetverbindung"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INTERNETSTATUS_CONNECTION, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u4e92\u8054\u7f51\u8fde\u63a5"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INTERNETSTATUS_LATENCY, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Latency"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INTERNETSTATUS_LATENCY, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Latencia"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INTERNETSTATUS_LATENCY, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Latence"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INTERNETSTATUS_LATENCY, XLANGUAGE_ISO_639_3_CODE_GER, NL("Latenz"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INTERNETSTATUS_LATENCY, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u5ef6\u8fdf"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INTERNETSTATUS_PUBLICIP, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Public IP"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INTERNETSTATUS_PUBLICIP, XLANGUAGE_ISO_639_3_CODE_SPA, NL("IP publica"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INTERNETSTATUS_PUBLICIP, XLANGUAGE_ISO_639_3_CODE_FRE, NL("IP publique"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INTERNETSTATUS_PUBLICIP, XLANGUAGE_ISO_639_3_CODE_GER, NL("\u00d6ffentliche IP"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INTERNETSTATUS_PUBLICIP, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u516c\u7f51 IP"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INTERNETSTATUS_LATENCYMS, XLANGUAGE_ISO_639_3_CODE_ENG, NL("%d ms"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INTERNETSTATUS_LATENCYMS, XLANGUAGE_ISO_639_3_CODE_SPA, NL("%d ms"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INTERNETSTATUS_LATENCYMS, XLANGUAGE_ISO_639_3_CODE_FRE, NL("%d ms"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INTERNETSTATUS_LATENCYMS, XLANGUAGE_ISO_639_3_CODE_GER, NL("%d ms"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INTERNETSTATUS_LATENCYMS, XLANGUAGE_ISO_639_3_CODE_CHI, NL("%d ms"), 0);

  //--- APPLICATION UPDATE -------------------------------------------------------------------------------------------

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_VERSIONAVAILABLE, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Update Version available %d.%d.%d"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_VERSIONAVAILABLE, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Version de actualizacion disponible %d.%d.%d"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_VERSIONAVAILABLE, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Version de mise \u00e0 jour disponible %d.%d.%d"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_VERSIONAVAILABLE, XLANGUAGE_ISO_639_3_CODE_GER, NL("Update-Version verf\u00fcgbar %d.%d.%d"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_VERSIONAVAILABLE, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u53ef\u7528\u66f4\u65b0\u7248\u672c %d.%d.%d"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_NOVERSIONAVAILABLE, XLANGUAGE_ISO_639_3_CODE_ENG, NL("No version available to update"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_NOVERSIONAVAILABLE, XLANGUAGE_ISO_639_3_CODE_SPA, NL("No hay version disponible para actualizar"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_NOVERSIONAVAILABLE, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Aucune version disponible pour la mise \u00e0 jour"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_NOVERSIONAVAILABLE, XLANGUAGE_ISO_639_3_CODE_GER, NL("Keine Version zum Aktualisieren verf\u00fcgbar"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_NOVERSIONAVAILABLE, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u6ca1\u6709\u53ef\u7528\u4e8e\u66f4\u65b0\u7684\u7248\u672c"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_VERSIONIS, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Version %d.%d.%d is "), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_VERSIONIS, XLANGUAGE_ISO_639_3_CODE_SPA, NL("La version %d.%d.%d es "), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_VERSIONIS, XLANGUAGE_ISO_639_3_CODE_FRE, NL("La version %d.%d.%d est "), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_VERSIONIS, XLANGUAGE_ISO_639_3_CODE_GER, NL("Version %d.%d.%d ist "), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_VERSIONIS, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u7248\u672c %d.%d.%d "), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_VERSION_OLD, XLANGUAGE_ISO_639_3_CODE_ENG, NL("old"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_VERSION_OLD, XLANGUAGE_ISO_639_3_CODE_SPA, NL("antigua"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_VERSION_OLD, XLANGUAGE_ISO_639_3_CODE_FRE, NL("ancienne"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_VERSION_OLD, XLANGUAGE_ISO_639_3_CODE_GER, NL("alt"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_VERSION_OLD, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u8f83\u65e7"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_VERSION_EQUAL, XLANGUAGE_ISO_639_3_CODE_ENG, NL("equal"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_VERSION_EQUAL, XLANGUAGE_ISO_639_3_CODE_SPA, NL("igual"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_VERSION_EQUAL, XLANGUAGE_ISO_639_3_CODE_FRE, NL("\u00e9gale"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_VERSION_EQUAL, XLANGUAGE_ISO_639_3_CODE_GER, NL("gleich"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_VERSION_EQUAL, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u76f8\u540c"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_VERSION_NEW, XLANGUAGE_ISO_639_3_CODE_ENG, NL("new"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_VERSION_NEW, XLANGUAGE_ISO_639_3_CODE_SPA, NL("nueva"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_VERSION_NEW, XLANGUAGE_ISO_639_3_CODE_FRE, NL("nouvelle"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_VERSION_NEW, XLANGUAGE_ISO_639_3_CODE_GER, NL("neu"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_VERSION_NEW, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u8f83\u65b0"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_WILLNOTBEUPDATED, XLANGUAGE_ISO_639_3_CODE_ENG, NL(". It will not be updated"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_WILLNOTBEUPDATED, XLANGUAGE_ISO_639_3_CODE_SPA, NL(". No se actualizara"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_WILLNOTBEUPDATED, XLANGUAGE_ISO_639_3_CODE_FRE, NL(". Elle ne sera pas mise \u00e0 jour"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_WILLNOTBEUPDATED, XLANGUAGE_ISO_639_3_CODE_GER, NL(". Sie wird nicht aktualisiert"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_WILLNOTBEUPDATED, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u3002\u4e0d\u4f1a\u66f4\u65b0"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_DOWNLOADFILE, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Download file %s (%dk)."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_DOWNLOADFILE, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Descarga fichero %s (%dk)."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_DOWNLOADFILE, XLANGUAGE_ISO_639_3_CODE_FRE, NL("T\u00e9l\u00e9chargement du fichier %s (%dk)."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_DOWNLOADFILE, XLANGUAGE_ISO_639_3_CODE_GER, NL("Datei herunterladen %s (%dk)."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_DOWNLOADFILE, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u4e0b\u8f7d\u6587\u4ef6 %s (%dk)\u3002"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_UNZIPFILE, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Unzip file %s (%dk) -> %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_UNZIPFILE, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Descomprimir fichero %s (%dk) -> %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_UNZIPFILE, XLANGUAGE_ISO_639_3_CODE_FRE, NL("D\u00e9compression du fichier %s (%dk) -> %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_UNZIPFILE, XLANGUAGE_ISO_639_3_CODE_GER, NL("Datei entpacken %s (%dk) -> %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_UNZIPFILE, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u89e3\u538b\u6587\u4ef6 %s (%dk) -> %s\u3002"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_DOWNLOADSFILE, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Downloads file %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_DOWNLOADSFILE, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Descargas de fichero %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_DOWNLOADSFILE, XLANGUAGE_ISO_639_3_CODE_FRE, NL("T\u00e9l\u00e9chargements du fichier %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_DOWNLOADSFILE, XLANGUAGE_ISO_639_3_CODE_GER, NL("Datei-Downloads %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_DOWNLOADSFILE, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u6587\u4ef6\u4e0b\u8f7d %s\u3002"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_BACKUPORIGINALFILE, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Backup original file %s (%dk)."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_BACKUPORIGINALFILE, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Copia de seguridad del fichero original %s (%dk)."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_BACKUPORIGINALFILE, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Sauvegarde du fichier original %s (%dk)."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_BACKUPORIGINALFILE, XLANGUAGE_ISO_639_3_CODE_GER, NL("Sicherung der Originaldatei %s (%dk)."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_BACKUPORIGINALFILE, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u5907\u4efd\u539f\u59cb\u6587\u4ef6 %s (%dk)\u3002"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_BACKUPORIGINALFILES, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Backup Original file %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_BACKUPORIGINALFILES, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Copia de seguridad de ficheros originales %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_BACKUPORIGINALFILES, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Sauvegarde des fichiers originaux %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_BACKUPORIGINALFILES, XLANGUAGE_ISO_639_3_CODE_GER, NL("Sicherung der Originaldateien %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_BACKUPORIGINALFILES, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u5907\u4efd\u539f\u59cb\u6587\u4ef6 %s\u3002"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_APP_RELEASERESOURCES, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Application release blocked resources"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_APP_RELEASERESOURCES, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Aplicacion libera recursos bloqueados"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_APP_RELEASERESOURCES, XLANGUAGE_ISO_639_3_CODE_FRE, NL("L'application lib\u00e8re les ressources bloqu\u00e9es"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_APP_RELEASERESOURCES, XLANGUAGE_ISO_639_3_CODE_GER, NL("Anwendung gibt blockierte Ressourcen frei"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_APP_RELEASERESOURCES, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u5e94\u7528\u91ca\u653e\u88ab\u9501\u5b9a\u7684\u8d44\u6e90"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_UPDATEFILE, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Copy update file %s (%dk)."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_UPDATEFILE, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Copiar fichero de actualizacion %s (%dk)."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_UPDATEFILE, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Copier le fichier de mise \u00e0 jour %s (%dk)."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_UPDATEFILE, XLANGUAGE_ISO_639_3_CODE_GER, NL("Update-Datei kopieren %s (%dk)."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_UPDATEFILE, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u590d\u5236\u66f4\u65b0\u6587\u4ef6 %s (%dk)\u3002"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_UPDATEEXECFILE, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Notify update EXEC file %s (%dk)."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_UPDATEEXECFILE, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Notificar actualizacion de EXEC %s (%dk)."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_UPDATEEXECFILE, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Notifier la mise \u00e0 jour du fichier EXEC %s (%dk)."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_UPDATEEXECFILE, XLANGUAGE_ISO_639_3_CODE_GER, NL("Update der EXEC-Datei melden %s (%dk)."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_UPDATEEXECFILE, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u901a\u77e5\u66f4\u65b0 EXEC \u6587\u4ef6 %s (%dk)\u3002"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_UPDATEFILES, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Update files %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_UPDATEFILES, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Actualizar ficheros %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_UPDATEFILES, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Mettre \u00e0 jour les fichiers %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_UPDATEFILES, XLANGUAGE_ISO_639_3_CODE_GER, NL("Dateien aktualisieren %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_UPDATEFILES, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u66f4\u65b0\u6587\u4ef6 %s\u3002"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_APP_END, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Terminate application"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_APP_END, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Terminar aplicacion"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_APP_END, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Terminer l'application"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_APP_END, XLANGUAGE_ISO_639_3_CODE_GER, NL("Anwendung beenden"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_APP_END, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u7ed3\u675f\u5e94\u7528"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_RESTOREUPDATEFILE, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Restore file %s from backup (%dk)."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_RESTOREUPDATEFILE, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Restaurar fichero %s desde copia (%dk)."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_RESTOREUPDATEFILE, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Restaurer le fichier %s depuis la sauvegarde (%dk)."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_RESTOREUPDATEFILE, XLANGUAGE_ISO_639_3_CODE_GER, NL("Datei %s aus Sicherung wiederherstellen (%dk)."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_RESTOREUPDATEFILE, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u4ece\u5907\u4efd\u6062\u590d\u6587\u4ef6 %s (%dk)\u3002"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_RESTOREUPDATEFILES, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Restore update files %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_RESTOREUPDATEFILES, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Restaurar ficheros de actualizacion %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_RESTOREUPDATEFILES, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Restaurer les fichiers de mise \u00e0 jour %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_RESTOREUPDATEFILES, XLANGUAGE_ISO_639_3_CODE_GER, NL("Update-Dateien wiederherstellen %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_RESTOREUPDATEFILES, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u6062\u590d\u66f4\u65b0\u6587\u4ef6 %s\u3002"), 0);

  //--- APPLICATION WEB SERVER ---------------------------------------------------------------------------------------

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_TLSCREDENTIALSNOTLOADED, XLANGUAGE_ISO_639_3_CODE_ENG, NL("WEB server: TLS credentials could not be loaded from the configured provider or credential paths."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_TLSCREDENTIALSNOTLOADED, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Servidor WEB: no se pudieron cargar las credenciales TLS del proveedor o rutas configuradas."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_TLSCREDENTIALSNOTLOADED, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Serveur WEB : les identifiants TLS n'ont pas pu \u00eatre charg\u00e9s depuis le fournisseur ou les chemins configur\u00e9s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_TLSCREDENTIALSNOTLOADED, XLANGUAGE_ISO_639_3_CODE_GER, NL("WEB-Server: TLS-Anmeldeinformationen konnten nicht vom konfigurierten Anbieter oder den Pfaden geladen werden."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_TLSCREDENTIALSNOTLOADED, XLANGUAGE_ISO_639_3_CODE_CHI, NL("WEB \u670d\u52a1\u5668\uff1a\u65e0\u6cd5\u4ece\u914d\u7f6e\u7684\u63d0\u4f9b\u8005\u6216\u8def\u5f84\u52a0\u8f7d TLS \u51ed\u8bc1\u3002"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_TLSCREDENTIALSNOTLOADED_PORT, XLANGUAGE_ISO_639_3_CODE_ENG, NL("WEB server (port %d): TLS credentials could not be loaded from the configured provider or credential paths."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_TLSCREDENTIALSNOTLOADED_PORT, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Servidor WEB (puerto %d): no se pudieron cargar las credenciales TLS del proveedor o rutas configuradas."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_TLSCREDENTIALSNOTLOADED_PORT, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Serveur WEB (port %d) : les identifiants TLS n'ont pas pu \u00eatre charg\u00e9s depuis le fournisseur ou les chemins configur\u00e9s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_TLSCREDENTIALSNOTLOADED_PORT, XLANGUAGE_ISO_639_3_CODE_GER, NL("WEB-Server (Port %d): TLS-Anmeldeinformationen konnten nicht vom konfigurierten Anbieter oder den Pfaden geladen werden."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_TLSCREDENTIALSNOTLOADED_PORT, XLANGUAGE_ISO_639_3_CODE_CHI, NL("WEB \u670d\u52a1\u5668\uff08\u7aef\u53e3 %d\uff09\uff1a\u65e0\u6cd5\u4ece\u914d\u7f6e\u7684\u63d0\u4f9b\u8005\u6216\u8def\u5f84\u52a0\u8f7d TLS \u51ed\u8bc1\u3002"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_AUTHNOPASSWORD, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Authenticated access is active but there is no password in the configuration, it will be resolved by events."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_AUTHNOPASSWORD, XLANGUAGE_ISO_639_3_CODE_SPA, NL("El acceso autenticado esta activo pero no hay contrasena en la configuracion, se resolvera por eventos."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_AUTHNOPASSWORD, XLANGUAGE_ISO_639_3_CODE_FRE, NL("L'acc\u00e8s authentifi\u00e9 est actif mais il n'y a pas de mot de passe dans la configuration, il sera r\u00e9solu par \u00e9v\u00e9nements."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_AUTHNOPASSWORD, XLANGUAGE_ISO_639_3_CODE_GER, NL("Authentifizierter Zugriff ist aktiv, aber es gibt kein Passwort in der Konfiguration; es wird per Ereignisse gel\u00f6st."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_AUTHNOPASSWORD, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u5df2\u542f\u7528\u8ba4\u8bc1\u8bbf\u95ee\uff0c\u4f46\u914d\u7f6e\u4e2d\u6ca1\u6709\u5bc6\u7801\uff0c\u5c06\u901a\u8fc7\u4e8b\u4ef6\u89e3\u51b3\u3002"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_AUTHCONFIGURED, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Authenticated access configured for the web server (user [%s])."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_AUTHCONFIGURED, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Acceso autenticado configurado para el servidor web (usuario [%s])."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_AUTHCONFIGURED, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Acc\u00e8s authentifi\u00e9 configur\u00e9 pour le serveur web (utilisateur [%s])."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_AUTHCONFIGURED, XLANGUAGE_ISO_639_3_CODE_GER, NL("Authentifizierter Zugriff f\u00fcr den Webserver konfiguriert (Benutzer [%s])."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_AUTHCONFIGURED, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u5df2\u4e3a Web \u670d\u52a1\u5668\u914d\u7f6e\u8ba4\u8bc1\u8bbf\u95ee\uff08\u7528\u6237 [%s]\uff09\u3002"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_AUTHREQUEST, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Request from [%s] to the web server, authentication of the user [%s]: %s"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_AUTHREQUEST, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Peticion desde [%s] al servidor web, autenticacion del usuario [%s]: %s"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_AUTHREQUEST, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Requ\u00eate depuis [%s] vers le serveur web, authentification de l'utilisateur [%s] : %s"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_AUTHREQUEST, XLANGUAGE_ISO_639_3_CODE_GER, NL("Anfrage von [%s] an den Webserver, Authentifizierung des Benutzers [%s]: %s"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_AUTHREQUEST, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u6765\u81ea [%s] \u5bf9 Web \u670d\u52a1\u5668\u7684\u8bf7\u6c42\uff0c\u7528\u6237 [%s] \u8ba4\u8bc1\uff1a%s"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_AUTH_INVALID, XLANGUAGE_ISO_639_3_CODE_ENG, NL("INVALID!"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_AUTH_INVALID, XLANGUAGE_ISO_639_3_CODE_SPA, NL("INVALIDO!"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_AUTH_INVALID, XLANGUAGE_ISO_639_3_CODE_FRE, NL("INVALIDE !"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_AUTH_INVALID, XLANGUAGE_ISO_639_3_CODE_GER, NL("UNG\u00dcLTIG!"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_AUTH_INVALID, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u65e0\u6548\uff01"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_INVALIDUSERORPASSWORD, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Invalid user or password!"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_INVALIDUSERORPASSWORD, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Usuario o contrasena invalidos!"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_INVALIDUSERORPASSWORD, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Utilisateur ou mot de passe invalide !"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_INVALIDUSERORPASSWORD, XLANGUAGE_ISO_639_3_CODE_GER, NL("Ung\u00fcltiger Benutzer oder ung\u00fcltiges Passwort!"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_INVALIDUSERORPASSWORD, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u7528\u6237\u6216\u5bc6\u7801\u65e0\u6548\uff01"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_WEBBROWSER, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Web browser    : %s, with: %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_WEBBROWSER, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Navegador web  : %s, con: %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_WEBBROWSER, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Navigateur web  : %s, avec : %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_WEBBROWSER, XLANGUAGE_ISO_639_3_CODE_GER, NL("Webbrowser      : %s, mit: %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_WEBBROWSER, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u6d4f\u89c8\u5668        \uff1a%s\uff0c\u4f7f\u7528\uff1a%s\u3002"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_ERRORTOEXECUTE, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Error to execute the %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_ERRORTOEXECUTE, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Error al ejecutar %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_ERRORTOEXECUTE, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Erreur lors de l'ex\u00e9cution de %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_ERRORTOEXECUTE, XLANGUAGE_ISO_639_3_CODE_GER, NL("Fehler beim Ausf\u00fchren von %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_ERRORTOEXECUTE, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u6267\u884c %s \u65f6\u51fa\u9519\u3002"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_REQUESTRESULT, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Request from [%s] to the web server \"%s\" %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_REQUESTRESULT, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Peticion desde [%s] al servidor web \"%s\" %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_REQUESTRESULT, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Requ\u00eate depuis [%s] vers le serveur web \"%s\" %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_REQUESTRESULT, XLANGUAGE_ISO_639_3_CODE_GER, NL("Anfrage von [%s] an den Webserver \"%s\" %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_REQUESTRESULT, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u6765\u81ea [%s] \u5bf9 Web \u670d\u52a1\u5668 \"%s\" \u7684\u8bf7\u6c42%s\u3002"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_SENT, XLANGUAGE_ISO_639_3_CODE_ENG, NL("sent"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_SENT, XLANGUAGE_ISO_639_3_CODE_SPA, NL("enviada"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_SENT, XLANGUAGE_ISO_639_3_CODE_FRE, NL("envoy\u00e9e"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_SENT, XLANGUAGE_ISO_639_3_CODE_GER, NL("gesendet"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_SENT, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u5df2\u53d1\u9001"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_NOTSEND, XLANGUAGE_ISO_639_3_CODE_ENG, NL("not send"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_NOTSEND, XLANGUAGE_ISO_639_3_CODE_SPA, NL("no enviada"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_NOTSEND, XLANGUAGE_ISO_639_3_CODE_FRE, NL("non envoy\u00e9e"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_NOTSEND, XLANGUAGE_ISO_639_3_CODE_GER, NL("nicht gesendet"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_NOTSEND, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u672a\u53d1\u9001"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_ERRORCOMMANDORPARAMS, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Error: command or erroneous parameters."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_ERRORCOMMANDORPARAMS, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Error: comando o parametros erroneos."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_ERRORCOMMANDORPARAMS, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Erreur : commande ou param\u00e8tres erron\u00e9s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_ERRORCOMMANDORPARAMS, XLANGUAGE_ISO_639_3_CODE_GER, NL("Fehler: Befehl oder fehlerhafte Parameter."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_ERRORCOMMANDORPARAMS, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u9519\u8bef\uff1a\u547d\u4ee4\u6216\u53c2\u6570\u6709\u8bef\u3002"), 0);

  //--- TRANSLATESCAN ------------------------------------------------------------------------------------------

  Sentence_Add(XTRANSLATION_GEN_ID_DIOSSHREVERSE_ACTIVATE_SERVICE_RETURN_CODE, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Activate service return code [%d]"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOSSHREVERSE_ACTIVATE_SERVICE_RETURN_CODE, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Activar c\u00F3digo de retorno del servicio [%d]"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOSSHREVERSE_ACTIVATE_SERVICE_RETURN_CODE, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Activer le code de retour du service [%d]"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOSSHREVERSE_ACTIVATE_SERVICE_RETURN_CODE, XLANGUAGE_ISO_639_3_CODE_GER, NL("Service-R\u00FCckgabecode aktivieren [%d]"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOSSHREVERSE_ACTIVATE_SERVICE_RETURN_CODE, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u6FC0\u6D3B\u670D\u52A1\u8FD4\u56DE\u4EE3\u7801 [%d]"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_DIOSSHREVERSE_DEACTIVATE_SERVICE_RETURN_CODE, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Deactivate service return code [%d]"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOSSHREVERSE_DEACTIVATE_SERVICE_RETURN_CODE, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Desactivar c\u00F3digo de retorno del servicio [%d]"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOSSHREVERSE_DEACTIVATE_SERVICE_RETURN_CODE, XLANGUAGE_ISO_639_3_CODE_FRE, NL("D\u00E9sactiver le code de retour du service [%d]"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOSSHREVERSE_DEACTIVATE_SERVICE_RETURN_CODE, XLANGUAGE_ISO_639_3_CODE_GER, NL("Dienstr\u00FCckgabecode deaktivieren [%d]"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOSSHREVERSE_DEACTIVATE_SERVICE_RETURN_CODE, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u505C\u7528\u670D\u52A1\u8FD4\u56DE\u4EE3\u7801 [%d]"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_DIOAIOBIMAINBOARD_AIOBI_MAIN_BOARD_ERROR_I2C_GPIO_IN_NOT_INIT, XLANGUAGE_ISO_639_3_CODE_ENG, NL("AIOBI Main Board: ERROR! I2C GPIO IN not init!"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOAIOBIMAINBOARD_AIOBI_MAIN_BOARD_ERROR_I2C_GPIO_IN_NOT_INIT, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Tablero principal de AIOBI: \u00A1ERROR! "), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOAIOBIMAINBOARD_AIOBI_MAIN_BOARD_ERROR_I2C_GPIO_IN_NOT_INIT, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Carte principale AIOBI\u00A0: ERREUR\u00A0! "), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOAIOBIMAINBOARD_AIOBI_MAIN_BOARD_ERROR_I2C_GPIO_IN_NOT_INIT, XLANGUAGE_ISO_639_3_CODE_GER, NL("AIOBI-Hauptplatine: FEHLER! "), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOAIOBIMAINBOARD_AIOBI_MAIN_BOARD_ERROR_I2C_GPIO_IN_NOT_INIT, XLANGUAGE_ISO_639_3_CODE_CHI, NL("AIOBI\u4E3B\u677F\uFF1A\u9519\u8BEF\uFF01 "), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_DIOAIOBIMAINBOARD_AIOBI_MAIN_BOARD_ERROR_I2C_GPIO_OUT_NOT_INIT, XLANGUAGE_ISO_639_3_CODE_ENG, NL("AIOBI Main Board: ERROR! I2C GPIO OUT not init!"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOAIOBIMAINBOARD_AIOBI_MAIN_BOARD_ERROR_I2C_GPIO_OUT_NOT_INIT, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Tablero principal de AIOBI: \u00A1ERROR! "), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOAIOBIMAINBOARD_AIOBI_MAIN_BOARD_ERROR_I2C_GPIO_OUT_NOT_INIT, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Carte principale AIOBI\u00A0: ERREUR\u00A0! "), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOAIOBIMAINBOARD_AIOBI_MAIN_BOARD_ERROR_I2C_GPIO_OUT_NOT_INIT, XLANGUAGE_ISO_639_3_CODE_GER, NL("AIOBI-Hauptplatine: FEHLER! "), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOAIOBIMAINBOARD_AIOBI_MAIN_BOARD_ERROR_I2C_GPIO_OUT_NOT_INIT, XLANGUAGE_ISO_639_3_CODE_CHI, NL("AIOBI\u4E3B\u677F\uFF1A\u9519\u8BEF\uFF01 "), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_DIOAIOBIMAINBOARD_AIOBI_MAIN_BOARD_ERROR_I2C_GPIO_RELAYS_NOT_INIT, XLANGUAGE_ISO_639_3_CODE_ENG, NL("AIOBI Main Board: ERROR! I2C GPIO Relays not init!"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOAIOBIMAINBOARD_AIOBI_MAIN_BOARD_ERROR_I2C_GPIO_RELAYS_NOT_INIT, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Tablero principal de AIOBI: \u00A1ERROR! "), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOAIOBIMAINBOARD_AIOBI_MAIN_BOARD_ERROR_I2C_GPIO_RELAYS_NOT_INIT, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Carte principale AIOBI\u00A0: ERREUR\u00A0! "), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOAIOBIMAINBOARD_AIOBI_MAIN_BOARD_ERROR_I2C_GPIO_RELAYS_NOT_INIT, XLANGUAGE_ISO_639_3_CODE_GER, NL("AIOBI-Hauptplatine: FEHLER! "), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOAIOBIMAINBOARD_AIOBI_MAIN_BOARD_ERROR_I2C_GPIO_RELAYS_NOT_INIT, XLANGUAGE_ISO_639_3_CODE_CHI, NL("AIOBI\u4E3B\u677F\uFF1A\u9519\u8BEF\uFF01 "), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_DIOAIOBIMAINBOARD_AIOBI_MAIN_BOARD_ERROR_I2C_SERIAL_EEPROM_NOT_INI, XLANGUAGE_ISO_639_3_CODE_ENG, NL("AIOBI Main Board: ERROR! I2C Serial EEPROM not init!"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOAIOBIMAINBOARD_AIOBI_MAIN_BOARD_ERROR_I2C_SERIAL_EEPROM_NOT_INI, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Tablero principal de AIOBI: \u00A1ERROR! "), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOAIOBIMAINBOARD_AIOBI_MAIN_BOARD_ERROR_I2C_SERIAL_EEPROM_NOT_INI, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Carte principale AIOBI\u00A0: ERREUR\u00A0! "), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOAIOBIMAINBOARD_AIOBI_MAIN_BOARD_ERROR_I2C_SERIAL_EEPROM_NOT_INI, XLANGUAGE_ISO_639_3_CODE_GER, NL("AIOBI-Hauptplatine: FEHLER! "), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOAIOBIMAINBOARD_AIOBI_MAIN_BOARD_ERROR_I2C_SERIAL_EEPROM_NOT_INI, XLANGUAGE_ISO_639_3_CODE_CHI, NL("AIOBI\u4E3B\u677F\uFF1A\u9519\u8BEF\uFF01 "), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_DIOAIOBIMAINBOARD_AIOBI_MAIN_BOARD_ERROR_I2C_PCA9685_PWM_NOT_INIT, XLANGUAGE_ISO_639_3_CODE_ENG, NL("AIOBI Main Board: ERROR! I2C PCA9685 (PWM) not init!"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOAIOBIMAINBOARD_AIOBI_MAIN_BOARD_ERROR_I2C_PCA9685_PWM_NOT_INIT, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Tablero principal de AIOBI: \u00A1ERROR! "), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOAIOBIMAINBOARD_AIOBI_MAIN_BOARD_ERROR_I2C_PCA9685_PWM_NOT_INIT, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Carte principale AIOBI\u00A0: ERREUR\u00A0! "), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOAIOBIMAINBOARD_AIOBI_MAIN_BOARD_ERROR_I2C_PCA9685_PWM_NOT_INIT, XLANGUAGE_ISO_639_3_CODE_GER, NL("AIOBI-Hauptplatine: FEHLER! "), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOAIOBIMAINBOARD_AIOBI_MAIN_BOARD_ERROR_I2C_PCA9685_PWM_NOT_INIT, XLANGUAGE_ISO_639_3_CODE_CHI, NL("AIOBI\u4E3B\u677F\uFF1A\u9519\u8BEF\uFF01 "), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_DIOAIOBIMAINBOARD_AIOBI_MAIN_BOARD_ERROR_GPIO_NOT_CREATED, XLANGUAGE_ISO_639_3_CODE_ENG, NL("AIOBI Main Board: ERROR! GPIO not created!"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOAIOBIMAINBOARD_AIOBI_MAIN_BOARD_ERROR_GPIO_NOT_CREATED, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Tablero principal de AIOBI: \u00A1ERROR! "), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOAIOBIMAINBOARD_AIOBI_MAIN_BOARD_ERROR_GPIO_NOT_CREATED, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Carte principale AIOBI\u00A0: ERREUR\u00A0! "), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOAIOBIMAINBOARD_AIOBI_MAIN_BOARD_ERROR_GPIO_NOT_CREATED, XLANGUAGE_ISO_639_3_CODE_GER, NL("AIOBI-Hauptplatine: FEHLER! "), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOAIOBIMAINBOARD_AIOBI_MAIN_BOARD_ERROR_GPIO_NOT_CREATED, XLANGUAGE_ISO_639_3_CODE_CHI, NL("AIOBI\u4E3B\u677F\uFF1A\u9519\u8BEF\uFF01 "), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_DIOAIOBIMAINBOARD_AIOBI_MAIN_BOARD_ERROR_GPIO_NOT_INIT, XLANGUAGE_ISO_639_3_CODE_ENG, NL("AIOBI Main Board: ERROR! GPIO not init!"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOAIOBIMAINBOARD_AIOBI_MAIN_BOARD_ERROR_GPIO_NOT_INIT, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Tablero principal de AIOBI: \u00A1ERROR! "), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOAIOBIMAINBOARD_AIOBI_MAIN_BOARD_ERROR_GPIO_NOT_INIT, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Carte principale AIOBI\u00A0: ERREUR\u00A0! "), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOAIOBIMAINBOARD_AIOBI_MAIN_BOARD_ERROR_GPIO_NOT_INIT, XLANGUAGE_ISO_639_3_CODE_GER, NL("AIOBI-Hauptplatine: FEHLER! "), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOAIOBIMAINBOARD_AIOBI_MAIN_BOARD_ERROR_GPIO_NOT_INIT, XLANGUAGE_ISO_639_3_CODE_CHI, NL("AIOBI\u4E3B\u677F\uFF1A\u9519\u8BEF\uFF01 "), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_SERVER_PAGE_INI, XLANGUAGE_ISO_639_3_CODE_ENG, NL("[%08X] Server THIS Page [%d] %s"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_SERVER_PAGE_INI, XLANGUAGE_ISO_639_3_CODE_SPA, NL("[%08X] Servidor ESTA p\u00E1gina [%d] %s"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_SERVER_PAGE_INI, XLANGUAGE_ISO_639_3_CODE_FRE, NL("[%08X] Serveur CETTE Page [%d] %s"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_SERVER_PAGE_INI, XLANGUAGE_ISO_639_3_CODE_GER, NL("[%08X] Server DIESE Seite [%d] %s"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_SERVER_PAGE_INI, XLANGUAGE_ISO_639_3_CODE_CHI, NL("[%08X] \u670D\u52A1\u5668\u6B64\u9875 [%d] %s"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_SERVER_PAGE_END, XLANGUAGE_ISO_639_3_CODE_ENG, NL("[%08X] Server Page END [%d] %s"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_SERVER_PAGE_END, XLANGUAGE_ISO_639_3_CODE_SPA, NL("[%08X] FINAL de la p\u00E1gina del servidor [%d] %s"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_SERVER_PAGE_END, XLANGUAGE_ISO_639_3_CODE_FRE, NL("[%08X] FIN de la page du serveur [%d] %s"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_SERVER_PAGE_END, XLANGUAGE_ISO_639_3_CODE_GER, NL("[%08X] Serverseite ENDE [%d] %s"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_SERVER_PAGE_END, XLANGUAGE_ISO_639_3_CODE_CHI, NL("[%08X] \u670D\u52A1\u5668\u9875\u9762\u7ED3\u675F [%d] %s"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_WEBSOCKET, XLANGUAGE_ISO_639_3_CODE_ENG, NL("[%08X] Websocket %s %d %s %s"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_WEBSOCKET, XLANGUAGE_ISO_639_3_CODE_SPA, NL("[%08X]Socket web %s %d %s %s"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_WEBSOCKET, XLANGUAGE_ISO_639_3_CODE_FRE, NL("[%08X] Websocket %s %d %s %s"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_WEBSOCKET, XLANGUAGE_ISO_639_3_CODE_GER, NL("[%08X] Websocket %s %d %s %s"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_WEBSOCKET, XLANGUAGE_ISO_639_3_CODE_CHI, NL("[%08X] Websocket %s %d %s %s"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_CONNECTED, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Connected"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_CONNECTED, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Conectado"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_CONNECTED, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Connect\u00E9"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_CONNECTED, XLANGUAGE_ISO_639_3_CODE_GER, NL("Verbunden"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_CONNECTED, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u5DF2\u8FDE\u63A5"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_NO_CONNECTED, XLANGUAGE_ISO_639_3_CODE_ENG, NL("No connected"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_NO_CONNECTED, XLANGUAGE_ISO_639_3_CODE_SPA, NL("No conectado"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_NO_CONNECTED, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Non connect\u00E9"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_NO_CONNECTED, XLANGUAGE_ISO_639_3_CODE_GER, NL("Keine Verbindung"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_NO_CONNECTED, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u672A\u8FDE\u63A5"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_WEBSOCKET_READ_DATA, XLANGUAGE_ISO_639_3_CODE_ENG, NL("[%08X] Websocket read data %d %s %s"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_WEBSOCKET_READ_DATA, XLANGUAGE_ISO_639_3_CODE_SPA, NL("[%08X] Websocket ley\u00F3 datos %d %s %s"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_WEBSOCKET_READ_DATA, XLANGUAGE_ISO_639_3_CODE_FRE, NL("[%08X] Donn\u00E9es de lecture Websocket %d %s %s"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_WEBSOCKET_READ_DATA, XLANGUAGE_ISO_639_3_CODE_GER, NL("[%08X] Websocket-Lesedaten %d %s %s"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_WEBSOCKET_READ_DATA, XLANGUAGE_ISO_639_3_CODE_CHI, NL("[%08X] Websocket \u8BFB\u53D6\u6570\u636E %d %s %s"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_WEBSOCKET_PING_RECEIVED, XLANGUAGE_ISO_639_3_CODE_ENG, NL("[%08X] Websocket Ping Received %d %s %s"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_WEBSOCKET_PING_RECEIVED, XLANGUAGE_ISO_639_3_CODE_SPA, NL("[%08X] Ping de Websocket recibido %d %s %s"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_WEBSOCKET_PING_RECEIVED, XLANGUAGE_ISO_639_3_CODE_FRE, NL("[%08X] Ping Websocket re\u00E7u %d %s %s"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_WEBSOCKET_PING_RECEIVED, XLANGUAGE_ISO_639_3_CODE_GER, NL("[%08X] Websocket-Ping empfangen %d %s %s"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_WEBSOCKET_PING_RECEIVED, XLANGUAGE_ISO_639_3_CODE_CHI, NL("[%08X] \u5DF2\u6536\u5230 Websocket Ping %d %s %s"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_WEBSOCKET_DISCONNECTED, XLANGUAGE_ISO_639_3_CODE_ENG, NL("[%08X] Websocket disconnected %d %s %s"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_WEBSOCKET_DISCONNECTED, XLANGUAGE_ISO_639_3_CODE_SPA, NL("[%08X] Websocket desconectado %d %s %s"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_WEBSOCKET_DISCONNECTED, XLANGUAGE_ISO_639_3_CODE_FRE, NL("[%08X] Websocket d\u00E9connect\u00E9 %d %s %s"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_WEBSOCKET_DISCONNECTED, XLANGUAGE_ISO_639_3_CODE_GER, NL("[%08X] Websocket getrennt %d %s %s"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_WEBSOCKET_DISCONNECTED, XLANGUAGE_ISO_639_3_CODE_CHI, NL("[%08X] Websocket \u5DF2\u65AD\u5F00\u8FDE\u63A5 %d %s %s"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_CONNECTION_HANDSHAKE_FAILED_ERROR, XLANGUAGE_ISO_639_3_CODE_ENG, NL("[%08X] Connection handshake failed [%s] error=%d"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_CONNECTION_HANDSHAKE_FAILED_ERROR, XLANGUAGE_ISO_639_3_CODE_SPA, NL("[%08X] Error en el protocolo de conexi\u00F3n [%s] error\\u003d%d"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_CONNECTION_HANDSHAKE_FAILED_ERROR, XLANGUAGE_ISO_639_3_CODE_FRE, NL("[%08X] \u00C9chec de l'\u00E9tablissement de liaison de connexion [%s] erreur\\u003d%d"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_CONNECTION_HANDSHAKE_FAILED_ERROR, XLANGUAGE_ISO_639_3_CODE_GER, NL("[%08X] Verbindungs-Handshake fehlgeschlagen [%s] Fehler\\u003d%d"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_CONNECTION_HANDSHAKE_FAILED_ERROR, XLANGUAGE_ISO_639_3_CODE_CHI, NL("[%08X] \u8FDE\u63A5\u63E1\u624B\u5931\u8D25 [%s] \u9519\u8BEF\\u003d%d"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_CONNECTION_ACTIVE, XLANGUAGE_ISO_639_3_CODE_ENG, NL("[%08X] Connection Active [%s]"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_CONNECTION_ACTIVE, XLANGUAGE_ISO_639_3_CODE_SPA, NL("[%08X] Conexi\u00F3n activa [%s]"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_CONNECTION_ACTIVE, XLANGUAGE_ISO_639_3_CODE_FRE, NL("[%08X] Connexion active [%s]"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_CONNECTION_ACTIVE, XLANGUAGE_ISO_639_3_CODE_GER, NL("[%08X] Verbindung aktiv [%s]"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_CONNECTION_ACTIVE, XLANGUAGE_ISO_639_3_CODE_CHI, NL("[%08X] \u8FDE\u63A5\u6D3B\u52A8 [%s]"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_WEB_SERVER_IN, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Web Server [%08X]: %s in %s:%d "), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_WEB_SERVER_IN, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Servidor web [%08X]: %s en %s:%d"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_WEB_SERVER_IN, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Serveur Web [%08X]\u00A0: %s dans %s\u00A0:%d"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_WEB_SERVER_IN, XLANGUAGE_ISO_639_3_CODE_GER, NL("Webserver [%08X]: %s in %s:%d"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_WEB_SERVER_IN, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u7F51\u7EDC\u670D\u52A1\u5668 [%08X]\uFF1A%s \u4E2D\u7684 %s\uFF1A%d"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_ACTIVE, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Active"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_ACTIVE, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Activo"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_ACTIVE, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Actif"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_ACTIVE, XLANGUAGE_ISO_639_3_CODE_GER, NL("Aktiv"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_ACTIVE, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u79EF\u6781\u7684"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DISABLE, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Disable"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DISABLE, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Desactivar"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DISABLE, XLANGUAGE_ISO_639_3_CODE_FRE, NL("D\u00E9sactiver"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DISABLE, XLANGUAGE_ISO_639_3_CODE_GER, NL("Deaktivieren"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DISABLE, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u7981\u7528"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DEFAULT, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Default"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DEFAULT, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Por defecto"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DEFAULT, XLANGUAGE_ISO_639_3_CODE_FRE, NL("D\u00E9faut"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DEFAULT, XLANGUAGE_ISO_639_3_CODE_GER, NL("Standard"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DEFAULT, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u9ED8\u8BA4"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DISABLE_WEB_SERVER, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Disable web server [%08X]."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DISABLE_WEB_SERVER, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Deshabilitar el servidor web [%08X]."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DISABLE_WEB_SERVER, XLANGUAGE_ISO_639_3_CODE_FRE, NL("D\u00E9sactivez le serveur Web [%08X]."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DISABLE_WEB_SERVER, XLANGUAGE_ISO_639_3_CODE_GER, NL("Deaktivieren Sie den Webserver [%08X]."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DISABLE_WEB_SERVER, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u7981\u7528\u7F51\u7EDC\u670D\u52A1\u5668 [%08X]\u3002"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DELETE_MAIN_THREAD, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Delete main thread ... "), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DELETE_MAIN_THREAD, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Eliminar hilo principal..."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DELETE_MAIN_THREAD, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Supprimer le fil principal..."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DELETE_MAIN_THREAD, XLANGUAGE_ISO_639_3_CODE_GER, NL("Hauptthread l\u00F6schen ..."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DELETE_MAIN_THREAD, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u5220\u9664\u4E3B\u7EBF\u7A0B..."), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_WAIT_TO_SEND_ALL_PAGES, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Wait to send all pages ..."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_WAIT_TO_SEND_ALL_PAGES, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Espere a enviar todas las p\u00E1ginas..."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_WAIT_TO_SEND_ALL_PAGES, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Attendez d'envoyer toutes les pages..."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_WAIT_TO_SEND_ALL_PAGES, XLANGUAGE_ISO_639_3_CODE_GER, NL("Warten Sie, bis alle Seiten gesendet sind ..."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_WAIT_TO_SEND_ALL_PAGES, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u7B49\u5F85\u53D1\u9001\u6240\u6709\u9875\u9762..."), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DELETE_CONNECTIONS_NOT_USE, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Delete connections not use ..."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DELETE_CONNECTIONS_NOT_USE, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Eliminar conexiones no usar..."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DELETE_CONNECTIONS_NOT_USE, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Supprimer les connexions non utilis\u00E9es..."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DELETE_CONNECTIONS_NOT_USE, XLANGUAGE_ISO_639_3_CODE_GER, NL("Nicht verwendete Verbindungen l\u00F6schen ..."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DELETE_CONNECTIONS_NOT_USE, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u5220\u9664\u8FDE\u63A5\u4E0D\u4F7F\u7528..."), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DELETE_CONNECTIONS_WAITING, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Delete connections waiting ..."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DELETE_CONNECTIONS_WAITING, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Eliminar conexiones en espera..."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DELETE_CONNECTIONS_WAITING, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Supprimer les connexions en attente..."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DELETE_CONNECTIONS_WAITING, XLANGUAGE_ISO_639_3_CODE_GER, NL("Wartende Verbindungen l\u00F6schen ..."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DELETE_CONNECTIONS_WAITING, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u5220\u9664\u6B63\u5728\u7B49\u5F85\u7684\u8FDE\u63A5..."), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DELETE_ALL_CONNECTIONS, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Delete all connections ... "), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DELETE_ALL_CONNECTIONS, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Eliminar todas las conexiones..."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DELETE_ALL_CONNECTIONS, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Supprimez toutes les connexions..."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DELETE_ALL_CONNECTIONS, XLANGUAGE_ISO_639_3_CODE_GER, NL("Alle Verbindungen l\u00F6schen ..."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DELETE_ALL_CONNECTIONS, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u5220\u9664\u6240\u6709\u8FDE\u63A5..."), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DELETE_RESOURCES, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Delete resources ... "), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DELETE_RESOURCES, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Eliminar recursos..."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DELETE_RESOURCES, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Supprimer des ressources..."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DELETE_RESOURCES, XLANGUAGE_ISO_639_3_CODE_GER, NL("Ressourcen l\u00F6schen ..."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DELETE_RESOURCES, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u5220\u9664\u8D44\u6E90..."), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DELETE_EVENTS, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Delete events ... "), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DELETE_EVENTS, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Eliminar eventos..."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DELETE_EVENTS, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Supprimer des \u00E9v\u00E9nements..."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DELETE_EVENTS, XLANGUAGE_ISO_639_3_CODE_GER, NL("Ereignisse l\u00F6schen ..."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DELETE_EVENTS, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u5220\u9664\u4E8B\u4EF6..."), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DELETE_STREAM_CONFIG, XLANGUAGE_ISO_639_3_CODE_ENG, NL("Delete stream config ... "), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DELETE_STREAM_CONFIG, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Eliminar configuraci\u00F3n de transmisi\u00F3n..."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DELETE_STREAM_CONFIG, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Supprimer la configuration du flux..."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DELETE_STREAM_CONFIG, XLANGUAGE_ISO_639_3_CODE_GER, NL("Stream-Konfiguration l\u00F6schen ..."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_DELETE_STREAM_CONFIG, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u5220\u9664\u6D41\u914D\u7F6E..."), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_END_WEB_SERVER, XLANGUAGE_ISO_639_3_CODE_ENG, NL("End web server [%08X]."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_END_WEB_SERVER, XLANGUAGE_ISO_639_3_CODE_SPA, NL("Finalizar servidor web [%08X]."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_END_WEB_SERVER, XLANGUAGE_ISO_639_3_CODE_FRE, NL("Terminer le serveur Web [%08X]."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_END_WEB_SERVER, XLANGUAGE_ISO_639_3_CODE_GER, NL("Beenden Sie den Webserver [%08X]."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_END_WEB_SERVER, XLANGUAGE_ISO_639_3_CODE_CHI, NL("\u7ED3\u675F\u7F51\u7EDC\u670D\u52A1\u5668 [%08X]\u3002"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_CONNECTION_DEACTIVE, XLANGUAGE_ISO_639_3_CODE_ENG, NL("[%08X] Connection Deactive [%s]"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_CONNECTION_DEACTIVE, XLANGUAGE_ISO_639_3_CODE_SPA, NL("[%08X] Conexi\u00F3n desactivada [%s]"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_CONNECTION_DEACTIVE, XLANGUAGE_ISO_639_3_CODE_FRE, NL("[%08X] Connexion d\u00E9sactiv\u00E9e [%s]"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_CONNECTION_DEACTIVE, XLANGUAGE_ISO_639_3_CODE_GER, NL("[%08X] Verbindung deaktiviert [%s]"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_DIOWEBSERVER_CONNECTION_DEACTIVE, XLANGUAGE_ISO_639_3_CODE_CHI, NL("[%08X] \u8FDE\u63A5\u5DF2\u505C\u7528 [%s]"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_XWINDOWSSERVICEBASE_SERVICE_FAILED_TO_START_ERROR, XLANGUAGE_ISO_639_3_CODE_ENG, NL("%s: Service failed to start Error[%08X]."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XWINDOWSSERVICEBASE_SERVICE_FAILED_TO_START_ERROR, XLANGUAGE_ISO_639_3_CODE_SPA, NL("%s: El servicio no pudo iniciar Error[%08X]."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XWINDOWSSERVICEBASE_SERVICE_FAILED_TO_START_ERROR, XLANGUAGE_ISO_639_3_CODE_FRE, NL("%s\u00A0: Le service n'a pas r\u00E9ussi \u00E0 d\u00E9marrer Erreur [%08X]."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XWINDOWSSERVICEBASE_SERVICE_FAILED_TO_START_ERROR, XLANGUAGE_ISO_639_3_CODE_GER, NL("%s: Dienst konnte nicht gestartet werden. Fehler[%08X]."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XWINDOWSSERVICEBASE_SERVICE_FAILED_TO_START_ERROR, XLANGUAGE_ISO_639_3_CODE_CHI, NL("%s\uFF1A\u670D\u52A1\u65E0\u6CD5\u542F\u52A8\u9519\u8BEF[%08X]\u3002"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_XWINDOWSSERVICEBASE_SERVICE_FAILED_TO_START, XLANGUAGE_ISO_639_3_CODE_ENG, NL("%s: Service failed to start."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XWINDOWSSERVICEBASE_SERVICE_FAILED_TO_START, XLANGUAGE_ISO_639_3_CODE_SPA, NL("%s: El servicio no pudo iniciarse."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XWINDOWSSERVICEBASE_SERVICE_FAILED_TO_START, XLANGUAGE_ISO_639_3_CODE_FRE, NL("%s\u00A0: Le service n'a pas pu d\u00E9marrer."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XWINDOWSSERVICEBASE_SERVICE_FAILED_TO_START, XLANGUAGE_ISO_639_3_CODE_GER, NL("%s: Dienst konnte nicht gestartet werden."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XWINDOWSSERVICEBASE_SERVICE_FAILED_TO_START, XLANGUAGE_ISO_639_3_CODE_CHI, NL("%s\uFF1A\u670D\u52A1\u542F\u52A8\u5931\u8D25\u3002"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_XWINDOWSSERVICEBASE_SERVICE_FAILED_TO_PAUSE, XLANGUAGE_ISO_639_3_CODE_ENG, NL("%s: Service failed to pause."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XWINDOWSSERVICEBASE_SERVICE_FAILED_TO_PAUSE, XLANGUAGE_ISO_639_3_CODE_SPA, NL("%s: No se pudo pausar el servicio."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XWINDOWSSERVICEBASE_SERVICE_FAILED_TO_PAUSE, XLANGUAGE_ISO_639_3_CODE_FRE, NL("%s\u00A0: Le service n'a pas pu \u00EAtre mis en pause."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XWINDOWSSERVICEBASE_SERVICE_FAILED_TO_PAUSE, XLANGUAGE_ISO_639_3_CODE_GER, NL("%s: Der Dienst konnte nicht angehalten werden."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XWINDOWSSERVICEBASE_SERVICE_FAILED_TO_PAUSE, XLANGUAGE_ISO_639_3_CODE_CHI, NL("%s\uFF1A\u670D\u52A1\u6682\u505C\u5931\u8D25\u3002"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_XWINDOWSSERVICEBASE_SERVICE_FAILED_TO_CONTINUE_ERROR, XLANGUAGE_ISO_639_3_CODE_ENG, NL("%s: Service failed to continue Error[%08X]."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XWINDOWSSERVICEBASE_SERVICE_FAILED_TO_CONTINUE_ERROR, XLANGUAGE_ISO_639_3_CODE_SPA, NL("%s: El servicio no pudo continuar Error[%08X]."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XWINDOWSSERVICEBASE_SERVICE_FAILED_TO_CONTINUE_ERROR, XLANGUAGE_ISO_639_3_CODE_FRE, NL("%s\u00A0: Le service n'a pas pu continuer Erreur [%08X]."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XWINDOWSSERVICEBASE_SERVICE_FAILED_TO_CONTINUE_ERROR, XLANGUAGE_ISO_639_3_CODE_GER, NL("%s: Dienst konnte nicht fortgesetzt werden. Fehler[%08X]."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XWINDOWSSERVICEBASE_SERVICE_FAILED_TO_CONTINUE_ERROR, XLANGUAGE_ISO_639_3_CODE_CHI, NL("%s\uFF1A\u670D\u52A1\u65E0\u6CD5\u7EE7\u7EED\u9519\u8BEF[%08X]\u3002"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_XWINDOWSSERVICEBASE_SERVICE_FAILED_TO_SHUTDOWN, XLANGUAGE_ISO_639_3_CODE_ENG, NL("%s: Service failed to shutdown."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XWINDOWSSERVICEBASE_SERVICE_FAILED_TO_SHUTDOWN, XLANGUAGE_ISO_639_3_CODE_SPA, NL("%s: El servicio no se pudo cerrar."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XWINDOWSSERVICEBASE_SERVICE_FAILED_TO_SHUTDOWN, XLANGUAGE_ISO_639_3_CODE_FRE, NL("%s\u00A0: Le service n'a pas pu s'arr\u00EAter."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XWINDOWSSERVICEBASE_SERVICE_FAILED_TO_SHUTDOWN, XLANGUAGE_ISO_639_3_CODE_GER, NL("%s: Der Dienst konnte nicht heruntergefahren werden."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XWINDOWSSERVICEBASE_SERVICE_FAILED_TO_SHUTDOWN, XLANGUAGE_ISO_639_3_CODE_CHI, NL("%s\uFF1A\u670D\u52A1\u5173\u95ED\u5931\u8D25\u3002"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_XWINDOWSSERVICEBASE_SERVICE_POWER_EVENT_EVENT, XLANGUAGE_ISO_639_3_CODE_ENG, NL("%s: Service Power Event : Event [%08X]."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XWINDOWSSERVICEBASE_SERVICE_POWER_EVENT_EVENT, XLANGUAGE_ISO_639_3_CODE_SPA, NL("%s: Evento de potencia del servicio: Evento [%08X]."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XWINDOWSSERVICEBASE_SERVICE_POWER_EVENT_EVENT, XLANGUAGE_ISO_639_3_CODE_FRE, NL("%s\u00A0: \u00C9v\u00E9nement d'alimentation du service\u00A0: \u00C9v\u00E9nement [%08X]."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XWINDOWSSERVICEBASE_SERVICE_POWER_EVENT_EVENT, XLANGUAGE_ISO_639_3_CODE_GER, NL("%s: Service Power Event: Ereignis [%08X]."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XWINDOWSSERVICEBASE_SERVICE_POWER_EVENT_EVENT, XLANGUAGE_ISO_639_3_CODE_CHI, NL("%s\uFF1A\u670D\u52A1\u7535\u6E90\u4E8B\u4EF6\uFF1A\u4E8B\u4EF6 [%08X]\u3002"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_XWINDOWSSERVICEBASE_SERVICE_SESSION_CHANGE_EVENT, XLANGUAGE_ISO_639_3_CODE_ENG, NL("%s: Service Session Change: Event [%08X]."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XWINDOWSSERVICEBASE_SERVICE_SESSION_CHANGE_EVENT, XLANGUAGE_ISO_639_3_CODE_SPA, NL("%s: Cambio de sesi\u00F3n de servicio: Evento [%08X]."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XWINDOWSSERVICEBASE_SERVICE_SESSION_CHANGE_EVENT, XLANGUAGE_ISO_639_3_CODE_FRE, NL("%s\u00A0: Modification de la session de service\u00A0: \u00E9v\u00E9nement [%08X]."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XWINDOWSSERVICEBASE_SERVICE_SESSION_CHANGE_EVENT, XLANGUAGE_ISO_639_3_CODE_GER, NL("%s: Dienstsitzungs\u00E4nderung: Ereignis [%08X]."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XWINDOWSSERVICEBASE_SERVICE_SESSION_CHANGE_EVENT, XLANGUAGE_ISO_639_3_CODE_CHI, NL("%s\uFF1A\u670D\u52A1\u4F1A\u8BDD\u66F4\u6539\uFF1A\u4E8B\u4EF6 [%08X]\u3002"), 0);

  #endif

  return true;
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRANSLATION_GEN::Sentences_DeleteAll()
* @brief      Sentences GEN_DELETE all
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRANSLATION_GEN::Sentences_DeleteAll()
{
  if(sentences.IsEmpty()) return false;

  sentences.DeleteContents();
  sentences.DeleteAll();

  return true;
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTRANSLATION_GEN::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTRANSLATION_GEN::Clean()
{
  codelanguage  = XLANGUAGE_ISO_639_3_CODE_ENG;

  emptysentence = NL("--");
}

