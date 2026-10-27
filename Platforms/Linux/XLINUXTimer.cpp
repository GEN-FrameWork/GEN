/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XLINUXTimer.cpp
* 
* @class      XLINUXTIMER
* @brief      LINUX eXtended Utils Timer class
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XLINUXTimer.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <cmath>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/rtc.h>

#include "XTrace.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XLINUXTIMER::XLINUXTIMER()
* @brief      Constructor of class
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
XLINUXTIMER::XLINUXTIMER()
{
  Reset();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XLINUXTIMER::~XLINUXTIMER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
XLINUXTIMER::~XLINUXTIMER()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XQWORD XLINUXTIMER::GetMicroSecondsTickCounter()
* @brief      Get micro seconds tick counter
* @ingroup    PLATFORM_LINUX
* 
* @return     XQWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XQWORD XLINUXTIMER::GetMicroSecondsTickCounter()
{ 
  XQWORD elapsedtime = 0;
  struct timespec t1;

  //#ifdef CLOCK_MONOTONIC
  clock_gettime(CLOCK_MONOTONIC, &t1);
  //#else
  //clock_gettime(CLOCK_REALTIME, &t1);
  //#endif

  elapsedtime = (XQWORD)(((double)(t1.tv_sec) * 1000000) + (double)(t1.tv_nsec/1000)); // to us

  return elapsedtime;
}



