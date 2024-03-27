/********************************************************************************************************
 * @file     aaa_led.c
 *
 * @brief    for TLSR chips
 *
 * @author	 telink
 * @date     Sep. 30, 2010
 *
 * @par      Copyright (c) 2010, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *			 The information contained herein is confidential and proprietary property of Telink
 * 		     Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *			 of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *			 Co., Ltd. and the licensee in separate contract or the terms described here-in.
 *           This heading MUST NOT be removed from this file.
 *
 * 			 Licensees are granted free, non-transferable use of the information in this
 *			 file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/

#include "AAA_public_config.h"


#if BLT_APP_LED_ENABLE
u8 bat_lvd_led_flag = 0;
typedef enum
{
    BAT_LVD_LED_PRIORITY,
    DPI_LED_PRIORITY,
    RECONN_LED_PRIORITY,
    ADV_LED_PRIORITY,
    MODE_LED_PRIORITY,
    CONNECTED_LED_PRIORITY,
    POWER_OFF_LED_PRIORITY,
} LED_CFG_ENUM_AAA;


_attribute_data_retention_user led_cfg_t dpi_led = {250, 250, 1, DPI_LED_PRIORITY};
const led_cfg_t adv_24g_led = {200,	200,	1,	ADV_LED_PRIORITY};
const led_cfg_t adv_ble_led = {200,	200,	1,	ADV_LED_PRIORITY};

const led_cfg_t bat_lvd_led = {300,	  300,  10,	BAT_LVD_LED_PRIORITY};
const led_cfg_t power_off_led = {300, 300,  10,	POWER_OFF_LED_PRIORITY};


const led_cfg_t mod_24_led = {500,	500,	1,	MODE_LED_PRIORITY};
const led_cfg_t mod_ble_led = {500,	500,	2,	MODE_LED_PRIORITY};

const led_cfg_t reconn_24g_led = {500, 500,	1,	RECONN_LED_PRIORITY};
const led_cfg_t reconn_ble_led = {500, 500,	1,	RECONN_LED_PRIORITY};

const led_cfg_t connected_led = {5000, 500,	1,	CONNECTED_LED_PRIORITY};
const led_cfg_t ch1_connected_led = {5000, 300,	1,	CONNECTED_LED_PRIORITY};
const led_cfg_t ch2_connected_led = {5000, 300,	1,	CONNECTED_LED_PRIORITY};


void dpi_led_set(u8 idx)
{
    dpi_led.repeatCount = idx;
    device_led_setup(dpi_led);
}

void led_ble_Adv_poll()
{
	if(bat_lvd_led_flag){
		device_led_init(PIN_BLE_LED_1, LED_ON_AAA);
		bat_lvd_led_flag = 0;
	}
    if (pair_flag)
    {
        device_led_setup(adv_ble_led);
    }
    else
    {
        device_led_setup(reconn_ble_led);
    }
}
void led_2p4_Adv_poll()
{
	if(bat_lvd_led_flag){
		device_led_init(PIN_24G_LED, LED_ON_AAA);
		bat_lvd_led_flag = 0;
	}
    if (pair_flag)
    {
        device_led_setup(adv_24g_led);
    }
    else
    {
        device_led_setup(reconn_24g_led);
    }
}

void led_bat_lvd()
{
    device_led_setup(bat_lvd_led);
}
void led_power_off()
{
    device_led_setup(power_off_led);
}
void led_ble_ConnectedStatus()
{
	if(bat_lvd_led_flag){
		device_led_init(PIN_BLE_LED_1, LED_ON_AAA);
		bat_lvd_led_flag = 0;
	}
	if (flash_dev_info.mast_id == 0) {
        device_led_setup(ch1_connected_led);
	} else {
        device_led_setup(ch2_connected_led);
	}
}

void led_24g_ConnectedStatus()
{
	if(bat_lvd_led_flag){
		device_led_init(PIN_24G_LED, LED_ON_AAA);
		bat_lvd_led_flag = 0;
	}
    device_led_setup(connected_led);
}

void led_hw_init()
{
    if (fun_mode == RF_1M_BLE_MODE)
    {
        device_led_init(PIN_BLE_LED_1, LED_ON_AAA);
        #if 0
		if (flash_dev_info.mast_id == 0) {
            device_led_init(PIN_BLE_LED_1, LED_ON_AAA);
		} else {
			device_led_init(PIN_BLE_LED_2, LED_ON_AAA);
		}
        #endif
    }
    else
    {
        device_led_init(PIN_24G_LED, LED_ON_AAA);
    }
}


void fenwei_led_on(void)
{
    //gpio_write(PIN_FENWEI_LED, LED_ON_AAA);
}

void fenwei_led_off(void)
{
    //gpio_write(PIN_FENWEI_LED, LED_OFF_AAA);
}

void mode_led_switch(void)
{
	static u8 last_connect = 0;
	if (last_connect != connect_ok)
	{
		last_connect = connect_ok;
		if (connect_ok)
		{
			Sensor_Concent_Init();
		}
		else
		{
			OPTSensor_Shutdown();
		}
	}
}


void low_battery_indecate(void)
{
    device_led_init(PIN_BAT_LED, LED_ON_AAA);
	bat_lvd_led_flag = 1;
    device_led_setup(bat_lvd_led);
}

void power_off_led_indecate(void)
{
    device_led_init(PIN_BAT_LED, LED_ON_AAA);
	bat_lvd_led_flag = 1;
    device_led_setup(power_off_led);
}

void ota_success_led(){
	wd_stop();
	for(u8 i = 0 ; i < 3; i ++){
		gpio_write(PIN_24G_LED, 0);
		sleep_ms(200);

		gpio_write(PIN_24G_LED,1);
		sleep_ms(200);
	}
}

void ota_fail_led(){
	wd_stop();
	for(u8 i = 0 ; i < 5; i ++){
		gpio_write(PIN_24G_LED, 0);
		sleep_ms(200);

		gpio_write(PIN_24G_LED,1);
		sleep_ms(200);
	}
}

#endif
