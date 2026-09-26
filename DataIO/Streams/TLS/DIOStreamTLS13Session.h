/**-------------------------------------------------------------------------------------------------------------------
*
* @file       DIOStreamTLS13Session.h
*
* @class      DIOSTREAMTLS13SESSION
* @brief      Data Input/Output Stream TLS 1.3 role-neutral Session class
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

#include "CipherECDSA.h"
#include "CipherECDSAX25519.h"

#ifdef CIPHER_ASYMMETRIC_MLKEM768_ACTIVE
#include "CipherSecP256r1MLKEM768.h"
#endif

#ifdef CIPHER_ASYMMETRIC_MLKEM1024_ACTIVE
#include "CipherSecP384r1MLKEM1024.h"
#endif

#if defined(CIPHER_ASYMMETRIC_X25519_ACTIVE) && defined(CIPHER_ASYMMETRIC_MLKEM768_ACTIVE)
#include "CipherX25519MLKEM768.h"
#endif

#include "DIOStreamTLS13KeySchedule.h"
#include "DIOStreamTLSRecord.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


#define DIOSTREAMTLS13SESSION_MAXHANDSHAKESIZE                    DIOSTREAMTLS_MSG_MAXHANDSHAKESIZE
#define DIOSTREAMTLS13SESSION_MAXRECORDINPUTSIZE                  (4*1024*1024)
// Applies to both directions: it bounds keyupdates[DIOSTREAMTLSKEYSCHEDULE_DIRECTION_LOCAL] (local KeyUpdate count,
// see KeyUpdate_Send()) and keyupdates[DIOSTREAMTLSKEYSCHEDULE_DIRECTION_REMOTE] (remote KeyUpdate count, see
// KeyUpdate_Process()) alike -- the same 2^48-1 ceiling is correct for either direction, only the name used to imply
// local-only.
#define DIOSTREAMTLS13SESSION_MAXKEYUPDATES                       ((((XQWORD)1) << 48) - 1)


enum DIOSTREAMTLS13SESSION_RESULT
{
  DIOSTREAMTLS13SESSION_RESULT_ERROR       = -1 ,
  DIOSTREAMTLS13SESSION_RESULT_INCOMPLETE  =  0 ,
  DIOSTREAMTLS13SESSION_RESULT_COMPLETE         ,
};

enum DIOSTREAMTLS13SESSION_EPOCH
{
  DIOSTREAMTLS13SESSION_EPOCH_CLEAR        = 0 ,
  DIOSTREAMTLS13SESSION_EPOCH_EARLY            ,
  DIOSTREAMTLS13SESSION_EPOCH_HANDSHAKE        ,
  DIOSTREAMTLS13SESSION_EPOCH_APPLICATION      ,
};

class DIOSTREAMTLSMEMORYPOLICY;

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOSTREAMTLS13SESSION
{
  public:
                                            DIOSTREAMTLS13SESSION                              ();
    virtual                                ~DIOSTREAMTLS13SESSION                              ();

    bool                                    Ini                                              (XWORD ciphersuite, DIOSTREAMTLSKEYSCHEDULE_ROLE role);
    bool                                    MemoryPolicy_Set                                 (DIOSTREAMTLSMEMORYPOLICY& policy);
    void                                    End                                              ();
    bool                                    IsIni                                            ();

    DIOSTREAMTLSKEYSCHEDULE_ROLE            GetRole                                          ();
    DIOSTREAMTLS13SESSION_EPOCH               GetEpoch                                         (DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction);

    DIOSTREAMTLS13KEYSCHEDULE*                GetKeySchedule                                   ();
    DIOSTREAMTLSRECORD*                     GetRecord                                         ();
    CIPHERECDSAX25519*                      GetKeyExchange                                    ();
    bool                                    KeyExchange_Generate                              (XWORD group, XBUFFER& publickey);
    bool                                    KeyExchange_SharedSecret                          (XWORD group, XBUFFER& publickey, XBUFFER& sharedsecret);
    bool                                    KeyExchange_ServerGenerate                        (XWORD group, XBUFFER& peerpublickey, XBUFFER& publickey, XBUFFER& sharedsecret, bool& invalidpeershare);
    void                                    KeyExchange_Delete                                ();
    bool                                    CipherSuite_Select                                (XWORD ciphersuite);

    XBUFFER*                                GetRecordInput                                    ();
    bool                                    RecordInput_Add                                  (XBYTE* data, XDWORD size);
    bool                                    RecordInput_Add                                  (XBUFFER& data);
    DIOSTREAMTLS13SESSION_RESULT              Record_Extract                                   (DIOSTREAMTLS_CONTENTTYPE& contenttype, XBUFFER& plain);
    DIOSTREAMTLS_ALERT_DESCRIPTION          GetLastRecordAlertDescription                     ();

    XBUFFER*                                GetHandshakeInput                                 ();
    bool                                    HandshakeInput_Add                               (XBYTE* data, XDWORD size);
    bool                                    HandshakeInput_Add                               (XBUFFER& data);
    DIOSTREAMTLS13SESSION_RESULT              Handshake_Extract                                (XBUFFER& message);

    XBUFFER*                                GetTranscript                                    ();
    bool                                    Transcript_Add                                   (XBUFFER& message);
    bool                                    TranscriptHash                                   (XBUFFER& transcripthash);

    bool                                    EarlyKeys_Activate                               (XBUFFER& PSK, XBUFFER& clienthello, DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction);
    bool                                    EarlyData_Protect                                (XBYTE* data, XDWORD size, XBUFFER& records);
    bool                                    EarlyData_Protect                                (XBUFFER& data, XBUFFER& records);
    XDWORD                                  EarlyData_Read                                   (XBYTE* data, XDWORD size);
    XDWORD                                  GetEarlyDataSize                                 ();
    bool                                    EarlyData_Commit                                  ();
    void                                    EarlyData_End                                    (DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction);
    void                                    EarlyKeys_Deactivate                             (DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction);
    void                                    EarlyData_Accepted                               (bool accepted);
    bool                                    EarlyData_Limit                                  (XDWORD maximumsize);
    bool                                    HandshakeKeys_Activate                           (XBUFFER& sharedsecret, XBUFFER* PSK = NULL);
    bool                                    ApplicationTrafficSecrets_Calculate              ();
    bool                                    ApplicationKeys_Activate                         (DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction);

    XBUFFER*                                GetApplicationInput                              ();
    bool                                    ApplicationData_Protect                          (XBYTE* data, XDWORD size, XBUFFER& records);
    bool                                    ApplicationData_Protect                          (XBUFFER& data, XBUFFER& records);
    XDWORD                                  ApplicationData_Read                             (XBYTE* data, XDWORD size);
    DIOSTREAMTLS13SESSION_RESULT              ApplicationData_Process                          ();
    bool                                    KeyUpdate_Create                                 (bool requestpeer, XBUFFER& records);
    bool                                    PostHandshakeOutput_Add                           (XBUFFER& records);
    bool                                    PostHandshakeOutput_Extract                       (XBUFFER& records);
    bool                                    NewSessionTicket_Extract                          (XBUFFER& message);

    bool                                    Alert_Create                                     (DIOSTREAMTLS_ALERT_LEVEL level, DIOSTREAMTLS_ALERT_DESCRIPTION description, XBUFFER& records);
    bool                                    CloseNotify_Create                               (XBUFFER& records);

    bool                                    IsCloseNotifySent                                ();
    bool                                    IsCloseNotifyReceived                            ();
    bool                                    IsError                                          ();
    bool                                    IsTransportClosedWithoutNotify                   ();
    DIOSTREAMTLS_ALERT_LEVEL                GetReceivedAlertLevel                            ();
    DIOSTREAMTLS_ALERT_DESCRIPTION          GetReceivedAlertDescription                      ();

    bool                                    TransportClosed                                  ();

  private:

    bool                                    KeyUpdate_Process                                (DIOSTREAMTLS_MSG_HANDSHAKE& handshake);
    void                                    Clean                                            ();

    DIOSTREAMTLSKEYSCHEDULE_ROLE            role;
    bool                                    isini;
    bool                                    applicationsecretscalculated;

    DIOSTREAMTLS13SESSION_EPOCH               epoch[DIOSTREAMTLSKEYSCHEDULE_MAXDIRECTIONS];

    DIOSTREAMTLS13KEYSCHEDULE                 keyschedule;
    DIOSTREAMTLSRECORD                      record;
    DIOSTREAMTLSRECORD                      earlyrecord;
    CIPHERECDSAX25519                       keyexchange;

    #ifdef CIPHER_ASYMMETRIC_MLKEM768_ACTIVE
    CIPHERSECP256R1MLKEM768                 keyexchangesecp256r1mlkem768;
    #endif

    #ifdef CIPHER_ASYMMETRIC_MLKEM1024_ACTIVE
    CIPHERSECP384R1MLKEM1024                keyexchangesecp384r1mlkem1024;
    #endif

    #if defined(CIPHER_ASYMMETRIC_X25519_ACTIVE) && defined(CIPHER_ASYMMETRIC_MLKEM768_ACTIVE)
    CIPHERX25519MLKEM768                    keyexchangex25519mlkem768;
    #endif

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
    XBUFFER                                 earlydatainput;
    XBUFFER                                 posthandshakeoutput;
    XBUFFER                                 newsessionticketinput;

    XDWORD                                  maximumrecordinputsize;
    XDWORD                                  maximumhandshakeinputsize;
    XDWORD                                  maximumtranscriptsize;
    XDWORD                                  maximumapplicationinputsize;

    XDWORD                                  maximumearlydatasize;
    XDWORD                                  earlydatareceived;
    XDWORD                                  earlydatasent;
    bool                                    earlydataaccepted;
    XQWORD                                  keyupdates[DIOSTREAMTLSKEYSCHEDULE_MAXDIRECTIONS];
    bool                                    keyupdaterequestpending;
    bool                                    keyupdateresponsepending;

    bool                                    closenotifysent;
    bool                                    closenotifyreceived;
    bool                                    iserror;
    bool                                    transportclosedwithoutnotify;
    DIOSTREAMTLS_ALERT_LEVEL                receivedalertlevel;
    DIOSTREAMTLS_ALERT_DESCRIPTION          receivedalertdescription;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
