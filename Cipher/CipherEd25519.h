/**-------------------------------------------------------------------------------------------------------------------
*
* @file       CipherEd25519.h
*
* @class      CIPHERED25519
* @brief      Ed25519 signature algorithm (RFC 8032)
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

#include "XBuffer.h"

#define CIPHERED25519_PRIVATEKEYSIZE 32
#define CIPHERED25519_PUBLICKEYSIZE  32
#define CIPHERED25519_SIGNATURESIZE  64

class CIPHERED25519
{
  public:
                          CIPHERED25519       ();
    virtual              ~CIPHERED25519       ();

    bool                  KeyPair_Create      (XBUFFER& privatekey, XBUFFER& publickey);
    bool                  PublicKey_Create    (XBUFFER& privatekey, XBUFFER& publickey);
    bool                  PublicKey_IsValid   (XBUFFER& publickey);
    bool                  Sign                (XBUFFER& privatekey, XBUFFER& publickey, XBUFFER& input, XBUFFER& signature);
    bool                  Verify              (XBUFFER& publickey, XBUFFER& input, XBUFFER& signature);

  private:
    bool                  Random              (XBYTE* data, XDWORD size);
    void                  Clean               ();
};
