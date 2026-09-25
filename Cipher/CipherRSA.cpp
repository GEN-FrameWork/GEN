/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       CipherRSA.cpp
* 
* @class      CIPHERRSA
* @brief      Cipher RSA class
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

#include "CipherRSA.h"

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "XFactory.h"
#include "XRand.h"
#include "XBER.h"

#include "Hash.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHERRSA_CONTEXT::CIPHERRSA_CONTEXT()
* @brief      Constructor of class
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERRSA_CONTEXT::CIPHERRSA_CONTEXT()

{
  Ini();
  Clean();
};


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHERRSA_CONTEXT::~CIPHERRSA_CONTEXT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERRSA_CONTEXT::~CIPHERRSA_CONTEXT()
{
  End();
  Clean();
};


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void CIPHERRSA_CONTEXT::Ini()
* @brief      Initialize the object
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
void CIPHERRSA_CONTEXT::Ini()
{
  N.Ini();
  E.Ini();
  D.Ini();
  P.Ini();
  Q.Ini();
  DP.Ini();
  DQ.Ini();
  QP.Ini();
  RN.Ini();
  RP.Ini();
  RQ.Ini();
  #if !defined(CIPHERRSA_RSANOCRT)
  Vi.Ini();
  Vf.Ini();
  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void CIPHERRSA_CONTEXT::End()
* @brief      End the object
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
void CIPHERRSA_CONTEXT::End()
{
  N.End();
  E.End();
  D.End();
  P.End();
  Q.End();
  DP.End();
  DQ.End();
  QP.End();
  RN.End();
  RP.End();
  RQ.End();
  #if !defined(CIPHERRSA_RSANOCRT)
  Vi.End();
  Vf.End();
  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void CIPHERRSA_CONTEXT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
void CIPHERRSA_CONTEXT::Clean()
{
  len     = 0;
  padding = 0;
  hashID  = 0;
}






/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHERRSA::CIPHERRSA() : CIPHER()
* @brief      Constructor of class
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERRSA::CIPHERRSA() : CIPHER()
{
  Clean();

  type = CIPHERTYPE_RSA;

  xrand=GEN_XFACTORY.CreateRand();
  if(xrand && (!xrand->Ini() || !xrand->IsCryptographicallySecure()))
    {
      GEN_XFACTORY.DeleteRand(xrand);
      xrand = NULL;
    }

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHERRSA::~CIPHERRSA()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERRSA::~CIPHERRSA()
{
  GEN_XFACTORY.DeleteRand(xrand);

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERRSA::SetKey(CIPHERKEY* key, bool integritycheck)
* @brief      Set key
* @ingroup    CIPHER
* 
* @param[in]  key : Key pointer to use.
* @param[in]  integritycheck : Integritycheck value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERRSA::SetKey(CIPHERKEY* key, bool integritycheck)
{
  if(!key) return false;

  switch(key->GetType())
    {
      case CIPHERKEYTYPE_SYMMETRICAL  : return false;

      case CIPHERKEYTYPE_RSA_PUBLIC   : { CIPHERKEYPUBLICRSA* publickey = (CIPHERKEYPUBLICRSA*)key;

                                          publickey->Get(context.N, context.E); // Assign key public to context

                                          if(integritycheck)
                                            {
                                              if(!CheckContextPublicKey()) return false;
                                            }
                                        }
                                        break;

      case CIPHERKEYTYPE_RSA_PRIVATE  : { CIPHERKEYPRIVATERSA* privatekey = (CIPHERKEYPRIVATERSA*)key;
                                          XMPINTEGER           P1;
                                          XMPINTEGER           Q1;

                                          privatekey->Get(context.P, context.Q, context.D);

                                          // Modulus N is not stored in the private key object; derive it so
                                          // Sign/Uncipher and GetKeySizeInBytes work when only the private key is set.
                                          if(!context.N.Multiplication(&context.P, &context.Q))     return false;
                                          context.len = (context.N.GetMSB() + 7) >> 3;

                                          if(!P1.SubtractionSigned(&context.P, 1))                  return false;
                                          if(!Q1.SubtractionSigned(&context.Q, 1))                  return false;
                                          if(!context.DP.Module(&context.DP, &context.D, &P1))      return false;
                                          if(!context.DQ.Module(&context.DQ, &context.D, &Q1))      return false;
                                          if(!context.QP.ModularInverse(&context.Q, &context.P))    return false;

                                          if(integritycheck)
                                            {
                                              if(!CheckContextPrivateKey()) return false;
                                            }

                                        }
                                        break;
                          default     : break;

    }

  return CIPHER::SetKey(key);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERRSA::Cipher(XBYTE* input, XDWORD size, CIPHERKEYTYPE keytouse, CIPHERRSAPKCS1VERSION pkcs1version)
* @brief      Cipher
* @ingroup    CIPHER
* 
* @param[in]  input : Input pointer to use.
* @param[in]  size : Size value.
* @param[in]  keytouse : Keytouse value.
* @param[in]  pkcs1version : Pkcs1version value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERRSA::Cipher(XBYTE* input, XDWORD size, CIPHERKEYTYPE keytouse, CIPHERRSAPKCS1VERSION pkcs1version)
{
  if(!size) return false;

  int keysize   = GetKeySizeInBytes(keytouse);
  if(!keysize) return false;

  XBUFFER _input;
  XBUFFER output;
  XBUFFER inputparcial;
  int     sizeblock = (keysize-3);
  int     rest      = (size%sizeblock);
  int     nblocks   = (size/sizeblock);
  bool    status    = false;

  _input.Add(input, size);

  result->Delete();

  for(int c=0; c<(nblocks+(rest?1:0)); c++)
    {
      int dosizeblock = sizeblock;

      inputparcial.Delete();
      output.Delete();

      if(c==nblocks) dosizeblock = rest;

      inputparcial.Add(&_input.Get()[sizeblock*c], dosizeblock);

      switch(pkcs1version)
        {
          case CIPHERRSAPKCS1VERSIONV15   : status = Cipher_PKCS1_V15(inputparcial.Get(), dosizeblock,  output, keytouse, pkcs1version, GenerateRandom, xrand);
                                            result->Add(output);
                                            break;

          case CIPHERRSAPKCS1VERSIONV21   : break;
        }
    }


  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERRSA::Cipher(XBUFFER& input, CIPHERKEYTYPE keytouse, CIPHERRSAPKCS1VERSION pkcs1version)
* @brief      Cipher
* @ingroup    CIPHER
* 
* @param[in]  input : Input value.
* @param[in]  keytouse : Keytouse value.
* @param[in]  pkcs1version : Pkcs1version value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERRSA::Cipher(XBUFFER& input, CIPHERKEYTYPE keytouse, CIPHERRSAPKCS1VERSION pkcs1version)
{
  return Cipher(input.Get(), input.GetSize(), keytouse, pkcs1version);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERRSA::Uncipher(XBYTE* input,XDWORD size, CIPHERKEYTYPE keytouse, CIPHERRSAPKCS1VERSION pkcs1version)
* @brief      Uncipher
* @ingroup    CIPHER
* 
* @param[in]  input : Input pointer to use.
* @param[in]  size : Size value.
* @param[in]  keytouse : Keytouse value.
* @param[in]  pkcs1version : Pkcs1version value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERRSA::Uncipher(XBYTE* input,XDWORD size, CIPHERKEYTYPE keytouse, CIPHERRSAPKCS1VERSION pkcs1version)
{
  if(!size) return false;

  int keysize = GetKeySizeInBytes(keytouse);
  if(!keysize) return false;

  XBUFFER _input;
  XBUFFER output;
  XBUFFER inputparcial;
  int     sizeblock = keysize;
  int     rest      = (size%sizeblock);
  int     nblocks   = (size/sizeblock);
  bool    status    = false;

  _input.Add(input, size);

  result->Delete();

  for(int c=0; c<(nblocks+(rest?1:0)); c++)
    {
      int dosizeblock = sizeblock;

      inputparcial.Delete();
      output.Delete();

      if(c==nblocks) dosizeblock = rest;

      inputparcial.Add(&_input.Get()[sizeblock*c], dosizeblock);

      switch(pkcs1version)
        {
          case CIPHERRSAPKCS1VERSIONV15   : status = Uncipher_PKCS1_V15(inputparcial.Get(), dosizeblock,  output, keytouse, pkcs1version, GenerateRandom, xrand);
                                            result->Add(output);
                                            break;

          case CIPHERRSAPKCS1VERSIONV21   : break;
        }
    }


  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERRSA::Uncipher(XBUFFER& input, CIPHERKEYTYPE keytouse, CIPHERRSAPKCS1VERSION pkcs1version)
* @brief      Uncipher
* @ingroup    CIPHER
* 
* @param[in]  input : Input value.
* @param[in]  keytouse : Keytouse value.
* @param[in]  pkcs1version : Pkcs1version value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERRSA::Uncipher(XBUFFER& input, CIPHERKEYTYPE keytouse, CIPHERRSAPKCS1VERSION pkcs1version)
{
  return Uncipher(input.Get(), input.GetSize(), keytouse, pkcs1version);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERRSA::Sign(XBYTE* input, XDWORD size, CIPHERKEYTYPE keytouse, HASH* hash, CIPHERRSAPKCS1VERSION pkcs1version)
* @brief      Sign
* @ingroup    CIPHER
* 
* @param[in]  input : Input pointer to use.
* @param[in]  size : Size value.
* @param[in]  keytouse : Keytouse value.
* @param[in]  hash : Hash pointer to use.
* @param[in]  pkcs1version : Pkcs1version value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERRSA::Sign(XBYTE* input, XDWORD size, CIPHERKEYTYPE keytouse, HASH* hash, CIPHERRSAPKCS1VERSION pkcs1version)
{
  if(!hash) return false;

  switch(pkcs1version)
    {
      case CIPHERRSAPKCS1VERSIONV15 : return Sign_PKCS1_V15(input, size, keytouse, hash);
      case CIPHERRSAPKCS1VERSIONV21 : return Sign_PKCS1_V21(input, size, hash, 0);
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool CIPHERRSA::Sign_PKCS1_V15(XBYTE* input, XDWORD size, CIPHERKEYTYPE keytouse, HASH* hash)
* @brief      Sign with EMSA-PKCS1-v1_5 (DigestInfo BER structure, then PKCS#1 v1.5 padding)
* @ingroup    CIPHER
*
* @param[in]  input : Data to sign.
* @param[in]  size : Data size.
* @param[in]  keytouse : Keytouse value. Unused: signing always uses the private key.
* @param[in]  hash : Hash algorithm to use.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERRSA::Sign_PKCS1_V15(XBYTE* input, XDWORD size, CIPHERKEYTYPE keytouse, HASH* hash)
{
  XBER      beroui;
  XBER      bernull;
  XBER      berhash;
  XBER      berseq1;
  XBER      berseq2;
  XSTRING*  OUINoSignstring = hash->GetOUINoSign();
  XBUFFER   sign;

  if(!OUINoSignstring) return false;

  if(!OUINoSignstring->GetSize()) return false;

  beroui.SetOID(OUINoSignstring->Get());
  bernull.SetNULL();

  hash->Do((XBYTE*)input, size);

  berhash.SetOCTETSTRING((*hash->GetResult()));

  berseq1.Sequence_AddTo(beroui);
  berseq1.Sequence_AddTo(bernull);

  berseq1.SetTagType(berseq1.GetTagType()|(XBYTE)(XBERTYPE_ISCONSTRUCTED));

  berseq2.Sequence_AddTo(berseq1);
  berseq2.Sequence_AddTo(berhash);
  berseq2.SetTagType(berseq2.GetTagType()|(XBYTE)(XBERTYPE_ISCONSTRUCTED));

  berseq2.GetDump(sign);

  return Cipher(sign, CIPHERKEYTYPE_RSA_PRIVATE, CIPHERRSAPKCS1VERSIONV15);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool CIPHERRSA::Sign_PKCS1_V21(XBYTE* input, XDWORD size, HASH* hash, XDWORD saltsize)
* @brief      Sign with EMSA-PSS (RFC 8017 9.1.1), the mirror of Verify_PKCS1_V21
* @ingroup    CIPHER
*
* @param[in]  input : Data to sign.
* @param[in]  size : Data size.
* @param[in]  hash : Hash algorithm to use.
* @param[in]  saltsize : Exact PSS salt size. Zero selects the hash size (RSASSA-PSS-RSAE, RFC 8446 4.2.3).
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERRSA::Sign_PKCS1_V21(XBYTE* input, XDWORD size, HASH* hash, XDWORD saltsize)
{
  XBUFFER messagehash;
  XBUFFER salt;
  XBUFFER M;
  XBUFFER H;
  XBUFFER DB;
  XBUFFER DBmask;
  XBUFFER maskedDB;
  XBUFFER EM;
  XBUFFER signature;
  XDWORD  hashsize;
  XDWORD  modulusbits;
  XDWORD  encodedbits;
  XDWORD  encodedsize;
  XDWORD  datablocksize;
  XDWORD  unusedbits;
  int     keysize;

  if(!input || !size || !hash)
    {
      return false;
    }

  keysize  = GetKeySizeInBytes(CIPHERKEYTYPE_RSA_PRIVATE);
  hashsize = (XDWORD)hash->GetDefaultSize();

  if(!keysize || !hashsize)
    {
      return false;
    }

  if(!saltsize)
    {
      saltsize = hashsize;
    }

  modulusbits = context.N.GetMSB();
  if(modulusbits < 2)
    {
      return false;
    }

  encodedbits = modulusbits - 1;
  encodedsize = (encodedbits + 7) / 8;

  if(encodedsize < (hashsize + saltsize + 2))
    {
      return false;
    }

  if(!hash->ResetResult() || !hash->Do(input, size) || !messagehash.Add((*hash->GetResult())))
    {
      return false;
    }

  if(!salt.Resize(saltsize) || !GenerateRandom(salt.Get(), saltsize, xrand))
    {
      return false;
    }

  for(int c=0; c<8; c++)
    {
      if(!M.Add((XBYTE)0x00))
        {
          return false;
        }
    }

  if(!M.Add(messagehash) || !M.Add(salt) ||
     !hash->ResetResult() || !hash->Do(M) || !H.Add((*hash->GetResult())) || (H.GetSize() != hashsize))
    {
      return false;
    }

  datablocksize = encodedsize - hashsize - 1;

  for(XDWORD c=0; c<(datablocksize - saltsize - 1); c++)
    {
      if(!DB.Add((XBYTE)0x00))
        {
          return false;
        }
    }

  if(!DB.Add((XBYTE)0x01) || !DB.Add(salt) || (DB.GetSize() != datablocksize))
    {
      return false;
    }

  if(!MaskGenerationFunction1(H, datablocksize, hash, DBmask))
    {
      return false;
    }

  if(!maskedDB.Resize(datablocksize))
    {
      return false;
    }

  for(XDWORD c=0; c<datablocksize; c++)
    {
      maskedDB.Set((XBYTE)(DB.GetByte(c) ^ DBmask.GetByte(c)), c);
    }

  unusedbits = (8 * encodedsize) - encodedbits;
  if(unusedbits)
    {
      maskedDB.Set((XBYTE)(maskedDB.GetByte(0) & (0xFF >> unusedbits)), 0);
    }

  if(!EM.Add(maskedDB) || !EM.Add(H) || !EM.Add((XBYTE)0xBC) || (EM.GetSize() != encodedsize))
    {
      return false;
    }

  if(!DoRSAPrivateOperation(EM, signature, GenerateRandom, xrand) || (signature.GetSize() != (XDWORD)keysize))
    {
      return false;
    }

  result->Delete();
  if(!result->Add(signature))
    {
      return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERRSA::Sign(XBUFFER& input, CIPHERKEYTYPE keytouse, HASH* hash, CIPHERRSAPKCS1VERSION pkcs1version)
* @brief      Sign
* @ingroup    CIPHER
* 
* @param[in]  input : Input value.
* @param[in]  keytouse : Keytouse value.
* @param[in]  hash : Hash pointer to use.
* @param[in]  pkcs1version : Pkcs1version value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERRSA::Sign(XBUFFER& input, CIPHERKEYTYPE keytouse, HASH* hash, CIPHERRSAPKCS1VERSION pkcs1version)
{
  return Sign(input.Get(), input.GetSize(), keytouse, hash, pkcs1version);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool CIPHERRSA::Verify(XBYTE* input, XDWORD size, XBUFFER& signature, HASH* hash, CIPHERRSAPKCS1VERSION pkcs1version, XDWORD saltsize)
* @brief      Verify an RSA signature
* @ingroup    CIPHER
*
* @param[in]  input : Signed data.
* @param[in]  size : Signed data size.
* @param[in]  signature : RSA signature.
* @param[in]  hash : Hash algorithm used by the signature.
* @param[in]  pkcs1version : PKCS#1 signature encoding.
* @param[in]  saltsize : Exact PSS salt size. Zero selects the hash size.
*
* @return     bool : true if the signature is valid; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERRSA::Verify(XBYTE* input, XDWORD size, XBUFFER& signature, HASH* hash, CIPHERRSAPKCS1VERSION pkcs1version, XDWORD saltsize)
{
  if(!input || !size || !hash || signature.IsEmpty())
    {
      return false;
    }

  switch(pkcs1version)
    {
      case CIPHERRSAPKCS1VERSIONV15 : return Verify_PKCS1_V15(input, size, signature, hash);
      case CIPHERRSAPKCS1VERSIONV21 : return Verify_PKCS1_V21(input, size, signature, hash, saltsize);
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool CIPHERRSA::Verify(XBUFFER& input, XBUFFER& signature, HASH* hash, CIPHERRSAPKCS1VERSION pkcs1version, XDWORD saltsize)
* @brief      Verify an RSA signature
* @ingroup    CIPHER
*
* @param[in]  input : Signed data.
* @param[in]  signature : RSA signature.
* @param[in]  hash : Hash algorithm used by the signature.
* @param[in]  pkcs1version : PKCS#1 signature encoding.
* @param[in]  saltsize : Exact PSS salt size. Zero selects the hash size.
*
* @return     bool : true if the signature is valid; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERRSA::Verify(XBUFFER& input, XBUFFER& signature, HASH* hash, CIPHERRSAPKCS1VERSION pkcs1version, XDWORD saltsize)
{
  return Verify(input.Get(), input.GetSize(), signature, hash, pkcs1version, saltsize);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XRAND* CIPHERRSA::GetXRand()
* @brief      Get X rand
* @ingroup    CIPHER
* 
* @return     XRAND* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XRAND* CIPHERRSA::GetXRand()
{ 
  return xrand;    
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERRSA::GenerateKeys(XDWORD nbits, int exponent, CIPHERKEYPUBLICRSA& publickey, CIPHERKEYPRIVATERSA& privatekey, XMPINTEGER_FUNCRANDOM funcrandom, void* paramrandom)
* @brief      Generate keys
* @ingroup    CIPHER
* 
* @param[in]  nbits : Nbits value.
* @param[in]  exponent : Exponent value.
* @param[in]  publickey : Publickey value.
* @param[in]  privatekey : Privatekey value.
* @param[in]  funcrandom : Funcrandom value.
* @param[in]  paramrandom : Paramrandom pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERRSA::GenerateKeys(XDWORD nbits, int exponent, CIPHERKEYPUBLICRSA& publickey, CIPHERKEYPRIVATERSA& privatekey, XMPINTEGER_FUNCRANDOM funcrandom, void* paramrandom)
{
  XMPINTEGER P1;
  XMPINTEGER Q1;
  XMPINTEGER H;
  XMPINTEGER G;
  bool       status;

  // Reject modulus sizes that are no longer safe to generate (see CIPHERRSA_MINKEYBITS). This only bounds keys
  // generated by this function; it does not affect loading/using an existing (e.g. legacy) key from a file.
  if(nbits<CIPHERRSA_MINKEYBITS || exponent<3 )  return false;

  P1.Ini();
  Q1.Ini();
  H.Ini();
  G.Ini();

  // find primes P and Q with Q < P so that: GCD( E, (P-1)*(Q-1) ) == 1
  status = context.E.LeftSet(exponent);
  if(status)
    {
      do{ if(!context.P.GeneratePrime((nbits+1)>>1, 0, funcrandom?funcrandom:(XMPINTEGER_FUNCRANDOM)GenerateRandom, funcrandom?paramrandom:xrand)) return false;
          if(!context.Q.GeneratePrime((nbits+1)>>1, 0, funcrandom?funcrandom:(XMPINTEGER_FUNCRANDOM)GenerateRandom, funcrandom?paramrandom:xrand)) return false;

          if(context.P.CompareSignedValues(context.Q) < 0)  context.P.Swap(&context.Q);

          if(context.P.CompareSignedValues(context.Q) == 0) continue;

          if(!context.N.Multiplication(&context.P, &context.Q)) return false;

          if(context.N.GetMSB()!=nbits) continue;

          if(!P1.SubtractionSigned(&context.P, 1)) return false;
          if(!Q1.SubtractionSigned(&context.Q, 1)) return false;

          if(!H.Multiplication(&P1, &Q1)) return false;

          if(!G.GreatestCommonDivisor(&G, &context.E, &H)) return false;

        } while(G.CompareSignedValues(1) != 0);


      // D  = E^-1 mod ((P-1)*(Q-1))
      // DP = D mod (P - 1)
      // DQ = D mod (Q - 1)
      // QP = Q^-1 mod P

      if(!context.D.ModularInverse(&context.E, &H))           return false;
      if(!context.DP.Module(&context.DP, &context.D, &P1))    return false;
      if(!context.DQ.Module(&context.DQ, &context.D, &Q1))    return false;
      if(!context.QP.ModularInverse(&context.Q, &context.P))  return false;

      context.len = (context.N.GetMSB() + 7) >> 3;
    }

  P1.End();
  Q1.End();
  H.End();
  G.End();

  if(!status)
    {
      context.End();
    }
   else
    {
      publickey.Set(context.N, context.E);
      privatekey.Set(context.P, context.Q, context.D);
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERRSA::CheckContextPublicKey()
* @brief      Check context public key
* @ingroup    CIPHER
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERRSA::CheckContextPublicKey()
{
  if(!context.N.GetLimbs()      || !context.E.GetLimbs())                       return false;
  if((context.N.GetLimbs()[0] & 1) == 0 || (context.E.GetLimbs()[0] & 1 ) == 0) return false;
  if((context.N.GetMSB() < 128) || (context.N.GetMSB() >  XMPINTEGER_MAXBITS )) return false;
  if((context.E.GetMSB() < 2)   || (context.E.GetMSB() >  64 ))                 return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERRSA::CheckContextPrivateKey()
* @brief      Check context private key
* @ingroup    CIPHER
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERRSA::CheckContextPrivateKey()
{
  XMPINTEGER PQ;
  XMPINTEGER DE;
  XMPINTEGER P1;
  XMPINTEGER Q1;
  XMPINTEGER H;
  XMPINTEGER I;
  XMPINTEGER G;
  XMPINTEGER G2;
  XMPINTEGER L1;
  XMPINTEGER L2;
  XMPINTEGER DP;
  XMPINTEGER DQ;
  XMPINTEGER QP;

  if(!CheckContextPublicKey()) return false;

  if((!context.P.GetLimbs()) || (!context.Q.GetLimbs()) || (!context.D.GetLimbs()))  return false;

  PQ.Ini();
  DE.Ini();
  P1.Ini();
  Q1.Ini();
  H.Ini();
  I.Ini();
  G.Ini();
  G2.Ini();
  L1.Ini();
  L2.Ini();
  DP.Ini();
  DQ.Ini();
  QP.Ini();

  if(!PQ.Multiplication(&context.P, &context.Q))    return false;
  if(!DE.Multiplication(&context.D, &context.E))    return false;
  if(!P1.SubtractionSigned(&context.P, 1))          return false;
  if(!Q1.SubtractionSigned(&context.Q, 1))          return false;
  if(!H.Multiplication(&P1, &Q1))                   return false;
  if(!G.GreatestCommonDivisor(&G, &context.E, &H))  return false;
  if(!G2.GreatestCommonDivisor(&G2, &P1, &Q1))      return false;
  if(!L1.Division(&L1, &L2, &H, &G2 ))              return false;
  if(!I.Module(&I, &DE, &L1))                       return false;
  if(!DP.Module( &DP, &context.D, &P1))             return false;
  if(!DQ.Module(&DQ, &context.D, &Q1))              return false;
  if(!QP.ModularInverse(&context.Q, &context.P))    return false;

  // Check for a valid PKCS1v2 private key

  if((PQ.CompareSignedValues(context.N ) != 0) || (DP.CompareSignedValues(context.DP) != 0) ||
     (DQ.CompareSignedValues(context.DQ) != 0) || (QP.CompareSignedValues(context.QP) != 0) ||
     (L2.CompareSignedValues(0)          != 0) || (I.CompareSignedValues(1)           != 0) ||
     (G.CompareSignedValues(1)           != 0)) return false;

  PQ.End();
  DE.End();
  P1.End();
  Q1.End();
  H.End();
  I.End();
  G.End();
  G2.End();
  L1.End();
  L2.End();
  DP.End();
  DQ.End();
  QP.End();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERRSA::Cipher_PKCS1_V15(XBYTE* buffer, XDWORD size, XBUFFER& output, CIPHERKEYTYPE keytypetouse, CIPHERRSAPKCS1VERSION pkcs1version, XMPINTEGER_FUNCRANDOM funcrandom, void* paramrandom)
* @brief      Cipher PKCS1V15
* @ingroup    CIPHER
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* @param[in]  output : Output output.
* @param[in]  keytypetouse : Keytypetouse value.
* @param[in]  pkcs1version : Pkcs1version value.
* @param[in]  funcrandom : Funcrandom value.
* @param[in]  paramrandom : Paramrandom pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERRSA::Cipher_PKCS1_V15(XBYTE* buffer, XDWORD size, XBUFFER& output, CIPHERKEYTYPE keytypetouse, CIPHERRSAPKCS1VERSION pkcs1version,  XMPINTEGER_FUNCRANDOM funcrandom, void* paramrandom)
{
  XBUFFER input;
  int     nb_pad;
  bool    status;

  if(!funcrandom) return false;

  int keysize = GetKeySizeInBytes(keytypetouse);
  if(!keysize) return false;

  if(keysize < (int)(size + 11)) return false;

  nb_pad = keysize - 3 - size;

  input.Add((XBYTE)0);

  if(keytypetouse == CIPHERKEYTYPE_RSA_PUBLIC)
    {
      XBYTE data = 0;

      input.Add((XBYTE)CIPHERRSA_CIPHERDATA);

       while(nb_pad > 0)
        {
          if(funcrandom(&data, 1, paramrandom))
            {
              if(!data) continue;
              input.Add((XBYTE)data);
              nb_pad--;
            }
        }
    }
   else
    {
      input.Add((XBYTE)CIPHERRSA_SINGDATA);

       while(nb_pad-- > 0)
         {
           input.Add((XBYTE)0xFF);
         }
    }


  input.Add((XBYTE)0);
  input.Add(buffer,size);

  status = false;

  switch(keytypetouse)
    {
      case CIPHERKEYTYPE_RSA_PUBLIC   : status = DoRSAPublicOperation(input, output);                              break;
      case CIPHERKEYTYPE_RSA_PRIVATE  : status = DoRSAPrivateOperation(input, output, funcrandom, paramrandom);    break;
                            default   : break;
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERRSA::Uncipher_PKCS1_V15(XBYTE* buffer, XDWORD size, XBUFFER& output, CIPHERKEYTYPE keytypetouse, CIPHERRSAPKCS1VERSION pkcs1version, XMPINTEGER_FUNCRANDOM funcrandom, void* paramrandom)
* @brief      Uncipher PKCS1V15
* @ingroup    CIPHER
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* @param[in]  output : Output output.
* @param[in]  keytypetouse : Keytypetouse value.
* @param[in]  pkcs1version : Pkcs1version value.
* @param[in]  funcrandom : Funcrandom value.
* @param[in]  paramrandom : Paramrandom pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERRSA::Uncipher_PKCS1_V15(XBYTE* buffer, XDWORD size, XBUFFER& output, CIPHERKEYTYPE keytypetouse, CIPHERRSAPKCS1VERSION pkcs1version,  XMPINTEGER_FUNCRANDOM funcrandom, void* paramrandom)
{
  XBUFFER   input;
  XBUFFER   _output;
  XBYTE     pad_chk;
  int       pad_count = 0;
  XBYTE     pad_done = 0;
  bool      status;

  if(!funcrandom) return false;

  int keysize = GetKeySizeInBytes(keytypetouse);
  if(!keysize) return false;

  if((keysize < 16) || (keysize > XMPINTEGER_MAXSIZE)) return false;

  input.Add(buffer, size);

  status = false;

  switch(keytypetouse)
    {
      case CIPHERKEYTYPE_RSA_PUBLIC   : status = DoRSAPublicOperation(input , _output);                            break;
      case CIPHERKEYTYPE_RSA_PRIVATE  : status = DoRSAPrivateOperation(input, _output, funcrandom, paramrandom);   break;
                            default   : break;
    }

  if(!status) return false;

  int index = 0;

  pad_chk  = 0;
  pad_chk |= _output.GetByte(index++);

  pad_chk |= (_output.GetByte(index++)) ^  ((keytypetouse == CIPHERKEYTYPE_RSA_PRIVATE)? CIPHERRSA_CIPHERDATA : CIPHERRSA_SINGDATA);

  for(XDWORD i=0; i<size-3; i++)
    {
      pad_done  |= (_output.GetByte(index+i) == 0);
      pad_count += (pad_done == 0);
    }

  index += pad_count;
  pad_chk |= _output.GetByte(index++); // Must be zero


  if(pad_chk) return false;

  output.Add(&_output.Get()[index], (size-index));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool CIPHERRSA::Verify_PKCS1_V15(XBYTE* input, XDWORD size, XBUFFER& signature, HASH* hash)
* @brief      Verify an EMSA-PKCS1-v1_5 signature
* @ingroup    CIPHER
*
* @param[in]  input : Signed data.
* @param[in]  size : Signed data size.
* @param[in]  signature : RSA signature.
* @param[in]  hash : Hash algorithm used by the signature.
*
* @return     bool : true if the signature is valid; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERRSA::Verify_PKCS1_V15(XBYTE* input, XDWORD size, XBUFFER& signature, HASH* hash)
{
  XBYTE     digestinfoSHA224[] = { 0x30, 0x2D, 0x30, 0x0D, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01,
                                   0x65, 0x03, 0x04, 0x02, 0x04, 0x05, 0x00, 0x04, 0x1C };
  XBYTE     digestinfoSHA256[] = { 0x30, 0x31, 0x30, 0x0D, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01,
                                   0x65, 0x03, 0x04, 0x02, 0x01, 0x05, 0x00, 0x04, 0x20 };
  XBYTE     digestinfoSHA384[] = { 0x30, 0x41, 0x30, 0x0D, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01,
                                   0x65, 0x03, 0x04, 0x02, 0x02, 0x05, 0x00, 0x04, 0x30 };
  XBYTE     digestinfoSHA512[] = { 0x30, 0x51, 0x30, 0x0D, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01,
                                   0x65, 0x03, 0x04, 0x02, 0x03, 0x05, 0x00, 0x04, 0x40 };
  XBYTE*    digestinfoheader   = NULL;
  XDWORD    digestinfoheadersize = 0;
  XBUFFER   decoded;
  XBUFFER   digestinfo;
  XBUFFER   expected;
  int       keysize;
  int       paddingsize;

  if(!input || !size || !hash)
    {
      return false;
    }

  keysize = GetKeySizeInBytes(CIPHERKEYTYPE_RSA_PUBLIC);
  if(!keysize || (signature.GetSize() != (XDWORD)keysize))
    {
      return false;
    }

  switch(hash->GetType())
    {
      case HASHTYPE_SHA224 : digestinfoheader     = digestinfoSHA224;
                             digestinfoheadersize = sizeof(digestinfoSHA224);
                             break;

      case HASHTYPE_SHA256 : digestinfoheader     = digestinfoSHA256;
                             digestinfoheadersize = sizeof(digestinfoSHA256);
                             break;

      case HASHTYPE_SHA384 : digestinfoheader     = digestinfoSHA384;
                             digestinfoheadersize = sizeof(digestinfoSHA384);
                             break;

      case HASHTYPE_SHA512 : digestinfoheader     = digestinfoSHA512;
                             digestinfoheadersize = sizeof(digestinfoSHA512);
                             break;

                  default : return false;
    }

  if(!hash->ResetResult() || !hash->Do(input, size))
    {
      return false;
    }

  if(!digestinfo.Add(digestinfoheader, digestinfoheadersize) ||
     !digestinfo.Add((*hash->GetResult())))
    {
      return false;
    }

  paddingsize = keysize - (int)digestinfo.GetSize() - 3;
  if(paddingsize < 8)
    {
      return false;
    }

  if(!expected.Add((XBYTE)0x00) || !expected.Add((XBYTE)0x01))
    {
      return false;
    }

  for(int c=0; c<paddingsize; c++)
    {
      if(!expected.Add((XBYTE)0xFF))
        {
          return false;
        }
    }

  if(!expected.Add((XBYTE)0x00) || !expected.Add(digestinfo))
    {
      return false;
    }

  if(!DoRSAPublicOperation(signature, decoded) || (decoded.GetSize() != expected.GetSize()))
    {
      return false;
    }

  return CompareConstantTime(decoded, expected);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool CIPHERRSA::Verify_PKCS1_V21(XBYTE* input, XDWORD size, XBUFFER& signature, HASH* hash, XDWORD saltsize)
* @brief      Verify an EMSA-PSS signature
* @ingroup    CIPHER
*
* @param[in]  input : Signed data.
* @param[in]  size : Signed data size.
* @param[in]  signature : RSA signature.
* @param[in]  hash : Hash algorithm used by the signature.
* @param[in]  saltsize : Exact salt size. Zero selects the hash size.
*
* @return     bool : true if the signature is valid; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERRSA::Verify_PKCS1_V21(XBYTE* input, XDWORD size, XBUFFER& signature, HASH* hash, XDWORD saltsize)
{
  XBUFFER decoded;
  XBUFFER encodedmessage;
  XBUFFER maskedDB;
  XBUFFER DBmask;
  XBUFFER DB;
  XBUFFER messagehash;
  XBUFFER M;
  XBUFFER H2;
  XDWORD  hashsize;
  XDWORD  modulusbits;
  XDWORD  encodedbits;
  XDWORD  encodedsize;
  XDWORD  datablocksize;
  XDWORD  unusedbits;
  XDWORD  saltindex;
  int     keysize;

  if(!input || !size || !hash)
    {
      return false;
    }

  keysize = GetKeySizeInBytes(CIPHERKEYTYPE_RSA_PUBLIC);
  hashsize = (XDWORD)hash->GetDefaultSize();

  if(!keysize || !hashsize || (signature.GetSize() != (XDWORD)keysize))
    {
      return false;
    }

  if(!saltsize)
    {
      saltsize = hashsize;
    }

  modulusbits = context.N.GetMSB();
  if(modulusbits < 2)
    {
      return false;
    }

  encodedbits = modulusbits - 1;
  encodedsize = (encodedbits + 7) / 8;

  if((encodedsize < (hashsize + saltsize + 2)) || (encodedsize > (XDWORD)keysize))
    {
      return false;
    }

  if(!DoRSAPublicOperation(signature, decoded) || (decoded.GetSize() != (XDWORD)keysize))
    {
      return false;
    }

  for(XDWORD c=0; c<(decoded.GetSize() - encodedsize); c++)
    {
      if(decoded.GetByte(c))
        {
          return false;
        }
    }

  if(!encodedmessage.Add(&decoded.Get()[decoded.GetSize() - encodedsize], encodedsize) ||
     (encodedmessage.GetByte(encodedsize - 1) != 0xBC))
    {
      return false;
    }

  datablocksize = encodedsize - hashsize - 1;
  unusedbits    = (8 * encodedsize) - encodedbits;

  if(unusedbits && (encodedmessage.GetByte(0) & (XBYTE)(0xFF << (8 - unusedbits))))
    {
      return false;
    }

  if(!maskedDB.Add(encodedmessage.Get(), datablocksize))
    {
      return false;
    }

  XBUFFER H;
  if(!H.Add(&encodedmessage.Get()[datablocksize], hashsize) ||
     !MaskGenerationFunction1(H, datablocksize, hash, DBmask))
    {
      return false;
    }

  if(!DB.Resize(datablocksize))
    {
      return false;
    }

  for(XDWORD c=0; c<datablocksize; c++)
    {
      DB.Set((XBYTE)(maskedDB.GetByte(c) ^ DBmask.GetByte(c)), c);
    }

  if(unusedbits)
    {
      DB.Set((XBYTE)(DB.GetByte(0) & (0xFF >> unusedbits)), 0);
    }

  saltindex = datablocksize - saltsize;
  if(!saltindex || (DB.GetByte(saltindex - 1) != 0x01))
    {
      return false;
    }

  for(XDWORD c=0; c<(saltindex - 1); c++)
    {
      if(DB.GetByte(c))
        {
          return false;
        }
    }

  if(!hash->ResetResult() || !hash->Do(input, size) || !messagehash.Add((*hash->GetResult())))
    {
      return false;
    }

  for(int c=0; c<8; c++)
    {
      if(!M.Add((XBYTE)0x00))
        {
          return false;
        }
    }

  if(!M.Add(messagehash) || !M.Add(&DB.Get()[saltindex], saltsize) ||
     !hash->ResetResult() || !hash->Do(M) || !H2.Add((*hash->GetResult())) || (H2.GetSize() != H.GetSize()))
    {
      return false;
    }

  return CompareConstantTime(H, H2);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool CIPHERRSA::MaskGenerationFunction1(XBUFFER& seed, XDWORD size, HASH* hash, XBUFFER& mask)
* @brief      Generate an MGF1 mask as specified by PKCS#1
* @ingroup    CIPHER
*
* @param[in]  seed : MGF1 seed.
* @param[in]  size : Requested mask size.
* @param[in]  hash : Hash used by MGF1.
* @param[out] mask : Generated mask.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERRSA::MaskGenerationFunction1(XBUFFER& seed, XDWORD size, HASH* hash, XBUFFER& mask)
{
  XBUFFER data;
  XDWORD  counter = 0;

  if(seed.IsEmpty() || !size || !hash || !hash->GetDefaultSize())
    {
      return false;
    }

  mask.Delete();

  while(mask.GetSize() < size)
    {
      data.Delete();

      if(!data.Add(seed) ||
         !data.Add((XBYTE)((counter >> 24) & 0xFF)) ||
         !data.Add((XBYTE)((counter >> 16) & 0xFF)) ||
         !data.Add((XBYTE)((counter >>  8) & 0xFF)) ||
         !data.Add((XBYTE)( counter        & 0xFF)) ||
         !hash->ResetResult() || !hash->Do(data) || !mask.Add((*hash->GetResult())))
        {
          return false;
        }

      counter++;
    }

  if(mask.GetSize() > size)
    {
      mask.Resize(size);
    }

  return (mask.GetSize() == size);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERRSA::DoRSAPublicOperation(XBUFFER& input, XBUFFER& output)
* @brief      Do RSA public operation
* @ingroup    CIPHER
* 
* @param[in]  input : Input value.
* @param[in]  output : Output output.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERRSA::DoRSAPublicOperation(XBUFFER& input, XBUFFER& output)
{
  int keysize = GetKeySizeInBytes(CIPHERKEYTYPE_RSA_PUBLIC);
  if(!keysize) return false;

  XMPINTEGER  T;

  T.Ini();

  if(!T.SetFromXBuffer(input)) return false;

  if(T.CompareSignedValues(context.N)>=0) return false;

  if(!T.SlidingWindowExponentiation(&T, &context.E, &context.N, &context.RN)) return false;

  if(!T.GetToXBuffer(output, keysize)) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERRSA::DoRSAPrivateOperation(XBUFFER& input, XBUFFER& output, XMPINTEGER_FUNCRANDOM funcrandom, void* paramrandom)
* @brief      Do RSA private operation
* @ingroup    CIPHER
* 
* @param[in]  input : Input value.
* @param[in]  output : Output output.
* @param[in]  funcrandom : Funcrandom value.
* @param[in]  paramrandom : Paramrandom pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERRSA::DoRSAPrivateOperation(XBUFFER& input, XBUFFER& output, XMPINTEGER_FUNCRANDOM funcrandom, void* paramrandom)
{
  XMPINTEGER  T;
  XMPINTEGER  T1;
  XMPINTEGER  T2;

  #if !defined(CIPHERRSA_RSANOCRT)
  XMPINTEGER* Vi;
  XMPINTEGER* Vf;

  Vi = &context.Vi;
  Vf = &context.Vf;
  #endif

  T.Ini();
  T1.Ini();
  T2.Ini();

  if(!T.SetFromXBuffer(input)) return false;

  if(T.CompareSignedValues(context.N)>=0) return false;

  #if defined(CIPHERRSA_RSANOCRT)
    if(!T.SlidingWindowExponentiation(&T, &context.D, &context.N, &context.RN)) return false;
  #else
    if(funcrandom != NULL)
      {
        // Blinding   T = T * Vi mod N
        if(!PrepareBlinding(Vi, Vf, funcrandom, paramrandom)) return false;
        if(!T.Multiplication(&T, Vi ))                        return false;
        if(!T.Module( &T, &T, &context.N))                    return false;
      }

    // faster decryption using the CRT
    // T1 = input ^ dP mod P
    // T2 = input ^ dQ mod Q

    if(!T1.SlidingWindowExponentiation(&T, &context.DP, &context.P, &context.RP)) return false;
    if(!T2.SlidingWindowExponentiation(&T, &context.DQ, &context.Q, &context.RQ)) return false;

    // T = (T1 - T2) * (Q^-1 mod P) mod P
    if(!T.SubtractionSigned(&T1, &T2))      return false;
    if(!T1.Multiplication(&T, &context.QP)) return false;
    if(!T.Module(&T, &T1, &context.P))      return false;

    // T = T2 + T * Q
    if(!T1.Multiplication(&T, &context.Q )) return false;
    if(!T.AdditionSigned(&T2, &T1))         return false;

    if(funcrandom!= NULL)
      {
        // Unblind T = T * Vf mod N
        if(!T.Multiplication(&T, Vf))   return false;
        if(!T.Module(&T, &T, &context.N)) return false;
      }
  #endif

  int keysize = GetKeySizeInBytes(CIPHERKEYTYPE_RSA_PRIVATE);
  if(!keysize) return false;

  if(!T.GetToXBuffer(output, keysize)) return false;

  T.End();
  T1.End();
  T2.End();

  return true;
}


#if !defined(CIPHERRSA_RSANOCRT)
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERRSA::PrepareBlinding(XMPINTEGER* Vi, XMPINTEGER* Vf, XMPINTEGER_FUNCRANDOM funcrandom, void* paramrandom)
* @brief      Prepare blinding
* @ingroup    CIPHER
* 
* @param[in]  Vi : Vi pointer to use.
* @param[in]  Vf : Vf pointer to use.
* @param[in]  funcrandom : Funcrandom value.
* @param[in]  paramrandom : Paramrandom pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERRSA::PrepareBlinding(XMPINTEGER* Vi, XMPINTEGER* Vf, XMPINTEGER_FUNCRANDOM funcrandom, void* paramrandom)
{
  int count = 0;

  if(context.Vf.GetLimbs()!= NULL)
    {
      // We already have blinding values, just update them by squaring
      if(!context.Vi.Multiplication(&context.Vi, &context.Vi))      return false;
      if(!context.Vi.Module(&context.Vi, &context.Vi, &context.N )) return false;
      if(!context.Vf.Multiplication(&context.Vf, &context.Vf))      return false;
      if(!context.Vf.Module(&context.Vf, &context.Vf, &context.N )) return false;
    }
   else
    {
      int keysize = GetKeySizeInBytes(CIPHERKEYTYPE_RSA_PRIVATE);
      if(!keysize) return false;

      // Unblinding value: Vf = random number, invertible mod N
      do{ if(count++ > 10) return false;

          if(!context.Vf.FillRandom(keysize-1, funcrandom, paramrandom))             return false;
          if(!context.Vi.GreatestCommonDivisor(&context.Vi, &context.Vf, &context.N )) return false;

        } while(context.Vi.CompareSignedValues(1)!=0);

      // Blinding value: Vi =  Vf^(-e) mod N
      if(!context.Vi.ModularInverse( &context.Vf, &context.N))                                       return false;
      if(!context.Vi.SlidingWindowExponentiation(&context.Vi, &context.E, &context.N, &context.RN )) return false;
    }

  if(Vi != &context.Vi)
    {
      if(!Vi->CopyFrom(&context.Vi)) return false;
      if(!Vf->CopyFrom(&context.Vf)) return false;
    }

  return true;
}
#endif


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int CIPHERRSA::GetKeySizeInBytes(CIPHERKEYTYPE keytouse)
* @brief      Get key size in bytes
* @ingroup    CIPHER
* 
* @param[in]  keytouse : Keytouse value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int CIPHERRSA::GetKeySizeInBytes(CIPHERKEYTYPE keytouse)
{
  int keysize = 0;

  CIPHERKEY* key = GetKey(keytouse);
  if(!key) return 0;

  keysize = key->GetSizeInBytes();

  return keysize;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERRSA::GenerateRandom(XBYTE* buffer, XDWORD size, void* param)
* @brief      Generate random
* @ingroup    CIPHER
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* @param[in]  param : Param pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERRSA::GenerateRandom(XBYTE* buffer, XDWORD size, void* param)
{
  XRAND* xrand = (XRAND*)param;

  if(!xrand || !xrand->IsCryptographicallySecure() || (!buffer && size)) return false;
  if(!size) return true;

  return xrand->Generate(buffer, size);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void CIPHERRSA::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
void CIPHERRSA::Clean()
{
  xrand = NULL;
}
