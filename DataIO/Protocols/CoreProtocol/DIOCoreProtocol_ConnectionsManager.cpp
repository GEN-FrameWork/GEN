/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOCoreProtocol_ConnectionsManager.cpp
* 
* @class      DIOCOREPROTOCOL_CONNECTIONSMANAGER
* @brief      Data Input/Output Core Protocol Connections Manager class
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

#include "DIOCoreProtocol_ConnectionsManager.h"

#include "XFactory.h"
#include "XSystem.h"
#include "XRand.h"
#include "XSerializable.h"

#include "DIOStream.h"
#include "DIOStreamConfig.h"
#include "DIOStreamEnumServers.h"

#include "DIOCoreProtocol.h"
#include "DIOCoreProtocol_CFG.h"
#include "DIOCoreProtocol_Connection.h"
#include "DIOCoreProtocol_ConnectionsManager_XEvent.h"
#include "DIOCoreProtocol_Messages.h"

#include "HashSHA2.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCOREPROTOCOL_CONNECTIONSMANAGER::DIOCOREPROTOCOL_CONNECTIONSMANAGER()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCOREPROTOCOL_CONNECTIONSMANAGER::DIOCOREPROTOCOL_CONNECTIONSMANAGER()
{
  Clean(); 

  diostreams.SetIsMulti(false);
  
  RegisterEvent(DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_CREATECONNECTION);
  RegisterEvent(DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_READMSG);
  RegisterEvent(DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_COMMANDRESPONSE);
  RegisterEvent(DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_UPDATECLASS); 
  RegisterEvent(DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_ASKUPDATECLASS); 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCOREPROTOCOL_CONNECTIONSMANAGER::~DIOCOREPROTOCOL_CONNECTIONSMANAGER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCOREPROTOCOL_CONNECTIONSMANAGER::~DIOCOREPROTOCOL_CONNECTIONSMANAGER()
{
  DeRegisterEvent(DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_CREATECONNECTION);
  DeRegisterEvent(DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_READMSG);
  DeRegisterEvent(DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_COMMANDRESPONSE);
  DeRegisterEvent(DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_UPDATECLASS);
  DeRegisterEvent(DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_ASKUPDATECLASS);

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTIONSMANAGER::Ini()
* @brief      Initialize the object
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTIONSMANAGER::Ini()
{ 
  bool status = false;

  if(!diostreams.GetSize())
    {
      return false;
    } 

  for(XDWORD c=0; c<diostreams.GetSize(); c++)
    {      
      DIOSTREAM* diostream = diostreams.GetElement(c);
      if(diostream)
        {      
          SubscribeEvent(DIOSTREAM_XEVENT_TYPE_CONNECTED    , diostream);
          SubscribeEvent(DIOSTREAM_XEVENT_TYPE_DISCONNECTED , diostream);
  
          if(diostream->Open())
            {
              if(diostream->GetConfig()->IsServer())
                {
                  if(!diostream->WaitToConnected(3))                
                    {
                      return false;
                    } 
                }            
            }
        }
    }

  updateclass_xmutex=GEN_XFACTORY.Create_Mutex();
  if(!updateclass_xmutex) 
    {
      return false;
    }

  connections_delete_xmutex=GEN_XFACTORY.Create_Mutex();
  if(connections_delete_xmutex) 
    {
      connections_xthread = CREATEXTHREAD(XTHREADGROUPID_DIOPROTOCOL_CONNECTIONMANAGER, __L("DIOPROTOCOL_CONNECTIONSMANAGER::Ini"), ThreadConnections, (void*)this);
      if(connections_xthread)
        {
          if(connections_xthread->Ini()) 
            {   
              connection_update_xthread = CREATEXTHREAD(XTHREADGROUPID_DIOPROTOCOL_CONNECTIONMANAGER, __L("DIOPROTOCOL_CONNECTIONSMANAGER::Ini"), ThreadUpdateConnection, (void*)this);
              if(connection_update_xthread)
                {
                  if(connection_update_xthread->Ini())
                    {
                      automaticoperations_xthread = CREATEXTHREAD(XTHREADGROUPID_DIOPROTOCOL_CONNECTIONMANAGER, __L("DIOPROTOCOL_CONNECTIONSMANAGER::Ini"), ThreadAutomaticOperations, (void*)this);
                      if(automaticoperations_xthread)
                        { 
                          automaticoperations_xthread->Ini();
                          return true;
                        }
                    }
                }
            }
        }      
    }
    

  End();

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTIONSMANAGER::End()
* @brief      End the object
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTIONSMANAGER::End()
{ 
  leavingactive = true;

  if(automaticoperations_xthread)
    {
      automaticoperations_xthread->End();
      DELETEXTHREAD(XTHREADGROUPID_DIOPROTOCOL_CONNECTIONMANAGER, automaticoperations_xthread);
      automaticoperations_xthread = NULL;
    }  

  if(connection_update_xthread)
    {
      connection_update_xthread->End();
      DELETEXTHREAD(XTHREADGROUPID_DIOPROTOCOL_CONNECTIONMANAGER, connection_update_xthread);
      connection_update_xthread = NULL;
    }  

  if(connections_xthread)
    {
      connections_xthread->End();
      DELETEXTHREAD(XTHREADGROUPID_DIOPROTOCOL_CONNECTIONMANAGER, connections_xthread);
      connections_xthread = NULL;
    }  
  
  Connections_DeleteAll();

  if(connections_delete_xmutex)
    {
      GEN_XFACTORY.Delete_Mutex(connections_delete_xmutex);
      connections_delete_xmutex = NULL;
    }

  if(updateclass_xmutex)
    {
      GEN_XFACTORY.Delete_Mutex(updateclass_xmutex);
      updateclass_xmutex = NULL;
    }

  for(XDWORD c=0; c<diostreams.GetSize(); c++)
    {      
      DIOSTREAM* diostream = diostreams.GetElement(c);
      if(diostream)
        {        
          UnSubscribeEvent(DIOSTREAM_XEVENT_TYPE_CONNECTED    , diostream);
          UnSubscribeEvent(DIOSTREAM_XEVENT_TYPE_DISCONNECTED , diostream);
  
          diostream->Close();          
        }
    }
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCOREPROTOCOL_CFG* DIOCOREPROTOCOL_CONNECTIONSMANAGER::GetProtocolCFG()
* @brief      Get protocol CFG
* @ingroup    DATAIO
* 
* @return     DIOCOREPROTOCOL_CFG* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCOREPROTOCOL_CFG* DIOCOREPROTOCOL_CONNECTIONSMANAGER::GetProtocolCFG()
{
  return &protocolCFG;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XMAP<DIOSTREAMCONFIG*, DIOSTREAM*>* DIOCOREPROTOCOL_CONNECTIONSMANAGER::DIOStream_GetAll()
* @brief      DIO stream get all
* @ingroup    DATAIO
* 
* @return     XMAP<DIOSTREAMCONFIG*, DIOSTREAM*>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XMAP<DIOSTREAMCONFIG*, DIOSTREAM*>* DIOCOREPROTOCOL_CONNECTIONSMANAGER::DIOStream_GetAll()
{
  return &diostreams;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTIONSMANAGER::DIOStream_Add(DIOSTREAMCONFIG* diostreamCFG, DIOSTREAM* diostream)
* @brief      DIO stream add
* @ingroup    DATAIO
* 
* @param[in]  diostreamCFG : Diostream configuration pointer to use.
* @param[in]  diostream : Diostream pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTIONSMANAGER::DIOStream_Add(DIOSTREAMCONFIG* diostreamCFG, DIOSTREAM* diostream)
{
  if(!diostreamCFG || !diostream)
    {
      return false;
    }

  return diostreams.Add(diostreamCFG, diostream);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTIONSMANAGER::DIOStream_Delete(DIOSTREAMCONFIG* diostreamCFG)
* @brief      DIO stream delete
* @ingroup    DATAIO
* 
* @param[in]  diostreamCFG : Diostream configuration pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTIONSMANAGER::DIOStream_Delete(DIOSTREAMCONFIG* diostreamCFG)
{
  if(!diostreamCFG)
    {
      return false;
    }

  DIOSTREAM* diostream = diostreams.Get(diostreamCFG);
  if(!diostream)
    {
      return false;
    }

  diostreams.Delete(diostreamCFG);

  GEN_DELETE diostreamCFG;
  GEN_DELETE diostream;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTIONSMANAGER::DIOStream_DeleteAll()
* @brief      DIO stream GEN_DELETE all
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTIONSMANAGER::DIOStream_DeleteAll()
{
  if(!diostreams.GetSize())
    {
      return false;
    }

  diostreams.DeleteKeyContents();
  diostreams.DeleteElementContents();
  
  diostreams.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCOREPROTOCOL_CONNECTION* DIOCOREPROTOCOL_CONNECTIONSMANAGER::CreateConnection()
* @brief      Create connection
* @ingroup    DATAIO
* 
* @return     DIOCOREPROTOCOL_CONNECTION* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCOREPROTOCOL_CONNECTION* DIOCOREPROTOCOL_CONNECTIONSMANAGER::CreateConnection()
{  
  DIOCOREPROTOCOL_CONNECTION* protocol = GEN_NEW DIOCOREPROTOCOL_CONNECTION();

  return protocol;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCOREPROTOCOL* DIOCOREPROTOCOL_CONNECTIONSMANAGER::CreateProtocol(DIOCOREPROTOCOL_CONNECTION* connection, DIOSTREAM* diostream)
* @brief      Create protocol
* @ingroup    DATAIO
* 
* @param[in]  connection : Connection pointer to use.
* @param[in]  diostream : Diostream pointer to use.
* 
* @return     DIOCOREPROTOCOL* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCOREPROTOCOL* DIOCOREPROTOCOL_CONNECTIONSMANAGER::CreateProtocol(DIOCOREPROTOCOL_CONNECTION* connection, DIOSTREAM* diostream)
{  
  DIOCOREPROTOCOL* protocol = GEN_NEW DIOCOREPROTOCOL(&protocolCFG, diostream);

  return protocol;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XMUTEX* DIOCOREPROTOCOL_CONNECTIONSMANAGER::UpdateClass_GetXMutex()
* @brief      update class  get Xmutex
* @ingroup    DATAIO
* 
* @return     XMUTEX* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XMUTEX* DIOCOREPROTOCOL_CONNECTIONSMANAGER::UpdateClass_GetXMutex()
{
  return updateclass_xmutex;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTIONSMANAGER::UpdateClass_Serialize(DIOCOREPROTOCOL_MESSAGE* message, XSERIALIZABLE* classcontent)
* @brief      Update class serialize
* @ingroup    DATAIO
* 
* @param[in]  message : Message text.
* @param[in]  classcontent : Classcontent pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTIONSMANAGER::UpdateClass_Serialize(DIOCOREPROTOCOL_MESSAGE* message, XSERIALIZABLE* classcontent)
{
  XFILEJSON              content;          
  XSERIALIZATIONMETHOD*  serializationmethod = XSERIALIZABLE::CreateInstance(content);  
  bool                   status              = false;

  if(!classcontent)
    {
      return false;
    }

  if(!serializationmethod)
    {
      return false;  
    } 

  classcontent->SetSerializationMethod(serializationmethod);    
  
  status = classcontent->Serialize();
  if(status)
    { 
      XSTRING contentstring;
      XBUFFER contentbinary;   
     
      content.EncodeAllLines();
      content.GetAllInOneLine(contentstring);
      contentstring.ConvertToUTF8(contentbinary);
      
      message->GetContent()->Empty();
      message->GetContent()->Add(contentbinary);
    } 
                
  GEN_DELETE serializationmethod;
                      
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTIONSMANAGER::UpdateClass_Deserialize(DIOCOREPROTOCOL_MESSAGE* message, XSERIALIZABLE* classcontent)
* @brief      Update class deserialize
* @ingroup    DATAIO
* 
* @param[in]  message : Message text.
* @param[in]  classcontent : Classcontent pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTIONSMANAGER::UpdateClass_Deserialize(DIOCOREPROTOCOL_MESSAGE* message, XSERIALIZABLE* classcontent)
{
  XFILEJSON              content;          
  XSERIALIZATIONMETHOD*  serializationmethod = XSERIALIZABLE::CreateInstance(content);  
  bool                   status              = false;

  if(!classcontent)
    {
      return false;
    }

  if(!serializationmethod)
    {
      return false;  
    }
   
  content.AddBufferLines(XFILETXTFORMATCHAR_UTF8, (*message->GetContent()));
  content.DecodeAllLines();
                  
  classcontent->SetSerializationMethod(serializationmethod);      

  status = classcontent->Deserialize();

  GEN_DELETE serializationmethod;
                      
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<DIOCOREPROTOCOL_CONNECTION*>* DIOCOREPROTOCOL_CONNECTIONSMANAGER::Connections_GetAll()
* @brief      Connections get all
* @ingroup    DATAIO
* 
* @return     XVECTOR<DIOCOREPROTOCOL_CONNECTION*>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<DIOCOREPROTOCOL_CONNECTION*>* DIOCOREPROTOCOL_CONNECTIONSMANAGER::Connections_GetAll()
{
  return &connections;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XMUTEX* DIOCOREPROTOCOL_CONNECTIONSMANAGER::Connections_GetXMutex()
* @brief      Connections get X mutex
* @ingroup    DATAIO
* 
* @return     XMUTEX* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XMUTEX* DIOCOREPROTOCOL_CONNECTIONSMANAGER::Connections_GetXMutex()
{
  return connections_delete_xmutex;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCOREPROTOCOL_CONNECTION* DIOCOREPROTOCOL_CONNECTIONSMANAGER::Connections_Add(DIOSTREAM* diostream)
* @brief      Connections add
* @ingroup    DATAIO
* 
* @param[in]  diostream : Diostream pointer to use.
* 
* @return     DIOCOREPROTOCOL_CONNECTION* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCOREPROTOCOL_CONNECTION* DIOCOREPROTOCOL_CONNECTIONSMANAGER::Connections_Add(DIOSTREAM* diostream)
{
  if(!diostream)
    {
      return NULL;
    }

  DIOCOREPROTOCOL_CONNECTION* connection = Connections_Get(diostream);
  if(!connection)
    {
      connection = CreateConnection();
      if(!connection)
        {
          return NULL;
        }      
    }

  if(connections_delete_xmutex)
    {
      connections_delete_xmutex->Lock();
    }

  bool status = connections.Add(connection);

  if(connections_delete_xmutex)
    {
      connections_delete_xmutex->UnLock();
    }

  return connection;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCOREPROTOCOL_CONNECTION* DIOCOREPROTOCOL_CONNECTIONSMANAGER::Connections_Get(DIOSTREAM* diostream)
* @brief      Connections get
* @ingroup    DATAIO
* 
* @param[in]  diostream : Diostream pointer to use.
* 
* @return     DIOCOREPROTOCOL_CONNECTION* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCOREPROTOCOL_CONNECTION* DIOCOREPROTOCOL_CONNECTIONSMANAGER::Connections_Get(DIOSTREAM* diostream)
{ 
  if(!connections.GetSize())
    {
      return NULL;
    }

  if(connections_delete_xmutex)
    {
      connections_delete_xmutex->Lock();
    }

  for(XDWORD c=0; c<connections.GetSize(); c++)
    {
      DIOCOREPROTOCOL_CONNECTION* connection = connections.Get(c);
      if(connection)  
        {
          if(connection->GetCoreProtocol()->GetDIOStream() == diostream)
            {
              if(connections_delete_xmutex)
                  {
                    connections_delete_xmutex->UnLock();
                  }

              return connection;
            }
        }
    }

  if(connections_delete_xmutex)
    {
      connections_delete_xmutex->UnLock();
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCOREPROTOCOL_CONNECTION* DIOCOREPROTOCOL_CONNECTIONSMANAGER::Connections_Get(XDWORD index)
* @brief      Connections get
* @ingroup    DATAIO
* 
* @param[in]  index : Index value.
* 
* @return     DIOCOREPROTOCOL_CONNECTION* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCOREPROTOCOL_CONNECTION* DIOCOREPROTOCOL_CONNECTIONSMANAGER::Connections_Get(XDWORD index)
{ 
  DIOCOREPROTOCOL_CONNECTION* connection = NULL;

  if(!connections.GetSize())
    {
      return NULL;
    }

  if(connections_delete_xmutex)
    {
      connections_delete_xmutex->Lock();
    }

  connection = connections.Get(index);
  
  if(connections_delete_xmutex)
    {
      connections_delete_xmutex->UnLock();
    }

  return connection;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTIONSMANAGER::Connections_Delete(DIOCOREPROTOCOL_CONNECTION* connection)
* @brief      Connections delete
* @ingroup    DATAIO
* 
* @param[in]  connection : Connection pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTIONSMANAGER::Connections_Delete(DIOCOREPROTOCOL_CONNECTION* connection)
{
  if(!connection)
    {
      return false;
    }

  if(connections_delete_xmutex)
    {
      connections_delete_xmutex->Lock();
    }

  connections.Delete(connection);
  GEN_DELETE connection;

  if(connections_delete_xmutex)
    {
      connections_delete_xmutex->UnLock();
    }
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTIONSMANAGER::Connections_DeleteAllDisconnected()
* @brief      Connections GEN_DELETE all disconnected
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTIONSMANAGER::Connections_DeleteAllDisconnected()
{
  if(!connections.GetSize())
    {
      return false;
    }

  int index = 0;

  if(connections_delete_xmutex)
    {
      connections_delete_xmutex->Lock();
    }

  do{ DIOCOREPROTOCOL_CONNECTION* connection = connections.Get(index);
      if(connection)  
        {
          if(connection->Status_Get() == DIOCOREPROTOCOL_CONNECTION_STATUS_DISCONNECTED)
            {            
              if(connection->GetXTimerStatus()->GetMeasureSeconds() >= connection->GetCoreProtocol()->GetProtocolCFG()->GetTimeToEliminateConnectionDisconnect())
                {                                                       
                  connections.Delete(connection);
                  GEN_DELETE connection;

                  continue;
                }
            }

          index++;
        }

    } while(index < connections.GetSize());


  if(connections_delete_xmutex)
    {
      connections_delete_xmutex->UnLock();
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTIONSMANAGER::Connections_DeleteAll()
* @brief      Connections GEN_DELETE all
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTIONSMANAGER::Connections_DeleteAll()
{
  if(!connections.GetSize())
    {
      return false;
    }

  if(connections_delete_xmutex)
    {
      connections_delete_xmutex->Lock();
    }
  
 
  int index = 0;

  do{ DIOCOREPROTOCOL_CONNECTION* connection = connections.Get(index);
      if(connection)  
        {
          connection->RemoteDisconnect();

          index++;
        }

    } while(index < connections.GetSize());

  connections.DeleteContents();
  connections.DeleteAll();

  if(connections_delete_xmutex)
    {
      connections_delete_xmutex->UnLock();
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTIONSMANAGER::CreateIDMachine(XUUID& ID)
* @brief      Create ID machine
* @ingroup    DATAIO
* 
* @param[in]  ID : Identifier to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTIONSMANAGER::CreateIDMachine(XUUID& ID)
{ 
  XSTRING   origin; 
  XBUFFER   originbuffer; 
  HASHSHA2* sha2        = NULL;
  XBUFFER*  result      = NULL;

  ID.Empty();

  GEN_XSYSTEM.GetOperativeSystemID(origin);
  
  origin.AddFormat(__L(" %s"), GEN_XSYSTEM.GetBIOSSerialNumber()->Get());
  origin.AddFormat(__L(" %s"), GEN_XSYSTEM.GetCPUSerialNumber()->Get());

  origin.ConvertToUTF8(originbuffer);
    
  sha2 = GEN_NEW HASHSHA2(HASHSHA2TYPE_256);
  if(!sha2)
    {
      return false;
    }

  sha2->Do(originbuffer);
  result = sha2->GetResult();

  if(!result)
    {
      return false;
    }

  if(!result->GetSize())
    {
      return false;
    }

  XDWORD data1 = 0; 
  XWORD  data2 = 0;
  XWORD  data3 = 0;
  XBYTE  data4 = 0;
  XBYTE  data5 = 0; 
  XBYTE* data6 = &result->Get()[12];

  result->Get((XDWORD&)data1);  
  result->Get((XWORD&)data2);  
  result->Get((XWORD&)data3);  
  result->Get((XBYTE&)data4);  
  result->Get((XBYTE&)data5);  

  ID.Set(data1, data2, data3, data4, data5, data6);

  ID.GetToString(origin);

  GEN_DELETE sha2;

  return true;
}
         

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTIONSMANAGER::Connections_ReadMessages()
* @brief      Connections read messages
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTIONSMANAGER::Connections_ReadMessages()
{
  if(!connections.GetSize())
    {
      return false;
    }

  int index = 0;

  do{ DIOCOREPROTOCOL_CONNECTION* connection = connections.Get(index);
      if(connection)  
        {  
          if((connection->Status_Get() == DIOCOREPROTOCOL_CONNECTION_STATUS_READY)              ||             
             (connection->Status_Get() == DIOCOREPROTOCOL_CONNECTION_STATUS_INSTABILITY))
            {              
              if(connection->GetCoreProtocol())
                {          
                  DIOCOREPROTOCOL_MESSAGES* messages = connection->Messages_GetAll();
                  if(messages)
                    {
                      int indexmsg = 0;
   
                      while(indexmsg < messages->GetAll()->GetSize())
                        {
                          DIOCOREPROTOCOL_MESSAGE* message[2] = { NULL, NULL };

                          message[0] = messages->GetAll()->GetKey(indexmsg);
                          message[1] = messages->GetAll()->GetElement(indexmsg);
                          
                          if(message[0])
                            {
                              if(!message[0]->IsConsumed())
                                {
                                  if(message[0]->GetHeader()->GetMessageType() == DIOCOREPROTOCOL_HEADER_MESSAGETYPE_REQUEST) 
                                    {                                                                             
                                      switch(message[0]->GetHeader()->GetOperation())
                                        {
                                          case DIOCOREPROTOCOL_HEADER_OPERATION_UNKNOWN         : break;

                                          case DIOCOREPROTOCOL_HEADER_OPERATION_COMMAND         : if(Received_AllCommandMessages(connection, message[0]))
                                                                                                    {                                                                                                       
                                                                                                      message[1] = messages->GetAll()->GetElement(indexmsg);                                                                                     
                                                                                                      if(message[1]) 
                                                                                                        {
                                                                                                          message[1]->SetIsConsumed(true);                                                                                                  
                                                                                                        }
                                                                                                    }

                                                                                                  // Test show message status
                                                                                                  // messages->ShowDebug(connection->IsServer());                                                                                              
                                                                                                  break;

                                          
                                          case DIOCOREPROTOCOL_HEADER_OPERATION_UPDATECLASS     : if(Received_AllUpdateClassMessages(connection, message[0]))
                                                                                                    {                                                                                                       
                                                                                                      message[1] = messages->GetAll()->GetElement(indexmsg);                                                                                     
                                                                                                      if(message[1]) 
                                                                                                        {
                                                                                                          message[1]->SetIsConsumed(true);                                                                                                  
                                                                                                        }
                                                                                                    }

                                                                                                  // Test show message status
                                                                                                  // messages->ShowDebug(connection->IsServer());                                                                                              
                                                                                                  break;

                                          case DIOCOREPROTOCOL_HEADER_OPERATION_ASKUPDATECLASS  : if(Received_AllAskUpdateClassMessages(connection, message[0]))
                                                                                                    {                                                                                                       
                                                                                                      message[1] = messages->GetAll()->GetElement(indexmsg);                                                                                     
                                                                                                      if(message[1]) 
                                                                                                        {
                                                                                                          message[1]->SetIsConsumed(true);                                                                                                  
                                                                                                        }
                                                                                                    }

                                                                                                  // Test show message status
                                                                                                  // messages->ShowDebug(connection->IsServer());                                                                                              
                                                                                                  break;

                                                                                   default      : break; 
                                        }                                       
                                    }
                                }
                            }
                             
                          indexmsg++;
                        }                                      
                    }
                }
            }
            
          index++;
        }

    } while(index < connections.GetSize());

   
  // -----------------------------------------------------------------------------------------------
  // Delete consumed messages
                      
  index = 0;

  do{ DIOCOREPROTOCOL_CONNECTION* connection = connections.Get(index);
      if(connection)  
        {  
          if((connection->Status_Get() == DIOCOREPROTOCOL_CONNECTION_STATUS_READY)              ||             
             (connection->Status_Get() == DIOCOREPROTOCOL_CONNECTION_STATUS_INSTABILITY))
            {        
              DIOCOREPROTOCOL_MESSAGES* messages = connection->Messages_GetAll();
              if(messages)
                {
                  int   indexmsg  = 0;
                  bool  deletemsg = false;

                  while(indexmsg < messages->GetAll()->GetSize())
                    {
                      DIOCOREPROTOCOL_MESSAGE* message[2];

                      message[0] = messages->GetAll()->GetKey(indexmsg);
                      message[1] = messages->GetAll()->GetElement(indexmsg);

                      deletemsg = false;
                          
                      if(message[0] && message[1])
                        {
                          if(message[0]->IsConsumed() && message[1]->IsConsumed())
                            {                              
                              if(messages->Delete(message[0]->GetHeader()->GetIDMessage()))
                                {
                                  deletemsg = true;            
                                }                              
                            }
                        }

                      if(!deletemsg)
                        {
                          indexmsg++;
                        }
                    }
                }
            }

          index++;
        }

    } while(index < connections.GetSize());

  // -----------------------------------------------------------------------------------------------
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTIONSMANAGER::Received_AllCommandMessages(DIOCOREPROTOCOL_CONNECTION* connection, DIOCOREPROTOCOL_MESSAGE* message)
* @brief      Received all command messages
* @ingroup    DATAIO
* 
* @param[in]  connection : Connection pointer to use.
* @param[in]  message : Message text.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTIONSMANAGER::Received_AllCommandMessages(DIOCOREPROTOCOL_CONNECTION* connection, DIOCOREPROTOCOL_MESSAGE* message)
{
  if(!connection)
    {
      return false;
    }  

  if(!message)
    {
      return false;
    }

  if(message->GetHeader()->GetOperation() != DIOCOREPROTOCOL_HEADER_OPERATION_COMMAND)
    {
      return false;
    }

  DIOCOREPROTOCOL* protocol = connection->GetCoreProtocol();
  if(!protocol)
    {
      return false;
    }

  connection->Mutex_Activate(connection->GetInsideCommandMutex(), true);
  
  bool managermessage = false;
  bool status         = false;
    
  if(!managermessage)
    {   
      if(!message->GetHeader()->GetOperationParam()->Compare(protocol->Commands_Get(DIOCOREPROTOCOL_COMMAND_TYPE_HEARTBEAT), true))
        {         
          message->SetIsConsumed(true); 

          connection->SetHeartBetsCounter(0);    
          connection->SetEvent(DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_READY);
      
          status = connection->Command_Do(message->GetHeader()->GetIDMessage(), DIOCOREPROTOCOL_COMMAND_TYPE_HEARTBEAT);
          if(status)
            {
              managermessage = true;      
            }
        }
    }
  
  if(!managermessage)
    {     
      if(!message->GetHeader()->GetOperationParam()->Compare(protocol->Commands_Get(DIOCOREPROTOCOL_COMMAND_TYPE_DISCONNECT), true))
        {         
          message->SetIsConsumed(true);           
          status = connection->SetEvent(DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_DISCONNECTED);
          if(status)
            {
              managermessage = true;      
            }                      
        }
    }
  
  if(!managermessage)
    {
      status = Received_AdditionalCommandMessages(connection, message);
    }
   else
    {
      status = true;
    }

  connection->Mutex_Activate(connection->GetInsideCommandMutex(), false);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTIONSMANAGER::Received_AdditionalCommandMessages(DIOCOREPROTOCOL_CONNECTION* connection, DIOCOREPROTOCOL_MESSAGE* message)
* @brief      Received additional command messages
* @ingroup    DATAIO
* 
* @param[in]  connection : Connection pointer to use.
* @param[in]  message : Message text.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTIONSMANAGER::Received_AdditionalCommandMessages(DIOCOREPROTOCOL_CONNECTION* connection, DIOCOREPROTOCOL_MESSAGE* message)
{
  bool status = false;

  if(!connection)
    {
      return false;
    }

  if(!message)
    {
      return false;
    }

  DIOCOREPROTOCOL* protocol = connection->GetCoreProtocol();
  if(!protocol)
    {
      return false;
    }

  for(XDWORD c=DIOCOREPROTOCOL_COMMAND_TYPE_LASTINTERNAL;  c<protocol->Commands_GetAll()->GetSize()+1; c++)
    {
      if(!message->GetHeader()->GetOperationParam()->Compare(protocol->Commands_Get(c), true))
        { 
          DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT xevent(this, DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_COMMANDRESPONSE);
          xevent.SetConnection(connection);
          xevent.SetMsg(message);
                      
          status = PostEvent(&xevent);  

          message->SetIsConsumed(true);     

          connection->SetHeartBetsCounter(0);    
          connection->SetEvent(DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_READY);   

          if(status)
            {     
              if(!xevent.GetContentResponseBuffer()->IsEmpty())
                {
                  status = connection->Command_Do(message->GetHeader()->GetIDMessage(), c, xevent.GetContentResponseBuffer());       
                }
               else
                {
                  if(!xevent.GetContentResponseString()->IsEmpty())
                    {
                      status = connection->Command_Do(message->GetHeader()->GetIDMessage(), c, xevent.GetContentResponseString()); 
                    }
                   else
                    {
                      status = connection->Command_Do(message->GetHeader()->GetIDMessage(), c, xevent.GetContentResponseXFileJSON());                     
                    } 
                }   
            }
        }
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTIONSMANAGER::Received_AllUpdateClassMessages(DIOCOREPROTOCOL_CONNECTION* connection, DIOCOREPROTOCOL_MESSAGE* message)
* @brief      Received all update class messages
* @ingroup    DATAIO
* 
* @param[in]  connection : Connection pointer to use.
* @param[in]  message : Message text.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTIONSMANAGER::Received_AllUpdateClassMessages(DIOCOREPROTOCOL_CONNECTION* connection, DIOCOREPROTOCOL_MESSAGE* message)
{
 if(!connection)
    {
      return false;
    }  

  if(!message)
    {
      return false;
    }

  if(message->GetHeader()->GetOperation() != DIOCOREPROTOCOL_HEADER_OPERATION_UPDATECLASS)
    {
      return false;
    }

  DIOCOREPROTOCOL* protocol = connection->GetCoreProtocol();
  if(!protocol)
    {
      return false;
    }
  
  bool managermessage = false;
  bool status         = false;


  for(XDWORD c=0; c<protocol->UpdateClass_GetAll()->GetSize(); c++)
    {
      DIOCOREPROTOCOL_UPDATECLASS* updateclass = protocol->UpdateClass_GetAll()->Get(c);
      if(updateclass)
        {    
          if(!message->GetHeader()->GetOperationParam()->Compare(updateclass->GetClassName()->Get(), true))
            { 
              XFILEJSON              content;          
              XSERIALIZATIONMETHOD*  serializationmethod = XSERIALIZABLE::CreateInstance(content);                          

              message->SetIsConsumed(true);

              connection->SetHeartBetsCounter(0);    
              connection->SetEvent(DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_READY); 
            
              if(serializationmethod)
                {
                  content.AddBufferLines(XFILETXTFORMATCHAR_UTF8, (*message->GetContent()));
                  content.DecodeAllLines();
                  
                  updateclass->GetClassPtr()->SetSerializationMethod(serializationmethod);                            
                  updateclass->GetClassPtr()->Deserialize();
                        
                  GEN_DELETE serializationmethod;

                  GenerateResponseUpdateClass(content, true);
                     
                  status = connection->UpdateClass_Do(message->GetHeader()->GetIDMessage(), message->GetHeader()->GetOperationParam()->Get(), &content);
                  if(status)
                    {                                    
                      managermessage = true;      
                    }         
                }                             
            }
        }
    }

  if(!managermessage)
    {
      status = Received_AdditionalUpdateClassMessages(connection, message);
    }
   else
    {
      status = true;
    }
 
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTIONSMANAGER::Received_AdditionalUpdateClassMessages(DIOCOREPROTOCOL_CONNECTION* connection, DIOCOREPROTOCOL_MESSAGE* message)
* @brief      Received additional update class messages
* @ingroup    DATAIO
* 
* @param[in]  connection : Connection pointer to use.
* @param[in]  message : Message text.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTIONSMANAGER::Received_AdditionalUpdateClassMessages(DIOCOREPROTOCOL_CONNECTION* connection, DIOCOREPROTOCOL_MESSAGE* message)
{ 
  bool status = false;

  if(!connection)
    {
      return false;
    }  

  if(!message)
    {
      return false;
    }
 
  if(message->GetHeader()->GetOperation() == DIOCOREPROTOCOL_HEADER_OPERATION_UPDATECLASS)
    {
      DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT xevent(this, DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_UPDATECLASS);
      xevent.SetConnection(connection);
      xevent.SetMsg(message);
                      
      status = PostEvent(&xevent);       

      message->SetIsConsumed(true);

      connection->SetHeartBetsCounter(0);    
      connection->SetEvent(DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_READY);  

      if(status)
        {     
          XFILEJSON xfileJSON;

          GenerateResponseUpdateClass(xfileJSON, true);
                     
          status = connection->UpdateClass_Do(message->GetHeader()->GetIDMessage(), message->GetHeader()->GetOperationParam()->Get(), &xfileJSON);          
        }           
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTIONSMANAGER::Received_AllAskUpdateClassMessages(DIOCOREPROTOCOL_CONNECTION* connection, DIOCOREPROTOCOL_MESSAGE* message)
* @brief      Received all ask update class messages
* @ingroup    DATAIO
* 
* @param[in]  connection : Connection pointer to use.
* @param[in]  message : Message text.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTIONSMANAGER::Received_AllAskUpdateClassMessages(DIOCOREPROTOCOL_CONNECTION* connection, DIOCOREPROTOCOL_MESSAGE* message)
{
  if(!connection)
    {
      return false;
    }  

  if(!message)
    {
      return false;
    }

  if(message->GetHeader()->GetOperation() != DIOCOREPROTOCOL_HEADER_OPERATION_ASKUPDATECLASS)
    {
      return false;
    }

  DIOCOREPROTOCOL* protocol = connection->GetCoreProtocol();
  if(!protocol)
    {
      return false;
    }
  
  bool managermessage = false;
  bool status         = false;

  for(XDWORD c=0; c<protocol->UpdateClass_GetAll()->GetSize(); c++)
    {
      DIOCOREPROTOCOL_UPDATECLASS* updateclass = protocol->UpdateClass_GetAll()->Get(c);
      if(updateclass)
        {      
          if(!message->GetHeader()->GetOperationParam()->Compare(updateclass->GetClassName()->Get(), true))
            {
              message->SetIsConsumed(true);

              connection->SetHeartBetsCounter(0);    
              connection->SetEvent(DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_READY); 

              XFILEJSON              content;          
              XSERIALIZATIONMETHOD*  serializationmethod = XSERIALIZABLE::CreateInstance(content);             
              
              updateclass->GetClassPtr()->SetSerializationMethod(serializationmethod);                            
              updateclass->GetClassPtr()->Serialize();

              if(serializationmethod)
                {
                  GEN_DELETE serializationmethod;
                }    
   
              status = connection->UpdateClass_Do(message->GetHeader()->GetIDMessage(), message->GetHeader()->GetOperationParam()->Get(), &content);
              if(status)
                {                                    
                  managermessage = true;      
                }         
            }    
        }
    }

  if(!managermessage)
    {
      status = Received_AdditionalAskUpdateClassMessages(connection, message);
    }
   else
    {
      status = true;
    }
 
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTIONSMANAGER::Received_AdditionalAskUpdateClassMessages(DIOCOREPROTOCOL_CONNECTION* connection, DIOCOREPROTOCOL_MESSAGE* message)
* @brief      Received additional ask update class messages
* @ingroup    DATAIO
* 
* @param[in]  connection : Connection pointer to use.
* @param[in]  message : Message text.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTIONSMANAGER::Received_AdditionalAskUpdateClassMessages(DIOCOREPROTOCOL_CONNECTION* connection, DIOCOREPROTOCOL_MESSAGE* message)
{ 
  bool status = false;

  if(!connection)
    {
      return false;
    }  

  if(!message)
    {
      return false;
    }
 
  if(message->GetHeader()->GetOperation() == DIOCOREPROTOCOL_HEADER_OPERATION_ASKUPDATECLASS)
    {
      DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT xevent(this, DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_ASKUPDATECLASS);
      xevent.SetConnection(connection);
      xevent.SetMsg(message);
                      
      status = PostEvent(&xevent);       

      message->SetIsConsumed(true);

      connection->SetHeartBetsCounter(0);    
      connection->SetEvent(DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_READY);  

      if(status)
        {     
          XFILEJSON content;

          content.AddBufferLines(XFILETXTFORMATCHAR_UTF8, (*message->GetContent()));
          content.DecodeAllLines();
                     
          status = connection->UpdateClass_Do(message->GetHeader()->GetIDMessage(), message->GetHeader()->GetOperationParam()->Get(), &content);          
        }           
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTIONSMANAGER::GenerateResponseUpdateClass(XFILEJSON& xfileJSON, bool statusresponse)
* @brief      Generate response update class
* @ingroup    DATAIO
* 
* @param[in]  xfileJSON : Xfile JSON value.
* @param[in]  statusresponse : Statusresponse value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTIONSMANAGER::GenerateResponseUpdateClass(XFILEJSON& xfileJSON, bool statusresponse)
{
  XFILEJSONOBJECT*  root;

  xfileJSON.DeleteAllLines();
  xfileJSON.DeleteAllObjects();
         
  root = xfileJSON.GetRoot();
  if(!root)
    {
      root = GEN_NEW XFILEJSONOBJECT();
      if(!root) 
        {
          return false;
        }

      xfileJSON.SetRoot(root);
    }

  XFILEJSON_ADDVALUE(root, DIOCOREPROTOCOL_UPDATECLASS_CONFIRM_PARAM, (bool*)statusresponse);   

  return true;         
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTIONSMANAGER::ManagerUpdateClasses()
* @brief      manager update class
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTIONSMANAGER::ManagerUpdateClasses()
{
  if(!GetProtocolCFG())
    {
      return false;
    }  

  if(GetProtocolCFG()->GetIsServer())
    {
      return false;
    }

  if(updateclass_xmutex)
    {
      updateclass_xmutex->Lock();
    }
 
  for(int c=0; c<connections.GetSize(); c++)
    {  
      DIOCOREPROTOCOL_CONNECTION* connection = connections.Get(c);
      if(connection)  
        {                                                    
          if((connection->Status_Get() != DIOCOREPROTOCOL_CONNECTION_STATUS_READY) &&
             (connection->Status_Get() != DIOCOREPROTOCOL_CONNECTION_STATUS_INSTABILITY))
            {       
              continue;
            }
            
          DIOCOREPROTOCOL* protocol = connection->GetCoreProtocol();
          if(!protocol)
            {    
              continue;
            }

          if(connection->Mutex_IsActive(connection->GetInsideCommandMutex()))
            {
              continue;
            }
                                    
          for(XDWORD d=0; d<protocol->UpdateClass_GetAll()->GetSize(); d++)
            {
              bool needtoupdate = false;

              DIOCOREPROTOCOL_UPDATECLASS* updateclass = protocol->UpdateClass_GetAll()->Get(d);
              if(!updateclass)     
                {                  
                  continue;        
                }
          
              if(!connection->CompletedInitialUpdateClasses() && updateclass->RequieredInitialUpdate())
                {
                  updateclass->GetClassPtr()->HasBeenChanged();
                  needtoupdate = true;               
                }              

              if(!needtoupdate)
                {
                  if(updateclass->GetTimeToUpdate())
                    {
                      if(updateclass->GetTimerLastUpdate()->GetMeasureSeconds() >= updateclass->GetTimeToUpdate())
                        {
                          needtoupdate = true;                                     
                        }   
                    }
                }

              if(!needtoupdate)
                {                                                                          
                  if(updateclass->IsFlag(DIOCOREPROTOCOL_UPDATECLASS_FLAG_FORCHANGE))
                    {  
                      if(updateclass->GetClassPtr())
                        {
                          if(!updateclass->GetClassPtr()->HasBeenChanged())
                            {
                              needtoupdate = false;    
                            }
                           else
                            {                      
                              needtoupdate = true;  
                            }           
                        }
                    } 
                }
                                         
              if(needtoupdate)
                {                                                             
                  bool status;
                       
                  status = connection->UpdateClass_Do(updateclass->GetClassName()->Get(), updateclass->GetClassPtr(), GetProtocolCFG()->GetTimeOutNoResponse());                                   
                                                 
                  if(status)
                    {                      
                      updateclass->GetTimerLastUpdate()->Reset();
                      updateclass->GetClassPtr()->SetHasBeenChanged(false);
                      updateclass->AddOneToNUpdates();                        
                    }                                                                                          
                }                                                                        
            }

          connection->SetCompletedInitialUpdateClasses(true);     
        }
    }

  if(updateclass_xmutex)
    {
      updateclass_xmutex->UnLock();
    }
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTIONSMANAGER::ManagerHeartBet()
* @brief      manager heart bet
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTIONSMANAGER::ManagerHeartBet()
{
  if(!GetProtocolCFG())
    {
      return false;
    }

  int index = 0;
  
  do{ DIOCOREPROTOCOL_CONNECTION* connection = connections.Get(index);
      if(connection)  
        {                           
          if((connection->Status_Get() == DIOCOREPROTOCOL_CONNECTION_STATUS_READY) || 
             (connection->Status_Get() == DIOCOREPROTOCOL_CONNECTION_STATUS_INSTABILITY))
            {       

             if(connection->Mutex_IsActive(connection->GetInsideCommandMutex()))
               {
                 continue;
               }

              DIOCOREPROTOCOL* protocol = connection->GetCoreProtocol();
              if(protocol)
                {                          
                  if(protocol->GetProtocolCFG()->GetNTrysToCheckConnection())
                    {
                      if(connection->GetXTimerWithoutConnexion()->GetMeasureSeconds() >= protocol->GetProtocolCFG()->GetTimeToCheckConnection())
                        {
                          XBUFFER result;
                          bool    status;
                                                   
                          status = connection->Command_Do(DIOCOREPROTOCOL_COMMAND_TYPE_HEARTBEAT, result, GetProtocolCFG()->GetTimeOutNoResponse()); 

                          if(connection->GetHeartBetsCounter() >= protocol->GetProtocolCFG()->GetNTrysToCheckConnection())
                            {
                              connection->SetEvent(DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_DISCONNECTED);   
                            }

                          if(connection->GetHeartBetsCounter() == 1)
                            {
                              connection->SetEvent(DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_INSTABILITY);   
                            }
                      
                          if(GetProtocolCFG()->GetIsServer())
                            { 
                              if(!status)                            
                                {
                                  connection->SetHeartBetsCounter(connection->GetHeartBetsCounter()+1);                                  
                                }
                            }

                          connection->GetXTimerWithoutConnexion()->Reset();                                
                        }                                     
                    }
                }              
            }
                      
          index++;
        }

    } while(index < connections.GetSize());  

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCOREPROTOCOL_CONNECTIONSMANAGER::HandleEvent_CoreProtocolConnectionsManager(DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT* event)
* @brief      Handle event core protocol connections manager
* @note       INTERNAL
* @ingroup    DATAIO
* 
* @param[in]  event : Event to process.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCOREPROTOCOL_CONNECTIONSMANAGER::HandleEvent_CoreProtocolConnectionsManager(DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT* event)
{
  if(!event) 
    {
      return;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCOREPROTOCOL_CONNECTIONSMANAGER::HandleEvent_DIOStream(DIOSTREAM_XEVENT* event)
* @brief      Handle event DIO stream
* @note       INTERNAL
* @ingroup    DATAIO
* 
* @param[in]  event : Event to process.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCOREPROTOCOL_CONNECTIONSMANAGER::HandleEvent_DIOStream(DIOSTREAM_XEVENT* event)
{
  if(!event) 
    {
      return;
    }

  switch(event->GetEventType())
    {
      case DIOSTREAM_XEVENT_TYPE_CONNECTED      : { DIOCOREPROTOCOL_CONNECTION* connection = Connections_Add(event->GetDIOStream());                                                                                                      
                                                    if(connection)          
                                                      {   
                                                        SubscribeEvent(DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_CREATECONNECTION, this);
                                                        SubscribeEvent(DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_STATUSCHANGE    , connection);
                                                        SubscribeEvent(DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_READMSG         , this);
                                                        SubscribeEvent(DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_WRITEMSG        , connection);

                                                        DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT xevent(this, DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_CREATECONNECTION);
                                                        xevent.SetConnection(connection);                                                                                          
                                                                                           
                                                        PostEvent(&xevent);   

                                                        if(!connection->GetRegisterData())
                                                          {
                                                            DIOCOREPROTOCOL_REGISTERDATA* registerdata = GEN_NEW DIOCOREPROTOCOL_REGISTERDATA();
                                                            if(registerdata)
                                                              {
                                                                connection->SetRegisterData(registerdata);
                                                              }
                                                          }    
                                                 
                                                        if(!connection->GetCoreProtocol() && connection->GetRegisterData())
                                                          { 
                                                            DIOCOREPROTOCOL* protocol = CreateProtocol(connection, event->GetDIOStream());
                                                            if(protocol)
                                                              { 
                                                                connection->SetCoreProtocol(protocol); 

                                                                if(connection->IsServer())
                                                                  {
                                                                    if(protocol->GenerateAuthenticationChallenge((*connection->GetAuthenticationChallenge())))
                                                                      {
                                                                        protocol->GenerateAuthenticationResponse((*connection->GetAuthenticationChallenge()), (*connection->GetAuthenticationResponse()));                                                                                                                              
                                                                      }                                                             
                                                                  }
                                                              }  
                                                          }
                                                      }
                                                  }
                                                  break;

      case DIOSTREAM_XEVENT_TYPE_DISCONNECTED   : { DIOCOREPROTOCOL_CONNECTION* connection = Connections_Get(event->GetDIOStream());
                                                    if(connection)
                                                      {                                                        
                                                        connection->SetEvent(DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_DISCONNECTED);                                                                                                             
                                                      
                                                        UnSubscribeEvent(DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_CREATECONNECTION, this);
                                                        UnSubscribeEvent(DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_STATUSCHANGE    , connection);
                                                        UnSubscribeEvent(DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_READMSG         , this);
                                                        UnSubscribeEvent(DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_WRITEMSG        , connection);   
                                                      }                                                 
                                                  }
                                                  break;

                                   default      : break; 
    }

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCOREPROTOCOL_CONNECTIONSMANAGER::HandleEvent(XEVENT* xevent)
* @brief      Handle event
* @note       INTERNAL
* @ingroup    DATAIO
* 
* @param[in]  xevent : Xevent pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCOREPROTOCOL_CONNECTIONSMANAGER::HandleEvent(XEVENT* xevent)
{
  if(!xevent) 
    {
      return;
    }

  switch(xevent->GetEventFamily())
    {
      case XEVENT_TYPE_DIOSTREAM                      : { DIOSTREAM_XEVENT* event = (DIOSTREAM_XEVENT*)xevent;
                                                          if(!event) return;

                                                          HandleEvent_DIOStream(event);
                                                        }
                                                        break;

      case XEVENT_TYPE_COREPROTOCOLCONNECTIONSMANAGER : { DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT* event = (DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT*)xevent;
                                                          if(!event) return;

                                                          HandleEvent_CoreProtocolConnectionsManager(event);
                                                        }
                                                        break; 

                                         default      : break; 
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCOREPROTOCOL_CONNECTIONSMANAGER::ThreadConnections(void* param)
* @brief      Thread connections
* @ingroup    DATAIO
* 
* @param[in]  param : Param pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCOREPROTOCOL_CONNECTIONSMANAGER::ThreadConnections(void* param)
{
  DIOCOREPROTOCOL_CONNECTIONSMANAGER* connectionsmanager = (DIOCOREPROTOCOL_CONNECTIONSMANAGER*)param;
  if(!connectionsmanager) 
    {
      return;
    }

  if(connectionsmanager->leavingactive)
    {
      return;
    }

  if(!connectionsmanager->GetProtocolCFG())
    {
      return;
    }

  if(connectionsmanager->Connections_GetAll()->IsEmpty())
    {
      for(XDWORD c=0; c<connectionsmanager->DIOStream_GetAll()->GetSize(); c++)
        {
          DIOSTREAM* diostream = connectionsmanager->DIOStream_GetAll()->GetElement(c);
          if(diostream)  
            {              
              if(diostream->GetConfig())
                {
                  if(diostream->GetStatus() == DIOSTREAMSTATUS_DISCONNECTED) 
                    {
                      diostream->Close();
                      diostream->Open();                     
                    }                  
                }                            
            }  
        }
    } 
   
  for(XDWORD c=0; c<connectionsmanager->Connections_GetAll()->GetSize(); c++)
    {
      DIOCOREPROTOCOL_CONNECTION* connection = connectionsmanager->Connections_GetAll()->Get(c);
      if(connection)
        {
          if(connection->Status_Get() != DIOCOREPROTOCOL_CONNECTION_STATUS_DISCONNECTED)             
            {   
              if(connection->GetCoreProtocol())
                {
                  DIOCOREPROTOCOL_HEADER  header;
                  XBUFFER                 content;
                  XDWORD                  sizeread;

                  sizeread = connection->GetCoreProtocol()->ReceivedMsg(header, content);
                  if(sizeread)
                    {
                      DIOCOREPROTOCOL_MESSAGE*  message = NULL;
                      bool                      status  = false;

                      switch(header.GetMessageType())
                        {
                          case DIOCOREPROTOCOL_HEADER_MESSAGETYPE_REQUEST   : { message = GEN_NEW DIOCOREPROTOCOL_MESSAGE();
                                                                                if(message)
                                                                                  {
                                                                                    message->SetAcquisitionType(DIOCOREPROTOCOL_MESSAGE_TYPE_ACQUISITION_READ);
                                                                                    message->GetHeader()->CopyFrom(&header);
                                                                                    message->GetContent()->CopyFrom(content);  
                                                                                    message->SetSizeAllMessage(sizeread);

                                                                                    status = connection->Messages_GetAll()->AddRequest(message);
                                                                                  }                                                                            
                                                                              } 
                                                                              break;

                          case DIOCOREPROTOCOL_HEADER_MESSAGETYPE_RESPONSE  : { message = GEN_NEW DIOCOREPROTOCOL_MESSAGE();
                                                                                if(message)
                                                                                  {                                                                               
                                                                                    message->SetAcquisitionType(DIOCOREPROTOCOL_MESSAGE_TYPE_ACQUISITION_READ);
                                                                                    message->GetHeader()->CopyFrom(&header);
                                                                                    message->GetContent()->CopyFrom(content);  
                                                                                    message->SetSizeAllMessage(sizeread);

                                                                                    status = connection->Messages_GetAll()->AddResponse(message);                                                                                
                                                                                  }                                                                            
                                                                              } 
                                                                              break;  

                                                               default      : break;                                           
                        }
                                    
                      if(status)
                        {
                          DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT xevent(connectionsmanager, DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_READMSG);

                          xevent.SetConnection(connection);
                          xevent.SetActualStatus(connection->Status_Get());                      
                          xevent.SetNextStatus(DIOCOREPROTOCOL_CONNECTION_STATUS_NONE);
                          xevent.SetMsg(message);
 
                          connectionsmanager->PostEvent(&xevent);    

                          connection->GetXTimerWithoutConnexion()->Reset();                                                     
                        }
                       else 
                        {                   
                          GEN_DELETE message;
                        }                       
                    }  
                } 
            }            
        }      
    }
 
  connectionsmanager->Connections_ReadMessages();  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCOREPROTOCOL_CONNECTIONSMANAGER::ThreadUpdateConnection(void* param)
* @brief      thread update connection
* @ingroup    DATAIO
* 
* @param[in]  param : Param pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCOREPROTOCOL_CONNECTIONSMANAGER::ThreadUpdateConnection(void* param)
{
  DIOCOREPROTOCOL_CONNECTIONSMANAGER* connectionsmanager = (DIOCOREPROTOCOL_CONNECTIONSMANAGER*)param;
  if(!connectionsmanager) 
    {
      return;
    }

  if(connectionsmanager->leavingactive)
    {
      return;
    }
   
  for(XDWORD c=0; c<connectionsmanager->Connections_GetAll()->GetSize(); c++)
    {
      DIOCOREPROTOCOL_CONNECTION* connection = connectionsmanager->Connections_GetAll()->Get(c);
      if(connection)
        {
          if(connection->Status_Get() != DIOCOREPROTOCOL_CONNECTION_STATUS_DISCONNECTED)             
            {   
              connection->Update();   
            }
        }
    } 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCOREPROTOCOL_CONNECTIONSMANAGER::ThreadAutomaticOperations(void* param)
* @brief      Thread automatic operations
* @ingroup    DATAIO
* 
* @param[in]  param : Param pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCOREPROTOCOL_CONNECTIONSMANAGER::ThreadAutomaticOperations(void* param)
{
  DIOCOREPROTOCOL_CONNECTIONSMANAGER* connectionsmanager = (DIOCOREPROTOCOL_CONNECTIONSMANAGER*)param;
  if(!connectionsmanager) 
    {
      return;
    }

  if(connectionsmanager->leavingactive)
    {
      return;
    }

  connectionsmanager->ManagerUpdateClasses();
   
  connectionsmanager->ManagerHeartBet();  

  connectionsmanager->Connections_DeleteAllDisconnected();  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCOREPROTOCOL_CONNECTIONSMANAGER::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCOREPROTOCOL_CONNECTIONSMANAGER::Clean()
{  
  leavingactive               = false;  

  updateclass_xmutex          = NULL;

  connections_delete_xmutex   = NULL;

  connections_xthread         = NULL;
  connection_update_xthread   = NULL;
 
  automaticoperations_xthread = NULL;
}




