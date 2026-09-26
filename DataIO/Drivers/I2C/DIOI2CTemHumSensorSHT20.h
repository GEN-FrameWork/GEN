/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOI2CTemHumSensorSHT20.h
* 
* @class      DIOI2CTEMHUMSENSORSHT20
* @brief      Data Input/Output I2C Sensor Sensirion SHT20 (Temperature and humidity module) class
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

#include "DIODeviceI2C.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOI2CTEMHUMSENSORSHT20_ADDRESS                               0x40

#define DIOI2CTEMHUMSENSORSHT20_CMD_SOFT_RESET                        0xFE

#define DIOI2CTEMHUMSENSORSHT20_CMD_TRIGGER_TEMP_MEASURE_HOLD         0xE3
#define DIOI2CTEMHUMSENSORSHT20_CMD_TRIGGER_HUMD_MEASURE_HOLD         0xE5
#define DIOI2CTEMHUMSENSORSHT20_CMD_TRIGGER_TEMP_MEASURE_NOHOLD       0xF3
#define DIOI2CTEMHUMSENSORSHT20_CMD_TRIGGER_HUMD_MEASURE_NOHOLD       0xF5

#define DIOI2CTEMHUMSENSORSHT20_CMD_WRITE_USER_REG                    0xE6
#define DIOI2CTEMHUMSENSORSHT20_CMD_READ_USER_REG                     0xE7

#define DIOI2CTEMHUMSENSORSHT20_SHIFTED_DIVISOR                       0x988000

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOI2CTEMHUMSENSORSHT20: public DIODEVICEI2C
{
  public:

                              DIOI2CTEMHUMSENSORSHT20       ();
    virtual                  ~DIOI2CTEMHUMSENSORSHT20       ();

    bool                      Reset                         ();

    bool                      SetResolution                 (XBYTE resolution);

    bool                      Read                          (float& temperature, float& humidity);

    bool                      End                           ();

  private:

    bool                      IniDevice                     ();

    bool                      ReadValue                     (XBYTE cmd, XWORD& value);

    bool                      ReadUserRegister              (XBYTE& userregister);
    bool                      WriteUserRegister             (XBYTE userregister);

    bool                      CheckCRC                      (XWORD datasensor, XBYTE checkvalue);

    void                      Clean                         ();
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




