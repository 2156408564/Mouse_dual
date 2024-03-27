/********************************************************************************************************
 * @file     aaa_app_att.c
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
#include "../../application/audio/audio_config.h"

#define CHAR_2_LEN  5//5
#define CHAR_16_LEN 19//19





const u16 clientCharacterCfgUUID = GATT_UUID_CLIENT_CHAR_CFG;

const u16 extReportRefUUID = GATT_UUID_EXT_REPORT_REF;

const u16 reportRefUUID = GATT_UUID_REPORT_REF;

const u16 characterPresentFormatUUID = GATT_UUID_CHAR_PRESENT_FORMAT;

const u16 my_primaryServiceUUID = GATT_UUID_PRIMARY_SERVICE;

const u16 my_characterUUID = GATT_UUID_CHARACTER;

const u16 userdesc_UUID	 = GATT_UUID_CHAR_USER_DESC;

#if 0
    const u8 PROP_READ = CHAR_PROP_READ;

    const u8 PROP_WRITE = CHAR_PROP_WRITE;

    const u8 PROP_INDICATE = CHAR_PROP_INDICATE;

    const u8 PROP_WRITE_NORSP = CHAR_PROP_WRITE_WITHOUT_RSP;

    const u8 PROP_READ_NOTIFY = CHAR_PROP_READ | CHAR_PROP_NOTIFY;

    const u8 PROP_READ_WRITE_NORSP = CHAR_PROP_READ | CHAR_PROP_WRITE_WITHOUT_RSP;

    const u8 PROP_READ_WRITE_WRITENORSP = CHAR_PROP_READ | CHAR_PROP_WRITE | CHAR_PROP_WRITE_WITHOUT_RSP;

    const u8 PROP_READ_WRITE = CHAR_PROP_READ | CHAR_PROP_WRITE;

    const u8 PROP_READ_WRITE_NORSP_NOTIFY = CHAR_PROP_READ | CHAR_PROP_WRITE_WITHOUT_RSP | CHAR_PROP_NOTIFY;
#endif
u8 publicInCCC[2] = {0, 0};

//GenericAccess Service ......................................................................
const u16 my_gapServiceUUID = SERVICE_UUID_GENERIC_ACCESS;


const u16 my_devNameUUID = GATT_UUID_DEVICE_NAME;
const u8 devNameChar[5] =
{
    CHAR_PROP_READ | CHAR_PROP_WRITE,
    GenericAccess_DeviceName_DP_H, 0,
    0x00, 0x2a,
};


const u16 my_appearance = DEVICE_APPEARANCE;
const u8 appearanceChar[5] =
{
    CHAR_PROP_READ,
    GenericAccess_Appearance_DP_H, 0x00,
    0x01, 0x2a,
};

const u16 my_periConnParamUUID = 0x2a04;
gap_periConnectParams_t my_periConnParameters = {6, 6, 0x3c, 600};
const u8 ConnParametersChar[5] =
{
    CHAR_PROP_READ,
    CONN_PARAM_DP_H, 0x00,
    0x04, 0x2a,
};

//gatt Service ......................................................................

const u16 my_gattServiceUUID = SERVICE_UUID_GENERIC_ATTRIBUTE;
#if SERVICE_CHANGE_ENABLE
const u16 serviceChangeUIID = GATT_UUID_SERVICE_CHANGE;
u16 serviceChangeVal[2] = {0};
static u8 serviceChangeCCC[2] = {0, 0};
const u8 serviceChangeChar[5] =
{
    CHAR_PROP_INDICATE,
    GenericAttribute_ServiceChanged_DP_H, 0x00,
    0x05, 0x2a,
};
#endif
//device information Service ......................................................................
const u16 my_devServiceUUID = SERVICE_UUID_DEVICE_INFORMATION;

const u16 my_PnPUUID = CHARACTERISTIC_UUID_PNP_ID;
//const u8  my_PnPtrs [7] = {0x02, 0x8a, 0x24, 0x66, 0x82, 0x01, 0x00};//telink
const u8  my_PnPtrs [7] = {0x01, (MICLINK_VID & 0xff), ((MICLINK_VID >> 8) & 0xff), (MICLINK_PID & 0xff), ((MICLINK_PID >> 8) & 0xff), 0x00, 0x02};

//const u8   my_PnPtrs [] = {0x02, 0xac, 0x05, 0x20, 0x02, 0x01, 0x00};//nordic keyboard
//const u8  my_PnPtrs [] = {0x02, 0xac, 0x05, 0x2c, 0x02, 0x01, 0x00};//apple
//const u8  my_PnPtrs [] = {0x02, 0xac, 0x05, 0x4f, 0x02, 0x01, 0x00};//apple

const u8 PnPtrsChar[5] =
{
    CHAR_PROP_READ,
    DeviceInformation_pnpID_DP_H, 0x00,
    0x50, 0x2a,
};


//HID Service ......................................................................

const u16 my_hidServiceUUID	  = SERVICE_UUID_HUMAN_INTERFACE_DEVICE;
const u16 hidIncludeUUID			= GATT_UUID_INCLUDE;

// Include attribute (Battery service)
//static u16 include[3] = {BATT_PS_H, BATT_LEVEL_INPUT_CCB_H, SERVICE_UUID_BATTERY};

const u16 hidProtocolModeUUID		= CHARACTERISTIC_UUID_HID_PROTOCOL_MODE;
u8 protocolMode = DFLT_HID_PROTOCOL_MODE;
const u8 protocolModeChar[5] =
{
    CHAR_PROP_READ | CHAR_PROP_WRITE_WITHOUT_RSP,
    HID_PROTOCOL_MODE_DP_H, 0x00,
    0x4E, 0x2a,
};

const u16 hidReportUUID			= CHARACTERISTIC_UUID_HID_REPORT;


#if HID_KEYBOARD_ATT_ENABLE


const u16 hidbootKeyInReportUUID = CHARACTERISTIC_UUID_HID_BOOT_KEY_INPUT;
static u8 bootKeyInReport[8] = {0, 0};
static u8 bootKeyInReportCCC[2] = {0, 0};
const u8 bootKeyInChar[5] =
{
    CHAR_PROP_READ | CHAR_PROP_NOTIFY | CHAR_PROP_WRITE,
    HID_BOOT_KB_REPORT_INPUT_DP_H, 0x00,
    0x22, 0x2a,
};

const u16 hidbootKeyOutReportUUID = CHARACTERISTIC_UUID_HID_BOOT_KEY_OUTPUT;
static u8 bootKeyOutReport;
const u8 bootKeyOutChar[5] =
{
    CHAR_PROP_READ | CHAR_PROP_WRITE_WITHOUT_RSP | CHAR_PROP_WRITE,
    HID_BOOT_KB_REPORT_OUTPUT_DP_H, 0x00,
    0x32, 0x2a,
};

static u8 reportConsumerControlIn[2];
static u8 reportConsumerControlInCCC[2] = {0, 0};
static u8 reportRefConsumerControlIn[2] = { HID_REPORT_ID_CONSUME_CONTROL_INPUT, HID_REPORT_TYPE_INPUT };
const u8 reportConsumerChar[5] =
{
    CHAR_PROP_READ | CHAR_PROP_NOTIFY | CHAR_PROP_WRITE,
    HID_CONSUME_REPORT_INPUT_DP_H, 0x00,
    0x4D, 0x2a,
};

u8 reportKeyIn[8];
u8 reportKeyInCCC[2] = {0, 0};
static u8 reportRefKeyIn[2] = {HID_REPORT_ID_KEYBOARD_INPUT, HID_REPORT_TYPE_INPUT };
const u8 reportKeyInChar[5] =
{
    CHAR_PROP_READ | CHAR_PROP_NOTIFY | CHAR_PROP_WRITE,
    HID_NORMAL_KB_REPORT_INPUT_DP_H, 0x00,
    0x4D, 0x2a,
};

u8 reportKeyOut[1];
static u8 reportRefKeyOut[2] =
{ HID_REPORT_ID_KEYBOARD_INPUT, HID_REPORT_TYPE_OUTPUT };
const u8 reportKeyOutChar[5] =
{
    CHAR_PROP_READ | CHAR_PROP_WRITE_WITHOUT_RSP | CHAR_PROP_WRITE,
    HID_NORMAL_KB_REPORT_OUTPUT_DP_H, 0x00,
    0x4D, 0x2a,
};
#endif

#if HID_MOUSE_ATT_ENABLE
static u8 reportMouseIn[MOUSE_DATA_LEN_AAA];
static u8 reportMouseInCCC[2] = {0, 0};
static u8 reportRefMouseIn[2] = {REPORT_ID_MOUSE_INPUT_AAA, HID_REPORT_TYPE_INPUT };
const u8 reportMouseInChar[5] =
{
    CHAR_PROP_READ | CHAR_PROP_NOTIFY | CHAR_PROP_WRITE,
    HID_MOUSE_REPORT_INPUT_DP_H, 0x00,
    0x4D, 0x2a,
};

const u16 hidbootMouseInReportUUID = CHARACTERISTIC_UUID_HID_BOOT_MOUSE_INPUT;

static u8 bootMouseInReport;
static u8 bootMouseInReportCCC[2] = {0, 0};
const u8 bootMouseInChar[5] =
{
    CHAR_PROP_READ | CHAR_PROP_NOTIFY | CHAR_PROP_WRITE,
    HID_BOOT_MOUSE_REPORT_INPUT_DP_H, 0x00,
    0x33, 0x2a,
};

#endif

#if 0
static u8 reportUseIn[4];
static u8 reportUseInCCC[2] = {0, 0};
static u8 reportRefUseIn[2] = {REPORT_ID_USER_FEATURE_AAA, HID_REPORT_TYPE_FEATURE};
const u8 reportUseInChar[5] =
{
    CHAR_PROP_READ | CHAR_PROP_NOTIFY | CHAR_PROP_WRITE,
    HID_USER_REPORT_INPUT_DP_H, 0x00,
    0x4D, 0x2a,
};
#endif

const u16 hidReportMapUUID 		= CHARACTERISTIC_UUID_HID_REPORT_MAP;
const u8 hidReportMapChar[5] =
{
    CHAR_PROP_READ,
    HID_REPORT_MAP_DP_H, 0x00,
    0x4B, 0x2a,
};

//static u16 extServiceUUID;

const u16 hidinformationUUID		= CHARACTERISTIC_UUID_HID_INFORMATION;
const u8 hidInformation[] =
{
    0x11, 0x01,			   // bcdHID (USB HID version)
    0x00,									   // bCountryCode
    0x01 									   // Flags
};

const u8 hidinformationChar[5] =
{
    CHAR_PROP_READ,
    HID_INFORMATION_DP_H, 0x00,
    0x4A, 0x2a,
};

const u16 hidCtrlPointUUID 		= CHARACTERISTIC_UUID_HID_CONTROL_POINT;
static u8 controlPoint;
const u8 hidCtrlPointChar[5] =
{
    CHAR_PROP_WRITE_WITHOUT_RSP,
    HID_CONTROL_POINT_DP_H, 0x00,
    0x4C, 0x2a,
};


static const u8 reportMap[] =
{

#if HID_KEYBOARD_ATT_ENABLE
    //keyboard report in
    0x05, 0x01,	 // Usage Pg (Generic Desktop)
    0x09, 0x06,	 // Usage (Keyboard)
    0xA1, 0x01,	 // Collection: (Application)
    0x85, HID_REPORT_ID_KEYBOARD_INPUT,	 // Report Id (keyboard)
    //
    0x05, 0x07,	 // Usage Pg (Key Codes)
    0x19, 0xE0,	 // Usage Min (224)  VK_CTRL:0xe0
    0x29, 0xE7,	 // Usage Max (231)  VK_RWIN:0xe7
    0x15, 0x00,	 // Log Min (0)
    0x25, 0x01,	 // Log Max (1)
    //
    // Modifier byte
    0x75, 0x01,	 // Report Size (1)   1 bit * 8
    0x95, 0x08,	 // Report Count (8)
    0x81, 0x02,	 // Input: (Data, Variable, Absolute)
    //
    // Reserved byte
    0x95, 0x01,	 // Report Count (1)
    0x75, 0x08,	 // Report Size (8)
    0x81, 0x01,	 // Input: (Constant)

    //keyboard output
    //5 bit led ctrl: NumLock CapsLock ScrollLock Compose kana
    0x95, 0x05,	//Report Count (5)
    0x75, 0x01,	//Report Size (1)
    0x05, 0x08,	//Usage Pg (LEDs )
    0x19, 0x01,	//Usage Min
    0x29, 0x05,	//Usage Max
    0x91, 0x02,	//Output (Data, Variable, Absolute)
    //3 bit reserved
    0x95, 0x01,	//Report Count (1)
    0x75, 0x03,	//Report Size (3)
    0x91, 0x01,	//Output (Constant)

    // Key arrays (6 bytes)
    0x95, 0x06,	 // Report Count (6)
    0x75, 0x08,	 // Report Size (8)
    0x15, 0x00,	 // Log Min (0)
    0x25, 0xF1,	 // Log Max (241)
    0x05, 0x07,	 // Usage Pg (Key Codes)
    0x19, 0x00,	 // Usage Min (0)
    0x29, 0xf1,	 // Usage Max (241)
    0x81, 0x00,	 // Input: (Data, Array)

    0xC0,			  // End Collection

    //consumer report in
    0x05, 0x0C,   // Usage Page (Consumer)
    0x09, 0x01,   // Usage (Consumer Control)
    0xA1, 0x01,   // Collection (Application)
    0x85, HID_REPORT_ID_CONSUME_CONTROL_INPUT,   //	 Report Id
    0x75, 0x10, 	//global, report size 16 bits
    0x95, 0x01, 	//global, report count 1
    0x15, 0x01, 	//global, min  0x01
    0x26, 0x8c, 0x02, //global, max  0x28c
    0x19, 0x01, 	//local, min   0x01
    0x2a, 0x8c, 0x02, //local, max	  0x28c
    0x81, 0x00, 	//main,  input data varible, absolute
    0xc0,		  //main, end collection
#endif

#if HID_MOUSE_ATT_ENABLE
			//mouse report in
	0x05, 0x01,  // Usage Page (Generic Desktop)
	0x09, 0x02,  // Usage (Mouse)
	0xA1, 0x01,  // Collection (Application)
	0x85, REPORT_ID_MOUSE_INPUT_AAA,  // Report Id
	0x09, 0x01,  //   Usage (Pointer)
	0xA1, 0x00,  //   Collection (Physical)
	0x05, 0x09,  //  Usage Page (Buttons)
	0x19, 0x01,  //  Usage Minimum (01) - Button 1
	0x29, 0x03,  //  Usage Maximum (03) - Button 3
	0x15, 0x00,  //  Logical Minimum (0)
	0x25, 0x01,  //  Logical Maximum (1)
	0x75, 0x01,  //  Report Size (1)
	0x95, 0x05,  //  Report Count (3)
	0x81, 0x02,  //  Input (Data, Variable, Absolute) - Button states
	0x75, 0x03,  //  Report Size (5)
	0x95, 0x01,  //  Report Count (1)
	0x81, 0x01,  //  Input (Constant) - Padding or Reserved bits

	0x05, 0x01, 	 //  Usage Page (Generic Desktop Control)
	0x09, 0x30,  // Usage (X)
	0x09, 0x31,  // Usage (Y)

#if (MOUSE_DATA_LEN_AAA==4)
	0x15, 0x81, //	LOGICAL_MINIMUM(0)
	0x25, 0x7f,
	0x75, 0x08, //	Report Size (8)
	0x95, 0x02, //	Report Count (2)
	0x81, 0x06, //	Input (Data, Variable, Relative)
#elif (MOUSE_DATA_LEN_AAA==6)
	0x16, 0x01, 0x80, //  LOGICAL_MINIMUM(0)
	0x26, 0xff, 0x7f,
	0x75, 0x10, //	Report Size (16)
	0x95, 0x02, //	Report Count (2)
	0x81, 0x06, //	Input (Data, Variable, Relative)

#endif
			//0x05,0x01,			 //  Usage Page (Generic Desktop Control)
	0x09, 0x38, 		 //  Usage (Wheel)
	0x15, 0x81, 		 //  Logical Minimum (-4)
	0x25, 0x7F, 		 //  Logical Maximum (3)
	0x75, 0x08, 		 //  Report Size (3)
	0x95, 0x01, 		 //  Report Count (1)
	0x81, 0x06, 		 //  Input (Data, Variable, Relative)

	0xC0,		  //   End Collection
	0xC0,		  // End Collection
#endif

#if 0
	0x06, 0x01, 0xFF,
	0x09, 0x01,
	0xA1, 0x01,
	0x85, REPORT_ID_USER_FEATURE_AAA,
	0x09, 0x05,
	0x15, 0x00,
	0x26, 0xFF, 0x00,
	0x75, 0x08,
	0x95, 0x04,
	0xB1, 0x02,
	0xC0,
#endif

	0x06, 0x91, 0xFF,  // Usage Page (Vendor Defined 0xFF91)
	0x09, 0x00, 	   // Usage (0x00)
	0xA1, 0x91, 	   // Collection (Vendor Defined 0x91)
	0x15, 0x00, 	   //	Logical Minimum (0)
	0x25, 0x7F, 	   //	Logical Maximum (127)
	0x75, 0x08, 	   //	Report Size (8)
	0x85, 0x0C, 	   //	Report ID (12)
	0x95, 0x14, 	   //	Report Count (20)
	0x09, 0x00, 	   //	Usage (0x00)
	0x81, 0x00, 	   //	Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0x95, 0x14, 	   //	Report Count (20)
	0x09, 0x00, 	   //	Usage (0x00)
	0x91, 0x02, 	  //   Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x85, 0x1B, 	   //	Report ID (27)
	//if use 128, the mac os bt will disconnect, so we use 8k reat in ble mode
	0x95, ADPCM_PACKET_LEN, 	   //	Report Count (64)
	0x09, 0x00, 	   //	Usage (0x00)
	0x81, 0x00, 	   //	Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0x95, 0x04, 	   //	Report Count (4)
	0x09, 0x00, 	   //	Usage (0x00)
	0x91, 0x02, 	   //	Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0xC0,			   // End Collection
};

//Battery Service ......................................................................

const u16 my_batServiceUUID				 = SERVICE_UUID_BATTERY;

const u16 my_batCharUUID					 = CHARACTERISTIC_UUID_BATTERY_LEVEL;
static u8 batteryValueInCCC[2] = {0, 0};
u8 	   my_batVal[1] 					 = {99};
const u8 batteryChar[5] =
{
    CHAR_PROP_READ | CHAR_PROP_NOTIFY,
    BATT_LEVEL_INPUT_DP_H, 0x00,
    0x19, 0x2a,
};
//ota Service ......................................................................
static const  u8 my_OtaUUID[16]					    = WRAPPING_BRACES(TELINK_SPP_DATA_OTA);
static const  u8 my_OtaServiceUUID[16]				= WRAPPING_BRACES(TELINK_OTA_UUID_SERVICE);
static u8 my_OtaData 						        = 0x00;
static u8 otaDataCCC[2] 							= {0,0};

//static const u8  my_MicName[] = {'M', 'i', 'c'};
//static const u8  my_SpeakerName[] = {'S', 'p', 'e', 'a', 'k', 'e', 'r'};
static const u8  my_OtaName[] = {'O', 'T', 'A'};
//// OTA attribute values
static const u8 my_OtaCharVal[19] = {
	CHAR_PROP_READ | CHAR_PROP_WRITE_WITHOUT_RSP | CHAR_PROP_NOTIFY,
	U16_LO(OTA_CMD_OUT_DP_H), U16_HI(OTA_CMD_OUT_DP_H),
	TELINK_SPP_DATA_OTA,
};

#if 0
const u8 my_OtaServiceUUID[16] 	 = {0x12,0x19,0x0d,0x0c,0x0b,0x0a,0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00};
u8 	 my_OtaData 	 = 0x00;

const u8 OtaDataChar[19] =
{
    CHAR_PROP_READ | CHAR_PROP_WRITE_WITHOUT_RSP,
    OTA_CMD_OUT_DP_H, 0x00,
    0x12, 0x2B, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00,
};

const u8 my_DevInfUUID[16] = {0x13, 0x2B, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00};
const u8 DevInfAaaChar[19] =
{
    CHAR_PROP_READ,
    OTA_CMD_INF_DP_H, 0x00,
    0x14, 0x2B, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00,
};
const u8 IcInfChar[19] =
{
    CHAR_PROP_READ| CHAR_PROP_WRITE_WITHOUT_RSP|CHAR_PROP_NOTIFY,
    OTA_IC_INF_DP_H, 0x00,
   0x15, 0x2B, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00,
};
#endif
//---------------------------------------------------
#if BLE_AUDIO_ENABLE
extern u32 connect_begin_tick;

const u8 my_AudioUUID[16]	 = {TELINK_AUDIO_UUID_SERVICE};

//const u8 my_MicUUID[16]	 = TELINK_MIC_DATA;
u8 	 my_MicData 	 = 0x80;
static u8 micDataCCC[2] = {0, 0};
const u8  my_MicName[] = {'M', 'i', 'c'};
const u8 MicChar[19] =
{
    CHAR_PROP_READ | CHAR_PROP_NOTIFY,
    AUDIO_MIC_INPUT_DP_H, 0x00,
    0x18, 0x2B, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00
    //TELINK_MIC_DATA// is error
};


const u8  my_SpeakerUUID[16]	 = {TELINK_SPEAKER_DATA};
u8 	   my_SpeakerData	 = 0x81;
const u8  my_SpeakerName[] = {'S', 'p', 'e', 'a', 'k', 'e', 'r'};
const u8 SpeakerChar[19] =
{
    CHAR_PROP_READ | CHAR_PROP_WRITE_WITHOUT_RSP,
    AUDIO_SPEAKER_OUT_DP_H, 0x00,
    0x19, 0x2B, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00

};

int speckWrite(void *p)
{
    #if 0
    u8 cmd_data[20];

	rf_packet_att_data_t *pw = (rf_packet_att_data_t *)p;
    int len = pw->l2cap - 3;
    printf("speckWrite\r\n");

    if (len > 20)
         return 0;
    memcpy(cmd_data, &pw->dat[0], len);
    for (int i = 0; i < len; i++) {
        ///cmd_data[i] = pw->dat[i];
        printf("%x ", pw->dat[i]);
    }

    miclink_host_cmd_handler(cmd_data);
    #endif
	return 0;
}

#endif

/////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////hid cmd report in////////////////////////////////////////////
extern u32 connect_begin_tick;
int cmd_in_cccWrite(void *p)
{
	rf_packet_att_write_t *pkt_cmd = (rf_packet_att_write_t *)p;
	printf("cmd_in_cccWrite->value = %d \r\n",pkt_cmd->value);
    my_fifo_reset (&fifo_km);
	connect_ok_flag = 1;
    connect_begin_tick = clock_time() | 1;
    return 0;
}

static const u8 my_hidReportCmd_inCharVal[5] = {
    CHAR_PROP_READ | CHAR_PROP_NOTIFY,
    U16_LO(HID_ML_REPORT_INPUT_CMD_DP_H), U16_HI(HID_ML_REPORT_INPUT_CMD_DP_H),
    U16_LO(CHARACTERISTIC_UUID_HID_REPORT), U16_HI(CHARACTERISTIC_UUID_HID_REPORT)
};

static u8 CmdHandle[1];
static u8 CmdCCC[2];
static u8 CmdReport[2] = {HID_REPORT_ID_CMD_ML_INPUT, HID_REPORT_TYPE_INPUT};

///////////////////////////////////hid cmd report out////////////////////////////////////////////
static const u8 my_hidReportCmd_outCharVal[5] = {
    CHAR_PROP_READ | CHAR_PROP_WRITE_WITHOUT_RSP | CHAR_PROP_WRITE,
    U16_LO(HID_ML_REPORT_OUTPUT_CMD_DP_H), U16_HI(HID_ML_REPORT_OUTPUT_CMD_DP_H),
    U16_LO(CHARACTERISTIC_UUID_HID_REPORT), U16_HI(CHARACTERISTIC_UUID_HID_REPORT)
};
static u8 reportRefCmdOut[2] =
{ HID_REPORT_ID_CMD_ML_OUTPUT, HID_REPORT_TYPE_OUTPUT };

u8 reportCmdOut[1];
int cmd_write_call_back(void *p)
{
    u8 cmd_data[20];

	rf_packet_att_data_t *pw = (rf_packet_att_data_t *)p;
    int len = pw->l2cap - 3;
    //printf("cmd_write_call_back\r\n");

    if (len > 20)
         return 0;
    memcpy(cmd_data, &pw->dat[0], len);
    for (int i = 0; i < len; i++) {
        ///cmd_data[i] = pw->dat[i];
        printf("%x ", pw->dat[i]);
    }

    miclink_host_cmd_handler(cmd_data);

#if 0
    int status = blc_gatt_pushHandleValueNotify(BLS_CONN_HANDLE, HID_ML_REPORT_INPUT_CMD_DP_H, test_data, sizeof(test_data));
    if (status != BLE_SUCCESS)
    {
        printf("notify_fail_reason=%x\r\n", status);
    }
#endif
	return 0;
}

////////////////////////////////////hid audio report in////////////////////////////////////////////
int audio_in_cccWrite(void *p)
{
	rf_packet_att_write_t *pkt_cmd = (rf_packet_att_write_t *)p;
	printf("audio_in_cccWrite->value = %d \r\n",pkt_cmd->value);
    my_fifo_reset (&fifo_km);
    connect_begin_tick = clock_time() | 1;
    return 0;
}

static const u8 my_hidReportAudio_inCharVal[5] = {
    CHAR_PROP_READ | CHAR_PROP_NOTIFY,
    U16_LO(HID_ML_REPORT_INPUT_AUDIO_DP_H), U16_HI(HID_ML_REPORT_INPUT_AUDIO_DP_H),
    U16_LO(CHARACTERISTIC_UUID_HID_REPORT), U16_HI(CHARACTERISTIC_UUID_HID_REPORT)
};

static u8 AudioHandle[1];
static u8 AudioCCC[2];
static u8 AudioReport[2] = {HID_REPORT_ID_AUDIO_ML_INPUT, HID_REPORT_TYPE_INPUT};

///////////////////////////////////hid audio report out////////////////////////////////////////////
static const u8 my_hidReportAudio_outCharVal[5] = {
    CHAR_PROP_READ | CHAR_PROP_WRITE_WITHOUT_RSP | CHAR_PROP_WRITE,
    U16_LO(HID_ML_REPORT_OUTPUT_AUDIO_DP_H), U16_HI(HID_ML_REPORT_OUTPUT_AUDIO_DP_H),
    U16_LO(CHARACTERISTIC_UUID_HID_REPORT), U16_HI(CHARACTERISTIC_UUID_HID_REPORT)
};
static u8 reportRefAudioOut[2] =
{ HID_REPORT_ID_AUDIO_ML_OUTPUT, HID_REPORT_TYPE_OUTPUT };

u8 reportAudioOut[1];

int audio_write_call_back(void *p)
{
	rf_packet_att_data_t *pw = (rf_packet_att_data_t *)p;
    int len = pw->l2cap - 3;
    printf("audio_write_call_back\r\n");
    for (int i = 0; i < len; i++) {
        printf("%x ", pw->dat[i]);
    }

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////

const attribute_t my_Attributes[] =
{

    {ATT_END_H - 1, 0, 0, 0, 0, 0}, // total num of attribute


    //..................................gap Service.........................................................................................................
    {7, ATT_PERMISSIONS_READ, 2, 2, (u8 *)(&my_primaryServiceUUID),	(u8 *)(&my_gapServiceUUID), 0},

    {0, ATT_PERMISSIONS_READ, 2, CHAR_2_LEN, (u8 *)(&my_characterUUID), (u8 *)(&devNameChar[0]), 0},
    {0, ATT_PERMISSIONS_RDWR, 2, (sizeof(DEVICE_NAME_AAA) - 1), (u8 *)(&devNameChar[3]), (u8 *)(/*user_cfg.device_name*/DEVICE_NAME_AAA), 0},

    {0, ATT_PERMISSIONS_READ, 2, CHAR_2_LEN, (u8 *)(&my_characterUUID), (u8 *)(&appearanceChar[0]), 0},
    {0, ATT_PERMISSIONS_READ, 2, 2, (u8 *)(&appearanceChar[3]), (u8 *)(&my_appearance), 0},

    {0, ATT_PERMISSIONS_READ, 2, CHAR_2_LEN, (u8 *)(&my_characterUUID), (u8 *)(&ConnParametersChar[0]), 0},
    {0, ATT_PERMISSIONS_READ, 2, 8, (u8 *)(&ConnParametersChar[3]), (u8 *)(&my_periConnParameters), 0},


    //..................................gatt Service...................................................................................................................................
    {1, ATT_PERMISSIONS_READ, 2, 2, (u8 *)(&my_primaryServiceUUID), (u8 *)(&my_gattServiceUUID), 0},

    // {0,ATT_PERMISSIONS_READ,2,CHAR_2_LEN,(u8*)(&my_characterUUID), 	 (u8*)(&serviceChangeChar[0]), 0},
    // {0,ATT_PERMISSIONS_READ,2,sizeof (serviceChangeVal), (u8*)(&serviceChangeChar[3]),  (u8*)(&serviceChangeVal), 0},
    // {0,ATT_PERMISSIONS_RDWR,2,sizeof (serviceChangeCCC),(u8*)(&clientCharacterCfgUUID), (u8*)(serviceChangeCCC), 0},


    //..................................device Information Service.........................................................................................................
    {3, ATT_PERMISSIONS_READ, 2, 2, (u8 *)(&my_primaryServiceUUID), (u8 *)(&my_devServiceUUID), 0},

    {0, ATT_PERMISSIONS_READ, 2, CHAR_2_LEN, (u8 *)(&my_characterUUID), (u8 *)(&PnPtrsChar[0]), 0},
    {0, ATT_PERMISSIONS_READ, 2, 7, (u8 *)(&PnPtrsChar[3]),	(u8 *)(my_PnPtrs), 0},

    //.............................. Battery Service..................................................................................................
    {4, ATT_PERMISSIONS_READ, 2, 2, (u8 *)(&my_primaryServiceUUID), (u8 *)(&my_batServiceUUID), 0},

    {0, ATT_PERMISSIONS_READ, 2, CHAR_2_LEN, (u8 *)(&my_characterUUID), (u8 *)(&batteryChar[0]), 0},			 //prop
    {0, ATT_PERMISSIONS_READ, 2, 1, (u8 *)(&batteryChar[3]), (u8 *)(my_batVal), 0},	    //value
    {0, ATT_PERMISSIONS_RDWR, 2, 2, (u8 *)(&clientCharacterCfgUUID), (u8 *)(batteryValueInCCC), 0},	   //value

    //..................... OTA ......................................................................................................................................
