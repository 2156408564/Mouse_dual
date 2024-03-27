/********************************************************************************************************
 * @file     aaa_public.c
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
#include "AAA_app_config.h"

u8 key_code[8] = {0, 0, 0, 0, 0, 0, 0, 0};
u8 need_pop_internet = 0;
u8 need_win_R_internet = 0;
u8 btn_long_internet_flag = 0;
u32 need_pop_internet_time = 0;
volatile u8 does_pop_internet_enable = 1;
u8 need_back_to_desktop  = 0;
volatile static u32 host_out_tick;

static const u8 win_r_data[][2] = {
	/*Send win + R	down and up*/
	{0x08,		0x00},
	{0x08,		KEYBOARD_R},
	{0x08,		0x00},
	{0x00,		0x00},
};

static const u8	key_data[][2] = {
	//byte 0,  byte 1

/*CAPS LOCK*/
	{0x00,		CAPS_LOCK},
	{0x00,		0x00},
/*W*/
	{0x00,		KEYBOARD_W},
	{0x00,		0x00},
/*W*/
	{0x00,		KEYBOARD_W},
	{0x00,		0x00},
/*W*/
	{0x00,		KEYBOARD_W},
	{0x00,		0x00},
/*dot*/
	{0x00,		KEYBOARD_DOT},
	{0x00,		0x00},
/*m*/
	{0x00,		KEYBOARD_M},
	{0x00,		0x00},
/*i*/
	{0x00,		KEYBOARD_I},
	{0x00,		0x00},
/*c*/
	{0x00,		KEYBOARD_C},
	{0x00,		0x00},
/*l*/
	{0x00,		KEYBOARD_L},
	{0x00,		0x00},
/*i*/
	{0x00,		KEYBOARD_I},
	{0x00,		0x00},
/*n*/
	{0x00,		KEYBOARD_N},
	{0x00,		0x00},
/*k*/
	{0x00,		KEYBOARD_K},
	{0x00,		0x00},
/*dot*/
	{0x00,		KEYBOARD_DOT},
	{0x00,		0x00},
/*n*/
	{0x00,		KEYBOARD_N},
	{0x00,		0x00},
/*e*/
	{0x00,		KEYBOARD_E},
	{0x00,		0x00},
/*t*/
	{0x00,		KEYBOARD_T},
	{0x00,		0x00},
/*/*/
	{0x00,		KEYBOARD_VIRGULE},
	{0x00,		0x00},
/*t*/
	{0x00,		KEYBOARD_B},
	{0x00,		0x00},
/*d*/
	{0x00,		KEYBOARD_D},
	{0x00,		0x00},
/*l*/
	{0x00,		KEYBOARD_L},
	{0x00,		0x00},
/*CAPS LOCK*/
	{0x00,		CAPS_LOCK},
	{0x00,		0x00},
/*ENTER*/
	{0x00,		KEYBOARD_ENTER},
	{0x00,		0x00},
};
void five_sec_win_r_internet(){
	static u16 step_cnt = 0;
    static u8 pop_step = 0;
    // www.miclink.net/tdl
    step_cnt++;

    if (step_cnt % 5 == 0) {
        //printf(" five_second_pop_internet\r\n");

        if (fun_mode == RF_1M_BLE_MODE) {
            key_code[0] = win_r_data[pop_step][0];
            key_code[2] = win_r_data[pop_step][1];
            u8 status = blc_gatt_pushHandleValueNotify(BLS_CONN_HANDLE, HID_NORMAL_KB_REPORT_INPUT_DP_H, key_code, sizeof(key_code));
            if (status != BLE_SUCCESS)
            {
                printf("notify fail\n");
            }
        } else {
            key_code[0] = KB_CMD;
            if (win_r_data[pop_step][0] == 0x08) {
                key_code[1] = 0xE3;/*L_WIN*/
                key_code[2] = win_r_data[pop_step][1];
            } else {
                key_code[1] = win_r_data[pop_step][1];
                key_code[2] = 0;
            }
            my_fifo_push(&fifo_km, key_code, sizeof(key_code));
        }

        pop_step++;

        if(pop_step >= (sizeof(win_r_data) / 2)) {
            need_win_R_internet = 0;
			need_pop_internet_time = clock_time();
            pop_step = 0;
            step_cnt = 0;
        }
    }
}

