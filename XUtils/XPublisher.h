/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XPublisher.h
* 
* @class      XPUBLISHER
* @brief      eXtended Utils Publisher class
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

#include "XVector.h"
#include "XMap.h"
#include "XThread.h"
#include "XEvent.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/




/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class  XOBSERVER;
class  XSUBJECT;
class  XPUBLISHER;

class XPUBLISHERENTRY
{
  public:
                                        XPUBLISHERENTRY             ();
    virtual                            ~XPUBLISHERENTRY             ();

    XDWORD                              type;
    XSUBJECT*                           subject;
    XOBSERVER*                          observer;

  private:

    void                                Clean                       ();
};

class XPUBLISHER
{
  public:

    static bool                         GetIsInstanced              ();
    static XPUBLISHER&                  GetInstance                 ();
    static bool                         DelInstance                 ();

    bool                                RegisterEvent               (XDWORD type, XSUBJECT* subject);
    bool                                DeRegisterEvent             (XDWORD type, XSUBJECT* subject);
    bool                                DeRegisterAllEvents         ();

    bool                                SubscribeEvent              (XDWORD type, XSUBJECT* subject, XOBSERVER* observer);
    bool                                UnSubscribeEvent            (XDWORD type, XSUBJECT* subject, XOBSERVER* observer);

    void                                Active                      (bool on);
    bool                                IsInEvent                   ();

    bool                                PostEvent                   (XEVENT* event, XSUBJECT* subject);

  private:
                                        XPUBLISHER                  ();
                                        XPUBLISHER                  (XPUBLISHER const&);
    virtual                            ~XPUBLISHER                  ();

    void                                operator =                  (XPUBLISHER const&);

    void                                Clean                       ();

    static XPUBLISHER*                  instance;

    bool                                on;
    bool                                inevent;
    XMUTEX*                             xmutexevents;
    XMAP<XDWORD, XSUBJECT*>             subjectsmap;
    XVECTOR<XPUBLISHERENTRY*>           eventsvector;
};


/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





