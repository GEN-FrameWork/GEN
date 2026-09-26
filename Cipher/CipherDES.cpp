/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       CipherDES.cpp
* 
* @class      CIPHERDES
* @brief      Cipher DES class
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

#include "CipherDES.h"

#include <string.h>

#include "XFactory.h"

#include "CipherKeySymmetrical.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

// Expanded DES S-boxes

XDWORD CIPHERDES::SB1[64] =     { 0x01010400, 0x00000000, 0x00010000, 0x01010404,
                                  0x01010004, 0x00010404, 0x00000004, 0x00010000,
                                  0x00000400, 0x01010400, 0x01010404, 0x00000400,
                                  0x01000404, 0x01010004, 0x01000000, 0x00000004,
                                  0x00000404, 0x01000400, 0x01000400, 0x00010400,
                                  0x00010400, 0x01010000, 0x01010000, 0x01000404,
                                  0x00010004, 0x01000004, 0x01000004, 0x00010004,
                                  0x00000000, 0x00000404, 0x00010404, 0x01000000,
                                  0x00010000, 0x01010404, 0x00000004, 0x01010000,
                                  0x01010400, 0x01000000, 0x01000000, 0x00000400,
                                  0x01010004, 0x00010000, 0x00010400, 0x01000004,
                                  0x00000400, 0x00000004, 0x01000404, 0x00010404,
                                  0x01010404, 0x00010004, 0x01010000, 0x01000404,
                                  0x01000004, 0x00000404, 0x00010404, 0x01010400,
                                  0x00000404, 0x01000400, 0x01000400, 0x00000000,
                                  0x00010004, 0x00010400, 0x00000000, 0x01010004
                                };

XDWORD CIPHERDES::SB2[64] =     { 0x80108020, 0x80008000, 0x00008000, 0x00108020,
                                  0x00100000, 0x00000020, 0x80100020, 0x80008020,
                                  0x80000020, 0x80108020, 0x80108000, 0x80000000,
                                  0x80008000, 0x00100000, 0x00000020, 0x80100020,
                                  0x00108000, 0x00100020, 0x80008020, 0x00000000,
                                  0x80000000, 0x00008000, 0x00108020, 0x80100000,
                                  0x00100020, 0x80000020, 0x00000000, 0x00108000,
                                  0x00008020, 0x80108000, 0x80100000, 0x00008020,
                                  0x00000000, 0x00108020, 0x80100020, 0x00100000,
                                  0x80008020, 0x80100000, 0x80108000, 0x00008000,
                                  0x80100000, 0x80008000, 0x00000020, 0x80108020,
                                  0x00108020, 0x00000020, 0x00008000, 0x80000000,
                                  0x00008020, 0x80108000, 0x00100000, 0x80000020,
                                  0x00100020, 0x80008020, 0x80000020, 0x00100020,
                                  0x00108000, 0x00000000, 0x80008000, 0x00008020,
                                  0x80000000, 0x80100020, 0x80108020, 0x00108000
                                };

XDWORD CIPHERDES::SB3[64] =     { 0x00000208, 0x08020200, 0x00000000, 0x08020008,
                                  0x08000200, 0x00000000, 0x00020208, 0x08000200,
                                  0x00020008, 0x08000008, 0x08000008, 0x00020000,
                                  0x08020208, 0x00020008, 0x08020000, 0x00000208,
                                  0x08000000, 0x00000008, 0x08020200, 0x00000200,
                                  0x00020200, 0x08020000, 0x08020008, 0x00020208,
                                  0x08000208, 0x00020200, 0x00020000, 0x08000208,
                                  0x00000008, 0x08020208, 0x00000200, 0x08000000,
                                  0x08020200, 0x08000000, 0x00020008, 0x00000208,
                                  0x00020000, 0x08020200, 0x08000200, 0x00000000,
                                  0x00000200, 0x00020008, 0x08020208, 0x08000200,
                                  0x08000008, 0x00000200, 0x00000000, 0x08020008,
                                  0x08000208, 0x00020000, 0x08000000, 0x08020208,
                                  0x00000008, 0x00020208, 0x00020200, 0x08000008,
                                  0x08020000, 0x08000208, 0x00000208, 0x08020000,
                                  0x00020208, 0x00000008, 0x08020008, 0x00020200
                                };

