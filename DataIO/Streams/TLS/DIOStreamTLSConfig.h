/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOStreamTLSConfig.h
* 
* @class      DIOSTREAMTLSCONFIG
* @brief      Data Input/Output Stream TLS (Transport Layer Security) Config class
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

#include "DIOIP.h"
#include "DIOURL.h"

#include "DIOStreamTCPIPConfig.h"
#include "DIOStreamConfigString.h"

#include "DIOStreamTLSMessages.h"
#include "CipherCertificateX509Validator.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum DIOSTREAMTLS_CLIENTAUTHENTICATION_MODE
{
  DIOSTREAMTLS_CLIENTAUTHENTICATION_MODE_NONE = 0 ,
  DIOSTREAMTLS_CLIENTAUTHENTICATION_MODE_OPTIONAL ,
  DIOSTREAMTLS_CLIENTAUTHENTICATION_MODE_REQUIRED ,
};


enum DIOSTREAMTLS_TRUSTSTORE_FALLBACKPOLICY
{
  DIOSTREAMTLS_TRUSTSTORE_FALLBACKPOLICY_NEVER = 0,
  DIOSTREAMTLS_TRUSTSTORE_FALLBACKPOLICY_ON_NATIVE_FAILURE,
};

enum DIOSTREAMTLS_REVOCATIONPOLICY
{
  DIOSTREAMTLS_REVOCATIONPOLICY_OFF = 0,
  DIOSTREAMTLS_REVOCATIONPOLICY_SOFT_FAIL,
  DIOSTREAMTLS_REVOCATIONPOLICY_HARD_FAIL,
  DIOSTREAMTLS_REVOCATIONPOLICY_MUST_STAPLE,
};


#define DIOSTREAMTLS13_SESSIONTICKET_DEFAULT_LIFETIME      86400
#define DIOSTREAMTLS13_SESSIONTICKET_MAX_LIFETIME         604800
#define DIOSTREAMTLS13_SESSIONTICKET_MAX_CACHED                8
#define DIOSTREAMTLS13_SESSIONTICKET_DEFAULT_KEYROTATION    86400
#define DIOSTREAMTLS13_EARLYDATA_DEFAULT_MAXSIZE                0
#define DIOSTREAMTLS13_EARLYDATA_MAXSIZE                    16384
#define DIOSTREAMTLS13_EARLYDATA_AGE_TOLERANCE_MS            10000
#define DIOSTREAMTLS_DEFAULT_CONNECTION_TIMEOUT                3
#define DIOSTREAMTLS_DEFAULT_HANDSHAKE_TIMEOUT                 10

#define DIOSTREAMTLS_MEMORY_DEFAULT_RECORD_INPUT              (4*1024*1024)
#define DIOSTREAMTLS_MEMORY_DEFAULT_HANDSHAKE_INPUT           (4*1024*1024)
#define DIOSTREAMTLS_MEMORY_DEFAULT_TRANSCRIPT                (8*1024*1024)
#define DIOSTREAMTLS_MEMORY_DEFAULT_APPLICATION_INPUT         (4*1024*1024)
#define DIOSTREAMTLS_MEMORY_DEFAULT_AIA_BODY                    (16*1024)
#define DIOSTREAMTLS_MEMORY_DEFAULT_AIA_HEADER                   (8*1024)
#define DIOSTREAMTLS_MEMORY_DEFAULT_AIA_FETCHES                         3
#define DIOSTREAMTLS_MEMORY_DEFAULT_OCSP_RESPONSE              (256*1024)
#define DIOSTREAMTLS_MEMORY_DEFAULT_CRL                       (4*1024*1024)
#define DIOSTREAMTLS_MEMORY_DEFAULT_CRLS                               64
#define DIOSTREAMTLS_MEMORY_DEFAULT_SESSION_TICKETS                     8
#define DIOSTREAMTLS_MEMORY_DEFAULT_TRUST_ROOTS                       2048
#define DIOSTREAMTLS_MEMORY_DEFAULT_TRUST_CERTIFICATE          (1024*1024)
#define DIOSTREAMTLS_MEMORY_DEFAULT_TRUST_TOTAL              (32*1024*1024)
#define DIOSTREAMTLS_MEMORY_MINIMUM_RECORD_INPUT                    (18*1024)
#define DIOSTREAMTLS_MEMORY_MAXIMUM_BUFFER                         (64*1024*1024)


