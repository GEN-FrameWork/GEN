/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOI2CTouchSensorAT42QT1060.h
* 
* @class      DIOI2CTOUCHSENSORAT42QT1060
* @brief      Data Input/Output I2C Touch Sensor AT42QT1060 class
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


#define DIOI2CTOUCHSENSORAT42QT1060_ADDR 				                    0x12
#define DIOI2CTOUCHSENSORAT42QT1060_HW_ID   		                    0x31

#define DIOI2CTOUCHSENSORAT42QT1060_CHIP_ID_REG 										0
#define DIOI2CTOUCHSENSORAT42QT1060_VERSION_CHIP_REG								1
#define DIOI2CTOUCHSENSORAT42QT1060_MINOR_VERSION_REG								2
#define DIOI2CTOUCHSENSORAT42QT1060_DETECTION_STATUS_REG						4
#define DIOI2CTOUCHSENSORAT42QT1060_INPUT_PORT_STATUS_REG						5
#define DIOI2CTOUCHSENSORAT42QT1060_CALIBRATE_REG										12
#define DIOI2CTOUCHSENSORAT42QT1060_RESET_REG												13
#define DIOI2CTOUCHSENSORAT42QT1060_DRIFT_OPTION_REG								14
#define DIOI2CTOUCHSENSORAT42QT1060_POSITIVE_RECAL_REG							15
#define DIOI2CTOUCHSENSORAT42QT1060_NTHR_KEY_0											16
#define DIOI2CTOUCHSENSORAT42QT1060_NTHR_KEY_1											17
#define DIOI2CTOUCHSENSORAT42QT1060_NTHR_KEY_2											18
#define DIOI2CTOUCHSENSORAT42QT1060_NTHR_KEY_3											19
#define DIOI2CTOUCHSENSORAT42QT1060_NTHR_KEY_4											20		
#define DIOI2CTOUCHSENSORAT42QT1060_NTHR_KEY_5											21	
#define DIOI2CTOUCHSENSORAT42QT1060_LPM_MODE												22
#define DIOI2CTOUCHSENSORAT42QT1060_I_O_MASK_REG										23
#define DIOI2CTOUCHSENSORAT42QT1060_KEY_MASK_REG										24
#define DIOI2CTOUCHSENSORAT42QT1060_AKS_MASK_REG										25
#define DIOI2CTOUCHSENSORAT42QT1060_PWM_MASK_REG										26
#define DIOI2CTOUCHSENSORAT42QT1060_DETECTION_MASK_REG							27
#define DIOI2CTOUCHSENSORAT42QT1060_ACTIVE_LEVEL_MASK_REG						28
#define DIOI2CTOUCHSENSORAT42QT1060_USER_OUPUT_BUFFER_REG						29
#define DIOI2CTOUCHSENSORAT42QT1060_DI_REG													30
#define DIOI2CTOUCHSENSORAT42QT1060_PWM_LEVEL_REG										31
#define DIOI2CTOUCHSENSORAT42QT1060_KEY_0_SIGNAL_LSB_REG						40
#define DIOI2CTOUCHSENSORAT42QT1060_KEY_0_SIGNAL_MSB_REG						41		
#define DIOI2CTOUCHSENSORAT42QT1060_KEY_1_SIGNAL_LSB_REG						42
#define DIOI2CTOUCHSENSORAT42QT1060_KEY_1_SIGNAL_MSB_REG						43		
#define DIOI2CTOUCHSENSORAT42QT1060_KEY_2_SIGNAL_LSB_REG						44
#define DIOI2CTOUCHSENSORAT42QT1060_KEY_2_SIGNAL_MSB_REG						45
#define DIOI2CTOUCHSENSORAT42QT1060_KEY_3_SIGNAL_LSB_REG						46
#define DIOI2CTOUCHSENSORAT42QT1060_KEY_3_SIGNAL_MSB_REG						47
#define DIOI2CTOUCHSENSORAT42QT1060_KEY_4_SIGNAL_LSB_REG						48
#define DIOI2CTOUCHSENSORAT42QT1060_KEY_4_SIGNAL_MSB_REG						49
#define DIOI2CTOUCHSENSORAT42QT1060_KEY_5_SIGNAL_LSB_REG						50
#define DIOI2CTOUCHSENSORAT42QT1060_KEY_5_SIGNAL_MSB_REG						51
#define DIOI2CTOUCHSENSORAT42QT1060_KEY_0_REFERENCE_LSB_REG					52
#define DIOI2CTOUCHSENSORAT42QT1060_KEY_0_REFERENCE_MSB_REG					53		
#define DIOI2CTOUCHSENSORAT42QT1060_KEY_1_REFERENCE_LSB_REG					54
#define DIOI2CTOUCHSENSORAT42QT1060_KEY_1_REFERENCE_MSB_REG					55		
#define DIOI2CTOUCHSENSORAT42QT1060_KEY_2_REFERENCE_LSB_REG					56
#define DIOI2CTOUCHSENSORAT42QT1060_KEY_2_REFERENCE_MSB_REG					57
#define DIOI2CTOUCHSENSORAT42QT1060_KEY_3_REFERENCE_LSB_REG					58
#define DIOI2CTOUCHSENSORAT42QT1060_KEY_3_REFERENCE_MSB_REG					59
#define DIOI2CTOUCHSENSORAT42QT1060_KEY_4_REFERENCE_LSB_REG					60
#define DIOI2CTOUCHSENSORAT42QT1060_KEY_4_REFERENCE_MSB_REG					61
#define DIOI2CTOUCHSENSORAT42QT1060_KEY_5_REFERENCE_LSB_REG					62
#define DIOI2CTOUCHSENSORAT42QT1060_KEY_5_REFERENCE_MSB_REG					63

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOI2CTOUCHSENSORAT42QT1060: public DIODEVICEI2C
{
  public:

                              DIOI2CTOUCHSENSORAT42QT1060     ();
    virtual                  ~DIOI2CTOUCHSENSORAT42QT1060     ();

    XDWORD                    GetGPIOInterruptEntryID         ();
    bool                      SetGPIOInterruptEntryID         (XDWORD GPIOentryID);

    bool                      GetHW_ID                        (XBYTE& ID);
    
    bool                      IsNecessaryUpdate               ();  
    void                      SetIsNecessaryUpdate            (bool isnecessaryupdate);  

    XBYTE                     GetStatus                       ();

    bool                      End                             ();   

  private:

    bool                      IniDevice                       ();

    bool                      Initializate                    ();

    bool                      ReadRegister                    (XBYTE offset, XBYTE& data);
    bool                      ReadRegister                    (XBYTE offset, XBYTE* data, XDWORD sizedata);

    bool                      WriteRegister                   (XBYTE offset, XBYTE data);
    bool                      WriteRegister                   (XBYTE offset, XBYTE* data, XDWORD sizedata);

    static void               Handler_Interrupt               (void* param);
    

    void                      Clean                           ();

    XDWORD                    GPIOinterruptentryID; 
    bool                      isnecessaryupdate;  
    XBYTE                     prevtouch;
   
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