XDWORD CIPHERDES::SB4[64] =     { 0x00802001, 0x00002081, 0x00002081, 0x00000080,
                                  0x00802080, 0x00800081, 0x00800001, 0x00002001,
                                  0x00000000, 0x00802000, 0x00802000, 0x00802081,
                                  0x00000081, 0x00000000, 0x00800080, 0x00800001,
                                  0x00000001, 0x00002000, 0x00800000, 0x00802001,
                                  0x00000080, 0x00800000, 0x00002001, 0x00002080,
                                  0x00800081, 0x00000001, 0x00002080, 0x00800080,
                                  0x00002000, 0x00802080, 0x00802081, 0x00000081,
                                  0x00800080, 0x00800001, 0x00802000, 0x00802081,
                                  0x00000081, 0x00000000, 0x00000000, 0x00802000,
                                  0x00002080, 0x00800080, 0x00800081, 0x00000001,
                                  0x00802001, 0x00002081, 0x00002081, 0x00000080,
                                  0x00802081, 0x00000081, 0x00000001, 0x00002000,
                                  0x00800001, 0x00002001, 0x00802080, 0x00800081,
                                  0x00002001, 0x00002080, 0x00800000, 0x00802001,
                                  0x00000080, 0x00800000, 0x00002000, 0x00802080
                                };

XDWORD CIPHERDES::SB5[64] =     { 0x00000100, 0x02080100, 0x02080000, 0x42000100,
                                  0x00080000, 0x00000100, 0x40000000, 0x02080000,
                                  0x40080100, 0x00080000, 0x02000100, 0x40080100,
                                  0x42000100, 0x42080000, 0x00080100, 0x40000000,
                                  0x02000000, 0x40080000, 0x40080000, 0x00000000,
                                  0x40000100, 0x42080100, 0x42080100, 0x02000100,
                                  0x42080000, 0x40000100, 0x00000000, 0x42000000,
                                  0x02080100, 0x02000000, 0x42000000, 0x00080100,
                                  0x00080000, 0x42000100, 0x00000100, 0x02000000,
                                  0x40000000, 0x02080000, 0x42000100, 0x40080100,
                                  0x02000100, 0x40000000, 0x42080000, 0x02080100,
                                  0x40080100, 0x00000100, 0x02000000, 0x42080000,
                                  0x42080100, 0x00080100, 0x42000000, 0x42080100,
                                  0x02080000, 0x00000000, 0x40080000, 0x42000000,
                                  0x00080100, 0x02000100, 0x40000100, 0x00080000,
                                  0x00000000, 0x40080000, 0x02080100, 0x40000100
                                };

XDWORD CIPHERDES::SB6[64] =     { 0x20000010, 0x20400000, 0x00004000, 0x20404010,
                                  0x20400000, 0x00000010, 0x20404010, 0x00400000,
                                  0x20004000, 0x00404010, 0x00400000, 0x20000010,
                                  0x00400010, 0x20004000, 0x20000000, 0x00004010,
                                  0x00000000, 0x00400010, 0x20004010, 0x00004000,
                                  0x00404000, 0x20004010, 0x00000010, 0x20400010,
                                  0x20400010, 0x00000000, 0x00404010, 0x20404000,
                                  0x00004010, 0x00404000, 0x20404000, 0x20000000,
                                  0x20004000, 0x00000010, 0x20400010, 0x00404000,
                                  0x20404010, 0x00400000, 0x00004010, 0x20000010,
                                  0x00400000, 0x20004000, 0x20000000, 0x00004010,
                                  0x20000010, 0x20404010, 0x00404000, 0x20400000,
                                  0x00404010, 0x20404000, 0x00000000, 0x20400010,
                                  0x00000010, 0x00004000, 0x20400000, 0x00404010,
                                  0x00004000, 0x00400010, 0x20004010, 0x00000000,
                                  0x20404000, 0x20000000, 0x00400010, 0x20004010
                                };