#if BLE_OTA_ENABLE_AAA
    #if 0
    {7, ATT_PERMISSIONS_READ, 2, 16, (u8 *)(&my_primaryServiceUUID), (u8 *)(&my_OtaServiceUUID), 0},

    {0, ATT_PERMISSIONS_READ, 2, CHAR_16_LEN, (u8 *)(&my_characterUUID), (u8 *)(&OtaDataChar[0]), 0},			 //prop
    {0, ATT_PERMISSIONS_RDWR, 16, sizeof(my_OtaData), (u8 *)(&OtaDataChar[3]), (&my_OtaData), &otaWrite, NULL}, 		 //value
    //{0,ATT_PERMISSIONS_READ, 2,sizeof (my_OtaName),(u8*)(&userdesc_UUID), (u8*)(my_OtaName), 0},

	{0, ATT_PERMISSIONS_READ, 2, CHAR_16_LEN, (u8 *)(&my_characterUUID), (u8 *)(&DevInfAaaChar[0]), 0},
    {0, ATT_PERMISSIONS_READ, 16, sizeof(OUTPUT_DEV_INFO_AAA), (u8 *)(&DevInfAaaChar[3]), (u8 *)(&output_dev_info.bin_crc), 0},
		
    {0, ATT_PERMISSIONS_READ, 2, CHAR_16_LEN, (u8 *)(&my_characterUUID), (u8 *)(&IcInfChar[0]), 0},
    {0, ATT_PERMISSIONS_RDWR, 16, 20, (u8 *)(&IcInfChar[3]), (u8 *)(&ic_inf[0]),0},
	{0, ATT_PERMISSIONS_RDWR, 2, 2, (u8 *)(&clientCharacterCfgUUID), (u8 *)(publicInCCC), &cccWrite}, //value
	#endif
	{5,ATT_PERMISSIONS_READ, 2,16,(u8*)(&my_primaryServiceUUID), 	(u8*)(&my_OtaServiceUUID), 0},
	{0,ATT_PERMISSIONS_READ, 2, sizeof(my_OtaCharVal),(u8*)(&my_characterUUID), (u8*)(my_OtaCharVal), 0},				//prop
	{0,ATT_PERMISSIONS_RDWR,16,sizeof(my_OtaData),(u8*)(&my_OtaUUID),	(&my_OtaData), &otaWrite, NULL},				//value
	{0,ATT_PERMISSIONS_RDWR,2,sizeof(otaDataCCC),(u8*)(&clientCharacterCfgUUID), 	(u8*)(otaDataCCC), 0},				//value
	{0,ATT_PERMISSIONS_READ, 2,sizeof (my_OtaName),(u8*)(&userdesc_UUID), (u8*)(my_OtaName), 0},
