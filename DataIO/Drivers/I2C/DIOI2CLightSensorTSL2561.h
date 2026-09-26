/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOI2CLightSensorTSL2561.h
* 
* @class      DIOI2CLIGHTSENSORTSL2561
* @brief      Data Input/Output I2C Sensor Light TSL2561 (Luminosity Sensor) class 
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

#include <stdio.h>

#include "XBase.h"
#include "XString.h"

#include "DIODeviceI2C.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOI2CLIGHTSENSORTSL2561_ADDR1                0x29            // Address 1
#define DIOI2CLIGHTSENSORTSL2561_ADDR2                0x39            // Address 2
#define DIOI2CLIGHTSENSORTSL2561_ADDR3                0x49            // Address 3


#define DIOI2CLIGHTSENSORTSL2561_VISIBLE              2               // channel 0 - channel 1
#define DIOI2CLIGHTSENSORTSL2561_INFRARED             1               // channel 1
#define DIOI2CLIGHTSENSORTSL2561_FULLSPECTRUM         0               // channel 0


#define DIOI2CLIGHTSENSORTSL2561_COMMAND_BIT          0x80            // Must be 1
#define DIOI2CLIGHTSENSORTSL2561_CLEAR_BIT            0x40            // Clears any pending interrupt (write 1 to clear)
#define DIOI2CLIGHTSENSORTSL2561_WORD_BIT             0x20            // 1 = read/write word (rather than byte)
#define DIOI2CLIGHTSENSORTSL2561_BLOCK_BIT            0x10            // 1 = using block read/write

#define DIOI2CLIGHTSENSORTSL2561_CONTROL_POWERON      0x03
#define DIOI2CLIGHTSENSORTSL2561_CONTROL_POWEROFF     0x00

#define DIOI2CLIGHTSENSORTSL2561_LUX_LUXSCALE         14              // Scale by 2^14
#define DIOI2CLIGHTSENSORTSL2561_LUX_RATIOSCALE       9               // Scale ratio by 2^9
#define DIOI2CLIGHTSENSORTSL2561_LUX_CHSCALE          10              // Scale channel values by 2^10
#define DIOI2CLIGHTSENSORTSL2561_LUX_CHSCALE_TINT0    0x7517          // 322/11   * 2^TSL2561_LUX_CHSCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_CHSCALE_TINT1    0x0FE7          // 322/81   * 2^TSL2561_LUX_CHSCALE

// T, FN and CL package values
#define DIOI2CLIGHTSENSORTSL2561_LUX_K1T              0x0040          // 0.125    * 2^RATIO_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_B1T              0x01f2          // 0.0304   * 2^LUX_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_M1T              0x01be          // 0.0272   * 2^LUX_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_K2T              0x0080          // 0.250    * 2^RATIO_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_B2T              0x0214          // 0.0325   * 2^LUX_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_M2T              0x02d1          // 0.0440   * 2^LUX_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_K3T              0x00c0          // 0.375    * 2^RATIO_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_B3T              0x023f          // 0.0351   * 2^LUX_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_M3T              0x037b          // 0.0544   * 2^LUX_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_K4T              0x0100          // 0.50     * 2^RATIO_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_B4T              0x0270          // 0.0381   * 2^LUX_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_M4T              0x03fe          // 0.0624   * 2^LUX_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_K5T              0x0138          // 0.61     * 2^RATIO_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_B5T              0x016f          // 0.0224   * 2^LUX_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_M5T              0x01fc          // 0.0310   * 2^LUX_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_K6T              0x019a          // 0.80     * 2^RATIO_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_B6T              0x00d2          // 0.0128   * 2^LUX_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_M6T              0x00fb          // 0.0153   * 2^LUX_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_K7T              0x029a          // 1.3      * 2^RATIO_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_B7T              0x0018          // 0.00146  * 2^LUX_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_M7T              0x0012          // 0.00112  * 2^LUX_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_K8T              0x029a          // 1.3      * 2^RATIO_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_B8T              0x0000          // 0.000    * 2^LUX_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_M8T              0x0000          // 0.000    * 2^LUX_SCALE

// CS package values
#define DIOI2CLIGHTSENSORTSL2561_LUX_K1C              0x0043          // 0.130    * 2^RATIO_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_B1C              0x0204          // 0.0315   * 2^LUX_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_M1C              0x01ad          // 0.0262   * 2^LUX_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_K2C              0x0085          // 0.260    * 2^RATIO_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_B2C              0x0228          // 0.0337   * 2^LUX_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_M2C              0x02c1          // 0.0430   * 2^LUX_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_K3C              0x00c8          // 0.390    * 2^RATIO_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_B3C              0x0253          // 0.0363   * 2^LUX_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_M3C              0x0363          // 0.0529   * 2^LUX_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_K4C              0x010a          // 0.520    * 2^RATIO_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_B4C              0x0282          // 0.0392   * 2^LUX_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_M4C              0x03df          // 0.0605   * 2^LUX_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_K5C              0x014d          // 0.65     * 2^RATIO_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_B5C              0x0177          // 0.0229   * 2^LUX_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_M5C              0x01dd          // 0.0291   * 2^LUX_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_K6C              0x019a          // 0.80     * 2^RATIO_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_B6C              0x0101          // 0.0157   * 2^LUX_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_M6C              0x0127          // 0.0180   * 2^LUX_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_K7C              0x029a          // 1.3      * 2^RATIO_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_B7C              0x0037          // 0.00338  * 2^LUX_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_M7C              0x002b          // 0.00260  * 2^LUX_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_K8C              0x029a          // 1.3      * 2^RATIO_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_B8C              0x0000          // 0.000    * 2^LUX_SCALE
#define DIOI2CLIGHTSENSORTSL2561_LUX_M8C              0x0000          // 0.000    * 2^LUX_SCALE

