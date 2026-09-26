/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOFactory.h
* 
* @class      DIOFACTORY
* @brief      Data Input/Output factory class
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


#ifdef DIO_ACTIVE


/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include <stdio.h>

#include "DIOStreamEnumDevices.h"
#include "DIOStream.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#if (defined(DIO_STREAMUART_ACTIVE) || defined(DIO_STREAMUSB_ACTIVE) || defined(DIO_STREAMICMP_ACTIVE) || defined(DIO_STREAMUDP_ACTIVE) || defined(DIO_STREAMTCPIP_ACTIVE)  || defined(DIO_STREAMBLUETOOTH_ACTIVE) || defined(DIO_STREAMBLUETOOTHLE_ACTIVE) || defined(DIO_STREAMSPI_ACTIVE) || defined(DIO_STREAMI2C_ACTIVE))
#define ANYTYPEOFDIOSTREAMIO
#endif

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOWIFIMANAGERMODE;
class DIOPCAP;
class DIONOTIFICATIONSMANAGER;
#ifdef DIO_LEDNEOPIXELWS2812B_ACTIVE 
class DIOLEDNEOPIXELWS2812B;
#endif

class DIOFACTORY
{
  public:

                                          DIOFACTORY                  ();
    virtual                              ~DIOFACTORY                  ();

    static bool                           GetIsInstanced              ();
    static DIOFACTORY&                    GetInstance                 ();
    static bool                           SetInstance                 (DIOFACTORY* instance);
    static bool                           DelInstance                 ();

    #ifdef ANYTYPEOFDIOSTREAMIO
    virtual DIOSTREAMENUMDEVICES*         CreateStreamEnumDevices     (DIOSTREAMENUMTYPE type);
    virtual bool                          DeleteStreamEnumDevices     (DIOSTREAMENUMDEVICES* enumdevices);

    virtual DIOSTREAM*                    CreateStreamIO              (DIOSTREAMCONFIG* config);
    virtual bool                          DeleteStreamIO              (DIOSTREAM* streamio);

    #endif

    #ifdef DIO_WIFIMANAGERMODE_ACTIVE
    virtual DIOWIFIMANAGERMODE*           CreateWifiManagerMode       ();
    virtual bool                          DeleteWifiManagerMode       (DIOWIFIMANAGERMODE* wifimanagermode);
    #endif

    #ifdef DIO_PCAP_ACTIVE
    virtual DIOPCAP*                      CreatePCap                  ();
    virtual bool                          DeletePCap                  (DIOPCAP* pcap);
    #endif
  
    virtual DIONOTIFICATIONSMANAGER*      CreateNotificationsManager  ();
    virtual bool                          DeleteNotificationsManager  (DIONOTIFICATIONSMANAGER* notificationsmanager);

    #ifdef DIO_LEDNEOPIXELWS2812B_ACTIVE    
    virtual DIOLEDNEOPIXELWS2812B*        CreateLedNeopixelWS2812B    ();
    virtual bool                          DeleteLedNeopixelWS2812B    (DIOLEDNEOPIXELWS2812B* ledneopixelws2812b);
    #endif
    
  private:

    void                                  Clean                       ();

    static DIOFACTORY*                    instance;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




#endif



