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


XLANGUAGE_ISO_639_3_ENTRY  iso_639_3_entry[] = {   {  XLANGUAGE_ISO_639_3_CODE_ENG,  NL("eng")   ,  NL("en")   ,  NL("English")               , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_SPA,  NL("spa")   ,  NL("es")   ,  NL("Spanish")               , NL("Castilian")          , NL("")                       },   

                                                   //#ifndef MICROCONTROLLER
                                                   {  XLANGUAGE_ISO_639_3_CODE_AAR,  NL("aar")   ,  NL("aa")   ,  NL("Afar")                  , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_ABK,  NL("abk")   ,  NL("ab")   ,  NL("Abkhazian")             , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_AFR,  NL("afr")   ,  NL("af")   ,  NL("Afrikaans")             , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_AKA,  NL("aka")   ,  NL("ak")   ,  NL("Akan")                  , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_ALB,  NL("alb")   ,  NL("sq")   ,  NL("Albanian")              , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_AMH,  NL("amh")   ,  NL("am")   ,  NL("Amharic")               , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_ARA,  NL("ara")   ,  NL("ar")   ,  NL("Arabic")                , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_ARG,  NL("arg")   ,  NL("an")   ,  NL("Aragonese")             , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_ARM,  NL("ARM")   ,  NL("hy")   ,  NL("Armenia")               , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_ASM,  NL("asm")   ,  NL("as")   ,  NL("Assamese")              , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_AVA,  NL("ava")   ,  NL("av")   ,  NL("Avaric")                , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_AVE,  NL("ave")   ,  NL("ae")   ,  NL("Avestan")               , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_AYM,  NL("aym")   ,  NL("ay")   ,  NL("Aymara")                , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_AZE,  NL("aze")   ,  NL("az")   ,  NL("Azerbaijani")           , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_BAK,  NL("bak")   ,  NL("ba")   ,  NL("Bashkir")               , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_BAM,  NL("bam")   ,  NL("bm")   ,  NL("Bambara")               , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_BAQ,  NL("baq")   ,  NL("eu")   ,  NL("Basque")                , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_BEL,  NL("bel")   ,  NL("be")   ,  NL("Belarusian")            , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_BEN,  NL("ben")   ,  NL("bn")   ,  NL("Bengali")               , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_BIH,  NL("bih")   ,  NL("bh")   ,  NL("Bihari languages")      , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_BIS,  NL("bis")   ,  NL("bi")   ,  NL("Bislama")               , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_BOS,  NL("bos")   ,  NL("bs")   ,  NL("Bosnian")               , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_BRE,  NL("bre")   ,  NL("br")   ,  NL("Breton")                , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_BUL,  NL("bul")   ,  NL("bg")   ,  NL("Bulgarian")             , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_BUR,  NL("bur")   ,  NL("my")   ,  NL("Burmese")               , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_CAT,  NL("cat")   ,  NL("ca")   ,  NL("Catalan")               , NL("Valencian")          , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_CHA,  NL("cha")   ,  NL("ch")   ,  NL("Chamorro")              , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_CHE,  NL("che")   ,  NL("ce")   ,  NL("Chechen")               , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_CHI,  NL("chi")   ,  NL("zh")   ,  NL("Chinese")               , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_CHU,  NL("chu")   ,  NL("cu")   ,  NL("Church Slavic")         , NL("Old Slavonic")       , NL("Church Slavonic")        },
                                                   {  XLANGUAGE_ISO_639_3_CODE_CHV,  NL("chv")   ,  NL("cv")   ,  NL("Chuvash")               , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_COR,  NL("cor")   ,  NL("kw")   ,  NL("Cornish")               , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_COS,  NL("cos")   ,  NL("co")   ,  NL("Corsican")              , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_CRE,  NL("cre")   ,  NL("cr")   ,  NL("Cree")                  , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_CZE,  NL("cze")   ,  NL("cs")   ,  NL("Czech")                 , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_DAN,  NL("dan")   ,  NL("da")   ,  NL("Danish")                , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_DIV,  NL("div")   ,  NL("dv")   ,  NL("Divehi")                , NL("Dhivehi")            , NL("Maldivian")              },
                                                   {  XLANGUAGE_ISO_639_3_CODE_DUT,  NL("dut")   ,  NL("nl")   ,  NL("Dutch")                 , NL("Flemish")            , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_DZO,  NL("dzo")   ,  NL("dz")   ,  NL("Dzongkha")              , NL("")                   , NL("")                       },                                                   
                                                   {  XLANGUAGE_ISO_639_3_CODE_EPO,  NL("epo")   ,  NL("eo")   ,  NL("Esperanto")             , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_EST,  NL("est")   ,  NL("et")   ,  NL("Estonian")              , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_EWE,  NL("ewe")   ,  NL("ee")   ,  NL("Ewe")                   , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_FAO,  NL("fao")   ,  NL("fo")   ,  NL("Faroese")               , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_FIJ,  NL("fij")   ,  NL("fj")   ,  NL("Fijian")                , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_FIN,  NL("fin")   ,  NL("fi")   ,  NL("Finnish")               , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_FRE,  NL("fre")   ,  NL("fr")   ,  NL("French")                , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_FRY,  NL("fry")   ,  NL("fy")   ,  NL("Western Frisian")       , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_FUL,  NL("ful")   ,  NL("ff")   ,  NL("Fulah")                 , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_GEO,  NL("geo")   ,  NL("ka")   ,  NL("Georgian")              , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_GER,  NL("ger")   ,  NL("de")   ,  NL("German")                , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_GLA,  NL("gla")   ,  NL("gd")   ,  NL("Gaelic")                , NL("Scottish Gaelic")    , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_GLE,  NL("gle")   ,  NL("ga")   ,  NL("Irish")                 , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_GLG,  NL("glg")   ,  NL("gl")   ,  NL("Galician")              , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_GLV,  NL("glv")   ,  NL("gv")   ,  NL("Manx")                  , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_GRE,  NL("gre")   ,  NL("el")   ,  NL("Greek Modern (1453-)")  , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_GRN,  NL("grn")   ,  NL("gn")   ,  NL("Guarani")               , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_GUJ,  NL("guj")   ,  NL("gu")   ,  NL("Gujarati")              , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_HAT,  NL("hat")   ,  NL("ht")   ,  NL("Haitian")               , NL("Haitian Creole")     , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_HAU,  NL("hau")   ,  NL("ha")   ,  NL("Hausa")                 , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_HEB,  NL("heb")   ,  NL("he")   ,  NL("Hebrew")                , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_HER,  NL("her")   ,  NL("hz")   ,  NL("Herero")                , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_HIN,  NL("hin")   ,  NL("hi")   ,  NL("Hindi")                 , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_HMO,  NL("hmo")   ,  NL("ho")   ,  NL("Hiri Motu")             , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_HRV,  NL("hrv")   ,  NL("hr")   ,  NL("Croatian")              , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_HUN,  NL("hun")   ,  NL("hu")   ,  NL("Hungarian")             , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_IBO,  NL("ibo")   ,  NL("ig")   ,  NL("Igbo")                  , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_ICE,  NL("ice")   ,  NL("is")   ,  NL("Icelandic")             , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_IDO,  NL("ido")   ,  NL("io")   ,  NL("Ido")                   , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_III,  NL("iii")   ,  NL("ii")   ,  NL("Sichuan Yi")            , NL("Nuosu")              , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_IKU,  NL("iku")   ,  NL("iu")   ,  NL("Inuktitut")             , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_ILE,  NL("ile")   ,  NL("ie")   ,  NL("Interlingue")           , NL("Occidental")         , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_INA,  NL("ina")   ,  NL("ia")   ,  NL("Interlingua")           , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_IND,  NL("ind")   ,  NL("id")   ,  NL("Indonesian")            , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_IPK,  NL("ipk")   ,  NL("ik")   ,  NL("Inupiaq")               , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_ITA,  NL("ita")   ,  NL("it")   ,  NL("Italian")               , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_JAV,  NL("jav")   ,  NL("jv")   ,  NL("Javanese")              , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_JPN,  NL("jpn")   ,  NL("ja")   ,  NL("Japanese")              , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_KAL,  NL("kal")   ,  NL("kl")   ,  NL("Kalaallisut")           , NL("Greenlandic")        , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_KAN,  NL("kan")   ,  NL("kn")   ,  NL("Kannada")               , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_KAS,  NL("kas")   ,  NL("ks")   ,  NL("Kashmiri")              , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_KAU,  NL("kau")   ,  NL("kr")   ,  NL("Kanuri")                , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_KAZ,  NL("kaz")   ,  NL("kk")   ,  NL("Kazakh")                , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_KHM,  NL("khm")   ,  NL("km")   ,  NL("Central Khmer")         , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_KIK,  NL("kik")   ,  NL("ki")   ,  NL("Kikuyu")                , NL("Gikuyu")             , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_KIN,  NL("kin")   ,  NL("rw")   ,  NL("Kinyarwanda")           , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_KIR,  NL("kir")   ,  NL("ky")   ,  NL("Kirghiz")               , NL("Kyrgyz")             , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_KOM,  NL("kom")   ,  NL("kv")   ,  NL("Komi")                  , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_KON,  NL("kon")   ,  NL("kg")   ,  NL("Kongo")                 , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_KOR,  NL("kor")   ,  NL("ko")   ,  NL("Korean")                , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_KUA,  NL("kua")   ,  NL("kj")   ,  NL("Kuanyama")              , NL("Kwanyama")           , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_KUR,  NL("kur")   ,  NL("ku")   ,  NL("Kurdish")               , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_LAO,  NL("lao")   ,  NL("lo")   ,  NL("Lao")                   , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_LAT,  NL("lat")   ,  NL("la")   ,  NL("Latin")                 , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_LAV,  NL("lav")   ,  NL("lv")   ,  NL("Latvian")               , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_LIM,  NL("lim")   ,  NL("li")   ,  NL("Limburgan")             , NL("Limburger")          , NL("Limburgish")             },
                                                   {  XLANGUAGE_ISO_639_3_CODE_LIN,  NL("lin")   ,  NL("ln")   ,  NL("Lingala")               , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_LIT,  NL("lit")   ,  NL("lt")   ,  NL("Lithuanian")            , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_LTZ,  NL("ltz")   ,  NL("lb")   ,  NL("Luxembourgish")         , NL("Letzeburgesch")      , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_LUB,  NL("lub")   ,  NL("lu")   ,  NL("Luba-Katanga")          , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_LUG,  NL("lug")   ,  NL("lg")   ,  NL("Ganda")                 , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_MAC,  NL("mac")   ,  NL("mk")   ,  NL("Macedonian")            , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_MAH,  NL("mah")   ,  NL("mh")   ,  NL("Marshallese")           , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_MAL,  NL("mal")   ,  NL("ml")   ,  NL("Malayalam")             , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_MAO,  NL("mao")   ,  NL("mi")   ,  NL("Maori")                 , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_MAR,  NL("mar")   ,  NL("mr")   ,  NL("Marathi")               , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_MAY,  NL("may")   ,  NL("ms")   ,  NL("Malay")                 , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_MLG,  NL("mlg")   ,  NL("mg")   ,  NL("Malagasy")              , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_MLT,  NL("mlt")   ,  NL("mt")   ,  NL("Maltese")               , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_MON,  NL("mon")   ,  NL("mn")   ,  NL("Mongolian")             , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_NAU,  NL("nau")   ,  NL("na")   ,  NL("Nauru")                 , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_NAV,  NL("nav")   ,  NL("nv")   ,  NL("Navajo")                , NL("Navaho")             , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_NBL,  NL("nbl")   ,  NL("nr")   ,  NL("Ndebele South")         , NL("South Ndebele")      , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_NDE,  NL("nde")   ,  NL("nd")   ,  NL("Ndebele North")         , NL("North Ndebele")      , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_NDO,  NL("ndo")   ,  NL("ng")   ,  NL("Ndonga")                , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_NEP,  NL("nep")   ,  NL("ne")   ,  NL("Nepali")                , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_NNO,  NL("nno")   ,  NL("nn")   ,  NL("Norwegian Nynorsk")     , NL("Nynorsk Norwegian")  , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_NOB,  NL("nob")   ,  NL("nb")   ,  NL("Bokm_l Norwegian")      , NL("Norwegian Bokm_l")   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_NOR,  NL("nor")   ,  NL("no")   ,  NL("Norwegian")             , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_NYA,  NL("nya")   ,  NL("ny")   ,  NL("Chichewa")              , NL("Chewa")              , NL("Nyanja")                 },
                                                   {  XLANGUAGE_ISO_639_3_CODE_OCI,  NL("oci")   ,  NL("oc")   ,  NL("Occitan (post 1500)")   , NL("Proven_al")          , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_OJI,  NL("oji")   ,  NL("oj")   ,  NL("Ojibwa")                , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_ORI,  NL("ori")   ,  NL("or")   ,  NL("Oriya")                 , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_ORM,  NL("orm")   ,  NL("om")   ,  NL("Oromo")                 , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_OSS,  NL("oss")   ,  NL("os")   ,  NL("Ossetian")              , NL("Ossetic")            , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_PAN,  NL("pan")   ,  NL("pa")   ,  NL("Panjabi")               , NL("Punjabi")            , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_PER,  NL("per")   ,  NL("fa")   ,  NL("Persian")               , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_PLI,  NL("pli")   ,  NL("pi")   ,  NL("Pali")                  , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_POL,  NL("pol")   ,  NL("pl")   ,  NL("Polish")                , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_POR,  NL("por")   ,  NL("pt")   ,  NL("Portuguese")            , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_PUS,  NL("pus")   ,  NL("ps")   ,  NL("Pushto")                , NL("Pashto")             , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_QUE,  NL("que")   ,  NL("qu")   ,  NL("Quechua")               , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_ROH,  NL("roh")   ,  NL("rm")   ,  NL("Romansh")               , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_RUM,  NL("rum")   ,  NL("ro")   ,  NL("Romanian")              , NL("Moldavian")          , NL("Moldovan")               },
                                                   {  XLANGUAGE_ISO_639_3_CODE_RUN,  NL("run")   ,  NL("rn")   ,  NL("Rundi")                 , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_RUS,  NL("rus")   ,  NL("ru")   ,  NL("Russian")               , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_SAG,  NL("sag")   ,  NL("sg")   ,  NL("Sango")                 , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_SAN,  NL("san")   ,  NL("sa")   ,  NL("Sanskrit")              , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_SIN,  NL("sin")   ,  NL("si")   ,  NL("Sinhala")               , NL("Sinhalese")          , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_SLO,  NL("slo")   ,  NL("sk")   ,  NL("Slovak")                , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_SLV,  NL("slv")   ,  NL("sl")   ,  NL("Slovenian")             , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_SME,  NL("sme")   ,  NL("se")   ,  NL("Northern Sami")         , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_SMO,  NL("smo")   ,  NL("sm")   ,  NL("Samoan")                , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_SNA,  NL("sna")   ,  NL("sn")   ,  NL("Shona")                 , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_SND,  NL("snd")   ,  NL("sd")   ,  NL("Sindhi")                , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_SOM,  NL("som")   ,  NL("so")   ,  NL("Somali")                , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_SOT,  NL("sot")   ,  NL("st")   ,  NL("Sotho Southern")        , NL("")                   , NL("")                       },                                                   
                                                   {  XLANGUAGE_ISO_639_3_CODE_SRD,  NL("srd")   ,  NL("sc")   ,  NL("Sardinian")             , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_SRP,  NL("srp")   ,  NL("sr")   ,  NL("Serbian")               , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_SSW,  NL("ssw")   ,  NL("ss")   ,  NL("Swati")                 , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_SUN,  NL("sun")   ,  NL("su")   ,  NL("Sundanese")             , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_SWA,  NL("swa")   ,  NL("sw")   ,  NL("Swahili")               , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_SWE,  NL("swe")   ,  NL("sv")   ,  NL("Swedish")               , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_TAH,  NL("tah")   ,  NL("ty")   ,  NL("Tahitian")              , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_TAM,  NL("tam")   ,  NL("ta")   ,  NL("Tamil")                 , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_TAT,  NL("tat")   ,  NL("tt")   ,  NL("Tatar")                 , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_TEL,  NL("tel")   ,  NL("te")   ,  NL("Telugu")                , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_TGK,  NL("tgk")   ,  NL("tg")   ,  NL("Tajik")                 , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_TGL,  NL("tgl")   ,  NL("tl")   ,  NL("Tagalog")               , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_THA,  NL("tha")   ,  NL("th")   ,  NL("Thai")                  , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_TIB,  NL("tib")   ,  NL("bo")   ,  NL("Tibetan")               , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_TIR,  NL("tir")   ,  NL("ti")   ,  NL("Tigrinya")              , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_TON,  NL("ton")   ,  NL("to")   ,  NL("Tonga (Tonga Islands)") , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_TSN,  NL("tsn")   ,  NL("tn")   ,  NL("Tswana")                , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_TSO,  NL("tso")   ,  NL("ts")   ,  NL("Tsonga")                , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_TUK,  NL("tuk")   ,  NL("tk")   ,  NL("Turkmen")               , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_TUR,  NL("tur")   ,  NL("tr")   ,  NL("Turkish")               , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_TWI,  NL("twi")   ,  NL("tw")   ,  NL("Twi")                   , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_UIG,  NL("uig")   ,  NL("ug")   ,  NL("Uighur")                , NL("Uyghur")             , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_UKR,  NL("ukr")   ,  NL("uk")   ,  NL("Ukrainian")             , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_URD,  NL("urd")   ,  NL("ur")   ,  NL("Urdu")                  , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_UZB,  NL("uzb")   ,  NL("uz")   ,  NL("Uzbek")                 , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_VEN,  NL("ven")   ,  NL("ve")   ,  NL("Venda")                 , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_VIE,  NL("vie")   ,  NL("vi")   ,  NL("Vietnamese")            , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_VOL,  NL("vol")   ,  NL("vo")   ,  NL("Volap_k")               , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_WEL,  NL("wel")   ,  NL("cy")   ,  NL("Welsh")                 , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_WLN,  NL("wln")   ,  NL("wa")   ,  NL("Walloon")               , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_WOL,  NL("wol")   ,  NL("wo")   ,  NL("Wolof")                 , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_XHO,  NL("xho")   ,  NL("xh")   ,  NL("Xhosa")                 , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_YID,  NL("yid")   ,  NL("yi")   ,  NL("Yiddish")               , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_YOR,  NL("yor")   ,  NL("yo")   ,  NL("Yoruba")                , NL("")                   , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_ZHA,  NL("zha")   ,  NL("za")   ,  NL("Zhuang")                , NL("Chuang")             , NL("")                       },
                                                   {  XLANGUAGE_ISO_639_3_CODE_ZUL,  NL("zul")   ,  NL("zu")   ,  NL("Zulu")                  , NL("")                   , NL("")                       }
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
  if(XSTRING::Compare(languageid, NL("iw"), true) == 0)
    {
      codealpha2 = NL("he");
      return true;
    }

  if(XSTRING::Compare(languageid, NL("jw"), true) == 0)
    {
      codealpha2 = NL("jv");
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



