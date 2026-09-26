/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       ANDROIDJNI.h
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include <jni.h>

#include "XString.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

struct android_app;

class ANDROIDJNI
{
  public:

    static void                     InitializeJNI             (android_app* application, jint javaversion);

    static JNIEnv*                  GetJNIEnv                 ();
    static void                     DetachJNIEnv              ();

    static jclass                   FindJNIClass              (const XSTRING & name);
    static jclass                   FindJNIClass              (XSTRING* name);
    static jclass                   FindJNIClass              (XCHAR* name);
    static jclass                   FindJNIClass              (const char * name);
    static jclass                   FindJNIEnvClassWrapper    (const char * name);

    static bool                     CheckJavaException        ();

  private:

    static JavaVM*                  currentjavaVM;
    static jint                     currentjavaversion;
    static jobject                  ClassLoader;
    static jmethodID                FindClassMethod;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





