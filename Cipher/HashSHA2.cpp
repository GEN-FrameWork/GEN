/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       HashSHA2.cpp
* 
* @class      HASHSHA2
* @brief      Hash SHA2 class
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

#include "HashSHA2.h"

#include <string.h>

#include "XBuffer.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

#if 0
#define UNROLL_LOOPS /* Enable loops unrolling */
#endif

#define SHFR(x, n)    (x >> n)
#define ROTR(x, n)   ((x >> n) | (x << ((sizeof(x) << 3) - n)))
#define ROTL(x, n)   ((x << n) | (x >> ((sizeof(x) << 3) - n)))
#define CH(x, y, z)  ((x & y) ^ (~x & z))
#define MAJ(x, y, z) ((x & y) ^ (x & z) ^ (y & z))

#define SHA256_F1(x) (ROTR(x,  2) ^ ROTR(x, 13) ^ ROTR(x, 22))
#define SHA256_F2(x) (ROTR(x,  6) ^ ROTR(x, 11) ^ ROTR(x, 25))
#define SHA256_F3(x) (ROTR(x,  7) ^ ROTR(x, 18) ^ SHFR(x,  3))
#define SHA256_F4(x) (ROTR(x, 17) ^ ROTR(x, 19) ^ SHFR(x, 10))

#define SHA512_F1(x) (ROTR(x, 28) ^ ROTR(x, 34) ^ ROTR(x, 39))
#define SHA512_F2(x) (ROTR(x, 14) ^ ROTR(x, 18) ^ ROTR(x, 41))
#define SHA512_F3(x) (ROTR(x,  1) ^ ROTR(x,  8) ^ SHFR(x,  7))
#define SHA512_F4(x) (ROTR(x, 19) ^ ROTR(x, 61) ^ SHFR(x,  6))

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         define UNPACK32(x, str)
* @brief      UNPACK32
* @ingroup    CIPHER
* 
* @param[in]  x : X coordinate.
* @param[in]  str : String to use.
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
#define UNPACK32(x, str)    {                                             \
                                *((str) + 3) = (XBYTE) ((x)      );       \
                                *((str) + 2) = (XBYTE) ((x) >>  8);       \
                                *((str) + 1) = (XBYTE) ((x) >> 16);       \
                                *((str) + 0) = (XBYTE) ((x) >> 24);       \
                            }

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         define PACK32(str, x)
* @brief      PACK32
* @ingroup    CIPHER
* 
* @param[in]  str : String to use.
* @param[in]  x : X coordinate.
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
#define PACK32(str, x)      {                                             \
                                *(x) =   ((XDWORD) *((str) + 3)      )    \
                                       | ((XDWORD) *((str) + 2) <<  8)    \
                                       | ((XDWORD) *((str) + 1) << 16)    \
                                       | ((XDWORD) *((str) + 0) << 24);   \
                            }

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         define UNPACK64(x, str)
* @brief      UNPACK64
* @ingroup    CIPHER
* 
* @param[in]  x : X coordinate.
* @param[in]  str : String to use.
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
#define UNPACK64(x, str)    {                                             \
                                *((str) + 7) = (XBYTE) ((x)      );       \
                                *((str) + 6) = (XBYTE) ((x) >>  8);       \
                                *((str) + 5) = (XBYTE) ((x) >> 16);       \
                                *((str) + 4) = (XBYTE) ((x) >> 24);       \
                                *((str) + 3) = (XBYTE) ((x) >> 32);       \
                                *((str) + 2) = (XBYTE) ((x) >> 40);       \
                                *((str) + 1) = (XBYTE) ((x) >> 48);       \
                                *((str) + 0) = (XBYTE) ((x) >> 56);       \
                            }

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         define PACK64(str, x)
* @brief      PACK64
* @ingroup    CIPHER
* 
* @param[in]  str : String to use.
* @param[in]  x : X coordinate.
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
#define PACK64(str, x)      {                                             \
                                *(x) =   ((XQWORD) *((str) + 7)      )    \
                                       | ((XQWORD) *((str) + 6) <<  8)    \
                                       | ((XQWORD) *((str) + 5) << 16)    \
                                       | ((XQWORD) *((str) + 4) << 24)    \
                                       | ((XQWORD) *((str) + 3) << 32)    \
                                       | ((XQWORD) *((str) + 2) << 40)    \
                                       | ((XQWORD) *((str) + 1) << 48)    \
                                       | ((XQWORD) *((str) + 0) << 56);   \
                            }

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         define SHA256_SCR(i)
* @brief      SHA256 SCR
* @ingroup    CIPHER
* 
* @param[in]  i : I value.
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
/* Macros used for loops unrolling */

