/**-------------------------------------------------------------------------------------------------------------------
*
* @file       DIOStreamTLS12KeySchedule.h
*
* @class      DIOSTREAMTLS12KEYSCHEDULE
* @brief      Data Input/Output Stream TLS 1.2 key schedule class (RFC 5246)
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


#define DIOSTREAMTLS12KEYSCHEDULE_MASTERSECRETSIZE            48                // Fixed by RFC 5246, section 8.1
#define DIOSTREAMTLS12KEYSCHEDULE_RANDOMSIZE                  32
#define DIOSTREAMTLS12KEYSCHEDULE_VERIFYDATASIZE              12                // RFC 5246, section 7.4.9
#define DIOSTREAMTLS12KEYSCHEDULE_AESGCM_FIXEDIVSIZE          4                 // RFC 5288
#define DIOSTREAMTLS12KEYSCHEDULE_CHACHA20_FIXEDIVSIZE        12                // RFC 7905
#define DIOSTREAMTLS12KEYSCHEDULE_AESGCM_RECORDIVSIZE         8                 // Explicit nonce sent with every AES-GCM record
#define DIOSTREAMTLS12KEYSCHEDULE_CHACHA20_RECORDIVSIZE       0                 // RFC 7905: no explicit nonce
#define DIOSTREAMTLS12KEYSCHEDULE_FIXEDIVSIZE                 DIOSTREAMTLS12KEYSCHEDULE_AESGCM_FIXEDIVSIZE
#define DIOSTREAMTLS12KEYSCHEDULE_RECORDIVSIZE                DIOSTREAMTLS12KEYSCHEDULE_AESGCM_RECORDIVSIZE

#define DIOSTREAMTLS12KEYSCHEDULE_LABEL_MASTERSECRET          "master secret"
#define DIOSTREAMTLS12KEYSCHEDULE_LABEL_EXTENDEDMASTERSECRET  "extended master secret"
#define DIOSTREAMTLS12KEYSCHEDULE_LABEL_KEYEXPANSION          "key expansion"
#define DIOSTREAMTLS12KEYSCHEDULE_LABEL_CLIENTFINISHED        "client finished"
#define DIOSTREAMTLS12KEYSCHEDULE_LABEL_SERVERFINISHED        "server finished"

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class HASH;

class DIOSTREAMTLS12KEYSCHEDULE
{
  public:
                                            DIOSTREAMTLS12KEYSCHEDULE                         ();
    virtual                                ~DIOSTREAMTLS12KEYSCHEDULE                         ();

    bool                                    Ini                                               (XWORD ciphersuite, DIOSTREAMTLSKEYSCHEDULE_ROLE role);
    void                                    End                                               ();
    bool                                    IsIni                                             ();

    XWORD                                   GetCipherSuite                                    ();
    DIOSTREAMTLSKEYSCHEDULE_ROLE            GetRole                                           ();

    XDWORD                                  GetHashSize                                       ();
    XDWORD                                  GetKeySize                                        ();
    XDWORD                                  GetFixedIVSize                                    ();
    HASH*                                   GetHash                                           ();

    bool                                    PRF                                               (XBUFFER& secret, const char* label, XBUFFER& seed, XDWORD outputsize, XBUFFER& output);

    bool                                    MasterSecret_Create                               (XBUFFER& premastersecret, XBUFFER& clientrandom, XBUFFER& serverrandom);
    bool                                    MasterSecretExtended_Create                       (XBUFFER& premastersecret, XBUFFER& sessionhash);
    XBUFFER*                                GetMasterSecret                                   ();

    bool                                    KeyBlock_Create                                   (XBUFFER& clientrandom, XBUFFER& serverrandom);

    XBUFFER*                                GetKey                                            (DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction);
    XBUFFER*                                GetFixedIV                                        (DIOSTREAMTLSKEYSCHEDULE_DIRECTION direction);

    
    bool                                    VerifyData_Create                                 (bool isclient, XBUFFER& handshakehash, XBUFFER& verifydata);

  private:

    void                                    Clean                                             ();

    bool                                    PHash                                             (XBUFFER& secret, XBUFFER& seed, XDWORD outputsize, XBUFFER& output);

    bool                                    isini;
    XWORD                                   ciphersuite;
    DIOSTREAMTLSKEYSCHEDULE_ROLE            role;

    HASH*                                   hash;
    XDWORD                                  hashsize;
    XDWORD                                  keysize;
    XDWORD                                  fixedIVsize;

    XSECUREBUFFER                           mastersecret;
    XSECUREBUFFER                           key[DIOSTREAMTLSKEYSCHEDULE_MAXDIRECTIONS];
    XBUFFER                                 fixedIV[DIOSTREAMTLSKEYSCHEDULE_MAXDIRECTIONS];
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
