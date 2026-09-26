/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOWINDOWSStreamUSBLocalEnumDevices.cpp
* 
* @class      DIOWINDOWSSTREAMUSBLOCALENUMDEVICES
* @brief      WINDOWS Data Input/Output Stream USB Local Enum Devices class
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



#if defined(DIO_ACTIVE) && defined(DIO_STREAMUSB_ACTIVE)


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOWINDOWSStreamUSBLocalEnumDevices.h"

#include <tchar.h>
#include <initguid.h>
#include <stdio.h>
#include <winioctl.h>
#include <cfgmgr32.h>
#include <usbioctl.h>

#include "XTrace.h"

#include "DIOStreamDeviceUSB.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

//This is the GUID for the USB device class
DEFINE_GUID(GUID_DEVINTERFACE_USB_PRINT, 0x28D78FAD , 0x5A12, 0x11D1, 0xAE, 0x5B, 0x00, 0x00, 0xF8, 0x03, 0xA8, 0xC2);



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWINDOWSSTREAMUSBLOCALENUMDEVICES::DIOWINDOWSSTREAMUSBLOCALENUMDEVICES() : DIOSTREAMUSBLOCALENUMDEVICES()
* @brief      Constructor of class
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWINDOWSSTREAMUSBLOCALENUMDEVICES::DIOWINDOWSSTREAMUSBLOCALENUMDEVICES() : DIOSTREAMUSBLOCALENUMDEVICES()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWINDOWSSTREAMUSBLOCALENUMDEVICES::~DIOWINDOWSSTREAMUSBLOCALENUMDEVICES()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWINDOWSSTREAMUSBLOCALENUMDEVICES::~DIOWINDOWSSTREAMUSBLOCALENUMDEVICES()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMUSBLOCALENUMDEVICES::Search()
* @brief      Search
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMUSBLOCALENUMDEVICES::Search()
{
  DelAllDevices();

  SearchGUID(&GUID_DEVINTERFACE_USB_DEVICE);
  SearchGUID(&GUID_DEVINTERFACE_USB_PRINT);

  SearchAndDeleteEqualDevices();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMUSBLOCALENUMDEVICES::SearchGUID(const GUID* interfaceclassguid)
* @brief      Search GUID
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  interfaceclassguid : Interfaceclassguid value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMUSBLOCALENUMDEVICES::SearchGUID(const GUID* interfaceclassguid)
{
  HDEVINFO                         hdevinfo;
  SP_DEVICE_INTERFACE_DATA         devinterfacedata;
  PSP_DEVICE_INTERFACE_DETAIL_DATA devinterfacedetaildata;
  SP_DEVINFO_DATA                  devinfodata;
  DWORD                            size;
  DWORD                            memberindex = 0;

  // We will try to get device information set for all USB devices that have a device interface and are currently present on the system (plugged in).
  //hdevinfo = SetupDiGetClassDevs(NULL, NULL, NULL, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT | DIGCF_ALLCLASSES);

  hdevinfo = SetupDiGetClassDevs(&GUID_CLASS_USB_DEVICE, NULL, NULL, (DIGCF_PRESENT | DIGCF_DEVICEINTERFACE));
  if(hdevinfo != INVALID_HANDLE_VALUE)
    {
      memset(&devinterfacedata, 0, sizeof(SP_DEVICE_INTERFACE_DATA));

      devinterfacedata.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
      memberindex = 0;

      // Next, we will keep calling this SetupDiEnumDeviceInterfaces(..) until this function causes GetLastError() to return  ERROR_NO_MORE_ITEMS. With each call the memberindex value needs to be incremented to retrieve the next device interface information.
      SetupDiEnumDeviceInterfaces(hdevinfo, NULL, interfaceclassguid, memberindex, &devinterfacedata);

      while(GetLastError() != ERROR_NO_MORE_ITEMS)
        {
          // As a last step we will need to get some more details for each of device interface information we are able to retrieve. This device interface detail gives us the information we need to identify the device (VID/PID), and decide if it's useful to us. It will also provide a DEVINFO_DATA structure which we can use to know the serial port name for a virtual com port.
          devinfodata.cbSize = sizeof(devinfodata);

          // Get the required buffer size. Call SetupDiGetDeviceInterfaceDetail with a NULL devinterfacedetaildata pointer, a devinterfacedetaildataSize  of zero, and a valid RequiredSize variable. In response to such a call, this function returns the required buffer size at size.
          SetupDiGetDeviceInterfaceDetail(hdevinfo, &devinterfacedata, NULL, 0, &size, NULL);

          // Allocate memory for the DeviceInterfaceDetail struct. Don't forget to deallocate it later!
          devinterfacedetaildata = (PSP_DEVICE_INTERFACE_DETAIL_DATA) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size);
          if(devinterfacedetaildata)
            {
              devinterfacedetaildata->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

              if(SetupDiGetDeviceInterfaceDetail(hdevinfo, &devinterfacedata, devinterfacedetaildata, size, &size, &devinfodata))
                {
                  DIOSTREAMDEVICEUSB* device = GEN_NEW DIOSTREAMDEVICEUSB();
                  if(device)
                    {
                      device->SetIndex(devices.GetSize());
                      device->SetType(DIOSTREAMDEVICE_TYPE_USB);

                      device->GetResource()->Set(devinterfacedetaildata->DevicePath);

                      XSTRING   trash[2];
                      XSTRING   serial;
                      int       vendorID;
                      int       productID;

                      trash[0].AdjustSize(_MAXSTR);
                      trash[1].AdjustSize(_MAXSTR);
                      serial.AdjustSize(_MAXSTR);

                      device->GetResource()->UnFormat(__L("%s#vid_%04x&pid_%04x#%s#%s"), trash[0].Get(), &vendorID, &productID, serial.Get(), trash[1].Get());

                      trash[0].AdjustSize();
                      trash[1].AdjustSize();
                      serial.AdjustSize();

                      device->GetSerialString()->Set(serial);
                      device->SetVendorID(vendorID);
                      device->SetProductID(productID);

                      GetInfoFromRegistry(device);
                      GetUSBDescriptor(hdevinfo, memberindex, device);

                      //device->DebugPrintInfo();

                      devices.Add(device);
                    }
                }
            }

          HeapFree(GetProcessHeap(), 0, devinterfacedetaildata);

          SetupDiEnumDeviceInterfaces( hdevinfo, NULL, interfaceclassguid, ++memberindex, &devinterfacedata);
        }

      SetupDiDestroyDeviceInfoList(hdevinfo);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMUSBLOCALENUMDEVICES::GetInfoFromRegistry(DIOSTREAMDEVICEUSB* device)
* @brief      Get info from registry
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  device : Device pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMUSBLOCALENUMDEVICES::GetInfoFromRegistry(DIOSTREAMDEVICEUSB* device)
{
  XSTRING name;
  HKEY    hkey;
  LONG    statusreg;
  XCHAR   value[_MAXSTR];
  DWORD   sizevalue;
  DWORD   type;
  int     indexcut;
  bool    status;

  name.Format(__L("SYSTEM\\CurrentControlSet\\Enum\\USB\\VID_%04X&PID_%04X\\%s"), device->GetVendorID(), device->GetProductID(), device->GetSerialString()->Get());
  name.ToUpperCase();

  statusreg = RegOpenKeyEx(HKEY_LOCAL_MACHINE, name.Get(), 0, KEY_QUERY_VALUE, &hkey);
  if(statusreg == ERROR_SUCCESS)
    {
      sizevalue = _MAXSTR-1;
      memset(value, 0, _MAXSTR);
      statusreg = RegQueryValueEx(hkey, __L("LocationInformation"), NULL, &type, (BYTE*)value, &sizevalue);
      if(statusreg == ERROR_SUCCESS) device->GetLocation()->Set(value);

      sizevalue = _MAXSTR-1;
      memset(value, 0, _MAXSTR);
      statusreg = RegQueryValueEx(hkey, __L("ParentIdPrefix"), NULL, &type, (BYTE*)value, &sizevalue);
      if(statusreg == ERROR_SUCCESS) device->GetParentID()->Set(value);

      sizevalue = _MAXSTR-1;
      memset(value, 0, _MAXSTR);
      statusreg = RegQueryValueEx(hkey, __L("DeviceDesc"), NULL, &type, (BYTE*)value, &sizevalue);
      if(statusreg == ERROR_SUCCESS) device->GetDescription()->Set(value);
      indexcut = device->GetDescription()->Find(__L(";"), true);
      if(indexcut !=  XSTRING_NOTFOUND) device->GetDescription()->DeleteCharacters(0, indexcut+1);

      XSTRING USBclassstring;
      int     USBclass = 0x00;

      sizevalue = _MAXSTR-1;
      memset(value, 0, _MAXSTR);
      statusreg = RegQueryValueEx(hkey, __L("CompatibleIDs"), NULL, &type, (BYTE*)value, &sizevalue);
      if(statusreg == ERROR_SUCCESS) USBclassstring.Set(value);
      if(USBclassstring.GetSize()) USBclassstring.UnFormat(__L("USB\\Class_%02x"), &USBclass);

      device->SetClass((DIOSTREAMDEVICEUSBCLASS)USBclass);


      RegCloseKey(hkey);

      status = device->GetName()->GetSize()?true:false;

    }
   else
    {
      for(int c=0;c<(int)devices.GetSize();c++)
        {
          DIOSTREAMDEVICEUSB* tmpdevice = (DIOSTREAMDEVICEUSB*)devices.Get(c);
          if(tmpdevice)
            {
              if((tmpdevice->GetVendorID()== device->GetVendorID() && (tmpdevice->GetProductID())== device->GetProductID()))
                {
                  if(device->GetSerialString()->Find((*tmpdevice->GetParentID()), true) != XSTRING_NOTFOUND)
                    {
                      for(int d=0; d<99; d++)
                        {
                          name.Format(__L("SYSTEM\\CurrentControlSet\\Enum\\USB\\VID_%04X&PID_%04X&MI_%02d\\%s"), device->GetVendorID(), device->GetProductID(), d, device->GetSerialString()->Get());
                          name.ToUpperCase();

                          statusreg = RegOpenKeyEx(HKEY_LOCAL_MACHINE, name.Get(), 0, KEY_QUERY_VALUE, &hkey);
                          if(statusreg == ERROR_SUCCESS)
                            {
                              device->GetLocation()->Set((*tmpdevice->GetLocation()));

                              sizevalue = _MAXSTR-1;
                              memset(value, 0, _MAXSTR);
                              statusreg = RegQueryValueEx(hkey, __L("DeviceDesc"), NULL, &type, (BYTE*)value, &sizevalue);
                              if(statusreg == ERROR_SUCCESS) device->GetDescription()->Set(value);
                              indexcut = device->GetDescription()->Find(__L(";"), true);
                              if(indexcut !=  XSTRING_NOTFOUND) device->GetDescription()->DeleteCharacters(0, indexcut+1);

                              XSTRING USBclassstring;
                              int     USBclass = 0x00;

                              sizevalue = _MAXSTR-1;
                              memset(value, 0, _MAXSTR);
                              statusreg = RegQueryValueEx(hkey, __L("CompatibleIDs"), NULL, &type, (BYTE*)value, &sizevalue);
                              if(statusreg == ERROR_SUCCESS) USBclassstring.Set(value);
                              if(USBclassstring.GetSize()) USBclassstring.UnFormat(__L("USB\\Class_%02x"), &USBclass);

                              device->SetClass((DIOSTREAMDEVICEUSBCLASS)USBclass);

                              RegCloseKey(hkey);

                              break;
                            }
                        }
                    }
                }
            }
        }

       status = false;
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMUSBLOCALENUMDEVICES::GetUSBDescriptor(HDEVINFO& hdevinfo, int memberindex, DIOSTREAMDEVICEUSB* device)
* @brief      Get USB descriptor
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  hdevinfo : Hdevinfo value.
* @param[in]  memberindex : Memberindex value.
* @param[in]  device : Device pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMUSBLOCALENUMDEVICES::GetUSBDescriptor(HDEVINFO& hdevinfo, int memberindex, DIOSTREAMDEVICEUSB* device)
{
  SP_DEVINFO_DATA devicedata;
  DEVINST         parentdevinst = 0;
  wchar_t         deviceID[MAX_PATH];
  XSTRING         devIDstring;
  GUID            classGUID     = GUID_CLASS_USBHUB;
  XSTRING         guidString;
  bool            status        = false;

  devicedata.cbSize = sizeof(SP_DEVINFO_DATA);

  SetupDiEnumDeviceInfo(hdevinfo, memberindex, &devicedata);

  CM_Get_Parent(&parentdevinst, devicedata.DevInst, 0);
  CM_Get_Device_ID(parentdevinst, deviceID, MAX_PATH, 0);

  devIDstring = deviceID;
  devIDstring.Character_Change(__C('\\'), __C('#'));
  devIDstring.Insert(__L("\\\\?\\"), 0);
  devIDstring.Add(__L("#"));

  guidString.Format(__L("{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}"), classGUID.Data1, classGUID.Data2, classGUID.Data3, classGUID.Data4[0], classGUID.Data4[1], classGUID.Data4[2], classGUID.Data4[3], classGUID.Data4[4], classGUID.Data4[5], classGUID.Data4[6], classGUID.Data4[7]);

  devIDstring.Add(guidString);

  HANDLE handle = CreateFile(devIDstring.Get(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
  if(handle)
    {
      DWORD  usbportnumber = 0;
      DWORD  requiredsize  = 0;

      if(SetupDiGetDeviceRegistryProperty(hdevinfo, &devicedata, SPDRP_ADDRESS, NULL, (PBYTE)&usbportnumber, sizeof(usbportnumber), &requiredsize))
        {
          USB_DESCRIPTOR_REQUEST*  requestpacket    = NULL;
          USB_DEVICE_DESCRIPTOR*   devicedescriptor = NULL;
          int                      buffersize       = sizeof(USB_DESCRIPTOR_REQUEST) + sizeof(USB_DEVICE_DESCRIPTOR);
          XBYTE*                   buffer           = GEN_NEW XBYTE[buffersize];

          if(buffer)
            {
              // We know from out previous discussion that the first part of the buffer contains the request packet, and latter part contains the data to be filled by the IOCTL - in our case the device descriptor
              requestpacket      = (USB_DESCRIPTOR_REQUEST*)buffer;
              devicedescriptor   = (USB_DEVICE_DESCRIPTOR*)((BYTE*)buffer + sizeof(USB_DESCRIPTOR_REQUEST));

              //fill information in packet
              requestpacket->SetupPacket.bmRequest  = 0x80;
              requestpacket->SetupPacket.bRequest   = USB_REQUEST_GET_DESCRIPTOR;
              requestpacket->ConnectionIndex        = usbportnumber;
              requestpacket->SetupPacket.wValue     = (USB_DEVICE_DESCRIPTOR_TYPE << 8 | 0 ); // Since only 1 device descriptor => index : 0
              requestpacket->SetupPacket.wLength    = sizeof(USB_DEVICE_DESCRIPTOR);

              // Issue ioctl
              DWORD bytesreturned = 0;
              status = DeviceIoControl(handle, IOCTL_USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION, buffer, buffersize, buffer, buffersize, &bytesreturned, NULL);
              if(status)
                {
                  device->GetDescriptor()->SetBCDUSB(devicedescriptor->bcdUSB);
                  device->GetDescriptor()->SetDeviceClass(devicedescriptor->bDeviceClass);
                  device->GetDescriptor()->SetDeviceSubClass(devicedescriptor->bDeviceSubClass);
                  device->GetDescriptor()->SetDeviceProtocol(devicedescriptor->bDeviceProtocol);
                  device->GetDescriptor()->SetMaxPacketSize(devicedescriptor->bMaxPacketSize0);
                  device->GetDescriptor()->SetIDVendor(devicedescriptor->idVendor);
                  device->GetDescriptor()->SetIDProduct(devicedescriptor->idProduct);
                  device->GetDescriptor()->SetBCDDevice(devicedescriptor->bcdDevice);
                  device->GetDescriptor()->SetManufacturer(devicedescriptor->iManufacturer);
                  device->GetDescriptor()->SetProduct(devicedescriptor->iProduct);
                  device->GetDescriptor()->SetSerialNumber(devicedescriptor->iSerialNumber);
                  device->GetDescriptor()->SetNumConfigurations(devicedescriptor->bNumConfigurations);
                }

              GEN_DELETE_ARRAY buffer;
            }
        }

      CloseHandle(handle);
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWINDOWSSTREAMUSBLOCALENUMDEVICES::SearchAndDeleteEqualDevices()
* @brief      Search and GEN_DELETE equal devices
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWINDOWSSTREAMUSBLOCALENUMDEVICES::SearchAndDeleteEqualDevices()
{
  if(devices.IsEmpty()) return false;

  bool found;

  do{ found = false;

      for(int c=0; c<(int)devices.GetSize(); c++)
        {
          DIOSTREAMDEVICEUSB* device1 = (DIOSTREAMDEVICEUSB*)devices.Get(c);
          if(device1)
            {
              for(int d=0; d<(int)devices.GetSize(); d++)
                {
                  DIOSTREAMDEVICEUSB* device2 = (DIOSTREAMDEVICEUSB*)devices.Get(d);
                  if(device2 && (c!=d))
                    {
                      if((!device1->GetLocation()->Compare(device2->GetLocation()->Get()) && (device1->GetClass() == device2->GetClass())))
                        {
                          devices.Delete(device1);
                          GEN_DELETE device1;

                          found = true;
                          break;
                        }
                    }
                }

              if(found) break;
            }
        }

      if(!found) break;

    } while(1);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWINDOWSSTREAMUSBLOCALENUMDEVICES::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWINDOWSSTREAMUSBLOCALENUMDEVICES::Clean()
{

}




#endif

