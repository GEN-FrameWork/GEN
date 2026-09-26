/**-------------------------------------------------------------------------------------------------------------------
*
* @file       DIOStreamTLS12Messages.h
*
* @class      DIOSTREAMTLS12MESSAGES
* @brief      Data Input/Output Stream TLS 1.2 (RFC 5246) Handshake Messages classes
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

// These classes only cover the handshake messages that DIOSTREAMTLS12 actually needs to speak: the ECDHE
// ServerKeyExchange/ClientKeyExchange pair (RFC 4492 / RFC 8422), the (simpler than TLS 1.3) plain X.509
// Certificate list, and the empty ServerHelloDone. Everything else reuses the version-agnostic wire formats
// already in DIOStreamTLSMessages.h (record header, generic handshake header, alert) without touching them:
// this is a PARALLEL implementation, not a branch inside the TLS 1.3 classes. Finished is also reused as-is
// from DIOStreamTLSMessagesHandShakeServerFlight.h: its body is just the raw verify_data, identical in both
// versions.

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOStreamTLSMessages.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


// RFC 4492 section 5.4 / RFC 8422: ECCurveType. DIOSTREAMTLS12 only ever offers named curves, so this is the
// only value it will accept from a server.
#define DIOSTREAMTLS12_MSG_ECCURVETYPE_NAMED_CURVE                           3

// RFC 5246 section 7.4.3: pre_master_secret size is fixed once the ECDHE exchange is done.

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOSTREAMTLS12_MSG_SERVERHELLODONE : public DIOSTREAMTLS_MSG_INTERFACE
{
  public:
                                            DIOSTREAMTLS12_MSG_SERVERHELLODONE                ();
    virtual                                ~DIOSTREAMTLS12_MSG_SERVERHELLODONE                ();

    bool                                    SetToBuffer                                       (XBUFFER& buffer, bool showdebug);
    bool                                    GetFromBuffer                                     (XBUFFER& buffer, bool showdebug);

  private:

    void                                    Clean                                             ();
};

class DIOSTREAMTLS12_MSG_SERVERKEYEXCHANGE_ECDHE : public DIOSTREAMTLS_MSG_INTERFACE
{
  public:
                                            DIOSTREAMTLS12_MSG_SERVERKEYEXCHANGE_ECDHE        ();
    virtual                                ~DIOSTREAMTLS12_MSG_SERVERKEYEXCHANGE_ECDHE        ();

    XBYTE                                   GetCurveType                                      ();

    XWORD                                   GetNamedCurve                                     ();
    void                                    SetNamedCurve                                     (XWORD namedcurve);

    XBUFFER*                                GetPublicKey                                      ();

    XWORD                                   GetSignatureAlgorithm                             ();
    void                                    SetSignatureAlgorithm                             (XWORD signaturealgorithm);

    XBUFFER*                                GetSignature                                      ();

    bool                                    GetSignedParams                                   (XBUFFER& params);

    bool                                    SetToBuffer                                       (XBUFFER& buffer, bool showdebug);
    bool                                    GetFromBuffer                                     (XBUFFER& buffer, bool showdebug);

  private:

    void                                    Clean                                             ();

    XBYTE                                   curvetype;
    XWORD                                   namedcurve;
    XBUFFER                                 publickey;
    XWORD                                   signaturealgorithm;
    XBUFFER                                 signature;
};

class DIOSTREAMTLS12_MSG_CLIENTKEYEXCHANGE_ECDHE : public DIOSTREAMTLS_MSG_INTERFACE
{
  public:
                                            DIOSTREAMTLS12_MSG_CLIENTKEYEXCHANGE_ECDHE        ();
    virtual                                ~DIOSTREAMTLS12_MSG_CLIENTKEYEXCHANGE_ECDHE        ();

    XBUFFER*                                GetPublicKey                                      ();
    bool                                    SetPublicKey                                      (XBUFFER& publickey);

    bool                                    SetToBuffer                                       (XBUFFER& buffer, bool showdebug);
    bool                                    GetFromBuffer                                     (XBUFFER& buffer, bool showdebug);

  private:

    void                                    Clean                                             ();

    XBUFFER                                 publickey;
};


class DIOSTREAMTLS12_MSG_CERTIFICATE : public DIOSTREAMTLS_MSG_INTERFACE
{
  public:
                                            DIOSTREAMTLS12_MSG_CERTIFICATE                    ();
    virtual                                ~DIOSTREAMTLS12_MSG_CERTIFICATE                    ();

    XDWORD                                  CertificateList_GetLength                         ();
    XVECTOR<XBUFFER*>*                      CertificateList_GetAll                            ();
    bool                                    CertificateList_Add                               (XBUFFER* certificate);
    bool                                    CertificateList_DeleteAll                         ();

    bool                                    SetToBuffer                                       (XBUFFER& buffer, bool showdebug);
    bool                                    GetFromBuffer                                     (XBUFFER& buffer, bool showdebug);

  private:

    void                                    Clean                                             ();

    XDWORD                                  certificatelistlength;
    XVECTOR<XBUFFER*>                       certificatelist;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
