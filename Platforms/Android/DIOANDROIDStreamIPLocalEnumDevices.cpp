/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOANDROIDStreamIPLocalEnumDevices.cpp
* 
* @class      DIOANDROIDSTREAMIPLOCALENUMDEVICES
* @brief      ANDROID Data Input/Output Stream IP Local Enum Devices class
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOANDROIDStreamIPLocalEnumDevices.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <linux/if_ether.h>
#include <arpa/inet.h>

#include "XTrace.h"
#include "DIOStreamDeviceIP.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOANDROIDSTREAMIPLOCALENUMDEVICES::DIOANDROIDSTREAMIPLOCALENUMDEVICES() : DIOSTREAMIPLOCALENUMDEVICES()
* @brief      Constructor of class
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOANDROIDSTREAMIPLOCALENUMDEVICES::DIOANDROIDSTREAMIPLOCALENUMDEVICES() : DIOSTREAMIPLOCALENUMDEVICES()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOANDROIDSTREAMIPLOCALENUMDEVICES::~DIOANDROIDSTREAMIPLOCALENUMDEVICES()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOANDROIDSTREAMIPLOCALENUMDEVICES::~DIOANDROIDSTREAMIPLOCALENUMDEVICES()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOANDROIDSTREAMIPLOCALENUMDEVICES::Search()
* @brief      Search
* @ingroup    PLATFORM_ANDROID
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOANDROIDSTREAMIPLOCALENUMDEVICES::Search()
{
  DelAllDevices();

  int           sock;
  struct ifconf ifconf;
  struct ifreq  ifreq[DIOANDROIDSTREAMIPLOCALENUMDEVICES_MAXIFS];
  int           ninterfaces;
  int           c;

  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) return false;

  ifconf.ifc_buf = (char *) ifreq;
  ifconf.ifc_len = sizeof ifreq;

  if(ioctl(sock, SIOCGIFCONF, &ifconf) == -1)
    {
      close(sock);
      return false;
    }

  ninterfaces = ifconf.ifc_len / sizeof(ifreq[0]);

  for(c=0; c<ninterfaces; c++)
    {
       char                 ip[INET_ADDRSTRLEN];
       struct sockaddr_in*  address = (struct sockaddr_in *) &ifreq[c].ifr_addr;

        if(!inet_ntop(AF_INET, &address->sin_addr, ip, sizeof(ip)))
          {
            close(sock);
            return false;
          }

        DIOSTREAMDEVICEIP* device = GEN_NEW DIOSTREAMDEVICEIP();
        if(device)
          {
            device->SetIndex(c);

            device->GetName()->Set(ifreq[c].ifr_name);
            device->GetDescription()->Set(_L(""));
            device->GetMAC()->Set((XBYTE*)ifreq[c].ifr_hwaddr.sa_data);

            device->SetIPType(DIOSTREAMIPDEVICE_TYPE_UNKNOWN);

            if(ifreq[c].ifr_flags & IFF_LOOPBACK)
              {
                device->SetIPType(DIOSTREAMIPDEVICE_TYPE_LOOPBACK);
              }
             else
              {
                if(device->GetName()->Find(_L("eth"), true)!=XSTRING_NOTFOUND)  device->SetIPType(DIOSTREAMIPDEVICE_TYPE_ETHERNET);
                if(device->GetName()->Find(_L("wlan"), true)!=XSTRING_NOTFOUND) device->SetIPType(DIOSTREAMIPDEVICE_TYPE_WIFI);
              }

            XSTRING IP;
            IP.Set(ip);
            device->GetIP()->Set(IP);

            devices.Add(device);
          }
    }

  close(sock);

  return true;
}



