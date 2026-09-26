/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOANDROIDFactory.h
* 
* @class      DIOANDROIDFACTORY
* @brief      ANDROID Data Input/Output Factory class
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

#pragma once

#ifdef DIO_ACTIVE

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOFactory.h"
#include "DIOPing.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOANDROIDFACTORY : public DIOFACTORY
{
  public:

    #ifdef ANYTYPEOFDIOSTREAMIO
    DIOSTREAMENUMDEVICES*           CreateStreamEnumDevices         (DIOSTREAMENUMTYPE type);
    bool                            DeleteStreamEnumDevices         (DIOSTREAMENUMDEVICES* enumdevices);

    DIOSTREAM*                      CreateStreamIO                  (DIOSTREAMCONFIG* config);
    bool                            DeleteStreamIO                  (DIOSTREAM* diostream);
    #endif

    #ifdef DIO_PING_ACTIVE
    DIOPING*                        CreatePing                      ();
    bool                            DeletePing                      (DIOPING* ping);
    #endif

    #ifdef DIO_PCAP_ACTIVE
    virtual DIOPCAP*                CreatePCap                      ();
    virtual bool                    DeletePCap                      (DIOPCAP* pcap);
    #endif

    #ifdef DIO_GPIO_ACTIVE
    DIOGPIO*                        CreateGPIO                      ();
    bool                            DeleteGPIO                      (DIOGPIO* gpio);
    #endif
};


/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/


#endif



