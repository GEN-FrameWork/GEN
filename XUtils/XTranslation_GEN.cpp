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

#include "XTranslation.h"



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


  #ifdef ANONYMOUS_MODE


  #else

  //--- GENERIC  -----------------------------------------------------------------------------------------------------

  Sentence_Add(XTRANSLATION_GEN_ID_OK, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Ok"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_ERROR, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Error"), 0); 
  Sentence_Add(XTRANSLATION_GEN_ID_YES, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Yes"), 0); 
  Sentence_Add(XTRANSLATION_GEN_ID_NO, XLANGUAGE_ISO_639_3_CODE_ENG, _L("No"), 0); 
  Sentence_Add(XTRANSLATION_GEN_ID_TRUE, XLANGUAGE_ISO_639_3_CODE_ENG, _L("True"), 0); 
  Sentence_Add(XTRANSLATION_GEN_ID_FALSE, XLANGUAGE_ISO_639_3_CODE_ENG, _L("False"), 0); 

  Sentence_Add(XTRANSLATION_GEN_ID_OK, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Correcto"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_ERROR, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Error"), 0); 
  Sentence_Add(XTRANSLATION_GEN_ID_YES, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Si"), 0); 
  Sentence_Add(XTRANSLATION_GEN_ID_NO, XLANGUAGE_ISO_639_3_CODE_SPA, _L("No"), 0); 
  Sentence_Add(XTRANSLATION_GEN_ID_TRUE, XLANGUAGE_ISO_639_3_CODE_SPA, _L("true"), 0); 
  Sentence_Add(XTRANSLATION_GEN_ID_FALSE, XLANGUAGE_ISO_639_3_CODE_SPA, _L("false"), 0); 


  //--- DATETIME -----------------------------------------------------------------------------------------------------

  // MONTHS 

  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_JANUARY, XLANGUAGE_ISO_639_3_CODE_ENG, _L("January"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_FEBRUARY, XLANGUAGE_ISO_639_3_CODE_ENG, _L("February"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_MARCH, XLANGUAGE_ISO_639_3_CODE_ENG, _L("March"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_APRIL, XLANGUAGE_ISO_639_3_CODE_ENG, _L("April"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_MAY, XLANGUAGE_ISO_639_3_CODE_ENG, _L("May"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_JUNE, XLANGUAGE_ISO_639_3_CODE_ENG, _L("June"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_JULY, XLANGUAGE_ISO_639_3_CODE_ENG, _L("July"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_AUGUST, XLANGUAGE_ISO_639_3_CODE_ENG, _L("August"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_SEPTEMBER, XLANGUAGE_ISO_639_3_CODE_ENG, _L("September"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_OCTOBER, XLANGUAGE_ISO_639_3_CODE_ENG, _L("October"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_NOVEMBER, XLANGUAGE_ISO_639_3_CODE_ENG, _L("November"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_DECEMBER, XLANGUAGE_ISO_639_3_CODE_ENG, _L("December"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_PRESEPARATOR, XLANGUAGE_ISO_639_3_CODE_ENG, _L("of"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_JANUARY, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Enero"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_FEBRUARY, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Febrero"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_MARCH, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Marzo"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_APRIL, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Abril"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_MAY, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Mayo"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_JUNE, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Junio"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_JULY, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Julio"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_AUGUST, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Agosto"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_SEPTEMBER, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Septiembre"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_OCTOBER, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Octubre"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_NOVEMBER, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Noviembre"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_DECEMBER, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Diciembre"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_MONTH_PRESEPARATOR, XLANGUAGE_ISO_639_3_CODE_SPA, _L("de"), 0);

  // DAYOFWEEK 

  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_SUNDAY, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Sunday"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_MONDAY, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Monday"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_TUESDAY, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Tuesday"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_WEDNESDAY, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Wednesday"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_THURSDAY, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Thursday"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_FRIDAY, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Friday"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_SATURDAY, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Saturday"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_SUNDAY, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Domingo"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_MONDAY, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Lunes"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_TUESDAY, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Martes"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_WEDNESDAY, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Miercoles"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_THURSDAY, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Jueves"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_FRIDAY, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Viernes"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XDATETIME_DAYOFWEEK_SATURDAY, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Sabado"), 0);

  //--- XTIMER -------------------------------------------------------------------------------------------------------

  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_XX_YEARS, XLANGUAGE_ISO_639_3_CODE_ENG, _L("%d years"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ONE_YEAR, XLANGUAGE_ISO_639_3_CODE_ENG, _L("one year"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_XX_MONTHS, XLANGUAGE_ISO_639_3_CODE_ENG, _L("%d months"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ONE_MONTH, XLANGUAGE_ISO_639_3_CODE_ENG, _L("one month"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_XX_DAYS, XLANGUAGE_ISO_639_3_CODE_ENG, _L("%d days"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ONE_DAY, XLANGUAGE_ISO_639_3_CODE_ENG, _L("one day"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_XX_HOURS, XLANGUAGE_ISO_639_3_CODE_ENG, _L("%d hours"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ONE_HOUR, XLANGUAGE_ISO_639_3_CODE_ENG, _L("one hour"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_XX_MINUTES, XLANGUAGE_ISO_639_3_CODE_ENG, _L("%d minutes"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ONE_MINUTE, XLANGUAGE_ISO_639_3_CODE_ENG, _L("one minute"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_XX_SECONDS, XLANGUAGE_ISO_639_3_CODE_ENG, _L("%d seconds"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ONE_SECOND, XLANGUAGE_ISO_639_3_CODE_ENG, _L("one second"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ZERO_SECONDS, XLANGUAGE_ISO_639_3_CODE_ENG, _L("zero seconds"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_XX_YEARS, XLANGUAGE_ISO_639_3_CODE_SPA, _L("%d a\xf1os"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ONE_YEAR, XLANGUAGE_ISO_639_3_CODE_SPA, _L("un a\xf1o"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_XX_MONTHS, XLANGUAGE_ISO_639_3_CODE_SPA, _L("%d meses"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ONE_MONTH, XLANGUAGE_ISO_639_3_CODE_SPA, _L("un mes"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_XX_DAYS, XLANGUAGE_ISO_639_3_CODE_SPA, _L("%d dias"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ONE_DAY, XLANGUAGE_ISO_639_3_CODE_SPA, _L("un dia"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_XX_HOURS, XLANGUAGE_ISO_639_3_CODE_SPA, _L("%d horas"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ONE_HOUR, XLANGUAGE_ISO_639_3_CODE_SPA, _L("una hora"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_XX_MINUTES, XLANGUAGE_ISO_639_3_CODE_SPA, _L("%d minutos"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ONE_MINUTE, XLANGUAGE_ISO_639_3_CODE_SPA, _L("un minuto"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_XX_SECONDS, XLANGUAGE_ISO_639_3_CODE_SPA, _L("%d segundos"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ONE_SECOND, XLANGUAGE_ISO_639_3_CODE_SPA, _L("un segundo"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_XTIMER_ZERO_SECONDS, XLANGUAGE_ISO_639_3_CODE_SPA, _L("cero segundos"), 0);


  //--- APPLICATION LOG ----------------------------------------------------------------------------------------------

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INILOG, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Activating LOG system"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_ENDLOG, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Disabling LOG system"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INIAPPSTATUS, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Activating App status"), 0);        
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_ENDAPPSTATUS, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Disabling App status"), 0); 
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INIINTERNETSTATUS, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Activating Internet status"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_ENDINTERNETSTATUS, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Disabling Internet status"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPROOTPATH, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Application Root Path"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPVERSION, XLANGUAGE_ISO_639_3_CODE_ENG, _L("APP version"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_SOVERSION, XLANGUAGE_ISO_639_3_CODE_ENG, _L("S.O. version"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_TOTALMEMORY, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Total memory %d Kb, free %d Kb (%d%%)."), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INILOG, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Activando sistema de LOG"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_ENDLOG, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Desactivando sistema de LOG"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INIAPPSTATUS, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Activando estado aplicacion"), 0);        
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_ENDAPPSTATUS, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Desactivando estado aplicacion"), 0); 
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INIINTERNETSTATUS, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Activando estado Internet"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_ENDINTERNETSTATUS, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Desactivando estado Internet"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPROOTPATH, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Camino raiz de application"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPVERSION, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Version APP"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_SOVERSION, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Version S.O."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_TOTALMEMORY, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Memoria total %d Kb, libre %d Kb (%d%%)."), 0);
  
  //--- APPLICATION CONSOLE EXIT CODE --------------------------------------------------------------------------------

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_NOTINFO, XLANGUAGE_ISO_639_3_CODE_ENG, _L("No closing information"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_SERIOUSERROR, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Application closed due to a serious error"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_UPDATE, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Application closed by update"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_APPLICATION, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Execution of the application concluded"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_USER, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Application closed by the user"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_SHUTDOWN, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Application closed by shutdown of the operating system"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_INVALIDLICENSE, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Application closed by invalid license"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_EXPIREDLICENSE, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Closed application for expired license"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_NOTINFO, XLANGUAGE_ISO_639_3_CODE_SPA, _L("No hay informacion de cierre"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_SERIOUSERROR, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Aplicacion cerrada debido a un error grave"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_UPDATE, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Aplicacion cerrada por actualizacion"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_APPLICATION, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Ejecucion de la aplicacion concluida"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_USER, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Aplicacion cerrada por el usuario"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_SHUTDOWN, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Aplicacion cerrada por apagado del sistema operativo"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_INVALIDLICENSE, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Aplication cerrada por licencia invalida"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWCONSOLE_EXIT_BY_EXPIREDLICENSE, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Aplication cerrada por licencia vencida"), 0);


  //--- APPLICATION STATUS SHOW --------------------------------------------------------------------------------------

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_OSVERSION, XLANGUAGE_ISO_639_3_CODE_ENG, _L("O.S Version"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_CPUMEMORY, XLANGUAGE_ISO_639_3_CODE_ENG, _L("CPU Memory"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_AVERANGE, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Averange"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_CURRENTDATE, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Current date"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_OPERATINGTIME, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Operating time"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_MEMORYFREE, XLANGUAGE_ISO_639_3_CODE_ENG, _L("%.1f %s, free %.1f %s (%d%%)"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_CPUTOTALAVG, XLANGUAGE_ISO_639_3_CODE_ENG, _L("avg. %d%% (max. %d%%)"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_CPUAPPAVG, XLANGUAGE_ISO_639_3_CODE_ENG, _L("app %s avg. %d%% (max. %d%%)"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_OSVERSION, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Version S.O."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_CPUMEMORY, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Memoria CPU"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_AVERANGE, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Media"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_CURRENTDATE, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Fecha actual"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_OPERATINGTIME, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Tiempo de funcionamiento"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_MEMORYFREE, XLANGUAGE_ISO_639_3_CODE_SPA, _L("%.1f %s, libre %.1f %s (%d%%)"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_CPUTOTALAVG, XLANGUAGE_ISO_639_3_CODE_SPA, _L("media %d%% (max. %d%%)"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_APPLICATIONSTATUS_CPUAPPAVG, XLANGUAGE_ISO_639_3_CODE_SPA, _L("app %s media %d%% (max. %d%%)"), 0);


  //--- INTERNET STATUS SHOW -----------------------------------------------------------------------------------------

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INTERNETSTATUS_LOCALIP, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Local IP"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INTERNETSTATUS_CONNECTION, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Internet Connection"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INTERNETSTATUS_LATENCY, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Latency"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INTERNETSTATUS_PUBLICIP, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Public IP"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INTERNETSTATUS_LATENCYMS, XLANGUAGE_ISO_639_3_CODE_ENG, _L("%d ms"), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INTERNETSTATUS_LOCALIP, XLANGUAGE_ISO_639_3_CODE_SPA, _L("IP local"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INTERNETSTATUS_CONNECTION, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Conexion a Internet"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INTERNETSTATUS_LATENCY, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Latencia"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INTERNETSTATUS_PUBLICIP, XLANGUAGE_ISO_639_3_CODE_SPA, _L("IP publica"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWEXTENDED_INTERNETSTATUS_LATENCYMS, XLANGUAGE_ISO_639_3_CODE_SPA, _L("%d ms"), 0);


  //--- APPLICATION UPDATE -------------------------------------------------------------------------------------------

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_VERSIONAVAILABLE, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Update Version available %d.%d.%d"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_NOVERSIONAVAILABLE, XLANGUAGE_ISO_639_3_CODE_ENG, _L("No version available to update"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_VERSIONIS, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Version %d.%d.%d is "), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_VERSION_OLD, XLANGUAGE_ISO_639_3_CODE_ENG, _L("old"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_VERSION_EQUAL, XLANGUAGE_ISO_639_3_CODE_ENG, _L("equal"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_VERSION_NEW, XLANGUAGE_ISO_639_3_CODE_ENG, _L("new"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_WILLNOTBEUPDATED, XLANGUAGE_ISO_639_3_CODE_ENG, _L(". It will not be updated"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_DOWNLOADFILE, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Download file %s (%dk)."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_UNZIPFILE, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Unzip file %s (%dk) -> %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_DOWNLOADSFILE, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Downloads file %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_BACKUPORIGINALFILE, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Backup original file %s (%dk)."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_BACKUPORIGINALFILES, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Backup Original file %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_APP_RELEASERESOURCES, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Application release blocked resources"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_UPDATEFILE, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Copy update file %s (%dk)."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_UPDATEEXECFILE, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Notify update EXEC file %s (%dk)."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_UPDATEFILES, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Update files %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_APP_END, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Terminate application"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_RESTOREUPDATEFILE, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Restore file %s from backup (%dk)."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_RESTOREUPDATEFILES, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Restore update files %s."), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_VERSIONAVAILABLE, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Version de actualizacion disponible %d.%d.%d"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_NOVERSIONAVAILABLE, XLANGUAGE_ISO_639_3_CODE_SPA, _L("No hay version disponible para actualizar"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_VERSIONIS, XLANGUAGE_ISO_639_3_CODE_SPA, _L("La version %d.%d.%d es "), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_VERSION_OLD, XLANGUAGE_ISO_639_3_CODE_SPA, _L("antigua"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_VERSION_EQUAL, XLANGUAGE_ISO_639_3_CODE_SPA, _L("igual"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_VERSION_NEW, XLANGUAGE_ISO_639_3_CODE_SPA, _L("nueva"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_WILLNOTBEUPDATED, XLANGUAGE_ISO_639_3_CODE_SPA, _L(". No se actualizara"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_DOWNLOADFILE, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Descarga fichero %s (%dk)."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_UNZIPFILE, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Descomprimir fichero %s (%dk) -> %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_DOWNLOADSFILE, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Descargas de fichero %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_BACKUPORIGINALFILE, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Copia de seguridad del fichero original %s (%dk)."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_BACKUPORIGINALFILES, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Copia de seguridad de ficheros originales %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_APP_RELEASERESOURCES, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Aplicacion libera recursos bloqueados"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_UPDATEFILE, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Copiar fichero de actualizacion %s (%dk)."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_UPDATEEXECFILE, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Notificar actualizacion de EXEC %s (%dk)."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_UPDATEFILES, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Actualizar ficheros %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_APP_END, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Terminar aplicacion"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_RESTOREUPDATEFILE, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Restaurar fichero %s desde copia (%dk)."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWUPDATE_RESTOREUPDATEFILES, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Restaurar ficheros de actualizacion %s."), 0);


  //--- APPLICATION WEB SERVER ---------------------------------------------------------------------------------------

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_TLSCREDENTIALSNOTLOADED, XLANGUAGE_ISO_639_3_CODE_ENG, _L("WEB server: TLS credentials could not be loaded from the configured provider or credential paths."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_TLSCREDENTIALSNOTLOADED_PORT, XLANGUAGE_ISO_639_3_CODE_ENG, _L("WEB server (port %d): TLS credentials could not be loaded from the configured provider or credential paths."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_AUTHNOPASSWORD, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Authenticated access is active but there is no password in the configuration, it will be resolved by events."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_AUTHCONFIGURED, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Authenticated access configured for the web server (user [%s])."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_AUTHREQUEST, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Request from [%s] to the web server, authentication of the user [%s]: %s"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_AUTH_INVALID, XLANGUAGE_ISO_639_3_CODE_ENG, _L("INVALID!"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_INVALIDUSERORPASSWORD, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Invalid user or password!"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_WEBBROWSER, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Web browser    : %s, with: %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_ERRORTOEXECUTE, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Error to execute the %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_REQUESTRESULT, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Request from [%s] to the web server \"%s\" %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_SENT, XLANGUAGE_ISO_639_3_CODE_ENG, _L("sent"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_NOTSEND, XLANGUAGE_ISO_639_3_CODE_ENG, _L("not send"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_ERRORCOMMANDORPARAMS, XLANGUAGE_ISO_639_3_CODE_ENG, _L("Error: command or erroneous parameters."), 0);

  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_TLSCREDENTIALSNOTLOADED, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Servidor WEB: no se pudieron cargar las credenciales TLS del proveedor o rutas configuradas."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_TLSCREDENTIALSNOTLOADED_PORT, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Servidor WEB (puerto %d): no se pudieron cargar las credenciales TLS del proveedor o rutas configuradas."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_AUTHNOPASSWORD, XLANGUAGE_ISO_639_3_CODE_SPA, _L("El acceso autenticado esta activo pero no hay contrasena en la configuracion, se resolvera por eventos."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_AUTHCONFIGURED, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Acceso autenticado configurado para el servidor web (usuario [%s])."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_AUTHREQUEST, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Peticion desde [%s] al servidor web, autenticacion del usuario [%s]: %s"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_AUTH_INVALID, XLANGUAGE_ISO_639_3_CODE_SPA, _L("INVALIDO!"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_INVALIDUSERORPASSWORD, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Usuario o contrasena invalidos!"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_WEBBROWSER, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Navegador web  : %s, con: %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_ERRORTOEXECUTE, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Error al ejecutar %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_REQUESTRESULT, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Peticion desde [%s] al servidor web \"%s\" %s."), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_SENT, XLANGUAGE_ISO_639_3_CODE_SPA, _L("enviada"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_NOTSEND, XLANGUAGE_ISO_639_3_CODE_SPA, _L("no enviada"), 0);
  Sentence_Add(XTRANSLATION_GEN_ID_APPFLOWWEBSERVER_ERRORCOMMANDORPARAMS, XLANGUAGE_ISO_639_3_CODE_SPA, _L("Error: comando o parametros erroneos."), 0);

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
* @fn         bool XTRANSLATION_GEN::Sentences_AddToTranslation(XDWORD codelanguage)
* @brief      Sentences add to translation
* @ingroup    XUTILS
* 
* @param[in]  codelanguage : Codelanguage value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRANSLATION_GEN::Sentences_AddToTranslation(XDWORD codelanguage)
{
  bool existsentenceGENlanguage = false;

  for(XDWORD c=0; c<sentences.GetSize(); c++)
    {
      XTRANSLATION_GEN_SENTENCE* sentenceGEN = sentences.Get(c);
      if(sentenceGEN)
        {
          if(sentenceGEN->codelanguage == codelanguage)
            {
              GEN_XTRANSLATION.Translate_Add(sentenceGEN->ID, sentenceGEN->sentence, sentenceGEN->fixed);
              existsentenceGENlanguage = true;
            }
        }
    }

  return existsentenceGENlanguage;
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

}





