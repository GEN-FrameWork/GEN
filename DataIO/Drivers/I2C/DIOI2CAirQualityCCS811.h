/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOI2CAirQualityCCS811.h
* 
* @class      DIOI2CAIRQUALITYCCS811
* @brief      Data Input/Output I2C Sensor CCS811 (Air Quality) class 
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


enum  DIOI2CAIRQUALITYCCS811_ERRORS
{
  DIOI2CAIRQUALITYCCS811_ERROR_WRITE_REG_INVALID      = 0 , // The CCS811 received an IC write request addressed to this station but with invalid register address ID
  DIOI2CAIRQUALITYCCS811_ERROR_READ_REG_INVALID           , // The CCS811 received an IC read request to a mailbox ID that is invalid
  DIOI2CAIRQUALITYCCS811_ERROR_MEASMODE_INVALID           , // The CCS811 received an IC request to write an unsupported mode to MEAS_MODE
  DIOI2CAIRQUALITYCCS811_ERROR_MAX_RESISTANCE             , // The sensor resistance measurement has reached or exceeded the maximum range
  DIOI2CAIRQUALITYCCS811_ERROR_HEATER_FAULT               , // The Heater current in the CCS811 is not in range
  DIOI2CAIRQUALITYCCS811_ERROR_HEATER_SUPPLY              , // The Heater voltage is not being applied correctly
  DIOI2CAIRQUALITYCCS811_ERROR_RESERVED1                  , // Reserved for Future Use
  DIOI2CAIRQUALITYCCS811_ERROR_RESERVED2                    // Reserved for Future Use
};

#define DIOI2CAIRQUALITYCCS811_ADDR1                    0x5A
#define DIOI2CAIRQUALITYCCS811_ADDR2                    0x5B

#define DIOI2CAIRQUALITYCCS811_OFFSET_STATUS            0x00
#define DIOI2CAIRQUALITYCCS811_OFFSET_MEAS_MODE         0x01
#define DIOI2CAIRQUALITYCCS811_OFFSET_ALG_RESULT_DATA   0x02
#define DIOI2CAIRQUALITYCCS811_OFFSET_RAW_DATA          0x03
#define DIOI2CAIRQUALITYCCS811_OFFSET_ENV_DATA          0x05
#define DIOI2CAIRQUALITYCCS811_OFFSET_NTC               0x06
#define DIOI2CAIRQUALITYCCS811_OFFSET_THRESHOLDS        0x10
#define DIOI2CAIRQUALITYCCS811_OFFSET_BASELINE          0x11
#define DIOI2CAIRQUALITYCCS811_OFFSET_HW_ID             0x20
#define DIOI2CAIRQUALITYCCS811_OFFSET_HW_VERSION        0x21
#define DIOI2CAIRQUALITYCCS811_OFFSET_FW_BOOT_VERSION   0x23
#define DIOI2CAIRQUALITYCCS811_OFFSET_FW_APP_VERSION    0x24
#define DIOI2CAIRQUALITYCCS811_OFFSET_ERROR_ID          0xE0
#define DIOI2CAIRQUALITYCCS811_OFFSET_APP_START         0xF4
#define DIOI2CAIRQUALITYCCS811_OFFSET_SW_RESET          0xFF

#define DIOI2CAIRQUALITYCCS811_HW_ID                    0x81

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOI2CAIRQUALITYCCS811: public DIODEVICEI2C
{
  public:

                              DIOI2CAIRQUALITYCCS811        ();
    virtual                  ~DIOI2CAIRQUALITYCCS811        ();

    bool                      GetHW_ID                      (XBYTE& ID);

    bool                      GetHW_Version                 (XBYTE& hwversion);
    bool                      GetBoot_Version               (XWORD& bootversion);
    bool                      GetApp_Version                (XWORD& appversion);

    bool                      ResetDevice                   ();

    XBYTE                     GetStatus                     (XBYTE& sensorstatus);

    bool                      IsAppValid                    ();
    bool                      IsDataAvailable               ();
    bool                      HaveStatusError               ();

    XBYTE                     GetStatusErrorCode            ();

    XWORD                     GetBaseline                   ();
    bool                      SetBaseline                   (XWORD baseline);

    bool                      SetDriveMode                  (XBYTE mode);
    bool                      SetEnvironmentalData          (float relativeHumidity, float temperature);

    float                     GetReferenceResistance        ();
    void                      SetReferenceResistance        (float referenceresistance);

    bool                      ReadNTC                       (float& temperature, float& resistance);

    bool                      ReadData                      (XWORD& TVOC, XWORD& CO2);

    bool                      SetInterrupt                  (bool activated);

    bool                      End                           ();

  private:

    bool                      IniDevice                     ();

    bool                      ReadRegister                  (XBYTE offset, XBYTE& data);
    bool                      ReadRegister                  (XBYTE offset, XBYTE* data, XDWORD sizedata);

    bool                      WriteRegister                 (XBYTE offset, XBYTE data);
    bool                      WriteRegister                 (XBYTE offset, XBYTE* data, XDWORD sizedata);

    void                      Clean                         ();

    float                     referenceresistance;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





