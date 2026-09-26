/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOSPITouchScreenSTMPE610.h
* 
* @class      DIOSPITOUCHSCREENSTMPE610
* @brief      Data Input/Output SPI STMPE610 (Touch Screen) class
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

#include "DIODevice.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


#define DIOSPITOUCHSCREENSTMPE610_REG_CHIP_ID1            0x00  //  0x0811 Device identification
#define DIOSPITOUCHSCREENSTMPE610_REG_CHIP_ID2            0x01
#define DIOSPITOUCHSCREENSTMPE610_REG_ID_VER              0x02  //  Revision number (0x01 for engineering sample   0x03 for final silicon)
#define DIOSPITOUCHSCREENSTMPE610_REG_SYS_CTRL1           0x03  //  Reset control
#define DIOSPITOUCHSCREENSTMPE610_REG_SYS_CTRL2           0x04  //  Clock control
#define DIOSPITOUCHSCREENSTMPE610_REG_SPI_CFG             0x08  //  SPI interface configuration
#define DIOSPITOUCHSCREENSTMPE610_REG_INT_CTRL            0x09  //  Interrupt control register
#define DIOSPITOUCHSCREENSTMPE610_REG_INT_EN              0x0A  //  Interrupt enable register
#define DIOSPITOUCHSCREENSTMPE610_REG_INT_STA             0x0B  //  Interrupt status register
#define DIOSPITOUCHSCREENSTMPE610_REG_GPIO_EN             0x0C  //  GPIO interrupt enable register
#define DIOSPITOUCHSCREENSTMPE610_REG_GPIO_INT_STA        0x0D  //  GPIO interrupt status register
#define DIOSPITOUCHSCREENSTMPE610_REG_ADC_INT             0x0E  //  ADC interrupt enable register
#define DIOSPITOUCHSCREENSTMPE610_REG_ADC_INT_STA         0x0F  //  ADC interrupt status register
#define DIOSPITOUCHSCREENSTMPE610_REG_GPIO_SET_PIN        0x10  //  GPIO set pin register
#define DIOSPITOUCHSCREENSTMPE610_REG_GPIO_CLR_PIN        0x11  //  GPIO clear pin register
#define DIOSPITOUCHSCREENSTMPE610_REG_GPIO_MP_STA         0x12  //  GPIO monitor pin state register
#define DIOSPITOUCHSCREENSTMPE610_REG_GPIO_DIR            0x13  //  GPIO direction register
#define DIOSPITOUCHSCREENSTMPE610_REG_GPIO_ED             0x14  //  GPIO edge detect register
#define DIOSPITOUCHSCREENSTMPE610_REG_GPIO_RE             0x15  //  GPIO rising edge register
#define DIOSPITOUCHSCREENSTMPE610_REG_GPIO_FE             0x16  //  GPIO falling edge register
#define DIOSPITOUCHSCREENSTMPE610_REG_GPIO_AF             0x17  //  Alternate function register
#define DIOSPITOUCHSCREENSTMPE610_REG_ADC_CTRL1           0x20  //  ADC control
#define DIOSPITOUCHSCREENSTMPE610_REG_ADC_CTRL2           0x21  //  ADC control
#define DIOSPITOUCHSCREENSTMPE610_REG_ADC_CAP             0x22  //  To initiate ADC data acquisition
#define DIOSPITOUCHSCREENSTMPE610_REG_ADC_DATA_CH0        0x30  //  ADC channel 0
#define DIOSPITOUCHSCREENSTMPE610_REG_ADC_DATA_CH1        0x32  //  ADC channel 1
#define DIOSPITOUCHSCREENSTMPE610_REG_ADC_DATA_CH4        0x38  //  ADC channel 4
#define DIOSPITOUCHSCREENSTMPE610_REG_ADC_DATA_CH5        0x3A  //  ADC channel 5
#define DIOSPITOUCHSCREENSTMPE610_REG_ADC_DATA_CH6        0x3C  //  ADC channel 6
#define DIOSPITOUCHSCREENSTMPE610_REG_ADC_DATA_CH7        0x3E  //  ADC channel 7
#define DIOSPITOUCHSCREENSTMPE610_REG_TSC_CTRL            0x40  //  4-wire touch display controller setup
#define DIOSPITOUCHSCREENSTMPE610_REG_TSC_CFG             0x41  //  Touch display controller configuration
#define DIOSPITOUCHSCREENSTMPE610_REG_WDW_TR_X            0x42  //  Window setup for top right X
#define DIOSPITOUCHSCREENSTMPE610_REG_WDW_TR_Y            0x44  //  Window setup for top right Y
#define DIOSPITOUCHSCREENSTMPE610_REG_WDW_BL_X            0x46  //  Window setup for bottom left X
#define DIOSPITOUCHSCREENSTMPE610_REG_WDW_BL_Y            0x48  //  Window setup for bottom left Y
#define DIOSPITOUCHSCREENSTMPE610_REG_FIFO_TH             0x4A  //  FIFO level to generate interrupt
#define DIOSPITOUCHSCREENSTMPE610_REG_FIFO_STA            0x4B  //  Current status of FIFO
#define DIOSPITOUCHSCREENSTMPE610_REG_FIFO_SIZE           0x4C  //  Current filled level of FIFO
#define DIOSPITOUCHSCREENSTMPE610_REG_TSC_DATA_X          0x4D  //  Data port for touch display controller data access
#define DIOSPITOUCHSCREENSTMPE610_REG_TSC_DATA_Y          0x4F  //  Data port for touch display controller data access
#define DIOSPITOUCHSCREENSTMPE610_REG_TSC_DATA_Z          0x51  //  Data port for touch display controller data access
#define DIOSPITOUCHSCREENSTMPE610_REG_TSC_DATA_XYZ        0x52  //  Data port for touch display controller data access
#define DIOSPITOUCHSCREENSTMPE610_REG_TSC_FRACT_XYZ       0x56  //  Select the range and accuracy of the pressure measurement
#define DIOSPITOUCHSCREENSTMPE610_REG_TSC_DATA            0x57  //  Data port for touch display controller data access
#define DIOSPITOUCHSCREENSTMPE610_REG_TSC_I_DRIVE         0x58  //  Touch display controller drive I
#define DIOSPITOUCHSCREENSTMPE610_REG_TSC_SHIELD          0x59  //  Touch display controller shield


