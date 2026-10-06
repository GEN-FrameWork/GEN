/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOWebServer_Plugin_CGI.cpp
* 
* @class      DIOWEBSERVER_PLUGIN_CGI
* @brief      Data Input/Output Web Server CGI (Common Gateway Interface) Plugin class
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

#include "DIOWebServer_Plugin_CGI.h"

#include "XFileTXT.h"
#include "XSystem.h"
#include "XProcessManager.h"
#include "XTrace.h"

#include "DIOWebServer_QueryStrings.h"
#include "DIOWebServer.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBSERVER_PLUGIN_CGI::DIOWEBSERVER_PLUGIN_CGI()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBSERVER_PLUGIN_CGI::DIOWEBSERVER_PLUGIN_CGI()
{
  Clean();

  GetName()->Set(_L("CGI Pluging"));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBSERVER_PLUGIN_CGI::~DIOWEBSERVER_PLUGIN_CGI()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBSERVER_PLUGIN_CGI::~DIOWEBSERVER_PLUGIN_CGI()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_PLUGIN_CGI::Config(XPATH* pathCGIinterpreter, XCHAR* nameexec)
* @brief      Config
* @ingroup    DATAIO
* 
* @param[in]  pathCGIinterpreter : Path CG Iinterpreter pointer to use.
* @param[in]  nameexec : Nameexec pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER_PLUGIN_CGI::Config(XPATH* pathCGIinterpreter, XCHAR* nameexec)
{
  if(!pathCGIinterpreter) return false;

  this->pathCGIinterpreter  = pathCGIinterpreter->Get();
  if(nameexec) this->nameexec = nameexec;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_PLUGIN_CGI::PageExtension(XPATH& pathfile, DIOWEBSERVER_REQUEST* request, DIOWEBSERVER_QUERYSTRINGS* querystrings, DIOWEBHEADER_RESULT& headerresult, XSTRING& result, bool istls)
* @brief      Page extension
* @ingroup    DATAIO
*
* @param[in]  pathfile : Pathfile value.
* @param[in]  request : Request pointer to use.
* @param[in]  querystrings : Querystrings pointer to use.
* @param[in]  headerresult : Output headerresult.
* @param[in]  result : Output result.
* @param[in]  istls : true if the request arrived over a TLS (HTTPS) listener; otherwise false.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER_PLUGIN_CGI::PageExtension(XPATH& pathfile, DIOWEBSERVER_REQUEST* request, DIOWEBSERVER_QUERYSTRINGS* querystrings, DIOWEBHEADER_RESULT& headerresult, XSTRING& result, bool istls)
{
  if(!request)      return false;
  if(!querystrings) return false;

  XSTRING allparam;

  headerresult = DIOWEBHEADER_RESULT_OK;

  querystrings->GetAllParam(allparam);

  XPATH pathexec;

  pathexec  = pathCGIinterpreter.Get();
  pathexec.Slash_Add();
  pathexec += nameexec;

  XSTRING methodstring;
  int     returnerror = 0;
  XBUFFER in;
  XBUFFER out;
  int     sizein = 0;
  XSTRING sizeinstr;
  bool    status;

  if(request->GetData())
    {
      sizein = request->GetData()->GetSize();
      if(sizein)
        {
          in.Add((*request->GetData()));

          sizeinstr.Format(_L("%d"), sizein);
        }
    }

  switch(request->GetMethod())
    {
      case DIOWEBHEADER_METHOD_GET     : methodstring = _L("\"GET\"");       break;
      case DIOWEBHEADER_METHOD_POST    : methodstring = _L("\"POST\"");      break;
      case DIOWEBHEADER_METHOD_PUT     : methodstring = _L("\"PUT\"");       break;
      case DIOWEBHEADER_METHOD_DELETE  : methodstring = _L("\"DELETE\"");    break;
                          default      : break;
    }


  GEN_XSYSTEM.SetEnviromentVariable(_L("GATEWAY_INTERFACE") , _L("CGI/1.1"));
  GEN_XSYSTEM.SetEnviromentVariable(_L("REQUEST_METHOD"), _L("POST"));
  GEN_XSYSTEM.SetEnviromentVariable(_L("SCRIPT_FILENAME"), pathfile.Get());
  GEN_XSYSTEM.SetEnviromentVariable(_L("CONTENT_LENGTH"), sizeinstr.Get());
  GEN_XSYSTEM.SetEnviromentVariable(_L("REDIRECT_STATUS"), _L("true"));
  GEN_XSYSTEM.SetEnviromentVariable(_L("CONTENT_TYPE"), _L("application/x-www-form-urlencoded"));

  // Standard CGI/1.1 convention (also followed by PHP's $_SERVER['HTTPS']): only set, and only to "on", when the
  // request arrived over TLS -- absent entirely for plain HTTP, never set to "off".
  if(istls) GEN_XSYSTEM.SetEnviromentVariable(_L("HTTPS"), _L("on"));

  if(request->GetMethod() == DIOWEBHEADER_METHOD_GET) GEN_XSYSTEM.SetEnviromentVariable(_L("QUERY_STRING"), allparam.GetSize()?allparam.Get():_L("\"\""));

  status = GEN_XPROCESSMANAGER.Application_Execute(pathexec.Get(), NULL, &in, &out, &returnerror);

  if(!out.IsEmpty())
    {
      result.Add(out);
    }

  GEN_XSYSTEM.DelEnviromentVariable(_L("GATEWAY_INTERFACE"));
  GEN_XSYSTEM.DelEnviromentVariable(_L("REQUEST_METHOD"));
  GEN_XSYSTEM.DelEnviromentVariable(_L("SCRIPT_FILENAME"));
  GEN_XSYSTEM.DelEnviromentVariable(_L("CONTENT_LENGTH"));
  GEN_XSYSTEM.DelEnviromentVariable(_L("REDIRECT_STATUS"));
  GEN_XSYSTEM.DelEnviromentVariable(_L("CONTENT_TYPE"));
  GEN_XSYSTEM.DelEnviromentVariable(_L("QUERY_STRING"));

  if(istls) GEN_XSYSTEM.DelEnviromentVariable(_L("HTTPS"));

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBSERVER_PLUGIN_CGI::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBSERVER_PLUGIN_CGI::Clean()
{
  pathCGIinterpreter.Empty();
}



