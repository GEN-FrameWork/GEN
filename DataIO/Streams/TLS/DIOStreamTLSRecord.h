/**-------------------------------------------------------------------------------------------------------------------
*
* @file       DIOStreamTLSRecord.h
*
* @class      DIOSTREAMTLSRECORD
* @brief      Data Input/Output Stream TLS Record Layer (TLS 1.3, RFC 8446 section 5) class
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

#include "CipherAESGCM.h"

#ifdef CIPHER_SYMMETRIC_CHACHA20POLY1305_ACTIVE
#include "CipherChaCha20Poly1305.h"
#endif

#include "CipherKeySymmetrical.h"

#include "DIOStreamTLSMessages.h"
#include "DIOStreamTLS13KeySchedule.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOSTREAMTLSRECORD_MAXPLAINSIZE                       16384             // 2^14, the limit set by RFC 8446 section 5.1
#define DIOSTREAMTLSRECORD_MAXCIPHERSIZE                      (16384 + 256)     // The limit for a protected record, section 5.2

#define DIOSTREAMTLSRECORD_LEGACYVERSION                      DIOSTREAMTLS_MSG_VERSION_TLS_1_2

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOSTREAMTLSRECORD
{
  public:
                                            DIOSTREAMTLSRECORD                                ();
    virtual                                ~DIOSTREAMTLSRECORD                                ();

    bool                                    Ini                                               (DIOSTREAMTLS13KEYSCHEDULE* keyschedule);
    void                                    End                                               ();

    bool                                    IsIni                                             ();

    bool                                    SetKeys                                           (DIOSTREAMTLS13KEYSCHEDULE_LEVEL level, DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction);
    bool                                    ClearKeys                                         (DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction);
    bool                                    IsProtected                                       (DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction);

    XQWORD                                  GetSequence                                       (DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction);
    bool                                    ResetSequence                                     (DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction);

    XWORD                                   GetMaxPlainSize                                   ();
    bool                                    SetMaxPlainSize                                   (XWORD maxplainsize);

    XBYTE                                   GetPaddingSize                                    ();
    void                                    SetPaddingSize                                    (XBYTE paddingsize);

    bool                                    Protect                                           (DIOSTREAMTLS_CONTENTTYPE contenttype, XBYTE* plain, XDWORD size, XBUFFER& records);
    bool                                    Protect                                           (DIOSTREAMTLS_CONTENTTYPE contenttype, XBUFFER& plain, XBUFFER& records);

    bool                                    Unprotect                                         (XBUFFER& record, DIOSTREAMTLS_CONTENTTYPE& contenttype, XBUFFER& plain);
    DIOSTREAMTLS_ALERT_DESCRIPTION           GetLastAlertDescription                           ();

    static bool                             Record_Extract                                    (XBUFFER& input, XBUFFER& record);

  private:

    bool                                    CalculateNonce                                    (DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction, XBUFFER& nonce);
    bool                                    Protect_OneRecord                                 (DIOSTREAMTLS_CONTENTTYPE contenttype, XBYTE* plain, XWORD size, XBUFFER& records);

    void                                    Clean                                             ();

    DIOSTREAMTLS13KEYSCHEDULE*                keyschedule;

    CIPHER*                                 cipher[DIOSTREAMTLSKEYSCHEDULE_MAXDIRECTIONS];
    CIPHERKEYSYMMETRICAL*                   key[DIOSTREAMTLSKEYSCHEDULE_MAXDIRECTIONS];
    XBUFFER                                 IV[DIOSTREAMTLSKEYSCHEDULE_MAXDIRECTIONS];
    XQWORD                                  sequence[DIOSTREAMTLSKEYSCHEDULE_MAXDIRECTIONS];
    bool                                    isprotected[DIOSTREAMTLSKEYSCHEDULE_MAXDIRECTIONS];

    XWORD                                   maxplainsize;
    XBYTE                                   paddingsize;
    bool                                    isini;
    DIOSTREAMTLS_ALERT_DESCRIPTION          lastalertdescription;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/


