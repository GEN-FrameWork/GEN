/**-------------------------------------------------------------------------------------------------------------------
*
* @file       CipherCredentialsLoader.cpp
*
* @class      CIPHERCREDENTIALSLOADER
* @brief      Central certificate/private-key format loader for TLS and other Cipher consumers
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
#include "CipherCredentialsLoader.h"
#include "Cipher.h"
#include "CipherPEMCodec.h"
#include "CipherCertificateX509.h"
#ifdef CIPHER_ASYMMETRIC_RSA_ACTIVE
#include "CipherKeyPublicRSA.h"
#include "CipherKeyPrivateRSA.h"
#endif
#include "CipherKeyECDSA.h"
#include "CipherECDSA.h"
#include "CipherKeySymmetrical.h"

#ifdef CIPHER_ASYMMETRIC_ED25519_ACTIVE
#include "CipherEd25519.h"
#endif
#include <string.h>
#include <stdio.h>
#include "GEN_Control.h"

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void CIPHERCREDENTIALSLOADER::Certificates_Delete(XVECTOR<XBUFFER*>& certificatechain)
* @brief      Certificates delete
* @ingroup    CIPHER
* 
* @param[in]  certificatechain : Pointer to certificatechain.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void CIPHERCREDENTIALSLOADER::Certificates_Delete(XVECTOR<XBUFFER*>& certificatechain)
{
  certificatechain.DeleteContents(); certificatechain.DeleteAll();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void CIPHERCREDENTIALSLOADER::PrivateKey_Delete(CIPHERKEY*& privatekey)
* @brief      Private key delete
* @ingroup    CIPHER
* 
* @param[in]  privatekey : Pointer to privatekey.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void CIPHERCREDENTIALSLOADER::PrivateKey_Delete(CIPHERKEY*& privatekey)
{
  if(privatekey) GEN_DELETE privatekey;
  privatekey=NULL;
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         static void CIPHERCREDENTIALSLOADER_StringWipe(XSTRING& string)
* @brief      String wipe
* @ingroup    CIPHER
* 
* @param[in]  string : String value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static void CIPHERCREDENTIALSLOADER_StringWipe(XSTRING& string)
{
  if(string.Get())
    {
      volatile XCHAR* data=string.Get();
      for(XDWORD c=0;c<string.GetSize();c++) data[c]=0;
    }
  string.Empty();
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         static bool CIPHERCREDENTIALSLOADER_LegacyHexLine(XBUFFER& filedata, XDWORD requested, XSTRING& line)
* @brief      Legacy hex line
* @ingroup    CIPHER
* 
* @param[in]  filedata : Filedata value.
* @param[in]  requested : Requested value.
* @param[in]  line : Line value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool CIPHERCREDENTIALSLOADER_LegacyHexLine(XBUFFER& filedata, XDWORD requested, XSTRING& line)
{
  line.Empty();
  XDWORD ordinal=0;
  XDWORD start=0;

  for(XDWORD end=0; end<=filedata.GetSize(); end++)
    {
      bool separator=(end==filedata.GetSize()) || filedata.GetByte(end)=='\r' || filedata.GetByte(end)=='\n';
      if(!separator) continue;

      XDWORD first=start;
      XDWORD last=end;
      while(first<last && (filedata.GetByte(first)==' ' || filedata.GetByte(first)=='\t')) first++;
      while(last>first && (filedata.GetByte(last-1)==' ' || filedata.GetByte(last-1)=='\t')) last--;

      if(first<last)
        {
          if(ordinal==requested)
            {
              for(XDWORD c=first;c<last;c++)
                {
                  XBYTE value=filedata.GetByte(c);
                  bool hexadecimal=((value>='0')&&(value<='9')) || ((value>='a')&&(value<='f')) || ((value>='A')&&(value<='F'));
                  if(!hexadecimal || !line.Add((XCHAR)value))
                    {
                      CIPHERCREDENTIALSLOADER_StringWipe(line);
                      return false;
                    }
                }
              return !line.IsEmpty();
            }
          ordinal++;
        }

      while((end+1)<filedata.GetSize() &&
            (filedata.GetByte(end+1)=='\r' || filedata.GetByte(end+1)=='\n')) end++;
      start=end+1;
    }

  return false;
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         static bool CIPHERCREDENTIALSLOADER_LegacyPrivateKey(XBUFFER& filedata, CIPHERKEYTYPE expectedpublickeytype, CIPHERKEY*& privatekey)
* @brief      Legacy private key
* @ingroup    CIPHER
* 
* @param[in]  filedata : Filedata value.
* @param[in]  expectedpublickeytype : Expectedpublickeytype value.
* @param[in]  privatekey : Pointer to privatekey.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool CIPHERCREDENTIALSLOADER_LegacyPrivateKey(XBUFFER& filedata, CIPHERKEYTYPE expectedpublickeytype, CIPHERKEY*& privatekey)
{
  XSTRING first,second,third,extra;
  bool status=false;

  if(expectedpublickeytype==CIPHERKEYTYPE_RSA_PUBLIC)
    {
#ifndef CIPHER_ASYMMETRIC_RSA_ACTIVE
      return false;
#else
      if(CIPHERCREDENTIALSLOADER_LegacyHexLine(filedata, 0, first) &&
         CIPHERCREDENTIALSLOADER_LegacyHexLine(filedata, 1, second) &&
         CIPHERCREDENTIALSLOADER_LegacyHexLine(filedata, 2, third) &&
         !CIPHERCREDENTIALSLOADER_LegacyHexLine(filedata, 3, extra))
        {
          XMPINTEGER p,q,d;
          if(p.SetFromString(16, first) && q.SetFromString(16, second) && d.SetFromString(16, third))
            {
              CIPHERKEYPRIVATERSA* key=GEN_NEW CIPHERKEYPRIVATERSA();
              if(key && key->Set(p, q, d)) { privatekey=key; status=true; }
              else if(key) GEN_DELETE key;
            }
        }
    }
#endif
  else
    {
      CIPHERKEYTYPE privatetype=CIPHERKEYTYPE_UNKNOWN;
      XDWORD coordinatesize=0;
      switch(expectedpublickeytype)
        {
          case CIPHERKEYTYPE_ECDSA_SECP256R1_PUBLIC: privatetype=CIPHERKEYTYPE_ECDSA_SECP256R1_PRIVATE; coordinatesize=CIPHERECDSA_P256_COORDINATE_SIZE; break;
          case CIPHERKEYTYPE_ECDSA_SECP384R1_PUBLIC: privatetype=CIPHERKEYTYPE_ECDSA_SECP384R1_PRIVATE; coordinatesize=CIPHERECDSA_P384_COORDINATE_SIZE; break;
          case CIPHERKEYTYPE_ECDSA_SECP521R1_PUBLIC: privatetype=CIPHERKEYTYPE_ECDSA_SECP521R1_PRIVATE; coordinatesize=CIPHERECDSA_P521_COORDINATE_SIZE; break;
                                                   default: break;
        }

      if(coordinatesize && CIPHERCREDENTIALSLOADER_LegacyHexLine(filedata, 0, first) &&
         !CIPHERCREDENTIALSLOADER_LegacyHexLine(filedata, 1, extra))
        {
          XMPINTEGER scalar;
          XBYTE value[CIPHERECDSA_MAXCOORDINATE_SIZE];
          memset(value, 0, sizeof(value));
          if(scalar.SetFromString(16, first) && scalar.ExportToBinary(value, coordinatesize))
            {
              CIPHERKEYECDSA* key=GEN_NEW CIPHERKEYECDSA();
              if(key)
                {
                  key->SetType(privatetype);
                  if(key->Set(value, coordinatesize)) { privatekey=key; status=true; }
                  else GEN_DELETE key;
                }
            }
          volatile XBYTE* wipe=value;
          for(XDWORD c=0;c<sizeof(value);c++) wipe[c]=0;
        }
    }

  CIPHERCREDENTIALSLOADER_StringWipe(first);
  CIPHERCREDENTIALSLOADER_StringWipe(second);
  CIPHERCREDENTIALSLOADER_StringWipe(third);
  CIPHERCREDENTIALSLOADER_StringWipe(extra);
  return status;
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         static int CIPHERCREDENTIALSLOADER_Find(XBYTE* data, XDWORD size, const char* marker, XDWORD from)
* @brief      Find
* @ingroup    CIPHER
* 
* @param[in]  data : Pointer to data.
* @param[in]  size : Size value.
* @param[in]  marker : Pointer to marker.
* @param[in]  from : From value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static int CIPHERCREDENTIALSLOADER_Find(XBYTE* data, XDWORD size, const char* marker, XDWORD from)
{
  XDWORD markersize=(XDWORD)strlen(marker);
  if(!data || !markersize || markersize>size) return -1;
  for(XDWORD c=from;c+markersize<=size;c++) if(!memcmp(data+c, marker, markersize)) return (int)c;
  return -1;
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERCREDENTIALSLOADER::PEMBlocks_Decode(XBUFFER& filedata, const char* label, XVECTOR<XBUFFER*>& blocks)
* @brief      Pem blocks decode
* @ingroup    CIPHER
* 
* @param[in]  filedata : Filedata value.
* @param[in]  label : Pointer to label.
* @param[in]  blocks : Pointer to blocks.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERCREDENTIALSLOADER::PEMBlocks_Decode(XBUFFER& filedata, const char* label, XVECTOR<XBUFFER*>& blocks)
{
  blocks.DeleteContents(); blocks.DeleteAll();
  if(!label || filedata.IsEmpty()) return false;
  char begin[96]; char end[96];
  snprintf(begin, sizeof(begin), "-----BEGIN %s-----", label);
  snprintf(end, sizeof(end), "-----END %s-----", label);
  XDWORD cursor=0;
  while(cursor<filedata.GetSize())
    {
      int b=CIPHERCREDENTIALSLOADER_Find(filedata.Get(), filedata.GetSize(), begin, cursor);
      if(b<0) break;
      XDWORD bodystart=(XDWORD)b+(XDWORD)strlen(begin);
      int e=CIPHERCREDENTIALSLOADER_Find(filedata.Get(), filedata.GetSize(), end, bodystart);
      if(e<0) { Certificates_Delete(blocks); return false; }
      XSTRING base64;
      for(XDWORD c=bodystart;c<(XDWORD)e;c++)
        {
          XBYTE value=filedata.Get()[c];
          if((value==' ')||(value=='\t')||(value=='\r')||(value=='\n')) continue;
          if(value&0x80) { Certificates_Delete(blocks); return false; }
          base64.Add((XCHAR)value);
        }
      XBUFFER* DER=GEN_NEW XBUFFER();
      if(!DER || !DER->ConvertFromBase64(base64) || DER->IsEmpty() || !blocks.Add(DER))
        { if(DER) GEN_DELETE DER; Certificates_Delete(blocks); return false; }
      cursor=(XDWORD)e+(XDWORD)strlen(end);
    }
  return !blocks.IsEmpty();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERCREDENTIALSLOADER::Certificates_Load(XBUFFER& filedata, XVECTOR<XBUFFER*>& certificatechain)
* @brief      Certificates load
* @ingroup    CIPHER
* 
* @param[in]  filedata : Filedata value.
* @param[in]  certificatechain : Pointer to certificatechain.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERCREDENTIALSLOADER::Certificates_Load(XBUFFER& filedata, XVECTOR<XBUFFER*>& certificatechain)
{
  Certificates_Delete(certificatechain);
  XVECTOR<XBUFFER*> blocks;
  bool status=PEMBlocks_Decode(filedata, "CERTIFICATE", blocks);
  if(!status) status=PEMBlocks_Decode(filedata, "PKCS7", blocks);
  if(!status) status=PEMBlocks_Decode(filedata, "CMS", blocks);

  if(status && blocks.GetSize()==1)
    {
      CIPHERCERTIFICATEX509 certificate;
      if(!certificate.Decode((*blocks.Get(0))))
        {
          XVECTOR<XBUFFER*> pkcs7;
          status=CIPHERPEMCODEC::PKCS7Certificates_Decode((*blocks.Get(0)), pkcs7);
          blocks.DeleteContents(); blocks.DeleteAll();
          if(status)
            {
              for(XDWORD c=0;c<pkcs7.GetSize();c++)
                if(!certificatechain.Add(pkcs7.Get(c))) { status=false; break; }
              if(status) pkcs7.DeleteAll();
              else { certificatechain.DeleteAll(); Certificates_Delete(pkcs7); }
            }
          return status;
        }
    }

  if(status)
    {
      for(XDWORD c=0;c<blocks.GetSize();c++)
        {
          CIPHERCERTIFICATEX509 certificate;
          if(!certificate.Decode((*blocks.Get(c)))) { status=false; break; }
        }
      if(status)
        {
          for(XDWORD c=0;c<blocks.GetSize();c++)
            if(!certificatechain.Add(blocks.Get(c))) { status=false; break; }
          if(status) { blocks.DeleteAll(); return true; }
          certificatechain.DeleteAll();
        }
      blocks.DeleteContents(); blocks.DeleteAll(); return false;
    }

  CIPHERCERTIFICATEX509 certificate;
  if(certificate.Decode(filedata))
    {
      XBUFFER* copy=GEN_NEW XBUFFER();
      if(copy && copy->Add(filedata) && certificatechain.Add(copy)) return true;
      if(copy) GEN_DELETE copy;
    }
  if(CIPHERPEMCODEC::PKCS7Certificates_Decode(filedata, certificatechain)) return true;
  Certificates_Delete(certificatechain); return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERCREDENTIALSLOADER::PrivateKeyDER_Decode(XBUFFER& DER, CIPHERKEYTYPE expectedpublickeytype, CIPHERKEY*& privatekey)
* @brief      Private key der decode
* @ingroup    CIPHER
* 
* @param[in]  DER : DER value.
* @param[in]  expectedpublickeytype : Expectedpublickeytype value.
* @param[in]  privatekey : Pointer to privatekey.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERCREDENTIALSLOADER::PrivateKeyDER_Decode(XBUFFER& DER, CIPHERKEYTYPE expectedpublickeytype, CIPHERKEY*& privatekey)
{
  PrivateKey_Delete(privatekey);
  XBUFFER inner;
  bool isrsa=false,isec=false,ised25519=false;
  XBUFFER* actual=&DER;
  bool ispkcs8=CIPHERPEMCODEC::PKCS8PrivateKey_Decode(DER, isrsa, isec, ised25519, inner);
  if(ispkcs8) actual=&inner;

  #ifdef CIPHER_ASYMMETRIC_RSA_ACTIVE
  if((expectedpublickeytype==CIPHERKEYTYPE_RSA_PUBLIC) && (!ispkcs8 || isrsa))
    {
      XMPINTEGER p,q,d;
      if(CIPHERPEMCODEC::RSAPrivateKey_Decode((*actual), p, q, d))
        {
          CIPHERKEYPRIVATERSA* key=GEN_NEW CIPHERKEYPRIVATERSA();
          if(key && key->Set(p, q, d)) { privatekey=key; inner.SecureDelete(); return true; }
          if(key) GEN_DELETE key;
        }
    }
  #endif
  #ifdef CIPHER_ASYMMETRIC_ED25519_ACTIVE

  if((expectedpublickeytype==CIPHERKEYTYPE_ED25519_PUBLIC) && ispkcs8 && ised25519)
    {
      XSECUREBUFFER seed;

      if(CIPHERPEMCODEC::Ed25519PrivateKey_Decode((*actual), seed) &&
         seed.GetSize()==CIPHERED25519_PRIVATEKEYSIZE)
        {
          CIPHERKEYSYMMETRICAL* key=GEN_NEW CIPHERKEYSYMMETRICAL();
          if(key)
            {
              key->SetType(CIPHERKEYTYPE_ED25519_PRIVATE);
              if(key->Set(seed)) privatekey=key;
              else GEN_DELETE key;
            }
        }

      seed.SecureDelete();
      inner.SecureDelete();
      return privatekey?true:false;
    }

  #endif

  else if(!ispkcs8 || isec)
    {
      CIPHERKEYTYPE privatetype=CIPHERKEYTYPE_UNKNOWN; XDWORD coordinatesize=0;
      switch(expectedpublickeytype)
        {
          case CIPHERKEYTYPE_ECDSA_SECP256R1_PUBLIC: privatetype=CIPHERKEYTYPE_ECDSA_SECP256R1_PRIVATE; coordinatesize=CIPHERECDSA_P256_COORDINATE_SIZE; break;
          case CIPHERKEYTYPE_ECDSA_SECP384R1_PUBLIC: privatetype=CIPHERKEYTYPE_ECDSA_SECP384R1_PRIVATE; coordinatesize=CIPHERECDSA_P384_COORDINATE_SIZE; break;
          case CIPHERKEYTYPE_ECDSA_SECP521R1_PUBLIC: privatetype=CIPHERKEYTYPE_ECDSA_SECP521R1_PRIVATE; coordinatesize=CIPHERECDSA_P521_COORDINATE_SIZE; break;
                                                   default: break;
        }
      XSECUREBUFFER scalar;
      if(coordinatesize && CIPHERPEMCODEC::ECPrivateKey_Decode((*actual), scalar) && scalar.GetSize()<=coordinatesize)
        {
          XBYTE value[CIPHERECDSA_MAXCOORDINATE_SIZE]; memset(value, 0, sizeof(value));
          memcpy(value+coordinatesize-scalar.GetSize(), scalar.Get(), scalar.GetSize());
          CIPHERKEYECDSA* key=GEN_NEW CIPHERKEYECDSA();
          if(key) { key->SetType(privatetype); if(key->Set(value, coordinatesize)) privatekey=key; else GEN_DELETE key; }
          volatile XBYTE* wipe=value; for(XDWORD c=0;c<sizeof(value);c++) wipe[c]=0;
          scalar.SecureDelete(); inner.SecureDelete();
          return privatekey?true:false;
        }
    }
  inner.SecureDelete(); return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         static bool CIPHERCREDENTIALSLOADER_PrivateKeyMatchesPublic(CIPHERKEY* privatekey, CIPHERKEY* publickey)
* @brief      Private key matches public
* @ingroup    CIPHER
* 
* @param[in]  privatekey : Pointer to privatekey.
* @param[in]  publickey : Pointer to publickey.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool CIPHERCREDENTIALSLOADER_PrivateKeyMatchesPublic(CIPHERKEY* privatekey, CIPHERKEY* publickey)
{
  if(!privatekey || !publickey) return false;

  #ifdef CIPHER_ASYMMETRIC_RSA_ACTIVE

  if(publickey->GetType()==CIPHERKEYTYPE_RSA_PUBLIC)
    {
      if(privatekey->GetType()!=CIPHERKEYTYPE_RSA_PRIVATE) return false;

      return ((CIPHERKEYPRIVATERSA*)privatekey)->Check((*((CIPHERKEYPUBLICRSA*)publickey)));
    }

  #endif

  CIPHERTYPE ecdsatype=CIPHERTYPE_ECDSA_SECP256R1;
  CIPHERKEYTYPE expectedprivatetype=CIPHERKEYTYPE_UNKNOWN;
  bool isecdsa=false;

  switch(publickey->GetType())
    {
      case CIPHERKEYTYPE_ECDSA_SECP256R1_PUBLIC: ecdsatype=CIPHERTYPE_ECDSA_SECP256R1; expectedprivatetype=CIPHERKEYTYPE_ECDSA_SECP256R1_PRIVATE; isecdsa=true; break;
      case CIPHERKEYTYPE_ECDSA_SECP384R1_PUBLIC: ecdsatype=CIPHERTYPE_ECDSA_SECP384R1; expectedprivatetype=CIPHERKEYTYPE_ECDSA_SECP384R1_PRIVATE; isecdsa=true; break;
      case CIPHERKEYTYPE_ECDSA_SECP521R1_PUBLIC: ecdsatype=CIPHERTYPE_ECDSA_SECP521R1; expectedprivatetype=CIPHERKEYTYPE_ECDSA_SECP521R1_PRIVATE; isecdsa=true; break;
                                                default: break;
    }

  if(isecdsa)
    {
      if(privatekey->GetType()!=expectedprivatetype) return false;

      CIPHERECDSA ecdsa(ecdsatype);
      return ecdsa.SetKey(publickey, true) && ecdsa.SetKey(privatekey, true);
    }

  #ifdef CIPHER_ASYMMETRIC_ED25519_ACTIVE

  if(publickey->GetType()==CIPHERKEYTYPE_ED25519_PUBLIC)
    {
      if(privatekey->GetType()!=CIPHERKEYTYPE_ED25519_PRIVATE) return false;

      CIPHERKEYSYMMETRICAL* privateed=(CIPHERKEYSYMMETRICAL*)privatekey;
      CIPHERKEYSYMMETRICAL* publiced =(CIPHERKEYSYMMETRICAL*)publickey;
      if(!privateed->Get() || !publiced->Get() ||
         privateed->Get()->GetSize()!=CIPHERED25519_PRIVATEKEYSIZE ||
         publiced->Get()->GetSize()!=CIPHERED25519_PUBLICKEYSIZE) return false;

      XSECUREBUFFER derived;
      CIPHERED25519 ed25519;
      bool status=ed25519.PublicKey_Create((*privateed->Get()), derived) &&
                  CIPHER::CompareConstantTime(derived.Get(), publiced->Get()->Get(), CIPHERED25519_PUBLICKEYSIZE);
      derived.SecureDelete();
      return status;
    }

  #endif

  return false;
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERCREDENTIALSLOADER::PrivateKey_Load(XBUFFER& filedata, XCHAR* password, CIPHERKEYTYPE expectedpublickeytype, CIPHERKEY*& privatekey)
* @brief      Private key load
* @ingroup    CIPHER
* 
* @param[in]  filedata : Filedata value.
* @param[in]  password : Pointer to password.
* @param[in]  expectedpublickeytype : Expectedpublickeytype value.
* @param[in]  privatekey : Pointer to privatekey.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERCREDENTIALSLOADER::PrivateKey_Load(XBUFFER& filedata, XCHAR* password, CIPHERKEYTYPE expectedpublickeytype, CIPHERKEY*& privatekey)
{
  PrivateKey_Delete(privatekey);
  XVECTOR<XBUFFER*> blocks;
  XSECUREBUFFER decrypted;
  bool encrypted=false;
  bool status=PEMBlocks_Decode(filedata, "ENCRYPTED PRIVATE KEY", blocks);
  if(status) encrypted=true;
  if(!status) status=PEMBlocks_Decode(filedata, "PRIVATE KEY", blocks);
  if(!status) status=PEMBlocks_Decode(filedata, "RSA PRIVATE KEY", blocks);
  if(!status) status=PEMBlocks_Decode(filedata, "EC PRIVATE KEY", blocks);

  XBUFFER* DER=NULL;
  if(status && blocks.GetSize()==1) DER=blocks.Get(0);
  else if(!status && !CIPHERPEMCODEC::IsPEM(filedata)) { DER=&filedata; status=true; }
  else status=false;

  if(status && encrypted)
    {
      XSTRING secret; if(password) secret.Set(password);
      status=password && CIPHERPEMCODEC::PKCS8EncryptedPrivateKey_Decode((*DER), secret, decrypted);
      if(secret.Get()) { volatile XCHAR* wipe=secret.Get(); for(XDWORD c=0;c<secret.GetSize();c++) wipe[c]=0; } secret.Empty();
      DER=&decrypted;
    }
  else if(!CIPHERPEMCODEC::IsPEM(filedata) && password)
    {
      XSTRING secret(password);
      if(CIPHERPEMCODEC::PKCS8EncryptedPrivateKey_Decode(filedata, secret, decrypted)) { DER=&decrypted; status=true; }
      if(secret.Get()) { volatile XCHAR* wipe=secret.Get(); for(XDWORD c=0;c<secret.GetSize();c++) wipe[c]=0; } secret.Empty();
    }

  if(status && DER)
    {
      status=PrivateKeyDER_Decode((*DER), expectedpublickeytype, privatekey);
      if(!status && (DER==&filedata)) status=CIPHERCREDENTIALSLOADER_LegacyPrivateKey(filedata, expectedpublickeytype, privatekey);
    }
  for(XDWORD c=0;c<blocks.GetSize();c++) if(blocks.Get(c)) blocks.Get(c)->SecureDelete();
  blocks.DeleteContents(); blocks.DeleteAll(); decrypted.SecureDelete();
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERCREDENTIALSLOADER::Credentials_Load(XBUFFER& certificatedata, XBUFFER& privatekeydata, XCHAR* password, XVECTOR<XBUFFER*>& certificatechain, CIPHERKEY*& privatekey)
* @brief      Credentials load
* @ingroup    CIPHER
* 
* @param[in]  certificatedata : Certificatedata value.
* @param[in]  privatekeydata : Privatekeydata value.
* @param[in]  password : Pointer to password.
* @param[in]  certificatechain : Pointer to certificatechain.
* @param[in]  privatekey : Pointer to privatekey.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERCREDENTIALSLOADER::Credentials_Load(XBUFFER& certificatedata, XBUFFER& privatekeydata, XCHAR* password, XVECTOR<XBUFFER*>& certificatechain, CIPHERKEY*& privatekey)
{
  Certificates_Delete(certificatechain); PrivateKey_Delete(privatekey);

  XSTRING secret; if(password) secret.Set(password);
  XSECUREBUFFER pfxprivatekey;
  if(CIPHERPEMCODEC::PKCS12_Decode(privatekeydata, secret, certificatechain, pfxprivatekey) ||
     ((&certificatedata!=&privatekeydata) && CIPHERPEMCODEC::PKCS12_Decode(certificatedata, secret, certificatechain, pfxprivatekey)))
    {
      bool loaded=false;
      XDWORD leafindex=0;

      for(XDWORD c=0; c<certificatechain.GetSize() && !loaded; c++)
        {
          XBUFFER* certificateder=certificatechain.Get(c);
          CIPHERCERTIFICATEX509 certificate;
          CIPHERKEY* publickey=NULL;

          if(!certificateder || !certificate.Decode((*certificateder))) continue;
          publickey=certificate.GetPublicCipherKey();
          if(!publickey) continue;

          if(PrivateKeyDER_Decode(pfxprivatekey, publickey->GetType(), privatekey) &&
             CIPHERCREDENTIALSLOADER_PrivateKeyMatchesPublic(privatekey, publickey))
            {
              loaded=true;
              leafindex=c;
            }
        }

      if(loaded && leafindex) loaded=certificatechain.Swap(0, leafindex);

      pfxprivatekey.SecureDelete();
      if(secret.Get()){volatile XCHAR* wipe=secret.Get();for(XDWORD c=0;c<secret.GetSize();c++)wipe[c]=0;} secret.Empty();
      if(loaded) return true;
      Certificates_Delete(certificatechain); PrivateKey_Delete(privatekey);
      return false;
    }
  pfxprivatekey.SecureDelete();
  if(secret.Get()){volatile XCHAR* wipe=secret.Get();for(XDWORD c=0;c<secret.GetSize();c++)wipe[c]=0;} secret.Empty();

  if(!Certificates_Load(certificatedata, certificatechain)) return false;
  CIPHERCERTIFICATEX509 leaf;
  if(!certificatechain.Get(0) || !leaf.Decode((*certificatechain.Get(0))) || !leaf.GetPublicCipherKey() ||
     !PrivateKey_Load(privatekeydata, password, leaf.GetPublicCipherKey()->GetType(), privatekey) ||
     !CIPHERCREDENTIALSLOADER_PrivateKeyMatchesPublic(privatekey, leaf.GetPublicCipherKey()))
    { Certificates_Delete(certificatechain); PrivateKey_Delete(privatekey); return false; }
  return true;
}
