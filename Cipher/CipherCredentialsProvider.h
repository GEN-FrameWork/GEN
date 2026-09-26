/**-------------------------------------------------------------------------------------------------------------------
*
* @file       CipherCredentialsProvider.h
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

#pragma once

#include "XBuffer.h"
#include "XString.h"
#include "XVector.h"
#include "CipherKey.h"

class CIPHERSECRETPROVIDER
{
  public:
                                CIPHERSECRETPROVIDER              ();
    virtual                    ~CIPHERSECRETPROVIDER              ();

    virtual bool                Secret_Get                         (XSTRING& secret) = 0;
    virtual bool                Secret_Release                     (XSTRING& secret);
};

class CIPHERSECRETPROVIDERSTRING : public CIPHERSECRETPROVIDER
{
  public:
                                CIPHERSECRETPROVIDERSTRING        ();
    virtual                    ~CIPHERSECRETPROVIDERSTRING        ();

    void                        SetSource                          (XSTRING* source);
    bool                        Secret_Get                         (XSTRING& secret);

  private:
    XSTRING*                    source;
};

class CIPHERCREDENTIALSPROVIDER
{
  public:
                                CIPHERCREDENTIALSPROVIDER         ();
    virtual                    ~CIPHERCREDENTIALSPROVIDER         ();

    virtual bool                Credentials_Load                   (XVECTOR<XBUFFER*>& certificatechain, CIPHERKEY*& privatekey) = 0;
};

class CIPHERCREDENTIALSPROVIDERBUFFER : public CIPHERCREDENTIALSPROVIDER
{
  public:
                                CIPHERCREDENTIALSPROVIDERBUFFER   ();
    virtual                    ~CIPHERCREDENTIALSPROVIDERBUFFER   ();

    void                        SetCertificateData                 (XBUFFER* certificatedata);
    void                        SetPrivateKeyData                  (XBUFFER* privatekeydata);
    void                        SetSecretProvider                  (CIPHERSECRETPROVIDER* secretprovider);

    bool                        Credentials_Load                   (XVECTOR<XBUFFER*>& certificatechain, CIPHERKEY*& privatekey);

  private:
    XBUFFER*                    certificatedata;
    XBUFFER*                    privatekeydata;
    CIPHERSECRETPROVIDER*       secretprovider;
};
