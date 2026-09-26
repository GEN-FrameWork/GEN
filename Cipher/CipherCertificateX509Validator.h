/**-------------------------------------------------------------------------------------------------------------------
*
* @file       CipherCertificateX509Validator.h
*
* @class      CIPHERCERTIFICATEX509VALIDATOR
* @brief      Cipher Certificate X.509 validation class
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

#include "XVector.h"

#include "CipherCertificateX509.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


#define CIPHERCERTIFICATEX509VALIDATOR_MAXCHAINSIZE           10                // Hard cap on the certificate_list size, to bound
                                                                                 // decoding work before any check has run (RFC 8446
                                                                                 // does not limit it; a real chain rarely exceeds 4-5).


enum CIPHERCERTIFICATEX509VALIDATOR_PURPOSE
{
  CIPHERCERTIFICATEX509VALIDATOR_PURPOSE_SERVER_AUTH = 0 ,
  CIPHERCERTIFICATEX509VALIDATOR_PURPOSE_CLIENT_AUTH     ,
};

enum CIPHERCERTIFICATEX509VALIDATOR_ERROR
{
  CIPHERCERTIFICATEX509VALIDATOR_ERROR_NONE                         = 0 ,
  CIPHERCERTIFICATEX509VALIDATOR_ERROR_INVALIDPARAMETER                 ,
  CIPHERCERTIFICATEX509VALIDATOR_ERROR_INVALIDCERTIFICATE               ,
  CIPHERCERTIFICATEX509VALIDATOR_ERROR_UNSUPPORTEDALGORITHM             ,
  CIPHERCERTIFICATEX509VALIDATOR_ERROR_INVALIDDATE                      ,
  CIPHERCERTIFICATEX509VALIDATOR_ERROR_INVALIDNAME                      ,
  CIPHERCERTIFICATEX509VALIDATOR_ERROR_INVALIDKEYUSAGE                  ,
  CIPHERCERTIFICATEX509VALIDATOR_ERROR_INVALIDCA                        ,
  CIPHERCERTIFICATEX509VALIDATOR_ERROR_INVALIDSIGNATURE                 ,
  CIPHERCERTIFICATEX509VALIDATOR_ERROR_UNTRUSTEDROOT                    ,
  CIPHERCERTIFICATEX509VALIDATOR_ERROR_UNKNOWNCRITICALEXTENSION         ,
  CIPHERCERTIFICATEX509VALIDATOR_ERROR_PATHLENGTH                       ,
  CIPHERCERTIFICATEX509VALIDATOR_ERROR_WEAKKEY                          ,
  CIPHERCERTIFICATEX509VALIDATOR_ERROR_NAMECONSTRAINT                   ,
  CIPHERCERTIFICATEX509VALIDATOR_ERROR_REVOKED                          ,
  CIPHERCERTIFICATEX509VALIDATOR_ERROR_REVOCATIONUNKNOWN                ,
};

class CIPHERCERTIFICATEX509VALIDATIONPOLICY
{
  public:
                                            CIPHERCERTIFICATEX509VALIDATIONPOLICY ();

    XDWORD                                  GetMinimumRSAKeyBits                  ();
    void                                    SetMinimumRSAKeyBits                  (XDWORD bits);
    bool                                    GetAllowSHA1                          ();
    void                                    SetAllowSHA1                          (bool allow);
    bool                                    GetAllowExpired                       ();
    void                                    SetAllowExpired                       (bool allow);
    bool                                    GetRequireServerAuthEKU               ();
    void                                    SetRequireServerAuthEKU               (bool require);
    bool                                    GetRequireClientAuthEKU               ();
    void                                    SetRequireClientAuthEKU               (bool require);
    XDWORD                                  GetMaximumChainDepth                  ();
    void                                    SetMaximumChainDepth                  (XDWORD depth);
    bool                                    IsSignatureAlgorithmAllowed           (CIPHERCERTIFICATEX509_ALGORITHM_TYPE algorithm);
    void                                    SetSignatureAlgorithmAllowed          (CIPHERCERTIFICATEX509_ALGORITHM_TYPE algorithm, bool allowed);

  private:
    XDWORD                                  minimumRSAKeyBits;
    bool                                    allowSHA1;
    bool                                    allowExpired;
    bool                                    requireServerAuthEKU;
    bool                                    requireClientAuthEKU;
    XDWORD                                  maximumChainDepth;
    XQWORD                                  allowedSignatureAlgorithms;
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class CIPHERCERTIFICATEX509VALIDATOR
{
  public:
                                            CIPHERCERTIFICATEX509VALIDATOR          ();
    virtual                                ~CIPHERCERTIFICATEX509VALIDATOR          ();

    bool                                    Validate                                 (XVECTOR<XBUFFER*>* certificatechain, XVECTOR<XBUFFER*>* trustedroots, XCHAR* servername, XDATETIME* datetime = NULL);
    bool                                    ValidateClient                           (XVECTOR<XBUFFER*>* certificatechain, XVECTOR<XBUFFER*>* trustedroots, XDATETIME* datetime = NULL);
    bool                                    ValidateAllPaths                         (XVECTOR<XBUFFER*>* certificatechain, XVECTOR<XBUFFER*>* trustedroots, XCHAR* servername, XDATETIME* datetime = NULL);
    bool                                    ValidateClientAllPaths                   (XVECTOR<XBUFFER*>* certificatechain, XVECTOR<XBUFFER*>* trustedroots, XDATETIME* datetime = NULL);

    CIPHERCERTIFICATEX509VALIDATIONPOLICY*  GetPolicy                                ();
    void                                    SetPolicy                                (CIPHERCERTIFICATEX509VALIDATIONPOLICY& policy);

    CIPHERCERTIFICATEX509VALIDATOR_ERROR    GetError                                 ();
    CIPHERCERTIFICATEX509*                  GetLeafCertificate                       ();
    XVECTOR<CIPHERCERTIFICATEX509*>*        GetCertificateChain                      ();

    void                                    End                                      ();

  private:

    bool                                    ValidateInternal                         (XVECTOR<XBUFFER*>* certificatechain, XVECTOR<XBUFFER*>* trustedroots, CIPHERCERTIFICATEX509VALIDATOR_PURPOSE purpose, XCHAR* servername, XDATETIME* datetime);
    bool                                    ValidateAllPathsInternal                 (XVECTOR<XBUFFER*>* certificatechain, XVECTOR<XBUFFER*>* trustedroots, CIPHERCERTIFICATEX509VALIDATOR_PURPOSE purpose, XCHAR* servername, XDATETIME* datetime);
    bool                                    IsSignatureAlgorithmSupported             (CIPHERCERTIFICATEX509* certificate);
    bool                                    IsSamePublicKey                           (CIPHERKEY* key1, CIPHERKEY* key2);
    bool                                    SetError                                  (CIPHERCERTIFICATEX509VALIDATOR_ERROR error);
    void                                    Clean                                     ();

    CIPHERCERTIFICATEX509VALIDATOR_ERROR    error;
    CIPHERCERTIFICATEX509VALIDATIONPOLICY   policy;
    XVECTOR<CIPHERCERTIFICATEX509*>         certificates;
    XVECTOR<CIPHERCERTIFICATEX509*>         roots;
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
