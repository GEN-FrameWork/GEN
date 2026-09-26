/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       APPFlowExtended_ApplicationStatus.h
* 
* @class      APPFLOWEXTENDED_APPLICATIONSTATUS
* @brief      Application Flow Extended Application Status class
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

#include "DIOWebServer_ResolveEndPoint.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XCONSOLE;
class XTIMER;
class APPFLOWCFG;
class APPFLOWCHECKRESOURCESHARDWARE;

class APPFLOWEXTENDED_APPLICATIONSTATUS : public DIOWEBSERVER_RESOLVEENDPOINT
{
  public:
                                        APPFLOWEXTENDED_APPLICATIONSTATUS           (APPFLOWCFG* cfg);
    virtual                            ~APPFLOWEXTENDED_APPLICATIONSTATUS           ();

    APPFLOWCHECKRESOURCESHARDWARE*      GetCheckResourcesHardware                   ();

    XSTRING*                            GetOSVersion                                ();
    XSTRING*                            GetAppVersion                               ();

    XDWORD                              GetMemoryTotal                              ();
    void                                SetMemoryTotal                              (XDWORD memory_total);

    XDWORD                              GetMemoryFree                               ();
    void                                SetMemoryFree                               (XDWORD memory_free);

    XDWORD                              GetMemoryFreePercent                        ();
    void                                SetMemoryFreePercent                        (XDWORD memory_freepercent);

    XSTRING*                            GetAverange                                 ();    

    XSTRING*                            GetCurrentDate                              ();
    XSTRING*                            GetOperatingTime                            ();

    bool                                Update                                      (); 
    bool                                Show                                        (XCONSOLE* console);

    bool                                Serialize                                   ();    
    bool                                Deserialize                                 ();
                                                               
  private:
   
    void                                Clean                                       ();

    APPFLOWCFG*                         cfg;
    XTIMER*                             xtimeroperatingtime;
    APPFLOWCHECKRESOURCESHARDWARE*      checkresourceshardware;

    
    XSTRING                             osversion;
    XSTRING                             appversion;
     
    XDWORD                              memory_total;
    XDWORD                              memory_free;
    XDWORD                              memory_freepercent;

    XSTRING                             averange;

    XSTRING                             currentdate;
    XSTRING                             operatingtime;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





