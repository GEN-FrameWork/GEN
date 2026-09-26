/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOWINDOWSStreamBluetooth.h
* 
* @class      DIOWINDOWSSTREAMBLUETOOTH
* @brief      WINDOWS Data Input/Output Bluetooth class
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


#if defined(DIO_ACTIVE) && (defined(DIO_STREAMBLUETOOTH_ACTIVE) || defined(DIO_STREAMBLUETOOTHLE_ACTIVE))


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include <Windows.h>

#include "XFSMachine.h"
#include "XThreadCollected.h"

#include "DIOStreamBluetooth.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum DIOWINDOWSBTFSMEVENTS
{
  DIOWINDOWSBTFSMEVENT_NONE               = 0 ,
  DIOWINDOWSBTFSMEVENT_GETTINGCONNECTION       ,
  DIOWINDOWSBTFSMEVENT_CONNECTED              ,
  DIOWINDOWSBTFSMEVENT_WAITINGTOREAD          ,
  DIOWINDOWSBTFSMEVENT_SENDINGDATA            ,
  DIOWINDOWSBTFSMEVENT_DISCONNECTING          ,

  DIOWINDOWSBT_LASTEVENT
};


enum DIOWINDOWSBTFSMSTATES
{
  DIOWINDOWSBTFSMSTATE_NONE               = 0 ,
  DIOWINDOWSBTFSMSTATE_GETTINGCONNECTION       ,
  DIOWINDOWSBTFSMSTATE_CONNECTED              ,
  DIOWINDOWSBTFSMSTATE_WAITINGTOREAD          ,
  DIOWINDOWSBTFSMSTATE_SENDINGDATA            ,
  DIOWINDOWSBTFSMSTATE_DISCONNECTING          ,

  DIOWINDOWSBT_LASTSTATE
};

#define BTHNS_RESULT_DEVICE_CONNECTED      0x00010000
#define BTHNS_RESULT_DEVICE_REMEMBERED     0x00020000
#define BTHNS_RESULT_DEVICE_AUTHENTICATED  0x00040000

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOWINDOWSSTREAMBLUETOOTH : public DIOSTREAMBLUETOOTH , public XFSMACHINE
{
  public:
                              DIOWINDOWSSTREAMBLUETOOTH           ();
    virtual                  ~DIOWINDOWSSTREAMBLUETOOTH           ();

    bool                      Open                                ();
    bool                      Disconnect                          ();
    bool                      Close                               ();

    int                       IsReadyConnect                      (SOCKET socket);

  protected:

    bool                      SDP_RegisterService                 (bool reg, char* service_name, char* service_dsc, char* service_prov, void* addr);

  private:

    void                      Clean                               ();

    static void               ThreadConnection                     (void* param);

    XTHREADCOLLECTED*         threadconnection;

    SOCKET                    handlesocket;
    void*                     addr;
    SOCKET                    handleserver;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




#endif



