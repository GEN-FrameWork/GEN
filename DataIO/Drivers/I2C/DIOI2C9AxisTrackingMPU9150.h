/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOI2C9AxisTrackingMPU9150.h
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include <stdio.h>

#include "XBase.h"
#include "XString.h"

#include "DIODeviceI2C.h"



/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOI2CMPU9150_RA_XG_OFFS_TC                   0x00 //[7] PWR_MODE, [6:1] XG_OFFS_TC, [0] OTP_BNK_VLD
#define DIOI2CMPU9150_RA_YG_OFFS_TC                   0x01 //[7] PWR_MODE, [6:1] YG_OFFS_TC, [0] OTP_BNK_VLD
#define DIOI2CMPU9150_RA_ZG_OFFS_TC                   0x02 //[7] PWR_MODE, [6:1] ZG_OFFS_TC, [0] OTP_BNK_VLD
#define DIOI2CMPU9150_RA_X_FINE_GAIN                  0x03 //[7:0] X_FINE_GAIN
#define DIOI2CMPU9150_RA_Y_FINE_GAIN                  0x04 //[7:0] Y_FINE_GAIN
#define DIOI2CMPU9150_RA_Z_FINE_GAIN                  0x05 //[7:0] Z_FINE_GAIN
#define DIOI2CMPU9150_RA_XA_OFFS_H                    0x06 //[15:0] XA_OFFS
#define DIOI2CMPU9150_RA_XA_OFFS_L_TC                 0x07
#define DIOI2CMPU9150_RA_YA_OFFS_H                    0x08 //[15:0] YA_OFFS
#define DIOI2CMPU9150_RA_YA_OFFS_L_TC                 0x09
#define DIOI2CMPU9150_RA_ZA_OFFS_H                    0x0A //[15:0] ZA_OFFS
#define DIOI2CMPU9150_RA_ZA_OFFS_L_TC                 0x0B
#define DIOI2CMPU9150_RA_XG_OFFS_USRH                 0x13 //[15:0] XG_OFFS_USR
#define DIOI2CMPU9150_RA_XG_OFFS_USRL                 0x14
#define DIOI2CMPU9150_RA_YG_OFFS_USRH                 0x15 //[15:0] YG_OFFS_USR
#define DIOI2CMPU9150_RA_YG_OFFS_USRL                 0x16
#define DIOI2CMPU9150_RA_ZG_OFFS_USRH                 0x17 //[15:0] ZG_OFFS_USR
#define DIOI2CMPU9150_RA_ZG_OFFS_USRL                 0x18
#define DIOI2CMPU9150_RA_SMPLRT_DIV                   0x19
#define DIOI2CMPU9150_RA_CONFIG                       0x1A
#define DIOI2CMPU9150_RA_GYRO_CONFIG                  0x1B
#define DIOI2CMPU9150_RA_ACCEL_CONFIG                 0x1C
#define DIOI2CMPU9150_RA_FF_THR                       0x1D
#define DIOI2CMPU9150_RA_FF_DUR                       0x1E
#define DIOI2CMPU9150_RA_MOT_THR                      0x1F
#define DIOI2CMPU9150_RA_MOT_DUR                      0x20
#define DIOI2CMPU9150_RA_ZRMOT_THR                    0x21
#define DIOI2CMPU9150_RA_ZRMOT_DUR                    0x22
#define DIOI2CMPU9150_RA_FIFO_EN                      0x23
#define DIOI2CMPU9150_RA_I2C_MST_CTRL                 0x24
#define DIOI2CMPU9150_RA_I2C_SLV0_ADDR                0x25
#define DIOI2CMPU9150_RA_I2C_SLV0_REG                 0x26
#define DIOI2CMPU9150_RA_I2C_SLV0_CTRL                0x27
#define DIOI2CMPU9150_RA_I2C_SLV1_ADDR                0x28
#define DIOI2CMPU9150_RA_I2C_SLV1_REG                 0x29
#define DIOI2CMPU9150_RA_I2C_SLV1_CTRL                0x2A
#define DIOI2CMPU9150_RA_I2C_SLV2_ADDR                0x2B
#define DIOI2CMPU9150_RA_I2C_SLV2_REG                 0x2C
#define DIOI2CMPU9150_RA_I2C_SLV2_CTRL                0x2D
#define DIOI2CMPU9150_RA_I2C_SLV3_ADDR                0x2E
#define DIOI2CMPU9150_RA_I2C_SLV3_REG                 0x2F
#define DIOI2CMPU9150_RA_I2C_SLV3_CTRL                0x30
#define DIOI2CMPU9150_RA_I2C_SLV4_ADDR                0x31
#define DIOI2CMPU9150_RA_I2C_SLV4_REG                 0x32
#define DIOI2CMPU9150_RA_I2C_SLV4_DO                  0x33
#define DIOI2CMPU9150_RA_I2C_SLV4_CTRL                0x34
#define DIOI2CMPU9150_RA_I2C_SLV4_DI                  0x35
#define DIOI2CMPU9150_RA_I2C_MST_STATUS               0x36
#define DIOI2CMPU9150_RA_INT_PIN_CFG                  0x37
#define DIOI2CMPU9150_RA_INT_ENABLE                   0x38
#define DIOI2CMPU9150_RA_DMP_INT_STATUS               0x39
#define DIOI2CMPU9150_RA_INT_STATUS                   0x3A
#define DIOI2CMPU9150_RA_ACCEL_XOUT_H                 0x3B
#define DIOI2CMPU9150_RA_ACCEL_XOUT_L                 0x3C
#define DIOI2CMPU9150_RA_ACCEL_YOUT_H                 0x3D
#define DIOI2CMPU9150_RA_ACCEL_YOUT_L                 0x3E
#define DIOI2CMPU9150_RA_ACCEL_ZOUT_H                 0x3F
#define DIOI2CMPU9150_RA_ACCEL_ZOUT_L                 0x40
#define DIOI2CMPU9150_RA_TEMP_OUT_H                   0x41
#define DIOI2CMPU9150_RA_TEMP_OUT_L                   0x42
#define DIOI2CMPU9150_RA_GYRO_XOUT_H                  0x43
#define DIOI2CMPU9150_RA_GYRO_XOUT_L                  0x44
#define DIOI2CMPU9150_RA_GYRO_YOUT_H                  0x45
#define DIOI2CMPU9150_RA_GYRO_YOUT_L                  0x46
#define DIOI2CMPU9150_RA_GYRO_ZOUT_H                  0x47
#define DIOI2CMPU9150_RA_GYRO_ZOUT_L                  0x48
#define DIOI2CMPU9150_RA_EXT_SENS_DATA_00             0x49
#define DIOI2CMPU9150_RA_EXT_SENS_DATA_01             0x4A
#define DIOI2CMPU9150_RA_EXT_SENS_DATA_02             0x4B
#define DIOI2CMPU9150_RA_EXT_SENS_DATA_03             0x4C
#define DIOI2CMPU9150_RA_EXT_SENS_DATA_04             0x4D
#define DIOI2CMPU9150_RA_EXT_SENS_DATA_05             0x4E
#define DIOI2CMPU9150_RA_EXT_SENS_DATA_06             0x4F
#define DIOI2CMPU9150_RA_EXT_SENS_DATA_07             0x50
#define DIOI2CMPU9150_RA_EXT_SENS_DATA_08             0x51
#define DIOI2CMPU9150_RA_EXT_SENS_DATA_09             0x52
#define DIOI2CMPU9150_RA_EXT_SENS_DATA_10             0x53
#define DIOI2CMPU9150_RA_EXT_SENS_DATA_11             0x54
#define DIOI2CMPU9150_RA_EXT_SENS_DATA_12             0x55
#define DIOI2CMPU9150_RA_EXT_SENS_DATA_13             0x56
#define DIOI2CMPU9150_RA_EXT_SENS_DATA_14             0x57
#define DIOI2CMPU9150_RA_EXT_SENS_DATA_15             0x58
#define DIOI2CMPU9150_RA_EXT_SENS_DATA_16             0x59
#define DIOI2CMPU9150_RA_EXT_SENS_DATA_17             0x5A
#define DIOI2CMPU9150_RA_EXT_SENS_DATA_18             0x5B
#define DIOI2CMPU9150_RA_EXT_SENS_DATA_19             0x5C
#define DIOI2CMPU9150_RA_EXT_SENS_DATA_20             0x5D
#define DIOI2CMPU9150_RA_EXT_SENS_DATA_21             0x5E
#define DIOI2CMPU9150_RA_EXT_SENS_DATA_22             0x5F
#define DIOI2CMPU9150_RA_EXT_SENS_DATA_23             0x60
#define DIOI2CMPU9150_RA_MOT_DETECT_STATUS            0x61
#define DIOI2CMPU9150_RA_I2C_SLV0_DO                  0x63
#define DIOI2CMPU9150_RA_I2C_SLV1_DO                  0x64
#define DIOI2CMPU9150_RA_I2C_SLV2_DO                  0x65
#define DIOI2CMPU9150_RA_I2C_SLV3_DO                  0x66
#define DIOI2CMPU9150_RA_I2C_MST_DELAY_CTRL           0x67
#define DIOI2CMPU9150_RA_SIGNAL_PATH_RESET            0x68
#define DIOI2CMPU9150_RA_MOT_DETECT_CTRL              0x69
#define DIOI2CMPU9150_RA_USER_CTRL                    0x6A
#define DIOI2CMPU9150_RA_PWR_MGMT_1                   0x6B
#define DIOI2CMPU9150_RA_PWR_MGMT_2                   0x6C
#define DIOI2CMPU9150_RA_BANK_SEL                     0x6D
#define DIOI2CMPU9150_RA_MEM_START_ADDR               0x6E
#define DIOI2CMPU9150_RA_MEM_R_W                      0x6F
#define DIOI2CMPU9150_RA_DMP_CFG_1                    0x70
#define DIOI2CMPU9150_RA_DMP_CFG_2                    0x71
#define DIOI2CMPU9150_RA_FIFO_COUNTH                  0x72
#define DIOI2CMPU9150_RA_FIFO_COUNTL                  0x73
#define DIOI2CMPU9150_RA_FIFO_R_W                     0x74
#define DIOI2CMPU9150_RA_WHO_AM_I                     0x75

