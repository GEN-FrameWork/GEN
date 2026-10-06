/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XWINDOWSServiceBase.h
* 
* @class      XWINDOWSSERVICEBASE
* @brief      WINDOWS eXtended Utils Service Base class
* @ingroup    PLATFORM_WINDOWS
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

#include <windows.h>

#include "XString.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define XWINDOWSSERVICEBASE_LOG_SECTIONID_SERVICE  _L("Service")

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XWINDOWSSERVICEBASE
{
  public:
                                        XWINDOWSSERVICEBASE         (XCHAR* servicename, bool can_stop = true, bool can_shutdown = true, bool can_pause = true, bool can_powerevent = true, bool can_sessionchange = true);                                    
    virtual                            ~XWINDOWSSERVICEBASE         ();

    void                                Setup                       (bool can_stop, bool can_shutdown, bool can_pause, bool can_powerevent, bool can_sessionchange);

    static bool                         Run                         (XWINDOWSSERVICEBASE& service);
    void                                Stop                        ();

    static bool                         Install                     (XCHAR* servicename, XCHAR* displayname, XDWORD starttype, XCHAR* dependencies, XCHAR* account, XCHAR* password);
    static bool                         Uninstall                   (XCHAR* servicename);

    static bool                         Start                       (XCHAR* servicename);        
    static bool                         Stopped                     (XCHAR* servicename);

    static LPQUERY_SERVICE_CONFIG       GetConfig                   (XCHAR* servicename);    
    static bool                         SetConfig                   (XCHAR* servicename, XDWORD type = SERVICE_NO_CHANGE, XDWORD starttype = SERVICE_NO_CHANGE, XDWORD errorcontrol = SERVICE_NO_CHANGE);

    static LPSERVICE_DESCRIPTION        GetDescription              (XCHAR* servicename);
    static bool                         SetDescription              (XCHAR* servicename, LPSERVICE_DESCRIPTION description);

    static LPSERVICE_FAILURE_ACTIONS    GetFailureActions           (XCHAR* servicename);
    static bool                         SetFailureActions           (XCHAR* servicename, LPSERVICE_FAILURE_ACTIONS failureactions);

    XCHAR*                              GetServiceName              ();

  protected:

    virtual void                        On_Start                    (DWORD argc, XCHAR** args);
    virtual void                        On_Stop                     ();
    virtual void                        On_Pause                    ();
    virtual void                        On_Continue                 ();
    virtual void                        On_Shutdown                 ();
    virtual void                        On_PowerEvent               (DWORD eventtype);
    virtual void                        On_SessionChange            (DWORD eventtype);

    void                                SetServiceStatus            (XDWORD currentstate, XDWORD win32exitcode = NO_ERROR, XDWORD waithint = 0);

    void                                WriteEventLogEntry          (XCHAR* message, XWORD type);
    void                                WriteErrorLogEntry          (XCHAR* function, XDWORD error = GetLastError());

  private:

    static bool                         GetServiceConfig            (XCHAR* servicename);
    static bool                         GetServiceDescription       (XCHAR* servicename);
    static bool                         GetServiceFaiilureActions   (XCHAR* servicename);

    static void WINAPI                  ServiceMain                 (DWORD argc, LPWSTR* argv);
    static void WINAPI                  ServiceCtrlHandler          (DWORD control, DWORD eventtype, LPVOID eventdata, LPVOID context);

    void                                Start                       (XDWORD argc, XCHAR** args);
    void                                Pause                       ();
    void                                Continue                    ();
    void                                Shutdown                    ();
    void                                PowerEvent                  (DWORD eventtype);
    void                                SessionChange               (DWORD eventtype);

    void                                Clean                       ();

    static XWINDOWSSERVICEBASE*         service;
    XCHAR*                              servicename;    
    SERVICE_STATUS                      status;
    SERVICE_STATUS_HANDLE               statushandle;

    static LPQUERY_SERVICE_CONFIG       service_config; 
    static LPSERVICE_DESCRIPTION        service_description;
    static LPSERVICE_FAILURE_ACTIONS    service_failureactions;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