#endif
    //..................... audio ......................................................................................................................................
#if (BLE_AUDIO_ENABLE)

    {6, ATT_PERMISSIONS_READ, 2, 16, (u8 *)(&my_primaryServiceUUID), (u8 *)(&my_AudioUUID), 0},

    // 0033 - 0036	MIC
    {0, ATT_PERMISSIONS_READ, 2, CHAR_16_LEN, (u8 *)(&my_characterUUID), (u8 *)(&MicChar[0]), 0},			 //prop
    {0, ATT_PERMISSIONS_READ, 16, sizeof(my_MicData), (u8 *)(&MicChar[3]), (u8 *)(&my_MicData), 0}, //value
    {0, ATT_PERMISSIONS_RDWR, 2, sizeof(micDataCCC), (u8 *)(&clientCharacterCfgUUID), (u8 *)(micDataCCC), 0}, //value
    //{0,ATT_PERMISSIONS_RDWR,2,sizeof (my_MicName),(u8*)(&userdesc_UUID), (u8*)(my_MicName), 0},

    // 0037 - 0039	SPEAKER
    {0, ATT_PERMISSIONS_READ, 2, CHAR_16_LEN, (u8 *)(&my_characterUUID), (u8 *)(&SpeakerChar[0]), 0},			 //prop
    {0, ATT_PERMISSIONS_RDWR, 16, sizeof(my_SpeakerData), (u8 *)(&SpeakerChar[3]), (u8 *)(&my_SpeakerData), &speckWrite}, //value
    //{0,ATT_PERMISSIONS_RDWR,2,sizeof (my_SpeakerName),(u8*)(&userdesc_UUID), (u8*)(my_SpeakerName), 0},
