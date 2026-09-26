/**-------------------------------------------------------------------------------------------------------------------
*
* @file       CipherAESGCM.cpp
*
* @class      CIPHERAESGCM
* @brief      Cipher AES GCM (Galois/Counter Mode, NIST SP 800-38D) class
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

#include "CipherAESGCM.h"

#include "XFactory.h"

#include "CipherKeySymmetrical.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         define GET_UINT32_BE(n, b, i)
* @brief      Get a 32 bit value from a buffer, in network order
* @ingroup    CIPHER
*
* --------------------------------------------------------------------------------------------------------------------*/
#define GET_UINT32_BE(n,b,i)  {  (n) =    ( (XDWORD) (b)[(i)    ] << 24 )              \
                                        | ( (XDWORD) (b)[(i) + 1] << 16 )              \
                                        | ( (XDWORD) (b)[(i) + 2] <<  8 )              \
                                        | ( (XDWORD) (b)[(i) + 3]       );  }


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         define PUT_UINT32_BE(n, b, i)
* @brief      Put a 32 bit value into a buffer, in network order
* @ingroup    CIPHER
*
* --------------------------------------------------------------------------------------------------------------------*/
#define PUT_UINT32_BE(n,b,i)  {  (b)[(i)    ] = (XBYTE) ( (n) >> 24 );                 \
                                 (b)[(i) + 1] = (XBYTE) ( (n) >> 16 );                 \
                                 (b)[(i) + 2] = (XBYTE) ( (n) >>  8 );                 \
                                 (b)[(i) + 3] = (XBYTE) ( (n)       );  }


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         define GET_UINT64_BE(n, b, i)
* @brief      Get a 64 bit value from a buffer, in network order
* @ingroup    CIPHER
*
* --------------------------------------------------------------------------------------------------------------------*/
#define GET_UINT64_BE(n,b,i)  {  (n) =    ( (XQWORD) (b)[(i)    ] << 56 )              \
                                        | ( (XQWORD) (b)[(i) + 1] << 48 )              \
                                        | ( (XQWORD) (b)[(i) + 2] << 40 )              \
                                        | ( (XQWORD) (b)[(i) + 3] << 32 )              \
                                        | ( (XQWORD) (b)[(i) + 4] << 24 )              \
                                        | ( (XQWORD) (b)[(i) + 5] << 16 )              \
                                        | ( (XQWORD) (b)[(i) + 6] <<  8 )              \
                                        | ( (XQWORD) (b)[(i) + 7]       );  }


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         define PUT_UINT64_BE(n, b, i)
* @brief      Put a 64 bit value into a buffer, in network order
* @ingroup    CIPHER
*
* --------------------------------------------------------------------------------------------------------------------*/
#define PUT_UINT64_BE(n,b,i)  {  (b)[(i)    ] = (XBYTE) ( (n) >> 56 );                 \
                                 (b)[(i) + 1] = (XBYTE) ( (n) >> 48 );                 \
                                 (b)[(i) + 2] = (XBYTE) ( (n) >> 40 );                 \
                                 (b)[(i) + 3] = (XBYTE) ( (n) >> 32 );                 \
                                 (b)[(i) + 4] = (XBYTE) ( (n) >> 24 );                 \
                                 (b)[(i) + 5] = (XBYTE) ( (n) >> 16 );                 \
                                 (b)[(i) + 6] = (XBYTE) ( (n) >>  8 );                 \
                                 (b)[(i) + 7] = (XBYTE) ( (n)       );  }


// Reduction values for the four lowest bits of the polynomial, used by the table driven GHASH.

