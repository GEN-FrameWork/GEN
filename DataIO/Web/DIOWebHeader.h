/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOWebHeader.h
* 
* @class      DIOWEBHEADER
* @brief      Data Input/Output Web Server Header class
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

#include "XVector.h"
#include "XString.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum DIOWEBHEADER_RESULT
{
  DIOWEBHEADER_RESULT_CONTINUE                            = 100 ,
  DIOWEBHEADER_RESULT_SWITCHINGPROTOCOLS                        ,

  DIOWEBHEADER_RESULT_OK                                  = 200 ,
  DIOWEBHEADER_RESULT_CREATED                                   ,
  DIOWEBHEADER_RESULT_ACCEPTED                                  ,
  DIOWEBHEADER_RESULT_NONAUTHORITATIVEINFORMATION               ,
  DIOWEBHEADER_RESULT_NOCONTENT                                 ,
  DIOWEBHEADER_RESULT_RESETCONTENT                              ,
  DIOWEBHEADER_RESULT_PARTIALCONTENT                            ,

  DIOWEBHEADER_RESULT_MULTIPLECHOICES                     = 300 ,
  DIOWEBHEADER_RESULT_MOVEDPERMANENTLY                          ,
  DIOWEBHEADER_RESULT_FOUND                                     ,
  DIOWEBHEADER_RESULT_SEEOTHER                                  ,
  DIOWEBHEADER_RESULT_NOTMODIFIED                               ,
  DIOWEBHEADER_RESULT_USEPROXY                                  ,
  DIOWEBHEADER_RESULT_UNUSED                                    ,
  DIOWEBHEADER_RESULT_TEMPORARYREDIRECT                         ,

  DIOWEBHEADER_RESULT_BADREQUEST                          = 400 ,
  DIOWEBHEADER_RESULT_UNAUTHORIZED                              ,
  DIOWEBHEADER_RESULT_PAYMENTREQUIRED                           ,
  DIOWEBHEADER_RESULT_FORBIDDEN                                 ,
  DIOWEBHEADER_RESULT_NOTFOUND                                  ,
  DIOWEBHEADER_RESULT_METHODNOTALLOWED                          ,
  DIOWEBHEADER_RESULT_NOTACCEPTABLE                             ,
  DIOWEBHEADER_RESULT_PROXYAUTHENTICATIONREQUIRED               ,
  DIOWEBHEADER_RESULT_REQUESTTIMEOUT                            ,
  DIOWEBHEADER_RESULT_CONFLICT                                  ,
  DIOWEBHEADER_RESULT_GONE                                      ,
  DIOWEBHEADER_RESULT_LENGTHREQUIRED                            ,
  DIOWEBHEADER_RESULT_PRECONDITIONFAILED                        ,
  DIOWEBHEADER_RESULT_REQUESTENTITYTOOLARGE                     ,
  DIOWEBHEADER_RESULT_REQUESTURITOOLONG                         ,
  DIOWEBHEADER_RESULT_UNSUPPORTEDMEDIATYPE                      ,
  DIOWEBHEADER_RESULT_REQUESTEDRANGENOTSATISFIABLE              ,
  DIOWEBHEADER_RESULT_EXPECTATIONFAILED                         ,

  DIOWEBHEADER_RESULT_INTERNALSERVERERROR                 = 500 ,
  DIOWEBHEADER_RESULT_NOTIMPLEMENTED                            ,
  DIOWEBHEADER_RESULT_BADGATEWAY                                ,
  DIOWEBHEADER_RESULT_SERVICEUNAVAILABLE                        ,
  DIOWEBHEADER_RESULT_GATEWAYTIMEOUT                            ,
  DIOWEBHEADER_RESULT_HTTPVERSIONNOTSUPPORTED                   ,
};


enum DIOWEBHEADER_METHOD
{
  DIOWEBHEADER_METHOD_UNKNOWN                               = 0 ,
  DIOWEBHEADER_METHOD_GET                                       ,
  DIOWEBHEADER_METHOD_HEAD                                      ,
  DIOWEBHEADER_METHOD_POST                                      ,
  DIOWEBHEADER_METHOD_PUT                                       ,   // not implement
  DIOWEBHEADER_METHOD_DELETE                                    ,   // not implement
  DIOWEBHEADER_METHOD_CONNECT                                   ,   // not implement
  DIOWEBHEADER_METHOD_OPTIONS                                   ,
  DIOWEBHEADER_METHOD_TRACE                                     ,   // not implement
};

#define DIOWEBHEADER_CONTENT_LENGTH                       _L("Content-Length")
#define DIOWEBHEADER_TRANSFER_ENCODING                    _L("Transfer-Encoding")
#define DIOWEBHEADER_CONTENT_ENCODING                     _L("Content-Encoding")
#define DIOWEBHEADER_ACCEPT_ENCODING                      _L("Accept-Encoding")
#define DIOWEBHEADER_ETAG                                 _L("ETag")
#define DIOWEBHEADER_WWW_AUTHENTICATE                     _L("WWW-Authenticate")
#define DIOWEBHEADER_LOCATION                             _L("Location")

#define DIOWEBHEADER_MAXLINE                              1024
#define DIOWEBHEADER_MAXSIZE                              (64*1024)

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XTIMER;
class DIOSTREAMTCPIP;

class DIOWEBHEADER
{
  public:
                                DIOWEBHEADER                      ();
    virtual                    ~DIOWEBHEADER                      ();

    bool                        Read                              (DIOSTREAMTCPIP* diostream, int timeout, XDWORD maximumsize = DIOWEBHEADER_MAXSIZE);
    bool                        Write                             (DIOSTREAMTCPIP* diostream, int timeout);

    bool                        AddLine                           (XCHAR* line);
    bool                        AddLine                           (XSTRING& line);

    XVECTOR<XSTRING*>*          GetLines                          ();
    bool                        GetLines                          (XSTRING& result);

    XCHAR*                      GetFieldValue                     (XCHAR* field);
    XCHAR*                      GetFieldValue                     (XSTRING& field);

    bool                        DeleteAllLines                    ();

  protected:

    XVECTOR<XSTRING*>           lines;

    bool                        GetFormat                         (bool isserver, int type, void* data);

  private:

    void                        Clean                             ();
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/


