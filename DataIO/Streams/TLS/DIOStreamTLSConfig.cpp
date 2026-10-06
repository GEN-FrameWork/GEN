/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOStreamTLSConfig.cpp
* 
* @class      DIOSTREAMTLSCONFIG
* @brief      Data Input/Output Stream TLS (Transport Layer Security) Config class
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

#include "DIOStreamTLSConfig.h"

#include "XFactory.h"
#include "XDateTime.h"
#include "XRand.h"
#include "XTimer.h"
#include "XThread.h"

#include "CipherCertificateX509.h"
#include "CipherCertificateX509Revocation.h"
#include "CipherAESGCM.h"
#include "CipherKeySymmetrical.h"
#include "CipherKeyPrivateRSA.h"
#include "CipherKeyECDSA.h"
#include "CipherTrustedRootCertificatesX509.h"

#include "DIOStreamTLSSignature.h"
#include "DIOStreamTLSRecord.h"

#include "DIOFactory.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTLSMEMORYPOLICY::DIOSTREAMTLSMEMORYPOLICY()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLSMEMORYPOLICY::DIOSTREAMTLSMEMORYPOLICY()
{
  maximumrecordinputsize      = DIOSTREAMTLS_MEMORY_DEFAULT_RECORD_INPUT;
  maximumhandshakeinputsize   = DIOSTREAMTLS_MEMORY_DEFAULT_HANDSHAKE_INPUT;
  maximumtranscriptsize       = DIOSTREAMTLS_MEMORY_DEFAULT_TRANSCRIPT;
  maximumapplicationinputsize = DIOSTREAMTLS_MEMORY_DEFAULT_APPLICATION_INPUT;
  maximumAIAheadersize        = DIOSTREAMTLS_MEMORY_DEFAULT_AIA_HEADER;
  maximumAIAbodysize          = DIOSTREAMTLS_MEMORY_DEFAULT_AIA_BODY;
  maximumAIAfetches           = DIOSTREAMTLS_MEMORY_DEFAULT_AIA_FETCHES;
  maximumOCSPresponsesize     = DIOSTREAMTLS_MEMORY_DEFAULT_OCSP_RESPONSE;
  maximumCRLsize              = DIOSTREAMTLS_MEMORY_DEFAULT_CRL;
  maximumCRLs                 = DIOSTREAMTLS_MEMORY_DEFAULT_CRLS;
  maximumsessiontickets       = DIOSTREAMTLS_MEMORY_DEFAULT_SESSION_TICKETS;
  maximumtrustroots           = DIOSTREAMTLS_MEMORY_DEFAULT_TRUST_ROOTS;
  maximumtrustcertificatesize = DIOSTREAMTLS_MEMORY_DEFAULT_TRUST_CERTIFICATE;
  maximumtruststoresize       = DIOSTREAMTLS_MEMORY_DEFAULT_TRUST_TOTAL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSMEMORYPOLICY::SetConnectionBufferLimits(XDWORD recordinput, XDWORD handshakeinput, XDWORD transcript, XDWORD applicationinput)
* @brief      Set connection buffer limits
* @ingroup    DATAIO
* 
* @param[in]  recordinput : Recordinput value.
* @param[in]  handshakeinput : Handshakeinput value.
* @param[in]  transcript : Transcript value.
* @param[in]  applicationinput : Applicationinput value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSMEMORYPOLICY::SetConnectionBufferLimits(XDWORD recordinput, XDWORD handshakeinput, XDWORD transcript, XDWORD applicationinput)
{
  if((recordinput < DIOSTREAMTLS_MEMORY_MINIMUM_RECORD_INPUT) ||
     (recordinput > DIOSTREAMTLS_MEMORY_MAXIMUM_BUFFER) ||
     (handshakeinput < (64*1024)) || (handshakeinput > DIOSTREAMTLS_MSG_MAXHANDSHAKESIZE) ||
     (transcript < handshakeinput) || (transcript > DIOSTREAMTLS_MEMORY_MAXIMUM_BUFFER) ||
     (applicationinput < DIOSTREAMTLSRECORD_MAXPLAINSIZE) ||
     (applicationinput > DIOSTREAMTLS_MEMORY_MAXIMUM_BUFFER)) return false;

  maximumrecordinputsize      = recordinput;
  maximumhandshakeinputsize   = handshakeinput;
  maximumtranscriptsize       = transcript;
  maximumapplicationinputsize = applicationinput;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSMEMORYPOLICY::SetAIALimits(XDWORD headersize, XDWORD bodysize, XDWORD maximumfetches)
* @brief      Set aia limits
* @ingroup    DATAIO
* 
* @param[in]  headersize : Headersize value.
* @param[in]  bodysize : Bodysize value.
* @param[in]  maximumfetches : Maximumfetches value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSMEMORYPOLICY::SetAIALimits(XDWORD headersize, XDWORD bodysize, XDWORD maximumfetches)
{
  if((headersize < 1024) || (headersize > (64*1024)) ||
     (bodysize < 1024) || (bodysize > (1024*1024)) ||
     !maximumfetches || (maximumfetches > CIPHERCERTIFICATEX509VALIDATOR_MAXCHAINSIZE)) return false;
  maximumAIAheadersize = headersize;
  maximumAIAbodysize   = bodysize;
  maximumAIAfetches    = maximumfetches;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSMEMORYPOLICY::SetRevocationLimits(XDWORD OCSPsize, XDWORD CRLsize, XDWORD maximumCRLs)
* @brief      Set revocation limits
* @ingroup    DATAIO
* 
* @param[in]  OCSPsize : OCSPsize value.
* @param[in]  CRLsize : CRLsize value.
* @param[in]  maximumCRLs : MaximumCRLs value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSMEMORYPOLICY::SetRevocationLimits(XDWORD OCSPsize, XDWORD CRLsize, XDWORD maximumCRLs)
{
  if(!OCSPsize || (OCSPsize > CIPHERCERTIFICATEX509REVOCATION_MAX_OCSP_SIZE) ||
     !CRLsize || (CRLsize > CIPHERCERTIFICATEX509REVOCATION_MAX_CRL_SIZE) ||
     !maximumCRLs || (maximumCRLs > 256)) return false;
  maximumOCSPresponsesize = OCSPsize;
  maximumCRLsize          = CRLsize;
  this->maximumCRLs       = maximumCRLs;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSMEMORYPOLICY::SetTrustStoreLimits(XDWORD maximumroots, XDWORD maximumcertificatesize, XDWORD maximumtotalsize)
* @brief      Set trust store limits
* @ingroup    DATAIO
* 
* @param[in]  maximumroots : Maximumroots value.
* @param[in]  maximumcertificatesize : Maximumcertificatesize value.
* @param[in]  maximumtotalsize : Maximumtotalsize value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSMEMORYPOLICY::SetTrustStoreLimits(XDWORD maximumroots, XDWORD maximumcertificatesize, XDWORD maximumtotalsize)
{
  if(!maximumroots || (maximumroots > 8192) || !maximumcertificatesize ||
     (maximumcertificatesize > (4*1024*1024)) ||
     (maximumtotalsize < maximumcertificatesize) || (maximumtotalsize > (128*1024*1024))) return false;
  maximumtrustroots           = maximumroots;
  maximumtrustcertificatesize = maximumcertificatesize;
  maximumtruststoresize       = maximumtotalsize;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSMEMORYPOLICY::SetMaximumSessionTickets(XDWORD maximumtickets)
* @brief      Set maximum session tickets
* @ingroup    DATAIO
* 
* @param[in]  maximumtickets : Maximumtickets value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSMEMORYPOLICY::SetMaximumSessionTickets(XDWORD maximumtickets)
{
  if(!maximumtickets || (maximumtickets > 64)) return false;
  maximumsessiontickets = maximumtickets;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOSTREAMTLSMEMORYPOLICY::GetMaximumRecordInputSize()
* @brief      Get maximum record input size
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAMTLSMEMORYPOLICY::GetMaximumRecordInputSize()      { return maximumrecordinputsize; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOSTREAMTLSMEMORYPOLICY::GetMaximumHandshakeInputSize()
* @brief      Get maximum handshake input size
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAMTLSMEMORYPOLICY::GetMaximumHandshakeInputSize()   { return maximumhandshakeinputsize; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOSTREAMTLSMEMORYPOLICY::GetMaximumTranscriptSize()
* @brief      Get maximum transcript size
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAMTLSMEMORYPOLICY::GetMaximumTranscriptSize()       { return maximumtranscriptsize; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOSTREAMTLSMEMORYPOLICY::GetMaximumApplicationInputSize()
* @brief      Get maximum application input size
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAMTLSMEMORYPOLICY::GetMaximumApplicationInputSize() { return maximumapplicationinputsize; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOSTREAMTLSMEMORYPOLICY::GetMaximumAIAHeaderSize()
* @brief      Get maximum aia header size
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAMTLSMEMORYPOLICY::GetMaximumAIAHeaderSize()        { return maximumAIAheadersize; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOSTREAMTLSMEMORYPOLICY::GetMaximumAIABodySize()
* @brief      Get maximum aia body size
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAMTLSMEMORYPOLICY::GetMaximumAIABodySize()          { return maximumAIAbodysize; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOSTREAMTLSMEMORYPOLICY::GetMaximumAIAFetches()
* @brief      Get maximum aia fetches
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAMTLSMEMORYPOLICY::GetMaximumAIAFetches()           { return maximumAIAfetches; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOSTREAMTLSMEMORYPOLICY::GetMaximumOCSPResponseSize()
* @brief      Get maximum ocsp response size
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAMTLSMEMORYPOLICY::GetMaximumOCSPResponseSize()     { return maximumOCSPresponsesize; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOSTREAMTLSMEMORYPOLICY::GetMaximumCRLSize()
* @brief      Get maximum crl size
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAMTLSMEMORYPOLICY::GetMaximumCRLSize()              { return maximumCRLsize; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOSTREAMTLSMEMORYPOLICY::GetMaximumCRLs()
* @brief      Get maximum cr ls
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAMTLSMEMORYPOLICY::GetMaximumCRLs()                 { return maximumCRLs; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOSTREAMTLSMEMORYPOLICY::GetMaximumSessionTickets()
* @brief      Get maximum session tickets
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAMTLSMEMORYPOLICY::GetMaximumSessionTickets()       { return maximumsessiontickets; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOSTREAMTLSMEMORYPOLICY::GetMaximumTrustRoots()
* @brief      Get maximum trust roots
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAMTLSMEMORYPOLICY::GetMaximumTrustRoots()           { return maximumtrustroots; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOSTREAMTLSMEMORYPOLICY::GetMaximumTrustCertificateSize()
* @brief      Get maximum trust certificate size
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAMTLSMEMORYPOLICY::GetMaximumTrustCertificateSize() { return maximumtrustcertificatesize; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOSTREAMTLSMEMORYPOLICY::GetMaximumTrustStoreSize()
* @brief      Get maximum trust store size
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAMTLSMEMORYPOLICY::GetMaximumTrustStoreSize()       { return maximumtruststoresize; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG_IsCipherSuiteSupported(XWORD ciphersuite)
* @brief      Is cipher suite supported
* @ingroup    DATAIO
* 
* @param[in]  ciphersuite : Ciphersuite value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool DIOSTREAMTLSCONFIG_IsCipherSuiteSupported(XWORD ciphersuite)
{
  switch(ciphersuite)
    {
      case DIOSTREAMTLS_MSG_CIPHER_AES_128_GCM_SHA256 :
      case DIOSTREAMTLS_MSG_CIPHER_AES_256_GCM_SHA384 : return true;

      #ifdef CIPHER_SYMMETRIC_CHACHA20POLY1305_ACTIVE

      case DIOSTREAMTLS_MSG_CIPHER_CHACHA20_POLY1305_SHA256 : return true;

      #endif

      default : return false;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG_IsTLS12CipherSuiteSupported(XWORD ciphersuite)
* @brief      Is tls12 cipher suite supported
* @ingroup    DATAIO
* 
* @param[in]  ciphersuite : Ciphersuite value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool DIOSTREAMTLSCONFIG_IsTLS12CipherSuiteSupported(XWORD ciphersuite)
{
  switch(ciphersuite)
    {
      case DIOSTREAMTLS12_CIPHER_ECDHE_RSA_WITH_AES_128_GCM_SHA256   :
      case DIOSTREAMTLS12_CIPHER_ECDHE_RSA_WITH_AES_256_GCM_SHA384   :
      case DIOSTREAMTLS12_CIPHER_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256 :
      case DIOSTREAMTLS12_CIPHER_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384 : return true;

      #ifdef CIPHER_SYMMETRIC_CHACHA20POLY1305_ACTIVE

      case DIOSTREAMTLS_MSG_CIPHER_ECDHE_RSA_WITH_CHACHA20_POLY1305_SHA256   :
      case DIOSTREAMTLS_MSG_CIPHER_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256 : return true;

      #endif

                                                                    default : return false;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG_IsSupportedGroupSupported(XWORD group)
* @brief      Is supported group supported
* @ingroup    DATAIO
* 
* @param[in]  group : Group value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool DIOSTREAMTLSCONFIG_IsSupportedGroupSupported(XWORD group)
{
  bool supported = (group == DIOSTREAMTLS_MSG_CURVEID_X25519)   ||
                   (group == DIOSTREAMTLS_MSG_CURVEID_SECP256R1) ||
                   (group == DIOSTREAMTLS_MSG_CURVEID_SECP384R1) ||
                   (group == DIOSTREAMTLS_MSG_CURVEID_SECP521R1);

  #ifdef CIPHER_ASYMMETRIC_MLKEM768_ACTIVE
  supported = supported || (group == DIOSTREAMTLS_MSG_CURVEID_SECP256R1MLKEM768);
  #endif

  #ifdef CIPHER_ASYMMETRIC_MLKEM1024_ACTIVE
  supported = supported || (group == DIOSTREAMTLS_MSG_CURVEID_SECP384R1MLKEM1024);
  #endif

  #if defined(CIPHER_ASYMMETRIC_X25519_ACTIVE) && defined(CIPHER_ASYMMETRIC_MLKEM768_ACTIVE)
  supported = supported || (group == DIOSTREAMTLS_MSG_CURVEID_X25519MLKEM768);
  #endif

  return supported;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG_IsSignatureSchemeSupported(XWORD scheme, bool certificate)
* @brief      Is signature scheme supported
* @ingroup    DATAIO
* 
* @param[in]  scheme : Scheme value.
* @param[in]  certificate : Certificate value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool DIOSTREAMTLSCONFIG_IsSignatureSchemeSupported(XWORD scheme, bool certificate)
{
  switch(scheme)
    {
      #ifdef CIPHER_ASYMMETRIC_ED25519_ACTIVE
      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_ED25519                  : return true;
      #endif
      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_ECDSA_SECP256R1_SHA256 :
      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_ECDSA_SECP384R1_SHA384 :
      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_ECDSA_SECP521R1_SHA512 :
      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA256    :
      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA384    :
      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA512    :
      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA256     :
      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA384     :
      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA512     : return true;
      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PKCS1_SHA256       :
      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PKCS1_SHA384       :
      case DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PKCS1_SHA512       : return certificate;
                                                               default : return false;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG_ServerNameMatch(XSTRING& pattern, XCHAR* servername)
* @brief      Server name match
* @ingroup    DATAIO
* 
* @param[in]  pattern : Pattern value.
* @param[in]  servername : Pointer to servername.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool DIOSTREAMTLSCONFIG_ServerNameMatch(XSTRING& pattern, XCHAR* servername)
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

class DIOSTREAMTLSCONFIG_LOCK
{
  public:
    DIOSTREAMTLSCONFIG_LOCK(XMUTEX* mutex) : mutex(mutex), locked(mutex?mutex->Lock():false) {}
    ~DIOSTREAMTLSCONFIG_LOCK() { if(locked) mutex->UnLock(); }
    /**-------------------------------------------------------------------------------------------------------------------
    * 
    * @fn         bool IsLocked()
    * @brief      Is locked
    * @ingroup    DATAIO
    * 
    * @return     bool : true if the condition is met; otherwise false.
    * 
    * --------------------------------------------------------------------------------------------------------------------*/
    bool IsLocked() { return locked; }
  private:
    XMUTEX* mutex;
    bool locked;
};


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG_AddUInt16BE(XBUFFER& buffer, XWORD value)
* @brief      Add u int16 be
* @ingroup    DATAIO
* 
* @param[in]  buffer : Buffer value.
* @param[in]  value : Value value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool DIOSTREAMTLSCONFIG_AddUInt16BE(XBUFFER& buffer, XWORD value)
{
  XBYTE data[2] = { (XBYTE)(value >> 8), (XBYTE)value };
  return buffer.Add(data, sizeof(data));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG_AddUInt32BE(XBUFFER& buffer, XDWORD value)
* @brief      Add u int32 be
* @ingroup    DATAIO
* 
* @param[in]  buffer : Buffer value.
* @param[in]  value : Value value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool DIOSTREAMTLSCONFIG_AddUInt32BE(XBUFFER& buffer, XDWORD value)
{
  XBYTE data[4] = { (XBYTE)(value >> 24), (XBYTE)(value >> 16), (XBYTE)(value >> 8), (XBYTE)value };
  return buffer.Add(data, sizeof(data));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG_AddUInt64BE(XBUFFER& buffer, XQWORD value)
* @brief      Add u int64 be
* @ingroup    DATAIO
* 
* @param[in]  buffer : Buffer value.
* @param[in]  value : Value value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool DIOSTREAMTLSCONFIG_AddUInt64BE(XBUFFER& buffer, XQWORD value)
{
  XBYTE data[8];
  for(XDWORD c=0; c<sizeof(data); c++) data[c] = (XBYTE)(value >> ((sizeof(data)-1-c)*8));
  return buffer.Add(data, sizeof(data));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG_GetUInt16BE(XBUFFER& buffer, XDWORD& position, XWORD& value)
* @brief      Get u int16 be
* @ingroup    DATAIO
* 
* @param[in]  buffer : Buffer value.
* @param[in]  position : Position value.
* @param[in]  value : Value value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool DIOSTREAMTLSCONFIG_GetUInt16BE(XBUFFER& buffer, XDWORD& position, XWORD& value)
{
  if(position > buffer.GetSize() || (buffer.GetSize()-position) < 2) return false;
  value = (XWORD)(((XWORD)buffer.Get()[position] << 8) | buffer.Get()[position+1]);
  position += 2;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG_GetUInt32BE(XBUFFER& buffer, XDWORD& position, XDWORD& value)
* @brief      Get u int32 be
* @ingroup    DATAIO
* 
* @param[in]  buffer : Buffer value.
* @param[in]  position : Position value.
* @param[in]  value : Value value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool DIOSTREAMTLSCONFIG_GetUInt32BE(XBUFFER& buffer, XDWORD& position, XDWORD& value)
{
  if(position > buffer.GetSize() || (buffer.GetSize()-position) < 4) return false;
  value = ((XDWORD)buffer.Get()[position] << 24) | ((XDWORD)buffer.Get()[position+1] << 16) |
          ((XDWORD)buffer.Get()[position+2] << 8) | buffer.Get()[position+3];
  position += 4;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG_GetUInt64BE(XBUFFER& buffer, XDWORD& position, XQWORD& value)
* @brief      Get u int64 be
* @ingroup    DATAIO
* 
* @param[in]  buffer : Buffer value.
* @param[in]  position : Position value.
* @param[in]  value : Value value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool DIOSTREAMTLSCONFIG_GetUInt64BE(XBUFFER& buffer, XDWORD& position, XQWORD& value)
{
  if(position > buffer.GetSize() || (buffer.GetSize()-position) < 8) return false;
  value = 0;
  for(XDWORD c=0; c<8; c++) value = (value << 8) | buffer.Get()[position+c];
  position += 8;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTLSCONFIG_SecureZero(void* data, XDWORD size)
* @brief      Secure zero
* @ingroup    DATAIO
* 
* @param[in]  data : Pointer to data.
* @param[in]  size : Size value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static void DIOSTREAMTLSCONFIG_SecureZero(void* data, XDWORD size)
{
  volatile XBYTE* clean = (volatile XBYTE*)data;
  while(size--) (*clean++) = 0;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLSSERVERCREDENTIALS::DIOSTREAMTLSSERVERCREDENTIALS()
* @brief      Constructor of class
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLSSERVERCREDENTIALS::DIOSTREAMTLSSERVERCREDENTIALS()
{
  privatekey = NULL;

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLSSERVERCREDENTIALS::~DIOSTREAMTLSSERVERCREDENTIALS()
* @brief      Destructor of class
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLSSERVERCREDENTIALS::~DIOSTREAMTLSSERVERCREDENTIALS()
{
  Delete();
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSTRING* DIOSTREAMTLSSERVERCREDENTIALS::GetServerName()
* @brief      Get the SNI server name pattern associated with these credentials
* @ingroup    DATAIO
*
* @return     XSTRING* : Server name pattern.
*
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOSTREAMTLSSERVERCREDENTIALS::GetServerName()
{
  return &servername;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XVECTOR<XBUFFER*>* DIOSTREAMTLSSERVERCREDENTIALS::GetCertificateChain()
* @brief      Get the certificate chain associated with this SNI name
* @ingroup    DATAIO
*
* @return     XVECTOR<XBUFFER*>* : Certificate chain.
*
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<XBUFFER*>* DIOSTREAMTLSSERVERCREDENTIALS::GetCertificateChain()
{
  return &certificatechain;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSSERVERCREDENTIALS::Certificate_Add(XBUFFER& certificate)
* @brief      Add a copied DER certificate to the SNI certificate chain
* @ingroup    DATAIO
*
* @param[in]  certificate : DER certificate.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSSERVERCREDENTIALS::Certificate_Add(XBUFFER& certificate)
{
  XBUFFER* copy;

  if(certificate.IsEmpty()) return false;

  copy = GEN_NEW XBUFFER();
  if(!copy) return false;

  if(!copy->Add(certificate) || !certificatechain.Add(copy))
    {
      GEN_DELETE copy;
      return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSSERVERCREDENTIALS::Certificates_Delete()
* @brief      Delete the SNI certificate chain
* @ingroup    DATAIO
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSSERVERCREDENTIALS::Certificates_Delete()
{
  certificatechain.DeleteContents();
  certificatechain.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         CIPHERKEY* DIOSTREAMTLSSERVERCREDENTIALS::GetPrivateKey()
* @brief      Get the private key associated with this SNI name
* @ingroup    DATAIO
*
* @return     CIPHERKEY* : Private key.
*
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERKEY* DIOSTREAMTLSSERVERCREDENTIALS::GetPrivateKey()
{
  return privatekey;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSSERVERCREDENTIALS::SetPrivateKey(CIPHERKEY* privatekey)
* @brief      Copy the private key associated with this SNI name
* @ingroup    DATAIO
*
* @param[in]  privatekey : Private key to copy.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSSERVERCREDENTIALS::SetPrivateKey(CIPHERKEY* privatekey)
{
  CIPHERKEY* copy = NULL;

  if(!privatekey || !privatekey->GetSizeInBytes()) return false;

  switch(privatekey->GetType())
    {
      case CIPHERKEYTYPE_RSA_PRIVATE :
        {
          CIPHERKEYPRIVATERSA* rsacopy = GEN_NEW CIPHERKEYPRIVATERSA();
          if(!rsacopy) return false;

          if(!rsacopy->CopyFrom((CIPHERKEYPRIVATERSA*)privatekey))
            {
              GEN_DELETE rsacopy;
              return false;
            }

          copy = rsacopy;
        }
        break;

      case CIPHERKEYTYPE_ECDSA_SECP256R1_PRIVATE :
      case CIPHERKEYTYPE_ECDSA_SECP384R1_PRIVATE :
      case CIPHERKEYTYPE_ECDSA_SECP521R1_PRIVATE :
        {
          CIPHERKEYECDSA* ecdsacopy = GEN_NEW CIPHERKEYECDSA();
          if(!ecdsacopy) return false;

          if(!ecdsacopy->CopyFrom((CIPHERKEYSYMMETRICAL*)privatekey))
            {
              GEN_DELETE ecdsacopy;
              return false;
            }

          copy = ecdsacopy;
        }
        break;

      #ifdef CIPHER_ASYMMETRIC_ED25519_ACTIVE
      case CIPHERKEYTYPE_ED25519_PRIVATE :
        {
          CIPHERKEYSYMMETRICAL* ed25519copy = GEN_NEW CIPHERKEYSYMMETRICAL();
          if(!ed25519copy) return false;

          if(!ed25519copy->CopyFrom((CIPHERKEYSYMMETRICAL*)privatekey))
            {
              GEN_DELETE ed25519copy;
              return false;
            }

          copy = ed25519copy;
        }
        break;
      #endif

                                        default : return false;
    }

  if(this->privatekey) GEN_DELETE this->privatekey;

  this->privatekey = copy;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBUFFER* DIOSTREAMTLSSERVERCREDENTIALS::GetOCSPStapledResponse()
* @brief      Get ocsp stapled response
* @ingroup    DATAIO
* 
* @return     XBUFFER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* DIOSTREAMTLSSERVERCREDENTIALS::GetOCSPStapledResponse()
{
  return &OCSPstapledresponse;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSSERVERCREDENTIALS::SetOCSPStapledResponse(XBUFFER& response)
* @brief      Set ocsp stapled response
* @ingroup    DATAIO
* 
* @param[in]  response : Response value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSSERVERCREDENTIALS::SetOCSPStapledResponse(XBUFFER& response)
{
  if(response.IsEmpty() || (response.GetSize() > CIPHERCERTIFICATEX509REVOCATION_MAX_OCSP_SIZE)) return false;

  OCSPstapledresponse.Delete();
  return OCSPstapledresponse.Add(response);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSSERVERCREDENTIALS::OCSPStapledResponse_Add(XBUFFER& response)
* @brief      Ocsp stapled response add
* @ingroup    DATAIO
* 
* @param[in]  response : Response value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSSERVERCREDENTIALS::OCSPStapledResponse_Add(XBUFFER& response)
{
  if(response.IsEmpty() || response.GetSize() > CIPHERCERTIFICATEX509REVOCATION_MAX_OCSP_SIZE) return false;
  XBUFFER* copy = GEN_NEW XBUFFER();
  if(!copy || !copy->Add(response) || !OCSPstapledresponses.Add(copy)) { if(copy) GEN_DELETE copy; return false; }
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<XBUFFER*>* DIOSTREAMTLSSERVERCREDENTIALS::GetOCSPStapledResponses()
* @brief      Get ocsp stapled responses
* @ingroup    DATAIO
* 
* @return     XVECTOR<XBUFFER*>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<XBUFFER*>* DIOSTREAMTLSSERVERCREDENTIALS::GetOCSPStapledResponses()
{
  return &OCSPstapledresponses;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSSERVERCREDENTIALS::DeleteOCSPStapledResponse()
* @brief      Delete ocsp stapled response
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSSERVERCREDENTIALS::DeleteOCSPStapledResponse()
{
  OCSPstapledresponse.Delete();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSSERVERCREDENTIALS::HasCredentials()
* @brief      Check whether this SNI entry contains a certificate chain and private key
* @ingroup    DATAIO
*
* @return     bool : true if credentials are configured; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSSERVERCREDENTIALS::HasCredentials()
{
  return !certificatechain.IsEmpty() && privatekey;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSSERVERCREDENTIALS::Delete()
* @brief      Delete all credentials owned by this SNI entry
* @ingroup    DATAIO
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSSERVERCREDENTIALS::Delete()
{
  Certificates_Delete();
  DeleteOCSPStapledResponse();
  OCSPstapledresponses.DeleteContents(); OCSPstapledresponses.DeleteAll();

  if(privatekey)
    {
      GEN_DELETE privatekey;
      privatekey = NULL;
    }

  servername.Empty();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLSSERVERCREDENTIALS::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLSSERVERCREDENTIALS::Clean()
{
  servername.Empty();
  privatekey = NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XQWORD DIOSTREAMTLSCONFIG_CurrentEpoch()
* @brief      Current epoch
* @ingroup    DATAIO
* 
* @return     XQWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static XQWORD DIOSTREAMTLSCONFIG_CurrentEpoch()
{
  XDATETIME* datetime = GEN_XFACTORY.CreateDateTime();
  XQWORD     epoch    = 0;

  if(datetime)
    {
      if(datetime->Read(false)) epoch = datetime->GetEPOCHFormat();
      GEN_XFACTORY.DeleteDateTime(datetime);
    }

  return epoch;
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG_ALPNFromType(DIOSTREAMTLS_ALPN_TYPE type, XBUFFER& protocol)
* @brief      Alpn from type
* @ingroup    DATAIO
* 
* @param[in]  type : Type value.
* @param[in]  protocol : Protocol value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool DIOSTREAMTLSCONFIG_ALPNFromType(DIOSTREAMTLS_ALPN_TYPE type, XBUFFER& protocol)
{
  protocol.Delete();
  if(type == DIOSTREAMTLS_ALPN_TYPE_UNKNOWN) return true;
  if(type == DIOSTREAMTLS_ALPN_TYPE_HTTP_1_1) { static XBYTE value[]={'h','t','t','p','/','1','.','1'}; return protocol.Add(value, sizeof(value)); }
  if(type == DIOSTREAMTLS_ALPN_TYPE_HTTP_2)   { static XBYTE value[]={'h','2'}; return protocol.Add(value, sizeof(value)); }
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTLS13SESSIONTICKET::DIOSTREAMTLS13SESSIONTICKET()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS13SESSIONTICKET::DIOSTREAMTLS13SESSIONTICKET()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTLS13SESSIONTICKET::~DIOSTREAMTLS13SESSIONTICKET()
* @brief      Destructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS13SESSIONTICKET::~DIOSTREAMTLS13SESSIONTICKET()
{
  Delete();
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOSTREAMTLS13SESSIONTICKET::GetServerName()
* @brief      Get server name
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOSTREAMTLS13SESSIONTICKET::GetServerName() { return &servername; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBUFFER* DIOSTREAMTLS13SESSIONTICKET::GetTicket()
* @brief      Get ticket
* @ingroup    DATAIO
* 
* @return     XBUFFER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* DIOSTREAMTLS13SESSIONTICKET::GetTicket() { return &ticket; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBUFFER* DIOSTREAMTLS13SESSIONTICKET::GetPSK()
* @brief      Get psk
* @ingroup    DATAIO
* 
* @return     XBUFFER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* DIOSTREAMTLS13SESSIONTICKET::GetPSK() { return &psk; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOSTREAMTLS13SESSIONTICKET::GetTicketAgeAdd()
* @brief      Get ticket age add
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAMTLS13SESSIONTICKET::GetTicketAgeAdd() { return ticketageadd; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTLS13SESSIONTICKET::SetTicketAgeAdd(XDWORD ageadd)
* @brief      Set ticket age add
* @ingroup    DATAIO
* 
* @param[in]  ageadd : Ageadd value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS13SESSIONTICKET::SetTicketAgeAdd(XDWORD ageadd) { ticketageadd = ageadd; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOSTREAMTLS13SESSIONTICKET::GetLifetime()
* @brief      Get lifetime
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAMTLS13SESSIONTICKET::GetLifetime() { return lifetime; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTLS13SESSIONTICKET::SetLifetime(XDWORD lifetime)
* @brief      Set lifetime
* @ingroup    DATAIO
* 
* @param[in]  lifetime : Lifetime value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS13SESSIONTICKET::SetLifetime(XDWORD lifetime) { this->lifetime = lifetime; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XQWORD DIOSTREAMTLS13SESSIONTICKET::GetReceivedEpoch()
* @brief      Get received epoch
* @ingroup    DATAIO
* 
* @return     XQWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XQWORD DIOSTREAMTLS13SESSIONTICKET::GetReceivedEpoch() { return receivedepoch; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTLS13SESSIONTICKET::SetReceivedEpoch(XQWORD epoch)
* @brief      Set received epoch
* @ingroup    DATAIO
* 
* @param[in]  epoch : Epoch value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS13SESSIONTICKET::SetReceivedEpoch(XQWORD epoch) { receivedepoch = epoch; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORD DIOSTREAMTLS13SESSIONTICKET::GetCipherSuite()
* @brief      Get cipher suite
* @ingroup    DATAIO
* 
* @return     XWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORD DIOSTREAMTLS13SESSIONTICKET::GetCipherSuite() { return ciphersuite; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTLS13SESSIONTICKET::SetCipherSuite(XWORD ciphersuite)
* @brief      Set cipher suite
* @ingroup    DATAIO
* 
* @param[in]  ciphersuite : Ciphersuite value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS13SESSIONTICKET::SetCipherSuite(XWORD ciphersuite) { this->ciphersuite = ciphersuite; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTLS_ALPN_TYPE DIOSTREAMTLS13SESSIONTICKET::GetApplicationProtocol()
* @brief      Get application protocol
* @ingroup    DATAIO
* 
* @return     DIOSTREAMTLS_ALPN_TYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_ALPN_TYPE DIOSTREAMTLS13SESSIONTICKET::GetApplicationProtocol() { return applicationprotocol; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTLS13SESSIONTICKET::SetApplicationProtocol(DIOSTREAMTLS_ALPN_TYPE protocol)
* @brief      Set application protocol
* @ingroup    DATAIO
* 
* @param[in]  protocol : Protocol value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS13SESSIONTICKET::SetApplicationProtocol(DIOSTREAMTLS_ALPN_TYPE protocol) { applicationprotocol = protocol; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBUFFER* DIOSTREAMTLS13SESSIONTICKET::GetApplicationProtocolRaw()
* @brief      Get application protocol raw
* @ingroup    DATAIO
* 
* @return     XBUFFER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* DIOSTREAMTLS13SESSIONTICKET::GetApplicationProtocolRaw() { return &applicationprotocolraw; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOSTREAMTLS13SESSIONTICKET::GetMaximumEarlyDataSize()
* @brief      Get maximum early data size
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAMTLS13SESSIONTICKET::GetMaximumEarlyDataSize() { return maximumearlydatasize; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTLS13SESSIONTICKET::SetMaximumEarlyDataSize(XDWORD size)
* @brief      Set maximum early data size
* @ingroup    DATAIO
* 
* @param[in]  size : Size value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS13SESSIONTICKET::SetMaximumEarlyDataSize(XDWORD size) { maximumearlydatasize = size; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS13SESSIONTICKET::IsExpired()
* @brief      Is expired
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13SESSIONTICKET::IsExpired()
{
  XQWORD now = DIOSTREAMTLSCONFIG_CurrentEpoch();
  if(!now || !receivedepoch || !lifetime) return true;
  return (now < receivedepoch) || ((now - receivedepoch) >= lifetime);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOSTREAMTLS13SESSIONTICKET::GetObfuscatedAge()
* @brief      Get obfuscated age
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAMTLS13SESSIONTICKET::GetObfuscatedAge()
{
  XQWORD now = DIOSTREAMTLSCONFIG_CurrentEpoch();
  if(!now || !receivedepoch || now < receivedepoch) return ticketageadd;

  XQWORD agems = (now - receivedepoch) * 1000ULL;
  return (XDWORD)((agems + ticketageadd) & 0xFFFFFFFFULL);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS13SESSIONTICKET::Delete()
* @brief      Delete
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13SESSIONTICKET::Delete()
{
  if(!psk.IsEmpty()) psk.FillBuffer(0);
  psk.SecureDelete();
  ticket.Delete();
  applicationprotocolraw.Delete();
  servername.Empty();
  Clean();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTLS13SESSIONTICKET::Clean()
* @brief      Clean
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS13SESSIONTICKET::Clean()
{
  ticketageadd        = 0;
  lifetime            = 0;
  receivedepoch       = 0;
  ciphersuite         = 0;
  applicationprotocol = DIOSTREAMTLS_ALPN_TYPE_UNKNOWN;
  applicationprotocolraw.Delete();
  maximumearlydatasize = 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTLSCONFIG::DIOSTREAMTLSCONFIG() : DIOSTREAMTCPIPCONFIG()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLSCONFIG::DIOSTREAMTLSCONFIG() : DIOSTREAMTCPIPCONFIG()
{
  localprivatekey = NULL;
  configmutex = GEN_XFACTORY.Create_Mutex();

  Clean();

  SetIsTLS(true);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTLSCONFIG::~DIOSTREAMTLSCONFIG()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLSCONFIG::~DIOSTREAMTLSCONFIG()
{
  Clean();
  if(configmutex) GEN_XFACTORY.Delete_Mutex(configmutex);
  configmutex = NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::Freeze()
* @brief      Freeze
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::Freeze()
{
  DIOSTREAMTLSCONFIG_LOCK lock(configmutex);
  if(!lock.IsLocked()) return false;
  if(!CryptographicPolicy_Validate() || !MemoryPolicy_Validate()) return false;
  frozen = true;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::IsFrozen()
* @brief      Is frozen
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::IsFrozen()
{
  DIOSTREAMTLSCONFIG_LOCK lock(configmutex);
  if(!lock.IsLocked()) return true;
  return frozen;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORD DIOSTREAMTLSCONFIG::GetCipherSuite()
* @brief      Get the TLS 1.3 cipher suite
* @ingroup    DATAIO
* 
* @return     XWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORD DIOSTREAMTLSCONFIG::GetCipherSuite()
{
  if(ciphersuites.IsEmpty()) return 0;

  return ciphersuites.Get(0);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTLSCONFIG::SetCipherSuite(XWORD ciphersuite)
* @brief      Set the TLS 1.3 cipher suite
* @ingroup    DATAIO
* 
* @param[in]  ciphersuite : Cipher suite value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLSCONFIG::SetCipherSuite(XWORD ciphersuite)
{
  if(IsFrozen() || !DIOSTREAMTLSCONFIG_IsCipherSuiteSupported(ciphersuite)) return;
  ciphersuites.DeleteAll();
  ciphersuites.Add(ciphersuite);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XVECTOR<XWORD>* DIOSTREAMTLSCONFIG::GetCipherSuites()
* @brief      Get the ordered TLS 1.3 cipher suite capabilities
* @ingroup    DATAIO
*
* @return     XVECTOR<XWORD>* : Pointer to the requested object; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<XWORD>* DIOSTREAMTLSCONFIG::GetCipherSuites()
{
  return &ciphersuites;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSCONFIG::CipherSuite_Add(XWORD ciphersuite)
* @brief      Add a supported TLS 1.3 cipher suite capability
* @ingroup    DATAIO
*
* @param[in]  ciphersuite : Cipher suite value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::CipherSuite_Add(XWORD ciphersuite)
{
  if(IsFrozen() || !DIOSTREAMTLSCONFIG_IsCipherSuiteSupported(ciphersuite)) return false;

  for(XDWORD c=0; c<ciphersuites.GetSize(); c++)
    {
      if(ciphersuites.Get(c) == ciphersuite) return false;
    }

  return ciphersuites.Add(ciphersuite);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSCONFIG::CipherSuites_Delete()
* @brief      Delete all TLS 1.3 cipher suite capabilities
* @ingroup    DATAIO
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::CipherSuites_Delete()
{
  if(IsFrozen()) return false;
  ciphersuites.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSCONFIG::CipherSuites_AddRecommendedProfile()
* @brief      Register the recommended TLS 1.3 cipher suite profile: every AEAD suite mandated/recommended by
*             RFC 8446 (AES-128-GCM, ChaCha20-Poly1305, AES-256-GCM), in preference order. Registering all three,
*             rather than a single suite, avoids limiting interoperability with peers that only implement a subset
*             (e.g. hardware without AES-NI prefers ChaCha20-Poly1305) while keeping every option equally strong.
*             Suites already present (added earlier via CipherSuite_Add()) are silently skipped.
* @ingroup    DATAIO
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::CipherSuites_AddRecommendedProfile()
{
  if(IsFrozen()) return false;

  CipherSuite_Add(DIOSTREAMTLS_MSG_CIPHER_AES_128_GCM_SHA256);
  CipherSuite_Add(DIOSTREAMTLS_MSG_CIPHER_CHACHA20_POLY1305_SHA256);
  CipherSuite_Add(DIOSTREAMTLS_MSG_CIPHER_AES_256_GCM_SHA384);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<XWORD>* DIOSTREAMTLSCONFIG::GetTLS12CipherSuites()
* @brief      Get tls12 cipher suites
* @ingroup    DATAIO
* 
* @return     XVECTOR<XWORD>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<XWORD>* DIOSTREAMTLSCONFIG::GetTLS12CipherSuites()
{
  return &TLS12ciphersuites;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::TLS12CipherSuite_Add(XWORD ciphersuite)
* @brief      Tls12 cipher suite add
* @ingroup    DATAIO
* 
* @param[in]  ciphersuite : Ciphersuite value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::TLS12CipherSuite_Add(XWORD ciphersuite)
{
  if(IsFrozen() || !DIOSTREAMTLSCONFIG_IsTLS12CipherSuiteSupported(ciphersuite)) return false;

  for(XDWORD c=0; c<TLS12ciphersuites.GetSize(); c++)
    {
      if(TLS12ciphersuites.Get(c) == ciphersuite) return false;
    }

  return TLS12ciphersuites.Add(ciphersuite);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::TLS12CipherSuites_Delete()
* @brief      Tls12 cipher suites delete
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::TLS12CipherSuites_Delete()
{
  if(IsFrozen()) return false;
  TLS12ciphersuites.DeleteAll();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XVECTOR<XWORD>* DIOSTREAMTLSCONFIG::GetSupportedGroups()
* @brief      Get the ordered key exchange group capabilities
* @ingroup    DATAIO
*
* @return     XVECTOR<XWORD>* : Pointer to the requested object; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<XWORD>* DIOSTREAMTLSCONFIG::GetSupportedGroups()
{
  return &supportedgroups;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSCONFIG::SupportedGroup_Add(XWORD supportedgroup)
* @brief      Add a supported key exchange group capability
* @ingroup    DATAIO
*
* @param[in]  supportedgroup : Supported group value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::SupportedGroup_Add(XWORD supportedgroup)
{
  if(IsFrozen()) return false;
  if(!DIOSTREAMTLSCONFIG_IsSupportedGroupSupported(supportedgroup)) return false;

  for(XDWORD c=0; c<supportedgroups.GetSize(); c++)
    {
      if(supportedgroups.Get(c) == supportedgroup) return false;
    }

  return supportedgroups.Add(supportedgroup);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSCONFIG::SupportedGroups_Delete()
* @brief      Delete all key exchange group capabilities
* @ingroup    DATAIO
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::SupportedGroups_Delete()
{
  if(IsFrozen()) return false;
  supportedgroups.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XVECTOR<XWORD>* DIOSTREAMTLSCONFIG::GetSignatureSchemes()
* @brief      Get the ordered TLS CertificateVerify signature capabilities
* @ingroup    DATAIO
*
* @return     XVECTOR<XWORD>* : Pointer to the requested object; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<XWORD>* DIOSTREAMTLSCONFIG::GetSignatureSchemes()
{
  return &signatureschemes;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSCONFIG::SignatureScheme_Add(XWORD signaturescheme)
* @brief      Add a supported TLS CertificateVerify signature capability
* @ingroup    DATAIO
*
* @param[in]  signaturescheme : Signature scheme value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::SignatureScheme_Add(XWORD signaturescheme)
{
  if(IsFrozen() || !DIOSTREAMTLSCONFIG_IsSignatureSchemeSupported(signaturescheme, false)) return false;

  for(XDWORD c=0; c<signatureschemes.GetSize(); c++)
    {
      if(signatureschemes.Get(c) == signaturescheme) return false;
    }

  return signatureschemes.Add(signaturescheme);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSCONFIG::SignatureSchemes_Delete()
* @brief      Delete all TLS CertificateVerify signature capabilities
* @ingroup    DATAIO
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::SignatureSchemes_Delete()
{
  if(IsFrozen()) return false;
  signatureschemes.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XVECTOR<XWORD>* DIOSTREAMTLSCONFIG::GetCertificateSignatureSchemes()
* @brief      Get the ordered X.509 certificate signature capabilities
* @ingroup    DATAIO
*
* @return     XVECTOR<XWORD>* : Pointer to the requested object; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<XWORD>* DIOSTREAMTLSCONFIG::GetCertificateSignatureSchemes()
{
  return &certificatesignatureschemes;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSCONFIG::CertificateSignatureScheme_Add(XWORD signaturescheme)
* @brief      Add a supported X.509 certificate signature capability
* @ingroup    DATAIO
*
* @param[in]  signaturescheme : Signature scheme value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::CertificateSignatureScheme_Add(XWORD signaturescheme)
{
  if(IsFrozen() || !DIOSTREAMTLSCONFIG_IsSignatureSchemeSupported(signaturescheme, true)) return false;

  for(XDWORD c=0; c<certificatesignatureschemes.GetSize(); c++)
    {
      if(certificatesignatureschemes.Get(c) == signaturescheme) return false;
    }

  return certificatesignatureschemes.Add(signaturescheme);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSCONFIG::CertificateSignatureSchemes_Delete()
* @brief      Delete all X.509 certificate signature capabilities
* @ingroup    DATAIO
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::CertificateSignatureSchemes_Delete()
{
  if(IsFrozen()) return false;
  certificatesignatureschemes.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XVECTOR<DIOSTREAMTLS_ALPN_TYPE>* DIOSTREAMTLSCONFIG::GetApplicationProtocols()
* @brief      Get the ordered ALPN application protocol capabilities
* @ingroup    DATAIO
*
* @return     XVECTOR<DIOSTREAMTLS_ALPN_TYPE>* : Pointer to the requested object; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<DIOSTREAMTLS_ALPN_TYPE>* DIOSTREAMTLSCONFIG::GetApplicationProtocols()
{
  return &applicationprotocols;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOSTREAMTLSCONFIG::GetApplicationProtocolsCount()
* @brief      Get application protocols count
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAMTLSCONFIG::GetApplicationProtocolsCount()
{
  XDWORD position = 0;
  XDWORD count = 0;
  while(position < applicationprotocolsraw.GetSize())
    {
      XBYTE size = applicationprotocolsraw.GetByte(position);
      if(!size || (position + 1 + size) > applicationprotocolsraw.GetSize()) return 0;
      position += 1 + size;
      count++;
    }
  return count;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::GetApplicationProtocol(XDWORD index, XBUFFER& applicationprotocol)
* @brief      Get application protocol
* @ingroup    DATAIO
* 
* @param[in]  index : Index value.
* @param[in]  applicationprotocol : Applicationprotocol value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::GetApplicationProtocol(XDWORD index, XBUFFER& applicationprotocol)
{
  XDWORD position = 0;
  XDWORD count = 0;
  applicationprotocol.Delete();
  while(position < applicationprotocolsraw.GetSize())
    {
      XBYTE size = applicationprotocolsraw.GetByte(position);
      if(!size || (position + 1 + size) > applicationprotocolsraw.GetSize()) return false;
      if(count == index) return applicationprotocol.Add(&applicationprotocolsraw.Get()[position + 1], size);
      position += 1 + size;
      count++;
    }
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::ApplicationProtocol_Add(XBUFFER& applicationprotocol)
* @brief      Application protocol add
* @ingroup    DATAIO
* 
* @param[in]  applicationprotocol : Applicationprotocol value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::ApplicationProtocol_Add(XBUFFER& applicationprotocol)
{
  if(IsFrozen()) return false;
  if(applicationprotocol.IsEmpty() || applicationprotocol.GetSize() > 255) return false;
  if((applicationprotocol.GetSize() == 2) &&
     (applicationprotocol.GetByte(0) == (XBYTE)'h') &&
     (applicationprotocol.GetByte(1) == (XBYTE)'3')) return false;
  if(applicationprotocol.GetSize() >= 3 && applicationprotocol.GetByte(0)=='h' &&
     applicationprotocol.GetByte(1)=='3' && applicationprotocol.GetByte(2)=='-') return false;
  if(applicationprotocolsraw.GetSize() > (0xFFFF - 1 - applicationprotocol.GetSize())) return false;
  for(XDWORD c=0; c<GetApplicationProtocolsCount(); c++)
    {
      XBUFFER existing;
      if(GetApplicationProtocol(c, existing) && existing.Compare(applicationprotocol)) return false;
    }
  return applicationprotocolsraw.Add((XBYTE)applicationprotocol.GetSize()) && applicationprotocolsraw.Add(applicationprotocol);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSCONFIG::ApplicationProtocol_Add(DIOSTREAMTLS_ALPN_TYPE applicationprotocol)
* @brief      Add an ALPN application protocol capability
* @ingroup    DATAIO
*
* @param[in]  applicationprotocol : Application protocol value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::ApplicationProtocol_Add(DIOSTREAMTLS_ALPN_TYPE applicationprotocol)
{
  if(IsFrozen()) return false;
  XBUFFER raw;
  switch(applicationprotocol)
    {
      case DIOSTREAMTLS_ALPN_TYPE_HTTP_1_1 : { static XBYTE value[] = { 'h','t','t','p','/','1','.','1' }; raw.Add(value, sizeof(value)); } break;
      case DIOSTREAMTLS_ALPN_TYPE_HTTP_2   : { static XBYTE value[] = { 'h','2' }; raw.Add(value, sizeof(value)); } break;
      case DIOSTREAMTLS_ALPN_TYPE_HTTP_3   : return false;
                                      default : return false;
    }

  for(XDWORD c=0; c<applicationprotocols.GetSize(); c++)
    {
      if(applicationprotocols.Get(c) == applicationprotocol) return false;
    }

  if(!ApplicationProtocol_Add(raw)) return false;
  return applicationprotocols.Add(applicationprotocol);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSCONFIG::ApplicationProtocols_Delete()
* @brief      Delete all ALPN application protocol capabilities
* @ingroup    DATAIO
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::ApplicationProtocols_Delete()
{
  if(IsFrozen()) return false;
  applicationprotocolsraw.Delete();
  applicationprotocols.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOSTREAMTLSCONFIG::GetServerName()
* @brief      Get the expected server name and SNI override
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOSTREAMTLSCONFIG::GetServerName()
{
  return &servername;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<XBUFFER*>* DIOSTREAMTLSCONFIG::GetTrustedRoots()
* @brief      Get the explicitly configured DER trust anchors
* @ingroup    DATAIO
* 
* @return     XVECTOR<XBUFFER*>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<XBUFFER*>* DIOSTREAMTLSCONFIG::GetTrustedRoots()
{
  return &trustedroots;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::TrustedRoot_Add(XBUFFER& root)
* @brief      Add a copied DER trust anchor
* @ingroup    DATAIO
* 
* @param[in]  root : DER certificate to copy.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::TrustedRoot_Add(XBUFFER& root)
{
  if(IsFrozen()) return false;
  XBUFFER* copy;
  XDWORD total = 0;

  if(root.IsEmpty() || root.GetSize() > memorypolicy.GetMaximumTrustCertificateSize() ||
     trustedroots.GetSize() >= memorypolicy.GetMaximumTrustRoots()) return false;

  for(XDWORD c=0; c<trustedroots.GetSize(); c++)
    {
      XBUFFER* existing = trustedroots.Get(c);
      if(!existing) continue;
      total += existing->GetSize();
      if(existing->GetSize() == root.GetSize() && !memcmp(existing->Get(), root.Get(), root.GetSize())) return true;
    }
  if(root.GetSize() > memorypolicy.GetMaximumTrustStoreSize() ||
     total > (memorypolicy.GetMaximumTrustStoreSize()-root.GetSize())) return false;

  copy = GEN_NEW XBUFFER();
  if(!copy) return false;

  if(!copy->Add(root) || !trustedroots.Add(copy))
    {
      GEN_DELETE copy;
      return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSCONFIG::TrustedRoots_Load(CIPHERTRUSTPROVIDERX509& provider)
* @brief      Replace the configured trust anchors with the roots supplied by a GEN trust provider
* @ingroup    DATAIO
*
* @param[in]  provider : Trust provider to load.
*
* @return     bool : true if at least one trust anchor is loaded; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::TrustedRoots_Load(CIPHERTRUSTPROVIDERX509& provider)
{
  if(IsFrozen()) return false;
  XVECTOR<XBUFFER*>* roots;

  if(!provider.Load()) return false;

  roots = provider.GetRoots();
  if(!roots || roots->IsEmpty()) return false;

  TrustedRoots_Delete();

  for(XDWORD c=0; c<roots->GetSize(); c++)
    {
      XBUFFER* root = roots->Get(c);

      if(root && !TrustedRoot_Add((*root)))
        {
          TrustedRoots_Delete();
          return false;
        }
    }

  return !trustedroots.IsEmpty();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSCONFIG::TrustedRoots_AddDefaults()
* @brief      Add the trust anchors supplied by the embedded GEN trust provider
* @ingroup    DATAIO
*
* @return     bool : true if at least one trust anchor is added; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::TrustedRoots_AddDefaults()
{
  if(IsFrozen()) return false;
  if(!trustedroots.IsEmpty()) return false;

  // Prefer OS trust material where it is available through portable file I/O
  // (Linux/Android). Windows uses the embedded GEN bundle (cacert.h) instead of
  // CryptoAPI so TLS does not depend on crypt32.
#if defined(ANDROID)
  CIPHERTRUSTPROVIDERX509ANDROID platformprovider;
#elif defined(LINUX)
  CIPHERTRUSTPROVIDERX509LINUX platformprovider;
#else
  CIPHERTRUSTPROVIDERX509GEN platformprovider;
#endif
  if(TrustedRoots_Load(platformprovider)) return true;

#if defined(ANDROID) || defined(LINUX)
  if(truststorefallbackpolicy == DIOSTREAMTLS_TRUSTSTORE_FALLBACKPOLICY_NEVER) return false;
  CIPHERTRUSTPROVIDERX509GEN fallbackprovider;
  return TrustedRoots_Load(fallbackprovider);
#else
  return false;
#endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTLS_TRUSTSTORE_FALLBACKPOLICY DIOSTREAMTLSCONFIG::GetTrustStoreFallbackPolicy()
* @brief      Get trust store fallback policy
* @ingroup    DATAIO
* 
* @return     DIOSTREAMTLS_TRUSTSTORE_FALLBACKPOLICY : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_TRUSTSTORE_FALLBACKPOLICY DIOSTREAMTLSCONFIG::GetTrustStoreFallbackPolicy()
{
  return truststorefallbackpolicy;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::SetTrustStoreFallbackPolicy(DIOSTREAMTLS_TRUSTSTORE_FALLBACKPOLICY policy)
* @brief      Set trust store fallback policy
* @ingroup    DATAIO
* 
* @param[in]  policy : Policy value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::SetTrustStoreFallbackPolicy(DIOSTREAMTLS_TRUSTSTORE_FALLBACKPOLICY policy)
{
  if(IsFrozen() || ((policy != DIOSTREAMTLS_TRUSTSTORE_FALLBACKPOLICY_NEVER) &&
                    (policy != DIOSTREAMTLS_TRUSTSTORE_FALLBACKPOLICY_ON_NATIVE_FAILURE))) return false;
  truststorefallbackpolicy = policy;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::TrustedRoots_Delete()
* @brief      Delete all copied trust anchors
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::TrustedRoots_Delete()
{
  if(IsFrozen()) return false;
  trustedroots.DeleteContents();
  trustedroots.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTLS_CLIENTAUTHENTICATION_MODE DIOSTREAMTLSCONFIG::GetClientAuthenticationMode()
* @brief      Get client authentication mode
* @ingroup    DATAIO
* 
* @return     DIOSTREAMTLS_CLIENTAUTHENTICATION_MODE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_CLIENTAUTHENTICATION_MODE DIOSTREAMTLSCONFIG::GetClientAuthenticationMode()
{
  return clientauthenticationmode;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTLSCONFIG::SetClientAuthenticationMode(DIOSTREAMTLS_CLIENTAUTHENTICATION_MODE mode)
* @brief      Set client authentication mode
* @ingroup    DATAIO
* 
* @param[in]  mode : Mode value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLSCONFIG::SetClientAuthenticationMode(DIOSTREAMTLS_CLIENTAUTHENTICATION_MODE mode)
{
  if(IsFrozen()) return;
  clientauthenticationmode = mode;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<XBUFFER*>* DIOSTREAMTLSCONFIG::GetClientTrustedRoots()
* @brief      Get client trusted roots
* @ingroup    DATAIO
* 
* @return     XVECTOR<XBUFFER*>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<XBUFFER*>* DIOSTREAMTLSCONFIG::GetClientTrustedRoots()
{
  return &clienttrustedroots;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::ClientTrustedRoot_Add(XBUFFER& root)
* @brief      Client trusted root add
* @ingroup    DATAIO
* 
* @param[in]  root : Root value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::ClientTrustedRoot_Add(XBUFFER& root)
{
  if(IsFrozen()) return false;
  XBUFFER* copy;
  XDWORD total = 0;

  if(root.IsEmpty() || root.GetSize() > memorypolicy.GetMaximumTrustCertificateSize() ||
     clienttrustedroots.GetSize() >= memorypolicy.GetMaximumTrustRoots()) return false;

  for(XDWORD c=0; c<clienttrustedroots.GetSize(); c++)
    {
      XBUFFER* existing = clienttrustedroots.Get(c);
      if(!existing) continue;
      total += existing->GetSize();
      if(existing->GetSize() == root.GetSize() && !memcmp(existing->Get(), root.Get(), root.GetSize())) return true;
    }
  if(root.GetSize() > memorypolicy.GetMaximumTrustStoreSize() ||
     total > (memorypolicy.GetMaximumTrustStoreSize()-root.GetSize())) return false;

  copy = GEN_NEW XBUFFER();
  if(!copy) return false;

  if(!copy->Add(root) || !clienttrustedroots.Add(copy))
    {
      GEN_DELETE copy;
      return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::ClientTrustedRoots_Load(CIPHERTRUSTPROVIDERX509& provider)
* @brief      Client trusted roots load
* @ingroup    DATAIO
* 
* @param[in]  provider : Provider value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::ClientTrustedRoots_Load(CIPHERTRUSTPROVIDERX509& provider)
{
  if(IsFrozen()) return false;
  XVECTOR<XBUFFER*>* roots;

  if(!provider.Load()) return false;

  roots = provider.GetRoots();
  if(!roots || roots->IsEmpty()) return false;

  ClientTrustedRoots_Delete();

  for(XDWORD c=0; c<roots->GetSize(); c++)
    {
      XBUFFER* root = roots->Get(c);
      if(root && !ClientTrustedRoot_Add((*root)))
        {
          ClientTrustedRoots_Delete();
          return false;
        }
    }

  return !clienttrustedroots.IsEmpty();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::ClientTrustedRoots_Delete()
* @brief      Client trusted roots delete
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::ClientTrustedRoots_Delete()
{
  if(IsFrozen()) return false;
  clienttrustedroots.DeleteContents();
  clienttrustedroots.DeleteAll();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XVECTOR<XBUFFER*>* DIOSTREAMTLSCONFIG::GetLocalCertificateChain()
* @brief      Get the copied DER certificate chain of the local end
* @ingroup    DATAIO
*
* @return     XVECTOR<XBUFFER*>* : Pointer to the requested object; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<XBUFFER*>* DIOSTREAMTLSCONFIG::GetLocalCertificateChain()
{
  return &localcertificatechain;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSCONFIG::LocalCertificate_Add(XBUFFER& certificate)
* @brief      Add a copied DER certificate to the local chain
* @ingroup    DATAIO
*
* @param[in]  certificate : DER certificate to copy. The leaf certificate must be added first.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::LocalCertificate_Add(XBUFFER& certificate)
{
  if(IsFrozen()) return false;
  XBUFFER* copy;

  if(certificate.IsEmpty()) return false;

  copy = GEN_NEW XBUFFER();
  if(!copy) return false;

  if(!copy->Add(certificate) || !localcertificatechain.Add(copy))
    {
      GEN_DELETE copy;
      return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSCONFIG::LocalCertificates_Delete()
* @brief      Delete all copied certificates of the local chain
* @ingroup    DATAIO
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::LocalCertificates_Delete()
{
  if(IsFrozen()) return false;
  localcertificatechain.DeleteContents();
  localcertificatechain.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         CIPHERKEY* DIOSTREAMTLSCONFIG::GetLocalPrivateKey()
* @brief      Get the copied private key of the local end
* @ingroup    DATAIO
*
* @return     CIPHERKEY* : Pointer to the requested object; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERKEY* DIOSTREAMTLSCONFIG::GetLocalPrivateKey()
{
  return localprivatekey;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBUFFER* DIOSTREAMTLSCONFIG::GetLocalOCSPStapledResponse()
* @brief      Get local ocsp stapled response
* @ingroup    DATAIO
* 
* @return     XBUFFER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* DIOSTREAMTLSCONFIG::GetLocalOCSPStapledResponse()
{
  return &localOCSPstapledresponse;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::SetLocalOCSPStapledResponse(XBUFFER& response)
* @brief      Set local ocsp stapled response
* @ingroup    DATAIO
* 
* @param[in]  response : Response value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::SetLocalOCSPStapledResponse(XBUFFER& response)
{
  if(IsFrozen() || response.IsEmpty() || (response.GetSize() > memorypolicy.GetMaximumOCSPResponseSize())) return false;

  localOCSPstapledresponse.Delete();
  return localOCSPstapledresponse.Add(response);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::DeleteLocalOCSPStapledResponse()
* @brief      Delete local ocsp stapled response
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::DeleteLocalOCSPStapledResponse()
{
  if(IsFrozen()) return false;
  localOCSPstapledresponse.Delete();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSCONFIG::SetLocalPrivateKey(CIPHERKEY* privatekey)
* @brief      Copy the private key of the local end
* @note       Accepts an RSA private key, an ECDSA private key for one of the curves CIPHERECDSA implements
*             (P-256/P-384/P-521 -- see CIPHERECDSA::Parameters_Set()), or an Ed25519 private key when Ed25519
*             support is active. Any other key type is rejected. The matching CertificateSignatureScheme_Add()/
*             SignatureScheme_Add() calls (see
*             DoDefault() below) still need to be picked to match whichever key ends up here -- callers building
*             a server profile from a loaded key, such as APPFLOWWEBSERVER::Ini_BuildTLSConfig(), should offer
*             only the scheme(s) that pair with this key's type.
* @ingroup    DATAIO
*
* @param[in]  privatekey : Private key to copy.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::SetLocalPrivateKey(CIPHERKEY* privatekey)
{
  if(IsFrozen()) return false;
  if(!privatekey || !privatekey->GetSizeInBytes()) return false;

  switch(privatekey->GetType())
    {
      case CIPHERKEYTYPE_RSA_PRIVATE :
        {
          CIPHERKEYPRIVATERSA* copy;

          copy = GEN_NEW CIPHERKEYPRIVATERSA();
          if(!copy) return false;

          if(!copy->CopyFrom((CIPHERKEYPRIVATERSA*)privatekey))
            {
              GEN_DELETE copy;
              return false;
            }

          if(localprivatekey) GEN_DELETE localprivatekey;

          localprivatekey = copy;
        }
        return true;

      case CIPHERKEYTYPE_ECDSA_SECP256R1_PRIVATE :
      case CIPHERKEYTYPE_ECDSA_SECP384R1_PRIVATE :
      case CIPHERKEYTYPE_ECDSA_SECP521R1_PRIVATE :
        {
          CIPHERKEYECDSA* copy;

          copy = GEN_NEW CIPHERKEYECDSA();
          if(!copy) return false;

          if(!copy->CopyFrom((CIPHERKEYSYMMETRICAL*)privatekey))
            {
              GEN_DELETE copy;
              return false;
            }

          if(localprivatekey) GEN_DELETE localprivatekey;

          localprivatekey = copy;
        }
        return true;

      #ifdef CIPHER_ASYMMETRIC_ED25519_ACTIVE
      case CIPHERKEYTYPE_ED25519_PRIVATE :
        {
          CIPHERKEYSYMMETRICAL* copy;

          copy = GEN_NEW CIPHERKEYSYMMETRICAL();
          if(!copy) return false;

          if(!copy->CopyFrom((CIPHERKEYSYMMETRICAL*)privatekey))
            {
              GEN_DELETE copy;
              return false;
            }

          if(localprivatekey) GEN_DELETE localprivatekey;

          localprivatekey = copy;
        }
        return true;
      #endif

                                        default : break;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSCONFIG::HasLocalCredentials()
* @brief      Check whether the local certificate chain and private key are configured
* @ingroup    DATAIO
*
* @return     bool : true if the condition is met; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::HasLocalCredentials()
{
  if(localcertificatechain.IsEmpty() || !localprivatekey) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSCONFIG::LocalCredentials_Validate()
* @brief      Validate the local TLS certificate chain and private key before using them in server mode
* @ingroup    DATAIO
*
* @return     bool : true if the local credentials are coherent and usable; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::LocalCredentials_Validate()
{
  localcredentialserror = DIOSTREAMTLS_LOCALCREDENTIALSERROR_NONE;

  if(localOCSPstapledresponse.GetSize() > memorypolicy.GetMaximumOCSPResponseSize())
    {
      localcredentialserror = DIOSTREAMTLS_LOCALCREDENTIALSERROR_INVALIDCERTIFICATE;
      return false;
    }

  if(!Credentials_Validate(&localcertificatechain, localprivatekey)) return false;

  for(XDWORD c=0; c<servercredentials.GetSize(); c++)
    {
      DIOSTREAMTLSSERVERCREDENTIALS* credentials = servercredentials.Get(c);

      if(!credentials || credentials->GetServerName()->IsEmpty() ||
         !Credentials_Validate(credentials->GetCertificateChain(), credentials->GetPrivateKey()) ||
         (credentials->GetOCSPStapledResponse()->GetSize() > memorypolicy.GetMaximumOCSPResponseSize()))
        {
          if(localcredentialserror == DIOSTREAMTLS_LOCALCREDENTIALSERROR_NONE)
            {
              localcredentialserror = DIOSTREAMTLS_LOCALCREDENTIALSERROR_NOTCONFIGURED;
            }

          return false;
        }

      XBUFFER* certificateDER = credentials->GetCertificateChain()->Get(0);
      CIPHERCERTIFICATEX509 certificate;
      XSTRING servernametest((*credentials->GetServerName()));
      bool    servernamematch = false;

      if(certificateDER && certificate.Decode((*certificateDER)))
        {
          if((servernametest.GetSize() >= 2) && (servernametest[0] == _C('*')) && (servernametest[1] == _C('.')))
            {
              for(XDWORD d=0; d<certificate.GetSubjectAlternativeNamesDNS()->GetSize(); d++)
                {
                  XSTRING* subjectname = certificate.GetSubjectAlternativeNamesDNS()->Get(d);
                  if(!subjectname) continue;

                  XSTRING normalized((*subjectname));
                  if(normalized.Character_GetLast() == _C('.')) normalized.DeleteLastCharacter();

                  if(!normalized.Compare(servernametest, true))
                    {
                      servernamematch = true;
                      break;
                    }
                }
            }
           else
            {
              servernamematch = certificate.IsServerNameValid(servernametest.Get());
            }
        }

      if(!servernamematch)
        {
          localcredentialserror = DIOSTREAMTLS_LOCALCREDENTIALSERROR_INVALIDSERVERNAME;
          return false;
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSCONFIG::Credentials_Validate(XVECTOR<XBUFFER*>* certificatechain, CIPHERKEY* privatekey)
* @brief      Validate one server certificate chain/private key pair
* @note       INTERNAL
* @ingroup    DATAIO
*
* @param[in]  certificatechain : Certificate chain to validate.
* @param[in]  privatekey : Private key associated with the leaf certificate.
*
* @return     bool : true if the credentials are coherent and usable; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::Credentials_Validate(XVECTOR<XBUFFER*>* certificatechain, CIPHERKEY* privatekey)
{
  XVECTOR<CIPHERCERTIFICATEX509*> certificates;
  XBUFFER                         content;
  XBUFFER                         signature;

  if(!certificatechain || certificatechain->IsEmpty() || !privatekey ||
     (certificatechain->GetSize() > certificatevalidationpolicy.GetMaximumChainDepth()))
    {
      localcredentialserror = DIOSTREAMTLS_LOCALCREDENTIALSERROR_NOTCONFIGURED;
      return false;
    }

  XDWORD certificatebytes = 0;
  for(XDWORD c=0; c<certificatechain->GetSize(); c++)
    {
      XBUFFER*               certificateDER = certificatechain->Get(c);
      CIPHERCERTIFICATEX509* certificate;

      if(!certificateDER || certificateDER->IsEmpty())
        {
          localcredentialserror = DIOSTREAMTLS_LOCALCREDENTIALSERROR_INVALIDCERTIFICATE;
          certificates.DeleteContents();
          certificates.DeleteAll();
          return false;
        }

      if(certificateDER->GetSize() > memorypolicy.GetMaximumHandshakeInputSize() ||
         certificatebytes > (memorypolicy.GetMaximumHandshakeInputSize()-certificateDER->GetSize()))
        {
          localcredentialserror = DIOSTREAMTLS_LOCALCREDENTIALSERROR_INVALIDCERTIFICATE;
          certificates.DeleteContents();
          certificates.DeleteAll();
          return false;
        }
      certificatebytes += certificateDER->GetSize();

      certificate = GEN_NEW CIPHERCERTIFICATEX509();
      if(!certificate || !certificate->Decode((*certificateDER)))
        {
          if(certificate) GEN_DELETE certificate;
          localcredentialserror = DIOSTREAMTLS_LOCALCREDENTIALSERROR_INVALIDCERTIFICATE;
          certificates.DeleteContents();
          certificates.DeleteAll();
          return false;
        }

      if(!certificates.Add(certificate))
        {
          GEN_DELETE certificate;
          localcredentialserror = DIOSTREAMTLS_LOCALCREDENTIALSERROR_INVALIDCERTIFICATE;
          certificates.DeleteContents();
          certificates.DeleteAll();
          return false;
        }
    }

  for(XDWORD c=0; c<certificates.GetSize(); c++)
    {
      CIPHERCERTIFICATEX509* certificate = certificates.Get(c);

      if(!certificate || certificate->HasUnknownCriticalExtension())
        {
          localcredentialserror = DIOSTREAMTLS_LOCALCREDENTIALSERROR_INVALIDCERTIFICATE;
          certificates.DeleteContents();
          certificates.DeleteAll();
          return false;
        }

      if(!certificate->IsValidDates())
        {
          localcredentialserror = DIOSTREAMTLS_LOCALCREDENTIALSERROR_INVALIDDATE;
          certificates.DeleteContents();
          certificates.DeleteAll();
          return false;
        }

      if(!c)
        {
          if(!certificate->IsPublicCipherKeyValid())
            {
              localcredentialserror = DIOSTREAMTLS_LOCALCREDENTIALSERROR_INVALIDCERTIFICATE;
              certificates.DeleteContents();
              certificates.DeleteAll();
              return false;
            }

          if(certificate->IsCertificateAuthority() ||
             (certificate->HasKeyUsage() && !certificate->IsKeyUsageDigitalSignature()) ||
             (certificate->HasExtendedKeyUsage() && !certificate->IsExtendedKeyUsageServerAuthentication()))
            {
              localcredentialserror = DIOSTREAMTLS_LOCALCREDENTIALSERROR_INVALIDKEYUSAGE;
              certificates.DeleteContents();
              certificates.DeleteAll();
              return false;
            }
        }
       else
        {
          if(!certificate->IsCertificateAuthority() ||
             (certificate->HasKeyUsage() && !certificate->IsKeyUsageCertificateSign()))
            {
              localcredentialserror = DIOSTREAMTLS_LOCALCREDENTIALSERROR_INVALIDKEYUSAGE;
              certificates.DeleteContents();
              certificates.DeleteAll();
              return false;
            }

          if((certificate->GetBasicConstraintsPathLength() >= 0) &&
             ((int)c - 1 > certificate->GetBasicConstraintsPathLength()))
            {
              localcredentialserror = DIOSTREAMTLS_LOCALCREDENTIALSERROR_INVALIDCHAIN;
              certificates.DeleteContents();
              certificates.DeleteAll();
              return false;
            }
        }
    }

  for(XDWORD c=0; c+1<certificates.GetSize(); c++)
    {
      CIPHERCERTIFICATEX509* certificate = certificates.Get(c);
      CIPHERCERTIFICATEX509* issuer      = certificates.Get(c+1);

      if(!certificate->GetIssuerData()->Compare((*issuer->GetSubjectData())) ||
         !certificate->VerifySignature(issuer->GetPublicCipherKey()))
        {
          localcredentialserror = DIOSTREAMTLS_LOCALCREDENTIALSERROR_INVALIDCHAIN;
          certificates.DeleteContents();
          certificates.DeleteAll();
          return false;
        }
    }

  CIPHERCERTIFICATEX509* leaf      = certificates.Get(0);
  CIPHERKEY*             publickey = leaf?leaf->GetPublicCipherKey():NULL;

  if(!publickey)
    {
      localcredentialserror = DIOSTREAMTLS_LOCALCREDENTIALSERROR_UNSUPPORTEDKEY;
      certificates.DeleteContents();
      certificates.DeleteAll();
      return false;
    }

  static const XBYTE testcontent[] = { 0x47, 0x45, 0x4E, 0x2D, 0x54, 0x4C, 0x53, 0x2D,
                                       0x43, 0x52, 0x45, 0x44, 0x45, 0x4E, 0x54, 0x49,
                                       0x41, 0x4C, 0x53 };

  if(!content.Add((XBYTE*)testcontent, sizeof(testcontent)))
    {
      localcredentialserror = DIOSTREAMTLS_LOCALCREDENTIALSERROR_KEYMISMATCH;
      certificates.DeleteContents();
      certificates.DeleteAll();
      return false;
    }

  bool havesupportedscheme = false;
  bool keypairvalid        = false;

  for(XDWORD c=0; c<signatureschemes.GetSize(); c++)
    {
      XWORD signaturescheme = signatureschemes.Get(c);

      if(!DIOSTREAMTLSSIGNATURE::IsSupported(signaturescheme, publickey)) continue;

      havesupportedscheme = true;
      signature.Delete();

      if(DIOSTREAMTLSSIGNATURE::Sign(signaturescheme, privatekey, publickey, content, signature) &&
         /**-------------------------------------------------------------------------------------------------------------------
         * 
         * @fn         DIOSTREAMTLSSIGNATURE::Verify(signaturescheme, publickey, content, signature))
         * @brief      Verify
         * @ingroup    DATAIO
         * 
         * 
         * --------------------------------------------------------------------------------------------------------------------*/
         DIOSTREAMTLSSIGNATURE::Verify(signaturescheme, publickey, content, signature))
        {
          keypairvalid = true;
          break;
        }
    }

  if(!havesupportedscheme || !keypairvalid)
    {
      localcredentialserror = havesupportedscheme?DIOSTREAMTLS_LOCALCREDENTIALSERROR_KEYMISMATCH:
                                                 DIOSTREAMTLS_LOCALCREDENTIALSERROR_UNSUPPORTEDKEY;
      certificates.DeleteContents();
      certificates.DeleteAll();
      return false;
    }

  certificates.DeleteContents();
  certificates.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_LOCALCREDENTIALSERROR DIOSTREAMTLSCONFIG::GetLocalCredentialsError()
* @brief      Get the result of the last local credential validation
* @ingroup    DATAIO
*
* @return     DIOSTREAMTLS_LOCALCREDENTIALSERROR : Last validation result.
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_LOCALCREDENTIALSERROR DIOSTREAMTLSCONFIG::GetLocalCredentialsError()
{
  return localcredentialserror;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSCONFIG::LocalCredentials_Delete()
* @brief      Delete the copied certificate chain and private key of the local end
* @ingroup    DATAIO
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::LocalCredentials_Delete()
{
  if(IsFrozen()) return false;
  LocalCertificates_Delete();
  DeleteLocalOCSPStapledResponse();

  if(localprivatekey)
    {
      GEN_DELETE localprivatekey;
      localprivatekey = NULL;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLSSERVERCREDENTIALS* DIOSTREAMTLSCONFIG::ServerCredentials_Add(XCHAR* servername)
* @brief      Add a server credential set selected by SNI
* @ingroup    DATAIO
*
* @param[in]  servername : Exact DNS name or single-label wildcard pattern.
*
* @return     DIOSTREAMTLSSERVERCREDENTIALS* : New credential set; NULL on error.
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLSSERVERCREDENTIALS* DIOSTREAMTLSCONFIG::ServerCredentials_Add(XCHAR* servername)
{
  if(IsFrozen()) return NULL;
  DIOSTREAMTLSSERVERCREDENTIALS* credentials;
  XSTRING                         pattern;

  if(!servername || !servername[0]) return NULL;

  pattern = servername;
  if(pattern.Character_GetLast() == _C('.')) pattern.DeleteLastCharacter();
  if(pattern.IsEmpty()) return NULL;

  if(pattern.FindCharacter(_C('*')) >= 0)
    {
      if((pattern.GetSize() < 3) || (pattern[0] != _C('*')) || (pattern[1] != _C('.')) ||
         (pattern.FindCharacter(_C('*'), 1) >= 0)) return NULL;
    }

  // More than one credential set may intentionally use the same SNI pattern.  TLS 1.3 can then choose
  // the best certificate/key pair according to the client's signature_algorithms and
  // signature_algorithms_cert extensions (for example Ed25519, ECDSA and RSA alternatives).

  credentials = GEN_NEW DIOSTREAMTLSSERVERCREDENTIALS();
  if(!credentials) return NULL;

  credentials->GetServerName()->Set(pattern);

  if(!servercredentials.Add(credentials))
    {
      GEN_DELETE credentials;
      return NULL;
    }

  return credentials;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XVECTOR<DIOSTREAMTLSSERVERCREDENTIALS*>* DIOSTREAMTLSCONFIG::GetServerCredentials()
* @brief      Get all SNI-specific server credentials
* @ingroup    DATAIO
*
* @return     XVECTOR<DIOSTREAMTLSSERVERCREDENTIALS*>* : Credential list.
*
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<DIOSTREAMTLSSERVERCREDENTIALS*>* DIOSTREAMTLSCONFIG::GetServerCredentials()
{
  return &servercredentials;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSCONFIG::ServerCredentials_Select(XCHAR* servername, XVECTOR<XBUFFER*>*& certificatechain, CIPHERKEY*& privatekey)
* @brief      Select server credentials for an SNI hostname, falling back to the default credentials
* @ingroup    DATAIO
*
* @param[in]  servername : Requested SNI DNS name; NULL or empty when SNI is absent.
* @param[out] certificatechain : Selected certificate chain.
* @param[out] privatekey : Selected private key.
*
* @return     bool : true if usable credentials are available; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::ServerCredentials_Select(XCHAR* servername, XVECTOR<XBUFFER*>*& certificatechain, CIPHERKEY*& privatekey)
{
  XBUFFER* OCSPstapledresponse = NULL;
  return ServerCredentials_Select(servername, certificatechain, privatekey, OCSPstapledresponse);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::ServerCredentials_Select(XCHAR* servername, XVECTOR<XBUFFER*>*& certificatechain, CIPHERKEY*& privatekey, XBUFFER*& OCSPstapledresponse)
* @brief      Server credentials select
* @ingroup    DATAIO
* 
* @param[in]  servername : Pointer to servername.
* @param[in]  certificatechain : Pointer to certificatechain.
* @param[in]  privatekey : Pointer to privatekey.
* @param[in]  OCSPstapledresponse : Pointer to OCSPstapledresponse.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::ServerCredentials_Select(XCHAR* servername, XVECTOR<XBUFFER*>*& certificatechain, CIPHERKEY*& privatekey, XBUFFER*& OCSPstapledresponse)
{
  certificatechain = NULL;
  privatekey        = NULL;
  OCSPstapledresponse = NULL;

  if(servername && servername[0])
    {
      // Exact names always win over wildcard entries regardless of insertion order.
      for(XDWORD c=0; c<servercredentials.GetSize(); c++)
        {
          DIOSTREAMTLSSERVERCREDENTIALS* credentials = servercredentials.Get(c);
          if(!credentials || !credentials->HasCredentials()) continue;

          XSTRING pattern((*credentials->GetServerName()));
          if(pattern.FindCharacter(_C('*')) >= 0) continue;

          if(DIOSTREAMTLSCONFIG_ServerNameMatch(pattern, servername))
            {
              certificatechain = credentials->GetCertificateChain();
              privatekey        = credentials->GetPrivateKey();
              OCSPstapledresponse = credentials->GetOCSPStapledResponse();
              return true;
            }
        }

      for(XDWORD c=0; c<servercredentials.GetSize(); c++)
        {
          DIOSTREAMTLSSERVERCREDENTIALS* credentials = servercredentials.Get(c);
          if(!credentials || !credentials->HasCredentials()) continue;

          XSTRING pattern((*credentials->GetServerName()));
          if(pattern.FindCharacter(_C('*')) < 0) continue;

          if(DIOSTREAMTLSCONFIG_ServerNameMatch(pattern, servername))
            {
              certificatechain = credentials->GetCertificateChain();
              privatekey        = credentials->GetPrivateKey();
              OCSPstapledresponse = credentials->GetOCSPStapledResponse();
              return true;
            }
        }
    }

  if(!HasLocalCredentials()) return false;

  certificatechain = &localcertificatechain;
  privatekey        = localprivatekey;
  OCSPstapledresponse = &localOCSPstapledresponse;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSCONFIG::ServerCredentials_Delete()
* @brief      Delete all SNI-specific server credentials
* @ingroup    DATAIO
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::ServerCredentials_Delete()
{
  if(IsFrozen()) return false;
  servercredentials.DeleteContents();
  servercredentials.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTLS_REVOCATIONPOLICY DIOSTREAMTLSCONFIG::GetRevocationPolicy()
* @brief      Get revocation policy
* @ingroup    DATAIO
* 
* @return     DIOSTREAMTLS_REVOCATIONPOLICY : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_REVOCATIONPOLICY DIOSTREAMTLSCONFIG::GetRevocationPolicy()
{
  return revocationpolicy;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::SetRevocationPolicy(DIOSTREAMTLS_REVOCATIONPOLICY policy)
* @brief      Set revocation policy
* @ingroup    DATAIO
* 
* @param[in]  policy : Policy value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::SetRevocationPolicy(DIOSTREAMTLS_REVOCATIONPOLICY policy)
{
  if(IsFrozen() || policy > DIOSTREAMTLS_REVOCATIONPOLICY_MUST_STAPLE) return false;
  revocationpolicy = policy;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSCONFIG::IsActiveAIAFetch()
* @brief      Check whether a missing-intermediate chain may be completed via AuthorityInfoAccess fetching
* @ingroup    DATAIO
*
* @return     bool : true if the condition is met; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::IsActiveAIAFetch()
{
  return aiafetchactive;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLSCONFIG::AIAFetch_Activate(bool activate)
* @brief      Activate or deactivate AuthorityInfoAccess fetching
* @ingroup    DATAIO
*
* @param[in]  activate : true to activate; false to deactivate.
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLSCONFIG::AIAFetch_Activate(bool activate)
{
  if(IsFrozen()) return;
  aiafetchactive = activate;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int DIOSTREAMTLSCONFIG::GetAIAFetchTimeout()
* @brief      Get the AuthorityInfoAccess fetch timeout, in seconds
* @ingroup    DATAIO
*
* @return     int : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
int DIOSTREAMTLSCONFIG::GetAIAFetchTimeout()
{
  return aiafetchtimeout;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSCONFIG::SetAIAFetchTimeout(int timeout)
* @brief      Set the AuthorityInfoAccess fetch timeout, in seconds
* @ingroup    DATAIO
*
* @param[in]  timeout : Timeout value, in seconds. Must be greater than zero.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::SetAIAFetchTimeout(int timeout)
{
  if(IsFrozen()) return false;
  if(timeout <= 0) return false;

  aiafetchtimeout = timeout;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOSTREAMTLSCONFIG::GetConnectionTimeout()
* @brief      Get connection timeout
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOSTREAMTLSCONFIG::GetConnectionTimeout()
{
  return connectiontimeout;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::SetConnectionTimeout(int timeout)
* @brief      Set connection timeout
* @ingroup    DATAIO
* 
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::SetConnectionTimeout(int timeout)
{
  if(IsFrozen()) return false;
  if((timeout <= 0) && (timeout != XTIMER_INFINITE)) return false;
  connectiontimeout = timeout;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOSTREAMTLSCONFIG::GetHandshakeTimeout()
* @brief      Get handshake timeout
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOSTREAMTLSCONFIG::GetHandshakeTimeout()
{
  return handshaketimeout;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::SetHandshakeTimeout(int timeout)
* @brief      Set handshake timeout
* @ingroup    DATAIO
* 
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::SetHandshakeTimeout(int timeout)
{
  if(IsFrozen()) return false;
  if((timeout <= 0) && (timeout != XTIMER_INFINITE)) return false;
  handshaketimeout = timeout;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHERCERTIFICATEX509VALIDATIONPOLICY* DIOSTREAMTLSCONFIG::GetCertificateValidationPolicy()
* @brief      Get certificate validation policy
* @ingroup    DATAIO
* 
* @return     CIPHERCERTIFICATEX509VALIDATIONPOLICY* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERCERTIFICATEX509VALIDATIONPOLICY* DIOSTREAMTLSCONFIG::GetCertificateValidationPolicy()
{
  return &certificatevalidationpolicy;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTLSCONFIG::SetCertificateValidationPolicy(CIPHERCERTIFICATEX509VALIDATIONPOLICY& policy)
* @brief      Set certificate validation policy
* @ingroup    DATAIO
* 
* @param[in]  policy : Policy value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLSCONFIG::SetCertificateValidationPolicy(CIPHERCERTIFICATEX509VALIDATIONPOLICY& policy)
{
  if(IsFrozen()) return;
  certificatevalidationpolicy = policy;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTLSMEMORYPOLICY* DIOSTREAMTLSCONFIG::GetMemoryPolicy()
* @brief      Get memory policy
* @ingroup    DATAIO
* 
* @return     DIOSTREAMTLSMEMORYPOLICY* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLSMEMORYPOLICY* DIOSTREAMTLSCONFIG::GetMemoryPolicy()
{
  return &memorypolicy;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::SetMemoryPolicy(DIOSTREAMTLSMEMORYPOLICY& policy)
* @brief      Set memory policy
* @ingroup    DATAIO
* 
* @param[in]  policy : Policy value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::SetMemoryPolicy(DIOSTREAMTLSMEMORYPOLICY& policy)
{
  if(IsFrozen()) return false;
  DIOSTREAMTLSMEMORYPOLICY previous = memorypolicy;
  memorypolicy = policy;
  if(!MemoryPolicy_Validate())
    {
      memorypolicy = previous;
      return false;
    }
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<XBUFFER*>* DIOSTREAMTLSCONFIG::GetCertificateRevocationLists()
* @brief      Get certificate revocation lists
* @ingroup    DATAIO
* 
* @return     XVECTOR<XBUFFER*>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<XBUFFER*>* DIOSTREAMTLSCONFIG::GetCertificateRevocationLists()
{
  return &certificaterevocationlists;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::CertificateRevocationList_Add(XBUFFER& CRL)
* @brief      Certificate revocation list add
* @ingroup    DATAIO
* 
* @param[in]  CRL : CRL value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::CertificateRevocationList_Add(XBUFFER& CRL)
{
  if(IsFrozen()) return false;
  if(CRL.IsEmpty() || (CRL.GetSize() > memorypolicy.GetMaximumCRLSize()) ||
     (certificaterevocationlists.GetSize() >= memorypolicy.GetMaximumCRLs())) return false;
  XBUFFER* copy=GEN_NEW XBUFFER();
  if(!copy || !copy->Add(CRL) || !certificaterevocationlists.Add(copy)) { if(copy) GEN_DELETE copy; return false; }
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::CertificateRevocationLists_Delete()
* @brief      Certificate revocation lists delete
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::CertificateRevocationLists_Delete()
{
  if(IsFrozen()) return false;
  certificaterevocationlists.DeleteContents(); certificaterevocationlists.DeleteAll(); return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTLSCONFIG::OCSPDirect_Set(DIOSTREAMTLS_OCSPDIRECTFETCHER fetcher, void* context)
* @brief      Ocsp direct set
* @ingroup    DATAIO
* 
* @param[in]  fetcher : Fetcher value.
* @param[in]  context : Pointer to context.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLSCONFIG::OCSPDirect_Set(DIOSTREAMTLS_OCSPDIRECTFETCHER fetcher, void* context)
{
  if(IsFrozen()) return;
  ocspdirectfetcher=fetcher; ocspdirectcontext=context;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTLS_OCSPDIRECTFETCHER DIOSTREAMTLSCONFIG::GetOCSPDirectFetcher()
* @brief      Get ocsp direct fetcher
* @ingroup    DATAIO
* 
* @return     DIOSTREAMTLS_OCSPDIRECTFETCHER : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_OCSPDIRECTFETCHER DIOSTREAMTLSCONFIG::GetOCSPDirectFetcher()
{
  return ocspdirectfetcher;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void* DIOSTREAMTLSCONFIG::GetOCSPDirectContext()
* @brief      Get ocsp direct context
* @ingroup    DATAIO
* 
* @return     void* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void* DIOSTREAMTLSCONFIG::GetOCSPDirectContext()
{
  return ocspdirectcontext;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XWORD DIOSTREAMTLSCONFIG::GetMinVersion()
* @brief      Get the minimum TLS version configured for this endpoint
* @ingroup    DATAIO
*
* @return     XWORD : DIOSTREAMTLS_MSG_VERSION_TLS_1_2 or DIOSTREAMTLS_MSG_VERSION_TLS_1_3.
*
* --------------------------------------------------------------------------------------------------------------------*/
XWORD DIOSTREAMTLSCONFIG::GetMinVersion()
{
  return minversion;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSCONFIG::SetMinVersion(XWORD version)
* @brief      Set the minimum TLS version configured for this endpoint
* @note       The current server-side DIOSTREAMTLS implementation supports TLS 1.3 only; a server configured
*             with any other version window is rejected by DIOSTREAMTLS::Open() and DIOWEBSERVER::Ini().
* @note       TLS 1.3 cipher suites are configured through GetCipherSuites(); TLS 1.2 suites use the separate
*             GetTLS12CipherSuites() list because the protocol versions assign different semantics to them.
*             Both versions share the configured groups, signature schemes and X.509 validation policy.
* @ingroup    DATAIO
*
* @param[in]  version : DIOSTREAMTLS_MSG_VERSION_TLS_1_2 or DIOSTREAMTLS_MSG_VERSION_TLS_1_3.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::SetMinVersion(XWORD version)
{
  if(IsFrozen()) return false;
  if((version != DIOSTREAMTLS_MSG_VERSION_TLS_1_2) && (version != DIOSTREAMTLS_MSG_VERSION_TLS_1_3)) return false;
  if(version > maxversion) return false;

  minversion = version;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XWORD DIOSTREAMTLSCONFIG::GetMaxVersion()
* @brief      Get the maximum TLS version configured for this endpoint
* @ingroup    DATAIO
*
* @return     XWORD : DIOSTREAMTLS_MSG_VERSION_TLS_1_2 or DIOSTREAMTLS_MSG_VERSION_TLS_1_3.
*
* --------------------------------------------------------------------------------------------------------------------*/
XWORD DIOSTREAMTLSCONFIG::GetMaxVersion()
{
  return maxversion;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSCONFIG::SetMaxVersion(XWORD version)
* @brief      Set the maximum TLS version configured for this endpoint
* @note       The current server-side DIOSTREAMTLS implementation supports TLS 1.3 only.
* @ingroup    DATAIO
*
* @param[in]  version : DIOSTREAMTLS_MSG_VERSION_TLS_1_2 or DIOSTREAMTLS_MSG_VERSION_TLS_1_3.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::SetMaxVersion(XWORD version)
{
  if(IsFrozen()) return false;
  if((version != DIOSTREAMTLS_MSG_VERSION_TLS_1_2) && (version != DIOSTREAMTLS_MSG_VERSION_TLS_1_3)) return false;
  if(version < minversion) return false;

  maxversion = version;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::IsEarlyDataActive()
* @brief      Is early data active
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::IsEarlyDataActive()
{
  return earlydataactive;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTLSCONFIG::EarlyData_Activate(bool active)
* @brief      Early data activate
* @ingroup    DATAIO
* 
* @param[in]  active : Active value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLSCONFIG::EarlyData_Activate(bool active)
{
  if(IsFrozen()) return;
  earlydataactive = active;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOSTREAMTLSCONFIG::GetMaximumEarlyDataSize()
* @brief      Get maximum early data size
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAMTLSCONFIG::GetMaximumEarlyDataSize()
{
  return maximumearlydatasize;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::SetMaximumEarlyDataSize(XDWORD size)
* @brief      Set maximum early data size
* @ingroup    DATAIO
* 
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::SetMaximumEarlyDataSize(XDWORD size)
{
  if(IsFrozen() || size > DIOSTREAMTLS13_EARLYDATA_MAXSIZE) return false;
  maximumearlydatasize = size;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::EarlyDataReplayCheck_Set(DIOSTREAMTLS_EARLYDATA_REPLAYCHECK callback, void* context)
* @brief      Early data replay check set
* @ingroup    DATAIO
* 
* @param[in]  callback : Callback value.
* @param[in]  context : Pointer to context.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::EarlyDataReplayCheck_Set(DIOSTREAMTLS_EARLYDATA_REPLAYCHECK callback, void* context)
{
  if(IsFrozen()) return false;
  earlydatareplaycheck = callback;
  earlydatareplaycontext = context;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::EarlyDataReplayCheck(XBUFFER& ticketidentity)
* @brief      Early data replay check
* @ingroup    DATAIO
* 
* @param[in]  ticketidentity : Ticketidentity value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::EarlyDataReplayCheck(XBUFFER& ticketidentity)
{
  return earlydataactive && earlydatareplaycheck && earlydatareplaycheck(ticketidentity, earlydatareplaycontext);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::IsEarlyDataAcceptable()
* @brief      Is early data acceptable
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::IsEarlyDataAcceptable()
{
  return earlydataactive && maximumearlydatasize && earlydatareplaycheck;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::EarlyDataTicketAge_IsAcceptable(XQWORD issueepoch, XDWORD obfuscatedage, XDWORD ageadd)
* @brief      Early data ticket age is acceptable
* @ingroup    DATAIO
* 
* @param[in]  issueepoch : Issueepoch value.
* @param[in]  obfuscatedage : Obfuscatedage value.
* @param[in]  ageadd : Ageadd value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::EarlyDataTicketAge_IsAcceptable(XQWORD issueepoch, XDWORD obfuscatedage, XDWORD ageadd)
{
  XQWORD now=DIOSTREAMTLSCONFIG_CurrentEpoch();
  if(!now || !issueepoch || now<issueepoch) return false;
  XQWORD serverage64=(now-issueepoch)*1000ULL;
  if(serverage64>0xFFFFFFFFULL) return false;
  XDWORD serverage=(XDWORD)serverage64;
  XDWORD clientage=(XDWORD)(obfuscatedage-ageadd);
  XDWORD diff=(clientage>serverage)?(clientage-serverage):(serverage-clientage);
  return diff<=DIOSTREAMTLS13_EARLYDATA_AGE_TOLERANCE_MS;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::IsSessionResumptionActive()
* @brief      Is session resumption active
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::IsSessionResumptionActive()
{
  DIOSTREAMTLSCONFIG_LOCK lock(configmutex);
  if(!lock.IsLocked()) return false;
  return sessionresumptionactive;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTLSCONFIG::SessionResumption_Activate(bool active)
* @brief      Session resumption activate
* @ingroup    DATAIO
* 
* @param[in]  active : Active value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLSCONFIG::SessionResumption_Activate(bool active)
{
  if(IsFrozen()) return;
  DIOSTREAMTLSCONFIG_LOCK lock(configmutex);
  if(!lock.IsLocked()) return;
  sessionresumptionactive = active;

  if(!active)
    {
      sessiontickets.DeleteContents();
      sessiontickets.DeleteAll();
      sessionticketserverkeycurrent.SecureDelete();
      sessionticketserverkeyprevious.SecureDelete();
      sessionticketserverkeycurrentID = sessionticketserverkeypreviousID = 0;
      sessionticketserverkeycurrentcreated = sessionticketserverkeypreviousexpires = 0;
      sessionticketkeyringdirty = false;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOSTREAMTLSCONFIG::GetSessionTicketLifetime()
* @brief      Get session ticket lifetime
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAMTLSCONFIG::GetSessionTicketLifetime()
{
  DIOSTREAMTLSCONFIG_LOCK lock(configmutex);
  if(!lock.IsLocked()) return 0;
  return sessionticketlifetime;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::SetSessionTicketLifetime(XDWORD lifetime)
* @brief      Set session ticket lifetime
* @ingroup    DATAIO
* 
* @param[in]  lifetime : Lifetime value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::SetSessionTicketLifetime(XDWORD lifetime)
{
  if(IsFrozen()) return false;
  if(!lifetime || (lifetime > DIOSTREAMTLS13_SESSIONTICKET_MAX_LIFETIME)) return false;
  DIOSTREAMTLSCONFIG_LOCK lock(configmutex);
  if(!lock.IsLocked()) return false;
  sessionticketlifetime = lifetime;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOSTREAMTLSCONFIG::GetSessionTicketKeyRotationInterval()
* @brief      Get session ticket key rotation interval
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAMTLSCONFIG::GetSessionTicketKeyRotationInterval()
{
  DIOSTREAMTLSCONFIG_LOCK lock(configmutex);
  if(!lock.IsLocked()) return 0;
  return sessionticketkeyrotationinterval;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::SetSessionTicketKeyRotationInterval(XDWORD interval)
* @brief      Set session ticket key rotation interval
* @ingroup    DATAIO
* 
* @param[in]  interval : Interval value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::SetSessionTicketKeyRotationInterval(XDWORD interval)
{
  if(IsFrozen()) return false;
  if(!interval || interval > DIOSTREAMTLS13_SESSIONTICKET_MAX_LIFETIME) return false;
  DIOSTREAMTLSCONFIG_LOCK lock(configmutex);
  if(!lock.IsLocked()) return false;
  sessionticketkeyrotationinterval = interval;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::SessionTicketKeyRing_Rotate()
* @brief      Session ticket key ring rotate
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::SessionTicketKeyRing_Rotate()
{
  XRAND* random = GEN_XFACTORY.CreateRand();
  if(!random || !random->IsCryptographicallySecure())
    {
      if(random) GEN_XFACTORY.DeleteRand(random);
      return false;
    }

  XBYTE key[32] = { 0 };
  XQWORD keyID = 0;
  bool status = random->Ini();
  if(status) status = random->Generate(key, sizeof(key));
  XQWORD currentID = 0;
  XQWORD previousID = 0;
  {
    DIOSTREAMTLSCONFIG_LOCK lock(configmutex);
    if(!lock.IsLocked() || !sessionresumptionactive) status = false;
    if(status)
      {
        currentID  = sessionticketserverkeycurrentID;
        previousID = sessionticketserverkeypreviousID;
      }
  }
  for(XDWORD attempt=0; status && attempt<16; attempt++)
    {
      status = random->Generate((XBYTE*)&keyID, sizeof(keyID));
      if(status && keyID && keyID != currentID && keyID != previousID) break;
      keyID = 0;
    }
  GEN_XFACTORY.DeleteRand(random);
  if(!keyID) status = false;

  if(status)
    {
      DIOSTREAMTLSCONFIG_LOCK lock(configmutex);
      XQWORD now = DIOSTREAMTLSCONFIG_CurrentEpoch();
      if(!lock.IsLocked() || !now || keyID == sessionticketserverkeycurrentID ||
         keyID == sessionticketserverkeypreviousID) status = false;
      if(status)
        {
          XSECUREBUFFER oldcurrent;
          XSECUREBUFFER oldprevious;
          XQWORD rotatedpreviousID = sessionticketserverkeycurrentID;
          if(sessionticketserverkeycurrent.GetSize() == 32 && !oldcurrent.Add(sessionticketserverkeycurrent)) status = false;
          if(status && sessionticketserverkeyprevious.GetSize() == 32 &&
             !oldprevious.Add(sessionticketserverkeyprevious)) status = false;
          if(status)
            {
              sessionticketserverkeyprevious.SecureDelete();
              if(!oldcurrent.IsEmpty() && !sessionticketserverkeyprevious.Add(oldcurrent)) status = false;
            }
          if(status)
            {
              sessionticketserverkeycurrent.SecureDelete();
              if(!sessionticketserverkeycurrent.Add(key, sizeof(key)))
                {
                  if(!oldcurrent.IsEmpty()) sessionticketserverkeycurrent.Add(oldcurrent);
                  status = false;
                }
            }
          if(status)
            {
              sessionticketserverkeypreviousID      = oldcurrent.IsEmpty()?0:rotatedpreviousID;
              sessionticketserverkeypreviousexpires = oldcurrent.IsEmpty()?0:(now + sessionticketlifetime);
              sessionticketserverkeycurrentID       = keyID;
              if(sessionticketserverkeygeneration == ~(XQWORD)0) status = false;
              else sessionticketserverkeygeneration++;
              sessionticketserverkeycurrentcreated  = now;
              sessionticketkeyringdirty             = sessionticketkeyringsave != NULL;
            }
          else
            {
              sessionticketserverkeycurrent.SecureDelete();
              sessionticketserverkeyprevious.SecureDelete();
              if(!oldcurrent.IsEmpty()) sessionticketserverkeycurrent.Add(oldcurrent);
              if(!oldprevious.IsEmpty()) sessionticketserverkeyprevious.Add(oldprevious);
            }
        }
    }

  DIOSTREAMTLSCONFIG_SecureZero(key, sizeof(key));
  if(status)
    {
      DIOSTREAMTLS_SESSIONTICKETKEYRING_SAVE save = NULL;
      {
        DIOSTREAMTLSCONFIG_LOCK lock(configmutex);
        if(lock.IsLocked()) save = sessionticketkeyringsave;
      }
      if(save) status = SessionTicketKeyRing_Synchronize(true);
    }
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::SessionTicketKeyRing_Export(XBUFFER& wrappingkey, XBUFFER& encryptedkeyring)
* @brief      Session ticket key ring export
* @ingroup    DATAIO
* 
* @param[in]  wrappingkey : Wrappingkey value.
* @param[in]  encryptedkeyring : Encryptedkeyring value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::SessionTicketKeyRing_Export(XBUFFER& wrappingkey, XBUFFER& encryptedkeyring)
{
  static XBYTE header[] = { 'G','E','N','-','T','L','S','1','3','-','K','R','-','2' };
  if(wrappingkey.GetSize() != 32) return false;

  XSECUREBUFFER plain;
  XSECUREBUFFER currentkey;
  XSECUREBUFFER previouskey;
  XBUFFER nonce;
  XBUFFER aad;
  XBUFFER tag;
  XQWORD currentID = 0, currentcreated = 0, previousID = 0, previousexpires = 0;
  XDWORD rotationinterval = 0, ticketlifetime = 0;
  {
    DIOSTREAMTLSCONFIG_LOCK lock(configmutex);
    if(!lock.IsLocked() || sessionticketserverkeycurrent.GetSize() != 32 || !sessionticketserverkeycurrentID ||
       !sessionticketserverkeycurrentcreated || !currentkey.Add(sessionticketserverkeycurrent)) return false;
    currentID        = sessionticketserverkeycurrentID;
    currentcreated   = sessionticketserverkeycurrentcreated;
    previousID       = sessionticketserverkeypreviousID;
    previousexpires  = sessionticketserverkeypreviousexpires;
    rotationinterval = sessionticketkeyrotationinterval;
    ticketlifetime   = sessionticketlifetime;
    if(sessionticketserverkeyprevious.GetSize() == 32 && !previouskey.Add(sessionticketserverkeyprevious)) return false;
  }

  XBYTE zeros[32] = { 0 };
  if(!DIOSTREAMTLSCONFIG_AddUInt64BE(plain, sessionticketserverkeygeneration) ||
     !DIOSTREAMTLSCONFIG_AddUInt64BE(plain, currentID) ||
     !DIOSTREAMTLSCONFIG_AddUInt64BE(plain, currentcreated) || !plain.Add(currentkey) ||
     !plain.Add((XBYTE)(previouskey.GetSize() == 32)) ||
     !DIOSTREAMTLSCONFIG_AddUInt64BE(plain, previousID) ||
     !DIOSTREAMTLSCONFIG_AddUInt64BE(plain, previousexpires) ||
     !(previouskey.GetSize() == 32?plain.Add(previouskey):plain.Add(zeros, sizeof(zeros))) ||
     !DIOSTREAMTLSCONFIG_AddUInt32BE(plain, rotationinterval) ||
     !DIOSTREAMTLSCONFIG_AddUInt32BE(plain, ticketlifetime)) return false;

  XRAND* random = GEN_XFACTORY.CreateRand();
  if(!random || !random->IsCryptographicallySecure())
    {
      if(random) GEN_XFACTORY.DeleteRand(random);
      return false;
    }
  bool status = nonce.Resize(CIPHERAESGCM_NONCESIZE) && random->Ini() && random->Generate(nonce.Get(), nonce.GetSize());
  GEN_XFACTORY.DeleteRand(random);
  if(!status) return false;

  CIPHERKEYSYMMETRICAL key;
  CIPHERAESGCM cipher;
  if(!aad.Add(header, sizeof(header)) || !key.Set(wrappingkey) || !cipher.SetKey(&key) ||
     !cipher.CipherAEAD(plain.Get(), plain.GetSize(), nonce, aad, tag) || !cipher.GetResult()) return false;

  encryptedkeyring.Delete();
  return encryptedkeyring.Add(header, sizeof(header)) && encryptedkeyring.Add(nonce) &&
         encryptedkeyring.Add((*cipher.GetResult())) && encryptedkeyring.Add(tag);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::SessionTicketKeyRing_Import(XBUFFER& wrappingkey, XBUFFER& encryptedkeyring)
* @brief      Session ticket key ring import
* @ingroup    DATAIO
* 
* @param[in]  wrappingkey : Wrappingkey value.
* @param[in]  encryptedkeyring : Encryptedkeyring value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::SessionTicketKeyRing_Import(XBUFFER& wrappingkey, XBUFFER& encryptedkeyring)
{
  static XBYTE header[] = { 'G','E','N','-','T','L','S','1','3','-','K','R','-','2' };
  const XDWORD plainsize = 113;
  const XDWORD expectedsize = sizeof(header) + CIPHERAESGCM_NONCESIZE + plainsize + CIPHERAESGCM_TAGSIZE;
  if(wrappingkey.GetSize() != 32 || encryptedkeyring.GetSize() != expectedsize ||
     memcmp(encryptedkeyring.Get(), header, sizeof(header))) return false;

  XBUFFER nonce;
  XBUFFER ciphertext;
  XBUFFER tag;
  XBUFFER aad;
  if(!nonce.Add(encryptedkeyring.Get()+sizeof(header), CIPHERAESGCM_NONCESIZE) ||
     !ciphertext.Add(encryptedkeyring.Get()+sizeof(header)+CIPHERAESGCM_NONCESIZE, plainsize) ||
     !tag.Add(encryptedkeyring.Get()+sizeof(header)+CIPHERAESGCM_NONCESIZE+plainsize, CIPHERAESGCM_TAGSIZE) ||
     !aad.Add(header, sizeof(header))) return false;

  CIPHERKEYSYMMETRICAL key;
  CIPHERAESGCM cipher;
  if(!key.Set(wrappingkey) || !cipher.SetKey(&key) ||
     !cipher.UncipherAEAD(ciphertext.Get(), ciphertext.GetSize(), nonce, aad, tag) || !cipher.GetResult()) return false;

  XSECUREBUFFER plain;
  XSECUREBUFFER currentkey;
  XSECUREBUFFER previouskey;
  if(!plain.Add((*cipher.GetResult())) || plain.GetSize() != plainsize) return false;

  XDWORD position = 0;
  XQWORD generation = 0, currentID = 0, currentcreated = 0, previousID = 0, previousexpires = 0;
  XDWORD rotationinterval = 0, ticketlifetime = 0;
  if(!DIOSTREAMTLSCONFIG_GetUInt64BE(plain, position, generation) ||
     !DIOSTREAMTLSCONFIG_GetUInt64BE(plain, position, currentID) ||
     !DIOSTREAMTLSCONFIG_GetUInt64BE(plain, position, currentcreated) ||
     !currentkey.Add(plain.Get()+position, 32)) return false;
  position += 32;
  if(position >= plain.GetSize()) return false;
  XBYTE hasprevious = plain.Get()[position++];
  if(hasprevious > 1 || !DIOSTREAMTLSCONFIG_GetUInt64BE(plain, position, previousID) ||
     !DIOSTREAMTLSCONFIG_GetUInt64BE(plain, position, previousexpires) ||
     !previouskey.Add(plain.Get()+position, 32)) return false;
  position += 32;
  if(!DIOSTREAMTLSCONFIG_GetUInt32BE(plain, position, rotationinterval) ||
     !DIOSTREAMTLSCONFIG_GetUInt32BE(plain, position, ticketlifetime) || position != plain.GetSize() ||
     !currentID || !currentcreated || !rotationinterval ||
     rotationinterval > DIOSTREAMTLS13_SESSIONTICKET_MAX_LIFETIME || !ticketlifetime ||
     ticketlifetime > DIOSTREAMTLS13_SESSIONTICKET_MAX_LIFETIME) return false;

  if(hasprevious)
    {
      if(!previousID || previousID == currentID || !previousexpires) return false;
    }
  else
    {
      if(previousID || previousexpires) return false;
      for(XDWORD c=0; c<previouskey.GetSize(); c++) if(previouskey.Get()[c]) return false;
      previouskey.SecureDelete();
    }

  DIOSTREAMTLSCONFIG_LOCK lock(configmutex);
  if(!lock.IsLocked() || !sessionresumptionactive || !generation || generation < sessionticketserverkeygeneration) return false;
  sessionticketserverkeycurrent.SecureDelete();
  sessionticketserverkeyprevious.SecureDelete();
  if(!sessionticketserverkeycurrent.Add(currentkey) || (hasprevious && !sessionticketserverkeyprevious.Add(previouskey)))
    {
      sessionticketserverkeycurrent.SecureDelete();
      sessionticketserverkeyprevious.SecureDelete();
      sessionticketserverkeycurrentID = sessionticketserverkeypreviousID = 0;
      sessionticketserverkeycurrentcreated = sessionticketserverkeypreviousexpires = 0;
      return false;
    }
  sessionticketserverkeycurrentID       = currentID;
  sessionticketserverkeygeneration     = generation;
  sessionticketserverkeycurrentcreated  = currentcreated;
  sessionticketserverkeypreviousID      = hasprevious?previousID:0;
  sessionticketserverkeypreviousexpires = hasprevious?previousexpires:0;
  sessionticketkeyrotationinterval      = rotationinterval;
  sessionticketlifetime                 = ticketlifetime;
  sessionticketkeyringdirty             = false;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::SessionTicketKeyRingSynchronizer_Set(DIOSTREAMTLS_SESSIONTICKETKEYRING_LOAD load, DIOSTREAMTLS_SESSIONTICKETKEYRING_SAVE save, XBUFFER& wrappingkey, void* context)
* @brief      Session ticket key ring synchronizer set
* @ingroup    DATAIO
* 
* @param[in]  load : Load value.
* @param[in]  save : Save value.
* @param[in]  wrappingkey : Wrappingkey value.
* @param[in]  context : Pointer to context.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::SessionTicketKeyRingSynchronizer_Set(DIOSTREAMTLS_SESSIONTICKETKEYRING_LOAD load, DIOSTREAMTLS_SESSIONTICKETKEYRING_SAVE save, XBUFFER& wrappingkey, void* context)
{
  if(IsFrozen() || (!load && !save) || wrappingkey.GetSize() != 32) return false;
  DIOSTREAMTLSCONFIG_LOCK lock(configmutex);
  if(!lock.IsLocked()) return false;
  sessionticketkeyringwrappingkey.SecureDelete();
  if(!sessionticketkeyringwrappingkey.Add(wrappingkey)) return false;
  sessionticketkeyringload    = load;
  sessionticketkeyringsave    = save;
  sessionticketkeyringcontext = context;
  sessionticketkeyringdirty   = save && sessionticketserverkeycurrent.GetSize() == 32;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::SessionTicketKeyRing_Synchronize(bool publish)
* @brief      Session ticket key ring synchronize
* @ingroup    DATAIO
* 
* @param[in]  publish : Publish value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::SessionTicketKeyRing_Synchronize(bool publish)
{
  DIOSTREAMTLS_SESSIONTICKETKEYRING_LOAD load = NULL;
  DIOSTREAMTLS_SESSIONTICKETKEYRING_SAVE save = NULL;
  void* context = NULL;
  XSECUREBUFFER wrappingkey;
  {
    DIOSTREAMTLSCONFIG_LOCK lock(configmutex);
    if(!lock.IsLocked() || sessionticketkeyringwrappingkey.GetSize() != 32 ||
       !wrappingkey.Add(sessionticketkeyringwrappingkey)) return false;
    load    = sessionticketkeyringload;
    save    = sessionticketkeyringsave;
    context = sessionticketkeyringcontext;
  }

  XBUFFER encryptedkeyring;
  if(publish)
    {
      XQWORD exportedID = 0;
      {
        DIOSTREAMTLSCONFIG_LOCK lock(configmutex);
        if(!lock.IsLocked()) return false;
        exportedID = sessionticketserverkeycurrentID;
      }
      bool status = save && SessionTicketKeyRing_Export(wrappingkey, encryptedkeyring) && save(encryptedkeyring, context);
      if(status)
        {
          DIOSTREAMTLSCONFIG_LOCK lock(configmutex);
          if(lock.IsLocked() && sessionticketserverkeycurrentID == exportedID) sessionticketkeyringdirty = false;
        }
      return status;
    }
  return load && load(encryptedkeyring, context) && SessionTicketKeyRing_Import(wrappingkey, encryptedkeyring);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::SessionResumption_ServerInitialize()
* @brief      Session resumption server initialize
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::SessionResumption_ServerInitialize()
{
  bool loadavailable = false;
  bool publishpending = false;
  {
    DIOSTREAMTLSCONFIG_LOCK lock(configmutex);
    if(!lock.IsLocked()) return false;
    if(!sessionresumptionactive) return true;
    XQWORD now = DIOSTREAMTLSCONFIG_CurrentEpoch();
    if(!now) return false;
    if((sessionticketserverkeycurrent.GetSize() == 32) && now >= sessionticketserverkeycurrentcreated &&
       ((now-sessionticketserverkeycurrentcreated) < sessionticketkeyrotationinterval))
      {
        if(!sessionticketkeyringdirty) return true;
        publishpending = sessionticketkeyringsave && sessionticketkeyringwrappingkey.GetSize() == 32;
      }
    loadavailable = sessionticketkeyringload && sessionticketkeyringwrappingkey.GetSize() == 32;
  }

  if(publishpending) return SessionTicketKeyRing_Synchronize(true);

  if(loadavailable)
    {
      if(!SessionTicketKeyRing_Synchronize(false)) return false;
      DIOSTREAMTLSCONFIG_LOCK lock(configmutex);
      XQWORD now = DIOSTREAMTLSCONFIG_CurrentEpoch();
      if(!lock.IsLocked() || !now) return false;
      if((sessionticketserverkeycurrent.GetSize() == 32) && now >= sessionticketserverkeycurrentcreated &&
         ((now-sessionticketserverkeycurrentcreated) < sessionticketkeyrotationinterval)) return true;
    }

  return SessionTicketKeyRing_Rotate();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::SessionTicket_Store(XCHAR* servername, XBUFFER& ticket, XBUFFER& PSK, XDWORD ageadd, XDWORD lifetime, XWORD ciphersuite, DIOSTREAMTLS_ALPN_TYPE applicationprotocol)
* @brief      Session ticket store
* @ingroup    DATAIO
* 
* @param[in]  servername : Pointer to servername.
* @param[in]  ticket : Ticket value.
* @param[in]  PSK : PSK value.
* @param[in]  ageadd : Ageadd value.
* @param[in]  lifetime : Lifetime value.
* @param[in]  ciphersuite : Ciphersuite value.
* @param[in]  applicationprotocol : Applicationprotocol value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::SessionTicket_Store(XCHAR* servername, XBUFFER& ticket, XBUFFER& PSK, XDWORD ageadd, XDWORD lifetime, XWORD ciphersuite, DIOSTREAMTLS_ALPN_TYPE applicationprotocol)
{
  XBUFFER protocol;
  if(!DIOSTREAMTLSCONFIG_ALPNFromType(applicationprotocol, protocol)) return false;
  return SessionTicket_StoreRaw(servername, ticket, PSK, ageadd, lifetime, ciphersuite, protocol.IsEmpty()?NULL:&protocol, 0);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::SessionTicket_StoreRaw(XCHAR* servername, XBUFFER& ticket, XBUFFER& PSK, XDWORD ageadd, XDWORD lifetime, XWORD ciphersuite, XBUFFER* applicationprotocol, XDWORD maximumearlydatasize)
* @brief      Session ticket store raw
* @ingroup    DATAIO
* 
* @param[in]  servername : Pointer to servername.
* @param[in]  ticket : Ticket value.
* @param[in]  PSK : PSK value.
* @param[in]  ageadd : Ageadd value.
* @param[in]  lifetime : Lifetime value.
* @param[in]  ciphersuite : Ciphersuite value.
* @param[in]  applicationprotocol : Pointer to applicationprotocol.
* @param[in]  maximumearlydatasize : Maximumearlydatasize value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::SessionTicket_StoreRaw(XCHAR* servername, XBUFFER& ticket, XBUFFER& PSK, XDWORD ageadd, XDWORD lifetime, XWORD ciphersuite, XBUFFER* applicationprotocol, XDWORD maximumearlydatasize)
{
  DIOSTREAMTLSCONFIG_LOCK lock(configmutex);
  if(!lock.IsLocked()) return false;
  if(!sessionresumptionactive || !servername || !servername[0] || ticket.IsEmpty() || PSK.IsEmpty() ||
     !lifetime || (lifetime > DIOSTREAMTLS13_SESSIONTICKET_MAX_LIFETIME)) return false;

  for(int c=(int)sessiontickets.GetSize()-1; c>=0; c--)
    {
      DIOSTREAMTLS13SESSIONTICKET* current = sessiontickets.Get(c);
      if(!current || current->IsExpired() || !current->GetServerName()->Compare(servername, true))
        {
          sessiontickets.Delete(current);
          if(current) GEN_DELETE current;
        }
    }

  while(sessiontickets.GetSize() >= memorypolicy.GetMaximumSessionTickets())
    {
      DIOSTREAMTLS13SESSIONTICKET* oldest = sessiontickets.Get(0);
      sessiontickets.Delete(oldest);
      if(oldest) GEN_DELETE oldest;
    }

  DIOSTREAMTLS13SESSIONTICKET* stored = GEN_NEW DIOSTREAMTLS13SESSIONTICKET();
  if(!stored) return false;

  stored->GetServerName()->Set(servername);
  stored->SetTicketAgeAdd(ageadd);
  stored->SetLifetime(lifetime);
  stored->SetReceivedEpoch(DIOSTREAMTLSCONFIG_CurrentEpoch());
  stored->SetCipherSuite(ciphersuite);
  stored->SetApplicationProtocol(DIOSTREAMTLS_ALPN_TYPE_UNKNOWN);
  stored->SetMaximumEarlyDataSize(maximumearlydatasize);
  if(applicationprotocol && (!stored->GetApplicationProtocolRaw()->Add((*applicationprotocol)) || applicationprotocol->GetSize() > 255))
    { GEN_DELETE stored; return false; }

  if(!stored->GetTicket()->Add(ticket) || !stored->GetPSK()->Add(PSK) || !stored->GetReceivedEpoch() ||
     !sessiontickets.Add(stored))
    {
      GEN_DELETE stored;
      return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::SessionTicket_Copy(XCHAR* servername, DIOSTREAMTLS13SESSIONTICKET& destination)
* @brief      Session ticket copy
* @ingroup    DATAIO
* 
* @param[in]  servername : Pointer to servername.
* @param[in]  destination : Destination value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::SessionTicket_Copy(XCHAR* servername, DIOSTREAMTLS13SESSIONTICKET& destination)
{
  DIOSTREAMTLSCONFIG_LOCK lock(configmutex);
  if(!lock.IsLocked() || !sessionresumptionactive || !servername || !servername[0]) return false;

  for(int c=(int)sessiontickets.GetSize()-1; c>=0; c--)
    {
      DIOSTREAMTLS13SESSIONTICKET* ticket = sessiontickets.Get(c);
      if(!ticket) continue;
      if(ticket->IsExpired())
        {
          sessiontickets.Delete(ticket);
          GEN_DELETE ticket;
          continue;
        }
      if(!ticket->GetServerName()->Compare(servername, true))
        {
          destination.Delete();
          destination.GetServerName()->Set(ticket->GetServerName()->Get());
          destination.SetTicketAgeAdd(ticket->GetTicketAgeAdd());
          destination.SetLifetime(ticket->GetLifetime());
          destination.SetReceivedEpoch(ticket->GetReceivedEpoch());
          destination.SetCipherSuite(ticket->GetCipherSuite());
          destination.SetApplicationProtocol(ticket->GetApplicationProtocol());
          destination.SetMaximumEarlyDataSize(ticket->GetMaximumEarlyDataSize());
          return destination.GetTicket()->Add((*ticket->GetTicket())) && destination.GetPSK()->Add((*ticket->GetPSK())) &&
                 (ticket->GetApplicationProtocolRaw()->IsEmpty() || destination.GetApplicationProtocolRaw()->Add((*ticket->GetApplicationProtocolRaw())));
        }
    }
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::SessionTickets_Delete()
* @brief      Session tickets delete
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::SessionTickets_Delete()
{
  DIOSTREAMTLSCONFIG_LOCK lock(configmutex);
  if(!lock.IsLocked()) return false;
  sessiontickets.DeleteContents();
  sessiontickets.DeleteAll();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::SessionTicket_Seal(XBUFFER& PSK, XWORD ciphersuite, DIOSTREAMTLS_ALPN_TYPE applicationprotocol, XCHAR* servername, XDWORD lifetime, XDWORD ageadd, XBUFFER& ticket, XDWORD maximumearlydatasize)
* @brief      Session ticket seal
* @ingroup    DATAIO
* 
* @param[in]  PSK : PSK value.
* @param[in]  ciphersuite : Ciphersuite value.
* @param[in]  applicationprotocol : Applicationprotocol value.
* @param[in]  servername : Pointer to servername.
* @param[in]  lifetime : Lifetime value.
* @param[in]  ageadd : Ageadd value.
* @param[in]  ticket : Ticket value.
* @param[in]  maximumearlydatasize : Maximumearlydatasize value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::SessionTicket_Seal(XBUFFER& PSK, XWORD ciphersuite, DIOSTREAMTLS_ALPN_TYPE applicationprotocol, XCHAR* servername, XDWORD lifetime, XDWORD ageadd, XBUFFER& ticket, XDWORD maximumearlydatasize)
{
  XBUFFER protocol;
  if(!DIOSTREAMTLSCONFIG_ALPNFromType(applicationprotocol, protocol)) return false;
  return SessionTicket_SealRaw(PSK, ciphersuite, protocol.IsEmpty()?NULL:&protocol, servername, lifetime, ageadd, ticket, maximumearlydatasize);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::SessionTicket_SealRaw(XBUFFER& PSK, XWORD ciphersuite, XBUFFER* applicationprotocol, XCHAR* servername, XDWORD lifetime, XDWORD ageadd, XBUFFER& ticket, XDWORD maximumearlydatasize)
* @brief      Session ticket seal raw
* @ingroup    DATAIO
* 
* @param[in]  PSK : PSK value.
* @param[in]  ciphersuite : Ciphersuite value.
* @param[in]  applicationprotocol : Pointer to applicationprotocol.
* @param[in]  servername : Pointer to servername.
* @param[in]  lifetime : Lifetime value.
* @param[in]  ageadd : Ageadd value.
* @param[in]  ticket : Ticket value.
* @param[in]  maximumearlydatasize : Maximumearlydatasize value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::SessionTicket_SealRaw(XBUFFER& PSK, XWORD ciphersuite, XBUFFER* applicationprotocol, XCHAR* servername, XDWORD lifetime, XDWORD ageadd, XBUFFER& ticket, XDWORD maximumearlydatasize)
{
  if(!sessionresumptionactive || PSK.IsEmpty() || !lifetime ||
     (lifetime > DIOSTREAMTLS13_SESSIONTICKET_MAX_LIFETIME) || maximumearlydatasize > DIOSTREAMTLS13_EARLYDATA_MAXSIZE ||
     !SessionResumption_ServerInitialize()) return false;

  XSECUREBUFFER plain;
  XBUFFER nonce;
  XBUFFER aad;
  XBUFFER tag;
  XSECUREBUFFER sealingkey;
  XQWORD keyID = 0;
  CIPHERKEYSYMMETRICAL key;
  CIPHERAESGCM cipher;
  XRAND* random = GEN_XFACTORY.CreateRand();
  XQWORD issueepoch = DIOSTREAMTLSCONFIG_CurrentEpoch();
  XBYTE namelength = 0;

  {
    DIOSTREAMTLSCONFIG_LOCK lock(configmutex);
    if(!lock.IsLocked() || sessionticketserverkeycurrent.GetSize() != 32) return false;
    keyID = sessionticketserverkeycurrentID;
    if(!sealingkey.Add(sessionticketserverkeycurrent)) return false;
  }

  if(!issueepoch || PSK.GetSize() > 255 || (applicationprotocol && applicationprotocol->GetSize() > 255)) return false;
  if(servername)
    {
      XDWORD length = 0;
      while(servername[length])
        {
          if((XDWORD)servername[length] > 0x7F || length >= 255) return false;
          length++;
        }
      namelength = (XBYTE)length;
    }

  if(!random || !random->IsCryptographicallySecure())
    {
      if(random) GEN_XFACTORY.DeleteRand(random);
      return false;
    }
  if(!nonce.Resize(CIPHERAESGCM_NONCESIZE)) { GEN_XFACTORY.DeleteRand(random); return false; }
  bool status = random->Ini() && random->Generate(nonce.Get(), nonce.GetSize());
  GEN_XFACTORY.DeleteRand(random);
  if(!status) return false;

  XBYTE protocolsize = applicationprotocol?(XBYTE)applicationprotocol->GetSize():0;
  if(!plain.Add((XBYTE)4) || !DIOSTREAMTLSCONFIG_AddUInt64BE(plain, issueepoch) ||
     !DIOSTREAMTLSCONFIG_AddUInt32BE(plain, lifetime) || !DIOSTREAMTLSCONFIG_AddUInt32BE(plain, ageadd) ||
     !DIOSTREAMTLSCONFIG_AddUInt16BE(plain, ciphersuite) || !DIOSTREAMTLSCONFIG_AddUInt32BE(plain, maximumearlydatasize) || !plain.Add(protocolsize) ||
     (protocolsize && !plain.Add((*applicationprotocol))) || !plain.Add((XBYTE)PSK.GetSize()) ||
     !plain.Add(PSK) || !plain.Add(namelength)) return false;

  for(XDWORD c=0; c<namelength; c++) if(!plain.Add((XBYTE)servername[c])) return false;

  static XBYTE aadbytes[] = { 'G','E','N','-','T','L','S','1','3','-','T','I','C','K','E','T','-','4' };
  if(!aad.Add(aadbytes, sizeof(aadbytes)) || !DIOSTREAMTLSCONFIG_AddUInt64BE(aad, keyID) ||
     !key.Set(sealingkey) || !cipher.SetKey(&key) ||
     !cipher.CipherAEAD(plain.Get(), plain.GetSize(), nonce, aad, tag) || !cipher.GetResult()) return false;

  ticket.Delete();
  if(!DIOSTREAMTLSCONFIG_AddUInt64BE(ticket, keyID) || !ticket.Add(nonce) ||
     !ticket.Add((*cipher.GetResult())) || !ticket.Add(tag)) return false;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::SessionTicket_Open(XBUFFER& ticket, XBUFFER& PSK, XWORD& ciphersuite, DIOSTREAMTLS_ALPN_TYPE& applicationprotocol, XSTRING& servername, XQWORD& issueepoch, XDWORD& lifetime, XDWORD& ageadd)
* @brief      Session ticket open
* @ingroup    DATAIO
* 
* @param[in]  ticket : Ticket value.
* @param[in]  PSK : PSK value.
* @param[in]  ciphersuite : Ciphersuite value.
* @param[in]  applicationprotocol : Applicationprotocol value.
* @param[in]  servername : Servername value.
* @param[in]  issueepoch : Issueepoch value.
* @param[in]  lifetime : Lifetime value.
* @param[in]  ageadd : Ageadd value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::SessionTicket_Open(XBUFFER& ticket, XBUFFER& PSK, XWORD& ciphersuite, DIOSTREAMTLS_ALPN_TYPE& applicationprotocol, XSTRING& servername, XQWORD& issueepoch, XDWORD& lifetime, XDWORD& ageadd)
{
  XBUFFER protocol;
  XDWORD maximumearlydatasize = 0;
  if(!SessionTicket_OpenRaw(ticket, PSK, ciphersuite, protocol, servername, issueepoch, lifetime, ageadd, maximumearlydatasize)) return false;
  applicationprotocol = DIOSTREAMTLS_ALPN_TYPE_UNKNOWN;
  if(protocol.GetSize() == 8 && !memcmp(protocol.Get(), "http/1.1", 8)) applicationprotocol = DIOSTREAMTLS_ALPN_TYPE_HTTP_1_1;
  if(protocol.GetSize() == 2 && !memcmp(protocol.Get(), "h2", 2)) applicationprotocol = DIOSTREAMTLS_ALPN_TYPE_HTTP_2;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::SessionTicket_OpenRaw(XBUFFER& ticket, XBUFFER& PSK, XWORD& ciphersuite, XBUFFER& applicationprotocol, XSTRING& servername, XQWORD& issueepoch, XDWORD& lifetime, XDWORD& ageadd, XDWORD& maximumearlydatasize)
* @brief      Session ticket open raw
* @ingroup    DATAIO
* 
* @param[in]  ticket : Ticket value.
* @param[in]  PSK : PSK value.
* @param[in]  ciphersuite : Ciphersuite value.
* @param[in]  applicationprotocol : Applicationprotocol value.
* @param[in]  servername : Servername value.
* @param[in]  issueepoch : Issueepoch value.
* @param[in]  lifetime : Lifetime value.
* @param[in]  ageadd : Ageadd value.
* @param[in]  maximumearlydatasize : Maximumearlydatasize value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::SessionTicket_OpenRaw(XBUFFER& ticket, XBUFFER& PSK, XWORD& ciphersuite, XBUFFER& applicationprotocol, XSTRING& servername, XQWORD& issueepoch, XDWORD& lifetime, XDWORD& ageadd, XDWORD& maximumearlydatasize)
{
  // Enforce the ticket-key cryptoperiod on the resumption path too.  Previously rotation was guaranteed when a new
  // ticket was sealed, but a server receiving only resumptions could keep opening tickets with the current key after
  // its configured rotation interval.  ServerInitialize rotates current -> previous while retaining the previous key
  // long enough for already-issued, individually unexpired tickets to remain usable.
  if(!sessionresumptionactive || !SessionResumption_ServerInitialize()) return false;

  if(ticket.GetSize() <= (sizeof(XQWORD) + CIPHERAESGCM_NONCESIZE + CIPHERAESGCM_TAGSIZE)) return false;

  XBUFFER nonce;
  XBUFFER ciphertext;
  XBUFFER tag;
  XBUFFER aad;
  CIPHERKEYSYMMETRICAL key;
  CIPHERAESGCM cipher;
  XSECUREBUFFER openingkey;
  XQWORD keyID = 0;
  XDWORD ticketposition = 0;
  if(!DIOSTREAMTLSCONFIG_GetUInt64BE(ticket, ticketposition, keyID)) return false;
  {
    DIOSTREAMTLSCONFIG_LOCK lock(configmutex);
    XQWORD now = DIOSTREAMTLSCONFIG_CurrentEpoch();
    if(!lock.IsLocked()) return false;
    if(!sessionresumptionactive) return false;
    if(keyID == sessionticketserverkeycurrentID) openingkey.Add(sessionticketserverkeycurrent);
    else if((keyID == sessionticketserverkeypreviousID) && now && now < sessionticketserverkeypreviousexpires)
      openingkey.Add(sessionticketserverkeyprevious);
  }
  if(openingkey.GetSize() != 32) return false;
  XDWORD ciphertextsize = ticket.GetSize() - sizeof(XQWORD) - CIPHERAESGCM_NONCESIZE - CIPHERAESGCM_TAGSIZE;

  if(!nonce.Add(ticket.Get() + sizeof(XQWORD), CIPHERAESGCM_NONCESIZE) ||
     !ciphertext.Add(ticket.Get() + sizeof(XQWORD) + CIPHERAESGCM_NONCESIZE, ciphertextsize) ||
     !tag.Add(ticket.Get() + sizeof(XQWORD) + CIPHERAESGCM_NONCESIZE + ciphertextsize, CIPHERAESGCM_TAGSIZE)) return false;

  static XBYTE aadbytes4[] = { 'G','E','N','-','T','L','S','1','3','-','T','I','C','K','E','T','-','4' };
  if(!aad.Add(aadbytes4, sizeof(aadbytes4)) || !DIOSTREAMTLSCONFIG_AddUInt64BE(aad, keyID) ||
     !key.Set(openingkey) || !cipher.SetKey(&key)) return false;

  XSECUREBUFFER plain;
  bool opened = cipher.UncipherAEAD(ciphertext.Get(), ciphertext.GetSize(), nonce, aad, tag) && cipher.GetResult();
  if(opened) opened=plain.Add((*cipher.GetResult()));

  // Backward compatibility with the stateless v3 tickets emitted before 0-RTT support. They remain usable for
  // ordinary 1-RTT resumption but can never authorize early data.
  if(!opened)
    {
      XBUFFER aad3;
      CIPHERAESGCM cipher3;
      static XBYTE aadbytes3[] = { 'G','E','N','-','T','L','S','1','3','-','T','I','C','K','E','T','-','3' };
      if(!aad3.Add(aadbytes3, sizeof(aadbytes3)) || !DIOSTREAMTLSCONFIG_AddUInt64BE(aad3, keyID) || !cipher3.SetKey(&key) ||
         !cipher3.UncipherAEAD(ciphertext.Get(), ciphertext.GetSize(), nonce, aad3, tag) || !cipher3.GetResult() ||
         !plain.Add((*cipher3.GetResult()))) return false;
    }

  XBYTE version = 0;
  XBYTE protocolsize = 0;
  XBYTE psklength = 0;
  XBYTE namelength = 0;

  XDWORD position = 0;
  if(position >= plain.GetSize()) return false;
  version = plain.Get()[position++];
  maximumearlydatasize = 0;
  if((version != 3 && version != 4) || !DIOSTREAMTLSCONFIG_GetUInt64BE(plain, position, issueepoch) ||
     !DIOSTREAMTLSCONFIG_GetUInt32BE(plain, position, lifetime) ||
     !DIOSTREAMTLSCONFIG_GetUInt32BE(plain, position, ageadd) ||
     !DIOSTREAMTLSCONFIG_GetUInt16BE(plain, position, ciphersuite)) return false;
  if(version==4 && (!DIOSTREAMTLSCONFIG_GetUInt32BE(plain, position, maximumearlydatasize) ||
                    maximumearlydatasize > DIOSTREAMTLS13_EARLYDATA_MAXSIZE)) return false;
  if(position >= plain.GetSize()) return false;
  protocolsize = plain.Get()[position++];
  if(position > plain.GetSize() || (plain.GetSize()-position) < ((XDWORD)protocolsize+2)) return false;

  applicationprotocol.Delete();
  if(protocolsize && (!applicationprotocol.Add(plain.Get()+position, protocolsize))) return false;
  position += protocolsize;
  if(position >= plain.GetSize()) return false;
  psklength = plain.Get()[position++];
  if(!psklength || position > plain.GetSize() || (plain.GetSize()-position) < ((XDWORD)psklength+1)) return false;

  PSK.SecureDelete();
  if(!PSK.Add(plain.Get()+position, psklength)) return false;
  position += psklength;
  if(position >= plain.GetSize()) { PSK.SecureDelete(); return false; }
  namelength = plain.Get()[position++];
  if((plain.GetSize()-position) != namelength) { PSK.SecureDelete(); return false; }

  servername.Empty();
  for(XDWORD c=0; c<namelength; c++)
    {
      XBYTE character = plain.Get()[position++];
      servername.Add((XCHAR)character);
    }

  XQWORD now = DIOSTREAMTLSCONFIG_CurrentEpoch();
  if(!now || !issueepoch || now < issueepoch || !lifetime ||
     lifetime > DIOSTREAMTLS13_SESSIONTICKET_MAX_LIFETIME || (now - issueepoch) >= lifetime)
    {
      PSK.FillBuffer(0);
      PSK.SecureDelete();
      return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::CryptographicPolicy_Validate()
* @brief      Cryptographic policy validate
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::CryptographicPolicy_Validate()
{
  const bool TLS12enabled = (minversion <= DIOSTREAMTLS_MSG_VERSION_TLS_1_2) &&
                            (maxversion >= DIOSTREAMTLS_MSG_VERSION_TLS_1_2);
  const bool TLS13enabled = (minversion <= DIOSTREAMTLS_MSG_VERSION_TLS_1_3) &&
                            (maxversion >= DIOSTREAMTLS_MSG_VERSION_TLS_1_3);

  if(!TLS12enabled && !TLS13enabled) return false;
  if((TLS13enabled && ciphersuites.IsEmpty()) ||
     (TLS12enabled && TLS12ciphersuites.IsEmpty()) ||
     supportedgroups.IsEmpty() || signatureschemes.IsEmpty() || certificatesignatureschemes.IsEmpty()) return false;

  for(XDWORD c=0; c<ciphersuites.GetSize(); c++)
    if(!DIOSTREAMTLSCONFIG_IsCipherSuiteSupported(ciphersuites.Get(c))) return false;

  for(XDWORD c=0; c<TLS12ciphersuites.GetSize(); c++)
    if(!DIOSTREAMTLSCONFIG_IsTLS12CipherSuiteSupported(TLS12ciphersuites.Get(c))) return false;

  bool TLS12group = false;
  for(XDWORD c=0; c<supportedgroups.GetSize(); c++)
    {
      XWORD group = supportedgroups.Get(c);
      if(!DIOSTREAMTLSCONFIG_IsSupportedGroupSupported(group)) return false;
      if((group == DIOSTREAMTLS_MSG_CURVEID_X25519) ||
         (group == DIOSTREAMTLS_MSG_CURVEID_SECP256R1) ||
         (group == DIOSTREAMTLS_MSG_CURVEID_SECP384R1) ||
         (group == DIOSTREAMTLS_MSG_CURVEID_SECP521R1)) TLS12group = true;
    }

  bool RSAsignature = false;
  bool ECDSAsignature = false;
  for(XDWORD c=0; c<signatureschemes.GetSize(); c++)
    {
      XWORD scheme = signatureschemes.Get(c);
      if(!DIOSTREAMTLSCONFIG_IsSignatureSchemeSupported(scheme, false)) return false;
      if((scheme == DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA256) ||
         (scheme == DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA384) ||
         (scheme == DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA512) ||
         (scheme == DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA256) ||
         (scheme == DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA384) ||
         (scheme == DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA512)) RSAsignature = true;
      if((scheme == DIOSTREAMTLS_MSG_SIGNATURESCHEME_ECDSA_SECP256R1_SHA256) ||
         (scheme == DIOSTREAMTLS_MSG_SIGNATURESCHEME_ECDSA_SECP384R1_SHA384) ||
         (scheme == DIOSTREAMTLS_MSG_SIGNATURESCHEME_ECDSA_SECP521R1_SHA512)) ECDSAsignature = true;
    }

  for(XDWORD c=0; c<certificatesignatureschemes.GetSize(); c++)
    if(!DIOSTREAMTLSCONFIG_IsSignatureSchemeSupported(certificatesignatureschemes.Get(c), true)) return false;

  if(TLS12enabled)
    {
      bool compatible = false;
      for(XDWORD c=0; c<TLS12ciphersuites.GetSize(); c++)
        {
          XWORD suite = TLS12ciphersuites.Get(c);
          if(((suite == DIOSTREAMTLS12_CIPHER_ECDHE_RSA_WITH_AES_128_GCM_SHA256) ||
              (suite == DIOSTREAMTLS12_CIPHER_ECDHE_RSA_WITH_AES_256_GCM_SHA384) ||
              (suite == DIOSTREAMTLS_MSG_CIPHER_ECDHE_RSA_WITH_CHACHA20_POLY1305_SHA256)) && RSAsignature) compatible = true;
          if(((suite == DIOSTREAMTLS12_CIPHER_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256) ||
              (suite == DIOSTREAMTLS12_CIPHER_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384) ||
              (suite == DIOSTREAMTLS_MSG_CIPHER_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256)) && ECDSAsignature) compatible = true;
        }

      if(!TLS12group || !compatible) return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLSCONFIG::MemoryPolicy_Validate()
* @brief      Memory policy validate
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSCONFIG::MemoryPolicy_Validate()
{
  if(memorypolicy.GetMaximumAIABodySize() > memorypolicy.GetMaximumHandshakeInputSize()) return false;

  XVECTOR<XBUFFER*>* stores[2] = { &trustedroots, &clienttrustedroots };

  for(int s=0; s<2; s++)
    {
      XVECTOR<XBUFFER*>* store = stores[s];
      XQWORD total = 0;
      if(store->GetSize() > memorypolicy.GetMaximumTrustRoots()) return false;
      for(XDWORD c=0; c<store->GetSize(); c++)
        {
          XBUFFER* certificate = store->Get(c);
          if(!certificate || certificate->IsEmpty() ||
             (certificate->GetSize() > memorypolicy.GetMaximumTrustCertificateSize())) return false;
          total += certificate->GetSize();
          if(total > memorypolicy.GetMaximumTrustStoreSize()) return false;
        }
    }

  if(certificaterevocationlists.GetSize() > memorypolicy.GetMaximumCRLs() ||
     sessiontickets.GetSize() > memorypolicy.GetMaximumSessionTickets() ||
     localOCSPstapledresponse.GetSize() > memorypolicy.GetMaximumOCSPResponseSize()) return false;

  for(XDWORD c=0; c<certificaterevocationlists.GetSize(); c++)
    {
      XBUFFER* CRL = certificaterevocationlists.Get(c);
      if(!CRL || CRL->IsEmpty() || (CRL->GetSize() > memorypolicy.GetMaximumCRLSize())) return false;
    }

  for(XDWORD c=0; c<servercredentials.GetSize(); c++)
    {
      DIOSTREAMTLSSERVERCREDENTIALS* credentials = servercredentials.Get(c);
      if(!credentials ||
         (credentials->GetOCSPStapledResponse()->GetSize() > memorypolicy.GetMaximumOCSPResponseSize())) return false;
    }

  {
    XQWORD total = 0;
    if(localcertificatechain.GetSize() > certificatevalidationpolicy.GetMaximumChainDepth()) return false;
    for(XDWORD c=0; c<localcertificatechain.GetSize(); c++)
    {
      XBUFFER* certificate = localcertificatechain.Get(c);
      if(!certificate || certificate->IsEmpty()) return false;
      total += certificate->GetSize();
      if(total > memorypolicy.GetMaximumHandshakeInputSize()) return false;
    }
  }

  for(XDWORD s=0; s<servercredentials.GetSize(); s++)
    {
      DIOSTREAMTLSSERVERCREDENTIALS* credentials = servercredentials.Get(s);
      XVECTOR<XBUFFER*>* chain = credentials?credentials->GetCertificateChain():NULL;
      XQWORD total = 0;
      if(!chain || (chain->GetSize() > certificatevalidationpolicy.GetMaximumChainDepth())) return false;
      for(XDWORD c=0; c<chain->GetSize(); c++)
        {
          XBUFFER* certificate = chain->Get(c);
          if(!certificate || certificate->IsEmpty()) return false;
          total += certificate->GetSize();
          if(total > memorypolicy.GetMaximumHandshakeInputSize()) return false;
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTLSCONFIG::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLSCONFIG::Clean()
{
  frozen = false;
  // Default negotiation window is TLS 1.3 only, matching every previous phase exactly: nothing changes for an
  // existing caller unless it explicitly calls SetMinVersion(DIOSTREAMTLS_MSG_VERSION_TLS_1_2).
  minversion = DIOSTREAMTLS_MSG_VERSION_TLS_1_3;
  maxversion = DIOSTREAMTLS_MSG_VERSION_TLS_1_3;

  CipherSuites_Delete();
  CipherSuites_AddRecommendedProfile();

  TLS12CipherSuites_Delete();
  TLS12CipherSuite_Add(DIOSTREAMTLS12_CIPHER_ECDHE_RSA_WITH_AES_128_GCM_SHA256);
  TLS12CipherSuite_Add(DIOSTREAMTLS12_CIPHER_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256);

  #ifdef CIPHER_SYMMETRIC_CHACHA20POLY1305_ACTIVE

  TLS12CipherSuite_Add(DIOSTREAMTLS_MSG_CIPHER_ECDHE_RSA_WITH_CHACHA20_POLY1305_SHA256);
  TLS12CipherSuite_Add(DIOSTREAMTLS_MSG_CIPHER_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256);

  #endif

  TLS12CipherSuite_Add(DIOSTREAMTLS12_CIPHER_ECDHE_RSA_WITH_AES_256_GCM_SHA384);
  TLS12CipherSuite_Add(DIOSTREAMTLS12_CIPHER_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384);

  SupportedGroups_Delete();
  SupportedGroup_Add(DIOSTREAMTLS_MSG_CURVEID_X25519MLKEM768);
  SupportedGroup_Add(DIOSTREAMTLS_MSG_CURVEID_SECP256R1MLKEM768);
  SupportedGroup_Add(DIOSTREAMTLS_MSG_CURVEID_SECP384R1MLKEM1024);
  SupportedGroup_Add(DIOSTREAMTLS_MSG_CURVEID_X25519);
  SupportedGroup_Add(DIOSTREAMTLS_MSG_CURVEID_SECP256R1);
  SupportedGroup_Add(DIOSTREAMTLS_MSG_CURVEID_SECP384R1);
  SupportedGroup_Add(DIOSTREAMTLS_MSG_CURVEID_SECP521R1);

  // Default handshake-signing profile. Keep the existing RSA-PSS defaults for backwards compatibility. The trust
  // anchor source is configured independently through CIPHERTRUSTPROVIDERX509, so applications can extend these
  // schemes without coupling that decision to the selected Root CA provider.
  SignatureSchemes_Delete();
  SignatureScheme_Add(DIOSTREAMTLS_MSG_SIGNATURESCHEME_ED25519);
  SignatureScheme_Add(DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA256);
  SignatureScheme_Add(DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA384);
  SignatureScheme_Add(DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA512);
  SignatureScheme_Add(DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA256);
  SignatureScheme_Add(DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA384);
  SignatureScheme_Add(DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA512);
  SignatureScheme_Add(DIOSTREAMTLS_MSG_SIGNATURESCHEME_ECDSA_SECP256R1_SHA256);
  SignatureScheme_Add(DIOSTREAMTLS_MSG_SIGNATURESCHEME_ECDSA_SECP384R1_SHA384);
  SignatureScheme_Add(DIOSTREAMTLS_MSG_SIGNATURESCHEME_ECDSA_SECP521R1_SHA512);

  CertificateSignatureSchemes_Delete();
  CertificateSignatureScheme_Add(DIOSTREAMTLS_MSG_SIGNATURESCHEME_ED25519);
  CertificateSignatureScheme_Add(DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA256);
  CertificateSignatureScheme_Add(DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA384);
  CertificateSignatureScheme_Add(DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA512);
  CertificateSignatureScheme_Add(DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA256);
  CertificateSignatureScheme_Add(DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA384);
  CertificateSignatureScheme_Add(DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA512);
  CertificateSignatureScheme_Add(DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PKCS1_SHA256);
  CertificateSignatureScheme_Add(DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PKCS1_SHA384);
  CertificateSignatureScheme_Add(DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PKCS1_SHA512);
  CertificateSignatureScheme_Add(DIOSTREAMTLS_MSG_SIGNATURESCHEME_ECDSA_SECP256R1_SHA256);
  CertificateSignatureScheme_Add(DIOSTREAMTLS_MSG_SIGNATURESCHEME_ECDSA_SECP384R1_SHA384);
  CertificateSignatureScheme_Add(DIOSTREAMTLS_MSG_SIGNATURESCHEME_ECDSA_SECP521R1_SHA512);

  ApplicationProtocols_Delete();

  servername.Empty();
  TrustedRoots_Delete();
  truststorefallbackpolicy = DIOSTREAMTLS_TRUSTSTORE_FALLBACKPOLICY_ON_NATIVE_FAILURE;
  ClientTrustedRoots_Delete();
  clientauthenticationmode = DIOSTREAMTLS_CLIENTAUTHENTICATION_MODE_NONE;
  LocalCredentials_Delete();
  ServerCredentials_Delete();
  localcredentialserror       = DIOSTREAMTLS_LOCALCREDENTIALSERROR_NONE;
  revocationpolicy            = DIOSTREAMTLS_REVOCATIONPOLICY_HARD_FAIL;

  aiafetchactive    = true;
  aiafetchtimeout   = 5;
  connectiontimeout = DIOSTREAMTLS_DEFAULT_CONNECTION_TIMEOUT;
  handshaketimeout  = DIOSTREAMTLS_DEFAULT_HANDSHAKE_TIMEOUT;
  CertificateRevocationLists_Delete();
  ocspdirectfetcher = NULL;
  ocspdirectcontext = NULL;

  SessionTickets_Delete();
  sessionticketserverkeycurrent.SecureDelete();
  sessionticketserverkeyprevious.SecureDelete();
  sessionticketserverkeycurrentID       = 0;
  sessionticketserverkeygeneration     = 0;
  sessionticketserverkeypreviousID      = 0;
  sessionticketserverkeycurrentcreated  = 0;
  sessionticketserverkeypreviousexpires = 0;
  sessionticketkeyringload    = NULL;
  sessionticketkeyringsave    = NULL;
  sessionticketkeyringcontext = NULL;
  sessionticketkeyringwrappingkey.SecureDelete();
  sessionticketkeyringdirty = false;
  sessionresumptionactive = true;
  earlydataactive = false;
  maximumearlydatasize = DIOSTREAMTLS13_EARLYDATA_DEFAULT_MAXSIZE;
  earlydatareplaycheck = NULL;
  earlydatareplaycontext = NULL;
  sessionticketlifetime   = DIOSTREAMTLS13_SESSIONTICKET_DEFAULT_LIFETIME;
  sessionticketkeyrotationinterval = DIOSTREAMTLS13_SESSIONTICKET_DEFAULT_KEYROTATION;
}