static const XQWORD CIPHERAESGCM_LAST4[CIPHERAESGCM_TABLESIZE] = { 0x0000, 0x1c20, 0x3840, 0x2460,
                                                                  0x7080, 0x6ca0, 0x48c0, 0x54e0,
                                                                  0xe100, 0xfd20, 0xd940, 0xc560,
                                                                  0x9180, 0x8da0, 0xa9c0, 0xb5e0 };



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         CIPHERAESGCM::CIPHERAESGCM()
* @brief      Constructor of class
* @ingroup    CIPHER
*
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERAESGCM::CIPHERAESGCM() : CIPHERAES()
{
  Clean();

  type          = CIPHERTYPE_AES;
  chainingmode  = CIPHERCHAININGMODE_GCM;

  tagsize       = CIPHERAESGCM_TAGSIZE;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         CIPHERAESGCM::~CIPHERAESGCM()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    CIPHER
*
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERAESGCM::~CIPHERAESGCM()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool CIPHERAESGCM::SetKey(CIPHERKEY* key, bool integritycheck)
* @brief      Set key: it also builds the AES key schedule and the GHASH tables, so it is done only once per key
* @ingroup    CIPHER
*
* @param[in]  key : Key to use. It must be a symmetrical key of 16, 24 or 32 bytes.
* @param[in]  integritycheck : Integritycheck value.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERAESGCM::SetKey(CIPHERKEY* key, bool integritycheck)
{
  keyisready = false;

  if(!CIPHER::SetKey(key, integritycheck))
    {
      return false;
    }

  CIPHERKEYSYMMETRICAL* symmetricalkey = (CIPHERKEYSYMMETRICAL*)GetKey();

  if(!symmetricalkey)
    {
      return false;
    }

  if(!symmetricalkey->Get())
    {
      return false;
    }

  if(!symmetricalkey->Get()->Get())
    {
      return false;
    }

  if(!AESSetKeyCipher(&ctx, symmetricalkey->Get()->Get(), symmetricalkey->Get()->GetSize() * 8))
    {
      return false;
    }

  if(!GCM_GenerateTable())
    {
      return false;
    }

  keyisready = true;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool CIPHERAESGCM::IsAEAD()
* @brief      Is this cipher an AEAD one
* @ingroup    CIPHER
*
* @return     bool : true if the condition is met; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERAESGCM::IsAEAD()
{
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD CIPHERAESGCM::GetAEADNonceSize()
* @brief      Get the recommended nonce size, in bytes
* @note       Other sizes are accepted by the AEAD operations, and are reduced with GHASH as the standard describes.
* @ingroup    CIPHER
*
* @return     XDWORD : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD CIPHERAESGCM::GetAEADNonceSize()
{
  return CIPHERAESGCM_NONCESIZE;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD CIPHERAESGCM::GetAEADTagSize()
* @brief      Get the size, in bytes, of the authentication tag
* @ingroup    CIPHER
*
* @return     XDWORD : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD CIPHERAESGCM::GetAEADTagSize()
{
  return tagsize;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool CIPHERAESGCM::SetAEADTagSize(XDWORD tagsize)
* @brief      Set the size, in bytes, of the authentication tag
* @note       TLS 1.3 always uses the full size. Shorter tags weaken the authentication and should be avoided.
* @ingroup    CIPHER
*
* @param[in]  tagsize : Size of the tag, from CIPHERAESGCM_TAGSIZE_MIN to CIPHERAESGCM_TAGSIZE.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERAESGCM::SetAEADTagSize(XDWORD tagsize)
{
  if((tagsize < CIPHERAESGCM_TAGSIZE_MIN) || (tagsize > CIPHERAESGCM_TAGSIZE))
    {
      return false;
    }

  this->tagsize = tagsize;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool CIPHERAESGCM::CipherAEAD(XBYTE* input, XDWORD size, XBUFFER& nonce, XBUFFER& additionaldata, XBUFFER& tag)
* @brief      Cipher with authentication: the ciphertext is left in the result buffer and the tag in tag
* @ingroup    CIPHER
*
* @param[in]  input : Input pointer to use.
* @param[in]  size : Size value. It may be zero, to authenticate the additional data only.
* @param[in]  nonce : Nonce, unique for every operation with the same key.
* @param[in]  additionaldata : Data authenticated but not ciphered. It may be empty.
* @param[out] tag : Buffer that receives the authentication tag.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERAESGCM::CipherAEAD(XBYTE* input, XDWORD size, XBUFFER& nonce, XBUFFER& additionaldata, XBUFFER& tag)
{
  return GCM_Do(input, size, nonce, additionaldata, tag, true);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool CIPHERAESGCM::UncipherAEAD(XBYTE* input, XDWORD size, XBUFFER& nonce, XBUFFER& additionaldata, XBUFFER& tag)
* @brief      Uncipher and verify the authentication tag: the plain text is left in the result buffer
* @note       When the tag does not match, the result buffer is emptied and false is returned. The plain text of a
*             message that fails authentication must never be used.
* @ingroup    CIPHER
*
* @param[in]  input : Input pointer to use.
* @param[in]  size : Size value.
* @param[in]  nonce : Nonce used by the cipher operation.
* @param[in]  additionaldata : Data authenticated but not ciphered. It may be empty.
* @param[in]  tag : Authentication tag to verify.
*
* @return     bool : true if the operation is successful and the tag is valid; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERAESGCM::UncipherAEAD(XBYTE* input, XDWORD size, XBUFFER& nonce, XBUFFER& additionaldata, XBUFFER& tag)
{
  return GCM_Do(input, size, nonce, additionaldata, tag, false);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool CIPHERAESGCM::GCM_GenerateTable()
* @brief      Calculate H = E(K, 0) and the 16 precalculated multiples used by the table driven GHASH
* @note       INTERNAL. The table takes 256 bytes, the four bit variant, chosen so it also fits a microcontroller.
* @ingroup    CIPHER
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERAESGCM::GCM_GenerateTable()
{
  XBYTE   h[CIPHERAESGCM_BLOCKSIZE];
  XQWORD  vh;
  XQWORD  vl;
  int     i;
  int     j;

  memset(h, 0, CIPHERAESGCM_BLOCKSIZE);

  if(!AESCipher_ECB_Block(&ctx, CIPHERAES_ENCRYPT, h, h))
    {
      return false;
    }

  GET_UINT64_BE(vh, h, 0);
  GET_UINT64_BE(vl, h, 8);

  HL[8] = vl;
  HH[8] = vh;
  HL[0] = 0;
  HH[0] = 0;

  for(i=4; i>0; i>>=1)
    {
      XDWORD T = (XDWORD)(vl & 1) * 0xe1000000;

      vl    = (vh << 63) | (vl >> 1);
      vh    = (vh >>  1) ^ ((XQWORD)T << 32);

      HL[i] = vl;
      HH[i] = vh;
    }

  for(i=2; i<=8; i*=2)
    {
      XQWORD* HiL = HL + i;
      XQWORD* HiH = HH + i;

      vh = *HiH;
      vl = *HiL;

      for(j=1; j<i; j++)
        {
          HiH[j] = vh ^ HH[j];
          HiL[j] = vl ^ HL[j];
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void CIPHERAESGCM::GCM_Multiplication(XBYTE* x, XBYTE* output)
* @brief      Multiply x by H in GF(2^128), the GHASH operation. x and output may be the same buffer
* @note       INTERNAL
* @ingroup    CIPHER
*
* @param[in]  x : Block of 16 bytes to multiply.
* @param[out] output : Block of 16 bytes that receives the product.
*
* --------------------------------------------------------------------------------------------------------------------*/
void CIPHERAESGCM::GCM_Multiplication(XBYTE* x, XBYTE* output)
{
  XBYTE   lo;
  XBYTE   hi;
  XBYTE   rem;
  XQWORD  zh;
  XQWORD  zl;
  int     i;

  lo  = (XBYTE)(x[15] & 0x0f);
  hi  = (XBYTE)(x[15] >> 4);

  zh  = HH[lo];
  zl  = HL[lo];

  rem = (XBYTE)(zl & 0x0f);
  zl  = (zh << 60) | (zl >> 4);
  zh  = (zh >> 4);
  zh ^= (XQWORD)CIPHERAESGCM_LAST4[rem] << 48;
  zh ^= HH[hi];
  zl ^= HL[hi];

  for(i=14; i>=0; i--)
    {
      lo  = (XBYTE)(x[i] & 0x0f);
      hi  = (XBYTE)(x[i] >> 4);

      rem = (XBYTE)(zl & 0x0f);
      zl  = (zh << 60) | (zl >> 4);
      zh  = (zh >> 4);
      zh ^= (XQWORD)CIPHERAESGCM_LAST4[rem] << 48;
      zh ^= HH[lo];
      zl ^= HL[lo];

      rem = (XBYTE)(zl & 0x0f);
      zl  = (zh << 60) | (zl >> 4);
      zh  = (zh >> 4);
      zh ^= (XQWORD)CIPHERAESGCM_LAST4[rem] << 48;
      zh ^= HH[hi];
      zl ^= HL[hi];
    }

  PUT_UINT64_BE(zh, output, 0);
  PUT_UINT64_BE(zl, output, 8);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool CIPHERAESGCM::GCM_Start(XBUFFER& nonce, XBUFFER& additionaldata)
* @brief      Prepare the counter block J0, calculate E(K, J0) and feed the additional data into GHASH
* @note       INTERNAL
* @ingroup    CIPHER
*
* @param[in]  nonce : Nonce to use.
* @param[in]  additionaldata : Data authenticated but not ciphered. It may be empty.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERAESGCM::GCM_Start(XBUFFER& nonce, XBUFFER& additionaldata)
{
  XBYTE   workbuffer[CIPHERAESGCM_BLOCKSIZE];
  XBYTE*  data;
  XDWORD  size;
  XDWORD  use;
  XDWORD  c;

  memset(counter , 0, CIPHERAESGCM_BLOCKSIZE);
  memset(ghash   , 0, CIPHERAESGCM_BLOCKSIZE);

  sizedata            = 0;
  sizeadditionaldata  = 0;

  if(!nonce.GetSize())
    {
      return false;
    }

  if(nonce.GetSize() == CIPHERAESGCM_NONCESIZE)
    {
      memcpy(counter, nonce.Get(), CIPHERAESGCM_NONCESIZE);

      counter[15] = 1;
    }
   else
    {
      XQWORD sizenoncebits = ((XQWORD)nonce.GetSize()) << 3;

      memset(workbuffer, 0, CIPHERAESGCM_BLOCKSIZE);

      PUT_UINT32_BE((XDWORD)(sizenoncebits >> 32), workbuffer, 8);
      PUT_UINT32_BE((XDWORD)(sizenoncebits      ), workbuffer, 12);

      data = nonce.Get();
      size = nonce.GetSize();

      while(size)
        {
          use = (size < CIPHERAESGCM_BLOCKSIZE)?size:CIPHERAESGCM_BLOCKSIZE;

          for(c=0; c<use; c++)
            {
              counter[c] ^= data[c];
            }

          GCM_Multiplication(counter, counter);

          size -= use;
          data += use;
        }

      for(c=0; c<CIPHERAESGCM_BLOCKSIZE; c++)
        {
          counter[c] ^= workbuffer[c];
        }

      GCM_Multiplication(counter, counter);
    }

  if(!AESCipher_ECB_Block(&ctx, CIPHERAES_ENCRYPT, counter, basectr))
    {
      return false;
    }

  sizeadditionaldata = additionaldata.GetSize();

  data = additionaldata.Get();
  size = additionaldata.GetSize();

  while(size)
    {
      use = (size < CIPHERAESGCM_BLOCKSIZE)?size:CIPHERAESGCM_BLOCKSIZE;

      for(c=0; c<use; c++)
        {
          ghash[c] ^= data[c];
        }

      GCM_Multiplication(ghash, ghash);

      size -= use;
      data += use;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool CIPHERAESGCM::GCM_Update(XBYTE* input, XDWORD size, XBYTE* output, bool iscipher)
* @brief      Run the counter mode over the data while feeding the ciphertext into GHASH
* @note       INTERNAL
* @ingroup    CIPHER
*
* @param[in]  input : Input pointer to use.
* @param[in]  size : Size value.
* @param[out] output : Output pointer, of at least size bytes.
* @param[in]  iscipher : true to cipher, false to uncipher.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERAESGCM::GCM_Update(XBYTE* input, XDWORD size, XBYTE* output, bool iscipher)
{
  XBYTE   streamblock[CIPHERAESGCM_BLOCKSIZE];
  XDWORD  offset = 0;
  XDWORD  use;
  XDWORD  c;

  if(!input || !output)
    {
      return false;
    }

  sizedata += size;

  while(size)
    {
      use = (size < CIPHERAESGCM_BLOCKSIZE)?size:CIPHERAESGCM_BLOCKSIZE;

      for(c=CIPHERAESGCM_BLOCKSIZE; c>12; c--)                                  // Increment the 32 bit counter of the block
        {
          if(++counter[c-1] != 0)
            {
              break;
            }
        }

      if(!AESCipher_ECB_Block(&ctx, CIPHERAES_ENCRYPT, counter, streamblock))
        {
          return false;
        }

      for(c=0; c<use; c++)
        {
          if(!iscipher)
            {
              ghash[c] ^= input[offset+c];
            }

          output[offset+c] = (XBYTE)(streamblock[c] ^ input[offset+c]);

          if(iscipher)
            {
              ghash[c] ^= output[offset+c];
            }
        }

      GCM_Multiplication(ghash, ghash);

      size   -= use;
      offset += use;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool CIPHERAESGCM::GCM_Finish(XBYTE* tag)
* @brief      Close GHASH with the lengths block and produce the authentication tag
* @note       INTERNAL
* @ingroup    CIPHER
*
* @param[out] tag : Buffer of at least CIPHERAESGCM_BLOCKSIZE bytes that receives the tag.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERAESGCM::GCM_Finish(XBYTE* tag)
{
  XBYTE   workbuffer[CIPHERAESGCM_BLOCKSIZE];
  XQWORD  sizedatabits;
  XQWORD  sizeadditionaldatabits;
  XDWORD  c;

  if(!tag)
    {
      return false;
    }

  memcpy(tag, basectr, tagsize);

  sizedatabits            = sizedata           << 3;
  sizeadditionaldatabits  = sizeadditionaldata << 3;

  if(sizedatabits || sizeadditionaldatabits)
    {
      memset(workbuffer, 0, CIPHERAESGCM_BLOCKSIZE);

      PUT_UINT32_BE((XDWORD)(sizeadditionaldatabits >> 32), workbuffer, 0);
      PUT_UINT32_BE((XDWORD)(sizeadditionaldatabits ), workbuffer, 4);
      PUT_UINT32_BE((XDWORD)(sizedatabits >> 32), workbuffer, 8);
      PUT_UINT32_BE((XDWORD)(sizedatabits                ), workbuffer, 12);

      for(c=0; c<CIPHERAESGCM_BLOCKSIZE; c++)
        {
          ghash[c] ^= workbuffer[c];
        }

      GCM_Multiplication(ghash, ghash);

      for(c=0; c<tagsize; c++)
        {
          tag[c] ^= ghash[c];
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool CIPHERAESGCM::GCM_Do(XBYTE* input, XDWORD size, XBUFFER& nonce, XBUFFER& additionaldata, XBUFFER& tag, bool iscipher)
* @brief      Run a complete AEAD operation, in one direction or the other
* @note       INTERNAL
* @ingroup    CIPHER
*
* @param[in]  input : Input pointer to use.
* @param[in]  size : Size value.
* @param[in]  nonce : Nonce to use.
* @param[in]  additionaldata : Data authenticated but not ciphered. It may be empty.
* @param[in]  tag : Tag produced when ciphering, tag to verify when unciphering.
* @param[in]  iscipher : true to cipher, false to uncipher.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERAESGCM::GCM_Do(XBYTE* input, XDWORD size, XBUFFER& nonce, XBUFFER& additionaldata, XBUFFER& tag, bool iscipher)
{
  XBYTE calculatedtag[CIPHERAESGCM_BLOCKSIZE];

  if(!keyisready || !result)
    {
      return false;
    }

  if(!input && size)
    {
      return false;
    }

  if(!iscipher)
    {
      if(tag.GetSize() != tagsize)                                              // Rejected before any work is done
        {
          return false;
        }
    }

  result->Delete();

  if(size)
    {
      if(!result->Resize(size))
        {
          return false;
        }
    }

  if(!GCM_Start(nonce, additionaldata))
    {
      result->Delete();
      return false;
    }

  if(size)
    {
      if(!GCM_Update(input, size, result->Get(), iscipher))
        {
          result->Delete();
          return false;
        }
    }

  memset(calculatedtag, 0, CIPHERAESGCM_BLOCKSIZE);

  if(!GCM_Finish(calculatedtag))
    {
      result->Delete();
      return false;
    }

  if(iscipher)
    {
      tag.Delete();

      if(!tag.Add(calculatedtag, tagsize))
        {
          result->Delete();
          return false;
        }
    }
   else
    {
      if(!CompareConstantTime(calculatedtag, tag.Get(), tagsize))
        {
          result->Delete();                                                     // Never expose the plain text of a message that fails authentication
          return false;
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void CIPHERAESGCM::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    CIPHER
*
* --------------------------------------------------------------------------------------------------------------------*/
void CIPHERAESGCM::Clean()
{
  memset(&ctx, 0, sizeof(CIPHERAES_CONTEXT));

  keyisready          = false;
  tagsize             = CIPHERAESGCM_TAGSIZE;

  memset(HL, 0, sizeof(HL));
  memset(HH, 0, sizeof(HH));

  memset(basectr , 0, CIPHERAESGCM_BLOCKSIZE);
  memset(counter , 0, CIPHERAESGCM_BLOCKSIZE);
  memset(ghash   , 0, CIPHERAESGCM_BLOCKSIZE);

  sizedata            = 0;
  sizeadditionaldata  = 0;
}
