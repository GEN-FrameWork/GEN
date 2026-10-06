/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XLanguage_ISO_639_3.h
* 
* @class      XLANGUAGE_ISO_639_3
* @brief      eXtended Utils Language ISO 639-3 class
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

#include "XBase.h"
#include "XString.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum XLANGUAGE_ISO_639_3_CODE
{
  XLANGUAGE_ISO_639_3_CODE_INVALID   =   0,     //   Invalid
  XLANGUAGE_ISO_639_3_CODE_AAR       =   1,     //   Afar
  XLANGUAGE_ISO_639_3_CODE_ABK       =   2,     //   Abkhazian
  XLANGUAGE_ISO_639_3_CODE_AFR       =   3,     //   Afrikaans
  XLANGUAGE_ISO_639_3_CODE_AKA       =   4,     //   Akan
  XLANGUAGE_ISO_639_3_CODE_ALB       =   5,     //   Albanian
  XLANGUAGE_ISO_639_3_CODE_AMH       =   6,     //   Amharic
  XLANGUAGE_ISO_639_3_CODE_ARA       =   7,     //   Arabic
  XLANGUAGE_ISO_639_3_CODE_ARG       =   8,     //   Aragonese
  XLANGUAGE_ISO_639_3_CODE_ARM       =   9,     //   Armenia
  XLANGUAGE_ISO_639_3_CODE_ASM       =  10,     //   Assamese
  XLANGUAGE_ISO_639_3_CODE_AVA       =  11,     //   Avaric
  XLANGUAGE_ISO_639_3_CODE_AVE       =  12,     //   Avestan
  XLANGUAGE_ISO_639_3_CODE_AYM       =  13,     //   Aymara
  XLANGUAGE_ISO_639_3_CODE_AZE       =  14,     //   Azerbaijani
  XLANGUAGE_ISO_639_3_CODE_BAK       =  15,     //   Bashkir
  XLANGUAGE_ISO_639_3_CODE_BAM       =  16,     //   Bambara
  XLANGUAGE_ISO_639_3_CODE_BAQ       =  17,     //   Basque
  XLANGUAGE_ISO_639_3_CODE_BEL       =  18,     //   Belarusian
  XLANGUAGE_ISO_639_3_CODE_BEN       =  19,     //   Bengali
  XLANGUAGE_ISO_639_3_CODE_BIH       =  20,     //   Bihari languages
  XLANGUAGE_ISO_639_3_CODE_BIS       =  21,     //   Bislama
  XLANGUAGE_ISO_639_3_CODE_BOS       =  22,     //   Bosnian
  XLANGUAGE_ISO_639_3_CODE_BRE       =  23,     //   Breton
  XLANGUAGE_ISO_639_3_CODE_BUL       =  24,     //   Bulgarian
  XLANGUAGE_ISO_639_3_CODE_BUR       =  25,     //   Burmese
  XLANGUAGE_ISO_639_3_CODE_CAT       =  26,     //   Catalan
  XLANGUAGE_ISO_639_3_CODE_CHA       =  27,     //   Chamorro
  XLANGUAGE_ISO_639_3_CODE_CHE       =  28,     //   Chechen
  XLANGUAGE_ISO_639_3_CODE_CHI       =  29,     //   Chinese
  XLANGUAGE_ISO_639_3_CODE_CHU       =  30,     //   Church Slavic
  XLANGUAGE_ISO_639_3_CODE_CHV       =  31,     //   Chuvash
  XLANGUAGE_ISO_639_3_CODE_COR       =  32,     //   Cornish
  XLANGUAGE_ISO_639_3_CODE_COS       =  33,     //   Corsican
  XLANGUAGE_ISO_639_3_CODE_CRE       =  34,     //   Cree
  XLANGUAGE_ISO_639_3_CODE_CZE       =  35,     //   Czech
  XLANGUAGE_ISO_639_3_CODE_DAN       =  36,     //   Danish
  XLANGUAGE_ISO_639_3_CODE_DIV       =  37,     //   Divehi
  XLANGUAGE_ISO_639_3_CODE_DUT       =  38,     //   Dutch
  XLANGUAGE_ISO_639_3_CODE_DZO       =  39,     //   Dzongkha
  XLANGUAGE_ISO_639_3_CODE_ENG       =  40,     //   English
  XLANGUAGE_ISO_639_3_CODE_EPO       =  41,     //   Esperanto
  XLANGUAGE_ISO_639_3_CODE_EST       =  42,     //   Estonian
  XLANGUAGE_ISO_639_3_CODE_EWE       =  43,     //   Ewe
  XLANGUAGE_ISO_639_3_CODE_FAO       =  44,     //   Faroese
  XLANGUAGE_ISO_639_3_CODE_FIJ       =  45,     //   Fijian
  XLANGUAGE_ISO_639_3_CODE_FIN       =  46,     //   Finnish
  XLANGUAGE_ISO_639_3_CODE_FRE       =  47,     //   French
  XLANGUAGE_ISO_639_3_CODE_FRY       =  48,     //   Western Frisian
  XLANGUAGE_ISO_639_3_CODE_FUL       =  49,     //   Fulah
  XLANGUAGE_ISO_639_3_CODE_GEO       =  50,     //   Georgian
  XLANGUAGE_ISO_639_3_CODE_GER       =  51,     //   German
  XLANGUAGE_ISO_639_3_CODE_GLA       =  52,     //   Gaelic
  XLANGUAGE_ISO_639_3_CODE_GLE       =  53,     //   Irish
  XLANGUAGE_ISO_639_3_CODE_GLG       =  54,     //   Galician
  XLANGUAGE_ISO_639_3_CODE_GLV       =  55,     //   Manx
  XLANGUAGE_ISO_639_3_CODE_GRE       =  56,     //   Greek Modern (1453-)
  XLANGUAGE_ISO_639_3_CODE_GRN       =  57,     //   Guarani
  XLANGUAGE_ISO_639_3_CODE_GUJ       =  58,     //   Gujarati
  XLANGUAGE_ISO_639_3_CODE_HAT       =  59,     //   Haitian
  XLANGUAGE_ISO_639_3_CODE_HAU       =  60,     //   Hausa
  XLANGUAGE_ISO_639_3_CODE_HEB       =  61,     //   Hebrew
  XLANGUAGE_ISO_639_3_CODE_HER       =  62,     //   Herero
  XLANGUAGE_ISO_639_3_CODE_HIN       =  63,     //   Hindi
  XLANGUAGE_ISO_639_3_CODE_HMO       =  64,     //   Hiri Motu
  XLANGUAGE_ISO_639_3_CODE_HRV       =  65,     //   Croatian
  XLANGUAGE_ISO_639_3_CODE_HUN       =  66,     //   Hungarian
  XLANGUAGE_ISO_639_3_CODE_IBO       =  67,     //   Igbo
  XLANGUAGE_ISO_639_3_CODE_ICE       =  68,     //   Icelandic
  XLANGUAGE_ISO_639_3_CODE_IDO       =  69,     //   Ido
  XLANGUAGE_ISO_639_3_CODE_III       =  70,     //   Sichuan Yi
  XLANGUAGE_ISO_639_3_CODE_IKU       =  72,     //   Inuktitut
  XLANGUAGE_ISO_639_3_CODE_ILE       =  73,     //   Interlingue
  XLANGUAGE_ISO_639_3_CODE_INA       =  74,     //   Interlingua
  XLANGUAGE_ISO_639_3_CODE_IND       =  75,     //   Indonesian
  XLANGUAGE_ISO_639_3_CODE_IPK       =  76,     //   Inupiaq
  XLANGUAGE_ISO_639_3_CODE_ITA       =  77,     //   Italian
  XLANGUAGE_ISO_639_3_CODE_JAV       =  78,     //   Javanese
  XLANGUAGE_ISO_639_3_CODE_JPN       =  79,     //   Japanese
  XLANGUAGE_ISO_639_3_CODE_KAL       =  80,     //   Kalaallisut
  XLANGUAGE_ISO_639_3_CODE_KAN       =  81,     //   Kannada
  XLANGUAGE_ISO_639_3_CODE_KAS       =  82,     //   Kashmiri
  XLANGUAGE_ISO_639_3_CODE_KAU       =  83,     //   Kanuri
  XLANGUAGE_ISO_639_3_CODE_KAZ       =  84,     //   Kazakh
  XLANGUAGE_ISO_639_3_CODE_KHM       =  85,     //   Central Khmer
  XLANGUAGE_ISO_639_3_CODE_KIK       =  86,     //   Kikuyu
  XLANGUAGE_ISO_639_3_CODE_KIN       =  87,     //   Kinyarwanda
  XLANGUAGE_ISO_639_3_CODE_KIR       =  88,     //   Kirghiz
  XLANGUAGE_ISO_639_3_CODE_KOM       =  89,     //   Komi
  XLANGUAGE_ISO_639_3_CODE_KON       =  90,     //   Kongo
  XLANGUAGE_ISO_639_3_CODE_KOR       =  91,     //   Korean
  XLANGUAGE_ISO_639_3_CODE_KUA       =  92,     //   Kuanyama
  XLANGUAGE_ISO_639_3_CODE_KUR       =  93,     //   Kurdish
  XLANGUAGE_ISO_639_3_CODE_LAO       =  94,     //   Lao
  XLANGUAGE_ISO_639_3_CODE_LAT       =  95,     //   Latin
  XLANGUAGE_ISO_639_3_CODE_LAV       =  96,     //   Latvian
  XLANGUAGE_ISO_639_3_CODE_LIM       =  97,     //   Limburgan
  XLANGUAGE_ISO_639_3_CODE_LIN       =  98,     //   Lingala
  XLANGUAGE_ISO_639_3_CODE_LIT       =  99,     //   Lithuanian
  XLANGUAGE_ISO_639_3_CODE_LTZ       = 100,     //   Luxembourgish
  XLANGUAGE_ISO_639_3_CODE_LUB       = 101,     //   Luba-Katanga
  XLANGUAGE_ISO_639_3_CODE_LUG       = 102,     //   Ganda
  XLANGUAGE_ISO_639_3_CODE_MAC       = 103,     //   Macedonian
  XLANGUAGE_ISO_639_3_CODE_MAH       = 104,     //   Marshallese
  XLANGUAGE_ISO_639_3_CODE_MAL       = 105,     //   Malayalam
  XLANGUAGE_ISO_639_3_CODE_MAO       = 106,     //   Maori
  XLANGUAGE_ISO_639_3_CODE_MAR       = 107,     //   Marathi
  XLANGUAGE_ISO_639_3_CODE_MAY       = 108,     //   Malay
  XLANGUAGE_ISO_639_3_CODE_MLG       = 109,     //   Malagasy
  XLANGUAGE_ISO_639_3_CODE_MLT       = 110,     //   Maltese
  XLANGUAGE_ISO_639_3_CODE_MON       = 111,     //   Mongolian
  XLANGUAGE_ISO_639_3_CODE_NAU       = 112,     //   Nauru
  XLANGUAGE_ISO_639_3_CODE_NAV       = 113,     //   Navajo
  XLANGUAGE_ISO_639_3_CODE_NBL       = 114,     //   Ndebele South
  XLANGUAGE_ISO_639_3_CODE_NDE       = 115,     //   Ndebele North
  XLANGUAGE_ISO_639_3_CODE_NDO       = 116,     //   Ndonga
  XLANGUAGE_ISO_639_3_CODE_NEP       = 117,     //   Nepali
  XLANGUAGE_ISO_639_3_CODE_NNO       = 118,     //   Norwegian Nynorsk
  XLANGUAGE_ISO_639_3_CODE_NOB       = 119,     //   Bokm_l Norwegian
  XLANGUAGE_ISO_639_3_CODE_NOR       = 120,     //   Norwegian
  XLANGUAGE_ISO_639_3_CODE_NYA       = 121,     //   Chichewa
  XLANGUAGE_ISO_639_3_CODE_OCI       = 122,     //   Occitan (post 1500)
  XLANGUAGE_ISO_639_3_CODE_OJI       = 123,     //   Ojibwa
  XLANGUAGE_ISO_639_3_CODE_ORI       = 124,     //   Oriya
  XLANGUAGE_ISO_639_3_CODE_ORM       = 125,     //   Oromo
  XLANGUAGE_ISO_639_3_CODE_OSS       = 126,     //   Ossetian
  XLANGUAGE_ISO_639_3_CODE_PAN       = 127,     //   Panjabi
  XLANGUAGE_ISO_639_3_CODE_PER       = 128,     //   Persian
  XLANGUAGE_ISO_639_3_CODE_PLI       = 129,     //   Pali
  XLANGUAGE_ISO_639_3_CODE_POL       = 130,     //   Polish
  XLANGUAGE_ISO_639_3_CODE_POR       = 131,     //   Portuguese
  XLANGUAGE_ISO_639_3_CODE_PUS       = 132,     //   Pushto
  XLANGUAGE_ISO_639_3_CODE_QUE       = 133,     //   Quechua
  XLANGUAGE_ISO_639_3_CODE_ROH       = 134,     //   Romansh
  XLANGUAGE_ISO_639_3_CODE_RUM       = 135,     //   Romanian
  XLANGUAGE_ISO_639_3_CODE_RUN       = 136,     //   Rundi
  XLANGUAGE_ISO_639_3_CODE_RUS       = 137,     //   Russian
  XLANGUAGE_ISO_639_3_CODE_SAG       = 138,     //   Sango
  XLANGUAGE_ISO_639_3_CODE_SAN       = 139,     //   Sanskrit
  XLANGUAGE_ISO_639_3_CODE_SIN       = 140,     //   Sinhala
  XLANGUAGE_ISO_639_3_CODE_SLO       = 141,     //   Slovak
  XLANGUAGE_ISO_639_3_CODE_SLV       = 142,     //   Slovenian
  XLANGUAGE_ISO_639_3_CODE_SME       = 143,     //   Northern Sami
  XLANGUAGE_ISO_639_3_CODE_SMO       = 144,     //   Samoan
  XLANGUAGE_ISO_639_3_CODE_SNA       = 145,     //   Shona
  XLANGUAGE_ISO_639_3_CODE_SND       = 146,     //   Sindhi
  XLANGUAGE_ISO_639_3_CODE_SOM       = 147,     //   Somali
  XLANGUAGE_ISO_639_3_CODE_SOT       = 148,     //   Sotho Southern
  XLANGUAGE_ISO_639_3_CODE_SPA       = 149,     //   Spanish
  XLANGUAGE_ISO_639_3_CODE_SRD       = 150,     //   Sardinian
  XLANGUAGE_ISO_639_3_CODE_SRP       = 151,     //   Serbian
  XLANGUAGE_ISO_639_3_CODE_SSW       = 152,     //   Swati
  XLANGUAGE_ISO_639_3_CODE_SUN       = 153,     //   Sundanese
  XLANGUAGE_ISO_639_3_CODE_SWA       = 154,     //   Swahili
  XLANGUAGE_ISO_639_3_CODE_SWE       = 155,     //   Swedish
  XLANGUAGE_ISO_639_3_CODE_TAH       = 156,     //   Tahitian
  XLANGUAGE_ISO_639_3_CODE_TAM       = 157,     //   Tamil
  XLANGUAGE_ISO_639_3_CODE_TAT       = 158,     //   Tatar
  XLANGUAGE_ISO_639_3_CODE_TEL       = 159,     //   Telugu
  XLANGUAGE_ISO_639_3_CODE_TGK       = 160,     //   Tajik
  XLANGUAGE_ISO_639_3_CODE_TGL       = 161,     //   Tagalog
  XLANGUAGE_ISO_639_3_CODE_THA       = 162,     //   Thai
  XLANGUAGE_ISO_639_3_CODE_TIB       = 163,     //   Tibetan
  XLANGUAGE_ISO_639_3_CODE_TIR       = 164,     //   Tigrinya
  XLANGUAGE_ISO_639_3_CODE_TON       = 165,     //   Tonga (Tonga Islands)
  XLANGUAGE_ISO_639_3_CODE_TSN       = 166,     //   Tswana
  XLANGUAGE_ISO_639_3_CODE_TSO       = 167,     //   Tsonga
  XLANGUAGE_ISO_639_3_CODE_TUK       = 168,     //   Turkmen
  XLANGUAGE_ISO_639_3_CODE_TUR       = 169,     //   Turkish
  XLANGUAGE_ISO_639_3_CODE_TWI       = 170,     //   Twi
  XLANGUAGE_ISO_639_3_CODE_UIG       = 171,     //   Uighur
  XLANGUAGE_ISO_639_3_CODE_UKR       = 172,     //   Ukrainian
  XLANGUAGE_ISO_639_3_CODE_URD       = 173,     //   Urdu
  XLANGUAGE_ISO_639_3_CODE_UZB       = 174,     //   Uzbek
  XLANGUAGE_ISO_639_3_CODE_VEN       = 175,     //   Venda
  XLANGUAGE_ISO_639_3_CODE_VIE       = 176,     //   Vietnamese
  XLANGUAGE_ISO_639_3_CODE_VOL       = 177,     //   Volap_k
  XLANGUAGE_ISO_639_3_CODE_WEL       = 178,     //   Welsh
  XLANGUAGE_ISO_639_3_CODE_WLN       = 179,     //   Walloon
  XLANGUAGE_ISO_639_3_CODE_WOL       = 180,     //   Wolof
  XLANGUAGE_ISO_639_3_CODE_XHO       = 181,     //   Xhosa
  XLANGUAGE_ISO_639_3_CODE_YID       = 182,     //   Yiddish
  XLANGUAGE_ISO_639_3_CODE_YOR       = 183,     //   Yoruba
  XLANGUAGE_ISO_639_3_CODE_ZHA       = 184,     //   Zhuang
  XLANGUAGE_ISO_639_3_CODE_ZUL       = 185,     //   Zulu
};

