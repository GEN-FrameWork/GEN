/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOStreamEnumDevices.h
* 
* @class      DIOSTREAMENUMDEVICES
* @brief      Data Input/Output Stream Enum Devices class
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

#include "XVector.h"
#include "XString.h"
#include "XSubject.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


enum DIOSTREAMENUMTYPE
{
  DIOSTREAMENUMTYPE_UNKNOWN               = 0 ,
  DIOSTREAMENUMTYPE_UART_LOCAL                ,
  DIOSTREAMENUMTYPE_USB_LOCAL                 ,
  DIOSTREAMENUMTYPE_IP_LOCAL                  ,
  DIOSTREAMENUMTYPE_LOCAL_SERVERS             ,
  DIOSTREAMENUMTYPE_TCPIP_LOCAL_SERVERS       ,
  DIOSTREAMENUMTYPE_BLUETOOTH_LOCAL           ,
  DIOSTREAMENUMTYPE_BLUETOOTH_REMOTE          ,
  DIOSTREAMENUMTYPE_BLUETOOTHLE_REMOTE        ,
  DIOSTREAMENUMTYPE_WIFI_REMOTE               ,
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XTIMER;
class DIOSTREAMDEVICE;
class DIOMAC;

class DIOSTREAMENUMDEVICES : public XSUBJECT
{
  public:
                                    DIOSTREAMENUMDEVICES            ();
    virtual                        ~DIOSTREAMENUMDEVICES            ();

    DIOSTREAMENUMTYPE               GetType                         ();
    bool                            SetType                         (DIOSTREAMENUMTYPE type);

    virtual bool                    Search                          ();
    virtual bool                    StopSearch                      (bool waitend);
    virtual bool                    IsSearching                     ();

    bool                            AddDevice                       (DIOSTREAMDEVICE* device);
    XVECTOR<DIOSTREAMDEVICE*>*      GetDevices                      ();
    bool                            DelAllDevices                   ();

    virtual bool                    IsDeviceAvailable               (XCHAR* string);
    bool                            IsDeviceAvailable               (XSTRING& string);
    bool                            AreDevicesAvailable             ();

    DIOSTREAMDEVICE*                GetDeviceByName                 (XCHAR* name);
    DIOSTREAMDEVICE*                GetDeviceByName                 (XSTRING& name);

    DIOSTREAMDEVICE*                GetDeviceByIndex                (int index);

  protected:

    XTIMER*                         xtimerout;

    DIOSTREAMENUMTYPE               type;

    XVECTOR<DIOSTREAMDEVICE*>       devices;

  private:

    void                            Clean                           ();
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




