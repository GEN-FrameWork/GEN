/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOProtocol_ConnectionsManager_XEvent.h
* 
* @class      DIOPROTOCOL_CONNECTIONSMANAGER_XEVENT
* @brief      Data Input/Output Protocol Connections Manager eXtended Event class
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

#include "XEvent.h"


/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum DIOPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE
{
  DIOPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_UNKNOWN                      = XEVENT_TYPE_PROTOCOLCONNECTIONS ,
  DIOPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_CONNECTEDCONNECTION                                           ,
  DIOPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_INITPROTOCOL                                                  ,
  DIOPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_DISCONNECTEDCONNECTION                                        ,
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOSTREAMENUMSERVERS;
class DIOSTREAMCONFIG;
class DIOPROTOCOL;
class DIOPROTOCOL_CONNECTION;
class DIOPROTOCOL_CONNECTIONSMANAGER;

class DIOPROTOCOL_CONNECTIONSMANAGER_XEVENT : public XEVENT
{
  public:
                                        DIOPROTOCOL_CONNECTIONSMANAGER_XEVENT         (XSUBJECT* subject, XDWORD type = DIOPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE_UNKNOWN, XDWORD family = XEVENT_TYPE_PROTOCOLCONNECTIONS);
    virtual                            ~DIOPROTOCOL_CONNECTIONSMANAGER_XEVENT         ();

    DIOSTREAMENUMSERVERS*               GetDIOStreamEnumServers                       ();
    void                                SetDIOStreamEnumServers                       (DIOSTREAMENUMSERVERS* diostreamenumservers);

    DIOSTREAMCONFIG*                    GetDIOStreamConfig                            ();
    void                                SetDIOStreamConfig                            (DIOSTREAMCONFIG* diostreamcfg);

    DIOPROTOCOL*                        GetDIOProtocol                                ();
    void                                SetDIOProtocol                                (DIOPROTOCOL* dioprotocol);

    DIOPROTOCOL_CONNECTION*             GetProtocolConnection                         ();
    void                                SetProtocolConnection                         (DIOPROTOCOL_CONNECTION* protocolconnection);

    DIOPROTOCOL_CONNECTIONSMANAGER*     GetProtocolConnectionsManager                 ();
    void                                SetProtocolConnectionsManager                 (DIOPROTOCOL_CONNECTIONSMANAGER* protocolconnectionsmanager);

    bool                                IsInitialized                                 ();
    void                                SetIsInitialized                              (bool isinitialized);

  private:

    void                                Clean                                         ();

    DIOSTREAMENUMSERVERS*               diostreamenumservers;
    DIOSTREAMCONFIG*                    diostreamcfg;
    DIOPROTOCOL*                        dioprotocol;
    DIOPROTOCOL_CONNECTION*             protocolconnection;
    DIOPROTOCOL_CONNECTIONSMANAGER*     protocolconnectionsmanager;
    bool                                isinitialized;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/






