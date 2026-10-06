/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOWebClient.h
* 
* @class      DIOWEBCLIENT
* @brief      Data Input/Output Web Client class
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
#include "XPath.h"
#include "XSubject.h"

#include "DIOURL.h"
#include "DIOStream.h"
#include "DIOWebHeader.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


#define DIOWEBCLIENT_TIMEOUT            8
#define DIOWEBCLIENT_MAXSIZEBUFFER      (64*1024)
#define DIOWEBCLIENT_DEFAULTPORT        80
#define DIOWEBCLIENT_DEFAULTSECUREPORT  443

#define DIOWEBCLIENT_DEFAULTUSERAGENT   _L("User-Agent: Mozilla 5.0\r\n")

#define DIOWEBCLIENT_MAXREDIRECTS       5                                      // Hard cap on chained 301 redirects followed automatically

enum DIOWEBCLIENT_AUTHENTICATION_METHOD
{
  DIOWEBCLIENT_AUTHENTICATION_METHOD_UNKNOWN               = 0 ,
  DIOWEBCLIENT_AUTHENTICATION_METHOD_BASIC                     ,
  DIOWEBCLIENT_AUTHENTICATION_METHOD_DIGEST                    ,      
};


enum DIOWEBCLIENT_TRANSPORTPOLICY
{
  DIOWEBCLIENT_TRANSPORTPOLICY_HTTPS_ONLY               = 0 ,
  DIOWEBCLIENT_TRANSPORTPOLICY_HTTPS_PREFER                 ,
  DIOWEBCLIENT_TRANSPORTPOLICY_HTTP_ONLY                    ,
};


enum DIOWEBCLIENT_BODYMODE
{
  DIOWEBCLIENT_BODYMODE_UNKNOWN                     = 0 ,
  DIOWEBCLIENT_BODYMODE_NONE                            ,
  DIOWEBCLIENT_BODYMODE_CONTENTLENGTH                   ,
  DIOWEBCLIENT_BODYMODE_CHUNKED                         ,
  DIOWEBCLIENT_BODYMODE_CONNECTIONCLOSE                 ,
};


enum DIOWEBCLIENT_CHUNKEDSTATE
{
  DIOWEBCLIENT_CHUNKEDSTATE_SIZE                     = 0 ,
  DIOWEBCLIENT_CHUNKEDSTATE_DATA                         ,
  DIOWEBCLIENT_CHUNKEDSTATE_DATAEND                      ,
  DIOWEBCLIENT_CHUNKEDSTATE_TRAILERS                     ,
  DIOWEBCLIENT_CHUNKEDSTATE_END                          ,
};


enum DIOWEBCLIENT_CHUNKEDRESULT
{
  DIOWEBCLIENT_CHUNKEDRESULT_ERROR                   = -1 ,
  DIOWEBCLIENT_CHUNKEDRESULT_INCOMPLETE               = 0 ,
  DIOWEBCLIENT_CHUNKEDRESULT_COMPLETE                     ,
};


enum DIOWEBCLIENT_ERRORSTAGE
{
  DIOWEBCLIENT_ERRORSTAGE_NONE                    = 0 ,
  DIOWEBCLIENT_ERRORSTAGE_CONFIGURATION               ,
  DIOWEBCLIENT_ERRORSTAGE_DNS                         ,
  DIOWEBCLIENT_ERRORSTAGE_TCP                         ,
  DIOWEBCLIENT_ERRORSTAGE_PROXY                       ,
  DIOWEBCLIENT_ERRORSTAGE_TLSPROTOCOL                 ,
  DIOWEBCLIENT_ERRORSTAGE_TLSAUTHENTICATION           ,
  DIOWEBCLIENT_ERRORSTAGE_HTTP                        ,
};


