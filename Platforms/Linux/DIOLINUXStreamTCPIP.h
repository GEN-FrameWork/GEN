/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOLINUXStreamTCPIP.h
* 
* @class      DIOLINUXSTREAMTCPIP
* @brief      LINUX Data Input/Output Stream TCP/IP class
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

#include "XFSMachine.h"
#include "XThreadCollected.h"

#include "DIOStreamTCPIP.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum DIOLINUXTCPIPFSMEVENTS
{
  DIOLINUXTCPIPFSMEVENT_NONE                = 0 ,

  DIOLINUXTCPIPFSMEVENT_GETTINGCONNECTION       ,
  DIOLINUXTCPIPFSMEVENT_CONNECTED               ,
  DIOLINUXTCPIPFSMEVENT_WAITINGTOREAD           ,
  DIOLINUXTCPIPFSMEVENT_SENDINGDATA             ,
  DIOLINUXTCPIPFSMEVENT_DISCONNECTING           ,

  DIOLINUXTCPIP_LASTEVENT
};

enum DIOLINUXTCPIPFSMSTATES
{
  DIOLINUXTCPIPFSMSTATE_NONE                = 0 ,

  DIOLINUXTCPIPFSMSTATE_GETTINGCONNECTION       ,
  DIOLINUXTCPIPFSMSTATE_CONNECTED               ,
  DIOLINUXTCPIPFSMSTATE_WAITINGTOREAD           ,
  DIOLINUXTCPIPFSMSTATE_SENDINGDATA             ,
  DIOLINUXTCPIPFSMSTATE_DISCONNECTING           ,

  DIOLINUXTCPIP_LASTSTATE
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOLINUXSTREAMTCPIP : public DIOSTREAMTCPIP , public XFSMACHINE
{
  public:
                              DIOLINUXSTREAMTCPIP                     ();
    virtual                  ~DIOLINUXSTREAMTCPIP                     ();

    bool                      Open                                    ();
    bool                      Disconnect                              ();
    bool                      Close                                   ();

    int                       GetHandleSocket                         (); 
    void                      SetHandleSocket                         (int handlesocket); 

  protected:

    int                       Accept                                  (int socket, void* addr, void* addrlen, XDWORD usec);
    int                       IsReadyConnect                          (int socket);

  private:

    bool                      GetHandleServer                         ();
    bool                      GetHandleClient                         ();

    static void               ThreadConnection                        (void* data); 

    void                      Clean                                   ();

    XTHREADCOLLECTED*         threadconnection;

    int                       handlesocket;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





