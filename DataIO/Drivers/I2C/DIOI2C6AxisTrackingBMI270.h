/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOI2C6AxisTrackingBMI270.h
* 
* @class      DIOI2C6AXISTRACKINGBMI270
* @brief      Data Input/Output I2C 6 AXIS Tracking BMI270 class
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

#include <stdint.h>
#include <stddef.h>

#include "XBase.h"
#include "DIODeviceI2C.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOI2C6AXISTRACKINGBMI270_ADDR1                         0x68
#define DIOI2C6AXISTRACKINGBMI270_ADDR2                         0x69


// BMI270 Chip identifier 
#define DIOI2C6AXISTRACKINGBMI270_CHIP_ID                       (XBYTE)(0x24)

// BMI270 feature input start addresses 
#define DIOI2C6AXISTRACKINGBMI270_CONFIG_ID_STRT_ADDR           (XBYTE)(0x00)
#define DIOI2C6AXISTRACKINGBMI270_MAX_BURST_LEN_STRT_ADDR       (XBYTE)(0x02)
#define DIOI2C6AXISTRACKINGBMI270_CRT_GYRO_SELF_TEST_STRT_ADDR  (XBYTE)(0x03)
#define DIOI2C6AXISTRACKINGBMI270_ABORT_STRT_ADDR               (XBYTE)(0x03)
#define DIOI2C6AXISTRACKINGBMI270_AXIS_MAP_STRT_ADDR            (XBYTE)(0x04)
#define DIOI2C6AXISTRACKINGBMI270_GYRO_SELF_OFF_STRT_ADDR       (XBYTE)(0x05)
#define DIOI2C6AXISTRACKINGBMI270_NVM_PROG_PREP_STRT_ADDR       (XBYTE)(0x05)
#define DIOI2C6AXISTRACKINGBMI270_GYRO_GAIN_UPDATE_STRT_ADDR    (XBYTE)(0x06)
#define DIOI2C6AXISTRACKINGBMI270_ANY_MOT_STRT_ADDR             (XBYTE)(0x0C)
#define DIOI2C6AXISTRACKINGBMI270_NO_MOT_STRT_ADDR              (XBYTE)(0x00)
#define DIOI2C6AXISTRACKINGBMI270_SIG_MOT_STRT_ADDR             (XBYTE)(0x04)
#define DIOI2C6AXISTRACKINGBMI270_STEP_CNT_1_STRT_ADDR          (XBYTE)(0x00)
#define DIOI2C6AXISTRACKINGBMI270_STEP_CNT_4_STRT_ADDR          (XBYTE)(0x02)
#define DIOI2C6AXISTRACKINGBMI270_WRIST_GEST_STRT_ADDR          (XBYTE)(0x06)
#define DIOI2C6AXISTRACKINGBMI270_WRIST_WEAR_WAKE_UP_STRT_ADDR  (XBYTE)(0x00)

// BMI270 feature output start addresses 
#define DIOI2C6AXISTRACKINGBMI270_STEP_CNT_OUT_STRT_ADDR        (XBYTE)(0x00)
#define DIOI2C6AXISTRACKINGBMI270_STEP_ACT_OUT_STRT_ADDR        (XBYTE)(0x04)
#define DIOI2C6AXISTRACKINGBMI270_WRIST_GEST_OUT_STRT_ADDR      (XBYTE)(0x06)
#define DIOI2C6AXISTRACKINGBMI270_GYR_USER_GAIN_OUT_STRT_ADDR   (XBYTE)(0x08)
#define DIOI2C6AXISTRACKINGBMI270_GYRO_CROSS_SENSE_STRT_ADDR    (XBYTE)(0x0C)
#define DIOI2C6AXISTRACKINGBMI270_NVM_VFRM_OUT_STRT_ADDR        (XBYTE)(0x0E)

// Defines maximum number of pages 
#define DIOI2C6AXISTRACKINGBMI270_MAX_PAGE_NUM                  (XBYTE)(8)

// Defines maximum number of feature input configurations
#define DIOI2C6AXISTRACKINGBMI270_MAX_FEAT_IN                   (XBYTE)(17)

// Defines maximum number of feature outputs 
#define DIOI2C6AXISTRACKINGBMI270_MAX_FEAT_OUT                  (XBYTE)(7)

// Mask definitions for feature interrupt status bits 
#define DIOI2C6AXISTRACKINGBMI270_SIG_MOT_STATUS_MASK           (XBYTE)(0x01)
#define DIOI2C6AXISTRACKINGBMI270_STEP_CNT_STATUS_MASK          (XBYTE)(0x02)
#define DIOI2C6AXISTRACKINGBMI270_STEP_ACT_STATUS_MASK          (XBYTE)(0x04)
#define DIOI2C6AXISTRACKINGBMI270_WRIST_WAKE_UP_STATUS_MASK     (XBYTE)(0x08)
#define DIOI2C6AXISTRACKINGBMI270_WRIST_GEST_STATUS_MASK        (XBYTE)(0x10)
#define DIOI2C6AXISTRACKINGBMI270_NO_MOT_STATUS_MASK            (XBYTE)(0x20)
#define DIOI2C6AXISTRACKINGBMI270_ANY_MOT_STATUS_MASK           (XBYTE)(0x40)

// Mask definitions for feature interrupt mapping bits 
#define DIOI2C6AXISTRACKINGBMI270_INT_SIG_MOT_MASK              (XBYTE)(0x01)
#define DIOI2C6AXISTRACKINGBMI270_INT_STEP_COUNTER_MASK         (XBYTE)(0x02)
#define DIOI2C6AXISTRACKINGBMI270_INT_STEP_DETECTOR_MASK        (XBYTE)(0x02)
#define DIOI2C6AXISTRACKINGBMI270_INT_STEP_ACT_MASK             (XBYTE)(0x04)
#define DIOI2C6AXISTRACKINGBMI270_INT_WRIST_WEAR_WAKEUP_MASK    (XBYTE)(0x08)
#define DIOI2C6AXISTRACKINGBMI270_INT_WRIST_GEST_MASK           (XBYTE)(0x10)
#define DIOI2C6AXISTRACKINGBMI270_INT_NO_MOT_MASK               (XBYTE)(0x20)
#define DIOI2C6AXISTRACKINGBMI270_INT_ANY_MOT_MASK              (XBYTE)(0x40)

// Defines maximum number of feature interrupts 
#define DIOI2C6AXISTRACKINGBMI270_MAX_INT_MAP                   (XBYTE)(8)

