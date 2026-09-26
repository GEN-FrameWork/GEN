/**-------------------------------------------------------------------------------------------------------------------
*
* @file       CipherCertificateX509Validator.cpp
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "CipherCertificateX509Validator.h"
#include "CipherCertificateX509PathBuilder.h"

#include "CipherKeyPublicRSA.h"
#include "CipherKeyECDSA.h"
#include "CipherKeySymmetrical.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERCERTIFICATEX509VALIDATOR_IsSelfIssued(CIPHERCERTIFICATEX509* certificate)
* @brief      Is self issued
* @ingroup    CIPHER
* 
* @param[in]  certificate : Pointer to certificate.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool CIPHERCERTIFICATEX509VALIDATOR_IsSelfIssued(CIPHERCERTIFICATEX509* certificate)
{
  return certificate && certificate->GetIssuerData() && certificate->GetSubjectData() &&
         certificate->GetIssuerData()->Compare((*certificate->GetSubjectData()));
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD CIPHERCERTIFICATEX509VALIDATOR_NonSelfIssuedCABelow(XVECTOR<CIPHERCERTIFICATEX509*>& certificates, XDWORD issuerindex)
* @brief      Non self issued ca below
* @ingroup    CIPHER
* 
* @param[in]  certificates : Pointer to certificates.
* @param[in]  issuerindex : Issuerindex value.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static XDWORD CIPHERCERTIFICATEX509VALIDATOR_NonSelfIssuedCABelow(XVECTOR<CIPHERCERTIFICATEX509*>& certificates, XDWORD issuerindex)
{
  XDWORD count = 0;
  XDWORD limit = issuerindex;
  if(limit > certificates.GetSize()) limit = certificates.GetSize();

  // Index 0 is the end-entity certificate. pathLenConstraint counts only non-self-issued
  // intermediate CA certificates below the CA carrying the constraint.
  for(XDWORD c=1; c<limit; c++)
    {
      CIPHERCERTIFICATEX509* certificate = certificates.Get(c);
      if(certificate && certificate->IsCertificateAuthority() &&
         !CIPHERCERTIFICATEX509VALIDATOR_IsSelfIssued(certificate)) count++;
    }

  return count;
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERCERTIFICATEX509VALIDATOR_IssuerIdentifierMatches(CIPHERCERTIFICATEX509* certificate, CIPHERCERTIFICATEX509* issuer)
* @brief      Issuer identifier matches
* @ingroup    CIPHER
* 
* @param[in]  certificate : Pointer to certificate.
* @param[in]  issuer : Pointer to issuer.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool CIPHERCERTIFICATEX509VALIDATOR_IssuerIdentifierMatches(CIPHERCERTIFICATEX509* certificate, CIPHERCERTIFICATEX509* issuer)
{
  if(!certificate || !issuer) return false;
  if(certificate->HasAuthorityKeyIdentifier() && issuer->HasSubjectKeyIdentifier())
    {
      XBUFFER* authority = certificate->GetAuthorityKeyIdentifier();
      XBUFFER* subject   = issuer->GetSubjectKeyIdentifier();
      if(!authority || !subject || !authority->Compare((*subject))) return false;
    }
  return true;
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERCERTIFICATEX509VALIDATOR_RootConstraintsPermit(CIPHERCERTIFICATEX509* root, XVECTOR<CIPHERCERTIFICATEX509*>& certificates, bool rootisinchain)
* @brief      Root constraints permit
* @ingroup    CIPHER
* 
* @param[in]  root : Pointer to root.
* @param[in]  certificates : Pointer to certificates.
* @param[in]  rootisinchain : Rootisinchain value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool CIPHERCERTIFICATEX509VALIDATOR_RootConstraintsPermit(CIPHERCERTIFICATEX509* root, XVECTOR<CIPHERCERTIFICATEX509*>& certificates, bool rootisinchain)
{
  if(!root || !root->HasNameConstraints()) return true;
  XDWORD limit=certificates.GetSize(); if(rootisinchain&&limit) limit--;
  for(XDWORD c=0;c<limit;c++)
    {
      CIPHERCERTIFICATEX509* certificate=certificates.Get(c);
      if((c>0) && CIPHERCERTIFICATEX509VALIDATOR_IsSelfIssued(certificate)) continue;
      if(!root->AreNamesPermitted(certificate)) return false;
    }
  return true;
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHERCERTIFICATEX509VALIDATIONPOLICY::CIPHERCERTIFICATEX509VALIDATIONPOLICY()
* @brief      Constructor of class
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERCERTIFICATEX509VALIDATIONPOLICY::CIPHERCERTIFICATEX509VALIDATIONPOLICY()
{
  minimumRSAKeyBits          = 2048;
  allowSHA1                  = false;
  allowExpired               = false;
  requireServerAuthEKU       = true;
  requireClientAuthEKU       = true;
  maximumChainDepth          = CIPHERCERTIFICATEX509VALIDATOR_MAXCHAINSIZE;
  allowedSignatureAlgorithms = 0;

  SetSignatureAlgorithmAllowed(CIPHERCERTIFICATEX509_ALGORITHM_TYPE_SHA256WITHRSAENCRYPTION, true);
  SetSignatureAlgorithmAllowed(CIPHERCERTIFICATEX509_ALGORITHM_TYPE_SHA384WITHRSAENCRYPTION, true);
  SetSignatureAlgorithmAllowed(CIPHERCERTIFICATEX509_ALGORITHM_TYPE_SHA512WITHRSAENCRYPTION, true);
  SetSignatureAlgorithmAllowed(CIPHERCERTIFICATEX509_ALGORITHM_TYPE_ECDSAWITHSHA256, true);
  SetSignatureAlgorithmAllowed(CIPHERCERTIFICATEX509_ALGORITHM_TYPE_ECDSAWITHSHA384, true);
  SetSignatureAlgorithmAllowed(CIPHERCERTIFICATEX509_ALGORITHM_TYPE_ECDSAWITHSHA512, true);
  SetSignatureAlgorithmAllowed(CIPHERCERTIFICATEX509_ALGORITHM_TYPE_RSASSAPSS, true);

  #ifdef CIPHER_ASYMMETRIC_ED25519_ACTIVE
  SetSignatureAlgorithmAllowed(CIPHERCERTIFICATEX509_ALGORITHM_TYPE_ED25519, true);
  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD CIPHERCERTIFICATEX509VALIDATIONPOLICY::GetMinimumRSAKeyBits()
* @brief      Get minimum rsa key bits
* @ingroup    CIPHER
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD CIPHERCERTIFICATEX509VALIDATIONPOLICY::GetMinimumRSAKeyBits() { return minimumRSAKeyBits; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void CIPHERCERTIFICATEX509VALIDATIONPOLICY::SetMinimumRSAKeyBits(XDWORD bits)
* @brief      Set minimum rsa key bits
* @ingroup    CIPHER
* 
* @param[in]  bits : Bits value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void CIPHERCERTIFICATEX509VALIDATIONPOLICY::SetMinimumRSAKeyBits(XDWORD bits) { minimumRSAKeyBits = bits; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERCERTIFICATEX509VALIDATIONPOLICY::GetAllowSHA1()
* @brief      Get allow sha1
* @ingroup    CIPHER
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERCERTIFICATEX509VALIDATIONPOLICY::GetAllowSHA1() { return allowSHA1; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void CIPHERCERTIFICATEX509VALIDATIONPOLICY::SetAllowSHA1(bool allow)
* @brief      Set allow sha1
* @ingroup    CIPHER
* 
* @param[in]  allow : Allow value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void CIPHERCERTIFICATEX509VALIDATIONPOLICY::SetAllowSHA1(bool allow) { allowSHA1 = allow; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERCERTIFICATEX509VALIDATIONPOLICY::GetAllowExpired()
* @brief      Get allow expired
* @ingroup    CIPHER
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERCERTIFICATEX509VALIDATIONPOLICY::GetAllowExpired() { return allowExpired; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void CIPHERCERTIFICATEX509VALIDATIONPOLICY::SetAllowExpired(bool allow)
* @brief      Set allow expired
* @ingroup    CIPHER
* 
* @param[in]  allow : Allow value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void CIPHERCERTIFICATEX509VALIDATIONPOLICY::SetAllowExpired(bool allow) { allowExpired = allow; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERCERTIFICATEX509VALIDATIONPOLICY::GetRequireServerAuthEKU()
* @brief      Get require server auth eku
* @ingroup    CIPHER
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERCERTIFICATEX509VALIDATIONPOLICY::GetRequireServerAuthEKU() { return requireServerAuthEKU; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void CIPHERCERTIFICATEX509VALIDATIONPOLICY::SetRequireServerAuthEKU(bool require)
* @brief      Set require server auth eku
* @ingroup    CIPHER
* 
* @param[in]  require : Require value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void CIPHERCERTIFICATEX509VALIDATIONPOLICY::SetRequireServerAuthEKU(bool require) { requireServerAuthEKU = require; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERCERTIFICATEX509VALIDATIONPOLICY::GetRequireClientAuthEKU()
* @brief      Get require client auth eku
* @ingroup    CIPHER
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERCERTIFICATEX509VALIDATIONPOLICY::GetRequireClientAuthEKU() { return requireClientAuthEKU; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void CIPHERCERTIFICATEX509VALIDATIONPOLICY::SetRequireClientAuthEKU(bool require)
* @brief      Set require client auth eku
* @ingroup    CIPHER
* 
* @param[in]  require : Require value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void CIPHERCERTIFICATEX509VALIDATIONPOLICY::SetRequireClientAuthEKU(bool require) { requireClientAuthEKU = require; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD CIPHERCERTIFICATEX509VALIDATIONPOLICY::GetMaximumChainDepth()
* @brief      Get maximum chain depth
* @ingroup    CIPHER
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD CIPHERCERTIFICATEX509VALIDATIONPOLICY::GetMaximumChainDepth() { return maximumChainDepth; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void CIPHERCERTIFICATEX509VALIDATIONPOLICY::SetMaximumChainDepth(XDWORD depth)
* @brief      Set maximum chain depth
* @ingroup    CIPHER
* 
* @param[in]  depth : Depth value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void CIPHERCERTIFICATEX509VALIDATIONPOLICY::SetMaximumChainDepth(XDWORD depth) { maximumChainDepth = depth; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERCERTIFICATEX509VALIDATIONPOLICY::IsSignatureAlgorithmAllowed(CIPHERCERTIFICATEX509_ALGORITHM_TYPE algorithm)
* @brief      Is signature algorithm allowed
* @ingroup    CIPHER
* 
* @param[in]  algorithm : Algorithm value.
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERCERTIFICATEX509VALIDATIONPOLICY::IsSignatureAlgorithmAllowed(CIPHERCERTIFICATEX509_ALGORITHM_TYPE algorithm)
{
  if((algorithm == CIPHERCERTIFICATEX509_ALGORITHM_TYPE_SHA1WITHRSAENCRYPTION) ||
     (algorithm == CIPHERCERTIFICATEX509_ALGORITHM_TYPE_ECDSAWITHSHA1)) return allowSHA1;
  if(((XDWORD)algorithm >= 64) || (algorithm == CIPHERCERTIFICATEX509_ALGORITHM_TYPE_UNKNOWN)) return false;
  return (allowedSignatureAlgorithms & (((XQWORD)1) << (XDWORD)algorithm))?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void CIPHERCERTIFICATEX509VALIDATIONPOLICY::SetSignatureAlgorithmAllowed(CIPHERCERTIFICATEX509_ALGORITHM_TYPE algorithm, bool allowed)
* @brief      Set signature algorithm allowed
* @ingroup    CIPHER
* 
* @param[in]  algorithm : Algorithm value.
* @param[in]  allowed : Allowed value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void CIPHERCERTIFICATEX509VALIDATIONPOLICY::SetSignatureAlgorithmAllowed(CIPHERCERTIFICATEX509_ALGORITHM_TYPE algorithm, bool allowed)
{
  if(((XDWORD)algorithm >= 64) || (algorithm == CIPHERCERTIFICATEX509_ALGORITHM_TYPE_UNKNOWN)) return;
  XQWORD mask = ((XQWORD)1) << (XDWORD)algorithm;
  if(allowed) allowedSignatureAlgorithms |= mask;
          else allowedSignatureAlgorithms &= ~mask;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         CIPHERCERTIFICATEX509VALIDATOR::CIPHERCERTIFICATEX509VALIDATOR()
* @brief      Constructor of class
* @ingroup    CIPHER
*
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERCERTIFICATEX509VALIDATOR::CIPHERCERTIFICATEX509VALIDATOR()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         CIPHERCERTIFICATEX509VALIDATOR::~CIPHERCERTIFICATEX509VALIDATOR()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    CIPHER
*
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERCERTIFICATEX509VALIDATOR::~CIPHERCERTIFICATEX509VALIDATOR()
{
  End();
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool CIPHERCERTIFICATEX509VALIDATOR::Validate(XVECTOR<XBUFFER*>* certificatechain, XVECTOR<XBUFFER*>* trustedroots, XCHAR* servername, XDATETIME* datetime)
* @brief      Validate an ordered X.509 server chain against explicitly configured trust anchors
* @ingroup    CIPHER
* 
* @param[in]  certificatechain : Pointer to certificatechain.
* @param[in]  trustedroots : Pointer to trustedroots.
* @param[in]  servername : Pointer to servername.
* @param[in]  datetime : Pointer to datetime.
*
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERCERTIFICATEX509VALIDATOR::Validate(XVECTOR<XBUFFER*>* certificatechain, XVECTOR<XBUFFER*>* trustedroots, XCHAR* servername, XDATETIME* datetime)
{
  return ValidateInternal(certificatechain, trustedroots, CIPHERCERTIFICATEX509VALIDATOR_PURPOSE_SERVER_AUTH, servername, datetime);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool CIPHERCERTIFICATEX509VALIDATOR::ValidateClient(XVECTOR<XBUFFER*>* certificatechain, XVECTOR<XBUFFER*>* trustedroots, XDATETIME* datetime)
* @brief      Validate an ordered X.509 client-authentication chain against explicitly configured trust anchors
* @ingroup    CIPHER
* 
* @param[in]  certificatechain : Pointer to certificatechain.
* @param[in]  trustedroots : Pointer to trustedroots.
* @param[in]  datetime : Pointer to datetime.
*
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERCERTIFICATEX509VALIDATOR::ValidateClient(XVECTOR<XBUFFER*>* certificatechain, XVECTOR<XBUFFER*>* trustedroots, XDATETIME* datetime)
{
  return ValidateInternal(certificatechain, trustedroots, CIPHERCERTIFICATEX509VALIDATOR_PURPOSE_CLIENT_AUTH, NULL, datetime);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERCERTIFICATEX509VALIDATOR::ValidateAllPaths(XVECTOR<XBUFFER*>* certificatechain, XVECTOR<XBUFFER*>* trustedroots, XCHAR* servername, XDATETIME* datetime)
* @brief      Validate all paths
* @ingroup    CIPHER
* 
* @param[in]  certificatechain : Pointer to certificatechain.
* @param[in]  trustedroots : Pointer to trustedroots.
* @param[in]  servername : Pointer to servername.
* @param[in]  datetime : Pointer to datetime.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERCERTIFICATEX509VALIDATOR::ValidateAllPaths(XVECTOR<XBUFFER*>* certificatechain, XVECTOR<XBUFFER*>* trustedroots, XCHAR* servername, XDATETIME* datetime)
{
  return ValidateAllPathsInternal(certificatechain, trustedroots, CIPHERCERTIFICATEX509VALIDATOR_PURPOSE_SERVER_AUTH, servername, datetime);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERCERTIFICATEX509VALIDATOR::ValidateClientAllPaths(XVECTOR<XBUFFER*>* certificatechain, XVECTOR<XBUFFER*>* trustedroots, XDATETIME* datetime)
* @brief      Validate client all paths
* @ingroup    CIPHER
* 
* @param[in]  certificatechain : Pointer to certificatechain.
* @param[in]  trustedroots : Pointer to trustedroots.
* @param[in]  datetime : Pointer to datetime.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERCERTIFICATEX509VALIDATOR::ValidateClientAllPaths(XVECTOR<XBUFFER*>* certificatechain, XVECTOR<XBUFFER*>* trustedroots, XDATETIME* datetime)
{
  return ValidateAllPathsInternal(certificatechain, trustedroots, CIPHERCERTIFICATEX509VALIDATOR_PURPOSE_CLIENT_AUTH, NULL, datetime);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERCERTIFICATEX509VALIDATOR::ValidateAllPathsInternal(XVECTOR<XBUFFER*>* certificatechain, XVECTOR<XBUFFER*>* trustedroots, CIPHERCERTIFICATEX509VALIDATOR_PURPOSE purpose, XCHAR* servername, XDATETIME* datetime)
* @brief      Validate all paths internal
* @ingroup    CIPHER
* 
* @param[in]  certificatechain : Pointer to certificatechain.
* @param[in]  trustedroots : Pointer to trustedroots.
* @param[in]  purpose : Purpose value.
* @param[in]  servername : Pointer to servername.
* @param[in]  datetime : Pointer to datetime.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERCERTIFICATEX509VALIDATOR::ValidateAllPathsInternal(XVECTOR<XBUFFER*>* certificatechain, XVECTOR<XBUFFER*>* trustedroots, CIPHERCERTIFICATEX509VALIDATOR_PURPOSE purpose, XCHAR* servername, XDATETIME* datetime)
{
  End();

  if(!certificatechain || certificatechain->IsEmpty() || !certificatechain->Get(0) ||
     !trustedroots || trustedroots->IsEmpty() ||
     (certificatechain->GetSize() > CIPHERCERTIFICATEX509VALIDATOR_MAXCHAINSIZE))
    {
      return ValidateInternal(certificatechain, trustedroots, purpose, servername, datetime);
    }

  XVECTOR<XBUFFER*> intermediates;
  for(XDWORD c=1; c<certificatechain->GetSize(); c++)
    {
      if(!intermediates.Add(certificatechain->Get(c)))
        {
          return SetError(CIPHERCERTIFICATEX509VALIDATOR_ERROR_INVALIDCERTIFICATE);
        }
    }

  CIPHERCERTIFICATEX509PATHBUILDER pathbuilder;
  XVECTOR<XVECTOR<XBUFFER*>*>      paths;

  if(!pathbuilder.BuildAll((*certificatechain->Get(0)), &intermediates, trustedroots, paths,
                           policy.GetMaximumChainDepth(), CIPHERCERTIFICATEX509PATHBUILDER_MAX_PATHS))
    {
      return ValidateInternal(certificatechain, trustedroots, purpose, servername, datetime);
    }

  XDWORD firstfailedpath = 0;
  bool   havefailedpath = false;

  for(XDWORD c=0; c<paths.GetSize(); c++)
    {
      XVECTOR<XBUFFER*>* path = paths.Get(c);
      if(!path) continue;

      if(ValidateInternal(path, trustedroots, purpose, servername, datetime))
        {
          CIPHERCERTIFICATEX509PATHBUILDER::Paths_Delete(paths);
          return true;
        }

      if(!havefailedpath)
        {
          firstfailedpath = c;
          havefailedpath  = true;
        }
    }

  // Preserve a deterministic validation error and decoded chain for diagnostics/AIA consumers.
  // ValidateInternal owns its decoded copy, so deleting the builder paths afterwards is safe.
  bool status = false;
  if(havefailedpath && paths.Get(firstfailedpath))
    {
      status = ValidateInternal(paths.Get(firstfailedpath), trustedroots, purpose, servername, datetime);
    }
   else
    {
      status = ValidateInternal(certificatechain, trustedroots, purpose, servername, datetime);
    }

  CIPHERCERTIFICATEX509PATHBUILDER::Paths_Delete(paths);
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHERCERTIFICATEX509VALIDATIONPOLICY* CIPHERCERTIFICATEX509VALIDATOR::GetPolicy()
* @brief      Get policy
* @ingroup    CIPHER
* 
* @return     CIPHERCERTIFICATEX509VALIDATIONPOLICY* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERCERTIFICATEX509VALIDATIONPOLICY* CIPHERCERTIFICATEX509VALIDATOR::GetPolicy()
{
  return &policy;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void CIPHERCERTIFICATEX509VALIDATOR::SetPolicy(CIPHERCERTIFICATEX509VALIDATIONPOLICY& policy)
* @brief      Set policy
* @ingroup    CIPHER
* 
* @param[in]  policy : Policy value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void CIPHERCERTIFICATEX509VALIDATOR::SetPolicy(CIPHERCERTIFICATEX509VALIDATIONPOLICY& policy)
{
  this->policy = policy;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool CIPHERCERTIFICATEX509VALIDATOR::ValidateInternal(XVECTOR<XBUFFER*>* certificatechain, XVECTOR<XBUFFER*>* trustedroots, CIPHERCERTIFICATEX509VALIDATOR_PURPOSE purpose, XCHAR* servername, XDATETIME* datetime)
* @brief      Validate an ordered X.509 chain for the requested TLS authentication purpose
* @note       INTERNAL
* @ingroup    CIPHER
* 
* @param[in]  certificatechain : Pointer to certificatechain.
* @param[in]  trustedroots : Pointer to trustedroots.
* @param[in]  purpose : Purpose value.
* @param[in]  servername : Pointer to servername.
* @param[in]  datetime : Pointer to datetime.
*
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERCERTIFICATEX509VALIDATOR::ValidateInternal(XVECTOR<XBUFFER*>* certificatechain, XVECTOR<XBUFFER*>* trustedroots, CIPHERCERTIFICATEX509VALIDATOR_PURPOSE purpose, XCHAR* servername, XDATETIME* datetime)
{
  End();

  if(!certificatechain || certificatechain->IsEmpty() || !trustedroots || trustedroots->IsEmpty() ||
     ((purpose == CIPHERCERTIFICATEX509VALIDATOR_PURPOSE_SERVER_AUTH) && (!servername || !servername[0])))
    {
      return SetError(CIPHERCERTIFICATEX509VALIDATOR_ERROR_INVALIDPARAMETER);
    }

  // Reject an oversized certificate_list before decoding a single certificate: a malicious server could otherwise
  // force unbounded CPU/memory work with a chain that was always going to be rejected as too long.
  if((certificatechain->GetSize() > CIPHERCERTIFICATEX509VALIDATOR_MAXCHAINSIZE) ||
     (certificatechain->GetSize() > policy.GetMaximumChainDepth()))
    {
      return SetError(CIPHERCERTIFICATEX509VALIDATOR_ERROR_PATHLENGTH);
    }

  for(XDWORD c=0; c<certificatechain->GetSize(); c++)
    {
      XBUFFER* certificateDER = certificatechain->Get(c);
      CIPHERCERTIFICATEX509* certificate;

      if(!certificateDER)
        {
          return SetError(CIPHERCERTIFICATEX509VALIDATOR_ERROR_INVALIDPARAMETER);
        }

      certificate = GEN_NEW CIPHERCERTIFICATEX509();
      if(!certificate)
        {
          return SetError(CIPHERCERTIFICATEX509VALIDATOR_ERROR_INVALIDCERTIFICATE);
        }

      if(!certificate->Decode((*certificateDER)) || !certificates.Add(certificate))
        {
          GEN_DELETE certificate;
          return SetError(CIPHERCERTIFICATEX509VALIDATOR_ERROR_INVALIDCERTIFICATE);
        }
    }

  for(XDWORD c=0; c<trustedroots->GetSize(); c++)
    {
      XBUFFER* rootDER = trustedroots->Get(c);
      CIPHERCERTIFICATEX509* root;

      if(!rootDER || rootDER->IsEmpty()) continue;

      root = GEN_NEW CIPHERCERTIFICATEX509();
      if(!root)
        {
          return SetError(CIPHERCERTIFICATEX509VALIDATOR_ERROR_INVALIDCERTIFICATE);
        }

      // A native trust store can contain certificates using formats or algorithms that this
      // build of GEN does not support.  Such an entry is not a usable trust anchor, but it is
      // unrelated to the path being checked and therefore must not reject that path globally.
      if(!root->Decode((*rootDER)))
        {
          GEN_DELETE root;
          continue;
        }

      if(root->GetPublicCipherKey() && (root->GetPublicCipherKey()->GetType()==CIPHERKEYTYPE_RSA_PUBLIC) &&
         ((XDWORD)root->GetPublicCipherKey()->GetSizeInBits()<policy.GetMinimumRSAKeyBits()))
        {
          GEN_DELETE root;
          continue;
        }

      if(!roots.Add(root))
        {
          GEN_DELETE root;
          return SetError(CIPHERCERTIFICATEX509VALIDATOR_ERROR_INVALIDCERTIFICATE);
        }
    }

  if(roots.IsEmpty()) return SetError(CIPHERCERTIFICATEX509VALIDATOR_ERROR_UNTRUSTEDROOT);

  CIPHERCERTIFICATEX509* leaf = certificates.Get(0);
  if(!leaf)
    {
      return SetError(CIPHERCERTIFICATEX509VALIDATOR_ERROR_INVALIDCERTIFICATE);
    }

  for(XDWORD c=0; c<certificates.GetSize(); c++)
    {
      CIPHERCERTIFICATEX509* certificate = certificates.Get(c);

      if(!certificate)
        {
          return SetError(CIPHERCERTIFICATEX509VALIDATOR_ERROR_INVALIDCERTIFICATE);
        }

      if(certificate->HasUnknownCriticalExtension())
        {
          return SetError(CIPHERCERTIFICATEX509VALIDATOR_ERROR_UNKNOWNCRITICALEXTENSION);
        }

      if(!policy.GetAllowExpired() && datetime)
        {
          if(!certificate->IsValidDates(datetime))
            {
              return SetError(CIPHERCERTIFICATEX509VALIDATOR_ERROR_INVALIDDATE);
            }
        }
       else if(!policy.GetAllowExpired() && !certificate->IsValidDates())
        {
          return SetError(CIPHERCERTIFICATEX509VALIDATOR_ERROR_INVALIDDATE);
        }

      if((c > 0) && (!certificate->IsCertificateAuthority() ||
                     (certificate->HasKeyUsage() && !certificate->IsKeyUsageCertificateSign()) ||
                     (certificate->HasExtendedKeyUsage() &&
                      ((purpose == CIPHERCERTIFICATEX509VALIDATOR_PURPOSE_SERVER_AUTH)?
                        (policy.GetRequireServerAuthEKU() && !certificate->IsExtendedKeyUsageServerAuthentication()):
                        (policy.GetRequireClientAuthEKU() && !certificate->IsExtendedKeyUsageClientAuthentication())))))
        {
          return SetError(CIPHERCERTIFICATEX509VALIDATOR_ERROR_INVALIDCA);
        }

      if((c > 0) && (certificate->GetBasicConstraintsPathLength() >= 0) &&
         (CIPHERCERTIFICATEX509VALIDATOR_NonSelfIssuedCABelow(certificates, c) >
          (XDWORD)certificate->GetBasicConstraintsPathLength()))
        {
          return SetError(CIPHERCERTIFICATEX509VALIDATOR_ERROR_PATHLENGTH);
        }


      CIPHERKEY* publickey = certificate->GetPublicCipherKey();
      if(publickey && (publickey->GetType() == CIPHERKEYTYPE_RSA_PUBLIC) &&
         ((XDWORD)publickey->GetSizeInBits() < policy.GetMinimumRSAKeyBits()))
        {
          return SetError(CIPHERCERTIFICATEX509VALIDATOR_ERROR_WEAKKEY);
        }
    }

  if(leaf->IsCertificateAuthority() ||
     (leaf->HasKeyUsage() && !leaf->IsKeyUsageDigitalSignature()) ||
     (leaf->HasExtendedKeyUsage() &&
      ((purpose == CIPHERCERTIFICATEX509VALIDATOR_PURPOSE_SERVER_AUTH)?
        (policy.GetRequireServerAuthEKU() && !leaf->IsExtendedKeyUsageServerAuthentication()):
        (policy.GetRequireClientAuthEKU() && !leaf->IsExtendedKeyUsageClientAuthentication()))))
    {
      return SetError(CIPHERCERTIFICATEX509VALIDATOR_ERROR_INVALIDKEYUSAGE);
    }

  if((purpose == CIPHERCERTIFICATEX509VALIDATOR_PURPOSE_SERVER_AUTH) && !leaf->IsServerNameValid(servername))
    {
      return SetError(CIPHERCERTIFICATEX509VALIDATOR_ERROR_INVALIDNAME);
    }

  // Constraints are cumulative: every CA constrains every certificate below it in the selected path.
  for(XDWORD issuerindex=1; issuerindex<certificates.GetSize(); issuerindex++)
    {
      CIPHERCERTIFICATEX509* issuer = certificates.Get(issuerindex);
      if(!issuer || !issuer->HasNameConstraints()) continue;
      for(XDWORD subjectindex=0; subjectindex<issuerindex; subjectindex++)
        {
          CIPHERCERTIFICATEX509* subject=certificates.Get(subjectindex);
          if((subjectindex>0) && CIPHERCERTIFICATEX509VALIDATOR_IsSelfIssued(subject)) continue;
          if(!issuer->AreNamesPermitted(subject))
            return SetError(CIPHERCERTIFICATEX509VALIDATOR_ERROR_NAMECONSTRAINT);
        }
    }

  for(XDWORD c=0; c+1<certificates.GetSize(); c++)
    {
      CIPHERCERTIFICATEX509* certificate = certificates.Get(c);
      CIPHERCERTIFICATEX509* issuer      = certificates.Get(c+1);

      if(!certificate->GetIssuerData()->Compare((*issuer->GetSubjectData())) ||
         !CIPHERCERTIFICATEX509VALIDATOR_IssuerIdentifierMatches(certificate, issuer))
        {
          return SetError(CIPHERCERTIFICATEX509VALIDATOR_ERROR_INVALIDCA);
        }

      if(!IsSignatureAlgorithmSupported(certificate))
        {
          return SetError(CIPHERCERTIFICATEX509VALIDATOR_ERROR_UNSUPPORTEDALGORITHM);
        }

      if(!certificate->VerifySignature(issuer->GetPublicCipherKey()))
        {
          return SetError(CIPHERCERTIFICATEX509VALIDATOR_ERROR_INVALIDSIGNATURE);
        }
    }

  CIPHERCERTIFICATEX509* chainend = certificates.GetLast();
  if(!chainend)
    {
      return SetError(CIPHERCERTIFICATEX509VALIDATOR_ERROR_INVALIDCERTIFICATE);
    }

  CIPHERCERTIFICATEX509VALIDATOR_ERROR rooterror = CIPHERCERTIFICATEX509VALIDATOR_ERROR_UNTRUSTEDROOT;
  bool                                 rooterrorfound = false;

  for(XDWORD c=0; c<roots.GetSize(); c++)
    {
      CIPHERCERTIFICATEX509* root = roots.Get(c);
      if(!root || root->HasUnknownCriticalExtension())
        {
          continue;
        }

      if(chainend->GetCertificateData()->Compare((*root->GetCertificateData())))
        {
          if(!CIPHERCERTIFICATEX509VALIDATOR_RootConstraintsPermit(root, certificates, true))
            {
              if(!rooterrorfound) rooterror = CIPHERCERTIFICATEX509VALIDATOR_ERROR_NAMECONSTRAINT;
              rooterrorfound = true;
              continue;
            }
          error = CIPHERCERTIFICATEX509VALIDATOR_ERROR_NONE;
          return true;
        }

      if(chainend->GetSubjectData()->Compare((*root->GetSubjectData())) &&
         root->IsCertificateAuthority() &&
         (!root->HasKeyUsage() || root->IsKeyUsageCertificateSign()) &&
         IsSamePublicKey(chainend->GetPublicCipherKey(), root->GetPublicCipherKey()))
        {
          if(!CIPHERCERTIFICATEX509VALIDATOR_RootConstraintsPermit(root, certificates, true))
            {
              if(!rooterrorfound) rooterror = CIPHERCERTIFICATEX509VALIDATOR_ERROR_NAMECONSTRAINT;
              rooterrorfound = true;
              continue;
            }
          error = CIPHERCERTIFICATEX509VALIDATOR_ERROR_NONE;
          return true;
        }

      if(!chainend->GetIssuerData()->Compare((*root->GetSubjectData())) ||
         !CIPHERCERTIFICATEX509VALIDATOR_IssuerIdentifierMatches(chainend, root))
        {
          continue;
        }

      if(!root->IsCertificateAuthority() ||
         (root->HasKeyUsage() && !root->IsKeyUsageCertificateSign()))
        {
          if(!rooterrorfound) rooterror = CIPHERCERTIFICATEX509VALIDATOR_ERROR_INVALIDCA;
          rooterrorfound = true;
          continue;
        }

      if((root->GetBasicConstraintsPathLength() >= 0) &&
         (CIPHERCERTIFICATEX509VALIDATOR_NonSelfIssuedCABelow(certificates, certificates.GetSize()) >
          (XDWORD)root->GetBasicConstraintsPathLength()))
        {
          if(!rooterrorfound) rooterror = CIPHERCERTIFICATEX509VALIDATOR_ERROR_PATHLENGTH;
          rooterrorfound = true;
          continue;
        }

      if(!IsSignatureAlgorithmSupported(chainend))
        {
          if(!rooterrorfound) rooterror = CIPHERCERTIFICATEX509VALIDATOR_ERROR_UNSUPPORTEDALGORITHM;
          rooterrorfound = true;
          continue;
        }

      if(chainend->VerifySignature(root->GetPublicCipherKey()))
        {
          if(!CIPHERCERTIFICATEX509VALIDATOR_RootConstraintsPermit(root, certificates, false))
            {
              if(!rooterrorfound) rooterror = CIPHERCERTIFICATEX509VALIDATOR_ERROR_NAMECONSTRAINT;
              rooterrorfound = true;
              continue;
            }
          error = CIPHERCERTIFICATEX509VALIDATOR_ERROR_NONE;
          return true;
        }

      if(!rooterrorfound) rooterror = CIPHERCERTIFICATEX509VALIDATOR_ERROR_INVALIDSIGNATURE;
      rooterrorfound = true;
    }

  return SetError(rooterrorfound?rooterror:CIPHERCERTIFICATEX509VALIDATOR_ERROR_UNTRUSTEDROOT);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         CIPHERCERTIFICATEX509VALIDATOR_ERROR CIPHERCERTIFICATEX509VALIDATOR::GetError()
* @brief      Get the last validation error
* @ingroup    CIPHER
*
* @return     CIPHERCERTIFICATEX509VALIDATOR_ERROR : Validation result.
*
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERCERTIFICATEX509VALIDATOR_ERROR CIPHERCERTIFICATEX509VALIDATOR::GetError()
{
  return error;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         CIPHERCERTIFICATEX509* CIPHERCERTIFICATEX509VALIDATOR::GetLeafCertificate()
* @brief      Get the validated leaf certificate
* @ingroup    CIPHER
*
* @return     CIPHERCERTIFICATEX509* : Leaf certificate, or NULL if unavailable.
*
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERCERTIFICATEX509* CIPHERCERTIFICATEX509VALIDATOR::GetLeafCertificate()
{
  return certificates.IsEmpty()?NULL:certificates.Get(0);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XVECTOR<CIPHERCERTIFICATEX509*>* CIPHERCERTIFICATEX509VALIDATOR::GetCertificateChain()
* @brief      Get the decoded server certificate chain
* @ingroup    CIPHER
*
* @return     XVECTOR<CIPHERCERTIFICATEX509*>* : Certificate chain.
*
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<CIPHERCERTIFICATEX509*>* CIPHERCERTIFICATEX509VALIDATOR::GetCertificateChain()
{
  return &certificates;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void CIPHERCERTIFICATEX509VALIDATOR::End()
* @brief      Release decoded certificates
* @ingroup    CIPHER
*
* --------------------------------------------------------------------------------------------------------------------*/
void CIPHERCERTIFICATEX509VALIDATOR::End()
{
  certificates.DeleteContents();
  certificates.DeleteAll();

  roots.DeleteContents();
  roots.DeleteAll();

  error = CIPHERCERTIFICATEX509VALIDATOR_ERROR_NONE;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool CIPHERCERTIFICATEX509VALIDATOR::IsSignatureAlgorithmSupported(CIPHERCERTIFICATEX509* certificate)
* @brief      Check the accepted X.509 signature profile
* @note       INTERNAL
* @ingroup    CIPHER
*
* @param[in]  certificate : Certificate to check.
*
* @return     bool : true if the algorithm is supported and accepted; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERCERTIFICATEX509VALIDATOR::IsSignatureAlgorithmSupported(CIPHERCERTIFICATEX509* certificate)
{
  if(!certificate)
    {
      return false;
    }

  if(!policy.IsSignatureAlgorithmAllowed(certificate->GetAlgorithmType())) return false;

  switch(certificate->GetAlgorithmType())
    {
      case CIPHERCERTIFICATEX509_ALGORITHM_TYPE_SHA256WITHRSAENCRYPTION :
      case CIPHERCERTIFICATEX509_ALGORITHM_TYPE_SHA384WITHRSAENCRYPTION :
      case CIPHERCERTIFICATEX509_ALGORITHM_TYPE_SHA512WITHRSAENCRYPTION : return true;
      case CIPHERCERTIFICATEX509_ALGORITHM_TYPE_ECDSAWITHSHA256         :
      case CIPHERCERTIFICATEX509_ALGORITHM_TYPE_ECDSAWITHSHA384         :
      case CIPHERCERTIFICATEX509_ALGORITHM_TYPE_ECDSAWITHSHA512         : return true;

      #ifdef CIPHER_ASYMMETRIC_ED25519_ACTIVE
      case CIPHERCERTIFICATEX509_ALGORITHM_TYPE_ED25519                 : return true;
      #endif

      case CIPHERCERTIFICATEX509_ALGORITHM_TYPE_RSASSAPSS               : return (certificate->GetRSASSAPSSHashType() !=
                                                                                  CIPHERCERTIFICATEX509_RSASSAPSS_HASH_TYPE_UNKNOWN);
      case CIPHERCERTIFICATEX509_ALGORITHM_TYPE_SHA1WITHRSAENCRYPTION   :
      case CIPHERCERTIFICATEX509_ALGORITHM_TYPE_ECDSAWITHSHA1           : return policy.GetAllowSHA1();
                                                                    default : break;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool CIPHERCERTIFICATEX509VALIDATOR::IsSamePublicKey(CIPHERKEY* key1, CIPHERKEY* key2)
* @brief      Check whether two supported public keys contain the same values
* @note       INTERNAL
* @ingroup    CIPHER
*
* @param[in]  key1 : First public key.
* @param[in]  key2 : Second public key.
*
* @return     bool : true if both keys contain the same values; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERCERTIFICATEX509VALIDATOR::IsSamePublicKey(CIPHERKEY* key1, CIPHERKEY* key2)
{
  if(!key1 || !key2 || (key1->GetType() != key2->GetType())) return false;

  switch(key1->GetType())
    {
      case CIPHERKEYTYPE_RSA_PUBLIC : { CIPHERKEYPUBLICRSA* keyRSA1 = (CIPHERKEYPUBLICRSA*)key1;
                                        CIPHERKEYPUBLICRSA* keyRSA2 = (CIPHERKEYPUBLICRSA*)key2;
                                        XMPINTEGER          modulus1;
                                        XMPINTEGER          exponent1;
                                        XMPINTEGER          modulus2;
                                        XMPINTEGER          exponent2;

                                        if(!keyRSA1->Get(modulus1, exponent1) || !keyRSA2->Get(modulus2, exponent2)) return false;

                                        return (!modulus1.CompareSignedValues(modulus2) &&
                                                !exponent1.CompareSignedValues(exponent2));
                                      }
                                 break;

      case CIPHERKEYTYPE_ECDSA_SECP256R1_PUBLIC :
      case CIPHERKEYTYPE_ECDSA_SECP384R1_PUBLIC :
      case CIPHERKEYTYPE_ECDSA_SECP521R1_PUBLIC : { CIPHERKEYECDSA* keyECDSA1 = (CIPHERKEYECDSA*)key1;
                                                    CIPHERKEYECDSA* keyECDSA2 = (CIPHERKEYECDSA*)key2;

                                                    if(!keyECDSA1->Get() || !keyECDSA2->Get()) return false;

                                                    return keyECDSA1->Get()->Compare((*keyECDSA2->Get()));
                                                  }
                                             break;

      #ifdef CIPHER_ASYMMETRIC_ED25519_ACTIVE

      case CIPHERKEYTYPE_ED25519_PUBLIC : { CIPHERKEYSYMMETRICAL* keyEd1=(CIPHERKEYSYMMETRICAL*)key1;
                                             CIPHERKEYSYMMETRICAL* keyEd2=(CIPHERKEYSYMMETRICAL*)key2;
                                             if(!keyEd1->Get() || !keyEd2->Get()) return false;
                                             return keyEd1->Get()->Compare((*keyEd2->Get()));
                                           }
                                      break;

      #endif

                               default : break;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool CIPHERCERTIFICATEX509VALIDATOR::SetError(CIPHERCERTIFICATEX509VALIDATOR_ERROR error)
* @brief      Set a validation error
* @note       INTERNAL
* @ingroup    CIPHER
*
* @param[in]  error : Error value.
*
* @return     bool : Always false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERCERTIFICATEX509VALIDATOR::SetError(CIPHERCERTIFICATEX509VALIDATOR_ERROR error)
{
  this->error = error;
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void CIPHERCERTIFICATEX509VALIDATOR::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    CIPHER
*
* --------------------------------------------------------------------------------------------------------------------*/
void CIPHERCERTIFICATEX509VALIDATOR::Clean()
{
  error = CIPHERCERTIFICATEX509VALIDATOR_ERROR_NONE;
}
