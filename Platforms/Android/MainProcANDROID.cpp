/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       MainProcANDROID.cpp
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "MainProcANDROID.h"

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#include "XANDROIDFactory.h"
#include "XANDROIDTrace.h"
#include "XANDROIDRand.h"
#include "XANDROIDSleep.h"

#ifdef XSYSTEM_ACTIVE
#include "XANDROIDSystem.h"
#endif

#ifdef XSHAREDMEMORYMANAGER_ACTIVE
#include "XANDROIDSharedMemoryManager.h"
#endif


#ifdef GRP_ACTIVE
#include "GRPANDROIDScreen.h"
#include "GRPANDROIDFactory.h"
#endif

#ifdef INP_ACTIVE
#include "INPANDROIDDeviceKeyboard.h"
#include "INPANDROIDDeviceMouse.h"
#include "INPManager.h"
#include "INPFactory.h"
#include "INPANDROIDFactory.h"
#endif

#ifdef SND_ACTIVE
#include "SNDANDROIDFactory.h"
#endif

#ifdef DIO_ACTIVE
#include "DIODNSResolver.h"
#include "DIOANDROIDFactory.h"

#ifdef DIO_ALERTS_ACTIVE
#include "DIOAlerts.h"
#endif
#endif

#include "VersionFrameWork.h"

#include "XFeedback_Control.h"
#include "XRand.h"
#include "XDir.h"
#include "XSleep.h"
#include "XPath.h"
#include "XString.h"
#include "XLog.h"
#include "XFile.h"
#include "XTimer.h"
#include "XThreadCollected.h"
#include "XDateTime.h"
#include "XTranslation_GEN.h"
#include "XTranslation.h"
#include "XPublisher.h"
#include "XFileZIP.h"

#include "APPFlowBase.h"
#include "APPFlowGraphics.h"

#include "GRPXEvent.h"
#include "GRPViewPort.h"
#include "GRP2DCanvas.h"

#include "ANDROIDJNI.h"


/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"


/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

#ifdef XTRACE_ACTIVE
XANDROIDTRACE    androiddebugtrace;
#endif

MAINPROCANDROID  androidmain;
void*            java_vm;


/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         MAINPROCANDROID::MAINPROCANDROID()
* @brief      Constructor of class
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
MAINPROCANDROID::MAINPROCANDROID()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         MAINPROCANDROID::~MAINPROCANDROID()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
MAINPROCANDROID::~MAINPROCANDROID()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MAINPROCANDROID::Ini(XSTRING* apkpath, XSTRING* datapath, APPFLOWMAIN* appmain, APPFLOWBASE_MODE_TYPE applicationmode)
* @brief      Initialize the object
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  apkpath : Apkpath pointer to use.
* @param[in]  datapath : Datapath pointer to use.
* @param[in]  appmain : Appmain pointer to use.
* @param[in]  applicationmode : Applicationmode value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MAINPROCANDROID::Ini(XSTRING* apkpath, XSTRING* datapath, APPFLOWMAIN* appmain, APPFLOWBASE_MODE_TYPE applicationmode)
{
  // required for openal as static library
  java_vm = androidapplication->activity->vm;

  this->appmain = appmain;

  if(!Factorys_Ini()) 
    {
      return false;
    }

  XPATH xpathroot;
  GEN_XPATHSMANAGER.GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpathroot);

  if(apkpath && datapath)  
    {
      XPATH _apkpath;
      XPATH _datapath;

      _apkpath  = apkpath->Get();
      _datapath = datapath->Get();

      OverturnAssetsToExternalLocation(_apkpath, _datapath, xpathroot);
    }

  GetDPI(androidapplication);

  #ifdef APPFLOW_ACTIVE

  if(!appmain)            
    {
      return false;
    }

  if(!appmain->Create())  
    {
      return false;
    }

  if(!appmain->Ini(this, applicationmode)) 
    {
      return false;
    }

  #endif

  GetXPathExec()->SetOnlyDriveAndPath();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MAINPROCANDROID::Update()