enum DIOSTREAMTLS_LOCALCREDENTIALSERROR
{
  DIOSTREAMTLS_LOCALCREDENTIALSERROR_NONE                         = 0 ,
  DIOSTREAMTLS_LOCALCREDENTIALSERROR_NOTCONFIGURED                   ,
  DIOSTREAMTLS_LOCALCREDENTIALSERROR_INVALIDCERTIFICATE              ,
  DIOSTREAMTLS_LOCALCREDENTIALSERROR_INVALIDDATE                     ,
  DIOSTREAMTLS_LOCALCREDENTIALSERROR_INVALIDKEYUSAGE                 ,
  DIOSTREAMTLS_LOCALCREDENTIALSERROR_UNSUPPORTEDKEY                  ,
  DIOSTREAMTLS_LOCALCREDENTIALSERROR_KEYMISMATCH                     ,
  DIOSTREAMTLS_LOCALCREDENTIALSERROR_INVALIDCHAIN                    ,
  DIOSTREAMTLS_LOCALCREDENTIALSERROR_INVALIDSERVERNAME               ,
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class CIPHERKEY;
class CIPHERTRUSTPROVIDERX509;
class XMUTEX;
typedef bool (*DIOSTREAMTLS_OCSPDIRECTFETCHER)(XSTRING& URL, CIPHERCERTIFICATEX509& certificate,
                                               CIPHERCERTIFICATEX509& issuer, XBUFFER& response, void* context);
typedef bool (*DIOSTREAMTLS_SESSIONTICKETKEYRING_LOAD)(XBUFFER& encryptedkeyring, void* context);
typedef bool (*DIOSTREAMTLS_SESSIONTICKETKEYRING_SAVE)(XBUFFER& encryptedkeyring, void* context);
typedef bool (*DIOSTREAMTLS_EARLYDATA_REPLAYCHECK)(XBUFFER& ticketidentity, void* context);

class DIOSTREAMTLSMEMORYPOLICY
{
  public:
                            DIOSTREAMTLSMEMORYPOLICY           ();

    bool                    SetConnectionBufferLimits         (XDWORD recordinput, XDWORD handshakeinput, XDWORD transcript, XDWORD applicationinput);
    bool                    SetAIALimits                      (XDWORD headersize, XDWORD bodysize, XDWORD maximumfetches);
    bool                    SetRevocationLimits               (XDWORD OCSPsize, XDWORD CRLsize, XDWORD maximumCRLs);
    bool                    SetTrustStoreLimits               (XDWORD maximumroots, XDWORD maximumcertificatesize, XDWORD maximumtotalsize);
    bool                    SetMaximumSessionTickets          (XDWORD maximumtickets);

    XDWORD                  GetMaximumRecordInputSize         ();
    XDWORD                  GetMaximumHandshakeInputSize      ();
    XDWORD                  GetMaximumTranscriptSize          ();
    XDWORD                  GetMaximumApplicationInputSize    ();
    XDWORD                  GetMaximumAIAHeaderSize           ();
    XDWORD                  GetMaximumAIABodySize             ();
    XDWORD                  GetMaximumAIAFetches              ();
    XDWORD                  GetMaximumOCSPResponseSize        ();
    XDWORD                  GetMaximumCRLSize                 ();
    XDWORD                  GetMaximumCRLs                    ();
    XDWORD                  GetMaximumSessionTickets          ();
    XDWORD                  GetMaximumTrustRoots              ();
    XDWORD                  GetMaximumTrustCertificateSize    ();
    XDWORD                  GetMaximumTrustStoreSize          ();

  private:
    XDWORD                  maximumrecordinputsize;
    XDWORD                  maximumhandshakeinputsize;
    XDWORD                  maximumtranscriptsize;
    XDWORD                  maximumapplicationinputsize;
    XDWORD                  maximumAIAheadersize;
    XDWORD                  maximumAIAbodysize;
    XDWORD                  maximumAIAfetches;
    XDWORD                  maximumOCSPresponsesize;
    XDWORD                  maximumCRLsize;
    XDWORD                  maximumCRLs;
    XDWORD                  maximumsessiontickets;
    XDWORD                  maximumtrustroots;
    XDWORD                  maximumtrustcertificatesize;
    XDWORD                  maximumtruststoresize;
};

class DIOSTREAMTLS13SESSIONTICKET
{
  public:
                            DIOSTREAMTLS13SESSIONTICKET       ();
    virtual                ~DIOSTREAMTLS13SESSIONTICKET       ();

