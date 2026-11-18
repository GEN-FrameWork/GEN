/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XANDROIDProcessManager.cpp
* 
* @class      XANDROIDPROCESSMANAGER
* @brief      ANDROID eXtended Utils process management class
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

#include "XANDROIDProcessManager.h"

#include <android_native_app_glue.h>

#include "XBuffer.h"
#include "XString.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XANDROIDPROCESSMANAGER::XANDROIDPROCESSMANAGER()
* @brief      Constructor of class
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
XANDROIDPROCESSMANAGER::XANDROIDPROCESSMANAGER()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XANDROIDPROCESSMANAGER::~XANDROIDPROCESSMANAGER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
XANDROIDPROCESSMANAGER::~XANDROIDPROCESSMANAGER()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XANDROIDPROCESSMANAGER::SetAndroidApplication(android_app* application)
* @brief      Set android application
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  application : Native android_app pointer (activity / JNI).
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XANDROIDPROCESSMANAGER::SetAndroidApplication(android_app* application)
{
  androidapplication = application;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XANDROIDPROCESSMANAGER::MakeSystemCommand(XCHAR* command)
* @brief      Make system command
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  command : Command pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XANDROIDPROCESSMANAGER::MakeSystemCommand(XCHAR* command)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XANDROIDPROCESSMANAGER::MakeCommand(XCHAR* command, XBUFFER* out, int* returncode)
* @brief      Make command
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  command : Command pointer to use.
* @param[out] out : Output buffer.
* @param[out] returncode : Return code pointer.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XANDROIDPROCESSMANAGER::MakeCommand(XCHAR* command, XBUFFER* out, int* returncode)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XANDROIDPROCESSMANAGER::OpenURL(XCHAR* url)
* @brief      Open URL via ACTION_VIEW Intent
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  url : URL to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XANDROIDPROCESSMANAGER::OpenURL(XCHAR* url)
{
  if(!url) return false;

  JNIEnv* env      = NULL;
  bool    attached = false;
  if(!AttachJNIEnv(&env, attached)) return false;

  bool status = false;

  do
    {
      if(!androidapplication || !androidapplication->activity || !androidapplication->activity->clazz)
        {
          break;
        }

      XSTRING urlstr;
      urlstr = url;

      XBUFFER utf8;
      if(!urlstr.ConvertToUTF8(utf8, true)) break;

      jclass intentclass = env->FindClass("android/content/Intent");
      if(!intentclass) break;

      jmethodID intentctor = env->GetMethodID(intentclass, "<init>", "(Ljava/lang/String;)V");
      jmethodID setdata    = env->GetMethodID(intentclass, "setData", "(Landroid/net/Uri;)Landroid/content/Intent;");
      jmethodID addflags   = env->GetMethodID(intentclass, "addFlags", "(I)Landroid/content/Intent;");
      if(!intentctor || !setdata || !addflags)
        {
          env->DeleteLocalRef(intentclass);
          break;
        }

      jstring jaction = env->NewStringUTF("android.intent.action.VIEW");
      jobject intent  = env->NewObject(intentclass, intentctor, jaction);
      env->DeleteLocalRef(jaction);
      if(!intent)
        {
          env->DeleteLocalRef(intentclass);
          break;
        }

      jclass uriclass = env->FindClass("android/net/Uri");
      if(!uriclass)
        {
          env->DeleteLocalRef(intent);
          env->DeleteLocalRef(intentclass);
          break;
        }

      jmethodID parse = env->GetStaticMethodID(uriclass, "parse", "(Ljava/lang/String;)Landroid/net/Uri;");
      if(!parse)
        {
          env->DeleteLocalRef(uriclass);
          env->DeleteLocalRef(intent);
          env->DeleteLocalRef(intentclass);
          break;
        }

      jstring jurl = env->NewStringUTF(utf8.GetSize()? (const char*)utf8.Get() : "");
      jobject uri  = env->CallStaticObjectMethod(uriclass, parse, jurl);
      env->DeleteLocalRef(jurl);
      env->DeleteLocalRef(uriclass);

      if(!uri)
        {
          env->DeleteLocalRef(intent);
          env->DeleteLocalRef(intentclass);
          break;
        }

      env->CallObjectMethod(intent, setdata, uri);
      env->DeleteLocalRef(uri);

      // FLAG_ACTIVITY_NEW_TASK = 0x10000000 — required when starting from a non-Activity context path
      env->CallObjectMethod(intent, addflags, (jint)0x10000000);

      jclass activityclass = env->GetObjectClass(androidapplication->activity->clazz);
      jmethodID startactivity = activityclass ? env->GetMethodID(activityclass, "startActivity", "(Landroid/content/Intent;)V") : NULL;
      if(!activityclass || !startactivity)
        {
          if(activityclass) env->DeleteLocalRef(activityclass);
          env->DeleteLocalRef(intent);
          env->DeleteLocalRef(intentclass);
          break;
        }

      env->CallVoidMethod(androidapplication->activity->clazz, startactivity, intent);
      if(env->ExceptionCheck())
        {
          env->ExceptionClear();
        }
      else
        {
          status = true;
        }

      env->DeleteLocalRef(activityclass);
      env->DeleteLocalRef(intent);
      env->DeleteLocalRef(intentclass);
    }
  while(false);

  DetachJNIEnv(attached);
  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XANDROIDPROCESSMANAGER::Application_Execute(XCHAR* applicationpath, XCHAR* params, XBUFFER* in, XBUFFER* out, int* returncode)
* @brief      Application execute
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  applicationpath : Application path pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  in : In pointer to use.
* @param[out] out : Output out.
* @param[out] returncode : Returncode pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XANDROIDPROCESSMANAGER::Application_Execute(XCHAR* applicationpath, XCHAR* params, XBUFFER* in, XBUFFER* out, int* returncode)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XANDROIDPROCESSMANAGER::Application_IsRunning(XCHAR* command, XDWORD* ID)
* @brief      Application is running
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  command : Command pointer to use.
* @param[out] ID : Process ID pointer.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XANDROIDPROCESSMANAGER::Application_IsRunning(XCHAR* command, XDWORD* ID)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XANDROIDPROCESSMANAGER::Application_GetRunningList(XVECTOR<XPROCESS*>& applist, bool onlywithvalidwindow)
* @brief      Application get running list
* @ingroup    PLATFORM_ANDROID
* 
* @param[out] applist : Application list.
* @param[in]  onlywithvalidwindow : Only with valid window.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XANDROIDPROCESSMANAGER::Application_GetRunningList(XVECTOR<XPROCESS*>& applist, bool onlywithvalidwindow)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XANDROIDPROCESSMANAGER::Application_FindWindowAtPoint(int screenx, int screeny, XPROCESS& outprocess)
* @brief      Application find window at point
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  screenx : Screen X.
* @param[in]  screeny : Screen Y.
* @param[out] outprocess : Out process.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XANDROIDPROCESSMANAGER::Application_FindWindowAtPoint(int screenx, int screeny, XPROCESS& outprocess)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XANDROIDPROCESSMANAGER::Application_Terminate(XDWORD processID, XDWORD exitcode)
* @brief      Application terminate
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  processID : Process ID.
* @param[in]  exitcode : Exit code.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XANDROIDPROCESSMANAGER::Application_Terminate(XDWORD processID, XDWORD exitcode)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XANDROIDPROCESSMANAGER::AttachJNIEnv(JNIEnv** env, bool& attached)
* @brief      Attach JNI env
* @note       INTERNAL
* @ingroup    PLATFORM_ANDROID
* 
* @param[out] env : JNI environment pointer.
* @param[out] attached : true if this call attached the current thread.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XANDROIDPROCESSMANAGER::AttachJNIEnv(JNIEnv** env, bool& attached)
{
  attached = false;
  if(!env) return false;
  *env = NULL;

  if(!androidapplication) return false;
  if(!androidapplication->activity) return false;
  if(!androidapplication->activity->vm) return false;

  JavaVM* vm = androidapplication->activity->vm;
  jint    rc = vm->GetEnv((void**)env, JNI_VERSION_1_6);

  if(rc == JNI_OK && *env) return true;

  if(rc == JNI_EDETACHED)
    {
      if(vm->AttachCurrentThread(env, NULL) != 0) return false;
      attached = true;
      return (*env)?true:false;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XANDROIDPROCESSMANAGER::DetachJNIEnv(bool attached)
* @brief      Detach JNI env
* @note       INTERNAL
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  attached : true if AttachJNIEnv attached the current thread.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XANDROIDPROCESSMANAGER::DetachJNIEnv(bool attached)
{
  if(!attached) return;
  if(!androidapplication) return;
  if(!androidapplication->activity) return;
  if(!androidapplication->activity->vm) return;

  androidapplication->activity->vm->DetachCurrentThread();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XANDROIDPROCESSMANAGER::Clean()
* @brief      Clean
* @note       INTERNAL
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XANDROIDPROCESSMANAGER::Clean()
{
  androidapplication = NULL;
}