#endif


    // 4. HID Service ...................................................................................

    {HID_CONTROL_POINT_DP_H - HID_PS_H + 1, ATT_PERMISSIONS_READ, 2, 2, (u8 *)(&my_primaryServiceUUID), (u8 *)(&my_hidServiceUUID), 0},

    // 0010  include battery service
    //{0,ATT_PERMISSIONS_READ,2,sizeof(include),(u8*)(&hidIncludeUUID),	 (u8*)(include), 0},

    // 0011 - 0012  protocol mode
    {0, ATT_PERMISSIONS_READ, 2, CHAR_2_LEN, (u8 *)(&my_characterUUID), (u8 *)(&protocolModeChar[0]), 0},				 //prop
    {0, ATT_PERMISSIONS_RDWR, 2, 1, (u8 *)(&protocolModeChar[3]), (u8 *)(&protocolMode), 0},	 //value

#if HID_KEYBOARD_ATT_ENABLE

    // 0013 - 0015  boot keyboard input report (char-val-client)
    {0, ATT_PERMISSIONS_READ, 2, CHAR_2_LEN, (u8 *)(&my_characterUUID), (u8 *)(&bootKeyInChar[0]), 0},				 //prop
    {0, ATT_PERMISSIONS_RDWR, 2, sizeof(bootKeyInReport), (u8 *)(&bootKeyInChar[3]), (u8 *)(&bootKeyInReport), 0},	 //value
    {0, ATT_PERMISSIONS_RDWR, 2, 2, (u8 *)(&clientCharacterCfgUUID), (u8 *)(bootKeyInReportCCC), &cccWrite}, //value

    // 0016 - 0017   boot keyboard output report (char-val)
    {0, ATT_PERMISSIONS_READ, 2, CHAR_2_LEN, (u8 *)(&my_characterUUID), (u8 *)(&bootKeyOutChar[0]), 0}, 			 //prop
    {0, ATT_PERMISSIONS_RDWR, 2, sizeof(bootKeyOutReport), (u8 *)(&bootKeyOutChar[3]), (u8 *)(&bootKeyOutReport), 0},	 //value


    // 0018 - 001b. consume report in: 4 (char-val-client-ref)
    {0, ATT_PERMISSIONS_READ, 2, CHAR_2_LEN, (u8 *)(&my_characterUUID), (u8 *)(&reportConsumerChar[0]), 0}, 			 //prop
    {0, ATT_PERMISSIONS_RDWR, 2, sizeof(reportConsumerControlIn), (u8 *)(&reportConsumerChar[3]), (u8 *)(reportConsumerControlIn), 0}, //value
    {0, ATT_PERMISSIONS_RDWR, 2, 2, (u8 *)(&clientCharacterCfgUUID), (u8 *)(reportConsumerControlInCCC), &cccWrite}, //value
    {0, ATT_PERMISSIONS_READ, 2, 2, (u8 *)(&reportRefUUID), (u8 *)(reportRefConsumerControlIn), 0}, //value

    // 001c - 001f . keyboard report in : 4 (char-val-client-ref)
    {0, ATT_PERMISSIONS_READ, 2, CHAR_2_LEN, (u8 *)(&my_characterUUID), (u8 *)(&reportKeyInChar[0]), 0},			 //prop
    {0, ATT_PERMISSIONS_RDWR, 2, sizeof(reportKeyIn), (u8 *)(&reportKeyInChar[3]), (u8 *)(reportKeyIn), 0}, //value
    {0, ATT_PERMISSIONS_RDWR, 2, sizeof(reportKeyInCCC), (u8 *)(&clientCharacterCfgUUID), (u8 *)(reportKeyInCCC), &cccWrite}, //value
    {0, ATT_PERMISSIONS_READ, 2, sizeof(reportRefKeyIn), (u8 *)(&reportRefUUID), (u8 *)(reportRefKeyIn), 0}, //value

    // 0020 - 0022 . keyboard report out: 3 (char-val-ref)
    {0, ATT_PERMISSIONS_READ, 2, CHAR_2_LEN, (u8 *)(&my_characterUUID), (u8 *)(&reportKeyOutChar[0]), 0},				 //prop
    {0, ATT_PERMISSIONS_RDWR, 2, 1, (u8 *)(&reportKeyOutChar[3]), (u8 *)(reportKeyOut), 0},	   //value
    {0, ATT_PERMISSIONS_READ, 2, 2, (u8 *)(&reportRefUUID), (u8 *)(reportRefKeyOut), 0},      //value
