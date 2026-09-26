/**-------------------------------------------------------------------------------------------------------------------
*
* @file       DIOStreamTLS13HandshakeClient.h
*
* @class      DIOSTREAMTLS13HANDSHAKECLIENT
* @brief      Data Input/Output Stream TLS 1.3 Client Handshake class
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

#include "DIOStreamTLS13Session.h"
#include "DIOStreamTLSMessagesHandShakeServerFlight.h"

#include "CipherCertificateX509Validator.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum DIOSTREAMTLS13HANDSHAKECLIENT_STATE
{
  DIOSTREAMTLS13HANDSHAKECLIENT_STATE_NONE                       = 0 ,
  DIOSTREAMTLS13HANDSHAKECLIENT_STATE_WAIT_SERVERHELLO               ,
  DIOSTREAMTLS13HANDSHAKECLIENT_STATE_WAIT_ENCRYPTEDEXTENSIONS       ,
  DIOSTREAMTLS13HANDSHAKECLIENT_STATE_WAIT_CERTIFICATE               ,
  DIOSTREAMTLS13HANDSHAKECLIENT_STATE_WAIT_CERTIFICATEVERIFY         ,
  DIOSTREAMTLS13HANDSHAKECLIENT_STATE_WAIT_FINISHED                  ,
  DIOSTREAMTLS13HANDSHAKECLIENT_STATE_SERVERFINISHED_VERIFIED        ,
  DIOSTREAMTLS13HANDSHAKECLIENT_STATE_HANDSHAKE_COMPLETED            ,
  DIOSTREAMTLS13HANDSHAKECLIENT_STATE_ERROR                          ,
};

enum DIOSTREAMTLS13HANDSHAKECLIENT_AUTHENTICATIONERROR
{
  DIOSTREAMTLS13HANDSHAKECLIENT_AUTHENTICATIONERROR_NONE             = 0 ,
  DIOSTREAMTLS13HANDSHAKECLIENT_AUTHENTICATIONERROR_CONFIGURATION        ,
  DIOSTREAMTLS13HANDSHAKECLIENT_AUTHENTICATIONERROR_CERTIFICATE          ,
  DIOSTREAMTLS13HANDSHAKECLIENT_AUTHENTICATIONERROR_CERTIFICATEVERIFY    ,
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOSTREAMTLSCONFIG;

class DIOSTREAMTLS13HANDSHAKECLIENT
{
  public:
                                            DIOSTREAMTLS13HANDSHAKECLIENT                      ();
    virtual                                ~DIOSTREAMTLS13HANDSHAKECLIENT                      ();

    bool                                    Ini                                              (DIOSTREAMTLS13SESSION* session);
    void                                    End                                              ();
    bool                                    IsIni                                            ();

    DIOSTREAMTLS13HANDSHAKECLIENT_STATE       GetState                                         ();
    bool                                    IsServerFinishedVerified                         ();
    bool                                    IsHandshakeCompleted                             ();
    bool                                    IsCertificateRequested                           ();
    bool                                    IsServerAuthenticated                            ();

    bool                                    Capabilities_Set                                 (DIOSTREAMTLSCONFIG* config);

    bool                                    SignatureSchemes_WidenECDSA                      ();
    bool                                    IsApplicationProtocolNegotiated                  ();
    bool                                    IsSessionResumed                                 ();
    bool                                    EarlyData_Prepare                                 (XDWORD size);
    bool                                    IsEarlyDataOffered                                ();
    bool                                    IsEarlyDataAccepted                               ();
    XDWORD                                  GetMaximumEarlyDataSize                           ();
    bool                                    EarlyData_Protect                                 (XBYTE* data, XDWORD size, XBUFFER& records);
    bool                                    EarlyData_Protect                                 (XBUFFER& data, XBUFFER& records);
    DIOSTREAMTLS_ALPN_TYPE                  GetApplicationProtocol                           ();
    XBUFFER*                                GetApplicationProtocolRaw                        ();

    bool                                    Authentication_Set                               (XCHAR* servername, XVECTOR<XBUFFER*>* trustedroots, XDATETIME* datetime = NULL);
    void                                    AIAFetch_Set                                      (bool active, int timeout);
    DIOSTREAMTLS13HANDSHAKECLIENT_AUTHENTICATIONERROR GetAuthenticationError                   ();
    CIPHERCERTIFICATEX509VALIDATOR_ERROR    GetCertificateValidationError                    ();

    DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATE* GetServerCertificate                            ();
    DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEVERIFY* GetServerCertificateVerify                 ();

    bool                                    ClientHello_Create                               (XCHAR* servername, XBUFFER& clienthello, XBUFFER& records);
    bool                                    Start                                            (XBUFFER& clienthello);
    bool                                    HelloRetryRequest_Process                         (XBUFFER& helloretryrequest, XBUFFER& clienthello, XBUFFER& records);
    bool                                    ServerHello_Process                              (XBUFFER& serverhello);
    bool                                    ServerHello_Process                              (XBUFFER& serverhello, XBUFFER& sharedsecret);
    bool                                    ClientFinished_Create                            (XBUFFER& clientfinished, XBUFFER& records);
    bool                                    NewSessionTicket_Process                         (XBUFFER& message);

    bool                                    RecordInput_Add                                  (XBYTE* data, XDWORD size);
    bool                                    RecordInput_Add                                  (XBUFFER& data);
    bool                                    Process                                          ();
    bool                                    Handshake_Process                                (XBUFFER& message);

  private:

    bool                                    EncryptedExtensions_Process                      (XBUFFER& message);
    bool                                    CertificateRequest_Process                       (XBUFFER& message);
    bool                                    Certificate_Process                              (XBUFFER& message);
    bool                                    CertificateVerify_Process                        (XBUFFER& message);
    bool                                    Finished_Process                                 (XBUFFER& message);

    bool                                    CertificateChain_CompleteViaAIA                   (XVECTOR<XBUFFER*>& certificatechain);

    bool                                    SetError                                         ();
    bool                                    SetAuthenticationError                           (DIOSTREAMTLS13HANDSHAKECLIENT_AUTHENTICATIONERROR error);
    bool                                    CipherSuite_IsOffered                            (XWORD ciphersuite);
    bool                                    SupportedGroup_IsOffered                         (XWORD supportedgroup);
    bool                                    KeyShare_IsOffered                               (XWORD supportedgroup);
    bool                                    SignatureScheme_IsOffered                        (XWORD signaturescheme);
    bool                                    ApplicationProtocol_IsOffered                    (DIOSTREAMTLS_ALPN_TYPE applicationprotocol);
    bool                                    ApplicationProtocol_IsOffered                    (XBUFFER& applicationprotocol);
    void                                    Clean                                            ();

    DIOSTREAMTLS13SESSION*                    session;
    DIOSTREAMTLSCONFIG*                       config;
    DIOSTREAMTLS13HANDSHAKECLIENT_STATE       state;
    bool                                    isini;
    bool                                    certificaterequested;
    bool                                    clientOCSPstaplingrequested;
    bool                                    authenticationconfigured;
    bool                                    serverauthenticated;

    DIOSTREAMTLS13HANDSHAKECLIENT_AUTHENTICATIONERROR authenticationerror;
    CIPHERCERTIFICATEX509VALIDATOR_ERROR    certificatevalidationerror;

    XSTRING                                 expectedservername;
    XVECTOR<XBUFFER*>                       trustedroots;
    XDATETIME                               validationdatetime;
    bool                                    hasvalidationdatetime;

    bool                                    aiafetchactive;
    int                                     aiafetchtimeout;

    CIPHERCERTIFICATEX509VALIDATOR          certificatevalidator;

    XBUFFER                                 legacysessionid;

    XVECTOR<XWORD>                          ciphersuites;
    XVECTOR<XWORD>                          supportedgroups;
    XVECTOR<XWORD>                          signatureschemes;
    XVECTOR<XWORD>                          certificatesignatureschemes;
    XVECTOR<DIOSTREAMTLS_ALPN_TYPE>          applicationprotocols;
    XVECTOR<XBUFFER*>                        applicationprotocolsraw;

    XVECTOR<XWORD>                          offeredciphersuites;
    XVECTOR<XWORD>                          offeredsupportedgroups;
    XVECTOR<XWORD>                          offeredkeysharegroups;
    XVECTOR<XWORD>                          offeredsignatureschemes;
    XVECTOR<XWORD>                          requestedclientsignatureschemes;
    XVECTOR<XWORD>                          requestedclientcertificatesignatureschemes;
    XVECTOR<XBUFFER*>                       requestedclientcertificateauthorities;
    XVECTOR<DIOSTREAMTLS_ALPN_TYPE>          offeredapplicationprotocols;
    XVECTOR<XBUFFER*>                        offeredapplicationprotocolsraw;

    bool                                    applicationprotocolnegotiated;
    DIOSTREAMTLS_ALPN_TYPE                  applicationprotocol;
    XBUFFER                                 applicationprotocolraw;

    bool                                    resumptionoffered;
    bool                                    resumptionaccepted;
    XBUFFER                                 resumptionpsk;
    XWORD                                   resumptionciphersuite;
    bool                                    earlydataoffered;
    bool                                    earlydataaccepted;
    XDWORD                                  maximumearlydatasize;
    XDWORD                                  preparedearlydatasize;

    XBUFFER                                 firstclienthello;
    XWORD                                   currentkeysharegroup;
    bool                                    helloretryrequestprocessed;

    DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATE* servercertificate;
    DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEVERIFY* servercertificateverify;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
