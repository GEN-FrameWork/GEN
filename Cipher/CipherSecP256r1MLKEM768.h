/**-------------------------------------------------------------------------------------------------------------------
*
* @file       CipherSecP256r1MLKEM768.h
*
* @class      CIPHERSECP256R1MLKEM768
* @brief      SecP256r1MLKEM768 hybrid key agreement (RFC 10024)
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
#include "CipherECDSA.h"
#include "CipherMLKEM768.h"

#define CIPHERSECP256R1MLKEM768_CLIENTSHARESIZE   1249
#define CIPHERSECP256R1MLKEM768_SERVERSHARESIZE   1153
#define CIPHERSECP256R1MLKEM768_SHAREDSECRETSIZE    64

class CIPHERSECP256R1MLKEM768
{
  public:
                          CIPHERSECP256R1MLKEM768         ();
    virtual              ~CIPHERSECP256R1MLKEM768         ();

    bool                  ClientKeyShare_Create           (XBUFFER& clientshare);
    bool                  ClientSharedSecret_Create       (XBUFFER& servershare, XBUFFER& sharedsecret, bool* invalidpeershare = NULL);
    bool                  ServerKeyShare_Create           (XBUFFER& clientshare, XBUFFER& servershare, XBUFFER& sharedsecret, bool* invalidpeershare = NULL);
    void                  Delete                          ();

  private:
    void                  Clean                           ();

    CIPHERECDSA           secp256r1;
    XSECUREBUFFER         secp256r1private;
    XBUFFER               secp256r1public;
    CIPHERMLKEM768        mlkem;
    XSECUREBUFFER         mlkemprivate;
};
