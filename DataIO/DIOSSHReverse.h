/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOSSHReverse.h
* 
* @class      DIOSSHREVERSE
* @brief      Data Input/Output SSH reverse class
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
#include "XString.h"
#include "XSystem.h"
#include "XProcessManager.h"
#include "XFile.h"
#include "XFileINI.h"
#include "XSleep.h"
#include "XThread.h"
#include "XLog.h"
#include "XTrace.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOSSHREVERSE_DEFAULTAPPLICATION      _L("sshpass")
#define DIOSSHREVERSE_DEFAULTAPPLICATION2     _L("autossh")
#define DIOSSHREVERSE_CFGREMOTENAMEFILE       _L("sshrem.ini")
#define DIOSSHREVERSE_DEFAULTPORTSSH          22

#define DIOSSHREVERSE_LOGSECTIONID            _L("SSHreverse")

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOSSHREVERSE
{
  public:

    static bool                   GetIsInstanced        (); 
    static DIOSSHREVERSE&         GetInstance           ();
    
    static bool                   DelInstance           ();

    XSTRING*                      GetURLTarget          ();
    XSTRING*                      GetLocalIP            ();
    XSTRING*                      GetLogin              ();
    XSTRING*                      GetPassword           ();

    XDWORD                        GetPort               ();
    void                          SetPort               (XDWORD port);

    bool                          DownloadCFG           (XCHAR* URL, XSTRING& publicIP, XSTRING& localIP);
    bool                          DownloadCFG           (XSTRING& URL, XSTRING& publicIP, XSTRING& localIP);    
    bool                          DownloadCFG           (DIOURL& URL, XSTRING& publicIP, XSTRING& localIP);
                                  
    bool                          Activate              ();
    bool                          DeActivate            ();

    bool                          IsRunning             ();

    bool                          Exec                  (XCHAR* URL, XSTRING& publicIP, XSTRING& localIP);    
    bool                          Exec                  (XSTRING& URL, XSTRING& publicIP, XSTRING& localIP);    
    bool                          Exec                  (DIOURL& URL, XSTRING& publicIP, XSTRING& localIP);
                                  

  private:
                                  DIOSSHREVERSE         ();
                                  DIOSSHREVERSE         (DIOSSHREVERSE const&);
    virtual                      ~DIOSSHREVERSE         ();
                                  
    void                          operator =            (DIOSSHREVERSE const&);

    void                          Clean                 ();
    
    XSTRING                       URLtarget;
    XSTRING                       localIP;
    XSTRING                       login;
    XSTRING                       password;
    XDWORD                        port;

    static DIOSSHREVERSE*         instance;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





