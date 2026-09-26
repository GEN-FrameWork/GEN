/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOCoreProtocol_ConnectionsManager.h
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XUUID.h"
#include "XObserver.h"
#include "XSubject.h"
#include "XThreadCollected.h"

#include "DIOStream_XEvent.h"
#include "DIOCoreProtocol_CFG.h"
#include "DIOCoreProtocol.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XTIMER;
class XMUTEX;
class XBUFFER;
class XSTRING;
class XFILEJSON;
class XSERIALIZABLE;
class DIOCOREPROTOCOL;
class DIOCOREPROTOCOL_CONNECTION;
class DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT;
class DIOCOREPROTOCOL_MESSAGE;

class DIOCOREPROTOCOL_CONNECTIONSMANAGER : public XOBSERVER, public XSUBJECT
{
  public:
                                              DIOCOREPROTOCOL_CONNECTIONSMANAGER             ();
    virtual                                  ~DIOCOREPROTOCOL_CONNECTIONSMANAGER             ();   

    bool                                      Ini                                            ();
    bool                                      End                                            ();

    DIOCOREPROTOCOL_CFG*                      GetProtocolCFG                                 ();

    XMAP<DIOSTREAMCONFIG*, DIOSTREAM*>*       DIOStream_GetAll                               ();
    bool                                      DIOStream_Add                                  (DIOSTREAMCONFIG* diostreamCFG, DIOSTREAM* diostream);
    bool                                      DIOStream_Delete                               (DIOSTREAMCONFIG* diostreamCFG);
    bool                                      DIOStream_DeleteAll                            ();

    virtual DIOCOREPROTOCOL_CONNECTION*       CreateConnection                               ();

    virtual DIOCOREPROTOCOL*                  CreateProtocol                                 (DIOCOREPROTOCOL_CONNECTION* connection, DIOSTREAM* diostream);    
 
    XMUTEX*                                   UpdateClass_GetXMutex                          ();  
    static bool                               UpdateClass_Serialize                          (DIOCOREPROTOCOL_MESSAGE* message, XSERIALIZABLE* classcontent);
    static bool                               UpdateClass_Deserialize                        (DIOCOREPROTOCOL_MESSAGE* message, XSERIALIZABLE* classcontent);
                              
    XVECTOR<DIOCOREPROTOCOL_CONNECTION*>*     Connections_GetAll                             ();
    XMUTEX*                                   Connections_GetXMutex                          ();  
    DIOCOREPROTOCOL_CONNECTION*               Connections_Add                                (DIOSTREAM* stream);
  
    DIOCOREPROTOCOL_CONNECTION*               Connections_Get                                (DIOSTREAM* stream);
    DIOCOREPROTOCOL_CONNECTION*               Connections_Get                                (XDWORD index);
    bool                                      Connections_Delete                             (DIOCOREPROTOCOL_CONNECTION* connection);    
    bool                                      Connections_DeleteAllDisconnected              ();
    bool                                      Connections_DeleteAll                          (); 

    static bool                               CreateIDMachine                                (XUUID& ID);
    
  protected:
    
    DIOCOREPROTOCOL_CFG                       protocolCFG;    
    XMAP<DIOSTREAMCONFIG*, DIOSTREAM*>        diostreams;

  private:

    bool                                      Connections_ReadMessages                       (); 

    bool                                      Received_AllCommandMessages                    (DIOCOREPROTOCOL_CONNECTION* connection, DIOCOREPROTOCOL_MESSAGE* message); 
    bool                                      Received_AdditionalCommandMessages             (DIOCOREPROTOCOL_CONNECTION* connection, DIOCOREPROTOCOL_MESSAGE* message);

    bool                                      Received_AllUpdateClassMessages                (DIOCOREPROTOCOL_CONNECTION* connection, DIOCOREPROTOCOL_MESSAGE* message);
    bool                                      Received_AdditionalUpdateClassMessages         (DIOCOREPROTOCOL_CONNECTION* connection, DIOCOREPROTOCOL_MESSAGE* message);

    bool                                      Received_AllAskUpdateClassMessages             (DIOCOREPROTOCOL_CONNECTION* connection, DIOCOREPROTOCOL_MESSAGE* message);
    bool                                      Received_AdditionalAskUpdateClassMessages      (DIOCOREPROTOCOL_CONNECTION* connection, DIOCOREPROTOCOL_MESSAGE* message);

    bool                                      GenerateResponseUpdateClass                    (XFILEJSON& xfileJSON, bool statusresponse);

    bool                                      ManagerUpdateClasses                           ();
    bool                                      ManagerHeartBet                                ();  
           
    virtual void                              HandleEvent_CoreProtocolConnectionsManager     (DIOCOREPROTOCOL_CONNECTIONSMANAGER_XEVENT* event);
    void                                      HandleEvent_DIOStream                          (DIOSTREAM_XEVENT* event);
    void                                      HandleEvent                                    (XEVENT* xevent);

    static void                               ThreadConnections                              (void* param);
    static void                               ThreadUpdateConnection                         (void* param);
    static void                               ThreadAutomaticOperations                      (void* param);

    void                                      Clean                                          ();
    
    
    XMUTEX*                                   updateclass_xmutex;
    XMUTEX*                                   connections_delete_xmutex;
    XTHREADCOLLECTED*                         connections_xthread;
    XTHREADCOLLECTED*                         connection_update_xthread;
    XTHREADCOLLECTED*                         automaticoperations_xthread;
   
    XVECTOR<DIOCOREPROTOCOL_CONNECTION*>      connections;   

    bool                                      leavingactive; 
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/