    XSTRING*                GetServerName                      ();
    XBUFFER*                GetTicket                          ();
    XBUFFER*                GetPSK                             ();
    XDWORD                  GetTicketAgeAdd                    ();
    void                    SetTicketAgeAdd                    (XDWORD ageadd);
    XDWORD                  GetLifetime                        ();
    void                    SetLifetime                        (XDWORD lifetime);
    XQWORD                  GetReceivedEpoch                   ();
    void                    SetReceivedEpoch                   (XQWORD epoch);
    XWORD                   GetCipherSuite                     ();
    void                    SetCipherSuite                     (XWORD ciphersuite);
    DIOSTREAMTLS_ALPN_TYPE  GetApplicationProtocol             ();
    void                    SetApplicationProtocol             (DIOSTREAMTLS_ALPN_TYPE protocol);
    XBUFFER*                GetApplicationProtocolRaw          ();
    XDWORD                  GetMaximumEarlyDataSize            ();
    void                    SetMaximumEarlyDataSize            (XDWORD size);
    bool                    IsExpired                          ();
    XDWORD                  GetObfuscatedAge                   ();
    bool                    Delete                             ();

  private:

    void                    Clean                              ();

    XSTRING                 servername;
    XBUFFER                 ticket;
    XSECUREBUFFER           psk;
    XDWORD                  ticketageadd;
    XDWORD                  lifetime;
    XQWORD                  receivedepoch;
    XWORD                   ciphersuite;
    DIOSTREAMTLS_ALPN_TYPE  applicationprotocol;
    XBUFFER                 applicationprotocolraw;
    XDWORD                  maximumearlydatasize;
};

class DIOSTREAMTLSSERVERCREDENTIALS
{
  public:
                            DIOSTREAMTLSSERVERCREDENTIALS      ();
    virtual                ~DIOSTREAMTLSSERVERCREDENTIALS      ();

    XSTRING*                GetServerName                       ();

    XVECTOR<XBUFFER*>*      GetCertificateChain                ();
    bool                    Certificate_Add                    (XBUFFER& certificate);
    bool                    Certificates_Delete                ();

    CIPHERKEY*              GetPrivateKey                       ();
    bool                    SetPrivateKey                       (CIPHERKEY* privatekey);

    XBUFFER*                GetOCSPStapledResponse              ();
    bool                    SetOCSPStapledResponse              (XBUFFER& response);
    bool                    OCSPStapledResponse_Add             (XBUFFER& response);
    XVECTOR<XBUFFER*>*      GetOCSPStapledResponses              ();
    bool                    DeleteOCSPStapledResponse           ();

    bool                    HasCredentials                      ();
    bool                    Delete                              ();

  private:

    void                    Clean                               ();

    XSTRING                 servername;
    XVECTOR<XBUFFER*>       certificatechain;
    CIPHERKEY*              privatekey;
    XBUFFER                 OCSPstapledresponse;
    XVECTOR<XBUFFER*>       OCSPstapledresponses;
};

class DIOSTREAMTLSCONFIG  : public DIOSTREAMTCPIPCONFIG
{
  public:
                            DIOSTREAMTLSCONFIG                ();
    virtual                ~DIOSTREAMTLSCONFIG                ();

    bool                    Freeze                            ();
    bool                    IsFrozen                          ();

    XWORD                   GetCipherSuite                    ();
    void                    SetCipherSuite                    (XWORD ciphersuite);

    XVECTOR<XWORD>*         GetCipherSuites                   ();
    bool                    CipherSuite_Add                   (XWORD ciphersuite);
    bool                    CipherSuites_Delete               ();
    bool                    CipherSuites_AddRecommendedProfile();

    XVECTOR<XWORD>*         GetTLS12CipherSuites              ();
    bool                    TLS12CipherSuite_Add              (XWORD ciphersuite);
    bool                    TLS12CipherSuites_Delete          ();

    XVECTOR<XWORD>*         GetSupportedGroups                ();
    bool                    SupportedGroup_Add                (XWORD supportedgroup);
    bool                    SupportedGroups_Delete            ();

