/**-------------------------------------------------------------------------------------------------------------------
*
* @file       CipherPEMCodec.cpp
*
* @class      CIPHERPEMCODEC
* @brief      Cipher PEM/DER private key codec class
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

#include "CipherPEMCodec.h"
#include "CipherAES.h"
#include "Cipher3DES.h"
#include "CipherKeySymmetrical.h"
#include "HashHMAC.h"
#include "HashSHA1.h"
#include "HashSHA2.h"
#include "CipherCertificateX509.h"

#include <string.h>



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

// rsaEncryption (1.2.840.113549.1.1.1) and id-ecPublicKey (1.2.840.10045.2.1), DER-encoded OBJECT IDENTIFIER
// content (i.e. without the leading 0x06 tag / length bytes) -- the only two PKCS#8 algorithms recognised.
static const XBYTE CIPHERPEMCODEC_OID_RSAENCRYPTION[] = { 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x01 };
static const XBYTE CIPHERPEMCODEC_OID_ECPUBLICKEY[]   = { 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x02, 0x01 };
static const XBYTE CIPHERPEMCODEC_OID_ED25519[]       = { 0x2B, 0x65, 0x70 };



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool CIPHERPEMCODEC::DER_ReadLength(XBYTE* data, XDWORD size, XDWORD& index, XDWORD& length)
* @brief      Read a DER length (short or long form) at data[index], advancing index past it.
* @note       INTERNAL. index/size are absolute offsets into data (size is an exclusive upper bound, not
*             necessarily the whole buffer's size -- this lets callers bound a read to a single nested SEQUENCE).
* @ingroup    CIPHER
* 
* @param[in]  data : Pointer to data.
* @param[in]  size : Size value.
* @param[in]  index : Index value.
* @param[in]  length : Length value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERPEMCODEC::DER_ReadLength(XBYTE* data, XDWORD size, XDWORD& index, XDWORD& length)
{
  XBYTE value;

  length = 0;

  if(!data || (index >= size)) return false;

  value = data[index++];
  if(!(value & 0x80))
    {
      length = value;
      return (length <= (size - index));
    }

  XDWORD nbytes = value & 0x7F;
  if(!nbytes || (nbytes > sizeof(XDWORD)) || (nbytes > (size - index))) return false;

  for(XDWORD c=0; c<nbytes; c++)
    {
      if(length > (0xFFFFFFFFUL >> 8)) return false;
      length = (length << 8) | data[index++];
    }

  return (length <= (size - index));
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool CIPHERPEMCODEC::DER_ReadTagLength(XBYTE* data, XDWORD size, XDWORD& index, XBYTE& tag, XDWORD& valueoffset, XDWORD& valuelength)
* @brief      Read a DER tag + length at data[index], advancing index past the whole TLV (tag+length+value).
* @note       INTERNAL
* @ingroup    CIPHER
* 
* @param[in]  data : Pointer to data.
* @param[in]  size : Size value.
* @param[in]  index : Index value.
* @param[in]  tag : Tag value.
* @param[in]  valueoffset : Valueoffset value.
* @param[in]  valuelength : Valuelength value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERPEMCODEC::DER_ReadTagLength(XBYTE* data, XDWORD size, XDWORD& index, XBYTE& tag, XDWORD& valueoffset, XDWORD& valuelength)
{
  if(!data || (index >= size)) return false;

  tag = data[index++];

  if(!DER_ReadLength(data, size, index, valuelength)) return false;

  valueoffset = index;
  index += valuelength;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool CIPHERPEMCODEC::DER_ReadInteger(XBYTE* data, XDWORD size, XDWORD& index, XMPINTEGER& value)
* @brief      Read a DER INTEGER at data[index] into value, advancing index past it.
* @note       INTERNAL. A leading 0x00 pad byte (present when the DER encoder needed to keep the value
*             non-negative) does not change the imported magnitude, so it is not stripped here.
* @ingroup    CIPHER
* 
* @param[in]  data : Pointer to data.
* @param[in]  size : Size value.
* @param[in]  index : Index value.
* @param[in]  value : Value value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERPEMCODEC::DER_ReadInteger(XBYTE* data, XDWORD size, XDWORD& index, XMPINTEGER& value)
{
  XBYTE  tag;
  XDWORD valueoffset;
  XDWORD valuelength;

  if(!DER_ReadTagLength(data, size, index, tag, valueoffset, valuelength) || (tag != 0x02) || !valuelength) return false;

  return value.ImportFromBinary(&data[valueoffset], valuelength);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool CIPHERPEMCODEC::IsPEM(XBUFFER& filedata)
* @brief      Is PEM (starts with "-----BEGIN ", ignoring leading whitespace)
* @ingroup    CIPHER
* 
* @param[in]  filedata : Filedata value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERPEMCODEC::IsPEM(XBUFFER& filedata)
{
  static const char marker[]  = "-----BEGIN ";
  XDWORD             markerlen = (XDWORD)(sizeof(marker) - 1);

  XBYTE* data = filedata.Get();
  XDWORD size = filedata.GetSize();
  XDWORD index = 0;

  if(!data || !size) return false;

  while((index < size) && ((data[index]==' ') || (data[index]=='\t') || (data[index]=='\r') || (data[index]=='\n'))) index++;

  if((size - index) < markerlen) return false;

  for(XDWORD c=0; c<markerlen; c++)
    {
      if(data[index + c] != (XBYTE)marker[c]) return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool CIPHERPEMCODEC::PrivateKeyBlock_Decode(XVECTOR<XSTRING*>& lines, XSTRING& blockheader, XBUFFER& der)
* @brief      Scan lines for the first complete "-----BEGIN xxx PRIVATE KEY----- ... -----END xxx PRIVATE KEY-----"
*             block and base64-decode its body into DER.
* @ingroup    CIPHER
* 
* @param[in]  lines : Pointer to lines.
* @param[in]  blockheader : Blockheader value.
* @param[in]  der : Der value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERPEMCODEC::PrivateKeyBlock_Decode(XVECTOR<XSTRING*>& lines, XSTRING& blockheader, XBUFFER& der)
{
  XSTRING keybase64;
  bool    inblock = false;

  blockheader.Empty();
  der.Delete();

  for(XDWORD c=0; c<lines.GetSize(); c++)
    {
      XSTRING* line = lines.Get(c);
      if(!line) continue;

      if(!inblock)
        {
          if((line->Find(__L("-----BEGIN "), true) != XSTRING_NOTFOUND) && (line->Find(__L("PRIVATE KEY-----"), true) != XSTRING_NOTFOUND))
            {
              blockheader = (*line);
              keybase64.Empty();
              inblock = true;
            }

          continue;
        }

      if(line->Find(__L("-----END "), true) != XSTRING_NOTFOUND)
        {
          if(keybase64.IsEmpty())
            {
              blockheader.Empty();
              return false;
            }

          if(!der.ConvertFromBase64(keybase64) || der.IsEmpty())
            {
              blockheader.Empty();
              der.Delete();
              return false;
            }

          return true;
        }

      if(!line->IsEmpty()) keybase64.Add(line->Get());
    }

  // BEGIN found but no matching END (or no BEGIN at all): incomplete/absent block.
  blockheader.Empty();
  der.Delete();

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool CIPHERPEMCODEC::RSAPrivateKey_Decode(XBUFFER& der, XMPINTEGER& prime1, XMPINTEGER& prime2, XMPINTEGER& exponent)
* @brief      Decode a PKCS#1 RSAPrivateKey DER blob into prime1/prime2/privateExponent.
* @ingroup    CIPHER
* 
* @param[in]  der : Der value.
* @param[in]  prime1 : Prime1 value.
* @param[in]  prime2 : Prime2 value.
* @param[in]  exponent : Exponent value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERPEMCODEC::RSAPrivateKey_Decode(XBUFFER& der, XMPINTEGER& prime1, XMPINTEGER& prime2, XMPINTEGER& exponent)
{
  XBYTE* data = der.Get();
  XDWORD size = der.GetSize();
  XDWORD index = 0;

  XBYTE  tag;
  XDWORD seqoffset;
  XDWORD seqlength;

  if(!data || !size) return false;
  if(!DER_ReadTagLength(data, size, index, tag, seqoffset, seqlength) || (tag != 0x30)) return false;

  XDWORD childindex = seqoffset;
  XDWORD seqend      = seqoffset + seqlength;

  XMPINTEGER version;
  XMPINTEGER modulus;
  XMPINTEGER publicexponent;

  // RSAPrivateKey ::= SEQUENCE { version, modulus, publicExponent, privateExponent, prime1, prime2,
  //                              exponent1, exponent2, coefficient, otherPrimeInfos OPTIONAL }
  // Only privateExponent/prime1/prime2 are needed by CIPHERKEYPRIVATERSA::Set() -- the rest are read (version,
  // modulus, publicExponent) or simply left unread (exponent1/exponent2/coefficient/otherPrimeInfos) as needed.
  if(!DER_ReadInteger(data, seqend, childindex, version))        return false;
  if(!DER_ReadInteger(data, seqend, childindex, modulus))        return false;
  if(!DER_ReadInteger(data, seqend, childindex, publicexponent)) return false;
  if(!DER_ReadInteger(data, seqend, childindex, exponent))       return false;
  if(!DER_ReadInteger(data, seqend, childindex, prime1))         return false;
  if(!DER_ReadInteger(data, seqend, childindex, prime2))         return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool CIPHERPEMCODEC::ECPrivateKey_Decode(XBUFFER& der, XBUFFER& privatekey)
* @brief      Decode a SEC1 ECPrivateKey DER blob into its privateKey OCTET STRING (the scalar D).
* @ingroup    CIPHER
* 
* @param[in]  der : Der value.
* @param[in]  privatekey : Privatekey value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERPEMCODEC::ECPrivateKey_Decode(XBUFFER& der, XBUFFER& privatekey)
{
  XBYTE* data = der.Get();
  XDWORD size = der.GetSize();
  XDWORD index = 0;

  XBYTE  tag;
  XDWORD seqoffset;
  XDWORD seqlength;

  privatekey.Delete();

  if(!data || !size) return false;
  if(!DER_ReadTagLength(data, size, index, tag, seqoffset, seqlength) || (tag != 0x30)) return false;

  XDWORD childindex = seqoffset;
  XDWORD seqend      = seqoffset + seqlength;

  XDWORD childoffset;
  XDWORD childlength;

  // ECPrivateKey ::= SEQUENCE { version INTEGER, privateKey OCTET STRING, parameters [0] OPTIONAL, publicKey [1] OPTIONAL }
  if(!DER_ReadTagLength(data, seqend, childindex, tag, childoffset, childlength) || (tag != 0x02)) return false; // version
  if(!DER_ReadTagLength(data, seqend, childindex, tag, childoffset, childlength) || (tag != 0x04) || !childlength) return false; // privateKey

  return privatekey.Add(&data[childoffset], childlength);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERPEMCODEC::Ed25519PrivateKey_Decode(XBUFFER& der, XBUFFER& privatekey)
* @brief      Ed25519 private key decode
* @ingroup    CIPHER
* 
* @param[in]  der : Der value.
* @param[in]  privatekey : Privatekey value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERPEMCODEC::Ed25519PrivateKey_Decode(XBUFFER& der, XBUFFER& privatekey)
{
  XBYTE* data=der.Get();
  XDWORD size=der.GetSize();
  XDWORD index=0;
  XBYTE tag;
  XDWORD offset;
  XDWORD length;

  privatekey.SecureDelete();

  // RFC 8410 CurvePrivateKey ::= OCTET STRING.  The outer PKCS#8 privateKey OCTET STRING
  // has already been removed by PKCS8PrivateKey_Decode(), leaving this exact 32-byte wrapper.
  if(!data || !DER_ReadTagLength(data, size, index, tag, offset, length) || tag!=0x04 ||
     length!=32 || index!=size)
    {
      return false;
    }

  return privatekey.Add(&data[offset], length);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool CIPHERPEMCODEC::PKCS8PrivateKey_Decode(XBUFFER& der, bool& isrsa, bool& isec,
*                                                         bool& ised25519, XBUFFER& innerkey)
* @brief      Decode an unencrypted PKCS#8 PrivateKeyInfo DER blob: identify its algorithm and return the nested
*             RSAPrivateKey / ECPrivateKey DER blob.
* @ingroup    CIPHER
* 
* @param[in]  der : Der value.
* @param[in]  isrsa : Isrsa value.
* @param[in]  isec : Isec value.
* @param[in]  ised25519 : Ised25519 value.
* @param[in]  innerkey : Innerkey value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERPEMCODEC::PKCS8PrivateKey_Decode(XBUFFER& der, bool& isrsa, bool& isec, bool& ised25519, XBUFFER& innerkey)
{
  XBYTE* data = der.Get();
  XDWORD size = der.GetSize();
  XDWORD index = 0;

  XBYTE  tag;
  XDWORD seqoffset;
  XDWORD seqlength;

  isrsa = false;
  isec  = false;
  ised25519 = false;
  innerkey.Delete();

  if(!data || !size) return false;
  if(!DER_ReadTagLength(data, size, index, tag, seqoffset, seqlength) || (tag != 0x30)) return false;

  XDWORD childindex = seqoffset;
  XDWORD seqend      = seqoffset + seqlength;

  XDWORD childoffset;
  XDWORD childlength;

  // PrivateKeyInfo ::= SEQUENCE { version INTEGER, privateKeyAlgorithm AlgorithmIdentifier, privateKey OCTET STRING, ... }
  if(!DER_ReadTagLength(data, seqend, childindex, tag, childoffset, childlength) || (tag != 0x02)) return false; // version

  // privateKeyAlgorithm ::= SEQUENCE { algorithm OBJECT IDENTIFIER, parameters ANY OPTIONAL }
  XDWORD algseqoffset;
  XDWORD algseqlength;

  if(!DER_ReadTagLength(data, seqend, childindex, tag, algseqoffset, algseqlength) || (tag != 0x30)) return false;

  XDWORD algindex = algseqoffset;
  XDWORD algseqend = algseqoffset + algseqlength;

  XDWORD oidoffset;
  XDWORD oidlength;

  if(!DER_ReadTagLength(data, algseqend, algindex, tag, oidoffset, oidlength) || (tag != 0x06) || !oidlength) return false;

  if((oidlength == sizeof(CIPHERPEMCODEC_OID_RSAENCRYPTION)) && !memcmp(&data[oidoffset], CIPHERPEMCODEC_OID_RSAENCRYPTION, oidlength))
    {
      isrsa = true;
    }
   else if((oidlength == sizeof(CIPHERPEMCODEC_OID_ECPUBLICKEY)) && !memcmp(&data[oidoffset], CIPHERPEMCODEC_OID_ECPUBLICKEY, oidlength))
    {
      isec = true;
    }
   else if((oidlength == sizeof(CIPHERPEMCODEC_OID_ED25519)) && !memcmp(&data[oidoffset], CIPHERPEMCODEC_OID_ED25519, oidlength))
    {
      // RFC 8410 requires AlgorithmIdentifier parameters to be absent for Ed25519.
      if(algindex != algseqend) return false;
      ised25519 = true;
    }
   else
    {
      return false; // Unrecognised/unsupported algorithm OID.
    }

  // privateKey OCTET STRING -- contains the nested RSAPrivateKey / ECPrivateKey DER blob.
  if(!DER_ReadTagLength(data, seqend, childindex, tag, childoffset, childlength) || (tag != 0x04) || !childlength) return false;

  return innerkey.Add(&data[childoffset], childlength);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERPEMCODEC_IntegerToDWORD(XBYTE* data, XDWORD size, XDWORD& value)
* @brief      Integer to dword
* @ingroup    CIPHER
* 
* @param[in]  data : Pointer to data.
* @param[in]  size : Size value.
* @param[in]  value : Value value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool CIPHERPEMCODEC_IntegerToDWORD(XBYTE* data, XDWORD size, XDWORD& value)
{
  value = 0;
  if(!data || !size || (size > 4) || (data[0] & 0x80)) return false;
  for(XDWORD c=0; c<size; c++) value = (value << 8) | data[c];
  return true;
}

static bool CIPHERPEMCODEC_PKCS12KDF(XSTRING& password, XBYTE id, XBUFFER& salt, XDWORD iterations, HASHTYPE type, XDWORD outputsize, XSECUREBUFFER& output);

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERPEMCODEC_PBKDF2(XBUFFER& password, XBUFFER& salt, XDWORD iterations, HASHTYPE hashtype, XDWORD keysize, XSECUREBUFFER& key)
* @brief      Pbkdf2
* @ingroup    CIPHER
* 
* @param[in]  password : Password value.
* @param[in]  salt : Salt value.
* @param[in]  iterations : Iterations value.
* @param[in]  hashtype : Hashtype value.
* @param[in]  keysize : Keysize value.
* @param[in]  key : Key value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool CIPHERPEMCODEC_PBKDF2(XBUFFER& password, XBUFFER& salt, XDWORD iterations, HASHTYPE hashtype, XDWORD keysize, XSECUREBUFFER& key)
{
  key.SecureDelete();
  if(!iterations || !keysize) return false;

  XDWORD block = 1;
  while(key.GetSize() < keysize)
    {
      HASH* hash = NULL;
      switch(hashtype)
        {
          case HASHTYPE_SHA1   : hash = GEN_NEW HASHSHA1(); break;
          case HASHTYPE_SHA256 : hash = GEN_NEW HASHSHA2(HASHSHA2TYPE_256); break;
          case HASHTYPE_SHA384 : hash = GEN_NEW HASHSHA2(HASHSHA2TYPE_384); break;
          case HASHTYPE_SHA512 : hash = GEN_NEW HASHSHA2(HASHSHA2TYPE_512); break;
                       default : return false;
        }

      HASHHMAC hmac(hash);
      XBUFFER input;
      XSECUREBUFFER U;
      XSECUREBUFFER T;
      XBYTE counter[4] = { (XBYTE)(block >> 24), (XBYTE)(block >> 16), (XBYTE)(block >> 8), (XBYTE)block };
      bool status = input.Add(salt) && input.Add(counter, sizeof(counter)) && hmac.SetKey(password) && hmac.Do(input) &&
                    U.Add((*hmac.GetResult())) && T.Add(U);

      for(XDWORD c=1; status && c<iterations; c++)
        {
          hmac.ResetResult();
          status = hmac.Do(U);
          if(status)
            {
              XBUFFER* digest = hmac.GetResult();
              if(!digest || (digest->GetSize() != T.GetSize())) status = false;
              else
                {
                  U.SecureDelete();
                  U.Add((*digest));
                  for(XDWORD n=0; n<T.GetSize(); n++) T.Get()[n] ^= U.Get()[n];
                }
            }
        }

      if(status)
        {
          XDWORD remaining = keysize - key.GetSize();
          status = key.Add(T.Get(), remaining<T.GetSize()?remaining:T.GetSize());
        }

      U.SecureDelete(); T.SecureDelete(); input.SecureDelete();
      GEN_DELETE hash;
      if(!status) { key.SecureDelete(); return false; }
      block++;
    }
  return true;
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERPEMCODEC::PKCS8EncryptedPrivateKey_Decode(XBUFFER& der, XSTRING& password, XSECUREBUFFER& privatekeyinfo)
* @brief      Pkcs8 encrypted private key decode
* @ingroup    CIPHER
* 
* @param[in]  der : Der value.
* @param[in]  password : Password value.
* @param[in]  privatekeyinfo : Privatekeyinfo value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERPEMCODEC::PKCS8EncryptedPrivateKey_Decode(XBUFFER& der, XSTRING& password, XSECUREBUFFER& privatekeyinfo)
{
  static const XBYTE OIDPBES2[]      = { 0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,0x05,0x0D };
  static const XBYTE OIDPKCS12PBE3DES[] = { 0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,0x0C,0x01,0x03 };
  static const XBYTE OIDPBKDF2[]     = { 0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,0x05,0x0C };
  static const XBYTE OIDHMACSHA1[]   = { 0x2A,0x86,0x48,0x86,0xF7,0x0D,0x02,0x07 };
  static const XBYTE OIDHMACSHA256[] = { 0x2A,0x86,0x48,0x86,0xF7,0x0D,0x02,0x09 };
  static const XBYTE OIDHMACSHA384[] = { 0x2A,0x86,0x48,0x86,0xF7,0x0D,0x02,0x0A };
  static const XBYTE OIDHMACSHA512[] = { 0x2A,0x86,0x48,0x86,0xF7,0x0D,0x02,0x0B };
  static const XBYTE OIDAES128CBC[]  = { 0x60,0x86,0x48,0x01,0x65,0x03,0x04,0x01,0x02 };
  static const XBYTE OIDAES192CBC[]  = { 0x60,0x86,0x48,0x01,0x65,0x03,0x04,0x01,0x16 };
  static const XBYTE OIDAES256CBC[]  = { 0x60,0x86,0x48,0x01,0x65,0x03,0x04,0x01,0x2A };

  privatekeyinfo.SecureDelete();
  XBYTE* data=der.Get(); XDWORD size=der.GetSize(), index=0, offset=0, length=0; XBYTE tag=0;
  if(!DER_ReadTagLength(data, size, index, tag, offset, length) || tag!=0x30 || index!=size) return false;
  XDWORD top=offset, topend=offset+length, algoff=0, alglen=0, encdataoff=0, encdatalen=0;
  if(!DER_ReadTagLength(data, topend, top, tag, algoff, alglen) || tag!=0x30 ||
     !DER_ReadTagLength(data, topend, top, tag, encdataoff, encdatalen) || tag!=0x04 || top!=topend) return false;

  XDWORD ai=algoff, aiend=algoff+alglen, oidoff=0, oidlen=0, paramoff=0, paramlen=0;
  if(!DER_ReadTagLength(data, aiend, ai, tag, oidoff, oidlen) || tag!=0x06 ||
     !DER_ReadTagLength(data, aiend, ai, tag, paramoff, paramlen) || tag!=0x30 || ai!=aiend) return false;

  if(oidlen==sizeof(OIDPKCS12PBE3DES) && !memcmp(data+oidoff, OIDPKCS12PBE3DES, oidlen))
    {
      XDWORD li=paramoff,liend=paramoff+paramlen,saltoff=0,saltlen=0,iteroff=0,iterlen=0,iterations=0;
      if(!DER_ReadTagLength(data, liend, li, tag, saltoff, saltlen)||tag!=0x04||!saltlen||
         !DER_ReadTagLength(data, liend, li, tag, iteroff, iterlen)||tag!=0x02||li!=liend||
         !CIPHERPEMCODEC_IntegerToDWORD(data+iteroff, iterlen, iterations)||!iterations)return false;
      XBUFFER salt;salt.Add(data+saltoff, saltlen);XSECUREBUFFER keybytes;XSECUREBUFFER ivbytes;
      bool status=CIPHERPEMCODEC_PKCS12KDF(password, 1, salt, iterations, HASHTYPE_SHA1, 24, keybytes)&&
                  CIPHERPEMCODEC_PKCS12KDF(password, 2, salt, iterations, HASHTYPE_SHA1, 8, ivbytes);
      if(status)
        {
          CIPHERKEYSYMMETRICAL key;CIPHER3DES cipher;status=key.Set(keybytes)&&cipher.SetKey(&key)&&cipher.SetInitVector(ivbytes);
          cipher.SetChainingMode(CIPHERCHAININGMODE_CBC);cipher.SetPaddingType(XBUFFER_PADDINGTYPE_PKCS7);cipher.SetPaddingAdjustSize(8);
          if(status)status=cipher.Uncipher(data+encdataoff, encdatalen)&&privatekeyinfo.Add((*cipher.GetResult()));
        }
      keybytes.SecureDelete();ivbytes.SecureDelete();if(!status)privatekeyinfo.SecureDelete();return status;
    }

  if(oidlen!=sizeof(OIDPBES2) || memcmp(data+oidoff, OIDPBES2, oidlen)) return false;

  XDWORD pi=paramoff, piend=paramoff+paramlen, kdfoff=0,kdflen=0, esoff=0,eslen=0;
  if(!DER_ReadTagLength(data, piend, pi, tag, kdfoff, kdflen) || tag!=0x30 ||
     !DER_ReadTagLength(data, piend, pi, tag, esoff, eslen) || tag!=0x30 || pi!=piend) return false;

  XDWORD ki=kdfoff, kiend=kdfoff+kdflen, kdfOIDoff=0,kdfOIDlen=0,kdfparamoff=0,kdfparamlen=0;
  if(!DER_ReadTagLength(data, kiend, ki, tag, kdfOIDoff, kdfOIDlen) || tag!=0x06 || kdfOIDlen!=sizeof(OIDPBKDF2) || memcmp(data+kdfOIDoff, OIDPBKDF2, kdfOIDlen) ||
     !DER_ReadTagLength(data, kiend, ki, tag, kdfparamoff, kdfparamlen) || tag!=0x30 || ki!=kiend) return false;

  XDWORD kpi=kdfparamoff,kpiend=kdfparamoff+kdfparamlen,saltoff=0,saltlen=0,iteroff=0,iterlen=0;
  if(!DER_ReadTagLength(data, kpiend, kpi, tag, saltoff, saltlen) || tag!=0x04 || !saltlen ||
     !DER_ReadTagLength(data, kpiend, kpi, tag, iteroff, iterlen) || tag!=0x02) return false;
  XDWORD iterations=0,keylength=0;
  if(!CIPHERPEMCODEC_IntegerToDWORD(data+iteroff, iterlen, iterations) || !iterations) return false;
  HASHTYPE hashtype=HASHTYPE_SHA1;
  if(kpi<kpiend)
    {
      XDWORD save=kpi,vo=0,vl=0;
      if(!DER_ReadTagLength(data, kpiend, kpi, tag, vo, vl)) return false;
      if(tag==0x02) { if(!CIPHERPEMCODEC_IntegerToDWORD(data+vo, vl, keylength)) return false; }
      else kpi=save;
    }
  if(kpi<kpiend)
    {
      XDWORD proff=0,prlen=0;
      if(!DER_ReadTagLength(data, kpiend, kpi, tag, proff, prlen) || tag!=0x30) return false;
      XDWORD pri=proff,priend=proff+prlen,proidoff=0,proidlen=0;
      if(!DER_ReadTagLength(data, priend, pri, tag, proidoff, proidlen) || tag!=0x06) return false;
      if(proidlen==sizeof(OIDHMACSHA1) && !memcmp(data+proidoff, OIDHMACSHA1, proidlen)) hashtype=HASHTYPE_SHA1;
      else if(proidlen==sizeof(OIDHMACSHA256) && !memcmp(data+proidoff, OIDHMACSHA256, proidlen)) hashtype=HASHTYPE_SHA256;
      else if(proidlen==sizeof(OIDHMACSHA384) && !memcmp(data+proidoff, OIDHMACSHA384, proidlen)) hashtype=HASHTYPE_SHA384;
      else if(proidlen==sizeof(OIDHMACSHA512) && !memcmp(data+proidoff, OIDHMACSHA512, proidlen)) hashtype=HASHTYPE_SHA512;
      else return false;
    }
  if(kpi!=kpiend) return false;

  XDWORD esi=esoff,esiend=esoff+eslen,esoidoff=0,esoidlen=0,ivoff=0,ivlen=0,aeskeysize=0;
  if(!DER_ReadTagLength(data, esiend, esi, tag, esoidoff, esoidlen) || tag!=0x06 ||
     !DER_ReadTagLength(data, esiend, esi, tag, ivoff, ivlen) || tag!=0x04 || ivlen!=16 || esi!=esiend) return false;
  if(esoidlen==sizeof(OIDAES128CBC) && !memcmp(data+esoidoff, OIDAES128CBC, esoidlen)) aeskeysize=16;
  else if(esoidlen==sizeof(OIDAES192CBC) && !memcmp(data+esoidoff, OIDAES192CBC, esoidlen)) aeskeysize=24;
  else if(esoidlen==sizeof(OIDAES256CBC) && !memcmp(data+esoidoff, OIDAES256CBC, esoidlen)) aeskeysize=32;
  else return false;
  if(keylength && keylength!=aeskeysize) return false;

  XSECUREBUFFER passwordbytes; XBUFFER salt; XBUFFER iv;
  for(XDWORD c=0;c<password.GetSize();c++) { if(password.Get()[c]>0x7F) return false; passwordbytes.Add((XBYTE)password.Get()[c]); }
  salt.Add(data+saltoff, saltlen); iv.Add(data+ivoff, ivlen);
  XSECUREBUFFER derivedkey;
  bool status=CIPHERPEMCODEC_PBKDF2(passwordbytes, salt, iterations, hashtype, aeskeysize, derivedkey);
  if(status)
    {
      CIPHERKEYSYMMETRICAL key; CIPHERAES aes;
      status=key.Set(derivedkey) && aes.SetKey(&key) && aes.SetInitVector(iv);
      aes.SetChainingMode(CIPHERCHAININGMODE_CBC); aes.SetPaddingType(XBUFFER_PADDINGTYPE_PKCS7); aes.SetPaddingAdjustSize(16);
      if(status) status=aes.Uncipher(data+encdataoff, encdatalen) && privatekeyinfo.Add((*aes.GetResult()));
    }
  passwordbytes.SecureDelete(); derivedkey.SecureDelete();
  if(!status) privatekeyinfo.SecureDelete();
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERPEMCODEC::PKCS7Certificates_Decode(XBUFFER& der, XVECTOR<XBUFFER*>& certificates)
* @brief      Pkcs7 certificates decode
* @ingroup    CIPHER
* 
* @param[in]  der : Der value.
* @param[in]  certificates : Pointer to certificates.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERPEMCODEC::PKCS7Certificates_Decode(XBUFFER& der, XVECTOR<XBUFFER*>& certificates)
{
  static const XBYTE OIDSIGNEDDATA[] = { 0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,0x07,0x02 };
  certificates.DeleteContents(); certificates.DeleteAll();
  XBYTE* data=der.Get(); XDWORD size=der.GetSize(),i=0,o=0,l=0; XBYTE tag=0;
  if(!DER_ReadTagLength(data, size, i, tag, o, l) || tag!=0x30 || i!=size) return false;
  XDWORD ci=o,ciend=o+l,oido=0,oidl=0,contento=0,contentl=0;
  if(!DER_ReadTagLength(data, ciend, ci, tag, oido, oidl) || tag!=0x06 || oidl!=sizeof(OIDSIGNEDDATA) || memcmp(data+oido, OIDSIGNEDDATA, oidl) ||
     !DER_ReadTagLength(data, ciend, ci, tag, contento, contentl) || tag!=0xA0 || ci!=ciend) return false;
  XDWORD ex=contento,exend=contento+contentl,sdo=0,sdl=0;
  if(!DER_ReadTagLength(data, exend, ex, tag, sdo, sdl) || tag!=0x30 || ex!=exend) return false;
  XDWORD sd=sdo,sdend=sdo+sdl,vo=0,vl=0;
  if(!DER_ReadTagLength(data, sdend, sd, tag, vo, vl) || tag!=0x02 ||
     !DER_ReadTagLength(data, sdend, sd, tag, vo, vl) || tag!=0x31 ||
     !DER_ReadTagLength(data, sdend, sd, tag, vo, vl) || tag!=0x30) return false;
  if(sd>=sdend) return false;
  XDWORD certseto=0,certsetl=0;
  if(!DER_ReadTagLength(data, sdend, sd, tag, certseto, certsetl) || tag!=0xA0) return false;
  XDWORD cs=certseto,csend=certseto+certsetl;
  while(cs<csend)
    {
      XDWORD start=cs,co=0,cl=0;
      if(!DER_ReadTagLength(data, csend, cs, tag, co, cl) || tag!=0x30) { certificates.DeleteContents(); certificates.DeleteAll(); return false; }
      XBUFFER candidate; candidate.Add(data+start, cs-start);
      CIPHERCERTIFICATEX509 parsed;
      if(!parsed.Decode(candidate)) continue; // CertificateChoices can also contain tagged non-X.509 choices.
      XBUFFER* copy=GEN_NEW XBUFFER();
      if(!copy || !copy->Add(candidate) || !certificates.Add(copy))
        { if(copy) GEN_DELETE copy; certificates.DeleteContents(); certificates.DeleteAll(); return false; }
    }
  return !certificates.IsEmpty();
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERPEMCODEC_DERLength(XBUFFER& output, XDWORD length)
* @brief      Der length
* @ingroup    CIPHER
* 
* @param[out] output : Output value.
* @param[in]  length : Length value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool CIPHERPEMCODEC_DERLength(XBUFFER& output, XDWORD length)
{
  if(length<128) return output.Add((XBYTE)length);
  XBYTE bytes[4]; XDWORD n=0,value=length; while(value){bytes[3-n++]=(XBYTE)value;value>>=8;}
  if(!output.Add((XBYTE)(0x80|n)))return false; return output.Add(bytes+4-n, n);
}
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERPEMCODEC_DERWrap(XBYTE tag, XBUFFER& content, XBUFFER& output)
* @brief      Der wrap
* @ingroup    CIPHER
* 
* @param[in]  tag : Tag value.
* @param[in]  content : Content value.
* @param[out] output : Output value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool CIPHERPEMCODEC_DERWrap(XBYTE tag, XBUFFER& content, XBUFFER& output)
{
  return output.Add(tag)&&CIPHERPEMCODEC_DERLength(output, content.GetSize())&&output.Add(content);
}
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERPEMCODEC_PKCS12SafeContents(XBUFFER& safe, XSTRING& password, XVECTOR<XBUFFER*>& certificates, XSECUREBUFFER& privatekeyinfo)
* @brief      Pkcs12 safe contents
* @ingroup    CIPHER
* 
* @param[in]  safe : Safe value.
* @param[in]  password : Password value.
* @param[in]  certificates : Pointer to certificates.
* @param[in]  privatekeyinfo : Privatekeyinfo value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool CIPHERPEMCODEC_PKCS12SafeContents(XBUFFER& safe, XSTRING& password, XVECTOR<XBUFFER*>& certificates, XSECUREBUFFER& privatekeyinfo)
{
  static const XBYTE OIDKEYBAG[]={0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,0x0C,0x0A,0x01,0x01};
  static const XBYTE OIDSHROUDED[]={0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,0x0C,0x0A,0x01,0x02};
  static const XBYTE OIDCERTBAG[]={0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,0x0C,0x0A,0x01,0x03};
  static const XBYTE OIDX509CERT[]={0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,0x09,0x16,0x01};
  XBYTE* data=safe.Get(); XDWORD size=safe.GetSize(),i=0,o=0,l=0; XBYTE tag=0;
  if(!CIPHERPEMCODEC::DER_ReadTagLength(data, size, i, tag, o, l)||tag!=0x30||i!=size)return false;
  XDWORD si=o,siend=o+l;
  while(si<siend)
    {
      XDWORD bago=0,bagl=0; if(!CIPHERPEMCODEC::DER_ReadTagLength(data, siend, si, tag, bago, bagl)||tag!=0x30)return false;
      XDWORD bi=bago,biend=bago+bagl,oido=0,oidl=0,valueo=0,valuel=0;
      if(!CIPHERPEMCODEC::DER_ReadTagLength(data, biend, bi, tag, oido, oidl)||tag!=0x06||
         !CIPHERPEMCODEC::DER_ReadTagLength(data, biend, bi, tag, valueo, valuel)||tag!=0xA0)return false;
      XDWORD vi=valueo,viend=valueo+valuel,innero=0,innerl=0,start=vi;
      if(!CIPHERPEMCODEC::DER_ReadTagLength(data, viend, vi, tag, innero, innerl)||vi!=viend)return false;
      XBUFFER encoded; encoded.Add(data+start, vi-start);
      if(oidl==sizeof(OIDKEYBAG)&&!memcmp(data+oido, OIDKEYBAG, oidl))
        { if(!privatekeyinfo.IsEmpty()||!privatekeyinfo.Add(encoded))return false; }
      else if(oidl==sizeof(OIDSHROUDED)&&!memcmp(data+oido, OIDSHROUDED, oidl))
        { if(!privatekeyinfo.IsEmpty()||!CIPHERPEMCODEC::PKCS8EncryptedPrivateKey_Decode(encoded, password, privatekeyinfo))return false; }
      else if(oidl==sizeof(OIDCERTBAG)&&!memcmp(data+oido, OIDCERTBAG, oidl))
        {
          if(tag!=0x30)return false; XDWORD ci=innero,ciend=innero+innerl,cido=0,cidl=0,cvo=0,cvl=0;
          if(!CIPHERPEMCODEC::DER_ReadTagLength(data, ciend, ci, tag, cido, cidl)||tag!=0x06||cidl!=sizeof(OIDX509CERT)||memcmp(data+cido, OIDX509CERT, cidl)||
             !CIPHERPEMCODEC::DER_ReadTagLength(data, ciend, ci, tag, cvo, cvl)||tag!=0xA0||ci!=ciend)return false;
          XDWORD cvi=cvo,cviend=cvo+cvl,co=0,cl=0; if(!CIPHERPEMCODEC::DER_ReadTagLength(data, cviend, cvi, tag, co, cl)||tag!=0x04||cvi!=cviend)return false;
          XBUFFER candidate; candidate.Add(data+co, cl); CIPHERCERTIFICATEX509 cert; if(!cert.Decode(candidate))return false;
          XBUFFER* copy=GEN_NEW XBUFFER(); if(!copy||!copy->Add(candidate)||!certificates.Add(copy)){if(copy)GEN_DELETE copy;return false;}
        }
    }
  return true;
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         HASH* CIPHERPEMCODEC_HashCreate(HASHTYPE type)
* @brief      Hash create
* @ingroup    CIPHER
* 
* @param[in]  type : Type value.
* 
* @return     HASH* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static HASH* CIPHERPEMCODEC_HashCreate(HASHTYPE type)
{
  switch(type)
    {
      case HASHTYPE_SHA1:return GEN_NEW HASHSHA1();
      case HASHTYPE_SHA256:return GEN_NEW HASHSHA2(HASHSHA2TYPE_256);
      case HASHTYPE_SHA384:return GEN_NEW HASHSHA2(HASHSHA2TYPE_384);
      case HASHTYPE_SHA512:return GEN_NEW HASHSHA2(HASHSHA2TYPE_512);
                   default:return NULL;
    }
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERPEMCODEC_PKCS12KDF(XSTRING& password, XBYTE id, XBUFFER& salt, XDWORD iterations, HASHTYPE type, XDWORD outputsize, XSECUREBUFFER& output)
* @brief      Pkcs12 kdf
* @ingroup    CIPHER
* 
* @param[in]  password : Password value.
* @param[in]  id : Id value.
* @param[in]  salt : Salt value.
* @param[in]  iterations : Iterations value.
* @param[in]  type : Type value.
* @param[in]  outputsize : Outputsize value.
* @param[out] output : Output value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool CIPHERPEMCODEC_PKCS12KDF(XSTRING& password, XBYTE id, XBUFFER& salt, XDWORD iterations, HASHTYPE type, XDWORD outputsize, XSECUREBUFFER& output)
{
  output.SecureDelete(); HASH* hash=CIPHERPEMCODEC_HashCreate(type); if(!hash||!iterations||!outputsize){if(hash)GEN_DELETE hash;return false;}
  XDWORD u=(XDWORD)hash->GetDefaultSize(); XDWORD v=(type==HASHTYPE_SHA384||type==HASHTYPE_SHA512)?128:64;
  XSECUREBUFFER passwordbytes;
  for(XDWORD c=0;c<password.GetSize();c++){XDWORD ch=(XDWORD)password.Get()[c];if(ch>0xFFFF){GEN_DELETE hash;return false;}passwordbytes.Add((XBYTE)(ch>>8));passwordbytes.Add((XBYTE)ch);}
  passwordbytes.Add((XBYTE)0);passwordbytes.Add((XBYTE)0);
  XBUFFER D;D.Resize(v);D.FillBuffer(id);XSECUREBUFFER I;
  if(!salt.IsEmpty()){XDWORD n=v*((salt.GetSize()+v-1)/v);for(XDWORD c=0;c<n;c++)I.Add(salt.Get()[c%salt.GetSize()]);}
  {XDWORD n=v*((passwordbytes.GetSize()+v-1)/v);for(XDWORD c=0;c<n;c++)I.Add(passwordbytes.Get()[c%passwordbytes.GetSize()]);}
  bool status=true;
  while(status&&output.GetSize()<outputsize)
    {
      XSECUREBUFFER A;XBUFFER input;input.Add(D);input.Add(I);hash->ResetResult();status=hash->Do(input)&&A.Add((*hash->GetResult()));
      for(XDWORD n=1;status&&n<iterations;n++){hash->ResetResult();status=hash->Do(A);if(status){A.SecureDelete();A.Add((*hash->GetResult()));}}
      XSECUREBUFFER B;if(status)for(XDWORD c=0;c<v;c++)B.Add(A.Get()[c%u]);
      for(XDWORD block=0;status&&block<I.GetSize();block+=v)
        {XDWORD carry=1;for(int n=(int)v-1;n>=0;n--){XDWORD sum=(XDWORD)I.Get()[block+n]+B.Get()[n]+carry;I.Get()[block+n]=(XBYTE)sum;carry=sum>>8;}}
      if(status){XDWORD remaining=outputsize-output.GetSize();status=output.Add(A.Get(), remaining<u?remaining:u);}A.SecureDelete();B.SecureDelete();input.SecureDelete();
    }
  passwordbytes.SecureDelete();I.SecureDelete();GEN_DELETE hash;if(!status)output.SecureDelete();return status;
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERPEMCODEC_PKCS12MACVerify(XBYTE* authsafe, XDWORD authsafesize, XBYTE* data, XDWORD size, XSTRING& password)
* @brief      Pkcs12 mac verify
* @ingroup    CIPHER
* 
* @param[in]  authsafe : Pointer to authsafe.
* @param[in]  authsafesize : Authsafesize value.
* @param[in]  data : Pointer to data.
* @param[in]  size : Size value.
* @param[in]  password : Password value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool CIPHERPEMCODEC_PKCS12MACVerify(XBYTE* authsafe, XDWORD authsafesize, XBYTE* data, XDWORD size, XSTRING& password)
{
  static const XBYTE SHA1OID[]={0x2B,0x0E,0x03,0x02,0x1A};
  static const XBYTE SHA256OID[]={0x60,0x86,0x48,0x01,0x65,0x03,0x04,0x02,0x01};
  static const XBYTE SHA384OID[]={0x60,0x86,0x48,0x01,0x65,0x03,0x04,0x02,0x02};
  static const XBYTE SHA512OID[]={0x60,0x86,0x48,0x01,0x65,0x03,0x04,0x02,0x03};
  XDWORD i=0,o=0,l=0;XBYTE tag=0;if(!CIPHERPEMCODEC::DER_ReadTagLength(data, size, i, tag, o, l)||tag!=0x30||i!=size)return false;
  XDWORD mi=o,miend=o+l,dio=0,dil=0,salto=0,saltl=0,iterations=1;
  if(!CIPHERPEMCODEC::DER_ReadTagLength(data, miend, mi, tag, dio, dil)||tag!=0x30||
     !CIPHERPEMCODEC::DER_ReadTagLength(data, miend, mi, tag, salto, saltl)||tag!=0x04||!saltl)return false;
  if(mi<miend){XDWORD ito=0,itl=0;if(!CIPHERPEMCODEC::DER_ReadTagLength(data, miend, mi, tag, ito, itl)||tag!=0x02||!CIPHERPEMCODEC_IntegerToDWORD(data+ito, itl, iterations)||!iterations)return false;}if(mi!=miend)return false;
  XDWORD di=dio,diend=dio+dil,aio=0,ail=0,digesto=0,digestl=0;if(!CIPHERPEMCODEC::DER_ReadTagLength(data, diend, di, tag, aio, ail)||tag!=0x30||
     !CIPHERPEMCODEC::DER_ReadTagLength(data, diend, di, tag, digesto, digestl)||tag!=0x04||di!=diend)return false;
  XDWORD ai=aio,aiend=aio+ail,oido=0,oidl=0;if(!CIPHERPEMCODEC::DER_ReadTagLength(data, aiend, ai, tag, oido, oidl)||tag!=0x06)return false;
  HASHTYPE type=HASHTYPE_NONE;if(oidl==sizeof(SHA1OID)&&!memcmp(data+oido, SHA1OID, oidl))type=HASHTYPE_SHA1;
  else if(oidl==sizeof(SHA256OID)&&!memcmp(data+oido, SHA256OID, oidl))type=HASHTYPE_SHA256;
  else if(oidl==sizeof(SHA384OID)&&!memcmp(data+oido, SHA384OID, oidl))type=HASHTYPE_SHA384;
  else if(oidl==sizeof(SHA512OID)&&!memcmp(data+oido, SHA512OID, oidl))type=HASHTYPE_SHA512;else return false;
  HASH* hash=CIPHERPEMCODEC_HashCreate(type);if(!hash)return false;XBUFFER salt;salt.Add(data+salto, saltl);XSECUREBUFFER key;
  bool status=CIPHERPEMCODEC_PKCS12KDF(password, 3, salt, iterations, type, (XDWORD)hash->GetDefaultSize(), key);
  if(status){HASHHMAC hmac(hash);status=hmac.SetKey(key)&&hmac.Do(authsafe, authsafesize)&&hmac.GetResult()->GetSize()==digestl&&CIPHER::CompareConstantTime(hmac.GetResult()->Get(), data+digesto, digestl);}
  key.SecureDelete();GEN_DELETE hash;return status;
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERPEMCODEC::PKCS12_Decode(XBUFFER& der, XSTRING& password, XVECTOR<XBUFFER*>& certificates, XSECUREBUFFER& privatekeyinfo)
* @brief      Decode
* @ingroup    CIPHER
* 
* @param[in]  der : Der value.
* @param[in]  password : Password value.
* @param[in]  certificates : Pointer to certificates.
* @param[in]  privatekeyinfo : Privatekeyinfo value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERPEMCODEC::PKCS12_Decode(XBUFFER& der, XSTRING& password, XVECTOR<XBUFFER*>& certificates, XSECUREBUFFER& privatekeyinfo)
{
  static const XBYTE OIDDATA[]={0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,0x07,0x01};
  static const XBYTE OIDENCRYPTEDDATA[]={0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,0x07,0x06};
  certificates.DeleteContents();certificates.DeleteAll();privatekeyinfo.SecureDelete();
  XBYTE* data=der.Get();XDWORD size=der.GetSize(),i=0,o=0,l=0;XBYTE tag=0;
  if(!DER_ReadTagLength(data, size, i, tag, o, l)||tag!=0x30||i!=size)return false;XDWORD p=o,pend=o+l,vo=0,vl=0;
  if(!DER_ReadTagLength(data, pend, p, tag, vo, vl)||tag!=0x02||vl!=1||data[vo]!=3)return false;
  XDWORD cio=0,cil=0;if(!DER_ReadTagLength(data, pend, p, tag, cio, cil)||tag!=0x30)return false;
  XDWORD ci=cio,ciend=cio+cil,oido=0,oidl=0,co=0,cl=0;
  if(!DER_ReadTagLength(data, ciend, ci, tag, oido, oidl)||tag!=0x06||oidl!=sizeof(OIDDATA)||memcmp(data+oido, OIDDATA, oidl)||
     !DER_ReadTagLength(data, ciend, ci, tag, co, cl)||tag!=0xA0||ci!=ciend)return false;
  XDWORD ex=co,exend=co+cl,aso=0,asl=0;if(!DER_ReadTagLength(data, exend, ex, tag, aso, asl)||tag!=0x04||ex!=exend)return false;
  if(p>=pend)return false;XDWORD macstart=p,macvalueo=0,macvaluel=0;
  if(!DER_ReadTagLength(data, pend, p, tag, macvalueo, macvaluel)||tag!=0x30||p!=pend||
     !CIPHERPEMCODEC_PKCS12MACVerify(data+aso, asl, data+macstart, p-macstart, password))return false;
  XBYTE* asdata=data+aso;XDWORD asi=0,asseqo=0,asseql=0;if(!DER_ReadTagLength(asdata, asl, asi, tag, asseqo, asseql)||tag!=0x30||asi!=asl)return false;
  XDWORD aci=asseqo,aciend=asseqo+asseql;
  while(aci<aciend)
    {
      XDWORD itemo=0,iteml=0;if(!DER_ReadTagLength(asdata, aciend, aci, tag, itemo, iteml)||tag!=0x30)goto error;
      XDWORD ii=itemo,iiend=itemo+iteml,ioido=0,ioidl=0,ico=0,icl=0;
      if(!DER_ReadTagLength(asdata, iiend, ii, tag, ioido, ioidl)||tag!=0x06||!DER_ReadTagLength(asdata, iiend, ii, tag, ico, icl)||tag!=0xA0)goto error;
      XSECUREBUFFER safe;
      if(ioidl==sizeof(OIDDATA)&&!memcmp(asdata+ioido, OIDDATA, ioidl))
        { XDWORD di=ico,diend=ico+icl,doff=0,dlen=0;if(!DER_ReadTagLength(asdata, diend, di, tag, doff, dlen)||tag!=0x04||di!=diend||!safe.Add(asdata+doff, dlen))goto error; }
      else if(ioidl==sizeof(OIDENCRYPTEDDATA)&&!memcmp(asdata+ioido, OIDENCRYPTEDDATA, ioidl))
        {
          XDWORD ei=ico,eiend=ico+icl,eso=0,esl=0;if(!DER_ReadTagLength(asdata, eiend, ei, tag, eso, esl)||tag!=0x30||ei!=eiend)goto error;
          XDWORD ed=eso,edend=eso+esl,tmpo=0,tmpl=0,ecio=0,ecil=0;if(!DER_ReadTagLength(asdata, edend, ed, tag, tmpo, tmpl)||tag!=0x02||!DER_ReadTagLength(asdata, edend, ed, tag, ecio, ecil)||tag!=0x30)goto error;
          XDWORD eci=ecio,eciend=ecio+ecil,ctoid=0,ctoidl=0,algo=0,algl=0,enco=0,encl=0,algstart=0;
          if(!DER_ReadTagLength(asdata, eciend, eci, tag, ctoid, ctoidl)||tag!=0x06||ctoidl!=sizeof(OIDDATA)||memcmp(asdata+ctoid, OIDDATA, ctoidl))goto error;
          algstart=eci;if(!DER_ReadTagLength(asdata, eciend, eci, tag, algo, algl)||tag!=0x30||!DER_ReadTagLength(asdata, eciend, eci, tag, enco, encl)||tag!=0x80)goto error;
          XBUFFER algencoded;algencoded.Add(asdata+algstart, eci-algstart-encl-2); // rebuilt below if long-form made subtraction ambiguous
          algencoded.Delete(); XDWORD algcursor=algstart,ignoreo=0,ignorel=0; DER_ReadTagLength(asdata, eciend, algcursor, tag, ignoreo, ignorel); algencoded.Add(asdata+algstart, algcursor-algstart);
          XBUFFER encbytes;encbytes.Add(asdata+enco, encl);XBUFFER octet,body,wrapped;CIPHERPEMCODEC_DERWrap(0x04, encbytes, octet);body.Add(algencoded);body.Add(octet);CIPHERPEMCODEC_DERWrap(0x30, body, wrapped);
          if(!PKCS8EncryptedPrivateKey_Decode(wrapped, password, safe))goto error;
        }
      else goto error;
      if(!CIPHERPEMCODEC_PKCS12SafeContents(safe, password, certificates, privatekeyinfo))goto error;
      safe.SecureDelete();
    }
  if(certificates.IsEmpty()||privatekeyinfo.IsEmpty())goto error;
  return true;
error:
  certificates.DeleteContents();certificates.DeleteAll();privatekeyinfo.SecureDelete();return false;
}
