/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOI2C6AxisTrackingBMI270.cpp
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOI2C6AxisTrackingBMI270.h"

#include <stdio.h>
#include <cmath>

#include "XFactory.h"
#include "XSleep.h"
#include "XBuffer.h"
#include "XTrace.h"

#include "DIOFactory.h"
#include "DIOStreamI2CConfig.h"
#include "DIOStreamI2C.h"
#include "DIOGPIO.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/


// Global array that stores the feature input configuration of BMI270 
const struct bmi2_feature_config bmi270_feat_in[DIOI2C6AXISTRACKINGBMI270_MAX_FEAT_IN]      = {   { BMI2_CONFIG_ID                  ,  BMI2_PAGE_1  ,  DIOI2C6AXISTRACKINGBMI270_CONFIG_ID_STRT_ADDR             },
                                                                                                  { BMI2_MAX_BURST_LEN              ,  BMI2_PAGE_1  ,  DIOI2C6AXISTRACKINGBMI270_MAX_BURST_LEN_STRT_ADDR         },
                                                                                                  { BMI2_CRT_GYRO_SELF_TEST         ,  BMI2_PAGE_1  ,  DIOI2C6AXISTRACKINGBMI270_CRT_GYRO_SELF_TEST_STRT_ADDR    },
                                                                                                  { BMI2_ABORT_CRT_GYRO_SELF_TEST   ,  BMI2_PAGE_1  ,  DIOI2C6AXISTRACKINGBMI270_ABORT_STRT_ADDR                 },
                                                                                                  { BMI2_AXIS_MAP                   ,  BMI2_PAGE_1  ,  DIOI2C6AXISTRACKINGBMI270_AXIS_MAP_STRT_ADDR              },
                                                                                                  { BMI2_GYRO_SELF_OFF              ,  BMI2_PAGE_1  ,  DIOI2C6AXISTRACKINGBMI270_GYRO_SELF_OFF_STRT_ADDR         },
                                                                                                  { BMI2_NVM_PROG_PREP              ,  BMI2_PAGE_1  ,  DIOI2C6AXISTRACKINGBMI270_NVM_PROG_PREP_STRT_ADDR         },
                                                                                                  { BMI2_GYRO_GAIN_UPDATE           ,  BMI2_PAGE_1  ,  DIOI2C6AXISTRACKINGBMI270_GYRO_GAIN_UPDATE_STRT_ADDR      },
                                                                                                  { BMI2_ANY_MOTION                 ,  BMI2_PAGE_1  ,  DIOI2C6AXISTRACKINGBMI270_ANY_MOT_STRT_ADDR               },
                                                                                                  { BMI2_NO_MOTION                  ,  BMI2_PAGE_2  ,  DIOI2C6AXISTRACKINGBMI270_NO_MOT_STRT_ADDR                },
                                                                                                  { BMI2_SIG_MOTION                 ,  BMI2_PAGE_2  ,  DIOI2C6AXISTRACKINGBMI270_SIG_MOT_STRT_ADDR               },
                                                                                                  { BMI2_STEP_COUNTER_PARAMS        ,  BMI2_PAGE_3  ,  DIOI2C6AXISTRACKINGBMI270_STEP_CNT_1_STRT_ADDR            },
                                                                                                  { BMI2_STEP_DETECTOR              ,  BMI2_PAGE_6  ,  DIOI2C6AXISTRACKINGBMI270_STEP_CNT_4_STRT_ADDR            },
                                                                                                  { BMI2_STEP_COUNTER               ,  BMI2_PAGE_6  ,  DIOI2C6AXISTRACKINGBMI270_STEP_CNT_4_STRT_ADDR            },
                                                                                                  { BMI2_STEP_ACTIVITY              ,  BMI2_PAGE_6  ,  DIOI2C6AXISTRACKINGBMI270_STEP_CNT_4_STRT_ADDR            },
                                                                                                  { BMI2_WRIST_GESTURE              ,  BMI2_PAGE_6  ,  DIOI2C6AXISTRACKINGBMI270_WRIST_GEST_STRT_ADDR            },
                                                                                                  { BMI2_WRIST_WEAR_WAKE_UP         ,  BMI2_PAGE_7  ,  DIOI2C6AXISTRACKINGBMI270_WRIST_WEAR_WAKE_UP_STRT_ADDR    },
                                                                                              };

// Global array that stores the feature output configuration 
const struct bmi2_feature_config bmi270_feat_out[DIOI2C6AXISTRACKINGBMI270_MAX_FEAT_OUT]    = {   { BMI2_STEP_COUNTER               ,  BMI2_PAGE_0  ,  DIOI2C6AXISTRACKINGBMI270_STEP_CNT_OUT_STRT_ADDR          },
                                                                                                  { BMI2_STEP_ACTIVITY              ,  BMI2_PAGE_0  ,  DIOI2C6AXISTRACKINGBMI270_STEP_ACT_OUT_STRT_ADDR          },
                                                                                                  { BMI2_WRIST_GESTURE              ,  BMI2_PAGE_0  ,  DIOI2C6AXISTRACKINGBMI270_WRIST_GEST_OUT_STRT_ADDR        },
                                                                                                  { BMI2_GYRO_GAIN_UPDATE           ,  BMI2_PAGE_0  ,  DIOI2C6AXISTRACKINGBMI270_GYR_USER_GAIN_OUT_STRT_ADDR     },
                                                                                                  { BMI2_GYRO_CROSS_SENSE           ,  BMI2_PAGE_0  ,  DIOI2C6AXISTRACKINGBMI270_GYRO_CROSS_SENSE_STRT_ADDR      },
                                                                                                  { BMI2_NVM_STATUS                 ,  BMI2_PAGE_0  ,  DIOI2C6AXISTRACKINGBMI270_NVM_VFRM_OUT_STRT_ADDR          },
                                                                                                  { BMI2_VFRM_STATUS                ,  BMI2_PAGE_0  ,  DIOI2C6AXISTRACKINGBMI270_NVM_VFRM_OUT_STRT_ADDR          }
                                                                                              };

// Global array that stores the feature interrupts of BMI270 
struct bmi2_map_int bmi270_map_int[DIOI2C6AXISTRACKINGBMI270_MAX_INT_MAP]                   = {   { BMI2_SIG_MOTION                 ,  DIOI2C6AXISTRACKINGBMI270_INT_SIG_MOT_MASK                                },
                                                                                                  { BMI2_STEP_COUNTER               ,  DIOI2C6AXISTRACKINGBMI270_INT_STEP_COUNTER_MASK                           },
                                                                                                  { BMI2_STEP_DETECTOR              ,  DIOI2C6AXISTRACKINGBMI270_INT_STEP_DETECTOR_MASK                          },
                                                                                                  { BMI2_STEP_ACTIVITY              ,  DIOI2C6AXISTRACKINGBMI270_INT_STEP_ACT_MASK                               },
                                                                                                  { BMI2_WRIST_GESTURE              ,  DIOI2C6AXISTRACKINGBMI270_INT_WRIST_GEST_MASK                             },
                                                                                                  { BMI2_WRIST_WEAR_WAKE_UP         ,  DIOI2C6AXISTRACKINGBMI270_INT_WRIST_WEAR_WAKEUP_MASK                      },
                                                                                                  { BMI2_ANY_MOTION                 ,  DIOI2C6AXISTRACKINGBMI270_INT_ANY_MOT_MASK                                },
                                                                                                  { BMI2_NO_MOTION                  ,  DIOI2C6AXISTRACKINGBMI270_INT_NO_MOT_MASK                                 },
                                                                                              };

 
// Select features and their pins to be mapped to.
struct bmi2_sens_int_config sens_int[4]                                                     = {   { BMI2_ANY_MOTION                 ,  BMI2_INT1                                              },
                                                                                                  { BMI2_NO_MOTION                  ,  BMI2_INT1                                              },
                                                                                                  { BMI2_STEP_DETECTOR              ,  BMI2_INT1                                              },			
                                                                                                  { BMI2_SIG_MOTION                 ,  BMI2_INT1                                              },			
                                                                                              };



// Global array that stores the configuration file of BMI270 
const XBYTE bmi270_config_file[]                                                            = { 0xc8, 0x2e, 0x00, 0x2e, 0x80, 0x2e, 0x3d, 0xb1, 0xc8, 0x2e, 0x00, 0x2e, 0x80, 0x2e, 0x91, 0x03, 0x80, 0x2e, 0xbc,
                                                                                                0xb0, 0x80, 0x2e, 0xa3, 0x03, 0xc8, 0x2e, 0x00, 0x2e, 0x80, 0x2e, 0x00, 0xb0, 0x50, 0x30, 0x21, 0x2e, 0x59, 0xf5,
                                                                                                0x10, 0x30, 0x21, 0x2e, 0x6a, 0xf5, 0x80, 0x2e, 0x3b, 0x03, 0x00, 0x00, 0x00, 0x00, 0x08, 0x19, 0x01, 0x00, 0x22,
                                                                                                0x00, 0x75, 0x00, 0x00, 0x10, 0x00, 0x10, 0xd1, 0x00, 0xb3, 0x43, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1,
                                                                                                0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00,
                                                                                                0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e,
                                                                                                0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80,
                                                                                                0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1,
                                                                                                0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00,
                                                                                                0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e,
                                                                                                0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80,
                                                                                                0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1,
                                                                                                0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0xe0, 0x5f, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
                                                                                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x92, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x19, 0x00, 0x00, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05,
                                                                                                0xe0, 0xaa, 0x38, 0x05, 0xe0, 0x90, 0x30, 0xfa, 0x00, 0x96, 0x00, 0x4b, 0x09, 0x11, 0x00, 0x11, 0x00, 0x02, 0x00,
                                                                                                0x2d, 0x01, 0xd4, 0x7b, 0x3b, 0x01, 0xdb, 0x7a, 0x04, 0x00, 0x3f, 0x7b, 0xcd, 0x6c, 0xc3, 0x04, 0x85, 0x09, 0xc3,
                                                                                                0x04, 0xec, 0xe6, 0x0c, 0x46, 0x01, 0x00, 0x27, 0x00, 0x19, 0x00, 0x96, 0x00, 0xa0, 0x00, 0x01, 0x00, 0x0c, 0x00,
                                                                                                0xf0, 0x3c, 0x00, 0x01, 0x01, 0x00, 0x03, 0x00, 0x01, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x32, 0x00, 0x05, 0x00, 0xee,
                                                                                                0x06, 0x04, 0x00, 0xc8, 0x00, 0x00, 0x00, 0x04, 0x00, 0xa8, 0x05, 0xee, 0x06, 0x00, 0x04, 0xbc, 0x02, 0xb3, 0x00,
                                                                                                0x85, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                                                0x00, 0x00, 0x00, 0xb4, 0x00, 0x01, 0x00, 0xb9, 0x00, 0x01, 0x00, 0x98, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                                                0x00, 0x00, 0x01, 0x00, 0x80, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x2e, 0x00, 0xc1, 0xfd, 0x2d, 0xde,
                                                                                                0x00, 0xeb, 0x00, 0xda, 0x00, 0x00, 0x0c, 0xff, 0x0f, 0x00, 0x04, 0xc0, 0x00, 0x5b, 0xf5, 0xc9, 0x01, 0x1e, 0xf2,
                                                                                                0x80, 0x00, 0x3f, 0xff, 0x19, 0xf4, 0x58, 0xf5, 0x66, 0xf5, 0x64, 0xf5, 0xc0, 0xf1, 0xf0, 0x00, 0xe0, 0x00, 0xcd,
                                                                                                0x01, 0xd3, 0x01, 0xdb, 0x01, 0xff, 0x7f, 0xff, 0x01, 0xe4, 0x00, 0x74, 0xf7, 0xf3, 0x00, 0xfa, 0x00, 0xff, 0x3f,
                                                                                                0xca, 0x03, 0x6c, 0x38, 0x56, 0xfe, 0x44, 0xfd, 0xbc, 0x02, 0xf9, 0x06, 0x00, 0xfc, 0x12, 0x02, 0xae, 0x01, 0x58,
                                                                                                0xfa, 0x9a, 0xfd, 0x77, 0x05, 0xbb, 0x02, 0x96, 0x01, 0x95, 0x01, 0x7f, 0x01, 0x82, 0x01, 0x89, 0x01, 0x87, 0x01,
                                                                                                0x88, 0x01, 0x8a, 0x01, 0x8c, 0x01, 0x8f, 0x01, 0x8d, 0x01, 0x92, 0x01, 0x91, 0x01, 0xdd, 0x00, 0x9f, 0x01, 0x7e,
                                                                                                0x01, 0xdb, 0x00, 0xb6, 0x01, 0x70, 0x69, 0x26, 0xd3, 0x9c, 0x07, 0x1f, 0x05, 0x9d, 0x00, 0x00, 0x08, 0xbc, 0x05,
                                                                                                0x37, 0xfa, 0xa2, 0x01, 0xaa, 0x01, 0xa1, 0x01, 0xa8, 0x01, 0xa0, 0x01, 0xa8, 0x05, 0xb4, 0x01, 0xb4, 0x01, 0xce,
                                                                                                0x00, 0xd0, 0x00, 0xfc, 0x00, 0xc5, 0x01, 0xff, 0xfb, 0xb1, 0x00, 0x00, 0x38, 0x00, 0x30, 0xfd, 0xf5, 0xfc, 0xf5,
                                                                                                0xcd, 0x01, 0xa0, 0x00, 0x5f, 0xff, 0x00, 0x40, 0xff, 0x00, 0x00, 0x80, 0x6d, 0x0f, 0xeb, 0x00, 0x7f, 0xff, 0xc2,
                                                                                                0xf5, 0x68, 0xf7, 0xb3, 0xf1, 0x67, 0x0f, 0x5b, 0x0f, 0x61, 0x0f, 0x80, 0x0f, 0x58, 0xf7, 0x5b, 0xf7, 0x83, 0x0f,
                                                                                                0x86, 0x00, 0x72, 0x0f, 0x85, 0x0f, 0xc6, 0xf1, 0x7f, 0x0f, 0x6c, 0xf7, 0x00, 0xe0, 0x00, 0xff, 0xd1, 0xf5, 0x87,
                                                                                                0x0f, 0x8a, 0x0f, 0xff, 0x03, 0xf0, 0x3f, 0x8b, 0x00, 0x8e, 0x00, 0x90, 0x00, 0xb9, 0x00, 0x2d, 0xf5, 0xca, 0xf5,
                                                                                                0xcb, 0x01, 0x20, 0xf2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x50, 0x98, 0x2e,
                                                                                                0xd7, 0x0e, 0x50, 0x32, 0x98, 0x2e, 0xfa, 0x03, 0x00, 0x30, 0xf0, 0x7f, 0x00, 0x2e, 0x00, 0x2e, 0xd0, 0x2e, 0x00,
                                                                                                0x2e, 0x01, 0x80, 0x08, 0xa2, 0xfb, 0x2f, 0x98, 0x2e, 0xba, 0x03, 0x21, 0x2e, 0x19, 0x00, 0x01, 0x2e, 0xee, 0x00,
                                                                                                0x00, 0xb2, 0x07, 0x2f, 0x01, 0x2e, 0x19, 0x00, 0x00, 0xb2, 0x03, 0x2f, 0x01, 0x50, 0x03, 0x52, 0x98, 0x2e, 0x07,
                                                                                                0xcc, 0x01, 0x2e, 0xdd, 0x00, 0x00, 0xb2, 0x27, 0x2f, 0x05, 0x2e, 0x8a, 0x00, 0x05, 0x52, 0x98, 0x2e, 0xc7, 0xc1,
                                                                                                0x03, 0x2e, 0xe9, 0x00, 0x40, 0xb2, 0xf0, 0x7f, 0x08, 0x2f, 0x01, 0x2e, 0x19, 0x00, 0x00, 0xb2, 0x04, 0x2f, 0x00,
                                                                                                0x30, 0x21, 0x2e, 0xe9, 0x00, 0x98, 0x2e, 0xb4, 0xb1, 0x01, 0x2e, 0x18, 0x00, 0x00, 0xb2, 0x10, 0x2f, 0x05, 0x50,
                                                                                                0x98, 0x2e, 0x4d, 0xc3, 0x05, 0x50, 0x98, 0x2e, 0x5a, 0xc7, 0x98, 0x2e, 0xf9, 0xb4, 0x98, 0x2e, 0x54, 0xb2, 0x98,
                                                                                                0x2e, 0x67, 0xb6, 0x98, 0x2e, 0x17, 0xb2, 0x10, 0x30, 0x21, 0x2e, 0x77, 0x00, 0x01, 0x2e, 0xef, 0x00, 0x00, 0xb2,
                                                                                                0x04, 0x2f, 0x98, 0x2e, 0x7a, 0xb7, 0x00, 0x30, 0x21, 0x2e, 0xef, 0x00, 0x01, 0x2e, 0xd4, 0x00, 0x04, 0xae, 0x0b,
                                                                                                0x2f, 0x01, 0x2e, 0xdd, 0x00, 0x00, 0xb2, 0x07, 0x2f, 0x05, 0x52, 0x98, 0x2e, 0x8e, 0x0e, 0x00, 0xb2, 0x02, 0x2f,
                                                                                                0x10, 0x30, 0x21, 0x2e, 0x7d, 0x00, 0x01, 0x2e, 0x7d, 0x00, 0x00, 0x90, 0x90, 0x2e, 0xf1, 0x02, 0x01, 0x2e, 0xd7,
                                                                                                0x00, 0x00, 0xb2, 0x04, 0x2f, 0x98, 0x2e, 0x2f, 0x0e, 0x00, 0x30, 0x21, 0x2e, 0x7b, 0x00, 0x01, 0x2e, 0x7b, 0x00,
                                                                                                0x00, 0xb2, 0x12, 0x2f, 0x01, 0x2e, 0xd4, 0x00, 0x00, 0x90, 0x02, 0x2f, 0x98, 0x2e, 0x1f, 0x0e, 0x09, 0x2d, 0x98,
                                                                                                0x2e, 0x81, 0x0d, 0x01, 0x2e, 0xd4, 0x00, 0x04, 0x90, 0x02, 0x2f, 0x50, 0x32, 0x98, 0x2e, 0xfa, 0x03, 0x00, 0x30,
                                                                                                0x21, 0x2e, 0x7b, 0x00, 0x01, 0x2e, 0x7c, 0x00, 0x00, 0xb2, 0x90, 0x2e, 0x09, 0x03, 0x01, 0x2e, 0x7c, 0x00, 0x01,
                                                                                                0x31, 0x01, 0x08, 0x00, 0xb2, 0x04, 0x2f, 0x98, 0x2e, 0x47, 0xcb, 0x10, 0x30, 0x21, 0x2e, 0x77, 0x00, 0x81, 0x30,
                                                                                                0x01, 0x2e, 0x7c, 0x00, 0x01, 0x08, 0x00, 0xb2, 0x61, 0x2f, 0x03, 0x2e, 0x89, 0x00, 0x01, 0x2e, 0xd4, 0x00, 0x98,
                                                                                                0xbc, 0x98, 0xb8, 0x05, 0xb2, 0x0f, 0x58, 0x23, 0x2f, 0x07, 0x90, 0x09, 0x54, 0x00, 0x30, 0x37, 0x2f, 0x15, 0x41,
                                                                                                0x04, 0x41, 0xdc, 0xbe, 0x44, 0xbe, 0xdc, 0xba, 0x2c, 0x01, 0x61, 0x00, 0x0f, 0x56, 0x4a, 0x0f, 0x0c, 0x2f, 0xd1,
                                                                                                0x42, 0x94, 0xb8, 0xc1, 0x42, 0x11, 0x30, 0x05, 0x2e, 0x6a, 0xf7, 0x2c, 0xbd, 0x2f, 0xb9, 0x80, 0xb2, 0x08, 0x22,
                                                                                                0x98, 0x2e, 0xc3, 0xb7, 0x21, 0x2d, 0x61, 0x30, 0x23, 0x2e, 0xd4, 0x00, 0x98, 0x2e, 0xc3, 0xb7, 0x00, 0x30, 0x21,
                                                                                                0x2e, 0x5a, 0xf5, 0x18, 0x2d, 0xe1, 0x7f, 0x50, 0x30, 0x98, 0x2e, 0xfa, 0x03, 0x0f, 0x52, 0x07, 0x50, 0x50, 0x42,
                                                                                                0x70, 0x30, 0x0d, 0x54, 0x42, 0x42, 0x7e, 0x82, 0xe2, 0x6f, 0x80, 0xb2, 0x42, 0x42, 0x05, 0x2f, 0x21, 0x2e, 0xd4,
                                                                                                0x00, 0x10, 0x30, 0x98, 0x2e, 0xc3, 0xb7, 0x03, 0x2d, 0x60, 0x30, 0x21, 0x2e, 0xd4, 0x00, 0x01, 0x2e, 0xd4, 0x00,
                                                                                                0x06, 0x90, 0x18, 0x2f, 0x01, 0x2e, 0x76, 0x00, 0x0b, 0x54, 0x07, 0x52, 0xe0, 0x7f, 0x98, 0x2e, 0x7a, 0xc1, 0xe1,
                                                                                                0x6f, 0x08, 0x1a, 0x40, 0x30, 0x08, 0x2f, 0x21, 0x2e, 0xd4, 0x00, 0x20, 0x30, 0x98, 0x2e, 0xaf, 0xb7, 0x50, 0x32,
                                                                                                0x98, 0x2e, 0xfa, 0x03, 0x05, 0x2d, 0x98, 0x2e, 0x38, 0x0e, 0x00, 0x30, 0x21, 0x2e, 0xd4, 0x00, 0x00, 0x30, 0x21,
                                                                                                0x2e, 0x7c, 0x00, 0x18, 0x2d, 0x01, 0x2e, 0xd4, 0x00, 0x03, 0xaa, 0x01, 0x2f, 0x98, 0x2e, 0x45, 0x0e, 0x01, 0x2e,
                                                                                                0xd4, 0x00, 0x3f, 0x80, 0x03, 0xa2, 0x01, 0x2f, 0x00, 0x2e, 0x02, 0x2d, 0x98, 0x2e, 0x5b, 0x0e, 0x30, 0x30, 0x98,
                                                                                                0x2e, 0xce, 0xb7, 0x00, 0x30, 0x21, 0x2e, 0x7d, 0x00, 0x50, 0x32, 0x98, 0x2e, 0xfa, 0x03, 0x01, 0x2e, 0x77, 0x00,
                                                                                                0x00, 0xb2, 0x24, 0x2f, 0x98, 0x2e, 0xf5, 0xcb, 0x03, 0x2e, 0xd5, 0x00, 0x11, 0x54, 0x01, 0x0a, 0xbc, 0x84, 0x83,
                                                                                                0x86, 0x21, 0x2e, 0xc9, 0x01, 0xe0, 0x40, 0x13, 0x52, 0xc4, 0x40, 0x82, 0x40, 0xa8, 0xb9, 0x52, 0x42, 0x43, 0xbe,
                                                                                                0x53, 0x42, 0x04, 0x0a, 0x50, 0x42, 0xe1, 0x7f, 0xf0, 0x31, 0x41, 0x40, 0xf2, 0x6f, 0x25, 0xbd, 0x08, 0x08, 0x02,
                                                                                                0x0a, 0xd0, 0x7f, 0x98, 0x2e, 0xa8, 0xcf, 0x06, 0xbc, 0xd1, 0x6f, 0xe2, 0x6f, 0x08, 0x0a, 0x80, 0x42, 0x98, 0x2e,
                                                                                                0x58, 0xb7, 0x00, 0x30, 0x21, 0x2e, 0xee, 0x00, 0x21, 0x2e, 0x77, 0x00, 0x21, 0x2e, 0xdd, 0x00, 0x80, 0x2e, 0xf4,
                                                                                                0x01, 0x1a, 0x24, 0x22, 0x00, 0x80, 0x2e, 0xec, 0x01, 0x10, 0x50, 0xfb, 0x7f, 0x98, 0x2e, 0xf3, 0x03, 0x57, 0x50,
                                                                                                0xfb, 0x6f, 0x01, 0x30, 0x71, 0x54, 0x11, 0x42, 0x42, 0x0e, 0xfc, 0x2f, 0xc0, 0x2e, 0x01, 0x42, 0xf0, 0x5f, 0x80,
                                                                                                0x2e, 0x00, 0xc1, 0xfd, 0x2d, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9a, 0x01,
                                                                                                0x34, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                                                0x00, 0x00, 0x20, 0x50, 0xe7, 0x7f, 0xf6, 0x7f, 0x06, 0x32, 0x0f, 0x2e, 0x61, 0xf5, 0xfe, 0x09, 0xc0, 0xb3, 0x04,
                                                                                                0x2f, 0x17, 0x30, 0x2f, 0x2e, 0xef, 0x00, 0x2d, 0x2e, 0x61, 0xf5, 0xf6, 0x6f, 0xe7, 0x6f, 0xe0, 0x5f, 0xc8, 0x2e,
                                                                                                0x20, 0x50, 0xe7, 0x7f, 0xf6, 0x7f, 0x46, 0x30, 0x0f, 0x2e, 0xa4, 0xf1, 0xbe, 0x09, 0x80, 0xb3, 0x06, 0x2f, 0x0d,
                                                                                                0x2e, 0xd4, 0x00, 0x84, 0xaf, 0x02, 0x2f, 0x16, 0x30, 0x2d, 0x2e, 0x7b, 0x00, 0x86, 0x30, 0x2d, 0x2e, 0x60, 0xf5,
                                                                                                0xf6, 0x6f, 0xe7, 0x6f, 0xe0, 0x5f, 0xc8, 0x2e, 0x01, 0x2e, 0x77, 0xf7, 0x09, 0xbc, 0x0f, 0xb8, 0x00, 0xb2, 0x10,
                                                                                                0x50, 0xfb, 0x7f, 0x10, 0x30, 0x0b, 0x2f, 0x03, 0x2e, 0x8a, 0x00, 0x96, 0xbc, 0x9f, 0xb8, 0x40, 0xb2, 0x05, 0x2f,
                                                                                                0x03, 0x2e, 0x68, 0xf7, 0x9e, 0xbc, 0x9f, 0xb8, 0x40, 0xb2, 0x07, 0x2f, 0x03, 0x2e, 0x7e, 0x00, 0x41, 0x90, 0x01,
                                                                                                0x2f, 0x98, 0x2e, 0xdc, 0x03, 0x03, 0x2c, 0x00, 0x30, 0x21, 0x2e, 0x7e, 0x00, 0xfb, 0x6f, 0xf0, 0x5f, 0xb8, 0x2e,
                                                                                                0x20, 0x50, 0xe0, 0x7f, 0xfb, 0x7f, 0x00, 0x2e, 0x27, 0x50, 0x98, 0x2e, 0x3b, 0xc8, 0x29, 0x50, 0x98, 0x2e, 0xa7,
                                                                                                0xc8, 0x01, 0x50, 0x98, 0x2e, 0x55, 0xcc, 0xe1, 0x6f, 0x2b, 0x50, 0x98, 0x2e, 0xe0, 0xc9, 0xfb, 0x6f, 0x00, 0x30,
                                                                                                0xe0, 0x5f, 0x21, 0x2e, 0x7e, 0x00, 0xb8, 0x2e, 0x73, 0x50, 0x01, 0x30, 0x57, 0x54, 0x11, 0x42, 0x42, 0x0e, 0xfc,
                                                                                                0x2f, 0xb8, 0x2e, 0x21, 0x2e, 0x59, 0xf5, 0x10, 0x30, 0xc0, 0x2e, 0x21, 0x2e, 0x4a, 0xf1, 0x90, 0x50, 0xf7, 0x7f,
                                                                                                0xe6, 0x7f, 0xd5, 0x7f, 0xc4, 0x7f, 0xb3, 0x7f, 0xa1, 0x7f, 0x90, 0x7f, 0x82, 0x7f, 0x7b, 0x7f, 0x98, 0x2e, 0x35,
                                                                                                0xb7, 0x00, 0xb2, 0x90, 0x2e, 0x97, 0xb0, 0x03, 0x2e, 0x8f, 0x00, 0x07, 0x2e, 0x91, 0x00, 0x05, 0x2e, 0xb1, 0x00,
                                                                                                0x3f, 0xba, 0x9f, 0xb8, 0x01, 0x2e, 0xb1, 0x00, 0xa3, 0xbd, 0x4c, 0x0a, 0x05, 0x2e, 0xb1, 0x00, 0x04, 0xbe, 0xbf,
                                                                                                0xb9, 0xcb, 0x0a, 0x4f, 0xba, 0x22, 0xbd, 0x01, 0x2e, 0xb3, 0x00, 0xdc, 0x0a, 0x2f, 0xb9, 0x03, 0x2e, 0xb8, 0x00,
                                                                                                0x0a, 0xbe, 0x9a, 0x0a, 0xcf, 0xb9, 0x9b, 0xbc, 0x01, 0x2e, 0x97, 0x00, 0x9f, 0xb8, 0x93, 0x0a, 0x0f, 0xbc, 0x91,
                                                                                                0x0a, 0x0f, 0xb8, 0x90, 0x0a, 0x25, 0x2e, 0x18, 0x00, 0x05, 0x2e, 0xc1, 0xf5, 0x2e, 0xbd, 0x2e, 0xb9, 0x01, 0x2e,
                                                                                                0x19, 0x00, 0x31, 0x30, 0x8a, 0x04, 0x00, 0x90, 0x07, 0x2f, 0x01, 0x2e, 0xd4, 0x00, 0x04, 0xa2, 0x03, 0x2f, 0x01,
                                                                                                0x2e, 0x18, 0x00, 0x00, 0xb2, 0x0c, 0x2f, 0x19, 0x50, 0x05, 0x52, 0x98, 0x2e, 0x4d, 0xb7, 0x05, 0x2e, 0x78, 0x00,
                                                                                                0x80, 0x90, 0x10, 0x30, 0x01, 0x2f, 0x21, 0x2e, 0x78, 0x00, 0x25, 0x2e, 0xdd, 0x00, 0x98, 0x2e, 0x3e, 0xb7, 0x00,
                                                                                                0xb2, 0x02, 0x30, 0x01, 0x30, 0x04, 0x2f, 0x01, 0x2e, 0x19, 0x00, 0x00, 0xb2, 0x00, 0x2f, 0x21, 0x30, 0x01, 0x2e,
                                                                                                0xea, 0x00, 0x08, 0x1a, 0x0e, 0x2f, 0x23, 0x2e, 0xea, 0x00, 0x33, 0x30, 0x1b, 0x50, 0x0b, 0x09, 0x01, 0x40, 0x17,
                                                                                                0x56, 0x46, 0xbe, 0x4b, 0x08, 0x4c, 0x0a, 0x01, 0x42, 0x0a, 0x80, 0x15, 0x52, 0x01, 0x42, 0x00, 0x2e, 0x01, 0x2e,
                                                                                                0x18, 0x00, 0x00, 0xb2, 0x1f, 0x2f, 0x03, 0x2e, 0xc0, 0xf5, 0xf0, 0x30, 0x48, 0x08, 0x47, 0xaa, 0x74, 0x30, 0x07,
                                                                                                0x2e, 0x7a, 0x00, 0x61, 0x22, 0x4b, 0x1a, 0x05, 0x2f, 0x07, 0x2e, 0x66, 0xf5, 0xbf, 0xbd, 0xbf, 0xb9, 0xc0, 0x90,
                                                                                                0x0b, 0x2f, 0x1d, 0x56, 0x2b, 0x30, 0xd2, 0x42, 0xdb, 0x42, 0x01, 0x04, 0xc2, 0x42, 0x04, 0xbd, 0xfe, 0x80, 0x81,
                                                                                                0x84, 0x23, 0x2e, 0x7a, 0x00, 0x02, 0x42, 0x02, 0x32, 0x25, 0x2e, 0x62, 0xf5, 0x05, 0x2e, 0xd6, 0x00, 0x81, 0x84,
                                                                                                0x25, 0x2e, 0xd6, 0x00, 0x02, 0x31, 0x25, 0x2e, 0x60, 0xf5, 0x05, 0x2e, 0x8a, 0x00, 0x0b, 0x50, 0x90, 0x08, 0x80,
                                                                                                0xb2, 0x0b, 0x2f, 0x05, 0x2e, 0xca, 0xf5, 0xf0, 0x3e, 0x90, 0x08, 0x25, 0x2e, 0xca, 0xf5, 0x05, 0x2e, 0x59, 0xf5,
                                                                                                0xe0, 0x3f, 0x90, 0x08, 0x25, 0x2e, 0x59, 0xf5, 0x90, 0x6f, 0xa1, 0x6f, 0xb3, 0x6f, 0xc4, 0x6f, 0xd5, 0x6f, 0xe6,
                                                                                                0x6f, 0xf7, 0x6f, 0x7b, 0x6f, 0x82, 0x6f, 0x70, 0x5f, 0xc8, 0x2e, 0xc0, 0x50, 0x90, 0x7f, 0xe5, 0x7f, 0xd4, 0x7f,
                                                                                                0xc3, 0x7f, 0xb1, 0x7f, 0xa2, 0x7f, 0x87, 0x7f, 0xf6, 0x7f, 0x7b, 0x7f, 0x00, 0x2e, 0x01, 0x2e, 0x60, 0xf5, 0x60,
                                                                                                0x7f, 0x98, 0x2e, 0x35, 0xb7, 0x02, 0x30, 0x63, 0x6f, 0x15, 0x52, 0x50, 0x7f, 0x62, 0x7f, 0x5a, 0x2c, 0x02, 0x32,
                                                                                                0x1a, 0x09, 0x00, 0xb3, 0x14, 0x2f, 0x00, 0xb2, 0x03, 0x2f, 0x09, 0x2e, 0x18, 0x00, 0x00, 0x91, 0x0c, 0x2f, 0x43,
                                                                                                0x7f, 0x98, 0x2e, 0x97, 0xb7, 0x1f, 0x50, 0x02, 0x8a, 0x02, 0x32, 0x04, 0x30, 0x25, 0x2e, 0x64, 0xf5, 0x15, 0x52,
                                                                                                0x50, 0x6f, 0x43, 0x6f, 0x44, 0x43, 0x25, 0x2e, 0x60, 0xf5, 0xd9, 0x08, 0xc0, 0xb2, 0x36, 0x2f, 0x98, 0x2e, 0x3e,
                                                                                                0xb7, 0x00, 0xb2, 0x06, 0x2f, 0x01, 0x2e, 0x19, 0x00, 0x00, 0xb2, 0x02, 0x2f, 0x50, 0x6f, 0x00, 0x90, 0x0a, 0x2f,
                                                                                                0x01, 0x2e, 0x79, 0x00, 0x00, 0x90, 0x19, 0x2f, 0x10, 0x30, 0x21, 0x2e, 0x79, 0x00, 0x00, 0x30, 0x98, 0x2e, 0xdc,
                                                                                                0x03, 0x13, 0x2d, 0x01, 0x2e, 0xc3, 0xf5, 0x0c, 0xbc, 0x0f, 0xb8, 0x12, 0x30, 0x10, 0x04, 0x03, 0xb0, 0x26, 0x25,
                                                                                                0x21, 0x50, 0x03, 0x52, 0x98, 0x2e, 0x4d, 0xb7, 0x10, 0x30, 0x21, 0x2e, 0xee, 0x00, 0x02, 0x30, 0x60, 0x7f, 0x25,
                                                                                                0x2e, 0x79, 0x00, 0x60, 0x6f, 0x00, 0x90, 0x05, 0x2f, 0x00, 0x30, 0x21, 0x2e, 0xea, 0x00, 0x15, 0x50, 0x21, 0x2e,
                                                                                                0x64, 0xf5, 0x15, 0x52, 0x23, 0x2e, 0x60, 0xf5, 0x02, 0x32, 0x50, 0x6f, 0x00, 0x90, 0x02, 0x2f, 0x03, 0x30, 0x27,
                                                                                                0x2e, 0x78, 0x00, 0x07, 0x2e, 0x60, 0xf5, 0x1a, 0x09, 0x00, 0x91, 0xa3, 0x2f, 0x19, 0x09, 0x00, 0x91, 0xa0, 0x2f,
                                                                                                0x90, 0x6f, 0xa2, 0x6f, 0xb1, 0x6f, 0xc3, 0x6f, 0xd4, 0x6f, 0xe5, 0x6f, 0x7b, 0x6f, 0xf6, 0x6f, 0x87, 0x6f, 0x40,
                                                                                                0x5f, 0xc8, 0x2e, 0xc0, 0x50, 0xe7, 0x7f, 0xf6, 0x7f, 0x26, 0x30, 0x0f, 0x2e, 0x61, 0xf5, 0x2f, 0x2e, 0x7c, 0x00,
                                                                                                0x0f, 0x2e, 0x7c, 0x00, 0xbe, 0x09, 0xa2, 0x7f, 0x80, 0x7f, 0x80, 0xb3, 0xd5, 0x7f, 0xc4, 0x7f, 0xb3, 0x7f, 0x91,
                                                                                                0x7f, 0x7b, 0x7f, 0x0b, 0x2f, 0x23, 0x50, 0x1a, 0x25, 0x12, 0x40, 0x42, 0x7f, 0x74, 0x82, 0x12, 0x40, 0x52, 0x7f,
                                                                                                0x00, 0x2e, 0x00, 0x40, 0x60, 0x7f, 0x98, 0x2e, 0x6a, 0xd6, 0x81, 0x30, 0x01, 0x2e, 0x7c, 0x00, 0x01, 0x08, 0x00,
                                                                                                0xb2, 0x42, 0x2f, 0x03, 0x2e, 0x89, 0x00, 0x01, 0x2e, 0x89, 0x00, 0x97, 0xbc, 0x06, 0xbc, 0x9f, 0xb8, 0x0f, 0xb8,
                                                                                                0x00, 0x90, 0x23, 0x2e, 0xd8, 0x00, 0x10, 0x30, 0x01, 0x30, 0x2a, 0x2f, 0x03, 0x2e, 0xd4, 0x00, 0x44, 0xb2, 0x05,
                                                                                                0x2f, 0x47, 0xb2, 0x00, 0x30, 0x2d, 0x2f, 0x21, 0x2e, 0x7c, 0x00, 0x2b, 0x2d, 0x03, 0x2e, 0xfd, 0xf5, 0x9e, 0xbc,
                                                                                                0x9f, 0xb8, 0x40, 0x90, 0x14, 0x2f, 0x03, 0x2e, 0xfc, 0xf5, 0x99, 0xbc, 0x9f, 0xb8, 0x40, 0x90, 0x0e, 0x2f, 0x03,
                                                                                                0x2e, 0x49, 0xf1, 0x25, 0x54, 0x4a, 0x08, 0x40, 0x90, 0x08, 0x2f, 0x98, 0x2e, 0x35, 0xb7, 0x00, 0xb2, 0x10, 0x30,
                                                                                                0x03, 0x2f, 0x50, 0x30, 0x21, 0x2e, 0xd4, 0x00, 0x10, 0x2d, 0x98, 0x2e, 0xaf, 0xb7, 0x00, 0x30, 0x21, 0x2e, 0x7c,
                                                                                                0x00, 0x0a, 0x2d, 0x05, 0x2e, 0x69, 0xf7, 0x2d, 0xbd, 0x2f, 0xb9, 0x80, 0xb2, 0x01, 0x2f, 0x21, 0x2e, 0x7d, 0x00,
                                                                                                0x23, 0x2e, 0x7c, 0x00, 0xe0, 0x31, 0x21, 0x2e, 0x61, 0xf5, 0xf6, 0x6f, 0xe7, 0x6f, 0x80, 0x6f, 0xa2, 0x6f, 0xb3,
                                                                                                0x6f, 0xc4, 0x6f, 0xd5, 0x6f, 0x7b, 0x6f, 0x91, 0x6f, 0x40, 0x5f, 0xc8, 0x2e, 0x60, 0x51, 0x0a, 0x25, 0x36, 0x88,
                                                                                                0xf4, 0x7f, 0xeb, 0x7f, 0x00, 0x32, 0x31, 0x52, 0x32, 0x30, 0x13, 0x30, 0x98, 0x2e, 0x15, 0xcb, 0x0a, 0x25, 0x33,
                                                                                                0x84, 0xd2, 0x7f, 0x43, 0x30, 0x05, 0x50, 0x2d, 0x52, 0x98, 0x2e, 0x95, 0xc1, 0xd2, 0x6f, 0x27, 0x52, 0x98, 0x2e,
                                                                                                0xd7, 0xc7, 0x2a, 0x25, 0xb0, 0x86, 0xc0, 0x7f, 0xd3, 0x7f, 0xaf, 0x84, 0x29, 0x50, 0xf1, 0x6f, 0x98, 0x2e, 0x4d,
                                                                                                0xc8, 0x2a, 0x25, 0xae, 0x8a, 0xaa, 0x88, 0xf2, 0x6e, 0x2b, 0x50, 0xc1, 0x6f, 0xd3, 0x6f, 0xf4, 0x7f, 0x98, 0x2e,
                                                                                                0xb6, 0xc8, 0xe0, 0x6e, 0x00, 0xb2, 0x32, 0x2f, 0x33, 0x54, 0x83, 0x86, 0xf1, 0x6f, 0xc3, 0x7f, 0x04, 0x30, 0x30,
                                                                                                0x30, 0xf4, 0x7f, 0xd0, 0x7f, 0xb2, 0x7f, 0xe3, 0x30, 0xc5, 0x6f, 0x56, 0x40, 0x45, 0x41, 0x28, 0x08, 0x03, 0x14,
                                                                                                0x0e, 0xb4, 0x08, 0xbc, 0x82, 0x40, 0x10, 0x0a, 0x2f, 0x54, 0x26, 0x05, 0x91, 0x7f, 0x44, 0x28, 0xa3, 0x7f, 0x98,
                                                                                                0x2e, 0xd9, 0xc0, 0x08, 0xb9, 0x33, 0x30, 0x53, 0x09, 0xc1, 0x6f, 0xd3, 0x6f, 0xf4, 0x6f, 0x83, 0x17, 0x47, 0x40,
                                                                                                0x6c, 0x15, 0xb2, 0x6f, 0xbe, 0x09, 0x75, 0x0b, 0x90, 0x42, 0x45, 0x42, 0x51, 0x0e, 0x32, 0xbc, 0x02, 0x89, 0xa1,
                                                                                                0x6f, 0x7e, 0x86, 0xf4, 0x7f, 0xd0, 0x7f, 0xb2, 0x7f, 0x04, 0x30, 0x91, 0x6f, 0xd6, 0x2f, 0xeb, 0x6f, 0xa0, 0x5e,
                                                                                                0xb8, 0x2e, 0x03, 0x2e, 0x97, 0x00, 0x1b, 0xbc, 0x60, 0x50, 0x9f, 0xbc, 0x0c, 0xb8, 0xf0, 0x7f, 0x40, 0xb2, 0xeb,
                                                                                                0x7f, 0x2b, 0x2f, 0x03, 0x2e, 0x7f, 0x00, 0x41, 0x40, 0x01, 0x2e, 0xc8, 0x00, 0x01, 0x1a, 0x11, 0x2f, 0x37, 0x58,
                                                                                                0x23, 0x2e, 0xc8, 0x00, 0x10, 0x41, 0xa0, 0x7f, 0x38, 0x81, 0x01, 0x41, 0xd0, 0x7f, 0xb1, 0x7f, 0x98, 0x2e, 0x64,
                                                                                                0xcf, 0xd0, 0x6f, 0x07, 0x80, 0xa1, 0x6f, 0x11, 0x42, 0x00, 0x2e, 0xb1, 0x6f, 0x01, 0x42, 0x11, 0x30, 0x01, 0x2e,
                                                                                                0xfc, 0x00, 0x00, 0xa8, 0x03, 0x30, 0xcb, 0x22, 0x4a, 0x25, 0x01, 0x2e, 0x7f, 0x00, 0x3c, 0x89, 0x35, 0x52, 0x05,
                                                                                                0x54, 0x98, 0x2e, 0xc4, 0xce, 0xc1, 0x6f, 0xf0, 0x6f, 0x98, 0x2e, 0x95, 0xcf, 0x04, 0x2d, 0x01, 0x30, 0xf0, 0x6f,
                                                                                                0x98, 0x2e, 0x95, 0xcf, 0xeb, 0x6f, 0xa0, 0x5f, 0xb8, 0x2e, 0x03, 0x2e, 0xb3, 0x00, 0x02, 0x32, 0xf0, 0x30, 0x03,
                                                                                                0x31, 0x30, 0x50, 0x8a, 0x08, 0x08, 0x08, 0xcb, 0x08, 0xe0, 0x7f, 0x80, 0xb2, 0xf3, 0x7f, 0xdb, 0x7f, 0x25, 0x2f,
                                                                                                0x03, 0x2e, 0xca, 0x00, 0x41, 0x90, 0x04, 0x2f, 0x01, 0x30, 0x23, 0x2e, 0xca, 0x00, 0x98, 0x2e, 0x3f, 0x03, 0xc0,
                                                                                                0xb2, 0x05, 0x2f, 0x03, 0x2e, 0xda, 0x00, 0x00, 0x30, 0x41, 0x04, 0x23, 0x2e, 0xda, 0x00, 0x98, 0x2e, 0x92, 0xb2,
                                                                                                0x10, 0x25, 0xf0, 0x6f, 0x00, 0xb2, 0x05, 0x2f, 0x01, 0x2e, 0xda, 0x00, 0x02, 0x30, 0x10, 0x04, 0x21, 0x2e, 0xda,
                                                                                                0x00, 0x40, 0xb2, 0x01, 0x2f, 0x23, 0x2e, 0xc8, 0x01, 0xdb, 0x6f, 0xe0, 0x6f, 0xd0, 0x5f, 0x80, 0x2e, 0x95, 0xcf,
                                                                                                0x01, 0x30, 0xe0, 0x6f, 0x98, 0x2e, 0x95, 0xcf, 0x11, 0x30, 0x23, 0x2e, 0xca, 0x00, 0xdb, 0x6f, 0xd0, 0x5f, 0xb8,
                                                                                                0x2e, 0xd0, 0x50, 0x0a, 0x25, 0x33, 0x84, 0x55, 0x50, 0xd2, 0x7f, 0xe2, 0x7f, 0x03, 0x8c, 0xc0, 0x7f, 0xbb, 0x7f,
                                                                                                0x00, 0x30, 0x05, 0x5a, 0x39, 0x54, 0x51, 0x41, 0xa5, 0x7f, 0x96, 0x7f, 0x80, 0x7f, 0x98, 0x2e, 0xd9, 0xc0, 0x05,
                                                                                                0x30, 0xf5, 0x7f, 0x20, 0x25, 0x91, 0x6f, 0x3b, 0x58, 0x3d, 0x5c, 0x3b, 0x56, 0x98, 0x2e, 0x67, 0xcc, 0xc1, 0x6f,
                                                                                                0xd5, 0x6f, 0x52, 0x40, 0x50, 0x43, 0xc1, 0x7f, 0xd5, 0x7f, 0x10, 0x25, 0x98, 0x2e, 0xfe, 0xc9, 0x10, 0x25, 0x98,
                                                                                                0x2e, 0x74, 0xc0, 0x86, 0x6f, 0x30, 0x28, 0x92, 0x6f, 0x82, 0x8c, 0xa5, 0x6f, 0x6f, 0x52, 0x69, 0x0e, 0x39, 0x54,
                                                                                                0xdb, 0x2f, 0x19, 0xa0, 0x15, 0x30, 0x03, 0x2f, 0x00, 0x30, 0x21, 0x2e, 0x81, 0x01, 0x0a, 0x2d, 0x01, 0x2e, 0x81,
                                                                                                0x01, 0x05, 0x28, 0x42, 0x36, 0x21, 0x2e, 0x81, 0x01, 0x02, 0x0e, 0x01, 0x2f, 0x98, 0x2e, 0xf3, 0x03, 0x57, 0x50,
                                                                                                0x12, 0x30, 0x01, 0x40, 0x98, 0x2e, 0xfe, 0xc9, 0x51, 0x6f, 0x0b, 0x5c, 0x8e, 0x0e, 0x3b, 0x6f, 0x57, 0x58, 0x02,
                                                                                                0x30, 0x21, 0x2e, 0x95, 0x01, 0x45, 0x6f, 0x2a, 0x8d, 0xd2, 0x7f, 0xcb, 0x7f, 0x13, 0x2f, 0x02, 0x30, 0x3f, 0x50,
                                                                                                0xd2, 0x7f, 0xa8, 0x0e, 0x0e, 0x2f, 0xc0, 0x6f, 0x53, 0x54, 0x02, 0x00, 0x51, 0x54, 0x42, 0x0e, 0x10, 0x30, 0x59,
                                                                                                0x52, 0x02, 0x30, 0x01, 0x2f, 0x00, 0x2e, 0x03, 0x2d, 0x50, 0x42, 0x42, 0x42, 0x12, 0x30, 0xd2, 0x7f, 0x80, 0xb2,
                                                                                                0x03, 0x2f, 0x00, 0x30, 0x21, 0x2e, 0x80, 0x01, 0x12, 0x2d, 0x01, 0x2e, 0xc9, 0x00, 0x02, 0x80, 0x05, 0x2e, 0x80,
                                                                                                0x01, 0x11, 0x30, 0x91, 0x28, 0x00, 0x40, 0x25, 0x2e, 0x80, 0x01, 0x10, 0x0e, 0x05, 0x2f, 0x01, 0x2e, 0x7f, 0x01,
                                                                                                0x01, 0x90, 0x01, 0x2f, 0x98, 0x2e, 0xf3, 0x03, 0x00, 0x2e, 0xa0, 0x41, 0x01, 0x90, 0xa6, 0x7f, 0x90, 0x2e, 0xe3,
                                                                                                0xb4, 0x01, 0x2e, 0x95, 0x01, 0x00, 0xa8, 0x90, 0x2e, 0xe3, 0xb4, 0x5b, 0x54, 0x95, 0x80, 0x82, 0x40, 0x80, 0xb2,
                                                                                                0x02, 0x40, 0x2d, 0x8c, 0x3f, 0x52, 0x96, 0x7f, 0x90, 0x2e, 0xc2, 0xb3, 0x29, 0x0e, 0x76, 0x2f, 0x01, 0x2e, 0xc9,
                                                                                                0x00, 0x00, 0x40, 0x81, 0x28, 0x45, 0x52, 0xb3, 0x30, 0x98, 0x2e, 0x0f, 0xca, 0x5d, 0x54, 0x80, 0x7f, 0x00, 0x2e,
                                                                                                0xa1, 0x40, 0x72, 0x7f, 0x82, 0x80, 0x82, 0x40, 0x60, 0x7f, 0x98, 0x2e, 0xfe, 0xc9, 0x10, 0x25, 0x98, 0x2e, 0x74,
                                                                                                0xc0, 0x62, 0x6f, 0x05, 0x30, 0x87, 0x40, 0xc0, 0x91, 0x04, 0x30, 0x05, 0x2f, 0x05, 0x2e, 0x83, 0x01, 0x80, 0xb2,
                                                                                                0x14, 0x30, 0x00, 0x2f, 0x04, 0x30, 0x05, 0x2e, 0xc9, 0x00, 0x73, 0x6f, 0x81, 0x40, 0xe2, 0x40, 0x69, 0x04, 0x11,
                                                                                                0x0f, 0xe1, 0x40, 0x16, 0x30, 0xfe, 0x29, 0xcb, 0x40, 0x02, 0x2f, 0x83, 0x6f, 0x83, 0x0f, 0x22, 0x2f, 0x47, 0x56,
                                                                                                0x13, 0x0f, 0x12, 0x30, 0x77, 0x2f, 0x49, 0x54, 0x42, 0x0e, 0x12, 0x30, 0x73, 0x2f, 0x00, 0x91, 0x0a, 0x2f, 0x01,
                                                                                                0x2e, 0x8b, 0x01, 0x19, 0xa8, 0x02, 0x30, 0x6c, 0x2f, 0x63, 0x50, 0x00, 0x2e, 0x17, 0x42, 0x05, 0x42, 0x68, 0x2c,
                                                                                                0x12, 0x30, 0x0b, 0x25, 0x08, 0x0f, 0x50, 0x30, 0x02, 0x2f, 0x21, 0x2e, 0x83, 0x01, 0x03, 0x2d, 0x40, 0x30, 0x21,
                                                                                                0x2e, 0x83, 0x01, 0x2b, 0x2e, 0x85, 0x01, 0x5a, 0x2c, 0x12, 0x30, 0x00, 0x91, 0x2b, 0x25, 0x04, 0x2f, 0x63, 0x50,
                                                                                                0x02, 0x30, 0x17, 0x42, 0x17, 0x2c, 0x02, 0x42, 0x98, 0x2e, 0xfe, 0xc9, 0x10, 0x25, 0x98, 0x2e, 0x74, 0xc0, 0x05,
                                                                                                0x2e, 0xc9, 0x00, 0x81, 0x84, 0x5b, 0x30, 0x82, 0x40, 0x37, 0x2e, 0x83, 0x01, 0x02, 0x0e, 0x07, 0x2f, 0x5f, 0x52,
                                                                                                0x40, 0x30, 0x62, 0x40, 0x41, 0x40, 0x91, 0x0e, 0x01, 0x2f, 0x21, 0x2e, 0x83, 0x01, 0x05, 0x30, 0x2b, 0x2e, 0x85,
                                                                                                0x01, 0x12, 0x30, 0x36, 0x2c, 0x16, 0x30, 0x15, 0x25, 0x81, 0x7f, 0x98, 0x2e, 0xfe, 0xc9, 0x10, 0x25, 0x98, 0x2e,
                                                                                                0x74, 0xc0, 0x19, 0xa2, 0x16, 0x30, 0x15, 0x2f, 0x05, 0x2e, 0x97, 0x01, 0x80, 0x6f, 0x82, 0x0e, 0x05, 0x2f, 0x01,
                                                                                                0x2e, 0x86, 0x01, 0x06, 0x28, 0x21, 0x2e, 0x86, 0x01, 0x0b, 0x2d, 0x03, 0x2e, 0x87, 0x01, 0x5f, 0x54, 0x4e, 0x28,
                                                                                                0x91, 0x42, 0x00, 0x2e, 0x82, 0x40, 0x90, 0x0e, 0x01, 0x2f, 0x21, 0x2e, 0x88, 0x01, 0x02, 0x30, 0x13, 0x2c, 0x05,
                                                                                                0x30, 0xc0, 0x6f, 0x08, 0x1c, 0xa8, 0x0f, 0x16, 0x30, 0x05, 0x30, 0x5b, 0x50, 0x09, 0x2f, 0x02, 0x80, 0x2d, 0x2e,
                                                                                                0x82, 0x01, 0x05, 0x42, 0x05, 0x80, 0x00, 0x2e, 0x02, 0x42, 0x3e, 0x80, 0x00, 0x2e, 0x06, 0x42, 0x02, 0x30, 0x90,
                                                                                                0x6f, 0x3e, 0x88, 0x01, 0x40, 0x04, 0x41, 0x4c, 0x28, 0x01, 0x42, 0x07, 0x80, 0x10, 0x25, 0x24, 0x40, 0x00, 0x40,
                                                                                                0x00, 0xa8, 0xf5, 0x22, 0x23, 0x29, 0x44, 0x42, 0x7a, 0x82, 0x7e, 0x88, 0x43, 0x40, 0x04, 0x41, 0x00, 0xab, 0xf5,
                                                                                                0x23, 0xdf, 0x28, 0x43, 0x42, 0xd9, 0xa0, 0x14, 0x2f, 0x00, 0x90, 0x02, 0x2f, 0xd2, 0x6f, 0x81, 0xb2, 0x05, 0x2f,
                                                                                                0x63, 0x54, 0x06, 0x28, 0x90, 0x42, 0x85, 0x42, 0x09, 0x2c, 0x02, 0x30, 0x5b, 0x50, 0x03, 0x80, 0x29, 0x2e, 0x7e,
                                                                                                0x01, 0x2b, 0x2e, 0x82, 0x01, 0x05, 0x42, 0x12, 0x30, 0x2b, 0x2e, 0x83, 0x01, 0x45, 0x82, 0x00, 0x2e, 0x40, 0x40,
                                                                                                0x7a, 0x82, 0x02, 0xa0, 0x08, 0x2f, 0x63, 0x50, 0x3b, 0x30, 0x15, 0x42, 0x05, 0x42, 0x37, 0x80, 0x37, 0x2e, 0x7e,
                                                                                                0x01, 0x05, 0x42, 0x12, 0x30, 0x01, 0x2e, 0xc9, 0x00, 0x02, 0x8c, 0x40, 0x40, 0x84, 0x41, 0x7a, 0x8c, 0x04, 0x0f,
                                                                                                0x03, 0x2f, 0x01, 0x2e, 0x8b, 0x01, 0x19, 0xa4, 0x04, 0x2f, 0x2b, 0x2e, 0x82, 0x01, 0x98, 0x2e, 0xf3, 0x03, 0x12,
                                                                                                0x30, 0x81, 0x90, 0x61, 0x52, 0x08, 0x2f, 0x65, 0x42, 0x65, 0x42, 0x43, 0x80, 0x39, 0x84, 0x82, 0x88, 0x05, 0x42,
                                                                                                0x45, 0x42, 0x85, 0x42, 0x05, 0x43, 0x00, 0x2e, 0x80, 0x41, 0x00, 0x90, 0x90, 0x2e, 0xe1, 0xb4, 0x65, 0x54, 0xc1,
                                                                                                0x6f, 0x80, 0x40, 0x00, 0xb2, 0x43, 0x58, 0x69, 0x50, 0x44, 0x2f, 0x55, 0x5c, 0xb7, 0x87, 0x8c, 0x0f, 0x0d, 0x2e,
                                                                                                0x96, 0x01, 0xc4, 0x40, 0x36, 0x2f, 0x41, 0x56, 0x8b, 0x0e, 0x2a, 0x2f, 0x0b, 0x52, 0xa1, 0x0e, 0x0a, 0x2f, 0x05,
                                                                                                0x2e, 0x8f, 0x01, 0x14, 0x25, 0x98, 0x2e, 0xfe, 0xc9, 0x4b, 0x54, 0x02, 0x0f, 0x69, 0x50, 0x05, 0x30, 0x65, 0x54,
                                                                                                0x15, 0x2f, 0x03, 0x2e, 0x8e, 0x01, 0x4d, 0x5c, 0x8e, 0x0f, 0x3a, 0x2f, 0x05, 0x2e, 0x8f, 0x01, 0x98, 0x2e, 0xfe,
                                                                                                0xc9, 0x4f, 0x54, 0x82, 0x0f, 0x05, 0x30, 0x69, 0x50, 0x65, 0x54, 0x30, 0x2f, 0x6d, 0x52, 0x15, 0x30, 0x42, 0x8c,
                                                                                                0x45, 0x42, 0x04, 0x30, 0x2b, 0x2c, 0x84, 0x43, 0x6b, 0x52, 0x42, 0x8c, 0x00, 0x2e, 0x85, 0x43, 0x15, 0x30, 0x24,
                                                                                                0x2c, 0x45, 0x42, 0x8e, 0x0f, 0x20, 0x2f, 0x0d, 0x2e, 0x8e, 0x01, 0xb1, 0x0e, 0x1c, 0x2f, 0x23, 0x2e, 0x8e, 0x01,
                                                                                                0x1a, 0x2d, 0x0e, 0x0e, 0x17, 0x2f, 0xa1, 0x0f, 0x15, 0x2f, 0x23, 0x2e, 0x8d, 0x01, 0x13, 0x2d, 0x98, 0x2e, 0x74,
                                                                                                0xc0, 0x43, 0x54, 0xc2, 0x0e, 0x0a, 0x2f, 0x65, 0x50, 0x04, 0x80, 0x0b, 0x30, 0x06, 0x82, 0x0b, 0x42, 0x79, 0x80,
                                                                                                0x41, 0x40, 0x12, 0x30, 0x25, 0x2e, 0x8c, 0x01, 0x01, 0x42, 0x05, 0x30, 0x69, 0x50, 0x65, 0x54, 0x84, 0x82, 0x43,
                                                                                                0x84, 0xbe, 0x8c, 0x84, 0x40, 0x86, 0x41, 0x26, 0x29, 0x94, 0x42, 0xbe, 0x8e, 0xd5, 0x7f, 0x19, 0xa1, 0x43, 0x40,
                                                                                                0x0b, 0x2e, 0x8c, 0x01, 0x84, 0x40, 0xc7, 0x41, 0x5d, 0x29, 0x27, 0x29, 0x45, 0x42, 0x84, 0x42, 0xc2, 0x7f, 0x01,
                                                                                                0x2f, 0xc0, 0xb3, 0x1d, 0x2f, 0x05, 0x2e, 0x94, 0x01, 0x99, 0xa0, 0x01, 0x2f, 0x80, 0xb3, 0x13, 0x2f, 0x80, 0xb3,
                                                                                                0x18, 0x2f, 0xc0, 0xb3, 0x16, 0x2f, 0x12, 0x40, 0x01, 0x40, 0x92, 0x7f, 0x98, 0x2e, 0x74, 0xc0, 0x92, 0x6f, 0x10,
                                                                                                0x0f, 0x20, 0x30, 0x03, 0x2f, 0x10, 0x30, 0x21, 0x2e, 0x7e, 0x01, 0x0a, 0x2d, 0x21, 0x2e, 0x7e, 0x01, 0x07, 0x2d,
                                                                                                0x20, 0x30, 0x21, 0x2e, 0x7e, 0x01, 0x03, 0x2d, 0x10, 0x30, 0x21, 0x2e, 0x7e, 0x01, 0xc2, 0x6f, 0x01, 0x2e, 0xc9,
                                                                                                0x00, 0xbc, 0x84, 0x02, 0x80, 0x82, 0x40, 0x00, 0x40, 0x90, 0x0e, 0xd5, 0x6f, 0x02, 0x2f, 0x15, 0x30, 0x98, 0x2e,
                                                                                                0xf3, 0x03, 0x41, 0x91, 0x05, 0x30, 0x07, 0x2f, 0x67, 0x50, 0x3d, 0x80, 0x2b, 0x2e, 0x8f, 0x01, 0x05, 0x42, 0x04,
                                                                                                0x80, 0x00, 0x2e, 0x05, 0x42, 0x02, 0x2c, 0x00, 0x30, 0x00, 0x30, 0xa2, 0x6f, 0x98, 0x8a, 0x86, 0x40, 0x80, 0xa7,
                                                                                                0x05, 0x2f, 0x98, 0x2e, 0xf3, 0x03, 0xc0, 0x30, 0x21, 0x2e, 0x95, 0x01, 0x06, 0x25, 0x1a, 0x25, 0xe2, 0x6f, 0x76,
                                                                                                0x82, 0x96, 0x40, 0x56, 0x43, 0x51, 0x0e, 0xfb, 0x2f, 0xbb, 0x6f, 0x30, 0x5f, 0xb8, 0x2e, 0x01, 0x2e, 0xb8, 0x00,
                                                                                                0x01, 0x31, 0x41, 0x08, 0x40, 0xb2, 0x20, 0x50, 0xf2, 0x30, 0x02, 0x08, 0xfb, 0x7f, 0x01, 0x30, 0x10, 0x2f, 0x05,
                                                                                                0x2e, 0xcc, 0x00, 0x81, 0x90, 0xe0, 0x7f, 0x03, 0x2f, 0x23, 0x2e, 0xcc, 0x00, 0x98, 0x2e, 0x55, 0xb6, 0x98, 0x2e,
                                                                                                0x1d, 0xb5, 0x10, 0x25, 0xfb, 0x6f, 0xe0, 0x6f, 0xe0, 0x5f, 0x80, 0x2e, 0x95, 0xcf, 0x98, 0x2e, 0x95, 0xcf, 0x10,
                                                                                                0x30, 0x21, 0x2e, 0xcc, 0x00, 0xfb, 0x6f, 0xe0, 0x5f, 0xb8, 0x2e, 0x00, 0x51, 0x05, 0x58, 0xeb, 0x7f, 0x2a, 0x25,
                                                                                                0x89, 0x52, 0x6f, 0x5a, 0x89, 0x50, 0x13, 0x41, 0x06, 0x40, 0xb3, 0x01, 0x16, 0x42, 0xcb, 0x16, 0x06, 0x40, 0xf3,
                                                                                                0x02, 0x13, 0x42, 0x65, 0x0e, 0xf5, 0x2f, 0x05, 0x40, 0x14, 0x30, 0x2c, 0x29, 0x04, 0x42, 0x08, 0xa1, 0x00, 0x30,
                                                                                                0x90, 0x2e, 0x52, 0xb6, 0xb3, 0x88, 0xb0, 0x8a, 0xb6, 0x84, 0xa4, 0x7f, 0xc4, 0x7f, 0xb5, 0x7f, 0xd5, 0x7f, 0x92,
                                                                                                0x7f, 0x73, 0x30, 0x04, 0x30, 0x55, 0x40, 0x42, 0x40, 0x8a, 0x17, 0xf3, 0x08, 0x6b, 0x01, 0x90, 0x02, 0x53, 0xb8,
                                                                                                0x4b, 0x82, 0xad, 0xbe, 0x71, 0x7f, 0x45, 0x0a, 0x09, 0x54, 0x84, 0x7f, 0x98, 0x2e, 0xd9, 0xc0, 0xa3, 0x6f, 0x7b,
                                                                                                0x54, 0xd0, 0x42, 0xa3, 0x7f, 0xf2, 0x7f, 0x60, 0x7f, 0x20, 0x25, 0x71, 0x6f, 0x75, 0x5a, 0x77, 0x58, 0x79, 0x5c,
                                                                                                0x75, 0x56, 0x98, 0x2e, 0x67, 0xcc, 0xb1, 0x6f, 0x62, 0x6f, 0x50, 0x42, 0xb1, 0x7f, 0xb3, 0x30, 0x10, 0x25, 0x98,
                                                                                                0x2e, 0x0f, 0xca, 0x84, 0x6f, 0x20, 0x29, 0x71, 0x6f, 0x92, 0x6f, 0xa5, 0x6f, 0x76, 0x82, 0x6a, 0x0e, 0x73, 0x30,
                                                                                                0x00, 0x30, 0xd0, 0x2f, 0xd2, 0x6f, 0xd1, 0x7f, 0xb4, 0x7f, 0x98, 0x2e, 0x2b, 0xb7, 0x15, 0xbd, 0x0b, 0xb8, 0x02,
                                                                                                0x0a, 0xc2, 0x6f, 0xc0, 0x7f, 0x98, 0x2e, 0x2b, 0xb7, 0x15, 0xbd, 0x0b, 0xb8, 0x42, 0x0a, 0xc0, 0x6f, 0x08, 0x17,
                                                                                                0x41, 0x18, 0x89, 0x16, 0xe1, 0x18, 0xd0, 0x18, 0xa1, 0x7f, 0x27, 0x25, 0x16, 0x25, 0x98, 0x2e, 0x79, 0xc0, 0x8b,
                                                                                                0x54, 0x90, 0x7f, 0xb3, 0x30, 0x82, 0x40, 0x80, 0x90, 0x0d, 0x2f, 0x7d, 0x52, 0x92, 0x6f, 0x98, 0x2e, 0x0f, 0xca,
                                                                                                0xb2, 0x6f, 0x90, 0x0e, 0x06, 0x2f, 0x8b, 0x50, 0x14, 0x30, 0x42, 0x6f, 0x51, 0x6f, 0x14, 0x42, 0x12, 0x42, 0x01,
                                                                                                0x42, 0x00, 0x2e, 0x31, 0x6f, 0x98, 0x2e, 0x74, 0xc0, 0x41, 0x6f, 0x80, 0x7f, 0x98, 0x2e, 0x74, 0xc0, 0x82, 0x6f,
                                                                                                0x10, 0x04, 0x43, 0x52, 0x01, 0x0f, 0x05, 0x2e, 0xcb, 0x00, 0x00, 0x30, 0x04, 0x30, 0x21, 0x2f, 0x51, 0x6f, 0x43,
                                                                                                0x58, 0x8c, 0x0e, 0x04, 0x30, 0x1c, 0x2f, 0x85, 0x88, 0x41, 0x6f, 0x04, 0x41, 0x8c, 0x0f, 0x04, 0x30, 0x16, 0x2f,
                                                                                                0x84, 0x88, 0x00, 0x2e, 0x04, 0x41, 0x04, 0x05, 0x8c, 0x0e, 0x04, 0x30, 0x0f, 0x2f, 0x82, 0x88, 0x31, 0x6f, 0x04,
                                                                                                0x41, 0x04, 0x05, 0x8c, 0x0e, 0x04, 0x30, 0x08, 0x2f, 0x83, 0x88, 0x00, 0x2e, 0x04, 0x41, 0x8c, 0x0f, 0x04, 0x30,
                                                                                                0x02, 0x2f, 0x21, 0x2e, 0xad, 0x01, 0x14, 0x30, 0x00, 0x91, 0x14, 0x2f, 0x03, 0x2e, 0xa1, 0x01, 0x41, 0x90, 0x0e,
                                                                                                0x2f, 0x03, 0x2e, 0xad, 0x01, 0x14, 0x30, 0x4c, 0x28, 0x23, 0x2e, 0xad, 0x01, 0x46, 0xa0, 0x06, 0x2f, 0x81, 0x84,
                                                                                                0x8d, 0x52, 0x48, 0x82, 0x82, 0x40, 0x21, 0x2e, 0xa1, 0x01, 0x42, 0x42, 0x5c, 0x2c, 0x02, 0x30, 0x05, 0x2e, 0xaa,
                                                                                                0x01, 0x80, 0xb2, 0x02, 0x30, 0x55, 0x2f, 0x03, 0x2e, 0xa9, 0x01, 0x92, 0x6f, 0xb3, 0x30, 0x98, 0x2e, 0x0f, 0xca,
                                                                                                0xb2, 0x6f, 0x90, 0x0f, 0x00, 0x30, 0x02, 0x30, 0x4a, 0x2f, 0xa2, 0x6f, 0x87, 0x52, 0x91, 0x00, 0x85, 0x52, 0x51,
                                                                                                0x0e, 0x02, 0x2f, 0x00, 0x2e, 0x43, 0x2c, 0x02, 0x30, 0xc2, 0x6f, 0x7f, 0x52, 0x91, 0x0e, 0x02, 0x30, 0x3c, 0x2f,
                                                                                                0x51, 0x6f, 0x81, 0x54, 0x98, 0x2e, 0xfe, 0xc9, 0x10, 0x25, 0xb3, 0x30, 0x21, 0x25, 0x98, 0x2e, 0x0f, 0xca, 0x32,
                                                                                                0x6f, 0xc0, 0x7f, 0xb3, 0x30, 0x12, 0x25, 0x98, 0x2e, 0x0f, 0xca, 0x42, 0x6f, 0xb0, 0x7f, 0xb3, 0x30, 0x12, 0x25,
                                                                                                0x98, 0x2e, 0x0f, 0xca, 0xb2, 0x6f, 0x90, 0x28, 0x83, 0x52, 0x98, 0x2e, 0xfe, 0xc9, 0xc2, 0x6f, 0x90, 0x0f, 0x00,
                                                                                                0x30, 0x02, 0x30, 0x1d, 0x2f, 0x05, 0x2e, 0xa1, 0x01, 0x80, 0xb2, 0x12, 0x30, 0x0f, 0x2f, 0x42, 0x6f, 0x03, 0x2e,
                                                                                                0xab, 0x01, 0x91, 0x0e, 0x02, 0x30, 0x12, 0x2f, 0x52, 0x6f, 0x03, 0x2e, 0xac, 0x01, 0x91, 0x0f, 0x02, 0x30, 0x0c,
                                                                                                0x2f, 0x21, 0x2e, 0xaa, 0x01, 0x0a, 0x2c, 0x12, 0x30, 0x03, 0x2e, 0xcb, 0x00, 0x8d, 0x58, 0x08, 0x89, 0x41, 0x40,
                                                                                                0x11, 0x43, 0x00, 0x43, 0x25, 0x2e, 0xa1, 0x01, 0xd4, 0x6f, 0x8f, 0x52, 0x00, 0x43, 0x3a, 0x89, 0x00, 0x2e, 0x10,
                                                                                                0x43, 0x10, 0x43, 0x61, 0x0e, 0xfb, 0x2f, 0x03, 0x2e, 0xa0, 0x01, 0x11, 0x1a, 0x02, 0x2f, 0x02, 0x25, 0x21, 0x2e,
                                                                                                0xa0, 0x01, 0xeb, 0x6f, 0x00, 0x5f, 0xb8, 0x2e, 0x91, 0x52, 0x10, 0x30, 0x02, 0x30, 0x95, 0x56, 0x52, 0x42, 0x4b,
                                                                                                0x0e, 0xfc, 0x2f, 0x8d, 0x54, 0x88, 0x82, 0x93, 0x56, 0x80, 0x42, 0x53, 0x42, 0x40, 0x42, 0x42, 0x86, 0x83, 0x54,
                                                                                                0xc0, 0x2e, 0xc2, 0x42, 0x00, 0x2e, 0xa3, 0x52, 0x00, 0x51, 0x52, 0x40, 0x47, 0x40, 0x1a, 0x25, 0x01, 0x2e, 0x97,
                                                                                                0x00, 0x8f, 0xbe, 0x72, 0x86, 0xfb, 0x7f, 0x0b, 0x30, 0x7c, 0xbf, 0xa5, 0x50, 0x10, 0x08, 0xdf, 0xba, 0x70, 0x88,
                                                                                                0xf8, 0xbf, 0xcb, 0x42, 0xd3, 0x7f, 0x6c, 0xbb, 0xfc, 0xbb, 0xc5, 0x0a, 0x90, 0x7f, 0x1b, 0x7f, 0x0b, 0x43, 0xc0,
                                                                                                0xb2, 0xe5, 0x7f, 0xb7, 0x7f, 0xa6, 0x7f, 0xc4, 0x7f, 0x90, 0x2e, 0x1c, 0xb7, 0x07, 0x2e, 0xd2, 0x00, 0xc0, 0xb2,
                                                                                                0x0b, 0x2f, 0x97, 0x52, 0x01, 0x2e, 0xcd, 0x00, 0x82, 0x7f, 0x98, 0x2e, 0xbb, 0xcc, 0x0b, 0x30, 0x37, 0x2e, 0xd2,
                                                                                                0x00, 0x82, 0x6f, 0x90, 0x6f, 0x1a, 0x25, 0x00, 0xb2, 0x8b, 0x7f, 0x14, 0x2f, 0xa6, 0xbd, 0x25, 0xbd, 0xb6, 0xb9,
                                                                                                0x2f, 0xb9, 0x80, 0xb2, 0xd4, 0xb0, 0x0c, 0x2f, 0x99, 0x54, 0x9b, 0x56, 0x0b, 0x30, 0x0b, 0x2e, 0xb1, 0x00, 0xa1,
                                                                                                0x58, 0x9b, 0x42, 0xdb, 0x42, 0x6c, 0x09, 0x2b, 0x2e, 0xb1, 0x00, 0x8b, 0x42, 0xcb, 0x42, 0x86, 0x7f, 0x73, 0x84,
                                                                                                0xa7, 0x56, 0xc3, 0x08, 0x39, 0x52, 0x05, 0x50, 0x72, 0x7f, 0x63, 0x7f, 0x98, 0x2e, 0xc2, 0xc0, 0xe1, 0x6f, 0x62,
                                                                                                0x6f, 0xd1, 0x0a, 0x01, 0x2e, 0xcd, 0x00, 0xd5, 0x6f, 0xc4, 0x6f, 0x72, 0x6f, 0x97, 0x52, 0x9d, 0x5c, 0x98, 0x2e,
                                                                                                0x06, 0xcd, 0x23, 0x6f, 0x90, 0x6f, 0x99, 0x52, 0xc0, 0xb2, 0x04, 0xbd, 0x54, 0x40, 0xaf, 0xb9, 0x45, 0x40, 0xe1,
                                                                                                0x7f, 0x02, 0x30, 0x06, 0x2f, 0xc0, 0xb2, 0x02, 0x30, 0x03, 0x2f, 0x9b, 0x5c, 0x12, 0x30, 0x94, 0x43, 0x85, 0x43,
                                                                                                0x03, 0xbf, 0x6f, 0xbb, 0x80, 0xb3, 0x20, 0x2f, 0x06, 0x6f, 0x26, 0x01, 0x16, 0x6f, 0x6e, 0x03, 0x45, 0x42, 0xc0,
                                                                                                0x90, 0x29, 0x2e, 0xce, 0x00, 0x9b, 0x52, 0x14, 0x2f, 0x9b, 0x5c, 0x00, 0x2e, 0x93, 0x41, 0x86, 0x41, 0xe3, 0x04,
                                                                                                0xae, 0x07, 0x80, 0xab, 0x04, 0x2f, 0x80, 0x91, 0x0a, 0x2f, 0x86, 0x6f, 0x73, 0x0f, 0x07, 0x2f, 0x83, 0x6f, 0xc0,
                                                                                                0xb2, 0x04, 0x2f, 0x54, 0x42, 0x45, 0x42, 0x12, 0x30, 0x04, 0x2c, 0x11, 0x30, 0x02, 0x2c, 0x11, 0x30, 0x11, 0x30,
                                                                                                0x02, 0xbc, 0x0f, 0xb8, 0xd2, 0x7f, 0x00, 0xb2, 0x0a, 0x2f, 0x01, 0x2e, 0xfc, 0x00, 0x05, 0x2e, 0xc7, 0x01, 0x10,
                                                                                                0x1a, 0x02, 0x2f, 0x21, 0x2e, 0xc7, 0x01, 0x03, 0x2d, 0x02, 0x2c, 0x01, 0x30, 0x01, 0x30, 0xb0, 0x6f, 0x98, 0x2e,
                                                                                                0x95, 0xcf, 0xd1, 0x6f, 0xa0, 0x6f, 0x98, 0x2e, 0x95, 0xcf, 0xe2, 0x6f, 0x9f, 0x52, 0x01, 0x2e, 0xce, 0x00, 0x82,
                                                                                                0x40, 0x50, 0x42, 0x0c, 0x2c, 0x42, 0x42, 0x11, 0x30, 0x23, 0x2e, 0xd2, 0x00, 0x01, 0x30, 0xb0, 0x6f, 0x98, 0x2e,
                                                                                                0x95, 0xcf, 0xa0, 0x6f, 0x01, 0x30, 0x98, 0x2e, 0x95, 0xcf, 0x00, 0x2e, 0xfb, 0x6f, 0x00, 0x5f, 0xb8, 0x2e, 0x83,
                                                                                                0x86, 0x01, 0x30, 0x00, 0x30, 0x94, 0x40, 0x24, 0x18, 0x06, 0x00, 0x53, 0x0e, 0x4f, 0x02, 0xf9, 0x2f, 0xb8, 0x2e,
                                                                                                0xa9, 0x52, 0x00, 0x2e, 0x60, 0x40, 0x41, 0x40, 0x0d, 0xbc, 0x98, 0xbc, 0xc0, 0x2e, 0x01, 0x0a, 0x0f, 0xb8, 0xab,
                                                                                                0x52, 0x53, 0x3c, 0x52, 0x40, 0x40, 0x40, 0x4b, 0x00, 0x82, 0x16, 0x26, 0xb9, 0x01, 0xb8, 0x41, 0x40, 0x10, 0x08,
                                                                                                0x97, 0xb8, 0x01, 0x08, 0xc0, 0x2e, 0x11, 0x30, 0x01, 0x08, 0x43, 0x86, 0x25, 0x40, 0x04, 0x40, 0xd8, 0xbe, 0x2c,
                                                                                                0x0b, 0x22, 0x11, 0x54, 0x42, 0x03, 0x80, 0x4b, 0x0e, 0xf6, 0x2f, 0xb8, 0x2e, 0x9f, 0x50, 0x10, 0x50, 0xad, 0x52,
                                                                                                0x05, 0x2e, 0xd3, 0x00, 0xfb, 0x7f, 0x00, 0x2e, 0x13, 0x40, 0x93, 0x42, 0x41, 0x0e, 0xfb, 0x2f, 0x98, 0x2e, 0xa5,
                                                                                                0xb7, 0x98, 0x2e, 0x87, 0xcf, 0x01, 0x2e, 0xd9, 0x00, 0x00, 0xb2, 0xfb, 0x6f, 0x0b, 0x2f, 0x01, 0x2e, 0x69, 0xf7,
                                                                                                0xb1, 0x3f, 0x01, 0x08, 0x01, 0x30, 0xf0, 0x5f, 0x23, 0x2e, 0xd9, 0x00, 0x21, 0x2e, 0x69, 0xf7, 0x80, 0x2e, 0x7a,
                                                                                                0xb7, 0xf0, 0x5f, 0xb8, 0x2e, 0x01, 0x2e, 0xc0, 0xf8, 0x03, 0x2e, 0xfc, 0xf5, 0x15, 0x54, 0xaf, 0x56, 0x82, 0x08,
                                                                                                0x0b, 0x2e, 0x69, 0xf7, 0xcb, 0x0a, 0xb1, 0x58, 0x80, 0x90, 0xdd, 0xbe, 0x4c, 0x08, 0x5f, 0xb9, 0x59, 0x22, 0x80,
                                                                                                0x90, 0x07, 0x2f, 0x03, 0x34, 0xc3, 0x08, 0xf2, 0x3a, 0x0a, 0x08, 0x02, 0x35, 0xc0, 0x90, 0x4a, 0x0a, 0x48, 0x22,
                                                                                                0xc0, 0x2e, 0x23, 0x2e, 0xfc, 0xf5, 0x10, 0x50, 0xfb, 0x7f, 0x98, 0x2e, 0x56, 0xc7, 0x98, 0x2e, 0x49, 0xc3, 0x10,
                                                                                                0x30, 0xfb, 0x6f, 0xf0, 0x5f, 0x21, 0x2e, 0xcc, 0x00, 0x21, 0x2e, 0xca, 0x00, 0xb8, 0x2e, 0x03, 0x2e, 0xd3, 0x00,
                                                                                                0x16, 0xb8, 0x02, 0x34, 0x4a, 0x0c, 0x21, 0x2e, 0x2d, 0xf5, 0xc0, 0x2e, 0x23, 0x2e, 0xd3, 0x00, 0x03, 0xbc, 0x21,
                                                                                                0x2e, 0xd5, 0x00, 0x03, 0x2e, 0xd5, 0x00, 0x40, 0xb2, 0x10, 0x30, 0x21, 0x2e, 0x77, 0x00, 0x01, 0x30, 0x05, 0x2f,
                                                                                                0x05, 0x2e, 0xd8, 0x00, 0x80, 0x90, 0x01, 0x2f, 0x23, 0x2e, 0x6f, 0xf5, 0xc0, 0x2e, 0x21, 0x2e, 0xd9, 0x00, 0x11,
                                                                                                0x30, 0x81, 0x08, 0x01, 0x2e, 0x6a, 0xf7, 0x71, 0x3f, 0x23, 0xbd, 0x01, 0x08, 0x02, 0x0a, 0xc0, 0x2e, 0x21, 0x2e,
                                                                                                0x6a, 0xf7, 0x30, 0x25, 0x00, 0x30, 0x21, 0x2e, 0x5a, 0xf5, 0x10, 0x50, 0x21, 0x2e, 0x7b, 0x00, 0x21, 0x2e, 0x7c,
                                                                                                0x00, 0xfb, 0x7f, 0x98, 0x2e, 0xc3, 0xb7, 0x40, 0x30, 0x21, 0x2e, 0xd4, 0x00, 0xfb, 0x6f, 0xf0, 0x5f, 0x03, 0x25,
                                                                                                0x80, 0x2e, 0xaf, 0xb7, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00,
                                                                                                0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e,
                                                                                                0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80,
                                                                                                0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x01, 0x2e, 0x5d, 0xf7, 0x08, 0xbc, 0x80, 0xac, 0x0e, 0xbb, 0x02, 0x2f,
                                                                                                0x00, 0x30, 0x41, 0x04, 0x82, 0x06, 0xc0, 0xa4, 0x00, 0x30, 0x11, 0x2f, 0x40, 0xa9, 0x03, 0x2f, 0x40, 0x91, 0x0d,
                                                                                                0x2f, 0x00, 0xa7, 0x0b, 0x2f, 0x80, 0xb3, 0xb3, 0x58, 0x02, 0x2f, 0x90, 0xa1, 0x26, 0x13, 0x20, 0x23, 0x80, 0x90,
                                                                                                0x10, 0x30, 0x01, 0x2f, 0xcc, 0x0e, 0x00, 0x2f, 0x00, 0x30, 0xb8, 0x2e, 0xb5, 0x50, 0x18, 0x08, 0x08, 0xbc, 0x88,
                                                                                                0xb6, 0x0d, 0x17, 0xc6, 0xbd, 0x56, 0xbc, 0xb7, 0x58, 0xda, 0xba, 0x04, 0x01, 0x1d, 0x0a, 0x10, 0x50, 0x05, 0x30,
                                                                                                0x32, 0x25, 0x45, 0x03, 0xfb, 0x7f, 0xf6, 0x30, 0x21, 0x25, 0x98, 0x2e, 0x37, 0xca, 0x16, 0xb5, 0x9a, 0xbc, 0x06,
                                                                                                0xb8, 0x80, 0xa8, 0x41, 0x0a, 0x0e, 0x2f, 0x80, 0x90, 0x02, 0x2f, 0x2d, 0x50, 0x48, 0x0f, 0x09, 0x2f, 0xbf, 0xa0,
                                                                                                0x04, 0x2f, 0xbf, 0x90, 0x06, 0x2f, 0xb7, 0x54, 0xca, 0x0f, 0x03, 0x2f, 0x00, 0x2e, 0x02, 0x2c, 0xb7, 0x52, 0x2d,
                                                                                                0x52, 0xf2, 0x33, 0x98, 0x2e, 0xd9, 0xc0, 0xfb, 0x6f, 0xf1, 0x37, 0xc0, 0x2e, 0x01, 0x08, 0xf0, 0x5f, 0xbf, 0x56,
                                                                                                0xb9, 0x54, 0xd0, 0x40, 0xc4, 0x40, 0x0b, 0x2e, 0xfd, 0xf3, 0xbf, 0x52, 0x90, 0x42, 0x94, 0x42, 0x95, 0x42, 0x05,
                                                                                                0x30, 0xc1, 0x50, 0x0f, 0x88, 0x06, 0x40, 0x04, 0x41, 0x96, 0x42, 0xc5, 0x42, 0x48, 0xbe, 0x73, 0x30, 0x0d, 0x2e,
                                                                                                0xd8, 0x00, 0x4f, 0xba, 0x84, 0x42, 0x03, 0x42, 0x81, 0xb3, 0x02, 0x2f, 0x2b, 0x2e, 0x6f, 0xf5, 0x06, 0x2d, 0x05,
                                                                                                0x2e, 0x77, 0xf7, 0xbd, 0x56, 0x93, 0x08, 0x25, 0x2e, 0x77, 0xf7, 0xbb, 0x54, 0x25, 0x2e, 0xc2, 0xf5, 0x07, 0x2e,
                                                                                                0xfd, 0xf3, 0x42, 0x30, 0xb4, 0x33, 0xda, 0x0a, 0x4c, 0x00, 0x27, 0x2e, 0xfd, 0xf3, 0x43, 0x40, 0xd4, 0x3f, 0xdc,
                                                                                                0x08, 0x43, 0x42, 0x00, 0x2e, 0x00, 0x2e, 0x43, 0x40, 0x24, 0x30, 0xdc, 0x0a, 0x43, 0x42, 0x04, 0x80, 0x03, 0x2e,
                                                                                                0xfd, 0xf3, 0x4a, 0x0a, 0x23, 0x2e, 0xfd, 0xf3, 0x61, 0x34, 0xc0, 0x2e, 0x01, 0x42, 0x00, 0x2e, 0x60, 0x50, 0x1a,
                                                                                                0x25, 0x7a, 0x86, 0xe0, 0x7f, 0xf3, 0x7f, 0x03, 0x25, 0xc3, 0x52, 0x41, 0x84, 0xdb, 0x7f, 0x33, 0x30, 0x98, 0x2e,
                                                                                                0x16, 0xc2, 0x1a, 0x25, 0x7d, 0x82, 0xf0, 0x6f, 0xe2, 0x6f, 0x32, 0x25, 0x16, 0x40, 0x94, 0x40, 0x26, 0x01, 0x85,
                                                                                                0x40, 0x8e, 0x17, 0xc4, 0x42, 0x6e, 0x03, 0x95, 0x42, 0x41, 0x0e, 0xf4, 0x2f, 0xdb, 0x6f, 0xa0, 0x5f, 0xb8, 0x2e,
                                                                                                0xb0, 0x51, 0xfb, 0x7f, 0x98, 0x2e, 0xe8, 0x0d, 0x5a, 0x25, 0x98, 0x2e, 0x0f, 0x0e, 0xcb, 0x58, 0x32, 0x87, 0xc4,
                                                                                                0x7f, 0x65, 0x89, 0x6b, 0x8d, 0xc5, 0x5a, 0x65, 0x7f, 0xe1, 0x7f, 0x83, 0x7f, 0xa6, 0x7f, 0x74, 0x7f, 0xd0, 0x7f,
                                                                                                0xb6, 0x7f, 0x94, 0x7f, 0x17, 0x30, 0xc7, 0x52, 0xc9, 0x54, 0x51, 0x7f, 0x00, 0x2e, 0x85, 0x6f, 0x42, 0x7f, 0x00,
                                                                                                0x2e, 0x51, 0x41, 0x45, 0x81, 0x42, 0x41, 0x13, 0x40, 0x3b, 0x8a, 0x00, 0x40, 0x4b, 0x04, 0xd0, 0x06, 0xc0, 0xac,
                                                                                                0x85, 0x7f, 0x02, 0x2f, 0x02, 0x30, 0x51, 0x04, 0xd3, 0x06, 0x41, 0x84, 0x05, 0x30, 0x5d, 0x02, 0xc9, 0x16, 0xdf,
                                                                                                0x08, 0xd3, 0x00, 0x8d, 0x02, 0xaf, 0xbc, 0xb1, 0xb9, 0x59, 0x0a, 0x65, 0x6f, 0x11, 0x43, 0xa1, 0xb4, 0x52, 0x41,
                                                                                                0x53, 0x41, 0x01, 0x43, 0x34, 0x7f, 0x65, 0x7f, 0x26, 0x31, 0xe5, 0x6f, 0xd4, 0x6f, 0x98, 0x2e, 0x37, 0xca, 0x32,
                                                                                                0x6f, 0x75, 0x6f, 0x83, 0x40, 0x42, 0x41, 0x23, 0x7f, 0x12, 0x7f, 0xf6, 0x30, 0x40, 0x25, 0x51, 0x25, 0x98, 0x2e,
                                                                                                0x37, 0xca, 0x14, 0x6f, 0x20, 0x05, 0x70, 0x6f, 0x25, 0x6f, 0x69, 0x07, 0xa2, 0x6f, 0x31, 0x6f, 0x0b, 0x30, 0x04,
                                                                                                0x42, 0x9b, 0x42, 0x8b, 0x42, 0x55, 0x42, 0x32, 0x7f, 0x40, 0xa9, 0xc3, 0x6f, 0x71, 0x7f, 0x02, 0x30, 0xd0, 0x40,
                                                                                                0xc3, 0x7f, 0x03, 0x2f, 0x40, 0x91, 0x15, 0x2f, 0x00, 0xa7, 0x13, 0x2f, 0x00, 0xa4, 0x11, 0x2f, 0x84, 0xbd, 0x98,
                                                                                                0x2e, 0x79, 0xca, 0x55, 0x6f, 0xb7, 0x54, 0x54, 0x41, 0x82, 0x00, 0xf3, 0x3f, 0x45, 0x41, 0xcb, 0x02, 0xf6, 0x30,
                                                                                                0x98, 0x2e, 0x37, 0xca, 0x35, 0x6f, 0xa4, 0x6f, 0x41, 0x43, 0x03, 0x2c, 0x00, 0x43, 0xa4, 0x6f, 0x35, 0x6f, 0x17,
                                                                                                0x30, 0x42, 0x6f, 0x51, 0x6f, 0x93, 0x40, 0x42, 0x82, 0x00, 0x41, 0xc3, 0x00, 0x03, 0x43, 0x51, 0x7f, 0x00, 0x2e,
                                                                                                0x94, 0x40, 0x41, 0x41, 0x4c, 0x02, 0xc4, 0x6f, 0xd1, 0x56, 0x63, 0x0e, 0x74, 0x6f, 0x51, 0x43, 0xa5, 0x7f, 0x8a,
                                                                                                0x2f, 0x09, 0x2e, 0xd8, 0x00, 0x01, 0xb3, 0x21, 0x2f, 0xcb, 0x58, 0x90, 0x6f, 0x13, 0x41, 0xb6, 0x6f, 0xe4, 0x7f,
                                                                                                0x00, 0x2e, 0x91, 0x41, 0x14, 0x40, 0x92, 0x41, 0x15, 0x40, 0x17, 0x2e, 0x6f, 0xf5, 0xb6, 0x7f, 0xd0, 0x7f, 0xcb,
                                                                                                0x7f, 0x98, 0x2e, 0x00, 0x0c, 0x07, 0x15, 0xc2, 0x6f, 0x14, 0x0b, 0x29, 0x2e, 0x6f, 0xf5, 0xc3, 0xa3, 0xc1, 0x8f,
                                                                                                0xe4, 0x6f, 0xd0, 0x6f, 0xe6, 0x2f, 0x14, 0x30, 0x05, 0x2e, 0x6f, 0xf5, 0x14, 0x0b, 0x29, 0x2e, 0x6f, 0xf5, 0x18,
                                                                                                0x2d, 0xcd, 0x56, 0x04, 0x32, 0xb5, 0x6f, 0x1c, 0x01, 0x51, 0x41, 0x52, 0x41, 0xc3, 0x40, 0xb5, 0x7f, 0xe4, 0x7f,
                                                                                                0x98, 0x2e, 0x1f, 0x0c, 0xe4, 0x6f, 0x21, 0x87, 0x00, 0x43, 0x04, 0x32, 0xcf, 0x54, 0x5a, 0x0e, 0xef, 0x2f, 0x15,
                                                                                                0x54, 0x09, 0x2e, 0x77, 0xf7, 0x22, 0x0b, 0x29, 0x2e, 0x77, 0xf7, 0xfb, 0x6f, 0x50, 0x5e, 0xb8, 0x2e, 0x10, 0x50,
                                                                                                0x01, 0x2e, 0xd4, 0x00, 0x00, 0xb2, 0xfb, 0x7f, 0x51, 0x2f, 0x01, 0xb2, 0x48, 0x2f, 0x02, 0xb2, 0x42, 0x2f, 0x03,
                                                                                                0x90, 0x56, 0x2f, 0xd7, 0x52, 0x79, 0x80, 0x42, 0x40, 0x81, 0x84, 0x00, 0x40, 0x42, 0x42, 0x98, 0x2e, 0x93, 0x0c,
                                                                                                0xd9, 0x54, 0xd7, 0x50, 0xa1, 0x40, 0x98, 0xbd, 0x82, 0x40, 0x3e, 0x82, 0xda, 0x0a, 0x44, 0x40, 0x8b, 0x16, 0xe3,
                                                                                                0x00, 0x53, 0x42, 0x00, 0x2e, 0x43, 0x40, 0x9a, 0x02, 0x52, 0x42, 0x00, 0x2e, 0x41, 0x40, 0x15, 0x54, 0x4a, 0x0e,
                                                                                                0x3a, 0x2f, 0x3a, 0x82, 0x00, 0x30, 0x41, 0x40, 0x21, 0x2e, 0x85, 0x0f, 0x40, 0xb2, 0x0a, 0x2f, 0x98, 0x2e, 0xb1,
                                                                                                0x0c, 0x98, 0x2e, 0x45, 0x0e, 0x98, 0x2e, 0x5b, 0x0e, 0xfb, 0x6f, 0xf0, 0x5f, 0x00, 0x30, 0x80, 0x2e, 0xce, 0xb7,
                                                                                                0xdd, 0x52, 0xd3, 0x54, 0x42, 0x42, 0x4f, 0x84, 0x73, 0x30, 0xdb, 0x52, 0x83, 0x42, 0x1b, 0x30, 0x6b, 0x42, 0x23,
                                                                                                0x30, 0x27, 0x2e, 0xd7, 0x00, 0x37, 0x2e, 0xd4, 0x00, 0x21, 0x2e, 0xd6, 0x00, 0x7a, 0x84, 0x17, 0x2c, 0x42, 0x42,
                                                                                                0x30, 0x30, 0x21, 0x2e, 0xd4, 0x00, 0x12, 0x2d, 0x21, 0x30, 0x00, 0x30, 0x23, 0x2e, 0xd4, 0x00, 0x21, 0x2e, 0x7b,
                                                                                                0xf7, 0x0b, 0x2d, 0x17, 0x30, 0x98, 0x2e, 0x51, 0x0c, 0xd5, 0x50, 0x0c, 0x82, 0x72, 0x30, 0x2f, 0x2e, 0xd4, 0x00,
                                                                                                0x25, 0x2e, 0x7b, 0xf7, 0x40, 0x42, 0x00, 0x2e, 0xfb, 0x6f, 0xf0, 0x5f, 0xb8, 0x2e, 0x70, 0x50, 0x0a, 0x25, 0x39,
                                                                                                0x86, 0xfb, 0x7f, 0xe1, 0x32, 0x62, 0x30, 0x98, 0x2e, 0xc2, 0xc4, 0xb5, 0x56, 0xa5, 0x6f, 0xab, 0x08, 0x91, 0x6f,
                                                                                                0x4b, 0x08, 0xdf, 0x56, 0xc4, 0x6f, 0x23, 0x09, 0x4d, 0xba, 0x93, 0xbc, 0x8c, 0x0b, 0xd1, 0x6f, 0x0b, 0x09, 0xcb,
                                                                                                0x52, 0xe1, 0x5e, 0x56, 0x42, 0xaf, 0x09, 0x4d, 0xba, 0x23, 0xbd, 0x94, 0x0a, 0xe5, 0x6f, 0x68, 0xbb, 0xeb, 0x08,
                                                                                                0xbd, 0xb9, 0x63, 0xbe, 0xfb, 0x6f, 0x52, 0x42, 0xe3, 0x0a, 0xc0, 0x2e, 0x43, 0x42, 0x90, 0x5f, 0xd1, 0x50, 0x03,
                                                                                                0x2e, 0x25, 0xf3, 0x13, 0x40, 0x00, 0x40, 0x9b, 0xbc, 0x9b, 0xb4, 0x08, 0xbd, 0xb8, 0xb9, 0x98, 0xbc, 0xda, 0x0a,
                                                                                                0x08, 0xb6, 0x89, 0x16, 0xc0, 0x2e, 0x19, 0x00, 0x62, 0x02, 0x10, 0x50, 0xfb, 0x7f, 0x98, 0x2e, 0x81, 0x0d, 0x01,
                                                                                                0x2e, 0xd4, 0x00, 0x31, 0x30, 0x08, 0x04, 0xfb, 0x6f, 0x01, 0x30, 0xf0, 0x5f, 0x23, 0x2e, 0xd6, 0x00, 0x21, 0x2e,
                                                                                                0xd7, 0x00, 0xb8, 0x2e, 0x01, 0x2e, 0xd7, 0x00, 0x03, 0x2e, 0xd6, 0x00, 0x48, 0x0e, 0x01, 0x2f, 0x80, 0x2e, 0x1f,
                                                                                                0x0e, 0xb8, 0x2e, 0xe3, 0x50, 0x21, 0x34, 0x01, 0x42, 0x82, 0x30, 0xc1, 0x32, 0x25, 0x2e, 0x62, 0xf5, 0x01, 0x00,
                                                                                                0x22, 0x30, 0x01, 0x40, 0x4a, 0x0a, 0x01, 0x42, 0xb8, 0x2e, 0xe3, 0x54, 0xf0, 0x3b, 0x83, 0x40, 0xd8, 0x08, 0xe5,
                                                                                                0x52, 0x83, 0x42, 0x00, 0x30, 0x83, 0x30, 0x50, 0x42, 0xc4, 0x32, 0x27, 0x2e, 0x64, 0xf5, 0x94, 0x00, 0x50, 0x42,
                                                                                                0x40, 0x42, 0xd3, 0x3f, 0x84, 0x40, 0x7d, 0x82, 0xe3, 0x08, 0x40, 0x42, 0x83, 0x42, 0xb8, 0x2e, 0xdd, 0x52, 0x00,
                                                                                                0x30, 0x40, 0x42, 0x7c, 0x86, 0xb9, 0x52, 0x09, 0x2e, 0x70, 0x0f, 0xbf, 0x54, 0xc4, 0x42, 0xd3, 0x86, 0x54, 0x40,
                                                                                                0x55, 0x40, 0x94, 0x42, 0x85, 0x42, 0x21, 0x2e, 0xd7, 0x00, 0x42, 0x40, 0x25, 0x2e, 0xfd, 0xf3, 0xc0, 0x42, 0x7e,
                                                                                                0x82, 0x05, 0x2e, 0x7d, 0x00, 0x80, 0xb2, 0x14, 0x2f, 0x05, 0x2e, 0x89, 0x00, 0x27, 0xbd, 0x2f, 0xb9, 0x80, 0x90,
                                                                                                0x02, 0x2f, 0x21, 0x2e, 0x6f, 0xf5, 0x0c, 0x2d, 0x07, 0x2e, 0x71, 0x0f, 0x14, 0x30, 0x1c, 0x09, 0x05, 0x2e, 0x77,
                                                                                                0xf7, 0xbd, 0x56, 0x47, 0xbe, 0x93, 0x08, 0x94, 0x0a, 0x25, 0x2e, 0x77, 0xf7, 0xe7, 0x54, 0x50, 0x42, 0x4a, 0x0e,
                                                                                                0xfc, 0x2f, 0xb8, 0x2e, 0x50, 0x50, 0x02, 0x30, 0x43, 0x86, 0xe5, 0x50, 0xfb, 0x7f, 0xe3, 0x7f, 0xd2, 0x7f, 0xc0,
                                                                                                0x7f, 0xb1, 0x7f, 0x00, 0x2e, 0x41, 0x40, 0x00, 0x40, 0x48, 0x04, 0x98, 0x2e, 0x74, 0xc0, 0x1e, 0xaa, 0xd3, 0x6f,
                                                                                                0x14, 0x30, 0xb1, 0x6f, 0xe3, 0x22, 0xc0, 0x6f, 0x52, 0x40, 0xe4, 0x6f, 0x4c, 0x0e, 0x12, 0x42, 0xd3, 0x7f, 0xeb,
                                                                                                0x2f, 0x03, 0x2e, 0x86, 0x0f, 0x40, 0x90, 0x11, 0x30, 0x03, 0x2f, 0x23, 0x2e, 0x86, 0x0f, 0x02, 0x2c, 0x00, 0x30,
                                                                                                0xd0, 0x6f, 0xfb, 0x6f, 0xb0, 0x5f, 0xb8, 0x2e, 0x40, 0x50, 0xf1, 0x7f, 0x0a, 0x25, 0x3c, 0x86, 0xeb, 0x7f, 0x41,
                                                                                                0x33, 0x22, 0x30, 0x98, 0x2e, 0xc2, 0xc4, 0xd3, 0x6f, 0xf4, 0x30, 0xdc, 0x09, 0x47, 0x58, 0xc2, 0x6f, 0x94, 0x09,
                                                                                                0xeb, 0x58, 0x6a, 0xbb, 0xdc, 0x08, 0xb4, 0xb9, 0xb1, 0xbd, 0xe9, 0x5a, 0x95, 0x08, 0x21, 0xbd, 0xf6, 0xbf, 0x77,
                                                                                                0x0b, 0x51, 0xbe, 0xf1, 0x6f, 0xeb, 0x6f, 0x52, 0x42, 0x54, 0x42, 0xc0, 0x2e, 0x43, 0x42, 0xc0, 0x5f, 0x50, 0x50,
                                                                                                0xf5, 0x50, 0x31, 0x30, 0x11, 0x42, 0xfb, 0x7f, 0x7b, 0x30, 0x0b, 0x42, 0x11, 0x30, 0x02, 0x80, 0x23, 0x33, 0x01,
                                                                                                0x42, 0x03, 0x00, 0x07, 0x2e, 0x80, 0x03, 0x05, 0x2e, 0xd3, 0x00, 0x23, 0x52, 0xe2, 0x7f, 0xd3, 0x7f, 0xc0, 0x7f,
                                                                                                0x98, 0x2e, 0xb6, 0x0e, 0xd1, 0x6f, 0x08, 0x0a, 0x1a, 0x25, 0x7b, 0x86, 0xd0, 0x7f, 0x01, 0x33, 0x12, 0x30, 0x98,
                                                                                                0x2e, 0xc2, 0xc4, 0xd1, 0x6f, 0x08, 0x0a, 0x00, 0xb2, 0x0d, 0x2f, 0xe3, 0x6f, 0x01, 0x2e, 0x80, 0x03, 0x51, 0x30,
                                                                                                0xc7, 0x86, 0x23, 0x2e, 0x21, 0xf2, 0x08, 0xbc, 0xc0, 0x42, 0x98, 0x2e, 0xa5, 0xb7, 0x00, 0x2e, 0x00, 0x2e, 0xd0,
                                                                                                0x2e, 0xb0, 0x6f, 0x0b, 0xb8, 0x03, 0x2e, 0x1b, 0x00, 0x08, 0x1a, 0xb0, 0x7f, 0x70, 0x30, 0x04, 0x2f, 0x21, 0x2e,
                                                                                                0x21, 0xf2, 0x00, 0x2e, 0x00, 0x2e, 0xd0, 0x2e, 0x98, 0x2e, 0x6d, 0xc0, 0x98, 0x2e, 0x5d, 0xc0, 0xed, 0x50, 0x98,
                                                                                                0x2e, 0x44, 0xcb, 0xef, 0x50, 0x98, 0x2e, 0x46, 0xc3, 0xf1, 0x50, 0x98, 0x2e, 0x53, 0xc7, 0x35, 0x50, 0x98, 0x2e,
                                                                                                0x64, 0xcf, 0x10, 0x30, 0x98, 0x2e, 0xdc, 0x03, 0x20, 0x26, 0xc0, 0x6f, 0x02, 0x31, 0x12, 0x42, 0xab, 0x33, 0x0b,
                                                                                                0x42, 0x37, 0x80, 0x01, 0x30, 0x01, 0x42, 0xf3, 0x37, 0xf7, 0x52, 0xfb, 0x50, 0x44, 0x40, 0xa2, 0x0a, 0x42, 0x42,
                                                                                                0x8b, 0x31, 0x09, 0x2e, 0x5e, 0xf7, 0xf9, 0x54, 0xe3, 0x08, 0x83, 0x42, 0x1b, 0x42, 0x23, 0x33, 0x4b, 0x00, 0xbc,
                                                                                                0x84, 0x0b, 0x40, 0x33, 0x30, 0x83, 0x42, 0x0b, 0x42, 0xe0, 0x7f, 0xd1, 0x7f, 0x98, 0x2e, 0x58, 0xb7, 0xd1, 0x6f,
                                                                                                0x80, 0x30, 0x40, 0x42, 0x03, 0x30, 0xe0, 0x6f, 0xf3, 0x54, 0x04, 0x30, 0x00, 0x2e, 0x00, 0x2e, 0x01, 0x89, 0x62,
                                                                                                0x0e, 0xfa, 0x2f, 0x43, 0x42, 0x11, 0x30, 0xfb, 0x6f, 0xc0, 0x2e, 0x01, 0x42, 0xb0, 0x5f, 0xc1, 0x4a, 0x00, 0x00,
                                                                                                0x6d, 0x57, 0x00, 0x00, 0x77, 0x8e, 0x00, 0x00, 0xe0, 0xff, 0xff, 0xff, 0xd3, 0xff, 0xff, 0xff, 0xe5, 0xff, 0xff,
                                                                                                0xff, 0xee, 0xe1, 0xff, 0xff, 0x7c, 0x13, 0x00, 0x00, 0x46, 0xe6, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x2e, 0x00, 0xc1, 0x80,
                                                                                                0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1,
                                                                                                0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00,
                                                                                                0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e,
                                                                                                0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80,
                                                                                                0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1,
                                                                                                0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00,
                                                                                                0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e,
                                                                                                0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80,
                                                                                                0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1,
                                                                                                0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00,
                                                                                                0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e,
                                                                                                0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80, 0x2e, 0x00, 0xc1, 0x80,
                                                                                                0x2e, 0x00, 0xc1
                                                                                            };



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOI2C6AXISTRACKINGBMI270::DIOI2C6AXISTRACKINGBMI270() : DIODEVICEI2C()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOI2C6AXISTRACKINGBMI270::DIOI2C6AXISTRACKINGBMI270() : DIODEVICEI2C()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOI2C6AXISTRACKINGBMI270::~DIOI2C6AXISTRACKINGBMI270()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOI2C6AXISTRACKINGBMI270::~DIOI2C6AXISTRACKINGBMI270()
{
  End();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOI2C6AXISTRACKINGBMI270::GetGPIOInterruptEntryID()
* @brief      Get GPIO interrupt entry ID
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOI2C6AXISTRACKINGBMI270::GetGPIOInterruptEntryID()
{
  return GPIOinterruptentryID;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C6AXISTRACKINGBMI270::SetGPIOInterruptEntryID(XDWORD GPIOentryID)
* @brief      Set GPIO interrupt entry ID
* @ingroup    DATAIO
* 
* @param[in]  GPIOentryID : GPI Oentry ID value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C6AXISTRACKINGBMI270::SetGPIOInterruptEntryID(XDWORD GPIOentryID)
{
  GPIOinterruptentryID = GPIOentryID;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C6AXISTRACKINGBMI270::IsNecessaryUpdate()
* @brief      Is necessary update
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C6AXISTRACKINGBMI270::IsNecessaryUpdate()
{
  return isnecessaryupdate;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C6AXISTRACKINGBMI270::SetIsNecessaryUpdate(bool isnecessaryupdate)
* @brief      Set is necessary update
* @ingroup    DATAIO
* 
* @param[in]  isnecessaryupdate : Isnecessaryupdate value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C6AXISTRACKINGBMI270::SetIsNecessaryUpdate(bool isnecessaryupdate)
{
   this->isnecessaryupdate = isnecessaryupdate; 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOI2C6AXISTRACKINGBMI270::Update()
* @brief      Update
* @ingroup    DATAIO
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOI2C6AXISTRACKINGBMI270::Update()
{
  XBYTE ret_value = 0xFF;
    
  if(!IsNecessaryUpdate()) return ret_value;
              
  XWORD int_status = 0;     // Variable to get interrupt status. 	    
  XBYTE rslt;	              // Status of api are returned to this variable. 
	
  // Loop to get interrupt. 
  // Clear buffer. 
  int_status = 0;

  // To get the interrupt status 
  rslt = bmi2_get_int_status(&int_status, &bmi2_dev);
  bmi2_error_codes_print_result(rslt);
	
  if(prev_status != -1 && int_status == prev_status || (int_status&0xFF) == 0)  
    {
      return ret_value;
    }

  // To check the interrupt status of any-motion. 
  if(int_status & DIOI2C6AXISTRACKINGBMI270_ANY_MOT_STATUS_MASK)
    {
      // XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("[ACCEL] Any-motion interrupt is generated"));         
      any_motion          = 1;       
    }
   else
    {
      any_motion          = 0;       
    }	    
    
  // To check the interrupt status of no-motion. 
  if(int_status & DIOI2C6AXISTRACKINGBMI270_NO_MOT_STATUS_MASK)
    {      
      // XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("[ACCEL] No-motion interrupt is generated"));       
      no_motion           = 1;      
    }
   else
    {
      no_motion           = 0;          
    }		
    
  // To check the interrupt status of no-motion. 
  if(int_status & DIOI2C6AXISTRACKINGBMI270_SIG_MOT_STATUS_MASK)
    {
      // XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("[ACCEL] Significant motion interrupt is generated"));  
      sig_motion          = 1;      
    }
   else
    {
      sig_motion          = 0;      
    }		
    
  if(int_status & DIOI2C6AXISTRACKINGBMI270_STEP_CNT_STATUS_MASK)
    {
      // XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("[ACCEL] Counter interrupt is generated"));       
      step_event          = 1;      
    }
   else
    {
      step_event          = 0;      
    }		
    
  if(int_status & DIOI2C6AXISTRACKINGBMI270_STEP_ACT_STATUS_MASK)
    {
      // XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("[ACCEL] Activity interrupt is generated"));      
    }
	
  prev_status = (XDWORDSIG)int_status;
    
  SetIsNecessaryUpdate(false);
      
  return ret_value;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C6AXISTRACKINGBMI270::GetSetAnyMove()
* @brief      Get set any move
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C6AXISTRACKINGBMI270::GetSetAnyMove()
{
  XBYTE any_motion_ = any_motion;
  
  if(any_motion)
    {
      any_motion          = 0;  
    }
  
  return any_motion_; 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C6AXISTRACKINGBMI270::GetSignificantMotion()
* @brief      Get significant motion
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C6AXISTRACKINGBMI270::GetSignificantMotion()
{
  XBYTE sig_motion_ = sig_motion;
  
  if(sig_motion)
    {
      sig_motion            = 0;  
      any_motion            = 0;
    }
        
  return sig_motion_;    
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C6AXISTRACKINGBMI270::GetStep_AddOne()
* @brief      Get step add one
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C6AXISTRACKINGBMI270::GetStep_AddOne()
{  
  XBYTE step_event_ = step_event;
  
  if(step_event)
    {
      step_event          = 0;
      any_motion          = 0;
    }        
  
  return step_event_; 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C6AXISTRACKINGBMI270::GetTap_AddOne()
* @brief      Get tap add one
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C6AXISTRACKINGBMI270::GetTap_AddOne()
{

  XBYTE tap_event_ = tap_event;
  
  if(tap_event)
    {
      any_motion          = 0;    
      tap_event           = 0;  
    }
    
  return tap_event_;         
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C6AXISTRACKINGBMI270::GetHighGMotion()
* @brief      Get high G motion
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C6AXISTRACKINGBMI270::GetHighGMotion()
{  
  XBYTE highGmotion_ = highGmotion;
  
  if(highGmotion)
    {
      any_motion          = 0;    
      highGmotion         = 0;  
    }
    
  return highGmotion_;      
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C6AXISTRACKINGBMI270::GetLowGMotion()
* @brief      Get low G motion
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C6AXISTRACKINGBMI270::GetLowGMotion()
{
  XBYTE lowGmotion_ = lowGmotion;
  
  if(lowGmotion)
    {
      any_motion            = 0;    
      lowGmotion            = 0;  
    }
      
  return lowGmotion_;     
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C6AXISTRACKINGBMI270::GetNotMove()
* @brief      Get not move
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C6AXISTRACKINGBMI270::GetNotMove()
{
  //  XBYTE no_motion_ = no_motion;
  //  
  //  if(no_motion)
  //      no_motion           = 0;  
  //  
  //  return no_motion_;    

  return 0;      
}
 

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C6AXISTRACKINGBMI270::End()
* @brief      End the object
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C6AXISTRACKINGBMI270::End()
{
  if(!DIODEVICE::End()) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C6AXISTRACKINGBMI270::IniDevice()
* @brief      Ini device
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C6AXISTRACKINGBMI270::IniDevice()
{
  if(!DIODEVICEI2C::IniDevice()) return false;

  if(diostream->Open())
    {
      if(diostream->WaitToConnected(timeout))
        {                   
          if(!Inicializate()) return false;

          return DIODEVICE::Ini();;
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C6AXISTRACKINGBMI270::Inicializate()
* @brief      Inicializate
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C6AXISTRACKINGBMI270::Inicializate()
{
  // Status of api are returned to this variable. 
  XBYTE rslt;

  // Accel sensor and any-motion feature are listed in array. 
  sens_list[0] = BMI2_ACCEL;
  sens_list[1] = BMI2_NO_MOTION;
  sens_list[2] = BMI2_SIG_MOTION;
  sens_list[3] = BMI2_ANY_MOTION;
  sens_list[4] = BMI2_STEP_DETECTOR;
  

  // Variable to get any-motion interrupt status. 
  //XWORD int_status = 0;

  // Interface reference is given as a parameter
  // For I2C : BMI2_I2C_INTF
  // For SPI : BMI2_SPI_INTF
   
  rslt = bmi2_interface_init(&bmi2_dev, BMI2_I2C_INTF);
  bmi2_error_codes_print_result(rslt);

  // Initialize bmi270. 
  rslt = bmi270_init(&bmi2_dev);
  bmi2_error_codes_print_result(rslt);

  if(rslt == BMI2_OK)
    {
        // Enable the selected sensors. 
        rslt = bmi270_sensor_enable(sens_list, 5, &bmi2_dev);        
        bmi2_error_codes_print_result(rslt);

        if(rslt == BMI2_OK)
          {
              // Set feature configurations 
              rslt = SetFeatureConfig(&bmi2_dev);
              bmi2_error_codes_print_result(rslt);

              if(rslt == BMI2_OK)
                {
                    // Map the feature interrupt. 
                    rslt = bmi270_map_feat_int(sens_int, 4, &bmi2_dev);
                    bmi2_error_codes_print_result(rslt);
                    
                    // XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("[ACCEL: Move the board]"));                  
                }
          }
    }
      
  sig_motion           = 0;
  highGmotion          = 0;
  lowGmotion           = 0;
  step_event           = 0;
  tap_event            = 0;
  any_motion           = 0;
  no_motion            = 0;

  //Enable interrupt pin
   
  return rslt;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C6AXISTRACKINGBMI270::ReadRegister(XBYTE offset, XBYTE* data, XDWORD sizedata)
* @brief      Read register
* @ingroup    DATAIO
* 
* @param[in]  offset : Offset value.
* @param[in]  data : Data buffer to use.
* @param[in]  sizedata : Sizedata value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C6AXISTRACKINGBMI270::ReadRegister(XBYTE offset, XBYTE* data, XDWORD sizedata)
{
  if(!diostream) return false;

  bool status;

  status = diostream->Write(&offset, 1)?true:false;
  if(status) 
    {
      status = diostream->WaitToFlushOutXBuffer(timeout);
    }

  if(!status) 
    {
      return false;
    }

  status = diostream->WaitToFilledReadingBuffer(sizedata, timeout);
  if(status) 
    {
      status = (diostream->Read(data, sizedata) == sizedata)?true:false;
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOI2C6AXISTRACKINGBMI270::WriteRegister(XBYTE offset, XBYTE* data, XDWORD sizedata)
* @brief      Write register
* @ingroup    DATAIO
* 
* @param[in]  offset : Offset value.
* @param[in]  data : Data buffer to use.
* @param[in]  sizedata : Sizedata value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOI2C6AXISTRACKINGBMI270::WriteRegister(XBYTE offset, XBYTE* data, XDWORD sizedata)
{
  if(!diostream) return false;

  XBUFFER buffer;
  bool    status;

  buffer.Add((XBYTE)offset);
  if(data && sizedata) buffer.Add(data, sizedata);

  status = diostream->Write(buffer)?true:false;
  if(status) status = diostream->WaitToFlushOutXBuffer(timeout);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         BMI2_INTF_RETURN_TYPE DIOI2C6AXISTRACKINGBMI270::bmi2_i2c_read(XBYTE reg_addr, XBYTE* reg_data, XDWORD len, void* intf_ptr)
* @brief      bmi2i2cread
* @ingroup    DATAIO
* 
* @param[in]  reg_addr : Reg addr value.
* @param[in]  reg_data : Reg data pointer to use.
* @param[in]  len : Len value.
* @param[in]  intf_ptr : Intf ptr pointer to use.
* 
* @return     BMI2_INTF_RETURN_TYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
BMI2_INTF_RETURN_TYPE DIOI2C6AXISTRACKINGBMI270::bmi2_i2c_read(XBYTE reg_addr, XBYTE* reg_data, XDWORD len, void* intf_ptr)
{
  return ReadRegister(reg_addr, reg_data, len);    
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         BMI2_INTF_RETURN_TYPE DIOI2C6AXISTRACKINGBMI270::bmi2_i2c_write(XBYTE reg_addr, const XBYTE* reg_data, XDWORD len, void* intf_ptr)
* @brief      bmi2i2cwrite
* @ingroup    DATAIO
* 
* @param[in]  reg_addr : Reg addr value.
* @param[in]  reg_data : Reg data value.
* @param[in]  len : Len value.
* @param[in]  intf_ptr : Intf ptr pointer to use.
* 
* @return     BMI2_INTF_RETURN_TYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
BMI2_INTF_RETURN_TYPE DIOI2C6AXISTRACKINGBMI270::bmi2_i2c_write(XBYTE reg_addr, const XBYTE* reg_data, XDWORD len, void* intf_ptr)
{
  return WriteRegister(reg_addr, (XBYTE*)reg_data, len);  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::SetFeatureConfig(struct bmi2_dev *bmi2_dev)
* @brief      Set feature config This internal API is used to set configurations for any-motion.
* @ingroup    DATAIO
* 
* @param[in]  bmi2_dev : Bmi2 dev pointer to use.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::SetFeatureConfig(struct bmi2_dev *bmi2_dev)
{

  /* Status of api are returned to this variable. */
  XBYTESIG rslt;

  /* Structure to define the type of sensor and its configurations. */
  struct bmi2_sens_config config;
    

    /**********************************************************************/    
//    /* Configure the type of feature. */
//    config.type = BMI2_ACCEL;
//
//    /* Get default configurations for the type of feature selected. */
//    rslt = bmi270_get_sensor_config(&config, 1, bmi2_dev);
//    bmi2_error_codes_print_result(rslt);
//
//    if(rslt == BMI2_OK)
//      {
//          /* NOTE: The user can change the following configuration parameters according to their requirement. */
//          /* Set Output Data Rate */
//          config.cfg.acc.odr = BMI2_ACC_ODR_200HZ;
//
//          /* Gravity range of the sensor (+/- 2G, 4G, 8G, 16G). */
//          config.cfg.acc.range = BMI2_ACC_RANGE_2G;
//
//          /* The bandwidth parameter is used to configure the number of sensor samples that are averaged
//           * if it is set to 2, then 2^(bandwidth parameter) samples
//           * are averaged, resulting in 4 averaged samples.
//           * Note1 : For more information, refer the datasheet.
//           * Note2 : A higher number of averaged samples will result in a lower noise level of the signal, but
//           * this has an adverse effect on the power consumed.
//           */
//          config.cfg.acc.bwp = BMI2_ACC_NORMAL_AVG4;
//
//          /* Enable the filter performance mode where averaging of samples
//           * will be done based on above set bandwidth and ODR.
//           * There are two modes
//           *  0 -> Ultra low power mode
//           *  1 -> High performance mode(Default)
//           * For more info refer datasheet.
//           */
//          config.cfg.acc.filter_perf = BMI2_PERF_OPT_MODE;
//
//          /* Set the accel configurations. */
//          rslt = bmi270_set_sensor_config(&config, 1, bmi2_dev);
//          bmi2_error_codes_print_result(rslt);
//      }
//        
    
	/**********************************************************************/
  /* Configure the type of feature. */
  config.type = BMI2_ANY_MOTION;

  /* Get default configurations for the type of feature selected. */
  rslt = bmi270_get_sensor_config(&config, 1, bmi2_dev);
  bmi2_error_codes_print_result(rslt);
  if(rslt == BMI2_OK)
    {
        /* NOTE: The user can change the following configuration parameters according to their requirement. */
        /* 1LSB equals 20ms. Default is 100ms, setting to 80ms. */
        config.cfg.any_motion.duration = 0x0A;//0x04;

        /* 1LSB equals to 0.48mg. Default is 83mg, setting to 50mg. */
        config.cfg.any_motion.threshold = 0x9C;//0x68;

        /* Set GEN_NEW configurations. */
        rslt = bmi270_set_sensor_config(&config, 1, bmi2_dev);
        bmi2_error_codes_print_result(rslt);
    }
	/**********************************************************************/		
	/* Configure the type of feature. */
  config.type = BMI2_NO_MOTION;

  /* Get default configurations for the type of feature selected. */
  rslt = bmi270_get_sensor_config(&config, 1, bmi2_dev);
  bmi2_error_codes_print_result(rslt);
  if(rslt == BMI2_OK)
    {
        /* NOTE: The user can change the following configuration parameters according to their requirement. */
        /* 1LSB equals 20ms. Default is 100ms, setting to 80ms. */
        config.cfg.no_motion.duration = 0x04;

        /* 1LSB equals to 0.48mg. Default is 83mg, setting to 50mg. */
        config.cfg.no_motion.threshold = 0x68;

        /* Set GEN_NEW configurations. */
        rslt = bmi270_set_sensor_config(&config, 1, bmi2_dev);
        bmi2_error_codes_print_result(rslt);
    }		
	/**********************************************************************/		
	/* Configure the type of feature. */
  config.type = BMI2_STEP_DETECTOR;

  /* Get default configurations for the type of feature selected. */
  rslt = bmi270_get_sensor_config(&config, 1, bmi2_dev);
  bmi2_error_codes_print_result(rslt);
  if(rslt == BMI2_OK)
    {
      //config.cfg.step_counter_params

        /* Set GEN_NEW configurations. */
        //rslt = bmi270_set_sensor_config(&config, 1, bmi2_dev);
        //bmi2_error_codes_print_result(rslt);
    }				
		
	/**********************************************************************/		
	/* Configure the type of feature. */
  config.type = BMI2_SIG_MOTION;

  /* Get default configurations for the type of feature selected. */
  rslt = bmi270_get_sensor_config(&config, 1, bmi2_dev);
  bmi2_error_codes_print_result(rslt);
  if(rslt == BMI2_OK)
    {

    }		
    
	/**********************************************************************/		
	/* Configure the type of feature. */
  //    config.type = BMI2_LOW_G;
  //
  //    /* Get default configurations for the type of feature selected. */
  //    rslt = bmi270_get_sensor_config(&config, 1, bmi2_dev);
  //    bmi2_error_codes_print_result(rslt); 
  //    if(rslt == BMI2_OK)
  //      {
  ////        config.cfg.low_g.duration
  ////        config.cfg.low_g.hysteresis
  ////        config.cfg.low_g.threshold         
  //      
  //      }		

  return rslt;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C6AXISTRACKINGBMI270::bmi2_delay_us(XDWORD period, void *intf_ptr)
* @brief      bmi2delayus
* @ingroup    DATAIO
* 
* @param[in]  period : Period value.
* @param[in]  intf_ptr : Intf ptr pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C6AXISTRACKINGBMI270::bmi2_delay_us(XDWORD period, void *intf_ptr)
{
    //coines_delay_usec(period);
    if(period >= 1000)
       GEN_XSLEEP.MilliSeconds(period/1000);
    else
      GEN_XSLEEP.MilliSeconds(1);
	
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_interface_init(struct bmi2_dev *bmi, XBYTE intf)
* @brief      bmi2interfaceinit
* @ingroup    DATAIO
* 
* @param[in]  bmi : Bmi pointer to use.
* @param[in]  intf : Intf value.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_interface_init(struct bmi2_dev *bmi, XBYTE intf)
{
  XBYTESIG rslt = BMI2_OK;
	//struct coines_board_info board_info;

  if(bmi != NULL)
    {
      GEN_XSLEEP.MilliSeconds(200);

      /* Bus configuration : I2C */
      if(intf == BMI2_I2C_INTF)
      {
          /* To initialize the user I2C function */
          dev_addr = BMI2_I2C_SEC_ADDR;
          bmi->intf = BMI2_I2C_INTF;
              
          //bmi->read = (bmi2_read_fptr_t)bmi2_i2c_read;
          //bmi->write = (bmi2_write_fptr_t)bmi2_i2c_write;            					
      }

      /* Assign device address to interface pointer */
      bmi->intf_ptr = &dev_addr;

      /* Configure delay in microseconds */
      //bmi->delay_us = (bmi2_delay_fptr_t)bmi2_delay_us;

      /* Configure max read/write length (in bytes) ( Supported length depends on target machine) */
      bmi->read_write_len = READ_WRITE_LEN;

      /* Assign to NULL to load the default config file. */
      bmi->config_file_ptr = NULL;

      GEN_XSLEEP.MilliSeconds(10);
    }
   else
    {
      rslt = BMI2_E_NULL_PTR;
    }

  return rslt;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C6AXISTRACKINGBMI270::bmi2_error_codes_print_result(XBYTESIG rslt)
* @brief      bmi2errorcodesprintresult
* @ingroup    DATAIO
* 
* @param[in]  rslt : Rslt value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C6AXISTRACKINGBMI270::bmi2_error_codes_print_result(XBYTESIG rslt)
{
	
  switch (rslt)
    {
      case BMI2_OK                            :                                                                                                                                                                                                                                                                                 break;
      case BMI2_W_FIFO_EMPTY                  : XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("[Warning [%d] : FIFO empty]"), rslt);              						                                                                                                                                                                        break;            
      case BMI2_W_PARTIAL_READ                : XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("Warning [%d] : FIFO partial read"), rslt);                                                                                                                                                                                             break;            
      case BMI2_E_NULL_PTR                    : XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("Error [%d] : Null pointer error. It occurs when the user tries to assign value (not address) to a pointer, which has been initialized to NULL."), rslt);                                                                               break;
      case BMI2_E_COM_FAIL                    : XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("Error [%d] : Communication failure error. It occurs due to read/write operation failure and also due " "to power failure during communication"), rslt);                                                                                break;
      case BMI2_E_DEV_NOT_FOUND               : XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("Error [%d] : Device not found error. It occurs when the device chip id is incorrectly read"), rslt);                                                                                                                                   break;
      case BMI2_E_INVALID_SENSOR              : XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("Error [%d] : Invalid sensor error. It occurs when there is a mismatch in the requested feature with the " "available one "), rslt);                                                                                                    break;
      case BMI2_E_SELF_TEST_FAIL              : XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("Error [%d] : Self-test failed error. It occurs when the validation of accel self-test data is " "not satisfied "), rslt);                                                                                                              break;
      case BMI2_E_INVALID_INT_PIN             : XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("Error [%d] : Invalid interrupt pin error. It occurs when the user tries to configure interrupt pins " "apart from INT1 and INT2 "), rslt);                                                                                             break;
      case BMI2_E_OUT_OF_RANGE                : XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("Error [%d] : Out of range error. It occurs when the data exceeds from filtered or unfiltered data from " "fifo and also when the range exceeds the maximum range for accel and gyro while performing FOC "), rslt);                    break;
      case BMI2_E_ACC_INVALID_CFG             : XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("Error [%d] : Invalid Accel configuration error. It occurs when there is an error in accel configuration" " register which could be one among range, BW or filter performance in reg address 0x40"), rslt);                             break;
      case BMI2_E_GYRO_INVALID_CFG            : XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("Error [%d] : Invalid Gyro configuration error. It occurs when there is a error in gyro configuration" "register which could be one among range, BW or filter performance in reg address 0x42 "), rslt);                                break;
      case BMI2_E_ACC_GYR_INVALID_CFG         : XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("Error [%d] : Invalid Accel-Gyro configuration error. It occurs when there is a error in accel and gyro" " configuration registers which could be one among range, BW or filter performance in reg address 0x40 " "and 0x42"), rslt);   break;
      case BMI2_E_CONFIG_LOAD                 : XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("Error [%d] : Configuration load error. It occurs when failure observed while loading the configuration " "into the sensor "), rslt);                                                                                                   break;
      case BMI2_E_INVALID_PAGE                : XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("Error [%d] : Invalid page error. It occurs due to failure in writing the correct feature configuration " "from selected page "), rslt);                                                                                                break;
      case BMI2_E_SET_APS_FAIL                : XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("Error [%d] : APS failure error. It occurs due to failure in write of advance power mode configuration " "register "), rslt);                                                                                                           break;
      case BMI2_E_AUX_INVALID_CFG             : XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("Error [%d] : Invalid AUX configuration error. It occurs when the auxiliary interface settings are not " "enabled properly "), rslt);                                                                                                   break;
      case BMI2_E_AUX_BUSY                    : XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("Error [%d] : AUX busy error. It occurs when the auxiliary interface buses are engaged while configuring" " the AUX "), rslt);                                                                                                          break;
      case BMI2_E_REMAP_ERROR                 : XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("Error [%d] : Remap error. It occurs due to failure in assigning the remap axes data for all the axes " "after change in axis position "), rslt);                                                                                       break;
      case BMI2_E_GYR_USER_GAIN_UPD_FAIL      :	XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("Error [%d] : Gyro user gain update fail error. It occurs when the reading of user gain update status " "fails "), rslt);                                                                                                               break;
      case BMI2_E_SELF_TEST_NOT_DONE          : XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("Error [%d] : Self-test not done error. It occurs when the self-test process is ongoing or not " "completed "), rslt);			                                                                                                            break;
      case BMI2_E_INVALID_INPUT               : XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("Error [%d] : Invalid input error. It occurs when the sensor input validity fails "), rslt);                                                                                                                                            break;
      case BMI2_E_INVALID_STATUS              : XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("Error [%d] : Invalid status error. It occurs when the feature/sensor validity fails "), rslt);                                                                                                                                       	break;
      case BMI2_E_CRT_ERROR                   : XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("Error [%d] : CRT error. It occurs when the CRT test has failed "), rslt);	                                                                                                                                                            break;
      case BMI2_E_ST_ALREADY_RUNNING          : XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("Error [%d] : Self-test already running error. It occurs when the self-test is already running and " "another has been initiated "), rslt);		                                                                                          break;
      case BMI2_E_CRT_READY_FOR_DL_FAIL_ABORT : XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("Error [%d] : CRT ready for download fail abort error. It occurs when download in CRT fails due to wrong " "address location "), rslt);	                                                                                                break;
      case BMI2_E_DL_ERROR                    : XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("Error [%d] : Download error. It occurs when write length exceeds that of the maximum burst length "), rslt);				                                                                                                                    break;
      case BMI2_E_PRECON_ERROR                : XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("Error [%d] : Pre-conditional error. It occurs when precondition to start the feature was not " "completed "), rslt);                                                                                                                  break;
      case BMI2_E_ABORT_ERROR                 : XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("Error [%d] : Abort error. It occurs when the device was shaken during CRT test "), rslt);				                                                                                                                                      break;
      case BMI2_E_WRITE_CYCLE_ONGOING         : XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("Error [%d] : Write cycle ongoing error. It occurs when the write cycle is already running and another " "has been initiated "), rslt);	                                                                                                break;
      case BMI2_E_ST_NOT_RUNING               : XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("Error [%d] : Self-test is not running error. It occurs when self-test running is disabled while it's " "running "), rslt);				                                                                                                      break;
      case BMI2_E_DATA_RDY_INT_FAILED         : XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("Error [%d] : Data ready interrupt error. It occurs when the sample count exceeds the FOC sample limit " "and data ready status is not updated "), rslt);	                                                                              break;
      case BMI2_E_INVALID_FOC_POSITION        : XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("Error [%d] : Invalid FOC position error. It occurs when average FOC data is obtained for the wrong" " axes "), rslt);	                                                                                                                break;
                                   default    : XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, _L("Error [%d] : Unknown error code "), rslt);                                                                                                                                                                                             break;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_sec_init(struct bmi2_dev *dev)
* @brief      bmi2secinit
* @ingroup    DATAIO
* 
* @param[in]  dev : Dev pointer to use.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_sec_init(struct bmi2_dev *dev)
{
  /* Variable to define error */
  XBYTESIG rslt;

  /* Variable to assign chip id */
  XBYTE chip_id = 0;

  /* Structure to define the default values for axes re-mapping */
  struct bmi2_axes_remap axes_remap = { BMI2_MAP_X_AXIS, BMI2_MAP_Y_AXIS,BMI2_MAP_Z_AXIS, BMI2_POS_SIGN, BMI2_POS_SIGN, BMI2_POS_SIGN };

  /* Null-pointer check */
  rslt = null_ptr_check(dev);
  if(rslt == BMI2_OK)
  {
      /* Perform soft-reset to bring all register values to their
        * default values
        */
      rslt = bmi2_soft_reset(dev);

      if(rslt == BMI2_OK)
      {
          /* Read chip-id of the BMI2 sensor */
          rslt = bmi2_get_regs(BMI2_CHIP_ID_ADDR, &chip_id, 1, dev);

          if(rslt == BMI2_OK)
          {
              /* Validate chip-id */
              if(chip_id == dev->chip_id)
              {
                  /* Assign resolution to the structure */
                  dev->resolution = 16;

                  /* Set manual enable flag */
                  dev->aux_man_en = 1;

                  /* Set the default values for axis
                    *  re-mapping in the device structure
                    */
                  dev->remap = axes_remap;
              }
              else
              {
                  /* Storing the chip-id value read from
                    * the register to identify the sensor
                    */
                  dev->chip_id = chip_id;
                  rslt = BMI2_E_DEV_NOT_FOUND;
              }
          }
      }
  }

  return rslt;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_get_regs(XBYTE reg_addr, XBYTE *data, XWORD len, struct bmi2_dev *dev)
* @brief      bmi2getregs
* @note       For most of the registers auto address increment applies, with the exception of a few special registers, which trap the address.
* @ingroup    DATAIO
* 
* @param[in]  reg_addr : Reg addr value.
* @param[in]  data : Data buffer to use.
* @param[in]  len : Len value.
* @param[in]  dev : Dev pointer to use.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_get_regs(XBYTE reg_addr, XBYTE *data, XWORD len, struct bmi2_dev *dev)
{
  // Variable to define error 
  XBYTESIG rslt;

  // Variable to define loop 
  XWORD index = 0;
    
  XBYTE temp_buf[200]; 

  // Null-pointer check 
  rslt = null_ptr_check(dev);
  if((rslt == BMI2_OK) && (data != NULL))
    {
      // Variable to define temporary length 
      XWORD temp_len = len + dev->dummy_byte;        
        
      if(temp_len > 200)
        return BMI2_E_NULL_PTR;

      // Configuring reg_addr for SPI Interface 
      if(dev->intf == BMI2_SPI_INTF)
        {
          reg_addr = (reg_addr | BMI2_SPI_RD_MASK);
        }

      dev->intf_rslt = bmi2_i2c_read(reg_addr, temp_buf, temp_len, dev->intf_ptr);

      if(dev->aps_status == BMI2_ENABLE)
        {
          //dev->delay_us(450, dev->intf_ptr);
          bmi2_delay_us(450, dev->intf_ptr);
        }
       else
        {
          //dev->delay_us(2, dev->intf_ptr);
          bmi2_delay_us(2, dev->intf_ptr);
        }

      if(dev->intf_rslt == BMI2_INTF_RET_SUCCESS)
      {
          /* Read the data from the position next to dummy byte */
          while (index < len)
          {
              data[index] = temp_buf[index + dev->dummy_byte];
              index++;
          }
      }
      else
      {
          rslt = BMI2_E_COM_FAIL;
      }
  }
  else
  {
      rslt = BMI2_E_NULL_PTR;
  }
    
  return rslt;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_set_regs(XBYTE reg_addr, const XBYTE *data, XWORD len, struct bmi2_dev *dev)
* @brief      bmi2setregs
* @ingroup    DATAIO
* 
* @param[in]  reg_addr : Reg addr value.
* @param[in]  data : Data buffer to use.
* @param[in]  len : Len value.
* @param[in]  dev : Dev pointer to use.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_set_regs(XBYTE reg_addr, const XBYTE *data, XWORD len, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if((rslt == BMI2_OK) && (data != NULL))
    {
        /* Configuring reg_addr for SPI Interface */
        if(dev->intf == BMI2_SPI_INTF)
        {
            reg_addr = (reg_addr & BMI2_SPI_WR_MASK);
        }

        //dev->intf_rslt = dev->write(reg_addr, data, len, dev->intf_ptr);
        dev->intf_rslt = bmi2_i2c_write(reg_addr, data, len, dev->intf_ptr);

        /* Delay for Low power mode of the sensor is 450 us */
        if(dev->aps_status == BMI2_ENABLE)
        {
            //dev->delay_us(450, dev->intf_ptr);
           bmi2_delay_us(450, dev->intf_ptr);
        }
        /* Delay for Normal mode of the sensor is 2 us */
        else
        {
            //dev->delay_us(2, dev->intf_ptr);
            bmi2_delay_us(2, dev->intf_ptr);
        }

        /* updating the advance power saver flag */
        if(reg_addr == BMI2_PWR_CONF_ADDR)
        {
            if(*data & BMI2_ADV_POW_EN_MASK)
            {
                dev->aps_status = BMI2_ENABLE;
            }
            else
            {
                dev->aps_status = BMI2_DISABLE;
            }
        }

        if(dev->intf_rslt != BMI2_INTF_RET_SUCCESS)
        {
            rslt = BMI2_E_COM_FAIL;
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_soft_reset(struct bmi2_dev *dev)
* @brief      bmi2softreset
* @ingroup    DATAIO
* 
* @param[in]  dev : Dev pointer to use.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_soft_reset(struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to define soft reset value */
    XBYTE data = BMI2_SOFT_RESET_CMD;

    /* Variable to read the dummy byte */
    XBYTE dummy_read = 0;

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if(rslt == BMI2_OK)
    {
        /* Reset bmi2 device */
        rslt = bmi2_set_regs(BMI2_CMD_REG_ADDR, &data, 1, dev);
        //dev->delay_us(2000, dev->intf_ptr);
        bmi2_delay_us(2000, dev->intf_ptr);

        /* set APS flag as after soft reset the sensor is on advance power save mode */
        dev->aps_status = BMI2_ENABLE;

        /* Performing a dummy read to bring interface back to SPI from
         * I2C after a soft-reset
         */
        if((rslt == BMI2_OK) && (dev->intf == BMI2_SPI_INTF))
        {
            rslt = bmi2_get_regs(BMI2_CHIP_ID_ADDR, &dummy_read, 1, dev);
        }

        if(rslt == BMI2_OK)
        {
            /* Write the configuration file */
            rslt = bmi2_write_config_file(dev);
        }

        /* Reset the sensor status flag in the device structure */
        if(rslt == BMI2_OK)
        {
            dev->sens_en_stat = 0;
        }
    }

    return rslt;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_get_config_file_version(XBYTE *config_major, XBYTE *config_minor, struct bmi2_dev *dev)
* @brief      bmi2getconfigfileversion
* @ingroup    DATAIO
* 
* @param[in]  config_major : Config major pointer to use.
* @param[in]  config_minor : Config minor pointer to use.
* @param[in]  dev : Dev pointer to use.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_get_config_file_version(XBYTE *config_major, XBYTE *config_minor, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* NULL pointer check */
    rslt = null_ptr_check(dev);
    if((rslt == BMI2_OK) && (config_major != NULL) && (config_minor != NULL))
    {
        /* Extract the config file identification from the dmr page and get the major and minor version */
        rslt = extract_config_file(config_major, config_minor, dev);
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_set_adv_power_save(XBYTE enable, struct bmi2_dev *dev)
* @brief      bmi2setadvpowersave
* @ingroup    DATAIO
* 
* @param[in]  enable : Enable value.
* @param[in]  dev : Dev pointer to use.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_set_adv_power_save(XBYTE enable, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to store data */
    XBYTE reg_data = 0;

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if(rslt == BMI2_OK)
    {
        rslt = bmi2_get_regs(BMI2_PWR_CONF_ADDR, &reg_data, 1, dev);
        if(rslt == BMI2_OK)
        {
            reg_data = BMI2_SET_BIT_POS0(reg_data, BMI2_ADV_POW_EN, enable);
            rslt = bmi2_set_regs(BMI2_PWR_CONF_ADDR, &reg_data, 1, dev);

            if(rslt != BMI2_OK)
            {
                /* Return error if enable/disable APS fails */
                rslt = BMI2_E_SET_APS_FAIL;
            }

            if(rslt == BMI2_OK)
            {
                dev->aps_status = BMI2_GET_BIT_POS0(reg_data, BMI2_ADV_POW_EN);
            }
        }
    }

    return rslt;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_get_adv_power_save(XBYTE *aps_status, struct bmi2_dev *dev)
* @brief      bmi2getadvpowersave
* @ingroup    DATAIO
* 
* @param[in]  aps_status : Aps status pointer to use.
* @param[in]  dev : Dev pointer to use.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_get_adv_power_save(XBYTE *aps_status, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to store data */
    XBYTE reg_data = 0;

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if((rslt == BMI2_OK) && (aps_status != NULL))
    {
        rslt = bmi2_get_regs(BMI2_PWR_CONF_ADDR, &reg_data, 1, dev);
        if(rslt == BMI2_OK)
        {
            *aps_status = BMI2_GET_BIT_POS0(reg_data, BMI2_ADV_POW_EN);
            dev->aps_status = *aps_status;
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_write_config_file(struct bmi2_dev *dev)
* @brief      bmi2writeconfigfile
* @ingroup    DATAIO
* 
* @param[in]  dev : Dev pointer to use.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_write_config_file(struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to know the load status */
    XBYTE load_status = 0;

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if((rslt == BMI2_OK) && (dev->config_size != 0))
    {
        /* Bytes written are multiples of 2 */
        if((dev->read_write_len % 2) != 0)
        {
            dev->read_write_len = dev->read_write_len - 1;
        }

        if(dev->read_write_len < 2)
        {
            dev->read_write_len = 2;
        }

        /* Write the configuration file */
        rslt = write_config_file(dev);
        if(rslt == BMI2_OK)
        {
            /* Check the configuration load status */
            rslt = bmi2_get_internal_status(&load_status, dev);

            /* Return error if loading not successful */
            if((rslt == BMI2_OK) && (!(load_status & BMI2_CONFIG_LOAD_SUCCESS)))
            {
                rslt = BMI2_E_CONFIG_LOAD;
            }
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_set_int_pin_config(const struct bmi2_int_pin_config *int_cfg, struct bmi2_dev *dev)
* @brief      bmi2setintpinconfig 2)The interrupt mode: permanently latched or non-latched.
* @ingroup    DATAIO
* 
* @param[in]  int_cfg : Int configuration value.
* @param[in]  dev : Dev pointer to use.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_set_int_pin_config(const struct bmi2_int_pin_config *int_cfg, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to define data array */
    XBYTE data_array[3] = { 0 };

    /* Variable to store register data */
    XBYTE reg_data = 0;

    /* Variable to define type of interrupt pin  */
    XBYTE int_pin = 0;

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if((rslt == BMI2_OK) && (int_cfg != NULL))
    {
        /* Copy the pin type to a local variable */
        int_pin = int_cfg->pin_type;
        if((int_pin > BMI2_INT_NONE) && (int_pin < BMI2_INT_PIN_MAX))
        {
            /* Get the previous configuration data */
            rslt = bmi2_get_regs(BMI2_INT1_IO_CTRL_ADDR, data_array, 3, dev);
            if(rslt == BMI2_OK)
            {
                /* Set interrupt pin 1 configuration */
                if((int_pin == BMI2_INT1) || (int_pin == BMI2_INT_BOTH))
                {
                    /* Configure active low or high */
                    reg_data = BMI2_SET_BITS(data_array[0], BMI2_INT_LEVEL, int_cfg->pin_cfg[0].lvl);

                    /* Configure push-pull or open drain */
                    reg_data = BMI2_SET_BITS(reg_data, BMI2_INT_OPEN_DRAIN, int_cfg->pin_cfg[0].od);

                    /* Configure output enable */
                    reg_data = BMI2_SET_BITS(reg_data, BMI2_INT_OUTPUT_EN, int_cfg->pin_cfg[0].output_en);

                    /* Configure input enable */
                    reg_data = BMI2_SET_BITS(reg_data, BMI2_INT_INPUT_EN, int_cfg->pin_cfg[0].input_en);

                    /* Copy the data to be written in the respective array */
                    data_array[0] = reg_data;
                }

                /* Set interrupt pin 2 configuration */
                if((int_pin == BMI2_INT2) || (int_pin == BMI2_INT_BOTH))
                {
                    /* Configure active low or high */
                    reg_data = BMI2_SET_BITS(data_array[1], BMI2_INT_LEVEL, int_cfg->pin_cfg[1].lvl);

                    /* Configure push-pull or open drain */
                    reg_data = BMI2_SET_BITS(reg_data, BMI2_INT_OPEN_DRAIN, int_cfg->pin_cfg[1].od);

                    /* Configure output enable */
                    reg_data = BMI2_SET_BITS(reg_data, BMI2_INT_OUTPUT_EN, int_cfg->pin_cfg[1].output_en);

                    /* Configure input enable */
                    reg_data = BMI2_SET_BITS(reg_data, BMI2_INT_INPUT_EN, int_cfg->pin_cfg[1].input_en);

                    /* Copy the data to be written in the respective array */
                    data_array[1] = reg_data;
                }

                /* Configure the interrupt mode */
                data_array[2] = BMI2_SET_BIT_POS0(data_array[2], BMI2_INT_LATCH, int_cfg->int_latch);

                /* Set the configurations simultaneously as
                 * INT1_IO_CTRL, INT2_IO_CTRL, and INT_LATCH lie
                 * in consecutive addresses
                 */
                rslt = bmi2_set_regs(BMI2_INT1_IO_CTRL_ADDR, data_array, 3, dev);
            }
        }
        else
        {
            rslt = BMI2_E_INVALID_INT_PIN;
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_get_int_pin_config(struct bmi2_int_pin_config *int_cfg, struct bmi2_dev *dev)
* @brief      bmi2getintpinconfig 1) The input output configuration of the selected interrupt pin: INT1 or INT2. 2) The interrupt mode: permanently latched or non-latched.
* @ingroup    DATAIO
* 
* @param[in]  int_cfg : Int configuration pointer to use.
* @param[in]  dev : Dev pointer to use.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_get_int_pin_config(struct bmi2_int_pin_config *int_cfg, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to define data array */
    XBYTE data_array[3] = { 0 };

    /* Variable to define type of interrupt pin  */
    XBYTE int_pin = 0;

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if((rslt == BMI2_OK) && (int_cfg != NULL))
    {
        /* Copy the pin type to a local variable */
        int_pin = int_cfg->pin_type;

        /* Get the previous configuration data */
        rslt = bmi2_get_regs(BMI2_INT1_IO_CTRL_ADDR, data_array, 3, dev);
        if(rslt == BMI2_OK)
        {
            /* Get interrupt pin 1 configuration */
            if((int_pin == BMI2_INT1) || (int_pin == BMI2_INT_BOTH))
            {
                /* Get active low or high */
                int_cfg->pin_cfg[0].lvl = BMI2_GET_BITS(data_array[0], BMI2_INT_LEVEL);

                /* Get push-pull or open drain */
                int_cfg->pin_cfg[0].od = BMI2_GET_BITS(data_array[0], BMI2_INT_OPEN_DRAIN);

                /* Get output enable */
                int_cfg->pin_cfg[0].output_en = BMI2_GET_BITS(data_array[0], BMI2_INT_OUTPUT_EN);

                /* Get input enable */
                int_cfg->pin_cfg[0].input_en = BMI2_GET_BITS(data_array[0], BMI2_INT_INPUT_EN);
            }

            /* Get interrupt pin 2 configuration */
            if((int_pin == BMI2_INT2) || (int_pin == BMI2_INT_BOTH))
            {
                /* Get active low or high */
                int_cfg->pin_cfg[1].lvl = BMI2_GET_BITS(data_array[1], BMI2_INT_LEVEL);

                /* Get push-pull or open drain */
                int_cfg->pin_cfg[1].od = BMI2_GET_BITS(data_array[1], BMI2_INT_OPEN_DRAIN);

                /* Get output enable */
                int_cfg->pin_cfg[1].output_en = BMI2_GET_BITS(data_array[1], BMI2_INT_OUTPUT_EN);

                /* Get input enable */
                int_cfg->pin_cfg[1].input_en = BMI2_GET_BITS(data_array[1], BMI2_INT_INPUT_EN);
            }

            /* Get interrupt mode */
            int_cfg->int_latch = BMI2_GET_BIT_POS0(data_array[2], BMI2_INT_LATCH);
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_get_int_status(XWORD *int_status, struct bmi2_dev *dev)
* @brief      bmi2getintstatus
* @ingroup    DATAIO
* 
* @param[in]  int_status : Int status pointer to use.
* @param[in]  dev : Dev pointer to use.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_get_int_status(XWORD *int_status, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Array to store data */
    XBYTE data_array[2] = { 0 };

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if((rslt == BMI2_OK) && (int_status != NULL))
    {
        /* Get the interrupt status */
        rslt = bmi2_get_regs(BMI2_INT_STATUS_0_ADDR, data_array, 2, dev);
        if(rslt == BMI2_OK)
        {
            *int_status = (XWORD) data_array[0] | ((XWORD) data_array[1] << 8);
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_sensor_enable(const XBYTE *sens_list, XBYTE n_sens, struct bmi2_dev *dev)
* @brief      bmi2sensorenable
* @ingroup    DATAIO
* 
* @param[in]  sens_list : Sens list value.
* @param[in]  n_sens : N sens value.
* @param[in]  dev : Dev pointer to use.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_sensor_enable(const XBYTE *sens_list, XBYTE n_sens, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to select sensor */
    XQWORD sensor_sel = 0;

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if((rslt == BMI2_OK) && (sens_list != NULL))
    {
        /* Get the selected sensors */
        rslt = select_sensor(sens_list, n_sens, &sensor_sel);
        if(rslt == BMI2_OK)
        {
            /* Enable the selected sensors */
            rslt = sensor_enable(sensor_sel, dev);
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_sensor_disable(const XBYTE *sens_list, XBYTE n_sens, struct bmi2_dev *dev)
* @brief      bmi2sensordisable
* @ingroup    DATAIO
* 
* @param[in]  sens_list : Sens list value.
* @param[in]  n_sens : N sens value.
* @param[in]  dev : Dev pointer to use.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_sensor_disable(const XBYTE *sens_list, XBYTE n_sens, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to select sensor */
    XQWORD sensor_sel = 0;

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if((rslt == BMI2_OK) && (sens_list != NULL))
    {
        /* Get the selected sensors */
        rslt = select_sensor(sens_list, n_sens, &sensor_sel);
        if(rslt == BMI2_OK)
        {
            /* Disable the selected sensors */
            rslt = sensor_disable(sensor_sel, dev);
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_set_sensor_config(struct bmi2_sens_config *sens_cfg, XBYTE n_sens, struct bmi2_dev *dev)
* @brief      bmi2setsensorconfig
* @ingroup    DATAIO
* 
* @param[in]  sens_cfg : Sens configuration pointer to use.
* @param[in]  n_sens : N sens value.
* @param[in]  dev : Dev pointer to use.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_set_sensor_config(struct bmi2_sens_config *sens_cfg, XBYTE n_sens, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to define loop */
    XBYTE loop;

    /* Variable to get the status of advance power save */
    XBYTE aps_stat = 0;

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if((rslt == BMI2_OK) && (sens_cfg != NULL))
    {
        /* Get status of advance power save mode */
        aps_stat = dev->aps_status;

        for (loop = 0; loop < n_sens; loop++)
        {
            /* Disable Advance power save if enabled for auxiliary
             * and feature configurations
             */
            if(aps_stat == BMI2_ENABLE)
            {
                /* Disable advance power save if
                 * enabled
                 */
                rslt = bmi2_set_adv_power_save(BMI2_DISABLE, dev);
            }

            if(rslt == BMI2_OK)
            {
                switch (sens_cfg[loop].type)
                {
                    /* Set accelerometer configuration */
                    case BMI2_ACCEL:
                        rslt = set_accel_config(&sens_cfg[loop].cfg.acc, dev);
                        break;

                    /* Set gyroscope configuration */
                    case BMI2_GYRO:
                        rslt = set_gyro_config(&sens_cfg[loop].cfg.gyr, dev);
                        break;

                    /* Set auxiliary configuration */
                    case BMI2_AUX:
                        rslt = set_aux_config(&sens_cfg[loop].cfg.aux, dev);
                        break;

                    /* Set gyroscope user gain configuration */
                    case BMI2_GYRO_GAIN_UPDATE:
                        rslt = set_gyro_user_gain_config(&sens_cfg[loop].cfg.gyro_gain_update, dev);
                        break;

                    default:
                        rslt = BMI2_E_INVALID_SENSOR;
                        break;
                }
            }

            /* Return error if any of the set configurations fail */
            if(rslt != BMI2_OK)
            {
                break;
            }
        }

        /* Enable Advance power save if disabled while configuring and
         * not when already disabled
         */
        if((aps_stat == BMI2_ENABLE) && (rslt == BMI2_OK))
        {
            rslt = bmi2_set_adv_power_save(BMI2_ENABLE, dev);
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_get_sensor_config(struct bmi2_sens_config *sens_cfg, XBYTE n_sens, struct bmi2_dev *dev)
* @brief      bmi2getsensorconfig
* @ingroup    DATAIO
* 
* @param[in]  sens_cfg : Sens configuration pointer to use.
* @param[in]  n_sens : N sens value.
* @param[in]  dev : Dev pointer to use.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_get_sensor_config(struct bmi2_sens_config *sens_cfg, XBYTE n_sens, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to define loop */
    XBYTE loop;

    /* Variable to get the status of advance power save */
    XBYTE aps_stat = 0;

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if((rslt == BMI2_OK) && (sens_cfg != NULL))
    {
        /* Get status of advance power save mode */
        aps_stat = dev->aps_status;
        for (loop = 0; loop < n_sens; loop++)
        {
            /* Disable Advance power save if enabled for auxiliary
             * and feature configurations
             */
            if((sens_cfg[loop].type >= BMI2_MAIN_SENS_MAX_NUM) || (sens_cfg[loop].type == BMI2_AUX))
            {

                if(aps_stat == BMI2_ENABLE)
                {
                    /* Disable advance power save if
                     * enabled
                     */
                    rslt = bmi2_set_adv_power_save(BMI2_DISABLE, dev);
                }
            }

            if(rslt == BMI2_OK)
            {
                switch (sens_cfg[loop].type)
                {
                    /* Get accelerometer configuration */
                    case BMI2_ACCEL:
                        rslt = get_accel_config(&sens_cfg[loop].cfg.acc, dev);
                        break;

                    /* Get gyroscope configuration */
                    case BMI2_GYRO:
                        rslt = get_gyro_config(&sens_cfg[loop].cfg.gyr, dev);
                        break;

                    /* Get auxiliary configuration */
                    case BMI2_AUX:
                        rslt = get_aux_config(&sens_cfg[loop].cfg.aux, dev);
                        break;

                    /* Get gyroscope user gain configuration */
                    case BMI2_GYRO_GAIN_UPDATE:
                        rslt = get_gyro_gain_update_config(&sens_cfg[loop].cfg.gyro_gain_update, dev);
                        break;

                    default:
                        rslt = BMI2_E_INVALID_SENSOR;
                        break;
                }
            }

            /* Return error if any of the get configurations fail */
            if(rslt != BMI2_OK)
            {
                break;
            }
        }

        /* Enable Advance power save if disabled while configuring and
         * not when already disabled
         */
        if((aps_stat == BMI2_ENABLE) && (rslt == BMI2_OK))
        {
            rslt = bmi2_set_adv_power_save(BMI2_ENABLE, dev);
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_get_sensor_data(struct bmi2_sensor_data *sensor_data, XBYTE n_sens, struct bmi2_dev *dev)
* @brief      bmi2getsensordata auxiliary sensor, step counter, high-g, gyroscope user-gain update, orientation, gyroscope cross sensitivity and error status for NVM and VFRM.
* @ingroup    DATAIO
* 
* @param[in]  sensor_data : Sensor data pointer to use.
* @param[in]  n_sens : N sens value.
* @param[in]  dev : Dev pointer to use.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_get_sensor_data(struct bmi2_sensor_data *sensor_data, XBYTE n_sens, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to define loop */
    XBYTE loop;

    /* Variable to get the status of advance power save */
    XBYTE aps_stat = 0;

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if((rslt == BMI2_OK) && (sensor_data != NULL))
    {
        /* Get status of advance power save mode */
        aps_stat = dev->aps_status;
        for (loop = 0; loop < n_sens; loop++)
        {
            /* Disable Advance power save if enabled for feature
             * configurations
             */
            if(sensor_data[loop].type >= BMI2_MAIN_SENS_MAX_NUM)
            {
                if(aps_stat == BMI2_ENABLE)
                {
                    /* Disable advance power save if
                     * enabled
                     */
                    rslt = bmi2_set_adv_power_save(BMI2_DISABLE, dev);
                }
            }

            if(rslt == BMI2_OK)
            {
                switch (sensor_data[loop].type)
                {
                    case BMI2_ACCEL:

                        /* Get accelerometer data */
                        rslt = get_accel_sensor_data(&sensor_data[loop].sens_data.acc, BMI2_ACC_X_LSB_ADDR, dev);
                        break;
                    case BMI2_GYRO:

                        /* Get gyroscope data */
                        rslt = get_gyro_sensor_data(&sensor_data[loop].sens_data.gyr, BMI2_GYR_X_LSB_ADDR, dev);
                        break;
                    case BMI2_AUX:

                        /* Get auxiliary sensor data in data mode */
                        rslt = read_aux_data_mode(sensor_data[loop].sens_data.aux_data, dev);
                        break;

                    case BMI2_GYRO_CROSS_SENSE:

                        /* Get Gyroscope cross sense value of z axis */
                        rslt = get_gyro_cross_sense(&sensor_data[loop].sens_data.correction_factor_zx, dev);
                        break;

                    case BMI2_GYRO_GAIN_UPDATE:

                        /* Get saturation status of gyroscope user gain update  */
                        rslt = get_gyro_gain_update_status(&sensor_data[loop].sens_data.gyro_user_gain_status, dev);
                        break;
                    default:
                        rslt = BMI2_E_INVALID_SENSOR;
                        break;
                }

                /* Return error if any of the get sensor data fails */
                if(rslt != BMI2_OK)
                {
                    break;
                }
            }

            /* Enable Advance power save if disabled while
             * configuring and not when already disabled
             */
            if((aps_stat == BMI2_ENABLE) && (rslt == BMI2_OK))
            {
                rslt = bmi2_set_adv_power_save(BMI2_ENABLE, dev);
            }
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_set_fifo_config(XWORD config, XBYTE enable, struct bmi2_dev *dev)
* @brief      bmi2setfifoconfig
* @ingroup    DATAIO
* 
* @param[in]  config : Configuration object to use.
* @param[in]  enable : Enable value.
* @param[in]  dev : Dev pointer to use.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_set_fifo_config(XWORD config, XBYTE enable, struct bmi2_dev *dev)
{
    XBYTESIG rslt;
    XBYTE data[2] = { 0 };
    XBYTE max_burst_len = 0;

    /* Variable to store data of FIFO configuration register 0 */
    XBYTE fifo_config_0 = (XBYTE)(config & BMI2_FIFO_CONFIG_0_MASK);

    /* Variable to store data of FIFO configuration register 1 */
    XBYTE fifo_config_1 = (XBYTE)((config & BMI2_FIFO_CONFIG_1_MASK) >> 8);

    rslt = null_ptr_check(dev);
    if(rslt == BMI2_OK)
    {
        rslt = bmi2_get_regs(BMI2_FIFO_CONFIG_0_ADDR, data, BMI2_FIFO_CONFIG_LENGTH, dev);
        if(rslt == BMI2_OK)
        {
            /* Get data to set FIFO configuration register 0 */
            if(fifo_config_0 > 0)
            {
                if(enable == BMI2_ENABLE)
                {
                    data[0] = data[0] | fifo_config_0;
                }
                else
                {
                    data[0] = data[0] & (~fifo_config_0);
                }
            }

            /* Get data to set FIFO configuration register 1 */
            if(enable == BMI2_ENABLE)
            {
                data[1] = data[1] | fifo_config_1;
                if(dev->variant_feature & BMI2_CRT_RTOSK_ENABLE)
                {

                    /* Burst length is needed for CRT
                     *  FIFO enable will reset the default values
                     *  So configure the max burst length again.
                     */
                    rslt = get_maxburst_len(&max_burst_len, dev);
                    if(rslt == BMI2_OK && max_burst_len == 0)
                    {
                        rslt = set_maxburst_len(BMI2_CRT_MIN_BURST_WORD_LENGTH, dev);
                    }
                }
            }
            else
            {
                data[1] = data[1] & (~fifo_config_1);
            }

            /* Set the FIFO configurations */
            if(rslt == BMI2_OK)
            {
                rslt = bmi2_set_regs(BMI2_FIFO_CONFIG_0_ADDR, data, 2, dev);
            }
        }
    }

    return rslt;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_get_fifo_config(XWORD *fifo_config, struct bmi2_dev *dev)
* @brief      bmi2getfifoconfig
* @ingroup    DATAIO
* 
* @param[in]  fifo_config : Fifo config pointer to use.
* @param[in]  dev : Dev pointer to use.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_get_fifo_config(XWORD *fifo_config, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Array to store data */
    XBYTE data[2] = { 0 };

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if((rslt == BMI2_OK) && (fifo_config != NULL))
    {
        /* Get the FIFO configuration value */
        rslt = bmi2_get_regs(BMI2_FIFO_CONFIG_0_ADDR, data, BMI2_FIFO_CONFIG_LENGTH, dev);
        if(rslt == BMI2_OK)
        {
            (*fifo_config) = (XWORD)((XWORD) data[0] & BMI2_FIFO_CONFIG_0_MASK);
            (*fifo_config) |= (XWORD)(((XWORD) data[1] << 8) & BMI2_FIFO_CONFIG_1_MASK);
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_read_fifo_data(struct bmi2_fifo_frame *fifo, struct bmi2_dev *dev)
* @brief      bmi2readfifodata
* @ingroup    DATAIO
* 
* @param[in]  fifo : Fifo pointer to use.
* @param[in]  dev : Dev pointer to use.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_read_fifo_data(struct bmi2_fifo_frame *fifo, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Array to store FIFO configuration data */
    XBYTE config_data[2] = { 0 };

    /* Variable to define FIFO address */
    XBYTE addr = BMI2_FIFO_DATA_ADDR;

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if((rslt == BMI2_OK) && (fifo != NULL))
    {
        /* Clear the FIFO data structure */
        reset_fifo_frame_structure(fifo, dev);

        /* Read FIFO data */
        rslt = bmi2_get_regs(addr, fifo->data, fifo->length, dev);

        if(rslt == BMI2_OK)
        {

            /* Get the set FIFO frame configurations */
            rslt = bmi2_get_regs(BMI2_FIFO_CONFIG_0_ADDR, config_data, 2, dev);
            if(rslt == BMI2_OK)
            {
                /* Get FIFO header status */
                fifo->header_enable = (XBYTE)((config_data[1]) & (BMI2_FIFO_HEADER_EN >> 8));

                /* Get sensor enable status, of which the data is to be read */
                fifo->data_enable =
                    (XWORD)(((config_data[0]) | ((XWORD) config_data[1] << 8)) & BMI2_FIFO_ALL_EN);
            }
        }
        else
        {
            rslt = BMI2_E_COM_FAIL;
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_extract_accel(struct bmi2_sens_axes_data* accel_data, XWORD* accel_length, struct bmi2_fifo_frame* fifo, const struct bmi2_dev* dev)
* @brief      bmi2extractaccel read by the "bmi2_read_fifo_data" API and stores it in the "accel_data" structure instance.
* @ingroup    DATAIO
* 
* @param[in]  accel_data : Accel data pointer to use.
* @param[in]  accel_length : Accel length pointer to use.
* @param[in]  fifo : Fifo pointer to use.
* @param[in]  dev : Dev value.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_extract_accel(struct bmi2_sens_axes_data* accel_data, XWORD* accel_length, struct bmi2_fifo_frame* fifo, const struct bmi2_dev* dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to index the bytes */
    XWORD data_index = 0;

    /* Variable to index accelerometer frames */
    XWORD accel_index = 0;

    /* Variable to store the number of bytes to be read */
    XWORD data_read_length = 0;

    /* Variable to define the data enable byte */
    XBYTE data_enable = 0;

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if((rslt == BMI2_OK) && (accel_data != NULL) && (accel_length != NULL) && (fifo != NULL))
    {
        /* Parsing the FIFO data in header-less mode */
        if(fifo->header_enable == 0)
        {

            /* Get the number of accelerometer bytes to be read */
            rslt = parse_fifo_accel_len(&data_index, &data_read_length, accel_length, fifo);

            /* Convert word to byte since all sensor enables are in a byte */
            data_enable = (XBYTE)(fifo->data_enable >> 8);
            for (; (data_index < data_read_length) && (rslt != BMI2_W_FIFO_EMPTY);)
            {
                /* Unpack frame to get the accelerometer data */
                rslt = unpack_accel_frame(accel_data, &data_index, &accel_index, data_enable, fifo, dev);

                if(rslt != BMI2_W_FIFO_EMPTY)
                {
                    /* Check for the availability of next two bytes of FIFO data */
                    rslt = check_empty_fifo(&data_index, fifo);
                }
            }

            /* Update number of accelerometer frames to be read */
            (*accel_length) = accel_index;

            /* Update the accelerometer byte index */
            fifo->acc_byte_start_idx = data_index;
        }
        else
        {
            /* Parsing the FIFO data in header mode */
            rslt = extract_accel_header_mode(accel_data, accel_length, fifo, dev);
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_extract_gyro(struct bmi2_sens_axes_data *gyro_data, XWORD *gyro_length, struct bmi2_fifo_frame *fifo, const struct bmi2_dev *dev)
* @brief      bmi2extractgyro read by the "bmi2_read_fifo_data" API and stores it in the "gyro_data" structure instance.
* @ingroup    DATAIO
* 
* @param[in]  gyro_data : Gyro data pointer to use.
* @param[in]  gyro_length : Gyro length pointer to use.
* @param[in]  fifo : Fifo pointer to use.
* @param[in]  dev : Dev value.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_extract_gyro(struct bmi2_sens_axes_data *gyro_data, XWORD *gyro_length, struct bmi2_fifo_frame *fifo, const struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to index the bytes */
    XWORD data_index = 0;

    /* Variable to index gyroscope frames */
    XWORD gyro_index = 0;

    /* Variable to store the number of bytes to be read */
    XWORD data_read_length = 0;

    /* Variable to define the data enable byte */
    XBYTE data_enable = 0;

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if((rslt == BMI2_OK) && (gyro_data != NULL) && (gyro_length != NULL) && (fifo != NULL))
    {
        /* Parsing the FIFO data in header-less mode */
        if(fifo->header_enable == 0)
        {
            /* Get the number of gyro bytes to be read */
            rslt = parse_fifo_gyro_len(&data_index, &data_read_length, gyro_length, fifo);

            /* Convert word to byte since all sensor enables are in a byte */
            data_enable = (XBYTE)(fifo->data_enable >> 8);
            for (; (data_index < data_read_length) && (rslt != BMI2_W_FIFO_EMPTY);)
            {
                /* Unpack frame to get gyroscope data */
                rslt = unpack_gyro_frame(gyro_data, &data_index, &gyro_index, data_enable, fifo, dev);
                if(rslt != BMI2_W_FIFO_EMPTY)
                {
                    /* Check for the availability of next two bytes of FIFO data */
                    rslt = check_empty_fifo(&data_index, fifo);
                }
            }

            /* Update number of gyroscope frames to be read */
            (*gyro_length) = gyro_index;

            /* Update the gyroscope byte index */
            fifo->acc_byte_start_idx = data_index;
        }
        else
        {
            /* Parsing the FIFO data in header mode */
            rslt = extract_gyro_header_mode(gyro_data, gyro_length, fifo, dev);
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_extract_aux(struct bmi2_aux_fifo_data *aux, XWORD *aux_length, struct bmi2_fifo_frame *fifo, const struct bmi2_dev *dev)
* @brief      bmi2extractaux read by the "bmi2_read_fifo_data" API and stores it in "aux_data" buffer.
* @ingroup    DATAIO
* 
* @param[in]  aux : Aux pointer to use.
* @param[in]  aux_length : Aux length pointer to use.
* @param[in]  fifo : Fifo pointer to use.
* @param[in]  dev : Dev value.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_extract_aux(struct bmi2_aux_fifo_data *aux, XWORD *aux_length, struct bmi2_fifo_frame *fifo, const struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to index the bytes */
    XWORD data_index = 0;

    /* Variable to index auxiliary frames */
    XWORD aux_index = 0;

    /* Variable to store the number of bytes to be read */
    XWORD data_read_length = 0;

    /* Variable to define the data enable byte */
    XBYTE data_enable = 0;

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if((rslt == BMI2_OK) && (aux != NULL) && (aux_length != NULL) && (fifo != NULL))
    {
        /* Parsing the FIFO data in header-less mode */
        if(fifo->header_enable == 0)
        {
            rslt = parse_fifo_aux_len(&data_index, &data_read_length, aux_length, fifo);

            /* Convert word to byte since all sensor enables are in
             * a byte
             */
            data_enable = (XBYTE)(fifo->data_enable >> 8);
            for (; (data_index < data_read_length) && (rslt != BMI2_W_FIFO_EMPTY);)
            {
                /* Unpack frame to get auxiliary data */
                rslt = unpack_aux_frame(aux, &data_index, &aux_index, data_enable, fifo, dev);
                if(rslt != BMI2_W_FIFO_EMPTY)
                {
                    /* Check for the availability of next
                     * two bytes of FIFO data
                     */
                    rslt = check_empty_fifo(&data_index, fifo);
                }
            }

            /* Update number of auxiliary frames to be read */
            *aux_length = aux_index;

            /* Update the auxiliary byte index */
            fifo->aux_byte_start_idx = data_index;
        }
        else
        {
            /* Parsing the FIFO data in header mode */
            rslt = extract_aux_header_mode(aux, aux_length, fifo, dev);
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_set_command_register(XBYTE command, struct bmi2_dev *dev)
* @brief      bmi2setcommandregister
* @ingroup    DATAIO
* 
* @param[in]  command : Command value.
* @param[in]  dev : Dev pointer to use.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_set_command_register(XBYTE command, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if(rslt == BMI2_OK)
    {
        /* Set the command in the command register */
        rslt = bmi2_set_regs(BMI2_CMD_REG_ADDR, &command, 1, dev);
    }

    return rslt;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_set_fifo_self_wake_up(XBYTE fifo_self_wake_up, struct bmi2_dev *dev)
* @brief      bmi2setfifoselfwakeup
* @ingroup    DATAIO
* 
* @param[in]  fifo_self_wake_up : Fifo self wake up value.
* @param[in]  dev : Dev pointer to use.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_set_fifo_self_wake_up(XBYTE fifo_self_wake_up, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to store data */
    XBYTE data = 0;

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if(rslt == BMI2_OK)
    {
        /* Set FIFO self wake-up */
        rslt = bmi2_get_regs(BMI2_PWR_CONF_ADDR, &data, 1, dev);
        if(rslt == BMI2_OK)
        {
            data = BMI2_SET_BITS(data, BMI2_FIFO_SELF_WAKE_UP, fifo_self_wake_up);
            rslt = bmi2_set_regs(BMI2_PWR_CONF_ADDR, &data, 1, dev);
        }
    }

    return rslt;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_get_fifo_self_wake_up(XBYTE *fifo_self_wake_up, struct bmi2_dev *dev)
* @brief      bmi2getfifoselfwakeup
* @ingroup    DATAIO
* 
* @param[in]  fifo_self_wake_up : Fifo self wake up pointer to use.
* @param[in]  dev : Dev pointer to use.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_get_fifo_self_wake_up(XBYTE *fifo_self_wake_up, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to store data */
    XBYTE data = 0;

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if((rslt == BMI2_OK) && (fifo_self_wake_up != NULL))
    {
        /* Get the status of FIFO self wake-up */
        rslt = bmi2_get_regs(BMI2_PWR_CONF_ADDR, &data, 1, dev);
        if(rslt == BMI2_OK)
        {
            (*fifo_self_wake_up) = BMI2_GET_BITS(data, BMI2_FIFO_SELF_WAKE_UP);
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_set_fifo_wm(XWORD fifo_wm, struct bmi2_dev *dev)
* @brief      bmi2setfifowm
* @ingroup    DATAIO
* 
* @param[in]  fifo_wm : Fifo wm value.
* @param[in]  dev : Dev pointer to use.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_set_fifo_wm(XWORD fifo_wm, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Array to store data */
    XBYTE data[2] = { 0 };

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if(rslt == BMI2_OK)
    {
        /* Get LSB value of FIFO water-mark */
        data[0] = BMI2_GET_LSB(fifo_wm);

        /* Get MSB value of FIFO water-mark */
        data[1] = BMI2_GET_MSB(fifo_wm);

        /* Set the FIFO water-mark level */
        rslt = bmi2_set_regs(BMI2_FIFO_WTM_0_ADDR, data, 2, dev);
    }

    return rslt;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_get_fifo_wm(XWORD *fifo_wm, struct bmi2_dev *dev)
* @brief      bmi2getfifowm
* @ingroup    DATAIO
* 
* @param[in]  fifo_wm : Fifo wm pointer to use.
* @param[in]  dev : Dev pointer to use.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_get_fifo_wm(XWORD *fifo_wm, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Array to to store data */
    XBYTE data[2] = { 0 };

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if((rslt == BMI2_OK) && (fifo_wm != NULL))
    {
        /* Read the FIFO water mark level */
        rslt = bmi2_get_regs(BMI2_FIFO_WTM_0_ADDR, data, BMI2_FIFO_WM_LENGTH, dev);
        if(rslt == BMI2_OK)
        {
            (*fifo_wm) = (XWORD)((XWORD) data[1] << 8) | (data[0]);
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_set_fifo_filter_data(XBYTE sens_sel, XBYTE fifo_filter_data, struct bmi2_dev *dev)
* @brief      bmi2setfifofilterdata
* @ingroup    DATAIO
* 
* @param[in]  sens_sel : Sens sel value.
* @param[in]  fifo_filter_data : Fifo filter data value.
* @param[in]  dev : Dev pointer to use.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_set_fifo_filter_data(XBYTE sens_sel, XBYTE fifo_filter_data, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to store data */
    XBYTE data = 0;

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if(rslt == BMI2_OK)
    {
        switch (sens_sel)
        {
            case BMI2_ACCEL:

                /* Validate filter mode */
                if(fifo_filter_data <= BMI2_MAX_VALUE_FIFO_FILTER)
                {
                    /* Set the accelerometer FIFO filter data */
                    rslt = bmi2_get_regs(BMI2_FIFO_DOWNS_ADDR, &data, 1, dev);
                    if(rslt == BMI2_OK)
                    {
                        data = BMI2_SET_BITS(data, BMI2_ACC_FIFO_FILT_DATA, fifo_filter_data);
                        rslt = bmi2_set_regs(BMI2_FIFO_DOWNS_ADDR, &data, 1, dev);
                    }
                }
                else
                {
                    rslt = BMI2_E_OUT_OF_RANGE;
                }

                break;
            case BMI2_GYRO:

                /* Validate filter mode */
                if(fifo_filter_data <= BMI2_MAX_VALUE_FIFO_FILTER)
                {
                    /* Set the gyroscope FIFO filter data */
                    rslt = bmi2_get_regs(BMI2_FIFO_DOWNS_ADDR, &data, 1, dev);
                    if(rslt == BMI2_OK)
                    {
                        data = BMI2_SET_BITS(data, BMI2_GYR_FIFO_FILT_DATA, fifo_filter_data);
                        rslt = bmi2_set_regs(BMI2_FIFO_DOWNS_ADDR, &data, 1, dev);
                    }
                }
                else
                {
                    rslt = BMI2_E_OUT_OF_RANGE;
                }

                break;
            default:
                rslt = BMI2_E_INVALID_SENSOR;
                break;
        }
    }

    return rslt;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_get_fifo_filter_data(XBYTE sens_sel, XBYTE *fifo_filter_data, struct bmi2_dev *dev)
* @brief      bmi2getfifofilterdata
* @ingroup    DATAIO
* 
* @param[in]  sens_sel : Sens sel value.
* @param[in]  fifo_filter_data : Fifo filter data pointer to use.
* @param[in]  dev : Dev pointer to use.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_get_fifo_filter_data(XBYTE sens_sel, XBYTE *fifo_filter_data, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to store FIFO filter mode */
    XBYTE data = 0;

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if((rslt == BMI2_OK) && (fifo_filter_data != NULL))
    {
        switch (sens_sel)
        {
            case BMI2_ACCEL:

                /* Read the accelerometer FIFO filter data */
                rslt = bmi2_get_regs(BMI2_FIFO_DOWNS_ADDR, &data, 1, dev);
                if(rslt == BMI2_OK)
                {
                    (*fifo_filter_data) = BMI2_GET_BITS(data, BMI2_ACC_FIFO_FILT_DATA);
                }

                break;
            case BMI2_GYRO:

                /* Read the gyroscope FIFO filter data */
                rslt = bmi2_get_regs(BMI2_FIFO_DOWNS_ADDR, &data, 1, dev);
                if(rslt == BMI2_OK)
                {
                    (*fifo_filter_data) = BMI2_GET_BITS(data, BMI2_GYR_FIFO_FILT_DATA);
                }

                break;
            default:
                rslt = BMI2_E_INVALID_SENSOR;
                break;
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_set_fifo_down_sample(XBYTE sens_sel, XBYTE fifo_down_samp, struct bmi2_dev *dev)
* @brief      bmi2setfifodownsample
* @ingroup    DATAIO
* 
* @param[in]  sens_sel : Sens sel value.
* @param[in]  fifo_down_samp : Fifo down samp value.
* @param[in]  dev : Dev pointer to use.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_set_fifo_down_sample(XBYTE sens_sel, XBYTE fifo_down_samp, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to store sampling rate */
    XBYTE data = 0;

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if(rslt == BMI2_OK)
    {
        switch (sens_sel)
        {
            case BMI2_ACCEL:

                /* Set the accelerometer FIFO down sampling rate */
                rslt = bmi2_get_regs(BMI2_FIFO_DOWNS_ADDR, &data, 1, dev);
                if(rslt == BMI2_OK)
                {
                    data = BMI2_SET_BITS(data, BMI2_ACC_FIFO_DOWNS, fifo_down_samp);
                    rslt = bmi2_set_regs(BMI2_FIFO_DOWNS_ADDR, &data, 1, dev);
                }

                break;
            case BMI2_GYRO:

                /* Set the gyroscope FIFO down sampling rate */
                rslt = bmi2_get_regs(BMI2_FIFO_DOWNS_ADDR, &data, 1, dev);
                if(rslt == BMI2_OK)
                {
                    data = BMI2_SET_BIT_POS0(data, BMI2_GYR_FIFO_DOWNS, fifo_down_samp);
                    rslt = bmi2_set_regs(BMI2_FIFO_DOWNS_ADDR, &data, 1, dev);
                }

                break;
            default:
                rslt = BMI2_E_INVALID_SENSOR;
                break;
        }
    }

    return rslt;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_get_fifo_down_sample(XBYTE sens_sel, XBYTE *fifo_down_samp, struct bmi2_dev *dev)
* @brief      bmi2getfifodownsample accelerometer or gyroscope FIFO data.
* @ingroup    DATAIO
* 
* @param[in]  sens_sel : Sens sel value.
* @param[in]  fifo_down_samp : Fifo down samp pointer to use.
* @param[in]  dev : Dev pointer to use.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_get_fifo_down_sample(XBYTE sens_sel, XBYTE *fifo_down_samp, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to store sampling rate */
    XBYTE data = 0;

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if((rslt == BMI2_OK) && (fifo_down_samp != NULL))
    {
        switch (sens_sel)
        {
            case BMI2_ACCEL:

                /* Read the accelerometer FIFO down data sampling rate */
                rslt = bmi2_get_regs(BMI2_FIFO_DOWNS_ADDR, &data, 1, dev);
                if(rslt == BMI2_OK)
                {
                    (*fifo_down_samp) = BMI2_GET_BITS(data, BMI2_ACC_FIFO_DOWNS);
                }

                break;
            case BMI2_GYRO:

                /* Read the gyroscope FIFO down data sampling rate */
                rslt = bmi2_get_regs(BMI2_FIFO_DOWNS_ADDR, &data, 1, dev);
                if(rslt == BMI2_OK)
                {
                    (*fifo_down_samp) = BMI2_GET_BIT_POS0(data, BMI2_GYR_FIFO_DOWNS);
                }

                break;
            default:
                rslt = BMI2_E_INVALID_SENSOR;
                break;
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_get_fifo_length(XWORD *fifo_length, struct bmi2_dev *dev)
* @brief      bmi2getfifolength
* @ingroup    DATAIO
* 
* @param[in]  fifo_length : Fifo length pointer to use.
* @param[in]  dev : Dev pointer to use.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_get_fifo_length(XWORD *fifo_length, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to define byte index */
    XBYTE index = 0;

    /* Array to store FIFO data length */
    XBYTE data[BMI2_FIFO_DATA_LENGTH] = { 0 };

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if((rslt == BMI2_OK) && (fifo_length != NULL))
    {
        /* Read FIFO length */
        rslt = bmi2_get_regs(BMI2_FIFO_LENGTH_0_ADDR, data, BMI2_FIFO_DATA_LENGTH, dev);
        if(rslt == BMI2_OK)
        {
            /* Get the MSB byte index */
            index = BMI2_FIFO_LENGTH_MSB_BYTE;

            /* Get the MSB byte of FIFO length */
            data[index] = BMI2_GET_BIT_POS0(data[index], BMI2_FIFO_BYTE_COUNTER_MSB);

            /* Get total FIFO length */
            (*fifo_length) = ((data[index] << 8) | data[index - 1]);
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_read_aux_man_mode(XBYTE reg_addr, XBYTE *aux_data, XWORD len, struct bmi2_dev *dev)
* @brief      bmi2readauxmanmode address of auxiliary sensor in manual mode.
* @note       Change of BMI2_AUX_RD_ADDR is only allowed if AUX is not busy.
* @ingroup    DATAIO
* 
* @param[in]  reg_addr : Reg addr value.
* @param[in]  aux_data : Aux data pointer to use.
* @param[in]  len : Len value.
* @param[in]  dev : Dev pointer to use.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_read_aux_man_mode(XBYTE reg_addr, XBYTE *aux_data, XWORD len, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to store burst length */
    XBYTE burst_len = 0;

    /* Variable to define APS status */
    XBYTE aps_stat = 0;

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if((rslt == BMI2_OK) && (aux_data != NULL))
    {
        /* Validate if manual mode */
        if(dev->aux_man_en)
        {
            /* Get status of advance power save mode */
            aps_stat = dev->aps_status;
            if(aps_stat == BMI2_ENABLE)
            {
                /* Disable APS if enabled */
                rslt = bmi2_set_adv_power_save(BMI2_DISABLE, dev);
            }

            if(rslt == BMI2_OK)
            {
                /* Map the register value set to that of burst
                 * length
                 */
                rslt = map_read_len(&burst_len, dev);
                if(rslt == BMI2_OK)
                {
                    /* Read auxiliary data */
                    rslt = read_aux_data(reg_addr, aux_data, len, burst_len, dev);
                }
            }

            /* Enable Advance power save if disabled for reading
             * data and not when already disabled
             */
            if((rslt == BMI2_OK) && (aps_stat == BMI2_ENABLE))
            {
                rslt = bmi2_set_adv_power_save(BMI2_ENABLE, dev);
            }
        }
        else
        {
            rslt = BMI2_E_AUX_INVALID_CFG;
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_write_aux_man_mode(XBYTE reg_addr, const XBYTE *aux_data, XWORD len, struct bmi2_dev *dev)
* @brief      bmi2writeauxmanmode auxiliary sensor where data is to be written in manual mode.
* @note       Change of BMI2_AUX_WR_ADDR is only allowed if AUX is not busy.
* @ingroup    DATAIO
* 
* @param[in]  reg_addr : Reg addr value.
* @param[in]  aux_data : Aux data value.
* @param[in]  len : Len value.
* @param[in]  dev : Dev pointer to use.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_write_aux_man_mode(XBYTE reg_addr, const XBYTE *aux_data, XWORD len, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to define loop */
    XBYTE loop = 0;

    /* Variable to define APS status */
    XBYTE aps_stat = 0;

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if((rslt == BMI2_OK) && (aux_data != NULL))
    {
        /* Validate if manual mode */
        if(dev->aux_man_en)
        {
            /* Get status of advance power save mode */
            aps_stat = dev->aps_status;
            if(aps_stat == BMI2_ENABLE)
            {
                /* Disable APS if enabled */
                rslt = bmi2_set_adv_power_save(BMI2_DISABLE, dev);
            }

            /* Byte write data in the corresponding address */
            if(rslt == BMI2_OK)
            {
                for (; ((loop < len) && (rslt == BMI2_OK)); loop++)
                {
                    rslt = write_aux_data((reg_addr + loop), aux_data[loop], dev);
                    //dev->delay_us(1000, dev->intf_ptr);
                    bmi2_delay_us(1000, dev->intf_ptr);
                }
            }

            /* Enable Advance power save if disabled for writing
             * data and not when already disabled
             */
            if((rslt == BMI2_OK) && (aps_stat == BMI2_ENABLE))
            {
                rslt = bmi2_set_adv_power_save(BMI2_ENABLE, dev);
            }
        }
        else
        {
            rslt = BMI2_E_AUX_INVALID_CFG;
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_write_aux_interleaved(XBYTE reg_addr, const XBYTE *aux_data, XWORD len, struct bmi2_dev *dev)
* @brief      bmi2writeauxinterleaved auxiliary sensor where data is to be written, from an interleaved input, in manual mode.
* @note       Change of BMI2_AUX_WR_ADDR is only allowed if AUX is not busy.
* @ingroup    DATAIO
* 
* @param[in]  reg_addr : Reg addr value.
* @param[in]  aux_data : Aux data value.
* @param[in]  len : Len value.
* @param[in]  dev : Dev pointer to use.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_write_aux_interleaved(XBYTE reg_addr, const XBYTE *aux_data, XWORD len, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to define loop */
    XBYTE loop = 1;

    /* Variable to define APS status */
    XBYTE aps_stat = 0;

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if((rslt == BMI2_OK) && (aux_data != NULL))
    {
        /* Validate if manual mode */
        if(dev->aux_man_en)
        {
            /* Get status of advance power save mode */
            aps_stat = dev->aps_status;
            if(aps_stat == BMI2_ENABLE)
            {
                /* Disable APS if enabled */
                rslt = bmi2_set_adv_power_save(BMI2_DISABLE, dev);
            }

            if(rslt == BMI2_OK)
            {
                /* Write the start register address extracted
                 * from the interleaved data
                 */
                rslt = write_aux_data(reg_addr, aux_data[0], dev);

                /* Extract the remaining address and data from
                 * the interleaved data and write it in the
                 * corresponding addresses byte by byte
                 */
                for (; ((loop < len) && (rslt == BMI2_OK)); loop += 2)
                {
                    rslt = write_aux_data(aux_data[loop], aux_data[loop + 1], dev);
                    //dev->delay_us(1000, dev->intf_ptr);
                    bmi2_delay_us(1000, dev->intf_ptr);
                }

                /* Enable Advance power save if disabled for
                 * writing data and not when already disabled
                 */
                if((rslt == BMI2_OK) && (aps_stat == BMI2_ENABLE))
                {
                    rslt = bmi2_set_adv_power_save(BMI2_ENABLE, dev);
                }
            }
        }
        else
        {
            rslt = BMI2_E_AUX_INVALID_CFG;
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_get_status(XBYTE *status, struct bmi2_dev *dev)
* @brief      bmi2getstatus auxiliary, command decoder and busy status of auxiliary.
* @ingroup    DATAIO
* 
* @param[in]  status : Status value to read or update.
* @param[in]  dev : Dev pointer to use.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_get_status(XBYTE *status, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if((rslt == BMI2_OK) && (status != NULL))
    {
        rslt = bmi2_get_regs(BMI2_STATUS_ADDR, status, 1, dev);
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_set_ois_interface(XBYTE enable, struct bmi2_dev *dev)
* @brief      bmi2setoisinterface
* @ingroup    DATAIO
* 
* @param[in]  enable : Enable value.
* @param[in]  dev : Dev pointer to use.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_set_ois_interface(XBYTE enable, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to store data */
    XBYTE reg_data = 0;

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if(rslt == BMI2_OK)
    {
        rslt = bmi2_get_regs(BMI2_IF_CONF_ADDR, &reg_data, 1, dev);
        if(rslt == BMI2_OK)
        {
            /* Enable/Disable OIS interface */
            reg_data = BMI2_SET_BITS(reg_data, BMI2_OIS_IF_EN, enable);
            if(enable)
            {
                /* Disable auxiliary interface if OIS is enabled */
                reg_data = BMI2_SET_BIT_VAL0(reg_data, BMI2_AUX_IF_EN);
            }

            /* Set the OIS interface configurations */
            rslt = bmi2_set_regs(BMI2_IF_CONF_ADDR, &reg_data, 1, dev);
        }
    }

    return rslt;
}

/*!
 * @brief This API can be used to write sync commands like ODR, sync period,
 * frequency and phase, resolution ratio, sync time and delay time.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_write_sync_commands(const XBYTE *command, XBYTE n_comm, struct bmi2_dev *dev)
* @brief      Bmi2 write sync commands
* @ingroup    DATAIO
* 
* @param[in]  command : 
* @param[in]  n_comm : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_write_sync_commands(const XBYTE *command, XBYTE n_comm, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if((rslt == BMI2_OK) && (command != NULL))
    {
        rslt = bmi2_set_regs(BMI2_SYNC_COMMAND_ADDR, command, n_comm, dev);
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API performs self-test to check the proper functionality of the
 * accelerometer sensor.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_perform_accel_self_test(struct bmi2_dev *dev)
* @brief      Bmi2 perform accel self test
* @ingroup    DATAIO
* 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_perform_accel_self_test(struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to store self-test result */
    XBYTESIG st_rslt = 0;

    /* Structure to define positive accelerometer axes */
    struct bmi2_sens_axes_data positive = { 0, 0, 0, 0 };

    /* Structure to define negative accelerometer axes */
    struct bmi2_sens_axes_data negative = { 0, 0, 0, 0 };

    /* Structure for difference of accelerometer values in g */
    struct bmi2_selftest_delta_limit accel_data_diff = { 0, 0, 0 };

    /* Structure for difference of accelerometer values in mg */
    struct bmi2_selftest_delta_limit accel_data_diff_mg = { 0, 0, 0 };

    /* Initialize the polarity of self-test as positive */
    XBYTESIG sign = BMI2_ENABLE;

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if(rslt == BMI2_OK)
    {
        /* Sets the configuration required before enabling self-test */
        rslt = pre_self_test_config(dev);

        /* Wait for greater than 2 milliseconds */
        //dev->delay_us(3000, dev->intf_ptr);
        bmi2_delay_us(3000, dev->intf_ptr);
        
        if(rslt == BMI2_OK)
        {
            do
            {
                /* Select positive first, then negative polarity
                 * after enabling self-test
                 */
                rslt = self_test_config((XBYTE) sign, dev);
                if(rslt == BMI2_OK)
                {
                    /* Wait for greater than 50 milli-sec */
                    //dev->delay_us(51000, dev->intf_ptr);
                  bmi2_delay_us(51000, dev->intf_ptr);                        

                    /* If polarity is positive */
                    if(sign == BMI2_ENABLE)
                    {
                        /* Read and store positive acceleration value */
                        rslt = read_accel_xyz(&positive, dev);
                    }
                    /* If polarity is negative */
                    else if(sign == BMI2_DISABLE)
                    {
                        /* Read and store negative acceleration value */
                        rslt = read_accel_xyz(&negative, dev);
                    }
                }
                else
                {
                    /* Break if error */
                    break;
                }

                /* Break if error */
                if(rslt != BMI2_OK)
                {
                    break;
                }

                /* Turn the polarity of self-test negative */
                sign--;
            } while (sign >= 0);
            if(rslt == BMI2_OK)
            {
                /* Subtract -ve acceleration values from that of +ve values */
                accel_data_diff.x = (positive.x) - (negative.x);
                accel_data_diff.y = (positive.y) - (negative.y);
                accel_data_diff.z = (positive.z) - (negative.z);

                /* Convert differences of acceleration values
                 * from 'g' to 'mg'
                 */
                convert_lsb_g(&accel_data_diff, &accel_data_diff_mg, dev);

                /* Validate self-test for acceleration values
                 * in mg and get the self-test result
                 */
                st_rslt = validate_self_test(&accel_data_diff_mg);

                /* Trigger a soft reset after performing self-test */
                rslt = bmi2_soft_reset(dev);

                /* Return the self-test result */
                if(rslt == BMI2_OK)
                {
                    rslt = st_rslt;
                }
            }
        }
    }

    return rslt;
}

/*!
 * @brief This API maps/unmaps feature interrupts to that of interrupt pins.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_map_feat_int(XBYTE type, enum bmi2_hw_int_pin hw_int_pin, struct bmi2_dev *dev)
* @brief      Bmi2 map feat int
* @ingroup    DATAIO
* 
* @param[in]  type : 
* @param[in]  hw_int_pin : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_map_feat_int(XBYTE type, enum bmi2_hw_int_pin hw_int_pin, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to define the value of feature interrupts */
    XBYTE feat_int = 0;

    /* Array to store the interrupt mask bits */
    XBYTE data_array[2] = { 0 };

    /* Structure to define map the interrupts */
    struct bmi2_map_int map_int = { 0 };

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if(rslt == BMI2_OK)
    {
        /* Read interrupt map1 and map2 and register */
        rslt = bmi2_get_regs(BMI2_INT1_MAP_FEAT_ADDR, data_array, 2, dev);

        if(rslt == BMI2_OK)
        {
            /* Get the value of the feature interrupt to be mapped */
            extract_feat_int_map(&map_int, type, dev);

            feat_int = map_int.sens_map_int;

            /* Map the interrupts */
            rslt = map_feat_int(data_array, hw_int_pin, feat_int);

            /* Map the interrupts to INT1 and INT2 map register */
            if(rslt == BMI2_OK)
            {
                rslt = bmi2_set_regs(BMI2_INT1_MAP_FEAT_ADDR, &data_array[0], 1, dev);
                if(rslt == BMI2_OK)
                {
                    rslt = bmi2_set_regs(BMI2_INT2_MAP_FEAT_ADDR, &data_array[1], 1, dev);
                }
            }
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API maps/un-maps data interrupts to that of interrupt pins.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_map_data_int(XBYTE data_int, enum bmi2_hw_int_pin int_pin, struct bmi2_dev *dev)
* @brief      Bmi2 map data int
* @ingroup    DATAIO
* 
* @param[in]  data_int : 
* @param[in]  int_pin : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_map_data_int(XBYTE data_int, enum bmi2_hw_int_pin int_pin, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to mask interrupt pin 1 - lower nibble */
    XBYTE int1_mask = data_int;

    /* Variable to mask interrupt pin 2 - higher nibble */
    XBYTE int2_mask = (XBYTE)(data_int << 4);

    /* Variable to store register data */
    XBYTE reg_data = 0;

    /* Read interrupt map1 and map2 and register */
    rslt = bmi2_get_regs(BMI2_INT_MAP_DATA_ADDR, &reg_data, 1, dev);
    if(rslt == BMI2_OK)
    {
        if(int_pin < BMI2_INT_PIN_MAX)
        {
            switch (int_pin)
            {
                case BMI2_INT_NONE:

                    /* Un-Map the corresponding data
                     * interrupt to both interrupt pin 1 and 2
                     */
                    reg_data &= ~(int1_mask | int2_mask);
                    break;
                case BMI2_INT1:

                    /* Map the corresponding data interrupt to
                     * interrupt pin 1
                     */
                    reg_data |= int1_mask;
                    break;
                case BMI2_INT2:

                    /* Map the corresponding data interrupt to
                     * interrupt pin 2
                     */
                    reg_data |= int2_mask;
                    break;
                case BMI2_INT_BOTH:

                    /* Map the corresponding data
                     * interrupt to both interrupt pin 1 and 2
                     */
                    reg_data |= (int1_mask | int2_mask);
                    break;
                default:
                    break;
            }

            /* Set the interrupts in the map register */
            rslt = bmi2_set_regs(BMI2_INT_MAP_DATA_ADDR, &reg_data, 1, dev);
        }
        else
        {
            /* Return error if invalid pin selection */
            rslt = BMI2_E_INVALID_INT_PIN;
        }
    }

    return rslt;
}

/*!
 * @brief This API gets the re-mapped x, y and z axes from the sensor and
 * updates the values in the device structure.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_get_remap_axes(struct bmi2_remap *remapped_axis, struct bmi2_dev *dev)
* @brief      Bmi2 get remap axes
* @ingroup    DATAIO
* 
* @param[in]  remapped_axis : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_get_remap_axes(struct bmi2_remap *remapped_axis, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Initialize the local structure for axis re-mapping */
    struct bmi2_axes_remap remap = { 0, 0, 0, 0, 0, 0 };

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if((rslt == BMI2_OK) && (remapped_axis != NULL))
    {
        /* Get the re-mapped axes from the sensor */
        rslt = get_remap_axes(&remap, dev);
        if(rslt == BMI2_OK)
        {
            /* Store the re-mapped x-axis value in device structure
             * and its user-value in the interface structure
             */
            switch (remap.x_axis)
            {
                case BMI2_MAP_X_AXIS:

                    /* If mapped to x-axis */
                    dev->remap.x_axis = BMI2_MAP_X_AXIS;
                    remapped_axis->x = BMI2_X;
                    break;
                case BMI2_MAP_Y_AXIS:

                    /* If mapped to y-axis */
                    dev->remap.x_axis = BMI2_MAP_Y_AXIS;
                    remapped_axis->x = BMI2_Y;
                    break;
                case BMI2_MAP_Z_AXIS:

                    /* If mapped to z-axis */
                    dev->remap.x_axis = BMI2_MAP_Z_AXIS;
                    remapped_axis->x = BMI2_Z;
                    break;
                default:
                    break;
            }

            /* Store the re-mapped x-axis sign in device structure
             * and its user-value in the interface structure
             */
            if(remap.x_axis_sign)
            {
                /* If x-axis is mapped to -ve sign */
                dev->remap.x_axis_sign = BMI2_NEG_SIGN;
                remapped_axis->x |= BMI2_AXIS_SIGN;
            }
            else
            {
                dev->remap.x_axis_sign = BMI2_POS_SIGN;
            }

            /* Store the re-mapped y-axis value in device structure
             *  and its user-value in the interface structure
             */
            switch (remap.y_axis)
            {
                case BMI2_MAP_X_AXIS:

                    /* If mapped to x-axis */
                    dev->remap.y_axis = BMI2_MAP_X_AXIS;
                    remapped_axis->y = BMI2_X;
                    break;
                case BMI2_MAP_Y_AXIS:

                    /* If mapped to y-axis */
                    dev->remap.y_axis = BMI2_MAP_Y_AXIS;
                    remapped_axis->y = BMI2_Y;
                    break;
                case BMI2_MAP_Z_AXIS:

                    /* If mapped to z-axis */
                    dev->remap.y_axis = BMI2_MAP_Z_AXIS;
                    remapped_axis->y = BMI2_Z;
                    break;
                default:
                    break;
            }

            /* Store the re-mapped y-axis sign in device structure
             * and its user-value in the interface structure
             */
            if(remap.y_axis_sign)
            {
                /* If y-axis is mapped to -ve sign */
                dev->remap.y_axis_sign = BMI2_NEG_SIGN;
                remapped_axis->y |= BMI2_AXIS_SIGN;
            }
            else
            {
                dev->remap.y_axis_sign = BMI2_POS_SIGN;
            }

            /* Store the re-mapped z-axis value in device structure
             *  and its user-value in the interface structure
             */
            switch (remap.z_axis)
            {
                case BMI2_MAP_X_AXIS:

                    /* If mapped to x-axis */
                    dev->remap.z_axis = BMI2_MAP_X_AXIS;
                    remapped_axis->z = BMI2_X;
                    break;
                case BMI2_MAP_Y_AXIS:

                    /* If mapped to y-axis */
                    dev->remap.z_axis = BMI2_MAP_Y_AXIS;
                    remapped_axis->z = BMI2_Y;
                    break;
                case BMI2_MAP_Z_AXIS:

                    /* If mapped to z-axis */
                    dev->remap.z_axis = BMI2_MAP_Z_AXIS;
                    remapped_axis->z = BMI2_Z;
                    break;
                default:
                    break;
            }

            /* Store the re-mapped z-axis sign in device structure
             * and its user-value in the interface structure
             */
            if(remap.z_axis_sign)
            {
                /* If z-axis is mapped to -ve sign */
                dev->remap.z_axis_sign = BMI2_NEG_SIGN;
                remapped_axis->z |= BMI2_AXIS_SIGN;
            }
            else
            {
                dev->remap.z_axis_sign = BMI2_POS_SIGN;
            }
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API sets the re-mapped x, y and z axes to the sensor and
 * updates the them in the device structure.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_set_remap_axes(const struct bmi2_remap *remapped_axis, struct bmi2_dev *dev)
* @brief      Bmi2 set remap axes
* @ingroup    DATAIO
* 
* @param[in]  remapped_axis : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_set_remap_axes(const struct bmi2_remap *remapped_axis, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to store all the re-mapped axes */
    XBYTE remap_axes = 0;

    /* Variable to store the re-mapped x-axes */
    XBYTE remap_x = 0;

    /* Variable to store the re-mapped y-axes */
    XBYTE remap_y = 0;

    /* Variable to store the re-mapped z-axes */
    XBYTE remap_z = 0;

    /* Initialize the local structure for axis re-mapping */
    struct bmi2_axes_remap remap = { 0, 0, 0, 0, 0, 0 };

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if((rslt == BMI2_OK) && (remapped_axis != NULL))
    {
        /* Check whether all the axes are re-mapped */
        remap_axes = remapped_axis->x | remapped_axis->y | remapped_axis->z;

        /* If all the axes are re-mapped */
        if((remap_axes & BMI2_AXIS_MASK) == BMI2_AXIS_MASK)
        {
            /* Get the re-mapped value of x, y and z axis */
            remap_x = remapped_axis->x & BMI2_AXIS_MASK;
            remap_y = remapped_axis->y & BMI2_AXIS_MASK;
            remap_z = remapped_axis->z & BMI2_AXIS_MASK;

            /* Store the value of re-mapped x-axis in both
             * device structure and the local structure
             */
            switch (remap_x)
            {
                case BMI2_X:

                    /* If mapped to x-axis */
                    dev->remap.x_axis = BMI2_MAP_X_AXIS;
                    remap.x_axis = BMI2_MAP_X_AXIS;
                    break;
                case BMI2_Y:

                    /* If mapped to y-axis */
                    dev->remap.x_axis = BMI2_MAP_Y_AXIS;
                    remap.x_axis = BMI2_MAP_Y_AXIS;
                    break;
                case BMI2_Z:

                    /* If mapped to z-axis */
                    dev->remap.x_axis = BMI2_MAP_Z_AXIS;
                    remap.x_axis = BMI2_MAP_Z_AXIS;
                    break;
                default:
                    break;
            }

            /* Store the re-mapped x-axis sign in the device
             * structure and its value in local structure
             */
            if(remapped_axis->x & BMI2_AXIS_SIGN)
            {
                /* If x-axis is mapped to -ve sign */
                dev->remap.x_axis_sign = BMI2_NEG_SIGN;
                remap.x_axis_sign = BMI2_MAP_NEGATIVE;
            }
            else
            {
                dev->remap.x_axis_sign = BMI2_POS_SIGN;
                remap.x_axis_sign = BMI2_MAP_POSITIVE;
            }

            /* Store the value of re-mapped y-axis in both
             * device structure and the local structure
             */
            switch (remap_y)
            {
                case BMI2_X:

                    /* If mapped to x-axis */
                    dev->remap.y_axis = BMI2_MAP_X_AXIS;
                    remap.y_axis = BMI2_MAP_X_AXIS;
                    break;
                case BMI2_Y:

                    /* If mapped to y-axis */
                    dev->remap.y_axis = BMI2_MAP_Y_AXIS;
                    remap.y_axis = BMI2_MAP_Y_AXIS;
                    break;
                case BMI2_Z:

                    /* If mapped to z-axis */
                    dev->remap.y_axis = BMI2_MAP_Z_AXIS;
                    remap.y_axis = BMI2_MAP_Z_AXIS;
                    break;
                default:
                    break;
            }

            /* Store the re-mapped y-axis sign in the device
             * structure and its value in local structure
             */
            if(remapped_axis->y & BMI2_AXIS_SIGN)
            {
                /* If y-axis is mapped to -ve sign */
                dev->remap.y_axis_sign = BMI2_NEG_SIGN;
                remap.y_axis_sign = BMI2_MAP_NEGATIVE;
            }
            else
            {
                dev->remap.y_axis_sign = BMI2_POS_SIGN;
                remap.y_axis_sign = BMI2_MAP_POSITIVE;
            }

            /* Store the value of re-mapped z-axis in both
             * device structure and the local structure
             */
            switch (remap_z)
            {
                case BMI2_X:

                    /* If mapped to x-axis */
                    dev->remap.z_axis = BMI2_MAP_X_AXIS;
                    remap.z_axis = BMI2_MAP_X_AXIS;
                    break;
                case BMI2_Y:

                    /* If mapped to y-axis */
                    dev->remap.z_axis = BMI2_MAP_Y_AXIS;
                    remap.z_axis = BMI2_MAP_Y_AXIS;
                    break;
                case BMI2_Z:

                    /* If mapped to z-axis */
                    dev->remap.z_axis = BMI2_MAP_Z_AXIS;
                    remap.z_axis = BMI2_MAP_Z_AXIS;
                    break;
                default:
                    break;
            }

            /* Store the re-mapped z-axis sign in the device
             * structure and its value in local structure
             */
            if(remapped_axis->z & BMI2_AXIS_SIGN)
            {
                /* If z-axis is mapped to -ve sign */
                dev->remap.z_axis_sign = BMI2_NEG_SIGN;
                remap.z_axis_sign = BMI2_MAP_NEGATIVE;
            }
            else
            {
                dev->remap.z_axis_sign = BMI2_POS_SIGN;
                remap.z_axis_sign = BMI2_MAP_POSITIVE;
            }

            /* Set the re-mapped axes in the sensor */
            rslt = set_remap_axes(&remap, dev);
        }
        else
        {
            rslt = BMI2_E_REMAP_ERROR;
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API enables/disables gyroscope offset compensation. It adds the
 * offsets defined in the offset register with gyroscope data.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_set_gyro_offset_comp(XBYTE enable, struct bmi2_dev *dev)
* @brief      Bmi2 set gyro offset comp
* @ingroup    DATAIO
* 
* @param[in]  enable : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_set_gyro_offset_comp(XBYTE enable, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to define register data */
    XBYTE reg_data = 0;

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if(rslt == BMI2_OK)
    {
        /* Get the status of gyroscope offset enable */
        rslt = bmi2_get_regs(BMI2_GYR_OFF_COMP_6_ADDR, &reg_data, 1, dev);
        if(rslt == BMI2_OK)
        {
            reg_data = BMI2_SET_BITS(reg_data, BMI2_GYR_OFF_COMP_EN, enable);

            /* Enable/Disable gyroscope offset compensation */
            rslt = bmi2_set_regs(BMI2_GYR_OFF_COMP_6_ADDR, &reg_data, 1, dev);
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API reads the gyroscope bias values for each axis which is used
 * for gyroscope offset compensation.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_read_gyro_offset_comp_axes(struct bmi2_sens_axes_data *gyr_off_comp_axes, struct bmi2_dev *dev)
* @brief      Bmi2 read gyro offset comp axes
* @ingroup    DATAIO
* 
* @param[in]  gyr_off_comp_axes : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_read_gyro_offset_comp_axes(struct bmi2_sens_axes_data *gyr_off_comp_axes, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to define register data */
    XBYTE reg_data[4] = { 0 };

    /* Variable to store LSB value of offset compensation for x-axis */
    XBYTE gyr_off_lsb_x;

    /* Variable to store LSB value of offset compensation for y-axis */
    XBYTE gyr_off_lsb_y;

    /* Variable to store LSB value of offset compensation for z-axis */
    XBYTE gyr_off_lsb_z;

    /* Variable to store MSB value of offset compensation for x-axis */
    XBYTE gyr_off_msb_x;

    /* Variable to store MSB value of offset compensation for y-axis */
    XBYTE gyr_off_msb_y;

    /* Variable to store MSB value of offset compensation for z-axis */
    XBYTE gyr_off_msb_z;

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if((rslt == BMI2_OK) && (gyr_off_comp_axes != NULL))
    {
        /* Get the gyroscope compensated offset values */
        rslt = bmi2_get_regs(BMI2_GYR_OFF_COMP_3_ADDR, reg_data, 4, dev);
        if(rslt == BMI2_OK)
        {
            /* Get LSB and MSB values of offset compensation for
             * x, y and z axis
             */
            gyr_off_lsb_x = reg_data[0];
            gyr_off_lsb_y = reg_data[1];
            gyr_off_lsb_z = reg_data[2];
            gyr_off_msb_x = reg_data[3] & BMI2_GYR_OFF_COMP_MSB_X_MASK;
            gyr_off_msb_y = reg_data[3] & BMI2_GYR_OFF_COMP_MSB_Y_MASK;
            gyr_off_msb_z = reg_data[3] & BMI2_GYR_OFF_COMP_MSB_Z_MASK;

            /* Gyroscope offset compensation value for x-axis */
            gyr_off_comp_axes->x = (XWORDSIG)(((XWORD) gyr_off_msb_x << 8) | gyr_off_lsb_x);

            /* Gyroscope offset compensation value for y-axis */
            gyr_off_comp_axes->y = (XWORDSIG)(((XWORD) gyr_off_msb_y << 6) | gyr_off_lsb_y);

            /* Gyroscope offset compensation value for z-axis */
            gyr_off_comp_axes->z = (XWORDSIG)(((XWORD) gyr_off_msb_z << 4) | gyr_off_lsb_z);
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API writes the gyroscope bias values for each axis which is used
 * for gyroscope offset compensation.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_write_gyro_offset_comp_axes(const struct bmi2_sens_axes_data *gyr_off_comp_axes, struct bmi2_dev *dev)
* @brief      Bmi2 write gyro offset comp axes
* @ingroup    DATAIO
* 
* @param[in]  gyr_off_comp_axes : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_write_gyro_offset_comp_axes(const struct bmi2_sens_axes_data *gyr_off_comp_axes, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to define register data */
    XBYTE reg_data[4] = { 0 };

    /* Variable to store MSB value of offset compensation for x-axis */
    XBYTE gyr_off_msb_x;

    /* Variable to store MSB value of offset compensation for y-axis */
    XBYTE gyr_off_msb_y;

    /* Variable to store MSB value of offset compensation for z-axis */
    XBYTE gyr_off_msb_z;

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if((rslt == BMI2_OK) && (gyr_off_comp_axes != NULL))
    {
        /* Get the MSB values of gyroscope compensated offset values */
        rslt = bmi2_get_regs(BMI2_GYR_OFF_COMP_6_ADDR, &reg_data[3], 1, dev);
        if(rslt == BMI2_OK)
        {
            /* Get MSB value of x-axis from user-input */
            gyr_off_msb_x = (XBYTE)((gyr_off_comp_axes->x & BMI2_GYR_OFF_COMP_MSB_MASK) >> 8);

            /* Get MSB value of y-axis from user-input */
            gyr_off_msb_y = (XBYTE)((gyr_off_comp_axes->y & BMI2_GYR_OFF_COMP_MSB_MASK) >> 8);

            /* Get MSB value of z-axis from user-input */
            gyr_off_msb_z = (XBYTE)((gyr_off_comp_axes->z & BMI2_GYR_OFF_COMP_MSB_MASK) >> 8);

            /* Get LSB value of x-axis from user-input */
            reg_data[0] = (XBYTE)(gyr_off_comp_axes->x & BMI2_GYR_OFF_COMP_LSB_MASK);

            /* Get LSB value of y-axis from user-input */
            reg_data[1] = (XBYTE)(gyr_off_comp_axes->y & BMI2_GYR_OFF_COMP_LSB_MASK);

            /* Get LSB value of z-axis from user-input */
            reg_data[2] = (XBYTE)(gyr_off_comp_axes->z & BMI2_GYR_OFF_COMP_LSB_MASK);

            /* Get MSB value of x-axis to be set */
            reg_data[3] = BMI2_SET_BIT_POS0(reg_data[3], BMI2_GYR_OFF_COMP_MSB_X, gyr_off_msb_x);

            /* Get MSB value of y-axis to be set */
            reg_data[3] = BMI2_SET_BITS(reg_data[3], BMI2_GYR_OFF_COMP_MSB_Y, gyr_off_msb_y);

            /* Get MSB value of z-axis to be set */
            reg_data[3] = BMI2_SET_BITS(reg_data[3], BMI2_GYR_OFF_COMP_MSB_Z, gyr_off_msb_z);

            /* Set the offset compensation values of axes */
            rslt = bmi2_set_regs(BMI2_GYR_OFF_COMP_3_ADDR, reg_data, 4, dev);
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API updates the cross sensitivity coefficient between gyroscope's
 * X and Z axes.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_get_gyro_cross_sense(struct bmi2_dev *dev)
* @brief      Bmi2 get gyro cross sense
* @ingroup    DATAIO
* 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_get_gyro_cross_sense(struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt = BMI2_OK;
    struct bmi2_sensor_data data;

    /* Check if the feature is supported by this variant */
    if(dev->variant_feature & BMI2_GYRO_CROSS_SENS_ENABLE)
    {
        rslt = null_ptr_check(dev);
        if(rslt == BMI2_OK)
        {
            /* Select the feature whose data is to be acquired */
            data.type = BMI2_GYRO_CROSS_SENSE;

            /* Get the respective data */
            rslt = bmi2_get_sensor_data(&data, 1, dev);
            if(rslt == BMI2_OK)
            {
                /* Update the gyroscope cross sense value of z axis
                 * in the device structure
                 */
                dev->gyr_cross_sens_zx = data.sens_data.correction_factor_zx;
            }
        }
        else
        {
            rslt = BMI2_E_NULL_PTR;
        }
    }

    return rslt;
}

/*!
 * @brief This API gets Error bits and message indicating internal status.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_get_internal_status(XBYTE *int_stat, struct bmi2_dev *dev)
* @brief      Bmi2 get internal status
* @ingroup    DATAIO
* 
* @param[in]  int_stat : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_get_internal_status(XBYTE *int_stat, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if((rslt == BMI2_OK) && (int_stat != NULL))
    {
        /* Delay to read the internal status */
        bmi2_delay_us(20000, dev->intf_ptr);              

        /* Get the error bits and message */
        rslt = bmi2_get_regs(BMI2_INTERNAL_STATUS_ADDR, int_stat, 1, dev);
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}


/* Suppressing doxygen warnings triggered for same static function names present across various sensor variant
 * directories */

/*!
 * @brief This API verifies and allows only the correct position to do Fast Offset Compensation for
 * accelerometer & gyro.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::verify_foc_position(XBYTE sens_list, const struct bmi2_accel_foc_g_value *accel_g_axis, struct bmi2_dev *dev)
* @brief      Verify foc position
* @ingroup    DATAIO
* 
* @param[in]  sens_list : 
* @param[in]  accel_g_axis : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::verify_foc_position(XBYTE sens_list, const struct bmi2_accel_foc_g_value *accel_g_axis, struct bmi2_dev *dev)
{
    XBYTESIG rslt;

    struct bmi2_sens_axes_data avg_foc_data = { 0 };
    struct bmi2_foc_temp_value temp_foc_data = { 0 };

    rslt = null_ptr_check(dev);
    if(rslt == BMI2_OK)
    {
        /* Enable sensor */
        rslt = bmi2_sensor_enable(&sens_list, 1, dev);
    }

    if(rslt == BMI2_OK)
    {

        rslt = get_average_of_sensor_data(sens_list, &temp_foc_data, dev);
        if(rslt == BMI2_OK)
        {
            if(sens_list == BMI2_ACCEL)
            {

                /* Taking modulus to make negative values as positive */
                if((accel_g_axis->x == 1) && (accel_g_axis->sign == 1))
                {
                    temp_foc_data.x = temp_foc_data.x * -1;
                }
                else if((accel_g_axis->y == 1) && (accel_g_axis->sign == 1))
                {
                    temp_foc_data.y = temp_foc_data.y * -1;
                }
                else if((accel_g_axis->z == 1) && (accel_g_axis->sign == 1))
                {
                    temp_foc_data.z = temp_foc_data.z * -1;
                }
            }

            /* Typecasting into 16bit */
            avg_foc_data.x = (XWORDSIG)(temp_foc_data.x);
            avg_foc_data.y = (XWORDSIG)(temp_foc_data.y);
            avg_foc_data.z = (XWORDSIG)(temp_foc_data.z);

            rslt = validate_foc_position(sens_list, accel_g_axis, avg_foc_data, dev);
        }
    }

    return rslt;
}


/*!
 * @brief This API performs Fast Offset Compensation for accelerometer.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_perform_accel_foc(const struct bmi2_accel_foc_g_value *accel_g_value, struct bmi2_dev *dev)
* @brief      Bmi2 perform accel foc
* @ingroup    DATAIO
* 
* @param[in]  accel_g_value : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_perform_accel_foc(const struct bmi2_accel_foc_g_value *accel_g_value, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Structure to define the accelerometer configurations */
    struct bmi2_accel_config acc_cfg = { 0, 0, 0, 0 };

    /* Variable to store status of advance power save */
    XBYTE aps = 0;

    /* Variable to store status of accelerometer enable */
    XBYTE acc_en = 0;

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if((rslt == BMI2_OK) && (accel_g_value != NULL))
    {
        /* Check for input validity */
        if((((BMI2_ABS(accel_g_value->x)) + (BMI2_ABS(accel_g_value->y)) + (BMI2_ABS(accel_g_value->z))) == 1) &&
            ((accel_g_value->sign == 1) || (accel_g_value->sign == 0)))
        {
            rslt = verify_foc_position(BMI2_ACCEL, accel_g_value, dev);
            if(rslt == BMI2_OK)
            {

                /* Save accelerometer configurations, accelerometer
                 * enable status and advance power save status
                 */
                rslt = save_accel_foc_config(&acc_cfg, &aps, &acc_en, dev);
            }

            /* Set configurations for FOC */
            if(rslt == BMI2_OK)
            {
                rslt = set_accel_foc_config(dev);
            }

            /* Perform accelerometer FOC */
            if(rslt == BMI2_OK)
            {
                rslt = perform_accel_foc(accel_g_value, &acc_cfg, dev);
            }

            /* Restore the saved configurations */
            if(rslt == BMI2_OK)
            {
                rslt = restore_accel_foc_config(&acc_cfg, aps, acc_en, dev);
            }
        }
        else
        {
            rslt = BMI2_E_INVALID_INPUT;
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API performs Fast Offset Compensation for gyroscope.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_perform_gyro_foc(struct bmi2_dev *dev)
* @brief      Bmi2 perform gyro foc
* @ingroup    DATAIO
* 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_perform_gyro_foc(struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Structure to define the gyroscope configurations */
    struct bmi2_gyro_config gyr_cfg = { 0, 0, 0, 0, 0, 0 };

    /* Variable to store status of advance power save */
    XBYTE aps = 0;

    /* Variable to store status of gyroscope enable */
    XBYTE gyr_en = 0;

    /* Array of structure to store gyroscope data */
    struct bmi2_sens_axes_data gyr_value[128];

    /* Structure to store gyroscope data temporarily */
    struct bmi2_foc_temp_value temp = { 0, 0, 0 };

    /* Variable to store status read from the status register */
    XBYTE reg_status = 0;

    /* Variable to define count */
    XBYTE loop = 0;

    /* Structure to store the offset values to be stored in the register */
    struct bmi2_sens_axes_data gyro_offset = { 0, 0, 0, 0 };

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if(rslt == BMI2_OK)
    {
        /* Argument2 is not applicable for gyro */
        rslt = verify_foc_position(BMI2_GYRO, 0, dev);
        if(rslt == BMI2_OK)
        {
            /* Save gyroscope configurations, gyroscope enable
             * status and advance power save status
             */
            rslt = save_gyro_config(&gyr_cfg, &aps, &gyr_en, dev);

            /* Set configurations for gyroscope FOC */
            if(rslt == BMI2_OK)
            {
                rslt = set_gyro_foc_config(dev);
            }

            /* Perform FOC */
            if(rslt == BMI2_OK)
            {
                for (loop = 0; loop < 128; loop++)
                {
                    /* Giving a delay of more than 40ms since ODR is configured as 25Hz */
                    bmi2_delay_us(50000, dev->intf_ptr);                    

                    /* Get gyroscope data ready interrupt status */
                    rslt = bmi2_get_status(&reg_status, dev);

                    /* Read 128 samples of gyroscope data on data ready interrupt */
                    if((rslt == BMI2_OK) && (reg_status & BMI2_DRDY_GYR))
                    {
                        rslt = read_gyro_xyz(&gyr_value[loop], dev);
                        if(rslt == BMI2_OK)
                        {
                            /* Store the data in a temporary structure */
                            temp.x = temp.x + (XDWORDSIG)gyr_value[loop].x;
                            temp.y = temp.y + (XDWORDSIG)gyr_value[loop].y;
                            temp.z = temp.z + (XDWORDSIG)gyr_value[loop].z;
                        }
                    }

                    if(rslt != BMI2_OK)
                    {
                        break;
                    }
                    else if((reg_status & BMI2_DRDY_GYR) != BMI2_DRDY_GYR)
                    {
                        rslt = BMI2_E_INVALID_STATUS;
                        break;
                    }
                }

                if(rslt == BMI2_OK)
                {
                    /* Take average of x, y and z data for lesser
                     * noise. It is same as offset data since lsb/dps
                     * is same for both data and offset register
                     */
                    gyro_offset.x = (XWORDSIG)(temp.x / 128);
                    gyro_offset.y = (XWORDSIG)(temp.y / 128);
                    gyro_offset.z = (XWORDSIG)(temp.z / 128);

                    /* Saturate gyroscope data since the offset
                     * registers are of 10 bit value where as the
                     * gyroscope data is of 16 bit value
                     */
                    saturate_gyro_data(&gyro_offset);

                    /* Invert the gyroscope offset  data */
                    invert_gyro_offset(&gyro_offset);

                    /* Write offset data in the gyroscope offset
                     * compensation register
                     */
                    rslt = bmi2_write_gyro_offset_comp_axes(&gyro_offset, dev);
                }

                /* Enable gyroscope offset compensation */
                if(rslt == BMI2_OK)
                {
                    rslt = bmi2_set_gyro_offset_comp(BMI2_ENABLE, dev);
                }

                /* Restore the saved gyroscope configurations */
                if(rslt == BMI2_OK)
                {
                    rslt = restore_gyro_config(&gyr_cfg, aps, gyr_en, dev);
                }
            }
        }
    }

    return rslt;
}

/*!
 * @brief This API is used to get the feature configuration from the
 * selected page.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_get_feat_config(XBYTE sw_page, XBYTE *feat_config, struct bmi2_dev *dev)
* @brief      Bmi2 get feat config
* @ingroup    DATAIO
* 
* @param[in]  sw_page : 
* @param[in]  feat_config : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_get_feat_config(XBYTE sw_page, XBYTE *feat_config, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to define bytes remaining to read */
    XBYTE bytes_remain = BMI2_FEAT_SIZE_IN_BYTES;

    /* Variable to define the read-write length */
    XBYTE read_write_len = 0;

    /* Variable to define the feature configuration address */
    XBYTE addr = BMI2_FEATURES_REG_ADDR;

    /* Variable to define index */
    XBYTE index = 0;

    if((feat_config == NULL) || (dev == NULL))
    {
        rslt = BMI2_E_NULL_PTR;
    }
    else
    {
        /* Check whether the page is valid */
        if(sw_page < dev->page_max)
        {
            /* Switch page */
            rslt = bmi2_set_regs(BMI2_FEAT_PAGE_ADDR, &sw_page, 1, dev);

            /* If user length is less than feature length */
            if((rslt == BMI2_OK) && (dev->read_write_len < BMI2_FEAT_SIZE_IN_BYTES))
            {
                /* Read-write should be even */
                if((dev->read_write_len % 2) != 0)
                {
                    dev->read_write_len--;
                }

                while (bytes_remain > 0)
                {
                    if(bytes_remain >= dev->read_write_len)
                    {
                        /* Read from the page */
                        rslt = bmi2_get_regs(addr, &feat_config[index], dev->read_write_len, dev);

                        /* Update index */
                        index += (XBYTE) dev->read_write_len;

                        /* Update address */
                        addr += (XBYTE) dev->read_write_len;

                        /* Update read-write length */
                        read_write_len += (XBYTE) dev->read_write_len;
                    }
                    else
                    {
                        /* Read from the page */
                        rslt = bmi2_get_regs(addr, (XBYTE *) (feat_config + index), (XWORD) bytes_remain, dev);

                        /* Update read-write length */
                        read_write_len += bytes_remain;
                    }

                    /* Remaining bytes */
                    bytes_remain = BMI2_FEAT_SIZE_IN_BYTES - read_write_len;

                    if(rslt != BMI2_OK)
                    {
                        break;
                    }
                }
            }
            else if(rslt == BMI2_OK)
            {
                /* Get configuration from the page */
                rslt = bmi2_get_regs(BMI2_FEATURES_REG_ADDR, feat_config, BMI2_FEAT_SIZE_IN_BYTES, dev);
            }
        }
        else
        {
            rslt = BMI2_E_INVALID_PAGE;
        }
    }

    return rslt;
}

/*!
 * @brief This API is used to extract the input feature configuration
 * details from the look-up table.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOI2C6AXISTRACKINGBMI270::bmi2_extract_input_feat_config(struct bmi2_feature_config *feat_config, XBYTE type, const struct bmi2_dev *dev)
* @brief      Bmi2 extract input feat config
* @ingroup    DATAIO
* 
* @param[in]  feat_config : 
* @param[in]  type : 
* @param[in]  dev : 
* 
* @return     XBYTE : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOI2C6AXISTRACKINGBMI270::bmi2_extract_input_feat_config(struct bmi2_feature_config *feat_config, XBYTE type, const struct bmi2_dev *dev)
{
    /* Variable to define loop */
    XBYTE loop = 0;

    /* Variable to set flag */
    XBYTE feat_found = BMI2_FALSE;

    /* Search for the input feature from the input configuration array */
    while (loop < dev->input_sens)
    {
        if(dev->feat_config[loop].type == type)
        {
            *feat_config = dev->feat_config[loop];
            feat_found = BMI2_TRUE;
            break;
        }

        loop++;
    }

    /* Return flag */
    return feat_found;
}


/// @cond


/* Suppressing doxygen warnings triggered for same static function names present across various sensor variant
 * directories */

/*!
 * @brief This internal API writes the configuration file.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::write_config_file(struct bmi2_dev *dev)
* @brief      Write config file
* @ingroup    DATAIO
* 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::write_config_file(struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to update the configuration file index */
    XWORD index = 0;

    /* config file size */
    XWORD config_size = dev->config_size;

    /* Variable to get the remainder */
    XBYTE remain = (XBYTE)(config_size % dev->read_write_len);

    /* Variable to get the balance bytes */
    XWORD bal_byte = 0;

    /* Variable to define temporary read/write length */
    XWORD read_write_len = 0;

    /* Disable advanced power save mode */
    rslt = bmi2_set_adv_power_save(BMI2_DISABLE, dev);
    if(rslt == BMI2_OK)
    {
        /* Disable loading of the configuration */
        rslt = set_config_load(BMI2_DISABLE, dev);
        if(rslt == BMI2_OK)
        {
            if(!remain)
            {
                /* Write the configuration file */
                for (index = 0; (index < config_size) && (rslt == BMI2_OK); index += dev->read_write_len)
                {
                    rslt = upload_file((dev->config_file_ptr + index), index, dev->read_write_len, dev);
                }
            }
            else
            {
                /* Get the balance bytes */
                bal_byte = (XWORD) config_size - (XWORD) remain;

                /* Write the configuration file for the balancem bytes */
                for (index = 0; (index < bal_byte) && (rslt == BMI2_OK); index += dev->read_write_len)
                {
                    rslt = upload_file((dev->config_file_ptr + index), index, dev->read_write_len, dev);
                }

                if(rslt == BMI2_OK)
                {
                    /* Update length in a temporary variable */
                    read_write_len = dev->read_write_len;

                    /* Write the remaining bytes in 2 bytes length */
                    dev->read_write_len = 2;

                    /* Write the configuration file for the remaining bytes */
                    for (index = bal_byte;
                         (index < config_size) && (rslt == BMI2_OK);
                         index += dev->read_write_len)
                    {
                        rslt = upload_file((dev->config_file_ptr + index), index, dev->read_write_len, dev);
                    }

                    /* Restore the user set length back from the temporary variable */
                    dev->read_write_len = read_write_len;
                }
            }

            if(rslt == BMI2_OK)
            {
                /* Enable loading of the configuration */
                rslt = set_config_load(BMI2_ENABLE, dev);

                /* Wait till ASIC is initialized */
                bmi2_delay_us(150000, dev->intf_ptr);
                
                if(rslt == BMI2_OK)
                {
                    /* Enable advanced power save mode */
                    rslt = bmi2_set_adv_power_save(BMI2_ENABLE, dev);
                }
            }
        }
    }

    return rslt;
}

/*!
 * @brief This internal API enables/disables the loading of the configuration
 * file.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_config_load(XBYTE enable, struct bmi2_dev *dev)
* @brief      Set config load
* @ingroup    DATAIO
* 
* @param[in]  enable : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_config_load(XBYTE enable, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to store data */
    XBYTE reg_data = 0;

    rslt = bmi2_get_regs(BMI2_INIT_CTRL_ADDR, &reg_data, 1, dev);
    if(rslt == BMI2_OK)
    {
        reg_data = BMI2_SET_BIT_POS0(reg_data, BMI2_CONF_LOAD_EN, enable);
        rslt = bmi2_set_regs(BMI2_INIT_CTRL_ADDR, &reg_data, 1, dev);
    }

    return rslt;
}

/*!
 * @brief This internal API loads the configuration file.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::upload_file(const XBYTE *config_data, XWORD index, XWORD write_len, struct bmi2_dev *dev)
* @brief      Upload file
* @ingroup    DATAIO
* 
* @param[in]  config_data : 
* @param[in]  index : 
* @param[in]  write_len : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::upload_file(const XBYTE *config_data, XWORD index, XWORD write_len, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Array to store address */
    XBYTE addr_array[2] = { 0 };

    if(config_data != NULL)
    {
        /* Store 0 to 3 bits of address in first byte */
        addr_array[0] = (XBYTE)((index / 2) & 0x0F);

        /* Store 4 to 11 bits of address in the second byte */
        addr_array[1] = (XBYTE)((index / 2) >> 4);

        /* Write the 2 bytes of address in consecutive locations */
        rslt = bmi2_set_regs(BMI2_INIT_ADDR_0, addr_array, 2, dev);
        if(rslt == BMI2_OK)
        {
            /* Burst write configuration file data corresponding to user set length */
            rslt = bmi2_set_regs(BMI2_INIT_DATA_ADDR, (XBYTE *)config_data, write_len, dev);
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This internal API validates bandwidth and performance mode of the
 * accelerometer set by the user.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::validate_bw_perf_mode(XBYTE *bandwidth, XBYTE *perf_mode, struct bmi2_dev *dev)
* @brief      Validate bw perf mode
* @ingroup    DATAIO
* 
* @param[in]  bandwidth : 
* @param[in]  perf_mode : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::validate_bw_perf_mode(XBYTE *bandwidth, XBYTE *perf_mode, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Validate and auto-correct performance mode */
    rslt = check_boundary_val(perf_mode, BMI2_POWER_OPT_MODE, BMI2_PERF_OPT_MODE, dev);
    if(rslt == BMI2_OK)
    {
        /* Validate and auto-correct bandwidth parameter */
        if(*perf_mode == BMI2_PERF_OPT_MODE)
        {
            /* Validate for continuous filter mode */
            rslt = check_boundary_val(bandwidth, BMI2_ACC_OSR4_AVG1, BMI2_ACC_CIC_AVG8, dev);
        }
        else
        {
            /* Validate for CIC averaging mode */
            rslt = check_boundary_val(bandwidth, BMI2_ACC_OSR4_AVG1, BMI2_ACC_RES_AVG128, dev);
        }
    }

    return rslt;
}

/*!
 * @brief This internal API validates ODR and range of the accelerometer set by
 * the user.
 */
 XBYTESIG DIOI2C6AXISTRACKINGBMI270::validate_odr_range(XBYTE *odr, XBYTE *range, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Validate and auto correct ODR */
    rslt = check_boundary_val(odr, BMI2_ACC_ODR_0_78HZ, BMI2_ACC_ODR_1600HZ, dev);
    if(rslt == BMI2_OK)
    {
        /* Validate and auto correct Range */
        rslt = check_boundary_val(range, BMI2_ACC_RANGE_2G, BMI2_ACC_RANGE_16G, dev);
    }

    return rslt;
}

/*!
 * @brief This internal API validates bandwidth, performance mode, low power/
 * high performance mode, ODR, and range set by the user.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::validate_gyro_config(struct bmi2_gyro_config *config, struct bmi2_dev *dev)
* @brief      Validate gyro config
* @ingroup    DATAIO
* 
* @param[in]  config : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::validate_gyro_config(struct bmi2_gyro_config *config, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Validate and auto-correct performance mode */
    rslt = check_boundary_val(&config->filter_perf, BMI2_POWER_OPT_MODE, BMI2_PERF_OPT_MODE, dev);
    if(rslt == BMI2_OK)
    {
        /* Validate and auto-correct bandwidth parameter */
        rslt = check_boundary_val(&config->bwp, BMI2_GYR_OSR4_MODE, BMI2_GYR_CIC_MODE, dev);
        if(rslt == BMI2_OK)
        {
            /* Validate and auto-correct low power/high-performance parameter */
            rslt = check_boundary_val(&config->noise_perf, BMI2_POWER_OPT_MODE, BMI2_PERF_OPT_MODE, dev);
            if(rslt == BMI2_OK)
            {
                /* Validate and auto-correct ODR parameter */
                rslt = check_boundary_val(&config->odr, BMI2_GYR_ODR_25HZ, BMI2_GYR_ODR_3200HZ, dev);
                if(rslt == BMI2_OK)
                {
                    /* Validate and auto-correct OIS range */
                    rslt = check_boundary_val(&config->ois_range, BMI2_GYR_OIS_250, BMI2_GYR_OIS_2000, dev);
                    if(rslt == BMI2_OK)
                    {
                        /* Validate and auto-correct range parameter */
                        rslt = check_boundary_val(&config->range, BMI2_GYR_RANGE_2000, BMI2_GYR_RANGE_125, dev);
                    }
                }
            }
        }
    }

    return rslt;
}

/*!
 * @brief This internal API shows the error status when illegal sensor
 * configuration is set.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::cfg_error_status(struct bmi2_dev *dev)
* @brief      Cfg error status
* @ingroup    DATAIO
* 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::cfg_error_status(struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to store data */
    XBYTE reg_data;

    /* Get error status of the set sensor configuration */
    rslt = bmi2_get_regs(BMI2_EVENT_ADDR, &reg_data, 1, dev);
    if(rslt == BMI2_OK)
    {
        reg_data = BMI2_GET_BITS(reg_data, BMI2_EVENT_FLAG);
        switch (reg_data)
        {
            case BMI2_NO_ERROR:
                rslt = BMI2_OK;
                break;
            case BMI2_ACC_ERROR:
                rslt = BMI2_E_ACC_INVALID_CFG;
                break;
            case BMI2_GYR_ERROR:
                rslt = BMI2_E_GYRO_INVALID_CFG;
                break;
            case BMI2_ACC_GYR_ERROR:
                rslt = BMI2_E_ACC_GYR_INVALID_CFG;
                break;
            default:
                break;
        }
    }

    return rslt;
}

/*!
 * @brief This internal API:
 * 1) Enables/Disables auxiliary interface.
 * 2) Sets auxiliary interface configurations like I2C address, manual/auto
 * mode enable, manual burst read length, AUX burst read length and AUX read
 * address.
 * 3)It maps/un-maps data interrupts to that of hardware interrupt line.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_aux_config(struct bmi2_aux_config *config, struct bmi2_dev *dev)
* @brief      Set aux config
* @ingroup    DATAIO
* 
* @param[in]  config : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_aux_config(struct bmi2_aux_config *config, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Validate auxiliary configurations */
    rslt = validate_aux_config(config, dev);
    if(rslt == BMI2_OK)
    {
        /* Enable/Disable auxiliary interface */
        rslt = set_aux_interface(config, dev);
        if(rslt == BMI2_OK)
        {
            /* Set the auxiliary interface configurations */
            rslt = config_aux_interface(config, dev);
            if(rslt == BMI2_OK)
            {
                /* Set read out offset and ODR */
                rslt = config_aux(config, dev);
            }
        }
    }

    return rslt;
}

/*!
 * @brief This internal API sets gyroscope user-gain configurations like gain
 * update value for x, y and z-axis.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_gyro_user_gain_config(const struct bmi2_gyro_user_gain_config *config, struct bmi2_dev *dev)
* @brief      Set gyro user gain config
* @ingroup    DATAIO
* 
* @param[in]  config : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_gyro_user_gain_config(const struct bmi2_gyro_user_gain_config *config, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Array to define the feature configuration */
    XBYTE feat_config[BMI2_FEAT_SIZE_IN_BYTES] = { 0 };

    /* Variable to define the array offset */
    XBYTE idx = 0;

    /* Variable to define index */
    XBYTE index = 0;

    /* Variable to set flag */
    XBYTE feat_found;

    /* Initialize feature configuration for user-gain */
    struct bmi2_feature_config user_gain_config = { 0, 0, 0 };

    /* Copy the feature configuration address to a local pointer */
    XWORD *data_p = (XWORD *) (void *)feat_config;

    /* Search for user-gain feature and extract its configuration details */
    feat_found = bmi2_extract_input_feat_config(&user_gain_config, BMI2_GYRO_GAIN_UPDATE, dev);
    if(feat_found)
    {
        /* Get the configuration from the page where user-gain feature resides */
        rslt = bmi2_get_feat_config(user_gain_config.page, feat_config, dev);
        if(rslt == BMI2_OK)
        {
            /* Define the offset in bytes for user-gain select */
            idx = user_gain_config.start_addr;

            /* Get offset in words since all the features are set in words length */
            idx = idx / 2;

            /* Set ratio_x */
            *(data_p + idx) = BMI2_SET_BIT_POS0(*(data_p + idx), BMI2_GYR_USER_GAIN_RATIO_X, config->ratio_x);

            /* Increment offset by 1 word to set ratio_y */
            idx++;

            /* Set ratio_y */
            *(data_p + idx) = BMI2_SET_BIT_POS0(*(data_p + idx), BMI2_GYR_USER_GAIN_RATIO_Y, config->ratio_y);

            /* Increment offset by 1 word to set ratio_z */
            idx++;

            /* Set ratio_z */
            *(data_p + idx) = BMI2_SET_BIT_POS0(*(data_p + idx), BMI2_GYR_USER_GAIN_RATIO_Z, config->ratio_z);

            /* Increment offset by 1 more word to get the total length in words */
            idx++;

            /* Get total length in bytes to copy from local pointer to the array */
            idx = (XBYTE)(idx * 2) - user_gain_config.start_addr;

            /* Copy the bytes to be set back to the array */
            for (index = 0; index < idx; index++)
            {
                feat_config[user_gain_config.start_addr +
                            index] = *((XBYTE *) data_p + user_gain_config.start_addr + index);
            }

            /* Set the configuration back to the page */
            rslt = bmi2_set_regs(BMI2_FEATURES_REG_ADDR, feat_config, BMI2_FEAT_SIZE_IN_BYTES, dev);
        }
    }
    else
    {
        rslt = BMI2_E_INVALID_SENSOR;
    }

    return rslt;
}

/*!
 * @brief This internal API enables/disables auxiliary interface.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_aux_interface(const struct bmi2_aux_config *config, struct bmi2_dev *dev)
* @brief      Set aux interface
* @ingroup    DATAIO
* 
* @param[in]  config : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_aux_interface(const struct bmi2_aux_config *config, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to store data */
    XBYTE reg_data;

    rslt = bmi2_get_regs(BMI2_IF_CONF_ADDR, &reg_data, 1, dev);
    if(rslt == BMI2_OK)
    {
        reg_data = BMI2_SET_BITS(reg_data, BMI2_AUX_IF_EN, config->aux_en);

        /* Enable/Disable auxiliary interface */
        rslt = bmi2_set_regs(BMI2_IF_CONF_ADDR, &reg_data, 1, dev);
    }

    return rslt;
}

/*!
 * @brief This internal API sets auxiliary configurations like manual/auto mode
 * FCU write command enable and read burst length for both data and manual mode.
 *
 * @note Auxiliary sensor should not be busy when configuring aux_i2c_addr,
 * man_rd_burst_len, aux_rd_burst_len and aux_rd_addr.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::config_aux_interface(const struct bmi2_aux_config *config, struct bmi2_dev *dev)
* @brief      Config aux interface
* @ingroup    DATAIO
* 
* @param[in]  config : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::config_aux_interface(const struct bmi2_aux_config *config, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to store data */
    XBYTE reg_data[2] = { 0 };

    /* Variable to store status */
    XBYTE status = 0;

    /* Variable to define count */
    XBYTE count = 0;

    rslt = bmi2_get_regs(BMI2_AUX_DEV_ID_ADDR, reg_data, 2, dev);
    if(rslt == BMI2_OK)
    {
        /* Set I2C address for AUX sensor */
        reg_data[0] = BMI2_SET_BITS(reg_data[0], BMI2_AUX_SET_I2C_ADDR, config->i2c_device_addr);

        /* Set the AUX IF to either manual or auto mode */
        reg_data[1] = BMI2_SET_BITS(reg_data[1], BMI2_AUX_MAN_MODE_EN, config->manual_en);

        /* Enables FCU write command on AUX IF for auxiliary sensors that need a trigger */
        reg_data[1] = BMI2_SET_BITS(reg_data[1], BMI2_AUX_FCU_WR_EN, config->fcu_write_en);

        /* Set the burst read length for manual mode */
        reg_data[1] = BMI2_SET_BITS(reg_data[1], BMI2_AUX_MAN_READ_BURST, config->man_rd_burst);

        /* Set the burst read length for data mode */
        reg_data[1] = BMI2_SET_BIT_POS0(reg_data[1], BMI2_AUX_READ_BURST, config->aux_rd_burst);
        for (;;)
        {
            /* Check if auxiliary sensor is busy */
            rslt = bmi2_get_status(&status, dev);
            if((rslt == BMI2_OK) && (!(status & BMI2_AUX_BUSY)))
            {
                /* Set the configurations if AUX is not busy */
                rslt = bmi2_set_regs(BMI2_AUX_DEV_ID_ADDR, reg_data, 2, dev);
                bmi2_delay_us(1000, dev->intf_ptr);
                
                if(rslt == BMI2_OK)
                {
                    /* If data mode */
                    if(!config->manual_en)
                    {
                        /* Disable manual enable flag in device structure */
                        dev->aux_man_en = 0;

                        /* Set the read address of the AUX sensor */
                        rslt = bmi2_set_regs(BMI2_AUX_RD_ADDR, (XBYTE *) &config->read_addr, 1, dev);
                        bmi2_delay_us(1000, dev->intf_ptr);
                        
                    }
                    else
                    {
                        /* Enable manual enable flag in device structure */
                        dev->aux_man_en = 1;

                        /* Update manual read burst length in device structure */
                        dev->aux_man_rd_burst_len = config->man_rd_burst;
                    }
                }

                /* Break after setting the register */
                break;
            }

            /* Increment count after every 10 seconds */
            bmi2_delay_us(10000, dev->intf_ptr);
            count++;

            /* Break after 2 seconds if AUX still busy - since slowest ODR is 0.78Hz*/
            if(count > 20)
            {
                rslt = BMI2_E_AUX_BUSY;
                break;
            }
        }
    }

    return rslt;
}

/*!
 * @brief This internal API triggers read out offset and sets ODR of the
 * auxiliary sensor.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::config_aux(const struct bmi2_aux_config *config, struct bmi2_dev *dev)
* @brief      Config aux
* @ingroup    DATAIO
* 
* @param[in]  config : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::config_aux(const struct bmi2_aux_config *config, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to store data */
    XBYTE reg_data;

    rslt = bmi2_get_regs(BMI2_AUX_CONF_ADDR, &reg_data, 1, dev);
    if(rslt == BMI2_OK)
    {
        /* Trigger read out offset */
        reg_data = BMI2_SET_BITS(reg_data, BMI2_AUX_OFFSET_READ_OUT, config->offset);

        /* Set ODR */
        reg_data = BMI2_SET_BIT_POS0(reg_data, BMI2_AUX_ODR_EN, config->odr);

        /* Set auxiliary configuration register */
        rslt = bmi2_set_regs(BMI2_AUX_CONF_ADDR, &reg_data, 1, dev);
        bmi2_delay_us(1000, dev->intf_ptr);
    }

    return rslt;
}

/*!
 * @brief This internal API checks the busy status of auxiliary sensor and sets
 * the auxiliary register addresses when not busy.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_if_aux_not_busy(XBYTE reg_addr, XBYTE reg_data, struct bmi2_dev *dev)
* @brief      Set if aux not busy
* @ingroup    DATAIO
* 
* @param[in]  reg_addr : 
* @param[in]  reg_data : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_if_aux_not_busy(XBYTE reg_addr, XBYTE reg_data, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to get status of AUX_BUSY */
    XBYTE status = 0;

    /* Variable to define count for time-out */
    XBYTE count = 0;

    for (;;)
    {
        /* Check if AUX is busy */
        rslt = bmi2_get_status(&status, dev);

        /* Set the registers if not busy */
        if((rslt == BMI2_OK) && (!(status & BMI2_AUX_BUSY)))
        {
            rslt = bmi2_set_regs(reg_addr, &reg_data, 1, dev);
            bmi2_delay_us(1000, dev->intf_ptr);

            /* Break after setting the register */
            break;
        }

        /* Increment count after every 10 seconds */
        bmi2_delay_us(10000, dev->intf_ptr);
        count++;

        /* Break after 2 seconds if AUX still busy - since slowest ODR is 0.78Hz*/
        if(count > 20)
        {
            rslt = BMI2_E_AUX_BUSY;
            break;
        }
    }

    return rslt;
}

/*!
 * @brief This internal API validates auxiliary configuration set by the user.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::validate_aux_config(struct bmi2_aux_config *config, struct bmi2_dev *dev)
* @brief      Validate aux config
* @ingroup    DATAIO
* 
* @param[in]  config : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::validate_aux_config(struct bmi2_aux_config *config, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Validate ODR for auxiliary sensor */
    rslt = check_boundary_val(&config->odr, BMI2_AUX_ODR_0_78HZ, BMI2_AUX_ODR_800HZ, dev);

    return rslt;
}

/*!
 * @brief This internal API gets accelerometer configurations like ODR,
 * bandwidth, performance mode and g-range.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_accel_config(struct bmi2_accel_config *config, struct bmi2_dev *dev)
* @brief      Get accel config
* @ingroup    DATAIO
* 
* @param[in]  config : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_accel_config(struct bmi2_accel_config *config, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Array to store data */
    XBYTE data_array[2] = { 0 };

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if((rslt == BMI2_OK) && (config != NULL))
    {
        /* Read the sensor configuration details */
        rslt = bmi2_get_regs(BMI2_ACC_CONF_ADDR, data_array, 2, dev);
        if(rslt == BMI2_OK)
        {
            /* Get accelerometer performance mode */
            config->filter_perf = BMI2_GET_BITS(data_array[0], BMI2_ACC_FILTER_PERF_MODE);

            /* Get accelerometer bandwidth */
            config->bwp = BMI2_GET_BITS(data_array[0], BMI2_ACC_BW_PARAM);

            /* Get accelerometer ODR */
            config->odr = BMI2_GET_BIT_POS0(data_array[0], BMI2_ACC_ODR);

            /* Get accelerometer range */
            config->range = BMI2_GET_BIT_POS0(data_array[1], BMI2_ACC_RANGE);
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This internal API gets gyroscope configurations like ODR, bandwidth,
 * low power/high performance mode, performance mode and range.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_gyro_config(struct bmi2_gyro_config *config, struct bmi2_dev *dev)
* @brief      Get gyro config
* @ingroup    DATAIO
* 
* @param[in]  config : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_gyro_config(struct bmi2_gyro_config *config, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Array to store data */
    XBYTE data_array[2] = { 0 };

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if((rslt == BMI2_OK) && (config != NULL))
    {
        /* Read the sensor configuration details */
        rslt = bmi2_get_regs(BMI2_GYR_CONF_ADDR, data_array, 2, dev);
        if(rslt == BMI2_OK)
        {
            /* Get gyroscope performance mode */
            config->filter_perf = BMI2_GET_BITS(data_array[0], BMI2_GYR_FILTER_PERF_MODE);

            /* Get gyroscope noise performance mode */
            config->noise_perf = BMI2_GET_BITS(data_array[0], BMI2_GYR_NOISE_PERF_MODE);

            /* Get gyroscope bandwidth */
            config->bwp = BMI2_GET_BITS(data_array[0], BMI2_GYR_BW_PARAM);

            /* Get gyroscope ODR */
            config->odr = BMI2_GET_BIT_POS0(data_array[0], BMI2_GYR_ODR);

            /* Get gyroscope OIS range */
            config->ois_range = BMI2_GET_BITS(data_array[1], BMI2_GYR_OIS_RANGE);

            /* Get gyroscope range */
            config->range = BMI2_GET_BIT_POS0(data_array[1], BMI2_GYR_RANGE);
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This internal API:
 * 1) Gets the status of auxiliary interface enable.
 * 2) Gets auxiliary interface configurations like I2C address, manual/auto
 * mode enable, manual burst read length, AUX burst read length and AUX read
 * address.
 * 3) Gets ODR and offset.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_aux_config(struct bmi2_aux_config *config, struct bmi2_dev *dev)
* @brief      Get aux config
* @ingroup    DATAIO
* 
* @param[in]  config : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_aux_config(struct bmi2_aux_config *config, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if((rslt == BMI2_OK) && (config != NULL))
    {
        /* Get enable status of auxiliary interface */
        rslt = get_aux_interface(config, dev);
        if(rslt == BMI2_OK)
        {
            /* Get the auxiliary interface configurations */
            rslt = get_aux_interface_config(config, dev);
            if(rslt == BMI2_OK)
            {
                /* Get read out offset and ODR */
                rslt = get_aux_cfg(config, dev);
            }
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This internal API gets gyroscope user-gain configurations like gain
 * update value for x, y and z-axis.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_gyro_gain_update_config(struct bmi2_gyro_user_gain_config *config, struct bmi2_dev *dev)
* @brief      Get gyro gain update config
* @ingroup    DATAIO
* 
* @param[in]  config : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_gyro_gain_update_config(struct bmi2_gyro_user_gain_config *config, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Array to define the feature configuration */
    XBYTE feat_config[BMI2_FEAT_SIZE_IN_BYTES] = { 0 };

    /* Variable to define the array offset */
    XBYTE idx = 0;

    /* Variable to define LSB */
    XWORD lsb = 0;

    /* Variable to define MSB */
    XWORD msb = 0;

    /* Variable to define a word */
    XWORD lsb_msb = 0;

    /* Variable to set flag */
    XBYTE feat_found;

    /* Initialize feature configuration for user-gain */
    struct bmi2_feature_config user_gain_config = { 0, 0, 0 };

    /* Search for user-gain feature and extract its configuration details */
    feat_found = bmi2_extract_input_feat_config(&user_gain_config, BMI2_GYRO_GAIN_UPDATE, dev);
    if(feat_found)
    {
        /* Get the configuration from the page where user-gain feature resides */
        rslt = bmi2_get_feat_config(user_gain_config.page, feat_config, dev);
        if(rslt == BMI2_OK)
        {
            /* Define the offset in bytes for user-gain select */
            idx = user_gain_config.start_addr;

            /* Get word to calculate ratio_x */
            lsb = (XWORD) feat_config[idx++];
            msb = ((XWORD) feat_config[idx++] << 8);
            lsb_msb = lsb | msb;

            /* Get ratio_x */
            config->ratio_x = lsb_msb & BMI2_GYR_USER_GAIN_RATIO_X_MASK;

            /* Get word to calculate ratio_y */
            lsb = (XWORD) feat_config[idx++];
            msb = ((XWORD) feat_config[idx++] << 8);
            lsb_msb = lsb | msb;

            /* Get ratio_y */
            config->ratio_y = lsb_msb & BMI2_GYR_USER_GAIN_RATIO_Y_MASK;

            /* Get word to calculate ratio_z */
            lsb = (XWORD) feat_config[idx++];
            msb = ((XWORD) feat_config[idx++] << 8);
            lsb_msb = lsb | msb;

            /* Get ratio_z */
            config->ratio_z = lsb_msb & BMI2_GYR_USER_GAIN_RATIO_Z_MASK;
        }
    }
    else
    {
        rslt = BMI2_E_INVALID_SENSOR;
    }

    return rslt;
}

/*!
 * @brief This internal API gets the enable status of auxiliary interface.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_aux_interface(struct bmi2_aux_config *config, struct bmi2_dev *dev)
* @brief      Get aux interface
* @ingroup    DATAIO
* 
* @param[in]  config : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_aux_interface(struct bmi2_aux_config *config, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to store data */
    XBYTE reg_data;

    /* Get the enable status of auxiliary interface */
    rslt = bmi2_get_regs(BMI2_IF_CONF_ADDR, &reg_data, 1, dev);
    if(rslt == BMI2_OK)
    {
        config->aux_en = BMI2_GET_BITS(reg_data, BMI2_AUX_IF_EN);
    }

    return rslt;
}

/*!
 * @brief This internal API gets auxiliary configurations like manual/auto mode
 * FCU write command enable and read burst length for both data and manual mode.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_aux_interface_config(struct bmi2_aux_config *config, struct bmi2_dev *dev)
* @brief      Get aux interface config
* @ingroup    DATAIO
* 
* @param[in]  config : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_aux_interface_config(struct bmi2_aux_config *config, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to store data */
    XBYTE reg_data[2] = { 0 };

    rslt = bmi2_get_regs(BMI2_AUX_DEV_ID_ADDR, reg_data, 2, dev);
    if(rslt == BMI2_OK)
    {
        /* Get I2C address for auxiliary sensor */
        config->i2c_device_addr = BMI2_GET_BITS(reg_data[0], BMI2_AUX_SET_I2C_ADDR);

        /* Get the AUX IF to either manual or auto mode */
        config->manual_en = BMI2_GET_BITS(reg_data[1], BMI2_AUX_MAN_MODE_EN);

        /* Enables FCU write command on AUX IF for auxiliary sensors that need a trigger */
        config->fcu_write_en = BMI2_GET_BITS(reg_data[1], BMI2_AUX_FCU_WR_EN);

        /* Get the burst read length for manual mode */
        config->man_rd_burst = BMI2_GET_BITS(reg_data[1], BMI2_AUX_MAN_READ_BURST);

        /* Get the burst read length for data mode */
        config->aux_rd_burst = BMI2_GET_BIT_POS0(reg_data[1], BMI2_AUX_READ_BURST);

        /* If data mode, get the read address of the auxiliary sensor from where data is to be read */
        if(!config->manual_en)
        {
            rslt = bmi2_get_regs(BMI2_AUX_RD_ADDR, &config->read_addr, 1, dev);
        }
    }

    return rslt;
}

/*!
 * @brief This internal API gets read out offset and ODR of the auxiliary
 * sensor.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_aux_cfg(struct bmi2_aux_config *config, struct bmi2_dev *dev)
* @brief      Get aux cfg
* @ingroup    DATAIO
* 
* @param[in]  config : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_aux_cfg(struct bmi2_aux_config *config, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to store data */
    XBYTE reg_data;

    rslt = bmi2_get_regs(BMI2_AUX_CONF_ADDR, &reg_data, 1, dev);
    if(rslt == BMI2_OK)
    {
        /* Get read out offset */
        config->offset = BMI2_GET_BITS(reg_data, BMI2_AUX_OFFSET_READ_OUT);

        /* Get ODR */
        config->odr = BMI2_GET_BIT_POS0(reg_data, BMI2_AUX_ODR_EN);
    }

    return rslt;
}

/*!
 * @brief This internal API maps/un-maps feature interrupts to that of interrupt
 * pins.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::map_feat_int(XBYTE *reg_data_array, enum bmi2_hw_int_pin int_pin, XBYTE int_mask)
* @brief      Map feat int
* @ingroup    DATAIO
* 
* @param[in]  reg_data_array : 
* @param[in]  int_pin : 
* @param[in]  int_mask : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::map_feat_int(XBYTE *reg_data_array, enum bmi2_hw_int_pin int_pin, XBYTE int_mask)
{
    /* Variable to define error */
    XBYTESIG rslt = BMI2_OK;

    /* Check for NULL error */
    if(reg_data_array != NULL)
    {
        /* Check validity on interrupt pin selection */
        if(int_pin < BMI2_INT_PIN_MAX)
        {
            switch (int_pin)
            {
                case BMI2_INT_NONE:

                    /* Un-Map the corresponding feature interrupt to interrupt pin 1 and 2 */
                    reg_data_array[0] &= ~(int_mask);
                    reg_data_array[1] &= ~(int_mask);
                    break;
                case BMI2_INT1:

                    /* Map the corresponding feature interrupt to interrupt pin 1 */
                    reg_data_array[0] |= int_mask;

                    /* Un-map the corresponding feature interrupt to interrupt pin 2 */
                    reg_data_array[1] &= ~(int_mask);
                    break;
                case BMI2_INT2:

                    /* Map the corresponding feature interrupt to interrupt pin 2 */
                    reg_data_array[1] |= int_mask;

                    /* Un-map the corresponding feature interrupt to interrupt pin 1 */
                    reg_data_array[0] &= ~(int_mask);
                    break;
                case BMI2_INT_BOTH:

                    /* Map the corresponding feature interrupt to interrupt pin 1 and 2 */
                    reg_data_array[0] |= int_mask;
                    reg_data_array[1] |= int_mask;
                    break;
                default:
                    break;
            }
        }
        else
        {
            /* Return error if invalid pin selection */
            rslt = BMI2_E_INVALID_INT_PIN;
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This internal API gets the accelerometer data from the register.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_accel_sensor_data(struct bmi2_sens_axes_data *data, XBYTE reg_addr, struct bmi2_dev *dev)
* @brief      Get accel sensor data
* @ingroup    DATAIO
* 
* @param[in]  data : 
* @param[in]  reg_addr : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_accel_sensor_data(struct bmi2_sens_axes_data *data, XBYTE reg_addr, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Array to define data stored in register */
    XBYTE reg_data[BMI2_ACC_GYR_NUM_BYTES] = { 0 };

    /* Read the sensor data */
    rslt = bmi2_get_regs(reg_addr, reg_data, BMI2_ACC_GYR_NUM_BYTES, dev);
    if(rslt == BMI2_OK)
    {
        /* Get accelerometer data from the register */
        get_acc_gyr_data(data, reg_data);

        /* Get the re-mapped accelerometer data */
        get_remapped_data(data, dev);
    }

    return rslt;
}

/*!
 * @brief This internal API gets the gyroscope data from the register.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_gyro_sensor_data(struct bmi2_sens_axes_data *data, XBYTE reg_addr, struct bmi2_dev *dev)
* @brief      Get gyro sensor data
* @ingroup    DATAIO
* 
* @param[in]  data : 
* @param[in]  reg_addr : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_gyro_sensor_data(struct bmi2_sens_axes_data *data, XBYTE reg_addr, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Array to define data stored in register */
    XBYTE reg_data[BMI2_ACC_GYR_NUM_BYTES] = { 0 };

    /* Read the sensor data */
    rslt = bmi2_get_regs(reg_addr, reg_data, BMI2_ACC_GYR_NUM_BYTES, dev);
    if(rslt == BMI2_OK)
    {
        /* Get gyroscope data from the register */
        get_acc_gyr_data(data, reg_data);

        /* Get the compensated gyroscope data */
        comp_gyro_cross_axis_sensitivity(data, dev);

        /* Get the re-mapped gyroscope data */
        get_remapped_data(data, dev);

    }

    return rslt;
}

/*!
 * @brief This internal API gets the accelerometer/gyroscope data.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C6AXISTRACKINGBMI270::get_acc_gyr_data(struct bmi2_sens_axes_data *data, const XBYTE *reg_data)
* @brief      Get acc gyr data
* @ingroup    DATAIO
* 
* @param[in]  data : 
* @param[in]  reg_data : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C6AXISTRACKINGBMI270::get_acc_gyr_data(struct bmi2_sens_axes_data *data, const XBYTE *reg_data)
{
    /* Variables to store msb value */
    XBYTE msb;

    /* Variables to store lsb value */
    XBYTE lsb;

    /* Variables to store both msb and lsb value */
    XWORD msb_lsb;

    /* Variables to define index */
    XBYTE index = 0;

    /* Read x-axis data */
    lsb = reg_data[index++];
    msb = reg_data[index++];
    msb_lsb = ((XWORD) msb << 8) | (XWORD) lsb;
    data->x = (XWORDSIG) msb_lsb;

    /* Read y-axis data */
    lsb = reg_data[index++];
    msb = reg_data[index++];
    msb_lsb = ((XWORD) msb << 8) | (XWORD) lsb;
    data->y = (XWORDSIG) msb_lsb;

    /* Read z-axis data */
    lsb = reg_data[index++];
    msb = reg_data[index++];
    msb_lsb = ((XWORD) msb << 8) | (XWORD) lsb;
    data->z = (XWORDSIG) msb_lsb;
}

/*!
 * @brief This internal API gets the re-mapped accelerometer/gyroscope data.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C6AXISTRACKINGBMI270::get_remapped_data(struct bmi2_sens_axes_data *data, const struct bmi2_dev *dev)
* @brief      Get remapped data
* @ingroup    DATAIO
* 
* @param[in]  data : 
* @param[in]  dev : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C6AXISTRACKINGBMI270::get_remapped_data(struct bmi2_sens_axes_data *data, const struct bmi2_dev *dev)
{
    /* Array to defined the re-mapped sensor data */
    XWORDSIG remap_data[3] = { 0 };
    XWORDSIG pos_multiplier = INT16_C(1);
    XWORDSIG neg_multiplier = INT16_C(-1);

    /* Fill the array with the un-mapped sensor data */
    remap_data[0] = data->x;
    remap_data[1] = data->y;
    remap_data[2] = data->z;

    /* Get the re-mapped x axis data */
    if(dev->remap.x_axis_sign == BMI2_POS_SIGN)
    {
        data->x = (XWORDSIG)(remap_data[dev->remap.x_axis] * pos_multiplier);
    }
    else
    {
        data->x = (XWORDSIG)(remap_data[dev->remap.x_axis] * neg_multiplier);
    }

    /* Get the re-mapped y axis data */
    if(dev->remap.y_axis_sign == BMI2_POS_SIGN)
    {
        data->y = (XWORDSIG)(remap_data[dev->remap.y_axis] * pos_multiplier);
    }
    else
    {
        data->y = (XWORDSIG)(remap_data[dev->remap.y_axis] * neg_multiplier);
    }

    /* Get the re-mapped z axis data */
    if(dev->remap.z_axis_sign == BMI2_POS_SIGN)
    {
        data->z = (XWORDSIG)(remap_data[dev->remap.z_axis] * pos_multiplier);
    }
    else
    {
        data->z = (XWORDSIG)(remap_data[dev->remap.z_axis] * neg_multiplier);
    }
}

/*!
 * @brief This internal API reads the user-defined bytes of data from the given
 * register address of auxiliary sensor in manual mode.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::read_aux_data(XBYTE reg_addr, XBYTE *aux_data, XWORD len, XBYTE burst_len, struct bmi2_dev *dev)
* @brief      Read aux data
* @ingroup    DATAIO
* 
* @param[in]  reg_addr : 
* @param[in]  aux_data : 
* @param[in]  len : 
* @param[in]  burst_len : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::read_aux_data(XBYTE reg_addr, XBYTE *aux_data, XWORD len, XBYTE burst_len, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt = BMI2_OK;

    /* Array to store the register data */
    XBYTE reg_data[15] = { 0 };

    /* Variable to define number of bytes to read */
    XWORD read_length = 0;

    /* Variable to define loop */
    XBYTE loop = 0;

    /* Variable to define counts to get the correct array index */
    XBYTE count = 0;

    /* Variable to define index for the array */
    XBYTE idx = 0;

    while (len > 0)
    {
        /* Set the read address if AUX is not busy */
        rslt = set_if_aux_not_busy(BMI2_AUX_RD_ADDR, reg_addr, dev);
        if(rslt == BMI2_OK)
        {
            /* Read data from bmi2 data register */
            rslt = bmi2_get_regs(BMI2_AUX_X_LSB_ADDR, reg_data, (XWORD) burst_len, dev);
            bmi2_delay_us(1000, dev->intf_ptr);
            if(rslt == BMI2_OK)
            {
                /* Get number of bytes to be read */
                if(len < burst_len)
                {
                    read_length = (XBYTE) len;
                }
                else
                {
                    read_length = burst_len;
                }

                /* Update array index and store the data */
                for (loop = 0; loop < read_length; loop++)
                {
                    idx = loop + count;
                    aux_data[idx] = reg_data[loop];
                }
            }
        }

        /* Update address for the next read */
        reg_addr += burst_len;

        /* Update count for the array index */
        count += burst_len;

        /* Update no of bytes left to read */
        len -= read_length;
    }

    return rslt;
}

/*!
 * @brief This internal API writes AUX write address and the user-defined bytes
 * of data to the AUX sensor in manual mode.
 *
 * @note Change of BMI2_AUX_WR_ADDR is only allowed if AUX is not busy.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::write_aux_data(XBYTE reg_addr, XBYTE reg_data, struct bmi2_dev *dev)
* @brief      Write aux data
* @ingroup    DATAIO
* 
* @param[in]  reg_addr : 
* @param[in]  reg_data : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::write_aux_data(XBYTE reg_addr, XBYTE reg_data, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Write data to be written to the AUX sensor in bmi2 register */
    rslt = bmi2_set_regs(BMI2_AUX_WR_DATA_ADDR, &reg_data, 1, dev);
    if(rslt == BMI2_OK)
    {
        /* Write the AUX address where data is to be stored when AUX is not busy */
        rslt = set_if_aux_not_busy(BMI2_AUX_WR_ADDR, reg_addr, dev);
    }

    return rslt;
}

/*!
 * @brief This internal API reads the user-defined bytes of data from the given
 * register address of auxiliary sensor in data mode.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::read_aux_data_mode(XBYTE *aux_data, struct bmi2_dev *dev)
* @brief      Read aux data mode
* @ingroup    DATAIO
* 
* @param[in]  aux_data : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::read_aux_data_mode(XBYTE *aux_data, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variables to define loop */
    XBYTE count = 0;

    /* Variables to define index */
    XBYTE index = 0;

    /* Array to define data stored in register */
    XBYTE reg_data[BMI2_AUX_NUM_BYTES] = { 0 };

    /* Check if data mode */
    if(!dev->aux_man_en)
    {
        /* Read the auxiliary sensor data */
        rslt = bmi2_get_regs(BMI2_AUX_X_LSB_ADDR, reg_data, BMI2_AUX_NUM_BYTES, dev);
        if(rslt == BMI2_OK)
        {
            /* Get the 8 bytes of auxiliary data */
            do
            {
                *(aux_data + count++) = *(reg_data + index++);
            } while (count < BMI2_AUX_NUM_BYTES);
        }
    }
    else
    {
        rslt = BMI2_E_AUX_INVALID_CFG;
    }

    return rslt;
}

/*!
 * @brief This internal API maps the actual burst read length with that of the
 * register value set by user.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::map_read_len(XBYTE *len, const struct bmi2_dev *dev)
* @brief      Map read len
* @ingroup    DATAIO
* 
* @param[in]  len : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::map_read_len(XBYTE *len, const struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt = BMI2_OK;

    /* Get the burst read length against the values set by the user */
    switch (dev->aux_man_rd_burst_len)
    {
        case BMI2_AUX_READ_LEN_0:
            *len = 1;
            break;
        case BMI2_AUX_READ_LEN_1:
            *len = 2;
            break;
        case BMI2_AUX_READ_LEN_2:
            *len = 6;
            break;
        case BMI2_AUX_READ_LEN_3:
            *len = 8;
            break;
        default:
            rslt = BMI2_E_AUX_INVALID_CFG;
            break;
    }

    return rslt;
}

/*!
 * @brief This internal API computes the number of bytes of accelerometer FIFO
 * data which is to be parsed in header-less mode.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::parse_fifo_accel_len(XWORD *start_idx, XWORD *len, const XWORD *acc_count, const struct bmi2_fifo_frame *fifo)
* @brief      Parse fifo accel len
* @ingroup    DATAIO
* 
* @param[in]  start_idx : 
* @param[in]  len : 
* @param[in]  acc_count : 
* @param[in]  fifo : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::parse_fifo_accel_len(XWORD *start_idx, XWORD *len, const XWORD *acc_count, const struct bmi2_fifo_frame *fifo)
{
    /* Variable to define error */
    XBYTESIG rslt = BMI2_OK;

    /* Data start index */
    (*start_idx) = fifo->acc_byte_start_idx;

    /* If only accelerometer is enabled */
    if(fifo->data_enable == BMI2_FIFO_ACC_EN)
    {
        /* Number of bytes to be read */
        (*len) = (XWORD)((*acc_count) * BMI2_FIFO_ACC_LENGTH);
    }
    /* If only accelerometer and auxiliary are enabled */
    else if(fifo->data_enable == (BMI2_FIFO_ACC_EN | BMI2_FIFO_AUX_EN))
    {
        /* Number of bytes to be read */
        (*len) = (XWORD)((*acc_count) * BMI2_FIFO_ACC_AUX_LENGTH);
    }
    /* If only accelerometer and gyroscope are enabled */
    else if(fifo->data_enable == (BMI2_FIFO_ACC_EN | BMI2_FIFO_GYR_EN))
    {
        /* Number of bytes to be read */
        (*len) = (XWORD)((*acc_count) * BMI2_FIFO_ACC_GYR_LENGTH);
    }
    /* If only accelerometer, gyroscope and auxiliary are enabled */
    else if(fifo->data_enable == (BMI2_FIFO_ACC_EN | BMI2_FIFO_GYR_EN | BMI2_FIFO_AUX_EN))
    {
        /* Number of bytes to be read */
        (*len) = (XWORD)((*acc_count) * BMI2_FIFO_ALL_LENGTH);
    }
    else
    {
        /* Move the data index to the last byte to mark completion when
         * no sensors or sensors apart from accelerometer are enabled
         */
        (*start_idx) = fifo->length;

        /* FIFO is empty */
        rslt = BMI2_W_FIFO_EMPTY;
    }

    /* If more data is requested than available */
    if((*len) > fifo->length)
    {
        (*len) = fifo->length;
    }

    return rslt;
}

/*!
 * @brief This internal API is used to parse the accelerometer data from the
 * FIFO in header mode.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::extract_accel_header_mode(struct bmi2_sens_axes_data *acc, XWORD *accel_length, struct bmi2_fifo_frame *fifo, const struct bmi2_dev *dev)
* @brief      Extract accel header mode
* @ingroup    DATAIO
* 
* @param[in]  acc : 
* @param[in]  accel_length : 
* @param[in]  fifo : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::extract_accel_header_mode(struct bmi2_sens_axes_data *acc, XWORD *accel_length, struct bmi2_fifo_frame *fifo, const struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt = BMI2_OK;

    /* Variable to define header frame */
    XBYTE frame_header = 0;

    /* Variable to index the data bytes */
    XWORD data_index;

    /* Variable to index accelerometer frames */
    XWORD accel_index = 0;

    /* Variable to indicate accelerometer frames read */
    XWORD frame_to_read = *accel_length;

    for (data_index = fifo->acc_byte_start_idx; data_index < fifo->length;)
    {
        /* Get frame header byte */
        frame_header = fifo->data[data_index] & BMI2_FIFO_TAG_INTR_MASK;

        /* Parse virtual header if S4S is enabled */
        parse_if_virtual_header(&frame_header, &data_index, fifo);

        /* Index shifted to next byte where data starts */
        data_index++;
        switch (frame_header)
        {
            /* If header defines accelerometer frame */
            case BMI2_FIFO_HEADER_ACC_FRM:
            case BMI2_FIFO_HEADER_AUX_ACC_FRM:
            case BMI2_FIFO_HEADER_GYR_ACC_FRM:
            case BMI2_FIFO_HEADER_ALL_FRM:

                /* Unpack from normal frames */
                rslt = unpack_accel_frame(acc, &data_index, &accel_index, frame_header, fifo, dev);
                break;

            /* If header defines only gyroscope frame */
            case BMI2_FIFO_HEADER_GYR_FRM:
                rslt = move_next_frame(&data_index, fifo->gyr_frm_len, fifo);
                break;

            /* If header defines only auxiliary frame */
            case BMI2_FIFO_HEADER_AUX_FRM:
                rslt = move_next_frame(&data_index, fifo->aux_frm_len, fifo);
                break;

            /* If header defines only auxiliary and gyroscope frame */
            case BMI2_FIFO_HEADER_AUX_GYR_FRM:
                rslt = move_next_frame(&data_index, fifo->aux_gyr_frm_len, fifo);
                break;

            /* If header defines sensor time frame */
            case BMI2_FIFO_HEADER_SENS_TIME_FRM:
                rslt = unpack_sensortime_frame(&data_index, fifo);
                break;

            /* If header defines skip frame */
            case BMI2_FIFO_HEADER_SKIP_FRM:
                rslt = unpack_skipped_frame(&data_index, fifo);
                break;

            /* If header defines Input configuration frame */
            case BMI2_FIFO_HEADER_INPUT_CFG_FRM:
                rslt = move_next_frame(&data_index, BMI2_FIFO_INPUT_CFG_LENGTH, fifo);
                break;

            /* If header defines invalid frame or end of valid data */
            case BMI2_FIFO_HEAD_OVER_READ_MSB:

                /* Move the data index to the last byte to mark completion */
                data_index = fifo->length;

                /* FIFO is empty */
                rslt = BMI2_W_FIFO_EMPTY;
                break;
            case BMI2_FIFO_VIRT_ACT_RECOG_FRM:
                rslt = move_next_frame(&data_index, BMI2_FIFO_VIRT_ACT_DATA_LENGTH, fifo);
                break;
            default:

                /* Move the data index to the last byte in case of invalid values */
                data_index = fifo->length;

                /* FIFO is empty */
                rslt = BMI2_W_FIFO_EMPTY;
                break;
        }

        /* Break if Number of frames to be read is complete or FIFO is mpty */
        if((frame_to_read == accel_index) || (rslt == BMI2_W_FIFO_EMPTY))
        {
            break;
        }
    }

    /* Update the accelerometer frame index */
    (*accel_length) = accel_index;

    /* Update the accelerometer byte index */
    fifo->acc_byte_start_idx = data_index;

    return rslt;
}

/*!
 * @brief This internal API is used to parse the accelerometer data from the
 * FIFO data in both header and header-less mode. It updates the current data
 * byte to be parsed.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::unpack_accel_frame(struct bmi2_sens_axes_data *acc, XWORD *idx, XWORD *acc_idx, XBYTE frame, const struct bmi2_fifo_frame *fifo, const struct bmi2_dev *dev)
* @brief      Unpack accel frame
* @ingroup    DATAIO
* 
* @param[in]  acc : 
* @param[in]  idx : 
* @param[in]  acc_idx : 
* @param[in]  frame : 
* @param[in]  fifo : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::unpack_accel_frame(struct bmi2_sens_axes_data *acc, XWORD *idx, XWORD *acc_idx, XBYTE frame, const struct bmi2_fifo_frame *fifo, const struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt = BMI2_OK;

    switch (frame)
    {
        /* If frame contains only accelerometer data */
        case BMI2_FIFO_HEADER_ACC_FRM:
        case BMI2_FIFO_HEAD_LESS_ACC_FRM:

            /* Partially read, then skip the data */
            if(((*idx) + fifo->acc_frm_len) > fifo->length)
            {
                /* Update the data index as complete*/
                (*idx) = fifo->length;

                /* FIFO is empty */
                rslt = BMI2_W_FIFO_EMPTY;
                break;
            }

            /* Get the accelerometer data */
            unpack_accel_data(&acc[(*acc_idx)], *idx, fifo, dev);

            /* Update data index */
            (*idx) = (*idx) + BMI2_FIFO_ACC_LENGTH;

            /* Get virtual sensor time if S4S is enabled */
            if(dev->sens_en_stat & BMI2_EXT_SENS_SEL)
            {
                unpack_virt_sensor_time(&acc[(*acc_idx)], idx, fifo);
            }

            /* Update accelerometer frame index */
            (*acc_idx)++;

            /* More frames could be read */
            rslt = BMI2_W_PARTIAL_READ;
            break;

        /* If frame contains accelerometer and gyroscope data */
        case BMI2_FIFO_HEADER_GYR_ACC_FRM:
        case BMI2_FIFO_HEAD_LESS_GYR_ACC_FRM:

            /* Partially read, then skip the data */
            if(((*idx) + fifo->acc_gyr_frm_len) > fifo->length)
            {
                /* Move the data index to the last byte */
                (*idx) = fifo->length;

                /* FIFO is empty */
                rslt = BMI2_W_FIFO_EMPTY;
                break;
            }

            /* Get the accelerometer data */
            unpack_accel_data(&acc[(*acc_idx)], ((*idx) + BMI2_FIFO_GYR_LENGTH), fifo, dev);

            /* Update data index */
            (*idx) = (*idx) + BMI2_FIFO_ACC_GYR_LENGTH;

            /* Get virtual sensor time if S4S is enabled */
            if(dev->sens_en_stat & BMI2_EXT_SENS_SEL)
            {
                unpack_virt_sensor_time(&acc[(*acc_idx)], idx, fifo);
            }

            /* Update accelerometer frame index */
            (*acc_idx)++;

            /* More frames could be read */
            rslt = BMI2_W_PARTIAL_READ;
            break;

        /* If frame contains accelerometer and auxiliary data */
        case BMI2_FIFO_HEADER_AUX_ACC_FRM:
        case BMI2_FIFO_HEAD_LESS_AUX_ACC_FRM:

            /* Partially read, then skip the data */
            if(((*idx) + fifo->acc_aux_frm_len) > fifo->length)
            {
                /* Move the data index to the last byte */
                (*idx) = fifo->length;

                /* FIFO is empty */
                rslt = BMI2_W_FIFO_EMPTY;
                break;
            }

            /* Get the accelerometer data */
            unpack_accel_data(&acc[(*acc_idx)], ((*idx) + BMI2_FIFO_AUX_LENGTH), fifo, dev);

            /* Update data index */
            (*idx) = (*idx) + BMI2_FIFO_ACC_AUX_LENGTH;

            /* Get virtual sensor time if S4S is enabled */
            if(dev->sens_en_stat & BMI2_EXT_SENS_SEL)
            {
                unpack_virt_sensor_time(&acc[(*acc_idx)], idx, fifo);
            }

            /* Update accelerometer frame index */
            (*acc_idx)++;

            /* More frames could be read */
            rslt = BMI2_W_PARTIAL_READ;
            break;

        /* If frame contains accelerometer, gyroscope and auxiliary data */
        case BMI2_FIFO_HEADER_ALL_FRM:
        case BMI2_FIFO_HEAD_LESS_ALL_FRM:

            /* Partially read, then skip the data*/
            if((*idx + fifo->all_frm_len) > fifo->length)
            {
                /* Move the data index to the last byte */
                (*idx) = fifo->length;

                /* FIFO is empty */
                rslt = BMI2_W_FIFO_EMPTY;
                break;
            }

            /* Get the accelerometer data */
            unpack_accel_data(&acc[(*acc_idx)], ((*idx) + BMI2_FIFO_GYR_AUX_LENGTH), fifo, dev);

            /* Update data index */
            (*idx) = (*idx) + BMI2_FIFO_ALL_LENGTH;

            /* Get virtual sensor time if S4S is enabled */
            if(dev->sens_en_stat & BMI2_EXT_SENS_SEL)
            {
                unpack_virt_sensor_time(&acc[(*acc_idx)], idx, fifo);
            }

            /* Update accelerometer frame index */
            (*acc_idx)++;

            /* More frames could be read */
            rslt = BMI2_W_PARTIAL_READ;
            break;

        /* If frame contains gyroscope and auxiliary data */
        case BMI2_FIFO_HEADER_AUX_GYR_FRM:
        case BMI2_FIFO_HEAD_LESS_GYR_AUX_FRM:

            /* Update data index */
            (*idx) = (*idx) + fifo->aux_gyr_frm_len;

            /* More frames could be read */
            rslt = BMI2_W_PARTIAL_READ;
            break;

        /* If frame contains only auxiliary data */
        case BMI2_FIFO_HEADER_AUX_FRM:
        case BMI2_FIFO_HEAD_LESS_AUX_FRM:

            /* Update data index */
            (*idx) = (*idx) + fifo->aux_frm_len;

            /* More frames could be read */
            rslt = BMI2_W_PARTIAL_READ;
            break;

        /* If frame contains only gyroscope data */
        case BMI2_FIFO_HEADER_GYR_FRM:
        case BMI2_FIFO_HEAD_LESS_GYR_FRM:

            /* Update data index */
            (*idx) = (*idx) + fifo->gyr_frm_len;

            /* More frames could be read */
            rslt = BMI2_W_PARTIAL_READ;
            break;
        default:

            /* Move the data index to the last byte in case of invalid values */
            (*idx) = fifo->length;

            /* FIFO is empty */
            rslt = BMI2_W_FIFO_EMPTY;
            break;
    }

    return rslt;
}

/*!
 * @brief This internal API is used to parse accelerometer data from the
 * FIFO data.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C6AXISTRACKINGBMI270::unpack_accel_data(struct bmi2_sens_axes_data *acc, XWORD data_start_index, const struct bmi2_fifo_frame *fifo, const struct bmi2_dev *dev)
* @brief      Unpack accel data
* @ingroup    DATAIO
* 
* @param[in]  acc : 
* @param[in]  data_start_index : 
* @param[in]  fifo : 
* @param[in]  dev : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C6AXISTRACKINGBMI270::unpack_accel_data(struct bmi2_sens_axes_data *acc, XWORD data_start_index, const struct bmi2_fifo_frame *fifo, const struct bmi2_dev *dev)
{
    /* Variables to store LSB value */
    XWORD data_lsb;

    /* Variables to store MSB value */
    XWORD data_msb;

    /* Accelerometer raw x data */
    data_lsb = fifo->data[data_start_index++];
    data_msb = fifo->data[data_start_index++];
    acc->x = (XWORDSIG)((data_msb << 8) | data_lsb);

    /* Accelerometer raw y data */
    data_lsb = fifo->data[data_start_index++];
    data_msb = fifo->data[data_start_index++];
    acc->y = (XWORDSIG)((data_msb << 8) | data_lsb);

    /* Accelerometer raw z data */
    data_lsb = fifo->data[data_start_index++];
    data_msb = fifo->data[data_start_index++];
    acc->z = (XWORDSIG)((data_msb << 8) | data_lsb);

    /* Get the re-mapped accelerometer data */
    get_remapped_data(acc, dev);
}

/*!
 * @brief This internal API computes the number of bytes of gyroscope FIFO data
 * which is to be parsed in header-less mode.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::parse_fifo_gyro_len(XWORD *start_idx, XWORD(*len), const XWORD *gyr_count, const struct bmi2_fifo_frame *fifo)
* @brief      Parse fifo gyro len
* @ingroup    DATAIO
* 
* @param[in]  start_idx : 
* @param[in]  len : 
* @param[in]  gyr_count : 
* @param[in]  fifo : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::parse_fifo_gyro_len(XWORD *start_idx, XWORD(*len), const XWORD *gyr_count, const struct bmi2_fifo_frame *fifo)
{
    /* Variable to define error */
    XBYTESIG rslt = BMI2_OK;

    /* Data start index */
    (*start_idx) = fifo->gyr_byte_start_idx;

    /* If only gyroscope is enabled */
    if(fifo->data_enable == BMI2_FIFO_GYR_EN)
    {
        /* Number of bytes to be read */
        (*len) = (XWORD)((*gyr_count) * BMI2_FIFO_GYR_LENGTH);
    }
    /* If only gyroscope and auxiliary are enabled */
    else if(fifo->data_enable == (BMI2_FIFO_GYR_EN | BMI2_FIFO_AUX_EN))
    {
        /* Number of bytes to be read */
        (*len) = (XWORD)((*gyr_count) * BMI2_FIFO_GYR_AUX_LENGTH);
    }
    /* If only accelerometer and gyroscope are enabled */
    else if(fifo->data_enable == (BMI2_FIFO_ACC_EN | BMI2_FIFO_GYR_EN))
    {
        /* Number of bytes to be read */
        (*len) = (XWORD)((*gyr_count) * BMI2_FIFO_ACC_GYR_LENGTH);
    }
    /* If only accelerometer, gyroscope and auxiliary are enabled */
    else if(fifo->data_enable == (BMI2_FIFO_GYR_EN | BMI2_FIFO_AUX_EN | BMI2_FIFO_ACC_EN))
    {
        /* Number of bytes to be read */
        (*len) = (XWORD)((*gyr_count) * BMI2_FIFO_ALL_LENGTH);
    }
    else
    {
        /* Move the data index to the last byte to mark completion when
         * no sensors or sensors apart from gyroscope are enabled
         */
        (*start_idx) = fifo->length;

        /* FIFO is empty */
        rslt = BMI2_W_FIFO_EMPTY;
    }

    /* If more data is requested than available */
    if(((*len)) > fifo->length)
    {
        (*len) = fifo->length;
    }

    return rslt;
}

/*!
 *  @brief This internal API is used to parse the gyroscope data from the
 *  FIFO data in header mode.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::extract_gyro_header_mode(struct bmi2_sens_axes_data *gyr, XWORD *gyro_length, struct bmi2_fifo_frame *fifo, const struct bmi2_dev *dev)
* @brief      Extract gyro header mode
* @ingroup    DATAIO
* 
* @param[in]  gyr : 
* @param[in]  gyro_length : 
* @param[in]  fifo : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::extract_gyro_header_mode(struct bmi2_sens_axes_data *gyr, XWORD *gyro_length, struct bmi2_fifo_frame *fifo, const struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt = BMI2_OK;

    /* Variable to define header frame */
    XBYTE frame_header = 0;

    /* Variable to index the data bytes */
    XWORD data_index;

    /* Variable to index gyroscope frames */
    XWORD gyro_index = 0;

    /* Variable to indicate gyroscope frames read */
    XWORD frame_to_read = (*gyro_length);

    for (data_index = fifo->gyr_byte_start_idx; data_index < fifo->length;)
    {
        /* Get frame header byte */
        frame_header = fifo->data[data_index] & BMI2_FIFO_TAG_INTR_MASK;

        /* Parse virtual header if S4S is enabled */
        parse_if_virtual_header(&frame_header, &data_index, fifo);

        /* Index shifted to next byte where data starts */
        data_index++;
        switch (frame_header)
        {
            /* If header defines gyroscope frame */
            case BMI2_FIFO_HEADER_GYR_FRM:
            case BMI2_FIFO_HEADER_GYR_ACC_FRM:
            case BMI2_FIFO_HEADER_AUX_GYR_FRM:
            case BMI2_FIFO_HEADER_ALL_FRM:

                /* Unpack from normal frames */
                rslt = unpack_gyro_frame(gyr, &data_index, &gyro_index, frame_header, fifo, dev);
                break;

            /* If header defines only accelerometer frame */
            case BMI2_FIFO_HEADER_ACC_FRM:
                rslt = move_next_frame(&data_index, fifo->acc_frm_len, fifo);
                break;

            /* If header defines only auxiliary frame */
            case BMI2_FIFO_HEADER_AUX_FRM:
                rslt = move_next_frame(&data_index, fifo->aux_frm_len, fifo);
                break;

            /* If header defines only auxiliary and accelerometer frame */
            case BMI2_FIFO_HEADER_AUX_ACC_FRM:
                rslt = move_next_frame(&data_index, fifo->acc_aux_frm_len, fifo);
                break;

            /* If header defines sensor time frame */
            case BMI2_FIFO_HEADER_SENS_TIME_FRM:
                rslt = unpack_sensortime_frame(&data_index, fifo);
                break;

            /* If header defines skip frame */
            case BMI2_FIFO_HEADER_SKIP_FRM:
                rslt = unpack_skipped_frame(&data_index, fifo);
                break;

            /* If header defines Input configuration frame */
            case BMI2_FIFO_HEADER_INPUT_CFG_FRM:
                rslt = move_next_frame(&data_index, BMI2_FIFO_INPUT_CFG_LENGTH, fifo);
                break;

            /* If header defines invalid frame or end of valid data */
            case BMI2_FIFO_HEAD_OVER_READ_MSB:

                /* Move the data index to the last byte */
                data_index = fifo->length;

                /* FIFO is empty */
                rslt = BMI2_W_FIFO_EMPTY;
                break;
            case BMI2_FIFO_VIRT_ACT_RECOG_FRM:
                rslt = move_next_frame(&data_index, BMI2_FIFO_VIRT_ACT_DATA_LENGTH, fifo);
                break;
            default:

                /* Move the data index to the last byte in case of invalid values */
                data_index = fifo->length;

                /* FIFO is empty */
                rslt = BMI2_W_FIFO_EMPTY;
                break;
        }

        /* Break if number of frames to be read is complete or FIFO is empty */
        if((frame_to_read == gyro_index) || (rslt == BMI2_W_FIFO_EMPTY))
        {
            break;
        }
    }

    /* Update the gyroscope frame index */
    (*gyro_length) = gyro_index;

    /* Update the gyroscope byte index */
    fifo->gyr_byte_start_idx = data_index;

    return rslt;
}

/*!
 * @brief This internal API is used to parse the gyroscope data from the FIFO
 * data in both header and header-less mode. It updates the current data byte to
 * be parsed.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::unpack_gyro_frame(struct bmi2_sens_axes_data *gyr, XWORD *idx, XWORD *gyr_idx, XBYTE frame, const struct bmi2_fifo_frame *fifo, const struct bmi2_dev *dev)
* @brief      Unpack gyro frame
* @ingroup    DATAIO
* 
* @param[in]  gyr : 
* @param[in]  idx : 
* @param[in]  gyr_idx : 
* @param[in]  frame : 
* @param[in]  fifo : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::unpack_gyro_frame(struct bmi2_sens_axes_data *gyr, XWORD *idx, XWORD *gyr_idx, XBYTE frame, const struct bmi2_fifo_frame *fifo, const struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt = BMI2_OK;

    switch (frame)
    {
        /* If frame contains only gyroscope data */
        case BMI2_FIFO_HEADER_GYR_FRM:
        case BMI2_FIFO_HEAD_LESS_GYR_FRM:

            /* Partially read, then skip the data */
            if(((*idx) + fifo->gyr_frm_len) > fifo->length)
            {
                /* Update the data index as complete*/
                (*idx) = fifo->length;

                /* FIFO is empty */
                rslt = BMI2_W_FIFO_EMPTY;
                break;
            }

            /* Get the gyroscope data */
            unpack_gyro_data(&gyr[(*gyr_idx)], *idx, fifo, dev);

            /* Update data index */
            (*idx) = (*idx) + BMI2_FIFO_GYR_LENGTH;

            /* Get virtual sensor time if S4S is enabled */
            if(dev->sens_en_stat & BMI2_EXT_SENS_SEL)
            {
                unpack_virt_sensor_time(&gyr[(*gyr_idx)], idx, fifo);
            }

            /* Update gyroscope frame index */
            (*gyr_idx)++;

            /* More frames could be read */
            rslt = BMI2_W_PARTIAL_READ;
            break;

        /* If frame contains accelerometer and gyroscope data */
        case BMI2_FIFO_HEADER_GYR_ACC_FRM:
        case BMI2_FIFO_HEAD_LESS_GYR_ACC_FRM:

            /* Partially read, then skip the data */
            if(((*idx) + fifo->acc_gyr_frm_len) > fifo->length)
            {
                /* Move the data index to the last byte */
                (*idx) = fifo->length;

                /* FIFO is empty */
                rslt = BMI2_W_FIFO_EMPTY;
                break;
            }

            /* Get the gyroscope data */
            unpack_gyro_data(&gyr[(*gyr_idx)], (*idx), fifo, dev);

            /* Update data index */
            (*idx) = (*idx) + BMI2_FIFO_ACC_GYR_LENGTH;

            /* Get virtual sensor time if S4S is enabled */
            if(dev->sens_en_stat & BMI2_EXT_SENS_SEL)
            {
                unpack_virt_sensor_time(&gyr[(*gyr_idx)], idx, fifo);
            }

            /* Update gyroscope frame index */
            (*gyr_idx)++;

            /* More frames could be read */
            rslt = BMI2_W_PARTIAL_READ;
            break;

        /* If frame contains gyroscope and auxiliary data */
        case BMI2_FIFO_HEADER_AUX_GYR_FRM:
        case BMI2_FIFO_HEAD_LESS_GYR_AUX_FRM:

            /* Partially read, then skip the data */
            if(((*idx) + fifo->aux_gyr_frm_len) > fifo->length)
            {
                /* Move the data index to the last byte */
                (*idx) = fifo->length;

                /* FIFO is empty */
                rslt = BMI2_W_FIFO_EMPTY;
                break;
            }

            /* Get the gyroscope data */
            unpack_gyro_data(&gyr[(*gyr_idx)], ((*idx) + BMI2_FIFO_AUX_LENGTH), fifo, dev);

            /* Update data index */
            (*idx) = (*idx) + BMI2_FIFO_GYR_AUX_LENGTH;

            /* Get virtual sensor time if S4S is enabled */
            if(dev->sens_en_stat & BMI2_EXT_SENS_SEL)
            {
                unpack_virt_sensor_time(&gyr[(*gyr_idx)], idx, fifo);
            }

            /* Update gyroscope frame index */
            (*gyr_idx)++;

            /* More frames could be read */
            rslt = BMI2_W_PARTIAL_READ;
            break;

        /* If frame contains accelerometer, gyroscope and auxiliary data */
        case BMI2_FIFO_HEADER_ALL_FRM:
        case BMI2_FIFO_HEAD_LESS_ALL_FRM:

            /* Partially read, then skip the data*/
            if((*idx + fifo->all_frm_len) > fifo->length)
            {
                /* Move the data index to the last byte */
                (*idx) = fifo->length;

                /* FIFO is empty */
                rslt = BMI2_W_FIFO_EMPTY;
                break;
            }

            /* Get the gyroscope data */
            unpack_gyro_data(&gyr[(*gyr_idx)], ((*idx) + BMI2_FIFO_AUX_LENGTH), fifo, dev);

            /* Update data index */
            (*idx) = (*idx) + BMI2_FIFO_ALL_LENGTH;

            /* Get virtual sensor time if S4S is enabled */
            if(dev->sens_en_stat & BMI2_EXT_SENS_SEL)
            {
                unpack_virt_sensor_time(&gyr[(*gyr_idx)], idx, fifo);
            }

            /* Update gyroscope frame index */
            (*gyr_idx)++;

            /* More frames could be read */
            rslt = BMI2_W_PARTIAL_READ;
            break;

        /* If frame contains accelerometer and auxiliary data */
        case BMI2_FIFO_HEADER_AUX_ACC_FRM:
        case BMI2_FIFO_HEAD_LESS_AUX_ACC_FRM:

            /* Update data index */
            (*idx) = (*idx) + fifo->acc_aux_frm_len;

            /* More frames could be read */
            rslt = BMI2_W_PARTIAL_READ;
            break;

        /* If frame contains only auxiliary data */
        case BMI2_FIFO_HEADER_AUX_FRM:
        case BMI2_FIFO_HEAD_LESS_AUX_FRM:

            /* Update data index */
            (*idx) = (*idx) + fifo->aux_frm_len;

            /* More frames could be read */
            rslt = BMI2_W_PARTIAL_READ;
            break;

        /* If frame contains only accelerometer data */
        case BMI2_FIFO_HEADER_ACC_FRM:
        case BMI2_FIFO_HEAD_LESS_ACC_FRM:

            /* Update data index */
            (*idx) = (*idx) + fifo->acc_frm_len;

            /* More frames could be read */
            rslt = BMI2_W_PARTIAL_READ;
            break;
        default:

            /* Move the data index to the last byte in case of invalid values */
            (*idx) = fifo->length;

            /* FIFO is empty */
            rslt = BMI2_W_FIFO_EMPTY;
            break;
    }

    return rslt;
}

/*!
 * @brief This internal API is used to parse gyroscope data from the FIFO data.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C6AXISTRACKINGBMI270::unpack_gyro_data(struct bmi2_sens_axes_data *gyr, XWORD data_start_index, const struct bmi2_fifo_frame *fifo, const struct bmi2_dev *dev)
* @brief      Unpack gyro data
* @ingroup    DATAIO
* 
* @param[in]  gyr : 
* @param[in]  data_start_index : 
* @param[in]  fifo : 
* @param[in]  dev : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C6AXISTRACKINGBMI270::unpack_gyro_data(struct bmi2_sens_axes_data *gyr, XWORD data_start_index, const struct bmi2_fifo_frame *fifo, const struct bmi2_dev *dev)
{
    /* Variables to store LSB value */
    XWORD data_lsb;

    /* Variables to store MSB value */
    XWORD data_msb;

    /* Gyroscope raw x data */
    data_lsb = fifo->data[data_start_index++];
    data_msb = fifo->data[data_start_index++];
    gyr->x = (XWORDSIG)((data_msb << 8) | data_lsb);

    /* Gyroscope raw y data */
    data_lsb = fifo->data[data_start_index++];
    data_msb = fifo->data[data_start_index++];
    gyr->y = (XWORDSIG)((data_msb << 8) | data_lsb);

    /* Gyroscope raw z data */
    data_lsb = fifo->data[data_start_index++];
    data_msb = fifo->data[data_start_index++];
    gyr->z = (XWORDSIG)((data_msb << 8) | data_lsb);

    /* Get the compensated gyroscope data */
    comp_gyro_cross_axis_sensitivity(gyr, dev);

    /* Get the re-mapped gyroscope data */
    get_remapped_data(gyr, dev);
}

/*!
 * @brief This API computes the number of bytes of auxiliary FIFO data which is
 * to be parsed in header-less mode.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::parse_fifo_aux_len(XWORD *start_idx, XWORD(*len), const XWORD *aux_count, const struct bmi2_fifo_frame *fifo)
* @brief      Parse fifo aux len
* @ingroup    DATAIO
* 
* @param[in]  start_idx : 
* @param[in]  len : 
* @param[in]  aux_count : 
* @param[in]  fifo : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::parse_fifo_aux_len(XWORD *start_idx, XWORD(*len), const XWORD *aux_count, const struct bmi2_fifo_frame *fifo)
{
    /* Variable to define error */
    XBYTESIG rslt = BMI2_OK;

    /* Data start index */
    *start_idx = fifo->aux_byte_start_idx;

    /* If only auxiliary is enabled */
    if(fifo->data_enable == BMI2_FIFO_AUX_EN)
    {
        /* Number of bytes to be read */
        (*len) = (XWORD)((*aux_count) * BMI2_FIFO_AUX_LENGTH);
    }
    /* If only accelerometer and auxiliary are enabled */
    else if(fifo->data_enable == (BMI2_FIFO_AUX_EN | BMI2_FIFO_ACC_EN))
    {
        /* Number of bytes to be read */
        (*len) = (XWORD)((*aux_count) * BMI2_FIFO_ACC_AUX_LENGTH);
    }
    /* If only accelerometer and gyroscope are enabled */
    else if(fifo->data_enable == (BMI2_FIFO_AUX_EN | BMI2_FIFO_GYR_EN))
    {
        /* Number of bytes to be read */
        (*len) = (XWORD)((*aux_count) * BMI2_FIFO_GYR_AUX_LENGTH);
    }
    /* If only accelerometer, gyroscope and auxiliary are enabled */
    else if(fifo->data_enable == (BMI2_FIFO_AUX_EN | BMI2_FIFO_GYR_EN | BMI2_FIFO_ACC_EN))
    {
        /* Number of bytes to be read */
        (*len) = (XWORD)((*aux_count) * BMI2_FIFO_ALL_LENGTH);
    }
    else
    {
        /* Move the data index to the last byte to mark completion when
         * no sensors or sensors apart from gyroscope are enabled
         */
        (*start_idx) = fifo->length;

        /* FIFO is empty */
        rslt = BMI2_W_FIFO_EMPTY;
    }

    /* If more data is requested than available */
    if(((*len)) > fifo->length)
    {
        (*len) = fifo->length;
    }

    return rslt;
}

/*!
 * @brief This API is used to parse the auxiliary data from the FIFO data in
 * header mode.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::extract_aux_header_mode(struct bmi2_aux_fifo_data *aux, XWORD *aux_len, struct bmi2_fifo_frame *fifo, const struct bmi2_dev *dev)
* @brief      Extract aux header mode
* @ingroup    DATAIO
* 
* @param[in]  aux : 
* @param[in]  aux_len : 
* @param[in]  fifo : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::extract_aux_header_mode(struct bmi2_aux_fifo_data *aux, XWORD *aux_len, struct bmi2_fifo_frame *fifo, const struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt = BMI2_OK;

    /* Variable to define header frame */
    XBYTE frame_header = 0;

    /* Variable to index the data bytes */
    XWORD data_index;

    /* Variable to index gyroscope frames */
    XWORD aux_index = 0;

    /* Variable to indicate auxiliary frames read */
    XWORD frame_to_read = *aux_len;

    for (data_index = fifo->aux_byte_start_idx; data_index < fifo->length;)
    {
        /* Get frame header byte */
        frame_header = fifo->data[data_index] & BMI2_FIFO_TAG_INTR_MASK;

        /* Parse virtual header if S4S is enabled */
        parse_if_virtual_header(&frame_header, &data_index, fifo);

        /* Index shifted to next byte where data starts */
        data_index++;
        switch (frame_header)
        {
            /* If header defines auxiliary frame */
            case BMI2_FIFO_HEADER_AUX_FRM:
            case BMI2_FIFO_HEADER_AUX_ACC_FRM:
            case BMI2_FIFO_HEADER_AUX_GYR_FRM:
            case BMI2_FIFO_HEADER_ALL_FRM:

                /* Unpack from normal frames */
                rslt = unpack_aux_frame(aux, &data_index, &aux_index, frame_header, fifo, dev);
                break;

            /* If header defines only accelerometer frame */
            case BMI2_FIFO_HEADER_ACC_FRM:
                rslt = move_next_frame(&data_index, fifo->acc_frm_len, fifo);
                break;

            /* If header defines only gyroscope frame */
            case BMI2_FIFO_HEADER_GYR_FRM:
                rslt = move_next_frame(&data_index, fifo->gyr_frm_len, fifo);
                break;

            /* If header defines only gyroscope and accelerometer frame */
            case BMI2_FIFO_HEADER_GYR_ACC_FRM:
                rslt = move_next_frame(&data_index, fifo->acc_gyr_frm_len, fifo);
                break;

            /* If header defines sensor time frame */
            case BMI2_FIFO_HEADER_SENS_TIME_FRM:
                rslt = unpack_sensortime_frame(&data_index, fifo);
                break;

            /* If header defines skip frame */
            case BMI2_FIFO_HEADER_SKIP_FRM:
                rslt = unpack_skipped_frame(&data_index, fifo);
                break;

            /* If header defines Input configuration frame */
            case BMI2_FIFO_HEADER_INPUT_CFG_FRM:
                rslt = move_next_frame(&data_index, BMI2_FIFO_INPUT_CFG_LENGTH, fifo);
                break;

            /* If header defines invalid frame or end of valid data */
            case BMI2_FIFO_HEAD_OVER_READ_MSB:

                /* Move the data index to the last byte */
                data_index = fifo->length;

                /* FIFO is empty */
                rslt = BMI2_W_FIFO_EMPTY;
                break;
            case BMI2_FIFO_VIRT_ACT_RECOG_FRM:
                rslt = move_next_frame(&data_index, BMI2_FIFO_VIRT_ACT_DATA_LENGTH, fifo);
                break;
            default:

                /* Move the data index to the last byte in case
                 * of invalid values
                 */
                data_index = fifo->length;

                /* FIFO is empty */
                rslt = BMI2_W_FIFO_EMPTY;
                break;
        }

        /* Break if number of frames to be read is complete or FIFO is
         * empty
         */
        if((frame_to_read == aux_index) || (rslt == BMI2_W_FIFO_EMPTY))
        {
            break;
        }
    }

    /* Update the gyroscope frame index */
    (*aux_len) = aux_index;

    /* Update the gyroscope byte index */
    fifo->aux_byte_start_idx = data_index;

    return rslt;
}

/*!
 * @brief This API is used to parse the auxiliary frame from the FIFO data in
 * both header mode and header-less mode and update the data_index value which
 * is used to store the index of the current data byte which is parsed.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::unpack_aux_frame(struct bmi2_aux_fifo_data *aux, XWORD *idx, XWORD *aux_idx, XBYTE frame, const struct bmi2_fifo_frame *fifo, const struct bmi2_dev *dev)
* @brief      Unpack aux frame
* @ingroup    DATAIO
* 
* @param[in]  aux : 
* @param[in]  idx : 
* @param[in]  aux_idx : 
* @param[in]  frame : 
* @param[in]  fifo : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::unpack_aux_frame(struct bmi2_aux_fifo_data *aux, XWORD *idx, XWORD *aux_idx, XBYTE frame, const struct bmi2_fifo_frame *fifo, const struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt = BMI2_OK;

    switch (frame)
    {
        /* If frame contains only auxiliary data */
        case BMI2_FIFO_HEADER_AUX_FRM:
        case BMI2_FIFO_HEAD_LESS_AUX_FRM:

            /* Partially read, then skip the data */
            if(((*idx) + fifo->aux_frm_len) > fifo->length)
            {
                /* Update the data index as complete*/
                (*idx) = fifo->length;

                /* FIFO is empty */
                rslt = BMI2_W_FIFO_EMPTY;
                break;
            }

            /* Get the auxiliary data */
            unpack_aux_data(&aux[(*aux_idx)], (*idx), fifo);

            /* Update data index */
            (*idx) = (*idx) + BMI2_FIFO_AUX_LENGTH;

            /* Get virtual sensor time if S4S is enabled */
            if(dev->sens_en_stat & BMI2_EXT_SENS_SEL)
            {
                unpack_virt_aux_sensor_time(&aux[(*aux_idx)], idx, fifo);
            }

            /* Update auxiliary frame index */
            (*aux_idx)++;

            /* More frames could be read */
            rslt = BMI2_W_PARTIAL_READ;
            break;

        /* If frame contains accelerometer and auxiliary data */
        case BMI2_FIFO_HEADER_AUX_ACC_FRM:
        case BMI2_FIFO_HEAD_LESS_AUX_ACC_FRM:

            /* Partially read, then skip the data */
            if(((*idx) + fifo->acc_aux_frm_len) > fifo->length)
            {
                /* Move the data index to the last byte */
                (*idx) = fifo->length;

                /* FIFO is empty */
                rslt = BMI2_W_FIFO_EMPTY;
                break;
            }

            /* Get the auxiliary data */
            unpack_aux_data(&aux[(*aux_idx)], (*idx), fifo);

            /* Update data index */
            (*idx) = (*idx) + BMI2_FIFO_ACC_AUX_LENGTH;

            /* Get virtual sensor time if S4S is enabled */
            if(dev->sens_en_stat & BMI2_EXT_SENS_SEL)
            {
                unpack_virt_aux_sensor_time(&aux[(*aux_idx)], idx, fifo);
            }

            /* Update auxiliary frame index */
            (*aux_idx)++;

            /* More frames could be read */
            rslt = BMI2_W_PARTIAL_READ;
            break;

        /* If frame contains gyroscope and auxiliary data */
        case BMI2_FIFO_HEADER_AUX_GYR_FRM:
        case BMI2_FIFO_HEAD_LESS_GYR_AUX_FRM:

            /* Partially read, then skip the data */
            if(((*idx) + fifo->aux_gyr_frm_len) > fifo->length)
            {
                /* Move the data index to the last byte */
                (*idx) = fifo->length;

                /* FIFO is empty */
                rslt = BMI2_W_FIFO_EMPTY;
                break;
            }

            /* Get the auxiliary data */
            unpack_aux_data(&aux[(*aux_idx)], (*idx), fifo);

            /* Update data index */
            (*idx) = (*idx) + BMI2_FIFO_GYR_AUX_LENGTH;

            /* Get virtual sensor time if S4S is enabled */
            if(dev->sens_en_stat & BMI2_EXT_SENS_SEL)
            {
                unpack_virt_aux_sensor_time(&aux[(*aux_idx)], idx, fifo);
            }

            /* Update auxiliary frame index */
            (*aux_idx)++;

            /* More frames could be read */
            rslt = BMI2_W_PARTIAL_READ;
            break;

        /* If frame contains accelerometer, gyroscope and auxiliary data */
        case BMI2_FIFO_HEADER_ALL_FRM:
        case BMI2_FIFO_HEAD_LESS_ALL_FRM:

            /* Partially read, then skip the data */
            if((*idx + fifo->all_frm_len) > fifo->length)
            {
                /* Move the data index to the last byte */
                (*idx) = fifo->length;

                /* FIFO is empty */
                rslt = BMI2_W_FIFO_EMPTY;
                break;
            }

            /* Get the auxiliary data */
            unpack_aux_data(&aux[(*aux_idx)], (*idx), fifo);

            /* Update data index */
            (*idx) = (*idx) + BMI2_FIFO_ALL_LENGTH;

            /* Get virtual sensor time if S4S is enabled */
            if(dev->sens_en_stat & BMI2_EXT_SENS_SEL)
            {
                unpack_virt_aux_sensor_time(&aux[(*aux_idx)], idx, fifo);
            }

            /* Update auxiliary frame index */
            (*aux_idx)++;

            /* More frames could be read */
            rslt = BMI2_W_PARTIAL_READ;
            break;

        /* If frame contains only accelerometer data */
        case BMI2_FIFO_HEADER_ACC_FRM:
        case BMI2_FIFO_HEAD_LESS_ACC_FRM:

            /* Update data index */
            (*idx) = (*idx) + fifo->acc_frm_len;

            /* More frames could be read */
            rslt = BMI2_W_PARTIAL_READ;
            break;

        /* If frame contains only gyroscope data */
        case BMI2_FIFO_HEADER_GYR_FRM:
        case BMI2_FIFO_HEAD_LESS_GYR_FRM:

            /* Update data index */
            (*idx) = (*idx) + fifo->gyr_frm_len;

            /* More frames could be read */
            rslt = BMI2_W_PARTIAL_READ;
            break;

        /* If frame contains accelerometer and gyroscope data */
        case BMI2_FIFO_HEADER_GYR_ACC_FRM:
        case BMI2_FIFO_HEAD_LESS_GYR_ACC_FRM:

            /* Update data index */
            (*idx) = (*idx) + fifo->acc_gyr_frm_len;

            /* More frames could be read */
            rslt = BMI2_W_PARTIAL_READ;
            break;
        default:

            /* Move the data index to the last byte in case of
             * invalid values
             */
            (*idx) = fifo->length;

            /* FIFO is empty */
            rslt = BMI2_W_FIFO_EMPTY;
            break;
    }

    return rslt;
}

/*!
 * @brief This internal API is used to parse auxiliary data from the FIFO data.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C6AXISTRACKINGBMI270::unpack_aux_data(struct bmi2_aux_fifo_data *aux, XWORD data_start_index, const struct bmi2_fifo_frame *fifo)
* @brief      Unpack aux data
* @ingroup    DATAIO
* 
* @param[in]  aux : 
* @param[in]  data_start_index : 
* @param[in]  fifo : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C6AXISTRACKINGBMI270::unpack_aux_data(struct bmi2_aux_fifo_data *aux, XWORD data_start_index, const struct bmi2_fifo_frame *fifo)
{
    /* Variables to store index */
    XWORD idx = 0;

    /* Get auxiliary data */
    for (; idx < 8; idx++)
    {
        aux->data[idx] = fifo->data[data_start_index++];
    }
}

/*!
 * @brief This internal API parses virtual frame header from the FIFO data.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C6AXISTRACKINGBMI270::parse_if_virtual_header(XBYTE *frame_header, XWORD *data_index, const struct bmi2_fifo_frame *fifo)
* @brief      Parse if virtual header
* @ingroup    DATAIO
* 
* @param[in]  frame_header : 
* @param[in]  data_index : 
* @param[in]  fifo : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C6AXISTRACKINGBMI270::parse_if_virtual_header(XBYTE *frame_header, XWORD *data_index, const struct bmi2_fifo_frame *fifo)
{
    /* Variable to extract virtual header byte */
    XBYTE virtual_header_mode;

    /* Extract virtual header mode from the frame header */
    virtual_header_mode = BMI2_GET_BITS(*frame_header, BMI2_FIFO_VIRT_FRM_MODE);

    /* If the extracted header byte is a virtual header */
    if(virtual_header_mode == BMI2_FIFO_VIRT_FRM_MODE)
    {
        /* If frame header is not activity recognition header */
        if(*frame_header != 0xC8)
        {
            /* Index shifted to next byte where sensor frame is present */
            (*data_index) = (*data_index) + 1;

            /* Get the sensor frame header */
            *frame_header = fifo->data[*data_index] & BMI2_FIFO_TAG_INTR_MASK;
        }
    }
}

/*!
 * @brief This internal API gets sensor time from the accelerometer and
 * gyroscope virtual frames and updates in the data structure.
 */
 void DIOI2C6AXISTRACKINGBMI270::unpack_virt_sensor_time(struct bmi2_sens_axes_data *sens, XWORD *idx, const struct bmi2_fifo_frame *fifo)
{
    /* Variables to define 3 bytes of sensor time */
    XDWORD sensor_time_byte3;
    XWORD sensor_time_byte2;
    XBYTE sensor_time_byte1;

    /* Get sensor time from the FIFO data */
    sensor_time_byte3 = (XDWORD)(fifo->data[(*idx) + BMI2_SENSOR_TIME_MSB_BYTE] << 16);
    sensor_time_byte2 = (XWORD) fifo->data[(*idx) + BMI2_SENSOR_TIME_XLSB_BYTE] << 8;
    sensor_time_byte1 = fifo->data[(*idx)];

    /* Store sensor time in the sensor data structure */
    sens->virt_sens_time = (XDWORD)(sensor_time_byte3 | sensor_time_byte2 | sensor_time_byte1);

    /* Move the data index by 3 bytes */
    (*idx) = (*idx) + BMI2_SENSOR_TIME_LENGTH;
}

/*!
 * @brief This internal API gets sensor time from the auxiliary virtual
 * frames and updates in the data structure.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C6AXISTRACKINGBMI270::unpack_virt_aux_sensor_time(struct bmi2_aux_fifo_data *aux, XWORD *idx, const struct bmi2_fifo_frame *fifo)
* @brief      Unpack virt aux sensor time
* @ingroup    DATAIO
* 
* @param[in]  aux : 
* @param[in]  idx : 
* @param[in]  fifo : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C6AXISTRACKINGBMI270::unpack_virt_aux_sensor_time(struct bmi2_aux_fifo_data *aux, XWORD *idx, const struct bmi2_fifo_frame *fifo)
{
    /* Variables to define 3 bytes of sensor time */
    XDWORD sensor_time_byte3;
    XWORD sensor_time_byte2;
    XBYTE sensor_time_byte1;

    /* Get sensor time from the FIFO data */
    sensor_time_byte3 = (XDWORD)(fifo->data[(*idx) + BMI2_SENSOR_TIME_MSB_BYTE] << 16);
    sensor_time_byte2 = (XWORD) fifo->data[(*idx) + BMI2_SENSOR_TIME_XLSB_BYTE] << 8;
    sensor_time_byte1 = fifo->data[(*idx)];

    /* Store sensor time in the sensor data structure */
    aux->virt_sens_time = (XDWORD)(sensor_time_byte3 | sensor_time_byte2 | sensor_time_byte1);

    /* Move the data index by 3 bytes */
    (*idx) = (*idx) + BMI2_SENSOR_TIME_LENGTH;
}

/*!
 * @brief This internal API is used to reset the FIFO related configurations in
 * the FIFO frame structure for the next FIFO read.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C6AXISTRACKINGBMI270::reset_fifo_frame_structure(struct bmi2_fifo_frame *fifo, const struct bmi2_dev *dev)
* @brief      Reset fifo frame structure
* @ingroup    DATAIO
* 
* @param[in]  fifo : 
* @param[in]  dev : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C6AXISTRACKINGBMI270::reset_fifo_frame_structure(struct bmi2_fifo_frame *fifo, const struct bmi2_dev *dev)
{
    /* Reset FIFO data structure */
    fifo->acc_byte_start_idx = 0;
    fifo->aux_byte_start_idx = 0;
    fifo->gyr_byte_start_idx = 0;
    fifo->sensor_time = 0;
    fifo->skipped_frame_count = 0;
    fifo->act_recog_byte_start_idx = 0;

    /* If S4S is enabled */
    if((dev->sens_en_stat & BMI2_EXT_SENS_SEL) == BMI2_EXT_SENS_SEL)
    {
        fifo->acc_frm_len = BMI2_FIFO_VIRT_ACC_LENGTH;
        fifo->gyr_frm_len = BMI2_FIFO_VIRT_GYR_LENGTH;
        fifo->aux_frm_len = BMI2_FIFO_VIRT_AUX_LENGTH;
        fifo->acc_gyr_frm_len = BMI2_FIFO_VIRT_ACC_GYR_LENGTH;
        fifo->acc_aux_frm_len = BMI2_FIFO_VIRT_ACC_AUX_LENGTH;
        fifo->aux_gyr_frm_len = BMI2_FIFO_VIRT_GYR_AUX_LENGTH;
        fifo->all_frm_len = BMI2_FIFO_VIRT_ALL_LENGTH;

        /* If S4S is not enabled */
    }
    else
    {
        fifo->acc_frm_len = BMI2_FIFO_ACC_LENGTH;
        fifo->gyr_frm_len = BMI2_FIFO_GYR_LENGTH;
        fifo->aux_frm_len = BMI2_FIFO_AUX_LENGTH;
        fifo->acc_gyr_frm_len = BMI2_FIFO_ACC_GYR_LENGTH;
        fifo->acc_aux_frm_len = BMI2_FIFO_ACC_AUX_LENGTH;
        fifo->aux_gyr_frm_len = BMI2_FIFO_GYR_AUX_LENGTH;
        fifo->all_frm_len = BMI2_FIFO_ALL_LENGTH;
    }
}

/*!
 * @brief This API internal checks whether the FIFO data read is an empty frame.
 * If empty frame, index is moved to the last byte.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::check_empty_fifo(XWORD *data_index, const struct bmi2_fifo_frame *fifo)
* @brief      Check empty fifo
* @ingroup    DATAIO
* 
* @param[in]  data_index : 
* @param[in]  fifo : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::check_empty_fifo(XWORD *data_index, const struct bmi2_fifo_frame *fifo)
{
    /* Variables to define error */
    XBYTESIG rslt = BMI2_OK;

    /* Validate data index */
    if(((*data_index) + 6) < fifo->length)
    {
        /* Check if FIFO is empty */
        if(((fifo->data[(*data_index)] == BMI2_FIFO_MSB_CONFIG_CHECK) &&
             (fifo->data[(*data_index) + 1] == BMI2_FIFO_LSB_CONFIG_CHECK)) &&
            ((fifo->data[(*data_index) + 2] == BMI2_FIFO_MSB_CONFIG_CHECK) &&
             (fifo->data[(*data_index) + 3] == BMI2_FIFO_LSB_CONFIG_CHECK)) &&
            ((fifo->data[(*data_index) + 4] == BMI2_FIFO_MSB_CONFIG_CHECK) &&
             (fifo->data[(*data_index) + 5] == BMI2_FIFO_LSB_CONFIG_CHECK)))
        {
            /* Move the data index to the last byte to mark completion */
            (*data_index) = fifo->length;

            /* FIFO is empty */
            rslt = BMI2_W_FIFO_EMPTY;
        }
        else
        {
            /* More frames could be read */
            rslt = BMI2_W_PARTIAL_READ;
        }
    }

    return rslt;
}

/*!
 * @brief This internal API is used to move the data index ahead of the
 * current_frame_length parameter when unnecessary FIFO data appears while
 * extracting the user specified data.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::move_next_frame(XWORD *data_index, XBYTE current_frame_length, const struct bmi2_fifo_frame *fifo)
* @brief      Move next frame
* @ingroup    DATAIO
* 
* @param[in]  data_index : 
* @param[in]  current_frame_length : 
* @param[in]  fifo : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::move_next_frame(XWORD *data_index, XBYTE current_frame_length, const struct bmi2_fifo_frame *fifo)
{
    /* Variables to define error */
    XBYTESIG rslt = BMI2_OK;

    /* Validate data index */
    if(((*data_index) + current_frame_length) > fifo->length)
    {
        /* Move the data index to the last byte */
        (*data_index) = fifo->length;

        /* FIFO is empty */
        rslt = BMI2_W_FIFO_EMPTY;
    }
    else
    {
        /* Move the data index to next frame */
        (*data_index) = (*data_index) + current_frame_length;

        /* More frames could be read */
        rslt = BMI2_W_PARTIAL_READ;
    }

    return rslt;
}

/*!
 * @brief This internal API is used to parse and store the sensor time from the
 * FIFO data.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::unpack_sensortime_frame(XWORD *data_index, struct bmi2_fifo_frame *fifo)
* @brief      Unpack sensortime frame
* @ingroup    DATAIO
* 
* @param[in]  data_index : 
* @param[in]  fifo : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::unpack_sensortime_frame(XWORD *data_index, struct bmi2_fifo_frame *fifo)
{
    /* Variables to define error */
    XBYTESIG rslt = BMI2_OK;

    /* Variables to define 3 bytes of sensor time */
    XDWORD sensor_time_byte3 = 0;
    XWORD sensor_time_byte2 = 0;
    XBYTE sensor_time_byte1 = 0;

    /* Validate data index */
    if(((*data_index) + BMI2_SENSOR_TIME_LENGTH) > fifo->length)
    {
        /* Move the data index to the last byte */
        (*data_index) = fifo->length;

        /* FIFO is empty */
        rslt = BMI2_W_FIFO_EMPTY;
    }
    else
    {
        /* Get sensor time from the FIFO data */
        sensor_time_byte3 = fifo->data[(*data_index) + BMI2_SENSOR_TIME_MSB_BYTE] << 16;
        sensor_time_byte2 = fifo->data[(*data_index) + BMI2_SENSOR_TIME_XLSB_BYTE] << 8;
        sensor_time_byte1 = fifo->data[(*data_index)];

        /* Update sensor time in the FIFO structure */
        fifo->sensor_time = (XDWORD)(sensor_time_byte3 | sensor_time_byte2 | sensor_time_byte1);

        /* Move the data index by 3 bytes */
        (*data_index) = (*data_index) + BMI2_SENSOR_TIME_LENGTH;

        /* More frames could be read */
        rslt = BMI2_W_PARTIAL_READ;
    }

    return rslt;
}

/*!
 * @brief This internal API is used to parse and store the skipped frame count
 * from the FIFO data.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::unpack_skipped_frame(XWORD *data_index, struct bmi2_fifo_frame *fifo)
* @brief      Unpack skipped frame
* @ingroup    DATAIO
* 
* @param[in]  data_index : 
* @param[in]  fifo : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::unpack_skipped_frame(XWORD *data_index, struct bmi2_fifo_frame *fifo)
{
    /* Variables to define error */
    XBYTESIG rslt = BMI2_OK;

    /* Validate data index */
    if((*data_index) >= fifo->length)
    {
        /* Update the data index to the last byte */
        (*data_index) = fifo->length;

        /* FIFO is empty */
        rslt = BMI2_W_FIFO_EMPTY;
    }
    else
    {
        /* Update skipped frame count in the FIFO structure */
        fifo->skipped_frame_count = fifo->data[(*data_index)];

        /* Move the data index by 1 byte */
        (*data_index) = (*data_index) + 1;

        /* More frames could be read */
        rslt = BMI2_W_PARTIAL_READ;
    }

    return rslt;
}

/*!
 * @brief This internal API enables and configures the accelerometer which is
 * needed for self-test operation. It also sets the amplitude for the self-test.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::pre_self_test_config(struct bmi2_dev *dev)
* @brief      Pre self test config
* @ingroup    DATAIO
* 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::pre_self_test_config(struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Structure to define sensor configurations */
    struct bmi2_sens_config sens_cfg;

    /* List the sensors to be selected */
    XBYTE sens_sel = BMI2_ACCEL;

    /* Enable accelerometer */
    rslt = bmi2_sensor_enable(&sens_sel, 1, dev);
    bmi2_delay_us(1000, dev->intf_ptr);

    /* Enable self-test amplitude */
    if(rslt == BMI2_OK)
    {
        rslt = set_accel_self_test_amp(BMI2_ENABLE, dev);
    }

    if(rslt == BMI2_OK)
    {
        /* Select accelerometer for sensor configurations */
        sens_cfg.type = BMI2_ACCEL;

        /* Get the default values */
        rslt = bmi2_get_sensor_config(&sens_cfg, 1, dev);
        if(rslt == BMI2_OK)
        {
            /* Set the configurations required for self-test */
            sens_cfg.cfg.acc.odr = BMI2_ACC_ODR_1600HZ;
            sens_cfg.cfg.acc.bwp = BMI2_ACC_NORMAL_AVG4;
            sens_cfg.cfg.acc.filter_perf = BMI2_PERF_OPT_MODE;
            sens_cfg.cfg.acc.range = BMI2_ACC_RANGE_16G;

            /* Set accelerometer configurations */
            rslt = bmi2_set_sensor_config(&sens_cfg, 1, dev);
        }
    }

    return rslt;
}

/*!
 * @brief This internal API performs the steps needed for self-test operation
 * before reading the accelerometer self-test data.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::self_test_config(XBYTE sign, struct bmi2_dev *dev)
* @brief      Self test config
* @ingroup    DATAIO
* 
* @param[in]  sign : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::self_test_config(XBYTE sign, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Enable the accelerometer self-test feature */
    rslt = set_accel_self_test_enable(BMI2_ENABLE, dev);
    if(rslt == BMI2_OK)
    {
        /* Selects the sign of accelerometer self-test excitation */
        rslt = set_acc_self_test_sign(sign, dev);
    }

    return rslt;
}

/*!
 * @brief This internal API enables or disables the accelerometer self-test
 * feature in the sensor.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_accel_self_test_enable(XBYTE enable, struct bmi2_dev *dev)
* @brief      Set accel self test enable
* @ingroup    DATAIO
* 
* @param[in]  enable : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_accel_self_test_enable(XBYTE enable, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to define data */
    XBYTE data = 0;

    /* Enable/Disable self-test feature */
    rslt = bmi2_get_regs(BMI2_ACC_SELF_TEST_ADDR, &data, 1, dev);
    if(rslt == BMI2_OK)
    {
        data = BMI2_SET_BIT_POS0(data, BMI2_ACC_SELF_TEST_EN, enable);
        rslt = bmi2_set_regs(BMI2_ACC_SELF_TEST_ADDR, &data, 1, dev);
    }

    return rslt;
}

/*!
 * @brief This internal API selects the sign for accelerometer self-test
 * excitation.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_acc_self_test_sign(XBYTE sign, struct bmi2_dev *dev)
* @brief      Set acc self test sign
* @ingroup    DATAIO
* 
* @param[in]  sign : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_acc_self_test_sign(XBYTE sign, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to define data */
    XBYTE data = 0;

    /* Select the sign for self-test excitation */
    rslt = bmi2_get_regs(BMI2_ACC_SELF_TEST_ADDR, &data, 1, dev);
    if(rslt == BMI2_OK)
    {
        data = BMI2_SET_BITS(data, BMI2_ACC_SELF_TEST_SIGN, sign);
        rslt = bmi2_set_regs(BMI2_ACC_SELF_TEST_ADDR, &data, 1, dev);
    }

    return rslt;
}

/*!
 * @brief This internal API sets the amplitude of the accelerometer self-test
 * deflection in the sensor.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_accel_self_test_amp(XBYTE amp, struct bmi2_dev *dev)
* @brief      Set accel self test amp
* @ingroup    DATAIO
* 
* @param[in]  amp : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_accel_self_test_amp(XBYTE amp, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to define data */
    XBYTE data = 0;

    /* Select amplitude of the self-test deflection */
    rslt = bmi2_get_regs(BMI2_ACC_SELF_TEST_ADDR, &data, 1, dev);
    if(rslt == BMI2_OK)
    {
        data = BMI2_SET_BITS(data, BMI2_ACC_SELF_TEST_AMP, amp);
        rslt = bmi2_set_regs(BMI2_ACC_SELF_TEST_ADDR, &data, 1, dev);
    }

    return rslt;
}

/*!
 * @brief This internal API reads the accelerometer data for x,y and z axis from
 * the sensor. The data units is in LSB format.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::read_accel_xyz(struct bmi2_sens_axes_data *accel, struct bmi2_dev *dev)
* @brief      Read accel xyz
* @ingroup    DATAIO
* 
* @param[in]  accel : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::read_accel_xyz(struct bmi2_sens_axes_data *accel, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to define LSB */
    XWORD lsb = 0;

    /* Variable to define MSB */
    XWORD msb = 0;

    /* Array to define data buffer */
    XBYTE data[BMI2_ACC_GYR_NUM_BYTES] = { 0 };

    rslt = bmi2_get_regs(BMI2_ACC_X_LSB_ADDR, data, BMI2_ACC_GYR_NUM_BYTES, dev);
    if(rslt == BMI2_OK)
    {
        /* Accelerometer data x axis */
        msb = data[1];
        lsb = data[0];
        accel->x = (XWORDSIG)((msb << 8) | lsb);

        /* Accelerometer data y axis */
        msb = data[3];
        lsb = data[2];
        accel->y = (XWORDSIG)((msb << 8) | lsb);

        /* Accelerometer data z axis */
        msb = data[5];
        lsb = data[4];
        accel->z = (XWORDSIG)((msb << 8) | lsb);
    }

    return rslt;
}

/*!
 * @brief This internal API reads the gyroscope data for x, y and z axis from
 * the sensor. The data units is in LSB format.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::read_gyro_xyz(struct bmi2_sens_axes_data *gyro, struct bmi2_dev *dev)
* @brief      Read gyro xyz
* @ingroup    DATAIO
* 
* @param[in]  gyro : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::read_gyro_xyz(struct bmi2_sens_axes_data *gyro, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to define LSB */
    XWORD lsb = 0;

    /* Variable to define MSB */
    XWORD msb = 0;

    /* Array to define data buffer */
    XBYTE data[BMI2_ACC_GYR_NUM_BYTES] = { 0 };

    rslt = bmi2_get_regs(BMI2_GYR_X_LSB_ADDR, data, BMI2_ACC_GYR_NUM_BYTES, dev);
    if(rslt == BMI2_OK)
    {
        /* Gyroscope data x axis */
        msb = data[1];
        lsb = data[0];
        gyro->x = (XWORDSIG)((msb << 8) | lsb);

        /* Gyroscope data y axis */
        msb = data[3];
        lsb = data[2];
        gyro->y = (XWORDSIG)((msb << 8) | lsb);

        /* Gyroscope data z axis */
        msb = data[5];
        lsb = data[4];
        gyro->z = (XWORDSIG)((msb << 8) | lsb);
    }

    return rslt;
}

/*!
 * @brief This internal API converts LSB value of accelerometer axes to form
 * 'g' to 'mg' for self-test.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C6AXISTRACKINGBMI270::convert_lsb_g(const struct bmi2_selftest_delta_limit *acc_data_diff, struct bmi2_selftest_delta_limit *acc_data_diff_mg, const struct bmi2_dev *dev)
* @brief      Convert lsb g
* @ingroup    DATAIO
* 
* @param[in]  acc_data_diff : 
* @param[in]  acc_data_diff_mg : 
* @param[in]  dev : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C6AXISTRACKINGBMI270::convert_lsb_g(const struct bmi2_selftest_delta_limit *acc_data_diff, struct bmi2_selftest_delta_limit *acc_data_diff_mg, const struct bmi2_dev *dev)
{
    /* Variable to define LSB/g value of axes */
    XDWORD lsb_per_g;

    /* Range considered for self-test is +/-16g */
    XBYTE range = BMI2_ACC_SELF_TEST_RANGE;

    /* lsb_per_g for the respective resolution and 16g range */
    lsb_per_g = (XDWORD)(power(2, dev->resolution) / (2 * range));

    /* Accelerometer x value in mg */
    acc_data_diff_mg->x = (acc_data_diff->x / (XDWORDSIG) lsb_per_g) * 1000;

    /* Accelerometer y value in mg */
    acc_data_diff_mg->y = (acc_data_diff->y / (XDWORDSIG) lsb_per_g) * 1000;

    /* Accelerometer z value in mg */
    acc_data_diff_mg->z = (acc_data_diff->z / (XDWORDSIG) lsb_per_g) * 1000;
}

/*!
 * @brief This internal API is used to calculate the power of a value.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORDSIG DIOI2C6AXISTRACKINGBMI270::power(XWORDSIG base, XBYTE resolution)
* @brief      Power
* @ingroup    DATAIO
* 
* @param[in]  base : 
* @param[in]  resolution : 
* 
* @return     XDWORDSIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORDSIG DIOI2C6AXISTRACKINGBMI270::power(XWORDSIG base, XBYTE resolution)
{
    /* Initialize loop */
    XBYTE loop = 1;

    /* Initialize variable to store the power of 2 value */
    XDWORDSIG value = 1;

    for (; loop <= resolution; loop++)
    {
        value = (XDWORDSIG)(value * base);
    }

    return value;
}

/*!
 * @brief This internal API validates the accelerometer self-test data and
 * decides the result of self-test operation.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::validate_self_test(const struct bmi2_selftest_delta_limit *accel_data_diff)
* @brief      Validate self test
* @ingroup    DATAIO
* 
* @param[in]  accel_data_diff : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::validate_self_test(const struct bmi2_selftest_delta_limit *accel_data_diff)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* As per the data sheet, The actually measured signal differences should be significantly
     * larger than the minimum differences for each axis in order for the self-test to pass.
     */
    if((accel_data_diff->x > BMI2_ST_ACC_X_SIG_MIN_DIFF) && (accel_data_diff->y < BMI2_ST_ACC_Y_SIG_MIN_DIFF) &&
        (accel_data_diff->z > BMI2_ST_ACC_Z_SIG_MIN_DIFF))
    {
        /* Self-test pass */
        rslt = BMI2_OK;
    }
    else
    {
        /* Self-test fail*/
        rslt = BMI2_E_SELF_TEST_FAIL;
    }

    return rslt;
}

/*!
 * @brief This internal API gets the re-mapped x, y and z axes from the sensor.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_remap_axes(struct bmi2_axes_remap *remap, struct bmi2_dev *dev)
* @brief      Get remap axes
* @ingroup    DATAIO
* 
* @param[in]  remap : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_remap_axes(struct bmi2_axes_remap *remap, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt = BMI2_OK;

    /* Array to define the feature configuration */
    XBYTE feat_config[BMI2_FEAT_SIZE_IN_BYTES] = { 0 };

    /* Variable to define the array offset */
    XBYTE idx = 0;

    /* Variable to set flag */
    XBYTE feat_found;

    /* Initialize feature configuration for axis re-mapping */
    struct bmi2_feature_config remap_config = { 0, 0, 0 };

    /* Variable to get the status of advance power save */
    XBYTE aps_stat;

    /* Get status of advance power save mode */
    aps_stat = dev->aps_status;
    if(aps_stat == BMI2_ENABLE)
    {
        /* Disable advance power save if enabled */
        rslt = bmi2_set_adv_power_save(BMI2_DISABLE, dev);
    }

    if(rslt == BMI2_OK)
    {
        /* Search for axis re-mapping and extract its configuration details */
        feat_found = bmi2_extract_input_feat_config(&remap_config, BMI2_AXIS_MAP, dev);
        if(feat_found)
        {
            rslt = bmi2_get_feat_config(remap_config.page, feat_config, dev);
            if(rslt == BMI2_OK)
            {
                /* Define the offset for axis re-mapping */
                idx = remap_config.start_addr;

                /* Get the re-mapped x-axis */
                remap->x_axis = BMI2_GET_BIT_POS0(feat_config[idx], BMI2_X_AXIS);

                /* Get the re-mapped x-axis polarity */
                remap->x_axis_sign = BMI2_GET_BITS(feat_config[idx], BMI2_X_AXIS_SIGN);

                /* Get the re-mapped y-axis */
                remap->y_axis = BMI2_GET_BITS(feat_config[idx], BMI2_Y_AXIS);

                /* Get the re-mapped y-axis polarity */
                remap->y_axis_sign = BMI2_GET_BITS(feat_config[idx], BMI2_Y_AXIS_SIGN);

                /* Get the re-mapped z-axis */
                remap->z_axis = BMI2_GET_BITS(feat_config[idx], BMI2_Z_AXIS);

                /* Increment byte to fetch the next data */
                idx++;

                /* Get the re-mapped z-axis polarity */
                remap->z_axis_sign = BMI2_GET_BIT_POS0(feat_config[idx], BMI2_Z_AXIS_SIGN);
            }
        }
        else
        {
            rslt = BMI2_E_INVALID_SENSOR;
        }

        /* Enable Advance power save if disabled while configuring and
         * not when already disabled
         */
        if((aps_stat == BMI2_ENABLE) && (rslt == BMI2_OK))
        {
            rslt = bmi2_set_adv_power_save(BMI2_ENABLE, dev);
        }
    }

    return rslt;
}

/*!
 * @brief This internal API sets the re-mapped x, y and z axes in the sensor.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_remap_axes(const struct bmi2_axes_remap *remap, struct bmi2_dev *dev)
* @brief      Set remap axes
* @ingroup    DATAIO
* 
* @param[in]  remap : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_remap_axes(const struct bmi2_axes_remap *remap, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt = BMI2_OK;

    /* Array to define the feature configuration */
    XBYTE feat_config[BMI2_FEAT_SIZE_IN_BYTES] = { 0 };

    /* Variable to define the array offset */
    XBYTE idx = 0;

    /* Variable to define the register address */
    XBYTE reg_addr = 0;

    /* Variable to set the re-mapped x-axes in the sensor */
    XBYTE x_axis = 0;

    /* Variable to set the re-mapped y-axes in the sensor */
    XBYTE y_axis = 0;

    /* Variable to set the re-mapped z-axes in the sensor */
    XBYTE z_axis = 0;

    /* Variable to set the re-mapped x-axes sign in the sensor */
    XBYTE x_axis_sign = 0;

    /* Variable to set the re-mapped y-axes sign in the sensor */
    XBYTE y_axis_sign = 0;

    /* Variable to set the re-mapped z-axes sign in the sensor */
    XBYTE z_axis_sign = 0;

    /* Variable to set flag */
    XBYTE feat_found;

    /* Initialize feature configuration for axis re-mapping */
    struct bmi2_feature_config remap_config = { 0, 0, 0 };

    /* Variable to get the status of advance power save */
    XBYTE aps_stat;

    /* Get status of advance power save mode */
    aps_stat = dev->aps_status;
    if(aps_stat == BMI2_ENABLE)
    {
        /* Disable advance power save if enabled */
        rslt = bmi2_set_adv_power_save(BMI2_DISABLE, dev);
    }

    if(rslt == BMI2_OK)
    {
        /* Search for axis-re-mapping and extract its configuration details */
        feat_found = bmi2_extract_input_feat_config(&remap_config, BMI2_AXIS_MAP, dev);
        if(feat_found)
        {
            /* Get the configuration from the page where axis re-mapping feature resides */
            rslt = bmi2_get_feat_config(remap_config.page, feat_config, dev);
            if(rslt == BMI2_OK)
            {
                /* Define the offset in bytes */
                idx = remap_config.start_addr;

                /* Set the value of re-mapped x-axis */
                x_axis = remap->x_axis & BMI2_X_AXIS_MASK;

                /* Set the value of re-mapped x-axis sign */
                x_axis_sign = ((remap->x_axis_sign << BMI2_X_AXIS_SIGN_POS) & BMI2_X_AXIS_SIGN_MASK);

                /* Set the value of re-mapped y-axis */
                y_axis = ((remap->y_axis << BMI2_Y_AXIS_POS) & BMI2_Y_AXIS_MASK);

                /* Set the value of re-mapped y-axis sign */
                y_axis_sign = ((remap->y_axis_sign << BMI2_Y_AXIS_SIGN_POS) & BMI2_Y_AXIS_SIGN_MASK);

                /* Set the value of re-mapped z-axis */
                z_axis = ((remap->z_axis << BMI2_Z_AXIS_POS) & BMI2_Z_AXIS_MASK);

                /* Set the value of re-mapped z-axis sign */
                z_axis_sign = remap->z_axis_sign & BMI2_Z_AXIS_SIGN_MASK;

                /* Arrange axes in the first byte */
                feat_config[idx] = x_axis | x_axis_sign | y_axis | y_axis_sign | z_axis;

                /* Increment the index */
                idx++;

                /* Cannot OR in the second byte since it holds
                 * gyroscope self-offset correction bit
                 */
                feat_config[idx] = BMI2_SET_BIT_POS0(feat_config[idx], BMI2_Z_AXIS_SIGN, z_axis_sign);

                /* Update the register address */
                reg_addr = BMI2_FEATURES_REG_ADDR + remap_config.start_addr;

                /* Set the configuration back to the page */
                rslt = bmi2_set_regs(reg_addr, &feat_config[remap_config.start_addr], 2, dev);
            }
        }
        else
        {
            rslt = BMI2_E_INVALID_SENSOR;
        }

        /* Enable Advance power save if disabled while configuring and
         * not when already disabled
         */
        if((aps_stat == BMI2_ENABLE) && (rslt == BMI2_OK))
        {
            rslt = bmi2_set_adv_power_save(BMI2_ENABLE, dev);
        }
    }

    return rslt;
}

/*!
 * @brief This internal API corrects the gyroscope cross-axis sensitivity
 * between the z and the x axis.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C6AXISTRACKINGBMI270::comp_gyro_cross_axis_sensitivity(struct bmi2_sens_axes_data *gyr_data, const struct bmi2_dev *dev)
* @brief      Comp gyro cross axis sensitivity
* @ingroup    DATAIO
* 
* @param[in]  gyr_data : 
* @param[in]  dev : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C6AXISTRACKINGBMI270::comp_gyro_cross_axis_sensitivity(struct bmi2_sens_axes_data *gyr_data, const struct bmi2_dev *dev)
{
    /* Get the compensated gyroscope x-axis */
    gyr_data->x = gyr_data->x - (XWORDSIG)(((XDWORDSIG) dev->gyr_cross_sens_zx * (XDWORDSIG) gyr_data->z) / 512);
}

/*!
 * @brief This internal API is used to validate the boundary conditions.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::check_boundary_val(XBYTE *val, XBYTE min, XBYTE max, struct bmi2_dev *dev)
* @brief      Check boundary val
* @ingroup    DATAIO
* 
* @param[in]  val : 
* @param[in]  min : 
* @param[in]  max : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::check_boundary_val(XBYTE *val, XBYTE min, XBYTE max, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt = BMI2_OK;

    if(val != NULL)
    {
        /* Check if value is below minimum value */
        if(*val < min)
        {
            /* Auto correct the invalid value to minimum value */
            *val = min;
            dev->info |= BMI2_I_MIN_VALUE;
        }

        /* Check if value is above maximum value */
        if(*val > max)
        {
            /* Auto correct the invalid value to maximum value */
            *val = max;
            dev->info |= BMI2_I_MAX_VALUE;
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This internal API saves the configurations before performing FOC.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::save_accel_foc_config(struct bmi2_accel_config *acc_cfg, XBYTE *aps, XBYTE *acc_en, struct bmi2_dev *dev)
* @brief      Save accel foc config
* @ingroup    DATAIO
* 
* @param[in]  acc_cfg : 
* @param[in]  aps : 
* @param[in]  acc_en : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::save_accel_foc_config(struct bmi2_accel_config *acc_cfg, XBYTE *aps, XBYTE *acc_en, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to get the status from PWR_CTRL register */
    XBYTE pwr_ctrl_data = 0;

    /* Get accelerometer configurations to be saved */
    rslt = get_accel_config(acc_cfg, dev);
    if(rslt == BMI2_OK)
    {
        /* Get accelerometer enable status to be saved */
        rslt = bmi2_get_regs(BMI2_PWR_CTRL_ADDR, &pwr_ctrl_data, 1, dev);
        *acc_en = BMI2_GET_BITS(pwr_ctrl_data, BMI2_ACC_EN);

        /* Get advance power save mode to be saved */
        if(rslt == BMI2_OK)
        {
            rslt = bmi2_get_adv_power_save(aps, dev);
        }
    }

    return rslt;
}

/*!
 * @brief This internal sets configurations for performing accelerometer FOC.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_accel_foc_config(struct bmi2_dev *dev)
* @brief      Set accel foc config
* @ingroup    DATAIO
* 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_accel_foc_config(struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to select the sensor */
    XBYTE sens_list = BMI2_ACCEL;

    /* Variable to set the accelerometer configuration value */
    XBYTE acc_conf_data = BMI2_FOC_ACC_CONF_VAL;

    /* Disabling offset compensation */
    rslt = set_accel_offset_comp(BMI2_DISABLE, dev);
    if(rslt == BMI2_OK)
    {
        /* Set accelerometer configurations to 50Hz, continuous mode, CIC mode */
        rslt = bmi2_set_regs(BMI2_ACC_CONF_ADDR, &acc_conf_data, 1, dev);
        if(rslt == BMI2_OK)
        {
            /* Set accelerometer to normal mode by enabling it */
            rslt = bmi2_sensor_enable(&sens_list, 1, dev);

            if(rslt == BMI2_OK)
            {
                /* Disable advance power save mode */
                rslt = bmi2_set_adv_power_save(BMI2_DISABLE, dev);
            }
        }
    }

    return rslt;
}

/*!
 * @brief This internal API performs Fast Offset Compensation for accelerometer.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::perform_accel_foc(const struct bmi2_accel_foc_g_value *accel_g_value, const struct bmi2_accel_config *acc_cfg, struct bmi2_dev *dev)
* @brief      Perform accel foc
* @ingroup    DATAIO
* 
* @param[in]  accel_g_value : 
* @param[in]  acc_cfg : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::perform_accel_foc(const struct bmi2_accel_foc_g_value *accel_g_value, const struct bmi2_accel_config *acc_cfg, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt = BMI2_E_INVALID_STATUS;

    /* Variable to define count */
    XBYTE loop;

    /* Variable to store status read from the status register */
    XBYTE reg_status = 0;

    /* Array of structure to store accelerometer data */
    struct bmi2_sens_axes_data accel_value[128] = { { 0 } };

    /* Structure to store accelerometer data temporarily */
    struct bmi2_foc_temp_value temp = { 0, 0, 0 };

    /* Structure to store the average of accelerometer data */
    struct bmi2_sens_axes_data accel_avg = { 0, 0, 0, 0 };

    /* Variable to define LSB per g value */
    XWORD lsb_per_g = 0;

    /* Variable to define range */
    XBYTE range = 0;

    /* Structure to store accelerometer data deviation from ideal value */
    struct bmi2_offset_delta delta = { 0, 0, 0 };

    /* Structure to store accelerometer offset values */
    struct bmi2_accel_offset offset = { 0, 0, 0 };

    /* Variable tries max 5 times for interrupt then generates timeout */
    XBYTE try_cnt;

    for (loop = 0; loop < 128; loop++)
    {
        try_cnt = 5;
        while (try_cnt && (!(reg_status & BMI2_DRDY_ACC)))
        {
            /* 20ms delay for 50Hz ODR */
            bmi2_delay_us(20000, dev->intf_ptr);
            rslt = bmi2_get_status(&reg_status, dev);
            try_cnt--;
        }

        if((rslt == BMI2_OK) && (reg_status & BMI2_DRDY_ACC))
        {
            rslt = read_accel_xyz(&accel_value[loop], dev);
        }

        if(rslt == BMI2_OK)
        {
            rslt = read_accel_xyz(&accel_value[loop], dev);
        }

        if(rslt == BMI2_OK)
        {
            /* Store the data in a temporary structure */
            temp.x = temp.x + (XDWORDSIG)accel_value[loop].x;
            temp.y = temp.y + (XDWORDSIG)accel_value[loop].y;
            temp.z = temp.z + (XDWORDSIG)accel_value[loop].z;
        }
        else
        {
            break;
        }
    }

    if(rslt == BMI2_OK)
    {
        /* Take average of x, y and z data for lesser noise */
        accel_avg.x = (XWORDSIG)(temp.x / 128);
        accel_avg.y = (XWORDSIG)(temp.y / 128);
        accel_avg.z = (XWORDSIG)(temp.z / 128);

        /* Get the exact range value */
        map_accel_range(acc_cfg->range, &range);

        /* Get the smallest possible measurable acceleration level given the range and
         * resolution */
        lsb_per_g = (XWORD)(power(2, dev->resolution) / (2 * range));

        /* Compensate acceleration data against gravity */
        comp_for_gravity(lsb_per_g, accel_g_value, &accel_avg, &delta);

        /* Scale according to offset register resolution */
        scale_accel_offset(range, &delta, &offset);

        /* Invert the accelerometer offset data */
        invert_accel_offset(&offset);

        /* Write offset data in the offset compensation register */
        rslt = write_accel_offset(&offset, dev);

        /* Enable offset compensation */
        if(rslt == BMI2_OK)
        {
            rslt = set_accel_offset_comp(BMI2_ENABLE, dev);
        }
    }

    return rslt;
}

/*!
 * @brief This internal API enables/disables the offset compensation for
 * filtered and un-filtered accelerometer data.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_accel_offset_comp(XBYTE offset_en, struct bmi2_dev *dev)
* @brief      Set accel offset comp
* @ingroup    DATAIO
* 
* @param[in]  offset_en : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_accel_offset_comp(XBYTE offset_en, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to store data */
    XBYTE data = 0;

    /* Enable/Disable offset compensation */
    rslt = bmi2_get_regs(BMI2_NV_CONF_ADDR, &data, 1, dev);
    if(rslt == BMI2_OK)
    {
        data = BMI2_SET_BITS(data, BMI2_NV_ACC_OFFSET, offset_en);
        rslt = bmi2_set_regs(BMI2_NV_CONF_ADDR, &data, 1, dev);
    }

    return rslt;
}

/*!
 * @brief This internal API converts the accelerometer range value into
 * corresponding integer value.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C6AXISTRACKINGBMI270::map_accel_range(XBYTE range_in, XBYTE *range_out)
* @brief      Map accel range
* @ingroup    DATAIO
* 
* @param[in]  range_in : 
* @param[in]  range_out : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C6AXISTRACKINGBMI270::map_accel_range(XBYTE range_in, XBYTE *range_out)
{
    switch (range_in)
    {
        case BMI2_ACC_RANGE_2G:
            *range_out = 2;
            break;
        case BMI2_ACC_RANGE_4G:
            *range_out = 4;
            break;
        case BMI2_ACC_RANGE_8G:
            *range_out = 8;
            break;
        case BMI2_ACC_RANGE_16G:
            *range_out = 16;
            break;
        default:

            /* By default RANGE 8G is set */
            *range_out = 8;
            break;
    }
}

/*!
 * @brief This internal API compensate the accelerometer data against gravity.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C6AXISTRACKINGBMI270::comp_for_gravity(XWORD lsb_per_g, const struct bmi2_accel_foc_g_value *g_val, const struct bmi2_sens_axes_data *data, struct bmi2_offset_delta *comp_data)
* @brief      Comp for gravity
* @ingroup    DATAIO
* 
* @param[in]  lsb_per_g : 
* @param[in]  g_val : 
* @param[in]  data : 
* @param[in]  comp_data : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C6AXISTRACKINGBMI270::comp_for_gravity(XWORD lsb_per_g, const struct bmi2_accel_foc_g_value *g_val, const struct bmi2_sens_axes_data *data, struct bmi2_offset_delta *comp_data)
{
    /* Array to store the accelerometer values in LSB */
    XWORDSIG accel_value_lsb[3] = { 0 };

    /* Convert g-value to LSB */
    accel_value_lsb[BMI2_X_AXIS] = (XWORDSIG)(lsb_per_g * g_val->x);
    accel_value_lsb[BMI2_Y_AXIS] = (XWORDSIG)(lsb_per_g * g_val->y);
    accel_value_lsb[BMI2_Z_AXIS] = (XWORDSIG)(lsb_per_g * g_val->z);

    /* Get the compensated values for X, Y and Z axis */
    comp_data->x = (data->x - accel_value_lsb[BMI2_X_AXIS]);
    comp_data->y = (data->y - accel_value_lsb[BMI2_Y_AXIS]);
    comp_data->z = (data->z - accel_value_lsb[BMI2_Z_AXIS]);
}

/*!
 * @brief This internal API scales the compensated accelerometer data according
 * to the offset register resolution.
 *
 * @note The bit position is always greater than 0 since accelerometer data is
 * 16 bit wide.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C6AXISTRACKINGBMI270::scale_accel_offset(XBYTE range, const struct bmi2_offset_delta *comp_data, struct bmi2_accel_offset *data)
* @brief      Scale accel offset
* @ingroup    DATAIO
* 
* @param[in]  range : 
* @param[in]  comp_data : 
* @param[in]  data : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C6AXISTRACKINGBMI270::scale_accel_offset(XBYTE range, const struct bmi2_offset_delta *comp_data, struct bmi2_accel_offset *data)
{
    /* Variable to store the position of bit having 3.9mg resolution */
    XBYTESIG bit_pos_3_9mg;

    /* Variable to store the position previous of bit having 3.9mg resolution */
    XBYTESIG bit_pos_3_9mg_prev_bit;

    /* Variable to store the round-off value */
    XBYTE round_off;

    /* Find the bit position of 3.9mg */
    bit_pos_3_9mg = get_bit_pos_3_9mg(range);

    /* Round off, consider if the next bit is high */
    bit_pos_3_9mg_prev_bit = bit_pos_3_9mg - 1;
    round_off = (XBYTE)(power(2, ((XBYTE) bit_pos_3_9mg_prev_bit)));

    /* Scale according to offset register resolution */
    data->x = (XBYTE)((comp_data->x + round_off) / power(2, ((XBYTE) bit_pos_3_9mg)));
    data->y = (XBYTE)((comp_data->y + round_off) / power(2, ((XBYTE) bit_pos_3_9mg)));
    data->z = (XBYTE)((comp_data->z + round_off) / power(2, ((XBYTE) bit_pos_3_9mg)));
}

/*!
 * @brief This internal API finds the bit position of 3.9mg according to given
 * range and resolution.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_bit_pos_3_9mg(XBYTE range)
* @brief      Get bit pos 3 9mg
* @ingroup    DATAIO
* 
* @param[in]  range : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_bit_pos_3_9mg(XBYTE range)
{
    /* Variable to store the bit position of 3.9mg resolution */
    XBYTESIG bit_pos_3_9mg;

    /* Variable to shift the bits according to the resolution  */
    XDWORD divisor = 1;

    /* Scaling factor to get the bit position of 3.9 mg resolution */
    XWORDSIG scale_factor = -1;

    /* Variable to store temporary value */
    XWORD temp;

    /* Shift left by the times of resolution */
    divisor = divisor << 16;

    /* Get the bit position to be shifted */
    temp = (XWORD)(divisor / (range * 256));

    /* Get the scaling factor until bit position is shifted to last bit */
    while (temp != 1)
    {
        scale_factor++;
        temp = temp >> 1;
    }

    /* Scaling factor is the bit position of 3.9 mg resolution */
    bit_pos_3_9mg = (XBYTESIG) scale_factor;

    return bit_pos_3_9mg;
}

/*!
 * @brief This internal API inverts the accelerometer offset data.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C6AXISTRACKINGBMI270::invert_accel_offset(struct bmi2_accel_offset *offset_data)
* @brief      Invert accel offset
* @ingroup    DATAIO
* 
* @param[in]  offset_data : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C6AXISTRACKINGBMI270::invert_accel_offset(struct bmi2_accel_offset *offset_data)
{
    /* Get the offset data */
    offset_data->x = (XBYTE)((offset_data->x) * (-1));
    offset_data->y = (XBYTE)((offset_data->y) * (-1));
    offset_data->z = (XBYTE)((offset_data->z) * (-1));
}

/*!
 * @brief This internal API writes the offset data in the offset compensation
 * register.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::write_accel_offset(const struct bmi2_accel_offset *offset, struct bmi2_dev *dev)
* @brief      Write accel offset
* @ingroup    DATAIO
* 
* @param[in]  offset : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::write_accel_offset(const struct bmi2_accel_offset *offset, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Array to store the offset data */
    XBYTE data_array[3] = { 0 };

    data_array[0] = offset->x;
    data_array[1] = offset->y;
    data_array[2] = offset->z;

    /* Offset values are written in the offset register */
    rslt = bmi2_set_regs(BMI2_ACC_OFF_COMP_0_ADDR, data_array, 3, dev);

    return rslt;
}

/*!
 * @brief This internal API restores the configurations saved before performing
 * accelerometer FOC.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::restore_accel_foc_config(struct bmi2_accel_config *acc_cfg, XBYTE aps, XBYTE acc_en, struct bmi2_dev *dev)
* @brief      Restore accel foc config
* @ingroup    DATAIO
* 
* @param[in]  acc_cfg : 
* @param[in]  aps : 
* @param[in]  acc_en : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::restore_accel_foc_config(struct bmi2_accel_config *acc_cfg, XBYTE aps, XBYTE acc_en, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to get the status from PWR_CTRL register */
    XBYTE pwr_ctrl_data = 0;

    /* Restore the saved accelerometer configurations */
    rslt = set_accel_config(acc_cfg, dev);
    if(rslt == BMI2_OK)
    {
        /* Restore the saved accelerometer enable status */
        rslt = bmi2_get_regs(BMI2_PWR_CTRL_ADDR, &pwr_ctrl_data, 1, dev);
        if(rslt == BMI2_OK)
        {
            pwr_ctrl_data = BMI2_SET_BITS(pwr_ctrl_data, BMI2_ACC_EN, acc_en);
            rslt = bmi2_set_regs(BMI2_PWR_CTRL_ADDR, &pwr_ctrl_data, 1, dev);

            /* Restore the saved advance power save */
            if(rslt == BMI2_OK)
            {
                rslt = bmi2_set_adv_power_save(aps, dev);
            }
        }
    }

    return rslt;
}

/*!
 * @brief This internal API sets accelerometer configurations like ODR,
 * bandwidth, performance mode and g-range.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_accel_config(struct bmi2_accel_config *config, struct bmi2_dev *dev)
* @brief      Set accel config
* @ingroup    DATAIO
* 
* @param[in]  config : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_accel_config(struct bmi2_accel_config *config, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to store data */
    XBYTE reg_data;

    /* Array to store the default value of accelerometer configuration
     * reserved registers
     */
    XBYTE data_array[2] = { 0 };

    /* Validate bandwidth and performance mode */
    rslt = validate_bw_perf_mode(&config->bwp, &config->filter_perf, dev);
    if(rslt == BMI2_OK)
    {
        /* Validate ODR and range */
        rslt = validate_odr_range(&config->odr, &config->range, dev);
        if(rslt == BMI2_OK)
        {
            /* Set accelerometer performance mode */
            reg_data = BMI2_SET_BITS(data_array[0], BMI2_ACC_FILTER_PERF_MODE, config->filter_perf);

            /* Set accelerometer bandwidth */
            reg_data = BMI2_SET_BITS(reg_data, BMI2_ACC_BW_PARAM, config->bwp);

            /* Set accelerometer ODR */
            reg_data = BMI2_SET_BIT_POS0(reg_data, BMI2_ACC_ODR, config->odr);

            /* Copy the register data to the array */
            data_array[0] = reg_data;

            /* Set accelerometer range */
            reg_data = BMI2_SET_BIT_POS0(data_array[1], BMI2_ACC_RANGE, config->range);

            /* Copy the register data to the array */
            data_array[1] = reg_data;

            /* Write accelerometer configuration to ACC_CONFand
             * ACC_RANGE registers simultaneously as they lie in consecutive places
             */
            rslt = bmi2_set_regs(BMI2_ACC_CONF_ADDR, data_array, 2, dev);

            /* Get error status to check for invalid configurations */
            if(rslt == BMI2_OK)
            {
                rslt = cfg_error_status(dev);
            }
        }
    }

    return rslt;
}

/*!
 * @brief This internal API sets gyroscope configurations like ODR, bandwidth,
 * low power/high performance mode, performance mode and range. It also
 * maps/un-maps data interrupts to that of hardware interrupt line.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_gyro_config(struct bmi2_gyro_config *config, struct bmi2_dev *dev)
* @brief      Set gyro config
* @ingroup    DATAIO
* 
* @param[in]  config : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_gyro_config(struct bmi2_gyro_config *config, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to store data */
    XBYTE reg_data;

    /* Array to store the default value of gyroscope configuration reserved registers  */
    XBYTE data_array[2] = { 0 };

    /* Validate gyroscope configurations */
    rslt = validate_gyro_config(config, dev);
    if(rslt == BMI2_OK)
    {
        /* Set gyroscope performance mode */
        reg_data = BMI2_SET_BITS(data_array[0], BMI2_GYR_FILTER_PERF_MODE, config->filter_perf);

        /* Set gyroscope noise performance mode */
        reg_data = BMI2_SET_BITS(reg_data, BMI2_GYR_NOISE_PERF_MODE, config->noise_perf);

        /* Set gyroscope bandwidth */
        reg_data = BMI2_SET_BITS(reg_data, BMI2_GYR_BW_PARAM, config->bwp);

        /* Set gyroscope ODR */
        reg_data = BMI2_SET_BIT_POS0(reg_data, BMI2_GYR_ODR, config->odr);

        /* Copy the register data to the array */
        data_array[0] = reg_data;

        /* Set gyroscope OIS range */
        reg_data = BMI2_SET_BITS(data_array[1], BMI2_GYR_OIS_RANGE, config->ois_range);

        /* Set gyroscope range */
        reg_data = BMI2_SET_BIT_POS0(reg_data, BMI2_GYR_RANGE, config->range);

        /* Copy the register data to the array */
        data_array[1] = reg_data;

        /* Write accelerometer configuration to GYR_CONF and GYR_RANGE
         * registers simultaneously as they lie in consecutive places
         */
        rslt = bmi2_set_regs(BMI2_GYR_CONF_ADDR, data_array, 2, dev);

        /* Get error status to check for invalid configurations */
        if(rslt == BMI2_OK)
        {
            rslt = cfg_error_status(dev);
        }
    }

    return rslt;
}

/*!
 * @brief This internal API saves the configurations before performing gyroscope
 * FOC.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::save_gyro_config(struct bmi2_gyro_config *gyr_cfg, XBYTE *aps, XBYTE *gyr_en, struct bmi2_dev *dev)
* @brief      Save gyro config
* @ingroup    DATAIO
* 
* @param[in]  gyr_cfg : 
* @param[in]  aps : 
* @param[in]  gyr_en : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::save_gyro_config(struct bmi2_gyro_config *gyr_cfg, XBYTE *aps, XBYTE *gyr_en, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to get the status from PWR_CTRL register */
    XBYTE pwr_ctrl_data = 0;

    /* Get gyroscope configurations to be saved */
    rslt = get_gyro_config(gyr_cfg, dev);
    if(rslt == BMI2_OK)
    {
        /* Get gyroscope enable status to be saved */
        rslt = bmi2_get_regs(BMI2_PWR_CTRL_ADDR, &pwr_ctrl_data, 1, dev);
        *gyr_en = BMI2_GET_BITS(pwr_ctrl_data, BMI2_GYR_EN);

        /* Get advance power save mode to be saved */
        if(rslt == BMI2_OK)
        {
            rslt = bmi2_get_adv_power_save(aps, dev);
        }
    }

    return rslt;
}

/*!
 * @brief This internal sets configurations for performing gyroscope FOC.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_gyro_foc_config(struct bmi2_dev *dev)
* @brief      Set gyro foc config
* @ingroup    DATAIO
* 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_gyro_foc_config(struct bmi2_dev *dev)
{
    XBYTESIG rslt;

    /* Variable to select the sensor */
    XBYTE sens_list = BMI2_GYRO;

    /* Array to set the gyroscope configuration value (ODR, Performance mode
     * and bandwidth) and gyroscope range
     */
    XBYTE gyr_conf_data[2] = { BMI2_FOC_GYR_CONF_VAL, BMI2_GYR_RANGE_2000 };

    /* Disabling gyroscope offset compensation */
    rslt = bmi2_set_gyro_offset_comp(BMI2_DISABLE, dev);
    if(rslt == BMI2_OK)
    {
        /* Set gyroscope configurations to 25Hz, continuous mode,
         * CIC mode, and 2000 dps range
         */
        rslt = bmi2_set_regs(BMI2_GYR_CONF_ADDR, gyr_conf_data, 2, dev);
        if(rslt == BMI2_OK)
        {
            /* Set gyroscope to normal mode by enabling it */
            rslt = bmi2_sensor_enable(&sens_list, 1, dev);

            if(rslt == BMI2_OK)
            {
                /* Disable advance power save mode */
                rslt = bmi2_set_adv_power_save(BMI2_DISABLE, dev);
            }
        }
    }

    return rslt;
}

/*!
 * @brief This internal API inverts the gyroscope offset data.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C6AXISTRACKINGBMI270::invert_gyro_offset(struct bmi2_sens_axes_data *offset_data)
* @brief      Invert gyro offset
* @ingroup    DATAIO
* 
* @param[in]  offset_data : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C6AXISTRACKINGBMI270::invert_gyro_offset(struct bmi2_sens_axes_data *offset_data)
{
    /* Invert the values */
    offset_data->x = (XWORDSIG)((offset_data->x) * (-1));
    offset_data->y = (XWORDSIG)((offset_data->y) * (-1));
    offset_data->z = (XWORDSIG)((offset_data->z) * (-1));
}

/*!
 * @brief This internal API restores the gyroscope configurations saved
 * before performing FOC.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::restore_gyro_config(struct bmi2_gyro_config *gyr_cfg, XBYTE aps, XBYTE gyr_en, struct bmi2_dev *dev)
* @brief      Restore gyro config
* @ingroup    DATAIO
* 
* @param[in]  gyr_cfg : 
* @param[in]  aps : 
* @param[in]  gyr_en : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::restore_gyro_config(struct bmi2_gyro_config *gyr_cfg, XBYTE aps, XBYTE gyr_en, struct bmi2_dev *dev)
{
    XBYTESIG rslt;
    XBYTE pwr_ctrl_data = 0;

    /* Restore the saved gyroscope configurations */
    rslt = set_gyro_config(gyr_cfg, dev);
    if(rslt == BMI2_OK)
    {
        /* Restore the saved gyroscope enable status */
        rslt = bmi2_get_regs(BMI2_PWR_CTRL_ADDR, &pwr_ctrl_data, 1, dev);
        if(rslt == BMI2_OK)
        {
            pwr_ctrl_data = BMI2_SET_BITS(pwr_ctrl_data, BMI2_GYR_EN, gyr_en);
            rslt = bmi2_set_regs(BMI2_PWR_CTRL_ADDR, &pwr_ctrl_data, 1, dev);

            /* Restore the saved advance power save */
            if(rslt == BMI2_OK)
            {
                rslt = bmi2_set_adv_power_save(aps, dev);
            }
        }
    }

    return rslt;
}

/*!
 * @brief This internal API saturates the gyroscope data value before writing to
 * to 10 bit offset register.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C6AXISTRACKINGBMI270::saturate_gyro_data(struct bmi2_sens_axes_data *gyr_off)
* @brief      Saturate gyro data
* @ingroup    DATAIO
* 
* @param[in]  gyr_off : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C6AXISTRACKINGBMI270::saturate_gyro_data(struct bmi2_sens_axes_data *gyr_off)
{
    if(gyr_off->x > 511)
    {
        gyr_off->x = 511;
    }

    if(gyr_off->x < -512)
    {
        gyr_off->x = -512;
    }

    if(gyr_off->y > 511)
    {
        gyr_off->y = 511;
    }

    if(gyr_off->y < -512)
    {
        gyr_off->y = -512;
    }

    if(gyr_off->z > 511)
    {
        gyr_off->z = 511;
    }

    if(gyr_off->z < -512)
    {
        gyr_off->z = -512;
    }
}

/*!
 * @brief This internal API is used to validate the device structure pointer for
 * null conditions.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::null_ptr_check(const struct bmi2_dev *dev)
* @brief      Null ptr check
* @ingroup    DATAIO
* 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::null_ptr_check(const struct bmi2_dev *dev)
{
    XBYTESIG rslt = BMI2_OK;

    if(dev == NULL /*|| (dev->read == NULL) || (dev->write == NULL) || (dev->delay_us == NULL)*/)
    {
        /* Device structure pointer is not valid */
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This internal API is to get the status of st_status from gry_crt_conf register
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_st_running(XBYTE *st_status, struct bmi2_dev *dev)
* @brief      Get st running
* @ingroup    DATAIO
* 
* @param[in]  st_status : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_st_running(XBYTE *st_status, struct bmi2_dev *dev)
{
    XBYTESIG rslt;
    XBYTE reg_data = 0;

    rslt = null_ptr_check(dev);
    if(rslt == BMI2_OK)
    {
        /* Get the status of crt running */
        rslt = bmi2_get_regs(BMI2_GYR_CRT_CONF_ADDR, &reg_data, 1, dev);
        if(rslt == BMI2_OK)
        {
            (*st_status) = BMI2_GET_BITS(reg_data, BMI2_GYR_CRT_RUNNING);
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API enables/disables the CRT running.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_st_running(XBYTE st_status, struct bmi2_dev *dev)
* @brief      Set st running
* @ingroup    DATAIO
* 
* @param[in]  st_status : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_st_running(XBYTE st_status, struct bmi2_dev *dev)
{
    XBYTESIG rslt;
    XBYTE reg_data = 0;

    rslt = null_ptr_check(dev);
    if(rslt == BMI2_OK)
    {
        rslt = bmi2_get_regs(BMI2_GYR_CRT_CONF_ADDR, &reg_data, 1, dev);
        if(rslt == BMI2_OK)
        {
            reg_data = BMI2_SET_BITS(reg_data, BMI2_GYR_CRT_RUNNING, st_status);
            rslt = bmi2_set_regs(BMI2_GYR_CRT_CONF_ADDR, &reg_data, 1, dev);
        }
    }

    return rslt;
}

/*!
 * @brief This API gets the status of rdy for dl bit.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_rdy_for_dl(XBYTE *rdy_for_dl, struct bmi2_dev *dev)
* @brief      Get rdy for dl
* @ingroup    DATAIO
* 
* @param[in]  rdy_for_dl : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_rdy_for_dl(XBYTE *rdy_for_dl, struct bmi2_dev *dev)
{
    XBYTESIG rslt;
    XBYTE reg_data = 0;

    rslt = null_ptr_check(dev);
    if(rslt == BMI2_OK)
    {
        /* Get the status of rdy_fo_dl */
        rslt = bmi2_get_regs(BMI2_GYR_CRT_CONF_ADDR, &reg_data, 1, dev);
        if(rslt == BMI2_OK)
        {
            (*rdy_for_dl) = BMI2_GET_BITS(reg_data, BMI2_GYR_RDY_FOR_DL);
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API does the crt process if max burst length is not zero.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::process_crt_download(XBYTE last_byte_flag, struct bmi2_dev *dev)
* @brief      Process crt download
* @ingroup    DATAIO
* 
* @param[in]  last_byte_flag : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::process_crt_download(XBYTE last_byte_flag, struct bmi2_dev *dev)
{
    XBYTESIG rslt;
    XBYTE rdy_for_dl = 0;
    XBYTE cmd = BMI2_G_TRIGGER_CMD;

    rslt = null_ptr_check(dev);
    if(rslt == BMI2_OK)
    {
        rslt = get_rdy_for_dl(&rdy_for_dl, dev);
    }

    /* Trigger next CRT command */
    if(rslt == BMI2_OK)
    {
        rslt = bmi2_set_regs(BMI2_CMD_REG_ADDR, &cmd, 1, dev);
    }

    if((!last_byte_flag) && (rslt == BMI2_OK))
    {
        rslt = wait_rdy_for_dl_toggle(BMI2_CRT_READY_FOR_DOWNLOAD_RETRY, rdy_for_dl, dev);
    }

    return rslt;
}

/*!
 * @brief This API to write the 2kb size of crt configuration
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::write_crt_config_file(XWORD write_len, XWORD config_file_size, XWORD start_index, struct bmi2_dev *dev)
* @brief      Write crt config file
* @ingroup    DATAIO
* 
* @param[in]  write_len : 
* @param[in]  config_file_size : 
* @param[in]  start_index : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::write_crt_config_file(XWORD write_len, XWORD config_file_size, XWORD start_index, struct bmi2_dev *dev)
{
    XBYTESIG rslt = BMI2_OK;
    XWORD index = 0;
    XBYTE last_byte_flag = 0;
    XBYTE remain = (XBYTE)(config_file_size % write_len);
    XWORD balance_byte = 0;

    if(!remain)
    {

        /* Write the configuration file */
        for (index = start_index;
             (index < (start_index + config_file_size)) && (rslt == BMI2_OK);
             index += write_len)
        {
            rslt = upload_file((dev->config_file_ptr + index), index, write_len, dev);
            if(index >= ((start_index + config_file_size) - (write_len)))
            {
                last_byte_flag = 1;
            }

            if(rslt == BMI2_OK)
            {
                rslt = process_crt_download(last_byte_flag, dev);
            }
        }
    }
    else
    {
        /* Get the balance bytes */
        balance_byte = (XWORD)start_index + (XWORD)config_file_size - (XWORD)remain;

        /* Write the configuration file for the balance bytes */
        for (index = start_index; (index < balance_byte) && (rslt == BMI2_OK); index += write_len)
        {
            rslt = upload_file((dev->config_file_ptr + index), index, write_len, dev);
            if(rslt == BMI2_OK)
            {
                rslt = process_crt_download(last_byte_flag, dev);
            }
        }

        if(rslt == BMI2_OK)
        {
            /* Write the remaining bytes in 2 bytes length */
            write_len = 2;
            rslt = set_maxburst_len(write_len, dev);

            /* Write the configuration file for the remaining bytes */
            for (index = balance_byte;
                 (index < (start_index + config_file_size)) && (rslt == BMI2_OK);
                 index += write_len)
            {
                rslt = upload_file((dev->config_file_ptr + index), index, write_len, dev);
                if(index < ((start_index + config_file_size) - write_len))
                {
                    last_byte_flag = 1;
                }

                if(rslt == BMI2_OK)
                {
                    rslt = process_crt_download(last_byte_flag, dev);
                }
            }
        }
    }

    return rslt;
}

/*!
 * @brief This API is to wait till the rdy for dl bit toggles after every pack of bytes.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::wait_rdy_for_dl_toggle(XBYTE retry_complete, XBYTE download_ready, struct bmi2_dev *dev)
* @brief      Wait rdy for dl toggle
* @ingroup    DATAIO
* 
* @param[in]  retry_complete : 
* @param[in]  download_ready : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::wait_rdy_for_dl_toggle(XBYTE retry_complete, XBYTE download_ready, struct bmi2_dev *dev)
{
    XBYTESIG rslt = BMI2_OK;
    XBYTE dl_ready = 0;
    XBYTE st_status = 0;

    while ((rslt == BMI2_OK) && (retry_complete--))
    {
        rslt = get_rdy_for_dl(&dl_ready, dev);
        if(download_ready != dl_ready)
        {
            break;
        }

        bmi2_delay_us(BMI2_CRT_READY_FOR_DOWNLOAD_US, dev->intf_ptr);
    }

    if((rslt == BMI2_OK) && (download_ready == dl_ready))
    {
        rslt = BMI2_E_CRT_READY_FOR_DL_FAIL_ABORT;
    }

    if(rslt == BMI2_OK)
    {
        rslt = get_st_running(&st_status, dev);
        if((rslt == BMI2_OK) && (st_status == 0))
        {
            rslt = BMI2_E_ST_ALREADY_RUNNING;
        }
    }

    return rslt;
}

/*!
 * @brief This API is to wait till crt status complete.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::wait_st_running(XBYTE retry_complete, struct bmi2_dev *dev)
* @brief      Wait st running
* @ingroup    DATAIO
* 
* @param[in]  retry_complete : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::wait_st_running(XBYTE retry_complete, struct bmi2_dev *dev)
{
    XBYTE st_status = 1;
    XBYTESIG rslt = BMI2_OK;

    while (retry_complete--)
    {
        rslt = get_st_running(&st_status, dev);
        if((rslt == BMI2_OK) && (st_status == 0))
        {
            break;
        }

        bmi2_delay_us(BMI2_CRT_WAIT_RUNNING_US, dev->intf_ptr);
    }

    if((rslt == BMI2_OK) && (st_status == 1))
    {
        rslt = BMI2_E_ST_ALREADY_RUNNING;
    }

    return rslt;
}

/*!
 * @brief This api is used to perform gyroscope self-test.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_do_gyro_st(struct bmi2_dev *dev)
* @brief      Bmi2 do gyro st
* @ingroup    DATAIO
* 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_do_gyro_st(struct bmi2_dev *dev)
{
    XBYTESIG rslt;

    rslt = do_gtrigger_test(BMI2_SELECT_GYRO_SELF_TEST, dev);

    return rslt;
}

/*!
 * @brief This API is to run the CRT process for both max burst length 0 and non zero condition.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_do_crt(struct bmi2_dev *dev)
* @brief      Bmi2 do crt
* @ingroup    DATAIO
* 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_do_crt(struct bmi2_dev *dev)
{
    XBYTESIG rslt;

    rslt = do_gtrigger_test(BMI2_SELECT_CRT, dev);

    return rslt;
}

/*!
 * @brief This API is to run the crt process for both max burst length 0 and non zero condition.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::do_gtrigger_test(XBYTE gyro_st_crt, struct bmi2_dev *dev)
* @brief      Do gtrigger test
* @ingroup    DATAIO
* 
* @param[in]  gyro_st_crt : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::do_gtrigger_test(XBYTE gyro_st_crt, struct bmi2_dev *dev)
{
    XBYTESIG rslt;
    XBYTESIG rslt_crt = BMI2_OK;
    XBYTE st_status = 0;
    XBYTE max_burst_length = 0;
    XBYTE download_ready = 0;
    XBYTE cmd = BMI2_G_TRIGGER_CMD;
    struct bmi2_gyro_self_test_status gyro_st_result = { 0 };

    /* Variable to get the status of advance power save */
    XBYTE aps_stat = 0;

    rslt = null_ptr_check(dev);
    if(rslt == BMI2_OK)
    {
        /* Check if the variant supports this feature */
        if(dev->variant_feature & BMI2_CRT_RTOSK_ENABLE)
        {
            /* Get status of advance power save mode */
            aps_stat = dev->aps_status;
            if(aps_stat == BMI2_ENABLE)
            {
                /* Disable advance power save if enabled */
                rslt = bmi2_set_adv_power_save(BMI2_DISABLE, dev);
            }

            /* Get max burst length */
            if(rslt == BMI2_OK)
            {
                rslt = get_maxburst_len(&max_burst_length, dev);
            }

            /* Checking for CRT running status */
            if(rslt == BMI2_OK)
            {
                rslt = get_st_running(&st_status, dev);
            }

            /* CRT is not running  and Max burst length is zero */
            if(st_status == 0)
            {
                if(rslt == BMI2_OK)
                {
                    rslt = set_st_running(BMI2_ENABLE, dev);
                }

                /* Preparing the setup */
                if(rslt == BMI2_OK)
                {
                    rslt = crt_prepare_setup(dev);
                }

                /* Enable the gyro self-test, CRT */
                if(rslt == BMI2_OK)
                {
                    rslt = select_self_test(gyro_st_crt, dev);
                }

                /* Check if FIFO is unchanged by checking the max burst length */
                if((rslt == BMI2_OK) && (max_burst_length == 0))
                {
                    /* Trigger CRT */
                    rslt = bmi2_set_regs(BMI2_CMD_REG_ADDR, &cmd, 1, dev);
                    if(rslt == BMI2_OK)
                    {
                        /* Wait until st_status = 0 or time out is 2 seconds */
                        rslt = wait_st_running(BMI2_CRT_WAIT_RUNNING_RETRY_EXECUTION, dev);

                        /* CRT Running wait & check is successful */
                        if(rslt == BMI2_OK)
                        {
                            rslt = crt_gyro_st_update_result(dev);
                        }
                    }
                }
                else
                {
                    /* FIFO may be used */
                    if(rslt == BMI2_OK)
                    {
                        if(dev->read_write_len < 2)
                        {
                            dev->read_write_len = 2;
                        }

                        if(dev->read_write_len > (BMI2_CRT_MAX_BURST_WORD_LENGTH * 2))
                        {
                            dev->read_write_len = BMI2_CRT_MAX_BURST_WORD_LENGTH * 2;
                        }

                        /* Reset the max burst length to default value */
                        rslt = set_maxburst_len(dev->read_write_len, dev);
                    }

                    if(rslt == BMI2_OK)
                    {
                        rslt = get_rdy_for_dl(&download_ready, dev);
                    }

                    /* Trigger CRT  */
                    if(rslt == BMI2_OK)
                    {
                        rslt = bmi2_set_regs(BMI2_CMD_REG_ADDR, &cmd, 1, dev);
                    }

                    /* Wait till either ready for download toggle or crt running = 0 */
                    if(rslt == BMI2_OK)
                    {
                        rslt = wait_rdy_for_dl_toggle(BMI2_CRT_READY_FOR_DOWNLOAD_RETRY, download_ready, dev);
                        if(rslt == BMI2_OK)
                        {
                            rslt = write_crt_config_file(dev->read_write_len, BMI2_CRT_CONFIG_FILE_SIZE, 0x1800, dev);
                        }

                        if(rslt == BMI2_OK)
                        {
                            rslt = wait_st_running(BMI2_CRT_WAIT_RUNNING_RETRY_EXECUTION, dev);
                            rslt_crt = crt_gyro_st_update_result(dev);
                            if(rslt == BMI2_OK)
                            {
                                rslt = rslt_crt;
                            }
                        }
                    }
                }
            }
            else
            {
                rslt = BMI2_E_ST_ALREADY_RUNNING;
            }

            if(rslt == BMI2_OK)
            {
                if(gyro_st_crt == BMI2_SELECT_GYRO_SELF_TEST)
                {
                    rslt = gyro_self_test_completed(&gyro_st_result, dev);
                }
            }

            /* Enable Advance power save if disabled while configuring and
             * not when already disabled
             */
            if((aps_stat == BMI2_ENABLE) && (rslt == BMI2_OK))
            {
                rslt = bmi2_set_adv_power_save(BMI2_ENABLE, dev);
            }
        }
    }

    return rslt;
}

/*!
 * @brief This API to set up environment for processing the crt.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::crt_prepare_setup(struct bmi2_dev *dev)
* @brief      Crt prepare setup
* @ingroup    DATAIO
* 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::crt_prepare_setup(struct bmi2_dev *dev)
{
    XBYTESIG rslt;

    /* Variable to select the sensor */
    XBYTE sens_list = BMI2_GYRO;

    rslt = null_ptr_check(dev);

    if(rslt == BMI2_OK)
    {
        /* Disable gyroscope */
        rslt = bmi2_sensor_disable(&sens_list, 1, dev);
    }

    /* Disable FIFO for all sensors */
    if(rslt == BMI2_OK)
    {
        rslt = bmi2_set_fifo_config(BMI2_FIFO_ALL_EN, BMI2_DISABLE, dev);
    }

    if(rslt == BMI2_OK)
    {
        /* Enable accelerometer */
        sens_list = BMI2_ACCEL;
        rslt = bmi2_sensor_enable(&sens_list, 1, dev);
    }

    if(rslt == BMI2_OK)
    {
        /* Disable Abort after 1 msec */
        bmi2_delay_us(1000, dev->intf_ptr);
        rslt = abort_bmi2(BMI2_DISABLE, dev);
    }

    return rslt;
}

/*!
 * @brief This API is to update the CRT or gyro self-test final result.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::crt_gyro_st_update_result(struct bmi2_dev *dev)
* @brief      Crt gyro st update result
* @ingroup    DATAIO
* 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::crt_gyro_st_update_result(struct bmi2_dev *dev)
{
    XBYTESIG rslt;
    struct bmi2_gyr_user_gain_status user_gain_stat = { 0, 0, 0, 0 };

    rslt = null_ptr_check(dev);

    /* CRT status has to be read from the config register map */
    if(rslt == BMI2_OK)
    {
        rslt = get_gyro_gain_update_status(&user_gain_stat, dev);
    }

    if(rslt == BMI2_OK)
    {
        switch (user_gain_stat.g_trigger_status)
        {
            case BMI2_G_TRIGGER_NO_ERROR:

                /* CRT is successful - Reset the Max Burst Length */
                rslt = set_maxburst_len(0, dev);
                break;

            case BMI2_G_TRIGGER_DL_ERROR:

                /* CRT is Download Error - Keep non zero value for Max Burst Length */
                rslt = set_maxburst_len(dev->read_write_len, dev);
                if(rslt == BMI2_OK)
                {
                    rslt = BMI2_E_DL_ERROR;
                }

                break;
            case BMI2_G_TRIGGER_ABORT_ERROR:

                /* Command is aborted either by host via the block bit or due to motion
                 * detection. Keep non zero value for Max Burst Length
                 */
                rslt = set_maxburst_len(dev->read_write_len, dev);
                if(rslt == BMI2_OK)
                {
                    rslt = BMI2_E_ABORT_ERROR;
                }

                break;

            case BMI2_G_TRIGGER_PRECON_ERROR:

                /* Pre-condition to start the feature was not completed. */
                rslt = BMI2_E_PRECON_ERROR;
                break;

            default:
                rslt = BMI2_E_INVALID_STATUS;

                break;
        }
    }

    return rslt;
}

/*!
 * @brief This internal API gets the max burst length.
 */
 XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_maxburst_len(XBYTE *max_burst_len, struct bmi2_dev *dev)
{
    XBYTESIG rslt = BMI2_OK;
    XBYTE feat_config[BMI2_FEAT_SIZE_IN_BYTES] = { 0 };
    XBYTE idx = 0;
    XBYTE feat_found = 0;
    struct bmi2_feature_config maxburst_length_bytes = { 0, 0, 0 };
    XBYTE aps_stat;

    if((dev->variant_feature & BMI2_CRT_IN_FIFO_NOT_REQ) != 0)
    {
        *max_burst_len = 0;

        return BMI2_OK;
    }

    /* Get status of advance power save mode */
    aps_stat = dev->aps_status;
    if(aps_stat == BMI2_ENABLE)
    {
        /* Disable advance power save if enabled */
        rslt = bmi2_set_adv_power_save(BMI2_DISABLE, dev);
    }

    if(rslt == BMI2_OK)
    {
        /* Search for max burst length */
        feat_found = bmi2_extract_input_feat_config(&maxburst_length_bytes, BMI2_MAX_BURST_LEN, dev);
        if(feat_found)
        {
            rslt = bmi2_get_feat_config(maxburst_length_bytes.page, feat_config, dev);
            if(rslt == BMI2_OK)
            {
                /* Define the offset for max burst length */
                idx = maxburst_length_bytes.start_addr;

                /* Get the max burst length */
                *max_burst_len = feat_config[idx];
            }
        }
        else
        {
            rslt = BMI2_E_INVALID_SENSOR;
        }

        /* Enable Advance power save if disabled while configuring and
         * not when already disabled
         */
        if((aps_stat == BMI2_ENABLE) && (rslt == BMI2_OK))
        {
            rslt = bmi2_set_adv_power_save(BMI2_ENABLE, dev);
        }
    }

    return rslt;
}

/*!
 * @brief This internal API sets the max burst length.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_maxburst_len(const XWORD write_len_byte, struct bmi2_dev *dev)
* @brief      Set maxburst len
* @ingroup    DATAIO
* 
* @param[in]  write_len_byte : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_maxburst_len(const XWORD write_len_byte, struct bmi2_dev *dev)
{
    XBYTESIG rslt = BMI2_OK;
    XBYTE feat_config[BMI2_FEAT_SIZE_IN_BYTES] = { 0 };
    XBYTE idx = 0;
    XBYTE reg_addr = 0;
    XBYTE max_burst_len = 0;
    XBYTE feat_found = 0;
    struct bmi2_feature_config maxburst_length_bytes = { 0, 0, 0 };
    XBYTE aps_stat;
    XWORD burst_len = write_len_byte / 2;

    /* for variant that support crt outside fifo, do not modify the max burst len */
    if((dev->variant_feature & BMI2_CRT_IN_FIFO_NOT_REQ) != 0)
    {
        return BMI2_OK;
    }

    /* Max burst length is only 1 byte */
    if(burst_len > BMI2_CRT_MAX_BURST_WORD_LENGTH)
    {
        max_burst_len = UINT8_C(0xFF);
    }
    else
    {
        max_burst_len = (XBYTE)burst_len;
    }

    /* Get status of advance power save mode */
    aps_stat = dev->aps_status;
    if(aps_stat == BMI2_ENABLE)
    {
        /* Disable advance power save if enabled */
        rslt = bmi2_set_adv_power_save(BMI2_DISABLE, dev);
    }

    if(rslt == BMI2_OK)
    {
        /* Search for axis-re-mapping and extract its configuration details */
        feat_found = bmi2_extract_input_feat_config(&maxburst_length_bytes, BMI2_MAX_BURST_LEN, dev);
        if(feat_found)
        {
            /* Get the configuration from the page where axis
             * re-mapping feature resides
             */
            rslt = bmi2_get_feat_config(maxburst_length_bytes.page, feat_config, dev);
            if(rslt == BMI2_OK)
            {
                /* Define the offset in bytes */
                idx = maxburst_length_bytes.start_addr;

                /* update Max burst length */
                feat_config[idx] = max_burst_len;

                /* Update the register address */
                reg_addr = BMI2_FEATURES_REG_ADDR + maxburst_length_bytes.start_addr;

                /* Set the configuration back to the page */
                rslt = bmi2_set_regs(reg_addr, &feat_config[maxburst_length_bytes.start_addr], 2, dev);
            }
        }
        else
        {
            rslt = BMI2_E_INVALID_SENSOR;
        }

        /* Enable Advance power save if disabled while configuring and
         * not when already disabled
         */
        if((aps_stat == BMI2_ENABLE) && (rslt == BMI2_OK))
        {
            rslt = bmi2_set_adv_power_save(BMI2_ENABLE, dev);
        }
    }

    return rslt;
}

/*!
 * @brief This api is used to trigger the preparation for system for NVM programming.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_nvm_prep_prog(XBYTE nvm_prep, struct bmi2_dev *dev)
* @brief      Set nvm prep prog
* @ingroup    DATAIO
* 
* @param[in]  nvm_prep : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_nvm_prep_prog(XBYTE nvm_prep, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt = BMI2_OK;

    /* Array to define the feature configuration */
    XBYTE feat_config[BMI2_FEAT_SIZE_IN_BYTES] = { 0 };

    /* Variable to define the array offset */
    XBYTE idx = 0;

    /* Variable to set flag */
    XBYTE feat_found;
    XBYTE reg_addr = 0;

    /* Initialize feature configuration for nvm preparation*/
    struct bmi2_feature_config nvm_config = { 0, 0, 0 };

    /* Search for bmi2 gyro self offset correction feature as nvm program preparation feature is
     * present in the same Word and extract its configuration details
     */
    feat_found = bmi2_extract_input_feat_config(&nvm_config, BMI2_NVM_PROG_PREP, dev);
    if(feat_found)
    {
        /* Get the configuration from the page where nvm preparation feature enable feature
         * resides */
        rslt = bmi2_get_feat_config(nvm_config.page, feat_config, dev);
        if(rslt == BMI2_OK)
        {
            /* Define the offset for nvm preparation feature enable */
            idx = nvm_config.start_addr;

            /* update nvm_prog_prep  enable bit */
            feat_config[idx] = BMI2_SET_BITS(feat_config[idx], BMI2_NVM_PREP_FEATURE_EN, nvm_prep);

            /* Update the register address */
            reg_addr = BMI2_FEATURES_REG_ADDR + nvm_config.start_addr - 1;

            /* Set the configuration back to the page */
            rslt = bmi2_set_regs(reg_addr, &feat_config[nvm_config.start_addr - 1], 2, dev);
        }
    }
    else
    {
        rslt = BMI2_E_INVALID_SENSOR;
    }

    return rslt;
}

/*!
 * @brief This api is used to enable the CRT.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::select_self_test(XBYTE gyro_st_crt, struct bmi2_dev *dev)
* @brief      Select self test
* @ingroup    DATAIO
* 
* @param[in]  gyro_st_crt : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::select_self_test(XBYTE gyro_st_crt, struct bmi2_dev *dev)
{
    XBYTESIG rslt;

    XBYTE feat_config[BMI2_FEAT_SIZE_IN_BYTES] = { 0 };

    XBYTE idx = 0;

    XBYTE feat_found;
    XBYTE reg_addr = 0;

    struct bmi2_feature_config gyro_self_test_crt_config = { 0, 0, 0 };

    /* Search for bmi2 crt gyro self-test feature and extract its configuration details */
    feat_found = bmi2_extract_input_feat_config(&gyro_self_test_crt_config, BMI2_CRT_GYRO_SELF_TEST, dev);
    if(feat_found)
    {
        /* Get the configuration from the page where gyro self-test and crt enable feature
         * resides */
        rslt = bmi2_get_feat_config(gyro_self_test_crt_config.page, feat_config, dev);
        if(rslt == BMI2_OK)
        {
            /* Define the offset in bytes */
            idx = gyro_self_test_crt_config.start_addr;

            /* update the gyro self-test crt enable bit */
            feat_config[idx] = BMI2_SET_BIT_POS0(feat_config[idx], BMI2_GYRO_SELF_TEST_CRT_EN, gyro_st_crt);

            /* Update the register address */
            reg_addr = BMI2_FEATURES_REG_ADDR + (gyro_self_test_crt_config.start_addr - 1);

            /* Set the configuration back to the page */
            rslt = bmi2_set_regs(reg_addr, &feat_config[gyro_self_test_crt_config.start_addr - 1], 2, dev);
        }
    }
    else
    {
        rslt = BMI2_E_INVALID_SENSOR;
    }

    return rslt;
}

/*!
 * @brief This api is used to abort ongoing crt or gyro self-test.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_abort_crt_gyro_st(struct bmi2_dev *dev)
* @brief      Bmi2 abort crt gyro st
* @ingroup    DATAIO
* 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_abort_crt_gyro_st(struct bmi2_dev *dev)
{
    XBYTESIG rslt = BMI2_OK;
    XBYTE aps_stat;
    XBYTE st_running = 0;
    XBYTE cmd = BMI2_G_TRIGGER_CMD;

    /* Get status of advance power save mode */
    aps_stat = dev->aps_status;
    if(aps_stat == BMI2_ENABLE)
    {
        /* Disable advance power save if enabled */
        rslt = bmi2_set_adv_power_save(BMI2_DISABLE, dev);
    }

    /* Checking for ST running status */
    if(rslt == BMI2_OK)
    {
        rslt = get_st_running(&st_running, dev);
        if(rslt == BMI2_OK)
        {
            /* ST is not running  */
            if(st_running == 0)
            {
                rslt = BMI2_E_ST_NOT_RUNING;
            }
        }
    }

    if(rslt == BMI2_OK)
    {
        rslt = abort_bmi2(BMI2_ENABLE, dev);
    }

    /* send the g trigger command */
    if(rslt == BMI2_OK)
    {
        rslt = bmi2_set_regs(BMI2_CMD_REG_ADDR, &cmd, 1, dev);
    }

    if(rslt == BMI2_OK)
    {
        /* wait until st_status = 0 or time out is 2 seconds */
        rslt = wait_st_running(BMI2_CRT_WAIT_RUNNING_RETRY_EXECUTION, dev);
    }

    /* Check G trigger status for error */
    if(rslt == BMI2_OK)
    {
        rslt = crt_gyro_st_update_result(dev);
        if(rslt == BMI2_E_ABORT_ERROR)
        {
            rslt = BMI2_OK;
        }
        else
        {
            rslt = BMI2_E_ABORT_ERROR;
        }
    }

    /* Enable Advance power save if disabled while configuring and
     * not when already disabled
     */
    if((aps_stat == BMI2_ENABLE) && (rslt == BMI2_OK))
    {
        rslt = bmi2_set_adv_power_save(BMI2_ENABLE, dev);
    }

    return rslt;
}

/*!
 * @brief This api is used to enable/disable abort.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::abort_bmi2(XBYTE abort_enable, struct bmi2_dev *dev)
* @brief      Abort bmi2
* @ingroup    DATAIO
* 
* @param[in]  abort_enable : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::abort_bmi2(XBYTE abort_enable, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Array to define the feature configuration */
    XBYTE feat_config[BMI2_FEAT_SIZE_IN_BYTES] = { 0 };

    /* Variable to define the array offset */
    XBYTE idx = 0;

    /* Variable to set flag */
    XBYTE feat_found;
    XBYTE reg_addr = 0;

    /* Initialize feature configuration for blocking a feature */
    struct bmi2_feature_config block_config = { 0, 0, 0 };

    /* Search for bmi2 Abort feature and extract its configuration details */
    feat_found = bmi2_extract_input_feat_config(&block_config, BMI2_ABORT_CRT_GYRO_SELF_TEST, dev);
    if(feat_found)
    {
        /* Get the configuration from the page where abort(block) feature resides */
        rslt = bmi2_get_feat_config(block_config.page, feat_config, dev);
        if(rslt == BMI2_OK)
        {
            /* Define the offset in bytes */
            idx = block_config.start_addr;

            /* update the gyro self-test crt abort enable bit */
            feat_config[idx] = BMI2_SET_BITS(feat_config[idx], BMI2_ABORT_FEATURE_EN, abort_enable);

            /* Update the register address */
            reg_addr = BMI2_FEATURES_REG_ADDR + (block_config.start_addr - 1);

            /* Set the configuration back to the page */
            rslt = bmi2_set_regs(reg_addr, &feat_config[block_config.start_addr - 1], 2, dev);
        }
    }
    else
    {
        rslt = BMI2_E_INVALID_SENSOR;
    }

    return rslt;
}

/*!
 * @brief This api is use to wait till  gyro self-test is completed and update the status of gyro
 * self-test.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::gyro_self_test_completed(struct bmi2_gyro_self_test_status *gyro_st_result, struct bmi2_dev *dev)
* @brief      Gyro self test completed
* @ingroup    DATAIO
* 
* @param[in]  gyro_st_result : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::gyro_self_test_completed(struct bmi2_gyro_self_test_status *gyro_st_result, struct bmi2_dev *dev)
{
    XBYTESIG rslt;
    XBYTE reg_data;

    rslt = bmi2_get_regs(BMI2_GYR_SELF_TEST_AXES_ADDR, &reg_data, 1, dev);
    if(rslt == BMI2_OK)
    {
        gyro_st_result->gyr_st_axes_done = BMI2_GET_BIT_POS0(reg_data, BMI2_GYR_ST_AXES_DONE);
        if(gyro_st_result->gyr_st_axes_done == 0x01)
        {
            gyro_st_result->gyr_axis_x_ok = BMI2_GET_BITS(reg_data, BMI2_GYR_AXIS_X_OK);
            gyro_st_result->gyr_axis_y_ok = BMI2_GET_BITS(reg_data, BMI2_GYR_AXIS_Y_OK);
            gyro_st_result->gyr_axis_z_ok = BMI2_GET_BITS(reg_data, BMI2_GYR_AXIS_Z_OK);
        }
        else
        {
            rslt = BMI2_E_SELF_TEST_NOT_DONE;
        }
    }

    return rslt;
}

/*!
 * @brief This api validates accel foc position as per the range
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::validate_foc_position(XBYTE sens_list, const struct bmi2_accel_foc_g_value *accel_g_axis, struct bmi2_sens_axes_data avg_foc_data, struct bmi2_dev *dev)
* @brief      Validate foc position
* @ingroup    DATAIO
* 
* @param[in]  sens_list : 
* @param[in]  accel_g_axis : 
* @param[in]  avg_foc_data : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::validate_foc_position(XBYTE sens_list, const struct bmi2_accel_foc_g_value *accel_g_axis, struct bmi2_sens_axes_data avg_foc_data, struct bmi2_dev *dev)
{
    XBYTESIG rslt = BMI2_E_INVALID_INPUT;

    if(sens_list == BMI2_ACCEL)
    {
        if(accel_g_axis->x == 1)
        {
            rslt = validate_foc_accel_axis(avg_foc_data.x, dev);
        }
        else if(accel_g_axis->y == 1)
        {
            rslt = validate_foc_accel_axis(avg_foc_data.y, dev);
        }
        else
        {
            rslt = validate_foc_accel_axis(avg_foc_data.z, dev);
        }
    }
    else if(sens_list == BMI2_GYRO)
    {
        if(((avg_foc_data.x >= BMI2_GYRO_FOC_NOISE_LIMIT_NEGATIVE) &&
             (avg_foc_data.x <= BMI2_GYRO_FOC_NOISE_LIMIT_POSITIVE)) &&
            ((avg_foc_data.y >= BMI2_GYRO_FOC_NOISE_LIMIT_NEGATIVE) &&
             (avg_foc_data.y <= BMI2_GYRO_FOC_NOISE_LIMIT_POSITIVE)) &&
            ((avg_foc_data.z >= BMI2_GYRO_FOC_NOISE_LIMIT_NEGATIVE) &&
             (avg_foc_data.z <= BMI2_GYRO_FOC_NOISE_LIMIT_POSITIVE)))
        {
            rslt = BMI2_OK;
        }
        else
        {
            rslt = BMI2_E_INVALID_FOC_POSITION;
        }
    }

    return rslt;
}

/*!
 * @brief This api validates depends on accel foc access input
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::validate_foc_accel_axis(XWORDSIG avg_foc_data, struct bmi2_dev *dev)
* @brief      Validate foc accel axis
* @ingroup    DATAIO
* 
* @param[in]  avg_foc_data : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::validate_foc_accel_axis(XWORDSIG avg_foc_data, struct bmi2_dev *dev)
{
    struct bmi2_sens_config sens_cfg = { 0 };
    XBYTE range;
    XBYTESIG rslt;

    sens_cfg.type = BMI2_ACCEL;
    rslt = bmi2_get_sensor_config(&sens_cfg, 1, dev);
    range = sens_cfg.cfg.acc.range;

    /* reference LSB value of 16G */
    if((range == BMI2_ACC_RANGE_2G) && (avg_foc_data > BMI2_ACC_2G_MIN_NOISE_LIMIT) &&
        (avg_foc_data < BMI2_ACC_2G_MAX_NOISE_LIMIT))
    {
        rslt = BMI2_OK;
    }
    /* reference LSB value of 16G */
    else if((range == BMI2_ACC_RANGE_4G) && (avg_foc_data > BMI2_ACC_4G_MIN_NOISE_LIMIT) &&
             (avg_foc_data < BMI2_ACC_4G_MAX_NOISE_LIMIT))
    {
        rslt = BMI2_OK;
    }
    /* reference LSB value of 16G */
    else if((range == BMI2_ACC_RANGE_8G) && (avg_foc_data > BMI2_ACC_8G_MIN_NOISE_LIMIT) &&
             (avg_foc_data < BMI2_ACC_8G_MAX_NOISE_LIMIT))
    {
        rslt = BMI2_OK;
    }
    /* reference LSB value of 16G */
    else if((range == BMI2_ACC_RANGE_16G) && (avg_foc_data > BMI2_ACC_16G_MIN_NOISE_LIMIT) &&
             (avg_foc_data < BMI2_ACC_16G_MAX_NOISE_LIMIT))
    {
        rslt = BMI2_OK;
    }
    else
    {
        rslt = BMI2_E_INVALID_FOC_POSITION;
    }

    return rslt;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_nvm_prog(struct bmi2_dev *dev)
* @brief      Bmi2 nvm prog
* @ingroup    DATAIO
* 
* @param[in]  dev : Dev pointer to use.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
/*! @brief This api is used for programming the non volatile memory(nvm) */
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi2_nvm_prog(struct bmi2_dev *dev)
{
    XBYTESIG rslt = BMI2_OK;

    /* Variable to get the status of advance power save */
    XBYTE aps_stat;
    XBYTE status;
    XBYTE cmd_rdy;
    XBYTE reg_data;
    XBYTE write_timeout = 100;

    /* Get status of advance power save mode */
    aps_stat = dev->aps_status;
    if(aps_stat == BMI2_ENABLE)
    {
        /* Disable advance power save if enabled */
        rslt = bmi2_set_adv_power_save(BMI2_DISABLE, dev);
    }

    /* Check the Write status and proceed only if there is no ongoing write cycle */
    if(rslt == BMI2_OK)
    {
        rslt = bmi2_get_status(&status, dev);

        cmd_rdy = BMI2_GET_BITS(status, BMI2_CMD_RDY);
        if(cmd_rdy)
        {
            rslt = set_nvm_prep_prog(BMI2_ENABLE, dev);
            if(rslt == BMI2_OK)
            {
                bmi2_delay_us(40000, dev->intf_ptr);

                /* Set the NVM_CONF.nvm_prog_en bit in order to enable the NVM
                 * programming */
                reg_data = BMI2_NVM_UNLOCK_ENABLE;
                rslt = bmi2_set_regs(BMI2_NVM_CONF_ADDR, &reg_data, 1, dev);
                if(rslt == BMI2_OK)
                {
                    /* Send NVM prog command to command register */
                    reg_data = BMI2_NVM_PROG_CMD;
                    rslt = bmi2_set_regs(BMI2_CMD_REG_ADDR, &reg_data, 1, dev);
                }

                /* Wait till write operation is completed */
                if(rslt == BMI2_OK)
                {
                    while (write_timeout--)
                    {
                        rslt = bmi2_get_status(&status, dev);
                        if(rslt == BMI2_OK)
                        {
                            cmd_rdy = BMI2_GET_BITS(status, BMI2_CMD_RDY);

                            /* Nvm is complete once cmd_rdy is 1, break if 1 */
                            if(cmd_rdy)
                            {
                                break;
                            }

                            /* Wait till cmd_rdy becomes 1 indicating
                             * nvm process completes */
                            bmi2_delay_us(20000, dev->intf_ptr);
                        }
                    }
                }

                if((rslt == BMI2_OK) && (cmd_rdy != BMI2_TRUE))
                {
                    rslt = BMI2_E_WRITE_CYCLE_ONGOING;
                }
            }
        }
        else
        {
            rslt = BMI2_E_WRITE_CYCLE_ONGOING;
        }
    }

    if(rslt == BMI2_OK)
    {
        /* perform soft reset */
        rslt = bmi2_soft_reset(dev);
    }

    /* Enable Advance power save if disabled while configuring and not when already disabled */
    if((aps_stat == BMI2_ENABLE) && (rslt == BMI2_OK))
    {
        rslt = bmi2_set_adv_power_save(BMI2_ENABLE, dev);
    }

    return rslt;
}

/*!
 * @brief This API reads and provides average for 128 samples of sensor data for foc operation
 * gyro.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_average_of_sensor_data(XBYTE sens_list, struct bmi2_foc_temp_value *temp_foc_data, struct bmi2_dev *dev)
* @brief      Get average of sensor data
* @ingroup    DATAIO
* 
* @param[in]  sens_list : 
* @param[in]  temp_foc_data : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_average_of_sensor_data(XBYTE sens_list, struct bmi2_foc_temp_value *temp_foc_data, struct bmi2_dev *dev)
{
    XBYTESIG rslt = 0;
    struct bmi2_sensor_data sensor_data = { 0 };
    XBYTE sample_count = 0;
    XBYTE datardy_try_cnt;
    XBYTE drdy_status = 0;
    XBYTE sensor_drdy = 0;

    sensor_data.type = sens_list;
    if(sens_list == BMI2_ACCEL)
    {
        sensor_drdy = BMI2_DRDY_ACC;
    }
    else
    {
        sensor_drdy = BMI2_DRDY_GYR;
    }

    /* Read sensor values before FOC */
    while (sample_count < BMI2_FOC_SAMPLE_LIMIT)
    {
        datardy_try_cnt = 5;
        do
        {
            bmi2_delay_us(20000, dev->intf_ptr);
            rslt = bmi2_get_status(&drdy_status, dev);
            datardy_try_cnt--;
        } while ((rslt == BMI2_OK) && (!(drdy_status & sensor_drdy)) && (datardy_try_cnt));

        if((rslt != BMI2_OK) || (datardy_try_cnt == 0))
        {
            rslt = BMI2_E_DATA_RDY_INT_FAILED;
            break;
        }

        rslt = bmi2_get_sensor_data(&sensor_data, 1, dev);

        if(rslt == BMI2_OK)
        {
            if(sensor_data.type == BMI2_ACCEL)
            {
                temp_foc_data->x += sensor_data.sens_data.acc.x;
                temp_foc_data->y += sensor_data.sens_data.acc.y;
                temp_foc_data->z += sensor_data.sens_data.acc.z;
            }
            else if(sensor_data.type == BMI2_GYRO)
            {
                temp_foc_data->x += sensor_data.sens_data.gyr.x;
                temp_foc_data->y += sensor_data.sens_data.gyr.y;
                temp_foc_data->z += sensor_data.sens_data.gyr.z;
            }
        }
        else
        {
            break;
        }

        sample_count++;
    }

    if(rslt == BMI2_OK)
    {
        temp_foc_data->x = (temp_foc_data->x / BMI2_FOC_SAMPLE_LIMIT);
        temp_foc_data->y = (temp_foc_data->y / BMI2_FOC_SAMPLE_LIMIT);
        temp_foc_data->z = (temp_foc_data->z / BMI2_FOC_SAMPLE_LIMIT);
    }

    return rslt;
}

/*!
 * @brief This internal API extract the identification feature from the DMR page
 * and retrieve the config file major and minor version.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::extract_config_file(XBYTE *config_major, XBYTE *config_minor, struct bmi2_dev *dev)
* @brief      Extract config file
* @ingroup    DATAIO
* 
* @param[in]  config_major : 
* @param[in]  config_minor : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::extract_config_file(XBYTE *config_major, XBYTE *config_minor, struct bmi2_dev *dev)
{
    /* Variable to define the result */
    XBYTESIG rslt = BMI2_OK;

    /* Variable to define the array offset */
    XBYTE idx = 0;

    /* Variable to define LSB */
    XWORD lsb = 0;

    /* Variable to define MSB */
    XWORD msb = 0;

    /* Variable to define a word */
    XWORD lsb_msb = 0;

    /* Variable to set flag */
    XBYTE feat_found;

    /* Variable to define advance power save mode status */
    XBYTE aps_stat;

    /* Array to define the feature configuration */
    XBYTE feat_config[BMI2_FEAT_SIZE_IN_BYTES] = { 0 };

    /* Initialize feature configuration for config file identification */
    struct bmi2_feature_config config_id = { 0, 0, 0 };

    /* Check the power mode status */
    aps_stat = dev->aps_status;
    if(aps_stat == BMI2_ENABLE)
    {
        /* Disable advance power save if enabled */
        rslt = bmi2_set_adv_power_save(BMI2_DISABLE, dev);
    }

    if(rslt == BMI2_OK)
    {
        /* Search for config file identification feature and extract its configuration
         * details */
        feat_found = bmi2_extract_input_feat_config(&config_id, BMI2_CONFIG_ID, dev);
        if(feat_found)
        {
            /* Get the configuration from the page where config file identification
             * feature resides */
            rslt = bmi2_get_feat_config(config_id.page, feat_config, dev);
            if(rslt == BMI2_OK)
            {
                /* Define the offset for config file identification */
                idx = config_id.start_addr;

                /* Get word to calculate config file identification */
                lsb = (XWORD) feat_config[idx++];
                msb = ((XWORD) feat_config[idx++] << 8);
                lsb_msb = lsb | msb;

                /* Get major and minor version */
                *config_major = BMI2_GET_BITS(lsb_msb, BMI2_CONFIG_MAJOR);
                *config_minor = BMI2_GET_BIT_POS0(lsb, BMI2_CONFIG_MINOR);
            }
        }

        /* Enable Advance power save if disabled while configuring and
         * not when already disabled
         */
        if((aps_stat == BMI2_ENABLE) && (rslt == BMI2_OK))
        {
            rslt = bmi2_set_adv_power_save(BMI2_ENABLE, dev);
        }
    }
    else
    {
        rslt = BMI2_E_INVALID_SENSOR;
    }

    return rslt;
}

/*!
 *@brief This internal API is used to map the interrupts to the sensor.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C6AXISTRACKINGBMI270::extract_feat_int_map(struct bmi2_map_int *map_int, XBYTE type, const struct bmi2_dev *dev)
* @brief      Extract feat int map
* @ingroup    DATAIO
* 
* @param[in]  map_int : 
* @param[in]  type : 
* @param[in]  dev : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C6AXISTRACKINGBMI270::extract_feat_int_map(struct bmi2_map_int *map_int, XBYTE type, const struct bmi2_dev *dev)
{
    /* Variable to define loop */
    XBYTE loop = 0;

    /* Search for the interrupts from the input configuration array */
    while (loop < dev->sens_int_map)
    {
        if(dev->map_int[loop].type == type)
        {
            *map_int = dev->map_int[loop];
            break;
        }

        loop++;
    }
}

/*!
 * @brief This internal API gets the saturation status for the gyroscope user
 * gain update.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_gyro_gain_update_status(struct bmi2_gyr_user_gain_status *user_gain_stat, struct bmi2_dev *dev)
* @brief      Get gyro gain update status
* @ingroup    DATAIO
* 
* @param[in]  user_gain_stat : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_gyro_gain_update_status(struct bmi2_gyr_user_gain_status *user_gain_stat, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Array to define the feature configuration */
    XBYTE feat_config[BMI2_FEAT_SIZE_IN_BYTES] = { 0 };

    /* Variables to define index */
    XBYTE idx = 0;

    /* Variable to set flag */
    XBYTE feat_found;

    /* Initialize feature output for gyroscope user gain status */
    struct bmi2_feature_config user_gain_cfg = { 0, 0, 0 };

    /* Search for gyroscope user gain status output feature and extract its
     * configuration details
     */
    feat_found = extract_output_feat_config(&user_gain_cfg, BMI2_GYRO_GAIN_UPDATE, dev);
    if(feat_found)
    {
        /* Get the feature output configuration for gyroscope user gain  status */
        rslt = bmi2_get_feat_config(user_gain_cfg.page, feat_config, dev);
        if(rslt == BMI2_OK)
        {
            /* Define the offset in bytes for gyroscope user gain status */
            idx = user_gain_cfg.start_addr;

            /* Get the saturation status for x-axis */
            user_gain_stat->sat_x = BMI2_GET_BIT_POS0(feat_config[idx], BMI2_GYR_USER_GAIN_SAT_STAT_X);

            /* Get the saturation status for y-axis */
            user_gain_stat->sat_y = BMI2_GET_BITS(feat_config[idx], BMI2_GYR_USER_GAIN_SAT_STAT_Y);

            /* Get the saturation status for z-axis */
            user_gain_stat->sat_z = BMI2_GET_BITS(feat_config[idx], BMI2_GYR_USER_GAIN_SAT_STAT_Z);

            /* Get g trigger status */
            user_gain_stat->g_trigger_status = BMI2_GET_BITS(feat_config[idx], BMI2_G_TRIGGER_STAT);
        }
    }
    else
    {
        rslt = BMI2_E_INVALID_SENSOR;
    }

    return rslt;
}

/*!
 * @brief This internal API is used to extract the output feature configuration
 * details from the look-up table.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE DIOI2C6AXISTRACKINGBMI270::extract_output_feat_config(struct bmi2_feature_config *feat_output, XBYTE type, const struct bmi2_dev *dev)
* @brief      Extract output feat config
* @ingroup    DATAIO
* 
* @param[in]  feat_output : 
* @param[in]  type : 
* @param[in]  dev : 
* 
* @return     XBYTE : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE DIOI2C6AXISTRACKINGBMI270::extract_output_feat_config(struct bmi2_feature_config *feat_output, XBYTE type, const struct bmi2_dev *dev)
{
    /* Variable to define loop */
    XBYTE loop = 0;

    /* Variable to set flag */
    XBYTE feat_found = BMI2_FALSE;

    /* Search for the output feature from the output configuration array */
    while (loop < dev->out_sens)
    {
        if(dev->feat_output[loop].type == type)
        {
            *feat_output = dev->feat_output[loop];
            feat_found = BMI2_TRUE;
            break;
        }

        loop++;
    }

    /* Return flag */
    return feat_found;
}

/*!
 * @brief This internal API gets the cross sensitivity coefficient between
 * gyroscope's X and Z axes.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_gyro_cross_sense(XWORDSIG *cross_sense, struct bmi2_dev *dev)
* @brief      Get gyro cross sense
* @ingroup    DATAIO
* 
* @param[in]  cross_sense : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_gyro_cross_sense(XWORDSIG *cross_sense, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Array to define the feature configuration */
    XBYTE feat_config[BMI2_FEAT_SIZE_IN_BYTES] = { 0 };

    /* Variable to define index */
    XBYTE idx = 0;

    /* Variable to set flag */
    XBYTE feat_found;

    XBYTE corr_fact_zx;

    /* Initialize feature output for gyroscope cross sensitivity */
    struct bmi2_feature_config cross_sense_out_config = { 0, 0, 0 };

    if(dev->variant_feature & BMI2_MAXIMUM_FIFO_VARIANT)
    {
        /* For maximum_fifo variant fetch the correction factor from GPIO0 */
        rslt = bmi2_get_regs(BMI2_GYR_CAS_GPIO0_ADDR, &corr_fact_zx, 1, dev);
        if(rslt == BMI2_OK)
        {
            /* Get the gyroscope cross sensitivity coefficient */
            if(corr_fact_zx & BMI2_GYRO_CROSS_AXES_SENSE_SIGN_BIT_MASK)
            {
                *cross_sense = (XWORDSIG)(((XWORDSIG)corr_fact_zx) - 128);
            }
            else
            {
                *cross_sense = (XWORDSIG)(corr_fact_zx);
            }
        }
    }
    else
    {
        /* Search for gyroscope cross sensitivity feature and extract its configuration details */
        feat_found = extract_output_feat_config(&cross_sense_out_config, BMI2_GYRO_CROSS_SENSE, dev);
        if(feat_found)
        {
            /* Get the feature output configuration for gyroscope cross sensitivity
             * feature */
            rslt = bmi2_get_feat_config(cross_sense_out_config.page, feat_config, dev);
            if(rslt == BMI2_OK)
            {
                /* Define the offset in bytes for gyroscope cross sensitivity output */
                idx = cross_sense_out_config.start_addr;

                /* discard the MSB as GYR_CAS is of only 7 bit */
                feat_config[idx] = feat_config[idx] & BMI2_GYRO_CROSS_AXES_SENSE_MASK;

                /* Get the gyroscope cross sensitivity coefficient */
                if(feat_config[idx] & BMI2_GYRO_CROSS_AXES_SENSE_SIGN_BIT_MASK)
                {
                    *cross_sense = (XWORDSIG)(((XWORDSIG)feat_config[idx]) - 128);
                }
                else
                {
                    *cross_sense = (XWORDSIG)(feat_config[idx]);
                }
            }
        }
        else
        {
            rslt = BMI2_E_INVALID_SENSOR;
        }
    }

    return rslt;
}

/*!
 * @brief This internal API selects the sensor/features to be enabled or
 * disabled.
 */
//XBYTESIG DIOI2C6AXISTRACKINGBMI270::select_sensor(const XBYTE *sens_list, XBYTE n_sens, XQWORD *sensor_sel)
//{
//    /* Variable to define error */
//    XBYTESIG rslt = BMI2_OK;
//
//    /* Variable to define loop */
//    XBYTE count;
//
//    for (count = 0; count < n_sens; count++)
//    {
//        switch (sens_list[count])
//        {
//            case BMI2_ACCEL:
//                *sensor_sel |= BMI2_ACCEL_SENS_SEL;
//                break;
//            case BMI2_GYRO:
//                *sensor_sel |= BMI2_GYRO_SENS_SEL;
//                break;
//            case BMI2_AUX:
//                *sensor_sel |= BMI2_AUX_SENS_SEL;
//                break;
//            case BMI2_TEMP:
//                *sensor_sel |= BMI2_TEMP_SENS_SEL;
//                break;
//            default:
//                rslt = BMI2_E_INVALID_SENSOR;
//                break;
//        }
//    }
//
//    return rslt;
//}

/*!
 * @brief This internal API enables the selected sensor/features.
 */
//XBYTESIG DIOI2C6AXISTRACKINGBMI270::sensor_enable(XQWORD sensor_sel, struct bmi2_dev *dev)
//{
//    /* Variable to define error */
//    XBYTESIG rslt;
//
//    /* Variable to store register values */
//    XBYTE reg_data = 0;
//
//    rslt = bmi2_get_regs(BMI2_PWR_CTRL_ADDR, &reg_data, 1, dev);
//    if(rslt == BMI2_OK)
//    {
//        /* Enable accelerometer */
//        if(sensor_sel & BMI2_ACCEL_SENS_SEL)
//        {
//            reg_data = BMI2_SET_BITS(reg_data, BMI2_ACC_EN, BMI2_ENABLE);
//        }
//
//        /* Enable gyroscope */
//        if(sensor_sel & BMI2_GYRO_SENS_SEL)
//        {
//            reg_data = BMI2_SET_BITS(reg_data, BMI2_GYR_EN, BMI2_ENABLE);
//        }
//
//        /* Enable auxiliary sensor */
//        if(sensor_sel & BMI2_AUX_SENS_SEL)
//        {
//            reg_data = BMI2_SET_BIT_POS0(reg_data, BMI2_AUX_EN, BMI2_ENABLE);
//        }
//
//        /* Enable temperature sensor */
//        if(sensor_sel & BMI2_TEMP_SENS_SEL)
//        {
//            reg_data = BMI2_SET_BITS(reg_data, BMI2_TEMP_EN, BMI2_ENABLE);
//        }
//
//        /* Enable the sensors that are set in the power control register */
//        if(sensor_sel & BMI2_MAIN_SENSORS)
//        {
//            rslt = bmi2_set_regs(BMI2_PWR_CTRL_ADDR, &reg_data, 1, dev);
//        }
//    }
//
//    return rslt;
//}

/*!
 * @brief This internal API disables the selected sensors/features.
 */
//XBYTESIG DIOI2C6AXISTRACKINGBMI270::sensor_disable(XQWORD sensor_sel, struct bmi2_dev *dev)
//{
//    /* Variable to define error */
//    XBYTESIG rslt;
//
//    /* Variable to store register values */
//    XBYTE reg_data = 0;
//
//    rslt = bmi2_get_regs(BMI2_PWR_CTRL_ADDR, &reg_data, 1, dev);
//    if(rslt == BMI2_OK)
//    {
//        /* Disable accelerometer */
//        if(sensor_sel & BMI2_ACCEL_SENS_SEL)
//        {
//            reg_data = BMI2_SET_BIT_VAL0(reg_data, BMI2_ACC_EN);
//        }
//
//        /* Disable gyroscope */
//        if(sensor_sel & BMI2_GYRO_SENS_SEL)
//        {
//            reg_data = BMI2_SET_BIT_VAL0(reg_data, BMI2_GYR_EN);
//        }
//
//        /* Disable auxiliary sensor */
//        if(sensor_sel & BMI2_AUX_SENS_SEL)
//        {
//            reg_data = BMI2_SET_BIT_VAL0(reg_data, BMI2_AUX_EN);
//        }
//
//        /* Disable temperature sensor */
//        if(sensor_sel & BMI2_TEMP_SENS_SEL)
//        {
//            reg_data = BMI2_SET_BIT_VAL0(reg_data, BMI2_TEMP_EN);
//        }
//
//        /* Enable the sensors that are set in the power control register */
//        if(sensor_sel & BMI2_MAIN_SENSORS)
//        {
//            rslt = bmi2_set_regs(BMI2_PWR_CTRL_ADDR, &reg_data, 1, dev);
//        }
//    }
//
//    return rslt;
//}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi270_init(struct bmi2_dev *dev)
* @brief      This API:
* @ingroup    DATAIO
* 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/

XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi270_init(struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if(rslt == BMI2_OK)
    {
        /* Assign chip id of BMI270 */
        dev->chip_id = DIOI2C6AXISTRACKINGBMI270_CHIP_ID;

        /* get the size of config array */
        dev->config_size = sizeof(bmi270_config_file);

        /* Enable the variant specific features if any */
        dev->variant_feature = BMI2_GYRO_CROSS_SENS_ENABLE | BMI2_CRT_RTOSK_ENABLE;

        /* An extra dummy byte is read during SPI read */
        if(dev->intf == BMI2_SPI_INTF)
        {
            dev->dummy_byte = 1;
        }
        else
        {
            dev->dummy_byte = 0;
        }

        /* If configuration file pointer is not assigned any address */
        if(!dev->config_file_ptr)
        {
            /* Give the address of the configuration file array to
             * the device pointer
             */
            dev->config_file_ptr = bmi270_config_file;
        }

        /* Initialize BMI2 sensor */
        rslt = bmi2_sec_init(dev);
        if(rslt == BMI2_OK)
        {
            /* Assign the offsets of the feature input
             * configuration to the device structure
             */
            dev->feat_config = bmi270_feat_in;

            /* Assign the offsets of the feature output to
             * the device structure
             */
            dev->feat_output = bmi270_feat_out;

            /* Assign the maximum number of pages to the
             * device structure
             */
            dev->page_max = DIOI2C6AXISTRACKINGBMI270_MAX_PAGE_NUM;

            /* Assign maximum number of input sensors/
             * features to device structure
             */
            dev->input_sens = DIOI2C6AXISTRACKINGBMI270_MAX_FEAT_IN;

            /* Assign maximum number of output sensors/
             * features to device structure
             */
            dev->out_sens = DIOI2C6AXISTRACKINGBMI270_MAX_FEAT_OUT;

            /* Assign the offsets of the feature interrupt
             * to the device structure
             */
            dev->map_int = bmi270_map_int;

            /* Assign maximum number of feature interrupts
             * to device structure
             */
            dev->sens_int_map = DIOI2C6AXISTRACKINGBMI270_MAX_INT_MAP;

            /* Get the gyroscope cross axis sensitivity */
            rslt = bmi2_get_gyro_cross_sense(dev);
        }
    }

    return rslt;
}

/*!
 * @brief This API selects the sensors/features to be enabled.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi270_sensor_enable(const XBYTE *sens_list, XBYTE n_sens, struct bmi2_dev *dev)
* @brief      Bmi270 sensor enable
* @ingroup    DATAIO
* 
* @param[in]  sens_list : 
* @param[in]  n_sens : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi270_sensor_enable(const XBYTE *sens_list, XBYTE n_sens, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to select sensor */
    XQWORD sensor_sel = 0;

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if((rslt == BMI2_OK) && (sens_list != NULL))
    {
        /* Get the selected sensors */
        rslt = select_sensor(sens_list, n_sens, &sensor_sel);
        if(rslt == BMI2_OK)
        {
            /* Enable the selected sensors */
            rslt = sensor_enable(sensor_sel, dev);
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API selects the sensors/features to be disabled.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi270_sensor_disable(const XBYTE *sens_list, XBYTE n_sens, struct bmi2_dev *dev)
* @brief      Bmi270 sensor disable
* @ingroup    DATAIO
* 
* @param[in]  sens_list : 
* @param[in]  n_sens : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi270_sensor_disable(const XBYTE *sens_list, XBYTE n_sens, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to select sensor */
    XQWORD sensor_sel = 0;

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if((rslt == BMI2_OK) && (sens_list != NULL))
    {
        /* Get the selected sensors */
        rslt = select_sensor(sens_list, n_sens, &sensor_sel);
        if(rslt == BMI2_OK)
        {
            /* Disable the selected sensors */
            rslt = sensor_disable(sensor_sel, dev);
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API sets the sensor/feature configuration.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi270_set_sensor_config(struct bmi2_sens_config *sens_cfg, XBYTE n_sens, struct bmi2_dev *dev)
* @brief      Bmi270 set sensor config
* @ingroup    DATAIO
* 
* @param[in]  sens_cfg : 
* @param[in]  n_sens : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi270_set_sensor_config(struct bmi2_sens_config *sens_cfg, XBYTE n_sens, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to define loop */
    XBYTE loop;

    /* Variable to get the status of advance power save */
    XBYTE aps_stat = 0;

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if((rslt == BMI2_OK) && (sens_cfg != NULL))
    {
        /* Get status of advance power save mode */
        aps_stat = dev->aps_status;

        for (loop = 0; loop < n_sens; loop++)
        {
            if((sens_cfg[loop].type == BMI2_ACCEL) || (sens_cfg[loop].type == BMI2_GYRO) ||
                (sens_cfg[loop].type == BMI2_AUX) || (sens_cfg[loop].type == BMI2_GYRO_GAIN_UPDATE))
            {
                rslt = bmi2_set_sensor_config(&sens_cfg[loop], 1, dev);
            }
            else
            {
                /* Disable Advance power save if enabled for auxiliary
                 * and feature configurations
                 */
                if(aps_stat == BMI2_ENABLE)
                {
                    /* Disable advance power save if
                     * enabled
                     */
                    rslt = bmi2_set_adv_power_save(BMI2_DISABLE, dev);
                }

                if(rslt == BMI2_OK)
                {
                    switch (sens_cfg[loop].type)
                    {
                        /* Set any motion configuration */
                        case BMI2_ANY_MOTION:
                            rslt = set_any_motion_config(&sens_cfg[loop].cfg.any_motion, dev);
                            break;

                        /* Set no motion configuration */
                        case BMI2_NO_MOTION:
                            rslt = set_no_motion_config(&sens_cfg[loop].cfg.no_motion, dev);
                            break;

                        /* Set sig-motion configuration */
                        case BMI2_SIG_MOTION:
                            rslt = set_sig_motion_config(&sens_cfg[loop].cfg.sig_motion, dev);
                            break;

                        /* Set the step counter parameters */
                        case BMI2_STEP_COUNTER_PARAMS:
                            rslt = set_step_count_params_config(sens_cfg[loop].cfg.step_counter_params, dev);
                            break;

                        /* Set step counter/detector/activity configuration */
                        case BMI2_STEP_DETECTOR:
                        case BMI2_STEP_COUNTER:
                        case BMI2_STEP_ACTIVITY:
                            rslt = set_step_config(&sens_cfg[loop].cfg.step_counter, dev);
                            break;

                        /* Set the wrist gesture configuration */
                        case BMI2_WRIST_GESTURE:
                            rslt = set_wrist_gest_config(&sens_cfg[loop].cfg.wrist_gest, dev);
                            break;

                        /* Set the wrist wear wake-up configuration */
                        case BMI2_WRIST_WEAR_WAKE_UP:
                            rslt = set_wrist_wear_wake_up_config(&sens_cfg[loop].cfg.wrist_wear_wake_up, dev);
                            break;

                        default:
                            rslt = BMI2_E_INVALID_SENSOR;
                            break;
                    }
                }

                /* Return error if any of the set configurations fail */
                if(rslt != BMI2_OK)
                {
                    break;
                }
            }
        }

        /* Enable Advance power save if disabled while configuring and
         * not when already disabled
         */
        if((aps_stat == BMI2_ENABLE) && (rslt == BMI2_OK))
        {
            rslt = bmi2_set_adv_power_save(BMI2_ENABLE, dev);
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API gets the sensor/feature configuration.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi270_get_sensor_config(struct bmi2_sens_config *sens_cfg, XBYTE n_sens, struct bmi2_dev *dev)
* @brief      Bmi270 get sensor config
* @ingroup    DATAIO
* 
* @param[in]  sens_cfg : 
* @param[in]  n_sens : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi270_get_sensor_config(struct bmi2_sens_config *sens_cfg, XBYTE n_sens, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to define loop */
    XBYTE loop;

    /* Variable to get the status of advance power save */
    XBYTE aps_stat = 0;

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if((rslt == BMI2_OK) && (sens_cfg != NULL))
    {
        /* Get status of advance power save mode */
        aps_stat = dev->aps_status;
        for (loop = 0; loop < n_sens; loop++)
        {
            if((sens_cfg[loop].type == BMI2_ACCEL) || (sens_cfg[loop].type == BMI2_GYRO) ||
                (sens_cfg[loop].type == BMI2_AUX) || (sens_cfg[loop].type == BMI2_GYRO_GAIN_UPDATE))
            {
                rslt = bmi2_get_sensor_config(&sens_cfg[loop], 1, dev);
            }
            else
            {
                /* Disable Advance power save if enabled for auxiliary
                 * and feature configurations
                 */
                if((sens_cfg[loop].type >= BMI2_MAIN_SENS_MAX_NUM) || (sens_cfg[loop].type == BMI2_AUX))
                {

                    if(aps_stat == BMI2_ENABLE)
                    {
                        /* Disable advance power save if
                         * enabled
                         */
                        rslt = bmi2_set_adv_power_save(BMI2_DISABLE, dev);
                    }
                }

                if(rslt == BMI2_OK)
                {
                    switch (sens_cfg[loop].type)
                    {
                        /* Get sig-motion configuration */
                        case BMI2_SIG_MOTION:
                            rslt = get_sig_motion_config(&sens_cfg[loop].cfg.sig_motion, dev);
                            break;

                        /* Get any motion configuration */
                        case BMI2_ANY_MOTION:
                            rslt = get_any_motion_config(&sens_cfg[loop].cfg.any_motion, dev);
                            break;

                        /* Get no motion configuration */
                        case BMI2_NO_MOTION:
                            rslt = get_no_motion_config(&sens_cfg[loop].cfg.no_motion, dev);
                            break;

                        /* Set the step counter parameters */
                        case BMI2_STEP_COUNTER_PARAMS:
                            rslt = get_step_count_params_config(sens_cfg[loop].cfg.step_counter_params, dev);
                            break;

                        /* Get step counter/detector/activity configuration */
                        case BMI2_STEP_DETECTOR:
                        case BMI2_STEP_COUNTER:
                        case BMI2_STEP_ACTIVITY:
                            rslt = get_step_config(&sens_cfg[loop].cfg.step_counter, dev);
                            break;

                        /* Get the wrist gesture configuration */
                        case BMI2_WRIST_GESTURE:
                            rslt = get_wrist_gest_config(&sens_cfg[loop].cfg.wrist_gest, dev);
                            break;

                        /* Get the wrist wear wake-up configuration */
                        case BMI2_WRIST_WEAR_WAKE_UP:
                            rslt = get_wrist_wear_wake_up_config(&sens_cfg[loop].cfg.wrist_wear_wake_up, dev);
                            break;

                        default:
                            rslt = BMI2_E_INVALID_SENSOR;
                            break;
                    }
                }

                /* Return error if any of the get configurations fail */
                if(rslt != BMI2_OK)
                {
                    break;
                }
            }
        }

        /* Enable Advance power save if disabled while configuring and
         * not when already disabled
         */
        if((aps_stat == BMI2_ENABLE) && (rslt == BMI2_OK))
        {
            rslt = bmi2_set_adv_power_save(BMI2_ENABLE, dev);
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API gets the sensor/feature data for accelerometer, gyroscope,
 * auxiliary sensor, step counter, high-g, gyroscope user-gain update,
 * orientation, gyroscope cross sensitivity and error status for NVM and VFRM.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi270_get_sensor_data(struct bmi2_sensor_data *sensor_data, XBYTE n_sens, struct bmi2_dev *dev)
* @brief      Bmi270 get sensor data
* @ingroup    DATAIO
* 
* @param[in]  sensor_data : 
* @param[in]  n_sens : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi270_get_sensor_data(struct bmi2_sensor_data *sensor_data, XBYTE n_sens, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to define loop */
    XBYTE loop;

    /* Variable to get the status of advance power save */
    XBYTE aps_stat = 0;

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if((rslt == BMI2_OK) && (sensor_data != NULL))
    {
        /* Get status of advance power save mode */
        aps_stat = dev->aps_status;
        for (loop = 0; loop < n_sens; loop++)
        {
            if((sensor_data[loop].type == BMI2_ACCEL) || (sensor_data[loop].type == BMI2_GYRO) ||
                (sensor_data[loop].type == BMI2_AUX) || (sensor_data[loop].type == BMI2_GYRO_GAIN_UPDATE) ||
                (sensor_data[loop].type == BMI2_GYRO_CROSS_SENSE))
            {
                rslt = bmi2_get_sensor_data(&sensor_data[loop], 1, dev);
            }
            else
            {
                /* Disable Advance power save if enabled for feature
                 * configurations
                 */
                if(sensor_data[loop].type >= BMI2_MAIN_SENS_MAX_NUM)
                {
                    if(aps_stat == BMI2_ENABLE)
                    {
                        /* Disable advance power save if
                         * enabled
                         */
                        rslt = bmi2_set_adv_power_save(BMI2_DISABLE, dev);
                    }
                }

                if(rslt == BMI2_OK)
                {
                    switch (sensor_data[loop].type)
                    {
                        case BMI2_STEP_COUNTER:

                            /* Get step counter output */
                            rslt = get_step_counter_output(&sensor_data[loop].sens_data.step_counter_output, dev);
                            break;
                        case BMI2_STEP_ACTIVITY:

                            /* Get step activity output */
                            rslt = get_step_activity_output(&sensor_data[loop].sens_data.activity_output, dev);
                            break;
                        case BMI2_NVM_STATUS:

                            /* Get NVM error status  */
                            rslt = get_nvm_error_status(&sensor_data[loop].sens_data.nvm_status, dev);
                            break;
                        case BMI2_VFRM_STATUS:

                            /* Get VFRM error status  */
                            rslt = get_vfrm_error_status(&sensor_data[loop].sens_data.vfrm_status, dev);
                            break;
                        case BMI2_WRIST_GESTURE:

                            /* Get wrist gesture status  */
                            rslt = get_wrist_gest_status(&sensor_data[loop].sens_data.wrist_gesture_output, dev);
                            break;
                        default:
                            rslt = BMI2_E_INVALID_SENSOR;
                            break;
                    }

                    /* Return error if any of the get sensor data fails */
                    if(rslt != BMI2_OK)
                    {
                        break;
                    }
                }
            }

            /* Enable Advance power save if disabled while
             * configuring and not when already disabled
             */
            if((aps_stat == BMI2_ENABLE) && (rslt == BMI2_OK))
            {
                rslt = bmi2_set_adv_power_save(BMI2_ENABLE, dev);
            }
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API updates the gyroscope user-gain.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi270_update_gyro_user_gain(const struct bmi2_gyro_user_gain_config *user_gain, struct bmi2_dev *dev)
* @brief      Bmi270 update gyro user gain
* @ingroup    DATAIO
* 
* @param[in]  user_gain : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi270_update_gyro_user_gain(const struct bmi2_gyro_user_gain_config *user_gain, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to select sensor */
    XBYTE sens_sel[2] = { BMI2_GYRO, BMI2_GYRO_GAIN_UPDATE };

    /* Structure to define sensor configurations */
    struct bmi2_sens_config sens_cfg;

    /* Variable to store status of user-gain update module */
    XBYTE status = 0;

    /* Variable to define count */
    XBYTE count = 100;

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if((rslt == BMI2_OK) && (user_gain != NULL))
    {
        /* Select type of feature */
        sens_cfg.type = BMI2_GYRO_GAIN_UPDATE;

        /* Get the user gain configurations */
        rslt = bmi270_get_sensor_config(&sens_cfg, 1, dev);
        if(rslt == BMI2_OK)
        {
            /* Get the user-defined ratio */
            sens_cfg.cfg.gyro_gain_update = *user_gain;

            /* Set rate ratio for all axes */
            rslt = bmi270_set_sensor_config(&sens_cfg, 1, dev);
        }

        /* Disable gyroscope */
        if(rslt == BMI2_OK)
        {
            rslt = bmi270_sensor_disable(&sens_sel[0], 1, dev);
        }

        /* Enable gyroscope user-gain update module */
        if(rslt == BMI2_OK)
        {
            rslt = bmi270_sensor_enable(&sens_sel[1], 1, dev);
        }

        /* Set the command to trigger the computation */
        if(rslt == BMI2_OK)
        {
            rslt = bmi2_set_command_register(BMI2_USR_GAIN_CMD, dev);
        }

        if(rslt == BMI2_OK)
        {
            /* Poll until enable bit of user-gain update is 0 */
            while (count--)
            {
                rslt = get_user_gain_upd_status(&status, dev);
                if((rslt == BMI2_OK) && (status == 0))
                {
                    /* Enable compensation of gain defined
                     * in the GAIN register
                     */
                    rslt = enable_gyro_gain(BMI2_ENABLE, dev);

                    /* Enable gyroscope */
                    if(rslt == BMI2_OK)
                    {
                        rslt = bmi270_sensor_enable(&sens_sel[0], 1, dev);
                    }

                    break;
                }

                bmi2_delay_us(10000, dev->intf_ptr);
            }

            /* Return error if user-gain update is failed */
            if((rslt == BMI2_OK) && (status != 0))
            {
                rslt = BMI2_E_GYR_USER_GAIN_UPD_FAIL;
            }
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API reads the compensated gyroscope user-gain values.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi270_read_gyro_user_gain(struct bmi2_gyro_user_gain_data *gyr_usr_gain, struct bmi2_dev *dev)
* @brief      Bmi270 read gyro user gain
* @ingroup    DATAIO
* 
* @param[in]  gyr_usr_gain : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi270_read_gyro_user_gain(struct bmi2_gyro_user_gain_data *gyr_usr_gain, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to define register data */
    XBYTE reg_data[3] = { 0 };

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if((rslt == BMI2_OK) && (gyr_usr_gain != NULL))
    {
        /* Get the gyroscope compensated gain values */
        rslt = bmi2_get_regs(BMI2_GYR_USR_GAIN_0_ADDR, reg_data, 3, dev);
        if(rslt == BMI2_OK)
        {
            /* Gyroscope user gain correction X-axis */
            gyr_usr_gain->x = (XBYTESIG)BMI2_GET_BIT_POS0(reg_data[0], BMI2_GYR_USR_GAIN_X);

            /* Gyroscope user gain correction Y-axis */
            gyr_usr_gain->y = (XBYTESIG)BMI2_GET_BIT_POS0(reg_data[1], BMI2_GYR_USR_GAIN_Y);

            /* Gyroscope user gain correction z-axis */
            gyr_usr_gain->z = (XBYTESIG)BMI2_GET_BIT_POS0(reg_data[2], BMI2_GYR_USR_GAIN_Z);
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 * @brief This API maps/unmaps feature interrupts to that of interrupt pins.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi270_map_feat_int(const struct bmi2_sens_int_config *sens_int, XBYTE n_sens, struct bmi2_dev *dev)
* @brief      Bmi270 map feat int
* @ingroup    DATAIO
* 
* @param[in]  sens_int : 
* @param[in]  n_sens : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::bmi270_map_feat_int(const struct bmi2_sens_int_config *sens_int, XBYTE n_sens, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to define loop */
    XBYTE loop;

    /* Null-pointer check */
    rslt = null_ptr_check(dev);
    if((rslt == BMI2_OK) && (sens_int != NULL))
    {
        for (loop = 0; loop < n_sens; loop++)
        {
            switch (sens_int[loop].type)
            {
                case BMI2_SIG_MOTION:
                case BMI2_WRIST_GESTURE:
                case BMI2_ANY_MOTION:
                case BMI2_NO_MOTION:
                case BMI2_STEP_COUNTER:
                case BMI2_STEP_DETECTOR:
                case BMI2_STEP_ACTIVITY:
                case BMI2_WRIST_WEAR_WAKE_UP:

                    rslt = bmi2_map_feat_int(sens_int[loop].type, sens_int[loop].hw_int_pin, dev);
                    break;
                default:
                    rslt = BMI2_E_INVALID_SENSOR;
                    break;
            }

            /* Return error if interrupt mapping fails */
            if(rslt != BMI2_OK)
            {
                break;
            }
        }
    }
    else
    {
        rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::select_sensor(const XBYTE *sens_list, XBYTE n_sens, XQWORD *sensor_sel)
* @brief      This internal API selects the sensor/features to be enabled or
* @ingroup    DATAIO
* 
* @param[in]  sens_list : 
* @param[in]  n_sens : 
* @param[in]  sensor_sel : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::select_sensor(const XBYTE *sens_list, XBYTE n_sens, XQWORD *sensor_sel)
{
    /* Variable to define error */
    XBYTESIG rslt = BMI2_OK;

    /* Variable to define loop */
    XBYTE count;

    for (count = 0; count < n_sens; count++)
    {
        switch (sens_list[count])
        {
            case BMI2_ACCEL   : *sensor_sel |= BMI2_ACCEL_SENS_SEL;    break;
            case BMI2_GYRO    :
                *sensor_sel |= BMI2_GYRO_SENS_SEL;
                break;
            case BMI2_AUX:
                *sensor_sel |= BMI2_AUX_SENS_SEL;
                break;
            case BMI2_TEMP:
                *sensor_sel |= BMI2_TEMP_SENS_SEL;
                break;
            case BMI2_SIG_MOTION:
                *sensor_sel |= BMI2_SIG_MOTION_SEL;
                break;
            case BMI2_ANY_MOTION:
                *sensor_sel |= BMI2_ANY_MOT_SEL;
                break;
            case BMI2_NO_MOTION:
                *sensor_sel |= BMI2_NO_MOT_SEL;
                break;
            case BMI2_STEP_DETECTOR:
                *sensor_sel |= BMI2_STEP_DETECT_SEL;
                break;
            case BMI2_STEP_COUNTER:
                *sensor_sel |= BMI2_STEP_COUNT_SEL;
                break;
            case BMI2_STEP_ACTIVITY:
                *sensor_sel |= BMI2_STEP_ACT_SEL;
                break;
            case BMI2_GYRO_GAIN_UPDATE:
                *sensor_sel |= BMI2_GYRO_GAIN_UPDATE_SEL;
                break;
            case BMI2_GYRO_SELF_OFF:
                *sensor_sel |= BMI2_GYRO_SELF_OFF_SEL;
                break;
            case BMI2_WRIST_GESTURE:
                *sensor_sel |= BMI2_WRIST_GEST_SEL;
                break;
            case BMI2_WRIST_WEAR_WAKE_UP:
                *sensor_sel |= BMI2_WRIST_WEAR_WAKE_UP_SEL;
                break;
            default:
                rslt = BMI2_E_INVALID_SENSOR;
                break;
        }
    }

    return rslt;
}

/*!
 * @brief This internal API enables the selected sensor/features.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::sensor_enable(XQWORD sensor_sel, struct bmi2_dev *dev)
* @brief      Sensor enable
* @ingroup    DATAIO
* 
* @param[in]  sensor_sel : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::sensor_enable(XQWORD sensor_sel, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to store register values */
    XBYTE reg_data = 0;

    /* Variable to define loop */
    XBYTE loop = 1;

    /* Variable to get the status of advance power save */
    XBYTE aps_stat = 0;

    rslt = bmi2_get_regs(BMI2_PWR_CTRL_ADDR, &reg_data, 1, dev);
    if(rslt == BMI2_OK)
    {
        /* Enable accelerometer */
        if(sensor_sel & BMI2_ACCEL_SENS_SEL)
        {
            reg_data = BMI2_SET_BITS(reg_data, BMI2_ACC_EN, BMI2_ENABLE);
        }

        /* Enable gyroscope */
        if(sensor_sel & BMI2_GYRO_SENS_SEL)
        {
            reg_data = BMI2_SET_BITS(reg_data, BMI2_GYR_EN, BMI2_ENABLE);
        }

        /* Enable auxiliary sensor */
        if(sensor_sel & BMI2_AUX_SENS_SEL)
        {
            reg_data = BMI2_SET_BIT_POS0(reg_data, BMI2_AUX_EN, BMI2_ENABLE);
        }

        /* Enable temperature sensor */
        if(sensor_sel & BMI2_TEMP_SENS_SEL)
        {
            reg_data = BMI2_SET_BITS(reg_data, BMI2_TEMP_EN, BMI2_ENABLE);
        }

        /* Enable the sensors that are set in the power control register */
        if(sensor_sel & BMI2_MAIN_SENSORS)
        {
            rslt = bmi2_set_regs(BMI2_PWR_CTRL_ADDR, &reg_data, 1, dev);
        }
    }

    if((rslt == BMI2_OK) && (sensor_sel & ~(BMI2_MAIN_SENSORS)))
    {
        /* Get status of advance power save mode */
        aps_stat = dev->aps_status;
        if(aps_stat == BMI2_ENABLE)
        {
            /* Disable advance power save if enabled */
            rslt = bmi2_set_adv_power_save(BMI2_DISABLE, dev);
        }

        if(rslt == BMI2_OK)
        {
            while (loop--)
            {
                /* Enable sig-motion feature */
                if(sensor_sel & BMI2_SIG_MOTION_SEL)
                {
                    rslt = set_sig_motion(BMI2_ENABLE, dev);
                    if(rslt == BMI2_OK)
                    {
                        dev->sens_en_stat |= BMI2_SIG_MOTION_SEL;
                    }
                    else
                    {
                        break;
                    }
                }

                /* Enable any motion feature */
                if(sensor_sel & BMI2_ANY_MOT_SEL)
                {
                    rslt = set_any_motion(BMI2_ENABLE, dev);
                    if(rslt == BMI2_OK)
                    {
                        dev->sens_en_stat |= BMI2_ANY_MOT_SEL;
                    }
                    else
                    {
                        break;
                    }
                }

                /* Enable no motion feature */
                if(sensor_sel & BMI2_NO_MOT_SEL)
                {
                    rslt = set_no_motion(BMI2_ENABLE, dev);
                    if(rslt == BMI2_OK)
                    {
                        dev->sens_en_stat |= BMI2_NO_MOT_SEL;
                    }
                    else
                    {
                        break;
                    }
                }

                /* Enable step detector feature */
                if(sensor_sel & BMI2_STEP_DETECT_SEL)
                {
                    rslt = set_step_detector(BMI2_ENABLE, dev);
                    if(rslt == BMI2_OK)
                    {
                        dev->sens_en_stat |= BMI2_STEP_DETECT_SEL;
                    }
                    else
                    {
                        break;
                    }
                }

                /* Enable step counter feature */
                if(sensor_sel & BMI2_STEP_COUNT_SEL)
                {
                    rslt = set_step_counter(BMI2_ENABLE, dev);
                    if(rslt == BMI2_OK)
                    {
                        dev->sens_en_stat |= BMI2_STEP_COUNT_SEL;
                    }
                    else
                    {
                        break;
                    }
                }

                /* Enable step activity feature */
                if(sensor_sel & BMI2_STEP_ACT_SEL)
                {
                    rslt = set_step_activity(BMI2_ENABLE, dev);
                    if(rslt == BMI2_OK)
                    {
                        dev->sens_en_stat |= BMI2_STEP_ACT_SEL;
                    }
                    else
                    {
                        break;
                    }
                }

                /* Enable gyroscope user gain */
                if(sensor_sel & BMI2_GYRO_GAIN_UPDATE_SEL)
                {
                    rslt = set_gyro_user_gain(BMI2_ENABLE, dev);
                    if(rslt == BMI2_OK)
                    {
                        dev->sens_en_stat |= BMI2_GYRO_GAIN_UPDATE_SEL;
                    }
                    else
                    {
                        break;
                    }
                }

                /* Enable gyroscope self-offset correction feature */
                if(sensor_sel & BMI2_GYRO_SELF_OFF_SEL)
                {
                    rslt = set_gyro_self_offset_corr(BMI2_ENABLE, dev);
                    if(rslt == BMI2_OK)
                    {
                        dev->sens_en_stat |= BMI2_GYRO_SELF_OFF_SEL;
                    }
                    else
                    {
                        break;
                    }
                }

                /* Enable wrist gesture feature for wearable variant */
                if(sensor_sel & BMI2_WRIST_GEST_SEL)
                {
                    rslt = set_wrist_gesture(BMI2_ENABLE, dev);
                    if(rslt == BMI2_OK)
                    {
                        dev->sens_en_stat |= BMI2_WRIST_GEST_SEL;
                    }
                    else
                    {
                        break;
                    }
                }

                /* Enable wrist wear wake-up feature */
                if(sensor_sel & BMI2_WRIST_WEAR_WAKE_UP_SEL)
                {
                    rslt = set_wrist_wear_wake_up(BMI2_ENABLE, dev);
                    if(rslt == BMI2_OK)
                    {
                        dev->sens_en_stat |= BMI2_WRIST_WEAR_WAKE_UP_SEL;
                    }
                    else
                    {
                        break;
                    }
                }
            }

            /* Enable Advance power save if disabled while
             * configuring and not when already disabled
             */
            if((aps_stat == BMI2_ENABLE) && (rslt == BMI2_OK))
            {
                rslt = bmi2_set_adv_power_save(BMI2_ENABLE, dev);
            }
        }
    }

    return rslt;
}

/*!
 * @brief This internal API disables the selected sensors/features.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::sensor_disable(XQWORD sensor_sel, struct bmi2_dev *dev)
* @brief      Sensor disable
* @ingroup    DATAIO
* 
* @param[in]  sensor_sel : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::sensor_disable(XQWORD sensor_sel, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to store register values */
    XBYTE reg_data = 0;

    /* Variable to define loop */
    XBYTE loop = 1;

    /* Variable to get the status of advance power save */
    XBYTE aps_stat = 0;

    rslt = bmi2_get_regs(BMI2_PWR_CTRL_ADDR, &reg_data, 1, dev);
    if(rslt == BMI2_OK)
    {
        /* Disable accelerometer */
        if(sensor_sel & BMI2_ACCEL_SENS_SEL)
        {
            reg_data = BMI2_SET_BIT_VAL0(reg_data, BMI2_ACC_EN);
        }

        /* Disable gyroscope */
        if(sensor_sel & BMI2_GYRO_SENS_SEL)
        {
            reg_data = BMI2_SET_BIT_VAL0(reg_data, BMI2_GYR_EN);
        }

        /* Disable auxiliary sensor */
        if(sensor_sel & BMI2_AUX_SENS_SEL)
        {
            reg_data = BMI2_SET_BIT_VAL0(reg_data, BMI2_AUX_EN);
        }

        /* Disable temperature sensor */
        if(sensor_sel & BMI2_TEMP_SENS_SEL)
        {
            reg_data = BMI2_SET_BIT_VAL0(reg_data, BMI2_TEMP_EN);
        }

        /* Disable the sensors that are set in the power control register */
        if(sensor_sel & BMI2_MAIN_SENSORS)
        {
            rslt = bmi2_set_regs(BMI2_PWR_CTRL_ADDR, &reg_data, 1, dev);
        }
    }

    if((rslt == BMI2_OK) && (sensor_sel & ~(BMI2_MAIN_SENSORS)))
    {
        /* Get status of advance power save mode */
        aps_stat = dev->aps_status;
        if(aps_stat == BMI2_ENABLE)
        {
            /* Disable advance power save if enabled */
            rslt = bmi2_set_adv_power_save(BMI2_DISABLE, dev);
        }

        if(rslt == BMI2_OK)
        {
            while (loop--)
            {
                /* Disable sig-motion feature */
                if(sensor_sel & BMI2_SIG_MOTION_SEL)
                {
                    rslt = set_sig_motion(BMI2_DISABLE, dev);
                    if(rslt == BMI2_OK)
                    {
                        dev->sens_en_stat &= ~BMI2_SIG_MOTION_SEL;
                    }
                    else
                    {
                        break;
                    }
                }

                /* Disable any-motion feature */
                if(sensor_sel & BMI2_ANY_MOT_SEL)
                {
                    rslt = set_any_motion(BMI2_DISABLE, dev);
                    if(rslt == BMI2_OK)
                    {
                        dev->sens_en_stat &= ~BMI2_ANY_MOT_SEL;
                    }
                    else
                    {
                        break;
                    }
                }

                /* Disable no-motion feature */
                if(sensor_sel & BMI2_NO_MOT_SEL)
                {
                    rslt = set_no_motion(BMI2_DISABLE, dev);
                    if(rslt == BMI2_OK)
                    {
                        dev->sens_en_stat &= ~BMI2_NO_MOT_SEL;
                    }
                    else
                    {
                        break;
                    }
                }

                /* Disable step detector feature */
                if(sensor_sel & BMI2_STEP_DETECT_SEL)
                {
                    rslt = set_step_detector(BMI2_DISABLE, dev);
                    if(rslt == BMI2_OK)
                    {
                        dev->sens_en_stat &= ~BMI2_STEP_DETECT_SEL;
                    }
                    else
                    {
                        break;
                    }
                }

                /* Disable step counter feature */
                if(sensor_sel & BMI2_STEP_COUNT_SEL)
                {
                    rslt = set_step_counter(BMI2_DISABLE, dev);
                    if(rslt == BMI2_OK)
                    {
                        dev->sens_en_stat &= ~BMI2_STEP_COUNT_SEL;
                    }
                    else
                    {
                        break;
                    }
                }

                /* Disable step activity feature */
                if(sensor_sel & BMI2_STEP_ACT_SEL)
                {
                    rslt = set_step_activity(BMI2_DISABLE, dev);
                    if(rslt == BMI2_OK)
                    {
                        dev->sens_en_stat &= ~BMI2_STEP_ACT_SEL;
                    }
                    else
                    {
                        break;
                    }
                }

                /* Disable gyroscope user gain */
                if(sensor_sel & BMI2_GYRO_GAIN_UPDATE_SEL)
                {
                    rslt = set_gyro_user_gain(BMI2_DISABLE, dev);
                    if(rslt == BMI2_OK)
                    {
                        dev->sens_en_stat &= ~BMI2_GYRO_GAIN_UPDATE_SEL;
                    }
                    else
                    {
                        break;
                    }
                }

                /* Disable gyroscope self-offset correction feature */
                if(sensor_sel & BMI2_GYRO_SELF_OFF_SEL)
                {
                    rslt = set_gyro_self_offset_corr(BMI2_DISABLE, dev);
                    if(rslt == BMI2_OK)
                    {
                        dev->sens_en_stat &= ~BMI2_GYRO_SELF_OFF_SEL;
                    }
                    else
                    {
                        break;
                    }
                }

                /* Disable wrist gesture feature for wearable variant*/
                if(sensor_sel & BMI2_WRIST_GEST_SEL)
                {
                    rslt = set_wrist_gesture(BMI2_DISABLE, dev);
                    if(rslt == BMI2_OK)
                    {
                        dev->sens_en_stat &= ~BMI2_WRIST_GEST_SEL;
                    }
                    else
                    {
                        break;
                    }
                }

                /* Enable wrist wear wake-up feature */
                if(sensor_sel & BMI2_WRIST_WEAR_WAKE_UP_SEL)
                {
                    rslt = set_wrist_wear_wake_up(BMI2_DISABLE, dev);
                    if(rslt == BMI2_OK)
                    {
                        dev->sens_en_stat &= ~BMI2_WRIST_WEAR_WAKE_UP_SEL;
                    }
                    else
                    {
                        break;
                    }
                }

                /* Enable Advance power save if disabled while
                 * configuring and not when already disabled
                 */
                if((aps_stat == BMI2_ENABLE) && (rslt == BMI2_OK))
                {
                    rslt = bmi2_set_adv_power_save(BMI2_ENABLE, dev);
                }
            }
        }
    }

    return rslt;
}

/*!
 * @brief This internal API is used to enable/disable any motion feature.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_any_motion(XBYTE enable, struct bmi2_dev *dev)
* @brief      Set any motion
* @ingroup    DATAIO
* 
* @param[in]  enable : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_any_motion(XBYTE enable, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Array to define the feature configuration */
    XBYTE feat_config[BMI2_FEAT_SIZE_IN_BYTES] = { 0 };

    /* Variable to define the array offset */
    XBYTE idx = 0;

    /* Variable to set flag */
    XBYTE feat_found;

    /* Initialize feature configuration for any-motion */
    struct bmi2_feature_config any_mot_config = { 0, 0, 0 };

    /* Search for any-motion feature and extract its configurations details */
    feat_found = bmi2_extract_input_feat_config(&any_mot_config, BMI2_ANY_MOTION, dev);
    if(feat_found)
    {
        /* Get the configuration from the page where any-motion feature resides */
        rslt = bmi2_get_feat_config(any_mot_config.page, feat_config, dev);
        if(rslt == BMI2_OK)
        {
            /* Define the offset for enable/disable of any-motion axes */
            idx = any_mot_config.start_addr + BMI2_ANY_MOT_FEAT_EN_OFFSET;

            /* Set the feature enable bit */
            feat_config[idx] = BMI2_SET_BITS(feat_config[idx], BMI2_ANY_NO_MOT_EN, enable);

            /* Set the configuration back to the page */
            rslt = bmi2_set_regs(BMI2_FEATURES_REG_ADDR, feat_config, BMI2_FEAT_SIZE_IN_BYTES, dev);
        }
    }
    else
    {
        rslt = BMI2_E_INVALID_SENSOR;
    }

    return rslt;
}

/*!
 * @brief This internal API is used to enable/disable no-motion feature.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_no_motion(XBYTE enable, struct bmi2_dev *dev)
* @brief      Set no motion
* @ingroup    DATAIO
* 
* @param[in]  enable : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_no_motion(XBYTE enable, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Array to define the feature configuration */
    XBYTE feat_config[BMI2_FEAT_SIZE_IN_BYTES] = { 0 };

    /* Variable to define the array offset */
    XBYTE idx = 0;

    /* Variable to set flag */
    XBYTE feat_found;

    /* Initialize feature configuration for no-motion */
    struct bmi2_feature_config no_mot_config = { 0, 0, 0 };

    /* Search for no-motion feature and extract its configurations details */
    feat_found = bmi2_extract_input_feat_config(&no_mot_config, BMI2_NO_MOTION, dev);
    if(feat_found)
    {
        /* Get the configuration from the page where any/no-motion feature resides */
        rslt = bmi2_get_feat_config(no_mot_config.page, feat_config, dev);
        if(rslt == BMI2_OK)
        {
            /* Define the offset for enable/disable of no-motion axes */
            idx = no_mot_config.start_addr + BMI2_NO_MOT_FEAT_EN_OFFSET;

            /* Set the feature enable bit */
            feat_config[idx] = BMI2_SET_BITS(feat_config[idx], BMI2_ANY_NO_MOT_EN, enable);

            /* Set the configuration back to the page */
            rslt = bmi2_set_regs(BMI2_FEATURES_REG_ADDR, feat_config, BMI2_FEAT_SIZE_IN_BYTES, dev);
        }
    }
    else
    {
        rslt = BMI2_E_INVALID_SENSOR;
    }

    return rslt;
}

/*!
 * @brief This internal API is used to enable/disable step detector feature.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_step_detector(XBYTE enable, struct bmi2_dev *dev)
* @brief      Set step detector
* @ingroup    DATAIO
* 
* @param[in]  enable : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_step_detector(XBYTE enable, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Array to define the feature configuration */
    XBYTE feat_config[BMI2_FEAT_SIZE_IN_BYTES] = { 0 };

    /* Variable to define the array offset */
    XBYTE idx = 0;

    /* Variable to set flag */
    XBYTE feat_found;

    /* Initialize feature configuration for step detector */
    struct bmi2_feature_config step_det_config = { 0, 0, 0 };

    /* Search for step detector feature and extract its configuration details */
    feat_found = bmi2_extract_input_feat_config(&step_det_config, BMI2_STEP_DETECTOR, dev);
    if(feat_found)
    {
        /* Get the configuration from the page where step detector feature resides */
        rslt = bmi2_get_feat_config(step_det_config.page, feat_config, dev);
        if(rslt == BMI2_OK)
        {
            /* Define the offset for enable/disable of step detector */
            idx = step_det_config.start_addr + BMI2_STEP_COUNT_FEAT_EN_OFFSET;

            /* Set the feature enable bit */
            feat_config[idx] = BMI2_SET_BITS(feat_config[idx], BMI2_STEP_DET_FEAT_EN, enable);

            /* Set the configuration back to the page */
            rslt = bmi2_set_regs(BMI2_FEATURES_REG_ADDR, feat_config, BMI2_FEAT_SIZE_IN_BYTES, dev);
        }
    }
    else
    {
        rslt = BMI2_E_INVALID_SENSOR;
    }

    return rslt;
}

/*!
 * @brief This internal API is used to enable/disable step counter feature.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_step_counter(XBYTE enable, struct bmi2_dev *dev)
* @brief      Set step counter
* @ingroup    DATAIO
* 
* @param[in]  enable : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_step_counter(XBYTE enable, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Array to define the feature configuration */
    XBYTE feat_config[BMI2_FEAT_SIZE_IN_BYTES] = { 0 };

    /* Variable to define the array offset */
    XBYTE idx = 0;

    /* Variable to set flag */
    XBYTE feat_found;

    /* Initialize feature configuration for step counter */
    struct bmi2_feature_config step_count_config = { 0, 0, 0 };

    /* Search for step counter feature and extract its configuration details */
    feat_found = bmi2_extract_input_feat_config(&step_count_config, BMI2_STEP_COUNTER, dev);
    if(feat_found)
    {
        /* Get the configuration from the page where step-counter feature resides */
        rslt = bmi2_get_feat_config(step_count_config.page, feat_config, dev);
        if(rslt == BMI2_OK)
        {
            /* Define the offset for enable/disable of step counter */
            idx = step_count_config.start_addr + BMI2_STEP_COUNT_FEAT_EN_OFFSET;

            /* Set the feature enable bit */
            feat_config[idx] = BMI2_SET_BITS(feat_config[idx], BMI2_STEP_COUNT_FEAT_EN, enable);

            /* Set the configuration back to the page */
            rslt = bmi2_set_regs(BMI2_FEATURES_REG_ADDR, feat_config, BMI2_FEAT_SIZE_IN_BYTES, dev);
        }
    }
    else
    {
        rslt = BMI2_E_INVALID_SENSOR;
    }

    return rslt;
}

/*!
 * @brief This internal API is used to enable/disable sig-motion feature.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_sig_motion(XBYTE enable, struct bmi2_dev *dev)
* @brief      Set sig motion
* @ingroup    DATAIO
* 
* @param[in]  enable : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_sig_motion(XBYTE enable, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Array to define the feature configuration */
    XBYTE feat_config[BMI2_FEAT_SIZE_IN_BYTES] = { 0 };

    /* Variable to define the array offset */
    XBYTE idx = 0;

    /* Variable to set flag */
    XBYTE feat_found;

    /* Initialize feature configuration for sig-motion */
    struct bmi2_feature_config sig_mot_config = { 0, 0, 0 };

    /* Search for sig-motion feature and extract its configuration details */
    feat_found = bmi2_extract_input_feat_config(&sig_mot_config, BMI2_SIG_MOTION, dev);
    if(feat_found)
    {
        /* Get the configuration from the page where sig-motion feature resides */
        rslt = bmi2_get_feat_config(sig_mot_config.page, feat_config, dev);
        if(rslt == BMI2_OK)
        {
            /* Define the offset for enable/disable of sig-motion */
            idx = sig_mot_config.start_addr + BMI2_SIG_MOT_FEAT_EN_OFFSET;

            /* Set the feature enable bit */
            feat_config[idx] = BMI2_SET_BIT_POS0(feat_config[idx], BMI2_SIG_MOT_FEAT_EN, enable);

            /* Set the configuration back to the page */
            rslt = bmi2_set_regs(BMI2_FEATURES_REG_ADDR, feat_config, BMI2_FEAT_SIZE_IN_BYTES, dev);
        }
    }
    else
    {
        rslt = BMI2_E_INVALID_SENSOR;
    }

    return rslt;
}

/*!
 * @brief This internal API is used to enable/disable step activity detection.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_step_activity(XBYTE enable, struct bmi2_dev *dev)
* @brief      Set step activity
* @ingroup    DATAIO
* 
* @param[in]  enable : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_step_activity(XBYTE enable, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Array to define the feature configuration */
    XBYTE feat_config[BMI2_FEAT_SIZE_IN_BYTES] = { 0 };

    /* Variable to define the array offset */
    XBYTE idx = 0;

    /* Variable to set flag */
    XBYTE feat_found;

    /* Initialize feature configuration for step activity */
    struct bmi2_feature_config step_act_config = { 0, 0, 0 };

    /* Search for step activity feature and extract its configuration details */
    feat_found = bmi2_extract_input_feat_config(&step_act_config, BMI2_STEP_ACTIVITY, dev);
    if(feat_found)
    {
        /* Get the configuration from the page where step-activity
         * feature resides
         */
        rslt = bmi2_get_feat_config(step_act_config.page, feat_config, dev);
        if(rslt == BMI2_OK)
        {
            /* Define the offset for enable/disable of step activity */
            idx = step_act_config.start_addr + BMI2_STEP_COUNT_FEAT_EN_OFFSET;

            /* Set the feature enable bit */
            feat_config[idx] = BMI2_SET_BITS(feat_config[idx], BMI2_STEP_ACT_FEAT_EN, enable);

            /* Set the configuration back to the page */
            rslt = bmi2_set_regs(BMI2_FEATURES_REG_ADDR, feat_config, BMI2_FEAT_SIZE_IN_BYTES, dev);
        }
    }
    else
    {
        rslt = BMI2_E_INVALID_SENSOR;
    }

    return rslt;
}

/*!
 * @brief This internal API gives an option to enable self-offset correction
 * feature of gyroscope, either internally or by the host.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_gyro_self_offset_corr(XBYTE enable, struct bmi2_dev *dev)
* @brief      Set gyro self offset corr
* @ingroup    DATAIO
* 
* @param[in]  enable : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_gyro_self_offset_corr(XBYTE enable, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Array to define the feature configuration */
    XBYTE feat_config[BMI2_FEAT_SIZE_IN_BYTES] = { 0 };

    /* Variable to define the array offset */
    XBYTE idx = 0;

    /* Variable to set flag */
    XBYTE feat_found;

    /* Initialize feature configuration for self-offset correction */
    struct bmi2_feature_config self_off_corr_cfg = { 0, 0, 0 };

    /* Search for self-offset correction and extract its configuration details */
    feat_found = bmi2_extract_input_feat_config(&self_off_corr_cfg, BMI2_GYRO_SELF_OFF, dev);
    if(feat_found)
    {
        /* Get the configuration from the page where self-offset
         * correction feature resides
         */
        rslt = bmi2_get_feat_config(self_off_corr_cfg.page, feat_config, dev);
        if(rslt == BMI2_OK)
        {
            /* Define the offset for enable/disable of self-offset correction */
            idx = self_off_corr_cfg.start_addr;

            /* Set the feature enable bit */
            feat_config[idx] = BMI2_SET_BITS(feat_config[idx], BMI2_GYR_SELF_OFF_CORR_FEAT_EN, enable);

            /* Set the configuration back to the page */
            rslt = bmi2_set_regs(BMI2_FEATURES_REG_ADDR, feat_config, BMI2_FEAT_SIZE_IN_BYTES, dev);
        }
    }
    else
    {
        rslt = BMI2_E_INVALID_SENSOR;
    }

    return rslt;
}

/*!
 * @brief This internal API enables the wrist gesture feature.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_wrist_gesture(XBYTE enable, struct bmi2_dev *dev)
* @brief      Set wrist gesture
* @ingroup    DATAIO
* 
* @param[in]  enable : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_wrist_gesture(XBYTE enable, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Array to define the feature configuration */
    XBYTE feat_config[BMI2_FEAT_SIZE_IN_BYTES] = { 0 };

    /* Variable to define the array offset */
    XBYTE idx = 0;

    /* Variable to set flag */
    XBYTE feat_found;

    /* Initialize feature configuration for wrist gesture */
    struct bmi2_feature_config wrist_gest_cfg = { 0, 0, 0 };

    /* Search for wrist gesture and extract its configuration details */
    feat_found = bmi2_extract_input_feat_config(&wrist_gest_cfg, BMI2_WRIST_GESTURE, dev);
    if(feat_found)
    {
        /* Get the configuration from the page where wrist gesture feature resides */
        rslt = bmi2_get_feat_config(wrist_gest_cfg.page, feat_config, dev);
        if(rslt == BMI2_OK)
        {
            /* Define the offset for enable/disable of wrist gesture */
            idx = wrist_gest_cfg.start_addr;

            /* Set the feature enable bit */
            feat_config[idx] = BMI2_SET_BITS(feat_config[idx], BMI2_WRIST_GEST_FEAT_EN, enable);

            /* Set the configuration back to the page */
            rslt = bmi2_set_regs(BMI2_FEATURES_REG_ADDR, feat_config, BMI2_FEAT_SIZE_IN_BYTES, dev);
        }
    }
    else
    {
        rslt = BMI2_E_INVALID_SENSOR;
    }

    return rslt;
}

/*!
 * @brief This internal API enables the wrist wear wake up feature.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_wrist_wear_wake_up(XBYTE enable, struct bmi2_dev *dev)
* @brief      Set wrist wear wake up
* @ingroup    DATAIO
* 
* @param[in]  enable : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_wrist_wear_wake_up(XBYTE enable, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Array to define the feature configuration */
    XBYTE feat_config[BMI2_FEAT_SIZE_IN_BYTES] = { 0 };

    /* Variable to define the array offset */
    XBYTE idx = 0;

    /* Variable to set flag */
    XBYTE feat_found;

    /* Initialize feature configuration for wrist wear wake up */
    struct bmi2_feature_config wrist_wake_up_cfg = { 0, 0, 0 };

    /* Search for wrist wear wake up and extract its configuration details */
    feat_found = bmi2_extract_input_feat_config(&wrist_wake_up_cfg, BMI2_WRIST_WEAR_WAKE_UP, dev);
    if(feat_found)
    {
        /* Get the configuration from the page where wrist wear wake up
         * feature resides
         */
        rslt = bmi2_get_feat_config(wrist_wake_up_cfg.page, feat_config, dev);
        if(rslt == BMI2_OK)
        {
            /* Define the offset for enable/disable of wrist wear wake up */
            idx = wrist_wake_up_cfg.start_addr;

            /* Set the feature enable bit */
            feat_config[idx] = BMI2_SET_BITS(feat_config[idx], BMI2_WRIST_WEAR_WAKE_UP_FEAT_EN, enable);

            /* Set the configuration back to the page */
            rslt = bmi2_set_regs(BMI2_FEATURES_REG_ADDR, feat_config, BMI2_FEAT_SIZE_IN_BYTES, dev);
        }
    }
    else
    {
        rslt = BMI2_E_INVALID_SENSOR;
    }

    return rslt;
}

/*!
 * @brief This internal API is used to enable/disable gyroscope user gain
 * feature.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_gyro_user_gain(XBYTE enable, struct bmi2_dev *dev)
* @brief      Set gyro user gain
* @ingroup    DATAIO
* 
* @param[in]  enable : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_gyro_user_gain(XBYTE enable, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Array to define the feature configuration */
    XBYTE feat_config[BMI2_FEAT_SIZE_IN_BYTES] = { 0 };

    /* Variable to define the array offset */
    XBYTE idx = 0;

    /* Variable to set flag */
    XBYTE feat_found;

    /* Initialize feature configuration for gyroscope user gain */
    struct bmi2_feature_config gyr_user_gain_cfg = { 0, 0, 0 };

    /* Search for user gain feature and extract its configuration details */
    feat_found = bmi2_extract_input_feat_config(&gyr_user_gain_cfg, BMI2_GYRO_GAIN_UPDATE, dev);
    if(feat_found)
    {
        /* Get the configuration from the page where user gain feature resides */
        rslt = bmi2_get_feat_config(gyr_user_gain_cfg.page, feat_config, dev);
        if(rslt == BMI2_OK)
        {
            /* Define the offset for enable/disable of user gain */
            idx = gyr_user_gain_cfg.start_addr + BMI2_GYR_USER_GAIN_FEAT_EN_OFFSET;

            /* Set the feature enable bit */
            feat_config[idx] = BMI2_SET_BITS(feat_config[idx], BMI2_GYR_USER_GAIN_FEAT_EN, enable);

            /* Set the configuration back to the page */
            rslt = bmi2_set_regs(BMI2_FEATURES_REG_ADDR, feat_config, BMI2_FEAT_SIZE_IN_BYTES, dev);
        }
    }
    else
    {
        rslt = BMI2_E_INVALID_SENSOR;
    }

    return rslt;
}

/*!
 * @brief This internal API sets any-motion configurations like axes select,
 * duration, threshold and output-configuration.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_any_motion_config(const struct bmi2_any_motion_config *config, struct bmi2_dev *dev)
* @brief      Set any motion config
* @ingroup    DATAIO
* 
* @param[in]  config : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_any_motion_config(const struct bmi2_any_motion_config *config, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Array to define the feature configuration */
    XBYTE feat_config[BMI2_FEAT_SIZE_IN_BYTES] = { 0 };

    /* Variable to define the array offset */
    XBYTE idx = 0;

    /* Variable to define count */
    XBYTE index = 0;

    /* Variable to set flag */
    XBYTE feat_found;

    /* Initialize feature configuration for any motion */
    struct bmi2_feature_config any_mot_config = { 0, 0, 0 };

    /* Copy the feature configuration address to a local pointer */
    XWORD *data_p = (XWORD *) (void *)feat_config;

    /* Search for any-motion feature and extract its configuration details */
    feat_found = bmi2_extract_input_feat_config(&any_mot_config, BMI2_ANY_MOTION, dev);
    if(feat_found)
    {
        /* Get the configuration from the page where any-motion feature resides */
        rslt = bmi2_get_feat_config(any_mot_config.page, feat_config, dev);
        if(rslt == BMI2_OK)
        {
            /* Define the offset in bytes for any-motion select */
            idx = any_mot_config.start_addr;

            /* Get offset in words since all the features are set in words length */
            idx = idx / 2;

            /* Set duration */
            *(data_p + idx) = BMI2_SET_BIT_POS0(*(data_p + idx), BMI2_ANY_NO_MOT_DUR, config->duration);

            /* Set x-select */
            *(data_p + idx) = BMI2_SET_BITS(*(data_p + idx), BMI2_ANY_NO_MOT_X_SEL, config->select_x);

            /* Set y-select */
            *(data_p + idx) = BMI2_SET_BITS(*(data_p + idx), BMI2_ANY_NO_MOT_Y_SEL, config->select_y);

            /* Set z-select */
            *(data_p + idx) = BMI2_SET_BITS(*(data_p + idx), BMI2_ANY_NO_MOT_Z_SEL, config->select_z);

            /* Increment offset by 1 word to set threshold and output configuration */
            idx++;

            /* Set threshold */
            *(data_p + idx) = BMI2_SET_BIT_POS0(*(data_p + idx), BMI2_ANY_NO_MOT_THRES, config->threshold);

            /* Increment offset by 1 more word to get the total length in words */
            idx++;

            /* Get total length in bytes to copy from local pointer to the array */
            idx = (XBYTE)(idx * 2) - any_mot_config.start_addr;

            /* Copy the bytes to be set back to the array */
            for (index = 0; index < idx; index++)
            {
                feat_config[any_mot_config.start_addr +
                            index] = *((XBYTE *) data_p + any_mot_config.start_addr + index);
            }

            /* Set the configuration back to the page */
            rslt = bmi2_set_regs(BMI2_FEATURES_REG_ADDR, feat_config, BMI2_FEAT_SIZE_IN_BYTES, dev);
        }
    }
    else
    {
        rslt = BMI2_E_INVALID_SENSOR;
    }

    return rslt;
}

/*!
 * @brief This internal API sets no-motion configurations like axes select,
 * duration, threshold and output-configuration.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_no_motion_config(const struct bmi2_no_motion_config *config, struct bmi2_dev *dev)
* @brief      Set no motion config
* @ingroup    DATAIO
* 
* @param[in]  config : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_no_motion_config(const struct bmi2_no_motion_config *config, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Array to define the feature configuration */
    XBYTE feat_config[BMI2_FEAT_SIZE_IN_BYTES] = { 0 };

    /* Variable to define the array offset */
    XBYTE idx = 0;

    /* Variable to define count */
    XBYTE index = 0;

    /* Variable to set flag */
    XBYTE feat_found;

    /* Initialize feature configuration for no-motion */
    struct bmi2_feature_config no_mot_config = { 0, 0, 0 };

    /* Copy the feature configuration address to a local pointer */
    XWORD *data_p = (XWORD *) (void *)feat_config;

    /* Search for no-motion feature and extract its configuration details */
    feat_found = bmi2_extract_input_feat_config(&no_mot_config, BMI2_NO_MOTION, dev);
    if(feat_found)
    {
        /* Get the configuration from the page where no-motion feature resides */
        rslt = bmi2_get_feat_config(no_mot_config.page, feat_config, dev);
        if(rslt == BMI2_OK)
        {
            /* Define the offset in bytes for no-motion select */
            idx = no_mot_config.start_addr;

            /* Get offset in words since all the features are set in words length */
            idx = idx / 2;

            /* Set duration */
            *(data_p + idx) = BMI2_SET_BIT_POS0(*(data_p + idx), BMI2_ANY_NO_MOT_DUR, config->duration);

            /* Set x-select */
            *(data_p + idx) = BMI2_SET_BITS(*(data_p + idx), BMI2_ANY_NO_MOT_X_SEL, config->select_x);

            /* Set y-select */
            *(data_p + idx) = BMI2_SET_BITS(*(data_p + idx), BMI2_ANY_NO_MOT_Y_SEL, config->select_y);

            /* Set z-select */
            *(data_p + idx) = BMI2_SET_BITS(*(data_p + idx), BMI2_ANY_NO_MOT_Z_SEL, config->select_z);

            /* Increment offset by 1 word to set threshold and output configuration */
            idx++;

            /* Set threshold */
            *(data_p + idx) = BMI2_SET_BIT_POS0(*(data_p + idx), BMI2_ANY_NO_MOT_THRES, config->threshold);

            /* Increment offset by 1 more word to get the total length in words */
            idx++;

            /* Get total length in bytes to copy from local pointer to the array */
            idx = (XBYTE)(idx * 2) - no_mot_config.start_addr;

            /* Copy the bytes to be set back to the array */
            for (index = 0; index < idx; index++)
            {
                feat_config[no_mot_config.start_addr +
                            index] = *((XBYTE *) data_p + no_mot_config.start_addr + index);
            }

            /* Set the configuration back to the page */
            rslt = bmi2_set_regs(BMI2_FEATURES_REG_ADDR, feat_config, BMI2_FEAT_SIZE_IN_BYTES, dev);
        }
    }
    else
    {
        rslt = BMI2_E_INVALID_SENSOR;
    }

    return rslt;
}

/*!
 * @brief This internal API sets sig-motion configurations like block-size,
 * output-configuration and other parameters.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_sig_motion_config(const struct bmi2_sig_motion_config *config, struct bmi2_dev *dev)
* @brief      Set sig motion config
* @ingroup    DATAIO
* 
* @param[in]  config : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_sig_motion_config(const struct bmi2_sig_motion_config *config, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Array to define the feature configuration */
    XBYTE feat_config[BMI2_FEAT_SIZE_IN_BYTES] = { 0 };

    /* Variable to define the array offset */
    XBYTE idx = 0;

    /* Variable to define index */
    XBYTE index = 0;

    /* Variable to set flag */
    XBYTE feat_found;

    /* Initialize feature configuration for sig-motion */
    struct bmi2_feature_config sig_mot_config = { 0, 0, 0 };

    /* Copy the feature configuration address to a local pointer */
    XWORD *data_p = (XWORD *) (void *)feat_config;

    /* Search for sig-motion feature and extract its configuration details */
    feat_found = bmi2_extract_input_feat_config(&sig_mot_config, BMI2_SIG_MOTION, dev);
    if(feat_found)
    {
        /* Get the configuration from the page where sig-motion feature resides */
        rslt = bmi2_get_feat_config(sig_mot_config.page, feat_config, dev);
        if(rslt == BMI2_OK)
        {
            /* Define the offset in bytes for sig-motion select */
            idx = sig_mot_config.start_addr;

            /* Get offset in words since all the features are set in words length */
            idx = idx / 2;

            /* Set parameter 1 */
            *(data_p + idx) = BMI2_SET_BIT_POS0(*(data_p + idx), BMI2_SIG_MOT_PARAM_1, config->block_size);

            /* Increment offset by 1 word to set parameter 2 */
            idx++;

            /* Set parameter 2 */
            *(data_p + idx) = BMI2_SET_BIT_POS0(*(data_p + idx), BMI2_SIG_MOT_PARAM_2, config->param_2);

            /* Increment offset by 1 word to set parameter 3 */
            idx++;

            /* Set parameter 3 */
            *(data_p + idx) = BMI2_SET_BIT_POS0(*(data_p + idx), BMI2_SIG_MOT_PARAM_3, config->param_3);

            /* Increment offset by 1 word to set parameter 4 */
            idx++;

            /* Set parameter 4 */
            *(data_p + idx) = BMI2_SET_BIT_POS0(*(data_p + idx), BMI2_SIG_MOT_PARAM_4, config->param_4);

            /* Increment offset by 1 word to set parameter 5 */
            idx++;

            /* Set parameter 5 */
            *(data_p + idx) = BMI2_SET_BIT_POS0(*(data_p + idx), BMI2_SIG_MOT_PARAM_5, config->param_5);

            /* Increment offset by 1 word  to set output- configuration */
            idx++;

            /* Get total length in bytes to copy from local pointer to the array */
            idx = (XBYTE)(idx * 2) - sig_mot_config.start_addr;

            /* Copy the bytes to be set back to the array */
            for (index = 0; index < idx; index++)
            {
                feat_config[sig_mot_config.start_addr +
                            index] = *((XBYTE *) data_p + sig_mot_config.start_addr + index);
            }

            /* Set the configuration back to the page */
            rslt = bmi2_set_regs(BMI2_FEATURES_REG_ADDR, feat_config, BMI2_FEAT_SIZE_IN_BYTES, dev);
        }
    }
    else
    {
        rslt = BMI2_E_INVALID_SENSOR;
    }

    return rslt;
}

/*!
 * @brief This internal API sets step counter parameter configurations.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_step_count_params_config(const XWORD *step_count_params, struct bmi2_dev *dev)
* @brief      Set step count params config
* @ingroup    DATAIO
* 
* @param[in]  step_count_params : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_step_count_params_config(const XWORD *step_count_params, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt = BMI2_OK;

    /* Array to define the feature configuration */
    XBYTE feat_config[BMI2_FEAT_SIZE_IN_BYTES] = { 0 };

    /* Variable to define index */
    XBYTE index = 0;

    /* Variable to set flag */
    XBYTE feat_found;

    /* Initialize feature configuration for step counter parameters */
    struct bmi2_feature_config step_params_config = { 0, 0, 0 };

    /* Variable to index the page number */
    XBYTE page_idx;

    /* Variable to define the start page */
    XBYTE start_page;

    /* Variable to define start address of the parameters */
    XBYTE start_addr;

    /* Variable to define number of bytes */
    XBYTE n_bytes = (BMI2_STEP_CNT_N_PARAMS * 2);

    /* Variable to store number of pages */
    XBYTE n_pages = (n_bytes / 16);

    /* Variable to define the end page */
    XBYTE end_page;

    /* Variable to define the remaining bytes to be read */
    XBYTE remain_len;

    /* Variable to define the maximum words(16 bytes or 8 words) to be read in a page */
    XBYTE max_len = 8;

    /* Variable index bytes in a page */
    XBYTE page_byte_idx;

    /* Variable to index the parameters */
    XBYTE param_idx = 0;

    /* Copy the feature configuration address to a local pointer */
    XWORD *data_p = (XWORD *) (void *)feat_config;

    /* Search for step counter parameter feature and extract its configuration details */
    feat_found = bmi2_extract_input_feat_config(&step_params_config, BMI2_STEP_COUNTER_PARAMS, dev);
    if(feat_found)
    {
        /* Get the start page for the step counter parameters */
        start_page = step_params_config.page;

        /* Get the end page for the step counter parameters */
        end_page = start_page + n_pages;

        /* Get the start address for the step counter parameters */
        start_addr = step_params_config.start_addr;

        /* Get the remaining length of bytes to be read */
        remain_len = (XBYTE)((n_bytes - (n_pages * 16)) + start_addr);
        for (page_idx = start_page; page_idx <= end_page; page_idx++)
        {
            /* Get the configuration from the respective page */
            rslt = bmi2_get_feat_config(page_idx, feat_config, dev);
            if(rslt == BMI2_OK)
            {
                /* Start from address 0x00 when switched to next page */
                if(page_idx > start_page)
                {
                    start_addr = 0;
                }

                /* Remaining number of words to be read in the page  */
                if(page_idx == end_page)
                {
                    max_len = (remain_len / 2);
                }

                /* Get offset in words since all the features are set in words length */
                page_byte_idx = start_addr / 2;
                for (; page_byte_idx < max_len;)
                {
                    /* Set parameters 1 to 25 */
                    *(data_p + page_byte_idx) = BMI2_SET_BIT_POS0(*(data_p + page_byte_idx), BMI2_STEP_COUNT_PARAMS, step_count_params[param_idx]);

                    /* Increment offset by 1 word to set to the next parameter */
                    page_byte_idx++;

                    /* Increment to next parameter */
                    param_idx++;
                }

                /* Get total length in bytes to copy from local pointer to the array */
                page_byte_idx = (XBYTE)(page_byte_idx * 2) - step_params_config.start_addr;

                /* Copy the bytes to be set back to the array */
                for (index = 0; index < page_byte_idx; index++)
                {
                    feat_config[step_params_config.start_addr +
                                index] = *((XBYTE *) data_p + step_params_config.start_addr + index);
                }

                /* Set the configuration back to the page */
                rslt = bmi2_set_regs(BMI2_FEATURES_REG_ADDR, feat_config, BMI2_FEAT_SIZE_IN_BYTES, dev);
            }
        }
    }
    else
    {
        rslt = BMI2_E_INVALID_SENSOR;
    }

    return rslt;
}

/* @brief This internal API sets step counter configurations like water-mark
 * level, reset-counter and output-configuration step detector and activity.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_step_config(const struct bmi2_step_config *config, struct bmi2_dev *dev)
* @brief      Set step config
* @ingroup    DATAIO
* 
* @param[in]  config : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_step_config(const struct bmi2_step_config *config, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Array to define the feature configuration */
    XBYTE feat_config[BMI2_FEAT_SIZE_IN_BYTES] = { 0 };

    /* Variable to define the array offset */
    XBYTE idx = 0;

    /* Variable to define index */
    XBYTE index = 0;

    /* Variable to set flag */
    XBYTE feat_found;

    /* Initialize feature configuration for step counter 4 */
    struct bmi2_feature_config step_count_config = { 0, 0, 0 };

    /* Copy the feature configuration address to a local pointer */
    XWORD *data_p = (XWORD *) (void *)feat_config;

    /* Search for step counter feature and extract its configuration details */
    feat_found = bmi2_extract_input_feat_config(&step_count_config, BMI2_STEP_COUNTER, dev);
    if(feat_found)
    {
        /* Get the configuration from the page where step counter resides */
        rslt = bmi2_get_feat_config(step_count_config.page, feat_config, dev);
        if(rslt == BMI2_OK)
        {
            /* Define the offset in bytes */
            idx = step_count_config.start_addr;

            /* Get offset in words since all the features are set in words length */
            idx = idx / 2;

            /* Set water-mark level */
            *(data_p + idx) = BMI2_SET_BIT_POS0(*(data_p + idx), BMI2_STEP_COUNT_WM_LEVEL, config->watermark_level);

            /* Set reset-counter */
            *(data_p + idx) = BMI2_SET_BITS(*(data_p + idx), BMI2_STEP_COUNT_RST_CNT, config->reset_counter);

            /* Increment offset by 1 word  to set output
             * configuration of step detector and step activity
             */
            idx++;

            /* Set step buffer size */
            *(data_p + idx) = BMI2_SET_BITS(*(data_p + idx), BMI2_STEP_BUFFER_SIZE, config->step_buffer_size);

            /* Increment offset by 1 more word to get the total length in words */
            idx++;

            /* Get total length in bytes to copy from local pointer to the array */
            idx = (XBYTE)(idx * 2) - step_count_config.start_addr;

            /* Copy the bytes to be set back to the array */
            for (index = 0; index < idx; index++)
            {
                feat_config[step_count_config.start_addr +
                            index] = *((XBYTE *) data_p + step_count_config.start_addr + index);
            }

            /* Set the configuration back to the page */
            rslt = bmi2_set_regs(BMI2_FEATURES_REG_ADDR, feat_config, BMI2_FEAT_SIZE_IN_BYTES, dev);
        }
    }
    else
    {
        rslt = BMI2_E_INVALID_SENSOR;
    }

    return rslt;
}

/*!
 * @brief This internal API sets wrist gesture configurations like wearable-arm,
 * and output-configuration.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_wrist_gest_config(const struct bmi2_wrist_gest_config *config, struct bmi2_dev *dev)
* @brief      Set wrist gest config
* @ingroup    DATAIO
* 
* @param[in]  config : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_wrist_gest_config(const struct bmi2_wrist_gest_config *config, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Array to define the feature configuration */
    XBYTE feat_config[BMI2_FEAT_SIZE_IN_BYTES] = { 0 };

    /* Variable to define the array offset */
    XBYTE idx = 0;

    /* Variable to define index */
    XBYTE index = 0;

    /* Variable to set flag */
    XBYTE feat_found;

    /* Initialize feature configuration for wrist gesture */
    struct bmi2_feature_config wrist_gest_config = { 0, 0, 0 };

    /* Copy the feature configuration address to a local pointer */
    XWORD *data_p = (XWORD *) (void *)feat_config;

    /* Search for wrist gesture feature and extract its configuration details */
    feat_found = bmi2_extract_input_feat_config(&wrist_gest_config, BMI2_WRIST_GESTURE, dev);
    if(feat_found)
    {
        /* Get the configuration from the page where wrist gesture feature resides */
        rslt = bmi2_get_feat_config(wrist_gest_config.page, feat_config, dev);
        if(rslt == BMI2_OK)
        {
            /* Define the offset in bytes for gesture select */
            idx = wrist_gest_config.start_addr;

            /* Get offset in words since all the features are set in words length */
            idx = idx / 2;

            /* Set wearable arm */
            *(data_p + idx) = BMI2_SET_BITS(*(data_p + idx), BMI2_WRIST_GEST_WEAR_ARM, config->wearable_arm);

            /* Increment offset by 1 more word to set minimum tilt angle (min_flick_peak) */
            idx++;
            *(data_p + idx) = config->min_flick_peak;

            /* Increment offset by 1 more word to set min_flick_samples */
            idx++;
            *(data_p + idx) = config->min_flick_samples;

            /* Increment offset by 1 more word to set max time within gesture moment has to be completed */
            idx++;
            *(data_p + idx) = config->max_duration;

            /* Increment offset by 1 more word to get the total length in words */
            idx++;

            /* Get total length in bytes to copy from local pointer to the array */
            idx = (XBYTE)(idx * 2) - wrist_gest_config.start_addr;

            /* Copy the bytes to be set back to the array */
            for (index = 0; index < idx; index++)
            {
                feat_config[wrist_gest_config.start_addr +
                            index] = *((XBYTE *) data_p + wrist_gest_config.start_addr + index);
            }

            /* Set the configuration back to the page */
            rslt = bmi2_set_regs(BMI2_FEATURES_REG_ADDR, feat_config, BMI2_FEAT_SIZE_IN_BYTES, dev);
        }
    }
    else
    {
        rslt = BMI2_E_INVALID_SENSOR;
    }

    return rslt;
}

/*!
 * @brief This internal API sets wrist wear wake-up configurations like
 * output-configuration.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_wrist_wear_wake_up_config(const struct bmi2_wrist_wear_wake_up_config *config, struct bmi2_dev *dev)
* @brief      Set wrist wear wake up config
* @ingroup    DATAIO
* 
* @param[in]  config : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::set_wrist_wear_wake_up_config(const struct bmi2_wrist_wear_wake_up_config *config, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Array to define the feature configuration */
    XBYTE feat_config[BMI2_FEAT_SIZE_IN_BYTES] = { 0 };

    /* Variable to define the array offset */
    XBYTE idx = 0;

    /* Variable to define index */
    XBYTE index = 0;

    /* Variable to set flag */
    XBYTE feat_found;

    /* Initialize feature configuration for wrist wear wake-up */
    struct bmi2_feature_config wrist_wake_up_config = { 0, 0, 0 };

    /* Copy the feature configuration address to a local pointer */
    XWORD *data_p = (XWORD *) (void *)feat_config;

    /* Search for wrist wear wake-up feature and extract its configuration details */
    feat_found = bmi2_extract_input_feat_config(&wrist_wake_up_config, BMI2_WRIST_WEAR_WAKE_UP, dev);
    if(feat_found)
    {
        /* Get the configuration from the page where wrist wear wake-up feature resides */
        rslt = bmi2_get_feat_config(wrist_wake_up_config.page, feat_config, dev);
        if(rslt == BMI2_OK)
        {
            /* Define the offset in bytes for wrist wear wake-up select */
            idx = wrist_wake_up_config.start_addr;

            /* Get offset in words since all the features are set in words length */
            idx = idx / 2;

            *(data_p + idx) = config->min_angle_focus;

            /* Increment offset by 1 more word to set min_angle_nonfocus */
            idx++;
            *(data_p + idx) = config->min_angle_nonfocus;

            /* Increment offset by 1 more word to set max_tilt_lr */
            idx++;
            *(data_p + idx) = config->max_tilt_lr;

            /* Increment offset by 1 more word to set max_tilt_ll */
            idx++;
            *(data_p + idx) = config->max_tilt_ll;

            /* Increment offset by 1 more word to set max_tilt_pd */
            idx++;
            *(data_p + idx) = config->max_tilt_pd;

            /* Increment offset by 1 more word to set max_tilt_pu */
            idx++;
            *(data_p + idx) = config->max_tilt_pu;

            /* Increment offset by 1 more word to get the total length in words */
            idx++;

            /* Get total length in bytes to copy from local pointer to the array */
            idx = (XBYTE)(idx * 2) - wrist_wake_up_config.start_addr;

            /* Copy the bytes to be set back to the array */
            for (index = 0; index < idx; index++)
            {
                feat_config[wrist_wake_up_config.start_addr +
                            index] = *((XBYTE *) data_p + wrist_wake_up_config.start_addr + index);
            }

            /* Set the configuration back to the page */
            rslt = bmi2_set_regs(BMI2_FEATURES_REG_ADDR, feat_config, BMI2_FEAT_SIZE_IN_BYTES, dev);
        }
    }
    else
    {
        rslt = BMI2_E_INVALID_SENSOR;
    }

    return rslt;
}

/*!
 * @brief This internal API gets any-motion configurations like axes select,
 * duration, threshold and output-configuration.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_any_motion_config(struct bmi2_any_motion_config *config, struct bmi2_dev *dev)
* @brief      Get any motion config
* @ingroup    DATAIO
* 
* @param[in]  config : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_any_motion_config(struct bmi2_any_motion_config *config, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Array to define the feature configuration */
    XBYTE feat_config[BMI2_FEAT_SIZE_IN_BYTES] = { 0 };

    /* Variable to define the array offset */
    XBYTE idx = 0;

    /* Variable to define LSB */
    XWORD lsb;

    /* Variable to define MSB */
    XWORD msb;

    /* Variable to define a word */
    XWORD lsb_msb;

    /* Variable to set flag */
    XBYTE feat_found;

    /* Initialize feature configuration for any-motion */
    struct bmi2_feature_config any_mot_config = { 0, 0, 0 };

    /* Search for any-motion feature and extract its configuration details */
    feat_found = bmi2_extract_input_feat_config(&any_mot_config, BMI2_ANY_MOTION, dev);
    if(feat_found)
    {
        /* Get the configuration from the page where any-motion feature resides */
        rslt = bmi2_get_feat_config(any_mot_config.page, feat_config, dev);
        if(rslt == BMI2_OK)
        {
            /* Define the offset for feature enable for any-motion */
            idx = any_mot_config.start_addr;

            /* Get word to calculate duration, x, y and z select */
            lsb = (XWORD) feat_config[idx++];
            msb = ((XWORD) feat_config[idx++] << 8);
            lsb_msb = lsb | msb;

            /* Get duration */
            config->duration = lsb_msb & BMI2_ANY_NO_MOT_DUR_MASK;

            /* Get x-select */
            config->select_x = (lsb_msb & BMI2_ANY_NO_MOT_X_SEL_MASK) >> BMI2_ANY_NO_MOT_X_SEL_POS;

            /* Get y-select */
            config->select_y = (lsb_msb & BMI2_ANY_NO_MOT_Y_SEL_MASK) >> BMI2_ANY_NO_MOT_Y_SEL_POS;

            /* Get z-select */
            config->select_z = (lsb_msb & BMI2_ANY_NO_MOT_Z_SEL_MASK) >> BMI2_ANY_NO_MOT_Z_SEL_POS;

            /* Get word to calculate threshold, output configuration from the same word */
            lsb = (XWORD) feat_config[idx++];
            msb = ((XWORD) feat_config[idx++] << 8);
            lsb_msb = lsb | msb;

            /* Get threshold */
            config->threshold = lsb_msb & BMI2_ANY_NO_MOT_THRES_MASK;
        }
    }
    else
    {
        rslt = BMI2_E_INVALID_SENSOR;
    }

    return rslt;
}

/*!
 * @brief This internal API gets no-motion configurations like axes select,
 * duration, threshold and output-configuration.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_no_motion_config(struct bmi2_no_motion_config *config, struct bmi2_dev *dev)
* @brief      Get no motion config
* @ingroup    DATAIO
* 
* @param[in]  config : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_no_motion_config(struct bmi2_no_motion_config *config, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Array to define the feature configuration */
    XBYTE feat_config[BMI2_FEAT_SIZE_IN_BYTES] = { 0 };

    /* Variable to define the array offset */
    XBYTE idx = 0;

    /* Variable to define LSB */
    XWORD lsb = 0;

    /* Variable to define MSB */
    XWORD msb = 0;

    /* Variable to define a word */
    XWORD lsb_msb = 0;

    /* Variable to set flag */
    XBYTE feat_found;

    /* Initialize feature configuration for no-motion */
    struct bmi2_feature_config no_mot_config = { 0, 0, 0 };

    /* Search for no-motion feature and extract its configuration details */
    feat_found = bmi2_extract_input_feat_config(&no_mot_config, BMI2_NO_MOTION, dev);
    if(feat_found)
    {
        /* Get the configuration from the page where no-motion feature resides */
        rslt = bmi2_get_feat_config(no_mot_config.page, feat_config, dev);
        if(rslt == BMI2_OK)
        {
            /* Define the offset for feature enable for no-motion */
            idx = no_mot_config.start_addr;

            /* Get word to calculate duration, x, y and z select */
            lsb = (XWORD) feat_config[idx++];
            msb = ((XWORD) feat_config[idx++] << 8);
            lsb_msb = lsb | msb;

            /* Get duration */
            config->duration = lsb_msb & BMI2_ANY_NO_MOT_DUR_MASK;

            /* Get x-select */
            config->select_x = (lsb_msb & BMI2_ANY_NO_MOT_X_SEL_MASK) >> BMI2_ANY_NO_MOT_X_SEL_POS;

            /* Get y-select */
            config->select_y = (lsb_msb & BMI2_ANY_NO_MOT_Y_SEL_MASK) >> BMI2_ANY_NO_MOT_Y_SEL_POS;

            /* Get z-select */
            config->select_z = (lsb_msb & BMI2_ANY_NO_MOT_Z_SEL_MASK) >> BMI2_ANY_NO_MOT_Z_SEL_POS;

            /* Get word to calculate threshold, output configuration from the same word */
            lsb = (XWORD) feat_config[idx++];
            msb = ((XWORD) feat_config[idx++] << 8);
            lsb_msb = lsb | msb;

            /* Get threshold */
            config->threshold = lsb_msb & BMI2_ANY_NO_MOT_THRES_MASK;
        }
    }
    else
    {
        rslt = BMI2_E_INVALID_SENSOR;
    }

    return rslt;
}

/*!
 * @brief This internal API gets sig-motion configurations like block-size,
 * output-configuration and other parameters.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_sig_motion_config(struct bmi2_sig_motion_config *config, struct bmi2_dev *dev)
* @brief      Get sig motion config
* @ingroup    DATAIO
* 
* @param[in]  config : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_sig_motion_config(struct bmi2_sig_motion_config *config, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Array to define the feature configuration */
    XBYTE feat_config[BMI2_FEAT_SIZE_IN_BYTES] = { 0 };

    /* Variable to define the array offset */
    XBYTE idx = 0;

    /* Variable to define LSB */
    XWORD lsb = 0;

    /* Variable to define MSB */
    XWORD msb = 0;

    /* Variable to define a word */
    XWORD lsb_msb = 0;

    /* Variable to set flag */
    XBYTE feat_found;

    /* Initialize feature configuration sig-motion */
    struct bmi2_feature_config sig_mot_config = { 0, 0, 0 };

    /* Search for sig-motion feature and extract its configuration details */
    feat_found = bmi2_extract_input_feat_config(&sig_mot_config, BMI2_SIG_MOTION, dev);
    if(feat_found)
    {
        /* Get the configuration from the page where sig-motion feature resides */
        rslt = bmi2_get_feat_config(sig_mot_config.page, feat_config, dev);
        if(rslt == BMI2_OK)
        {
            /* Define the offset for feature enable for sig-motion */
            idx = sig_mot_config.start_addr;

            /* Get word to calculate parameter 1 */
            lsb = (XWORD) feat_config[idx++];
            msb = ((XWORD) feat_config[idx++] << 8);
            lsb_msb = lsb | msb;

            /* Get parameter 1  */
            config->block_size = lsb_msb & BMI2_SIG_MOT_PARAM_1_MASK;

            /* Get word to calculate parameter 2 */
            lsb = (XWORD) feat_config[idx++];
            msb = ((XWORD) feat_config[idx++] << 8);
            lsb_msb = lsb | msb;

            /* Get parameter 2  */
            config->param_2 = lsb_msb & BMI2_SIG_MOT_PARAM_2_MASK;

            /* Get word to calculate parameter 3 */
            lsb = (XWORD) feat_config[idx++];
            msb = ((XWORD) feat_config[idx++] << 8);
            lsb_msb = lsb | msb;

            /* Get parameter 3  */
            config->param_3 = lsb_msb & BMI2_SIG_MOT_PARAM_3_MASK;

            /* Get word to calculate parameter 4 */
            lsb = (XWORD) feat_config[idx++];
            msb = ((XWORD) feat_config[idx++] << 8);
            lsb_msb = lsb | msb;

            /* Get parameter 4  */
            config->param_4 = lsb_msb & BMI2_SIG_MOT_PARAM_4_MASK;

            /* Get word to calculate parameter 5 */
            lsb = (XWORD) feat_config[idx++];
            msb = ((XWORD) feat_config[idx++] << 8);
            lsb_msb = lsb | msb;

            /* Get parameter 5  */
            config->param_5 = lsb_msb & BMI2_SIG_MOT_PARAM_5_MASK;
        }
    }
    else
    {
        rslt = BMI2_E_INVALID_SENSOR;
    }

    return rslt;
}

/*!
 * @brief This internal API gets step counter parameter configurations.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_step_count_params_config(XWORD *step_count_params, struct bmi2_dev *dev)
* @brief      Get step count params config
* @ingroup    DATAIO
* 
* @param[in]  step_count_params : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_step_count_params_config(XWORD *step_count_params, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt = BMI2_OK;

    /* Array to define the feature configuration */
    XBYTE feat_config[BMI2_FEAT_SIZE_IN_BYTES] = { 0 };

    /* Variable to set flag */
    XBYTE feat_found;

    /* Variable to define LSB */
    XWORD lsb = 0;

    /* Variable to define MSB */
    XWORD msb = 0;

    /* Variable to define a word */
    XWORD lsb_msb = 0;

    /* Initialize feature configuration for step counter 1 */
    struct bmi2_feature_config step_params_config = { 0, 0, 0 };

    /* Variable to index the page number */
    XBYTE page_idx;

    /* Variable to define the start page */
    XBYTE start_page;

    /* Variable to define start address of the parameters */
    XBYTE start_addr;

    /* Variable to define number of bytes */
    XBYTE n_bytes = (BMI2_STEP_CNT_N_PARAMS * 2);

    /* Variable to store number of pages */
    XBYTE n_pages = (n_bytes / 16);

    /* Variable to define the end page */
    XBYTE end_page;

    /* Variable to define the remaining bytes to be read */
    XBYTE remain_len;

    /* Variable to define the maximum words to be read in a page */
    XBYTE max_len = BMI2_FEAT_SIZE_IN_BYTES;

    /* Variable index bytes in a page */
    XBYTE page_byte_idx;

    /* Variable to index the parameters */
    XBYTE param_idx = 0;

    /* Search for step counter parameter feature and extract its configuration details */
    feat_found = bmi2_extract_input_feat_config(&step_params_config, BMI2_STEP_COUNTER_PARAMS, dev);
    if(feat_found)
    {
        /* Get the start page for the step counter parameters */
        start_page = step_params_config.page;

        /* Get the end page for the step counter parameters */
        end_page = start_page + n_pages;

        /* Get the start address for the step counter parameters */
        start_addr = step_params_config.start_addr;

        /* Get the remaining length of bytes to be read */
        remain_len = (XBYTE)((n_bytes - (n_pages * 16)) + start_addr);
        for (page_idx = start_page; page_idx <= end_page; page_idx++)
        {
            /* Get the configuration from the respective page */
            rslt = bmi2_get_feat_config(page_idx, feat_config, dev);
            if(rslt == BMI2_OK)
            {
                /* Start from address 0x00 when switched to next page */
                if(page_idx > start_page)
                {
                    start_addr = 0;
                }

                /* Remaining number of bytes to be read in the page  */
                if(page_idx == end_page)
                {
                    max_len = remain_len;
                }

                /* Get the offset */
                page_byte_idx = start_addr;
                while (page_byte_idx < max_len)
                {
                    /* Get word to calculate the parameter*/
                    lsb = (XWORD) feat_config[page_byte_idx++];
                    if(page_byte_idx < max_len)
                    {
                        msb = ((XWORD) feat_config[page_byte_idx++] << 8);
                    }

                    lsb_msb = lsb | msb;

                    /* Get parameters 1 to 25 */
                    step_count_params[param_idx] = lsb_msb & BMI2_STEP_COUNT_PARAMS_MASK;

                    /* Increment to next parameter */
                    param_idx++;
                }
            }
        }
    }
    else
    {
        rslt = BMI2_E_INVALID_SENSOR;
    }

    return rslt;
}

/*!
 * @brief This internal API gets step counter/detector/activity configurations.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_step_config(struct bmi2_step_config *config, struct bmi2_dev *dev)
* @brief      Get step config
* @ingroup    DATAIO
* 
* @param[in]  config : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_step_config(struct bmi2_step_config *config, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Array to define the feature configuration */
    XBYTE feat_config[BMI2_FEAT_SIZE_IN_BYTES] = { 0 };

    /* Variable to define the array offset */
    XBYTE idx = 0;

    /* Variable to define LSB */
    XWORD lsb = 0;

    /* Variable to define MSB */
    XWORD msb = 0;

    /* Variable to define a word */
    XWORD lsb_msb = 0;

    /* Variable to set flag */
    XBYTE feat_found;

    /* Initialize feature configuration for step counter */
    struct bmi2_feature_config step_count_config = { 0, 0, 0 };

    /* Search for step counter 4 feature and extract its configuration details */
    feat_found = bmi2_extract_input_feat_config(&step_count_config, BMI2_STEP_COUNTER, dev);
    if(feat_found)
    {
        /* Get the configuration from the page where step counter 4 parameter resides */
        rslt = bmi2_get_feat_config(step_count_config.page, feat_config, dev);
        if(rslt == BMI2_OK)
        {
            /* Define the offset for feature enable for step counter/detector/activity */
            idx = step_count_config.start_addr;

            /* Get word to calculate water-mark level and reset counter */
            lsb = (XWORD) feat_config[idx++];
            msb = ((XWORD) feat_config[idx++] << 8);
            lsb_msb = lsb | msb;

            /* Get water-mark level */
            config->watermark_level = lsb_msb & BMI2_STEP_COUNT_WM_LEVEL_MASK;

            /* Get reset counter */
            config->reset_counter = (lsb_msb & BMI2_STEP_COUNT_RST_CNT_MASK) >> BMI2_STEP_COUNT_RST_CNT_POS;

            /* Get word to calculate output configuration of step detector and activity */
            lsb = (XWORD) feat_config[idx++];
            msb = ((XWORD) feat_config[idx++] << 8);
            lsb_msb = lsb | msb;

            config->step_buffer_size = (lsb_msb & BMI2_STEP_BUFFER_SIZE_MASK) >> BMI2_STEP_BUFFER_SIZE_POS;
        }
    }
    else
    {
        rslt = BMI2_E_INVALID_SENSOR;
    }

    return rslt;
}

/*!
 * @brief This internal API gets wrist gesture configurations like wearable-arm,
 * and output-configuration.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_wrist_gest_config(struct bmi2_wrist_gest_config *config, struct bmi2_dev *dev)
* @brief      Get wrist gest config
* @ingroup    DATAIO
* 
* @param[in]  config : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_wrist_gest_config(struct bmi2_wrist_gest_config *config, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Array to define the feature configuration */
    XBYTE feat_config[BMI2_FEAT_SIZE_IN_BYTES] = { 0 };

    /* Variable to define the array offset */
    XBYTE idx = 0;

    /* Variable to set flag */
    XBYTE feat_found;

    /* Initialize feature configuration for wrist gesture */
    struct bmi2_feature_config wrist_gest_config = { 0, 0, 0 };

    /* Copy the feature configuration address to a local pointer */
    XWORD *data_p = (XWORD *) (void *)feat_config;

    /* Search for wrist gesture feature and extract its configuration details */
    feat_found = bmi2_extract_input_feat_config(&wrist_gest_config, BMI2_WRIST_GESTURE, dev);
    if(feat_found)
    {
        /* Get the configuration from the page where wrist gesture feature  resides */
        rslt = bmi2_get_feat_config(wrist_gest_config.page, feat_config, dev);
        if(rslt == BMI2_OK)
        {
            /* Define the offset in bytes for wrist gesture select */
            idx = wrist_gest_config.start_addr;

            /* Get offset in words since all the features are set in words length */
            idx = idx / 2;

            /* Get wearable arm */
            config->wearable_arm = (*(data_p + idx) & BMI2_WRIST_GEST_WEAR_ARM_MASK) >> BMI2_WRIST_GEST_WEAR_ARM_POS;

            /* Increment the offset by 1 word to get min_flick_peak */
            idx++;
            config->min_flick_peak = *(data_p + idx);

            /* Increment the offset by 1 word to get min_flick_samples */
            idx++;
            config->min_flick_samples = *(data_p + idx);

            /* Increment the offset by 1 word to get max_duration */
            idx++;
            config->max_duration = *(data_p + idx);
        }
    }
    else
    {
        rslt = BMI2_E_INVALID_SENSOR;
    }

    return rslt;
}

/*!
 * @brief This internal API gets wrist wear wake-up configurations like
 * output-configuration.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_wrist_wear_wake_up_config(struct bmi2_wrist_wear_wake_up_config *config, struct bmi2_dev *dev)
* @brief      Get wrist wear wake up config
* @ingroup    DATAIO
* 
* @param[in]  config : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_wrist_wear_wake_up_config(struct bmi2_wrist_wear_wake_up_config *config, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Array to define the feature configuration */
    XBYTE feat_config[BMI2_FEAT_SIZE_IN_BYTES] = { 0 };

    /* Variable to define the array offset */
    XBYTE idx = 0;

    /* Variable to set flag */
    XBYTE feat_found;

    /* Initialize feature configuration for wrist wear wake-up */
    struct bmi2_feature_config wrist_wake_up_config = { 0, 0, 0 };

    /* Copy the feature configuration address to a local pointer */
    XWORD *data_p = (XWORD *) (void *)feat_config;

    /* Search for wrist wear wake-up feature and extract its configuration details */
    feat_found = bmi2_extract_input_feat_config(&wrist_wake_up_config, BMI2_WRIST_WEAR_WAKE_UP, dev);
    if(feat_found)
    {
        /* Get the configuration from the page where wrist wear wake-up feature  resides */
        rslt = bmi2_get_feat_config(wrist_wake_up_config.page, feat_config, dev);
        if(rslt == BMI2_OK)
        {
            /* Define the offset in bytes for wrist wear wake-up select */
            idx = wrist_wake_up_config.start_addr;

            /* Get offset in words since all the features are set in words length */
            idx = idx / 2;

            config->min_angle_focus = *(data_p + idx);

            /* Increment the offset value by 1 word to get min_angle_nonfocus */
            idx++;
            config->min_angle_nonfocus = *(data_p + idx);

            /* Increment the offset value by 1 word to get max_tilt_lr */
            idx++;
            config->max_tilt_lr = *(data_p + idx);

            /* Increment the offset value by 1 word to get max_tilt_ll */
            idx++;
            config->max_tilt_ll = *(data_p + idx);

            /* Increment the offset value by 1 word to get max_tilt_pd */
            idx++;
            config->max_tilt_pd = *(data_p + idx);

            /* Increment the offset value by 1 word to get max_tilt_pu */
            idx++;
            config->max_tilt_pu = *(data_p + idx);

        }
    }
    else
    {
        rslt = BMI2_E_INVALID_SENSOR;
    }

    return rslt;
}

/*!
 * @brief This internal API gets the output values of the wrist gesture.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_wrist_gest_status(XBYTE *wrist_gest, struct bmi2_dev *dev)
* @brief      Get wrist gest status
* @ingroup    DATAIO
* 
* @param[in]  wrist_gest : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_wrist_gest_status(XBYTE *wrist_gest, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Array to define the feature configuration */
    XBYTE feat_config[BMI2_FEAT_SIZE_IN_BYTES] = { 0 };

    /* Variables to define index */
    XBYTE idx = 0;

    /* Variable to set flag */
    XBYTE feat_found;

    /* Initialize feature output for wrist gesture */
    struct bmi2_feature_config wrist_gest_out_config = { 0, 0, 0 };

    /* Search for wrist gesture feature and extract its configuration details */
    feat_found = extract_output_feat_config(&wrist_gest_out_config, BMI2_WRIST_GESTURE, dev);
    if(feat_found)
    {
        /* Get the feature output configuration for wrist gesture */
        rslt = bmi2_get_feat_config(wrist_gest_out_config.page, feat_config, dev);
        if(rslt == BMI2_OK)
        {
            /* Define the offset in bytes for wrist gesture output */
            idx = wrist_gest_out_config.start_addr;

            /* Get the wrist gesture output */
            *wrist_gest = feat_config[idx];
        }
    }
    else
    {
        rslt = BMI2_E_INVALID_SENSOR;
    }

    return rslt;
}

/*!
 * @brief This internal API gets the output values of step counter.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_step_counter_output(XDWORD *step_count, struct bmi2_dev *dev)
* @brief      Get step counter output
* @ingroup    DATAIO
* 
* @param[in]  step_count : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_step_counter_output(XDWORD *step_count, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Array to define the feature configuration */
    XBYTE feat_config[BMI2_FEAT_SIZE_IN_BYTES] = { 0 };

    /* Variables to define index */
    XBYTE idx = 0;

    /* Variable to set flag */
    XBYTE feat_found;

    /* Initialize feature output for step counter */
    struct bmi2_feature_config step_cnt_out_config = { 0, 0, 0 };

    /* Search for step counter output feature and extract its configuration details */
    feat_found = extract_output_feat_config(&step_cnt_out_config, BMI2_STEP_COUNTER, dev);
    if(feat_found)
    {
        /* Get the feature output configuration for step-counter */
        rslt = bmi2_get_feat_config(step_cnt_out_config.page, feat_config, dev);
        if(rslt == BMI2_OK)
        {
            /* Define the offset in bytes for step counter output */
            idx = step_cnt_out_config.start_addr;

            /* Get the step counter output in 4 bytes */
            *step_count = (XDWORD) feat_config[idx++];
            *step_count |= ((XDWORD) feat_config[idx++] << 8);
            *step_count |= ((XDWORD) feat_config[idx++] << 16);
            *step_count |= ((XDWORD) feat_config[idx++] << 24);
        }
    }
    else
    {
        rslt = BMI2_E_INVALID_SENSOR;
    }

    return rslt;
}

/*!
 * @brief This internal API gets the error status related to NVM.
 */
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_nvm_error_status(struct bmi2_nvm_err_status *nvm_err_stat, struct bmi2_dev *dev)
* @brief      Get nvm error status
* @ingroup    DATAIO
* 
* @param[in]  nvm_err_stat : 
* @param[in]  dev : 
* 
* @return     XBYTESIG : 
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_nvm_error_status(struct bmi2_nvm_err_status *nvm_err_stat, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Array to define the feature configuration */
    XBYTE feat_config[BMI2_FEAT_SIZE_IN_BYTES] = { 0 };

    /* Variables to define index */
    XBYTE idx = 0;

    /* Variable to set flag */
    XBYTE feat_found;

    /* Initialize feature output for NVM error status */
    struct bmi2_feature_config nvm_err_cfg = { 0, 0, 0 };

    /* Search for NVM error status feature and extract its configuration details */
    feat_found = extract_output_feat_config(&nvm_err_cfg, BMI2_NVM_STATUS, dev);
    if(feat_found)
    {
        /* Get the feature output configuration for NVM error status */
        rslt = bmi2_get_feat_config(nvm_err_cfg.page, feat_config, dev);
        if(rslt == BMI2_OK)
        {
            /* Define the offset in bytes for NVM error status */
            idx = nvm_err_cfg.start_addr;

            /* Increment index to get the error status */
            idx++;

            /* Error when NVM load action fails */
            nvm_err_stat->load_error = BMI2_GET_BIT_POS0(feat_config[idx], BMI2_NVM_LOAD_ERR_STATUS);

            /* Error when NVM program action fails */
            nvm_err_stat->prog_error = BMI2_GET_BITS(feat_config[idx], BMI2_NVM_PROG_ERR_STATUS);

            /* Error when NVM erase action fails */
            nvm_err_stat->erase_error = BMI2_GET_BITS(feat_config[idx], BMI2_NVM_ERASE_ERR_STATUS);

            /* Error when NVM program limit is exceeded */
            nvm_err_stat->exceed_error = BMI2_GET_BITS(feat_config[idx], BMI2_NVM_END_EXCEED_STATUS);

            /* Error when NVM privilege mode is not acquired */
            nvm_err_stat->privil_error = BMI2_GET_BITS(feat_config[idx], BMI2_NVM_PRIV_ERR_STATUS);
        }
    }
    else
    {
        rslt = BMI2_E_INVALID_SENSOR;
    }

    return rslt;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_user_gain_upd_status(XBYTE *status, struct bmi2_dev *dev)
* @brief      getusergainupdstatus
* @ingroup    DATAIO
* 
* @param[in]  status : Status value to read or update.
* @param[in]  dev : Dev pointer to use.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_user_gain_upd_status(XBYTE *status, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt = BMI2_OK;

    /* Array to define the feature configuration */
    XBYTE feat_config[BMI2_FEAT_SIZE_IN_BYTES] = { 0 };

    /* Variable to define the array offset */
    XBYTE idx = 0;

    /* Variable to set flag */
    XBYTE feat_found;

    /* Variable to check APS status */
    XBYTE aps_stat = 0;

    /* Initialize feature configuration for gyroscope user gain */
    struct bmi2_feature_config gyr_user_gain_cfg = { 0, 0, 0 };

    /* Search for user gain feature and extract its configuration details */
    feat_found = bmi2_extract_input_feat_config(&gyr_user_gain_cfg, BMI2_GYRO_GAIN_UPDATE, dev);
    if(feat_found)
    {
        /* Disable advance power save */
        aps_stat = dev->aps_status;
        if(aps_stat == BMI2_ENABLE)
        {
            rslt = bmi2_set_adv_power_save(BMI2_DISABLE, dev);
        }

        if(rslt == BMI2_OK)
        {
            /* Get the configuration from the page where user gain feature resides */
            rslt = bmi2_get_feat_config(gyr_user_gain_cfg.page, feat_config, dev);
            if(rslt == BMI2_OK)
            {
                /* Define the offset for enable/disable of user gain */
                idx = gyr_user_gain_cfg.start_addr + BMI2_GYR_USER_GAIN_FEAT_EN_OFFSET;

                /* Set the feature enable status */
                *status = BMI2_GET_BITS(feat_config[idx], BMI2_GYR_USER_GAIN_FEAT_EN);
            }
        }
    }
    else
    {
        rslt = BMI2_E_INVALID_SENSOR;
    }

    /* Enable Advance power save if disabled while configuring and not when already disabled */
    if((rslt == BMI2_OK) && (aps_stat == BMI2_ENABLE))
    {
        rslt = bmi2_set_adv_power_save(BMI2_ENABLE, dev);
    }

    return rslt;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_step_activity_output(XBYTE *step_act, struct bmi2_dev *dev)
* @brief      getstepactivityoutput
* @ingroup    DATAIO
* 
* @param[in]  step_act : Step act pointer to use.
* @param[in]  dev : Dev pointer to use.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_step_activity_output(XBYTE *step_act, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Array to define the feature configuration */
    XBYTE feat_config[BMI2_FEAT_SIZE_IN_BYTES] = { 0 };

    /* Variables to define index */
    XBYTE idx = 0;

    /* Variable to set flag */
    XBYTE feat_found;

    /* Initialize feature output for step activity */
    struct bmi2_feature_config step_act_out_config = { 0, 0, 0 };

    /* Search for step activity output feature and extract its configuration details */
    feat_found = extract_output_feat_config(&step_act_out_config, BMI2_STEP_ACTIVITY, dev);
    if(feat_found)
    {
        /* Get the feature output configuration for step-activity */
        rslt = bmi2_get_feat_config(step_act_out_config.page, feat_config, dev);
        if(rslt == BMI2_OK)
        {
            /* Define the offset in bytes for step activity output */
            idx = step_act_out_config.start_addr;

            /* Get the step activity output */
            *step_act = feat_config[idx];
        }
    }
    else
    {
        rslt = BMI2_E_INVALID_SENSOR;
    }

    return rslt;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_vfrm_error_status(struct bmi2_vfrm_err_status *vfrm_err_stat, struct bmi2_dev *dev)
* @brief      getvfrmerrorstatus
* @ingroup    DATAIO
* 
* @param[in]  vfrm_err_stat : Vfrm err stat pointer to use.
* @param[in]  dev : Dev pointer to use.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::get_vfrm_error_status(struct bmi2_vfrm_err_status *vfrm_err_stat, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Array to define the feature configuration */
    XBYTE feat_config[BMI2_FEAT_SIZE_IN_BYTES] = { 0 };

    /* Variables to define index */
    XBYTE idx = 0;

    /* Variable to set flag */
    XBYTE feat_found;

    /* Initialize feature output for VFRM error status */
    struct bmi2_feature_config vfrm_err_cfg = { 0, 0, 0 };

    /* Search for VFRM error status feature and extract its configuration details */
    feat_found = extract_output_feat_config(&vfrm_err_cfg, BMI2_VFRM_STATUS, dev);
    if(feat_found)
    {
        /* Get the feature output configuration for VFRM error status */
        rslt = bmi2_get_feat_config(vfrm_err_cfg.page, feat_config, dev);
        if(rslt == BMI2_OK)
        {
            /* Define the offset in bytes for VFRM error status */
            idx = vfrm_err_cfg.start_addr;

            /* Increment index to get the error status */
            idx++;

            /* Internal error while acquiring lock for FIFO */
            vfrm_err_stat->lock_error = BMI2_GET_BITS(feat_config[idx], BMI2_VFRM_LOCK_ERR_STATUS);

            /* Internal error while writing byte into FIFO */
            vfrm_err_stat->write_error = BMI2_GET_BITS(feat_config[idx], BMI2_VFRM_WRITE_ERR_STATUS);

            /* Internal error while writing into FIFO */
            vfrm_err_stat->fatal_error = BMI2_GET_BITS(feat_config[idx], BMI2_VFRM_FATAL_ERR_STATUS);
        }
    }
    else
    {
        rslt = BMI2_E_INVALID_SENSOR;
    }

    return rslt;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTESIG DIOI2C6AXISTRACKINGBMI270::enable_gyro_gain(XBYTE enable, struct bmi2_dev *dev)
* @brief      enablegyrogain
* @ingroup    DATAIO
* 
* @param[in]  enable : Enable value.
* @param[in]  dev : Dev pointer to use.
* 
* @return     XBYTESIG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTESIG DIOI2C6AXISTRACKINGBMI270::enable_gyro_gain(XBYTE enable, struct bmi2_dev *dev)
{
    /* Variable to define error */
    XBYTESIG rslt;

    /* Variable to define register data */
    XBYTE reg_data = 0;

    rslt = bmi2_get_regs(BMI2_GYR_OFF_COMP_6_ADDR, &reg_data, 1, dev);
    if(rslt == BMI2_OK)
    {
        reg_data = BMI2_SET_BITS(reg_data, BMI2_GYR_GAIN_EN, enable);
        rslt = bmi2_set_regs(BMI2_GYR_OFF_COMP_6_ADDR, &reg_data, 1, dev);
    }

    return rslt;
}


/*!
 * @brief This internal API is used to extract the output feature configuration
 * details from the look-up table.
 */
//XBYTE DIOI2C6AXISTRACKINGBMI270::extract_output_feat_config(struct bmi2_feature_config *feat_output,
//                                          XBYTE type,
//                                          const struct bmi2_dev *dev)
//{
//    /* Variable to define loop */
//    XBYTE loop = 0;
//
//    /* Variable to set flag */
//    XBYTE feat_found = BMI2_FALSE;
//
//    /* Search for the output feature from the output configuration array */
//    while (loop < dev->out_sens)
//    {
//        if(dev->feat_output[loop].type == type)
//        {
//            *feat_output = dev->feat_output[loop];
//            feat_found = BMI2_TRUE;
//            break;
//        }
//
//        loop++;
//    }
//
//    /* Return flag */
//    return feat_found;
//}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C6AXISTRACKINGBMI270::Handler_Interrupt(void* param)
* @brief      Handler interrupt
* @ingroup    DATAIO
* 
* @param[in]  param : Param pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C6AXISTRACKINGBMI270::Handler_Interrupt(void* param)
{
  DIOI2C6AXISTRACKINGBMI270* imu = (DIOI2C6AXISTRACKINGBMI270*)param;
  if(imu) imu->SetIsNecessaryUpdate(true);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOI2C6AXISTRACKINGBMI270::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOI2C6AXISTRACKINGBMI270::Clean()
{
  GPIOinterruptentryID  =  DIOGPIO_ID_NOTDEFINED;

  isnecessaryupdate     = false;  

  dev_addr              = 0;
  memset(&bmi2_dev, 0, sizeof(struct bmi2_dev));

  memset(sens_list, 0, 5);
    
  sig_motion            = 0;
  highGmotion           = 0;
  lowGmotion            = 0;
  
  step_event            = 0;
  tap_event             = 0;
  any_motion            = 0;
  no_motion             = 0;    
    
  prev_status           = -1;    
}

/// @endcond