    XVECTOR<XWORD>*         GetSignatureSchemes               ();
    bool                    SignatureScheme_Add               (XWORD signaturescheme);
    bool                    SignatureSchemes_Delete           ();

    XVECTOR<XWORD>*         GetCertificateSignatureSchemes    ();
    bool                    CertificateSignatureScheme_Add    (XWORD signaturescheme);
    bool                    CertificateSignatureSchemes_Delete ();

    XVECTOR<DIOSTREAMTLS_ALPN_TYPE>* GetApplicationProtocols  ();
    XDWORD                  GetApplicationProtocolsCount      ();
    bool                    GetApplicationProtocol            (XDWORD index, XBUFFER& applicationprotocol);
    bool                    ApplicationProtocol_Add           (XBUFFER& applicationprotocol);
    bool                    ApplicationProtocol_Add           (DIOSTREAMTLS_ALPN_TYPE applicationprotocol);
    bool                    ApplicationProtocols_Delete       ();

    XSTRING*                GetServerName                     ();

    XVECTOR<XBUFFER*>*      GetTrustedRoots                   ();
    bool                    TrustedRoot_Add                   (XBUFFER& root);
    bool                    TrustedRoots_Load                 (CIPHERTRUSTPROVIDERX509& provider);
    bool                    TrustedRoots_AddDefaults          ();
    bool                    TrustedRoots_Delete               ();
    DIOSTREAMTLS_TRUSTSTORE_FALLBACKPOLICY GetTrustStoreFallbackPolicy ();
    bool                    SetTrustStoreFallbackPolicy       (DIOSTREAMTLS_TRUSTSTORE_FALLBACKPOLICY policy);

    DIOSTREAMTLS_CLIENTAUTHENTICATION_MODE GetClientAuthenticationMode ();
    void                    SetClientAuthenticationMode       (DIOSTREAMTLS_CLIENTAUTHENTICATION_MODE mode);
    XVECTOR<XBUFFER*>*      GetClientTrustedRoots             ();
    bool                    ClientTrustedRoot_Add             (XBUFFER& root);
    bool                    ClientTrustedRoots_Load           (CIPHERTRUSTPROVIDERX509& provider);
    bool                    ClientTrustedRoots_Delete         ();

    XVECTOR<XBUFFER*>*      GetLocalCertificateChain          ();
    bool                    LocalCertificate_Add              (XBUFFER& certificate);
    bool                    LocalCertificates_Delete          ();

    CIPHERKEY*              GetLocalPrivateKey                ();
    bool                    SetLocalPrivateKey                (CIPHERKEY* privatekey);

    XBUFFER*                GetLocalOCSPStapledResponse       ();
    bool                    SetLocalOCSPStapledResponse       (XBUFFER& response);
    bool                    DeleteLocalOCSPStapledResponse    ();

    bool                    HasLocalCredentials               ();
    bool                    LocalCredentials_Validate         ();
    DIOSTREAMTLS_LOCALCREDENTIALSERROR GetLocalCredentialsError ();
    bool                    LocalCredentials_Delete           ();

    DIOSTREAMTLSSERVERCREDENTIALS* ServerCredentials_Add      (XCHAR* servername);
    XVECTOR<DIOSTREAMTLSSERVERCREDENTIALS*>* GetServerCredentials ();
    bool                    ServerCredentials_Select           (XCHAR* servername, XVECTOR<XBUFFER*>*& certificatechain, CIPHERKEY*& privatekey);
    bool                    ServerCredentials_Select           (XCHAR* servername, XVECTOR<XBUFFER*>*& certificatechain, CIPHERKEY*& privatekey, XBUFFER*& OCSPstapledresponse);
    bool                    ServerCredentials_Delete           ();

    DIOSTREAMTLS_REVOCATIONPOLICY GetRevocationPolicy         ();
    bool                    SetRevocationPolicy               (DIOSTREAMTLS_REVOCATIONPOLICY policy);

    bool                    IsActiveAIAFetch                  ();
    void                    AIAFetch_Activate                 (bool activate);

    int                     GetAIAFetchTimeout                ();
    bool                    SetAIAFetchTimeout                (int timeout);
    int                     GetConnectionTimeout              ();
    bool                    SetConnectionTimeout              (int timeout);
    int                     GetHandshakeTimeout               ();
    bool                    SetHandshakeTimeout               (int timeout);

