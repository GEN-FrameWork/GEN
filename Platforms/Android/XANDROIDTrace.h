/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XANDROIDTrace.h
* 
* @class      XANDROIDTRACE
* @brief      ANDROID eXtended Debug Trace class
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


#ifdef XTRACE_ACTIVE

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include <android/log.h>

#include "XLINUXTrace.h"

#include "VersionFrameWork.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define XANDROIDDEBUG_LOG_INFO(...)       __android_log_print(ANDROID_LOG_INFO, VERSIONFRAMEWORK_NAME, "%s", __VA_ARGS__)
#define XANDROIDDEBUG_LOG_ERROR(...)      __android_log_print(ANDROID_LOG_INFO, VERSIONFRAMEWORK_NAME, "%s", __VA_ARGS__)
#define XANDROIDDEBUG_LOG_WARNING(...)    __android_log_print(ANDROID_LOG_INFO, VERSIONFRAMEWORK_NAME, "%s", __VA_ARGS__)
#define XANDROIDDEBUG_LOG_DEBUG(...)      __android_log_print(ANDROID_LOG_INFO, VERSIONFRAMEWORK_NAME, "%s", __VA_ARGS__)



/*---- CLASS ---------------------------------------------------------------------------------------------------------*/


class XANDROIDTRACE : public XLINUXTRACE
{
  public:
                          XANDROIDTRACE      ();
    virtual              ~XANDROIDTRACE      ();

    void                  PrintSpecial            (XTRACE_TARGET* target, XBYTE level, XCHAR* string);
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




#endif



