/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOCoreProtocol_Connection.h
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XFSMachine.h"

#include "CipherKeySymmetrical.h"
#include "CipherAES.h"
#include "CipherECDSAX25519.h"

#include "DIOCoreProtocol_Messages.h"
#include "DIOCoreProtocol_RegisterData.h"




/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum DIOCOREPROTOCOL_CONNECTION_XFSMEVENTS
{
  DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_NONE                 = 0 ,
  DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_CONNECTED                ,
  DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_KEYEXCHANGE              ,
  DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_AUTHENTICATION           ,
  DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_REGISTRATION             , 
  DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_WAITREADY                , 
  DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_READY                    ,
  DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_INSTABILITY              ,
  DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_DISCONNECTED             ,

  DIOCOREPROTOCOL_CONNECTION_LASTEVENT
};


enum DIOCOREPROTOCOL_CONNECTION_XFSMSTATES
{
  DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_NONE                 = 0 ,
  DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_CONNECTED                ,
  DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_KEYEXCHANGE              ,
  DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_AUTHENTICATION           , 
  DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_REGISTRATION             , 
  DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_WAITREADY                ,
  DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_READY                    , 
  DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_INSTABILITY              ,  
  DIOCOREPROTOCOL_CONNECTION_XFSMSTATE_DISCONNECTED             ,

  DIOCOREPROTOCOL_CONNECTION_LASTSTATE
};


enum DIOCOREPROTOCOL_CONNECTION_STATUS
{
  DIOCOREPROTOCOL_CONNECTION_STATUS_NONE                    = 0 , 
  DIOCOREPROTOCOL_CONNECTION_STATUS_CONNECTED                   , 
  DIOCOREPROTOCOL_CONNECTION_STATUS_KEYEXCHANGE                 , 
  DIOCOREPROTOCOL_CONNECTION_STATUS_AUTHENTICATED               ,   
  DIOCOREPROTOCOL_CONNECTION_STATUS_REGISTERED                  ,
  DIOCOREPROTOCOL_CONNECTION_STATUS_WAITREADY                   ,   
  DIOCOREPROTOCOL_CONNECTION_STATUS_READY                       ,
  DIOCOREPROTOCOL_CONNECTION_STATUS_INSTABILITY                 ,
  DIOCOREPROTOCOL_CONNECTION_STATUS_DISCONNECTED                ,
};


 #define TIMEOUT_CHAGE_STATE(protocol, xtimeroutresponse)                     if(protocol->GetProtocolCFG()->GetTimeOutNoResponse())                                    \
                                                                                {                                                                                       \
                                                                                  if(xtimeroutresponse)                                                                 \
                                                                                    {                                                                                   \
                                                                                      if(xtimeroutresponse->GetMeasureSeconds() >= protocol->GetProtocolCFG()->GetTimeOutNoResponse()) \
                                                                                        {                                                                               \
                                                                                          SetEvent(DIOCOREPROTOCOL_CONNECTION_XFSMEVENT_DISCONNECTED);                  \
                                                                                        }                                                                               \
                                                                                    }                                                                                   \
                                                                                }
  

#define COMMAND_DO_WITHOUTPARAMS(command_type, result, timeout)                {  XUUID ID_message;                                                                     \
                                                                                  bool  status  = false;                                                                \
                                                                                                                                                                        \
                                                                                  if(Mutex_Activate(operation_mutex, true))                                             \
                                                                                    {                                                                                   \
                                                                                      if(Command_Do(&ID_message, command_type))                                         \
                                                                                        {                                                                               \
                                                                                          status = GetResult(&ID_message, result, timeout);                             \
                                                                                        }                                                                               \
                                                                                                                                                                        \
                                                                                      Mutex_Activate(operation_mutex, false);                                           \
                                                                                    }                                                                                   \
                                                                                                                                                                        \
                                                                                  return status;                                                                        \
                                                                               }                                                                                         

 #define COMMAND_DO(command_type, params, result, timeout)                     {  XUUID ID_message;                                                                     \
                                                                                  bool  status  = false;                                                                \
                                                                                                                                                                        \
                                                                                  if(Mutex_Activate(operation_mutex, true))                                             \
                                                                                    {                                                                                   \
                                                                                      if(Command_Do(&ID_message, command_type, params))                                 \
                                                                                        {                                                                               \
                                                                                          status = GetResult(&ID_message, result, timeout);                             \
                                                                                        }                                                                               \
                                                                                                                                                                        \
                                                                                      Mutex_Activate(operation_mutex, false);                                           \
                                                                                    }                                                                                   \
                                                                                                                                                                        \
                                                                                 return status;                                                                         \
                                                                               }                                                                                         

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOCOREPROTOCOL_CFG;
class DIOCOREPROTOCOL;

