/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       ANDROIDNativeActivity.h
* 
* @class      ANDROIDNATIVEACTIVITY
* @brief      ANDROID Native Activity class
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

#include <jni.h>

#include <android/window.h>
#include <android/input.h>
#include <android/sensor.h>
#include <android_native_app_glue.h>
#include <android/native_activity.h>

#include "XBase.h"
#include "XSubject.h"


/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

typedef int32_t       STATUS;

const STATUS          STATUS_OK   = 0;
const STATUS          STATUS_KO   = -1;
const STATUS          STATUS_EXIT = -2;

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class ANDROIDNATIVEINPUT_HANDLER
{
  public:

                                     ANDROIDNATIVEINPUT_HANDLER()                                               {               };
    virtual                         ~ANDROIDNATIVEINPUT_HANDLER()                                               {               };

    virtual bool                     OnTouchEvent                             (AInputEvent* event)  = 0;
    virtual bool                     OnKeyboardEvent                          (AInputEvent* event)  = 0;
    virtual bool                     OnTrackballEvent                         (AInputEvent* event)  = 0;

};


class ANDROIDNATIVEACTIVITY_HANDLER
{
  public:
                                      ANDROIDNATIVEACTIVITY_HANDLER           ()                                {               };
    virtual                          ~ANDROIDNATIVEACTIVITY_HANDLER           ()                                {               };

    virtual STATUS                    OnActivate                              () = 0;
    virtual void                      OnDeactivate                            () = 0;
    virtual STATUS                    OnStep                                  () = 0;

    virtual void                      OnStart                                 ()                                {               };
    virtual void                      OnResume                                ()                                {               };
    virtual void                      OnPause                                 ()                                {               };
    virtual void                      OnStop                                  ()                                {               };
    virtual void                      OnDestroy                               ()                                {               };

    virtual void                      OnSaveState                             (void** data, size_t* size)       {               };
    virtual void                      OnConfigurationChanged                  ()                                {               };
    virtual void                      OnLowMemory                             ()                                {               };

    virtual void                      OnCreateWindow                          ()                                {               };
    virtual void                      OnDestroyWindow                         ()                                {               };
    virtual void                      OnGainFocus                             ()                                {               };
    virtual void                      OnLostFocus                             ()                                {               };
};

class ANDROIDNATIVEACTIVITY_EVENTLOOP
{
  public:
                                      ANDROIDNATIVEACTIVITY_EVENTLOOP         (android_app* application);
    virtual                          ~ANDROIDNATIVEACTIVITY_EVENTLOOP         ();

    void                              Run                                     (ANDROIDNATIVEACTIVITY_HANDLER* activityhandler, ANDROIDNATIVEINPUT_HANDLER* inputhandler);

  protected:

    void                              Activate                                ();
    void                              Deactivate                              ();

    int32_t                           ProcessInputEvent                       (AInputEvent* event, android_app* application);
    void                              ProcessApplicationEvent                 (int32_t command, android_app* application);

  private:

    void                              Clean                                   ();

    static int32_t                    CallbackInput                           (android_app* application, AInputEvent* event);
    static void                       CallbackEvent                           (android_app* app, int32_t command);


    bool                              enabled;
    bool                              quit;

    android_app*                      application;
    ANDROIDNATIVEACTIVITY_HANDLER*    activityhandler;
    ANDROIDNATIVEINPUT_HANDLER*       inputhandler;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





