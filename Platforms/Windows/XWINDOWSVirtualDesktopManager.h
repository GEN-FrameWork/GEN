/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XWINDOWSVirtualDesktopManager.h
* 
* @class      XWINDOWSVIRTUALDESKTOPMANAGER
* @brief      WINDOWS eXtended Utils Management Instrumentation Interface class
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

#include <Windows.h>

#include "XVector.h"
#include "XString.h"


/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define GEN_XWINDOWSVIRTUALDESKTOPMANAGER    XWINDOWSVIRTUALDESKTOPMANAGER::GetInstance()

typedef bool(*XWINDOWSVIRTUALDESKTOPMANAGER_HOOK_FUNC)(void);

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XWINDOWSVIRTUALDESKTOP
{
  public:
                                              XWINDOWSVIRTUALDESKTOP            ();
    virtual                                  ~XWINDOWSVIRTUALDESKTOP            ();

    XSTRING*                                  GetName                           ();

  private:

    void                                      Clean                             ();

    XSTRING                                   name;
};

class XWINDOWSVIRTUALDESKTOPMANAGER
{
  public:

    static bool                               GetIsInstanced                    ();
    static XWINDOWSVIRTUALDESKTOPMANAGER&     GetInstance                       ();
    static bool                               DelInstance                       ();

    XVECTOR<XWINDOWSVIRTUALDESKTOP*>*         EnumAllDesktops                   ();
    bool                                      AddDesktopToList                  (XSTRING& name);

    bool                                      CreateNewDesktop                  (XCHAR* name, bool addexplorer);
    bool                                      CreateNewDesktop                  (XSTRING& name, bool addexplorer);

    bool                                      SwitchDesktop                     (XCHAR* name, bool inherit);
    bool                                      SwitchDesktop                     (XSTRING& name, bool inherit);

    bool                                      IsCurrentDesktop                  (XCHAR* desktopname);
    bool                                      IsCurrentDesktop                  (XSTRING& desktopname);

    bool                                      IsDesktopCreated                  (XCHAR* desktopname);
    bool                                      IsDesktopCreated                  (XSTRING& desktopname);

    bool                                      SwitchProcessSpecifiedDesktop     (XCHAR* desktopname);
    bool                                      SwitchProcessSpecifiedDesktop     (XSTRING& desktopname);

    bool                                      IsUserInteractive                 ();

  private:
                                              XWINDOWSVIRTUALDESKTOPMANAGER     ();
                                              XWINDOWSVIRTUALDESKTOPMANAGER     (XWINDOWSVIRTUALDESKTOPMANAGER const&);
    virtual                                  ~XWINDOWSVIRTUALDESKTOPMANAGER     ();

    void                                      operator =                        (XWINDOWSVIRTUALDESKTOPMANAGER const&);

    bool                                      LaunchApplication                 (XCHAR* applicationfilepath, XCHAR* desktopname);
    static BOOL __stdcall                     EnumDesktopProc                   (LPTSTR desktop, LPARAM param);

    void                                      Clean                             ();

    static XWINDOWSVIRTUALDESKTOPMANAGER*     instance;

    DESKTOPENUMPROC                           enumdesktopfunc;
    XVECTOR<XWINDOWSVIRTUALDESKTOP*>          listdesktops;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





