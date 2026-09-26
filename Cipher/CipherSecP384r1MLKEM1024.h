/**-------------------------------------------------------------------------------------------------------------------
*
* @file       CipherSecP384r1MLKEM1024.h
*
* @class      CIPHERSECP384R1MLKEM1024
* @brief      SecP384r1MLKEM1024 hybrid key agreement (RFC 10024)
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
#include "CipherMLKEM1024.h"

#define CIPHERSECP384R1MLKEM1024_CLIENTSHARESIZE   1665
#define CIPHERSECP384R1MLKEM1024_SERVERSHARESIZE   1665
#define CIPHERSECP384R1MLKEM1024_SHAREDSECRETSIZE    80

class CIPHERSECP384R1MLKEM1024
{
  public:
                          CIPHERSECP384R1MLKEM1024         ();
    virtual              ~CIPHERSECP384R1MLKEM1024         ();

    bool                  ClientKeyShare_Create           (XBUFFER& clientshare);
    bool                  ClientSharedSecret_Create       (XBUFFER& servershare, XBUFFER& sharedsecret, bool* invalidpeershare = NULL);
    bool                  ServerKeyShare_Create           (XBUFFER& clientshare, XBUFFER& servershare, XBUFFER& sharedsecret, bool* invalidpeershare = NULL);
    void                  Delete                          ();

  private:
    void                  Clean                           ();

    CIPHERECDSA           secp384r1;
    XSECUREBUFFER         secp384r1private;
    XBUFFER               secp384r1public;
    CIPHERMLKEM1024        mlkem;
    XSECUREBUFFER         mlkemprivate;
};
