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

#ifndef _AAA_CONFIG_H_
#define _AAA_CONFIG_H_

#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"
#include "aaa_sensor_pix.h"
#include "aaa_sensor.h"
#include "../common/blt_led.h"
#include "../../vendor/common/user_config.h"
#include "aaa_24g_rf_frame.h"
#include "aaa_24g_rf.h"
#include "aaa_battery_check.h"
#include "aaa_emi.h"
#include "../common/blt_soft_timer.h"
#include "vendor/common/blt_common.h"
#include "app_audio.h"
#include "aaa_ml_m_protocol.h"
#include "../../application/audio/audio_config.h"
#include "../../drivers/8258/flash/flash_type.h"




extern my_fifo_t	fifo_km;

typedef struct
{
    /** Minimum value for the connection event (interval. 0x0006 - 0x0C80 * 1.25 ms) */
    u16 Min;
    /** Maximum value for the connection event (interval. 0x0006 - 0x0C80 * 1.25 ms) */
    u16 Max;
    /** Number of LL latency connection events (0x0000 - 0x03e8) */
    u16 latency;
    /** Connection Timeout (0x000A - 0x0C80 * 10 ms) */
    u16 timeout;
} gap_periConnectParams_t;


//--------------------function defein----------------------------------------

#define MS_BTN_LEFT 	0X01
#define MS_BTN_RIGHT 	0X02
#define MS_BTN_MIDDLE 	0X04
#define MS_BTN_K4 		0X08
#define MS_BTN_K5 		0X10

#define MS_BTN_MODE     0X20
#define MS_BTN_PAIR     0X40
#define MS_BTN_CPI 		0X80

#define MS_BTN_DOUBLE_LEFT 		0X100
#define MS_BTN_DEVICE_1 0x200
#define MS_BTN_DEVICE_2 0x400
#define MS_BTN_24G_MODE 0x200
#define MS_BTN_BLE_MODE 0x400

#define MS_BTN_DEVICE_3 0x800
#define MS_BTN_DEVICE_4 0x1000

#define MS_BTN_KEY_WRITE         MS_BTN_K5
#define MS_BTN_KEY_TRANSLATOR    MS_BTN_CPI
#define MS_BTN_SEARCH	         MS_BTN_K4

#define MS_BTN_RELEASE  0x0000
enum {
    BTN_LONG_RELEASE = 0,
    BTN_LONG_DOWN,
};

#if(MCU_CORE_TYPE == MCU_CORE_825x)
		#define DEFAULT_NORMAL_TX_POWER   RF_POWER_P8p13dBm
		#define DEFAULT_PAIR_TX_POWER	  RF_POWER_P0dBm
		#define DEFAULT_EMI_TX_POWER      RF_POWER_P0dBm
#elif(MCU_CORE_TYPE == MCU_CORE_827x)
	#define DEFAULT_NORMAL_TX_POWER   RF_POWER_P8p92dBm
	#define DEFAULT_PAIR_TX_POWER	  RF_POWER_P0dBm
	#define DEFAULT_EMI_TX_POWER      RF_POWER_P0dBm
#endif

//------------tx power define-------------------
//--------------------------------------------------

//----------------------------------------------------------------------------



typedef struct
{
    u32  dev_mac; //00~03
    u16  group_id;//04~05
    u16  rf_vid;	//06~07
    u8   bat_type;	//08
    u8	 tx_power;	//09
    u8	 paring_tx_power;//0a
    u8   emi_tx_power;   //0b
    u8	 wheel_direct;     //0c
    u8	 internal_cap;   //0d
    u8   sensor_direct;  //0e
    //15
    //18
    u8   device_name[18];	//0f 10 11 12 13 14 15 16 17 18 19 1a 1b 1c 1d 1e 1f 20
    //10
    u8   sn[10];//0x21 22 23 24 25 26 27 28 29 2a
    //5
    u8 mouse_type;//0x2b
    u8 pop_int;//0x2c
    u8 audio_reat;//0x2d
    u8 resv[2];
    //total 48byte
} custom_cfg_t;
extern u8 report_rate;

_attribute_data_retention_user extern custom_cfg_t   user_cfg;
_attribute_data_retention_user extern  u8 pub_key[];
_attribute_data_retention_user extern  u8 device_name_len;
_attribute_data_retention_user extern u8	tbl_advData_aaa[];

 extern u8 sensor_type;
 extern u8 product_id1;
 extern u8 product_id2; 
 extern u8 product_id3;
  extern u8 connect_ok;
 extern u8 connect_ok_flag;
 extern u8 has_double_click_left;