* @brief      Update
* @ingroup    PLATFORM_ANDROID
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MAINPROCANDROID::Update()
{
  #ifdef INP_ACTIVE
  GEN_INPMANAGER.Update();
  #endif

  #ifdef APPFLOW_ACTIVE
  if(appmain)
    {
      if(!appmain->Update()) return false;
    }
  #endif

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MAINPROCANDROID::End()
* @brief      End the object
* @ingroup    PLATFORM_ANDROID
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MAINPROCANDROID::End()
{
  #ifdef APPFLOW_ACTIVE
  if(appmain) 
    {
      appmain->End();
      appmain->Delete();
    }
  #endif


  DeleteAllExecParams();

  GetXPathExec()->Empty();

  XFILE_DISPLAYNOTCLOSEFILES
  XFEEDBACK_CONTROL_DISPLAYFEEDBACK  
  XFEEDBACK_CONTROL_DELETE 

  Factorys_End();

  XMEMORY_CONTROL_DEACTIVATED
  XMEMORY_CONTROL_DISPLAYMEMORYLEAKS

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         android_app* MAINPROCANDROID::GetAndroidApplication()
* @brief      Get android application
* @ingroup    PLATFORM_ANDROID
* 
* @return     android_app* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
android_app* MAINPROCANDROID::GetAndroidApplication()
{
  return androidapplication;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void MAINPROCANDROID::SetAndroidApplication(android_app* application)
* @brief      Set android application
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  application : Application pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void MAINPROCANDROID::SetAndroidApplication(android_app* application)
{
  androidapplication = application;
}


#ifdef INP_ACTIVE
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPDEVICE* MAINPROCANDROID::GetKeyboard()
* @brief      Get keyboard
* @ingroup    PLATFORM_ANDROID
* 
* @return     INPDEVICE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPDEVICE* MAINPROCANDROID::GetKeyboard()
{
  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPDEVICE* MAINPROCANDROID::GetTouchscreen()
* @brief      Get touchscreen
* @ingroup    PLATFORM_ANDROID
* 
* @return     INPDEVICE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPDEVICE* MAINPROCANDROID::GetTouchscreen()
{  
  return NULL;
}
#endif


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MAINPROCANDROID::OnTouchEvent(AInputEvent* event)
* @brief      On touch event
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  event : Event to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MAINPROCANDROID::OnTouchEvent(AInputEvent* event)
{
  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE , __L("[ANDROID Event] INI OnTouchEvent"));

  int32_t action        = AMotionEvent_getAction(event);
  size_t  pointercount  = AMotionEvent_getPointerCount(event);

  if(!mouse) mouse = (INPANDROIDDEVICEMOUSE*)GEN_INPMANAGER.GetDevice(INPDEVICE_TYPE_MOUSE);

  if(mouse)
    {
      for(size_t c = 0; c<pointercount; ++c)
        {
          int32_t pointerID    = c;
          int32_t pointerIndex = AMotionEvent_getPointerId(event, c);

          if(pointerIndex >= mouse->GetCursors()->GetSize())
            {
              INPCURSOR* cursor = GEN_NEW INPCURSOR();
              if(cursor)
                {
                  cursor->SetID(INPCURSOR_ID(INPCURSOR_ID_TOUCHSCREEN1+pointerIndex));
                  cursor->SetHavePreSelect(false);

                  mouse->GetCursors()->Add(cursor);
                }
            }

          if(pointerIndex >= mouse->GetButtons()->GetSize())
            {
              INPBUTTON* button = GEN_NEW INPBUTTON();
              if(button)
                {
                  button->SetID(INPBUTTON_ID_TOUCHSCREEN);
                  mouse->GetButtons()->Add(button);
                }
            }

          float x        = AMotionEvent_getX(event, pointerID);
          float y        = AMotionEvent_getY(event, pointerID);
          float pressure = AMotionEvent_getPressure(event, pointerID);

          switch(action & AMOTION_EVENT_ACTION_MASK)
            {
              case AMOTION_EVENT_ACTION_UP            :
              case AMOTION_EVENT_ACTION_POINTER_UP    : { mouse->AddPosition(pointerIndex, x, y, false);
                                                          if(mouse->GetButton(pointerIndex))
                                                            {
                                                              mouse->GetButton(pointerIndex)->SetPressed(false);
                                                              mouse->GetButton(pointerIndex)->SetPressure(pressure);

                                                              if(mouse->GetButton(pointerIndex)->GetState() != INPBUTTON_STATE_UP)
                                                                {
                                                                  mouse->GetButton(pointerIndex)->SetState(INPBUTTON_STATE_RELEASED);
                                                                  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("> ANDROID Motion %d x %f y %f p %f a %d RELEASED"), pointerID, x, y, pressure, action);

                                                                }
                                                                else
                                                                {
                                                                  mouse->GetButton(pointerIndex)->SetState(INPBUTTON_STATE_UP);
                                                                  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("> ANDROID Motion %d x %f y %f p %f a %d UP"), pointerID, x, y, pressure, action);
                                                                }
                                                            }
                                                        }
                                                        break;
              case AMOTION_EVENT_ACTION_MOVE          :
              case AMOTION_EVENT_ACTION_DOWN          :
              case AMOTION_EVENT_ACTION_POINTER_DOWN  : { mouse->AddPosition(pointerIndex, x, y, true);
                                                          if(mouse->GetButton(pointerIndex))
                                                            {
                                                              mouse->GetButton(pointerIndex)->SetPressed(true);
                                                              mouse->GetButton(pointerIndex)->SetPressure(pressure);

                                                              if(mouse->GetButton(pointerIndex)->GetState() == INPBUTTON_STATE_UP)
                                                                {
                                                                  mouse->GetButton(pointerIndex)->SetState(INPBUTTON_STATE_PRESSED);
                                                                  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("> ANDROID Motion %d x %f y %f p %f a %d PRESSED"), pointerID, x, y, pressure, action);
                                                                }
                                                                else
                                                                {
                                                                  mouse->GetButton(pointerIndex)->SetState(INPBUTTON_STATE_HOLD);
                                                                  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("> ANDROID Motion %d x %f y %f p %f a %d HOLD"), pointerID, x, y, pressure, action);
                                                                }
                                                            }
                                                        }
                                                        break;

              case AMOTION_EVENT_ACTION_CANCEL        : if (mouse->GetButton(pointerIndex))
                                                        {
                                                          mouse->GetButton(pointerIndex)->SetPressed(false);
                                                          mouse->GetButton(pointerIndex)->SetState(INPBUTTON_STATE_UP);
                                                        }
                                                        break;
            }
        }
    }


  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE , __L("[ANDROID Event] END OnTouchEvent"));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MAINPROCANDROID::OnKeyboardEvent(AInputEvent* event)
