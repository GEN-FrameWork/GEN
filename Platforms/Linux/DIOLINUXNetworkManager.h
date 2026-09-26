/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOLINUXNetworkManager.h
* 
* @class      DIOLINUXNETWORKMANAGER
* @brief      LINUX Data Input/Output Network Manager class
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

#include "XString.h"
#include "XObserver.h"
#include "XVariant.h"

#include "DIOStreamDeviceIP.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum DIOLINUXNETWORKMANAGER_STATE
{
  DIOLINUXNETWORKMANAGER_STATE_UNKNOWN            = 0 ,  // networking state is unknown
  DIOLINUXNETWORKMANAGER_STATE_ASLEEP                 ,  // networking is not enabled 
  DIOLINUXNETWORKMANAGER_STATE_DISCONNECTED           ,  // there is no active network connection
  DIOLINUXNETWORKMANAGER_STATE_DISCONNECTING          ,  // network connections are being cleaned up
  DIOLINUXNETWORKMANAGER_STATE_CONNECTING             ,  // a network connection is being started
  DIOLINUXNETWORKMANAGER_STATE_CONNECTED_LOCAL        ,  // there is only local IPv4 and/or IPv6 connectivity
  DIOLINUXNETWORKMANAGER_STATE_CONNECTED_SITE         ,  // there is only site-wide IPv4 and/or IPv6 connectivity
  DIOLINUXNETWORKMANAGER_STATE_CONNECTED_GLOBAL       ,  // there is global IPv4 and/or IPv6 Internet connectivity
};

#define DIOLINUXNETWORKMANAGER_DEFAULT_DBUS_DESTINATION        __L("org.freedesktop.NetworkManager")
#define DIOLINUXNETWORKMANAGER_DEFAULT_DBUS_PATH               __L("/org/freedesktop/NetworkManager")
#define DIOLINUXNETWORKMANAGER_DEFAULT_DBUS_IFACE_MAIN         __L("org.freedesktop.NetworkManager")
#define DIOLINUXNETWORKMANAGER_DEFAULT_DBUS_IFACE_DEVICE       __L("org.freedesktop.NetworkManager.Device")
#define DIOLINUXNETWORKMANAGER_DEFAULT_DBUS_IFACE_IP4CONFIG    __L("org.freedesktop.NetworkManager.IP4Config")

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/

class DIOLINUXDBUS_XEVENT;
class DIOLINUXDBUS;

class DIOLINUXNETWORKMANAGER : public XOBSERVER
{
  public:
                                    DIOLINUXNETWORKMANAGER      ();
    virtual                        ~DIOLINUXNETWORKMANAGER      ();

    bool                            Ini                         ();

    bool                            Enable                      (bool on);  
    DIOLINUXNETWORKMANAGER_STATE    GetConnectionState          ();

    bool                            GetDevices                  (XVECTOR<DIOSTREAMDEVICEIP*>& devices);  

    bool                            Signal_Add                  (XCHAR* type, XCHAR* name);  
    bool                            Signal_Add                  (XSTRING& type, XSTRING& name);  

    bool                            End                         ();

  private:       

    bool                            GetDevicePathList           (XVECTOR<XSTRING*>& pathdevices);  
    bool                            GetProperty                 (XCHAR* iface, XCHAR* pathdevice, XCHAR* propertyname, XVARIANT& variant);  

    void                            HandleEvent_Signal          (DIOLINUXDBUS_XEVENT* event);
    void                            HandleEvent                 (XEVENT* xevent);

    void                            Clean                       ();       

    DIOLINUXDBUS*                   dbus;
    XSTRING                         dbus_destination;
    XSTRING                         dbus_path;
};





