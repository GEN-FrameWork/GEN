/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOWINDOWSStreamICMP.h
* 
* @class      DIOWINDOWSSTREAMICMP
* @brief      WINDOWS Data Input/Output Stream ICMP class
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


#if defined(DIO_ACTIVE) && defined(DIO_STREAMICMP_ACTIVE)

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/


#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include "XFSMachine.h"
#include "XThreadCollected.h"

#include "DIOStreamICMP.h"




/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum DIOWINDOWSICMPFSMEVENTS
{
  DIOWINDOWSICMPFSMEVENT_NONE                 = 0 ,

  DIOWINDOWSICMPFSMEVENT_GETTINGCONNECTION         ,
  DIOWINDOWSICMPFSMEVENT_CONNECTED                ,
  DIOWINDOWSICMPFSMEVENT_WAITINGTOREAD            ,
  DIOWINDOWSICMPFSMEVENT_SENDINGDATA              ,
  DIOWINDOWSICMPFSMEVENT_DISCONNECTING            ,

  DIOWINDOWSICMP_LASTEVENT
};

enum DIOWINDOWSICMPFSMSTATES
{
  DIOWINDOWSICMPFSMSTATE_NONE                 = 0 ,

  DIOWINDOWSICMPFSMSTATE_GETTINGCONNECTION         ,
  DIOWINDOWSICMPFSMSTATE_CONNECTED                ,
  DIOWINDOWSICMPFSMSTATE_WAITINGTOREAD            ,
  DIOWINDOWSICMPFSMSTATE_SENDINGDATA              ,
  DIOWINDOWSICMPFSMSTATE_DISCONNECTING            ,

  DIOWINDOWSICMP_LASTSTATE
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/


class DIOWINDOWSSTREAMICMP : public DIOSTREAMICMP , public XFSMACHINE
{
  public:
                              DIOWINDOWSSTREAMICMP                    ();
    virtual                  ~DIOWINDOWSSTREAMICMP                    ();

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

    bool                      hastimeperiodactive;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




#endif



