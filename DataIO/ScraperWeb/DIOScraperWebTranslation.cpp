/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOScraperWebTranslation.cpp
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOScraperWebTranslation.h"

#include "XThread.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOTRANSLATION_RESULT::DIOTRANSLATION_RESULT()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOTRANSLATION_RESULT::DIOTRANSLATION_RESULT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOTRANSLATION_RESULT::~DIOTRANSLATION_RESULT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOTRANSLATION_RESULT::~DIOTRANSLATION_RESULT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XLANGUAGE_CODE DIOTRANSLATION_RESULT::GetLanguageTextCode()
* @brief      Get language text code
* @ingroup    DATAIO
* 
* @return     XLANGUAGE_CODE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XLANGUAGE_CODE DIOTRANSLATION_RESULT::GetLanguageTextCode()
{
  return languagetextcode;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOTRANSLATION_RESULT::SetLanguageTextCode(XLANGUAGE_CODE code)
* @brief      Set language text code
* @ingroup    DATAIO
* 
* @param[in]  code : Code value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOTRANSLATION_RESULT::SetLanguageTextCode(XLANGUAGE_CODE code)
{
  languagetextcode = code;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XLANGUAGE_CODE DIOTRANSLATION_RESULT::GetLanguageTranslationCode()
* @brief      Get language translation code
* @ingroup    DATAIO
* 
* @return     XLANGUAGE_CODE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XLANGUAGE_CODE DIOTRANSLATION_RESULT::GetLanguageTranslationCode()
{
  return languagetranslationcode;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOTRANSLATION_RESULT::SetLanguageTranslationCode(XLANGUAGE_CODE code)
* @brief      Set language translation code
* @ingroup    DATAIO
* 
* @param[in]  code : Code value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOTRANSLATION_RESULT::SetLanguageTranslationCode(XLANGUAGE_CODE code)
{
  languagetranslationcode = code;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOTRANSLATION_RESULT::GetTranslation()
* @brief      Get translation
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOTRANSLATION_RESULT::GetTranslation()
{
  return &translation;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOTRANSLATION_RESULT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOTRANSLATION_RESULT::Clean()
{
  languagetextcode        = XLANGUAGE_ISO_639_3_CODE_INVALID;
  languagetranslationcode = XLANGUAGE_ISO_639_3_CODE_INVALID;
}






/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSCRAPERWEBTRANSLATION::DIOSCRAPERWEBTRANSLATION() : DIOSCRAPERWEB()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSCRAPERWEBTRANSLATION::DIOSCRAPERWEBTRANSLATION() : DIOSCRAPERWEB()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSCRAPERWEBTRANSLATION::~DIOSCRAPERWEBTRANSLATION()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSCRAPERWEBTRANSLATION::~DIOSCRAPERWEBTRANSLATION()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEBTRANSLATION::ChangeURL(XCHAR* maskurl, DIOURL& url)
* @brief      Change URL
* @ingroup    DATAIO
* 
* @param[in]  maskurl : Maskurl pointer to use.
* @param[in]  url : URL to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEBTRANSLATION::ChangeURL(XCHAR* maskurl, DIOURL& url)
{
  XSTRING _maskurl;

  _maskurl = maskurl;

  int found = _maskurl.Find(__L("google"), true);
  if(found != XSTRING_NOTFOUND)
    {
      DIOURL  urltext;
      XBUFFER urlbuffer;
      XSTRING googlecodetext;
      XSTRING googlecodetranslate;

      urltext = text.Get();
      urltext.ConvertToUTF8(urlbuffer);
      urltext.EncodeUnsafeCharsFromBuffer(urlbuffer);

      GoogleLanguageCode(languagetextcode       , googlecodetext);
      GoogleLanguageCode(languagetranslationcode, googlecodetranslate);

      url.Format(maskurl, urltext.Get(), googlecodetext.Get(), googlecodetext.Get(), googlecodetranslate.Get());

      return true;
    }


  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEBTRANSLATION::Get(XCHAR* text, XLANGUAGE_CODE languagetextcode, XLANGUAGE_CODE languagetranslationcode, XSTRING& translation, int timeoutforurl, XSTRING* localIP, bool usecache)
* @brief      Get value
* @ingroup    DATAIO
* 
* @param[in]  text : Text to use.
* @param[in]  languagetextcode : Languagetextcode value.
* @param[in]  languagetranslationcode : Languagetranslationcode value.
* @param[in]  translation : Translation value.
* @param[in]  timeoutforurl : Timeoutforurl value.
* @param[in]  localIP : Local IP pointer to use.
* @param[in]  usecache : Usecache value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEBTRANSLATION::Get(XCHAR* text, XLANGUAGE_CODE languagetextcode, XLANGUAGE_CODE languagetranslationcode, XSTRING& translation, int timeoutforurl, XSTRING* localIP, bool usecache)
{
  XSTRING _text;
  bool    status = false;

  if(xmutexdo) xmutexdo->Lock();

  _text.Format(__L("%s %02d %02d"), text, languagetextcode, languagetranslationcode);

  this->text                      = text;
  this->languagetextcode          = languagetextcode;
  this->languagetranslationcode   = languagetranslationcode;

  DIOTRANSLATION_RESULT* translationresult;

  if(usecache)
    {
      translationresult= (DIOTRANSLATION_RESULT*)cache->Get(_text);
      if(translationresult)
        {
          translation = (*translationresult->GetTranslation());

          if(xmutexdo) xmutexdo->UnLock();
          return true;
        }
    }

  if(Load(DIOSCRAPERWEBTRANSLATION_NAMEFILE))
    {
      if(Do(DIOSCRAPERWEBTRANSLATION_NAMESERVICE, timeoutforurl, localIP))
        {
          translation = GetValue(__L("TRANSLATION"));

          if(usecache)
            {
              translationresult = GEN_NEW DIOTRANSLATION_RESULT();
              if(translationresult)
                {
                  translationresult->SetLanguageTextCode(languagetextcode);
                  translationresult->SetLanguageTranslationCode(languagetranslationcode);
                  translationresult->GetTranslation()->Set(translation);

                  cache->Add(_text, translationresult);
                }
            }

          status = true;
        }
    }

  if(xmutexdo) xmutexdo->UnLock();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEBTRANSLATION::Get(XSTRING& text, XLANGUAGE_CODE languagetextcode, XLANGUAGE_CODE languagetranslationcode, XSTRING& translation, int timeoutforurl, XSTRING* localIP, bool usecache)
* @brief      Get value
* @ingroup    DATAIO
* 
* @param[in]  text : Text to use.
* @param[in]  languagetextcode : Languagetextcode value.
* @param[in]  languagetranslationcode : Languagetranslationcode value.
* @param[in]  translation : Translation value.
* @param[in]  timeoutforurl : Timeoutforurl value.
* @param[in]  localIP : Local IP pointer to use.
* @param[in]  usecache : Usecache value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEBTRANSLATION::Get(XSTRING& text, XLANGUAGE_CODE languagetextcode, XLANGUAGE_CODE languagetranslationcode, XSTRING& translation, int timeoutforurl, XSTRING* localIP, bool usecache)
{
  return Get(text.Get(), languagetextcode, languagetranslationcode, translation, timeoutforurl, localIP, usecache);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEBTRANSLATION::GoogleLanguageCode(XLANGUAGE_CODE languagecode, XSTRING& googlecode)
* @brief      Google language code
* @ingroup    DATAIO
* 
* @param[in]  languagecode : Languagecode value.
* @param[in]  googlecode : Googlecode value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEBTRANSLATION::GoogleLanguageCode(XLANGUAGE_CODE languagecode, XSTRING& googlecode)
{
  googlecode.Empty();

  switch(languagecode)
    {
      case XLANGUAGE_ISO_639_3_CODE_INVALID :                             break;  //   Invalid
      case XLANGUAGE_ISO_639_3_CODE_AAR     :                             break;  //   Afar
      case XLANGUAGE_ISO_639_3_CODE_ABK     :                             break;  //   Abkhazian
      case XLANGUAGE_ISO_639_3_CODE_AFR     : googlecode = __L("af");     break;  //   Afrikaans
      case XLANGUAGE_ISO_639_3_CODE_AKA     :                             break;  //   Akan
      case XLANGUAGE_ISO_639_3_CODE_ALB     : googlecode = __L("sq");     break;  //   Albanian
      case XLANGUAGE_ISO_639_3_CODE_AMH     :                             break;  //   Amharic
      case XLANGUAGE_ISO_639_3_CODE_ARA     : googlecode = __L("ar");     break;  //   Arabic
      case XLANGUAGE_ISO_639_3_CODE_ARG     :                             break;  //   Aragonese
      case XLANGUAGE_ISO_639_3_CODE_ARM     :                             break;  //   Armenia
      case XLANGUAGE_ISO_639_3_CODE_ASM     :                             break;  //   Assamese
      case XLANGUAGE_ISO_639_3_CODE_AVA     :                             break;  //   Avaric
      case XLANGUAGE_ISO_639_3_CODE_AVE     :                             break;  //   Avestan
      case XLANGUAGE_ISO_639_3_CODE_AYM     :                             break;  //   Aymara
      case XLANGUAGE_ISO_639_3_CODE_AZE     : googlecode = __L("az");     break;  //   Azerbaijani
      case XLANGUAGE_ISO_639_3_CODE_BAK     :                             break;  //   Bashkir
      case XLANGUAGE_ISO_639_3_CODE_BAM     :                             break;  //   Bambara
      case XLANGUAGE_ISO_639_3_CODE_BAQ     : googlecode = __L("eu");     break;  //   Basque
      case XLANGUAGE_ISO_639_3_CODE_BEL     : googlecode = __L("be");     break;  //   Belarusian
      case XLANGUAGE_ISO_639_3_CODE_BEN     : googlecode = __L("bn");     break;  //   Bengali
      case XLANGUAGE_ISO_639_3_CODE_BIH     :                             break;  //   Bihari languages
      case XLANGUAGE_ISO_639_3_CODE_BIS     :                             break;  //   Bislama
      case XLANGUAGE_ISO_639_3_CODE_BOS     :                             break;  //   Bosnian
      case XLANGUAGE_ISO_639_3_CODE_BRE     :                             break;  //   Breton
      case XLANGUAGE_ISO_639_3_CODE_BUL     : googlecode = __L("bg");     break;  //   Bulgarian
      case XLANGUAGE_ISO_639_3_CODE_BUR     :                             break;  //   Burmese
      case XLANGUAGE_ISO_639_3_CODE_CAT     : googlecode = __L("ca");     break;  //   Catalan
      case XLANGUAGE_ISO_639_3_CODE_CHA     :                             break;  //   Chamorro
      case XLANGUAGE_ISO_639_3_CODE_CHE     :                             break;  //   Chechen
      case XLANGUAGE_ISO_639_3_CODE_CHI     : googlecode = __L("zh-CN");  break;  //   Chinese    CHINESE_SIMPLIFIED  __L("zh-CN") CHINESE_TRADITIONAL __L("zh-TW")
      case XLANGUAGE_ISO_639_3_CODE_CHU     :                             break;  //   Church Slavic
      case XLANGUAGE_ISO_639_3_CODE_CHV     :                             break;  //   Chuvash
      case XLANGUAGE_ISO_639_3_CODE_COR     :                             break;  //   Cornish
      case XLANGUAGE_ISO_639_3_CODE_COS     :                             break;  //   Corsican
      case XLANGUAGE_ISO_639_3_CODE_CRE     :                             break;  //   Cree
      case XLANGUAGE_ISO_639_3_CODE_CZE     : googlecode = __L("cs");     break;  //   Czech
      case XLANGUAGE_ISO_639_3_CODE_DAN     : googlecode = __L("da");     break;  //   Danish
      case XLANGUAGE_ISO_639_3_CODE_DIV     :                             break;  //   Divehi
      case XLANGUAGE_ISO_639_3_CODE_DUT     : googlecode = __L("nl");     break;  //   Dutch
      case XLANGUAGE_ISO_639_3_CODE_DZO     :                             break;  //   Dzongkha
      case XLANGUAGE_ISO_639_3_CODE_ENG     : googlecode = __L("en");     break;  //   English
      case XLANGUAGE_ISO_639_3_CODE_EPO     : googlecode = __L("eo");     break;  //   Esperanto
      case XLANGUAGE_ISO_639_3_CODE_EST     : googlecode = __L("et");     break;  //   Estonian
      case XLANGUAGE_ISO_639_3_CODE_EWE     :                             break;  //   Ewe
      case XLANGUAGE_ISO_639_3_CODE_FAO     :                             break;  //   Faroese
      case XLANGUAGE_ISO_639_3_CODE_FIJ     :                             break;  //   Fijian
      case XLANGUAGE_ISO_639_3_CODE_FIN     : googlecode = __L("fi");     break;  //   Finnish
      case XLANGUAGE_ISO_639_3_CODE_FRE     : googlecode = __L("fr");     break;  //   French
      case XLANGUAGE_ISO_639_3_CODE_FRY     :                             break;  //   Western Frisian
      case XLANGUAGE_ISO_639_3_CODE_FUL     :                             break;  //   Fulah
      case XLANGUAGE_ISO_639_3_CODE_GEO     : googlecode = __L("ka");     break;  //   Georgian
      case XLANGUAGE_ISO_639_3_CODE_GER     : googlecode = __L("de");     break;  //   German
      case XLANGUAGE_ISO_639_3_CODE_GLA     :                             break;  //   Gaelic
      case XLANGUAGE_ISO_639_3_CODE_GLE     : googlecode = __L("ga");     break;  //   Irish
      case XLANGUAGE_ISO_639_3_CODE_GLG     : googlecode = __L("gl");     break;  //   Galician
      case XLANGUAGE_ISO_639_3_CODE_GLV     :                             break;  //   Manx
      case XLANGUAGE_ISO_639_3_CODE_GRE     : googlecode = __L("el");     break;  //   Greek Modern (1453-)
      case XLANGUAGE_ISO_639_3_CODE_GRN     :                             break;  //   Guarani
      case XLANGUAGE_ISO_639_3_CODE_GUJ     : googlecode = __L("gu");     break;  //   Gujarati
      case XLANGUAGE_ISO_639_3_CODE_HAT     : googlecode = __L("ht");     break;  //   Haitian
      case XLANGUAGE_ISO_639_3_CODE_HAU     :                             break;  //   Hausa
      case XLANGUAGE_ISO_639_3_CODE_HEB     : googlecode = __L("iw");     break;  //   Hebrew
      case XLANGUAGE_ISO_639_3_CODE_HER     :                             break;  //   Herero
      case XLANGUAGE_ISO_639_3_CODE_HIN     : googlecode = __L("hi");     break;  //   Hindi
      case XLANGUAGE_ISO_639_3_CODE_HMO     :                             break;  //   Hiri Motu
      case XLANGUAGE_ISO_639_3_CODE_HRV     : googlecode = __L("hr");     break;  //   Croatian
      case XLANGUAGE_ISO_639_3_CODE_HUN     : googlecode = __L("hu");     break;  //   Hungarian
      case XLANGUAGE_ISO_639_3_CODE_IBO     :                             break;  //   Igbo
      case XLANGUAGE_ISO_639_3_CODE_ICE     : googlecode = __L("is");     break;  //   Icelandic
      case XLANGUAGE_ISO_639_3_CODE_IDO     :                             break;  //   Ido
      case XLANGUAGE_ISO_639_3_CODE_III     :                             break;  //   Sichuan Yi
      case XLANGUAGE_ISO_639_3_CODE_IKU     :                             break;  //   Inuktitut
      case XLANGUAGE_ISO_639_3_CODE_ILE     :                             break;  //   Interlingue
      case XLANGUAGE_ISO_639_3_CODE_INA     :                             break;  //   Interlingua
      case XLANGUAGE_ISO_639_3_CODE_IND     : googlecode = __L("id");     break;  //   Indonesian
      case XLANGUAGE_ISO_639_3_CODE_IPK     :                             break;  //   Inupiaq
      case XLANGUAGE_ISO_639_3_CODE_ITA     : googlecode = __L("it");     break;  //   Italian
      case XLANGUAGE_ISO_639_3_CODE_JAV     :                             break;  //   Javanese
      case XLANGUAGE_ISO_639_3_CODE_JPN     : googlecode = __L("ja");     break;  //   Japanese
      case XLANGUAGE_ISO_639_3_CODE_KAL     :                             break;  //   Kalaallisut
      case XLANGUAGE_ISO_639_3_CODE_KAN     : googlecode = __L("kn");     break;  //   Kannada
      case XLANGUAGE_ISO_639_3_CODE_KAS     :                             break;  //   Kashmiri
      case XLANGUAGE_ISO_639_3_CODE_KAU     :                             break;  //   Kanuri
      case XLANGUAGE_ISO_639_3_CODE_KAZ     :                             break;  //   Kazakh
      case XLANGUAGE_ISO_639_3_CODE_KHM     :                             break;  //   Central Khmer
      case XLANGUAGE_ISO_639_3_CODE_KIK     :                             break;  //   Kikuyu
      case XLANGUAGE_ISO_639_3_CODE_KIN     :                             break;  //   Kinyarwanda
      case XLANGUAGE_ISO_639_3_CODE_KIR     :                             break;  //   Kirghiz
      case XLANGUAGE_ISO_639_3_CODE_KOM     :                             break;  //   Komi
      case XLANGUAGE_ISO_639_3_CODE_KON     :                             break;  //   Kongo
      case XLANGUAGE_ISO_639_3_CODE_KOR     : googlecode = __L("ko");     break;  //   Korean
      case XLANGUAGE_ISO_639_3_CODE_KUA     :                             break;  //   Kuanyama
      case XLANGUAGE_ISO_639_3_CODE_KUR     :                             break;  //   Kurdish
      case XLANGUAGE_ISO_639_3_CODE_LAO     :                             break;  //   Lao
      case XLANGUAGE_ISO_639_3_CODE_LAT     : googlecode = __L("la");     break;  //   Latin
      case XLANGUAGE_ISO_639_3_CODE_LAV     : googlecode = __L("lv");     break;  //   Latvian
      case XLANGUAGE_ISO_639_3_CODE_LIM     :                             break;  //   Limburgan
      case XLANGUAGE_ISO_639_3_CODE_LIN     :                             break;  //   Lingala
      case XLANGUAGE_ISO_639_3_CODE_LIT     : googlecode = __L("lt");     break;  //   Lithuanian
      case XLANGUAGE_ISO_639_3_CODE_LTZ     :                             break;  //   Luxembourgish
      case XLANGUAGE_ISO_639_3_CODE_LUB     :                             break;  //   Luba-Katanga
      case XLANGUAGE_ISO_639_3_CODE_LUG     :                             break;  //   Ganda
      case XLANGUAGE_ISO_639_3_CODE_MAC     : googlecode = __L("mk");     break;  //   Macedonian
      case XLANGUAGE_ISO_639_3_CODE_MAH     :                             break;  //   Marshallese
      case XLANGUAGE_ISO_639_3_CODE_MAL     :                             break;  //   Malayalam
      case XLANGUAGE_ISO_639_3_CODE_MAO     :                             break;  //   Maori
      case XLANGUAGE_ISO_639_3_CODE_MAR     :                             break;  //   Marathi
      case XLANGUAGE_ISO_639_3_CODE_MAY     : googlecode = __L("ms");     break;  //   Malay
      case XLANGUAGE_ISO_639_3_CODE_MLG     :                             break;  //   Malagasy
      case XLANGUAGE_ISO_639_3_CODE_MLT     : googlecode = __L("mt");     break;  //   Maltese
      case XLANGUAGE_ISO_639_3_CODE_MON     :                             break;  //   Mongolian
      case XLANGUAGE_ISO_639_3_CODE_NAU     :                             break;  //   Nauru
      case XLANGUAGE_ISO_639_3_CODE_NAV     :                             break;  //   Navajo
      case XLANGUAGE_ISO_639_3_CODE_NBL     :                             break;  //   Ndebele South
      case XLANGUAGE_ISO_639_3_CODE_NDE     :                             break;  //   Ndebele North
      case XLANGUAGE_ISO_639_3_CODE_NDO     :                             break;  //   Ndonga
      case XLANGUAGE_ISO_639_3_CODE_NEP     :                             break;  //   Nepali
      case XLANGUAGE_ISO_639_3_CODE_NNO     :                             break;  //   Norwegian Nynorsk
      case XLANGUAGE_ISO_639_3_CODE_NOB     :                             break;  //   Bokm_l Norwegian
      case XLANGUAGE_ISO_639_3_CODE_NOR     : googlecode = __L("no");     break;  //   Norwegian
      case XLANGUAGE_ISO_639_3_CODE_NYA     :                             break;  //   Chichewa
      case XLANGUAGE_ISO_639_3_CODE_OCI     :                             break;  //   Occitan (post 1500)
      case XLANGUAGE_ISO_639_3_CODE_OJI     :                             break;  //   Ojibwa
      case XLANGUAGE_ISO_639_3_CODE_ORI     :                             break;  //   Oriya
      case XLANGUAGE_ISO_639_3_CODE_ORM     :                             break;  //   Oromo
      case XLANGUAGE_ISO_639_3_CODE_OSS     :                             break;  //   Ossetian
      case XLANGUAGE_ISO_639_3_CODE_PAN     :                             break;  //   Panjabi
      case XLANGUAGE_ISO_639_3_CODE_PER     : googlecode = __L("fa");     break;  //   Persian
      case XLANGUAGE_ISO_639_3_CODE_PLI     :                             break;  //   Pali
      case XLANGUAGE_ISO_639_3_CODE_POL     : googlecode = __L("pl");     break;  //   Polish
      case XLANGUAGE_ISO_639_3_CODE_POR     : googlecode = __L("pt");     break;  //   Portuguese
      case XLANGUAGE_ISO_639_3_CODE_PUS     :                             break;  //   Pushto
      case XLANGUAGE_ISO_639_3_CODE_QUE     :                             break;  //   Quechua
      case XLANGUAGE_ISO_639_3_CODE_ROH     :                             break;  //   Romansh
      case XLANGUAGE_ISO_639_3_CODE_RUM     : googlecode = __L("ro");     break;  //   Romanian
      case XLANGUAGE_ISO_639_3_CODE_RUN     :                             break;  //   Rundi
      case XLANGUAGE_ISO_639_3_CODE_RUS     : googlecode = __L("ru");     break;  //   Russian
      case XLANGUAGE_ISO_639_3_CODE_SAG     :                             break;  //   Sango
      case XLANGUAGE_ISO_639_3_CODE_SAN     :                             break;  //   Sanskrit
      case XLANGUAGE_ISO_639_3_CODE_SIN     :                             break;  //   Sinhala
      case XLANGUAGE_ISO_639_3_CODE_SLO     : googlecode = __L("sk");     break;  //   Slovak
      case XLANGUAGE_ISO_639_3_CODE_SLV     : googlecode = __L("sl");     break;  //   Slovenian
      case XLANGUAGE_ISO_639_3_CODE_SME     :                             break;  //   Northern Sami
      case XLANGUAGE_ISO_639_3_CODE_SMO     :                             break;  //   Samoan
      case XLANGUAGE_ISO_639_3_CODE_SNA     :                             break;  //   Shona
      case XLANGUAGE_ISO_639_3_CODE_SND     :                             break;  //   Sindhi
      case XLANGUAGE_ISO_639_3_CODE_SOM     :                             break;  //   Somali
      case XLANGUAGE_ISO_639_3_CODE_SOT     :                             break;  //   Sotho Southern
      case XLANGUAGE_ISO_639_3_CODE_SPA     : googlecode = __L("es");     break;  //   Spanish
      case XLANGUAGE_ISO_639_3_CODE_SRD     :                             break;  //   Sardinian
      case XLANGUAGE_ISO_639_3_CODE_SRP     : googlecode = __L("sr");     break;  //   Serbian
      case XLANGUAGE_ISO_639_3_CODE_SSW     :                             break;  //   Swati
      case XLANGUAGE_ISO_639_3_CODE_SUN     :                             break;  //   Sundanese
      case XLANGUAGE_ISO_639_3_CODE_SWA     : googlecode = __L("sw");     break;  //   Swahili
      case XLANGUAGE_ISO_639_3_CODE_SWE     : googlecode = __L("sv");     break;  //   Swedish
      case XLANGUAGE_ISO_639_3_CODE_TAH     :                             break;  //   Tahitian
      case XLANGUAGE_ISO_639_3_CODE_TAM     : googlecode = __L("ta");     break;  //   Tamil
      case XLANGUAGE_ISO_639_3_CODE_TAT     :                             break;  //   Tatar
      case XLANGUAGE_ISO_639_3_CODE_TEL     : googlecode = __L("te");     break;  //   Telugu
      case XLANGUAGE_ISO_639_3_CODE_TGK     :                             break;  //   Tajik
      case XLANGUAGE_ISO_639_3_CODE_TGL     :                             break;  //   Tagalog
      case XLANGUAGE_ISO_639_3_CODE_THA     : googlecode = __L("th");     break;  //   Thai
      case XLANGUAGE_ISO_639_3_CODE_TIB     :                             break;  //   Tibetan
      case XLANGUAGE_ISO_639_3_CODE_TIR     :                             break;  //   Tigrinya
      case XLANGUAGE_ISO_639_3_CODE_TON     :                             break;  //   Tonga (Tonga Islands)
      case XLANGUAGE_ISO_639_3_CODE_TSN     :                             break;  //   Tswana
      case XLANGUAGE_ISO_639_3_CODE_TSO     :                             break;  //   Tsonga
      case XLANGUAGE_ISO_639_3_CODE_TUK     :                             break;  //   Turkmen
      case XLANGUAGE_ISO_639_3_CODE_TUR     : googlecode = __L("tr");     break;  //   Turkish
      case XLANGUAGE_ISO_639_3_CODE_TWI     :                             break;  //   Twi
      case XLANGUAGE_ISO_639_3_CODE_UIG     :                             break;  //   Uighur
      case XLANGUAGE_ISO_639_3_CODE_UKR     : googlecode = __L("uk");     break;  //   Ukrainian
      case XLANGUAGE_ISO_639_3_CODE_URD     : googlecode = __L("ur");     break;  //   Urdu
      case XLANGUAGE_ISO_639_3_CODE_UZB     :                             break;  //   Uzbek
      case XLANGUAGE_ISO_639_3_CODE_VEN     :                             break;  //   Venda
      case XLANGUAGE_ISO_639_3_CODE_VIE     : googlecode = __L("vi");     break;  //   Vietnamese
      case XLANGUAGE_ISO_639_3_CODE_VOL     :                             break;  //   Volap k
      case XLANGUAGE_ISO_639_3_CODE_WEL     : googlecode = __L("cy");     break;  //   Welsh
      case XLANGUAGE_ISO_639_3_CODE_WLN     :                             break;  //   Walloon
      case XLANGUAGE_ISO_639_3_CODE_WOL     :                             break;  //   Wolof
      case XLANGUAGE_ISO_639_3_CODE_XHO     :                             break;  //   Xhosa
      case XLANGUAGE_ISO_639_3_CODE_YID     : googlecode = __L("yi");     break;  //   Yiddish
      case XLANGUAGE_ISO_639_3_CODE_YOR     :                             break;  //   Yoruba
      case XLANGUAGE_ISO_639_3_CODE_ZHA     :                             break;  //   Zhuang
      case XLANGUAGE_ISO_639_3_CODE_ZUL     :                             break;  //   Zulu
    }

  return googlecode.IsEmpty()?false:true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSCRAPERWEBTRANSLATION::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSCRAPERWEBTRANSLATION::Clean()
{

}





