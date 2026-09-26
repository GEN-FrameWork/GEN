/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       CipherBlowfish.h
* 
* @class      CIPHERBLOWFISH
* @brief      Cipher Blowfish class
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

#include "Cipher.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define CIPHERBLOWFISH_ENCRYPT     1
#define CIPHERBLOWFISH_DECRYPT     0
#define CIPHERBLOWFISH_MAX_KEY     448
#define CIPHERBLOWFISH_MIN_KEY     32
#define CIPHERBLOWFISH_ROUNDS      16         // when increasing this value, make sure to extend the initialisation vectors 
#define CIPHERBLOWFISH_BLOCKSIZE   8          // Blowfish uses 64 bit blocks 

typedef struct  
{ 
  XDWORD  P[CIPHERBLOWFISH_ROUNDS + 2];
  XDWORD  S[4][256];

} CIPHERBLOWFISH_CONTEXT;

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class CIPHERBLOWFISH : public CIPHER
{
  public:

                          CIPHERBLOWFISH          ();
    virtual              ~CIPHERBLOWFISH          ();

    using                 CIPHER::Cipher;
    using                 CIPHER::Uncipher;

    bool                  Cipher                  (XBYTE* input, XDWORD size);
    bool                  Uncipher                (XBYTE* input, XDWORD size);

  private:

    void                  Clean                   ();

    static XDWORD         P[CIPHERBLOWFISH_ROUNDS + 2];
    static XDWORD         S[4][256];

    XDWORD                F                       (CIPHERBLOWFISH_CONTEXT* ctx, XDWORD x);

    void                  BlowfishCipher          (CIPHERBLOWFISH_CONTEXT* ctx, XDWORD* xl, XDWORD* xr);
    void                  BlowfishUncipher        (CIPHERBLOWFISH_CONTEXT* ctx, XDWORD* xl, XDWORD* xr) ;

    bool                  BlowfishSetKey          (CIPHERBLOWFISH_CONTEXT* ctx, XBYTE* key, XDWORD keysize);

    bool                  BlowfishCipher_ECB      (CIPHERBLOWFISH_CONTEXT* ctx, int mode, XBYTE input[CIPHERBLOWFISH_BLOCKSIZE], XBYTE output[CIPHERBLOWFISH_BLOCKSIZE]);
    bool                  BlowfishCipher_CBC      (CIPHERBLOWFISH_CONTEXT* ctx, int mode, XDWORD size, XBYTE iv[CIPHERBLOWFISH_BLOCKSIZE], XBYTE* input, XBYTE* output);
    bool                  BlowfishCipher_CFB64    (CIPHERBLOWFISH_CONTEXT* ctx, int mode, XDWORD size, int* iv_off, XBYTE iv[CIPHERBLOWFISH_BLOCKSIZE], XBYTE* input, XBYTE* output);
    bool                  BlowfishCipher_CTR      (CIPHERBLOWFISH_CONTEXT* ctx, XDWORD size, int* nc_off, XBYTE nonce_counter[CIPHERBLOWFISH_BLOCKSIZE], XBYTE stream_block[CIPHERBLOWFISH_BLOCKSIZE], XBYTE* input, XBYTE* output);
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





