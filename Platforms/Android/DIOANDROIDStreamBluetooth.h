/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOANDROIDStreamBluetooth.h
* 
* @class      DIOANDROIDSTREAMBLUETOOTH
* @brief      ANDROID Data Input/Output Stream Bluetooth class
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


#if defined(DIO_ACTIVE) && defined(DIO_STREAMBLUETOOTH_ACTIVE)


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



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum DIOANDROIDBTFSMEVENTS
{
  DIOANDROIDBTFSMEVENT_NONE             = 0 ,
  DIOANDROIDBTFSMEVENT_SEARCHMAC            ,
  DIOANDROIDBTFSMEVENT_SEARCHSERVICES       ,
  DIOANDROIDBTFSMEVENT_SEARCHEND            ,

  DIOANDROIDBTFSMEVENT_GETTINGCONNECTION     ,
  DIOANDROIDBTFSMEVENT_CONNECTED            ,
  DIOANDROIDBTFSMEVENT_WAITINGTOREAD        ,
  DIOANDROIDBTFSMEVENT_SENDINGDATA          ,
  DIOANDROIDBTFSMEVENT_DISCONNECTING        ,

  DIOANDROIDBT_LASTEVENT
};


enum DIOANDROIDBTFSMSTATES
{
  DIOANDROIDBTFSMSTATE_NONE             = 0 ,
  DIOANDROIDBTFSMSTATE_SEARCHMAC            ,
  DIOANDROIDBTFSMSTATE_SEARCHSERVICES       ,
  DIOANDROIDBTFSMSTATE_SEARCHEND            ,

  DIOANDROIDBTFSMSTATE_GETTINGCONNECTION     ,
  DIOANDROIDBTFSMSTATE_CONNECTED            ,
  DIOANDROIDBTFSMSTATE_WAITINGTOREAD        ,
  DIOANDROIDBTFSMSTATE_SENDINGDATA          ,
  DIOANDROIDBTFSMSTATE_DISCONNECTING        ,

  DIOANDROIDBT_LASTSTATE
};

#define DIOANDROIDSTREAMBLUETOOTH_SCANBLOCKING
#define DIOANDROIDSTREAMBLUETOOTH_DBUSAGENTPATH     "/org/bluez"

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XTHREAD;

class DIOANDROIDSTREAMBLUETOOTH : public DIOSTREAMBLUETOOTH , public XFSMACHINE
{
  public:
                              DIOANDROIDSTREAMBLUETOOTH             ( );
    virtual                  ~DIOANDROIDSTREAMBLUETOOTH             ();

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

    XTHREAD*                  threadconnection;

    int                       handlesocket;
    void*                     sdpserversession;
    int                       handleserver;

    XBYTE                     buffer[DIOSTREAM_MAXBUFFER];
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




#endif



