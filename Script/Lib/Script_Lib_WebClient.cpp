/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       Script_Lib_WebClient.cpp
* 
* @class      SCRIPT_LIB_WEBCLIENT
* @brief      Script Library Web Client
* @ingroup    SCRIPT
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

#include "Script_Lib_WebClient.h"

#include "XPath.h"
#include "XVariant.h"

#include "DIOURL.h"
#include "DIOWebClient.h"

#include "Script.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/




/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SCRIPT_LIB_WEBCLIENT::SCRIPT_LIB_WEBCLIENT()
* @brief      Constructor of class
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
SCRIPT_LIB_WEBCLIENT::SCRIPT_LIB_WEBCLIENT() : SCRIPT_LIB(SCRIPT_LIB_NAME_WEBCLIENT)
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SCRIPT_LIB_WEBCLIENT::~SCRIPT_LIB_WEBCLIENT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
SCRIPT_LIB_WEBCLIENT::~SCRIPT_LIB_WEBCLIENT()
{
  if(webclient)
    {
      GEN_DELETE webclient;
    }

  lastbody.Delete();
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT_LIB_WEBCLIENT::AddLibraryFunctions(SCRIPT* script)
* @brief      Add library functions
* @ingroup    SCRIPT
* 
* @param[in]  script : Script pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT_LIB_WEBCLIENT::AddLibraryFunctions(SCRIPT* script)
{
  if(!script)
    {
      return false;
    }

  this->script = script;

  script->AddLibraryFunction(this, _L("WebClient_Get")            , Call_WebClient_Get);
  script->AddLibraryFunction(this, _L("WebClient_Post")           , Call_WebClient_Post);
  script->AddLibraryFunction(this, _L("WebClient_GetToFile")      , Call_WebClient_GetToFile);
  script->AddLibraryFunction(this, _L("WebClient_GetBody")        , Call_WebClient_GetBody);
  script->AddLibraryFunction(this, _L("WebClient_GetStatus")      , Call_WebClient_GetStatus);
  script->AddLibraryFunction(this, _L("WebClient_GetHeader")      , Call_WebClient_GetHeader);
  script->AddLibraryFunction(this, _L("WebClient_GetLastError")   , Call_WebClient_GetLastError);
  script->AddLibraryFunction(this, _L("WebClient_SetLogin")       , Call_WebClient_SetLogin);
  script->AddLibraryFunction(this, _L("WebClient_DoStopHTTPError"), Call_WebClient_DoStopHTTPError);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBCLIENT* SCRIPT_LIB_WEBCLIENT::GetWebClient()
* @brief      Get (or create) the underlying GEN web client
* @ingroup    SCRIPT
* 
* @return     DIOWEBCLIENT* : Client pointer, or NULL on allocation failure.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBCLIENT* SCRIPT_LIB_WEBCLIENT::GetWebClient()
{
  if(!webclient)
    {
      webclient = GEN_NEW DIOWEBCLIENT();
    }

  return webclient;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBUFFER* SCRIPT_LIB_WEBCLIENT::GetLastBodyBuffer()
* @brief      Get last response body buffer
* @ingroup    SCRIPT
* 
* @return     XBUFFER* : Buffer pointer.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* SCRIPT_LIB_WEBCLIENT::GetLastBodyBuffer()
{
  return &lastbody;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT_LIB_WEBCLIENT::BodyBufferToString(XSTRING& out)
* @brief      Convert last body buffer to string (UTF-8 preferred, ASCII fallback)
* @ingroup    SCRIPT
* 
* @param[out] out : Destination string.
* 
* @return     bool : true if conversion produced content or empty body is valid.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT_LIB_WEBCLIENT::BodyBufferToString(XSTRING& out)
{
  out.Empty();

  if(!lastbody.GetSize())
    {
      return true;
    }

  if(out.ConvertFromUTF8(lastbody))
    {
      return true;
    }

  out = lastbody;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SCRIPT_LIB_WEBCLIENT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SCRIPT_LIB_WEBCLIENT::Clean()
{
  webclient = NULL;
}




/*---- LIBRARY FUNCTIONS ---------------------------------------------------------------------------------------------*/


static SCRIPT_LIB_WEBCLIENT* ScriptLibWebClient_AsLib(SCRIPT_LIB* library)
{
  return (SCRIPT_LIB_WEBCLIENT*)library;
}


static XCHAR* ScriptLibWebClient_OptionalHeader(SCRIPT_LIB* library, XVECTOR<XVARIANT*>* params, int index, XSTRING& storage)
{
  storage.Empty();

  if((!library) || (!params) || (params->GetSize() <= (XDWORD)index))
    {
      return NULL;
    }

  if(!library->GetParamConverted(params->Get(index), storage))
    {
      return NULL;
    }

  if(storage.IsEmpty())
    {
      return NULL;
    }

  return storage.Get();
}


static int ScriptLibWebClient_OptionalTimeout(SCRIPT_LIB* library, XVECTOR<XVARIANT*>* params, int index)
{
  int timeout = DIOWEBCLIENT_TIMEOUT;

  if((!library) || (!params) || (params->GetSize() <= (XDWORD)index))
    {
      return timeout;
    }

  library->GetParamConverted(params->Get(index), timeout);
  if(timeout <= 0) timeout = DIOWEBCLIENT_TIMEOUT;

  return timeout;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_WebClient_Get(...)
* @brief      WebClient_Get(url [, headers [, timeout]]) → bool
* @ingroup    SCRIPT
* 
* @note       Downloads URL into the library body buffer. Optional headers are raw HTTP lines
*             (e.g. "User-Agent: ...\r\nAccept: ...\r\n").
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_WebClient_Get(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params) || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();
  (*returnvalue) = false;

  if(params->GetSize() < 1)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  SCRIPT_LIB_WEBCLIENT* weblib = ScriptLibWebClient_AsLib(library);
  DIOWEBCLIENT*         client = weblib ? weblib->GetWebClient() : NULL;
  XSTRING               url;
  XSTRING               headers;
  XCHAR*                addheader = NULL;
  int                   timeout   = DIOWEBCLIENT_TIMEOUT;
  bool                  status    = false;

  if(!client)
    {
      return;
    }

  if(!library->GetParamConverted(params->Get(0), url) || url.IsEmpty())
    {
      return;
    }

  addheader = ScriptLibWebClient_OptionalHeader(library, params, 1, headers);
  timeout   = ScriptLibWebClient_OptionalTimeout(library, params, 2);

  weblib->GetLastBodyBuffer()->Delete();
  status = client->Get(url.Get(), (*weblib->GetLastBodyBuffer()), addheader, timeout, NULL);

  (*returnvalue) = status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_WebClient_Post(...)
* @brief      WebClient_Post(url, body [, headers [, timeout]]) → bool
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_WebClient_Post(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params) || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();
  (*returnvalue) = false;

  if(params->GetSize() < 2)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  SCRIPT_LIB_WEBCLIENT* weblib = ScriptLibWebClient_AsLib(library);
  DIOWEBCLIENT*         client = weblib ? weblib->GetWebClient() : NULL;
  XSTRING               url;
  XSTRING               body;
  XSTRING               headers;
  XBUFFER               postdata;
  XCHAR*                addheader = NULL;
  int                   timeout   = DIOWEBCLIENT_TIMEOUT;
  bool                  status    = false;

  if(!client)
    {
      return;
    }

  if(!library->GetParamConverted(params->Get(0), url) || url.IsEmpty())
    {
      return;
    }

  if(!library->GetParamConverted(params->Get(1), body))
    {
      return;
    }

  XBUFFER* postptr = NULL;
  if(!body.IsEmpty() && body.ConvertToUTF8(postdata, false))
    {
      postptr = &postdata;
    }

  addheader = ScriptLibWebClient_OptionalHeader(library, params, 2, headers);
  timeout   = ScriptLibWebClient_OptionalTimeout(library, params, 3);

  weblib->GetLastBodyBuffer()->Delete();
  status = client->Post(url.Get(), (*weblib->GetLastBodyBuffer()), postptr, addheader, timeout, NULL);

  (*returnvalue) = status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_WebClient_GetToFile(...)
* @brief      WebClient_GetToFile(url, path [, headers [, timeout]]) → bool
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_WebClient_GetToFile(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params) || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();
  (*returnvalue) = false;

  if(params->GetSize() < 2)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  SCRIPT_LIB_WEBCLIENT* weblib = ScriptLibWebClient_AsLib(library);
  DIOWEBCLIENT*         client = weblib ? weblib->GetWebClient() : NULL;
  XSTRING               url;
  XSTRING               pathstr;
  XSTRING               headers;
  XPATH                 path;
  XCHAR*                addheader = NULL;
  int                   timeout   = DIOWEBCLIENT_TIMEOUT;
  bool                  status    = false;

  if(!client)
    {
      return;
    }

  if(!library->GetParamConverted(params->Get(0), url) || url.IsEmpty())
    {
      return;
    }

  if(!library->GetParamConverted(params->Get(1), pathstr) || pathstr.IsEmpty())
    {
      return;
    }

  path = pathstr.Get();

  addheader = ScriptLibWebClient_OptionalHeader(library, params, 2, headers);
  timeout   = ScriptLibWebClient_OptionalTimeout(library, params, 3);

  weblib->GetLastBodyBuffer()->Delete();
  status = client->Get(url.Get(), path, addheader, timeout, NULL);

  (*returnvalue) = status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_WebClient_GetBody(...)
* @brief      WebClient_GetBody() → string (last response body)
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_WebClient_GetBody(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params) || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();

  SCRIPT_LIB_WEBCLIENT* weblib = ScriptLibWebClient_AsLib(library);
  XSTRING               body;

  if(weblib)
    {
      weblib->BodyBufferToString(body);
    }

  (*returnvalue) = body;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_WebClient_GetStatus(...)
* @brief      WebClient_GetStatus() → HTTP status code of last response (0 if none)
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_WebClient_GetStatus(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params) || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();
  (*returnvalue) = 0;

  SCRIPT_LIB_WEBCLIENT* weblib = ScriptLibWebClient_AsLib(library);
  DIOWEBCLIENT*         client = weblib ? weblib->GetWebClient() : NULL;

  if(client && client->GetHeader())
    {
      (*returnvalue) = client->GetHeader()->GetResultServer();
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_WebClient_GetHeader(...)
* @brief      WebClient_GetHeader(name) → response header field value (empty if missing)
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_WebClient_GetHeader(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params) || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();

  if(params->GetSize() < 1)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  SCRIPT_LIB_WEBCLIENT* weblib = ScriptLibWebClient_AsLib(library);
  DIOWEBCLIENT*         client = weblib ? weblib->GetWebClient() : NULL;
  XSTRING               name;
  XSTRING               value;

  if(!library->GetParamConverted(params->Get(0), name) || name.IsEmpty())
    {
      (*returnvalue) = value;
      return;
    }

  if(client && client->GetHeader())
    {
      XCHAR* field = client->GetHeader()->GetFieldValue(name);
      if(field)
        {
          value = field;
        }
    }

  (*returnvalue) = value;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_WebClient_GetLastError(...)
* @brief      WebClient_GetLastError() → DIOWEBCLIENT_ERROR code of last operation
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_WebClient_GetLastError(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params) || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();
  (*returnvalue) = (int)DIOWEBCLIENT_ERROR_NONE;

  SCRIPT_LIB_WEBCLIENT*          weblib = ScriptLibWebClient_AsLib(library);
  DIOWEBCLIENT*                  client = weblib ? weblib->GetWebClient() : NULL;
  DIOWEBCLIENT_OPERATIONERROR*   operror = client ? client->GetLastOperationError() : NULL;

  if(operror)
    {
      (*returnvalue) = (int)operror->GetError();
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_WebClient_SetLogin(...)
* @brief      WebClient_SetLogin(user, password) → bool
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_WebClient_SetLogin(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params) || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();
  (*returnvalue) = false;

  if(params->GetSize() < 2)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  SCRIPT_LIB_WEBCLIENT* weblib = ScriptLibWebClient_AsLib(library);
  DIOWEBCLIENT*         client = weblib ? weblib->GetWebClient() : NULL;
  XSTRING               user;
  XSTRING               password;

  if(!client)
    {
      return;
    }

  if(!library->GetParamConverted(params->Get(0), user))
    {
      return;
    }

  if(!library->GetParamConverted(params->Get(1), password))
    {
      return;
    }

  if(client->GetLogin())    (*client->GetLogin())    = user;
  if(client->GetPassword()) (*client->GetPassword()) = password;

  (*returnvalue) = true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_WebClient_DoStopHTTPError(...)
* @brief      WebClient_DoStopHTTPError(activate) → bool (configures whether HTTP >=400 fails the call)
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_WebClient_DoStopHTTPError(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params) || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();
  (*returnvalue) = false;

  if(params->GetSize() < 1)
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  SCRIPT_LIB_WEBCLIENT* weblib = ScriptLibWebClient_AsLib(library);
  DIOWEBCLIENT*         client = weblib ? weblib->GetWebClient() : NULL;
  bool                  activate = true;

  if(!client)
    {
      return;
    }

  library->GetParamConverted(params->Get(0), activate);
  client->DoStopHTTPError(activate);

  (*returnvalue) = true;
}
