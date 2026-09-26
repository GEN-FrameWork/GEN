/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       Cipher.h
* 
* @class      CIPHER
* @brief      Cipher interface class
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

#include "CipherKey.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define CIPHERMAXKEYS 2

enum CIPHERTYPE
{
  CIPHERTYPE_XOR                      = 0 ,
  CIPHERTYPE_DES                          ,
  CIPHERTYPE_3DES                         ,
  CIPHERTYPE_BLOWFISH                     ,
  CIPHERTYPE_AES                          ,
  CIPHERTYPE_RSA                          ,
  CIPHERTYPE_ECDSA_SECP224R1	            ,
  CIPHERTYPE_ECDSA_SECP192R1	            ,
  CIPHERTYPE_ECDSA_SECP256R1	            ,
  CIPHERTYPE_ECDSA_SECP384R1	            ,
  CIPHERTYPE_ECDSA_SECP521R1	            ,
  CIPHERTYPE_ECDSA_SECP256K1	            ,
  CIPHERTYPE_ECDSA_SECT163K1	            ,
  CIPHERTYPE_ECDSA_BRAINPOOLP256R1	      ,
  CIPHERTYPE_ECDSA_BRAINPOOLP384R1	      ,
  CIPHERTYPE_ECDSA_BRAINPOOLP512R1	      ,
  CIPHERTYPE_ECDSA_X25519                 , // Not neccesary (Interchange Keys Only)
  CIPHERTYPE_CHACHA20_POLY1305            ,
  CIPHERTYPE_ED25519                      ,
};                                        

enum CIPHERCHAININGMODE
{
  CIPHERCHAININGMODE_UNKNOWN    = 0 ,
  CIPHERCHAININGMODE_ECB            ,
  CIPHERCHAININGMODE_CBC            ,
  CIPHERCHAININGMODE_CFB            ,
  CIPHERCHAININGMODE_CTR            ,
  CIPHERCHAININGMODE_GCM            ,
  CIPHERCHAININGMODE_POLY1305       ,
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class CIPHER
{
  public:
                          CIPHER                    ();
    virtual              ~CIPHER                    ();

    CIPHERTYPE            GetType                   ();
    void                  SetType                   (CIPHERTYPE type);

    CIPHERCHAININGMODE    GetChainingMode           ();
    void                  SetChainingMode           (CIPHERCHAININGMODE chainingmode);

    XBUFFER_PADDINGTYPE   GetPaddingType            ();
    void                  SetPaddingType            (XBUFFER_PADDINGTYPE paddingtype);

    int                   GetPaddingAdjustSize      ();
    void                  SetPaddingAdjustSize      (int paddingadjustsize);

    CIPHERKEY*            GetKey                    (CIPHERKEYTYPE type = CIPHERKEYTYPE_SYMMETRICAL);
    virtual bool          SetKey                    (CIPHERKEY* key, bool integritycheck = false);

    XBYTE*                GetInitVector             ();
    bool                  SetInitVector             (XBYTE* inivector, XDWORD size);
    bool                  SetInitVector             (XBUFFER& inivector);

    virtual bool          Cipher                    (XBYTE* input, XDWORD size);
    bool                  Cipher                    (XBUFFER& input);

    virtual bool          Uncipher                  (XBYTE* input, XDWORD size);
    bool                  Uncipher                  (XBUFFER& input);

    virtual bool          IsAEAD                    ();
    virtual XDWORD        GetAEADNonceSize          ();
    virtual XDWORD        GetAEADTagSize            ();

    virtual bool          CipherAEAD                (XBYTE* input, XDWORD size, XBUFFER& nonce, XBUFFER& additionaldata, XBUFFER& tag);
    bool                  CipherAEAD                (XBUFFER& input, XBUFFER& nonce, XBUFFER& additionaldata, XBUFFER& tag);

    virtual bool          UncipherAEAD              (XBYTE* input, XDWORD size, XBUFFER& nonce, XBUFFER& additionaldata, XBUFFER& tag);
    bool                  UncipherAEAD              (XBUFFER& input, XBUFFER& nonce, XBUFFER& additionaldata, XBUFFER& tag);

    virtual XBYTE*        GetResult                 (int& resultsize);
    XBUFFER*              GetResult                 ();
    bool                  GetResultString           (XSTRING& stringhex);

    static bool           CompareConstantTime       (XBYTE* data1, XBYTE* data2, XDWORD size);
    static bool           CompareConstantTime       (XBUFFER& data1, XBUFFER& data2);

  protected:

    CIPHERTYPE            type;
    CIPHERCHAININGMODE    chainingmode;
    XBUFFER_PADDINGTYPE   paddingtype;
    int                   paddingadjustsize;

    CIPHERKEY*            keys[CIPHERMAXKEYS];
    XBUFFER*              inivector;
    XBUFFER*              result;

  private:
    
    CIPHERKEY*            GetKey                    (int index);
    bool                  SetKey                    (int index, CIPHERKEY* key);

    void                  Clean                     ();

};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





