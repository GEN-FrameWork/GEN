/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOAIOBIMainBoard.h
* 
* @class      DIOAIOBIMAINBOARD
* @brief      Data Input/Output AIOBI Main board class
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

#include "XLog.h"
#include "XTrace.h"

#include "DIOFactory.h"
#include "DIOStreamI2C.h"
#include "DIOI2CGPIOPCF8574.h"
#include "DIOI2CEEprom24XXX.h"
#include "DIOI2CPWMControlerPCA9685.h"
#include "DIOGPIO.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOAIOBIMAINBOARD_I2CDEVICENAME           __L("/dev/i2c-1")

#define DIOAIOBIMAINBOARD_I2CDIGIGPIO_ID_IN       0x3C
#define DIOAIOBIMAINBOARD_I2CDIGIGPIO_ID_OUT      0x3D
#define DIOAIOBIMAINBOARD_I2CDIGIGPIO_ID_RELAYS   0x39
#define DIOAIOBIMAINBOARD_I2CEEPROM               0x50
#define DIOAIOBIMAINBOARD_I2CPWM                  0x40


enum DIOAIOBIMAINBOARD_HEADER
{
  DIOAIOBIMAINBOARD_HEADER_UNKNOW               = 0 ,
  DIOAIOBIMAINBOARD_HEADER_P1                       ,
  DIOAIOBIMAINBOARD_HEADER_P24                      ,
  DIOAIOBIMAINBOARD_HEADER_SLOT                     ,
};


enum DIOAIOBIMAINBOARD_I2CDIGIGPIO
{
  DIOAIOBIMAINBOARD_I2CDIGIGPIO_IN              = 0 ,
  DIOAIOBIMAINBOARD_I2CDIGIGPIO_OUT                 ,
  DIOAIOBIMAINBOARD_I2CDIGIGPIO_RELAYS              ,
};


#define DIOAIOBIMAINBOARD_P1_PIN1                 37     // GPIO 26 Raspberry Pi.
#define DIOAIOBIMAINBOARD_P1_PIN2                 35     // GPIO 19 Raspberry Pi.
#define DIOAIOBIMAINBOARD_P1_PIN3                 33     // GPIO 13 Raspberry Pi.
#define DIOAIOBIMAINBOARD_P1_PIN4                 32     // GPIO 12 Raspberry Pi.
#define DIOAIOBIMAINBOARD_P1_PIN5                 22     // GPIO 25 Raspberry Pi.
#define DIOAIOBIMAINBOARD_P1_PIN6                 18     // GPIO 24 Raspberry Pi.
#define DIOAIOBIMAINBOARD_P1_PIN7                        // 5v
#define DIOAIOBIMAINBOARD_P1_PIN8                        // GND

#define DIOAIOBIMAINBOARD_P24_PIN1                       // 3v3
#define DIOAIOBIMAINBOARD_P24_PIN2                15     // GPIO 22 Raspberry Pi.     Extern button Button A
#define DIOAIOBIMAINBOARD_P24_PIN3                12     // GPIO 18 Raspberry Pi.     Extern button Button B
#define DIOAIOBIMAINBOARD_P24_PIN4                13     // GPIO 27 Raspberry Pi.     Extern button Led    A
#define DIOAIOBIMAINBOARD_P24_PIN5                11     // GPIO 11 Raspberry Pi.     Extern button Led    B
#define DIOAIOBIMAINBOARD_P24_PIN6                       // GND

#define DIOAIOBIMAINBOARD_SLOT_PIN9               38     // GPIO 20 Raspberry Pi.
#define DIOAIOBIMAINBOARD_SLOT_PIN10              29     // GPIO  5 Raspberry Pi.
#define DIOAIOBIMAINBOARD_SLOT_PIN11              31     // GPIO  6 Raspberry Pi.
#define DIOAIOBIMAINBOARD_SLOT_PIN21              40     // GPIO 21 Raspberry Pi.

#define DIOAIOBIMAINBOARD_DEFAULTTIMEOUT          10

#define DIOAIOBIMAINBOARD_LOGSECTIONID            __L("AIOBI MainBoard")

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOAIOBIMAINBOARD
{
  public:

                                DIOAIOBIMAINBOARD               ();
    virtual                    ~DIOAIOBIMAINBOARD               ();

    bool                        Ini                             ();

    bool                        IsIni                           ();

    bool                        SetModeHeaderPin                (DIOAIOBIMAINBOARD_HEADER header, int pin, bool isinput);

    bool                        GetStatusHeaderPin              (DIOAIOBIMAINBOARD_HEADER header, int pin);
    bool                        SetStatusHeaderPin              (DIOAIOBIMAINBOARD_HEADER header, int pin, bool active);

    bool                        SetP4HeaderForExternalButton    (bool on = true);

    bool                        IsButtonAPressed                ();
    bool                        IsButtonBPressed                ();

    bool                        SetLedA                         (bool on);
    bool                        SetLedB                         (bool on);

    bool                        GetDigitalInput                 (XBYTE&data);
    bool                        SetDigitalOutput                (XBYTE bit, bool on);

    bool                        SwitchReleK1                    (bool on);
    bool                        SwitchReleK2                    (bool on);
    bool                        SwitchReleK3                    (bool on);

    bool                        EEProm                          (bool write, XBUFFER& xbuffer);

    DIOGPIO*                    GetGPIO                         ();

    bool                        SetPWM                          (XBYTE channel, XWORD on, XWORD off, bool wait = true);


    bool                        ResetMicroControler             ();

    bool                        End                             ();


  protected:

    DIOI2CGPIOPCF8574*          pcf8574[3];
    DIOI2CEEPROM24XXX*          eeprom;
    DIOGPIO*                    gpio;
    DIOI2CPWMCONTROLERPCA9685*  pca9685;

  private:

    void                        Clean                           ();

    bool                        isini;
    bool                        issetp24forexternalbutton;
    XBYTE                       outputdigitaldata;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/







