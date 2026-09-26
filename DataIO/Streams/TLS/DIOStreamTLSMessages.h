/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOStreamTLSMessages.h
* 
* @class      DIOSTREAMTLSMESSAGES
* @brief      Data Input/Output Stream TLS (Transport Layer Security) Messages class
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



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


#define DIOSTREAMTLS_MSG_VERSION_SSL_3_0                                      0x0300    // SSL v3.0
#define DIOSTREAMTLS_MSG_VERSION_TLS_1_0                                      0x0301    // TLS v1.0 
#define DIOSTREAMTLS_MSG_VERSION_TLS_1_1                                      0x0302    // TLS v1.1 
#define DIOSTREAMTLS_MSG_VERSION_TLS_1_2                                      0x0303    // TLS v1.2 
#define DIOSTREAMTLS_MSG_VERSION_TLS_1_3                                      0x0304    // TLS v1.3 

#define DIOSTREAMTLS_MSG_RANDOM_SIZE                                          32
#define DIOSTREAMTLS_MSG_SESSIONID_SIZE                                       32
#define DIOSTREAMTLS_MSG_HANDSHAKEHEADER_SIZE                                 4
#define DIOSTREAMTLS_MSG_MAXLENGTH24                                          0x00FFFFFF
#define DIOSTREAMTLS_MSG_MAXHANDSHAKESIZE                                     (4*1024*1024)

#define DIOSTREAMTLS_AESGCM_MAXKEYUSAGERECORDS                              23726566ULL     // floor(2^24.5), RFC 8446 section 5.5
#define DIOSTREAMTLS_AESGCM_PROACTIVEKEYUSAGERECORDS                              (((XQWORD)1) << 24) // Conservative operational threshold
#define DIOSTREAMTLS_CHACHA20POLY1305_PROACTIVEKEYUSAGERECORDS              0xFFFFFFFFFFFFFF00ULL // Update before the record sequence can wrap

