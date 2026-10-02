/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XID_IBAN.h
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XString.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XID_IBAN
{
  public:
                        XID_IBAN                      ();
    virtual            ~XID_IBAN                      ();

    XSTRING*            Get                           ();
    bool                Set                           (XCHAR* IBAN);

    XSTRING*            GetCountry                    ();

  private:

    bool                IsValidSizeCountry            (XCHAR* countrystr, int size);
    int                 Mod97                         (XSTRING& IBANstr);

    int                 Spain_CalculeControlDigit     (XSTRING& IBANstr);
    bool                Spain_ValidateControlDigit    (XSTRING& IBANstr);

    void                Clean                         ();

    XSTRING             IBANstr;

    XSTRING             IDcountry;
    XSTRING             country;
    int                 size;
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




















