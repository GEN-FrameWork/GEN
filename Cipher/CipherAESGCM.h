/**-------------------------------------------------------------------------------------------------------------------
*
* @file       CipherAESGCM.h
*
* @class      CIPHERAESGCM
* @brief      Cipher AES GCM (Galois/Counter Mode, NIST SP 800-38D) class
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

#include "XBuffer.h"

#include "CipherAES.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define CIPHERAESGCM_BLOCKSIZE                16
#define CIPHERAESGCM_TABLESIZE                16

#define CIPHERAESGCM_TAGSIZE                  16
#define CIPHERAESGCM_TAGSIZE_MIN               4

#define CIPHERAESGCM_NONCESIZE                12                                // NIST SP 800-38D recommended, and the only one TLS 1.3 uses

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class CIPHERAESGCM : public CIPHERAES
{
  public:
                          CIPHERAESGCM            ();
    virtual              ~CIPHERAESGCM           ();

    bool                  SetKey                  (CIPHERKEY* key, bool integritycheck = false);

    bool                  IsAEAD                  ();
    XDWORD                GetAEADNonceSize        ();
    XDWORD                GetAEADTagSize          ();
    bool                  SetAEADTagSize          (XDWORD tagsize);

    using                 CIPHER::CipherAEAD;
    using                 CIPHER::UncipherAEAD;

    bool                  CipherAEAD              (XBYTE* input, XDWORD size, XBUFFER& nonce, XBUFFER& additionaldata, XBUFFER& tag);
    bool                  UncipherAEAD            (XBYTE* input, XDWORD size, XBUFFER& nonce, XBUFFER& additionaldata, XBUFFER& tag);

  private:

    bool                  GCM_GenerateTable       ();
    void                  GCM_Multiplication      (XBYTE* x, XBYTE* output);

    bool                  GCM_Start               (XBUFFER& nonce, XBUFFER& additionaldata);
    bool                  GCM_Update              (XBYTE* input, XDWORD size, XBYTE* output, bool iscipher);
    bool                  GCM_Finish              (XBYTE* tag);

    bool                  GCM_Do                  (XBYTE* input, XDWORD size, XBUFFER& nonce, XBUFFER& additionaldata, XBUFFER& tag, bool iscipher);

    void                  Clean                   ();

    CIPHERAES_CONTEXT     ctx;
    bool                  keyisready;
    XDWORD                tagsize;

    XQWORD                HL[CIPHERAESGCM_TABLESIZE];
    XQWORD                HH[CIPHERAESGCM_TABLESIZE];

    XBYTE                 basectr[CIPHERAESGCM_BLOCKSIZE];
    XBYTE                 counter[CIPHERAESGCM_BLOCKSIZE];
    XBYTE                 ghash[CIPHERAESGCM_BLOCKSIZE];

    XQWORD                sizedata;
    XQWORD                sizeadditionaldata;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/



