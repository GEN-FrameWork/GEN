/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOI2CMonitorGaugeLTC2942.h
* 
* @class      DIOI2CMONITORGAUGELTC2942
* @brief      Data Input/Output I2C Monitor Gauge LTC2942 class
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

#include "DIODeviceI2C.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


// LTC2942 ADC mode enumeration 
enum DIOI2CMONITORGAUGELTC2942_ADC
{
  DIOI2CMONITORGAUGELTC2942_ADC_AUTO                        = 0xC0  , // Automatic mode
  DIOI2CMONITORGAUGELTC2942_ADC_M_VOL                       = 0x80  , // Manual voltage mode
  DIOI2CMONITORGAUGELTC2942_ADC_M_TEMP                      = 0x40  , // Manual temperature mode
  DIOI2CMONITORGAUGELTC2942_ADC_SLEEP                       = 0x00    // Sleep
};

// LTC2942 prescaler M enumeration 
enum DIOI2CMONITORGAUGELTC2942_PSCM
{
  DIOI2CMONITORGAUGELTC2942_PSCM_1                          = 0x00  ,
  DIOI2CMONITORGAUGELTC2942_PSCM_2                          = 0x08  ,
  DIOI2CMONITORGAUGELTC2942_PSCM_4                          = 0x10  ,
  DIOI2CMONITORGAUGELTC2942_PSCM_8                          = 0x18  ,
  DIOI2CMONITORGAUGELTC2942_PSCM_16                         = 0x20  ,
  DIOI2CMONITORGAUGELTC2942_PSCM_32                         = 0x28  ,
  DIOI2CMONITORGAUGELTC2942_PSCM_64                         = 0x30  ,
  DIOI2CMONITORGAUGELTC2942_PSCM_128                        = 0x38
};



// LTC2942 AL/CC pin mode enumeration 
enum DIOI2CMONITORGAUGELTC2942_ALLCC
{
  DIOI2CMONITORGAUGELTC2942_ALCC_DISABLED                   = 0x00  , // AL/CC pin disabled
  DIOI2CMONITORGAUGELTC2942_ALCC_CHG                        = 0x02  , // Charge complete mode
  DIOI2CMONITORGAUGELTC2942_ALCC_ALERT                      = 0x04    // Alert mode
};


// State of analog section enumeration 
enum DIOI2CMONITORGAUGELTC2942_AN
{
  DIOI2CMONITORGAUGELTC2942_AN_DISABLED                     = 0x00,
  DIOI2CMONITORGAUGELTC2942_AN_ENABLED                      = !DIOI2CMONITORGAUGELTC2942_AN_DISABLED
};


// LTC2942 I2C address        
#define DIOI2CMONITORGAUGELTC2942_ADDR 								        0x64    
#define DIOI2CMONITORGAUGELTC2942_HIGHSPEED_400KHZ						0
#define DIOI2CMONITORGAUGELTC2942_LOWSPEED_100KHZ						  1

#define DIOI2CMONITORGAUGELTC2942_AC_MAXVALUE                 39530