#define DIOI2CMPU9150_TC_PWR_MODE_BIT                 7
#define DIOI2CMPU9150_TC_OFFSET_BIT                   6
#define DIOI2CMPU9150_TC_OFFSET_LENGTH                6
#define DIOI2CMPU9150_TC_OTP_BNK_VLD_BIT              0

#define DIOI2CMPU9150_VDDIO_LEVEL_VLOGIC              0
#define DIOI2CMPU9150_VDDIO_LEVEL_VDD                 1

#define DIOI2CMPU9150_CFG_EXT_SYNC_SET_BIT            5
#define DIOI2CMPU9150_CFG_EXT_SYNC_SET_LENGTH         3
#define DIOI2CMPU9150_CFG_DLPF_CFG_BIT                2
#define DIOI2CMPU9150_CFG_DLPF_CFG_LENGTH             3

#define DIOI2CMPU9150_EXT_SYNC_DISABLED               0x0
#define DIOI2CMPU9150_EXT_SYNC_TEMP_OUT_L             0x1
#define DIOI2CMPU9150_EXT_SYNC_GYRO_XOUT_L            0x2
#define DIOI2CMPU9150_EXT_SYNC_GYRO_YOUT_L            0x3
#define DIOI2CMPU9150_EXT_SYNC_GYRO_ZOUT_L            0x4
#define DIOI2CMPU9150_EXT_SYNC_ACCEL_XOUT_L           0x5
#define DIOI2CMPU9150_EXT_SYNC_ACCEL_YOUT_L           0x6
#define DIOI2CMPU9150_EXT_SYNC_ACCEL_ZOUT_L           0x7