class DIOCOREPROTOCOL_CONNECTION : public XFSMACHINE, public XSUBJECT
{
  public:
                                          DIOCOREPROTOCOL_CONNECTION            ();
    virtual                              ~DIOCOREPROTOCOL_CONNECTION            ();

    bool                                  InitFSMachine                         (); 
    
    bool                                  IsServer                              ();

    XUUID*                                GetIDConnection                       ();
    
    DIOCOREPROTOCOL*                      GetCoreProtocol                       ();
    bool                                  SetCoreProtocol                       (DIOCOREPROTOCOL* protocol);

    XBUFFER*                              GetAuthenticationChallenge            ();
    XBUFFER*                              GetAuthenticationResponse             ();

    CIPHERKEYSYMMETRICAL*                 GetCipherKey                          ();

    DIOCOREPROTOCOL_CONNECTION_STATUS     Status_Get                            ();  
    void                                  Status_Set                            (DIOCOREPROTOCOL_CONNECTION_STATUS status);
    bool                                  Status_GetString                      (DIOCOREPROTOCOL_CONNECTION_STATUS status, XSTRING& statusstring);
    bool                                  Status_GetString                      (XSTRING& statusstring); 

    XTIMER*                               GetXTimerStatus                       ();
    XTIMER*                               GetXTimerWithoutConnexion             ();
       
    DIOCOREPROTOCOL_MESSAGES*             Messages_GetAll                       ();

    bool                                  Command_Do                            (XDWORD command_type, XBUFFER& result, XDWORD timeout);
    bool                                  Command_Do                            (XDWORD command_type, XSTRING& result, XDWORD timeout);
    bool                                  Command_Do                            (XDWORD command_type, XFILEJSON& result, XDWORD timeout);

    bool                                  Command_Do                            (XDWORD command_type, XBUFFER* params, XBUFFER& result, XDWORD timeout);
    bool                                  Command_Do                            (XDWORD command_type, XSTRING* params, XBUFFER& result, XDWORD timeout);
    bool                                  Command_Do                            (XDWORD command_type, XFILEJSON* params, XBUFFER& result, XDWORD timeout);
    
    bool                                  Command_Do                            (XDWORD command_type, XBUFFER* params, XSTRING& result, XDWORD timeout);
    bool                                  Command_Do                            (XDWORD command_type, XSTRING* params, XSTRING& result, XDWORD timeout);
    bool                                  Command_Do                            (XDWORD command_type, XFILEJSON* params, XSTRING& result, XDWORD timeout);
    
    bool                                  Command_Do                            (XDWORD command_type, XBUFFER* params, XFILEJSON& result, XDWORD timeout);
    bool                                  Command_Do                            (XDWORD command_type, XSTRING* params, XFILEJSON& result, XDWORD timeout);
    bool                                  Command_Do                            (XDWORD command_type, XFILEJSON* params, XFILEJSON& result, XDWORD timeout);    

    bool                                  Command_Do                            (XUUID* ID_message, XDWORD command_type);
    bool                                  Command_Do                            (XUUID* ID_message, XDWORD command_type, XBUFFER* params);
    bool                                  Command_Do                            (XUUID* ID_message, XDWORD command_type, XSTRING* params);
    bool                                  Command_Do                            (XUUID* ID_message, XDWORD command_type, XFILEJSON* params);
    
    bool                                  UpdateClass_Do                        (XCHAR* classname, XSERIALIZABLE* classcontent, XDWORD timeout);
    bool                                  UpdateClass_DoAsk                     (XCHAR* classname, XSERIALIZABLE* classserializable, XDWORD timeout);    

