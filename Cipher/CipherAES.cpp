/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       CipherAES.cpp
* 
* @class      CIPHERAES
* @brief      Cipher AES class
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

#include "CipherAES.h"

#include <string.h>

#include "XFactory.h"

#include "CipherKeySymmetrical.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

#define GET_UINT32_LE(n,b,i)                  {   (n) =   ( (XDWORD) (b)[(i)    ]       )     \
                                                        | ( (XDWORD) (b)[(i) + 1] <<  8 )     \
                                                        | ( (XDWORD) (b)[(i) + 2] << 16 )     \
                                                        | ( (XDWORD) (b)[(i) + 3] << 24 );    \
                                              }

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         define PUT_UINT32_LE(n, b, i)
* @brief      PUT UINT32 LE
* @ingroup    CIPHER
* 
* @param[in]  n : N value.
* @param[in]  b : B value.
* @param[in]  i : I value.
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
#define PUT_UINT32_LE(n,b,i)                  {   (b)[(i)    ] = (XBYTE) ( (n)       );       \
                                                  (b)[(i) + 1] = (XBYTE) ( (n) >>  8 );       \
                                                  (b)[(i) + 2] = (XBYTE) ( (n) >> 16 );       \
                                                  (b)[(i) + 3] = (XBYTE) ( (n) >> 24 );       \
                                              }

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         define AES_FROUND(X0, X1, X2, X3, Y0, Y1, Y2, Y3)
* @brief      AES FROUND
* @ingroup    CIPHER
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
#define AES_FROUND(X0,X1,X2,X3,Y0,Y1,Y2,Y3)   {   X0 = *RK++ ^ FT0[ ( Y0       ) & 0xFF ] ^   \
                                                               FT1[ ( Y1 >>  8 ) & 0xFF ] ^   \
                                                               FT2[ ( Y2 >> 16 ) & 0xFF ] ^   \
                                                               FT3[ ( Y3 >> 24 ) & 0xFF ];    \
                                                                                              \
                                                  X1 = *RK++ ^ FT0[ ( Y1       ) & 0xFF ] ^   \
                                                               FT1[ ( Y2 >>  8 ) & 0xFF ] ^   \
                                                               FT2[ ( Y3 >> 16 ) & 0xFF ] ^   \
                                                               FT3[ ( Y0 >> 24 ) & 0xFF ];    \
                                                                                              \
                                                  X2 = *RK++ ^ FT0[ ( Y2       ) & 0xFF ] ^   \
                                                               FT1[ ( Y3 >>  8 ) & 0xFF ] ^   \
                                                               FT2[ ( Y0 >> 16 ) & 0xFF ] ^   \
                                                               FT3[ ( Y1 >> 24 ) & 0xFF ];    \
                                                                                              \
                                                  X3 = *RK++ ^ FT0[ ( Y3       ) & 0xFF ] ^   \
                                                               FT1[ ( Y0 >>  8 ) & 0xFF ] ^   \
                                                               FT2[ ( Y1 >> 16 ) & 0xFF ] ^   \
                                                               FT3[ ( Y2 >> 24 ) & 0xFF ];    \
                                              }

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         define AES_RROUND(X0, X1, X2, X3, Y0, Y1, Y2, Y3)
* @brief      AES RROUND
* @ingroup    CIPHER
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
#define AES_RROUND(X0,X1,X2,X3,Y0,Y1,Y2,Y3)   {   X0 = *RK++ ^ RT0[ ( Y0       ) & 0xFF ] ^   \
                                                               RT1[ ( Y3 >>  8 ) & 0xFF ] ^   \
                                                               RT2[ ( Y2 >> 16 ) & 0xFF ] ^   \
                                                               RT3[ ( Y1 >> 24 ) & 0xFF ];    \
                                                                                              \
                                                  X1 = *RK++ ^ RT0[ ( Y1       ) & 0xFF ] ^   \
                                                               RT1[ ( Y0 >>  8 ) & 0xFF ] ^   \
                                                               RT2[ ( Y3 >> 16 ) & 0xFF ] ^   \
                                                               RT3[ ( Y2 >> 24 ) & 0xFF ];    \
                                                                                              \
                                                  X2 = *RK++ ^ RT0[ ( Y2       ) & 0xFF ] ^   \
                                                               RT1[ ( Y1 >>  8 ) & 0xFF ] ^   \
                                                               RT2[ ( Y0 >> 16 ) & 0xFF ] ^   \
                                                               RT3[ ( Y3 >> 24 ) & 0xFF ];    \
                                                                                              \
                                                  X3 = *RK++ ^ RT0[ ( Y3       ) & 0xFF ] ^   \
                                                               RT1[ ( Y2 >>  8 ) & 0xFF ] ^   \
                                                               RT2[ ( Y1 >> 16 ) & 0xFF ] ^   \
                                                               RT3[ ( Y0 >> 24 ) & 0xFF ];    \
                                              }



