/**-------------------------------------------------------------------------------------------------------------------
*
* @file       CipherHKDF.h
*
* @class      CIPHERHKDF
* @brief      Cipher HKDF (HMAC-based Extract-and-Expand Key Derivation Function, RFC 5869) class
* @ingroup    CIPHER
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

#include "XBuffer.h"
#include "XString.h"

#include "Hash.h"
#include "HashHMAC.h"

#include "Cipher.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define CIPHERHKDF_MAXEXPANDBLOCKS          255
#define CIPHERHKDF_MAXLABELSIZE             255
#define CIPHERHKDF_MAXCONTEXTSIZE           255

#define CIPHERHKDF_LABELPREFIX_TLS13        _L("tls13 ")
#define CIPHERHKDF_LABELPREFIX_QUIC         _L("quic ")

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class CIPHERHKDF : public CIPHER
{
  public:
                                    CIPHERHKDF                            (HASH* hash = NULL);
    virtual                        ~CIPHERHKDF                            ();

    HASH*                           GetHash                               ();
    bool                            SetHash                               (HASH* hash);

    XDWORD                          GetHashSize                           ();

    XSTRING*                        GetLabelPrefix                        ();
    bool                            SetLabelPrefix                        (XCHAR* labelprefix);

    bool                            Extract                               (XBUFFER& salt, XBUFFER& inputkeymaterial, XBUFFER& pseudorandomkey);
    bool                            Expand                                (XBUFFER& pseudorandomkey, XBUFFER& info, XDWORD size, XBUFFER& outputkeymaterial);

    bool                            ExpandLabel                           (XBUFFER& secret, XCHAR* label, XBUFFER& context, XDWORD size, XBUFFER& outputkeymaterial);
    bool                            DeriveSecret                          (XBUFFER& secret, XCHAR* label, XBUFFER& messages, XBUFFER& outputsecret);

  private:

    bool                            MakeHKDFLabel                         (XCHAR* label, XBUFFER& context, XDWORD size, XBUFFER& hkdflabel);

    void                            Clean                                 ();

    HASH*                           hash;
    HASHHMAC*                       hashhmac;
    XSTRING                         labelprefix;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/



