/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIODNSResolver.cpp
* 
* @class      DIODNSRESOLVER
* @brief      Data Input/Output Domain Network System (DNS) resolved class
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

#include "DIODNSResolver.h"

#include "XFactory.h"
#include "XDateTime.h"
#include "XTrace.h"
#include "XTimer.h"

#include "DIOFactory.h"
#include "DIOStreamUDPConfig.h"
#include "DIOStreamUDP.h"
#include "DIODNSProtocol_Client.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

DIODNSRESOLVER* DIODNSRESOLVER::instance = NULL;



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/



/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIODNSRESOLVER_HOSTRESOLVED::DIODNSRESOLVER_HOSTRESOLVED()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIODNSRESOLVER_HOSTRESOLVED::DIODNSRESOLVER_HOSTRESOLVED()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIODNSRESOLVER_HOSTRESOLVED::~DIODNSRESOLVER_HOSTRESOLVED()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIODNSRESOLVER_HOSTRESOLVED::~DIODNSRESOLVER_HOSTRESOLVED()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIODNSRESOLVER_HOSTRESOLVED::GetHost()
* @brief      Get host
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIODNSRESOLVER_HOSTRESOLVED::GetHost()
{
  return &host;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIODNSRESOLVER_HOSTRESOLVED::GetIP()
* @brief      Get IP
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIODNSRESOLVER_HOSTRESOLVED::GetIP()
{
  return &IPresolved;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIODNSRESOLVER_HOSTRESOLVED::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIODNSRESOLVER_HOSTRESOLVED::Clean()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIODNSRESOLVER_DNSSERVER::DIODNSRESOLVER_DNSSERVER()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIODNSRESOLVER_DNSSERVER::DIODNSRESOLVER_DNSSERVER()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIODNSRESOLVER_DNSSERVER::~DIODNSRESOLVER_DNSSERVER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIODNSRESOLVER_DNSSERVER::~DIODNSRESOLVER_DNSSERVER()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOIP* DIODNSRESOLVER_DNSSERVER::GetIP()
* @brief      Get IP
* @ingroup    DATAIO
* 
* @return     DIOIP* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOIP* DIODNSRESOLVER_DNSSERVER::GetIP()
{
  return &IP;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORD DIODNSRESOLVER_DNSSERVER::GetPort()
* @brief      Get port
* @ingroup    DATAIO
* 
* @return     XWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORD DIODNSRESOLVER_DNSSERVER::GetPort()
{
  return port;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIODNSRESOLVER_DNSSERVER::SetPort(XWORD port)
* @brief      Set port
* @ingroup    DATAIO
* 
* @param[in]  port : Port number to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIODNSRESOLVER_DNSSERVER::SetPort(XWORD port)
{
  this->port = port;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIODNSRESOLVER_DNSSERVER::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIODNSRESOLVER_DNSSERVER::Clean()
{
  port = 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIODNSRESOLVER::GetIsInstanced()
* @brief      Get is instanced
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIODNSRESOLVER::GetIsInstanced()
{
  return instance!=NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIODNSRESOLVER& DIODNSRESOLVER::GetInstance()
* @brief      Get instance
* @ingroup    DATAIO
* 
* @return     DIODNSRESOLVER& : Reference to the requested object.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIODNSRESOLVER& DIODNSRESOLVER::GetInstance()
{
  if(!instance) instance = GEN_NEW DIODNSRESOLVER();

  return (*instance);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIODNSRESOLVER::DelInstance()
* @brief      Del instance
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIODNSRESOLVER::DelInstance()
{
  if(instance)
    {
      GEN_DELETE instance;
      instance = NULL;

      return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIODNSRESOLVER::ResolveURL(XCHAR* URL, DIOIP& IPresolved, int querytype, XDWORD timeout)
* @brief      Resolve URL
* @ingroup    DATAIO
* 
* @param[in]  URL : URL to use.
* @param[in]  IPresolved : I Presolved value.
* @param[in]  querytype : Querytype value.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIODNSRESOLVER::ResolveURL(XCHAR* URL, DIOIP& IPresolved, int querytype, XDWORD timeout)
{
  if(xmutexresolved)
    {
      //xmutexresolved->Lock();
    }

  if(Host_FindIP(URL, IPresolved)) 
    {
      if(xmutexresolved)
        {
          //xmutexresolved->UnLock();
        }

      return true;
    }

  if(!listhostresolved.GetSize())
    {
      GEN_DIODNSRESOLVER.Host_Add(_L("localhost"), _L("127.0.0.1"));
    }

  DIOURL url;
  
  url = URL;  

  if(!url.IsAURL())
    {  
      IPresolved.Set(URL);  
    }
   else
    { 
      XTIMER* xtimerout=GEN_XFACTORY.CreateTimer();
      if(xtimerout) 
        { 
          bool status;

          if(!listDNSservers.GetSize())
            {
              GEN_DIODNSRESOLVER.DNSServer_AddDNSServer(_L("8.8.8.8"));
              GEN_DIODNSRESOLVER.DNSServer_AddDNSServer(_L("8.8.4.4"));
              GEN_DIODNSRESOLVER.DNSServer_AddDNSServer(_L("64.6.64.6"));
              GEN_DIODNSRESOLVER.DNSServer_AddDNSServer(_L("64.6.65.6"));
            }

          DIODNSPROTOCOLCLIENT* dnsprotocolclient = GEN_NEW DIODNSPROTOCOLCLIENT();
          if(dnsprotocolclient)
            {
              for(XDWORD c=0; c<listDNSservers.GetSize(); c++)
                {
                  if(xtimerout->GetMeasureSeconds() > (timeout * listDNSservers.GetSize()))
                    {
                      status = false;
                      break;
                    }

                  DIODNSRESOLVER_DNSSERVER* serverDNS = listDNSservers.Get(c);
                  if(serverDNS)
                    {
                      XSTRING serverIP;

                      serverDNS->GetIP()->GetXString(serverIP);

                      dnsprotocolclient->SetServer(serverIP.Get(), serverDNS->GetPort());

                      status = dnsprotocolclient->ResolveURL(URL, IPresolved, querytype, timeout);
                      if(status) 
                        {
                          break;
                        }
                    }
                }

              GEN_DELETE dnsprotocolclient;
            }

          GEN_XFACTORY.DeleteTimer(xtimerout);
        }
    }

  if(xmutexresolved)
    {
      //xmutexresolved->UnLock();
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIODNSRESOLVER::ResolveURL(DIOURL& URL, DIOIP& IPresolved, int querytype, XDWORD timeout)
* @brief      Resolve URL
* @ingroup    DATAIO
* 
* @param[in]  URL : URL to use.
* @param[in]  IPresolved : I Presolved value.
* @param[in]  querytype : Querytype value.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIODNSRESOLVER::ResolveURL(DIOURL& URL, DIOIP& IPresolved, int querytype, XDWORD timeout)
{
  return ResolveURL(URL.Get(), IPresolved, querytype, timeout);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIODNSRESOLVER::ResolveURL(XSTRING& URL, DIOIP& IPresolved, int querytype, XDWORD timeout)
* @brief      Resolve URL
* @ingroup    DATAIO
* 
* @param[in]  URL : URL to use.
* @param[in]  IPresolved : I Presolved value.
* @param[in]  querytype : Querytype value.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIODNSRESOLVER::ResolveURL(XSTRING& URL, DIOIP& IPresolved, int querytype, XDWORD timeout)
{
  return ResolveURL(URL.Get(), IPresolved, querytype, timeout);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIODNSRESOLVER::DNSServer_AddDNSServer(XCHAR* server)
* @brief      DNS server add DNS server
* @ingroup    DATAIO
* 
* @param[in]  server : Server pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIODNSRESOLVER::DNSServer_AddDNSServer(XCHAR* server)
{
  if(!server) return false;

  DIODNSRESOLVER_DNSSERVER* DNSserver = GEN_NEW DIODNSRESOLVER_DNSSERVER();
  if(!DNSserver) return false;

  XSTRING string;
  XSTRING IPstring;
  int     port = 0;

  string = server;
  IPstring.AdjustSize(_MAXSTR);
  string.UnFormat(_L("%s:%d"), IPstring.Get(), &port);
  IPstring.AdjustSize();

  if(IPstring.IsEmpty())
    {
      GEN_DELETE DNSserver;

      return false;
    }

  DNSserver->GetIP()->Set(IPstring);
  DNSserver->SetPort(port);

  if(DNSServer_IsIPonList((*DNSserver->GetIP()))) 
    {
      GEN_DELETE DNSserver;

      return false;
    }

  return listDNSservers.Add(DNSserver);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIODNSRESOLVER::DNSServer_AddDNSServer(XSTRING& server)
* @brief      DNS server add DNS server
* @ingroup    DATAIO
* 
* @param[in]  server : Server value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIODNSRESOLVER::DNSServer_AddDNSServer(XSTRING& server)
{
  return DNSServer_AddDNSServer(server.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIODNSRESOLVER::DNSServer_AddDNSServer(XBYTE* serverIP, XWORD port)
* @brief      DNS server add DNS server
* @ingroup    DATAIO
* 
* @param[in]  serverIP : Server IP pointer to use.
* @param[in]  port : Port number to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIODNSRESOLVER::DNSServer_AddDNSServer(XBYTE* serverIP, XWORD port)
{
  DIODNSRESOLVER_DNSSERVER* DNSserver = GEN_NEW DIODNSRESOLVER_DNSSERVER();
  if(!DNSserver) return false;

  DNSserver->GetIP()->Set(serverIP);
  DNSserver->SetPort(port);

  if(DNSServer_IsIPonList((*DNSserver->GetIP()))) 
    {
      GEN_DELETE DNSserver;

      return false;
    }

  return listDNSservers.Add(DNSserver);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIODNSRESOLVER::DNSServer_AddDNSServer(DIOIP& serverIP, XWORD port)
* @brief      DNS server add DNS server
* @ingroup    DATAIO
* 
* @param[in]  serverIP : Server IP value.
* @param[in]  port : Port number to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIODNSRESOLVER::DNSServer_AddDNSServer(DIOIP& serverIP, XWORD port)
{
  if(DNSServer_IsIPonList(serverIP)) return false;
  
  DIODNSRESOLVER_DNSSERVER* DNSserver = GEN_NEW DIODNSRESOLVER_DNSSERVER();
  if(!DNSserver) return false;

  DNSserver->GetIP()->Set(serverIP.Get());
  DNSserver->SetPort(port);

  return listDNSservers.Add(DNSserver);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIODNSRESOLVER::DNSServer_IsIPonList(DIOIP& serverIP)
* @brief      DNS server is I pon list
* @ingroup    DATAIO
* 
* @param[in]  serverIP : Server IP value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIODNSRESOLVER::DNSServer_IsIPonList(DIOIP& serverIP)
{
  for(XDWORD c=0; c<listDNSservers.GetSize(); c++)
    {
      DIODNSRESOLVER_DNSSERVER* DNSserver = listDNSservers.Get(c);
      if(DNSserver)
        {
          if(serverIP.Compare((*DNSserver->GetIP())))
            {
              return true;
            }          
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<DIODNSRESOLVER_DNSSERVER*>* DIODNSRESOLVER::DNSServer_GetList()
* @brief      DNS server get list
* @ingroup    DATAIO
* 
* @return     XVECTOR<DIODNSRESOLVER_DNSSERVER*>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<DIODNSRESOLVER_DNSSERVER*>* DIODNSRESOLVER::DNSServer_GetList()
{
  return &listDNSservers;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIODNSRESOLVER::Host_FindIP(XCHAR* host, DIOIP& IPresolved)
* @brief      Host find IP
* @ingroup    DATAIO
* 
* @param[in]  host : Host pointer to use.
* @param[in]  IPresolved : I Presolved value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIODNSRESOLVER::Host_FindIP(XCHAR* host, DIOIP& IPresolved)
{
  bool status = false;

  for(XDWORD c=0; c<listhostresolved.GetSize(); c++)
    {
      DIODNSRESOLVER_HOSTRESOLVED* hostresolved = listhostresolved.Get(c);
      if(hostresolved)
        {
          if(!hostresolved->GetHost()->Compare(host, true))
            {
              IPresolved.Set(hostresolved->GetIP()->Get());
              status = true;
              break;
            }
        }
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIODNSRESOLVER::Host_FindIP(XSTRING& host, DIOIP& IPresolved)
* @brief      Host find IP
* @ingroup    DATAIO
* 
* @param[in]  host : Host value.
* @param[in]  IPresolved : I Presolved value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIODNSRESOLVER::Host_FindIP(XSTRING& host, DIOIP& IPresolved)
{
  return Host_FindIP(host.Get(), IPresolved);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIODNSRESOLVER::DNSServer_DeleteAllList()
* @brief      DNS server GEN_DELETE all list
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIODNSRESOLVER::DNSServer_DeleteAllList()
{
  listDNSservers.DeleteContents();
  listDNSservers.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIODNSRESOLVER::Host_Add(XCHAR* host, XCHAR* IPresolved)
* @brief      Host add
* @ingroup    DATAIO
* 
* @param[in]  host : Host pointer to use.
* @param[in]  IPresolved : I Presolved pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIODNSRESOLVER::Host_Add(XCHAR* host, XCHAR* IPresolved)
{
  DIODNSRESOLVER_HOSTRESOLVED* hostresolved = GEN_NEW DIODNSRESOLVER_HOSTRESOLVED();
  if(!hostresolved) return false;

  hostresolved->GetHost()->Set(host);
  hostresolved->GetIP()->Set(IPresolved);

  return listhostresolved.Add(hostresolved);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIODNSRESOLVER::Host_Add(XSTRING& host, XSTRING& IPresolved)
* @brief      Host add
* @ingroup    DATAIO
* 
* @param[in]  host : Host value.
* @param[in]  IPresolved : I Presolved value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIODNSRESOLVER::Host_Add(XSTRING& host, XSTRING& IPresolved)
{
  return Host_Add(host.Get(), IPresolved.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<DIODNSRESOLVER_HOSTRESOLVED*>* DIODNSRESOLVER::Host_GetList()
* @brief      Host get list
* @ingroup    DATAIO
* 
* @return     XVECTOR<DIODNSRESOLVER_HOSTRESOLVED*>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<DIODNSRESOLVER_HOSTRESOLVED*>*  DIODNSRESOLVER::Host_GetList()
{
  return &listhostresolved;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIODNSRESOLVER::Host_DeleteAllList()
* @brief      Host GEN_DELETE all list
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIODNSRESOLVER::Host_DeleteAllList()
{
  listhostresolved.DeleteContents();
  listhostresolved.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIODNSRESOLVER::DIODNSRESOLVER()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIODNSRESOLVER::DIODNSRESOLVER()
{
  Clean();

  xmutexresolved = GEN_XFACTORY.Create_Mutex();  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIODNSRESOLVER::~DIODNSRESOLVER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIODNSRESOLVER::~DIODNSRESOLVER()
{  
  if(xmutexresolved)
    {
      GEN_XFACTORY.Delete_Mutex(xmutexresolved);
      xmutexresolved = NULL;
    }

  DNSServer_DeleteAllList();
  Host_DeleteAllList();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIODNSRESOLVER::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIODNSRESOLVER::Clean()
{
  xmutexresolved        = NULL;
}





