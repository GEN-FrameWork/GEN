/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XThread.h
* 
* @class      XTHREAD
* @brief      eXtended Utils Thread class
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

#include <stdio.h>

#include "XString.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum XTHREADSTATUS
{
  XTHREADSTATUS_NONE                                  ,
  XTHREADSTATUS_EXIT                                  ,
  XTHREADSTATUS_STOP                                  ,
  XTHREADSTATUS_RUN                                   ,
  XTHREADSTATUS_END
};


enum XTHREADPRIORITY
{
  XTHREADPRIORITY_UNKNOWN                         = 0 ,
  XTHREADPRIORITY_LOW                                 ,
  XTHREADPRIORITY_NORMAL                              ,
  XTHREADPRIORITY_HIGH                                ,
  XTHREADPRIORITY_REALTIME                            ,
};


enum XTHREADGROUPID
{
  XTHREADGROUPID_UNKNOWN                          = 0 ,
  XTHREADGROUPID_UNGROUP                              ,
  XTHREADGROUPID_XTIMERCLOCK                          ,
  XTHREADGROUPID_SCHEDULER                            ,
  XTHREADGROUPID_SCRIPT                               ,
  XTHREADGROUPID_DIODNSRESOLVED                       ,
  XTHREADGROUPID_DIOSTREAM                            ,
  XTHREADGROUPID_DIOSTREAMUART                        ,
  XTHREADGROUPID_DIOSTREAMUDP                         ,
  XTHREADGROUPID_DIOSTREAMTCPIP                       ,
  XTHREADGROUPID_DIOSTREAMWIFI                        ,
  XTHREADGROUPID_DIOSTREAMBLUETOOTH                   ,
  XTHREADGROUPID_DIOSTREAMUSB                         ,
  XTHREADGROUPID_DIOSTREAMICMP                        ,
  XTHREADGROUPID_DIOSTREAMI2C                         ,
  XTHREADGROUPID_DIOSTREAMSPI                         ,
  XTHREADGROUPID_DIOSTREAMCIPHER                      ,
  XTHREADGROUPID_DIOSTREAMTLS                         ,
  XTHREADGROUPID_DIOSSHREVERSE                        ,
  XTHREADGROUPID_DIOCHECKTCPIPCONNECTIONS             ,
  XTHREADGROUPID_DIOPROTOCOL                          ,
  XTHREADGROUPID_DIOPROTOCOL_CONNECTIONMANAGER        ,
  XTHREADGROUPID_DIOCOREPROTOCOL_CONNECTIONMANAGER    ,
  XTHREADGROUPID_DIOPROTOCOL_CLI_BUS                  ,
  XTHREADGROUPID_DIOWEBSERVER                         ,
  XTHREADGROUPID_DIOATCMS                             ,
  XTHREADGROUPID_DIOALERTS                            ,
  XTHREADGROUPID_DIOPCAP                              , 
  XTHREADGROUPID_DIONEOPIXEL                          ,
  XTHREADGROUPID_DIOLINUXDBUS                         ,
  XTHREADGROUPID_VIDEO                                ,
  XTHREADGROUPID_GRPLOADER                            ,
  XTHREADGROUPID_GRPFRAMERATE                         ,
  XTHREADGROUPID_SOUND                                ,
  XTHREADGROUPID_APPFLOWEXTENDED                      ,                         
 
  XTHREADGROUPID_APPOWNER
};

#define XTHREAD_TIMEOUT                     60   // seconds
#define XTHREAD_DEFAULTWAITYIELD            10   // milliseconds

#define XTHREAD_DEFAULTTIMEOUTEND           30   // seconds

typedef void (*XTHREADFUNCTION)(void* data);




/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XMUTEX 
{
  public:
                                  XMUTEX            ();
    virtual                      ~XMUTEX            ();

    bool                          IsLock            ();

    virtual bool                  Lock              ();
    virtual bool                  UnLock            ();

  protected:

    bool                          islock;

  private:

    void                          Clean             ();
};

class XTHREAD 
{
  public:
                                  XTHREAD           (XTHREADGROUPID groupID, XCHAR* ID, XTHREADFUNCTION function = NULL, void* data = NULL);
    virtual                      ~XTHREAD           ();

    virtual bool                  Ini               (bool run = true);
    virtual bool                  Wait              (int miliseconds = XTHREAD_DEFAULTWAITYIELD);

    virtual bool                  End               ();

    bool                          IsRunning         ();

    bool                          Run               (bool activate);

    virtual bool                  WaitToEnd         (XDWORD timeout = XTHREAD_DEFAULTTIMEOUTEND);

    XDWORD                        GetWaitYield      ();
    void                          SetWaitYield      (XDWORD waityield = XTHREAD_DEFAULTWAITYIELD);

    bool                          Exit              ();

    XTHREADGROUPID                GetGroupID        ();
    void                          SetGroupID        (XTHREADGROUPID groupID);

    XSTRING*                      GetID             ();
    XTHREADFUNCTION               GetFunction       ();
    void*                         GetParam          ();

    XTHREADPRIORITY               GetPriority       ();
    void                          SetPriority       (XTHREADPRIORITY priority);

    XDWORD                        GetStackSize      ();
    void                          SetStackSize      (XDWORD stacksize);

    bool                          IsInFunction      ();
    XTHREADSTATUS                 GetStatusFunc     ();

  protected:

    XTHREADGROUPID                groupID;
    XSTRING                       ID;
    XTHREADFUNCTION               function;
    void*                         param;

    XDWORD                        waityield;

    XTHREADPRIORITY               priority;
    XDWORD                        stacksize;

    bool                          gotofunction;
    bool                          isinfunction;
    XTHREADSTATUS                 statusfunc;

  private:

    void                          Clean             ();
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