#define SHA256_SCR(i)       {                                             \
                                w[i] =  SHA256_F4(w[i -  2]) + w[i -  7]  \
                                      + SHA256_F3(w[i - 15]) + w[i - 16]; \
                            }

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         define SHA512_SCR(i)
* @brief      SHA512 SCR
* @ingroup    CIPHER
* 
* @param[in]  i : I value.
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
#define SHA512_SCR(i)       {                                             \
                                w[i] =  SHA512_F4(w[i -  2]) + w[i -  7]  \
                                      + SHA512_F3(w[i - 15]) + w[i - 16]; \
                            }

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         define SHA256_EXP(a, b, c, d, e, f, g, h, j)
* @brief      SHA256 EXP
* @ingroup    CIPHER
* 
* @param[in]  a : A value.
* @param[in]  b : B value.
* @param[in]  c : C value.
* @param[in]  d : D value.
* @param[in]  e : E value.
* @param[in]  f : F value.
* @param[in]  g : G value.
* @param[in]  h : H value.
* @param[in]  j : J value.
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
#define SHA256_EXP(a, b, c, d, e, f, g, h, j) {                                                           \
                                                  t1 = wv[h] + SHA256_F2(wv[e]) + CH(wv[e], wv[f], wv[g]) \
                                                       + sha256_k[j] + w[j];                              \
                                                  t2 = SHA256_F1(wv[a]) + MAJ(wv[a], wv[b], wv[c]);       \
                                                  wv[d] += t1;                                            \
                                                  wv[h] = t1 + t2;                                        \
                                              }

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         define SHA512_EXP(a, b, c, d, e, f, g, h, j)
* @brief      SHA512 EXP
* @ingroup    CIPHER
* 
* @param[in]  a : A value.
* @param[in]  b : B value.
* @param[in]  c : C value.
* @param[in]  d : D value.
* @param[in]  e : E value.
* @param[in]  f : F value.
* @param[in]  g : G value.
* @param[in]  h : H value.
* @param[in]  j : J value.
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
#define SHA512_EXP(a, b, c, d, e, f, g ,h, j) {                                                           \
                                                  t1 = wv[h] + SHA512_F2(wv[e]) + CH(wv[e], wv[f], wv[g]) \
                                                       + sha512_k[j] + w[j];                              \
                                                  t2 = SHA512_F1(wv[a]) + MAJ(wv[a], wv[b], wv[c]);       \
                                                  wv[d] += t1;                                            \
                                                  wv[h] = t1 + t2;                                        \
                                              }


XDWORD HASHSHA2::Sha224_h0[8] =   { 0xc1059ed8, 0x367cd507, 0x3070dd17, 0xf70e5939,
                                    0xffc00b31, 0x68581511, 0x64f98fa7, 0xbefa4fa4
                                  };

XDWORD HASHSHA2::Sha256_h0[8] =   { 0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
                                    0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
                                  };

XQWORD HASHSHA2::Sha384_h0[8] =   { 0xcbbb9d5dc1059ed8ULL, 0x629a292a367cd507ULL,
                                    0x9159015a3070dd17ULL, 0x152fecd8f70e5939ULL,
                                    0x67332667ffc00b31ULL, 0x8eb44a8768581511ULL,
                                    0xdb0c2e0d64f98fa7ULL, 0x47b5481dbefa4fa4ULL
                                  };