#endif

#if HID_MOUSE_ATT_ENABLE
    // 0013 - 0015  boot keyboard input report (char-val-client)
    {0, ATT_PERMISSIONS_READ, 2, CHAR_2_LEN, (u8 *)(&my_characterUUID), (u8 *)(&bootMouseInChar[0]), 0},			 //prop
    {0, ATT_PERMISSIONS_READ, 2, sizeof(bootMouseInReport), (u8 *)(&bootMouseInChar[3]), (u8 *)(&bootMouseInReport), 0}, //value
    {0, ATT_PERMISSIONS_RDWR, 2, 2, (u8 *)(&clientCharacterCfgUUID), (u8 *)(bootMouseInReportCCC), 0},	 //value

    // 001c - 001f . keyboard report in : 4 (char-val-client-ref)
    {0, ATT_PERMISSIONS_READ, 2, CHAR_2_LEN, (u8 *)(&my_characterUUID), (u8 *)(&reportMouseInChar[0]), 0},				 //prop
    {0, ATT_PERMISSIONS_RDWR, 2, sizeof(reportMouseIn), (u8 *)(&reportMouseInChar[3]), (u8 *)(reportMouseIn), 0},	 //value
    {0, ATT_PERMISSIONS_RDWR, 2, 2, (u8 *)(&clientCharacterCfgUUID), (u8 *)(reportMouseInCCC), &cccWrite},	 //value
    {0, ATT_PERMISSIONS_READ, 2, 2, (u8 *)(&reportRefUUID), (u8 *)(reportRefMouseIn), 0},	 //value
