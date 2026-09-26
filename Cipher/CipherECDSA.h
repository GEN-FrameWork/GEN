/**-------------------------------------------------------------------------------------------------------------------
*
* @file       CipherECDSA.h
*
* @class      CIPHERECDSA
* @brief      Cipher ECDSA class
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

#include "Cipher.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define CIPHERECDSA_P256_PUBLICKEY_SIZE       65
#define CIPHERECDSA_P256_COORDINATE_SIZE      32
#define CIPHERECDSA_P256_CURVEBITS            256

#define CIPHERECDSA_P384_PUBLICKEY_SIZE       97
#define CIPHERECDSA_P384_COORDINATE_SIZE      48
#define CIPHERECDSA_P384_CURVEBITS            384

#define CIPHERECDSA_P521_PUBLICKEY_SIZE       133
#define CIPHERECDSA_P521_COORDINATE_SIZE      66
#define CIPHERECDSA_P521_CURVEBITS            521

#define CIPHERECDSA_MAXCOORDINATE_SIZE        CIPHERECDSA_P521_COORDINATE_SIZE

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class HASH;

class CIPHERECDSA : public CIPHER
{
  public:

                                    CIPHERECDSA                       (CIPHERTYPE curvetype = CIPHERTYPE_ECDSA_SECP256R1);
    virtual                        ~CIPHERECDSA                       ();

    bool                            SetKey                            (CIPHERKEY* key, bool integritycheck = false);

    bool                            Verify                            (XBYTE* input, XDWORD size, XBUFFER& signature, HASH* hash);
    bool                            Verify                            (XBUFFER& input, XBUFFER& signature, HASH* hash);

    bool                            Sign                              (XBYTE* input, XDWORD size, HASH* hash);
    bool                            Sign                              (XBUFFER& input, HASH* hash);

    bool                            KeyPair_Create                    (XBUFFER& privatekey, XBUFFER& publickey);
    bool                            SharedSecret_Create               (XBUFFER& privatekey, XBUFFER& publickey, XBUFFER& sharedsecret);
    bool                            PublicKey_Check                   (XBUFFER& publickey);

  private:

    bool                            Parameters_Set                    ();
    bool                            PublicKey_Check                   ();
    bool                            Signature_Decode                  (XBUFFER& signature, XMPINTEGER& R, XMPINTEGER& S);
    bool                            Signature_Encode                  (XMPINTEGER& R, XMPINTEGER& S, XBUFFER& signature);

    void                            Clean                             ();

    XMPINTEGER                      prime;
    XMPINTEGER                      coefficientA;
    XMPINTEGER                      coefficientB;
    XMPINTEGER                      generatorX;
    XMPINTEGER                      generatorY;
    XMPINTEGER                      order;
    XMPINTEGER                      publickeyX;
    XMPINTEGER                      publickeyY;
    bool                            havepublickey;
    XMPINTEGER                      privatekeyD;
    bool                            haveprivatekey;


    XDWORD                          coordinatesize;
    XDWORD                          publickeysize;
    XDWORD                          curvebits;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/