#define DIOSTREAMTLS_MSG_CIPHER_DES_CBC3_SHA                                  0x000A    // SSLv3   
#define DIOSTREAMTLS_MSG_CIPHER_RSA_WITH_AES_128_CBC_SHA                      0x002F    // SSLv3   
#define DIOSTREAMTLS_MSG_CIPHER_RSA_WITH_AES_256_CBC_SHA                      0x0035    // SSLv3  
#define DIOSTREAMTLS_MSG_CIPHER_RSA_WITH_AES_128_CBC_SHA256                   0x003C    // TLSv1.2 
#define DIOSTREAMTLS_MSG_CIPHER_RSA_WITH_AES_256_CBC_SHA256                   0x003D    // TLSv1.2 
#define DIOSTREAMTLS_MSG_CIPHER_RSA_WITH_AES_128_GCM_SHA256                   0x009C    // TLSv1.2 
#define DIOSTREAMTLS_MSG_CIPHER_RSA_WITH_AES_256_GCM_SHA384                   0x009D    // TLSv1.2 
#define DIOSTREAMTLS_MSG_CIPHER_DHE_RSA_WITH_AES_128_CBC_SHA                  0x0033
#define DIOSTREAMTLS_MSG_CIPHER_DHE_RSA_WITH_AES_256_CBC_SHA                  0x0039
#define DIOSTREAMTLS_MSG_CIPHER_DHE_RSA_WITH_AES_128_CBC_SHA256               0x0067    // TLSv1.2 
#define DIOSTREAMTLS_MSG_CIPHER_DHE_RSA_WITH_AES_256_CBC_SHA256               0x006B    // TLSv1.2 
#define DIOSTREAMTLS_MSG_CIPHER_DHE_RSA_WITH_AES_128_GCM_SHA256               0x009E    // TLSv1.2 
#define DIOSTREAMTLS_MSG_CIPHER_DHE_RSA_WITH_AES_256_GCM_SHA384               0x009F    // TLSv1.2 
#define DIOSTREAMTLS_MSG_CIPHER_AES_128_GCM_SHA256                            0x1301    // TLSv1.3 
#define DIOSTREAMTLS_MSG_CIPHER_AES_256_GCM_SHA384                            0x1302    // TLSv1.3 
#define DIOSTREAMTLS_MSG_CIPHER_CHACHA20_POLY1305_SHA256                      0x1303    // TLSv1.3 
#define DIOSTREAMTLS_MSG_CIPHER_AES_128_CCM_SHA256                            0x1304
#define DIOSTREAMTLS_MSG_CIPHER_AES_128_CCM_8_SHA256                          0x1305
#define DIOSTREAMTLS_MSG_CIPHER_ECDHE_ECDSA_WITH_AES_128_CBC_SHA              0xC009    // TLSv1   
#define DIOSTREAMTLS_MSG_CIPHER_ECDHE_ECDSA_WITH_AES_256_CBC_SHA              0xC00A    // TLSv1  
#define DIOSTREAMTLS_MSG_CIPHER_ECDHE_RSA_WITH_AES_128_CBC_SHA                0xC013    // TLSv1
#define DIOSTREAMTLS_MSG_CIPHER_ECDHE_RSA_WITH_AES_256_CBC_SHA                0xC014    // TLSv1 
#define DIOSTREAMTLS_MSG_CIPHER_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256           0xC023    // TLSv1.2 
#define DIOSTREAMTLS_MSG_CIPHER_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384           0xC024    // TLSv1.2 
#define DIOSTREAMTLS_MSG_CIPHER_ECDHE_RSA_WITH_AES_128_CBC_SHA256             0xC027    // TLSv1.2
#define DIOSTREAMTLS_MSG_CIPHER_ECDHE_RSA_WITH_AES256_SHA384                  0xC028    // TLSv1.2 
#define DIOSTREAMTLS_MSG_CIPHER_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256           0xC02B    // TLSv1.2 
#define DIOSTREAMTLS_MSG_CIPHER_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384           0xC02C    // TLSv1.2 
#define DIOSTREAMTLS_MSG_CIPHER_ECDHE_RSA_WITH_AES_128_GCM_SHA256             0xC02F    // TLSv1.2

#define DIOSTREAMTLS_MSG_CIPHER_ECDHE_RSA_WITH_AES_256_GCM_SHA384             0xC030    // TLSv1.2 
#define DIOSTREAMTLS_MSG_CIPHER_ECDHE_RSA_WITH_CHACHA20_POLY1305_SHA256       0xCCA8    // TLSv1.2
#define DIOSTREAMTLS_MSG_CIPHER_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256     0xCCA9    // TLSv1.2
#define DIOSTREAMTLS_MSG_CIPHER_DHE_RSA_WITH_CHACHA20_POLY1305_SHA256         0xCCAA    // TLSv1.2 
#define DIOSTREAMTLS_MSG_CIPHER_FALLBACK_SCSV                                 0x5600    // RFC 7507: only on a real protocol fallback retry

// TLS 1.2 (RFC 5246 + RFC 5289) suites supported by DIOSTREAMTLS12: ECDHE key exchange with AEAD GCM only.
// The CBC ones are deliberately left out; they need MAC-then-encrypt, and no modern server requires them.
#define DIOSTREAMTLS12_CIPHER_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256             0xC02B
#define DIOSTREAMTLS12_CIPHER_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384             0xC02C
#define DIOSTREAMTLS12_CIPHER_ECDHE_RSA_WITH_AES_128_GCM_SHA256               0xC02F
#define DIOSTREAMTLS12_CIPHER_ECDHE_RSA_WITH_AES_256_GCM_SHA384               0xC030
                                        
#define DIOSTREAMTLS_MSG_COMPRESS_METHOD_NULL                                 0x00

