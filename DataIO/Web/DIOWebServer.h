/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOWebServer.h
* 
* @class      DIOWEBSERVER
* @brief      Data Input/Output Web Server class
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

#include "XBase.h"
#include "XVector.h"
#include "XBuffer.h"
#include "XString.h"
#include "XEvent.h"
#include "XObserver.h"
#include "XSubject.h"
#include "XPath.h"
#include "XThreadCollected.h"

#include "DIOStream.h"

#include "DIOWebHeader.h"
#include <atomic>
#include "DIOWebClient.h"

#include "DIOWebServer_Plugin.h"
#include "DIOWebServer_PluginsManager.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum DIOWEBSERVER_TYPE
{
  DIOWEBSERVER_TYPE_NONE                                  = 0 ,
  DIOWEBSERVER_TYPE_SYMPLE                                    ,
  DIOWEBSERVER_TYPE_FULL
};


enum DIOWEBSERVER_ENCODINGFLAGS
{
  DIOWEBSERVER_ENCODINGFLAGS_NONE                         = 0     ,
  DIOWEBSERVER_ENCODINGFLAGS_GZIP                         = 1<<1  ,
  DIOWEBSERVER_ENCODINGFLAGS_DEFLATE                      = 1<<2  ,
  DIOWEBSERVER_ENCODINGFLAGS_COMPRESS                     = 1<<3  ,
  DIOWEBSERVER_ENCODINGFLAGS_SHAREDDICTIONARY             = 1<<4
};


enum DIOWEBSERVER_CONNECTION_MODE
{
   DIOWEBSERVER_CONNECTION_MODE_UNKNOWN                   = 0 ,
   DIOWEBSERVER_CONNECTION_MODE_NORMAL                        ,
   DIOWEBSERVER_CONNECTION_MODE_KEEPALIVE                     ,
   DIOWEBSERVER_CONNECTION_MODE_WEBSOCKET
};


enum DIOWEBSERVER_WEBSOCKET_OPCODE
{
   DIOWEBSERVER_WEBSOCKET_OPCODE_CONTINUATION_FRAME       = 0 ,     // 0x00 Continuation Frame
   DIOWEBSERVER_WEBSOCKET_OPCODE_TEXT_FRAME                   ,     // 0x01 Text Frame
   DIOWEBSERVER_WEBSOCKET_OPCODE_BINARY_FRAME                 ,     // 0x02 Binary Frame
   DIOWEBSERVER_WEBSOCKET_OPCODE_RESERVED1                    ,     // 0x03 - 0x07 Reserved
   DIOWEBSERVER_WEBSOCKET_OPCODE_RESERVED2                    ,     //
   DIOWEBSERVER_WEBSOCKET_OPCODE_RESERVED3                    ,     //
   DIOWEBSERVER_WEBSOCKET_OPCODE_RESERVED4                    ,     //
   DIOWEBSERVER_WEBSOCKET_OPCODE_RESERVED5                    ,     //
   DIOWEBSERVER_WEBSOCKET_OPCODE_CONNECTION_CLOSE             ,     // 0x08 Close Connection
   DIOWEBSERVER_WEBSOCKET_OPCODE_PING                         ,     // 0x09 Ping
   DIOWEBSERVER_WEBSOCKET_OPCODE_PONG                         ,     // 0x0A Pong
   DIOWEBSERVER_WEBSOCKET_OPCODE_RESERVED6                    ,     // 0x0B-0x0F Reserved
   DIOWEBSERVER_WEBSOCKET_OPCODE_RESERVED7                    ,     //
   DIOWEBSERVER_WEBSOCKET_OPCODE_RESERVED8                    ,     //
   DIOWEBSERVER_WEBSOCKET_OPCODE_RESERVED9                    ,     //
   DIOWEBSERVER_WEBSOCKET_OPCODE_RESERVED10                   ,     //
};

#define DIOWEBSERVER_DEFAULTCONNECTIONTIMEOUT     3     // Seconds
#define DIOWEBSERVER_DEFAULTTIMEOUTSERVERPAGE     30    // Seconds
#define DIOWEBSERVER_DEFAULTTLSHANDSHAKETIMEOUT    10    // Seconds