#define DIOI2CMPU9150_DLPF_BW_256                     0x00
#define DIOI2CMPU9150_DLPF_BW_188                     0x01
#define DIOI2CMPU9150_DLPF_BW_98                      0x02
#define DIOI2CMPU9150_DLPF_BW_42                      0x03
#define DIOI2CMPU9150_DLPF_BW_20                      0x04
#define DIOI2CMPU9150_DLPF_BW_10                      0x05
#define DIOI2CMPU9150_DLPF_BW_5                       0x06

#define DIOI2CMPU9150_GCONFIG_FS_SEL_BIT              4
#define DIOI2CMPU9150_GCONFIG_FS_SEL_LENGTH           2

//Magnetometer Registers
#define DIOI2CMPU9150_RA_MAG_ADDRESS                  0x0C
#define DIOI2CMPU9150_RA_MAG_XOUT_L                   0x03
#define DIOI2CMPU9150_RA_MAG_XOUT_H                   0x04
#define DIOI2CMPU9150_RA_MAG_YOUT_L                   0x05
#define DIOI2CMPU9150_RA_MAG_YOUT_H                   0x06
#define DIOI2CMPU9150_RA_MAG_ZOUT_L                   0x07
#define DIOI2CMPU9150_RA_MAG_ZOUT_H                   0x08


#define DIOI2CMPU9150_GYRO_FS_250                     0x00
#define DIOI2CMPU9150_GYRO_FS_500                     0x01
#define DIOI2CMPU9150_GYRO_FS_1000                    0x02
#define DIOI2CMPU9150_GYRO_FS_2000                    0x03

#define DIOI2CMPU9150_ACONFIG_XA_ST_BIT               7
#define DIOI2CMPU9150_ACONFIG_YA_ST_BIT               6
#define DIOI2CMPU9150_ACONFIG_ZA_ST_BIT               5
#define DIOI2CMPU9150_ACONFIG_AFS_SEL_BIT             4
#define DIOI2CMPU9150_ACONFIG_AFS_SEL_LENGTH          2
#define DIOI2CMPU9150_ACONFIG_ACCEL_HPF_BIT           2
#define DIOI2CMPU9150_ACONFIG_ACCEL_HPF_LENGTH        3

#define DIOI2CMPU9150_ACCEL_FS_2                      0x00
#define DIOI2CMPU9150_ACCEL_FS_4                      0x01
#define DIOI2CMPU9150_ACCEL_FS_8                      0x02
#define DIOI2CMPU9150_ACCEL_FS_16                     0x03

#define DIOI2CMPU9150_DHPF_RESET                      0x00
#define DIOI2CMPU9150_DHPF_5                          0x01
#define DIOI2CMPU9150_DHPF_2P5                        0x02
#define DIOI2CMPU9150_DHPF_1P25                       0x03
#define DIOI2CMPU9150_DHPF_0P63                       0x04
#define DIOI2CMPU9150_DHPF_HOLD                       0x07

#define DIOI2CMPU9150_TEMP_FIFO_EN_BIT                7
#define DIOI2CMPU9150_XG_FIFO_EN_BIT                  6
#define DIOI2CMPU9150_YG_FIFO_EN_BIT                  5
#define DIOI2CMPU9150_ZG_FIFO_EN_BIT                  4
#define DIOI2CMPU9150_ACCEL_FIFO_EN_BIT               3
#define DIOI2CMPU9150_SLV2_FIFO_EN_BIT                2
#define DIOI2CMPU9150_SLV1_FIFO_EN_BIT                1
#define DIOI2CMPU9150_SLV0_FIFO_EN_BIT                0

#define DIOI2CMPU9150_MULT_MST_EN_BIT                 7
#define DIOI2CMPU9150_WAIT_FOR_ES_BIT                 6
#define DIOI2CMPU9150_SLV_3_FIFO_EN_BIT               5
#define DIOI2CMPU9150_I2C_MST_P_NSR_BIT               4
#define DIOI2CMPU9150_I2C_MST_CLK_BIT                 3
#define DIOI2CMPU9150_I2C_MST_CLK_LENGTH              4

#define DIOI2CMPU9150_CLOCK_DIV_348                   0x0
#define DIOI2CMPU9150_CLOCK_DIV_333                   0x1
#define DIOI2CMPU9150_CLOCK_DIV_320                   0x2
#define DIOI2CMPU9150_CLOCK_DIV_308                   0x3
#define DIOI2CMPU9150_CLOCK_DIV_296                   0x4
#define DIOI2CMPU9150_CLOCK_DIV_286                   0x5
#define DIOI2CMPU9150_CLOCK_DIV_276                   0x6
#define DIOI2CMPU9150_CLOCK_DIV_267                   0x7
#define DIOI2CMPU9150_CLOCK_DIV_258                   0x8
#define DIOI2CMPU9150_CLOCK_DIV_500                   0x9
#define DIOI2CMPU9150_CLOCK_DIV_471                   0xA
#define DIOI2CMPU9150_CLOCK_DIV_444                   0xB
#define DIOI2CMPU9150_CLOCK_DIV_421                   0xC
#define DIOI2CMPU9150_CLOCK_DIV_400                   0xD
#define DIOI2CMPU9150_CLOCK_DIV_381                   0xE
#define DIOI2CMPU9150_CLOCK_DIV_364                   0xF

#define DIOI2CMPU9150_I2C_SLV_RW_BIT                  7
#define DIOI2CMPU9150_I2C_SLV_ADDR_BIT                6
#define DIOI2CMPU9150_I2C_SLV_ADDR_LENGTH             7
#define DIOI2CMPU9150_I2C_SLV_EN_BIT                  7
#define DIOI2CMPU9150_I2C_SLV_BYTE_SW_BIT             6
#define DIOI2CMPU9150_I2C_SLV_REG_DIS_BIT             5
#define DIOI2CMPU9150_I2C_SLV_GRP_BIT                 4
#define DIOI2CMPU9150_I2C_SLV_LEN_BIT                 3
#define DIOI2CMPU9150_I2C_SLV_LEN_LENGTH              4

