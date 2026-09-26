/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       MainProcANDROID.h
* 
* @class      MAINPROCANDROID
* @brief      ANDROID Main Proc class
* @ingroup    PLATFORM_ANDROID
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

#include "XBase.h"
#include "XString.h"
#include "XPath.h"

#include "XANDROIDTrace.h"

#include "GRPANDROIDScreen.h"

#include "ANDROIDNativeActivity.h"

#ifdef APPFLOW_ACTIVE
#include "APPFlowBase.h"
#include "APPFlowMain.h"
#endif

#include "MainProc.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XPATHS;
class XPATH;
class XSTRING;
class XAPPLICATION;
class XANDROIDFACTORY;
class XANDROIDSYSTEM;
class GRPANDROIDFACTORY;
class INPMANAGER;
class INPANDROIDDEVICEKEYBOARD;
class INPANDROIDDEVICEMOUSE;
class INPDEVICE;

class MAINPROCANDROID :  public ANDROIDNATIVEACTIVITY_HANDLER, public ANDROIDNATIVEINPUT_HANDLER, public MAINPROC, public XSUBJECT
{
  public:
                                    MAINPROCANDROID                     ();
    virtual                        ~MAINPROCANDROID                     ();

    bool                            Ini                                 (XSTRING* apkpath, XSTRING* datapath, APPFLOWMAIN* appmain = NULL, APPFLOWBASE_MODE_TYPE applicationmode = APPFLOWBASE_MODE_TYPE_UNKNOWN);
    bool                            Update                              ();
    bool                            End                                 ();

    #ifdef INP_ACTIVE
    INPDEVICE*                      GetKeyboard                         ();
    INPDEVICE*                      GetTouchscreen                      ();
    #endif

    android_app*                    GetAndroidApplication               ();
    void                            SetAndroidApplication               (android_app* application);

  protected:

    bool                            OnTouchEvent                        (AInputEvent* event);
    bool                            OnKeyboardEvent                     (AInputEvent* event);
    bool                            OnTrackballEvent                    (AInputEvent* event);

    STATUS                          OnActivate                          ();
    void                            OnDeactivate                        ();
    STATUS                          OnStep                              ();

    void                            OnStart                             ();
    void                            OnResume                            ();
    void                            OnPause                             ();
    void                            OnStop                              ();
    void                            OnDestroy                           ();

    void                            OnSaveState                         (void** data, size_t* size);
    void                            OnConfigurationChanged              ();
    void                            OnLowMemory                         ();

    void                            OnCreateWindow                      ();
    void                            OnDestroyWindow                     ();
    void                            OnGainFocus                         ();
    void                            OnLostFocus                         ();

    bool                            GetPackageResourcePath              (struct android_app* app, XSTRING& path);
    bool                            GetAbsolutePath                     (struct android_app* app, XSTRING& path);
    bool                            GetDPI                              (struct android_app* app);

  private:

    bool                            Factorys_Ini                        ();
    bool                            Factorys_End                        ();

    bool                            OverturnAssetsToExternalLocation    (XPATH& origin, XPATH& target, XPATH& addtarget);

    bool                            AssetsDir_CreateAll                 (XPATH& origin);
    bool                            AssetsDir_Add                       (XPATH* assetsdir);
    XVECTOR<XPATH*>*                AssetsDir_GetAll                    ();
    XPATH*                          AssetsDir_Get                       (int index);
    bool                            AssetsDir_DeleteAll                 ();

    #ifdef INP_ACTIVE
    bool                            CreateInputDevices                  (INPMANAGER* inpmanager, GRPANDROIDSCREEN* screen);
    bool                            DeleteInputDevices                  ();
    #endif  
  
    void                            Clean                               ();

    #ifdef INP_ACTIVE
    INPANDROIDDEVICEKEYBOARD*       keyboard;
    INPANDROIDDEVICEMOUSE*          mouse;
    #endif

    android_app*                    androidapplication;

    bool                            initialized;
    bool                            screenactived;

    XVECTOR<XPATH*>                 assetsdirs;

};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