#define DIOWEBSERVER_MAXPAGECONNECTIONS           10

#define DIOWEBSERVER_IDSERVER                     _L("GEN Web Server v2.0")
#define DIOWEBSERVER_LOGSECTIONID                 _L("WEB Server")
#define DIOWEBSERVER_LOGSECTIONID_VERBOSE         _L("WEB Server Verbose")

#define DIOWEBSERVER_AUTHENTICATION_ALLGUESTS     _L("*")
#define DIOWEBSERVER_AUTHENTICATION_DEFAULTREALM  _L("GEN Web Server")

#define DIOWEBSERVER_DEFAULTPORT                  80
#define DIOWEBSERVER_KEEPALIVE                    5     // Seconds
#define DIOWEBSERVER_KEEPALIVE_MAXRESOURCES       100
#define DIOWEBSERVER_MAXSENDFILE                  DIOSTREAM_MAXBUFFER/2
#define DIOWEBSERVER_MAXBUFFER                    (512*1024)

#define DIOWEBSERVER_MAXBUFFERFILE                ((1024*1024)*300)
#define DIOWEBSERVER_DEFAULTMAXHEADERSIZE          (64*1024)
#define DIOWEBSERVER_DEFAULTMAXBODYSIZE            (16*1024*1024)

struct DIOWEBSERVER_TIMEOUTS
{
  DIOWEBSERVER_TIMEOUTS() : connectaccept(DIOWEBSERVER_DEFAULTCONNECTIONTIMEOUT),
                            tlshandshake(DIOWEBSERVER_DEFAULTTLSHANDSHAKETIMEOUT),
                            httpheaders(DIOWEBSERVER_DEFAULTCONNECTIONTIMEOUT),
                            httpbody(DIOWEBSERVER_DEFAULTCONNECTIONTIMEOUT),
                            keepalive(DIOWEBSERVER_KEEPALIVE) {}

  int connectaccept;
  int tlshandshake;
  int httpheaders;
  int httpbody;
  int keepalive;
};

struct DIOWEBSERVER_LIMITS
{
  DIOWEBSERVER_LIMITS() : maximumheadersize(DIOWEBSERVER_DEFAULTMAXHEADERSIZE),
                          maximumbodysize(DIOWEBSERVER_DEFAULTMAXBODYSIZE) {}

  XDWORD maximumheadersize;
  XDWORD maximumbodysize;
};

typedef struct
{
  XBYTE  size  : 7;
  XBYTE  mask  : 1;
  XBYTE  opcode: 4;
  XBYTE  rsv3  : 1;
  XBYTE  rsv2  : 1;
  XBYTE  rsv1  : 1;
  XBYTE  final : 1;

} WEBSOCKET_HEADERFRAME;

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XPATHS;
class XBUFFER;
class XTIMER;
class XLOG;
class DIOSTREAM_XEVENT;
class DIOSTREAMTCPIPCONFIG ;
class DIOSTREAMTLSCONFIG;
class DIOSTREAMTCPIP;
class DIOWEBSERVER_REQUEST;
class DIOWEBSERVER_CONNECTION;
class DIOWEBSERVER;


class DIOWEBSERVER_HEADER : public DIOWEBHEADER
{
  public:
                                                DIOWEBSERVER_HEADER                     ();
    virtual                                    ~DIOWEBSERVER_HEADER                     ();

    int                                         GetVersion                              ();
    int                                         GetSubVersion                           ();
    int                                         GetResult                               ();
    bool                                        GetResultString                         (XSTRING& coderesultstring);

    bool                                        CreateContentType                       (XSTRING* resource, XSTRING& contenttype);

    bool                                        Create                                  (XSTRING* resource, int version, int subversion, DIOWEBHEADER_RESULT result, int size, XSTRING* entitytag = NULL, bool isplayed = true, bool password = false);

    bool                                        AddWebSocketKey                         (XSTRING& websocketaccept, XSTRING& websocketprotocol);

  private:

    bool                                        GetDescriptionResult                    (DIOWEBHEADER_RESULT result, XSTRING& description);

