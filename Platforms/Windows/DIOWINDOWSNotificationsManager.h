/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOWINDOWSNotificationsManager.h
* 
* @class      DIOWINDOWSNOTIFICATIONSMANAGER
* @brief      WINDOWS Data Input/Output Notifications manager class
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

//#undef XMEMORY_CONTROL_ACTIVE

#include <windows.h>
#include <string>

#include <wintoastlib.h>

#include "XString.h"

#include "DIONotificationsManager.h"


using namespace WinToastLib;

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOWINDOWSNOTIFICATIONSMANAGER_HANDLER : public IWinToastHandler 
{
  public:
            
    void                                      toastActivated                          () const;     
    void                                      toastActivated                          (int actionindex) const;    
    void                                      toastActivated                          (const char* response) const;
    void                                      toastDismissed                          (WinToastDismissalReason state) const;
    void                                      toastFailed                             () const;
};


class DIOWINDOWSNOTIFICATIONSMANAGER : public DIONOTIFICATIONSMANAGER
{
  public:
                                              DIOWINDOWSNOTIFICATIONSMANAGER          ();
    virtual                                  ~DIOWINDOWSNOTIFICATIONSMANAGER          ();

    bool                                      Ini                                     (XCHAR* ownertitle, XCHAR* appname) override;
    bool                                      Do                                      (DIONOTIFICATION* notification) override;
    bool                                      End                                     () override;

  private:
    
    void                                      Clean                                   ();  

    DIOWINDOWSNOTIFICATIONSMANAGER_HANDLER*   handler; 
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





