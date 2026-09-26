/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOLINUXStreamBluetoothRemoteEnumDevices.h
* 
* @class      DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES
* @brief      LINUX Data Input/Output Stream Bluetooth Remote Enum Devices class
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

#include <bluetooth/bluetooth.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/l2cap.h>

#include "XFSMachine.h"
#include "XThreadCollected.h"

#include "DIOMAC.h"
#include "DIOStreamBluetoothRemoteEnumDevices.h"



#if defined(DIO_ACTIVE) && defined(DIO_STREAMBLUETOOTH_ACTIVE)


/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum DIOLINUXBTENUMFSMFSMEVENTS
{
  DIOLINUXBTENUMFSMEVENT_NONE             = 0 ,
  DIOLINUXBTENUMFSMEVENT_SEARCHMAC            ,
  DIOLINUXBTENUMFSMEVENT_SEARCHSERVICES       ,
  DIOLINUXBTENUMFSMEVENT_SEARCHNAME           ,
  DIOLINUXBTENUMFSMEVENT_SEARCHEND            ,

  DIOLINUXBTENUM_LASTEVENT
};

enum DIOLINUXBTENUMFSMSTATES
{
  DIOLINUXBTENUMFSMSTATE_NONE             = 0 ,
  DIOLINUXBTENUMFSMSTATE_SEARCHMAC            ,
  DIOLINUXBTENUMFSMSTATE_SEARCHSERVICES       ,
  DIOLINUXBTENUMFSMSTATE_SEARCHNAME           ,
  DIOLINUXBTENUMFSMSTATE_SEARCHEND            ,

  DIOLINUXBTENUM_LASTSTATE
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOSTREAMDEVICEBLUETOOTH;

struct search_context
{
  char*       svc;
  uuid_t      group;
  int         tree;
  uint32_t    handle;
};

class DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES :  public DIOSTREAMBLUETOOTHREMOTEENUMDEVICES, public XFSMACHINE
{
  public:
                              DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES        ();
    virtual                  ~DIOLINUXSTREAMBLUETOOTHREMOTEENUMDEVICES        ();

    bool                      Search                                          ();
    bool                      StopSearch                                      (bool waitend);
    bool                      IsSearching                                     ();

  private:  

    bool                      GetAddrFromLocalDevice                          (int localdeviceindex, char* btaddr);

    int                       IsReadyConnect                                  (int socket);

    void                      SearchServices                                  ();
    bool                      ScanDevices                                     (XVECTOR<DIOSTREAMDEVICE*>* devices);
    bool                      ScanDevicesName                                 (DIOSTREAMDEVICEBLUETOOTH* device);
    bool                      ScanDevicesServices                             (XVECTOR<DIOSTREAMDEVICE*>* devices);
    bool                      ScanDeviceServices                              (DIOSTREAMDEVICEBLUETOOTH* device, struct search_context* context);

    static void               ThreadEnumDevices                               (void* thread);  

    void                      Clean                                           ();

    XTHREADCOLLECTED*         threadenumdevices;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




#endif