typedef enum{
	RF_IDLE_STATUS =	0,
    RF_TX_START_STATUS=1,
    RF_TX_END_STATUS=2,
    RF_RX_START_STATUS=3,
    RF_RX_END_STATUS=4,
	RF_RX_TIMEOUT_STATUS=5,
		
}RF_STATUS_USER;
extern  volatile unsigned int  rf_state;
extern  volatile int	   device_ack_received;
extern  u8	device_channel;

typedef enum
{
    TWO_SPEED_SWITCH = 0,
    THREE_SPEED_SWITCH = 1,
} SWITCH_TYPE_AAA;

typedef enum
{
    POWER_ON_ANA_AAA = 0,
    DEEP_SLEEP_ANA_AAA = 1,
    MODE_CHANGE_REBOOT_ANA_AAA = 2,
    BLE_PAIR_REBOOT_ANA_AAA = 3,
    CONN_PARAM_FAIL_REBOOT_ANA_AA = 4,
    CLEAR_FLAG_ANA_AAA = 5,
    MUTI_DEVICE_REBOOT_ANA_AAA = 6,
    RESET_PRODUCT_REBOOT_ANA_AAA=7,
} ANA_STATUS_AAA;


typedef enum
{
    POWER_ON_STATUS_AAA = 0,
    HIGH_ADV_STATUS_AAA = 1,
    LOW_ADV_STATUS_AAA = 2,
    BEGIN_CONNECTED_STATUS_AAA = 3,
    OK_CONNECTED_STATUS_AAA = 4,
    T5S_CONNECTED_STATUS_AAA = 5,
    DEEP_SLEEPE_STATUS_AAA = 6,
    DEEP_TERMINATE_STATUS_AAA = 7,
   // MODE_CHANGE_TERMINATE_STATUS_AAA = 8,
   // CONN_PARAM_FAIL_TERMINATE_STATUS_AAA = 9,
   // MUTI_DEVICE_CHANGE_STATUS_AAA = 10,
} BLE_STATUS_AAA;
typedef enum
{
    KEY_PRESS_EVENT_AAA = BIT(0),
    NEW_KEY_EVENT_AAA = BIT(1),
    SENSOR_DATA_EVENT_AAA = BIT(2),
    WHEEL_DATA_EVENT_AAA = BIT(3),
} key_EVENT_AAA;
typedef enum
{
    UNKNOW_OS_TYPE = 0,
    APPLE_OS_TYPE = 1,
    ANDROID_OS_TYPE = 2,
    WINDOWS_OS_TYPE = 3,
} PC_OS_TYPE_AAA;
typedef enum
{
    HAS_MOUSE_REPORT = BIT(0),
    HAS_KEYBOARD_REPORT = BIT(1),
    HAS_CONSUMER_REPORT = BIT(2),
    HAS_JOYSTIC_REPORT = BIT(3),
} HAS_REPORT_TYPE_AAA;
typedef enum
{
    FN_PRESS_AAA = BIT(0),
    T_BIND_PRESS_AAA = BIT(1),
    MOUSE_KEY_PRESS_AAA = BIT(2),
    T_ESC_PRESS_AAA = BIT(3),
    T_Q_PRESS_AAA = BIT(4),
    T_W_PRESS_AAA = BIT(5),
    T_E_PRESS_AAA = BIT(6),
    T_R_PRESS_AAA = BIT(7),
    T_T_PRESS_AAA = BIT(8),
    T_Y_PRESS_AAA = BIT(9),
    T_U_PRESS_AAA = BIT(10),
    T_I_PRESS_AAA = BIT(11),
    T_O_PRESS_AAA = BIT(12),
    T_F1_PRESS_AAA = BIT(13),
    T_SPACE_PRESS_AAA = BIT(14),
} SPECIAL_KEY_PRESS_FLAG_AAA;

#define MAX_BTN_CNT_AAA 6
typedef struct
{
    u8 cnt;
    u8 keycode[MAX_BTN_CNT_AAA];
    u16 special_key_press_f;
    u8 press_cnt;
} kb_data_t_aaa;
_attribute_data_retention_user extern kb_data_t_aaa key_buf_aaa;

typedef enum
{
    MULTIPIPE_1 = 0,
    MULTIPIPE_1_DOT_5 = 1,
    MULTIPIPE_2_DOT_0 = 2,
    MULTIPIPE_2_DOT_5 = 3,
    MULTIPIPE_3_DOT_0 = 4,
} SENSOR_MULTIPIPE_XY;



