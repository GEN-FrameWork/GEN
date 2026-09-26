/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOEmailAddress.h
* 
* @class      DIOEMAILADDRESS
* @brief      Data Input/Output Email Address class
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XString.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOEMAILADDRESS_SEPARATOR  __L("@")

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOFACTORY;

class DIOEMAILADDRESS: public XSTRING
{
  public:

                      DIOEMAILADDRESS            ();
    virtual          ~DIOEMAILADDRESS            ();

                      DIOEMAILADDRESS            (XDWORD size);
                      DIOEMAILADDRESS            (const char* url);
                      DIOEMAILADDRESS            (const XCHAR* url);
                      DIOEMAILADDRESS            (const XCHAR* url, XDWORD size);
                      DIOEMAILADDRESS            (DIOEMAILADDRESS& url);
                      DIOEMAILADDRESS            (XSTRING& url);
                      DIOEMAILADDRESS            (XWORD* url);

    using             XSTRING::operator =;
    using             XSTRING::operator +=;
    using             XSTRING::operator <;
    using             XSTRING::operator >;
    using             XSTRING::operator <=;
    using             XSTRING::operator >=;
    using             XSTRING::operator ==;
    using             XSTRING::operator !=;
    using             XSTRING::operator [];

    bool              GetUser                    (XSTRING& user);
    bool              GetDomain                  (XSTRING& domain);
    bool              IsValid                    ();

  private:

    void              Clean                      ();

};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





