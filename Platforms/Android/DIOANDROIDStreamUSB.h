/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOANDROIDStreamUSB.h
* 
* @class      DIOANDROIDSTREAMUSB
* @brief      ANDROID Data Input/Output Stream USB class
* @ingroup    PLATFORM_ANDROID
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

#include "XBuffer.h"
#include "XFSMachine.h"

#include "DIOStreamUSB.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum DIOANDROIDUSBFSMEVENTS
{
  DIOANDROIDUSBFSMEVENT_NONE               = 0  ,
  DIOANDROIDUSBFSMEVENT_CONNECTED             ,
  DIOANDROIDUSBFSMEVENT_WAITINGTOREAD         ,
  DIOANDROIDUSBFSMEVENT_SENDINGDATA           ,
  DIOANDROIDUSBFSMEVENT_DISCONNECTING         ,

  DIOANDROIDUSB_LASTEVENT
};

enum DIOANDROIDUSBFSMSTATES
{
  DIOANDROIDUSBFSMSTATE_NONE               = 0  ,

  DIOANDROIDUSBFSMSTATE_CONNECTED             ,
  DIOANDROIDUSBFSMSTATE_WAITINGTOREAD         ,
  DIOANDROIDUSBFSMSTATE_SENDINGDATA           ,
  DIOANDROIDUSBFSMSTATE_DISCONNECTING         ,

  DIOANDROIDUSB_LASTSTATE
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XTHREAD;

class DIOANDROIDSTREAMUSB : public DIOSTREAMUSB , public XFSMACHINE
{
  public:
                          DIOANDROIDSTREAMUSB           ();
    virtual              ~DIOANDROIDSTREAMUSB           ();

    DIOSTREAMSTATUS       GetStatus              ();

    bool                  Open                          ();

    bool                  Disconnect                    ();
    bool                  Close                         ();

    bool                  CleanBuffers                  ();

  protected:
   
    static void           ThreadConnection              (void* data);

    XDWORD                ReadBuffer                    (XBYTE* buffer, XDWORD size);
    XDWORD                WriteBuffer                   (XBYTE* buffer, XDWORD size);

    void                  Clean                         ();

    XTHREAD*              threadconnection;

    int                   fd;

    int                   readtimeout;
    int                   writetimeout;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




#endif



