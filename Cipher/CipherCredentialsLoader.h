/**-------------------------------------------------------------------------------------------------------------------
*
* @file       CipherCredentialsLoader.h
*
* @class      CIPHERCREDENTIALSLOADER
* @brief      Central certificate/private-key format loader for TLS and other Cipher consumers
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

class CIPHERCREDENTIALSLOADER
{
  public:
    static bool                             Certificates_Load              (XBUFFER& filedata, XVECTOR<XBUFFER*>& certificatechain);
    static bool                             PrivateKey_Load                (XBUFFER& filedata, XCHAR* password, CIPHERKEYTYPE expectedpublickeytype, CIPHERKEY*& privatekey);
    static bool                             Credentials_Load               (XBUFFER& certificatedata, XBUFFER& privatekeydata, XCHAR* password, XVECTOR<XBUFFER*>& certificatechain, CIPHERKEY*& privatekey);
    static void                             Certificates_Delete           (XVECTOR<XBUFFER*>& certificatechain);
    static void                             PrivateKey_Delete              (CIPHERKEY*& privatekey);

  private:
    static bool                             PEMBlocks_Decode               (XBUFFER& filedata, const char* label, XVECTOR<XBUFFER*>& blocks);
    static bool                             PrivateKeyDER_Decode           (XBUFFER& DER, CIPHERKEYTYPE expectedpublickeytype, CIPHERKEY*& privatekey);
};

