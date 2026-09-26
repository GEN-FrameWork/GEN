/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOLINUXStreamUSBLocalEnumDevices.h
* 
* @class      DIOLINUXSTREAMUSBLOCALENUMDEVICES
* @brief      LINUX Data Input/Output Stream USB Local Enum Devices class
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

#if defined(DIO_ACTIVE) && defined(DIO_STREAMUSB_ACTIVE)

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XString.h"

#include "DIOStreamUSBLocalEnumDevices.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/




/*---- CLASS ---------------------------------------------------------------------------------------------------------*/


class DIOLINUXSTREAMUSBLOCALENUMDEVICES :  public DIOSTREAMUSBLOCALENUMDEVICES
{
  public:
                          DIOLINUXSTREAMUSBLOCALENUMDEVICES           ();
    virtual              ~DIOLINUXSTREAMUSBLOCALENUMDEVICES           ();

    bool                  Search                                      ();
    bool                  IsDeviceAvailable                           (XCHAR* resource);

  private:

    bool                  GetDescriptor                               (struct udev_device* udevdevice, DIOSTREAMDEVICEUSB_DESCRIPTOR* descriptor);
    bool                  SearchAndDeleteEqualDevices                 ();

};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




#endif