#define DIOSTREAMTLS_MSG_CURVEID_SECP256R1MLKEM768                               0x11EB    // RFC 10024
#define DIOSTREAMTLS_MSG_CURVEID_X25519MLKEM768                                  0x11EC    // RFC 10024
#define DIOSTREAMTLS_MSG_CURVEID_SECP384R1MLKEM1024                              0x11ED    // RFC 10024
#define DIOSTREAMTLS_MSG_CURVEID_X25519	                                      0x001D    // Optimizated              // Curvas modernas (TLS 1.3 recomendadas)
#define DIOSTREAMTLS_MSG_CURVEID_X448	                                        0x001E	
#define DIOSTREAMTLS_MSG_CURVEID_SECP256R1                                    0x0017	  // 256 bits
#define DIOSTREAMTLS_MSG_CURVEID_SECP384R1	                                  0x0018    // 384 bits 	
#define DIOSTREAMTLS_MSG_CURVEID_SECP521R1	                                  0x0019	  // 521 bits
#define DIOSTREAMTLS_MSG_CURVEID_FFDHE2048	                                  0x0100		// 2048 bits                // Finite Field DH (FFDHE, TLS 1.3)
#define DIOSTREAMTLS_MSG_CURVEID_FFDHE3072	                                  0x0101		// 3072 bits
#define DIOSTREAMTLS_MSG_CURVEID_FFDHE4096	                                  0x0102		// 4096 bits
#define DIOSTREAMTLS_MSG_CURVEID_FFDHE6144	                                  0x0103		// 6144 bits
#define DIOSTREAMTLS_MSG_CURVEID_FFDHE8192	                                  0x0104		// 8192 bits
#define DIOSTREAMTLS_MSG_CURVEID_SECP192R1   	                                0x0013	  // 192 bits (obsolete)      // Old Curves  (TLS 1.2)
#define DIOSTREAMTLS_MSG_CURVEID_SECP224R1 	                                  0x0015	  // 254 bits (obsolete)
#define DIOSTREAMTLS_MSG_CURVEID_SECP256K1	                                  0x0016	  // Bitcoin, not for TLS
#define DIOSTREAMTLS_MSG_CURVEID_BRAINPOOLP256R1	                            0x001A	  // Alternativa europea
#define DIOSTREAMTLS_MSG_CURVEID_BRAINPOOLP384R1	                            0x001B		
#define DIOSTREAMTLS_MSG_CURVEID_BRAINPOOLP512R1	                            0x001C	

#define DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA256	                0x0804	  // RSA-PSS con SHA-256      // Algoritm RSA-PSS (TLS 1.3 Mandatory)
#define DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA384	                0x0805	  // RSA-PSS con SHA-384
#define DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_RSAE_SHA512	                0x0806	  // RSA-PSS con SHA-512
#define DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA256	                  0x0809	  // RSA-PSS puro, SHA-256
#define DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA384	                  0x080A	  // RSA-PSS puro, SHA-384
#define DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PSS_PSS_SHA512	                  0x080B	  // RSA-PSS puro, SHA-512
#define DIOSTREAMTLS_MSG_SIGNATURESCHEME_ECDSA_SECP256R1_SHA256	              0x0403	  // ECDSA P-256 con SHA-256  // Algoritm ECDSA
#define DIOSTREAMTLS_MSG_SIGNATURESCHEME_ECDSA_SECP384R1_SHA384	              0x0503	  // ECDSA P-384 con SHA-384
#define DIOSTREAMTLS_MSG_SIGNATURESCHEME_ECDSA_SECP521R1_SHA512	              0x0603	  // ECDSA P-521 con SHA-512
#define DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PKCS1_SHA256	                    0x0401                                // Algoritm RSA PKCS#1 v1.5 (obsolete, only TLS 1.2)
#define DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PKCS1_SHA384	                    0x0501    
#define DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PKCS1_SHA512	                    0x0601    
#define DIOSTREAMTLS_MSG_SIGNATURESCHEME_ED25519	                            0x0807                                // Algoritm EdDSA (TLS 1.3 last)
#define DIOSTREAMTLS_MSG_SIGNATURESCHEME_ED448	                              0x0808     
#define DIOSTREAMTLS_MSG_SIGNATURESCHEME_RSA_PKCS1_SHA1	                      0x0201                                // Other old (only TLS 1.2, rare)
#define DIOSTREAMTLS_MSG_SIGNATURESCHEME_ECDSA_SHA1	                          0x0203       

