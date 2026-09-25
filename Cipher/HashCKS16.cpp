/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       HashCKS16.cpp
* 
* @class      HASHCKS16
* @brief      Hash CKS16 class
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

#include "HashCKS16.h"

#include "XBuffer.h"
#include "XFactory.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         HASHCKS16::HASHCKS16() : HASH()
* @brief      Constructor of class
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
HASHCKS16::HASHCKS16() : HASH()
{
  Clean();

  type = HASHTYPE_CKS16;

  resulttmp = GEN_NEW XBUFFER();

  XWORD value = 0x0000;
  resulttmp->Add((XWORD)value);

  Ini();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         HASHCKS16::~HASHCKS16()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
HASHCKS16::~HASHCKS16()
{
  if(resulttmp)
    {
      GEN_DELETE resulttmp;
      resulttmp = NULL;
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool HASHCKS16::Do(XBYTE* input, XQWORD size)
* @brief      Execute the operation
* @ingroup    CIPHER
* 
* @param[in]  input : Input pointer to use.
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool HASHCKS16::Do(XBYTE* input, XQWORD size)
{
  if(!input) return false;
  if(!size)  return false;

  XWORD value = 0;

  resulttmp->Extract(value);

  // Accumulate 16-bit words. Build each word from bytes (no *(XWORD*) cast):
  // - avoids unaligned access UB
  // - matches little-endian *(XWORD*) on Windows/intel
  // Previous loop ran `size` times while advancing `sizeof(XWORD)` each step, so a
  // 3-byte buffer was walked 6 bytes off the end — OOB reads that collided on
  // intel64 Debug stack padding (DifferentInputsLikelyDifferentChecksum).
  while(size > 1)
    {
      XWORD word = (XWORD)input[0] | (((XWORD)input[1]) << 8);
      value = Update(value, word);
      input += 2;
      size  -= 2;
    }

  // Classic 16-bit checksum: odd trailing byte in the low half of the word.
  if(size == 1)
    {
      value = Update(value, (XWORD)input[0]);
    }

  value = ~value;

  resulttmp->Add((XWORD)value);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool HASHCKS16::ResetResult()
* @brief      Reset result
* @ingroup    CIPHER
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool HASHCKS16::ResetResult()
{
  HASH::ResetResult();

  resulttmp->Delete();

  XWORD value = 0x0000;
  resulttmp->Add((XWORD)value);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int HASHCKS16::GetDefaultSize()
* @brief      Get default size
* @ingroup    CIPHER
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int HASHCKS16::GetDefaultSize()
{
  return sizeof(XWORD);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBUFFER* HASHCKS16::GetResult()
* @brief      Get result
* @ingroup    CIPHER
* 
* @return     XBUFFER* : Pointer to the requested buffer; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* HASHCKS16::GetResult()
{
  GetResultCKS16();

  return result;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE* HASHCKS16::GetResult(XDWORD& resultsize)
* @brief      Get result
* @ingroup    CIPHER
* 
* @param[in]  resultsize : Output resultsize.
* 
* @return     XBYTE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE* HASHCKS16::GetResult(XDWORD& resultsize)
{
  GetResultCKS16();

  resultsize = result->GetSize();

  return result->Get();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORD HASHCKS16::GetResultCKS16()
* @brief      Get result CKS16
* @ingroup    CIPHER
* 
* @return     XWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORD HASHCKS16::GetResultCKS16()
{
  XWORD value = 0;

  result->Delete();

  resulttmp->Extract(value);

  value ^= 0x0000;

  result->Add((XWORD)value);

  return value;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void HASHCKS16::Ini(void)
* @brief      Initialize the object
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
void HASHCKS16::Ini(void)
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORD HASHCKS16::Update(XWORD CKS16, XWORD c)
* @brief      Update
* @ingroup    CIPHER
* 
* @param[in]  CKS16 : CKS16 value.
* @param[in]  c : C value.
* 
* @return     XWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORD HASHCKS16::Update(XWORD CKS16, XWORD c)
{
  XDWORD tmp = (XDWORD)CKS16 + (XDWORD)c;

  // loop while we have a carry
  while(tmp > 0xFFFF)
    {
      XWORD carry = (tmp >> 16) & (0xFFFF);
      tmp = tmp + carry;
    }

  return (XWORD)tmp;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void HASHCKS16::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
void HASHCKS16::Clean()
{
  resulttmp = NULL;
}



