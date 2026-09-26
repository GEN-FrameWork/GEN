/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       CipherRSA.h
* 
* @class      CIPHERRSA
* @brief      Cipher RSA class
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

#include "XMPInteger.h"

#include "CipherKeyPublicRSA.h"
#include "CipherKeyPrivateRSA.h"
#include "Cipher.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum CIPHERRSAPKCS1VERSION
{
  CIPHERRSAPKCS1VERSIONV15    = 0 ,
  CIPHERRSAPKCS1VERSIONV21
};

#define CIPHERRSA_SINGDATA        1
#define CIPHERRSA_CIPHERDATA      2

// Minimum RSA modulus size accepted by GenerateKeys(). 2048 bits matches the minimum this framework already
// enforces on *received* certificates (CIPHERCERTIFICATEX509VALIDATOR's default minimumRSAKeyBits), so a key
// generated here can never be weaker than what the validator itself would accept from a peer. Smaller sizes
// (e.g. 512/1024 bits) are practically factorable today and must never be produced for real use.
#define CIPHERRSA_MINKEYBITS      2048

//#define CIPHERRSA_RSANOCRT

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XRAND;
class HASH;

class CIPHERRSA_CONTEXT
{
  public:
                                    CIPHERRSA_CONTEXT                 ();  
    virtual                        ~CIPHERRSA_CONTEXT                 ();
                                    
    void                            Ini                               ();
    
    void                            End                               ();
    
    void                            Clean                             ();
                                    
    size_t                          len;

    XMPINTEGER                      N;
    XMPINTEGER                      E;

    XMPINTEGER                      D;
    XMPINTEGER                      P;
    XMPINTEGER                      Q;
    XMPINTEGER                      DP;
    XMPINTEGER                      DQ;
    XMPINTEGER                      QP;

    XMPINTEGER                      RN;
    XMPINTEGER                      RP;
    XMPINTEGER                      RQ;

    #if !defined(CIPHERRSA_RSANOCRT)
    XMPINTEGER                      Vi;
    XMPINTEGER                      Vf;
    #endif

    int                             padding;
    int                             hashID;
};

class CIPHERRSA : public CIPHER
{
  public:

                                    CIPHERRSA                         ();
    virtual                        ~CIPHERRSA                         ();

    using                           CIPHER::Cipher;
    using                           CIPHER::Uncipher;

    bool                            SetKey                            (CIPHERKEY* key, bool integritycheck = false);

    bool                            Cipher                            (XBYTE* input, XDWORD size, CIPHERKEYTYPE keytouse, CIPHERRSAPKCS1VERSION pkcs1version = CIPHERRSAPKCS1VERSIONV15);
    bool                            Cipher                            (XBUFFER& input, CIPHERKEYTYPE keytouse, CIPHERRSAPKCS1VERSION pkcs1version = CIPHERRSAPKCS1VERSIONV15);

    bool                            Uncipher                          (XBYTE* input, XDWORD size, CIPHERKEYTYPE keytouse, CIPHERRSAPKCS1VERSION pkcs1version = CIPHERRSAPKCS1VERSIONV15);
    bool                            Uncipher                          (XBUFFER& input, CIPHERKEYTYPE keytouse, CIPHERRSAPKCS1VERSION pkcs1version = CIPHERRSAPKCS1VERSIONV15);

    bool                            Sign                              (XBYTE* input, XDWORD size, CIPHERKEYTYPE keytouse, HASH* hash, CIPHERRSAPKCS1VERSION pkcs1version = CIPHERRSAPKCS1VERSIONV15);
    bool                            Sign                              (XBUFFER& input, CIPHERKEYTYPE keytouse, HASH* hash, CIPHERRSAPKCS1VERSION pkcs1version = CIPHERRSAPKCS1VERSIONV15);

    bool                            Verify                            (XBYTE* input, XDWORD size, XBUFFER& signature, HASH* hash, CIPHERRSAPKCS1VERSION pkcs1version = CIPHERRSAPKCS1VERSIONV15, XDWORD saltsize = 0);
    bool                            Verify                            (XBUFFER& input, XBUFFER& signature, HASH* hash, CIPHERRSAPKCS1VERSION pkcs1version = CIPHERRSAPKCS1VERSIONV15, XDWORD saltsize = 0);

    XRAND*                          GetXRand                          ();
    bool                            GenerateKeys                      (XDWORD nbits, int exponent, CIPHERKEYPUBLICRSA& publickey, CIPHERKEYPRIVATERSA& privatekey, XMPINTEGER_FUNCRANDOM funcrandom = NULL, void* paramrandom = NULL);

  private:

    bool                            CheckContextPublicKey             ();
    bool                            CheckContextPrivateKey            ();

    bool                            Cipher_PKCS1_V15                  (XBYTE* buffer, XDWORD size, XBUFFER& output, CIPHERKEYTYPE keytypetouse, CIPHERRSAPKCS1VERSION pkcs1version, XMPINTEGER_FUNCRANDOM funcrandom, void* paramrandom);
    bool                            Uncipher_PKCS1_V15                (XBYTE* buffer, XDWORD size, XBUFFER& output, CIPHERKEYTYPE keytypetouse, CIPHERRSAPKCS1VERSION pkcs1version, XMPINTEGER_FUNCRANDOM funcrandom, void* paramrandom);

    bool                            Sign_PKCS1_V15                     (XBYTE* input, XDWORD size, CIPHERKEYTYPE keytouse, HASH* hash);
    bool                            Sign_PKCS1_V21                     (XBYTE* input, XDWORD size, HASH* hash, XDWORD saltsize);

    bool                            Verify_PKCS1_V15                  (XBYTE* input, XDWORD size, XBUFFER& signature, HASH* hash);
    bool                            Verify_PKCS1_V21                  (XBYTE* input, XDWORD size, XBUFFER& signature, HASH* hash, XDWORD saltsize);
    bool                            MaskGenerationFunction1          (XBUFFER& seed, XDWORD size, HASH* hash, XBUFFER& mask);

    bool                            DoRSAPublicOperation              (XBUFFER& input, XBUFFER& output);
    bool                            DoRSAPrivateOperation             (XBUFFER& input, XBUFFER& output, XMPINTEGER_FUNCRANDOM funcrandom, void* paramrandom);

    #if !defined(CIPHERRSA_RSANOCRT)
    bool                            PrepareBlinding                   (XMPINTEGER* Vi, XMPINTEGER* Vf, XMPINTEGER_FUNCRANDOM funcrandom, void* paramrandom);
    #endif

    int                             GetKeySizeInBytes                 (CIPHERKEYTYPE keytouse);

    static bool                     GenerateRandom                    (XBYTE* buffer, XDWORD size, void* param);    

    void                            Clean                             ();


    CIPHERRSA_CONTEXT               context;
    XRAND*                          xrand;
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/



