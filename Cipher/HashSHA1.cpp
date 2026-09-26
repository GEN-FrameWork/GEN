/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       HashSHA1.cpp
* 
* @class      HASHSHA1
* @brief      Hash SHA1 class
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

#include "HashSHA1.h"

#include <string.h>

#include "XBuffer.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         HASHSHA1::HASHSHA1() : HASH()
* @brief      Constructor of class
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
HASHSHA1::HASHSHA1() : HASH()
{
  Clean();

  type = HASHTYPE_SHA1;
  
  Reset();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         HASHSHA1::~HASHSHA1()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
HASHSHA1::~HASHSHA1()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool HASHSHA1::Do(XBYTE* input, XQWORD size)
* @brief      Execute the operation
* @ingroup    CIPHER
* 
* @param[in]  input : Input pointer to use.
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool HASHSHA1::Do(XBYTE* input, XQWORD size)
{
  XDWORD msgdigest[5] = { 0, 0, 0, 0, 0 };

  Reset();

  result->Delete();

  Input(input, size);

  Result(msgdigest);

  for(int c=0;c<5;c++)
    {
      result->Add((XDWORD)msgdigest[c]);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int HASHSHA1::GetDefaultSize()
* @brief      Get default size
* @ingroup    CIPHER
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int HASHSHA1::GetDefaultSize()
{
  return sizeof(XDWORD)*5;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void HASHSHA1::Reset()
* @brief      Reset
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
void HASHSHA1::Reset()
{
  lengthlow     = 0;
  lengthhigh    = 0;
  msgblockindex = 0;

  H[0]          = 0x67452301;
  H[1]          = 0xEFCDAB89;
  H[2]          = 0x98BADCFE;
  H[3]          = 0x10325476;
  H[4]          = 0xC3D2E1F0;

  computed      = false;
  corrupted     = false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void HASHSHA1::Input(XBYTE* input, XQWORD length)
* @brief      Input
* @ingroup    CIPHER
* 
* @param[in]  input : Input pointer to use.
* @param[in]  length : Length value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void HASHSHA1::Input(XBYTE* input, XQWORD length)
{
  if(!length) return;

  if(computed || corrupted)
    {
      corrupted = true;
      return;
    }

  while(length-- && !corrupted)
    {
      if(msgblockindex < 64) msgblock[msgblockindex++] = (*input & 0xFF);

      lengthlow += 8;
      lengthlow &= 0xFFFFFFFF;                            // Force it to 32 bits

      if(lengthlow == 0)
        {
            lengthhigh++;
            lengthhigh &= 0xFFFFFFFF;                    // Force it to 32 bits

            if(lengthhigh == 0) corrupted = true;        // Message is too long
        }

       if(msgblockindex == 64) ProcessMessageBlock();

       input++;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void HASHSHA1::ProcessMessageBlock()
* @brief      Process message block
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
void HASHSHA1::ProcessMessageBlock()
{
  XDWORD  K[] = { 0x5A827999 ,
                  0x6ED9EBA1 ,
                  0x8F1BBCDC ,
                  0xCA62C1D6
                };
  int     t;                          // Loop counter
  XDWORD  temp;                       // Temporary word value
  XDWORD  W[80];                      // Word sequence
  XDWORD  A;
  XDWORD  B;
  XDWORD  C;
  XDWORD  D;
  XDWORD  E;                          // Word buffers

  for(t=0;t<16;t++)
    {
      W[t]  = ((unsigned) msgblock[t * 4])     << 24;
      W[t] |= ((unsigned) msgblock[t * 4 + 1]) << 16;
      W[t] |= ((unsigned) msgblock[t * 4 + 2]) << 8;
      W[t] |= ((unsigned) msgblock[t * 4 + 3]);
    }

  for(t=16;t<80;t++)
    {
      W[t] = CircularShift(1, W[t-3] ^ W[t-8] ^ W[t-14] ^ W[t-16]);
    }

  A = H[0];
  B = H[1];
  C = H[2];
  D = H[3];
  E = H[4];

  for(t=0;t<20;t++)
    {
      temp  = CircularShift(5, A) + ((B & C) | ((~B) & D)) + E + W[t] + K[0];
      temp &= 0xFFFFFFFF;
      E     = D;
      D     = C;
      C     = CircularShift(30, B);
      B     = A;
      A     = temp;
    }

  for(t=20;t<40;t++)
    {
      temp = CircularShift(5, A) + (B ^ C ^ D) + E + W[t] + K[1];
      temp &= 0xFFFFFFFF;
      E     = D;
      D     = C;
      C     = CircularShift(30, B);
      B     = A;
      A     = temp;
    }

  for(t=40;t<60;t++)
    {
      temp  = CircularShift(5, A) + ((B & C) | (B & D) | (C & D)) + E + W[t] + K[2];
      temp &= 0xFFFFFFFF;
      E     = D;
      D     = C;
      C     = CircularShift(30, B);
      B     = A;
      A     = temp;
    }

  for(t=60;t<80;t++)
    {
      temp  = CircularShift(5, A) + (B ^ C ^ D) + E + W[t] + K[3];
      temp &= 0xFFFFFFFF;
      E     = D;
      D     = C;
      C     = CircularShift(30, B);
      B     = A;
      A     = temp;
    }

  H[0] = (H[0] + A) & 0xFFFFFFFF;
  H[1] = (H[1] + B) & 0xFFFFFFFF;
  H[2] = (H[2] + C) & 0xFFFFFFFF;
  H[3] = (H[3] + D) & 0xFFFFFFFF;
  H[4] = (H[4] + E) & 0xFFFFFFFF;

  msgblockindex = 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void HASHSHA1::PadMessage()
* @brief      Pad message
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
void HASHSHA1::PadMessage()
{
  // Check to see if the current message block is too small to hold
  // the initial padding bits and length.  If so, we will pad the
  // block, process it, and then continue padding into a second block.

  if(msgblockindex > 55)
    {
      msgblock[msgblockindex++] = 0x80;

      while(msgblockindex < 64)
        {
          msgblock[msgblockindex++] = 0;
        }

      ProcessMessageBlock();

      while(msgblockindex < 56)
        {
          msgblock[msgblockindex++] = 0;
        }
    }
   else
    {
      msgblock[msgblockindex++] = 0x80;

      while(msgblockindex < 56)
        {
          msgblock[msgblockindex++] = 0;
        }

    }


  // Store the message length as the last 8 octets

  msgblock[56] = (lengthhigh >> 24) & 0xFF;
  msgblock[57] = (lengthhigh >> 16) & 0xFF;
  msgblock[58] = (lengthhigh >> 8) & 0xFF;
  msgblock[59] = (lengthhigh) & 0xFF;

  msgblock[60] = (lengthlow >> 24) & 0xFF;
  msgblock[61] = (lengthlow >> 16) & 0xFF;
  msgblock[62] = (lengthlow >> 8) & 0xFF;
  msgblock[63] = (lengthlow) & 0xFF;

  ProcessMessageBlock();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD HASHSHA1::CircularShift(int bits, XDWORD word)
* @brief      Circular shift
* @ingroup    CIPHER
* 
* @param[in]  bits : Bits value.
* @param[in]  word : Word value.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD HASHSHA1::CircularShift(int bits, XDWORD word)
{
  return ((word << bits) & 0xFFFFFFFF) | ((word & 0xFFFFFFFF) >> (32-bits));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool HASHSHA1::Result(XDWORD* msgdigest)
* @brief      Result
* @ingroup    CIPHER
* 
* @param[in]  msgdigest : Msgdigest pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool HASHSHA1::Result(XDWORD* msgdigest)
{
  if(corrupted) return false;

  if(!computed)
    {
      PadMessage();
      computed = true;
    }

  for(int i=0; i<5; i++)
    {
       msgdigest[i] = H[i];
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void HASHSHA1::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
void HASHSHA1::Clean()
{

}