    CIPHERCERTIFICATEX509VALIDATIONPOLICY* GetCertificateValidationPolicy ();
    void                    SetCertificateValidationPolicy    (CIPHERCERTIFICATEX509VALIDATIONPOLICY& policy);
    DIOSTREAMTLSMEMORYPOLICY* GetMemoryPolicy                 ();
    bool                    SetMemoryPolicy                   (DIOSTREAMTLSMEMORYPOLICY& policy);
    XVECTOR<XBUFFER*>*      GetCertificateRevocationLists     ();
    bool                    CertificateRevocationList_Add     (XBUFFER& CRL);
    bool                    CertificateRevocationLists_Delete ();
    void                    OCSPDirect_Set                     (DIOSTREAMTLS_OCSPDIRECTFETCHER fetcher, void* context = NULL);
    DIOSTREAMTLS_OCSPDIRECTFETCHER GetOCSPDirectFetcher       ();
    void*                   GetOCSPDirectContext               ();

    XWORD                   GetMinVersion                     ();
    bool                    SetMinVersion                     (XWORD version);

    XWORD                   GetMaxVersion                     ();
    bool                    SetMaxVersion                     (XWORD version);

    bool                    IsSessionResumptionActive         ();
    bool                    IsEarlyDataActive                 ();
    void                    EarlyData_Activate                 (bool active);
    XDWORD                  GetMaximumEarlyDataSize            ();
    bool                    SetMaximumEarlyDataSize            (XDWORD size);
    bool                    EarlyDataReplayCheck_Set           (DIOSTREAMTLS_EARLYDATA_REPLAYCHECK callback, void* context = NULL);
    bool                    EarlyDataReplayCheck               (XBUFFER& ticketidentity);
    bool                    IsEarlyDataAcceptable              ();
    bool                    EarlyDataTicketAge_IsAcceptable     (XQWORD issueepoch, XDWORD obfuscatedage, XDWORD ageadd);
    void                    SessionResumption_Activate        (bool active);
    XDWORD                  GetSessionTicketLifetime          ();
    bool                    SetSessionTicketLifetime          (XDWORD lifetime);
    bool                    SessionResumption_ServerInitialize ();
    XDWORD                  GetSessionTicketKeyRotationInterval ();
    bool                    SetSessionTicketKeyRotationInterval (XDWORD interval);
    bool                    SessionTicketKeyRing_Rotate       ();
    bool                    SessionTicketKeyRing_Export       (XBUFFER& wrappingkey, XBUFFER& encryptedkeyring);
    bool                    SessionTicketKeyRing_Import       (XBUFFER& wrappingkey, XBUFFER& encryptedkeyring);
    bool                    SessionTicketKeyRingSynchronizer_Set (DIOSTREAMTLS_SESSIONTICKETKEYRING_LOAD load, DIOSTREAMTLS_SESSIONTICKETKEYRING_SAVE save, XBUFFER& wrappingkey, void* context = NULL);
    bool                    SessionTicketKeyRing_Synchronize  (bool publish);
    bool                    SessionTicket_Store               (XCHAR* servername, XBUFFER& ticket, XBUFFER& PSK, XDWORD ageadd, XDWORD lifetime, XWORD ciphersuite, DIOSTREAMTLS_ALPN_TYPE applicationprotocol);
    bool                    SessionTicket_StoreRaw            (XCHAR* servername, XBUFFER& ticket, XBUFFER& PSK, XDWORD ageadd, XDWORD lifetime, XWORD ciphersuite, XBUFFER* applicationprotocol, XDWORD maximumearlydatasize = 0);
    bool                    SessionTicket_Copy                (XCHAR* servername, DIOSTREAMTLS13SESSIONTICKET& destination);
    bool                    SessionTickets_Delete             ();
    bool                    SessionTicket_Seal                (XBUFFER& PSK, XWORD ciphersuite, DIOSTREAMTLS_ALPN_TYPE applicationprotocol, XCHAR* servername, XDWORD lifetime, XDWORD ageadd, XBUFFER& ticket, XDWORD maximumearlydatasize = 0);
    bool                    SessionTicket_SealRaw             (XBUFFER& PSK, XWORD ciphersuite, XBUFFER* applicationprotocol, XCHAR* servername, XDWORD lifetime, XDWORD ageadd, XBUFFER& ticket, XDWORD maximumearlydatasize = 0);
    bool                    SessionTicket_Open                (XBUFFER& ticket, XBUFFER& PSK, XWORD& ciphersuite, DIOSTREAMTLS_ALPN_TYPE& applicationprotocol, XSTRING& servername, XQWORD& issueepoch, XDWORD& lifetime, XDWORD& ageadd);
    bool                    SessionTicket_OpenRaw             (XBUFFER& ticket, XBUFFER& PSK, XWORD& ciphersuite, XBUFFER& applicationprotocol, XSTRING& servername, XQWORD& issueepoch, XDWORD& lifetime, XDWORD& ageadd, XDWORD& maximumearlydatasize);