#endif

	// 001c - 001f . user define ml cmd report in : 4 (char-val-client-ref)
	{0,ATT_PERMISSIONS_READ,2,sizeof(my_hidReportCmd_inCharVal),(u8*)(&my_characterUUID), (u8*)(my_hidReportCmd_inCharVal), 0},				//prop
	{0,ATT_PERMISSIONS_READ,2, sizeof(CmdHandle),(u8*)(&hidReportUUID),(u8*)(CmdHandle), 0},	//value  handle 36
	{0,ATT_PERMISSIONS_RDWR,2,sizeof(CmdCCC),(u8*)(&clientCharacterCfgUUID), (u8*)(CmdCCC), &cmd_in_cccWrite},	//value
	{0,ATT_PERMISSIONS_RDWR,2,sizeof(CmdReport),(u8*)(&reportRefUUID), (u8*)(CmdReport), 0},	    //value

	// 0020 - 0022 . user define ml cmd report out: 3 (char-val-ref)
	{0,ATT_PERMISSIONS_READ,2,sizeof(my_hidReportCmd_outCharVal),(u8*)(&my_characterUUID), (u8*)(my_hidReportCmd_outCharVal), 0},				//prop
	{0,ATT_PERMISSIONS_RDWR,2,sizeof(reportCmdOut),(u8*)(&hidReportUUID), 	(u8*)(reportCmdOut), &cmd_write_call_back},	//value
	{0,ATT_PERMISSIONS_RDWR,2,sizeof(reportRefCmdOut),(u8*)(&reportRefUUID), 	(u8*)(reportRefCmdOut), 0},	//value//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

   	// 001c - 001f . user define ml audio report in : 4 (char-val-client-ref)
	{0,ATT_PERMISSIONS_READ,2,sizeof(my_hidReportAudio_inCharVal),(u8*)(&my_characterUUID), (u8*)(my_hidReportAudio_inCharVal), 0},				//prop
	{0,ATT_PERMISSIONS_READ,2, sizeof(AudioHandle),(u8*)(&hidReportUUID),(u8*)(AudioHandle), 0},	//value  handle 36
	{0,ATT_PERMISSIONS_RDWR,2,sizeof(AudioCCC),(u8*)(&clientCharacterCfgUUID), (u8*)(AudioCCC), &audio_in_cccWrite},	//value
	{0,ATT_PERMISSIONS_RDWR,2,sizeof(AudioReport),(u8*)(&reportRefUUID), (u8*)(AudioReport), 0},	    //value

	// 0020 - 0022 . user define ml audio report out: 3 (char-val-ref)
	{0,ATT_PERMISSIONS_READ,2,sizeof(my_hidReportAudio_outCharVal),(u8*)(&my_characterUUID), (u8*)(my_hidReportAudio_outCharVal), 0},				//prop
	{0,ATT_PERMISSIONS_RDWR,2,sizeof(reportAudioOut),(u8*)(&hidReportUUID), 	(u8*)(reportAudioOut), &audio_write_call_back},	//value
	{0,ATT_PERMISSIONS_RDWR,2,sizeof(reportRefAudioOut),(u8*)(&reportRefUUID), 	(u8*)(reportRefAudioOut), 0},	//value
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if 0
    {0, ATT_PERMISSIONS_READ, 2, CHAR_2_LEN, (u8 *)(&my_characterUUID), (u8 *)(&reportUseInChar[0]), 0},				//prop
    {0, ATT_PERMISSIONS_RDWR, 2, sizeof(reportUseIn), (u8 *)(&reportUseInChar[3]), (u8 *)(reportUseIn), 0},	//value
    {0, ATT_PERMISSIONS_RDWR, 2, 2, (u8 *)(&clientCharacterCfgUUID), (u8 *)(reportUseInCCC), &cccWrite},	//value
    {0, ATT_PERMISSIONS_READ, 2, 2, (u8 *)(&reportRefUUID), (u8 *)(reportRefUseIn), 0},	//value
