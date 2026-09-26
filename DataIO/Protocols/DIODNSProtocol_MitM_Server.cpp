/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIODNSProtocol_MitM_Server.cpp
* 
* @class      DIODNSPROTOCOL_MITM_SERVER
* @brief      Data Input/Output Domain Network System (DNS) protocol Man-in-the-Middle (MitM) Server class
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

#include "DIODNSProtocol_MitM_Server.h"

#include "XFactory.h"
#include "XSleep.h"
#include "XTrace.h"

#include "DIOFactory.h"
#include "DIOStreamUDP.h"
#include "DIODNSProtocol_Client.h"
#include "DIODNSProtocol_MitM_Server_XEvent.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIODNSPROTOCOL_MITM_SERVER::DIODNSPROTOCOL_MITM_SERVER()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIODNSPROTOCOL_MITM_SERVER::DIODNSPROTOCOL_MITM_SERVER()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIODNSPROTOCOL_MITM_SERVER::~DIODNSPROTOCOL_MITM_SERVER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIODNSPROTOCOL_MITM_SERVER::~DIODNSPROTOCOL_MITM_SERVER()
{
  End();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIODNSPROTOCOL_MITM_SERVER::Ini()
* @brief      Initialize the object
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIODNSPROTOCOL_MITM_SERVER::Ini()
{
  RegisterEvent(DIODNSPROTOCOL_MITM_SERVER_XEVENT_TYPE_ASKDNS);
  RegisterEvent(DIODNSPROTOCOL_MITM_SERVER_XEVENT_TYPE_ANSWERDNS);

  servermutex = GEN_XFACTORY.Create_Mutex();
  if(!servermutex)
    {
      return false;
    }

  XSTRING servercfgstr;

  diostreamudpcfg.SetMode(DIOSTREAMMODE_SERVER);
  diostreamudpcfg.GetRemoteURL()->Set(__L("localhost"));  
  diostreamudpcfg.SetRemotePort(DIODNSPROTOCOL_DEFAULTPORT);  
  diostreamudpcfg.SetIsUsedDatagrams(false);
  
	diostreamudp = (DIOSTREAMUDP*)GEN_DIOFACTORY.CreateStreamIO(&diostreamudpcfg);
  if(!diostreamudp)
    {
      GEN_XFACTORY.Delete_Mutex(servermutex);
      servermutex = NULL;
      
      return false;
    }

  AddDNSServer(__L("8.8.8.8"));
  AddDNSServer(__L("8.8.4.4"));
  //AddDNSServer(__L("64.6.64.6"));
  //AddDNSServer(__L("64.6.65.6"));
  
  serverthread = GEN_XFACTORY.CreateThread(XTHREADGROUPID_DIODNSRESOLVED, __L("DIODNSPROTOCOL_MITM_SERVER::Ini"), DIODNSPROTOCOL_MITM_SERVER::ThreadServer, this);  
  if(!serverthread)
    {
      GEN_XFACTORY.Delete_Mutex(servermutex);
      servermutex = NULL;

      GEN_DIOFACTORY.DeleteStreamIO(diostreamudp);
      diostreamudp = NULL;

      return false;
    }

  return serverthread->Ini(false);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIODNSPROTOCOL_MITM_SERVER::Activate(bool active)
* @brief      Activate
* @ingroup    DATAIO
* 
* @param[in]  active : Active value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIODNSPROTOCOL_MITM_SERVER::Activate(bool active)
{
  if(active)
    {  
      if(!diostreamudp->Open())
        {
          return false;
        }

      if(!diostreamudp->WaitToConnected(3000))
        {
          return false;
        }       
    }
   else
    {
      diostreamudp->Close();
    }

  if(!serverthread)
    {
      return false; 
    }

  return serverthread->Run(active);
}





/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIODNSPROTOCOL_MITM_SERVER::Update()
* @brief      Update
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIODNSPROTOCOL_MITM_SERVER::Update()
{ 
  XBUFFER                   receivedbuffer[2]; 
  DIODNSPROTOCOL_HEADER*    header    = NULL;        
  DIODNSPROTOCOL_QUESTION*  question  = NULL; 
  XWORD                     askcount  = 0;
  XBYTE*                    reader    = NULL;  
  int                       stop      = 0;       
  bool                      status    = false;

  if(!diostreamudp)
    {
      return false;
    }

  if(diostreamudp->GetInXBuffer()->GetSize() < sizeof(DIODNSPROTOCOL_HEADER))
    {
      return false;
    }  

  receivedbuffer[0].Empty();
  receivedbuffer[0].Resize(65535);

  XDWORD sizeread = diostreamudp->Read(receivedbuffer[0]);
  if(!sizeread)
    {
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[DSN MitM Server] Error to Read header"));      
      return false;
    }  
  
  header    = (DIODNSPROTOCOL_HEADER*)receivedbuffer[0].Get();
  askcount  = SwapWORD(header->q_count);
      
  reader  = &receivedbuffer[0].Get()[sizeof(DIODNSPROTOCOL_HEADER)];
 
  XSTRING nameURLstr;  

  for(XWORD c=0; c<askcount; c++)
    {
      XBYTE*  nameURL = NULL;
     
      nameURL     =  DIODNSPROTOCOLCLIENT::GetBufferName(reader, receivedbuffer[0].Get(), &stop);
      nameURLstr.Set((char*)nameURL);

      GEN_DELETE_ARRAY nameURL;
      nameURL = NULL;
       
      DIODNSPROTOCOL_MITM_SERVER_XEVENT xevent(this, DIODNSPROTOCOL_MITM_SERVER_XEVENT_TYPE_ASKDNS);
      xevent.GetAskedURL()->Set(nameURLstr);
      xevent.GetAskedBuffer()->CopyFrom(receivedbuffer[0]);

      PostEvent(&xevent);
      
      reader     += stop;
      question    = (DIODNSPROTOCOL_QUESTION*)reader;
    }

  for(XDWORD c=0; c<dnsservers.GetSize(); c++)
    {
      DIODNSRESOLVER_DNSSERVER* DNSserver = dnsservers.Get(c);
      if(DNSserver)
        {
          XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[DSN MitM Server] %d server..."), c);      

          status = Detour(DNSserver, receivedbuffer[0], receivedbuffer[1]);
          if(status)
            {
              XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[DSN MitM Server] Buffer receiver"));      
              XTRACE_PRINTDATABLOCKCOLOR(XTRACE_COLOR_BLUE, receivedbuffer[1]); 

              break;          
            }
           else
            {
              XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[DSN MitM Server] Error to Detour"));      
            }                 
        }    
    }

  status = diostreamudp->Write(receivedbuffer[1]);
  if(status)                        
    {
      //status = diostreamudp->WaitToFlushOutXBuffer(3);                        
      if(status) 
        {
          DIODNSPROTOCOL_MITM_SERVER_XEVENT xevent(this, DIODNSPROTOCOL_MITM_SERVER_XEVENT_TYPE_ANSWERDNS);
          xevent.GetAskedURL()->Set(nameURLstr);
          xevent.GetAnsweredBuffer()->CopyFrom(receivedbuffer[1]);

          PostEvent(&xevent);

          status = true;
        } 
       else
        {
          XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[DSN MitM Server] Error to Write answer buffer"));      
        }                          
    }
   else
    {
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[DSN MitM Server] Error to Write answer"));      
    }     
     
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIODNSPROTOCOL_MITM_SERVER::End()
* @brief      End the object
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIODNSPROTOCOL_MITM_SERVER::End()
{
  DeRegisterEvent(DIODNSPROTOCOL_MITM_SERVER_XEVENT_TYPE_ASKDNS);
  DeRegisterEvent(DIODNSPROTOCOL_MITM_SERVER_XEVENT_TYPE_ANSWERDNS);

  if(serverthread)
    {
      serverthread->End();

      GEN_XFACTORY.DeleteThread(XTHREADGROUPID_DIODNSRESOLVED, serverthread);
      serverthread = NULL;
    }

  if(diostreamudp)
    {
      diostreamudp->Close();

      GEN_DIOFACTORY.DeleteStreamIO(diostreamudp);
      diostreamudp = NULL;
    }
    
  if(servermutex)
    {
      GEN_XFACTORY.Delete_Mutex(servermutex);
      servermutex = NULL;
    }

  DeleteAllDNSServers();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIODNSPROTOCOL_MITM_SERVER::AddDNSServer(XCHAR* server, XWORD port)
* @brief      Add DNS server
* @ingroup    DATAIO
* 
* @param[in]  server : Server pointer to use.
* @param[in]  port : Port number to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIODNSPROTOCOL_MITM_SERVER::AddDNSServer(XCHAR* server, XWORD port)
{
  if(!server)
    {
      return false;
    }

  DIODNSRESOLVER_DNSSERVER* dnsserver = GEN_NEW DIODNSRESOLVER_DNSSERVER();
  if(!dnsserver)
    {
      return false;
    }

  dnsserver->GetIP()->Set(server);
  dnsserver->SetPort(port);

  return dnsservers.Add(dnsserver);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIODNSPROTOCOL_MITM_SERVER::AddDNSServer(XSTRING& server, XWORD port)
* @brief      Add DNS server
* @ingroup    DATAIO
* 
* @param[in]  server : Server value.
* @param[in]  port : Port number to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIODNSPROTOCOL_MITM_SERVER::AddDNSServer(XSTRING& server, XWORD port)
{
  return AddDNSServer(server.Get(), port);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIODNSPROTOCOL_MITM_SERVER::AddDNSServer(XBYTE* serverIP, XWORD port)
* @brief      Add DNS server
* @ingroup    DATAIO
* 
* @param[in]  serverIP : Server IP pointer to use.
* @param[in]  port : Port number to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIODNSPROTOCOL_MITM_SERVER::AddDNSServer(XBYTE* serverIP, XWORD port)
{
  if(!serverIP)
    {
      return false;
    }

  DIODNSRESOLVER_DNSSERVER* dnsserver = GEN_NEW DIODNSRESOLVER_DNSSERVER();
  if(!dnsserver)
    {
      return false;
    }

  dnsserver->GetIP()->Set(serverIP);
  dnsserver->SetPort(port);

  return dnsservers.Add(dnsserver);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIODNSPROTOCOL_MITM_SERVER::AddDNSServer(DIOIP& serverIP, XWORD port)
* @brief      Add DNS server
* @ingroup    DATAIO
* 
* @param[in]  serverIP : Server IP value.
* @param[in]  port : Port number to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIODNSPROTOCOL_MITM_SERVER::AddDNSServer(DIOIP& serverIP, XWORD port)
{
  DIODNSRESOLVER_DNSSERVER* dnsserver = GEN_NEW DIODNSRESOLVER_DNSSERVER();
  if(!dnsserver)
    {
      return false;
    }

  dnsserver->GetIP()->Set(serverIP.Get());
  dnsserver->SetPort(port);

  return dnsservers.Add(dnsserver);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIODNSPROTOCOL_MITM_SERVER::DeleteAllDNSServers()
* @brief      Delete all DNS servers
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIODNSPROTOCOL_MITM_SERVER::DeleteAllDNSServers()
{
  if(dnsservers.IsEmpty())
    {
      return false;
    }

  dnsservers.DeleteContents();
  dnsservers.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XMUTEX* DIODNSPROTOCOL_MITM_SERVER::GetServerMutex()
* @brief      Get server mutex
* @ingroup    DATAIO
* 
* @return     XMUTEX* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XMUTEX* DIODNSPROTOCOL_MITM_SERVER::GetServerMutex()
{
  return servermutex;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIODNSPROTOCOL_MITM_SERVER::Detour(DIODNSRESOLVER_DNSSERVER* DNSserver, XBUFFER& sendbuffer, XBUFFER& receivedbuffer)
* @brief      Detour
* @ingroup    DATAIO
* 
* @param[in]  DNSserver : DN Sserver pointer to use.
* @param[in]  sendbuffer : Sendbuffer value.
* @param[in]  receivedbuffer : Receivedbuffer value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIODNSPROTOCOL_MITM_SERVER::Detour(DIODNSRESOLVER_DNSSERVER* DNSserver, XBUFFER& sendbuffer, XBUFFER& receivedbuffer)
{
  if(!DNSserver)
    {
      return false;
    }

  DIOSTREAMUDPCONFIG   dioudpcfg;
	DIOSTREAMUDP*				 dioudp     = NULL;
  XSTRING              remoteIP; 
  int                  serverport = 0;
  bool                 status     = false;

  DNSserver->GetIP()->GetXString(remoteIP);

  dioudpcfg.SetMode(DIOSTREAMMODE_CLIENT);
  dioudpcfg.SetIsUsedDatagrams(false);
  dioudpcfg.GetRemoteURL()->Set(remoteIP);

  serverport = DNSserver->GetPort();

  if(!serverport)
    {
      dioudpcfg.SetRemotePort(DIODNSPROTOCOL_DEFAULTPORT);
    }
   else  
    {
      dioudpcfg.SetRemotePort(serverport);
    }

  
	dioudp = (DIOSTREAMUDP*)GEN_DIOFACTORY.CreateStreamIO(&dioudpcfg);
  if(dioudp)
    {
      if(dioudp->Open())
        {                   
          status = dioudp->Write(sendbuffer);
          if(status) 
            {
              status = diostreamudp->WaitToFlushOutXBuffer(3);
            }

          if(status)
            {
              receivedbuffer.Empty();
              receivedbuffer.Resize(65535);
              
              status = diostreamudp->WaitToFilledReadingBuffer(sizeof(DIODNSPROTOCOL_HEADER), 2);
              if(status) 
                {
                  status = diostreamudp->Read(receivedbuffer);
                }
            }
          
          dioudp->Close();
        }

      GEN_DIOFACTORY.DeleteStreamIO(dioudp);  
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIODNSPROTOCOL_MITM_SERVER::ThreadServer(void* param)
* @brief      Thread server
* @ingroup    DATAIO
* 
* @param[in]  param : Param pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIODNSPROTOCOL_MITM_SERVER::ThreadServer(void* param)
{
  DIODNSPROTOCOL_MITM_SERVER* mitm_server = (DIODNSPROTOCOL_MITM_SERVER*)param;
  if(!mitm_server)
    {
      return;
    }

  if(mitm_server->GetServerMutex())
    {
      mitm_server->GetServerMutex()->Lock();
    }

  mitm_server->Update();
    
  if(mitm_server->GetServerMutex())
    {
      mitm_server->GetServerMutex()->UnLock();
    }

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIODNSPROTOCOL_MITM_SERVER::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIODNSPROTOCOL_MITM_SERVER::Clean()
{
  serverthread    = NULL;
  servermutex     = NULL;   

  diostreamudp    = NULL;
}



