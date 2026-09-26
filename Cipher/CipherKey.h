/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       CipherKey.h
* 
* @class      CIPHERKEY
* @brief      Cipher Key interface class
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



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum CIPHERKEYTYPE
{
  CIPHERKEYTYPE_UNKNOWN                          = 0 ,

  CIPHERKEYTYPE_SYMMETRICAL                          ,
  
  CIPHERKEYTYPE_RSA_PUBLIC                           ,
  CIPHERKEYTYPE_RSA_PRIVATE                          ,  

  CIPHERKEYTYPE_ECDSA_SECP192R1_PUBLIC	             ,
  CIPHERKEYTYPE_ECDSA_SECP192R1_PRIVATE	             ,
  CIPHERKEYTYPE_ECDSA_SECP224R1_PUBLIC	             ,
  CIPHERKEYTYPE_ECDSA_SECP224R1_PRIVATE	             ,
  CIPHERKEYTYPE_ECDSA_SECP256R1_PUBLIC	             ,
  CIPHERKEYTYPE_ECDSA_SECP256R1_PRIVATE	             ,
  CIPHERKEYTYPE_ECDSA_SECP384R1_PUBLIC	             , 
  CIPHERKEYTYPE_ECDSA_SECP384R1_PRIVATE	             ,
  CIPHERKEYTYPE_ECDSA_SECP521R1_PUBLIC	             ,
  CIPHERKEYTYPE_ECDSA_SECP521R1_PRIVATE	             ,
  CIPHERKEYTYPE_ECDSA_SECP256K1_PUBLIC	             ,
  CIPHERKEYTYPE_ECDSA_SECP256K1_PRIVATE	             ,
  CIPHERKEYTYPE_ECDSA_SECT163K1_PUBLIC	             , 
  CIPHERKEYTYPE_ECDSA_SECT163K1_PRIVATE	             ,
  CIPHERKEYTYPE_ECDSA_SECT233K1_PUBLIC	             , 
  CIPHERKEYTYPE_ECDSA_SECT233K1_PRIVATE	             , 
  CIPHERKEYTYPE_ECDSA_BRAINPOOLP256R1_PUBLIC	       , 
  CIPHERKEYTYPE_ECDSA_BRAINPOOLP256R1_PRIVATE	       , 
  CIPHERKEYTYPE_ECDSA_BRAINPOOLP384R1_PUBLIC	       , 
  CIPHERKEYTYPE_ECDSA_BRAINPOOLP384R1_PRIVATE	       ,
  CIPHERKEYTYPE_ECDSA_BRAINPOOLP512R1_PUBLIC	       ,                                                     
  CIPHERKEYTYPE_ECDSA_BRAINPOOLP512R1_PRIVATE	       ,

  CIPHERKEYTYPE_ED25519_PUBLIC                      ,
  CIPHERKEYTYPE_ED25519_PRIVATE                     ,
  
  CIPHERKEYTYPE_LASTTYPE                           
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/


class CIPHERKEY
{
  public:
                          CIPHERKEY                 ();
    virtual              ~CIPHERKEY                 ();

    CIPHERKEYTYPE         GetType                   ();
    void                  SetType                   (CIPHERKEYTYPE type);

    XCHAR*                GetTypeStr                ();

    virtual int           GetSizeInBytes            ();
    int                   GetSizeInBits             ();

    bool                  CopyFrom                  (CIPHERKEY* key);

  protected:

    CIPHERKEYTYPE         type;

  private:

    void                  Clean                     ();
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




