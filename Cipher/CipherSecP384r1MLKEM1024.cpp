/**-------------------------------------------------------------------------------------------------------------------
*
* @file       CipherSecP384r1MLKEM1024.cpp
*
* @class      CIPHERSECP384R1MLKEM1024
* @brief      SecP384r1MLKEM1024 hybrid key agreement (RFC 10024)
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

#include "CipherSecP384r1MLKEM1024.h"

#include "GEN_Control.h"

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHERSECP384R1MLKEM1024::CIPHERSECP384R1MLKEM1024() : secp384r1(CIPHERTYPE_ECDSA_SECP384R1)
* @brief      Constructor of class
* @ingroup    CIPHER
* 
* @param[in]  Value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERSECP384R1MLKEM1024::CIPHERSECP384R1MLKEM1024()
  : secp384r1(CIPHERTYPE_ECDSA_SECP384R1)
{
  Clean();
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHERSECP384R1MLKEM1024::~CIPHERSECP384R1MLKEM1024()
* @brief      Destructor of class
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERSECP384R1MLKEM1024::~CIPHERSECP384R1MLKEM1024()
{
  Delete();
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERSECP384R1MLKEM1024::ClientKeyShare_Create(XBUFFER& clientshare)
* @brief      Client key share create
* @ingroup    CIPHER
* 
* @param[in]  clientshare : Clientshare value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERSECP384R1MLKEM1024::ClientKeyShare_Create(XBUFFER& clientshare)
{
  XBUFFER mlkempublic;

  Delete();
  clientshare.Delete();

  bool status = secp384r1.KeyPair_Create(secp384r1private, secp384r1public) &&
                mlkem.KeyPair_Create(mlkempublic, mlkemprivate) &&
                clientshare.Add(secp384r1public) && clientshare.Add(mlkempublic) &&
                (clientshare.GetSize() == CIPHERSECP384R1MLKEM1024_CLIENTSHARESIZE);

  if(!status)
    {
      clientshare.Delete();
      Delete();
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERSECP384R1MLKEM1024::ClientSharedSecret_Create(XBUFFER& servershare, XBUFFER& sharedsecret, bool* invalidpeershare)
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
bool CIPHERSECP384R1MLKEM1024::ClientSharedSecret_Create(XBUFFER& servershare, XBUFFER& sharedsecret, bool* invalidpeershare)
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
      if(servershare.GetSize() != CIPHERSECP384R1MLKEM1024_SERVERSHARESIZE)
        {
          if(invalidpeershare) (*invalidpeershare) = true;
          break;
        }

      if((secp384r1private.GetSize() != CIPHERECDSA_P384_COORDINATE_SIZE) ||
         (mlkemprivate.GetSize() != CIPHERMLKEM1024_PRIVATEKEYSIZE)) break;

      if(!peerpublic.Add(servershare.Get(), CIPHERECDSA_P384_PUBLICKEY_SIZE) ||
         !secp384r1.PublicKey_Check(peerpublic))
        {
          if(invalidpeershare) (*invalidpeershare) = true;
          break;
        }

      if(!ciphertext.Add(servershare.Get() + CIPHERECDSA_P384_PUBLICKEY_SIZE,
                         CIPHERMLKEM1024_CIPHERTEXTSIZE) ||
         !secp384r1.SharedSecret_Create(secp384r1private, peerpublic, ecdhsecret) ||
         !mlkem.Decapsulate(mlkemprivate, ciphertext, mlkemsecret)) break;

      status = sharedsecret.Add(ecdhsecret) && sharedsecret.Add(mlkemsecret) &&
               (sharedsecret.GetSize() == CIPHERSECP384R1MLKEM1024_SHAREDSECRETSIZE);
    }
  while(false);

  if(!status) sharedsecret.SecureDelete();
  Delete();
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERSECP384R1MLKEM1024::ServerKeyShare_Create(XBUFFER& clientshare, XBUFFER& servershare, XBUFFER& sharedsecret, bool* invalidpeershare)
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
bool CIPHERSECP384R1MLKEM1024::ServerKeyShare_Create(XBUFFER& clientshare, XBUFFER& servershare, XBUFFER& sharedsecret, bool* invalidpeershare)
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
      if(clientshare.GetSize() != CIPHERSECP384R1MLKEM1024_CLIENTSHARESIZE)
        {
          if(invalidpeershare) (*invalidpeershare) = true;
          break;
        }

      if(!peerpublic.Add(clientshare.Get(), CIPHERECDSA_P384_PUBLICKEY_SIZE) ||
         !secp384r1.PublicKey_Check(peerpublic))
        {
          if(invalidpeershare) (*invalidpeershare) = true;
          break;
        }

      if(!mlkempublic.Add(clientshare.Get() + CIPHERECDSA_P384_PUBLICKEY_SIZE,
                          CIPHERMLKEM1024_PUBLICKEYSIZE) ||
         !mlkem.PublicKey_Check(mlkempublic))
        {
          if(invalidpeershare) (*invalidpeershare) = true;
          break;
        }

      if(!secp384r1.KeyPair_Create(serverprivate, serverpublic) ||
         !secp384r1.SharedSecret_Create(serverprivate, peerpublic, ecdhsecret) ||
         !mlkem.Encapsulate(mlkempublic, ciphertext, mlkemsecret)) break;

      status = servershare.Add(serverpublic) && servershare.Add(ciphertext) &&
               sharedsecret.Add(ecdhsecret) && sharedsecret.Add(mlkemsecret) &&
               (servershare.GetSize() == CIPHERSECP384R1MLKEM1024_SERVERSHARESIZE) &&
               (sharedsecret.GetSize() == CIPHERSECP384R1MLKEM1024_SHAREDSECRETSIZE);
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
* @fn         void CIPHERSECP384R1MLKEM1024::Delete()
* @brief      Delete
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
void CIPHERSECP384R1MLKEM1024::Delete()
{
  secp384r1private.SecureDelete();
  secp384r1public.Delete();
  mlkemprivate.SecureDelete();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void CIPHERSECP384R1MLKEM1024::Clean()
* @brief      Clean
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
void CIPHERSECP384R1MLKEM1024::Clean()
{
}
