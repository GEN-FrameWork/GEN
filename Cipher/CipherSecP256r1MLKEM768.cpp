/**-------------------------------------------------------------------------------------------------------------------
*
* @file       CipherSecP256r1MLKEM768.cpp
*
* @class      CIPHERSECP256R1MLKEM768
* @brief      SecP256r1MLKEM768 hybrid key agreement (RFC 10024)
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

#include "CipherSecP256r1MLKEM768.h"

#include "GEN_Control.h"

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHERSECP256R1MLKEM768::CIPHERSECP256R1MLKEM768() : secp256r1(CIPHERTYPE_ECDSA_SECP256R1)
* @brief      Constructor of class
* @ingroup    CIPHER
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERSECP256R1MLKEM768::CIPHERSECP256R1MLKEM768()
  : secp256r1(CIPHERTYPE_ECDSA_SECP256R1)
{
  Clean();
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHERSECP256R1MLKEM768::~CIPHERSECP256R1MLKEM768()
* @brief      Destructor of class
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERSECP256R1MLKEM768::~CIPHERSECP256R1MLKEM768()
{
  Delete();
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERSECP256R1MLKEM768::ClientKeyShare_Create(XBUFFER& clientshare)
* @brief      Client key share create
* @ingroup    CIPHER
* 
* @param[in]  clientshare : Clientshare value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERSECP256R1MLKEM768::ClientKeyShare_Create(XBUFFER& clientshare)
{
  XBUFFER mlkempublic;

  Delete();
  clientshare.Delete();

  bool status = secp256r1.KeyPair_Create(secp256r1private, secp256r1public) &&
                mlkem.KeyPair_Create(mlkempublic, mlkemprivate) &&
                clientshare.Add(secp256r1public) && clientshare.Add(mlkempublic) &&
                (clientshare.GetSize() == CIPHERSECP256R1MLKEM768_CLIENTSHARESIZE);

  if(!status)
    {
      clientshare.Delete();
      Delete();
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERSECP256R1MLKEM768::ClientSharedSecret_Create(XBUFFER& servershare, XBUFFER& sharedsecret, bool* invalidpeershare)
* @brief      Client shared secret create
* @ingroup    CIPHER
* 
* @param[in]  servershare : Servershare value.
* @param[in]  sharedsecret : Sharedsecret value.
* @param[in]  invalidpeershare : Pointer to invalidpeershare.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERSECP256R1MLKEM768::ClientSharedSecret_Create(XBUFFER& servershare, XBUFFER& sharedsecret, bool* invalidpeershare)
{
  XBUFFER         peerpublic;
  XBUFFER         ciphertext;
  XSECUREBUFFER   ecdhsecret;
  XSECUREBUFFER   mlkemsecret;
  bool            status = false;

  if(invalidpeershare) (*invalidpeershare) = false;
  sharedsecret.SecureDelete();

  do
    {
      if(servershare.GetSize() != CIPHERSECP256R1MLKEM768_SERVERSHARESIZE)
        {
          if(invalidpeershare) (*invalidpeershare) = true;
          break;
        }

      if((secp256r1private.GetSize() != CIPHERECDSA_P256_COORDINATE_SIZE) ||
         (mlkemprivate.GetSize() != CIPHERMLKEM768_PRIVATEKEYSIZE)) break;

      if(!peerpublic.Add(servershare.Get(), CIPHERECDSA_P256_PUBLICKEY_SIZE) ||
         !secp256r1.PublicKey_Check(peerpublic))
        {
          if(invalidpeershare) (*invalidpeershare) = true;
          break;
        }

      if(!ciphertext.Add(servershare.Get() + CIPHERECDSA_P256_PUBLICKEY_SIZE,
                         CIPHERMLKEM768_CIPHERTEXTSIZE) ||
         !secp256r1.SharedSecret_Create(secp256r1private, peerpublic, ecdhsecret) ||
         !mlkem.Decapsulate(mlkemprivate, ciphertext, mlkemsecret)) break;

      status = sharedsecret.Add(ecdhsecret) && sharedsecret.Add(mlkemsecret) &&
               (sharedsecret.GetSize() == CIPHERSECP256R1MLKEM768_SHAREDSECRETSIZE);
    }
  while(false);

  if(!status) sharedsecret.SecureDelete();
  Delete();
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERSECP256R1MLKEM768::ServerKeyShare_Create(XBUFFER& clientshare, XBUFFER& servershare, XBUFFER& sharedsecret, bool* invalidpeershare)
* @brief      Server key share create
* @ingroup    CIPHER
* 
* @param[in]  clientshare : Clientshare value.
* @param[in]  servershare : Servershare value.
* @param[in]  sharedsecret : Sharedsecret value.
* @param[in]  invalidpeershare : Pointer to invalidpeershare.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERSECP256R1MLKEM768::ServerKeyShare_Create(XBUFFER& clientshare, XBUFFER& servershare, XBUFFER& sharedsecret, bool* invalidpeershare)
{
  XBUFFER         peerpublic;
  XBUFFER         mlkempublic;
  XBUFFER         serverpublic;
  XSECUREBUFFER   serverprivate;
  XBUFFER         ciphertext;
  XSECUREBUFFER   ecdhsecret;
  XSECUREBUFFER   mlkemsecret;
  bool            status = false;

  if(invalidpeershare) (*invalidpeershare) = false;
  Delete();
  servershare.Delete();
  sharedsecret.SecureDelete();

  do
    {
      if(clientshare.GetSize() != CIPHERSECP256R1MLKEM768_CLIENTSHARESIZE)
        {
          if(invalidpeershare) (*invalidpeershare) = true;
          break;
        }

      if(!peerpublic.Add(clientshare.Get(), CIPHERECDSA_P256_PUBLICKEY_SIZE) ||
         !secp256r1.PublicKey_Check(peerpublic))
        {
          if(invalidpeershare) (*invalidpeershare) = true;
          break;
        }

      if(!mlkempublic.Add(clientshare.Get() + CIPHERECDSA_P256_PUBLICKEY_SIZE,
                          CIPHERMLKEM768_PUBLICKEYSIZE) ||
         !mlkem.PublicKey_Check(mlkempublic))
        {
          if(invalidpeershare) (*invalidpeershare) = true;
          break;
        }

      if(!secp256r1.KeyPair_Create(serverprivate, serverpublic) ||
         !secp256r1.SharedSecret_Create(serverprivate, peerpublic, ecdhsecret) ||
         !mlkem.Encapsulate(mlkempublic, ciphertext, mlkemsecret)) break;

      status = servershare.Add(serverpublic) && servershare.Add(ciphertext) &&
               sharedsecret.Add(ecdhsecret) && sharedsecret.Add(mlkemsecret) &&
               (servershare.GetSize() == CIPHERSECP256R1MLKEM768_SERVERSHARESIZE) &&
               (sharedsecret.GetSize() == CIPHERSECP256R1MLKEM768_SHAREDSECRETSIZE);
    }
  while(false);

  serverprivate.SecureDelete();

  if(!status)
    {
      servershare.Delete();
      sharedsecret.SecureDelete();
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void CIPHERSECP256R1MLKEM768::Delete()
* @brief      Delete
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
void CIPHERSECP256R1MLKEM768::Delete()
{
  secp256r1private.SecureDelete();
  secp256r1public.Delete();
  mlkemprivate.SecureDelete();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void CIPHERSECP256R1MLKEM768::Clean()
* @brief      Clean
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
void CIPHERSECP256R1MLKEM768::Clean()
{
}