// Auto-gain thresholds
#define DIOI2CLIGHTSENSORTSL2561_AGC_THI_13MS         4850            // Max value at Ti 13ms = 5047
#define DIOI2CLIGHTSENSORTSL2561_AGC_TLO_13MS         100
#define DIOI2CLIGHTSENSORTSL2561_AGC_THI_101MS        36000           // Max value at Ti 101ms = 37177
#define DIOI2CLIGHTSENSORTSL2561_AGC_TLO_101MS        200
#define DIOI2CLIGHTSENSORTSL2561_AGC_THI_402MS        63000           // Max value at Ti 402ms = 65535
#define DIOI2CLIGHTSENSORTSL2561_AGC_TLO_402MS        500

// Clipping thresholds
#define DIOI2CLIGHTSENSORTSL2561_CLIPPING_13MS        4900
#define DIOI2CLIGHTSENSORTSL2561_CLIPPING_101MS       37000
#define DIOI2CLIGHTSENSORTSL2561_CLIPPING_402MS       65000


enum DIOI2CLIGHTSENSORTSL2561REGISTER
{
  DIOI2CLIGHTSENSORTSL2561REGISTER_CONTROL          = 0x00,
  DIOI2CLIGHTSENSORTSL2561REGISTER_TIMING           = 0x01,
  DIOI2CLIGHTSENSORTSL2561REGISTER_THRESHHOLDL_LOW  = 0x02,
  DIOI2CLIGHTSENSORTSL2561REGISTER_THRESHHOLDL_HIGH = 0x03,
  DIOI2CLIGHTSENSORTSL2561REGISTER_THRESHHOLDH_LOW  = 0x04,
  DIOI2CLIGHTSENSORTSL2561REGISTER_THRESHHOLDH_HIGH = 0x05,
  DIOI2CLIGHTSENSORTSL2561REGISTER_INTERRUPT        = 0x06,
  DIOI2CLIGHTSENSORTSL2561REGISTER_CRC              = 0x08,
  DIOI2CLIGHTSENSORTSL2561REGISTER_ID               = 0x0A,
  DIOI2CLIGHTSENSORTSL2561REGISTER_CHAN0_LOW        = 0x0C,
  DIOI2CLIGHTSENSORTSL2561REGISTER_CHAN0_HIGH       = 0x0D,
  DIOI2CLIGHTSENSORTSL2561REGISTER_CHAN1_LOW        = 0x0E,
  DIOI2CLIGHTSENSORTSL2561REGISTER_CHAN1_HIGH       = 0x0F
};

enum DIOI2CLIGHTSENSORTSL2561INTEGRATIONTIME
{
  DIOI2CLIGHTSENSORTSL2561INTEGRATIONTIME_13MS      = 0x00,    // 13.7ms
  DIOI2CLIGHTSENSORTSL2561INTEGRATIONTIME_101MS     = 0x01,    // 101ms
  DIOI2CLIGHTSENSORTSL2561INTEGRATIONTIME_402MS     = 0x02     // 402ms
};

enum DIOI2CLIGHTSENSORTSL2561GAIN
{
  DIOI2CLIGHTSENSORTSL2561GAIN_1X                   = 0x00,    // No gain
  DIOI2CLIGHTSENSORTSL2561GAIN_16X                  = 0x10,    // 16x gain
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOI2CLIGHTSENSORTSL2561 : public DIODEVICEI2C
{
  public:

                                                DIOI2CLIGHTSENSORTSL2561      ();
    virtual                                    ~DIOI2CLIGHTSENSORTSL2561      ();

    bool                                        SetIntegrationTime            (DIOI2CLIGHTSENSORTSL2561INTEGRATIONTIME integrationtime);
    bool                                        SetGain                       (DIOI2CLIGHTSENSORTSL2561GAIN gain);

    bool                                        Get                           (XWORD& fullspectrum, XWORD& infrared, XDWORD& lux, bool autogain);

    bool                                        End                           ();

  protected:

    DIOI2CLIGHTSENSORTSL2561INTEGRATIONTIME     integrationtime;
    DIOI2CLIGHTSENSORTSL2561GAIN                gain;

  private:

    bool                                        IniDevice                     ();

    bool                                        WriteReg                      (XBYTE reg, XBYTE value, int timeout);
    bool                                        ReadData16                    (XBYTE reg, XWORD& value, int timeout);
    bool                                        GetData                       (XWORD& fullspectrum, XWORD& infrared);

    bool                                        Activate                      ();
    bool                                        Deactivate                    ();

    XDWORD                                      CalculateLux                  (XWORD broadband, XWORD inflared);

    void                                        Clean                         ();
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/







