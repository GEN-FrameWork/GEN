/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       CipherAES.h
* 
* @class      CIPHERAES
* @brief      Cipher AES class
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

#define CIPHERAES_ENCRYPT     1
#define CIPHERAES_DECRYPT     0

typedef struct  
{ 
  int       nr;
  XDWORD*   rk;
  XDWORD    buf[68];

} CIPHERAES_CONTEXT;

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class CIPHERAES : public CIPHER
{
  public:

                          CIPHERAES               ();
    virtual              ~CIPHERAES               ();

    using                 CIPHER::Cipher;
    using                 CIPHER::Uncipher;

    bool                  Cipher                  (XBYTE* input, XDWORD size);
    bool                  Uncipher                (XBYTE* input, XDWORD size);

  protected:

    void                  AESGenTables            (void);

    bool                  AESSetKeyCipher         (CIPHERAES_CONTEXT* ctx, XBYTE* key, XDWORD keysize);
    bool                  AESSetKeyUncipher       (CIPHERAES_CONTEXT* ctx, XBYTE* key, XDWORD keysize);

    bool                  AESCipher_ECB_Block     (CIPHERAES_CONTEXT* ctx, int mode, XBYTE input[16], XBYTE output[16]);

    bool                  AESCipher_ECB           (CIPHERAES_CONTEXT* ctx, int mode, XDWORD size, XBYTE* input, XBYTE* output);
    bool                  AESCipher_CBC           (CIPHERAES_CONTEXT* ctx, int mode, XDWORD size, XBYTE iv[16], XBYTE* input, XBYTE* output);
    bool                  AESCipher_CFB128        (CIPHERAES_CONTEXT* ctx, int mode, XDWORD size, int* iv_off, XBYTE iv[16], XBYTE* input, XBYTE* output);
    bool                  AESCipher_CTR           (CIPHERAES_CONTEXT* ctx, XDWORD size, int* nc_off, XBYTE nonce_counter[16], XBYTE stream_block[16], XBYTE* input, XBYTE* output);

    void                  Clean                   ();

    static XBYTE          FSb[256];
    static XDWORD         FT0[256];
    static XDWORD         FT1[256];
    static XDWORD         FT2[256];
    static XDWORD         FT3[256];

    static XBYTE          RSb[256];
    static XDWORD         RT0[256];
    static XDWORD         RT1[256];
    static XDWORD         RT2[256];
    static XDWORD         RT3[256];

    static XDWORD         RCON[10];

    static int            initdone;
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




