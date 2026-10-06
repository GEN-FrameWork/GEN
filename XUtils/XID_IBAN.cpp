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
    if (!IDcountry.Compare(_L("ES"), true))
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
    int       size;
    XCHAR* contry;

  } CONTRYLIST;


  CONTRYLIST  countrylist[] = { { _L("DE"), 22, _L("Germany")                },
                                  { _L("AD"), 24, _L("Andorra")                },
                                  { _L("SA"), 24, _L("Saudi Arabia")           },
                                  { _L("AT"), 20, _L("Austria")                },
                                  { _L("BH"), 22, _L("Bahrain")                },
                                  { _L("BE"), 16, _L("Belgium")                },
                                  { _L("BA"), 20, _L("Bosnia-Herzegovina")     },
                                  { _L("BG"), 22, _L("Bulgaria")               },
                                  { _L("QA"), 29, _L("Qatar")                  },
                                  { _L("CY"), 28, _L("Cyprus")                 },
                                  { _L("CR"), 22, _L("Costa Rica")             },
                                  { _L("HR"), 21, _L("Croatia")                },
                                  { _L("DK"), 18, _L("Denmark")                },
                                  { _L("AE"), 23, _L("United Arab Emirates")   },
                                  { _L("SK"), 24, _L("Slovakia")               },
                                  { _L("SI"), 19, _L("Slovenia")               },
                                  { _L("ES"), 24, _L("Spain")                  },
                                  { _L("EE"), 20, _L("Estonia")                },
                                  { _L("FI"), 18, _L("Finland")                },
                                  { _L("FR"), 27, _L("France")                 },
                                  { _L("GE"), 22, _L("Georgia")                },
                                  { _L("GI"), 23, _L("Gibraltar")              },
                                  { _L("GB"), 22, _L("Great Britain")          },
                                  { _L("GR"), 27, _L("Greece")                 },
                                  { _L("GL"), 18, _L("Greenland")              },
                                  { _L("GB"), 22, _L("Guernsey")               },
                                  { _L("HU"), 28, _L("Hungary")                },
                                  { _L("IE"), 22, _L("Ireland")                },
                                  { _L("GB"), 22, _L("Isle of Man")            },
                                  { _L("IM"), 22, _L("Isle of Man")            },
                                  { _L("IS"), 26, _L("Iceland")                },
                                  { _L("FO"), 18, _L("Faroe Islands")          },
                                  { _L("IT"), 27, _L("Italy")                  },
                                  { _L("GB"), 22, _L("Jersey")                 },
                                  { _L("JO"), 30, _L("Jordan")                 },
                                  { _L("KZ"), 20, _L("Kazakhstan")             },
                                  { _L("KW"), 30, _L("Kuwait")                 },
                                  { _L("LV"), 21, _L("Latvia")                 },
                                  { _L("LB"), 28, _L("Lebanon")                },
                                  { _L("LI"), 21, _L("Liechtenstein")          },
                                  { _L("LT"), 20, _L("Lithuania")              },
                                  { _L("LU"), 20, _L("Luxembourg")             },
                                  { _L("MK"), 19, _L("Macedonia")              },
                                  { _L("MT"), 31, _L("Malta")                  },
                                  { _L("MD"), 24, _L("Moldova")                },
                                  { _L("MC"), 27, _L("Monaco")                 },
                                  { _L("ME"), 22, _L("Montenegro")             },
                                  { _L("NO"), 15, _L("Norway")                 },
                                  { _L("NL"), 18, _L("Netherlands")            },
                                  { _L("PS"), 29, _L("Palestine")              },
                                  { _L("PL"), 28, _L("Poland")                 },
                                  { _L("PT"), 25, _L("Portugal")               },
                                  { _L("CZ"), 24, _L("Czech Republic")         },
                                  { _L("RO"), 24, _L("Romania")                },
                                  { _L("SM"), 27, _L("San Marino")             },
                                  { _L("SE"), 24, _L("Sweden")                 },
                                  { _L("CH"), 21, _L("Switzerland")            },
                                  { _L("TN"), 24, _L("Tunisia")                },
                                  { _L("TR"), 26, _L("Turkiye")                }
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
  DCstring.Format(_L("%02d"), DC);

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





