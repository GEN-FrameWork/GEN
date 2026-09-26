/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XScheduler.h
* 
* @class      XSCHEDULER
* @brief      eXtended Utils Scheduler class
* @ingroup    XUTILS
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

#include "XDateTime.h"
#include "XVector.h"
#include "XTimer.h"
#include "XSubject.h"
#include "XThreadCollected.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


#define XSCHEDULER_CYCLEFOREVER    -1

#define XSCHEDULER_DAYWEEK_NONE           0x00
#define XSCHEDULER_DAYWEEK_ISSUNDAY       0x01
#define XSCHEDULER_DAYWEEK_ISMONDAY       0x02
#define XSCHEDULER_DAYWEEK_ISTUESDAY      0x04
#define XSCHEDULER_DAYWEEK_ISWEDNESDAY    0x08
#define XSCHEDULER_DAYWEEK_ISTHURSDAY     0x10
#define XSCHEDULER_DAYWEEK_ISFRIDAY       0x20
#define XSCHEDULER_DAYWEEK_ISSATURDAY     0x40
#define XSCHEDULER_DAYWEEK_ISWORKDAY      (XSCHEDULER_DAYWEEK_ISMONDAY   | XSCHEDULER_DAYWEEK_ISTUESDAY |  XSCHEDULER_DAYWEEK_ISWEDNESDAY | XSCHEDULER_DAYWEEK_ISTHURSDAY | XSCHEDULER_DAYWEEK_ISFRIDAY)
#define XSCHEDULER_DAYWEEK_ISWEEKEND      (XSCHEDULER_DAYWEEK_ISSATURDAY | XSCHEDULER_DAYWEEK_ISSUNDAY)

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XPUBLISHER;
class XMUTEX;
class XTHREADCOLLECTED;
class XTIMER;
class XSCHEDULERTASK;
class XSCHEDULER;

class XSCHEDULERTASK
{
  public:
                                XSCHEDULERTASK                  (XSCHEDULER* xscheduler);
    virtual                    ~XSCHEDULERTASK                  ();

    bool                        IsActive                        ();
    bool                        SetIsActive                     (bool active);

    XDWORD                      GetID                           ();
    void                        SetID                           (XDWORD ID);

    XTIMER*                     GetXTimer                       ();

    bool                        IsInValidTimeLimit              ();
    XDATETIME*                  GetTimeLimitStart               ();
    XDATETIME*                  GetTimeLimitEnd                 ();

    bool                        IsCyclic                        ();
    int                         GetNCyclesTodo                  ();
    bool                        SetNCycles                      (int ncyclestodo = XSCHEDULER_CYCLEFOREVER, XDATETIME* xtimecadence = NULL);
    bool                        SetNCycles                      (int ncyclestodo, XQWORD cadenceinseconds);
    bool                        SetNCycles                      (int ncyclestodo, int cadenceinseconds);

    bool                        IsStartImmediatelyCycles        ();
    void                        SetIsStartImmediatelyCycles     (bool isstartimmediatelycycles);

    bool                        SetTimeLimits                   (XDATETIME* xtimelimitstart = NULL, XDATETIME* xtimelimitend = NULL);

    void                        SetConditionDayWeek             (XBYTE mask);

    bool                        ResetCondition                  ();
    bool                        StartConditionImmediately       ();
    
    virtual bool                CheckCondition                  (XDATETIME* xtimeactual, XTIMER* xtimeractual);

  private:

    XBYTE                       GetDayOfWeekMask                (XDATETIME* xtimeactual);
    void                        Clean                           ();

    XSCHEDULER*                 xscheduler;

    XTIMER*                     xtimer;

    XDWORD                      ID;

    bool                        active;

    bool                        isstartimmediatelycycles;
    int                         ncyclesmade;
    int                         ncyclesactual;
    int                         ncyclestodo;

    XDATETIME                   xdatetimecadence;

    XDATETIME                   xdatetimelimitstart;
    XDATETIME                   xdatetimelimitend;

    bool                        isinvalidtimelimit;

    XBYTE                       conditiondayweekmask;
};

class XSCHEDULER  : public XSUBJECT
{
  public:
                                XSCHEDULER                      ();
    virtual                    ~XSCHEDULER                      ();

    bool                        Ini                             ();

    bool                        IsActive                        ();
    void                        Activate                        (bool on);

    XMUTEX*                     GetMutexScheduler               ();
    XDATETIME*                  GetDateTimeActual               ();

    bool                        Task_Add                        (XSCHEDULERTASK* task);
    XSCHEDULERTASK*             Task_Get                        (int index);
    XSCHEDULERTASK*             Task_GetForID                   (XDWORD ID);
    bool                        Task_Del                        (int index);
    bool                        Task_DelForID                   (XDWORD ID);
    bool                        Task_DelAll                     ();

    bool                        End                             ();

  private:

    static void                 ThreadScheduler                 (void* data);

    void                        Clean                           ();

    XMUTEX*                     xmutexscheduler;
    XTHREADCOLLECTED*           xthreadscheduler;
    XTIMER*                     xtimerwait;  

    bool                        isactive;

    XDATETIME*                  xdatetimeactual;
    XVECTOR<XSCHEDULERTASK*>    tasks;
    int                         indextask;
};






/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