* @brief      On keyboard event
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  event : Event to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MAINPROCANDROID::OnKeyboardEvent(AInputEvent* event)
{
  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[ANDROID Event] INI OnKeyboardEvent Android code [%04x]"), AKeyEvent_getKeyCode(event));

  INPDEVICE* keyboard = androidmain.GetKeyboard();
  if(!keyboard) return false;

  INPBUTTON* button = keyboard->GetButtonByCode(AKeyEvent_getKeyCode(event));
  if(!button) return false;

  XDWORD keyEvent = AKeyEvent_getAction(event);
  switch(keyEvent)
    {
      case AKEY_EVENT_ACTION_UP     : button->SetPressed(false);
                                      XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("> ANDROID Keyboard %d ID %d RELEASED"), keyEvent, button->GetID());
                                      break;

      case AKEY_EVENT_ACTION_DOWN   : button->SetPressed(true);
                                      XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("> ANDROID Keyboard %d ID %d PRESSED"), keyEvent, button->GetID());
                                      break;
    }

  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE , __L("[ANDROID Event] END OnKeyboardEvent"));

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MAINPROCANDROID::OnTrackballEvent(AInputEvent* event)
* @brief      On trackball event
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  event : Event to process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MAINPROCANDROID::OnTrackballEvent(AInputEvent* event)
{
  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE , __L("[ANDROID Event] OnTrackballEvent"));
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         STATUS MAINPROCANDROID::OnActivate()
* @brief      On activate
* @ingroup    PLATFORM_ANDROID
* 
* @return     STATUS : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
STATUS MAINPROCANDROID::OnActivate()
{
  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE , __L("[ANDROID Event] OnActivate"));
  
  return STATUS_OK;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void MAINPROCANDROID::OnDeactivate()
* @brief      On deactivate
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
void MAINPROCANDROID::OnDeactivate()
{
  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE , __L("[ANDROID Event] OnDeactivate"));  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         STATUS MAINPROCANDROID::OnStep()
