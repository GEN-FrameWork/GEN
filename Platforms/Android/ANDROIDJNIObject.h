/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       ANDROIDJNIObject.h
* 
* @class      ANDROIDJNIOBJECT
* @brief      ANDROID JNI Object class 
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

struct ANDROIDJNIMETHOD
{
  XSTRING   name;
  XSTRING   signature;
  jmethodID method;
};

class ANDROIDJNIOBJECT
{
  public:

                                ANDROIDJNIOBJECT        (XSTRING name, XSTRING constructorsignature, ...);
    virtual                    ~ANDROIDJNIOBJECT        ();

    ANDROIDJNIMETHOD            GetClassMethod          (XSTRING method, XSTRING signature);

    template<typename T>
    T                           CallMethod              (ANDROIDJNIMETHOD method, ...);

    jobject                     GetObject               ()                                        
                                { 
                                  return jniobject; 
                                }

    static jstring              GetJString              (const XSTRING& str);

  protected:

    jobject                     jniobject;
    jclass                      jniclass;

  private:

    void                        Clean()
                                {
                                  jniobject = 0;
                                  jniclass  = 0;
                                }

                                ANDROIDJNIOBJECT        (const ANDROIDJNIOBJECT& rhs);
  ANDROIDJNIOBJECT&             operator =              (const ANDROIDJNIOBJECT& rhs);

};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





