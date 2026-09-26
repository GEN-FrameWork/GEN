/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOStreamTLSMessagesHandShakeServerHello.h
* 
* @class      DIOSTREAMTLSMESSAGESHANDSHAKESERVERHELLO
* @brief      Data Input/Output Stream TLS (Transport Layer Security) Handshake Server Hello Message class
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

#define DIOSTREAMTLS_MSG_HELLORETRYREQUEST_RANDOM_SIZE                       32

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOSTREAMTLS_MSG_HANDSHAKE_SERVERHELLO : public DIOSTREAMTLS_MSG_INTERFACE
{
  public:
                                            DIOSTREAMTLS_MSG_HANDSHAKE_SERVERHELLO            ();
    virtual                                ~DIOSTREAMTLS_MSG_HANDSHAKE_SERVERHELLO            ();
                                            
    XWORD                                   GetLegacyVersion                                  ();    
    void                                    SetLegacyVersion                                  (XWORD legacy_version);
                                            
    XBYTE*                                  GetRandom                                         ();

    XBYTE                                   GetSessionIDLength                                ();    
    void                                    SetSessionIDLength                                (XBYTE sessionID_length);
    XBYTE*                                  GetSessionID                                      ();

    XWORD                                   GetCiphersuitesLength                             ();
    void                                    SetCiphersuitesLength                             (XWORD ciphersuites_length);
    XVECTOR<XWORD>*                         GetCipherSuites                                   ();
    XWORD                                   GetCipherSuite                                    ();
    void                                    SetCipherSuite                                    (XWORD ciphersuite);

    XBYTE                                   GetCompressionLength                              ();
    void                                    SetCompressionLength                              (XBYTE compress_length);

    XBYTE                                   GetCompressionMethod                              ();
    void                                    SetCompressionMethod                              (XBYTE compress_method);

    XWORD                                   Extensions_GetLenght                              ();
    void                                    Extensions_SetLenght                              (XWORD extension_lenght);   
    XVECTOR<DIOSTREAMTLS_MSG_EXTENSION*>*   Extensions_GetAll                                 ();  
    bool                                    Extensions_Add                                    (DIOSTREAMTLS_MSG_EXTENSION* extension); 
    bool                                    Extensions_DeleteAll                              (); 
                                                
    bool                                    SetToBuffer                                       (XBUFFER& buffer, bool showdebug);    
    bool                                    GetFromBuffer                                     (XBUFFER& buffer, bool showdebug);

    bool                                    IsHelloRetryRequest                               ();
                                            
  private:

    void                                    Clean                                             ();
    
    XWORD                                   legacy_version;

    XBYTE                                   random[DIOSTREAMTLS_MSG_RANDOM_SIZE];

    XBYTE                                   sessionID_length;
    XBYTE                                   sessionID[DIOSTREAMTLS_MSG_SESSIONID_SIZE];
    
    XWORD                                   ciphersuites_length;
    XVECTOR<XWORD>                          ciphersuites;

    XBYTE                                   compress_length;
    XBYTE                                   compress_method;
                                             
    XWORD                                   extensions_lenght;
    XVECTOR<DIOSTREAMTLS_MSG_EXTENSION*>    extensions;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/