#endif

    // 0023 - 0025 . report map: 3
    {0, ATT_PERMISSIONS_READ, 2, CHAR_2_LEN, (u8 *)(&my_characterUUID), (u8 *)(&hidReportMapChar[0]), 0},				 //prop
    {0, ATT_PERMISSIONS_ENCRYPT_READ, 2, sizeof(reportMap), (u8 *)(&hidReportMapChar[3]), (u8 *)(reportMap), 0},	 //value
    //{0,ATT_PERMISSIONS_READ|ATT_PERMISSIONS_WRITE,2,sizeof(extServiceUUID),(u8*)(&extReportRefUUID),  (u8*)(&extServiceUUID), 0}, //value

    // 0026 - 0027 . hid information: 2
    {0, ATT_PERMISSIONS_READ, 2, CHAR_2_LEN, (u8 *)(&my_characterUUID), (u8 *)(&hidinformationChar[0]), 0}, 			 //prop
    {0, ATT_PERMISSIONS_READ, 2, 4, (u8 *)(&hidinformationChar[3]), (u8 *)(hidInformation), 0}, //value

    // 0028 - 0029 . control point: 2
    {0, ATT_PERMISSIONS_READ, 2, CHAR_2_LEN, (u8 *)(&my_characterUUID), (u8 *)(&hidCtrlPointChar[0]), 0},				 //prop
    {0, ATT_PERMISSIONS_WRITE, 2, sizeof(controlPoint), (u8 *)(&hidCtrlPointChar[3]), (u8 *)(&controlPoint), 0},	 //valu
};

void my_att_init()
{
    bls_att_setAttributeTable((u8 *)my_Attributes);
}


