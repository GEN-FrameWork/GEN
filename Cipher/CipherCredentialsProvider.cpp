/**-------------------------------------------------------------------------------------------------------------------
*
* @file       CipherCredentialsProvider.cpp
*
* @class      CIPHERCREDENTIALSPROVIDER
* @brief      Abstract source of certificate chains, private keys and secrets for Cipher consumers
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

#include "CipherCredentialsProvider.h"
#include "CipherCredentialsLoader.h"

#include "GEN_Control.h"


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHERSECRETPROVIDER::CIPHERSECRETPROVIDER()
* @brief      Constructor of class
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERSECRETPROVIDER::CIPHERSECRETPROVIDER()
{
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHERSECRETPROVIDER::~CIPHERSECRETPROVIDER()
* @brief      Destructor of class
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERSECRETPROVIDER::~CIPHERSECRETPROVIDER()
{
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERSECRETPROVIDER::Secret_Release(XSTRING& secret)
* @brief      Secret release
* @ingroup    CIPHER
* 
* @param[in]  secret : Secret value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERSECRETPROVIDER::Secret_Release(XSTRING& secret)
{
  if(secret.Get())
    {
      volatile XCHAR* data = secret.Get();
      for(XDWORD c=0; c<secret.GetSize(); c++) data[c]=0;
    }

  secret.Empty();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHERSECRETPROVIDERSTRING::CIPHERSECRETPROVIDERSTRING()
* @brief      Constructor of class
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERSECRETPROVIDERSTRING::CIPHERSECRETPROVIDERSTRING()
{
  source = NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHERSECRETPROVIDERSTRING::~CIPHERSECRETPROVIDERSTRING()
* @brief      Destructor of class
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERSECRETPROVIDERSTRING::~CIPHERSECRETPROVIDERSTRING()
{
  source = NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void CIPHERSECRETPROVIDERSTRING::SetSource(XSTRING* source)
* @brief      Set source
* @ingroup    CIPHER
* 
* @param[in]  source : Pointer to source.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void CIPHERSECRETPROVIDERSTRING::SetSource(XSTRING* source)
{
  this->source = source;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERSECRETPROVIDERSTRING::Secret_Get(XSTRING& secret)
* @brief      Secret get
* @ingroup    CIPHER
* 
* @param[in]  secret : Secret value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERSECRETPROVIDERSTRING::Secret_Get(XSTRING& secret)
{
  Secret_Release(secret);

  if(!source || source->IsEmpty()) return true;

  return secret.Set((*source));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHERCREDENTIALSPROVIDER::CIPHERCREDENTIALSPROVIDER()
* @brief      Constructor of class
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERCREDENTIALSPROVIDER::CIPHERCREDENTIALSPROVIDER()
{
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHERCREDENTIALSPROVIDER::~CIPHERCREDENTIALSPROVIDER()
* @brief      Destructor of class
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERCREDENTIALSPROVIDER::~CIPHERCREDENTIALSPROVIDER()
{
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHERCREDENTIALSPROVIDERBUFFER::CIPHERCREDENTIALSPROVIDERBUFFER()
* @brief      Constructor of class
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERCREDENTIALSPROVIDERBUFFER::CIPHERCREDENTIALSPROVIDERBUFFER()
{
  certificatedata = NULL;
  privatekeydata  = NULL;
  secretprovider  = NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHERCREDENTIALSPROVIDERBUFFER::~CIPHERCREDENTIALSPROVIDERBUFFER()
* @brief      Destructor of class
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERCREDENTIALSPROVIDERBUFFER::~CIPHERCREDENTIALSPROVIDERBUFFER()
{
  certificatedata = NULL;
  privatekeydata  = NULL;
  secretprovider  = NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void CIPHERCREDENTIALSPROVIDERBUFFER::SetCertificateData(XBUFFER* certificatedata)
* @brief      Set certificate data
* @ingroup    CIPHER
* 
* @param[in]  certificatedata : Pointer to certificatedata.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void CIPHERCREDENTIALSPROVIDERBUFFER::SetCertificateData(XBUFFER* certificatedata)
{
  this->certificatedata = certificatedata;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void CIPHERCREDENTIALSPROVIDERBUFFER::SetPrivateKeyData(XBUFFER* privatekeydata)
* @brief      Set private key data
* @ingroup    CIPHER
* 
* @param[in]  privatekeydata : Pointer to privatekeydata.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void CIPHERCREDENTIALSPROVIDERBUFFER::SetPrivateKeyData(XBUFFER* privatekeydata)
{
  this->privatekeydata = privatekeydata;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void CIPHERCREDENTIALSPROVIDERBUFFER::SetSecretProvider(CIPHERSECRETPROVIDER* secretprovider)
* @brief      Set secret provider
* @ingroup    CIPHER
* 
* @param[in]  secretprovider : Pointer to secretprovider.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void CIPHERCREDENTIALSPROVIDERBUFFER::SetSecretProvider(CIPHERSECRETPROVIDER* secretprovider)
{
  this->secretprovider = secretprovider;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERCREDENTIALSPROVIDERBUFFER::Credentials_Load(XVECTOR<XBUFFER*>& certificatechain, CIPHERKEY*& privatekey)
* @brief      Credentials load
* @ingroup    CIPHER
* 
* @param[in]  certificatechain : Pointer to certificatechain.
* @param[in]  privatekey : Pointer to privatekey.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERCREDENTIALSPROVIDERBUFFER::Credentials_Load(XVECTOR<XBUFFER*>& certificatechain, CIPHERKEY*& privatekey)
{
  if(!certificatedata || !privatekeydata) return false;

  XSTRING secret;
  XCHAR* password = NULL;

  if(secretprovider)
    {
      if(!secretprovider->Secret_Get(secret))
        {
          secretprovider->Secret_Release(secret);
          return false;
        }

      if(!secret.IsEmpty()) password = secret.Get();
    }

  bool status = CIPHERCREDENTIALSLOADER::Credentials_Load((*certificatedata), (*privatekeydata), password, certificatechain, privatekey);

  if(secretprovider) secretprovider->Secret_Release(secret);
  else
    {
      if(secret.Get())
        {
          volatile XCHAR* data = secret.Get();
          for(XDWORD c=0; c<secret.GetSize(); c++) data[c]=0;
        }

      secret.Empty();
    }

  return status;
}
