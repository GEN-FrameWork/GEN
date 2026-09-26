/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOProtocol_ConnectionsManager.h
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XVector.h"
#include "XEvent.h"
#include "XObserver.h"
#include "XSubject.h"
#include "XThreadCollected.h"

#include "DIOURL.h"
#include "DIOProtocol.h"

#include "DIOProtocol_ConnectionsManager_XEvent.h"


/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOPROTOCOL_CONNECTIONS_DEFAULTIMETRYCONNECTIONS         8    // Seconds
#define DIOPROTOCOL_CONNECTIONS_DEFAULTIMECHECKCONNECTIONS      60    // Seconds
#define DIOPROTOCOL_CONNECTIONS_UNLIMITEDNCONNECTIONS           -1

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XTIMER;
class CIPHER;
class DIOSTREAMCONFIG;
class DIOSTREAMTCPIPCONFIG;
class DIOSTREAM;
class DIOPROTOCOL;
class DIOPROTOCOL_CONNECTION;
class DIOPROTOCOL_CONNECTIONSMANAGER;
class DIOSTREAMENUMSERVERS;

class DIOPROTOCOL_CONNECTION
{
  public:
                                        DIOPROTOCOL_CONNECTION                        (DIOSTREAMCONFIG* diostreamcfg);
                                       ~DIOPROTOCOL_CONNECTION                        ();

    DIOSTREAMCONFIG*                    GetDIOStreamConfig                            ();
    DIOPROTOCOL*                        GetDIOProtocol                                ();
    void                                SetDIOProtocol                                (DIOPROTOCOL* protocol);

    bool                                Connect                                       ();
    bool                                Disconected                                   ();

    bool                                SetInUse                                      (bool inuse, XDWORD ID);
    bool                                IsInUse                                       ();

    bool                                IsSendEventConnected                          ();
    void                                SetIsSendEventConnected                       (bool issendeventconnected);

  protected:

    DIOSTREAMCONFIG*                    diostreamcfg;
    DIOPROTOCOL*                        dioprotocol;
    bool                                issendeventconnected;
    XMUTEX*                             xmutexinuseID;
    XVECTOR<XDWORD>                     inuseID;

  private:

    void                                Clean                                          ();

};

class DIOPROTOCOL_CONNECTIONSMANAGER : public XSUBJECT
{
  public:
                                        DIOPROTOCOL_CONNECTIONSMANAGER                 ();
                                       ~DIOPROTOCOL_CONNECTIONSMANAGER                 ();

    bool                                Ini                                            (bool isserver, DIOSTREAMCONFIG* diostreamcfg, DIOSTREAMENUMSERVERS* diostreamenumservers = NULL);
    bool                                End                                            ();

    virtual DIOPROTOCOL*                CreateProtocol                                 ()                      = 0;
    virtual bool                        DeleteProtocol                                 (DIOPROTOCOL* protocol) = 0;

    int                                 TargetURL_GetNTargets                          ();
    bool                                TargetURL_Add                                  (XCHAR* URL);
    bool                                TargetURL_Add                                  (XSTRING& URL);
    bool                                TargetURL_Add                                  (DIOURL& URL);
    XSTRING*                            TargetURL_Get                                  (int index);
    bool                                TargetURL_Delete                               (int index);
    bool                                TargetURL_DeleteAll                            ();

    int                                 ProtocolConnections_GetNLimit                  ();
    void                                ProtocolConnections_SetNLimit                  (int protocolconnectionsnlimit);
    int                                 ProtocolConnections_GetNAvailable              ();
    DIOPROTOCOL_CONNECTION*             ProtocolConnections_Get                        (int index);
    DIOPROTOCOL_CONNECTION*             ProtocolConnections_GetFirstConnected          ();
    DIOPROTOCOL_CONNECTION*             ProtocolConnections_GetFirstOperative          ();
    virtual DIOPROTOCOL_CONNECTION*     ProtocolConnections_GetByDIOStream             (DIOSTREAM* diostream);
    DIOPROTOCOL*                        ProtocolConnections_GetProtocol                (int index = 0);
    int                                 ProtocolConnections_GetNConnected              ();
    int                                 ProtocolConnections_GetNFreeToConnect          ();
    bool                                ProtocolConnections_SendEventConnected         ();
    bool                                ProtocolConnections_DeleteDisconnected         ();

    bool                                ProtocolConnections_DeleteAllWaitConnections   ();
    bool                                ProtocolConnections_Disconnect                 (int index);
    bool                                ProtocolConnections_DisconnectAll              ();
    bool                                ProtocolConnections_Delete                     (int index);
    bool                                ProtocolConnections_DeleteAll                  ();

    bool                                WaitToAnyConnectionIsConnected                 (int timeout);
    bool                                WaitToAllConnectionsCanBeDeleted               (int timeout = 0);

    bool                                SendEvent                                      (DIOPROTOCOL_CONNECTIONSMANAGER_XEVENT_TYPE type, DIOPROTOCOL_CONNECTION* protocolconnection);

    void                                Application_GetVersion                          (XDWORD& version, XDWORD& subversion, XDWORD& subversionerr);
    void                                Application_SetVersion                          (XDWORD version, XDWORD subversion, XDWORD subversionerr);

    XSTRING*                            Application_GetName                             ();

  protected:

    DIOSTREAMENUMSERVERS*               diostreamenumservers;
    DIOSTREAMCONFIG*                    diostreamcfg;

    XWORD                               applicationversion;
    XWORD                               applicationsubversion;
    XWORD                               applicationsubversionerr;

    XSTRING                             applicationname;

    bool                                isserver;
    int                                 protocolconnectionsnlimit;
    XVECTOR<DIOPROTOCOL_CONNECTION*>    protocolconnections;


  private:

    void                                ManageProtocolConnectionsServer               ();
    void                                ManageProtocolConnectionsClient               ();

    static void                         ThreadProtocolConnections                     (void* param);

    void                                Clean                                         ();

    XTHREADCOLLECTED*                   xthreadconnections;
    XTIMER*                             xtimerconnections;
    XTIMER*                             xtimerclienttry;
    XTIMER*                             xtimerout;

    XMUTEX*                             xmutexprotocolconnections;

    XVECTOR<DIOURL*>                    targetURLs;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





