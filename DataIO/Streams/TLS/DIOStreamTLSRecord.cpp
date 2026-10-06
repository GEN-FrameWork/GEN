/**-------------------------------------------------------------------------------------------------------------------
*
* @file       DIOStreamTLSRecord.cpp
*
* @class      DIOSTREAMTLSRECORD
* @brief      Data Input/Output Stream TLS Record Layer (TLS 1.3, RFC 8446 section 5) class
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

#include "DIOStreamTLSRecord.h"

#include "XFactory.h"
#include "XTrace.h"



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
* @fn         DIOSTREAMTLSRECORD::DIOSTREAMTLSRECORD()
* @brief      Constructor of class
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLSRECORD::DIOSTREAMTLSRECORD()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLSRECORD::~DIOSTREAMTLSRECORD()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLSRECORD::~DIOSTREAMTLSRECORD()
{
  End();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSRECORD::Ini(DIOSTREAMTLS13KEYSCHEDULE* keyschedule)
* @brief      Prepare the record layer over a key schedule
* @note       Until SetKeys() is called for a direction, the records of that direction travel in the clear, which is
*             what the first flight of the handshake needs.
* @ingroup    DATAIO
*
* @param[in]  keyschedule : Key schedule of the connection. It is not owned by this class.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSRECORD::Ini(DIOSTREAMTLS13KEYSCHEDULE* keyschedule)
{
  End();

  if(!keyschedule)
    {
      return false;
    }

  if(!keyschedule->IsIni())
    {
      return false;
    }

  this->keyschedule = keyschedule;

  isini             = true;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLSRECORD::End()
* @brief      Release the ciphers and wipe the key material
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLSRECORD::End()
{
  for(int c=0; c<DIOSTREAMTLSKEYSCHEDULE_MAXDIRECTIONS; c++)
    {
      if(key[c] && key[c]->Get()) DIOStreamTLS_BufferErase((*key[c]->Get()));
      DIOStreamTLS_BufferErase(IV[c]);

      if(cipher[c])
        {
          GEN_DELETE cipher[c];
          cipher[c] = NULL;
        }

      if(key[c])
        {
          GEN_DELETE key[c];
          key[c] = NULL;
        }

      sequence[c]    = 0;
      isprotected[c] = false;
    }

  keyschedule = NULL;
  isini       = false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSRECORD::IsIni()
* @brief      Is the record layer ready to be used
* @ingroup    DATAIO
*
* @return     bool : true if the condition is met; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSRECORD::IsIni()
{
  return isini;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSRECORD::ClearKeys(DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction)
* @brief      Remove the traffic keys for one direction and return it to clear records
* @ingroup    DATAIO
* 
* @param[in]  direction : Direction value.
*
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSRECORD::ClearKeys(DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction)
{
  if(!isini || direction >= DIOSTREAMTLSKEYSCHEDULE_MAXDIRECTIONS) return false;

  if(key[direction] && key[direction]->Get()) DIOStreamTLS_BufferErase((*key[direction]->Get()));
  DIOStreamTLS_BufferErase(IV[direction]);

  if(cipher[direction]) { GEN_DELETE cipher[direction]; cipher[direction] = NULL; }
  if(key[direction])    { GEN_DELETE key[direction];    key[direction] = NULL; }

  sequence[direction] = 0;
  isprotected[direction] = false;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSRECORD::SetKeys(DIOSTREAMTLS13KEYSCHEDULE_LEVEL level, DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction)
* @brief      Install the traffic keys of a level in one direction, and restart its sequence number
* @note       RFC 8446 section 5.3: every time the keys change, the sequence number goes back to zero.
* @ingroup    DATAIO
*
* @param[in]  level : Handshake or application.
* @param[in]  direction : Local, what this end writes, or remote, what the other end writes.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSRECORD::SetKeys(DIOSTREAMTLS13KEYSCHEDULE_LEVEL level, DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction)
{
  if(!isini || !keyschedule)
    {
      return false;
    }

  if(direction >= DIOSTREAMTLSKEYSCHEDULE_MAXDIRECTIONS)
    {
      return false;
    }

  XSECUREBUFFER trafficzkey;

  if(!keyschedule->GetTrafficKeys(level, direction, trafficzkey, IV[direction]))
    {
      return false;
    }

  if(cipher[direction])
    {
      GEN_DELETE cipher[direction];
      cipher[direction] = NULL;
    }

  if(key[direction])
    {
      GEN_DELETE key[direction];
      key[direction] = NULL;
    }

  key[direction] = GEN_NEW CIPHERKEYSYMMETRICAL();
  if(!key[direction])
    {
      return false;
    }

  if(!key[direction]->Set(trafficzkey))
    {
      GEN_DELETE key[direction];
      key[direction] = NULL;
      IV[direction].Delete();
      return false;
    }

  switch(keyschedule->GetCipherSuite())
    {
      case DIOSTREAMTLS_MSG_CIPHER_AES_128_GCM_SHA256:
      case DIOSTREAMTLS_MSG_CIPHER_AES_256_GCM_SHA384:       cipher[direction] = GEN_NEW CIPHERAESGCM();                break;

      #ifdef CIPHER_SYMMETRIC_CHACHA20POLY1305_ACTIVE

      case DIOSTREAMTLS_MSG_CIPHER_CHACHA20_POLY1305_SHA256: cipher[direction] = GEN_NEW CIPHERCHACHA20POLY1305();      break;

      #endif

                                                  default:   cipher[direction] = NULL;                                   break;
    }

  if(!cipher[direction])
    {
      GEN_DELETE key[direction];
      key[direction] = NULL;
      IV[direction].Delete();
      return false;
    }

  if(!cipher[direction]->SetKey(key[direction]))
    {
      GEN_DELETE cipher[direction];
      cipher[direction] = NULL;

      GEN_DELETE key[direction];
      key[direction] = NULL;

      IV[direction].Delete();
      return false;
    }

  sequence[direction]    = 0;
  isprotected[direction] = true;

  /* XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("[TLS Record] Keys of the %s direction installed: %s level, key %d, iv %d"),
                                       (direction == DIOSTREAMTLSKEYSCHEDULE_DIRECTION_LOCAL)?_L("local"):_L("remote"),
                                       (level == DIOSTREAMTLS13KEYSCHEDULE_LEVEL_HANDSHAKE)?_L("handshake"):_L("application"),
                                       trafficzkey.GetSize(), IV[direction].GetSize()); */

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSRECORD::IsProtected(DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction)
* @brief      Are the records of a direction ciphered
* @ingroup    DATAIO
*
* @param[in]  direction : Local or remote.
*
* @return     bool : true if the condition is met; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSRECORD::IsProtected(DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction)
{
  if(direction >= DIOSTREAMTLSKEYSCHEDULE_MAXDIRECTIONS)
    {
      return false;
    }

  return isprotected[direction];
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XQWORD DIOSTREAMTLSRECORD::GetSequence(DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction)
* @brief      Get the sequence number of a direction
* @ingroup    DATAIO
*
* @param[in]  direction : Local or remote.
*
* @return     XQWORD : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
XQWORD DIOSTREAMTLSRECORD::GetSequence(DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction)
{
  if(direction >= DIOSTREAMTLSKEYSCHEDULE_MAXDIRECTIONS)
    {
      return 0;
    }

  return sequence[direction];
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSRECORD::ResetSequence(DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction)
* @brief      Set the sequence number of a direction back to zero
* @ingroup    DATAIO
*
* @param[in]  direction : Local or remote.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSRECORD::ResetSequence(DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction)
{
  if(direction >= DIOSTREAMTLSKEYSCHEDULE_MAXDIRECTIONS)
    {
      return false;
    }

  sequence[direction] = 0;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XWORD DIOSTREAMTLSRECORD::GetMaxPlainSize()
* @brief      Get the maximum size, in bytes, of the plain text carried by one record
* @ingroup    DATAIO
*
* @return     XWORD : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
XWORD DIOSTREAMTLSRECORD::GetMaxPlainSize()
{
  return maxplainsize;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSRECORD::SetMaxPlainSize(XWORD maxplainsize)
* @brief      Set the maximum size, in bytes, of the plain text carried by one record
* @note       Useful on a microcontroller, where a smaller limit keeps the working buffers within reach.
* @ingroup    DATAIO
*
* @param[in]  maxplainsize : Size, from 1 to DIOSTREAMTLSRECORD_MAXPLAINSIZE.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSRECORD::SetMaxPlainSize(XWORD maxplainsize)
{
  if(!maxplainsize || (maxplainsize > DIOSTREAMTLSRECORD_MAXPLAINSIZE))
    {
      return false;
    }

  this->maxplainsize = maxplainsize;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XBYTE DIOSTREAMTLSRECORD::GetPaddingSize()
* @brief      Get the number of zero bytes added to every record to hide its real length
* @ingroup    DATAIO
*
* @return     XBYTE : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOSTREAMTLSRECORD::GetPaddingSize()
{
  return paddingsize;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLSRECORD::SetPaddingSize(XBYTE paddingsize)
* @brief      Set the number of zero bytes added to every record to hide its real length
* @ingroup    DATAIO
*
* @param[in]  paddingsize : Number of bytes. Zero, the default, adds no padding.
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLSRECORD::SetPaddingSize(XBYTE paddingsize)
{
  this->paddingsize = paddingsize;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSRECORD::Protect(DIOSTREAMTLS_CONTENTTYPE contenttype, XBYTE* plain, XDWORD size, XBUFFER& records)
* @brief      Turn a piece of content into one or more records, ciphered when the keys of the local direction are set
* @note       The records are added to the output buffer, so several calls can be accumulated into one write.
* @ingroup    DATAIO
*
* @param[in]  contenttype : Type of the content.
* @param[in]  plain : Content pointer to use.
* @param[in]  size : Size of the content.
* @param[out] records : Buffer that receives the records.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSRECORD::Protect(DIOSTREAMTLS_CONTENTTYPE contenttype, XBYTE* plain, XDWORD size, XBUFFER& records)
{
  if(!isini)
    {
      return false;
    }

  if(!plain && size)
    {
      return false;
    }

  XDWORD offset = 0;

  if(!size)                                                                     // An empty record is legal, an alert or a handshake fragment of zero length is not
    {
      return Protect_OneRecord(contenttype, NULL, 0, records);
    }

  while(offset < size)
    {
      XDWORD use = ((size - offset) < maxplainsize)?(size - offset):maxplainsize;

      if(!Protect_OneRecord(contenttype, &plain[offset], (XWORD)use, records))
        {
          return false;
        }

      offset += use;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSRECORD::Protect(DIOSTREAMTLS_CONTENTTYPE contenttype, XBUFFER& plain, XBUFFER& records)
* @brief      Turn a piece of content into one or more records
* @ingroup    DATAIO
*
* @param[in]  contenttype : Type of the content.
* @param[in]  plain : Content buffer to use.
* @param[out] records : Buffer that receives the records.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSRECORD::Protect(DIOSTREAMTLS_CONTENTTYPE contenttype, XBUFFER& plain, XBUFFER& records)
{
  return Protect(contenttype, plain.Get(), plain.GetSize(), records);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSRECORD::Unprotect(XBUFFER& record, DIOSTREAMTLS_CONTENTTYPE& contenttype, XBUFFER& plain)
* @brief      Decode and authenticate one complete TLS record
* @ingroup    DATAIO
*
* @param[in]  record : One complete record, header included.
* @param[out] contenttype : Decoded content type.
* @param[out] plain : Buffer that receives the plain text.
*
* @return     bool : true if the record is valid; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSRECORD::Unprotect(XBUFFER& record, DIOSTREAMTLS_CONTENTTYPE& contenttype, XBUFFER& plain)
{
  DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction = DIOSTREAMTLSKEYSCHEDULE_DIRECTION_REMOTE;
  DIOSTREAMTLS_MSG_RECORDHEADER     header;
  XWORD                             length;

  plain.Delete();
  lastalertdescription = DIOSTREAMTLS_ALERT_DESCRIPTION_DECODE_ERROR;

  if(!isini) return false;
  if(!header.Peek(record)) return false;

  contenttype = header.GetContenType();
  length      = header.GetLength();

  if(record.GetSize() != (XDWORD)(DIOSTREAMTLS_MSG_RECORDHEADER_SIZE + length)) return false;

  if(!isprotected[direction] || (contenttype == DIOSTREAMTLS_MSG_CONTENTTYPE_CHANGE_CIPHER_SPEC))
    {
      if(length > DIOSTREAMTLSRECORD_MAXPLAINSIZE)
        {
          lastalertdescription = DIOSTREAMTLS_ALERT_DESCRIPTION_RECORD_OVERFLOW;
          return false;
        }

      if(length && !plain.Add(&record.Get()[DIOSTREAMTLS_MSG_RECORDHEADER_SIZE], length))
        {
          lastalertdescription = DIOSTREAMTLS_ALERT_DESCRIPTION_INTERNAL_ERROR;
          return false;
        }

      return true;
    }

  // RFC 8446 sections 5.1 and 5.2: protected TLS 1.3 records use the TLS 1.2
  // legacy_record_version (0x0303) and application_data as the outer content type.
  if(header.GetProtocolVersion() != DIOSTREAMTLSRECORD_LEGACYVERSION)
    {
      lastalertdescription = DIOSTREAMTLS_ALERT_DESCRIPTION_PROTOCOL_VERSION;
      return false;
    }

  if(contenttype != DIOSTREAMTLS_MSG_CONTENTTYPE_APPLICATION_DATA)
    {
      lastalertdescription = DIOSTREAMTLS_ALERT_DESCRIPTION_UNEXPECTED_MESSAGE;
      return false;
    }

  if(!cipher[direction])
    {
      lastalertdescription = DIOSTREAMTLS_ALERT_DESCRIPTION_INTERNAL_ERROR;
      return false;
    }

  if((keyschedule->GetCipherSuite() == DIOSTREAMTLS_MSG_CIPHER_AES_128_GCM_SHA256 ||
      keyschedule->GetCipherSuite() == DIOSTREAMTLS_MSG_CIPHER_AES_256_GCM_SHA384) &&
     sequence[direction] >= DIOSTREAMTLS_AESGCM_MAXKEYUSAGERECORDS)
    {
      // The peer has exhausted the traffic key without updating it.  Do not process
      // any more ciphertext with a key that has reached the RFC 8446 usage bound.
      lastalertdescription = DIOSTREAMTLS_ALERT_DESCRIPTION_UNEXPECTED_MESSAGE;
      return false;
    }

  if(sequence[direction] == 0xFFFFFFFFFFFFFFFFULL)
    {
      lastalertdescription = DIOSTREAMTLS_ALERT_DESCRIPTION_INTERNAL_ERROR;
      return false;
    }

  XDWORD tagsize = cipher[direction]->GetAEADTagSize();

  if(length <= tagsize)
    {
      lastalertdescription = DIOSTREAMTLS_ALERT_DESCRIPTION_BAD_RECORD_MAC;
      return false;
    }

  XBUFFER additionaldata;
  XBUFFER ciphertext;
  XBUFFER tag;
  XBUFFER nonce;

  if(!additionaldata.Add(record.Get(), DIOSTREAMTLS_MSG_RECORDHEADER_SIZE) ||
     !ciphertext.Add(&record.Get()[DIOSTREAMTLS_MSG_RECORDHEADER_SIZE], length - tagsize) ||
     !tag.Add(&record.Get()[DIOSTREAMTLS_MSG_RECORDHEADER_SIZE + length - tagsize], tagsize))
    {
      lastalertdescription = DIOSTREAMTLS_ALERT_DESCRIPTION_INTERNAL_ERROR;
      return false;
    }

  if(!CalculateNonce(direction, nonce))
    {
      lastalertdescription = DIOSTREAMTLS_ALERT_DESCRIPTION_INTERNAL_ERROR;
      return false;
    }

  if(!cipher[direction]->UncipherAEAD(ciphertext, nonce, additionaldata, tag))
    {
      lastalertdescription = DIOSTREAMTLS_ALERT_DESCRIPTION_BAD_RECORD_MAC;
      return false;
    }

  XBUFFER* inner = cipher[direction]->GetResult();
  XDWORD   size  = inner->GetSize();

  while(size)
    {
      if(inner->Get()[size-1]) break;
      size--;
    }

  if(!size)
    {
      lastalertdescription = DIOSTREAMTLS_ALERT_DESCRIPTION_UNEXPECTED_MESSAGE;
      return false;
    }

  contenttype = (DIOSTREAMTLS_CONTENTTYPE)inner->Get()[size-1];

  switch(contenttype)
    {
      case DIOSTREAMTLS_MSG_CONTENTTYPE_ALERT             :
      case DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE         :
      case DIOSTREAMTLS_MSG_CONTENTTYPE_APPLICATION_DATA  :
      case DIOSTREAMTLS_MSG_CONTENTTYPE_TLS_1_3_HEARTBEAT : break;

      default                                             : lastalertdescription = DIOSTREAMTLS_ALERT_DESCRIPTION_UNEXPECTED_MESSAGE;
                                                            return false;
    }

  if((size - 1) > DIOSTREAMTLSRECORD_MAXPLAINSIZE)
    {
      lastalertdescription = DIOSTREAMTLS_ALERT_DESCRIPTION_RECORD_OVERFLOW;
      return false;
    }

  if((size > 1) && !plain.Add(inner->Get(), size - 1))
    {
      lastalertdescription = DIOSTREAMTLS_ALERT_DESCRIPTION_INTERNAL_ERROR;
      return false;
    }

  sequence[direction]++;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTREAMTLS_ALERT_DESCRIPTION DIOSTREAMTLSRECORD::GetLastAlertDescription()
* @brief      Get the alert that best describes the last record-layer error
* @ingroup    DATAIO
*
* @return     DIOSTREAMTLS_ALERT_DESCRIPTION : Alert description.
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLS_ALERT_DESCRIPTION DIOSTREAMTLSRECORD::GetLastAlertDescription()
{
  return lastalertdescription;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSRECORD::Record_Extract(XBUFFER& input, XBUFFER& record)
* @brief      Take one complete record out of a stream buffer, leaving the rest untouched
* @note       STATIC. When the buffer does not hold a complete record yet, nothing is consumed and false is returned,
*             which is what lets the caller keep reading from a socket that delivers the record in pieces.
* @ingroup    DATAIO
*
* @param[in]  input : Buffer that accumulates what has been read from the stream.
* @param[out] record : Buffer that receives one complete record, header included.
*
* @return     bool : true if a complete record was taken out; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSRECORD::Record_Extract(XBUFFER& input, XBUFFER& record)
{
  DIOSTREAMTLS_MSG_RECORDHEADER header;

  record.Delete();

  if(!header.Peek(input))
    {
      return false;
    }

  if(header.GetLength() > DIOSTREAMTLSRECORD_MAXCIPHERSIZE)
    {
      return false;
    }

  XDWORD sizerecord = DIOSTREAMTLS_MSG_RECORDHEADER_SIZE + header.GetLength();

  if(input.GetSize() < sizerecord)
    {
      return false;
    }

  if(!record.Resize(sizerecord))
    {
      return false;
    }

  if(input.Extract(record.Get(), 0, sizerecord) != sizerecord)
    {
      record.Delete();
      return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSRECORD::CalculateNonce(DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction, XBUFFER& nonce)
* @brief      Nonce = write IV XOR the sequence number, right aligned and padded on the left with zeros
* @note       INTERNAL. RFC 8446, section 5.3.
* @ingroup    DATAIO
*
* @param[in]  direction : Local or remote.
* @param[out] nonce : Buffer that receives the nonce.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSRECORD::CalculateNonce(DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction, XBUFFER& nonce)
{
  if(direction >= DIOSTREAMTLSKEYSCHEDULE_MAXDIRECTIONS)
    {
      return false;
    }

  XDWORD sizeIV = IV[direction].GetSize();

  if(sizeIV < sizeof(XQWORD))
    {
      return false;
    }

  nonce.Delete();

  if(!nonce.Add(IV[direction]))
    {
      return false;
    }

  for(XDWORD c=0; c<sizeof(XQWORD); c++)
    {
      nonce.Get()[sizeIV - 1 - c] ^= (XBYTE)(sequence[direction] >> (8 * c));
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTREAMTLSRECORD::Protect_OneRecord(DIOSTREAMTLS_CONTENTTYPE contenttype, XBYTE* plain, XWORD size, XBUFFER& records)
* @brief      Build one record, in the clear or ciphered, and add it to the output
* @note       INTERNAL
* @ingroup    DATAIO
*
* @param[in]  contenttype : Type of the content.
* @param[in]  plain : Content pointer to use.
* @param[in]  size : Size of the content, already within the record limit.
* @param[out] records : Buffer that receives the record.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTLSRECORD::Protect_OneRecord(DIOSTREAMTLS_CONTENTTYPE contenttype, XBYTE* plain, XWORD size, XBUFFER& records)
{
  DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction = DIOSTREAMTLSKEYSCHEDULE_DIRECTION_LOCAL;

  DIOSTREAMTLS_MSG_RECORDHEADER header;

  header.SetProtocolVersion(DIOSTREAMTLSRECORD_LEGACYVERSION);

  if(!isprotected[direction])
    {
      XBUFFER encodedrecord;

      header.SetContenType(contenttype);
      header.SetLength(size);

      if(!header.SetToBuffer(encodedrecord, false)) return false;
      if(size && !encodedrecord.Add(plain, size))    return false;

      return records.Add(encodedrecord);
    }

  if(!cipher[direction])
    {
      return false;
    }

  if((keyschedule->GetCipherSuite() == DIOSTREAMTLS_MSG_CIPHER_AES_128_GCM_SHA256 ||
      keyschedule->GetCipherSuite() == DIOSTREAMTLS_MSG_CIPHER_AES_256_GCM_SHA384) &&
     sequence[direction] >= DIOSTREAMTLS_AESGCM_MAXKEYUSAGERECORDS)              // RFC 8446 / RFC 8446 key-usage bound for AES-GCM
    {
      return false;
    }

  if(sequence[direction] == 0xFFFFFFFFFFFFFFFFULL)                              // RFC 8446 section 5.3: the sequence number must never wrap
    {
      return false;
    }

  XBUFFER inner;
  XBUFFER additionaldata;
  XBUFFER nonce;
  XBUFFER tag;
  XDWORD  tagsize = cipher[direction]->GetAEADTagSize();

  if(size && !inner.Add(plain, size)) return false;

  if(!inner.Add((XBYTE)contenttype)) return false;                                                // The real type travels inside, RFC 8446 section 5.2

  for(XBYTE c=0; c<paddingsize; c++)
    {
      if(!inner.Add((XBYTE)0x00)) return false;
    }

  if((inner.GetSize() + tagsize) > DIOSTREAMTLSRECORD_MAXCIPHERSIZE)
    {
      return false;
    }

  header.SetContenType(DIOSTREAMTLS_MSG_CONTENTTYPE_APPLICATION_DATA);          // Outside, every protected record looks like application data
  header.SetLength((XWORD)(inner.GetSize() + tagsize));
  if(!header.SetToBuffer(additionaldata, false)) return false;                // The header of the record is also its authenticated data

  if(!CalculateNonce(direction, nonce))
    {
      return false;
    }

  if(!cipher[direction]->CipherAEAD(inner, nonce, additionaldata, tag))
    {
      return false;
    }

  /* XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("[TLS Record] Out %llu: type %02X, %d bytes"), sequence[direction], (XBYTE)contenttype, size); */

  XBUFFER encodedrecord;

  if(!encodedrecord.Add(additionaldata))               return false;
  if(!encodedrecord.Add(cipher[direction]->GetResult())) return false;
  if(!encodedrecord.Add(tag))                            return false;
  if(!records.Add(encodedrecord))                        return false;

  sequence[direction]++;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTREAMTLSRECORD::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTLSRECORD::Clean()
{
  keyschedule   = NULL;

  for(int c=0; c<DIOSTREAMTLSKEYSCHEDULE_MAXDIRECTIONS; c++)
    {
      cipher[c]       = NULL;
      key[c]          = NULL;
      sequence[c]     = 0;
      isprotected[c]  = false;
    }

  maxplainsize           = DIOSTREAMTLSRECORD_MAXPLAINSIZE;
  paddingsize              = 0;
  isini                    = false;
  lastalertdescription     = DIOSTREAMTLS_ALERT_DESCRIPTION_DECODE_ERROR;
}