void five_sec_pop_internet(void)
{
    static u16 step_cnt = 0;
    static u8 pop_step = 0;
	if(need_pop_internet_time && (!clock_time_exceed(need_pop_internet_time, 300 * 1000))){
		return;
	}

    // www.miclink.net/tdl
    step_cnt++;

    if (step_cnt % 7 == 0) {
        //printf(" five_second_pop_internet\r\n");

        if (fun_mode == RF_1M_BLE_MODE) {
            key_code[0] = key_data[pop_step][0];
            key_code[2] = key_data[pop_step][1];
            u8 status = blc_gatt_pushHandleValueNotify(BLS_CONN_HANDLE, HID_NORMAL_KB_REPORT_INPUT_DP_H, key_code, sizeof(key_code));
            if (status != BLE_SUCCESS)
            {
                printf("notify fail\n");
            }
        } else {
            key_code[0] = KB_CMD;
            if (key_data[pop_step][0] == 0x08) {
                key_code[1] = 0xE3;/*L_WIN*/
                key_code[2] = key_data[pop_step][1];
            } else {
                key_code[1] = key_data[pop_step][1];
                key_code[2] = 0;
            }
            my_fifo_push(&fifo_km, key_code, sizeof(key_code));
        }

        pop_step++;

        if(pop_step >= (sizeof(key_data) / 2)) {
            need_pop_internet = 0;
            pop_step = 0;
            step_cnt = 0;
        }
    }
}


static const u8 desktop_data[][2] = {
	//byte 0,  byte 1
/*Send win + D	desktop*/
	{0x08,		0x00},
	{0x08,		KEYBOARD_D},
	{0x08,		0x00},
	{0x00,		0x00},
};

void back_to_desktop(void)
{
	static u8 pop_step = 0;

    printf(" back_to_desktop\r\n");

    for (int i = 0; i < (sizeof(desktop_data) / 2); i++) {
    	key_code[0] = desktop_data[i][0];
        key_code[2] = desktop_data[i][1];
		u8 status = blc_gatt_pushHandleValueNotify(BLS_CONN_HANDLE, HID_NORMAL_KB_REPORT_INPUT_DP_H, key_code, sizeof(key_code));
		if (status != BLE_SUCCESS)
		{
			printf("notify fail\n");
		}

		pop_step++;

		if(pop_step >= (sizeof(desktop_data) / 2)) {
			need_back_to_desktop = 0;
			pop_step = 0;
		}
    }
}


void miclink_key_handle(u8 event_new)
{

	/*five second press middle key , pop internet*/
	if (btn_long_internet_flag &&((btn_value&MS_BTN_MIDDLE) == MS_BTN_MIDDLE) && (clock_time_exceed(btn_tick, 3100000))) {
		btn_tick = clock_time();
		if (does_pop_internet_enable){
			need_win_R_internet = 1;
			need_pop_internet = 1;
		}
		btn_long_internet_flag = 0;
	}
	if (btn_long_internet_flag && ((btn_value&MS_BTN_KEY_WRITE) == MS_BTN_KEY_WRITE) && (clock_time_exceed(btn_tick, 3100000))) {
		btn_tick = clock_time();
		if (does_pop_internet_enable){
			need_win_R_internet = 1;
			need_pop_internet = 1;
		}
		btn_long_internet_flag = 0;
	}
	if (btn_long_internet_flag && ((btn_value&MS_BTN_SEARCH) == MS_BTN_SEARCH) && (clock_time_exceed(btn_tick, 3100000))) {
		btn_tick = clock_time();
		if (does_pop_internet_enable){
			need_win_R_internet = 1;
			need_pop_internet = 1;
		}
		btn_long_internet_flag = 0;
	}
	if (btn_long_internet_flag && ((btn_value&MS_BTN_KEY_TRANSLATOR) == MS_BTN_KEY_TRANSLATOR) && (clock_time_exceed(btn_tick, 3100000))) {
		btn_tick = clock_time();
		if (does_pop_internet_enable){
			need_win_R_internet = 1;
			need_pop_internet = 1;
		}
		btn_long_internet_flag = 0;
	}
}
////////////////////////////////////////MICLINK INFO//////////////////////////////////////////////