enum DIOWEBCLIENT_ERROR
{
  DIOWEBCLIENT_ERROR_NONE                         = 0 ,
  DIOWEBCLIENT_ERROR_INVALIDARGUMENT                  ,
  DIOWEBCLIENT_ERROR_INVALIDURL                       ,
  DIOWEBCLIENT_ERROR_TRANSPORTUNAVAILABLE             ,
  DIOWEBCLIENT_ERROR_DNSRESOLUTION                    ,
  DIOWEBCLIENT_ERROR_TCPCONNECTION                    ,
  DIOWEBCLIENT_ERROR_TCPTIMEOUT                       ,
  DIOWEBCLIENT_ERROR_PROXY                            ,
  DIOWEBCLIENT_ERROR_TLSCONFIGURATION                 ,
  DIOWEBCLIENT_ERROR_TLSPROTOCOL                      ,
  DIOWEBCLIENT_ERROR_TLSAUTHENTICATION                ,
  DIOWEBCLIENT_ERROR_HTTPWRITE                        ,
  DIOWEBCLIENT_ERROR_HTTPRESPONSE                     ,
  DIOWEBCLIENT_ERROR_HTTPSTATUS                       ,
  DIOWEBCLIENT_ERROR_HTTPREDIRECT                     ,
  DIOWEBCLIENT_ERROR_HTTPBODY                         ,
  DIOWEBCLIENT_ERROR_HTTPCONTENTENCODING              ,
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XTIMER;
class XFILE;
class DIOSTREAMTCPIPCONFIG;
class DIOSTREAMTCPIPPROXYCFG;
class DIOSTREAMTCPIP;
class DIOSTREAMTLSCONFIG;
class DIOWEBCLIENT_XEVENT;
class COMPRESSMANAGER;

class DIOWEBCLIENT_OPERATIONERROR
{
  public:
                                              DIOWEBCLIENT_OPERATIONERROR       ();

    DIOWEBCLIENT_ERRORSTAGE                   GetStage                          ();
    DIOWEBCLIENT_ERROR                        GetError                          ();
    DIOSTREAMERROR                            GetStreamError                    ();
    int                                       GetHTTPStatus                     ();
    const XCHAR*                              GetDescription                    ();
    bool                                      IsSet                             ();

  private:

    friend class DIOWEBCLIENT;

    void                                      Set                               (DIOWEBCLIENT_ERRORSTAGE stage, DIOWEBCLIENT_ERROR error, DIOSTREAMERROR streamerror, int HTTPstatus = 0);
    void                                      Clean                             ();

    DIOWEBCLIENT_ERRORSTAGE                   stage;
    DIOWEBCLIENT_ERROR                        error;
    DIOSTREAMERROR                            streamerror;
    int                                       HTTPstatus;
};


class DIOWEBCLIENT_HEADER : public DIOWEBHEADER
{
  public:
                                              DIOWEBCLIENT_HEADER               ();
    virtual                                  ~DIOWEBCLIENT_HEADER               ();

    bool                                      GetMethod                         (XSTRING& method);
    bool                                      GetResource                       (XSTRING& resource);
    int                                       GetVersion                        ();
    int                                       GetSubVersion                     ();
    int                                       GetResultServer                   ();

    XQWORD                                    GetContentLength                  ();
    bool                                      HasContentLength                  ();
    bool                                      GetContentLength                  (XQWORD& contentlength);
    bool                                      GetTransferEncoding              (XSTRING& transferencoding);
    bool                                      GetContentEncoding                (XSTRING& contentencoding);
    bool                                      GetETag                           (XSTRING& etag);
    bool                                      GetWWWAuthenticate                (XSTRING& authenticate);
    bool                                      GetLocation                       (XSTRING& location);

  private:

    void                                      Clean                             ();
};

class DIOWEBCLIENT : public XSUBJECT
{
  public:
                                              DIOWEBCLIENT                      (XDWORD maxsizebuffer = DIOWEBCLIENT_MAXSIZEBUFFER);
    virtual                                  ~DIOWEBCLIENT                      ();

