/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       APPFlowCheckResourcesHardware_XEvent.h
* 
* @class      APPFLOWCHECKRESOURCESHARDWARE_XEVENT
* @brief      Application Flow Check Resources Hardware eXtended Event class
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

enum APPFLOWCHECKRESOURCESHARDWARE_XEVENT_TYPE
{
  APPFLOWCHECKRESOURCESHARDWARE_XEVENT_TYPE_UNKNOWN              = XEVENT_TYPE_APPFLOWCHECKRESOURCESHW      ,
  APPFLOWCHECKRESOURCESHARDWARE_XEVENT_TYPE_MEMFREELIMIT                                                    ,
  APPFLOWCHECKRESOURCESHARDWARE_XEVENT_TYPE_TOTALCPUUSAGELIMIT                                              ,
  APPFLOWCHECKRESOURCESHARDWARE_XEVENT_TYPE_APPCPUUSAGELIMIT                                       
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XTIMER;
class XPUBLISHER;

class APPFLOWCHECKRESOURCESHARDWARE_XEVENT : public XEVENT
{
  public:
                         APPFLOWCHECKRESOURCESHARDWARE_XEVENT    (XSUBJECT* subject, XDWORD type = APPFLOWCHECKRESOURCESHARDWARE_XEVENT_TYPE_UNKNOWN, XDWORD family = XEVENT_TYPE_APPFLOWCHECKRESOURCESHW);
    virtual             ~APPFLOWCHECKRESOURCESHARDWARE_XEVENT    ();
   
    bool                 GetActualMemFree                        (XDWORD& memfree_inbytes, XBYTE& memfree_percent);
    void                 SetActualMemFree                        (XDWORD memfree_inbytes, XBYTE memfree_percent);
   
    int                  GetActualTotalCPUUsage                  ();
    void                 SetActualTotalCPUUsage                  (int& CPUusage);
   
    int                  GetActualAppCPUUsage                    ();
    void                 SetActualAppCPUUsage                    (int& CPUusage);
   
  private:   
   
    void                 Clean                                   ();
   
    XDWORD               memfree_inbytes;
    XBYTE                memfree_percent;
   
    int                  totalCPUusage;    
    int                  appCPUusage;    
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