    void                                        Clean                                   ();
};

class DIOWEBSOCKET_HEADER
{
  public:
                                                DIOWEBSOCKET_HEADER                     ();
    virtual                                    ~DIOWEBSOCKET_HEADER                     ();

  private:

    void                                        Clean                                   ();
};


class DIOWEBSERVER_AUTHENTICATION
{
  public:
                                                DIOWEBSERVER_AUTHENTICATION             ();
    virtual                                    ~DIOWEBSERVER_AUTHENTICATION             ();

    XSTRING*                                    GetGuest                                ();
    XSTRING*                                    GetLogin                                ();
    XSTRING*                                    GetPassword                             ();

    bool                                        Set                                     (XSTRING& login, XSTRING& password);
    bool                                        Set                                     (XSTRING& guest, XSTRING& login, XSTRING& password);

  private:

    void                                        Clean                                   ();

    XSTRING                                     guest;
    XSTRING                                     login;
    XSTRING                                     password;
};

class DIOWEBSERVER_REQUEST
{
  public:
                                                DIOWEBSERVER_REQUEST                    ();
    virtual                                    ~DIOWEBSERVER_REQUEST                    ();

    DIOWEBSERVER_TYPE                           GetType                                 ();
    void                                        SetType                                 (DIOWEBSERVER_TYPE type);

    DIOWEBHEADER_METHOD                         GetMethod                               ();
    void                                        SetMethod                               (DIOWEBHEADER_METHOD method);

    XSTRING*                                    GetResource                             ();

    XSTRING*                                    GetGuest                                ();

    XSTRING*                                    GetUserAgent                            ();

    int                                         GetSize                                 ();
    void                                        SetSize                                 (XDWORD size);

    XSTRING*                                    GetIfNoneMatch                          ();
    void                                        SetIfNoneMatch                          (XSTRING& ifnonematch);

    XSTRING*                                    GetAcceptedCharset                      ();
    XSTRING*                                    GetAcceptedControlRequestMethod         ();
    XSTRING*                                    GetAcceptedLanguage                     ();
    XSTRING*                                    GetAcceptedAllowHeaders                 ();

    void                                        ProcessAcceptedEncodingFlags            (XSTRING& string);

    XSTRING*                                    GetAccessControlAllowOrigin             ();
    XSTRING*                                    GetAccessControlAllowMethods            ();
    XSTRING*                                    GetAccessControlAllowHeaders            ();

    XSTRING*                                    GetOrigin                               ();

    XSTRING*                                    GetConnection                           ();
    XSTRING*                                    GetUpgrade                              ();

    XBUFFER*                                    GetData                                 ();

    bool                                        HaveLoginPassword                       ();
    bool                                        GetLoginPassword                        (XSTRING& login, XSTRING& password);
    void                                        SetLoginPassword                        (XSTRING& loginpassword);

    bool                                        ConvertFromWeb                          (XSTRING& string);

    bool                                        IsAcceptedEncodingFlag                  (DIOWEBSERVER_ENCODINGFLAGS flag);

    XSTRING*                                    WebSocket_GetKey                        ();
    XSTRING*                                    WebSocket_GetProtocol                   ();
    int                                         WebSocket_GetVersion                    ();
    void                                        WebSocket_SetVersion                    (int version);

  private:

    void                                        Clean                                   ();

    DIOWEBSERVER_TYPE                           type;
    DIOWEBHEADER_METHOD                         method;
    XSTRING                                     resource;
    XSTRING                                     guest;
    XSTRING                                     useragent;
    XSTRING                                     loginpassword;
    XSTRING                                     ifnonematch;

    XSTRING                                     acceptedcharset;
    XSTRING                                     acceptedcontrolrequestmethod;
    XSTRING                                     acceptedlanguage;
    XSTRING                                     acceptedallowheaders;

    XDWORD                                      acceptedencodingflags;

    XSTRING                                     accesscontrolalloworigin;
    XSTRING                                     accesscontrolallowmethods;
    XSTRING                                     accesscontrolallowheaders;

    XSTRING                                     origin;

    XSTRING                                     connection;
    XSTRING                                     upgrade;

