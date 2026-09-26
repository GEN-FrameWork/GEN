/**-------------------------------------------------------------------------------------------------------------------
*
* @file       CipherCertificateX509Revocation.cpp
*
* @class      CIPHERREV_READER
* @brief      Signed OCSP/CRL validation primitives for X.509 consumers
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

#include "GEN_Defines.h"
#include "CipherCertificateX509Revocation.h"

#include <string.h>

#include "Cipher.h"
#ifdef CIPHER_ASYMMETRIC_RSA_ACTIVE
#include "CipherRSA.h"
#endif
#include "HashSHA1.h"
#include "HashSHA2.h"
#include "XFactory.h"
#include "GEN_Control.h"

#define CIPHERREV_CLOCKSKEW_SECONDS                    300
#define CIPHERREV_OCSP_WITHOUT_NEXTUPDATE_MAX_SECONDS  86400

struct CIPHERREV_DER
{
  XBYTE tag; const XBYTE* encoded; XDWORD encodedsize; const XBYTE* data; XDWORD size;
};

class CIPHERREV_READER
{
  public:
    CIPHERREV_READER(const XBYTE* data, XDWORD size) : data(data), size(size), position(0) {}
    /**-------------------------------------------------------------------------------------------------------------------
    * 
    * @fn         bool Read(CIPHERREV_DER& item)
    * @brief      Read
    * @ingroup    CIPHER
    * 
    * @param[in]  item : Item value.
    * 
    * @return     bool : true if the operation is successful; otherwise false.
    * 
    * --------------------------------------------------------------------------------------------------------------------*/
    bool Read(CIPHERREV_DER& item)
    {
      if(!data || position>=size) return false;
      XDWORD start=position; item.tag=data[position++];
      if(((item.tag&0x1F)==0x1F) || position>=size) return false;
      XDWORD length=data[position++];
      if(length&0x80)
        {
          XDWORD octets=length&0x7F;
          if(!octets || octets>4 || octets>size-position || !data[position]) return false;
          length=0;
          for(XDWORD c=0;c<octets;c++) length=(length<<8)|data[position++];
          if(length<128) return false;
        }
      if(length>size-position) return false;
      item.encoded=data+start; item.data=data+position; item.size=length;
      position+=length; item.encodedsize=position-start; return true;
    }
    /**-------------------------------------------------------------------------------------------------------------------
    * 
    * @fn         bool End() const
    * @brief      End
    * @ingroup    CIPHER
    * 
    * @return     bool : true if the operation is successful; otherwise false.
    * 
    * --------------------------------------------------------------------------------------------------------------------*/
    bool End() const { return position==size; }
  private:
    const XBYTE* data; XDWORD size; XDWORD position;
};

