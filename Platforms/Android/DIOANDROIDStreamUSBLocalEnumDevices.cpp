/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOANDROIDStreamUSBLocalEnumDevices.cpp
* 
* @class      DIOANDROIDSTREAMUSBLOCALENUMDEVICES
* @brief      ANDROID Data Input/Output Stream USB Local Enum Devices class
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



#if defined(DIO_ACTIVE) && defined(DIO_STREAMUSB_ACTIVE)


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOANDROIDStreamUSBLocalEnumDevices.h"

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "XTrace.h"
#include "XPath.h"
#include "DIOStreamDeviceUSB.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOANDROIDSTREAMUSBLOCALENUMDEVICES::DIOANDROIDSTREAMUSBLOCALENUMDEVICES() : DIOSTREAMUSBLOCALENUMDEVICES()
* @brief      Constructor of class
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOANDROIDSTREAMUSBLOCALENUMDEVICES::DIOANDROIDSTREAMUSBLOCALENUMDEVICES() : DIOSTREAMUSBLOCALENUMDEVICES()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOANDROIDSTREAMUSBLOCALENUMDEVICES::~DIOANDROIDSTREAMUSBLOCALENUMDEVICES()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_ANDROID
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOANDROIDSTREAMUSBLOCALENUMDEVICES::~DIOANDROIDSTREAMUSBLOCALENUMDEVICES()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOANDROIDSTREAMUSBLOCALENUMDEVICES::Search()
* @brief      Search
* @ingroup    PLATFORM_ANDROID
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOANDROIDSTREAMUSBLOCALENUMDEVICES::Search()
{
  DelAllDevices();

  struct udev*              udev                = NULL;
  struct udev_enumerate*    udevenumerate       = NULL;
  struct udev_list_entry*   udevdeviceslist     = NULL;
  struct udev_list_entry*   udevdevicelistentry = NULL;
  int                       index = 0;

  // Create the udev object
  udev = udev_new();
  if (!udev) return false;

  udevenumerate   = udev_enumerate_new(udev);
//udev_enumerate_add_match_subsystem(udev_enumerate, "usb");
  udev_enumerate_scan_devices(udevenumerate);

  udevdeviceslist = udev_enumerate_get_list_entry(udevenumerate);

  udev_list_entry_foreach(udevdevicelistentry, udevdeviceslist)
    {
      struct udev_device* udevdevice          = NULL;
      struct udev_device* udevdeviceinterface = NULL;
      struct udev_device* udevsearchdevice    = NULL;
      const char*         path;
      XSTRING             classstr;

      path              = udev_list_entry_get_name(udevdevicelistentry);
      udevsearchdevice  = udev_device_new_from_syspath(udev, path);

      udevdeviceinterface =  udev_device_get_parent_with_subsystem_devtype(udevsearchdevice, "usb", "usb_interface");
      if(udevdeviceinterface) classstr  = udev_device_get_sysattr_value(udevdeviceinterface, "bInterfaceClass");

      udevdevice = udev_device_get_parent_with_subsystem_devtype(udevsearchdevice, "usb", "usb_device");
      if(udevdevice)
        {
          XSTRING   devicepath;
          XSTRING   typestr;
          XSTRING   hubstr;
          XSTRING   resource;
          XSTRING   vid;
          XSTRING   pid;
          int       vendorID  = 0;
          int       productID = 0;

          devicepath = path;

          typestr   = udev_device_get_devtype(udevdevice);
          hubstr    = udev_device_get_sysattr_value(udevdevice, "busnum");
          resource  = udev_device_get_devnode(udevsearchdevice);
          vid       = udev_device_get_sysattr_value(udevdevice, "idVendor");
          pid       = udev_device_get_sysattr_value(udevdevice, "idProduct");

          if(!resource.IsEmpty())
            {
              if(resource.Find(_L("/dev/bus/"), true) == XSTRING_NOTFOUND)
                {
                  int       port;
                  int       empty[2];
                  XSTRING   string[2];

                  string[0].AdjustSize(_MAXSTR);
                  string[1].AdjustSize(_MAXSTR);

                  devicepath.UnFormat(_L("%s-%d.%d:%d%s"), string[0].Get(), &empty[0], &port, &empty[1], string[1].Get());

                  string[0].AdjustSize();
                  string[1].AdjustSize();

                  //XTRACE_PRINTCOLOR(2, _L("%s - [port %d] class[%s]"), devicepath.Get(), port, classstr.Get());

                  DIOSTREAMDEVICEUSB* device = GEN_NEW DIOSTREAMDEVICEUSB();
                  if(device)
                    {
                      vid.UnFormat(_L("%08X"), &vendorID);
                      pid.UnFormat(_L("%08X"), &productID);

                      device->SetIndex(index);
                      device->SetType(DIOSTREAMDEVICE_TYPE_USB);

                      device->GetLocation()->Format(_L("Port_#%04d.Hub_#%04d"), port, hubstr.ConvertToInt());
                      device->GetDescription()->Set(udev_device_get_sysattr_value(udevdevice, "product"));
                      device->GetResource()->Set(resource);

                      device->SetVendorID(vendorID);
                      device->SetProductID(productID);

                      device->SetClass((DIOSTREAMDEVICEUSBCLASS)classstr.ConvertToInt());
                      device->GetSerialString()->Set(udev_device_get_sysattr_value(udevdevice, "serial"));

                      devices.Add(device);

                      index++;
                    }
                }
            }
        }

      if(udevsearchdevice)
        {
          udev_device_unref(udevsearchdevice);
          udevsearchdevice    = NULL;
        }
    }

  // Free the enumerator object
  if(udevenumerate) udev_enumerate_unref(udevenumerate);
  if(udev)          udev_unref(udev);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOANDROIDSTREAMUSBLOCALENUMDEVICES::IsDeviceAvailable(XCHAR* resource)
* @brief      Is device available
* @ingroup    PLATFORM_ANDROID
* 
* @param[in]  resource : Resource pointer to use.
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOANDROIDSTREAMUSBLOCALENUMDEVICES::IsDeviceAvailable(XCHAR* resource)
{

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOANDROIDSTREAMUSBLOCALENUMDEVICES::SearchAndDeleteEqualDevices()
* @brief      Search and GEN_DELETE equal devices
* @ingroup    PLATFORM_ANDROID
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOANDROIDSTREAMUSBLOCALENUMDEVICES::SearchAndDeleteEqualDevices()
{
  if(devices.IsEmpty()) return false;

  bool found;

  do{ found = false;

      for(int c=0; c<devices.GetSize(); c++)
        {
          DIOSTREAMDEVICEUSB* device1 = (DIOSTREAMDEVICEUSB*)devices.Get(c);
          if(device1)
            {
              for(int d=0; d<devices.GetSize(); d++)
                {
                  DIOSTREAMDEVICEUSB* device2 = (DIOSTREAMDEVICEUSB*)devices.Get(d);
                  if(device2 && (c!=d))
                    {
                      if(!device1->GetName()->Compare(device2->GetName()->Get()))
                        {
                          GEN_DELETE device1;
                          devices.Delete(device1);

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




#endif

