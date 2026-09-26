/**-------------------------------------------------------------------------------------------------------------------
*
* @file       DIOStreamTLS12KeySchedule.cpp
*
* @class      DIOSTREAMTLS12KEYSCHEDULE
* @brief      Data Input/Output Stream TLS 1.2 key schedule class (RFC 5246)
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

#include "DIOStreamTLS12KeySchedule.h"

#include <string.h>

#include "Hash.h"
#include "HashSHA2.h"
#include "HashHMAC.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"



/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         static void DIOStreamTLS12_BufferErase(XBUFFER& buffer)
* @brief      Dio stream tls12 buffer erase
* @ingroup    DATAIO
* 
* @param[in]  buffer : Buffer value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static void DIOStreamTLS12_BufferErase(XBUFFER& buffer)
{
  volatile XBYTE* data = buffer.Get();

  for(XDWORD c=0; c<buffer.GetSize(); c++) data[c] = 0;

  buffer.Delete();
}


class DIOSTREAMTLS12SECUREBUFFER : public XBUFFER
{
  public:

    /**-------------------------------------------------------------------------------------------------------------------
    * 
    * @fn         virtual ~DIOSTREAMTLS12SECUREBUFFER()
    * @brief      Destructor of class
    * @ingroup    DATAIO
    * 
    * --------------------------------------------------------------------------------------------------------------------*/
    virtual ~DIOSTREAMTLS12SECUREBUFFER()
    {
      DIOStreamTLS12_BufferErase((*this));
    }
};


class DIOSTREAMTLS12SECUREHMAC : public HASHHMAC
{
  public:

    /**-------------------------------------------------------------------------------------------------------------------
    * 
    * @fn         virtual ~DIOSTREAMTLS12SECUREHMAC()
    * @brief      Destructor of class
    * @ingroup    DATAIO
    * 
    * --------------------------------------------------------------------------------------------------------------------*/
    virtual ~DIOSTREAMTLS12SECUREHMAC()
    {
      if(GetKey())    DIOStreamTLS12_BufferErase((*GetKey()));
      if(GetResult()) DIOStreamTLS12_BufferErase((*GetResult()));
    }
};



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS12KEYSCHEDULE::DIOSTREAMTLS12KEYSCHEDULE()
* @brief      Constructor of class
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS12KEYSCHEDULE::DIOSTREAMTLS12KEYSCHEDULE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS12KEYSCHEDULE::~DIOSTREAMTLS12KEYSCHEDULE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS12KEYSCHEDULE::~DIOSTREAMTLS12KEYSCHEDULE()
{
  End();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS12KEYSCHEDULE::Ini(XWORD ciphersuite, DIOSTREAMTLSKEYSCHEDULE_ROLE role)
* @brief      Initialize the key schedule for one negotiated TLS 1.2 cipher suite
* @ingroup    DATAIO
*
* @param[in]  ciphersuite : Negotiated cipher suite.
* @param[in]  role : Role of this end.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS12KEYSCHEDULE::Ini(XWORD ciphersuite, DIOSTREAMTLSKEYSCHEDULE_ROLE role)
{
  End();

  // Only ECDHE + AEAD suites are supported. The CBC ones are deliberately absent: they need
  // MAC-then-encrypt with all its padding handling, and no modern server requires them.
  switch(ciphersuite)
    {
      case DIOSTREAMTLS12_CIPHER_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256 :
      case DIOSTREAMTLS12_CIPHER_ECDHE_RSA_WITH_AES_128_GCM_SHA256   : hash        = GEN_NEW HASHSHA2(HASHSHA2TYPE_256);
                                                                       keysize     = 16;
                                                                       fixedIVsize = DIOSTREAMTLS12KEYSCHEDULE_AESGCM_FIXEDIVSIZE;
                                                                       break;

      case DIOSTREAMTLS12_CIPHER_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384 :
      case DIOSTREAMTLS12_CIPHER_ECDHE_RSA_WITH_AES_256_GCM_SHA384   : hash        = GEN_NEW HASHSHA2(HASHSHA2TYPE_384);
                                                                       keysize     = 32;
                                                                       fixedIVsize = DIOSTREAMTLS12KEYSCHEDULE_AESGCM_FIXEDIVSIZE;
                                                                       break;

      #ifdef CIPHER_SYMMETRIC_CHACHA20POLY1305_ACTIVE

      case DIOSTREAMTLS_MSG_CIPHER_ECDHE_RSA_WITH_CHACHA20_POLY1305_SHA256   :
      case DIOSTREAMTLS_MSG_CIPHER_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256 : hash        = GEN_NEW HASHSHA2(HASHSHA2TYPE_256);
                                                                               keysize     = 32;
                                                                               fixedIVsize = DIOSTREAMTLS12KEYSCHEDULE_CHACHA20_FIXEDIVSIZE;
                                                                               break;

      #endif

                                                            default  : return false;
    }

  if(!hash) return false;

  hashsize = (XDWORD)hash->GetDefaultSize();

  if(!hashsize)
    {
      End();
      return false;
    }

  this->ciphersuite = ciphersuite;
  this->role        = role;

  isini = true;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS12KEYSCHEDULE::End()
* @brief      Release every secret and the hash of the negotiated suite
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS12KEYSCHEDULE::End()
{
  if(hash)
    {
      if(hash->GetResult()) DIOStreamTLS12_BufferErase((*hash->GetResult()));

      GEN_DELETE hash;
      hash = NULL;
    }

  DIOStreamTLS12_BufferErase(mastersecret);

  for(int c=0; c<DIOSTREAMTLSKEYSCHEDULE_MAXDIRECTIONS; c++)
    {
      DIOStreamTLS12_BufferErase(key[c]);
      DIOStreamTLS12_BufferErase(fixedIV[c]);
    }

  isini       = false;
  ciphersuite = 0;
  hashsize    = 0;
  keysize     = 0;
  fixedIVsize = 0;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS12KEYSCHEDULE::IsIni()
* @brief      Check whether the key schedule is initialized
* @ingroup    DATAIO
*
* @return     bool : true if it is initialized; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS12KEYSCHEDULE::IsIni()
{
  return isini;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XWORD DIOSTREAMTLS12KEYSCHEDULE::GetCipherSuite()
* @brief      Get the negotiated cipher suite
* @ingroup    DATAIO
*
* @return     XWORD : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
XWORD DIOSTREAMTLS12KEYSCHEDULE::GetCipherSuite()
{
  return ciphersuite;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLSKEYSCHEDULE_ROLE DIOSTREAMTLS12KEYSCHEDULE::GetRole()
* @brief      Get the role of this end
* @ingroup    DATAIO
*
* @return     DIOSTREAMTLSKEYSCHEDULE_ROLE : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLSKEYSCHEDULE_ROLE DIOSTREAMTLS12KEYSCHEDULE::GetRole()
{
  return role;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD DIOSTREAMTLS12KEYSCHEDULE::GetHashSize()
* @brief      Get the output size of the hash of the negotiated suite
* @ingroup    DATAIO
*
* @return     XDWORD : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAMTLS12KEYSCHEDULE::GetHashSize()
{
  return hashsize;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD DIOSTREAMTLS12KEYSCHEDULE::GetKeySize()
* @brief      Get the size of the write key of the negotiated suite
* @ingroup    DATAIO
*
* @return     XDWORD : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAMTLS12KEYSCHEDULE::GetKeySize()
{
  return keysize;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD DIOSTREAMTLS12KEYSCHEDULE::GetFixedIVSize()
* @brief      Get the size of the AEAD salt of the negotiated suite
* @ingroup    DATAIO
*
* @return     XDWORD : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOSTREAMTLS12KEYSCHEDULE::GetFixedIVSize()
{
  return fixedIVsize;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         HASH* DIOSTREAMTLS12KEYSCHEDULE::GetHash()
* @brief      Get the hash of the negotiated suite
* @ingroup    DATAIO
*
* @return     HASH* : Pointer to the requested object; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
HASH* DIOSTREAMTLS12KEYSCHEDULE::GetHash()
{
  return hash;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS12KEYSCHEDULE::PHash(XBUFFER& secret, XBUFFER& seed, XDWORD outputsize, XBUFFER& output)
* @brief      Expand a secret with P_hash, the data expansion function of RFC 5246 section 5
* @note       INTERNAL
* @ingroup    DATAIO
*
* @param[in]  secret : Secret to expand.
* @param[in]  seed : Seed, already built as label + seed by the caller.
* @param[in]  outputsize : Number of bytes required.
* @param[out] output : Buffer that receives exactly outputsize bytes.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS12KEYSCHEDULE::PHash(XBUFFER& secret, XBUFFER& seed, XDWORD outputsize, XBUFFER& output)
{
  if(!isini || !hash || !outputsize) return false;

  output.Delete();

  // A(0) = seed, A(i) = HMAC(secret, A(i-1))
  // output = HMAC(secret, A(1) + seed) + HMAC(secret, A(2) + seed) + ...

  DIOSTREAMTLS12SECUREBUFFER A;

  if(!A.Add(seed)) return false;

  while(output.GetSize() < outputsize)
    {
      DIOSTREAMTLS12SECUREHMAC   HMAC;
      DIOSTREAMTLS12SECUREBUFFER newA;
      DIOSTREAMTLS12SECUREBUFFER block;

      if(!HMAC.SetHash(hash) || !HMAC.SetKey(secret) || !HMAC.Do(A)) return false;
      if(!newA.Add(HMAC.GetResult()->Get(), hashsize))               return false;

      DIOSTREAMTLS12SECUREHMAC   HMACblock;
      DIOSTREAMTLS12SECUREBUFFER input;

      if(!input.Add(newA) || !input.Add(seed))                                       return false;
      if(!HMACblock.SetHash(hash) || !HMACblock.SetKey(secret) || !HMACblock.Do(input)) return false;
      if(!block.Add(HMACblock.GetResult()->Get(), hashsize))                         return false;

      if(!output.Add(block)) return false;

      DIOStreamTLS12_BufferErase(A);
      if(!A.Add(newA)) return false;
    }

  // The last block is normally longer than what is left to fill.
  if(output.GetSize() > outputsize)
    {
      if(!output.Resize(outputsize)) return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS12KEYSCHEDULE::PRF(XBUFFER& secret, const char* label, XBUFFER& seed, XDWORD outputsize, XBUFFER& output)
* @brief      TLS 1.2 pseudorandom function, PRF(secret, label, seed) = P_hash(secret, label + seed)
* @ingroup    DATAIO
*
* @param[in]  secret : Secret to expand.
* @param[in]  label : ASCII label, without its terminator.
* @param[in]  seed : Seed that follows the label.
* @param[in]  outputsize : Number of bytes required.
* @param[out] output : Buffer that receives exactly outputsize bytes.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS12KEYSCHEDULE::PRF(XBUFFER& secret, const char* label, XBUFFER& seed, XDWORD outputsize, XBUFFER& output)
{
  if(!isini || !label) return false;

  XBUFFER labelandseed;

  if(!labelandseed.Add((XBYTE*)label, (XDWORD)strlen(label))) return false;
  if(!labelandseed.Add(seed))                                return false;

  return PHash(secret, labelandseed, outputsize, output);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS12KEYSCHEDULE::MasterSecret_Create(XBUFFER& premastersecret, XBUFFER& clientrandom, XBUFFER& serverrandom)
* @brief      Derive the master secret from the pre master secret, RFC 5246 section 8.1
* @ingroup    DATAIO
*
* @param[in]  premastersecret : Pre master secret; for ECDHE it is the raw shared secret.
* @param[in]  clientrandom : Client random, 32 bytes.
* @param[in]  serverrandom : Server random, 32 bytes.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS12KEYSCHEDULE::MasterSecret_Create(XBUFFER& premastersecret, XBUFFER& clientrandom, XBUFFER& serverrandom)
{
  if(!isini || premastersecret.IsEmpty())                                     return false;
  if(clientrandom.GetSize() != DIOSTREAMTLS12KEYSCHEDULE_RANDOMSIZE)          return false;
  if(serverrandom.GetSize() != DIOSTREAMTLS12KEYSCHEDULE_RANDOMSIZE)          return false;

  XBUFFER seed;

  if(!seed.Add(clientrandom) || !seed.Add(serverrandom)) return false;

  DIOStreamTLS12_BufferErase(mastersecret);

  return PRF(premastersecret, DIOSTREAMTLS12KEYSCHEDULE_LABEL_MASTERSECRET, seed, DIOSTREAMTLS12KEYSCHEDULE_MASTERSECRETSIZE, mastersecret);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS12KEYSCHEDULE::MasterSecretExtended_Create(XBUFFER& premastersecret, XBUFFER& sessionhash)
* @brief      Derive the extended master secret according to RFC 7627
* @ingroup    DATAIO
* 
* @param[in]  premastersecret : Premastersecret value.
* @param[in]  sessionhash : Sessionhash value.
*
* @return     bool : true if the operation is successful; otherwise false.
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS12KEYSCHEDULE::MasterSecretExtended_Create(XBUFFER& premastersecret, XBUFFER& sessionhash)
{
  if(!isini || premastersecret.IsEmpty() || (sessionhash.GetSize() != hashsize)) return false;

  mastersecret.SecureDelete();

  return PRF(premastersecret, DIOSTREAMTLS12KEYSCHEDULE_LABEL_EXTENDEDMASTERSECRET, sessionhash, DIOSTREAMTLS12KEYSCHEDULE_MASTERSECRETSIZE, mastersecret);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XBUFFER* DIOSTREAMTLS12KEYSCHEDULE::GetMasterSecret()
* @brief      Get the master secret
* @ingroup    DATAIO
*
* @return     XBUFFER* : Pointer to the requested object; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* DIOSTREAMTLS12KEYSCHEDULE::GetMasterSecret()
{
  return &mastersecret;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS12KEYSCHEDULE::KeyBlock_Create(XBUFFER& clientrandom, XBUFFER& serverrandom)
* @brief      Expand the key block and split it into the write key and AEAD salt of each direction
* @ingroup    DATAIO
*
* @param[in]  clientrandom : Client random, 32 bytes.
* @param[in]  serverrandom : Server random, 32 bytes.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS12KEYSCHEDULE::KeyBlock_Create(XBUFFER& clientrandom, XBUFFER& serverrandom)
{
  if(!isini || mastersecret.IsEmpty())                                        return false;
  if(clientrandom.GetSize() != DIOSTREAMTLS12KEYSCHEDULE_RANDOMSIZE)          return false;
  if(serverrandom.GetSize() != DIOSTREAMTLS12KEYSCHEDULE_RANDOMSIZE)          return false;

  XBUFFER                     seed;
  DIOSTREAMTLS12SECUREBUFFER keyblock;

  // RFC 5246 section 6.3: server_random FIRST here, the opposite order of the master secret.
  if(!seed.Add(serverrandom) || !seed.Add(clientrandom)) return false;

  // An AEAD suite has no MAC key, so the block is only the two write keys and the two salts.
  XDWORD keyblocksize = (keysize * 2) + (fixedIVsize * 2);

  if(!PRF(mastersecret, DIOSTREAMTLS12KEYSCHEDULE_LABEL_KEYEXPANSION, seed, keyblocksize, keyblock)) return false;
  if(keyblock.GetSize() != keyblocksize)                                                            return false;

  // Order inside the block: client_write_key, server_write_key, client_write_IV, server_write_IV.
  XDWORD position = 0;

  DIOSTREAMTLS12SECUREBUFFER clientkey;
  DIOSTREAMTLS12SECUREBUFFER serverkey;
  DIOSTREAMTLS12SECUREBUFFER clientIV;
  DIOSTREAMTLS12SECUREBUFFER serverIV;

  if(!clientkey.Add(keyblock.Get() + position, keysize)) return false;
  position += keysize;

  if(!serverkey.Add(keyblock.Get() + position, keysize)) return false;
  position += keysize;

  if(!clientIV.Add(keyblock.Get() + position, fixedIVsize)) return false;
  position += fixedIVsize;

  if(!serverIV.Add(keyblock.Get() + position, fixedIVsize)) return false;

  // Stored by direction, never by role, so that a future server end reuses this class untouched.
  bool isclient = (role == DIOSTREAMTLSKEYSCHEDULE_ROLE_CLIENT);

  for(int c=0; c<DIOSTREAMTLSKEYSCHEDULE_MAXDIRECTIONS; c++)
    {
      DIOStreamTLS12_BufferErase(key[c]);
      DIOStreamTLS12_BufferErase(fixedIV[c]);
    }

  if(!key[DIOSTREAMTLSKEYSCHEDULE_DIRECTION_LOCAL].Add(isclient?clientkey:serverkey))       return false;
  if(!key[DIOSTREAMTLSKEYSCHEDULE_DIRECTION_REMOTE].Add(isclient?serverkey:clientkey))      return false;
  if(!fixedIV[DIOSTREAMTLSKEYSCHEDULE_DIRECTION_LOCAL].Add(isclient?clientIV:serverIV))     return false;
  if(!fixedIV[DIOSTREAMTLSKEYSCHEDULE_DIRECTION_REMOTE].Add(isclient?serverIV:clientIV))    return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XBUFFER* DIOSTREAMTLS12KEYSCHEDULE::GetKey(DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction)
* @brief      Get the write key of one direction
* @ingroup    DATAIO
*
* @param[in]  direction : Direction value.
*
* @return     XBUFFER* : Pointer to the requested object; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* DIOSTREAMTLS12KEYSCHEDULE::GetKey(DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction)
{
  if(direction >= DIOSTREAMTLSKEYSCHEDULE_MAXDIRECTIONS) return NULL;

  return &key[direction];
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XBUFFER* DIOSTREAMTLS12KEYSCHEDULE::GetFixedIV(DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction)
* @brief      Get the AEAD salt of one direction
* @ingroup    DATAIO
*
* @param[in]  direction : Direction value.
*
* @return     XBUFFER* : Pointer to the requested object; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* DIOSTREAMTLS12KEYSCHEDULE::GetFixedIV(DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction)
{
  if(direction >= DIOSTREAMTLSKEYSCHEDULE_MAXDIRECTIONS) return NULL;

  return &fixedIV[direction];
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLS12KEYSCHEDULE::VerifyData_Create(bool isclient, XBUFFER& handshakehash, XBUFFER& verifydata)
* @brief      Compute the verify_data of a Finished message, RFC 5246 section 7.4.9
* @ingroup    DATAIO
*
* @param[in]  isclient : true for the Finished written by the client; false for the one written by the server.
* @param[in]  handshakehash : Hash of every handshake message exchanged so far.
* @param[out] verifydata : Buffer that receives the 12 bytes of verify_data.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLS12KEYSCHEDULE::VerifyData_Create(bool isclient, XBUFFER& handshakehash, XBUFFER& verifydata)
{
  if(!isini || mastersecret.IsEmpty() || handshakehash.IsEmpty()) return false;

  verifydata.Delete();

  return PRF(mastersecret, isclient?DIOSTREAMTLS12KEYSCHEDULE_LABEL_CLIENTFINISHED:DIOSTREAMTLS12KEYSCHEDULE_LABEL_SERVERFINISHED, handshakehash, DIOSTREAMTLS12KEYSCHEDULE_VERIFYDATASIZE, verifydata);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLS12KEYSCHEDULE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLS12KEYSCHEDULE::Clean()
{
  isini       = false;
  ciphersuite = 0;
  role        = DIOSTREAMTLSKEYSCHEDULE_ROLE_CLIENT;
  hash        = NULL;
  hashsize    = 0;
  keysize     = 0;
  fixedIVsize = 0;
}
