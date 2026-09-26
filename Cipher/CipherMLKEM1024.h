/**-------------------------------------------------------------------------------------------------------------------
*
* @file       CipherMLKEM1024.h
*
* @class      CIPHERMLKEM1024
* @brief      ML-KEM-1024 (FIPS 203) key encapsulation mechanism
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
#include "CipherMLKEM1024Core.h"

class CIPHERMLKEM1024
{
  public:
                          CIPHERMLKEM1024       ();
    virtual              ~CIPHERMLKEM1024       ();

    bool                  KeyPair_Create       (XBUFFER& publickey, XBUFFER& privatekey);
    bool                  Encapsulate          (XBUFFER& publickey, XBUFFER& ciphertext, XBUFFER& sharedsecret);
    bool                  Decapsulate          (XBUFFER& privatekey, XBUFFER& ciphertext, XBUFFER& sharedsecret);
    bool                  PublicKey_Check      (XBUFFER& publickey);

  private:
    bool                  Random               (XBYTE* data, XDWORD size);
    void                  Clean                ();
};
