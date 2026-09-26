/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       APPFlowExtended.h
* 
* @class      APPFLOWEXTENDED
* @brief      Application Flow Extended class
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

#include "XPath.h"
#include "XString.h"
#include "XLog.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define APPFLOW_EXTENDED                APPFLOWEXTENDED::GetInstance()

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XCONSOLE;
class XTHREAD;
class APPFLOWCFG;
class APPFLOWCONSOLE;
class APPFLOWEXTENDED_APPLICATIONSTATUS;
class APPFLOWEXTENDED_INTERNETSTATUS;

class APPFLOWEXTENDED
{
  public:

    static bool                         GetIsInstanced                  ();
    static APPFLOWEXTENDED&             GetInstance                     ();
    static bool                         DelInstance                     ();

    bool                                APPStart                        (APPFLOWCFG* cfg, APPFLOWCONSOLE* console = NULL);
    bool                                APPEnd                          ();
    
    APPFLOWCFG*                         GetCFG                          ();
    APPFLOWCONSOLE*                     GetConsole                      ();

    #ifdef APPFLOW_EXTENDED_APPLICATIONSTATUS_ACTIVE
    APPFLOWEXTENDED_APPLICATIONSTATUS*  GetApplicationStatus            (); 
    #endif

    #ifdef APPFLOW_EXTENDED_INTERNETSTATUS_ACTIVE
    APPFLOWEXTENDED_INTERNETSTATUS*     GetInternetStatus               (); 
    #endif

    bool                                ShowAll                         ();
         
  private:
                                        APPFLOWEXTENDED                 ();
                                        APPFLOWEXTENDED                 (APPFLOWEXTENDED const&);
    virtual                            ~APPFLOWEXTENDED                 ();

    void                                operator =                      (APPFLOWEXTENDED const&);

    static void                         ThreadFunction_Update           (void* param);

    void                                Clean                           ();

    static APPFLOWEXTENDED*             instance;

    APPFLOWCFG*                         appcfg;
    APPFLOWCONSOLE*                     appconsole;
    
    XMUTEX*                             updatemutex;
    XTHREAD*                            updatethread;  
    bool                                exitincurse;
    
    #ifdef APPFLOW_EXTENDED_APPLICATIONSTATUS_ACTIVE
    APPFLOWEXTENDED_APPLICATIONSTATUS*  applicationstatus;
    #endif

    #ifdef APPFLOW_EXTENDED_INTERNETSTATUS_ACTIVE
    APPFLOWEXTENDED_INTERNETSTATUS*     internetstatus;
    #endif
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





