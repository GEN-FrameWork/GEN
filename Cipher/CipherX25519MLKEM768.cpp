/**-------------------------------------------------------------------------------------------------------------------
*
* @file       CipherX25519MLKEM768.cpp
*
* @class      CIPHERX25519MLKEM768
* @brief      X25519MLKEM768 hybrid key agreement (RFC 10024)
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

#include "CipherX25519MLKEM768.h"

#include "GEN_Control.h"

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHERX25519MLKEM768::CIPHERX25519MLKEM768()
* @brief      Constructor of class
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERX25519MLKEM768::CIPHERX25519MLKEM768()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHERX25519MLKEM768::~CIPHERX25519MLKEM768()
* @brief      Destructor of class
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERX25519MLKEM768::~CIPHERX25519MLKEM768()
{
  Delete();
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERX25519MLKEM768::X25519KeyPair_Create(CIPHERECDSAX25519& x25519, XBUFFER& publickey)
* @brief      X25519 key pair create
* @ingroup    CIPHER
* 
* @param[in]  x25519 : X25519 value.
* @param[in]  publickey : Publickey value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERX25519MLKEM768::X25519KeyPair_Create(CIPHERECDSAX25519& x25519, XBUFFER& publickey)
{
  publickey.Delete();
  x25519.CleanAllKeys();
  return x25519.GenerateRandomPrivateKey() && x25519.CreatePublicKey() &&
         publickey.Add(x25519.GetKey(CIPHERECDSAX25519_TYPEKEY_PUBLIC), CIPHERECDSAX25519_MAXKEY);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERX25519MLKEM768::X25519SharedSecret_Create(CIPHERECDSAX25519& x25519, XBYTE* peerpublic, XBUFFER& sharedsecret, bool* invalidpeershare)
* @brief      X25519 shared secret create
* @ingroup    CIPHER
* 
* @param[in]  x25519 : X25519 value.
* @param[in]  peerpublic : Pointer to peerpublic.
* @param[in]  sharedsecret : Sharedsecret value.
* @param[in]  invalidpeershare : Pointer to invalidpeershare.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERX25519MLKEM768::X25519SharedSecret_Create(CIPHERECDSAX25519& x25519, XBYTE* peerpublic, XBUFFER& sharedsecret, bool* invalidpeershare)
{
  if(invalidpeershare) (*invalidpeershare) = false;

  sharedsecret.SecureDelete();
  if(!peerpublic || !x25519.GetKey(CIPHERECDSAX25519_TYPEKEY_PRIVATE)) return false;

  // CIPHERECDSAX25519::CreateSharedKey() returns false for the all-zero result. With a fresh local private
  // scalar that result is a peer-share validation failure, as required by RFC 10024 / RFC 7748.
  if(!x25519.CreateSharedKey(peerpublic))
    {
      if(invalidpeershare) (*invalidpeershare) = true;
      return false;
    }

  XBYTE* secret = x25519.GetKey(CIPHERECDSAX25519_TYPEKEY_SHARED);
  if(!secret) return false;

  XBYTE any = 0;
  for(int c=0; c<CIPHERECDSAX25519_MAXKEY; c++) any |= secret[c];
  if(!any)
    {
      if(invalidpeershare) (*invalidpeershare) = true;
      return false;
    }

  return sharedsecret.Add(secret, CIPHERECDSAX25519_MAXKEY);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERX25519MLKEM768::ClientKeyShare_Create(XBUFFER& clientshare)
* @brief      Client key share create
* @ingroup    CIPHER
* 
* @param[in]  clientshare : Clientshare value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERX25519MLKEM768::ClientKeyShare_Create(XBUFFER& clientshare)
{
  XBUFFER mlkempublic;
  XBUFFER xpublic;

  Delete();
  clientshare.Delete();

  bool status = mlkem.KeyPair_Create(mlkempublic, mlkemprivate) &&
                X25519KeyPair_Create(x25519, xpublic) &&
                clientshare.Add(mlkempublic) && clientshare.Add(xpublic) &&
                (clientshare.GetSize() == CIPHERX25519MLKEM768_CLIENTSHARESIZE);

  if(!status)
    {
      clientshare.Delete();
      Delete();
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERX25519MLKEM768::ClientSharedSecret_Create(XBUFFER& servershare, XBUFFER& sharedsecret, bool* invalidpeershare)
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
bool CIPHERX25519MLKEM768::ClientSharedSecret_Create(XBUFFER& servershare, XBUFFER& sharedsecret, bool* invalidpeershare)
{
  XBUFFER ciphertext;
  XSECUREBUFFER mlkemsecret;
  XSECUREBUFFER xsecret;
  bool status = false;

  if(invalidpeershare) (*invalidpeershare) = false;
  sharedsecret.SecureDelete();

  do
    {
      if(servershare.GetSize() != CIPHERX25519MLKEM768_SERVERSHARESIZE)
        {
          if(invalidpeershare) (*invalidpeershare) = true;
          break;
        }

      if(mlkemprivate.GetSize() != CIPHERMLKEM768_PRIVATEKEYSIZE) break;

      if(!ciphertext.Add(servershare.Get(), CIPHERMLKEM768_CIPHERTEXTSIZE) ||
         !mlkem.Decapsulate(mlkemprivate, ciphertext, mlkemsecret) ||
         !X25519SharedSecret_Create(x25519, servershare.Get()+CIPHERMLKEM768_CIPHERTEXTSIZE,
                                    xsecret, invalidpeershare)) break;

      // RFC 10024 fixes this order: ML-KEM shared secret first, then X25519.
      status = sharedsecret.Add(mlkemsecret) && sharedsecret.Add(xsecret) &&
               (sharedsecret.GetSize() == CIPHERX25519MLKEM768_SHAREDSECRETSIZE);
    }
  while(false);

  if(!status) sharedsecret.SecureDelete();
  Delete();
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERX25519MLKEM768::ServerKeyShare_Create(XBUFFER& clientshare, XBUFFER& servershare, XBUFFER& sharedsecret, bool* invalidpeershare)
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
bool CIPHERX25519MLKEM768::ServerKeyShare_Create(XBUFFER& clientshare, XBUFFER& servershare, XBUFFER& sharedsecret, bool* invalidpeershare)
{
  XBUFFER mlkempublic;
  XBUFFER ciphertext;
  XSECUREBUFFER mlkemsecret;
  XBUFFER xpublic;
  XSECUREBUFFER xsecret;
  bool status = false;

  if(invalidpeershare) (*invalidpeershare) = false;
  Delete();
  servershare.Delete();
  sharedsecret.SecureDelete();

  do
    {
      if(clientshare.GetSize() != CIPHERX25519MLKEM768_CLIENTSHARESIZE)
        {
          if(invalidpeershare) (*invalidpeershare) = true;
          break;
        }

      if(!mlkempublic.Add(clientshare.Get(), CIPHERMLKEM768_PUBLICKEYSIZE)) break;
      if(!mlkem.PublicKey_Check(mlkempublic))
        {
          // FIPS 203 encapsulation-key check required before consuming encapsulation randomness.
          if(invalidpeershare) (*invalidpeershare) = true;
          break;
        }

      if(!X25519KeyPair_Create(x25519, xpublic) ||
         !X25519SharedSecret_Create(x25519, clientshare.Get()+CIPHERMLKEM768_PUBLICKEYSIZE,
                                    xsecret, invalidpeershare) ||
         !mlkem.Encapsulate(mlkempublic, ciphertext, mlkemsecret)) break;

      status = servershare.Add(ciphertext) && servershare.Add(xpublic) &&
               sharedsecret.Add(mlkemsecret) && sharedsecret.Add(xsecret) &&
               (servershare.GetSize() == CIPHERX25519MLKEM768_SERVERSHARESIZE) &&
               (sharedsecret.GetSize() == CIPHERX25519MLKEM768_SHAREDSECRETSIZE);
    }
  while(false);

  x25519.CleanAllKeys();
  if(!status)
    {
      servershare.Delete();
      sharedsecret.SecureDelete();
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void CIPHERX25519MLKEM768::Delete()
* @brief      Delete
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
void CIPHERX25519MLKEM768::Delete()
{
  x25519.CleanAllKeys();
  mlkemprivate.SecureDelete();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void CIPHERX25519MLKEM768::Clean()
* @brief      Clean
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
void CIPHERX25519MLKEM768::Clean()
{
}
