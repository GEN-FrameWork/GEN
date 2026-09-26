/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       APPFlowExtended_InternetStatus.h
* 
* @class      APPFLOWEXTENDED_INTERNETSTATUS
* @brief      Application Flow Extended Internet Status class
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
class APPFLOWCFG;
class APPFLOWINTERNETSERVICES;

class APPFLOWEXTENDED_INTERNETSTATUS : public DIOWEBSERVER_RESOLVEENDPOINT
{
  public:
                                        APPFLOWEXTENDED_INTERNETSTATUS              (APPFLOWCFG* cfg);
    virtual                            ~APPFLOWEXTENDED_INTERNETSTATUS              ();

    APPFLOWINTERNETSERVICES*            GetInternetServices                         ();

    XSTRING*                            GetLocalIP                                  ();

    bool                                GetHaveInternetConnection                   ();
    XSTRING*                            GetPublicIP                                 ();    

    XDWORD                              GetLatency                                   ();
    void                                SetLatency                                   (XDWORD latency);
   
    bool                                Update                                      (); 
    bool                                Show                                        (XCONSOLE* console);

    bool                                Serialize                                   ();    
    bool                                Deserialize                                 ();
                                                               
  private:

    void                                Clean                                      ();

    APPFLOWCFG*                         cfg;
    APPFLOWINTERNETSERVICES*            internetservices;

    XSTRING                             localIP;

    bool                                haveinternetconnection;
    XSTRING                             publicIP;    

    XDWORD                              latency;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