#define DIOSTREAMTLS_MSG_PSKKEYEXCHANGEMODE_PSK                             	0x00		  // PSK pure, without Diffie-Hellman 
#define DIOSTREAMTLS_MSG_PSKKEYEXCHANGEMODE_PSK_DHE	                          0x01	    // PSK with Diffie-Hellman (DHE) 


enum DIOSTREAMTLS_CONTENTTYPE
{
   DIOSTREAMTLS_MSG_CONTENTTYPE_CHANGE_CIPHER_SPEC                            =    0x14  , 
   DIOSTREAMTLS_MSG_CONTENTTYPE_ALERT                                         =    0x15  , 
   DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE                                     =    0x16  ,
   DIOSTREAMTLS_MSG_CONTENTTYPE_APPLICATION_DATA                              =    0x17  , 
   DIOSTREAMTLS_MSG_CONTENTTYPE_TLS_1_3_HEARTBEAT                             =    0x18  , 
};


enum DIOSTREAMTLS_CONTENTTYPE_HANDSHAKE
{
  DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_HELLO_REQUEST                        =      0  , 
  DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_CLIENT_HELLO                         =      1  , 
  DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_SERVER_HELLO                         =      2  ,
  DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_NEW_SESSION_TICKET                   =      4  ,
  DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_END_OF_EARLY_DATA                    =      5  ,
  DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_ENCRYPTED_EXTENSIONS                 =      8  ,
  DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_CERTIFICATE                          =     11  , 
  DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_SERVER_KEY_EXCHANGE                  =     12  ,
  DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_CERTIFICATE_REQUEST                  =     13  , 
  DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_SERVER_HELLO_DONE                    =     14  ,
  DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_CERTIFICATE_VERIFY                   =     15  , 
  DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_CLIENT_KEY_EXCHANGE                  =     16  ,
  DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_FINISHED                             =     20  , 
  DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_KEY_UPDATE                           =     24  ,
  DIOSTREAMTLS_MSG_CONTENTTYPE_HANDSHAKE_MESSAGE_HASH                         =    254  ,      
 };


enum DIOSTREAMTLS_ALERT_LEVEL
{
  DIOSTREAMTLS_ALERT_LEVEL_WARNING                                            =      1  ,
  DIOSTREAMTLS_ALERT_LEVEL_FATAL                                              =      2  ,
};


