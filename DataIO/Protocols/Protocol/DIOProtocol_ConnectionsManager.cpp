/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOProtocol_ConnectionsManager.cpp
* 
* @class      DIOPROTOCOL_CONNECTIONSMANAGER
* @brief      Data Input/Output Protocol Connections Manager class
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

#include "DIOProtocol_ConnectionsManager.h"

#include "XFactory.h"
#include "XSleep.h"
#include "XTimer.h"
#include "XTrace.h"
#include "XThreadCollected.h"

#include "DIOFactory.h"
#include "DIOIP.h"
#include "DIOURL.h"
#include "DIOStream_XEvent.h"
#include "DIOStreamDeviceIP.h"
#include "DIOStreamUDPLocalEnumServers.h"
#include "DIOStreamTCPIPConfig.h"
#include "DIOStreamTCPIP.h"
#include "DIOProtocolConnectionsInUseDefinitions.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOPROTOCOL_CONNECTION::DIOPROTOCOL_CONNECTION(DIOSTREAMCONFIG* diostreamcfg)
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* @param[in]  diostreamcfg : Diostreamconfiguration pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOPROTOCOL_CONNECTION::DIOPROTOCOL_CONNECTION(DIOSTREAMCONFIG* diostreamcfg)
{
  Clean();

  this->diostreamcfg  = diostreamcfg;

  xmutexinuseID=GEN_XFACTORY.Create_Mutex();

  inuseID.SetIsMulti(true);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOPROTOCOL_CONNECTION::~DIOPROTOCOL_CONNECTION()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOPROTOCOL_CONNECTION::~DIOPROTOCOL_CONNECTION()
{
  GEN_XFACTORY.Delete_Mutex(xmutexinuseID);

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSTREAMCONFIG* DIOPROTOCOL_CONNECTION::GetDIOStreamConfig()
* @brief      Get DIO stream config
* @ingroup    DATAIO
* 
* @return     DIOSTREAMCONFIG* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTREAMCONFIG* DIOPROTOCOL_CONNECTION::GetDIOStreamConfig()
{
  return diostreamcfg;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOPROTOCOL* DIOPROTOCOL_CONNECTION::GetDIOProtocol()
* @brief      Get DIO protocol
* @ingroup    DATAIO
* 
* @return     DIOPROTOCOL* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOPROTOCOL* DIOPROTOCOL_CONNECTION::GetDIOProtocol()
{
  return dioprotocol;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOPROTOCOL_CONNECTION::SetDIOProtocol(DIOPROTOCOL* protocol)
* @brief      Set DIO protocol
* @ingroup    DATAIO
* 
* @param[in]  protocol : Protocol pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOPROTOCOL_CONNECTION::SetDIOProtocol(DIOPROTOCOL* protocol)
{
  this->dioprotocol = protocol;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL_CONNECTION::Connect()
* @brief      Connect
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL_CONNECTION::Connect()
{
  if(!dioprotocol) return false;

  return dioprotocol->Connect();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL_CONNECTION::Disconected()
* @brief      Disconected
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL_CONNECTION::Disconected()
{
  if(!dioprotocol)  return false;

  if(dioprotocol->IsConnected())
    {
      dioprotocol->Disconnect();
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL_CONNECTION::SetInUse(bool inuse, XDWORD ID)
* @brief      Set in use
* @ingroup    DATAIO
* 
* @param[in]  inuse : Inuse value.
* @param[in]  ID : Identifier to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL_CONNECTION::SetInUse(bool inuse, XDWORD ID)
{
  if(!xmutexinuseID)  return false;

  xmutexinuseID->Lock();

  if(inuse)
        inuseID.Add(ID);
    else inuseID.Delete(ID);

  xmutexinuseID->UnLock();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL_CONNECTION::IsInUse()
* @brief      Is in use
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL_CONNECTION::IsInUse()
{
  bool inuse = false;

  if(!xmutexinuseID)

  xmutexinuseID->Lock();
  inuse = (inuseID.GetSize()?true:false);
  xmutexinuseID->UnLock();

  return inuse;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL_CONNECTION::IsSendEventConnected()
* @brief      Is send event connected
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL_CONNECTION::IsSendEventConnected()
{
  return issendeventconnected;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOPROTOCOL_CONNECTION::SetIsSendEventConnected(bool issendeventconnected)
* @brief      Set is send event connected
* @ingroup    DATAIO
* 
* @param[in]  issendeventconnected : Issendeventconnected value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOPROTOCOL_CONNECTION::SetIsSendEventConnected(bool issendeventconnected)
{
  this->issendeventconnected = issendeventconnected;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOPROTOCOL_CONNECTION::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOPROTOCOL_CONNECTION::Clean()
{
  diostreamcfg            = NULL;
  dioprotocol             = NULL;
  issendeventconnected    = false;

  xmutexinuseID           = NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOPROTOCOL_CONNECTIONSMANAGER::DIOPROTOCOL_CONNECTIONSMANAGER()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOPROTOCOL_CONNECTIONSMANAGER::DIOPROTOCOL_CONNECTIONSMANAGER()
{
  Clean();

  protocolconnectionsnlimit  = DIOPROTOCOL_CONNECTIONS_UNLIMITEDNCONNECTIONS;

  RegisterEvent(DIOPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_CONNECTEDCONNECTION);
  RegisterEvent(DIOPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_INITPROTOCOL);
  RegisterEvent(DIOPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_DISCONNECTEDCONNECTION);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOPROTOCOL_CONNECTIONSMANAGER::~DIOPROTOCOL_CONNECTIONSMANAGER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOPROTOCOL_CONNECTIONSMANAGER::~DIOPROTOCOL_CONNECTIONSMANAGER()
{
  DeRegisterEvent(DIOPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_CONNECTEDCONNECTION);
  DeRegisterEvent(DIOPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_INITPROTOCOL);
  DeRegisterEvent(DIOPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_DISCONNECTEDCONNECTION);

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL_CONNECTIONSMANAGER::Ini(bool isserver, DIOSTREAMCONFIG* diostreamcfg, DIOSTREAMENUMSERVERS* diostreamenumservers)
* @brief      Initialize the object
* @ingroup    DATAIO
* 
* @param[in]  isserver : Isserver value.
* @param[in]  diostreamcfg : Diostreamconfiguration pointer to use.
* @param[in]  diostreamenumservers : Diostreamenumservers pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL_CONNECTIONSMANAGER::Ini(bool isserver, DIOSTREAMCONFIG* diostreamcfg, DIOSTREAMENUMSERVERS* diostreamenumservers)
{
  if(!diostreamcfg) return false;

  this->isserver              = isserver;
  this->diostreamcfg          = diostreamcfg;
  this->diostreamenumservers  = diostreamenumservers;

  this->diostreamcfg->SetMode(isserver?DIOSTREAMMODE_SERVER:DIOSTREAMMODE_CLIENT);

  xtimerout=GEN_XFACTORY.CreateTimer();
  if(!xtimerout)  return false;
  
  xmutexprotocolconnections=GEN_XFACTORY.Create_Mutex();
  if(!xmutexprotocolconnections) return false;

  xtimerconnections=GEN_XFACTORY.CreateTimer();
  if(xtimerconnections)
    {      
      xthreadconnections = CREATEXTHREAD(XTHREADGROUPID_DIOPROTOCOL_CONNECTIONMANAGER, __L("DIOPROTOCOL_CONNECTIONSMANAGER::Ini"), ThreadProtocolConnections, (void*)this);
      if(xthreadconnections)
        {
          if(!xthreadconnections->Ini()) return false;
        }
    }

  xtimerclienttry=GEN_XFACTORY.CreateTimer();
  if(!xtimerclienttry) return false;
  
  xtimerclienttry->AddSeconds(DIOPROTOCOL_CONNECTIONS_DEFAULTIMETRYCONNECTIONS+1);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL_CONNECTIONSMANAGER::End()
* @brief      End the object
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL_CONNECTIONSMANAGER::End()
{
  WaitToAllConnectionsCanBeDeleted();

  if(xthreadconnections)
    {
      xthreadconnections->End();
      DELETEXTHREAD(XTHREADGROUPID_DIOPROTOCOL_CONNECTIONMANAGER, xthreadconnections);
      xthreadconnections = NULL;
    }

  ProtocolConnections_DeleteAllWaitConnections();

  ProtocolConnections_DisconnectAll();

  ProtocolConnections_DeleteAll();

  if(xtimerclienttry)
    {
      GEN_XFACTORY.DeleteTimer(xtimerclienttry);
      xtimerclienttry = NULL;
    }

  if(xtimerconnections)
    {
      GEN_XFACTORY.DeleteTimer(xtimerconnections);
      xtimerconnections = NULL;
    }

  if(xtimerout)
    {
      GEN_XFACTORY.DeleteTimer(xtimerout);
      xtimerout = NULL;
    }

  if(xmutexprotocolconnections)
    {
      GEN_XFACTORY.Delete_Mutex(xmutexprotocolconnections);
      xmutexprotocolconnections = NULL;
    }

  TargetURL_DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOPROTOCOL_CONNECTIONSMANAGER::TargetURL_GetNTargets()
* @brief      Target URL get N targets
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOPROTOCOL_CONNECTIONSMANAGER::TargetURL_GetNTargets()
{
  return targetURLs.GetSize();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL_CONNECTIONSMANAGER::TargetURL_Add(XCHAR* URL)
* @brief      Target URL add
* @ingroup    DATAIO
* 
* @param[in]  URL : URL to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL_CONNECTIONSMANAGER::TargetURL_Add(XCHAR* URL)
{
  if(!URL)    return false;

  DIOURL* _URL = GEN_NEW DIOURL();
  if(!_URL) return false;

  (*_URL) = URL;

  targetURLs.Add(_URL);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL_CONNECTIONSMANAGER::TargetURL_Add(XSTRING& URL)
* @brief      Target URL add
* @ingroup    DATAIO
* 
* @param[in]  URL : URL to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL_CONNECTIONSMANAGER::TargetURL_Add(XSTRING& URL)
{
  return TargetURL_Add(URL.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL_CONNECTIONSMANAGER::TargetURL_Add(DIOURL& URL)
* @brief      Target URL add
* @ingroup    DATAIO
* 
* @param[in]  URL : URL to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL_CONNECTIONSMANAGER::TargetURL_Add(DIOURL& URL)
{
  return TargetURL_Add(URL.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOPROTOCOL_CONNECTIONSMANAGER::TargetURL_Get(int index)
* @brief      Target URL get
* @ingroup    DATAIO
* 
* @param[in]  index : Index value.
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOPROTOCOL_CONNECTIONSMANAGER::TargetURL_Get(int index)
{
  return (XSTRING*)targetURLs.Get(index);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL_CONNECTIONSMANAGER::TargetURL_Delete(int index)
* @brief      Target URL delete
* @ingroup    DATAIO
* 
* @param[in]  index : Index value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL_CONNECTIONSMANAGER::TargetURL_Delete(int index)
{
  DIOURL* URL = (DIOURL*)targetURLs.Get(index);
  if(!URL) return false;

  targetURLs.Delete(URL);

  GEN_DELETE URL;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL_CONNECTIONSMANAGER::TargetURL_DeleteAll()
* @brief      Target URL GEN_DELETE all
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL_CONNECTIONSMANAGER::TargetURL_DeleteAll()
{
  if(targetURLs.IsEmpty())  return false;

  targetURLs.DeleteContents();
  targetURLs.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOPROTOCOL_CONNECTIONSMANAGER::ProtocolConnections_GetNLimit()
* @brief      Protocol connections get N limit
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOPROTOCOL_CONNECTIONSMANAGER::ProtocolConnections_GetNLimit()
{
  return protocolconnectionsnlimit;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOPROTOCOL_CONNECTIONSMANAGER::ProtocolConnections_SetNLimit(int protocolconnectionsnlimit)
* @brief      Protocol connections set N limit
* @ingroup    DATAIO
* 
* @param[in]  protocolconnectionsnlimit : Protocolconnectionsnlimit value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOPROTOCOL_CONNECTIONSMANAGER::ProtocolConnections_SetNLimit(int protocolconnectionsnlimit)
{
  this->protocolconnectionsnlimit = protocolconnectionsnlimit;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOPROTOCOL_CONNECTIONSMANAGER::ProtocolConnections_GetNAvailable()
* @brief      Protocol connections get N available
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOPROTOCOL_CONNECTIONSMANAGER::ProtocolConnections_GetNAvailable()
{
  return protocolconnections.GetSize();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOPROTOCOL_CONNECTION* DIOPROTOCOL_CONNECTIONSMANAGER::ProtocolConnections_Get(int index)
* @brief      Protocol connections get
* @ingroup    DATAIO
* 
* @param[in]  index : Index value.
* 
* @return     DIOPROTOCOL_CONNECTION* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOPROTOCOL_CONNECTION* DIOPROTOCOL_CONNECTIONSMANAGER::ProtocolConnections_Get(int index)
{
  if(!xmutexprotocolconnections)  return NULL;

  xmutexprotocolconnections->Lock();

  DIOPROTOCOL_CONNECTION* protocolconnection = NULL;

  if(!protocolconnections.IsEmpty())
    {
      if(index<(int)ProtocolConnections_GetNAvailable())
        {
          protocolconnection = protocolconnections.Get(index);
        }
    }

  xmutexprotocolconnections->UnLock();

  return protocolconnection;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOPROTOCOL_CONNECTION* DIOPROTOCOL_CONNECTIONSMANAGER::ProtocolConnections_GetFirstConnected()
* @brief      Protocol connections get first connected
* @ingroup    DATAIO
* 
* @return     DIOPROTOCOL_CONNECTION* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOPROTOCOL_CONNECTION* DIOPROTOCOL_CONNECTIONSMANAGER::ProtocolConnections_GetFirstConnected()
{
  if(!xmutexprotocolconnections)  return NULL;

  xmutexprotocolconnections->Lock();

  DIOPROTOCOL_CONNECTION* protocolconnection = NULL;
  if(!protocolconnections.IsEmpty())
    {
      for(int c=0; c<(int)ProtocolConnections_GetNAvailable(); c++)
        {
          protocolconnection = protocolconnections.Get(c);
          if(protocolconnection)
            {
              DIOPROTOCOL* protocol = protocolconnection->GetDIOProtocol();
              if(protocol)
                {
                  if(protocol->IsConnected())
                    {
                      protocolconnection = protocolconnections.Get(c);
                      break;

                    } else protocolconnection = NULL;

                } else protocolconnection = NULL;
            }
        }
    }

  xmutexprotocolconnections->UnLock();

  return protocolconnection;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOPROTOCOL_CONNECTION* DIOPROTOCOL_CONNECTIONSMANAGER::ProtocolConnections_GetFirstOperative()
* @brief      Protocol connections get first operative
* @ingroup    DATAIO
* 
* @return     DIOPROTOCOL_CONNECTION* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOPROTOCOL_CONNECTION* DIOPROTOCOL_CONNECTIONSMANAGER::ProtocolConnections_GetFirstOperative()
{
  if(!xmutexprotocolconnections)  return NULL;

  xmutexprotocolconnections->Lock();

  DIOPROTOCOL_CONNECTION* protocolconnection = NULL;
  if(!protocolconnections.IsEmpty())
    {
      for(int c=0; c<(int)ProtocolConnections_GetNAvailable(); c++)
        {
          protocolconnection = protocolconnections.Get(c);
          if(protocolconnection)
            {
              DIOPROTOCOL* protocol = protocolconnection->GetDIOProtocol();
              if(protocol)
                {
                  if(protocol->IsOperational())
                    {
                      protocolconnection = protocolconnections.Get(c);
                      break;

                    } else protocolconnection = NULL;

                } else protocolconnection = NULL;
            }
        }
    }

  xmutexprotocolconnections->UnLock();

  return protocolconnection;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOPROTOCOL_CONNECTION* DIOPROTOCOL_CONNECTIONSMANAGER::ProtocolConnections_GetByDIOStream(DIOSTREAM* diostream)
* @brief      Protocol connections get by DIO stream
* @ingroup    DATAIO
* 
* @param[in]  diostream : Diostream pointer to use.
* 
* @return     DIOPROTOCOL_CONNECTION* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOPROTOCOL_CONNECTION* DIOPROTOCOL_CONNECTIONSMANAGER::ProtocolConnections_GetByDIOStream(DIOSTREAM* diostream)
{
  DIOPROTOCOL_CONNECTION* protocolconnection = NULL;

  if(!xmutexprotocolconnections)  return NULL;

  xmutexprotocolconnections->Lock();

  if(!protocolconnections.IsEmpty())
    {
      for(int index=0; index< ProtocolConnections_GetNAvailable(); index++)
        {
          DIOPROTOCOL_CONNECTION* _protocolconnection = (DIOPROTOCOL_CONNECTION*)protocolconnections.Get(index);
          if(_protocolconnection)
            {
              if(_protocolconnection->GetDIOProtocol())
                {
                  if(_protocolconnection->GetDIOProtocol()->GetDIOStream() == diostream)
                    {
                      protocolconnection = _protocolconnection;
                      break;
                    }
                }
            }
        }
    }

  xmutexprotocolconnections->UnLock();

  return protocolconnection;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOPROTOCOL* DIOPROTOCOL_CONNECTIONSMANAGER::ProtocolConnections_GetProtocol(int index)
* @brief      Protocol connections get protocol
* @ingroup    DATAIO
* 
* @param[in]  index : Index value.
* 
* @return     DIOPROTOCOL* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOPROTOCOL* DIOPROTOCOL_CONNECTIONSMANAGER::ProtocolConnections_GetProtocol(int index)
{
  DIOPROTOCOL* protocol = NULL;

  if(!xmutexprotocolconnections)  return NULL;

  xmutexprotocolconnections->Lock();

  if(index<(int)ProtocolConnections_GetNAvailable())
    {
      DIOPROTOCOL_CONNECTION* protocolconnection = (DIOPROTOCOL_CONNECTION*)protocolconnections.Get(index);
      if(protocolconnection)
        {
          if(protocolconnection->GetDIOProtocol())
            {
              if(protocolconnection->GetDIOProtocol()->IsConnected())
                {
                  protocol = protocolconnection->GetDIOProtocol();
                }
            }
        }
    }

  xmutexprotocolconnections->UnLock();

  return protocol;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOPROTOCOL_CONNECTIONSMANAGER::ProtocolConnections_GetNConnected()
* @brief      Protocol connections get N connected
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOPROTOCOL_CONNECTIONSMANAGER::ProtocolConnections_GetNConnected()
{
  if(!xmutexprotocolconnections)  return false;

  xmutexprotocolconnections->Lock();

  int nconnections      = 0;
  int ntotalconnections = ProtocolConnections_GetNAvailable();

  for(int index=0; index < ntotalconnections; index++)
    {
      DIOPROTOCOL_CONNECTION* connection = (DIOPROTOCOL_CONNECTION*)protocolconnections.Get(index);
      if(connection)
        {
          if(connection->GetDIOProtocol())
            {
              if(connection->GetDIOProtocol()->IsConnected()) nconnections++;
            }
        }
    }

  xmutexprotocolconnections->UnLock();

  return nconnections;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOPROTOCOL_CONNECTIONSMANAGER::ProtocolConnections_GetNFreeToConnect()
* @brief      Protocol connections get N free to connect
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOPROTOCOL_CONNECTIONSMANAGER::ProtocolConnections_GetNFreeToConnect()
{
  if(!xmutexprotocolconnections)  return 0;

  xmutexprotocolconnections->Lock();

  int nconnections      = 0;
  int ntotalconnections = ProtocolConnections_GetNAvailable();

  for(int index=0; index < ntotalconnections; index++)
    {
      DIOPROTOCOL_CONNECTION* connection = (DIOPROTOCOL_CONNECTION*)protocolconnections.Get(index);
      if(connection)
        {
          if(connection->GetDIOProtocol())
            {
              if(connection->GetDIOProtocol()->IsConnecting()) nconnections++;
            }
        }
    }

  xmutexprotocolconnections->UnLock();

  return nconnections;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL_CONNECTIONSMANAGER::ProtocolConnections_SendEventConnected()
* @brief      Protocol connections send event connected
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL_CONNECTIONSMANAGER::ProtocolConnections_SendEventConnected()
{
  if(!xmutexprotocolconnections)  return false;

  xmutexprotocolconnections->Lock();

  int ntotalconnections = ProtocolConnections_GetNAvailable();

  for(int index = 0; index < ntotalconnections; index++)
    {
      DIOPROTOCOL_CONNECTION* connection = (DIOPROTOCOL_CONNECTION*)protocolconnections.Get(index);
      if(connection)
        {
          if(!connection->IsSendEventConnected())
            {
              if(connection->GetDIOProtocol())
                {
                  if(connection->GetDIOProtocol()->IsConnected())
                    {
                      bool sendeventconnected = SendEvent(DIOPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_CONNECTEDCONNECTION, connection);

                      connection->SetIsSendEventConnected(sendeventconnected);
                    }
                }
            }
        }
    }

  xmutexprotocolconnections->UnLock();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL_CONNECTIONSMANAGER::ProtocolConnections_DeleteAllWaitConnections()
* @brief      Protocol connections GEN_DELETE all wait connections
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL_CONNECTIONSMANAGER::ProtocolConnections_DeleteAllWaitConnections()
{
  int index = 0;

  if(!xmutexprotocolconnections)  return false;

  xmutexprotocolconnections->Lock();

  while(index < ProtocolConnections_GetNAvailable())
    {
      DIOPROTOCOL_CONNECTION* protocolconnection = (DIOPROTOCOL_CONNECTION*)protocolconnections.Get(index);
      if(protocolconnection)
        {
          if(protocolconnection->GetDIOProtocol())
            {
              if(protocolconnection->GetDIOProtocol()->GetDIOStream())
                {
                  if(protocolconnection->GetDIOProtocol()->GetDIOStream()->GetStatus() == DIOSTREAMSTATUS_GETTINGCONNECTION)
                    {
                      DeleteProtocol(protocolconnection->GetDIOProtocol());
                      protocolconnection->SetDIOProtocol(NULL);

                      protocolconnections.Delete(protocolconnection);
                      GEN_DELETE protocolconnection;

                    } else index++;

                } else index++;

            } else index++;
        }
    }

  xmutexprotocolconnections->UnLock();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL_CONNECTIONSMANAGER::ProtocolConnections_DeleteDisconnected()
* @brief      Protocol connections GEN_DELETE disconnected
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL_CONNECTIONSMANAGER::ProtocolConnections_DeleteDisconnected()
{
  int index = 0;

  if(!xmutexprotocolconnections)  return false;

  xmutexprotocolconnections->Lock();

  while(index < ProtocolConnections_GetNAvailable())
    {
      DIOPROTOCOL_CONNECTION* protocolconnection = (DIOPROTOCOL_CONNECTION*)protocolconnections.Get(index);
      if(protocolconnection)
        {
          if(protocolconnection->GetDIOProtocol())
            {
              if((protocolconnection->GetDIOProtocol()->IsDisconnected())  && (!protocolconnection->IsInUse()))
                {
                  SendEvent(DIOPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_DISCONNECTEDCONNECTION, protocolconnection);
                  protocolconnection->Disconected();

                  DeleteProtocol(protocolconnection->GetDIOProtocol());
                  protocolconnection->SetDIOProtocol(NULL);

                  protocolconnections.Delete(protocolconnection);
                  GEN_DELETE protocolconnection;

                } else index++;

            } else index++;
        }
    }

  xmutexprotocolconnections->UnLock();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL_CONNECTIONSMANAGER::ProtocolConnections_Disconnect(int index)
* @brief      Protocol connections disconnect
* @ingroup    DATAIO
* 
* @param[in]  index : Index value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL_CONNECTIONSMANAGER::ProtocolConnections_Disconnect(int index)
{
  bool status = false;

  if(!xmutexprotocolconnections)  return false;

  xmutexprotocolconnections->Lock();

  if(index<(int)ProtocolConnections_GetNAvailable())
    {
      DIOPROTOCOL_CONNECTION* protocolconnection = (DIOPROTOCOL_CONNECTION*)protocolconnections.Get(index);
      if(protocolconnection)
        {
          if(protocolconnection->GetDIOProtocol())
            {
              if(protocolconnection->GetDIOProtocol()->IsConnected())
                {
                  SendEvent(DIOPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_DISCONNECTEDCONNECTION, protocolconnection);
                  protocolconnection->Disconected();

                  status = true;
                }
            }
        }
    }

  xmutexprotocolconnections->UnLock();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL_CONNECTIONSMANAGER::ProtocolConnections_DisconnectAll()
* @brief      Protocol connections disconnect all
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL_CONNECTIONSMANAGER::ProtocolConnections_DisconnectAll()
{
  if(!xmutexprotocolconnections)  return false;

  xmutexprotocolconnections->Lock();

  if(!protocolconnections.IsEmpty())
    {
      int ntotalconnections = ProtocolConnections_GetNAvailable();

      for(int index=0; index < ntotalconnections; index++)
        {
          DIOPROTOCOL_CONNECTION* protocolconnection = (DIOPROTOCOL_CONNECTION*)protocolconnections.Get(index);
          if(protocolconnection)
            {
              if(protocolconnection->GetDIOProtocol())
                {
                  if(protocolconnection->GetDIOProtocol()->IsConnected())
                    {
                      SendEvent(DIOPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_DISCONNECTEDCONNECTION, protocolconnection);
                      protocolconnection->Disconected();
                    }
                }
            }
        }
    }

  xmutexprotocolconnections->UnLock();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL_CONNECTIONSMANAGER::ProtocolConnections_Delete(int index)
* @brief      Protocol connections delete
* @ingroup    DATAIO
* 
* @param[in]  index : Index value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL_CONNECTIONSMANAGER::ProtocolConnections_Delete(int index)
{
  bool status = false;

  if(!xmutexprotocolconnections)  return false;

  xmutexprotocolconnections->Lock();

  if(index<(int)ProtocolConnections_GetNAvailable())
    {
      DIOPROTOCOL_CONNECTION* protocolconnection = (DIOPROTOCOL_CONNECTION*)protocolconnections.Get(index);
      if(protocolconnection)
        {
          if(protocolconnection->GetDIOProtocol())
            {
              if(!protocolconnection->IsInUse())
                {
                  DIOPROTOCOL* protocol = protocolconnection->GetDIOProtocol();

                  protocolconnections.Delete(protocolconnection);
                  GEN_DELETE protocolconnection;

                  DeleteProtocol(protocol);

                  status = true;
                }
            }
        }
    }

  xmutexprotocolconnections->UnLock();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL_CONNECTIONSMANAGER::ProtocolConnections_DeleteAll()
* @brief      Protocol connections GEN_DELETE all
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL_CONNECTIONSMANAGER::ProtocolConnections_DeleteAll()
{
  if(!xmutexprotocolconnections)  return false;

  xmutexprotocolconnections->Lock();

  int index = 0;

  while(index < ProtocolConnections_GetNAvailable())
    {
      DIOPROTOCOL_CONNECTION* protocolconnection = (DIOPROTOCOL_CONNECTION*)protocolconnections.Get(index);
      if(protocolconnection)
        {
          if((!protocolconnection->IsInUse()))
            {
              DIOPROTOCOL* protocol = protocolconnection->GetDIOProtocol();

              protocolconnections.Delete(protocolconnection);
              GEN_DELETE protocolconnection;

              DeleteProtocol(protocol);

            } else index++;

        } else index++;
    }

  xmutexprotocolconnections->UnLock();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL_CONNECTIONSMANAGER::WaitToAnyConnectionIsConnected(int timeout)
* @brief      Wait to any connection is connected
* @ingroup    DATAIO
* 
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL_CONNECTIONSMANAGER::WaitToAnyConnectionIsConnected(int timeout)
{
  int  index  = 0;
  bool status = true;

  xtimerout->Reset();

  while(1)
    {
      if(ProtocolConnections_GetNConnected())
        {
          status = false;
          break;
        }

      GEN_XSLEEP.MilliSeconds(10);

      if(xtimerout->GetMeasureSeconds() >= (XDWORD)timeout)
        {
          status = false;
          break;
        }
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL_CONNECTIONSMANAGER::WaitToAllConnectionsCanBeDeleted(int timeout)
* @brief      Wait to all connections can be deleted
* @ingroup    DATAIO
* 
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL_CONNECTIONSMANAGER::WaitToAllConnectionsCanBeDeleted(int timeout)
{
  if(!xmutexprotocolconnections)  return false;

  xmutexprotocolconnections->Lock();

  int  index  = 0;
  bool status = true;

  if(timeout) xtimerout->Reset();

  while(1)
    {
      bool candelete = true;

      for(int index=0; index< ProtocolConnections_GetNAvailable(); index++)
        {
          DIOPROTOCOL_CONNECTION* protocolconnection = (DIOPROTOCOL_CONNECTION*)protocolconnections.Get(index);
          if(protocolconnection)
            {
              if(protocolconnection->IsInUse()) candelete = false;
            }
        }

      if(candelete) break;

      GEN_XSLEEP.MilliSeconds(10);

      if(timeout)
        {
          if(xtimerout->GetMeasureSeconds() > 0)
            {
              if(xtimerout->GetMeasureSeconds() >= (XDWORD)timeout)
                {
                  status = false;
                  break;
                }
            }
        }
    }

  xmutexprotocolconnections->UnLock();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPROTOCOL_CONNECTIONSMANAGER::SendEvent(DIOPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE type, DIOPROTOCOL_CONNECTION* protocolconnection)
* @brief      Send event
* @ingroup    DATAIO
* 
* @param[in]  type : Type value.
* @param[in]  protocolconnection : Protocolconnection pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPROTOCOL_CONNECTIONSMANAGER::SendEvent(DIOPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE type, DIOPROTOCOL_CONNECTION* protocolconnection)
{
  if(!protocolconnection) return false;

  DIOPROTOCOL_CONNECTIONSMANAGER_XEVENT xevent(this, type);

  xevent.SetDIOStreamEnumServers(diostreamenumservers);
  xevent.SetDIOStreamConfig(protocolconnection->GetDIOStreamConfig());
  xevent.SetDIOProtocol(protocolconnection->GetDIOProtocol());
  xevent.SetProtocolConnection(protocolconnection);
  xevent.SetProtocolConnectionsManager(this);

  return PostEvent(&xevent);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOPROTOCOL_CONNECTIONSMANAGER::Application_GetVersion(XDWORD& version, XDWORD& subversion, XDWORD& subversionerr)
* @brief      Application get version
* @ingroup    DATAIO
* 
* @param[in]  version : Version value.
* @param[in]  subversion : Subversion value.
* @param[in]  subversionerr : Subversionerr value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOPROTOCOL_CONNECTIONSMANAGER::Application_GetVersion(XDWORD& version, XDWORD& subversion, XDWORD& subversionerr)
{
  version         = this->applicationversion;
  subversion      = this->applicationsubversion;
  subversionerr   = this->applicationsubversionerr;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOPROTOCOL_CONNECTIONSMANAGER::Application_SetVersion(XDWORD version, XDWORD subversion, XDWORD subversionerr)
* @brief      Application set version
* @ingroup    DATAIO
* 
* @param[in]  version : Version value.
* @param[in]  subversion : Subversion value.
* @param[in]  subversionerr : Subversionerr value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOPROTOCOL_CONNECTIONSMANAGER::Application_SetVersion(XDWORD version, XDWORD subversion, XDWORD subversionerr)
{
  this->applicationversion        = version;
  this->applicationsubversion     = subversion;
  this->applicationsubversionerr  = subversionerr;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOPROTOCOL_CONNECTIONSMANAGER::Application_GetName()
* @brief      Application get name
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOPROTOCOL_CONNECTIONSMANAGER::Application_GetName()
{
  return &applicationname;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOPROTOCOL_CONNECTIONSMANAGER::ManageProtocolConnectionsServer()
* @brief      Manage protocol connections server
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOPROTOCOL_CONNECTIONSMANAGER::ManageProtocolConnectionsServer()
{
  if(!xthreadconnections)              return;
  if(!xthreadconnections->IsRunning()) return;

  xtimerconnections->Reset();

  ProtocolConnections_DeleteDisconnected();

  if(protocolconnectionsnlimit != DIOPROTOCOL_CONNECTIONS_UNLIMITEDNCONNECTIONS)
    {
      if(!protocolconnectionsnlimit) return;

      if(ProtocolConnections_GetNConnected() >= protocolconnectionsnlimit)
        {
          return;
        }
    }

  ProtocolConnections_SendEventConnected();

  if(!ProtocolConnections_GetNFreeToConnect())
    {
      DIOPROTOCOL_CONNECTION* protocolconnection = GEN_NEW DIOPROTOCOL_CONNECTION(diostreamcfg);
      if(protocolconnection)
        {
          DIOPROTOCOL* protocol = CreateProtocol();
          if(protocol)
            {
              protocolconnection->SetDIOProtocol(protocol);

              if(protocolconnection->Connect())
                {
                  if(xmutexprotocolconnections) xmutexprotocolconnections->Lock();

                  protocol->SetConnection(protocolconnection);

                  protocolconnections.Add(protocolconnection);

                  if(xmutexprotocolconnections) xmutexprotocolconnections->UnLock();
                }
            }
           else
            {
              DeleteProtocol(protocol);
              protocol = NULL;

              GEN_DELETE protocolconnection;
              protocolconnection = NULL;
            }
        }
       else
        {
          GEN_DELETE protocolconnection;
          protocolconnection = NULL;
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOPROTOCOL_CONNECTIONSMANAGER::ManageProtocolConnectionsClient()
* @brief      Manage protocol connections client
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOPROTOCOL_CONNECTIONSMANAGER::ManageProtocolConnectionsClient()
{
  if(!xmutexprotocolconnections)       return;
  if(!xthreadconnections)              return;
  if(!xthreadconnections->IsRunning()) return;
  if(!xtimerconnections)               return;
  if(!xtimerclienttry)                return;

  ProtocolConnections_DeleteDisconnected();

  if(protocolconnectionsnlimit != DIOPROTOCOL_CONNECTIONS_UNLIMITEDNCONNECTIONS)
    {
      if(!protocolconnectionsnlimit) return;

      if(ProtocolConnections_GetNConnected() >= protocolconnectionsnlimit)
        {
          return;
        }
    }

  if(xtimerclienttry->GetMeasureSeconds() <= DIOPROTOCOL_CONNECTIONS_DEFAULTIMETRYCONNECTIONS) return;

  //DEBUG_PRINT(__L("---------------------------------------------------------------"));

  for(int c=0;c<(int)targetURLs.GetSize();c++)
    {
      if(!xthreadconnections->IsRunning()) break;

      DIOURL* URLclient = (DIOURL*)targetURLs.Get(c);
      if(URLclient)
        {
          bool haveservervalid = false;

          switch(diostreamcfg->GetType())
            {
                                  default   :
              case DIOSTREAMTYPE_UNKNOWN    : break;
              case DIOSTREAMTYPE_UART       : break;
              case DIOSTREAMTYPE_USB        : break;
              case DIOSTREAMTYPE_BLUETOOTH  : break;

              case DIOSTREAMTYPE_TCPIP      : { DIOSTREAMTCPIPCONFIG* scfg = (DIOSTREAMTCPIPCONFIG*)diostreamcfg;
                                                XSTRING               address;
                                                if(scfg)
                                                  {
                                                    if(URLclient->IsEmpty() && diostreamenumservers)
                                                      {
                                                        diostreamenumservers->Search();

                                                        while(diostreamenumservers->IsSearching())
                                                          {
                                                            GEN_XSLEEP.MilliSeconds(1);
                                                          }

                                                        diostreamenumservers->StopSearch(true);

                                                        if(diostreamenumservers->AreDevicesAvailable())
                                                          {
                                                            DIOSTREAMDEVICEIP* device = (DIOSTREAMDEVICEIP*)diostreamenumservers->GetDevices()->Get(0);
                                                            if(device) device->GetIP()->GetXString(address);
                                                          }

                                                      } else address = URLclient->Get();

                                                    if(!address.IsEmpty())
                                                      {
                                                        DIOURL* url= GEN_NEW DIOURL();
                                                        if(url)
                                                          {
                                                            (*url) = address.Get();

                                                            url->ResolveURL();
                                                            scfg->GetRemoteURL()->Set(address);
                                                            GEN_DELETE url;

                                                            haveservervalid = true;
                                                          }
                                                      }
                                                  }

                                                break;
                                              }

            }

          if(haveservervalid)
            {
              DIOPROTOCOL_CONNECTION* protocolconnection = GEN_NEW DIOPROTOCOL_CONNECTION(diostreamcfg);
              if(protocolconnection)
                {
                  DIOPROTOCOL* protocol = CreateProtocol();
                  if(protocol)
                    {
                      protocolconnection->SetDIOProtocol(protocol);

                      if(protocolconnection->Connect())
                        {
                          xtimerconnections->Reset();

                          DIOSTREAMTCPIPCONFIG* scfg = (DIOSTREAMTCPIPCONFIG*)diostreamcfg;

                          while(1)
                            {
                              if(protocolconnection->GetDIOProtocol()->IsConnected())
                                {
                                  if(xmutexprotocolconnections) xmutexprotocolconnections->Lock();

                                  protocol->SetConnection(protocolconnection);
                                  protocolconnections.Add(protocolconnection);

                                  if(xmutexprotocolconnections) xmutexprotocolconnections->UnLock();

                                  SendEvent(DIOPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_CONNECTEDCONNECTION, protocolconnection);

                                  break;
                                }

                              if((xtimerconnections->GetMeasureSeconds()>=5) || (!xthreadconnections->IsRunning()))
                                {
                                  DeleteProtocol(protocol);
                                  protocol = NULL;

                                  GEN_DELETE protocolconnection;
                                  protocolconnection = NULL;

                                  break;
                                }

                              GEN_XSLEEP.MilliSeconds(50);
                            }

                         XTRACE_PRINTCOLOR(1, __L("Trying to connect to %s [%s]: %s (%d) seconds."), URLclient->Get(), scfg->GetRemoteURL()->Get(), protocolconnection?__L("Connected!."):__L("Not Connected!."), xtimerconnections->GetMeasureSeconds());
                        }
                       else
                        {
                          DeleteProtocol(protocol);
                          protocol = NULL;

                          GEN_DELETE protocolconnection;
                          protocolconnection = NULL;
                        }
                    }
                   else
                    {
                      GEN_DELETE protocolconnection;
                      protocolconnection = NULL;
                    }
                }
            }
        }
    }

  xtimerclienttry->Reset();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOPROTOCOL_CONNECTIONSMANAGER::ThreadProtocolConnections(void* param)
* @brief      Thread protocol connections
* @ingroup    DATAIO
* 
* @param[in]  param : Param pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOPROTOCOL_CONNECTIONSMANAGER::ThreadProtocolConnections(void* param)
{
  DIOPROTOCOL_CONNECTIONSMANAGER* protocolconnections = (DIOPROTOCOL_CONNECTIONSMANAGER*)param;
  if(!protocolconnections) return;

  if(protocolconnections->isserver)
    {
      protocolconnections->ManageProtocolConnectionsServer();
    }
   else
    {
      protocolconnections->ManageProtocolConnectionsClient();
    }

  int c=0;
  while(c < protocolconnections->ProtocolConnections_GetNAvailable())
    {
      DIOPROTOCOL_CONNECTION* protocolconnection = (DIOPROTOCOL_CONNECTION*)protocolconnections->protocolconnections.Get(c);
      if(protocolconnection)
        {
          DIOPROTOCOL* protocol = protocolconnection->GetDIOProtocol();
          if(protocol)
            {
              if(protocol->IsConnected())
                {
                  if(!protocol->IsInitialized())
                    {
                      if(protocolconnections->isserver)
                        {
                          protocolconnection->SetInUse(true, DIOPROTOCOL_CONNECTIONSINUSEID_INIPROTOCOL);

                          protocol->GetDIOStream()->GetXTimerNotActivity()->Reset();
                          protocolconnections->SendEvent(DIOPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_INITPROTOCOL, protocolconnection);
                          if(protocol->IsInitialized()) protocol->CMD_SendIsInitServerProtocol(true);

                          protocolconnection->SetInUse(false, DIOPROTOCOL_CONNECTIONSINUSEID_INIPROTOCOL);
                        }
                       else
                        {
                          XTIMER* timerout=GEN_XFACTORY.CreateTimer();
                          
                          while(1)
                            {
                              if(protocolconnection->GetDIOProtocol()->IsDisconnected()) break;
                              if(protocol->IsServerInitialized())                       break;

                              if(timerout)
                                {
                                  if(timerout->GetMeasureSeconds() > 5) break;
                                }

                              GEN_XSLEEP.MilliSeconds(10);
                            }

                          GEN_XFACTORY.DeleteTimer(timerout);


                          if(protocol->IsServerInitialized())
                            {
                              protocolconnection->SetInUse(true, DIOPROTOCOL_CONNECTIONSINUSEID_INIPROTOCOL);

                              protocol->GetDIOStream()->GetXTimerNotActivity()->Reset();
                              protocolconnections->SendEvent(DIOPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_INITPROTOCOL, protocolconnection);

                              protocolconnection->SetInUse(false, DIOPROTOCOL_CONNECTIONSINUSEID_INIPROTOCOL);
                            }
                        }

                     XTRACE_PRINTCOLOR((protocol->IsInitialized()?2:4), __L("Protocol Connection Manager: %s Init Protocol %s") , protocolconnections->isserver?__L("Server"):__L("Client"), protocol->IsInitialized()?__L("Ok!"):__L("ERROR!"));

                      if(!protocol->IsInitialized())
                        {
                          protocolconnections->ProtocolConnections_Disconnect(c);
                        }
                    }
                   else
                    {
                      if(protocol->GetDIOStream()->GetXTimerNotActivity()->GetMeasureSeconds() > DIOPROTOCOL_CONNECTIONS_DEFAULTIMECHECKCONNECTIONS)
                        {
                          XDWORD milliseconds;

                          bool status = protocol->CMD_Ping(milliseconds);
                          if(!status)
                            {
                              protocolconnections->ProtocolConnections_Disconnect(c);

                             XTRACE_PRINTCOLOR(status?1:4, __L("Protocol Connection Manager: Check connection protocol %s"), status?__L("Ok!"):__L("ERROR!"));
                            }
                        }
                    }
                }
            }
        }

      c++;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOPROTOCOL_CONNECTIONSMANAGER::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOPROTOCOL_CONNECTIONSMANAGER::Clean()
{
  diostreamenumservers            = NULL;
  diostreamcfg                    = NULL;

  applicationversion              = 0;
  applicationsubversion           = 0;
  applicationsubversionerr        = 0;

  isserver                        = false;
  protocolconnectionsnlimit       = 0;

  xthreadconnections              = NULL;
  xtimerconnections               = NULL;
  xtimerclienttry                 = NULL;
  xtimerout                       = NULL;

  xmutexprotocolconnections       = NULL;
}







