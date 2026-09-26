/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       CipherDES.h
* 
* @class      CIPHERDES
* @brief      Cipher DES class
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

#define CIPHERDES_ENCRYPT             1
#define CIPHERDES_DECRYPT             0

#define CIPHERDES_KEYSIZE             8
#define CIPHERDES_WEAKKEYCOUNT        16


#define GET_XDWORD_BE(n,b,i)  { (n) =     ( (XDWORD) (b)[(i)    ] << 24 )                    \
                                        | ( (XDWORD) (b)[(i) + 1] << 16 )                    \
                                        | ( (XDWORD) (b)[(i) + 2] <<  8 )                    \
                                        | ( (XDWORD) (b)[(i) + 3]       );                   \
                              }

#define PUT_XDWORD_BE(n,b,i)  {  (b)[(i)    ] = (XBYTE) ( (n) >> 24 );                       \
                                 (b)[(i) + 1] = (XBYTE) ( (n) >> 16 );                       \
                                 (b)[(i) + 2] = (XBYTE) ( (n) >>  8 );                       \
                                 (b)[(i) + 3] = (XBYTE) ( (n)       );                       \
                              }


// Initial Permutation macro
#define DES_IP(X,Y)           {  T = ((X >>  4) ^ Y) & 0x0F0F0F0F; Y ^= T; X ^= (T <<  4);   \
                                 T = ((X >> 16) ^ Y) & 0x0000FFFF; Y ^= T; X ^= (T << 16);   \
                                 T = ((Y >>  2) ^ X) & 0x33333333; X ^= T; Y ^= (T <<  2);   \
                                 T = ((Y >>  8) ^ X) & 0x00FF00FF; X ^= T; Y ^= (T <<  8);   \
                                 Y = ((Y << 1) | (Y >> 31)) & 0xFFFFFFFF;                    \
                                 T = (X ^ Y) & 0xAAAAAAAA; Y ^= T; X ^= T;                   \
                                 X = ((X << 1) | (X >> 31)) & 0xFFFFFFFF;                    \
                              }
// Final Permutation macro

#define DES_FP(X,Y)           {  X = ((X << 31) | (X >> 1)) & 0xFFFFFFFF;                    \
                                 T = (X ^ Y) & 0xAAAAAAAA; X ^= T; Y ^= T;                   \
                                 Y = ((Y << 31) | (Y >> 1)) & 0xFFFFFFFF;                    \
                                 T = ((Y >>  8) ^ X) & 0x00FF00FF; X ^= T; Y ^= (T <<  8);   \
                                 T = ((Y >>  2) ^ X) & 0x33333333; X ^= T; Y ^= (T <<  2);   \
                                 T = ((X >> 16) ^ Y) & 0x0000FFFF; Y ^= T; X ^= (T << 16);   \
                                 T = ((X >>  4) ^ Y) & 0x0F0F0F0F; Y ^= T; X ^= (T <<  4);   \
                              }


// DES round macro
#define DES_ROUND(X,Y)        {  T = *SK++ ^ X;                                              \
                                 Y ^= SB8[ (T      ) & 0x3F ] ^                              \
                                      SB6[ (T >>  8) & 0x3F ] ^                              \
                                      SB4[ (T >> 16) & 0x3F ] ^                              \
                                      SB2[ (T >> 24) & 0x3F ];                               \
                                                                                             \
                                 T = *SK++ ^ ((X << 28) | (X >> 4));                         \
                                 Y ^= SB7[ (T      ) & 0x3F ] ^                              \
                                      SB5[ (T >>  8) & 0x3F ] ^                              \
                                      SB3[ (T >> 16) & 0x3F ] ^                              \
                                      SB1[ (T >> 24) & 0x3F ];                               \
                              }

#define SWAP(a,b)             { XDWORD t = a; a = b; b = t; t = 0; }

typedef struct  
{ 
  int     mode;
  XDWORD  sk[32];

} CIPHERDES_CONTEXT;

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class CIPHERDES : public CIPHER
{
  public:

                          CIPHERDES               ();
    virtual              ~CIPHERDES               ();

    using                 CIPHER::Cipher;
    using                 CIPHER::Uncipher;

    bool                  Cipher                  (XBYTE* input, XDWORD size);
    bool                  Uncipher                (XBYTE* input, XDWORD size);

  protected:

    static XDWORD         SB1[64];
    static XDWORD         SB2[64];
    static XDWORD         SB3[64];
    static XDWORD         SB4[64];
    static XDWORD         SB5[64];
    static XDWORD         SB6[64];
    static XDWORD         SB7[64];
    static XDWORD         SB8[64];
    static XDWORD         LHs[16];
    static XDWORD         RHs[16];
    static XBYTE          oddparitytable[128];
    static XBYTE          weakkeytable[CIPHERDES_WEAKKEYCOUNT][CIPHERDES_KEYSIZE];


    void                  DESKeySetParity         (XBYTE key[CIPHERDES_KEYSIZE]);
    int                   DESKeyCheckKeyParity    (XBYTE key[CIPHERDES_KEYSIZE]);
    int                   DESKeyCheckWeak         (XBYTE key[CIPHERDES_KEYSIZE]);

    void                  DESSetKey               (XDWORD SK[32], XBYTE key[CIPHERDES_KEYSIZE]);
    int                   DESSetKeyCipher         (CIPHERDES_CONTEXT* ctx, XBYTE key[CIPHERDES_KEYSIZE]);
    int                   DESSetKeyUncipher       (CIPHERDES_CONTEXT* ctx, XBYTE key[CIPHERDES_KEYSIZE]);

    int                   DESCipher               (CIPHERDES_CONTEXT* ctx, XBYTE input[8], XBYTE output[8]);

    bool                  DESCipher_ECB           (CIPHERDES_CONTEXT* ctx, int mode, size_t length, XBYTE iv[8], XBYTE* input, XBYTE* output);
    bool                  DESCipher_CBC           (CIPHERDES_CONTEXT* ctx, int mode, size_t length, XBYTE iv[8], XBYTE* input, XBYTE* output);

  private:

    void                  Clean                   ();
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