enum DIOSTREAMTLS_ALERT_DESCRIPTION
{
  DIOSTREAMTLS_ALERT_DESCRIPTION_CLOSE_NOTIFY                                =      0  ,
  DIOSTREAMTLS_ALERT_DESCRIPTION_UNEXPECTED_MESSAGE                           =     10  ,
  DIOSTREAMTLS_ALERT_DESCRIPTION_BAD_RECORD_MAC                               =     20  ,
  DIOSTREAMTLS_ALERT_DESCRIPTION_RECORD_OVERFLOW                              =     22  ,
  DIOSTREAMTLS_ALERT_DESCRIPTION_HANDSHAKE_FAILURE                            =     40  ,
  DIOSTREAMTLS_ALERT_DESCRIPTION_BAD_CERTIFICATE                              =     42  ,
  DIOSTREAMTLS_ALERT_DESCRIPTION_UNSUPPORTED_CERTIFICATE                      =     43  ,
  DIOSTREAMTLS_ALERT_DESCRIPTION_CERTIFICATE_REVOKED                          =     44  ,
  DIOSTREAMTLS_ALERT_DESCRIPTION_CERTIFICATE_EXPIRED                          =     45  ,
  DIOSTREAMTLS_ALERT_DESCRIPTION_CERTIFICATE_UNKNOWN                          =     46  ,
  DIOSTREAMTLS_ALERT_DESCRIPTION_ILLEGAL_PARAMETER                            =     47  ,
  DIOSTREAMTLS_ALERT_DESCRIPTION_UNKNOWN_CA                                   =     48  ,
  DIOSTREAMTLS_ALERT_DESCRIPTION_ACCESS_DENIED                                =     49  ,
  DIOSTREAMTLS_ALERT_DESCRIPTION_DECODE_ERROR                                 =     50  ,
  DIOSTREAMTLS_ALERT_DESCRIPTION_DECRYPT_ERROR                                =     51  ,
  DIOSTREAMTLS_ALERT_DESCRIPTION_PROTOCOL_VERSION                             =     70  ,
  DIOSTREAMTLS_ALERT_DESCRIPTION_INSUFFICIENT_SECURITY                        =     71  ,
  DIOSTREAMTLS_ALERT_DESCRIPTION_INTERNAL_ERROR                               =     80  ,
  DIOSTREAMTLS_ALERT_DESCRIPTION_INAPPROPRIATE_FALLBACK                       =     86  ,
  DIOSTREAMTLS_ALERT_DESCRIPTION_USER_CANCELED                                =     90  ,
  DIOSTREAMTLS_ALERT_DESCRIPTION_MISSING_EXTENSION                            =    109  ,
  DIOSTREAMTLS_ALERT_DESCRIPTION_UNSUPPORTED_EXTENSION                        =    110  ,
  DIOSTREAMTLS_ALERT_DESCRIPTION_UNRECOGNIZED_NAME                            =    112  ,
  DIOSTREAMTLS_ALERT_DESCRIPTION_BAD_CERTIFICATE_STATUS_RESPONSE              =    113  ,
  DIOSTREAMTLS_ALERT_DESCRIPTION_UNKNOWN_PSK_IDENTITY                         =    115  ,
  DIOSTREAMTLS_ALERT_DESCRIPTION_CERTIFICATE_REQUIRED                         =    116  ,
  DIOSTREAMTLS_ALERT_DESCRIPTION_NO_APPLICATION_PROTOCOL                      =    120  ,
};


enum DIOSTREAMTLS_ALPN_TYPE
{
  DIOSTREAMTLS_ALPN_TYPE_UNKNOWN        = -1 ,
  DIOSTREAMTLS_ALPN_TYPE_HTTP_1_1       ,
  DIOSTREAMTLS_ALPN_TYPE_HTTP_2         ,
  DIOSTREAMTLS_ALPN_TYPE_HTTP_3
};


// Shared between the TLS 1.3 key schedule (DIOSTREAMTLS13KEYSCHEDULE) and the TLS 1.2 one (DIOSTREAMTLS12KEYSCHEDULE
// / DIOSTREAMTLS12RECORD / DIOSTREAMTLS12SESSION): every secret or key is asked for by role/direction, never by a
// version-specific type, which is what lets both parallel implementations share this one vocabulary. Kept here
// (not in either version-numbered KeySchedule header) precisely because it is common ground, not exclusive to one.

enum DIOSTREAMTLSKEYSCHEDULE_ROLE
{
  DIOSTREAMTLSKEYSCHEDULE_ROLE_CLIENT               = 0 ,
  DIOSTREAMTLSKEYSCHEDULE_ROLE_SERVER                   ,
};

enum DIOSTREAMTLSKEYSCHEDULE_DIRECTION
{
  DIOSTREAMTLSKEYSCHEDULE_DIRECTION_LOCAL           = 0 ,                       // What this end writes
  DIOSTREAMTLSKEYSCHEDULE_DIRECTION_REMOTE              ,                       // What the other end writes

  DIOSTREAMTLSKEYSCHEDULE_MAXDIRECTIONS
};





