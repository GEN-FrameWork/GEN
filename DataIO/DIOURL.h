/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOURL.h
* 
* @class      DIOURL
* @brief      Data Input/Output URL class
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

#include "DIOIP.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOURL_WEBURLID        _L("http://")
#define DIOURL_WEBURLID_SECURE _L("https://")

enum DIOURL_HOSTTYPE
{
  DIOURL_HOSTTYPE_UNKNOWN = 0,
  DIOURL_HOSTTYPE_DNS,
  DIOURL_HOSTTYPE_IPV4,
  DIOURL_HOSTTYPE_IPV6,
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOURL : public XSTRING
{
  public:
                              DIOURL                            ();
    virtual                  ~DIOURL                            ();

                              DIOURL                            (XDWORD size);
                              DIOURL                            (const char* url);
                              DIOURL                            (const XCHAR* url);
                              DIOURL                            (const XCHAR* url, XDWORD size);
                              DIOURL                            (DIOURL& url);
                              DIOURL                            (XSTRING& url);
                              DIOURL                            (XWORD* url);

    using                     XSTRING::operator =;
    using                     XSTRING::operator +=;
    using                     XSTRING::operator <;
    using                     XSTRING::operator >;
    using                     XSTRING::operator <=;
    using                     XSTRING::operator >=;
    using                     XSTRING::operator ==;
    using                     XSTRING::operator !=;
    using                     XSTRING::operator [];

    bool                      GetHTTPServer                     (XSTRING& server, XSTRING& login, XSTRING& password);
    bool                      GetHTTPResource                   (XSTRING& resource);

    bool                      IsAURL                            ();

    bool                      ResolveURL                        ();
    bool                      ResolveURL                        (XSTRING& IP);
    bool                      ResolveURL                        (DIOIP& IP);

    bool                      IsLocalAddress                    ();

    bool                      Slash_Add                         ();
    bool                      Slash_Normalize                   ();
    bool                      Slash_Delete                      ();

    bool                      IsUnsafeChar                      (XCHAR character);
    bool                      EncodeUnsafeChar                  (XCHAR character, XSTRING& encodechar);

    bool                      EncodeUnsafeCharsFromString       (XSTRING& string);
    bool                      EncodeUnsafeCharsFromString       (const char* string);
    bool                      EncodeUnsafeCharsFromString       (const XCHAR* string);
    bool                      EncodeUnsafeCharsFromBuffer       (XBUFFER& buffer);

    bool                      EncodeUnsafeChars                 ();

    bool                      DecodeUnsafeCharsToString         (XSTRING& string);
    bool                      DecodeUnsafeChars                 ();

    bool                      Normalize                         ();

    bool                      HaveHTTPID                        ();
    bool                      AddHTTPID                         ();
    bool                      DeleteHTTPID                      ();

    bool                      GetExtension                      (XSTRING& extension);
    bool                      GetFileName                       (XSTRING& filename);

    static DIOURL_HOSTTYPE    Host_GetType                     (XCHAR* host);
    static bool               Host_Canonicalize                (XCHAR* host, XSTRING& canonicalhost, DIOURL_HOSTTYPE& type);

  private:

    void                      Clean                             ();

};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




