/**-------------------------------------------------------------------------------------------------------------------
*
* @file       CipherPEMCodec.h
*
* @class      CIPHERPEMCODEC
* @brief      Cipher PEM/DER private key codec class
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
#include "XVector.h"
#include "XMPInteger.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class CIPHERPEMCODEC
{
  public:

    
    static bool             IsPEM                              (XBUFFER& filedata);

    static bool              PrivateKeyBlock_Decode            (XVECTOR<XSTRING*>& lines, XSTRING& blockheader, XBUFFER& der);

    static bool              RSAPrivateKey_Decode              (XBUFFER& der, XMPINTEGER& prime1, XMPINTEGER& prime2, XMPINTEGER& exponent);

    static bool              ECPrivateKey_Decode               (XBUFFER& der, XBUFFER& privatekey);
    static bool              Ed25519PrivateKey_Decode          (XBUFFER& der, XBUFFER& privatekey);

    static bool              PKCS8PrivateKey_Decode            (XBUFFER& der, bool& isrsa, bool& isec, bool& ised25519, XBUFFER& innerkey);
    static bool              PKCS8EncryptedPrivateKey_Decode   (XBUFFER& der, XSTRING& password, XSECUREBUFFER& privatekeyinfo);
    static bool              PKCS7Certificates_Decode          (XBUFFER& der, XVECTOR<XBUFFER*>& certificates);
    static bool              PKCS12_Decode                     (XBUFFER& der, XSTRING& password, XVECTOR<XBUFFER*>& certificates, XSECUREBUFFER& privatekeyinfo);

  public:

    static bool              DER_ReadLength                    (XBYTE* data, XDWORD size, XDWORD& index, XDWORD& length);
    static bool              DER_ReadTagLength                 (XBYTE* data, XDWORD size, XDWORD& index, XBYTE& tag, XDWORD& valueoffset, XDWORD& valuelength);
    static bool              DER_ReadInteger                   (XBYTE* data, XDWORD size, XDWORD& index, XMPINTEGER& value);
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
