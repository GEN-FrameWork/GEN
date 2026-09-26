/**-------------------------------------------------------------------------------------------------------------------
*
* @file       CipherX25519MLKEM768.h
*
* @class      CIPHERX25519MLKEM768
* @brief      X25519MLKEM768 hybrid key agreement (RFC 10024)
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
#include "CipherECDSAX25519.h"
#include "CipherMLKEM768.h"

#define CIPHERX25519MLKEM768_CLIENTSHARESIZE  1216
#define CIPHERX25519MLKEM768_SERVERSHARESIZE  1120
#define CIPHERX25519MLKEM768_SHAREDSECRETSIZE 64

class CIPHERX25519MLKEM768
{
  public:
                          CIPHERX25519MLKEM768          ();
    virtual              ~CIPHERX25519MLKEM768          ();

    bool                  ClientKeyShare_Create         (XBUFFER& clientshare);
    bool                  ClientSharedSecret_Create     (XBUFFER& servershare, XBUFFER& sharedsecret, bool* invalidpeershare = NULL);
    bool                  ServerKeyShare_Create         (XBUFFER& clientshare, XBUFFER& servershare, XBUFFER& sharedsecret, bool* invalidpeershare = NULL);
    void                  Delete                        ();

  private:
    bool                  X25519KeyPair_Create          (CIPHERECDSAX25519& x25519, XBUFFER& publickey);
    bool                  X25519SharedSecret_Create     (CIPHERECDSAX25519& x25519, XBYTE* peerpublic, XBUFFER& sharedsecret, bool* invalidpeershare);
    void                  Clean                         ();

    CIPHERECDSAX25519      x25519;
    CIPHERMLKEM768         mlkem;
    XSECUREBUFFER          mlkemprivate;
};