// LTC2942 register definitions 
#define DIOI2CMONITORGAUGELTC2942_REG_STATUS                  0x00  // (A) Status
#define DIOI2CMONITORGAUGELTC2942_REG_CONTROL                 0x01  // (B) Control
#define DIOI2CMONITORGAUGELTC2942_REG_AC_H                    0x02  // (C) Accumulated charge MSB
#define DIOI2CMONITORGAUGELTC2942_REG_AC_L                    0x03  // (D) Accumulated charge LSB
#define DIOI2CMONITORGAUGELTC2942_REG_CTH_H                   0x04  // (E) Charge threshold high MSB
#define DIOI2CMONITORGAUGELTC2942_REG_CTH_L                   0x05  // (F) Charge threshold high LSB
#define DIOI2CMONITORGAUGELTC2942_REG_CTL_H                   0x06  // (G) Charge threshold low MSB
#define DIOI2CMONITORGAUGELTC2942_REG_CTL_L                   0x07  // (H) Charge threshold low LSB
#define DIOI2CMONITORGAUGELTC2942_REG_VOL_H                   0x08  // (I) Voltage MSB
#define DIOI2CMONITORGAUGELTC2942_REG_VOL_L                   0x09  // (J) Voltage LSB
#define DIOI2CMONITORGAUGELTC2942_REG_VOLT_H                  0x0A  // (K) Voltage threshold high
#define DIOI2CMONITORGAUGELTC2942_REG_VOLT_L                  0x0B  // (L) Voltage threshold low
#define DIOI2CMONITORGAUGELTC2942_REG_TEMP_H                  0x0C  // (M) Temperature MSB
#define DIOI2CMONITORGAUGELTC2942_REG_TEMP_L                  0x0D  // (N) Temperature LSB
#define DIOI2CMONITORGAUGELTC2942_REG_TEMPT_H                 0x0E  // (O) Temperature threshold high
#define DIOI2CMONITORGAUGELTC2942_REG_TEMPT_L                 0x0F  // (P) Temperature threshold low

// LTC2942 status register bit definitions 
#define DIOI2CMONITORGAUGELTC2942_STATUS_CHIPID               0x80  // A[7] Chip identification (0: LTC2942-1, 0: LTC2941-1)
#define DIOI2CMONITORGAUGELTC2942_STATUS_AC_OVR               0x20  // A[5] Accumulated charge overflow/underflow
#define DIOI2CMONITORGAUGELTC2942_STATUS_TEMP_ALRT            0x10  // A[4] Temperature alert
#define DIOI2CMONITORGAUGELTC2942_STATUS_CHG_ALRT_H           0x08  // A[3] Charge alert high
#define DIOI2CMONITORGAUGELTC2942_STATUS_CHG_ALRT_L           0x04  // A[2] Charge alert low
#define DIOI2CMONITORGAUGELTC2942_STATUS_VOL_ALRT             0x02  // A[1] Voltage alert
#define DIOI2CMONITORGAUGELTC2942_STATUS_UVLO_ALRT            0x01  // A[0] Undervoltage lockout alert

// LTC2942 control register bit definitions */
#define DIOI2CMONITORGAUGELTC2942_CTL_ADC_MSK                 0x3F // ADC mode bits [7:6]
#define DIOI2CMONITORGAUGELTC2942_CTL_PSCM_MSK                0xC7 // Prescaler M bits [5:3]
#define DIOI2CMONITORGAUGELTC2942_CTL_ALCC_MSK                0xF9 // AL/CC pin control [2:1]
#define DIOI2CMONITORGAUGELTC2942_CTL_SHUTDOWN                0x01 // B[0] Shutdown
#define DIOI2CMONITORGAUGELTC2942_CTL_POWERON                 0xFE // B[0] POWER on

#define DIOI2CMONITORGAUGELTC2942_VOLTAGE_READ_ERROR      		0X8000

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOI2CMONITORGAUGELTC2942: public DIODEVICEI2C
{
  public:

                              DIOI2CMONITORGAUGELTC2942     ();
    virtual                  ~DIOI2CMONITORGAUGELTC2942     ();

    float                     GetVoltage                    ();
    
    bool                      SetAC                         (XWORD ac);
    XWORD                     GetAC                         ();
    XWORD                     GetMaxAC                      ();
    
    bool                      End                           ();

  private:

    bool                      IniDevice                     ();

    bool                      Initialize                    ();
    
    bool                      ReadRegister                  (XBYTE offset, XBYTE& data);
    bool                      ReadRegister                  (XBYTE offset, XBYTE* data, XDWORD sizedata);

    bool                      WriteRegister                 (XBYTE offset, XBYTE data);
    bool                      WriteRegister                 (XBYTE offset, XBYTE* data, XDWORD sizedata);



    void                      Clean                         ();

    float                     referenceresistance;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





