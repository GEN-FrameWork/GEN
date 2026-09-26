/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOLINUXStreamTCPIPServer.h
* 
* @class      DIOLINUXSTREAMTCPIPSERVER
* @brief      LINUX Data Input/Output Stream TCP/IP Server class
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

#include "DIOStreamTCPIPServer.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum DIOLINUXTCPIPSERVERFSMEVENTS
{
  DIOLINUXTCPIPSERVERFSMEVENT_NONE                = 0 ,

  DIOLINUXTCPIPSERVERFSMEVENT_GETTINGCONNECTION       ,
  DIOLINUXTCPIPSERVERFSMEVENT_CONNECTED               ,
  DIOLINUXTCPIPSERVERFSMEVENT_WAITINGTOREAD           ,
  DIOLINUXTCPIPSERVERFSMEVENT_SENDINGDATA             ,
  DIOLINUXTCPIPSERVERFSMEVENT_DISCONNECTING           ,

  DIOLINUXTCPIPSERVER_LASTEVENT
};

enum DIOLINUXTCPIPSERVERFSMSTATES
{
  DIOLINUXTCPIPSERVERFSMSTATE_NONE                = 0 ,

  DIOLINUXTCPIPSERVERFSMSTATE_GETTINGCONNECTION       ,
  DIOLINUXTCPIPSERVERFSMSTATE_CONNECTED               ,
  DIOLINUXTCPIPSERVERFSMSTATE_WAITINGTOREAD           ,
  DIOLINUXTCPIPSERVERFSMSTATE_SENDINGDATA             ,
  DIOLINUXTCPIPSERVERFSMSTATE_DISCONNECTING           ,

  DIOLINUXTCPIPSERVER_LASTSTATE
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOLINUXSTREAMTCPIPSERVER : public DIOSTREAMTCPIPSERVER , public XFSMACHINE
{
  public:
                              DIOLINUXSTREAMTCPIPSERVER               ();
    virtual                  ~DIOLINUXSTREAMTCPIPSERVER               ();

    bool                      Open                                    ();
    bool                      Disconnect                              ();
    bool                      Close                                   ();

  protected:

    int                       Accept                                  (int socket, void* addr, void* addrlen, XDWORD usec);
    int                       IsReadyConnect                          (int socket);

  private:

    DIOSTREAM*                CreateStream                            ();       
    bool                      DeleteAllStreamDisconnected             ();
    bool                      DeleteAllStream                         ();

    bool                      GetHandleServer                         (DIOSTREAMTCPIP* diostream);

    static void               ThreadConnection                        (void* data); 

    void                      Clean                                   ();

    XTHREADCOLLECTED*         threadconnection;    
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





