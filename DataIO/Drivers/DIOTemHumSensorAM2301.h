/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOTemHumSensorAM2301.h
* 
* @class      DIOTEMHUMSENSORAM2301
* @brief      Data Input/Output AOSONG AM2301 Sensor (Temperature and humidity module) class
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

#include "XBase.h"
#include "XThreadCollected.h"

#include "DIODevice.h"
#include "DIOGPIO.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOTEMHUMSENSORAM2301_HIGH    true
#define DIOTEMHUMSENSORAM2301_LOW     false

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XTIMER;
class XMUTEX;

class DIOTEMHUMSENSORAM2301 : public DIODEVICE
{
  public:
                                  DIOTEMHUMSENSORAM2301       (int pindata, bool activatecache);
    virtual                      ~DIOTEMHUMSENSORAM2301       ();

    bool                          Ini                         (int cadenceread = 30);

    int                           GetPinData                  ();

    bool                          ReadFromCache               (float& temperature, float& humidity);
    bool                          ReadDirect                  (float& temperature, float& humidity);

    bool                          End                         ();

  private:    

    bool                          WaitTo                      (bool tohigh, int timeout, int* timeelapsed = NULL);

    static void                   ThreadRunFunction           (void* param);

    void                          Clean                       ();
   
    int                           pindata;

    int                           cadenceread;

    XTIMER*                       xtimer;
    XTHREADCOLLECTED*             threadcache;
    XMUTEX*                       xmutexread;

    XDWORD                        nreads;
    float                         temperaturecache;
    float                         humiditycache;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





