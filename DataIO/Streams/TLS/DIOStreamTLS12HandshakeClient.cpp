/**-------------------------------------------------------------------------------------------------------------------
*
* @file       DIOStreamTLS12HandshakeClient.cpp
*
* @class      DIOSTREAMTLS12HANDSHAKECLIENT
* @brief      Data Input/Output Stream TLS 1.2 (RFC 5246) Client Handshake class
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

#include "DIOStreamTLS12HandshakeClient.h"

#include "DIOStreamTLSMessagesHandShakeClientHello.h"
#include "DIOStreamTLSMessagesHandShakeServerHello.h"
#include "DIOStreamTLSMessagesHandShakeServerFlight.h"
#include "DIOStreamTLSSignature.h"
#include "DIOStreamTLSAIAFetcher.h"
#include "CipherCertificateX509Revocation.h"
#include "Cipher.h"

#include "XRand.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"

#include "XTrace.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS12HANDSHAKECLIENT::DIOSTREAMTLS12HANDSHAKECLIENT()
* @brief      Constructor of class
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS12HANDSHAKECLIENT::DIOSTREAMTLS12HANDSHAKECLIENT()
{
  servercertificate = NULL;
  serverkeyexchange = NULL;

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS12HANDSHAKECLIENT::~DIOSTREAMTLS12HANDSHAKECLIENT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS12HANDSHAKECLIENT::~DIOSTREAMTLS12HANDSHAKECLIENT()
{
  End();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS12HANDSHAKECLIENT::Ini(bool checkdowngradesentinel)
* @brief      Initialize the client handshake
* @ingroup    DATAIO
*
* @param[in]  checkdowngradesentinel : Reject the RFC 8446 4.1.3 downgrade sentinel in ServerHello.random. Pass
*             true only for a dual-version TLS 1.3-to-1.2 fallback attempt; leave false for a deliberately
*             configured TLS-1.2-only connection (see the declaration in the header for why).
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS12HANDSHAKECLIENT::Ini(bool checkdowngradesentinel)
{
  End();

  this->checkdowngradesentinel     = checkdowngradesentinel;
  state                            = DIOSTREAMTLS12HANDSHAKECLIENT_STATE_NONE;
  isini                            = true;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS12HANDSHAKECLIENT::Capabilities_Set(DIOSTREAMTLSCONFIG* config)
* @brief      Capabilities set
* @ingroup    DATAIO
* 
* @param[in]  config : Pointer to config.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS12HANDSHAKECLIENT::Capabilities_Set(DIOSTREAMTLSCONFIG* config)
{
  if(!isini || (state != DIOSTREAMTLS12HANDSHAKECLIENT_STATE_NONE) || !config ||
     !config->GetTLS12CipherSuites() || config->GetTLS12CipherSuites()->IsEmpty() ||
     !config->GetSupportedGroups() || config->GetSupportedGroups()->IsEmpty() ||
     !config->GetSignatureSchemes() || config->GetSignatureSchemes()->IsEmpty()) return false;

  this->config = config;
  ciphersuites.DeleteAll();
  supportedgroups.DeleteAll();
  signatureschemes.DeleteAll();

  for(XDWORD c=0; c<config->GetSignatureSchemes()->GetSize(); c++)
    {
      XWORD scheme = config->GetSignatureSchemes()->Get(c);
      switch(scheme)
        {
          case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA256    :
          case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA384    :
          case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA512    :
          case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA256     :
          case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA384     :
          case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA512     :
          case DIOSTREAMTLS_MSG_SIGNATURESCHEME_ECDSA_SECP256R1_SHA256 :
          case DIOSTREAMTLS_MSG_SIGNATURESCHEME_ECDSA_SECP384R1_SHA384 :
          case DIOSTREAMTLS_MSG_SIGNATURESCHEME_ECDSA_SECP521R1_SHA512 : break;
                                                                     default : continue;
        }

      if(!signatureschemes.Add(scheme)) return false;
    }

  // TLS 1.2 still permits RSA PKCS#1 v1.5 with SHA-2 for ServerKeyExchange. TLS 1.3 deliberately
  // excludes these schemes, so DIOSTREAMTLSCONFIG keeps them in the certificate-signature policy instead of the
  // TLS 1.3 handshake-signature list. In TLS 1.2 there is only one signature_algorithms extension for this purpose;
  // therefore add the SHA-2 PKCS#1 schemes only when the caller's certificate policy explicitly authorizes them.
  if(config->GetCertificateSignatureSchemes())
    {
      for(XDWORD c=0; c<config->GetCertificateSignatureSchemes()->GetSize(); c++)
        {
          XWORD scheme = config->GetCertificateSignatureSchemes()->Get(c);

          if((scheme != DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PKCS1_SHA256) &&
             (scheme != DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PKCS1_SHA384) &&
             (scheme != DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PKCS1_SHA512)) continue;

          bool alreadypresent = false;
          for(XDWORD d=0; d<signatureschemes.GetSize(); d++)
            {
              if(signatureschemes.Get(d) == scheme)
                {
                  alreadypresent = true;
                  break;
                }
            }

          if(!alreadypresent && !signatureschemes.Add(scheme)) return false;
        }
    }

  for(XDWORD c=0; c<config->GetSupportedGroups()->GetSize(); c++)
    {
      XWORD group = config->GetSupportedGroups()->Get(c);
      if((group != DIOSTREAMTLS_MSG_CURVEID_X25519) &&
         (group != DIOSTREAMTLS_MSG_CURVEID_SECP256R1) &&
         (group != DIOSTREAMTLS_MSG_CURVEID_SECP384R1) &&
         (group != DIOSTREAMTLS_MSG_CURVEID_SECP521R1)) continue;
      if(!supportedgroups.Add(group)) return false;
    }

  bool allowRSA = false;
  bool allowECDSA = false;
  for(XDWORD c=0; c<signatureschemes.GetSize(); c++)
    {
      XWORD scheme = signatureschemes.Get(c);
      if((scheme == DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA256) ||
         (scheme == DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA384) ||
         (scheme == DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA512) ||
         (scheme == DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA256)  ||
         (scheme == DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA384)  ||
         (scheme == DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA512)  ||
         (scheme == DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PKCS1_SHA256)     ||
         (scheme == DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PKCS1_SHA384)     ||
         (scheme == DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PKCS1_SHA512)) allowRSA = true;
      if((scheme == DIOSTREAMTLS_MSG_SIGNATURESCHEME_ECDSA_SECP256R1_SHA256) ||
         (scheme == DIOSTREAMTLS_MSG_SIGNATURESCHEME_ECDSA_SECP384R1_SHA384) ||
         (scheme == DIOSTREAMTLS_MSG_SIGNATURESCHEME_ECDSA_SECP521R1_SHA512)) allowECDSA = true;
    }

  for(XDWORD c=0; c<config->GetTLS12CipherSuites()->GetSize(); c++)
    {
      XWORD suite = config->GetTLS12CipherSuites()->Get(c);
      bool  RSA   = (suite == DIOSTREAMTLS12_CIPHER_ECDHE_RSA_WITH_AES_128_GCM_SHA256) ||
                    (suite == DIOSTREAMTLS12_CIPHER_ECDHE_RSA_WITH_AES_256_GCM_SHA384) ||
                    (suite == DIOSTREAMTLS_MSG_CIPHER_ECDHE_RSA_WITH_CHACHA20_POLY1305_SHA256);
      bool  ECDSA = (suite == DIOSTREAMTLS12_CIPHER_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256) ||
                    (suite == DIOSTREAMTLS12_CIPHER_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384) ||
                    (suite == DIOSTREAMTLS_MSG_CIPHER_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256);

      if((RSA && allowRSA) || (ECDSA && allowECDSA))
        if(!ciphersuites.Add(suite)) return false;
    }

  if(ciphersuites.IsEmpty() || supportedgroups.IsEmpty() || signatureschemes.IsEmpty()) return false;

  if(!session.MemoryPolicy_Set((*config->GetMemoryPolicy()))) return false;
  certificatevalidator.SetPolicy((*config->GetCertificateValidationPolicy()));
  AIAFetch_Set(config->IsActiveAIAFetch(), config->GetAIAFetchTimeout());
  RevocationLists_Set(config->GetCertificateRevocationLists());
  OCSPDirect_Set(config->GetOCSPDirectFetcher(), config->GetOCSPDirectContext());
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS12HANDSHAKECLIENT::CipherSuitesAndSchemes_WidenECDSA()
* @brief      Last-resort fallback: add the ECDHE-ECDSA cipher suites and ECDSA schemes for one retry attempt
* @ingroup    DATAIO
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS12HANDSHAKECLIENT::CipherSuitesAndSchemes_WidenECDSA()
{
  // Kept for source compatibility. Capabilities_Set() already installs the complete configured policy;
  // a retry must never enable algorithms that the caller did not authorize.
  return isini && config && (state == DIOSTREAMTLS12HANDSHAKECLIENT_STATE_NONE) &&
         !ciphersuites.IsEmpty() && !signatureschemes.IsEmpty();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS12HANDSHAKECLIENT::IsAlgorithmRejected()
* @brief      Whether the peer answered with a FATAL handshake_failure/insufficient_security alert
* @ingroup    DATAIO
*
* @return     bool : true if the peer explicitly rejected the offer; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS12HANDSHAKECLIENT::IsAlgorithmRejected()
{
  return algorithmrejected;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS12HANDSHAKECLIENT::End()
* @brief      End the client handshake and release decoded peer messages
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS12HANDSHAKECLIENT::End()
{
  if(servercertificate)
    {
      GEN_DELETE servercertificate;
      servercertificate = NULL;
    }

  if(serverkeyexchange)
    {
      GEN_DELETE serverkeyexchange;
      serverkeyexchange = NULL;
    }

  session.End();

  ciphersuites.DeleteAll();
  supportedgroups.DeleteAll();
  signatureschemes.DeleteAll();

  trustedroots.DeleteContents();
  trustedroots.DeleteAll();
  certificatevalidator.End();
  expectedservername.Empty();

  clientrandom.Delete();
  serverrandom.Delete();
  clientkeyshare.Delete();
  premastersecret.FillBuffer(0);
  premastersecret.SecureDelete();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS12HANDSHAKECLIENT::IsIni()
* @brief      Check whether the client handshake is initialized
* @ingroup    DATAIO
*
* @return     bool : true if the condition is met; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS12HANDSHAKECLIENT::IsIni()
{
  return isini;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS12SESSION* DIOSTREAMTLS12HANDSHAKECLIENT::GetSession()
* @brief      Get the underlying session (record protection, key schedule)
* @ingroup    DATAIO
*
* @return     DIOSTREAMTLS12SESSION* : Pointer to the requested object.
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS12SESSION* DIOSTREAMTLS12HANDSHAKECLIENT::GetSession()
{
  return &session;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS12HANDSHAKECLIENT_STATE DIOSTREAMTLS12HANDSHAKECLIENT::GetState()
* @brief      Get the current client handshake state
* @ingroup    DATAIO
*
* @return     DIOSTREAMTLS12HANDSHAKECLIENT_STATE : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS12HANDSHAKECLIENT_STATE DIOSTREAMTLS12HANDSHAKECLIENT::GetState()
{
  return state;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS12HANDSHAKECLIENT::IsHandshakeCompleted()
* @brief      Check whether both application traffic directions are active
* @ingroup    DATAIO
*
* @return     bool : true if the condition is met; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS12HANDSHAKECLIENT::IsHandshakeCompleted()
{
  return (state == DIOSTREAMTLS12HANDSHAKECLIENT_STATE_HANDSHAKE_COMPLETED);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS12HANDSHAKECLIENT::IsServerAuthenticated()
* @brief      Check whether X.509 and the ServerKeyExchange signature authenticated the server
* @ingroup    DATAIO
*
* @return     bool : true if the server identity is authenticated; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS12HANDSHAKECLIENT::IsServerAuthenticated()
{
  return serverauthenticated;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS12HANDSHAKECLIENT::Authentication_Set(XCHAR* servername, XVECTOR<XBUFFER*>* trustedroots, XDATETIME* datetime)
* @brief      Configure the expected server name and trusted root store
* @ingroup    DATAIO
*
* @param[in]  servername : Expected server name (used both for SNI and certificate name checking).
* @param[in]  trustedroots : Trusted root certificates (DER), copied.
* @param[in]  datetime : Optional explicit validation instant; NULL uses the current time.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS12HANDSHAKECLIENT::Authentication_Set(XCHAR* servername, XVECTOR<XBUFFER*>* trustedroots, XDATETIME* datetime)
{
  if(!isini || (state != DIOSTREAMTLS12HANDSHAKECLIENT_STATE_NONE) ||
     !servername || !servername[0] ||
     !trustedroots || trustedroots->IsEmpty())
    {
      return SetAuthenticationError(DIOSTREAMTLS12HANDSHAKECLIENT_AUTHENTICATIONERROR_CONFIGURATION);
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
          return SetAuthenticationError(DIOSTREAMTLS12HANDSHAKECLIENT_AUTHENTICATIONERROR_CONFIGURATION);
        }

      copy = GEN_NEW XBUFFER();
      if(!copy || !copy->Add((*root)) || !this->trustedroots.Add(copy))
        {
          if(copy) GEN_DELETE copy;
          return SetAuthenticationError(DIOSTREAMTLS12HANDSHAKECLIENT_AUTHENTICATIONERROR_CONFIGURATION);
        }
    }

  expectedservername = servername;

  hasvalidationdatetime = false;
  if(datetime)
    {
      if(!datetime->IsValidDate() || !validationdatetime.CopyFrom(datetime))
        {
          return SetAuthenticationError(DIOSTREAMTLS12HANDSHAKECLIENT_AUTHENTICATIONERROR_CONFIGURATION);
        }

      hasvalidationdatetime = true;
    }

  authenticationconfigured   = true;
  authenticationerror        = DIOSTREAMTLS12HANDSHAKECLIENT_AUTHENTICATIONERROR_NONE;
  certificatevalidationerror = CIPHERCERTIFICATEX509VALIDATOR_ERROR_NONE;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS12HANDSHAKECLIENT::AIAFetch_Set(bool active, int timeout)
* @brief      Configure best-effort AuthorityInfoAccess fetching for an incomplete certificate chain
* @ingroup    DATAIO
*
* @param[in]  active : true to fetch a missing intermediate via id-ad-caIssuers when the chain the server sent
*             does not reach a trusted root by itself; false to keep the previous strict behaviour.
* @param[in]  timeout : Fetch connect / idle timeout, in seconds.
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS12HANDSHAKECLIENT::AIAFetch_Set(bool active, int timeout)
{
  aiafetchactive  = active;
  aiafetchtimeout = (timeout > 0)?timeout:DIOSTREAMTLSAIAFETCHER_TIMEOUT;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTLS12HANDSHAKECLIENT::ValidationPolicy_Set(CIPHERCERTIFICATEX509VALIDATIONPOLICY& policy)
* @brief      Validation policy set
* @ingroup    DATAIO
* 
* @param[in]  policy : Policy value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS12HANDSHAKECLIENT::ValidationPolicy_Set(CIPHERCERTIFICATEX509VALIDATIONPOLICY& policy)
{
  certificatevalidator.SetPolicy(policy);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTLS12HANDSHAKECLIENT::RevocationLists_Set(XVECTOR<XBUFFER*>* CRLs)
* @brief      Revocation lists set
* @ingroup    DATAIO
* 
* @param[in]  CRLs : Pointer to CRLs.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS12HANDSHAKECLIENT::RevocationLists_Set(XVECTOR<XBUFFER*>* CRLs)
{
  revocationlists = CRLs;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTLS12HANDSHAKECLIENT::OCSPDirect_Set(DIOSTREAMTLS_OCSPDIRECTFETCHER fetcher, void* context)
* @brief      Ocsp direct set
* @ingroup    DATAIO
* 
* @param[in]  fetcher : Fetcher value.
* @param[in]  context : Pointer to context.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS12HANDSHAKECLIENT::OCSPDirect_Set(DIOSTREAMTLS_OCSPDIRECTFETCHER fetcher, void* context)
{
  ocspdirectfetcher=fetcher; ocspdirectcontext=context;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS12HANDSHAKECLIENT_AUTHENTICATIONERROR DIOSTREAMTLS12HANDSHAKECLIENT::GetAuthenticationError()
* @brief      Get the last server authentication error
* @ingroup    DATAIO
*
* @return     DIOSTREAMTLS12HANDSHAKECLIENT_AUTHENTICATIONERROR : Authentication result.
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS12HANDSHAKECLIENT_AUTHENTICATIONERROR DIOSTREAMTLS12HANDSHAKECLIENT::GetAuthenticationError()
{
  return authenticationerror;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         CIPHERCERTIFICATEX509VALIDATOR_ERROR DIOSTREAMTLS12HANDSHAKECLIENT::GetCertificateValidationError()
* @brief      Get the detailed X.509 validation result
* @ingroup    DATAIO
*
* @return     CIPHERCERTIFICATEX509VALIDATOR_ERROR : X.509 validation result.
*
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERCERTIFICATEX509VALIDATOR_ERROR DIOSTREAMTLS12HANDSHAKECLIENT::GetCertificateValidationError()
{
  return certificatevalidationerror;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS12_MSG_CERTIFICATE* DIOSTREAMTLS12HANDSHAKECLIENT::GetServerCertificate()
* @brief      Get the retained server certificate message
* @ingroup    DATAIO
*
* @return     DIOSTREAMTLS12_MSG_CERTIFICATE* : Pointer to the requested object; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS12_MSG_CERTIFICATE* DIOSTREAMTLS12HANDSHAKECLIENT::GetServerCertificate()
{
  return servercertificate;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS12_MSG_SERVERKEYEXCHANGE_ECDHE* DIOSTREAMTLS12HANDSHAKECLIENT::GetServerKeyExchange()
* @brief      Get the retained server ServerKeyExchange message
* @ingroup    DATAIO
*
* @return     DIOSTREAMTLS12_MSG_SERVERKEYEXCHANGE_ECDHE* : Pointer to the requested object; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS12_MSG_SERVERKEYEXCHANGE_ECDHE* DIOSTREAMTLS12HANDSHAKECLIENT::GetServerKeyExchange()
{
  return serverkeyexchange;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS12HANDSHAKECLIENT::ClientHello_Create(XCHAR* servername, XBUFFER& clienthello, XBUFFER& records)
* @brief      Create the ClientHello and start the transcript
* @ingroup    DATAIO
*
* @param[in]  servername : Optional server name for the SNI extension.
* @param[out] clienthello : Complete encoded ClientHello.
* @param[out] records : Clear-text TLS records ready for transport.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS12HANDSHAKECLIENT::ClientHello_Create(XCHAR* servername, XBUFFER& clienthello, XBUFFER& records)
{
  DIOSTREAMTLS_MSG_FRAGMENT<DIOSTREAMTLS_MSG_HANDSHAKE_CLIENTHELLO> message;
  DIOSTREAMTLS_MSG_HANDSHAKE_CLIENTHELLO*                           body;
  XRAND*                                                            xrand;
  XBYTE                                                             random[DIOSTREAMTLS_MSG_RANDOM_SIZE];
  bool                                                               status;

  if(!isini || (state != DIOSTREAMTLS12HANDSHAKECLIENT_STATE_NONE) || (&clienthello == &records))
    {
      return SetError();
    }

  if(!authenticationconfigured || expectedservername.IsEmpty())
    {
      SetAuthenticationError(DIOSTREAMTLS12HANDSHAKECLIENT_AUTHENTICATIONERROR_CONFIGURATION);
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

  GEN_XFACTORY.DeleteRand(xrand);

  if(!status)
    {
      return SetError();
    }

  clientrandom.Delete();
  if(!clientrandom.Add(random, sizeof(random)))
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
  body->SetSessionIDLength(0);
  body->SetCompressionMethod(DIOSTREAMTLS_MSG_COMPRESS_METHOD_NULL);

  for(XDWORD c=0; c<ciphersuites.GetSize(); c++)
    {
      if(!body->GetCipherSuites()->Add(ciphersuites.Get(c))) return SetError();
    }

  // RFC 7507: this value is not a negotiable cipher suite. It is sent only when this TLS 1.2 ClientHello is a
  // retry after a failed TLS 1.3 attempt. A caller explicitly configured as TLS-1.2-only must not send it.
  if(checkdowngradesentinel &&
     !body->GetCipherSuites()->Add(DIOSTREAMTLS_MSG_CIPHER_FALLBACK_SCSV))
    {
      return SetError();
    }

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

  DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDGROUPS* offeredgroups;

  offeredgroups = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDGROUPS();
  if(!offeredgroups)
    {
      return SetError();
    }

  for(XDWORD c=0; c<supportedgroups.GetSize(); c++)
    {
      if(!offeredgroups->List_Add(supportedgroups.Get(c)))
        {
          GEN_DELETE offeredgroups;
          return SetError();
        }
    }

  if(!body->Extensions_Add(offeredgroups))
    {
      GEN_DELETE offeredgroups;
      return SetError();
    }

  DIOSTREAMTLS_MSG_EXTENSION_ECPOINTFORMATS* pointformats;

  pointformats = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_ECPOINTFORMATS();
  if(!pointformats)
    {
      return SetError();
    }

  if(!pointformats->List_Add(0) /* uncompressed */ || !body->Extensions_Add(pointformats))
    {
      GEN_DELETE pointformats;
      return SetError();
    }

  DIOSTREAMTLS_MSG_EXTENSION_SIGNATUREALGORITHMS* offeredschemes;

  offeredschemes = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_SIGNATUREALGORITHMS();
  if(!offeredschemes)
    {
      return SetError();
    }

  for(XDWORD c=0; c<signatureschemes.GetSize(); c++)
    {
      if(!offeredschemes->List_Add(signatureschemes.Get(c)))
        {
          GEN_DELETE offeredschemes;
          return SetError();
        }
    }

  if(!body->Extensions_Add(offeredschemes))
    {
      GEN_DELETE offeredschemes;
      return SetError();
    }

  DIOSTREAMTLS_MSG_EXTENSION_EMS* EMS = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_EMS();
  if(!EMS || !body->Extensions_Add(EMS))
    {
      if(EMS) GEN_DELETE EMS;
      return SetError();
    }

  DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN* renegotiationinfo = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN();
  if(!renegotiationinfo) return SetError();
  renegotiationinfo->SetType(DIOSTREAMTLS_MSG_EXTENSION_TYPE_RENEGOTIATIONINFO);
  if(!renegotiationinfo->GetData()->Add((XBYTE)0) || !body->Extensions_Add(renegotiationinfo))
    {
      GEN_DELETE renegotiationinfo;
      return SetError();
    }

  if(!message.SetToBuffer(clienthello, false) || !session.Transcript_Add(clienthello))
    {
      clienthello.Delete();
      return SetError();
    }

  // ClientHello always travels in the clear: no session/record is even initialized yet (the cipher suite,
  // and therefore the key schedule hash, is not known until ServerHello arrives).
  DIOSTREAMTLS_MSG_RECORDHEADER header;

  header.SetContenType(DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE);
  header.SetProtocolVersion(DIOSTREAMTLS_MSG_VERSION_TLS_1_2);
  header.SetLength((XWORD)clienthello.GetSize());

  if(!header.SetToBuffer(records, false) || !records.Add(clienthello))
    {
      clienthello.Delete();
      records.Delete();
      return SetError();
    }

  state = DIOSTREAMTLS12HANDSHAKECLIENT_STATE_WAIT_SERVERHELLO;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS12HANDSHAKECLIENT::RecordInput_Add(XBYTE* data, XDWORD size)
