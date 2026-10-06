/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOWebServer.cpp
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOWebServer.h"

#include "XFactory.h"
#include "XSleep.h"
#include "XTimer.h"
#include "XBuffer.h"
#include "XPath.h"
#include "XLog.h"
#include "XFileTXT.h"
#include "XSystem.h"

#include "HashSHA1.h"

#include "CompressManager.h"

#include "DIOFactory.h"
#include "DIOURL.h"
#include "DIOStream_XEvent.h"
#include "DIOStreamTCPIPConfig.h"
#include "DIOStreamTCPIP.h"
#include "DIOStreamTLSConfig.h"
#include "DIOWebServer_Plugin.h"
#include "DIOWebServer_XEvent.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBSERVER_HEADER::DIOWEBSERVER_HEADER()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBSERVER_HEADER::DIOWEBSERVER_HEADER()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBSERVER_HEADER::~DIOWEBSERVER_HEADER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBSERVER_HEADER::~DIOWEBSERVER_HEADER()
{
  DeleteAllLines();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOWEBSERVER_HEADER::GetVersion()
* @brief      Get version
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOWEBSERVER_HEADER::GetVersion()
{
  int data = 0;

  if(!GetFormat(true, 0, &data)) return false;

  return data;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOWEBSERVER_HEADER::GetSubVersion()
* @brief      Get sub version
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOWEBSERVER_HEADER::GetSubVersion()
{
  int data = 0;

  if(!GetFormat(true, 1, &data)) return false;

  return data;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOWEBSERVER_HEADER::GetResult()
* @brief      Get result
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOWEBSERVER_HEADER::GetResult()
{
  int data = 0;

  if(!GetFormat(true, 2, &data)) return false;

  return data;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_HEADER::GetResultString(XSTRING& resultstring)
* @brief      Get result string
* @ingroup    DATAIO
* 
* @param[in]  resultstring : Output resultstring.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER_HEADER::GetResultString(XSTRING& resultstring)
{
  if(!GetFormat(true, 3, &resultstring)) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_HEADER::CreateContentType(XSTRING* resource, XSTRING& contenttype)
* @brief      Create content type
* @ingroup    DATAIO
* 
* @param[in]  resource : Resource pointer to use.
* @param[in]  contenttype : Contenttype value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER_HEADER::CreateContentType(XSTRING* resource, XSTRING& contenttype)
{
  contenttype.Empty();

       if(resource->Find(_L(".htm")  , true) != XSTRING_NOTFOUND)  contenttype = _L("text/html");
  else if(resource->Find(_L(".xml")  , true) != XSTRING_NOTFOUND)  contenttype = _L("text/xml");
  else if(resource->Find(_L(".exe")  , true) != XSTRING_NOTFOUND)  contenttype = _L("application/octet-stream");
  else if(resource->Find(_L(".gif")  , true) != XSTRING_NOTFOUND)  contenttype = _L("image/gif");
  else if(resource->Find(_L(".jpg")  , true) != XSTRING_NOTFOUND)  contenttype = _L("image/jpeg");
  else if(resource->Find(_L(".jpeg") , true) != XSTRING_NOTFOUND)  contenttype = _L("image/jpeg");
  else if(resource->Find(_L(".png")  , true) != XSTRING_NOTFOUND)  contenttype = _L("image/png");
  else if(resource->Find(_L(".wav")  , true) != XSTRING_NOTFOUND)  contenttype = _L("audio/x-wav");
  else if(resource->Find(_L(".mp3")  , true) != XSTRING_NOTFOUND)  contenttype = _L("audio/mp3");
  else if(resource->Find(_L(".gz")   , true) != XSTRING_NOTFOUND)  contenttype = _L("application/x-gzip");
  else if(resource->Find(_L(".mpg")  , true) != XSTRING_NOTFOUND)  contenttype = _L("video/mpeg");
  else if(resource->Find(_L(".pdf")  , true) != XSTRING_NOTFOUND)  contenttype = _L("application/pdf");
  else if(resource->Find(_L(".avi")  , true) != XSTRING_NOTFOUND)  contenttype = _L("video/x-msvideof");
  else if(resource->Find(_L(".ogv")  , true) != XSTRING_NOTFOUND)  contenttype = _L("video/ogg");
  else if(resource->Find(_L(".ogg")  , true) != XSTRING_NOTFOUND)  contenttype = _L("video/ogg");
  else if(resource->Find(_L(".css")  , true) != XSTRING_NOTFOUND)  contenttype = _L("text/css");
  else if(resource->Find(_L(".js")   , true) != XSTRING_NOTFOUND)  contenttype = _L("application/javascript");
  else if(resource->Find(_L(".json") , true) != XSTRING_NOTFOUND)  contenttype = _L("application/json");
  else if(resource->Find(_L(".zip")  , true) != XSTRING_NOTFOUND)  contenttype = _L("application/zip");
  else if(resource->Find(_L(".otf")  , true) != XSTRING_NOTFOUND)  contenttype = _L("application/x-font-otf");

  if(!resource->Compare(_L("/"), true))  contenttype = _L("text/html");

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_HEADER::Create(XSTRING* resource, int version, int subversion, DIOWEBHEADER_RESULT result, int size, XSTRING* entitytag, bool isplayable, bool password)
* @brief      Create resource
* @ingroup    DATAIO
* 
* @param[in]  resource : Resource pointer to use.
* @param[in]  version : Version value.
* @param[in]  subversion : Subversion value.
* @param[in]  result : Output result.
* @param[in]  size : Size value.
* @param[in]  entitytag : Entitytag pointer to use.
* @param[in]  isplayable : Isplayable value.
* @param[in]  password : Password text.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER_HEADER::Create(XSTRING* resource, int version, int subversion, DIOWEBHEADER_RESULT result, int size, XSTRING* entitytag, bool isplayable, bool password)
{
  /*
      Accept - Ranges:bytes
      Connection : Keep - Alive
      Content - Length : 6
      Content - Type : text / html
      Date : Mon, 06 Nov 2017 11 : 21 : 24 GMT
      ETag : "6-55d4e3b3b9f20"
      Keep - Alive : timeout = 5, max = 93
      Last - Modified : Mon, 06 Nov 2017 10 : 51 : 06 GMT
      Server : Apache / 2.4.23 (Win64)PHP / 5.6.25
  */

  XSTRING line;
  line.Format(_L("HTTP/%d.%d %d "), version, subversion, result);

  if(password)
    {
      XSTRING description;
      XSTRING authenticateline;

      GetDescriptionResult(DIOWEBHEADER_RESULT_UNAUTHORIZED, description);

      line += description;
      AddLine(line);

      // Without the realm the browsers do not show the login/password dialog.
      authenticateline.Format(_L("%s: Basic realm=\"%s\", charset=\"UTF-8\""), DIOWEBHEADER_WWW_AUTHENTICATE, DIOWEBSERVER_AUTHENTICATION_DEFAULTREALM);
      AddLine(authenticateline);
    }
   else
    {
      XSTRING description;

      GetDescriptionResult(result, description);

      line += description;
      AddLine(line);
    }


  AddLine(_L("Accept-Ranges: bytes"));

  line.Empty();
  CreateContentType(resource, line);

  if(!line.IsEmpty())
    {
      line.Insert(_L("Content-Type: "), 0);
      AddLine(line.Get());
    }

  XSTRING date;
  XDATETIME* datetime=GEN_XFACTORY.CreateDateTime();
  if(datetime)
    {
      XSTRING day;
      XSTRING month;
      XSTRING hour;

      datetime->Read();
      datetime->GetDateTimeToString(XDATETIME_FORMAT_ADDTIME | XDATETIME_FORMAT_TIMEWITHSECONDS, hour);
      datetime->GetMonthString(month);

      switch((int)datetime->GetDayOfWeek())
        {
          case  1:  day.Set(_L("Mon")); break;
          case  2:  day.Set(_L("Tue")); break;
          case  3:  day.Set(_L("Wed")); break;
          case  4:  day.Set(_L("Thu")); break;
          case  5:  day.Set(_L("Fri")); break;
          case  6:  day.Set(_L("Sat")); break;
          case  0:
          case  7:  day.Set(_L("Sun")); break;
          default:  break;
        }

      switch(datetime->GetMonth())
        {
          case  1:  month.Set(_L("Jan")); break;
          case  2:  month.Set(_L("Feb")); break;
          case  3:  month.Set(_L("Mar")); break;
          case  4:  month.Set(_L("Apr")); break;
          case  5:  month.Set(_L("May")); break;
          case  6:  month.Set(_L("Jun")); break;
          case  7:  month.Set(_L("Jul")); break;
          case  8:  month.Set(_L("Aug")); break;
          case  9:  month.Set(_L("Sep")); break;
          case 10:  month.Set(_L("Oct")); break;
          case 11:  month.Set(_L("Nov")); break;
          case 12:  month.Set(_L("Dec")); break;
          default:  break;
        }

      //Date: Wed, 21 Oct 2015 07:28:00 GMT
      date.Format(_L("%s, %d %s %d %s GMT"), day.Get(), datetime->GetDay(), month.Get(), datetime->GetYear(), hour.Get());
      line.Format(_L("Date:%s"), date.Get());  AddLine(line);
      delete(datetime);
   }

  if(entitytag)
    {
      if(!entitytag->IsEmpty())
        {
          line.Format(_L("ETag: \"%s\""), entitytag->Get());
          AddLine(line.Get());
        }
    }

  //---------------------------------------------------------------

  AddLine(_L("Connection: Keep-Alive"));
  line.Format(_L("Keep-Alive: timeout=%d,max=%d"), DIOWEBSERVER_KEEPALIVE, DIOWEBSERVER_KEEPALIVE_MAXRESOURCES);
  AddLine(line);

  //---------------------------------------------------------------

  line.Format(_L("Last-Modified: %s"), date.Get());                  AddLine(line);
  line.Format(_L("Server: %s "), DIOWEBSERVER_IDSERVER);   AddLine(line);

  if(size != -1)
    {
      line.Format(_L("Content-Length: %d"), size);
      AddLine(line);

    }

  /*
  AddLine(_L("Access-Control-Allow-Headers: DNT,X-CustomHeader,Keep-Alive,User-Agent,X-Requested-With,If-Modified-Since,Cache-Control,Content-Type,Content-Encoding,Vary"));
  AddLine(_L("Vary : Accept-Encoding"));

  connection->SetActualEncodingFlag(DIOWEBSERVER_ENCODINGFLAGS_NONE);

  bool validsize = false;

  if(size > 100)  validsize = true;
  if(size == -1)  validsize = true;

  if(request->IsAcceptedEncodingFlag(DIOWEBSERVER_ENCODINGFLAGS_GZIP))
    {
      if(webserver->IsIncludedEncodingFlag(DIOWEBSERVER_ENCODINGFLAGS_GZIP))
        {
          if((validsize) && (isplayable))
            {
              connection->SetActualEncodingFlag(DIOWEBSERVER_ENCODINGFLAGS_GZIP);
              AddLine(_L("Content-Encoding: gzip"));

            }
        }
    }
  */

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_HEADER::AddWebSocketKey(XSTRING& websocketaccept, XSTRING& websocketprotocol)
* @brief      Add web socket key
* @ingroup    DATAIO
* 
* @param[in]  websocketaccept : Websocketaccept value.
* @param[in]  websocketprotocol : Websocketprotocol value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER_HEADER::AddWebSocketKey(XSTRING& websocketaccept, XSTRING& websocketprotocol)
{
  XSTRING line;

  AddLine(_L("Access-Control-Allow-Headers: x-websocket-version"));
  AddLine(_L("Connection: Upgrade"));
  AddLine(_L("Upgrade: websocket"));

  line.Format(_L("Sec-WebSocket-Accept: %s"), websocketaccept.Get());
  AddLine(line.Get());

  if(websocketprotocol.GetSize())
    {
      line.Format(_L("Sec-WebSocket-Protocol: %s"), websocketprotocol.Get());
      AddLine(line.Get());
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_HEADER::GetDescriptionResult(DIOWEBHEADER_RESULT result, XSTRING& description)
* @brief      Get description result
* @ingroup    DATAIO
* 
* @param[in]  result : Output result.
* @param[in]  description : Description value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER_HEADER::GetDescriptionResult(DIOWEBHEADER_RESULT result, XSTRING& description)
{
  typedef struct
  {
    DIOWEBHEADER_RESULT  result;
    XCHAR*               description;

  } DESCRIPTION;

  DESCRIPTION  descriptions[] = { { DIOWEBHEADER_RESULT_CONTINUE                     ,  _L("Continue")                         } ,
                                  { DIOWEBHEADER_RESULT_SWITCHINGPROTOCOLS           ,  _L("Switching Protocols")              } ,

                                  { DIOWEBHEADER_RESULT_OK                           ,  _L("OK")                               } ,
                                  { DIOWEBHEADER_RESULT_CREATED                      ,  _L("Created")                          } ,
                                  { DIOWEBHEADER_RESULT_ACCEPTED                     ,  _L("Accepted")                         } ,
                                  { DIOWEBHEADER_RESULT_NONAUTHORITATIVEINFORMATION  ,  _L("Non-Authoritative Information")    } ,
                                  { DIOWEBHEADER_RESULT_NOCONTENT                    ,  _L("No Content")                       } ,
                                  { DIOWEBHEADER_RESULT_RESETCONTENT                 ,  _L("Reset Content")                    } ,
                                  { DIOWEBHEADER_RESULT_PARTIALCONTENT               ,  _L("Partial Content")                  } ,

                                  { DIOWEBHEADER_RESULT_MULTIPLECHOICES              ,  _L("Multiple Choices")                 } ,
                                  { DIOWEBHEADER_RESULT_MOVEDPERMANENTLY             ,  _L("Moved Permanently")                } ,
                                  { DIOWEBHEADER_RESULT_FOUND                        ,  _L("Found")                            } ,
                                  { DIOWEBHEADER_RESULT_SEEOTHER                     ,  _L("See Other")                        } ,
                                  { DIOWEBHEADER_RESULT_NOTMODIFIED                  ,  _L("Not Modified")                     } ,
                                  { DIOWEBHEADER_RESULT_USEPROXY                     ,  _L("Use Proxy")                        } ,
                                  { DIOWEBHEADER_RESULT_UNUSED                       ,  _L("(Unused)")                         } ,
                                  { DIOWEBHEADER_RESULT_TEMPORARYREDIRECT            ,  _L("Temporary Redirect")               } ,

                                  { DIOWEBHEADER_RESULT_BADREQUEST                   ,  _L("Bad Request")                      } ,
                                  { DIOWEBHEADER_RESULT_UNAUTHORIZED                 ,  _L("Unauthorized")                     } ,
                                  { DIOWEBHEADER_RESULT_PAYMENTREQUIRED              ,  _L("Payment Required")                 } ,
                                  { DIOWEBHEADER_RESULT_FORBIDDEN                    ,  _L("Forbidden")                        } ,
                                  { DIOWEBHEADER_RESULT_NOTFOUND                     ,  _L("Not Found")                        } ,
                                  { DIOWEBHEADER_RESULT_METHODNOTALLOWED             ,  _L("Method Not Allowed")               } ,
                                  { DIOWEBHEADER_RESULT_NOTACCEPTABLE                ,  _L("Not Acceptable")                   } ,
                                  { DIOWEBHEADER_RESULT_PROXYAUTHENTICATIONREQUIRED  ,  _L("Proxy Authentication Required")    } ,
                                  { DIOWEBHEADER_RESULT_REQUESTTIMEOUT               ,  _L("Request Timeout")                  } ,
                                  { DIOWEBHEADER_RESULT_CONFLICT                     ,  _L("Conflict")                         } ,
                                  { DIOWEBHEADER_RESULT_GONE                         ,  _L("Gone")                             } ,
                                  { DIOWEBHEADER_RESULT_LENGTHREQUIRED               ,  _L("Length Required")                  } ,
                                  { DIOWEBHEADER_RESULT_PRECONDITIONFAILED           ,  _L("Precondition Failed")              } ,
                                  { DIOWEBHEADER_RESULT_REQUESTENTITYTOOLARGE        ,  _L("Request Entity Too Large")         } ,
                                  { DIOWEBHEADER_RESULT_REQUESTURITOOLONG            ,  _L("Request-URI Too Long")             } ,
                                  { DIOWEBHEADER_RESULT_UNSUPPORTEDMEDIATYPE         ,  _L("Unsupported Media Type")           } ,
                                  { DIOWEBHEADER_RESULT_REQUESTEDRANGENOTSATISFIABLE ,  _L("Requested Range Not Satisfiable")  } ,
                                  { DIOWEBHEADER_RESULT_EXPECTATIONFAILED            ,  _L("Expectation Failed")               } ,

                                  { DIOWEBHEADER_RESULT_INTERNALSERVERERROR          ,  _L("Internal Server Error")            } ,
                                  { DIOWEBHEADER_RESULT_NOTIMPLEMENTED               ,  _L("Not Implemented")                  } ,
                                  { DIOWEBHEADER_RESULT_BADGATEWAY                   ,  _L("Bad Gateway")                      } ,
                                  { DIOWEBHEADER_RESULT_SERVICEUNAVAILABLE           ,  _L("Service Unavailable")              } ,
                                  { DIOWEBHEADER_RESULT_GATEWAYTIMEOUT               ,  _L("Gateway Timeout")                  } ,
                                  { DIOWEBHEADER_RESULT_HTTPVERSIONNOTSUPPORTED      ,  _L("HTTP Version Not Supported")       }
                                };

  description.Empty();

  for(int c=0;c<sizeof(descriptions)/sizeof(DESCRIPTION);c++)
    {
      if(result == descriptions[c].result)
        {
          description = descriptions[c].description;

          return true;
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBSERVER_HEADER::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBSERVER_HEADER::Clean()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBSOCKET_HEADER::DIOWEBSOCKET_HEADER()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBSOCKET_HEADER::DIOWEBSOCKET_HEADER()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBSOCKET_HEADER::~DIOWEBSOCKET_HEADER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBSOCKET_HEADER::~DIOWEBSOCKET_HEADER()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBSOCKET_HEADER::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBSOCKET_HEADER::Clean()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBSERVER_AUTHENTICATION::DIOWEBSERVER_AUTHENTICATION()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBSERVER_AUTHENTICATION::DIOWEBSERVER_AUTHENTICATION()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBSERVER_AUTHENTICATION::~DIOWEBSERVER_AUTHENTICATION()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBSERVER_AUTHENTICATION::~DIOWEBSERVER_AUTHENTICATION()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOWEBSERVER_AUTHENTICATION::GetGuest()
* @brief      Get guest
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOWEBSERVER_AUTHENTICATION::GetGuest()
{
  return &guest;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOWEBSERVER_AUTHENTICATION::GetLogin()
* @brief      Get login
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOWEBSERVER_AUTHENTICATION::GetLogin()
{
  return &login;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOWEBSERVER_AUTHENTICATION::GetPassword()
* @brief      Get password
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOWEBSERVER_AUTHENTICATION::GetPassword()
{
  return &password;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_AUTHENTICATION::Set(XSTRING& login, XSTRING& password)
* @brief      Set value
* @ingroup    DATAIO
* 
* @param[in]  login : Login value.
* @param[in]  password : Password text.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER_AUTHENTICATION::Set(XSTRING& login, XSTRING& password)
{
  this->login    = login;
  this->password = password;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_AUTHENTICATION::Set(XSTRING& guest, XSTRING& login, XSTRING& password)
* @brief      Set value
* @ingroup    DATAIO
* 
* @param[in]  guest : Guest value.
* @param[in]  login : Login value.
* @param[in]  password : Password text.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER_AUTHENTICATION::Set(XSTRING& guest, XSTRING& login, XSTRING& password)
{
  this->guest    = guest;
  this->login    = login;
  this->password = password;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBSERVER_AUTHENTICATION::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBSERVER_AUTHENTICATION::Clean()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBSERVER_REQUEST::DIOWEBSERVER_REQUEST()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBSERVER_REQUEST::DIOWEBSERVER_REQUEST()
{
  Clean();
  data = GEN_NEW XBUFFER();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBSERVER_REQUEST::~DIOWEBSERVER_REQUEST()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBSERVER_REQUEST::~DIOWEBSERVER_REQUEST()
{
  GEN_DELETE data;

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBSERVER_TYPE DIOWEBSERVER_REQUEST::GetType()
* @brief      Get type
* @ingroup    DATAIO
* 
* @return     DIOWEBSERVER_TYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBSERVER_TYPE DIOWEBSERVER_REQUEST::GetType()
{
  return type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBSERVER_REQUEST::SetType(DIOWEBSERVER_TYPE type)
* @brief      Set type
* @ingroup    DATAIO
* 
* @param[in]  type : Type value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBSERVER_REQUEST::SetType(DIOWEBSERVER_TYPE type)
{
  this->type = type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBHEADER_METHOD DIOWEBSERVER_REQUEST::GetMethod()
* @brief      Get method
* @ingroup    DATAIO
* 
* @return     DIOWEBHEADER_METHOD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBHEADER_METHOD DIOWEBSERVER_REQUEST::GetMethod()
{
  return method;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBSERVER_REQUEST::SetMethod(DIOWEBHEADER_METHOD method)
* @brief      Set method
* @ingroup    DATAIO
* 
* @param[in]  method : Method descriptor to call.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBSERVER_REQUEST::SetMethod(DIOWEBHEADER_METHOD method)
{
  this->method = method;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOWEBSERVER_REQUEST::GetResource()
* @brief      Get resource
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOWEBSERVER_REQUEST::GetResource()
{
  return &resource;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOWEBSERVER_REQUEST::GetGuest()
* @brief      Get guest
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOWEBSERVER_REQUEST::GetGuest()
{
  return &guest;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOWEBSERVER_REQUEST::GetUserAgent()
* @brief      Get user agent
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOWEBSERVER_REQUEST::GetUserAgent()
{
  return &useragent;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOWEBSERVER_REQUEST::GetSize()
* @brief      Get size
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOWEBSERVER_REQUEST::GetSize()
{
  return size;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBSERVER_REQUEST::SetSize(XDWORD size)
* @brief      Set size
* @ingroup    DATAIO
* 
* @param[in]  size : Size value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBSERVER_REQUEST::SetSize(XDWORD size)
{
  this->size = size;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOWEBSERVER_REQUEST::GetIfNoneMatch()
* @brief      Get if none match
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOWEBSERVER_REQUEST::GetIfNoneMatch()
{
  return &ifnonematch;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBSERVER_REQUEST::SetIfNoneMatch(XSTRING& ifnonematch)
* @brief      Set if none match
* @ingroup    DATAIO
* 
* @param[in]  ifnonematch : Ifnonematch value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBSERVER_REQUEST::SetIfNoneMatch(XSTRING& ifnonematch)
{
  XSTRING special;

  special = ifnonematch;

  special.DeleteCharacter(0x20, XSTRINGCONTEXT_FROM_FIRST);
  special.DeleteCharacter(0x20, XSTRINGCONTEXT_TO_END);
  special.DeleteCharacter(_C('"'), XSTRINGCONTEXT_FROM_FIRST);
  special.DeleteCharacter(_C('"'), XSTRINGCONTEXT_TO_END);

  this->ifnonematch = special;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOWEBSERVER_REQUEST::GetAcceptedCharset()
* @brief      Get accepted charset
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOWEBSERVER_REQUEST::GetAcceptedCharset()
{
  return &acceptedcharset;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOWEBSERVER_REQUEST::GetAcceptedControlRequestMethod()
* @brief      Get accepted control request method
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOWEBSERVER_REQUEST::GetAcceptedControlRequestMethod()
{
  return &acceptedcontrolrequestmethod;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOWEBSERVER_REQUEST::GetAcceptedLanguage()
* @brief      Get accepted language
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOWEBSERVER_REQUEST::GetAcceptedLanguage()
{
  return &acceptedlanguage;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOWEBSERVER_REQUEST::GetAcceptedAllowHeaders()
* @brief      Get accepted allow headers
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOWEBSERVER_REQUEST::GetAcceptedAllowHeaders()
{
  return &acceptedallowheaders;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBSERVER_REQUEST::ProcessAcceptedEncodingFlags(XSTRING& string)
* @brief      Process accepted encoding flags
* @ingroup    DATAIO
* 
* @param[in]  string : String to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBSERVER_REQUEST::ProcessAcceptedEncodingFlags(XSTRING& string)
{
  if(string.IsEmpty()) return;

  //DIEGO: Achtung! you have to calculate the Q = 0 of the headers just in case it is a browser Obsolete

  if(string.Find(_L("gzip"), false) != NOTFOUND)   acceptedencodingflags |= DIOWEBSERVER_ENCODINGFLAGS_GZIP;
  if(string.Find(_L("deflate"), false) != NOTFOUND)   acceptedencodingflags |= DIOWEBSERVER_ENCODINGFLAGS_DEFLATE;
  if(string.Find(_L("compress"), false) != NOTFOUND)   acceptedencodingflags |= DIOWEBSERVER_ENCODINGFLAGS_COMPRESS;
  if(string.Find(_L("sdhc"), false) != NOTFOUND)   acceptedencodingflags |= DIOWEBSERVER_ENCODINGFLAGS_SHAREDDICTIONARY;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOWEBSERVER_REQUEST::GetAccessControlAllowOrigin()
* @brief      Get access control allow origin
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOWEBSERVER_REQUEST::GetAccessControlAllowOrigin()
{
  return &accesscontrolalloworigin;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOWEBSERVER_REQUEST::GetAccessControlAllowMethods()
* @brief      Get access control allow methods
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOWEBSERVER_REQUEST::GetAccessControlAllowMethods()
{
  return &accesscontrolallowmethods;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOWEBSERVER_REQUEST::GetAccessControlAllowHeaders()
* @brief      Get access control allow headers
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOWEBSERVER_REQUEST::GetAccessControlAllowHeaders()
{
  return &accesscontrolallowheaders;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOWEBSERVER_REQUEST::GetOrigin()
* @brief      Get origin
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOWEBSERVER_REQUEST::GetOrigin()
{
  return &origin;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOWEBSERVER_REQUEST::GetConnection()
* @brief      Get connection
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOWEBSERVER_REQUEST::GetConnection()
{
  return &connection;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOWEBSERVER_REQUEST::GetUpgrade()
* @brief      Get upgrade
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOWEBSERVER_REQUEST::GetUpgrade()
{
  return &upgrade;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBUFFER* DIOWEBSERVER_REQUEST::GetData()
* @brief      Get data
* @ingroup    DATAIO
* 
* @return     XBUFFER* : Pointer to the requested buffer; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* DIOWEBSERVER_REQUEST::GetData()
{
  return data;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_REQUEST::HaveLoginPassword()
* @brief      Have login password
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER_REQUEST::HaveLoginPassword()
{
  return (loginpassword.IsEmpty()?false:true);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_REQUEST::GetLoginPassword(XSTRING& login, XSTRING& password)
* @brief      Get login password
* @ingroup    DATAIO
* 
* @param[in]  login : Login value.
* @param[in]  password : Password text.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER_REQUEST::GetLoginPassword(XSTRING& login, XSTRING& password)
{
  login.Empty();
  password.Empty();

  if(loginpassword.IsEmpty()) return false;

  XSTRING base64;
  XBUFFER decoded;
  XSTRING lp;

  // The value of the header "Authorization: Basic xxxxxx" is the BASE64 of "login:password",
  // so it has to be DECODED (before it was encoded, destroying the received value).

  base64 = loginpassword;
  base64.DeleteNoCharacters(_L(" \t\r\n"), 0, XSTRINGCONTEXT_ALLSTRING);

  if(base64.IsEmpty())                        return false;
  if(!base64.ConvertBase64ToBinary(decoded))  return false;
  if(!decoded.GetSize())                      return false;

  if(!lp.Set(decoded.Get(), decoded.GetSize())) return false;

  int index = lp.Find(_L(":"), true);
  if(index ==  XSTRING_NOTFOUND) return false;

  lp.Copy(0, index, login);
  lp.Copy(index+1, password);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBSERVER_REQUEST::SetLoginPassword(XSTRING& loginpassword)
* @brief      Set login password
* @ingroup    DATAIO
* 
* @param[in]  loginpassword : Loginpassword value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBSERVER_REQUEST::SetLoginPassword(XSTRING& loginpassword)
{
  this->loginpassword = loginpassword;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_REQUEST::ConvertFromWeb(XSTRING& string)
* @brief      Convert from web
* @ingroup    DATAIO
* 
* @param[in]  string : String to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER_REQUEST::ConvertFromWeb(XSTRING& string)
{
  int c = 0;

  XCHAR* str = string.Get();

  while(str[c])
    {
      if(str[c]==_C('%'))
        {
          XSTRING string2;

          string2.Format(_L("%c%c"), str[c+2], str[c+1]);

          int code = string2.ConvertToInt();
          if(code)
            {
              str[c]=code;

              string.DeleteCharacters(c+1, 2);
              str = string.Get();
            }

        } else c++;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_REQUEST::IsAcceptedEncodingFlag(DIOWEBSERVER_ENCODINGFLAGS flag)
* @brief      Is accepted encoding flag
* @ingroup    DATAIO
* 
* @param[in]  flag : Flag value.
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER_REQUEST::IsAcceptedEncodingFlag(DIOWEBSERVER_ENCODINGFLAGS flag)
{
  return ((acceptedencodingflags&flag) != 0);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOWEBSERVER_REQUEST::WebSocket_GetKey()
* @brief      Web socket get key
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOWEBSERVER_REQUEST::WebSocket_GetKey()
{
  return &websocket_key;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOWEBSERVER_REQUEST::WebSocket_GetProtocol()
* @brief      Web socket get protocol
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOWEBSERVER_REQUEST::WebSocket_GetProtocol()
{
  return &websocket_protocol;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOWEBSERVER_REQUEST::WebSocket_GetVersion()
* @brief      Web socket get version
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOWEBSERVER_REQUEST::WebSocket_GetVersion()
{
  return websocket_version;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBSERVER_REQUEST::WebSocket_SetVersion(int version)
* @brief      Web socket set version
* @ingroup    DATAIO
* 
* @param[in]  version : Version value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBSERVER_REQUEST::WebSocket_SetVersion(int version)
{
  websocket_version = version;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBSERVER_REQUEST::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBSERVER_REQUEST::Clean()
{
  type                      = DIOWEBSERVER_TYPE_NONE;
  method                    = DIOWEBHEADER_METHOD_UNKNOWN;

  resource.Empty();
  guest.Empty();
  useragent.Empty();
  loginpassword.Empty();
  ifnonematch.Empty();

  acceptedcharset.Empty();
  acceptedcontrolrequestmethod.Empty();
  acceptedlanguage.Empty();
  acceptedallowheaders.Empty();

  acceptedencodingflags     = DIOWEBSERVER_ENCODINGFLAGS_NONE;

  accesscontrolalloworigin.Empty();
  accesscontrolallowmethods.Empty();
  accesscontrolallowheaders.Empty();

  origin.Empty();

  connection.Empty();
  upgrade.Empty();

  iskeepalive               = false;

  websocket_isrequest       = false;
  websocket_key.Empty();
  websocket_protocol.Empty();
  websocket_version         = 0;

  size                      = -1;
  data                      = NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBSERVER_CONNECTION::DIOWEBSERVER_CONNECTION()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBSERVER_CONNECTION::DIOWEBSERVER_CONNECTION()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBSERVER_CONNECTION::~DIOWEBSERVER_CONNECTION()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBSERVER_CONNECTION::~DIOWEBSERVER_CONNECTION()
{
  End();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_CONNECTION::Ini(DIOWEBSERVER* webserver, DIOSTREAMTCPIPCONFIG* diostreamcfg)
* @brief      Initialize the object
* @ingroup    DATAIO
* 
* @param[in]  webserver : Webserver pointer to use.
* @param[in]  diostreamcfg : Diostreamconfiguration pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER_CONNECTION::Ini(DIOWEBSERVER* webserver, DIOSTREAMTCPIPCONFIG* diostreamcfg)
{
  this->webserver     = webserver;
  this->diostreamcfg  = diostreamcfg;
  this->isopening     = false;
  if(webserver)
    {
      operativetimeouts = (*webserver->GetTimeouts());
      operativelimits   = (*webserver->GetLimits());
    }

  SetMode(DIOWEBSERVER_CONNECTION_MODE_UNKNOWN);

  xtimerconnection=GEN_XFACTORY.CreateTimer();
  if(!xtimerconnection)      return false;
  
  xtimerdisconnection=GEN_XFACTORY.CreateTimer();
  if(!xtimerdisconnection)   return false;
    
  nresourcesprocessed = 0;

  diostream = (DIOSTREAMTCPIP*)GEN_DIOFACTORY.CreateStreamIO(diostreamcfg);
  if(!diostream)          return false;

  threadconnection   = CREATEXTHREAD(XTHREADGROUPID_DIOWEBSERVER, _L("DIOWEBSERVER_CONNECTION::Ini"), ThreadRunFunction, (void*)this);
  if(!threadconnection)   return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTCPIP* DIOWEBSERVER_CONNECTION::GetDIOStream()
* @brief      Get DIO stream
* @ingroup    DATAIO
* 
* @return     DIOSTREAMTCPIP* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTCPIP* DIOWEBSERVER_CONNECTION::GetDIOStream()
{
  return diostream;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTIMER* DIOWEBSERVER_CONNECTION::GetTimerConnection()
* @brief      Get timer connection
* @ingroup    DATAIO
* 
* @return     XTIMER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTIMER* DIOWEBSERVER_CONNECTION::GetTimerConnection()
{
  return xtimerconnection;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_CONNECTION::IsActive()
* @brief      Is active
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER_CONNECTION::IsActive()
{
  return isactive;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_CONNECTION::Activate()
* @brief      Activate
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER_CONNECTION::Activate()
{
  if(!threadconnection) return false;
  if(!xtimerconnection) return false;

  isactive = true;

  threadconnection->Ini();

  xtimerconnection->Reset();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_CONNECTION::ActivateOpen()
* @brief      Starts the connection worker and lets it perform the stream Open().
* @note       Used by TLS server connections because DIOSTREAMTLS::Open() includes the complete synchronous handshake.
*             Running it in the connection worker prevents the web server accept loop from being blocked by a slow peer.
* @ingroup    DATAIO
* 
* @return     bool : true if the worker was started; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER_CONNECTION::ActivateOpen()
{
  if(!threadconnection) return false;
  if(!xtimerconnection) return false;
  if(!diostream)        return false;

  isopening = true;
  isactive  = false;

  threadconnection->Ini();

  xtimerconnection->Reset();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_CONNECTION::IsOpening()
* @brief      Returns whether the connection worker is opening the stream / completing the TLS handshake.
* @ingroup    DATAIO
* 
* @return     bool : true while Open() is in progress.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER_CONNECTION::IsOpening()
{
  return isopening;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_CONNECTION::Deactivate()
* @brief      Deactivate
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER_CONNECTION::Deactivate()
{
  if(!GetDIOStream())   return false;
  if(!threadconnection) return false;

 // End Server connection page
  GetDIOStream()->WaitToFlushOutXBuffer(5);
  GetDIOStream()->Disconnect();

  threadconnection->Run(false);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBSERVER_CONNECTION_MODE DIOWEBSERVER_CONNECTION::GetMode()
* @brief      Get mode
* @ingroup    DATAIO
* 
* @return     DIOWEBSERVER_CONNECTION_MODE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBSERVER_CONNECTION_MODE DIOWEBSERVER_CONNECTION::GetMode()
{
  return mode;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBSERVER_CONNECTION::SetMode(DIOWEBSERVER_CONNECTION_MODE mode)
* @brief      Set mode
* @ingroup    DATAIO
* 
* @param[in]  mode : Mode value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBSERVER_CONNECTION::SetMode(DIOWEBSERVER_CONNECTION_MODE mode)
{
  this->mode = mode;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_CONNECTION::Receiver(XBYTE* buffer, XDWORD& size, int timeout)
* @brief      Receiver
* @ingroup    DATAIO
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER_CONNECTION::Receiver(XBYTE* buffer, XDWORD& size, int timeout)
{
  if(!diostream)                return false;
  if(!diostream->IsConnected()) return false;
  if(!size)                     return false;

  XDWORD br;

  XTIMER* xtimerout=GEN_XFACTORY.CreateTimer();
  if(!xtimerout)  return false;
  
  bool status = true;

  while(1)
    {
      if(!diostream)
        {
          status = false;
          break;
        }

      if(!diostream->IsConnected())
        {
          status = false;
          break;
        }

      br = diostream->Read(buffer, size);
      if(br) break;

      if(xtimerout->GetMeasureSeconds()>= (XDWORD)timeout) break;
    }

  GEN_XFACTORY.DeleteTimer(xtimerout);

  if(!br) return false;
  size = br;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_CONNECTION::Send(XBYTE* buffer, XDWORD& size, int timeout)
* @brief      Send
* @ingroup    DATAIO
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER_CONNECTION::Send(XBYTE* buffer, XDWORD& size, int timeout)
{
  if(!diostream) return false;
  if(!size)      return false;

  bool status = true;
  int  bw     = 0;

  bw = diostream->Write(buffer, size);
  if(size != bw) status = false;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_CONNECTION::Send(XBUFFER& xbuffer, int timeout)
* @brief      Send
* @ingroup    DATAIO
* 
* @param[in]  xbuffer : Buffer to use.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER_CONNECTION::Send(XBUFFER& xbuffer, int timeout)
{
  XDWORD size = xbuffer.GetSize();

  return Send(xbuffer.Get(), size, timeout);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_CONNECTION::IsRequestInProgress()
* @brief      Is request in progress
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER_CONNECTION::IsRequestInProgress()
{
  return isrequestinprogress;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBSERVER* DIOWEBSERVER_CONNECTION::GetServer()
* @brief      Get server
* @ingroup    DATAIO
* 
* @return     DIOWEBSERVER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBSERVER* DIOWEBSERVER_CONNECTION::GetServer()
{
  return this->webserver;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBCLIENT_HEADER* DIOWEBSERVER_CONNECTION::GetHeader()
* @brief      Get header
* @ingroup    DATAIO
* 
* @return     DIOWEBCLIENT_HEADER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBCLIENT_HEADER* DIOWEBSERVER_CONNECTION::GetHeader()
{
  return &header;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBSERVER_REQUEST* DIOWEBSERVER_CONNECTION::GetRequest()
* @brief      Get request
* @ingroup    DATAIO
* 
* @return     DIOWEBSERVER_REQUEST* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBSERVER_REQUEST* DIOWEBSERVER_CONNECTION::GetRequest()
{
  return &request;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBSERVER_ENCODINGFLAGS DIOWEBSERVER_CONNECTION::GetActualEncodingFlag()
* @brief      Get actual encoding flag
* @ingroup    DATAIO
* 
* @return     DIOWEBSERVER_ENCODINGFLAGS : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBSERVER_ENCODINGFLAGS DIOWEBSERVER_CONNECTION::GetActualEncodingFlag()
{
  return actualencodingflag;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_CONNECTION::SetActualEncodingFlag(DIOWEBSERVER_ENCODINGFLAGS actualencodingflag)
* @brief      Set actual encoding flag
* @ingroup    DATAIO
* 
* @param[in]  actualencodingflag : Actualencodingflag value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER_CONNECTION::SetActualEncodingFlag(DIOWEBSERVER_ENCODINGFLAGS actualencodingflag)
{
  if(!webserver) return false;

  if(!webserver->IsIncludedEncodingFlag(actualencodingflag))
    {
      actualencodingflag = DIOWEBSERVER_ENCODINGFLAGS_NONE;
      return false;
    }

  this->actualencodingflag = actualencodingflag;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_CONNECTION::WebSocket_Write(XCHAR* string, int timeout)
* @brief      Web socket write
* @ingroup    DATAIO
* 
* @param[in]  string : String to use.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER_CONNECTION::WebSocket_Write(XCHAR* string, int timeout)
{
  XBUFFER  data;
  XSTRING  _string;

  _string = string;

  WebSocket_CreateHeader(DIOWEBSERVER_WEBSOCKET_OPCODE_TEXT_FRAME, _string.GetSize(), data);
       
  XBUFFER charstr;
  
  _string.ConvertToASCII(charstr); 
  data.Add((XBYTE*)charstr.Get(), _string.GetSize());
  
  return Send(data, timeout);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_CONNECTION::WebSocket_Write(XSTRING& string, int timeout)
* @brief      Web socket write
* @ingroup    DATAIO
* 
* @param[in]  string : String to use.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER_CONNECTION::WebSocket_Write(XSTRING& string, int timeout)
{
  return WebSocket_Write(string.Get(), timeout);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_CONNECTION::WebSocket_Write(XBYTE* buffer, XDWORD size, int timeout)
* @brief      Web socket write
* @ingroup    DATAIO
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  size : Size value.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER_CONNECTION::WebSocket_Write(XBYTE* buffer, XDWORD size, int timeout)
{
  XBUFFER data;

  WebSocket_CreateHeader(DIOWEBSERVER_WEBSOCKET_OPCODE_BINARY_FRAME, size, data);

  data.Add(buffer, size);

  return Send(data, timeout);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_CONNECTION::WebSocket_Write(XBUFFER& buffer, int timeout)
* @brief      Web socket write
* @ingroup    DATAIO
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER_CONNECTION::WebSocket_Write(XBUFFER& buffer, int timeout)
{
  return WebSocket_Write(buffer.Get(), buffer.GetSize(), timeout);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_CONNECTION::WebSocket_WritePingPong(bool isping, XCHAR* string, int timeout)
* @brief      Web socket write ping pong
* @ingroup    DATAIO
* 
* @param[in]  isping : Isping value.
* @param[in]  string : String to use.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER_CONNECTION::WebSocket_WritePingPong(bool isping, XCHAR* string, int timeout)
{
  XBUFFER  data;
  XSTRING  _string;

  _string = string;

  WebSocket_CreateHeader((isping?DIOWEBSERVER_WEBSOCKET_OPCODE_PING:DIOWEBSERVER_WEBSOCKET_OPCODE_PONG), _string.GetSize(), data);

  XBUFFER charstr;
  
  _string.ConvertToASCII(charstr);
  data.Add((XBYTE*)charstr.Get(), _string.GetSize());
  
  return Send(data, timeout);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_CONNECTION::WebSocket_WritePingPong(bool isping, XSTRING& string, int timeout)
* @brief      Web socket write ping pong
* @ingroup    DATAIO
* 
* @param[in]  isping : Isping value.
* @param[in]  string : String to use.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER_CONNECTION::WebSocket_WritePingPong(bool isping, XSTRING& string, int timeout)
{
  return WebSocket_WritePingPong(isping, string.Get(), timeout);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_CONNECTION::WebSocket_WritePingPong(bool isping, XBUFFER& data, int timeout)
* @brief      Web socket write ping pong
* @ingroup    DATAIO
* 
* @param[in]  isping : Isping value.
* @param[in]  data : Data buffer to use.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER_CONNECTION::WebSocket_WritePingPong(bool isping, XBUFFER& data, int timeout)
{
  XBUFFER _data;

  WebSocket_CreateHeader((isping?DIOWEBSERVER_WEBSOCKET_OPCODE_PING:DIOWEBSERVER_WEBSOCKET_OPCODE_PONG), data.GetSize(), _data);

  _data.Add(data.Get(), data.GetSize());

  return Send(_data, timeout);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTIMER* DIOWEBSERVER_CONNECTION::GetTimerDisconnection()
* @brief      Get timer disconnection
* @ingroup    DATAIO
* 
* @return     XTIMER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTIMER* DIOWEBSERVER_CONNECTION::GetTimerDisconnection()
{
 return xtimerdisconnection;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_CONNECTION::End()
* @brief      End the object
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER_CONNECTION::End()
{
  if(threadconnection)
    {
      threadconnection->End();
      DELETEXTHREAD(XTHREADGROUPID_DIOWEBSERVER, threadconnection);
      threadconnection = NULL;
    }

  if(diostream)
    {
      diostream->Close();

      GEN_DIOFACTORY.DeleteStreamIO(diostream);
      diostream = NULL;
    }

  if(xtimerconnection)
    {
      GEN_XFACTORY.DeleteTimer(xtimerconnection);
      xtimerconnection = NULL;
    }

  if(xtimerdisconnection)
    {
      GEN_XFACTORY.DeleteTimer(xtimerdisconnection);
      xtimerdisconnection = NULL;
    }

  isactive            = false;
  isopening           = false;
  isrequestinprogress = false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_CONNECTION::TryAcquireReference()
* @brief      Try acquire reference
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER_CONNECTION::TryAcquireReference()
{
  if(!isactive) return false;
  websocketreferences.fetch_add(1, std::memory_order_acquire);
  if(!isactive) { websocketreferences.fetch_sub(1, std::memory_order_release); return false; }
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBSERVER_CONNECTION::ReleaseReference()
* @brief      Release reference
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBSERVER_CONNECTION::ReleaseReference()
{
  websocketreferences.fetch_sub(1, std::memory_order_release);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOWEBSERVER_CONNECTION::GetReferenceCount() const
* @brief      Get reference count
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOWEBSERVER_CONNECTION::GetReferenceCount() const
{
  return websocketreferences.load(std::memory_order_acquire);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_CONNECTION::ReadRequest()
* @brief      Read request
* @ingroup    DATAIO
* 
* @param[in]  line : Line value.
* @param[in]  colon : Colon value.
* @param[in]  name : Pointer to name.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool DIOWEBSERVER_HeaderNameIs(XSTRING& line, XDWORD colon, const XCHAR* name)
{
  if(!name || colon != XSTRING::GetSize(name)) return false;
  for(XDWORD c=0; c<colon; c++)
    {
      XCHAR left = line[c];
      XCHAR right = name[c];
      if(left >= _C('A') && left <= _C('Z')) left += _C('a') - _C('A');
      if(right >= _C('A') && right <= _C('Z')) right += _C('a') - _C('A');
      if(left != right) return false;
    }
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_HeaderNameCharacterIsValid(XCHAR character)
* @brief      Header name character is valid
* @ingroup    DATAIO
* 
* @param[in]  character : Character value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool DIOWEBSERVER_HeaderNameCharacterIsValid(XCHAR character)
{
  if((character >= _C('a') && character <= _C('z')) ||
     (character >= _C('A') && character <= _C('Z')) ||
     (character >= _C('0') && character <= _C('9'))) return true;

  switch(character)
    {
      case _C('!'): case _C('#'): case _C('$'): case _C('%'): case _C('&'): case _C('\''):
      case _C('*'): case _C('+'): case _C('-'): case _C('.'): case _C('^'): case _C('_'):
      case _C('`'): case _C('|'): case _C('~'): return true;
      default: return false;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_RequestFraming_Get(DIOWEBHEADER& header, XDWORD maximumbodysize, XDWORD& contentlength)
* @brief      Request framing get
* @ingroup    DATAIO
* 
* @param[in]  header : Header value.
* @param[in]  maximumbodysize : Maximumbodysize value.
* @param[in]  contentlength : Contentlength value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool DIOWEBSERVER_RequestFraming_Get(DIOWEBHEADER& header, XDWORD maximumbodysize, XDWORD& contentlength)
{
  XVECTOR<XSTRING*>* lines = header.GetLines();
  if(!lines || lines->IsEmpty()) return false;

  contentlength = 0;
  bool havecontentlength = false;
  bool havehost = false;

  for(XDWORD c=1; c<lines->GetSize(); c++)
    {
      XSTRING* line = lines->Get(c);
      if(!line || line->IsEmpty() || (*line)[0] == _C(' ') || (*line)[0] == _C('\t')) return false;

      XDWORD colon = 0;
      while(colon < line->GetSize() && (*line)[colon] != _C(':'))
        {
          if(!DIOWEBSERVER_HeaderNameCharacterIsValid((*line)[colon])) return false;
          colon++;
        }
      if(!colon || colon >= line->GetSize()) return false;

      if(DIOWEBSERVER_HeaderNameIs((*line), colon, _L("Transfer-Encoding"))) return false;

      if(DIOWEBSERVER_HeaderNameIs((*line), colon, _L("Host")))
        {
          if(havehost) return false;
          havehost = true;
        }

      if(DIOWEBSERVER_HeaderNameIs((*line), colon, _L("Content-Length")))
        {
          if(havecontentlength) return false;
          havecontentlength = true;

          XDWORD start = colon + 1;
          XDWORD end = line->GetSize();
          while(start < end && ((*line)[start] == _C(' ') || (*line)[start] == _C('\t'))) start++;
          while(end > start && ((*line)[end-1] == _C(' ') || (*line)[end-1] == _C('\t'))) end--;
          if(start == end) return false;

          XQWORD value = 0;
          for(XDWORD d=start; d<end; d++)
            {
              if((*line)[d] < _C('0') || (*line)[d] > _C('9')) return false;
              XDWORD digit = (XDWORD)((*line)[d] - _C('0'));
              if((value > ((XQWORD)maximumbodysize / 10)) ||
                 ((value == ((XQWORD)maximumbodysize / 10)) && (digit > (maximumbodysize % 10)))) return false;
              value = (value * 10) + digit;
            }
          if(value > maximumbodysize || value > 0x7FFFFFFF) return false;
          contentlength = (XDWORD)value;
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_CONNECTION::ReadRequest()
* @brief      Read request
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER_CONNECTION::ReadRequest()
{
  if(!diostream) return false;

  if(!diostream->IsConnected())                 return false;

  if(diostream->GetInXBuffer()->GetSize() < 5)  return false;

  int headerstimeout = operativetimeouts.httpheaders;
  XDWORD maximumheadersize = operativelimits.maximumheadersize;
  bool headerstatus = header.Read(diostream, headerstimeout, maximumheadersize);

  if(!headerstatus)  return false;

  XDWORD contentlength = 0;
  XDWORD maximumbodysize = operativelimits.maximumbodysize;
  if(!DIOWEBSERVER_RequestFraming_Get(header, maximumbodysize, contentlength)) return false;

  XSTRING string;

  string = (*(XSTRING*)(header.GetLines()->Get(0)));

  if(string.Find(_L("HTTP/"), true, 4)!=XSTRING_NOTFOUND)
    {
      request.SetType(DIOWEBSERVER_TYPE_FULL);

    } else request.SetType(DIOWEBSERVER_TYPE_SYMPLE);

  header.GetMethod(string);

  //XTRACE_PRINTCOLOR(1, _L("method: %s"), string.Get());

  request.SetMethod(DIOWEBHEADER_METHOD_UNKNOWN);

  if(!string.Compare(_L("GET")     , true))  request.SetMethod(DIOWEBHEADER_METHOD_GET);
  if(!string.Compare(_L("HEAD")    , true))  request.SetMethod(DIOWEBHEADER_METHOD_HEAD);
  if(!string.Compare(_L("POST")    , true))  request.SetMethod(DIOWEBHEADER_METHOD_POST);
  if(!string.Compare(_L("PUT")     , true))  request.SetMethod(DIOWEBHEADER_METHOD_PUT);
  if(!string.Compare(_L("DELETE")  , true))  request.SetMethod(DIOWEBHEADER_METHOD_DELETE);
  if(!string.Compare(_L("CONNECT") , true))  request.SetMethod(DIOWEBHEADER_METHOD_CONNECT);
  if(!string.Compare(_L("OPTIONS") , true))  request.SetMethod(DIOWEBHEADER_METHOD_OPTIONS);
  if(!string.Compare(_L("TRACE")   , true))  request.SetMethod(DIOWEBHEADER_METHOD_TRACE);

  header.GetResource(string);
  if(string.GetSize()) request.GetResource()->Set(string);

  string = header.GetFieldValue(_L("Host:"));
  request.GetGuest()->Set(string);

  string = header.GetFieldValue(_L("User-Agent:"));
  request.GetUserAgent()->Set(string);

  request.SetSize(contentlength);

  string = header.GetFieldValue(_L("If-None-Match:"));
  request.SetIfNoneMatch(string);

  string = header.GetFieldValue(_L("Authorization: basic"));
  request.SetLoginPassword(string);

  string = header.GetFieldValue(_L("Accept-Encoding:"));
  request.ProcessAcceptedEncodingFlags(string);

  string = header.GetFieldValue(_L("Accept-Language:"));
  request.GetAcceptedLanguage()->Set(string);

  string = header.GetFieldValue(_L("Accept-Charset:"));
  request.GetAcceptedCharset()->Set(string);

  string = header.GetFieldValue(_L("Access-Control-Allow-Origin:"));
  request.GetAccessControlAllowOrigin()->Set(string);

  string = header.GetFieldValue(_L("Access-Control-Allow-Methods:"));
  request.GetAcceptedControlRequestMethod()->Set(string);

  string = header.GetFieldValue(_L("Access-Control-Allow-Headers:"));
  request.GetAccessControlAllowHeaders()->Set(string);

  string = header.GetFieldValue(_L("origin:"));
  request.GetOrigin()->Set(string);

  string = header.GetFieldValue(_L("Connection:"));
  request.GetConnection()->Set(string);

  string = header.GetFieldValue(_L("Upgrade:"));
  request.GetUpgrade()->Set(string);

  string = header.GetFieldValue(_L("Sec-WebSocket-Key:"));
  request.WebSocket_GetKey()->Set(string);

  string = header.GetFieldValue(_L("Sec-WebSocket-Protocol:"));
  request.WebSocket_GetProtocol()->Set(string);

  string = header.GetFieldValue(_L("Sec-WebSocket-Version:"));
  request.WebSocket_SetVersion(string.ConvertToInt());

  if(request.GetSize()>0)
    {
      XBUFFER* data = request.GetData();
      if(!data) return false;

      XDWORD size;

      data->Delete();

      XBYTE* buffer = GEN_NEW XBYTE[DIOWEBSERVER_MAXBUFFER];
      if(!buffer) return false;

      XTIMER* bodytimer = GEN_XFACTORY.CreateTimer();
      if(!bodytimer)
        {
          GEN_DELETE_ARRAY buffer;
          return false;
        }

      bool bodystatus = true;
      int bodytimeout = operativetimeouts.httpbody;

      do{ memset(buffer, 0, DIOWEBSERVER_MAXBUFFER);
          size = (request.GetSize() - data->GetSize());
          if(size>DIOWEBSERVER_MAXBUFFER) size = DIOWEBSERVER_MAXBUFFER;

          if(size)
            {
              XDWORD elapsed = bodytimer->GetMeasureSeconds();
              int remaining = (elapsed < (XDWORD)bodytimeout)?(bodytimeout - (int)elapsed):0;
              if(!remaining || !Receiver((XBYTE*)buffer, size, remaining))
                {
                  bodystatus = false;
                  size = 0;
                }
              if(size) data->Add((XBYTE*)buffer, size);
            }

        } while(size);

      GEN_XFACTORY.DeleteTimer(bodytimer);
      GEN_DELETE_ARRAY buffer;

      if(!bodystatus || (data->GetSize() != (XDWORD)request.GetSize())) return false;
    }

  //----------------------------------------------------------------------------------

  SetMode(DIOWEBSERVER_CONNECTION_MODE_NORMAL);

  if(!request.GetConnection()->Compare(_L("Keep-Alive"), true))
    {
      SetMode(DIOWEBSERVER_CONNECTION_MODE_KEEPALIVE);
    }

  if(!request.GetConnection()->Compare(_L("Upgrade"), true) &&
     !request.GetUpgrade()->Compare(_L("websocket")))
    {
      SetMode(DIOWEBSERVER_CONNECTION_MODE_WEBSOCKET);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_CONNECTION::SendRequest()
* @brief      Send request
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER_CONNECTION::SendRequest()
{
  if(!webserver) return false;
  if(!diostream) return false;

  if(request.GetMethod() == DIOWEBHEADER_METHOD_UNKNOWN)
    {
      return false;
    }

  GEN_XLOG.AddEntry(XLOGLEVEL_INFO, DIOWEBSERVER_LOGSECTIONID_VERBOSE, false, _L("[%08X] Server Page INI [%d] %s"), this, nresourcesprocessed, request.GetResource()->Get());

  DIOWEBSERVER_XEVENT xevent(webserver, DIOWEBSERVER_XEVENT_TYPE_REQUEST);
  xevent.SetServer(webserver);
  xevent.SetConnection(this);
  xevent.SetRequest(&request);

  webserver->PostEvent(&xevent);

  GEN_XLOG.AddEntry(XLOGLEVEL_INFO, DIOWEBSERVER_LOGSECTIONID_VERBOSE, false, _L("[%08X] Server Page END [%d] %s"), this, nresourcesprocessed, request.GetResource()->Get());

  nresourcesprocessed++;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_CONNECTION::WebSocket_CreateAcceptKey(XSTRING& key, XSTRING& result)
* @brief      Web socket create accept key
* @ingroup    DATAIO
* 
* @param[in]  key : Key value.
* @param[in]  result : Output result.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER_CONNECTION::WebSocket_CreateAcceptKey(XSTRING& key, XSTRING& result)
{
  if(key.IsEmpty()) return false;

  #define DIOWEBSERVER_WEBSOCKET_GUID  _L("258EAFA5-E914-47DA-95CA-C5AB0DC85B11")

  XSTRING resultbrute;
  bool    status;

  result.Empty();

  resultbrute  = key;
  resultbrute += DIOWEBSERVER_WEBSOCKET_GUID;

  result.Empty();

  HASHSHA1* sha1 = GEN_NEW HASHSHA1();
  if(!sha1) return false;

  XBUFFER charstr;
  
  resultbrute.ConvertToASCII(charstr);
  status = sha1->Do((XBYTE*)charstr.Get(), resultbrute.GetSize());
  
  sha1->GetResult()->ConvertToBase64(result);

  GEN_DELETE sha1;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_CONNECTION::WebSocket_CreateHeader(DIOWEBSERVER_WEBSOCKET_OPCODE opcode, XDWORD size, XBUFFER& data)
* @brief      Web socket create header
* @ingroup    DATAIO
* 
* @param[in]  opcode : Opcode value.
* @param[in]  size : Size value.
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER_CONNECTION::WebSocket_CreateHeader(DIOWEBSERVER_WEBSOCKET_OPCODE opcode, XDWORD size, XBUFFER& data)
{
  data.Empty();

  XWORD miniheader = 0x8000;

  miniheader |= (opcode << 8);

  if(size >= 65536)
    {
      miniheader |= 127;
      data.Add((XWORD)miniheader);

      XQWORD sizedata = size;
      data.Add((XQWORD)sizedata);
    }
   else
    {
      if(size >= 256)
        {
          miniheader |= 126;
          data.Add((XWORD)miniheader);

          XWORD sizedata = size;
          data.Add((XWORD)sizedata);
        }
       else
        {
          miniheader |=  (XBYTE)size;
          data.Add((XWORD)miniheader);
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_CONNECTION::Websocket_ResolveConnection(DIOWEBSERVER_REQUEST* request)
* @brief      Websocket resolve connection
* @ingroup    DATAIO
* 
* @param[in]  request : Request pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER_CONNECTION::Websocket_ResolveConnection(DIOWEBSERVER_REQUEST* request)
{
  DIOWEBSERVER_HEADER webserverheader;
  XSTRING             resultwebsocketaccept;
  bool                status = false;

  if(!WebSocket_CreateAcceptKey((*GetRequest()->WebSocket_GetKey()) , resultwebsocketaccept)) return false;

  webserverheader.Create(GetRequest()->GetResource(), 1, 1, DIOWEBHEADER_RESULT_SWITCHINGPROTOCOLS, -1, NULL, false);
  webserverheader.AddWebSocketKey(resultwebsocketaccept, (*GetRequest()->WebSocket_GetProtocol()));

  status = webserverheader.Write(GetDIOStream(), 5);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_CONNECTION::WebSocket_ReadRequest(DIOWEBSERVER_WEBSOCKET_OPCODE& opcode, XBUFFER& data)
* @brief      Web socket read request
* @ingroup    DATAIO
* 
* @param[in]  opcode : Opcode value.
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER_CONNECTION::WebSocket_ReadRequest(DIOWEBSERVER_WEBSOCKET_OPCODE& opcode, XBUFFER& data)
{
  opcode = DIOWEBSERVER_WEBSOCKET_OPCODE_RESERVED10;

  if(!diostream)                return false;
  if(!diostream->IsConnected()) return false;

  XDWORD  sizetoread = diostream->GetInXBuffer()->GetSize();
  if(sizetoread <  sizeof(WEBSOCKET_HEADERFRAME))  return false;

   XWORD                 hf          = 0;
   WEBSOCKET_HEADERFRAME headerframe;

   if(!diostream->GetInXBuffer()->Get((XBYTE*)&hf, sizeof(XWORD))) return false;

   if(GEN_XSYSTEM.HardwareUseLittleEndian())  SWAPWORD(hf);

   memcpy(&headerframe, &hf, sizeof(WEBSOCKET_HEADERFRAME));

   XQWORD sizeallmsg      = sizeof(XWORD);
   XBYTE  extpayloadsize  = 0;
   XQWORD sizedata        = headerframe.size;

   if(sizedata == 127)
     {
       if(!diostream->GetInXBuffer()->Get((XBYTE*)&sizedata, 8, 2 )) return false;
       if(GEN_XSYSTEM.HardwareUseLittleEndian())  SWAPQWORD(sizedata);

       extpayloadsize = sizeof(XQWORD);
       sizeallmsg     += sizedata;
     }
    else
     {
       if(sizedata == 126)
         {
            XWORD msize = 0;
            if(!diostream->GetInXBuffer()->Get((XBYTE*)&msize, 2, 2 )) return false;
            if(GEN_XSYSTEM.HardwareUseLittleEndian())  SWAPWORD(msize);

            sizedata       = msize;
            extpayloadsize = sizeof(XWORD);
            sizeallmsg    += sizedata;
         }
        else
         {
           sizeallmsg += headerframe.size;
         }
    }

  sizeallmsg += extpayloadsize;
  if(headerframe.mask) sizeallmsg += sizeof(XDWORD);

  // Have all frame available to Read?
  if(sizetoread < sizeallmsg) return false;

  if(!diostream->GetInXBuffer()->Extract(NULL, 0, (sizeof(XWORD) + extpayloadsize))) return false;

  XBYTE maskcipher[4];


  if(headerframe.mask)
    {
      if(!diostream->Read(maskcipher, sizeof(maskcipher))) return false;
    }

  data.Empty();

  if(sizedata)
    {
      data.Resize((XDWORD)sizedata);

      if(!diostream->Read(data)) return false;

      if(headerframe.mask)
        {
          for(XDWORD c = 0; c<data.GetSize(); c++)
            {
              data.Get()[c] ^= maskcipher[c % 4];
            }
        }
    }

  opcode = (DIOWEBSERVER_WEBSOCKET_OPCODE)headerframe.opcode;

  websocket_xbufferframepart.Add(data);

  if(opcode == DIOWEBSERVER_WEBSOCKET_OPCODE_CONTINUATION_FRAME)
    {
      return false;
    }

  if(headerframe.final)
    {
      data.Empty();
      data.Add(websocket_xbufferframepart);

      websocket_xbufferframepart.Empty();

      return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_CONNECTION::WebSocket_SendEvent_Connected()
* @brief      Web socket send event connected
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER_CONNECTION::WebSocket_SendEvent_Connected()
{
  if(!webserver) return false;
  if(!diostream) return false;

  if(request.GetMethod() == DIOWEBHEADER_METHOD_UNKNOWN) return false;

  bool status = false;

  DIOWEBSERVER_XEVENT xevent(webserver, DIOWEBSERVER_XEVENT_TYPE_WEBSOCKET_CONNECTED);
  xevent.SetServer(webserver);
  xevent.SetConnection(this);
  xevent.SetRequest(&request);
  xevent.SetStatus(false);

  webserver->PostEvent(&xevent);

  if(xevent.GetStatus()) status = Websocket_ResolveConnection(&request);

  GEN_XLOG.AddEntry((status?XLOGLEVEL_INFO:XLOGLEVEL_WARNING), DIOWEBSERVER_LOGSECTIONID_VERBOSE, false, _L("[%08X] Websocket %s %d %s %s"), this, (status?_L("Connected"):_L("No connected")), request.WebSocket_GetVersion(), request.WebSocket_GetProtocol()->Get(), request.GetResource()->Get());

  isrequestinprogress = false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_CONNECTION::WebSocket_SendEvent_ReadData(XBUFFER& data)
* @brief      Web socket send event read data
* @ingroup    DATAIO
* 
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER_CONNECTION::WebSocket_SendEvent_ReadData(XBUFFER& data)
{
  if(!webserver) return false;
  if(!diostream) return false;

  DIOWEBSERVER_XEVENT xevent(webserver, DIOWEBSERVER_XEVENT_TYPE_WEBSOCKET_READDATA);
  xevent.SetServer(webserver);
  xevent.SetConnection(this);
  xevent.SetRequest(&request);
  xevent.GetReadDataWebSocket()->Empty();
  xevent.GetReadDataWebSocket()->Add(data);

  webserver->PostEvent(&xevent);

  GEN_XLOG.AddEntry(XLOGLEVEL_INFO, DIOWEBSERVER_LOGSECTIONID_VERBOSE, false, _L("[%08X] Websocket read data %d %s %s"), this, request.WebSocket_GetVersion(), request.WebSocket_GetProtocol()->Get(), request.GetResource()->Get());
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_CONNECTION::WebSocket_SendEvent_Pong(XBUFFER& data)
* @brief      Web socket send event pong
* @ingroup    DATAIO
* 
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER_CONNECTION::WebSocket_SendEvent_Pong(XBUFFER& data)
{
  if(!webserver) return false;
  if(!diostream) return false;

  DIOWEBSERVER_XEVENT xevent(webserver, DIOWEBSERVER_XEVENT_TYPE_WEBSOCKET_PONG);
  xevent.SetServer(webserver);
  xevent.SetConnection(this);
  xevent.SetRequest(&request);
  xevent.GetReadDataWebSocket()->Empty();
  xevent.GetReadDataWebSocket()->Add(data);

  webserver->PostEvent(&xevent);

  GEN_XLOG.AddEntry(XLOGLEVEL_INFO, DIOWEBSERVER_LOGSECTIONID_VERBOSE, false, _L("[%08X] Websocket Ping Received %d %s %s"), this, request.WebSocket_GetVersion(), request.WebSocket_GetProtocol()->Get(), request.GetResource()->Get());
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_CONNECTION::WebSocket_SendEvent_Disconnected()
* @brief      Web socket send event disconnected
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER_CONNECTION::WebSocket_SendEvent_Disconnected()
{
  if(!webserver) return false;
  if(!diostream) return false;

  DIOWEBSERVER_XEVENT xevent(webserver, DIOWEBSERVER_XEVENT_TYPE_WEBSOCKET_DISCONNECTED);
  xevent.SetServer(webserver);
  xevent.SetConnection(this);
  xevent.SetRequest(&request);
  xevent.GetReadDataWebSocket()->Empty();

  webserver->PostEvent(&xevent);

  GEN_XLOG.AddEntry(XLOGLEVEL_INFO, DIOWEBSERVER_LOGSECTIONID_VERBOSE, false, _L("[%08X] Websocket disconnected %d %s %s"), this, request.WebSocket_GetVersion(), request.WebSocket_GetProtocol()->Get(), request.GetResource()->Get());
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBSERVER_CONNECTION::ThreadRunFunction(void* param)
* @brief      Thread run function
* @ingroup    DATAIO
* 
* @param[in]  param : Param pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBSERVER_CONNECTION::ThreadRunFunction(void* param)
{
  DIOWEBSERVER_CONNECTION* wsconn = (DIOWEBSERVER_CONNECTION*)param;
  if(!wsconn) return;

  DIOWEBSERVER_WEBSOCKET_OPCODE opcode      = DIOWEBSERVER_WEBSOCKET_OPCODE_RESERVED10;
  bool                          sendrequest = false;

  // TLS server Open() includes accept + complete handshake and is intentionally synchronous.
  // Execute it here, in the per-connection worker, never in DIOWEBSERVER's accept/control thread.
  if(wsconn->isopening)
    {
      bool openstatus = wsconn->diostream?wsconn->diostream->Open():false;

      wsconn->isopening = false;

      if(!openstatus)
        {
          // The TCP accept() already succeeded by this point (Open() failed during/after the TLS handshake),
          // so the client IP is still available even though the connection never became usable. Logging this
          // is the only visibility the server has into port scans, malformed handshakes and failed handshake
          // attempts against the TLS listener; the success path below already logs symmetrically.
          if(wsconn->diostream)
            {
              XSTRING IPstring;
              wsconn->diostream->GetClientIP()->GetXString(IPstring);
              GEN_XLOG.AddEntry(XLOGLEVEL_WARNING, DIOWEBSERVER_LOGSECTIONID_VERBOSE, false, _L("[%08X] Connection handshake failed [%s] error=%d"), wsconn, IPstring.Get(), (int)wsconn->diostream->PeekLastDIOError());
            }

          wsconn->isactive = false;
          if(wsconn->threadconnection) wsconn->threadconnection->Run(false);
          return;
        }

      wsconn->isactive = true;
      if(wsconn->xtimerconnection) wsconn->xtimerconnection->Reset();

      if(wsconn->webserver)
        {
          XSTRING IPstring;
          wsconn->diostream->GetClientIP()->GetXString(IPstring);
          GEN_XLOG.AddEntry(XLOGLEVEL_INFO, DIOWEBSERVER_LOGSECTIONID_VERBOSE, false, _L("[%08X] Connection Active [%s]"), wsconn, IPstring.Get());
        }
    }

  switch(wsconn->GetMode())
    {
      case DIOWEBSERVER_CONNECTION_MODE_UNKNOWN       :
      case DIOWEBSERVER_CONNECTION_MODE_NORMAL        :
      case DIOWEBSERVER_CONNECTION_MODE_KEEPALIVE     : if(wsconn->ReadRequest())
                                                          {
                                                            wsconn->isrequestinprogress = true;

                                                            switch(wsconn->GetMode())
                                                              {
                                                                case DIOWEBSERVER_CONNECTION_MODE_UNKNOWN      :
                                                                case DIOWEBSERVER_CONNECTION_MODE_NORMAL       :
                                                                case DIOWEBSERVER_CONNECTION_MODE_KEEPALIVE    : sendrequest = wsconn->SendRequest();
                                                                                                                 if(sendrequest) wsconn->GetTimerDisconnection()->Reset();
                                                                                                                 wsconn->isrequestinprogress = false;
                                                                                                                 break;

                                                                case DIOWEBSERVER_CONNECTION_MODE_WEBSOCKET    : sendrequest = wsconn->WebSocket_SendEvent_Connected();
                                                                                                                 break;
                                                              }                                                          
                                                          }

                                                        wsconn->GetTimerDisconnection()->Reset();
                                                        break;

      case DIOWEBSERVER_CONNECTION_MODE_WEBSOCKET     : { XBUFFER data;
                                                          if(wsconn->WebSocket_ReadRequest(opcode, data))
                                                            {
                                                              switch(opcode)
                                                                {
                                                                  case DIOWEBSERVER_WEBSOCKET_OPCODE_CONTINUATION_FRAME   : break;

                                                                  case DIOWEBSERVER_WEBSOCKET_OPCODE_TEXT_FRAME           :
                                                                  case DIOWEBSERVER_WEBSOCKET_OPCODE_BINARY_FRAME         : wsconn->WebSocket_SendEvent_ReadData(data);
                                                                                                                            break;

                                                                  case DIOWEBSERVER_WEBSOCKET_OPCODE_CONNECTION_CLOSE     : break;

                                                                  case DIOWEBSERVER_WEBSOCKET_OPCODE_PING                 : wsconn->WebSocket_WritePingPong(false, data);
                                                                                                                            break;

                                                                  case DIOWEBSERVER_WEBSOCKET_OPCODE_PONG                 : wsconn->WebSocket_SendEvent_Pong(data);
                                                                                                                            break;

                                                                                                            default       : break;
                                                               }

                                                            }
                                                        }
                                                       break;
    }


  switch(wsconn->GetMode())
    {
      case DIOWEBSERVER_CONNECTION_MODE_UNKNOWN       :
      case DIOWEBSERVER_CONNECTION_MODE_NORMAL        : if((wsconn->xtimerconnection->GetMeasureSeconds() >= (XDWORD)wsconn->operativetimeouts.keepalive) || (sendrequest))
                                                          {
                                                            if(wsconn->GetTimerDisconnection()->GetMeasureSeconds() > 5) wsconn->Deactivate();
                                                          }
                                                        break;

      case DIOWEBSERVER_CONNECTION_MODE_KEEPALIVE     : if((wsconn->xtimerconnection->GetMeasureSeconds() >= (XDWORD)wsconn->operativetimeouts.keepalive) ||
                                                          (wsconn->nresourcesprocessed                    >= DIOWEBSERVER_KEEPALIVE_MAXRESOURCES))     wsconn->Deactivate();
                                                        break;

      case DIOWEBSERVER_CONNECTION_MODE_WEBSOCKET     : if(opcode == DIOWEBSERVER_WEBSOCKET_OPCODE_CONNECTION_CLOSE)
                                                          {
                                                            wsconn->Deactivate();
                                                            wsconn->WebSocket_SendEvent_Disconnected();
                                                          }
                                                        break;
    }


  return;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBSERVER_CONNECTION::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBSERVER_CONNECTION::Clean()
{
  websocketreferences.store(0, std::memory_order_relaxed);
  webserver             = NULL;
  diostreamcfg          = NULL;
  diostream             = NULL;

  isactive              = false;
  isopening             = false;
  mode                  =  DIOWEBSERVER_CONNECTION_MODE_UNKNOWN;
  xtimerconnection      = NULL;
  xtimerdisconnection   = NULL;
  nresourcesprocessed   = 0;
  isrequestinprogress   = false;

  threadconnection      = NULL;
  actualencodingflag    = DIOWEBSERVER_ENCODINGFLAGS_NONE;

  origin.Empty();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBSERVER::DIOWEBSERVER()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBSERVER::DIOWEBSERVER()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBSERVER::~DIOWEBSERVER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBSERVER::~DIOWEBSERVER()
{
  End();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER::Ini(int port, bool doinitialconnectitivitytest, int timeoutserverpage, XSTRING* addrlocal)
* @brief      Initialize the object
* @ingroup    DATAIO
* 
* @param[in]  port : Port number to use.
* @param[in]  doinitialconnectitivitytest : Doinitialconnectitivitytest value.
* @param[in]  timeoutserverpage : Timeoutserverpage value.
* @param[in]  addrlocal : Addrlocal pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER::Ini(int port, bool doinitialconnectitivitytest, int timeoutserverpage, XSTRING* addrlocal)
{
  DIOSTREAMTCPIPCONFIG* newcfg = GEN_NEW DIOSTREAMTCPIPCONFIG();
  if(!newcfg) return false;

  // On failure newcfg is left assigned to diostreamcfg (matches the original single-function behaviour): the
  // caller is expected to destroy this DIOWEBSERVER, whose destructor/End() frees diostreamcfg either way.
  return Ini_Internal(newcfg, port, doinitialconnectitivitytest, timeoutserverpage, addrlocal);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOWEBSERVER::Ini(DIOSTREAMTLSCONFIG* tlsconfig, int port, int timeoutserverpage, XSTRING* addrlocal)
* @brief      Initialize the object as an HTTPS listener
* @note       See the declaration in DIOWebServer.h for the ownership and ordering requirements on tlsconfig.
* @ingroup    DATAIO
*
* @param[in]  tlsconfig : TLS server configuration, already carrying local certificate/private key and offered
*             cipher suites / groups / signature schemes / ALPN protocols. Ownership transfers to this object.
* @param[in]  port : Port number to use.
* @param[in]  timeoutserverpage : Timeoutserverpage value.
* @param[in]  addrlocal : Addrlocal pointer to use.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER::Ini(DIOSTREAMTLSCONFIG* tlsconfig, int port, int timeoutserverpage, XSTRING* addrlocal)
{
  if(!tlsconfig) return false;

  if(!tlsconfig->LocalCredentials_Validate())
    {
      GEN_DELETE tlsconfig;
      return false;
    }

  if((tlsconfig->GetClientAuthenticationMode() != DIOSTREAMTLS_CLIENTAUTHENTICATION_MODE_NONE) &&
     (!tlsconfig->GetClientTrustedRoots() || tlsconfig->GetClientTrustedRoots()->IsEmpty()))
    {
      GEN_DELETE tlsconfig;
      return false;
    }

  // The current GEN TLS server implementation is TLS 1.3 only.  Validate this here, before the listener is
  // started, so a TLS 1.2 server configuration fails at Ini() instead of being silently ignored until a client
  // reaches DIOSTREAMTLS::Open().
  if((tlsconfig->GetMinVersion() != DIOSTREAMTLS_MSG_VERSION_TLS_1_3) ||
     (tlsconfig->GetMaxVersion() != DIOSTREAMTLS_MSG_VERSION_TLS_1_3))
    {
      GEN_DELETE tlsconfig;
      return false;
    }

  // Session tickets are sealed with a server-wide key stored in the shared TLS configuration. Initialize it before
  // connection workers can start so all handshakes use the same immutable key and no two workers can race while
  // creating it. Resumption remains disabled automatically when client authentication (mTLS) is enabled.
  if(tlsconfig->IsSessionResumptionActive() &&
     !tlsconfig->SessionResumption_ServerInitialize())
    {
      GEN_DELETE tlsconfig;
      return false;
    }

  // doinitialconnectitivitytest is forced off here: it assumes Open() on the probe stream succeeds without a
  // peer (plain TCP bind+listen only), whereas DIOSTREAMTLS<T>'s server Open() accepts AND completes a full
  // handshake in that single call, which needs a real client -- reusing the plain-TCP self-test would make
  // this Ini() fail every time at startup, before any client has had a chance to connect.
  return Ini_Internal(tlsconfig, port, false, timeoutserverpage, addrlocal);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOWEBSERVER::Ini_Internal(DIOSTREAMTCPIPCONFIG* newcfg, int port, bool doinitialconnectitivitytest, int timeoutserverpage, XSTRING* addrlocal)
* @brief      Common Ini() body shared by the plain-TCP and TLS entry points
* @note       INTERNAL. Takes ownership of newcfg (plain DIOSTREAMTCPIPCONFIG or a DIOSTREAMTLSCONFIG passed as
*             one) on both success and failure -- the caller must not delete it itself either way.
* @ingroup    DATAIO
*
* @param[in]  newcfg : Stream configuration to adopt as diostreamcfg.
* @param[in]  port : Port number to use.
* @param[in]  doinitialconnectitivitytest : Doinitialconnectitivitytest value.
* @param[in]  timeoutserverpage : Timeoutserverpage value.
* @param[in]  addrlocal : Addrlocal pointer to use.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER::Ini_Internal(DIOSTREAMTCPIPCONFIG* newcfg, int port, bool doinitialconnectitivitytest, int timeoutserverpage, XSTRING* addrlocal)
{
  isactive        = false;
  doexit          = false;

  diostreamcfg = newcfg;

  if(diostreamcfg && diostreamcfg->IsTLS())
    {
      DIOSTREAMTLSCONFIG* tlscfg = dynamic_cast<DIOSTREAMTLSCONFIG*>(diostreamcfg);
      if(!tlscfg || !tlscfg->SetConnectionTimeout(timeouts.connectaccept) ||
                    !tlscfg->SetHandshakeTimeout(timeouts.tlshandshake) ||
                    !tlscfg->Freeze()) return false;
    }

  this->port                      = port;
  this->timeoutserverpage         = timeoutserverpage;
  if(addrlocal) this->addrlocal   = (*addrlocal);

  AddPlayablePageExtension(_L(".HTML"));
  AddPlayablePageExtension(_L(".HTM"));

  RegisterEvent(DIOWEBSERVER_XEVENT_TYPE_REQUEST);
  RegisterEvent(DIOWEBSERVER_XEVENT_TYPE_WEBSOCKET_CONNECTED);
  RegisterEvent(DIOWEBSERVER_XEVENT_TYPE_WEBSOCKET_READDATA);
  RegisterEvent(DIOWEBSERVER_XEVENT_TYPE_WEBSOCKET_PONG);
  RegisterEvent(DIOWEBSERVER_XEVENT_TYPE_WEBSOCKET_DISCONNECTED);

  xtimercontrol=GEN_XFACTORY.CreateTimer();
  if(!xtimercontrol) return false;
  
  diostreamcfg->SetMode(DIOSTREAMMODE_SERVER);
  diostreamcfg->GetLocalIP()->Set(this->addrlocal);
  diostreamcfg->SetRemotePort(port);

  isactive = true;

  xmutexconnections=GEN_XFACTORY.Create_Mutex();
  if(!xmutexconnections) return false;

  bool status = true;

  // ------------------------------------------------------------------------------------
  // TEST Can open socket for WEB server

  if(doinitialconnectitivitytest)
    {
      xtimercontrol->Reset();

      DIOSTREAMTCPIP* diostream = (DIOSTREAMTCPIP*)GEN_DIOFACTORY.CreateStreamIO(diostreamcfg);
      if(diostream)
        {
          if(diostream->Open())
            {                    
              while(1)
                {
                  if(diostream->GetStatus() == DIOSTREAMSTATUS_DISCONNECTED)
                    {
                      status = false;
                      break;
                    }

                  if(xtimercontrol->GetMeasureSeconds() > 3) break;
                }

              diostream->Close();
            
          
            } else status = false;

          GEN_DIOFACTORY.DeleteStreamIO(diostream);
        }

      if(!status) return false;
    }

  // ------------------------------------------------------------------------------------

  xtimercontrol->Reset();

  threadserver = CREATEXTHREAD(XTHREADGROUPID_DIOWEBSERVER, _L("DIOWEBSERVER::Ini"), ThreadRunFunction, (void*)this);
  if(!threadserver) return false;

  if(!threadserver->Ini()) return false;

  if(addrlocal) GEN_XLOG.AddEntry(XLOGLEVEL_INFO, DIOWEBSERVER_LOGSECTIONID_VERBOSE, false, _L("Web Server [%08X]: %s in %s:%d "), this, status?_L("Active"):_L("Disable"), addrlocal->IsEmpty()?_L("Default"):addrlocal->Get(), port);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOWEBSERVER::GetPort()
* @brief      Get port
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOWEBSERVER::GetPort()
{
  return port;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOWEBSERVER::GetTimeoutServerPage()
* @brief      Get timeout server page
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOWEBSERVER::GetTimeoutServerPage()
{
  return timeoutserverpage;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD DIOWEBSERVER::GetMaxPageConnections()
* @brief      Get the maximum number of connections that may be simultaneously waiting for a page (handshake /
*             request in progress). Also acts as an anti-slowloris limit.
* @ingroup    DATAIO
*
* @return     XDWORD : Requested value.
*
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOWEBSERVER::GetMaxPageConnections()
{
  return maxpageconnections;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOWEBSERVER::SetMaxPageConnections(XDWORD maxpageconnections)
* @brief      Set the maximum number of connections that may be simultaneously waiting for a page, so a deployment
*             can size the anti-slowloris limit to its own capacity instead of the DIOWEBSERVER_MAXPAGECONNECTIONS
*             compile-time default.
* @ingroup    DATAIO
*
* @param[in]  maxpageconnections : new limit to apply. Must be greater than zero.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER::SetMaxPageConnections(XDWORD maxpageconnections)
{
  if(!maxpageconnections) return false;

  this->maxpageconnections = maxpageconnections;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBSERVER_TIMEOUTS* DIOWEBSERVER::GetTimeouts()
* @brief      Get timeouts
* @ingroup    DATAIO
* 
* @return     DIOWEBSERVER_TIMEOUTS* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBSERVER_TIMEOUTS* DIOWEBSERVER::GetTimeouts()
{
  return &timeouts;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER::SetTimeouts(DIOWEBSERVER_TIMEOUTS& timeouts)
* @brief      Set timeouts
* @ingroup    DATAIO
* 
* @param[out] timeouts : Timeouts value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER::SetTimeouts(DIOWEBSERVER_TIMEOUTS& timeouts)
{
  if(isactive) return false;
  if(timeouts.connectaccept <= 0 || timeouts.tlshandshake <= 0 || timeouts.httpheaders <= 0 ||
     timeouts.httpbody <= 0 || timeouts.keepalive <= 0) return false;

  this->timeouts = timeouts;

  if(diostreamcfg && diostreamcfg->IsTLS())
    {
      DIOSTREAMTLSCONFIG* tlscfg = dynamic_cast<DIOSTREAMTLSCONFIG*>(diostreamcfg);
      if(tlscfg)
        {
          tlscfg->SetConnectionTimeout(timeouts.connectaccept);
          tlscfg->SetHandshakeTimeout(timeouts.tlshandshake);
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBSERVER_LIMITS* DIOWEBSERVER::GetLimits()
* @brief      Get limits
* @ingroup    DATAIO
* 
* @return     DIOWEBSERVER_LIMITS* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBSERVER_LIMITS* DIOWEBSERVER::GetLimits()
{
  return &limits;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER::SetLimits(DIOWEBSERVER_LIMITS& limits)
* @brief      Set limits
* @ingroup    DATAIO
* 
* @param[in]  limits : Limits value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER::SetLimits(DIOWEBSERVER_LIMITS& limits)
{
  if(isactive || !limits.maximumheadersize || !limits.maximumbodysize ||
     limits.maximumheadersize > DIOSTREAM_MAXBUFFER || limits.maximumbodysize > 0x7FFFFFFF) return false;

  this->limits = limits;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTCPIPCONFIG* DIOWEBSERVER::GetDIOStreamCFG()
* @brief      Get DIO stream CFG
* @ingroup    DATAIO
* 
* @return     DIOSTREAMTCPIPCONFIG* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTCPIPCONFIG* DIOWEBSERVER::GetDIOStreamCFG()
{
  return diostreamcfg;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER::IsSendingAnyPage()
* @brief      Is sending any page
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER::IsSendingAnyPage()
{
  bool sending = false;

  if(!connections.IsEmpty())
    {
      for(XDWORD c=0;c<connections.GetSize();c++)
        {
          DIOWEBSERVER_CONNECTION* connection = (DIOWEBSERVER_CONNECTION*)connections.Get(c);
          if(connection)
            {
              if(connection->IsRequestInProgress())
                {
                  sending = true;
                  break;
                }
            }
        }
    }

  return sending;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER::UnSubscribeAllPages()
* @brief      Un subscribe all pages
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER::UnSubscribeAllPages()
{
  if(!connections.IsEmpty())
    {
      for(XDWORD c=0;c<connections.GetSize();c++)
        {
          DIOWEBSERVER_CONNECTION* connection = (DIOWEBSERVER_CONNECTION*)connections.Get(c);
          if(connection)
            {
              UnSubscribeEvent(DIOSTREAM_XEVENT_TYPE_CONNECTED, connection->GetDIOStream());
              UnSubscribeEvent(DIOSTREAM_XEVENT_TYPE_DISCONNECTED , connection->GetDIOStream());
            }
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER::AddAuthentication(XSTRING& guest, XSTRING& login, XSTRING& password)
* @brief      Add authentication
* @ingroup    DATAIO
* 
* @param[in]  guest : Guest value.
* @param[in]  login : Login value.
* @param[in]  password : Password text.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER::AddAuthentication(XSTRING& guest, XSTRING& login, XSTRING& password)
{
  DIOWEBSERVER_AUTHENTICATION* authentication = GetAuthentication(guest);
  if(authentication)
    {
      authentication->Set(login, password);
    }
   else
    {
      authentication = GEN_NEW DIOWEBSERVER_AUTHENTICATION();
      if(!authentication) return false;

      authentication->Set(guest, login, password);

      authentications.Add(authentication);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBSERVER_AUTHENTICATION* DIOWEBSERVER::GetAuthentication(XSTRING& guest)
* @brief      Get authentication
* @ingroup    DATAIO
* 
* @param[in]  guest : Guest value.
* 
* @return     DIOWEBSERVER_AUTHENTICATION* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBSERVER_AUTHENTICATION* DIOWEBSERVER::GetAuthentication(XSTRING& guest)
{
  if(authentications.IsEmpty()) return NULL;

  DIOWEBSERVER_AUTHENTICATION* authentication = NULL;

  for(int c=0;c<(int)authentications.GetSize();c++)
    {
      DIOWEBSERVER_AUTHENTICATION* auth = (DIOWEBSERVER_AUTHENTICATION*)authentications.Get(c);
      if(auth)
        {
          if(auth->GetGuest())
            {
              XSTRING* tguest = auth->GetGuest();
              if(tguest)
                {
                  if((*tguest) == guest)
                    {
                      authentication = auth;
                      break;
                    }
                }
            }
        }
    }

  return authentication;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER::HaveAuthentications()
* @brief      Have authentications configured
* @ingroup    DATAIO
* 
* @return     bool : true if there is at least one authentication registered; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER::HaveAuthentications()
{
  return authentications.IsEmpty()?false:true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER::CheckAuthentication(XSTRING& guest, XSTRING& login, XSTRING& password)
* @brief      Check the login/password received against the registered authentications.
*             The guest (Host of the request) is used first, if it is not found the wildcard guest is used.
*             An empty registered login means that any login is valid (only the password is checked).
* @ingroup    DATAIO
* 
* @param[in]  guest : Guest (host) value.
* @param[in]  login : Login value.
* @param[in]  password : Password text.
* 
* @return     bool : true if the credentials are valid; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER::CheckAuthentication(XSTRING& guest, XSTRING& login, XSTRING& password)
{
  DIOWEBSERVER_AUTHENTICATION* authentication = GetAuthentication(guest);

  if(!authentication)
    {
      // The Host header can come as "host:port", try only with the host part.
      int index = guest.FindCharacter(_C(':'));
      if(index != XSTRING_NOTFOUND)
        {
          XSTRING onlyguest;

          guest.Copy(0, index, onlyguest);
          authentication = GetAuthentication(onlyguest);
        }
    }

  if(!authentication)
    {
      XSTRING allguests;

      allguests = DIOWEBSERVER_AUTHENTICATION_ALLGUESTS;
      authentication = GetAuthentication(allguests);
    }

  if(!authentication) return false;

  if(!authentication->GetLogin()->IsEmpty())
    {
      if(authentication->GetLogin()->Compare(login, true)) return false;
    }

  if(authentication->GetPassword()->Compare(password, false)) return false;   // The password is case sensitive.

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER::DelAuthentication(XSTRING& guest)
* @brief      Del authentication
* @ingroup    DATAIO
* 
* @param[in]  guest : Guest value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER::DelAuthentication(XSTRING& guest)
{
  DIOWEBSERVER_AUTHENTICATION* authentication = GetAuthentication(guest);
  if(!authentication) return false;

  authentications.Delete(authentication);

  GEN_DELETE authentication;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER::DelAllAuthentication()
* @brief      Del all authentication
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER::DelAllAuthentication()
{
  if(authentications.IsEmpty()) return false;

  authentications.DeleteContents();

  authentications.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER::IsPlayablePage(XCHAR* extension)
* @brief      Is playable page
* @ingroup    DATAIO
* 
* @param[in]  extension : Extension pointer to use.
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER::IsPlayablePage(XCHAR* extension)
{
  if(playablepageextensions.IsEmpty()) return false;

  for(int c=0;c<(int)playablepageextensions.GetSize();c++)
    {
      XSTRING* _extension = (XSTRING*)playablepageextensions.Get(c);
      if(_extension)
        {
          if(!_extension->Compare(extension, true)) return true;
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER::IsPlayablePage(XSTRING& extension)
* @brief      Is playable page
* @ingroup    DATAIO
* 
* @param[in]  extension : Extension value.
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER::IsPlayablePage(XSTRING& extension)
{
  return IsPlayablePage(extension.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER::AddPlayablePageExtension(XCHAR* extension)
* @brief      Add playable page extension
* @ingroup    DATAIO
* 
* @param[in]  extension : Extension pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER::AddPlayablePageExtension(XCHAR* extension)
{
  if(!extension) return false;

  XSTRING* _string = GEN_NEW XSTRING(extension);
  if(!_string) return false;

  playablepageextensions.Add(_string);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER::AddPlayablePageExtension(XSTRING& extension)
* @brief      Add playable page extension
* @ingroup    DATAIO
* 
* @param[in]  extension : Extension value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER::AddPlayablePageExtension(XSTRING& extension)
{
  XSTRING* _string = GEN_NEW XSTRING(extension);
  if(!_string) return false;

  playablepageextensions.Add(_string);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER::DeleteAllPlayablePageExtensions()
* @brief      Delete all playable page extensions
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER::DeleteAllPlayablePageExtensions()
{
  if(playablepageextensions.IsEmpty()) return false;

  playablepageextensions.DeleteContents();
  playablepageextensions.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBSERVER::AddIncludedEncodingFlag(DIOWEBSERVER_ENCODINGFLAGS flag)
* @brief      Add included encoding flag
* @ingroup    DATAIO
* 
* @param[in]  flag : Flag value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBSERVER::AddIncludedEncodingFlag(DIOWEBSERVER_ENCODINGFLAGS flag)
{
  includedencodingflags |= flag;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER::IsIncludedEncodingFlag(DIOWEBSERVER_ENCODINGFLAGS flag)
* @brief      Is included encoding flag
* @ingroup    DATAIO
* 
* @param[in]  flag : Flag value.
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER::IsIncludedEncodingFlag(DIOWEBSERVER_ENCODINGFLAGS flag)
{
  return ((includedencodingflags&flag) != 0);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XMUTEX* DIOWEBSERVER::GetMutextConnections()
* @brief      Get mutext connections
* @ingroup    DATAIO
* 
* @return     XMUTEX* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XMUTEX* DIOWEBSERVER::GetMutextConnections()
{
  return xmutexconnections;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<DIOWEBSERVER_CONNECTION*>* DIOWEBSERVER::GetConnections()
* @brief      Get connections
* @ingroup    DATAIO
* 
* @return     XVECTOR<DIOWEBSERVER_CONNECTION*>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<DIOWEBSERVER_CONNECTION*>* DIOWEBSERVER::GetConnections()
{
  return &connections;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBSERVER_CONNECTION* DIOWEBSERVER::Websocket_GetConnection()
* @brief      Websocket get connection
* @ingroup    DATAIO
* 
* @return     DIOWEBSERVER_CONNECTION* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBSERVER_CONNECTION* DIOWEBSERVER::Websocket_GetConnection()
{
  if(connections.IsEmpty()) return NULL;

  websocket_search_indexconnection = 0;
  websocket_search_protocol.Empty();
  websocket_search_version         = 0;
  websocket_search_resource.Empty();

  return Websocket_GetNextConnection();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBSERVER_CONNECTION* DIOWEBSERVER::Websocket_GetConnection(XCHAR* protocol)
* @brief      Websocket get connection
* @ingroup    DATAIO
* 
* @param[in]  protocol : Protocol pointer to use.
* 
* @return     DIOWEBSERVER_CONNECTION* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBSERVER_CONNECTION* DIOWEBSERVER::Websocket_GetConnection(XCHAR* protocol)
{
  if(connections.IsEmpty()) return NULL;

  websocket_search_indexconnection = 0;
  websocket_search_protocol        = protocol;
  websocket_search_version         = 0;
  websocket_search_resource.Empty();

  return Websocket_GetNextConnection();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBSERVER_CONNECTION* DIOWEBSERVER::Websocket_GetConnection(XCHAR* protocol, int version)
* @brief      Websocket get connection
* @ingroup    DATAIO
* 
* @param[in]  protocol : Protocol pointer to use.
* @param[in]  version : Version value.
* 
* @return     DIOWEBSERVER_CONNECTION* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBSERVER_CONNECTION* DIOWEBSERVER::Websocket_GetConnection(XCHAR* protocol, int version)
{
  if(connections.IsEmpty()) return NULL;
 
  websocket_search_indexconnection = 0;
  websocket_search_protocol        = protocol;
  websocket_search_version         = version;
  websocket_search_resource.Empty();

  return Websocket_GetNextConnection();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBSERVER_CONNECTION* DIOWEBSERVER::Websocket_GetConnection(XCHAR* protocol, int version, XCHAR* resource)
* @brief      Websocket get connection
* @ingroup    DATAIO
* 
* @param[in]  protocol : Protocol pointer to use.
* @param[in]  version : Version value.
* @param[in]  resource : Resource pointer to use.
* 
* @return     DIOWEBSERVER_CONNECTION* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBSERVER_CONNECTION* DIOWEBSERVER::Websocket_GetConnection(XCHAR* protocol, int version, XCHAR* resource)
{
  if(connections.IsEmpty()) return NULL;

  websocket_search_indexconnection = 0;
  websocket_search_protocol        = protocol;
  websocket_search_version         = version;
  websocket_search_resource        = resource;

  return Websocket_GetNextConnection();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBSERVER_CONNECTION* DIOWEBSERVER::Websocket_GetNextConnection()
* @brief      Websocket get connection
* @ingroup    DATAIO
* 
* @return     DIOWEBSERVER_CONNECTION* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBSERVER_CONNECTION* DIOWEBSERVER::Websocket_GetNextConnection()
{
  if(connections.IsEmpty()) return NULL;

  if(websocket_search_indexconnection == -1) return NULL;

  DIOWEBSERVER_CONNECTION* connection    = NULL;
  XBYTE                    filter_mask   = 0x00;
  XBYTE                    filter_status = 0x00;

  if(websocket_search_protocol.GetSize())  filter_mask  |= 0x01;
  if(websocket_search_version)             filter_mask  |= 0x02;
  if(websocket_search_resource.GetSize())  filter_mask  |= 0x04;

  if(xmutexconnections) xmutexconnections->Lock();

  for( ; websocket_search_indexconnection<(int)connections.GetSize(); websocket_search_indexconnection++)
    {
      connection = connections.Get(websocket_search_indexconnection);
      if(connection)
        {
          if(connection->GetMode() == DIOWEBSERVER_CONNECTION_MODE_WEBSOCKET)
            {
              if(!filter_mask) break;

              filter_status = 0x00;
              if(!connection->GetRequest()->WebSocket_GetProtocol()->Compare(websocket_search_protocol))   filter_status |= 0x01;
              if(connection->GetRequest()->WebSocket_GetVersion()  == websocket_search_version)            filter_status |= 0x02;
              if(!connection->GetRequest()->GetResource()->Compare(websocket_search_resource))             filter_status |= 0x04;

              if(filter_mask == filter_status) break;

            } else connection = NULL;
        }
    }

  if(xmutexconnections) xmutexconnections->UnLock();

  if(!connection)
         websocket_search_indexconnection = -1;
    else websocket_search_indexconnection++;

  return connection;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER::Websocket_Write(XCHAR* string, int timeout, XCHAR* protocol, int version, XCHAR* resource)
* @brief      Websocket write
* @ingroup    DATAIO
* 
* @param[in]  string : String to use.
* @param[in]  timeout : Timeout value.
* @param[in]  protocol : Protocol pointer to use.
* @param[in]  version : Version value.
* @param[in]  resource : Resource pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER::Websocket_Write(XCHAR* string, int timeout, XCHAR* protocol, int version, XCHAR* resource)
{
  // NOTE: this does NOT use Websocket_GetConnection()/Websocket_GetNextConnection() -- their search position is
  // kept in shared DIOWEBSERVER members (websocket_search_indexconnection, ...), which is not safe when more than
  // one WebSocket connection can call into this at the same time (each connection processes its own incoming
  // frames on its own thread): two concurrent calls stomp on the same shared search state, and a connection can be
  // silently skipped (its event never gets a reply). The whole search + write is done here under a single lock,
  // using only local state, so concurrent calls from different connections cannot interfere with each other.

  XSTRING _protocol;
  XSTRING _resource;
  XBYTE   filter_mask = 0x00;
  bool    status      = true;

  _protocol = protocol;
  _resource = resource;

  if(_protocol.GetSize()) filter_mask |= 0x01;
  if(version)              filter_mask |= 0x02;
  if(_resource.GetSize()) filter_mask |= 0x04;

  if(xmutexconnections) xmutexconnections->Lock();
  XVECTOR<DIOWEBSERVER_CONNECTION*> targets;
  for(int c = 0; c<(int)connections.GetSize(); c++)
    {
      DIOWEBSERVER_CONNECTION* connection = connections.Get(c);
      if(!connection)                                                     continue;
      if(connection->GetMode() != DIOWEBSERVER_CONNECTION_MODE_WEBSOCKET) continue;

      if(filter_mask)
        {
          XBYTE filter_status = 0x00;

          if(!connection->GetRequest()->WebSocket_GetProtocol()->Compare(_protocol))  filter_status |= 0x01;
          if(connection->GetRequest()->WebSocket_GetVersion()  == version)            filter_status |= 0x02;
          if(!connection->GetRequest()->GetResource()->Compare(_resource))            filter_status |= 0x04;

          if(filter_mask != filter_status) continue;
        }

      if(connection->TryAcquireReference()) targets.Add(connection);
    }
  if(xmutexconnections) xmutexconnections->UnLock();
  for(XDWORD c=0;c<targets.GetSize();c++) { if(!targets.Get(c)->WebSocket_Write(string, timeout)) status=false; targets.Get(c)->ReleaseReference(); }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER::Websocket_Write(XSTRING& string, int timeout, XCHAR* protocol, int version, XCHAR* resource)
* @brief      Websocket write
* @ingroup    DATAIO
* 
* @param[in]  string : String to use.
* @param[in]  timeout : Timeout value.
* @param[in]  protocol : Protocol pointer to use.
* @param[in]  version : Version value.
* @param[in]  resource : Resource pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER::Websocket_Write(XSTRING& string, int timeout, XCHAR* protocol, int version, XCHAR* resource)
{
  return Websocket_Write(string.Get(), timeout, protocol, version, resource);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER::Websocket_Write(XBUFFER& data, int timeout, XCHAR* protocol, int version, XCHAR* resource)
* @brief      Websocket write
* @ingroup    DATAIO
* 
* @param[in]  data : Data buffer to use.
* @param[in]  timeout : Timeout value.
* @param[in]  protocol : Protocol pointer to use.
* @param[in]  version : Version value.
* @param[in]  resource : Resource pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER::Websocket_Write(XBUFFER& data, int timeout, XCHAR* protocol, int version, XCHAR* resource)
{
  // See DIOWEBSERVER::Websocket_Write(XCHAR*, ...) above: same self-contained, mutex-protected search + write,
  // instead of the shared-state Websocket_GetConnection()/Websocket_GetNextConnection() pair, which is not safe
  // to call concurrently from more than one WebSocket connection's thread.

  XSTRING _protocol;
  XSTRING _resource;
  XBYTE   filter_mask = 0x00;
  bool    status      = true;

  _protocol = protocol;
  _resource = resource;

  if(_protocol.GetSize()) filter_mask |= 0x01;
  if(version)              filter_mask |= 0x02;
  if(_resource.GetSize()) filter_mask |= 0x04;

  if(xmutexconnections) xmutexconnections->Lock();
  XVECTOR<DIOWEBSERVER_CONNECTION*> targets;
  for(int c = 0; c<(int)connections.GetSize(); c++)
    {
      DIOWEBSERVER_CONNECTION* connection = connections.Get(c);
      if(!connection)                                                     continue;
      if(connection->GetMode() != DIOWEBSERVER_CONNECTION_MODE_WEBSOCKET) continue;

      if(filter_mask)
        {
          XBYTE filter_status = 0x00;

          if(!connection->GetRequest()->WebSocket_GetProtocol()->Compare(_protocol))  filter_status |= 0x01;
          if(connection->GetRequest()->WebSocket_GetVersion()  == version)            filter_status |= 0x02;
          if(!connection->GetRequest()->GetResource()->Compare(_resource))            filter_status |= 0x04;

          if(filter_mask != filter_status) continue;
        }

      if(connection->TryAcquireReference()) targets.Add(connection);
    }
  if(xmutexconnections) xmutexconnections->UnLock();
  for(XDWORD c=0;c<targets.GetSize();c++) { if(!targets.Get(c)->WebSocket_Write(data, timeout)) status=false; targets.Get(c)->ReleaseReference(); }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER::Websocket_Ping(XCHAR* string, int timeout, XCHAR* protocol, int version, XCHAR* resource)
* @brief      Websocket ping
* @ingroup    DATAIO
* 
* @param[in]  string : String to use.
* @param[in]  timeout : Timeout value.
* @param[in]  protocol : Protocol pointer to use.
* @param[in]  version : Version value.
* @param[in]  resource : Resource pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER::Websocket_Ping(XCHAR* string, int timeout, XCHAR* protocol, int version, XCHAR* resource)
{
  // See DIOWEBSERVER::Websocket_Write(XCHAR*, ...) above: same self-contained, mutex-protected search + write,
  // instead of the shared-state Websocket_GetConnection()/Websocket_GetNextConnection() pair, which is not safe
  // to call concurrently from more than one WebSocket connection's thread.

  XSTRING _protocol;
  XSTRING _resource;
  XBYTE   filter_mask = 0x00;
  bool    status      = true;

  _protocol = protocol;
  _resource = resource;

  if(_protocol.GetSize()) filter_mask |= 0x01;
  if(version)              filter_mask |= 0x02;
  if(_resource.GetSize()) filter_mask |= 0x04;

  if(xmutexconnections) xmutexconnections->Lock();
  XVECTOR<DIOWEBSERVER_CONNECTION*> targets;
  for(int c = 0; c<(int)connections.GetSize(); c++)
    {
      DIOWEBSERVER_CONNECTION* connection = connections.Get(c);
      if(!connection)                                                     continue;
      if(connection->GetMode() != DIOWEBSERVER_CONNECTION_MODE_WEBSOCKET) continue;

      if(filter_mask)
        {
          XBYTE filter_status = 0x00;

          if(!connection->GetRequest()->WebSocket_GetProtocol()->Compare(_protocol))  filter_status |= 0x01;
          if(connection->GetRequest()->WebSocket_GetVersion()  == version)            filter_status |= 0x02;
          if(!connection->GetRequest()->GetResource()->Compare(_resource))            filter_status |= 0x04;

          if(filter_mask != filter_status) continue;
        }

      if(connection->TryAcquireReference()) targets.Add(connection);
    }
  if(xmutexconnections) xmutexconnections->UnLock();
  for(XDWORD c=0;c<targets.GetSize();c++) { if(!targets.Get(c)->WebSocket_WritePingPong(true, string, timeout)) status=false; targets.Get(c)->ReleaseReference(); }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER::Websocket_Ping(XSTRING& string, int timeout, XCHAR* protocol, int version, XCHAR* resource)
* @brief      Websocket ping
* @ingroup    DATAIO
* 
* @param[in]  string : String to use.
* @param[in]  timeout : Timeout value.
* @param[in]  protocol : Protocol pointer to use.
* @param[in]  version : Version value.
* @param[in]  resource : Resource pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER::Websocket_Ping(XSTRING& string, int timeout, XCHAR* protocol, int version, XCHAR* resource)
{
  return Websocket_Ping(string.Get(), timeout, protocol, version, resource);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBSERVER_PLUGINSMANAGER* DIOWEBSERVER::GetPluginsManager()
* @brief      Get plugins manager
* @ingroup    DATAIO
* 
* @return     DIOWEBSERVER_PLUGINSMANAGER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBSERVER_PLUGINSMANAGER* DIOWEBSERVER::GetPluginsManager()
{
  return &pluginsmanager;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER::IsExit()
* @brief      Is exit
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER::IsExit()
{
  return doexit;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER::End()
* @brief      End the object
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER::End()
{
  doexit = true;

  GEN_XLOG.AddEntry(XLOGLEVEL_INFO, DIOWEBSERVER_LOGSECTIONID_VERBOSE, false, _L("Disable web server [%08X]."), this);

  UnSubscribeAllPages();

  GEN_XLOG.AddEntry(XLOGLEVEL_INFO, DIOWEBSERVER_LOGSECTIONID_VERBOSE, false , _L("Delete main thread ... "));

  if(threadserver)
    {
      threadserver->End();
      DELETEXTHREAD(XTHREADGROUPID_DIOWEBSERVER, threadserver);
      threadserver = NULL;
    }

  GEN_XLOG.AddEntry(XLOGLEVEL_INFO, DIOWEBSERVER_LOGSECTIONID_VERBOSE, false, _L("Wait to send all pages ..."));
  WaitToSentAllPages();

  GEN_XLOG.AddEntry(XLOGLEVEL_INFO, DIOWEBSERVER_LOGSECTIONID_VERBOSE, false, _L("Delete connections not use ..."));
  Connections_DeleteUsed();

  GEN_XLOG.AddEntry(XLOGLEVEL_INFO, DIOWEBSERVER_LOGSECTIONID_VERBOSE, false, _L("Delete connections waiting ..."));
  Connections_DeleteWaiting();

  GEN_XLOG.AddEntry(XLOGLEVEL_INFO, DIOWEBSERVER_LOGSECTIONID_VERBOSE, false, _L("Delete all connections ... "));
  Connections_DeleteAll();

  if(xmutexconnections)
    {
      GEN_XFACTORY.Delete_Mutex(xmutexconnections);
      xmutexconnections = NULL;
    }

  GEN_XLOG.AddEntry(XLOGLEVEL_INFO, DIOWEBSERVER_LOGSECTIONID_VERBOSE, false, _L("Delete resources ... "));

  if(xtimercontrol)
    {
      GEN_XFACTORY.DeleteTimer(xtimercontrol);
      xtimercontrol = NULL;
    }

  DeleteAllPlayablePageExtensions();

  DelAllAuthentication();

  GEN_XLOG.AddEntry(XLOGLEVEL_INFO, DIOWEBSERVER_LOGSECTIONID_VERBOSE, false, _L("Delete events ... "));

  DeRegisterEvent(DIOWEBSERVER_XEVENT_TYPE_REQUEST);
  DeRegisterEvent(DIOWEBSERVER_XEVENT_TYPE_WEBSOCKET_CONNECTED);
  DeRegisterEvent(DIOWEBSERVER_XEVENT_TYPE_WEBSOCKET_READDATA);
  DeRegisterEvent(DIOWEBSERVER_XEVENT_TYPE_WEBSOCKET_PONG);
  DeRegisterEvent(DIOWEBSERVER_XEVENT_TYPE_WEBSOCKET_DISCONNECTED);

  GEN_XLOG.AddEntry(XLOGLEVEL_INFO, DIOWEBSERVER_LOGSECTIONID_VERBOSE, false, _L("Delete stream config ... "));

  if(diostreamcfg)
    {
      GEN_DELETE diostreamcfg;
      diostreamcfg = NULL;
    }

  GEN_XLOG.AddEntry(XLOGLEVEL_INFO, DIOWEBSERVER_LOGSECTIONID_VERBOSE, false, _L("End web server [%08X]."), this);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER::WaitToSentAllPages()
* @brief      Wait to sent all pages
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER::WaitToSentAllPages()
{
  if(!connections.IsEmpty())
    {
      if(xtimercontrol)
        {
          do{ GEN_XSLEEP.MilliSeconds(10);

            } while(IsSendingAnyPage());
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOWEBSERVER::Connections_GetNWaiting()
* @brief      Connections get N waiting
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOWEBSERVER::Connections_GetNWaiting()
{
  if(xmutexconnections) xmutexconnections->Lock();

  int nconnectionswait = 0;

  if(!connections.IsEmpty())
    {
      for(XDWORD c=0;c<connections.GetSize();c++)
        {
          DIOWEBSERVER_CONNECTION* connection = (DIOWEBSERVER_CONNECTION*)connections.Get(c);
          if(connection)
            {
              if(connection->IsOpening() || ((!connection->IsRequestInProgress()) && (connection->GetDIOStream()->GetStatus() == DIOSTREAMSTATUS_GETTINGCONNECTION)))
                {
                  nconnectionswait++;
                }
            }
        }
    }

  if(xmutexconnections) xmutexconnections->UnLock();

  return nconnectionswait;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOWEBSERVER::Connections_GetNConnectionsSendingPage()
* @brief      Connections get N connections sending page
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOWEBSERVER::Connections_GetNConnectionsSendingPage()
{
  int nconnections = 0;

  if(!connections.IsEmpty())
    {
      for(XDWORD c=0;c<connections.GetSize();c++)
        {
          DIOWEBSERVER_CONNECTION* connection = (DIOWEBSERVER_CONNECTION*)connections.Get(c);
          if(connection)
            {
              if(connection->IsRequestInProgress())
                {
                  nconnections++;
                }
            }
        }
    }

  return nconnections;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER::Connections_CreateNew()
* @brief      Connections create GEN_NEW
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER::Connections_CreateNew()
{
  XDWORD nwaiting = Connections_GetNWaiting();
  if(nwaiting >= maxpageconnections)  return false;

  DIOWEBSERVER_CONNECTION* connection = GEN_NEW DIOWEBSERVER_CONNECTION();
  if(!connection) return false;

  if(!connection->Ini(this, diostreamcfg))
    {
      GEN_DELETE connection;
      return false;
    }

  // DIOSTREAMTLS<T>::Open() in server mode performs accept + the complete TLS handshake synchronously.
  // Do not execute it in the DIOWEBSERVER control/accept thread: a slow or incomplete peer would stop this
  // thread from creating/accepting the following connections. The per-connection worker owns that blocking work.
  if(diostreamcfg && diostreamcfg->IsTLS())
    {
      SubscribeEvent(DIOSTREAM_XEVENT_TYPE_CONNECTED, connection->GetDIOStream());
      SubscribeEvent(DIOSTREAM_XEVENT_TYPE_DISCONNECTED , connection->GetDIOStream());

      if(xmutexconnections) xmutexconnections->Lock();
      connections.Add(connection);

      // Set the opening state and start the worker while the connection is already registered. ActivateOpen()
      // returns immediately; the network wait/handshake happens in DIOWEBSERVER_CONNECTION::ThreadRunFunction().
      bool status = connection->ActivateOpen();
      if(xmutexconnections) xmutexconnections->UnLock();

      if(!status)
        {
          if(xmutexconnections) xmutexconnections->Lock();
          connections.Delete(connection);
          if(xmutexconnections) xmutexconnections->UnLock();

          UnSubscribeEvent(DIOSTREAM_XEVENT_TYPE_CONNECTED, connection->GetDIOStream());
          UnSubscribeEvent(DIOSTREAM_XEVENT_TYPE_DISCONNECTED , connection->GetDIOStream());

          GEN_DELETE connection;
          return false;
        }

      return true;
    }

  // Keep the established non-TLS behavior unchanged. Its Open() only starts the native stream accept and the
  // CONNECTED event activates the per-connection request worker afterwards.
  if(!connection->GetDIOStream()->Open())
    {
      connection->End();
      GEN_DELETE connection;
      return false;
    }

  SubscribeEvent(DIOSTREAM_XEVENT_TYPE_CONNECTED, connection->GetDIOStream());
  SubscribeEvent(DIOSTREAM_XEVENT_TYPE_DISCONNECTED , connection->GetDIOStream());

  if(xmutexconnections) xmutexconnections->Lock();
  connections.Add(connection);
  if(xmutexconnections) xmutexconnections->UnLock();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER::Connections_DeleteUsed()
* @brief      Connections GEN_DELETE used
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER::Connections_DeleteUsed()
{
  if(xmutexconnections) xmutexconnections->Lock();

  if(!connections.IsEmpty())
    {
      int c = 0;

      do{ DIOWEBSERVER_CONNECTION* connection = (DIOWEBSERVER_CONNECTION*)connections.Get(c);
          if(connection)
            {
              if((!connection->IsOpening()) && (connection->GetDIOStream()->GetStatus() == DIOSTREAMSTATUS_DISCONNECTED) && (!connection->IsRequestInProgress()) && connection->GetReferenceCount()==0)
                {
                  UnSubscribeEvent(DIOSTREAM_XEVENT_TYPE_CONNECTED, connection->GetDIOStream());
                  UnSubscribeEvent(DIOSTREAM_XEVENT_TYPE_DISCONNECTED , connection->GetDIOStream());

                  // XTRACE_PRINTCOLOR(1, _L("Delete Connexion: [%08X]"), connection);

                  connections.Delete(connection);
                  GEN_DELETE connection;

                } else c++;
            }

        } while(c<(int)connections.GetSize());
    }

  if(xmutexconnections) xmutexconnections->UnLock();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER::Connections_DeleteWaiting()
* @brief      Connections GEN_DELETE waiting
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER::Connections_DeleteWaiting()
{
  if(xmutexconnections) xmutexconnections->Lock();

  if(!connections.IsEmpty())
    {
      int c = 0;

      do{ DIOWEBSERVER_CONNECTION* connection = (DIOWEBSERVER_CONNECTION*)connections.Get(c);
          if(connection)
            {
              if((connection->GetReferenceCount()==0) && (connection->IsOpening() || ((!connection->IsRequestInProgress()) && (connection->GetDIOStream()->GetStatus() == DIOSTREAMSTATUS_GETTINGCONNECTION))))
                {
                  connections.Delete(connection);
                  GEN_DELETE connection;

                } else c++;
            }

        } while(c<(int)connections.GetSize());
    }

  if(xmutexconnections) xmutexconnections->UnLock();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER::Connections_DeleteAll()
* @brief      Connections GEN_DELETE all
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER::Connections_DeleteAll()
{
  if(xmutexconnections) xmutexconnections->Lock();

  if(!connections.IsEmpty())
    {
      connections.DeleteContents();
      connections.DeleteAll();
    }

  if(xmutexconnections) xmutexconnections->UnLock();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBSERVER::HandleEvent_DIOStream(DIOSTREAM_XEVENT* event)
* @brief      Handle event DIO stream
* @note       INTERNAL
* @ingroup    DATAIO
* 
* @param[in]  event : Event to process.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBSERVER::HandleEvent_DIOStream(DIOSTREAM_XEVENT* event)
{
  if(!event) return;

  switch(event->GetEventType())
    {
      case DIOSTREAM_XEVENT_TYPE_CONNECTED    : { for(XDWORD c=0; c<connections.GetSize(); c++)
                                                  {
                                                    DIOWEBSERVER_CONNECTION* connection = (DIOWEBSERVER_CONNECTION*)connections.Get(c);
                                                    if(connection)
                                                      {
                                                        if(connection->GetDIOStream() == event->GetDIOStream())
                                                          {
                                                            // A TLS connection worker is started before Open(). Its underlying transport
                                                            // may post CONNECTED before (or be dispatched after) the TLS handshake finishes,
                                                            // so this event must never start that worker a second time. The worker marks/logs
                                                            // the connection active only after the complete TLS handshake succeeds.
                                                            if(!diostreamcfg || !diostreamcfg->IsTLS())
                                                              {
                                                                connection->Activate();

                                                                XSTRING IPstring;
                                                                connection->GetDIOStream()->GetClientIP()->GetXString(IPstring);
                                                                GEN_XLOG.AddEntry(XLOGLEVEL_INFO, DIOWEBSERVER_LOGSECTIONID_VERBOSE, false, _L("[%08X] Connection Active [%s]"), connection, IPstring.Get());
                                                              }

                                                            break;
                                                          }
                                                      }
                                                  }
                                              }
                                              break;

      case DIOSTREAM_XEVENT_TYPE_DISCONNECTED : { for(XDWORD c=0; c<connections.GetSize(); c++)
                                                  {
                                                    DIOWEBSERVER_CONNECTION* connection = (DIOWEBSERVER_CONNECTION*)connections.Get(c);
                                                    if(connection)
                                                      {
                                                        if(connection->GetDIOStream() == event->GetDIOStream())
                                                          {
                                                            XSTRING IPstring;
                                                            connection->GetDIOStream()->GetClientIP()->GetXString(IPstring);
                                                            GEN_XLOG.AddEntry(XLOGLEVEL_INFO, DIOWEBSERVER_LOGSECTIONID_VERBOSE, false, _L("[%08X] Connection Deactive [%s]"), connection, IPstring.Get());

                                                            break;
                                                          }
                                                      }
                                                  }
                                              }
                                              break;
    }

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBSERVER::HandleEvent(XEVENT* xevent)
* @brief      Handle event
* @note       INTERNAL
* @ingroup    DATAIO
* 
* @param[in]  xevent : Xevent pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBSERVER::HandleEvent(XEVENT* xevent)
{
  if(!xevent) return;

  switch(xevent->GetEventFamily())
    {
      case XEVENT_TYPE_DIOSTREAM   : { DIOSTREAM_XEVENT* event = (DIOSTREAM_XEVENT*)xevent;
                                      if(!event) return;

                                      HandleEvent_DIOStream(event);
                                    }
                                    break;

    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBSERVER::ThreadRunFunction(void* param)
* @brief      Thread run function
* @ingroup    DATAIO
* 
* @param[in]  param : Param pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBSERVER::ThreadRunFunction(void* param)
{
  DIOWEBSERVER* webserver = (DIOWEBSERVER*)param;
  if(!webserver) return;

  if(!webserver->diostreamcfg) return;

  if(webserver->doexit) return;

  webserver->Connections_DeleteUsed();
  webserver->Connections_CreateNew();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBSERVER::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBSERVER::Clean()
{
  diostreamcfg                      = NULL;
  port                              = DIOWEBSERVER_DEFAULTPORT;
  timeoutserverpage                 = 0;
  maxpageconnections                = DIOWEBSERVER_MAXPAGECONNECTIONS;
  xmutexconnections                 = NULL;

  isactive                          = false;
  doexit                            = false;
  xtimercontrol                     = NULL;
  threadserver                      = NULL;

  includedencodingflags             = DIOWEBSERVER_ENCODINGFLAGS_NONE;

  websocket_search_indexconnection  = -1;
  websocket_search_protocol.Empty();
  websocket_search_version          = 0;
  websocket_search_resource.Empty();
}