    bool                                  UpdateClass_Do                        (XUUID* ID_message, XCHAR* classname, XFILEJSON* classcontent = NULL); 
    bool                                  UpdateClass_DoAsk                     (XUUID* ID_message, XCHAR* classname, XSTRING* classcontent = NULL);
 
    bool                                  GetResult                             (XUUID* ID_message, XBUFFER& result, XDWORD timeout);
    bool                                  GetResult                             (XUUID* ID_message, XSTRING& result, XDWORD timeout);
    bool                                  GetResult                             (XUUID* ID_message, XFILEJSON& result, XDWORD timeout); 

    bool                                  Update                                ();  

    XDWORD                                GetHeartBetsCounter                   ();
    void                                  SetHeartBetsCounter                   (XDWORD heartbetscounter = 0);

    DIOCOREPROTOCOL_REGISTERDATA*         GetRegisterData                       ();
    void                                  SetRegisterData                       (DIOCOREPROTOCOL_REGISTERDATA* registerdata);

    bool                                  CompletedInitialUpdateClasses         ();
    void                                  SetCompletedInitialUpdateClasses      (bool completedinitialupdateclasses);

    bool                                  Disconnect                            ();

    bool                                  RemoteDisconnect                      ();

    XMUTEX*                               GetOperationMutex                     ();  
    XMUTEX*                               GetInsideCommandMutex                 ();  

    bool                                  Mutex_Activate                        (XMUTEX* mutex, bool activate);
    bool                                  Mutex_IsActive                        (XMUTEX* mutex);
    
  private:
   
    bool                                  CreateIDConnection                    (XUUID& ID);

    bool                                  SendMsg                               (XUUID* ID_message, DIOCOREPROTOCOL_HEADER_OPERATION operation, XCHAR* operation_param);
    bool                                  SendMsg                               (XUUID* ID_message, DIOCOREPROTOCOL_HEADER_OPERATION operation, XCHAR* operation_param, XBUFFER* content);
    bool                                  SendMsg                               (XUUID* ID_message, DIOCOREPROTOCOL_HEADER_OPERATION operation, XCHAR* operation_param, XSTRING* content);
    bool                                  SendMsg                               (XUUID* ID_message, DIOCOREPROTOCOL_HEADER_OPERATION operation, XCHAR* operation_param, XFILEJSON* content);   

    bool                                  GetMsg                                (bool isrequest, DIOCOREPROTOCOL_HEADER_OPERATION operation, XCHAR* operation_param, DIOCOREPROTOCOL_HEADER& header, XBUFFER& content);
    bool                                  GetMsg                                (bool isrequest, DIOCOREPROTOCOL_HEADER_OPERATION operation, XCHAR* operation_param, DIOCOREPROTOCOL_HEADER& header, XSTRING& content);
    bool                                  GetMsg                                (bool isrequest, DIOCOREPROTOCOL_HEADER_OPERATION operation, XCHAR* operation_param, DIOCOREPROTOCOL_HEADER& header, XFILEJSON& content);

    void                                  Clean                                 ();    
    
    DIOCOREPROTOCOL*                      protocol;   

    XUUID                                 ID_connection;

    XBUFFER                               authentication_challenge;  
    XBUFFER                               authentication_response;
    CIPHERECDSAX25519                      ciphercurve;
    CIPHERKEYSYMMETRICAL                  cipherkey;     

    DIOCOREPROTOCOL_CONNECTION_STATUS     status;

    
    XMUTEX*                               operation_mutex;  
    XMUTEX*                               insidecommand_mutex;  
    
    XTIMER*                               xtimerstatus;  
    XTIMER*                               xtimerwithoutconnexion;  
    XTIMER*                               xtimeroutresponse; 

    XDWORD                                heartbetscounter;

    DIOCOREPROTOCOL_MESSAGES              messages; 
    DIOCOREPROTOCOL_REGISTERDATA*         registerdata;

    bool                                  completedinitialupdateclasses;

    bool                                  isreceivedcommand;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/