#define DIOSPITOUCHSCREENSTMPE610_SYS_CTRL1_RESET         0x02
#define DIOSPITOUCHSCREENSTMPE610_TSC_CTRL                0x40
#define DIOSPITOUCHSCREENSTMPE610_TSC_CTRL_EN             0x01
#define DIOSPITOUCHSCREENSTMPE610_TSC_CTRL_XYZ            0x00
#define DIOSPITOUCHSCREENSTMPE610_TSC_CTRL_XY             0x02

#define DIOSPITOUCHSCREENSTMPE610_INT_CTRL_POL_HIGH       0x04
#define DIOSPITOUCHSCREENSTMPE610_INT_CTRL_POL_LOW        0x00
#define DIOSPITOUCHSCREENSTMPE610_INT_CTRL_EDGE           0x02
#define DIOSPITOUCHSCREENSTMPE610_INT_CTRL_LEVEL          0x00
#define DIOSPITOUCHSCREENSTMPE610_INT_CTRL_ENABLE         0x01
#define DIOSPITOUCHSCREENSTMPE610_INT_CTRL_DISABLE        0x00

#define DIOSPITOUCHSCREENSTMPE610_INT_EN_TOUCHDET         0x01
#define DIOSPITOUCHSCREENSTMPE610_INT_EN_FIFOTH           0x02
#define DIOSPITOUCHSCREENSTMPE610_INT_EN_FIFOOF           0x04
#define DIOSPITOUCHSCREENSTMPE610_INT_EN_FIFOFULL         0x08
#define DIOSPITOUCHSCREENSTMPE610_INT_EN_FIFOEMPTY        0x10
#define DIOSPITOUCHSCREENSTMPE610_INT_EN_ADC              0x40
#define DIOSPITOUCHSCREENSTMPE610_INT_EN_GPIO             0x80

#define DIOSPITOUCHSCREENSTMPE610_INT_STA_TOUCHDET        0x01

#define DIOSPITOUCHSCREENSTMPE610_ADC_CTRL1_12BIT         0x08
#define DIOSPITOUCHSCREENSTMPE610_ADC_CTRL1_10BIT         0x00

#define DIOSPITOUCHSCREENSTMPE610_ADC_CTRL2_1_625MHZ      0x00
#define DIOSPITOUCHSCREENSTMPE610_ADC_CTRL2_3_25MHZ       0x01
#define DIOSPITOUCHSCREENSTMPE610_ADC_CTRL2_6_5MHZ        0x02