XQWORD HASHSHA2::Sha512_h0[8] =   { 0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL,
                                    0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL,
                                    0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL,
                                    0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL
                                  };

XDWORD HASHSHA2::Sha256_k[64] =   { 0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
                                    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
                                    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
                                    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
                                    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
                                    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
                                    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
                                    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
                                    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
                                    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
                                    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
                                    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
                                    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
                                    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
                                    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
                                    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
                                  };

 XQWORD HASHSHA2::Sha512_k[80] =  { 0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL,
                                    0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL,
                                    0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL,
                                    0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL,
                                    0xd807aa98a3030242ULL, 0x12835b0145706fbeULL,
                                    0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL,
                                    0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL,
                                    0x9bdc06a725c71235ULL, 0xc19bf174cf692694ULL,
                                    0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL,
                                    0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL,
                                    0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL,
                                    0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL,
                                    0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL,
                                    0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL,
                                    0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL,
                                    0x06ca6351e003826fULL, 0x142929670a0e6e70ULL,
                                    0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL,
                                    0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL,
                                    0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL,
                                    0x81c2c92e47edaee6ULL, 0x92722c851482353bULL,
                                    0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL,
                                    0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL,
                                    0xd192e819d6ef5218ULL, 0xd69906245565a910ULL,
                                    0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL,
                                    0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL,
                                    0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL,
                                    0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL,
                                    0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL,
                                    0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL,
                                    0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL,
                                    0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL,
                                    0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL,
                                    0xca273eceea26619cULL, 0xd186b8c721c0c207ULL,
                                    0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL,
                                    0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL,
                                    0x113f9804bef90daeULL, 0x1b710b35131c471bULL,
                                    0x28db77f523047d84ULL, 0x32caab7b40c72493ULL,
                                    0x3c9ebe0a15c9bebcULL, 0x431d67c49c100d4cULL,
                                    0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL,
                                    0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL
                                  };