void miclink_host_cmd_handler(u8 *host_cmd)
{
	u8 data[21];
    u8 cmd;
    u8 data_crrect = 0;
	//can be saved 
	u8 keycode[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	u8 need_notify = 1;

    memset(data, 0, sizeof(data));

	data[0] = USER_DEFINE_CMD;
	data[1] = PROTOCOL_HEARD;

	if (host_cmd[0] == PROTOCOL_HEARD) {

		cmd = host_cmd[2];
		does_pop_internet_enable = 0;
		switch (cmd) {
			case HOST_GET_INFO:
				data[2] = 0x14;
				data[3] = HOST_GET_INFO;
				/* SN */
				data[4] = flash_dev_info.host_set_sn[0];
				data[5] = flash_dev_info.host_set_sn[1];
				data[6] = flash_dev_info.host_set_sn[2];
				data[7] = flash_dev_info.host_set_sn[3];
				data[8] = flash_dev_info.host_set_sn[4];
				data[9] = flash_dev_info.host_set_sn[5];
				data[10] = flash_dev_info.host_set_sn[6];
				data[11] = flash_dev_info.host_set_sn[9];
				data[12] = flash_dev_info.host_set_sn[8];
				data[13] = flash_dev_info.host_set_sn[7];

				/* bit 2: mic reat, 1 16k, 0 8k
				 *bit 1: LED ; 
				 *bit 0 MIC 
				 */
				data[14] = (flash_dev_info.audio_reat << 2)|audio_status;
#if SENSOR_FUN_ENABLE_AAA
				/* sensor type */
				if (SENSOR_M8650A == sensor_type) {
					data[15] = 0x06;
				} else if (SENSOR_PAW3212 == sensor_type) {
					data[15] = 0x13;
				} else if (SENSOR_3205_TJ3T == sensor_type || SENSOR_3205_TJNT == sensor_type) {
					data[15] = 0x11;
				} else if (SENSOR_3065 == sensor_type) {
                    data[15] = 0x0f;
                } else if (SENSOR_3065XY == sensor_type) {
                    data[15] = 0x12;
                } else if (SENSOR_KA8G2 == sensor_type) {
                    data[15] = 0x08;
                } else if (SENSOR_S201B == sensor_type) {
                    data[15] = 0x03;
                }
#endif

				/* DPI */
				data[16] = flash_dev_info.dpi;
				/* Mouse type */
				data[17] = flash_dev_info.mouse_type;
				/* battery */
				data[18] = my_batVal[0];
				/* version V1.0 */
				data[19] = 0x26;
				data[20] = PROTOCOL_TAIL;
                printf("s_type: %x, dpi: %d\r\n", sensor_type, flash_dev_info.dpi);
				break;

			case HOST_OPEN_LED:
                data[2] = 0x05;
                data[3] = HOST_OPEN_LED;
                data[4] = 0x00;
                data[5] = PROTOCOL_TAIL;
                break;

			case HOST_CLOSE_LED:
                data[2] = 0x05;
                data[3] = HOST_CLOSE_LED;
                data[4] = 0x00;
                data[5] = PROTOCOL_TAIL;
				break;

			case HOST_OPEN_MIC:
                data[2] = 0x05;
				data[3] = HOST_OPEN_MIC;
                data[4] = 0x00;
                data[5] = PROTOCOL_TAIL;
                
                //printf("=======audio_status %d\r\n", audio_status);
                if (audio_status == false) {
                    printf("start audio by cmd\r\n");
                    audio_status = true;
                    key_voice_is_press();
                }
                break;

			case HOST_CLOSE_MIC:
                data[2] = 0x05;
				data[3] = HOST_CLOSE_MIC;
                data[4] = 0x00;
                data[5] = PROTOCOL_TAIL;
                if (audio_status == true) {
                    printf("stop audio by cmd\r\n");
                    audio_status = false;
                    key_voice_is_release();
                }
				break;

			case HOST_ENABLE_POP_INT:
				does_pop_internet_enable  = 1;
				data[2] = 0x05;
				data[3] = HOST_ENABLE_POP_INT;
				data[4] = 0x00;
				data[5] = PROTOCOL_TAIL;
				break;

			case HOST_DISABLE_POP_INT:
				does_pop_internet_enable = 0;
				data[2] = 0x05;
				data[3] = HOST_DISABLE_POP_INT;
				data[4] = 0x00;
				data[5] = PROTOCOL_TAIL;
				break;

			case HOST_SET_DPI:
                if (0 < host_cmd[3] &&  host_cmd[3] < 6) {
                    flash_dev_info.dpi = host_cmd[3];
                    #if SENSOR_FUN_ENABLE_AAA
                    sensor_dpi_set(flash_dev_info.dpi - 1);
                    #endif
                    #if DPI_SAVE_FLASH
                    save_dev_info_flash();
                    #endif
                    data[2] = 0x05;
                    data[3] = HOST_SET_DPI;
                    data[4] = 0x00;
                    data[5] = PROTOCOL_TAIL;
                }

				break;
            case HOST_SET_MOUSE_SN:

                if (fun_mode == RF_1M_BLE_MODE) {
                    memcpy(&flash_dev_info.host_set_sn[0], &host_cmd[3], 10);
                    save_dev_info_flash();
                }
                data[2] = 0x05;
                data[3] = HOST_SET_MOUSE_SN;
                data[4] = 0;
                data[5] = PROTOCOL_TAIL;
                break;

            case HOST_SET_MOUSE_TYPE:
                if (host_cmd[3] != 0 && host_cmd[3]!= U8_MAX) {
                    flash_dev_info.mouse_type = host_cmd[3];
                    save_dev_info_flash();
                }
                data[2] = 0x05;
                data[3] = HOST_SET_MOUSE_TYPE;
                data[4] = 0;
                data[5] = PROTOCOL_TAIL;
                break;

            case HOST_SET_MOUSE_WEB:
                if (host_cmd[3]!= U8_MAX) {
                    flash_dev_info.pop_int = host_cmd[3];
                    save_dev_info_flash();
                }

                data[2] = 0x05;
                data[3] = HOST_SET_MOUSE_WEB;
                data[4] = 0;
                data[5] = PROTOCOL_TAIL;
                break;

            case HOST_SET_AUDIO_REAT:
                //u8 sensor_direct;//25
                if (host_cmd[3] == AUDIO_8K || host_cmd[3] == AUDIO_16K) {
                    flash_dev_info.audio_reat = host_cmd[3];
                    audio_reat_setting = flash_dev_info.audio_reat;
                    save_dev_info_flash();
                }

                data[2] = 0x05;
                data[3] = HOST_SET_AUDIO_REAT;
                data[4] = 0;
                data[5] = PROTOCOL_TAIL;
                break;

            case HOST_SET_SENSOR_DIRECT:
                //no used
                #if 0
                if (0 <= host_cmd[3] && host_cmd[3]< 4) {
                    flash_dev_info.sensor_direct = host_cmd[3];
                    save_dev_info_flash();
                }
                data[2] = 0x05;
                data[3] = HOST_SET_SENSOR_DIRECT;
                data[4] = 0;
                data[5] = PROTOCOL_TAIL;
                #endif
				need_notify = 0;
                break;

			case HOST_CONNECTED:
                host_out_tick = clock_time();
				host_connected = 1;
				need_notify = 0;
				break;

			case HOST_SEND_KEYBOARD_CODE:
				keycode[0] = host_cmd[3];
				keycode[2] = host_cmd[4];
                if (fun_mode == RF_1M_BLE_MODE) {
                    blc_gatt_pushHandleValueNotify(BLS_CONN_HANDLE, HID_NORMAL_KB_REPORT_INPUT_DP_H, keycode, sizeof(keycode));
                    memset(keycode, 0,sizeof(keycode));
                    blc_gatt_pushHandleValueNotify(BLS_CONN_HANDLE, HID_NORMAL_KB_REPORT_INPUT_DP_H, keycode, sizeof(keycode));
                }
#if 0
                else {
                    data[0] = KB_CMD;
                    data[1] = host_cmd[3];
                    data[2] = 0;
                    data[3] = host_cmd[4];
                    data[4] = 0;
                    data[5] = 0;
                    data[6] = 0;
                    printf("kb cmd %x %x\r\n", data[1], data[3]);
                    my_fifo_push(&fifo_km, data, 7);
                    data[1] = 0;
                    data[3] = 0;
                    my_fifo_push(&fifo_km, data, 7);
                }
#endif
                data[2] = 0x05;
				data[3] = HOST_SEND_KEYBOARD_CODE;
				data[4] = 0x00;
				data[5] = PROTOCOL_TAIL;
				break;
			case DOES_MOUSE_CONNECTED:
				data[2] = 0x05;
				data[3] = DOES_MOUSE_CONNECTED;
				data[4] = (connect_ok)?0:1;
				data[5] = my_batVal[0];
				data[6] = PROTOCOL_TAIL;
				break;
			case HOST_ERASE_FLASH_INFO:
				printf("HOST_ERASE_FLASH_INFO\r\n");
				flash_erase_sector(CFG_DEVICE_MODE_ADDR);
				user_reboot(DEEP_SLEEP_ANA_AAA);
				break;
			default:
				need_notify = 0;
				break;
		}

        #if 0
        if (fun_mode == RF_1M_BLE_MODE) {
			if (need_notify) {
                status = blc_gatt_pushHandleValueNotify(BLS_CONN_HANDLE, HID_ML_REPORT_INPUT_CMD_DP_H, &data[1], send_len);
                if (status != BLE_SUCCESS) {
                    printf("error\n");
            }
        }
        //2.4G Mode
        } else {
			if (need_notify) {
                my_fifo_push(&fifo_km, data, sizeof(data));
			}
        }
        #endif
	}

    if (need_notify) {
        my_fifo_push(&fifo_km, data, sizeof(data));
    }
}


void check_host_status(void)
{

    if (host_connected && clock_time_exceed(host_out_tick, 5*1000*1000)) {
        host_out_tick = clock_time();
        host_connected = 0;
        if (audio_status == true) {
            printf("stop audio because host disconnect\r\n");
            audio_status = false;
            key_voice_is_release();
        }

    }
}

