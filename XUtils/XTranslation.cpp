/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XTranslation.cpp
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XTranslation.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "XBase.h"
#include "XSystem.h"
#include "XTranslation_GEN.h"
#ifdef XFILE_ACTIVE
#include "XFileJSON.h"
#endif



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

XTRANSLATION* XTRANSLATION::instance  = NULL;



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRANSLATION::GetIsInstanced()
* @brief      Get is instanced
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRANSLATION::GetIsInstanced()
{
  return instance!=NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTRANSLATION& XTRANSLATION::GetInstance()
* @brief      Get instance
* @ingroup    XUTILS
* 
* @return     XTRANSLATION& : Reference to the requested object.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTRANSLATION& XTRANSLATION::GetInstance()
{
  if(!instance) instance = GEN_NEW XTRANSLATION();

  return (*instance);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRANSLATION::DelInstance()
* @brief      Del instance
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRANSLATION::DelInstance()
{
  if(instance)
    {
      GEN_DELETE instance;
      instance = NULL;

      return true;
    }

  return false;
}



#ifdef XFILE_ACTIVE
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRANSLATION::Ini(XPATH& xpath)
* @brief      Initialize the object
* @ingroup    XUTILS
* 
* @param[in]  xpath : path of the language file
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRANSLATION::Ini(XPATH& xpath)
{
   this->xpath = xpath;

   if(!LoadLanguageAvailable()) return false;

   return SetActual();
}
#else
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRANSLATION::Ini()
* @brief      Initialize the object
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRANSLATION::Ini()
{   
   if(!LoadLanguageAvailable()) return false;

   return SetActual();
}
#endif


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRANSLATION::IsInitialized()
* @brief      True after a successful Ini/SetActual (a language code is active).
* @ingroup    XUTILS
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRANSLATION::IsInitialized()
{
  return (codelanguage != XLANGUAGE_ISO_639_3_CODE_INVALID);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XTRANSLATION::GetActual()
* @brief      Get actual
* @ingroup    XUTILS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XTRANSLATION::GetActual()
{
  return codelanguage;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRANSLATION::SetActual()
* @brief      Fallback language selection when CFG has no valid defaultlanguage:
*             OS UI language if present in both catalogues, otherwise English.
*             Prefer APPFLOWCFG::Language_ApplyDefault() for the full sequence
*             (config → OS → English).
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRANSLATION::SetActual()
{
  XDWORD code = GEN_XSYSTEM.GetLanguageSO();

  if((code == XLANGUAGE_ISO_639_3_CODE_INVALID) ||
     !IsLanguageAvailable(code) ||
     !XTRANSLATION_GEN::GetInstance().IsLanguageAvailable(code))
    {
      code = XLANGUAGE_ISO_639_3_CODE_ENG;
    }

  return SetActual(code);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRANSLATION::SetActual(XDWORD codelanguage)
* @brief      Set actual
* @ingroup    XUTILS
* 
* @param[in]  codelanguage : Code language value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRANSLATION::SetActual(XDWORD codelanguage)
{
  XDWORD previous;

  if(this->codelanguage == codelanguage) return true;

  if(!IsLanguageAvailable(codelanguage)) return false;

  if(!XTRANSLATION_GEN::GetInstance().IsLanguageAvailable(codelanguage)) return false;

  previous = this->codelanguage;
  this->codelanguage = codelanguage;

  Translate_Delete();

  if(!Translate_Load())
    {
      // Never leave the catalogue empty after a failed switch: restore the previous language.
      this->codelanguage = previous;

      if(previous != XLANGUAGE_ISO_639_3_CODE_INVALID)
        {
          Translate_Delete();
          if(Translate_Load())
            {
              XTRANSLATION_GEN::GetInstance().SetActual(previous);
            }
        }

      return false;
    }

  XTRANSLATION_GEN::GetInstance().SetActual(codelanguage);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XTRANSLATION::SetActual_Next()
* @brief      Advance to the next language listed in the application catalogue that also exists in GEN.
* @ingroup    XUTILS
*
* @return     bool : true if the operation is successful; otherwise false.
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XTRANSLATION::SetActual_Next()
{
  XDWORD n = languageavailables.GetSize();
  int    start;

  if(n == 0) return false;

  start = GetIndexLanguageByCode(codelanguage);
  if(start < 0) start = -1;

  for(XDWORD step=1; step<=n; step++)
    {
      XDWORD idx  = (XDWORD)((start + (int)step) % (int)n);
      XDWORD code = languageavailables.Get(idx);

      if(code == XLANGUAGE_ISO_639_3_CODE_INVALID) continue;
      if(code == codelanguage) continue;
      if(!XTRANSLATION_GEN::GetInstance().IsLanguageAvailable(code)) continue;

      // Skip catalogue gaps (e.g. language listed but some sentences lack that column).
      if(SetActual(code)) return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRANSLATION::Translate_Add(XDWORD ID, XCHAR* sentence, XDWORD fixed)
* @brief      Translate add
* @ingroup    XUTILS
* 
* @param[in]  ID : Identifier to use.
* @param[in]  sentence : Sentence pointer to use.
* @param[in]  fixed : Fixed value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRANSLATION::Translate_Add(XDWORD ID, XCHAR* sentence, XDWORD fixed)
{
  if(!ID || ID >= XTRANSLATION_MAXSENTENCES) return false;
  if(!sentence) return false;

  XDWORD sizesentence = XSTRING::GetSize(sentence);
  if(fixed)
    {
      if(sizesentence > fixed) sizesentence = fixed;
    }

  XCHAR* newsentence = GEN_NEW XCHAR[sizesentence+1];
  if(newsentence)
    {
      memset(newsentence, 0, (sizesentence+1) * sizeof(XCHAR));
      memcpy(newsentence, sentence, sizesentence * sizeof(XCHAR));

      if(sentences[ID]) GEN_DELETE_ARRAY sentences[ID];
      sentences[ID] = newsentence;
      return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* XTRANSLATION::Translate_GetSentence(XDWORD ID)
* @brief      Translate get sentence
* @ingroup    XUTILS
* 
* @param[in]  ID : Identifier to use.
* 
* @return     XCHAR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* XTRANSLATION::Translate_GetSentence(XDWORD ID)
{
  if(ID >= XTRANSLATION_MAXSENTENCES) 
    {
      return emptysentence.Get();
    }

  if(!sentences[ID])  
    {
      return emptysentence.Get();
    }

  return sentences[ID];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRANSLATION::End()
* @brief      End the object
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRANSLATION::End()
{
  languageavailables.DeleteAll();

  Translate_Delete();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTRANSLATION::XTRANSLATION()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTRANSLATION::XTRANSLATION()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTRANSLATION::~XTRANSLATION()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTRANSLATION::~XTRANSLATION()
{
  End();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRANSLATION::LoadLanguageAvailable()
* @brief      Load language available
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRANSLATION::LoadLanguageAvailable()
{
  #ifdef XFILE_ACTIVE

  XFILEJSON xfilejson;

  if(!xfilejson.Open(xpath))       return false;
  if(!xfilejson.ReadAllFile())     return false;
  if(!xfilejson.DecodeAllLines())  return false;

  XFILEJSONARRAY* languages  = (XFILEJSONARRAY*)xfilejson.GetObj(NL("languages"));
  if(languages)
    {
      for(XDWORD c=0; c<languages->GetValues()->GetSize(); c++)
        {
          XFILEJSONVALUE* value = languages->GetValues()->Get(c);
          if(value)
            {
              if(value->GetType() == XFILEJSONVALUETYPE_STRING)
                {
                  XSTRING strvalue = (XSTRING)(value->GetValueString());
                  
                  if(!strvalue.IsEmpty())
                    {
                      XDWORD code = ISO_639_3.Code_GetByCodeAlpha3(strvalue.Get());
                      if(code != XLANGUAGE_ISO_639_3_CODE_INVALID)
                        {
                          languageavailables.Add(code);
                        }
                        else
                        {
                          XDWORD code = ISO_639_3.Code_GetByEnglishName(strvalue.Get());
                          if(code != XLANGUAGE_ISO_639_3_CODE_INVALID)
                            {
                              languageavailables.Add(code);
                            }
                            else
                            {
                              XDWORD code = ISO_639_3.Code_GetByAlias(strvalue.Get());
                              if(code != XLANGUAGE_ISO_639_3_CODE_INVALID)
                                {
                                  languageavailables.Add(code);
                                }
                            }
                        }
                    }
                    
                }
            }
        }
    }

  xfilejson.Close();

  return true;

  #else

  return false;

  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRANSLATION::IsLanguageAvailable(XDWORD code)
* @brief      Is language available
* @ingroup    XUTILS
* 
* @param[in]  code : Code value.
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRANSLATION::IsLanguageAvailable(XDWORD code)
{
  if(GetIndexLanguageByCode(code)!= -1) return true;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRANSLATION::AddLanguageAvailable(XDWORD code)
* @brief      Add language available
* @ingroup    XUTILS
* 
* @param[in]  code : Code value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRANSLATION::AddLanguageAvailable(XDWORD code)
{
  if(!IsLanguageAvailable(code)) return false;

  languageavailables.Add(code);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XTRANSLATION::GetIndexLanguageByCode(XDWORD code)
* @brief      Get index language by code
* @ingroup    XUTILS
* 
* @param[in]  code : Code value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XTRANSLATION::GetIndexLanguageByCode(XDWORD code)
{
  for(XDWORD c=0; c<languageavailables.GetSize(); c++)
    {
      if(languageavailables.Get(c) == code) return c;
    }

  return -1;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRANSLATION::Translate_Load()
* @brief      Translate load
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRANSLATION::Translate_Load()
{
  #ifdef XFILE_ACTIVE

  XFILEJSON xfilejson;

  if(!xfilejson.Open(xpath))       return false;
  if(!xfilejson.ReadAllFile())     return false;
  if(!xfilejson.DecodeAllLines())  return false;

  int languageindex = GetIndexLanguageByCode(codelanguage);
  if(languageindex == -1) return false;

  XFILEJSONARRAY* translations  = (XFILEJSONARRAY*)xfilejson.GetObj(NL("translations"));
  if(translations)
    {
      for(XDWORD c=0; c<translations->GetValues()->GetSize(); c++)
        {
          XFILEJSONVALUE* translation = translations->GetValues()->Get(c);
          if(translation)
            {
              XSTRING IDstr = translation->GetName()->Get();
              if(!IDstr.IsEmpty())
                {
                  XDWORD ID = IDstr.ConvertToDWord();
                  if(ID < XTRANSLATION_MAXSENTENCES)
                    {
                      XFILEJSONARRAY* translationarray  = (XFILEJSONARRAY*)translation->GetValuePointer();
                      if(translationarray)
                        {
                          XDWORD nvalues = translationarray->GetValues()->GetSize();
                          XDWORD slot    = (XDWORD)(languageindex + 1);

                          // languages[i] maps to translation array slot i+1 (slot 0 is fixed width).
                          // If this language column is missing, fall back to English, then to the first string.
                          if(slot >= nvalues)
                            {
                              int engindex = GetIndexLanguageByCode(XLANGUAGE_ISO_639_3_CODE_ENG);

                              if((engindex >= 0) && ((XDWORD)(engindex + 1) < nvalues))
                                {
                                  slot = (XDWORD)(engindex + 1);
                                }
                               else if(nvalues >= 2)
                                {
                                  slot = 1;
                                }
                               else
                                {
                                  continue;
                                }
                            }

                          XFILEJSONVALUE* fixedvalue = translationarray->GetValues()->Get(0);
                          XFILEJSONVALUE* sentencevalue = translationarray->GetValues()->Get(slot);
                          if(!fixedvalue || !sentencevalue) continue;

                          XDWORD  sizesentencefixed = (XDWORD)fixedvalue->GetValueInteger();
                          XSTRING sentence          = (XSTRING)sentencevalue->GetValueString();

                          Translate_Add(ID, sentence.Get(), sizesentencefixed);
                        }
                    }
                }
            }
        }
    }

  xfilejson.Close();

  return true;

  #else

  return false;

  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRANSLATION::Translate_Delete()
* @brief      Translate delete
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRANSLATION::Translate_Delete()
{
  for(XDWORD c=0; c<XTRANSLATION_MAXSENTENCES; c++)
    {
      if(sentences[c])
        {
          GEN_DELETE_ARRAY sentences[c];
          sentences[c] = NULL;
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTRANSLATION::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTRANSLATION::Clean()
{
  codelanguage  = XLANGUAGE_ISO_639_3_CODE_INVALID;

  emptysentence = NL("--");

  for(XDWORD c=0; c<XTRANSLATION_MAXSENTENCES; c++)
    {
      sentences[c] = NULL;
    }
}