  protected:

  private:

    bool                    Credentials_Validate              (XVECTOR<XBUFFER*>* certificatechain, CIPHERKEY* privatekey);
    bool                    CryptographicPolicy_Validate      ();
    bool                    MemoryPolicy_Validate             ();
    void                    Clean                             ();

    XWORD                   minversion;
    XWORD                   maxversion;

    XVECTOR<XWORD>          ciphersuites;
    XVECTOR<XWORD>          TLS12ciphersuites;
    XVECTOR<XWORD>          supportedgroups;
    XVECTOR<XWORD>          signatureschemes;
    XVECTOR<XWORD>          certificatesignatureschemes;
    XVECTOR<DIOSTREAMTLS_ALPN_TYPE> applicationprotocols;
    XBUFFER                 applicationprotocolsraw;
    XSTRING                 servername;
    XVECTOR<XBUFFER*>       trustedroots;
    DIOSTREAMTLS_TRUSTSTORE_FALLBACKPOLICY truststorefallbackpolicy;
    DIOSTREAMTLS_CLIENTAUTHENTICATION_MODE clientauthenticationmode;
    XVECTOR<XBUFFER*>       clienttrustedroots;
    XVECTOR<XBUFFER*>       localcertificatechain;
    CIPHERKEY*              localprivatekey;
    XBUFFER                 localOCSPstapledresponse;
    XVECTOR<DIOSTREAMTLSSERVERCREDENTIALS*> servercredentials;
    DIOSTREAMTLS_LOCALCREDENTIALSERROR localcredentialserror;
    DIOSTREAMTLS_REVOCATIONPOLICY revocationpolicy;

    bool                    aiafetchactive;
    int                     aiafetchtimeout;
    int                     connectiontimeout;
    int                     handshaketimeout;
    CIPHERCERTIFICATEX509VALIDATIONPOLICY certificatevalidationpolicy;
    DIOSTREAMTLSMEMORYPOLICY memorypolicy;
    XVECTOR<XBUFFER*>       certificaterevocationlists;
    DIOSTREAMTLS_OCSPDIRECTFETCHER ocspdirectfetcher;
    void*                   ocspdirectcontext;

    bool                    sessionresumptionactive;
    bool                    earlydataactive;
    XDWORD                  maximumearlydatasize;
    DIOSTREAMTLS_EARLYDATA_REPLAYCHECK earlydatareplaycheck;
    void*                   earlydatareplaycontext;
    XDWORD                  sessionticketlifetime;
    XDWORD                  sessionticketkeyrotationinterval;
    XSECUREBUFFER           sessionticketserverkeycurrent;
    XSECUREBUFFER           sessionticketserverkeyprevious;
    XQWORD                  sessionticketserverkeycurrentID;
    XQWORD                  sessionticketserverkeygeneration;
    XQWORD                  sessionticketserverkeypreviousID;
    XQWORD                  sessionticketserverkeycurrentcreated;
    XQWORD                  sessionticketserverkeypreviousexpires;
    DIOSTREAMTLS_SESSIONTICKETKEYRING_LOAD sessionticketkeyringload;
    DIOSTREAMTLS_SESSIONTICKETKEYRING_SAVE sessionticketkeyringsave;
    void*                   sessionticketkeyringcontext;
    XSECUREBUFFER           sessionticketkeyringwrappingkey;
    bool                    sessionticketkeyringdirty;
    XVECTOR<DIOSTREAMTLS13SESSIONTICKET*> sessiontickets;
    XMUTEX*                 configmutex;
    bool                    frozen;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
