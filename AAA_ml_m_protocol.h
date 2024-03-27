/********************************************************************************************************
 * @file     aaa_public_config.h
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

#ifndef _AAA_ML_M_PROTOCOL_H_
#define _AAA_ML_M_PROTOCOL_H_

#define KEYBOARD_R 0x15
#define KEYBOARD_ENTER 0x28
#define KEYBOARD_W 0x1A
#define KEYBOARD_DOT 0x37
#define KEYBOARD_M 0x10
#define KEYBOARD_I 0x0c
#define KEYBOARD_C 0x06
#define KEYBOARD_L 0x0F
#define KEYBOARD_N 0x11
#define KEYBOARD_K 0x0E
#define KEYBOARD_E 0x08
#define KEYBOARD_T 0x17
#define KEYBOARD_D 0x07
#define KEYBOARD_B 0x05
#define KEYBOARD_P 0x13
#define KEYBOARD_VIRGULE 0x38
#define CAPS_LOCK 0x39


#define PROTOCOL_HEARD      0x4D
#define PROTOCOL_TAIL       0x4C

enum {
	HOST_CONNECTED = 0x10,
	HOST_SEND_KEYBOARD_CODE = 0x20,
	DEVICE_CHANGE_MODE = 0x60,
	HOST_GET_INFO = 0x61,
	HOST_OPEN_MIC = 0x62,
	HOST_CLOSE_MIC = 0x63,
	HOST_OPEN_LED = 0x64,
	HOST_CLOSE_LED = 0x65,
	HOST_ENABLE_POP_INT = 0x66,
	HOST_DISABLE_POP_INT = 0x67,
	HOST_SET_DPI = 0x68,
	DOES_MOUSE_CONNECTED = 0x69,
	HOST_SET_MOUSE_SN = 0x6a,
	HOST_SET_MOUSE_TYPE = 0x6b,
	HOST_SET_MOUSE_WEB = 0x6c,
	HOST_SET_AUDIO_REAT = 0x6d,
	HOST_SET_SENSOR_DIRECT = 0x6e,
	HOST_ERASE_FLASH_INFO = 0x6f,
	HOST_SLAVE_CONNECT = 0x70,

	NEED_POP_INTERNET = 0xFD,
	NEED_BACK_HOME = 0xFE,
};

enum {
    WRITE_KEY_SINGLE_CLICK = 0x33,
    WRITE_KEY_DOUBLE_CLICK = 0x40,
    WRITE_KEY_LONG_DOWN = 0x37,
    WRITE_KEY_LONG_UP = 0x38,
    SEARCH_KEY_SINGLE_CLICK = 0x35,
    SEARCH_KEY_DOUBLE_CLICK = 0x41,
    SEARCH_KEY_LONG_DOWN = 0x31,
    SEARCH_KEY_LONG_UP = 0x32,
    TRANSLATOR_KEY_SINGLE_CLICK = 0x42,
    TRANSLATOR_KEY_DOUBLE_CLICK = 0x43,
    TRANSLATOR_KEY_LONG_DOWN = 0x44,
    TRANSLATOR_KEY_LONG_UP = 0x45,
};


//2.4G：  VID 3151  PID 60AB
//ble：   VID 3151 PID  61AB
#define MICLINK_VID		0x248A
#define MICLINK_PID		0x61ab
#define MOUSE_TYPE		147


extern u8 key_code[8];
extern u8 need_pop_internet;
extern u8 need_win_R_internet;
volatile extern u8 does_pop_internet_enable;
extern u8 need_back_to_desktop;
extern u8 host_connected;
extern u8 btn_long_internet_flag;
extern bool audio_status;

extern void five_sec_win_r_internet();
extern void five_sec_pop_internet(void);
extern void back_to_desktop(void);
extern void miclink_key_handle(u8 event_new);
extern void miclink_host_cmd_handler(u8 *host_cmd);
extern void reset_idle_status();
extern void check_host_status(void);

#endif

