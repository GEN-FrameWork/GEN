/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOCheckTCPIPConnections.h
* 
* @class      DIOCHECKTCPIPCONNECTIONS
* @brief      Data Input/Output Check connections TPC/IP class
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

#include "XFactory.h"
#include "XDateTime.h"
#include "XRand.h"
#include "XTimer.h"
#include "XVector.h"
#include "XString.h"
#include "XThreadCollected.h"

#include "HashCRC32.h"

#include "DIOFactory.h"
#include "DIOURL.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOCHECKTCPIPCONNECTIONS_DEFAULTTIMECONNECTIONCHECKS              10   // seconds

#define DIOCHECKTCPIPCONNECTIONS_DEFAULTNCHECKSFOREVERYCONNECTION          5
#define DIOCHECKTCPIPCONNECTIONS_DEFAULTTIMERCONNECTIONCHECK             100   // milliseconds

#define DIOCHECKTCPIPCONNECTIONS_MAXNCONNECTIONCUTS                      250

#define DIOCHECKTCPIPCONNECTIONS_INFINITECUT                             -1

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XFACTORY;
class XPUBLISHER;
class XRAND;
class XBUFFER;
class DIOFACTORY;
class DIOPING;


class DIOCHECKTCPIPCONNECTION_CUT
{
  public:
                                            DIOCHECKTCPIPCONNECTION_CUT           ();
    virtual                                ~DIOCHECKTCPIPCONNECTION_CUT           ();

    XDATETIME*                              GetStartDateTime                      ();

    int                                     GetMeasureNSeconds                    ();
    void                                    SetMeasureNSeconds                    (XDWORD nseconds);

    bool                                    GetMeasureTimeString                  (XSTRING& measuretime, bool large = false);

    bool                                    CopyFrom                              (DIOCHECKTCPIPCONNECTION_CUT* connectioncut);
    bool                                    CopyTo                                (DIOCHECKTCPIPCONNECTION_CUT* connectioncut);

  private:

    void                                    Clean                                 ();

    XDATETIME*                              startdatetime;
    int                                     nseconds;
};

class DIOCHECKTCPIPCONNECTION
{
  public:
                                            DIOCHECKTCPIPCONNECTION               ();
    virtual                                ~DIOCHECKTCPIPCONNECTION               ();

    XDWORD                                  GetID                                 ();
    DIOURL*                                 GetURL                                ();

    bool                                    IsConnected                           ();
    void                                    SetIsConnected                        (bool isconnected);

    XDWORD                                  GetNChecks                            ();
    void                                    SetNChecks                            (XDWORD nchecks);
    XDWORD                                  IncNChecks                            ();

    bool                                    Set                                   (XCHAR* url);

    XDWORD                                  GetElapsedTime                        ();
    void                                    SetElapsedTime                        (XDWORD elapsedtime);

    bool                                    ResetTimeConnexionStatus              ();
    XQWORD                                  GetTimeConnexionStatus                ();

  private:

    XDWORD                                  CreateID                              ();
    void                                    Clean                                 ();

    XDWORD                                  ID;
    DIOURL*                                 url;
    bool                                    isconnected;
    XDWORD                                  nchecks;
    XDWORD                                  elapsedtime;
    XTIMER*                                 timerconnexion;
};

class DIOCHECKTCPIPCONNECTIONS
{
  public:
                                            DIOCHECKTCPIPCONNECTIONS               ();
    virtual                                ~DIOCHECKTCPIPCONNECTIONS               ();

    bool                                    Ini                                    (int timerconnectionscheck = DIOCHECKTCPIPCONNECTIONS_DEFAULTTIMECONNECTIONCHECKS, bool validsomeisconnected = false, bool dispersionmode = false);

    bool                                    Run                                    (bool activate = true);

    int                                     GetTimeConnectionChecks                ();

    bool                                    IsCheckTimeConnections                 ();
    void                                    SetIsCheckTimeConnections              (bool ischecktimeconnections);

    void                                    Setup                                  (int timeconnectionchecks = DIOCHECKTCPIPCONNECTIONS_DEFAULTTIMECONNECTIONCHECKS, bool validsomeisconnected = false, bool dispersionmode = false);

    bool                                    Connection_Add                         (XCHAR* url, XDWORD& ID);
    bool                                    Connection_Add                         (XSTRING& url, XDWORD& ID);
    bool                                    Connection_Add                         (DIOURL& url, XDWORD& ID);

    DIOCHECKTCPIPCONNECTION*                Connection_GetByID                     (XDWORD ID);
    bool                                    Connection_IsActive                    (XDWORD ID, bool& isactive);
    bool                                    Connection_DeleteByID                  (XDWORD ID);

    XMUTEX*                                 Connections_GetMutex                   ();
    bool                                    Connections_IsAllChecked               ();
    bool                                    Connections_IsSomeChecked              ();
    bool                                    Connections_WaitToAllChecked           (int timeout);
    bool                                    Connections_WaitToSomeIsChecked        (int timeout);
    bool                                    Connections_SomeIsConnected            ();
    XDWORD                                  Connections_GetLatency                  ();

    bool                                    Connections_DeleteAll                  ();

    bool                                    IsActivedExit                          (); 
    bool                                    SetActivedExit                         (bool actived); 

    bool                                    End                                    ();


  private:


    static void                             ThreadCheckConnections                 (void* param);

    void                                    Clean                                  ();

    int                                     timeconnectionchecks;
    bool                                    validsomeisconnected;
    bool                                    dispersionmode;

    XTIMER*                                 xtimerfortimeconnectionchecks;
    bool                                    ischecktimeconnections;
    
    XMUTEX*                                 xmutexconnections;
    XVECTOR<DIOCHECKTCPIPCONNECTION*>       connections;
    XTHREADCOLLECTED*                       threadcheckconnections;

    bool                                    activedexit;  
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





