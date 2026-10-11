/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XID_IBAN.cpp
*
* @class      XID_IBAN
* @brief      eXtended Utils IBAN Number (International Bank Account Number)
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

#include "XID_IBAN.h"

#include "XBuffer.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XID_IBAN::XID_IBAN()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XID_IBAN::XID_IBAN()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XID_IBAN::~XID_IBAN()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XID_IBAN::~XID_IBAN()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* XID_IBAN::Get()
* @brief      Get value
* @ingroup    XUTILS
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* XID_IBAN::Get()
{
  return &IBANstr;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XID_IBAN::Set(XCHAR* IBAN)
* @brief      Set value
* @ingroup    XUTILS
* 
* @param[in]  IBAN : IBAN pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XID_IBAN::Set(XCHAR* IBAN)
{
  bool status = false;

  IBANstr = IBAN;

  IBANstr.DeleteCharacter(_C(' '));
  IBANstr.DeleteCharacter(_C('-'));
  IBANstr.DeleteCharacter(_C('_'));
  IBANstr.ToUpperCase();

  IBANstr.Copy(0, 2, IDcountry);

  status = IsValidSizeCountry(IDcountry.Get(), IBANstr.GetSize());
  if (status)
  {
    if (!IDcountry.Compare(NL("ES"), true))
    {
      if (!Spain_ValidateControlDigit(IBANstr))
      {
        return false;
      }
    }

    XSTRING IBANtempo;
    XSTRING IBANtempo2;
    XSTRING tempo;

    size = IBANstr.GetSize();

    IBANtempo = IBANstr.Get();

    IBANtempo.Copy(0, 4, tempo);
    IBANtempo.DeleteCharacters(0, 4);
    IBANtempo.Add(tempo);

    for (int c = 0; c < IBANtempo.GetSize(); c++)
    {
      if (!IBANtempo.Character_IsNumber(IBANtempo.Get()[c], false))
      {
        XCHAR code1 = (IBANtempo.Get()[c] - 55) / 10 + '0';
        XCHAR code2 = (IBANtempo.Get()[c] - 55) % 10 + '0';

        IBANtempo2 += code1;
        IBANtempo2 += code2;
      }
      else
      {
        IBANtempo2 += IBANtempo.Get()[c];
      }
    }

    if (Mod97(IBANtempo2) != 1)
    {
      IBANstr.Empty();

      return false;
    }

    return true;

  }
  else
  {
    IDcountry.Empty();
  }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* XID_IBAN::GetCountry()
* @brief      Get country
* @ingroup    XUTILS
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* XID_IBAN::GetCountry()
{
  return &country;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XID_IBAN::IsValidSizeCountry(XCHAR* countrystr, int size)
* @brief      Is valid size country
* @ingroup    XUTILS
* 
* @param[in]  countrystr : Countrystr pointer to use.
* @param[in]  size : Size value.
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XID_IBAN::IsValidSizeCountry(XCHAR* countrystr, int size)
{
  typedef struct
  {
    XCHAR* IDcontry;
    int    size;
    XCHAR* contry;

  } CONTRYLIST;


  CONTRYLIST  countrylist[] = {   { NL("DE"), 22, NL("Germany")                },
                                  { NL("AD"), 24, NL("Andorra")                },
                                  { NL("SA"), 24, NL("Saudi Arabia")           },
                                  { NL("AT"), 20, NL("Austria")                },
                                  { NL("BH"), 22, NL("Bahrain")                },
                                  { NL("BE"), 16, NL("Belgium")                },
                                  { NL("BA"), 20, NL("Bosnia-Herzegovina")     },
                                  { NL("BG"), 22, NL("Bulgaria")               },
                                  { NL("QA"), 29, NL("Qatar")                  },
                                  { NL("CY"), 28, NL("Cyprus")                 },
                                  { NL("CR"), 22, NL("Costa Rica")             },
                                  { NL("HR"), 21, NL("Croatia")                },
                                  { NL("DK"), 18, NL("Denmark")                },
                                  { NL("AE"), 23, NL("United Arab Emirates")   },
                                  { NL("SK"), 24, NL("Slovakia")               },
                                  { NL("SI"), 19, NL("Slovenia")               },
                                  { NL("ES"), 24, NL("Spain")                  },
                                  { NL("EE"), 20, NL("Estonia")                },
                                  { NL("FI"), 18, NL("Finland")                },
                                  { NL("FR"), 27, NL("France")                 },
                                  { NL("GE"), 22, NL("Georgia")                },
                                  { NL("GI"), 23, NL("Gibraltar")              },
                                  { NL("GB"), 22, NL("Great Britain")          },
                                  { NL("GR"), 27, NL("Greece")                 },
                                  { NL("GL"), 18, NL("Greenland")              },
                                  { NL("GB"), 22, NL("Guernsey")               },
                                  { NL("HU"), 28, NL("Hungary")                },
                                  { NL("IE"), 22, NL("Ireland")                },
                                  { NL("GB"), 22, NL("Isle of Man")            },
                                  { NL("IM"), 22, NL("Isle of Man")            },
                                  { NL("IS"), 26, NL("Iceland")                },
                                  { NL("FO"), 18, NL("Faroe Islands")          },
                                  { NL("IT"), 27, NL("Italy")                  },
                                  { NL("GB"), 22, NL("Jersey")                 },
                                  { NL("JO"), 30, NL("Jordan")                 },
                                  { NL("KZ"), 20, NL("Kazakhstan")             },
                                  { NL("KW"), 30, NL("Kuwait")                 },
                                  { NL("LV"), 21, NL("Latvia")                 },
                                  { NL("LB"), 28, NL("Lebanon")                },
                                  { NL("LI"), 21, NL("Liechtenstein")          },
                                  { NL("LT"), 20, NL("Lithuania")              },
                                  { NL("LU"), 20, NL("Luxembourg")             },
                                  { NL("MK"), 19, NL("Macedonia")              },
                                  { NL("MT"), 31, NL("Malta")                  },
                                  { NL("MD"), 24, NL("Moldova")                },
                                  { NL("MC"), 27, NL("Monaco")                 },
                                  { NL("ME"), 22, NL("Montenegro")             },
                                  { NL("NO"), 15, NL("Norway")                 },
                                  { NL("NL"), 18, NL("Netherlands")            },
                                  { NL("PS"), 29, NL("Palestine")              },
                                  { NL("PL"), 28, NL("Poland")                 },
                                  { NL("PT"), 25, NL("Portugal")               },
                                  { NL("CZ"), 24, NL("Czech Republic")         },
                                  { NL("RO"), 24, NL("Romania")                },
                                  { NL("SM"), 27, NL("San Marino")             },
                                  { NL("SE"), 24, NL("Sweden")                 },
                                  { NL("CH"), 21, NL("Switzerland")            },
                                  { NL("TN"), 24, NL("Tunisia")                },
                                  { NL("TR"), 26, NL("Turkiye")                }
  };
  XSTRING     cs;

  cs = countrystr;

  for (XDWORD c = 0; c < sizeof(countrylist) / sizeof(CONTRYLIST); c++)
  {
    CONTRYLIST* countryitem = &countrylist[c];
    if (countryitem)
    {
      if (!cs.Compare(countryitem->IDcontry, false))
      {
        if (countryitem->size == size)
        {
          country = countryitem->contry;

          return true;
        }
      }
    }
  }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XID_IBAN::Mod97(XSTRING& IBANstr)
* @brief      Mod97
* @ingroup    XUTILS
* 
* @param[in]  IBANstr : IBA Nstr value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XID_IBAN::Mod97(XSTRING& IBANstr)
{
  XBUFFER     xbuffer;
  const char* iban = NULL;
  int         size = 0;
  char* temp;
  int         mod = 0;
  int         i;

  IBANstr.ConvertToASCII(xbuffer);

  iban = (char*)xbuffer.Get();
  size = (int)strlen(iban);

  temp = GEN_NEW char[size + 5];

  if (!temp)
  {
    return 0;
  }

  strcpy(temp, iban);

  for (i = 0; i < strlen(temp); i++)
  {
    if (temp[i] >= 'A' && temp[i] <= 'Z')
    {
      mod = ((mod * 10) + (temp[i] - 'A' + 10)) % 97;
    }
    else
    {
      if (temp[i] >= 'a' && temp[i] <= 'z')
      {
        mod = ((mod * 10) + (temp[i] - 'a' + 10)) % 97;
      }
      else
      {
        mod = ((mod * 10) + (temp[i] - '0')) % 97;
      }
    }
  }

  GEN_DELETE_ARRAY temp;

  return mod;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XID_IBAN::Spain_CalculeControlDigit(XSTRING& IBANstr)
* @brief      Spain calcule control digit
* @ingroup    XUTILS
* 
* @param[in]  IBANstr : IBA Nstr value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XID_IBAN::Spain_CalculeControlDigit(XSTRING& IBANstr)
{
  int firstdigit = 0;
  int seconddigit = 0;
  int rest = 0;
  int bankID[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  int accountnumber[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };


  bankID[0] = (IBANstr.Get()[4] - 48) * 4;
  bankID[1] = (IBANstr.Get()[5] - 48) * 8;
  bankID[2] = (IBANstr.Get()[6] - 48) * 5;
  bankID[3] = (IBANstr.Get()[7] - 48) * 10;

  bankID[4] = (IBANstr.Get()[8] - 48) * 9;
  bankID[5] = (IBANstr.Get()[9] - 48) * 7;
  bankID[6] = (IBANstr.Get()[10] - 48) * 3;
  bankID[7] = (IBANstr.Get()[11] - 48) * 6;

  for(int c = 0; c < 8; c++)
    {
      firstdigit += bankID[c];
    }

  rest = (firstdigit % 11);

  firstdigit = (11 - rest);

  switch(firstdigit)
  {
    case 10 : firstdigit = 1;
              break;

    case 11 : firstdigit = 0;
              break;  
  }

  accountnumber[0] = (IBANstr.Get()[14] - 48) * 1;
  accountnumber[1] = (IBANstr.Get()[15] - 48) * 2;
  accountnumber[2] = (IBANstr.Get()[16] - 48) * 4;
  accountnumber[3] = (IBANstr.Get()[17] - 48) * 8;
  accountnumber[4] = (IBANstr.Get()[18] - 48) * 5;
  accountnumber[5] = (IBANstr.Get()[19] - 48) * 10;
  accountnumber[6] = (IBANstr.Get()[20] - 48) * 9;
  accountnumber[7] = (IBANstr.Get()[21] - 48) * 7;
  accountnumber[8] = (IBANstr.Get()[22] - 48) * 3;
  accountnumber[9] = (IBANstr.Get()[23] - 48) * 6;

  for (int c = 0; c < 10; c++)
    {
      seconddigit += accountnumber[c];
    }


  rest = (seconddigit % 11);

  seconddigit = (11 - rest);

  switch(seconddigit)
    {
      case 10 : seconddigit = 1;
                break;

      case 11 : seconddigit = 0;
                break;
    }

  return (firstdigit * 10) + (seconddigit);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XID_IBAN::Spain_ValidateControlDigit(XSTRING& IBANstr)
* @brief      Spain validate control digit
* @ingroup    XUTILS
* 
* @param[in]  IBANstr : IBA Nstr value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XID_IBAN::Spain_ValidateControlDigit(XSTRING& IBANstr)
{
  if (IBANstr.GetSize() != 24)
  {
    return false;
  }

  XSTRING DCstring;
  XSTRING DCstring2;
  int     DC = 0;

  DC = Spain_CalculeControlDigit(IBANstr);
  DCstring.Format(NL("%02d"), DC);

  DCstring2.Add(IBANstr.Get()[12]);
  DCstring2.Add(IBANstr.Get()[13]);

  if (!DCstring.Compare(DCstring2.Get(), true))
  {
    return true;
  }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XID_IBAN::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XID_IBAN::Clean()
{
  IBANstr.Empty();
  country.Empty();
  size = 0;
}





