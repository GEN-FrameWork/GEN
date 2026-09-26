/**-------------------------------------------------------------------------------------------------------------------
*
* @file       DIOStreamTLS13Session.cpp
*
* @class      DIOSTREAMTLS13SESSION
* @brief      Data Input/Output Stream TLS 1.3 role-neutral Session class
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

#include "DIOStreamTLS13Session.h"
#include "DIOStreamTLSConfig.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS13SESSION::DIOSTREAMTLS13SESSION()
* @brief      Constructor of class
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS13SESSION::DIOSTREAMTLS13SESSION() : keyexchangep256(CIPHERTYPE_ECDSA_SECP256R1), keyexchangep384(CIPHERTYPE_ECDSA_SECP384R1), keyexchangep521(CIPHERTYPE_ECDSA_SECP521R1)
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS13SESSION::~DIOSTREAMTLS13SESSION()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS13SESSION::~DIOSTREAMTLS13SESSION()
{
  End();
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13SESSION::Ini(XWORD ciphersuite, DIOSTREAMTLSKEYSCHEDULE_ROLE role)
* @brief      Initialize a role-neutral TLS 1.3 session
* @ingroup    DATAIO
*
* @param[in]  ciphersuite : TLS 1.3 cipher suite.
* @param[in]  role : Role of this end.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13SESSION::Ini(XWORD ciphersuite, DIOSTREAMTLSKEYSCHEDULE_ROLE role)
{
  End();

  if((role != DIOSTREAMTLSKEYSCHEDULE_ROLE_CLIENT) &&
     (role != DIOSTREAMTLSKEYSCHEDULE_ROLE_SERVER))
    {
      return false;
    }

  if(!keyschedule.Ini(ciphersuite, role))
    {
      return false;
    }

  if(!record.Ini(&keyschedule) || !earlyrecord.Ini(&keyschedule))
    {
      keyschedule.End();
      return false;
    }

  this->role = role;
  isini      = true;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS13SESSION::MemoryPolicy_Set(DIOSTREAMTLSMEMORYPOLICY& policy)
* @brief      Memory policy set
* @ingroup    DATAIO
* 
* @param[in]  policy : Policy value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13SESSION::MemoryPolicy_Set(DIOSTREAMTLSMEMORYPOLICY& policy)
{
  if(!recordinput.IsEmpty() || !handshakeinput.IsEmpty() || !transcript.IsEmpty() || !applicationinput.IsEmpty()) return false;
  maximumrecordinputsize      = policy.GetMaximumRecordInputSize();
  maximumhandshakeinputsize   = policy.GetMaximumHandshakeInputSize();
  maximumtranscriptsize       = policy.GetMaximumTranscriptSize();
  maximumapplicationinputsize = policy.GetMaximumApplicationInputSize();
  maximumearlydatasize = maximumapplicationinputsize>DIOSTREAMTLS13_EARLYDATA_MAXSIZE?DIOSTREAMTLS13_EARLYDATA_MAXSIZE:maximumapplicationinputsize;
  return maximumrecordinputsize && maximumhandshakeinputsize && maximumtranscriptsize && maximumapplicationinputsize;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS13SESSION::End()
* @brief      End the session and erase its transient state
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS13SESSION::End()
{
  earlyrecord.End();
  record.End();
  keyschedule.End();
  KeyExchange_Delete();

  recordinput.Delete();
  handshakeinput.Delete();
  transcript.Delete();
  applicationinput.Delete();
  earlydatainput.Delete();
  posthandshakeoutput.Delete();
  newsessionticketinput.Delete();

  for(int c=0; c<DIOSTREAMTLSKEYSCHEDULE_MAXDIRECTIONS; c++)
    {
      epoch[c]      = DIOSTREAMTLS13SESSION_EPOCH_CLEAR;
      keyupdates[c] = 0;
    }

  applicationsecretscalculated = false;
  keyupdaterequestpending      = false;
  keyupdateresponsepending     = false;
  closenotifysent              = false;
  closenotifyreceived          = false;
  iserror                      = false;
  transportclosedwithoutnotify = false;
  receivedalertlevel           = (DIOSTREAMTLS_ALERT_LEVEL)0;
  receivedalertdescription     = (DIOSTREAMTLS_ALERT_DESCRIPTION)0;
  isini                       = false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13SESSION::IsIni()
* @brief      Check whether the session is initialized
* @ingroup    DATAIO
*
* @return     bool : true if the condition is met; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13SESSION::IsIni()
{
  return isini;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLSKEYSCHEDULE_ROLE DIOSTREAMTLS13SESSION::GetRole()
* @brief      Get the role of this end of the session
* @ingroup    DATAIO
*
* @return     DIOSTREAMTLSKEYSCHEDULE_ROLE : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLSKEYSCHEDULE_ROLE DIOSTREAMTLS13SESSION::GetRole()
{
  return role;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS13SESSION_EPOCH DIOSTREAMTLS13SESSION::GetEpoch(DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction)
* @brief      Get the traffic key epoch for one direction
* @ingroup    DATAIO
*
* @param[in]  direction : Local or remote traffic direction.
*
* @return     DIOSTREAMTLS13SESSION_EPOCH : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS13SESSION_EPOCH DIOSTREAMTLS13SESSION::GetEpoch(DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction)
{
  if(direction >= DIOSTREAMTLSKEYSCHEDULE_MAXDIRECTIONS)
    {
      return DIOSTREAMTLS13SESSION_EPOCH_CLEAR;
    }

  return epoch[direction];
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS13KEYSCHEDULE* DIOSTREAMTLS13SESSION::GetKeySchedule()
* @brief      Get the key schedule
* @ingroup    DATAIO
*
* @return     DIOSTREAMTLS13KEYSCHEDULE* : Pointer to the requested object; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS13KEYSCHEDULE* DIOSTREAMTLS13SESSION::GetKeySchedule()
{
  return &keyschedule;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLSRECORD* DIOSTREAMTLS13SESSION::GetRecord()
* @brief      Get the record layer
* @ingroup    DATAIO
*
* @return     DIOSTREAMTLSRECORD* : Pointer to the requested object; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLSRECORD* DIOSTREAMTLS13SESSION::GetRecord()
{
  return &record;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         CIPHERECDSAX25519* DIOSTREAMTLS13SESSION::GetKeyExchange()
* @brief      Get the X25519 key exchange object
* @ingroup    DATAIO
*
* @return     CIPHERECDSAX25519* : Pointer to the requested object; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERECDSAX25519* DIOSTREAMTLS13SESSION::GetKeyExchange()
{
  return &keyexchange;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13SESSION::KeyExchange_Generate(XWORD group, XBUFFER& publickey)
* @brief      Generate a fresh ephemeral key pair for a supported TLS group
* @ingroup    DATAIO
*
* @param[in]  group : TLS supported group value.
* @param[out] publickey : Encoded public key share.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13SESSION::KeyExchange_Generate(XWORD group, XBUFFER& publickey)
{
  publickey.Delete();

  if(!isini) return false;

  switch(group)
    {
      #ifdef CIPHER_ASYMMETRIC_MLKEM1024_ACTIVE

      case DIOSTREAMTLS_MSG_CURVEID_SECP384R1MLKEM1024 : if(role != DIOSTREAMTLSKEYSCHEDULE_ROLE_CLIENT) return false;

                                                           keyexchangesecp384r1mlkem1024.Delete();
                                                           if(!keyexchangesecp384r1mlkem1024.ClientKeyShare_Create(publickey))
                                                             {
                                                               keyexchangesecp384r1mlkem1024.Delete();
                                                               return false;
                                                             }
                                                           break;

      #endif

      #ifdef CIPHER_ASYMMETRIC_MLKEM768_ACTIVE

      case DIOSTREAMTLS_MSG_CURVEID_SECP256R1MLKEM768 : if(role != DIOSTREAMTLSKEYSCHEDULE_ROLE_CLIENT) return false;

                                                          keyexchangesecp256r1mlkem768.Delete();
                                                          if(!keyexchangesecp256r1mlkem768.ClientKeyShare_Create(publickey))
                                                            {
                                                              keyexchangesecp256r1mlkem768.Delete();
                                                              return false;
                                                            }
                                                          break;

      #endif

      #if defined(CIPHER_ASYMMETRIC_X25519_ACTIVE) && defined(CIPHER_ASYMMETRIC_MLKEM768_ACTIVE)

      case DIOSTREAMTLS_MSG_CURVEID_X25519MLKEM768 : if(role != DIOSTREAMTLSKEYSCHEDULE_ROLE_CLIENT) return false;

                                                       keyexchangex25519mlkem768.Delete();
                                                       if(!keyexchangex25519mlkem768.ClientKeyShare_Create(publickey))
                                                         {
                                                           keyexchangex25519mlkem768.Delete();
                                                           return false;
                                                         }
                                                       break;

      #endif

      case DIOSTREAMTLS_MSG_CURVEID_X25519    : keyexchange.CleanAllKeys();

                                                if(!keyexchange.GenerateRandomPrivateKey() ||
                                                   !keyexchange.CreatePublicKey() ||
                                                   !publickey.Add(keyexchange.GetKey(CIPHERECDSAX25519_TYPEKEY_PUBLIC),
                                                                  CIPHERECDSAX25519_MAXKEY))
                                                  {
                                                    keyexchange.CleanAllKeys();
                                                    return false;
                                                  }
                                                break;

      case DIOSTREAMTLS_MSG_CURVEID_SECP256R1 : keyexchangep256private.FillBuffer(0);
                                                keyexchangep256private.SecureDelete();
                                                keyexchangep256public.Delete();

                                                if(!keyexchangep256.KeyPair_Create(keyexchangep256private,
                                                                                  keyexchangep256public) ||
                                                   !publickey.Add(keyexchangep256public))
                                                  {
                                                    keyexchangep256private.FillBuffer(0);
                                                    keyexchangep256private.SecureDelete();
                                                    keyexchangep256public.Delete();
                                                    return false;
                                                  }
                                                break;

      case DIOSTREAMTLS_MSG_CURVEID_SECP384R1 : keyexchangep384private.FillBuffer(0);
                                                keyexchangep384private.SecureDelete();
                                                keyexchangep384public.Delete();

                                                if(!keyexchangep384.KeyPair_Create(keyexchangep384private,
                                                                                  keyexchangep384public) ||
                                                   !publickey.Add(keyexchangep384public))
                                                  {
                                                    keyexchangep384private.FillBuffer(0);
                                                    keyexchangep384private.SecureDelete();
                                                    keyexchangep384public.Delete();
                                                    return false;
                                                  }
                                                break;

      case DIOSTREAMTLS_MSG_CURVEID_SECP521R1 : keyexchangep521private.FillBuffer(0);
                                                keyexchangep521private.SecureDelete();
                                                keyexchangep521public.Delete();

                                                if(!keyexchangep521.KeyPair_Create(keyexchangep521private,
                                                                                  keyexchangep521public) ||
                                                   !publickey.Add(keyexchangep521public))
                                                  {
                                                    keyexchangep521private.FillBuffer(0);
                                                    keyexchangep521private.SecureDelete();
                                                    keyexchangep521public.Delete();
                                                    return false;
                                                  }
                                                break;

                                      default : return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13SESSION::KeyExchange_SharedSecret(XWORD group, XBUFFER& publickey, XBUFFER& sharedsecret)
* @brief      Calculate the shared secret for a supported TLS group
* @ingroup    DATAIO
*
* @param[in]  group : Negotiated TLS supported group value.
* @param[in]  publickey : Encoded public key share of the remote end.
* @param[out] sharedsecret : Calculated shared secret.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13SESSION::KeyExchange_SharedSecret(XWORD group, XBUFFER& publickey, XBUFFER& sharedsecret)
{
  sharedsecret.SecureDelete();

  if(!isini) return false;

  switch(group)
    {
      #ifdef CIPHER_ASYMMETRIC_MLKEM1024_ACTIVE

      case DIOSTREAMTLS_MSG_CURVEID_SECP384R1MLKEM1024 : { bool invalidpeershare = false;

                                                           if((role != DIOSTREAMTLSKEYSCHEDULE_ROLE_CLIENT) ||
                                                              !keyexchangesecp384r1mlkem1024.ClientSharedSecret_Create(publickey, sharedsecret,
                                                                                                                          &invalidpeershare))
                                                             {
                                                               return false;
                                                             }
                                                         }
                                                         break;

      #endif

      #ifdef CIPHER_ASYMMETRIC_MLKEM768_ACTIVE

      case DIOSTREAMTLS_MSG_CURVEID_SECP256R1MLKEM768 : { bool invalidpeershare = false;

                                                          if((role != DIOSTREAMTLSKEYSCHEDULE_ROLE_CLIENT) ||
                                                             !keyexchangesecp256r1mlkem768.ClientSharedSecret_Create(publickey, sharedsecret,
                                                                                                                         &invalidpeershare))
                                                            {
                                                              return false;
                                                            }
                                                        }
                                                        break;

      #endif

      #if defined(CIPHER_ASYMMETRIC_X25519_ACTIVE) && defined(CIPHER_ASYMMETRIC_MLKEM768_ACTIVE)

      case DIOSTREAMTLS_MSG_CURVEID_X25519MLKEM768 : { bool invalidpeershare = false;

                                                       if((role != DIOSTREAMTLSKEYSCHEDULE_ROLE_CLIENT) ||
                                                          !keyexchangex25519mlkem768.ClientSharedSecret_Create(publickey, sharedsecret,
                                                                                                                 &invalidpeershare))
                                                         {
                                                           return false;
                                                         }
                                                     }
                                                     break;

      #endif

      case DIOSTREAMTLS_MSG_CURVEID_X25519    : if((publickey.GetSize() != CIPHERECDSAX25519_MAXKEY) ||
                                                   !keyexchange.GetKey(CIPHERECDSAX25519_TYPEKEY_PRIVATE) ||
                                                   !keyexchange.CreateSharedKey(publickey.Get()) ||
                                                   !sharedsecret.Add(keyexchange.GetKey(CIPHERECDSAX25519_TYPEKEY_SHARED),
                                                                     CIPHERECDSAX25519_MAXKEY))
                                                  {
                                                    return false;
                                                  }
                                                break;

      case DIOSTREAMTLS_MSG_CURVEID_SECP256R1 : if(keyexchangep256private.IsEmpty() ||
                                                   !keyexchangep256.SharedSecret_Create(keyexchangep256private,
                                                                                       publickey, sharedsecret))
                                                  {
                                                    return false;
                                                  }
                                                break;

      case DIOSTREAMTLS_MSG_CURVEID_SECP384R1 : if(keyexchangep384private.IsEmpty() ||
                                                   !keyexchangep384.SharedSecret_Create(keyexchangep384private,
                                                                                       publickey, sharedsecret))
                                                  {
                                                    return false;
                                                  }
                                                break;

      case DIOSTREAMTLS_MSG_CURVEID_SECP521R1 : if(keyexchangep521private.IsEmpty() ||
                                                   !keyexchangep521.SharedSecret_Create(keyexchangep521private,
                                                                                       publickey, sharedsecret))
                                                  {
                                                    return false;
                                                  }
                                                break;

                                      default : return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13SESSION::KeyExchange_ServerGenerate(XWORD group, XBUFFER& peerpublickey,
*                                                                     XBUFFER& publickey, XBUFFER& sharedsecret,
*                                                                     bool& invalidpeershare)
* @brief      Generate a server share and its shared secret, including KEM groups whose response depends on the client
* @ingroup    DATAIO
*
* @param[in]  group : Negotiated TLS supported group value.
* @param[in]  peerpublickey : Encoded client key share.
* @param[out] publickey : Encoded server key share.
* @param[out] sharedsecret : Calculated shared secret.
* @param[out] invalidpeershare : true when failure was caused by an invalid remote share.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13SESSION::KeyExchange_ServerGenerate(XWORD group, XBUFFER& peerpublickey, XBUFFER& publickey, XBUFFER& sharedsecret, bool& invalidpeershare)
{
  invalidpeershare = false;
  publickey.Delete();
  sharedsecret.SecureDelete();

  if(!isini || (role != DIOSTREAMTLSKEYSCHEDULE_ROLE_SERVER)) return false;

  KeyExchange_Delete();

  #ifdef CIPHER_ASYMMETRIC_MLKEM1024_ACTIVE

  if(group == DIOSTREAMTLS_MSG_CURVEID_SECP384R1MLKEM1024)
    {
      return keyexchangesecp384r1mlkem1024.ServerKeyShare_Create(peerpublickey, publickey, sharedsecret, &invalidpeershare);
    }

  #endif

  #ifdef CIPHER_ASYMMETRIC_MLKEM768_ACTIVE

  if(group == DIOSTREAMTLS_MSG_CURVEID_SECP256R1MLKEM768)
    {
      return keyexchangesecp256r1mlkem768.ServerKeyShare_Create(peerpublickey, publickey, sharedsecret, &invalidpeershare);
    }

  #endif

  #if defined(CIPHER_ASYMMETRIC_X25519_ACTIVE) && defined(CIPHER_ASYMMETRIC_MLKEM768_ACTIVE)

  if(group == DIOSTREAMTLS_MSG_CURVEID_X25519MLKEM768)
    {
      return keyexchangex25519mlkem768.ServerKeyShare_Create(peerpublickey, publickey, sharedsecret, &invalidpeershare);
    }

  #endif

  if(!KeyExchange_Generate(group, publickey))
    {
      KeyExchange_Delete();
      publickey.Delete();
      return false;
    }

  if(!KeyExchange_SharedSecret(group, peerpublickey, sharedsecret))
    {
      // The classical paths historically classify a correctly-sized share that fails point/DH validation as
      // illegal_parameter. Preserve that wire behaviour while keeping generation/allocation failures internal.
      invalidpeershare = true;
      KeyExchange_Delete();
      publickey.Delete();
      sharedsecret.SecureDelete();
      return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS13SESSION::KeyExchange_Delete()
* @brief      Erase all ephemeral key exchange material
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS13SESSION::KeyExchange_Delete()
{
  keyexchange.CleanAllKeys();

  #ifdef CIPHER_ASYMMETRIC_MLKEM768_ACTIVE
  keyexchangesecp256r1mlkem768.Delete();
  #endif

  #ifdef CIPHER_ASYMMETRIC_MLKEM1024_ACTIVE
  keyexchangesecp384r1mlkem1024.Delete();
  #endif

  #if defined(CIPHER_ASYMMETRIC_X25519_ACTIVE) && defined(CIPHER_ASYMMETRIC_MLKEM768_ACTIVE)
  keyexchangex25519mlkem768.Delete();
  #endif

  keyexchangep256private.FillBuffer(0);
  keyexchangep256private.SecureDelete();
  keyexchangep256public.Delete();

  keyexchangep384private.FillBuffer(0);
  keyexchangep384private.SecureDelete();
  keyexchangep384public.Delete();

  keyexchangep521private.FillBuffer(0);
  keyexchangep521private.SecureDelete();
  keyexchangep521public.Delete();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13SESSION::CipherSuite_Select(XWORD ciphersuite)
* @brief      Select an offered cipher suite before the handshake keys are derived
* @ingroup    DATAIO
*
* @param[in]  ciphersuite : Cipher suite selected by the remote end.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13SESSION::CipherSuite_Select(XWORD ciphersuite)
{
  if(!isini) return false;
  bool localearly=(epoch[DIOSTREAMTLSKEYSCHEDULE_DIRECTION_LOCAL]==DIOSTREAMTLS13SESSION_EPOCH_EARLY);
  bool remoteearly=(epoch[DIOSTREAMTLSKEYSCHEDULE_DIRECTION_REMOTE]==DIOSTREAMTLS13SESSION_EPOCH_EARLY);
  if((epoch[DIOSTREAMTLSKEYSCHEDULE_DIRECTION_LOCAL]!=DIOSTREAMTLS13SESSION_EPOCH_CLEAR && !localearly) ||
     (epoch[DIOSTREAMTLSKEYSCHEDULE_DIRECTION_REMOTE]!=DIOSTREAMTLS13SESSION_EPOCH_CLEAR && !remoteearly)) return false;

  if(keyschedule.GetCipherSuite() == ciphersuite) return true;

  if(localearly) epoch[DIOSTREAMTLSKEYSCHEDULE_DIRECTION_LOCAL]=DIOSTREAMTLS13SESSION_EPOCH_CLEAR;
  if(remoteearly) epoch[DIOSTREAMTLSKEYSCHEDULE_DIRECTION_REMOTE]=DIOSTREAMTLS13SESSION_EPOCH_CLEAR;
  earlyrecord.End();
  record.End();

  if(!keyschedule.Ini(ciphersuite, role) || !record.Ini(&keyschedule) || !earlyrecord.Ini(&keyschedule))
    {
      keyschedule.End();
      isini = false;
      return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XBUFFER* DIOSTREAMTLS13SESSION::GetRecordInput()
* @brief      Get the transport record accumulator
* @ingroup    DATAIO
*
* @return     XBUFFER* : Pointer to the requested buffer; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* DIOSTREAMTLS13SESSION::GetRecordInput()
{
  return &recordinput;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13SESSION::RecordInput_Add(XBYTE* data, XDWORD size)
* @brief      Add bytes to the transport record accumulator
* @ingroup    DATAIO
*
* @param[in]  data : Bytes to add.
* @param[in]  size : Number of bytes to add.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13SESSION::RecordInput_Add(XBYTE* data, XDWORD size)
{
  if(!isini || (!data && size)) return false;

  // RFC 8446 section 6.1: data received after close_notify MUST be ignored.
  if(closenotifyreceived) return true;

  if((size > maximumrecordinputsize) ||
     (recordinput.GetSize() > (maximumrecordinputsize - size)))
    {
      return false;
    }

  return size?recordinput.Add(data, size):true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13SESSION::RecordInput_Add(XBUFFER& data)
* @brief      Add a buffer to the transport record accumulator
* @ingroup    DATAIO
*
* @param[in]  data : Buffer to add.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13SESSION::RecordInput_Add(XBUFFER& data)
{
  return RecordInput_Add(data.Get(), data.GetSize());
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS13SESSION_RESULT DIOSTREAMTLS13SESSION::Record_Extract(DIOSTREAMTLS_CONTENTTYPE& contenttype, XBUFFER& plain)
* @brief      Extract and decode one record from the transport accumulator
* @ingroup    DATAIO
*
* @param[out] contenttype : Actual type of the record content.
* @param[out] plain : Decoded record content.
*
* @return     DIOSTREAMTLS13SESSION_RESULT : Complete, incomplete or error.
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS13SESSION_RESULT DIOSTREAMTLS13SESSION::Record_Extract(DIOSTREAMTLS_CONTENTTYPE& contenttype, XBUFFER& plain)
{
  lastrecordalertdescription = DIOSTREAMTLS_ALERT_DESCRIPTION_DECODE_ERROR;

  DIOSTREAMTLS_MSG_RECORDHEADER header;
  XBUFFER                       onerecord;

  plain.Delete();

  if(!isini)
    {
      return DIOSTREAMTLS13SESSION_RESULT_ERROR;
    }

  if(recordinput.GetSize() < DIOSTREAMTLS_MSG_RECORDHEADER_SIZE)
    {
      return DIOSTREAMTLS13SESSION_RESULT_INCOMPLETE;
    }

  if(!header.Peek(recordinput))
    {
      lastrecordalertdescription = DIOSTREAMTLS_ALERT_DESCRIPTION_DECODE_ERROR;
      return DIOSTREAMTLS13SESSION_RESULT_ERROR;
    }

  if(header.GetLength() > DIOSTREAMTLSRECORD_MAXCIPHERSIZE)
    {
      lastrecordalertdescription = DIOSTREAMTLS_ALERT_DESCRIPTION_RECORD_OVERFLOW;
      return DIOSTREAMTLS13SESSION_RESULT_ERROR;
    }

  if(recordinput.GetSize() < ((XDWORD)DIOSTREAMTLS_MSG_RECORDHEADER_SIZE + header.GetLength()))
    {
      return DIOSTREAMTLS13SESSION_RESULT_INCOMPLETE;
    }

  if(!DIOSTREAMTLSRECORD::Record_Extract(recordinput, onerecord))
    {
      lastrecordalertdescription = DIOSTREAMTLS_ALERT_DESCRIPTION_DECODE_ERROR;
      return DIOSTREAMTLS13SESSION_RESULT_ERROR;
    }

  bool useearly = (role==DIOSTREAMTLSKEYSCHEDULE_ROLE_SERVER &&
                   epoch[DIOSTREAMTLSKEYSCHEDULE_DIRECTION_REMOTE]==DIOSTREAMTLS13SESSION_EPOCH_EARLY &&
                   header.GetContenType()==DIOSTREAMTLS_MSG_CONTENTTYPE_APPLICATION_DATA);
  if(useearly)
    {
      if(earlyrecord.Unprotect(onerecord, contenttype, plain))
        {
          if(contenttype!=DIOSTREAMTLS_MSG_CONTENTTYPE_APPLICATION_DATA || earlydatareceived>maximumearlydatasize ||
             plain.GetSize()>(maximumearlydatasize-earlydatareceived) ||
             (earlydataaccepted && plain.GetSize() && !earlydatainput.Add(plain)))
            {
              lastrecordalertdescription=DIOSTREAMTLS_ALERT_DESCRIPTION_UNEXPECTED_MESSAGE;
              return DIOSTREAMTLS13SESSION_RESULT_ERROR;
            }
          earlydatareceived += plain.GetSize();
          return DIOSTREAMTLS13SESSION_RESULT_COMPLETE;
        }
      // The first handshake-key record (EndOfEarlyData when accepted, Finished when rejected) has the same
      // outer application_data type. Retry the exact ciphertext with handshake keys before declaring failure.
      plain.Delete();
    }
  if(!record.Unprotect(onerecord, contenttype, plain))
    {
      lastrecordalertdescription = record.GetLastAlertDescription();
      return DIOSTREAMTLS13SESSION_RESULT_ERROR;
    }
  if(useearly && !earlydataaccepted) EarlyData_End(DIOSTREAMTLSKEYSCHEDULE_DIRECTION_REMOTE);

  return DIOSTREAMTLS13SESSION_RESULT_COMPLETE;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_ALERT_DESCRIPTION DIOSTREAMTLS13SESSION::GetLastRecordAlertDescription()
* @brief      Get the TLS alert associated with the last record-layer error
* @ingroup    DATAIO
*
* @return     DIOSTREAMTLS_ALERT_DESCRIPTION : Alert description.
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_ALERT_DESCRIPTION DIOSTREAMTLS13SESSION::GetLastRecordAlertDescription()
{
  return lastrecordalertdescription;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XBUFFER* DIOSTREAMTLS13SESSION::GetHandshakeInput()
* @brief      Get the handshake message accumulator
* @ingroup    DATAIO
*
* @return     XBUFFER* : Pointer to the requested buffer; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* DIOSTREAMTLS13SESSION::GetHandshakeInput()
{
  return &handshakeinput;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13SESSION::HandshakeInput_Add(XBYTE* data, XDWORD size)
* @brief      Add bytes to the handshake message accumulator
* @ingroup    DATAIO
*
* @param[in]  data : Bytes to add.
* @param[in]  size : Number of bytes to add.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13SESSION::HandshakeInput_Add(XBYTE* data, XDWORD size)
{
  if(!isini || (!data && size) ||
     (size > maximumhandshakeinputsize) ||
     (handshakeinput.GetSize() > (maximumhandshakeinputsize - size)))
    {
      return false;
    }

  return size?handshakeinput.Add(data, size):true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13SESSION::HandshakeInput_Add(XBUFFER& data)
* @brief      Add a buffer to the handshake message accumulator
* @ingroup    DATAIO
*
* @param[in]  data : Buffer to add.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13SESSION::HandshakeInput_Add(XBUFFER& data)
{
  return HandshakeInput_Add(data.Get(), data.GetSize());
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS13SESSION_RESULT DIOSTREAMTLS13SESSION::Handshake_Extract(XBUFFER& message)
* @brief      Extract one complete handshake message from the handshake accumulator
* @ingroup    DATAIO
*
* @param[out] message : Complete handshake message, including its header.
*
* @return     DIOSTREAMTLS13SESSION_RESULT : Complete, incomplete or error.
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS13SESSION_RESULT DIOSTREAMTLS13SESSION::Handshake_Extract(XBUFFER& message)
{
  message.Delete();

  if(!isini)
    {
      return DIOSTREAMTLS13SESSION_RESULT_ERROR;
    }

  if(handshakeinput.GetSize() < DIOSTREAMTLS_MSG_HANDSHAKEHEADER_SIZE)
    {
      return DIOSTREAMTLS13SESSION_RESULT_INCOMPLETE;
    }

  XBYTE* data = handshakeinput.Get();
  if(!data)
    {
      return DIOSTREAMTLS13SESSION_RESULT_ERROR;
    }

  XDWORD length        = ((XDWORD)data[1] << 16) | ((XDWORD)data[2] << 8) | (XDWORD)data[3];
  XDWORD messagelength = DIOSTREAMTLS_MSG_HANDSHAKEHEADER_SIZE + length;

  if(messagelength > maximumhandshakeinputsize)
    {
      return DIOSTREAMTLS13SESSION_RESULT_ERROR;
    }

  if(handshakeinput.GetSize() < messagelength)
    {
      return DIOSTREAMTLS13SESSION_RESULT_INCOMPLETE;
    }

  if(!DIOSTREAMTLS_MSG_HANDSHAKE::Message_Extract(handshakeinput, message))
    {
      return DIOSTREAMTLS13SESSION_RESULT_ERROR;
    }

  return DIOSTREAMTLS13SESSION_RESULT_COMPLETE;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XBUFFER* DIOSTREAMTLS13SESSION::GetTranscript()
* @brief      Get the exact handshake transcript
* @ingroup    DATAIO
*
* @return     XBUFFER* : Pointer to the requested buffer; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* DIOSTREAMTLS13SESSION::GetTranscript()
{
  return &transcript;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13SESSION::Transcript_Add(XBUFFER& message)
* @brief      Add one exact handshake message to the transcript
* @ingroup    DATAIO
*
* @param[in]  message : Complete handshake message, including its header.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13SESSION::Transcript_Add(XBUFFER& message)
{
  XBUFFER                   workbuffer;
  DIOSTREAMTLS_MSG_HANDSHAKE handshake;

  if(!isini || message.IsEmpty())
    {
      return false;
    }

  workbuffer.Add(message);

  if(!handshake.GetFromBuffer(workbuffer, false) || !workbuffer.IsEmpty())
    {
      return false;
    }

  if(message.GetSize() > maximumtranscriptsize ||
     transcript.GetSize() > (maximumtranscriptsize-message.GetSize())) return false;
  return transcript.Add(message);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13SESSION::TranscriptHash(XBUFFER& transcripthash)
* @brief      Calculate the current handshake transcript hash
* @ingroup    DATAIO
*
* @param[out] transcripthash : Calculated transcript hash.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13SESSION::TranscriptHash(XBUFFER& transcripthash)
{
  if(!isini)
    {
      return false;
    }

  return keyschedule.TranscriptHash(transcript, transcripthash);
}


/**-------------------------------------------------------------------------------------------------------------------
* @fn         bool DIOSTREAMTLS13SESSION::EarlyKeys_Activate(XBUFFER& PSK, XBUFFER& clienthello, DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction)
* @brief      Install TLS 1.3 client early traffic keys in the requested direction
* @ingroup    DATAIO
*
* @param[in]  PSK : Pre-shared key used to derive the early secret.
* @param[in]  clienthello : ClientHello message covered by the early traffic secret transcript hash.
* @param[in]  direction : Direction (local/remote) the early keys are activated for.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13SESSION::EarlyKeys_Activate(XBUFFER& PSK, XBUFFER& clienthello, DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction)
{
  XBUFFER hash;
  if(!isini || PSK.IsEmpty() || clienthello.IsEmpty() || direction>=DIOSTREAMTLSKEYSCHEDULE_MAXDIRECTIONS) return false;
  if(direction==DIOSTREAMTLSKEYSCHEDULE_DIRECTION_LOCAL && role!=DIOSTREAMTLSKEYSCHEDULE_ROLE_CLIENT) return false;
  if(direction==DIOSTREAMTLSKEYSCHEDULE_DIRECTION_REMOTE && role!=DIOSTREAMTLSKEYSCHEDULE_ROLE_SERVER) return false;
  if(epoch[direction]!=DIOSTREAMTLS13SESSION_EPOCH_CLEAR) return false;
  if(!keyschedule.EarlySecret_Calculate(&PSK) || !keyschedule.TranscriptHash(clienthello, hash) ||
     !keyschedule.EarlyTrafficSecret_Calculate(hash) || !earlyrecord.SetKeys(DIOSTREAMTLS13KEYSCHEDULE_LEVEL_EARLY, direction)) return false;
  epoch[direction]=DIOSTREAMTLS13SESSION_EPOCH_EARLY;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS13SESSION::EarlyData_Protect(XBYTE* data, XDWORD size, XBUFFER& records)
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
bool DIOSTREAMTLS13SESSION::EarlyData_Protect(XBYTE* data, XDWORD size, XBUFFER& records)
{
  if(!isini || role!=DIOSTREAMTLSKEYSCHEDULE_ROLE_CLIENT || epoch[DIOSTREAMTLSKEYSCHEDULE_DIRECTION_LOCAL]!=DIOSTREAMTLS13SESSION_EPOCH_EARLY ||
     (!data && size) || !size || earlydatasent>maximumearlydatasize || size>(maximumearlydatasize-earlydatasent)) return false;
  if(!earlyrecord.Protect(DIOSTREAMTLS_MSG_CONTENTTYPE_APPLICATION_DATA, data, size, records)) return false;
  earlydatasent += size;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS13SESSION::EarlyData_Protect(XBUFFER& data, XBUFFER& records)
* @brief      Early data protect
* @ingroup    DATAIO
* 
* @param[in]  data : Data value.
* @param[in]  records : Records value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13SESSION::EarlyData_Protect(XBUFFER& data, XBUFFER& records) { return EarlyData_Protect(data.Get(), data.GetSize(), records); }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOSTREAMTLS13SESSION::EarlyData_Read(XBYTE* data, XDWORD size)
* @brief      Early data read
* @ingroup    DATAIO
* 
* @param[in]  data : Pointer to data.
* @param[in]  size : Size value.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAMTLS13SESSION::EarlyData_Read(XBYTE* data, XDWORD size)
{
  if(!data || !size) return 0;
  if(size>earlydatainput.GetSize()) size=earlydatainput.GetSize();
  return earlydatainput.Extract(data, 0, size);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOSTREAMTLS13SESSION::GetEarlyDataSize()
* @brief      Get early data size
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAMTLS13SESSION::GetEarlyDataSize() { return earlydatainput.GetSize(); }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS13SESSION::EarlyData_Commit()
* @brief      Early data commit
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13SESSION::EarlyData_Commit()
{
  if(!isini || role != DIOSTREAMTLSKEYSCHEDULE_ROLE_SERVER || !earlydataaccepted) return false;
  if(earlydatainput.IsEmpty()) return true;
  if(earlydatainput.GetSize() > maximumapplicationinputsize ||
     applicationinput.GetSize() > (maximumapplicationinputsize-earlydatainput.GetSize())) return false;
  if(!applicationinput.Add(earlydatainput)) return false;
  earlydatainput.Delete();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTLS13SESSION::EarlyData_End(DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction)
* @brief      Early data end
* @ingroup    DATAIO
* 
* @param[in]  direction : Direction value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS13SESSION::EarlyData_End(DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction)
{
  if(direction<DIOSTREAMTLSKEYSCHEDULE_MAXDIRECTIONS && epoch[direction]==DIOSTREAMTLS13SESSION_EPOCH_EARLY)
    {
      earlyrecord.ClearKeys(direction);
      epoch[direction]=DIOSTREAMTLS13SESSION_EPOCH_HANDSHAKE;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTLS13SESSION::EarlyKeys_Deactivate(DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction)
* @brief      Early keys deactivate
* @ingroup    DATAIO
* 
* @param[in]  direction : Direction value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS13SESSION::EarlyKeys_Deactivate(DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction)
{
  if(direction<DIOSTREAMTLSKEYSCHEDULE_MAXDIRECTIONS && epoch[direction]==DIOSTREAMTLS13SESSION_EPOCH_EARLY)
    {
      earlyrecord.ClearKeys(direction);
      epoch[direction]=DIOSTREAMTLS13SESSION_EPOCH_CLEAR;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTLS13SESSION::EarlyData_Accepted(bool accepted)
* @brief      Early data accepted
* @ingroup    DATAIO
* 
* @param[in]  accepted : Accepted value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS13SESSION::EarlyData_Accepted(bool accepted) { earlydataaccepted=accepted; }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS13SESSION::EarlyData_Limit(XDWORD maximumsize)
* @brief      Early data limit
* @ingroup    DATAIO
* 
* @param[in]  maximumsize : Maximumsize value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13SESSION::EarlyData_Limit(XDWORD maximumsize)
{
  if(!maximumsize || maximumsize>DIOSTREAMTLS13_EARLYDATA_MAXSIZE || maximumsize>maximumapplicationinputsize || earlydatareceived) return false;
  maximumearlydatasize=maximumsize;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* @fn         bool DIOSTREAMTLS13SESSION::HandshakeKeys_Activate(XBUFFER& sharedsecret, XBUFFER* PSK)
* @brief      Derive and install both handshake traffic directions
* @ingroup    DATAIO
*
* @param[in]  sharedsecret : (EC)DHE shared secret used to derive the handshake secret.
* @param[in]  PSK : Pointer to the pre-shared key when resuming; NULL for a full handshake.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13SESSION::HandshakeKeys_Activate(XBUFFER& sharedsecret, XBUFFER* PSK)
{
  XBUFFER transcripthash;

  // Shared-secret size is group-dependent: X25519 and secp256r1 both happen to produce 32 bytes, secp384r1
  // produces 48 (its coordinate size) -- so this must enumerate every group KeyExchange_SharedSecret() can
  // hand back here, not assume the X25519 constant fits them all.
  bool sharedsecretsizevalid = (sharedsecret.GetSize() == CIPHERECDSAX25519_MAXKEY) ||
                               (sharedsecret.GetSize() == CIPHERECDSA_P256_COORDINATE_SIZE) ||
                               (sharedsecret.GetSize() == CIPHERECDSA_P384_COORDINATE_SIZE) ||
                               (sharedsecret.GetSize() == CIPHERECDSA_P521_COORDINATE_SIZE);

  #ifdef CIPHER_ASYMMETRIC_MLKEM768_ACTIVE
  sharedsecretsizevalid = sharedsecretsizevalid ||
                          (sharedsecret.GetSize() == CIPHERSECP256R1MLKEM768_SHAREDSECRETSIZE);
  #endif

  #ifdef CIPHER_ASYMMETRIC_MLKEM1024_ACTIVE
  sharedsecretsizevalid = sharedsecretsizevalid ||
                          (sharedsecret.GetSize() == CIPHERSECP384R1MLKEM1024_SHAREDSECRETSIZE);
  #endif

  #if defined(CIPHER_ASYMMETRIC_X25519_ACTIVE) && defined(CIPHER_ASYMMETRIC_MLKEM768_ACTIVE)
  sharedsecretsizevalid = sharedsecretsizevalid ||
                          (sharedsecret.GetSize() == CIPHERX25519MLKEM768_SHAREDSECRETSIZE);
  #endif

  if(!isini || !sharedsecretsizevalid)
    {
      return false;
    }

  if((epoch[DIOSTREAMTLSKEYSCHEDULE_DIRECTION_LOCAL]  != DIOSTREAMTLS13SESSION_EPOCH_CLEAR && epoch[DIOSTREAMTLSKEYSCHEDULE_DIRECTION_LOCAL] != DIOSTREAMTLS13SESSION_EPOCH_EARLY) ||
     (epoch[DIOSTREAMTLSKEYSCHEDULE_DIRECTION_REMOTE] != DIOSTREAMTLS13SESSION_EPOCH_CLEAR && epoch[DIOSTREAMTLSKEYSCHEDULE_DIRECTION_REMOTE] != DIOSTREAMTLS13SESSION_EPOCH_EARLY))
    {
      return false;
    }

  if(!keyschedule.EarlySecret_Calculate(PSK))                      return false;
  if(!keyschedule.HandshakeSecret_Calculate(sharedsecret))         return false;
  if(!TranscriptHash(transcripthash))                              return false;
  if(!keyschedule.HandshakeTrafficSecrets_Calculate(transcripthash)) return false;
  if(!keyschedule.MasterSecret_Calculate())                        return false;

  bool localearly  = (epoch[DIOSTREAMTLSKEYSCHEDULE_DIRECTION_LOCAL]  == DIOSTREAMTLS13SESSION_EPOCH_EARLY);
  bool remoteearly = (epoch[DIOSTREAMTLSKEYSCHEDULE_DIRECTION_REMOTE] == DIOSTREAMTLS13SESSION_EPOCH_EARLY);

  if(!record.SetKeys(DIOSTREAMTLS13KEYSCHEDULE_LEVEL_HANDSHAKE, DIOSTREAMTLSKEYSCHEDULE_DIRECTION_LOCAL))  return false;
  if(!record.SetKeys(DIOSTREAMTLS13KEYSCHEDULE_LEVEL_HANDSHAKE, DIOSTREAMTLSKEYSCHEDULE_DIRECTION_REMOTE)) return false;

  epoch[DIOSTREAMTLSKEYSCHEDULE_DIRECTION_LOCAL]  = localearly ?DIOSTREAMTLS13SESSION_EPOCH_EARLY:DIOSTREAMTLS13SESSION_EPOCH_HANDSHAKE;
  epoch[DIOSTREAMTLSKEYSCHEDULE_DIRECTION_REMOTE] = remoteearly?DIOSTREAMTLS13SESSION_EPOCH_EARLY:DIOSTREAMTLS13SESSION_EPOCH_HANDSHAKE;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13SESSION::ApplicationTrafficSecrets_Calculate()
* @brief      Calculate both application traffic secrets from the current transcript
* @ingroup    DATAIO
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13SESSION::ApplicationTrafficSecrets_Calculate()
{
  XBUFFER transcripthash;

  if(!isini)
    {
      return false;
    }

  if(applicationsecretscalculated)
    {
      return true;
    }

  if((epoch[DIOSTREAMTLSKEYSCHEDULE_DIRECTION_LOCAL]  != DIOSTREAMTLS13SESSION_EPOCH_HANDSHAKE) ||
     (epoch[DIOSTREAMTLSKEYSCHEDULE_DIRECTION_REMOTE] != DIOSTREAMTLS13SESSION_EPOCH_HANDSHAKE))
    {
      return false;
    }

  if(!TranscriptHash(transcripthash)) return false;
  if(!keyschedule.ApplicationTrafficSecrets_Calculate(transcripthash)) return false;

  applicationsecretscalculated = true;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13SESSION::ApplicationKeys_Activate(DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction)
* @brief      Install the application traffic keys for one direction
* @ingroup    DATAIO
*
* @param[in]  direction : Local or remote traffic direction.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13SESSION::ApplicationKeys_Activate(DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction)
{
  if(!isini || (direction >= DIOSTREAMTLSKEYSCHEDULE_MAXDIRECTIONS) ||
     (epoch[direction] != DIOSTREAMTLS13SESSION_EPOCH_HANDSHAKE))
    {
      return false;
    }

  if(!ApplicationTrafficSecrets_Calculate())
    {
      return false;
    }

  if(!record.SetKeys(DIOSTREAMTLS13KEYSCHEDULE_LEVEL_APPLICATION, direction))
    {
      return false;
    }

  epoch[direction] = DIOSTREAMTLS13SESSION_EPOCH_APPLICATION;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XBUFFER* DIOSTREAMTLS13SESSION::GetApplicationInput()
* @brief      Get the decoded application data accumulator
* @ingroup    DATAIO
*
* @return     XBUFFER* : Pointer to the requested object; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* DIOSTREAMTLS13SESSION::GetApplicationInput()
{
  return &applicationinput;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13SESSION::ApplicationData_Protect(XBYTE* data, XDWORD size, XBUFFER& records)
* @brief      Protect application data with the local application traffic keys
* @ingroup    DATAIO
*
* @param[in]  data : Application data pointer.
* @param[in]  size : Number of application bytes.
* @param[out] records : Buffer where the protected records are added.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13SESSION::ApplicationData_Protect(XBYTE* data, XDWORD size, XBUFFER& records)
{
  if(!isini || (!data && size) || iserror || closenotifysent || closenotifyreceived ||
     (epoch[DIOSTREAMTLSKEYSCHEDULE_DIRECTION_LOCAL] != DIOSTREAMTLS13SESSION_EPOCH_APPLICATION))
    {
      return false;
    }

  XQWORD recordsneeded = size?(((XQWORD)size + record.GetMaxPlainSize() - 1) / record.GetMaxPlainSize()):1;
  XQWORD sequence      = record.GetSequence(DIOSTREAMTLSKEYSCHEDULE_DIRECTION_LOCAL);
  XQWORD usagelimit    = (keyschedule.GetCipherSuite() == DIOSTREAMTLS_MSG_CIPHER_CHACHA20_POLY1305_SHA256)?
                          DIOSTREAMTLS_CHACHA20POLY1305_PROACTIVEKEYUSAGERECORDS:
                          DIOSTREAMTLS_AESGCM_PROACTIVEKEYUSAGERECORDS;

  if((sequence >= usagelimit) || (recordsneeded >= usagelimit) || (recordsneeded >= (usagelimit-sequence)))
    {
      XBUFFER keyupdaterecords;

      if(!KeyUpdate_Create(false, keyupdaterecords) || !records.Add(keyupdaterecords))
        {
          iserror = true;
          return false;
        }
    }

  return record.Protect(DIOSTREAMTLS_MSG_CONTENTTYPE_APPLICATION_DATA, data, size, records);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13SESSION::ApplicationData_Protect(XBUFFER& data, XBUFFER& records)
* @brief      Protect an application buffer with the local application traffic keys
* @ingroup    DATAIO
*
* @param[in]  data : Application data.
* @param[out] records : Buffer where the protected records are added.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13SESSION::ApplicationData_Protect(XBUFFER& data, XBUFFER& records)
{
  return ApplicationData_Protect(data.Get(), data.GetSize(), records);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD DIOSTREAMTLS13SESSION::ApplicationData_Read(XBYTE* data, XDWORD size)
* @brief      Read decoded application data
* @ingroup    DATAIO
*
* @param[out] data : Destination buffer.
* @param[in]  size : Maximum number of bytes to read.
*
* @return     XDWORD : Number of bytes read.
*
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAMTLS13SESSION::ApplicationData_Read(XBYTE* data, XDWORD size)
{
  if(!data || !size) return 0;

  return applicationinput.Extract(data, 0, size);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS13SESSION_RESULT DIOSTREAMTLS13SESSION::ApplicationData_Process()
* @brief      Process all complete records received after the handshake
* @note       NewSessionTicket is consumed, KeyUpdate is processed and other post-handshake messages are rejected.
* @ingroup    DATAIO
*
* @return     DIOSTREAMTLS13SESSION_RESULT : Complete, incomplete or error.
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS13SESSION_RESULT DIOSTREAMTLS13SESSION::ApplicationData_Process()
{
  bool processed = false;

  if(!isini || iserror ||
     (epoch[DIOSTREAMTLSKEYSCHEDULE_DIRECTION_REMOTE] != DIOSTREAMTLS13SESSION_EPOCH_APPLICATION))
    {
      return DIOSTREAMTLS13SESSION_RESULT_ERROR;
    }

  while(true)
    {
      // RFC 8446 section 6.1: once close_notify is received, any bytes that
      // follow it on the peer write side are ignored rather than processed.
      if(closenotifyreceived)
        {
          recordinput.Delete();
          return processed?DIOSTREAMTLS13SESSION_RESULT_COMPLETE:DIOSTREAMTLS13SESSION_RESULT_INCOMPLETE;
        }

      DIOSTREAMTLS_CONTENTTYPE   contenttype = (DIOSTREAMTLS_CONTENTTYPE)0;
      DIOSTREAMTLS13SESSION_RESULT result;
      XBUFFER                    plain;

      result = Record_Extract(contenttype, plain);

      if(result == DIOSTREAMTLS13SESSION_RESULT_INCOMPLETE)
        {
          return processed?DIOSTREAMTLS13SESSION_RESULT_COMPLETE:DIOSTREAMTLS13SESSION_RESULT_INCOMPLETE;
        }

      if(result == DIOSTREAMTLS13SESSION_RESULT_ERROR)
        {
          iserror = true;
          return DIOSTREAMTLS13SESSION_RESULT_ERROR;
        }

      processed = true;

      switch(contenttype)
        {
          case DIOSTREAMTLS_MSG_CONTENTTYPE_APPLICATION_DATA    : if((plain.GetSize() > maximumapplicationinputsize) ||
                                                                     (applicationinput.GetSize() > (maximumapplicationinputsize-plain.GetSize())))
                                                                    {
                                                                      lastrecordalertdescription = DIOSTREAMTLS_ALERT_DESCRIPTION_INTERNAL_ERROR;
                                                                      iserror                    = true;
                                                                      return DIOSTREAMTLS13SESSION_RESULT_ERROR;
                                                                    }

                                                                  if(plain.GetSize() && !applicationinput.Add(plain))
                                                                    {
                                                                      lastrecordalertdescription = DIOSTREAMTLS_ALERT_DESCRIPTION_INTERNAL_ERROR;
                                                                      iserror                    = true;
                                                                      return DIOSTREAMTLS13SESSION_RESULT_ERROR;
                                                                    }
                                                                  break;

          case DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE           : { if(!HandshakeInput_Add(plain))
                                                                      {
                                                                        iserror = true;
                                                                        return DIOSTREAMTLS13SESSION_RESULT_ERROR;
                                                                      }

                                                                    while(true)
                                                                      {
                                                                        DIOSTREAMTLS13SESSION_RESULT handshakeresult;
                                                                        DIOSTREAMTLS_MSG_HANDSHAKE handshake;
                                                                        XBUFFER                    message;
                                                                        XBUFFER                    workbuffer;

                                                                        handshakeresult = Handshake_Extract(message);
                                                                        if(handshakeresult == DIOSTREAMTLS13SESSION_RESULT_INCOMPLETE) break;

                                                                        if(handshakeresult == DIOSTREAMTLS13SESSION_RESULT_ERROR)
                                                                          {
                                                                            lastrecordalertdescription = DIOSTREAMTLS_ALERT_DESCRIPTION_DECODE_ERROR;
                                                                            iserror                    = true;
                                                                            return DIOSTREAMTLS13SESSION_RESULT_ERROR;
                                                                          }

                                                                        workbuffer.Add(message);
                                                                        if(!handshake.GetFromBuffer(workbuffer, false) || !workbuffer.IsEmpty())
                                                                          {
                                                                            lastrecordalertdescription = DIOSTREAMTLS_ALERT_DESCRIPTION_DECODE_ERROR;
                                                                            iserror                    = true;
                                                                            return DIOSTREAMTLS13SESSION_RESULT_ERROR;
                                                                          }

                                                                        switch(handshake.GetMsgType())
                                                                          {
                                                                            case DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_NEW_SESSION_TICKET : if(role != DIOSTREAMTLSKEYSCHEDULE_ROLE_CLIENT)
                                                                                                                                          {
                                                                                                                                            lastrecordalertdescription = DIOSTREAMTLS_ALERT_DESCRIPTION_UNEXPECTED_MESSAGE;
                                                                                                                                            iserror                    = true;
                                                                                                                                            return DIOSTREAMTLS13SESSION_RESULT_ERROR;
                                                                                                                                          }

                                                                                                                                        if((message.GetSize() > maximumhandshakeinputsize) ||
                                                                                                                                           (newsessionticketinput.GetSize() > (maximumhandshakeinputsize-message.GetSize())))
                                                                                                                                          {
                                                                                                                                            lastrecordalertdescription = DIOSTREAMTLS_ALERT_DESCRIPTION_INTERNAL_ERROR;
                                                                                                                                            iserror                    = true;
                                                                                                                                            return DIOSTREAMTLS13SESSION_RESULT_ERROR;
                                                                                                                                          }

                                                                                                                                        if(!newsessionticketinput.Add(message))
                                                                                                                                          {
                                                                                                                                            lastrecordalertdescription = DIOSTREAMTLS_ALERT_DESCRIPTION_INTERNAL_ERROR;
                                                                                                                                            iserror                    = true;
                                                                                                                                            return DIOSTREAMTLS13SESSION_RESULT_ERROR;
                                                                                                                                          }
                                                                                                                                        break;

                                                                            case DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_KEY_UPDATE         : if(!KeyUpdate_Process(handshake))
                                                                                                                                              {
                                                                                                                                                iserror = true;
                                                                                                                                                return DIOSTREAMTLS13SESSION_RESULT_ERROR;
                                                                                                                                              }

                                                                                                                                            if(!handshakeinput.IsEmpty())
                                                                                                                                              {
                                                                                                                                                lastrecordalertdescription = DIOSTREAMTLS_ALERT_DESCRIPTION_UNEXPECTED_MESSAGE;
                                                                                                                                                iserror                    = true;
                                                                                                                                                return DIOSTREAMTLS13SESSION_RESULT_ERROR;
                                                                                                                                              }
                                                                                                                                            break;

                                                                                                                                  default : lastrecordalertdescription = DIOSTREAMTLS_ALERT_DESCRIPTION_UNEXPECTED_MESSAGE;
                                                                                                                                            iserror                    = true;
                                                                                                                                            return DIOSTREAMTLS13SESSION_RESULT_ERROR;
                                                                          }
                                                                      }
                                                                  }
                                                                  break;

          case DIOSTREAMTLS_MSG_CONTENTTYPE_ALERT               : { DIOSTREAMTLS_MSG_ALERT alert;

                                                                    if(!alert.GetFromBuffer(plain, false) || !plain.IsEmpty())
                                                                      {
                                                                        lastrecordalertdescription = DIOSTREAMTLS_ALERT_DESCRIPTION_DECODE_ERROR;
                                                                        iserror                    = true;
                                                                        return DIOSTREAMTLS13SESSION_RESULT_ERROR;
                                                                      }

                                                                    DIOSTREAMTLS_ALERT_LEVEL       alertlevel       = alert.GetLevel();
                                                                    DIOSTREAMTLS_ALERT_DESCRIPTION alertdescription = alert.GetDescription();

                                                                    // RFC 8446 section 6: close_notify and user_canceled use
                                                                    // warning.  Every error alert is fatal.
                                                                    bool warningallowed = (alertdescription == DIOSTREAMTLS_ALERT_DESCRIPTION_CLOSE_NOTIFY) ||
                                                                                          (alertdescription == DIOSTREAMTLS_ALERT_DESCRIPTION_USER_CANCELED);

                                                                    if((warningallowed && (alertlevel != DIOSTREAMTLS_ALERT_LEVEL_WARNING)) ||
                                                                       (!warningallowed && (alertlevel != DIOSTREAMTLS_ALERT_LEVEL_FATAL)))
                                                                      {
                                                                        lastrecordalertdescription = DIOSTREAMTLS_ALERT_DESCRIPTION_ILLEGAL_PARAMETER;
                                                                        iserror                    = true;
                                                                        return DIOSTREAMTLS13SESSION_RESULT_ERROR;
                                                                      }

                                                                    receivedalertlevel       = alertlevel;
                                                                    receivedalertdescription = alertdescription;

                                                                    if(alertdescription == DIOSTREAMTLS_ALERT_DESCRIPTION_CLOSE_NOTIFY)
                                                                      {
                                                                        if(!handshakeinput.IsEmpty())
                                                                          {
                                                                            lastrecordalertdescription = DIOSTREAMTLS_ALERT_DESCRIPTION_DECODE_ERROR;
                                                                            iserror                    = true;
                                                                            return DIOSTREAMTLS13SESSION_RESULT_ERROR;
                                                                          }

                                                                        closenotifyreceived = true;
                                                                      }
                                                                     else if(alertdescription == DIOSTREAMTLS_ALERT_DESCRIPTION_USER_CANCELED)
                                                                      {
                                                                        // Advisory warning. RFC 8446 recommends following it with close_notify.
                                                                      }
                                                                     else
                                                                      {
                                                                        // A valid fatal alert from the peer terminates the connection.
                                                                        // The transport wrapper must not answer it with another fatal alert.
                                                                        iserror = true;
                                                                        return DIOSTREAMTLS13SESSION_RESULT_ERROR;
                                                                      }
                                                                  }
                                                                  break;

          case DIOSTREAMTLS_MSG_CONTENTTYPE_CHANGE_CIPHER_SPEC  : lastrecordalertdescription = DIOSTREAMTLS_ALERT_DESCRIPTION_UNEXPECTED_MESSAGE;
                                                                  iserror                    = true;
                                                                  return DIOSTREAMTLS13SESSION_RESULT_ERROR;

                                                        default : lastrecordalertdescription = DIOSTREAMTLS_ALERT_DESCRIPTION_UNEXPECTED_MESSAGE;
                                                                  iserror                    = true;
                                                                  return DIOSTREAMTLS13SESSION_RESULT_ERROR;
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13SESSION::KeyUpdate_Create(bool requestpeer, XBUFFER& records)
* @brief      Create a protected TLS 1.3 KeyUpdate and advance the local application traffic keys
* @ingroup    DATAIO
*
* @param[in]  requestpeer : Request the remote end to update its sending keys too.
* @param[out] records : Protected KeyUpdate records.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13SESSION::KeyUpdate_Create(bool requestpeer, XBUFFER& records)
{
  DIOSTREAMTLS_MSG_HANDSHAKE keyupdate;
  XBUFFER                    message;

  records.Delete();

  if(!isini || iserror || closenotifysent || closenotifyreceived ||
     (epoch[DIOSTREAMTLSKEYSCHEDULE_DIRECTION_LOCAL] != DIOSTREAMTLS13SESSION_EPOCH_APPLICATION) ||
     (keyupdates[DIOSTREAMTLSKEYSCHEDULE_DIRECTION_LOCAL] >= DIOSTREAMTLS13SESSION_MAXKEYUPDATES) ||
     (requestpeer && keyupdaterequestpending))
    {
      return false;
    }

  keyupdate.SetMsgType(DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_KEY_UPDATE);

  if(!keyupdate.GetBody()->Add((XBYTE)(requestpeer?1:0)) ||
     !keyupdate.SetToBuffer(message, false) ||
     !record.Protect(DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE, message, records) ||
     !keyschedule.UpdateTrafficSecret(DIOSTREAMTLSKEYSCHEDULE_DIRECTION_LOCAL) ||
     !record.SetKeys(DIOSTREAMTLS13KEYSCHEDULE_LEVEL_APPLICATION, DIOSTREAMTLSKEYSCHEDULE_DIRECTION_LOCAL))
    {
      iserror = true;
      records.Delete();
      return false;
    }

  keyupdates[DIOSTREAMTLSKEYSCHEDULE_DIRECTION_LOCAL]++;

  if(requestpeer) keyupdaterequestpending = true;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13SESSION::PostHandshakeOutput_Add(XBUFFER& records)
* @brief      Queue protected records generated while processing post-handshake messages
* @ingroup    DATAIO
*
* @param[in]  records : Pending records to append.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13SESSION::PostHandshakeOutput_Add(XBUFFER& records)
{
  if(!isini || records.IsEmpty()) return false;
  return posthandshakeoutput.Add(records);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS13SESSION::NewSessionTicket_Extract(XBUFFER& message)
* @brief      New session ticket extract
* @ingroup    DATAIO
* 
* @param[in]  message : Message value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13SESSION::NewSessionTicket_Extract(XBUFFER& message)
{
  message.Delete();
  if(!isini || newsessionticketinput.IsEmpty()) return true;
  return DIOSTREAMTLS_MSG_HANDSHAKE::Message_Extract(newsessionticketinput, message);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTLS13SESSION::PostHandshakeOutput_Extract(XBUFFER& records)
* @brief      Post handshake output extract
* @ingroup    DATAIO
* 
* @param[out] records : Records value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13SESSION::PostHandshakeOutput_Extract(XBUFFER& records)
{
  records.Delete();

  if(!isini) return false;

  if(keyupdateresponsepending)
    {
      XBUFFER keyupdaterecords;

      if(keyupdates[DIOSTREAMTLSKEYSCHEDULE_DIRECTION_LOCAL] >= DIOSTREAMTLS13SESSION_MAXKEYUPDATES)
        {
          lastrecordalertdescription = DIOSTREAMTLS_ALERT_DESCRIPTION_INTERNAL_ERROR;
          iserror                    = true;
          return false;
        }

      if(!KeyUpdate_Create(false, keyupdaterecords) || !posthandshakeoutput.Add(keyupdaterecords))
        {
          lastrecordalertdescription = DIOSTREAMTLS_ALERT_DESCRIPTION_INTERNAL_ERROR;
          iserror                    = true;
          return false;
        }

      keyupdateresponsepending = false;
    }

  if(posthandshakeoutput.IsEmpty()) return true;

  if(!records.Add(posthandshakeoutput)) return false;

  posthandshakeoutput.Delete();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13SESSION::KeyUpdate_Process(DIOSTREAMTLS_MSG_HANDSHAKE& handshake)
* @brief      Validate a remote KeyUpdate, advance the reading keys and prepare an optional response
* @note       INTERNAL
* @ingroup    DATAIO
*
* @param[in]  handshake : Decoded KeyUpdate message.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13SESSION::KeyUpdate_Process(DIOSTREAMTLS_MSG_HANDSHAKE& handshake)
{
  XBYTE requestupdate;

  if(handshake.GetMsgType() != DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_KEY_UPDATE)
    {
      lastrecordalertdescription = DIOSTREAMTLS_ALERT_DESCRIPTION_UNEXPECTED_MESSAGE;
      return false;
    }

  if(handshake.GetBody()->GetSize() != 1)
    {
      lastrecordalertdescription = DIOSTREAMTLS_ALERT_DESCRIPTION_DECODE_ERROR;
      return false;
    }

  requestupdate = handshake.GetBody()->GetByte(0);
  if(requestupdate > 1)
    {
      lastrecordalertdescription = DIOSTREAMTLS_ALERT_DESCRIPTION_ILLEGAL_PARAMETER;
      return false;
    }

  if(keyupdates[DIOSTREAMTLSKEYSCHEDULE_DIRECTION_REMOTE] >= DIOSTREAMTLS13SESSION_MAXKEYUPDATES)
    {
      lastrecordalertdescription = DIOSTREAMTLS_ALERT_DESCRIPTION_UNEXPECTED_MESSAGE;
      return false;
    }

  if(!keyschedule.UpdateTrafficSecret(DIOSTREAMTLSKEYSCHEDULE_DIRECTION_REMOTE) ||
     !record.SetKeys(DIOSTREAMTLS13KEYSCHEDULE_LEVEL_APPLICATION, DIOSTREAMTLSKEYSCHEDULE_DIRECTION_REMOTE))
    {
      lastrecordalertdescription = DIOSTREAMTLS_ALERT_DESCRIPTION_INTERNAL_ERROR;
      return false;
    }

  keyupdates[DIOSTREAMTLSKEYSCHEDULE_DIRECTION_REMOTE]++;

  keyupdaterequestpending = false;

  if(requestupdate) keyupdateresponsepending = true;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13SESSION::Alert_Create(DIOSTREAMTLS_ALERT_LEVEL level, DIOSTREAMTLS_ALERT_DESCRIPTION description, XBUFFER& records)
* @brief      Create a TLS alert protected with the current local traffic keys
* @ingroup    DATAIO
*
* @param[in]  level : Alert level.
* @param[in]  description : Alert description.
* @param[out] records : Encoded alert records.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13SESSION::Alert_Create(DIOSTREAMTLS_ALERT_LEVEL level, DIOSTREAMTLS_ALERT_DESCRIPTION description, XBUFFER& records)
{
  DIOSTREAMTLS_MSG_ALERT alert;
  XBUFFER                plain;

  if(!isini || closenotifysent) return false;

  bool warningallowed = (description == DIOSTREAMTLS_ALERT_DESCRIPTION_CLOSE_NOTIFY) ||
                        (description == DIOSTREAMTLS_ALERT_DESCRIPTION_USER_CANCELED);

  if((warningallowed && (level != DIOSTREAMTLS_ALERT_LEVEL_WARNING)) ||
     (!warningallowed && (level != DIOSTREAMTLS_ALERT_LEVEL_FATAL)))
    {
      return false;
    }

  alert.SetLevel(level);
  alert.SetDescription(description);

  if(!alert.SetToBuffer(plain, false) ||
     !record.Protect(DIOSTREAMTLS_MSG_CONTENTTYPE_ALERT, plain, records))
    {
      return false;
    }

  if(description == DIOSTREAMTLS_ALERT_DESCRIPTION_CLOSE_NOTIFY) closenotifysent = true;
  if(level == DIOSTREAMTLS_ALERT_LEVEL_FATAL)                     iserror         = true;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13SESSION::CloseNotify_Create(XBUFFER& records)
* @brief      Create an idempotent close_notify alert
* @ingroup    DATAIO
*
* @param[out] records : Encoded close_notify record, or an empty buffer when it was already sent.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13SESSION::CloseNotify_Create(XBUFFER& records)
{
  records.Delete();

  if(closenotifysent) return true;

  if(!isini ||
     (epoch[DIOSTREAMTLSKEYSCHEDULE_DIRECTION_LOCAL] != DIOSTREAMTLS13SESSION_EPOCH_APPLICATION))
    {
      return false;
    }

  return Alert_Create(DIOSTREAMTLS_ALERT_LEVEL_WARNING, DIOSTREAMTLS_ALERT_DESCRIPTION_CLOSE_NOTIFY, records);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13SESSION::IsCloseNotifySent()
* @brief      Check whether close_notify was sent
* @ingroup    DATAIO
*
* @return     bool : true if the condition is met; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13SESSION::IsCloseNotifySent()
{
  return closenotifysent;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13SESSION::IsCloseNotifyReceived()
* @brief      Check whether close_notify was received
* @ingroup    DATAIO
*
* @return     bool : true if the condition is met; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13SESSION::IsCloseNotifyReceived()
{
  return closenotifyreceived;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13SESSION::IsError()
* @brief      Check whether the application record stream is in error
* @ingroup    DATAIO
*
* @return     bool : true if the condition is met; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13SESSION::IsError()
{
  return iserror;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13SESSION::IsTransportClosedWithoutNotify()
* @brief      Check whether the transport ended without close_notify
* @ingroup    DATAIO
*
* @return     bool : true if the condition is met; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13SESSION::IsTransportClosedWithoutNotify()
{
  return transportclosedwithoutnotify;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_ALERT_LEVEL DIOSTREAMTLS13SESSION::GetReceivedAlertLevel()
* @brief      Get the last received alert level
* @ingroup    DATAIO
*
* @return     DIOSTREAMTLS_ALERT_LEVEL : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_ALERT_LEVEL DIOSTREAMTLS13SESSION::GetReceivedAlertLevel()
{
  return receivedalertlevel;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_ALERT_DESCRIPTION DIOSTREAMTLS13SESSION::GetReceivedAlertDescription()
* @brief      Get the last received alert description
* @ingroup    DATAIO
*
* @return     DIOSTREAMTLS_ALERT_DESCRIPTION : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_ALERT_DESCRIPTION DIOSTREAMTLS13SESSION::GetReceivedAlertDescription()
{
  return receivedalertdescription;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS13SESSION::TransportClosed()
* @brief      Register the end of the underlying transport
* @ingroup    DATAIO
*
* @return     bool : true when close_notify had been received; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS13SESSION::TransportClosed()
{
  if(!isini) return false;

  if(closenotifyreceived) return true;

  transportclosedwithoutnotify = true;
  iserror                      = true;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS13SESSION::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS13SESSION::Clean()
{
  role                         = DIOSTREAMTLSKEYSCHEDULE_ROLE_CLIENT;
  isini                        = false;
  applicationsecretscalculated = false;
  maximumrecordinputsize       = DIOSTREAMTLS_MEMORY_DEFAULT_RECORD_INPUT;
  maximumhandshakeinputsize    = DIOSTREAMTLS_MEMORY_DEFAULT_HANDSHAKE_INPUT;
  maximumtranscriptsize        = DIOSTREAMTLS_MEMORY_DEFAULT_TRANSCRIPT;
  maximumapplicationinputsize  = DIOSTREAMTLS_MEMORY_DEFAULT_APPLICATION_INPUT;
  maximumearlydatasize        = DIOSTREAMTLS13_EARLYDATA_MAXSIZE;
  earlydatareceived           = 0;
  earlydatasent               = 0;
  earlydataaccepted           = false;

  for(int c=0; c<DIOSTREAMTLSKEYSCHEDULE_MAXDIRECTIONS; c++)
    {
      epoch[c] = DIOSTREAMTLS13SESSION_EPOCH_CLEAR;
    }

  recordinput.Delete();
  handshakeinput.Delete();
  transcript.Delete();
  applicationinput.Delete();
  posthandshakeoutput.Delete();
  newsessionticketinput.Delete();

  KeyExchange_Delete();

  keyupdaterequestpending       = false;
  keyupdateresponsepending      = false;
  closenotifysent               = false;
  closenotifyreceived           = false;
  iserror                       = false;
  transportclosedwithoutnotify = false;
  receivedalertlevel            = (DIOSTREAMTLS_ALERT_LEVEL)0;
  receivedalertdescription      = (DIOSTREAMTLS_ALERT_DESCRIPTION)0;
  lastrecordalertdescription    = DIOSTREAMTLS_ALERT_DESCRIPTION_DECODE_ERROR;

  for(int c=0; c<DIOSTREAMTLSKEYSCHEDULE_MAXDIRECTIONS; c++)
    {
      keyupdates[c] = 0;
    }
}
