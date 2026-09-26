/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOCoreProtocol_Connection.cpp
* 
* @class      DIOCOREPROTOCOL_CONNECTION
* @brief      Data Input/Output Core Protocol Connection class
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

#include "DIOCoreProtocol_Connection.h"

#include "XFactory.h"
#include "XTimer.h"
#include "XThreadCollected.h"

#include "DIOStream.h"
#include "DIOStreamEnumServers.h"

#include "DIOCoreProtocol_Header.h"
#include "DIOCoreProtocol.h"
#include "DIOCoreProtocol_CFG.h"
#include "DIOCoreProtocol_ConnectionsManager_XEvent.h"

#include "HashSHA2.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCOREPROTOCOL_CONNECTION::DIOCOREPROTOCOL_CONNECTION() : XFSMACHINE(0)
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCOREPROTOCOL_CONNECTION::DIOCOREPROTOCOL_CONNECTION() : XFSMACHINE(0)
{
  Clean(); 

  operation_mutex         = GEN_XFACTORY.Create_Mutex();
  insidecommand_mutex     = GEN_XFACTORY.Create_Mutex();

  xtimerstatus            = GEN_XFACTORY.CreateTimer();
  xtimerwithoutconnexion  = GEN_XFACTORY.CreateTimer();
  xtimeroutresponse       = GEN_XFACTORY.CreateTimer();

  InitFSMachine();

  CreateIDConnection(ID_connection);

  ciphercurve.GenerateRandomPrivateKey();
  ciphercurve.CreatePublicKey();

  RegisterEvent(DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_STATUSCHANGE);
  RegisterEvent(DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_WRITEMSG);

  SetEvent(DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_CONNECTED);  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCOREPROTOCOL_CONNECTION::~DIOCOREPROTOCOL_CONNECTION()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCOREPROTOCOL_CONNECTION::~DIOCOREPROTOCOL_CONNECTION()
{
  DeRegisterEvent(DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_STATUSCHANGE);
  RegisterEvent(DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_WRITEMSG);

  if(registerdata)
    {
      GEN_DELETE registerdata;
    }

  if(protocol)
    {
      GEN_DELETE protocol;
    }

  if(xtimerstatus)
    {
      GEN_XFACTORY.DeleteTimer(xtimerstatus);
    }

  if(xtimerwithoutconnexion)
    {
      GEN_XFACTORY.DeleteTimer(xtimerwithoutconnexion);
    }

  if(xtimeroutresponse)
    {
      GEN_XFACTORY.DeleteTimer(xtimeroutresponse);
    }

  if(operation_mutex)
    {
      GEN_XFACTORY.Delete_Mutex(operation_mutex);
    }

  if(insidecommand_mutex)
    {
      GEN_XFACTORY.Delete_Mutex(insidecommand_mutex);
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTION::InitFSMachine()
* @brief      Init FS machine
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTION::InitFSMachine()
{
  if(!AddState( DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_NONE               ,
                DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_CONNECTED          ,  DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_CONNECTED           ,
                DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_DISCONNECTED       ,  DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_DISCONNECTED        ,                
                XFSMACHINESTATE_EVENTDEFEND)) return false;

  if(!AddState( DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_CONNECTED          ,
                DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_KEYEXCHANGE        ,  DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_KEYEXCHANGE         ,                
                DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_DISCONNECTED       ,  DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_DISCONNECTED        ,                
                XFSMACHINESTATE_EVENTDEFEND)) return false; 

  if(!AddState( DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_KEYEXCHANGE        ,
                DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_AUTHENTICATION     ,  DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_AUTHENTICATION      ,
                DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_DISCONNECTED       ,  DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_DISCONNECTED        ,                
                XFSMACHINESTATE_EVENTDEFEND)) return false;

  if(!AddState( DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_AUTHENTICATION     ,
                DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_REGISTRATION       ,  DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_REGISTRATION        ,
                DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_DISCONNECTED       ,  DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_DISCONNECTED        ,                
                XFSMACHINESTATE_EVENTDEFEND)) return false;

  if(!AddState( DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_REGISTRATION       ,
                DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_WAITREADY          ,  DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_WAITREADY           ,
                DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_DISCONNECTED       ,  DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_DISCONNECTED        ,                
                XFSMACHINESTATE_EVENTDEFEND)) return false;

  if(!AddState( DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_WAITREADY          ,
                DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_READY              ,  DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_READY               ,
                DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_DISCONNECTED       ,  DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_DISCONNECTED        ,                
                XFSMACHINESTATE_EVENTDEFEND)) return false;
  
  if(!AddState( DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_READY              ,
                DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_INSTABILITY        ,  DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_INSTABILITY         ,
                DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_DISCONNECTED       ,  DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_DISCONNECTED        ,                
                XFSMACHINESTATE_EVENTDEFEND)) return false;

  if(!AddState( DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_INSTABILITY        ,
                DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_READY              ,  DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_READY               ,
                DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_DISCONNECTED       ,  DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_DISCONNECTED        ,                
                XFSMACHINESTATE_EVENTDEFEND)) return false;
  
  if(!AddState( DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_DISCONNECTED       ,
                DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_NONE               ,  DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_NONE                ,                
                XFSMACHINESTATE_EVENTDEFEND)) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTION::IsServer()
* @brief      Is server
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTION::IsServer()
{
  if(!protocol)
    {
      return false;
    }

  if(!protocol->GetProtocolCFG())
    {
      return false;
    }

  return protocol->GetProtocolCFG()->GetIsServer();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XUUID* DIOCOREPROTOCOL_CONNECTION::GetIDConnection()
* @brief      Get ID connection
* @ingroup    DATAIO
* 
* @return     XUUID* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XUUID* DIOCOREPROTOCOL_CONNECTION::GetIDConnection()
{
  return &ID_connection;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCOREPROTOCOL* DIOCOREPROTOCOL_CONNECTION::GetCoreProtocol()
* @brief      Get core protocol
* @ingroup    DATAIO
* 
* @return     DIOCOREPROTOCOL* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCOREPROTOCOL* DIOCOREPROTOCOL_CONNECTION::GetCoreProtocol()
{
  return protocol;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTION::SetCoreProtocol(DIOCOREPROTOCOL* protocol)
* @brief      Set core protocol
* @ingroup    DATAIO
* 
* @param[in]  protocol : Protocol pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTION::SetCoreProtocol(DIOCOREPROTOCOL* protocol)
{
  this->protocol = protocol;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBUFFER* DIOCOREPROTOCOL_CONNECTION::GetAuthenticationChallenge()
* @brief      Get authentication challenge
* @ingroup    DATAIO
* 
* @return     XBUFFER* : Pointer to the requested buffer; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* DIOCOREPROTOCOL_CONNECTION::GetAuthenticationChallenge()
{
  return &authentication_challenge;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBUFFER* DIOCOREPROTOCOL_CONNECTION::GetAuthenticationResponse()
* @brief      Get authentication response
* @ingroup    DATAIO
* 
* @return     XBUFFER* : Pointer to the requested buffer; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* DIOCOREPROTOCOL_CONNECTION::GetAuthenticationResponse()
{
  return &authentication_response;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHERKEYSYMMETRICAL* DIOCOREPROTOCOL_CONNECTION::GetCipherKey()
* @brief      Get cipher key
* @ingroup    DATAIO
* 
* @return     CIPHERKEYSYMMETRICAL* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERKEYSYMMETRICAL* DIOCOREPROTOCOL_CONNECTION::GetCipherKey()
{
  return &cipherkey;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCOREPROTOCOL_CONNECTION_STATUS DIOCOREPROTOCOL_CONNECTION::Status_Get()
* @brief      Status get
* @ingroup    DATAIO
* 
* @return     DIOCOREPROTOCOL_CONNECTION_STATUS : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCOREPROTOCOL_CONNECTION_STATUS DIOCOREPROTOCOL_CONNECTION::Status_Get()
{
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCOREPROTOCOL_CONNECTION::Status_Set(DIOCOREPROTOCOL_CONNECTION_STATUS status)
* @brief      Status set
* @ingroup    DATAIO
* 
* @param[in]  status : Status value to read or update.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCOREPROTOCOL_CONNECTION::Status_Set(DIOCOREPROTOCOL_CONNECTION_STATUS status)
{
  if(this->status != status)
    {
      if(xtimerstatus)
        {
          xtimerstatus->Reset();
        }

      DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT xevent(this, DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_STATUSCHANGE);
      xevent.SetConnection(this);
      xevent.SetActualStatus(this->status);
      xevent.SetNextStatus(status);

      PostEvent(&xevent);
    }

  this->status = status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTION::Status_GetString(DIOCOREPROTOCOL_CONNECTION_STATUS status, XSTRING& statusstring)
* @brief      Status get string
* @ingroup    DATAIO
* 
* @param[in]  status : Status value to read or update.
* @param[in]  statusstring : Statusstring value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTION::Status_GetString(DIOCOREPROTOCOL_CONNECTION_STATUS status, XSTRING& statusstring)
{
  statusstring.Empty();

  switch(status)
    {
      case DIOCOREPROTOCOL_CONNECTION_STATUS_NONE               : statusstring = __L("None");                 break;
      case DIOCOREPROTOCOL_CONNECTION_STATUS_CONNECTED          : statusstring = __L("Connected");            break;     
      case DIOCOREPROTOCOL_CONNECTION_STATUS_KEYEXCHANGE        : statusstring = __L("Key exchange");         break;
      case DIOCOREPROTOCOL_CONNECTION_STATUS_AUTHENTICATED      : statusstring = __L("Authenticated");        break;
      case DIOCOREPROTOCOL_CONNECTION_STATUS_REGISTERED         : statusstring = __L("Registered");           break;  
      case DIOCOREPROTOCOL_CONNECTION_STATUS_WAITREADY          : statusstring = __L("WaitReady");            break;    
      case DIOCOREPROTOCOL_CONNECTION_STATUS_READY              : statusstring = __L("Ready");                break;
      case DIOCOREPROTOCOL_CONNECTION_STATUS_INSTABILITY        : statusstring = __L("Instability");          break;
      case DIOCOREPROTOCOL_CONNECTION_STATUS_DISCONNECTED       : statusstring = __L("Disconnected");         break;
    }

  if(statusstring.IsEmpty())
    {
      return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTION::Status_GetString(XSTRING& statusstring)
* @brief      Status get string
* @ingroup    DATAIO
* 
* @param[in]  statusstring : Statusstring value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTION::Status_GetString(XSTRING& statusstring)
{  
  return Status_GetString(status, statusstring);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTIMER* DIOCOREPROTOCOL_CONNECTION::GetXTimerStatus()
* @brief      Get X timer status
* @ingroup    DATAIO
* 
* @return     XTIMER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTIMER* DIOCOREPROTOCOL_CONNECTION::GetXTimerStatus()
{
  return xtimerstatus;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTIMER* DIOCOREPROTOCOL_CONNECTION::GetXTimerWithoutConnexion()
* @brief      Get X timer without connexion
* @ingroup    DATAIO
* 
* @return     XTIMER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTIMER* DIOCOREPROTOCOL_CONNECTION::GetXTimerWithoutConnexion()
{
  return xtimerwithoutconnexion;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCOREPROTOCOL_MESSAGES* DIOCOREPROTOCOL_CONNECTION::Messages_GetAll()
* @brief      Messages get all
* @ingroup    DATAIO
* 
* @return     DIOCOREPROTOCOL_MESSAGES* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCOREPROTOCOL_MESSAGES* DIOCOREPROTOCOL_CONNECTION::Messages_GetAll()
{
  return &messages;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTION::Command_Do(XDWORD command_type, XBUFFER& result, XDWORD timeout)
* @brief      command  do
* @ingroup    DATAIO
* 
* @param[in]  command_type : Command type value.
* @param[in]  result : Output result.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTION::Command_Do(XDWORD command_type, XBUFFER& result, XDWORD timeout)
{
  COMMAND_DO_WITHOUTPARAMS(command_type, result, timeout)
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTION::Command_Do(XDWORD command_type, XSTRING& result, XDWORD timeout)
* @brief      command  do
* @ingroup    DATAIO
* 
* @param[in]  command_type : Command type value.
* @param[in]  result : Output result.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTION::Command_Do(XDWORD command_type, XSTRING& result, XDWORD timeout)
{
  COMMAND_DO_WITHOUTPARAMS(command_type, result, timeout)
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTION::Command_Do(XDWORD command_type, XFILEJSON& result, XDWORD timeout)
* @brief      command  do
* @ingroup    DATAIO
* 
* @param[in]  command_type : Command type value.
* @param[in]  result : Output result.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTION::Command_Do(XDWORD command_type, XFILEJSON& result, XDWORD timeout)
{
  COMMAND_DO_WITHOUTPARAMS(command_type, result, timeout)
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTION::Command_Do(XDWORD command_type, XBUFFER* params, XBUFFER& result, XDWORD timeout)
* @brief      command  do
* @ingroup    DATAIO
* 
* @param[in]  command_type : Command type value.
* @param[in]  params : Params pointer to use.
* @param[in]  result : Output result.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTION::Command_Do(XDWORD command_type, XBUFFER* params, XBUFFER& result, XDWORD timeout)
{
  COMMAND_DO(command_type, params, result, timeout)
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTION::Command_Do(XDWORD command_type, XSTRING* params, XBUFFER& result, XDWORD timeout)
* @brief      command  do
* @ingroup    DATAIO
* 
* @param[in]  command_type : Command type value.
* @param[in]  params : Params pointer to use.
* @param[in]  result : Output result.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTION::Command_Do(XDWORD command_type, XSTRING* params, XBUFFER& result, XDWORD timeout)
{
  COMMAND_DO(command_type, params, result, timeout)
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTION::Command_Do(XDWORD command_type, XFILEJSON* params, XBUFFER& result, XDWORD timeout)
* @brief      command  do
* @ingroup    DATAIO
* 
* @param[in]  command_type : Command type value.
* @param[in]  params : Params pointer to use.
* @param[in]  result : Output result.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTION::Command_Do(XDWORD command_type, XFILEJSON* params, XBUFFER& result, XDWORD timeout)
{
  COMMAND_DO(command_type, params, result, timeout)
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTION::Command_Do(XDWORD command_type, XBUFFER* params, XSTRING& result, XDWORD timeout)
* @brief      command  do
* @ingroup    DATAIO
* 
* @param[in]  command_type : Command type value.
* @param[in]  params : Params pointer to use.
* @param[in]  result : Output result.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTION::Command_Do(XDWORD command_type, XBUFFER* params, XSTRING& result, XDWORD timeout)
{
  COMMAND_DO(command_type, params, result, timeout)
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTION::Command_Do(XDWORD command_type, XSTRING* params, XSTRING& result, XDWORD timeout)
* @brief      command  do
* @ingroup    DATAIO
* 
* @param[in]  command_type : Command type value.
* @param[in]  params : Params pointer to use.
* @param[in]  result : Output result.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTION::Command_Do(XDWORD command_type, XSTRING* params, XSTRING& result, XDWORD timeout)
{
  COMMAND_DO(command_type, params, result, timeout)
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTION::Command_Do(XDWORD command_type, XFILEJSON* params, XSTRING& result, XDWORD timeout)
* @brief      command  do
* @ingroup    DATAIO
* 
* @param[in]  command_type : Command type value.
* @param[in]  params : Params pointer to use.
* @param[in]  result : Output result.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTION::Command_Do(XDWORD command_type, XFILEJSON* params, XSTRING& result, XDWORD timeout)
{
  COMMAND_DO(command_type, params, result, timeout)
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTION::Command_Do(XDWORD command_type, XBUFFER* params, XFILEJSON& result, XDWORD timeout)
* @brief      command  do
* @ingroup    DATAIO
* 
* @param[in]  command_type : Command type value.
* @param[in]  params : Params pointer to use.
* @param[in]  result : Output result.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTION::Command_Do(XDWORD command_type, XBUFFER* params, XFILEJSON& result, XDWORD timeout)
{
  COMMAND_DO(command_type, params, result, timeout)
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTION::Command_Do(XDWORD command_type, XSTRING* params, XFILEJSON& result, XDWORD timeout)
* @brief      command  do
* @ingroup    DATAIO
* 
* @param[in]  command_type : Command type value.
* @param[in]  params : Params pointer to use.
* @param[in]  result : Output result.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTION::Command_Do(XDWORD command_type, XSTRING* params, XFILEJSON& result, XDWORD timeout)
{
  COMMAND_DO(command_type, params, result, timeout)
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTION::Command_Do(XDWORD command_type, XFILEJSON* params, XFILEJSON& result, XDWORD timeout)
* @brief      command  do
* @ingroup    DATAIO
* 
* @param[in]  command_type : Command type value.
* @param[in]  params : Params pointer to use.
* @param[in]  result : Output result.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTION::Command_Do(XDWORD command_type, XFILEJSON* params, XFILEJSON& result, XDWORD timeout)
{
  COMMAND_DO(command_type, params, result, timeout)
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTION::Command_Do(XUUID* ID_message, XDWORD command_type)
* @brief      command  do
* @ingroup    DATAIO
* 
* @param[in]  ID_message : ID message pointer to use.
* @param[in]  command_type : Command type value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTION::Command_Do(XUUID* ID_message, XDWORD command_type)
{
  XSTRING commandstr;
  bool    status;
  
  if(!protocol)
    {
      return false;
    }

  DIOCOREPROTOCOL_COMMAND* coreprotocol_command = protocol->Commands_GetCoreProtocol(command_type);
  if(!coreprotocol_command)
    {
      return false;  
    }

  if(ID_message->IsEmpty())
    {
      if(!IsServer())
        {
          if(coreprotocol_command->GetBidirectionalityMode() == DIOCOREPROTOCOL_BIDIRECTIONALITYMODE_TOCLIENT)
            {
              return false;
            }
        }
       else
        {
          if(coreprotocol_command->GetBidirectionalityMode() == DIOCOREPROTOCOL_BIDIRECTIONALITYMODE_TOSERVER)
            {
              return false;
            }
        }
    }

  status = SendMsg(ID_message, DIOCOREPROTOCOL_HEADER_OPERATION_COMMAND, coreprotocol_command->GetTypeString()->Get());
 
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTION::Command_Do(XUUID* ID_message, XDWORD command_type, XBUFFER* params)
* @brief      command  do
* @ingroup    DATAIO
* 
* @param[in]  ID_message : ID message pointer to use.
* @param[in]  command_type : Command type value.
* @param[in]  params : Params pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTION::Command_Do(XUUID* ID_message, XDWORD command_type, XBUFFER* params)
{
  XSTRING commandstr;
  bool    status;
  
  if(!protocol)
    {
      return false;
    }
 
  DIOCOREPROTOCOL_COMMAND* coreprotocol_command = protocol->Commands_GetCoreProtocol(command_type);
  if(!coreprotocol_command)
    {
      return false;  
    }

  if(ID_message->IsEmpty())
    {
      if(!IsServer())
        {
          if(coreprotocol_command->GetBidirectionalityMode() == DIOCOREPROTOCOL_BIDIRECTIONALITYMODE_TOCLIENT)
            {
              return false;
            }
        }
       else
        {
          if(coreprotocol_command->GetBidirectionalityMode() == DIOCOREPROTOCOL_BIDIRECTIONALITYMODE_TOSERVER)
            {
              return false;
            }
        }
    }

  status = SendMsg(ID_message, DIOCOREPROTOCOL_HEADER_OPERATION_COMMAND, coreprotocol_command->GetTypeString()->Get(), params);
 
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTION::Command_Do(XUUID* ID_message, XDWORD command_type, XSTRING* params)
* @brief      command  do
* @ingroup    DATAIO
* 
* @param[in]  ID_message : ID message pointer to use.
* @param[in]  command_type : Command type value.
* @param[in]  params : Params pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTION::Command_Do(XUUID* ID_message, XDWORD command_type, XSTRING* params)
{
  XSTRING commandstr;
  bool    status;
  
  if(!protocol)
    {
      return false;
    }

  DIOCOREPROTOCOL_COMMAND* coreprotocol_command = protocol->Commands_GetCoreProtocol(command_type);
  if(!coreprotocol_command)
    {
      return false;  
    }

  if(ID_message->IsEmpty())
    {
      if(!IsServer())
        {
          if(coreprotocol_command->GetBidirectionalityMode() == DIOCOREPROTOCOL_BIDIRECTIONALITYMODE_TOCLIENT)
            {
              return false;
            }
        }
       else
        {
          if(coreprotocol_command->GetBidirectionalityMode() == DIOCOREPROTOCOL_BIDIRECTIONALITYMODE_TOSERVER)
            {
              return false;
            }
        }
    }

  status = SendMsg(ID_message, DIOCOREPROTOCOL_HEADER_OPERATION_COMMAND, coreprotocol_command->GetTypeString()->Get(), params);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTION::Command_Do(XUUID* ID_message, XDWORD command_type, XFILEJSON* params)
* @brief      command  do
* @ingroup    DATAIO
* 
* @param[in]  ID_message : ID message pointer to use.
* @param[in]  command_type : Command type value.
* @param[in]  params : Params pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTION::Command_Do(XUUID* ID_message, XDWORD command_type, XFILEJSON* params)
{
  XSTRING commandstr;
  bool    status;
  
  if(!protocol)
    {
      return false;
    }

  DIOCOREPROTOCOL_COMMAND* coreprotocol_command = protocol->Commands_GetCoreProtocol(command_type);
  if(!coreprotocol_command)
    {
      return false;  
    }

  if(ID_message->IsEmpty())
    {
      if(!IsServer())
        {
          if(coreprotocol_command->GetBidirectionalityMode() == DIOCOREPROTOCOL_BIDIRECTIONALITYMODE_TOCLIENT)
            {
              return false;
            }
        }
       else
        {
          if(coreprotocol_command->GetBidirectionalityMode() == DIOCOREPROTOCOL_BIDIRECTIONALITYMODE_TOSERVER)
            {
              return false;
            }
        }
    }

  status = SendMsg(ID_message, DIOCOREPROTOCOL_HEADER_OPERATION_COMMAND, coreprotocol_command->GetTypeString()->Get(), params);

  return status; 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTION::UpdateClass_Do(XCHAR* classname, XSERIALIZABLE* classserializable, XDWORD timeout)
* @brief      update class  do
* @ingroup    DATAIO
* 
* @param[in]  classname : Classname pointer to use.
* @param[in]  classserializable : Classserializable pointer to use.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTION::UpdateClass_Do(XCHAR* classname, XSERIALIZABLE* classserializable, XDWORD timeout)
{
  XUUID                 ID_message;      
  XFILEJSON             classcontent;                                                               
  bool                  status  = false;
  XSERIALIZATIONMETHOD* serializationmethod = XSERIALIZABLE::CreateInstance(classcontent);
  if(!serializationmethod)
    {
      return false;
    }
    
  if(!classserializable)
    {
      return false;
    } 
  
  classserializable->SetSerializationMethod(serializationmethod);
  classserializable->Serialize(); 

  if(serializationmethod)
    {
      GEN_DELETE serializationmethod;
    }
                
  if(UpdateClass_Do(&ID_message, classname, &classcontent))
    {     
      XFILEJSON resultcontent;  
                                                                              
      status = GetResult(&ID_message, resultcontent, timeout);                   
      if(status)
        {
          XFILEJSONVALUE* value = resultcontent.GetValue(DIOCOREPROTOCOL_UPDATECLASS_CONFIRM_PARAM);
          if(value)
            {
              status = value->GetValueBoolean();
            }  
        }
    }
                                                                    
  return status;       
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTION::UpdateClass_DoAsk(XCHAR* classname, XSERIALIZABLE* classserializable, XDWORD timeout)
* @brief      update class  do ask
* @ingroup    DATAIO
* 
* @param[in]  classname : Classname pointer to use.
* @param[in]  classserializable : Classserializable pointer to use.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTION::UpdateClass_DoAsk(XCHAR* classname, XSERIALIZABLE* classserializable, XDWORD timeout)
{
  XUUID                 ID_message;      
  bool                  status  = false;

  if(!classserializable)
    {
      return false;
    } 
                
  if(UpdateClass_DoAsk(&ID_message, classname))
    {     
      XFILEJSON resultcontent;  
                                                                              
      status = GetResult(&ID_message, resultcontent, timeout);                   
      if(status)
        {  
          XSERIALIZATIONMETHOD* serializationmethod = XSERIALIZABLE::CreateInstance(resultcontent);
          if(!serializationmethod)
            {  
              return false;
            }
                 
          classserializable->SetSerializationMethod(serializationmethod);
          status = classserializable->Deserialize(); 

          if(serializationmethod)
            {
              GEN_DELETE serializationmethod;
            }
        }
    }
                                             
  return status;       
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTION::UpdateClass_Do(XUUID* ID_message, XCHAR* classname, XFILEJSON* classcontent)
* @brief      update class  do
* @ingroup    DATAIO
* 
* @param[in]  ID_message : ID message pointer to use.
* @param[in]  classname : Classname pointer to use.
* @param[in]  classcontent : Classcontent pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTION::UpdateClass_Do(XUUID* ID_message, XCHAR* classname, XFILEJSON* classcontent)
{
  bool status = false;
  
  if(!protocol)
    {
      return false;
    }

  if(protocol->GetProtocolCFG())
    {
      if(protocol->GetProtocolCFG()->GetIsServer())
        {
          //return false;
        }
    }
  
  status = SendMsg(ID_message, DIOCOREPROTOCOL_HEADER_OPERATION_UPDATECLASS, classname, classcontent);

  return status; 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTION::UpdateClass_DoAsk(XUUID* ID_message, XCHAR* classname, XSTRING* classcontent)
* @brief      update class  do ask
* @ingroup    DATAIO
* 
* @param[in]  ID_message : ID message pointer to use.
* @param[in]  classname : Classname pointer to use.
* @param[in]  classcontent : Classcontent pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTION::UpdateClass_DoAsk(XUUID* ID_message, XCHAR* classname, XSTRING* classcontent)
{
  bool status = false;
  
  if(!protocol)
    {
      return false;
    }

  if(protocol->GetProtocolCFG())
    {
      if(!protocol->GetProtocolCFG()->GetIsServer())
        {
          //return false;
        }
    }
  
  status = SendMsg(ID_message, DIOCOREPROTOCOL_HEADER_OPERATION_ASKUPDATECLASS, classname, classcontent);

  return status; 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTION::GetResult(XUUID* ID_message, XBUFFER& result, XDWORD timeout)
* @brief      get result
* @ingroup    DATAIO
* 
* @param[in]  ID_message : ID message pointer to use.
* @param[in]  result : Output result.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTION::GetResult(XUUID* ID_message, XBUFFER& result, XDWORD timeout)
{
  XTIMER* xtimer = GEN_XFACTORY.CreateTimer();
  if(!xtimer)
    {
      return false;
    }

  bool status  = false;

  while(1)
    {
      int index = Messages_GetAll()->FindResponse(ID_message);
      if(index != NOTFOUND)
        {
          DIOCOREPROTOCOL_MESSAGE* message = Messages_GetAll()->GetAll()->GetElement(index);
          if(message)
            {
              message->SetIsConsumed(true);
              result.CopyFrom((*message->GetContent()));

              status = true;
              break;   
            }
        }
      
      if(xtimer->GetMeasureSeconds() >= timeout)
        {
          break;
        }
    }
  
  GEN_XFACTORY.DeleteTimer(xtimer);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTION::GetResult(XUUID* ID_message, XSTRING& result, XDWORD timeout)
* @brief      get result
* @ingroup    DATAIO
* 
* @param[in]  ID_message : ID message pointer to use.
* @param[in]  result : Output result.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTION::GetResult(XUUID* ID_message, XSTRING& result, XDWORD timeout)
{ 
  XTIMER* xtimer = GEN_XFACTORY.CreateTimer();
  if(!xtimer)
    {
      return false;
    }

  bool status  = false;

  while(1)
    {
      int index = Messages_GetAll()->FindResponse(ID_message);
      if(index != NOTFOUND)
        {
          DIOCOREPROTOCOL_MESSAGE* message = Messages_GetAll()->GetAll()->GetElement(index);
          if(message)
            {
              message->SetIsConsumed(true);

              result.ConvertFromUTF8((*message->GetContent()));

              status = true;
              break;   
            }
        }
      
      if(xtimer->GetMeasureSeconds() >= timeout)
        {
          break;
        }
    }
  
  GEN_XFACTORY.DeleteTimer(xtimer);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTION::GetResult(XUUID* ID_message, XFILEJSON& result, XDWORD timeout)
* @brief      get result
* @ingroup    DATAIO
* 
* @param[in]  ID_message : ID message pointer to use.
* @param[in]  result : Output result.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTION::GetResult(XUUID* ID_message, XFILEJSON& result, XDWORD timeout)
{
  XTIMER* xtimer = GEN_XFACTORY.CreateTimer();
  if(!xtimer)
    {
      return false;
    }

  bool status  = false;

  while(1)
    {
      int index = Messages_GetAll()->FindResponse(ID_message);
      if(index != NOTFOUND)
        {
          DIOCOREPROTOCOL_MESSAGE* message = Messages_GetAll()->GetAll()->GetElement(index);
          if(message)
            {              
              message->SetIsConsumed(true);
              
              result.AddBufferLines(XFILETXTFORMATCHAR_UTF8, (*message->GetContent()));
              result.DecodeAllLines();

              status = true;
              break;   
            }
        }
      
      if(xtimer->GetMeasureSeconds() >= timeout)
        {
          break;
        }
    }
  
  GEN_XFACTORY.DeleteTimer(xtimer);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTION::Update()
* @brief      Update
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTION::Update()
{
  if(GetEvent() == DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_NONE) // Not GEN_NEW event
    {
      switch(GetCurrentState())
        {
          case DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_NONE                  : break; 

          case DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_CONNECTED             : break; 
          
          case DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_KEYEXCHANGE           : { DIOCOREPROTOCOL_HEADER  header;
                                                                              XBUFFER                 content;    

                                                                              if(!protocol)
                                                                                {
                                                                                  break;
                                                                                }

                                                                              TIMEOUT_CHAGE_STATE(protocol, xtimeroutresponse)     
                                                                                                                                                                                                                                          
                                                                              if(!IsServer())
                                                                                {                                                                                  
                                                                                  if(GetMsg(false, DIOCOREPROTOCOL_HEADER_OPERATION_KEYEXCHANGE, DIOCOREPROTOCOL_KEYEXCHANGE_CLIENT_OPERATION_PARAM, header, content))
                                                                                    {                                                                                   
                                                                                      if(header.GetContentType() == DIOCOREPROTOCOL_HEADER_CONTENTTYPE_BINARY)
                                                                                        {
                                                                                          if(content.GetSize() == CIPHERECDSAX25519_MAXKEY) 
                                                                                            {
                                                                                              ciphercurve.CreateSharedKey(content.Get());
                                                                                              cipherkey.Set(ciphercurve.GetKey(CIPHERECDSAX25519_TYPEKEY_SHARED), CIPHERECDSAX25519_MAXKEY); 
                                                                                              
                                                                                              Status_Set(DIOCOREPROTOCOL_CONNECTION_STATUS_KEYEXCHANGE); 
                                                                                              SetEvent(DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_AUTHENTICATION);                                                                                                     
                                                                                            }
                                                                                        }
                                                                                    }
                                                                                }                                                                                                                                                                                                                                                                                             
                                                                               else   
                                                                                {                                                                                     
                                                                                  if(GetMsg(true, DIOCOREPROTOCOL_HEADER_OPERATION_KEYEXCHANGE, DIOCOREPROTOCOL_KEYEXCHANGE_SERVER_OPERATION_PARAM, header, content))
                                                                                    {                                                                                      
                                                                                      if(header.GetContentType() == DIOCOREPROTOCOL_HEADER_CONTENTTYPE_BINARY)
                                                                                        {
                                                                                          if(content.GetSize() == CIPHERECDSAX25519_MAXKEY) 
                                                                                            {  
                                                                                              ciphercurve.CreateSharedKey(content.Get());
                                                                                              cipherkey.Set(ciphercurve.GetKey(CIPHERECDSAX25519_TYPEKEY_SHARED), CIPHERECDSAX25519_MAXKEY); 

                                                                                              static bool sended =  false;
                                                                                              if(!sended)     
                                                                                                {        
                                                                                                  XBUFFER publickey;

                                                                                                  publickey.Add(ciphercurve.GetKey(CIPHERECDSAX25519_TYPEKEY_PUBLIC), CIPHERECDSAX25519_MAXKEY);

                                                                                                  sended = SendMsg(header.GetIDMessage(), DIOCOREPROTOCOL_HEADER_OPERATION_KEYEXCHANGE, DIOCOREPROTOCOL_KEYEXCHANGE_CLIENT_OPERATION_PARAM, &publickey);                                                                                                            
                                                                                                  if(sended)
                                                                                                    {                                                                                                    
                                                                                                      Status_Set(DIOCOREPROTOCOL_CONNECTION_STATUS_KEYEXCHANGE);    
                                                                                                      SetEvent(DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_AUTHENTICATION);                                                                                                      
                                                                                                    } 

                                                                                                   sended = false;
                                                                                                }                                                                                                                                                                                                       
                                                                                            }
                                                                                        }                                                                                        
                                                                                    }                                                                                  
                                                                                }  
                                                                            }                                                                                                                                                       
                                                                            break;

          case DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_AUTHENTICATION        : { DIOCOREPROTOCOL_HEADER  header;
                                                                              XBUFFER                 content;    

                                                                              if(!protocol)
                                                                                {
                                                                                  break;
                                                                                }

                                                                              TIMEOUT_CHAGE_STATE(protocol, xtimeroutresponse)

                                                                              if(IsServer())
                                                                                {      
                                                                                  if(GetMsg(false, DIOCOREPROTOCOL_HEADER_OPERATION_AUTHENTICATE, DIOCOREPROTOCOL_AUTHENTICATION_RESPONSE_OPERATION_PARAM, header, content))
                                                                                    {
                                                                                      if(GetAuthenticationResponse()->Compare(content))
                                                                                        {
                                                                                          Status_Set(DIOCOREPROTOCOL_CONNECTION_STATUS_AUTHENTICATED); 
                                                                                          SetEvent(DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_REGISTRATION);                                                                                                           
                                                                                        }
                                                                                       else
                                                                                        {
                                                                                          SetEvent(DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_DISCONNECTED);                                                                                          
                                                                                        }                                                                                                       
                                                                                    }                                     
                                                                                
                                                                                }
                                                                               else 
                                                                                {
                                                                                  if(GetMsg(true, DIOCOREPROTOCOL_HEADER_OPERATION_AUTHENTICATE, DIOCOREPROTOCOL_AUTHENTICATION_CHALLENGE_OPERATION_PARAM, header, content))
                                                                                    {
                                                                                      static bool sended =  false;
                                                                                      if(!sended)
                                                                                        {
                                                                                          GetAuthenticationChallenge()->CopyFrom(content);

                                                                                          if(protocol)
                                                                                            {
                                                                                              protocol->GenerateAuthenticationResponse((*GetAuthenticationChallenge()), (*GetAuthenticationResponse()));                                                                                                                                                                                                                     
                                                                                              sended = SendMsg(header.GetIDMessage(), DIOCOREPROTOCOL_HEADER_OPERATION_AUTHENTICATE, DIOCOREPROTOCOL_AUTHENTICATION_RESPONSE_OPERATION_PARAM, GetAuthenticationResponse());                                                                                                            
                                                                                              if(sended)
                                                                                                {
                                                                                                  Status_Set(DIOCOREPROTOCOL_CONNECTION_STATUS_AUTHENTICATED); 
                                                                                                  SetEvent(DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_REGISTRATION);                                                                                                   
                                                                                                }
                                                                                            }

                                                                                          sended = false;
                                                                                        }
                                                                                    }                                                                                     
                                                                                }
                                                                            }
                                                                            break; 

          case DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_REGISTRATION          : { DIOCOREPROTOCOL_HEADER  header;
                                                                              XFILEJSON               classcontent;
                                                                              XSTRING                 confirmstr;  
                                                                              XSERIALIZATIONMETHOD*   serializationmethod = XSERIALIZABLE::CreateInstance(classcontent);

                                                                              if(!serializationmethod)
                                                                                {
                                                                                  break;
                                                                                }      
                                                                                                                                                                     
                                                                              if(!GetRegisterData())
                                                                                {
                                                                                  break;
                                                                                }  

                                                                              if(!protocol)
                                                                                {
                                                                                  break;
                                                                                }

                                                                              TIMEOUT_CHAGE_STATE(protocol, xtimeroutresponse)

                                                                              GetRegisterData()->SetSerializationMethod(serializationmethod);                                                           
  
                                                                              if(IsServer())
                                                                                { 
                                                                                  if(GetMsg(true, DIOCOREPROTOCOL_HEADER_OPERATION_REGISTERDATA, DIOCOREPROTOCOL_REGISTRATIONDATA_SEND_OPERATION_PARAM, header, classcontent))
                                                                                    { 
                                                                                      static bool sended =  false;

                                                                                      GetRegisterData()->Deserialize();  
                                                                                    
                                                                                      if(GetRegisterData()->InitializeData(IsServer()))
                                                                                        {                                                                                        
                                                                                          //GetRegisterData()->ShowDebug();                                                                                  
                                                                                      
                                                                                          if(!sended)
                                                                                            { 
                                                                                              classcontent.DecodeAllLines();
                                                                                              classcontent.DeleteAllObjects();    

                                                                                              if(serializationmethod)
                                                                                                {
                                                                                                  GEN_DELETE serializationmethod;
                                                                                                  serializationmethod = NULL;  
                                                                                                } 

                                                                                              serializationmethod = XSERIALIZABLE::CreateInstance(classcontent);
                                                                                              GetRegisterData()->SetSerializationMethod(serializationmethod);  

                                                                                              GetRegisterData()->GetIDConnection()->CopyFrom((*GetIDConnection()));
                                                                                                                                                                                                                    
                                                                                              GetRegisterData()->Serialize();
                                                                                          
                                                                                              sended = SendMsg(header.GetIDMessage(), DIOCOREPROTOCOL_HEADER_OPERATION_REGISTERDATA, DIOCOREPROTOCOL_REGISTRATIONDATA_RESPONSE_OPERATION_PARAM, &classcontent);                                                                                                            
                                                                                              if(sended)
                                                                                                {
                                                                                                  Status_Set(DIOCOREPROTOCOL_CONNECTION_STATUS_REGISTERED); 
                                                                                                  SetEvent(DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_WAITREADY);                                                                                               
                                                                                                }

                                                                                              sended = false;
                                                                                            }
                                                                                        }
                                                                                       else
                                                                                        {                                                                                         
                                                                                          SetEvent(DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_DISCONNECTED);                                                                                                                                                                                   
                                                                                        }
                                                                                    }                          
                                                                                                                                                                                  
                                                                                }
                                                                               else 
                                                                                {
                                                                                  if(GetMsg(false, DIOCOREPROTOCOL_HEADER_OPERATION_REGISTERDATA, DIOCOREPROTOCOL_REGISTRATIONDATA_RESPONSE_OPERATION_PARAM, header, classcontent))
                                                                                    {
                                                                                      GetRegisterData()->Deserialize();   

                                                                                      GetIDConnection()->CopyFrom((*GetRegisterData()->GetIDConnection()));      

                                                                                      //GetRegisterData()->ShowDebug();  
                                                                                      
                                                                                      Status_Set(DIOCOREPROTOCOL_CONNECTION_STATUS_REGISTERED); 
                                                                                      SetEvent(DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_WAITREADY);                                                                                                                                                                                                                                                                                      
                                                                                    }                                                                                                                                                             
                                                                                }

                                                                              if(serializationmethod)
                                                                                {
                                                                                  GEN_DELETE serializationmethod;
                                                                                  serializationmethod = NULL;  
                                                                                }                                                                                           
                                                                            }
                                                                            break; 

          case DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_WAITREADY             : { DIOCOREPROTOCOL_HEADER  header;
                                                                              XSTRING                 content;

                                                                              if(!protocol)
                                                                                {
                                                                                  break;
                                                                                }

                                                                              TIMEOUT_CHAGE_STATE(protocol, xtimeroutresponse)

                                                                              if(IsServer())
                                                                                { 
                                                                                  if(GetMsg(true, DIOCOREPROTOCOL_HEADER_OPERATION_TOREADYSERVER, DIOCOREPROTOCOL_WAITREADY_READY_OPERATION_PARAM, header, content))
                                                                                    {   
                                                                                      XUUID   ID_message;      
                                                                                      XSTRING content;                                                                                                                                                   

                                                                                      if(SendMsg(&ID_message, DIOCOREPROTOCOL_HEADER_OPERATION_TOREADYCLIENT, DIOCOREPROTOCOL_WAITREADY_READY_OPERATION_PARAM, &content))
                                                                                        {
                                                                                          SetEvent(DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_READY);                                                                                      
                                                                                        }                                                                                                                                                                        
                                                                                    }
                                                                                } 
                                                                               else
                                                                                {
                                                                                  if(GetMsg(true, DIOCOREPROTOCOL_HEADER_OPERATION_TOREADYCLIENT, DIOCOREPROTOCOL_WAITREADY_READY_OPERATION_PARAM, header, content))
                                                                                    {   
                                                                                      SetEvent(DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_READY);                                                                                                                                                                                                                                                                                                                                                      
                                                                                    }              
                                                                                }                                                                                 
                                                                            }
                                                                            break; 
             
          case DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_READY                 : break; 

          case DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_INSTABILITY           : break;
          
          case DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_DISCONNECTED          : break;         
        }
    }
   else //  New event
    {
      if(GetEvent()<DIOCOREPROTOCOL_CONNECTION_LASTEVENT)
        {
          CheckTransition();

          if(xtimeroutresponse)              
            {
              xtimeroutresponse->Reset();
            }

          switch(GetCurrentState())
            {
              case DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_NONE                  : break; 

              case DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_CONNECTED             : Status_Set(DIOCOREPROTOCOL_CONNECTION_STATUS_CONNECTED);    
                                                                                SetEvent(DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_KEYEXCHANGE);                                                                          
                                                                                break;  

              case DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_KEYEXCHANGE           : if(!IsServer())
                                                                                  {          
                                                                                    XUUID   ID_message;                                                                          
                                                                                    XBUFFER publickey;
                                                                                    
                                                                                    publickey.Add(ciphercurve.GetKey(CIPHERECDSAX25519_TYPEKEY_PUBLIC), CIPHERECDSAX25519_MAXKEY);

                                                                                    SendMsg(&ID_message, DIOCOREPROTOCOL_HEADER_OPERATION_KEYEXCHANGE, DIOCOREPROTOCOL_KEYEXCHANGE_SERVER_OPERATION_PARAM, &publickey);
                                                                                  }                                                                               
                                                                                break;
                
              case DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_AUTHENTICATION        : if(IsServer())
                                                                                  {   
                                                                                    XUUID ID_message;
                                                                                 
                                                                                    SendMsg(&ID_message, DIOCOREPROTOCOL_HEADER_OPERATION_AUTHENTICATE, DIOCOREPROTOCOL_AUTHENTICATION_CHALLENGE_OPERATION_PARAM, GetAuthenticationChallenge());
                                                                                  }
                                                                                break;

              case DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_REGISTRATION          : if(!IsServer())
                                                                                  {   
                                                                                    XUUID                 ID_message;      
                                                                                    XFILEJSON             classcontent;                                                                                                                                                   
                                                                                    XSERIALIZATIONMETHOD* serializationmethod = XSERIALIZABLE::CreateInstance(classcontent);
                                                                                    if(!serializationmethod)
                                                                                      {
                                                                                        break;
                                                                                      }                                                                
  
                                                                                    if(!GetRegisterData())
                                                                                      {
                                                                                        break;
                                                                                      } 

                                                                                    GetRegisterData()->InitializeData(IsServer());

                                                                                    GetRegisterData()->SetSerializationMethod(serializationmethod);
                                                                                    GetRegisterData()->Serialize(); 

                                                                                    if(serializationmethod)
                                                                                      {
                                                                                        GEN_DELETE serializationmethod;
                                                                                      }
                
                                                                                    SendMsg(&ID_message, DIOCOREPROTOCOL_HEADER_OPERATION_REGISTERDATA, DIOCOREPROTOCOL_REGISTRATIONDATA_SEND_OPERATION_PARAM, &classcontent);
                                                                                  }                                                                              
                                                                                break;  

              case DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_WAITREADY             : Status_Set(DIOCOREPROTOCOL_CONNECTION_STATUS_WAITREADY);

                                                                                if(!IsServer())
                                                                                  {   
                                                                                    XUUID   ID_message;      
                                                                                    XSTRING content;                                                                                                                                                   

                                                                                    SendMsg(&ID_message, DIOCOREPROTOCOL_HEADER_OPERATION_TOREADYSERVER, DIOCOREPROTOCOL_WAITREADY_READY_OPERATION_PARAM, &content);                                                                                          
                                                                                  }                                                                                  
                                                                                break; 
                          
              case DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_READY                 : messages.DeleteAll();

                                                                                Status_Set(DIOCOREPROTOCOL_CONNECTION_STATUS_READY); 
                                                                                break; 

              case DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_INSTABILITY           : Status_Set(DIOCOREPROTOCOL_CONNECTION_STATUS_INSTABILITY); 
                                                                                break;

              case DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_DISCONNECTED          : Status_Set(DIOCOREPROTOCOL_CONNECTION_STATUS_DISCONNECTED);  

                                                                                messages.DeleteAll(); 

                                                                                RemoteDisconnect();  

                                                                                if(GetCoreProtocol())
                                                                                  {
                                                                                    DIOSTREAM* diostream =  GetCoreProtocol()->GetDIOStream();  
                                                                                    if(diostream)  
                                                                                      { 
                                                                                        if(diostream->GetStatus() != DIOSTREAMSTATUS_DISCONNECTED)
                                                                                          {
                                                                                            diostream->Disconnect();                                                                                                                                                                                  
                                                                                          }
                                                                                      }                                                                                
                                                                                  }
                                                                                break;         
            }
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOCOREPROTOCOL_CONNECTION::GetHeartBetsCounter()
* @brief      Get heart bets counter
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOCOREPROTOCOL_CONNECTION::GetHeartBetsCounter()
{
  return heartbetscounter;
}

    
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCOREPROTOCOL_CONNECTION::SetHeartBetsCounter(XDWORD heartbetscounter)
* @brief      Set heart bets counter
* @ingroup    DATAIO
* 
* @param[in]  heartbetscounter : Heartbetscounter value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCOREPROTOCOL_CONNECTION::SetHeartBetsCounter(XDWORD heartbetscounter)
{
  this->heartbetscounter = heartbetscounter;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOCOREPROTOCOL_REGISTERDATA* DIOCOREPROTOCOL_CONNECTION::GetRegisterData()
* @brief      Get register data
* @ingroup    DATAIO
* 
* @return     DIOCOREPROTOCOL_REGISTERDATA* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOCOREPROTOCOL_REGISTERDATA* DIOCOREPROTOCOL_CONNECTION::GetRegisterData()
{
  return registerdata;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCOREPROTOCOL_CONNECTION::SetRegisterData(DIOCOREPROTOCOL_REGISTERDATA* registerdata)
* @brief      Set register data
* @ingroup    DATAIO
* 
* @param[in]  registerdata : Registerdata pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCOREPROTOCOL_CONNECTION::SetRegisterData(DIOCOREPROTOCOL_REGISTERDATA* registerdata)
{
  this->registerdata = registerdata;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTION::CompletedInitialUpdateClasses()
* @brief      completed initial update classes
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTION::CompletedInitialUpdateClasses()
{
  return completedinitialupdateclasses;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCOREPROTOCOL_CONNECTION::SetCompletedInitialUpdateClasses(bool completedinitialupdateclasses)
* @brief      set completed initial update classes
* @ingroup    DATAIO
* 
* @param[in]  completedinitialupdateclasses : Completedinitialupdateclasses value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCOREPROTOCOL_CONNECTION::SetCompletedInitialUpdateClasses(bool completedinitialupdateclasses)
{
  this->completedinitialupdateclasses = completedinitialupdateclasses; 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTION::Disconnect()
* @brief      disconnect
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTION::Disconnect()
{  
  if(!GetCoreProtocol())
    {
      return false;
    }

  if(!GetCoreProtocol()->GetDIOStream())
    {
      return false;
    }

  return GetCoreProtocol()->GetDIOStream()->Disconnect();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTION::RemoteDisconnect()
* @brief      remote disconnect
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTION::RemoteDisconnect()
{
  XUUID  ID_message;                                        
  bool   status      = false;
  
  status = Command_Do(&ID_message, DIOCOREPROTOCOL_COMMAND_TYPE_DISCONNECT);   
                     
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XMUTEX* DIOCOREPROTOCOL_CONNECTION::GetOperationMutex()
* @brief      get operation mutex
* @ingroup    DATAIO
* 
* @return     XMUTEX* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XMUTEX* DIOCOREPROTOCOL_CONNECTION::GetOperationMutex()
{
  return operation_mutex;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XMUTEX* DIOCOREPROTOCOL_CONNECTION::GetInsideCommandMutex()
* @brief      get inside command mutex
* @ingroup    DATAIO
* 
* @return     XMUTEX* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XMUTEX* DIOCOREPROTOCOL_CONNECTION::GetInsideCommandMutex()
{
  return insidecommand_mutex;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTION::Mutex_Activate(XMUTEX* mutex, bool activate)
* @brief      mutex  activate
* @ingroup    DATAIO
* 
* @param[in]  mutex : Mutex pointer to use.
* @param[in]  activate : Activate value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTION::Mutex_Activate(XMUTEX* mutex, bool activate)
{
  if(!mutex)
    {
      return false;
    }

  if(!protocol)
    {
      return false;
    }

  if(!protocol->GetProtocolCFG())
    {
      return false;
    }

  /*
  if(!protocol->GetProtocolCFG()->BusMode_IsActive())
    {
      return true;
    }
  */

  bool status = false;

  if(activate)
    {
      status = mutex->Lock();
    }
   else
    {
      status = mutex->UnLock();
    }
    
  return true;  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTION::Mutex_IsActive(XMUTEX* mutex)
* @brief      mutex  is active
* @ingroup    DATAIO
* 
* @param[in]  mutex : Mutex pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTION::Mutex_IsActive(XMUTEX* mutex)
{
   if(!mutex)
    {
      return false;
    }

  if(!protocol)
    {
      return false;
    }

  if(!protocol->GetProtocolCFG())
    {
      return false;
    }

  /*
  if(!protocol->GetProtocolCFG()->BusMode_IsActive())
    {
      return true;
    }
  */

  return mutex->IsLock();  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTION::CreateIDConnection(XUUID& ID)
* @brief      Create ID connection
* @ingroup    DATAIO
* 
* @param[in]  ID : Identifier to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTION::CreateIDConnection(XUUID& ID)
{ 
  XSTRING     timestr; 
  XSTRING     origin; 
  XBUFFER     originbuffer; 
  HASHSHA2*   sha2        = NULL;
  XBUFFER*    result      = NULL;
  XDATETIME*  datetime    = NULL;

  datetime = GEN_XFACTORY.CreateDateTime();
  if(datetime)
    {
      datetime->Read();
      datetime->GetDateTimeToString(XDATETIME_FORMAT_STANDARD | XDATETIME_FORMAT_TIMEWITHMILLISECONDS, timestr);        

      GEN_XFACTORY.DeleteDateTime(datetime);
    }
  
  ID.Empty();
  
  origin.AddFormat(__L(" %s"), this);
  origin.AddFormat(__L(" %s"), timestr.Get());

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
* @fn         bool DIOCOREPROTOCOL_CONNECTION::SendMsg(XUUID* ID_message, DIOCOREPROTOCOL_HEADER_OPERATION operation, XCHAR* operation_param)
* @brief      Send msg
* @ingroup    DATAIO
* 
* @param[in]  ID_message : ID message pointer to use.
* @param[in]  operation : Operation value.
* @param[in]  operation_param : Operation param pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTION::SendMsg(XUUID* ID_message, DIOCOREPROTOCOL_HEADER_OPERATION operation, XCHAR* operation_param)
{
  DIOCOREPROTOCOL_HEADER* header          = NULL;
  XBUFFER                 contentresult;
  bool                    IDmsgempty      = false;
  bool                    status          = false;

  if(!protocol)
    {
      return false;  
    }  
   
  if(ID_message)
    {    
      if(ID_message->IsEmpty())
        {
          IDmsgempty = true;
        }
    }
   else
    {
      return false;
    }

  header = protocol->CreateHeader(ID_message, operation, operation_param);
  if(!header)
    {      
      return false;
    }

  XDWORD sendsize = protocol->SendMsg(header, contentresult);
  if(sendsize)
    {
      DIOCOREPROTOCOL_MESSAGE* message = GEN_NEW DIOCOREPROTOCOL_MESSAGE();
      if(message)
        {
          message->SetAcquisitionType(DIOCOREPROTOCOL_MESSAGE_TYPE_ACQUISITION_WRITE);
          message->GetHeader()->CopyFrom(header);
          message->GetContent()->CopyFrom(contentresult);  
          message->SetSizeAllMessage(sendsize);

          DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT xevent(this, DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_WRITEMSG);
          xevent.SetConnection(this);
          xevent.SetActualStatus(Status_Get());                      
          xevent.SetNextStatus(DIOCOREPROTOCOL_CONNECTION_STATUS_NONE);
          xevent.SetMsg(message);

          PostEvent(&xevent);

          if(IDmsgempty)
            {  
              message->SetIsConsumed(true);             
              status = Messages_GetAll()->AddRequest(message);
            }
           else
            {
              status = Messages_GetAll()->AddResponse(message);
            }       

          if(!status)
            {
              GEN_DELETE message;
            }  
        }                                                                                                                                                       
    }

  GEN_DELETE header;
           
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTION::SendMsg(XUUID* ID_message, DIOCOREPROTOCOL_HEADER_OPERATION operation, XCHAR* operation_param, XBUFFER* content)
* @brief      Send msg
* @ingroup    DATAIO
* 
* @param[in]  ID_message : ID message pointer to use.
* @param[in]  operation : Operation value.
* @param[in]  operation_param : Operation param pointer to use.
* @param[in]  content : Content pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTION::SendMsg(XUUID* ID_message, DIOCOREPROTOCOL_HEADER_OPERATION operation, XCHAR* operation_param, XBUFFER* content)
{
  DIOCOREPROTOCOL_HEADER* header          = NULL;
  XBUFFER                 contentresult;
  bool                    IDmsgempty      = false;
  bool                    status          = false;

  if(!protocol)
    {
      return false;  
    }  
   
  if(ID_message)
    {    
      if(ID_message->IsEmpty())
        {
          IDmsgempty = true;
        }
    }
   else
    {
      return false;
    }

  header = protocol->CreateHeader(ID_message, operation, operation_param, content, &contentresult);
  if(!header)
    {      
      return false;
    }

  XDWORD sendsize = protocol->SendMsg(header, contentresult);
  if(sendsize)
    {
      DIOCOREPROTOCOL_MESSAGE* message = GEN_NEW DIOCOREPROTOCOL_MESSAGE();
      if(message)
        {
          message->SetAcquisitionType(DIOCOREPROTOCOL_MESSAGE_TYPE_ACQUISITION_WRITE);
          message->GetHeader()->CopyFrom(header);
          message->GetContent()->CopyFrom(contentresult);  
          message->SetSizeAllMessage(sendsize);

          DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT xevent(this, DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_WRITEMSG);
          xevent.SetConnection(this);
          xevent.SetActualStatus(Status_Get());                      
          xevent.SetNextStatus(DIOCOREPROTOCOL_CONNECTION_STATUS_NONE);
          xevent.SetMsg(message);

          PostEvent(&xevent);

          if(IDmsgempty)
            {
              message->SetIsConsumed(true);
              status = Messages_GetAll()->AddRequest(message);
            }
           else
            {
              status = Messages_GetAll()->AddResponse(message);
            }       

          if(!status)           
            {
              GEN_DELETE message;
            }  
        }                                                                                                                                                       
    }

  GEN_DELETE header;
           
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTION::SendMsg(XUUID* ID_message, DIOCOREPROTOCOL_HEADER_OPERATION operation, XCHAR* operation_param, XSTRING* content)
* @brief      Send msg
* @ingroup    DATAIO
* 
* @param[in]  ID_message : ID message pointer to use.
* @param[in]  operation : Operation value.
* @param[in]  operation_param : Operation param pointer to use.
* @param[in]  content : Content pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTION::SendMsg(XUUID* ID_message, DIOCOREPROTOCOL_HEADER_OPERATION operation, XCHAR* operation_param, XSTRING* content)
{
  DIOCOREPROTOCOL_HEADER* header          = NULL;
  XBUFFER                 contentresult;
  bool                    IDmsgempty      = false;
  bool                    status          = false;

  if(!protocol)
    {
      return false;  
    } 
 
  if(ID_message)
    {    
      if(ID_message->IsEmpty())
        {
          IDmsgempty = true;
        }
    }
   else
    {
      return false;
    }
  
  header = protocol->CreateHeader(ID_message, operation, operation_param, content, &contentresult);
  if(!header)
    {      
      return false;
    }
  
  XDWORD sendsize = protocol->SendMsg(header, contentresult);
  if(sendsize)
    {
      DIOCOREPROTOCOL_MESSAGE* message = GEN_NEW DIOCOREPROTOCOL_MESSAGE();
      if(message)
        {
          message->SetAcquisitionType(DIOCOREPROTOCOL_MESSAGE_TYPE_ACQUISITION_WRITE);
          message->GetHeader()->CopyFrom(header);
          message->GetContent()->CopyFrom(contentresult);  
          message->SetSizeAllMessage(sendsize); 

          DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT xevent(this, DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_WRITEMSG);
          xevent.SetConnection(this);
          xevent.SetActualStatus(Status_Get());                      
          xevent.SetNextStatus(DIOCOREPROTOCOL_CONNECTION_STATUS_NONE);
          xevent.SetMsg(message);

          PostEvent(&xevent);

          if(IDmsgempty)
            { 
              message->SetIsConsumed(true);             
              status = Messages_GetAll()->AddRequest(message);
            }
           else
            {
              status = Messages_GetAll()->AddResponse(message);
            }       

          if(!status)                       
            {
              GEN_DELETE message;
            }  
        }                                                                                                                                                 
    }

  GEN_DELETE header;
   
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTION::SendMsg(XUUID* ID_message, DIOCOREPROTOCOL_HEADER_OPERATION operation, XCHAR* operation_param, XFILEJSON* content)
* @brief      send msg
* @ingroup    DATAIO
* 
* @param[in]  ID_message : ID message pointer to use.
* @param[in]  operation : Operation value.
* @param[in]  operation_param : Operation param pointer to use.
* @param[in]  content : Content pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTION::SendMsg(XUUID* ID_message, DIOCOREPROTOCOL_HEADER_OPERATION operation, XCHAR* operation_param, XFILEJSON* content)
{
  DIOCOREPROTOCOL_HEADER* header          = NULL;  
  XBUFFER                 contentresult;  
  bool                    IDmsgempty      = false;
  bool                    status          = false;

  if(!protocol)
    {
      return false;  
    } 

  if(ID_message)
    {    
      if(ID_message->IsEmpty())
        {
          IDmsgempty = true;
        }
    }
   else
    {
      return false;
    }  

  header = protocol->CreateHeader(ID_message, operation, operation_param, content, &contentresult);
  if(!header)
    {      
      return false;
    }

  XDWORD sendsize = protocol->SendMsg(header, contentresult);
  if(sendsize)
    {     
      DIOCOREPROTOCOL_MESSAGE* message = GEN_NEW DIOCOREPROTOCOL_MESSAGE();
      if(message)
        {
          message->SetAcquisitionType(DIOCOREPROTOCOL_MESSAGE_TYPE_ACQUISITION_WRITE);
          message->GetHeader()->CopyFrom(header);
          message->GetContent()->CopyFrom(contentresult);  
          message->SetSizeAllMessage(sendsize);
           
          DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT xevent(this, DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_WRITEMSG);
          xevent.SetConnection(this);
          xevent.SetActualStatus(Status_Get());                      
          xevent.SetNextStatus(DIOCOREPROTOCOL_CONNECTION_STATUS_NONE);
          xevent.SetMsg(message);

          PostEvent(&xevent);

          if(ID_message)
            {    
              if(ID_message->IsEmpty())
                {
                  IDmsgempty = true;
                }
            }
          
          if(IDmsgempty)
            {
              message->SetIsConsumed(true);
              status = Messages_GetAll()->AddRequest(message);
            }
           else
            {
              status = Messages_GetAll()->AddResponse(message);
            }       

          if(!status)          
            {
              GEN_DELETE message;
            }  
        }                                                                                                                                                 
    }

  GEN_DELETE header;
               
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTION::GetMsg(bool isrequest, DIOCOREPROTOCOL_HEADER_OPERATION operation, XCHAR* operation_param, DIOCOREPROTOCOL_HEADER& header, XBUFFER& content)
* @brief      get msg
* @ingroup    DATAIO
* 
* @param[in]  isrequest : Isrequest value.
* @param[in]  operation : Operation value.
* @param[in]  operation_param : Operation param pointer to use.
* @param[in]  header : Header value.
* @param[in]  content : Content value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTION::GetMsg(bool isrequest, DIOCOREPROTOCOL_HEADER_OPERATION operation, XCHAR* operation_param, DIOCOREPROTOCOL_HEADER& header, XBUFFER& content)
{
  int index = NOTFOUND;

  if(!protocol)
    {
      return false;  
    }   
 
  if(isrequest)
    {
      index = Messages_GetAll()->FindRequest(operation, operation_param);                                                                        
    }
   else
    {
      index = Messages_GetAll()->FindResponse(operation, operation_param);                                                                        
    }
 
  if(index != NOTFOUND)
    { 
      DIOCOREPROTOCOL_MESSAGE* message = NULL;

      if(isrequest)
        {
          message = Messages_GetAll()->GetAll()->GetKey(index); 
        }
       else 
        {
          message = Messages_GetAll()->GetAll()->GetElement(index); 
        }

      if(message)
        {
          header.CopyFrom(message->GetHeader());
          content.Add(message->GetContent());
            
          return true;
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTION::GetMsg(bool isrequest, DIOCOREPROTOCOL_HEADER_OPERATION operation, XCHAR* operation_param, DIOCOREPROTOCOL_HEADER& header, XSTRING& content)
* @brief      get msg
* @ingroup    DATAIO
* 
* @param[in]  isrequest : Isrequest value.
* @param[in]  operation : Operation value.
* @param[in]  operation_param : Operation param pointer to use.
* @param[in]  header : Header value.
* @param[in]  content : Content value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTION::GetMsg(bool isrequest, DIOCOREPROTOCOL_HEADER_OPERATION operation, XCHAR* operation_param, DIOCOREPROTOCOL_HEADER& header, XSTRING& content)
{
  int index = NOTFOUND;

  if(!protocol)
    {
      return false;  
    }   
  
  if(isrequest)
    {
      index = Messages_GetAll()->FindRequest(operation, operation_param);                                                                        
    }
   else
    {
      index = Messages_GetAll()->FindResponse(operation, operation_param);                                                                        
    }
 
  if(index != NOTFOUND)
    { 
      DIOCOREPROTOCOL_MESSAGE* message = NULL;

      if(isrequest)
        {
          message = Messages_GetAll()->GetAll()->GetKey(index); 
        }
       else 
        {
          message = Messages_GetAll()->GetAll()->GetElement(index); 
        }

      if(message)
        {
          header.CopyFrom(message->GetHeader());

          content.ConvertFromUTF8((*message->GetContent()));
            
          return true;
        }
    }

  return false;
}

 
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOCOREPROTOCOL_CONNECTION::GetMsg(bool isrequest, DIOCOREPROTOCOL_HEADER_OPERATION operation, XCHAR* operation_param, DIOCOREPROTOCOL_HEADER& header, XFILEJSON& content)
* @brief      get msg
* @ingroup    DATAIO
* 
* @param[in]  isrequest : Isrequest value.
* @param[in]  operation : Operation value.
* @param[in]  operation_param : Operation param pointer to use.
* @param[in]  header : Header value.
* @param[in]  content : Content value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOCOREPROTOCOL_CONNECTION::GetMsg(bool isrequest, DIOCOREPROTOCOL_HEADER_OPERATION operation, XCHAR* operation_param, DIOCOREPROTOCOL_HEADER& header, XFILEJSON& content)
{
  int index = NOTFOUND;

  if(!protocol)
    {
      return false;  
    }   
 
  if(isrequest)
    {
      index = Messages_GetAll()->FindRequest(operation, operation_param);                                                                        
    }
   else
    {
      index = Messages_GetAll()->FindResponse(operation, operation_param);                                                                        
    }
 
  if(index != NOTFOUND)
    { 
      DIOCOREPROTOCOL_MESSAGE* message = NULL;

      if(isrequest)
        {
          message = Messages_GetAll()->GetAll()->GetKey(index); 
        }
       else 
        {
          message = Messages_GetAll()->GetAll()->GetElement(index); 
        }

      if(message)
        {
          header.CopyFrom(message->GetHeader());

          content.AddBufferLines(XFILETXTFORMATCHAR_UTF8, (*message->GetContent()));
          content.DecodeAllLines();
            
          return true;
        }
    }
 
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOCOREPROTOCOL_CONNECTION::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOCOREPROTOCOL_CONNECTION::Clean()
{
  protocol                        = NULL;

  status                          = DIOCOREPROTOCOL_CONNECTION_STATUS_NONE; 
  
  operation_mutex                 = NULL;
  insidecommand_mutex             = NULL;  

  xtimerstatus                    = NULL;
  xtimerwithoutconnexion          = NULL;
  xtimeroutresponse               = NULL;

  heartbetscounter                = 0;

  registerdata                    = NULL;

  completedinitialupdateclasses   = false;

  isreceivedcommand               = false;
}



