/********************************************************************************************************
 * @file     AAA_Sensor.h
 *
 * @brief    This is the header file for KMD SDK
 *
 * @author	 KMD GROUP
 * @date         01,2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *******************************************************************************************************/

#ifndef MOUSE_SENSOR_H_
#define MOUSE_SENSOR_H_


//#include "aaa_sensor_pix.h"


#ifndef MOUSE_SENSOR_CUS
    #define MOUSE_SENSOR_CUS      1
#endif

#ifndef MOUSE_SENSOR_MOTION
    #define MOUSE_SENSOR_MOTION      1
#endif

#ifndef MOUSE_SENSOR_A3000_EN
    #define MOUSE_SENSOR_A3000_EN   0//((MCU_CORE_TYPE == MCU_CORE_8366) || 0)
#endif

#define SENSOR_DATA_LENGTH_12_BIT_ENABLE	1 //0:8Bit  1:12bit

#define SENSOR_DIRECTION_CLOCK_12	0
#define SENSOR_DIRECTION_CLOCK_3	1
#define SENSOR_DIRECTION_CLOCK_9	2
#define SENSOR_DIRECTION_CLOCK_6	3

#define SENSOR_MODE_POWERDOWN  0x40
#define SENSOR_MODE_WORKING    0x20
#define SENSOR_MODE_POWERUP    0x10
#define SENSOR_MODE_A3000      0x80

#define SENSOR_STATUS_CTRL  0xf0
#define SENSOR_TYPE_CTRL    0x0f

//---------------------------------------------
// Sensor ID
//---------------------------------------------
//---------------------------------------------
// Sensor type
//---------------------------------------------
#define SENSOR_3204_SUPPORT_ENABLE  0
#define SENSOR_3204UL_SUPPORT_ENABLE  0
#define SENSOR_3205_TJ3T_SUPPORT_ENABLE  1
#define SENSOR_3205_TJNT_SUPPORT_ENABLE  1
#define SENSOR_3605_SUPPORT_ENABLE  1
#define SENSOR_3605XYD_SUPPORT_ENABLE  1
#define SENSOR_VT108_SUPPORT_ENABLE  1
#define SENSOR_M8650A_SUPPORT_ENABLE  1
#define SENSOR_OM18_SUPPORT_ENABLE  1
#define SENSOR_YS8009_SUPPORT_ENABLE  1
#define SENSOR_KA8G2_SUPPORT_ENABLE	1
#define SENSOR_3212_SUPPORT_ENABLE  1
#define SENSOR_S201B_SUPPORT_ENABLE  1



#define SENSOR_3204       0x00
#define SENSOR_3204LL     0x01
#define SENSOR_3205_TJ3T	  0x02
#define SENSOR_3205_TJNT	  0x03
#define SENSOR_3065	  	  0x04  //Stop production
#define SENSOR_3065XY	  0x05
#define SENSOR_VT108	  0x06
#define SENSOR_M8650A 	  0x07
#define SENSOR_OM18		  0x08
#define SENSOR_YS8009		0x09
#define SENSOR_KA8G2		0x0a
#define SENSOR_PAW3212	    0x0b
#define SENSOR_S201B	    0x0C

#define SENSOR_A3000		0x0D




unsigned int OPTSensor_motion_report(u32 no_overflow);
void sensor_dpi_set(u8 dpi);

extern  u8 dpi_max_level;
void check_sensor_dircet(u8 sensor_dir);

extern int (*pf_sensor_motion_report)(signed char *pBuf, u32 no_overflow);

u32 mouse_sensor_sleep_wakeup(u8 *p_sensor, u8 *sleep, u32 wakeup);
void Sensor_Concent_Init(void);

#if 0
//sim sensor blinky when mouse is in sensor sleep mode
static inline u32 mouse_sensor_blinky_wkup(u32 blink_en)
{
    _attribute_data_retention_user  static u8 sns_blnky_cnt  = 0;
    if (blink_en)
        sns_blnky_cnt++;
    else
        sns_blnky_cnt = 0;
    return sns_blnky_cnt & 0x08;
}

static inline void mouse_sensor_dir_adjust(signed char *px, signed char *py, unsigned char sensor_dir)
{
    if (sensor_dir & SENSOR_X_REVERSE)
        *px = - *px;

    if (sensor_dir & SENSOR_Y_REVERSE)
        *py = - *py;

    if (sensor_dir & SENSOR_XY_SWITCH)
    {
        s8 tmp = *py;
        *py = *px;
        *px = tmp;
    }
}
#endif


#endif /* MOUSE_SENSOR_H_ */

