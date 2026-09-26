/**-------------------------------------------------------------------------------------------------------------------
*
* @file       DIOStreamTLS12Session.h
*
* @class      DIOSTREAMTLS12SESSION
* @brief      Data Input/Output Stream TLS 1.2 (RFC 5246) role-neutral Session class
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

// PARALLEL to DIOStreamTLS13Session.h (TLS 1.3): same responsibilities (ephemeral ECDHE key exchange, record
// input/handshake input accumulation, transcript, record protection), but built on DIOSTREAMTLS12KEYSCHEDULE
// and DIOSTREAMTLS12RECORD instead. Nothing here is shared state with the TLS 1.3 session; the two can run
// side by side. The ECDHE cipher primitives (CIPHERECDSAX25519 / CIPHERECDSA) are generic, version-agnostic
// crypto and are reused directly, exactly like the TLS 1.3 session already does.

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XBuffer.h"

#include "CipherECDSA.h"
#include "CipherECDSAX25519.h"

#include "DIOStreamTLS12KeySchedule.h"
#include "DIOStreamTLS12Record.h"


/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOSTREAMTLS12SESSION_MAXHANDSHAKESIZE                  DIOSTREAMTLS_MSG_MAXHANDSHAKESIZE
#define DIOSTREAMTLS12SESSION_MAXRECORDINPUTSIZE                (4*1024*1024)

enum DIOSTREAMTLS12SESSION_RESULT
{
  DIOSTREAMTLS12SESSION_RESULT_ERROR       = -1 ,
  DIOSTREAMTLS12SESSION_RESULT_INCOMPLETE  =  0 ,
  DIOSTREAMTLS12SESSION_RESULT_COMPLETE         ,
};

class DIOSTREAMTLSMEMORYPOLICY;

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOSTREAMTLS12SESSION
{
  public:
                                            DIOSTREAMTLS12SESSION                             ();
    virtual                                ~DIOSTREAMTLS12SESSION                             ();

    bool                                    Ini                                               (XWORD ciphersuite, DIOSTREAMTLSKEYSCHEDULE_ROLE role);
    bool                                    MemoryPolicy_Set                                  (DIOSTREAMTLSMEMORYPOLICY& policy);
    void                                    End                                               ();
    bool                                    IsIni                                             ();

    DIOSTREAMTLSKEYSCHEDULE_ROLE            GetRole                                           ();

    DIOSTREAMTLS12KEYSCHEDULE*              GetKeySchedule                                    ();
    DIOSTREAMTLS12RECORD*                   GetRecord                                         ();

    bool                                    KeyExchange_Generate                              (XWORD group, XBUFFER& publickey);
    bool                                    KeyExchange_SharedSecret                          (XWORD group, XBUFFER& publickey, XBUFFER& sharedsecret);
    void                                    KeyExchange_Delete                                ();

    bool                                    Keys_Activate                                     (XBUFFER& premastersecret, XBUFFER& clientrandom, XBUFFER& serverrandom, XBUFFER* sessionhash = NULL);

    XBUFFER*                                GetRecordInput                                    ();
    bool                                    RecordInput_Add                                  (XBYTE* data, XDWORD size);
    bool                                    RecordInput_Add                                  (XBUFFER& data);
    DIOSTREAMTLS12SESSION_RESULT            Record_Extract                                    (DIOSTREAMTLS_CONTENTTYPE& contenttype, XBUFFER& plain);
    DIOSTREAMTLS_ALERT_DESCRIPTION          GetLastRecordAlertDescription                     ();

    XBUFFER*                                GetHandshakeInput                                 ();
    bool                                    HandshakeInput_Add                               (XBYTE* data, XDWORD size);
    bool                                    HandshakeInput_Add                               (XBUFFER& data);
    DIOSTREAMTLS12SESSION_RESULT            Handshake_Extract                                (XBUFFER& message);

    XBUFFER*                                GetTranscript                                    ();
    bool                                    Transcript_Add                                   (XBUFFER& message);
    bool                                    TranscriptHash                                   (XBUFFER& transcripthash);

    XBUFFER*                                GetApplicationInput                              ();
    bool                                    ApplicationData_CanProtect                       (XDWORD size);
    bool                                    ApplicationData_Protect                          (XBYTE* data, XDWORD size, XBUFFER& records);
    bool                                    ApplicationData_Protect                          (XBUFFER& data, XBUFFER& records);
    XDWORD                                  ApplicationData_Read                             (XBYTE* data, XDWORD size);
    DIOSTREAMTLS12SESSION_RESULT            ApplicationData_Process                          ();

    bool                                    Alert_Create                                     (DIOSTREAMTLS_ALERT_LEVEL level, DIOSTREAMTLS_ALERT_DESCRIPTION description, XBUFFER& records);
    bool                                    CloseNotify_Create                               (XBUFFER& records);

    bool                                    IsCloseNotifySent                                ();
    bool                                    IsCloseNotifyReceived                            ();
    bool                                    IsError                                          ();
    bool                                    IsTransportClosedWithoutNotify                   ();
    bool                                    TransportClosed                                  ();

  private:

    void                                    Clean                                             ();

    DIOSTREAMTLSKEYSCHEDULE_ROLE            role;
    bool                                    isini;
    bool                                    keysactivated;

    DIOSTREAMTLS12KEYSCHEDULE               keyschedule;
    DIOSTREAMTLS12RECORD                    record;
    CIPHERECDSAX25519                       keyexchange;
    CIPHERECDSA                             keyexchangep256;
    XSECUREBUFFER                           keyexchangep256private;
    XBUFFER                                 keyexchangep256public;
    CIPHERECDSA                             keyexchangep384;
    XSECUREBUFFER                           keyexchangep384private;
    XBUFFER                                 keyexchangep384public;
    CIPHERECDSA                             keyexchangep521;
    XSECUREBUFFER                           keyexchangep521private;
    XBUFFER                                 keyexchangep521public;

    XBUFFER                                 recordinput;
    DIOSTREAMTLS_ALERT_DESCRIPTION          lastrecordalertdescription;
    XBUFFER                                 handshakeinput;
    XBUFFER                                 transcript;
    XBUFFER                                 applicationinput;

    XDWORD                                  maximumrecordinputsize;
    XDWORD                                  maximumhandshakeinputsize;
    XDWORD                                  maximumtranscriptsize;
    XDWORD                                  maximumapplicationinputsize;

    bool                                    iserror;
    bool                                    closenotifysent;
    bool                                    closenotifyreceived;
    bool                                    transportclosedwithoutnotify;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
