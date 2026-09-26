/**-------------------------------------------------------------------------------------------------------------------
*
* @file       DIOStreamTLS12HandshakeClient.h
*
* @class      DIOSTREAMTLS12HANDSHAKECLIENT
* @brief      Data Input/Output Stream TLS 1.2 (RFC 5246) Client Handshake class
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

// PARALLEL to DIOStreamTLS13HandshakeClient.h (TLS 1.3). It owns its DIOSTREAMTLS12SESSION directly (TLS 1.2 does
// not need any key material before ServerHello, unlike TLS 1.3, so there is no reason to construct the session
// any earlier). Scope: ECDHE key exchange only (RFC 4492 / RFC 8422), AEAD-GCM suites and the RSA-PSS,
// RSA-PKCS#1-v1.5/SHA-2 and ECDSA ServerKeyExchange schemes enabled by DIOSTREAMTLSCONFIG — no client
// certificates, no session resumption, no renegotiation.

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOStreamTLS12Session.h"
#include "DIOStreamTLS12Messages.h"
#include "DIOStreamTLSConfig.h"

#include "CipherCertificateX509Validator.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum DIOSTREAMTLS12HANDSHAKECLIENT_STATE
{
  DIOSTREAMTLS12HANDSHAKECLIENT_STATE_NONE                       = 0 ,
  DIOSTREAMTLS12HANDSHAKECLIENT_STATE_WAIT_SERVERHELLO              ,
  DIOSTREAMTLS12HANDSHAKECLIENT_STATE_WAIT_CERTIFICATE              ,
  DIOSTREAMTLS12HANDSHAKECLIENT_STATE_WAIT_SERVERKEYEXCHANGE        ,
  DIOSTREAMTLS12HANDSHAKECLIENT_STATE_WAIT_SERVERHELLODONE          ,
  DIOSTREAMTLS12HANDSHAKECLIENT_STATE_READY_CLIENTFLIGHT            ,
  DIOSTREAMTLS12HANDSHAKECLIENT_STATE_WAIT_CHANGECIPHERSPEC         ,
  DIOSTREAMTLS12HANDSHAKECLIENT_STATE_WAIT_FINISHED                 ,
  DIOSTREAMTLS12HANDSHAKECLIENT_STATE_HANDSHAKE_COMPLETED           ,
  DIOSTREAMTLS12HANDSHAKECLIENT_STATE_ERROR                         ,
};

enum DIOSTREAMTLS12HANDSHAKECLIENT_AUTHENTICATIONERROR
{
  DIOSTREAMTLS12HANDSHAKECLIENT_AUTHENTICATIONERROR_NONE             = 0 ,
  DIOSTREAMTLS12HANDSHAKECLIENT_AUTHENTICATIONERROR_CONFIGURATION        ,
  DIOSTREAMTLS12HANDSHAKECLIENT_AUTHENTICATIONERROR_CERTIFICATE          ,
  DIOSTREAMTLS12HANDSHAKECLIENT_AUTHENTICATIONERROR_SERVERKEYEXCHANGE    ,

  // RFC 8446 4.1.3 downgrade sentinel found in ServerHello.random: a TLS 1.3-capable server is required to mark
  // any negotiation it settles below TLS 1.3 with these last 8 bytes, precisely so a client that might have
  // wanted 1.3 can detect an on-path attacker forcing it down. Phase 6 hardening: see ServerHello_Process().
  DIOSTREAMTLS12HANDSHAKECLIENT_AUTHENTICATIONERROR_DOWNGRADEDETECTED    ,
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOSTREAMTLS12HANDSHAKECLIENT
{
  public:
                                            DIOSTREAMTLS12HANDSHAKECLIENT                    ();
    virtual                                ~DIOSTREAMTLS12HANDSHAKECLIENT                    ();

    bool                                    Ini                                              (bool checkdowngradesentinel = false);
    bool                                    Capabilities_Set                                 (DIOSTREAMTLSCONFIG* config);
    void                                    End                                              ();
    bool                                    IsIni                                            ();

    DIOSTREAMTLS12SESSION*                  GetSession                                       ();

    DIOSTREAMTLS12HANDSHAKECLIENT_STATE     GetState                                         ();
    bool                                    IsHandshakeCompleted                             ();
    bool                                    IsServerAuthenticated                            ();

    bool                                    Authentication_Set                               (XCHAR* servername, XVECTOR<XBUFFER*>* trustedroots, XDATETIME* datetime = NULL);
    void                                    AIAFetch_Set                                      (bool active, int timeout);
    void                                    ValidationPolicy_Set                              (CIPHERCERTIFICATEX509VALIDATIONPOLICY& policy);
    void                                    RevocationLists_Set                               (XVECTOR<XBUFFER*>* CRLs);
    void                                    OCSPDirect_Set                                    (DIOSTREAMTLS_OCSPDIRECTFETCHER fetcher, void* context);
    DIOSTREAMTLS12HANDSHAKECLIENT_AUTHENTICATIONERROR GetAuthenticationError                 ();
    CIPHERCERTIFICATEX509VALIDATOR_ERROR    GetCertificateValidationError                    ();

    DIOSTREAMTLS12_MSG_CERTIFICATE*         GetServerCertificate                            ();
    DIOSTREAMTLS12_MSG_SERVERKEYEXCHANGE_ECDHE* GetServerKeyExchange                        ();

    bool                                    CipherSuitesAndSchemes_WidenECDSA                ();

    bool                                    IsAlgorithmRejected                              ();

    bool                                    ClientHello_Create                               (XCHAR* servername, XBUFFER& clienthello, XBUFFER& records);

    bool                                    RecordInput_Add                                  (XBYTE* data, XDWORD size);
    bool                                    RecordInput_Add                                  (XBUFFER& data);
    bool                                    Process                                          ();

    bool                                    ClientFlight_Create                              (XBUFFER& records);

  private:

    bool                                    Handshake_Process                                (XBUFFER& message);

    bool                                    ServerHello_Process                              (XBUFFER& message);
    bool                                    Certificate_Process                              (XBUFFER& message);
    bool                                    ServerKeyExchange_Process                        (XBUFFER& message);
    bool                                    ServerHelloDone_Process                          (XBUFFER& message);
    bool                                    Finished_Process                                 (XBUFFER& message);

    bool                                    CertificateChain_CompleteViaAIA                   (XVECTOR<XBUFFER*>& certificatechain);

    bool                                    SetError                                         ();
    bool                                    SetAuthenticationError                           (DIOSTREAMTLS12HANDSHAKECLIENT_AUTHENTICATIONERROR error);
    bool                                    CipherSuite_IsOffered                            (XWORD ciphersuite);
    bool                                    SupportedGroup_IsOffered                         (XWORD supportedgroup);
    bool                                    SignatureScheme_IsOffered                        (XWORD signaturescheme);
    void                                    Clean                                            ();

    DIOSTREAMTLS12SESSION                   session;
    DIOSTREAMTLSCONFIG*                     config;
    DIOSTREAMTLS12HANDSHAKECLIENT_STATE     state;
    bool                                    isini;
    bool                                    authenticationconfigured;
    bool                                    serverauthenticated;
    bool                                    checkdowngradesentinel;
    bool                                    algorithmrejected;

    DIOSTREAMTLS12HANDSHAKECLIENT_AUTHENTICATIONERROR authenticationerror;
    CIPHERCERTIFICATEX509VALIDATOR_ERROR    certificatevalidationerror;

    XSTRING                                 expectedservername;
    XVECTOR<XBUFFER*>                       trustedroots;
    XDATETIME                                validationdatetime;
    bool                                    hasvalidationdatetime;

    bool                                    aiafetchactive;
    int                                     aiafetchtimeout;

    CIPHERCERTIFICATEX509VALIDATOR          certificatevalidator;
    XVECTOR<XBUFFER*>*                      revocationlists;
    DIOSTREAMTLS_OCSPDIRECTFETCHER          ocspdirectfetcher;
    void*                                   ocspdirectcontext;

    XVECTOR<XWORD>                          ciphersuites;
    XVECTOR<XWORD>                          supportedgroups;
    XVECTOR<XWORD>                          signatureschemes;

    XBUFFER                                 clientrandom;
    XBUFFER                                 serverrandom;

    XWORD                                   servergroup;
    XBUFFER                                 clientkeyshare;
    XSECUREBUFFER                           premastersecret;

    DIOSTREAMTLS12_MSG_CERTIFICATE*         servercertificate;
    DIOSTREAMTLS12_MSG_SERVERKEYEXCHANGE_ECDHE* serverkeyexchange;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
