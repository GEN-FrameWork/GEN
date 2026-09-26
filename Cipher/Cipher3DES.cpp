/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       Cipher3DES.cpp
* 
* @class      CIPHER3DES
* @brief      Cipher 3DES class
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

#include "Cipher3DES.h"

#include <string.h>

#include "XFactory.h"
#include "XBuffer.h"

#include "CipherKeySymmetrical.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHER3DES::CIPHER3DES()
* @brief      Constructor of class
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHER3DES::CIPHER3DES() 
{
  Clean();

  type             = CIPHERTYPE_3DES;
  paddingadjustsize = 16;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHER3DES::~CIPHER3DES()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHER3DES::~CIPHER3DES()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHER3DES::Cipher(XBYTE* input, XDWORD size)
* @brief      Cipher
* @ingroup    CIPHER
* 
* @param[in]  input : Input pointer to use.
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHER3DES::Cipher(XBYTE* input, XDWORD size)
{
  if(!size) return false;

  XBUFFER             inputpadding;
  CIPHER3DES_CONTEXT  ctx;
  XBYTE               iv[8];

  memset(iv, 0, 8);
  if(inivector && inivector->GetSize()>=8) memcpy(iv, inivector->Get(), 8);

  inputpadding.Add(input, size);
  inputpadding.Padding_Add(paddingadjustsize, paddingtype);

  result->Delete();
  result->Add(input, size);
  result->Padding_Add(paddingadjustsize, paddingtype);

  CIPHERKEYSYMMETRICAL* key = (CIPHERKEYSYMMETRICAL*)GetKey();
  if(!key)               return false;
  if(!key->Get()->Get()) return false;

  if(key->Get()->GetSize() <= 16)
        DES3Set2KeyCipher(&ctx, (XBYTE *)key->Get()->Get());
   else DES3Set3KeyCipher(&ctx, (XBYTE *)key->Get()->Get());

  bool status;

  if(this->GetChainingMode() == CIPHERCHAININGMODE_ECB)
         status = DES3Cipher_ECB(&ctx, CIPHERDES_ENCRYPT, result->GetSize(), iv, inputpadding.Get(), result->Get());
    else status = DES3Cipher_CBC(&ctx, CIPHERDES_ENCRYPT, result->GetSize(), iv, inputpadding.Get(), result->Get());

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHER3DES::Uncipher(XBYTE* input, XDWORD size)
* @brief      Uncipher
* @ingroup    CIPHER
* 
* @param[in]  input : Input pointer to use.
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHER3DES::Uncipher(XBYTE* input, XDWORD size)
{
  if(!size) return false;

  CIPHER3DES_CONTEXT ctx;
  XBYTE               iv[8];

  memset(iv, 0, 8);
  if(inivector && inivector->GetSize()>=8) memcpy(iv, inivector->Get(), 8);

  result->Delete();
  result->Add(input, size);

  CIPHERKEYSYMMETRICAL* key = (CIPHERKEYSYMMETRICAL*)GetKey();
  if(!key)               return false;
  if(!key->Get()->Get()) return false;

  if(key->Get()->GetSize() <= 16)
        DES3Set2KeyUncipher(&ctx, (XBYTE *)key->Get()->Get());
   else DES3Set3KeyUncipher(&ctx, (XBYTE *)key->Get()->Get());

  bool status;

  if(this->GetChainingMode() == CIPHERCHAININGMODE_ECB)
         status = DES3Cipher_ECB(&ctx, CIPHERDES_DECRYPT, size, iv, input, result->Get());
    else status = DES3Cipher_CBC(&ctx, CIPHERDES_DECRYPT, size, iv, input, result->Get());

  result->Padding_Delete();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void CIPHER3DES::DES3Set2Key(XDWORD esk[96], XDWORD dsk[96], XBYTE key[CIPHERDES_KEYSIZE*2])
* @brief      DES3 set2 key
* @ingroup    CIPHER
* 
* @param[in]  esk : Esk value.
* @param[in]  dsk : Dsk value.
* @param[in]  key : Key pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void CIPHER3DES::DES3Set2Key(XDWORD esk[96], XDWORD dsk[96], XBYTE key[CIPHERDES_KEYSIZE*2])
{
  DESSetKey(esk, key);

  DESSetKey(dsk + 32, key + 8);

  for(int c=0; c<32; c+=2 )
    {
       dsk[c     ] = esk[30 - c];
       dsk[c +  1] = esk[31 - c];

       esk[c + 32] = dsk[62 - c];
       esk[c + 33] = dsk[63 - c];

       esk[c + 64] = esk[c    ];
       esk[c + 65] = esk[c + 1];

       dsk[c + 64] = dsk[c    ];
       dsk[c + 65] = dsk[c + 1];
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int CIPHER3DES::DES3Set2KeyCipher(CIPHER3DES_CONTEXT* ctx, XBYTE key[CIPHERDES_KEYSIZE*2])
* @brief      DES3 set2 key cipher
* @ingroup    CIPHER
* 
* @param[in]  ctx : Ctx pointer to use.
* @param[in]  key : Key pointer to use.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int CIPHER3DES::DES3Set2KeyCipher(CIPHER3DES_CONTEXT* ctx, XBYTE key[CIPHERDES_KEYSIZE*2])
{
  XDWORD sk[96];

  DES3Set2Key(ctx->sk, sk, key);

  memset(sk, 0, sizeof(sk));

  return 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int CIPHER3DES::DES3Set2KeyUncipher(CIPHER3DES_CONTEXT *ctx, XBYTE key[CIPHERDES_KEYSIZE*2])
* @brief      DES3 set2 key uncipher
* @ingroup    CIPHER
* 
* @param[in]  ctx : Ctx pointer to use.
* @param[in]  key : Key pointer to use.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int CIPHER3DES::DES3Set2KeyUncipher(CIPHER3DES_CONTEXT *ctx, XBYTE key[CIPHERDES_KEYSIZE*2])
{
  XDWORD sk[96];

  DES3Set2Key(sk, ctx->sk, key);

  memset(sk, 0, sizeof(sk));

  return 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void CIPHER3DES::DES3Set3Key(XDWORD esk[96], XDWORD dsk[96], XBYTE key[24])
* @brief      DES3 set3 key
* @ingroup    CIPHER
* 
* @param[in]  esk : Esk value.
* @param[in]  dsk : Dsk value.
* @param[in]  key : Key value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void CIPHER3DES::DES3Set3Key(XDWORD esk[96], XDWORD dsk[96], XBYTE key[24])
{
  DESSetKey(esk, key);
  DESSetKey(dsk + 32, key +  8);
  DESSetKey(esk + 64, key + 16);

  for(int c=0; c<32; c+=2 )
    {
      dsk[c     ] = esk[94 - c];
      dsk[c +  1] = esk[95 - c];

      esk[c + 32] = dsk[62 - c];
      esk[c + 33] = dsk[63 - c];

      dsk[c + 64] = esk[30 - c];
      dsk[c + 65] = esk[31 - c];
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int CIPHER3DES::DES3Set3KeyCipher(CIPHER3DES_CONTEXT* ctx, XBYTE key[CIPHERDES_KEYSIZE*3])
* @brief      DES3 set3 key cipher
* @ingroup    CIPHER
* 
* @param[in]  ctx : Ctx pointer to use.
* @param[in]  key : Key pointer to use.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int CIPHER3DES::DES3Set3KeyCipher(CIPHER3DES_CONTEXT* ctx, XBYTE key[CIPHERDES_KEYSIZE*3])
{
  XDWORD sk[96];

  DES3Set3Key(ctx->sk, sk, key);

  memset(sk, 0, sizeof(sk));

  return 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int CIPHER3DES::DES3Set3KeyUncipher(CIPHER3DES_CONTEXT* ctx, XBYTE key[CIPHERDES_KEYSIZE*3])
* @brief      DES3 set3 key uncipher
* @ingroup    CIPHER
* 
* @param[in]  ctx : Ctx pointer to use.
* @param[in]  key : Key pointer to use.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int CIPHER3DES::DES3Set3KeyUncipher(CIPHER3DES_CONTEXT* ctx, XBYTE key[CIPHERDES_KEYSIZE*3])
{
  XDWORD sk[96];

  DES3Set3Key(sk, ctx->sk, key);

  memset(sk, 0, sizeof(sk));

  return 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int CIPHER3DES::DES3Cipher(CIPHER3DES_CONTEXT* ctx, XBYTE input[8], XBYTE output[8])
* @brief      DES3 cipher
* @ingroup    CIPHER
* 
* @param[in]  ctx : Ctx pointer to use.
* @param[in]  input : Input value.
* @param[in]  output : Output output.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int CIPHER3DES::DES3Cipher(CIPHER3DES_CONTEXT* ctx, XBYTE input[8], XBYTE output[8])
{
  XDWORD  X;
  XDWORD  Y;
  XDWORD  T;
  XDWORD* SK;

  SK = ctx->sk;

  GET_XDWORD_BE( X, input, 0 );
  GET_XDWORD_BE( Y, input, 4 );

  DES_IP( X, Y );

  for(int c=0; c<8; c++)
    {
      DES_ROUND(Y, X);
      DES_ROUND(X, Y);
    }

  for(int c=0; c<8; c++)
    {
      DES_ROUND(X, Y);
      DES_ROUND(Y, X);
    }

  for(int c=0; c<8; c++ )
    {
      DES_ROUND(Y, X);
      DES_ROUND(X, Y);
    }

  DES_FP( Y, X );

  PUT_XDWORD_BE( Y, output, 0 );
  PUT_XDWORD_BE( X, output, 4 );

  return 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHER3DES::DES3Cipher_ECB(CIPHER3DES_CONTEXT* ctx, int mode, size_t length, XBYTE iv[8], XBYTE* input, XBYTE* output)
* @brief      DES3 cipher ECB
* @ingroup    CIPHER
* 
* @param[in]  ctx : Ctx pointer to use.
* @param[in]  mode : Mode value.
* @param[in]  length : Length value.
* @param[in]  iv : Iv value.
* @param[in]  input : Input pointer to use.
* @param[in]  output : Output output.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHER3DES::DES3Cipher_ECB(CIPHER3DES_CONTEXT* ctx, int mode, size_t length, XBYTE iv[8], XBYTE* input, XBYTE* output)
{
  XBYTE temp[8];

  if(length % 8) return false;

  if(mode == CIPHERDES_ENCRYPT)
    {
      while(length > 0)
        {
          DES3Cipher( ctx, output, output);

          memcpy( iv, output, 8 );

          input  += 8;
          output += 8;
          length -= 8;
        }
    }
   else // CIPHERDES_DECRYPT 
    {
      while(length > 0)
        {
          memcpy(temp, input, 8);

          DES3Cipher( ctx, input, output );

          memcpy( iv, temp, 8 );

          input  += 8;
          output += 8;
          length -= 8;
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHER3DES::DES3Cipher_CBC(CIPHER3DES_CONTEXT* ctx, int mode, size_t length, XBYTE iv[8], XBYTE* input, XBYTE* output)
* @brief      DES3 cipher CBC
* @ingroup    CIPHER
* 
* @param[in]  ctx : Ctx pointer to use.
* @param[in]  mode : Mode value.
* @param[in]  length : Length value.
* @param[in]  iv : Iv value.
* @param[in]  input : Input pointer to use.
* @param[in]  output : Output output.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHER3DES::DES3Cipher_CBC(CIPHER3DES_CONTEXT* ctx, int mode, size_t length, XBYTE iv[8], XBYTE* input, XBYTE* output)
{
  XBYTE temp[8];

  if(length % 8) return false;

  if(mode == CIPHERDES_ENCRYPT)
    {
      while(length > 0)
        {
          for(int c=0; c<8; c++)
            {
              output[c] = (XBYTE)(input[c] ^ iv[c]);
            }

          DES3Cipher( ctx, output, output);

          memcpy( iv, output, 8 );

          input  += 8;
          output += 8;
          length -= 8;
        }
    }
   else // CIPHERDES_DECRYPT 
    {
      while(length > 0)
        {
          memcpy(temp, input, 8);

          DES3Cipher( ctx, input, output );

          for(int c=0; c<8; c++ )
            {
              output[c] = (XBYTE)( output[c] ^ iv[c]);
            }

          memcpy( iv, temp, 8 );

          input  += 8;
          output += 8;
          length -= 8;
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void CIPHER3DES::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
void CIPHER3DES::Clean()
{

}



