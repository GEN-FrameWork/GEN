/**-------------------------------------------------------------------------------------------------------------------
*
* @file       DIOStreamTLS13KeySchedule.h
*
* @class      DIOSTREAMTLS13KEYSCHEDULE
* @brief      Data Input/Output Stream TLS Key Schedule (TLS 1.3, RFC 8446 section 7.1) class
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

#include "Hash.h"
#include "CipherHKDF.h"

#include "DIOStreamTLSMessages.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


#define DIOSTREAMTLS13KEYSCHEDULE_IVSIZE                        12                // Fixed by RFC 8446, section 5.3, for every cipher suite

// Dumping key material to the trace is only ever a debugging aid, for example to follow the trace of the RFC 8448.
// It must never be left active in a delivered build: it writes the secrets of the connection in the clear.
//#define DIOSTREAMTLS13KEYSCHEDULE_TRACE_SECRETS

#define DIOSTREAMTLS13KEYSCHEDULE_LABEL_DERIVED                 __L("derived")
#define DIOSTREAMTLS13KEYSCHEDULE_LABEL_CLIENTEARLY             __L("c e traffic")
#define DIOSTREAMTLS13KEYSCHEDULE_LABEL_CLIENTHANDSHAKE         __L("c hs traffic")
#define DIOSTREAMTLS13KEYSCHEDULE_LABEL_SERVERHANDSHAKE         __L("s hs traffic")
#define DIOSTREAMTLS13KEYSCHEDULE_LABEL_CLIENTAPPLICATION       __L("c ap traffic")
#define DIOSTREAMTLS13KEYSCHEDULE_LABEL_SERVERAPPLICATION       __L("s ap traffic")
#define DIOSTREAMTLS13KEYSCHEDULE_LABEL_RESUMPTION              __L("res master")
#define DIOSTREAMTLS13KEYSCHEDULE_LABEL_RESUMPTIONPSK           __L("resumption")
#define DIOSTREAMTLS13KEYSCHEDULE_LABEL_RESUMPTIONBINDER        __L("res binder")
#define DIOSTREAMTLS13KEYSCHEDULE_LABEL_KEY                     __L("key")
#define DIOSTREAMTLS13KEYSCHEDULE_LABEL_IV                      __L("iv")
#define DIOSTREAMTLS13KEYSCHEDULE_LABEL_FINISHED                __L("finished")
#define DIOSTREAMTLS13KEYSCHEDULE_LABEL_TRAFFICUPDATE           __L("traffic upd")

// DIOSTREAMTLSKEYSCHEDULE_ROLE and _DIRECTION/_MAXDIRECTIONS moved to DIOStreamTLSMessages.h: they are shared
// vocabulary with the TLS 1.2 key schedule (DIOSTREAMTLS12KEYSCHEDULE), not exclusive to this TLS 1.3 one. Every
// secret here is still asked for by direction, never by role -- that's what lets the server-role build of this
// class reuse it untouched, only the role given to Ini() changes.

enum DIOSTREAMTLS13KEYSCHEDULE_LEVEL
{
  DIOSTREAMTLS13KEYSCHEDULE_LEVEL_NONE                = 0 ,
  DIOSTREAMTLS13KEYSCHEDULE_LEVEL_EARLY                  ,
  DIOSTREAMTLS13KEYSCHEDULE_LEVEL_HANDSHAKE               ,
  DIOSTREAMTLS13KEYSCHEDULE_LEVEL_APPLICATION             ,
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOSTREAMTLS13KEYSCHEDULE
{
  public:
                                            DIOSTREAMTLS13KEYSCHEDULE                           ();
    virtual                                ~DIOSTREAMTLS13KEYSCHEDULE                           ();

    bool                                    Ini                                               (XWORD ciphersuite, DIOSTREAMTLSKEYSCHEDULE_ROLE role);
    void                                    End                                               ();

    bool                                    IsIni                                             ();

    XWORD                                   GetCipherSuite                                    ();
    DIOSTREAMTLSKEYSCHEDULE_ROLE            GetRole                                           ();

    XDWORD                                  GetHashSize                                       ();
    XDWORD                                  GetKeySize                                        ();
    XDWORD                                  GetIVSize                                         ();

    HASH*                                   GetHash                                           ();
    CIPHERHKDF*                             GetHKDF                                           ();

    bool                                    TranscriptHash                                    (XBUFFER& messages, XBUFFER& transcripthash);

    bool                                    EarlySecret_Calculate                             (XBUFFER* PSK = NULL);
    bool                                    HandshakeSecret_Calculate                         (XBUFFER& sharedsecret);
    bool                                    MasterSecret_Calculate                            ();

    bool                                    EarlyTrafficSecret_Calculate                     (XBUFFER& transcripthash);
    bool                                    HandshakeTrafficSecrets_Calculate                 (XBUFFER& transcripthash);
    bool                                    ApplicationTrafficSecrets_Calculate               (XBUFFER& transcripthash);
    bool                                    ResumptionSecret_Calculate                        (XBUFFER& transcripthash);
    bool                                    ResumptionPSK_Calculate                           (XBUFFER& ticketnonce, XBUFFER& PSK);
    bool                                    Binder_Calculate                                  (XBUFFER& transcript, XBUFFER& binder);

    XBUFFER*                                GetEarlySecret                                    ();
    XBUFFER*                                GetHandshakeSecret                                ();
    XBUFFER*                                GetMasterSecret                                   ();
    XBUFFER*                                GetResumptionSecret                               ();

    XBUFFER*                                GetTrafficSecret                                  (DIOSTREAMTLS13KEYSCHEDULE_LEVEL level, DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction);

    bool                                    GetTrafficKeys                                    (DIOSTREAMTLS13KEYSCHEDULE_LEVEL level, DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction, XBUFFER& key, XBUFFER& IV);
    bool                                    GetFinishedKey                                    (DIOSTREAMTLS13KEYSCHEDULE_LEVEL level, DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction, XBUFFER& finishedkey);

    bool                                    CalculateFinished                                 (DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction, XBUFFER& transcripthash, XBUFFER& verifydata);
    bool                                    VerifyFinished                                    (DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction, XBUFFER& transcripthash, XBUFFER& verifydata);

    bool                                    UpdateTrafficSecret                               (DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction);

  private:

    bool                                    IsDirectionOfTheClient                            (DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction);
    bool                                    DeriveEmptySecret                                 (XBUFFER& secret, XCHAR* label, XBUFFER& derivedsecret);

    void                                    Clean                                             ();

    XWORD                                   ciphersuite;
    DIOSTREAMTLSKEYSCHEDULE_ROLE            role;
    bool                                    isini;

    XDWORD                                  hashsize;
    XDWORD                                  keysize;
    XDWORD                                  IVsize;

    HASH*                                   hash;
    CIPHERHKDF*                             HKDF;

    XSECUREBUFFER                           earlysecret;
    XSECUREBUFFER                           handshakesecret;
    XSECUREBUFFER                           mastersecret;
    XSECUREBUFFER                           resumptionsecret;

    XSECUREBUFFER                           clientearlytrafficsecret;
    XSECUREBUFFER                           clienthandshaketrafficsecret;
    XSECUREBUFFER                           serverhandshaketrafficsecret;
    XSECUREBUFFER                           clientapplicationtrafficsecret;
    XSECUREBUFFER                           serverapplicationtrafficsecret;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/