typedef struct
{
  XDWORD        code;
  XCHAR*        codealpha3;
  XCHAR*        codealpha2;
  XCHAR*        englishname;
  XCHAR*        alias1name;
  XCHAR*        alias2name;

} XLANGUAGE_ISO_639_3_ENTRY;

#define XLANGUAGE_ISO_639_3_NENTRYS         sizeof(iso_639_3_entry)/sizeof(XLANGUAGE_ISO_639_3_ENTRY)

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/


class XLANGUAGE_ISO_639_3
{
  public:
                      XLANGUAGE_ISO_639_3             ();
    virtual          ~XLANGUAGE_ISO_639_3             ();

    XDWORD            Code_GetByCodeAlpha3            (XCHAR* codealpha3);
    XDWORD            Code_GetByCodeAlpha2            (XCHAR* codealpha2);
    XDWORD            Code_GetByEnglishName           (XCHAR* englishname);
    XDWORD            Code_GetByAlias                 (XCHAR* alias);

    bool              CodeAlpha3_GetByCode            (XDWORD code, XSTRING& codealpha3);
    bool              CodeAlpha3_GetByCodeAlpha2      (XCHAR* codealpha2, XSTRING& codealpha3);
    bool              CodeAlpha3_GetByEnglishName     (XCHAR* englishname, XSTRING& codealpha3);
    bool              CodeAlpha3_GetByAlias           (XCHAR* alias, XSTRING& codealpha3);

    bool              CodeAlpha2_GetByCode            (XDWORD code, XSTRING& codealpha2);
    bool              CodeAlpha2_GetByCodeAlpha3      (XCHAR* codealpha3, XSTRING& codealpha2);
    bool              CodeAlpha2_Resolve              (XCHAR* languageid, XSTRING& codealpha2);

    bool              EnglishName_GetByCode           (XDWORD code, XSTRING& englishname);
    bool              EnglishName_GetByCodeAlpha3     (XCHAR* codealpha2, XSTRING& englishname);
    bool              EnglishName_GetByCodeAlpha2     (XCHAR* codealpha2, XSTRING& englishname);
    bool              EnglishName_GetByAlias          (XCHAR* alias, XSTRING& englishname);

  private:

    void              Clean                           ();
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