XDWORD CIPHERDES::SB7[64] =     { 0x00200000, 0x04200002, 0x04000802, 0x00000000,
                                  0x00000800, 0x04000802, 0x00200802, 0x04200800,
                                  0x04200802, 0x00200000, 0x00000000, 0x04000002,
                                  0x00000002, 0x04000000, 0x04200002, 0x00000802,
                                  0x04000800, 0x00200802, 0x00200002, 0x04000800,
                                  0x04000002, 0x04200000, 0x04200800, 0x00200002,
                                  0x04200000, 0x00000800, 0x00000802, 0x04200802,
                                  0x00200800, 0x00000002, 0x04000000, 0x00200800,
                                  0x04000000, 0x00200800, 0x00200000, 0x04000802,
                                  0x04000802, 0x04200002, 0x04200002, 0x00000002,
                                  0x00200002, 0x04000000, 0x04000800, 0x00200000,
                                  0x04200800, 0x00000802, 0x00200802, 0x04200800,
                                  0x00000802, 0x04000002, 0x04200802, 0x04200000,
                                  0x00200800, 0x00000000, 0x00000002, 0x04200802,
                                  0x00000000, 0x00200802, 0x04200000, 0x00000800,
                                  0x04000002, 0x04000800, 0x00000800, 0x00200002
                                };

XDWORD CIPHERDES::SB8[64] =     { 0x10001040, 0x00001000, 0x00040000, 0x10041040,
                                  0x10000000, 0x10001040, 0x00000040, 0x10000000,
                                  0x00040040, 0x10040000, 0x10041040, 0x00041000,
                                  0x10041000, 0x00041040, 0x00001000, 0x00000040,
                                  0x10040000, 0x10000040, 0x10001000, 0x00001040,
                                  0x00041000, 0x00040040, 0x10040040, 0x10041000,
                                  0x00001040, 0x00000000, 0x00000000, 0x10040040,
                                  0x10000040, 0x10001000, 0x00041040, 0x00040000,
                                  0x00041040, 0x00040000, 0x10041000, 0x00001000,
                                  0x00000040, 0x10040040, 0x00001000, 0x00041040,
                                  0x10001000, 0x00000040, 0x10000040, 0x10040000,
                                  0x10040040, 0x10000000, 0x00040000, 0x10001040,
                                  0x00000000, 0x10041040, 0x00040040, 0x10000040,
                                  0x10040000, 0x10001000, 0x10001040, 0x00000000,
                                  0x10041040, 0x00041000, 0x00041000, 0x00001040,
                                  0x00001040, 0x00040040, 0x10000000, 0x10041000
                                };


// PC1: left and right halves bit-swap

XDWORD CIPHERDES::LHs[16] =     { 0x00000000, 0x00000001, 0x00000100, 0x00000101,
                                  0x00010000, 0x00010001, 0x00010100, 0x00010101,
                                  0x01000000, 0x01000001, 0x01000100, 0x01000101,
                                  0x01010000, 0x01010001, 0x01010100, 0x01010101
                                };

XDWORD CIPHERDES::RHs[16] =     { 0x00000000, 0x01000000, 0x00010000, 0x01010000,
                                  0x00000100, 0x01000100, 0x00010100, 0x01010100,
                                  0x00000001, 0x01000001, 0x00010001, 0x01010001,
                                  0x00000101, 0x01000101, 0x00010101, 0x01010101,
                                };


