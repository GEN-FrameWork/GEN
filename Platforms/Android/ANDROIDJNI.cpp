/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       ANDROIDJNI.cpp
* 
* @class      ANDROIDJNI
* @brief      ANDROID JNI Interface class
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

#include "ANDROIDJNI.h"

#include "android_native_app_glue.h"

#include "XString.h"
#include "XTrace.h"

//#pragma endregion


/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

JavaVM*   ANDROIDJNI::currentjavaVM;
jint      ANDROIDJNI::currentjavaversion;
jobject   ANDROIDJNI::ClassLoader;
jmethodID ANDROIDJNI::FindClassMethod;



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void JavaEnvDestructor(void*)
* @brief      Thread-local key destructor: detaches the calling thread's JNIEnv when the thread ends
* @ingroup    PLATFORM_ANDROID
*
* @param[in]  (unnamed) : Unused, required by the pthread_key_create() destructor signature.
*
* --------------------------------------------------------------------------------------------------------------------*/
static void JavaEnvDestructor(void*)
{
  ANDROIDJNI::DetachJNIEnv();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void ANDROIDJNI::InitializeJNI(android_app* application, jint javaversion)
* @brief      Initialize JNI
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  application : Application pointer to use.
* @param[in]  javaversion : Javaversion value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void ANDROIDJNI::InitializeJNI(android_app* application, jint javaversion)
{
  currentjavaVM       = application->activity->vm;
  currentjavaversion  = javaversion;

  JNIEnv* env = ANDROIDJNI::GetJNIEnv();
  if(!env)
    {
      return;
    }

  jobject   mainactivity  = application->activity->clazz;
  
  jclass    MainClass             = env->GetObjectClass(mainactivity);
  jclass    classClass            = env->FindClass("java/lang/Class");
  jclass    classLoaderClass      = env->FindClass("java/lang/ClassLoader");
  jmethodID getClassLoaderMethod  = env->GetMethodID(classClass, "getClassLoader", "()Ljava/lang/ClassLoader;");
  jobject   classLoader           = env->CallObjectMethod(MainClass, getClassLoaderMethod);

  ClassLoader = env->NewGlobalRef(classLoader);

  FindClassMethod = env->GetMethodID(classLoaderClass, "findClass", "(Ljava/lang/String;)Ljava/lang/Class;");
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         JNIEnv* ANDROIDJNI::GetJNIEnv()
* @brief      Get JNI env
* @ingroup    PLATFORM_ANDROID
* 
* @return     JNIEnv* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
JNIEnv* ANDROIDJNI::GetJNIEnv()
{
  static XDWORD threadslot = 0;
  if(!threadslot)
    {
      pthread_key_create((pthread_key_t*)&threadslot, &JavaEnvDestructor);
    }

  JNIEnv* env = NULL;

  if(!currentjavaVM)
    {
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[ANDROID] No Current Java VM"));
    }

  jint getenvresult = currentjavaVM->GetEnv((void**)&env, currentjavaversion);
  if(getenvresult == JNI_EDETACHED)
    {
      jint attachresult = currentjavaVM->AttachCurrentThread(&env, NULL);

      if(attachresult == JNI_ERR)
        {
          return NULL;
        }
    }
   else 
    {
      if(getenvresult != JNI_OK)
        {
          return NULL;
        }
    }

  return env;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void ANDROIDJNI::DetachJNIEnv()
* @brief      Detach JNI env
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
void ANDROIDJNI::DetachJNIEnv()
{
  currentjavaVM->DetachCurrentThread();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         jclass ANDROIDJNI::FindJNIClass(const XSTRING& name)
* @brief      Find JNI class
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  name : Name to use.
* 
* @return     jclass : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
jclass ANDROIDJNI::FindJNIClass(const XSTRING& name)
{
  XBUFFER cname;
  XSTRING _name;

  _name = name;

  (XSTRING)_name.ConvertToASCII(cname);

  return ANDROIDJNI::FindJNIClass((const char*)cname.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         jclass ANDROIDJNI::FindJNIClass(XSTRING* name)
* @brief      Find JNI class
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  name : Name to use.
* 
* @return     jclass : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
jclass ANDROIDJNI::FindJNIClass(XSTRING* name)
{
  return ANDROIDJNI::FindJNIClass(*name);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         jclass ANDROIDJNI::FindJNIClass(XCHAR* name)
* @brief      Find JNI class
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  name : Name to use.
* 
* @return     jclass : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
jclass ANDROIDJNI::FindJNIClass(XCHAR* name)
{
  return ANDROIDJNI::FindJNIClass(XSTRING(name));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         jclass ANDROIDJNI::FindJNIClass(const char* name)
* @brief      Find JNI class
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  name : Name to use.
* 
* @return     jclass : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
jclass ANDROIDJNI::FindJNIClass(const char* name)
{
  JNIEnv* env = ANDROIDJNI::GetJNIEnv();
  if(!env)
    {
      return NULL;
    }

  jstring ClassNameObj = env->NewStringUTF(name);
  jclass FoundClass = static_cast<jclass>(env->CallObjectMethod(ClassLoader, FindClassMethod, ClassNameObj));

  ANDROIDJNI::CheckJavaException();
  env->DeleteLocalRef(ClassNameObj);

  return FoundClass;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         jclass ANDROIDJNI::FindJNIEnvClassWrapper(const char* name)
* @brief      Find JNI env class wrapper
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  name : Name to use.
* 
* @return     jclass : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
jclass ANDROIDJNI::FindJNIEnvClassWrapper(const char* name)
{
  JNIEnv* env = ANDROIDJNI::GetJNIEnv();
  if(!env)
    {
      return NULL;
    }

  return env->FindClass(name);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool ANDROIDJNI::CheckJavaException()
* @brief      Check java exception
* @ingroup    PLATFORM_ANDROID
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool ANDROIDJNI::CheckJavaException()
{
  JNIEnv* env = ANDROIDJNI::GetJNIEnv();
  if(!env)
    {
      return true;
    }

  if(env->ExceptionCheck())
    {
      env->ExceptionDescribe();

      env->ExceptionClear();
      return true;
    }

  return false;
}


