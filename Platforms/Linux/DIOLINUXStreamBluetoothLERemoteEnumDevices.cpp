/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOLINUXStreamBluetoothLERemoteEnumDevices.cpp
* 
* @class      DIOLINUXSTREAMBLUETOOTHLEREMOTEENUMDEVICES
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"



#if defined(DIO_ACTIVE) && defined(DIO_STREAMBLUETOOTHLE_ACTIVE)


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOLINUXStreamBluetoothLERemoteEnumDevices.h"

#include "XFactory.h"
#include "XSleep.h"
#include "XVector.h"
#include "XString.h"

#include "DIOStreamDeviceBluetoothLE.h"

#include "DIOStreamBluetoothLERemoteEnumDevices_XEvent.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOLINUXSTREAMBLUETOOTHLEREMOTEENUMDEVICES::DIOLINUXSTREAMBLUETOOTHLEREMOTEENUMDEVICES() : DIOSTREAMBLUETOOTHLEREMOTEENUMDEVICES()
* @brief      Constructor of class
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOLINUXSTREAMBLUETOOTHLEREMOTEENUMDEVICES::DIOLINUXSTREAMBLUETOOTHLEREMOTEENUMDEVICES() : DIOSTREAMBLUETOOTHLEREMOTEENUMDEVICES() 
{
  Clean();
  
  RegisterEvent(DIOSTREAMBLUETOOTH_XEVENT_TYPE_ENUMREMOTEBLUETOOTHLE_DEVICEFOUND);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOLINUXSTREAMBLUETOOTHLEREMOTEENUMDEVICES::~DIOLINUXSTREAMBLUETOOTHLEREMOTEENUMDEVICES()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOLINUXSTREAMBLUETOOTHLEREMOTEENUMDEVICES::~DIOLINUXSTREAMBLUETOOTHLEREMOTEENUMDEVICES()
{
  DeRegisterEvent(DIOSTREAMBLUETOOTH_XEVENT_TYPE_ENUMREMOTEBLUETOOTHLE_DEVICEFOUND);

  StopSearch(false);

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXSTREAMBLUETOOTHLEREMOTEENUMDEVICES::Search()
* @brief      Search
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXSTREAMBLUETOOTHLEREMOTEENUMDEVICES::Search()
{
  bool status = false;

  if(threadenumdevices)      return false;  
  if(threadenumdevicesevent) return false;  


  DelAllDevices();

  memset(&hcidevinfo, 0x00, sizeof(hcidevinfo));

  DIOMAC* MAC = GetDeviceSelect();
  if(MAC)
    {
      if(!MAC->IsZero())
        {
          XSTRING MACstr; 

          MAC->GetXString(MACstr);
      
          XBUFFER charstr;
          
          MACstr.ConvertToASCII(charstr);          
          hcideviceID = hci_devid(charstr.GetPtrChar());
          
          //XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[Blueooth LE enum Remote]  Select local bluetooth: %s"), MACstr.Get());
        }
       else 
        {
          hcideviceID =  hci_get_route(NULL);
          //XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[Blueooth LE enum Remote]  Default local bluetooth."));
        }
    }
   else
    {
      hcideviceID =  hci_get_route(NULL);
    }
  
  if(hcideviceID < 0) hcideviceID = 0;        // use device 0, if device id is invalid

  //XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[LE Remote Search] start device [%d]..."), hcideviceID);
    
  hcisocket = hci_open_dev(hcideviceID);
  if(hcisocket == -1) 
    {    
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("[Blueooth LE enum Remote]  error to hci_copen_dev. "));
      return false;
    }

  hcidevinfo.dev_id = hcideviceID;

  // ----------------------------------------------------------------------------------------------  
  // Get old HCI filter

  hci_filter_clear(&old_hcifilter);
  socklen_t sizefilter = sizeof(old_hcifilter);
  getsockopt(hcisocket, SOL_HCI, HCI_FILTER, &old_hcifilter, &sizefilter);


  // ----------------------------------------------------------------------------------------------  
  // Setup GEN_NEW HCI filter

  hci_filter_clear(&new_hcifilter);
  hci_filter_set_ptype(HCI_EVENT_PKT, &new_hcifilter);
  hci_filter_set_event(EVT_LE_META_EVENT, &new_hcifilter);

  setsockopt(hcisocket, SOL_HCI, HCI_FILTER, &new_hcifilter, sizeof(new_hcifilter));

  // ----------------------------------------------------------------------------------------------  
  // Activate search

  hci_le_set_scan_enable(hcisocket, 0x00, 0, 1000);
  hci_le_set_scan_parameters(hcisocket, 0x00, htobs(0x0080), htobs(0x0080), 0x00, 0x00, 1000);    
  //hci_le_set_scan_parameters(hcisocket, 0x00, htobs(0x0080), htobs(0x0080), 0x00, 0x00, 1000);    
  //hci_le_set_scan_parameters(hcisocket, 0x00, htobs(0x3E8), htobs(0x3E8), 0x00, 0x00, 100);    
  hci_le_set_scan_enable(hcisocket, 0x01, 0, 1000);
  

  xmutexdevicesevent=GEN_XFACTORY.Create_Mutex();
  if(!xmutexdevicesevent) return false;
  
  threadenumdevices = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMBLUETOOTH, __L("DIOLINUXSTREAMBLUETOOTHLEREMOTEENUMDEVICES::DIOLINUXSTREAMBLUETOOTHLEREMOTEENUMDEVICES"), ThreadEnumDevices, (void*)this);
  if(!threadenumdevices)  return false;

  threadenumdevices->SetPriority(XTHREADPRIORITY_REALTIME);

  status = threadenumdevices->Ini();

  if(GetSearchMode() & DIOSTREAMBLUETOOTHLEREMOTEENUMDEVICES_SEARCHMODE_EVENT)
    {
      threadenumdevicesevent = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMBLUETOOTH, __L("DIOLINUXSTREAMBLUETOOTHLEREMOTEENUMDEVICES::DIOLINUXSTREAMBLUETOOTHLEREMOTEENUMDEVICES_EVENT"), ThreadEnumDevicesEvent, (void*)this);
      if(!threadenumdevicesevent)  return false;

      //threadenumdevicesevent->SetPriority(XTHREADPRIORITY_HIGH);
      
      status = threadenumdevicesevent->Ini();
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXSTREAMBLUETOOTHLEREMOTEENUMDEVICES::StopSearch(bool waitend)
* @brief      Stop search
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  waitend : Waitend value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXSTREAMBLUETOOTHLEREMOTEENUMDEVICES::StopSearch(bool waitend)
{

  if(threadenumdevicesevent) 
    {
      threadenumdevicesevent->End();
      DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMBLUETOOTH, threadenumdevicesevent);

      threadenumdevicesevent = NULL;
    } 

  if(threadenumdevices) 
    {
      threadenumdevices->End();
      DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMBLUETOOTH, threadenumdevices);

      threadenumdevices = NULL;
    } 

  if(xmutexdevicesevent)
    {
      GEN_XFACTORY.Delete_Mutex(xmutexdevicesevent);
      xmutexdevicesevent = NULL;
    }

  
  // disable LE scan
  hci_le_set_scan_enable(hcisocket, 0x00, 0, 1000);

  // restore original filter
  setsockopt(hcisocket, SOL_HCI, HCI_FILTER, &old_hcifilter, sizeof(old_hcifilter));

  close(hcisocket);

  hcisocket = -1;

  // XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("[LE Remote Search] stop...."));
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOLINUXSTREAMBLUETOOTHLEREMOTEENUMDEVICES::IsSearching()
* @brief      Is searching
* @ingroup    PLATFORM_LINUX
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOLINUXSTREAMBLUETOOTHLEREMOTEENUMDEVICES::IsSearching()
{
  if(!threadenumdevices) return false;

  if(!threadenumdevices->IsRunning()) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOLINUXSTREAMBLUETOOTHLEREMOTEENUMDEVICES::ThreadEnumDevices(void* param)
* @brief      Thread enum devices
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  param : Param pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOLINUXSTREAMBLUETOOTHLEREMOTEENUMDEVICES::ThreadEnumDevices(void* param)
{
  DIOLINUXSTREAMBLUETOOTHLEREMOTEENUMDEVICES* LEremotedevices = (DIOLINUXSTREAMBLUETOOTHLEREMOTEENUMDEVICES*)param;
  if(!LEremotedevices) return;

  XBYTE                   hcieventbuffer[HCI_MAX_EVENT_SIZE];
  int                     hcieventsize;
  evt_le_meta_event*      LEmetaevent;
  le_advertising_info*    LEadvertisinginfo;
  char                    LEMACdevicestr[18];

  hcieventsize = read(LEremotedevices->hcisocket, hcieventbuffer, sizeof(hcieventbuffer));

  LEmetaevent   = (evt_le_meta_event *)(hcieventbuffer + (1 + HCI_EVENT_HDR_SIZE));
  hcieventsize -= (1 + HCI_EVENT_HDR_SIZE);
    
  if(LEmetaevent->subevent == EVT_LE_ADVERTISING_REPORT) 
    {    
      LEadvertisinginfo = (le_advertising_info *)(LEmetaevent->data + 1);
  
      ba2str(&LEadvertisinginfo->bdaddr, LEMACdevicestr);
      int8_t rssi = (char)*(LEadvertisinginfo->data + LEadvertisinginfo->length);

      
      XSTRING LEMACdevicestring;      

      LEMACdevicestring.Set(LEMACdevicestr);
      
      if(LEremotedevices->GetSearchMode() & DIOSTREAMBLUETOOTHLEREMOTEENUMDEVICES_SEARCHMODE_ADDLIST)
        {     
           
          bool                        isnewdevice = false;
          DIOSTREAMDEVICEBLUETOOTHLE* device = (DIOSTREAMDEVICEBLUETOOTHLE*)LEremotedevices->GetDeviceByMAC(LEMACdevicestring.Get());
          if(!device) 
            {
              device = GEN_NEW DIOSTREAMDEVICEBLUETOOTHLE();
              isnewdevice = true;
            }       
                
          if(device)
            {
              device->GetMAC()->Set(LEMACdevicestring);
              device->SetRSSI(rssi);
              device->GetAdvertisingData()->Delete();
              device->GetAdvertisingData()->Add(LEadvertisinginfo->data, + LEadvertisinginfo->length);

              //XTRACE_PRINTCOLOR((isnewdevice?XTRACE_COLOR_PURPLE:XTRACE_COLOR_BLUE), __L("MAC %s RSSI: %d"), LEMACdevicestring.Get(), rssi);
              //XTRACE_PRINTDATABLOCKCOLOR((isnewdevice?XTRACE_COLOR_PURPLE:XTRACE_COLOR_BLUE), (*device->GetAdvertisingData()));

              device->GetLastScanTimer()->Reset();
              device->SetIsReadLastScan(false);

              if(isnewdevice) LEremotedevices->GetDevices()->Add(device);             
            }           
         }
      
      if(LEremotedevices->GetSearchMode() & DIOSTREAMBLUETOOTHLEREMOTEENUMDEVICES_SEARCHMODE_EVENT)
        {
          DIOSTREAMDEVICEBLUETOOTHLE* device_event = GEN_NEW DIOSTREAMDEVICEBLUETOOTHLE();
          if(device_event)
            {
                
              // XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("MAC %s RSSI: %d  [%d]"), LEMACdevicestring.Get(), rssi, LEremotedevices->devicesevent.GetSize());
              
              device_event->GetMAC()->Set(LEMACdevicestring);
              device_event->SetRSSI(rssi);              
              device_event->GetAdvertisingData()->Add(LEadvertisinginfo->data, + LEadvertisinginfo->length);

              if(LEremotedevices->xmutexdevicesevent)  
                {                           
                  LEremotedevices->xmutexdevicesevent->Lock();
                  LEremotedevices->devicesevent.Add(device_event);                                    
                  LEremotedevices->xmutexdevicesevent->UnLock();                                      
                }             
            }                     
        }        
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOLINUXSTREAMBLUETOOTHLEREMOTEENUMDEVICES::ThreadEnumDevicesEvent(void* param)
* @brief      Thread enum devices event
* @ingroup    PLATFORM_LINUX
* 
* @param[in]  param : Param pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOLINUXSTREAMBLUETOOTHLEREMOTEENUMDEVICES::ThreadEnumDevicesEvent(void* param)
{
  DIOLINUXSTREAMBLUETOOTHLEREMOTEENUMDEVICES* LEremotedevices = (DIOLINUXSTREAMBLUETOOTHLEREMOTEENUMDEVICES*)param;
  if(!LEremotedevices) return;
  
  if(!LEremotedevices->xmutexdevicesevent) return;
 
  DIOSTREAMDEVICEBLUETOOTHLE* device_event = NULL;

  LEremotedevices->xmutexdevicesevent->Lock();
  device_event = (DIOSTREAMDEVICEBLUETOOTHLE*)LEremotedevices->devicesevent.Get(0);  
  LEremotedevices->xmutexdevicesevent->UnLock();
       
  if(device_event)
    {
      DIOSTREAMBLUETOOTHLEREMOTEENUMDEVICES_XEVENT xevent(LEremotedevices, DIOSTREAMBLUETOOTH_XEVENT_TYPE_ENUMREMOTEBLUETOOTHLE_DEVICEFOUND);
      xevent.SetDeviceSearch(device_event);
                    
      LEremotedevices->PostEvent(&xevent);  
         
      LEremotedevices->xmutexdevicesevent->Lock();       
      LEremotedevices->devicesevent.Delete(device_event);  
        
      /*
      if(LEremotedevices->devicesevent.GetSize() > 100)
        {
          LEremotedevices->devicesevent.DeleteContents();
          LEremotedevices->devicesevent.DeleteAll();      
        }
      */
     
      LEremotedevices->xmutexdevicesevent->UnLock();
    }

  GEN_DELETE device_event;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOLINUXSTREAMBLUETOOTHLEREMOTEENUMDEVICES::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOLINUXSTREAMBLUETOOTHLEREMOTEENUMDEVICES::Clean()
{ 
  threadenumdevices       = NULL;
  threadenumdevicesevent  = NULL;

  hcisocket               = -1;
  hcideviceID             =  0;

  xmutexdevicesevent      = NULL;  

}




#endif