#define DIOI2CMPU9150_I2C_SLV4_RW_BIT                 7
#define DIOI2CMPU9150_I2C_SLV4_ADDR_BIT               6
#define DIOI2CMPU9150_I2C_SLV4_ADDR_LENGTH            7
#define DIOI2CMPU9150_I2C_SLV4_EN_BIT                 7
#define DIOI2CMPU9150_I2C_SLV4_INT_EN_BIT             6
#define DIOI2CMPU9150_I2C_SLV4_REG_DIS_BIT            5
#define DIOI2CMPU9150_I2C_SLV4_MST_DLY_BIT            4
#define DIOI2CMPU9150_I2C_SLV4_MST_DLY_LENGTH         5

#define DIOI2CMPU9150_MST_PASS_THROUGH_BIT            7
#define DIOI2CMPU9150_MST_I2C_SLV4_DONE_BIT           6
#define DIOI2CMPU9150_MST_I2C_LOST_ARB_BIT            5
#define DIOI2CMPU9150_MST_I2C_SLV4_NACK_BIT           4
#define DIOI2CMPU9150_MST_I2C_SLV3_NACK_BIT           3
#define DIOI2CMPU9150_MST_I2C_SLV2_NACK_BIT           2
#define DIOI2CMPU9150_MST_I2C_SLV1_NACK_BIT           1
#define DIOI2CMPU9150_MST_I2C_SLV0_NACK_BIT           0

#define DIOI2CMPU9150_INTCFG_INT_LEVEL_BIT            7
#define DIOI2CMPU9150_INTCFG_INT_OPEN_BIT             6
#define DIOI2CMPU9150_INTCFG_LATCH_INT_EN_BIT         5
#define DIOI2CMPU9150_INTCFG_INT_RD_CLEAR_BIT         4
#define DIOI2CMPU9150_INTCFG_FSYNC_INT_LEVEL_BIT      3
#define DIOI2CMPU9150_INTCFG_FSYNC_INT_EN_BIT         2
#define DIOI2CMPU9150_INTCFG_I2C_BYPASS_EN_BIT        1
#define DIOI2CMPU9150_INTCFG_CLKOUT_EN_BIT            0

#define DIOI2CMPU9150_INTMODE_ACTIVEHIGH              0x00
#define DIOI2CMPU9150_INTMODE_ACTIVELOW               0x01

#define DIOI2CMPU9150_INTDRV_PUSHPULL                 0x00
#define DIOI2CMPU9150_INTDRV_OPENDRAIN                0x01

#define DIOI2CMPU9150_INTLATCH_50USPULSE              0x00
#define DIOI2CMPU9150_INTLATCH_WAITCLEAR              0x01

#define DIOI2CMPU9150_INTCLEAR_STATUSREAD             0x00
#define DIOI2CMPU9150_INTCLEAR_ANYREAD                0x01

#define DIOI2CMPU9150_INTERRUPT_FF_BIT                7
#define DIOI2CMPU9150_INTERRUPT_MOT_BIT               6
#define DIOI2CMPU9150_INTERRUPT_ZMOT_BIT              5
#define DIOI2CMPU9150_INTERRUPT_FIFO_OFLOW_BIT        4
#define DIOI2CMPU9150_INTERRUPT_I2C_MST_INT_BIT       3
#define DIOI2CMPU9150_INTERRUPT_PLL_RDY_INT_BIT       2
#define DIOI2CMPU9150_INTERRUPT_DMP_INT_BIT           1
#define DIOI2CMPU9150_INTERRUPT_DATA_RDY_BIT          0

// TODO: figure out what these actually do
// UMPL source code is not very obivous
#define DIOI2CMPU9150_DMPINT_5_BIT                    5
#define DIOI2CMPU9150_DMPINT_4_BIT                    4
#define DIOI2CMPU9150_DMPINT_3_BIT                    3
#define DIOI2CMPU9150_DMPINT_2_BIT                    2
#define DIOI2CMPU9150_DMPINT_1_BIT                    1
#define DIOI2CMPU9150_DMPINT_0_BIT                    0

#define DIOI2CMPU9150_MOTION_MOT_XNEG_BIT             7
#define DIOI2CMPU9150_MOTION_MOT_XPOS_BIT             6
#define DIOI2CMPU9150_MOTION_MOT_YNEG_BIT             5
#define DIOI2CMPU9150_MOTION_MOT_YPOS_BIT             4
#define DIOI2CMPU9150_MOTION_MOT_ZNEG_BIT             3
#define DIOI2CMPU9150_MOTION_MOT_ZPOS_BIT             2
#define DIOI2CMPU9150_MOTION_MOT_ZRMOT_BIT            0

#define DIOI2CMPU9150_DELAYCTRL_DELAY_ES_SHADOW_BIT   7
#define DIOI2CMPU9150_DELAYCTRL_I2C_SLV4_DLY_EN_BIT   4
#define DIOI2CMPU9150_DELAYCTRL_I2C_SLV3_DLY_EN_BIT   3
#define DIOI2CMPU9150_DELAYCTRL_I2C_SLV2_DLY_EN_BIT   2
#define DIOI2CMPU9150_DELAYCTRL_I2C_SLV1_DLY_EN_BIT   1
#define DIOI2CMPU9150_DELAYCTRL_I2C_SLV0_DLY_EN_BIT   0

#define DIOI2CMPU9150_PATHRESET_GYRO_RESET_BIT        2
#define DIOI2CMPU9150_PATHRESET_ACCEL_RESET_BIT       1
#define DIOI2CMPU9150_PATHRESET_TEMP_RESET_BIT        0

