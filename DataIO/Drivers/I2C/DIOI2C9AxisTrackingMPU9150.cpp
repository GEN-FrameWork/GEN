/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOI2C9AxisTrackingMPU9150.cpp
* 
* @class      DIOI2C9AXISTRACKINGMPU9150
* @brief      Data Input/Output I2C 9 Axis Motion Tracking MPU9150 (InvenSense - MPU9150) class 
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOI2C9AxisTrackingMPU9150.h"

#include <stdio.h>
#include <stdlib.h>

#include "XFactory.h"
#include "XBuffer.h"
#include "XTrace.h"
#include "XSleep.h"

#include "DIOFactory.h"
#include "DIOStreamI2CConfig.h"
#include "DIOStreamI2C.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOI2C9AXISTRACKINGMPU9150::DIOI2C9AXISTRACKINGMPU9150() : DIODEVICEI2C()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOI2C9AXISTRACKINGMPU9150::DIOI2C9AXISTRACKINGMPU9150() : DIODEVICEI2C()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOI2C9AXISTRACKINGMPU9150::~DIOI2C9AXISTRACKINGMPU9150()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOI2C9AXISTRACKINGMPU9150::~DIOI2C9AXISTRACKINGMPU9150()
{
  End();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::End()
* @brief      End the object
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::End()
{
  if(!DIODEVICEI2C::End()) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::IniDevice()
* @brief      Ini device
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::IniDevice()
{
  if(!DIODEVICEI2C::IniDevice()) return false;

  if(diostream->Open())
    {
      if(diostream->WaitToConnected(timeout))
        {
          return DIODEVICE::Ini();
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::Initialize()
* @brief      Initialize
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::Initialize()
{
  SetClockSource(DIOI2CMPU9150_CLOCK_PLL_XGYRO);
  SetFullScaleGyroRange(DIOI2CMPU9150_GYRO_FS_250);
  SetFullScaleAccelRange(DIOI2CMPU9150_ACCEL_FS_2);
  SetSleepEnabled(false); // thanks to Jack Elston for pointing this one out!
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::TestConnection()
* @brief      Test connection
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::TestConnection()
{
  return (GetDeviceID() == 0x34)?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOI2C9AXISTRACKINGMPU9150::GetAuxVDDIOLevel()
* @brief      Get aux VDDIO level When set to 1, the auxiliary I2C bus high logic level is VDD. When cleared to 0, the auxiliary I2C bus high logic level is VLOGIC. This does not apply to the MPU-6000, which does not have a VLOGIC pin.
* @ingroup    DATAIO
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetAuxVDDIOLevel()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_YG_OFFS_TC, DIOI2CMPU9150_TC_PWR_MODE_BIT, buffer);
  return buffer[0];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetAuxVDDIOLevel(XBYTE level)
* @brief      Set aux VDDIO level When set to 1, the auxiliary I2C bus high logic level is VDD. When cleared to 0, the auxiliary I2C bus high logic level is VLOGIC. This does not apply to the MPU-6000, which does not have a VLOGIC pin.
* @ingroup    DATAIO
* 
* @param[in]  level : Level value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetAuxVDDIOLevel(XBYTE level)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_YG_OFFS_TC, DIOI2CMPU9150_TC_PWR_MODE_BIT, level);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOI2C9AXISTRACKINGMPU9150::GetRate()
* @brief      Get rate The sensor register output, FIFO output, DMP sampling, Motion detection, Zero Motion detection, and Free Fall detection are all based on the Sample Rate. The Sample Rate is generated by dividing the gyroscope output rate by SMPLRT_DIV:
* @ingroup    DATAIO
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetRate()
{
  ReadByte(deviceaddr, DIOI2CMPU9150_RA_SMPLRT_DIV, buffer);
  return buffer[0];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetRate(XBYTE rate)
* @brief      Set rate
* @ingroup    DATAIO
* 
* @param[in]  rate : Rate value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetRate(XBYTE rate)
{
  WriteByte(deviceaddr, DIOI2CMPU9150_RA_SMPLRT_DIV, rate);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOI2C9AXISTRACKINGMPU9150::CheckMagStatus()
* @brief      Check mag status
* @ingroup    DATAIO
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::CheckMagStatus()
{
   ReadByte(DIOI2CMPU9150_RA_MAG_ADDRESS, 0x02, buffer);
   return buffer[0];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOI2C9AXISTRACKINGMPU9150::GetExternalFrameSync()
* @brief      Get external frame sync Configures the external Frame Synchronization (FSYNC) pin sampling. An external signal connected to the FSYNC pin can be sampled by configuring EXT_SYNC_SET. Signal changes to the FSYNC pin are latched so that short strobes may be captured. The latched FSYNC signal will be sampled at the Sampling Rate, as defined in register 25. After sampling, the latch will reset to the current FSYNC signal state.
* @ingroup    DATAIO
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetExternalFrameSync()
{
  ReadBits(deviceaddr, DIOI2CMPU9150_RA_CONFIG, DIOI2CMPU9150_CFG_EXT_SYNC_SET_BIT, DIOI2CMPU9150_CFG_EXT_SYNC_SET_LENGTH, buffer);
  return buffer[0];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetExternalFrameSync(XBYTE sync)
* @brief      Set external frame sync
* @ingroup    DATAIO
* 
* @param[in]  sync : New FSYNC configuration value
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetExternalFrameSync(XBYTE sync)
{
  WriteBits(deviceaddr, DIOI2CMPU9150_RA_CONFIG, DIOI2CMPU9150_CFG_EXT_SYNC_SET_BIT, DIOI2CMPU9150_CFG_EXT_SYNC_SET_LENGTH, sync);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOI2C9AXISTRACKINGMPU9150::GetDLPFMode()
* @brief      Get DLPF mode The DLPF_CFG parameter sets the digital low pass filter configuration. It also determines the internal sampling rate used by the device as shown in the table below.
* @ingroup    DATAIO
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetDLPFMode()
{
  ReadBits(deviceaddr, DIOI2CMPU9150_RA_CONFIG, DIOI2CMPU9150_CFG_DLPF_CFG_BIT, DIOI2CMPU9150_CFG_DLPF_CFG_LENGTH, buffer);
  return buffer[0];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetDLPFMode(XBYTE mode)
* @brief      Set DLPF mode
* @ingroup    DATAIO
* 
* @param[in]  mode : New DLFP configuration setting
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetDLPFMode(XBYTE mode)
{
  WriteBits(deviceaddr, DIOI2CMPU9150_RA_CONFIG, DIOI2CMPU9150_CFG_DLPF_CFG_BIT, DIOI2CMPU9150_CFG_DLPF_CFG_LENGTH, mode);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOI2C9AXISTRACKINGMPU9150::GetFullScaleGyroRange()
* @brief      Get full scale gyro range The FS_SEL parameter allows setting the full-scale range of the gyro sensors, as described in the table below. 0 = +/- 250 degrees/sec 1 = +/- 500 degrees/sec 2 = +/- 1000 degrees/sec 3 = +/- 2000 degrees/sec
* @ingroup    DATAIO
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetFullScaleGyroRange()
{
  ReadBits(deviceaddr, DIOI2CMPU9150_RA_GYRO_CONFIG, DIOI2CMPU9150_GCONFIG_FS_SEL_BIT, DIOI2CMPU9150_GCONFIG_FS_SEL_LENGTH, buffer);
  return buffer[0];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetFullScaleGyroRange(XBYTE range)
* @brief      Set full scale gyro range
* @ingroup    DATAIO
* 
* @param[in]  range : New full-scale gyroscope range value
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetFullScaleGyroRange(XBYTE range)
{
  WriteBits(deviceaddr, DIOI2CMPU9150_RA_GYRO_CONFIG, DIOI2CMPU9150_GCONFIG_FS_SEL_BIT, DIOI2CMPU9150_GCONFIG_FS_SEL_LENGTH, range);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetAccelXSelfTest()
* @brief      Get accel X self test
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetAccelXSelfTest()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_ACCEL_CONFIG, DIOI2CMPU9150_ACONFIG_XA_ST_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetAccelXSelfTest(bool enabled)
* @brief      Set accel X self test
* @ingroup    DATAIO
* 
* @param[in]  enabled : Self-test enabled value
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetAccelXSelfTest(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_ACCEL_CONFIG, DIOI2CMPU9150_ACONFIG_XA_ST_BIT, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetAccelYSelfTest()
* @brief      Get accel Y self test
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetAccelYSelfTest()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_ACCEL_CONFIG, DIOI2CMPU9150_ACONFIG_YA_ST_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetAccelYSelfTest(bool enabled)
* @brief      Set accel Y self test
* @ingroup    DATAIO
* 
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetAccelYSelfTest(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_ACCEL_CONFIG, DIOI2CMPU9150_ACONFIG_YA_ST_BIT, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetAccelZSelfTest()
* @brief      Get accel Z self test
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetAccelZSelfTest()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_ACCEL_CONFIG, DIOI2CMPU9150_ACONFIG_ZA_ST_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetAccelZSelfTest(bool enabled)
* @brief      Set accel Z self test
* @ingroup    DATAIO
* 
* @param[in]  enabled : Self-test enabled value
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetAccelZSelfTest(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_ACCEL_CONFIG, DIOI2CMPU9150_ACONFIG_ZA_ST_BIT, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOI2C9AXISTRACKINGMPU9150::GetFullScaleAccelRange()
* @brief      Get full scale accel range The FS_SEL parameter allows setting the full-scale range of the accelerometer sensors, as described in the table below.
* @ingroup    DATAIO
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetFullScaleAccelRange()
{
  ReadBits(deviceaddr, DIOI2CMPU9150_RA_ACCEL_CONFIG, DIOI2CMPU9150_ACONFIG_AFS_SEL_BIT, DIOI2CMPU9150_ACONFIG_AFS_SEL_LENGTH, buffer);
  return buffer[0];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetFullScaleAccelRange(XBYTE range)
* @brief      Set full scale accel range
* @ingroup    DATAIO
* 
* @param[in]  range : New full-scale accelerometer range setting
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetFullScaleAccelRange(XBYTE range)
{
  WriteBits(deviceaddr, DIOI2CMPU9150_RA_ACCEL_CONFIG, DIOI2CMPU9150_ACONFIG_AFS_SEL_BIT, DIOI2CMPU9150_ACONFIG_AFS_SEL_LENGTH, range);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOI2C9AXISTRACKINGMPU9150::GetDHPFMode()
* @brief      Get DHPF mode The DHPF is a filter module in the path leading to motion detectors (Free Fall, Motion threshold, and Zero Motion). The high pass filter output is not available to the data registers (see Figure in Section 8 of the MPU-6000/ MPU-6050 Product Specification document).
* @ingroup    DATAIO
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetDHPFMode()
{
  ReadBits(deviceaddr, DIOI2CMPU9150_RA_ACCEL_CONFIG, DIOI2CMPU9150_ACONFIG_ACCEL_HPF_BIT, DIOI2CMPU9150_ACONFIG_ACCEL_HPF_LENGTH, buffer);
  return buffer[0];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetDHPFMode(XBYTE bandwidth)
* @brief      Set DHPF mode
* @ingroup    DATAIO
* 
* @param[in]  bandwidth : Bandwidth value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetDHPFMode(XBYTE bandwidth)
{
  WriteBits(deviceaddr, DIOI2CMPU9150_RA_ACCEL_CONFIG, DIOI2CMPU9150_ACONFIG_ACCEL_HPF_BIT, DIOI2CMPU9150_ACONFIG_ACCEL_HPF_LENGTH, bandwidth);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOI2C9AXISTRACKINGMPU9150::GetFreefallDetectionThreshold()
* @brief      Get freefall detection threshold This register configures the detection threshold for Free Fall event detection. The unit of FF_THR is 1LSB = 2mg. Free Fall is detected when the absolute value of the accelerometer measurements for the three axes are each less than the detection threshold. This condition increments the Free Fall duration counter (Register 30). The Free Fall interrupt is triggered when the Free Fall duration counter reaches the time specified in FF_DUR.
* @ingroup    DATAIO
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetFreefallDetectionThreshold()
{
  ReadByte(deviceaddr, DIOI2CMPU9150_RA_FF_THR, buffer);
  return buffer[0];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetFreefallDetectionThreshold(XBYTE threshold)
* @brief      Set freefall detection threshold
* @ingroup    DATAIO
* 
* @param[in]  threshold : Threshold value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetFreefallDetectionThreshold(XBYTE threshold)
{
  WriteByte(deviceaddr, DIOI2CMPU9150_RA_FF_THR, threshold);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOI2C9AXISTRACKINGMPU9150::GetFreefallDetectionDuration()
* @brief      Get freefall detection duration this register configures the duration counter threshold for Free Fall event detection. The duration counter ticks at 1kHz, therefore FF_DUR has a unit of 1 LSB = 1 ms.
* @ingroup    DATAIO
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetFreefallDetectionDuration()
{
  ReadByte(deviceaddr, DIOI2CMPU9150_RA_FF_DUR, buffer);
  return buffer[0];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetFreefallDetectionDuration(XBYTE duration)
* @brief      Set freefall detection duration
* @ingroup    DATAIO
* 
* @param[in]  duration : Duration value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetFreefallDetectionDuration(XBYTE duration)
{
  WriteByte(deviceaddr, DIOI2CMPU9150_RA_FF_DUR, duration);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOI2C9AXISTRACKINGMPU9150::GetMotionDetectionThreshold()
* @brief      Get motion detection threshold This register configures the detection threshold for Motion interrupt generation. The unit of MOT_THR is 1LSB = 2mg. Motion is detected when the absolute value of any of the accelerometer measurements exceeds this Motion detection threshold. This condition increments the Motion detection duration counter (Register 32). The Motion detection interrupt is triggered when the Motion Detection counter reaches the time count specified in MOT_DUR (Register 32).
* @ingroup    DATAIO
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetMotionDetectionThreshold()
{
  ReadByte(deviceaddr, DIOI2CMPU9150_RA_MOT_THR, buffer);
  return buffer[0];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetMotionDetectionThreshold(XBYTE threshold)
* @brief      Set motion detection threshold
* @ingroup    DATAIO
* 
* @param[in]  threshold : New motion detection acceleration threshold value (LSB = 2mg)
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetMotionDetectionThreshold(XBYTE threshold)
{
  WriteByte(deviceaddr, DIOI2CMPU9150_RA_MOT_THR, threshold);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOI2C9AXISTRACKINGMPU9150::GetMotionDetectionDuration()
* @brief      Get motion detection duration This register configures the duration counter threshold for Motion interrupt generation. The duration counter ticks at 1 kHz, therefore MOT_DUR has a unit of 1LSB = 1ms. The Motion detection duration counter increments when the absolute value of any of the accelerometer measurements exceeds the Motion detection threshold (Register 31). The Motion detection interrupt is triggered when the Motion detection counter reaches the time count specified in this register.
* @ingroup    DATAIO
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetMotionDetectionDuration()
{
  ReadByte(deviceaddr, DIOI2CMPU9150_RA_MOT_DUR, buffer);
  return buffer[0];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetMotionDetectionDuration(XBYTE duration)
* @brief      Set motion detection duration
* @ingroup    DATAIO
* 
* @param[in]  duration : New motion detection duration threshold value (LSB = 1ms)
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetMotionDetectionDuration(XBYTE duration)
{
  WriteByte(deviceaddr, DIOI2CMPU9150_RA_MOT_DUR, duration);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOI2C9AXISTRACKINGMPU9150::GetZeroMotionDetectionThreshold()
* @brief      Get zero motion detection threshold This register configures the detection threshold for Zero Motion interrupt generation. The unit of ZRMOT_THR is 1LSB = 2mg. Zero Motion is detected when the absolute value of the accelerometer measurements for the 3 axes are each less than the detection threshold. This condition increments the Zero Motion duration counter (Register 34). The Zero Motion interrupt is triggered when the Zero Motion duration counter reaches the time count specified in ZRMOT_DUR (Register 34).
* @ingroup    DATAIO
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetZeroMotionDetectionThreshold()
{
  ReadByte(deviceaddr, DIOI2CMPU9150_RA_ZRMOT_THR, buffer);
  return buffer[0];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetZeroMotionDetectionThreshold(XBYTE threshold)
* @brief      Set zero motion detection threshold
* @ingroup    DATAIO
* 
* @param[in]  threshold : New zero motion detection acceleration threshold value (LSB = 2mg)
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetZeroMotionDetectionThreshold(XBYTE threshold)
{
  WriteByte(deviceaddr, DIOI2CMPU9150_RA_ZRMOT_THR, threshold);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOI2C9AXISTRACKINGMPU9150::GetZeroMotionDetectionDuration()
* @brief      Get zero motion detection duration This register configures the duration counter threshold for Zero Motion interrupt generation. The duration counter ticks at 16 Hz, therefore ZRMOT_DUR has a unit of 1 LSB = 64 ms. The Zero Motion duration counter increments while the absolute value of the accelerometer measurements are each less than the detection threshold (Register 33). The Zero Motion interrupt is triggered when the Zero Motion duration counter reaches the time count specified in this register.
* @ingroup    DATAIO
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetZeroMotionDetectionDuration()
{
  ReadByte(deviceaddr, DIOI2CMPU9150_RA_ZRMOT_DUR, buffer);
  return buffer[0];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetZeroMotionDetectionDuration(XBYTE duration)
* @brief      Set zero motion detection duration
* @ingroup    DATAIO
* 
* @param[in]  duration : New zero motion detection duration threshold value (LSB = 1ms)
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetZeroMotionDetectionDuration(XBYTE duration)
{
  WriteByte(deviceaddr, DIOI2CMPU9150_RA_ZRMOT_DUR, duration);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetTempFIFOEnabled()
* @brief      Get temp FIFO enabled When set to 1, this bit enables TEMP_OUT_H and TEMP_OUT_L (Registers 65 and 66) to be written into the FIFO buffer.
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetTempFIFOEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_FIFO_EN, DIOI2CMPU9150_TEMP_FIFO_EN_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetTempFIFOEnabled(bool enabled)
* @brief      Set temp FIFO enabled
* @ingroup    DATAIO
* 
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetTempFIFOEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_FIFO_EN, DIOI2CMPU9150_TEMP_FIFO_EN_BIT, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetXGyroFIFOEnabled()
* @brief      Get X gyro FIFO enabled When set to 1, this bit enables GYRO_XOUT_H and GYRO_XOUT_L (Registers 67 and 68) to be written into the FIFO buffer.
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetXGyroFIFOEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_FIFO_EN, DIOI2CMPU9150_XG_FIFO_EN_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetXGyroFIFOEnabled(bool enabled)
* @brief      Set X gyro FIFO enabled
* @ingroup    DATAIO
* 
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetXGyroFIFOEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_FIFO_EN, DIOI2CMPU9150_XG_FIFO_EN_BIT, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetYGyroFIFOEnabled()
* @brief      Get Y gyro FIFO enabled When set to 1, this bit enables GYRO_YOUT_H and GYRO_YOUT_L (Registers 69 and 70) to be written into the FIFO buffer.
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetYGyroFIFOEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_FIFO_EN, DIOI2CMPU9150_YG_FIFO_EN_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetYGyroFIFOEnabled(bool enabled)
* @brief      Set Y gyro FIFO enabled
* @ingroup    DATAIO
* 
* @param[in]  enabled : New gyroscope Y-axis FIFO enabled value
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetYGyroFIFOEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_FIFO_EN, DIOI2CMPU9150_YG_FIFO_EN_BIT, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetZGyroFIFOEnabled()
* @brief      Get Z gyro FIFO enabled When set to 1, this bit enables GYRO_ZOUT_H and GYRO_ZOUT_L (Registers 71 and 72) to be written into the FIFO buffer.
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetZGyroFIFOEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_FIFO_EN, DIOI2CMPU9150_ZG_FIFO_EN_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetZGyroFIFOEnabled(bool enabled)
* @brief      Set Z gyro FIFO enabled
* @ingroup    DATAIO
* 
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetZGyroFIFOEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_FIFO_EN, DIOI2CMPU9150_ZG_FIFO_EN_BIT, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetAccelFIFOEnabled()
* @brief      Get accel fifo enabled
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetAccelFIFOEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_FIFO_EN, DIOI2CMPU9150_ACCEL_FIFO_EN_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetAccelFIFOEnabled(bool enabled)
* @brief      Set accel fifo enabled
* @ingroup    DATAIO
* 
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetAccelFIFOEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_FIFO_EN, DIOI2CMPU9150_ACCEL_FIFO_EN_BIT, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetSlave2FIFOEnabled()
* @brief      Get slave2 fifo enabled
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetSlave2FIFOEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_FIFO_EN, DIOI2CMPU9150_SLV2_FIFO_EN_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetSlave2FIFOEnabled(bool enabled)
* @brief      Set slave2 fifo enabled
* @ingroup    DATAIO
* 
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetSlave2FIFOEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_FIFO_EN, DIOI2CMPU9150_SLV2_FIFO_EN_BIT, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetSlave1FIFOEnabled()
* @brief      Get slave1 fifo enabled
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetSlave1FIFOEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_FIFO_EN, DIOI2CMPU9150_SLV1_FIFO_EN_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetSlave1FIFOEnabled(bool enabled)
* @brief      Set slave1 fifo enabled
* @ingroup    DATAIO
* 
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetSlave1FIFOEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_FIFO_EN, DIOI2CMPU9150_SLV1_FIFO_EN_BIT, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetSlave0FIFOEnabled()
* @brief      Get slave0 fifo enabled
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetSlave0FIFOEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_FIFO_EN, DIOI2CMPU9150_SLV0_FIFO_EN_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetSlave0FIFOEnabled(bool enabled)
* @brief      Set slave0 fifo enabled
* @ingroup    DATAIO
* 
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetSlave0FIFOEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_FIFO_EN, DIOI2CMPU9150_SLV0_FIFO_EN_BIT, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetMultiMasterEnabled()
* @brief      Get multi master enabled
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetMultiMasterEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_I2C_MST_CTRL, DIOI2CMPU9150_MULT_MST_EN_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetMultiMasterEnabled(bool enabled)
* @brief      Set multi master enabled
* @ingroup    DATAIO
* 
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetMultiMasterEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_I2C_MST_CTRL, DIOI2CMPU9150_MULT_MST_EN_BIT, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetWaitForExternalSensorEnabled()
* @brief      Get wait for external sensor enabled
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetWaitForExternalSensorEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_I2C_MST_CTRL, DIOI2CMPU9150_WAIT_FOR_ES_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetWaitForExternalSensorEnabled(bool enabled)
* @brief      Set wait for external sensor enabled
* @ingroup    DATAIO
* 
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetWaitForExternalSensorEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_I2C_MST_CTRL, DIOI2CMPU9150_WAIT_FOR_ES_BIT, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetSlave3FIFOEnabled()
* @brief      Get slave3 fifo enabled
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetSlave3FIFOEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_I2C_MST_CTRL, DIOI2CMPU9150_SLV_3_FIFO_EN_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetSlave3FIFOEnabled(bool enabled)
* @brief      Set slave3 fifo enabled
* @ingroup    DATAIO
* 
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetSlave3FIFOEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_I2C_MST_CTRL, DIOI2CMPU9150_SLV_3_FIFO_EN_BIT, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetSlaveReadWriteTransitionEnabled()
* @brief      Get slave read write transition enabled
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetSlaveReadWriteTransitionEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_I2C_MST_CTRL, DIOI2CMPU9150_I2C_MST_P_NSR_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetSlaveReadWriteTransitionEnabled(bool enabled)
* @brief      Set slave read write transition enabled
* @ingroup    DATAIO
* 
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetSlaveReadWriteTransitionEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_I2C_MST_CTRL, DIOI2CMPU9150_I2C_MST_P_NSR_BIT, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOI2C9AXISTRACKINGMPU9150::GetMasterClockSpeed()
* @brief      Get master clock speed
* @ingroup    DATAIO
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetMasterClockSpeed()
{
  ReadBits(deviceaddr, DIOI2CMPU9150_RA_I2C_MST_CTRL, DIOI2CMPU9150_I2C_MST_CLK_BIT, DIOI2CMPU9150_I2C_MST_CLK_LENGTH, buffer);
  return buffer[0];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetMasterClockSpeed(XBYTE speed)
* @brief      Set master clock speed
* @ingroup    DATAIO
* 
* @param[in]  speed : Speed value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetMasterClockSpeed(XBYTE speed)
{
  WriteBits(deviceaddr, DIOI2CMPU9150_RA_I2C_MST_CTRL, DIOI2CMPU9150_I2C_MST_CLK_BIT, DIOI2CMPU9150_I2C_MST_CLK_LENGTH, speed);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetSlaveAddress(XBYTE num, XBYTE address)
* @brief      Set slave address
* @ingroup    DATAIO
* 
* @param[in]  num : Num value.
* @param[in]  address : Address value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetSlaveAddress(XBYTE num, XBYTE address)
{
   if(num > 3) return;

   WriteByte(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV0_ADDR + num*3, address);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOI2C9AXISTRACKINGMPU9150::GetSlaveRegister(XBYTE num)
* @brief      Get slave register
* @ingroup    DATAIO
* 
* @param[in]  num : Num value.
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetSlaveRegister(XBYTE num)
{
  if(num > 3) return 0;

  ReadByte(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV0_REG + num*3, buffer);
  return buffer[0];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetSlaveRegister(XBYTE num, XBYTE reg)
* @brief      Set slave register
* @ingroup    DATAIO
* 
* @param[in]  num : Num value.
* @param[in]  reg : Reg value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetSlaveRegister(XBYTE num, XBYTE reg)
{
  if(num > 3) return;

  WriteByte(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV0_REG + num*3, reg);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetSlaveEnabled(XBYTE num)
* @brief      Get slave enabled
* @ingroup    DATAIO
* 
* @param[in]  num : Num value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetSlaveEnabled(XBYTE num)
{
  if(num > 3) return false;

  ReadBit(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV0_CTRL + num*3, DIOI2CMPU9150_I2C_SLV_EN_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetSlaveEnabled(XBYTE num, bool enabled)
* @brief      Set slave enabled
* @ingroup    DATAIO
* 
* @param[in]  num : Num value.
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetSlaveEnabled(XBYTE num, bool enabled)
{
  if(num > 3) return;

  WriteBit(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV0_CTRL + num*3, DIOI2CMPU9150_I2C_SLV_EN_BIT, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetSlaveWordByteSwap(XBYTE num)
* @brief      Get slave word byte swap
* @ingroup    DATAIO
* 
* @param[in]  num : Num value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetSlaveWordByteSwap(XBYTE num)
{
  if(num > 3) return false;

  ReadBit(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV0_CTRL + num*3, DIOI2CMPU9150_I2C_SLV_BYTE_SW_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetSlaveWordByteSwap(XBYTE num, bool enabled)
* @brief      Set slave word byte swap
* @ingroup    DATAIO
* 
* @param[in]  num : Num value.
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetSlaveWordByteSwap(XBYTE num, bool enabled)
{
  if(num > 3) return;

  WriteBit(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV0_CTRL + num*3, DIOI2CMPU9150_I2C_SLV_BYTE_SW_BIT, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetSlaveWriteMode(XBYTE num)
* @brief      Get slave write mode
* @ingroup    DATAIO
* 
* @param[in]  num : Num value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetSlaveWriteMode(XBYTE num)
{
  if(num > 3) return false;

  ReadBit(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV0_CTRL + num*3, DIOI2CMPU9150_I2C_SLV_REG_DIS_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetSlaveWriteMode(XBYTE num, bool mode)
* @brief      Set slave write mode
* @ingroup    DATAIO
* 
* @param[in]  num : Num value.
* @param[in]  mode : Mode value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetSlaveWriteMode(XBYTE num, bool mode)
{
  if(num > 3) return;

  WriteBit(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV0_CTRL + num*3, DIOI2CMPU9150_I2C_SLV_REG_DIS_BIT, mode);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetSlaveWordGroupOffset(XBYTE num)
* @brief      Get slave word group offset
* @ingroup    DATAIO
* 
* @param[in]  num : Num value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetSlaveWordGroupOffset(XBYTE num)
{
  if(num > 3) return false;

  ReadBit(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV0_CTRL + num*3, DIOI2CMPU9150_I2C_SLV_GRP_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetSlaveWordGroupOffset(XBYTE num, bool enabled)
* @brief      Set slave word group offset
* @ingroup    DATAIO
* 
* @param[in]  num : Num value.
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetSlaveWordGroupOffset(XBYTE num, bool enabled)
{
  if(num > 3) return;

  WriteBit(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV0_CTRL + num*3, DIOI2CMPU9150_I2C_SLV_GRP_BIT, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOI2C9AXISTRACKINGMPU9150::GetSlaveDataLength(XBYTE num)
* @brief      Get slave data length
* @ingroup    DATAIO
* 
* @param[in]  num : Num value.
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetSlaveDataLength(XBYTE num)
{
  if(num > 3) return 0;
  ReadBits(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV0_CTRL + num*3, DIOI2CMPU9150_I2C_SLV_LEN_BIT, DIOI2CMPU9150_I2C_SLV_LEN_LENGTH, buffer);
  return buffer[0];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetSlaveDataLength(XBYTE num, XBYTE length)
* @brief      Set slave data length
* @ingroup    DATAIO
* 
* @param[in]  num : Num value.
* @param[in]  length : Length value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetSlaveDataLength(XBYTE num, XBYTE length)
{
  if(num > 3) return;
  WriteBits(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV0_CTRL + num*3, DIOI2CMPU9150_I2C_SLV_LEN_BIT, DIOI2CMPU9150_I2C_SLV_LEN_LENGTH, length);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOI2C9AXISTRACKINGMPU9150::GetSlave4Address()
* @brief      Get slave4 address
* @ingroup    DATAIO
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetSlave4Address()
{
  ReadByte(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV4_ADDR, buffer);
  return buffer[0];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetSlave4Address(XBYTE address)
* @brief      Set slave4 address
* @ingroup    DATAIO
* 
* @param[in]  address : Address value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetSlave4Address(XBYTE address)
{
  WriteByte(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV4_ADDR, address);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOI2C9AXISTRACKINGMPU9150::GetSlave4Register()
* @brief      Get slave4 register
* @ingroup    DATAIO
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetSlave4Register()
{
  ReadByte(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV4_REG, buffer);
  return buffer[0];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetSlave4Register(XBYTE reg)
* @brief      Set slave4 register
* @ingroup    DATAIO
* 
* @param[in]  reg : Reg value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetSlave4Register(XBYTE reg)
{
  WriteByte(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV4_REG, reg);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetSlave4OutputByte(XBYTE data)
* @brief      Set slave4 output byte
* @ingroup    DATAIO
* 
* @param[in]  data : Data buffer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetSlave4OutputByte(XBYTE data)
{
  WriteByte(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV4_DO, data);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetSlave4Enabled()
* @brief      Get slave4 enabled
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetSlave4Enabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV4_CTRL, DIOI2CMPU9150_I2C_SLV4_EN_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetSlave4Enabled(bool enabled)
* @brief      Set slave4 enabled
* @ingroup    DATAIO
* 
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetSlave4Enabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV4_CTRL, DIOI2CMPU9150_I2C_SLV4_EN_BIT, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetSlave4InterruptEnabled()
* @brief      Get slave4 interrupt enabled
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetSlave4InterruptEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV4_CTRL, DIOI2CMPU9150_I2C_SLV4_INT_EN_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetSlave4InterruptEnabled(bool enabled)
* @brief      Set slave4 interrupt enabled
* @ingroup    DATAIO
* 
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetSlave4InterruptEnabled(bool enabled)
{
   WriteBit(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV4_CTRL, DIOI2CMPU9150_I2C_SLV4_INT_EN_BIT, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetSlave4WriteMode()
* @brief      Get slave4 write mode
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetSlave4WriteMode()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV4_CTRL, DIOI2CMPU9150_I2C_SLV4_REG_DIS_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetSlave4WriteMode(bool mode)
* @brief      Set slave4 write mode
* @ingroup    DATAIO
* 
* @param[in]  mode : Mode value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetSlave4WriteMode(bool mode)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV4_CTRL, DIOI2CMPU9150_I2C_SLV4_REG_DIS_BIT, mode);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOI2C9AXISTRACKINGMPU9150::GetSlave4MasterDelay()
* @brief      Get slave4 master delay
* @ingroup    DATAIO
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetSlave4MasterDelay()
{
  ReadBits(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV4_CTRL, DIOI2CMPU9150_I2C_SLV4_MST_DLY_BIT, DIOI2CMPU9150_I2C_SLV4_MST_DLY_LENGTH, buffer);
  return buffer[0];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::GetSlave4MasterDelay(XBYTE delay)
* @brief      Get slave4 master delay
* @ingroup    DATAIO
* 
* @param[in]  delay : Delay value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::GetSlave4MasterDelay(XBYTE delay)
{
  WriteBits(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV4_CTRL, DIOI2CMPU9150_I2C_SLV4_MST_DLY_BIT, DIOI2CMPU9150_I2C_SLV4_MST_DLY_LENGTH, delay);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOI2C9AXISTRACKINGMPU9150::GetSlate4InputByte()
* @brief      Get slate4 input byte
* @ingroup    DATAIO
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetSlate4InputByte()
{
  ReadByte(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV4_DI, buffer);
  return buffer[0];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetPassthroughStatus()
* @brief      Get passthrough status
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetPassthroughStatus()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_I2C_MST_STATUS, DIOI2CMPU9150_MST_PASS_THROUGH_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetSlave4IsDone()
* @brief      Get slave4 is done
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetSlave4IsDone()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_I2C_MST_STATUS, DIOI2CMPU9150_MST_I2C_SLV4_DONE_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetLostArbitration()
* @brief      Get lost arbitration
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetLostArbitration()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_I2C_MST_STATUS, DIOI2CMPU9150_MST_I2C_LOST_ARB_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetSlave4Nack()
* @brief      Get slave4 nack
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetSlave4Nack()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_I2C_MST_STATUS, DIOI2CMPU9150_MST_I2C_SLV4_NACK_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetSlave3Nack()
* @brief      Get slave3 nack
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetSlave3Nack()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_I2C_MST_STATUS, DIOI2CMPU9150_MST_I2C_SLV3_NACK_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetSlave2Nack()
* @brief      Get slave2 nack
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetSlave2Nack()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_I2C_MST_STATUS, DIOI2CMPU9150_MST_I2C_SLV2_NACK_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetSlave1Nack()
* @brief      Get slave1 nack
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetSlave1Nack()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_I2C_MST_STATUS, DIOI2CMPU9150_MST_I2C_SLV1_NACK_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetSlave0Nack()
* @brief      Get slave0 nack
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetSlave0Nack()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_I2C_MST_STATUS, DIOI2CMPU9150_MST_I2C_SLV0_NACK_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetInterruptMode()
* @brief      Get interrupt mode
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetInterruptMode()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_PIN_CFG, DIOI2CMPU9150_INTCFG_INT_LEVEL_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetInterruptMode(bool mode)
* @brief      Set interrupt mode
* @ingroup    DATAIO
* 
* @param[in]  mode : Mode value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetInterruptMode(bool mode)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_INT_PIN_CFG, DIOI2CMPU9150_INTCFG_INT_LEVEL_BIT, mode);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetInterruptDrive()
* @brief      Get interrupt drive
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetInterruptDrive()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_PIN_CFG, DIOI2CMPU9150_INTCFG_INT_OPEN_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetInterruptDrive(bool drive)
* @brief      Set interrupt drive
* @ingroup    DATAIO
* 
* @param[in]  drive : Drive value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetInterruptDrive(bool drive)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_INT_PIN_CFG, DIOI2CMPU9150_INTCFG_INT_OPEN_BIT, drive);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetInterruptLatch()
* @brief      Get interrupt latch
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetInterruptLatch()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_PIN_CFG, DIOI2CMPU9150_INTCFG_LATCH_INT_EN_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetInterruptLatch(bool latch)
* @brief      Set interrupt latch
* @ingroup    DATAIO
* 
* @param[in]  latch : Latch value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetInterruptLatch(bool latch)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_INT_PIN_CFG, DIOI2CMPU9150_INTCFG_LATCH_INT_EN_BIT, latch);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetInterruptLatchClear()
* @brief      Get interrupt latch clear
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetInterruptLatchClear()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_PIN_CFG, DIOI2CMPU9150_INTCFG_INT_RD_CLEAR_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetInterruptLatchClear(bool clear)
* @brief      Set interrupt latch clear
* @ingroup    DATAIO
* 
* @param[in]  clear : Clear value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetInterruptLatchClear(bool clear)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_INT_PIN_CFG, DIOI2CMPU9150_INTCFG_INT_RD_CLEAR_BIT, clear);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetFSyncInterruptLevel()
* @brief      Get f sync interrupt level
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetFSyncInterruptLevel()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_PIN_CFG, DIOI2CMPU9150_INTCFG_FSYNC_INT_LEVEL_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetFSyncInterruptLevel(bool level)
* @brief      Set f sync interrupt level
* @ingroup    DATAIO
* 
* @param[in]  level : Level value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetFSyncInterruptLevel(bool level)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_INT_PIN_CFG, DIOI2CMPU9150_INTCFG_FSYNC_INT_LEVEL_BIT, level);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetFSyncInterruptEnabled()
* @brief      Get f sync interrupt enabled
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetFSyncInterruptEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_PIN_CFG, DIOI2CMPU9150_INTCFG_FSYNC_INT_EN_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetFSyncInterruptEnabled(bool enabled)
* @brief      Set f sync interrupt enabled
* @ingroup    DATAIO
* 
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetFSyncInterruptEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_INT_PIN_CFG, DIOI2CMPU9150_INTCFG_FSYNC_INT_EN_BIT, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetI2CBypassEnabled()
* @brief      Get i2 c bypass enabled
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetI2CBypassEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_PIN_CFG, DIOI2CMPU9150_INTCFG_I2C_BYPASS_EN_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetI2CBypassEnabled(bool enabled)
* @brief      Set i2 c bypass enabled
* @ingroup    DATAIO
* 
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetI2CBypassEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_INT_PIN_CFG, DIOI2CMPU9150_INTCFG_I2C_BYPASS_EN_BIT, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetClockOutputEnabled()
* @brief      Get clock output enabled
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetClockOutputEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_PIN_CFG, DIOI2CMPU9150_INTCFG_CLKOUT_EN_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetClockOutputEnabled(bool enabled)
* @brief      Set clock output enabled
* @ingroup    DATAIO
* 
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetClockOutputEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_INT_PIN_CFG, DIOI2CMPU9150_INTCFG_CLKOUT_EN_BIT, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOI2C9AXISTRACKINGMPU9150::GetIntEnabled()
* @brief      Get int enabled
* @ingroup    DATAIO
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetIntEnabled()
{
  ReadByte(deviceaddr, DIOI2CMPU9150_RA_INT_ENABLE, buffer);
  return buffer[0];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetIntEnabled(XBYTE enabled)
* @brief      Set int enabled
* @ingroup    DATAIO
* 
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetIntEnabled(XBYTE enabled)
{
  WriteByte(deviceaddr, DIOI2CMPU9150_RA_INT_ENABLE, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetIntFreefallEnabled()
* @brief      Get int freefall enabled
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetIntFreefallEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_ENABLE, DIOI2CMPU9150_INTERRUPT_FF_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetIntFreefallEnabled(bool enabled)
* @brief      Set int freefall enabled
* @ingroup    DATAIO
* 
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetIntFreefallEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_INT_ENABLE, DIOI2CMPU9150_INTERRUPT_FF_BIT, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetIntMotionEnabled()
* @brief      Get int motion enabled
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetIntMotionEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_ENABLE, DIOI2CMPU9150_INTERRUPT_MOT_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetIntMotionEnabled(bool enabled)
* @brief      Set int motion enabled
* @ingroup    DATAIO
* 
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetIntMotionEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_INT_ENABLE, DIOI2CMPU9150_INTERRUPT_MOT_BIT, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetIntZeroMotionEnabled()
* @brief      Get int zero motion enabled
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetIntZeroMotionEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_ENABLE, DIOI2CMPU9150_INTERRUPT_ZMOT_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetIntZeroMotionEnabled(bool enabled)
* @brief      Set int zero motion enabled
* @ingroup    DATAIO
* 
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetIntZeroMotionEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_INT_ENABLE, DIOI2CMPU9150_INTERRUPT_ZMOT_BIT, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetIntFIFOBufferOverflowEnabled()
* @brief      Get int fifo buffer overflow enabled
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetIntFIFOBufferOverflowEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_ENABLE, DIOI2CMPU9150_INTERRUPT_FIFO_OFLOW_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetIntFIFOBufferOverflowEnabled(bool enabled)
* @brief      Set int fifo buffer overflow enabled
* @ingroup    DATAIO
* 
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetIntFIFOBufferOverflowEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_INT_ENABLE, DIOI2CMPU9150_INTERRUPT_FIFO_OFLOW_BIT, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetIntI2CMasterEnabled()
* @brief      Get int i2 c master enabled
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetIntI2CMasterEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_ENABLE, DIOI2CMPU9150_INTERRUPT_I2C_MST_INT_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetIntI2CMasterEnabled(bool enabled)
* @brief      Set int i2 c master enabled
* @ingroup    DATAIO
* 
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetIntI2CMasterEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_INT_ENABLE, DIOI2CMPU9150_INTERRUPT_I2C_MST_INT_BIT, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetIntDataReadyEnabled()
* @brief      Get int data ready enabled
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetIntDataReadyEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_ENABLE, DIOI2CMPU9150_INTERRUPT_DATA_RDY_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetIntDataReadyEnabled(bool enabled)
* @brief      Set int data ready enabled
* @ingroup    DATAIO
* 
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetIntDataReadyEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_INT_ENABLE, DIOI2CMPU9150_INTERRUPT_DATA_RDY_BIT, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOI2C9AXISTRACKINGMPU9150::GetIntStatus()
* @brief      Get int status
* @ingroup    DATAIO
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetIntStatus()
{
  ReadByte(deviceaddr, DIOI2CMPU9150_RA_INT_STATUS, buffer);
  return buffer[0];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetIntFreefallStatus()
* @brief      Get int freefall status
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetIntFreefallStatus()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_STATUS, DIOI2CMPU9150_INTERRUPT_FF_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetIntMotionStatus()
* @brief      Get int motion status
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetIntMotionStatus()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_STATUS, DIOI2CMPU9150_INTERRUPT_MOT_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetIntZeroMotionStatus()
* @brief      Get int zero motion status
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetIntZeroMotionStatus()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_STATUS, DIOI2CMPU9150_INTERRUPT_ZMOT_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetIntFIFOBufferOverflowStatus()
* @brief      Get int fifo buffer overflow status
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetIntFIFOBufferOverflowStatus()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_STATUS, DIOI2CMPU9150_INTERRUPT_FIFO_OFLOW_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetIntI2CMasterStatus()
* @brief      Get int i2 c master status
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetIntI2CMasterStatus()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_STATUS, DIOI2CMPU9150_INTERRUPT_I2C_MST_INT_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetIntDataReadyStatus()
* @brief      Get int data ready status
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetIntDataReadyStatus()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_STATUS, DIOI2CMPU9150_INTERRUPT_DATA_RDY_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::GetMotion9(XWORDSIG* ax, XWORDSIG* ay, XWORDSIG* az, XWORDSIG* gx, XWORDSIG* gy, XWORDSIG* gz, XWORDSIG* mx, XWORDSIG* my, XWORDSIG* mz)
* @brief      Get motion9
* @ingroup    DATAIO
* 
* @param[in]  ax : Ax pointer to use.
* @param[in]  ay : Ay pointer to use.
* @param[in]  az : Az pointer to use.
* @param[in]  gx : Gx pointer to use.
* @param[in]  gy : Gy pointer to use.
* @param[in]  gz : Gz pointer to use.
* @param[in]  mx : Mx pointer to use.
* @param[in]  my : My pointer to use.
* @param[in]  mz : Mz pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::GetMotion9(XWORDSIG* ax, XWORDSIG* ay, XWORDSIG* az, XWORDSIG* gx, XWORDSIG* gy, XWORDSIG* gz, XWORDSIG* mx, XWORDSIG* my, XWORDSIG* mz)
{
  //get accel and gyro
  GetMotion6(ax, ay, az, gx, gy, gz);

  //read mag
  WriteByte(deviceaddr, DIOI2CMPU9150_RA_INT_PIN_CFG, 0x02); //set i2c bypass enable pin to true to access magnetometer
  GEN_XSLEEP.MilliSeconds(10);

  WriteByte(DIOI2CMPU9150_RA_MAG_ADDRESS, 0x0A, 0x01); //enable the magnetometer
  GEN_XSLEEP.MilliSeconds(10);

  ReadBytes(DIOI2CMPU9150_RA_MAG_ADDRESS, DIOI2CMPU9150_RA_MAG_XOUT_L, 6, buffer);
  *mx = (((XWORDSIG)buffer[1]) << 8) | buffer[0];
  *my = (((XWORDSIG)buffer[3]) << 8) | buffer[2];
  *mz = (((XWORDSIG)buffer[5]) << 8) | buffer[4];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::GetMag(XWORDSIG* mx, XWORDSIG* my, XWORDSIG* mz)
* @brief      Get mag
* @ingroup    DATAIO
* 
* @param[in]  mx : Mx pointer to use.
* @param[in]  my : My pointer to use.
* @param[in]  mz : Mz pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::GetMag(XWORDSIG* mx, XWORDSIG* my, XWORDSIG* mz)
{
  //read mag
  WriteByte(deviceaddr, DIOI2CMPU9150_RA_INT_PIN_CFG, 0x02); //set i2c bypass enable pin to true to access magnetometer
  GEN_XSLEEP.MilliSeconds(10);

  WriteByte(DIOI2CMPU9150_RA_MAG_ADDRESS, 0x0A, 0x01); //enable the magnetometer
  GEN_XSLEEP.MilliSeconds(10);

  ReadBytes(DIOI2CMPU9150_RA_MAG_ADDRESS, DIOI2CMPU9150_RA_MAG_XOUT_L, 6, buffer);
  *mx = (((XWORDSIG)buffer[1]) << 8) | buffer[0];
  *my = (((XWORDSIG)buffer[3]) << 8) | buffer[2];
  *mz = (((XWORDSIG)buffer[5]) << 8) | buffer[4];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::GetMotion6(XWORDSIG* ax, XWORDSIG* ay, XWORDSIG* az, XWORDSIG* gx, XWORDSIG* gy, XWORDSIG* gz)
* @brief      Get motion6
* @ingroup    DATAIO
* 
* @param[in]  ax : Ax pointer to use.
* @param[in]  ay : Ay pointer to use.
* @param[in]  az : Az pointer to use.
* @param[in]  gx : Gx pointer to use.
* @param[in]  gy : Gy pointer to use.
* @param[in]  gz : Gz pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::GetMotion6(XWORDSIG* ax, XWORDSIG* ay, XWORDSIG* az, XWORDSIG* gx, XWORDSIG* gy, XWORDSIG* gz)
{
  ReadBytes(deviceaddr, DIOI2CMPU9150_RA_ACCEL_XOUT_H, 14, buffer);
  *ax = (((XWORDSIG)buffer[0])  << 8) | buffer[1];
  *ay = (((XWORDSIG)buffer[2])  << 8) | buffer[3];
  *az = (((XWORDSIG)buffer[4])  << 8) | buffer[5];
  *gx = (((XWORDSIG)buffer[8])  << 8) | buffer[9];
  *gy = (((XWORDSIG)buffer[10]) << 8) | buffer[11];
  *gz = (((XWORDSIG)buffer[12]) << 8) | buffer[13];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::GetAcceleration(XWORDSIG* x, XWORDSIG* y, XWORDSIG* z)
* @brief      Get acceleration
* @ingroup    DATAIO
* 
* @param[in]  x : X coordinate.
* @param[in]  y : Y coordinate.
* @param[in]  z : Z coordinate.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::GetAcceleration(XWORDSIG* x, XWORDSIG* y, XWORDSIG* z)
{
  ReadBytes(deviceaddr, DIOI2CMPU9150_RA_ACCEL_XOUT_H, 6, buffer);
  *x = (((XWORDSIG)buffer[0]) << 8) | buffer[1];
  *y = (((XWORDSIG)buffer[2]) << 8) | buffer[3];
  *z = (((XWORDSIG)buffer[4]) << 8) | buffer[5];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORDSIG DIOI2C9AXISTRACKINGMPU9150::GetAccelerationX()
* @brief      Get acceleration x
* @ingroup    DATAIO
* 
* @return     XWORDSIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORDSIG DIOI2C9AXISTRACKINGMPU9150::GetAccelerationX()
{
  ReadBytes(deviceaddr, DIOI2CMPU9150_RA_ACCEL_XOUT_H, 2, buffer);
  return (((XWORDSIG)buffer[0]) << 8) | buffer[1];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORDSIG DIOI2C9AXISTRACKINGMPU9150::GetAccelerationY()
* @brief      Get acceleration y
* @ingroup    DATAIO
* 
* @return     XWORDSIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORDSIG DIOI2C9AXISTRACKINGMPU9150::GetAccelerationY()
{
  ReadBytes(deviceaddr, DIOI2CMPU9150_RA_ACCEL_YOUT_H, 2, buffer);
  return (((XWORDSIG)buffer[0]) << 8) | buffer[1];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORDSIG DIOI2C9AXISTRACKINGMPU9150::GetAccelerationZ()
* @brief      Get acceleration z
* @ingroup    DATAIO
* 
* @return     XWORDSIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORDSIG DIOI2C9AXISTRACKINGMPU9150::GetAccelerationZ()
{
  ReadBytes(deviceaddr, DIOI2CMPU9150_RA_ACCEL_ZOUT_H, 2, buffer);
  return (((XWORDSIG)buffer[0]) << 8) | buffer[1];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORDSIG DIOI2C9AXISTRACKINGMPU9150::GetTemperature()
* @brief      Get temperature
* @ingroup    DATAIO
* 
* @return     XWORDSIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORDSIG DIOI2C9AXISTRACKINGMPU9150::GetTemperature()
{
  ReadBytes(deviceaddr, DIOI2CMPU9150_RA_TEMP_OUT_H, 2, buffer);
  return (((XWORDSIG)buffer[0]) << 8) | buffer[1];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::GetRotation(XWORDSIG* x, XWORDSIG* y, XWORDSIG* z)
* @brief      Get rotation
* @ingroup    DATAIO
* 
* @param[in]  x : X coordinate.
* @param[in]  y : Y coordinate.
* @param[in]  z : Z coordinate.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::GetRotation(XWORDSIG* x, XWORDSIG* y, XWORDSIG* z)
{
  ReadBytes(deviceaddr, DIOI2CMPU9150_RA_GYRO_XOUT_H, 6, buffer);
  *x = (((XWORDSIG)buffer[0]) << 8) | buffer[1];
  *y = (((XWORDSIG)buffer[2]) << 8) | buffer[3];
  *z = (((XWORDSIG)buffer[4]) << 8) | buffer[5];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORDSIG DIOI2C9AXISTRACKINGMPU9150::GetRotationX()
* @brief      Get rotation x
* @ingroup    DATAIO
* 
* @return     XWORDSIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORDSIG DIOI2C9AXISTRACKINGMPU9150::GetRotationX()
{
  ReadBytes(deviceaddr, DIOI2CMPU9150_RA_GYRO_XOUT_H, 2, buffer);
  return (((XWORDSIG)buffer[0]) << 8) | buffer[1];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORDSIG DIOI2C9AXISTRACKINGMPU9150::GetRotationY()
* @brief      Get rotation y
* @ingroup    DATAIO
* 
* @return     XWORDSIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORDSIG DIOI2C9AXISTRACKINGMPU9150::GetRotationY()
{
  ReadBytes(deviceaddr, DIOI2CMPU9150_RA_GYRO_YOUT_H, 2, buffer);
  return (((XWORDSIG)buffer[0]) << 8) | buffer[1];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORDSIG DIOI2C9AXISTRACKINGMPU9150::GetRotationZ()
* @brief      Get rotation z
* @ingroup    DATAIO
* 
* @return     XWORDSIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORDSIG DIOI2C9AXISTRACKINGMPU9150::GetRotationZ()
{
  ReadBytes(deviceaddr, DIOI2CMPU9150_RA_GYRO_ZOUT_H, 2, buffer);
  return (((XWORDSIG)buffer[0]) << 8) | buffer[1];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOI2C9AXISTRACKINGMPU9150::GetExternalSensorByte(int position)
* @brief      Get external sensor byte
* @ingroup    DATAIO
* 
* @param[in]  position : Position value.
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetExternalSensorByte(int position)
{
  ReadByte(deviceaddr, DIOI2CMPU9150_RA_EXT_SENS_DATA_00 + position, buffer);
  return buffer[0];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORDSIG DIOI2C9AXISTRACKINGMPU9150::GetExternalSensorWord(int position)
* @brief      Get external sensor word
* @ingroup    DATAIO
* 
* @param[in]  position : Position value.
* 
* @return     XWORDSIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORDSIG DIOI2C9AXISTRACKINGMPU9150::GetExternalSensorWord(int position)
{
  ReadBytes(deviceaddr, DIOI2CMPU9150_RA_EXT_SENS_DATA_00 + position, 2, buffer);
  return (XWORDSIG)(((XWORDSIG)buffer[0]) << 8) | buffer[1];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOI2C9AXISTRACKINGMPU9150::GetExternalSensorDWord(int position)
* @brief      Get external sensor d word
* @ingroup    DATAIO
* 
* @param[in]  position : Position value.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOI2C9AXISTRACKINGMPU9150::GetExternalSensorDWord(int position)
{
  ReadBytes(deviceaddr, DIOI2CMPU9150_RA_EXT_SENS_DATA_00 + position, 4, buffer);
  return (((XDWORD)buffer[0]) << 24) | (((XDWORD)buffer[1]) << 16) | (((XWORDSIG)buffer[2]) << 8) | buffer[3];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetXNegMotionDetected()
* @brief      Get x neg motion detected
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetXNegMotionDetected()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_MOT_DETECT_STATUS, DIOI2CMPU9150_MOTION_MOT_XNEG_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetXPosMotionDetected()
* @brief      Get x pos motion detected
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetXPosMotionDetected()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_MOT_DETECT_STATUS, DIOI2CMPU9150_MOTION_MOT_XPOS_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetYNegMotionDetected()
* @brief      Get y neg motion detected
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetYNegMotionDetected()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_MOT_DETECT_STATUS, DIOI2CMPU9150_MOTION_MOT_YNEG_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetYPosMotionDetected()
* @brief      Get y pos motion detected
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetYPosMotionDetected()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_MOT_DETECT_STATUS, DIOI2CMPU9150_MOTION_MOT_YPOS_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetZNegMotionDetected()
* @brief      Get z neg motion detected
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetZNegMotionDetected()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_MOT_DETECT_STATUS, DIOI2CMPU9150_MOTION_MOT_ZNEG_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetZPosMotionDetected()
* @brief      Get z pos motion detected
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetZPosMotionDetected()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_MOT_DETECT_STATUS, DIOI2CMPU9150_MOTION_MOT_ZPOS_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetZeroMotionDetected()
* @brief      Get zero motion detected
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetZeroMotionDetected()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_MOT_DETECT_STATUS, DIOI2CMPU9150_MOTION_MOT_ZRMOT_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetSlaveOutputByte(XBYTE num, XBYTE data)
* @brief      Set slave output byte
* @ingroup    DATAIO
* 
* @param[in]  num : Num value.
* @param[in]  data : Data buffer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetSlaveOutputByte(XBYTE num, XBYTE data)
{
  if(num > 3) return;

  WriteByte(deviceaddr, DIOI2CMPU9150_RA_I2C_SLV0_DO + num, data);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetExternalShadowDelayEnabled()
* @brief      Get external shadow delay enabled
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetExternalShadowDelayEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_I2C_MST_DELAY_CTRL, DIOI2CMPU9150_DELAYCTRL_DELAY_ES_SHADOW_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetExternalShadowDelayEnabled(bool enabled)
* @brief      Set external shadow delay enabled
* @ingroup    DATAIO
* 
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetExternalShadowDelayEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_I2C_MST_DELAY_CTRL, DIOI2CMPU9150_DELAYCTRL_DELAY_ES_SHADOW_BIT, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetSlaveDelayEnabled(XBYTE num)
* @brief      Get slave delay enabled
* @ingroup    DATAIO
* 
* @param[in]  num : Num value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetSlaveDelayEnabled(XBYTE num)
{
  // DIOI2CMPU9150_DELAYCTRL_I2C_SLV4_DLY_EN_BIT is 4, SLV3 is 3, etc.
  if(num > 4) return false;
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_I2C_MST_DELAY_CTRL, num, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetSlaveDelayEnabled(XBYTE num, bool enabled)
* @brief      Set slave delay enabled
* @ingroup    DATAIO
* 
* @param[in]  num : Num value.
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetSlaveDelayEnabled(XBYTE num, bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_I2C_MST_DELAY_CTRL, num, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::ResetGyroscopePath()
* @brief      Reset Gyroscope Path
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::ResetGyroscopePath()
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_SIGNAL_PATH_RESET, DIOI2CMPU9150_PATHRESET_GYRO_RESET_BIT, true);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::ResetAccelerometerPath()
* @brief      Reset Accelerometer Path
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::ResetAccelerometerPath()
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_SIGNAL_PATH_RESET, DIOI2CMPU9150_PATHRESET_ACCEL_RESET_BIT, true);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::ResetTemperaturePath()
* @brief      Reset Temperature Path
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::ResetTemperaturePath()
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_SIGNAL_PATH_RESET, DIOI2CMPU9150_PATHRESET_TEMP_RESET_BIT, true);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOI2C9AXISTRACKINGMPU9150::GetAccelerometerPowerOnDelay()
* @brief      Get accelerometer power on delay
* @ingroup    DATAIO
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetAccelerometerPowerOnDelay()
{
  ReadBits(deviceaddr, DIOI2CMPU9150_RA_MOT_DETECT_CTRL, DIOI2CMPU9150_DETECT_ACCEL_ON_DELAY_BIT, DIOI2CMPU9150_DETECT_ACCEL_ON_DELAY_LENGTH, buffer);
  return buffer[0];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetAccelerometerPowerOnDelay(XBYTE delay)
* @brief      Set accelerometer power on delay
* @ingroup    DATAIO
* 
* @param[in]  delay : Delay value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetAccelerometerPowerOnDelay(XBYTE delay)
{
  WriteBits(deviceaddr, DIOI2CMPU9150_RA_MOT_DETECT_CTRL, DIOI2CMPU9150_DETECT_ACCEL_ON_DELAY_BIT, DIOI2CMPU9150_DETECT_ACCEL_ON_DELAY_LENGTH, delay);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOI2C9AXISTRACKINGMPU9150::GetFreefallDetectionCounterDecrement()
* @brief      Get freefall detection counter decrement
* @ingroup    DATAIO
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetFreefallDetectionCounterDecrement()
{
  ReadBits(deviceaddr, DIOI2CMPU9150_RA_MOT_DETECT_CTRL, DIOI2CMPU9150_DETECT_FF_COUNT_BIT, DIOI2CMPU9150_DETECT_FF_COUNT_LENGTH, buffer);
  return buffer[0];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetFreefallDetectionCounterDecrement(XBYTE decrement)
* @brief      Set freefall detection counter decrement
* @ingroup    DATAIO
* 
* @param[in]  decrement : Decrement value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetFreefallDetectionCounterDecrement(XBYTE decrement)
{
  WriteBits(deviceaddr, DIOI2CMPU9150_RA_MOT_DETECT_CTRL, DIOI2CMPU9150_DETECT_FF_COUNT_BIT, DIOI2CMPU9150_DETECT_FF_COUNT_LENGTH, decrement);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOI2C9AXISTRACKINGMPU9150::GetMotionDetectionCounterDecrement()
* @brief      Get motion detection counter decrement
* @ingroup    DATAIO
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetMotionDetectionCounterDecrement()
{
  ReadBits(deviceaddr, DIOI2CMPU9150_RA_MOT_DETECT_CTRL, DIOI2CMPU9150_DETECT_MOT_COUNT_BIT, DIOI2CMPU9150_DETECT_MOT_COUNT_LENGTH, buffer);
  return buffer[0];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetMotionDetectionCounterDecrement(XBYTE decrement)
* @brief      Set motion detection counter decrement
* @ingroup    DATAIO
* 
* @param[in]  decrement : Decrement value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetMotionDetectionCounterDecrement(XBYTE decrement)
{
  WriteBits(deviceaddr, DIOI2CMPU9150_RA_MOT_DETECT_CTRL, DIOI2CMPU9150_DETECT_MOT_COUNT_BIT, DIOI2CMPU9150_DETECT_MOT_COUNT_LENGTH, decrement);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetFIFOEnabled()
* @brief      Get fifo enabled
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetFIFOEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_USER_CTRL, DIOI2CMPU9150_USERCTRL_FIFO_EN_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetFIFOEnabled(bool enabled)
* @brief      Set fifo enabled
* @ingroup    DATAIO
* 
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetFIFOEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_USER_CTRL, DIOI2CMPU9150_USERCTRL_FIFO_EN_BIT, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetI2CMasterModeEnabled()
* @brief      Get i2 c master mode enabled
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetI2CMasterModeEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_USER_CTRL, DIOI2CMPU9150_USERCTRL_I2C_MST_EN_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetI2CMasterModeEnabled(bool enabled)
* @brief      Set i2 c master mode enabled
* @ingroup    DATAIO
* 
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetI2CMasterModeEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_USER_CTRL, DIOI2CMPU9150_USERCTRL_I2C_MST_EN_BIT, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SwitchSPIEnabled(bool enabled)
* @brief      Switch SPI Enabled
* @ingroup    DATAIO
* 
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SwitchSPIEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_USER_CTRL, DIOI2CMPU9150_USERCTRL_I2C_IF_DIS_BIT, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::ResetFIFO()
* @brief      Reset FIFO
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::ResetFIFO()
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_USER_CTRL, DIOI2CMPU9150_USERCTRL_FIFO_RESET_BIT, true);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::ResetI2CMaster()
* @brief      Reset I2 C Master
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::ResetI2CMaster()
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_USER_CTRL, DIOI2CMPU9150_USERCTRL_I2C_MST_RESET_BIT, true);
}



/**
 *
 *
 * When resetting only the signal path (and not the sensor registers), please
 * use Register 104, SIGNAL_PATH_RESET.
 *
 * @see DIOI2CMPU9150_RA_USER_CTRL
 * @see DIOI2CMPU9150_USERCTRL_SIG_COND_RESET_BIT
 */

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::ResetSensors()
* @brief      Reset Sensors
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::ResetSensors()
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_USER_CTRL, DIOI2CMPU9150_USERCTRL_SIG_COND_RESET_BIT, true);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::Reset()
* @brief      Reset state
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::Reset()
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_1, DIOI2CMPU9150_PWR1_DEVICE_RESET_BIT, true);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetSleepEnabled()
* @brief      Get sleep enabled
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetSleepEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_1, DIOI2CMPU9150_PWR1_SLEEP_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetSleepEnabled(bool enabled)
* @brief      Set sleep enabled
* @ingroup    DATAIO
* 
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetSleepEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_1, DIOI2CMPU9150_PWR1_SLEEP_BIT, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetWakeCycleEnabled()
* @brief      Get wake cycle enabled
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetWakeCycleEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_1, DIOI2CMPU9150_PWR1_CYCLE_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetWakeCycleEnabled(bool enabled)
* @brief      Set wake cycle enabled
* @ingroup    DATAIO
* 
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetWakeCycleEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_1, DIOI2CMPU9150_PWR1_CYCLE_BIT, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetTempSensorEnabled()
* @brief      Get temp sensor enabled
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetTempSensorEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_1, DIOI2CMPU9150_PWR1_TEMP_DIS_BIT, buffer);
  return (buffer[0] == 0)?true:false; // 1 is actually disabled here
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetTempSensorEnabled(bool enabled)
* @brief      Set temp sensor enabled
* @ingroup    DATAIO
* 
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetTempSensorEnabled(bool enabled)
{
  // 1 is actually disabled here
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_1, DIOI2CMPU9150_PWR1_TEMP_DIS_BIT, !enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOI2C9AXISTRACKINGMPU9150::GetClockSource()
* @brief      Get clock source
* @ingroup    DATAIO
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetClockSource()
{
  ReadBits(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_1, DIOI2CMPU9150_PWR1_CLKSEL_BIT, DIOI2CMPU9150_PWR1_CLKSEL_LENGTH, buffer);
  return buffer[0];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetClockSource(XBYTE source)
* @brief      Set clock source
* @ingroup    DATAIO
* 
* @param[in]  source : Source value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetClockSource(XBYTE source)
{
  WriteBits(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_1, DIOI2CMPU9150_PWR1_CLKSEL_BIT, DIOI2CMPU9150_PWR1_CLKSEL_LENGTH, source);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOI2C9AXISTRACKINGMPU9150::GetWakeFrequency()
* @brief      Get wake frequency
* @ingroup    DATAIO
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetWakeFrequency()
{
  ReadBits(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_2, DIOI2CMPU9150_PWR2_LP_WAKE_CTRL_BIT, DIOI2CMPU9150_PWR2_LP_WAKE_CTRL_LENGTH, buffer);
  return buffer[0];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetWakeFrequency(XBYTE frequency)
* @brief      Set wake frequency
* @ingroup    DATAIO
* 
* @param[in]  frequency : Frequency value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetWakeFrequency(XBYTE frequency)
{
  WriteBits(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_2, DIOI2CMPU9150_PWR2_LP_WAKE_CTRL_BIT, DIOI2CMPU9150_PWR2_LP_WAKE_CTRL_LENGTH, frequency);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetStandbyXAccelEnabled()
* @brief      Get standby x accel enabled
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetStandbyXAccelEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_2, DIOI2CMPU9150_PWR2_STBY_XA_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetStandbyXAccelEnabled(bool enabled)
* @brief      Set standby x accel enabled
* @ingroup    DATAIO
* 
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetStandbyXAccelEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_2, DIOI2CMPU9150_PWR2_STBY_XA_BIT, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetStandbyYAccelEnabled()
* @brief      Get standby y accel enabled
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetStandbyYAccelEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_2, DIOI2CMPU9150_PWR2_STBY_YA_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetStandbyYAccelEnabled(bool enabled)
* @brief      Set standby y accel enabled
* @ingroup    DATAIO
* 
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetStandbyYAccelEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_2, DIOI2CMPU9150_PWR2_STBY_YA_BIT, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetStandbyZAccelEnabled()
* @brief      Get standby z accel enabled
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetStandbyZAccelEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_2, DIOI2CMPU9150_PWR2_STBY_ZA_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetStandbyZAccelEnabled(bool enabled)
* @brief      Set standby z accel enabled
* @ingroup    DATAIO
* 
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetStandbyZAccelEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_2, DIOI2CMPU9150_PWR2_STBY_ZA_BIT, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetStandbyXGyroEnabled()
* @brief      Get standby x gyro enabled
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetStandbyXGyroEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_2, DIOI2CMPU9150_PWR2_STBY_XG_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetStandbyXGyroEnabled(bool enabled)
* @brief      Set standby x gyro enabled
* @ingroup    DATAIO
* 
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetStandbyXGyroEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_2, DIOI2CMPU9150_PWR2_STBY_XG_BIT, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetStandbyYGyroEnabled()
* @brief      Get standby y gyro enabled
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetStandbyYGyroEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_2, DIOI2CMPU9150_PWR2_STBY_YG_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetStandbyYGyroEnabled(bool enabled)
* @brief      Set standby y gyro enabled
* @ingroup    DATAIO
* 
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetStandbyYGyroEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_2, DIOI2CMPU9150_PWR2_STBY_YG_BIT, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetStandbyZGyroEnabled()
* @brief      Get standby z gyro enabled
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetStandbyZGyroEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_2, DIOI2CMPU9150_PWR2_STBY_ZG_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetStandbyZGyroEnabled(bool enabled)
* @brief      Set standby z gyro enabled
* @ingroup    DATAIO
* 
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetStandbyZGyroEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_PWR_MGMT_2, DIOI2CMPU9150_PWR2_STBY_ZG_BIT, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORDSIG DIOI2C9AXISTRACKINGMPU9150::GetFIFOCount()
* @brief      Get fifo count
* @ingroup    DATAIO
* 
* @return     XWORDSIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORDSIG DIOI2C9AXISTRACKINGMPU9150::GetFIFOCount()
{
  ReadBytes(deviceaddr, DIOI2CMPU9150_RA_FIFO_COUNTH, 2, buffer);
  return (((XWORDSIG)buffer[0]) << 8) | buffer[1];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOI2C9AXISTRACKINGMPU9150::GetFIFOByte()
* @brief      Get fifo byte
* @ingroup    DATAIO
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetFIFOByte()
{
  ReadByte(deviceaddr, DIOI2CMPU9150_RA_FIFO_R_W, buffer);
  return buffer[0];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::GetFIFOBytes(XBYTE* data, XBYTE length)
* @brief      Get FIFO bytes
* @ingroup    DATAIO
* 
* @param[in]  data : Data buffer to use.
* @param[in]  length : Length value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::GetFIFOBytes(XBYTE* data, XBYTE length)
{
  ReadBytes(deviceaddr, DIOI2CMPU9150_RA_FIFO_R_W, length, data);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetFIFOByte(XBYTE data)
* @brief      Set FIFO byte
* @ingroup    DATAIO
* 
* @param[in]  data : Data buffer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetFIFOByte(XBYTE data)
{
  WriteByte(deviceaddr, DIOI2CMPU9150_RA_FIFO_R_W, data);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOI2C9AXISTRACKINGMPU9150::GetDeviceID()
* @brief      Get device ID
* @ingroup    DATAIO
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetDeviceID()
{
  ReadBits(deviceaddr, DIOI2CMPU9150_RA_WHO_AM_I, DIOI2CMPU9150_WHO_AM_I_BIT, DIOI2CMPU9150_WHO_AM_I_LENGTH, buffer);
  return buffer[0];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOI2C9AXISTRACKINGMPU9150::GetOTPBankValid()
* @brief      Get OTP bank valid
* @ingroup    DATAIO
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetOTPBankValid()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_XG_OFFS_TC, DIOI2CMPU9150_TC_OTP_BNK_VLD_BIT, buffer);
  return buffer[0];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetOTPBankValid(bool enabled)
* @brief      Set OTP bank valid
* @ingroup    DATAIO
* 
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetOTPBankValid(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_XG_OFFS_TC, DIOI2CMPU9150_TC_OTP_BNK_VLD_BIT, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C9AXISTRACKINGMPU9150::GetXGyroOffset()
* @brief      Get X gyro offset
* @ingroup    DATAIO
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C9AXISTRACKINGMPU9150::GetXGyroOffset()
{
  ReadBits(deviceaddr, DIOI2CMPU9150_RA_XG_OFFS_TC, DIOI2CMPU9150_TC_OFFSET_BIT, DIOI2CMPU9150_TC_OFFSET_LENGTH, buffer);
  return buffer[0];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetXGyroOffset(XBYTESIG offset)
* @brief      Set X gyro offset
* @ingroup    DATAIO
* 
* @param[in]  offset : Offset value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetXGyroOffset(XBYTESIG offset)
{
  WriteBits(deviceaddr, DIOI2CMPU9150_RA_XG_OFFS_TC, DIOI2CMPU9150_TC_OFFSET_BIT, DIOI2CMPU9150_TC_OFFSET_LENGTH, offset);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C9AXISTRACKINGMPU9150::GetYGyroOffset()
* @brief      Get Y gyro offset
* @ingroup    DATAIO
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C9AXISTRACKINGMPU9150::GetYGyroOffset()
{
  ReadBits(deviceaddr, DIOI2CMPU9150_RA_YG_OFFS_TC, DIOI2CMPU9150_TC_OFFSET_BIT, DIOI2CMPU9150_TC_OFFSET_LENGTH, buffer);
  return buffer[0];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetYGyroOffset(XBYTESIG offset)
* @brief      Set Y gyro offset
* @ingroup    DATAIO
* 
* @param[in]  offset : Offset value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetYGyroOffset(XBYTESIG offset)
{
  WriteBits(deviceaddr, DIOI2CMPU9150_RA_YG_OFFS_TC, DIOI2CMPU9150_TC_OFFSET_BIT, DIOI2CMPU9150_TC_OFFSET_LENGTH, offset);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C9AXISTRACKINGMPU9150::GetZGyroOffset()
* @brief      Get Z gyro offset
* @ingroup    DATAIO
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C9AXISTRACKINGMPU9150::GetZGyroOffset()
{
  ReadBits(deviceaddr, DIOI2CMPU9150_RA_ZG_OFFS_TC, DIOI2CMPU9150_TC_OFFSET_BIT, DIOI2CMPU9150_TC_OFFSET_LENGTH, buffer);
  return buffer[0];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetZGyroOffset(XBYTESIG offset)
* @brief      Set Z gyro offset
* @ingroup    DATAIO
* 
* @param[in]  offset : Offset value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetZGyroOffset(XBYTESIG offset)
{
  WriteBits(deviceaddr, DIOI2CMPU9150_RA_ZG_OFFS_TC, DIOI2CMPU9150_TC_OFFSET_BIT, DIOI2CMPU9150_TC_OFFSET_LENGTH, offset);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C9AXISTRACKINGMPU9150::GetXFineGain()
* @brief      Get X fine gain
* @ingroup    DATAIO
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C9AXISTRACKINGMPU9150::GetXFineGain()
{
  ReadByte(deviceaddr, DIOI2CMPU9150_RA_X_FINE_GAIN, buffer);
  return buffer[0];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetXFineGain(XBYTESIG gain)
* @brief      Set X fine gain
* @ingroup    DATAIO
* 
* @param[in]  gain : Gain value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetXFineGain(XBYTESIG gain)
{
  WriteByte(deviceaddr, DIOI2CMPU9150_RA_X_FINE_GAIN, gain);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C9AXISTRACKINGMPU9150::GetYFineGain()
* @brief      Get Y fine gain
* @ingroup    DATAIO
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C9AXISTRACKINGMPU9150::GetYFineGain()
{
  ReadByte(deviceaddr, DIOI2CMPU9150_RA_Y_FINE_GAIN, buffer);
  return buffer[0];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::GetYFineGain(XBYTESIG gain)
* @brief      Get Y fine gain
* @ingroup    DATAIO
* 
* @param[in]  gain : Gain value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::GetYFineGain(XBYTESIG gain)
{
  WriteByte(deviceaddr, DIOI2CMPU9150_RA_Y_FINE_GAIN, gain);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C9AXISTRACKINGMPU9150::GetZFineGain()
* @brief      Get Z fine gain
* @ingroup    DATAIO
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C9AXISTRACKINGMPU9150::GetZFineGain()
{
  ReadByte(deviceaddr, DIOI2CMPU9150_RA_Z_FINE_GAIN, buffer);
  return buffer[0];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetZFineGain(XBYTESIG gain)
* @brief      Set Z fine gain
* @ingroup    DATAIO
* 
* @param[in]  gain : Gain value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetZFineGain(XBYTESIG gain)
{
  WriteByte(deviceaddr, DIOI2CMPU9150_RA_Z_FINE_GAIN, gain);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORDSIG DIOI2C9AXISTRACKINGMPU9150::GetXAccelOffset()
* @brief      Get X accel offset
* @ingroup    DATAIO
* 
* @return     XWORDSIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORDSIG DIOI2C9AXISTRACKINGMPU9150::GetXAccelOffset()
{
  ReadBytes(deviceaddr, DIOI2CMPU9150_RA_XA_OFFS_H, 2, buffer);
  return (((XWORDSIG)buffer[0]) << 8) | buffer[1];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetXAccelOffset(XWORDSIG offset)
* @brief      Set X accel offset
* @ingroup    DATAIO
* 
* @param[in]  offset : Offset value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetXAccelOffset(XWORDSIG offset)
{
  WriteWord(deviceaddr, DIOI2CMPU9150_RA_XA_OFFS_H, offset);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORDSIG DIOI2C9AXISTRACKINGMPU9150::GetYAccelOffset()
* @brief      Get Y accel offset
* @ingroup    DATAIO
* 
* @return     XWORDSIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORDSIG DIOI2C9AXISTRACKINGMPU9150::GetYAccelOffset()
{
  ReadBytes(deviceaddr, DIOI2CMPU9150_RA_YA_OFFS_H, 2, buffer);
  return (((XWORDSIG)buffer[0]) << 8) | buffer[1];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetYAccelOffset(XWORDSIG offset)
* @brief      Set Y accel offset
* @ingroup    DATAIO
* 
* @param[in]  offset : Offset value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetYAccelOffset(XWORDSIG offset)
{
  WriteWord(deviceaddr, DIOI2CMPU9150_RA_YA_OFFS_H, offset);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORDSIG DIOI2C9AXISTRACKINGMPU9150::GetZAccelOffset()
* @brief      Get Z accel offset
* @ingroup    DATAIO
* 
* @return     XWORDSIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORDSIG DIOI2C9AXISTRACKINGMPU9150::GetZAccelOffset()
{
  ReadBytes(deviceaddr, DIOI2CMPU9150_RA_ZA_OFFS_H, 2, buffer);
  return (((XWORDSIG)buffer[0]) << 8) | buffer[1];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetZAccelOffset(XWORDSIG offset)
* @brief      Set Z accel offset
* @ingroup    DATAIO
* 
* @param[in]  offset : Offset value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetZAccelOffset(XWORDSIG offset)
{
  WriteWord(deviceaddr, DIOI2CMPU9150_RA_ZA_OFFS_H, offset);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORDSIG DIOI2C9AXISTRACKINGMPU9150::GetXGyroOffsetUser()
* @brief      Get X gyro offset user
* @ingroup    DATAIO
* 
* @return     XWORDSIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORDSIG DIOI2C9AXISTRACKINGMPU9150::GetXGyroOffsetUser()
{
  ReadBytes(deviceaddr, DIOI2CMPU9150_RA_XG_OFFS_USRH, 2, buffer);
  return (((XWORDSIG)buffer[0]) << 8) | buffer[1];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetXGyroOffsetUser(XWORDSIG offset)
* @brief      Set X gyro offset user
* @ingroup    DATAIO
* 
* @param[in]  offset : Offset value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetXGyroOffsetUser(XWORDSIG offset)
{
  WriteWord(deviceaddr, DIOI2CMPU9150_RA_XG_OFFS_USRH, offset);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORDSIG DIOI2C9AXISTRACKINGMPU9150::GetYGyroOffsetUser()
* @brief      Get Y gyro offset user
* @ingroup    DATAIO
* 
* @return     XWORDSIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORDSIG DIOI2C9AXISTRACKINGMPU9150::GetYGyroOffsetUser()
{
  ReadBytes(deviceaddr, DIOI2CMPU9150_RA_YG_OFFS_USRH, 2, buffer);
  return (((XWORDSIG)buffer[0]) << 8) | buffer[1];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetYGyroOffsetUser(XWORDSIG offset)
* @brief      Set Y gyro offset user
* @ingroup    DATAIO
* 
* @param[in]  offset : Offset value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetYGyroOffsetUser(XWORDSIG offset)
{
  WriteWord(deviceaddr, DIOI2CMPU9150_RA_YG_OFFS_USRH, offset);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORDSIG DIOI2C9AXISTRACKINGMPU9150::GetZGyroOffsetUser()
* @brief      Get Z gyro offset user
* @ingroup    DATAIO
* 
* @return     XWORDSIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORDSIG DIOI2C9AXISTRACKINGMPU9150::GetZGyroOffsetUser()
{
  ReadBytes(deviceaddr, DIOI2CMPU9150_RA_ZG_OFFS_USRH, 2, buffer);
  return (((XWORDSIG)buffer[0]) << 8) | buffer[1];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetZGyroOffsetUser(XWORDSIG offset)
* @brief      Set Z gyro offset user
* @ingroup    DATAIO
* 
* @param[in]  offset : Offset value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetZGyroOffsetUser(XWORDSIG offset)
{
  WriteWord(deviceaddr, DIOI2CMPU9150_RA_ZG_OFFS_USRH, offset);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetIntPLLReadyEnabled()
* @brief      Get int PLL ready enabled
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetIntPLLReadyEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_ENABLE, DIOI2CMPU9150_INTERRUPT_PLL_RDY_INT_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetIntPLLReadyEnabled(bool enabled)
* @brief      Set int PLL ready enabled
* @ingroup    DATAIO
* 
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetIntPLLReadyEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_INT_ENABLE, DIOI2CMPU9150_INTERRUPT_PLL_RDY_INT_BIT, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetIntDMPEnabled()
* @brief      Get int DMP enabled
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetIntDMPEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_ENABLE, DIOI2CMPU9150_INTERRUPT_DMP_INT_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetIntDMPEnabled(bool enabled)
* @brief      Set int DMP enabled
* @ingroup    DATAIO
* 
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetIntDMPEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_INT_ENABLE, DIOI2CMPU9150_INTERRUPT_DMP_INT_BIT, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetDMPInt5Status()
* @brief      Get DMP int5 status
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetDMPInt5Status()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_DMP_INT_STATUS, DIOI2CMPU9150_DMPINT_5_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetDMPInt4Status()
* @brief      Get DMP int4 status
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetDMPInt4Status()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_DMP_INT_STATUS, DIOI2CMPU9150_DMPINT_4_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetDMPInt3Status()
* @brief      Get DMP int3 status
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetDMPInt3Status()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_DMP_INT_STATUS, DIOI2CMPU9150_DMPINT_3_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetDMPInt2Status()
* @brief      Get DMP int2 status
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetDMPInt2Status()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_DMP_INT_STATUS, DIOI2CMPU9150_DMPINT_2_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetDMPInt1Status()
* @brief      Get DMP int1 status
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetDMPInt1Status()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_DMP_INT_STATUS, DIOI2CMPU9150_DMPINT_1_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetDMPInt0Status()
* @brief      Get DMP int0 status
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetDMPInt0Status()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_DMP_INT_STATUS, DIOI2CMPU9150_DMPINT_0_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetIntPLLReadyStatus()
* @brief      Get int PLL ready status
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetIntPLLReadyStatus()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_STATUS, DIOI2CMPU9150_INTERRUPT_PLL_RDY_INT_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetIntDMPStatus()
* @brief      Get int DMP status
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetIntDMPStatus()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_INT_STATUS, DIOI2CMPU9150_INTERRUPT_DMP_INT_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::GetDMPEnabled()
* @brief      Get DMP enabled
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::GetDMPEnabled()
{
  ReadBit(deviceaddr, DIOI2CMPU9150_RA_USER_CTRL, DIOI2CMPU9150_USERCTRL_DMP_EN_BIT, buffer);
  return buffer[0]?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetDMPEnabled(bool enabled)
* @brief      Set DMP enabled
* @ingroup    DATAIO
* 
* @param[in]  enabled : Enabled value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetDMPEnabled(bool enabled)
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_USER_CTRL, DIOI2CMPU9150_USERCTRL_DMP_EN_BIT, enabled);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::ResetDMP()
* @brief      Reset DMP
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::ResetDMP()
{
  WriteBit(deviceaddr, DIOI2CMPU9150_RA_USER_CTRL, DIOI2CMPU9150_USERCTRL_DMP_RESET_BIT, true);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetMemoryBank(XBYTE bank, bool prefetchEnabled, bool userBank)
* @brief      Set memory bank
* @ingroup    DATAIO
* 
* @param[in]  bank : Bank value.
* @param[in]  prefetchEnabled : Prefetch Enabled value.
* @param[in]  userBank : User Bank value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetMemoryBank(XBYTE bank, bool prefetchEnabled, bool userBank)
{
  bank &= 0x1F;

  if(userBank)        bank |= 0x20;
  if(prefetchEnabled) bank |= 0x40;

  WriteByte(deviceaddr, DIOI2CMPU9150_RA_BANK_SEL, bank);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetMemoryStartAddress(XBYTE address)
* @brief      Set memory start address
* @ingroup    DATAIO
* 
* @param[in]  address : Address value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetMemoryStartAddress(XBYTE address)
{
  WriteByte(deviceaddr, DIOI2CMPU9150_RA_MEM_START_ADDR, address);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOI2C9AXISTRACKINGMPU9150::ReadMemoryByte()
* @brief      Read memory byte
* @ingroup    DATAIO
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::ReadMemoryByte()
{
  ReadByte(deviceaddr, DIOI2CMPU9150_RA_MEM_R_W, buffer);
  return buffer[0];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::WriteMemoryByte(XBYTE data)
* @brief      Write memory byte
* @ingroup    DATAIO
* 
* @param[in]  data : Data buffer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::WriteMemoryByte(XBYTE data)
{
  WriteByte(deviceaddr, DIOI2CMPU9150_RA_MEM_R_W, data);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOI2C9AXISTRACKINGMPU9150::GetDMPConfig1()
* @brief      Get DMP config1
* @ingroup    DATAIO
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetDMPConfig1()
{
  ReadByte(deviceaddr, DIOI2CMPU9150_RA_DMP_CFG_1, buffer);
  return buffer[0];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetDMPConfig1(XBYTE config)
* @brief      Set DMP config1
* @ingroup    DATAIO
* 
* @param[in]  config : Configuration object to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetDMPConfig1(XBYTE config)
{
  WriteByte(deviceaddr, DIOI2CMPU9150_RA_DMP_CFG_1, config);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOI2C9AXISTRACKINGMPU9150::GetDMPConfig2()
* @brief      Get DMP config2
* @ingroup    DATAIO
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOI2C9AXISTRACKINGMPU9150::GetDMPConfig2()
{
  ReadByte(deviceaddr, DIOI2CMPU9150_RA_DMP_CFG_2, buffer);
  return buffer[0];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::SetDMPConfig2(XBYTE config)
* @brief      Set DMP config2
* @ingroup    DATAIO
* 
* @param[in]  config : Configuration object to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::SetDMPConfig2(XBYTE config)
{
  WriteByte(deviceaddr, DIOI2CMPU9150_RA_DMP_CFG_2, config);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C9AXISTRACKINGMPU9150::ReadBit(XBYTE deviceaddr, XBYTE regaddr, XBYTE bitnum, XBYTE* data, XWORD timeout)
* @brief      Read bit
* @ingroup    DATAIO
* 
* @param[in]  deviceaddr : Deviceaddr value.
* @param[in]  regaddr : Regaddr value.
* @param[in]  bitnum : Bitnum value.
* @param[in]  data : Data buffer to use.
* @param[in]  timeout : Timeout value.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C9AXISTRACKINGMPU9150::ReadBit(XBYTE deviceaddr, XBYTE regaddr, XBYTE bitnum, XBYTE* data, XWORD timeout)
{
  return 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C9AXISTRACKINGMPU9150::ReadBitW(XBYTE deviceaddr, XBYTE regaddr, XBYTE bitnum, XWORD* data, XWORD timeout)
* @brief      Read bit w
* @ingroup    DATAIO
* 
* @param[in]  deviceaddr : Deviceaddr value.
* @param[in]  regaddr : Regaddr value.
* @param[in]  bitnum : Bitnum value.
* @param[in]  data : Data buffer to use.
* @param[in]  timeout : Timeout value.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C9AXISTRACKINGMPU9150::ReadBitW(XBYTE deviceaddr, XBYTE regaddr, XBYTE bitnum, XWORD* data, XWORD timeout)
{
  return 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C9AXISTRACKINGMPU9150::ReadBits(XBYTE deviceaddr, XBYTE regaddr, XBYTE bitstart, XBYTE length, XBYTE* data, XWORD timeout)
* @brief      Read bits
* @ingroup    DATAIO
* 
* @param[in]  deviceaddr : Deviceaddr value.
* @param[in]  regaddr : Regaddr value.
* @param[in]  bitstart : Bitstart value.
* @param[in]  length : Length value.
* @param[in]  data : Data buffer to use.
* @param[in]  timeout : Timeout value.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C9AXISTRACKINGMPU9150::ReadBits(XBYTE deviceaddr, XBYTE regaddr, XBYTE bitstart, XBYTE length, XBYTE* data, XWORD timeout)
{
  return 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C9AXISTRACKINGMPU9150::ReadBitsW(XBYTE deviceaddr, XBYTE regaddr, XBYTE bitstart, XBYTE length, XWORD* data, XWORD timeout)
* @brief      Read bits w
* @ingroup    DATAIO
* 
* @param[in]  deviceaddr : Deviceaddr value.
* @param[in]  regaddr : Regaddr value.
* @param[in]  bitstart : Bitstart value.
* @param[in]  length : Length value.
* @param[in]  data : Data buffer to use.
* @param[in]  timeout : Timeout value.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C9AXISTRACKINGMPU9150::ReadBitsW(XBYTE deviceaddr, XBYTE regaddr, XBYTE bitstart, XBYTE length, XWORD* data, XWORD timeout)
{
  return 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C9AXISTRACKINGMPU9150::ReadByte(XBYTE deviceaddr, XBYTE regaddr, XBYTE* data, XWORD timeout)
* @brief      Read byte
* @ingroup    DATAIO
* 
* @param[in]  deviceaddr : Deviceaddr value.
* @param[in]  regaddr : Regaddr value.
* @param[in]  data : Data buffer to use.
* @param[in]  timeout : Timeout value.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C9AXISTRACKINGMPU9150::ReadByte(XBYTE deviceaddr, XBYTE regaddr, XBYTE* data, XWORD timeout)
{
  return 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C9AXISTRACKINGMPU9150::ReadWord(XBYTE deviceaddr, XBYTE regaddr, XWORD* data, XWORD timeout)
* @brief      Read word
* @ingroup    DATAIO
* 
* @param[in]  deviceaddr : Deviceaddr value.
* @param[in]  regaddr : Regaddr value.
* @param[in]  data : Data buffer to use.
* @param[in]  timeout : Timeout value.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C9AXISTRACKINGMPU9150::ReadWord(XBYTE deviceaddr, XBYTE regaddr, XWORD* data, XWORD timeout)
{
  return 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C9AXISTRACKINGMPU9150::ReadBytes(XBYTE deviceaddr, XBYTE regaddr, XBYTE length, XBYTE* data, XWORD timeout)
* @brief      Read bytes
* @ingroup    DATAIO
* 
* @param[in]  deviceaddr : Deviceaddr value.
* @param[in]  regaddr : Regaddr value.
* @param[in]  length : Length value.
* @param[in]  data : Data buffer to use.
* @param[in]  timeout : Timeout value.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C9AXISTRACKINGMPU9150::ReadBytes(XBYTE deviceaddr, XBYTE regaddr, XBYTE length, XBYTE* data, XWORD timeout)
{
  return 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C9AXISTRACKINGMPU9150::ReadWords(XBYTE deviceaddr, XBYTE regaddr, XBYTE length, XWORD* data, XWORD timeout)
* @brief      Read words
* @ingroup    DATAIO
* 
* @param[in]  deviceaddr : Deviceaddr value.
* @param[in]  regaddr : Regaddr value.
* @param[in]  length : Length value.
* @param[in]  data : Data buffer to use.
* @param[in]  timeout : Timeout value.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C9AXISTRACKINGMPU9150::ReadWords(XBYTE deviceaddr, XBYTE regaddr, XBYTE length, XWORD* data, XWORD timeout)
{
  return 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::WriteBit(XBYTE deviceaddr, XBYTE regaddr, XBYTE bitnum, XBYTE data)
* @brief      Write bit
* @ingroup    DATAIO
* 
* @param[in]  deviceaddr : Deviceaddr value.
* @param[in]  regaddr : Regaddr value.
* @param[in]  bitnum : Bitnum value.
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::WriteBit(XBYTE deviceaddr, XBYTE regaddr, XBYTE bitnum, XBYTE data)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::WriteBitW(XBYTE deviceaddr, XBYTE regaddr, XBYTE bitnum, XWORD data)
* @brief      Write bit w
* @ingroup    DATAIO
* 
* @param[in]  deviceaddr : Deviceaddr value.
* @param[in]  regaddr : Regaddr value.
* @param[in]  bitnum : Bitnum value.
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::WriteBitW(XBYTE deviceaddr, XBYTE regaddr, XBYTE bitnum, XWORD data)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::WriteBits(XBYTE deviceaddr, XBYTE regaddr, XBYTE bitstart, XBYTE length, XBYTE data)
* @brief      Write bits
* @ingroup    DATAIO
* 
* @param[in]  deviceaddr : Deviceaddr value.
* @param[in]  regaddr : Regaddr value.
* @param[in]  bitstart : Bitstart value.
* @param[in]  length : Length value.
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::WriteBits(XBYTE deviceaddr, XBYTE regaddr, XBYTE bitstart, XBYTE length, XBYTE data)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::WriteBitsW(XBYTE deviceaddr, XBYTE regaddr, XBYTE bitstart, XBYTE length, XWORD data)
* @brief      Write bits w
* @ingroup    DATAIO
* 
* @param[in]  deviceaddr : Deviceaddr value.
* @param[in]  regaddr : Regaddr value.
* @param[in]  bitstart : Bitstart value.
* @param[in]  length : Length value.
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::WriteBitsW(XBYTE deviceaddr, XBYTE regaddr, XBYTE bitstart, XBYTE length, XWORD data)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::WriteByte(XBYTE deviceaddr, XBYTE regaddr, XBYTE data)
* @brief      Write byte
* @ingroup    DATAIO
* 
* @param[in]  deviceaddr : Deviceaddr value.
* @param[in]  regaddr : Regaddr value.
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::WriteByte(XBYTE deviceaddr, XBYTE regaddr, XBYTE data)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::WriteWord(XBYTE deviceaddr, XBYTE regaddr, XWORD data)
* @brief      Write word
* @ingroup    DATAIO
* 
* @param[in]  deviceaddr : Deviceaddr value.
* @param[in]  regaddr : Regaddr value.
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::WriteWord(XBYTE deviceaddr, XBYTE regaddr, XWORD data)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::WriteBytes(XBYTE deviceaddr, XBYTE regaddr, XBYTE length, XBYTE* data)
* @brief      Write bytes
* @ingroup    DATAIO
* 
* @param[in]  deviceaddr : Deviceaddr value.
* @param[in]  regaddr : Regaddr value.
* @param[in]  length : Length value.
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::WriteBytes(XBYTE deviceaddr, XBYTE regaddr, XBYTE length, XBYTE* data)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C9AXISTRACKINGMPU9150::WriteWords(XBYTE deviceaddr, XBYTE regaddr, XBYTE length, XWORD* data)
* @brief      Write words
* @ingroup    DATAIO
* 
* @param[in]  deviceaddr : Deviceaddr value.
* @param[in]  regaddr : Regaddr value.
* @param[in]  length : Length value.
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C9AXISTRACKINGMPU9150::WriteWords(XBYTE deviceaddr, XBYTE regaddr, XBYTE length, XWORD* data)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C9AXISTRACKINGMPU9150::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C9AXISTRACKINGMPU9150::Clean()
{
  
}







