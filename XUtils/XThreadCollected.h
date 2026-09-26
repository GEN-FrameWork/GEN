/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XThreadCollected.h
* 
* @class      XTHREADCOLLECTED
* @brief      eXtended Utils Thread Collected class
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


#ifdef XTHREADCOLLECTED_ACTIVE

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>

#include "XFactory.h"
#include "XSleep.h"
#include "XString.h"
#include "XVector.h"
#include "XTimer.h"
#include "XThread.h"
#include "XTrace.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define CREATEXTHREAD               XTHREADSCOLLECTEDMANAGER::GetInstance().CreateThread
#define DELETEXTHREAD               XTHREADSCOLLECTEDMANAGER::GetInstance().DeleteThread

#define ACTIVATEXTHREADGROUP(id)    (void)XTHREADSCOLLECTEDMANAGER::GetInstance().ActivateGroup(id)
#define DEACTIVATEXTHREADGROUP(id)  (void)XTHREADSCOLLECTEDMANAGER::GetInstance().DeactivateGroup(id)

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XTHREADCOLLECTED
{
  public:
                                            XTHREADCOLLECTED                ();
                                            XTHREADCOLLECTED                (XTHREADGROUPID groupID, XCHAR* ID, XTHREADFUNCTION function, void* param);
    virtual                                ~XTHREADCOLLECTED                ();

    bool                                    Ini                             (bool run = true);

    void                                    Wait                            (int milliseconds = XTHREAD_DEFAULTWAITYIELD);

    bool                                    End                             ();

    bool                                    IsRunning                       ();

    bool                                    Run                             (bool activate);

    bool                                    Exit                            ();

    XTHREADGROUPID                          GetGroupID                      ();
    void                                    SetGroupID                      (XTHREADGROUPID groupID);

    XSTRING*                                GetID                           ();

    XTHREADFUNCTION                         GetFunction                     ();

    void                                    SetFunction                     (XTHREADFUNCTION function);

    void*                                   GetParam                        ();
    void                                    SetParam                        (void* param);

     XDWORD                                 GetWaitYield                    ();
     void                                   SetWaitYield                    (XDWORD waityield = XTHREAD_DEFAULTWAITYIELD);

    XTHREADPRIORITY                         GetPriority                     ();
    void                                    SetPriority                     (XTHREADPRIORITY priority);

    XDWORD                                  GetStackSize                    ();
    void                                    SetStackSize                    (XDWORD stacksize);

    bool                                    IsInFunction                    ();
    void                                    SetIsInFunction                 (bool isinfunction);

    XTHREADSTATUS                           GetStatus                       ();
    void                                    SetStatus                       (XTHREADSTATUS status);

  private:

    bool                                    WaitToEnd                       ();

    void                                    Clean                           ();

    XTHREADGROUPID                          groupID;
    XSTRING                                 ID;
    XTHREADFUNCTION                         function;
    void*                                   param;

    XDWORD                                  waityield;

    XTHREADPRIORITY                         priority;
    XDWORD                                  stacksize;

    bool                                    isinfunction;
    XTHREADSTATUS                           status;

    XTIMER*                                 xtimerout;
    XTHREAD*                                directxthread;
};

class XTHREADSCOLLECTED
{
  public:
                                            XTHREADSCOLLECTED               (XTHREADGROUPID groupID);
                                            XTHREADSCOLLECTED               ();
    virtual                                ~XTHREADSCOLLECTED               ();

    XTHREADGROUPID                          GetGroupID                      ();
    void                                    SetGroupID                      (XTHREADGROUPID groupID);

    XTHREADCOLLECTED*                       Create                          (XTHREADGROUPID groupID, XCHAR* ID, XTHREADFUNCTION function, void* param);

    bool                                    Delete                          (XTHREADGROUPID groupID, XTHREADCOLLECTED* xthreadcollected);
    bool                                    ExitAll                         ();

    bool                                    DeleteAll                       ();

  private:

    static void                             ThreadRunFunction               (void* param);
    bool                                    GetGroupThreadIDString          (XSTRING& stringID);

    void                                    Clean                           ();

    XTHREADGROUPID                          groupID;
    XMUTEX*                                 xthreadsvectormutex;
    XVECTOR<XTHREADCOLLECTED*>              xthreadsvector;
    XTHREAD*                                xthread;
};

class XTHREADSCOLLECTEDMANAGER
{
  public:

    static XTHREADSCOLLECTEDMANAGER&        GetInstance                     ();
    static bool                             DelInstance                     ();

    bool                                    ActivateGroup                   (XTHREADGROUPID groupID);
    bool                                    DeactivateGroup                 (XTHREADGROUPID groupID);
    bool                                    IsActivateGroup                 (XTHREADGROUPID groupID);

    XTHREADSCOLLECTED*                      GetThreadsCollectedByGroupID    (XTHREADGROUPID groupID);

    XTHREADCOLLECTED*                       CreateThread                    (XTHREADGROUPID groupID, XCHAR* ID, XTHREADFUNCTION function, void* param);

    bool                                    DeleteThread                    (XTHREADGROUPID groupID, XTHREADCOLLECTED* xthreadcollected);
    bool                                    DeleteAll                       ();

  private:
                                            XTHREADSCOLLECTEDMANAGER        ();
    virtual                                ~XTHREADSCOLLECTEDMANAGER        ();

    void                                    Clean                           ();

    static XTHREADSCOLLECTEDMANAGER*        instance;

    XTHREAD*                                nogroupthread;
    XVECTOR<XTHREADGROUPID>                 groupthreadsactivate;
    XMUTEX*                                 groupthreadsmutex;
    XVECTOR<XTHREADSCOLLECTED*>             groupthreadsvector;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




#else

  #include "XThread.h"

  #define CREATEXTHREAD               GEN_XFACTORY.CreateThread
  #define DELETEXTHREAD               GEN_XFACTORY.DeleteThread

  #define XTHREADCOLLECTED            XTHREAD

  #define ACTIVATEXTHREADGROUP(id)    (void)(id)
  #define DEACTIVATEXTHREADGROUP(id)  (void)(id)

#endif



