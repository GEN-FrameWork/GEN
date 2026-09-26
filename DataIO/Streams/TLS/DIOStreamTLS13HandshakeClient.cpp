/**-------------------------------------------------------------------------------------------------------------------
*
* @file       DIOStreamTLS13HandshakeClient.cpp
*
* @class      DIOSTREAMTLS13HANDSHAKECLIENT
* @brief      Data Input/Output Stream TLS 1.3 Client Handshake class
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

#include "DIOStreamTLS13HandshakeClient.h"
#include "DIOStreamTLSConfig.h"
#include "DIOStreamTLSSignature.h"
#include "DIOStreamTLSMessagesHandShakeClientHello.h"
#include "DIOStreamTLSMessagesHandShakeServerHello.h"

#include <string.h>

#include "XFactory.h"
#include "XRand.h"
#include "XString.h"
#include "XTrace.h"

#include "CipherCertificateX509.h"
#include "CipherCertificateX509Revocation.h"
#include "DIOStreamTLSAIAFetcher.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         static bool DIOSTREAMTLS13_HANDSHAKECLIENT_SignatureSchemeOffered(XVECTOR<XWORD>& offered, XWORD scheme)
* @brief      Signature scheme offered
* @ingroup    DATAIO
* 
* @param[in]  offered : Offered value.
* @param[in]  scheme : Scheme value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool DIOSTREAMTLS13_HANDSHAKECLIENT_SignatureSchemeOffered(XVECTOR<XWORD>& offered, XWORD scheme)
{
  for(XDWORD c=0; c<offered.GetSize(); c++) if(offered.Get(c) == scheme) return true;
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         static XWORD DIOSTREAMTLS13_HANDSHAKECLIENT_CertificateSignatureScheme(CIPHERCERTIFICATEX509& certificate)
* @brief      Certificate signature scheme
* @ingroup    DATAIO
* 
* @param[in]  certificate : Certificate value.
* 
* @return     XWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static XWORD DIOSTREAMTLS13_HANDSHAKECLIENT_CertificateSignatureScheme(CIPHERCERTIFICATEX509& certificate)
{
  switch(certificate.GetAlgorithmType())
    {
      case CIPHERCERTIFICATEX509_ALGORITHM_TYPE_SHA256WITHRSAENCRYPTION : return DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PKCS1_SHA256;
      case CIPHERCERTIFICATEX509_ALGORITHM_TYPE_SHA384WITHRSAENCRYPTION : return DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PKCS1_SHA384;
      case CIPHERCERTIFICATEX509_ALGORITHM_TYPE_SHA512WITHRSAENCRYPTION : return DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PKCS1_SHA512;
      case CIPHERCERTIFICATEX509_ALGORITHM_TYPE_ECDSAWITHSHA256         : return DIOSTREAMTLS_MSG_SIGNATURESCHEME_ECDSA_SECP256R1_SHA256;
      case CIPHERCERTIFICATEX509_ALGORITHM_TYPE_ECDSAWITHSHA384         : return DIOSTREAMTLS_MSG_SIGNATURESCHEME_ECDSA_SECP384R1_SHA384;
      case CIPHERCERTIFICATEX509_ALGORITHM_TYPE_ECDSAWITHSHA512         : return DIOSTREAMTLS_MSG_SIGNATURESCHEME_ECDSA_SECP521R1_SHA512;
      case CIPHERCERTIFICATEX509_ALGORITHM_TYPE_ED25519                 : return DIOSTREAMTLS_MSG_SIGNATURESCHEME_ED25519;
      case CIPHERCERTIFICATEX509_ALGORITHM_TYPE_RSASSAPSS               :
        {
          switch(certificate.GetRSASSAPSSHashType())
            {
              case CIPHERCERTIFICATEX509_RSASSAPSS_HASH_TYPE_SHA256 : return DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA256;
              case CIPHERCERTIFICATEX509_RSASSAPSS_HASH_TYPE_SHA384 : return DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA384;
              case CIPHERCERTIFICATEX509_RSASSAPSS_HASH_TYPE_SHA512 : return DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA512;
              default                                                : break;
            }
        }
        break;
      default : break;
    }
  return 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         static bool DIOSTREAMTLS13_HANDSHAKECLIENT_CertificateChainCompatible(XVECTOR<XBUFFER*>* chain, XVECTOR<XWORD>& offered)
* @brief      Certificate chain compatible
* @ingroup    DATAIO
* 
* @param[in]  chain : Pointer to chain.
* @param[in]  offered : Offered value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool DIOSTREAMTLS13_HANDSHAKECLIENT_CertificateChainCompatible(XVECTOR<XBUFFER*>* chain, XVECTOR<XWORD>& offered)
{
  if(!chain || chain->IsEmpty() || offered.IsEmpty()) return false;
  for(XDWORD c=0; c<chain->GetSize(); c++)
    {
      XBUFFER* certificateDER = chain->Get(c);
      CIPHERCERTIFICATEX509 certificate;
      if(!certificateDER || certificateDER->IsEmpty() || !certificate.Decode((*certificateDER))) return false;
      if(certificate.GetAlgorithmType() == CIPHERCERTIFICATEX509_ALGORITHM_TYPE_RSASSAPSS)
        {
          XWORD RSAE=0, PSS=0;
          switch(certificate.GetRSASSAPSSHashType())
            {
              case CIPHERCERTIFICATEX509_RSASSAPSS_HASH_TYPE_SHA256 : RSAE=DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA256; PSS=DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA256; break;
              case CIPHERCERTIFICATEX509_RSASSAPSS_HASH_TYPE_SHA384 : RSAE=DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA384; PSS=DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA384; break;
              case CIPHERCERTIFICATEX509_RSASSAPSS_HASH_TYPE_SHA512 : RSAE=DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA512; PSS=DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA512; break;
              default                                                : return false;
            }
          if(!DIOSTREAMTLS13_HANDSHAKECLIENT_SignatureSchemeOffered(offered, RSAE) &&
             !DIOSTREAMTLS13_HANDSHAKECLIENT_SignatureSchemeOffered(offered, PSS)) return false;
        }
       else
        {
          XWORD scheme=DIOSTREAMTLS13_HANDSHAKECLIENT_CertificateSignatureScheme(certificate);
          if(!scheme || !DIOSTREAMTLS13_HANDSHAKECLIENT_SignatureSchemeOffered(offered, scheme)) return false;
        }
    }
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         static bool DIOSTREAMTLS13_HANDSHAKECLIENT_CertificateAuthorityCompatible(XVECTOR<XBUFFER*>* chain, XVECTOR<XBUFFER*>& authorities)
* @brief      Certificate authority compatible
* @ingroup    DATAIO
* 
* @param[in]  chain : Pointer to chain.
* @param[in]  authorities : Pointer to authorities.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool DIOSTREAMTLS13_HANDSHAKECLIENT_CertificateAuthorityCompatible(XVECTOR<XBUFFER*>* chain, XVECTOR<XBUFFER*>& authorities)
{
  if(authorities.IsEmpty()) return true;
  if(!chain || chain->IsEmpty()) return false;

  for(XDWORD c=0; c<chain->GetSize(); c++)
    {
      XBUFFER* certificateDER=chain->Get(c);
      CIPHERCERTIFICATEX509 certificate;
      if(!certificateDER || !certificate.Decode((*certificateDER))) return false;
      XBUFFER* names[2]={ certificate.GetIssuerData(), certificate.GetSubjectData() };
      for(int n=0;n<2;n++)
        {
          if(!names[n] || names[n]->IsEmpty()) continue;
          for(XDWORD a=0;a<authorities.GetSize();a++)
            {
              XBUFFER* authority=authorities.Get(a);
              if(authority && names[n]->Compare((*authority))) return true;
            }
        }
    }
  return false;
}


/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS13HANDSHAKECLIENT::DIOSTREAMTLS13HANDSHAKECLIENT()
* @brief      Constructor of class
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS13HANDSHAKECLIENT::DIOSTREAMTLS13HANDSHAKECLIENT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS13HANDSHAKECLIENT::~DIOSTREAMTLS13HANDSHAKECLIENT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS13HANDSHAKECLIENT::~DIOSTREAMTLS13HANDSHAKECLIENT()
{
  End();
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKECLIENT::Ini(DIOSTREAMTLS13SESSION* session)
* @brief      Initialize the client handshake over a role-neutral session
* @ingroup    DATAIO
*
* @param[in]  session : Initialized client session. It is not owned by this class.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKECLIENT::Ini(DIOSTREAMTLS13SESSION* session)
{
  End();

  if(!session || !session->IsIni() ||
     (session->GetRole() != DIOSTREAMTLSKEYSCHEDULE_ROLE_CLIENT))
    {
      return false;
    }

  this->session                    = session;
  state                            = DIOSTREAMTLS13HANDSHAKECLIENT_STATE_NONE;
  isini                            = true;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS13HANDSHAKECLIENT::End()
* @brief      End the client handshake and release decoded peer messages
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS13HANDSHAKECLIENT::End()
{
  if(servercertificate)
    {
      GEN_DELETE servercertificate;
      servercertificate = NULL;
    }

  if(servercertificateverify)
    {
      GEN_DELETE servercertificateverify;
      servercertificateverify = NULL;
    }

  legacysessionid.Delete();

  ciphersuites.DeleteAll();
  supportedgroups.DeleteAll();
  signatureschemes.DeleteAll();
  certificatesignatureschemes.DeleteAll();
  applicationprotocols.DeleteAll();
  applicationprotocolsraw.DeleteContents();
  applicationprotocolsraw.DeleteAll();

  offeredciphersuites.DeleteAll();
  offeredsupportedgroups.DeleteAll();
  offeredkeysharegroups.DeleteAll();
  offeredsignatureschemes.DeleteAll();
  requestedclientsignatureschemes.DeleteAll();
  requestedclientcertificatesignatureschemes.DeleteAll();
  requestedclientcertificateauthorities.DeleteContents();
  requestedclientcertificateauthorities.DeleteAll();
  offeredapplicationprotocols.DeleteAll();
  offeredapplicationprotocolsraw.DeleteContents();
  offeredapplicationprotocolsraw.DeleteAll();

  firstclienthello.Delete();

  trustedroots.DeleteContents();
  trustedroots.DeleteAll();
  certificatevalidator.End();
  expectedservername.Empty();

  session                      = NULL;
  config                       = NULL;
  state                        = DIOSTREAMTLS13HANDSHAKECLIENT_STATE_NONE;
  isini                        = false;
  certificaterequested         = false;
  authenticationconfigured    = false;
  serverauthenticated          = false;
  authenticationerror         = DIOSTREAMTLS13HANDSHAKECLIENT_AUTHENTICATIONERROR_NONE;
  certificatevalidationerror  = CIPHERCERTIFICATEX509VALIDATOR_ERROR_NONE;
  hasvalidationdatetime       = false;
  applicationprotocolnegotiated = false;
  applicationprotocol           = DIOSTREAMTLS_ALPN_TYPE_UNKNOWN;
  applicationprotocolraw.Delete();
  resumptionoffered             = false;
  resumptionaccepted            = false;
  if(!resumptionpsk.IsEmpty()) resumptionpsk.FillBuffer(0);
  resumptionpsk.Delete();
  resumptionciphersuite         = 0;
  earlydataoffered              = false;
  earlydataaccepted             = false;
  maximumearlydatasize          = 0;
  preparedearlydatasize          = 0;
  currentkeysharegroup          = 0;
  helloretryrequestprocessed    = false;
  aiafetchactive                = true;
  aiafetchtimeout                = DIOSTREAMTLSAIAFETCHER_TIMEOUT;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKECLIENT::IsIni()
* @brief      Check whether the client handshake is initialized
* @ingroup    DATAIO
*
* @return     bool : true if the condition is met; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKECLIENT::IsIni()
{
  return isini;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS13HANDSHAKECLIENT_STATE DIOSTREAMTLS13HANDSHAKECLIENT::GetState()
* @brief      Get the current client handshake state
* @ingroup    DATAIO
*
* @return     DIOSTREAMTLS13HANDSHAKECLIENT_STATE : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS13HANDSHAKECLIENT_STATE DIOSTREAMTLS13HANDSHAKECLIENT::GetState()
{
  return state;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKECLIENT::IsServerFinishedVerified()
* @brief      Check whether the server Finished has been verified
* @ingroup    DATAIO
*
* @return     bool : true if the condition is met; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKECLIENT::IsServerFinishedVerified()
{
  return ((state == DIOSTREAMTLS13HANDSHAKECLIENT_STATE_SERVERFINISHED_VERIFIED) ||
          (state == DIOSTREAMTLS13HANDSHAKECLIENT_STATE_HANDSHAKE_COMPLETED));
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKECLIENT::IsHandshakeCompleted()
* @brief      Check whether both application traffic directions are active
* @ingroup    DATAIO
*
* @return     bool : true if the condition is met; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKECLIENT::IsHandshakeCompleted()
{
  return (state == DIOSTREAMTLS13HANDSHAKECLIENT_STATE_HANDSHAKE_COMPLETED);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKECLIENT::IsCertificateRequested()
* @brief      Check whether the server requested a client certificate
* @ingroup    DATAIO
*
* @return     bool : true if the condition is met; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKECLIENT::IsCertificateRequested()
{
  return certificaterequested;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKECLIENT::IsServerAuthenticated()
* @brief      Check whether X.509 and CertificateVerify authenticated the server
* @ingroup    DATAIO
*
* @return     bool : true if the server identity is authenticated; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKECLIENT::IsServerAuthenticated()
{
  return serverauthenticated;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKECLIENT::Capabilities_Set(DIOSTREAMTLSCONFIG* config)
* @brief      Copy the ordered TLS client capabilities from a stream configuration
* @ingroup    DATAIO
*
* @param[in]  config : TLS stream configuration.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKECLIENT::Capabilities_Set(DIOSTREAMTLSCONFIG* config)
{
  if(!isini || !session || (state != DIOSTREAMTLS13HANDSHAKECLIENT_STATE_NONE) || !config ||
     !config->GetCipherSuites() || config->GetCipherSuites()->IsEmpty() ||
     !config->GetSupportedGroups() || config->GetSupportedGroups()->IsEmpty() ||
     !config->GetSignatureSchemes() || config->GetSignatureSchemes()->IsEmpty() ||
     !config->GetCertificateSignatureSchemes() || config->GetCertificateSignatureSchemes()->IsEmpty())
    {
      return false;
    }

  this->config = config;
  certificatevalidator.SetPolicy((*config->GetCertificateValidationPolicy()));

  ciphersuites.DeleteAll();
  supportedgroups.DeleteAll();
  signatureschemes.DeleteAll();
  certificatesignatureschemes.DeleteAll();
  applicationprotocols.DeleteAll();

  for(XDWORD c=0; c<config->GetCipherSuites()->GetSize(); c++)
    {
      XWORD ciphersuite = config->GetCipherSuites()->Get(c);

      bool supported = (ciphersuite == DIOSTREAMTLS_MSG_CIPHER_AES_128_GCM_SHA256) ||
                       (ciphersuite == DIOSTREAMTLS_MSG_CIPHER_AES_256_GCM_SHA384);

      #ifdef CIPHER_SYMMETRIC_CHACHA20POLY1305_ACTIVE

      supported = supported || (ciphersuite == DIOSTREAMTLS_MSG_CIPHER_CHACHA20_POLY1305_SHA256);

      #endif

      if(!supported) return false;
      if(!ciphersuites.Add(ciphersuite)) return false;
    }

  for(XDWORD c=0; c<config->GetSupportedGroups()->GetSize(); c++)
    {
      XWORD supportedgroup = config->GetSupportedGroups()->Get(c);

      bool supported = (supportedgroup == DIOSTREAMTLS_MSG_CURVEID_X25519) ||
                       (supportedgroup == DIOSTREAMTLS_MSG_CURVEID_SECP256R1) ||
                       (supportedgroup == DIOSTREAMTLS_MSG_CURVEID_SECP384R1) ||
                       (supportedgroup == DIOSTREAMTLS_MSG_CURVEID_SECP521R1);

      #ifdef CIPHER_ASYMMETRIC_MLKEM768_ACTIVE
      supported = supported || (supportedgroup == DIOSTREAMTLS_MSG_CURVEID_SECP256R1MLKEM768);
      #endif

      #ifdef CIPHER_ASYMMETRIC_MLKEM1024_ACTIVE
      supported = supported || (supportedgroup == DIOSTREAMTLS_MSG_CURVEID_SECP384R1MLKEM1024);
      #endif

      #if defined(CIPHER_ASYMMETRIC_X25519_ACTIVE) && defined(CIPHER_ASYMMETRIC_MLKEM768_ACTIVE)
      supported = supported || (supportedgroup == DIOSTREAMTLS_MSG_CURVEID_X25519MLKEM768);
      #endif

      if(!supported || !supportedgroups.Add(supportedgroup)) return false;
    }

  // A signature scheme this build cannot actually verify is SKIPPED, not treated as a fatal configuration error.
  // Advertising only what can be verified is the correct behaviour anyway, and it keeps a configuration that
  // names a newer scheme (for example a DIOSTREAMTLSCONFIG updated ahead of this file) from disabling the whole
  // TLS client: previously a single unrecognised entry made Capabilities_Set() fail, which aborts DIOSTREAMTLS
  // ::Open() before the handshake and so breaks EVERY TLS connection, including plain RSA servers. An empty
  // resulting list is still an error, since a ClientHello without signature algorithms is not usable.
  for(XDWORD c=0; c<config->GetSignatureSchemes()->GetSize(); c++)
    {
      XWORD signaturescheme = config->GetSignatureSchemes()->Get(c);

      switch(signaturescheme)
        {
          #ifdef CIPHER_ASYMMETRIC_ED25519_ACTIVE
          case DIOSTREAMTLS_MSG_SIGNATURESCHEME_ED25519                  :
          #endif
          case DIOSTREAMTLS_MSG_SIGNATURESCHEME_ECDSA_SECP256R1_SHA256 :
          case DIOSTREAMTLS_MSG_SIGNATURESCHEME_ECDSA_SECP384R1_SHA384 :
          case DIOSTREAMTLS_MSG_SIGNATURESCHEME_ECDSA_SECP521R1_SHA512 :
          case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA256    :
          case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA384    :
          case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA512    :
          case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA256     :
          case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA384     :
          case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA512     : break;
                                                             default   : continue;
        }

      if(!signatureschemes.Add(signaturescheme)) return false;
    }

  if(signatureschemes.IsEmpty()) return false;

  for(XDWORD c=0; c<config->GetCertificateSignatureSchemes()->GetSize(); c++)
    {
      XWORD signaturescheme = config->GetCertificateSignatureSchemes()->Get(c);

      switch(signaturescheme)
        {
          #ifdef CIPHER_ASYMMETRIC_ED25519_ACTIVE
          case DIOSTREAMTLS_MSG_SIGNATURESCHEME_ED25519                  :
          #endif
          case DIOSTREAMTLS_MSG_SIGNATURESCHEME_ECDSA_SECP256R1_SHA256 :
          case DIOSTREAMTLS_MSG_SIGNATURESCHEME_ECDSA_SECP384R1_SHA384 :
          case DIOSTREAMTLS_MSG_SIGNATURESCHEME_ECDSA_SECP521R1_SHA512 :
          case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA256 :
          case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA384 :
          case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA512 :
          case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA256  :
          case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA384  :
          case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA512  :
          case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PKCS1_SHA256     :
          case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PKCS1_SHA384     :
          case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PKCS1_SHA512     : break;
                                                            default : continue;
        }

      if(!certificatesignatureschemes.Add(signaturescheme)) return false;
    }

  if(certificatesignatureschemes.IsEmpty()) return false;

  if(config->GetApplicationProtocols())
    {
      for(XDWORD c=0; c<config->GetApplicationProtocols()->GetSize(); c++)
        {
          DIOSTREAMTLS_ALPN_TYPE applicationprotocol = config->GetApplicationProtocols()->Get(c);

          if((applicationprotocol != DIOSTREAMTLS_ALPN_TYPE_HTTP_1_1) &&
             (applicationprotocol != DIOSTREAMTLS_ALPN_TYPE_HTTP_2)) return false;
          if(!applicationprotocols.Add(applicationprotocol)) return false;
        }
    }

  for(XDWORD c=0; c<config->GetApplicationProtocolsCount(); c++)
    {
      XBUFFER* protocol = GEN_NEW XBUFFER();
      if(!protocol || !config->GetApplicationProtocol(c, (*protocol)) || !applicationprotocolsraw.Add(protocol))
        {
          if(protocol) GEN_DELETE protocol;
          return false;
        }
    }

  AIAFetch_Set(config->IsActiveAIAFetch(), config->GetAIAFetchTimeout());

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKECLIENT::SignatureSchemes_WidenECDSA()
* @brief      Last-resort fallback: add the three ECDSA schemes to signature_algorithms for one retry attempt
* @ingroup    DATAIO
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKECLIENT::SignatureSchemes_WidenECDSA()
{
  // Kept for source compatibility. Capabilities_Set() already installs the complete configured policy;
  // a retry must never enable algorithms that the caller did not authorize.
  return isini && config && (state == DIOSTREAMTLS13HANDSHAKECLIENT_STATE_NONE) && !signatureschemes.IsEmpty();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKECLIENT::IsApplicationProtocolNegotiated()
* @brief      Check whether the server selected an ALPN application protocol
* @ingroup    DATAIO
*
* @return     bool : true if the condition is met; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKECLIENT::IsApplicationProtocolNegotiated()
{
  return applicationprotocolnegotiated;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_ALPN_TYPE DIOSTREAMTLS13HANDSHAKECLIENT::GetApplicationProtocol()
* @brief      Get the ALPN application protocol selected by the server
* @ingroup    DATAIO
*
* @return     DIOSTREAMTLS_ALPN_TYPE : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_ALPN_TYPE DIOSTREAMTLS13HANDSHAKECLIENT::GetApplicationProtocol()
{
  return applicationprotocol;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBUFFER* DIOSTREAMTLS13HANDSHAKECLIENT::GetApplicationProtocolRaw()
* @brief      Get application protocol raw
* @ingroup    DATAIO
* 
* @return     XBUFFER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* DIOSTREAMTLS13HANDSHAKECLIENT::GetApplicationProtocolRaw()
{
  return &applicationprotocolraw;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS13HANDSHAKECLIENT::IsSessionResumed()
* @brief      Is session resumed
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKECLIENT::IsSessionResumed()
{
  return resumptionaccepted;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS13HANDSHAKECLIENT::IsEarlyDataOffered()
* @brief      Is early data offered
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKECLIENT::IsEarlyDataOffered() { return earlydataoffered; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS13HANDSHAKECLIENT::IsEarlyDataAccepted()
* @brief      Is early data accepted
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKECLIENT::IsEarlyDataAccepted() { return earlydataaccepted; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOSTREAMTLS13HANDSHAKECLIENT::GetMaximumEarlyDataSize()
* @brief      Get maximum early data size
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAMTLS13HANDSHAKECLIENT::GetMaximumEarlyDataSize() { return maximumearlydatasize; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS13HANDSHAKECLIENT::EarlyData_Prepare(XDWORD size)
* @brief      Early data prepare
* @ingroup    DATAIO
* 
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKECLIENT::EarlyData_Prepare(XDWORD size)
{
  if(state!=DIOSTREAMTLS13HANDSHAKECLIENT_STATE_NONE || !config || !config->IsEarlyDataActive() || !size || size>config->GetMaximumEarlyDataSize()) return false;
  preparedearlydatasize=size;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS13HANDSHAKECLIENT::EarlyData_Protect(XBYTE* data, XDWORD size, XBUFFER& records)
* @brief      Early data protect
* @ingroup    DATAIO
* 
* @param[in]  data : Pointer to data.
* @param[in]  size : Size value.
* @param[in]  records : Records value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKECLIENT::EarlyData_Protect(XBYTE* data, XDWORD size, XBUFFER& records)
{
  if(!earlydataoffered || !maximumearlydatasize || size>maximumearlydatasize) return false;
  return session && session->EarlyData_Protect(data, size, records);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS13HANDSHAKECLIENT::EarlyData_Protect(XBUFFER& data, XBUFFER& records)
* @brief      Early data protect
* @ingroup    DATAIO
* 
* @param[in]  data : Data value.
* @param[in]  records : Records value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKECLIENT::EarlyData_Protect(XBUFFER& data, XBUFFER& records)
{
  return EarlyData_Protect(data.Get(), data.GetSize(), records);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKECLIENT::Authentication_Set(XCHAR* servername, XVECTOR<XBUFFER*>* trustedroots, XDATETIME* datetime)
* @brief      Configure the expected server identity and explicit X.509 trust anchors
* @ingroup    DATAIO
*
* @param[in]  servername : Expected DNS name or IP address.
* @param[in]  trustedroots : DER trust anchors. The buffers are copied and owned by this class.
* @param[in]  datetime : Optional UTC validation time, intended for deterministic protocol vectors.
*
* @return     bool : true if the authentication policy was copied; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKECLIENT::Authentication_Set(XCHAR* servername, XVECTOR<XBUFFER*>* trustedroots, XDATETIME* datetime)
{
  if(!isini || !session || (state != DIOSTREAMTLS13HANDSHAKECLIENT_STATE_NONE) ||
     !servername || !servername[0] ||
     !trustedroots || trustedroots->IsEmpty())
    {
      return SetAuthenticationError(DIOSTREAMTLS13HANDSHAKECLIENT_AUTHENTICATIONERROR_CONFIGURATION);
    }

  this->trustedroots.DeleteContents();
  this->trustedroots.DeleteAll();
  expectedservername.Empty();
  certificatevalidationerror = CIPHERCERTIFICATEX509VALIDATOR_ERROR_NONE;

  for(XDWORD c=0; c<trustedroots->GetSize(); c++)
    {
      XBUFFER* root = trustedroots->Get(c);
      XBUFFER* copy;

      if(!root || root->IsEmpty())
        {
          return SetAuthenticationError(DIOSTREAMTLS13HANDSHAKECLIENT_AUTHENTICATIONERROR_CONFIGURATION);
        }

      copy = GEN_NEW XBUFFER();
      if(!copy || !copy->Add((*root)) || !this->trustedroots.Add(copy))
        {
          if(copy) GEN_DELETE copy;
          return SetAuthenticationError(DIOSTREAMTLS13HANDSHAKECLIENT_AUTHENTICATIONERROR_CONFIGURATION);
        }
    }

  expectedservername = servername;

  hasvalidationdatetime = false;
  if(datetime)
    {
      if(!datetime->IsValidDate() || !validationdatetime.CopyFrom(datetime))
        {
          return SetAuthenticationError(DIOSTREAMTLS13HANDSHAKECLIENT_AUTHENTICATIONERROR_CONFIGURATION);
        }

      hasvalidationdatetime = true;
    }

  authenticationconfigured   = true;
  authenticationerror        = DIOSTREAMTLS13HANDSHAKECLIENT_AUTHENTICATIONERROR_NONE;
  certificatevalidationerror = CIPHERCERTIFICATEX509VALIDATOR_ERROR_NONE;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS13HANDSHAKECLIENT::AIAFetch_Set(bool active, int timeout)
* @brief      Configure best-effort AuthorityInfoAccess fetching for an incomplete certificate chain
* @ingroup    DATAIO
*
* @param[in]  active : true to fetch a missing intermediate via id-ad-caIssuers when the chain the server sent
*             does not reach a trusted root by itself; false to keep the previous strict behaviour.
* @param[in]  timeout : Fetch connect / idle timeout, in seconds.
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS13HANDSHAKECLIENT::AIAFetch_Set(bool active, int timeout)
{
  aiafetchactive  = active;
  aiafetchtimeout = (timeout > 0)?timeout:DIOSTREAMTLSAIAFETCHER_TIMEOUT;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS13HANDSHAKECLIENT_AUTHENTICATIONERROR DIOSTREAMTLS13HANDSHAKECLIENT::GetAuthenticationError()
* @brief      Get the last server authentication error
* @ingroup    DATAIO
*
* @return     DIOSTREAMTLS13HANDSHAKECLIENT_AUTHENTICATIONERROR : Authentication result.
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS13HANDSHAKECLIENT_AUTHENTICATIONERROR DIOSTREAMTLS13HANDSHAKECLIENT::GetAuthenticationError()
{
  return authenticationerror;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         CIPHERCERTIFICATEX509VALIDATOR_ERROR DIOSTREAMTLS13HANDSHAKECLIENT::GetCertificateValidationError()
* @brief      Get the detailed X.509 validation result
* @ingroup    DATAIO
*
* @return     CIPHERCERTIFICATEX509VALIDATOR_ERROR : X.509 validation result.
*
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERCERTIFICATEX509VALIDATOR_ERROR DIOSTREAMTLS13HANDSHAKECLIENT::GetCertificateValidationError()
{
  return certificatevalidationerror;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATE* DIOSTREAMTLS13HANDSHAKECLIENT::GetServerCertificate()
* @brief      Get the retained server certificate message
* @ingroup    DATAIO
*
* @return     DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATE* : Pointer to the requested object; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATE* DIOSTREAMTLS13HANDSHAKECLIENT::GetServerCertificate()
{
  return servercertificate;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEVERIFY* DIOSTREAMTLS13HANDSHAKECLIENT::GetServerCertificateVerify()
* @brief      Get the retained server CertificateVerify message
* @ingroup    DATAIO
*
* @return     DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEVERIFY* : Pointer to the requested object; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEVERIFY* DIOSTREAMTLS13HANDSHAKECLIENT::GetServerCertificateVerify()
{
  return servercertificateverify;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKECLIENT::ClientHello_Create(XCHAR* servername, XBUFFER& clienthello, XBUFFER& records)
* @brief      Create and register a TLS 1.3 ClientHello with a fresh key share
* @ingroup    DATAIO
*
* @param[in]  servername : Optional server name for the SNI extension.
* @param[out] clienthello : Complete encoded ClientHello.
* @param[out] records : Clear-text TLS records ready for transport.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKECLIENT::ClientHello_Create(XCHAR* servername, XBUFFER& clienthello, XBUFFER& records)
{
  DIOSTREAMTLS_MSG_FRAGMENT<DIOSTREAMTLS_MSG_HANDSHAKE_CLIENTHELLO> message;
  DIOSTREAMTLS_MSG_HANDSHAKE_CLIENTHELLO*                           body;
  XRAND*                                                            xrand;
  XBYTE                                                             random[DIOSTREAMTLS_MSG_RANDOM_SIZE];
  XBYTE                                                             sessionid[DIOSTREAMTLS_MSG_SESSIONID_SIZE];
  XWORD                                                             keysharegroup;
  XBUFFER                                                           keysharepublic;
  bool                                                              status;

  if(!isini || !session || (state != DIOSTREAMTLS13HANDSHAKECLIENT_STATE_NONE) ||
     !session->GetTranscript()->IsEmpty() || (&clienthello == &records))
    {
      return SetError();
    }

  if(!authenticationconfigured || expectedservername.IsEmpty())
    {
      SetAuthenticationError(DIOSTREAMTLS13HANDSHAKECLIENT_AUTHENTICATIONERROR_CONFIGURATION);
      return SetError();
    }

  clienthello.Delete();
  records.Delete();

  xrand = GEN_XFACTORY.CreateRand();
  if(!xrand || !xrand->IsCryptographicallySecure())
    {
      if(xrand) GEN_XFACTORY.DeleteRand(xrand);
      return SetError();
    }

  status = xrand->Ini();
  if(status) status = xrand->Generate(random, sizeof(random));
  if(status) status = xrand->Generate(sessionid, sizeof(sessionid));

  GEN_XFACTORY.DeleteRand(xrand);

  if(!status)
    {
      return SetError();
    }

  if(supportedgroups.IsEmpty()) return SetError();

  keysharegroup = supportedgroups.Get(0);
  session->KeyExchange_Delete();

  if(!session->KeyExchange_Generate(keysharegroup, keysharepublic))
    {
      return SetError();
    }

  message.SetMsgType(DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_CLIENT_HELLO);

  body = message.GetBody();
  if(!body)
    {
      return SetError();
    }

  body->SetClientVersion(DIOSTREAMTLS_MSG_VERSION_TLS_1_2);
  memcpy(body->GetRandom(), random, sizeof(random));
  body->SetSessionIDLength(sizeof(sessionid));
  memcpy(body->GetSessionID(), sessionid, sizeof(sessionid));

  for(XDWORD c=0; c<ciphersuites.GetSize(); c++)
    {
      if(!body->GetCipherSuites()->Add(ciphersuites.Get(c))) return SetError();
    }

  body->SetCompressionMethod(DIOSTREAMTLS_MSG_COMPRESS_METHOD_NULL);

  if(servername && servername[0])
    {
      DIOSTREAMTLS_MSG_EXTENSION_SNI*            extension;
      DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME name;

      name.Name_SetType(0);
      name.Name_GetHost()->Set(servername);

      extension = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_SNI();
      if(!extension)
        {
          return SetError();
        }

      if(!extension->List_Add(&name) || !body->Extensions_Add(extension))
        {
          GEN_DELETE extension;
          return SetError();
        }
    }

  DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN* statusrequest = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN();
  if(!statusrequest)
    {
      return SetError();
    }
  statusrequest->SetType(DIOSTREAMTLS_MSG_EXTENSION_TYPE_STATUSREQUEST);
  XBYTE statusrequestdata[5] = { 1, 0, 0, 0, 0 };
  if(!statusrequest->GetData()->Add(statusrequestdata, sizeof(statusrequestdata)) || !body->Extensions_Add(statusrequest))
    {
      GEN_DELETE statusrequest;
      return SetError();
    }

  DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDGROUPS* supportedgroups;

  supportedgroups = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDGROUPS();
  if(!supportedgroups)
    {
      return SetError();
    }

  for(XDWORD c=0; c<this->supportedgroups.GetSize(); c++)
    {
      if(!supportedgroups->List_Add(this->supportedgroups.Get(c)))
        {
          GEN_DELETE supportedgroups;
          return SetError();
        }
    }

  if(!body->Extensions_Add(supportedgroups))
    {
      GEN_DELETE supportedgroups;
      return SetError();
    }

  DIOSTREAMTLS_MSG_EXTENSION_SIGNATUREALGORITHMS* signaturealgorithms;

  signaturealgorithms = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_SIGNATUREALGORITHMS();
  if(!signaturealgorithms)
    {
      return SetError();
    }

  for(XDWORD c=0; c<signatureschemes.GetSize(); c++)
    {
      if(!signaturealgorithms->List_Add(signatureschemes.Get(c)))
        {
          GEN_DELETE signaturealgorithms;
          return SetError();
        }
    }

  if(!body->Extensions_Add(signaturealgorithms))
    {
      GEN_DELETE signaturealgorithms;
      return SetError();
    }

  DIOSTREAMTLS_MSG_EXTENSION_SIGNATUREALGORITHMSCERT* certificatesignaturealgorithms;

  certificatesignaturealgorithms = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_SIGNATUREALGORITHMSCERT();
  if(!certificatesignaturealgorithms)
    {
      return SetError();
    }

  for(XDWORD c=0; c<certificatesignatureschemes.GetSize(); c++)
    {
      if(!certificatesignaturealgorithms->List_Add(certificatesignatureschemes.Get(c)))
        {
          GEN_DELETE certificatesignaturealgorithms;
          return SetError();
        }
    }

  if(!body->Extensions_Add(certificatesignaturealgorithms))
    {
      GEN_DELETE certificatesignaturealgorithms;
      return SetError();
    }

  DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS* supportedversions;

  supportedversions = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS();
  if(!supportedversions)
    {
      return SetError();
    }

  if(!supportedversions->List_Add(DIOSTREAMTLS_MSG_VERSION_TLS_1_3) ||
     !body->Extensions_Add(supportedversions))
    {
      GEN_DELETE supportedversions;
      return SetError();
    }

  if(!applicationprotocolsraw.IsEmpty())
    {
      DIOSTREAMTLS_MSG_EXTENSION_ALPN* ALPN;

      ALPN = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_ALPN();
      if(!ALPN)
        {
          return SetError();
        }

      for(XDWORD c=0; c<applicationprotocolsraw.GetSize(); c++)
        {
          XBUFFER* protocol = applicationprotocolsraw.Get(c);
          if(!protocol || !ALPN->List_Add((*protocol)))
            {
              GEN_DELETE ALPN;
              return SetError();
            }
        }

      if(!body->Extensions_Add(ALPN))
        {
          GEN_DELETE ALPN;
          return SetError();
        }
    }

  DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE* keyshare;
  DIOSTREAMTLS_MSG_EXTENSION_KEY       key;

  keyshare = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE();
  if(!keyshare)
    {
      return SetError();
    }

  key.SetKeyType(keysharegroup);

  if(!key.GetKeyData()->Add(keysharepublic) ||
     !keyshare->List_Add(&key) || !body->Extensions_Add(keyshare))
    {
      GEN_DELETE keyshare;
      return SetError();
    }

  resumptionoffered  = false;
  resumptionaccepted = false;
  if(!resumptionpsk.IsEmpty()) resumptionpsk.FillBuffer(0);
  resumptionpsk.Delete();
  resumptionciphersuite = 0;
  earlydataoffered = false;
  earlydataaccepted = false;
  maximumearlydatasize = 0;

  DIOSTREAMTLS13SESSIONTICKET cachedticketcopy;
  DIOSTREAMTLS13SESSIONTICKET* cachedticket = (config && config->IsSessionResumptionActive() && !expectedservername.IsEmpty() &&
                                                config->SessionTicket_Copy(expectedservername.Get(), cachedticketcopy))?&cachedticketcopy:NULL;

  if(cachedticket)
    {
      bool ciphersuiteoffered = false;
      bool alpncompatible     = cachedticket->GetApplicationProtocolRaw()->IsEmpty()?applicationprotocolsraw.IsEmpty():false;

      for(XDWORD c=0; c<ciphersuites.GetSize(); c++)
        {
          if(ciphersuites.Get(c) == cachedticket->GetCipherSuite()) { ciphersuiteoffered = true; break; }
        }

      if(!cachedticket->GetApplicationProtocolRaw()->IsEmpty())
        {
          for(XDWORD c=0; c<applicationprotocolsraw.GetSize(); c++)
            {
              XBUFFER* protocol = applicationprotocolsraw.Get(c);
              if(protocol && protocol->Compare((*cachedticket->GetApplicationProtocolRaw()))) { alpncompatible = true; break; }
            }
        }

      if(ciphersuiteoffered && alpncompatible)
        {
          DIOSTREAMTLS13KEYSCHEDULE binderkeyschedule;
          DIOSTREAMTLS_MSG_EXTENSION_PSKKEYEXCHANGEMODES* pskmodes = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_PSKKEYEXCHANGEMODES();
          DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY*         presharedkey = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY();
          DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY           identity;
          XBUFFER                                           placeholder;

          if(!pskmodes || !presharedkey ||
             !binderkeyschedule.Ini(cachedticket->GetCipherSuite(), DIOSTREAMTLSKEYSCHEDULE_ROLE_CLIENT) ||
             !binderkeyschedule.EarlySecret_Calculate(cachedticket->GetPSK()) ||
             !placeholder.Resize(binderkeyschedule.GetHashSize()))
            {
              if(pskmodes) GEN_DELETE pskmodes;
              if(presharedkey) GEN_DELETE presharedkey;
              return SetError();
            }

          memset(placeholder.Get(), 0, placeholder.GetSize());
          if(!identity.GetIdentity()->Add((*cachedticket->GetTicket())))
            {
              GEN_DELETE pskmodes;
              GEN_DELETE presharedkey;
              return SetError();
            }
          identity.SetObfuscatedTicketAge(cachedticket->GetObfuscatedAge());

          if(!pskmodes->List_Add(DIOSTREAMTLS_MSG_PSKKEYEXCHANGEMODE_PSK_DHE))
            {
              GEN_DELETE pskmodes;
              GEN_DELETE presharedkey;
              return SetError();
            }

          if(!body->Extensions_Add(pskmodes))
            {
              GEN_DELETE pskmodes;
              GEN_DELETE presharedkey;
              return SetError();
            }

          XDWORD ticketearly = cachedticket->GetMaximumEarlyDataSize();
          XDWORD configearly = config->GetMaximumEarlyDataSize();
          XDWORD allowedearly = ticketearly < configearly ? ticketearly : configearly;
          bool offerearly = config->IsEarlyDataActive() && preparedearlydatasize && allowedearly && preparedearlydatasize <= allowedearly;
          if(offerearly)
            {
              DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN* early = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN();
              if(!early) { GEN_DELETE presharedkey; return SetError(); }
              early->SetType(DIOSTREAMTLS_MSG_EXTENSION_TYPE_EARLYDATA);
              if(!body->Extensions_Add(early)) { GEN_DELETE early; GEN_DELETE presharedkey; return SetError(); }
            }

          if(!presharedkey->Identities_Add(&identity) || !presharedkey->Binders_Add(&placeholder))
            {
              GEN_DELETE presharedkey;
              return SetError();
            }

          if(!body->Extensions_Add(presharedkey))
            {
              GEN_DELETE presharedkey;
              return SetError();
            }

          clienthello.Delete();
          if(!message.SetToBuffer(clienthello, false)) return SetError();

          XDWORD removesize = sizeof(XWORD) + sizeof(XBYTE) + binderkeyschedule.GetHashSize();
          if(clienthello.GetSize() <= removesize) return SetError();

          XBUFFER truncatedclienthello;
          XBUFFER binder;
          if(!truncatedclienthello.Add(clienthello.Get(), clienthello.GetSize() - removesize) ||
             !binderkeyschedule.Binder_Calculate(truncatedclienthello, binder)) return SetError();

          XBUFFER* storedplaceholder = presharedkey->Binders_GetAll()->Get(0);
          if(!storedplaceholder) return SetError();
          storedplaceholder->Delete();
          if(!storedplaceholder->Add(binder)) return SetError();

          clienthello.Delete();
          if(!message.SetToBuffer(clienthello, false) || !resumptionpsk.Add((*cachedticket->GetPSK()))) return SetError();

          resumptionoffered     = true;
          resumptionciphersuite = cachedticket->GetCipherSuite();
          if(offerearly)
            {
              maximumearlydatasize = allowedearly;
              if(maximumearlydatasize > config->GetMaximumEarlyDataSize()) maximumearlydatasize = config->GetMaximumEarlyDataSize();
              if(!session->CipherSuite_Select(cachedticket->GetCipherSuite()) ||
                 !session->EarlyKeys_Activate(resumptionpsk, clienthello, DIOSTREAMTLSKEYSCHEDULE_DIRECTION_LOCAL)) return SetError();
              earlydataoffered = true;
            }
        }
    }

  if((clienthello.IsEmpty() && !message.SetToBuffer(clienthello, false)) ||
     !session->GetRecord()->Protect(DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE, clienthello, records) ||
     !Start(clienthello))
    {
      clienthello.Delete();
      records.Delete();
      return SetError();
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKECLIENT::Start(XBUFFER& clienthello)
* @brief      Register the exact ClientHello sent by this end and start the transcript
* @ingroup    DATAIO
*
* @param[in]  clienthello : Complete encoded ClientHello.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKECLIENT::Start(XBUFFER& clienthello)
{
  DIOSTREAMTLS_MSG_FRAGMENT<DIOSTREAMTLS_MSG_HANDSHAKE_CLIENTHELLO> message;
  XBUFFER                                                          workbuffer;
  bool                                                             ciphersuitefound = false;
  bool                                                             supportedgroupsfound = false;
  bool                                                             keysharefound = false;

  if(!isini || !session || (state != DIOSTREAMTLS13HANDSHAKECLIENT_STATE_NONE) ||
     !session->GetTranscript()->IsEmpty())
    {
      return SetError();
    }

  workbuffer.Add(clienthello);

  if(!message.GetFromBuffer(workbuffer, false) || !workbuffer.IsEmpty() ||
     (message.GetMsgType() != DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_CLIENT_HELLO))
    {
      return SetError();
    }

  for(XDWORD c=0; c<message.GetBody()->GetCipherSuites()->GetSize(); c++)
    {
      XWORD ciphersuite = message.GetBody()->GetCipherSuites()->Get(c);

      if(!offeredciphersuites.Add(ciphersuite)) return SetError();

      if(ciphersuite == session->GetKeySchedule()->GetCipherSuite())
        {
          ciphersuitefound = true;
        }
    }

  for(XDWORD c=0; c<message.GetBody()->Extensions_GetAll()->GetSize(); c++)
    {
      DIOSTREAMTLS_MSG_EXTENSION* extension = message.GetBody()->Extensions_GetAll()->Get(c);
      if(!extension) return SetError();

      if(extension->GetType() == DIOSTREAMTLS_MSG_EXTENSION_TYPE_SIGNATUREALGORITHMS)
        {
          DIOSTREAMTLS_MSG_EXTENSION_SIGNATUREALGORITHMS* algorithms;

          algorithms = (DIOSTREAMTLS_MSG_EXTENSION_SIGNATUREALGORITHMS*)extension;
          for(XDWORD d=0; d<algorithms->List_Get()->GetSize(); d++)
            {
              if(!offeredsignatureschemes.Add(algorithms->List_Get()->Get(d))) return SetError();
            }
        }

      if(extension->GetType() == DIOSTREAMTLS_MSG_EXTENSION_TYPE_SUPPORTEDGROUPS)
        {
          DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDGROUPS* groups;

          groups = (DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDGROUPS*)extension;
          if(groups->List_Get()->IsEmpty()) return SetError();

          for(XDWORD d=0; d<groups->List_Get()->GetSize(); d++)
            {
              XWORD group = groups->List_Get()->Get(d);

              if(SupportedGroup_IsOffered(group) || !offeredsupportedgroups.Add(group)) return SetError();
            }

          supportedgroupsfound = true;
        }

      if(extension->GetType() == DIOSTREAMTLS_MSG_EXTENSION_TYPE_KEYSHARE)
        {
          DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE* keyshare;

          keyshare = (DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE*)extension;
          if(keyshare->List_Get()->IsEmpty()) return SetError();

          for(XDWORD d=0; d<keyshare->List_Get()->GetSize(); d++)
            {
              DIOSTREAMTLS_MSG_EXTENSION_KEY* key = keyshare->List_Get()->Get(d);

              if(!key || !SupportedGroup_IsOffered(key->GetKeyType()) ||
                 KeyShare_IsOffered(key->GetKeyType()) || !offeredkeysharegroups.Add(key->GetKeyType())) return SetError();
            }

          keysharefound = true;
        }

      if(extension->GetType() == DIOSTREAMTLS_MSG_EXTENSION_TYPE_EARLYDATA)
        {
          DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN* early=(DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN*)extension;
          if(!resumptionaccepted || !earlydataoffered || !early->GetData()->IsEmpty() || earlydataaccepted) return SetError();
          earlydataaccepted=true;
        }

      if(extension->GetType() == DIOSTREAMTLS_MSG_EXTENSION_TYPE_ALPN)
        {
          DIOSTREAMTLS_MSG_EXTENSION_ALPN* ALPN = (DIOSTREAMTLS_MSG_EXTENSION_ALPN*)extension;

          for(XDWORD d=0; d<ALPN->List_GetNProtocols(); d++)
            {
              DIOSTREAMTLS_ALPN_TYPE applicationprotocol;
              XBUFFER* raw = GEN_NEW XBUFFER();

              if(!raw || !ALPN->List_Get(d, (*raw)) || !offeredapplicationprotocolsraw.Add(raw))
                { if(raw) GEN_DELETE raw; return SetError(); }
              if(!ALPN->List_Get(d, applicationprotocol)) return SetError();
              if(applicationprotocol != DIOSTREAMTLS_ALPN_TYPE_UNKNOWN) offeredapplicationprotocols.Add(applicationprotocol);
            }
        }
    }

  if(!ciphersuitefound || !supportedgroupsfound || !keysharefound ||
     offeredsignatureschemes.IsEmpty() || !session->Transcript_Add(clienthello) ||
     !firstclienthello.Add(clienthello))
    {
      return SetError();
    }

  legacysessionid.Delete();

  if(message.GetBody()->GetSessionIDLength() &&
     !legacysessionid.Add(message.GetBody()->GetSessionID(), message.GetBody()->GetSessionIDLength()))
    {
      return SetError();
    }

  state = DIOSTREAMTLS13HANDSHAKECLIENT_STATE_WAIT_SERVERHELLO;
  currentkeysharegroup = offeredkeysharegroups.Get(0);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKECLIENT::HelloRetryRequest_Process(XBUFFER& helloretryrequest, XBUFFER& clienthello, XBUFFER& records)
* @brief      Validate a TLS 1.3 HelloRetryRequest and create the second ClientHello
* @ingroup    DATAIO
*
* @param[in]  helloretryrequest : Complete encoded HelloRetryRequest.
* @param[out] clienthello : Complete encoded second ClientHello.
* @param[out] records : Clear-text TLS records ready for transport.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKECLIENT::HelloRetryRequest_Process(XBUFFER& helloretryrequest, XBUFFER& clienthello, XBUFFER& records)
{
  DIOSTREAMTLS_MSG_FRAGMENT<DIOSTREAMTLS_MSG_HANDSHAKE_SERVERHELLO> hrr;
  DIOSTREAMTLS_MSG_FRAGMENT<DIOSTREAMTLS_MSG_HANDSHAKE_CLIENTHELLO> clienthellomessage;
  DIOSTREAMTLS_MSG_HANDSHAKE                                       messagehash;
  XBUFFER                                                          workbuffer;
  XBUFFER                                                          firsthash;
  XBUFFER                                                          messagehashbuffer;
  XBUFFER                                                          keysharepublic;
  XBUFFER                                                          cookiedata;
  XWORD                                                             selectedgroup = currentkeysharegroup;
  bool                                                              versionvalid = false;
  bool                                                              keysharerequested = false;
  bool                                                              cookiepresent = false;

  if(!isini || !session || helloretryrequestprocessed ||
     (state != DIOSTREAMTLS13HANDSHAKECLIENT_STATE_WAIT_SERVERHELLO) ||
     firstclienthello.IsEmpty() || (&clienthello == &records))
    {
      return SetError();
    }

  clienthello.Delete();
  records.Delete();
  workbuffer.Add(helloretryrequest);

  if(!hrr.GetFromBuffer(workbuffer, false) || !workbuffer.IsEmpty() ||
     (hrr.GetMsgType() != DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_SERVER_HELLO) ||
     !hrr.GetBody()->IsHelloRetryRequest() ||
     (hrr.GetBody()->GetLegacyVersion() != DIOSTREAMTLS_MSG_VERSION_TLS_1_2) ||
     (hrr.GetBody()->GetSessionIDLength() != legacysessionid.GetSize()) ||
     (hrr.GetBody()->GetSessionIDLength() &&
      memcmp(hrr.GetBody()->GetSessionID(), legacysessionid.Get(), legacysessionid.GetSize())) ||
     (hrr.GetBody()->GetCompressionMethod() != DIOSTREAMTLS_MSG_COMPRESS_METHOD_NULL) ||
     !CipherSuite_IsOffered(hrr.GetBody()->GetCipherSuite()))
    {
      return SetError();
    }

  for(XDWORD c=0; c<hrr.GetBody()->Extensions_GetAll()->GetSize(); c++)
    {
      DIOSTREAMTLS_MSG_EXTENSION* extension = hrr.GetBody()->Extensions_GetAll()->Get(c);
      if(!extension) return SetError();

      switch(extension->GetType())
        {
          case DIOSTREAMTLS_MSG_EXTENSION_TYPE_SUPPORTEDVERSIONS : { DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS_SERVER* supportedversions;

                                                                    supportedversions = (DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS_SERVER*)extension;
                                                                    versionvalid = (supportedversions->GetVersion() == DIOSTREAMTLS_MSG_VERSION_TLS_1_3);
                                                                  }
                                                                  break;

          case DIOSTREAMTLS_MSG_EXTENSION_TYPE_KEYSHARE          : { DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_HELLORETRYREQUEST* keyshare;

                                                                    keyshare = (DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_HELLORETRYREQUEST*)extension;
                                                                    selectedgroup = keyshare->GetSelectedGroup();

                                                                    if(!SupportedGroup_IsOffered(selectedgroup) || KeyShare_IsOffered(selectedgroup))
                                                                      {
                                                                        return SetError();
                                                                      }

                                                                    keysharerequested = true;
                                                                  }
                                                                  break;

          case DIOSTREAMTLS_MSG_EXTENSION_TYPE_COOKIE            : { DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN* cookie;
                                                                    XWORD                            cookielength;

                                                                    cookie = (DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN*)extension;
                                                                    if(!cookie->GetData() || (cookie->GetData()->GetSize() < 3)) return SetError();

                                                                    cookielength = (XWORD)(((XWORD)cookie->GetData()->GetByte(0) << 8) |
                                                                                            cookie->GetData()->GetByte(1));

                                                                    if(!cookielength ||
                                                                       (cookielength != (cookie->GetData()->GetSize() - sizeof(XWORD))) ||
                                                                       !cookiedata.Add((*cookie->GetData())))
                                                                      {
                                                                        return SetError();
                                                                      }

                                                                    cookiepresent = true;
                                                                  }
                                                                  break;

                                                        default : return SetError();
        }
    }

  if(!versionvalid || (!keysharerequested && !cookiepresent) ||
     !session->CipherSuite_Select(hrr.GetBody()->GetCipherSuite()) ||
     !session->TranscriptHash(firsthash))
    {
      return SetError();
    }

  workbuffer.Delete();
  workbuffer.Add(firstclienthello);

  if(!clienthellomessage.GetFromBuffer(workbuffer, false) || !workbuffer.IsEmpty() ||
     (clienthellomessage.GetMsgType() != DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_CLIENT_HELLO))
    {
      return SetError();
    }

  if(keysharerequested)
    {
      DIOSTREAMTLS_MSG_EXTENSION_KEY key;
      bool                           keysharefound = false;

      session->KeyExchange_Delete();
      if(!session->KeyExchange_Generate(selectedgroup, keysharepublic)) return SetError();

      key.SetKeyType(selectedgroup);
      if(!key.GetKeyData()->Add(keysharepublic)) return SetError();

      for(XDWORD c=0; c<clienthellomessage.GetBody()->Extensions_GetAll()->GetSize(); c++)
        {
          DIOSTREAMTLS_MSG_EXTENSION* extension = clienthellomessage.GetBody()->Extensions_GetAll()->Get(c);

          if(extension && (extension->GetType() == DIOSTREAMTLS_MSG_EXTENSION_TYPE_KEYSHARE))
            {
              DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE* keyshare = (DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE*)extension;

              if(!keyshare->List_DeleteAll() || !keyshare->List_Add(&key)) return SetError();

              keysharefound = true;
              break;
            }
        }

      if(!keysharefound) return SetError();
    }

  if(earlydataoffered)
    {
      for(int c=(int)clienthellomessage.GetBody()->Extensions_GetAll()->GetSize()-1;c>=0;c--)
        {
          DIOSTREAMTLS_MSG_EXTENSION* extension=clienthellomessage.GetBody()->Extensions_GetAll()->Get(c);
          if(extension && extension->GetType()==DIOSTREAMTLS_MSG_EXTENSION_TYPE_EARLYDATA)
            {
              clienthellomessage.GetBody()->Extensions_GetAll()->Delete(extension);
              GEN_DELETE extension;
            }
        }
      earlydataaccepted=false;
      session->EarlyKeys_Deactivate(DIOSTREAMTLSKEYSCHEDULE_DIRECTION_LOCAL);
    }

  if(cookiepresent)
    {
      DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN* cookie = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN();

      if(!cookie) return SetError();

      cookie->SetType(DIOSTREAMTLS_MSG_EXTENSION_TYPE_COOKIE);

      if(!cookie->GetData()->Add(cookiedata) || !clienthellomessage.GetBody()->Extensions_Add(cookie))
        {
          GEN_DELETE cookie;
          return SetError();
        }
    }

  messagehash.SetMsgType(DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_MESSAGE_HASH);

  if(!messagehash.GetBody()->Add(firsthash) ||
     !messagehash.SetToBuffer(messagehashbuffer, false))
    {
      clienthello.Delete();
      records.Delete();
      return SetError();
    }

  // A ClientHello2 that keeps a resumption PSK must recompute its obfuscated ticket age and binder over
  // message_hash(ClientHello1) + HelloRetryRequest + Truncate(ClientHello2). GEN's own server deliberately falls
  // back to a full certificate handshake after HRR, but doing this here keeps the client interoperable with servers
  // that legitimately select the PSK on the second ClientHello.
  if(resumptionoffered)
    {
      DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY* presharedkey = NULL;

      for(XDWORD c=0; c<clienthellomessage.GetBody()->Extensions_GetAll()->GetSize(); c++)
        {
          DIOSTREAMTLS_MSG_EXTENSION* extension = clienthellomessage.GetBody()->Extensions_GetAll()->Get(c);
          if(extension && (extension->GetType() == DIOSTREAMTLS_MSG_EXTENSION_TYPE_PRESHAREDKEY))
            {
              if(c != (clienthellomessage.GetBody()->Extensions_GetAll()->GetSize()-1)) return SetError();
              presharedkey = (DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY*)extension;
              break;
            }
        }

      if(!presharedkey || (presharedkey->Identities_GetAll()->GetSize() != 1) ||
         (presharedkey->Binders_GetAll()->GetSize() != 1) || resumptionpsk.IsEmpty()) return SetError();

      DIOSTREAMTLS13SESSIONTICKET cachedticketcopy;
      DIOSTREAMTLS13SESSIONTICKET* cachedticket = (config && !expectedservername.IsEmpty() &&
                                                    config->SessionTicket_Copy(expectedservername.Get(), cachedticketcopy))?&cachedticketcopy:NULL;
      DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY* identity = presharedkey->Identities_GetAll()->Get(0);
      XBUFFER*                                storedbinder = presharedkey->Binders_GetAll()->Get(0);
      DIOSTREAMTLS13KEYSCHEDULE               binderkeyschedule;
      XBUFFER                                 placeholder;
      XBUFFER                                 encodedclienthello;
      XBUFFER                                 truncatedclienthello;
      XBUFFER                                 bindertranscript;
      XBUFFER                                 binder;

      if(!identity || !storedbinder ||
         !binderkeyschedule.Ini(resumptionciphersuite, DIOSTREAMTLSKEYSCHEDULE_ROLE_CLIENT) ||
         !binderkeyschedule.EarlySecret_Calculate(&resumptionpsk) ||
         !placeholder.Resize(binderkeyschedule.GetHashSize())) return SetError();

      if(cachedticket && !cachedticket->GetTicket()->Compare((*identity->GetIdentity())))
        {
          identity->SetObfuscatedTicketAge(cachedticket->GetObfuscatedAge());
        }

      memset(placeholder.Get(), 0, placeholder.GetSize());
      storedbinder->Delete();
      if(!storedbinder->Add(placeholder) || !clienthellomessage.SetToBuffer(encodedclienthello, false)) return SetError();

      XDWORD removesize = sizeof(XWORD) + presharedkey->Binders_GetLength();
      if(encodedclienthello.GetSize() <= removesize ||
         !truncatedclienthello.Add(encodedclienthello.Get(), encodedclienthello.GetSize()-removesize) ||
         !bindertranscript.Add(messagehashbuffer) || !bindertranscript.Add(helloretryrequest) ||
         !bindertranscript.Add(truncatedclienthello) ||
         !binderkeyschedule.Binder_Calculate(bindertranscript, binder)) return SetError();

      storedbinder->Delete();
      if(!storedbinder->Add(binder)) return SetError();
    }

  if(!clienthellomessage.SetToBuffer(clienthello, false) ||
     !session->GetRecord()->Protect(DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE, clienthello, records))
    {
      clienthello.Delete();
      records.Delete();
      return SetError();
    }

  session->GetTranscript()->Delete();

  if(!session->Transcript_Add(messagehashbuffer) ||
     !session->Transcript_Add(helloretryrequest) ||
     !session->Transcript_Add(clienthello))
    {
      clienthello.Delete();
      records.Delete();
      return SetError();
    }

  offeredkeysharegroups.DeleteAll();
  if(!offeredkeysharegroups.Add(selectedgroup)) return SetError();

  currentkeysharegroup       = selectedgroup;
  helloretryrequestprocessed = true;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKECLIENT::ServerHello_Process(XBUFFER& serverhello)
* @brief      Calculate the negotiated shared secret from ServerHello and activate handshake keys
* @ingroup    DATAIO
*
* @param[in]  serverhello : Complete encoded ServerHello.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKECLIENT::ServerHello_Process(XBUFFER& serverhello)
{
  DIOSTREAMTLS_MSG_FRAGMENT<DIOSTREAMTLS_MSG_HANDSHAKE_SERVERHELLO> message;
  XBUFFER                                                          workbuffer;
  XSECUREBUFFER                                                    sharedsecret;
  XBUFFER*                                                         peerpublickey = NULL;

  if(!isini || !session || (state != DIOSTREAMTLS13HANDSHAKECLIENT_STATE_WAIT_SERVERHELLO))
    {
      return SetError();
    }

  workbuffer.Add(serverhello);

  if(!message.GetFromBuffer(workbuffer, false) || !workbuffer.IsEmpty() ||
     (message.GetMsgType() != DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_SERVER_HELLO) ||
     message.GetBody()->IsHelloRetryRequest())
    {
      return SetError();
    }

  for(XDWORD c=0; c<message.GetBody()->Extensions_GetAll()->GetSize(); c++)
    {
      DIOSTREAMTLS_MSG_EXTENSION* extension = message.GetBody()->Extensions_GetAll()->Get(c);
      if(!extension)
        {
          return SetError();
        }

      if(extension->GetType() == DIOSTREAMTLS_MSG_EXTENSION_TYPE_KEYSHARE)
        {
          DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_SERVER* keyshare;
          DIOSTREAMTLS_MSG_EXTENSION_KEY*             key;

          keyshare = (DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_SERVER*)extension;
          key      = keyshare->GetKey();

          if(key && (key->GetKeyType() == currentkeysharegroup))
            {
              peerpublickey = key->GetKeyData();
            }
        }
    }

  if(!peerpublickey || !session->KeyExchange_SharedSecret(currentkeysharegroup, (*peerpublickey), sharedsecret))
    {
      return SetError();
    }

  bool status = ServerHello_Process(serverhello, sharedsecret);

  sharedsecret.FillBuffer(0);
  sharedsecret.Delete();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKECLIENT::ServerHello_Process(XBUFFER& serverhello, XBUFFER& sharedsecret)
* @brief      Validate ServerHello and activate the handshake traffic keys
* @note       This overload keeps deterministic protocol vectors independent of random private keys.
* @ingroup    DATAIO
*
* @param[in]  serverhello : Complete encoded ServerHello.
* @param[in]  sharedsecret : Explicit negotiated shared secret used by a deterministic test vector.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKECLIENT::ServerHello_Process(XBUFFER& serverhello, XBUFFER& sharedsecret)
{
  DIOSTREAMTLS_MSG_FRAGMENT<DIOSTREAMTLS_MSG_HANDSHAKE_SERVERHELLO> message;
  XBUFFER                                                          workbuffer;
  bool                                                             versionvalid  = false;
  bool                                                             keysharevalid = false;
  bool                                                             pskselected   = false;

  if(!isini || !session ||
     (state != DIOSTREAMTLS13HANDSHAKECLIENT_STATE_WAIT_SERVERHELLO))
    {
      return SetError();
    }

  workbuffer.Add(serverhello);

  if(!message.GetFromBuffer(workbuffer, false) || !workbuffer.IsEmpty() ||
     (message.GetMsgType() != DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_SERVER_HELLO) ||
     message.GetBody()->IsHelloRetryRequest() ||
     (message.GetBody()->GetLegacyVersion() != DIOSTREAMTLS_MSG_VERSION_TLS_1_2) ||
     (message.GetBody()->GetSessionIDLength() != legacysessionid.GetSize()) ||
     (message.GetBody()->GetSessionIDLength() &&
      memcmp(message.GetBody()->GetSessionID(), legacysessionid.Get(), legacysessionid.GetSize())) ||
     (message.GetBody()->GetCompressionMethod() != DIOSTREAMTLS_MSG_COMPRESS_METHOD_NULL))
    {
      return SetError();
    }

  for(XDWORD c=0; c<message.GetBody()->Extensions_GetAll()->GetSize(); c++)
    {
      DIOSTREAMTLS_MSG_EXTENSION* extension = message.GetBody()->Extensions_GetAll()->Get(c);
      if(!extension) return SetError();

      switch(extension->GetType())
        {
          case DIOSTREAMTLS_MSG_EXTENSION_TYPE_SUPPORTEDVERSIONS : { DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS_SERVER* supportedversions;

                                                                    supportedversions = (DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS_SERVER*)extension;
                                                                    versionvalid = (supportedversions->GetVersion() == DIOSTREAMTLS_MSG_VERSION_TLS_1_3);
                                                                  }
                                                                  break;

          case DIOSTREAMTLS_MSG_EXTENSION_TYPE_KEYSHARE          : { DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_SERVER* keyshare;
                                                                    DIOSTREAMTLS_MSG_EXTENSION_KEY*             key;

                                                                    keyshare = (DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_SERVER*)extension;
                                                                    key      = keyshare->GetKey();

                                                                    if(key && (key->GetKeyType() == currentkeysharegroup) &&
                                                                       (
                                                                        #ifdef CIPHER_ASYMMETRIC_MLKEM768_ACTIVE
                                                                        ((currentkeysharegroup == DIOSTREAMTLS_MSG_CURVEID_SECP256R1MLKEM768) &&
                                                                         (key->GetKeyData()->GetSize() == CIPHERSECP256R1MLKEM768_SERVERSHARESIZE)) ||
                                                                        #endif
                                                                        #ifdef CIPHER_ASYMMETRIC_MLKEM1024_ACTIVE
                                                                        ((currentkeysharegroup == DIOSTREAMTLS_MSG_CURVEID_SECP384R1MLKEM1024) &&
                                                                         (key->GetKeyData()->GetSize() == CIPHERSECP384R1MLKEM1024_SERVERSHARESIZE)) ||
                                                                        #endif
                                                                        #if defined(CIPHER_ASYMMETRIC_X25519_ACTIVE) && defined(CIPHER_ASYMMETRIC_MLKEM768_ACTIVE)
                                                                        ((currentkeysharegroup == DIOSTREAMTLS_MSG_CURVEID_X25519MLKEM768) &&
                                                                         (key->GetKeyData()->GetSize() == CIPHERX25519MLKEM768_SERVERSHARESIZE)) ||
                                                                        #endif
                                                                        ((currentkeysharegroup == DIOSTREAMTLS_MSG_CURVEID_X25519) &&
                                                                         (key->GetKeyData()->GetSize() == CIPHERECDSAX25519_MAXKEY)) ||
                                                                        ((currentkeysharegroup == DIOSTREAMTLS_MSG_CURVEID_SECP256R1) &&
                                                                         (key->GetKeyData()->GetSize() == CIPHERECDSA_P256_PUBLICKEY_SIZE)) ||
                                                                        ((currentkeysharegroup == DIOSTREAMTLS_MSG_CURVEID_SECP384R1) &&
                                                                         (key->GetKeyData()->GetSize() == CIPHERECDSA_P384_PUBLICKEY_SIZE)) ||
                                                                        ((currentkeysharegroup == DIOSTREAMTLS_MSG_CURVEID_SECP521R1) &&
                                                                         (key->GetKeyData()->GetSize() == CIPHERECDSA_P521_PUBLICKEY_SIZE))))
                                                                      {
                                                                        keysharevalid = true;
                                                                      }
                                                                  }
                                                                  break;

          case DIOSTREAMTLS_MSG_EXTENSION_TYPE_PRESHAREDKEY      : { DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY_SERVER* selectedpsk;

                                                                    selectedpsk = (DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY_SERVER*)extension;
                                                                    if(!resumptionoffered ||
                                                                       (selectedpsk->GetSelectedIdentity() != 0) ||
                                                                       (message.GetBody()->GetCipherSuite() != resumptionciphersuite))
                                                                      {
                                                                        return SetError();
                                                                      }

                                                                    pskselected = true;
                                                                  }
                                                                  break;

                                                        default : return SetError();
        }
    }

  if(!versionvalid || !keysharevalid ||
     (helloretryrequestprocessed &&
      (message.GetBody()->GetCipherSuite() != session->GetKeySchedule()->GetCipherSuite())) ||
     !CipherSuite_IsOffered(message.GetBody()->GetCipherSuite()) ||
     !session->CipherSuite_Select(message.GetBody()->GetCipherSuite()) ||
     !session->Transcript_Add(serverhello) ||
     !session->HandshakeKeys_Activate(sharedsecret, pskselected?&resumptionpsk:NULL))
    {
      return SetError();
    }

  resumptionaccepted = pskselected;
  if(pskselected)
    {
      serverauthenticated = true;
      authenticationerror = DIOSTREAMTLS13HANDSHAKECLIENT_AUTHENTICATIONERROR_NONE;
    }

  state = DIOSTREAMTLS13HANDSHAKECLIENT_STATE_WAIT_ENCRYPTEDEXTENSIONS;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKECLIENT::ClientFinished_Create(XBUFFER& clientfinished, XBUFFER& records)
* @brief      Create the optional client-authentication flight and the client Finished
* @ingroup    DATAIO
*
* @param[out] clientfinished : Complete encoded client Finished.
* @param[out] records : Encrypted TLS records ready for transport.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKECLIENT::ClientFinished_Create(XBUFFER& clientfinished, XBUFFER& records)
{
  DIOSTREAMTLS_MSG_FRAGMENT<DIOSTREAMTLS_MSG_HANDSHAKE_FINISHED> finished;
  XBUFFER                                                       transcripthash;
  XBUFFER                                                       verifydata;

  if(!isini || !session || !config || (state != DIOSTREAMTLS13HANDSHAKECLIENT_STATE_SERVERFINISHED_VERIFIED) ||
     (&clientfinished == &records))
    {
      return SetError();
    }

  clientfinished.Delete();
  records.Delete();

  if(earlydataaccepted)
    {
      DIOSTREAMTLS_MSG_HANDSHAKE endofearly;
      XBUFFER endbuffer;
      XBUFFER endrecords;
      endofearly.SetMsgType(DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_END_OF_EARLY_DATA);
      endofearly.SetLength(0);
      if(!endofearly.SetToBuffer(endbuffer, false) || !session->GetRecord()->Protect(DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE, endbuffer, endrecords) ||
         !session->Transcript_Add(endbuffer) || !records.Add(endrecords)) return SetError();
      session->EarlyData_End(DIOSTREAMTLSKEYSCHEDULE_DIRECTION_LOCAL);
    }

  if(certificaterequested)
    {
      DIOSTREAMTLS_MSG_FRAGMENT<DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATE> certificatemessage;
      XBUFFER                                                          certificatebuffer;
      XBUFFER                                                          flightrecords;
      XVECTOR<XBUFFER*>*                                               localcertificatechain = config->GetLocalCertificateChain();
      CIPHERKEY*                                                       localprivatekey       = config->GetLocalPrivateKey();
      bool                                                             havecredentials       = localcertificatechain && !localcertificatechain->IsEmpty() && localprivatekey;
      CIPHERCERTIFICATEX509                                            clientleafcertificate;
      CIPHERCERTIFICATEX509                                            clientissuercertificate;
      XWORD                                                            clientsignaturescheme = 0;
      XBUFFER*                                                         localOCSPresponse      = config->GetLocalOCSPStapledResponse();
      bool                                                             sendOCSPresponse       = false;

      if(havecredentials)
        {
          if(clientleafcertificate.Decode((*localcertificatechain->Get(0))) && clientleafcertificate.GetPublicCipherKey())
            {
              for(XDWORD c=0; c<requestedclientsignatureschemes.GetSize(); c++)
                {
                  XWORD candidate = requestedclientsignatureschemes.Get(c);
                  if(DIOSTREAMTLSSIGNATURE::IsSupported(candidate, &clientleafcertificate))
                    {
                      clientsignaturescheme = candidate;
                      break;
                    }
                }
            }

          if(!clientsignaturescheme) havecredentials = false;
          if(havecredentials && !DIOSTREAMTLS13_HANDSHAKECLIENT_CertificateChainCompatible(localcertificatechain, requestedclientcertificatesignatureschemes)) havecredentials=false;
          if(havecredentials && !DIOSTREAMTLS13_HANDSHAKECLIENT_CertificateAuthorityCompatible(localcertificatechain, requestedclientcertificateauthorities)) havecredentials=false;

          if(havecredentials && clientOCSPstaplingrequested && localOCSPresponse && !localOCSPresponse->IsEmpty() &&
             (localcertificatechain->GetSize() >= 2) && localcertificatechain->Get(1) &&
             clientissuercertificate.Decode((*localcertificatechain->Get(1))))
            {
              CIPHERCERTIFICATEX509REVOCATION_RESULT OCSPresult = CIPHERCERTIFICATEX509REVOCATION::ValidateOCSP((*localOCSPresponse), clientleafcertificate, clientissuercertificate);

              if(OCSPresult == CIPHERCERTIFICATEX509REVOCATION_RESULT_REVOKED) return SetError();
              sendOCSPresponse = (OCSPresult == CIPHERCERTIFICATEX509REVOCATION_RESULT_GOOD);
            }
        }

      certificatemessage.SetMsgType(DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_CERTIFICATE);
      if(!certificatemessage.GetBody()->SetRequestContext(NULL, 0)) return SetError();

      if(havecredentials)
        {
          for(XDWORD c=0; c<localcertificatechain->GetSize(); c++)
            {
              XBUFFER* dercertificate = localcertificatechain->Get(c);
              DIOSTREAMTLS_MSG_CERTIFICATEENTRY* entry;

              if(!dercertificate) return SetError();
              entry = GEN_NEW DIOSTREAMTLS_MSG_CERTIFICATEENTRY();
              if(!entry) return SetError();

              if(!entry->GetCertificateData()->Add((*dercertificate)))
                {
                  GEN_DELETE entry;
                  return SetError();
                }

              if(!c && sendOCSPresponse)
                {
                  DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN* statusrequest = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN();
                  XDWORD                               responsesize  = localOCSPresponse->GetSize();

                  if(!statusrequest)
                    {
                      GEN_DELETE entry;
                      return SetError();
                    }

                  statusrequest->SetType(DIOSTREAMTLS_MSG_EXTENSION_TYPE_STATUSREQUEST);
                  if(!statusrequest->GetData()->Add((XBYTE)1) ||
                     !statusrequest->GetData()->Add((XBYTE)((responsesize >> 16) & 0xFF)) ||
                     !statusrequest->GetData()->Add((XBYTE)((responsesize >> 8) & 0xFF)) ||
                     !statusrequest->GetData()->Add((XBYTE)(responsesize & 0xFF)) ||
                     !statusrequest->GetData()->Add((*localOCSPresponse)) || !entry->Extensions_Add(statusrequest))
                    {
                      GEN_DELETE statusrequest;
                      GEN_DELETE entry;
                      return SetError();
                    }
                }

              if(!certificatemessage.GetBody()->CertificateList_Add(entry))
                {
                  GEN_DELETE entry;
                  return SetError();
                }
            }
        }

      if(!certificatemessage.SetToBuffer(certificatebuffer, false) ||
         !session->GetRecord()->Protect(DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE, certificatebuffer, flightrecords) ||
         !session->Transcript_Add(certificatebuffer) || !records.Add(flightrecords))
        {
          records.Delete();
          return SetError();
        }

      if(havecredentials)
        {
          static XBYTE context[] = { 'T', 'L', 'S', ' ', '1', '.', '3', ',', ' ',
                                     'c', 'l', 'i', 'e', 'n', 't', ' ',
                                     'C', 'e', 'r', 't', 'i', 'f', 'i', 'c', 'a', 't', 'e', 'V', 'e', 'r', 'i', 'f', 'y' };

          DIOSTREAMTLS_MSG_FRAGMENT<DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEVERIFY> certificateverify;
          XBUFFER                                                                certificateverifybuffer;
          XBUFFER                                                                certificateverifyhash;
          XBUFFER                                                                signedcontent;
          XBUFFER                                                                signature;

          if(!session->TranscriptHash(certificateverifyhash)) return SetError();

          for(int c=0; c<64; c++) if(!signedcontent.Add((XBYTE)0x20)) return SetError();

          if(!signedcontent.Add(context, sizeof(context)) || !signedcontent.Add((XBYTE)0x00) ||
             !signedcontent.Add(certificateverifyhash) ||
             !DIOSTREAMTLSSIGNATURE::Sign(clientsignaturescheme, localprivatekey, clientleafcertificate.GetPublicCipherKey(), signedcontent, signature))
            {
              records.Delete();
              return SetError();
            }

          certificateverify.SetMsgType(DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_CERTIFICATE_VERIFY);
          certificateverify.GetBody()->SetAlgorithm(clientsignaturescheme);
          flightrecords.Delete();

          if(!certificateverify.GetBody()->GetSignature()->Add(signature) ||
             !certificateverify.SetToBuffer(certificateverifybuffer, false) ||
             !session->GetRecord()->Protect(DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE, certificateverifybuffer, flightrecords) ||
             !session->Transcript_Add(certificateverifybuffer) || !records.Add(flightrecords))
            {
              records.Delete();
              return SetError();
            }
        }
    }

  if(!session->TranscriptHash(transcripthash) ||
     !session->GetKeySchedule()->CalculateFinished(DIOSTREAMTLSKEYSCHEDULE_DIRECTION_LOCAL,
                                                    transcripthash, verifydata))
    {
      return SetError();
    }

  finished.SetMsgType(DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_FINISHED);

  XBUFFER finishedrecords;
  if(!finished.GetBody()->GetVerifyData()->Add(verifydata) ||
     !finished.SetToBuffer(clientfinished, false) ||
     !session->GetRecord()->Protect(DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE, clientfinished, finishedrecords) ||
     !session->Transcript_Add(clientfinished) || !records.Add(finishedrecords) ||
     !session->ApplicationKeys_Activate(DIOSTREAMTLSKEYSCHEDULE_DIRECTION_LOCAL))
    {
      clientfinished.Delete();
      records.Delete();
      return SetError();
    }

  XBUFFER resumptiontranscripthash;
  if(!session->TranscriptHash(resumptiontranscripthash) ||
     !session->GetKeySchedule()->ResumptionSecret_Calculate(resumptiontranscripthash))
    {
      return SetError();
    }

  state = DIOSTREAMTLS13HANDSHAKECLIENT_STATE_HANDSHAKE_COMPLETED;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKECLIENT::NewSessionTicket_Process(XBUFFER& message)
* @brief      Process and cache a TLS 1.3 NewSessionTicket for a future PSK-DHE resumption
* @ingroup    DATAIO
*
* @param[in]  message : Complete NewSessionTicket handshake message.
*
* @return     bool : true if the ticket was accepted or deliberately ignored; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKECLIENT::NewSessionTicket_Process(XBUFFER& message)
{
  if(!isini || !session || !config || !config->IsSessionResumptionActive() ||
     (state != DIOSTREAMTLS13HANDSHAKECLIENT_STATE_HANDSHAKE_COMPLETED)) return true;

  DIOSTREAMTLS_MSG_FRAGMENT<DIOSTREAMTLS_MSG_HANDSHAKE_NEWSESSIONTICKET> ticketmessage;
  XBUFFER workbuffer;
  XSECUREBUFFER PSK;

  workbuffer.Add(message);
  if(!ticketmessage.GetFromBuffer(workbuffer, false) || !workbuffer.IsEmpty() ||
     (ticketmessage.GetMsgType() != DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_NEW_SESSION_TICKET)) return false;

  if(!ticketmessage.GetBody()->GetTicketLifetime()) return true;
  if(!session->GetKeySchedule()->ResumptionPSK_Calculate((*ticketmessage.GetBody()->GetTicketNonce()), PSK)) return false;

  XBUFFER* protocol = applicationprotocolnegotiated?&applicationprotocolraw:NULL;
  bool status = config->SessionTicket_StoreRaw(expectedservername.Get(), (*ticketmessage.GetBody()->GetTicket()), PSK, ticketmessage.GetBody()->GetTicketAgeAdd(), ticketmessage.GetBody()->GetTicketLifetime(), session->GetKeySchedule()->GetCipherSuite(), protocol, ticketmessage.GetBody()->GetMaximumEarlyDataSize());

  if(!PSK.IsEmpty()) PSK.FillBuffer(0);
  PSK.SecureDelete();
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKECLIENT::RecordInput_Add(XBYTE* data, XDWORD size)
* @brief      Add received transport bytes to the TLS session
* @ingroup    DATAIO
*
* @param[in]  data : Bytes to add.
* @param[in]  size : Number of bytes to add.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKECLIENT::RecordInput_Add(XBYTE* data, XDWORD size)
{
  if(!isini || !session)
    {
      return false;
    }

  return session->RecordInput_Add(data, size);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKECLIENT::RecordInput_Add(XBUFFER& data)
* @brief      Add a received transport buffer to the TLS session
* @ingroup    DATAIO
*
* @param[in]  data : Buffer to add.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKECLIENT::RecordInput_Add(XBUFFER& data)
{
  return RecordInput_Add(data.Get(), data.GetSize());
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKECLIENT::Process()
* @brief      Process every complete record and handshake message currently accumulated
* @note       An incomplete record or message is retained and is not an error.
* @ingroup    DATAIO
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKECLIENT::Process()
{
  if(!isini || !session || (state == DIOSTREAMTLS13HANDSHAKECLIENT_STATE_ERROR))
    {
      return false;
    }

  while(true)
    {
      DIOSTREAMTLS_CONTENTTYPE    contenttype = (DIOSTREAMTLS_CONTENTTYPE)0;
      DIOSTREAMTLS13SESSION_RESULT  result;
      XBUFFER                     plain;

      result = session->Record_Extract(contenttype, plain);

      if(result == DIOSTREAMTLS13SESSION_RESULT_INCOMPLETE)
        {
          return true;
        }

      if(result == DIOSTREAMTLS13SESSION_RESULT_ERROR)
        {
          return SetError();
        }

      switch(contenttype)
        {
          case DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE          : if(!session->HandshakeInput_Add(plain)) return SetError();
                                                                break;

          case DIOSTREAMTLS_MSG_CONTENTTYPE_CHANGE_CIPHER_SPEC : if((plain.GetSize() != 1) || (plain.GetByte(0) != 1)) return SetError();
                                                                break;

          case DIOSTREAMTLS_MSG_CONTENTTYPE_ALERT              : { DIOSTREAMTLS_MSG_ALERT alert;

                                                                  if(!alert.GetFromBuffer(plain, false) || !plain.IsEmpty()) return SetError();

                                                                  // The peer said WHY it is refusing. Without this the caller only sees
                                                                  // "Handshake_Client() returned false" and every cause looks identical.
                                                                  // description 40 = handshake_failure (no acceptable parameters offered),
                                                                  // 47 = illegal_parameter, 42/48 = certificate/CA problems, 70 = version.
                                                                  return SetError();
                                                                }

                                                       default :
                                                                 return SetError();
        }

      while(true)
        {
          XBUFFER handshake;

          result = session->Handshake_Extract(handshake);

          if(result == DIOSTREAMTLS13SESSION_RESULT_INCOMPLETE)
            {
              break;
            }

          if(result == DIOSTREAMTLS13SESSION_RESULT_ERROR)
            {
              return SetError();
            }

          if(!Handshake_Process(handshake))
            {
              return false;
            }
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKECLIENT::Handshake_Process(XBUFFER& message)
* @brief      Process one exact server handshake message according to the current state
* @ingroup    DATAIO
*
* @param[in]  message : Complete handshake message, including its header.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKECLIENT::Handshake_Process(XBUFFER& message)
{
  DIOSTREAMTLS_MSG_HANDSHAKE genericmessage;
  XBUFFER                    workbuffer;

  if(!isini || !session || (state == DIOSTREAMTLS13HANDSHAKECLIENT_STATE_ERROR))
    {
      return false;
    }

  workbuffer.Add(message);

  if(!genericmessage.GetFromBuffer(workbuffer, false) || !workbuffer.IsEmpty())
    {
      return SetError();
    }

  switch(genericmessage.GetMsgType())
    {
      case DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_ENCRYPTED_EXTENSIONS : return EncryptedExtensions_Process(message);
      case DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_CERTIFICATE_REQUEST  : return CertificateRequest_Process(message);
      case DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_CERTIFICATE          : return Certificate_Process(message);
      case DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_CERTIFICATE_VERIFY   : return CertificateVerify_Process(message);
      case DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_FINISHED             : return Finished_Process(message);
                                                            default   : return SetError();
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKECLIENT::EncryptedExtensions_Process(XBUFFER& message)
* @brief      Decode EncryptedExtensions and advance the transcript
* @note       INTERNAL
* @ingroup    DATAIO
*
* @param[in]  message : Complete handshake message, including its header.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKECLIENT::EncryptedExtensions_Process(XBUFFER& message)
{
  DIOSTREAMTLS_MSG_FRAGMENT<DIOSTREAMTLS_MSG_HANDSHAKE_ENCRYPTEDEXTENSIONS> encryptedextensions;
  XBUFFER                                                                   workbuffer;

  if(state != DIOSTREAMTLS13HANDSHAKECLIENT_STATE_WAIT_ENCRYPTEDEXTENSIONS)
    {
      return SetError();
    }

  workbuffer.Add(message);

  if(!encryptedextensions.GetFromBuffer(workbuffer, false) || !workbuffer.IsEmpty() ||
     (encryptedextensions.GetMsgType() != DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_ENCRYPTED_EXTENSIONS))
    {
      return SetError();
    }

  applicationprotocolnegotiated = false;
  applicationprotocol           = DIOSTREAMTLS_ALPN_TYPE_UNKNOWN;
  applicationprotocolraw.Delete();

  for(XDWORD c=0; c<encryptedextensions.GetBody()->Extensions_GetAll()->GetSize(); c++)
    {
      DIOSTREAMTLS_MSG_EXTENSION* extension = encryptedextensions.GetBody()->Extensions_GetAll()->Get(c);
      if(!extension) return SetError();

      if(extension->GetType() == DIOSTREAMTLS_MSG_EXTENSION_TYPE_ALPN)
        {
          DIOSTREAMTLS_MSG_EXTENSION_ALPN* ALPN = (DIOSTREAMTLS_MSG_EXTENSION_ALPN*)extension;

          if(offeredapplicationprotocolsraw.IsEmpty() || (ALPN->List_GetNProtocols() != 1) ||
             !ALPN->List_Get(0, applicationprotocolraw) ||
             (applicationprotocolraw.GetSize() >= 3 && applicationprotocolraw.GetByte(0)=='h' &&
              applicationprotocolraw.GetByte(1)=='3' && applicationprotocolraw.GetByte(2)=='-') ||
             !ApplicationProtocol_IsOffered(applicationprotocolraw)) return SetError();

          ALPN->List_Get(0, applicationprotocol);

          applicationprotocolnegotiated = true;
        }
    }

  if(!session->Transcript_Add(message)) return SetError();
  if(earlydataoffered && !earlydataaccepted) session->EarlyData_End(DIOSTREAMTLSKEYSCHEDULE_DIRECTION_LOCAL);

  state = resumptionaccepted?DIOSTREAMTLS13HANDSHAKECLIENT_STATE_WAIT_FINISHED:
                             DIOSTREAMTLS13HANDSHAKECLIENT_STATE_WAIT_CERTIFICATE;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKECLIENT::CertificateRequest_Process(XBUFFER& message)
* @brief      Decode an optional CertificateRequest and advance the transcript
* @note       INTERNAL
* @ingroup    DATAIO
*
* @param[in]  message : Complete handshake message, including its header.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKECLIENT::CertificateRequest_Process(XBUFFER& message)
{
  DIOSTREAMTLS_MSG_FRAGMENT<DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEREQUEST> certificaterequest;
  XBUFFER                                                                 workbuffer;
  bool                                                                    signaturealgorithms     = false;
  bool                                                                    signaturealgorithmscert = false;
  bool                                                                    certificateauthorities  = false;
  bool                                                                    statusrequest           = false;

  if((state != DIOSTREAMTLS13HANDSHAKECLIENT_STATE_WAIT_CERTIFICATE) || certificaterequested)
    {
      return SetError();
    }

  workbuffer.Add(message);

  if(!certificaterequest.GetFromBuffer(workbuffer, false) || !workbuffer.IsEmpty() ||
     (certificaterequest.GetMsgType() != DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_CERTIFICATE_REQUEST) ||
     !certificaterequest.GetBody()->GetRequestContext()->IsEmpty())
    {
      return SetError();
    }

  for(XDWORD c=0; c<certificaterequest.GetBody()->Extensions_GetAll()->GetSize(); c++)
    {
      DIOSTREAMTLS_MSG_EXTENSION* extension = certificaterequest.GetBody()->Extensions_GetAll()->Get(c);
      if(!extension) return SetError();

      if(extension->GetType() == DIOSTREAMTLS_MSG_EXTENSION_TYPE_SIGNATUREALGORITHMS)
        {
          DIOSTREAMTLS_MSG_EXTENSION_SIGNATUREALGORITHMS* algorithms = (DIOSTREAMTLS_MSG_EXTENSION_SIGNATUREALGORITHMS*)extension;

          if(signaturealgorithms) return SetError();
          signaturealgorithms = true;
          requestedclientsignatureschemes.DeleteAll();

          for(XDWORD d=0; d<algorithms->List_Get()->GetSize(); d++)
            {
              if(!requestedclientsignatureschemes.Add(algorithms->List_Get()->Get(d))) return SetError();
            }
        }

      if(extension->GetType() == DIOSTREAMTLS_MSG_EXTENSION_TYPE_SIGNATUREALGORITHMSCERT)
        {
          DIOSTREAMTLS_MSG_EXTENSION_SIGNATUREALGORITHMSCERT* algorithms = (DIOSTREAMTLS_MSG_EXTENSION_SIGNATUREALGORITHMSCERT*)extension;
          if(signaturealgorithmscert) return SetError();
          signaturealgorithmscert = true;
          requestedclientcertificatesignatureschemes.DeleteAll();
          for(XDWORD d=0; d<algorithms->List_Get()->GetSize(); d++)
            if(!requestedclientcertificatesignatureschemes.Add(algorithms->List_Get()->Get(d))) return SetError();
        }

      if(extension->GetType() == DIOSTREAMTLS_MSG_EXTENSION_TYPE_CERTIFICATEAUTHORITIES)
        {
          DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN* CAextension = (DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN*)extension;
          XBUFFER* data=CAextension->GetData();
          if(certificateauthorities || !data || data->GetSize()<2) return SetError();
          certificateauthorities=true;
          requestedclientcertificateauthorities.DeleteContents();
          requestedclientcertificateauthorities.DeleteAll();
          XDWORD declared=((XDWORD)data->GetByte(0)<<8)|data->GetByte(1);
          if(declared != data->GetSize()-2) return SetError();
          XDWORD offset=2;
          while(offset < data->GetSize())
            {
              if(offset+2 > data->GetSize()) return SetError();
              XDWORD size=((XDWORD)data->GetByte(offset)<<8)|data->GetByte(offset+1); offset+=2;
              if(!size || offset+size > data->GetSize()) return SetError();
              XBUFFER* authority=GEN_NEW XBUFFER();
              if(!authority || !authority->Add(data->Get()+offset, size) || !requestedclientcertificateauthorities.Add(authority))
                { if(authority) GEN_DELETE authority; return SetError(); }
              offset+=size;
            }
        }

      if(extension->GetType() == DIOSTREAMTLS_MSG_EXTENSION_TYPE_STATUSREQUEST)
        {
          DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN* OCSPrequest = (DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN*)extension;

          if(statusrequest || !OCSPrequest->GetData() || !OCSPrequest->GetData()->IsEmpty()) return SetError();
          statusrequest = true;
        }
    }

  if(!signaturealgorithms || !session->Transcript_Add(message)) return SetError();
  if(!signaturealgorithmscert)
    {
      requestedclientcertificatesignatureschemes.DeleteAll();
      for(XDWORD c=0;c<requestedclientsignatureschemes.GetSize();c++)
        if(!requestedclientcertificatesignatureschemes.Add(requestedclientsignatureschemes.Get(c))) return SetError();
    }

  certificaterequested = true;
  clientOCSPstaplingrequested = statusrequest;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKECLIENT::CertificateChain_CompleteViaAIA(XVECTOR<XBUFFER*>& certificatechain)
* @brief      Retry validation after completing a chain the server sent incomplete, via AuthorityInfoAccess
* @note       INTERNAL
* @ingroup    DATAIO
*
* @param[in,out] certificatechain : Chain last passed to certificatevalidator.Validate(), extended in place with
*                each fetched intermediate. Only called right after that call left certificatevalidator holding an
*                UNTRUSTEDROOT error, so GetCertificateChain() below reflects the chain as received from the server.
*
* @return     bool : true if the chain now validates after the fetch(es); otherwise false, and certificatevalidator
*             is left holding whichever error the last attempt produced (its original UNTRUSTEDROOT error when no
*             fetch was even possible).
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKECLIENT::CertificateChain_CompleteViaAIA(XVECTOR<XBUFFER*>& certificatechain)
{
  if(!aiafetchactive || (certificatevalidator.GetError() != CIPHERCERTIFICATEX509VALIDATOR_ERROR_UNTRUSTEDROOT))
    {
      return false;
    }

  XVECTOR<XBUFFER*> fetchedcerts;
  bool              validated = false;

  XDWORD maximumfetches = config?config->GetMemoryPolicy()->GetMaximumAIAFetches():DIOSTREAMTLSAIAFETCHER_MAXCHAINFETCHES;
  for(XDWORD attempt=0; attempt<maximumfetches; attempt++)
    {
      XVECTOR<CIPHERCERTIFICATEX509*>* decodedchain = certificatevalidator.GetCertificateChain();
      CIPHERCERTIFICATEX509*           chainend     = decodedchain?decodedchain->GetLast():NULL;

      if(!chainend || !chainend->HasCAIssuersURL()) break;

      XBUFFER* fetched = GEN_NEW XBUFFER();
      if(!fetched) break;

      DIOSTREAMTLSAIAFETCHER fetcher;
      if(!fetcher.Fetch((*chainend->GetCAIssuersURL()), (*fetched), aiafetchtimeout,
                        config?config->GetMemoryPolicy()->GetMaximumAIAHeaderSize():DIOSTREAMTLSAIAFETCHER_MAXHEADERSIZE,
                        config?config->GetMemoryPolicy()->GetMaximumAIABodySize():DIOSTREAMTLSAIAFETCHER_MAXBODYSIZE) ||
         !fetchedcerts.Add(fetched) || !certificatechain.Add(fetched))
        {
          GEN_DELETE fetched;
          break;
        }

      if(certificatevalidator.ValidateAllPaths(&certificatechain, &trustedroots, expectedservername.Get(),
                                               hasvalidationdatetime?&validationdatetime:NULL))
        {
          validated = true;
          break;
        }

      if(certificatevalidator.GetError() != CIPHERCERTIFICATEX509VALIDATOR_ERROR_UNTRUSTEDROOT) break;
    }

  fetchedcerts.DeleteContents();
  fetchedcerts.DeleteAll();

  return validated;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKECLIENT::Certificate_Process(XBUFFER& message)
* @brief      Decode and retain the server Certificate message
* @note       INTERNAL
* @ingroup    DATAIO
*
* @param[in]  message : Complete handshake message, including its header.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKECLIENT::Certificate_Process(XBUFFER& message)
{
  DIOSTREAMTLS_MSG_FRAGMENT<DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATE> certificate;
  XBUFFER                                                          workbuffer;
  XVECTOR<XBUFFER*>                                                certificatechain;

  if(state != DIOSTREAMTLS13HANDSHAKECLIENT_STATE_WAIT_CERTIFICATE)
    {
      return SetError();
    }

  workbuffer.Add(message);

  if(!certificate.GetFromBuffer(workbuffer, false) || !workbuffer.IsEmpty() ||
     (certificate.GetMsgType() != DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_CERTIFICATE) ||
     !certificate.GetBody()->GetRequestContext()->IsEmpty() ||
     certificate.GetBody()->CertificateList_GetAll()->IsEmpty())
    {
      return SetError();
    }

  if(!authenticationconfigured)
    {
      SetAuthenticationError(DIOSTREAMTLS13HANDSHAKECLIENT_AUTHENTICATIONERROR_CONFIGURATION);
      return SetError();
    }

  for(XDWORD c=0; c<certificate.GetBody()->CertificateList_GetAll()->GetSize(); c++)
    {
      DIOSTREAMTLS_MSG_CERTIFICATEENTRY* entry;

      entry = certificate.GetBody()->CertificateList_GetAll()->Get(c);
      if(!entry || !entry->GetCertificateData() || !certificatechain.Add(entry->GetCertificateData()))
        {
          SetAuthenticationError(DIOSTREAMTLS13HANDSHAKECLIENT_AUTHENTICATIONERROR_CERTIFICATE);
          return SetError();
        }
    }

  if(!certificatevalidator.ValidateAllPaths(&certificatechain, &trustedroots, expectedservername.Get(),
                                             hasvalidationdatetime?&validationdatetime:NULL) &&
     !CertificateChain_CompleteViaAIA(certificatechain))
    {
      certificatevalidationerror = certificatevalidator.GetError();

      // 2=invalid certificate, 3=unsupported algorithm, 4=dates, 5=name, 6=key usage, 7=invalid CA,
      // 8=invalid signature, 9=untrusted root, 10=unknown critical extension, 11=path length.

      SetAuthenticationError(DIOSTREAMTLS13HANDSHAKECLIENT_AUTHENTICATIONERROR_CERTIFICATE);
      return SetError();
    }

  certificatevalidationerror = CIPHERCERTIFICATEX509VALIDATOR_ERROR_NONE;

  DIOSTREAMTLS_MSG_CERTIFICATEENTRY* leafentry = certificate.GetBody()->CertificateList_GetAll()->Get(0);
  if(!leafentry)
    {
      SetAuthenticationError(DIOSTREAMTLS13HANDSHAKECLIENT_AUTHENTICATIONERROR_CERTIFICATE);
      return SetError();
    }

  bool hasstapledOCSP=false;
  const bool revocationenabled = !config || config->GetRevocationPolicy()!=DIOSTREAMTLS_REVOCATIONPOLICY_OFF;
  for(XDWORD c=0; revocationenabled && c<leafentry->Extensions_GetAll()->GetSize(); c++)
    {
      DIOSTREAMTLS_MSG_EXTENSION* extension = leafentry->Extensions_GetAll()->Get(c);
      if(!extension || extension->GetType()!=DIOSTREAMTLS_MSG_EXTENSION_TYPE_STATUSREQUEST) continue;
      if(hasstapledOCSP)
        {
          certificatevalidationerror=CIPHERCERTIFICATEX509VALIDATOR_ERROR_REVOCATIONUNKNOWN;
          SetAuthenticationError(DIOSTREAMTLS13HANDSHAKECLIENT_AUTHENTICATIONERROR_CERTIFICATE);
          return SetError();
        }
      DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN* status = (DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN*)extension;
      hasstapledOCSP=true;
      XBUFFER* data = status->GetData();
      XVECTOR<CIPHERCERTIFICATEX509*>* validatedchain = certificatevalidator.GetCertificateChain();
      if(!data || data->GetSize()<5 || data->Get()[0]!=1 ||
         (((XDWORD)data->Get()[1]<<16)|((XDWORD)data->Get()[2]<<8)|data->Get()[3]) != data->GetSize()-4 ||
         ((data->GetSize()-4) > (config?config->GetMemoryPolicy()->GetMaximumOCSPResponseSize():CIPHERCERTIFICATEX509REVOCATION_MAX_OCSP_SIZE)) ||
         !validatedchain || validatedchain->GetSize()<2)
        {
          certificatevalidationerror=CIPHERCERTIFICATEX509VALIDATOR_ERROR_REVOCATIONUNKNOWN;
          SetAuthenticationError(DIOSTREAMTLS13HANDSHAKECLIENT_AUTHENTICATIONERROR_CERTIFICATE);
          return SetError();
        }
      XBUFFER OCSP;
      if(!OCSP.Add(data->Get()+4, data->GetSize()-4)) return SetError();
      CIPHERCERTIFICATEX509REVOCATION_RESULT result = CIPHERCERTIFICATEX509REVOCATION::ValidateOCSP(OCSP, (*validatedchain->Get(0)), (*validatedchain->Get(1)));
      if(result==CIPHERCERTIFICATEX509REVOCATION_RESULT_REVOKED ||
         (config && config->GetRevocationPolicy()!=DIOSTREAMTLS_REVOCATIONPOLICY_SOFT_FAIL &&
          result!=CIPHERCERTIFICATEX509REVOCATION_RESULT_GOOD))
        {
          certificatevalidationerror=(result==CIPHERCERTIFICATEX509REVOCATION_RESULT_REVOKED)?
            CIPHERCERTIFICATEX509VALIDATOR_ERROR_REVOKED:CIPHERCERTIFICATEX509VALIDATOR_ERROR_REVOCATIONUNKNOWN;
          SetAuthenticationError(DIOSTREAMTLS13HANDSHAKECLIENT_AUTHENTICATIONERROR_CERTIFICATE);
          return SetError();
        }
    }

  if(revocationenabled && !hasstapledOCSP && config && config->GetOCSPDirectFetcher())
    {
      XVECTOR<CIPHERCERTIFICATEX509*>* chain=certificatevalidator.GetCertificateChain();
      XBUFFER response;
      CIPHERCERTIFICATEX509REVOCATION_RESULT result=CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID;
      if(chain && chain->GetSize()>=2 && chain->Get(0)->HasOCSPURL() &&
         config->GetOCSPDirectFetcher()((*chain->Get(0)->GetOCSPURL()),(*chain->Get(0)),(*chain->Get(1)),response,config->GetOCSPDirectContext()) &&
         response.GetSize() <= config->GetMemoryPolicy()->GetMaximumOCSPResponseSize())
        result=CIPHERCERTIFICATEX509REVOCATION::ValidateOCSP(response, (*chain->Get(0)), (*chain->Get(1)));
      if(result==CIPHERCERTIFICATEX509REVOCATION_RESULT_REVOKED ||
         (config->GetRevocationPolicy()!=DIOSTREAMTLS_REVOCATIONPOLICY_SOFT_FAIL &&
          result!=CIPHERCERTIFICATEX509REVOCATION_RESULT_GOOD))
        {
          certificatevalidationerror=(result==CIPHERCERTIFICATEX509REVOCATION_RESULT_REVOKED)?
            CIPHERCERTIFICATEX509VALIDATOR_ERROR_REVOKED:CIPHERCERTIFICATEX509VALIDATOR_ERROR_REVOCATIONUNKNOWN;
          SetAuthenticationError(DIOSTREAMTLS13HANDSHAKECLIENT_AUTHENTICATIONERROR_CERTIFICATE);
          return SetError();
        }
    }

  if(revocationenabled && config && config->GetCertificateRevocationLists() && !config->GetCertificateRevocationLists()->IsEmpty())
    {
      XVECTOR<CIPHERCERTIFICATEX509*>* chain=certificatevalidator.GetCertificateChain();
      if(!chain || chain->GetSize()<2) return SetError();
      for(XDWORD certindex=0; certindex+1<chain->GetSize(); certindex++)
        {
          bool valid=false;
          for(XDWORD c=0;c<config->GetCertificateRevocationLists()->GetSize();c++)
            {
              XBUFFER* CRL=config->GetCertificateRevocationLists()->Get(c);
              if(!CRL || CRL->GetSize() > config->GetMemoryPolicy()->GetMaximumCRLSize()) continue;
              CIPHERCERTIFICATEX509REVOCATION_RESULT result=CIPHERCERTIFICATEX509REVOCATION::ValidateCRL((*CRL), (*chain->Get(certindex)), (*chain->Get(certindex+1)));
              if(result==CIPHERCERTIFICATEX509REVOCATION_RESULT_REVOKED)
                { certificatevalidationerror=CIPHERCERTIFICATEX509VALIDATOR_ERROR_REVOKED; SetAuthenticationError(DIOSTREAMTLS13HANDSHAKECLIENT_AUTHENTICATIONERROR_CERTIFICATE); return SetError(); }
              if(result==CIPHERCERTIFICATEX509REVOCATION_RESULT_GOOD) valid=true;
            }
          if(!valid && config->GetRevocationPolicy()!=DIOSTREAMTLS_REVOCATIONPOLICY_SOFT_FAIL)
            { certificatevalidationerror=CIPHERCERTIFICATEX509VALIDATOR_ERROR_REVOCATIONUNKNOWN; SetAuthenticationError(DIOSTREAMTLS13HANDSHAKECLIENT_AUTHENTICATIONERROR_CERTIFICATE); return SetError(); }
        }
    }
  if(config && config->GetRevocationPolicy()==DIOSTREAMTLS_REVOCATIONPOLICY_MUST_STAPLE && !hasstapledOCSP)
    {
      certificatevalidationerror=CIPHERCERTIFICATEX509VALIDATOR_ERROR_REVOCATIONUNKNOWN;
      SetAuthenticationError(DIOSTREAMTLS13HANDSHAKECLIENT_AUTHENTICATIONERROR_CERTIFICATE);
      return SetError();
    }

  DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATE* decodedcertificate = GEN_NEW DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATE();
  if(!decodedcertificate)
    {
      return SetError();
    }

  XBUFFER bodybuffer;
  if(!certificate.GetBody()->SetToBuffer(bodybuffer, false))
    {
      GEN_DELETE decodedcertificate;
      return SetError();
    }

  if(!decodedcertificate->GetFromBuffer(bodybuffer, false) || !bodybuffer.IsEmpty() ||
     !session->Transcript_Add(message))
    {
      GEN_DELETE decodedcertificate;
      return SetError();
    }

  if(servercertificate) GEN_DELETE servercertificate;
  servercertificate = decodedcertificate;
  state             = DIOSTREAMTLS13HANDSHAKECLIENT_STATE_WAIT_CERTIFICATEVERIFY;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKECLIENT::CertificateVerify_Process(XBUFFER& message)
* @brief      Decode, cryptographically verify and retain the server CertificateVerify message
* @note       INTERNAL
* @ingroup    DATAIO
*
* @param[in]  message : Complete handshake message, including its header.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKECLIENT::CertificateVerify_Process(XBUFFER& message)
{
  DIOSTREAMTLS_MSG_FRAGMENT<DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEVERIFY> certificateverify;
  XBUFFER                                                              workbuffer;
  XBUFFER                                                              transcripthash;
  XBUFFER                                                              signedcontent;

  if(state != DIOSTREAMTLS13HANDSHAKECLIENT_STATE_WAIT_CERTIFICATEVERIFY)
    {
      return SetError();
    }

  workbuffer.Add(message);

  if(!certificateverify.GetFromBuffer(workbuffer, false) || !workbuffer.IsEmpty() ||
     (certificateverify.GetMsgType() != DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_CERTIFICATE_VERIFY) ||
     !SignatureScheme_IsOffered(certificateverify.GetBody()->GetAlgorithm()))
    {
      SetAuthenticationError(DIOSTREAMTLS13HANDSHAKECLIENT_AUTHENTICATIONERROR_CERTIFICATEVERIFY);
      return SetError();
    }

  static XBYTE context[] = { 'T', 'L', 'S', ' ', '1', '.', '3', ',', ' ',
                             's', 'e', 'r', 'v', 'e', 'r', ' ',
                             'C', 'e', 'r', 't', 'i', 'f', 'i', 'c', 'a', 't', 'e', 'V', 'e', 'r', 'i', 'f', 'y' };

  CIPHERCERTIFICATEX509* leaf = certificatevalidator.GetLeafCertificate();
  if(!authenticationconfigured || !leaf || !leaf->GetPublicCipherKey() ||
     !DIOSTREAMTLSSIGNATURE::IsSupported(certificateverify.GetBody()->GetAlgorithm(), leaf) ||
     !session->TranscriptHash(transcripthash))
    {
      SetAuthenticationError(DIOSTREAMTLS13HANDSHAKECLIENT_AUTHENTICATIONERROR_CERTIFICATEVERIFY);
      return SetError();
    }

  for(int c=0; c<64; c++)
    {
      if(!signedcontent.Add((XBYTE)0x20))
        {
          SetAuthenticationError(DIOSTREAMTLS13HANDSHAKECLIENT_AUTHENTICATIONERROR_CERTIFICATEVERIFY);
          return SetError();
        }
    }

  if(!signedcontent.Add(context, sizeof(context)) || !signedcontent.Add((XBYTE)0x00) ||
     !signedcontent.Add(transcripthash))
    {
      SetAuthenticationError(DIOSTREAMTLS13HANDSHAKECLIENT_AUTHENTICATIONERROR_CERTIFICATEVERIFY);
      return SetError();
    }

  if(!DIOSTREAMTLSSIGNATURE::Verify(certificateverify.GetBody()->GetAlgorithm(), leaf->GetPublicCipherKey(),
                                    signedcontent, *certificateverify.GetBody()->GetSignature()))
    {

      SetAuthenticationError(DIOSTREAMTLS13HANDSHAKECLIENT_AUTHENTICATIONERROR_CERTIFICATEVERIFY);
      return SetError();
    }


  DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEVERIFY* decodedcertificateverify;
  decodedcertificateverify = GEN_NEW DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEVERIFY();
  if(!decodedcertificateverify)
    {
      return SetError();
    }

  XBUFFER bodybuffer;
  if(!certificateverify.GetBody()->SetToBuffer(bodybuffer, false))
    {
      GEN_DELETE decodedcertificateverify;
      return SetError();
    }

  if(!decodedcertificateverify->GetFromBuffer(bodybuffer, false) || !bodybuffer.IsEmpty() ||
     !session->Transcript_Add(message))
    {
      GEN_DELETE decodedcertificateverify;
      return SetError();
    }

  if(servercertificateverify) GEN_DELETE servercertificateverify;
  servercertificateverify = decodedcertificateverify;

  serverauthenticated = true;
  authenticationerror = DIOSTREAMTLS13HANDSHAKECLIENT_AUTHENTICATIONERROR_NONE;

  state                   = DIOSTREAMTLS13HANDSHAKECLIENT_STATE_WAIT_FINISHED;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKECLIENT::Finished_Process(XBUFFER& message)
* @brief      Verify the server Finished and activate remote application keys
* @note       INTERNAL
* @ingroup    DATAIO
*
* @param[in]  message : Complete handshake message, including its header.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKECLIENT::Finished_Process(XBUFFER& message)
{
  DIOSTREAMTLS_MSG_FRAGMENT<DIOSTREAMTLS_MSG_HANDSHAKE_FINISHED> finished;
  XBUFFER                                                       workbuffer;
  XBUFFER                                                       transcripthash;

  if((state != DIOSTREAMTLS13HANDSHAKECLIENT_STATE_WAIT_FINISHED) ||
     !serverauthenticated)
    {
      return SetError();
    }

  workbuffer.Add(message);

  if(!finished.GetFromBuffer(workbuffer, false) || !workbuffer.IsEmpty() ||
     (finished.GetMsgType() != DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_FINISHED) ||
     (finished.GetBody()->GetVerifyData()->GetSize() != session->GetKeySchedule()->GetHashSize()) ||
     !session->TranscriptHash(transcripthash) ||
     !session->GetKeySchedule()->VerifyFinished(DIOSTREAMTLSKEYSCHEDULE_DIRECTION_REMOTE,
                                                transcripthash, *finished.GetBody()->GetVerifyData()))
    {
      return SetError();
    }

  if(!session->Transcript_Add(message) ||
     !session->ApplicationKeys_Activate(DIOSTREAMTLSKEYSCHEDULE_DIRECTION_REMOTE))
    {
      return SetError();
    }

  state = DIOSTREAMTLS13HANDSHAKECLIENT_STATE_SERVERFINISHED_VERIFIED;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKECLIENT::SetError()
* @brief      Move the handshake to its terminal error state
* @note       INTERNAL
* @ingroup    DATAIO
*
* @return     bool : Always false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKECLIENT::SetError()
{
  state = DIOSTREAMTLS13HANDSHAKECLIENT_STATE_ERROR;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKECLIENT::SetAuthenticationError(DIOSTREAMTLS13HANDSHAKECLIENT_AUTHENTICATIONERROR error)
* @brief      Set a server authentication error
* @note       INTERNAL
* @ingroup    DATAIO
*
* @param[in]  error : Error value.
*
* @return     bool : Always false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKECLIENT::SetAuthenticationError(DIOSTREAMTLS13HANDSHAKECLIENT_AUTHENTICATIONERROR error)
{
  authenticationerror = error;
  serverauthenticated = false;

  if(error == DIOSTREAMTLS13HANDSHAKECLIENT_AUTHENTICATIONERROR_CONFIGURATION)
    {
      authenticationconfigured = false;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKECLIENT::CipherSuite_IsOffered(XWORD ciphersuite)
* @brief      Check whether a cipher suite was present in the registered ClientHello
* @note       INTERNAL
* @ingroup    DATAIO
*
* @param[in]  ciphersuite : Cipher suite value.
*
* @return     bool : true if the condition is met; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKECLIENT::CipherSuite_IsOffered(XWORD ciphersuite)
{
  for(XDWORD c=0; c<offeredciphersuites.GetSize(); c++)
    {
      if(offeredciphersuites.Get(c) == ciphersuite) return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKECLIENT::SupportedGroup_IsOffered(XWORD supportedgroup)
* @brief      Check whether a group was present in the registered supported_groups extension
* @note       INTERNAL
* @ingroup    DATAIO
*
* @param[in]  supportedgroup : Supported group value.
*
* @return     bool : true if the group was offered; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKECLIENT::SupportedGroup_IsOffered(XWORD supportedgroup)
{
  for(XDWORD c=0; c<offeredsupportedgroups.GetSize(); c++)
    {
      if(offeredsupportedgroups.Get(c) == supportedgroup) return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKECLIENT::KeyShare_IsOffered(XWORD supportedgroup)
* @brief      Check whether a group was present in the registered key_share extension
* @note       INTERNAL
* @ingroup    DATAIO
*
* @param[in]  supportedgroup : Supported group value.
*
* @return     bool : true if the key share was offered; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKECLIENT::KeyShare_IsOffered(XWORD supportedgroup)
{
  for(XDWORD c=0; c<offeredkeysharegroups.GetSize(); c++)
    {
      if(offeredkeysharegroups.Get(c) == supportedgroup) return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKECLIENT::SignatureScheme_IsOffered(XWORD signaturescheme)
* @brief      Check whether a signature scheme was present in the registered ClientHello
* @note       INTERNAL
* @ingroup    DATAIO
*
* @param[in]  signaturescheme : Signature scheme value.
*
* @return     bool : true if the condition is met; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKECLIENT::SignatureScheme_IsOffered(XWORD signaturescheme)
{
  for(XDWORD c=0; c<offeredsignatureschemes.GetSize(); c++)
    {
      if(offeredsignatureschemes.Get(c) == signaturescheme) return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKECLIENT::ApplicationProtocol_IsOffered(DIOSTREAMTLS_ALPN_TYPE applicationprotocol)
* @brief      Check whether an ALPN protocol was present in the registered ClientHello
* @note       INTERNAL
* @ingroup    DATAIO
*
* @param[in]  applicationprotocol : Application protocol value.
*
* @return     bool : true if the condition is met; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKECLIENT::ApplicationProtocol_IsOffered(DIOSTREAMTLS_ALPN_TYPE applicationprotocol)
{
  for(XDWORD c=0; c<offeredapplicationprotocols.GetSize(); c++)
    {
      if(offeredapplicationprotocols.Get(c) == applicationprotocol) return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS13HANDSHAKECLIENT::ApplicationProtocol_IsOffered(XBUFFER& applicationprotocol)
* @brief      Application protocol is offered
* @ingroup    DATAIO
* 
* @param[in]  applicationprotocol : Applicationprotocol value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKECLIENT::ApplicationProtocol_IsOffered(XBUFFER& applicationprotocol)
{
  for(XDWORD c=0; c<offeredapplicationprotocolsraw.GetSize(); c++)
    {
      XBUFFER* offered = offeredapplicationprotocolsraw.Get(c);
      if(offered && offered->Compare(applicationprotocol)) return true;
    }
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS13HANDSHAKECLIENT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS13HANDSHAKECLIENT::Clean()
{
  session                    = NULL;
  config                     = NULL;
  state                      = DIOSTREAMTLS13HANDSHAKECLIENT_STATE_NONE;
  isini                      = false;
  certificaterequested       = false;
  clientOCSPstaplingrequested = false;
  requestedclientsignatureschemes.DeleteAll();
  requestedclientcertificatesignatureschemes.DeleteAll();
  requestedclientcertificateauthorities.DeleteContents();
  requestedclientcertificateauthorities.DeleteAll();
  authenticationconfigured  = false;
  serverauthenticated        = false;
  authenticationerror        = DIOSTREAMTLS13HANDSHAKECLIENT_AUTHENTICATIONERROR_NONE;
  certificatevalidationerror = CIPHERCERTIFICATEX509VALIDATOR_ERROR_NONE;
  hasvalidationdatetime      = false;
  applicationprotocolnegotiated = false;
  applicationprotocol           = DIOSTREAMTLS_ALPN_TYPE_UNKNOWN;
  applicationprotocolraw.Delete();
  resumptionoffered             = false;
  resumptionaccepted            = false;
  resumptionpsk.Delete();
  resumptionciphersuite         = 0;
  earlydataoffered              = false;
  earlydataaccepted             = false;
  maximumearlydatasize          = 0;
  preparedearlydatasize          = 0;
  currentkeysharegroup          = 0;
  helloretryrequestprocessed    = false;
  servercertificate          = NULL;
  servercertificateverify    = NULL;
  aiafetchactive              = true;
  aiafetchtimeout             = DIOSTREAMTLSAIAFETCHER_TIMEOUT;
}