/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         HASHSHA2::HASHSHA2(HASHSHA2TYPE type) : HASH()
* @brief      Constructor of class
* @ingroup    CIPHER
* 
* 
* @param[in]  type : SHA-2 hash type.
* 
* --------------------------------------------------------------------------------------------------------------------*/
HASHSHA2::HASHSHA2(HASHSHA2TYPE type) : HASH()
{
  Clean();

  typesha2 = type;

  switch(typesha2)
    {
      case HASHSHA2TYPE_224    : this->type = HASHTYPE_SHA224;  break;
      case HASHSHA2TYPE_256    : this->type = HASHTYPE_SHA256;  break;
      case HASHSHA2TYPE_384    : this->type = HASHTYPE_SHA384;  break;
      case HASHSHA2TYPE_512    : this->type = HASHTYPE_SHA512;  break;
      default: break;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         HASHSHA2::~HASHSHA2()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
HASHSHA2::~HASHSHA2()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool HASHSHA2::Do(XBYTE* input, XQWORD size)
* @brief      Execute the operation
* @ingroup    CIPHER
* 
* @param[in]  input : Input pointer to use.
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool HASHSHA2::Do(XBYTE* input, XQWORD size)
{
  XBYTE digest[128];

  memset(digest , 0, 128);

  switch(typesha2)
    {
      case HASHSHA2TYPE_UNKNOWN : return false;

      case HASHSHA2TYPE_224     : Sha2_224(input, (XDWORD)size, digest);
                                  result->Add(digest, HASHSHA2_224_DIGEST_SIZE);
                                  break;

      case HASHSHA2TYPE_256     : Sha2_256(input, (XDWORD)size, digest);
                                  result->Add(digest, HASHSHA2_256_DIGEST_SIZE);
                                  break;

      case HASHSHA2TYPE_384     : Sha2_384(input, (XDWORD)size, digest);
                                  result->Add(digest, HASHSHA2_384_DIGEST_SIZE);
                                  break;

      case HASHSHA2TYPE_512     : Sha2_512(input, (XDWORD)size, digest);
                                  result->Add(digest, HASHSHA2_512_DIGEST_SIZE);
                                  break;

                    default     : return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int HASHSHA2::GetDefaultSize()
* @brief      Get default size
* @ingroup    CIPHER
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int HASHSHA2::GetDefaultSize()
{
  switch(typesha2)
    {
      case HASHSHA2TYPE_224     : return HASHSHA2_224_DIGEST_SIZE;
      case HASHSHA2TYPE_256     : return HASHSHA2_256_DIGEST_SIZE;
      case HASHSHA2TYPE_384     : return HASHSHA2_384_DIGEST_SIZE;
      case HASHSHA2TYPE_512     : return HASHSHA2_512_DIGEST_SIZE;
      case HASHSHA2TYPE_UNKNOWN :
                     default    : return 0;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void HASHSHA2::Sha2_256_Transf(HASHSHA2_256_CTX* ctx, XBYTE* input, XDWORD blocknb)
* @brief      Sha2256 transf
* @ingroup    CIPHER
* 
* @param[in]  ctx : Ctx pointer to use.
* @param[in]  input : Input pointer to use.
* @param[in]  blocknb : Blocknb value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void HASHSHA2::Sha2_256_Transf(HASHSHA2_256_CTX* ctx, XBYTE* input, XDWORD blocknb)
{
  XDWORD  w[64];
  XDWORD  wv[8];
  XDWORD  t1;
  XDWORD  t2;
  XBYTE*  subblock;
  int     i;
  int     j;

  for(i=0; i<(int)blocknb; i++)
    {
      subblock = input + (i << 6);

      for(j=0; j<16; j++)
        {
          PACK32(&subblock[j << 2], &w[j]);
        }

      for(j=16; j<64; j++)
        {
          SHA256_SCR(j);
        }

      for(j=0; j<8; j++)
        {
            wv[j] = ctx->h[j];
        }

      for(j=0; j<64; j++)
        {
          t1 = wv[7] + SHA256_F2(wv[4]) + CH(wv[4], wv[5], wv[6]) + Sha256_k[j] + w[j];
          t2 = SHA256_F1(wv[0]) + MAJ(wv[0], wv[1], wv[2]);

          wv[7] = wv[6];
          wv[6] = wv[5];
          wv[5] = wv[4];
          wv[4] = wv[3] + t1;
          wv[3] = wv[2];
          wv[2] = wv[1];
          wv[1] = wv[0];
          wv[0] = t1 + t2;
        }

      for(j=0; j<8; j++)
        {
          ctx->h[j] += wv[j];
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void HASHSHA2::Sha2_256(XBYTE* input, XDWORD size, XBYTE* digest)
* @brief      Sha2256
* @ingroup    CIPHER
* 
* @param[in]  input : Input pointer to use.
* @param[in]  size : Size value.
* @param[in]  digest : Digest pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void HASHSHA2::Sha2_256(XBYTE* input, XDWORD size, XBYTE* digest)
{
  HASHSHA2_256_CTX ctx;

  Sha2_256_Ini(&ctx);
  Sha2_256_Update(&ctx, input, size);
  Sha2_256_End(&ctx, digest);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void HASHSHA2::Sha2_256_Ini(HASHSHA2_256_CTX* ctx)
* @brief      Sha2256 ini
* @ingroup    CIPHER
* 
* @param[in]  ctx : Ctx pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void HASHSHA2::Sha2_256_Ini(HASHSHA2_256_CTX* ctx)
{
  int i;

  for(i=0; i<8; i++)
    {
      ctx->h[i] = Sha256_h0[i];
    }

  ctx->size = 0;
  ctx->totalsize = 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void HASHSHA2::Sha2_256_Update(HASHSHA2_256_CTX* ctx, XBYTE* input, XDWORD size)
* @brief      Sha2256 update
* @ingroup    CIPHER
* 
* @param[in]  ctx : Ctx pointer to use.
* @param[in]  input : Input pointer to use.
* @param[in]  size : Size value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void HASHSHA2::Sha2_256_Update(HASHSHA2_256_CTX* ctx, XBYTE* input, XDWORD size)
{
  XDWORD  blocknb;
  XDWORD  new_len, rem_len, tmp_len;
  XBYTE*  shifted_input;

  tmp_len = HASHSHA2_256_BLOCK_SIZE - ctx->size;
  rem_len = size < tmp_len ? size : tmp_len;

  memcpy(&ctx->block[ctx->size], input, rem_len);

  if(ctx->size + size < HASHSHA2_256_BLOCK_SIZE)
    {
      ctx->size += size;
      return;
    }

  new_len = size - rem_len;
  blocknb = new_len / HASHSHA2_256_BLOCK_SIZE;

  shifted_input = input + rem_len;

  Sha2_256_Transf(ctx, ctx->block, 1);
  Sha2_256_Transf(ctx, shifted_input, blocknb);

  rem_len = new_len % HASHSHA2_256_BLOCK_SIZE;

  memcpy(ctx->block, &shifted_input[blocknb << 6], rem_len);

  ctx->size       = rem_len;
  ctx->totalsize += (blocknb + 1) << 6;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void HASHSHA2::Sha2_256_End(HASHSHA2_256_CTX* ctx, XBYTE* digest)
* @brief      Sha2256 end
* @ingroup    CIPHER
* 
* @param[in]  ctx : Ctx pointer to use.
* @param[in]  digest : Digest pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void HASHSHA2::Sha2_256_End(HASHSHA2_256_CTX* ctx, XBYTE* digest)
{
  XDWORD  blocknb;
  XDWORD  pm_len;
  XDWORD  len_b;
  int     i;

  blocknb = (1 + ((HASHSHA2_256_BLOCK_SIZE - 9) < (ctx->size % HASHSHA2_256_BLOCK_SIZE)));

  len_b  = (ctx->totalsize + ctx->size) << 3;
  pm_len = blocknb << 6;

  memset(ctx->block + ctx->size, 0, pm_len - ctx->size);
  ctx->block[ctx->size] = 0x80;
  UNPACK32(len_b, ctx->block + pm_len - 4);

  Sha2_256_Transf(ctx, ctx->block, blocknb);

  for(i=0 ; i<8; i++)
    {
      UNPACK32(ctx->h[i], &digest[i << 2]);
    }

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void HASHSHA2::Sha2_512_Transf(HASHSHA2_512_CTX* ctx, XBYTE* input, XDWORD blocknb)
* @brief      Sha2512 transf
* @ingroup    CIPHER
* 
* @param[in]  ctx : Ctx pointer to use.
* @param[in]  input : Input pointer to use.
* @param[in]  blocknb : Blocknb value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void HASHSHA2::Sha2_512_Transf(HASHSHA2_512_CTX* ctx, XBYTE* input, XDWORD blocknb)
{
  XQWORD  w[80];
  XQWORD  wv[8];
  XQWORD  t1;
  XQWORD  t2;
  XBYTE*  sub_block;
  int     i;
  int     j;

  for(i=0; i<(int)blocknb; i++)
    {
      sub_block = input + (i << 7);

      for(j=0; j<16; j++)
        {
          PACK64(&sub_block[j << 3], &w[j]);
        }

      for(j=16; j<80; j++)
        {
          SHA512_SCR(j);
        }

      for(j=0; j<8; j++)
        {
          wv[j] = ctx->h[j];
        }

      for(j=0; j<80; j++)
        {
          t1 = wv[7] + SHA512_F2(wv[4]) + CH(wv[4], wv[5], wv[6]) + Sha512_k[j] + w[j];
          t2 = SHA512_F1(wv[0]) + MAJ(wv[0], wv[1], wv[2]);

          wv[7] = wv[6];
          wv[6] = wv[5];
          wv[5] = wv[4];
          wv[4] = wv[3] + t1;
          wv[3] = wv[2];
          wv[2] = wv[1];
          wv[1] = wv[0];
          wv[0] = t1 + t2;
        }

      for(j=0; j<8; j++)
        {
          ctx->h[j] += wv[j];
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void HASHSHA2::Sha2_512(XBYTE* input, XDWORD size, XBYTE* digest)
* @brief      Sha2512
* @ingroup    CIPHER
* 
* @param[in]  input : Input pointer to use.
* @param[in]  size : Size value.
* @param[in]  digest : Digest pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void HASHSHA2::Sha2_512(XBYTE* input, XDWORD size, XBYTE* digest)
{
  HASHSHA2_512_CTX ctx;

  Sha2_512_Ini(&ctx);
  Sha2_512_Update(&ctx, input, size);
  Sha2_512_End(&ctx, digest);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void HASHSHA2::Sha2_512_Ini(HASHSHA2_512_CTX* ctx)
* @brief      Sha2512 ini
* @ingroup    CIPHER
* 
* @param[in]  ctx : Ctx pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void HASHSHA2::Sha2_512_Ini(HASHSHA2_512_CTX* ctx)
{
  int i;

  for(i=0; i<8; i++)
    {
      ctx->h[i] = Sha512_h0[i];
    }

    ctx->size = 0;
    ctx->totalsize = 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void HASHSHA2::Sha2_512_Update(HASHSHA2_512_CTX* ctx, XBYTE* input, XDWORD size)
* @brief      Sha2512 update
* @ingroup    CIPHER
* 
* @param[in]  ctx : Ctx pointer to use.
* @param[in]  input : Input pointer to use.
* @param[in]  size : Size value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void HASHSHA2::Sha2_512_Update(HASHSHA2_512_CTX* ctx, XBYTE* input, XDWORD size)
{
  XDWORD blocknb;
  XDWORD new_len, rem_len, tmp_len;
  XBYTE* shifted_input;

  tmp_len = HASHSHA2_512_BLOCK_SIZE - ctx->size;
  rem_len = size < tmp_len ? size : tmp_len;

  memcpy(&ctx->block[ctx->size], input, rem_len);

  if(ctx->size + size < HASHSHA2_512_BLOCK_SIZE)
    {
      ctx->size += size;
      return;
    }

  new_len = size - rem_len;
  blocknb = new_len / HASHSHA2_512_BLOCK_SIZE;

  shifted_input = input + rem_len;

  Sha2_512_Transf(ctx, ctx->block, 1);
  Sha2_512_Transf(ctx, shifted_input, blocknb);

  rem_len = new_len % HASHSHA2_512_BLOCK_SIZE;

  memcpy(ctx->block, &shifted_input[blocknb << 7], rem_len);

  ctx->size = rem_len;
  ctx->totalsize += (blocknb + 1) << 7;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void HASHSHA2::Sha2_512_End(HASHSHA2_512_CTX* ctx, XBYTE* digest)
* @brief      Sha2512 end
* @ingroup    CIPHER
* 
* @param[in]  ctx : Ctx pointer to use.
* @param[in]  digest : Digest pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void HASHSHA2::Sha2_512_End(HASHSHA2_512_CTX* ctx, XBYTE* digest)
{
  XDWORD  blocknb;
  XDWORD  pm_len;
  XDWORD  len_b;
  int     i;

  blocknb = 1 + ((HASHSHA2_512_BLOCK_SIZE - 17) < (ctx->size % HASHSHA2_512_BLOCK_SIZE));

  len_b = (ctx->totalsize + ctx->size) << 3;
  pm_len = blocknb << 7;

  memset(ctx->block + ctx->size, 0, pm_len - ctx->size);
  ctx->block[ctx->size] = 0x80;

  UNPACK32(len_b, ctx->block + pm_len - 4);

  Sha2_512_Transf(ctx, ctx->block, blocknb);

  for(i=0 ; i<8; i++)
    {
      UNPACK64(ctx->h[i], &digest[i << 3]);
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void HASHSHA2::Sha2_384(XBYTE* input, XDWORD size, XBYTE* digest)
* @brief      Sha2384
* @ingroup    CIPHER
* 
* @param[in]  input : Input pointer to use.
* @param[in]  size : Size value.
* @param[in]  digest : Digest pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void HASHSHA2::Sha2_384(XBYTE* input, XDWORD size, XBYTE* digest)
{
  HASHSHA2_384_CTX ctx;

  Sha2_384_Ini(&ctx);
  Sha2_384_Update(&ctx, input, size);
  Sha2_384_End(&ctx, digest);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void HASHSHA2::Sha2_384_Ini(HASHSHA2_384_CTX* ctx)
* @brief      Sha2384 ini
* @ingroup    CIPHER
* 
* @param[in]  ctx : Ctx pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void HASHSHA2::Sha2_384_Ini(HASHSHA2_384_CTX* ctx)
{
  int i;

  for(i=0; i<8; i++)
    {
      ctx->h[i] = Sha384_h0[i];
    }

  ctx->size      = 0;
  ctx->totalsize = 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void HASHSHA2::Sha2_384_Update(HASHSHA2_384_CTX* ctx, XBYTE* input, XDWORD len)
* @brief      Sha2384 update
* @ingroup    CIPHER
* 
* @param[in]  ctx : Ctx pointer to use.
* @param[in]  input : Input pointer to use.
* @param[in]  len : Len value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void HASHSHA2::Sha2_384_Update(HASHSHA2_384_CTX* ctx, XBYTE* input, XDWORD len)
{
  XDWORD block_nb;
  XDWORD new_len, rem_len, tmp_len;
  XBYTE* shifted_input;

  tmp_len = HASHSHA2_384_BLOCK_SIZE - ctx->size;
  rem_len = len < tmp_len ? len : tmp_len;

  memcpy(&ctx->block[ctx->size], input, rem_len);

  if(ctx->size + len < HASHSHA2_384_BLOCK_SIZE)
    {
      ctx->size += len;
      return;
    }

  new_len = len - rem_len;
  block_nb = new_len / HASHSHA2_384_BLOCK_SIZE;

  shifted_input = input + rem_len;

  Sha2_512_Transf(ctx, ctx->block, 1);
  Sha2_512_Transf(ctx, shifted_input, block_nb);

  rem_len = new_len % HASHSHA2_384_BLOCK_SIZE;

  memcpy(ctx->block, &shifted_input[block_nb << 7], rem_len);

  ctx->size       = rem_len;
  ctx->totalsize += (block_nb + 1) << 7;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void HASHSHA2::Sha2_384_End(HASHSHA2_384_CTX* ctx, XBYTE* digest)
* @brief      Sha2384 end
* @ingroup    CIPHER
* 
* @param[in]  ctx : Ctx pointer to use.
* @param[in]  digest : Digest pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void HASHSHA2::Sha2_384_End(HASHSHA2_384_CTX* ctx, XBYTE* digest)
{
  XDWORD block_nb;
  XDWORD pm_len;
  XDWORD len_b;
  int    i;

  block_nb = (1 + ((HASHSHA2_384_BLOCK_SIZE - 17) < (ctx->size % HASHSHA2_384_BLOCK_SIZE)));

  len_b = (ctx->totalsize + ctx->size) << 3;
  pm_len = block_nb << 7;

  memset(ctx->block + ctx->size, 0, pm_len - ctx->size);
  ctx->block[ctx->size] = 0x80;

  UNPACK32(len_b, ctx->block + pm_len - 4);

  Sha2_512_Transf(ctx, ctx->block, block_nb);

  for(i=0 ; i<6; i++)
    {
      UNPACK64(ctx->h[i], &digest[i << 3]);
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void HASHSHA2::Sha2_224(XBYTE* input, XDWORD len, XBYTE* digest)
* @brief      Sha2224
* @ingroup    CIPHER
* 
* @param[in]  input : Input pointer to use.
* @param[in]  len : Len value.
* @param[in]  digest : Digest pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void HASHSHA2::Sha2_224(XBYTE* input, XDWORD len, XBYTE* digest)
{
  HASHSHA2_224_CTX ctx;

  Sha2_224_Ini(&ctx);
  Sha2_224_Update(&ctx, input, len);
  Sha2_224_End(&ctx, digest);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void HASHSHA2::Sha2_224_Ini(HASHSHA2_224_CTX* ctx)
* @brief      Sha2224 ini
* @ingroup    CIPHER
* 
* @param[in]  ctx : Ctx pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void HASHSHA2::Sha2_224_Ini(HASHSHA2_224_CTX* ctx)
{
  int i;

  for(i=0; i<8; i++)
    {
      ctx->h[i] = Sha224_h0[i];
    }

  ctx->size      = 0;
  ctx->totalsize = 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void HASHSHA2::Sha2_224_Update(HASHSHA2_224_CTX* ctx, XBYTE* input, XDWORD len)
* @brief      Sha2224 update
* @ingroup    CIPHER
* 
* @param[in]  ctx : Ctx pointer to use.
* @param[in]  input : Input pointer to use.
* @param[in]  len : Len value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void HASHSHA2::Sha2_224_Update(HASHSHA2_224_CTX* ctx, XBYTE* input, XDWORD len)
{
  XDWORD  block_nb;
  XDWORD  new_len;
  XDWORD  rem_len;
  XDWORD  tmp_len;
  XBYTE*  shifted_input;

  tmp_len = HASHSHA2_224_BLOCK_SIZE - ctx->size;
  rem_len = len < tmp_len ? len : tmp_len;

  memcpy(&ctx->block[ctx->size], input, rem_len);

  if(ctx->size + len < HASHSHA2_224_BLOCK_SIZE)
    {
      ctx->size += len;
      return;
    }

  new_len = len - rem_len;
  block_nb = new_len / HASHSHA2_224_BLOCK_SIZE;

  shifted_input = input + rem_len;

  Sha2_256_Transf(ctx, ctx->block, 1);
  Sha2_256_Transf(ctx, shifted_input, block_nb);

  rem_len = new_len % HASHSHA2_224_BLOCK_SIZE;

  memcpy(ctx->block, &shifted_input[block_nb << 6], rem_len);

  ctx->size       = rem_len;
  ctx->totalsize += (block_nb + 1) << 6;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void HASHSHA2::Sha2_224_End(HASHSHA2_224_CTX* ctx, XBYTE* digest)
* @brief      Sha2224 end
* @ingroup    CIPHER
* 
* @param[in]  ctx : Ctx pointer to use.
* @param[in]  digest : Digest pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void HASHSHA2::Sha2_224_End(HASHSHA2_224_CTX* ctx, XBYTE* digest)
{
  XDWORD block_nb;
  XDWORD pm_len;
  XDWORD len_b;
  int    i;

  block_nb = (1 + ((HASHSHA2_224_BLOCK_SIZE - 9) < (ctx->size % HASHSHA2_224_BLOCK_SIZE)));

  len_b = (ctx->totalsize + ctx->size) << 3;
  pm_len = block_nb << 6;

  memset(ctx->block + ctx->size, 0, pm_len - ctx->size);
  ctx->block[ctx->size] = 0x80;

  UNPACK32(len_b, ctx->block + pm_len - 4);

  Sha2_256_Transf(ctx, ctx->block, block_nb);

  for(i=0 ; i<7; i++)
    {
      UNPACK32(ctx->h[i], &digest[i << 2]);
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void HASHSHA2::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
void HASHSHA2::Clean()
{

}