typedef struct
{
    u32 dongle_id; //4	
    u8 slave_mac_addr[4];//8
    u8 mode; //9
    u8 mast_id; //10
	u8 dpi;//11
    u8 host_set_sn[10];//21
    u8 mouse_type; //22
    u8 pop_int; //23
    u8 audio_reat;//24
    u8 sensor_direct;//25
	u8 temp[7]; //32
		
} FLASH_DEV_INFO_AAA;
#define SAVE_MAX_IN_FLASH  32
_attribute_data_retention_user extern FLASH_DEV_INFO_AAA flash_dev_info;


typedef struct
{
    u32 bin_crc;
    u8 rsv_1[6];
    u8 sensor_type;
    u8 sensor_pd1;
    u8 sensor_pd2;
    u8 sensor_pd3;
#if SHOW_MAST_REAL_MAC_DEBUG
    u8 master_mac[6];
#endif
} OUTPUT_DEV_INFO_AAA;
_attribute_data_retention_user extern OUTPUT_DEV_INFO_AAA output_dev_info;
extern u8 ic_inf[];


extern device_led_t  ble_hw_led;
extern device_led_t  d24g_hw_led;
extern device_led_t  lvd_hw_led;
extern unsigned	int	 ota_program_bootAddr;


extern u32 blt_ota_start_tick;
extern int bond_device_flash_cfg_idx;
extern u32 ota_program_offset;
extern	int	SMP_PARAM_NV_ADDR_START;


_attribute_data_retention_user	extern u8  user_devName[];

extern int		device_sync;

_attribute_data_retention_user extern u8 deep_flag;
_attribute_data_retention_user extern u8 pair_flag;
_attribute_data_retention_user extern u8 ana_reg1_aaa;
_attribute_data_retention_user extern u8 has_been_paired_flag;
_attribute_data_retention_user extern u8 user_force_pair_flag;

_attribute_data_retention_user extern u8 d24g_power_on_Pair_flag;

 extern  u8 suspend_wake_up_enable;
_attribute_data_retention_user extern u8 has_new_key_event;

_attribute_data_retention_user extern u32 row_pins[3];
_attribute_data_retention_user extern u32 col_pins[2];
_attribute_data_retention_user extern u8 fun_mode;

_attribute_data_retention_user extern u32 btn_tick;
_attribute_data_retention_user  extern u32 power_on_tick;
_attribute_data_retention_user extern u32 start_tick;
extern u8 dpi_value;

_attribute_data_retention_user extern u8 active_disconnect_reason;

_attribute_data_retention_user extern u8 ble_status_aaa;

extern ble_sts_t  blc_att_setServerDataPendingTime_upon_ClientCmd(u8 num_10ms);

extern _attribute_data_retention_user u8 encKey[];
//extern _attribute_data_retention_   ll_mac_t  bltMac;
extern _attribute_data_retention_user u8 	 my_batVal[1] ;
extern _attribute_data_retention_ u8 need_batt_data_notify;


_attribute_data_retention_user extern u32		advertise_begin_tick;
_attribute_data_retention_user extern int binding_master_addr_idx;
_attribute_data_retention_user extern u8 binding_master_addr[];

_attribute_data_retention_user extern mouse_data_t ms_data;
_attribute_data_retention_user extern mouse_data_t ms_buf;

_attribute_data_retention_user extern u8 mouse_btn_in_sensor;

_attribute_data_retention_user extern u8 has_new_report_aaa;
//_attribute_data_retention_user extern u8 combination_flag;

extern u32 adv_begin_tick;
extern u32 adv_count;
extern u32 idle_tick;
extern u32  idle_count;
extern u32 loop_cnt;
extern u8 need_save_battery_flag;

_attribute_data_retention_user extern u16 adv_timer_count;
_attribute_data_retention_user extern u32 adv_led_interval;

_attribute_data_retention_user extern u16 btn_value;
_attribute_data_retention_user extern u16 last_btn_value;
extern int deepRetWakeUp;
_attribute_data_retention_user extern u8 device_status;
_attribute_data_retention_user extern u32 wakeup_next_tick;


#if HAS_BACK_LIGHT_AAA
    _attribute_data_retention_user extern u8 back_light_switch_flag;

#endif
#if SENSOR_FUN_ENABLE_AAA
    _attribute_data_retention_user extern u8 sensor_fun;
#endif
#if BATT_CHECK_FUN_ENABLE
    _attribute_data_retention_user extern u8 low_bat_count;
#endif

_attribute_data_retention_user  extern u8 xy_multiple_flag;
_attribute_data_retention_user extern int dev_info_idx;

