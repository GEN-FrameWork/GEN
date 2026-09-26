/**-------------------------------------------------------------------------------------------------------------------
*
* @file       DIOStreamTLS12Record.h
*
* @class      DIOSTREAMTLS12RECORD
* @brief      Data Input/Output Stream TLS 1.2 record protection class (RFC 5246 + RFC 5288 + RFC 7905)
* @ingroup    DATAIO
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

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XBuffer.h"

#include "DIOStreamTLSMessages.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOSTREAMTLS12RECORD_MAXPLAINSIZE                     16384             // 2^14, RFC 5246 section 6.2.1
#define DIOSTREAMTLS12RECORD_MAXCIPHERSIZE                    (16384 + 2048)    // Plain text plus the AEAD expansion
#define DIOSTREAMTLS12RECORD_EXPLICITNONCESIZE                8                 // AES-GCM GenericAEADCipher.nonce_explicit, RFC 5288
#define DIOSTREAMTLS12RECORD_CHACHA20_EXPLICITNONCESIZE       0                 // RFC 7905
#define DIOSTREAMTLS12RECORD_TAGSIZE                          16
#define DIOSTREAMTLS12RECORD_AADSIZE                          13                // seq(8) + type(1) + version(2) + length(2)

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class CIPHER;
class CIPHERKEYSYMMETRICAL;

class DIOSTREAMTLS12RECORD
{
  public:
                                            DIOSTREAMTLS12RECORD                              ();
    virtual                                ~DIOSTREAMTLS12RECORD                              ();

    bool                                    Ini                                               ();
    void                                    End                                               ();
    bool                                    IsIni                                             ();

    bool                                    SetKeys                                           (DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction, XBUFFER& key, XBUFFER& fixedIV);

    bool                                    IsProtected                                       (DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction);

    XQWORD                                  GetSequence                                       (DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction);
    bool                                    ResetSequence                                     (DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction);

    bool                                    Protect                                           (DIOSTREAMTLS_CONTENTTYPE contenttype, XBYTE* plain, XDWORD size, XBUFFER& records);
    bool                                    Protect                                           (DIOSTREAMTLS_CONTENTTYPE contenttype, XBUFFER& plain, XBUFFER& records);

    bool                                    Unprotect                                         (XBUFFER& record, DIOSTREAMTLS_CONTENTTYPE& contenttype, XBUFFER& plain);
    DIOSTREAMTLS_ALERT_DESCRIPTION          GetLastAlertDescription                           ();

    static bool                             Record_Extract                                    (XBUFFER& input, XBUFFER& record);

  private:

    void                                    Clean                                             ();

    
    
    bool                                    Nonce_Build                                       (DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction, XBYTE* explicitnonce, XBUFFER& nonce);

    
    
    bool                                    AAD_Build                                         (XQWORD sequence, DIOSTREAMTLS_CONTENTTYPE contenttype, XWORD version, XWORD plainlength, XBUFFER& additionaldata);

    bool                                    Protect_OneRecord                                 (DIOSTREAMTLS_CONTENTTYPE contenttype, XBYTE* plain, XWORD size, XBUFFER& records);

    bool                                    isini;

    CIPHER*                                 cipher[DIOSTREAMTLSKEYSCHEDULE_MAXDIRECTIONS];
    CIPHERKEYSYMMETRICAL*                   key[DIOSTREAMTLSKEYSCHEDULE_MAXDIRECTIONS];
    XBUFFER                                 fixedIV[DIOSTREAMTLSKEYSCHEDULE_MAXDIRECTIONS];

    XQWORD                                  sequence[DIOSTREAMTLSKEYSCHEDULE_MAXDIRECTIONS];
    bool                                    isprotected[DIOSTREAMTLSKEYSCHEDULE_MAXDIRECTIONS];
    DIOSTREAMTLS_ALERT_DESCRIPTION          lastalertdescription;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
