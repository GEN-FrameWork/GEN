/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOWINDOWSWifiManagerMode.cpp
* 
* @class      DIOWINDOWSWIFIMANAGERMODE
* @brief      WINDOWS Data Input/Output Wifi Manager Mode
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

#include "DIOWINDOWSWifiManagerMode.h"

#include <windows.h>

#include <wlanapi.h>
#include <Winbase.h>
#include <Commctrl.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>

#include <iostream>
#include <string>
#include <tchar.h>
#include <process.h>
#include <tlhelp32.h>

#include <wtsapi32.h>
#include <userenv.h>


#include "XFactory.h"
#include "XSleep.h"
#include "XTrace.h"
#include "XTimer.h"
#include "XFileXML.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWINDOWSWIFIMANAGERMODE::DIOWINDOWSWIFIMANAGERMODE()
* @brief      Constructor of class
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWINDOWSWIFIMANAGERMODE::DIOWINDOWSWIFIMANAGERMODE()    
{ 
  Clean();                            
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWINDOWSWIFIMANAGERMODE::~DIOWINDOWSWIFIMANAGERMODE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWINDOWSWIFIMANAGERMODE::~DIOWINDOWSWIFIMANAGERMODE()    
{ 
  Clean();                            
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSWIFIMANAGERMODE::SetMode_Client(XCHAR* SSID, XCHAR* password, bool serialize)
* @brief      Set mode client
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  SSID : SSID pointer to use.
* @param[in]  password : Password text.
* @param[in]  serialize : Serialize value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSWIFIMANAGERMODE::SetMode_Client(XCHAR* SSID, XCHAR* password, bool serialize)
{
  HANDLE                        handleclient      = NULL;
  DWORD                         max_client        = 2;
  DWORD                         current_version   = 0;  
  PWLAN_INTERFACE_INFO_LIST     interfacelist     = NULL;
  PWLAN_INTERFACE_INFO          interfaceinfo     = NULL;
  XSTRING                       profilename; 
  DWORD                         result            = 1;
  
  if(!SSID)  return false;

  profilename = SSID;
  
  result = WlanOpenHandle(max_client, NULL, &current_version, &handleclient);
  if(result != ERROR_SUCCESS) return false;

  result = WlanEnumInterfaces(handleclient, NULL, &interfacelist);
  if(result != ERROR_SUCCESS) return false;

  for(int i = 0; i < (int) interfacelist->dwNumberOfItems; i++)
    {
      interfaceinfo = (WLAN_INTERFACE_INFO *) &interfacelist->InterfaceInfo[i];
     
      WLAN_CONNECTION_PARAMETERS  wlanconnparams; 
      XSTRING                     profile;
                  
      if(password)
        {
          profile.Format(_L("<?xml version=\"1.0\" encoding=\"US-ASCII\"?>" "<WLANProfile xmlns=\"http://www.microsoft.com/networking/WLAN/profile/v1\">" "<name>%s</name>" "<SSIDConfig>" "<SSID>" "<name>%s</name>" "</SSID>" "</SSIDConfig>" "<connectionType>ESS</connectionType>" "<connectionMode>auto</connectionMode>" "<autoSwitch>false</autoSwitch>" "<MSM>" "<security>" "<authEncryption>" "<authentication>WPA2PSK</authentication>" "<encryption>AES</encryption>" "<useOneX>false</useOneX>" "</authEncryption>" "<sharedKey>" "<keyType>passPhrase</keyType>" "<protected>false</protected>" "<keyMaterial>%s</keyMaterial>" "</sharedKey>" "</security>" "</MSM>" "</WLANProfile>"), profilename.Get(), profilename.Get(), password);
        }
        else 
        {              
          profile.Format(_L("<?xml version=\"1.0\" encoding=\"US-ASCII\"?>" "<WLANProfile xmlns=\"http://www.microsoft.com/networking/WLAN/profile/v1\">" "<name>%s</name>" "<SSIDConfig>" "<SSID>" "<name>%s</name>" "</SSID>" "</SSIDConfig>" "<connectionType>ESS</connectionType>" "<connectionMode>auto</connectionMode>" "<autoSwitch>false</autoSwitch>" "<MSM>" "<security>" "<authEncryption>" "<authentication>open</authentication>" "<encryption>none</encryption>" "<useOneX>false</useOneX>" "</authEncryption>" "</security>" "</MSM>" "</WLANProfile>"), profilename.Get(), profilename.Get());
        }
                      
      wlanconnparams.wlanConnectionMode   = wlan_connection_mode_temporary_profile;         // YES,WE CONNECT AP VIA THE PROFILE 
      wlanconnparams.strProfile           = profile.Get();                                  // Set the profile name 
      wlanconnparams.pDot11Ssid           = NULL; //&wlan_BSS_entry.dot11Ssid;                      // SET SSID NULL 
      wlanconnparams.dot11BssType         = dot11_BSS_type_infrastructure;                  // dot11_BSS_type_any,I do not need it this time.   
      wlanconnparams.pDesiredBssidList    = NULL;                                           // the desired BSSID list is empty 
      wlanconnparams.dwFlags              = WLAN_CONNECTION_HIDDEN_NETWORK;                 // it works on my WIN7\8 

      result =  WlanConnect(handleclient, &interfaceinfo->InterfaceGuid, &wlanconnparams, NULL);                  
      if(result == ERROR_SUCCESS) 
        {          
          break;
        }                   
    }

  if(interfacelist != NULL)
    {
      WlanFreeMemory(interfacelist);
      interfacelist = NULL;
    }

  WlanCloseHandle(handleclient, NULL);
  
  if(result == ERROR_SUCCESS) 
    {
      XTIMER* xtimer=GEN_XFACTORY.CreateTimer();
      if(xtimer)
        {       
          while(!IsConnected())
            {
              if(xtimer->GetMeasureSeconds() > 5) 
                {
                  result = ERROR_ACCESS_DENIED;
                  break;
                }

              GEN_XSLEEP.MilliSeconds(100);              
            }

          GEN_XFACTORY.DeleteTimer(xtimer);
        }      
    }
  
  if(result==ERROR_SUCCESS) GEN_XSLEEP.Seconds(5);
      
  return (result==ERROR_SUCCESS)?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSWIFIMANAGERMODE::IsDisconnected()
* @brief      Is disconnected
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSWIFIMANAGERMODE::IsDisconnected()
{
  HANDLE                        handleclient      = NULL;
  DWORD                         max_client        = 2;
  DWORD                         current_version   = 0;
  DWORD                         result            = 0;
  PWLAN_INTERFACE_INFO_LIST     interfacelist     = NULL;
  PWLAN_INTERFACE_INFO          interfacelinfo    = NULL;
  bool                          status            = false;

  result = WlanOpenHandle(max_client, NULL, &current_version, &handleclient);
  if(result != ERROR_SUCCESS) return false;
    

  result = WlanEnumInterfaces(handleclient, NULL, &interfacelist);
  if(result != ERROR_SUCCESS) return false;
    

  for(int i = 0; i < (int)interfacelist->dwNumberOfItems; i++)
    {
      interfacelinfo = (WLAN_INTERFACE_INFO *) &interfacelist->InterfaceInfo[i];
      
      bool isconnected     = false;
      bool isdisconnected  = false;

      switch (interfacelinfo->isState)
        {
          case wlan_interface_state_not_ready               : break;
          case wlan_interface_state_connected               : isconnected     = true;    break;
          case wlan_interface_state_ad_hoc_network_formed   : break;
          case wlan_interface_state_disconnecting           : break;
          case wlan_interface_state_disconnected            : isdisconnected  = true;    break;
          case wlan_interface_state_associating             : break;
          case wlan_interface_state_discovering             : break;
          case wlan_interface_state_authenticating          : break;
                                          default           : break;
        }

      if((!isconnected) ||  (isdisconnected))  status = true;
      
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
* @fn         bool DIOWINDOWSWIFIMANAGERMODE::Disconnect()
* @brief      Disconnect
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSWIFIMANAGERMODE::Disconnect()
{
  HANDLE                        handleclient      = NULL;
  DWORD                         max_client        = 2;
  DWORD                         current_version   = 0;
  PWLAN_INTERFACE_INFO_LIST     interfacelist     = NULL;
  PWLAN_INTERFACE_INFO          interfaceinfo     = NULL;
  DWORD                         result            = 1;
   
  result = WlanOpenHandle(max_client, NULL, &current_version, &handleclient);
  if(result != ERROR_SUCCESS) return false;

  result = WlanEnumInterfaces(handleclient, NULL, &interfacelist);
  if(result != ERROR_SUCCESS) return false;

  for(int i = 0; i < (int) interfacelist->dwNumberOfItems; i++)
    {
      interfaceinfo = (WLAN_INTERFACE_INFO *) &interfacelist->InterfaceInfo[i];
           
      result = WlanDisconnect(handleclient, &interfaceinfo->InterfaceGuid, NULL); // disconnect first 
      if(result == ERROR_SUCCESS) break;
    } 


  if(interfacelist != NULL)
    {
      WlanFreeMemory(interfacelist);
      interfacelist = NULL;
    }

  WlanCloseHandle(handleclient, NULL);

  return (result==ERROR_SUCCESS)?true:false;
}  


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSWIFIMANAGERMODE::IsConnected()
* @brief      Is connected
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSWIFIMANAGERMODE::IsConnected()
{
  HANDLE                        handleclient      = NULL;
  DWORD                         max_client        = 2;
  DWORD                         current_version   = 0;
  DWORD                         result            = 0;
  PWLAN_INTERFACE_INFO_LIST     interfacelist     = NULL;
  PWLAN_INTERFACE_INFO          interfacelinfo    = NULL;
  bool                          status            = false;

  result = WlanOpenHandle(max_client, NULL, &current_version, &handleclient);
  if(result != ERROR_SUCCESS) return false;
   
  result = WlanEnumInterfaces(handleclient, NULL, &interfacelist);
  if(result != ERROR_SUCCESS) return false;
    
  for(int i = 0; i < (int)interfacelist->dwNumberOfItems; i++)
    {
      interfacelinfo = (WLAN_INTERFACE_INFO *) &interfacelist->InterfaceInfo[i];
      
      bool isconnected     = false;
      
      switch (interfacelinfo->isState)
        {
          case wlan_interface_state_not_ready               : break;
          case wlan_interface_state_connected               : isconnected     = true;    break;
          case wlan_interface_state_ad_hoc_network_formed   : break;
          case wlan_interface_state_disconnecting           : break;
          case wlan_interface_state_disconnected            : break;
          case wlan_interface_state_associating             : break;
          case wlan_interface_state_discovering             : break;
          case wlan_interface_state_authenticating          : break;
                                          default           : break;
        }

      if(isconnected) 
        {          
          status = true;
          break;
        }      
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
* @fn         void DIOWINDOWSWIFIMANAGERMODE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWINDOWSWIFIMANAGERMODE::Clean()
{

}