/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOSTREAMTLS_MSG_INTERFACE
{ 
  public:

                                            DIOSTREAMTLS_MSG_INTERFACE                        ();                                            
    virtual                                ~DIOSTREAMTLS_MSG_INTERFACE                        ();
                                            
    virtual bool                            SetToBuffer                                       (XBUFFER& buffer, bool showdebug) = 0;
    virtual bool                            GetFromBuffer                                     (XBUFFER& buffer, bool showdebug) = 0;

    XDWORD                                  GetLengthBuffer                                   ();
    
  private:

    void                                    Clean                                             ();    
};

#define DIOSTREAMTLS_MSG_RECORDHEADER_SIZE                                    5

class DIOSTREAMTLS_MSG_RECORDHEADER : public DIOSTREAMTLS_MSG_INTERFACE
{
  public:
                                            DIOSTREAMTLS_MSG_RECORDHEADER                     ();
    virtual                                ~DIOSTREAMTLS_MSG_RECORDHEADER                     ();

    DIOSTREAMTLS_CONTENTTYPE                GetContenType                                     ();
    void                                    SetContenType                                     (DIOSTREAMTLS_CONTENTTYPE contenttype);

    XWORD                                   GetProtocolVersion                                ();
    void                                    SetProtocolVersion                                (XWORD protocolversion);

    XWORD                                   GetLength                                         ();
    void                                    SetLength                                         (XWORD length);

    bool                                    SetToBuffer                                       (XBUFFER& buffer, bool showdebug);
    bool                                    GetFromBuffer                                     (XBUFFER& buffer, bool showdebug);

    bool                                    Peek                                              (XBUFFER& buffer);

  private:

    void                                    Clean                                             ();

    DIOSTREAMTLS_CONTENTTYPE                contenttype;
    XWORD                                   protocolversion;
    XWORD                                   length;
};

class DIOSTREAMTLS_MSG_HANDSHAKE : public DIOSTREAMTLS_MSG_INTERFACE
{
  public:
                                            DIOSTREAMTLS_MSG_HANDSHAKE                      ();
    virtual                                ~DIOSTREAMTLS_MSG_HANDSHAKE                      ();

    XBYTE                                   GetMsgType                                      ();
    void                                    SetMsgType                                      (XBYTE msgtype);

    XDWORD                                  GetLength                                       ();
    void                                    SetLength                                       (XDWORD length);

    XBUFFER*                                GetBody                                         ();

    bool                                    SetToBuffer                                     (XBUFFER& buffer, bool showdebug);
    bool                                    GetFromBuffer                                   (XBUFFER& buffer, bool showdebug);

    static bool                             Message_Extract                                 (XBUFFER& input, XBUFFER& message);

  private:

    void                                    Clean                                           ();

    XBYTE                                   msgtype;
    XDWORD                                  length;
    XBUFFER                                 body;
};


class DIOSTREAMTLS_MSG_ALERT : public DIOSTREAMTLS_MSG_INTERFACE
{
  public:
                                            DIOSTREAMTLS_MSG_ALERT                          ();
    virtual                                ~DIOSTREAMTLS_MSG_ALERT                          ();

    DIOSTREAMTLS_ALERT_LEVEL                GetLevel                                        ();
    void                                    SetLevel                                        (DIOSTREAMTLS_ALERT_LEVEL level);

    DIOSTREAMTLS_ALERT_DESCRIPTION          GetDescription                                  ();
    void                                    SetDescription                                  (DIOSTREAMTLS_ALERT_DESCRIPTION description);

    static XCHAR*                           GetDescriptionString                            (DIOSTREAMTLS_ALERT_DESCRIPTION description);

    bool                                    SetToBuffer                                     (XBUFFER& buffer, bool showdebug);
    bool                                    GetFromBuffer                                   (XBUFFER& buffer, bool showdebug);

  private:

    void                                    Clean                                           ();

    XBYTE                                   level;
    XBYTE                                   description;
};

