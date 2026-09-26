/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOWebServer_XEvent.h
* 
* @class      DIOWEBSERVER_XEVENT
* @brief      Data Input/Output Web Server eXtended Event class
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

#include "XEvent.h"
#include "XString.h"
#include "XBuffer.h"

#include "DIOWebHeader.h"
#include "DIOWebServer_Plugin.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum DIOWEBSERVER_XEVENT_TYPE
{
  DIOWEBSERVER_XEVENT_TYPE_UNKNOWN                 = XEVENT_TYPE_WEBSERVER ,
  DIOWEBSERVER_XEVENT_TYPE_REQUEST                                        ,
  DIOWEBSERVER_XEVENT_TYPE_DOAUTHENTICATE                                 ,
  DIOWEBSERVER_XEVENT_TYPE_CHECKAUTHENTICATE                              ,
  DIOWEBSERVER_XEVENT_TYPE_REQUEST_ENDPOINT                               ,
  DIOWEBSERVER_XEVENT_TYPE_WEBSOCKET_CONNECTED                            ,
  DIOWEBSERVER_XEVENT_TYPE_WEBSOCKET_READDATA                             ,
  DIOWEBSERVER_XEVENT_TYPE_WEBSOCKET_PONG                                 ,
  DIOWEBSERVER_XEVENT_TYPE_WEBSOCKET_DISCONNECTED                         ,
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XSUBJECT;
class DIOWEBSERVER_REQUEST;
class DIOWEBSERVER_CONNECTION;
class DIOWEBSERVER;
class DIOWEBSERVER_QUERYSTRINGS;

class DIOWEBSERVER_XEVENT : public XEVENT
{
  public:
                                        DIOWEBSERVER_XEVENT               (XSUBJECT* subject, XDWORD type = DIOWEBSERVER_XEVENT_TYPE_UNKNOWN, XDWORD family = XEVENT_TYPE_WEBSERVER);
    virtual                            ~DIOWEBSERVER_XEVENT               ();

    DIOWEBSERVER*                       GetServer                         ();
    void                                SetServer                         (DIOWEBSERVER* server);

    DIOWEBSERVER_CONNECTION*            GetConnection                     ();
    void                                SetConnection                     (DIOWEBSERVER_CONNECTION* connection);

    DIOWEBSERVER_REQUEST*               GetRequest                        ();
    void                                SetRequest                        (DIOWEBSERVER_REQUEST* request);

    int                                 GetTimeOut                        ();
    void                                SetTimeOut                        (int timeout);

    DIOWEBSERVER_QUERYSTRINGS*          GetQueryString                    ();
    void                                SetQueryString                    (DIOWEBSERVER_QUERYSTRINGS* querystring);

    XBUFFER*                            GetReadDataWebSocket              ();

    XSTRING*                            GetOutputString                   ();
    XBUFFER*                            GetOutputBuffer                   ();

    bool                                IsRequestAuthenticate             ();
    void                                SetIsRequestAuthenticate          (bool isauthenticate);

    DIOWEBHEADER_RESULT                 GetResult                         ();
    void                                SetResult                         (DIOWEBHEADER_RESULT result);

    bool                                GetStatus                         ();
    void                                SetStatus                         (bool status);

  private:

    void                                Clean                             ();

    DIOWEBSERVER*                       server;
    DIOWEBSERVER_CONNECTION*            connection;
    DIOWEBSERVER_REQUEST*               request;
    int                                 timeout;
    DIOWEBSERVER_QUERYSTRINGS*          querystring;
    XBUFFER                             readdatawebsocket;
    XSTRING                             outputstring;
    XBUFFER                             outputbuffer;
    bool                                isrequestauthenticate;
    DIOWEBHEADER_RESULT                 result;
    bool                                status;
};





/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





