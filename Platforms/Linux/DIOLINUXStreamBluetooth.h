/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOLINUXStreamBluetooth.h
* 
* @class      DIOLINUXSTREAMBLUETOOTH
* @brief      LINUX Data Input/Output Stream Bluetooth class
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


#if defined(DIO_ACTIVE) && (defined(DIO_STREAMBLUETOOTH_ACTIVE) || defined(DIO_STREAMBLUETOOTHLE_ACTIVE))


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/param.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <malloc.h>
#include <syslog.h>
#include <signal.h>
#include <netinet/in.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include <bluetooth/hci.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/l2cap.h>

#include "XBuffer.h"
#include "XFSMachine.h"
#include "XThreadCollected.h"

#include "DIOStreamBluetooth.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum DIOLINUXBTFSMEVENTS
{
  DIOLINUXBTFSMEVENT_NONE             = 0 ,
  DIOLINUXBTFSMEVENT_SEARCHMAC            ,
  DIOLINUXBTFSMEVENT_SEARCHSERVICES       ,
  DIOLINUXBTFSMEVENT_SEARCHEND            ,

  DIOLINUXBTFSMEVENT_GETTINGCONNECTION    ,
  DIOLINUXBTFSMEVENT_CONNECTED            ,
  DIOLINUXBTFSMEVENT_WAITINGTOREAD        ,
  DIOLINUXBTFSMEVENT_SENDINGDATA          ,
  DIOLINUXBTFSMEVENT_DISCONNECTING        ,

  DIOLINUXBT_LASTEVENT
};


enum DIOLINUXBTFSMSTATES
{
  DIOLINUXBTFSMSTATE_NONE             = 0 ,
  DIOLINUXBTFSMSTATE_SEARCHMAC            ,
  DIOLINUXBTFSMSTATE_SEARCHSERVICES       ,
  DIOLINUXBTFSMSTATE_SEARCHEND            ,

  DIOLINUXBTFSMSTATE_GETTINGCONNECTION    ,
  DIOLINUXBTFSMSTATE_CONNECTED            ,
  DIOLINUXBTFSMSTATE_WAITINGTOREAD        ,
  DIOLINUXBTFSMSTATE_SENDINGDATA          ,
  DIOLINUXBTFSMSTATE_DISCONNECTING        ,

  DIOLINUXBT_LASTSTATE
};

#define DIOLINUXSTREAMBLUETOOTH_SCANBLOCKING

#define DIOLINUXSTREAMBLUETOOTH_DBUSAGENTPATH     "/org/bluez"

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOLINUXSTREAMBLUETOOTH : public DIOSTREAMBLUETOOTH , public XFSMACHINE
{
  public:
                              DIOLINUXSTREAMBLUETOOTH             ();
    virtual                  ~DIOLINUXSTREAMBLUETOOTH             ();

    bool                      Open                                ();
    bool                      Disconnect                          ();
    bool                      Close                               ();

    int                       IsReadyConnect                      (int socket);

  protected:

    sdp_session_t*            SDP_RegisterService                 (char* service_name, char* service_dsc, char* service_prov, int rfcomm_channel);

  private:
    
    bool                      ManagementOfPIN                     (bool active, XSTRING &locMACstring, XSTRING &remMACstring, XSTRING& PIN);

    static void               ThreadRunFunction                   (void* param);
  
    void                      Clean                               ();

    XTHREADCOLLECTED*         threadconnection;

    int                       handlesocket;
    void*                     sdpserversession;
    int                       handleserver;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




#endif