    int                                       Get_Port                          ();
    void                                      Set_Port                          (int port);

    DIOURL*                                   GetProxyURL                       ();
    DIOSTREAMTCPIPPROXYCFG*                 GetProxyCFG                       ();
    int                                       GetProxyPort                      ();
    void                                      SetProxyPort                      (int port);

    DIOWEBCLIENT_AUTHENTICATION_METHOD        GetAuthenticationMethod           ();
    void                                      SetAuthenticationMethod           (DIOWEBCLIENT_AUTHENTICATION_METHOD authenticationmethod);

    XSTRING*                                  GetLogin                          ();
    XSTRING*                                  GetPassword                       ();

    DIOSTREAMTCPIPCONFIG*                     GetStreamCFG                      ();
    DIOSTREAMTLSCONFIG*                       GetStreamTLSCFG                   ();

    DIOWEBCLIENT_HEADER*                      GetHeader                         ();
    
    bool                                      IsActiveDoStopHTTPError           ();
    void                                      DoStopHTTPError                   (bool activate);

    DIOWEBCLIENT_TRANSPORTPOLICY              GetTransportPolicy                ();
    void                                      SetTransportPolicy                (DIOWEBCLIENT_TRANSPORTPOLICY policy);
    bool                                      IsInsecureRedirectAllowed         ();
    void                                      AllowInsecureRedirect             (bool allow);
    bool                                      IsActiveAutoHTTPFallback          ();
    void                                      AutoHTTPFallback_Activate         (bool activate);

    DIOWEBCLIENT_OPERATIONERROR*              GetLastOperationError             ();
    DIOWEBCLIENT_OPERATIONERROR*              GetLastHTTPSAttemptError          ();
    bool                                      WasHTTPFallbackUsed               ();

    bool                                      IsActiveContentEncoding           ();
    void                                      ContentEncoding_Activate          (bool activate);

    bool                                      IsActiveCompressRequestBody       ();
    void                                      CompressRequestBody_Activate      (bool activate);

    bool                                      Get                               (DIOURL& url, XBUFFER& tobuffer , XCHAR* addheader = NULL, int timeout = DIOWEBCLIENT_TIMEOUT, XSTRING* localIP = NULL);
    bool                                      Get                               (XCHAR* url, XBUFFER& tobuffer, XCHAR* addheader = NULL, int timeout = DIOWEBCLIENT_TIMEOUT, XSTRING* localIP = NULL);
    bool                                      Get                               (DIOURL& url, XPATH& pathfile, XCHAR* addheader = NULL, int timeout = DIOWEBCLIENT_TIMEOUT, XSTRING* localIP = NULL);
    bool                                      Get                               (XCHAR* url, XPATH& pathfile, XCHAR* addheader = NULL, int timeout = DIOWEBCLIENT_TIMEOUT, XSTRING* localIP = NULL);

    bool                                      Put                               (DIOURL& url, XBUFFER& tobuffer , XCHAR* addheader = NULL, int timeout = DIOWEBCLIENT_TIMEOUT, XSTRING* localIP = NULL);
    bool                                      Put                               (XCHAR* url, XBUFFER& tobuffer, XCHAR* addheader = NULL, int timeout = DIOWEBCLIENT_TIMEOUT, XSTRING* localIP = NULL);
    bool                                      Put                               (DIOURL& url, XPATH& pathfile, XCHAR* addheader = NULL, int timeout = DIOWEBCLIENT_TIMEOUT, XSTRING* localIP = NULL);
    bool                                      Put                               (XCHAR* url, XPATH& pathfile, XCHAR* addheader = NULL, int timeout = DIOWEBCLIENT_TIMEOUT, XSTRING* localIP = NULL);