* @brief      On step
* @ingroup    PLATFORM_ANDROID
* 
* @return     STATUS : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
STATUS MAINPROCANDROID::OnStep()
{
  //XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE , __L("[ANDROID Event] INI OnStep"));

  // Do NOT treat !screenactived as a fatal error. On real devices APP_CMD_GAINED_FOCUS
  // (Activate → enabled OnStep loop) can race ahead of a failed/partial OnCreateWindow, or
  // Create() may still be pending. Returning STATUS_KO here calls ANativeActivity_finish()
  // with no Java exception — the classic "flash for ~1s then exits silently" symptom that
  // BlueStacks often hides because its window/focus ordering is more forgiving.
  if(!screenactived)
    {
      return STATUS_OK;
    }

  if(!androidmain.Update()) return STATUS_KO;

  //XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE , __L("[ANDROID Event] END OnStep"));

  return STATUS_OK;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void MAINPROCANDROID::OnStart()
* @brief      On start
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
void MAINPROCANDROID::OnStart()
{
  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE , __L("[ANDROID Event] OnStart"));

  if(initialized)
    {
      androidmain.End(); 
      initialized = false;
    }

  XSTRING apkpath;
  XSTRING datapath;

  androidmain.GetPackageResourcePath(androidapplication, apkpath);
  androidmain.GetAbsolutePath(androidapplication, datapath);
   
  androidmain.GetXPathExec()->Set(apkpath.Get());

  GEN_XPATHSMANAGER.GetAppExecPath()->Set(GetXPathExec()->Get());
  GetXPathExec()->SetOnlyDriveAndPath();
  
  GEN_XPATHSMANAGER.AddPathSection(XPATHSMANAGERSECTIONTYPE_ROOT, datapath);
  
  #ifdef APPFLOW_ACTIVE  
  if(androidmain.Ini(&apkpath, &datapath, &GEN_appmain, APPFLOWBASE_MODE_TYPE_APPLICATION))
  #else
  if(androidmain.Ini(&apkpath))
  #endif
    {
      ANativeActivity_setWindowFlags(androidapplication->activity, AWINDOW_FLAG_FULLSCREEN, 0);
    }  

  initialized = true;

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void MAINPROCANDROID::OnResume()
* @brief      On resume
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
void MAINPROCANDROID::OnResume()
{
  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE , __L("[ANDROID Event] OnResume"));
 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void MAINPROCANDROID::OnPause()
* @brief      On pause
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
void MAINPROCANDROID::OnPause()
{
  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE , __L("[ANDROID Event] OnPause"));  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void MAINPROCANDROID::OnStop()
* @brief      On stop
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
void MAINPROCANDROID::OnStop()
{
  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE , __L("[ANDROID Event] OnStop"));  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void MAINPROCANDROID::OnDestroy()
* @brief      On destroy
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
void MAINPROCANDROID::OnDestroy()
{
  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE , __L("[ANDROID Event] OnDestroy"));  

  if(initialized) 
    {
      androidmain.End(); 
      initialized = false;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void MAINPROCANDROID::OnSaveState(void** data, size_t* size)
* @brief      On save state
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  data : Data buffer to use.
* @param[in]  size : Size value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void MAINPROCANDROID::OnSaveState(void** data, size_t* size)
{
  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE , __L("[ANDROID Event] OnSaveState"));  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void MAINPROCANDROID::OnConfigurationChanged()
* @brief      On configuration changed
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
void MAINPROCANDROID::OnConfigurationChanged()
{
  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE , __L("[ANDROID Event] OnConfigurationChanged"));

  if(!androidapplication)         return;
  if(!androidapplication->window) return;

  ANativeActivity_setWindowFlags(androidapplication->activity, AWINDOW_FLAG_FULLSCREEN, 0);

  int32_t maxwidth  = ANativeWindow_getWidth(androidapplication->window);
  int32_t maxheight = ANativeWindow_getHeight(androidapplication->window);

  APPFLOWBASE* app = NULL;
  if(androidmain.GetAppMain()) app = androidmain.GetAppMain()->GetApplication();

  if(!app) return;

  APPFLOWGRAPHICS* applicationgrp  = (APPFLOWGRAPHICS*)app;
  if(applicationgrp)
    {
      GRPANDROIDSCREEN* mainscreen = (GRPANDROIDSCREEN*)applicationgrp->GetMainScreen();
      if(mainscreen)
        {
          // Track P / Fase 5.3: design-resolution screens (UI_System dashboard 1440x900, Canvas2D, etc.)
          // must NOT be resized to the native window. GLES letterbox + MapWindowToCanvas already fit the
          // surface. UpdateSize(native) would wipe the design canvas and break touch mapping.
          // Still fire CHANGESIZE so observers can refresh (UI_System keeps design size + UIScale=1).
          #ifdef GRP_OPENGL_ACTIVE
          {
            XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[ANDROID] OnConfigurationChanged: keep design %dx%d; native surface %dx%d"), mainscreen->GetWidth(), mainscreen->GetHeight(), maxwidth, maxheight);
            GRPXEVENT grpevent(this, GRPXEVENT_TYPE_SCREEN_CHANGESIZE);
            grpevent.SetScreen(mainscreen);
            applicationgrp->PostEvent(&grpevent);
          }
          #else
          mainscreen->UpdateSize(maxwidth, maxheight);
          #endif
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void MAINPROCANDROID::OnLowMemory()
* @brief      On low memory
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
void MAINPROCANDROID::OnLowMemory()
{
  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE , __L("[ANDROID Event] OnLowMemory"));  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void MAINPROCANDROID::OnCreateWindow()
* @brief      On create window
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
void MAINPROCANDROID::OnCreateWindow()
{
  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE , __L("[ANDROID Event] OnCreateWindow"));
  
  if(!androidapplication)         return;
  if(!androidapplication->window) return;

  ANativeActivity_setWindowFlags(androidapplication->activity, AWINDOW_FLAG_FULLSCREEN, 0);

  
  #ifndef GRP_OPENGL_ACTIVE
  // Software path: lock/unlock to query the real buffer geometry (stride).
  ANativeWindow_Buffer abuffer;
  if(ANativeWindow_lock(androidapplication->window, &abuffer, NULL) >= 0)
    {
      ANativeWindow_unlockAndPost(androidapplication->window);
    }  

  int32_t w         = abuffer.width;          
  int32_t h         = abuffer.height;  

  if(w > h)  
         w = abuffer.stride; 
    else h = abuffer.stride;           
  #else
  // OpenGL/EGL path: NEVER call ANativeWindow_lock here. lock() connects the
  // window to the CPU buffer API (native_window_api_connect) and that bond is
  // NOT released by unlockAndPost, so a later eglCreateWindowSurface would fail
  // with EGL_BAD_ALLOC ("already connected to another API"). Query the size
  // without touching the producer side.
  int32_t w         = ANativeWindow_getWidth(androidapplication->window);
  int32_t h         = ANativeWindow_getHeight(androidapplication->window);
  #endif

  int32_t maxwidth  = 0;
  int32_t maxheight = 0;

  if(w > h)
    {
      maxwidth  = w;
      maxheight = h;
    }
   else
    {
      maxwidth  = h;
      maxheight = w;
    }

  APPFLOWBASE* app = NULL;
  if(androidmain.GetAppMain()) app = androidmain.GetAppMain()->GetApplication();

  if(!app)
    {
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[ANDROID Event] OnCreateWindow: application not ready (Ini failed?)"));
      return;
    }

  APPFLOWGRAPHICS* applicationgrp  = (APPFLOWGRAPHICS*)app;
  if(applicationgrp)
    {
      GRPANDROIDSCREEN* mainscreen = (GRPANDROIDSCREEN*)applicationgrp->GetMainScreen();
      if(mainscreen)
        {         
          mainscreen->SetMaxSize(maxwidth, maxheight);

          if(!mainscreen->GetWidth() || !mainscreen->GetHeight()) mainscreen->SetSize(maxwidth, maxheight);

          // Step 1 — SCREEN_CREATING: let the app configure screen size, style, etc.
          GRPXEVENT grpevent(this, GRPXEVENT_TYPE_SCREEN_CREATING);
          grpevent.SetScreen(mainscreen);
          applicationgrp->PostEvent(&grpevent);

          #ifndef GRP_OPENGL_ACTIVE
          ANativeWindow_setBuffersGeometry(androidapplication->window, mainscreen->GetWidth(), mainscreen->GetHeight(), WINDOW_FORMAT_RGBA_8888);

          ANativeWindow_Buffer abuffer;
          if(ANativeWindow_lock(androidapplication->window, &abuffer, NULL) >= 0)
            {
              ANativeWindow_unlockAndPost(androidapplication->window);
            }  

          int32_t w         = abuffer.width;          
          int32_t h         = abuffer.height;  

          if(w > h)  
                 w = abuffer.stride; 
            else h = abuffer.stride;           
  
          mainscreen->SetSize(w, h);
          mainscreen->SetMaxSize(w, h);

          ANativeWindow_setBuffersGeometry(androidapplication->window, mainscreen->GetWidth(), mainscreen->GetHeight(), WINDOW_FORMAT_RGBA_8888);
          #else
          // OpenGL/EGL path: the application's SCREEN_CREATING handler (above) has
          // already set the desired canvas/screen size (e.g. 1024x768 in Canvas2D).
          // Do NOT overwrite it with the native window size: the canvas must keep the
          // design resolution. The full device/window resolution (e.g. 2712x1220) is
          // only used for the EGL surface + viewport, which the blitter handles via
          // eglQuerySurface and letterbox scaling. We also must NOT lock the window
          // (that bonds it to the CPU API and breaks eglCreateWindowSurface) nor set
          // its buffer geometry here (the blitter does it in PostCreateHook with the
          // EGL native visual id). So: leave the screen size exactly as the app set it.
          #endif

          // Step 2 — Create the screen (allocates screencanvas).  Must happen BEFORE
          // SCREEN_CANVASCREATING so that CreateViewport() has a fully-initialised screen.
          if(!mainscreen->Create(true))
            {
              XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[ANDROID Event] OnCreateWindow: mainscreen->Create failed"));
              return;
            }

          mainscreen->SetAndroidHandle(androidapplication->window);

          // Step 3 — SCREEN_CANVASCREATING: let the app create its own named viewport
          // (e.g. "message_viewport" in UI_Message, "canvas2d_viewport" in Canvas2D).
          // On Linux/Windows this event is fired inside APPFlowGraphics::CreateMainScreenProcess()
          // (the #ifndef ANDROID block).  On Android that block is skipped, so we fire it here.
          // IMPORTANT: this must come BEFORE the fallback viewport creation below, so that
          // the app viewport (if any) is found by GetViewport(0) and the fallback is skipped.
          {
            GRPXEVENT grpeventcanvas(this, GRPXEVENT_TYPE_SCREEN_CANVASCREATING);
            grpeventcanvas.SetScreen(mainscreen);
            applicationgrp->PostEvent(&grpeventcanvas);
          }

          // Step 4 — Fallback: if the app did not create any viewport, create the default one.
          // This ensures headless/simple apps still have a viewport to render into.
          {
            GRPVIEWPORT* viewport = mainscreen->GetViewport(0);
            if(!viewport)
              {
                if(!mainscreen->CreateViewport(GRPVIEWPORT_ID_MAIN, 0.0f, 0.0f, (float)mainscreen->GetWidth(), (float)mainscreen->GetHeight(), 0, 0, (float)mainscreen->GetWidth(), (float)mainscreen->GetHeight()))
                  {
                    return;
                  }                  
              }
          }

          {
              #ifdef INP_ACTIVE
              if(!CreateInputDevices(&INPMANAGER::GetInstance(), (GRPANDROIDSCREEN*)mainscreen)) return;
              #endif

              screenactived = true;
          }
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void MAINPROCANDROID::OnDestroyWindow()
* @brief      On destroy window
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
void MAINPROCANDROID::OnDestroyWindow()
{
  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE , __L("[ANDROID Event] OnDestroyWindow"));
  
  #ifdef INP_ACTIVE
  DeleteInputDevices();  
  #endif
  
  APPFLOWBASE* app = NULL;
  if(androidmain.GetAppMain()) app = androidmain.GetAppMain()->GetApplication();

  if(!app) return;

  APPFLOWGRAPHICS* applicationgrp  = (APPFLOWGRAPHICS*)app;
  if(applicationgrp)
    {
      GRPANDROIDSCREEN* mainscreen = (GRPANDROIDSCREEN*)applicationgrp->GetMainScreen();
      if(mainscreen) 
        {         
          if(screenactived)
            {
              mainscreen->Delete();  
              screenactived = false; 
            }       
        }      
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void MAINPROCANDROID::OnGainFocus()
* @brief      On gain focus
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
void MAINPROCANDROID::OnGainFocus()
{
  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE , __L("[ANDROID Event] OnGainFocus"));

  //APPFLOWGRAPHICS* gap = dynamic_cast<APPFLOWGRAPHICS*>(application);
  //if(gap) gap->OnFocus();  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void MAINPROCANDROID::OnLostFocus()
* @brief      On lost focus
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
void MAINPROCANDROID::OnLostFocus()
{
  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE , __L("[ANDROID Event] OnLostFocus"));

  //APPFLOWGRAPHICS* gap = dynamic_cast<APPFLOWGRAPHICS*>(application);
  //if(gap) gap->OnLostFocus();  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MAINPROCANDROID::GetPackageResourcePath(struct android_app* app, XSTRING& path)
* @brief      Get package resource path
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  app : Application pointer to use.
* @param[in]  path : Path to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MAINPROCANDROID::GetPackageResourcePath(struct android_app* app, XSTRING& path)
{
  JNIEnv* env;
  JavaVM* vm     = app->activity->vm;
  bool    status = false;

  vm->AttachCurrentThread(&env, NULL);

  jclass    clazz   = env->GetObjectClass(app->activity->clazz);
  jmethodID methodj = env->GetMethodID(clazz, "getPackageResourcePath", "()Ljava/lang/String;");

  jstring   jstr    = (jstring)env->CallObjectMethod(app->activity->clazz, methodj);
  if(jstr != NULL)
    {
      const char* str = env->GetStringUTFChars(jstr, NULL);
      path =  str;
      env->ReleaseStringUTFChars(jstr, str);

      status = true;
    }

  vm->DetachCurrentThread();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MAINPROCANDROID::GetAbsolutePath(struct android_app* app, XSTRING& path)
* @brief      Get absolute path
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  app : Application pointer to use.
* @param[in]  path : Path to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MAINPROCANDROID::GetAbsolutePath(struct android_app* app, XSTRING& path)
{
  JNIEnv* env;
  JavaVM* vm     = app->activity->vm;
  bool    status = false;

  vm->AttachCurrentThread(&env, NULL);

  jclass    activityclass   = env->GetObjectClass(app->activity->clazz);
  jmethodID getfilesdir     = env->GetMethodID(activityclass, "getFilesDir", "()Ljava/io/File;");
  jobject   file            = env->CallObjectMethod(app->activity->clazz, getfilesdir);
  jclass    fileclass       = env->FindClass("java/io/File");
  jmethodID getabsolutepath = env->GetMethodID(fileclass, "getAbsolutePath", "()Ljava/lang/String;");

  jstring jpath = (jstring)env->CallObjectMethod(file, getabsolutepath);
  if(jpath != NULL)
    {
      const char* str = env->GetStringUTFChars(jpath, NULL);
      path =  str;
      env->ReleaseStringUTFChars(jpath, str);
      status = true;
    }

  vm->DetachCurrentThread();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MAINPROCANDROID::GetDPI(struct android_app* app)
* @brief      Get DPI
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  app : Application pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MAINPROCANDROID::GetDPI(struct android_app* app)
{

  //#ifdef GRP_ACTIVE
  //grpfactory  = GEN_NEW GRPANDROIDFACTORY();
  //if(!grpfactory) return false;

  //// get the screen dpi  //DIEGO : supongo que meter esto en una funci?n consum_a tinta de monitor, no? de hecho , no hay una clase para manejar el JNI????
  //JNIEnv* env = NULL;

  //jint      res     = androidapplication->activity->vm->AttachCurrentThread(&env, NULL);

  //jclass    clazz   = env->GetObjectClass(androidapplication->activity->clazz);
  //jfieldID  fid     = env->GetStaticFieldID(clazz, "dpi", "I");
  //jint      i       = env->GetStaticIntField(clazz, fid);

  //androidapplication->activity->vm->DetachCurrentThread();

  //XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("jni %lld"), timer->GetMeasureMilliSeconds());
  //
  //// assign it

  //grpfactory->dpi = i;

  //#endif

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void MAINPROCANDROID::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
void MAINPROCANDROID::Clean()
{
  #ifdef INP_ACTIVE
  keyboard            = NULL;
  mouse               = NULL;
  #endif

  initialized         = false;
  screenactived       = false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MAINPROCANDROID::Factorys_Ini()
* @brief      Factorys ini
* @ingroup    PLATFORM_ANDROID
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MAINPROCANDROID::Factorys_Ini()
{
  if(!XFACTORY::SetInstance(GEN_NEW XANDROIDFACTORY()))
    {
      return false;
    }

  XFEEDBACK_CONTROL_CREATE

  #ifdef XSYSTEM_ACTIVE  
  if(!XSYSTEM::SetInstance(GEN_NEW XANDROIDSYSTEM())) 
    {
      return false;
    }
  XBUFFER::SetGlobalHardwareUseLittleEndian(GEN_XSYSTEM.HardwareUseLittleEndian());
  #endif
  
  if(!XRAND::SetInstance(GEN_NEW XANDROIDRAND())) 
    {
      return false;
    }
  
  #ifdef XSLEEP_ACTIVE
  if(!XSLEEP::SetInstance(GEN_NEW XANDROIDSLEEP())) 
    {
      return false;
    }
  #endif

  #ifdef XSHAREDMEMORYMANAGER_ACTIVE
  if(!XSHAREDMEMORYMANAGER::SetInstance(GEN_NEW XANDROIDSHAREDMEMORYMANAGER())) 
    {
      return false;  
    }
  #endif
    

  #ifdef XTRACE_VIRTUALCLOCKTICK
  xtimerclock = GEN_NEW XTIMERCLOCK();
  if(!xtimerclock) 
    {
      return false;
    }
  #endif

  #ifdef INP_ACTIVE  
  if(!INPFACTORY::SetInstance(GEN_NEW INPANDROIDFACTORY())) 
    {
      return false;
    }
  #endif

  #ifdef DIO_ACTIVE
  if(!DIOFACTORY::SetInstance(GEN_NEW DIOANDROIDFACTORY())) 
    {
      return false;
    }
  #endif

  #ifdef SND_ACTIVE  
  if(!SNDFACTORY::SetInstance(GEN_NEW SNDANDROIDFACTORY())) 
    {    
      return false;
    }

  if(!SNDFACTORY::GetInstance().Ini()) 
    {
      // return false;
    }
  #endif

  #ifdef GRP_ACTIVE
  if(!GRPFACTORY::SetInstance(GEN_NEW GRPANDROIDFACTORY())) 
    {
      return false;
    }
  #endif

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MAINPROCANDROID::Factorys_End()
* @brief      Factorys end
* @ingroup    PLATFORM_ANDROID
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MAINPROCANDROID::Factorys_End()
{
  #ifdef GRP_ACTIVE
  GRPFACTORY::DelInstance();
  #endif

  #ifdef SND_ACTIVE
  if(SNDFACTORY::GetIsInstanced())
    {
      SNDFACTORY::GetInstance().End();
      SNDFACTORY::DelInstance();
    }
  #endif

  #ifdef DIO_ACTIVE
  #ifdef DIO_GPIO_ACTIVE
  if(DIOGPIO::GetIsInstanced())
    {
      DIOGPIO::GetInstance().End();
      DIOGPIO::DelInstance();
    }
  #endif
	
	#ifdef DIO_STREAMUDP_ACTIVE
	DIODNSRESOLVER::DelInstance();
	#endif 
	
  DIOFACTORY::DelInstance();
  #endif

  #ifdef INP_ACTIVE
  INPMANAGER::DelInstance();
  INPFACTORY::DelInstance();
  #endif

  #ifdef XTRACE_VIRTUALCLOCKTICK
  if(xtimerclock)
    {
      GEN_DELETE xtimerclock;
      xtimerclock = NULL;
    }
  #endif

  #ifdef DIO_ALERTS_ACTIVE
  DIOALERTS::DelInstance();
  #endif

  #ifdef XLOG_ACTIVE
  XLOG::DelInstance();
  #endif

  XPATHSMANAGER::DelInstance();

  XTRANSLATION::DelInstance();
  XTRANSLATION_GEN::DelInstance();

  #ifdef XPROCESSMANAGER_ACTIVE
  XPROCESSMANAGER::DelInstance();
  #endif

  #ifdef XSHAREDMEMORYMANAGER_ACTIVE   
  XSHAREDMEMORYMANAGER::DelInstance();
  #endif

  #ifdef XSLEEP_ACTIVE
  XSLEEP::DelInstance();
  #endif

  XRAND::DelInstance();

  XSYSTEM::DelInstance();

  #ifdef XPUBLISHER_ACTIVE
  XPUBLISHER::DelInstance();
  #endif

  #ifdef XTHREADCOLLECTED_ACTIVE
  XTHREADSCOLLECTEDMANAGER::DelInstance();
  #endif

  XFACTORY::DelInstance();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MAINPROCANDROID::OverturnAssetsToExternalLocation(XPATH& origin, XPATH& datapath, XPATH& target)
* @brief      Overturn assets to external location
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  origin : Origin value.
* @param[in]  datapath : Datapath value.
* @param[in]  target : Target value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MAINPROCANDROID::OverturnAssetsToExternalLocation(XPATH& origin, XPATH& datapath, XPATH& target)
{
  XFILEUNZIP* unzip = GEN_NEW XFILEUNZIP();
  if(!unzip) return false;

  XSTRING  assetsstring;
  bool     status  = false;

  assetsstring = __L("assets");

  XDIR* xdir=GEN_XFACTORY.Create_Dir();
  if(xdir)
    {
      status = unzip->Open(origin);

      //XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[Android Resources] ZIP File: %s"), origin.Get());

      if(status)
        {
          XFILECMPZIP* filecmp;
          int          index = 0;

          status = true;

          do{
              filecmp = unzip->GotoFile(index);
              if(filecmp)
                {
                  XPATH targetpath;

                  targetpath  = datapath;
                  targetpath.Slash_Add();
                  targetpath += filecmp->GetName();

                  targetpath.SetOnlyPath();

                  int position=targetpath.Find(assetsstring, true);
                  if(position!=-1)
                    {
                      XPATH targetfile;
                      XPATH namefile;

                      namefile = filecmp->GetName();
                      namefile.SetOnlyNamefileExt();

                      //targetpath.DeleteCharacters(position, assetsstring.GetSize()+1);

                      targetfile  = targetpath;
                      targetfile += namefile.Get();

                      XFILE* xfile=GEN_XFACTORY.Create_File();
                      if(xfile)
                        {
                          //XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[Android Resources] ZIP descompress File: %s"), targetfile.Get());

                          if(!xfile->Exist(targetfile.Get()))
                            {
                              xdir->Make(targetpath.Get(), true);
                              status = unzip->DecompressFile(filecmp, targetpath, namefile.Get());
                            }

                          GEN_XFACTORY.Delete_File(xfile);
                        }
                    }

                  GEN_DELETE filecmp;

                  index++;

                } else break;

            } while(status);

          unzip->Close();

        }

      GEN_XFACTORY.Delete_Dir(xdir);
    }

  GEN_DELETE unzip;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MAINPROCANDROID::AssetsDir_CreateAll(XPATH& origin)
* @brief      Assets dir create all
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  origin : Origin value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MAINPROCANDROID::AssetsDir_CreateAll(XPATH& origin)
{
  AssetsDir_DeleteAll();

  XPATH       targetpath;
  XFILEUNZIP* unzip   = GEN_NEW XFILEUNZIP();
  XSTRING     assetsstring;
  bool        status  = false;

  if(!unzip) return false;

  assetsstring = __L("assets");

  status = unzip->Open(origin);
  if(status)
    {
      XFILECMPZIP* filecmp;
      int          index = 0;

      status = true;

      do{ filecmp = unzip->GotoFile(index);
          if(filecmp)
            {
              targetpath.Empty();
              
              targetpath += filecmp->GetName();

              targetpath.SetOnlyPath();

              int position=targetpath.Find(assetsstring, true);
              if(position!=-1)
                {   
                  bool found = false;
             
                  targetpath.DeleteCharacters(position, assetsstring.GetSize()+1);   

                  for(int c=0; c<assetsdirs.GetSize(); c++)
                    {
                      XPATH* xpath;

                      xpath = assetsdirs.Get(c);
                      if(!xpath->Compare(targetpath)) 
                        {
                          found = true;
                          break;                               
                        }
                    }               

                  if(!found)
                    {
                      XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("Assets XPATH: %s"), targetpath.Get());
                      
                      AssetsDir_Add(&targetpath);                                                                                                  
                    }
                } 

              index++;

              GEN_DELETE filecmp;

            } else break;

        } while(status);

      unzip->Close();        
    }
      
  GEN_DELETE unzip;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MAINPROCANDROID::AssetsDir_Add(XPATH* assetsdir)
* @brief      Assets dir add
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  assetsdir : Assetsdir pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MAINPROCANDROID::AssetsDir_Add(XPATH* assetsdir)
{
  if(!assetsdir) return false;

  XPATH* newpath = GEN_NEW XPATH();
  if(!newpath) return false;

  newpath->Set(assetsdir->Get());   

  assetsdirs.Add(newpath);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<XPATH*>* MAINPROCANDROID::AssetsDir_GetAll()
* @brief      Assets dir get all
* @ingroup    PLATFORM_ANDROID
* 
* @return     XVECTOR<XPATH*>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<XPATH*>* MAINPROCANDROID::AssetsDir_GetAll()
{
  return &assetsdirs;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XPATH* MAINPROCANDROID::AssetsDir_Get(int index)
* @brief      Assets dir get
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  index : Index value.
* 
* @return     XPATH* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XPATH* MAINPROCANDROID::AssetsDir_Get(int index)
{
  if(assetsdirs.IsEmpty()) return NULL;

  return assetsdirs.Get(index);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MAINPROCANDROID::AssetsDir_DeleteAll()
* @brief      Assets dir GEN_DELETE all
* @ingroup    PLATFORM_ANDROID
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MAINPROCANDROID::AssetsDir_DeleteAll()
{
  if(assetsdirs.IsEmpty()) return false;

  assetsdirs.DeleteContents();
  assetsdirs.DeleteAll();

  return true;
}
    

#ifdef INP_ACTIVE

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MAINPROCANDROID::CreateInputDevices(INPMANAGER* inpmanager, GRPANDROIDSCREEN* screen)
* @brief      Create input devices
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  inpmanager : Inpmanager pointer to use.
* @param[in]  screen : Screen object to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MAINPROCANDROID::CreateInputDevices(INPMANAGER* inpmanager, GRPANDROIDSCREEN* screen)
{
  keyboard  = (INPANDROIDDEVICEKEYBOARD*)INPANDROIDFACTORY::GetInstance().CreateDevice(INPDEVICE_TYPE_KEYBOARD , screen);
  if(keyboard) GEN_INPMANAGER.AddDevice(keyboard);

  mouse     = (INPANDROIDDEVICEMOUSE*)INPANDROIDFACTORY::GetInstance().CreateDevice(INPDEVICE_TYPE_MOUSE, screen);
  if(mouse) GEN_INPMANAGER.AddDevice(mouse);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool MAINPROCANDROID::DeleteInputDevices()
* @brief      Delete input devices
* @ingroup    PLATFORM_ANDROID
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool MAINPROCANDROID::DeleteInputDevices()
{  
  if(keyboard) 
    {
      GEN_INPMANAGER.DelDevice(keyboard);
      keyboard = NULL;
    }
  
  if(mouse) 
    {
      GEN_INPMANAGER.DelDevice(mouse);
      mouse = NULL;
    }

  return true;
}

#endif


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         JNIEXPORT jint JNI_OnLoad(JavaVM* injavavm, void* inreserved)
* @brief      JNI_OnLoad
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  injavavm : Injavavm pointer to use.
* @param[in]  inreserved : Inreserved pointer to use.
* 
* @return     JNIEXPORT jint : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
JNIEXPORT jint JNI_OnLoad(JavaVM* injavavm, void* inreserved)
{
  return JNI_VERSION_1_6;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void android_main(android_app* application)
* @brief      android_main
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  application : Application pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void android_main(android_app* application)
{
  ANDROIDJNI::InitializeJNI(application, JNI_VERSION_1_6);

  ANDROIDNATIVEACTIVITY_EVENTLOOP eventloop(application);

  androidmain.SetAndroidApplication(application);

  eventloop.Run(&androidmain, &androidmain);

  VERSIONFRAMEWORK::DelInstance();

  XFILE_DISPLAYNOTCLOSEFILES
  XFEEDBACK_CONTROL_DISPLAYFEEDBACK
  XFEEDBACK_CONTROL_DELETERESOURCES
  XMEMORY_CONTROL_DISPLAYMEMORYLEAKS
}



