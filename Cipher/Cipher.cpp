/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       Cipher.cpp
* 
* @class      CIPHER
* @brief      Cipher interface class
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

#include "Cipher.h"

#include <string.h>

#include "XFactory.h"

#include "CipherKeySymmetrical.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHER::CIPHER()
* @brief      Constructor of class
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHER::CIPHER()
{
  Clean();

  type             = CIPHERTYPE_XOR;
  paddingadjustsize = 0;

  inivector = GEN_NEW XBUFFER();
  result    = GEN_NEW XBUFFER();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHER::~CIPHER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHER::~CIPHER()
{
  if(inivector)
    {
      GEN_DELETE inivector;
      inivector = NULL;
    }

  if(result)
    {
      GEN_DELETE result;
      result = NULL;
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHERTYPE CIPHER::GetType()
* @brief      Get type
* @ingroup    CIPHER
* 
* @return     CIPHERTYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERTYPE CIPHER::GetType()
{ 
  return type;                                      
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void CIPHER::SetType(CIPHERTYPE type)
* @brief      Set type
* @ingroup    CIPHER
* 
* @param[in]  type : Type value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void CIPHER::SetType(CIPHERTYPE type)
{ 
  this->type = type;                                
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHERCHAININGMODE CIPHER::GetChainingMode()
* @brief      Get chaining mode
* @ingroup    CIPHER
* 
* @return     CIPHERCHAININGMODE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERCHAININGMODE CIPHER::GetChainingMode()
{ 
  return chainingmode;                              
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void CIPHER::SetChainingMode(CIPHERCHAININGMODE chainingmode)
* @brief      Set chaining mode
* @ingroup    CIPHER
* 
* @param[in]  chainingmode : Chainingmode value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void CIPHER::SetChainingMode(CIPHERCHAININGMODE chainingmode)
{ 
  this->chainingmode = chainingmode;                
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBUFFER_PADDINGTYPE CIPHER::GetPaddingType()
* @brief      Get padding type
* @ingroup    CIPHER
* 
* @return     XBUFFER_PADDINGTYPE : Requested buffer.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER_PADDINGTYPE CIPHER::GetPaddingType()
{ 
  return paddingtype;                               
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void CIPHER::SetPaddingType(XBUFFER_PADDINGTYPE paddingtype)
* @brief      Set padding type
* @ingroup    CIPHER
* 
* @param[in]  paddingtype : Paddingtype value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void CIPHER::SetPaddingType(XBUFFER_PADDINGTYPE paddingtype)
{ 
  this->paddingtype = paddingtype;                  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int CIPHER::GetPaddingAdjustSize()
* @brief      Get padding adjust size
* @ingroup    CIPHER
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int CIPHER::GetPaddingAdjustSize()
{ 
  return paddingadjustsize;                         
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void CIPHER::SetPaddingAdjustSize(int paddingadjustsize)
* @brief      Set padding adjust size
* @ingroup    CIPHER
* 
* @param[in]  paddingadjustsize : Paddingadjustsize value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void CIPHER::SetPaddingAdjustSize(int paddingadjustsize)
{ 
  this->paddingadjustsize = paddingadjustsize;      
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHERKEY* CIPHER::GetKey(CIPHERKEYTYPE type)
* @brief      Get key
* @ingroup    CIPHER
* 
* @param[in]  type : Type value.
* 
* @return     CIPHERKEY* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERKEY* CIPHER::GetKey(CIPHERKEYTYPE type)
{
  int index;

  switch(type)
    {
      case CIPHERKEYTYPE_SYMMETRICAL  :
      case CIPHERKEYTYPE_RSA_PUBLIC   : index = 0;   break;
      case CIPHERKEYTYPE_RSA_PRIVATE  : index = 1;   break;
                      default         : return NULL;
    }

  return keys[index];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHER::SetKey(CIPHERKEY* key, bool integritycheck)
* @brief      Set key
* @ingroup    CIPHER
* 
* @param[in]  key : Key pointer to use.
* @param[in]  integritycheck : Integritycheck value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHER::SetKey(CIPHERKEY* key, bool integritycheck)
{
  if(!key) return false;

  int index;

  switch(key->GetType())
    {
      case CIPHERKEYTYPE_SYMMETRICAL  :
      case CIPHERKEYTYPE_RSA_PUBLIC   : index = 0;   break;
      case CIPHERKEYTYPE_RSA_PRIVATE  : index = 1;   break;
                default               : return false;
    }

  return SetKey(index, key);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE* CIPHER::GetInitVector()
* @brief      Get init vector
* @ingroup    CIPHER
* 
* @return     XBYTE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE* CIPHER::GetInitVector()
{
  if(!inivector) return NULL;

  return inivector->Get();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHER::SetInitVector(XBYTE* vector, XDWORD size)
* @brief      Set init vector
* @ingroup    CIPHER
* 
* @param[in]  vector : Vector pointer to use.
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHER::SetInitVector(XBYTE* vector, XDWORD size)
{
  if(!vector)    return false;
  if(!inivector) return false;

  inivector->Delete();
  inivector->Add(vector, size);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHER::SetInitVector(XBUFFER& vector)
* @brief      Set init vector
* @ingroup    CIPHER
* 
* @param[in]  vector : Vector value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHER::SetInitVector(XBUFFER& vector)
{
  inivector->Delete();
  inivector->Add(vector);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHER::Cipher(XBYTE* input, XDWORD size)
* @brief      Cipher
* @ingroup    CIPHER
* 
* @param[in]  input : Input pointer to use.
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHER::Cipher(XBYTE* input, XDWORD size)
{
  result->Delete();
  result->Add(input, size);

  CIPHERKEYSYMMETRICAL* key = (CIPHERKEYSYMMETRICAL*)GetKey(0);
  if(!key)                return false;
  if(!key->Get()->Get())  return false;

  int indexkey = 0;

  for(XDWORD c=0; c<size; c++)
    {
      result->Get()[c] ^= key->Get()->GetByte(indexkey);

      indexkey++;
      if(indexkey >= (int)key->Get()->GetSize()) indexkey = 0;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHER::Cipher(XBUFFER& input)
* @brief      Cipher
* @ingroup    CIPHER
* 
* @param[in]  input : Input value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHER::Cipher(XBUFFER& input)
{
  return Cipher(input.Get(), input.GetSize());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHER::Uncipher(XBYTE* input, XDWORD size)
* @brief      Uncipher
* @ingroup    CIPHER
* 
* @param[in]  input : Input pointer to use.
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHER::Uncipher(XBYTE* input, XDWORD size)
{
  return Cipher(input, size);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHER::Uncipher(XBUFFER& input)
* @brief      Uncipher
* @ingroup    CIPHER
* 
* @param[in]  input : Input value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHER::Uncipher(XBUFFER& input)
{
  return Uncipher(input.Get(), input.GetSize());
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool CIPHER::IsAEAD()
* @brief      Is this cipher an AEAD (Authenticated Encryption with Associated Data) one
* @note       VIRTUAL. Ciphers that authenticate as well as encrypt, such as AES-GCM, override this.
* @ingroup    CIPHER
*
* @return     bool : true if the condition is met; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHER::IsAEAD()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD CIPHER::GetAEADNonceSize()
* @brief      Get the size, in bytes, of the nonce expected by the AEAD operations
* @note       VIRTUAL
* @ingroup    CIPHER
*
* @return     XDWORD : Requested value. Zero if this cipher is not an AEAD one.
*
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD CIPHER::GetAEADNonceSize()
{
  return 0;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD CIPHER::GetAEADTagSize()
* @brief      Get the size, in bytes, of the authentication tag produced by the AEAD operations
* @note       VIRTUAL
* @ingroup    CIPHER
*
* @return     XDWORD : Requested value. Zero if this cipher is not an AEAD one.
*
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD CIPHER::GetAEADTagSize()
{
  return 0;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool CIPHER::CipherAEAD(XBYTE* input, XDWORD size, XBUFFER& nonce, XBUFFER& additionaldata, XBUFFER& tag)
* @brief      Cipher with authentication: the ciphertext is left in the result buffer and the tag in tag
* @note       VIRTUAL. The base implementation always fails, because a generic cipher is not an AEAD one.
* @ingroup    CIPHER
*
* @param[in]  input : Input pointer to use.
* @param[in]  size : Size value.
* @param[in]  nonce : Nonce, unique for every operation with the same key.
* @param[in]  additionaldata : Data authenticated but not ciphered. It may be empty.
* @param[out] tag : Buffer that receives the authentication tag.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHER::CipherAEAD(XBYTE* input, XDWORD size, XBUFFER& nonce, XBUFFER& additionaldata, XBUFFER& tag)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool CIPHER::CipherAEAD(XBUFFER& input, XBUFFER& nonce, XBUFFER& additionaldata, XBUFFER& tag)
* @brief      Cipher with authentication
* @ingroup    CIPHER
*
* @param[in]  input : Input buffer to use.
* @param[in]  nonce : Nonce, unique for every operation with the same key.
* @param[in]  additionaldata : Data authenticated but not ciphered. It may be empty.
* @param[out] tag : Buffer that receives the authentication tag.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHER::CipherAEAD(XBUFFER& input, XBUFFER& nonce, XBUFFER& additionaldata, XBUFFER& tag)
{
  return CipherAEAD(input.Get(), input.GetSize(), nonce, additionaldata, tag);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool CIPHER::UncipherAEAD(XBYTE* input, XDWORD size, XBUFFER& nonce, XBUFFER& additionaldata, XBUFFER& tag)
* @brief      Uncipher and verify the authentication tag: the plain text is left in the result buffer
* @note       VIRTUAL. The base implementation always fails, because a generic cipher is not an AEAD one.
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
bool CIPHER::UncipherAEAD(XBYTE* input, XDWORD size, XBUFFER& nonce, XBUFFER& additionaldata, XBUFFER& tag)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool CIPHER::UncipherAEAD(XBUFFER& input, XBUFFER& nonce, XBUFFER& additionaldata, XBUFFER& tag)
* @brief      Uncipher and verify the authentication tag
* @ingroup    CIPHER
*
* @param[in]  input : Input buffer to use.
* @param[in]  nonce : Nonce used by the cipher operation.
* @param[in]  additionaldata : Data authenticated but not ciphered. It may be empty.
* @param[in]  tag : Authentication tag to verify.
*
* @return     bool : true if the operation is successful and the tag is valid; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHER::UncipherAEAD(XBUFFER& input, XBUFFER& nonce, XBUFFER& additionaldata, XBUFFER& tag)
{
  return UncipherAEAD(input.Get(), input.GetSize(), nonce, additionaldata, tag);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XBYTE* CIPHER::GetResult(int& resultsize)
* @brief      Get result
* @ingroup    CIPHER
* 
* @param[in]  resultsize : Output resultsize.
* 
* @return     XBYTE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE* CIPHER::GetResult(int& resultsize)
{
  if(!result) return NULL;

  resultsize = result->GetSize();

  return result->Get();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBUFFER* CIPHER::GetResult()
* @brief      Get result
* @ingroup    CIPHER
* 
* @return     XBUFFER* : Pointer to the requested buffer; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* CIPHER::GetResult()
{
  return result;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHER::GetResultString(XSTRING& stringhex)
* @brief      Get result string
* @ingroup    CIPHER
* 
* @param[in]  stringhex : Stringhex value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHER::GetResultString(XSTRING& stringhex)
{
  stringhex.Empty();

  XBUFFER* xbuffer = GetResult();
  if(!xbuffer) return false;

  if(!xbuffer->GetSize()) return false;

  for(int c=0;c<(int)xbuffer->GetSize();c++)
    {
      XSTRING strbyte;

      strbyte.Format(_L("%02x"), xbuffer->GetByte(c));

      stringhex += strbyte;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHERKEY* CIPHER::GetKey(int index)
* @brief      Get key
* @ingroup    CIPHER
* 
* @param[in]  index : Index value.
* 
* @return     CIPHERKEY* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERKEY* CIPHER::GetKey(int index)
{
  if(index < 0)              return NULL;
  if(index >= CIPHERMAXKEYS) return NULL;

  return keys[index];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHER::SetKey(int index, CIPHERKEY* key)
* @brief      Set key
* @ingroup    CIPHER
* 
* @param[in]  index : Index value.
* @param[in]  key : Key pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHER::SetKey(int index, CIPHERKEY* key)
{
  if(index < 0)               return false;
  if(index >= CIPHERMAXKEYS) return false;

  this->keys[index] = key;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool CIPHER::CompareConstantTime(XBYTE* data1, XBYTE* data2, XDWORD size)
* @brief      Compare two blocks in a time that does not depend on where they start to differ
* @note       STATIC. Authentication tags, MACs and verify data must never be compared with memcmp: the duration of
*             an ordinary comparison tells an attacker how much of a forged value was already right.
* @ingroup    CIPHER
*
* @param[in]  data1 : First block.
* @param[in]  data2 : Second block.
* @param[in]  size : Size of both blocks, in bytes.
*
* @return     bool : true if both blocks are equal; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHER::CompareConstantTime(XBYTE* data1, XBYTE* data2, XDWORD size)
{
  XBYTE difference = 0;

  if(!data1 || !data2)
    {
      return false;
    }

  if(!size)
    {
      return false;
    }

  for(XDWORD c=0; c<size; c++)
    {
      difference |= (XBYTE)(data1[c] ^ data2[c]);
    }

  return (difference == 0)?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool CIPHER::CompareConstantTime(XBUFFER& data1, XBUFFER& data2)
* @brief      Compare two buffers in a time that does not depend on where they start to differ
* @note       STATIC. Buffers of a different size are told apart at once, because their size is not a secret.
* @ingroup    CIPHER
*
* @param[in]  data1 : First buffer.
* @param[in]  data2 : Second buffer.
*
* @return     bool : true if both buffers are equal; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHER::CompareConstantTime(XBUFFER& data1, XBUFFER& data2)
{
  if(data1.GetSize() != data2.GetSize())
    {
      return false;
    }

  return CompareConstantTime(data1.Get(), data2.Get(), data1.GetSize());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void CIPHER::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
void CIPHER::Clean()
{
  paddingtype  = XBUFFER_PADDINGTYPE_NONE;
  chainingmode = CIPHERCHAININGMODE_UNKNOWN;

  for(int c=0; c<CIPHERMAXKEYS; c++)
    {
      keys[c] = NULL;
    }

  inivector = NULL;
  result    = NULL;
}