    bool                                        iskeepalive;

    bool                                        websocket_isrequest;
    XSTRING                                     websocket_key;
    XSTRING                                     websocket_protocol;
    int                                         websocket_version;

    int                                         size;
    XBUFFER*                                    data;
};

class DIOWEBSERVER_CONNECTION
{
  public:
                                                DIOWEBSERVER_CONNECTION                 ();
    virtual                                    ~DIOWEBSERVER_CONNECTION                 ();

    bool                                        Ini                                     (DIOWEBSERVER* webserver, DIOSTREAMTCPIPCONFIG* diostreamcfg);

    DIOSTREAMTCPIP*                             GetDIOStream                            ();
    XTIMER*                                     GetTimerConnection                      ();

    bool                                        IsActive                                ();
    bool                                        Activate                                ();
    bool                                        ActivateOpen                            ();
    bool                                        IsOpening                               ();

    bool                                        Deactivate                              ();

    DIOWEBSERVER_CONNECTION_MODE                GetMode                                 ();
    void                                        SetMode                                 (DIOWEBSERVER_CONNECTION_MODE mode);

    bool                                        Receiver                                (XBYTE* buffer, XDWORD& size, int timeout = DIOWEBSERVER_DEFAULTCONNECTIONTIMEOUT);

    bool                                        Send                                    (XBYTE* buffer, XDWORD& size, int timeout = DIOWEBSERVER_DEFAULTCONNECTIONTIMEOUT);
    bool                                        Send                                    (XBUFFER& xbuffer, int timeout = DIOWEBSERVER_DEFAULTCONNECTIONTIMEOUT);

    bool                                        IsRequestInProgress                     ();

    DIOWEBSERVER*                               GetServer                               ();
    DIOWEBCLIENT_HEADER*                        GetHeader                               ();
    DIOWEBSERVER_REQUEST*                       GetRequest                              ();

    DIOWEBSERVER_ENCODINGFLAGS                  GetActualEncodingFlag                   ();
    bool                                        SetActualEncodingFlag                   (DIOWEBSERVER_ENCODINGFLAGS actualencodingflag);

    bool                                        WebSocket_Write                         (XCHAR* string, int timeout = DIOWEBSERVER_DEFAULTCONNECTIONTIMEOUT);
    bool                                        WebSocket_Write                         (XSTRING& string, int timeout = DIOWEBSERVER_DEFAULTCONNECTIONTIMEOUT);
    bool                                        WebSocket_Write                         (XBYTE* buffer, XDWORD size, int timeout = DIOWEBSERVER_DEFAULTCONNECTIONTIMEOUT);
    bool                                        WebSocket_Write                         (XBUFFER& buffer, int timeout = DIOWEBSERVER_DEFAULTCONNECTIONTIMEOUT);
    bool                                        WebSocket_WritePingPong                 (bool isping, XCHAR* string, int timeout = DIOWEBSERVER_DEFAULTCONNECTIONTIMEOUT);
    bool                                        WebSocket_WritePingPong                 (bool isping, XSTRING& string, int timeout = DIOWEBSERVER_DEFAULTCONNECTIONTIMEOUT);
    bool                                        WebSocket_WritePingPong                 (bool isping, XBUFFER& data, int timeout = DIOWEBSERVER_DEFAULTCONNECTIONTIMEOUT);

    XTIMER*                                     GetTimerDisconnection                   ();

    bool                                        End                                     ();
    bool                                        TryAcquireReference                   ();
    void                                        ReleaseReference                      ();
    int                                         GetReferenceCount                     () const;

  protected:

    DIOWEBSERVER*                               webserver;
    DIOSTREAMTCPIPCONFIG *                      diostreamcfg;
    DIOSTREAMTCPIP*                             diostream;
    DIOWEBSERVER_TIMEOUTS                       operativetimeouts;
    DIOWEBSERVER_LIMITS                         operativelimits;