bool                                        DIOSTREAMTLS_MSG_AddLength24                    (XBUFFER& buffer, XDWORD length);
bool                                        DIOSTREAMTLS_MSG_ExtractLength24                (XBUFFER& buffer, XDWORD& length);

template<typename T>
class DIOSTREAMTLS_MSG_RECORD : public DIOSTREAMTLS_MSG_INTERFACE
{
  public:
                                            DIOSTREAMTLS_MSG_RECORD                           ()
                                            {
                                              Clean();
                                            }

    virtual                                ~DIOSTREAMTLS_MSG_RECORD                           ()
                                            { 
                                              Clean();
                                            }

    DIOSTREAMTLS_MSG_RECORDHEADER*          GetHeader                                         ()
                                            {
                                              return &header;
                                            }

    DIOSTREAMTLS_CONTENTTYPE                GetContenType                                     ()
                                            {
                                              return header.GetContenType();
                                            }

    void                                    SetContenType                                     (DIOSTREAMTLS_CONTENTTYPE contenttype)
                                            {
                                              header.SetContenType(contenttype);
                                            }

    XWORD                                   GetProtocolVersion                                ()
                                            {
                                              return header.GetProtocolVersion();
                                            }

    void                                    SetProtocolVersion                                (XWORD protocolversion)
                                            {
                                              header.SetProtocolVersion(protocolversion);
                                            }

    XWORD                                   GetLength                                         ()
                                            {
                                              return header.GetLength();
                                            }

    void                                    SetLength                                         (XWORD length)
                                            {
                                              header.SetLength(length);
                                            }

    void                                    CalculateLength                                   ()
                                            {
                                              XBUFFER fragmentbuffer;

                                              if(fragment.SetToBuffer(fragmentbuffer, false))
                                                {
                                                  SetLength((XWORD)fragmentbuffer.GetSize());
                                                }
                                            } 

    T*                                      GetFragment                                       ()
                                            {
                                              return &fragment;
                                            } 
 
    bool                                    SetToBuffer                                       (XBUFFER& buffer, bool showdebug)
                                            {
                                              XBUFFER fragmentbuffer;

                                              if(!fragment.SetToBuffer(fragmentbuffer, showdebug))
                                                {
                                                  return false;
                                                }

                                              if(fragmentbuffer.GetSize() > 0xFFFF)
                                                {
                                                  return false;
                                                }

                                              header.SetLength((XWORD)fragmentbuffer.GetSize());

                                              buffer.Delete();

                                              if(!header.SetToBuffer(buffer, showdebug))
                                                {
                                                  return false;
                                                }

                                              return buffer.Add(fragmentbuffer);
                                            }

    bool                                    GetFromBuffer                                     (XBUFFER& buffer, bool showdebug)
                                            {
                                              XBUFFER                       workbuffer;
                                              XBUFFER                       fragmentbuffer;
                                              DIOSTREAMTLS_MSG_RECORDHEADER newheader;
                                              XDWORD                        sizeconsumed = 0;

                                              workbuffer.Add(buffer);

                                              if(!newheader.GetFromBuffer(workbuffer, showdebug))
                                                {
                                                  return false;
                                                }

                                              if(workbuffer.GetSize() < newheader.GetLength())
                                                {
                                                  return false;
                                                }

                                              fragmentbuffer.Resize(newheader.GetLength());

                                              if(newheader.GetLength())
                                                {
                                                  if(workbuffer.Extract(fragmentbuffer.Get(), 0, newheader.GetLength()) != newheader.GetLength())
                                                    {
                                                      return false;
                                                    }
                                                }

                                              if(!fragment.GetFromBuffer(fragmentbuffer, showdebug))
                                                {
                                                  return false;
                                                }

                                              if(!fragmentbuffer.IsEmpty())
                                                {
                                                  return false;
                                                }

                                              header       = newheader;
                                              sizeconsumed = DIOSTREAMTLS_MSG_RECORDHEADER_SIZE + newheader.GetLength();

                                              return (buffer.Extract(NULL, 0, sizeconsumed) == sizeconsumed);
                                            }

