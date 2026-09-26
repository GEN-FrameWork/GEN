/**-------------------------------------------------------------------------------------------------------------------
*
* @file       DIOStreamTLS13KeySchedule.cpp
*
* @class      DIOSTREAMTLS13KEYSCHEDULE
* @brief      Data Input/Output Stream TLS Key Schedule (TLS 1.3, RFC 8446 section 7.1) class
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

#include "DIOStreamTLS13KeySchedule.h"

#include "XFactory.h"
#include "XTrace.h"

#include "HashSHA2.h"
#include "HashHMAC.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOStreamTLS_BufferErase(XBUFFER& buffer)
* @brief      Dio stream tls buffer erase
* @ingroup    DATAIO
* 
* @param[in]  buffer : Buffer value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static void DIOStreamTLS_BufferErase(XBUFFER& buffer)
{
  volatile XBYTE* data = buffer.Get();

  for(XDWORD c=0; c<buffer.GetSize(); c++) data[c] = 0;

  buffer.Delete();
}



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS13KEYSCHEDULE::DIOSTREAMTLS13KEYSCHEDULE()
* @brief      Constructor of class
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS13KEYSCHEDULE::DIOSTREAMTLS13KEYSCHEDULE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS13KEYSCHEDULE::~DIOSTREAMTLS13KEYSCHEDULE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS13KEYSCHEDULE::~DIOSTREAMTLS13KEYSCHEDULE()
{
  End();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13KEYSCHEDULE::Ini(XWORD ciphersuite, DIOSTREAMTLSKEYSCHEDULE_ROLE role)
* @brief      Prepare the key schedule for a cipher suite and a role
* @note       The role only decides which of the two traffic secrets is the local one. Everything else is symmetrical,
*             which is what makes this class usable by a server without any change.
* @ingroup    DATAIO
*
* @param[in]  ciphersuite : Negotiated TLS 1.3 cipher suite.
* @param[in]  role : Role of this end of the connection.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13KEYSCHEDULE::Ini(XWORD ciphersuite, DIOSTREAMTLSKEYSCHEDULE_ROLE role)
{
  End();

  switch(ciphersuite)
    {
      case DIOSTREAMTLS_MSG_CIPHER_AES_128_GCM_SHA256   : hash     = GEN_NEW HASHSHA2(HASHSHA2TYPE_256);
                                                          keysize  = 16;
                                                          break;

      case DIOSTREAMTLS_MSG_CIPHER_AES_256_GCM_SHA384   : hash     = GEN_NEW HASHSHA2(HASHSHA2TYPE_384);
                                                          keysize  = 32;
                                                          break;

      #ifdef CIPHER_SYMMETRIC_CHACHA20POLY1305_ACTIVE

      case DIOSTREAMTLS_MSG_CIPHER_CHACHA20_POLY1305_SHA256
                                                        : hash     = GEN_NEW HASHSHA2(HASHSHA2TYPE_256);
                                                          keysize  = 32;
                                                          break;

      #endif

                                              default   : return false;
    }

  if(!hash)
    {
      return false;
    }

  hashsize = (XDWORD)hash->GetDefaultSize();
  IVsize   = DIOSTREAMTLS13KEYSCHEDULE_IVSIZE;

  if(!hashsize)
    {
      End();
      return false;
    }

  HKDF = GEN_NEW CIPHERHKDF(hash);
  if(!HKDF)
    {
      End();
      return false;
    }

  this->ciphersuite = ciphersuite;
  this->role        = role;

  isini             = true;

  /* XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[TLS Key Schedule] Ini: cipher suite %04X, %s, hash %d, key %d, iv %d"), ciphersuite,
                                       (role == DIOSTREAMTLSKEYSCHEDULE_ROLE_CLIENT)?__L("client"):__L("server"), hashsize, keysize, IVsize); */

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS13KEYSCHEDULE::End()
* @brief      Release everything and wipe every secret
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS13KEYSCHEDULE::End()
{
  if(HKDF)
    {
      if(HKDF->GetResult()) DIOStreamTLS_BufferErase((*HKDF->GetResult()));

      GEN_DELETE HKDF;
      HKDF = NULL;
    }

  if(hash)
    {
      if(hash->GetResult()) DIOStreamTLS_BufferErase((*hash->GetResult()));

      GEN_DELETE hash;
      hash = NULL;
    }

  DIOStreamTLS_BufferErase(earlysecret);
  DIOStreamTLS_BufferErase(handshakesecret);
  DIOStreamTLS_BufferErase(mastersecret);
  DIOStreamTLS_BufferErase(resumptionsecret);

  DIOStreamTLS_BufferErase(clientearlytrafficsecret);
  DIOStreamTLS_BufferErase(clienthandshaketrafficsecret);
  DIOStreamTLS_BufferErase(serverhandshaketrafficsecret);
  DIOStreamTLS_BufferErase(clientapplicationtrafficsecret);
  DIOStreamTLS_BufferErase(serverapplicationtrafficsecret);

  ciphersuite = 0;
  hashsize    = 0;
  keysize     = 0;
  IVsize      = 0;
  isini       = false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13KEYSCHEDULE::IsIni()
* @brief      Is the key schedule ready to be used
* @ingroup    DATAIO
*
* @return     bool : true if the condition is met; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13KEYSCHEDULE::IsIni()
{
  return isini;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XWORD DIOSTREAMTLS13KEYSCHEDULE::GetCipherSuite()
* @brief      Get cipher suite
* @ingroup    DATAIO
*
* @return     XWORD : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
XWORD DIOSTREAMTLS13KEYSCHEDULE::GetCipherSuite()
{
  return ciphersuite;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLSKEYSCHEDULE_ROLE DIOSTREAMTLS13KEYSCHEDULE::GetRole()
* @brief      Get role
* @ingroup    DATAIO
*
* @return     DIOSTREAMTLSKEYSCHEDULE_ROLE : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLSKEYSCHEDULE_ROLE DIOSTREAMTLS13KEYSCHEDULE::GetRole()
{
  return role;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD DIOSTREAMTLS13KEYSCHEDULE::GetHashSize()
* @brief      Get the size, in bytes, of the digest of the negotiated hash
* @ingroup    DATAIO
*
* @return     XDWORD : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAMTLS13KEYSCHEDULE::GetHashSize()
{
  return hashsize;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD DIOSTREAMTLS13KEYSCHEDULE::GetKeySize()
* @brief      Get the size, in bytes, of the traffic keys
* @ingroup    DATAIO
*
* @return     XDWORD : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAMTLS13KEYSCHEDULE::GetKeySize()
{
  return keysize;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD DIOSTREAMTLS13KEYSCHEDULE::GetIVSize()
* @brief      Get the size, in bytes, of the traffic initialization vectors
* @ingroup    DATAIO
*
* @return     XDWORD : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAMTLS13KEYSCHEDULE::GetIVSize()
{
  return IVsize;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         HASH* DIOSTREAMTLS13KEYSCHEDULE::GetHash()
* @brief      Get the hash of the negotiated cipher suite
* @ingroup    DATAIO
*
* @return     HASH* : Pointer to the requested object; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
HASH* DIOSTREAMTLS13KEYSCHEDULE::GetHash()
{
  return hash;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         CIPHERHKDF* DIOSTREAMTLS13KEYSCHEDULE::GetHKDF()
* @brief      Get the key derivation function
* @ingroup    DATAIO
*
* @return     CIPHERHKDF* : Pointer to the requested object; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERHKDF* DIOSTREAMTLS13KEYSCHEDULE::GetHKDF()
{
  return HKDF;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13KEYSCHEDULE::TranscriptHash(XBUFFER& messages, XBUFFER& transcripthash)
* @brief      Calculate the transcript hash of the concatenated handshake messages
* @ingroup    DATAIO
*
* @param[in]  messages : Concatenation of the handshake messages. It may be empty.
* @param[out] transcripthash : Buffer that receives the digest.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13KEYSCHEDULE::TranscriptHash(XBUFFER& messages, XBUFFER& transcripthash)
{
  if(!isini || !hash)
    {
      return false;
    }

  XBYTE emptymessages = 0;

  transcripthash.Delete();

  hash->ResetResult();

  if(!hash->Do(messages.GetSize()?messages.Get():&emptymessages, (XQWORD)messages.GetSize()))
    {
      return false;
    }

  transcripthash.Add(hash->GetResult());

  hash->ResetResult();

  return (transcripthash.GetSize() == hashsize)?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13KEYSCHEDULE::EarlySecret_Calculate(XBUFFER* PSK)
* @brief      Early Secret = HKDF-Extract(0, PSK). Without a PSK the input keying material is HashLen zero bytes
* @ingroup    DATAIO
*
* @param[in]  PSK : Pre shared key. NULL when there is none, which is the usual case of a full handshake.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13KEYSCHEDULE::EarlySecret_Calculate(XBUFFER* PSK)
{
  if(!isini || !HKDF)
    {
      return false;
    }

  XBUFFER salt;
  XBUFFER zeroPSK;

  if(!PSK)
    {
      if(!zeroPSK.Resize(hashsize))
        {
          return false;
        }

      memset(zeroPSK.Get(), 0, hashsize);
    }

  bool status = HKDF->Extract(salt, PSK?(*PSK):zeroPSK, earlysecret);

  /* XTRACE_PRINTCOLOR(XTRACE_COLOR_CHECKERROR(status), __L("[TLS Key Schedule] Early Secret: %s"), status?__L("Ok"):__L("Error!")); */

  #ifdef DIOSTREAMTLS13KEYSCHEDULE_TRACE_SECRETS
  if(status)
    {
      XTRACE_PRINTDATABLOCKCOLOR(XTRACE_COLOR_BLUE, earlysecret);
    }
  #endif

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13KEYSCHEDULE::HandshakeSecret_Calculate(XBUFFER& sharedsecret)
* @brief      Handshake Secret = HKDF-Extract(Derive-Secret(Early Secret, "derived", ""), (EC)DHE)
* @ingroup    DATAIO
*
* @param[in]  sharedsecret : Shared secret of the key exchange, for example the X25519 one.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13KEYSCHEDULE::HandshakeSecret_Calculate(XBUFFER& sharedsecret)
{
  if(!isini || !HKDF)
    {
      return false;
    }

  if(earlysecret.GetSize() != hashsize)
    {
      return false;
    }

  if(!sharedsecret.GetSize())
    {
      return false;
    }

  XSECUREBUFFER derivedsecret;

  if(!DeriveEmptySecret(earlysecret, DIOSTREAMTLS13KEYSCHEDULE_LABEL_DERIVED, derivedsecret))
    {
      return false;
    }

  bool status = HKDF->Extract(derivedsecret, sharedsecret, handshakesecret);

  /* XTRACE_PRINTCOLOR(XTRACE_COLOR_CHECKERROR(status), __L("[TLS Key Schedule] Handshake Secret: %s"), status?__L("Ok"):__L("Error!")); */

  #ifdef DIOSTREAMTLS13KEYSCHEDULE_TRACE_SECRETS
  if(status)
    {
      XTRACE_PRINTDATABLOCKCOLOR(XTRACE_COLOR_BLUE, handshakesecret);
    }
  #endif

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13KEYSCHEDULE::MasterSecret_Calculate()
* @brief      Master Secret = HKDF-Extract(Derive-Secret(Handshake Secret, "derived", ""), 0)
* @ingroup    DATAIO
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13KEYSCHEDULE::MasterSecret_Calculate()
{
  if(!isini || !HKDF)
    {
      return false;
    }

  if(handshakesecret.GetSize() != hashsize)
    {
      return false;
    }

  XSECUREBUFFER derivedsecret;
  XSECUREBUFFER zerokeymaterial;

  if(!DeriveEmptySecret(handshakesecret, DIOSTREAMTLS13KEYSCHEDULE_LABEL_DERIVED, derivedsecret))
    {
      return false;
    }

  if(!zerokeymaterial.Resize(hashsize))
    {
      return false;
    }

  memset(zerokeymaterial.Get(), 0, hashsize);

  bool status = HKDF->Extract(derivedsecret, zerokeymaterial, mastersecret);

  /* XTRACE_PRINTCOLOR(XTRACE_COLOR_CHECKERROR(status), __L("[TLS Key Schedule] Master Secret: %s"), status?__L("Ok"):__L("Error!")); */

  #ifdef DIOSTREAMTLS13KEYSCHEDULE_TRACE_SECRETS
  if(status)
    {
      XTRACE_PRINTDATABLOCKCOLOR(XTRACE_COLOR_BLUE, mastersecret);
    }
  #endif

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13KEYSCHEDULE::EarlyTrafficSecret_Calculate(XBUFFER& transcripthash)
* @brief      Derive the client early traffic secret from ClientHello transcript
* @ingroup    DATAIO
*
* @param[in]  transcripthash : Hash of the ClientHello that carries the PSK binder.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13KEYSCHEDULE::EarlyTrafficSecret_Calculate(XBUFFER& transcripthash)
{
  if(!isini || !HKDF || (earlysecret.GetSize() != hashsize) || (transcripthash.GetSize() != hashsize)) return false;

  DIOStreamTLS_BufferErase(clientearlytrafficsecret);
  return HKDF->ExpandLabel(earlysecret, DIOSTREAMTLS13KEYSCHEDULE_LABEL_CLIENTEARLY, transcripthash, hashsize, clientearlytrafficsecret);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS13KEYSCHEDULE::HandshakeTrafficSecrets_Calculate(XBUFFER& transcripthash)
* @brief      Handshake traffic secrets calculate
* @ingroup    DATAIO
* 
* @param[in]  transcripthash : Transcripthash value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13KEYSCHEDULE::HandshakeTrafficSecrets_Calculate(XBUFFER& transcripthash)
{
  if(!isini || !HKDF)
    {
      return false;
    }

  if((handshakesecret.GetSize() != hashsize) || (transcripthash.GetSize() != hashsize))
    {
      return false;
    }

  if(!HKDF->ExpandLabel(handshakesecret, DIOSTREAMTLS13KEYSCHEDULE_LABEL_CLIENTHANDSHAKE, transcripthash, hashsize, clienthandshaketrafficsecret))
    {
      return false;
    }

  bool status = HKDF->ExpandLabel(handshakesecret, DIOSTREAMTLS13KEYSCHEDULE_LABEL_SERVERHANDSHAKE, transcripthash, hashsize, serverhandshaketrafficsecret);

  /* XTRACE_PRINTCOLOR(XTRACE_COLOR_CHECKERROR(status), __L("[TLS Key Schedule] Handshake traffic secrets: %s"), status?__L("Ok"):__L("Error!")); */

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13KEYSCHEDULE::ApplicationTrafficSecrets_Calculate(XBUFFER& transcripthash)
* @brief      Derive both application traffic secrets from the transcript up to the server Finished
* @ingroup    DATAIO
*
* @param[in]  transcripthash : Transcript hash of ClientHello..server Finished.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13KEYSCHEDULE::ApplicationTrafficSecrets_Calculate(XBUFFER& transcripthash)
{
  if(!isini || !HKDF)
    {
      return false;
    }

  if((mastersecret.GetSize() != hashsize) || (transcripthash.GetSize() != hashsize))
    {
      return false;
    }

  if(!HKDF->ExpandLabel(mastersecret, DIOSTREAMTLS13KEYSCHEDULE_LABEL_CLIENTAPPLICATION, transcripthash, hashsize, clientapplicationtrafficsecret))
    {
      return false;
    }

  bool status = HKDF->ExpandLabel(mastersecret, DIOSTREAMTLS13KEYSCHEDULE_LABEL_SERVERAPPLICATION, transcripthash, hashsize, serverapplicationtrafficsecret);

  /* XTRACE_PRINTCOLOR(XTRACE_COLOR_CHECKERROR(status), __L("[TLS Key Schedule] Application traffic secrets: %s"), status?__L("Ok"):__L("Error!")); */

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13KEYSCHEDULE::ResumptionSecret_Calculate(XBUFFER& transcripthash)
* @brief      Derive the resumption master secret from the transcript up to the client Finished
* @ingroup    DATAIO
*
* @param[in]  transcripthash : Transcript hash of ClientHello..client Finished.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13KEYSCHEDULE::ResumptionSecret_Calculate(XBUFFER& transcripthash)
{
  if(!isini || !HKDF)
    {
      return false;
    }

  if((mastersecret.GetSize() != hashsize) || (transcripthash.GetSize() != hashsize))
    {
      return false;
    }

  return HKDF->ExpandLabel(mastersecret, DIOSTREAMTLS13KEYSCHEDULE_LABEL_RESUMPTION, transcripthash, hashsize, resumptionsecret);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XBUFFER* DIOSTREAMTLS13KEYSCHEDULE::GetEarlySecret()
* @brief      Get early secret
* @ingroup    DATAIO
*
* @return     XBUFFER* : Pointer to the requested buffer.
*
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* DIOSTREAMTLS13KEYSCHEDULE::GetEarlySecret()
{
  return &earlysecret;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XBUFFER* DIOSTREAMTLS13KEYSCHEDULE::GetHandshakeSecret()
* @brief      Get handshake secret
* @ingroup    DATAIO
*
* @return     XBUFFER* : Pointer to the requested buffer.
*
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* DIOSTREAMTLS13KEYSCHEDULE::GetHandshakeSecret()
{
  return &handshakesecret;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XBUFFER* DIOSTREAMTLS13KEYSCHEDULE::GetMasterSecret()
* @brief      Get master secret
* @ingroup    DATAIO
*
* @return     XBUFFER* : Pointer to the requested buffer.
*
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* DIOSTREAMTLS13KEYSCHEDULE::GetMasterSecret()
{
  return &mastersecret;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XBUFFER* DIOSTREAMTLS13KEYSCHEDULE::GetResumptionSecret()
* @brief      Get resumption master secret
* @ingroup    DATAIO
*
* @return     XBUFFER* : Pointer to the requested buffer.
*
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* DIOSTREAMTLS13KEYSCHEDULE::GetResumptionSecret()
{
  return &resumptionsecret;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13KEYSCHEDULE::ResumptionPSK_Calculate(XBUFFER& ticketnonce, XBUFFER& PSK)
* @brief      Derive the PSK associated with one NewSessionTicket nonce
* @ingroup    DATAIO
*
* @param[in]  ticketnonce : Nonce carried by NewSessionTicket.
* @param[out] PSK : Derived resumption PSK.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13KEYSCHEDULE::ResumptionPSK_Calculate(XBUFFER& ticketnonce, XBUFFER& PSK)
{
  if(!isini || !HKDF || (resumptionsecret.GetSize() != hashsize)) return false;

  PSK.SecureDelete();
  return HKDF->ExpandLabel(resumptionsecret, DIOSTREAMTLS13KEYSCHEDULE_LABEL_RESUMPTIONPSK, ticketnonce, hashsize, PSK);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13KEYSCHEDULE::Binder_Calculate(XBUFFER& transcript, XBUFFER& binder)
* @brief      Calculate a resumption-PSK binder over the truncated ClientHello transcript
* @ingroup    DATAIO
*
* @param[in]  transcript : Binder transcript (ClientHello with the binders list removed, including correct lengths).
* @param[out] binder : Calculated binder.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13KEYSCHEDULE::Binder_Calculate(XBUFFER& transcript, XBUFFER& binder)
{
  XSECUREBUFFER binderkey;
  XSECUREBUFFER finishedkey;
  XBUFFER  transcripthash;
  XBUFFER  context;
  HASHHMAC hashHMAC(hash);

  if(!isini || !HKDF || !hash || (earlysecret.GetSize() != hashsize)) return false;
  if(!DeriveEmptySecret(earlysecret, DIOSTREAMTLS13KEYSCHEDULE_LABEL_RESUMPTIONBINDER, binderkey)) return false;
  if(!HKDF->ExpandLabel(binderkey, DIOSTREAMTLS13KEYSCHEDULE_LABEL_FINISHED, context, hashsize, finishedkey)) return false;
  if(!TranscriptHash(transcript, transcripthash) || !hashHMAC.SetKey(finishedkey)) return false;

  hashHMAC.ResetResult();
  if(!hashHMAC.Do(transcripthash)) return false;

  binder.Delete();
  if(!binder.Add(hashHMAC.GetResult())) return false;

  return (binder.GetSize() == hashsize);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XBUFFER* DIOSTREAMTLS13KEYSCHEDULE::GetTrafficSecret(DIOSTREAMTLS13KEYSCHEDULE_LEVEL level, DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction)
* @brief      Get a traffic secret by level and direction, never by role
* @ingroup    DATAIO
*
* @param[in]  level : Handshake or application.
* @param[in]  direction : Local, what this end writes, or remote, what the other end writes.
*
* @return     XBUFFER* : Pointer to the requested buffer; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* DIOSTREAMTLS13KEYSCHEDULE::GetTrafficSecret(DIOSTREAMTLS13KEYSCHEDULE_LEVEL level, DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction)
{
  bool isclient = IsDirectionOfTheClient(direction);

  switch(level)
    {
      case DIOSTREAMTLS13KEYSCHEDULE_LEVEL_EARLY        : return isclient?&clientearlytrafficsecret:NULL;

      case DIOSTREAMTLS13KEYSCHEDULE_LEVEL_HANDSHAKE    : return isclient?&clienthandshaketrafficsecret:&serverhandshaketrafficsecret;

      case DIOSTREAMTLS13KEYSCHEDULE_LEVEL_APPLICATION  : return isclient?&clientapplicationtrafficsecret:&serverapplicationtrafficsecret;

      case DIOSTREAMTLS13KEYSCHEDULE_LEVEL_NONE         :
                                          default     : break;
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13KEYSCHEDULE::GetTrafficKeys(DIOSTREAMTLS13KEYSCHEDULE_LEVEL level, DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction, XBUFFER& key, XBUFFER& IV)
* @brief      Derive the write key and the write initialization vector of a traffic secret
* @ingroup    DATAIO
*
* @param[in]  level : Handshake or application.
* @param[in]  direction : Local or remote.
* @param[out] key : Buffer that receives the key.
* @param[out] IV : Buffer that receives the initialization vector.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13KEYSCHEDULE::GetTrafficKeys(DIOSTREAMTLS13KEYSCHEDULE_LEVEL level, DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction, XBUFFER& key, XBUFFER& IV)
{
  if(!isini || !HKDF)
    {
      return false;
    }

  XBUFFER* trafficsecret = GetTrafficSecret(level, direction);
  XBUFFER  context;

  if(!trafficsecret)
    {
      return false;
    }

  if(trafficsecret->GetSize() != hashsize)
    {
      return false;
    }

  if(!HKDF->ExpandLabel((*trafficsecret), DIOSTREAMTLS13KEYSCHEDULE_LABEL_KEY, context, keysize, key))
    {
      return false;
    }

  return HKDF->ExpandLabel((*trafficsecret), DIOSTREAMTLS13KEYSCHEDULE_LABEL_IV, context, IVsize, IV);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13KEYSCHEDULE::GetFinishedKey(DIOSTREAMTLS13KEYSCHEDULE_LEVEL level, DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction, XBUFFER& finishedkey)
* @brief      Derive the key that computes the verify data of a Finished message
* @ingroup    DATAIO
*
* @param[in]  level : Level of the base key, normally handshake.
* @param[in]  direction : Local for the Finished this end sends, remote for the one it receives.
* @param[out] finishedkey : Buffer that receives the key.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13KEYSCHEDULE::GetFinishedKey(DIOSTREAMTLS13KEYSCHEDULE_LEVEL level, DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction, XBUFFER& finishedkey)
{
  if(!isini || !HKDF)
    {
      return false;
    }

  XBUFFER* trafficsecret = GetTrafficSecret(level, direction);
  XBUFFER  context;

  if(!trafficsecret)
    {
      return false;
    }

  if(trafficsecret->GetSize() != hashsize)
    {
      return false;
    }

  return HKDF->ExpandLabel((*trafficsecret), DIOSTREAMTLS13KEYSCHEDULE_LABEL_FINISHED, context, hashsize, finishedkey);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13KEYSCHEDULE::CalculateFinished(DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction, XBUFFER& transcripthash, XBUFFER& verifydata)
* @brief      verify_data = HMAC(finished_key, Transcript-Hash(Handshake Context, Certificate*, CertificateVerify*))
* @ingroup    DATAIO
*
* @param[in]  direction : Local for the Finished this end sends, remote for the one it receives.
* @param[in]  transcripthash : Transcript hash up to, but not including, the Finished message.
* @param[out] verifydata : Buffer that receives the verify data.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13KEYSCHEDULE::CalculateFinished(DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction, XBUFFER& transcripthash, XBUFFER& verifydata)
{
  if(!isini || !hash)
    {
      return false;
    }

  if(transcripthash.GetSize() != hashsize)
    {
      return false;
    }

  XSECUREBUFFER finishedkey;
  HASHHMAC  hashHMAC(hash);

  if(!GetFinishedKey(DIOSTREAMTLS13KEYSCHEDULE_LEVEL_HANDSHAKE, direction, finishedkey))
    {
      return false;
    }

  if(!hashHMAC.SetKey(finishedkey))
    {
      return false;
    }

  hashHMAC.ResetResult();

  if(!hashHMAC.Do(transcripthash))
    {
      return false;
    }

  verifydata.Delete();
  verifydata.Add(hashHMAC.GetResult());

  return (verifydata.GetSize() == hashsize)?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13KEYSCHEDULE::VerifyFinished(DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction, XBUFFER& transcripthash, XBUFFER& verifydata)
* @brief      Verify the verify data of a received Finished message
* @note       The comparison is made in constant time, so that its duration does not tell an attacker how much of a
*             forged value was right.
* @ingroup    DATAIO
*
* @param[in]  direction : Direction of the Finished being verified.
* @param[in]  transcripthash : Transcript hash up to, but not including, the Finished message.
* @param[in]  verifydata : Verify data received.
*
* @return     bool : true if the verify data is valid; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13KEYSCHEDULE::VerifyFinished(DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction, XBUFFER& transcripthash, XBUFFER& verifydata)
{
  XBUFFER calculatedverifydata;

  if(verifydata.GetSize() != hashsize)
    {
      return false;
    }

  if(!CalculateFinished(direction, transcripthash, calculatedverifydata))
    {
      return false;
    }

  bool status = CIPHER::CompareConstantTime(calculatedverifydata, verifydata);

  /* XTRACE_PRINTCOLOR(XTRACE_COLOR_CHECKERROR(status), __L("[TLS Key Schedule] Finished of the %s end: %s"),
                                       (direction == DIOSTREAMTLSKEYSCHEDULE_DIRECTION_LOCAL)?__L("local"):__L("remote"),
                                       status?__L("verified"):__L("NOT VALID!")); */

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13KEYSCHEDULE::UpdateTrafficSecret(DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction)
* @brief      Advance an application traffic secret one generation, as the KeyUpdate message requires
* @ingroup    DATAIO
*
* @param[in]  direction : Direction whose secret is advanced.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13KEYSCHEDULE::UpdateTrafficSecret(DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction)
{
  if(!isini || !HKDF)
    {
      return false;
    }

  XBUFFER* trafficsecret = GetTrafficSecret(DIOSTREAMTLS13KEYSCHEDULE_LEVEL_APPLICATION, direction);
  XBUFFER  context;
  XSECUREBUFFER newtrafficsecret;

  if(!trafficsecret)
    {
      return false;
    }

  if(trafficsecret->GetSize() != hashsize)
    {
      return false;
    }

  if(!HKDF->ExpandLabel((*trafficsecret), DIOSTREAMTLS13KEYSCHEDULE_LABEL_TRAFFICUPDATE, context, hashsize, newtrafficsecret))
    {
      return false;
    }

  trafficsecret->SecureDelete();

  return trafficsecret->Add(newtrafficsecret);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13KEYSCHEDULE::IsDirectionOfTheClient(DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction)
* @brief      Resolve a direction into one of the two roles, using the role of this end
* @note       INTERNAL. This single method is the whole of the client and server asymmetry of the key schedule.
* @ingroup    DATAIO
*
* @param[in]  direction : Local or remote.
*
* @return     bool : true if that direction belongs to the client; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13KEYSCHEDULE::IsDirectionOfTheClient(DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction)
{
  if(direction == DIOSTREAMTLSKEYSCHEDULE_DIRECTION_LOCAL)
    {
      return (role == DIOSTREAMTLSKEYSCHEDULE_ROLE_CLIENT)?true:false;
    }

  return (role == DIOSTREAMTLSKEYSCHEDULE_ROLE_SERVER)?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13KEYSCHEDULE::DeriveEmptySecret(XBUFFER& secret, XCHAR* label, XBUFFER& derivedsecret)
* @brief      Derive-Secret(secret, label, "") : the transitions between the three stages of the schedule
* @note       INTERNAL
* @ingroup    DATAIO
*
* @param[in]  secret : Secret to derive from.
* @param[in]  label : Label, without the prefix.
* @param[out] derivedsecret : Buffer that receives the derived secret.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13KEYSCHEDULE::DeriveEmptySecret(XBUFFER& secret, XCHAR* label, XBUFFER& derivedsecret)
{
  XBUFFER nomessages;
  XBUFFER transcripthash;

  if(!TranscriptHash(nomessages, transcripthash))
    {
      return false;
    }

  return HKDF->ExpandLabel(secret, label, transcripthash, hashsize, derivedsecret);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS13KEYSCHEDULE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS13KEYSCHEDULE::Clean()
{
  ciphersuite = 0;
  role        = DIOSTREAMTLSKEYSCHEDULE_ROLE_CLIENT;
  isini       = false;

  hashsize    = 0;
  keysize     = 0;
  IVsize      = 0;

  hash        = NULL;
  HKDF        = NULL;
}
