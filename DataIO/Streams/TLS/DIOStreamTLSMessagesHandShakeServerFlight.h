/**-------------------------------------------------------------------------------------------------------------------
*
* @file       DIOStreamTLSMessagesHandShakeServerFlight.h
*
* @class      DIOSTREAMTLSMESSAGESHANDSHAKESERVERFLIGHT
* @brief      Data Input/Output Stream TLS 1.3 Handshake Server Flight Messages classes
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

#include "DIOStreamTLSMessages.h"
#include "DIOStreamTLSMessagesExtension.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOSTREAMTLS_MSG_FINISHED_MAXSIZE                                   64

// Size in bytes of a TLS handshake 24-bit ("uint24") length field (e.g. Certificate.certificate_list entry length).
#define DIOSTREAMTLS_MSG_LENGTH24_SIZE                                      (sizeof(XBYTE) * 3)



/*---- CLASS ---------------------------------------------------------------------------------------------------------*/


class DIOSTREAMTLS_MSG_HANDSHAKE_ENCRYPTEDEXTENSIONS : public DIOSTREAMTLS_MSG_INTERFACE
{
  public:
                                            DIOSTREAMTLS_MSG_HANDSHAKE_ENCRYPTEDEXTENSIONS   ();
    virtual                                ~DIOSTREAMTLS_MSG_HANDSHAKE_ENCRYPTEDEXTENSIONS   ();

    XWORD                                   Extensions_GetLength                              ();
    XVECTOR<DIOSTREAMTLS_MSG_EXTENSION*>*   Extensions_GetAll                                ();
    bool                                    Extensions_Add                                   (DIOSTREAMTLS_MSG_EXTENSION* extension);
    bool                                    Extensions_DeleteAll                             ();

    bool                                    SetToBuffer                                      (XBUFFER& buffer, bool showdebug);
    bool                                    GetFromBuffer                                    (XBUFFER& buffer, bool showdebug);

  private:

    void                                    Clean                                            ();

    XWORD                                   extensionslength;
    XVECTOR<DIOSTREAMTLS_MSG_EXTENSION*>    extensions;
};


class DIOSTREAMTLS_MSG_CERTIFICATEENTRY : public DIOSTREAMTLS_MSG_INTERFACE
{
  public:
                                            DIOSTREAMTLS_MSG_CERTIFICATEENTRY                 ();
    virtual                                ~DIOSTREAMTLS_MSG_CERTIFICATEENTRY                 ();

    XBUFFER*                                GetCertificateData                                ();

    XWORD                                   Extensions_GetLength                              ();
    XVECTOR<DIOSTREAMTLS_MSG_EXTENSION*>*   Extensions_GetAll                                ();
    bool                                    Extensions_Add                                   (DIOSTREAMTLS_MSG_EXTENSION* extension);
    bool                                    Extensions_DeleteAll                             ();

    bool                                    SetToBuffer                                      (XBUFFER& buffer, bool showdebug);
    bool                                    GetFromBuffer                                    (XBUFFER& buffer, bool showdebug);

  private:

    void                                    Clean                                            ();

    XBUFFER                                 certificatedata;
    XWORD                                   extensionslength;
    XVECTOR<DIOSTREAMTLS_MSG_EXTENSION*>    extensions;
};


class DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATE : public DIOSTREAMTLS_MSG_INTERFACE
{
  public:
                                            DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATE            ();
    virtual                                ~DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATE            ();

    XBUFFER*                                GetRequestContext                                 ();
    bool                                    SetRequestContext                                 (XBYTE* context, XBYTE size);

    XDWORD                                  CertificateList_GetLength                        ();
    XVECTOR<DIOSTREAMTLS_MSG_CERTIFICATEENTRY*>* CertificateList_GetAll                       ();
    bool                                    CertificateList_Add                              (DIOSTREAMTLS_MSG_CERTIFICATEENTRY* certificate);
    bool                                    CertificateList_DeleteAll                        ();

    bool                                    SetToBuffer                                      (XBUFFER& buffer, bool showdebug);
    bool                                    GetFromBuffer                                    (XBUFFER& buffer, bool showdebug);

  private:

