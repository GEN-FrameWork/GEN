/**-------------------------------------------------------------------------------------------------------------------
*
* @file       DIOStreamTLS13HandshakeServer.cpp
*
* @class      DIOSTREAMTLS13HANDSHAKESERVER
* @brief      Data Input/Output Stream TLS 1.3 Server Handshake class
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

#include "DIOStreamTLS13HandshakeServer.h"

#include "DIOStreamTLSConfig.h"
#include "DIOStreamTLSSignature.h"
#include "DIOStreamTLSMessagesHandShakeClientHello.h"
#include "DIOStreamTLSMessagesHandShakeServerHello.h"

#include "CipherCertificateX509.h"
#include "CipherCertificateX509Revocation.h"
#include "CipherKey.h"

#include "XRand.h"
#include "XFactory.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/


static XBYTE DIOSTREAMTLS13_HANDSHAKESERVER_HELLORETRYREQUEST_RANDOM[DIOSTREAMTLS_MSG_HELLORETRYREQUEST_RANDOM_SIZE] =
{
  0xCF, 0x21, 0xAD, 0x74, 0xE5, 0x9A, 0x61, 0x11,
  0xBE, 0x1D, 0x8C, 0x02, 0x1E, 0x65, 0xB8, 0x91,
  0xC2, 0xA2, 0x11, 0x16, 0x7A, 0xBB, 0x8C, 0x5E,
  0x07, 0x9E, 0x09, 0xE2, 0xC8, 0xA8, 0x33, 0x9C
};


static const XWORD DIOSTREAMTLS13_HANDSHAKESERVER_EXTENSION_PADDING    = 0x0015;
static const XWORD DIOSTREAMTLS13_HANDSHAKESERVER_EXTENSION_EARLYDATA  = 0x002A;


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS13_HANDSHAKESERVER_ServerNameMatch(XSTRING& pattern, XCHAR* servername)
* @brief      Server name match
* @ingroup    DATAIO
* 
* @param[in]  pattern : Pattern value.
* @param[in]  servername : Pointer to servername.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool DIOSTREAMTLS13_HANDSHAKESERVER_ServerNameMatch(XSTRING& pattern, XCHAR* servername)
{
  XSTRING hostname;
  int     dot;

  if(!servername || !servername[0] || pattern.IsEmpty()) return false;

  hostname = servername;
  if(hostname.Character_GetLast() == _C('.')) hostname.DeleteLastCharacter();
  if(pattern.Character_GetLast()  == _C('.')) pattern.DeleteLastCharacter();

  if(!pattern.Compare(hostname, true)) return true;

  if((pattern.GetSize() < 3) || (pattern[0] != _C('*')) || (pattern[1] != _C('.')) ||
     (pattern.FindCharacter(_C('*'), 1) >= 0)) return false;

  dot = hostname.FindCharacter(_C('.'));
  if(dot <= 0) return false;

  XSTRING hostnamesuffix(&hostname.Get()[dot]);
  XSTRING patternsuffix(&pattern.Get()[1]);

  return !hostnamesuffix.Compare(patternsuffix, true);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS13_HANDSHAKESERVER_SignatureSchemeOffered(XVECTOR<XWORD>& offered, XWORD scheme)
* @brief      Signature scheme offered
* @ingroup    DATAIO
* 
* @param[in]  offered : Offered value.
* @param[in]  scheme : Scheme value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool DIOSTREAMTLS13_HANDSHAKESERVER_SignatureSchemeOffered(XVECTOR<XWORD>& offered, XWORD scheme)
{
  for(XDWORD c=0; c<offered.GetSize(); c++)
    {
      if(offered.Get(c) == scheme) return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORD DIOSTREAMTLS13_HANDSHAKESERVER_CertificateSignatureScheme(CIPHERCERTIFICATEX509& certificate)
* @brief      Certificate signature scheme
* @ingroup    DATAIO
* 
* @param[in]  certificate : Certificate value.
* 
* @return     XWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static XWORD DIOSTREAMTLS13_HANDSHAKESERVER_CertificateSignatureScheme(CIPHERCERTIFICATEX509& certificate)
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
* @fn         bool DIOSTREAMTLS13_HANDSHAKESERVER_CertificateChainCompatible(XVECTOR<XBUFFER*>* chain, XVECTOR<XWORD>& offered)
* @brief      Certificate chain compatible
* @ingroup    DATAIO
* 
* @param[in]  chain : Pointer to chain.
* @param[in]  offered : Offered value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool DIOSTREAMTLS13_HANDSHAKESERVER_CertificateChainCompatible(XVECTOR<XBUFFER*>* chain, XVECTOR<XWORD>& offered)
{
  if(!chain || chain->IsEmpty() || offered.IsEmpty()) return false;

  for(XDWORD c=0; c<chain->GetSize(); c++)
    {
      XBUFFER* certificateDER = chain->Get(c);
      if(!certificateDER || certificateDER->IsEmpty()) return false;

      CIPHERCERTIFICATEX509 certificate;
      if(!certificate.Decode((*certificateDER))) return false;

      if(certificate.GetAlgorithmType() == CIPHERCERTIFICATEX509_ALGORITHM_TYPE_RSASSAPSS)
        {
          XWORD RSAEscheme = 0;
          XWORD PSSscheme  = 0;

          switch(certificate.GetRSASSAPSSHashType())
            {
              case CIPHERCERTIFICATEX509_RSASSAPSS_HASH_TYPE_SHA256 : RSAEscheme = DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA256; PSSscheme = DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA256; break;
              case CIPHERCERTIFICATEX509_RSASSAPSS_HASH_TYPE_SHA384 : RSAEscheme = DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA384; PSSscheme = DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA384; break;
              case CIPHERCERTIFICATEX509_RSASSAPSS_HASH_TYPE_SHA512 : RSAEscheme = DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA512; PSSscheme = DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA512; break;
              default                                                : return false;
            }

          if(!DIOSTREAMTLS13_HANDSHAKESERVER_SignatureSchemeOffered(offered, RSAEscheme) &&
             !DIOSTREAMTLS13_HANDSHAKESERVER_SignatureSchemeOffered(offered, PSSscheme)) return false;
          continue;
        }

      XWORD scheme = DIOSTREAMTLS13_HANDSHAKESERVER_CertificateSignatureScheme(certificate);
      if(!scheme || !DIOSTREAMTLS13_HANDSHAKESERVER_SignatureSchemeOffered(offered, scheme)) return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS13_HANDSHAKESERVER_OCSPRequest_Parse(XBUFFER* data, bool& requested)
* @brief      Ocsp request parse
* @ingroup    DATAIO
* 
* @param[in]  data : Pointer to data.
* @param[in]  requested : Requested value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool DIOSTREAMTLS13_HANDSHAKESERVER_OCSPRequest_Parse(XBUFFER* data, bool& requested)
{
  requested = false;
  if(!data || data->IsEmpty()) return false;

  // status_type values unknown to this implementation are ignored.  The RFC 6066 OCSP request body is parsed
  // completely before accepting status_type=ocsp, including both nested length fields.
  if(data->GetByte(0) != 1) return true;
  if(data->GetSize() < 5) return false;

  XDWORD responderlistlength = ((XDWORD)data->GetByte(1) << 8) | data->GetByte(2);
  XDWORD offset              = 3;
  XDWORD responderlistend    = offset + responderlistlength;

  if((responderlistend + 2) > data->GetSize()) return false;

  while(offset < responderlistend)
    {
      if((offset + 2) > responderlistend) return false;
      XDWORD responderlength = ((XDWORD)data->GetByte(offset) << 8) | data->GetByte(offset+1);
      offset += 2;
      if(!responderlength || ((offset + responderlength) > responderlistend)) return false;
      offset += responderlength;
    }

  XDWORD requestextensionslength = ((XDWORD)data->GetByte(responderlistend) << 8) |
                                    data->GetByte(responderlistend+1);
  if((responderlistend + 2 + requestextensionslength) != data->GetSize()) return false;

  requested = true;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTLS_ALERT_DESCRIPTION DIOSTREAMTLS13_HANDSHAKESERVER_CertificateAlert(CIPHERCERTIFICATEX509VALIDATOR_ERROR error)
* @brief      Certificate alert
* @ingroup    DATAIO
* 
* @param[in]  error : Error value.
* 
* @return     DIOSTREAMTLS_ALERT_DESCRIPTION : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static DIOSTREAMTLS_ALERT_DESCRIPTION DIOSTREAMTLS13_HANDSHAKESERVER_CertificateAlert(CIPHERCERTIFICATEX509VALIDATOR_ERROR error)
{
  switch(error)
    {
      case CIPHERCERTIFICATEX509VALIDATOR_ERROR_UNTRUSTEDROOT            : return DIOSTREAMTLS_ALERT_DESCRIPTION_UNKNOWN_CA;
      case CIPHERCERTIFICATEX509VALIDATOR_ERROR_INVALIDDATE              : return DIOSTREAMTLS_ALERT_DESCRIPTION_CERTIFICATE_EXPIRED;
      case CIPHERCERTIFICATEX509VALIDATOR_ERROR_UNSUPPORTEDALGORITHM     : return DIOSTREAMTLS_ALERT_DESCRIPTION_UNSUPPORTED_CERTIFICATE;
      case CIPHERCERTIFICATEX509VALIDATOR_ERROR_UNKNOWNCRITICALEXTENSION : return DIOSTREAMTLS_ALERT_DESCRIPTION_UNSUPPORTED_CERTIFICATE;
      case CIPHERCERTIFICATEX509VALIDATOR_ERROR_WEAKKEY                  : return DIOSTREAMTLS_ALERT_DESCRIPTION_UNSUPPORTED_CERTIFICATE;
      case CIPHERCERTIFICATEX509VALIDATOR_ERROR_NAMECONSTRAINT           : return DIOSTREAMTLS_ALERT_DESCRIPTION_BAD_CERTIFICATE;
      case CIPHERCERTIFICATEX509VALIDATOR_ERROR_REVOKED                  : return DIOSTREAMTLS_ALERT_DESCRIPTION_CERTIFICATE_REVOKED;
      case CIPHERCERTIFICATEX509VALIDATOR_ERROR_REVOCATIONUNKNOWN        : return DIOSTREAMTLS_ALERT_DESCRIPTION_CERTIFICATE_UNKNOWN;
                                                               default    : return DIOSTREAMTLS_ALERT_DESCRIPTION_BAD_CERTIFICATE;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS13_HANDSHAKESERVER_ExtensionIsEqual(DIOSTREAMTLS_MSG_EXTENSION* first, DIOSTREAMTLS_MSG_EXTENSION* second)
* @brief      Extension is equal
* @ingroup    DATAIO
* 
* @param[in]  first : Pointer to first.
* @param[in]  second : Pointer to second.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool DIOSTREAMTLS13_HANDSHAKESERVER_ExtensionIsEqual(DIOSTREAMTLS_MSG_EXTENSION* first, DIOSTREAMTLS_MSG_EXTENSION* second)
{
  XBUFFER firstbuffer;
  XBUFFER secondbuffer;

  if(!first || !second || (first->GetType() != second->GetType()) ||
     !first->SetToBuffer(firstbuffer, false) || !second->SetToBuffer(secondbuffer, false) ||
     (firstbuffer.GetSize() != secondbuffer.GetSize()))
    {
      return false;
    }

  if(!firstbuffer.GetSize()) return true;

  return !memcmp(firstbuffer.Get(), secondbuffer.Get(), firstbuffer.GetSize());
}



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS13HANDSHAKESERVER::DIOSTREAMTLS13HANDSHAKESERVER()
* @brief      Constructor of class
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS13HANDSHAKESERVER::DIOSTREAMTLS13HANDSHAKESERVER()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS13HANDSHAKESERVER::~DIOSTREAMTLS13HANDSHAKESERVER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS13HANDSHAKESERVER::~DIOSTREAMTLS13HANDSHAKESERVER()
{
  End();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKESERVER::Ini(DIOSTREAMTLS13SESSION* session, DIOSTREAMTLSCONFIG* config)
* @brief      Initialize the handshake with the role-neutral session and the server configuration to use
* @ingroup    DATAIO
*
* @param[in]  session : TLS 1.3 session, already Ini() with DIOSTREAMTLSKEYSCHEDULE_ROLE_SERVER.
* @param[in]  config : Server configuration (cipher suites / groups / signature schemes / ALPN preference order,
*             plus the local certificate chain and private key to authenticate with).
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKESERVER::Ini(DIOSTREAMTLS13SESSION* session, DIOSTREAMTLSCONFIG* config)
{
  if(isini || !session || !session->IsIni() || (session->GetRole() != DIOSTREAMTLSKEYSCHEDULE_ROLE_SERVER) ||
     !config || !config->HasLocalCredentials())
    {
      return false;
    }

  Clean();

  this->session = session;
  this->config  = config;
  clientcertificatevalidator.SetPolicy((*config->GetCertificateValidationPolicy()));
  state          = DIOSTREAMTLS13HANDSHAKESERVER_STATE_NONE;
  isini          = true;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS13HANDSHAKESERVER::End()
* @brief      End the handshake
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS13HANDSHAKESERVER::End()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKESERVER::IsIni()
* @brief      Check if the class is initialized
* @ingroup    DATAIO
*
* @return     bool : true if the condition is met; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKESERVER::IsIni()
{
  return isini;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS13HANDSHAKESERVER_STATE DIOSTREAMTLS13HANDSHAKESERVER::GetState()
* @brief      Get the current handshake state
* @ingroup    DATAIO
*
* @return     DIOSTREAMTLS13HANDSHAKESERVER_STATE : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS13HANDSHAKESERVER_STATE DIOSTREAMTLS13HANDSHAKESERVER::GetState()
{
  return state;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKESERVER::IsHandshakeCompleted()
* @brief      Check whether the handshake has completed
* @ingroup    DATAIO
*
* @return     bool : true if the condition is met; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKESERVER::IsHandshakeCompleted()
{
  return (state == DIOSTREAMTLS13HANDSHAKESERVER_STATE_HANDSHAKE_COMPLETED);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKESERVER::IsWaitingClientHelloRetry()
* @brief      Check whether a HelloRetryRequest was sent and the second ClientHello is still pending
* @ingroup    DATAIO
*
* @return     bool : true if the condition is met; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKESERVER::IsWaitingClientHelloRetry()
{
  return (state == DIOSTREAMTLS13HANDSHAKESERVER_STATE_WAIT_CLIENTHELLO_RETRY);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS13HANDSHAKESERVER::IsClientAuthenticated()
* @brief      Is client authenticated
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKESERVER::IsClientAuthenticated()
{
  return clientcertificateprovided && clientcertificatevalidator.GetLeafCertificate();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS13HANDSHAKESERVER::IsSessionResumed()
* @brief      Is session resumed
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKESERVER::IsSessionResumed()
{
  return resumptionaccepted;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS13HANDSHAKESERVER::IsEarlyDataOffered()
* @brief      Is early data offered
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKESERVER::IsEarlyDataOffered() { return earlydataoffered; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS13HANDSHAKESERVER::IsEarlyDataAccepted()
* @brief      Is early data accepted
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKESERVER::IsEarlyDataAccepted() { return earlydataaccepted; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOSTREAMTLS13HANDSHAKESERVER::GetEarlyDataSize()
* @brief      Get early data size
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAMTLS13HANDSHAKESERVER::GetEarlyDataSize() { return session?session->GetEarlyDataSize():0; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOSTREAMTLS13HANDSHAKESERVER::EarlyData_Read(XBYTE* data, XDWORD size)
* @brief      Early data read
* @ingroup    DATAIO
* 
* @param[in]  data : Pointer to data.
* @param[in]  size : Size value.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAMTLS13HANDSHAKESERVER::EarlyData_Read(XBYTE* data, XDWORD size) { return session?session->EarlyData_Read(data, size):0; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHERCERTIFICATEX509* DIOSTREAMTLS13HANDSHAKESERVER::GetClientCertificate()
* @brief      Get client certificate
* @ingroup    DATAIO
* 
* @return     CIPHERCERTIFICATEX509* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERCERTIFICATEX509* DIOSTREAMTLS13HANDSHAKESERVER::GetClientCertificate()
{
  return IsClientAuthenticated()?clientcertificatevalidator.GetLeafCertificate():NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKESERVER::IsApplicationProtocolNegotiated()
* @brief      Check whether an ALPN protocol was negotiated
* @ingroup    DATAIO
*
* @return     bool : true if the condition is met; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKESERVER::IsApplicationProtocolNegotiated()
{
  return applicationprotocolnegotiated;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_ALPN_TYPE DIOSTREAMTLS13HANDSHAKESERVER::GetApplicationProtocol()
* @brief      Get the negotiated ALPN protocol
* @ingroup    DATAIO
*
* @return     DIOSTREAMTLS_ALPN_TYPE : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_ALPN_TYPE DIOSTREAMTLS13HANDSHAKESERVER::GetApplicationProtocol()
{
  return applicationprotocol;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBUFFER* DIOSTREAMTLS13HANDSHAKESERVER::GetApplicationProtocolRaw()
* @brief      Get application protocol raw
* @ingroup    DATAIO
* 
* @return     XBUFFER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* DIOSTREAMTLS13HANDSHAKESERVER::GetApplicationProtocolRaw()
{
  return &applicationprotocolraw;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_ALERT_DESCRIPTION DIOSTREAMTLS13HANDSHAKESERVER::GetErrorAlertDescription()
* @brief      Get the TLS alert that best describes the current handshake error
* @ingroup    DATAIO
*
* @return     DIOSTREAMTLS_ALERT_DESCRIPTION : Alert description.
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_ALERT_DESCRIPTION DIOSTREAMTLS13HANDSHAKESERVER::GetErrorAlertDescription()
{
  return erroralertdescription;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKESERVER::ShouldSendErrorAlert()
* @brief      Check whether the current handshake error was generated locally and should be reported to the peer
* @ingroup    DATAIO
*
* @return     bool : true when a fatal alert should be sent; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKESERVER::ShouldSendErrorAlert()
{
  return senderroralert;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKESERVER::CipherSuite_Select(XVECTOR<XWORD>& offered, XWORD& selected)
* @brief      Select the first configured cipher suite (server preference order) that the client also offered
* @note       INTERNAL
* @ingroup    DATAIO
*
* @param[in]  offered : Cipher suites offered by the client.
* @param[out] selected : Selected cipher suite.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKESERVER::CipherSuite_Select(XVECTOR<XWORD>& offered, XWORD& selected)
{
  for(XDWORD c=0; c<config->GetCipherSuites()->GetSize(); c++)
    {
      XWORD candidate = config->GetCipherSuites()->Get(c);

      for(XDWORD d=0; d<offered.GetSize(); d++)
        {
          if(offered.Get(d) == candidate)
            {
              selected = candidate;
              return true;
            }
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKESERVER::Group_Select(DIOSTREAMTLS_MSG_HANDSHAKE_CLIENTHELLO* clienthello, XWORD& selectedgroup, XBUFFER& peerpublickey, bool& helloretryrequestrequired)
* @brief      Select the first configured mutually supported group, requesting a new key_share when needed
* @note       INTERNAL. Server preference order is preserved. A HelloRetryRequest is requested only when the
*             selected group was advertised in supported_groups but no key_share for it was sent.
* @ingroup    DATAIO
*
* @param[in]  clienthello : Decoded ClientHello body.
* @param[out] selectedgroup : Selected group.
* @param[out] peerpublickey : Client key_share public key when already available.
* @param[out] helloretryrequestrequired : true when the selected group needs a retried key_share.
* @param[in]  invalidkeyshare : Invalidkeyshare value.
*
* @return     bool : true if a mutually supported group was selected; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKESERVER::Group_Select(DIOSTREAMTLS_MSG_HANDSHAKE_CLIENTHELLO* clienthello, XWORD& selectedgroup, XBUFFER& peerpublickey, bool& helloretryrequestrequired, bool& invalidkeyshare)
{
  DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDGROUPS* supportedgroups = NULL;
  DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE*        keyshare        = NULL;

  peerpublickey.Delete();
  helloretryrequestrequired = false;
  invalidkeyshare           = false;

  if(!clienthello) return false;

  for(XDWORD c=0; c<clienthello->Extensions_GetAll()->GetSize(); c++)
    {
      DIOSTREAMTLS_MSG_EXTENSION* extension = clienthello->Extensions_GetAll()->Get(c);
      if(!extension) return false;

      if(extension->GetType() == DIOSTREAMTLS_MSG_EXTENSION_TYPE_SUPPORTEDGROUPS)
        {
          if(supportedgroups) return false;
          supportedgroups = (DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDGROUPS*)extension;
        }

      if(extension->GetType() == DIOSTREAMTLS_MSG_EXTENSION_TYPE_KEYSHARE)
        {
          if(keyshare) return false;
          keyshare = (DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE*)extension;
        }
    }

  if(!supportedgroups || !keyshare) return false;

  // First preference: use a key_share that the client already supplied.  HRR is not a mechanism for
  // enforcing the server's absolute group preference when another mutually supported and usable key_share
  // is already present; avoiding an unnecessary retry also improves interoperability with normal browsers.
  for(XDWORD c=0; c<config->GetSupportedGroups()->GetSize(); c++)
    {
      XWORD candidate = config->GetSupportedGroups()->Get(c);

      for(XDWORD d=0; d<keyshare->List_Get()->GetSize(); d++)
        {
          DIOSTREAMTLS_MSG_EXTENSION_KEY* key = keyshare->List_Get()->Get(d);

          if(key && (key->GetKeyType() == candidate))
            {
              bool validsize = ((candidate == DIOSTREAMTLS_MSG_CURVEID_X25519)    && (key->GetKeyData()->GetSize() == CIPHERECDSAX25519_MAXKEY))          ||
                               ((candidate == DIOSTREAMTLS_MSG_CURVEID_SECP256R1) && (key->GetKeyData()->GetSize() == CIPHERECDSA_P256_PUBLICKEY_SIZE)) ||
                               ((candidate == DIOSTREAMTLS_MSG_CURVEID_SECP384R1) && (key->GetKeyData()->GetSize() == CIPHERECDSA_P384_PUBLICKEY_SIZE)) ||
                               ((candidate == DIOSTREAMTLS_MSG_CURVEID_SECP521R1) && (key->GetKeyData()->GetSize() == CIPHERECDSA_P521_PUBLICKEY_SIZE));

              #ifdef CIPHER_ASYMMETRIC_MLKEM768_ACTIVE
              validsize = validsize ||
                          ((candidate == DIOSTREAMTLS_MSG_CURVEID_SECP256R1MLKEM768) &&
                           (key->GetKeyData()->GetSize() == CIPHERSECP256R1MLKEM768_CLIENTSHARESIZE));
              #endif

              #ifdef CIPHER_ASYMMETRIC_MLKEM1024_ACTIVE
              validsize = validsize ||
                          ((candidate == DIOSTREAMTLS_MSG_CURVEID_SECP384R1MLKEM1024) &&
                           (key->GetKeyData()->GetSize() == CIPHERSECP384R1MLKEM1024_CLIENTSHARESIZE));
              #endif

              #if defined(CIPHER_ASYMMETRIC_X25519_ACTIVE) && defined(CIPHER_ASYMMETRIC_MLKEM768_ACTIVE)
              validsize = validsize ||
                          ((candidate == DIOSTREAMTLS_MSG_CURVEID_X25519MLKEM768) &&
                           (key->GetKeyData()->GetSize() == CIPHERX25519MLKEM768_CLIENTSHARESIZE));
              #endif

              if(!validsize)
                {
                  invalidkeyshare = true;
                  return false;
                }

              selectedgroup = candidate;
              return peerpublickey.Add((*key->GetKeyData()));
            }
        }
    }

  // No configured key_share can be used directly.  Only now look for a mutually supported group and
  // request a new share with HelloRetryRequest.  Keep the configured server preference order.
  for(XDWORD c=0; c<config->GetSupportedGroups()->GetSize(); c++)
    {
      XWORD candidate = config->GetSupportedGroups()->Get(c);

      for(XDWORD d=0; d<supportedgroups->List_Get()->GetSize(); d++)
        {
          if(supportedgroups->List_Get()->Get(d) == candidate)
            {
              selectedgroup = candidate;
              helloretryrequestrequired = true;
              return true;
            }
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKESERVER::SignatureScheme_Select(XVECTOR<XWORD>& offered, CIPHERCERTIFICATEX509* leafcertificate, XWORD& selected)
* @brief      Select the first configured signature scheme (server preference order) offered by the client and
*             usable with the local leaf certificate's public key
* @note       INTERNAL
* @ingroup    DATAIO
*
* @param[in]  offered : Signature schemes offered by the client (signature_algorithms extension).
* @param[in]  leafpublickey : Public cipher key decoded from the local leaf certificate.
* @param[out] selected : Selected signature scheme.
* @param[in]  leafcertificate : Pointer to leafcertificate.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKESERVER::SignatureScheme_Select(XVECTOR<XWORD>& offered, CIPHERCERTIFICATEX509* leafcertificate, XWORD& selected)
{
  for(XDWORD c=0; c<config->GetSignatureSchemes()->GetSize(); c++)
    {
      XWORD candidate = config->GetSignatureSchemes()->Get(c);

      if(!DIOSTREAMTLSSIGNATURE::IsSupported(candidate, leafcertificate)) continue;

      for(XDWORD d=0; d<offered.GetSize(); d++)
        {
          if(offered.Get(d) == candidate)
            {
              selected = candidate;
              return true;
            }
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS13HANDSHAKESERVER::ServerCredentials_Select(XCHAR* servername, XVECTOR<XWORD>& offeredsignatures, XVECTOR<XWORD>& offeredcertificatesignatures, XVECTOR<XBUFFER*>*& certificatechain, CIPHERKEY*& privatekey, XBUFFER*& OCSPstapledresponse, CIPHERCERTIFICATEX509& leafcertificate, XWORD& signaturescheme)
* @brief      Server credentials select
* @ingroup    DATAIO
* 
* @param[in]  servername : Pointer to servername.
* @param[in]  offeredsignatures : Offeredsignatures value.
* @param[in]  offeredcertificatesignatures : Offeredcertificatesignatures value.
* @param[in]  certificatechain : Pointer to certificatechain.
* @param[in]  privatekey : Pointer to privatekey.
* @param[in]  OCSPstapledresponse : Pointer to OCSPstapledresponse.
* @param[in]  leafcertificate : Leafcertificate value.
* @param[in]  signaturescheme : Signaturescheme value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKESERVER::ServerCredentials_Select(XCHAR* servername, XVECTOR<XWORD>& offeredsignatures, XVECTOR<XWORD>& offeredcertificatesignatures, XVECTOR<XBUFFER*>*& certificatechain, CIPHERKEY*& privatekey, XBUFFER*& OCSPstapledresponse, CIPHERCERTIFICATEX509& leafcertificate, XWORD& signaturescheme)
{
  certificatechain     = NULL;
  privatekey            = NULL;
  OCSPstapledresponse   = NULL;
  signaturescheme       = 0;

  if(!config || offeredsignatures.IsEmpty() || offeredcertificatesignatures.IsEmpty()) return false;

  // SNI specificity has priority over certificate algorithm preference. Within one SNI level, the configured
  // signature-scheme order selects the preferred usable credential.
  XDWORD firstpass = (servername && servername[0])?0:2;

  for(XDWORD pass=firstpass; pass<3; pass++)
    {
      for(XDWORD s=0; s<config->GetSignatureSchemes()->GetSize(); s++)
        {
          XWORD candidate = config->GetSignatureSchemes()->Get(s);
          if(!DIOSTREAMTLS13_HANDSHAKESERVER_SignatureSchemeOffered(offeredsignatures, candidate)) continue;

          if(pass < 2)
            {
              for(XDWORD c=0; c<config->GetServerCredentials()->GetSize(); c++)
                {
                  DIOSTREAMTLSSERVERCREDENTIALS* credentials = config->GetServerCredentials()->Get(c);
                  if(!credentials || !credentials->HasCredentials()) continue;

                  XSTRING pattern((*credentials->GetServerName()));
                  bool wildcard = (pattern.FindCharacter(_C('*')) >= 0);
                  if((pass == 0 && wildcard) || (pass == 1 && !wildcard)) continue;
                  if(!DIOSTREAMTLS13_HANDSHAKESERVER_ServerNameMatch(pattern, servername)) continue;

                  XVECTOR<XBUFFER*>* chain = credentials->GetCertificateChain();
                  if(!chain || chain->IsEmpty() || !credentials->GetPrivateKey()) continue;

                  CIPHERCERTIFICATEX509 leaf;
                  if(!leaf.Decode((*chain->Get(0))) ||
                     !DIOSTREAMTLSSIGNATURE::IsSupported(candidate, &leaf) ||
                     !DIOSTREAMTLS13_HANDSHAKESERVER_CertificateChainCompatible(chain, offeredcertificatesignatures)) continue;

                  certificatechain     = chain;
                  privatekey            = credentials->GetPrivateKey();
                  OCSPstapledresponse   = credentials->GetOCSPStapledResponse();
                  if(!leafcertificate.Decode((*chain->Get(0)))) return false;
                  signaturescheme       = candidate;
                  return true;
                }
            }
           else
            {
              if(!config->HasLocalCredentials()) continue;

              XVECTOR<XBUFFER*>* chain = config->GetLocalCertificateChain();
              if(!chain || chain->IsEmpty() || !config->GetLocalPrivateKey()) continue;

              CIPHERCERTIFICATEX509 leaf;
              if(!leaf.Decode((*chain->Get(0))) ||
                 !DIOSTREAMTLSSIGNATURE::IsSupported(candidate, &leaf) ||
                 !DIOSTREAMTLS13_HANDSHAKESERVER_CertificateChainCompatible(chain, offeredcertificatesignatures)) continue;

              certificatechain     = chain;
              privatekey            = config->GetLocalPrivateKey();
              OCSPstapledresponse   = config->GetLocalOCSPStapledResponse();
              if(!leafcertificate.Decode((*chain->Get(0)))) return false;
              signaturescheme       = candidate;
              return true;
            }
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS13HANDSHAKESERVER::ApplicationProtocol_Select(XVECTOR<DIOSTREAMTLS_ALPN_TYPE>& offered)
* @brief      Select the first configured ALPN protocol (server preference order) the client also offered
* @note       INTERNAL. If the client sent ALPN, the caller must treat no common protocol as
*             no_application_protocol.  Absence of the ALPN extension is a different, valid case.
* @ingroup    DATAIO
*
* @param[in]  offered : Application protocols offered by the client (ALPN extension).
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS13HANDSHAKESERVER::ApplicationProtocol_Select(DIOSTREAMTLS_MSG_EXTENSION_ALPN* offered)
{
  applicationprotocolnegotiated = false;
  applicationprotocol           = DIOSTREAMTLS_ALPN_TYPE_UNKNOWN;
  applicationprotocolraw.Delete();

  if(!offered) return;

  for(XDWORD c=0; c<config->GetApplicationProtocolsCount(); c++)
    {
      XBUFFER candidate;
      if(!config->GetApplicationProtocol(c, candidate)) return;

      for(XDWORD d=0; d<offered->List_GetNProtocols(); d++)
        {
          XBUFFER offeredprotocol;
          if(!offered->List_Get(d, offeredprotocol)) return;
          if(offeredprotocol.Compare(candidate))
            {
              offered->List_Get(d, applicationprotocol);
              applicationprotocolraw.Add(candidate);
              applicationprotocolnegotiated = true;
              return;
            }
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKESERVER::ClientHelloRetry_Validate(DIOSTREAMTLS_MSG_HANDSHAKE_CLIENTHELLO* clienthello)
* @brief      Validate the second ClientHello received after a TLS 1.3 HelloRetryRequest
* @ingroup    DATAIO
*
* @param[in]  clienthello : Decoded second ClientHello body.
*
* @return     bool : true if ClientHello2 is valid for the outstanding retry; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKESERVER::ClientHelloRetry_Validate(DIOSTREAMTLS_MSG_HANDSHAKE_CLIENTHELLO* clienthello)
{
  DIOSTREAMTLS_MSG_FRAGMENT<DIOSTREAMTLS_MSG_HANDSHAKE_CLIENTHELLO> firstmessage;
  XBUFFER                                                           workbuffer;

  if(!clienthello || firstclienthello.IsEmpty() || !retryselectedgroup) return false;

  workbuffer.Add(firstclienthello);

  if(!firstmessage.GetFromBuffer(workbuffer, false) || !workbuffer.IsEmpty() ||
     (firstmessage.GetMsgType() != DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_CLIENT_HELLO))
    {
      return false;
    }

  DIOSTREAMTLS_MSG_HANDSHAKE_CLIENTHELLO* first = firstmessage.GetBody();
  if(!first) return false;

  if((first->GetClientVersion() != clienthello->GetClientVersion()) ||
     memcmp(first->GetRandom(), clienthello->GetRandom(), DIOSTREAMTLS_MSG_RANDOM_SIZE) ||
     (first->GetSessionIDLength() != clienthello->GetSessionIDLength()) ||
     (first->GetSessionIDLength() && memcmp(first->GetSessionID(), clienthello->GetSessionID(), first->GetSessionIDLength())) ||
     (first->GetCompressionLength() != clienthello->GetCompressionLength()) ||
     (first->GetCompressionMethod() != clienthello->GetCompressionMethod()) ||
     (first->GetCipherSuites()->GetSize() != clienthello->GetCipherSuites()->GetSize()))
    {
      return false;
    }

  for(XDWORD c=0; c<first->GetCipherSuites()->GetSize(); c++)
    {
      if(first->GetCipherSuites()->Get(c) != clienthello->GetCipherSuites()->Get(c)) return false;
    }

  bool keysharevalidated = false;

  // ClientHello2 is the same ClientHello except for the modifications explicitly permitted after HRR.
  // GEN v1 emits only a key_share request, but accept the standard client-side changes for early_data,
  // pre_shared_key and padding so normal TLS 1.3 clients are not rejected unnecessarily.
  for(XDWORD c=0; c<clienthello->Extensions_GetAll()->GetSize(); c++)
    {
      DIOSTREAMTLS_MSG_EXTENSION* secondextension = clienthello->Extensions_GetAll()->Get(c);
      if(!secondextension) return false;

      if(secondextension->GetType() == DIOSTREAMTLS13_HANDSHAKESERVER_EXTENSION_EARLYDATA) return false;

      if(secondextension->GetType() == DIOSTREAMTLS_MSG_EXTENSION_TYPE_KEYSHARE)
        {
          if(keysharevalidated) return false;

          DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE* secondkeyshare = (DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE*)secondextension;
          if(secondkeyshare->List_Get()->GetSize() != 1) return false;

          DIOSTREAMTLS_MSG_EXTENSION_KEY* key = secondkeyshare->List_Get()->Get(0);
          if(!key || (key->GetKeyType() != retryselectedgroup)) return false;

          keysharevalidated = true;
          continue;
        }

      if((secondextension->GetType() == DIOSTREAMTLS13_HANDSHAKESERVER_EXTENSION_PADDING) ||
         (secondextension->GetType() == DIOSTREAMTLS_MSG_EXTENSION_TYPE_PRESHAREDKEY))
        {
          continue;
        }

      DIOSTREAMTLS_MSG_EXTENSION* firstextension = NULL;

      for(XDWORD d=0; d<first->Extensions_GetAll()->GetSize(); d++)
        {
          DIOSTREAMTLS_MSG_EXTENSION* candidate = first->Extensions_GetAll()->Get(d);
          if(candidate && (candidate->GetType() == secondextension->GetType()))
            {
              firstextension = candidate;
              break;
            }
        }

      if(!firstextension || !DIOSTREAMTLS13_HANDSHAKESERVER_ExtensionIsEqual(firstextension, secondextension)) return false;
    }

  if(!keysharevalidated) return false;

  // Every unchanged extension from ClientHello1 must still be present.  early_data may be removed, padding may
  // be changed/added/removed, and pre_shared_key may be recomputed as allowed by TLS 1.3 after HRR.
  for(XDWORD c=0; c<first->Extensions_GetAll()->GetSize(); c++)
    {
      DIOSTREAMTLS_MSG_EXTENSION* firstextension = first->Extensions_GetAll()->Get(c);
      if(!firstextension) return false;

      if((firstextension->GetType() == DIOSTREAMTLS_MSG_EXTENSION_TYPE_KEYSHARE) ||
         (firstextension->GetType() == DIOSTREAMTLS13_HANDSHAKESERVER_EXTENSION_EARLYDATA) ||
         (firstextension->GetType() == DIOSTREAMTLS13_HANDSHAKESERVER_EXTENSION_PADDING) ||
         (firstextension->GetType() == DIOSTREAMTLS_MSG_EXTENSION_TYPE_PRESHAREDKEY))
        {
          continue;
        }

      bool found = false;

      for(XDWORD d=0; d<clienthello->Extensions_GetAll()->GetSize(); d++)
        {
          DIOSTREAMTLS_MSG_EXTENSION* candidate = clienthello->Extensions_GetAll()->Get(d);
          if(candidate && (candidate->GetType() == firstextension->GetType()) &&
             DIOSTREAMTLS13_HANDSHAKESERVER_ExtensionIsEqual(firstextension, candidate))
            {
              found = true;
              break;
            }
        }

      if(!found) return false;
    }

  // The requested group must not have appeared in ClientHello1's key_share; otherwise HRR itself would have
  // been invalid and a second ClientHello for that group must not be accepted.
  for(XDWORD c=0; c<first->Extensions_GetAll()->GetSize(); c++)
    {
      DIOSTREAMTLS_MSG_EXTENSION* extension = first->Extensions_GetAll()->Get(c);

      if(extension && (extension->GetType() == DIOSTREAMTLS_MSG_EXTENSION_TYPE_KEYSHARE))
        {
          DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE* firstkeyshare = (DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE*)extension;

          for(XDWORD d=0; d<firstkeyshare->List_Get()->GetSize(); d++)
            {
              DIOSTREAMTLS_MSG_EXTENSION_KEY* key = firstkeyshare->List_Get()->Get(d);
              if(key && (key->GetKeyType() == retryselectedgroup)) return false;
            }
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKESERVER::HelloRetryRequest_Create(DIOSTREAMTLS_MSG_HANDSHAKE_CLIENTHELLO* clienthello, XBUFFER& clienthellobuffer, XWORD ciphersuite, XWORD group, XBUFFER& records)
* @brief      Create a TLS 1.3 HelloRetryRequest and rebuild the transcript using message_hash(ClientHello1)
* @note       INTERNAL. Only a key_share retry is generated; GEN v1 does not issue HRR cookies.
* @ingroup    DATAIO
*
* @param[in]  clienthello : Decoded first ClientHello body.
* @param[in]  clienthellobuffer : Complete encoded first ClientHello.
* @param[in]  ciphersuite : Cipher suite selected for the retry and final handshake.
* @param[in]  group : Mutually supported group whose key_share is requested.
* @param[out] records : Clear-text TLS records containing the HelloRetryRequest.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKESERVER::HelloRetryRequest_Create(DIOSTREAMTLS_MSG_HANDSHAKE_CLIENTHELLO* clienthello, XBUFFER& clienthellobuffer, XWORD ciphersuite, XWORD group, XBUFFER& records)
{
  DIOSTREAMTLS_MSG_FRAGMENT<DIOSTREAMTLS_MSG_HANDSHAKE_SERVERHELLO> hrrmessage;
  DIOSTREAMTLS_MSG_HANDSHAKE_SERVERHELLO*                           hrrbody;
  DIOSTREAMTLS_MSG_HANDSHAKE                                       messagehash;
  XBUFFER                                                           firsthash;
  XBUFFER                                                           messagehashbuffer;
  XBUFFER                                                           hrrbuffer;

  if(!clienthello || firstclienthello.GetSize() || !group || !session->CipherSuite_Select(ciphersuite) ||
     !session->Transcript_Add(clienthellobuffer) || !session->TranscriptHash(firsthash))
    {
      return SetError();
    }

  messagehash.SetMsgType(DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_MESSAGE_HASH);

  if(!messagehash.GetBody()->Add(firsthash) || !messagehash.SetToBuffer(messagehashbuffer, false))
    {
      return SetError();
    }

  hrrmessage.SetMsgType(DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_SERVER_HELLO);
  hrrbody = hrrmessage.GetBody();
  if(!hrrbody) return SetError();

  hrrbody->SetLegacyVersion(DIOSTREAMTLS_MSG_VERSION_TLS_1_2);
  memcpy(hrrbody->GetRandom(), DIOSTREAMTLS13_HANDSHAKESERVER_HELLORETRYREQUEST_RANDOM, DIOSTREAMTLS_MSG_HELLORETRYREQUEST_RANDOM_SIZE);

  hrrbody->SetSessionIDLength(clienthello->GetSessionIDLength());
  if(clienthello->GetSessionIDLength())
    {
      memcpy(hrrbody->GetSessionID(), clienthello->GetSessionID(), clienthello->GetSessionIDLength());
    }

  hrrbody->SetCipherSuite(ciphersuite);
  hrrbody->SetCompressionMethod(DIOSTREAMTLS_MSG_COMPRESS_METHOD_NULL);

  DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS_SERVER* supportedversions = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS_SERVER();
  if(!supportedversions) return SetError();

  supportedversions->SetVersion(DIOSTREAMTLS_MSG_VERSION_TLS_1_3);

  if(!hrrbody->Extensions_Add(supportedversions))
    {
      GEN_DELETE supportedversions;
      return SetError();
    }

  DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_HELLORETRYREQUEST* keyshare = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_HELLORETRYREQUEST();
  if(!keyshare) return SetError();

  keyshare->SetSelectedGroup(group);

  if(!hrrbody->Extensions_Add(keyshare))
    {
      GEN_DELETE keyshare;
      return SetError();
    }

  records.Delete();

  if(!hrrmessage.SetToBuffer(hrrbuffer, false) ||
     !session->GetRecord()->Protect(DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE, hrrbuffer, records) ||
     !firstclienthello.Add(clienthellobuffer))
    {
      records.Delete();
      return SetError();
    }

  session->GetTranscript()->Delete();

  if(!session->Transcript_Add(messagehashbuffer) || !session->Transcript_Add(hrrbuffer))
    {
      records.Delete();
      return SetError();
    }

  retryselectedgroup = group;
  retryciphersuite   = ciphersuite;
  state              = DIOSTREAMTLS13HANDSHAKESERVER_STATE_WAIT_CLIENTHELLO_RETRY;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKESERVER::ResumptionPSK_Select(DIOSTREAMTLS_MSG_HANDSHAKE_CLIENTHELLO* clienthello, XBUFFER& clienthellobuffer, XCHAR* servername, DIOSTREAMTLS_ALPN_TYPE applicationprotocol, XWORD& ciphersuite, XBUFFER& PSK)
* @brief      Validate one stateless resumption ticket and its PSK binder
* @note       INTERNAL. GEN accepts only psk_dhe_ke, preserving forward secrecy. Early data, when offered, is bound to identity 0.
* @ingroup    DATAIO
* 
* @param[in]  clienthello : Pointer to clienthello.
* @param[in]  clienthellobuffer : Clienthellobuffer value.
* @param[in]  servername : Pointer to servername.
* @param[in]  applicationprotocol : Pointer to applicationprotocol.
* @param[in]  ciphersuite : Ciphersuite value.
* @param[in]  PSK : PSK value.
*
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKESERVER::ResumptionPSK_Select(DIOSTREAMTLS_MSG_HANDSHAKE_CLIENTHELLO* clienthello, XBUFFER& clienthellobuffer, XCHAR* servername, XBUFFER* applicationprotocol, XWORD& ciphersuite, XBUFFER& PSK)
{
  if(!config || !config->IsSessionResumptionActive() || !clienthello ||
     (config->GetClientAuthenticationMode() != DIOSTREAMTLS_CLIENTAUTHENTICATION_MODE_NONE)) return false;

  DIOSTREAMTLS_MSG_EXTENSION_PSKKEYEXCHANGEMODES* modes = NULL;
  DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY*         offeredpsk = NULL;
  DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN*              earlyextension = NULL;

  for(XDWORD c=0; c<clienthello->Extensions_GetAll()->GetSize(); c++)
    {
      DIOSTREAMTLS_MSG_EXTENSION* extension = clienthello->Extensions_GetAll()->Get(c);
      if(!extension) return false;

      if(extension->GetType() == DIOSTREAMTLS_MSG_EXTENSION_TYPE_PSKKEYEXCHANGEMODES)
        {
          if(modes) return false;
          modes = (DIOSTREAMTLS_MSG_EXTENSION_PSKKEYEXCHANGEMODES*)extension;
        }

      if(extension->GetType() == DIOSTREAMTLS_MSG_EXTENSION_TYPE_EARLYDATA)
        {
          if(earlyextension || state==DIOSTREAMTLS13HANDSHAKESERVER_STATE_WAIT_CLIENTHELLO_RETRY) return false;
          earlyextension=(DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN*)extension;
          if(!earlyextension->GetData()->IsEmpty()) return false;
        }

      if(extension->GetType() == DIOSTREAMTLS_MSG_EXTENSION_TYPE_PRESHAREDKEY)
        {
          if(offeredpsk || c != (clienthello->Extensions_GetAll()->GetSize()-1)) return false;
          offeredpsk = (DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY*)extension;
        }
    }

  if(!modes || !offeredpsk || offeredpsk->Identities_GetAll()->IsEmpty() ||
     (offeredpsk->Identities_GetAll()->GetSize() != offeredpsk->Binders_GetAll()->GetSize())) return false;

  bool pskdhe = false;
  for(XDWORD c=0; c<modes->List_Get()->GetSize(); c++)
    {
      if(modes->List_Get()->Get(c) == DIOSTREAMTLS_MSG_PSKKEYEXCHANGEMODE_PSK_DHE) { pskdhe = true; break; }
    }
  if(!pskdhe) return false;

  for(XDWORD c=0; c<offeredpsk->Identities_GetAll()->GetSize(); c++)
    {
      // TLS 1.3 binds early_data to identity 0, but ordinary 1-RTT resumption may select any offered identity.
      // If early_data is present, identities after 0 remain eligible only for 1-RTT and must not authorize early data.
      DIOSTREAMTLS_MSG_EXTENSION_PSKIDENTITY* identity = offeredpsk->Identities_GetAll()->Get(c);
      XBUFFER* receivedbinder = offeredpsk->Binders_GetAll()->Get(c);
      if(!identity || !receivedbinder) continue;

      XBUFFER ticketpsk;
      XWORD ticketcipher = 0;
      XBUFFER ticketprotocol;
      XSTRING ticketservername;
      XQWORD issueepoch = 0;
      XDWORD lifetime = 0;
      XDWORD ageadd = 0;
      XDWORD ticketmaximumearlydata = 0;

      if(!config->SessionTicket_OpenRaw((*identity->GetIdentity()), ticketpsk, ticketcipher, ticketprotocol,
                                     ticketservername, issueepoch, lifetime, ageadd, ticketmaximumearlydata)) continue;

      bool servernamematch    = servername && servername[0] ? !ticketservername.Compare(servername, true) : ticketservername.IsEmpty();
      bool protocolmatch      = applicationprotocol?ticketprotocol.Compare((*applicationprotocol)):ticketprotocol.IsEmpty();
      bool clientcipherfound  = false;
      bool servercipherallowed = false;

      for(XDWORD d=0; d<clienthello->GetCipherSuites()->GetSize(); d++)
        {
          if(clienthello->GetCipherSuites()->Get(d) == ticketcipher) { clientcipherfound = true; break; }
        }

      // A ticket authenticates the cipher suite used to derive its PSK, but it must not bypass a later server policy
      // change.  Resumption is therefore allowed only when that exact TLS 1.3 suite is still enabled locally as well
      // as being offered by the client.  Exact-suite matching also preserves the PSK hash binding required by TLS 1.3.
      if(config->GetCipherSuites())
        {
          for(XDWORD d=0; d<config->GetCipherSuites()->GetSize(); d++)
            {
              if(config->GetCipherSuites()->Get(d) == ticketcipher) { servercipherallowed = true; break; }
            }
        }

      if(!servernamematch || !protocolmatch || !clientcipherfound || !servercipherallowed)
        {
          if(!ticketpsk.IsEmpty()) ticketpsk.FillBuffer(0);
          continue;
        }

      // Ticket freshness itself is authenticated and enforced by SessionTicket_Open(). The obfuscated ticket age is
      // used only to decide whether 0-RTT is fresh enough for anti-replay processing; an age skew must not reject an
      // otherwise valid 1-RTT resumption PSK.
      (void)issueepoch;
      (void)lifetime;
      (void)ageadd;

      DIOSTREAMTLS13KEYSCHEDULE binderkeyschedule;
      if(!binderkeyschedule.Ini(ticketcipher, DIOSTREAMTLSKEYSCHEDULE_ROLE_SERVER) ||
         !binderkeyschedule.EarlySecret_Calculate(&ticketpsk) ||
         (receivedbinder->GetSize() != binderkeyschedule.GetHashSize()))
        {
          ticketpsk.FillBuffer(0);
          continue;
        }

      XDWORD removesize = sizeof(XWORD) + offeredpsk->Binders_GetLength();
      if(clienthellobuffer.GetSize() <= removesize)
        {
          ticketpsk.FillBuffer(0);
          continue;
        }

      XBUFFER truncatedclienthello;
      XBUFFER bindertranscript;
      XBUFFER calculatedbinder;
      if(!truncatedclienthello.Add(clienthellobuffer.Get(), clienthellobuffer.GetSize()-removesize))
        {
          ticketpsk.FillBuffer(0);
          return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_INTERNAL_ERROR);
        }

      if(state == DIOSTREAMTLS13HANDSHAKESERVER_STATE_WAIT_CLIENTHELLO_RETRY)
        {
          if(!bindertranscript.Add((*session->GetTranscript())) || !bindertranscript.Add(truncatedclienthello))
            {
              ticketpsk.FillBuffer(0);
              return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_INTERNAL_ERROR);
            }
        }
       else
        {
          if(!bindertranscript.Add(truncatedclienthello))
            {
              ticketpsk.FillBuffer(0);
              return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_INTERNAL_ERROR);
            }
        }

      if(!binderkeyschedule.Binder_Calculate(bindertranscript, calculatedbinder))
        {
          ticketpsk.FillBuffer(0);
          return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_INTERNAL_ERROR);
        }

      // Once a known, compatible PSK identity has been selected, an invalid binder is a fatal handshake error. Falling
      // back to certificate authentication here would ignore a failed proof of possession of a PSK that GEN recognized.
      if(!CIPHER::CompareConstantTime(calculatedbinder, (*receivedbinder)))
        {
          ticketpsk.FillBuffer(0);
          return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_DECRYPT_ERROR);
        }

      PSK.SecureDelete();
      if(!PSK.Add(ticketpsk))
        {
          ticketpsk.FillBuffer(0);
          return false;
        }

      earlydataoffered = (earlyextension != NULL) && (c == 0);
      earlydataaccepted = false;
      // Keep the authenticated ticket limit even when local policy rejects 0-RTT: the peer may already have sent
      // early records before learning the rejection and GEN must authenticate/discard them within the ticket bound.
      maximumearlydatasize = earlydataoffered ? ticketmaximumearlydata : 0;
      earlydataticketidentity.Delete();
      if(earlydataoffered && ticketmaximumearlydata && config->IsEarlyDataAcceptable())
        {
          XDWORD allowed=config->GetMaximumEarlyDataSize();
          if(allowed>ticketmaximumearlydata) allowed=ticketmaximumearlydata;
          if(allowed && config->EarlyDataTicketAge_IsAcceptable(issueepoch, identity->GetObfuscatedTicketAge(), ageadd) &&
             earlydataticketidentity.Add((*identity->GetIdentity())) && config->EarlyDataReplayCheck(earlydataticketidentity))
            {
              earlydataaccepted=true;
              maximumearlydatasize=allowed;
            }
        }

      ticketpsk.FillBuffer(0);
      ciphersuite = ticketcipher;
      return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKESERVER::NewSessionTicket_Create()
* @brief      Issue one post-handshake ticket for TLS 1.3 PSK-DHE resumption
* @note       INTERNAL. early_data is advertised only when explicitly enabled and an anti-replay callback is installed.
* @ingroup    DATAIO
*
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKESERVER::NewSessionTicket_Create()
{
  if(!config || !config->IsSessionResumptionActive() ||
     (config->GetClientAuthenticationMode() != DIOSTREAMTLS_CLIENTAUTHENTICATION_MODE_NONE)) return true;

  XBYTE noncebytes[8];
  XBYTE agebytes[4];
  XRAND* random = GEN_XFACTORY.CreateRand();
  if(!random || !random->IsCryptographicallySecure())
    {
      if(random) GEN_XFACTORY.DeleteRand(random);
      return false;
    }

  bool status = random->Ini();
  if(status) status = random->Generate(noncebytes, sizeof(noncebytes));
  if(status) status = random->Generate(agebytes, sizeof(agebytes));
  GEN_XFACTORY.DeleteRand(random);
  if(!status) return false;

  XBUFFER nonce;
  XSECUREBUFFER PSK;
  XBUFFER ticket;
  XDWORD ageadd = ((XDWORD)agebytes[0] << 24) | ((XDWORD)agebytes[1] << 16) | ((XDWORD)agebytes[2] << 8) | agebytes[3];

  if(!nonce.Add(noncebytes, sizeof(noncebytes)) ||
     !session->GetKeySchedule()->ResumptionPSK_Calculate(nonce, PSK) ||
     !config->SessionTicket_SealRaw(PSK, session->GetKeySchedule()->GetCipherSuite(),
                                 applicationprotocolnegotiated?&applicationprotocolraw:NULL,
                                 negotiatedservername.IsEmpty()?NULL:negotiatedservername.Get(),
                                 config->GetSessionTicketLifetime(), ageadd, ticket,
                                 config->IsEarlyDataAcceptable()?config->GetMaximumEarlyDataSize():0))
    {
      if(!PSK.IsEmpty()) PSK.FillBuffer(0);
      return false;
    }

  DIOSTREAMTLS_MSG_FRAGMENT<DIOSTREAMTLS_MSG_HANDSHAKE_NEWSESSIONTICKET> message;
  XBUFFER messagebuffer;
  XBUFFER records;

  message.SetMsgType(DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_NEW_SESSION_TICKET);
  message.GetBody()->SetTicketLifetime(config->GetSessionTicketLifetime());
  message.GetBody()->SetTicketAgeAdd(ageadd);
  message.GetBody()->SetMaximumEarlyDataSize(config->IsEarlyDataAcceptable()?config->GetMaximumEarlyDataSize():0);

  status = message.GetBody()->GetTicketNonce()->Add(nonce) &&
           message.GetBody()->GetTicket()->Add(ticket) &&
           message.SetToBuffer(messagebuffer, false) &&
           session->GetRecord()->Protect(DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE, messagebuffer, records) &&
           session->PostHandshakeOutput_Add(records);

  if(!PSK.IsEmpty()) PSK.FillBuffer(0);
  PSK.SecureDelete();
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKESERVER::ClientHello_Process(XBUFFER& clienthello, XBUFFER& records)
* @brief      Process a ClientHello and produce either HelloRetryRequest or the complete server flight
* @ingroup    DATAIO
*
* @param[in]  clienthello : Complete encoded ClientHello, as received (with its handshake header).
* @param[out] records : TLS records ready for transport. On retry this contains the clear-text
*             HelloRetryRequest; otherwise it contains ServerHello followed by EncryptedExtensions, Certificate,
*             CertificateVerify and Finished protected under the handshake traffic keys.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKESERVER::ClientHello_Process(XBUFFER& clienthello, XBUFFER& records)
{
  DIOSTREAMTLS_MSG_FRAGMENT<DIOSTREAMTLS_MSG_HANDSHAKE_CLIENTHELLO> clienthellomessage;
  XBUFFER                                                          workbuffer;
  XVECTOR<XWORD>                                                   offeredciphersuites;
  XVECTOR<XWORD>                                                   offeredsignatureschemes;
  XVECTOR<XWORD>                                                   offeredcertificatesignatureschemes;
  DIOSTREAMTLS_MSG_EXTENSION_ALPN*                                 offeredapplicationprotocols = NULL;
  bool                                                              signaturealgorithmsfound = false;
  bool                                                              certificatesignaturealgorithmsfound = false;
  bool                                                              supportedversionsfound = false;
  bool                                                              alpnfound              = false;
  bool                                                              servernamefound        = false;
  bool                                                              presharedkeyfound      = false;
  bool                                                              pskmodesfound          = false;
  bool                                                              statusrequestfound     = false;
  XSTRING                                                           requestedservername;
  DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDGROUPS*                       supportedgroups         = NULL;
  DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE*                              clientkeyshare          = NULL;
  bool                                                              helloretryrequestrequired = false;
  bool                                                              invalidkeyshare            = false;
  bool                                                              isretry                 = (state == DIOSTREAMTLS13HANDSHAKESERVER_STATE_WAIT_CLIENTHELLO_RETRY);
  XWORD                                                             ciphersuite;
  XWORD                                                             group;
  XBUFFER                                                           peerpublickey;
  XBUFFER                                                           serverpublickey;
  XSECUREBUFFER                                                     sharedsecret;
  XWORD                                                             signaturescheme;
  XVECTOR<XBUFFER*>*                                                localcertificatechain   = NULL;
  CIPHERKEY*                                                        localprivatekey         = NULL;
  XBUFFER*                                                          localOCSPresponse       = NULL;
  CIPHERCERTIFICATEX509                                            leafcertificate;
  CIPHERCERTIFICATEX509                                            issuercertificate;
  bool                                                              sendOCSPresponse        = false;
  XRAND*                                                            xrand;
  XBYTE                                                             random[DIOSTREAMTLS_MSG_RANDOM_SIZE];
  bool                                                              status;

  if(!isini || !session ||
     ((state != DIOSTREAMTLS13HANDSHAKESERVER_STATE_NONE) &&
      (state != DIOSTREAMTLS13HANDSHAKESERVER_STATE_WAIT_CLIENTHELLO_RETRY)) ||
     (!isretry && !session->GetTranscript()->IsEmpty()) ||
     (isretry && (session->GetTranscript()->IsEmpty() || firstclienthello.IsEmpty())) ||
     (&clienthello == &records))
    {
      return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_UNEXPECTED_MESSAGE);
    }

  records.Delete();

  // ClientHello2 after HRR is sent in cleartext. 0-RTT is rejected by HRR, so remove the first
  // ClientHello's early receive keys before processing the retried ClientHello.
  if(isretry && session->GetEpoch(DIOSTREAMTLSKEYSCHEDULE_DIRECTION_REMOTE)==DIOSTREAMTLS13SESSION_EPOCH_EARLY)
    {
      session->EarlyKeys_Deactivate(DIOSTREAMTLSKEYSCHEDULE_DIRECTION_REMOTE);
    }

  workbuffer.Add(clienthello);

  if(!clienthellomessage.GetFromBuffer(workbuffer, false) || !workbuffer.IsEmpty() ||
     (clienthellomessage.GetMsgType() != DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_CLIENT_HELLO))
    {
      return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_DECODE_ERROR);
    }

  DIOSTREAMTLS_MSG_HANDSHAKE_CLIENTHELLO* body = clienthellomessage.GetBody();

  for(XDWORD c=0; c<body->GetCipherSuites()->GetSize(); c++)
    {
      if(!offeredciphersuites.Add(body->GetCipherSuites()->Get(c))) return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_INTERNAL_ERROR);
    }

  for(XDWORD c=0; c<body->Extensions_GetAll()->GetSize(); c++)
    {
      DIOSTREAMTLS_MSG_EXTENSION* extension = body->Extensions_GetAll()->Get(c);
      if(!extension) return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_DECODE_ERROR);

      if(extension->GetType() == DIOSTREAMTLS_MSG_EXTENSION_TYPE_PRESHAREDKEY)
        {
          presharedkeyfound = true;
          if(c != (body->Extensions_GetAll()->GetSize()-1))
            {
              return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_ILLEGAL_PARAMETER);
            }
        }

      if(extension->GetType() == DIOSTREAMTLS_MSG_EXTENSION_TYPE_PSKKEYEXCHANGEMODES)
        {
          pskmodesfound = true;
        }

      if(extension->GetType() == DIOSTREAMTLS_MSG_EXTENSION_TYPE_SUPPORTEDVERSIONS)
        {
          DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS* versions = (DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS*)extension;

          for(XDWORD d=0; d<versions->List_Get()->GetSize(); d++)
            {
              if(versions->List_Get()->Get(d) == DIOSTREAMTLS_MSG_VERSION_TLS_1_3) supportedversionsfound = true;
            }
        }

      if(extension->GetType() == DIOSTREAMTLS_MSG_EXTENSION_TYPE_SIGNATUREALGORITHMS)
        {
          if(signaturealgorithmsfound) return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_ILLEGAL_PARAMETER);
          signaturealgorithmsfound = true;

          DIOSTREAMTLS_MSG_EXTENSION_SIGNATUREALGORITHMS* algorithms = (DIOSTREAMTLS_MSG_EXTENSION_SIGNATUREALGORITHMS*)extension;

          for(XDWORD d=0; d<algorithms->List_Get()->GetSize(); d++)
            {
              if(!offeredsignatureschemes.Add(algorithms->List_Get()->Get(d))) return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_INTERNAL_ERROR);
            }
        }

      if(extension->GetType() == DIOSTREAMTLS_MSG_EXTENSION_TYPE_SIGNATUREALGORITHMSCERT)
        {
          if(certificatesignaturealgorithmsfound) return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_ILLEGAL_PARAMETER);
          certificatesignaturealgorithmsfound = true;

          DIOSTREAMTLS_MSG_EXTENSION_SIGNATUREALGORITHMSCERT* algorithms = (DIOSTREAMTLS_MSG_EXTENSION_SIGNATUREALGORITHMSCERT*)extension;

          for(XDWORD d=0; d<algorithms->List_Get()->GetSize(); d++)
            {
              if(!offeredcertificatesignatureschemes.Add(algorithms->List_Get()->Get(d))) return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_INTERNAL_ERROR);
            }
        }

      if(extension->GetType() == DIOSTREAMTLS_MSG_EXTENSION_TYPE_SUPPORTEDGROUPS)
        {
          supportedgroups = (DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDGROUPS*)extension;
        }

      if(extension->GetType() == DIOSTREAMTLS_MSG_EXTENSION_TYPE_KEYSHARE)
        {
          clientkeyshare = (DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE*)extension;
        }

      if(extension->GetType() == DIOSTREAMTLS_MSG_EXTENSION_TYPE_SNI)
        {
          DIOSTREAMTLS_MSG_EXTENSION_SNI* SNI = (DIOSTREAMTLS_MSG_EXTENSION_SNI*)extension;

          for(XDWORD d=0; d<SNI->List_Get()->GetSize(); d++)
            {
              DIOSTREAMTLS_MSG_EXTENSION_SNI_SERVERNAME* name = SNI->List_Get()->Get(d);
              if(!name) return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_DECODE_ERROR);

              // RFC 6066 currently defines host_name as name_type 0. Unknown future name types are ignored.
              if(name->Name_GetType() != 0) continue;
              if(servernamefound || name->Name_GetHost()->IsEmpty())
                {
                  return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_ILLEGAL_PARAMETER);
                }

              requestedservername.Set(name->Name_GetHost()->Get());
              servernamefound = true;
            }
        }

      if(extension->GetType() == DIOSTREAMTLS_MSG_EXTENSION_TYPE_ALPN)
        {
          DIOSTREAMTLS_MSG_EXTENSION_ALPN* ALPN = (DIOSTREAMTLS_MSG_EXTENSION_ALPN*)extension;

          alpnfound = true;
          if(offeredapplicationprotocols || !ALPN->List_GetNProtocols()) return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_DECODE_ERROR);
          offeredapplicationprotocols = ALPN;
        }

      if(extension->GetType() == DIOSTREAMTLS_MSG_EXTENSION_TYPE_STATUSREQUEST)
        {
          DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN* OCSPrequest = (DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN*)extension;
          bool                                 OCSPrequested;

          if(statusrequestfound || !DIOSTREAMTLS13_HANDSHAKESERVER_OCSPRequest_Parse(OCSPrequest->GetData(), OCSPrequested))
            {
              return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_DECODE_ERROR);
            }

          statusrequestfound = true;
          serverOCSPstaplingrequested = OCSPrequested;
        }
    }

  if(!statusrequestfound) serverOCSPstaplingrequested = false;

  // RFC 8446: when signature_algorithms_cert is absent, signature_algorithms also constrains certificate signatures.
  if(!certificatesignaturealgorithmsfound)
    {
      for(XDWORD c=0; c<offeredsignatureschemes.GetSize(); c++)
        {
          if(!offeredcertificatesignatureschemes.Add(offeredsignatureschemes.Get(c))) return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_INTERNAL_ERROR);
        }
    }

  // This server is TLS 1.3-only.  Version negotiation must be resolved before applying TLS 1.3-specific
  // mandatory-extension rules; otherwise an older ClientHello could incorrectly receive missing_extension.
  if(!supportedversionsfound)
    {
      return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_PROTOCOL_VERSION);
    }

  if(presharedkeyfound && !pskmodesfound)
    {
      return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_MISSING_EXTENSION);
    }

  // RFC 8446 fixes these legacy ClientHello fields for TLS 1.3.  Accepting any other value makes the
  // message syntactically decodable but semantically invalid.
  if((body->GetClientVersion() != DIOSTREAMTLS_MSG_VERSION_TLS_1_2) ||
     (body->GetCompressionLength() != 1) ||
     (body->GetCompressionMethod() != DIOSTREAMTLS_MSG_COMPRESS_METHOD_NULL))
    {
      return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_ILLEGAL_PARAMETER);
    }

  // Every KeyShareEntry must name a group from supported_groups and must preserve the order in which those
  // groups were advertised.  The vector itself may be empty in ClientHello1; that valid case is what allows
  // the server to select a group with HelloRetryRequest.
  if(!supportedgroups || !clientkeyshare)
    {
      return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_MISSING_EXTENSION);
    }

  if(!supportedgroups->List_Get()->GetSize())
    {
      return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_ILLEGAL_PARAMETER);
    }

  XDWORD lastgroupindex = 0;
  bool   haslastgroup   = false;

  for(XDWORD c=0; c<clientkeyshare->List_Get()->GetSize(); c++)
    {
      DIOSTREAMTLS_MSG_EXTENSION_KEY* key = clientkeyshare->List_Get()->Get(c);
      if(!key)
        {
          return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_ILLEGAL_PARAMETER);
        }

      bool   groupfound = false;
      XDWORD groupindex = 0;

      for(XDWORD d=0; d<supportedgroups->List_Get()->GetSize(); d++)
        {
          if(supportedgroups->List_Get()->Get(d) == key->GetKeyType())
            {
              groupfound = true;
              groupindex = d;
              break;
            }
        }

      if(!groupfound || (haslastgroup && (groupindex <= lastgroupindex)))
        {
          return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_ILLEGAL_PARAMETER);
        }

      lastgroupindex = groupindex;
      haslastgroup   = true;
    }

  ApplicationProtocol_Select(offeredapplicationprotocols);
  if(alpnfound && !applicationprotocolnegotiated)
    {
      return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_NO_APPLICATION_PROTOCOL);
    }

  if(!Group_Select(body, group, peerpublickey, helloretryrequestrequired, invalidkeyshare))
    {
      return SetError(invalidkeyshare?DIOSTREAMTLS_ALERT_DESCRIPTION_ILLEGAL_PARAMETER: DIOSTREAMTLS_ALERT_DESCRIPTION_HANDSHAKE_FAILURE);
    }

  resumptionaccepted = false;
  if(!resumptionpsk.IsEmpty()) resumptionpsk.FillBuffer(0);
  resumptionpsk.Delete();

  if(presharedkeyfound)
    {
      resumptionaccepted = ResumptionPSK_Select(body, clienthello, servernamefound?requestedservername.Get():NULL, applicationprotocolnegotiated?&applicationprotocolraw:NULL, ciphersuite, resumptionpsk);

      if(state == DIOSTREAMTLS13HANDSHAKESERVER_STATE_ERROR) return false;
    }

  if(!resumptionaccepted)
    {
      // A full certificate handshake needs signature_algorithms and usable local credentials. A ClientHello may
      // omit signature_algorithms only when a valid resumption PSK is actually selected.
      if(offeredsignatureschemes.IsEmpty())
        {
          return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_MISSING_EXTENSION);
        }

      if(!CipherSuite_Select(offeredciphersuites, ciphersuite) ||
         !ServerCredentials_Select(servernamefound?requestedservername.Get():NULL, offeredsignatureschemes,
                                   offeredcertificatesignatureschemes, localcertificatechain, localprivatekey,
                                   localOCSPresponse, leafcertificate, signaturescheme) ||
         !localcertificatechain || localcertificatechain->IsEmpty() || !localprivatekey)
        {
          return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_HANDSHAKE_FAILURE);
        }

      if(serverOCSPstaplingrequested && localOCSPresponse && !localOCSPresponse->IsEmpty() &&
         (localcertificatechain->GetSize() >= 2) && localcertificatechain->Get(1) &&
         issuercertificate.Decode((*localcertificatechain->Get(1))))
        {
          CIPHERCERTIFICATEX509REVOCATION_RESULT OCSPresult = CIPHERCERTIFICATEX509REVOCATION::ValidateOCSP((*localOCSPresponse), leafcertificate, issuercertificate);

          if(OCSPresult == CIPHERCERTIFICATEX509REVOCATION_RESULT_REVOKED)
            {
              return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_CERTIFICATE_REVOKED);
            }

          sendOCSPresponse = (OCSPresult == CIPHERCERTIFICATEX509REVOCATION_RESULT_GOOD);
        }
    }

  if(isretry)
    {
      if(!ClientHelloRetry_Validate(body) || helloretryrequestrequired ||
         (group != retryselectedgroup) || (ciphersuite != retryciphersuite))
        {
          return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_ILLEGAL_PARAMETER);
        }
    }

  negotiatedservername.Empty();
  if(servernamefound) negotiatedservername.Set(requestedservername.Get());

  if(!isretry && helloretryrequestrequired)
    {
      // RFC 8446 forbids accepting 0-RTT after HelloRetryRequest, but early records already sent after ClientHello1
      // may still be queued on TCP.  If the PSK was valid, derive the CH1 early read keys only to authenticate and
      // discard those records while waiting for ClientHello2.  No application bytes are delivered.
      if(resumptionaccepted && earlydataoffered)
        {
          if(!session->CipherSuite_Select(ciphersuite) ||
             !session->EarlyKeys_Activate(resumptionpsk, clienthello, DIOSTREAMTLSKEYSCHEDULE_DIRECTION_REMOTE))
            return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_INTERNAL_ERROR);
          session->EarlyData_Accepted(false);
          earlydataaccepted=false;
        }
      return HelloRetryRequest_Create(body, clienthello, ciphersuite, group, records);
    }

  bool invalidpeershare = false;
  if(!session->KeyExchange_ServerGenerate(group, peerpublickey, serverpublickey, sharedsecret, invalidpeershare))
    {
      sharedsecret.FillBuffer(0);
      return SetError(invalidpeershare?DIOSTREAMTLS_ALERT_DESCRIPTION_ILLEGAL_PARAMETER: DIOSTREAMTLS_ALERT_DESCRIPTION_INTERNAL_ERROR);
    }

  if(!session->CipherSuite_Select(ciphersuite))
    {
      sharedsecret.FillBuffer(0);
      return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_INTERNAL_ERROR);
    }

  if(resumptionaccepted && earlydataoffered)
    {
      XDWORD earlylimit=maximumearlydatasize;
      if(!earlylimit)
        {
          earlylimit=config->GetMaximumEarlyDataSize();
          if(!earlylimit) earlylimit=DIOSTREAMTLS13_EARLYDATA_MAXSIZE;
        }
      if(!session->EarlyKeys_Activate(resumptionpsk, clienthello, DIOSTREAMTLSKEYSCHEDULE_DIRECTION_REMOTE) ||
         !session->EarlyData_Limit(earlylimit))
        {
          sharedsecret.FillBuffer(0);
          return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_INTERNAL_ERROR);
        }
      session->EarlyData_Accepted(earlydataaccepted);
    }

  if(!session->Transcript_Add(clienthello))
    {
      sharedsecret.FillBuffer(0);
      return SetError();
    }

  // ---- ServerHello (cleartext: local direction is still epoch CLEAR at this point) --------------------------

  xrand = GEN_XFACTORY.CreateRand();
  if(!xrand || !xrand->IsCryptographicallySecure())
    {
      if(xrand) GEN_XFACTORY.DeleteRand(xrand);
      sharedsecret.FillBuffer(0);
      return SetError();
    }

  status = xrand->Ini();
  if(status) status = xrand->Generate(random, sizeof(random));

  GEN_XFACTORY.DeleteRand(xrand);

  if(!status)
    {
      sharedsecret.FillBuffer(0);
      return SetError();
    }

  DIOSTREAMTLS_MSG_FRAGMENT<DIOSTREAMTLS_MSG_HANDSHAKE_SERVERHELLO> serverhellomessage;
  DIOSTREAMTLS_MSG_HANDSHAKE_SERVERHELLO*                           serverhellobody;
  XBUFFER                                                           serverhello;

  serverhellomessage.SetMsgType(DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_SERVER_HELLO);

  serverhellobody = serverhellomessage.GetBody();
  if(!serverhellobody)
    {
      sharedsecret.FillBuffer(0);
      return SetError();
    }

  serverhellobody->SetLegacyVersion(DIOSTREAMTLS_MSG_VERSION_TLS_1_2);
  memcpy(serverhellobody->GetRandom(), random, sizeof(random));

  // Echo the client's legacy_session_id back for middlebox compatibility mode (RFC 8446 4.1.3).
  serverhellobody->SetSessionIDLength(body->GetSessionIDLength());
  if(body->GetSessionIDLength()) memcpy(serverhellobody->GetSessionID(), body->GetSessionID(), body->GetSessionIDLength());

  serverhellobody->SetCipherSuite(ciphersuite);
  serverhellobody->SetCompressionMethod(DIOSTREAMTLS_MSG_COMPRESS_METHOD_NULL);

  DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS_SERVER* supportedversions = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_SUPPORTEDVERSIONS_SERVER();
  if(!supportedversions)
    {
      sharedsecret.FillBuffer(0);
      return SetError();
    }

  supportedversions->SetVersion(DIOSTREAMTLS_MSG_VERSION_TLS_1_3);

  if(!serverhellobody->Extensions_Add(supportedversions))
    {
      GEN_DELETE supportedversions;
      sharedsecret.FillBuffer(0);
      return SetError();
    }

  DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_SERVER* keyshare = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_KEYSHARE_SERVER();
  if(!keyshare)
    {
      sharedsecret.FillBuffer(0);
      return SetError();
    }

  keyshare->GetKey()->SetKeyType(group);

  if(!keyshare->GetKey()->GetKeyData()->Add(serverpublickey) || !serverhellobody->Extensions_Add(keyshare))
    {
      GEN_DELETE keyshare;
      sharedsecret.FillBuffer(0);
      return SetError();
    }

  if(resumptionaccepted)
    {
      DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY_SERVER* selectedpsk = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_PRESHAREDKEY_SERVER();
      if(!selectedpsk) { sharedsecret.FillBuffer(0); return SetError(); }

      selectedpsk->SetSelectedIdentity(0);
      if(!serverhellobody->Extensions_Add(selectedpsk))
        {
          GEN_DELETE selectedpsk;
          sharedsecret.FillBuffer(0);
          return SetError();
        }
    }

  if(!serverhellomessage.SetToBuffer(serverhello, false) ||
     !session->GetRecord()->Protect(DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE, serverhello, records) ||
     !session->Transcript_Add(serverhello) ||
     !session->HandshakeKeys_Activate(sharedsecret, resumptionaccepted?&resumptionpsk:NULL))
    {
      sharedsecret.FillBuffer(0);
      records.Delete();
      return SetError();
    }

  sharedsecret.FillBuffer(0);
  sharedsecret.Delete();

  // ---- EncryptedExtensions, Certificate, CertificateVerify, Finished (protected under handshake keys) -------

  DIOSTREAMTLS_MSG_FRAGMENT<DIOSTREAMTLS_MSG_HANDSHAKE_ENCRYPTEDEXTENSIONS> encryptedextensionsmessage;
  XBUFFER                                                                   encryptedextensions;
  XBUFFER                                                                   flightrecords;

  encryptedextensionsmessage.SetMsgType(DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_ENCRYPTED_EXTENSIONS);

  if(servernamefound)
    {
      // RFC 6066 acknowledgement: when SNI is used for certificate selection, the server sends an empty
      // server_name extension. In TLS 1.3 this acknowledgement is carried in EncryptedExtensions.
      DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN* SNIack = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN();
      if(!SNIack) return SetError();

      SNIack->SetType(DIOSTREAMTLS_MSG_EXTENSION_TYPE_SNI);

      if(!encryptedextensionsmessage.GetBody()->Extensions_Add(SNIack))
        {
          GEN_DELETE SNIack;
          return SetError();
        }
    }

  if(applicationprotocolnegotiated)
    {
      DIOSTREAMTLS_MSG_EXTENSION_ALPN* ALPN = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_ALPN();
      if(!ALPN) return SetError();

      if(!ALPN->List_Add(applicationprotocolraw) || !encryptedextensionsmessage.GetBody()->Extensions_Add(ALPN))
        {
          GEN_DELETE ALPN;
          return SetError();
        }
    }

  if(earlydataaccepted)
    {
      DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN* earlyack=GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN();
      if(!earlyack) return SetError();
      earlyack->SetType(DIOSTREAMTLS_MSG_EXTENSION_TYPE_EARLYDATA);
      if(!encryptedextensionsmessage.GetBody()->Extensions_Add(earlyack)) { GEN_DELETE earlyack; return SetError(); }
    }

  if(!encryptedextensionsmessage.SetToBuffer(encryptedextensions, false) ||
     !session->GetRecord()->Protect(DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE, encryptedextensions, flightrecords) ||
     !session->Transcript_Add(encryptedextensions) || !records.Add(flightrecords))
    {
      records.Delete();
      return SetError();
    }

  if(!resumptionaccepted && (config->GetClientAuthenticationMode() != DIOSTREAMTLS_CLIENTAUTHENTICATION_MODE_NONE))
    {
      DIOSTREAMTLS_MSG_FRAGMENT<DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEREQUEST> certificaterequest;
      DIOSTREAMTLS_MSG_EXTENSION_SIGNATUREALGORITHMS*                         algorithms;
      DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN*                                     statusrequest;
      XBUFFER                                                                 certificaterequestbuffer;

      certificaterequest.SetMsgType(DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_CERTIFICATE_REQUEST);
      if(!certificaterequest.GetBody()->SetRequestContext(NULL, 0)) return SetError();

      algorithms = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_SIGNATUREALGORITHMS();
      if(!algorithms) return SetError();

      for(XDWORD c=0; c<config->GetSignatureSchemes()->GetSize(); c++)
        {
          if(!algorithms->List_Add(config->GetSignatureSchemes()->Get(c)))
            {
              GEN_DELETE algorithms;
              return SetError();
            }
        }

      if(!certificaterequest.GetBody()->Extensions_Add(algorithms))
        {
          GEN_DELETE algorithms;
          return SetError();
        }

      DIOSTREAMTLS_MSG_EXTENSION_SIGNATUREALGORITHMSCERT* certificationalgorithms = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_SIGNATUREALGORITHMSCERT();
      if(!certificationalgorithms) return SetError();
      for(XDWORD c=0; c<config->GetCertificateSignatureSchemes()->GetSize(); c++)
        if(!certificationalgorithms->List_Add(config->GetCertificateSignatureSchemes()->Get(c)))
          { GEN_DELETE certificationalgorithms; return SetError(); }
      if(!certificaterequest.GetBody()->Extensions_Add(certificationalgorithms))
        { GEN_DELETE certificationalgorithms; return SetError(); }

      if(config->GetClientTrustedRoots() && !config->GetClientTrustedRoots()->IsEmpty())
        {
          DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN* authorities=GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN();
          XBUFFER authoritylist;
          if(!authorities) return SetError();
          for(XDWORD c=0;c<config->GetClientTrustedRoots()->GetSize();c++)
            {
              XBUFFER* rootDER=config->GetClientTrustedRoots()->Get(c);
              CIPHERCERTIFICATEX509 rootcertificate;
              if(!rootDER || !rootcertificate.Decode((*rootDER)) || !rootcertificate.GetSubjectData() || rootcertificate.GetSubjectData()->IsEmpty())
                { GEN_DELETE authorities; return SetError(); }
              XDWORD size=rootcertificate.GetSubjectData()->GetSize();
              if(size>0xFFFF || authoritylist.GetSize()+2+size>0xFFFF) { GEN_DELETE authorities; return SetError(); }
              if(!authoritylist.Add((XBYTE)((size>>8)&0xFF)) || !authoritylist.Add((XBYTE)(size&0xFF)) ||
                 !authoritylist.Add((*rootcertificate.GetSubjectData()))) { GEN_DELETE authorities; return SetError(); }
            }
          authorities->SetType(DIOSTREAMTLS_MSG_EXTENSION_TYPE_CERTIFICATEAUTHORITIES);
          XDWORD totalsize=authoritylist.GetSize();
          if(!authorities->GetData()->Add((XBYTE)((totalsize>>8)&0xFF)) || !authorities->GetData()->Add((XBYTE)(totalsize&0xFF)) ||
             !authorities->GetData()->Add(authoritylist) || !certificaterequest.GetBody()->Extensions_Add(authorities))
            { GEN_DELETE authorities; return SetError(); }
        }

      if(config->GetRevocationPolicy() != DIOSTREAMTLS_REVOCATIONPOLICY_OFF)
        {
          statusrequest = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN();
          if(!statusrequest) return SetError();

          statusrequest->SetType(DIOSTREAMTLS_MSG_EXTENSION_TYPE_STATUSREQUEST);
          if(!certificaterequest.GetBody()->Extensions_Add(statusrequest))
            {
              GEN_DELETE statusrequest;
              return SetError();
            }

          clientOCSPstaplingrequested = true;
        }
       else
        {
          clientOCSPstaplingrequested = false;
        }

      flightrecords.Delete();
      if(!certificaterequest.SetToBuffer(certificaterequestbuffer, false) ||
         !session->GetRecord()->Protect(DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE, certificaterequestbuffer, flightrecords) ||
         !session->Transcript_Add(certificaterequestbuffer) || !records.Add(flightrecords))
        {
          records.Delete();
          return SetError();
        }
    }

  if(!resumptionaccepted)
    {
  DIOSTREAMTLS_MSG_FRAGMENT<DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATE> certificatemessage;
  XBUFFER                                                          certificatebuffer;

  certificatemessage.SetMsgType(DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_CERTIFICATE);

  if(!certificatemessage.GetBody()->SetRequestContext(NULL, 0))
    {
      records.Delete();
      return SetError();
    }

  for(XDWORD c=0; c<localcertificatechain->GetSize(); c++)
    {
      XBUFFER* dercertificate = localcertificatechain->Get(c);
      DIOSTREAMTLS_MSG_CERTIFICATEENTRY* entry;

      if(!dercertificate)
        {
          records.Delete();
          return SetError();
        }

      entry = GEN_NEW DIOSTREAMTLS_MSG_CERTIFICATEENTRY();
      if(!entry)
        {
          records.Delete();
          return SetError();
        }

      if(!entry->GetCertificateData()->Add((*dercertificate)) || !certificatemessage.GetBody()->CertificateList_Add(entry))
        {
          GEN_DELETE entry;
          records.Delete();
          return SetError();
        }

      if(!c && sendOCSPresponse)
        {
          DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN* statusrequest = GEN_NEW DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN();
          XDWORD                               responsesize  = localOCSPresponse->GetSize();

          if(!statusrequest)
            {
              records.Delete();
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
              records.Delete();
              return SetError();
            }
        }
    }

  flightrecords.Delete();

  if(!certificatemessage.SetToBuffer(certificatebuffer, false) ||
     !session->GetRecord()->Protect(DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE, certificatebuffer, flightrecords) ||
     !session->Transcript_Add(certificatebuffer) || !records.Add(flightrecords))
    {
      records.Delete();
      return SetError();
    }

  static XBYTE certificateverifycontext[] = { 'T', 'L', 'S', ' ', '1', '.', '3', ',', ' ',
                                              's', 'e', 'r', 'v', 'e', 'r', ' ',
                                              'C', 'e', 'r', 't', 'i', 'f', 'i', 'c', 'a', 't', 'e', 'V', 'e', 'r', 'i', 'f', 'y' };

  DIOSTREAMTLS_MSG_FRAGMENT<DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEVERIFY> certificateverifymessage;
  XBUFFER                                                                certificateverifybuffer;
  XBUFFER                                                                transcripthash;
  XBUFFER                                                                signedcontent;
  XBUFFER                                                                signature;

  if(!session->TranscriptHash(transcripthash))
    {
      records.Delete();
      return SetError();
    }

  for(int c=0; c<64; c++)
    {
      if(!signedcontent.Add((XBYTE)0x20))
        {
          records.Delete();
          return SetError();
        }
    }

  if(!signedcontent.Add(certificateverifycontext, sizeof(certificateverifycontext)) || !signedcontent.Add((XBYTE)0x00) ||
     !signedcontent.Add(transcripthash) ||
     !DIOSTREAMTLSSIGNATURE::Sign(signaturescheme, localprivatekey, leafcertificate.GetPublicCipherKey(),
                                  signedcontent, signature))
    {
      records.Delete();
      return SetError();
    }

  certificateverifymessage.SetMsgType(DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_CERTIFICATE_VERIFY);
  certificateverifymessage.GetBody()->SetAlgorithm(signaturescheme);

  flightrecords.Delete();

  if(!certificateverifymessage.GetBody()->GetSignature()->Add(signature) ||
     !certificateverifymessage.SetToBuffer(certificateverifybuffer, false) ||
     !session->GetRecord()->Protect(DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE, certificateverifybuffer, flightrecords) ||
     !session->Transcript_Add(certificateverifybuffer) || !records.Add(flightrecords))
    {
      records.Delete();
      return SetError();
    }

    }

  DIOSTREAMTLS_MSG_FRAGMENT<DIOSTREAMTLS_MSG_HANDSHAKE_FINISHED> finishedmessage;
  XBUFFER                                                       finishedbuffer;
  XBUFFER                                                       finishedtranscripthash;
  XBUFFER                                                       verifydata;

  if(!session->TranscriptHash(finishedtranscripthash) ||
     !session->GetKeySchedule()->CalculateFinished(DIOSTREAMTLSKEYSCHEDULE_DIRECTION_LOCAL, finishedtranscripthash, verifydata))
    {
      records.Delete();
      return SetError();
    }

  finishedmessage.SetMsgType(DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_FINISHED);

  flightrecords.Delete();

  if(!finishedmessage.GetBody()->GetVerifyData()->Add(verifydata) ||
     !finishedmessage.SetToBuffer(finishedbuffer, false) ||
     !session->GetRecord()->Protect(DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE, finishedbuffer, flightrecords) ||
     !session->Transcript_Add(finishedbuffer) || !records.Add(flightrecords) ||
     !session->ApplicationKeys_Activate(DIOSTREAMTLSKEYSCHEDULE_DIRECTION_LOCAL))
    {
      records.Delete();
      return SetError();
    }

  firstclienthello.Delete();
  retryselectedgroup = 0;
  retryciphersuite   = 0;
  clientcertificateprovided = false;
  state = earlydataaccepted?DIOSTREAMTLS13HANDSHAKESERVER_STATE_WAIT_END_OF_EARLY_DATA:
          ((resumptionaccepted || (config->GetClientAuthenticationMode() == DIOSTREAMTLS_CLIENTAUTHENTICATION_MODE_NONE))? DIOSTREAMTLS13HANDSHAKESERVER_STATE_WAIT_FINISHED: DIOSTREAMTLS13HANDSHAKESERVER_STATE_WAIT_CLIENT_CERTIFICATE);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKESERVER::ClientCertificate_Process(XBUFFER& message)
* @brief      Decode and validate the client Certificate message requested for mTLS
* @note       INTERNAL
* @ingroup    DATAIO
* 
* @param[in]  message : Message value.
*
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKESERVER::ClientCertificate_Process(XBUFFER& message)
{
  DIOSTREAMTLS_MSG_FRAGMENT<DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATE> certificate;
  XBUFFER                                                          workbuffer;
  XVECTOR<XBUFFER*>                                                certificatechain;

  if(state != DIOSTREAMTLS13HANDSHAKESERVER_STATE_WAIT_CLIENT_CERTIFICATE)
    {
      return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_UNEXPECTED_MESSAGE);
    }

  workbuffer.Add(message);
  if(!certificate.GetFromBuffer(workbuffer, false) || !workbuffer.IsEmpty() ||
     (certificate.GetMsgType() != DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_CERTIFICATE) ||
     !certificate.GetBody()->GetRequestContext()->IsEmpty())
    {
      return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_DECODE_ERROR);
    }

  if(certificate.GetBody()->CertificateList_GetAll()->IsEmpty())
    {
      if(!session->Transcript_Add(message)) return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_INTERNAL_ERROR);

      if(config->GetClientAuthenticationMode() == DIOSTREAMTLS_CLIENTAUTHENTICATION_MODE_REQUIRED)
        {
          return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_CERTIFICATE_REQUIRED);
        }

      clientcertificateprovided = false;
      state = DIOSTREAMTLS13HANDSHAKESERVER_STATE_WAIT_FINISHED;
      return true;
    }

  for(XDWORD c=0; c<certificate.GetBody()->CertificateList_GetAll()->GetSize(); c++)
    {
      DIOSTREAMTLS_MSG_CERTIFICATEENTRY* entry = certificate.GetBody()->CertificateList_GetAll()->Get(c);
      if(!entry || !entry->GetCertificateData() || !certificatechain.Add(entry->GetCertificateData()))
        {
          return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_BAD_CERTIFICATE);
        }
    }

  if(!config->GetClientTrustedRoots() || config->GetClientTrustedRoots()->IsEmpty())
    {
      return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_INTERNAL_ERROR);
    }

  if(!clientcertificatevalidator.ValidateClientAllPaths(&certificatechain, config->GetClientTrustedRoots()))
    {
      return SetError(DIOSTREAMTLS13_HANDSHAKESERVER_CertificateAlert(clientcertificatevalidator.GetError()));
    }

  DIOSTREAMTLS_MSG_CERTIFICATEENTRY* leafentry = certificate.GetBody()->CertificateList_GetAll()->Get(0);
  bool                                hasstapledOCSP = false;

  if(!leafentry) return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_BAD_CERTIFICATE);

  for(XDWORD c=0; c<leafentry->Extensions_GetAll()->GetSize(); c++)
    {
      DIOSTREAMTLS_MSG_EXTENSION* extension = leafentry->Extensions_GetAll()->Get(c);
      if(!extension || (extension->GetType() != DIOSTREAMTLS_MSG_EXTENSION_TYPE_STATUSREQUEST)) continue;

      if(!clientOCSPstaplingrequested) return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_UNSUPPORTED_EXTENSION);
      if(hasstapledOCSP) return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_BAD_CERTIFICATE_STATUS_RESPONSE);

      DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN* status = (DIOSTREAMTLS_MSG_EXTENSION_UNKNOWN*)extension;
      XBUFFER*                            data   = status->GetData();
      XVECTOR<CIPHERCERTIFICATEX509*>*    validatedchain = clientcertificatevalidator.GetCertificateChain();

      hasstapledOCSP = true;
      if(!data || (data->GetSize() < 5) || (data->GetByte(0) != 1) ||
         ((((XDWORD)data->GetByte(1) << 16) | ((XDWORD)data->GetByte(2) << 8) | data->GetByte(3)) != (data->GetSize()-4)) ||
         ((data->GetSize()-4) > config->GetMemoryPolicy()->GetMaximumOCSPResponseSize()) ||
         !validatedchain || (validatedchain->GetSize() < 2))
        {
          return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_BAD_CERTIFICATE_STATUS_RESPONSE);
        }

      XBUFFER OCSPresponse;
      if(!OCSPresponse.Add(data->Get()+4, data->GetSize()-4))
        {
          return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_INTERNAL_ERROR);
        }

      CIPHERCERTIFICATEX509REVOCATION_RESULT OCSPresult = CIPHERCERTIFICATEX509REVOCATION::ValidateOCSP(OCSPresponse, (*validatedchain->Get(0)), (*validatedchain->Get(1)));

      if(OCSPresult == CIPHERCERTIFICATEX509REVOCATION_RESULT_REVOKED)
        {
          return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_CERTIFICATE_REVOKED);
        }

      if((OCSPresult != CIPHERCERTIFICATEX509REVOCATION_RESULT_GOOD) &&
         (config->GetRevocationPolicy() != DIOSTREAMTLS_REVOCATIONPOLICY_SOFT_FAIL))
        {
          return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_BAD_CERTIFICATE_STATUS_RESPONSE);
        }
    }

  const bool revocationenabled=(config->GetRevocationPolicy()!=DIOSTREAMTLS_REVOCATIONPOLICY_OFF);
  XVECTOR<CIPHERCERTIFICATEX509*>* validatedchain=clientcertificatevalidator.GetCertificateChain();

  if(revocationenabled && !hasstapledOCSP && config->GetOCSPDirectFetcher() && validatedchain && validatedchain->GetSize()>=2)
    {
      CIPHERCERTIFICATEX509* leaf=validatedchain->Get(0);
      CIPHERCERTIFICATEX509* issuer=validatedchain->Get(1);
      XBUFFER response;
      CIPHERCERTIFICATEX509REVOCATION_RESULT result=CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID;
      if(leaf && issuer && leaf->HasOCSPURL() &&
         config->GetOCSPDirectFetcher()((*leaf->GetOCSPURL()),(*leaf),(*issuer),response,config->GetOCSPDirectContext()) &&
         response.GetSize()<=config->GetMemoryPolicy()->GetMaximumOCSPResponseSize())
        result=CIPHERCERTIFICATEX509REVOCATION::ValidateOCSP(response, (*leaf), (*issuer));
      if(result==CIPHERCERTIFICATEX509REVOCATION_RESULT_REVOKED) return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_CERTIFICATE_REVOKED);
      if((result!=CIPHERCERTIFICATEX509REVOCATION_RESULT_GOOD) &&
         (config->GetRevocationPolicy()!=DIOSTREAMTLS_REVOCATIONPOLICY_SOFT_FAIL))
        return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_BAD_CERTIFICATE_STATUS_RESPONSE);
    }

  if(revocationenabled && config->GetCertificateRevocationLists() && !config->GetCertificateRevocationLists()->IsEmpty())
    {
      if(!validatedchain || validatedchain->GetSize()<2) return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_BAD_CERTIFICATE);
      for(XDWORD certindex=0;certindex+1<validatedchain->GetSize();certindex++)
        {
          bool valid=false;
          for(XDWORD c=0;c<config->GetCertificateRevocationLists()->GetSize();c++)
            {
              XBUFFER* CRL=config->GetCertificateRevocationLists()->Get(c);
              if(!CRL || CRL->GetSize()>config->GetMemoryPolicy()->GetMaximumCRLSize()) continue;
              CIPHERCERTIFICATEX509REVOCATION_RESULT result=CIPHERCERTIFICATEX509REVOCATION::ValidateCRL((*CRL), (*validatedchain->Get(certindex)), (*validatedchain->Get(certindex+1)));
              if(result==CIPHERCERTIFICATEX509REVOCATION_RESULT_REVOKED) return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_CERTIFICATE_REVOKED);
              if(result==CIPHERCERTIFICATEX509REVOCATION_RESULT_GOOD) valid=true;
            }
          if(!valid && config->GetRevocationPolicy()!=DIOSTREAMTLS_REVOCATIONPOLICY_SOFT_FAIL)
            return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_BAD_CERTIFICATE_STATUS_RESPONSE);
        }
    }

  if((config->GetRevocationPolicy()==DIOSTREAMTLS_REVOCATIONPOLICY_MUST_STAPLE) && !hasstapledOCSP)
    return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_BAD_CERTIFICATE_STATUS_RESPONSE);

  if(!session->Transcript_Add(message)) return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_INTERNAL_ERROR);

  clientcertificateprovided = true;
  state = DIOSTREAMTLS13HANDSHAKESERVER_STATE_WAIT_CLIENT_CERTIFICATEVERIFY;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKESERVER::ClientCertificateVerify_Process(XBUFFER& message)
* @brief      Verify the TLS 1.3 client CertificateVerify message
* @note       INTERNAL
* @ingroup    DATAIO
* 
* @param[in]  message : Message value.
*
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKESERVER::ClientCertificateVerify_Process(XBUFFER& message)
{
  DIOSTREAMTLS_MSG_FRAGMENT<DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEVERIFY> certificateverify;
  XBUFFER                                                                workbuffer;
  XBUFFER                                                                transcripthash;
  XBUFFER                                                                signedcontent;
  CIPHERCERTIFICATEX509*                                                 leaf;

  if((state != DIOSTREAMTLS13HANDSHAKESERVER_STATE_WAIT_CLIENT_CERTIFICATEVERIFY) || !clientcertificateprovided)
    {
      return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_UNEXPECTED_MESSAGE);
    }

  workbuffer.Add(message);
  if(!certificateverify.GetFromBuffer(workbuffer, false) || !workbuffer.IsEmpty() ||
     (certificateverify.GetMsgType() != DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_CERTIFICATE_VERIFY))
    {
      return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_DECODE_ERROR);
    }

  bool schemeoffered = false;
  for(XDWORD c=0; c<config->GetSignatureSchemes()->GetSize(); c++)
    {
      if(config->GetSignatureSchemes()->Get(c) == certificateverify.GetBody()->GetAlgorithm())
        {
          schemeoffered = true;
          break;
        }
    }

  leaf = clientcertificatevalidator.GetLeafCertificate();
  if(!schemeoffered || !leaf || !leaf->GetPublicCipherKey() ||
     !DIOSTREAMTLSSIGNATURE::IsSupported(certificateverify.GetBody()->GetAlgorithm(), leaf) ||
     !session->TranscriptHash(transcripthash))
    {
      return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_DECRYPT_ERROR);
    }

  static XBYTE context[] = { 'T', 'L', 'S', ' ', '1', '.', '3', ',', ' ',
                             'c', 'l', 'i', 'e', 'n', 't', ' ',
                             'C', 'e', 'r', 't', 'i', 'f', 'i', 'c', 'a', 't', 'e', 'V', 'e', 'r', 'i', 'f', 'y' };

  for(int c=0; c<64; c++) if(!signedcontent.Add((XBYTE)0x20)) return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_INTERNAL_ERROR);

  if(!signedcontent.Add(context, sizeof(context)) || !signedcontent.Add((XBYTE)0x00) ||
     !signedcontent.Add(transcripthash) ||
     !DIOSTREAMTLSSIGNATURE::Verify(certificateverify.GetBody()->GetAlgorithm(), leaf->GetPublicCipherKey(),
                                    signedcontent, (*certificateverify.GetBody()->GetSignature())))
    {
      return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_DECRYPT_ERROR);
    }

  if(!session->Transcript_Add(message)) return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_INTERNAL_ERROR);

  state = DIOSTREAMTLS13HANDSHAKESERVER_STATE_WAIT_FINISHED;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKESERVER::EndOfEarlyData_Process(XBUFFER& message)
* @brief      Process the TLS 1.3 EndOfEarlyData marker and switch receive processing to handshake keys
* @note       INTERNAL
* @ingroup    DATAIO
*
* @param[in]  message : Complete EndOfEarlyData handshake message.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKESERVER::EndOfEarlyData_Process(XBUFFER& message)
{
  if(state != DIOSTREAMTLS13HANDSHAKESERVER_STATE_WAIT_END_OF_EARLY_DATA || !earlydataaccepted)
    {
      return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_UNEXPECTED_MESSAGE);
    }

  DIOSTREAMTLS_MSG_HANDSHAKE handshake;
  XBUFFER                    work;

  if(!work.Add(message) || !handshake.GetFromBuffer(work, false) || !work.IsEmpty() ||
     (handshake.GetMsgType() != DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_END_OF_EARLY_DATA) ||
     handshake.GetLength())
    {
      return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_DECODE_ERROR);
    }

  if(!session->Transcript_Add(message) || !session->EarlyData_Commit())
    {
      return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_INTERNAL_ERROR);
    }

  session->EarlyData_End(DIOSTREAMTLSKEYSCHEDULE_DIRECTION_REMOTE);
  state = DIOSTREAMTLS13HANDSHAKESERVER_STATE_WAIT_FINISHED;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKESERVER::Finished_Process(XBUFFER& message)
* @brief      Verify the client Finished and activate remote application keys
* @note       INTERNAL
* @ingroup    DATAIO
*
* @param[in]  message : Complete handshake message, including its header.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKESERVER::Finished_Process(XBUFFER& message)
{
  DIOSTREAMTLS_MSG_FRAGMENT<DIOSTREAMTLS_MSG_HANDSHAKE_FINISHED> finished;
  XBUFFER                                                       workbuffer;
  XBUFFER                                                       transcripthash;

  if(state != DIOSTREAMTLS13HANDSHAKESERVER_STATE_WAIT_FINISHED)
    {
      return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_UNEXPECTED_MESSAGE);
    }

  workbuffer.Add(message);

  if(!finished.GetFromBuffer(workbuffer, false) || !workbuffer.IsEmpty() ||
     (finished.GetMsgType() != DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_FINISHED) ||
     (finished.GetBody()->GetVerifyData()->GetSize() != session->GetKeySchedule()->GetHashSize()))
    {
      return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_DECODE_ERROR);
    }

  if(!session->TranscriptHash(transcripthash))
    {
      return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_INTERNAL_ERROR);
    }

  if(!session->GetKeySchedule()->VerifyFinished(DIOSTREAMTLSKEYSCHEDULE_DIRECTION_REMOTE,
                                                transcripthash, *finished.GetBody()->GetVerifyData()))
    {
      return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_DECRYPT_ERROR);
    }

  if(!session->Transcript_Add(message) ||
     !session->ApplicationKeys_Activate(DIOSTREAMTLSKEYSCHEDULE_DIRECTION_REMOTE))
    {
      return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_INTERNAL_ERROR);
    }

  XBUFFER resumptiontranscripthash;
  if(!session->TranscriptHash(resumptiontranscripthash) ||
     !session->GetKeySchedule()->ResumptionSecret_Calculate(resumptiontranscripthash))
    {
      return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_INTERNAL_ERROR);
    }

  state = DIOSTREAMTLS13HANDSHAKESERVER_STATE_HANDSHAKE_COMPLETED;

  if(!NewSessionTicket_Create())
    {
      return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_INTERNAL_ERROR);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKESERVER::RecordInput_Add(XBYTE* data, XDWORD size)
* @brief      Add received transport bytes to the TLS session
* @ingroup    DATAIO
*
* @param[in]  data : Bytes to add.
* @param[in]  size : Number of bytes to add.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKESERVER::RecordInput_Add(XBYTE* data, XDWORD size)
{
  if(!isini || !session)
    {
      return false;
    }

  return session->RecordInput_Add(data, size);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKESERVER::RecordInput_Add(XBUFFER& data)
* @brief      Add a received transport buffer to the TLS session
* @ingroup    DATAIO
*
* @param[in]  data : Buffer to add.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKESERVER::RecordInput_Add(XBUFFER& data)
{
  return RecordInput_Add(data.Get(), data.GetSize());
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKESERVER::Process()
* @brief      Process every complete record and handshake message currently accumulated
* @note       An incomplete record or message is retained and is not an error.
* @ingroup    DATAIO
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKESERVER::Process()
{
  if(!isini || !session || (state == DIOSTREAMTLS13HANDSHAKESERVER_STATE_ERROR))
    {
      return false;
    }

  while(true)
    {
      DIOSTREAMTLS_CONTENTTYPE   contenttype = (DIOSTREAMTLS_CONTENTTYPE)0;
      DIOSTREAMTLS13SESSION_RESULT result;
      XBUFFER                    plain;

      result = session->Record_Extract(contenttype, plain);

      if(result == DIOSTREAMTLS13SESSION_RESULT_INCOMPLETE)
        {
          return true;
        }

      if(result == DIOSTREAMTLS13SESSION_RESULT_ERROR)
        {
          return SetError(session->GetLastRecordAlertDescription());
        }

      switch(contenttype)
        {
          case DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE          : if(!session->HandshakeInput_Add(plain)) return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_INTERNAL_ERROR);
                                                                break;

          case DIOSTREAMTLS_MSG_CONTENTTYPE_CHANGE_CIPHER_SPEC : if((plain.GetSize() != 1) || (plain.GetByte(0) != 1)) return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_UNEXPECTED_MESSAGE);
                                                                break;

          case DIOSTREAMTLS_MSG_CONTENTTYPE_APPLICATION_DATA   : if(!earlydataoffered ||
                                                                   (session->GetEpoch(DIOSTREAMTLSKEYSCHEDULE_DIRECTION_REMOTE) != DIOSTREAMTLS13SESSION_EPOCH_EARLY))
                                                                  {
                                                                    return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_UNEXPECTED_MESSAGE);
                                                                  }
                                                                if(earlydataaccepted && session->GetEarlyDataSize()>maximumearlydatasize)
                                                                  {
                                                                    return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_UNEXPECTED_MESSAGE);
                                                                  }
                                                                break;

          case DIOSTREAMTLS_MSG_CONTENTTYPE_ALERT              : { DIOSTREAMTLS_MSG_ALERT alert;

                                                                  if(!alert.GetFromBuffer(plain, false))
                                                                    {
                                                                      return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_DECODE_ERROR);
                                                                    }

                                                                  return SetError(alert.GetDescription(), false);
                                                                }

                                                       default :
                                                                 return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_UNEXPECTED_MESSAGE);
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
              return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_DECODE_ERROR);
            }

          if(!Handshake_Process(handshake))
            {
              return false;
            }

          // The client's Finished message just completed the handshake and switched the remote epoch to
          // DIOSTREAMTLS13SESSION_EPOCH_APPLICATION (see Finished_Process() / ApplicationKeys_Activate()). Any
          // bytes still queued past this point -- e.g. the client's very first request, pipelined right behind
          // its Finished and pulled in by the very same Transport_Read() -- are already application-data records,
          // not handshake ones: this loop (and the switch above, which only understands HANDSHAKE /
          // CHANGE_CIPHER_SPEC / ALERT) must not try to interpret them, or it fails the whole connection via the
          // "default: SetError()" case below. Leave them queued in the shared session for the normal
          // post-handshake path (DIOSTREAMTLS<T>::ApplicationInput_Process() -> session.ApplicationData_Process())
          // to pick up on its own next call.
          if(state == DIOSTREAMTLS13HANDSHAKESERVER_STATE_HANDSHAKE_COMPLETED) return true;
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKESERVER::Handshake_Process(XBUFFER& message)
* @brief      Process one exact client handshake message according to the current state
* @ingroup    DATAIO
*
* @param[in]  message : Complete handshake message, including its header.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKESERVER::Handshake_Process(XBUFFER& message)
{
  DIOSTREAMTLS_MSG_HANDSHAKE genericmessage;
  XBUFFER                    workbuffer;

  if(!isini || !session || (state == DIOSTREAMTLS13HANDSHAKESERVER_STATE_ERROR))
    {
      return false;
    }

  workbuffer.Add(message);

  if(!genericmessage.GetFromBuffer(workbuffer, false) || !workbuffer.IsEmpty())
    {
      return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_DECODE_ERROR);
    }

  switch(genericmessage.GetMsgType())
    {
      case DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_CERTIFICATE        : return ClientCertificate_Process(message);
      case DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_CERTIFICATE_VERIFY : return ClientCertificateVerify_Process(message);
      case DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_END_OF_EARLY_DATA  : return EndOfEarlyData_Process(message);
      case DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_FINISHED           : return Finished_Process(message);
                                                               default : return SetError(DIOSTREAMTLS_ALERT_DESCRIPTION_UNEXPECTED_MESSAGE);
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13HANDSHAKESERVER::SetError(DIOSTREAMTLS_ALERT_DESCRIPTION alertdescription, bool sendalert)
* @brief      Move the handshake to its terminal error state and preserve the precise TLS alert cause
* @note       INTERNAL
* @ingroup    DATAIO
*
* @param[in]  alertdescription : TLS alert that best describes the failure.
* @param[in]  sendalert : true when the failure was generated locally and should be reported to the peer.
*
* @return     bool : Always false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13HANDSHAKESERVER::SetError(DIOSTREAMTLS_ALERT_DESCRIPTION alertdescription, bool sendalert)
{
  erroralertdescription = alertdescription;
  senderroralert        = sendalert;
  state                 = DIOSTREAMTLS13HANDSHAKESERVER_STATE_ERROR;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS13HANDSHAKESERVER::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS13HANDSHAKESERVER::Clean()
{
  session                       = NULL;
  config                        = NULL;
  state                          = DIOSTREAMTLS13HANDSHAKESERVER_STATE_NONE;
  isini                          = false;
  applicationprotocolnegotiated = false;
  applicationprotocol            = DIOSTREAMTLS_ALPN_TYPE_UNKNOWN;
  applicationprotocolraw.Delete();
  erroralertdescription           = DIOSTREAMTLS_ALERT_DESCRIPTION_HANDSHAKE_FAILURE;
  senderroralert                  = false;
  firstclienthello.Delete();
  retryselectedgroup               = 0;
  retryciphersuite                 = 0;
  clientcertificateprovided       = false;
  resumptionaccepted              = false;
  earlydataoffered                = false;
  earlydataaccepted               = false;
  maximumearlydatasize            = 0;
  earlydataticketidentity.Delete();
  serverOCSPstaplingrequested      = false;
  clientOCSPstaplingrequested      = false;
  clientcertificatevalidator.End();
}
