/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XLINUXTrace.h
* 
* @class      XLINUXTRACE
* @brief      LINUX eXtended Utils debug trace class
* @ingroup    PLATFORM_LINUX
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

#include <pthread.h>

#include "XBase.h"
#include "XString.h"
#include "DIOURL.h"

#include "XLINUXFactory.h"
#include "XLINUXDateTime.h"

#include "XTrace.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XLINUXTRACE : public XTRACE
{
  public:
                          XLINUXTRACE       ();
    virtual              ~XLINUXTRACE       ();

    void                  PrintSpecial          (XTRACE_TARGET* target, XBYTE level, XCHAR* string);
    void                  PrintFile             (XTRACE_TARGET* target, XBYTE level, XCHAR* string);
    void                  PrintNet              (XTRACE_TARGET* target, XBYTE level, XCHAR* string);

    bool                  GetHandleNet          (XTRACE_TARGET* target);
    bool                  CloseHandleNet        (XTRACE_TARGET* target);


  protected:

    bool                  Lock                  ();
    bool                  UnLock                ();

    XLINUXDATETIME        xdatetime;
    pthread_mutexattr_t   attr;
    pthread_mutex_t       mutex;
    XDWORD                mutexhandle;


  private:

    void                  Clean                 ()
                          {
                            mutexhandle  = 0;
                          }
};




#endif



