/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOANDROIDStreamWifiRemoteEnumDevices.h
* 
* @class      DIOANDROIDSTREAMWIFIREMOTEENUMDEVICES
* @brief      ANDROID Data Input/Output Stream Wifi Remote Enum Devices class
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


#if defined(DIO_ACTIVE) && defined(DIO_STREAMTWIFI_ACTIVE)


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XFSMachine.h"

#include "DIOMAC.h"
#include "DIOStreamWifiRemoteEnumDevices.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum DIOANDROIDWIFIENUMFSMFSMEVENTS
{
  DIOANDROIDWIFIENUMFSMEVENT_NONE             = 0 ,
  DIOANDROIDWIFIENUMFSMEVENT_SEARCH               ,
  DIOANDROIDWIFIENUMFSMEVENT_SEARCHEND            ,

  DIOANDROIDWIFIENUM_LASTEVENT
};

enum DIOANDROIDWIFIENUMFSMSTATES
{
  DIOANDROIDWIFIENUMFSMSTATE_NONE             = 0 ,
  DIOANDROIDWIFIENUMFSMSTATE_SEARCH               ,
  DIOANDROIDWIFIENUMFSMSTATE_SEARCHEND            ,

  DIOANDROIDWIFIENUM_LASTSTATE
};


/*---- CLASS ---------------------------------------------------------------------------------------------------------*/


class XTHREAD;
class DIOSTREAMDEVICEWIFI;


class DIOANDROIDSTREAMWIFIREMOTEENUMDEVICES :  public DIOSTREAMWIFIREMOTEENUMDEVICES, public XFSMACHINE
{
  public:
                              DIOANDROIDSTREAMWIFIREMOTEENUMDEVICES           ();
    virtual                  ~DIOANDROIDSTREAMWIFIREMOTEENUMDEVICES           ();

    bool                      Search                                          ();
    bool                      StopSearch                                      (bool waitend);
    bool                      IsSearching                                     ();

  private:

    static void               ThreadEnumDevices                               (void* thread);

    void                      Clean                                           ();

    XTHREAD*                  threadenumdevices;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




#endif



