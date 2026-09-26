/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XLINUXThread.h
* 
* @class      XLINUXTHREAD
* @brief      LINUX eXtended Utils Thread and Mutex class
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

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <malloc.h>
#include <errno.h>
#include <pthread.h>

#include "XThread.h"


/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/



/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XLINUXMUTEX : public XMUTEX
{
  public:
                          XLINUXMUTEX           ();
    virtual              ~XLINUXMUTEX           ();

    bool                  Lock                  ();
    bool                  UnLock                ();

  private:

    void                  Clean                 ();

    bool                  init;
    pthread_mutex_t       mutex;
    pthread_mutexattr_t   attr;
};


class XLINUXTHREAD  : public XTHREAD
{
  public:
                         XLINUXTHREAD          (XTHREADGROUPID groupID, XCHAR* ID, XTHREADFUNCTION function = NULL, void* param = NULL);
    virtual             ~XLINUXTHREAD          ();

    bool                 Ini                   (bool run = true);
    bool                 Wait                  (int miliseconds = XTHREAD_DEFAULTWAITYIELD);
    bool                 End                   ();


  private:
    
    static void*         Callback              (void* thread);

    void                 Clean                 ();

    pthread_t            thhandle;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





