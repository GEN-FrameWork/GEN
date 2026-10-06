/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XLanguage_ISO_639_3.cpp
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XLanguage_ISO_639_3.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/


XLANGUAGE_ISO_639_3_ENTRY  iso_639_3_entry[] = {   {  XLANGUAGE_ISO_639_3_CODE_ENG,  _L("eng")   ,  _L("en")   ,  _L("English")               , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_SPA,  _L("spa")   ,  _L("es")   ,  _L("Spanish")               , _L("Castilian")          , _L("")                       },   

                                                   //#ifndef MICROCONTROLLER
                                                   {  XLANGUAGE_ISO_639_3_CODE_AAR,  _L("aar")   ,  _L("aa")   ,  _L("Afar")                  , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_ABK,  _L("abk")   ,  _L("ab")   ,  _L("Abkhazian")             , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_AFR,  _L("afr")   ,  _L("af")   ,  _L("Afrikaans")             , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_AKA,  _L("aka")   ,  _L("ak")   ,  _L("Akan")                  , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_ALB,  _L("alb")   ,  _L("sq")   ,  _L("Albanian")              , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_AMH,  _L("amh")   ,  _L("am")   ,  _L("Amharic")               , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_ARA,  _L("ara")   ,  _L("ar")   ,  _L("Arabic")                , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_ARG,  _L("arg")   ,  _L("an")   ,  _L("Aragonese")             , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_ARM,  _L("ARM")   ,  _L("hy")   ,  _L("Armenia")               , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_ASM,  _L("asm")   ,  _L("as")   ,  _L("Assamese")              , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_AVA,  _L("ava")   ,  _L("av")   ,  _L("Avaric")                , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_AVE,  _L("ave")   ,  _L("ae")   ,  _L("Avestan")               , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_AYM,  _L("aym")   ,  _L("ay")   ,  _L("Aymara")                , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_AZE,  _L("aze")   ,  _L("az")   ,  _L("Azerbaijani")           , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_BAK,  _L("bak")   ,  _L("ba")   ,  _L("Bashkir")               , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_BAM,  _L("bam")   ,  _L("bm")   ,  _L("Bambara")               , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_BAQ,  _L("baq")   ,  _L("eu")   ,  _L("Basque")                , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_BEL,  _L("bel")   ,  _L("be")   ,  _L("Belarusian")            , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_BEN,  _L("ben")   ,  _L("bn")   ,  _L("Bengali")               , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_BIH,  _L("bih")   ,  _L("bh")   ,  _L("Bihari languages")      , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_BIS,  _L("bis")   ,  _L("bi")   ,  _L("Bislama")               , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_BOS,  _L("bos")   ,  _L("bs")   ,  _L("Bosnian")               , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_BRE,  _L("bre")   ,  _L("br")   ,  _L("Breton")                , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_BUL,  _L("bul")   ,  _L("bg")   ,  _L("Bulgarian")             , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_BUR,  _L("bur")   ,  _L("my")   ,  _L("Burmese")               , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_CAT,  _L("cat")   ,  _L("ca")   ,  _L("Catalan")               , _L("Valencian")          , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_CHA,  _L("cha")   ,  _L("ch")   ,  _L("Chamorro")              , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_CHE,  _L("che")   ,  _L("ce")   ,  _L("Chechen")               , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_CHI,  _L("chi")   ,  _L("zh")   ,  _L("Chinese")               , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_CHU,  _L("chu")   ,  _L("cu")   ,  _L("Church Slavic")         , _L("Old Slavonic")       , _L("Church Slavonic")        },
                                                   {  XLANGUAGE_ISO_639_3_CODE_CHV,  _L("chv")   ,  _L("cv")   ,  _L("Chuvash")               , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_COR,  _L("cor")   ,  _L("kw")   ,  _L("Cornish")               , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_COS,  _L("cos")   ,  _L("co")   ,  _L("Corsican")              , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_CRE,  _L("cre")   ,  _L("cr")   ,  _L("Cree")                  , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_CZE,  _L("cze")   ,  _L("cs")   ,  _L("Czech")                 , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_DAN,  _L("dan")   ,  _L("da")   ,  _L("Danish")                , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_DIV,  _L("div")   ,  _L("dv")   ,  _L("Divehi")                , _L("Dhivehi")            , _L("Maldivian")              },
                                                   {  XLANGUAGE_ISO_639_3_CODE_DUT,  _L("dut")   ,  _L("nl")   ,  _L("Dutch")                 , _L("Flemish")            , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_DZO,  _L("dzo")   ,  _L("dz")   ,  _L("Dzongkha")              , _L("")                   , _L("")                       },                                                   
                                                   {  XLANGUAGE_ISO_639_3_CODE_EPO,  _L("epo")   ,  _L("eo")   ,  _L("Esperanto")             , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_EST,  _L("est")   ,  _L("et")   ,  _L("Estonian")              , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_EWE,  _L("ewe")   ,  _L("ee")   ,  _L("Ewe")                   , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_FAO,  _L("fao")   ,  _L("fo")   ,  _L("Faroese")               , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_FIJ,  _L("fij")   ,  _L("fj")   ,  _L("Fijian")                , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_FIN,  _L("fin")   ,  _L("fi")   ,  _L("Finnish")               , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_FRE,  _L("fre")   ,  _L("fr")   ,  _L("French")                , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_FRY,  _L("fry")   ,  _L("fy")   ,  _L("Western Frisian")       , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_FUL,  _L("ful")   ,  _L("ff")   ,  _L("Fulah")                 , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_GEO,  _L("geo")   ,  _L("ka")   ,  _L("Georgian")              , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_GER,  _L("ger")   ,  _L("de")   ,  _L("German")                , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_GLA,  _L("gla")   ,  _L("gd")   ,  _L("Gaelic")                ,  _L("Scottish Gaelic")   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_GLE,  _L("gle")   ,  _L("ga")   ,  _L("Irish")                 , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_GLG,  _L("glg")   ,  _L("gl")   ,  _L("Galician")              , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_GLV,  _L("glv")   ,  _L("gv")   ,  _L("Manx")                  , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_GRE,  _L("gre")   ,  _L("el")   ,  _L("Greek Modern (1453-)")  , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_GRN,  _L("grn")   ,  _L("gn")   ,  _L("Guarani")               , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_GUJ,  _L("guj")   ,  _L("gu")   ,  _L("Gujarati")              , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_HAT,  _L("hat")   ,  _L("ht")   ,  _L("Haitian")               ,  _L("Haitian Creole")    , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_HAU,  _L("hau")   ,  _L("ha")   ,  _L("Hausa")                 , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_HEB,  _L("heb")   ,  _L("he")   ,  _L("Hebrew")                , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_HER,  _L("her")   ,  _L("hz")   ,  _L("Herero")                , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_HIN,  _L("hin")   ,  _L("hi")   ,  _L("Hindi")                 , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_HMO,  _L("hmo")   ,  _L("ho")   ,  _L("Hiri Motu")             , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_HRV,  _L("hrv")   ,  _L("hr")   ,  _L("Croatian")              , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_HUN,  _L("hun")   ,  _L("hu")   ,  _L("Hungarian")             , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_IBO,  _L("ibo")   ,  _L("ig")   ,  _L("Igbo")                  , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_ICE,  _L("ice")   ,  _L("is")   ,  _L("Icelandic")             , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_IDO,  _L("ido")   ,  _L("io")   ,  _L("Ido")                   , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_III,  _L("iii")   ,  _L("ii")   ,  _L("Sichuan Yi")            ,  _L("Nuosu")             , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_IKU,  _L("iku")   ,  _L("iu")   ,  _L("Inuktitut")             , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_ILE,  _L("ile")   ,  _L("ie")   ,  _L("Interlingue")           ,  _L("Occidental")        , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_INA,  _L("ina")   ,  _L("ia")   ,  _L("Interlingua")           , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_IND,  _L("ind")   ,  _L("id")   ,  _L("Indonesian")            , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_IPK,  _L("ipk")   ,  _L("ik")   ,  _L("Inupiaq")               , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_ITA,  _L("ita")   ,  _L("it")   ,  _L("Italian")               , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_JAV,  _L("jav")   ,  _L("jv")   ,  _L("Javanese")              , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_JPN,  _L("jpn")   ,  _L("ja")   ,  _L("Japanese")              , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_KAL,  _L("kal")   ,  _L("kl")   ,  _L("Kalaallisut")           ,  _L("Greenlandic")       , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_KAN,  _L("kan")   ,  _L("kn")   ,  _L("Kannada")               , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_KAS,  _L("kas")   ,  _L("ks")   ,  _L("Kashmiri")              , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_KAU,  _L("kau")   ,  _L("kr")   ,  _L("Kanuri")                , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_KAZ,  _L("kaz")   ,  _L("kk")   ,  _L("Kazakh")                , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_KHM,  _L("khm")   ,  _L("km")   ,  _L("Central Khmer")         , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_KIK,  _L("kik")   ,  _L("ki")   ,  _L("Kikuyu")                ,  _L("Gikuyu")            , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_KIN,  _L("kin")   ,  _L("rw")   ,  _L("Kinyarwanda")           , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_KIR,  _L("kir")   ,  _L("ky")   ,  _L("Kirghiz")               ,  _L("Kyrgyz")            , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_KOM,  _L("kom")   ,  _L("kv")   ,  _L("Komi")                  , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_KON,  _L("kon")   ,  _L("kg")   ,  _L("Kongo")                 , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_KOR,  _L("kor")   ,  _L("ko")   ,  _L("Korean")                , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_KUA,  _L("kua")   ,  _L("kj")   ,  _L("Kuanyama")              ,  _L("Kwanyama")          , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_KUR,  _L("kur")   ,  _L("ku")   ,  _L("Kurdish")               , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_LAO,  _L("lao")   ,  _L("lo")   ,  _L("Lao")                   , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_LAT,  _L("lat")   ,  _L("la")   ,  _L("Latin")                 , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_LAV,  _L("lav")   ,  _L("lv")   ,  _L("Latvian")               , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_LIM,  _L("lim")   ,  _L("li")   ,  _L("Limburgan")             ,  _L("Limburger")         , _L("Limburgish")             },
                                                   {  XLANGUAGE_ISO_639_3_CODE_LIN,  _L("lin")   ,  _L("ln")   ,  _L("Lingala")               , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_LIT,  _L("lit")   ,  _L("lt")   ,  _L("Lithuanian")            , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_LTZ,  _L("ltz")   ,  _L("lb")   ,  _L("Luxembourgish")         ,  _L("Letzeburgesch")     , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_LUB,  _L("lub")   ,  _L("lu")   ,  _L("Luba-Katanga")          , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_LUG,  _L("lug")   ,  _L("lg")   ,  _L("Ganda")                 , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_MAC,  _L("mac")   ,  _L("mk")   ,  _L("Macedonian")            , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_MAH,  _L("mah")   ,  _L("mh")   ,  _L("Marshallese")           , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_MAL,  _L("mal")   ,  _L("ml")   ,  _L("Malayalam")             , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_MAO,  _L("mao")   ,  _L("mi")   ,  _L("Maori")                 , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_MAR,  _L("mar")   ,  _L("mr")   ,  _L("Marathi")               , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_MAY,  _L("may")   ,  _L("ms")   ,  _L("Malay")                 , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_MLG,  _L("mlg")   ,  _L("mg")   ,  _L("Malagasy")              , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_MLT,  _L("mlt")   ,  _L("mt")   ,  _L("Maltese")               , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_MON,  _L("mon")   ,  _L("mn")   ,  _L("Mongolian")             , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_NAU,  _L("nau")   ,  _L("na")   ,  _L("Nauru")                 , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_NAV,  _L("nav")   ,  _L("nv")   ,  _L("Navajo")                , _L("Navaho")             , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_NBL,  _L("nbl")   ,  _L("nr")   ,  _L("Ndebele South")         , _L("South Ndebele")      , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_NDE,  _L("nde")   ,  _L("nd")   ,  _L("Ndebele North")         , _L("North Ndebele")      , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_NDO,  _L("ndo")   ,  _L("ng")   ,  _L("Ndonga")                , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_NEP,  _L("nep")   ,  _L("ne")   ,  _L("Nepali")                , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_NNO,  _L("nno")   ,  _L("nn")   ,  _L("Norwegian Nynorsk")     , _L("Nynorsk Norwegian")  , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_NOB,  _L("nob")   ,  _L("nb")   ,  _L("Bokm_l Norwegian")     , _L("Norwegian Bokm_l")  , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_NOR,  _L("nor")   ,  _L("no")   ,  _L("Norwegian")             , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_NYA,  _L("nya")   ,  _L("ny")   ,  _L("Chichewa")              , _L("Chewa")              ,  _L("Nyanja")                },
                                                   {  XLANGUAGE_ISO_639_3_CODE_OCI,  _L("oci")   ,  _L("oc")   ,  _L("Occitan (post 1500)")   , _L("Proven_al")         , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_OJI,  _L("oji")   ,  _L("oj")   ,  _L("Ojibwa")                , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_ORI,  _L("ori")   ,  _L("or")   ,  _L("Oriya")                 , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_ORM,  _L("orm")   ,  _L("om")   ,  _L("Oromo")                 , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_OSS,  _L("oss")   ,  _L("os")   ,  _L("Ossetian")              , _L("Ossetic")            , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_PAN,  _L("pan")   ,  _L("pa")   ,  _L("Panjabi")               , _L("Punjabi")            , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_PER,  _L("per")   ,  _L("fa")   ,  _L("Persian")               , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_PLI,  _L("pli")   ,  _L("pi")   ,  _L("Pali")                  , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_POL,  _L("pol")   ,  _L("pl")   ,  _L("Polish")                , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_POR,  _L("por")   ,  _L("pt")   ,  _L("Portuguese")            , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_PUS,  _L("pus")   ,  _L("ps")   ,  _L("Pushto")                , _L("Pashto")             , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_QUE,  _L("que")   ,  _L("qu")   ,  _L("Quechua")               , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_ROH,  _L("roh")   ,  _L("rm")   ,  _L("Romansh")               , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_RUM,  _L("rum")   ,  _L("ro")   ,  _L("Romanian")              , _L("Moldavian")          , _L("Moldovan")               },
                                                   {  XLANGUAGE_ISO_639_3_CODE_RUN,  _L("run")   ,  _L("rn")   ,  _L("Rundi")                 , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_RUS,  _L("rus")   ,  _L("ru")   ,  _L("Russian")               , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_SAG,  _L("sag")   ,  _L("sg")   ,  _L("Sango")                 , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_SAN,  _L("san")   ,  _L("sa")   ,  _L("Sanskrit")              , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_SIN,  _L("sin")   ,  _L("si")   ,  _L("Sinhala")               , _L("Sinhalese")          , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_SLO,  _L("slo")   ,  _L("sk")   ,  _L("Slovak")                , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_SLV,  _L("slv")   ,  _L("sl")   ,  _L("Slovenian")             , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_SME,  _L("sme")   ,  _L("se")   ,  _L("Northern Sami")         , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_SMO,  _L("smo")   ,  _L("sm")   ,  _L("Samoan")                , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_SNA,  _L("sna")   ,  _L("sn")   ,  _L("Shona")                 , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_SND,  _L("snd")   ,  _L("sd")   ,  _L("Sindhi")                , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_SOM,  _L("som")   ,  _L("so")   ,  _L("Somali")                , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_SOT,  _L("sot")   ,  _L("st")   ,  _L("Sotho Southern")        , _L("")                   , _L("")                       },                                                   
                                                   {  XLANGUAGE_ISO_639_3_CODE_SRD,  _L("srd")   ,  _L("sc")   ,  _L("Sardinian")             , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_SRP,  _L("srp")   ,  _L("sr")   ,  _L("Serbian")               , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_SSW,  _L("ssw")   ,  _L("ss")   ,  _L("Swati")                 , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_SUN,  _L("sun")   ,  _L("su")   ,  _L("Sundanese")             , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_SWA,  _L("swa")   ,  _L("sw")   ,  _L("Swahili")               , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_SWE,  _L("swe")   ,  _L("sv")   ,  _L("Swedish")               , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_TAH,  _L("tah")   ,  _L("ty")   ,  _L("Tahitian")              , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_TAM,  _L("tam")   ,  _L("ta")   ,  _L("Tamil")                 , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_TAT,  _L("tat")   ,  _L("tt")   ,  _L("Tatar")                 , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_TEL,  _L("tel")   ,  _L("te")   ,  _L("Telugu")                , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_TGK,  _L("tgk")   ,  _L("tg")   ,  _L("Tajik")                 , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_TGL,  _L("tgl")   ,  _L("tl")   ,  _L("Tagalog")               , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_THA,  _L("tha")   ,  _L("th")   ,  _L("Thai")                  , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_TIB,  _L("tib")   ,  _L("bo")   ,  _L("Tibetan")               , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_TIR,  _L("tir")   ,  _L("ti")   ,  _L("Tigrinya")              , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_TON,  _L("ton")   ,  _L("to")   ,  _L("Tonga (Tonga Islands)") , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_TSN,  _L("tsn")   ,  _L("tn")   ,  _L("Tswana")                , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_TSO,  _L("tso")   ,  _L("ts")   ,  _L("Tsonga")                , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_TUK,  _L("tuk")   ,  _L("tk")   ,  _L("Turkmen")               , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_TUR,  _L("tur")   ,  _L("tr")   ,  _L("Turkish")               , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_TWI,  _L("twi")   ,  _L("tw")   ,  _L("Twi")                   , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_UIG,  _L("uig")   ,  _L("ug")   ,  _L("Uighur")                , _L("Uyghur")             , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_UKR,  _L("ukr")   ,  _L("uk")   ,  _L("Ukrainian")             , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_URD,  _L("urd")   ,  _L("ur")   ,  _L("Urdu")                  , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_UZB,  _L("uzb")   ,  _L("uz")   ,  _L("Uzbek")                 , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_VEN,  _L("ven")   ,  _L("ve")   ,  _L("Venda")                 , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_VIE,  _L("vie")   ,  _L("vi")   ,  _L("Vietnamese")            , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_VOL,  _L("vol")   ,  _L("vo")   ,  _L("Volap_k")              , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_WEL,  _L("wel")   ,  _L("cy")   ,  _L("Welsh")                 , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_WLN,  _L("wln")   ,  _L("wa")   ,  _L("Walloon")               , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_WOL,  _L("wol")   ,  _L("wo")   ,  _L("Wolof")                 , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_XHO,  _L("xho")   ,  _L("xh")   ,  _L("Xhosa")                 , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_YID,  _L("yid")   ,  _L("yi")   ,  _L("Yiddish")               , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_YOR,  _L("yor")   ,  _L("yo")   ,  _L("Yoruba")                , _L("")                   , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_ZHA,  _L("zha")   ,  _L("za")   ,  _L("Zhuang")                , _L("Chuang")             , _L("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_ZUL,  _L("zul")   ,  _L("zu")   ,  _L("Zulu")                  , _L("")                   , _L("")                       }
                                                   //#endif
                                               };



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XLANGUAGE_ISO_639_3::XLANGUAGE_ISO_639_3()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XLANGUAGE_ISO_639_3::XLANGUAGE_ISO_639_3()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XLANGUAGE_ISO_639_3::~XLANGUAGE_ISO_639_3()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XLANGUAGE_ISO_639_3::~XLANGUAGE_ISO_639_3()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XLANGUAGE_ISO_639_3::Code_GetByCodeAlpha3(XCHAR* codealpha3)
* @brief      Code get by code alpha3
* @ingroup    XUTILS
* 
* @param[in]  codealpha3 : Codealpha3 pointer to use.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XLANGUAGE_ISO_639_3::Code_GetByCodeAlpha3(XCHAR* codealpha3)
{
  for(XDWORD c=0; c<XLANGUAGE_ISO_639_3_NENTRYS; c++)
    {
      XSTRING _codealpha3 = iso_639_3_entry[c].codealpha3;

      if(!_codealpha3.Compare(codealpha3)) return iso_639_3_entry[c].code;
    }

  return XLANGUAGE_ISO_639_3_CODE_INVALID;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XLANGUAGE_ISO_639_3::Code_GetByCodeAlpha2(XCHAR* codealpha2)
* @brief      Code get by code alpha2
* @ingroup    XUTILS
* 
* @param[in]  codealpha2 : Codealpha2 pointer to use.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XLANGUAGE_ISO_639_3::Code_GetByCodeAlpha2(XCHAR* codealpha2)
{
  for(XDWORD c=0; c<XLANGUAGE_ISO_639_3_NENTRYS; c++)
    {
      XSTRING _codealpha2 = iso_639_3_entry[c].codealpha2;

      if(!_codealpha2.Compare(codealpha2)) return iso_639_3_entry[c].code;
    }

  return XLANGUAGE_ISO_639_3_CODE_INVALID;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XLANGUAGE_ISO_639_3::Code_GetByEnglishName(XCHAR* englishname)
* @brief      Code get by english name
* @ingroup    XUTILS
* 
* @param[in]  englishname : Englishname pointer to use.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XLANGUAGE_ISO_639_3::Code_GetByEnglishName(XCHAR* englishname)
{
  for(XDWORD c=0; c<XLANGUAGE_ISO_639_3_NENTRYS; c++)
    {
      XSTRING _englishname = iso_639_3_entry[c].englishname;

      if(!_englishname.Compare(englishname)) return iso_639_3_entry[c].code;
    }

  return XLANGUAGE_ISO_639_3_CODE_INVALID;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XLANGUAGE_ISO_639_3::Code_GetByAlias(XCHAR* alias)
* @brief      Code get by alias
* @ingroup    XUTILS
* 
* @param[in]  alias : Alias pointer to use.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XLANGUAGE_ISO_639_3::Code_GetByAlias(XCHAR* alias)
{
  for(XDWORD c=0; c<XLANGUAGE_ISO_639_3_NENTRYS; c++)
    {
      XSTRING _aliasname;

      _aliasname = iso_639_3_entry[c].alias1name;
      if(!_aliasname.Compare(alias)) return iso_639_3_entry[c].code;

      _aliasname = iso_639_3_entry[c].alias2name;
      if(!_aliasname.Compare(alias)) return iso_639_3_entry[c].code;
    }

  return XLANGUAGE_ISO_639_3_CODE_INVALID;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLANGUAGE_ISO_639_3::CodeAlpha3_GetByCode(XDWORD code, XSTRING& codealpha3)
* @brief      Code alpha3 get by code
* @ingroup    XUTILS
* 
* @param[in]  code : Code value.
* @param[in]  codealpha3 : Codealpha3 value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLANGUAGE_ISO_639_3::CodeAlpha3_GetByCode(XDWORD code, XSTRING& codealpha3)
{
  codealpha3.Empty();

  for(XDWORD c=0; c<XLANGUAGE_ISO_639_3_NENTRYS; c++)
    {
      if(iso_639_3_entry[c].code == code)
        {
          codealpha3 = iso_639_3_entry[c].codealpha3;
          return true;
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLANGUAGE_ISO_639_3::CodeAlpha3_GetByCodeAlpha2(XCHAR* codealpha2, XSTRING& codealpha3)
* @brief      Code alpha3 get by code alpha2
* @ingroup    XUTILS
* 
* @param[in]  codealpha2 : Codealpha2 pointer to use.
* @param[in]  codealpha3 : Codealpha3 value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLANGUAGE_ISO_639_3::CodeAlpha3_GetByCodeAlpha2(XCHAR* codealpha2, XSTRING& codealpha3)
{
  codealpha3.Empty();

  for(XDWORD c=0; c<XLANGUAGE_ISO_639_3_NENTRYS; c++)
    {
      XSTRING _codealpha2 = iso_639_3_entry[c].codealpha2;

      if(!_codealpha2.Compare(codealpha2))
        {
          codealpha3 = iso_639_3_entry[c].codealpha3;
          return true;
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XLANGUAGE_ISO_639_3::CodeAlpha2_GetByCode(XDWORD code, XSTRING& codealpha2)
* @brief      Code alpha2 get by code (ISO 639-1; used by web translate APIs)
* @ingroup    XUTILS
*
* --------------------------------------------------------------------------------------------------------------------*/
bool XLANGUAGE_ISO_639_3::CodeAlpha2_GetByCode(XDWORD code, XSTRING& codealpha2)
{
  codealpha2.Empty();

  for(XDWORD c=0; c<XLANGUAGE_ISO_639_3_NENTRYS; c++)
    {
      if(iso_639_3_entry[c].code == code)
        {
          codealpha2 = iso_639_3_entry[c].codealpha2;
          return (!codealpha2.IsEmpty());
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XLANGUAGE_ISO_639_3::CodeAlpha2_GetByCodeAlpha3(XCHAR* codealpha3, XSTRING& codealpha2)
* @brief      Code alpha2 get by code alpha3
* @ingroup    XUTILS
*
* --------------------------------------------------------------------------------------------------------------------*/
bool XLANGUAGE_ISO_639_3::CodeAlpha2_GetByCodeAlpha3(XCHAR* codealpha3, XSTRING& codealpha2)
{
  codealpha2.Empty();

  for(XDWORD c=0; c<XLANGUAGE_ISO_639_3_NENTRYS; c++)
    {
      XSTRING _codealpha3 = iso_639_3_entry[c].codealpha3;

      if(!_codealpha3.Compare(codealpha3, true))
        {
          codealpha2 = iso_639_3_entry[c].codealpha2;
          return (!codealpha2.IsEmpty());
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XLANGUAGE_ISO_639_3::CodeAlpha2_Resolve(XCHAR* languageid, XSTRING& codealpha2)
* @brief      Resolve alpha2 / alpha3 (or legacy translate aliases) to ISO 639-1 alpha2
* @ingroup    XUTILS
*
* @note       Output is the codealpha2 used by Google Translate / MyMemory (en, es, zh, he, ...).
*             Accepts case-insensitive alpha2/alpha3. Maps legacy Google codes iw→he, jw→jv.
*             Does not rewrite BCP-47 tags such as zh-CN (caller should pass those through).
*
* --------------------------------------------------------------------------------------------------------------------*/
bool XLANGUAGE_ISO_639_3::CodeAlpha2_Resolve(XCHAR* languageid, XSTRING& codealpha2)
{
  codealpha2.Empty();

  if((!languageid) || (!languageid[0])) return false;

  // Already a known ISO 639-1 alpha2?
  for(XDWORD c=0; c<XLANGUAGE_ISO_639_3_NENTRYS; c++)
    {
      XSTRING _codealpha2 = iso_639_3_entry[c].codealpha2;

      if(!_codealpha2.IsEmpty() && !_codealpha2.Compare(languageid, true))
        {
          codealpha2 = _codealpha2;
          return true;
        }
    }

  // GEN / ISO 639-2/3 alpha3 → alpha2
  if(CodeAlpha2_GetByCodeAlpha3(languageid, codealpha2)) return true;

  // Legacy Google Translate codes still seen in some responses / docs
  if(XSTRING::Compare(languageid, _L("iw"), true) == 0)
    {
      codealpha2 = _L("he");
      return true;
    }

  if(XSTRING::Compare(languageid, _L("jw"), true) == 0)
    {
      codealpha2 = _L("jv");
      return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLANGUAGE_ISO_639_3::CodeAlpha3_GetByEnglishName(XCHAR* englishname, XSTRING& codealpha3)
* @brief      Code alpha3 get by english name
* @ingroup    XUTILS
* 
* @param[in]  englishname : Englishname pointer to use.
* @param[in]  codealpha3 : Codealpha3 value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLANGUAGE_ISO_639_3::CodeAlpha3_GetByEnglishName(XCHAR* englishname, XSTRING& codealpha3)
{
  codealpha3.Empty();

  for(XDWORD c=0; c<XLANGUAGE_ISO_639_3_NENTRYS; c++)
    {
      XSTRING _englishname = iso_639_3_entry[c].englishname;

      if(!_englishname.Compare(englishname))
        {
          codealpha3 = iso_639_3_entry[c].codealpha3;
          return true;
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLANGUAGE_ISO_639_3::CodeAlpha3_GetByAlias(XCHAR* alias, XSTRING& codealpha3)
* @brief      Code alpha3 get by alias
* @ingroup    XUTILS
* 
* @param[in]  alias : Alias pointer to use.
* @param[in]  codealpha3 : Codealpha3 value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLANGUAGE_ISO_639_3::CodeAlpha3_GetByAlias(XCHAR* alias, XSTRING& codealpha3)
{
  codealpha3.Empty();

  for(XDWORD c=0; c<XLANGUAGE_ISO_639_3_NENTRYS; c++)
    {
      XSTRING _aliasname;

      _aliasname = iso_639_3_entry[c].alias1name;
      if(!_aliasname.Compare(alias))
        {
          codealpha3 = iso_639_3_entry[c].codealpha3;
          return true;
        }

      _aliasname = iso_639_3_entry[c].alias2name;
      if(!_aliasname.Compare(alias))
        {
          codealpha3 = iso_639_3_entry[c].codealpha3;
          return true;
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLANGUAGE_ISO_639_3::EnglishName_GetByCode(XDWORD code, XSTRING& englishname)
* @brief      English name get by code
* @ingroup    XUTILS
* 
* @param[in]  code : Code value.
* @param[in]  englishname : Englishname value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLANGUAGE_ISO_639_3::EnglishName_GetByCode(XDWORD code, XSTRING& englishname)
{
  englishname.Empty();

  for(XDWORD c=0; c<XLANGUAGE_ISO_639_3_NENTRYS; c++)
    {
      if(iso_639_3_entry[c].code == code)
        {
          englishname = iso_639_3_entry[c].englishname;
          return true;
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLANGUAGE_ISO_639_3::EnglishName_GetByCodeAlpha3(XCHAR* codealpha3, XSTRING& englishname)
* @brief      English name get by code alpha3
* @ingroup    XUTILS
* 
* @param[in]  codealpha3 : Codealpha3 pointer to use.
* @param[in]  englishname : Englishname value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLANGUAGE_ISO_639_3::EnglishName_GetByCodeAlpha3(XCHAR* codealpha3, XSTRING& englishname)
{
  englishname.Empty();

  for(XDWORD c=0; c<XLANGUAGE_ISO_639_3_NENTRYS; c++)
    {
      XSTRING _codealpha3 = iso_639_3_entry[c].codealpha3;

      if(!_codealpha3.Compare(codealpha3))
        {
          englishname = iso_639_3_entry[c].englishname;
          return true;
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLANGUAGE_ISO_639_3::EnglishName_GetByCodeAlpha2(XCHAR* codealpha2, XSTRING& englishname)
* @brief      English name get by code alpha2
* @ingroup    XUTILS
* 
* @param[in]  codealpha2 : Codealpha2 pointer to use.
* @param[in]  englishname : Englishname value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLANGUAGE_ISO_639_3::EnglishName_GetByCodeAlpha2(XCHAR* codealpha2, XSTRING& englishname)
{
  englishname.Empty();

  for(XDWORD c=0; c<XLANGUAGE_ISO_639_3_NENTRYS; c++)
    {
      XSTRING _codealpha2 = iso_639_3_entry[c].codealpha2;

      if(!_codealpha2.Compare(codealpha2))
        {
          englishname = iso_639_3_entry[c].englishname;
          return true;
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLANGUAGE_ISO_639_3::EnglishName_GetByAlias(XCHAR* alias, XSTRING& englishname)
* @brief      English name get by alias
* @ingroup    XUTILS
* 
* @param[in]  alias : Alias pointer to use.
* @param[in]  englishname : Englishname value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLANGUAGE_ISO_639_3::EnglishName_GetByAlias(XCHAR* alias, XSTRING& englishname)
{
  englishname.Empty();

  for(XDWORD c=0; c<XLANGUAGE_ISO_639_3_NENTRYS; c++)
    {
      XSTRING _aliasname;

      _aliasname = iso_639_3_entry[c].alias1name;
      if(!_aliasname.Compare(alias))
        {
          englishname = iso_639_3_entry[c].englishname;
          return true;
        }

      _aliasname = iso_639_3_entry[c].alias2name;
      if(!_aliasname.Compare(alias))
        {
          englishname = iso_639_3_entry[c].englishname;
          return true;
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XLANGUAGE_ISO_639_3::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XLANGUAGE_ISO_639_3::Clean()
{

}



