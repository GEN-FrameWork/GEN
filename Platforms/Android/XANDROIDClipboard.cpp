/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XANDROIDClipboard.cpp
* 
* @class      XANDROIDCLIPBOARD
* @brief      ANDROID eXtended Utils Clipboard class (JNI ClipboardManager)
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

#include "XANDROIDClipboard.h"

#include <android_native_app_glue.h>

#include "XBuffer.h"
#include "ANDROIDJNI.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XANDROIDCLIPBOARD::XANDROIDCLIPBOARD()
* @brief      Constructor of class
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
XANDROIDCLIPBOARD::XANDROIDCLIPBOARD()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XANDROIDCLIPBOARD::~XANDROIDCLIPBOARD()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
XANDROIDCLIPBOARD::~XANDROIDCLIPBOARD()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XANDROIDCLIPBOARD::SetAndroidApplication(android_app* application)
* @brief      Set android application
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  application : Native android_app pointer (activity / JNI).
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XANDROIDCLIPBOARD::SetAndroidApplication(android_app* application)
{
  androidapplication = application;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XANDROIDCLIPBOARD::SetText(XSTRING& text)
* @brief      Set text into the Android clipboard (ClipboardManager)
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  text : Text to place on the clipboard.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XANDROIDCLIPBOARD::SetText(XSTRING& text)
{
  JNIEnv* env      = NULL;
  bool    attached = false;
  if(!AttachJNIEnv(&env, attached)) return false;

  jobject clipboard = GetClipboardManager(env);
  if(!clipboard)
    {
      DetachJNIEnv(attached);
      return false;
    }

  XBUFFER utf8;
  if(!text.IsEmpty())
    {
      if(!text.ConvertToUTF8(utf8, true))
        {
          env->DeleteLocalRef(clipboard);
          DetachJNIEnv(attached);
          return false;
        }
    }

  jstring jlabel = env->NewStringUTF("GEN");
  jstring jtext  = env->NewStringUTF(utf8.GetSize()? (const char*)utf8.Get() : "");
  if(!jlabel || !jtext)
    {
      if(jlabel) env->DeleteLocalRef(jlabel);
      if(jtext)  env->DeleteLocalRef(jtext);
      env->DeleteLocalRef(clipboard);
      DetachJNIEnv(attached);
      return false;
    }

  jclass clipdataclass = env->FindClass("android/content/ClipData");
  if(!clipdataclass)
    {
      env->DeleteLocalRef(jlabel);
      env->DeleteLocalRef(jtext);
      env->DeleteLocalRef(clipboard);
      DetachJNIEnv(attached);
      return false;
    }

  jmethodID newplaintext = env->GetStaticMethodID(clipdataclass, "newPlainText", "(Ljava/lang/CharSequence;Ljava/lang/CharSequence;)Landroid/content/ClipData;");
  jobject   clipdata     = env->CallStaticObjectMethod(clipdataclass, newplaintext, jlabel, jtext);
  if(!clipdata || ANDROIDJNI::CheckJavaException())
    {
      env->DeleteLocalRef(clipdataclass);
      env->DeleteLocalRef(jlabel);
      env->DeleteLocalRef(jtext);
      env->DeleteLocalRef(clipboard);
      DetachJNIEnv(attached);
      return false;
    }

  jclass    clipboardclass = env->GetObjectClass(clipboard);
  jmethodID setprimaryclip = env->GetMethodID(clipboardclass, "setPrimaryClip", "(Landroid/content/ClipData;)V");
  env->CallVoidMethod(clipboard, setprimaryclip, clipdata);
  bool status = !ANDROIDJNI::CheckJavaException();

  env->DeleteLocalRef(clipboardclass);
  env->DeleteLocalRef(clipdata);
  env->DeleteLocalRef(clipdataclass);
  env->DeleteLocalRef(jlabel);
  env->DeleteLocalRef(jtext);
  env->DeleteLocalRef(clipboard);
  DetachJNIEnv(attached);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XANDROIDCLIPBOARD::GetText(XSTRING& text)
* @brief      Get text from the Android clipboard (ClipboardManager)
* @ingroup    PLATFORM_ANDROID
* 
* @param[out] text : Clipboard text (cleared on failure).
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XANDROIDCLIPBOARD::GetText(XSTRING& text)
{
  text.Empty();

  JNIEnv* env      = NULL;
  bool    attached = false;
  if(!AttachJNIEnv(&env, attached)) return false;

  jobject clipboard = GetClipboardManager(env);
  if(!clipboard)
    {
      DetachJNIEnv(attached);
      return false;
    }

  jclass    clipboardclass = env->GetObjectClass(clipboard);
  jmethodID gethasprimary  = env->GetMethodID(clipboardclass, "hasPrimaryClip", "()Z");
  jboolean  hasprimary     = env->CallBooleanMethod(clipboard, gethasprimary);
  if(!hasprimary || ANDROIDJNI::CheckJavaException())
    {
      env->DeleteLocalRef(clipboardclass);
      env->DeleteLocalRef(clipboard);
      DetachJNIEnv(attached);
      return false;
    }

  jmethodID getprimaryclip = env->GetMethodID(clipboardclass, "getPrimaryClip", "()Landroid/content/ClipData;");
  jobject   clipdata       = env->CallObjectMethod(clipboard, getprimaryclip);
  if(!clipdata || ANDROIDJNI::CheckJavaException())
    {
      env->DeleteLocalRef(clipboardclass);
      env->DeleteLocalRef(clipboard);
      DetachJNIEnv(attached);
      return false;
    }

  jclass    clipdataclass = env->GetObjectClass(clipdata);
  jmethodID getitemcount  = env->GetMethodID(clipdataclass, "getItemCount", "()I");
  jint      itemcount     = env->CallIntMethod(clipdata, getitemcount);
  if(itemcount <= 0)
    {
      env->DeleteLocalRef(clipdataclass);
      env->DeleteLocalRef(clipdata);
      env->DeleteLocalRef(clipboardclass);
      env->DeleteLocalRef(clipboard);
      DetachJNIEnv(attached);
      return false;
    }

  jmethodID getitem = env->GetMethodID(clipdataclass, "getItemAt", "(I)Landroid/content/ClipData$Item;");
  jobject   item    = env->CallObjectMethod(clipdata, getitem, 0);
  if(!item || ANDROIDJNI::CheckJavaException())
    {
      env->DeleteLocalRef(clipdataclass);
      env->DeleteLocalRef(clipdata);
      env->DeleteLocalRef(clipboardclass);
      env->DeleteLocalRef(clipboard);
      DetachJNIEnv(attached);
      return false;
    }

  jclass    itemclass      = env->GetObjectClass(item);
  jmethodID coercetotext   = env->GetMethodID(itemclass, "coerceToText", "(Landroid/content/Context;)Ljava/lang/CharSequence;");
  jobject   charsequence   = env->CallObjectMethod(item, coercetotext, androidapplication->activity->clazz);
  if(!charsequence || ANDROIDJNI::CheckJavaException())
    {
      env->DeleteLocalRef(itemclass);
      env->DeleteLocalRef(item);
      env->DeleteLocalRef(clipdataclass);
      env->DeleteLocalRef(clipdata);
      env->DeleteLocalRef(clipboardclass);
      env->DeleteLocalRef(clipboard);
      DetachJNIEnv(attached);
      return false;
    }

  jclass    charseqclass = env->GetObjectClass(charsequence);
  jmethodID tostring     = env->GetMethodID(charseqclass, "toString", "()Ljava/lang/String;");
  jstring   jtext        = (jstring)env->CallObjectMethod(charsequence, tostring);
  bool      status       = false;

  if(jtext && !ANDROIDJNI::CheckJavaException())
    {
      const char* utf8 = env->GetStringUTFChars(jtext, NULL);
      if(utf8)
        {
          text.Set(utf8);
          env->ReleaseStringUTFChars(jtext, utf8);
          status = true;
        }
    }

  if(jtext) env->DeleteLocalRef(jtext);
  env->DeleteLocalRef(charseqclass);
  env->DeleteLocalRef(charsequence);
  env->DeleteLocalRef(itemclass);
  env->DeleteLocalRef(item);
  env->DeleteLocalRef(clipdataclass);
  env->DeleteLocalRef(clipdata);
  env->DeleteLocalRef(clipboardclass);
  env->DeleteLocalRef(clipboard);
  DetachJNIEnv(attached);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XANDROIDCLIPBOARD::Clear()
* @brief      Clear the Android clipboard
* @ingroup    PLATFORM_ANDROID
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XANDROIDCLIPBOARD::Clear()
{
  XSTRING empty;
  return SetText(empty);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XANDROIDCLIPBOARD::HasText()
* @brief      Has text available on the Android clipboard
* @ingroup    PLATFORM_ANDROID
* 
* @return     bool : true if clipboard holds text; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XANDROIDCLIPBOARD::HasText()
{
  JNIEnv* env      = NULL;
  bool    attached = false;
  if(!AttachJNIEnv(&env, attached)) return false;

  jobject clipboard = GetClipboardManager(env);
  if(!clipboard)
    {
      DetachJNIEnv(attached);
      return false;
    }

  jclass    clipboardclass = env->GetObjectClass(clipboard);
  jmethodID gethasprimary  = env->GetMethodID(clipboardclass, "hasPrimaryClip", "()Z");
  jboolean  hasprimary     = env->CallBooleanMethod(clipboard, gethasprimary);
  bool      status         = (hasprimary && !ANDROIDJNI::CheckJavaException())?true:false;

  env->DeleteLocalRef(clipboardclass);
  env->DeleteLocalRef(clipboard);
  DetachJNIEnv(attached);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         jobject XANDROIDCLIPBOARD::GetClipboardManager(JNIEnv* env)
* @brief      Get clipboard manager
* @note       INTERNAL
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  env : JNI environment.
* 
* @return     jobject : ClipboardManager local reference, or NULL.
* 
* --------------------------------------------------------------------------------------------------------------------*/
jobject XANDROIDCLIPBOARD::GetClipboardManager(JNIEnv* env)
{
  if(!env) return NULL;
  if(!androidapplication) return NULL;
  if(!androidapplication->activity) return NULL;
  if(!androidapplication->activity->clazz) return NULL;

  jclass activityclass = env->GetObjectClass(androidapplication->activity->clazz);
  if(!activityclass) return NULL;

  jmethodID getsystemservice = env->GetMethodID(activityclass, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
  jstring   service          = env->NewStringUTF("clipboard");
  jobject   clipboard        = env->CallObjectMethod(androidapplication->activity->clazz, getsystemservice, service);

  env->DeleteLocalRef(service);
  env->DeleteLocalRef(activityclass);

  if(!clipboard || ANDROIDJNI::CheckJavaException()) return NULL;
  return clipboard;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XANDROIDCLIPBOARD::AttachJNIEnv(JNIEnv** env, bool& attached)
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
bool XANDROIDCLIPBOARD::AttachJNIEnv(JNIEnv** env, bool& attached)
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
* @fn         void XANDROIDCLIPBOARD::DetachJNIEnv(bool attached)
* @brief      Detach JNI env
* @note       INTERNAL
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  attached : true if AttachJNIEnv attached the current thread.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XANDROIDCLIPBOARD::DetachJNIEnv(bool attached)
{
  if(!attached) return;
  if(!androidapplication) return;
  if(!androidapplication->activity) return;
  if(!androidapplication->activity->vm) return;

  androidapplication->activity->vm->DetachCurrentThread();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XANDROIDCLIPBOARD::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XANDROIDCLIPBOARD::Clean()
{
  androidapplication = NULL;
}