extern u16 led_pin;
extern u8 low_Batt_flag;
extern u16 batt_vol_mv;
extern u16 min_vol_mv;

extern volatile u8 varb_ADPCM_PACKET_LEN;
extern volatile u16 varb_TL_MIC_ADPCM_UNIT_SIZE;
extern volatile u16 varb_TL_MIC_BUFFER_SIZE;
extern volatile u8 audio_reat_setting;

extern void debug_loop_toggle();

extern void user_init_normal();
extern void user_init_deepRetn();

extern void main_loop(void);
extern void save_dev_info_flash();
extern int cccWrite(void *p);
extern int sppWrite(void *p);

void blc_smp_set_simple_multi_mac_en(u8 en);


extern void ble_pair_process_aaa();
extern void clear_pair_flag();
extern void clear_fifo();

extern void set_pair_flag();
extern void user_reboot(u8 reason);

extern void d24_start_pair();
extern void reset_idle_status();
extern void idle_status_poll();
extern  void adv_count_poll();

extern void wheel_set_wakeup_level_deep();
extern _attribute_ram_code_ u8 wheel_get_value(u32 wheel_prepare_tick);
extern void sensor_gpio_powerDownConfig();
extern void	rf_power_enable(int en);
extern u8 adaptive_smoother();
extern void battery_check();
extern void battery_hw_init(void);
extern void ble_main_loop_aaa();
extern void ble_user_init_aaa();

extern void ble_notify_data_proc_aaa();
extern  u16 btn_scan();
extern  u8 btn_get_value();
extern void btn_set_wakeup_level_deep();
extern void btn_set_wakeup_level_suspend(u8 enable);
extern void sensor_set_wakeup_level_suspend(u8 enable);
extern void sensor_set_wakeup_level_deepsleep(u8 enable);

extern void wheel_set_wakeup_level_suspend(u8 enable);

extern void change_mode_aaa(u8 event_new);
extern u8 check_fifo_has_data();
extern void check_sensor_dircet(u8 sensor_dir);

extern u8 clear_press_key_aaa();
extern void d24_main_loop();
extern void d24_user_init();
extern void enter_deep_aaa();

extern void deep_wakeup_proc_aaa();
extern  _attribute_ram_code_ int device_send_packet(u8 *p, int retry, u16 rx_waittime);
extern void btn_dpi_set();
extern void sensor_dpi_set(u8 dpi);



extern void emi_process();

extern int flash_info_load_aaa(u32 s_addr, u8 *d_addr,  int len);
extern u8 get_ble_data_report_aaa();
extern void get_24g_data_report_aaa();
extern void hw_init();
extern void led_hw_init();

extern void led_24g_ConnectedStatus();
extern void led_ble_ConnectedStatus();

extern void led_24g_mode_display();
extern void led_ble_mode_display();
extern void led_bat_lvd();
void led_power_off();
extern void led_ble_Adv_poll();
extern void led_2p4_Adv_poll();
extern void dpi_led_set(u8 idx);
extern u32 mouse_wheel_prepare_tick(void);

extern _attribute_ram_code_ unsigned int OPTSensor_motion_report(u32 no_overflow);
extern void OPTSensor_Shutdown(void);
extern u8 Draw_a_square_test();
void sensor_check_lift(u8 en);


extern void delet_soft_time();
extern void ui_loop();
extern void add_soft_time();
extern void check_softe_time();
extern void ble_pm_aaa();
extern void d24_check_pair_process();
extern void switch_type_poll();
extern void switch_type_init();

extern void user_batt_check_init();
extern void user_batt_check_proc();

extern void aes_ll_decryption(u8 *key, u8 *plaintext, u8 *result);
extern void swapX(const u8 *src, u8 *dst, int len);
extern void aes_user_encryption(u8 *key, u8 *plaintext, u8 *encrypted_data);
extern void aes_user_decryption(u8 *key, u8 *encrypted_data, u8 *decrypted_data);
extern  void aes_user_set_key();

extern void  double_click_left_24G_mode();

extern  int speckWrite(void *p);
extern  void write_deep_ana0(u8 buf);
void fenwei_led_on(void);
void fenwei_led_off(void);
extern void flash_write_page_user(unsigned long addr, unsigned long len, unsigned char *buf);
extern void user_battery_power_check();
extern void ota_success_led();
extern void ota_fail_led();
extern void d24g_ota_loop();
extern void mode_led_switch(void);
extern void flash_lock_init(void);
extern void flash_unlock_init(void);
#endif

