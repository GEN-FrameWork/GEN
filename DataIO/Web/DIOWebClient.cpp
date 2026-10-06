/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOWebClient.cpp
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOWebClient.h"

#include "XFactory.h"
#include "XSleep.h"
#include "XTimer.h"
#include "XPath.h"
#include "XFile.h"
#include "XTrace.h"

#include "HashMD5.h"

#include "DIOFactory.h"
#include "DIOURL.h"
#include "DIOStreamTCPIPConfig.h"
#include "DIOStreamTCPIP.h"

#ifdef DIO_STREAMTLS_ACTIVE
#include "DIOStreamTLSConfig.h"
#endif

#ifdef COMPRESS_ACTIVE
#include "CompressManager.h"
#endif

#include "DIOWebClient_XEvent.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBCLIENT_OPERATIONERROR::DIOWEBCLIENT_OPERATIONERROR()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBCLIENT_OPERATIONERROR::DIOWEBCLIENT_OPERATIONERROR()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBCLIENT_ERRORSTAGE DIOWEBCLIENT_OPERATIONERROR::GetStage()
* @brief      Get stage
* @ingroup    DATAIO
* 
* @return     DIOWEBCLIENT_ERRORSTAGE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBCLIENT_ERRORSTAGE DIOWEBCLIENT_OPERATIONERROR::GetStage()
{
  return stage;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBCLIENT_ERROR DIOWEBCLIENT_OPERATIONERROR::GetError()
* @brief      Get error
* @ingroup    DATAIO
* 
* @return     DIOWEBCLIENT_ERROR : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBCLIENT_ERROR DIOWEBCLIENT_OPERATIONERROR::GetError()
{
  return error;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMERROR DIOWEBCLIENT_OPERATIONERROR::GetStreamError()
* @brief      Get stream error
* @ingroup    DATAIO
* 
* @return     DIOSTREAMERROR : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMERROR DIOWEBCLIENT_OPERATIONERROR::GetStreamError()
{
  return streamerror;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOWEBCLIENT_OPERATIONERROR::GetHTTPStatus()
* @brief      Get http status
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOWEBCLIENT_OPERATIONERROR::GetHTTPStatus()
{
  return HTTPstatus;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         const XCHAR* DIOWEBCLIENT_OPERATIONERROR::GetDescription()
* @brief      Get description
* @ingroup    DATAIO
* 
* @return     const XCHAR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
const XCHAR* DIOWEBCLIENT_OPERATIONERROR::GetDescription()
{
  switch(error)
    {
      case DIOWEBCLIENT_ERROR_INVALIDARGUMENT       : return _L("invalid operation argument");
      case DIOWEBCLIENT_ERROR_INVALIDURL            : return _L("invalid or unsupported URL");
      case DIOWEBCLIENT_ERROR_TRANSPORTUNAVAILABLE  : return _L("requested transport is not available");
      case DIOWEBCLIENT_ERROR_DNSRESOLUTION         : return _L("DNS resolution failed");
      case DIOWEBCLIENT_ERROR_TCPCONNECTION         : return _L("TCP connection failed");
      case DIOWEBCLIENT_ERROR_TCPTIMEOUT            : return _L("TCP connection timed out");
      case DIOWEBCLIENT_ERROR_PROXY                 : return _L("proxy connection or negotiation failed");
      case DIOWEBCLIENT_ERROR_TLSCONFIGURATION      : return _L("TLS configuration is invalid or unavailable");
      case DIOWEBCLIENT_ERROR_TLSPROTOCOL           : return _L("TLS protocol negotiation or record processing failed");
      case DIOWEBCLIENT_ERROR_TLSAUTHENTICATION     : return _L("TLS peer authentication failed");
      case DIOWEBCLIENT_ERROR_HTTPWRITE             : return _L("HTTP request could not be sent");
      case DIOWEBCLIENT_ERROR_HTTPRESPONSE          : return _L("HTTP response is missing or malformed");
      case DIOWEBCLIENT_ERROR_HTTPSTATUS            : return _L("HTTP server returned an error status");
      case DIOWEBCLIENT_ERROR_HTTPREDIRECT          : return _L("HTTP redirect was rejected");
      case DIOWEBCLIENT_ERROR_HTTPBODY              : return _L("HTTP response body could not be read");
      case DIOWEBCLIENT_ERROR_HTTPCONTENTENCODING   : return _L("HTTP response content encoding could not be decoded");
                                             default : return _L("no error");
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBCLIENT_OPERATIONERROR::IsSet()
* @brief      Is set
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBCLIENT_OPERATIONERROR::IsSet()
{
  return error != DIOWEBCLIENT_ERROR_NONE;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBCLIENT_OPERATIONERROR::Set(DIOWEBCLIENT_ERRORSTAGE stage, DIOWEBCLIENT_ERROR error, DIOSTREAMERROR streamerror, int HTTPstatus)
* @brief      Set
* @ingroup    DATAIO
* 
* @param[in]  stage : Stage value.
* @param[in]  error : Error value.
* @param[in]  streamerror : Streamerror value.
* @param[in]  HTTPstatus : HTTPstatus value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBCLIENT_OPERATIONERROR::Set(DIOWEBCLIENT_ERRORSTAGE stage, DIOWEBCLIENT_ERROR error, DIOSTREAMERROR streamerror, int HTTPstatus)
{
  this->stage       = stage;
  this->error       = error;
  this->streamerror = streamerror;
  this->HTTPstatus  = HTTPstatus;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBCLIENT_OPERATIONERROR::Clean()
* @brief      Clean
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBCLIENT_OPERATIONERROR::Clean()
{
  stage       = DIOWEBCLIENT_ERRORSTAGE_NONE;
  error       = DIOWEBCLIENT_ERROR_NONE;
  streamerror = DIOSTREAMERROR_NONE;
  HTTPstatus  = 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBCLIENT_HEADER::DIOWEBCLIENT_HEADER()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBCLIENT_HEADER::DIOWEBCLIENT_HEADER()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBCLIENT_HEADER::~DIOWEBCLIENT_HEADER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBCLIENT_HEADER::~DIOWEBCLIENT_HEADER()
{
  DeleteAllLines();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBCLIENT_HEADER::GetMethod(XSTRING& method)
* @brief      Get method
* @ingroup    DATAIO
* 
* @param[in]  method : Method descriptor to call.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBCLIENT_HEADER::GetMethod(XSTRING& method)
{
  if(!GetFormat(false, 0, &method)) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBCLIENT_HEADER::GetResource(XSTRING& resource)
* @brief      Get resource
* @ingroup    DATAIO
* 
* @param[in]  resource : Resource value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBCLIENT_HEADER::GetResource(XSTRING& resource)
{
  if(!GetFormat(false, 1, &resource)) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOWEBCLIENT_HEADER::GetVersion()
* @brief      Get version
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOWEBCLIENT_HEADER::GetVersion()
{
  int data = 0;

  if(!GetFormat(false, 2, &data)) return false;

  return data;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOWEBCLIENT_HEADER::GetSubVersion()
* @brief      Get sub version
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOWEBCLIENT_HEADER::GetSubVersion()
{
  int data = 0;

  if(!GetFormat(false, 3, &data)) return false;

  return data;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOWEBCLIENT_HEADER::GetResultServer()
* @brief      Get result server
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOWEBCLIENT_HEADER::GetResultServer()
{
  int data = 0;

  if(!GetFormat(true, 2, &data)) return false;

  return data;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XQWORD DIOWEBCLIENT_HEADER::GetContentLength()
* @brief      Get content length
* @ingroup    DATAIO
* 
* @return     XQWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XQWORD DIOWEBCLIENT_HEADER::GetContentLength()
{
  XQWORD contentlength = 0;

  GetContentLength(contentlength);

  return contentlength;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBCLIENT_HEADER::HasContentLength()
* @brief      Check whether a Content-Length response field is present
* @ingroup    DATAIO
* 
* @return     bool : true when the field is present; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBCLIENT_HEADER::HasContentLength()
{
  for(XDWORD c=0; c<lines.GetSize(); c++)
    {
      XSTRING* line = lines.Get(c);
      if(line)
        {
          int index = line->Find(_L(":"), true);
          if(index != XSTRING_NOTFOUND)
            {
              XSTRING field;

              line->Copy(0, index, field);
              if(!field.Compare(DIOWEBHEADER_CONTENT_LENGTH, true)) return true;
            }
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBCLIENT_HEADER::GetContentLength(XQWORD& contentlength)
* @brief      Get and validate the Content-Length response field
* @ingroup    DATAIO
* 
* @param[out] contentlength : Parsed content length.
* 
* @return     bool : true when a valid Content-Length field is present; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBCLIENT_HEADER::GetContentLength(XQWORD& contentlength)
{
  bool found = false;

  contentlength = 0;

  for(XDWORD c=0; c<lines.GetSize(); c++)
    {
      XSTRING* line = lines.Get(c);
      if(line)
        {
          int index = line->Find(_L(":"), true);
          if(index != XSTRING_NOTFOUND)
            {
              XSTRING field;

              line->Copy(0, index, field);
              if(field.Compare(DIOWEBHEADER_CONTENT_LENGTH, true)) continue;

              XDWORD  position = index + 1;
              XQWORD  value    = 0;
              bool    hasdigit = false;

              while(position < line->GetSize())
                {
                  XCHAR character = line->Get()[position];

                  if((character != _C(' ')) && (character != _C('\t'))) break;

                  position++;
                }

              while(position < line->GetSize())
                {
                  XCHAR character = line->Get()[position];

                  if((character < _C('0')) || (character > _C('9'))) break;

                  XQWORD digit   = character - _C('0');
                  XQWORD maximum = (XQWORD)-1;

                  if(value > ((maximum - digit) / 10)) return false;

                  value     = (value * 10) + digit;
                  hasdigit  = true;
                  position++;
                }

              if(!hasdigit) return false;

              while(position < line->GetSize())
                {
                  XCHAR character = line->Get()[position];

                  if((character != _C(' ')) && (character != _C('\t'))) return false;

                  position++;
                }

              if(found && (contentlength != value)) return false;

              contentlength = value;
              found         = true;
            }
        }
    }

  return found;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBCLIENT_HEADER::GetTransferEncoding(XSTRING& transferencoding)
* @brief      Get the combined Transfer-Encoding response field
* @ingroup    DATAIO
* 
* @param[out] transferencoding : Transfer coding list.
* 
* @return     bool : true when the field is present; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBCLIENT_HEADER::GetTransferEncoding(XSTRING& transferencoding)
{
  bool found = false;

  transferencoding.Empty();

  for(XDWORD c=0; c<lines.GetSize(); c++)
    {
      XSTRING* line = lines.Get(c);
      if(line)
        {
          int index = line->Find(_L(":"), true);
          if(index != XSTRING_NOTFOUND)
            {
              XSTRING field;
              XSTRING value;

              line->Copy(0, index, field);
              if(field.Compare(DIOWEBHEADER_TRANSFER_ENCODING, true)) continue;

              line->Copy(index+1, value);

              while(value.GetSize() && ((value.Get()[0] == _C(' ')) || (value.Get()[0] == _C('\t'))))
                {
                  value.DeleteCharacters(0, 1);
                }

              while(value.GetSize() && ((value.Get()[value.GetSize()-1] == _C(' ')) ||
                                        (value.Get()[value.GetSize()-1] == _C('\t'))))
                {
                  value.DeleteCharacters(value.GetSize()-1, 1);
                }

              if(found) transferencoding += _L(",");

              transferencoding += value;
              found              = true;
            }
        }
    }

  return found;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOWEBCLIENT_HEADER::GetContentEncoding(XSTRING& contentencoding)
* @brief      Get the Content-Encoding response field (e.g. "gzip", "deflate")
* @ingroup    DATAIO
*
* @param[out] contentencoding : Content coding value.
*
* @return     bool : true if the field is present and not empty; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBCLIENT_HEADER::GetContentEncoding(XSTRING& contentencoding)
{
  contentencoding.Empty();

  XCHAR* string = GetFieldValue(DIOWEBHEADER_CONTENT_ENCODING);

  while(string && ((*string == _C(' ')) || (*string == _C('\t'))))
    {
      string++;
    }

  if(string)  contentencoding = string;

  return contentencoding.GetSize()?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOWEBCLIENT_HEADER::GetETag(XSTRING& etag)
* @brief      Get E tag
* @ingroup    DATAIO
* 
* @param[in]  etag : Etag value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBCLIENT_HEADER::GetETag(XSTRING& etag)
{
  etag.Empty();

  XCHAR* string = GetFieldValue(DIOWEBHEADER_ETAG);
  if(string)  etag = string;

  return etag.GetSize()?true:false;

  /*
  for(XDWORD c=0; c<lines.GetSize(); c++)
    {
      XSTRING* line = lines.Get(c);
      if(line)
        {
          if(line.Find(DIOWEBHEADER_ETAG, true) != XSTRING_NOTFOUND)
            {
              int index = line.Find(_L(":"), true);
              if(index != XSTRING_NOTFOUND)  line.Copy(index+1, etag);
            }
        }
    }

  return etag.GetSize()?true:false;
  */
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBCLIENT_HEADER::GetWWWAuthenticate(XSTRING& authenticate)
* @brief      Get WWW authenticate
* @ingroup    DATAIO
* 
* @param[in]  authenticate : Authenticate value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBCLIENT_HEADER::GetWWWAuthenticate(XSTRING& authenticate)
{
  authenticate.Empty();

  XCHAR* string = GetFieldValue(DIOWEBHEADER_WWW_AUTHENTICATE);
  if(string)  authenticate = string;

  return authenticate.GetSize()?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOWEBCLIENT_HEADER::GetLocation(XSTRING& location)
* @brief      Get the Location response field (redirection target)
* @ingroup    DATAIO
*
* @param[out] location : Location value.
*
* @return     bool : true if the field is present and not empty; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBCLIENT_HEADER::GetLocation(XSTRING& location)
{
  location.Empty();

  XCHAR* string = GetFieldValue(DIOWEBHEADER_LOCATION);

  while(string && ((*string == _C(' ')) || (*string == _C('\t'))))
    {
      string++;
    }

  if(string)  location = string;

  return location.GetSize()?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBCLIENT_HEADER::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBCLIENT_HEADER::Clean()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBCLIENT::DIOWEBCLIENT(XDWORD maxsizebuffer)
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* @param[in]  maxsizebuffer : Maxsizebuffer value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBCLIENT::DIOWEBCLIENT(XDWORD maxsizebuffer)
{
  Clean();

  this->maxsizebuffer = maxsizebuffer;

  RegisterEvent(DIOWEBCLIENT_XEVENT_TYPE_OPENWEB);
  RegisterEvent(DIOWEBCLIENT_XEVENT_TYPE_WRITEHEADER);
  RegisterEvent(DIOWEBCLIENT_XEVENT_TYPE_SENDPOSTDATA);
  RegisterEvent(DIOWEBCLIENT_XEVENT_TYPE_HEADERERROR);
  RegisterEvent(DIOWEBCLIENT_XEVENT_TYPE_READHEADER);
  RegisterEvent(DIOWEBCLIENT_XEVENT_TYPE_READBODYBLOCK);
  RegisterEvent(DIOWEBCLIENT_XEVENT_TYPE_CLOSEWEB);

  timerout=GEN_XFACTORY.CreateTimer();
  
  #ifdef DIO_STREAMTLS_ACTIVE

  diostreamcfg = GEN_NEW DIOSTREAMTLSCONFIG();

  // Automatic version negotiation, by default, for every HTTPS request made through DIOWEBCLIENT: a caller never
  // has to know or decide whether the target speaks TLS 1.3 or only TLS 1.2. GetOpen() tries TLS 1.3 first and,
  // only if that specific server rejects it at the handshake stage, retries the same request once as pure TLS
  // 1.2 (see DIOSTREAMTLS<T>::Open() / Handshake_Attempt()). A caller that wants strict TLS 1.3-only behavior
  // can still narrow the window back with GetStreamTLSCFG()->SetMaxVersion(DIOSTREAMTLS_MSG_VERSION_TLS_1_3) /
  // SetMinVersion(DIOSTREAMTLS_MSG_VERSION_TLS_1_3) after construction.
  if(diostreamcfg)
    {
      DIOSTREAMTLSCONFIG* tlsconfig = (DIOSTREAMTLSCONFIG*)diostreamcfg;

      tlsconfig->SetMinVersion(DIOSTREAMTLS_MSG_VERSION_TLS_1_2);

      // DIOWEBCLIENT only speaks HTTP/1.1.  Configure ALPN once, while the static TLS
      // configuration is still mutable; subsequent requests may reuse the same frozen
      // configuration without attempting to modify it.
      tlsconfig->ApplicationProtocol_Add(DIOSTREAMTLS_ALPN_TYPE_HTTP_1_1);
    }

  #else

  diostreamcfg = GEN_NEW DIOSTREAMTCPIPCONFIG();

  #endif

  if(diostreamcfg)
    {
      diostreamcfg->SetIsTLS(false);
      Stream_Create(false);
    }

  #ifdef COMPRESS_ACTIVE
  compressmanager = GEN_NEW COMPRESSMANAGER();
  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBCLIENT::~DIOWEBCLIENT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBCLIENT::~DIOWEBCLIENT()
{
  DeRegisterEvent(DIOWEBCLIENT_XEVENT_TYPE_OPENWEB);
  DeRegisterEvent(DIOWEBCLIENT_XEVENT_TYPE_WRITEHEADER);
  DeRegisterEvent(DIOWEBCLIENT_XEVENT_TYPE_SENDPOSTDATA);
  DeRegisterEvent(DIOWEBCLIENT_XEVENT_TYPE_HEADERERROR);
  DeRegisterEvent(DIOWEBCLIENT_XEVENT_TYPE_READHEADER);
  DeRegisterEvent(DIOWEBCLIENT_XEVENT_TYPE_READBODYBLOCK);
  DeRegisterEvent(DIOWEBCLIENT_XEVENT_TYPE_CLOSEWEB);

  if(diostream)
    {
      GEN_DIOFACTORY.DeleteStreamIO(diostream);
      diostream = NULL;
    }

  if(diostreamcfg)
    {
      GEN_DELETE diostreamcfg;
      diostreamcfg = NULL;
    }

  if(timerout)
    {
      GEN_XFACTORY.DeleteTimer(timerout);
      timerout = NULL;
    }

  #ifdef COMPRESS_ACTIVE
  if(compressmanager)
    {
      GEN_DELETE compressmanager;
      compressmanager = NULL;
    }
  #endif

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOWEBCLIENT::Get_Port()
* @brief      Get port
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOWEBCLIENT::Get_Port()
{
  return port;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBCLIENT::Set_Port(int port)
* @brief      Set port
* @ingroup    DATAIO
* 
* @param[in]  port : Port number to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBCLIENT::Set_Port(int port)
{
  this->port       = port;
  isportconfigured = true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOURL* DIOWEBCLIENT::GetProxyURL()
* @brief      Get proxy URL
* @ingroup    DATAIO
* 
* @return     DIOURL* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOURL* DIOWEBCLIENT::GetProxyURL()
{
  if(!diostreamcfg) return NULL;

  return diostreamcfg->GetProxyCFG()->GetURL();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTCPIPPROXYCFG* DIOWEBCLIENT::GetProxyCFG()
* @brief      Get proxy configuration
* @ingroup    DATAIO
* 
* @return     DIOSTREAMTCPIPPROXYCFG* : Proxy configuration.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTCPIPPROXYCFG* DIOWEBCLIENT::GetProxyCFG()
{
  if(!diostreamcfg) return NULL;

  return diostreamcfg->GetProxyCFG();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOWEBCLIENT::GetProxyPort()
* @brief      Get proxy port
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOWEBCLIENT::GetProxyPort()
{
  if(!diostreamcfg) return 0;

  return diostreamcfg->GetProxyCFG()->GetPort();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBCLIENT::SetProxyPort(int port)
* @brief      Set proxy port
* @ingroup    DATAIO
* 
* @param[in]  port : Port number to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBCLIENT::SetProxyPort(int port)
{
  if(!diostreamcfg) return;

  DIOSTREAMTCPIPPROXYCFG* proxycfg = diostreamcfg->GetProxyCFG();

  proxycfg->SetPort(port);
  if(port > 0) proxycfg->SetType(DIOSTREAMTCPIPPROXYTYPE_HTTP);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBCLIENT_AUTHENTICATION_METHOD DIOWEBCLIENT::GetAuthenticationMethod()
* @brief      Get authentication method
* @ingroup    DATAIO
* 
* @return     DIOWEBCLIENT_AUTHENTICATION_METHOD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBCLIENT_AUTHENTICATION_METHOD DIOWEBCLIENT::GetAuthenticationMethod()
{
  return authenticationmethod;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBCLIENT::SetAuthenticationMethod(DIOWEBCLIENT_AUTHENTICATION_METHOD authenticationmethod)
* @brief      Set authentication method
* @ingroup    DATAIO
* 
* @param[in]  authenticationmethod : Authenticationmethod value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBCLIENT::SetAuthenticationMethod(DIOWEBCLIENT_AUTHENTICATION_METHOD authenticationmethod)
{
  this->authenticationmethod = authenticationmethod;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOWEBCLIENT::GetLogin()
* @brief      Get login
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING*  DIOWEBCLIENT::GetLogin()
{
  return &login;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOWEBCLIENT::GetPassword()
* @brief      Get password
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING*  DIOWEBCLIENT::GetPassword()
{
  return &password;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTCPIPCONFIG* DIOWEBCLIENT::GetStreamCFG()
* @brief      Get stream CFG
* @ingroup    DATAIO
* 
* @return     DIOSTREAMTCPIPCONFIG* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTCPIPCONFIG* DIOWEBCLIENT::GetStreamCFG()
{
  return diostreamcfg;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTLSCONFIG* DIOWEBCLIENT::GetStreamTLSCFG()
* @brief      Get TLS stream CFG
* @ingroup    DATAIO
* 
* @return     DIOSTREAMTLSCONFIG* : Pointer to the TLS configuration; NULL if TLS is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTLSCONFIG* DIOWEBCLIENT::GetStreamTLSCFG()
{
  #ifdef DIO_STREAMTLS_ACTIVE

  return (DIOSTREAMTLSCONFIG*)diostreamcfg;

  #else

  return NULL;

  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBCLIENT_HEADER* DIOWEBCLIENT::GetHeader()
* @brief      Get header
* @ingroup    DATAIO
* 
* @return     DIOWEBCLIENT_HEADER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBCLIENT_HEADER* DIOWEBCLIENT::GetHeader()
{
  return &header;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBCLIENT::IsActiveDoStopHTTPError()
* @brief      Is active do stop HTTP error
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBCLIENT::IsActiveDoStopHTTPError()
{
  return dostophttperror;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBCLIENT::DoStopHTTPError(bool activate)
* @brief      Do stop HTTP error
* @ingroup    DATAIO
* 
* @param[in]  activate : Activate value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBCLIENT::DoStopHTTPError(bool activate)
{
  dostophttperror = activate;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBCLIENT_TRANSPORTPOLICY DIOWEBCLIENT::GetTransportPolicy()
* @brief      Get the transport policy used for URLs without an explicit scheme
* @ingroup    DATAIO
* 
* @return     DIOWEBCLIENT_TRANSPORTPOLICY : Current transport policy.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBCLIENT_TRANSPORTPOLICY DIOWEBCLIENT::GetTransportPolicy()
{
  return transportpolicy;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBCLIENT::SetTransportPolicy(DIOWEBCLIENT_TRANSPORTPOLICY policy)
* @brief      Set the transport policy used for URLs without an explicit scheme
* @ingroup    DATAIO
* 
* @param[in]  policy : Transport policy.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBCLIENT::SetTransportPolicy(DIOWEBCLIENT_TRANSPORTPOLICY policy)
{
  transportpolicy = policy;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBCLIENT::IsInsecureRedirectAllowed()
* @brief      Check whether HTTPS to HTTP redirects are explicitly allowed
* @ingroup    DATAIO
* 
* @return     bool : true if insecure redirects are allowed; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBCLIENT::IsInsecureRedirectAllowed()
{
  return allowinsecureredirect;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBCLIENT::AllowInsecureRedirect(bool allow)
* @brief      Allow or deny HTTPS to HTTP redirects
* @ingroup    DATAIO
* 
* @param[in]  allow : true to allow the downgrade explicitly; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBCLIENT::AllowInsecureRedirect(bool allow)
{
  allowinsecureredirect = allow;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOWEBCLIENT::IsActiveAutoHTTPFallback()
* @brief      Check whether the automatic HTTP fallback is active
* @ingroup    DATAIO
* @note       This governs the automatic retry over plain HTTP that MakeOperation() performs when a scheme-less
*             URL (no explicit http:// or https://) fails its HTTPS attempt with a cause that
*             OperationError_AllowsHTTPFallback() considers eligible (transport/TCP/proxy/TLS-protocol errors).
*             A URL with an explicit scheme is never affected by this setting, and a TLS authentication failure
*             (certificate/hostname rejected) never falls back to HTTP regardless of this setting either.
*
* @return     bool : true if the automatic HTTP fallback is active; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBCLIENT::IsActiveAutoHTTPFallback()
{
  return autohttpfallbackactive;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOWEBCLIENT::AutoHTTPFallback_Activate(bool activate)
* @brief      Activate or deactivate the automatic HTTP fallback for scheme-less URLs
* @ingroup    DATAIO
* @note       When deactivated, a scheme-less URL that fails its HTTPS attempt is never retried over plain HTTP:
*             MakeOperation() returns the HTTPS failure as-is. This does not change how http:// or https:// are
*             resolved when the URL carries an explicit scheme.
*
* @param[in]  activate : true to activate the automatic fallback; false to deactivate it.
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBCLIENT::AutoHTTPFallback_Activate(bool activate)
{
  autohttpfallbackactive = activate;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBCLIENT_OPERATIONERROR* DIOWEBCLIENT::GetLastOperationError()
* @brief      Get last operation error
* @ingroup    DATAIO
* 
* @return     DIOWEBCLIENT_OPERATIONERROR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBCLIENT_OPERATIONERROR* DIOWEBCLIENT::GetLastOperationError()
{
  return &lastoperationerror;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBCLIENT_OPERATIONERROR* DIOWEBCLIENT::GetLastHTTPSAttemptError()
* @brief      Get last https attempt error
* @ingroup    DATAIO
* 
* @return     DIOWEBCLIENT_OPERATIONERROR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBCLIENT_OPERATIONERROR* DIOWEBCLIENT::GetLastHTTPSAttemptError()
{
  return &lastHTTPSattempterror;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBCLIENT::WasHTTPFallbackUsed()
* @brief      Was http fallback used
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBCLIENT::WasHTTPFallbackUsed()
{
  return HTTPfallbackused;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOWEBCLIENT::IsActiveContentEncoding()
* @brief      Is Content-Encoding support active
* @ingroup    DATAIO
*
* @return     bool : true if the condition is met; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBCLIENT::IsActiveContentEncoding()
{
  return contentencodingactive;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOWEBCLIENT::ContentEncoding_Activate(bool activate)
* @brief      Activate or deactivate Content-Encoding support (request Accept-Encoding, decompress the response)
* @ingroup    DATAIO
*
* @param[in]  activate : Activate value.
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBCLIENT::ContentEncoding_Activate(bool activate)
{
  contentencodingactive = activate;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOWEBCLIENT::IsActiveCompressRequestBody()
* @brief      Is outgoing request body compression active
* @ingroup    DATAIO
*
* @return     bool : true if the condition is met; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBCLIENT::IsActiveCompressRequestBody()
{
  return compressrequestbody;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOWEBCLIENT::CompressRequestBody_Activate(bool activate)
* @brief      Activate or deactivate gzip-compressing an outgoing Put()/Post() body
* @ingroup    DATAIO
*
* @param[in]  activate : Activate value.
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBCLIENT::CompressRequestBody_Activate(bool activate)
{
  compressrequestbody = activate;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBCLIENT::Get(DIOURL& url, XBUFFER& tobuffer, XCHAR* addheader, int timeout, XSTRING* localIP)
* @brief      Get value
* @ingroup    DATAIO
* 
* @param[in]  url : URL to use.
* @param[in]  tobuffer : Tobuffer value.
* @param[in]  addheader : Addheader pointer to use.
* @param[in]  timeout : Timeout value.
* @param[in]  localIP : Local IP pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBCLIENT::Get(DIOURL& url, XBUFFER& tobuffer, XCHAR* addheader, int timeout, XSTRING* localIP)
{
  return MakeOperation(DIOWEBHEADER_METHOD_GET, url, NULL, addheader, timeout, localIP, true, (void*)&tobuffer);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBCLIENT::Get(XCHAR* url, XBUFFER& tobuffer, XCHAR* addheader, int timeout, XSTRING* localIP)
* @brief      Get value
* @ingroup    DATAIO
* 
* @param[in]  url : URL to use.
* @param[in]  tobuffer : Tobuffer value.
* @param[in]  addheader : Addheader pointer to use.
* @param[in]  timeout : Timeout value.
* @param[in]  localIP : Local IP pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBCLIENT::Get(XCHAR* url, XBUFFER& tobuffer, XCHAR* addheader, int timeout, XSTRING* localIP)
{
  DIOURL _url;
  _url = url;

  return MakeOperation(DIOWEBHEADER_METHOD_GET, _url, NULL, addheader, timeout, localIP, true, (void*)&tobuffer);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBCLIENT::Get(DIOURL& url, XPATH& pathfile, XCHAR* addheader, int timeout, XSTRING* localIP)
* @brief      Get value
* @ingroup    DATAIO
* 
* @param[in]  url : URL to use.
* @param[in]  pathfile : Pathfile value.
* @param[in]  addheader : Addheader pointer to use.
* @param[in]  timeout : Timeout value.
* @param[in]  localIP : Local IP pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBCLIENT::Get(DIOURL& url, XPATH& pathfile, XCHAR* addheader, int timeout, XSTRING* localIP)
{
  OperationError_Reset();
  XFILE* file=GEN_XFACTORY.Create_File();
  if(!file)
    {
      OperationError_Set(DIOWEBCLIENT_ERROR_INVALIDARGUMENT);
      return false;
    }
  
  bool status  = false;

  if(!file->Open(pathfile, false))
    {
      if(file->Create(pathfile))
        {
          status = true;
        }

    } else status = true;

  if(status)
    {      
      status = MakeOperation(DIOWEBHEADER_METHOD_GET, url, NULL, addheader, timeout, localIP, false, (void*)file);

      file->Close();

      if(!status)  file->Erase(pathfile);
    }

  GEN_XFACTORY.Delete_File(file);

  if(!status && !lastoperationerror.IsSet()) OperationError_Set(DIOWEBCLIENT_ERROR_INVALIDARGUMENT);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBCLIENT::Get(XCHAR* url, XPATH& pathfile, XCHAR* addheader, int timeout, XSTRING* localIP)
* @brief      Get value
* @ingroup    DATAIO
* 
* @param[in]  url : URL to use.
* @param[in]  pathfile : Pathfile value.
* @param[in]  addheader : Addheader pointer to use.
* @param[in]  timeout : Timeout value.
* @param[in]  localIP : Local IP pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBCLIENT::Get(XCHAR* url, XPATH& pathfile, XCHAR* addheader, int timeout, XSTRING* localIP)
{
  DIOURL _url;
  _url = url;

  return Get(_url, pathfile, addheader, timeout, localIP);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBCLIENT::Put(DIOURL& url, XBUFFER& tobuffer, XCHAR* addheader, int timeout, XSTRING* localIP)
* @brief      Put
* @ingroup    DATAIO
* 
* @param[in]  url : URL to use.
* @param[in]  tobuffer : Tobuffer value.
* @param[in]  addheader : Addheader pointer to use.
* @param[in]  timeout : Timeout value.
* @param[in]  localIP : Local IP pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBCLIENT::Put(DIOURL& url, XBUFFER& tobuffer, XCHAR* addheader, int timeout, XSTRING* localIP)
{
  return MakeOperation(DIOWEBHEADER_METHOD_PUT, url, NULL, addheader, timeout, localIP, true, (void*)&tobuffer);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBCLIENT::Put(XCHAR* url, XBUFFER& tobuffer, XCHAR* addheader, int timeout, XSTRING* localIP)
* @brief      Put
* @ingroup    DATAIO
* 
* @param[in]  url : URL to use.
* @param[in]  tobuffer : Tobuffer value.
* @param[in]  addheader : Addheader pointer to use.
* @param[in]  timeout : Timeout value.
* @param[in]  localIP : Local IP pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBCLIENT::Put(XCHAR* url, XBUFFER& tobuffer, XCHAR* addheader, int timeout, XSTRING* localIP)
{
  DIOURL _url;
  _url = url;

  return MakeOperation(DIOWEBHEADER_METHOD_PUT, _url, NULL, addheader, timeout, localIP, true, (void*)&tobuffer);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBCLIENT::Put(DIOURL& url, XPATH& pathfile, XCHAR* addheader, int timeout, XSTRING* localIP)
* @brief      Put
* @ingroup    DATAIO
* 
* @param[in]  url : URL to use.
* @param[in]  pathfile : Pathfile value.
* @param[in]  addheader : Addheader pointer to use.
* @param[in]  timeout : Timeout value.
* @param[in]  localIP : Local IP pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBCLIENT::Put(DIOURL& url, XPATH& pathfile, XCHAR* addheader, int timeout, XSTRING* localIP)
{
  OperationError_Reset();
  XFILE* file=GEN_XFACTORY.Create_File();
  if(!file)
    {
      OperationError_Set(DIOWEBCLIENT_ERROR_INVALIDARGUMENT);
      return false;
    }

  XBUFFER buffer;
  bool    status  = false;

  if(!file->Open(pathfile, false))
    {
      if(file->Create(pathfile))
        {
          status = true;
        }

    } else status = true;

  if(status)
    {
      status = MakeOperation(DIOWEBHEADER_METHOD_PUT, url, &buffer, addheader, timeout, localIP, false, (void*)file);
      if(status) file->Write(buffer);

      file->Close();

      if(!status)  file->Erase(pathfile);
    }

  GEN_XFACTORY.Delete_File(file);

  if(!status && !lastoperationerror.IsSet()) OperationError_Set(DIOWEBCLIENT_ERROR_INVALIDARGUMENT);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBCLIENT::Put(XCHAR* url, XPATH& pathfile, XCHAR* addheader, int timeout, XSTRING* localIP)
* @brief      Put
* @ingroup    DATAIO
* 
* @param[in]  url : URL to use.
* @param[in]  pathfile : Pathfile value.
* @param[in]  addheader : Addheader pointer to use.
* @param[in]  timeout : Timeout value.
* @param[in]  localIP : Local IP pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBCLIENT::Put(XCHAR* url, XPATH& pathfile, XCHAR* addheader, int timeout, XSTRING* localIP)
{
  DIOURL _url;
  _url = url;

  return Put(_url, pathfile, addheader, timeout, localIP);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBCLIENT::Post(DIOURL& url, XBUFFER& tobuffer, XBUFFER* postdata, XCHAR* addheader, int timeout, XSTRING* localIP)
* @brief      Post
* @ingroup    DATAIO
* 
* @param[in]  url : URL to use.
* @param[in]  tobuffer : Tobuffer value.
* @param[in]  postdata : Postdata pointer to use.
* @param[in]  addheader : Addheader pointer to use.
* @param[in]  timeout : Timeout value.
* @param[in]  localIP : Local IP pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBCLIENT::Post(DIOURL& url, XBUFFER& tobuffer, XBUFFER* postdata, XCHAR* addheader, int timeout, XSTRING* localIP)
{
  return MakeOperation(DIOWEBHEADER_METHOD_POST, url, postdata, addheader, timeout, localIP, true, (void*)&tobuffer);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBCLIENT::Post(XCHAR* url, XBUFFER& tobuffer, XBUFFER* postdata, XCHAR* addheader, int timeout, XSTRING* localIP)
* @brief      Post
* @ingroup    DATAIO
* 
* @param[in]  url : URL to use.
* @param[in]  tobuffer : Tobuffer value.
* @param[in]  postdata : Postdata pointer to use.
* @param[in]  addheader : Addheader pointer to use.
* @param[in]  timeout : Timeout value.
* @param[in]  localIP : Local IP pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBCLIENT::Post(XCHAR* url, XBUFFER& tobuffer, XBUFFER* postdata, XCHAR* addheader, int timeout, XSTRING* localIP)
{
  DIOURL _url;
  _url = url;

  return MakeOperation(DIOWEBHEADER_METHOD_POST, _url, postdata, addheader, timeout, localIP, true, (void*)&tobuffer);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBCLIENT::Post(DIOURL& url, XPATH& pathfile, XBUFFER* postdata, XCHAR* addheader, int timeout, XSTRING* localIP)
* @brief      Post
* @ingroup    DATAIO
* 
* @param[in]  url : URL to use.
* @param[in]  pathfile : Pathfile value.
* @param[in]  postdata : Postdata pointer to use.
* @param[in]  addheader : Addheader pointer to use.
* @param[in]  timeout : Timeout value.
* @param[in]  localIP : Local IP pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBCLIENT::Post(DIOURL& url, XPATH& pathfile, XBUFFER* postdata, XCHAR* addheader, int timeout, XSTRING* localIP)
{
  OperationError_Reset();
  XFILE* file=GEN_XFACTORY.Create_File();
  if(!file)
    {
      OperationError_Set(DIOWEBCLIENT_ERROR_INVALIDARGUMENT);
      return false;
    }

  XBUFFER buffer;
  bool    status  = false;

  if(file->Create(pathfile))
    {
      status = Post(url, buffer, postdata, addheader, timeout, localIP);

      if(status) file->Write(buffer.Get(), buffer.GetSize());

      file->Close();

      if(!status) file->Erase(pathfile);
    }

  GEN_XFACTORY.Delete_File(file);

  if(!status && !lastoperationerror.IsSet()) OperationError_Set(DIOWEBCLIENT_ERROR_INVALIDARGUMENT);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBCLIENT::Post(XCHAR* url, XPATH& pathfile, XBUFFER* postdata, XCHAR* addheader, int timeout, XSTRING* localIP)
* @brief      Post
* @ingroup    DATAIO
* 
* @param[in]  url : URL to use.
* @param[in]  pathfile : Pathfile value.
* @param[in]  postdata : Postdata pointer to use.
* @param[in]  addheader : Addheader pointer to use.
* @param[in]  timeout : Timeout value.
* @param[in]  localIP : Local IP pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBCLIENT::Post(XCHAR* url, XPATH& pathfile, XBUFFER* postdata, XCHAR* addheader, int timeout, XSTRING* localIP)
{
  DIOURL _url;
  _url = url;

  return Post(_url, pathfile, postdata, addheader, timeout, localIP);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBCLIENT::Header_Read(int timeout)
* @brief      Read the final HTTP response header
* @ingroup    DATAIO
* 
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if a final response header was read; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBCLIENT::Header_Read(int timeout)
{
  for(int c=0; c<8; c++)
    {
      if(!header.Read(diostream, timeout)) return false;

      int result = header.GetResultServer();
      if(!result) return false;

      if((result < 100) || (result >= 200)) return true;
      if(result == DIOWEBHEADER_RESULT_SWITCHINGPROTOCOLS) return false;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBCLIENT::BodyBlock_Write(XBYTE* data, XDWORD size, bool istobuffer, void* to, XQWORD& totalsizeread, XQWORD contentlength, XTIMER* timerdownload, DIOWEBCLIENT_XEVENT& xevent)
* @brief      Write one decoded HTTP body block
* @ingroup    DATAIO
* 
* @param[in]  data : Body data.
* @param[in]  size : Body data size.
* @param[in]  istobuffer : true to write to an XBUFFER; false to write to an XFILE.
* @param[in]  to : Destination object.
* @param[in,out] totalsizeread : Total decoded body size.
* @param[in]  contentlength : Expected content length, or zero when it is not known.
* @param[in]  timerdownload : Download timer.
* @param[in]  xevent : Web client event.
* 
* @return     bool : true if the block was written; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBCLIENT::BodyBlock_Write(XBYTE* data, XDWORD size, bool istobuffer, void* to, XQWORD& totalsizeread, XQWORD contentlength, XTIMER* timerdownload, DIOWEBCLIENT_XEVENT& xevent)
{
  if(!data || !size || !to || !timerdownload) return false;

  XQWORD maximum = (XQWORD)-1;
  if(totalsizeread > (maximum - size)) return false;

  bool status;

  if(istobuffer)
    {
      status = ((XBUFFER*)to)->Add(data, size);
    }
   else
    {
      status = ((XFILE*)to)->Write(data, size);
    }

  if(!status) return false;

  totalsizeread += size;

  xevent.SetEventType(DIOWEBCLIENT_XEVENT_TYPE_READBODYBLOCK);
  xevent.SetDownloadSize(totalsizeread);
  xevent.SetContentLenght(contentlength);

  if(contentlength) xevent.SetOperationPercent((float)((totalsizeread * 100.0f) / contentlength));

  if(timerdownload->GetMeasureSeconds()) xevent.SetMBPSSpeed((XDWORD)(((totalsizeread * 8) / timerdownload->GetMeasureSeconds()) / 1000));

  PostEvent(&xevent);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBCLIENT_CHUNKEDRESULT DIOWEBCLIENT::ChunkSize_Get(XBUFFER& input, XQWORD& chunksize)
* @brief      Extract and validate one HTTP chunk-size line
* @ingroup    DATAIO
* 
* @param[in,out] input : Pending transfer-coded bytes.
* @param[out] chunksize : Parsed chunk size.
* 
* @return     DIOWEBCLIENT_CHUNKEDRESULT : Complete, incomplete or error.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBCLIENT_CHUNKEDRESULT DIOWEBCLIENT::ChunkSize_Get(XBUFFER& input, XQWORD& chunksize)
{
  int lineend = XBUFFER_INVALIDPOSITION;

  chunksize = 0;

  for(XDWORD c=0; (c+1)<input.GetSize(); c++)
    {
      if((input.GetByte(c) == 0x0D) && (input.GetByte(c+1) == 0x0A))
        {
          lineend = c;
          break;
        }
    }

  if(lineend == XBUFFER_INVALIDPOSITION)
    {
      if(input.GetSize() > DIOWEBHEADER_MAXLINE) return DIOWEBCLIENT_CHUNKEDRESULT_ERROR;

      return DIOWEBCLIENT_CHUNKEDRESULT_INCOMPLETE;
    }

  bool   hasdigit    = false;
  bool   afterdigits = false;
  XQWORD maximum     = (XQWORD)-1;

  for(int c=0; c<lineend; c++)
    {
      XBYTE character = input.GetByte(c);

      if(character == _C(';')) break;

      if((character == _C(' ')) || (character == _C('\t')))
        {
          if(hasdigit) afterdigits = true;
          continue;
        }

      if(afterdigits) return DIOWEBCLIENT_CHUNKEDRESULT_ERROR;

      XBYTE digit;

      if((character >= _C('0')) && (character <= _C('9'))) digit = character - _C('0');
       else if((character >= _C('a')) && (character <= _C('f'))) digit = character - _C('a') + 10;
       else if((character >= _C('A')) && (character <= _C('F'))) digit = character - _C('A') + 10;
       else return DIOWEBCLIENT_CHUNKEDRESULT_ERROR;

      if(chunksize > ((maximum - digit) >> 4)) return DIOWEBCLIENT_CHUNKEDRESULT_ERROR;

      chunksize = (chunksize << 4) + digit;
      hasdigit  = true;
    }

  if(!hasdigit) return DIOWEBCLIENT_CHUNKEDRESULT_ERROR;

  input.Extract(NULL, 0, lineend+2);

  return DIOWEBCLIENT_CHUNKEDRESULT_COMPLETE;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBCLIENT::Body_Read(DIOWEBCLIENT_BODYMODE bodymode, bool isTLS, XQWORD contentlength, int timeout, bool istobuffer, void* to, DIOWEBCLIENT_XEVENT& xevent)
* @brief      Read one framed HTTP response body
* @ingroup    DATAIO
* 
* @param[in]  bodymode : Response body framing mode.
* @param[in]  isTLS : true when the transport is TLS.
* @param[in]  contentlength : Expected body size for Content-Length framing.
* @param[in]  timeout : Timeout value.
* @param[in]  istobuffer : true to write to an XBUFFER; false to write to an XFILE.
* @param[in]  to : Destination object.
* @param[in]  xevent : Web client event.
* 
* @return     bool : true if the complete body was read; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBCLIENT::Body_Read(DIOWEBCLIENT_BODYMODE bodymode, bool isTLS, XQWORD contentlength, int timeout, bool istobuffer, void* to, DIOWEBCLIENT_XEVENT& xevent)
{
  if(!to || !maxsizebuffer || (bodymode == DIOWEBCLIENT_BODYMODE_UNKNOWN)) return false;

  if(istobuffer) ((XBUFFER*)to)->Delete();

  if(bodymode == DIOWEBCLIENT_BODYMODE_NONE) return true;

  XTIMER* timerdownload = GEN_XFACTORY.CreateTimer();
  if(!timerdownload) return false;

  XBYTE* buffer = GEN_NEW XBYTE[maxsizebuffer];
  if(!buffer)
    {
      GEN_XFACTORY.DeleteTimer(timerdownload);
      return false;
    }

  XBUFFER                   chunkinput;
  DIOWEBCLIENT_CHUNKEDSTATE chunkstate       = DIOWEBCLIENT_CHUNKEDSTATE_SIZE;
  XQWORD                    chunkremaining   = 0;
  XQWORD                    contentremaining = contentlength;
  XQWORD                    totalsizeread    = 0;
  bool                      completed        = false;
  bool                      status           = true;

  if((bodymode == DIOWEBCLIENT_BODYMODE_CONTENTLENGTH) && !contentremaining) completed = true;

  timerout->Reset();

  while(status && !completed)
    {
      XDWORD sizetoread = maxsizebuffer;

      if((bodymode == DIOWEBCLIENT_BODYMODE_CONTENTLENGTH) && (contentremaining < sizetoread))
        {
          sizetoread = (XDWORD)contentremaining;
        }

      if(!sizetoread)
        {
          completed = true;
          break;
        }

      memset(buffer, 0, sizetoread);

      XDWORD sizeread = diostream->Read(buffer, sizetoread);
      if(sizeread)
        {
          timerout->Reset();

          switch(bodymode)
            {
              case DIOWEBCLIENT_BODYMODE_CONTENTLENGTH     : if(!BodyBlock_Write(buffer, sizeread, istobuffer, to, totalsizeread, contentlength, timerdownload, xevent))
                                                               {
                                                                 status = false;
                                                                 break;
                                                               }

                                                             if(sizeread > contentremaining)
                                                               {
                                                                 status = false;
                                                                 break;
                                                               }

                                                             contentremaining -= sizeread;
                                                             if(!contentremaining) completed = true;
                                                             break;

              case DIOWEBCLIENT_BODYMODE_CHUNKED          : if(!chunkinput.Add(buffer, sizeread))
                                                               {
                                                                 status = false;
                                                                 break;
                                                               }

                                                             while(status && !completed)
                                                               {
                                                                 XDWORD                    inputsizebefore      = chunkinput.GetSize();
                                                                 XQWORD                    chunkremainingbefore = chunkremaining;
                                                                 DIOWEBCLIENT_CHUNKEDSTATE chunkstatebefore     = chunkstate;

                                                                 switch(chunkstate)
                                                                   {
                                                                     case DIOWEBCLIENT_CHUNKEDSTATE_SIZE     : { DIOWEBCLIENT_CHUNKEDRESULT result = ChunkSize_Get(chunkinput, chunkremaining);

                                                                                                                   if(result == DIOWEBCLIENT_CHUNKEDRESULT_ERROR)
                                                                                                                     {
                                                                                                                       status = false;
                                                                                                                       break;
                                                                                                                     }

                                                                                                                   if(result == DIOWEBCLIENT_CHUNKEDRESULT_INCOMPLETE) break;

                                                                                                                   chunkstate = chunkremaining?DIOWEBCLIENT_CHUNKEDSTATE_DATA:DIOWEBCLIENT_CHUNKEDSTATE_TRAILERS;
                                                                                                                 }
                                                                                                                 break;

                                                                     case DIOWEBCLIENT_CHUNKEDSTATE_DATA     : { if(chunkinput.IsEmpty()) break;

                                                                                                                   XDWORD blocksize = chunkinput.GetSize();
                                                                                                                   if(chunkremaining < blocksize) blocksize = (XDWORD)chunkremaining;

                                                                                                                   if(!BodyBlock_Write(chunkinput.Get(), blocksize, istobuffer, to, totalsizeread, 0, timerdownload, xevent))
                                                                                                                     {
                                                                                                                       status = false;
                                                                                                                       break;
                                                                                                                     }

                                                                                                                   chunkinput.Extract(NULL, 0, blocksize);
                                                                                                                   chunkremaining -= blocksize;

                                                                                                                   if(!chunkremaining) chunkstate = DIOWEBCLIENT_CHUNKEDSTATE_DATAEND;
                                                                                                                 }
                                                                                                                 break;

                                                                     case DIOWEBCLIENT_CHUNKEDSTATE_DATAEND  : if(chunkinput.GetSize() < 2) break;

                                                                                                                 if((chunkinput.GetByte(0) != 0x0D) || (chunkinput.GetByte(1) != 0x0A))
                                                                                                                   {
                                                                                                                     status = false;
                                                                                                                     break;
                                                                                                                   }

                                                                                                                 chunkinput.Extract(NULL, 0, 2);
                                                                                                                 chunkstate = DIOWEBCLIENT_CHUNKEDSTATE_SIZE;
                                                                                                                 break;

                                                                     case DIOWEBCLIENT_CHUNKEDSTATE_TRAILERS : { int lineend = XBUFFER_INVALIDPOSITION;

                                                                                                                   for(XDWORD c=0; (c+1)<chunkinput.GetSize(); c++)
                                                                                                                     {
                                                                                                                       if((chunkinput.GetByte(c) == 0x0D) && (chunkinput.GetByte(c+1) == 0x0A))
                                                                                                                         {
                                                                                                                           lineend = c;
                                                                                                                           break;
                                                                                                                         }
                                                                                                                     }

                                                                                                                   if(lineend == XBUFFER_INVALIDPOSITION)
                                                                                                                     {
                                                                                                                       if(chunkinput.GetSize() > DIOWEBHEADER_MAXLINE) status = false;
                                                                                                                       break;
                                                                                                                     }

                                                                                                                   if(!lineend)
                                                                                                                     {
                                                                                                                       chunkinput.Extract(NULL, 0, 2);

                                                                                                                       if(!chunkinput.IsEmpty())
                                                                                                                         {
                                                                                                                           status = false;
                                                                                                                           break;
                                                                                                                         }

                                                                                                                       chunkstate = DIOWEBCLIENT_CHUNKEDSTATE_END;
                                                                                                                       completed  = true;
                                                                                                                       break;
                                                                                                                     }

                                                                                                                   bool hasseparator = false;

                                                                                                                   for(int c=1; c<lineend; c++)
                                                                                                                     {
                                                                                                                       if(chunkinput.GetByte(c) == _C(':'))
                                                                                                                         {
                                                                                                                           hasseparator = true;
                                                                                                                           break;
                                                                                                                         }
                                                                                                                     }

                                                                                                                   if(!hasseparator)
                                                                                                                     {
                                                                                                                       status = false;
                                                                                                                       break;
                                                                                                                     }

                                                                                                                   chunkinput.Extract(NULL, 0, lineend+2);
                                                                                                                 }
                                                                                                                 break;

                                                                     case DIOWEBCLIENT_CHUNKEDSTATE_END      : completed = true;
                                                                                                                 break;
                                                                   }

                                                                 if(!status || completed) break;

                                                                 if((chunkinput.GetSize() == inputsizebefore) &&
                                                                    (chunkremaining == chunkremainingbefore) &&
                                                                    (chunkstate == chunkstatebefore)) break;

                                                                 if((chunkstate == DIOWEBCLIENT_CHUNKEDSTATE_SIZE) && chunkinput.IsEmpty()) break;
                                                                 if((chunkstate == DIOWEBCLIENT_CHUNKEDSTATE_DATA) && chunkinput.IsEmpty()) break;
                                                                 if((chunkstate == DIOWEBCLIENT_CHUNKEDSTATE_DATAEND) && (chunkinput.GetSize() < 2)) break;
                                                                 if((chunkstate == DIOWEBCLIENT_CHUNKEDSTATE_TRAILERS) && (chunkinput.GetSize() < 2)) break;
                                                               }
                                                             break;

              case DIOWEBCLIENT_BODYMODE_CONNECTIONCLOSE : if(!BodyBlock_Write(buffer, sizeread, istobuffer, to, totalsizeread, 0, timerdownload, xevent)) status = false;
                                                             break;

                                                  default : status = false;
                                                             break;
            }
        }
       else
        {
          if(timeout && (timerout->GetMeasureSeconds() > (XDWORD)timeout))
            {
              status = false;
              break;
            }

          if(diostream->GetStatus() != DIOSTREAMSTATUS_CONNECTED)
            {
              if((bodymode == DIOWEBCLIENT_BODYMODE_CONNECTIONCLOSE) && !isTLS) completed = true;
               else status = false;

              break;
            }

          GEN_XSLEEP.MilliSeconds(1);
        }
    }

  GEN_DELETE_ARRAY buffer;
  GEN_XFACTORY.DeleteTimer(timerdownload);

  return status && completed;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOWEBCLIENT::Body_Decompress(bool istobuffer, void* to)
* @brief      Transparently decompress a response body already written to "to" when the server sent a
*             Content-Encoding (gzip/deflate) this client understands
* @ingroup    DATAIO
*
* @param[in]  istobuffer : true when the body was written to an XBUFFER; false when it was written to an XFILE.
* @param[in]  to : Destination that received the raw (still encoded) body; replaced in place with the decoded body.
*
* @return     bool : true when there was nothing to decompress, or decompression succeeded; false when the
*                    response declared an encoding this client understands but decompressing it failed (a
*                    truncated or corrupt body): "to" is left untouched in that case.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBCLIENT::Body_Decompress(bool istobuffer, void* to)
{
  #ifdef COMPRESS_ACTIVE

  if(!contentencodingactive || !to) return true;

  XSTRING contentencoding;
  if(!header.GetContentEncoding(contentencoding)) return true;

  COMPRESSBASE_TYPE type;

  if(!contentencoding.Compare(_L("gzip"), true) || !contentencoding.Compare(_L("x-gzip"), true))
    {
      type = COMPRESSBASE_TYPE_GZ;
    }
   else if(!contentencoding.Compare(_L("deflate"), true))
    {
      type = COMPRESSBASE_TYPE_DEFLATE;
    }
   else
    {
      return true;                                                             // "identity", "br", ... : not ours
    }

  if(istobuffer && !((XBUFFER*)to)->GetSize()) return true;                    // nothing to decompress
  if(!istobuffer && !((XFILE*)to)->GetSize())  return true;

  COMPRESSBASE* compressor = compressmanager?compressmanager->Create(type):NULL;

  if(!compressor) return true;

  if(compressor->GetType() != type)
    {
      // The requested compressor module (COMPRESS_GZ_ACTIVE / COMPRESS_DEFLATE_ACTIVE) is not compiled into
      // this build: COMPRESSMANAGER::Create() silently fell back to a plain COMPRESSBASE. Leave the body as-is
      // (still encoded) rather than pretend to have decompressed it.
      GEN_DELETE compressor;
      return true;
    }

  bool status;

  if(istobuffer)
    {
      XBUFFER* buffer = (XBUFFER*)to;
      XBUFFER  decompressed;

      status = compressor->Decompress(buffer->Get(), buffer->GetSize(), &decompressed);

      if(status)
        {
          buffer->Delete();
          status = decompressed.IsEmpty() || buffer->Add(decompressed.Get(), decompressed.GetSize());
        }
    }
   else
    {
      XFILE* file = (XFILE*)to;

      // The whole (still compressed) file is re-read into memory to decompress it, so this path is bounded to
      // whatever fits in an XDWORD-sized XBUFFER; a compressed download at or beyond that size is left encoded
      // on disk rather than risk a truncated read.
      XQWORD filesize = file->GetSize();

      if(filesize > (XQWORD)(XDWORD)-1)
        {
          GEN_DELETE compressor;
          return true;
        }

      XBUFFER raw;
      XBUFFER decompressed;

      status = raw.Resize((XDWORD)filesize) && file->SetPosition(0) && file->Read(raw.Get(), (XDWORD)filesize) &&
               compressor->Decompress(raw.Get(), raw.GetSize(), &decompressed);

      if(status)
        {
          status = file->SetPosition(0) && file->SetSize(0) &&
                   (decompressed.IsEmpty() || file->Write(decompressed.Get(), decompressed.GetSize()));
        }
    }

  GEN_DELETE compressor;

  return status;

  #else

  return true;

  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBCLIENT::MakeOperation(DIOWEBHEADER_METHOD method, DIOURL& url, XBUFFER* postdata, XCHAR* addhead, int timeout, XSTRING* localIP, bool istobuffer, void* to)
* @brief      Make operation
* @ingroup    DATAIO
* 
* @param[in]  method : Method descriptor to call.
* @param[in]  url : URL to use.
* @param[in]  postdata : Postdata pointer to use.
* @param[in]  addhead : Addhead pointer to use.
* @param[in]  timeout : Timeout value.
* @param[in]  localIP : Local IP pointer to use.
* @param[in]  istobuffer : Istobuffer value.
* @param[in]  to : To pointer to use.
* @param[in]  redirectcount : Redirectcount value.
* @param[in]  internaloperation : Internaloperation value.
* @param[in]  inheritedtimer : Pointer to inheritedtimer.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBCLIENT::MakeOperation(DIOWEBHEADER_METHOD method, DIOURL& url, XBUFFER* postdata, XCHAR* addhead, int timeout, XSTRING* localIP, bool istobuffer, void* to, int redirectcount, bool internaloperation, XTIMER* inheritedtimer)
{
  XTIMER operationtimer;
  XTIMER* timer = inheritedtimer?inheritedtimer:&operationtimer;
  if(!inheritedtimer) operationtimer.Reset();
  if(!internaloperation) OperationError_Reset();

  if(!diostreamcfg || !timerout || !to)
    {
      OperationError_Set(DIOWEBCLIENT_ERROR_INVALIDARGUMENT);
      return false;
    }

  bool explicitHTTP  = (url.Find(DIOURL_WEBURLID, true) == 0);
  bool explicitHTTPS = (url.Find(DIOURL_WEBURLID_SECURE, true) == 0);

  // http:// and https:// are mandatory when explicitly present. Only a genuinely scheme-less URL is governed by
  // transportpolicy. Reject another explicit URI scheme instead of accidentally prefixing it with https://.
  if(!explicitHTTP && !explicitHTTPS)
    {
      DIOURL targeturl;

      if(url.Find(_L("://"), true) != XSTRING_NOTFOUND)
        {
          OperationError_Set(DIOWEBCLIENT_ERROR_INVALIDURL);
          return false;
        }

      if(transportpolicy == DIOWEBCLIENT_TRANSPORTPOLICY_HTTP_ONLY)
        {
          targeturl  = DIOURL_WEBURLID;
          targeturl += url.Get();
          int remaining=timeout; if(timeout!=XTIMER_INFINITE) { XDWORD e=timer->GetMeasureSeconds(); if(e>=(XDWORD)timeout) return false; remaining=timeout-(int)e; }
          return MakeOperation(method, targeturl, postdata, addhead, remaining, localIP, istobuffer, to, 0, true, timer);
        }

      targeturl  = DIOURL_WEBURLID_SECURE;
      targeturl += url.Get();

      if(transportpolicy == DIOWEBCLIENT_TRANSPORTPOLICY_HTTPS_ONLY)
        {
          int remaining=timeout; if(timeout!=XTIMER_INFINITE) { XDWORD e=timer->GetMeasureSeconds(); if(e>=(XDWORD)timeout) return false; remaining=timeout-(int)e; }
          return MakeOperation(method, targeturl, postdata, addhead, remaining, localIP, istobuffer, to, 0, true, timer);
        }

      // Preserve the exact HTTPS failure before considering a weaker transport. Authentication, DNS, local
      // configuration and every failure after an HTTP request was sent are authoritative and never permit fallback.
      int remaining=timeout; if(timeout!=XTIMER_INFINITE) { XDWORD e=timer->GetMeasureSeconds(); if(e>=(XDWORD)timeout) return false; remaining=timeout-(int)e; }
      if(MakeOperation(method, targeturl, postdata, addhead, remaining, localIP, istobuffer, to, 0, true, timer)) return true;

      lastHTTPSattempterror = lastoperationerror;
      if(!OperationError_AllowsHTTPFallback()) return false;

      targeturl  = DIOURL_WEBURLID;
      targeturl += url.Get();
      lastoperationerror.Clean();
      HTTPfallbackused = true;
      int remaininghttp=timeout; if(timeout!=XTIMER_INFINITE) { XDWORD e=timer->GetMeasureSeconds(); if(e>=(XDWORD)timeout) return false; remaininghttp=timeout-(int)e; }
      return MakeOperation(method, targeturl, postdata, addhead, remaininghttp, localIP, istobuffer, to, 0, true, timer);
    }

  if(istobuffer) ((XBUFFER*)to)->Delete();

  bool  status          = true;
  bool  isTLS           = IsSecureURL(url);
  int   defaultport     = isTLS?DIOWEBCLIENT_DEFAULTSECUREPORT:DIOWEBCLIENT_DEFAULTPORT;
  int   operationport;

  XSTRING               server;
  DIOURL                resource;
  XSTRING               methodstring;
  DIOWEBCLIENT_XEVENT   xevent(this, XEVENT_TYPE_NONE);

  xevent.GetURL()->Set(url.Get());
  xevent.SetMethod(method);

  diostreamcfg->SetRemotePort(0);
  if(!diostreamcfg->SetFromString(url.Get()))
    {
      OperationError_Set(DIOWEBCLIENT_ERROR_INVALIDURL);
      return false;
    }

  url  = diostreamcfg->GetRemoteURL()->Get();
  if(diostreamcfg->GetRemotePort()) operationport = diostreamcfg->GetRemotePort();
                              else operationport = isportconfigured?port:defaultport;

  if(!isportconfigured) port = operationport;

  url.GetHTTPServer(server, login, password);
  url.GetHTTPResource(resource);

  if(resource.IsEmpty()) resource.Slash_Add();

  diostreamcfg->SetMode(DIOSTREAMMODE_CLIENT);
  if(localIP)
    {
      if(!localIP->IsEmpty()) diostreamcfg->GetLocalIP()->Set(localIP->Get());
    }

  diostreamcfg->GetRemoteURL()->Set(server.Get());
  diostreamcfg->SetRemotePort(operationport);

  DIOSTREAMTCPIPPROXYCFG* proxycfg = diostreamcfg->GetProxyCFG();
  if(proxycfg && !proxycfg->GetURL()->IsEmpty() && (proxycfg->GetPort() > 0))
    {
      if(proxycfg->GetType() == DIOSTREAMTCPIPPROXYTYPE_NONE) proxycfg->SetType(DIOSTREAMTCPIPPROXYTYPE_HTTP);
      proxycfg->SetMode(isTLS?DIOSTREAMTCPIPPROXYMODE_TUNNEL:DIOSTREAMTCPIPPROXYMODE_FORWARD);
    }
   else
    {
      if(proxycfg) proxycfg->SetMode(DIOSTREAMTCPIPPROXYMODE_NONE);
    }

  #ifdef DIO_STREAMTLS_ACTIVE

  if(isTLS)
    {
      DIOSTREAMTLSCONFIG* tlsconfig = GetStreamTLSCFG();
      XBUFFER             applicationprotocol;
      static const XBYTE  HTTP11ALPN[] = { 'h','t','t','p','/','1','.','1' };

      if(!tlsconfig)
        {
          OperationError_Set(DIOWEBCLIENT_ERROR_TLSCONFIGURATION, DIOSTREAMERROR_TLSCONFIGURATION);
          return false;
        }

      // The TLS stream derives SNI and the certificate name from RemoteURL.  Keeping
      // ServerName empty is intentional: it makes the host connection-specific and
      // avoids mutating frozen shared configuration on redirects or client reuse.
      if((tlsconfig->GetApplicationProtocolsCount() != 1) ||
         !tlsconfig->GetApplicationProtocol(0, applicationprotocol) ||
         (applicationprotocol.GetSize() != sizeof(HTTP11ALPN)) ||
         memcmp(applicationprotocol.Get(), HTTP11ALPN, sizeof(HTTP11ALPN)))
        {
          OperationError_Set(DIOWEBCLIENT_ERROR_TLSCONFIGURATION, DIOSTREAMERROR_TLSCONFIGURATION);
          return false;
        }
    }

  #endif

  if(!Stream_Create(isTLS))
    {
      #ifndef DIO_STREAMTLS_ACTIVE

      OperationError_Set(isTLS?DIOWEBCLIENT_ERROR_TRANSPORTUNAVAILABLE:DIOWEBCLIENT_ERROR_TCPCONNECTION);

      #else

      OperationError_Set(isTLS?DIOWEBCLIENT_ERROR_TLSCONFIGURATION:DIOWEBCLIENT_ERROR_TCPCONNECTION, isTLS?DIOSTREAMERROR_TLSCONFIGURATION:DIOSTREAMERROR_TCPCONNECTION);
      #endif

      return false;
    }

  //--- Connection WEB server -------------------

  if(!diostream->Open())
    {
      OperationError_FromStream(isTLS?DIOWEBCLIENT_ERROR_TLSPROTOCOL:DIOWEBCLIENT_ERROR_TCPCONNECTION);
      diostream->Close();
      return false;
    }

  if(!diostream->WaitToConnected(timeout))
    {
      if(diostream->PeekLastDIOError() == DIOSTREAMERROR_NONE)
        {
          diostream->SetLastDIOError(DIOSTREAMERROR_TCPTIMEOUT);
        }
      OperationError_FromStream(DIOWEBCLIENT_ERROR_TCPTIMEOUT);
      diostream->Close();
      return false;
    }

  xevent.SetEventType(DIOWEBCLIENT_XEVENT_TYPE_OPENWEB);
  PostEvent(&xevent);

  //--- Send Header ----------------------------

  XSTRING sendheader;

  //resource.EncodeUnsafeChars();

  switch(method)
    {
      case DIOWEBHEADER_METHOD_GET  : methodstring = _L("GET");     break;
      case DIOWEBHEADER_METHOD_PUT  : methodstring = _L("PUT");     break;
      case DIOWEBHEADER_METHOD_POST : methodstring = _L("POST");    break;

                            default : { xevent.SetEventType(DIOWEBCLIENT_XEVENT_TYPE_HEADERERROR);
                                        PostEvent(&xevent);

                                        OperationError_Set(DIOWEBCLIENT_ERROR_INVALIDARGUMENT);
                                        diostream->Close();
                                      }
                                      return false;
    }


  XSTRING requestresource;

  if(proxycfg && proxycfg->IsActive() && (proxycfg->GetMode() == DIOSTREAMTCPIPPROXYMODE_FORWARD))
    {
      requestresource = _L("http://");
      requestresource += server;

      if(operationport != DIOWEBCLIENT_DEFAULTPORT) requestresource.AddFormat(_L(":%d"), operationport);

      if(resource.IsEmpty() || (resource.Get()[0] != _C('/'))) requestresource += _L("/");
      requestresource += resource;
    }
   else
    {
      requestresource = resource;
    }

  sendheader.AddFormat(_L("%s %s HTTP/1.1\r\n"), methodstring.Get(), requestresource.Get());

  sendheader += _L("Host: ");
  sendheader += server.Get();

  if(operationport!=defaultport)
    {
      XSTRING stringport;

      stringport.ConvertFromInt(operationport);

      sendheader += _L(":");
      sendheader += stringport.Get();
    }
  sendheader += _L("\r\n");

  if(proxycfg && proxycfg->IsActive() && (proxycfg->GetMode() == DIOSTREAMTCPIPPROXYMODE_FORWARD) &&
     (!proxycfg->GetLogin()->IsEmpty() || !proxycfg->GetPassword()->IsEmpty()))
    {
      XSTRING proxycredentials;
      XSTRING proxycredentialsbase64;

      proxycredentials.Format(_L("%s:%s"), proxycfg->GetLogin()->Get(), proxycfg->GetPassword()->Get());
      proxycredentials.ConvertToBase64(proxycredentialsbase64);

      sendheader += _L("Proxy-Authorization: Basic ");
      sendheader += proxycredentialsbase64;
      sendheader += _L("\r\n");
    }

  if(addhead) sendheader += addhead;

  // Some servers/CDNs/WAFs treat a request with no User-Agent at all as non-browser/bot traffic and respond
  // differently (a canonical-redirect loop has been observed against at least one real site) -- send the
  // default identification unless the caller already supplied their own via addhead.
  if(sendheader.Find(_L("User-Agent:"), true) == XSTRING_NOTFOUND) sendheader += DIOWEBCLIENT_DEFAULTUSERAGENT;

  #ifdef COMPRESS_ACTIVE

  if(contentencodingactive)
    {
      bool hasacceptencoding = false;

      if(addhead)
        {
          XSTRING addheadstring;

          addheadstring = addhead;
          hasacceptencoding = (addheadstring.Find(DIOWEBHEADER_ACCEPT_ENCODING, true) != XSTRING_NOTFOUND);
        }

      if(!hasacceptencoding) sendheader += _L("Accept-Encoding: gzip, deflate\r\n");
    }

  #endif


  switch(authenticationmethod)
    {
      case DIOWEBCLIENT_AUTHENTICATION_METHOD_UNKNOWN : if(password.IsEmpty()) break;
                                                        authenticationmethod = DIOWEBCLIENT_AUTHENTICATION_METHOD_BASIC;

      case DIOWEBCLIENT_AUTHENTICATION_METHOD_BASIC   : { XSTRING loginpassword;
                                                          XSTRING loginpasswordbase64;

                                                          loginpassword.Format(_L("%s:%s"), login.Get(), password.Get());
                                                          loginpassword.ConvertToBase64(loginpasswordbase64);

                                                          sendheader += _L("Authorization: Basic ");
                                                          sendheader += loginpasswordbase64;
                                                          sendheader += _L("\r\n");
                                                        }
                                                        break;

      case DIOWEBCLIENT_AUTHENTICATION_METHOD_DIGEST  : break;
    }


  // effectivepostdata is postdata itself unless CompressRequestBody_Activate(true) is set and gzip-compressing
  // it actually succeeds, in which case it points at compressedpostdata and a matching Content-Encoding is sent.
  // It is what actually goes out on the wire below (and again on the digest-auth retry further down), so the
  // Content-Length advertised here always matches the bytes written.
  XBUFFER  compressedpostdata;
  XBUFFER* effectivepostdata = postdata;

  #ifdef COMPRESS_ACTIVE

  if(compressrequestbody && postdata && !postdata->IsEmpty())
    {
      COMPRESSBASE* compressor = compressmanager?compressmanager->Create(COMPRESSBASE_TYPE_GZ):NULL;

      if(compressor)
        {
          if((compressor->GetType() == COMPRESSBASE_TYPE_GZ) &&
             compressor->Compress(postdata->Get(), postdata->GetSize(), &compressedpostdata))
            {
              effectivepostdata = &compressedpostdata;
            }

          GEN_DELETE compressor;
        }
    }

  #endif

  if(effectivepostdata)
    {
      XSTRING stringlenght;
      stringlenght.Format(_L("%s: %d\r\n"), DIOWEBHEADER_CONTENT_LENGTH, effectivepostdata->GetSize());

      sendheader += stringlenght;

      if(effectivepostdata == &compressedpostdata) sendheader += _L("Content-Encoding: gzip\r\n");
    }

  sendheader += _L("Connection: close\r\n");
  sendheader += _L("\r\n");

  if(!diostream->WriteStr(sendheader))
    {
      OperationError_FromStream(DIOWEBCLIENT_ERROR_HTTPWRITE);
      diostream->Close();
      return false;
    }

  //XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("%s"), sendheader.Get());

  xevent.SetEventType(DIOWEBCLIENT_XEVENT_TYPE_WRITEHEADER);
  PostEvent(&xevent);

  //--- Send Post data -------------------------

  if(effectivepostdata)
    {
      if(diostream->Write(effectivepostdata->Get(), effectivepostdata->GetSize()) != effectivepostdata->GetSize())
        {
          OperationError_FromStream(DIOWEBCLIENT_ERROR_HTTPWRITE);
          diostream->Close();
          return false;
        }

      xevent.SetEventType(DIOWEBCLIENT_XEVENT_TYPE_SENDPOSTDATA);
      PostEvent(&xevent);
    }

  if(!diostream->WaitToFlushOutXBuffer(timeout))
    {
      OperationError_FromStream(DIOWEBCLIENT_ERROR_HTTPWRITE);
      diostream->Close();
      return false;
    }

  //--- Read Header ----------------------------

  if(!Header_Read(timeout))
    {
      OperationError_FromStream(DIOWEBCLIENT_ERROR_HTTPRESPONSE);
      diostream->Close();
      return false;
    }


  //--- Read Content ---------------------------

  { int resultserver = header.GetResultServer();

  if((resultserver == 301) || (resultserver == 302) || (resultserver == 303) ||
     (resultserver == 307) || (resultserver == 308))
    {
      // Only GET/HEAD are auto-followed: a redirect must preserve the method per RFC 7231, and blindly
      // replaying a POST/PUT body against a redirected URL risks an unintended duplicate side effect.
      // 307/308 require the method AND body to stay identical on redirect, which is automatically true
      // here since only bodyless GET/HEAD are ever auto-followed in the first place. The redirect chain
      // is bounded, and a missing/empty Location simply falls through unchanged to the existing
      // behaviour below (the redirect response is then read/returned as-is).
      XSTRING location;


      if(((method == DIOWEBHEADER_METHOD_GET) || (method == DIOWEBHEADER_METHOD_HEAD)) &&
         (redirectcount < DIOWEBCLIENT_MAXREDIRECTS) && header.GetLocation(location))
        {
          DIOURL redirecturl;

          if(location.Find(_L("://"), true) != XSTRING_NOTFOUND)
            {
              redirecturl = location.Get();                                   // Absolute Location: use it as-is
            }
           else
            {
              XSTRING scheme;

              scheme = isTLS?_L("https://"):_L("http://");

              redirecturl  = scheme;
              redirecturl += server;

              if(operationport != defaultport)
                {
                  XSTRING stringport;

                  stringport.ConvertFromInt(operationport);

                  redirecturl += _L(":");
                  redirecturl += stringport;
                }

              if(!location.IsEmpty() && (location.Get()[0] != _C('/'))) redirecturl += _L("/");

              redirecturl += location;
            }

          // Force a full teardown-and-recreate of the transport for the retry, instead of leaving
          // Stream_Create() to reuse this same diostream instance (which it does whenever the redirect
          // target keeps the same scheme, i.e. isTLS unchanged). A platform stream implementation is not
          // guaranteed to support being Open()'d again on the very same object instance after a prior
          // Close() -- reconnecting via a brand-new instance sidesteps any such reuse bug entirely, at
          // the cost of one extra allocation per redirect hop (redirect chains are already bounded by
          // DIOWEBCLIENT_MAXREDIRECTS).
          bool redirectisTLS = IsSecureURL(redirecturl);
          if(isTLS && !redirectisTLS && !allowinsecureredirect)
            {
              OperationError_Set(DIOWEBCLIENT_ERROR_HTTPREDIRECT, DIOSTREAMERROR_NONE, resultserver);
              return false;
            }

          bool    sameorigin      = RedirectOrigin_IsSame(url, redirecturl);
          XSTRING filteredheaders;
          XCHAR*  redirectheaders = addhead;

          if(addhead && !sameorigin)
            {
              if(!Headers_FilterSensitive(addhead, filteredheaders))
                {
                  OperationError_Set(DIOWEBCLIENT_ERROR_HTTPREDIRECT, DIOSTREAMERROR_NONE, resultserver);
                  return false;
                }
              redirectheaders = filteredheaders.IsEmpty()?NULL:filteredheaders.Get();
            }

          XSTRING                            savedlogin;
          XSTRING                            savedpassword;
          DIOWEBCLIENT_AUTHENTICATION_METHOD savedauthenticationmethod = authenticationmethod;

          if(!sameorigin)
            {
              savedlogin     = login;
              savedpassword  = password;
              login.Empty();
              password.Empty();
              authenticationmethod = DIOWEBCLIENT_AUTHENTICATION_METHOD_UNKNOWN;
            }

          diostream->Close();
          GEN_DIOFACTORY.DeleteStreamIO(diostream);
          diostream = NULL;

          int remaining=timeout; if(timeout!=XTIMER_INFINITE) { XDWORD e=timer->GetMeasureSeconds(); if(e>=(XDWORD)timeout) return false; remaining=timeout-(int)e; }
          bool redirectstatus = MakeOperation(method, redirecturl, postdata, redirectheaders, remaining, localIP, istobuffer, to, redirectcount+1, true, timer);

          if(!sameorigin)
            {
              login                = savedlogin;
              password             = savedpassword;
              authenticationmethod = savedauthenticationmethod;
            }

          return redirectstatus;
        }
    }
  }

  if((header.GetResultServer() == 401) && (authenticationmethod == DIOWEBCLIENT_AUTHENTICATION_METHOD_DIGEST))
    {
      XSTRING   www_authenticate;
      HASHMD5   md5;
      XSTRING   HA1;
      XSTRING   HA2;
      XSTRING   calculatehash;
      XSTRING   realm_str;
      XSTRING   nonce_str;
      XSTRING   auth_str;
      XDWORD    nc;
      XDWORD    cnonce;
      XSTRING   nc_str;
      XSTRING   cnonce_str;
      XSTRING   opaque_str;
      XSTRING   algorithm_str;
      XSTRING   response;
      
      sendheader.DeleteCharacters(sendheader.GetSize()-2, 2);

      diostream->Close();

      if(!diostream->Open())
        {
          OperationError_FromStream(isTLS?DIOWEBCLIENT_ERROR_TLSPROTOCOL:DIOWEBCLIENT_ERROR_TCPCONNECTION);
          diostream->Close();
          return false;
        }

      if(!diostream->WaitToConnected(timeout))
        {
          if(diostream->PeekLastDIOError() == DIOSTREAMERROR_NONE)
            {
              diostream->SetLastDIOError(DIOSTREAMERROR_TCPTIMEOUT);
            }
          OperationError_FromStream(DIOWEBCLIENT_ERROR_TCPTIMEOUT);
          diostream->Close();
          return false;
        }

      xevent.SetEventType(DIOWEBCLIENT_XEVENT_TYPE_OPENWEB);
      PostEvent(&xevent);


      auth_str    = _L("auth");
      nc          = 0x0000001;
      cnonce      = 0x0AF113B;

      nc_str.Format(_L("%08x"), nc);
      cnonce_str.Format(_L("%08x"), cnonce);
      
       
      header.GetWWWAuthenticate(www_authenticate);
      
      //XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("%s"), www_authenticate.Get());

      GetSubStringWWWWAuthenticate(www_authenticate, _L("realm"), realm_str);
      GetSubStringWWWWAuthenticate(www_authenticate, _L("nonce"), nonce_str);
      GetSubStringWWWWAuthenticate(www_authenticate, _L("opaque"), opaque_str);
      GetSubStringWWWWAuthenticate(www_authenticate, _L("algorithm") , algorithm_str, false);

      /*
      if(algorithm_str.Compare(_L("MD5"), true))
        {
          diostream->Close();
          return false;
        }    
      */
     
      { calculatehash.Format(_L("%s:%s:%s"), login.Get(), realm_str.Get(), password.Get());
       
        XBUFFER charhash;
        
        calculatehash.ConvertToASCII(charhash);       
        md5.Do((XBYTE*)charhash.Get(), calculatehash.GetSize());
        
        md5.GetResultString(HA1);
      }

      md5.ResetResult();

      { calculatehash.Format(_L("%s:%s"), methodstring.Get(), resource.Get());
  
        XBUFFER charhash;
        
        calculatehash.ConvertToASCII(charhash);       
        md5.Do((XBYTE*)charhash.Get(), calculatehash.GetSize());
        
        md5.GetResultString(HA2);
      }

      md5.ResetResult();  

      { calculatehash.Format(_L("%s:%s:%s:%s:%s:%s"), HA1.Get(), nonce_str.Get(), nc_str.Get(), cnonce_str.Get(), auth_str.Get(), HA2.Get());
  
        XBUFFER charhash;
        
        calculatehash.ConvertToASCII(charhash);       
        md5.Do((XBYTE*)charhash.Get(), calculatehash.GetSize());
        
        md5.GetResultString(response);
      }
     
   
      sendheader.AddFormat(_L("Authorization: Digest "));     
      sendheader.AddFormat(_L("username=\"%s\", "), login.Get());           
      sendheader.AddFormat(_L("realm=\"%s\", "), realm_str.Get());           
      sendheader.AddFormat(_L("nonce=\"%s\", "), nonce_str.Get());           
      sendheader.AddFormat(_L("uri=\"%s\", "), resource.Get());     
      sendheader.AddFormat(_L("algorithm=%s, "), algorithm_str.Get());
      sendheader.AddFormat(_L("response=\"%s\", "), response.Get());   
      sendheader.AddFormat(_L("qop=%s, "), auth_str.Get());     
      sendheader.AddFormat(_L("nc=%s, "), nc_str.Get());     
      sendheader.AddFormat(_L("cnonce=\"%s\" "), cnonce_str.Get());   
      if(opaque_str.GetSize()) sendheader.AddFormat(_L(", opaque=\"%s\""), opaque_str.Get());           
      sendheader += _L("\r\n");

      sendheader += _L("\r\n");
      
      //XTRACE_PRINTCOLOR(XTRACE_COLOR_PURPLE, _L("%s"), sendheader.Get());

      if(!diostream->WriteStr(sendheader))
        {
          OperationError_FromStream(DIOWEBCLIENT_ERROR_HTTPWRITE);
          diostream->Close();
          return false;
        }

      xevent.SetEventType(DIOWEBCLIENT_XEVENT_TYPE_WRITEHEADER);
      PostEvent(&xevent);

      //--- Send Post data -------------------------

      if(effectivepostdata)
        {
          if(diostream->Write(effectivepostdata->Get(), effectivepostdata->GetSize()) != effectivepostdata->GetSize())
            {
              OperationError_FromStream(DIOWEBCLIENT_ERROR_HTTPWRITE);
              diostream->Close();
              return false;
            }

          xevent.SetEventType(DIOWEBCLIENT_XEVENT_TYPE_SENDPOSTDATA);
          PostEvent(&xevent);
        }

      if(!diostream->WaitToFlushOutXBuffer(timeout))
        {
          OperationError_FromStream(DIOWEBCLIENT_ERROR_HTTPWRITE);
          diostream->Close();
          return false;
        }

      //--- Read Header ----------------------------

      if(!Header_Read(timeout))
        {
          OperationError_FromStream(DIOWEBCLIENT_ERROR_HTTPRESPONSE);
          diostream->Close();
          return false;
        }
    }


  if((header.GetResultServer()>=400)||(!header.GetResultServer()))
    {     
      xevent.SetEventType(DIOWEBCLIENT_XEVENT_TYPE_HEADERERROR);
      PostEvent(&xevent);

      if(IsActiveDoStopHTTPError())
        {
          if(proxycfg && proxycfg->IsActive() && (header.GetResultServer() == 407))
            {
              OperationError_Set(DIOWEBCLIENT_ERROR_PROXY, DIOSTREAMERROR_TCPIPPROXY, header.GetResultServer());
            }
           else OperationError_Set(DIOWEBCLIENT_ERROR_HTTPSTATUS, DIOSTREAMERROR_NONE, header.GetResultServer());
          diostream->Close();
          return false;
        }
    }

  XQWORD                    contentlength        = 0;
  XSTRING                   transferencoding;
  XSTRING                   etag;
  DIOWEBCLIENT_BODYMODE     bodymode             = DIOWEBCLIENT_BODYMODE_UNKNOWN;
  int                       resultserver         = header.GetResultServer();
  bool                      hascontentlength     = header.HasContentLength();
  bool                      validcontentlength   = header.GetContentLength(contentlength);
  bool                      hastransferencoding  = header.GetTransferEncoding(transferencoding);
  bool                      hasnobody            = ((method == DIOWEBHEADER_METHOD_HEAD) ||
                                                    ((resultserver >= 100) && (resultserver < 200)) ||
                                                    (resultserver == DIOWEBHEADER_RESULT_NOCONTENT) ||
                                                    (resultserver == DIOWEBHEADER_RESULT_RESETCONTENT) ||
                                                    (resultserver == DIOWEBHEADER_RESULT_NOTMODIFIED));

  if(hasnobody)
    {
      bodymode     = DIOWEBCLIENT_BODYMODE_NONE;
      contentlength = 0;
    }
   else if(hastransferencoding)
    {
      transferencoding.DeleteCharacter(_C(' '));
      transferencoding.DeleteCharacter(_C('\t'));

      if(hascontentlength || transferencoding.Compare(_L("chunked"), true))
        {
          xevent.SetEventType(DIOWEBCLIENT_XEVENT_TYPE_HEADERERROR);
          PostEvent(&xevent);

          OperationError_Set(DIOWEBCLIENT_ERROR_HTTPRESPONSE, DIOSTREAMERROR_NONE, resultserver);
          diostream->Close();
          return false;
        }

      bodymode      = DIOWEBCLIENT_BODYMODE_CHUNKED;
      contentlength = 0;
    }
   else if(hascontentlength)
    {
      if(!validcontentlength)
        {
          xevent.SetEventType(DIOWEBCLIENT_XEVENT_TYPE_HEADERERROR);
          PostEvent(&xevent);

          OperationError_Set(DIOWEBCLIENT_ERROR_HTTPRESPONSE, DIOSTREAMERROR_NONE, resultserver);
          diostream->Close();
          return false;
        }

      bodymode = DIOWEBCLIENT_BODYMODE_CONTENTLENGTH;
    }
   else
    {
      bodymode = DIOWEBCLIENT_BODYMODE_CONNECTIONCLOSE;

      if(isTLS)
        {
          xevent.SetEventType(DIOWEBCLIENT_XEVENT_TYPE_HEADERERROR);
          PostEvent(&xevent);

          OperationError_Set(DIOWEBCLIENT_ERROR_HTTPRESPONSE, DIOSTREAMERROR_NONE, resultserver);
          diostream->Close();
          return false;
        }
    }

  header.GetETag(etag);

  xevent.SetContentLenght(contentlength);
  xevent.GetEtag()->Set(etag);

  xevent.SetEventType(DIOWEBCLIENT_XEVENT_TYPE_READHEADER);

  PostEvent(&xevent);

  //--- Read Content ---------------------------

  status = Body_Read(bodymode, isTLS, contentlength, timeout, istobuffer, to, xevent);

  if(!status)
    {
      OperationError_FromStream(DIOWEBCLIENT_ERROR_HTTPBODY);
    }
   else if(!Body_Decompress(istobuffer, to))
    {
      OperationError_Set(DIOWEBCLIENT_ERROR_HTTPCONTENTENCODING, DIOSTREAMERROR_NONE, resultserver);
      status = false;
    }

  //--- Close ----------------------------------

  diostream->Close();

  xevent.SetEventType(DIOWEBCLIENT_XEVENT_TYPE_CLOSEWEB);
  PostEvent(&xevent);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBCLIENT::Stream_Create(bool isTLS)
* @brief      Create the transport required by the URL scheme
* @ingroup    DATAIO
* 
* @param[in]  isTLS : true to create a TLS transport; false to create a TCP/IP transport.
* 
* @return     bool : true if the required transport is available; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBCLIENT::Stream_Create(bool isTLS)
{
  if(!diostreamcfg) return false;

  #ifndef DIO_STREAMTLS_ACTIVE

  if(isTLS) return false;

  #endif

  if(diostream && (isstreamTLS != isTLS))
    {
      diostream->Close();
      GEN_DIOFACTORY.DeleteStreamIO(diostream);
      diostream = NULL;
    }

  diostreamcfg->SetIsTLS(isTLS);

  #ifdef DIO_STREAMTLS_ACTIVE

  if(isTLS)
    {
      DIOSTREAMTLSCONFIG* tlsconfig = GetStreamTLSCFG();

      if(!tlsconfig) return false;
      if(tlsconfig->GetTrustedRoots()->IsEmpty())
        {
          if(!tlsconfig->TrustedRoots_AddDefaults()) return false;
        }
    }

  #endif

  if(!diostream)
    {
      diostream = (DIOSTREAMTCPIP*)GEN_DIOFACTORY.CreateStreamIO(diostreamcfg);
      if(!diostream) return false;
    }

  isstreamTLS = isTLS;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBCLIENT::IsSecureURL(DIOURL& url)
* @brief      Check whether the URL selects HTTPS
* @ingroup    DATAIO
* 
* @param[in]  url : URL to check.
* 
* @return     bool : true if the URL starts with the HTTPS identifier; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBCLIENT::IsSecureURL(DIOURL& url)
{
  return (url.Find(DIOURL_WEBURLID_SECURE, true) == 0)?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBCLIENT::GetSubStringWWWWAuthenticate(XSTRING& www_authenticate, XCHAR* field, XSTRING& value, bool betweenquotation)
* @brief      Get sub string WWWW authenticate
* @ingroup    DATAIO
* 
* @param[in]  www_authenticate : Www authenticate value.
* @param[in]  field : Field pointer to use.
* @param[in]  value : Value value.
* @param[in]  betweenquotation : Betweenquotation value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBCLIENT::GetSubStringWWWWAuthenticate(XSTRING& www_authenticate, XCHAR* field, XSTRING& value, bool betweenquotation)
{
  if(www_authenticate.IsEmpty()) return false;

  XSTRING sfield;

  sfield = field;

  int index = www_authenticate.Find(field, true);
  if(index == XSTRING_NOTFOUND) return false;
  index +=  sfield.GetSize();

  index = www_authenticate.Find(_L("="), true, index);
  if(index == XSTRING_NOTFOUND) return false;

  int start = 0;
  int end   = 0;

  if(betweenquotation)
    {
      start = www_authenticate.Find(_L("\""), true, index+1);
      if(start == XSTRING_NOTFOUND) return false;

      end = www_authenticate.Find(_L("\""), true, start+1);
      if(end == XSTRING_NOTFOUND) return false;
    }
   else
    { 
      start = index;

      end = www_authenticate.Find(_L(","), true, start+1);
      if(end == XSTRING_NOTFOUND) return false;
    }

  return www_authenticate.Copy(start+1, end, value)?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBCLIENT::RedirectOrigin_IsSame(DIOURL& first, DIOURL& second)
* @brief      Check whether two URLs belong to the same security origin
* @note       INTERNAL
* @ingroup    DATAIO
* 
* @param[in]  first : First value.
* @param[in]  second : Second value.
* 
* @return     bool : true when scheme and authority are equal; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBCLIENT::RedirectOrigin_IsSame(DIOURL& first, DIOURL& second)
{
  XSTRING firstserver;
  XSTRING firstlogin;
  XSTRING firstpassword;
  XSTRING secondserver;
  XSTRING secondlogin;
  XSTRING secondpassword;

  if(IsSecureURL(first) != IsSecureURL(second)) return false;
  if(!first.GetHTTPServer(firstserver, firstlogin, firstpassword)) return false;
  if(!second.GetHTTPServer(secondserver, secondlogin, secondpassword)) return false;

  return !firstserver.Compare(secondserver, true);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBCLIENT::Headers_FilterSensitive(XCHAR* source, XSTRING& filtered)
* @brief      Remove credential-bearing request headers before a cross-origin redirect
* @note       INTERNAL
* @ingroup    DATAIO
* 
* @param[in]  source : Pointer to source.
* @param[in]  filtered : Filtered value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBCLIENT::Headers_FilterSensitive(XCHAR* source, XSTRING& filtered)
{
  filtered.Empty();
  if(!source) return true;

  XSTRING headers;
  headers = source;

  int start = 0;
  while(start < (int)headers.GetSize())
    {
      int end = headers.Find(_L("\n"), true, start);
      if(end == XSTRING_NOTFOUND) end = headers.GetSize();

      XSTRING line;
      headers.Copy(start, end-start, line);
      if(!line.IsEmpty() && (line.Get()[line.GetSize()-1] == _C('\r'))) line.DeleteLastCharacter();

      XSTRING lowerline;
      lowerline = line;
      lowerline.ToLowerCase();

      bool sensitive = !lowerline.Find(_L("authorization:"), true) ||
                       !lowerline.Find(_L("proxy-authorization:"), true) ||
                       !lowerline.Find(_L("cookie:"), true);

      if(!sensitive && !line.IsEmpty())
        {
          filtered += line;
          filtered += _L("\r\n");
        }

      start = end + 1;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBCLIENT::OperationError_Reset()
* @brief      Operation error reset
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBCLIENT::OperationError_Reset()
{
  lastoperationerror.Clean();
  lastHTTPSattempterror.Clean();
  HTTPfallbackused = false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBCLIENT::OperationError_Set(DIOWEBCLIENT_ERROR error, DIOSTREAMERROR streamerror, int HTTPstatus)
* @brief      Operation error set
* @ingroup    DATAIO
* 
* @param[in]  error : Error value.
* @param[in]  streamerror : Streamerror value.
* @param[in]  HTTPstatus : HTTPstatus value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBCLIENT::OperationError_Set(DIOWEBCLIENT_ERROR error, DIOSTREAMERROR streamerror, int HTTPstatus)
{
  DIOWEBCLIENT_ERRORSTAGE stage = DIOWEBCLIENT_ERRORSTAGE_CONFIGURATION;

  switch(error)
    {
      case DIOWEBCLIENT_ERROR_NONE                : stage = DIOWEBCLIENT_ERRORSTAGE_NONE;                   break;
      case DIOWEBCLIENT_ERROR_DNSRESOLUTION       : stage = DIOWEBCLIENT_ERRORSTAGE_DNS;                    break;
      case DIOWEBCLIENT_ERROR_TCPCONNECTION       :
      case DIOWEBCLIENT_ERROR_TCPTIMEOUT          : stage = DIOWEBCLIENT_ERRORSTAGE_TCP;                    break;
      case DIOWEBCLIENT_ERROR_PROXY               : stage = DIOWEBCLIENT_ERRORSTAGE_PROXY;                  break;
      case DIOWEBCLIENT_ERROR_TLSPROTOCOL         : stage = DIOWEBCLIENT_ERRORSTAGE_TLSPROTOCOL;            break;
      case DIOWEBCLIENT_ERROR_TLSAUTHENTICATION   : stage = DIOWEBCLIENT_ERRORSTAGE_TLSAUTHENTICATION;      break;
      case DIOWEBCLIENT_ERROR_HTTPWRITE           :
      case DIOWEBCLIENT_ERROR_HTTPRESPONSE        :
      case DIOWEBCLIENT_ERROR_HTTPSTATUS          :
      case DIOWEBCLIENT_ERROR_HTTPREDIRECT        :
      case DIOWEBCLIENT_ERROR_HTTPBODY            :
      case DIOWEBCLIENT_ERROR_HTTPCONTENTENCODING : stage = DIOWEBCLIENT_ERRORSTAGE_HTTP;                   break;
                                             default : stage = DIOWEBCLIENT_ERRORSTAGE_CONFIGURATION;        break;
    }

  lastoperationerror.Set(stage, error, streamerror, HTTPstatus);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBCLIENT::OperationError_FromStream(DIOWEBCLIENT_ERROR defaulterror)
* @brief      Operation error from stream
* @ingroup    DATAIO
* 
* @param[in]  defaulterror : Defaulterror value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBCLIENT::OperationError_FromStream(DIOWEBCLIENT_ERROR defaulterror)
{
  DIOSTREAMERROR streamerror = diostream?diostream->PeekLastDIOError():DIOSTREAMERROR_NONE;

  switch(streamerror)
    {
      case DIOSTREAMERROR_DNS               : OperationError_Set(DIOWEBCLIENT_ERROR_DNSRESOLUTION, streamerror);       return;
      case DIOSTREAMERROR_TCPCONNECTION     :
      case DIOSTREAMERROR_ADDRINUSE         : OperationError_Set(DIOWEBCLIENT_ERROR_TCPCONNECTION, streamerror);       return;
      case DIOSTREAMERROR_TCPTIMEOUT        : OperationError_Set(DIOWEBCLIENT_ERROR_TCPTIMEOUT, streamerror);          return;
      case DIOSTREAMERROR_TCPIPPROXY        : OperationError_Set(DIOWEBCLIENT_ERROR_PROXY, streamerror);               return;
      case DIOSTREAMERROR_TLSCONFIGURATION  : OperationError_Set(DIOWEBCLIENT_ERROR_TLSCONFIGURATION, streamerror);    return;
      case DIOSTREAMERROR_TLSPROTOCOL       : OperationError_Set(DIOWEBCLIENT_ERROR_TLSPROTOCOL, streamerror);         return;
      case DIOSTREAMERROR_TLSAUTHENTICATION : OperationError_Set(DIOWEBCLIENT_ERROR_TLSAUTHENTICATION, streamerror);   return;
      case DIOSTREAMERROR_URLNOTVALID       : OperationError_Set(DIOWEBCLIENT_ERROR_INVALIDURL, streamerror);          return;
                                      default : break;
    }

  OperationError_Set(defaulterror, streamerror);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBCLIENT::OperationError_AllowsHTTPFallback()
* @brief      Operation error allows http fallback
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBCLIENT::OperationError_AllowsHTTPFallback()
{
  // Master switch: when the automatic HTTP fallback is deactivated, a scheme-less URL never retries over
  // plain HTTP, whatever the cause of the HTTPS failure was. This is only reached from the scheme-less-URL
  // retry in MakeOperation(): a URL with an explicit https:// scheme never falls back to HTTP either way.
  if(!autohttpfallbackactive) return false;

  switch(lastoperationerror.GetError())
    {
      case DIOWEBCLIENT_ERROR_TRANSPORTUNAVAILABLE :
      case DIOWEBCLIENT_ERROR_TCPCONNECTION        :
      case DIOWEBCLIENT_ERROR_TCPTIMEOUT           :
      case DIOWEBCLIENT_ERROR_PROXY                :
      case DIOWEBCLIENT_ERROR_TLSPROTOCOL          : return true;
                                                default : return false;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBCLIENT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBCLIENT::Clean()
{
  maxsizebuffer           = 0;

  timerout                = NULL;

  diostreamcfg            = NULL;
  diostream               = NULL;

  port                    = DIOWEBCLIENT_DEFAULTPORT;
  isportconfigured        = false;
  isstreamTLS             = false;

  authenticationmethod    = DIOWEBCLIENT_AUTHENTICATION_METHOD_UNKNOWN;

  dostophttperror         = true;
  // A scheme-less URL first uses HTTPS and retries over HTTP only when the HTTPS transport cannot be established.
  // Explicit http:// and https:// schemes are mandatory and are never replaced by this policy.
  transportpolicy         = DIOWEBCLIENT_TRANSPORTPOLICY_HTTPS_PREFER;
  allowinsecureredirect   = false;
  // Preserves the pre-existing HTTPS_PREFER behavior by default: a scheme-less URL whose HTTPS attempt fails
  // may still retry automatically over plain HTTP, unless the application opts out via
  // AutoHTTPFallback_Activate(false).
  autohttpfallbackactive  = true;
  OperationError_Reset();

  contentencodingactive   = true;
  compressrequestbody     = false;

  compressmanager         = NULL;
}
