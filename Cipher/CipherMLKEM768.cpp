/**-------------------------------------------------------------------------------------------------------------------
*
* @file       CipherMLKEM768.cpp
*
* @class      CIPHERMLKEM768
* @brief      ML-KEM-768 (FIPS 203) key encapsulation mechanism
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

#include "GEN_Defines.h"

#include "CipherMLKEM768.h"

#include "XFactory.h"
#include "XRand.h"

#include "GEN_Control.h"

namespace
{
  /**-------------------------------------------------------------------------------------------------------------------
  * 
  * @fn         static void CIPHERMLKEM768_SecureErase(void* data, XDWORD size)
  * @brief      Secure erase
  * @ingroup    CIPHER
  * 
  * @param[in]  data : Pointer to data.
  * @param[in]  size : Size value.
  * 
  * --------------------------------------------------------------------------------------------------------------------*/
  static void CIPHERMLKEM768_SecureErase(void* data, XDWORD size)
  {
    volatile XBYTE* bytes = (volatile XBYTE*)data;
    for(XDWORD c=0; c<size; c++) bytes[c] = 0;
  }
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHERMLKEM768::CIPHERMLKEM768()
* @brief      Constructor of class
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERMLKEM768::CIPHERMLKEM768()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHERMLKEM768::~CIPHERMLKEM768()
* @brief      Destructor of class
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERMLKEM768::~CIPHERMLKEM768()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERMLKEM768::Random(XBYTE* data, XDWORD size)
* @brief      Random
* @ingroup    CIPHER
* 
* @param[in]  data : Pointer to data.
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERMLKEM768::Random(XBYTE* data, XDWORD size)
{
  if(!data || !size) return false;

  XRAND* xrand = GEN_XFACTORY.CreateRand();
  if(!xrand) return false;

  bool status = false;
  if(xrand->Ini() && xrand->IsCryptographicallySecure()) status = xrand->Generate(data, size);
  GEN_XFACTORY.DeleteRand(xrand);
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERMLKEM768::KeyPair_Create(XBUFFER& publickey, XBUFFER& privatekey)
* @brief      Key pair create
* @ingroup    CIPHER
* 
* @param[in]  publickey : Publickey value.
* @param[in]  privatekey : Privatekey value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERMLKEM768::KeyPair_Create(XBUFFER& publickey, XBUFFER& privatekey)
{
  XBYTE d[32];
  XBYTE z[32];
  XBYTE pk[CIPHERMLKEM768_PUBLICKEYSIZE];
  XBYTE sk[CIPHERMLKEM768_PRIVATEKEYSIZE];

  publickey.Delete();
  privatekey.SecureDelete();

  bool status = Random(d, sizeof(d)) && Random(z, sizeof(z)) &&
                CIPHERMLKEM768CORE::KeyPair(d, z, pk, sk) &&
                publickey.Add(pk, sizeof(pk)) && privatekey.Add(sk, sizeof(sk));

  if(!status)
    {
      publickey.Delete();
      privatekey.SecureDelete();
    }

  CIPHERMLKEM768_SecureErase(d, sizeof(d));
  CIPHERMLKEM768_SecureErase(z, sizeof(z));
  CIPHERMLKEM768_SecureErase(pk, sizeof(pk));
  CIPHERMLKEM768_SecureErase(sk, sizeof(sk));
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERMLKEM768::Encapsulate(XBUFFER& publickey, XBUFFER& ciphertext, XBUFFER& sharedsecret)
* @brief      Encapsulate
* @ingroup    CIPHER
* 
* @param[in]  publickey : Publickey value.
* @param[in]  ciphertext : Ciphertext value.
* @param[in]  sharedsecret : Sharedsecret value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERMLKEM768::Encapsulate(XBUFFER& publickey, XBUFFER& ciphertext, XBUFFER& sharedsecret)
{
  XBYTE randomness[32];
  XBYTE ct[CIPHERMLKEM768_CIPHERTEXTSIZE];
  XBYTE ss[CIPHERMLKEM768_SHAREDSECRETSIZE];

  ciphertext.Delete();
  sharedsecret.SecureDelete();

  if(!PublicKey_Check(publickey)) return false;

  bool status = Random(randomness, sizeof(randomness)) &&
                CIPHERMLKEM768CORE::Encapsulate(randomness, publickey.Get(), ct, ss) &&
                ciphertext.Add(ct, sizeof(ct)) && sharedsecret.Add(ss, sizeof(ss));

  if(!status)
    {
      ciphertext.Delete();
      sharedsecret.SecureDelete();
    }

  CIPHERMLKEM768_SecureErase(randomness, sizeof(randomness));
  CIPHERMLKEM768_SecureErase(ct, sizeof(ct));
  CIPHERMLKEM768_SecureErase(ss, sizeof(ss));
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERMLKEM768::Decapsulate(XBUFFER& privatekey, XBUFFER& ciphertext, XBUFFER& sharedsecret)
* @brief      Decapsulate
* @ingroup    CIPHER
* 
* @param[in]  privatekey : Privatekey value.
* @param[in]  ciphertext : Ciphertext value.
* @param[in]  sharedsecret : Sharedsecret value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERMLKEM768::Decapsulate(XBUFFER& privatekey, XBUFFER& ciphertext, XBUFFER& sharedsecret)
{
  XBYTE ss[CIPHERMLKEM768_SHAREDSECRETSIZE];

  sharedsecret.SecureDelete();

  if(privatekey.GetSize() != CIPHERMLKEM768_PRIVATEKEYSIZE || ciphertext.GetSize() != CIPHERMLKEM768_CIPHERTEXTSIZE)
    {
      return false;
    }

  bool status = CIPHERMLKEM768CORE::Decapsulate(privatekey.Get(), ciphertext.Get(), ss) &&
                sharedsecret.Add(ss, sizeof(ss));
  if(!status) sharedsecret.SecureDelete();
  CIPHERMLKEM768_SecureErase(ss, sizeof(ss));
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERMLKEM768::PublicKey_Check(XBUFFER& publickey)
* @brief      Public key check
* @ingroup    CIPHER
* 
* @param[in]  publickey : Publickey value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERMLKEM768::PublicKey_Check(XBUFFER& publickey)
{
  return publickey.GetSize() == CIPHERMLKEM768_PUBLICKEYSIZE && CIPHERMLKEM768CORE::PublicKey_Check(publickey.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void CIPHERMLKEM768::Clean()
* @brief      Clean
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
void CIPHERMLKEM768::Clean()
{
}
