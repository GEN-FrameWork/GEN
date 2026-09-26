/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOStreamDeviceIP.h
* 
* @class      DIOSTREAMDEVICEIP
* @brief      Data Input/Output Stream Device IP class
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

#include "DIOIP.h"
#include "DIOMAC.h"

#include "DIOStreamDevice.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum DIOSTREAMIPDEVICE_TYPE
{
  DIOSTREAMIPDEVICE_TYPE_UNKNOWN            = 0 ,
  DIOSTREAMIPDEVICE_TYPE_ETHERNET               ,
  DIOSTREAMIPDEVICE_TYPE_WIFI                   ,
  DIOSTREAMIPDEVICE_TYPE_PPP                    ,
  DIOSTREAMIPDEVICE_TYPE_WWAN                   ,
  DIOSTREAMIPDEVICE_TYPE_LOOPBACK               ,
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOSTREAMDEVICEIP : public DIOSTREAMDEVICE
{
  public:

                                  DIOSTREAMDEVICEIP       ();
    virtual                      ~DIOSTREAMDEVICEIP       ();

    bool                          IsActive                ();  
    void                          SetIsActive             (bool isactive);      

    DIOMAC*                       GetMAC                  ();
    DIOIP*                        GetIP                   ();

    DIOSTREAMIPDEVICE_TYPE        GetIPType               ();
    bool                          SetIPType               (DIOSTREAMIPDEVICE_TYPE iptype);

    XVECTOR<XSTRING*>*            GetDNSservers           ();

    virtual bool                  DebugPrintInfo          ();

  protected:

    bool                          isactive;  
    DIOMAC                        MAC;
    DIOIP                         IP;
    DIOSTREAMIPDEVICE_TYPE        iptype;
    XVECTOR<XSTRING*>             DNSservers;

  private:

    void                          Clean                   ();
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





