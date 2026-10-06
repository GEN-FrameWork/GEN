/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOWINDOWSStreamWifiRemoteEnumDevices.cpp
* 
* @class      DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES
* @brief      WINDOWS Data Input/Output Stream Wi-Fi remote Enum Devices class
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



#if defined(DIO_ACTIVE) && defined(DIO_STREAMTWIFI_ACTIVE)


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/


//#pragma comment(lib, "wlanapi.lib")
//#pragma comment(lib, "ole32.lib")
//#pragma comment(lib, "Rpcrt4.lib")

#include "DIOWINDOWSStreamWifiRemoteEnumDevices.h"

#include <stdio.h>
#include <stdlib.h>

#include "XFactory.h"
#include "XTrace.h"
#include "XFileXML.h"

#include "DIOStream.h"
#include "DIOStreamDeviceWifi.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


void    WLanCallback      (WLAN_NOTIFICATION_DATA *scannotificationdata, PVOID context);


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES::DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES() : DIOSTREAMWIFIREMOTEENUMDEVICES()
* @brief      Constructor of class
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES::DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES() : DIOSTREAMWIFIREMOTEENUMDEVICES()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES::~DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES::~DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES()
{
  StopSearch(true);

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES::Search()
* @brief      Search
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES::Search()
{
  HANDLE                        handleclient      = NULL;
  DWORD                         max_client        = 2;
  DWORD                         current_version   = 0;
  DWORD                         result            = 0;
  WCHAR                         GUIDstring[39]    = { 0 };
  PWLAN_INTERFACE_INFO_LIST     interfacelist     = NULL;
  PWLAN_INTERFACE_INFO          interfacelinfo    = NULL;
  PWLAN_BSS_LIST                bsslist           = NULL;
  PWLAN_BSS_ENTRY               bssentry          = NULL;
  bool                          status            = false;

  result = WlanOpenHandle(max_client, NULL, &current_version, &handleclient);
  if(result != ERROR_SUCCESS)
    {
      return false;
    }

  result = WlanEnumInterfaces(handleclient, NULL, &interfacelist);
  if(result != ERROR_SUCCESS)
    {
      return false;
    }

  for(int i = 0; i < (int)interfacelist->dwNumberOfItems; i++)
    {
      interfacelinfo = (WLAN_INTERFACE_INFO *) &interfacelist->InterfaceInfo[i];

      /* ireturn = */ StringFromGUID2(interfacelinfo->InterfaceGuid, (LPOLESTR) &GUIDstring, sizeof(GUIDstring)/sizeof(*GUIDstring));

      XSTRING line;

      switch (interfacelinfo->isState)
        {
          case wlan_interface_state_not_ready               : line = _L("Not ready");                                                      break;
          case wlan_interface_state_connected               : line = _L("Connected");                                                      break;
          case wlan_interface_state_ad_hoc_network_formed   : line = _L("First node in a ad hoc network");                                 break;
          case wlan_interface_state_disconnecting           : line = _L("Disconnecting");                                                  break;
          case wlan_interface_state_disconnected            : line = _L("Not connected");                                                  break;
          case wlan_interface_state_associating             : line = _L("Attempting to associate with a network");                         break;
          case wlan_interface_state_discovering             : line = _L("Auto configuration is discovering settings for the network");     break;
          case wlan_interface_state_authenticating          : line = _L("In process of authenticating");                                   break;
                                          default           : line.Format(_L("Unknown state %ld"), interfacelinfo->isState);               break;
        }

     // XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("[Wi-Fi enum] Interface State[%d]: %s"), i, line.Get());



      // Scan the connected SSID
      result = WlanScan(handleclient, &interfacelinfo->InterfaceGuid, NULL, NULL, NULL);
      if(result != ERROR_SUCCESS)
        {
          return false;
        }
      

      WLAN_CALLBACK_INFO callbackinfo;                                            

      memset(&callbackinfo, 0, sizeof(WLAN_CALLBACK_INFO));

      callbackinfo.interfaceGUID      = interfacelist->InterfaceInfo[i].InterfaceGuid;
      callbackinfo.scanevent          = CreateEvent( NULL, FALSE, FALSE, NULL);           
      
      WlanRegisterNotification(handleclient, WLAN_NOTIFICATION_SOURCE_ALL, TRUE, (WLAN_NOTIFICATION_CALLBACK)WLanCallback, (PVOID)&callbackinfo, NULL, NULL);  
      
      DWORD waitresult = WaitForSingleObject(callbackinfo.scanevent, 20000);       
      if(waitresult == WAIT_OBJECT_0)
        {          
          if(callbackinfo.callbackreason == wlan_notification_acm_scan_complete)
            {

            }
           else
            {
              if(callbackinfo.callbackreason == wlan_notification_acm_scan_fail)
                {
                   
                }
            }
        }
       else
        {
          if(waitresult == WAIT_TIMEOUT)
            {

            }           
        }

      CloseHandle(callbackinfo.scanevent);

      
      // Get the BSS Entry
      result = WlanGetNetworkBssList(handleclient, &interfacelinfo->InterfaceGuid, NULL, dot11_BSS_type_any, TRUE, NULL, &bsslist);
      if(result != ERROR_SUCCESS)
        {
          status = false;
          break;
        }

      for(XDWORD j = 0; j<bsslist->dwNumberOfItems; j++)
        {
          bssentry = (WLAN_BSS_ENTRY*)&bsslist->wlanBssEntries[j];
          if(bssentry)
            {
              DIOSTREAMDEVICEWIFI* wifidevice = GEN_NEW DIOSTREAMDEVICEWIFI();
              if(wifidevice)
                {
                  XSTRING name;
                  for(ULONG a=0; a<bsslist->wlanBssEntries[j].dot11Ssid.uSSIDLength; a++)
                    {
                      name.Add((XCHAR)btowc(bsslist->wlanBssEntries[j].dot11Ssid.ucSSID[a]));
                    }

                  wifidevice->SetIndex(j);

                  //------------------------------------------------------------------------------------------

                  wifidevice->GetName()->Set(name);
                  wifidevice->GetMAC()->Set((XBYTE*)bsslist->wlanBssEntries[j].dot11Bssid);

                  //wifidevice->SetHasSecurity(bsslist->wlanBssEntries[j].bSecurityEnabled?true:false);
                  wifidevice->SetTransmisionPower((int)bsslist->wlanBssEntries[j].lRssi);

                  //------------------------------------------------------------------------------------------

                  AddDevice(wifidevice);

                }
            }
        }


    }

  if(bsslist != NULL)
    {
      WlanFreeMemory(bsslist);
      bsslist = NULL;
    }

  if(interfacelist != NULL)
    {
      WlanFreeMemory(interfacelist);
      interfacelist = NULL;
    }

  if(handleclient)
    {
      WlanCloseHandle(handleclient, NULL);
      handleclient = NULL;
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES::SetAllConnectionInAutomaticMode(bool automaticmodeactive)
* @brief      Set all connection in automatic mode
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  automaticmodeactive : Automaticmodeactive value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES::SetAllConnectionInAutomaticMode(bool automaticmodeactive)
{
  HANDLE                        handleclient      = NULL;
  DWORD                         max_client        = 2;
  DWORD                         current_version   = 0;
  DWORD                         result            = 0;
  WCHAR                         GUIDstring[39]    = { 0 };
  PWLAN_INTERFACE_INFO_LIST     interfacelist     = NULL;
  PWLAN_INTERFACE_INFO          interfacelinfo    = NULL;
  PWLAN_PROFILE_INFO_LIST       profilelist       = NULL;
  PWLAN_PROFILE_INFO            profileinfo       = NULL;
  XSTRING                       line;

  result = WlanOpenHandle(max_client, NULL, &current_version, &handleclient);
  if(result != ERROR_SUCCESS) return false;

  result = WlanEnumInterfaces(handleclient, NULL, &interfacelist);
  if(result != ERROR_SUCCESS) return false;

  for(int i = 0; i < (int) interfacelist->dwNumberOfItems; i++)
    {
      interfacelinfo = (WLAN_INTERFACE_INFO *) &interfacelist->InterfaceInfo[i];

      /* ireturn = */ StringFromGUID2(interfacelinfo->InterfaceGuid, (LPOLESTR) &GUIDstring, sizeof(GUIDstring)/sizeof(*GUIDstring));

      result = WlanGetProfileList(handleclient, &interfacelinfo->InterfaceGuid, NULL, &profilelist);
      if(result != ERROR_SUCCESS)
        {
          // returnval = 1;
        }
       else
        {
          for(XDWORD j=0; j<profilelist->dwNumberOfItems; j++)
            {
              profileinfo = (WLAN_PROFILE_INFO*)&profilelist->ProfileInfo[j];

              XFILEXML          filexml;
              XSTRING           profileXML;
              LPWSTR            originalprofileXML = NULL;
              DWORD             flags              = 0;
              DWORD             grantedaccess      = 0;
              WLAN_REASON_CODE  wlanreason         = 0;
              int               index   = 0;
              XSTRING           nameelement;

              result = WlanGetProfile(handleclient, &(interfacelist->InterfaceInfo[0].InterfaceGuid), profileinfo->strProfileName, NULL, &originalprofileXML, &flags, &grantedaccess);

              profileXML =  originalprofileXML;

              filexml.AddLine(profileXML);
              filexml.DecodeAllLines();

              nameelement = _L("connectionMode");
              XFILEXMLELEMENT* element = filexml.SearchElement(nameelement, index, NULL);
              if(element)
                {
                  element->SetValue(automaticmodeactive?_L("auto"):_L("manual"));
                }

              filexml.EncodeAllLines(false);
              profileXML.Empty();
              filexml.GetAllInOneLine(profileXML);

              result = WlanSetProfile(handleclient, &(interfacelist->InterfaceInfo[0].InterfaceGuid), 0, profileXML.Get(), NULL, TRUE, NULL, &wlanreason);


            }
        }
    }

  if(profilelist != NULL)
    {
      WlanFreeMemory(profilelist);
      profilelist = NULL;
    }

  if(interfacelist != NULL)
    {
      WlanFreeMemory(interfacelist);
      interfacelist = NULL;
    }

  WlanCloseHandle(handleclient, NULL);

  return result?false:true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES::SetAllConnectionStatus(bool activate)
* @brief      Set all connection status
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  activate : Activate value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES::SetAllConnectionStatus(bool activate)
{
  HANDLE                      handleclient        = NULL;
  DWORD                       max_client          = 2;
  DWORD                       current_version     = 0;
  DWORD                       result              = 0;  
  PWLAN_INTERFACE_INFO_LIST   interfacelist       = NULL;
  PWLAN_INTERFACE_INFO        interfacelinfo      = NULL;  
  PWLAN_INTERFACE_CAPABILITY  interfacecapability = NULL;
  
  result = WlanOpenHandle(max_client, NULL, &current_version, &handleclient);
  if(result != ERROR_SUCCESS) return false;

  result = WlanEnumInterfaces(handleclient, NULL, &interfacelist);
  if(result != ERROR_SUCCESS) return false;


  for(DWORD c=0; c<interfacelist->dwNumberOfItems; c++)
    {
      bool needactivated = false;

      interfacelinfo = (WLAN_INTERFACE_INFO*)&interfacelist->InterfaceInfo[c];
      
      switch(interfacelinfo->isState) 
        {
          case wlan_interface_state_not_ready             : break;  // Not ready

          case wlan_interface_state_connected             : break;  // Connected

          case wlan_interface_state_ad_hoc_network_formed : break;  // First node in a ad hoc network            

          case wlan_interface_state_disconnecting         : break;  // Disconnecting

          case wlan_interface_state_disconnected          : needactivated = true;
                                                            break;  // Not connected                

          case wlan_interface_state_associating           : break;  // Attempting to associate with a network

          case wlan_interface_state_discovering           : break;  // Auto configuration is discovering settings for the network

          case wlan_interface_state_authenticating        : break;  // In process of authenticating

                                          default         : break;  // Unknown state 
        }

      if(needactivated)
        {
          // get interface capability, which includes the supported PHYs   
          result = WlanGetInterfaceCapability(handleclient, &interfacelinfo->InterfaceGuid, NULL, &interfacecapability);
          if(result == ERROR_SUCCESS)
            {                
              // set radio state on every PHY   
              for(DWORD d=0; d<interfacecapability->dwNumberOfSupportedPhys; d++)
                {
                  WLAN_PHY_RADIO_STATE  wlanphyradiostate;
                  
                  // set radio state on every PHY   
                  wlanphyradiostate.dwPhyIndex = d;

                  if(activate)    
                    {
                      wlanphyradiostate.dot11SoftwareRadioState = dot11_radio_state_on;    
                      wlanphyradiostate.dot11HardwareRadioState = dot11_radio_state_on;    
                    }                                    
                   else 
                    {
                      wlanphyradiostate.dot11SoftwareRadioState = dot11_radio_state_off;
                      wlanphyradiostate.dot11HardwareRadioState = dot11_radio_state_off;
                    }    

                  PVOID pdata = &wlanphyradiostate;
    
                  result = WlanSetInterface(handleclient, &interfacelinfo->InterfaceGuid, wlan_intf_opcode_radio_state, sizeof(WLAN_PHY_RADIO_STATE), pdata, NULL);
                  if(result == ERROR_SUCCESS)
                    {
                      //XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("Wifi Activated.")); 
                    }
                   else
                    {
                      //XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, _L("Wifi Activate Error!")); 
                    }    
                }
            }         
        }
    }
  
  if(interfacelist != NULL)
    {
      WlanFreeMemory(interfacelist);
      interfacelist = NULL;
    }

  WlanCloseHandle(handleclient, NULL);

  return result?false:true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES::StopSearch(bool waitend)
* @brief      Stop search
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  waitend : Waitend value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES::StopSearch(bool waitend)
{
  if(!IsSearching()) return false;

  return true;
};


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES::IsSearching()
* @brief      Is searching
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES::IsSearching()
{
  return issearching;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES::TriggerScan(HANDLE wlanhandle, WLAN_INTERFACE_INFO_LIST* interfaces)
* @brief      Trigger scan
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  wlanhandle : Wlanhandle value.
* @param[in]  interfaces : Interfaces pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES::TriggerScan(HANDLE wlanhandle, WLAN_INTERFACE_INFO_LIST* interfaces)
{

  for(ULONG i=0; i<interfaces->dwNumberOfItems; i++)
    {
      //Declare the callback parameter struct
      WLAN_CALLBACK_INFO callbackinfo;

      memset(&callbackinfo, 0, sizeof(WLAN_CALLBACK_INFO));

      callbackinfo.interfaceGUID = interfaces->InterfaceInfo[i].InterfaceGuid;

      //Create an event to be triggered in the scan case
      callbackinfo.scanevent = CreateEvent( NULL, FALSE, FALSE, NULL);


      //Register for wlan scan notifications
      WlanRegisterNotification(wlanhandle, WLAN_NOTIFICATION_SOURCE_ALL, TRUE, (WLAN_NOTIFICATION_CALLBACK)WLanCallback, (PVOID)&callbackinfo, NULL, NULL);

      //Start a scan. If the WlanScan call fails, log the error
      WlanScan(wlanhandle, &(interfaces->InterfaceInfo[i].InterfaceGuid), NULL, NULL, NULL);
      if(GetLastError() != ERROR_SUCCESS)
        {
          continue;
        }

      //Wait for the event to be signaled, or an error to occur. Don't wait longer than 15 seconds.
      DWORD waitresult = WaitForSingleObject(callbackinfo.scanevent, 15000);

      //Check how we got here, via callback or timeout
      if(waitresult == WAIT_OBJECT_0)
        {
          if(callbackinfo.callbackreason == wlan_notification_acm_scan_complete)
            {

            }
           else
            {
              if(callbackinfo.callbackreason == wlan_notification_acm_scan_fail)
                {

                }
            }
        }
       else
        {
          if(waitresult == WAIT_TIMEOUT)
            {

            }
           else
            {

            }
        }
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void WLanCallback(WLAN_NOTIFICATION_DATA *scannotificationdata, PVOID context)
* @brief      LanCallback
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  scannotificationdata : Scannotificationdata pointer to use.
* @param[in]  context : Context object to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void WLanCallback(WLAN_NOTIFICATION_DATA *scannotificationdata, PVOID context)
{
  // Get the data from my struct. If it's null, nothing to do
  WLAN_CALLBACK_INFO* callbackinfo = (WLAN_CALLBACK_INFO*)context;
  if(!callbackinfo)
    {
      return;
    }

  // Check the GUID in the struct against the GUID in the notification data, return if they don't match
  if(memcmp(&callbackinfo->interfaceGUID, &scannotificationdata->InterfaceGuid, sizeof(GUID)) != 0)
    {
      return;
    }

  // If the notification was for a scan complete or failure then we need to set the event
  if((scannotificationdata->NotificationCode == wlan_notification_acm_scan_complete) || (scannotificationdata->NotificationCode == wlan_notification_acm_scan_fail))
    {
      //Set the notification code as the callbackReason
      callbackinfo->callbackreason = scannotificationdata->NotificationCode;

      //Set the event
      SetEvent(callbackinfo->scanevent);
    }

  return;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES::Clean()
{
  issearching = false;
}




#endif
