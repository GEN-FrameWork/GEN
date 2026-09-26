/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOI2CBatteryChargerBQ24295.h
* 
* @class      DIOI2CBATTERYCHARGERBQ24295
* @brief      Data Input/output Battery Charger BQ24295 class
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

enum DIOI2CBATTERYCHARGERBQ24295_GPIOENTRY
{
  DIOI2CBATTERYCHARGERBQ24295_GPIOENTRYID_DET           ,
  DIOI2CBATTERYCHARGERBQ24295_GPIOENTRYID_STAT          ,
  DIOI2CBATTERYCHARGERBQ24295_GPIOENTRYID_QON           ,    
  DIOI2CBATTERYCHARGERBQ24295_GPIOENTRYID_CE            ,
  DIOI2CBATTERYCHARGERBQ24295_GPIOENTRYID_INT           ,

  DIOI2CBATTERYCHARGERBQ24295_GPIOENTRYID_END           ,
};

#define DIOI2CBATTERYCHARGERBQ24295_ADDR                                      0x6B

// Register map
#define DIOI2CBATTERYCHARGERBQ24295_INPUT_SRC_CTRL_REG00                      0x00
#define DIOI2CBATTERYCHARGERBQ24295_PWRON_CONF_REG01                          0x01
#define DIOI2CBATTERYCHARGERBQ24295_CHRG_CURRENT_CTRL_REG02                   0x02
#define DIOI2CBATTERYCHARGERBQ24295_PRE_CHRG_TERM_CURR_CTRL_REG03             0x03
#define DIOI2CBATTERYCHARGERBQ24295_CHRG_VOLT_CTRL_REG04                      0x04
#define DIOI2CBATTERYCHARGERBQ24295_CHRG_TERM_TIMER_CTRL_REG05                0x05
#define DIOI2CBATTERYCHARGERBQ24295_BOOST_VOLTAGE_THR_REG_CTRL_REG06          0x06
#define DIOI2CBATTERYCHARGERBQ24295_MISC_OPERATION_CTRL_REG07                 0x07
#define DIOI2CBATTERYCHARGERBQ24295_SYS_STATUS_REG08                          0x08
#define DIOI2CBATTERYCHARGERBQ24295_FAULT_REG_REG09                           0x09
#define DIOI2CBATTERYCHARGERBQ24295_VENDOR_PART_REVISION_REG0A                0x0A

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOI2CBATTERYCHARGERBQ24295: public DIODEVICEI2C
{
  public:

                              DIOI2CBATTERYCHARGERBQ24295         ();
    virtual                  ~DIOI2CBATTERYCHARGERBQ24295         ();

    XDWORD                    GetGPIOEntryID                      (int indexentry);
    bool                      SetGPIOEntryID                      (int indexentry, XDWORD GPIOentryID);
    
    void                      Update                              ();  

    void                      SetChargerEnable                    (bool enable);
    void                      SetShippingModeEnable               (bool enable);
    
    void                      SetIsCharging                       (bool enable);
    bool                      IsBatteryFullyCharged               ();

    XBYTE                     GetFaultDetection                   ();

    bool                      End                                 ();

  private:

    bool                      IniDevice                           ();

    bool                      Initialize                          ();

    bool                      ReadRegister                        (XBYTE offset, XBYTE& data);
    bool                      ReadRegister                        (XBYTE offset, XBYTE* data, XDWORD sizedata);

    bool                      WriteRegister                       (XBYTE offset, XBYTE data);
    bool                      WriteRegister                       (XBYTE offset, XBYTE* data, XDWORD sizedata);

    static void               Handler_Interrupt                   (void* param);
      
    void                      Clean                               ();


    XDWORD                    GPIOentryID[DIOI2CBATTERYCHARGERBQ24295_GPIOENTRYID_END];
    bool                      charging;    
    XBYTE                     fault;
    bool                      fullycharge;    
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