XBYTE CIPHERDES::oddparitytable[128] =      {   1,   2,   4,   7,   8,  11,  13,  14,  16,  19,  21,  22,  25,  26,  28,  31,
                                               32,  35,  37,  38,  41,  42,  44,  47,  49,  50,  52,  55,  56,  59,  61,  62,
                                               64,  67,  69,  70,  73,  74,  76,  79,  81,  82,  84,  87,  88,  91,  93,  94,
                                               97,  98, 100, 103, 104, 107, 109, 110, 112, 115, 117, 118, 121, 122, 124, 127,
                                              128, 131, 133, 134, 137, 138, 140, 143, 145, 146, 148, 151, 152, 155, 157, 158,
                                              161, 162, 164, 167, 168, 171, 173, 174, 176, 179, 181, 182, 185, 186, 188, 191,
                                              193, 194, 196, 199, 200, 203, 205, 206, 208, 211, 213, 214, 217, 218, 220, 223,
                                              224, 227, 229, 230, 233, 234, 236, 239, 241, 242, 244, 247, 248, 251, 253, 254
                                            };



XBYTE CIPHERDES::weakkeytable[CIPHERDES_WEAKKEYCOUNT][CIPHERDES_KEYSIZE] =    { { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
                                                                                { 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE },
                                                                                { 0x1F, 0x1F, 0x1F, 0x1F, 0x0E, 0x0E, 0x0E, 0x0E },
                                                                                { 0xE0, 0xE0, 0xE0, 0xE0, 0xF1, 0xF1, 0xF1, 0xF1 },

                                                                                { 0x01, 0x1F, 0x01, 0x1F, 0x01, 0x0E, 0x01, 0x0E },
                                                                                { 0x1F, 0x01, 0x1F, 0x01, 0x0E, 0x01, 0x0E, 0x01 },
                                                                                { 0x01, 0xE0, 0x01, 0xE0, 0x01, 0xF1, 0x01, 0xF1 },
                                                                                { 0xE0, 0x01, 0xE0, 0x01, 0xF1, 0x01, 0xF1, 0x01 },
                                                                                { 0x01, 0xFE, 0x01, 0xFE, 0x01, 0xFE, 0x01, 0xFE },
                                                                                { 0xFE, 0x01, 0xFE, 0x01, 0xFE, 0x01, 0xFE, 0x01 },
                                                                                { 0x1F, 0xE0, 0x1F, 0xE0, 0x0E, 0xF1, 0x0E, 0xF1 },
                                                                                { 0xE0, 0x1F, 0xE0, 0x1F, 0xF1, 0x0E, 0xF1, 0x0E },
                                                                                { 0x1F, 0xFE, 0x1F, 0xFE, 0x0E, 0xFE, 0x0E, 0xFE },
                                                                                { 0xFE, 0x1F, 0xFE, 0x1F, 0xFE, 0x0E, 0xFE, 0x0E },
                                                                                { 0xE0, 0xFE, 0xE0, 0xFE, 0xF1, 0xFE, 0xF1, 0xFE },
                                                                                { 0xFE, 0xE0, 0xFE, 0xE0, 0xFE, 0xF1, 0xFE, 0xF1 }
                                                                              };




/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHERDES::CIPHERDES()
* @brief      Constructor of class
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERDES::CIPHERDES()
{
  Clean();

  type              = CIPHERTYPE_DES;
  paddingadjustsize = 8;

  SetChainingMode(CIPHERCHAININGMODE_ECB);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHERDES::~CIPHERDES()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERDES::~CIPHERDES()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERDES::Cipher(XBYTE* input, XDWORD size)
* @brief      Cipher
* @ingroup    CIPHER
* 
* @param[in]  input : Input pointer to use.
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERDES::Cipher(XBYTE* input, XDWORD size)
{
  if(!size) return false;

  XBUFFER            inputpadding;
  CIPHERDES_CONTEXT  ctx;
  XBYTE              iv[8];

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

  DESSetKeyCipher(&ctx, (XBYTE *)key->Get()->Get());

  bool status;

  if(this->GetChainingMode() == CIPHERCHAININGMODE_ECB)
         status = DESCipher_ECB(&ctx, CIPHERDES_ENCRYPT, result->GetSize(), iv, inputpadding.Get(), result->Get());
    else status = DESCipher_CBC(&ctx, CIPHERDES_ENCRYPT, result->GetSize(), iv, inputpadding.Get(), result->Get());

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERDES::Uncipher(XBYTE* input, XDWORD size)
* @brief      Uncipher
* @ingroup    CIPHER
* 
* @param[in]  input : Input pointer to use.
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERDES::Uncipher(XBYTE* input, XDWORD size)
{
  if(!size) return false;

  CIPHERDES_CONTEXT  ctx;
  XBYTE              iv[8];

  memset(iv, 0 , 8);
  if(inivector && inivector->GetSize()>=8) memcpy(iv, inivector->Get(), 8);

  result->Delete();
  result->Add(input, size);

  CIPHERKEYSYMMETRICAL* key = (CIPHERKEYSYMMETRICAL*)GetKey();
  if(!key)               return false;
  if(!key->Get()->Get()) return false;

  DESSetKeyUncipher(&ctx, (XBYTE *)key->Get()->Get());

  bool status;

  if(this->GetChainingMode() == CIPHERCHAININGMODE_ECB)
         status = DESCipher_ECB(&ctx, CIPHERDES_DECRYPT, size, iv, input, result->Get());
    else status = DESCipher_CBC(&ctx, CIPHERDES_DECRYPT, size, iv, input, result->Get());

  result->Padding_Delete();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void CIPHERDES::DESKeySetParity(XBYTE key[CIPHERDES_KEYSIZE])
* @brief      DES key set parity
* @ingroup    CIPHER
* 
* @param[in]  key : Key value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void CIPHERDES::DESKeySetParity(XBYTE key[CIPHERDES_KEYSIZE])
{
  for(int c = 0; c<CIPHERDES_KEYSIZE; c++)
    {
      key[c] = oddparitytable[key[c] / 2];
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int CIPHERDES::DESKeyCheckKeyParity(XBYTE key[CIPHERDES_KEYSIZE])
* @brief      DES key check key parity
* @ingroup    CIPHER
* 
* @param[in]  key : Key value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int CIPHERDES::DESKeyCheckKeyParity(XBYTE key[CIPHERDES_KEYSIZE])
{
  for(int c=0; c<CIPHERDES_KEYSIZE; c++)
    {
      if(key[c] != oddparitytable[key[c] / 2]) return 1;
    }

  return 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int CIPHERDES::DESKeyCheckWeak(XBYTE key[CIPHERDES_KEYSIZE])
* @brief      DES key check weak
* @ingroup    CIPHER
* 
* @param[in]  key : Key value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int CIPHERDES::DESKeyCheckWeak(XBYTE key[CIPHERDES_KEYSIZE])
{
  for(int c=0; c<CIPHERDES_WEAKKEYCOUNT; c++)
    {
      if(memcmp(weakkeytable[c], key, CIPHERDES_KEYSIZE) == 0)  return 1;
    }

  return 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void CIPHERDES::DESSetKey(XDWORD SK[32], XBYTE key[CIPHERDES_KEYSIZE])
* @brief      DES set key
* @ingroup    CIPHER
* 
* @param[in]  SK : SK value.
* @param[in]  key : Key value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void CIPHERDES::DESSetKey(XDWORD SK[32], XBYTE key[CIPHERDES_KEYSIZE])
{
  XDWORD X;
  XDWORD Y;
  XDWORD T;

  GET_XDWORD_BE(X, key, 0);
  GET_XDWORD_BE(Y, key, 4);

  T = ((Y >>  4) ^ X) & 0x0F0F0F0F;  X ^= T; Y ^= (T <<  4);
  T = ((Y      ) ^ X) & 0x10101010;  X ^= T; Y ^= (T      );

  X = (LHs[ (X      ) & 0xF] << 3) | (LHs[ (X >>  8) & 0xF ] << 2)
        | (LHs[ (X >> 16) & 0xF] << 1) | (LHs[ (X >> 24) & 0xF ]     )
        | (LHs[ (X >>  5) & 0xF] << 7) | (LHs[ (X >> 13) & 0xF ] << 6)
        | (LHs[ (X >> 21) & 0xF] << 5) | (LHs[ (X >> 29) & 0xF ] << 4);

  Y = (RHs[ (Y >>  1) & 0xF] << 3) | (RHs[ (Y >>  9) & 0xF ] << 2)
        | (RHs[ (Y >> 17) & 0xF] << 1) | (RHs[ (Y >> 25) & 0xF ]     )
        | (RHs[ (Y >>  4) & 0xF] << 7) | (RHs[ (Y >> 12) & 0xF ] << 6)
        | (RHs[ (Y >> 20) & 0xF] << 5) | (RHs[ (Y >> 28) & 0xF ] << 4);

  X &= 0x0FFFFFFF;
  Y &= 0x0FFFFFFF;

  for(int c=0; c<16; c++ )
    {
      if(c<2 || c==8 || c==15)
        {
           X = ((X <<  1) | (X >> 27)) & 0x0FFFFFFF;
           Y = ((Y <<  1) | (Y >> 27)) & 0x0FFFFFFF;
        }
       else
        {
           X = ((X <<  2) | (X >> 26)) & 0x0FFFFFFF;
           Y = ((Y <<  2) | (Y >> 26)) & 0x0FFFFFFF;
        }

      *SK++ =   ((X <<  4) & 0x24000000) | ((X << 28) & 0x10000000)
              | ((X << 14) & 0x08000000) | ((X << 18) & 0x02080000)
              | ((X <<  6) & 0x01000000) | ((X <<  9) & 0x00200000)
              | ((X >>  1) & 0x00100000) | ((X << 10) & 0x00040000)
              | ((X <<  2) & 0x00020000) | ((X >> 10) & 0x00010000)
              | ((Y >> 13) & 0x00002000) | ((Y >>  4) & 0x00001000)
              | ((Y <<  6) & 0x00000800) | ((Y >>  1) & 0x00000400)
              | ((Y >> 14) & 0x00000200) | ((Y      ) & 0x00000100)
              | ((Y >>  5) & 0x00000020) | ((Y >> 10) & 0x00000010)
              | ((Y >>  3) & 0x00000008) | ((Y >> 18) & 0x00000004)
              | ((Y >> 26) & 0x00000002) | ((Y >> 24) & 0x00000001);

      *SK++ =   ((X << 15) & 0x20000000) | ((X << 17) & 0x10000000)
              | ((X << 10) & 0x08000000) | ((X << 22) & 0x04000000)
              | ((X >>  2) & 0x02000000) | ((X <<  1) & 0x01000000)
              | ((X << 16) & 0x00200000) | ((X << 11) & 0x00100000)
              | ((X <<  3) & 0x00080000) | ((X >>  6) & 0x00040000)
              | ((X << 15) & 0x00020000) | ((X >>  4) & 0x00010000)
              | ((Y >>  2) & 0x00002000) | ((Y <<  8) & 0x00001000)
              | ((Y >> 14) & 0x00000808) | ((Y >>  9) & 0x00000400)
              | ((Y      ) & 0x00000200) | ((Y <<  7) & 0x00000100)
              | ((Y >>  7) & 0x00000020) | ((Y >>  3) & 0x00000011)
              | ((Y <<  2) & 0x00000004) | ((Y >> 21) & 0x00000002);
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int CIPHERDES::DESSetKeyCipher(CIPHERDES_CONTEXT* ctx, XBYTE key[CIPHERDES_KEYSIZE])
* @brief      DES set key cipher
* @ingroup    CIPHER
* 
* @param[in]  ctx : Ctx pointer to use.
* @param[in]  key : Key value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int CIPHERDES::DESSetKeyCipher(CIPHERDES_CONTEXT* ctx, XBYTE key[CIPHERDES_KEYSIZE])
{
  DESSetKey(ctx->sk, key);

  return 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int CIPHERDES::DESSetKeyUncipher(CIPHERDES_CONTEXT* ctx, XBYTE key[CIPHERDES_KEYSIZE])
* @brief      DES set key uncipher
* @ingroup    CIPHER
* 
* @param[in]  ctx : Ctx pointer to use.
* @param[in]  key : Key value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int CIPHERDES::DESSetKeyUncipher(CIPHERDES_CONTEXT* ctx, XBYTE key[CIPHERDES_KEYSIZE])
{
  DESSetKey(ctx->sk, key);

  for(int c=0; c<16; c+=2)
    {
      SWAP(ctx->sk[c    ], ctx->sk[30 - c]);
      SWAP(ctx->sk[c + 1], ctx->sk[31 - c]);
    }

  return 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int CIPHERDES::DESCipher(CIPHERDES_CONTEXT* ctx, XBYTE input[8], XBYTE output[8])
* @brief      DES cipher
* @ingroup    CIPHER
* 
* @param[in]  ctx : Ctx pointer to use.
* @param[in]  input : Input value.
* @param[in]  output : Output output.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int CIPHERDES::DESCipher(CIPHERDES_CONTEXT* ctx, XBYTE input[8], XBYTE output[8])
{
  XDWORD  X;
  XDWORD  Y;
  XDWORD  T;
  XDWORD* SK;

  SK = ctx->sk;

  GET_XDWORD_BE(X, input, 0);
  GET_XDWORD_BE(Y, input, 4);

  DES_IP(X, Y);

  for(int c=0; c<8; c++ )
    {
      DES_ROUND(Y, X);
      DES_ROUND(X, Y);
    }

  DES_FP(Y, X);

  PUT_XDWORD_BE(Y, output, 0);
  PUT_XDWORD_BE(X, output, 4);

  return 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERDES::DESCipher_ECB(CIPHERDES_CONTEXT* ctx, int mode, size_t length, XBYTE iv[8], XBYTE* input, XBYTE* output)
* @brief      DES cipher ECB
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
bool CIPHERDES::DESCipher_ECB(CIPHERDES_CONTEXT* ctx, int mode, size_t length, XBYTE iv[8], XBYTE* input, XBYTE* output)
{
  XBYTE temp[8];

  if(length % 8) return false;

  if(mode==CIPHERDES_ENCRYPT)
    {
      while(length > 0)
        {
          DESCipher(ctx, output, output);

          memcpy(iv, output, 8);

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

          DESCipher( ctx, input, output);

          memcpy(iv, temp, 8);

          input  += 8;
          output += 8;
          length -= 8;
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERDES::DESCipher_CBC(CIPHERDES_CONTEXT* ctx, int mode, size_t length, XBYTE iv[8], XBYTE* input, XBYTE* output)
* @brief      DES cipher CBC
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
bool CIPHERDES::DESCipher_CBC(CIPHERDES_CONTEXT* ctx, int mode, size_t length, XBYTE iv[8], XBYTE* input, XBYTE* output)
{
  XBYTE temp[8];

  if(length % 8) return false;

  if(mode==CIPHERDES_ENCRYPT)
    {
      while(length > 0)
        {

          for(int c=0; c<8; c++ )
            {
              output[c] = (XBYTE)( input[c] ^ iv[c]);
            }

          DESCipher(ctx, output, output);

          memcpy(iv, output, 8);

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

          DESCipher( ctx, input, output);

          for(int c=0; c<8; c++)
            {
              output[c] = (XBYTE)( output[c] ^ iv[c]);
            }

          memcpy(iv, temp, 8);

          input  += 8;
          output += 8;
          length -= 8;
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void CIPHERDES::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
void CIPHERDES::Clean()
{

}