    bool                                        isactive;
    bool                                        isopening;
    DIOWEBSERVER_CONNECTION_MODE                mode;
    XTIMER*                                     xtimerconnection;
    XTIMER*                                     xtimerdisconnection;
    XDWORD                                      nresourcesprocessed;
    DIOWEBCLIENT_HEADER                         header;
    DIOWEBSERVER_REQUEST                        request;
    bool                                        isrequestinprogress;
    XTHREADCOLLECTED*                           threadconnection;
    DIOWEBSERVER_ENCODINGFLAGS                  actualencodingflag;
    XSTRING                                     origin;

    XBUFFER                                     websocket_xbufferframepart;
    std::atomic<int>                            websocketreferences;

  private:

    bool                                        ReadRequest                             ();
    bool                                        SendRequest                             ();

    bool                                        WebSocket_CreateAcceptKey               (XSTRING& key, XSTRING& result);
    bool                                        WebSocket_CreateHeader                  (DIOWEBSERVER_WEBSOCKET_OPCODE opcode, XDWORD size, XBUFFER& data);
    bool                                        Websocket_ResolveConnection             (DIOWEBSERVER_REQUEST* request);
    bool                                        WebSocket_ReadRequest                   (DIOWEBSERVER_WEBSOCKET_OPCODE& opcode, XBUFFER& data);
    bool                                        WebSocket_SendEvent_Connected           ();
    bool                                        WebSocket_SendEvent_ReadData            (XBUFFER& data);
    bool                                        WebSocket_SendEvent_Pong                (XBUFFER& data);
    bool                                        WebSocket_SendEvent_Disconnected        ();

    static void                                 ThreadRunFunction                       (void* param);

    void                                        Clean                                   ();
};

class DIOWEBSERVER :  public XOBSERVER, public XSUBJECT
{
  public:
                                                DIOWEBSERVER                            ();
    virtual                                    ~DIOWEBSERVER                            ();

    bool                                        Ini                                     (int port = DIOWEBSERVER_DEFAULTPORT, bool doinitialconnectitivitytest = true, int timeoutserverpage = DIOWEBSERVER_DEFAULTTIMEOUTSERVERPAGE, XSTRING* addrlocal = NULL); 
    bool                                        Ini                                     (DIOSTREAMTLSCONFIG* tlsconfig, int port, int timeoutserverpage = DIOWEBSERVER_DEFAULTTIMEOUTSERVERPAGE, XSTRING* addrlocal = NULL);

    int                                         GetPort                                 ();
    int                                         GetTimeoutServerPage                    ();
    XDWORD                                      GetMaxPageConnections                   ();
    bool                                        SetMaxPageConnections                   (XDWORD maxpageconnections);
    DIOWEBSERVER_TIMEOUTS*                      GetTimeouts                             ();
    bool                                        SetTimeouts                             (DIOWEBSERVER_TIMEOUTS& timeouts);
    DIOWEBSERVER_LIMITS*                        GetLimits                               ();
    bool                                        SetLimits                               (DIOWEBSERVER_LIMITS& limits);

    DIOSTREAMTCPIPCONFIG *                      GetDIOStreamCFG                         ();

    bool                                        IsSendingAnyPage                        ();
    bool                                        UnSubscribeAllPages                     ();

    bool                                        AddAuthentication                       (XSTRING& guest, XSTRING& login, XSTRING& password);
    DIOWEBSERVER_AUTHENTICATION*                GetAuthentication                       (XSTRING& guest);
    bool                                        HaveAuthentications                     ();
    bool                                        CheckAuthentication                     (XSTRING& guest, XSTRING& login, XSTRING& password);
    bool                                        DelAuthentication                       (XSTRING& guest);
    bool                                        DelAllAuthentication                    ();

    bool                                        IsPlayablePage                          (XCHAR* extension);
    bool                                        IsPlayablePage                          (XSTRING& extension);

    bool                                        AddPlayablePageExtension                (XCHAR* extension);
    bool                                        AddPlayablePageExtension                (XSTRING& extension);

    bool                                        DeleteAllPlayablePageExtensions         ();

    void                                        AddIncludedEncodingFlag                 (DIOWEBSERVER_ENCODINGFLAGS flag);
    bool                                        IsIncludedEncodingFlag                  (DIOWEBSERVER_ENCODINGFLAGS flag);

