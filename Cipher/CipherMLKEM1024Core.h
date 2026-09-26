/**-------------------------------------------------------------------------------------------------------------------
*
* @file       CipherMLKEM1024Core.h
*
* @class      CIPHERMLKEM1024CORE
* @brief      CIPHERMLKEM1024CORE class
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

#include <stdint.h>
#include <stddef.h>

#define CIPHERMLKEM1024_PUBLICKEYSIZE      1568
#define CIPHERMLKEM1024_PRIVATEKEYSIZE     3168
#define CIPHERMLKEM1024_CIPHERTEXTSIZE     1568
#define CIPHERMLKEM1024_SHAREDSECRETSIZE   32
#define CIPHERMLKEM1024_SEEDSIZE           32

class CIPHERMLKEM1024CORE
{
  public:
    static bool KeyPair(const uint8_t d[32], const uint8_t z[32], uint8_t publickey[CIPHERMLKEM1024_PUBLICKEYSIZE], uint8_t privatekey[CIPHERMLKEM1024_PRIVATEKEYSIZE]);
    static bool Encapsulate(const uint8_t randomness[32], const uint8_t publickey[CIPHERMLKEM1024_PUBLICKEYSIZE], uint8_t ciphertext[CIPHERMLKEM1024_CIPHERTEXTSIZE], uint8_t sharedsecret[CIPHERMLKEM1024_SHAREDSECRETSIZE]);
    static bool Decapsulate(const uint8_t privatekey[CIPHERMLKEM1024_PRIVATEKEYSIZE], const uint8_t ciphertext[CIPHERMLKEM1024_CIPHERTEXTSIZE], uint8_t sharedsecret[CIPHERMLKEM1024_SHAREDSECRETSIZE]);
    static bool PublicKey_Check(const uint8_t publickey[CIPHERMLKEM1024_PUBLICKEYSIZE]);
};
