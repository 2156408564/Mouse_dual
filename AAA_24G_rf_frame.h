/********************************************************************************************************
 * @file     AAA_24G_rf_frame.h
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

/*
 * led_rf_frame.h
 *
 *  Created on: Jan 13, 2014
 *      Author: xuzhen
 */

#ifndef _AAA_24G_RF_FRAME_H_
#define _AAA_24G_RF_FRAME_H_

#define MOUSE_DATA_LEN_AAA  6

enum
{
    PIPE_PARING			= 0x00,
    PIPE_MOUSE			= 0x01,
    PIPE_KEYBOARD		= 0x02,
    PIPE_ALL   = 0X3F,
};

enum
{
    EMPTY_CMD			= 0x00,
    EMPTY_ACK_CMD=1,
    
    PAIR_CMD=2,
    PAIR_ACK_CMD=3,

	MOUSE_CMD=4,
	MOUSE_ACK_CMD=5,

	KB_CMD=6,
	KB_ACK_CMD=7,

	RECONNECT_CMD=8,
	RECONNECT_ACK_CMD=9,
	USER_DEFINE_CMD = 10,
	USER_DEFINE_ACK_CMD = 11,
	VOICE_CMD = 12,
	VOICE_ACK_CMD = 13,
#if D24G_OTA_ENABLE_AAA
	D24G_OTA_CMD = 0xF7,
	D24G_OTA_ACK_CMD = 0xF8,
#endif
};

typedef struct
{
    u8 btn;
#if (MOUSE_DATA_LEN_AAA==6)
    s16 x;
    s16 y;
#elif (MOUSE_DATA_LEN_AAA==4)
    s8 x;
    s8 y;
#endif
    s8 wheel;
} mouse_data_t;


typedef struct
{
	u32 did;

	u8 key[12];
}AES_KEY;

typedef struct
{
    u32 dma_len;

    u8  rf_len; 
    u8 dat[59];

} rf_packet_t;

typedef struct
{	
	u8 cmd;//data type
	u8 seq_no;
	u8 pno_no;
	
	u32 did;
	u8 key[12];

} pair_data_t;

typedef struct
{	
	u8 cmd;//data type
	u8 seq_no;
	u8 pno_no;
	
	u8 tick_0;
	u8 tick_1;
	u8 chn;
	u8 host_led_status;
	
	u32 gid;
	u32 did;
	u8 key[12];

} pair_ack_data_t;



typedef struct
{
	u8	cmd;
	u8	seq_no;
	u32 did;
	u8  km_dat[32];//mouse data or kb
} km_3_c_1_data_t;




typedef struct
{
	u8 cmd;//data type
	u8 seq_no;
	u8 data[24];
} km_ack_data_t;

#if D24G_OTA_ENABLE_AAA
#define USB_OTA_LENGTH  64
typedef struct{
	u8	report_id;
	u8 	opcode;
	u16	length;
	u8	dat[60];
}usb_data_t;


typedef struct
{
	u8	cmd;//bit7=0: no aes  =1: aes
	u8  pno_no;
	u32 did;

	u8	report_id;
	u8 	opcode;
	u16	length;	
	u8 dat[20]; //include:u16 package_cnt; u8 data[16]; u16 crc16;
} ota_data_t;

typedef struct
{
	u16 cmd;
	u8 buf[16];
	u16 crc;
}ota_data_st;


typedef struct
{
	u8	cmd;//bit7=0: no aes  =1: aes
	u8  pno_no;
	u8	report_id;
	u8 	opcode;
	u16	length;	
	u8 dat[20]; //include:u16 package_cnt; u8 data[16]; u16 crc16;
} ota_ack_data_t;
#endif




#endif /* LED_RF_FRAME_H_ */

