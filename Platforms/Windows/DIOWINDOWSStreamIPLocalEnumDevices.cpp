/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOWINDOWSStreamIPLocalEnumDevices.cpp
* 
* @class      DIOWINDOWSSTREAMIPLOCALENUMDEVICES
* @brief      WINDOWS Data Input/Output Stream IP Local Enum Devices class
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <winsock2.h>
#include <iphlpapi.h>
#include <ws2tcpip.h>

#include "DIOWINDOWSStreamIPLocalEnumDevices.h"

#include "DIOStreamDeviceIP.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWINDOWSSTREAMIPLOCALENUMDEVICES::DIOWINDOWSSTREAMIPLOCALENUMDEVICES() : DIOSTREAMIPLOCALENUMDEVICES()
* @brief      Constructor of class
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWINDOWSSTREAMIPLOCALENUMDEVICES::DIOWINDOWSSTREAMIPLOCALENUMDEVICES() : DIOSTREAMIPLOCALENUMDEVICES()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWINDOWSSTREAMIPLOCALENUMDEVICES::~DIOWINDOWSSTREAMIPLOCALENUMDEVICES()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWINDOWSSTREAMIPLOCALENUMDEVICES::~DIOWINDOWSSTREAMIPLOCALENUMDEVICES()
{
   
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMIPLOCALENUMDEVICES::Search()
* @brief      Search
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMIPLOCALENUMDEVICES::Search()
{
  PIP_ADAPTER_ADDRESSES               adapter_addresses;
  PIP_ADAPTER_ADDRESSES               aa;
  PIP_ADAPTER_UNICAST_ADDRESS         ua;
  PIP_ADAPTER_DNS_SERVER_ADDRESS_XP   dnsa;
  XDWORD                              index = 0;      
  DWORD                               size;
  DWORD                               status; 

  status = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, NULL, &size);
  if(status != ERROR_BUFFER_OVERFLOW) 
    {
      return false;
    }

  adapter_addresses = (PIP_ADAPTER_ADDRESSES)malloc(size);

  status = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, adapter_addresses, &size);
  if(status != ERROR_SUCCESS) 
    {
      free(adapter_addresses);
      return false;
    }

  for(aa = adapter_addresses; aa != NULL; aa = aa->Next) 
    {       
      DIOSTREAMDEVICEIP* device = GEN_NEW DIOSTREAMDEVICEIP();
      if(device)
        {
          device->SetIsActive((aa->OperStatus == IfOperStatusUp?true:false));

          char buffer[BUFSIZ];

          device->SetIndex(index++);
          device->SetType(DIOSTREAMDEVICE_TYPE_IP);

          switch(aa->IfType)
            {
              case IF_TYPE_SOFTWARE_LOOPBACK  : device->SetIPType(DIOSTREAMIPDEVICE_TYPE_LOOPBACK);   
                                                break;

              case IF_TYPE_ETHERNET_CSMACD    : device->SetIPType(DIOSTREAMIPDEVICE_TYPE_ETHERNET);   
                                                break;

              case IF_TYPE_IEEE80211          : device->SetIPType(DIOSTREAMIPDEVICE_TYPE_WIFI);       
                                                break;

                                  default     : device->SetIPType(DIOSTREAMIPDEVICE_TYPE_UNKNOWN);
                                                break;
            }

          memset(buffer, 0, BUFSIZ);
          WideCharToMultiByte(CP_ACP, 0, aa->FriendlyName, (int)wcslen(aa->FriendlyName), buffer, BUFSIZ, NULL, NULL);    
          device->GetName()->Set(buffer);

          memset(buffer, 0, BUFSIZ);
          WideCharToMultiByte(CP_ACP, 0, aa->Description, (int)wcslen(aa->Description), buffer, BUFSIZ, NULL, NULL);    
          device->GetDescription()->Set(buffer);

          device->GetMAC()->Set((XBYTE*)aa->PhysicalAddress);

          for(ua = aa->FirstUnicastAddress; ua != NULL; ua = ua->Next)
            {
              int family = ua->Address.lpSockaddr->sa_family;
              if(family == AF_INET)
                {
                  XDWORD  mask;

                  memset(buffer, 0, BUFSIZ);

                  if(!getnameinfo(ua->Address.lpSockaddr, ua->Address.iSockaddrLength, buffer, sizeof(buffer), NULL, 0, NI_NUMERICHOST))
                    {
                      ConvertLengthToIpv4Mask(ua->OnLinkPrefixLength, (ULONG*)&mask);                  

                      SWAPDWORD(mask);
                      device->GetIP()->Set(buffer);
                      device->GetIP()->GetMask()->Set((XBYTE)(mask >> 24), (XBYTE)(mask >> 16), (XBYTE)(mask >> 8), (XBYTE)(mask));   
                      break;
                    }
                }
            }          

          for(dnsa = aa->FirstDnsServerAddress; dnsa != NULL; dnsa = dnsa->Next)
            {
              int family = dnsa->Address.lpSockaddr->sa_family;
              if(family == AF_INET)
                {
                  memset(buffer, 0, BUFSIZ);
                  if(!getnameinfo(dnsa->Address.lpSockaddr, dnsa->Address.iSockaddrLength, buffer, sizeof(buffer), NULL, 0, NI_NUMERICHOST))
                    {
                      XSTRING* DNSserver = GEN_NEW XSTRING();
                      if(DNSserver)
                        {
                          DNSserver->Set(buffer);
                          if(!DNSserver->IsEmpty())
                            {
                              device->GetDNSservers()->Add(DNSserver);
                            }
                            else
                            {
                              GEN_DELETE DNSserver;
                            }
                        }
                    }
                }
            }

          devices.Add(device);
        }

    }

  free(adapter_addresses);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWINDOWSSTREAMIPLOCALENUMDEVICES::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWINDOWSSTREAMIPLOCALENUMDEVICES::Clean()
{

}