    XMUTEX*                                     GetMutextConnections                    ();
    XVECTOR<DIOWEBSERVER_CONNECTION*>*          GetConnections                          ();

    DIOWEBSERVER_CONNECTION*                    Websocket_GetConnection                 ();
    DIOWEBSERVER_CONNECTION*                    Websocket_GetConnection                 (XCHAR* protocol);
    DIOWEBSERVER_CONNECTION*                    Websocket_GetConnection                 (XCHAR* protocol, int version);
    DIOWEBSERVER_CONNECTION*                    Websocket_GetConnection                 (XCHAR* protocol, int version, XCHAR* resource);
    DIOWEBSERVER_CONNECTION*                    Websocket_GetNextConnection             ();

    bool                                        Websocket_Write                         (XCHAR* string, int timeout = DIOWEBSERVER_DEFAULTCONNECTIONTIMEOUT, XCHAR* protocol = NULL, int version = 0, XCHAR* resource = NULL);
    bool                                        Websocket_Write                         (XSTRING& string , int timeout = DIOWEBSERVER_DEFAULTCONNECTIONTIMEOUT, XCHAR* protocol = NULL, int version = 0, XCHAR* resource = NULL);
    bool                                        Websocket_Write                         (XBUFFER& data, int timeout = DIOWEBSERVER_DEFAULTCONNECTIONTIMEOUT, XCHAR* protocol = NULL, int version = 0, XCHAR* resource = NULL);
    bool                                        Websocket_Ping                          (XCHAR* string, int timeout = DIOWEBSERVER_DEFAULTCONNECTIONTIMEOUT, XCHAR* protocol = NULL, int version = 0, XCHAR* resource = NULL);
    bool                                        Websocket_Ping                          (XSTRING& string , int timeout = DIOWEBSERVER_DEFAULTCONNECTIONTIMEOUT, XCHAR* protocol = NULL, int version = 0, XCHAR* resource = NULL);

    DIOWEBSERVER_PLUGINSMANAGER*                GetPluginsManager                       ();

    bool                                        IsExit                                  ();

    bool                                        End                                     ();

  protected:

    DIOSTREAMTCPIPCONFIG*                       diostreamcfg;
    int                                         port;
    int                                         timeoutserverpage;
    DIOWEBSERVER_TIMEOUTS                       timeouts;
    DIOWEBSERVER_LIMITS                         limits;
    XDWORD                                      maxpageconnections;
    XSTRING                                     addrlocal;
    XMUTEX*                                     xmutexconnections;
    XVECTOR<DIOWEBSERVER_CONNECTION*>           connections;
    bool                                        isactive;
    bool                                        doexit;
    XVECTOR<DIOWEBSERVER_AUTHENTICATION*>       authentications;
    XVECTOR<XSTRING*>                           playablepageextensions;
    XTIMER*                                     xtimercontrol;
    XTHREADCOLLECTED*                           threadserver;
    XDWORD                                      includedencodingflags;
    DIOWEBSERVER_PLUGINSMANAGER                 pluginsmanager;

    int                                         websocket_search_indexconnection;
    XSTRING                                     websocket_search_protocol;
    int                                         websocket_search_version;
    XSTRING                                     websocket_search_resource;


  private:

    bool                                        Ini_Internal                             (DIOSTREAMTCPIPCONFIG* newcfg, int port, bool doinitialconnectitivitytest, int timeoutserverpage, XSTRING* addrlocal);

    bool                                        WaitToSentAllPages                       ();

    XDWORD                                      Connections_GetNWaiting                  ();

    int                                         Connections_GetNConnectionsSendingPage   ();
    bool                                        Connections_CreateNew                    ();
    bool                                        Connections_DeleteUsed                   ();
    bool                                        Connections_DeleteWaiting                ();
    bool                                        Connections_DeleteAll                    ();

    static void                                 ThreadRunFunction                        (void* param);

    void                                        HandleEvent_DIOStream                    (DIOSTREAM_XEVENT* event);
    void                                        HandleEvent                              (XEVENT* xevent);

    void                                        Clean                                    ();
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/
