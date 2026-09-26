/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       APPFlowInternetServices_XEvent.h
* 
* @class      APPFLOWINTERNETSERVICES_XEVENT
* @brief      Application Flow Internet Services eXtended Event class
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

#include "XEvent.h"
#include "XSubject.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum APPFLOWINTERNETSERVICES_XEVENT_TYPE
{
  APPFLOWINTERNETSERVICES_XEVENT_TYPE_UNKNOWN                           = XEVENT_TYPE_APPFLOWINTERNETSERVICES     ,
  APPFLOWINTERNETSERVICES_XEVENT_TYPE_CHECKINTERNETCONNEXION                                                      ,
  APPFLOWINTERNETSERVICES_XEVENT_TYPE_CHANGEIP                                                                    , 
  APPFLOWINTERNETSERVICES_XEVENT_TYPE_ADJUSTDATETIME                                                    
};

enum APPFLOWINTERNETSERVICES_CHECKINTERNETCONNEXION_STATE
{ 
  APPFLOWINTERNETSERVICES_CHECKINTERNETCONNEXION_STATE_NONE              = 0                                      ,
  APPFLOWINTERNETSERVICES_CHECKINTERNETCONNEXION_STATE_CHECKING                                                   ,
  APPFLOWINTERNETSERVICES_CHECKINTERNETCONNEXION_STATE_CHECKED                                                    ,  
  APPFLOWINTERNETSERVICES_CHECKINTERNETCONNEXION_STATE_CUT                                                        ,      
  APPFLOWINTERNETSERVICES_CHECKINTERNETCONNEXION_STATE_RESTORE                                                    ,        
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XTIMER;
class XPUBLISHER;
class DIOCHECKTCPIPCONNECTION_CUT;

class APPFLOWINTERNETSERVICES_XEVENT : public XEVENT
{
  public:
                                                            APPFLOWINTERNETSERVICES_XEVENT         (XSUBJECT* subject, XDWORD type = APPFLOWINTERNETSERVICES_XEVENT_TYPE_UNKNOWN, XDWORD family = XEVENT_TYPE_APPFLOWINTERNETSERVICES);
    virtual                                                ~APPFLOWINTERNETSERVICES_XEVENT         ();

    APPFLOWINTERNETSERVICES_CHECKINTERNETCONNEXION_STATE    GetInternetConnexionState              ();
    void                                                    SetInternetConnexionState              (APPFLOWINTERNETSERVICES_CHECKINTERNETCONNEXION_STATE internetconnexionstate);

    DIOCHECKTCPIPCONNECTION_CUT*                            GetInternetConnextionCut               ();
    void                                                    SetInternetConnextionCut               (DIOCHECKTCPIPCONNECTION_CUT* internetconnectioncut);

    XDWORD                                                  GetLatency                             ();
    void                                                    SetLatency                             (XDWORD latency);

    bool                                                    IsChangePublicIP                       ();     
    void                                                    SetIsChangePublicIP                    (bool ischangepublicIP);     

    bool                                                    IsChangeLocalIP                        ();     
    void                                                    SetIsChangeLocalIP                     (bool ischangelocalIP);     

    XSTRING*                                                GetChangePublicIP                      ();     
    XSTRING*                                                GetChangeLocalIP                       (); 

    XDWORD                                                  GetNChangesIP                          ();
    void                                                    SetNChangesIP                          (XDWORD nchangesIP);
    
    XDWORD                                                  GetNChangesLocalIP                     ();
    void                                                    SetNChangesLocalIP                     (XDWORD nchangeslocalIP);
    
    XDWORD                                                  GetNChangesPublicIP                    ();
    void                                                    SetNChangesPublicIP                    (XDWORD nchangeslocalIP);         

    
  private:

    void                                                    Clean                                  ();  

    APPFLOWINTERNETSERVICES_CHECKINTERNETCONNEXION_STATE    internetconnexionstate;
    DIOCHECKTCPIPCONNECTION_CUT*                            internetconnectioncut;

    XDWORD                                                  latency;

    bool                                                    ischangepublicIP;
    bool                                                    ischangelocalIP;
    
    XSTRING                                                 changepublicIP;     
    XSTRING                                                 changelocalIP;  

    XDWORD                                                  nchangesIP;
    XDWORD                                                  nchangeslocalIP;
    XDWORD                                                  nchangespublicIP;   
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