#define DIOI2CMPU9150_DETECT_ACCEL_ON_DELAY_BIT       5
#define DIOI2CMPU9150_DETECT_ACCEL_ON_DELAY_LENGTH    2
#define DIOI2CMPU9150_DETECT_FF_COUNT_BIT             3
#define DIOI2CMPU9150_DETECT_FF_COUNT_LENGTH          2
#define DIOI2CMPU9150_DETECT_MOT_COUNT_BIT            1
#define DIOI2CMPU9150_DETECT_MOT_COUNT_LENGTH         2

#define DIOI2CMPU9150_DETECT_DECREMENT_RESET          0x0
#define DIOI2CMPU9150_DETECT_DECREMENT_1              0x1
#define DIOI2CMPU9150_DETECT_DECREMENT_2              0x2
#define DIOI2CMPU9150_DETECT_DECREMENT_4              0x3

#define DIOI2CMPU9150_USERCTRL_DMP_EN_BIT             7
#define DIOI2CMPU9150_USERCTRL_FIFO_EN_BIT            6
#define DIOI2CMPU9150_USERCTRL_I2C_MST_EN_BIT         5
#define DIOI2CMPU9150_USERCTRL_I2C_IF_DIS_BIT         4
#define DIOI2CMPU9150_USERCTRL_DMP_RESET_BIT          3
#define DIOI2CMPU9150_USERCTRL_FIFO_RESET_BIT         2
#define DIOI2CMPU9150_USERCTRL_I2C_MST_RESET_BIT      1
#define DIOI2CMPU9150_USERCTRL_SIG_COND_RESET_BIT     0

#define DIOI2CMPU9150_PWR1_DEVICE_RESET_BIT           7
#define DIOI2CMPU9150_PWR1_SLEEP_BIT                  6
#define DIOI2CMPU9150_PWR1_CYCLE_BIT                  5
#define DIOI2CMPU9150_PWR1_TEMP_DIS_BIT               3
#define DIOI2CMPU9150_PWR1_CLKSEL_BIT                 2
#define DIOI2CMPU9150_PWR1_CLKSEL_LENGTH              3

#define DIOI2CMPU9150_CLOCK_INTERNAL                  0x00
#define DIOI2CMPU9150_CLOCK_PLL_XGYRO                 0x01
#define DIOI2CMPU9150_CLOCK_PLL_YGYRO                 0x02
#define DIOI2CMPU9150_CLOCK_PLL_ZGYRO                 0x03
#define DIOI2CMPU9150_CLOCK_PLL_EXT32K                0x04
#define DIOI2CMPU9150_CLOCK_PLL_EXT19M                0x05
#define DIOI2CMPU9150_CLOCK_KEEP_RESET                0x07

#define DIOI2CMPU9150_PWR2_LP_WAKE_CTRL_BIT           7
#define DIOI2CMPU9150_PWR2_LP_WAKE_CTRL_LENGTH        2
#define DIOI2CMPU9150_PWR2_STBY_XA_BIT                5
#define DIOI2CMPU9150_PWR2_STBY_YA_BIT                4
#define DIOI2CMPU9150_PWR2_STBY_ZA_BIT                3
#define DIOI2CMPU9150_PWR2_STBY_XG_BIT                2
#define DIOI2CMPU9150_PWR2_STBY_YG_BIT                1
#define DIOI2CMPU9150_PWR2_STBY_ZG_BIT                0

#define DIOI2CMPU9150_WAKE_FREQ_1P25                  0x0
#define DIOI2CMPU9150_WAKE_FREQ_2P5                   0x1
#define DIOI2CMPU9150_WAKE_FREQ_5                     0x2
#define DIOI2CMPU9150_WAKE_FREQ_10                    0x3

#define DIOI2CMPU9150_BANKSEL_PRFTCH_EN_BIT           6
#define DIOI2CMPU9150_BANKSEL_CFG_USER_BANK_BIT       5
#define DIOI2CMPU9150_BANKSEL_MEM_SEL_BIT             4
#define DIOI2CMPU9150_BANKSEL_MEM_SEL_LENGTH          5

#define DIOI2CMPU9150_WHO_AM_I_BIT                    6
#define DIOI2CMPU9150_WHO_AM_I_LENGTH                 6

#define DIOI2CMPU9150_DMP_MEMORY_BANKS                8
#define DIOI2CMPU9150_DMP_MEMORY_BANK_SIZE            256
#define DIOI2CMPU9150_DMP_MEMORY_CHUNK_SIZE           16

#define DIOI2CMPU9150_DEFAULTTIMEOUT                  5

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XTIMER;
class DIOSTREAMI2CCONFIG;
class DIOSTREAMI2C;

class DIOI2C9AXISTRACKINGMPU9150 : public DIODEVICEI2C
{
  public:

                              DIOI2C9AXISTRACKINGMPU9150            ();
    virtual                  ~DIOI2C9AXISTRACKINGMPU9150            ();

    bool                      End                                   ();

  private:

    bool                      IniDevice                             ();

    void                      Initialize                            ();
    bool                      TestConnection                        ();
    XBYTE                     GetAuxVDDIOLevel                      ();
    void                      SetAuxVDDIOLevel                      (XBYTE level);
    XBYTE                     GetRate                               ();
    void                      SetRate                               (XBYTE rate);
    XBYTE                     CheckMagStatus                        ();
    XBYTE                     GetExternalFrameSync                  ();
    void                      SetExternalFrameSync                  (XBYTE sync);
    XBYTE                     GetDLPFMode                           ();
    void                      SetDLPFMode                           (XBYTE mode);
    XBYTE                     GetFullScaleGyroRange                 ();
    void                      SetFullScaleGyroRange                 (XBYTE range);
    bool                      GetAccelXSelfTest                     ();
    void                      SetAccelXSelfTest                     (bool enabled);
    bool                      GetAccelYSelfTest                     ();
    void                      SetAccelYSelfTest                     (bool enabled);
    bool                      GetAccelZSelfTest                     ();
    void                      SetAccelZSelfTest                     (bool enabled);
    XBYTE                     GetFullScaleAccelRange                ();
    void                      SetFullScaleAccelRange                (XBYTE range);
    XBYTE                     GetDHPFMode                           ();
    void                      SetDHPFMode                           (XBYTE bandwidth);
    XBYTE                     GetFreefallDetectionThreshold         ();
    void                      SetFreefallDetectionThreshold         (XBYTE threshold);
    XBYTE                     GetFreefallDetectionDuration          ();
    void                      SetFreefallDetectionDuration          (XBYTE duration);
    XBYTE                     GetMotionDetectionThreshold           ();
    void                      SetMotionDetectionThreshold           (XBYTE threshold);
    XBYTE                     GetMotionDetectionDuration            ();
    void                      SetMotionDetectionDuration            (XBYTE duration);
    XBYTE                     GetZeroMotionDetectionThreshold       ();
    void                      SetZeroMotionDetectionThreshold       (XBYTE threshold);
    XBYTE                     GetZeroMotionDetectionDuration        ();
    void                      SetZeroMotionDetectionDuration        (XBYTE duration);

