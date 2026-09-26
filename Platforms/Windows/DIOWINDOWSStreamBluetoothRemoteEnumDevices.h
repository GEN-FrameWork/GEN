/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOWINDOWSStreamBluetoothRemoteEnumDevices.h
* 
* @class      DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES
* @brief      WINDOWS Data Input/Output Stream Bluetooth Remote Enum Devices class
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


#if defined(DIO_ACTIVE) && defined(DIO_STREAMBLUETOOTH_ACTIVE)


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include <Windows.h>

#include "XFSMachine.h"
#include "XThreadCollected.h"

#include "DIOStream.h"

#include "DIOStreamBluetoothRemoteEnumDevices.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum DIOWINDOWSBTENUMFSMFSMEVENTS
{
  DIOWINDOWSBTENUMFSMEVENT_NONE             = 0 ,
  DIOWINDOWSBTENUMFSMEVENT_SEARCHMAC            ,
  DIOWINDOWSBTENUMFSMEVENT_SEARCHSERVICES       ,
  DIOWINDOWSBTENUMFSMEVENT_SEARCHNAME           ,
  DIOWINDOWSBTENUMFSMEVENT_SEARCHEND            ,

  DIOWINDOWSBTENUM_LASTEVENT
};


enum DIOWINDOWSBTENUMFSMSTATES
{
  DIOWINDOWSBTENUMFSMSTATE_NONE             = 0 ,
  DIOWINDOWSBTENUMFSMSTATE_SEARCHMAC            ,
  DIOWINDOWSBTENUMFSMSTATE_SEARCHSERVICES       ,
  DIOWINDOWSBTENUMFSMSTATE_SEARCHNAME           ,
  DIOWINDOWSBTENUMFSMSTATE_SEARCHEND            ,

  DIOWINDOWSBTENUM_LASTSTATE
};


enum DIOWINDOWSBTENUMPROTOCOLUUID
{
  DIOWINDOWSBTENUMPROTOCOLUUID_NONE         = 0X0000 ,
  DIOWINDOWSBTENUMPROTOCOLUUID_SDP          = 0X0001 ,
  DIOWINDOWSBTENUMPROTOCOLUUID_UDP          = 0X0002 ,
  DIOWINDOWSBTENUMPROTOCOLUUID_RFCOMM       = 0X0003 ,
  DIOWINDOWSBTENUMPROTOCOLUUID_TCP          = 0X0004 ,
  DIOWINDOWSBTENUMPROTOCOLUUID_TCS_BIN      = 0X0005 ,
  DIOWINDOWSBTENUMPROTOCOLUUID_TCS_AT       = 0X0006 ,
  DIOWINDOWSBTENUMPROTOCOLUUID_OBEX         = 0X0008 ,
  DIOWINDOWSBTENUMPROTOCOLUUID_IP           = 0X0009 ,
  DIOWINDOWSBTENUMPROTOCOLUUID_FTP          = 0X000A ,
  DIOWINDOWSBTENUMPROTOCOLUUID_HTTP         = 0X000C ,
  DIOWINDOWSBTENUMPROTOCOLUUID_WSP          = 0X000E ,
  DIOWINDOWSBTENUMPROTOCOLUUID_BNEP         = 0X000F ,
  DIOWINDOWSBTENUMPROTOCOLUUID_UPNP         = 0X0010 ,
  DIOWINDOWSBTENUMPROTOCOLUUID_HIDP         = 0X0011 ,
  DIOWINDOWSBTENUMPROTOCOLUUID_HCRP_CTRL    = 0X0012 ,
  DIOWINDOWSBTENUMPROTOCOLUUID_HCRP_DATA    = 0X0014 ,
  DIOWINDOWSBTENUMPROTOCOLUUID_HCRP_NOTE    = 0X0016 ,
  DIOWINDOWSBTENUMPROTOCOLUUID_AVCTP        = 0X0017 ,
  DIOWINDOWSBTENUMPROTOCOLUUID_AVDTP        = 0X0019 ,
  DIOWINDOWSBTENUMPROTOCOLUUID_CMTP         = 0X001B ,
  DIOWINDOWSBTENUMPROTOCOLUUID_UDI          = 0X001D ,
  DIOWINDOWSBTENUMPROTOCOLUUID_MCAP_CTRL    = 0X001E ,
  DIOWINDOWSBTENUMPROTOCOLUUID_MCAP_DATA    = 0X001F ,
  DIOWINDOWSBTENUMPROTOCOLUUID_L2CAP        = 0X0100
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/


class DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES :  public DIOSTREAMBLUETOOTHREMOTEENUMDEVICES, public XFSMACHINE
{
  public:
                              DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES      ();
    virtual                  ~DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES      ();

    bool                      Search                                          ();
    bool                      StopSearch                                      (bool waitend);
    bool                      IsSearching                                     ();

  private:

    void                      Clean                                           ();

    static void               ThreadEnumDevices                               (void* thread);
    void                      SearchServices                                  ();


    XTHREADCOLLECTED*         threadenumdevices;

    WSAQUERYSET               wsaq;
    HANDLE                    hlookup;
    XBYTE                     buffer[DIOSTREAM_MAXBUFFER];

};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/

BOOL __stdcall    SDP_ServiceCallback     (ULONG attribID, LPBYTE valuestream, ULONG cbstreamsize, LPVOID vparam);



#endif



