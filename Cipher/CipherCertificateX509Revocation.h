/**-------------------------------------------------------------------------------------------------------------------
*
* @file       CipherCertificateX509Revocation.h
*
* @class      CIPHERCERTIFICATEX509REVOCATION
* @brief      Signed OCSP/CRL validation primitives for X.509 consumers
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

#include "CipherCertificateX509.h"

#define CIPHERCERTIFICATEX509REVOCATION_MAX_OCSP_SIZE   (256*1024)
#define CIPHERCERTIFICATEX509REVOCATION_MAX_CRL_SIZE    (4*1024*1024)

enum CIPHERCERTIFICATEX509REVOCATION_RESULT
{
  CIPHERCERTIFICATEX509REVOCATION_RESULT_INVALID = 0,
  CIPHERCERTIFICATEX509REVOCATION_RESULT_GOOD,
  CIPHERCERTIFICATEX509REVOCATION_RESULT_REVOKED,
  CIPHERCERTIFICATEX509REVOCATION_RESULT_UNKNOWN
};

class CIPHERCERTIFICATEX509REVOCATION
{
  public:
    static CIPHERCERTIFICATEX509REVOCATION_RESULT ValidateOCSP (XBUFFER& response, CIPHERCERTIFICATEX509& certificate, CIPHERCERTIFICATEX509& issuer);
    static CIPHERCERTIFICATEX509REVOCATION_RESULT ValidateCRL  (XBUFFER& CRL, CIPHERCERTIFICATEX509& certificate, CIPHERCERTIFICATEX509& issuer);
};
