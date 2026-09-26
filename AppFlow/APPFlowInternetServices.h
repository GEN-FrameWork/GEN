/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       APPFlowInternetServices.h
* 
* @class      APPFLOWINTERNETSERVICES
* @brief      Application Flow Internet Connection Services class
* @ingroup    APPFLOW
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

#include "XString.h"
#include "XObserver.h"
#include "XSubject.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum APPFLOWINTERNETSERVICES_TASKID
{
  APPFLOWINTERNETSERVICES_TASKID_UNKNOWN                      = 0 ,
  APPFLOWINTERNETSERVICES_TASKID_CHECKCONNECTIONINTERNET          ,
  APPFLOWINTERNETSERVICES_TASKID_GETIPS                           ,  
  APPFLOWINTERNETSERVICES_TASKID_CHECKNTPDATETIME                 ,
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XSCHEDULER;
class XSCHEDULER_XEVENT;
class XDATETIME;
class XMUTEX;
class DIOCHECKTCPIPCONNECTIONS;
class DIOCHECKINTERNETCONNECTION;
class DIOSCRAPERWEBPUBLICIP;
class DIONTP;
class DIODYNDNS_MANAGER;
class APPFLOWCFG;

class APPFLOWINTERNETSERVICES : public XOBSERVER, public XSUBJECT
{
  public:
                                    APPFLOWINTERNETSERVICES         ();
    virtual                        ~APPFLOWINTERNETSERVICES         ();

    bool                            Ini                             (APPFLOWCFG* cfg, XDWORD timeoutgetpublicip = 0);

    bool                            HaveInternetConnection          ();

    XDWORD                           GetInternetLatency               ();

    XSTRING*                        GetAutomaticLocalIP             ();
    XSTRING*                        GetAllLocalIP                   ();
    XSTRING*                        GetPublicIP                     ();

    bool                            ChangeCadenceCheck              (APPFLOWINTERNETSERVICES_TASKID taskID, int timecadenceseconds, bool startimmediatelycycles = false);
    
    XDATETIME*                      DateTime_GetLocal               (bool active_daylightsave = true, bool active_meridian = true);
    XDATETIME*                      DateTime_GetUTC                 ();
    int                             DateTime_GetMeridian            ();
    
    DIOCHECKTCPIPCONNECTIONS*       GetCheckTCPIPConnections        ();
    DIOCHECKINTERNETCONNECTION*     GetCheckInternetConnection      ();
    DIODYNDNS_MANAGER*              GetDynDNSManager                ();

    XSCHEDULER*                     GetXScheduler                   ();

    bool                            IsActivedExit                   (); 
    bool                            SetActivedExit                  (bool actived); 

    bool                            End                             ();

  private:

    bool                            CheckInternetConnection         ();
    bool                            CheckInternetStatus             ();
    
    bool                            UpdateIPs                       (XSTRING& actualpublicIP);
    #ifdef APPFLOW_CFG_DYNDNSMANAGER_ACTIVE
    bool                            UpdateDynDNSURLs                (XSTRING& actualpublicIP);
    #endif
    bool                            AdjustTimerByNTP                (XVECTOR<XSTRING*>* servers);

    void                            HandleEvent_Scheduler           (XSCHEDULER_XEVENT* xevent);
    void                            HandleEvent                     (XEVENT* xevent);

    void                            Clean                           ();

    APPFLOWCFG*                     cfg;

    XSCHEDULER*                     xscheduler;

    XSTRING                         automaticlocalIP;
    XSTRING                         alllocalIP;
    XDWORD                          nchangesIP;
    XDWORD                          nchangeslocalIP;
    XDWORD                          nchangespublicIP;
    
    DIOCHECKINTERNETCONNECTION*     checkinternetconnection;

    bool                            haveinternetconnection;

    bool                            endservices;

    XSTRING                         publicIP;

    XVECTOR<XSTRING*>               NTPservers;

    XMUTEX*                         xmutex_datetime_local;
    XDATETIME*                      xdatetime_local;

    XMUTEX*                         xmutex_datetime_UTC;
    XDATETIME*                      xdatetime_UTC;

    DIODYNDNS_MANAGER*              dyndnsmanager;
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





