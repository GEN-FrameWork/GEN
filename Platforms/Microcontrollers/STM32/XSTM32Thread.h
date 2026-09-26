/**-------------------------------------------------------------------------------------------------------------------
*
* @file       XSTM32Thread.h
*
* @class      XSTM32THREAD
* @brief      eXtended utils STM32 mutex & thread class
* @ingroup    PLATFORM_STM32
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

#include "XThread.h"


/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XSTM32MUTEX : public XMUTEX
{
  public:

                          XSTM32MUTEX               ();
    virtual              ~XSTM32MUTEX               ();

    virtual bool          Lock                      ();
    virtual bool          UnLock                    ();

  protected:

     void                 Clean                     ();

     XDWORD               counterlock;
};



class XSTM32THREAD  : public XTHREAD
{
  public:
                          XSTM32THREAD              (XTHREADGROUPID groupID, XCHAR* ID, XTHREADFUNCTION function = NULL, void* data = NULL);
     virtual             ~XSTM32THREAD              ();

     virtual bool         Ini                       (bool run = true);
     virtual bool         Wait                      (int miliseconds = XTHREAD_DEFAULTWAITYIELD);
     virtual bool         End                       ();


  private:

     void                 Clean                     ();
};

/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/