enum CIPHERREV_HASH
{
  CIPHERREV_HASH_UNKNOWN=0, CIPHERREV_HASH_SHA1, CIPHERREV_HASH_SHA256,
  CIPHERREV_HASH_SHA384, CIPHERREV_HASH_SHA512
};

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         static bool CIPHERREV_OID(const CIPHERREV_DER& item, const XBYTE* OID, XDWORD size)
* @brief      Oid
* @ingroup    CIPHER
* 
* @param[in]  item : Item value.
* @param[in]  OID : Pointer to OID.
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool CIPHERREV_OID(const CIPHERREV_DER& item, const XBYTE* OID, XDWORD size)
{
  return item.tag==0x06 && item.size==size && !memcmp(item.data, OID, size);
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         static bool CIPHERREV_AlgorithmIdentifier(const CIPHERREV_DER& algorithm, CIPHERREV_DER& OID, CIPHERREV_DER* parameters=NULL)
* @brief      Algorithm identifier
* @ingroup    CIPHER
* 
* @param[in]  algorithm : Algorithm value.
* @param[in]  OID : OID value.
* @param[in]  parameters : Pointer to parameters.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool CIPHERREV_AlgorithmIdentifier(const CIPHERREV_DER& algorithm, CIPHERREV_DER& OID, CIPHERREV_DER* parameters=NULL)
{
  if(algorithm.tag!=0x30) return false;
  CIPHERREV_READER reader(algorithm.data, algorithm.size);
  if(!reader.Read(OID) || OID.tag!=0x06) return false;
  if(parameters)
    {
      memset(parameters, 0, sizeof(CIPHERREV_DER));
      if(!reader.End() && !reader.Read((*parameters))) return false;
    }
  return reader.End();
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         static CIPHERREV_HASH CIPHERREV_HashAlgorithm(const CIPHERREV_DER& algorithm)
* @brief      Hash algorithm
* @ingroup    CIPHER
* 
* @param[in]  algorithm : Algorithm value.
* 
* @return     CIPHERREV_HASH : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static CIPHERREV_HASH CIPHERREV_HashAlgorithm(const CIPHERREV_DER& algorithm)
{
  static const XBYTE SHA1[]   ={0x2B,0x0E,0x03,0x02,0x1A};
  static const XBYTE SHA256[] ={0x60,0x86,0x48,0x01,0x65,0x03,0x04,0x02,0x01};
  static const XBYTE SHA384[] ={0x60,0x86,0x48,0x01,0x65,0x03,0x04,0x02,0x02};
  static const XBYTE SHA512[] ={0x60,0x86,0x48,0x01,0x65,0x03,0x04,0x02,0x03};
  CIPHERREV_DER OID,parameters;
  if(!CIPHERREV_AlgorithmIdentifier(algorithm, OID, &parameters) ||
     (parameters.tag && (parameters.tag!=0x05 || parameters.size))) return CIPHERREV_HASH_UNKNOWN;
  if(CIPHERREV_OID(OID, SHA1, sizeof(SHA1))) return CIPHERREV_HASH_SHA1;
  if(CIPHERREV_OID(OID, SHA256, sizeof(SHA256))) return CIPHERREV_HASH_SHA256;
  if(CIPHERREV_OID(OID, SHA384, sizeof(SHA384))) return CIPHERREV_HASH_SHA384;
  if(CIPHERREV_OID(OID, SHA512, sizeof(SHA512))) return CIPHERREV_HASH_SHA512;
  return CIPHERREV_HASH_UNKNOWN;
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         static bool CIPHERREV_Hash(CIPHERREV_HASH type, const XBYTE* data, XDWORD size, XBUFFER& result)
* @brief      Hash
* @ingroup    CIPHER
* 
* @param[in]  type : Type value.
* @param[in]  data : Pointer to data.
* @param[in]  size : Size value.
* @param[out] result : Result value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool CIPHERREV_Hash(CIPHERREV_HASH type, const XBYTE* data, XDWORD size, XBUFFER& result)
{
  result.Empty();
  if(type==CIPHERREV_HASH_SHA1)
    {
      HASHSHA1 hash; return hash.Do((XBYTE*)data, size) && result.Add((*hash.GetResult()));
    }
  HASHSHA2TYPE SHA2type=(type==CIPHERREV_HASH_SHA256)?HASHSHA2TYPE_256:
                        (type==CIPHERREV_HASH_SHA384)?HASHSHA2TYPE_384:
                        (type==CIPHERREV_HASH_SHA512)?HASHSHA2TYPE_512:HASHSHA2TYPE_UNKNOWN;
  if(SHA2type==HASHSHA2TYPE_UNKNOWN) return false;
  HASHSHA2 hash(SHA2type); return hash.Do((XBYTE*)data, size) && result.Add((*hash.GetResult()));
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         static CIPHERCERTIFICATEX509_ALGORITHM_TYPE CIPHERREV_SignatureAlgorithm(const CIPHERREV_DER& algorithm)
* @brief      Signature algorithm
* @ingroup    CIPHER
* 
* @param[in]  algorithm : Algorithm value.
* 
* @return     CIPHERCERTIFICATEX509_ALGORITHM_TYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static CIPHERCERTIFICATEX509_ALGORITHM_TYPE CIPHERREV_SignatureAlgorithm(const CIPHERREV_DER& algorithm)
{
  static const XBYTE RSA1[]  ={0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,0x01,0x05};
  static const XBYTE RSA256[]={0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,0x01,0x0B};
  static const XBYTE RSA384[]={0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,0x01,0x0C};
  static const XBYTE RSA512[]={0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,0x01,0x0D};
  static const XBYTE EC1[]   ={0x2A,0x86,0x48,0xCE,0x3D,0x04,0x01};
  static const XBYTE EC256[] ={0x2A,0x86,0x48,0xCE,0x3D,0x04,0x03,0x02};
  static const XBYTE EC384[] ={0x2A,0x86,0x48,0xCE,0x3D,0x04,0x03,0x03};
  static const XBYTE EC512[] ={0x2A,0x86,0x48,0xCE,0x3D,0x04,0x03,0x04};
  static const XBYTE ED[]    ={0x2B,0x65,0x70};
  CIPHERREV_DER OID,parameters;
  if(!CIPHERREV_AlgorithmIdentifier(algorithm, OID, &parameters)) return CIPHERCERTIFICATEX509_ALGORITHM_TYPE_UNKNOWN;
  if(CIPHERREV_OID(OID, ED, sizeof(ED))) return parameters.tag?CIPHERCERTIFICATEX509_ALGORITHM_TYPE_UNKNOWN:CIPHERCERTIFICATEX509_ALGORITHM_TYPE_ED25519;
  if(parameters.tag && (parameters.tag!=0x05 || parameters.size)) return CIPHERCERTIFICATEX509_ALGORITHM_TYPE_UNKNOWN;
  if(CIPHERREV_OID(OID, RSA1, sizeof(RSA1))) return CIPHERCERTIFICATEX509_ALGORITHM_TYPE_SHA1WITHRSAENCRYPTION;
  if(CIPHERREV_OID(OID, RSA256, sizeof(RSA256))) return CIPHERCERTIFICATEX509_ALGORITHM_TYPE_SHA256WITHRSAENCRYPTION;
  if(CIPHERREV_OID(OID, RSA384, sizeof(RSA384))) return CIPHERCERTIFICATEX509_ALGORITHM_TYPE_SHA384WITHRSAENCRYPTION;
  if(CIPHERREV_OID(OID, RSA512, sizeof(RSA512))) return CIPHERCERTIFICATEX509_ALGORITHM_TYPE_SHA512WITHRSAENCRYPTION;
  if(CIPHERREV_OID(OID, EC1, sizeof(EC1))) return CIPHERCERTIFICATEX509_ALGORITHM_TYPE_ECDSAWITHSHA1;
  if(CIPHERREV_OID(OID, EC256, sizeof(EC256))) return CIPHERCERTIFICATEX509_ALGORITHM_TYPE_ECDSAWITHSHA256;
  if(CIPHERREV_OID(OID, EC384, sizeof(EC384))) return CIPHERCERTIFICATEX509_ALGORITHM_TYPE_ECDSAWITHSHA384;
  if(CIPHERREV_OID(OID, EC512, sizeof(EC512))) return CIPHERCERTIFICATEX509_ALGORITHM_TYPE_ECDSAWITHSHA512;
  return CIPHERCERTIFICATEX509_ALGORITHM_TYPE_UNKNOWN;
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         static bool CIPHERREV_PSSParameters(const CIPHERREV_DER& algorithm, CIPHERREV_HASH& hashtype, XDWORD& saltsize)
* @brief      Pss parameters
* @ingroup    CIPHER
* 
* @param[in]  algorithm : Algorithm value.
* @param[in]  hashtype : Hashtype value.
* @param[in]  saltsize : Saltsize value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool CIPHERREV_PSSParameters(const CIPHERREV_DER& algorithm, CIPHERREV_HASH& hashtype, XDWORD& saltsize)
{
  static const XBYTE PSS[]={0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,0x01,0x0A};
  static const XBYTE MGF1[]={0x2A,0x86,0x48,0x86,0xF7,0x0D,0x01,0x01,0x08};
  CIPHERREV_DER OID,parameters;
  if(!CIPHERREV_AlgorithmIdentifier(algorithm, OID, &parameters) || !CIPHERREV_OID(OID, PSS, sizeof(PSS)) || parameters.tag!=0x30) return false;
  hashtype=CIPHERREV_HASH_SHA1; CIPHERREV_HASH MGFhash=CIPHERREV_HASH_SHA1; saltsize=20; XDWORD trailer=1; XBYTE lasttag=0;
  CIPHERREV_READER fields(parameters.data, parameters.size);
  while(!fields.End())
    {
      CIPHERREV_DER field,value;
      if(!fields.Read(field) || field.tag<0xA0 || field.tag>0xA3 || field.tag<=lasttag) return false;
      lasttag=field.tag; CIPHERREV_READER explicitreader(field.data, field.size);
      if(!explicitreader.Read(value) || !explicitreader.End()) return false;
      if(field.tag==0xA0)
        {
          hashtype=CIPHERREV_HashAlgorithm(value); if(hashtype==CIPHERREV_HASH_UNKNOWN) return false;
        }
      else if(field.tag==0xA1)
        {
          CIPHERREV_DER mgfOID,mgfparameters;
          if(!CIPHERREV_AlgorithmIdentifier(value, mgfOID, &mgfparameters) || !CIPHERREV_OID(mgfOID, MGF1, sizeof(MGF1))) return false;
          MGFhash=CIPHERREV_HashAlgorithm(mgfparameters); if(MGFhash==CIPHERREV_HASH_UNKNOWN) return false;
        }
      else
        {
          if(value.tag!=0x02 || !value.size || value.size>4 || (value.data[0]&0x80) ||
             (value.size>1 && !value.data[0] && !(value.data[1]&0x80))) return false;
          XDWORD number=0; for(XDWORD c=0;c<value.size;c++) number=(number<<8)|value.data[c];
          if(field.tag==0xA2) saltsize=number; else trailer=number;
        }
    }
  return hashtype==MGFhash && trailer==1 &&
         (hashtype==CIPHERREV_HASH_SHA256 || hashtype==CIPHERREV_HASH_SHA384 || hashtype==CIPHERREV_HASH_SHA512);
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         static bool CIPHERREV_Verify(const CIPHERREV_DER& TBS, const CIPHERREV_DER& algorithm, const CIPHERREV_DER& bits, CIPHERCERTIFICATEX509& signer)
* @brief      Verify
* @ingroup    CIPHER
* 
* @param[in]  TBS : TBS value.
* @param[in]  algorithm : Algorithm value.
* @param[in]  bits : Bits value.
* @param[in]  signer : Signer value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool CIPHERREV_Verify(const CIPHERREV_DER& TBS, const CIPHERREV_DER& algorithm, const CIPHERREV_DER& bits, CIPHERCERTIFICATEX509& signer)
{
  if(bits.tag!=0x03 || bits.size<2 || bits.data[0]) return false;
  XBUFFER data,signature;
  if(!data.Add((XBYTE*)TBS.encoded, TBS.encodedsize) || !signature.Add((XBYTE*)&bits.data[1], bits.size-1)) return false;
  CIPHERCERTIFICATEX509_ALGORITHM_TYPE type=CIPHERREV_SignatureAlgorithm(algorithm);
  if(type!=CIPHERCERTIFICATEX509_ALGORITHM_TYPE_UNKNOWN)
    return CIPHERCERTIFICATEX509::VerifyDataSignature(signer.GetPublicCipherKey(), type, data, signature);
  CIPHERREV_HASH PSShash; XDWORD saltsize;
#ifndef CIPHER_ASYMMETRIC_RSA_ACTIVE
  return false;
#else
  if(!CIPHERREV_PSSParameters(algorithm, PSShash, saltsize) || !signer.GetPublicCipherKey() ||
     signer.GetPublicCipherKey()->GetType()!=CIPHERKEYTYPE_RSA_PUBLIC) return false;
  CIPHERRSA RSA; if(!RSA.SetKey(signer.GetPublicCipherKey(), true)) return false;
  if(PSShash==CIPHERREV_HASH_SHA256) { HASHSHA2 hash(HASHSHA2TYPE_256); return RSA.Verify(data, signature, &hash, CIPHERRSAPKCS1VERSIONV21, saltsize); }
  if(PSShash==CIPHERREV_HASH_SHA384) { HASHSHA2 hash(HASHSHA2TYPE_384); return RSA.Verify(data, signature, &hash, CIPHERRSAPKCS1VERSIONV21, saltsize); }
  if(PSShash==CIPHERREV_HASH_SHA512) { HASHSHA2 hash(HASHSHA2TYPE_512); return RSA.Verify(data, signature, &hash, CIPHERRSAPKCS1VERSIONV21, saltsize); }
  return false;
#endif
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         static bool CIPHERREV_Serial(const CIPHERREV_DER& serial)
* @brief      Serial
* @ingroup    CIPHER
* 
* @param[in]  serial : Serial value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool CIPHERREV_Serial(const CIPHERREV_DER& serial)
{
  return serial.tag==0x02 && serial.size && !(serial.data[0]&0x80) &&
         !(serial.size>1 && !serial.data[0] && !(serial.data[1]&0x80));
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         static bool CIPHERREV_SerialEqual(const CIPHERREV_DER& serial, XBUFFER* expected)
* @brief      Serial equal
* @ingroup    CIPHER
* 
* @param[in]  serial : Serial value.
* @param[in]  expected : Pointer to expected.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool CIPHERREV_SerialEqual(const CIPHERREV_DER& serial, XBUFFER* expected)
{
  if(!CIPHERREV_Serial(serial) || !expected || expected->IsEmpty()) return false;
  const XBYTE* left=serial.data; XDWORD leftsize=serial.size;
  XBYTE* right=expected->Get(); XDWORD rightsize=expected->GetSize();
  while(leftsize>1 && !left[0]) { left++; leftsize--; }
  while(rightsize>1 && !right[0]) { right++; rightsize--; }
  return leftsize==rightsize && CIPHER::CompareConstantTime((XBYTE*)left, right, leftsize);
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         static bool CIPHERREV_Time(const CIPHERREV_DER& item, XDATETIME& datetime, bool generalizedonly=false)
* @brief      Time
* @ingroup    CIPHER
* 
* @param[in]  item : Item value.
* @param[in]  datetime : Datetime value.
* @param[in]  generalizedonly : Generalizedonly value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool CIPHERREV_Time(const CIPHERREV_DER& item, XDATETIME& datetime, bool generalizedonly=false)
{
  if((item.tag!=0x17 && item.tag!=0x18) || (generalizedonly && item.tag!=0x18) ||
     item.size!=(item.tag==0x17?13:15) || item.data[item.size-1]!='Z') return false;
  for(XDWORD c=0;c<item.size-1;c++) if(item.data[c]<'0' || item.data[c]>'9') return false;
  int index,year;
  if(item.tag==0x17) { year=(item.data[0]-'0')*10+item.data[1]-'0'; year+=(year>=50)?1900:2000; index=2; }
  else { year=(item.data[0]-'0')*1000+(item.data[1]-'0')*100+(item.data[2]-'0')*10+item.data[3]-'0'; index=4; }
  datetime.SetYear(year); datetime.SetMonth((item.data[index]-'0')*10+item.data[index+1]-'0');
  datetime.SetDay((item.data[index+2]-'0')*10+item.data[index+3]-'0');
  datetime.SetHours((item.data[index+4]-'0')*10+item.data[index+5]-'0');
  datetime.SetMinutes((item.data[index+6]-'0')*10+item.data[index+7]-'0');
  datetime.SetSeconds((item.data[index+8]-'0')*10+item.data[index+9]-'0');
  datetime.SetMilliSeconds(0); datetime.SetIsLocal(false); return datetime.IsValidDate();
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         static bool CIPHERREV_Now(XQWORD& now)
* @brief      Now
* @ingroup    CIPHER
* 
* @param[in]  now : Now value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool CIPHERREV_Now(XQWORD& now)
{
  XDATETIME* datetime=GEN_XFACTORY.CreateDateTime(); if(!datetime) return false;
  bool status=datetime->Read(false); if(status) now=datetime->GetSeconsFromDate();
  GEN_XFACTORY.DeleteDateTime(datetime); return status;
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         static bool CIPHERREV_TimeWindow(const CIPHERREV_DER& from, const CIPHERREV_DER* until, XQWORD now, bool generalizedonly)
* @brief      Time window
* @ingroup    CIPHER
* 
* @param[in]  from : From value.
* @param[in]  until : Pointer to until.
* @param[in]  now : Now value.
* @param[in]  generalizedonly : Generalizedonly value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool CIPHERREV_TimeWindow(const CIPHERREV_DER& from, const CIPHERREV_DER* until, XQWORD now, bool generalizedonly)
{
  XDATETIME first;
  if(!CIPHERREV_Time(from, first, generalizedonly)) return false;
  XQWORD firstseconds=first.GetSeconsFromDate();
  if(firstseconds>now+CIPHERREV_CLOCKSKEW_SECONDS) return false;
  if(!until) return now<=firstseconds || now-firstseconds<=CIPHERREV_OCSP_WITHOUT_NEXTUPDATE_MAX_SECONDS;
  XDATETIME last;
  if(!CIPHERREV_Time((*until), last, generalizedonly)) return false;
  XQWORD lastseconds=last.GetSeconsFromDate();
  return lastseconds>=firstseconds && now<=lastseconds+CIPHERREV_CLOCKSKEW_SECONDS;
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         static bool CIPHERREV_Extensions(const CIPHERREV_DER& wrapper)
* @brief      Extensions
* @ingroup    CIPHER
* 
* @param[in]  wrapper : Wrapper value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool CIPHERREV_Extensions(const CIPHERREV_DER& wrapper)
{
  CIPHERREV_DER sequence=wrapper;
  if(wrapper.tag!=0x30)
    {
      CIPHERREV_READER explicitreader(wrapper.data, wrapper.size);
      if(!explicitreader.Read(sequence) || !explicitreader.End() || sequence.tag!=0x30) return false;
    }
  CIPHERREV_READER extensions(sequence.data, sequence.size);
  while(!extensions.End())
    {
      CIPHERREV_DER extension,OID,value;
      if(!extensions.Read(extension) || extension.tag!=0x30) return false;
      CIPHERREV_READER fields(extension.data, extension.size);
      if(!fields.Read(OID) || OID.tag!=0x06 || !fields.Read(value)) return false;
      if(value.tag==0x01)
        {
          if(value.size!=1 || (value.data[0]!=0 && value.data[0]!=0xFF) || value.data[0] || !fields.Read(value)) return false;
        }
      if(!fields.End() || value.tag!=0x04) return false;
    }
  return true;
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         static bool CIPHERREV_CRLEntryExtensions(const CIPHERREV_DER& sequence)
* @brief      Crl entry extensions
* @ingroup    CIPHER
* 
* @param[in]  sequence : Sequence value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool CIPHERREV_CRLEntryExtensions(const CIPHERREV_DER& sequence)
{
  static const XBYTE REASON[]={0x55,0x1D,0x15};
  static const XBYTE CERTIFICATEISSUER[]={0x55,0x1D,0x1D};
  if(sequence.tag!=0x30) return false;
  CIPHERREV_READER extensions(sequence.data, sequence.size);
  while(!extensions.End())
    {
      CIPHERREV_DER extension,OID,value; bool critical=false;
      if(!extensions.Read(extension) || extension.tag!=0x30) return false;
      CIPHERREV_READER fields(extension.data, extension.size);
      if(!fields.Read(OID) || OID.tag!=0x06 || !fields.Read(value)) return false;
      if(value.tag==0x01)
        {
          if(value.size!=1 || (value.data[0]!=0 && value.data[0]!=0xFF)) return false;
          critical=value.data[0]?true:false;
          if(!fields.Read(value)) return false;
        }
      if(!fields.End() || value.tag!=0x04 || CIPHERREV_OID(OID, CERTIFICATEISSUER, sizeof(CERTIFICATEISSUER))) return false;
      if(CIPHERREV_OID(OID, REASON, sizeof(REASON)))
        {
          CIPHERREV_READER reasonreader(value.data, value.size); CIPHERREV_DER reason;
          if(!reasonreader.Read(reason) || !reasonreader.End() || reason.tag!=0x0A || reason.size!=1 ||
             reason.data[0]>10 || reason.data[0]==7 || reason.data[0]==8) return false;
        }
      else if(critical) return false;
    }
  return true;
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         static bool CIPHERREV_ResponderMatches(const CIPHERREV_DER& responder, CIPHERCERTIFICATEX509& signer)
* @brief      Responder matches
* @ingroup    CIPHER
* 
* @param[in]  responder : Responder value.
* @param[in]  signer : Signer value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool CIPHERREV_ResponderMatches(const CIPHERREV_DER& responder, CIPHERCERTIFICATEX509& signer)
{
  if(responder.tag==0xA1)
    {
      XBUFFER* subject=signer.GetSubjectData();
      return subject && responder.size==subject->GetSize() && !memcmp(responder.data, subject->Get(), responder.size);
    }
  if(responder.tag==0xA2)
    {
      CIPHERREV_READER explicitreader(responder.data, responder.size); CIPHERREV_DER keyhash;
      if(!explicitreader.Read(keyhash) || !explicitreader.End() || keyhash.tag!=0x04 || keyhash.size!=20) return false;
      XBUFFER hash; XBUFFER* key=signer.GetSubjectPublicKeyData();
      return key && CIPHERREV_Hash(CIPHERREV_HASH_SHA1, key->Get(), key->GetSize(), hash) &&
             CIPHER::CompareConstantTime(hash.Get(), (XBYTE*)keyhash.data, keyhash.size);
    }
  return false;
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         static bool CIPHERREV_DelegatedAuthorized(CIPHERCERTIFICATEX509& responder, CIPHERCERTIFICATEX509& issuer)
* @brief      Delegated authorized
* @ingroup    CIPHER
* 
* @param[in]  responder : Responder value.
* @param[in]  issuer : Issuer value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool CIPHERREV_DelegatedAuthorized(CIPHERCERTIFICATEX509& responder, CIPHERCERTIFICATEX509& issuer)
{
  return responder.GetIssuerData() && issuer.GetSubjectData() &&
         responder.GetIssuerData()->GetSize()==issuer.GetSubjectData()->GetSize() &&
         !memcmp(responder.GetIssuerData()->Get(), issuer.GetSubjectData()->Get(), responder.GetIssuerData()->GetSize()) &&
         responder.VerifySignature(issuer.GetPublicCipherKey()) && responder.IsValidDates() &&
         responder.HasExtendedKeyUsage() && responder.IsExtendedKeyUsageOCSPSigning() &&
         (!responder.HasKeyUsage() || responder.IsKeyUsageDigitalSignature()) && !responder.HasUnknownCriticalExtension();
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         static bool CIPHERREV_CertIDMatches(const CIPHERREV_DER& certID, CIPHERCERTIFICATEX509& certificate, CIPHERCERTIFICATEX509& issuer)
* @brief      Cert id matches
* @ingroup    CIPHER
* 
* @param[in]  certID : CertID value.
* @param[in]  certificate : Certificate value.
* @param[in]  issuer : Issuer value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool CIPHERREV_CertIDMatches(const CIPHERREV_DER& certID, CIPHERCERTIFICATEX509& certificate, CIPHERCERTIFICATEX509& issuer)
{
  if(certID.tag!=0x30) return false;
  CIPHERREV_READER reader(certID.data, certID.size); CIPHERREV_DER algorithm,namehash,keyhash,serial;
  if(!reader.Read(algorithm) || !reader.Read(namehash) || !reader.Read(keyhash) || !reader.Read(serial) || !reader.End() ||
     namehash.tag!=0x04 || keyhash.tag!=0x04 || !CIPHERREV_SerialEqual(serial, certificate.GetSerial())) return false;
  CIPHERREV_HASH hashtype=CIPHERREV_HashAlgorithm(algorithm); XBUFFER expectedname,expectedkey;
  XBUFFER* name=issuer.GetSubjectData(); XBUFFER* key=issuer.GetSubjectPublicKeyData();
  return hashtype!=CIPHERREV_HASH_UNKNOWN && name && key && CIPHERREV_Hash(hashtype, name->Get(), name->GetSize(), expectedname) &&
         CIPHERREV_Hash(hashtype, key->Get(), key->GetSize(), expectedkey) && expectedname.GetSize()==namehash.size &&
         expectedkey.GetSize()==keyhash.size && CIPHER::CompareConstantTime(expectedname.Get(), (XBYTE*)namehash.data, namehash.size) &&
         CIPHER::CompareConstantTime(expectedkey.Get(), (XBYTE*)keyhash.data, keyhash.size);
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         static bool CIPHERREV_CRLExtensions(const CIPHERREV_DER& wrapper, CIPHERCERTIFICATEX509& issuer)
* @brief      Crl extensions
* @ingroup    CIPHER
* 
* @param[in]  wrapper : Wrapper value.
* @param[in]  issuer : Issuer value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool CIPHERREV_CRLExtensions(const CIPHERREV_DER& wrapper, CIPHERCERTIFICATEX509& issuer)
{
  static const XBYTE AKI[]={0x55,0x1D,0x23}, NUMBER[]={0x55,0x1D,0x14};
  static const XBYTE DELTA[]={0x55,0x1D,0x1B}, IDP[]={0x55,0x1D,0x1C};
  CIPHERREV_READER explicitreader(wrapper.data, wrapper.size); CIPHERREV_DER sequence;
  if(!explicitreader.Read(sequence) || !explicitreader.End() || sequence.tag!=0x30) return false;
  CIPHERREV_READER extensions(sequence.data, sequence.size);
  while(!extensions.End())
    {
      CIPHERREV_DER extension,OID,value; bool critical=false;
      if(!extensions.Read(extension) || extension.tag!=0x30) return false;
      CIPHERREV_READER fields(extension.data, extension.size);
      if(!fields.Read(OID) || OID.tag!=0x06 || !fields.Read(value)) return false;
      if(value.tag==0x01)
        {
          if(value.size!=1 || (value.data[0]!=0 && value.data[0]!=0xFF)) return false;
          critical=value.data[0]?true:false;
          if(!fields.Read(value)) return false;
        }
      if(!fields.End() || value.tag!=0x04 || CIPHERREV_OID(OID, DELTA, sizeof(DELTA)) || CIPHERREV_OID(OID, IDP, sizeof(IDP))) return false;
      if(CIPHERREV_OID(OID, NUMBER, sizeof(NUMBER)))
        {
          CIPHERREV_READER nr(value.data, value.size); CIPHERREV_DER number;
          if(!nr.Read(number) || !nr.End() || !CIPHERREV_Serial(number)) return false;
        }
      else if(CIPHERREV_OID(OID, AKI, sizeof(AKI)))
        {
          CIPHERREV_READER ar(value.data, value.size); CIPHERREV_DER sequence2;
          if(!ar.Read(sequence2) || !ar.End() || sequence2.tag!=0x30) return false;
          CIPHERREV_READER af(sequence2.data, sequence2.size); CIPHERREV_DER keyid;
          if(!af.End() && (!af.Read(keyid) || (keyid.tag==0x80 && issuer.HasSubjectKeyIdentifier() &&
             (keyid.size!=issuer.GetSubjectKeyIdentifier()->GetSize() ||
              !CIPHER::CompareConstantTime((XBYTE*)keyid.data, issuer.GetSubjectKeyIdentifier()->Get(), keyid.size))))) return false;
        }
      else if(critical) return false;
    }
  return true;
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHERCERTIFICATEX509REVOCATION_RESULT CIPHERCERTIFICATEX509REVOCATION::ValidateCRL(XBUFFER& CRL, CIPHERCERTIFICATEX509& certificate, CIPHERCERTIFICATEX509& issuer)
* @brief      Validate crl
* @ingroup    CIPHER
* 
* @param[in]  CRL : CRL value.
* @param[in]  certificate : Certificate value.
* @param[in]  issuer : Issuer value.
* 
* @return     CIPHERCERTIFICATEX509REVOCATION_RESULT : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERCERTIFICATEX509REVOCATION_RESULT CIPHERCERTIFICATEX509REVOCATION::ValidateCRL(XBUFFER& CRL, CIPHERCERTIFICATEX509& certificate, CIPHERCERTIFICATEX509& issuer)
{
  if(CRL.IsEmpty() || CRL.GetSize()>CIPHERCERTIFICATEX509REVOCATION_MAX_CRL_SIZE ||
     (issuer.HasKeyUsage() && !issuer.IsKeyUsageCRLSign())) return CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID;
  CIPHERREV_READER outer(CRL.Get(), CRL.GetSize()); CIPHERREV_DER sequence,TBS,outeralgorithm,bits;
  if(!outer.Read(sequence) || !outer.End() || sequence.tag!=0x30) return CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID;
  CIPHERREV_READER top(sequence.data, sequence.size);
  if(!top.Read(TBS) || !top.Read(outeralgorithm) || !top.Read(bits) || !top.End() || TBS.tag!=0x30)
    return CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID;
  CIPHERREV_READER fields(TBS.data, TBS.size); CIPHERREV_DER item,inneralgorithm,issuername;
  bool version2=false;
  if(!fields.Read(item)) return CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID;
  if(item.tag==0x02)
    {
      if(item.size!=1 || item.data[0]!=1 || !fields.Read(item)) return CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID;
      version2=true;
    }
  inneralgorithm=item;
  if(inneralgorithm.encodedsize!=outeralgorithm.encodedsize || memcmp(inneralgorithm.encoded, outeralgorithm.encoded, inneralgorithm.encodedsize) ||
     !CIPHERREV_Verify(TBS, outeralgorithm, bits, issuer)) return CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID;
  if(!fields.Read(issuername) || issuername.tag!=0x30 || !issuer.GetSubjectData() ||
     issuername.encodedsize!=issuer.GetSubjectData()->GetSize() || memcmp(issuername.encoded, issuer.GetSubjectData()->Get(), issuername.encodedsize))
    return CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID;
  CIPHERREV_DER thisupdate,nextupdate; XQWORD now;
  memset(&nextupdate, 0, sizeof(nextupdate));
  if(!fields.Read(thisupdate) || !CIPHERREV_Now(now))
    return CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID;

  bool haveitem=false;
  if(!fields.End())
    {
      if(!fields.Read(item)) return CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID;
      haveitem=true;
      if((item.tag==0x17) || (item.tag==0x18))
        {
          nextupdate=item;
          haveitem=false;
        }
    }

  if(!CIPHERREV_TimeWindow(thisupdate, nextupdate.tag?&nextupdate:NULL, now, false))
    return CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID;

  if(!haveitem && !fields.End())
    {
      if(!fields.Read(item)) return CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID;
      haveitem=true;
    }

  bool revoked=false;
  if(haveitem && (item.tag==0x30))
    {
      CIPHERREV_READER entries(item.data, item.size);
      while(!entries.End())
        {
          CIPHERREV_DER entry,serial,date;
          if(!entries.Read(entry) || entry.tag!=0x30) return CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID;
          CIPHERREV_READER entryfields(entry.data, entry.size); XDATETIME ignored;
          if(!entryfields.Read(serial) || !CIPHERREV_Serial(serial) || !entryfields.Read(date) || !CIPHERREV_Time(date, ignored))
            return CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID;
          if(!entryfields.End())
            {
              CIPHERREV_DER extensions;
              if(!version2 || !entryfields.Read(extensions) || !entryfields.End() || !CIPHERREV_CRLEntryExtensions(extensions))
                return CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID;
            }
          if(CIPHERREV_SerialEqual(serial, certificate.GetSerial())) revoked=true;
        }

      haveitem=false;
      if(!fields.End())
        {
          if(!fields.Read(item)) return CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID;
          haveitem=true;
        }
    }

  if(haveitem && (!version2 || item.tag!=0xA0 || !fields.End() || !CIPHERREV_CRLExtensions(item, issuer)))
    return CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID;
  if(!haveitem && !fields.End()) return CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID;
  return revoked?CIPHERCERTIFICATEX509REVOCATION_RESULT_REVOKED:CIPHERCERTIFICATEX509REVOCATION_RESULT_GOOD;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHERCERTIFICATEX509REVOCATION_RESULT CIPHERCERTIFICATEX509REVOCATION::ValidateOCSP(XBUFFER& response, CIPHERCERTIFICATEX509& certificate, CIPHERCERTIFICATEX509& issuer)
* @brief      Validate ocsp
* @ingroup    CIPHER
* 
* @param[in]  response : Response value.
* @param[in]  certificate : Certificate value.
* @param[in]  issuer : Issuer value.
* 
* @return     CIPHERCERTIFICATEX509REVOCATION_RESULT : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERCERTIFICATEX509REVOCATION_RESULT CIPHERCERTIFICATEX509REVOCATION::ValidateOCSP(XBUFFER& response, CIPHERCERTIFICATEX509& certificate, CIPHERCERTIFICATEX509& issuer)
{
  static const XBYTE BASIC[]={0x2B,0x06,0x01,0x05,0x05,0x07,0x30,0x01,0x01};
  if(response.IsEmpty() || response.GetSize()>CIPHERCERTIFICATEX509REVOCATION_MAX_OCSP_SIZE)
    return CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID;
  CIPHERREV_READER outer(response.Get(), response.GetSize()); CIPHERREV_DER sequence,status,responsewrapper;
  if(!outer.Read(sequence) || !outer.End() || sequence.tag!=0x30) return CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID;
  CIPHERREV_READER fields(sequence.data, sequence.size);
  if(!fields.Read(status) || status.tag!=0x0A || status.size!=1 || status.data[0]>6 || status.data[0]==4)
    return CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID;
  if(status.data[0]) return fields.End()?CIPHERCERTIFICATEX509REVOCATION_RESULT_UNKNOWN:CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID;
  if(!fields.Read(responsewrapper) || !fields.End() || responsewrapper.tag!=0xA0) return CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID;
  CIPHERREV_READER rw(responsewrapper.data, responsewrapper.size); CIPHERREV_DER responsebytes,OID,octets;
  if(!rw.Read(responsebytes) || !rw.End() || responsebytes.tag!=0x30) return CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID;
  CIPHERREV_READER rbf(responsebytes.data, responsebytes.size);
  if(!rbf.Read(OID) || !CIPHERREV_OID(OID, BASIC, sizeof(BASIC)) || !rbf.Read(octets) || !rbf.End() || octets.tag!=0x04)
    return CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID;
  CIPHERREV_READER bo(octets.data, octets.size); CIPHERREV_DER basic,TBS,algorithm,bits,certswrapper;
  memset(&certswrapper, 0, sizeof(certswrapper));
  if(!bo.Read(basic) || !bo.End() || basic.tag!=0x30) return CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID;
  CIPHERREV_READER bf(basic.data, basic.size);
  if(!bf.Read(TBS) || !bf.Read(algorithm) || !bf.Read(bits) || TBS.tag!=0x30) return CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID;
  if(!bf.End() && (!bf.Read(certswrapper) || certswrapper.tag!=0xA0)) return CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID;
  if(!bf.End()) return CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID;
  CIPHERREV_READER metadata(TBS.data, TBS.size); CIPHERREV_DER item;
  if(!metadata.Read(item)) return CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID;
  if(item.tag==0xA0)
    {
      CIPHERREV_READER vr(item.data, item.size); CIPHERREV_DER version;
      if(!vr.Read(version) || !vr.End() || version.tag!=0x02 || version.size!=1 || version.data[0] || !metadata.Read(item))
        return CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID;
    }
  CIPHERREV_DER responder=item,producedat,responses;
  if((responder.tag!=0xA1 && responder.tag!=0xA2) || !metadata.Read(producedat) || producedat.tag!=0x18 ||
     !metadata.Read(responses) || responses.tag!=0x30 || !responses.size) return CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID;
  if(!metadata.End())
    {
      CIPHERREV_DER extensions;
      if(!metadata.Read(extensions) || !metadata.End() || extensions.tag!=0xA1 || !CIPHERREV_Extensions(extensions))
        return CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID;
    }
  XQWORD now; XDATETIME produced;
  if(!CIPHERREV_Now(now) || !CIPHERREV_Time(producedat, produced, true) || produced.GetSeconsFromDate()>now+CIPHERREV_CLOCKSKEW_SECONDS)
    return CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID;
  CIPHERCERTIFICATEX509* signer=CIPHERREV_ResponderMatches(responder, issuer)?&issuer:NULL;
  CIPHERCERTIFICATEX509* delegated=NULL;
  if(!signer && certswrapper.tag)
    {
      CIPHERREV_READER ce(certswrapper.data, certswrapper.size); CIPHERREV_DER certsequence;
      if(!ce.Read(certsequence) || !ce.End() || certsequence.tag!=0x30) return CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID;
      CIPHERREV_READER certs(certsequence.data, certsequence.size);
      while(!certs.End())
        {
          CIPHERREV_DER encoded; if(!certs.Read(encoded) || encoded.tag!=0x30) { if(delegated) GEN_DELETE delegated; return CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID; }
          CIPHERCERTIFICATEX509* candidate=GEN_NEW CIPHERCERTIFICATEX509(); XBUFFER DER;
          if(!candidate || !DER.Add((XBYTE*)encoded.encoded, encoded.encodedsize) || !candidate->Decode(DER))
            { if(candidate) GEN_DELETE candidate; if(delegated) GEN_DELETE delegated; return CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID; }
          if(!delegated && CIPHERREV_ResponderMatches(responder, (*candidate)) && CIPHERREV_DelegatedAuthorized((*candidate), issuer)) delegated=candidate;
          else GEN_DELETE candidate;
        }
      signer=delegated;
    }
  if(!signer || !CIPHERREV_Verify(TBS, algorithm, bits, (*signer)))
    { if(delegated) GEN_DELETE delegated; return CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID; }
  CIPHERCERTIFICATEX509REVOCATION_RESULT result=CIPHERCERTIFICATEX509REVOCATION_RESULT_UNKNOWN; bool found=false;
  CIPHERREV_READER singles(responses.data, responses.size);
  while(!singles.End())
    {
      CIPHERREV_DER single,certID,certstatus,thisupdate,nextupdate; CIPHERREV_DER* next=NULL;
      if(!singles.Read(single) || single.tag!=0x30) { result=CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID; break; }
      CIPHERREV_READER sf(single.data, single.size);
      if(!sf.Read(certID) || !sf.Read(certstatus) || !sf.Read(thisupdate) || (certstatus.tag!=0x80 && certstatus.tag!=0xA1 && certstatus.tag!=0x82) || thisupdate.tag!=0x18)
        { result=CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID; break; }
      memset(&item, 0, sizeof(item));
      if(!sf.End())
        {
          if(!sf.Read(item)) { result=CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID; break; }
          if(item.tag==0xA0)
            {
              CIPHERREV_READER ne(item.data, item.size);
              if(!ne.Read(nextupdate) || !ne.End() || nextupdate.tag!=0x18) { result=CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID; break; }
              next=&nextupdate;
              if(!sf.End()) { if(!sf.Read(item)) { result=CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID; break; } }
              else memset(&item, 0, sizeof(item));
            }
          if(item.tag && (item.tag!=0xA1 || !sf.End() || !CIPHERREV_Extensions(item)))
            { result=CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID; break; }
        }
      if(!CIPHERREV_TimeWindow(thisupdate, next, now, true)) { result=CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID; break; }
      if(!CIPHERREV_CertIDMatches(certID, certificate, issuer)) continue;
      CIPHERCERTIFICATEX509REVOCATION_RESULT current;
      if(certstatus.tag==0x80) { if(certstatus.size) { result=CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID; break; } current=CIPHERCERTIFICATEX509REVOCATION_RESULT_GOOD; }
      else if(certstatus.tag==0x82) { if(certstatus.size) { result=CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID; break; } current=CIPHERCERTIFICATEX509REVOCATION_RESULT_UNKNOWN; }
      else
        {
          CIPHERREV_READER ri(certstatus.data, certstatus.size); CIPHERREV_DER revocationtime; XDATETIME revokedat;
          if(!ri.Read(revocationtime) || !CIPHERREV_Time(revocationtime, revokedat, true) ||
             revokedat.GetSeconsFromDate()>now+CIPHERREV_CLOCKSKEW_SECONDS)
            { result=CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID; break; }
          if(!ri.End())
            {
              CIPHERREV_DER reasonwrapper,reason;
              if(!ri.Read(reasonwrapper) || !ri.End() || reasonwrapper.tag!=0xA0)
                { result=CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID; break; }
              CIPHERREV_READER reasonreader(reasonwrapper.data, reasonwrapper.size);
              if(!reasonreader.Read(reason) || !reasonreader.End() || reason.tag!=0x0A || reason.size!=1 ||
                 reason.data[0]>10 || reason.data[0]==7)
                { result=CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID; break; }
            }
          current=CIPHERCERTIFICATEX509REVOCATION_RESULT_REVOKED;
        }
      if(found && result!=current) { result=CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID; break; }
      found=true; result=current;
    }
  if(delegated) GEN_DELETE delegated;
  return found?result:CIPHERCERTIFICATEX509REVOCATION_RESULT_UNKNOWN;
}