    bool                      GetTempFIFOEnabled                    ();
    void                      SetTempFIFOEnabled                    (bool enabled);
    bool                      GetXGyroFIFOEnabled                   ();
    void                      SetXGyroFIFOEnabled                   (bool enabled);
    bool                      GetYGyroFIFOEnabled                   ();
    void                      SetYGyroFIFOEnabled                   (bool enabled);
    bool                      GetZGyroFIFOEnabled                   ();
    void                      SetZGyroFIFOEnabled                   (bool enabled);
    bool                      GetAccelFIFOEnabled                   ();
    void                      SetAccelFIFOEnabled                   (bool enabled);
    bool                      GetSlave2FIFOEnabled                  ();
    void                      SetSlave2FIFOEnabled                  (bool enabled);
    bool                      GetSlave1FIFOEnabled                  ();
    void                      SetSlave1FIFOEnabled                  (bool enabled);
    bool                      GetSlave0FIFOEnabled                  ();
    void                      SetSlave0FIFOEnabled                  (bool enabled);

    bool                      GetMultiMasterEnabled                 ();
    void                      SetMultiMasterEnabled                 (bool enabled);
    bool                      GetWaitForExternalSensorEnabled       ();
    void                      SetWaitForExternalSensorEnabled       (bool enabled);
    bool                      GetSlave3FIFOEnabled                  ();
    void                      SetSlave3FIFOEnabled                  (bool enabled);
    bool                      GetSlaveReadWriteTransitionEnabled    ();
    void                      SetSlaveReadWriteTransitionEnabled    (bool enabled);
    XBYTE                     GetMasterClockSpeed                   ();
    void                      SetMasterClockSpeed                   (XBYTE speed);
    XBYTE                     GetSlaveAddress                       (XBYTE num);
    void                      SetSlaveAddress                       (XBYTE num, XBYTE address);
    XBYTE                     GetSlaveRegister                      (XBYTE num);
    void                      SetSlaveRegister                      (XBYTE num, XBYTE reg);
    bool                      GetSlaveEnabled                       (XBYTE num);
    void                      SetSlaveEnabled                       (XBYTE num, bool enabled);
    bool                      GetSlaveWordByteSwap                  (XBYTE num);
    void                      SetSlaveWordByteSwap                  (XBYTE num, bool enabled);
    bool                      GetSlaveWriteMode                     (XBYTE num);
    void                      SetSlaveWriteMode                     (XBYTE num, bool mode);
    bool                      GetSlaveWordGroupOffset               (XBYTE num);
    void                      SetSlaveWordGroupOffset               (XBYTE num, bool enabled);
    XBYTE                     GetSlaveDataLength                    (XBYTE num);
    void                      SetSlaveDataLength                    (XBYTE num, XBYTE length);
    XBYTE                     GetSlave4Address                      ();
    void                      SetSlave4Address                      (XBYTE address);
    XBYTE                     GetSlave4Register                     ();
    void                      SetSlave4Register                     (XBYTE reg);
    void                      SetSlave4OutputByte                   (XBYTE data);
    bool                      GetSlave4Enabled                      ();
    void                      SetSlave4Enabled                      (bool enabled) ;
    bool                      GetSlave4InterruptEnabled             ();
    void                      SetSlave4InterruptEnabled             (bool enabled);
    bool                      GetSlave4WriteMode                    ();
    void                      SetSlave4WriteMode                    (bool mode);
    XBYTE                     GetSlave4MasterDelay                  ();
    void                      GetSlave4MasterDelay                  (XBYTE delay);
    XBYTE                     GetSlate4InputByte                    ();
    bool                      GetPassthroughStatus                  ();
    bool                      GetSlave4IsDone                       ();
    bool                      GetLostArbitration                    ();
    bool                      GetSlave4Nack                         ();
    bool                      GetSlave3Nack                         ();
    bool                      GetSlave2Nack                         ();
    bool                      GetSlave1Nack                         ();
    bool                      GetSlave0Nack                         ();

    bool                      GetInterruptMode                      ();
    void                      SetInterruptMode                      (bool mode);
    bool                      GetInterruptDrive                     ();
    void                      SetInterruptDrive                     (bool drive);
    bool                      GetInterruptLatch                     ();
    void                      SetInterruptLatch                     (bool latch);
    bool                      GetInterruptLatchClear                ();
    void                      SetInterruptLatchClear                (bool clear);
    bool                      GetFSyncInterruptLevel                ();
    void                      SetFSyncInterruptLevel                (bool level);
    bool                      GetFSyncInterruptEnabled              ();
    void                      SetFSyncInterruptEnabled              (bool enabled);

    bool                      GetI2CBypassEnabled                   ();
    void                      SetI2CBypassEnabled                   (bool enabled);

    bool                      GetClockOutputEnabled                 ();
    void                      SetClockOutputEnabled                 (bool enabled);

