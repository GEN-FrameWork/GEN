/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOWINDOWSStreamUDP.h
* 
* @class      DIOWINDOWSSTREAMUDP
* @brief      WINDOWS Data Input/Output Stream UDP class
* @ingroup    PLATFORM_WINDOWS
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

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include "XFSMachine.h"
#include "XThreadCollected.h"

#include "DIOStreamUDP.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum DIOWINDOWSUDPFSMEVENTS
{
  DIOWINDOWSUDPFSMEVENT_NONE              = 0 ,

  DIOWINDOWSUDPFSMEVENT_GETTINGCONNECTION     ,
  DIOWINDOWSUDPFSMEVENT_CONNECTED             ,
  DIOWINDOWSUDPFSMEVENT_WAITINGTOREAD         ,
  DIOWINDOWSUDPFSMEVENT_SENDINGDATA           ,
  DIOWINDOWSUDPFSMEVENT_DISCONNECTING         ,

  DIOWINDOWSUDP_LASTEVENT
};

enum DIOWINDOWSUDPFSMSTATES
{
  DIOWINDOWSUDPFSMSTATE_NONE              = 0 ,

  DIOWINDOWSUDPFSMSTATE_GETTINGCONNECTION     ,
  DIOWINDOWSUDPFSMSTATE_CONNECTED             ,
  DIOWINDOWSUDPFSMSTATE_WAITINGTOREAD         ,
  DIOWINDOWSUDPFSMSTATE_SENDINGDATA           ,
  DIOWINDOWSUDPFSMSTATE_DISCONNECTING         ,

  DIOWINDOWSUDP_LASTSTATE
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XFACTORY;
class XPUBLISHER;
class DIOFACTORY;

class DIOWINDOWSSTREAMUDP : public DIOSTREAMUDP , public XFSMACHINE
{
  public:
                              DIOWINDOWSSTREAMUDP                     ();
    virtual                  ~DIOWINDOWSSTREAMUDP                     ();

    bool                      Open                                    ();
    bool                      Disconnect                              ();
    bool                      Close                                   ();

  protected:

    int                       IsReadyConnect                          (SOCKET socket);

  private:

    void                      Clean                                   ();
    static void               ThreadConnection                        (void* data);

    XTHREADCOLLECTED*         threadconnection;
    SOCKET                    handle;
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




