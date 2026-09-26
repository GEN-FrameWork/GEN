/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       HashMD5.cpp
* 
* @class      HASHMD5
* @brief      Hash MD5 class
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

#include "HashMD5.h"

#include "XBuffer.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

#define S11   7
#define S12   12
#define S13   17
#define S14   22
#define S21   5
#define S22   9
#define S23   14
#define S24   20
#define S31   4
#define S32   11
#define S33   16
#define S34   23
#define S41   6
#define S42   10
#define S43   15
#define S44   21



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         HASHMD5::HASHMD5() : HASH()
* @brief      Constructor of class
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
HASHMD5::HASHMD5() : HASH()
{
  Clean();

  type = HASHTYPE_MD5;

  Ini();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         HASHMD5::~HASHMD5()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
HASHMD5::~HASHMD5()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool HASHMD5::Do(XBYTE* input, XQWORD size)
* @brief      Execute the operation
* @ingroup    CIPHER
* 
* @param[in]  input : Input pointer to use.
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool HASHMD5::Do(XBYTE* input, XQWORD size)
{
  if(!result) return false;

  result->Delete();

  Ini();

  Update(input, size);

  End();

  result->Add(digest, 16);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int HASHMD5::GetDefaultSize()
* @brief      Get default size
* @ingroup    CIPHER
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int HASHMD5::GetDefaultSize()
{
  return 16;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void HASHMD5::Ini()
* @brief      Initialize the object
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
void HASHMD5::Ini()
{
  finalized=false;

  count[0] = 0;
  count[1] = 0;

  // load magic initialization constants.
  state[0] = 0x67452301;
  state[1] = 0xefcdab89;
  state[2] = 0x98badcfe;
  state[3] = 0x10325476;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void HASHMD5::Update(XBYTE* input, XQWORD size)
* @brief      Update
* @ingroup    CIPHER
* 
* @param[in]  input : Input pointer to use.
* @param[in]  size : Size value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void HASHMD5::Update(XBYTE* input, XQWORD size)
{
  // compute number of bytes mod 64
  XDWORD index = count[0] / 8 % HASHMD5_BLOCKSIZE;

  // Update number of bits
  if ((count[0] += (XDWORD)(size << 3)) < (XDWORD)(size << 3)) count[1]++;

  count[1] += (XDWORD)(size >> 29);

  // number of bytes we need to fill in buffer
  XDWORD firstpart = 64 - index;

  XDWORD i;

  // transform as many times as possible.
  if(size >= firstpart)
    {
      // fill buffer first, transform
      memcpy(&buffer[index], input, firstpart);

      Transform(buffer);

    // transform chunks of HASHMD5_BLOCKSIZE (64 bytes)
    for(i = firstpart; i + HASHMD5_BLOCKSIZE <= (XDWORD)size; i += HASHMD5_BLOCKSIZE)
      {
        Transform(&input[i]);
      }

    index = 0;

  } else i = 0;

  // buffer remaining input
  memcpy(&buffer[index], &input[i], (XDWORD)(size-i));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void HASHMD5::Transform(XBYTE block[HASHMD5_BLOCKSIZE])
* @brief      Transform
* @ingroup    CIPHER
* 
* @param[in]  block : Block value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void HASHMD5::Transform(XBYTE block[HASHMD5_BLOCKSIZE])
{
  XDWORD a = state[0];
  XDWORD b = state[1];
  XDWORD c = state[2];
  XDWORD d = state[3];
  XDWORD x[16];

  Decode(x, block, HASHMD5_BLOCKSIZE);

  // Round 1 
  FF (a, b, c, d, x[ 0], S11, 0xd76aa478); // 1 
  FF (d, a, b, c, x[ 1], S12, 0xe8c7b756); // 2 
  FF (c, d, a, b, x[ 2], S13, 0x242070db); // 3 
  FF (b, c, d, a, x[ 3], S14, 0xc1bdceee); // 4 
  FF (a, b, c, d, x[ 4], S11, 0xf57c0faf); // 5 
  FF (d, a, b, c, x[ 5], S12, 0x4787c62a); // 6 
  FF (c, d, a, b, x[ 6], S13, 0xa8304613); // 7 
  FF (b, c, d, a, x[ 7], S14, 0xfd469501); // 8 
  FF (a, b, c, d, x[ 8], S11, 0x698098d8); // 9 
  FF (d, a, b, c, x[ 9], S12, 0x8b44f7af); // 10 
  FF (c, d, a, b, x[10], S13, 0xffff5bb1); // 11 
  FF (b, c, d, a, x[11], S14, 0x895cd7be); // 12 
  FF (a, b, c, d, x[12], S11, 0x6b901122); // 13 
  FF (d, a, b, c, x[13], S12, 0xfd987193); // 14 
  FF (c, d, a, b, x[14], S13, 0xa679438e); // 15 
  FF (b, c, d, a, x[15], S14, 0x49b40821); // 16 

  // Round 2 
  GG (a, b, c, d, x[ 1], S21, 0xf61e2562); // 17 
  GG (d, a, b, c, x[ 6], S22, 0xc040b340); // 18 
  GG (c, d, a, b, x[11], S23, 0x265e5a51); // 19 
  GG (b, c, d, a, x[ 0], S24, 0xe9b6c7aa); // 20 
  GG (a, b, c, d, x[ 5], S21, 0xd62f105d); // 21 
  GG (d, a, b, c, x[10], S22, 0x2441453); // 22 
  GG (c, d, a, b, x[15], S23, 0xd8a1e681); // 23 
  GG (b, c, d, a, x[ 4], S24, 0xe7d3fbc8); // 24 
  GG (a, b, c, d, x[ 9], S21, 0x21e1cde6); // 25 
  GG (d, a, b, c, x[14], S22, 0xc33707d6); // 26 
  GG (c, d, a, b, x[ 3], S23, 0xf4d50d87); // 27 
  GG (b, c, d, a, x[ 8], S24, 0x455a14ed); // 28 
  GG (a, b, c, d, x[13], S21, 0xa9e3e905); // 29 
  GG (d, a, b, c, x[ 2], S22, 0xfcefa3f8); // 30 
  GG (c, d, a, b, x[ 7], S23, 0x676f02d9); // 31 
  GG (b, c, d, a, x[12], S24, 0x8d2a4c8a); // 32 

  // Round 3 
  HH (a, b, c, d, x[ 5], S31, 0xfffa3942); // 33 
  HH (d, a, b, c, x[ 8], S32, 0x8771f681); // 34 
  HH (c, d, a, b, x[11], S33, 0x6d9d6122); // 35 
  HH (b, c, d, a, x[14], S34, 0xfde5380c); // 36 
  HH (a, b, c, d, x[ 1], S31, 0xa4beea44); // 37 
  HH (d, a, b, c, x[ 4], S32, 0x4bdecfa9); // 38 
  HH (c, d, a, b, x[ 7], S33, 0xf6bb4b60); // 39 
  HH (b, c, d, a, x[10], S34, 0xbebfbc70); // 40 
  HH (a, b, c, d, x[13], S31, 0x289b7ec6); // 41 
  HH (d, a, b, c, x[ 0], S32, 0xeaa127fa); // 42 
  HH (c, d, a, b, x[ 3], S33, 0xd4ef3085); // 43 
  HH (b, c, d, a, x[ 6], S34, 0x4881d05); // 44 
  HH (a, b, c, d, x[ 9], S31, 0xd9d4d039); // 45 
  HH (d, a, b, c, x[12], S32, 0xe6db99e5); // 46 
  HH (c, d, a, b, x[15], S33, 0x1fa27cf8); // 47 
  HH (b, c, d, a, x[ 2], S34, 0xc4ac5665); // 48 

  // Round 4 
  II (a, b, c, d, x[ 0], S41, 0xf4292244); // 49 
  II (d, a, b, c, x[ 7], S42, 0x432aff97); // 50 
  II (c, d, a, b, x[14], S43, 0xab9423a7); // 51 
  II (b, c, d, a, x[ 5], S44, 0xfc93a039); // 52 
  II (a, b, c, d, x[12], S41, 0x655b59c3); // 53 
  II (d, a, b, c, x[ 3], S42, 0x8f0ccc92); // 54 
  II (c, d, a, b, x[10], S43, 0xffeff47d); // 55 
  II (b, c, d, a, x[ 1], S44, 0x85845dd1); // 56 
  II (a, b, c, d, x[ 8], S41, 0x6fa87e4f); // 57 
  II (d, a, b, c, x[15], S42, 0xfe2ce6e0); // 58 
  II (c, d, a, b, x[ 6], S43, 0xa3014314); // 59 
  II (b, c, d, a, x[13], S44, 0x4e0811a1); // 60 
  II (a, b, c, d, x[ 4], S41, 0xf7537e82); // 61 
  II (d, a, b, c, x[11], S42, 0xbd3af235); // 62 
  II (c, d, a, b, x[ 2], S43, 0x2ad7d2bb); // 63 
  II (b, c, d, a, x[ 9], S44, 0xeb86d391); // 64 

  state[0] += a;
  state[1] += b;
  state[2] += c;
  state[3] += d;

  // Zeroize sensitive information.
  memset(x, 0, sizeof(x));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void HASHMD5::Decode(XDWORD* output, XBYTE* input, XDWORD size)
* @brief      Decode
* @ingroup    CIPHER
* 
* @param[in]  output : Output output.
* @param[in]  input : Input pointer to use.
* @param[in]  size : Size value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void HASHMD5::Decode(XDWORD* output, XBYTE* input, XDWORD size)
{
  for(XDWORD i=0, j=0; j<size; i++, j+=4)
    {
      output[i] = ((XDWORD)input[j]) | (((XDWORD)input[j+1]) << 8) | (((XDWORD)input[j+2]) << 16) | (((XDWORD)input[j+3]) << 24);
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void HASHMD5::Encode(XBYTE* output, XDWORD* input, XDWORD size)
* @brief      Encode
* @ingroup    CIPHER
* 
* @param[in]  output : Output output.
* @param[in]  input : Input pointer to use.
* @param[in]  size : Size value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void HASHMD5::Encode(XBYTE* output, XDWORD* input, XDWORD size)
{
  for(XDWORD i=0, j=0; j<size; i++, j+= 4)
    {
      output[j] = input[i] & 0xff;
      output[j+1] = (input[i] >> 8) & 0xff;
      output[j+2] = (input[i] >> 16) & 0xff;
      output[j+3] = (input[i] >> 24) & 0xff;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void HASHMD5::End()
* @brief      End the object
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
void HASHMD5::End()
{
  unsigned char padding[64] = { 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                              };

  if(finalized) return;

    // Save number of bits
  XBYTE bits[8];

  Encode(bits, count, 8);

    // pad out to 56 mod 64.
  XDWORD index = count[0] / 8 % 64;
  XDWORD padLen = (index < 56) ? (56 - index) : (120 - index);

  Update(padding, padLen);

    // Append size (before padding)
  Update(bits, 8);

   // Store state in digest
  Encode(digest, state, 16);

  // Zeroize sensitive information.
  memset(buffer, 0, sizeof buffer);
  memset(count, 0, sizeof count);

  finalized=true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         inline XDWORD HASHMD5::F(XDWORD x, XDWORD y, XDWORD z)
* @brief      F
* @ingroup    CIPHER
* 
* @param[in]  x : X coordinate.
* @param[in]  y : Y coordinate.
* @param[in]  z : Z coordinate.
* 
* @return     inline XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
inline XDWORD HASHMD5::F(XDWORD x, XDWORD y, XDWORD z)
{
  return (x&y) | (~x&z);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         inline XDWORD HASHMD5::G(XDWORD x, XDWORD y, XDWORD z)
* @brief      G
* @ingroup    CIPHER
* 
* @param[in]  x : X coordinate.
* @param[in]  y : Y coordinate.
* @param[in]  z : Z coordinate.
* 
* @return     inline XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
inline XDWORD HASHMD5::G(XDWORD x, XDWORD y, XDWORD z)
{
  return (x&z) | (y&~z);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         inline XDWORD HASHMD5::H(XDWORD x, XDWORD y, XDWORD z)
* @brief      H
* @ingroup    CIPHER
* 
* @param[in]  x : X coordinate.
* @param[in]  y : Y coordinate.
* @param[in]  z : Z coordinate.
* 
* @return     inline XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
inline XDWORD HASHMD5::H(XDWORD x, XDWORD y, XDWORD z)
{
  return x^y^z;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         inline XDWORD HASHMD5::I(XDWORD x, XDWORD y, XDWORD z)
* @brief      I
* @ingroup    CIPHER
* 
* @param[in]  x : X coordinate.
* @param[in]  y : Y coordinate.
* @param[in]  z : Z coordinate.
* 
* @return     inline XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
inline XDWORD HASHMD5::I(XDWORD x, XDWORD y, XDWORD z)
{
  return y ^ (x | ~z);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         inline XDWORD HASHMD5::RotateLeft(XDWORD x, int n)
* @brief      Rotate left
* @ingroup    CIPHER
* 
* @param[in]  x : X coordinate.
* @param[in]  n : N value.
* 
* @return     inline XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
inline XDWORD HASHMD5::RotateLeft(XDWORD x, int n)
{
  return (x << n) | (x >> (32-n));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         inline void HASHMD5::FF(XDWORD& a, XDWORD b, XDWORD c, XDWORD d, XDWORD x, XDWORD s, XDWORD ac)
* @brief      FF
* @ingroup    CIPHER
* 
* @param[in]  a : A value.
* @param[in]  b : B value.
* @param[in]  c : C value.
* @param[in]  d : D value.
* @param[in]  x : X coordinate.
* @param[in]  s : S value.
* @param[in]  ac : Ac value.
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
inline void HASHMD5::FF(XDWORD& a, XDWORD b, XDWORD c, XDWORD d, XDWORD x, XDWORD s, XDWORD ac)
{
  a = RotateLeft(a+ F(b, c, d) + x + ac, s) + b;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         inline void HASHMD5::GG(XDWORD& a, XDWORD b, XDWORD c, XDWORD d, XDWORD x, XDWORD s, XDWORD ac)
* @brief      GG
* @ingroup    CIPHER
* 
* @param[in]  a : A value.
* @param[in]  b : B value.
* @param[in]  c : C value.
* @param[in]  d : D value.
* @param[in]  x : X coordinate.
* @param[in]  s : S value.
* @param[in]  ac : Ac value.
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
inline void HASHMD5::GG(XDWORD& a, XDWORD b, XDWORD c, XDWORD d, XDWORD x, XDWORD s, XDWORD ac)
{
  a = RotateLeft(a + G(b, c, d) + x + ac, s) + b;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         inline void HASHMD5::HH(XDWORD& a, XDWORD b, XDWORD c, XDWORD d, XDWORD x, XDWORD s, XDWORD ac)
* @brief      HH
* @ingroup    CIPHER
* 
* @param[in]  a : A value.
* @param[in]  b : B value.
* @param[in]  c : C value.
* @param[in]  d : D value.
* @param[in]  x : X coordinate.
* @param[in]  s : S value.
* @param[in]  ac : Ac value.
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
inline void HASHMD5::HH(XDWORD& a, XDWORD b, XDWORD c, XDWORD d, XDWORD x, XDWORD s, XDWORD ac)
{
  a = RotateLeft(a + H(b, c, d) + x + ac, s) + b;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         inline void HASHMD5::II(XDWORD& a, XDWORD b, XDWORD c, XDWORD d, XDWORD x, XDWORD s, XDWORD ac)
* @brief      II
* @ingroup    CIPHER
* 
* @param[in]  a : A value.
* @param[in]  b : B value.
* @param[in]  c : C value.
* @param[in]  d : D value.
* @param[in]  x : X coordinate.
* @param[in]  s : S value.
* @param[in]  ac : Ac value.
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
inline void HASHMD5::II(XDWORD& a, XDWORD b, XDWORD c, XDWORD d, XDWORD x, XDWORD s, XDWORD ac)
{
  a = RotateLeft(a + I(b, c, d) + x + ac, s) + b;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void HASHMD5::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
void HASHMD5::Clean()
{
  finalized=false;

  count[0] = 0;
  count[1] = 0;

  // load magic initialization constants.
  state[0] = 0x67452301;
  state[1] = 0xefcdab89;
  state[2] = 0x98badcfe;
  state[3] = 0x10325476;
}