* @brief      Add received transport bytes to the TLS session
* @ingroup    DATAIO
*
* @param[in]  data : Bytes to add.
* @param[in]  size : Number of bytes to add.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS12HANDSHAKECLIENT::RecordInput_Add(XBYTE* data, XDWORD size)
{
  if(!isini)
    {
      return false;
    }

  return session.RecordInput_Add(data, size);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS12HANDSHAKECLIENT::RecordInput_Add(XBUFFER& data)
* @brief      Add a buffer of received transport bytes to the TLS session
* @ingroup    DATAIO
*
* @param[in]  data : Buffer to add.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS12HANDSHAKECLIENT::RecordInput_Add(XBUFFER& data)
{
  return RecordInput_Add(data.Get(), data.GetSize());
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS12HANDSHAKECLIENT::Process()
* @brief      Consume every complete record and handshake message currently buffered
* @ingroup    DATAIO
*
* @return     bool : true if the operation is successful (including "nothing more to do yet"); otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS12HANDSHAKECLIENT::Process()
{
  if(!isini || (state == DIOSTREAMTLS12HANDSHAKECLIENT_STATE_ERROR) || !session.IsIni())
    {
      // Before the cipher suite is known (state == WAIT_SERVERHELLO and the session not yet initialized),
      // ServerHello itself is still a plain handshake record: peel it off by hand, once, right here.
      if(isini && (state == DIOSTREAMTLS12HANDSHAKECLIENT_STATE_WAIT_SERVERHELLO) && !session.IsIni())
        {
          XBUFFER*                      recordinput = session.GetRecordInput();
          DIOSTREAMTLS_MSG_RECORDHEADER header;

          while(true)
            {
              if(recordinput->GetSize() < DIOSTREAMTLS_MSG_RECORDHEADER_SIZE) return true;
              if(!header.Peek(*recordinput))
                {
                  return SetError();
                }
              if(header.GetLength() > DIOSTREAMTLS12RECORD_MAXCIPHERSIZE) return SetError();
              if((XDWORD)recordinput->GetSize() < ((XDWORD)DIOSTREAMTLS_MSG_RECORDHEADER_SIZE + header.GetLength())) return true;

              XBUFFER onerecord;
              if(!DIOSTREAMTLS12RECORD::Record_Extract(*recordinput, onerecord))
                {
                  return SetError();
                }

              if(header.GetContenType() != DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE)
                {
                  if(header.GetContenType() == DIOSTREAMTLS_MSG_CONTENTTYPE_ALERT)
                    {
                      DIOSTREAMTLS_MSG_ALERT alert;
                      XBUFFER                alertbody;

                      alertbody.Add(&onerecord.Get()[DIOSTREAMTLS_MSG_RECORDHEADER_SIZE], header.GetLength());

                      if(alert.GetFromBuffer(alertbody, false) && alertbody.IsEmpty())
                        {
                          if((alert.GetLevel() == DIOSTREAMTLS_ALERT_LEVEL_FATAL) &&
                             (alert.GetDescription() == DIOSTREAMTLS_ALERT_DESCRIPTION_INAPPROPRIATE_FALLBACK))
                            {
                              SetAuthenticationError(DIOSTREAMTLS12HANDSHAKECLIENT_AUTHENTICATIONERROR_DOWNGRADEDETECTED);
                            }

                          if((alert.GetLevel() == DIOSTREAMTLS_ALERT_LEVEL_FATAL) &&
                             ((alert.GetDescription() == DIOSTREAMTLS_ALERT_DESCRIPTION_HANDSHAKE_FAILURE) ||
                              (alert.GetDescription() == DIOSTREAMTLS_ALERT_DESCRIPTION_INSUFFICIENT_SECURITY)))
                            {
                              algorithmrejected = true;
                            }
                        }
                    }
                   else
                    {
                    }

                  return SetError();
                }

              XBUFFER plain;
              plain.Add(&onerecord.Get()[DIOSTREAMTLS_MSG_RECORDHEADER_SIZE], header.GetLength());

              if(!session.HandshakeInput_Add(plain)) return SetError();

              while(true)
                {
                  XBUFFER handshake;

                  DIOSTREAMTLS12SESSION_RESULT handshakeresult = session.Handshake_Extract(handshake);
                  if(handshakeresult == DIOSTREAMTLS12SESSION_RESULT_INCOMPLETE) break;
                  if(handshakeresult == DIOSTREAMTLS12SESSION_RESULT_ERROR) return SetError();

                  if(!Handshake_Process(handshake)) return false;

                  if(session.IsIni()) break;   // ServerHello just switched us into the real session; hand off below.
                }

              if(session.IsIni()) break;
            }
        }
       else
        {
          return false;
        }
    }

  if(!session.IsIni())
    {
      return true;
    }

  while(true)
    {
      DIOSTREAMTLS_CONTENTTYPE         contenttype = (DIOSTREAMTLS_CONTENTTYPE)0;
      DIOSTREAMTLS12SESSION_RESULT     result;
      XBUFFER                          plain;

      result = session.Record_Extract(contenttype, plain);

      if(result == DIOSTREAMTLS12SESSION_RESULT_INCOMPLETE)
        {
          return true;
        }

      if(result == DIOSTREAMTLS12SESSION_RESULT_ERROR)
        {
          return SetError();
        }

      switch(contenttype)
        {
          case DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE          : if(!session.HandshakeInput_Add(plain)) return SetError();
                                                                break;

          case DIOSTREAMTLS_MSG_CONTENTTYPE_CHANGE_CIPHER_SPEC : if((state != DIOSTREAMTLS12HANDSHAKECLIENT_STATE_WAIT_CHANGECIPHERSPEC) ||
                                                                      (plain.GetSize() != 1) || (plain.GetByte(0) != 1))
                                                                    {
                                                                      return SetError();
                                                                    }

                                                                  // RFC 5246 section 7.4.9: a peer Finished is valid only after its
                                                                  // ChangeCipherSpec. A duplicate CCS is rejected because the state moves on.
                                                                  state = DIOSTREAMTLS12HANDSHAKECLIENT_STATE_WAIT_FINISHED;
                                                                break;

          case DIOSTREAMTLS_MSG_CONTENTTYPE_ALERT              : { DIOSTREAMTLS_MSG_ALERT alert;

                                                                  if(!alert.GetFromBuffer(plain, false) || !plain.IsEmpty()) return SetError();


                                                                  // The two descriptions a real TLS 1.2 server sends when nothing allowed by the
                                                                  // configured common cryptographic policy is mutually usable.
                                                                  if(alert.GetLevel() == DIOSTREAMTLS_ALERT_LEVEL_FATAL)
                                                                    {
                                                                      if((alert.GetDescription() == DIOSTREAMTLS_ALERT_DESCRIPTION_HANDSHAKE_FAILURE) ||
                                                                         (alert.GetDescription() == DIOSTREAMTLS_ALERT_DESCRIPTION_INSUFFICIENT_SECURITY))
                                                                        {
                                                                          algorithmrejected = true;
                                                                        }

                                                                      if(alert.GetDescription() == DIOSTREAMTLS_ALERT_DESCRIPTION_INAPPROPRIATE_FALLBACK)
                                                                        {
                                                                          SetAuthenticationError(DIOSTREAMTLS12HANDSHAKECLIENT_AUTHENTICATIONERROR_DOWNGRADEDETECTED);
                                                                        }
                                                                    }

                                                                  return SetError();
                                                                }

                                                       default :
                                                                 return SetError();
        }

      while(true)
        {
          XBUFFER handshake;

          if(session.GetHandshakeInput()->GetSize() < DIOSTREAMTLS_MSG_HANDSHAKEHEADER_SIZE) break;

          DIOSTREAMTLS12SESSION_RESULT handshakeresult = session.Handshake_Extract(handshake);

          if(handshakeresult == DIOSTREAMTLS12SESSION_RESULT_INCOMPLETE) break;
          if(handshakeresult == DIOSTREAMTLS12SESSION_RESULT_ERROR) return SetError();

          if(!Handshake_Process(handshake)) return false;
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS12HANDSHAKECLIENT::Handshake_Process(XBUFFER& message)
* @brief      Dispatch one exact handshake message according to the current state
* @note       INTERNAL
* @ingroup    DATAIO
*
* @param[in]  message : Complete handshake message, including its header.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS12HANDSHAKECLIENT::Handshake_Process(XBUFFER& message)
{
  DIOSTREAMTLS_MSG_HANDSHAKE genericmessage;
  XBUFFER                    workbuffer;

  if(!isini || (state == DIOSTREAMTLS12HANDSHAKECLIENT_STATE_ERROR))
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
      case DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_SERVER_HELLO         : return ServerHello_Process(message);
      case DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_CERTIFICATE          : return Certificate_Process(message);
      case DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_SERVER_KEY_EXCHANGE  : return ServerKeyExchange_Process(message);
      case DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_SERVER_HELLO_DONE    : return ServerHelloDone_Process(message);
      case DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_FINISHED             : return Finished_Process(message);
                                                            default   :
                                                                       return SetError();
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS12HANDSHAKECLIENT::ServerHello_Process(XBUFFER& message)
* @brief      Decode ServerHello, select the cipher suite and initialize the real session
* @note       INTERNAL
* @ingroup    DATAIO
*
* @param[in]  message : Complete handshake message, including its header.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS12HANDSHAKECLIENT::ServerHello_Process(XBUFFER& message)
{
  DIOSTREAMTLS_MSG_FRAGMENT<DIOSTREAMTLS_MSG_HANDSHAKE_SERVERHELLO> serverhello;
  XBUFFER                                                          workbuffer;
  XBUFFER                                                          pendinginput;

  if(state != DIOSTREAMTLS12HANDSHAKECLIENT_STATE_WAIT_SERVERHELLO)
    {
      return SetError();
    }

  workbuffer.Add(message);

  if(!serverhello.GetFromBuffer(workbuffer, false) || !workbuffer.IsEmpty() ||
     (serverhello.GetMsgType() != DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_SERVER_HELLO) ||
     (serverhello.GetBody()->GetLegacyVersion() != DIOSTREAMTLS_MSG_VERSION_TLS_1_2) ||
     (serverhello.GetBody()->GetCompressionMethod() != DIOSTREAMTLS_MSG_COMPRESS_METHOD_NULL) ||
     !CipherSuite_IsOffered(serverhello.GetBody()->GetCipherSuite()))
    {
      return SetError();
    }

  bool hasEMS = false;
  bool hassecurerenegotiation = false;
  XVECTOR<DIOSTREAMTLS_MSG_EXTENSION*>* serverextensions = serverhello.GetBody()->Extensions_GetAll();

  for(XDWORD c=0; c<serverextensions->GetSize(); c++)
    {
      DIOSTREAMTLS_MSG_EXTENSION* extension = serverextensions->Get(c);
      if(!extension) return SetError();

      if(extension->GetType() == DIOSTREAMTLS_MSG_EXTENSION_TYPE_EMS) hasEMS = true;

      if(extension->GetType() == DIOSTREAMTLS_MSG_EXTENSION_TYPE_RENEGOTIATIONINFO)
        {
          DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN* unknown = (DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN*)extension;
          if((unknown->GetData()->GetSize()!=1) || unknown->GetData()->GetByte(0)) return SetError();
          hassecurerenegotiation = true;
        }
    }

  if(!hasEMS || !hassecurerenegotiation) return SetError();

  serverrandom.Delete();
  if(!serverrandom.Add(serverhello.GetBody()->GetRandom(), DIOSTREAMTLS_MSG_RANDOM_SIZE))
    {
      return SetError();
    }

  // RFC 8446 4.1.3 downgrade protection -- only checked for a dual-version fallback attempt (checkdowngradesentinel,
  // set via Ini(), true only in that case; see its declaration for why a deliberately configured TLS-1.2-only
  // connection must NOT check this). ANY TLS 1.3-capable server MUST stamp the last 8 bytes of ServerHello.random
  // with a fixed value whenever it settles for 1.2 or below, precisely so a client that tried TLS 1.3 first (as
  // this one just did, over a separate earlier attempt) can tell an on-path attacker forced this negotiation
  // down from one where the peer genuinely does not speak TLS 1.3.
  if(checkdowngradesentinel &&
     (serverrandom.GetSize() == DIOSTREAMTLS_MSG_RANDOM_SIZE) &&
     ((!memcmp(serverrandom.Get() + DIOSTREAMTLS_MSG_RANDOM_SIZE - 8, "\x44\x4F\x57\x4E\x47\x52\x44\x01", 8)) ||
      (!memcmp(serverrandom.Get() + DIOSTREAMTLS_MSG_RANDOM_SIZE - 8, "\x44\x4F\x57\x4E\x47\x52\x44\x00", 8))))
    {
      SetAuthenticationError(DIOSTREAMTLS12HANDSHAKECLIENT_AUTHENTICATIONERROR_DOWNGRADEDETECTED);
      return SetError();
    }

  // Preserve whatever bytes already arrived after ServerHello (Certificate, and often more, land in the same
  // TCP segment): they were parked in the pre-session record accumulator and must move into the real one.
  pendinginput.Add(*session.GetRecordInput());
  session.GetRecordInput()->Delete();

  if(!session.Ini(serverhello.GetBody()->GetCipherSuite(), DIOSTREAMTLSKEYSCHEDULE_ROLE_CLIENT) ||
     !session.Transcript_Add(message))
    {
      return SetError();
    }

  if(!pendinginput.IsEmpty() && !session.RecordInput_Add(pendinginput))
    {
      return SetError();
    }

  state = DIOSTREAMTLS12HANDSHAKECLIENT_STATE_WAIT_CERTIFICATE;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS12HANDSHAKECLIENT::CertificateChain_CompleteViaAIA(XVECTOR<XBUFFER*>& certificatechain)
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
bool DIOSTREAMTLS12HANDSHAKECLIENT::CertificateChain_CompleteViaAIA(XVECTOR<XBUFFER*>& certificatechain)
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
* @fn         bool DIOSTREAMTLS12HANDSHAKECLIENT::Certificate_Process(XBUFFER& message)
* @brief      Decode the server Certificate and validate the chain
* @note       INTERNAL
* @ingroup    DATAIO
*
* @param[in]  message : Complete handshake message, including its header.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS12HANDSHAKECLIENT::Certificate_Process(XBUFFER& message)
{
  DIOSTREAMTLS_MSG_FRAGMENT<DIOSTREAMTLS12_MSG_CERTIFICATE> certificate;
  XBUFFER                                                  workbuffer;
  XVECTOR<XBUFFER*>                                         certificatechain;

  if(state != DIOSTREAMTLS12HANDSHAKECLIENT_STATE_WAIT_CERTIFICATE)
    {
      return SetError();
    }

  workbuffer.Add(message);

  if(!certificate.GetFromBuffer(workbuffer, false) || !workbuffer.IsEmpty() ||
     (certificate.GetMsgType() != DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_CERTIFICATE) ||
     certificate.GetBody()->CertificateList_GetAll()->IsEmpty())
    {
      return SetError();
    }

  if(!authenticationconfigured)
    {
      SetAuthenticationError(DIOSTREAMTLS12HANDSHAKECLIENT_AUTHENTICATIONERROR_CONFIGURATION);
      return SetError();
    }

  for(XDWORD c=0; c<certificate.GetBody()->CertificateList_GetAll()->GetSize(); c++)
    {
      XBUFFER* der = certificate.GetBody()->CertificateList_GetAll()->Get(c);
      if(!der || !certificatechain.Add(der))
        {
          SetAuthenticationError(DIOSTREAMTLS12HANDSHAKECLIENT_AUTHENTICATIONERROR_CERTIFICATE);
          return SetError();
        }
    }

  if(!certificatevalidator.ValidateAllPaths(&certificatechain, &trustedroots, expectedservername.Get(),
                                             hasvalidationdatetime?&validationdatetime:NULL) &&
     !CertificateChain_CompleteViaAIA(certificatechain))
    {
      certificatevalidationerror = certificatevalidator.GetError();


      SetAuthenticationError(DIOSTREAMTLS12HANDSHAKECLIENT_AUTHENTICATIONERROR_CERTIFICATE);
      return SetError();
    }

  certificatevalidationerror = CIPHERCERTIFICATEX509VALIDATOR_ERROR_NONE;

  const bool revocationenabled = !config || config->GetRevocationPolicy()!=DIOSTREAMTLS_REVOCATIONPOLICY_OFF;
  if(revocationenabled && revocationlists && !revocationlists->IsEmpty())
    {
      XVECTOR<CIPHERCERTIFICATEX509*>* chain=certificatevalidator.GetCertificateChain();
      if(!chain || chain->GetSize()<2) return SetError();
      for(XDWORD certindex=0; certindex+1<chain->GetSize(); certindex++)
        {
          bool valid=false;
          for(XDWORD c=0;c<revocationlists->GetSize();c++)
            {
              XBUFFER* CRL=revocationlists->Get(c);
              if(!CRL || (config && CRL->GetSize() > config->GetMemoryPolicy()->GetMaximumCRLSize())) continue;
              CIPHERCERTIFICATEX509REVOCATION_RESULT result=CIPHERCERTIFICATEX509REVOCATION::ValidateCRL((*CRL), (*chain->Get(certindex)), (*chain->Get(certindex+1)));
              if(result==CIPHERCERTIFICATEX509REVOCATION_RESULT_REVOKED)
                { certificatevalidationerror=CIPHERCERTIFICATEX509VALIDATOR_ERROR_REVOKED; SetAuthenticationError(DIOSTREAMTLS12HANDSHAKECLIENT_AUTHENTICATIONERROR_CERTIFICATE); return SetError(); }
              if(result==CIPHERCERTIFICATEX509REVOCATION_RESULT_GOOD) valid=true;
            }
          if(!valid && (!config || config->GetRevocationPolicy()!=DIOSTREAMTLS_REVOCATIONPOLICY_SOFT_FAIL))
            { certificatevalidationerror=CIPHERCERTIFICATEX509VALIDATOR_ERROR_REVOCATIONUNKNOWN; SetAuthenticationError(DIOSTREAMTLS12HANDSHAKECLIENT_AUTHENTICATIONERROR_CERTIFICATE); return SetError(); }
        }
    }


  if(revocationenabled && ocspdirectfetcher)
    {
      XVECTOR<CIPHERCERTIFICATEX509*>* chain=certificatevalidator.GetCertificateChain();
      if(!chain || chain->GetSize()<2) return SetError();
      for(XDWORD certindex=0; certindex+1<chain->GetSize(); certindex++)
        {
          XBUFFER response;
          CIPHERCERTIFICATEX509REVOCATION_RESULT result=CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID;
          if(chain->Get(certindex)->HasOCSPURL() &&
             ocspdirectfetcher((*chain->Get(certindex)->GetOCSPURL()), (*chain->Get(certindex)), (*chain->Get(certindex+1)), response, ocspdirectcontext) &&
             (!config || response.GetSize() <= config->GetMemoryPolicy()->GetMaximumOCSPResponseSize()))
            result=CIPHERCERTIFICATEX509REVOCATION::ValidateOCSP(response, (*chain->Get(certindex)), (*chain->Get(certindex+1)));
          if(result==CIPHERCERTIFICATEX509REVOCATION_RESULT_REVOKED ||
             (!config || config->GetRevocationPolicy()!=DIOSTREAMTLS_REVOCATIONPOLICY_SOFT_FAIL) &&
             result!=CIPHERCERTIFICATEX509REVOCATION_RESULT_GOOD)
            { certificatevalidationerror=(result==CIPHERCERTIFICATEX509REVOCATION_RESULT_REVOKED)?
                CIPHERCERTIFICATEX509VALIDATOR_ERROR_REVOKED:CIPHERCERTIFICATEX509VALIDATOR_ERROR_REVOCATIONUNKNOWN;
              SetAuthenticationError(DIOSTREAMTLS12HANDSHAKECLIENT_AUTHENTICATIONERROR_CERTIFICATE); return SetError(); }
        }
    }
  if(config && config->GetRevocationPolicy()==DIOSTREAMTLS_REVOCATIONPOLICY_MUST_STAPLE && !ocspdirectfetcher)
    {
      certificatevalidationerror=CIPHERCERTIFICATEX509VALIDATOR_ERROR_REVOCATIONUNKNOWN;
      SetAuthenticationError(DIOSTREAMTLS12HANDSHAKECLIENT_AUTHENTICATIONERROR_CERTIFICATE);
      return SetError();
    }

  DIOSTREAMTLS12_MSG_CERTIFICATE* decodedcertificate = GEN_NEW DIOSTREAMTLS12_MSG_CERTIFICATE();
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
     !session.Transcript_Add(message))
    {
      GEN_DELETE decodedcertificate;
      return SetError();
    }

  if(servercertificate) GEN_DELETE servercertificate;
  servercertificate = decodedcertificate;
  state             = DIOSTREAMTLS12HANDSHAKECLIENT_STATE_WAIT_SERVERKEYEXCHANGE;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS12HANDSHAKECLIENT::ServerKeyExchange_Process(XBUFFER& message)
* @brief      Decode, verify, and act on the server ServerKeyExchange (ECDHE, signed)
* @note       INTERNAL. Also generates the client's own ephemeral key pair and computes the pre_master_secret,
*             since both need the negotiated curve, which is only known from this message.
* @ingroup    DATAIO
*
* @param[in]  message : Complete handshake message, including its header.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS12HANDSHAKECLIENT::ServerKeyExchange_Process(XBUFFER& message)
{
  DIOSTREAMTLS_MSG_FRAGMENT<DIOSTREAMTLS12_MSG_SERVERKEYEXCHANGE_ECDHE> ske;
  XBUFFER                                                              workbuffer;
  XBUFFER                                                              params;
  XBUFFER                                                              signedcontent;

  if(state != DIOSTREAMTLS12HANDSHAKECLIENT_STATE_WAIT_SERVERKEYEXCHANGE)
    {
      return SetError();
    }

  workbuffer.Add(message);

  if(!ske.GetFromBuffer(workbuffer, false) || !workbuffer.IsEmpty() ||
     (ske.GetMsgType() != DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_SERVER_KEY_EXCHANGE) ||
     !SupportedGroup_IsOffered(ske.GetBody()->GetNamedCurve()) ||
     !SignatureScheme_IsOffered(ske.GetBody()->GetSignatureAlgorithm()) ||
     !ske.GetBody()->GetSignedParams(params))
    {
      SetAuthenticationError(DIOSTREAMTLS12HANDSHAKECLIENT_AUTHENTICATIONERROR_SERVERKEYEXCHANGE);
      return SetError();
    }

  CIPHERCERTIFICATEX509* leaf = certificatevalidator.GetLeafCertificate();

  if(!authenticationconfigured || !leaf || !leaf->GetPublicCipherKey() ||
     !DIOSTREAMTLSSIGNATURE::IsSupported(ske.GetBody()->GetSignatureAlgorithm(), leaf))
    {
      SetAuthenticationError(DIOSTREAMTLS12HANDSHAKECLIENT_AUTHENTICATIONERROR_SERVERKEYEXCHANGE);
      return SetError();
    }

  // RFC 5246 section 7.4.3: signed_params = client_random + server_random + ServerECDHParams.
  if(!signedcontent.Add(clientrandom) || !signedcontent.Add(serverrandom) || !signedcontent.Add(params))
    {
      SetAuthenticationError(DIOSTREAMTLS12HANDSHAKECLIENT_AUTHENTICATIONERROR_SERVERKEYEXCHANGE);
      return SetError();
    }

  if(!DIOSTREAMTLSSIGNATURE::Verify(ske.GetBody()->GetSignatureAlgorithm(), leaf->GetPublicCipherKey(),
                                    signedcontent, *ske.GetBody()->GetSignature()))
    {

      SetAuthenticationError(DIOSTREAMTLS12HANDSHAKECLIENT_AUTHENTICATIONERROR_SERVERKEYEXCHANGE);
      return SetError();
    }

  servergroup = ske.GetBody()->GetNamedCurve();

  if(!session.KeyExchange_Generate(servergroup, clientkeyshare) ||
     !session.KeyExchange_SharedSecret(servergroup, *ske.GetBody()->GetPublicKey(), premastersecret))
    {
      return SetError();
    }

  DIOSTREAMTLS12_MSG_SERVERKEYEXCHANGE_ECDHE* decoded = GEN_NEW DIOSTREAMTLS12_MSG_SERVERKEYEXCHANGE_ECDHE();
  if(!decoded)
    {
      return SetError();
    }

  XBUFFER bodybuffer;
  if(!ske.GetBody()->SetToBuffer(bodybuffer, false))
    {
      GEN_DELETE decoded;
      return SetError();
    }

  if(!decoded->GetFromBuffer(bodybuffer, false) || !bodybuffer.IsEmpty() || !session.Transcript_Add(message))
    {
      GEN_DELETE decoded;
      return SetError();
    }

  if(serverkeyexchange) GEN_DELETE serverkeyexchange;
  serverkeyexchange = decoded;

  serverauthenticated = true;
  authenticationerror = DIOSTREAMTLS12HANDSHAKECLIENT_AUTHENTICATIONERROR_NONE;

  state = DIOSTREAMTLS12HANDSHAKECLIENT_STATE_WAIT_SERVERHELLODONE;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS12HANDSHAKECLIENT::ServerHelloDone_Process(XBUFFER& message)
* @brief      Decode ServerHelloDone and make the client's response flight available
* @note       INTERNAL
* @ingroup    DATAIO
*
* @param[in]  message : Complete handshake message, including its header.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS12HANDSHAKECLIENT::ServerHelloDone_Process(XBUFFER& message)
{
  DIOSTREAMTLS_MSG_FRAGMENT<DIOSTREAMTLS12_MSG_SERVERHELLODONE> shd;
  XBUFFER                                                       workbuffer;

  if((state != DIOSTREAMTLS12HANDSHAKECLIENT_STATE_WAIT_SERVERHELLODONE) ||
     !serverauthenticated)
    {
      return SetError();
    }

  workbuffer.Add(message);

  if(!shd.GetFromBuffer(workbuffer, false) || !workbuffer.IsEmpty() ||
     (shd.GetMsgType() != DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_SERVER_HELLO_DONE) ||
     !session.Transcript_Add(message))
    {
      return SetError();
    }

  state = DIOSTREAMTLS12HANDSHAKECLIENT_STATE_READY_CLIENTFLIGHT;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS12HANDSHAKECLIENT::ClientFlight_Create(XBUFFER& records)
* @brief      Build ClientKeyExchange (clear), ChangeCipherSpec (clear) and Finished (encrypted)
* @ingroup    DATAIO
*
* @param[out] records : Complete TLS records ready for transport.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS12HANDSHAKECLIENT::ClientFlight_Create(XBUFFER& records)
{
  DIOSTREAMTLS_MSG_FRAGMENT<DIOSTREAMTLS12_MSG_CLIENTKEYEXCHANGE_ECDHE> cke;
  DIOSTREAMTLS_MSG_FRAGMENT<DIOSTREAMTLS_MSG_HANDSHAKE_FINISHED>        finished;
  XBUFFER                                                              ckebuffer;
  XBUFFER                                                              transcripthash;
  XBUFFER                                                              verifydata;
  XBUFFER                                                              finishedbuffer;

  if(!isini || (state != DIOSTREAMTLS12HANDSHAKECLIENT_STATE_READY_CLIENTFLIGHT))
    {
      return SetError();
    }

  records.Delete();

  // 1) ClientKeyExchange, still in the clear: no key has been activated yet.
  cke.SetMsgType(DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_CLIENT_KEY_EXCHANGE);

  if(!cke.GetBody()->SetPublicKey(clientkeyshare) ||
     !cke.SetToBuffer(ckebuffer, false) ||
     !session.Transcript_Add(ckebuffer) ||
     !session.GetRecord()->Protect(DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE, ckebuffer, records))
    {
      records.Delete();
      return SetError();
    }

  // 2) ChangeCipherSpec, still in the clear (isprotected[LOCAL] is still false): the single byte 0x01.
  {
    XBYTE ccsbyte = 0x01;

    if(!session.GetRecord()->Protect(DIOSTREAMTLS_MSG_CONTENTTYPE_CHANGE_CIPHER_SPEC, &ccsbyte, 1, records))
      {
        records.Delete();
        return SetError();
      }
  }

  // 3) NOW derive master_secret and key_block, and activate both record directions.
  if(!session.TranscriptHash(transcripthash) ||
     !session.Keys_Activate(premastersecret, clientrandom, serverrandom, &transcripthash))
    {
      records.Delete();
      return SetError();
    }

  premastersecret.FillBuffer(0);
  premastersecret.SecureDelete();

  // 4) Finished, the first record actually encrypted with the just-activated LOCAL key (sequence 0).
  if(!session.TranscriptHash(transcripthash) ||
     !session.GetKeySchedule()->VerifyData_Create(true, transcripthash, verifydata))
    {
      records.Delete();
      return SetError();
    }

  finished.SetMsgType(DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_FINISHED);

  if(!finished.GetBody()->GetVerifyData()->Add(verifydata) ||
     !finished.SetToBuffer(finishedbuffer, false) ||
     !session.Transcript_Add(finishedbuffer) ||
     !session.GetRecord()->Protect(DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE, finishedbuffer, records))
    {
      records.Delete();
      return SetError();
    }

  state = DIOSTREAMTLS12HANDSHAKECLIENT_STATE_WAIT_CHANGECIPHERSPEC;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS12HANDSHAKECLIENT::Finished_Process(XBUFFER& message)
* @brief      Verify the server Finished and complete the handshake
* @note       INTERNAL
* @ingroup    DATAIO
*
* @param[in]  message : Complete handshake message, including its header.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS12HANDSHAKECLIENT::Finished_Process(XBUFFER& message)
{
  DIOSTREAMTLS_MSG_FRAGMENT<DIOSTREAMTLS_MSG_HANDSHAKE_FINISHED> finished;
  XBUFFER                                                       workbuffer;
  XBUFFER                                                       transcripthash;
  XBUFFER                                                       expectedverifydata;

  if(state != DIOSTREAMTLS12HANDSHAKECLIENT_STATE_WAIT_FINISHED)
    {
      return SetError();
    }

  workbuffer.Add(message);

  if(!finished.GetFromBuffer(workbuffer, false) || !workbuffer.IsEmpty() ||
     (finished.GetMsgType() != DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_FINISHED) ||
     (finished.GetBody()->GetVerifyData()->GetSize() != DIOSTREAMTLS12KEYSCHEDULE_VERIFYDATASIZE))
    {
      return SetError();
    }

  if(!session.TranscriptHash(transcripthash) ||
     !session.GetKeySchedule()->VerifyData_Create(false, transcripthash, expectedverifydata) ||
     (expectedverifydata.GetSize() != finished.GetBody()->GetVerifyData()->GetSize()) ||
     !CIPHER::CompareConstantTime(expectedverifydata.Get(), finished.GetBody()->GetVerifyData()->Get(), expectedverifydata.GetSize()))
    {
      return SetError();
    }

  if(!session.Transcript_Add(message))
    {
      return SetError();
    }

  state = DIOSTREAMTLS12HANDSHAKECLIENT_STATE_HANDSHAKE_COMPLETED;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS12HANDSHAKECLIENT::SetError()
* @brief      Move the handshake to its terminal error state
* @note       INTERNAL
* @ingroup    DATAIO
*
* @return     bool : Always false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS12HANDSHAKECLIENT::SetError()
{
  state = DIOSTREAMTLS12HANDSHAKECLIENT_STATE_ERROR;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS12HANDSHAKECLIENT::SetAuthenticationError(DIOSTREAMTLS12HANDSHAKECLIENT_AUTHENTICATIONERROR error)
* @brief      Record a server authentication error
* @note       INTERNAL
* @ingroup    DATAIO
*
* @param[in]  error : Authentication error to record.
*
* @return     bool : Always false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS12HANDSHAKECLIENT::SetAuthenticationError(DIOSTREAMTLS12HANDSHAKECLIENT_AUTHENTICATIONERROR error)
{
  authenticationerror = error;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS12HANDSHAKECLIENT::CipherSuite_IsOffered(XWORD ciphersuite)
* @brief      Check whether a cipher suite was offered in ClientHello
* @note       INTERNAL
* @ingroup    DATAIO
*
* @param[in]  ciphersuite : Cipher suite to check.
*
* @return     bool : true if the condition is met; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS12HANDSHAKECLIENT::CipherSuite_IsOffered(XWORD ciphersuite)
{
  for(XDWORD c=0; c<ciphersuites.GetSize(); c++)
    {
      if(ciphersuites.Get(c) == ciphersuite) return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS12HANDSHAKECLIENT::SupportedGroup_IsOffered(XWORD supportedgroup)
* @brief      Check whether a supported group was offered in ClientHello
* @note       INTERNAL
* @ingroup    DATAIO
*
* @param[in]  supportedgroup : Supported group to check.
*
* @return     bool : true if the condition is met; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS12HANDSHAKECLIENT::SupportedGroup_IsOffered(XWORD supportedgroup)
{
  for(XDWORD c=0; c<supportedgroups.GetSize(); c++)
    {
      if(supportedgroups.Get(c) == supportedgroup) return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS12HANDSHAKECLIENT::SignatureScheme_IsOffered(XWORD signaturescheme)
* @brief      Check whether a signature scheme was offered in ClientHello
* @note       INTERNAL
* @ingroup    DATAIO
*
* @param[in]  signaturescheme : Signature scheme to check.
*
* @return     bool : true if the condition is met; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS12HANDSHAKECLIENT::SignatureScheme_IsOffered(XWORD signaturescheme)
{
  for(XDWORD c=0; c<signatureschemes.GetSize(); c++)
    {
      if(signatureschemes.Get(c) == signaturescheme) return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS12HANDSHAKECLIENT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS12HANDSHAKECLIENT::Clean()
{
  config                           = NULL;
  state                       = DIOSTREAMTLS12HANDSHAKECLIENT_STATE_NONE;
  isini                       = false;
  checkdowngradesentinel      = false;
  algorithmrejected           = false;
  authenticationconfigured    = false;
  serverauthenticated         = false;
  authenticationerror         = DIOSTREAMTLS12HANDSHAKECLIENT_AUTHENTICATIONERROR_NONE;
  certificatevalidationerror  = CIPHERCERTIFICATEX509VALIDATOR_ERROR_NONE;
  hasvalidationdatetime       = false;
  servergroup                 = 0;
  aiafetchactive               = true;
  aiafetchtimeout              = DIOSTREAMTLSAIAFETCHER_TIMEOUT;
  revocationlists              = NULL;
  ocspdirectfetcher            = NULL;
  ocspdirectcontext            = NULL;
}