    XBYTE                     GetIntEnabled                         ();
    void                      SetIntEnabled                         (XBYTE enabled);
    bool                      GetIntFreefallEnabled                 ();
    void                      SetIntFreefallEnabled                 (bool enabled);

    bool                      GetIntMotionEnabled                   ();
    void                      SetIntMotionEnabled                   (bool enabled);
    bool                      GetIntZeroMotionEnabled               ();
    void                      SetIntZeroMotionEnabled               (bool enabled);
    bool                      GetIntFIFOBufferOverflowEnabled       ();
    void                      SetIntFIFOBufferOverflowEnabled       (bool enabled);
    bool                      GetIntI2CMasterEnabled                ();
    void                      SetIntI2CMasterEnabled                (bool enabled);
    bool                      GetIntDataReadyEnabled                ();
    void                      SetIntDataReadyEnabled                (bool enabled);
    XBYTE                     GetIntStatus                          ();
    bool                      GetIntFreefallStatus                  ();
    bool                      GetIntMotionStatus                    ();
    bool                      GetIntZeroMotionStatus                ();
    bool                      GetIntFIFOBufferOverflowStatus        ();
    bool                      GetIntI2CMasterStatus                 ();
    bool                      GetIntDataReadyStatus                 ();

    void                      GetMotion9                            (XWORDSIG* ax, XWORDSIG* ay, XWORDSIG* az, XWORDSIG* gx, XWORDSIG* gy, XWORDSIG* gz, XWORDSIG* mx, XWORDSIG* my, XWORDSIG* mz);
    void                      GetMag                                (XWORDSIG* mx, XWORDSIG* my, XWORDSIG* mz);
    void                      GetMotion6                            (XWORDSIG* ax, XWORDSIG* ay, XWORDSIG* az, XWORDSIG* gx, XWORDSIG* gy, XWORDSIG* gz);
    void                      GetAcceleration                       (XWORDSIG* x, XWORDSIG* y, XWORDSIG* z);
    XWORDSIG                  GetAccelerationX                      ();
    XWORDSIG                  GetAccelerationY                      ();
    XWORDSIG                  GetAccelerationZ                      ();
    XWORDSIG                  GetTemperature                        ();
    void                      GetRotation                           (XWORDSIG* x, XWORDSIG* y, XWORDSIG* z);
    XWORDSIG                  GetRotationX                          ();
    XWORDSIG                  GetRotationY                          ();
    XWORDSIG                  GetRotationZ                          ();
    XBYTE                     GetExternalSensorByte                 (int position);
    XWORDSIG                  GetExternalSensorWord                 (int position);
    XDWORD                    GetExternalSensorDWord                (int position);
    bool                      GetXNegMotionDetected                 ();
    bool                      GetXPosMotionDetected                 ();
    bool                      GetYNegMotionDetected                 ();
    bool                      GetYPosMotionDetected                 ();
    bool                      GetZNegMotionDetected                 ();
    bool                      GetZPosMotionDetected                 ();
    bool                      GetZeroMotionDetected                 ();

    void                      SetSlaveOutputByte                    (XBYTE num, XBYTE data);
    bool                      GetExternalShadowDelayEnabled         ();
    void                      SetExternalShadowDelayEnabled         (bool enabled);
    bool                      GetSlaveDelayEnabled                  (XBYTE num);
    void                      SetSlaveDelayEnabled                  (XBYTE num, bool enabled);
    void                      ResetGyroscopePath                    ();
    void                      ResetAccelerometerPath                ();
    void                      ResetTemperaturePath                  ();
    XBYTE                     GetAccelerometerPowerOnDelay          ();
    void                      SetAccelerometerPowerOnDelay          (XBYTE delay);
    XBYTE                     GetFreefallDetectionCounterDecrement  ();
    void                      SetFreefallDetectionCounterDecrement  (XBYTE decrement);
    XBYTE                     GetMotionDetectionCounterDecrement    ();
    void                      SetMotionDetectionCounterDecrement    (XBYTE decrement);
    bool                      GetFIFOEnabled                        ();
    void                      SetFIFOEnabled                        (bool enabled);
    bool                      GetI2CMasterModeEnabled               ();
    void                      SetI2CMasterModeEnabled               (bool enabled);
    void                      SwitchSPIEnabled                      (bool enabled);

    void                      ResetFIFO                             ();
    void                      ResetI2CMaster                        ();
    void                      ResetSensors                          ();
    void                      Reset                                 ();
    bool                      GetSleepEnabled                       ();
    void                      SetSleepEnabled                       (bool enabled);
    bool                      GetWakeCycleEnabled                   ();
    void                      SetWakeCycleEnabled                   (bool enabled);
    bool                      GetTempSensorEnabled                  ();
    void                      SetTempSensorEnabled                  (bool enabled);
    XBYTE                     GetClockSource                        ();
    void                      SetClockSource                        (XBYTE source);
    XBYTE                     GetWakeFrequency                      ();
    void                      SetWakeFrequency                      (XBYTE frequency);
    bool                      GetStandbyXAccelEnabled               ();
    void                      SetStandbyXAccelEnabled               (bool enabled);
    bool                      GetStandbyYAccelEnabled               ();
    void                      SetStandbyYAccelEnabled               (bool enabled);
    bool                      GetStandbyZAccelEnabled               ();
    void                      SetStandbyZAccelEnabled               (bool enabled);
    bool                      GetStandbyXGyroEnabled                ();
    void                      SetStandbyXGyroEnabled                (bool enabled);
    bool                      GetStandbyYGyroEnabled                ();
    void                      SetStandbyYGyroEnabled                (bool enabled);
    bool                      GetStandbyZGyroEnabled                ();
    void                      SetStandbyZGyroEnabled                (bool enabled);
    XWORDSIG                  GetFIFOCount                          ();
    XBYTE                     GetFIFOByte                           ();
    void                      GetFIFOBytes                          (XBYTE* data, XBYTE length);
    void                      SetFIFOByte                           (XBYTE data);
    XBYTE                     GetDeviceID                           ();
    XBYTE                     GetOTPBankValid                       ();
    void                      SetOTPBankValid                       (bool enabled);
    XBYTESIG                  GetXGyroOffset                        ();
    void                      SetXGyroOffset                        (XBYTESIG offset);
    XBYTESIG                  GetYGyroOffset                        ();
    void                      SetYGyroOffset                        (XBYTESIG offset);
    XBYTESIG                  GetZGyroOffset                        ();
    void                      SetZGyroOffset                        (XBYTESIG offset);
    XBYTESIG                  GetXFineGain                          ();
    void                      SetXFineGain                          (XBYTESIG gain);
    XBYTESIG                  GetYFineGain                          ();
    void                      GetYFineGain                          (XBYTESIG gain);
    XBYTESIG                  GetZFineGain                          ();
    void                      SetZFineGain                          (XBYTESIG gain);
    XWORDSIG                  GetXAccelOffset                       ();
    void                      SetXAccelOffset                       (XWORDSIG offset);
    XWORDSIG                  GetYAccelOffset                       ();
    void                      SetYAccelOffset                       (XWORDSIG offset);
    XWORDSIG                  GetZAccelOffset                       ();
    void                      SetZAccelOffset                       (XWORDSIG offset);
    XWORDSIG                  GetXGyroOffsetUser                    ();
    void                      SetXGyroOffsetUser                    (XWORDSIG offset);
    XWORDSIG                  GetYGyroOffsetUser                    ();
    void                      SetYGyroOffsetUser                    (XWORDSIG offset);
    XWORDSIG                  GetZGyroOffsetUser                    ();
    void                      SetZGyroOffsetUser                    (XWORDSIG offset);