    void                                    Clean                                            ();

    XBUFFER                                 requestcontext;
    XDWORD                                  certificatelistlength;
    XVECTOR<DIOSTREAMTLS_MSG_CERTIFICATEENTRY*> certificatelist;
};


class DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEREQUEST : public DIOSTREAMTLS_MSG_INTERFACE
{
  public:
                                            DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEREQUEST     ();
    virtual                                ~DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEREQUEST     ();

    XBUFFER*                                GetRequestContext                                 ();
    bool                                    SetRequestContext                                 (XBYTE* context, XBYTE size);

    XWORD                                   Extensions_GetLength                              ();
    XVECTOR<DIOSTREAMTLS_MSG_EXTENSION*>*   Extensions_GetAll                                ();
    bool                                    Extensions_Add                                   (DIOSTREAMTLS_MSG_EXTENSION* extension);
    bool                                    Extensions_DeleteAll                             ();

    bool                                    SetToBuffer                                      (XBUFFER& buffer, bool showdebug);
    bool                                    GetFromBuffer                                    (XBUFFER& buffer, bool showdebug);

  private:

    void                                    Clean                                            ();

    XBUFFER                                 requestcontext;
    XWORD                                   extensionslength;
    XVECTOR<DIOSTREAMTLS_MSG_EXTENSION*>    extensions;
};


class DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEVERIFY : public DIOSTREAMTLS_MSG_INTERFACE
{
  public:
                                            DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEVERIFY      ();
    virtual                                ~DIOSTREAMTLS_MSG_HANDSHAKE_CERTIFICATEVERIFY      ();

    XWORD                                   GetAlgorithm                                      ();
    void                                    SetAlgorithm                                      (XWORD algorithm);

    XBUFFER*                                GetSignature                                      ();

    bool                                    SetToBuffer                                      (XBUFFER& buffer, bool showdebug);
    bool                                    GetFromBuffer                                    (XBUFFER& buffer, bool showdebug);

  private:

    void                                    Clean                                            ();

    XWORD                                   algorithm;
    XBUFFER                                 signature;
};

class DIOSTREAMTLS_MSG_HANDSHAKE_FINISHED : public DIOSTREAMTLS_MSG_INTERFACE
{
  public:
                                            DIOSTREAMTLS_MSG_HANDSHAKE_FINISHED               ();
    virtual                                ~DIOSTREAMTLS_MSG_HANDSHAKE_FINISHED               ();

    XBUFFER*                                GetVerifyData                                     ();

    bool                                    SetToBuffer                                      (XBUFFER& buffer, bool showdebug);
    bool                                    GetFromBuffer                                    (XBUFFER& buffer, bool showdebug);

  private:

    void                                    Clean                                            ();

    XBUFFER                                 verifydata;
};

class DIOSTREAMTLS_MSG_HANDSHAKE_NEWSESSIONTICKET : public DIOSTREAMTLS_MSG_INTERFACE
{
  public:
                                            DIOSTREAMTLS_MSG_HANDSHAKE_NEWSESSIONTICKET        ();
    virtual                                ~DIOSTREAMTLS_MSG_HANDSHAKE_NEWSESSIONTICKET        ();

    XDWORD                                  GetTicketLifetime                                   ();
    void                                    SetTicketLifetime                                   (XDWORD lifetime);
    XDWORD                                  GetTicketAgeAdd                                     ();
    void                                    SetTicketAgeAdd                                     (XDWORD ageadd);
    XBUFFER*                                GetTicketNonce                                      ();
    XBUFFER*                                GetTicket                                           ();
    XDWORD                                  GetMaximumEarlyDataSize                              ();
    void                                    SetMaximumEarlyDataSize                              (XDWORD size);

    bool                                    SetToBuffer                                         (XBUFFER& buffer, bool showdebug);
    bool                                    GetFromBuffer                                       (XBUFFER& buffer, bool showdebug);

  private:

    void                                    Clean                                               ();

    XDWORD                                  ticketlifetime;
    XDWORD                                  ticketageadd;
    XBUFFER                                 ticketnonce;
    XBUFFER                                 ticket;
    XDWORD                                  maximumearlydatasize;
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





