/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       Cipher3DES.h
* 
* @class      CIPHER3DES
* @brief      Cipher 3DES class
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

#include "CipherDES.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

typedef struct
{
  int     mode;
  XDWORD  sk[96];

} CIPHER3DES_CONTEXT;

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class CIPHER3DES : public CIPHERDES
{
  public:

                          CIPHER3DES              ();
    virtual              ~CIPHER3DES              ();

    using                 CIPHER::Cipher;
    using                 CIPHER::Uncipher;

    bool                  Cipher                  (XBYTE* input, XDWORD size);
    bool                  Uncipher                (XBYTE* input, XDWORD size);

  private:
  
    void                  DES3Set2Key             (XDWORD esk[96], XDWORD dsk[96], XBYTE key[CIPHERDES_KEYSIZE*2]);
    int                   DES3Set2KeyCipher       (CIPHER3DES_CONTEXT* ctx, XBYTE key[CIPHERDES_KEYSIZE*2]);
    int                   DES3Set2KeyUncipher     (CIPHER3DES_CONTEXT *ctx, XBYTE key[CIPHERDES_KEYSIZE*2]);

    void                  DES3Set3Key             (XDWORD esk[96], XDWORD dsk[96], XBYTE key[24]);
    int                   DES3Set3KeyCipher       (CIPHER3DES_CONTEXT* ctx, XBYTE key[CIPHERDES_KEYSIZE*3]);
    int                   DES3Set3KeyUncipher     (CIPHER3DES_CONTEXT* ctx, XBYTE key[CIPHERDES_KEYSIZE*3]);

    int                   DES3Cipher              (CIPHER3DES_CONTEXT* ctx, XBYTE input[8], XBYTE output[8]);

    bool                  DES3Cipher_ECB          (CIPHER3DES_CONTEXT* ctx, int mode, size_t length, XBYTE iv[8], XBYTE* input, XBYTE* output);
    bool                  DES3Cipher_CBC          (CIPHER3DES_CONTEXT* ctx, int mode, size_t length, XBYTE iv[8], XBYTE* input, XBYTE* output);

    void                  Clean                   ();
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





