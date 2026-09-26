/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOStreamTCPIPProxy.cpp
* 
* @class      DIOSTREAMTCPIPPROXYCFG
* 
* @class      DIOSTREAMTCPIPPROXY
* @brief      Data Input/Output proxy configuration and tunnel negotiation classes
* @ingroup    DATAIO
* 
* @copyright  EndoraSoft. All rights reserved.
* 
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

#include "DIOStreamTCPIPProxy.h"
#include "DIOStream.h"


/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"


/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTCPIPPROXYCFG::DIOSTREAMTCPIPPROXYCFG()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTCPIPPROXYCFG::DIOSTREAMTCPIPPROXYCFG()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTCPIPPROXYCFG::~DIOSTREAMTCPIPPROXYCFG()
* @brief      Destructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTCPIPPROXYCFG::~DIOSTREAMTCPIPPROXYCFG()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTCPIPPROXYTYPE DIOSTREAMTCPIPPROXYCFG::GetType()
* @brief      Get type
* @ingroup    DATAIO
* 
* @return     DIOSTREAMTCPIPPROXYTYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTCPIPPROXYTYPE DIOSTREAMTCPIPPROXYCFG::GetType()
{
  return type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTCPIPPROXYCFG::SetType(DIOSTREAMTCPIPPROXYTYPE type)
* @brief      Set type
* @ingroup    DATAIO
* 
* @param[in]  type : Type value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTCPIPPROXYCFG::SetType(DIOSTREAMTCPIPPROXYTYPE type)
{
  this->type = type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTCPIPPROXYMODE DIOSTREAMTCPIPPROXYCFG::GetMode()
* @brief      Get mode
* @ingroup    DATAIO
* 
* @return     DIOSTREAMTCPIPPROXYMODE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTCPIPPROXYMODE DIOSTREAMTCPIPPROXYCFG::GetMode()
{
  return mode;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTCPIPPROXYCFG::SetMode(DIOSTREAMTCPIPPROXYMODE mode)
* @brief      Set mode
* @ingroup    DATAIO
* 
* @param[in]  mode : Mode value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTCPIPPROXYCFG::SetMode(DIOSTREAMTCPIPPROXYMODE mode)
{
  this->mode = mode;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTCPIPPROXYCFG::IsActive()
* @brief      Is active
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTCPIPPROXYCFG::IsActive()
{
  if(type == DIOSTREAMTCPIPPROXYTYPE_NONE) return false;
  if(mode == DIOSTREAMTCPIPPROXYMODE_NONE) return false;
  if(url.IsEmpty()) return false;
  if(port <= 0) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOURL* DIOSTREAMTCPIPPROXYCFG::GetURL()
* @brief      Get url
* @ingroup    DATAIO
* 
* @return     DIOURL* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOURL* DIOSTREAMTCPIPPROXYCFG::GetURL()
{
  return &url;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOSTREAMTCPIPPROXYCFG::GetPort()
* @brief      Get port
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOSTREAMTCPIPPROXYCFG::GetPort()
{
  return port;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTCPIPPROXYCFG::SetPort(int port)
* @brief      Set port
* @ingroup    DATAIO
* 
* @param[in]  port : Port value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTCPIPPROXYCFG::SetPort(int port)
{
  this->port = port;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOSTREAMTCPIPPROXYCFG::GetLogin()
* @brief      Get login
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOSTREAMTCPIPPROXYCFG::GetLogin()
{
  return &login;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOSTREAMTCPIPPROXYCFG::GetPassword()
* @brief      Get password
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOSTREAMTCPIPPROXYCFG::GetPassword()
{
  return &password;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSTREAMTCPIPPROXYCFG::Clean()
* @brief      Clean
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTREAMTCPIPPROXYCFG::Clean()
{
  type = DIOSTREAMTCPIPPROXYTYPE_NONE;
  mode = DIOSTREAMTCPIPPROXYMODE_NONE;
  port = 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTCPIPPROXY::DIOSTREAMTCPIPPROXY()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTCPIPPROXY::DIOSTREAMTCPIPPROXY()
{
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMTCPIPPROXY::~DIOSTREAMTCPIPPROXY()
* @brief      Destructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMTCPIPPROXY::~DIOSTREAMTCPIPPROXY()
{
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTCPIPPROXY::Connect(DIOSTREAM* stream, DIOSTREAMTCPIPPROXYCFG* cfg, XCHAR* target, int targetport, int timeout)
* @brief      Connect
* @ingroup    DATAIO
* 
* @param[in]  stream : Pointer to stream.
* @param[in]  cfg : Pointer to cfg.
* @param[out] target : Pointer to target.
* @param[in]  targetport : Targetport value.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTCPIPPROXY::Connect(DIOSTREAM* stream, DIOSTREAMTCPIPPROXYCFG* cfg, XCHAR* target, int targetport, int timeout)
{
  if(!stream || !cfg || !target || !target[0] || (targetport <= 0)) return false;
  if(!cfg->IsActive()) return true;
  if(cfg->GetMode() != DIOSTREAMTCPIPPROXYMODE_TUNNEL) return true;

  switch(cfg->GetType())
    {
      case DIOSTREAMTCPIPPROXYTYPE_HTTP : return ConnectHTTP(stream, cfg, target, targetport, timeout);
                 default     : break;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSTREAMTCPIPPROXY::ConnectHTTP(DIOSTREAM* stream, DIOSTREAMTCPIPPROXYCFG* cfg, XCHAR* target, int targetport, int timeout)
* @brief      Connect http
* @ingroup    DATAIO
* 
* @param[in]  stream : Pointer to stream.
* @param[in]  cfg : Pointer to cfg.
* @param[out] target : Pointer to target.
* @param[in]  targetport : Targetport value.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTREAMTCPIPPROXY::ConnectHTTP(DIOSTREAM* stream, DIOSTREAMTCPIPPROXYCFG* cfg, XCHAR* target, int targetport, int timeout)
{
  XSTRING request;
  XSTRING line;

  request.Format(__L("CONNECT %s:%d HTTP/1.1\r\nHost: %s:%d\r\n"), target, targetport, target, targetport);

  if(!cfg->GetLogin()->IsEmpty() || !cfg->GetPassword()->IsEmpty())
    {
      XSTRING credentials;
      XSTRING credentialsbase64;

      credentials.Format(__L("%s:%s"), cfg->GetLogin()->Get(), cfg->GetPassword()->Get());
      credentials.ConvertToBase64(credentialsbase64);

      request += __L("Proxy-Authorization: Basic ");
      request += credentialsbase64;
      request += __L("\r\n");
    }

  request += __L("Proxy-Connection: Keep-Alive\r\n\r\n");

  if(!stream->WriteStr(request)) return false;
  if(!stream->WaitToFlushOutXBuffer(timeout)) return false;

  if(!stream->ReadStr(line, timeout)) return false;

  int statuscode = 0;
  XSTRING protocol;

  protocol.AdjustSize(32);
  if(!line.UnFormat(__L("%s %d"), protocol.Get(), &statuscode)) return false;
  protocol.AdjustSize();

  if((statuscode < 200) || (statuscode > 299)) return false;

  for(;;)
    {
      line.Empty();
      if(!stream->ReadStr(line, timeout)) return false;
      if(line.IsEmpty()) break;
    }

  return true;
}
