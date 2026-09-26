/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOWINDOWSStreamUSB.h
* 
* @class      DIOWINDOWSSTREAMUSB
* @brief      WINDOWS Data Input/Output Stream USB class
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


#if defined(DIO_ACTIVE) && defined(DIO_STREAMUSB_ACTIVE)


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/


#include <windows.h>

#include "XFSMachine.h"
#include "XThreadCollected.h"

#include "DIOStreamUSB.h"
#include "DIOStreamUSBConfig.h"




/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum DIOWINDOWSUSBFSMEVENTS
{
  DIOWINDOWSUSBFSMEVENT_NONE                = 0 ,
  DIOWINDOWSUSBFSMEVENT_CONNECTED             ,
  DIOWINDOWSUSBFSMEVENT_WAITINGTOREAD         ,
  DIOWINDOWSUSBFSMEVENT_SENDINGDATA           ,
  DIOWINDOWSUSBFSMEVENT_DISCONNECTING         ,

  DIOWINDOWSUSB_LASTEVENT
};

enum DIOWINDOWSUSBFSMSTATES
{
  DIOWINDOWSUSBFSMSTATE_NONE                = 0 ,
  DIOWINDOWSUSBFSMSTATE_CONNECTED             ,
  DIOWINDOWSUSBFSMSTATE_WAITINGTOREAD         ,
  DIOWINDOWSUSBFSMSTATE_SENDINGDATA           ,
  DIOWINDOWSUSBFSMSTATE_DISCONNECTING         ,

  DIOWINDOWSUSB_LASTSTATE
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOWINDOWSSTREAMUSB : public DIOSTREAMUSB , public XFSMACHINE
{
  public:
                          DIOWINDOWSSTREAMUSB                           ();
    virtual              ~DIOWINDOWSSTREAMUSB                           ();

    DIOSTREAMSTATUS       GetStatus                              ();

    bool                  Open                                          ();
    bool                  Close                                         ();

    bool                  CleanBuffers                                  ();

  protected:

    static void           ThreadConnection                              (void* data);

    XDWORD                ReadBuffer                                    (XBYTE* buffer, XDWORD size);
    XDWORD                WriteBuffer                                   (XBYTE* buffer, XDWORD size);

    void                  Clean                                         ();

    XTHREADCOLLECTED*     threadconnection;

    HANDLE                handle;
    XDWORD                mask;
    HANDLE                hevent;
    OVERLAPPED            ovi;

    int                   readtimeout;
    int                   writetimeout;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




#endif