//  Utility macros 
#define BMI2_SET_BITS(reg_data, bitname, data)                  ((reg_data & ~(bitname##_MASK)) | \
                                                                ((data << bitname##_POS) & bitname##_MASK))

#define BMI2_GET_BITS(reg_data, bitname)                        ((reg_data & (bitname##_MASK)) >> \
                                                                (bitname##_POS))

#define BMI2_SET_BIT_POS0(reg_data, bitname, data)              ((reg_data & ~(bitname##_MASK)) | \
                                                                (data & bitname##_MASK))

#define BMI2_GET_BIT_POS0(reg_data, bitname)                    (reg_data & (bitname##_MASK))
#define BMI2_SET_BIT_VAL0(reg_data, bitname)                    (reg_data & ~(bitname##_MASK))

// For getting LSB and MSB 
#define BMI2_GET_LSB(var)                                       (XBYTE)(var & BMI2_SET_LOW_BYTE)
#define BMI2_GET_MSB(var)                                       (XBYTE)((var & BMI2_SET_HIGH_BYTE) >> 8)

#ifndef BMI2_INTF_RETURN_TYPE
#define BMI2_INTF_RETURN_TYPE                                   XBYTESIG
#endif

// For defining absolute values 
#define BMI2_ABS(a)                                             ((a) > 0 ? (a) : -(a))

// LSB and MSB mask definitions 
#define BMI2_SET_LOW_BYTE                                       (XWORD)(0x00FF)
#define BMI2_SET_HIGH_BYTE                                      (XWORD)(0xFF00)
#define BMI2_SET_LOW_NIBBLE                                     (XBYTE)(0x0F)

// For enable and disable 
#define BMI2_ENABLE                                             (XBYTE)(1)
#define BMI2_DISABLE                                            (XBYTE)(0)

// To define TRUE or FALSE 
#define BMI2_TRUE                                               (XBYTE)(1)
#define BMI2_FALSE                                              (XBYTE)(0)

// To define sensor interface success code 
#define BMI2_INTF_RET_SUCCESS                                   (XBYTESIG)(0)

// To define success code 
#define BMI2_OK                                                 (XBYTESIG)(0)

// To define error codes 
#define BMI2_E_NULL_PTR                                         (XBYTESIG)(-1)
#define BMI2_E_COM_FAIL                                         (XBYTESIG)(-2)
#define BMI2_E_DEV_NOT_FOUND                                    (XBYTESIG)(-3)
#define BMI2_E_OUT_OF_RANGE                                     (XBYTESIG)(-4)
#define BMI2_E_ACC_INVALID_CFG                                  (XBYTESIG)(-5)
#define BMI2_E_GYRO_INVALID_CFG                                 (XBYTESIG)(-6)
#define BMI2_E_ACC_GYR_INVALID_CFG                              (XBYTESIG)(-7)
#define BMI2_E_INVALID_SENSOR                                   (XBYTESIG)(-8)
#define BMI2_E_CONFIG_LOAD                                      (XBYTESIG)(-9)
#define BMI2_E_INVALID_PAGE                                     (XBYTESIG)(-10)
#define BMI2_E_INVALID_FEAT_BIT                                 (XBYTESIG)(-11)
#define BMI2_E_INVALID_INT_PIN                                  (XBYTESIG)(-12)
#define BMI2_E_SET_APS_FAIL                                     (XBYTESIG)(-13)
#define BMI2_E_AUX_INVALID_CFG                                  (XBYTESIG)(-14)
#define BMI2_E_AUX_BUSY                                         (XBYTESIG)(-15)
#define BMI2_E_SELF_TEST_FAIL                                   (XBYTESIG)(-16)
#define BMI2_E_REMAP_ERROR                                      (XBYTESIG)(-17)
#define BMI2_E_GYR_USER_GAIN_UPD_FAIL                           (XBYTESIG)(-18)
#define BMI2_E_SELF_TEST_NOT_DONE                               (XBYTESIG)(-19)
#define BMI2_E_INVALID_INPUT                                    (XBYTESIG)(-20)
#define BMI2_E_INVALID_STATUS                                   (XBYTESIG)(-21)
#define BMI2_E_CRT_ERROR                                        (XBYTESIG)(-22)
#define BMI2_E_ST_ALREADY_RUNNING                               (XBYTESIG)(-23)
#define BMI2_E_CRT_READY_FOR_DL_FAIL_ABORT                      (XBYTESIG)(-24)
#define BMI2_E_DL_ERROR                                         (XBYTESIG)(-25)
#define BMI2_E_PRECON_ERROR                                     (XBYTESIG)(-26)
#define BMI2_E_ABORT_ERROR                                      (XBYTESIG)(-27)
#define BMI2_E_GYRO_SELF_TEST_ERROR                             (XBYTESIG)(-28)
#define BMI2_E_GYRO_SELF_TEST_TIMEOUT                           (XBYTESIG)(-29)
#define BMI2_E_WRITE_CYCLE_ONGOING                              (XBYTESIG)(-30)
#define BMI2_E_WRITE_CYCLE_TIMEOUT                              (XBYTESIG)(-31)
#define BMI2_E_ST_NOT_RUNING                                    (XBYTESIG)(-32)
#define BMI2_E_DATA_RDY_INT_FAILED                              (XBYTESIG)(-33)
#define BMI2_E_INVALID_FOC_POSITION                             (XBYTESIG)(-34)

// To define warnings for FIFO activity 
#define BMI2_W_FIFO_EMPTY                                       (XBYTESIG)(1)
#define BMI2_W_PARTIAL_READ                                     (XBYTESIG)(2)

// Bit wise to define information 
#define BMI2_I_MIN_VALUE                                        (XBYTE)(1)
#define BMI2_I_MAX_VALUE                                        (XBYTE)(2)


// BMI2 register addresses 
#define BMI2_CHIP_ID_ADDR                                       (XBYTE)(0x00)
#define BMI2_STATUS_ADDR                                        (XBYTE)(0x03)
#define BMI2_AUX_X_LSB_ADDR                                     (XBYTE)(0x04)
#define BMI2_ACC_X_LSB_ADDR                                     (XBYTE)(0x0C)
#define BMI2_GYR_X_LSB_ADDR                                     (XBYTE)(0x12)
#define BMI2_EVENT_ADDR                                         (XBYTE)(0x1B)
#define BMI2_INT_STATUS_0_ADDR                                  (XBYTE)(0x1C)
#define BMI2_INT_STATUS_1_ADDR                                  (XBYTE)(0x1D)
#define BMI2_SC_OUT_0_ADDR                                      (XBYTE)(0x1E)
#define BMI2_SYNC_COMMAND_ADDR                                  (XBYTE)(0x1E)
#define BMI2_GYR_CAS_GPIO0_ADDR                                 (XBYTE)(0x1E)
#define BMI2_INTERNAL_STATUS_ADDR                               (XBYTE)(0x21)
#define BMI2_FIFO_LENGTH_0_ADDR                                 (XBYTE)(0X24)
#define BMI2_FIFO_DATA_ADDR                                     (XBYTE)(0X26)
#define BMI2_FEAT_PAGE_ADDR                                     (XBYTE)(0x2F)
#define BMI2_FEATURES_REG_ADDR                                  (XBYTE)(0x30)
#define BMI2_ACC_CONF_ADDR                                      (XBYTE)(0x40)
#define BMI2_GYR_CONF_ADDR                                      (XBYTE)(0x42)
#define BMI2_AUX_CONF_ADDR                                      (XBYTE)(0x44)
#define BMI2_FIFO_DOWNS_ADDR                                    (XBYTE)(0X45)
#define BMI2_FIFO_WTM_0_ADDR                                    (XBYTE)(0X46)
#define BMI2_FIFO_WTM_1_ADDR                                    (XBYTE)(0X47)
#define BMI2_FIFO_CONFIG_0_ADDR                                 (XBYTE)(0X48)
#define BMI2_FIFO_CONFIG_1_ADDR                                 (XBYTE)(0X49)
#define BMI2_AUX_DEV_ID_ADDR                                    (XBYTE)(0x4B)
#define BMI2_AUX_IF_CONF_ADDR                                   (XBYTE)(0x4C)
#define BMI2_AUX_RD_ADDR                                        (XBYTE)(0x4D)
#define BMI2_AUX_WR_ADDR                                        (XBYTE)(0x4E)
#define BMI2_AUX_WR_DATA_ADDR                                   (XBYTE)(0x4F)
#define BMI2_INT1_IO_CTRL_ADDR                                  (XBYTE)(0x53)
#define BMI2_INT2_IO_CTRL_ADDR                                  (XBYTE)(0x54)
#define BMI2_INT_LATCH_ADDR                                     (XBYTE)(0x55)
#define BMI2_INT1_MAP_FEAT_ADDR                                 (XBYTE)(0x56)
#define BMI2_INT2_MAP_FEAT_ADDR                                 (XBYTE)(0x57)
#define BMI2_INT_MAP_DATA_ADDR                                  (XBYTE)(0x58)
#define BMI2_INIT_CTRL_ADDR                                     (XBYTE)(0x59)
#define BMI2_INIT_ADDR_0                                        (XBYTE)(0x5B)
#define BMI2_INIT_ADDR_1                                        (XBYTE)(0x5C)
#define BMI2_INIT_DATA_ADDR                                     (XBYTE)(0x5E)
#define BMI2_AUX_IF_TRIM                                        (XBYTE)(0x68)
#define BMI2_GYR_CRT_CONF_ADDR                                  (XBYTE)(0X69)
#define BMI2_NVM_CONF_ADDR                                      (XBYTE)(0x6A)
#define BMI2_IF_CONF_ADDR                                       (XBYTE)(0X6B)
#define BMI2_ACC_SELF_TEST_ADDR                                 (XBYTE)(0X6D)
#define BMI2_GYR_SELF_TEST_AXES_ADDR                            (XBYTE)(0x6E)
#define BMI2_SELF_TEST_MEMS_ADDR                                (XBYTE)(0X6F)
#define BMI2_NV_CONF_ADDR                                       (XBYTE)(0x70)
#define BMI2_ACC_OFF_COMP_0_ADDR                                (XBYTE)(0X71)
#define BMI2_GYR_OFF_COMP_3_ADDR                                (XBYTE)(0X74)
#define BMI2_GYR_OFF_COMP_6_ADDR                                (XBYTE)(0X77)
#define BMI2_GYR_USR_GAIN_0_ADDR                                (XBYTE)(0X78)
#define BMI2_PWR_CONF_ADDR                                      (XBYTE)(0x7C)
#define BMI2_PWR_CTRL_ADDR                                      (XBYTE)(0x7D)
#define BMI2_CMD_REG_ADDR                                       (XBYTE)(0x7E)

// BMI2 I2C address 
#define BMI2_I2C_PRIM_ADDR                                      (XBYTE)(0x68)
#define BMI2_I2C_SEC_ADDR                                       (XBYTE)(0x69)

// BMI2 Commands 
#define BMI2_G_TRIGGER_CMD                                      (XBYTE)(0x02)
#define BMI2_USR_GAIN_CMD                                       (XBYTE)(0x03)
#define BMI2_NVM_PROG_CMD                                       (XBYTE)(0xA0)
#define BMI2_SOFT_RESET_CMD                                     (XBYTE)(0xB6)
#define BMI2_FIFO_FLUSH_CMD                                     (XBYTE)(0xB0)

// BMI2 sensor data bytes 

#define BMI2_ACC_GYR_NUM_BYTES                                  (XBYTE)(6)
#define BMI2_AUX_NUM_BYTES                                      (XBYTE)(8)
#define BMI2_CRT_CONFIG_FILE_SIZE                               (XWORD)(2048)
#define BMI2_FEAT_SIZE_IN_BYTES                                 (XBYTE)(16)
#define BMI2_ACC_CONFIG_LENGTH                                  (XBYTE)(2)

// BMI2 configuration load status 
#define BMI2_CONFIG_LOAD_SUCCESS                                (XBYTE)(1)

// To define BMI2 pages 
#define BMI2_PAGE_0                                             (XBYTE)(0)
#define BMI2_PAGE_1                                             (XBYTE)(1)
#define BMI2_PAGE_2                                             (XBYTE)(2)
#define BMI2_PAGE_3                                             (XBYTE)(3)
#define BMI2_PAGE_4                                             (XBYTE)(4)
#define BMI2_PAGE_5                                             (XBYTE)(5)
#define BMI2_PAGE_6                                             (XBYTE)(6)
#define BMI2_PAGE_7                                             (XBYTE)(7)

// Array Parameter DefinItions 
#define BMI2_SENSOR_TIME_LSB_BYTE                               (XBYTE)(0)
#define BMI2_SENSOR_TIME_XLSB_BYTE                              (XBYTE)(1)
#define BMI2_SENSOR_TIME_MSB_BYTE                               (XBYTE)(2)

// Mask definitions for Gyro CRT  
#define BMI2_GYR_RDY_FOR_DL_MASK                                (XBYTE)(0x08)
#define BMI2_GYR_CRT_RUNNING_MASK                               (XBYTE)(0x04)

// mask definition for status register 
#define BMI2_AUX_BUSY_MASK                                      (XBYTE)(0x04)
#define BMI2_CMD_RDY_MASK                                       (XBYTE)(0x10)
#define BMI2_DRDY_AUX_MASK                                      (XBYTE)(0x20)
#define BMI2_DRDY_GYR_MASK                                      (XBYTE)(0x40)
#define BMI2_DRDY_ACC_MASK                                      (XBYTE)(0x80)

// Mask definitions for SPI read/write address 
#define BMI2_SPI_RD_MASK                                        (XBYTE)(0x80)
#define BMI2_SPI_WR_MASK                                        (XBYTE)(0x7F)

// Mask definitions for power configuration register 
#define BMI2_ADV_POW_EN_MASK                                    (XBYTE)(0x01)

// Mask definitions for initialization control register 
#define BMI2_CONF_LOAD_EN_MASK                                  (XBYTE)(0x01)

// Mask definitions for power control register 
#define BMI2_AUX_EN_MASK                                        (XBYTE)(0x01)
#define BMI2_GYR_EN_MASK                                        (XBYTE)(0x02)
#define BMI2_ACC_EN_MASK                                        (XBYTE)(0x04)
#define BMI2_TEMP_EN_MASK                                       (XBYTE)(0x08)

// Mask definitions for sensor event flags 
#define BMI2_EVENT_FLAG_MASK                                    (XBYTE)(0x1C)

// Mask definitions to switch page 
#define BMI2_SWITCH_PAGE_EN_MASK                                (XBYTE)(0x07)

// Mask definitions of NVM register 
#define BMI2_NV_ACC_OFFSET_MASK                                 (XBYTE)(0x08)

// Mask definition for config version 
#define BMI2_CONFIG_MAJOR_MASK                                  (XWORD)(0x3C0)
#define BMI2_CONFIG_MINOR_MASK                                  (XBYTE)(0x3F)

// mask and bit position for activity recognition settings 
#define BMI2_ACT_RECG_POST_PROS_EN_DIS_MASK                     (XBYTE)(0x01)
#define BMI2_ACT_RECG_BUFF_SIZE_MASK                            (XBYTE)(0x0F)
#define BMI2_ACT_RECG_MIN_SEG_CONF_MASK                         (XBYTE)(0x0F)

// mask and bit position for activity recognition hc settings 
#define BMI2_HC_ACT_RECG_SEGMENT_SIZE_MASK                      (XBYTE)(0x03)
#define BMI2_HC_ACT_RECG_PP_EN_MASK                             (XBYTE)(0x01)
#define BMI2_HC_ACT_RECG_MIN_GDI_THRES_MASK                     (XWORD)(0xFFFF)
#define BMI2_HC_ACT_RECG_MAX_GDI_THRES_MASK                     (XWORD)(0xFFFF)
#define BMI2_HC_ACT_RECG_BUF_SIZE_MASK                          (XWORD)(0xFFFF)
#define BMI2_HC_ACT_RECG_MIN_SEG_CONF_MASK                      (XWORD)(0xFFFF)

#define BMI2_GYRO_CROSS_AXES_SENSE_MASK                         (XBYTE)(0x7F)
#define BMI2_GYRO_CROSS_AXES_SENSE_SIGN_BIT_MASK                (XBYTE)(0x40)

// Bit position definitions for Gyro CRT 
#define BMI2_GYR_RDY_FOR_DL_POS                                 (XBYTE)(0x03)
#define BMI2_GYR_CRT_RUNNING_POS                                (XBYTE)(0x02)

// Bit position for status register
#define BMI2_AUX_BUSY_POS                                       (XBYTE)(0x02)
#define BMI2_CMD_RDY_POS                                        (XBYTE)(0x04)
#define BMI2_DRDY_AUX_POS                                       (XBYTE)(0x05)
#define BMI2_DRDY_GYR_POS                                       (XBYTE)(0x06)
#define BMI2_DRDY_ACC_POS                                       (XBYTE)(0x07)

// Bit position definitions for power control register 
#define BMI2_GYR_EN_POS                                         (XBYTE)(0x01)
#define BMI2_ACC_EN_POS                                         (XBYTE)(0x02)
#define BMI2_TEMP_EN_POS                                        (XBYTE)(0x03)

// Bit position definitions for sensor event flags 
#define BMI2_EVENT_FLAG_POS                                     (XBYTE)(0x02)

// Bit position definitions of NVM register 
#define BMI2_NV_ACC_OFFSET_POS                                  (XBYTE)(0x03)

// Bit position for major version from config 
#define BMI2_CONFIG_MAJOR_POS                                   (XBYTE)(0x06)

// Accelerometer and Gyroscope Filter/Noise performance modes 
// Power optimized mode 
#define BMI2_POWER_OPT_MODE                                     (XBYTE)(0)

// Performance optimized  
#define BMI2_PERF_OPT_MODE                                      (XBYTE)(1)

// index for config major minor information 
#define BMI2_CONFIG_INFO_LOWER                                  (XBYTE)(52)
#define BMI2_CONFIG_INFO_HIGHER                                 (XBYTE)(53)

// Sensor status 
#define BMI2_DRDY_ACC                                           (XBYTE)(0x80)
#define BMI2_DRDY_GYR                                           (XBYTE)(0x40)
#define BMI2_DRDY_AUX                                           (XBYTE)(0x20)
#define BMI2_CMD_RDY                                            (XBYTE)(0x10)
#define BMI2_AUX_BUSY                                           (XBYTE)(0x04)

// Macro to define accelerometer configuration value for FOC 
#define BMI2_FOC_ACC_CONF_VAL                                   (XBYTE)(0xB7)

// Macro to define gyroscope configuration value for FOC 
#define BMI2_FOC_GYR_CONF_VAL                                   (XBYTE)(0xB6)

// Macro to define X Y and Z axis for an array 
#define BMI2_X_AXIS                                             (XBYTE)(0)
#define BMI2_Y_AXIS                                             (XBYTE)(1)
#define BMI2_Z_AXIS                                             (XBYTE)(2)

// Macros to define BMI2 sensor/feature types 
#define BMI2_ACCEL                                              (XBYTE)(0)
#define BMI2_GYRO                                               (XBYTE)(1)
#define BMI2_AUX                                                (XBYTE)(2)
#define BMI2_SIG_MOTION                                         (XBYTE)(3)
#define BMI2_ANY_MOTION                                         (XBYTE)(4)
#define BMI2_NO_MOTION                                          (XBYTE)(5)
#define BMI2_STEP_DETECTOR                                      (XBYTE)(6)
#define BMI2_STEP_COUNTER                                       (XBYTE)(7)
#define BMI2_STEP_ACTIVITY                                      (XBYTE)(8)
#define BMI2_GYRO_GAIN_UPDATE                                   (XBYTE)(9)
#define BMI2_TILT                                               (XBYTE)(10)
#define BMI2_UP_HOLD_TO_WAKE                                    (XBYTE)(11)
#define BMI2_GLANCE_DETECTOR                                    (XBYTE)(12)
#define BMI2_WAKE_UP                                            (XBYTE)(13)
#define BMI2_ORIENTATION                                        (XBYTE)(14)
#define BMI2_HIGH_G                                             (XBYTE)(15)
#define BMI2_LOW_G                                              (XBYTE)(16)
#define BMI2_FLAT                                               (XBYTE)(17)
#define BMI2_EXT_SENS_SYNC                                      (XBYTE)(18)
#define BMI2_WRIST_GESTURE                                      (XBYTE)(19)
#define BMI2_WRIST_WEAR_WAKE_UP                                 (XBYTE)(20)
#define BMI2_WRIST_WEAR_WAKE_UP_WH                              (XBYTE)(21)
#define BMI2_WRIST_GESTURE_WH                                   (XBYTE)(22)
#define BMI2_PRIMARY_OIS                                        (XBYTE)(23)
#define BMI2_FREE_FALL_DET                                      (XBYTE)(24)
#define BMI2_SINGLE_TAP                                         (XBYTE)(25)
#define BMI2_DOUBLE_TAP                                         (XBYTE)(26)
#define BMI2_TRIPLE_TAP                                         (XBYTE)(27)
#define BMI2_TAP                                                (XBYTE)(28)

// Non virtual sensor features 
#define BMI2_STEP_COUNTER_PARAMS                                (XBYTE)(29)
#define BMI2_TAP_DETECTOR_1                                     (XBYTE)(30)
#define BMI2_TAP_DETECTOR_2                                     (XBYTE)(31)
#define BMI2_TEMP                                               (XBYTE)(32)
#define BMI2_ACCEL_SELF_TEST                                    (XBYTE)(33)
#define BMI2_GYRO_SELF_OFF                                      (XBYTE)(34)
#define BMI2_ACTIVITY_RECOGNITION                               (XBYTE)(35)
#define BMI2_MAX_BURST_LEN                                      (XBYTE)(36)
#define BMI2_SENS_MAX_NUM                                       (XBYTE)(37)
#define BMI2_AXIS_MAP                                           (XBYTE)(38)
#define BMI2_NVM_STATUS                                         (XBYTE)(39)
#define BMI2_VFRM_STATUS                                        (XBYTE)(40)
#define BMI2_GYRO_CROSS_SENSE                                   (XBYTE)(41)
#define BMI2_CRT_GYRO_SELF_TEST                                 (XBYTE)(42)
#define BMI2_ABORT_CRT_GYRO_SELF_TEST                           (XBYTE)(43)
#define BMI2_NVM_PROG_PREP                                      (XBYTE)(44)
#define BMI2_ACTIVITY_RECOGNITION_SETTINGS                      (XBYTE)(45)
#define BMI2_OIS_OUTPUT                                         (XBYTE)(46)
#define BMI2_CONFIG_ID                                          (XBYTE)(47)

// Bit wise for selecting BMI2 sensors/features 
#define BMI2_ACCEL_SENS_SEL                                     (1)
#define BMI2_GYRO_SENS_SEL                                      (1 << BMI2_GYRO)
#define BMI2_AUX_SENS_SEL                                       (1 << BMI2_AUX)
#define BMI2_TEMP_SENS_SEL                                      ((XQWORD)1 << BMI2_TEMP)
#define BMI2_ANY_MOT_SEL                                        (1 << BMI2_ANY_MOTION)
#define BMI2_NO_MOT_SEL                                         (1 << BMI2_NO_MOTION)
#define BMI2_TILT_SEL                                           (1 << BMI2_TILT)
#define BMI2_ORIENT_SEL                                         (1 << BMI2_ORIENTATION)
#define BMI2_SIG_MOTION_SEL                                     (1 << BMI2_SIG_MOTION)
#define BMI2_STEP_DETECT_SEL                                    (1 << BMI2_STEP_DETECTOR)
#define BMI2_STEP_COUNT_SEL                                     (1 << BMI2_STEP_COUNTER)
#define BMI2_STEP_ACT_SEL                                       (1 << BMI2_STEP_ACTIVITY)
#define BMI2_GYRO_GAIN_UPDATE_SEL                               (1 << BMI2_GYRO_GAIN_UPDATE)
#define BMI2_UP_HOLD_TO_WAKE_SEL                                (1 << BMI2_UP_HOLD_TO_WAKE)
#define BMI2_GLANCE_DET_SEL                                     (1 << BMI2_GLANCE_DETECTOR)
#define BMI2_WAKE_UP_SEL                                        (1 << BMI2_WAKE_UP)
#define BMI2_TAP_SEL                                            (1 << BMI2_TAP)
#define BMI2_HIGH_G_SEL                                         (1 << BMI2_HIGH_G)
#define BMI2_LOW_G_SEL                                          (1 << BMI2_LOW_G)
#define BMI2_FLAT_SEL                                           (1 << BMI2_FLAT)
#define BMI2_EXT_SENS_SEL                                       (1 << BMI2_EXT_SENS_SYNC)
#define BMI2_SINGLE_TAP_SEL                                     (1 << BMI2_SINGLE_TAP)
#define BMI2_DOUBLE_TAP_SEL                                     (1 << BMI2_DOUBLE_TAP)
#define BMI2_TRIPLE_TAP_SEL                                     (1 << BMI2_TRIPLE_TAP)
#define BMI2_GYRO_SELF_OFF_SEL                                  ((XQWORD)1 << BMI2_GYRO_SELF_OFF)
#define BMI2_WRIST_GEST_SEL                                     (1 << BMI2_WRIST_GESTURE)
#define BMI2_WRIST_WEAR_WAKE_UP_SEL                             (1 << BMI2_WRIST_WEAR_WAKE_UP)
#define BMI2_ACTIVITY_RECOGNITION_SEL                           ((XQWORD)1 << BMI2_ACTIVITY_RECOGNITION)
#define BMI2_ACCEL_SELF_TEST_SEL                                ((XQWORD)1 << BMI2_ACCEL_SELF_TEST)
#define BMI2_WRIST_GEST_W_SEL                                   (1 << BMI2_WRIST_GESTURE_WH)
#define BMI2_WRIST_WEAR_WAKE_UP_WH_SEL                          (1 << BMI2_WRIST_WEAR_WAKE_UP_WH)
#define BMI2_PRIMARY_OIS_SEL                                    (1 << BMI2_PRIMARY_OIS)
#define BMI2_FREE_FALL_DET_SEL                                  (1 << BMI2_FREE_FALL_DET)

// Mask definitions for BMI2 wake-up feature configuration for bmi260 
#define BMI2_WAKEUP_SENSITIVITY_MASK                            (XBYTE)(0x0E)
#define BMI2_WAKEUP_SINGLE_TAP_EN_MASK                          (XBYTE)(0x01)
#define BMI2_WAKEUP_DOUBLE_TAP_EN_MASK                          (XBYTE)(0x02)
#define BMI2_WAKEUP_TRIPLE_TAP_EN_MASK                          (XBYTE)(0x04)
#define BMI2_WAKEUP_DATA_REG_EN_MASK                            (XBYTE)(0x08)
#define BMI2_WAKEUP_AXIS_SEL_MASK                               (XBYTE)(0x03)

// Bit position definitions for BMI2 wake-up feature configuration for bmi260 
#define BMI2_WAKEUP_SENSITIVITY_POS                             (XBYTE)(0x01)
#define BMI2_WAKEUP_DOUBLE_TAP_EN_POS                           (XBYTE)(0x01)
#define BMI2_WAKEUP_TRIPLE_TAP_EN_POS                           (XBYTE)(0x02)
#define BMI2_WAKEUP_DATA_REG_EN_POS                             (XBYTE)(0x03)

// Mask definitions for BMI2 tap feature configuration for bmi260t 
#define BMI2_TAP_SENSITIVITY_MASK                               (XBYTE)(0x0E)
#define BMI2_TAP_SINGLE_TAP_EN_MASK                             (XBYTE)(0x01)
#define BMI2_TAP_DOUBLE_TAP_EN_MASK                             (XBYTE)(0x02)
#define BMI2_TAP_TRIPLE_TAP_EN_MASK                             (XBYTE)(0x04)
#define BMI2_TAP_DATA_REG_EN_MASK                               (XBYTE)(0x08)
#define BMI2_TAP_AXIS_SEL_MASK                                  (XBYTE)(0x03)

// Bit position definitions for BMI2 tap feature configuration for bmi260t 
#define BMI2_TAP_SENSITIVITY_POS                                (XBYTE)(0x01)
#define BMI2_TAP_DOUBLE_TAP_EN_POS                              (XBYTE)(0x01)
#define BMI2_TAP_TRIPLE_TAP_EN_POS                              (XBYTE)(0x02)
#define BMI2_TAP_DATA_REG_EN_POS                                (XBYTE)(0x03)

// Mask definitions for BMI2 wake-up feature configuration for other than bmi261 
#define BMI2_WAKE_UP_SENSITIVITY_MASK                           (XWORD)(0x000E)
#define BMI2_WAKE_UP_SINGLE_TAP_EN_MASK                         (XWORD)(0x0010)

// Bit position definitions for BMI2 wake-up feature configuration for other than bmi261 
#define BMI2_WAKE_UP_SENSITIVITY_POS                            (XBYTE)(0x01)
#define BMI2_WAKE_UP_SINGLE_TAP_EN_POS                          (XBYTE)(0x04)

// Offsets from feature start address for BMI2 feature enable/disable 
#define BMI2_ANY_MOT_FEAT_EN_OFFSET                             (XBYTE)(0x03)
#define BMI2_NO_MOT_FEAT_EN_OFFSET                              (XBYTE)(0x03)
#define BMI2_SIG_MOT_FEAT_EN_OFFSET                             (XBYTE)(0x0A)
#define BMI2_STEP_COUNT_FEAT_EN_OFFSET                          (XBYTE)(0x01)
#define BMI2_GYR_USER_GAIN_FEAT_EN_OFFSET                       (XBYTE)(0x05)
#define BMI2_HIGH_G_FEAT_EN_OFFSET                              (XBYTE)(0x03)
#define BMI2_LOW_G_FEAT_EN_OFFSET                               (XBYTE)(0x03)
#define BMI2_TILT_FEAT_EN_OFFSET                                (XBYTE)(0x00)

// Mask definitions for BMI2 feature enable/disable 
#define BMI2_ANY_NO_MOT_EN_MASK                                 (XBYTE)(0x80)
#define BMI2_TILT_FEAT_EN_MASK                                  (XBYTE)(0x01)
#define BMI2_ORIENT_FEAT_EN_MASK                                (XBYTE)(0x01)
#define BMI2_SIG_MOT_FEAT_EN_MASK                               (XBYTE)(0x01)
#define BMI2_STEP_DET_FEAT_EN_MASK                              (XBYTE)(0x08)
#define BMI2_STEP_COUNT_FEAT_EN_MASK                            (XBYTE)(0x10)
#define BMI2_STEP_ACT_FEAT_EN_MASK                              (XBYTE)(0x20)
#define BMI2_GYR_USER_GAIN_FEAT_EN_MASK                         (XBYTE)(0x08)
#define BMI2_UP_HOLD_TO_WAKE_FEAT_EN_MASK                       (XBYTE)(0x01)
#define BMI2_GLANCE_FEAT_EN_MASK                                (XBYTE)(0x01)
#define BMI2_WAKE_UP_FEAT_EN_MASK                               (XBYTE)(0x01)
#define BMI2_TAP_FEAT_EN_MASK                                   (XBYTE)(0x01)
#define BMI2_HIGH_G_FEAT_EN_MASK                                (XBYTE)(0x80)
#define BMI2_LOW_G_FEAT_EN_MASK                                 (XBYTE)(0x10)
#define BMI2_FLAT_FEAT_EN_MASK                                  (XBYTE)(0x01)
#define BMI2_EXT_SENS_SYNC_FEAT_EN_MASK                         (XBYTE)(0x01)
#define BMI2_GYR_SELF_OFF_CORR_FEAT_EN_MASK                     (XBYTE)(0x02)
#define BMI2_WRIST_GEST_FEAT_EN_MASK                            (XBYTE)(0x20)
#define BMI2_WRIST_WEAR_WAKE_UP_FEAT_EN_MASK                    (XBYTE)(0x10)
#define BMI2_ACTIVITY_RECOG_EN_MASK                             (XBYTE)(0x01)
#define BMI2_ACC_SELF_TEST_FEAT_EN_MASK                         (XBYTE)(0x02)
#define BMI2_GYRO_SELF_TEST_CRT_EN_MASK                         (XBYTE)(0x01)
#define BMI2_ABORT_FEATURE_EN_MASK                              (XBYTE)(0x02)
#define BMI2_NVM_PREP_FEATURE_EN_MASK                           (XBYTE)(0x04)
#define BMI2_FREE_FALL_DET_FEAT_EN_MASK                         (XBYTE)(0x01)
#define BMI2_WRIST_GEST_WH_FEAT_EN_MASK                         (XBYTE)(0x02)

// Bit position definitions for BMI2 feature enable/disable 
#define BMI2_ANY_NO_MOT_EN_POS                                  (XBYTE)(0x07)
#define BMI2_STEP_DET_FEAT_EN_POS                               (XBYTE)(0x03)
#define BMI2_STEP_COUNT_FEAT_EN_POS                             (XBYTE)(0x04)
#define BMI2_STEP_ACT_FEAT_EN_POS                               (XBYTE)(0x05)
#define BMI2_GYR_USER_GAIN_FEAT_EN_POS                          (XBYTE)(0x03)
#define BMI2_HIGH_G_FEAT_EN_POS                                 (XBYTE)(0x07)
#define BMI2_LOW_G_FEAT_EN_POS                                  (XBYTE)(0x04)
#define BMI2_GYR_SELF_OFF_CORR_FEAT_EN_POS                      (XBYTE)(0x01)
#define BMI2_WRIST_GEST_FEAT_EN_POS                             (XBYTE)(0x05)
#define BMI2_WRIST_WEAR_WAKE_UP_FEAT_EN_POS                     (XBYTE)(0x04)
#define BMI2_ACC_SELF_TEST_FEAT_EN_POS                          (XBYTE)(0x01)
#define BMI2_ABORT_FEATURE_EN_POS                               (XBYTE)(0x1)
#define BMI2_NVM_PREP_FEATURE_EN_POS                            (XBYTE)(0x02)
#define BMI2_WRIST_GEST_WH_FEAT_EN_POS                          (XBYTE)(0x01)

// Primary OIS low pass filter configuration position and mask 
#define BMI2_LP_FILTER_EN_MASK                                  (XBYTE)(0x01)

#define BMI2_LP_FILTER_CONFIG_POS                               (XBYTE)(0x01)
#define BMI2_LP_FILTER_CONFIG_MASK                              (XBYTE)(0x06)

#define BMI2_PRIMARY_OIS_GYR_EN_POS                             (XBYTE)(0x06)
#define BMI2_PRIMARY_OIS_GYR_EN_MASK                            (XBYTE)(0x40)

#define BMI2_PRIMARY_OIS_ACC_EN_POS                             (XBYTE)(0x07)
#define BMI2_PRIMARY_OIS_ACC_EN_MASK                            (XBYTE)(0x80)

// Mask definitions for BMI2 any and no-motion feature configuration 
#define BMI2_ANY_NO_MOT_DUR_MASK                                (XWORD)(0x1FFF)
#define BMI2_ANY_NO_MOT_X_SEL_MASK                              (XWORD)(0x2000)
#define BMI2_ANY_NO_MOT_Y_SEL_MASK                              (XWORD)(0x4000)
#define BMI2_ANY_NO_MOT_Z_SEL_MASK                              (XWORD)(0x8000)
#define BMI2_ANY_NO_MOT_THRES_MASK                              (XWORD)(0x07FF)
#define BMI2_ANY_MOT_INT_MASK                                   (XBYTE)(0x40)

// Mask definitions for BMI2 no-motion interrupt mapping 
#define BMI2_NO_MOT_INT_MASK                                    (XBYTE)(0x20)

// Bit position definitions for BMI2 any and no-motion feature configuration
 
#define BMI2_ANY_NO_MOT_X_SEL_POS                               (XBYTE)(0x0D)
#define BMI2_ANY_NO_MOT_Y_SEL_POS                               (XBYTE)(0x0E)
#define BMI2_ANY_NO_MOT_Z_SEL_POS                               (XBYTE)(0x0F)

// Mask definitions for BMI2 orientation feature configuration 
#define BMI2_ORIENT_UP_DOWN_MASK                                (XWORD)(0x0002)
#define BMI2_ORIENT_SYMM_MODE_MASK                              (XWORD)(0x000C)
#define BMI2_ORIENT_BLOCK_MODE_MASK                             (XWORD)(0x0030)
#define BMI2_ORIENT_THETA_MASK                                  (XWORD)(0x0FC0)
#define BMI2_ORIENT_HYST_MASK                                   (XWORD)(0x07FF)

// Bit position definitions for BMI2 orientation feature configuration 
#define BMI2_ORIENT_UP_DOWN_POS                                 (XBYTE)(0x01)
#define BMI2_ORIENT_SYMM_MODE_POS                               (XBYTE)(0x02)
#define BMI2_ORIENT_BLOCK_MODE_POS                              (XBYTE)(0x04)
#define BMI2_ORIENT_THETA_POS                                   (XBYTE)(0x06)

// Mask definitions for BMI2 sig-motion feature configuration 
#define BMI2_SIG_MOT_PARAM_1_MASK                               (XWORD)(0xFFFF)
#define BMI2_SIG_MOT_PARAM_2_MASK                               (XWORD)(0xFFFF)
#define BMI2_SIG_MOT_PARAM_3_MASK                               (XWORD)(0xFFFF)
#define BMI2_SIG_MOT_PARAM_4_MASK                               (XWORD)(0xFFFF)
#define BMI2_SIG_MOT_PARAM_5_MASK                               (XWORD)(0xFFFF)

// Mask definitions for BMI2 parameter configurations 
#define BMI2_STEP_COUNT_PARAMS_MASK                             (XWORD)(0xFFFF)

// Mask definitions for BMI2 step-counter/detector feature configuration 
#define BMI2_STEP_COUNT_WM_LEVEL_MASK                           (XWORD)(0x03FF)
#define BMI2_STEP_COUNT_RST_CNT_MASK                            (XWORD)(0x0400)
#define BMI2_STEP_BUFFER_SIZE_MASK                              (XWORD)(0XFF00)
#define BMI2_STEP_COUNT_INT_MASK                                (XBYTE)(0x02)
#define BMI2_STEP_ACT_INT_MASK                                  (XBYTE)(0x04)

// Bit position definitions for BMI2 step-counter/detector feature configuration
 
#define BMI2_STEP_COUNT_RST_CNT_POS                             (XBYTE)(0x0A)
#define BMI2_STEP_BUFFER_SIZE_POS                               (XBYTE)(0X08)

// Mask definitions for BMI2 gyroscope user gain feature configuration
 
#define BMI2_GYR_USER_GAIN_RATIO_X_MASK                         (XWORD)(0x07FF)
#define BMI2_GYR_USER_GAIN_RATIO_Y_MASK                         (XWORD)(0x07FF)
#define BMI2_GYR_USER_GAIN_RATIO_Z_MASK                         (XWORD)(0x07FF)

// Mask definitions for BMI2 gyroscope user gain saturation status 
#define BMI2_GYR_USER_GAIN_SAT_STAT_X_MASK                      (XBYTE)(0x01)
#define BMI2_GYR_USER_GAIN_SAT_STAT_Y_MASK                      (XBYTE)(0x02)
#define BMI2_GYR_USER_GAIN_SAT_STAT_Z_MASK                      (XBYTE)(0x04)
#define BMI2_G_TRIGGER_STAT_MASK                                (XBYTE)(0x38)

// Bit position definitions for BMI2 gyroscope user gain saturation status 
#define BMI2_GYR_USER_GAIN_SAT_STAT_Y_POS                       (XBYTE)(0x01)
#define BMI2_GYR_USER_GAIN_SAT_STAT_Z_POS                       (XBYTE)(0x02)
#define BMI2_G_TRIGGER_STAT_POS                                 (XBYTE)(0x03)

// Mask definitions for MSB values of BMI2 gyroscope compensation 
#define BMI2_GYR_OFF_COMP_MSB_X_MASK                            (XBYTE)(0x03)
#define BMI2_GYR_OFF_COMP_MSB_Y_MASK                            (XBYTE)(0x0C)
#define BMI2_GYR_OFF_COMP_MSB_Z_MASK                            (XBYTE)(0x30)

// Bit positions for MSB values of BMI2 gyroscope compensation 
#define BMI2_GYR_OFF_COMP_MSB_Y_POS                             (XBYTE)(0x02)
#define BMI2_GYR_OFF_COMP_MSB_Z_POS                             (XBYTE)(0x04)

// Mask definitions for MSB values of BMI2 gyroscope compensation from user input 
#define BMI2_GYR_OFF_COMP_MSB_MASK                              (XWORD)(0x0300)
#define BMI2_GYR_OFF_COMP_LSB_MASK                              (XWORD)(0x00FF)

// Mask definitions for BMI2 orientation status 
#define BMI2_ORIENT_DETECT_MASK                                 (XBYTE)(0x03)
#define BMI2_ORIENT_FACE_UP_DWN_MASK                            (XBYTE)(0x04)

// Bit position definitions for BMI2 orientation status 
#define BMI2_ORIENT_FACE_UP_DWN_POS                             (XBYTE)(0x02)

// Mask definitions for NVM-VFRM error status 
#define BMI2_NVM_LOAD_ERR_STATUS_MASK                           (XBYTE)(0x01)
#define BMI2_NVM_PROG_ERR_STATUS_MASK                           (XBYTE)(0x02)
#define BMI2_NVM_ERASE_ERR_STATUS_MASK                          (XBYTE)(0x04)
#define BMI2_NVM_END_EXCEED_STATUS_MASK                         (XBYTE)(0x08)
#define BMI2_NVM_PRIV_ERR_STATUS_MASK                           (XBYTE)(0x10)
#define BMI2_VFRM_LOCK_ERR_STATUS_MASK                          (XBYTE)(0x20)
#define BMI2_VFRM_WRITE_ERR_STATUS_MASK                         (XBYTE)(0x40)
#define BMI2_VFRM_FATAL_ERR_STATUS_MASK                         (XBYTE)(0x80)

// Bit positions for NVM-VFRM error status 
#define BMI2_NVM_PROG_ERR_STATUS_POS                            (XBYTE)(0x01)
#define BMI2_NVM_ERASE_ERR_STATUS_POS                           (XBYTE)(0x02)
#define BMI2_NVM_END_EXCEED_STATUS_POS                          (XBYTE)(0x03)
#define BMI2_NVM_PRIV_ERR_STATUS_POS                            (XBYTE)(0x04)
#define BMI2_VFRM_LOCK_ERR_STATUS_POS                           (XBYTE)(0x05)
#define BMI2_VFRM_WRITE_ERR_STATUS_POS                          (XBYTE)(0x06)
#define BMI2_VFRM_FATAL_ERR_STATUS_POS                          (XBYTE)(0x07)

// Mask definitions for accelerometer self-test status 
#define BMI2_ACC_SELF_TEST_DONE_MASK                            (XBYTE)(0x01)
#define BMI2_ACC_X_OK_MASK                                      (XBYTE)(0x02)
#define BMI2_ACC_Y_OK_MASK                                      (XBYTE)(0x04)
#define BMI2_ACC_Z_OK_MASK                                      (XBYTE)(0x08)

// Bit Positions for accelerometer self-test status 
#define BMI2_ACC_X_OK_POS                                       (XBYTE)(0x01)
#define BMI2_ACC_Y_OK_POS                                       (XBYTE)(0x02)
#define BMI2_ACC_Z_OK_POS                                       (XBYTE)(0x03)

// Mask definitions for BMI2 high-g feature configuration 
#define BMI2_HIGH_G_THRES_MASK                                  (XWORD)(0x7FFF)
#define BMI2_HIGH_G_HYST_MASK                                   (XWORD)(0x0FFF)
#define BMI2_HIGH_G_X_SEL_MASK                                  (XWORD)(0x1000)
#define BMI2_HIGH_G_Y_SEL_MASK                                  (XWORD)(0x2000)
#define BMI2_HIGH_G_Z_SEL_MASK                                  (XWORD)(0x4000)
#define BMI2_HIGH_G_DUR_MASK                                    (XWORD)(0x0FFF)

// Bit position definitions for BMI2 high-g feature configuration 
#define BMI2_HIGH_G_X_SEL_POS                                   (XBYTE)(0x0C)
#define BMI2_HIGH_G_Y_SEL_POS                                   (XBYTE)(0x0D)
#define BMI2_HIGH_G_Z_SEL_POS                                   (XBYTE)(0x0E)

// Mask definitions for BMI2 low-g feature configuration 
#define BMI2_LOW_G_THRES_MASK                                   (XWORD)(0x7FFF)
#define BMI2_LOW_G_HYST_MASK                                    (XWORD)(0x0FFF)
#define BMI2_LOW_G_DUR_MASK                                     (XWORD)(0x0FFF)

// Mask definitions for BMI2 free-fall detection feature configuration 
#define BMI2_FREE_FALL_ACCEL_SETT_MASK                          (XWORD)(0xFFFF)

// Mask definitions for BMI2 flat feature configuration 
#define BMI2_FLAT_THETA_MASK                                    (XWORD)(0x007E)
#define BMI2_FLAT_BLOCK_MASK                                    (XWORD)(0x0180)
#define BMI2_FLAT_HYST_MASK                                     (XWORD)(0x003F)
#define BMI2_FLAT_HOLD_TIME_MASK                                (XWORD)(0x3FC0)

// Bit position definitions for BMI2 flat feature configuration 
#define BMI2_FLAT_THETA_POS                                     (XBYTE)(0x01)
#define BMI2_FLAT_BLOCK_POS                                     (XBYTE)(0x07)
#define BMI2_FLAT_HOLD_TIME_POS                                 (XBYTE)(0x06)

// Mask definitions for BMI2 wrist gesture configuration 
#define BMI2_WRIST_GEST_WEAR_ARM_MASK                           (XWORD)(0x0010)

// Bit position definitions for wrist gesture configuration 
#define BMI2_WRIST_GEST_WEAR_ARM_POS                            (XBYTE)(0x04)

// Mask definitions for BMI2 wrist gesture wh configuration 
#define BMI2_WRIST_GEST_WH_DEVICE_POS_MASK                      (XWORD)(0x0001)
#define BMI2_WRIST_GEST_WH_INT                                  (XBYTE)(0x10)
#define BMI2_WRIST_GEST_WH_START_ADD                            (XBYTE)(0x08)

// Mask definitions for BMI2 wrist wear wake-up configuration 
#define BMI2_WRIST_WAKE_UP_WH_INT_MASK                          (XBYTE)(0x08)

// Mask definition for BMI2 wrist wear wake-up configuration for wearable variant 
#define BMI2_WRIST_WAKE_UP_ANGLE_LR_MASK                        (XWORD)(0x00FF)
#define BMI2_WRIST_WAKE_UP_ANGLE_LL_MASK                        (XWORD)(0xFF00)
#define BMI2_WRIST_WAKE_UP_ANGLE_PD_MASK                        (XWORD)(0x00FF)
#define BMI2_WRIST_WAKE_UP_ANGLE_PU_MASK                        (XWORD)(0xFF00)
#define BMI2_WRIST_WAKE_UP_MIN_DUR_MOVED_MASK                   (XWORD)(0x00FF)
#define BMI2_WRIST_WAKE_UP_MIN_DUR_QUITE_MASK                   (XWORD)(0xFF00)

// Bit position definition for BMI2 wrist wear wake-up configuration for wearable variant 
#define BMI2_WRIST_WAKE_UP_ANGLE_LL_POS                         (XWORD)(0x0008)
#define BMI2_WRIST_WAKE_UP_ANGLE_PU_POS                         (XWORD)(0x0008)
#define BMI2_WRIST_WAKE_UP_MIN_DUR_QUITE_POS                    (XWORD)(0x0008)

// Macros to define values of BMI2 axis and its sign for re-map settings
 
#define BMI2_MAP_X_AXIS                                         (XBYTE)(0x00)
#define BMI2_MAP_Y_AXIS                                         (XBYTE)(0x01)
#define BMI2_MAP_Z_AXIS                                         (XBYTE)(0x02)
#define BMI2_MAP_POSITIVE                                       (XBYTE)(0x00)
#define BMI2_MAP_NEGATIVE                                       (XBYTE)(0x01)

// Mask definitions of BMI2 axis re-mapping 
#define BMI2_X_AXIS_MASK                                        (XBYTE)(0x03)
#define BMI2_X_AXIS_SIGN_MASK                                   (XBYTE)(0x04)
#define BMI2_Y_AXIS_MASK                                        (XBYTE)(0x18)
#define BMI2_Y_AXIS_SIGN_MASK                                   (XBYTE)(0x20)
#define BMI2_Z_AXIS_MASK                                        (XBYTE)(0xC0)
#define BMI2_Z_AXIS_SIGN_MASK                                   (XBYTE)(0x01)

// Bit position definitions of BMI2 axis re-mapping 
#define BMI2_X_AXIS_SIGN_POS                                    (XBYTE)(0x02)
#define BMI2_Y_AXIS_POS                                         (XBYTE)(0x03)
#define BMI2_Y_AXIS_SIGN_POS                                    (XBYTE)(0x05)
#define BMI2_Z_AXIS_POS                                         (XBYTE)(0x06)

// Macros to define polarity 
#define BMI2_NEG_SIGN                                           (XBYTE)(1)
#define BMI2_POS_SIGN                                           (XBYTE)(0)

// Macro to define related to CRT 
#define BMI2_CRT_READY_FOR_DOWNLOAD_US                          (XWORD)(2000)
#define BMI2_CRT_READY_FOR_DOWNLOAD_RETRY                       (XBYTE)(100)

#define BMI2_CRT_WAIT_RUNNING_US                                (XWORD)(10000)
#define BMI2_CRT_WAIT_RUNNING_RETRY_EXECUTION                   (XBYTE)(200)

#define BMI2_CRT_MIN_BURST_WORD_LENGTH                          (XBYTE)(2)
#define BMI2_CRT_MAX_BURST_WORD_LENGTH                          (XWORD)(255)

#define BMI2_ACC_FOC_2G_REF                                     (XWORD)(16384)
#define BMI2_ACC_FOC_4G_REF                                     (XWORD)(8192)
#define BMI2_ACC_FOC_8G_REF                                     (XWORD)(4096)
#define BMI2_ACC_FOC_16G_REF                                    (XWORD)(2048)

#define BMI2_GYRO_FOC_NOISE_LIMIT_NEGATIVE                      (XBYTESIG)(-20)
#define BMI2_GYRO_FOC_NOISE_LIMIT_POSITIVE                      (XBYTESIG)(20)

// reference value with positive and negative noise range in lsb 
#define BMI2_ACC_2G_MAX_NOISE_LIMIT                             (BMI2_ACC_FOC_2G_REF + (XWORD)(255))
#define BMI2_ACC_2G_MIN_NOISE_LIMIT                             (BMI2_ACC_FOC_2G_REF - (XWORD)(255))
#define BMI2_ACC_4G_MAX_NOISE_LIMIT                             (BMI2_ACC_FOC_4G_REF + (XWORD)(255))
#define BMI2_ACC_4G_MIN_NOISE_LIMIT                             (BMI2_ACC_FOC_4G_REF - (XWORD)(255))
#define BMI2_ACC_8G_MAX_NOISE_LIMIT                             (BMI2_ACC_FOC_8G_REF + (XWORD)(255))
#define BMI2_ACC_8G_MIN_NOISE_LIMIT                             (BMI2_ACC_FOC_8G_REF - (XWORD)(255))
#define BMI2_ACC_16G_MAX_NOISE_LIMIT                            (BMI2_ACC_FOC_16G_REF + (XWORD)(255))
#define BMI2_ACC_16G_MIN_NOISE_LIMIT                            (BMI2_ACC_FOC_16G_REF - (XWORD)(255))

#define BMI2_FOC_SAMPLE_LIMIT                                   (XBYTE)(128)

// Bit wise selection of BMI2 sensors 
#define BMI2_MAIN_SENSORS                                       (BMI2_ACCEL_SENS_SEL | BMI2_GYRO_SENS_SEL | BMI2_AUX_SENS_SEL | BMI2_TEMP_SENS_SEL)

// Maximum number of BMI2 main sensors 
#define BMI2_MAIN_SENS_MAX_NUM                                  (XBYTE)(4)

// Macro to specify the number of step counter parameters 
#define BMI2_STEP_CNT_N_PARAMS                                  (XBYTE)(25)

// Macro to specify the number of free-fall accel setting parameters 
#define BMI2_FREE_FALL_ACCEL_SET_PARAMS                         (XBYTE)(7)

#define BMI2_SELECT_GYRO_SELF_TEST                              (XBYTE)(0)
#define BMI2_SELECT_CRT                                         (XBYTE)(1)

// Macro for NVM enable 
#define BMI2_NVM_UNLOCK_ENABLE                                  (XBYTE)(0x02)
#define BMI2_NVM_UNLOCK_DISABLE                                 (XBYTE)(0x00)

// macro to select between gyro self test and CRT 
#define BMI2_GYRO_SELF_TEST_SEL                                 (XBYTE)(0)
#define BMI2_CRT_SEL                                            (XBYTE)(1)

// Accelerometer Macro Definitions               

// Accelerometer Bandwidth parameters 
#define BMI2_ACC_OSR4_AVG1                                      (XBYTE)(0x00)
#define BMI2_ACC_OSR2_AVG2                                      (XBYTE)(0x01)
#define BMI2_ACC_NORMAL_AVG4                                    (XBYTE)(0x02)
#define BMI2_ACC_CIC_AVG8                                       (XBYTE)(0x03)
#define BMI2_ACC_RES_AVG16                                      (XBYTE)(0x04)
#define BMI2_ACC_RES_AVG32                                      (XBYTE)(0x05)
#define BMI2_ACC_RES_AVG64                                      (XBYTE)(0x06)
#define BMI2_ACC_RES_AVG128                                     (XBYTE)(0x07)

// Accelerometer Output Data Rate 
#define BMI2_ACC_ODR_0_78HZ                                     (XBYTE)(0x01)
#define BMI2_ACC_ODR_1_56HZ                                     (XBYTE)(0x02)
#define BMI2_ACC_ODR_3_12HZ                                     (XBYTE)(0x03)
#define BMI2_ACC_ODR_6_25HZ                                     (XBYTE)(0x04)
#define BMI2_ACC_ODR_12_5HZ                                     (XBYTE)(0x05)
#define BMI2_ACC_ODR_25HZ                                       (XBYTE)(0x06)
#define BMI2_ACC_ODR_50HZ                                       (XBYTE)(0x07)
#define BMI2_ACC_ODR_100HZ                                      (XBYTE)(0x08)
#define BMI2_ACC_ODR_200HZ                                      (XBYTE)(0x09)
#define BMI2_ACC_ODR_400HZ                                      (XBYTE)(0x0A)
#define BMI2_ACC_ODR_800HZ                                      (XBYTE)(0x0B)
#define BMI2_ACC_ODR_1600HZ                                     (XBYTE)(0x0C)

// Accelerometer G Range 
#define BMI2_ACC_RANGE_2G                                       (XBYTE)(0x00)
#define BMI2_ACC_RANGE_4G                                       (XBYTE)(0x01)
#define BMI2_ACC_RANGE_8G                                       (XBYTE)(0x02)
#define BMI2_ACC_RANGE_16G                                      (XBYTE)(0x03)

// Mask definitions for accelerometer configuration register 
#define BMI2_ACC_RANGE_MASK                                     (XBYTE)(0x03)
#define BMI2_ACC_ODR_MASK                                       (XBYTE)(0x0F)
#define BMI2_ACC_BW_PARAM_MASK                                  (XBYTE)(0x70)
#define BMI2_ACC_FILTER_PERF_MODE_MASK                          (XBYTE)(0x80)

// Bit position definitions for accelerometer configuration register 
#define BMI2_ACC_BW_PARAM_POS                                   (XBYTE)(0x04)
#define BMI2_ACC_FILTER_PERF_MODE_POS                           (XBYTE)(0x07)

// Self test macro to define range 
#define BMI2_ACC_SELF_TEST_RANGE                                (XBYTE)(16)

// Self test macro to show resulting minimum and maximum difference * signal of the axes in mg
 
#define BMI2_ST_ACC_X_SIG_MIN_DIFF                              (XWORDSIG)(16000)
#define BMI2_ST_ACC_Y_SIG_MIN_DIFF                              (XWORDSIG)(-15000)
#define BMI2_ST_ACC_Z_SIG_MIN_DIFF                              (XWORDSIG)(10000)

// Mask definitions for accelerometer self-test 
#define BMI2_ACC_SELF_TEST_EN_MASK                              (XBYTE)(0x01)
#define BMI2_ACC_SELF_TEST_SIGN_MASK                            (XBYTE)(0x04)
#define BMI2_ACC_SELF_TEST_AMP_MASK                             (XBYTE)(0x08)

// Bit Positions for accelerometer self-test 
#define BMI2_ACC_SELF_TEST_SIGN_POS                             (XBYTE)(0x02)
#define BMI2_ACC_SELF_TEST_AMP_POS                              (XBYTE)(0x03)

// MASK definition for gyro self test status  
#define BMI2_GYR_ST_AXES_DONE_MASK                              (XBYTE)(0X01)
#define BMI2_GYR_AXIS_X_OK_MASK                                 (XBYTE)(0x02)
#define BMI2_GYR_AXIS_Y_OK_MASK                                 (XBYTE)(0x04)
#define BMI2_GYR_AXIS_Z_OK_MASK                                 (XBYTE)(0x08)

// Bit position for gyro self test status  
#define BMI2_GYR_AXIS_X_OK_POS                                  (XBYTE)(0x01)
#define BMI2_GYR_AXIS_Y_OK_POS                                  (XBYTE)(0x02)
#define BMI2_GYR_AXIS_Z_OK_POS                                  (XBYTE)(0x03)

// Gyroscope Macro Definitions               

// Gyroscope Bandwidth parameters 
#define BMI2_GYR_OSR4_MODE                                      (XBYTE)(0x00)
#define BMI2_GYR_OSR2_MODE                                      (XBYTE)(0x01)
#define BMI2_GYR_NORMAL_MODE                                    (XBYTE)(0x02)
#define BMI2_GYR_CIC_MODE                                       (XBYTE)(0x03)

// Gyroscope Output Data Rate 
#define BMI2_GYR_ODR_25HZ                                       (XBYTE)(0x06)
#define BMI2_GYR_ODR_50HZ                                       (XBYTE)(0x07)
#define BMI2_GYR_ODR_100HZ                                      (XBYTE)(0x08)
#define BMI2_GYR_ODR_200HZ                                      (XBYTE)(0x09)
#define BMI2_GYR_ODR_400HZ                                      (XBYTE)(0x0A)
#define BMI2_GYR_ODR_800HZ                                      (XBYTE)(0x0B)
#define BMI2_GYR_ODR_1600HZ                                     (XBYTE)(0x0C)
#define BMI2_GYR_ODR_3200HZ                                     (XBYTE)(0x0D)

// Gyroscope OIS Range 
#define BMI2_GYR_OIS_250                                        (XBYTE)(0x00)
#define BMI2_GYR_OIS_2000                                       (XBYTE)(0x01)

// Gyroscope Angular Rate Measurement Range 
#define BMI2_GYR_RANGE_2000                                     (XBYTE)(0x00)
#define BMI2_GYR_RANGE_1000                                     (XBYTE)(0x01)
#define BMI2_GYR_RANGE_500                                      (XBYTE)(0x02)
#define BMI2_GYR_RANGE_250                                      (XBYTE)(0x03)
#define BMI2_GYR_RANGE_125                                      (XBYTE)(0x04)

// Mask definitions for gyroscope configuration register 
#define BMI2_GYR_RANGE_MASK                                     (XBYTE)(0x07)
#define BMI2_GYR_OIS_RANGE_MASK                                 (XBYTE)(0x08)
#define BMI2_GYR_ODR_MASK                                       (XBYTE)(0x0F)
#define BMI2_GYR_BW_PARAM_MASK                                  (XBYTE)(0x30)
#define BMI2_GYR_NOISE_PERF_MODE_MASK                           (XBYTE)(0x40)
#define BMI2_GYR_FILTER_PERF_MODE_MASK                          (XBYTE)(0x80)

// Bit position definitions for gyroscope configuration register 
#define BMI2_GYR_OIS_RANGE_POS                                  (XBYTE)(0x03)
#define BMI2_GYR_BW_PARAM_POS                                   (XBYTE)(0x04)
#define BMI2_GYR_NOISE_PERF_MODE_POS                            (XBYTE)(0x06)
#define BMI2_GYR_FILTER_PERF_MODE_POS                           (XBYTE)(0x07)

// Auxiliary Output Data Rate 
#define BMI2_AUX_ODR_RESERVED                                   (XBYTE)(0x00)
#define BMI2_AUX_ODR_0_78HZ                                     (XBYTE)(0x01)
#define BMI2_AUX_ODR_1_56HZ                                     (XBYTE)(0x02)
#define BMI2_AUX_ODR_3_12HZ                                     (XBYTE)(0x03)
#define BMI2_AUX_ODR_6_25HZ                                     (XBYTE)(0x04)
#define BMI2_AUX_ODR_12_5HZ                                     (XBYTE)(0x05)
#define BMI2_AUX_ODR_25HZ                                       (XBYTE)(0x06)
#define BMI2_AUX_ODR_50HZ                                       (XBYTE)(0x07)
#define BMI2_AUX_ODR_100HZ                                      (XBYTE)(0x08)
#define BMI2_AUX_ODR_200HZ                                      (XBYTE)(0x09)
#define BMI2_AUX_ODR_400HZ                                      (XBYTE)(0x0A)
#define BMI2_AUX_ODR_800HZ                                      (XBYTE)(0x0B)

// Macro to define burst read lengths for both manual and auto modes 
#define BMI2_AUX_READ_LEN_0                                     (XBYTE)(0x00)
#define BMI2_AUX_READ_LEN_1                                     (XBYTE)(0x01)
#define BMI2_AUX_READ_LEN_2                                     (XBYTE)(0x02)
#define BMI2_AUX_READ_LEN_3                                     (XBYTE)(0x03)

// Mask definitions for auxiliary interface configuration register 
#define BMI2_AUX_SET_I2C_ADDR_MASK                              (XBYTE)(0xFE)
#define BMI2_AUX_MAN_MODE_EN_MASK                               (XBYTE)(0x80)
#define BMI2_AUX_FCU_WR_EN_MASK                                 (XBYTE)(0x40)
#define BMI2_AUX_MAN_READ_BURST_MASK                            (XBYTE)(0x0C)
#define BMI2_AUX_READ_BURST_MASK                                (XBYTE)(0x03)
#define BMI2_AUX_ODR_EN_MASK                                    (XBYTE)(0x0F)
#define BMI2_AUX_OFFSET_READ_OUT_MASK                           (XBYTE)(0xF0)

// Bit positions for auxiliary interface configuration register 
#define BMI2_AUX_SET_I2C_ADDR_POS                               (XBYTE)(0x01)
#define BMI2_AUX_MAN_MODE_EN_POS                                (XBYTE)(0x07)
#define BMI2_AUX_FCU_WR_EN_POS                                  (XBYTE)(0x06)
#define BMI2_AUX_MAN_READ_BURST_POS                             (XBYTE)(0x02)
#define BMI2_AUX_OFFSET_READ_OUT_POS                            (XBYTE)(0x04)


// FIFO Macro Definitions                                        


// Macros to define virtual FIFO frame mode 
#define BMI2_FIFO_VIRT_FRM_MODE                                 (XBYTE)(0x03)

// FIFO Header Mask definitions 
#define BMI2_FIFO_HEADER_ACC_FRM                                (XBYTE)(0x84)
#define BMI2_FIFO_HEADER_AUX_FRM                                (XBYTE)(0x90)
#define BMI2_FIFO_HEADER_GYR_FRM                                (XBYTE)(0x88)
#define BMI2_FIFO_HEADER_GYR_ACC_FRM                            (XBYTE)(0x8C)
#define BMI2_FIFO_HEADER_AUX_ACC_FRM                            (XBYTE)(0x94)
#define BMI2_FIFO_HEADER_AUX_GYR_FRM                            (XBYTE)(0x98)
#define BMI2_FIFO_HEADER_ALL_FRM                                (XBYTE)(0x9C)
#define BMI2_FIFO_HEADER_SENS_TIME_FRM                          (XBYTE)(0x44)
#define BMI2_FIFO_HEADER_SKIP_FRM                               (XBYTE)(0x40)
#define BMI2_FIFO_HEADER_INPUT_CFG_FRM                          (XBYTE)(0x48)
#define BMI2_FIFO_HEAD_OVER_READ_MSB                            (XBYTE)(0x80)
#define BMI2_FIFO_VIRT_ACT_RECOG_FRM                            (XBYTE)(0xC8)

// BMI2 sensor selection for header-less frames  
#define BMI2_FIFO_HEAD_LESS_ACC_FRM                             (XBYTE)(0x40)
#define BMI2_FIFO_HEAD_LESS_AUX_FRM                             (XBYTE)(0x20)
#define BMI2_FIFO_HEAD_LESS_GYR_FRM                             (XBYTE)(0x80)
#define BMI2_FIFO_HEAD_LESS_GYR_AUX_FRM                         (XBYTE)(0xA0)
#define BMI2_FIFO_HEAD_LESS_GYR_ACC_FRM                         (XBYTE)(0xC0)
#define BMI2_FIFO_HEAD_LESS_AUX_ACC_FRM                         (XBYTE)(0x60)
#define BMI2_FIFO_HEAD_LESS_ALL_FRM                             (XBYTE)(0xE0)

// Mask definitions for FIFO frame content configuration 
#define BMI2_FIFO_STOP_ON_FULL                                  (XWORD)(0x0001)
#define BMI2_FIFO_TIME_EN                                       (XWORD)(0x0002)
#define BMI2_FIFO_TAG_INT1                                      (XWORD)(0x0300)
#define BMI2_FIFO_TAG_INT2                                      (XWORD)(0x0C00)
#define BMI2_FIFO_HEADER_EN                                     (XWORD)(0x1000)
#define BMI2_FIFO_AUX_EN                                        (XWORD)(0x2000)
#define BMI2_FIFO_ACC_EN                                        (XWORD)(0x4000)
#define BMI2_FIFO_GYR_EN                                        (XWORD)(0x8000)
#define BMI2_FIFO_ALL_EN                                        (XWORD)(0xE000)

// FIFO sensor data lengths 
#define BMI2_FIFO_ACC_LENGTH                                    (XBYTE)(6)
#define BMI2_FIFO_GYR_LENGTH                                    (XBYTE)(6)
#define BMI2_FIFO_AUX_LENGTH                                    (XBYTE)(8)
#define BMI2_FIFO_ACC_AUX_LENGTH                                (XBYTE)(14)
#define BMI2_FIFO_GYR_AUX_LENGTH                                (XBYTE)(14)
#define BMI2_FIFO_ACC_GYR_LENGTH                                (XBYTE)(12)
#define BMI2_FIFO_ALL_LENGTH                                    (XBYTE)(20)
#define BMI2_SENSOR_TIME_LENGTH                                 (XBYTE)(3)
#define BMI2_FIFO_CONFIG_LENGTH                                 (XBYTE)(2)
#define BMI2_FIFO_WM_LENGTH                                     (XBYTE)(2)
#define BMI2_MAX_VALUE_FIFO_FILTER                              (XBYTE)(1)
#define BMI2_FIFO_DATA_LENGTH                                   (XBYTE)(2)
#define BMI2_FIFO_LENGTH_MSB_BYTE                               (XBYTE)(1)
#define BMI2_FIFO_INPUT_CFG_LENGTH                              (XBYTE)(4)
#define BMI2_FIFO_SKIP_FRM_LENGTH                               (XBYTE)(1)

// FIFO sensor virtual data lengths: sensor data plus sensor time 
#define BMI2_FIFO_VIRT_ACC_LENGTH                               (XBYTE)(9)
#define BMI2_FIFO_VIRT_GYR_LENGTH                               (XBYTE)(9)
#define BMI2_FIFO_VIRT_AUX_LENGTH                               (XBYTE)(11)
#define BMI2_FIFO_VIRT_ACC_AUX_LENGTH                           (XBYTE)(17)
#define BMI2_FIFO_VIRT_GYR_AUX_LENGTH                           (XBYTE)(17)
#define BMI2_FIFO_VIRT_ACC_GYR_LENGTH                           (XBYTE)(15)
#define BMI2_FIFO_VIRT_ALL_LENGTH                               (XBYTE)(23)

// FIFO sensor virtual data lengths: activity recognition 
#define BMI2_FIFO_VIRT_ACT_DATA_LENGTH                          (XBYTE)(6)
#define BMI2_FIFO_VIRT_ACT_TIME_LENGTH                          (XBYTE)(4)
#define BMI2_FIFO_VIRT_ACT_TYPE_LENGTH                          (XBYTE)(1)
#define BMI2_FIFO_VIRT_ACT_STAT_LENGTH                          (XBYTE)(1)

// BMI2 FIFO data filter modes 
#define BMI2_FIFO_UNFILTERED_DATA                               (XBYTE)(0)
#define BMI2_FIFO_FILTERED_DATA                                 (XBYTE)(1)

// FIFO frame masks 
#define BMI2_FIFO_LSB_CONFIG_CHECK                              (XBYTE)(0x00)
#define BMI2_FIFO_MSB_CONFIG_CHECK                              (XBYTE)(0x80)
#define BMI2_FIFO_TAG_INTR_MASK                                 (XBYTE)(0xFF)

// BMI2 Mask definitions of FIFO configuration registers 
#define BMI2_FIFO_CONFIG_0_MASK                                 (XWORD)(0x0003)
#define BMI2_FIFO_CONFIG_1_MASK                                 (XWORD)(0xFF00)

// FIFO self wake-up mask definition 
#define BMI2_FIFO_SELF_WAKE_UP_MASK                             (XBYTE)(0x02)

// FIFO down sampling mask definition 
#define BMI2_ACC_FIFO_DOWNS_MASK                                (XBYTE)(0x70)
#define BMI2_GYR_FIFO_DOWNS_MASK                                (XBYTE)(0x07)

// FIFO down sampling bit positions 
#define BMI2_ACC_FIFO_DOWNS_POS                                 (XBYTE)(0x04)

// FIFO filter mask definition 
#define BMI2_ACC_FIFO_FILT_DATA_MASK                            (XBYTE)(0x80)
#define BMI2_GYR_FIFO_FILT_DATA_MASK                            (XBYTE)(0x08)

// FIFO filter bit positions 
#define BMI2_ACC_FIFO_FILT_DATA_POS                             (XBYTE)(0x07)
#define BMI2_GYR_FIFO_FILT_DATA_POS                             (XBYTE)(0x03)

// FIFO byte counter mask definition 
#define BMI2_FIFO_BYTE_COUNTER_MSB_MASK                         (XBYTE)(0x3F)

// FIFO self wake-up bit positions 
#define BMI2_FIFO_SELF_WAKE_UP_POS                              (XBYTE)(0x01)

// Mask Definitions for Virtual FIFO frames 
#define BMI2_FIFO_VIRT_FRM_MODE_MASK                            (XBYTE)(0xC0)
#define BMI2_FIFO_VIRT_PAYLOAD_MASK                             (XBYTE)(0x3C)

// Bit Positions for Virtual FIFO frames 
#define BMI2_FIFO_VIRT_FRM_MODE_POS                             (XBYTE)(0x06)
#define BMI2_FIFO_VIRT_PAYLOAD_POS                              (XBYTE)(0x02)


// Interrupt Macro Definitions                  


// BMI2 Interrupt Modes 
// Non latched 
#define BMI2_INT_NON_LATCH                                      (XBYTE)(0)

// Permanently latched 
#define BMI2_INT_LATCH                                          (XBYTE)(1)

// BMI2 Interrupt Pin Behavior 
#define BMI2_INT_PUSH_PULL                                      (XBYTE)(0)
#define BMI2_INT_OPEN_DRAIN                                     (XBYTE)(1)

// BMI2 Interrupt Pin Level 
#define BMI2_INT_ACTIVE_LOW                                     (XBYTE)(0)
#define BMI2_INT_ACTIVE_HIGH                                    (XBYTE)(1)

// BMI2 Interrupt Output Enable 
#define BMI2_INT_OUTPUT_DISABLE                                 (XBYTE)(0)
#define BMI2_INT_OUTPUT_ENABLE                                  (XBYTE)(1)

// BMI2 Interrupt Input Enable 
#define BMI2_INT_INPUT_DISABLE                                  (XBYTE)(0)
#define BMI2_INT_INPUT_ENABLE                                   (XBYTE)(1)

// Mask definitions for interrupt pin configuration 
#define BMI2_INT_LATCH_MASK                                     (XBYTE)(0x01)
#define BMI2_INT_LEVEL_MASK                                     (XBYTE)(0x02)
#define BMI2_INT_OPEN_DRAIN_MASK                                (XBYTE)(0x04)
#define BMI2_INT_OUTPUT_EN_MASK                                 (XBYTE)(0x08)
#define BMI2_INT_INPUT_EN_MASK                                  (XBYTE)(0x10)

// Bit position definitions for interrupt pin configuration 
#define BMI2_INT_LEVEL_POS                                      (XBYTE)(0x01)
#define BMI2_INT_OPEN_DRAIN_POS                                 (XBYTE)(0x02)
#define BMI2_INT_OUTPUT_EN_POS                                  (XBYTE)(0x03)
#define BMI2_INT_INPUT_EN_POS                                   (XBYTE)(0x04)

// Mask definitions for data interrupt mapping 
#define BMI2_FFULL_INT                                          (XBYTE)(0x01)
#define BMI2_FWM_INT                                            (XBYTE)(0x02)
#define BMI2_DRDY_INT                                           (XBYTE)(0x04)
#define BMI2_ERR_INT                                            (XBYTE)(0x08)

// Mask definitions for data interrupt status bits 
#define BMI2_FFULL_INT_STATUS_MASK                              (XWORD)(0x0100)
#define BMI2_FWM_INT_STATUS_MASK                                (XWORD)(0x0200)
#define BMI2_ERR_INT_STATUS_MASK                                (XWORD)(0x0400)
#define BMI2_AUX_DRDY_INT_MASK                                  (XWORD)(0x2000)
#define BMI2_GYR_DRDY_INT_MASK                                  (XWORD)(0x4000)
#define BMI2_ACC_DRDY_INT_MASK                                  XWORD)(0x8000)

// Maximum number of interrupt pins 
#define BMI2_INT_PIN_MAX_NUM                                    (XBYTE)(2)

// Macro for mapping feature interrupts 
#define BMI2_FEAT_BIT_DISABLE                                   (XBYTE)(0)
#define BMI2_FEAT_BIT0                                          (XBYTE)(1)
#define BMI2_FEAT_BIT1                                          (XBYTE)(2)
#define BMI2_FEAT_BIT2                                          (XBYTE)(3)
#define BMI2_FEAT_BIT3                                          (XBYTE)(4)
#define BMI2_FEAT_BIT4                                          (XBYTE)(5)
#define BMI2_FEAT_BIT5                                          (XBYTE)(6)
#define BMI2_FEAT_BIT6                                          (XBYTE)(7)
#define BMI2_FEAT_BIT7                                          (XBYTE)(8)
#define BMI2_FEAT_BIT_MAX                                       (XBYTE)(9)


// OIS Interface Macro Definitions                       

// Mask definitions for interface configuration register 
#define BMI2_OIS_IF_EN_MASK                                     (XBYTE)(0x10)
#define BMI2_AUX_IF_EN_MASK                                     (XBYTE)(0x20)

// Bit positions for OIS interface enable 
#define BMI2_OIS_IF_EN_POS                                      (XBYTE)(0x04)
#define BMI2_AUX_IF_EN_POS                                      (XBYTE)(0x05)


// Macro Definitions for Axes re-mapping             

// Macros for the user-defined values of axes and their polarities 
#define BMI2_X                                                  (XBYTE)(0x01)
#define BMI2_NEG_X                                              (XBYTE)(0x09)
#define BMI2_Y                                                  (XBYTE)(0x02)
#define BMI2_NEG_Y                                              (XBYTE)(0x0A)
#define BMI2_Z                                                  (XBYTE)(0x04)
#define BMI2_NEG_Z                                              (XBYTE)(0x0C)
#define BMI2_AXIS_MASK                                          (XBYTE)(0x07)
#define BMI2_AXIS_SIGN                                          (XBYTE)(0x08)


// Macro Definitions for offset and gain compensation          

// Mask definitions of gyroscope offset compensation registers 
#define BMI2_GYR_GAIN_EN_MASK                                   (XBYTE)(0x80)
#define BMI2_GYR_OFF_COMP_EN_MASK                               (XBYTE)(0x40)

// Bit positions of gyroscope offset compensation registers 
#define BMI2_GYR_OFF_COMP_EN_POS                                (XBYTE)(0x06)

// Mask definitions of gyroscope user-gain registers 
#define BMI2_GYR_USR_GAIN_X_MASK                                (XBYTE)(0x7F)
#define BMI2_GYR_USR_GAIN_Y_MASK                                (XBYTE)(0x7F)
#define BMI2_GYR_USR_GAIN_Z_MASK                                (XBYTE)(0x7F)

// Bit positions of gyroscope offset compensation registers 
#define BMI2_GYR_GAIN_EN_POS                                    (XBYTE)(0x07)

// Macro Definitions for internal status                 

#define BMI2_NOT_INIT                                           (XBYTE)(0x00)
#define BMI2_INIT_OK                                            (XBYTE)(0x01)
#define BMI2_INIT_ERR                                           (XBYTE)(0x02)
#define BMI2_DRV_ERR                                            (XBYTE)(0x03)
#define BMI2_SNS_STOP                                           (XBYTE)(0x04)
#define BMI2_NVM_ERROR                                          (XBYTE)(0x05)
#define BMI2_START_UP_ERROR                                     (XBYTE)(0x06)
#define BMI2_COMPAT_ERROR                                       (XBYTE)(0x07)
#define BMI2_VFM_SKIPPED                                        (XBYTE)(0x10)
#define BMI2_AXES_MAP_ERROR                                     (XBYTE)(0x20)
#define BMI2_ODR_50_HZ_ERROR                                    (XBYTE)(0x40)
#define BMI2_ODR_HIGH_ERROR                                     (XBYTE)(0x80)


// Error status form gyro gain update status.               

#define BMI2_G_TRIGGER_NO_ERROR                                 (XBYTE)(0x00)

#define BMI2_G_TRIGGER_PRECON_ERROR                             (XBYTE)(0x01)
#define BMI2_G_TRIGGER_DL_ERROR                                 (XBYTE)(0x02)
#define BMI2_G_TRIGGER_ABORT_ERROR                              (XBYTE)(0x03)

// Variant specific features selection macros            

#define BMI2_CRT_RTOSK_ENABLE                                   (XBYTE)(0x01)
#define BMI2_GYRO_CROSS_SENS_ENABLE                             (XBYTE)(0x02)
#define BMI2_GYRO_USER_GAIN_ENABLE                              (XBYTE)(0x08)
#define BMI2_NO_FEATURE_ENABLE                                  (XBYTE)(0x00)
#define BMI2_CRT_IN_FIFO_NOT_REQ                                (XBYTE)(0x10)
#define BMI2_MAXIMUM_FIFO_VARIANT                               (XBYTE)(0x20)

// Pull-up configuration for ASDA               
#define BMI2_ASDA_PUPSEL_OFF                                    (XBYTE)(0x00)
#define BMI2_ASDA_PUPSEL_40K                                    (XBYTE)(0x01)
#define BMI2_ASDA_PUPSEL_10K                                    (XBYTE)(0x02)
#define BMI2_ASDA_PUPSEL_2K                                     (XBYTE)(0x03)



// Macro definitions                                         
#define BMI2XY_SHUTTLE_ID                                       (XWORD)(0x1B8)

// Macro that defines read write length 
#define READ_WRITE_LEN                                          (XBYTE)(46)


// Function Pointers                             

/*!
 * @brief Bus communication function pointer which should be mapped to
 * the platform specific read functions of the user
 *
 * @param[in] reg_addr       : Register address from which data is read.
 * @param[out] reg_data     : Pointer to data buffer where read data is stored.
 * @param[in] len            : Number of bytes of data to be read.
 * @param[in, out] intf_ptr  : Void pointer that can enable the linking of descriptors
 *                                  for interface related call backs.
 *
 *  retval =  BMA4_INTF_RET_SUCCESS -> Success
 *  retval != BMA4_INTF_RET_SUCCESS -> Failure
 *
 */
typedef BMI2_INTF_RETURN_TYPE (*bmi2_read_fptr_t)(XBYTE reg_addr, XBYTE *reg_data, XDWORD len, void *intf_ptr);

/*!
 * @brief Bus communication function pointer which should be mapped to
 * the platform specific write functions of the user
 *
 * @param[in] reg_addr      : Register address to which the data is written.
 * @param[in] reg_data     : Pointer to data buffer in which data to be written
 *                            is stored.
 * @param[in] len           : Number of bytes of data to be written.
 * @param[in, out] intf_ptr : Void pointer that can enable the linking of descriptors
 *                            for interface related call backs
 *
 * retval  = BMA4_INTF_RET_SUCCESS -> Success
 * retval != BMA4_INTF_RET_SUCCESS -> Failure
 *
 */
typedef BMI2_INTF_RETURN_TYPE (*bmi2_write_fptr_t)(XBYTE reg_addr, const XBYTE *reg_data, XDWORD len,
                                                   void *intf_ptr);

/*!
 * @brief Delay function pointer which should be mapped to
 * delay function of the user
 *
 * @param[in] period              : Delay in microseconds.
 * @param[in, out] intf_ptr       : Void pointer that can enable the linking of descriptors
 *                                  for interface related call backs
 *
 */
typedef void (*bmi2_delay_fptr_t)(XDWORD period, void *intf_ptr);

/*!
 * @brief To get the configurations for wake_up feature, since wakeup feature is different for bmi260 and bmi261.
 *
 * @param[out]      wake_up    : Void pointer to store bmi2_wake_up_config structure.
 * @param[in, out]  bmi2_dev   : Void pointer to store bmi2_dev structure.
 *
 * @return Result of API execution status
 *
 * @retval BMI2_OK - Success.
 * @retval BMI2_E_COM_FAIL - Error: Communication fail
 * @retval BMI2_E_NULL_PTR - Error: Null pointer error
 * @retval BMI2_E_INVALID_PAGE - Error: Invalid Page
 *
 */
typedef XBYTESIG (*bmi2_wake_up_fptr_t)(void *wake_up, void *bmi2_dev);

/*!
 * @brief To get the configurations for tap feature.
 *
 * @param[out]      tap        : Void pointer to store bmi2_tap_config structure.
 * @param[in, out]  bmi2_dev   : Void pointer to store bmi2_dev structure.
 *
 * @return Result of API execution status
 *
 * @retval BMI2_OK - Success.
 * @retval BMI2_E_COM_FAIL - Error: Communication fail
 * @retval BMI2_E_NULL_PTR - Error: Null pointer error
 * @retval BMI2_E_INVALID_PAGE - Error: Invalid Page
 *
 */
typedef XBYTESIG (*bmi2_tap_fptr_t)(void *tap, void *bmi2_dev);

// Enum Declarations                                  

// Enum to define BMI2 sensor interfaces */
enum bmi2_intf 
{
  BMI2_SPI_INTF           = 0 ,
  BMI2_I2C_INTF               ,
  BMI2_I3C_INTF
};

// Enum to define BMI2 sensor configuration errors for accelerometer and gyroscope
enum bmi2_sensor_config_error 
 {
  BMI2_NO_ERROR               ,
  BMI2_ACC_ERROR              ,
  BMI2_GYR_ERROR              ,
  BMI2_ACC_GYR_ERROR
};

// Enum to define interrupt lines 
enum bmi2_hw_int_pin 
{
  BMI2_INT_NONE               ,
  BMI2_INT1                   ,
  BMI2_INT2                   ,
  BMI2_INT_BOTH               ,
  BMI2_INT_PIN_MAX
};

// Enum for the position of the wearable device 
enum bmi2_wear_arm_pos 
{
  BMI2_ARM_LEFT               ,
  BMI2_ARM_RIGHT
};

// Enum to display type of activity recognition 
enum bmi2_act_recog_type 
{
  BMI2_ACT_UNKNOWN            ,
  BMI2_ACT_STILL              ,
  BMI2_ACT_WALK               ,
  BMI2_ACT_RUN                ,
  BMI2_ACT_BIKE               ,
  BMI2_ACT_VEHICLE            ,
  BMI2_ACT_TILTED
};

// Enum to display activity recognition status 
enum bmi2_act_recog_stat 
{
  BMI2_ACT_START          = 1 ,
  BMI2_ACT_END
};




// Structure Declarations                            

// Structure to store the compensated user-gain data of gyroscope 
struct bmi2_gyro_user_gain_data
{   
  XBYTESIG                                      x;
  XBYTESIG                                      y;
  XBYTESIG                                      z;
};

// Structure to store the re-mapped axis 
struct bmi2_remap
{    
  XBYTE                                         x;
  XBYTE                                         y;
  XBYTE                                         z;
};

// Structure to store the value of re-mapped axis and its sign 
struct bmi2_axes_remap
{  
  XBYTE                                         x_axis;
  XBYTE                                         y_axis;
  XBYTE                                         z_axis;
  XBYTESIG                                      x_axis_sign;
  XBYTESIG                                      y_axis_sign;
  XBYTESIG                                      z_axis_sign;
};

// Structure to define the type of sensor and its interrupt pin 
struct bmi2_sens_int_config
{
  XBYTE                                         type;
  enum bmi2_hw_int_pin                          hw_int_pin;
};

// Structure to define output for activity recognition 
struct bmi2_act_recog_output
{   
  XDWORD                                        time_stamp;
  XBYTE                                         curr_act;
  XBYTE                                         prev_act;
};

// Structure to define FIFO frame configuration 
struct bmi2_fifo_frame
{   
  XBYTE*                                        data;
  XWORD                                         length;
  XBYTE                                         header_enable;
  XWORD                                         data_enable;
  XWORD                                         acc_byte_start_idx;
  XWORD                                         act_recog_byte_start_idx;
  XWORD                                         aux_byte_start_idx;
  XWORD                                         gyr_byte_start_idx;
  XDWORD                                        sensor_time;
  XBYTE                                         skipped_frame_count;
  XBYTE                                         data_int_map;
  XWORD                                         wm_lvl;
  XBYTE                                         acc_frm_len;
  XBYTE                                         gyr_frm_len;
  XBYTE                                         aux_frm_len;
  XBYTE                                         acc_gyr_frm_len;
  XBYTE                                         acc_aux_frm_len;
  XBYTE                                         aux_gyr_frm_len;
  XBYTE                                         all_frm_len;
};


// Structure to define Interrupt pin configuration 
struct bmi2_int_pin_cfg
{   
  XBYTE                                         lvl;
  XBYTE                                         od;
  XBYTE                                         output_en;
  XBYTE                                         input_en;
};


// Structure to define interrupt pin type, mode and configurations 
struct bmi2_int_pin_config
{    
  XBYTE                                         pin_type;
  XBYTE                                         int_latch;
  struct bmi2_int_pin_cfg                       pin_cfg[BMI2_INT_PIN_MAX_NUM];
};


// Structure to define an array of 8 auxiliary data bytes 
struct bmi2_aux_fifo_data
{  
  XBYTE                                         data[8];
  XDWORD                                        virt_sens_time;
};


// Structure to define accelerometer and gyroscope sensor axes and sensor time for virtual frames 
struct bmi2_sens_axes_data
{  
  XWORDSIG                                      x;
  XWORDSIG                                      y;
  XWORDSIG                                      z;
  XDWORD                                        virt_sens_time;
};


// Structure to define gyroscope saturation status of user gain 
struct bmi2_gyr_user_gain_status
{   
  XBYTE                                         sat_x;
  XBYTE                                         sat_y;
  XBYTE                                         sat_z;
  XBYTE                                         g_trigger_status;
};


// Structure to store the status of gyro self test result 
struct bmi2_gyro_self_test_status
{    
  XBYTE                                         gyr_st_axes_done  : 1;
  XBYTE                                         gyr_axis_x_ok     : 1;
  XBYTE                                         gyr_axis_y_ok     : 1;
  XBYTE                                         gyr_axis_z_ok     : 1;
};


// Structure to define NVM error status 
struct bmi2_nvm_err_status
{   
  XBYTE                                         load_error;
  XBYTE                                         prog_error;
  XBYTE                                         erase_error;
  XBYTE                                         exceed_error;
  XBYTE                                         privil_error;
};

// Structure to define VFRM error status 
struct bmi2_vfrm_err_status
{
  XBYTE                                         lock_error;
  XBYTE                                         write_error;
  XBYTE                                         fatal_error;
};

// Structure to define accelerometer self test feature status 
struct bmi2_acc_self_test_status
{    
  XBYTE                                         acc_self_test_done;
  XBYTE                                         acc_x_ok;
  XBYTE                                         acc_y_ok;
  XBYTE                                         acc_z_ok;
};


// Structure to define orientation output 
struct bmi2_orientation_output
{    
  XBYTE                                         portrait_landscape;
  XBYTE                                         faceup_down;
};


// Structure to define OIS output 
struct bmi2_ois_output
{    
  XWORDSIG                                      ois_acc_x;
  XWORDSIG                                      ois_acc_y;
  XWORDSIG                                      ois_acc_z;
  XWORDSIG                                      ois_gyro_x;
  XWORDSIG                                      ois_gyro_y;
  XWORDSIG                                      ois_gyro_z;
};


// Union to define BMI2 sensor data 
union bmi2_sens_data
{    
  struct bmi2_sens_axes_data                    acc;                                                                        // Accelerometer axes data
  struct bmi2_sens_axes_data                    gyr;                                                                        // Gyroscope axes data
  XBYTE                                         aux_data[BMI2_AUX_NUM_BYTES];                                               // Auxiliary sensor data 
  XDWORD                                        step_counter_output;                                                        // Step counter output
  XBYTE                                         activity_output;                                                            // Step activity output
  struct bmi2_orientation_output                orient_output;                                                              // Orientation output
  XBYTE                                         high_g_output;                                                              // High-g output
  struct bmi2_gyr_user_gain_status              gyro_user_gain_status;                                                      // Gyroscope user gain saturation status
  struct bmi2_nvm_err_status                    nvm_status;                                                                 // NVM error status
  struct bmi2_vfrm_err_status                   vfrm_status;                                                                // Virtual frame error status
  XBYTE                                         wrist_gesture_output;                                                       // Wrist gesture output
  XWORDSIG                                      correction_factor_zx;                                                       // Gyroscope cross sense value of z axis
  struct bmi2_acc_self_test_status              accel_self_test_output;                                                     // Accelerometer self test feature status    
  struct bmi2_ois_output                        ois_output;                                                                 // OIS output 
};


// Structure to define type of sensor and their respective data 
struct bmi2_sensor_data
{   
  XBYTE                                         type;
  union bmi2_sens_data                          sens_data;
};


// Structure to define accelerometer configuration 
struct bmi2_accel_config
{   
  XBYTE                                         odr;
  XBYTE                                         bwp;
  XBYTE                                         filter_perf;
  XBYTE                                         range;
};

// Structure to define gyroscope configuration 
struct bmi2_gyro_config
{    
  XBYTE                                         odr;
  XBYTE                                         bwp;
  XBYTE                                         filter_perf;
  XBYTE                                         ois_range;
  XBYTE                                         range;
  XBYTE                                         noise_perf;
};

// Structure to define auxiliary sensor configuration 
struct bmi2_aux_config
{  
  XBYTE                                         aux_en;
  XBYTE                                         manual_en;
  XBYTE                                         fcu_write_en;
  XBYTE                                         man_rd_burst;
  XBYTE                                         aux_rd_burst;
  XBYTE                                         odr;
  XBYTE                                         offset;
  XBYTE                                         i2c_device_addr;
  XBYTE                                         read_addr;
};

// Structure to define any-motion configuration 
struct bmi2_any_motion_config
{   
  XWORD                                         duration;
  XWORD                                         threshold;
  XWORD                                         select_x;
  XWORD                                         select_y;
  XWORD                                         select_z;
};

// Structure to define no-motion configuration 
struct bmi2_no_motion_config
{   
  XWORD                                         duration;
  XWORD                                         threshold;
  XWORD                                         select_x;
  XWORD                                         select_y;
  XWORD                                         select_z;
};

// Structure to define sig-motion configuration 
struct bmi2_sig_motion_config
{  
  XWORD                                         block_size;
  XWORD                                         param_2;
  XWORD                                         param_3;
  XWORD                                         param_4;
  XWORD                                         param_5;
};


// Structure to define step counter/detector/activity configuration 
struct bmi2_step_config
{    
  XWORD                                         watermark_level;
  XWORD                                         reset_counter;
  XBYTE                                         step_buffer_size;
};

// Structure to define gyroscope user gain configuration 
struct bmi2_gyro_user_gain_config
{
  XWORD                                         ratio_x;
  XWORD                                         ratio_y;
  XWORD                                         ratio_z;
};

// Structure to define wake-up configuration 
struct bmi2_wake_up_config
{    
  XWORD                                         sensitivity;
  XWORD                                         single_tap_en;
  XWORD                                         data_reg_en;
  XWORD                                         tap_sens_thres;
  XWORD                                         max_gest_dur;
  XWORD                                         quite_time_after_gest;
  XWORD                                         wait_for_timeout;
  XWORD                                         axis_sel;
};

// Structure to define tap configuration 
struct bmi2_tap_config
{   
  XWORD                                         sensitivity;
  XWORD                                         single_tap_en;
  XWORD                                         data_reg_en;
  XWORD                                         tap_sens_thres;
  XWORD                                         max_gest_dur;
  XWORD                                         quite_time_after_gest;
  XWORD                                         wait_for_timeout;
  XWORD                                         axis_sel;
};

// Structure to define orientation configuration 
struct bmi2_orient_config
{   
  XWORD                                         ud_en;
  XWORD                                         mode;
  XWORD                                         blocking;
  XWORD                                         theta;
  XWORD                                         hysteresis;
};

// Structure to define high-g configuration 
struct bmi2_high_g_config
{    
  XWORD                                         threshold;
  XWORD                                         hysteresis;
  XWORD                                         select_x;
  XWORD                                         select_y;
  XWORD                                         select_z;
  XWORD                                         duration;
};


// Structure to define low-g configuration 
struct bmi2_low_g_config
{    
  XWORD                                         threshold;
  XWORD                                         hysteresis;
  XWORD                                         duration;
};


// Structure to define flat configuration 
struct bmi2_flat_config
{    
  XWORD                                         theta;
  XWORD                                         blocking;
  XWORD                                         hysteresis;
  XWORD                                         hold_time;
};


// Structure to define wrist gesture configuration 
struct bmi2_wrist_gest_config
{    
  XWORD                                         wearable_arm;
  XWORD                                         min_flick_peak;
  XWORD                                         min_flick_samples;
  XWORD                                         max_duration;
};


// Structure to define wrist wear wake-up configuration 
struct bmi2_wrist_wear_wake_up_config
{    
  XWORD                                         min_angle_focus;
  XWORD                                         min_angle_nonfocus;
  XWORD                                         max_tilt_lr;
  XWORD                                         max_tilt_ll;
  XWORD                                         max_tilt_pd;
  XWORD                                         max_tilt_pu;
};

// Structure to define wrist gesture configuration for wearable variant 
struct bmi2_wrist_gest_w_config
{   
  XBYTE                                         device_position;
  XWORD                                         min_flick_peak_y_threshold;
  XWORD                                         min_flick_peak_z_threshold;
  XWORD                                         gravity_bounds_x_pos;
  XWORD                                         gravity_bounds_x_neg;
  XWORD                                         gravity_bounds_y_neg;
  XWORD                                         gravity_bounds_z_neg;
  XWORD                                         flick_peak_decay_coeff;
  XWORD                                         lp_mean_filter_coeff;
  XWORD                                         max_duration_jiggle_peaks;
};

// Structure to define wrist wear wake-up configuration for wearable configuration 
struct bmi2_wrist_wear_wake_up_wh_config
{    
  XWORD                                         min_angle_focus;
  XWORD                                         min_angle_nonfocus;
  XBYTE                                         angle_lr;
  XBYTE                                         angle_ll;
  XBYTE                                         angle_pd;
  XBYTE                                         angle_pu;
  XBYTE                                         min_dur_mov;
  XBYTE                                         min_dur_quite;
};

// Structure to define primary OIS configuration 
struct bmi2_primary_ois_config
{
  XBYTE                                         lp_filter_enable;
  XBYTE                                         lp_filter_config;
  XBYTE                                         primary_ois_reserved;
  XBYTE                                         primary_ois_gyro_en;
  XBYTE                                         primary_ois_accel_en;
};

// Structure to configure free-fall detection settings 
struct bmi2_free_fall_det_config
{
  XWORD                                         freefall_accel_settings[BMI2_FREE_FALL_ACCEL_SET_PARAMS];
};


// Union to define the sensor configurations 
union bmi2_sens_config_types
{    
  struct bmi2_accel_config                      acc;                                                                        // Accelerometer configuration 
  struct bmi2_gyro_config                       gyr;                                                                        // Gyroscope configuration 
  struct bmi2_aux_config                        aux;                                                                        // Auxiliary configuration 
  struct bmi2_any_motion_config                 any_motion;                                                                 // Any-motion configuration 
  struct bmi2_no_motion_config                  no_motion;                                                                  // No-motion configuration
  struct bmi2_sig_motion_config                 sig_motion;                                                                 // Sig_motion configuration
  XWORD                                         step_counter_params[BMI2_STEP_CNT_N_PARAMS];                                // Step counter parameter configuration 
  struct bmi2_step_config                       step_counter;                                                               // Step counter/detector/activity configuration 
  struct bmi2_gyro_user_gain_config             gyro_gain_update;                                                           // Gyroscope user gain configuration
  struct bmi2_wake_up_config                    tap;                                                                        // Wake-up configuration
  struct bmi2_tap_config                        tap_conf;                                                                   // Tap configuration
  struct bmi2_orient_config                     orientation;                                                                // Orientation configuration
  struct bmi2_high_g_config                     high_g;                                                                     // High-g configuration
  struct bmi2_low_g_config                      low_g;                                                                      // Low-g configuration
  struct bmi2_flat_config                       flat;                                                                       // Flat configuration
  struct bmi2_wrist_gest_config                 wrist_gest;                                                                 // Wrist gesture configuration
  struct bmi2_wrist_wear_wake_up_config         wrist_wear_wake_up;                                                         // Wrist wear wake-up configuration
  struct bmi2_wrist_gest_w_config               wrist_gest_w;                                                               // Wrist gesture configuration for wearable variant 
  struct bmi2_wrist_wear_wake_up_wh_config      wrist_wear_wake_up_wh;                                                      // Wrist wear wake-up configuration for wearable variant 
  struct bmi2_primary_ois_config                primary_ois;                                                                // Primary OIS configuration
  struct bmi2_free_fall_det_config              free_fall_det;                                                              // Free-fall detection configurations 
};

// Structure to define the type of the sensor and its configurations  
struct bmi2_sens_config
{    
  XBYTE                                         type;
  union bmi2_sens_config_types                  cfg;
};


// Structure to define the feature configuration 
struct bmi2_feature_config
{    
  XBYTE                                         type;
  XBYTE                                         page;
  XBYTE                                         start_addr;
};


// Structure to define the feature interrupt configurations 
struct bmi2_map_int
{
  XBYTE                                         type;
  XBYTE                                         sens_map_int;
};


// Structure to define BMI2 sensor configurations 
struct bmi2_dev
{   
  XBYTE                                         chip_id;
  void*                                         intf_ptr;
  XBYTE                                         info;
  enum bmi2_intf                                intf;
  BMI2_INTF_RETURN_TYPE                         intf_rslt;
  XBYTE                                         dummy_byte;
  XBYTE                                         resolution;
  XWORD                                         read_write_len;
  const XBYTE*                                  config_file_ptr;
  XBYTE                                         page_max;
  XBYTE                                         input_sens;
  XBYTE                                         out_sens;
  XBYTE                                         aux_man_en;
  XBYTE                                         aux_man_rd_burst_len;
  const struct bmi2_feature_config*             feat_config;
  const struct bmi2_feature_config*             feat_output;
  struct bmi2_axes_remap                        remap;
  XQWORD                                        sens_en_stat;
  XWORDSIG                                      gyr_cross_sens_zx;
  XBYTE                                         gyro_en             : 1;
  XBYTE                                         aps_status;
  XWORD                                         variant_feature;
  XWORD                                         config_size;
  bmi2_wake_up_fptr_t                           get_wakeup_config;
  bmi2_wake_up_fptr_t                           set_wakeup_config;
  bmi2_tap_fptr_t                               get_tap_config;
  bmi2_tap_fptr_t                               set_tap_config;
  struct bmi2_map_int*                          map_int;
  XBYTE                                         sens_int_map;
};

// Structure to enable an accel axis for foc 
struct bmi2_accel_foc_g_value
{    
  XBYTE                                         x;
  XBYTE                                         y;
  XBYTE                                         z;
  XBYTE                                         sign;
};

// Structure to configure activity recognition settings 
struct bmi2_act_recg_sett
{   
  XBYTE                                         act_rec_1       : 1;
  XWORD                                         act_rec_2;
  XWORD                                         act_rec_3;
  XBYTE                                         act_rec_4       : 4;
  XBYTE                                         act_rec_5       : 4;
};

// Structure to configure activity recognition settings for bmi270hc 
struct bmi2_hc_act_recg_sett
{    
  XBYTE                                         segment_size;
  XBYTE                                         pp_en;
  XWORD                                         min_gdi_thres;
  XWORD                                         max_gdi_thres;
  XWORD                                         buf_size;
  XWORD                                         min_seg_conf;
};

// Local structures

// Structure to define the difference in accelerometer values  
struct bmi2_selftest_delta_limit
{  
  XDWORDSIG                                     x;
  XDWORDSIG                                     y;
  XDWORDSIG                                     z;
};

// Structure to store temporary accelerometer/gyroscope values 
struct bmi2_foc_temp_value
{   
  XDWORDSIG                                     x;
  XDWORDSIG                                     y;
  XDWORDSIG                                     z;
};

// Structure to store accelerometer data deviation from ideal value 
struct bmi2_offset_delta
{   
  XWORDSIG                                      x;
  XWORDSIG                                      y;
  XWORDSIG                                      z;
};

// Structure to store accelerometer offset values 
struct bmi2_accel_offset
{    
  XBYTE                                         x;
  XBYTE                                         y;
  XBYTE                                         z;
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOI2C6AXISTRACKINGBMI270: public DIODEVICEI2C
{
  public:

                              DIOI2C6AXISTRACKINGBMI270             ();
    virtual                  ~DIOI2C6AXISTRACKINGBMI270             ();

    XDWORD                    GetGPIOInterruptEntryID               ();
    bool                      SetGPIOInterruptEntryID               (XDWORD GPIOentryID);

    bool                      IsNecessaryUpdate                     ();  
    void                      SetIsNecessaryUpdate                  (bool isnecessaryupdate);  
    
    bool                      GetSetAnyMove                         ();
    bool                      GetSignificantMotion                  ();
    bool                      GetStep_AddOne                        ();
    bool                      GetTap_AddOne                         ();
    bool                      GetHighGMotion                        ();
    bool                      GetLowGMotion                         ();
    bool                      GetNotMove                            ();    

    XBYTE                     Update                                ();

    bool                      End                                   ();   

  private:

    bool                      IniDevice                             ();

    bool                      Inicializate                          ();
        
    bool                      ReadRegister                          (XBYTE offset, XBYTE* data, XDWORD sizedata);    
    bool                      WriteRegister                         (XBYTE offset, XBYTE* data, XDWORD sizedata);
      
    BMI2_INTF_RETURN_TYPE     bmi2_i2c_write                        (XBYTE reg_addr, const XBYTE *reg_data, XDWORD len, void *intf_ptr);
    BMI2_INTF_RETURN_TYPE     bmi2_i2c_read                         (XBYTE reg_addr, XBYTE *reg_data, XDWORD len, void *intf_ptr);
 
    XBYTESIG                  SetFeatureConfig                      (struct bmi2_dev *bmi2_dev);      
    
    void                      bmi2_delay_us                         (XDWORD period, void *intf_ptr);
    XBYTESIG                  bmi2_interface_init                   (struct bmi2_dev *bmi, XBYTE intf);
    void                      bmi2_error_codes_print_result         (XBYTESIG rslt);
    XBYTESIG                  null_ptr_check                        (const struct bmi2_dev *dev);
     
    XBYTESIG                  bmi2_sec_init                         (struct bmi2_dev *dev);
    XBYTESIG                  bmi2_get_regs                         (XBYTE reg_addr, XBYTE *data, XWORD len, struct bmi2_dev *dev);
    XBYTESIG                  bmi2_set_regs                         (XBYTE reg_addr, const XBYTE *data, XWORD len, struct bmi2_dev *dev);
    XBYTESIG                  bmi2_soft_reset                       (struct bmi2_dev *dev);
    XBYTESIG                  bmi2_get_config_file_version          (XBYTE *config_major, XBYTE *config_minor, struct bmi2_dev *dev);
    XBYTESIG                  bmi2_set_adv_power_save               (XBYTE enable, struct bmi2_dev *dev);
    XBYTESIG                  bmi2_get_adv_power_save               (XBYTE *aps_status, struct bmi2_dev *dev);
    XBYTESIG                  bmi2_write_config_file                (struct bmi2_dev *dev);
    XBYTESIG                  bmi2_set_int_pin_config               (const struct bmi2_int_pin_config *int_cfg, struct bmi2_dev *dev);
    XBYTESIG                  bmi2_get_int_pin_config               (struct bmi2_int_pin_config *int_cfg, struct bmi2_dev *dev);
    XBYTESIG                  bmi2_get_int_status                   (XWORD *int_status, struct bmi2_dev *dev);
    XBYTESIG                  bmi2_sensor_enable                    (const XBYTE *sens_list, XBYTE n_sens, struct bmi2_dev *dev);
    XBYTESIG                  bmi2_sensor_disable                   (const XBYTE *sens_list, XBYTE n_sens, struct bmi2_dev *dev);
    XBYTESIG                  bmi2_set_sensor_config                (struct bmi2_sens_config *sens_cfg, XBYTE n_sens, struct bmi2_dev *dev);
    XBYTESIG                  bmi2_get_sensor_config                (struct bmi2_sens_config *sens_cfg, XBYTE n_sens, struct bmi2_dev *dev);
    XBYTESIG                  bmi2_get_sensor_data                  (struct bmi2_sensor_data *sensor_data, XBYTE n_sens, struct bmi2_dev *dev);
    XBYTESIG                  bmi2_set_fifo_config                  (XWORD config, XBYTE enable, struct bmi2_dev *dev);
    XBYTESIG                  bmi2_get_fifo_config                  (XWORD *fifo_config, struct bmi2_dev *dev);
    XBYTESIG                  bmi2_read_fifo_data                   (struct bmi2_fifo_frame *fifo, struct bmi2_dev *dev);
    XBYTESIG                  bmi2_extract_accel                    (struct bmi2_sens_axes_data *accel_data, XWORD *accel_length, struct bmi2_fifo_frame *fifo, const struct bmi2_dev *dev);
    XBYTESIG                  bmi2_extract_gyro                     (struct bmi2_sens_axes_data *gyro_data, XWORD *gyro_length, struct bmi2_fifo_frame *fifo, const struct bmi2_dev *dev);
    XBYTESIG                  bmi2_extract_aux                      (struct bmi2_aux_fifo_data *aux, XWORD *aux_length, struct bmi2_fifo_frame *fifo, const struct bmi2_dev *dev);
    XBYTESIG                  bmi2_set_command_register             (XBYTE command, struct bmi2_dev *dev);
    XBYTESIG                  bmi2_set_fifo_self_wake_up            (XBYTE fifo_self_wake_up, struct bmi2_dev *dev);
    XBYTESIG                  bmi2_get_fifo_self_wake_up            (XBYTE *fifo_self_wake_up, struct bmi2_dev *dev);
    XBYTESIG                  bmi2_set_fifo_wm                      (XWORD fifo_wm, struct bmi2_dev *dev);
    XBYTESIG                  bmi2_get_fifo_wm                      (XWORD *fifo_wm, struct bmi2_dev *dev);
    XBYTESIG                  bmi2_set_fifo_filter_data             (XBYTE sens_sel, XBYTE fifo_filter_data, struct bmi2_dev *dev);
    XBYTESIG                  bmi2_get_fifo_filter_data             (XBYTE sens_sel, XBYTE *fifo_filter_data, struct bmi2_dev *dev);
    XBYTESIG                  bmi2_set_fifo_down_sample             (XBYTE sens_sel, XBYTE fifo_down_samp, struct bmi2_dev *dev);
    XBYTESIG                  bmi2_get_fifo_down_sample             (XBYTE sens_sel, XBYTE *fifo_down_samp, struct bmi2_dev *dev);
    XBYTESIG                  bmi2_get_fifo_length                  (XWORD *fifo_length, struct bmi2_dev *dev);
    XBYTESIG                  bmi2_read_aux_man_mode                (XBYTE reg_addr, XBYTE *aux_data, XWORD len, struct bmi2_dev *dev);
    XBYTESIG                  bmi2_write_aux_man_mode               (XBYTE reg_addr, const XBYTE *aux_data, XWORD len, struct bmi2_dev *dev);
    XBYTESIG                  bmi2_write_aux_interleaved            (XBYTE reg_addr, const XBYTE *aux_data, XWORD len, struct bmi2_dev *dev);
    XBYTESIG                  bmi2_get_status                       (XBYTE *status, struct bmi2_dev *dev);
    XBYTESIG                  bmi2_set_ois_interface                (XBYTE enable, struct bmi2_dev *dev);
    XBYTESIG                  bmi2_write_sync_commands              (const XBYTE *command, XBYTE n_comm, struct bmi2_dev *dev);
    XBYTESIG                  bmi2_perform_accel_self_test          (struct bmi2_dev *dev);
    XBYTESIG                  bmi2_map_feat_int                     (XBYTE type, enum bmi2_hw_int_pin hw_int_pin, struct bmi2_dev *dev);
    XBYTESIG                  bmi2_map_data_int                     (XBYTE data_int, enum bmi2_hw_int_pin int_pin, struct bmi2_dev *dev);
    XBYTESIG                  bmi2_get_remap_axes                   (struct bmi2_remap *remapped_axis, struct bmi2_dev *dev);
    XBYTESIG                  bmi2_set_remap_axes                   (const struct bmi2_remap *remapped_axis, struct bmi2_dev *dev);
    XBYTESIG                  bmi2_set_gyro_offset_comp             (XBYTE enable, struct bmi2_dev *dev);
    XBYTESIG                  bmi2_read_gyro_offset_comp_axes       (struct bmi2_sens_axes_data *gyr_off_comp_axes, struct bmi2_dev *dev);
    XBYTESIG                  bmi2_write_gyro_offset_comp_axes      (const struct bmi2_sens_axes_data *gyr_off_comp_axes, struct bmi2_dev *dev);
    XBYTESIG                  bmi2_get_gyro_cross_sense             (struct bmi2_dev *dev);
    XBYTESIG                  bmi2_get_internal_status              (XBYTE *int_stat, struct bmi2_dev *dev);
    XBYTESIG                  verify_foc_position                   (XBYTE sens_list, const struct bmi2_accel_foc_g_value *accel_g_axis, struct bmi2_dev *dev);
    XBYTESIG                  bmi2_perform_accel_foc                (const struct bmi2_accel_foc_g_value *accel_g_value, struct bmi2_dev *dev);
    XBYTESIG                  bmi2_perform_gyro_foc                 (struct bmi2_dev *dev);
    XBYTESIG                  bmi2_get_feat_config                  (XBYTE sw_page, XBYTE *feat_config, struct bmi2_dev *dev);
    XBYTE                     bmi2_extract_input_feat_config        (struct bmi2_feature_config *feat_config, XBYTE type, const struct bmi2_dev *dev);
    XBYTESIG                  write_config_file                     (struct bmi2_dev *dev);
    XBYTESIG                  set_config_load                       (XBYTE enable, struct bmi2_dev *dev);
    XBYTESIG                  upload_file                           (const XBYTE *config_data, XWORD index, XWORD write_len, struct bmi2_dev *dev);
    XBYTESIG                  validate_bw_perf_mode                 (XBYTE *bandwidth, XBYTE *perf_mode, struct bmi2_dev *dev);
    XBYTESIG                  validate_odr_range                    (XBYTE *odr, XBYTE *range, struct bmi2_dev *dev);
    XBYTESIG                  validate_gyro_config                  (struct bmi2_gyro_config *config, struct bmi2_dev *dev);
    XBYTESIG                  cfg_error_status                      (struct bmi2_dev *dev);
    XBYTESIG                  set_aux_config                        (struct bmi2_aux_config *config, struct bmi2_dev *dev);
    XBYTESIG                  set_gyro_user_gain_config             (const struct bmi2_gyro_user_gain_config *config, struct bmi2_dev *dev);
    XBYTESIG                  set_aux_interface                     (const struct bmi2_aux_config *config, struct bmi2_dev *dev);
    XBYTESIG                  config_aux_interface                  (const struct bmi2_aux_config *config, struct bmi2_dev *dev);
    XBYTESIG                  config_aux                            (const struct bmi2_aux_config *config, struct bmi2_dev *dev);
    XBYTESIG                  set_if_aux_not_busy                   (XBYTE reg_addr, XBYTE reg_data, struct bmi2_dev *dev);
    XBYTESIG                  validate_aux_config                   (struct bmi2_aux_config *config, struct bmi2_dev *dev);
    XBYTESIG                  get_accel_config                      (struct bmi2_accel_config *config, struct bmi2_dev *dev);
    XBYTESIG                  get_gyro_config                       (struct bmi2_gyro_config *config, struct bmi2_dev *dev);
    XBYTESIG                  get_aux_config                        (struct bmi2_aux_config *config, struct bmi2_dev *dev);
    XBYTESIG                  get_gyro_gain_update_config           (struct bmi2_gyro_user_gain_config *config, struct bmi2_dev *dev);
    XBYTESIG                  get_aux_interface                     (struct bmi2_aux_config *config, struct bmi2_dev *dev);
    XBYTESIG                  get_aux_interface_config              (struct bmi2_aux_config *config, struct bmi2_dev *dev);
    XBYTESIG                  get_aux_cfg                           (struct bmi2_aux_config *config, struct bmi2_dev *dev);
    XBYTESIG                  map_feat_int                          (XBYTE *reg_data_array, enum bmi2_hw_int_pin int_pin, XBYTE int_mask);
    XBYTESIG                  get_accel_sensor_data                 (struct bmi2_sens_axes_data *data, XBYTE reg_addr, struct bmi2_dev *dev);
    XBYTESIG                  get_gyro_sensor_data                  (struct bmi2_sens_axes_data *data, XBYTE reg_addr, struct bmi2_dev *dev);
    void                      get_acc_gyr_data                      (struct bmi2_sens_axes_data *data, const XBYTE *reg_data);
    void                      get_remapped_data                     (struct bmi2_sens_axes_data *data, const struct bmi2_dev *dev);
    XBYTESIG                  read_aux_data                         (XBYTE reg_addr, XBYTE *aux_data, XWORD len, XBYTE burst_len, struct bmi2_dev *dev);
    XBYTESIG                  write_aux_data                        (XBYTE reg_addr, XBYTE reg_data, struct bmi2_dev *dev);
    XBYTESIG                  read_aux_data_mode                    (XBYTE *aux_data, struct bmi2_dev *dev);
    XBYTESIG                  map_read_len                          (XBYTE *len, const struct bmi2_dev *dev);
    XBYTESIG                  parse_fifo_accel_len                  (XWORD *start_idx, XWORD *len, const XWORD *acc_count, const struct bmi2_fifo_frame *fifo);
    XBYTESIG                  extract_accel_header_mode             (struct bmi2_sens_axes_data *acc, XWORD *accel_length, struct bmi2_fifo_frame *fifo, const struct bmi2_dev *dev);
    XBYTESIG                  unpack_accel_frame                    (struct bmi2_sens_axes_data *acc, XWORD *idx, XWORD *acc_idx, XBYTE frame, const struct bmi2_fifo_frame *fifo, const struct bmi2_dev *dev);
    void                      unpack_accel_data                     (struct bmi2_sens_axes_data *acc, XWORD data_start_index, const struct bmi2_fifo_frame *fifo, const struct bmi2_dev *dev);
    XBYTESIG                  parse_fifo_gyro_len                   (XWORD *start_idx, XWORD(*len), const XWORD *gyr_count, const struct bmi2_fifo_frame *fifo);
    XBYTESIG                  extract_gyro_header_mode              (struct bmi2_sens_axes_data *gyr, XWORD *gyro_length, struct bmi2_fifo_frame *fifo, const struct bmi2_dev *dev);
    XBYTESIG                  unpack_gyro_frame                     (struct bmi2_sens_axes_data *gyr, XWORD *idx, XWORD *gyr_idx, XBYTE frame, const struct bmi2_fifo_frame *fifo, const struct bmi2_dev *dev);
    void                      unpack_gyro_data                      (struct bmi2_sens_axes_data *gyr, XWORD data_start_index, const struct bmi2_fifo_frame *fifo, const struct bmi2_dev *dev);

    XBYTESIG                  parse_fifo_aux_len                    (XWORD *start_idx, XWORD(*len), const XWORD *aux_count, const struct bmi2_fifo_frame *fifo);
    XBYTESIG                  extract_aux_header_mode               (struct bmi2_aux_fifo_data *aux, XWORD *aux_len, struct bmi2_fifo_frame *fifo, const struct bmi2_dev *dev);
    XBYTESIG                  unpack_aux_frame                      (struct bmi2_aux_fifo_data *aux, XWORD *idx, XWORD *aux_idx, XBYTE frame, const struct bmi2_fifo_frame *fifo, const struct bmi2_dev *dev);
    void                      unpack_aux_data                       (struct bmi2_aux_fifo_data *aux, XWORD data_start_index, const struct bmi2_fifo_frame *fifo);
    void                      parse_if_virtual_header               (XBYTE *frame_header, XWORD *data_index, const struct bmi2_fifo_frame *fifo);
    void                      unpack_virt_sensor_time               (struct bmi2_sens_axes_data *sens, XWORD *idx, const struct bmi2_fifo_frame *fifo);
    void                      unpack_virt_aux_sensor_time           (struct bmi2_aux_fifo_data *aux, XWORD *idx, const struct bmi2_fifo_frame *fifo);
    void                      reset_fifo_frame_structure            (struct bmi2_fifo_frame *fifo, const struct bmi2_dev *dev);
    XBYTESIG                  check_empty_fifo                      (XWORD *data_index, const struct bmi2_fifo_frame *fifo);
    XBYTESIG                  move_next_frame                       (XWORD *data_index, XBYTE current_frame_length, const struct bmi2_fifo_frame *fifo);
    XBYTESIG                  unpack_sensortime_frame               (XWORD *data_index, struct bmi2_fifo_frame *fifo);
    XBYTESIG                  unpack_skipped_frame                  (XWORD *data_index, struct bmi2_fifo_frame *fifo);
    XBYTESIG                  pre_self_test_config                  (struct bmi2_dev *dev);
    XBYTESIG                  self_test_config                      (XBYTE sign, struct bmi2_dev *dev);
    XBYTESIG                  set_accel_self_test_enable            (XBYTE enable, struct bmi2_dev *dev);
    XBYTESIG                  set_acc_self_test_sign                (XBYTE sign, struct bmi2_dev *dev);
    XBYTESIG                  set_accel_self_test_amp               (XBYTE amp, struct bmi2_dev *dev);
    XBYTESIG                  read_accel_xyz                        (struct bmi2_sens_axes_data *accel, struct bmi2_dev *dev);
    XBYTESIG                  read_gyro_xyz                         (struct bmi2_sens_axes_data *gyro, struct bmi2_dev *dev);
    void                      convert_lsb_g                         (const struct bmi2_selftest_delta_limit *acc_data_diff, struct bmi2_selftest_delta_limit *acc_data_diff_mg, const struct bmi2_dev *dev);XDWORDSIG power(XWORDSIG base, XBYTE resolution);
    XBYTESIG                  validate_self_test                    (const struct bmi2_selftest_delta_limit *accel_data_diff);
    XBYTESIG                  get_remap_axes                        (struct bmi2_axes_remap *remap, struct bmi2_dev *dev);
    XBYTESIG                  set_remap_axes                        (const struct bmi2_axes_remap *remap, struct bmi2_dev *dev);
    void                      comp_gyro_cross_axis_sensitivity      (struct bmi2_sens_axes_data *gyr_data, const struct bmi2_dev *dev);
    XBYTESIG                  check_boundary_val                    (XBYTE *val, XBYTE min, XBYTE max, struct bmi2_dev *dev);
    XBYTESIG                  save_accel_foc_config                 (struct bmi2_accel_config *acc_cfg, XBYTE *aps, XBYTE *acc_en, struct bmi2_dev *dev);
    XBYTESIG                  set_accel_foc_config                  (struct bmi2_dev *dev);
    XBYTESIG                  perform_accel_foc                     (const struct bmi2_accel_foc_g_value *accel_g_value, const struct bmi2_accel_config *acc_cfg, struct bmi2_dev *dev);
    XBYTESIG                  set_accel_offset_comp                 (XBYTE offset_en, struct bmi2_dev *dev);
    void                      map_accel_range                       (XBYTE range_in, XBYTE *range_out);
    void                      comp_for_gravity                      (XWORD lsb_per_g, const struct bmi2_accel_foc_g_value *g_val, const struct bmi2_sens_axes_data *data, struct bmi2_offset_delta *comp_data);
    void                      scale_accel_offset                    (XBYTE range, const struct bmi2_offset_delta *comp_data, struct bmi2_accel_offset *data);
    XBYTESIG                  get_bit_pos_3_9mg                     (XBYTE range);
    void                      invert_accel_offset                   (struct bmi2_accel_offset *offset_data);
    XBYTESIG                  write_accel_offset                    (const struct bmi2_accel_offset *offset, struct bmi2_dev *dev);
    XBYTESIG                  restore_accel_foc_config              (struct bmi2_accel_config *acc_cfg, XBYTE aps, XBYTE acc_en, struct bmi2_dev *dev);
    XBYTESIG                  set_accel_config                      (struct bmi2_accel_config *config, struct bmi2_dev *dev);
    XBYTESIG                  set_gyro_config                       (struct bmi2_gyro_config *config, struct bmi2_dev *dev);
    XBYTESIG                  save_gyro_config                      (struct bmi2_gyro_config *gyr_cfg, XBYTE *aps, XBYTE *gyr_en, struct bmi2_dev *dev);
    XBYTESIG                  set_gyro_foc_config                   (struct bmi2_dev *dev);void invert_gyro_offset(struct bmi2_sens_axes_data *offset_data);
    XBYTESIG                  restore_gyro_config                   (struct bmi2_gyro_config *gyr_cfg, XBYTE aps, XBYTE gyr_en, struct bmi2_dev *dev);
    void                      saturate_gyro_data                    (struct bmi2_sens_axes_data *gyr_off);

    XBYTESIG                  get_st_running                        (XBYTE *st_status, struct bmi2_dev *dev);
    XBYTESIG                  set_st_running                        (XBYTE st_status, struct bmi2_dev *dev);
    XBYTESIG                  get_rdy_for_dl                        (XBYTE *rdy_for_dl, struct bmi2_dev *dev);
    XBYTESIG                  process_crt_download                  (XBYTE last_byte_flag, struct bmi2_dev *dev);
    XBYTESIG                  write_crt_config_file                 (XWORD write_len, XWORD config_file_size, XWORD start_index, struct bmi2_dev *dev);
    XBYTESIG                  wait_rdy_for_dl_toggle                (XBYTE retry_complete, XBYTE download_ready, struct bmi2_dev *dev);
    XBYTESIG                  wait_st_running                       (XBYTE retry_complete, struct bmi2_dev *dev);
    XBYTESIG                  bmi2_do_gyro_st                       (struct bmi2_dev *dev);
    XBYTESIG                  bmi2_do_crt                           (struct bmi2_dev *dev);
    XBYTESIG                  do_gtrigger_test                      (XBYTE gyro_st_crt, struct bmi2_dev *dev);
    XBYTESIG                  crt_prepare_setup                     (struct bmi2_dev *dev);
    XBYTESIG                  crt_gyro_st_update_result             (struct bmi2_dev *dev);
    XBYTESIG                  get_maxburst_len                      (XBYTE *max_burst_len, struct bmi2_dev *dev);
    XBYTESIG                  set_maxburst_len                      (const XWORD write_len_byte, struct bmi2_dev *dev);
    XBYTESIG                  set_nvm_prep_prog                     (XBYTE nvm_prep, struct bmi2_dev *dev);
    XBYTESIG                  select_self_test                      (XBYTE gyro_st_crt, struct bmi2_dev *dev);
    XBYTESIG                  bmi2_abort_crt_gyro_st                (struct bmi2_dev *dev);
    XBYTESIG                  abort_bmi2                            (XBYTE abort_enable, struct bmi2_dev *dev);
    XBYTESIG                  gyro_self_test_completed              (struct bmi2_gyro_self_test_status *gyro_st_result, struct bmi2_dev *dev);
    XBYTESIG                  validate_foc_position                 (XBYTE sens_list, const struct bmi2_accel_foc_g_value *accel_g_axis, struct bmi2_sens_axes_data avg_foc_data, struct bmi2_dev *dev);
    XBYTESIG                  validate_foc_accel_axis               (XWORDSIG avg_foc_data, struct bmi2_dev *dev);
    XBYTESIG                  bmi2_nvm_prog                         (struct bmi2_dev *dev);
    XBYTESIG                  get_average_of_sensor_data            (XBYTE sens_list, struct bmi2_foc_temp_value *temp_foc_data, struct bmi2_dev *dev);
    XBYTESIG                  extract_config_file                   (XBYTE *config_major, XBYTE *config_minor, struct bmi2_dev *dev);
    void                      extract_feat_int_map                  (struct bmi2_map_int *map_int, XBYTE type, const struct bmi2_dev *dev);
    XBYTESIG                  get_gyro_gain_update_status           (struct bmi2_gyr_user_gain_status *user_gain_stat, struct bmi2_dev *dev);
    XBYTE                     extract_output_feat_config            (struct bmi2_feature_config *feat_output, XBYTE type, const struct bmi2_dev *dev);
    XBYTESIG                  get_gyro_cross_sense                  (XWORDSIG *cross_sense, struct bmi2_dev *dev);
    XBYTESIG                  select_sensor                         (const XBYTE *sens_list, XBYTE n_sens, XQWORD *sensor_sel);
    XBYTESIG                  sensor_enable                         (XQWORD sensor_sel, struct bmi2_dev *dev);
    XBYTESIG                  sensor_disable                        (XQWORD sensor_sel, struct bmi2_dev *dev);

    XBYTESIG                  bmi270_init                           (struct bmi2_dev *dev);
    XBYTESIG                  bmi270_sensor_enable                  (const XBYTE *sens_list, XBYTE n_sens, struct bmi2_dev *dev);
    XBYTESIG                  bmi270_sensor_disable                 (const XBYTE *sens_list, XBYTE n_sens, struct bmi2_dev *dev);
    XBYTESIG                  bmi270_set_sensor_config              (struct bmi2_sens_config *sens_cfg, XBYTE n_sens, struct bmi2_dev *dev);
    XBYTESIG                  bmi270_get_sensor_config              (struct bmi2_sens_config *sens_cfg, XBYTE n_sens, struct bmi2_dev *dev);
    XBYTESIG                  bmi270_get_sensor_data                (struct bmi2_sensor_data *sensor_data, XBYTE n_sens, struct bmi2_dev *dev);
    XBYTESIG                  bmi270_update_gyro_user_gain          (const struct bmi2_gyro_user_gain_config *user_gain, struct bmi2_dev *dev);
    XBYTESIG                  bmi270_read_gyro_user_gain            (struct bmi2_gyro_user_gain_data *gyr_usr_gain, struct bmi2_dev *dev);
    XBYTESIG                  bmi270_map_feat_int                   (const struct bmi2_sens_int_config *sens_int, XBYTE n_sens, struct bmi2_dev *dev);
    
    XBYTESIG                  set_any_motion                        (XBYTE enable, struct bmi2_dev *dev);
    XBYTESIG                  set_no_motion                         (XBYTE enable, struct bmi2_dev *dev);
    XBYTESIG                  set_step_detector                     (XBYTE enable, struct bmi2_dev *dev);
    XBYTESIG                  set_step_counter                      (XBYTE enable, struct bmi2_dev *dev);
    XBYTESIG                  set_sig_motion                        (XBYTE enable, struct bmi2_dev *dev);
    XBYTESIG                  set_step_activity                     (XBYTE enable, struct bmi2_dev *dev);
    XBYTESIG                  set_gyro_self_offset_corr             (XBYTE enable, struct bmi2_dev *dev);
    XBYTESIG                  set_wrist_gesture                     (XBYTE enable, struct bmi2_dev *dev);
    XBYTESIG                  set_wrist_wear_wake_up                (XBYTE enable, struct bmi2_dev *dev);
    XBYTESIG                  set_gyro_user_gain                    (XBYTE enable, struct bmi2_dev *dev);
    XBYTESIG                  set_any_motion_config                 (const struct bmi2_any_motion_config *config, struct bmi2_dev *dev);
    XBYTESIG                  set_no_motion_config                  (const struct bmi2_no_motion_config *config, struct bmi2_dev *dev);
    XBYTESIG                  set_sig_motion_config                 (const struct bmi2_sig_motion_config *config, struct bmi2_dev *dev);
    XBYTESIG                  set_step_count_params_config          (const XWORD *step_count_params, struct bmi2_dev *dev);
    XBYTESIG                  set_step_config                       (const struct bmi2_step_config *config, struct bmi2_dev *dev);
    XBYTESIG                  set_wrist_gest_config                 (const struct bmi2_wrist_gest_config *config, struct bmi2_dev *dev);
    XBYTESIG                  set_wrist_wear_wake_up_config         (const struct bmi2_wrist_wear_wake_up_config *config, struct bmi2_dev *dev);
    XBYTESIG                  get_any_motion_config                 (struct bmi2_any_motion_config *config, struct bmi2_dev *dev);
    XBYTESIG                  get_no_motion_config                  (struct bmi2_no_motion_config *config, struct bmi2_dev *dev);
    XBYTESIG                  get_sig_motion_config                 (struct bmi2_sig_motion_config *config, struct bmi2_dev *dev);
    XBYTESIG                  get_step_count_params_config          (XWORD *step_count_params, struct bmi2_dev *dev);
    XBYTESIG                  get_step_config                       (struct bmi2_step_config *config, struct bmi2_dev *dev);
    XBYTESIG                  get_wrist_gest_config                 (struct bmi2_wrist_gest_config *config, struct bmi2_dev *dev);
    XBYTESIG                  get_wrist_wear_wake_up_config         (struct bmi2_wrist_wear_wake_up_config *config, struct bmi2_dev *dev);
    XBYTESIG                  get_wrist_gest_status                 (XBYTE *wrist_gest, struct bmi2_dev *dev);
    XBYTESIG                  get_step_counter_output               (XDWORD *step_count, struct bmi2_dev *dev);
    XBYTESIG                  get_nvm_error_status                  (struct bmi2_nvm_err_status *nvm_err_stat, struct bmi2_dev *dev);
    XBYTESIG                  get_user_gain_upd_status              (XBYTE *status, struct bmi2_dev *dev);
    XBYTESIG                  get_step_activity_output              (XBYTE *step_act, struct bmi2_dev *dev);
    XBYTESIG                  get_vfrm_error_status                 (struct bmi2_vfrm_err_status *vfrm_err_stat, struct bmi2_dev *dev);
    XBYTESIG                  enable_gyro_gain                      (XBYTE enable, struct bmi2_dev *dev);
    
    static void               Handler_Interrupt                     (void* param);

    void                      Clean                                 ();
    
    XDWORD                    GPIOinterruptentryID;    
    bool                      isnecessaryupdate;  

    XBYTE                     dev_addr;
    struct bmi2_dev           bmi2_dev;
    XBYTE                     sens_list[5];
    
    XBYTE                     sig_motion;
    XBYTE                     highGmotion;
    XBYTE                     lowGmotion;
    XBYTE                     step_event;
    XBYTE                     tap_event;
    XBYTE                     any_motion;
    XBYTE                     no_motion;    
    
    XDWORD                    prev_status;        
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/