#define DIOSPITOUCHSCREENSTMPE610_TSC_CFG_1SAMPLE         0x00
#define DIOSPITOUCHSCREENSTMPE610_TSC_CFG_2SAMPLE         0x40
#define DIOSPITOUCHSCREENSTMPE610_TSC_CFG_4SAMPLE         0x80
#define DIOSPITOUCHSCREENSTMPE610_TSC_CFG_8SAMPLE         0xC0
#define DIOSPITOUCHSCREENSTMPE610_TSC_CFG_DELAY_10US      0x00
#define DIOSPITOUCHSCREENSTMPE610_TSC_CFG_DELAY_50US      0x08
#define DIOSPITOUCHSCREENSTMPE610_TSC_CFG_DELAY_100US     0x10
#define DIOSPITOUCHSCREENSTMPE610_TSC_CFG_DELAY_500US     0x18
#define DIOSPITOUCHSCREENSTMPE610_TSC_CFG_DELAY_1MS       0x20
#define DIOSPITOUCHSCREENSTMPE610_TSC_CFG_DELAY_5MS       0x28
#define DIOSPITOUCHSCREENSTMPE610_TSC_CFG_DELAY_10MS      0x30
#define DIOSPITOUCHSCREENSTMPE610_TSC_CFG_DELAY_50MS      0x38
#define DIOSPITOUCHSCREENSTMPE610_TSC_CFG_SETTLE_10US     0x00
#define DIOSPITOUCHSCREENSTMPE610_TSC_CFG_SETTLE_100US    0x01
#define DIOSPITOUCHSCREENSTMPE610_TSC_CFG_SETTLE_500US    0x02
#define DIOSPITOUCHSCREENSTMPE610_TSC_CFG_SETTLE_1MS      0x03
#define DIOSPITOUCHSCREENSTMPE610_TSC_CFG_SETTLE_5MS      0x04
#define DIOSPITOUCHSCREENSTMPE610_TSC_CFG_SETTLE_10MS     0x05
#define DIOSPITOUCHSCREENSTMPE610_TSC_CFG_SETTLE_50MS     0x06
#define DIOSPITOUCHSCREENSTMPE610_TSC_CFG_SETTLE_100MS    0x07

#define DIOSPITOUCHSCREENSTMPE610_FIFO_STA_RESET          0x01
#define DIOSPITOUCHSCREENSTMPE610_FIFO_STA_OFLOW          0x80
#define DIOSPITOUCHSCREENSTMPE610_FIFO_STA_FULL           0x40
#define DIOSPITOUCHSCREENSTMPE610_FIFO_STA_EMPTY          0x20
#define DIOSPITOUCHSCREENSTMPE610_FIFO_STA_THTRIG         0x10

#define DIOSPITOUCHSCREENSTMPE610_TSC_I_DRIVE_20MA        0x00
#define DIOSPITOUCHSCREENSTMPE610_TSC_I_DRIVE_50MA        0x01

#define DIOSPITOUCHSCREENSTMPE610_GPIO_SET_PIN            0x10
#define DIOSPITOUCHSCREENSTMPE610_GPIO_CLR_PIN            0x11
#define DIOSPITOUCHSCREENSTMPE610_GPIO_DIR                0x13
#define DIOSPITOUCHSCREENSTMPE610_GPIO_ALT_FUNCT          0x17

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XBUFFER;
class XTIMER;
class DIOSTREAMSPICONFIG;
class DIOSTREAMSPI;

class DIOSPITOUCHSCREENSTMPE610 : public DIODEVICE
{
  public:

                              DIOSPITOUCHSCREENSTMPE610       ();
    virtual                  ~DIOSPITOUCHSCREENSTMPE610       ();

    bool                      IniDevice                       (int chipselect, int timeout);

    XWORD                     GetVersion                      ();
    bool                      IsTouched                       ();
    bool                      IsBufferEmpty                   ();
    XBYTE                     GetBufferSize                   ();

    bool                      ReadPosition                    (XWORD& x, XWORD& y, XBYTE& z);

    bool                      End                             ();

    DIOSTREAMSPI*             GetDIOStreamSPI                 ();
    void                      SetDIOStreamSPI                 (DIOSTREAMSPI* diostreamSPI);

  private:


    bool                      TouchScreen_Ini                 ();

    bool                      ReadRegister                    (XBYTE reg, XBYTE& data);
    bool                      WriteRegister                   (XBYTE reg, XBYTE data);

    void                      Clean                           ();

    int                       timeout;

    XTIMER*                   xtimer;
    XBUFFER*                  databuffer;

    DIOSTREAMSPICONFIG*       diostreamSPIcfg;
    DIOSTREAMSPI*             diostreamSPI;
    bool                      isdiostreamSPIexternal;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





