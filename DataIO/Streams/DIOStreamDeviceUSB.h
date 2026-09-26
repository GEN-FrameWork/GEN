/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOStreamDeviceUSB.h
* 
* @class      DIOSTREAMDEVICEUSB
* @brief      Data Input/Output Stream Device USB class
* @ingroup    DATAIO
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

#include "DIOStreamDevice.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum DIOSTREAMDEVICEUSBCLASS
{
  DIOSTREAMDEVICEUSBCLASS_DEVICE_USECLASSINFORMATIONINTERFACEDESCRIPTORS  = 0x00 ,
  DIOSTREAMDEVICEUSBCLASS_INTERFACE_AUDIO                                 = 0x01 ,
  DIOSTREAMDEVICEUSBCLASS_BOTH_COMMUNICATIONS_CDC_CONTROL                 = 0x02 ,
  DIOSTREAMDEVICEUSBCLASS_INTERFACE_HID                                   = 0x03 ,
  DIOSTREAMDEVICEUSBCLASS_INTERFACE_PHYSICAL                              = 0x05 ,
  DIOSTREAMDEVICEUSBCLASS_INTERFACE_IMAGE                                 = 0x06 ,
  DIOSTREAMDEVICEUSBCLASS_INTERFACE_PRINTER                               = 0x07 ,
  DIOSTREAMDEVICEUSBCLASS_INTERFACE_MASSSTORAGE                           = 0x08 ,
  DIOSTREAMDEVICEUSBCLASS_DEVICE_HUB                                      = 0x09 ,
  DIOSTREAMDEVICEUSBCLASS_INTERFACE_CDC_DATA                              = 0x0A ,
  DIOSTREAMDEVICEUSBCLASS_INTERFACE_SMARTCARD                             = 0x0B ,
  DIOSTREAMDEVICEUSBCLASS_INTERFACE_CONTENTSECURITY                       = 0x0D ,
  DIOSTREAMDEVICEUSBCLASS_INTERFACE_VIDEO                                 = 0x0E ,
  DIOSTREAMDEVICEUSBCLASS_INTERFACE_PERSONAL_HEALTHCARE                   = 0x0F ,
  DIOSTREAMDEVICEUSBCLASS_INTERFACE_AUDIO_VIDEO_DEVICES                   = 0x10 ,
  DIOSTREAMDEVICEUSBCLASS_DEVICE_BILLBOARD_DEVICE                         = 0x11 ,
  DIOSTREAMDEVICEUSBCLASS_BOTH_DIAGNOSTIC_DEVICE                          = 0xDC ,
  DIOSTREAMDEVICEUSBCLASS_INTERFACE_WIRELESS_CONTROLLER                   = 0xE0 ,
  DIOSTREAMDEVICEUSBCLASS_BOTH_MISCELLANEOUS                              = 0xEF ,
  DIOSTREAMDEVICEUSBCLASS_INTERFACE_APPLICATION_SPECIFIC                  = 0xFE ,
  DIOSTREAMDEVICEUSBCLASS_BOTH_VENDOR_SPECIFIC                            = 0xFF ,
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOSTREAMDEVICEUSB_DESCRIPTOR
{
  public :
                                                    DIOSTREAMDEVICEUSB_DESCRIPTOR           ();
                                                   ~DIOSTREAMDEVICEUSB_DESCRIPTOR           ();

    XWORD                                           GetBCDUSB                               ();
    void                                            SetBCDUSB                               (XWORD BCDUSB);

    XBYTE                                           GetDeviceClass                          ();
    void                                            SetDeviceClass                          (XBYTE deviceclass);

    XBYTE                                           GetDeviceSubClass                       ();
    void                                            SetDeviceSubClass                       (XBYTE devicesubclass);

    XBYTE                                           GetDeviceProtocol                       ();
    void                                            SetDeviceProtocol                       (XBYTE deviceprotocol);

    XBYTE                                           GetMaxPacketSize                        ();
    void                                            SetMaxPacketSize                        (XBYTE maxpacketsize);

    XWORD                                           GetIDVendor                             ();
    void                                            SetIDVendor                             (XWORD IDvendor);

    XWORD                                           GetIDProduct                            ();
    void                                            SetIDProduct                            (XWORD IDproduct);

    XWORD                                           GetBCDDevice                            ();
    void                                            SetBCDDevice                            (XWORD BCDdevice);

    XBYTE                                           GetManufacturer                         ();
    void                                            SetManufacturer                         (XBYTE manufacturer);

    XBYTE                                           GetProduct                              ();
    void                                            SetProduct                              (XBYTE product);

    XBYTE                                           GetSerialNumber                         ();
    void                                            SetSerialNumber                         (XBYTE serialnumber);

    XBYTE                                           GetNumConfigurations                    ();
    void                                            SetNumConfigurations                    (XBYTE numconfigurations);

  private:

    void                                            Clean                                   ();

    XWORD                                           BCDUSB;
    XBYTE                                           deviceclass;
    XBYTE                                           devicesubclass;
    XBYTE                                           deviceprotocol;
    XBYTE                                           maxpacketsize;
    XWORD                                           IDvendor;
    XWORD                                           IDproduct;
    XWORD                                           BCDdevice;
    XBYTE                                           manufacturer;
    XBYTE                                           product;
    XBYTE                                           serialnumber;
    XBYTE                                           numconfigurations;
};

class DIOSTREAMDEVICEUSB : public DIOSTREAMDEVICE
{
  public:

                                                    DIOSTREAMDEVICEUSB                      ();
    virtual                                        ~DIOSTREAMDEVICEUSB                      ();

    bool                                            IsHUB                                   ();
    void                                            SetIsHUB                                (bool isHUB);

    XSTRING*                                        GetLocation                             ();

    XSTRING*                                        GetParentID                             ();

    XWORD                                           GetVendorID                             ();
    void                                            SetVendorID                             (XWORD vendorID);

    XWORD                                           GetProductID                            ();
    void                                            SetProductID                            (XWORD productID);

    DIOSTREAMDEVICEUSBCLASS                         GetClass                                ();
    void                                            SetClass                                (DIOSTREAMDEVICEUSBCLASS classdev);

    XSTRING*                                        GetSerialString                         ();

    DIOSTREAMDEVICEUSB_DESCRIPTOR*                  GetDescriptor                           ();

    bool                                            DebugPrintInfo                          ();

  protected:

    bool                                            isHUB;
    XSTRING                                         location;
    XSTRING                                         parentID;
    XWORD                                           vendorID;
    XWORD                                           productID;
    DIOSTREAMDEVICEUSBCLASS                         classdev;
    XSTRING                                         serialstring;

    DIOSTREAMDEVICEUSB_DESCRIPTOR                   descriptor;

  private:

    void                                            Clean                                   ();
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