    bool                                      Post                              (DIOURL& url, XBUFFER& tobuffer , XBUFFER* postdata = NULL, XCHAR* addheader = NULL, int timeout = DIOWEBCLIENT_TIMEOUT, XSTRING* localIP = NULL);
    bool                                      Post                              (XCHAR* url, XBUFFER& tobuffer, XBUFFER* postdata = NULL, XCHAR* addheader = NULL, int timeout = DIOWEBCLIENT_TIMEOUT, XSTRING* localIP = NULL);
    bool                                      Post                              (DIOURL& url, XPATH& pathfile, XBUFFER* postdata = NULL, XCHAR* addheader = NULL, int timeout = DIOWEBCLIENT_TIMEOUT, XSTRING* localIP = NULL);
    bool                                      Post                              (XCHAR* url, XPATH& pathfile, XBUFFER* postdata = NULL, XCHAR* addheader = NULL, int timeout = DIOWEBCLIENT_TIMEOUT, XSTRING* localIP = NULL);
   
  private:

    bool                                      MakeOperation                     (DIOWEBHEADER_METHOD method, DIOURL& url, XBUFFER* postdata, XCHAR* addhead, int timeout, XSTRING* localIP, bool istobuffer, void* to, int redirectcount = 0, bool internaloperation = false, XTIMER* operationtimer = NULL);
    bool                                      Header_Read                       (int timeout);
    bool                                      Body_Read                         (DIOWEBCLIENT_BODYMODE bodymode, bool isTLS, XQWORD contentlength, int timeout, bool istobuffer, void* to, DIOWEBCLIENT_XEVENT& xevent);
    bool                                      Body_Decompress                   (bool istobuffer, void* to);
    bool                                      BodyBlock_Write                   (XBYTE* data, XDWORD size, bool istobuffer, void* to, XQWORD& totalsizeread, XQWORD contentlength, XTIMER* timerdownload, DIOWEBCLIENT_XEVENT& xevent);
    DIOWEBCLIENT_CHUNKEDRESULT                ChunkSize_Get                     (XBUFFER& input, XQWORD& chunksize);
    bool                                      Stream_Create                     (bool isTLS);
    bool                                      IsSecureURL                       (DIOURL& url);
    bool                                      RedirectOrigin_IsSame              (DIOURL& first, DIOURL& second);
    bool                                      Headers_FilterSensitive            (XCHAR* source, XSTRING& filtered);
    void                                      OperationError_Reset               ();
    void                                      OperationError_Set                 (DIOWEBCLIENT_ERROR error, DIOSTREAMERROR streamerror = DIOSTREAMERROR_NONE, int HTTPstatus = 0);
    void                                      OperationError_FromStream          (DIOWEBCLIENT_ERROR defaulterror);
    bool                                      OperationError_AllowsHTTPFallback  ();

    bool                                      GetSubStringWWWWAuthenticate      (XSTRING& www_authenticate, XCHAR* field, XSTRING& value, bool betweenquotation = true);

    void                                      Clean                             ();

    XDWORD                                    maxsizebuffer;

    XTIMER*                                   timerout;

    DIOSTREAMTCPIPCONFIG*                     diostreamcfg;
    DIOSTREAMTCPIP*                           diostream;

    int                                       port;
    bool                                      isportconfigured;
    bool                                      isstreamTLS;

    DIOWEBCLIENT_AUTHENTICATION_METHOD        authenticationmethod;

    XSTRING                                   login;
    XSTRING                                   password;

    DIOWEBCLIENT_HEADER                       header;
    bool                                      dostophttperror;
    DIOWEBCLIENT_TRANSPORTPOLICY              transportpolicy;
    bool                                      allowinsecureredirect;
    bool                                      autohttpfallbackactive;
    DIOWEBCLIENT_OPERATIONERROR               lastoperationerror;
    DIOWEBCLIENT_OPERATIONERROR               lastHTTPSattempterror;
    bool                                      HTTPfallbackused;

    bool                                      contentencodingactive;
    bool                                      compressrequestbody;





    COMPRESSMANAGER*                          compressmanager;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
