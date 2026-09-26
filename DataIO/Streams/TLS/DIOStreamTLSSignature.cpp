/**-------------------------------------------------------------------------------------------------------------------
*
* @file       DIOStreamTLSSignature.cpp
*
* @class      DIOSTREAMTLSSIGNATURE
* @brief      Data Input/Output Stream TLS Signature class
* @ingroup    DATAIO
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOStreamTLSSignature.h"
#include "DIOStreamTLSMessages.h"

#include "CipherKey.h"
#include "CipherCertificateX509.h"
#include "CipherRSA.h"
#include "CipherECDSA.h"
#include "CipherKeySymmetrical.h"
#include "HashSHA2.h"

#ifdef CIPHER_ASYMMETRIC_ED25519_ACTIVE
#include "CipherEd25519.h"
#endif



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSSIGNATURE::IsSupported(XWORD signaturescheme, CIPHERKEY* key)
* @brief      Check whether a TLS signature scheme is supported for a key
* @ingroup    DATAIO
*
* @param[in]  signaturescheme : TLS signature scheme value.
* @param[in]  key : Cipher key to check.
*
* @return     bool : true if the condition is met; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSSIGNATURE::IsSupported(XWORD signaturescheme, CIPHERKEY* key)
{
  if(!key) return false;

  #ifdef CIPHER_ASYMMETRIC_ED25519_ACTIVE
  if((key->GetType() == CIPHERKEYTYPE_ED25519_PUBLIC) &&
     (signaturescheme == DIOSTREAMTLS_MSG_SIGNATURESCHEME_ED25519)) return true;
  #endif

  if((key->GetType() == CIPHERKEYTYPE_ECDSA_SECP256R1_PUBLIC) &&
     (signaturescheme == DIOSTREAMTLS_MSG_SIGNATURESCHEME_ECDSA_SECP256R1_SHA256)) return true;

  if((key->GetType() == CIPHERKEYTYPE_ECDSA_SECP384R1_PUBLIC) &&
     (signaturescheme == DIOSTREAMTLS_MSG_SIGNATURESCHEME_ECDSA_SECP384R1_SHA384)) return true;

  if((key->GetType() == CIPHERKEYTYPE_ECDSA_SECP521R1_PUBLIC) &&
     (signaturescheme == DIOSTREAMTLS_MSG_SIGNATURESCHEME_ECDSA_SECP521R1_SHA512)) return true;

  if(key->GetType() != CIPHERKEYTYPE_RSA_PUBLIC) return false;

  switch(signaturescheme)
    {
      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA256 :
      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA384 :
      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA512 :
      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA256  :
      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA384  :
      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA512  :
      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PKCS1_SHA256     :
      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PKCS1_SHA384     :
      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PKCS1_SHA512     : return true;
                                                        default : break;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSSIGNATURE::IsSupported(XWORD signaturescheme, CIPHERCERTIFICATEX509* certificate)
* @brief      Is supported
* @ingroup    DATAIO
* 
* @param[in]  signaturescheme : Signaturescheme value.
* @param[in]  certificate : Pointer to certificate.
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSSIGNATURE::IsSupported(XWORD signaturescheme, CIPHERCERTIFICATEX509* certificate)
{
  if(!certificate || !certificate->GetPublicCipherKey()) return false;

  CIPHERKEY* key = certificate->GetPublicCipherKey();
  if(key->GetType() != CIPHERKEYTYPE_RSA_PUBLIC) return IsSupported(signaturescheme, key);

  XSTRING* keyOID = certificate->GetPublicCipherKeyID();
  if(!keyOID) return false;

  bool RSAE   = !keyOID->Compare(__L("1.2.840.113549.1.1.1"), false);
  bool RSAPSS = !keyOID->Compare(__L("1.2.840.113549.1.1.10"), false);

  switch(signaturescheme)
    {
      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA256 :
      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA384 :
      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA512 :
      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PKCS1_SHA256     :
      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PKCS1_SHA384     :
      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PKCS1_SHA512     : return RSAE;

      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA256  : return RSAPSS &&
          (certificate->GetPublicKeyRSASSAPSSHashType() == CIPHERCERTIFICATEX509_RSASSAPSS_HASH_TYPE_SHA256) &&
          (certificate->GetPublicKeyRSASSAPSSSaltSize() == HASHSHA2_256_DIGEST_SIZE);
      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA384  : return RSAPSS &&
          (certificate->GetPublicKeyRSASSAPSSHashType() == CIPHERCERTIFICATEX509_RSASSAPSS_HASH_TYPE_SHA384) &&
          (certificate->GetPublicKeyRSASSAPSSSaltSize() == HASHSHA2_384_DIGEST_SIZE);
      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA512  : return RSAPSS &&
          (certificate->GetPublicKeyRSASSAPSSHashType() == CIPHERCERTIFICATEX509_RSASSAPSS_HASH_TYPE_SHA512) &&
          (certificate->GetPublicKeyRSASSAPSSSaltSize() == HASHSHA2_512_DIGEST_SIZE);
                                                        default : break;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSSIGNATURE::Verify(XWORD signaturescheme, CIPHERKEY* key, XBUFFER& content, XBUFFER& signature)
* @brief      Verify a TLS signature with the scheme-specific hash and padding parameters
* @ingroup    DATAIO
*
* @param[in]  signaturescheme : TLS signature scheme value.
* @param[in]  key : Public cipher key.
* @param[in]  content : Signed content.
* @param[in]  signature : Signature value.
*
* @return     bool : true if the signature is valid; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSSIGNATURE::Verify(XWORD signaturescheme, CIPHERKEY* key, XBUFFER& content, XBUFFER& signature)
{
  if(!IsSupported(signaturescheme, key) || content.IsEmpty() || signature.IsEmpty()) return false;

  #ifdef CIPHER_ASYMMETRIC_ED25519_ACTIVE

  if(signaturescheme == DIOSTREAMTLS_MSG_SIGNATURESCHEME_ED25519)
    {
      CIPHERKEYSYMMETRICAL* edkey = (CIPHERKEYSYMMETRICAL*)key;
      if(!edkey->Get() || edkey->Get()->GetSize() != CIPHERED25519_PUBLICKEYSIZE) return false;
      CIPHERED25519 Ed25519;
      return Ed25519.Verify((*edkey->Get()), content, signature);
    }

  #endif

  if(signaturescheme == DIOSTREAMTLS_MSG_SIGNATURESCHEME_ECDSA_SECP256R1_SHA256)
    {
      CIPHERECDSA ECDSA(CIPHERTYPE_ECDSA_SECP256R1);
      HASHSHA2    hash(HASHSHA2TYPE_256);

      return ECDSA.SetKey(key, true) && ECDSA.Verify(content, signature, &hash);
    }

  if(signaturescheme == DIOSTREAMTLS_MSG_SIGNATURESCHEME_ECDSA_SECP384R1_SHA384)
    {
      CIPHERECDSA ECDSA(CIPHERTYPE_ECDSA_SECP384R1);
      HASHSHA2    hash(HASHSHA2TYPE_384);

      return ECDSA.SetKey(key, true) && ECDSA.Verify(content, signature, &hash);
    }

  if(signaturescheme == DIOSTREAMTLS_MSG_SIGNATURESCHEME_ECDSA_SECP521R1_SHA512)
    {
      CIPHERECDSA ECDSA(CIPHERTYPE_ECDSA_SECP521R1);
      HASHSHA2    hash(HASHSHA2TYPE_512);

      return ECDSA.SetKey(key, true) && ECDSA.Verify(content, signature, &hash);
    }

  CIPHERRSA RSA;
  if(!RSA.SetKey(key, true)) return false;

  switch(signaturescheme)
    {
      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA256 : { HASHSHA2 hash(HASHSHA2TYPE_256);
                                                                    return RSA.Verify(content, signature, &hash, CIPHERRSAPKCS1VERSIONV21, HASHSHA2_256_DIGEST_SIZE);
                                                                  }
      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA256  : { HASHSHA2 hash(HASHSHA2TYPE_256);
                                                                    return RSA.Verify(content, signature, &hash, CIPHERRSAPKCS1VERSIONV21, HASHSHA2_256_DIGEST_SIZE);
                                                                  }

      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA384 : { HASHSHA2 hash(HASHSHA2TYPE_384);
                                                                    return RSA.Verify(content, signature, &hash, CIPHERRSAPKCS1VERSIONV21, HASHSHA2_384_DIGEST_SIZE);
                                                                  }
      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA384  : { HASHSHA2 hash(HASHSHA2TYPE_384);
                                                                    return RSA.Verify(content, signature, &hash, CIPHERRSAPKCS1VERSIONV21, HASHSHA2_384_DIGEST_SIZE);
                                                                  }

      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA512 : { HASHSHA2 hash(HASHSHA2TYPE_512);
                                                                    return RSA.Verify(content, signature, &hash, CIPHERRSAPKCS1VERSIONV21, HASHSHA2_512_DIGEST_SIZE);
                                                                  }
      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA512  : { HASHSHA2 hash(HASHSHA2TYPE_512);
                                                                    return RSA.Verify(content, signature, &hash, CIPHERRSAPKCS1VERSIONV21, HASHSHA2_512_DIGEST_SIZE);
                                                                  }

      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PKCS1_SHA256     : { HASHSHA2 hash(HASHSHA2TYPE_256);
                                                                    return RSA.Verify(content, signature, &hash, CIPHERRSAPKCS1VERSIONV15);
                                                                  }

      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PKCS1_SHA384     : { HASHSHA2 hash(HASHSHA2TYPE_384);
                                                                    return RSA.Verify(content, signature, &hash, CIPHERRSAPKCS1VERSIONV15);
                                                                  }

      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PKCS1_SHA512     : { HASHSHA2 hash(HASHSHA2TYPE_512);
                                                                    return RSA.Verify(content, signature, &hash, CIPHERRSAPKCS1VERSIONV15);
                                                                  }

                                                        default : break;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSSIGNATURE::Sign(XWORD signaturescheme, CIPHERKEY* privatekey, CIPHERKEY* publickey, XBUFFER& content, XBUFFER& signature)
* @brief      Sign with the scheme-specific hash and padding parameters (server-side CertificateVerify/ServerKeyExchange)
* @ingroup    DATAIO
*
* @param[in]  privatekey : Local private key (CIPHERKEYTYPE_RSA_PRIVATE).
* @param[in]  publickey : Local public key from the same key pair (CIPHERKEYTYPE_RSA_PUBLIC) -- typically the
*             leaf certificate's public key, needed because the private key alone does not carry the modulus.
* @param[in]  content : Content to sign.
* @param[out] signature : Produced signature.
* @param[in]  signaturescheme : Signaturescheme value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSSIGNATURE::Sign(XWORD signaturescheme, CIPHERKEY* privatekey, CIPHERKEY* publickey, XBUFFER& content, XBUFFER& signature)
{
  if(!privatekey || !publickey || content.IsEmpty()) return false;

  #ifdef CIPHER_ASYMMETRIC_ED25519_ACTIVE

  if(signaturescheme == DIOSTREAMTLS_MSG_SIGNATURESCHEME_ED25519)
    {
      if((privatekey->GetType() != CIPHERKEYTYPE_ED25519_PRIVATE) ||
         (publickey->GetType()  != CIPHERKEYTYPE_ED25519_PUBLIC)) return false;

      CIPHERKEYSYMMETRICAL* privateed = (CIPHERKEYSYMMETRICAL*)privatekey;
      CIPHERKEYSYMMETRICAL* publiced  = (CIPHERKEYSYMMETRICAL*)publickey;
      if(!privateed->Get() || !publiced->Get() ||
         privateed->Get()->GetSize()!=CIPHERED25519_PRIVATEKEYSIZE ||
         publiced->Get()->GetSize()!=CIPHERED25519_PUBLICKEYSIZE) return false;

      CIPHERED25519 Ed25519;
      return Ed25519.Sign((*privateed->Get()), (*publiced->Get()), content, signature);
    }

  #endif

  if(signaturescheme == DIOSTREAMTLS_MSG_SIGNATURESCHEME_ECDSA_SECP256R1_SHA256)
    {
      if((privatekey->GetType() != CIPHERKEYTYPE_ECDSA_SECP256R1_PRIVATE) ||
         (publickey->GetType()  != CIPHERKEYTYPE_ECDSA_SECP256R1_PUBLIC))
        {
          return false;
        }

      CIPHERECDSA ECDSA(CIPHERTYPE_ECDSA_SECP256R1);
      HASHSHA2    hash(HASHSHA2TYPE_256);

      // Order matters, same reason as CIPHERRSA below: SetKey(ECDSA_PRIVATE, integritycheck=true) cross-checks
      // privatekey*G == publickey, so the public point has to be loaded into the shared state first.
      if(!ECDSA.SetKey(publickey, true) || !ECDSA.SetKey(privatekey, true) || !ECDSA.Sign(content, &hash)) return false;

      signature.Delete();
      return signature.Add((*ECDSA.GetResult()));
    }

  if(signaturescheme == DIOSTREAMTLS_MSG_SIGNATURESCHEME_ECDSA_SECP384R1_SHA384)
    {
      if((privatekey->GetType() != CIPHERKEYTYPE_ECDSA_SECP384R1_PRIVATE) ||
         (publickey->GetType()  != CIPHERKEYTYPE_ECDSA_SECP384R1_PUBLIC))
        {
          return false;
        }

      CIPHERECDSA ECDSA(CIPHERTYPE_ECDSA_SECP384R1);
      HASHSHA2    hash(HASHSHA2TYPE_384);

      if(!ECDSA.SetKey(publickey, true) || !ECDSA.SetKey(privatekey, true) || !ECDSA.Sign(content, &hash)) return false;

      signature.Delete();
      return signature.Add((*ECDSA.GetResult()));
    }

  if(signaturescheme == DIOSTREAMTLS_MSG_SIGNATURESCHEME_ECDSA_SECP521R1_SHA512)
    {
      if((privatekey->GetType() != CIPHERKEYTYPE_ECDSA_SECP521R1_PRIVATE) ||
         (publickey->GetType()  != CIPHERKEYTYPE_ECDSA_SECP521R1_PUBLIC))
        {
          return false;
        }

      CIPHERECDSA ECDSA(CIPHERTYPE_ECDSA_SECP521R1);
      HASHSHA2    hash(HASHSHA2TYPE_512);

      if(!ECDSA.SetKey(publickey, true) || !ECDSA.SetKey(privatekey, true) || !ECDSA.Sign(content, &hash)) return false;

      signature.Delete();
      return signature.Add((*ECDSA.GetResult()));
    }

  if((privatekey->GetType() != CIPHERKEYTYPE_RSA_PRIVATE) || (publickey->GetType() != CIPHERKEYTYPE_RSA_PUBLIC)) return false;

  switch(signaturescheme)
    {
      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA256 :
      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA384 :
      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA512 :
      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA256  :
      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA384  :
      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA512  :
      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PKCS1_SHA256     :
      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PKCS1_SHA384     :
      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PKCS1_SHA512     : break;
                                                        default : return false;
    }

  CIPHERRSA RSA;

  // Order matters: SetKey(RSA_PRIVATE, integritycheck=true) cross-checks P*Q == N, so the modulus (from the
  // public key) has to be loaded into the shared context first.
  if(!RSA.SetKey(publickey, true) || !RSA.SetKey(privatekey, true)) return false;

  bool status = false;

  switch(signaturescheme)
    {
      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA256 : { HASHSHA2 hash(HASHSHA2TYPE_256);
                                                                    status = RSA.Sign(content, CIPHERKEYTYPE_RSA_PRIVATE, &hash, CIPHERRSAPKCS1VERSIONV21);
                                                                  }
                                                                  break;

      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA256  : { HASHSHA2 hash(HASHSHA2TYPE_256);
                                                                    status = RSA.Sign(content, CIPHERKEYTYPE_RSA_PRIVATE, &hash, CIPHERRSAPKCS1VERSIONV21);
                                                                  }
                                                                  break;

      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA384 : { HASHSHA2 hash(HASHSHA2TYPE_384);
                                                                    status = RSA.Sign(content, CIPHERKEYTYPE_RSA_PRIVATE, &hash, CIPHERRSAPKCS1VERSIONV21);
                                                                  }
                                                                  break;

      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA384  : { HASHSHA2 hash(HASHSHA2TYPE_384);
                                                                    status = RSA.Sign(content, CIPHERKEYTYPE_RSA_PRIVATE, &hash, CIPHERRSAPKCS1VERSIONV21);
                                                                  }
                                                                  break;

      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA512 : { HASHSHA2 hash(HASHSHA2TYPE_512);
                                                                    status = RSA.Sign(content, CIPHERKEYTYPE_RSA_PRIVATE, &hash, CIPHERRSAPKCS1VERSIONV21);
                                                                  }
                                                                  break;

      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA512  : { HASHSHA2 hash(HASHSHA2TYPE_512);
                                                                    status = RSA.Sign(content, CIPHERKEYTYPE_RSA_PRIVATE, &hash, CIPHERRSAPKCS1VERSIONV21);
                                                                  }
                                                                  break;

      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PKCS1_SHA256     : { HASHSHA2 hash(HASHSHA2TYPE_256);
                                                                    status = RSA.Sign(content, CIPHERKEYTYPE_RSA_PRIVATE, &hash, CIPHERRSAPKCS1VERSIONV15);
                                                                  }
                                                                  break;

      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PKCS1_SHA384     : { HASHSHA2 hash(HASHSHA2TYPE_384);
                                                                    status = RSA.Sign(content, CIPHERKEYTYPE_RSA_PRIVATE, &hash, CIPHERRSAPKCS1VERSIONV15);
                                                                  }
                                                                  break;

      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PKCS1_SHA512     : { HASHSHA2 hash(HASHSHA2TYPE_512);
                                                                    status = RSA.Sign(content, CIPHERKEYTYPE_RSA_PRIVATE, &hash, CIPHERRSAPKCS1VERSIONV15);
                                                                  }
                                                                  break;

                                                        default : break;
    }

  if(!status) return false;

  signature.Delete();

  return signature.Add((*RSA.GetResult()));
}