  private:

    void                                    Clean                                             ()
                                            {

                                            }

    DIOSTREAMTLS_MSG_RECORDHEADER           header;
    T                                       fragment;
};

template<typename T>
class DIOSTREAMTLS_MSG_FRAGMENT : public DIOSTREAMTLS_MSG_INTERFACE
{ 
  public:
                                            DIOSTREAMTLS_MSG_FRAGMENT                         ()
                                            {
                                              Clean();
                                            }

    virtual                                ~DIOSTREAMTLS_MSG_FRAGMENT                         ()
                                            {
                                              Clean();
                                            }

    XBYTE                                   GetMsgType                                        ()
                                            {
                                              return msgtype;
                                            }

    void                                    SetMsgType                                        (XBYTE msgtype)
                                            {
                                              this->msgtype = msgtype;
                                            }

    XDWORD                                  GetLength                                         ()
                                            {
                                              return length;    
                                            } 
    
    void                                    SetLength                                         (XDWORD length)
                                            {
                                              this->length = length;    
                                            } 

    T*                                      GetBody                                           ()
                                            {
                                              return &body;  
                                            }  

    bool                                    SetToBuffer                                       (XBUFFER& buffer, bool showdebug)
                                            {
                                              XBUFFER bodybuffer;

                                              if(!body.SetToBuffer(bodybuffer, showdebug))
                                                {
                                                  return false;
                                                }

                                              if(bodybuffer.GetSize() > DIOSTREAMTLS_MSG_MAXLENGTH24)
                                                {
                                                  return false;
                                                }

                                              length = bodybuffer.GetSize();

                                              if(!buffer.Add((XBYTE)msgtype))
                                                {
                                                  return false;
                                                }

                                              if(!DIOSTREAMTLS_MSG_AddLength24(buffer, length))
                                                {
                                                  return false;
                                                }

                                              return buffer.Add(bodybuffer);
                                            }

    bool                                    GetFromBuffer                                     (XBUFFER& buffer, bool showdebug)
                                            {
                                              XBUFFER workbuffer;
                                              XBUFFER bodybuffer;
                                              XBYTE   newmsgtype = 0;
                                              XDWORD  newlength  = 0;
                                              XDWORD  sizeconsumed;

                                              workbuffer.Add(buffer);

                                              if(workbuffer.GetSize() < DIOSTREAMTLS_MSG_HANDSHAKEHEADER_SIZE)
                                                {
                                                  return false;
                                                }

                                              if(!workbuffer.Extract(newmsgtype))
                                                {
                                                  return false;
                                                }

                                              if(!DIOSTREAMTLS_MSG_ExtractLength24(workbuffer, newlength))
                                                {
                                                  return false;
                                                }

                                              if(workbuffer.GetSize() < newlength)
                                                {
                                                  return false;
                                                }

                                              bodybuffer.Resize(newlength);

                                              if(newlength)
                                                {
                                                  if(workbuffer.Extract(bodybuffer.Get(), 0, newlength) != newlength)
                                                    {
                                                      return false;
                                                    }
                                                }

                                              if(!body.GetFromBuffer(bodybuffer, showdebug))
                                                {
                                                  return false;
                                                }

                                              if(!bodybuffer.IsEmpty())
                                                {
                                                  return false;
                                                }

                                              msgtype     = newmsgtype;
                                              length      = newlength;
                                              sizeconsumed = DIOSTREAMTLS_MSG_HANDSHAKEHEADER_SIZE + newlength;

                                              return (buffer.Extract(NULL, 0, sizeconsumed) == sizeconsumed);
                                            }
 
  private:

    void                                    Clean                                             ()
                                            {
                                              msgtype = 0;
                                              length  = 0;
                                            }    


    XBYTE                                   msgtype;      
    XDWORD                                  length;      
    T                                       body;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