#define ROTL8(x)  (( x << 8 ) & 0xFFFFFFFF ) | ( x >> 24 )
#define XTIME(x)  (( x << 1 ) ^ ( ( x & 0x80 ) ? 0x1B : 0x00 ))
#define MUL(x,y)  (( x && y ) ? pow[(log[x]+log[y]) % 255] : 0)


XBYTE   CIPHERAES::FSb[256];
XDWORD  CIPHERAES::FT0[256];
XDWORD  CIPHERAES::FT1[256];
XDWORD  CIPHERAES::FT2[256];
XDWORD  CIPHERAES::FT3[256];

XBYTE   CIPHERAES::RSb[256];
XDWORD  CIPHERAES::RT0[256];
XDWORD  CIPHERAES::RT1[256];
XDWORD  CIPHERAES::RT2[256];
XDWORD  CIPHERAES::RT3[256];

XDWORD  CIPHERAES::RCON[10];



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHERAES::CIPHERAES()
* @brief      Constructor of class
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERAES::CIPHERAES()
{
  Clean();

  type              = CIPHERTYPE_AES;
  paddingadjustsize = 16;

  AESGenTables();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHERAES::~CIPHERAES()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERAES::~CIPHERAES()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERAES::Cipher(XBYTE* input, XDWORD size)
* @brief      Cipher
* @ingroup    CIPHER
* 
* @param[in]  input : Input pointer to use.
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERAES::Cipher(XBYTE* input, XDWORD size)
{
  if(!size) return false;

  XBUFFER             inputpadding;
  CIPHERAES_CONTEXT   ctx;
  XBYTE               iv[16];

  memset(iv, 0, 16);
  if(inivector && inivector->GetSize()>=16) memcpy(iv, inivector->Get(), 16);

  inputpadding.Add(input, size);
  inputpadding.Padding_Add(paddingadjustsize, paddingtype);

  result->Delete();
  result->Add(input, size);
  result->Padding_Add(paddingadjustsize, paddingtype);

  CIPHERKEYSYMMETRICAL* key = (CIPHERKEYSYMMETRICAL*)GetKey();
  if(!key)               return false;
  if(!key->Get()->Get()) return false;

  bool status = AESSetKeyCipher(&ctx, (XBYTE *)key->Get()->Get(), key->Get()->GetSize()*8);
  if(!status) return false;

  XBYTE stream_block[16];
  XBYTE nonce_counter[16];
  int   offset  = 0;

  memset(stream_block, 0, 16);
  memset(nonce_counter, 0, 16);
  memcpy(nonce_counter, iv, 16);

  switch(this->GetChainingMode())
    {
      case CIPHERCHAININGMODE_UNKNOWN : status =false;
                                        break;

      case CIPHERCHAININGMODE_ECB     : status = AESCipher_ECB(&ctx, CIPHERAES_ENCRYPT, result->GetSize(), inputpadding.Get(), result->Get());
                                        break;

      case CIPHERCHAININGMODE_CBC     : status = AESCipher_CBC(&ctx, CIPHERAES_ENCRYPT, result->GetSize(), iv, inputpadding.Get(), result->Get());
                                        break;

      case CIPHERCHAININGMODE_CFB     : status = AESCipher_CFB128(&ctx, CIPHERAES_ENCRYPT, result->GetSize(), &offset, iv, inputpadding.Get(), result->Get());
                                        break;

      case CIPHERCHAININGMODE_CTR     : status = AESCipher_CTR(&ctx, result->GetSize(), &offset, nonce_counter, stream_block, inputpadding.Get(), result->Get());
                                        break;

      case CIPHERCHAININGMODE_GCM     :                                             // AEAD modes go through CipherAEAD()/UncipherAEAD(), see CIPHERAESGCM
      case CIPHERCHAININGMODE_POLY1305: status = false;
                                        break;
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERAES::Uncipher(XBYTE* input, XDWORD size)
* @brief      Uncipher
* @ingroup    CIPHER
* 
* @param[in]  input : Input pointer to use.
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERAES::Uncipher(XBYTE* input, XDWORD size)
{
  if(size == 0) 
    {
      return false;
    }

  CIPHERAES_CONTEXT ctx;
  XBYTE             iv[16];

  memset(iv, 0, 16);
  if(inivector && inivector->GetSize()>=8) memcpy(iv, inivector->Get(), 16);

  result->Delete();
  result->Add(input, size);

  CIPHERKEYSYMMETRICAL* key = (CIPHERKEYSYMMETRICAL*)GetKey();
  if(!key)               return false;
  if(!key->Get()->Get()) return false;

  AESSetKeyUncipher(&ctx, (XBYTE *)key->Get()->Get(), key->Get()->GetSize()*8);

  XBYTE stream_block[16];
  XBYTE nonce_counter[16];
  bool  status  = false;
  int   offset  = 0;

  memset(stream_block, 0, 16);
  memset(nonce_counter, 0, 16);

  switch(this->GetChainingMode())
    {
      case CIPHERCHAININGMODE_UNKNOWN : status =false;
                                        break;

      case CIPHERCHAININGMODE_ECB     : status = AESCipher_ECB(&ctx, CIPHERAES_DECRYPT, size, input, result->Get());
                                        break;

      case CIPHERCHAININGMODE_CBC     : status = AESCipher_CBC(&ctx, CIPHERAES_DECRYPT, size, iv, input, result->Get());
                                        break;

      case CIPHERCHAININGMODE_CFB     : status = AESCipher_CFB128(&ctx, CIPHERAES_DECRYPT, size, &offset, iv, input, result->Get());
                                        break;

      case CIPHERCHAININGMODE_CTR     : status = AESCipher_CTR(&ctx, size, &offset, nonce_counter, stream_block, input, result->Get());
                                        break;

      case CIPHERCHAININGMODE_GCM     :                                             // AEAD modes go through CipherAEAD()/UncipherAEAD(), see CIPHERAESGCM
      case CIPHERCHAININGMODE_POLY1305: status = false;
                                        break;
    }

  result->Padding_Delete();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void CIPHERAES::AESGenTables(void)
* @brief      AES gen tables
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
void CIPHERAES::AESGenTables(void)
{
  int pow[256];
  int log[256];
  int i;
  int x;
  int y;
  int z;

  // Compute pow and log tables over GF(2^8)

  for(i=0, x=1; i<256; i++)
    {
      pow[i] = x;
      log[x] = i;
      x = ( x ^ XTIME( x ) ) & 0xFF;
    }


  // Calculate the round constants

  for(i=0, x=1; i<10; i++)
    {
      RCON[i] = (XDWORD) x;
      x = XTIME( x ) & 0xFF;
    }


  // Generate the forward and reverse S-boxes

  FSb[0x00] = 0x63;
  RSb[0x63] = 0x00;

  for(i=1; i<256; i++)
    {
      x = pow[255 - log[i]];

      y  = x; y = ( (y << 1) | (y >> 7) ) & 0xFF;
      x ^= y; y = ( (y << 1) | (y >> 7) ) & 0xFF;
      x ^= y; y = ( (y << 1) | (y >> 7) ) & 0xFF;
      x ^= y; y = ( (y << 1) | (y >> 7) ) & 0xFF;
      x ^= y ^ 0x63;

      FSb[i] = (XBYTE) x;
      RSb[x] = (XBYTE) i;
    }


  // Generate the forward and reverse tables

  for( i = 0; i < 256; i++ )
    {
      x = FSb[i];
      y = XTIME( x ) & 0xFF;
      z =  ( y ^ x ) & 0xFF;

      FT0[i] = ( (XDWORD) y       ) ^
               ( (XDWORD) x <<  8 ) ^
               ( (XDWORD) x << 16 ) ^
               ( (XDWORD) z << 24 );

      FT1[i] = ROTL8( FT0[i] );
      FT2[i] = ROTL8( FT1[i] );
      FT3[i] = ROTL8( FT2[i] );

      x = RSb[i];

      RT0[i] = ( (XDWORD) MUL( 0x0E, x )       ) ^
               ( (XDWORD) MUL( 0x09, x ) <<  8 ) ^
               ( (XDWORD) MUL( 0x0D, x ) << 16 ) ^
               ( (XDWORD) MUL( 0x0B, x ) << 24 );

      RT1[i] = ROTL8( RT0[i] );
      RT2[i] = ROTL8( RT1[i] );
      RT3[i] = ROTL8( RT2[i] );
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERAES::AESSetKeyCipher(CIPHERAES_CONTEXT* ctx, XBYTE* key, XDWORD keysize)
* @brief      AES set key cipher
* @ingroup    CIPHER
* 
* @param[in]  ctx : Ctx pointer to use.
* @param[in]  key : Key pointer to use.
* @param[in]  keysize : Keysize value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERAES::AESSetKeyCipher(CIPHERAES_CONTEXT* ctx, XBYTE* key, XDWORD keysize)
{
  XDWORD* RK;

  switch( keysize )
    {
      case 128 : ctx->nr = 10; break;
      case 192 : ctx->nr = 12; break;
      case 256 : ctx->nr = 14; break;
      default  : return false;
    }

  ctx->rk = RK = ctx->buf;

  for(XDWORD i = 0; i < (keysize >> 5); i++ )
    {
      GET_UINT32_LE( RK[i], key, i << 2 );
    }

  switch( ctx->nr )
    {
      case  10: for(XDWORD i = 0; i < 10; i++, RK += 4 )
                  {
                    RK[4]  = RK[0] ^ RCON[i] ^
                             ( (XDWORD) FSb[ ( RK[3] >>  8 ) & 0xFF ]       ) ^
                             ( (XDWORD) FSb[ ( RK[3] >> 16 ) & 0xFF ] <<  8 ) ^
                             ( (XDWORD) FSb[ ( RK[3] >> 24 ) & 0xFF ] << 16 ) ^
                             ( (XDWORD) FSb[ ( RK[3]       ) & 0xFF ] << 24 );

                    RK[5]  = RK[1] ^ RK[4];
                    RK[6]  = RK[2] ^ RK[5];
                    RK[7]  = RK[3] ^ RK[6];
                  }
                break;

      case  12: for(XDWORD i = 0; i < 8; i++, RK += 6 )
                  {
                    RK[6]  = RK[0] ^ RCON[i] ^
                             ( (XDWORD) FSb[ ( RK[5] >>  8 ) & 0xFF ]       ) ^
                             ( (XDWORD) FSb[ ( RK[5] >> 16 ) & 0xFF ] <<  8 ) ^
                             ( (XDWORD) FSb[ ( RK[5] >> 24 ) & 0xFF ] << 16 ) ^
                             ( (XDWORD) FSb[ ( RK[5]       ) & 0xFF ] << 24 );

                    RK[7]  = RK[1] ^ RK[6];
                    RK[8]  = RK[2] ^ RK[7];
                    RK[9]  = RK[3] ^ RK[8];
                    RK[10] = RK[4] ^ RK[9];
                    RK[11] = RK[5] ^ RK[10];
                  }
                break;

      case  14: for(XDWORD i = 0; i < 7; i++, RK += 8 )
                  {
                    RK[8]  = RK[0] ^ RCON[i] ^
                             ( (XDWORD) FSb[ ( RK[7] >>  8 ) & 0xFF ]       ) ^
                             ( (XDWORD) FSb[ ( RK[7] >> 16 ) & 0xFF ] <<  8 ) ^
                             ( (XDWORD) FSb[ ( RK[7] >> 24 ) & 0xFF ] << 16 ) ^
                             ( (XDWORD) FSb[ ( RK[7]       ) & 0xFF ] << 24 );

                    RK[9]  = RK[1] ^ RK[8];
                    RK[10] = RK[2] ^ RK[9];
                    RK[11] = RK[3] ^ RK[10];

                    RK[12] = RK[4] ^
                             ( (XDWORD) FSb[ ( RK[11]       ) & 0xFF ]       ) ^
                             ( (XDWORD) FSb[ ( RK[11] >>  8 ) & 0xFF ] <<  8 ) ^
                             ( (XDWORD) FSb[ ( RK[11] >> 16 ) & 0xFF ] << 16 ) ^
                             ( (XDWORD) FSb[ ( RK[11] >> 24 ) & 0xFF ] << 24 );

                    RK[13] = RK[5] ^ RK[12];
                    RK[14] = RK[6] ^ RK[13];
                    RK[15] = RK[7] ^ RK[14];
                  }
                 break;

        default: break;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERAES::AESSetKeyUncipher(CIPHERAES_CONTEXT* ctx, XBYTE* key, XDWORD keysize)
* @brief      AES set key uncipher
* @ingroup    CIPHER
* 
* @param[in]  ctx : Ctx pointer to use.
* @param[in]  key : Key pointer to use.
* @param[in]  keysize : Keysize value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERAES::AESSetKeyUncipher(CIPHERAES_CONTEXT* ctx, XBYTE* key, XDWORD keysize)
{
  CIPHERAES_CONTEXT cty;
  int                 i;
  int                 j;
  XDWORD*             RK;
  XDWORD*             SK;

  switch(keysize)
    {
      case 128: ctx->nr = 10; break;
      case 192: ctx->nr = 12; break;
      case 256: ctx->nr = 14; break;
      default : return false;
    }


  ctx->rk = RK = ctx->buf;

  if(!AESSetKeyCipher( &cty, key, keysize )) return false;

  SK = cty.rk + cty.nr * 4;

  *RK++ = *SK++;
  *RK++ = *SK++;
  *RK++ = *SK++;
  *RK++ = *SK++;

  for(i=ctx->nr-1, SK-=8; i>0; i--, SK-= 8)
    {
      for(j=0; j<4; j++, SK++)
        {
          *RK++ = RT0[ FSb[ ( *SK       ) & 0xFF ] ] ^
                  RT1[ FSb[ ( *SK >>  8 ) & 0xFF ] ] ^
                  RT2[ FSb[ ( *SK >> 16 ) & 0xFF ] ] ^
                  RT3[ FSb[ ( *SK >> 24 ) & 0xFF ] ];
        }
    }

  *RK++ = *SK++;
  *RK++ = *SK++;
  *RK++ = *SK++;
  *RK++ = *SK++;

  memset( &cty, 0, sizeof( CIPHERAES_CONTEXT ) );

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERAES::AESCipher_ECB_Block(CIPHERAES_CONTEXT* ctx, int mode, XBYTE input[16], XBYTE output[16])
* @brief      AES cipher ECB block
* @ingroup    CIPHER
* 
* @param[in]  ctx : Ctx pointer to use.
* @param[in]  mode : Mode value.
* @param[in]  input : Input value.
* @param[in]  output : Output output.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERAES::AESCipher_ECB_Block(CIPHERAES_CONTEXT* ctx, int mode, XBYTE input[16], XBYTE output[16])
{
  if(!ctx) return false;

  XDWORD* RK;
  XDWORD  X0;
  XDWORD  X1;
  XDWORD  X2;
  XDWORD  X3;
  XDWORD  Y0;
  XDWORD  Y1;
  XDWORD  Y2;
  XDWORD  Y3;
  int     i;

  RK = ctx->rk;

  GET_UINT32_LE(X0, input, 0); X0 ^= *RK++;
  GET_UINT32_LE(X1, input, 4); X1 ^= *RK++;
  GET_UINT32_LE(X2, input, 8); X2 ^= *RK++;
  GET_UINT32_LE( X3, input, 12 ); X3 ^= *RK++;

  if(mode == CIPHERAES_DECRYPT)
    {
      for( i = (ctx->nr >> 1) - 1; i > 0; i-- )
        {
          AES_RROUND( Y0, Y1, Y2, Y3, X0, X1, X2, X3 );
          AES_RROUND( X0, X1, X2, X3, Y0, Y1, Y2, Y3 );
        }

      AES_RROUND( Y0, Y1, Y2, Y3, X0, X1, X2, X3 );

      X0 = *RK++ ^ \
           ( (XDWORD) RSb[ ( Y0       ) & 0xFF ]       ) ^
           ( (XDWORD) RSb[ ( Y3 >>  8 ) & 0xFF ] <<  8 ) ^
           ( (XDWORD) RSb[ ( Y2 >> 16 ) & 0xFF ] << 16 ) ^
           ( (XDWORD) RSb[ ( Y1 >> 24 ) & 0xFF ] << 24 );

      X1 = *RK++ ^ \
           ( (XDWORD) RSb[ ( Y1       ) & 0xFF ]       ) ^
           ( (XDWORD) RSb[ ( Y0 >>  8 ) & 0xFF ] <<  8 ) ^
           ( (XDWORD) RSb[ ( Y3 >> 16 ) & 0xFF ] << 16 ) ^
           ( (XDWORD) RSb[ ( Y2 >> 24 ) & 0xFF ] << 24 );

      X2 = *RK++ ^ \
           ( (XDWORD) RSb[ ( Y2       ) & 0xFF ]       ) ^
           ( (XDWORD) RSb[ ( Y1 >>  8 ) & 0xFF ] <<  8 ) ^
           ( (XDWORD) RSb[ ( Y0 >> 16 ) & 0xFF ] << 16 ) ^
           ( (XDWORD) RSb[ ( Y3 >> 24 ) & 0xFF ] << 24 );

      X3 = *RK++ ^ \
           ( (XDWORD) RSb[ ( Y3       ) & 0xFF ]       ) ^
           ( (XDWORD) RSb[ ( Y2 >>  8 ) & 0xFF ] <<  8 ) ^
           ( (XDWORD) RSb[ ( Y1 >> 16 ) & 0xFF ] << 16 ) ^
           ( (XDWORD) RSb[ ( Y0 >> 24 ) & 0xFF ] << 24 );
    }
   else
    {
      for( i = (ctx->nr >> 1) - 1; i > 0; i-- )
        {
          AES_FROUND( Y0, Y1, Y2, Y3, X0, X1, X2, X3 );
          AES_FROUND( X0, X1, X2, X3, Y0, Y1, Y2, Y3 );
        }

      AES_FROUND( Y0, Y1, Y2, Y3, X0, X1, X2, X3 );

      X0 = *RK++ ^ \
           ( (XDWORD) FSb[ ( Y0       ) & 0xFF ]       ) ^
           ( (XDWORD) FSb[ ( Y1 >>  8 ) & 0xFF ] <<  8 ) ^
           ( (XDWORD) FSb[ ( Y2 >> 16 ) & 0xFF ] << 16 ) ^
           ( (XDWORD) FSb[ ( Y3 >> 24 ) & 0xFF ] << 24 );

      X1 = *RK++ ^ \
           ( (XDWORD) FSb[ ( Y1       ) & 0xFF ]       ) ^
           ( (XDWORD) FSb[ ( Y2 >>  8 ) & 0xFF ] <<  8 ) ^
           ( (XDWORD) FSb[ ( Y3 >> 16 ) & 0xFF ] << 16 ) ^
           ( (XDWORD) FSb[ ( Y0 >> 24 ) & 0xFF ] << 24 );

      X2 = *RK++ ^ \
           ( (XDWORD) FSb[ ( Y2       ) & 0xFF ]       ) ^
           ( (XDWORD) FSb[ ( Y3 >>  8 ) & 0xFF ] <<  8 ) ^
           ( (XDWORD) FSb[ ( Y0 >> 16 ) & 0xFF ] << 16 ) ^
           ( (XDWORD) FSb[ ( Y1 >> 24 ) & 0xFF ] << 24 );

      X3 = *RK++ ^ \
           ( (XDWORD) FSb[ ( Y3       ) & 0xFF ]       ) ^
           ( (XDWORD) FSb[ ( Y0 >>  8 ) & 0xFF ] <<  8 ) ^
           ( (XDWORD) FSb[ ( Y1 >> 16 ) & 0xFF ] << 16 ) ^
           ( (XDWORD) FSb[ ( Y2 >> 24 ) & 0xFF ] << 24 );
    }

  PUT_UINT32_LE(X0, output, 0);
  PUT_UINT32_LE(X1, output, 4);
  PUT_UINT32_LE(X2, output, 8);
  PUT_UINT32_LE( X3, output, 12 );

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERAES::AESCipher_ECB(CIPHERAES_CONTEXT* ctx, int mode, XDWORD size, XBYTE* input, XBYTE* output)
* @brief      AES cipher ECB
* @ingroup    CIPHER
* 
* @param[in]  ctx : Ctx pointer to use.
* @param[in]  mode : Mode value.
* @param[in]  size : Size value.
* @param[in]  input : Input pointer to use.
* @param[in]  output : Output output.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERAES::AESCipher_ECB(CIPHERAES_CONTEXT* ctx, int mode, XDWORD size, XBYTE* input, XBYTE* output)
{
  if(size % 16) return false;

  while(size > 0)
    {
      AESCipher_ECB_Block(ctx, mode, input, output);

      input  += 16;
      output += 16;
      size   -= 16;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERAES::AESCipher_CBC(CIPHERAES_CONTEXT* ctx, int mode, XDWORD size, XBYTE iv[16], XBYTE* input, XBYTE* output)
* @brief      AES cipher CBC
* @ingroup    CIPHER
* 
* @param[in]  ctx : Ctx pointer to use.
* @param[in]  mode : Mode value.
* @param[in]  size : Size value.
* @param[in]  iv : Iv value.
* @param[in]  input : Input pointer to use.
* @param[in]  output : Output output.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERAES::AESCipher_CBC(CIPHERAES_CONTEXT* ctx, int mode, XDWORD size, XBYTE iv[16], XBYTE* input, XBYTE* output)
{
  XBYTE temp[16];
  int   i;

  if(size % 16) return false;

  if(mode == CIPHERAES_DECRYPT)
    {
      while(size > 0)
        {
          memcpy(temp, input, 16);
          AESCipher_ECB_Block(ctx, mode, input, output);

          for(i = 0; i<16; i++)
            {
              output[i] = (XBYTE)( output[i] ^ iv[i]);
            }

          memcpy( iv, temp, 16 );

          input  += 16;
          output += 16;
          size   -= 16;
        }
    }
   else
    {
      while(size > 0)
        {
          for(i = 0; i < 16; i++)
            {
              output[i] = (XBYTE)( input[i] ^ iv[i] );
            }

          AESCipher_ECB_Block(ctx, mode, output, output);
          memcpy( iv, output, 16 );

          input  += 16;
          output += 16;
          size   -= 16;
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERAES::AESCipher_CFB128(CIPHERAES_CONTEXT* ctx, int mode, XDWORD size, int* iv_off, XBYTE iv[16], XBYTE* input, XBYTE* output)
* @brief      AES cipher CFB128
* @ingroup    CIPHER
* 
* @param[in]  ctx : Ctx pointer to use.
* @param[in]  mode : Mode value.
* @param[in]  size : Size value.
* @param[in]  iv_off : Iv off pointer to use.
* @param[in]  iv : Iv value.
* @param[in]  input : Input pointer to use.
* @param[in]  output : Output output.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERAES::AESCipher_CFB128(CIPHERAES_CONTEXT* ctx, int mode, XDWORD size, int* iv_off, XBYTE iv[16], XBYTE* input, XBYTE* output)
{
  int c;
  size_t n = *iv_off;

  if( mode == CIPHERAES_DECRYPT )
    {
      while(size--)
        {
          if(n == 0)
            {
              AESCipher_ECB_Block( ctx, CIPHERAES_ENCRYPT, iv, iv );
            }

          c = *input++;
          *output++ = (XBYTE)( c ^ iv[n] );
          iv[n] = (XBYTE) c;

          n = (n + 1) & 0x0F;
        }
    }
   else
    {
      while(size --)
        {
          if(n == 0)
            {
              AESCipher_ECB_Block( ctx, CIPHERAES_ENCRYPT, iv, iv);
            }

          iv[n] = *output++ = (XBYTE)( iv[n] ^ *input++ );

          n = (n + 1) & 0x0F;
        }
    }

  *iv_off = (int)n;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERAES::AESCipher_CTR(CIPHERAES_CONTEXT* ctx, XDWORD size, int* nc_off, XBYTE nonce_counter[16], XBYTE stream_block[16], XBYTE* input, XBYTE* output)
* @brief      AES cipher CTR
* @ingroup    CIPHER
* 
* @param[in]  ctx : Ctx pointer to use.
* @param[in]  size : Size value.
* @param[in]  nc_off : Nc off pointer to use.
* @param[in]  nonce_counter : Nonce counter value.
* @param[in]  stream_block : Stream block value.
* @param[in]  input : Input pointer to use.
* @param[in]  output : Output output.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERAES::AESCipher_CTR(CIPHERAES_CONTEXT* ctx, XDWORD size, int* nc_off, XBYTE nonce_counter[16], XBYTE stream_block[16], XBYTE* input, XBYTE* output)
{
  int c;
  int i;
  int n = *nc_off;

  while(size--)
    {
      if(n == 0)
        {
          AESCipher_ECB_Block( ctx, CIPHERAES_ENCRYPT, nonce_counter, stream_block );

          for( i = 16; i > 0; i-- )
            {
              if(++nonce_counter[i - 1] != 0) break;
            }
        }
      c = *input++;
      *output++ = (XBYTE)( c ^ stream_block[n] );

      n = (n + 1) & 0x0F;
    }

  *nc_off = n;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void CIPHERAES::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
void CIPHERAES::Clean()
{

}



