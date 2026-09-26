/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOCheckInternetConnection.h
* 
* @class      DIOCHECKINTERNETCONNECTION
* @brief      Data Input/Output Check Connection Internet class
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

#include "DIOCheckTCPIPConnections.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOCHECKINTERNETCONNECTION_MAXCONNECTIONS        16

#define DIOCHECKINTERNETCONNECTION_DEFAULTTIMECHECK      30

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XPUBLISHER;
class DIOFACTORY;
class DIOCHECKTCPIPCONNECTIONS;

class DIOCHECKINTERNETCONNECTION
{
  public:
                                            DIOCHECKINTERNETCONNECTION             (int timeconnectionchecks = DIOCHECKINTERNETCONNECTION_DEFAULTTIMECHECK, bool waitforcheck = true);
    virtual                                ~DIOCHECKINTERNETCONNECTION             ();

    DIOCHECKTCPIPCONNECTIONS*               GetCheckConnections                    ();

    bool                                    Check                                  ();

    bool                                    AddConnectionCut                       (DIOCHECKTCPIPCONNECTION_CUT& connectioncut);
    bool                                    DeleteAllConnectionCuts                ();
    XVECTOR<DIOCHECKTCPIPCONNECTION_CUT*>*  GetConnectionCuts                      ();
    DIOCHECKTCPIPCONNECTION_CUT*            GetLastConnectionsCut                  ();

    bool                                    IsChangeConnectionStatus               ();

    XDWORD                                  GetLatency                             ();

  private:

    void                                    Clean                                 ();

    DIOCHECKTCPIPCONNECTIONS*               checkconnections;

    XDWORD                                  connectionID[DIOCHECKINTERNETCONNECTION_MAXCONNECTIONS];

    bool                                    ischangeconnectionstatus;

    XDWORD                                  latency;

    XMUTEX*                                 xmutexconnectioncuts;
    XVECTOR<DIOCHECKTCPIPCONNECTION_CUT*>   connectioncuts;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