    bool                      GetIntPLLReadyEnabled                 ();
    void                      SetIntPLLReadyEnabled                 (bool enabled);
    bool                      GetIntDMPEnabled                      ();
    void                      SetIntDMPEnabled                      (bool enabled);
    bool                      GetDMPInt5Status                      ();
    bool                      GetDMPInt4Status                      ();
    bool                      GetDMPInt3Status                      ();
    bool                      GetDMPInt2Status                      ();
    bool                      GetDMPInt1Status                      ();
    bool                      GetDMPInt0Status                      ();
    bool                      GetIntPLLReadyStatus                  ();
    bool                      GetIntDMPStatus                       ();
    bool                      GetDMPEnabled                         ();
    void                      SetDMPEnabled                         (bool enabled);
    void                      ResetDMP                              ();
    void                      SetMemoryBank                         (XBYTE bank, bool prefetchenabled = false, bool userbank =false);
    void                      SetMemoryStartAddress                 (XBYTE address);
    XBYTE                     ReadMemoryByte                        ();
    void                      WriteMemoryByte                       (XBYTE data);

    XBYTE                     GetDMPConfig1                         ();
    void                      SetDMPConfig1                         (XBYTE config);
    XBYTE                     GetDMPConfig2                         ();
    void                      SetDMPConfig2                         (XBYTE config);

    XBYTESIG                  ReadBit                               (XBYTE deviceaddr, XBYTE regaddr, XBYTE bitnum, XBYTE* data, XWORD timeout = DIOI2CMPU9150_DEFAULTTIMEOUT);
    XBYTESIG                  ReadBitW                              (XBYTE deviceaddr, XBYTE regaddr, XBYTE bitnum, XWORD* data, XWORD timeout = DIOI2CMPU9150_DEFAULTTIMEOUT);
    XBYTESIG                  ReadBits                              (XBYTE deviceaddr, XBYTE regaddr, XBYTE bitstart, XBYTE length, XBYTE* data, XWORD timeout = DIOI2CMPU9150_DEFAULTTIMEOUT);
    XBYTESIG                  ReadBitsW                             (XBYTE deviceaddr, XBYTE regaddr, XBYTE bitstart, XBYTE length, XWORD* data, XWORD timeout = DIOI2CMPU9150_DEFAULTTIMEOUT);
    XBYTESIG                  ReadByte                              (XBYTE deviceaddr, XBYTE regaddr, XBYTE* data, XWORD timeout = DIOI2CMPU9150_DEFAULTTIMEOUT);
    XBYTESIG                  ReadWord                              (XBYTE deviceaddr, XBYTE regaddr, XWORD* data, XWORD timeout = DIOI2CMPU9150_DEFAULTTIMEOUT);
    XBYTESIG                  ReadBytes                             (XBYTE deviceaddr, XBYTE regaddr, XBYTE length, XBYTE* data, XWORD timeout = DIOI2CMPU9150_DEFAULTTIMEOUT);
    XBYTESIG                  ReadWords                             (XBYTE deviceaddr, XBYTE regaddr, XBYTE length, XWORD* data, XWORD timeout = DIOI2CMPU9150_DEFAULTTIMEOUT);

    bool                      WriteBit                              (XBYTE deviceaddr, XBYTE regaddr, XBYTE bitnum, XBYTE data);
    bool                      WriteBitW                             (XBYTE deviceaddr, XBYTE regaddr, XBYTE bitnum, XWORD data);
    bool                      WriteBits                             (XBYTE deviceaddr, XBYTE regaddr, XBYTE bitstart, XBYTE length, XBYTE data);
    bool                      WriteBitsW                            (XBYTE deviceaddr, XBYTE regaddr, XBYTE bitstart, XBYTE length, XWORD data);
    bool                      WriteByte                             (XBYTE deviceaddr, XBYTE regaddr, XBYTE data);
    bool                      WriteWord                             (XBYTE deviceaddr, XBYTE regaddr, XWORD data);
    bool                      WriteBytes                            (XBYTE deviceaddr, XBYTE regaddr, XBYTE length, XBYTE* data);
    bool                      WriteWords                            (XBYTE deviceaddr, XBYTE regaddr, XBYTE length, XWORD* data);

    void                      Clean                                 ();

    XBYTE                     deviceaddr;
    XBYTE                     buffer[16];

};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





