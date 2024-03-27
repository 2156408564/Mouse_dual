/********************************************************************************************************
 * @file     AAA_24G_APP.c
 *
 * @brief    This is the source file for KMD SDK
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

#include "AAA_public_config.h"

extern void mouse_task_when_rf();

#define DEVICE_TYPE_INDEX  1// 1 mouse  ,2 keyboard

#define  DATA_3_CHOOSE_1_ENABLE   0

#define IDLE_TIME_S    60
u8 dongle_id_need_save_flag;
u8 need_save_battery_flag = 1;





typedef enum
{
    STATE_POWERON = 0,
    STATE_PAIRING,
    STATE_NORMAL,
	STATE_OTA,
} MOUSE_MODE;

 static u8 last_connect_ok=0xff;

 u8 device_status = 0;
 u32 dongle_id;
 u8 mouse_send_need_f = 0;
 volatile u32 no_ack = 0;

 rf_packet_t	rf_pair_buf =
{
    20,	// dma_len
    19,	// rf_len
};
pair_data_t *p_pair_dat=(pair_data_t*)&rf_pair_buf.dat[0];

 rf_packet_t rf_km_buf =
{
#if DATA_3_CHOOSE_1_ENABLE
	55,	// dma_len
    54,	// rf_len
#else
    14,	// dma_len
    13,	// rf_len
#endif
 };
 
 km_3_c_1_data_t *p_km_data=(km_3_c_1_data_t*)&rf_km_buf.dat[0];

 u32 wakeup_next_tick = 0;

 u8 pair_success_flag = 0;

MYFIFO_INIT (fifo_km, 36, 32);//The size must be a multiple of 4 bytes

volatile unsigned int rf_rx_timeout_us = 350;

#if D24G_OTA_ENABLE_AAA
u8 d24g_ota_status = 0;
u8 d24g_ota_start_flag = 0;
u8 d24g_ota_success_flag = 0;

enum {
	KM_24G_OTA_SUCCESS = 0x2A,
	KM_24G_OTA_FAIL = 0x2B,
};

rf_packet_t rf_ota_buf =
{
    sizeof(ota_data_t)+1,	// dma_len
    sizeof(ota_data_t),	// rf_len
};

ota_data_t *p_ota_data = (ota_data_t*)&rf_ota_buf.dat[0];
ota_ack_data_t p_ota_ack_data ;

#define D24G_OTA_LENGTH  24
 typedef struct{
     u8  report_id;
     u8  opcode;
     u16 length;
     u8  dat[20];
 }ota_buff_t;
 ota_buff_t ota_buff;
 u8 ota_buff_valid_flag;

void d24g_ota_resut(u8 result)
{
	my_printf_aaa("---d24g_ota_result=0x%x\r\n",result);
	my_printf_aaa("---fifo num =%d\r\n", my_fifo_number(&fifo_km));

	if(result == OTA_SUCCESS)
	{
		p_ota_ack_data.pno_no = KM_24G_OTA_SUCCESS;//2.4G OTA success
		my_fifo_push(&fifo_km, (u8 *)&p_ota_ack_data.pno_no, 25);
	}
	else
	{
		p_ota_ack_data.pno_no = KM_24G_OTA_FAIL;//2.4G OTA failed
		my_fifo_push(&fifo_km, (u8 *)&p_ota_ack_data.pno_no, 25);
	}

	//user_reboot(DEEP_SLEEP_ANA_AAA);
}

u8 d24g_ota_write(ota_buff_t *p_usb)
{
	static	u16 ota_index=0;
	static u16 start_index=0;
	static u32 flash_write_addr=0;
	static u8 first_data_buf[16];
	static u32 fw_size=0;
	static u8 ota_error_flag=0;
	ota_data_st *pd=(ota_data_st*)&p_usb->dat[0];

	if((pd->cmd== CMD_OTA_START)&&(p_usb->length==2))
	{
		blt_ota_start_tick = clock_time();  //mark time
		d24g_ota_start_flag = 1;  //mark time
		//notify_rsp_buf_init();
		wd_stop();
		ota_index=0;

		flash_write_addr=0;
		start_index=0;
		fw_size=0;
		//fw_check_value=0;
		//fw_cal_crc=0xffffffff;
		ota_error_flag=OTA_SUCCESS;
		flash_erase_sector(ota_program_offset);
		//app_enter_ota_mode();
	#if (BLT_APP_LED_ENABLE && BLE_OTA_LED_DEBUG)
        // TODO:add led
		//gpio_set_output_en(PIN_24G_LED, 1);
		//gpio_write(PIN_24G_LED, LED_ON_AAA);
	#endif
		blt_ota_start_tick = clock_time();  //mark time
		my_printf_aaa("24g_ota_start\r\n");
		return ota_error_flag;
	}
	else if((ota_error_flag==OTA_SUCCESS)&&(d24g_ota_start_flag))
	{
		if((pd->cmd == CMD_OTA_END)&&(p_usb->length==6))
		{
			my_printf_aaa("24g_ota_end\r\n");
			u32 *telink_mark=(u32*)&first_data_buf[8];
			if(telink_mark[0]!=0x544c4e4b)
			{
				ota_error_flag=OTA_FIRMWARE_MARK_ERR;
				return ota_error_flag;
			}
			u32 real_bin_size=0;
			real_bin_size=fw_size-4;
			if(real_bin_size!=(start_index*16))
			{
				ota_error_flag=OTA_FW_SIZE_ERR;
				return ota_error_flag;
			}
			
			flash_write_page(ota_program_offset,16,first_data_buf);

			u8 read_flash_buf[16];  
			flash_read_page(ota_program_offset,16, read_flash_buf);

			if(memcmp(read_flash_buf, first_data_buf, 16))
			{//do not equal
				flash_erase_sector(ota_program_offset);
				ota_error_flag=OTA_WRITE_FLASH_ERR;
				return ota_error_flag;
			}
			
			u32 temp_ota_program_offset;
			temp_ota_program_offset=ota_program_offset;	

			if(!ota_program_offset) ////zero, firmware is stored at flash 0x20000.
			{
				ota_program_offset = ota_program_bootAddr; ///NOTE: this flash offset need to set according to OTA offset
			}
			else ////note zero, firmware is stored at flash 0x00000.
			{
				ota_program_offset = 0x00000;
			}
			u8 ret=flash_fw_check(0xffffffff);
			ota_program_offset=temp_ota_program_offset;
			
			if(ret==0)
			{
				extern u32 fw_crc_init;
				my_printf_aaa("fw_crc_init=%x\r\n",fw_crc_init);
				my_printf_aaa("2.4G_ota_success\r\n");
				u8 flag = 0;
				flash_write_page((ota_program_offset ? 0 : ota_program_bootAddr) + 0x08, 1, (u8 *)&flag);	//Invalid flag
				d24g_ota_success_flag = 1;
			}
			else
			{
				flash_erase_sector(ota_program_offset);
				ota_error_flag=OTA_FW_CHECK_ERR;
				return ota_error_flag;
				
			}
		}
		else 
		{
			//my_printf_aaa("length=%d\r\n",p_usb->length);
			
			if((p_usb->length%20)!=0)
			{
				ota_error_flag=OTA_PDU_LEN_ERR;
				return ota_error_flag;
			}
			u8 cnt=p_usb->length/20;
			
			for(u8 i=0;i<cnt;i++)
			{
				pd=(ota_data_st*)&p_usb->dat[20*i];
				//my_printf_aaa("cmd=%d,crc=0x%04x_0x%04x.\r\n",pd->cmd,crc16((u8*)&pd->cmd,18),pd->crc);
				if(crc16((u8*)&pd->cmd,18)==pd->crc) //crc16_user
				{
					
					if(pd->cmd==0x0000)//first_data
                    {
						memcpy(first_data_buf,pd->buf,16);
						start_index=0;
                    }
					else
					{
						if((start_index+1)!=pd->cmd)
						{
							ota_error_flag=OTA_DATA_PACKET_SEQ_ERR;
							return ota_error_flag;
						}
						start_index=pd->cmd;
						if(pd->cmd==0x0001)//second 
						{
							fw_size=pd->buf[8] | (pd->buf[9] <<8) |(pd->buf[10]<<16) | (pd->buf[11]<<24);
							if((fw_size)>ota_program_bootAddr)//128k
							{
								ota_error_flag=OTA_FW_SIZE_ERR;
								return ota_error_flag;
							}
						}
						
						if((fw_size)<flash_write_addr)
						{
							ota_error_flag=OTA_FW_SIZE_ERR;
							return ota_error_flag;
						}
						
						if(ota_error_flag==OTA_SUCCESS)
						{
							if((flash_write_addr%4096)==0)
                            {
								flash_erase_sector(flash_write_addr+ota_program_offset);
                            }
                            flash_write_page(flash_write_addr+ota_program_offset,16,pd->buf);
						}
						
					}
					flash_write_addr+=16;
				}
				else
				{
					ota_error_flag=OTA_DATA_CRC_ERR;
					return ota_error_flag;
				}
			}
       }
	}
	return ota_error_flag;
}

void d24g_ota_loop()
{
	if(d24g_ota_start_flag)
	{
		if(clock_time_exceed(blt_ota_start_tick , 120 *1000 *1000))
		{
			d24g_ota_resut(OTA_TIMEOUT);
		}
		else if(d24g_ota_success_flag)
		{
			d24g_ota_success_flag = 0;

			d24g_ota_resut(OTA_SUCCESS);
		}
	}
}
#endif




_attribute_ram_code_sec_ u8  rf_rx_process(rf_packet_t *p_rf_data)
{
	if(device_status==STATE_PAIRING)
	{
		pair_ack_data_t *pair_ack_dat_ptr=(pair_ack_data_t*)&p_rf_data->dat[0];
		//if (p_pkt->proto == RF_PROTO_BYTE)
		{
        	if (((pair_ack_dat_ptr->cmd == PAIR_ACK_CMD) || (pair_ack_dat_ptr->cmd == RECONNECT_ACK_CMD )) && (pair_ack_dat_ptr->did == p_pair_dat->did))

			{
            	dongle_id = pair_ack_dat_ptr->gid;
				ll_chn_mask = pair_ack_dat_ptr->chn;
				//golden_dongle_test = p_pkt->per;//if p_pkt->per is 0xff, it is golden dongle paired
            	pair_success_flag = 1;
				if(pair_ack_dat_ptr->cmd == PAIR_ACK_CMD){
					dongle_id_need_save_flag = 1;
				}else{
					dongle_id_need_save_flag = 0;
					need_save_battery_flag = 0;
				}
				printf("pairing success--------- %x\n", pair_ack_dat_ptr->gid);
            	return 1;
        	}
		}
	}
	else if (device_status == STATE_NORMAL)
	{
		km_ack_data_t *km_ack_dat_ptr=(km_ack_data_t*)&p_rf_data->dat[0];
		if((mouse_send_need_f == 2))//mouse ack
		{
			#if 0
			rf_packet_ack_mouse_t *tmp=(rf_packet_ack_mouse_t *)p_pkt;

			u16 crc_c=0;
			for(u8 i=0;i<3;i++)
		    {
				crc_c= crc16_user(&tmp->dat[i].proto, sizeof(ack_mouse_rf_user_data_t)-2);
				if((crc_c==tmp->dat[0].crc16)||(crc_c==tmp->dat[1].crc16)||(crc_c==tmp->dat[2].crc16))
				{
					if(tmp->dat[i].type == FRAME_TYPE_ACK_MOUSE)
					{
						 return 1;
					}
				
				}
		    }
			#endif
			#if (DEVICE_TYPE_INDEX==1)
			if(km_ack_dat_ptr->cmd == MOUSE_ACK_CMD || km_ack_dat_ptr->cmd == USER_DEFINE_ACK_CMD\
                ||km_ack_dat_ptr->cmd == KB_ACK_CMD||km_ack_dat_ptr->cmd == VOICE_ACK_CMD)
			{
                //parse host cmd
                if (p_rf_data->rf_len == 8 && km_ack_dat_ptr->cmd == USER_DEFINE_ACK_CMD && km_ack_dat_ptr->data[0] == PROTOCOL_HEARD) {
                    miclink_host_cmd_handler(&km_ack_dat_ptr->data[0]);
					km_ack_dat_ptr->cmd = 0;
					km_ack_dat_ptr->data[0] = 0;
                }
				return 1;
			}
            #if D24G_OTA_ENABLE_AAA
            else if (km_ack_dat_ptr->cmd == D24G_OTA_ACK_CMD) {
                device_status = STATE_OTA;
                d24g_ota_status = 1;    //Enter 2.4g ota mode
                mouse_send_need_f = 3;
                rf_rx_timeout_us = 500;
				my_fifo_reset(&fifo_km);
#if (APP_FLASH_LOCK_ENABLE) //flash lock en
				flash_unlock_init();//flash lock init
#endif

            }
            #endif
			#endif
			#if (DEVICE_TYPE_INDEX==2)
			if(km_ack_dat_ptr->cmd == KB_ACK_CMD)
			{
				kb_led_status = km_ack_dat_ptr->host_led_status;
				kb_led_out_aaa(kb_led_status);
				ll_chn_mask = km_ack_dat_ptr->chn;
				return 1;
			}
			#endif
		}
	}
#if D24G_OTA_ENABLE_AAA

    if(device_status == STATE_OTA)
    {
        if(mouse_send_need_f == 3)//ota ack
        {
            ota_ack_data_t *ota_ack_dat_ptr = (ota_ack_data_t *)&p_rf_data->dat[0];

            #if 0
            printf("mrec: ");
            for (int i = 0; i < (24 + 2); i++) {
                 printf("%1x ", p_rf_data->dat[i]);
            }
            printf("\r\n");
            #endif

            if(ota_ack_dat_ptr->cmd == D24G_OTA_ACK_CMD)
            {
                if (ota_ack_dat_ptr->pno_no == 1) {
                    static u16 packet_cnt = 0;
                    ota_data_st *pd = (ota_data_st *)&ota_ack_dat_ptr->dat[0];
                    ota_buff_t *ota_ptr = (ota_buff_t *)&p_rf_data->dat[2];

                    switch(ota_ptr->length)
                    {
                        case 2: //ota - start
                        case 6: //ota - end
                            //printf("pd cmd %x\r\n", pd->cmd);
    						if((pd->cmd==0xff01)||(pd->cmd==0xff02))
    						{
    							ota_buff_valid_flag = 1;
    							memcpy(&ota_buff.report_id,&ota_ptr->report_id, D24G_OTA_LENGTH);
    							packet_cnt=0;
                                printf("-s-");
    						}
                            break;
                        case 20:
                            //printf("crc %x pd cmd %x\r\n", pd->crc, pd->cmd);
    						if((crc16((u8*)&ota_ack_dat_ptr->dat[0],18) == pd->crc)&&\
    							(packet_cnt == pd->cmd))
    						{
    							ota_buff_valid_flag = 1;
    							memcpy(&ota_buff.report_id,&ota_ptr->report_id, D24G_OTA_LENGTH);
    							packet_cnt++;
                                //printf("o");
    						}
                            break;
                        default:
                            printf("---err data len %d\n", ota_ptr->length);
                            break;
                    }

                    memcpy((u8 *)&p_ota_ack_data.pno_no, &ota_ack_dat_ptr->pno_no, 25);
                    my_fifo_push(&fifo_km, (u8 *)&p_ota_ack_data.pno_no, 25);
                } else if (ota_ack_dat_ptr->pno_no == KM_24G_OTA_SUCCESS) {
						printf("--24G OTA success confirm, reboot\n");
						ota_success_led();
						flash_erase_sector(CFG_DEVICE_MODE_ADDR);
						user_reboot(DEEP_SLEEP_ANA_AAA);
				} else if (ota_ack_dat_ptr->pno_no == KM_24G_OTA_FAIL) {
					printf("--24G OTA fail confirm, reboot\n");
					ota_fail_led();
					user_reboot(DEEP_SLEEP_ANA_AAA);
				}

                return 1;
            }
        }
    }
#endif

    return 0;
}


void d24_start_pair()
{
    set_pair_flag();
    user_reboot(CLEAR_FLAG_ANA_AAA);
	adv_count=0;
	adv_begin_tick=clock_time()|1;
}



void d24_user_init()
{
	
	printf("d24_user_init\n");
    /*****************   set up did(device id)    ********************/
	u32 device_id = ((user_cfg.dev_mac<<8)|DEVICE_TYPE_INDEX);

	p_pair_dat->cmd=PAIR_CMD;
	p_pair_dat->did=device_id;
	p_km_data->did=device_id;
	
	flash_dev_info.audio_reat = 1;
	audio_reat_setting = AUDIO_16K;

#if (DEVICE_TYPE_INDEX==2)
    p_km_data->cmd=KB_CMD;
#else
	p_km_data->cmd=MOUSE_CMD;
#endif

#if D24G_OTA_ENABLE_AAA
        p_ota_data->cmd = D24G_OTA_CMD;
        p_ota_data->did = device_id;
#endif

    dongle_id = flash_dev_info.dongle_id;

	printf("dongle_id %x\n", flash_dev_info.dongle_id);
#if  ENTER_PAIR_WHEN_NEVER_PAIRED_ENABLE
   if ((dongle_id == U32_MAX) || (dongle_id == 0))
        set_pair_flag();
#endif

#if (ADAPT_ALL_HW_AUTO_DRAW_IN_D24G_MODE_DEBUG==1)
	set_pair_flag();
#endif


    if (pair_flag)
    {
        device_status = STATE_PAIRING;
		printf("pkt_pairing.did %x\n", p_pair_dat->did);

    } else {
        	device_status = STATE_NORMAL;
    }

    wakeup_next_tick = clock_time();
    reset_idle_status();
}

////////////////////////////////////////////////////////////////////
u8 need_suspend_flag=0; 
u8 rptr = 0;
u8 rx_fifo_count=0;
void check_rf_complet_status()
{
	rf_state = RF_IDLE_STATUS;
	static u32 ack_miss_no = 0;
	
	if (device_ack_received)
	{
		#if (HOPING_METHOD==2)
		device_channel = get_next_channel_with_mask(ll_chn_mask, device_channel);
		#endif
		ack_miss_no = 0;
		no_ack= 0;
		if(device_status <= STATE_PAIRING)
		{

			if(pair_success_flag)
			{
				device_status = STATE_NORMAL;
				
                connect_ok=1;
				connect_ok_flag = 1;
				
                if (flash_dev_info.dongle_id != dongle_id)
                {
                    flash_dev_info.dongle_id = dongle_id;
					if(dongle_id_need_save_flag){
						save_dev_info_flash();
						printf("save info flash %x\n", dongle_id);
					}
                }

                reset_idle_status();
                clear_pair_flag();
			}
				my_fifo_reset(&fifo_km);
		}
		else if (device_status == STATE_NORMAL)
		{
			if (mouse_send_need_f )	// skip to next packet
			{
				my_fifo_pop(&fifo_km);
			}
			if (!connect_ok)
			{
				connect_ok_flag = 1;
			}
			connect_ok=1;
		} 
#if D24G_OTA_ENABLE_AAA
        else if (device_status == STATE_OTA) {
            if (mouse_send_need_f)
            {
                my_fifo_pop(&fifo_km);
                if(ota_buff_valid_flag)
                {
                    u8 ota_error_flag = d24g_ota_write(&ota_buff);
                    if(ota_error_flag)
                    {
                        printf("-err flag=%d.\n",ota_error_flag);
                        d24g_ota_resut(ota_error_flag);
                    }
                    memset(&ota_buff, 0, sizeof(ota_buff_t));
                    ota_buff_valid_flag = 0;
                    idle_status_poll();
                }
            }
            connect_ok=1;
        }
        #endif
		mouse_send_need_f = 0;
	}
	else
	{
		no_ack++;
		ack_miss_no ++;
		if (ack_miss_no >= LL_CHANNEL_SEARCH_TH) 
		{
			ll_chn_mask = LL_CHANNEL_SEARCH_FLAG;
		}
		if (ack_miss_no >= 2)
		{
			device_channel = get_next_channel_with_mask(ll_chn_mask, device_channel);
		}
	
		if(no_ack>125)
		{
			connect_ok=0;
		}
	}
}

unsigned short 	crc16_poly1[2] = {0, 0xa001};


_attribute_ram_code_sec_ unsigned short crc16_user (unsigned char *pD, int len)
{
    unsigned short crc = 0xffff;
    int i,j;

    for(j=len; j>0; j--)
    {
        unsigned char ds = *pD++;
        for(i=0; i<8; i++)
        {
            crc = (crc >> 1) ^ crc16_poly1[(crc ^ ds ) & 1];
            ds = ds >> 1;
        }
    }

     return crc;
}


void d24g_rf_loop()
{
	u8 *ptr = 0;

	if(rf_state==RF_IDLE_STATUS)
	{
		if(device_status <= STATE_PAIRING)
		{
#if 0
			if (user_cfg.rf_vid != U16_MAX)
			{
				set_pair_access_code(rf_access_code_16to32(user_cfg.rf_vid));
			}
			else
			{
				set_pair_access_code(0x39517695);//
			}
#else
            set_pair_access_code(rf_access_code_16to32(0x7c2a));
#endif
			pair_success_flag = 0;
			if (device_status == STATE_PAIRING)
			{
			    rf_set_power_level_index(user_cfg.paring_tx_power);
			}
			connect_ok=0;
			ptr=(u8 *)&rf_pair_buf;
			mouse_send_need_f = 1;//pairing
		}
		else if (device_status == STATE_NORMAL)
		{
			ptr = (u8 *)&rf_km_buf;

			set_data_access_code(flash_dev_info.dongle_id);
			rf_set_power_level_index(user_cfg.tx_power);

			if((mouse_send_need_f==0))
			{ 
				u8 *p =  my_fifo_get (&fifo_km);
				if(p)
				{
                    mouse_send_need_f = 2;
                    /*不修改mouse数据结构体了
                     * 更加fifo中第一个字节来判断当前数据是mouse/cmd/voice
                     * mouse: p[0] 是btn 值一定小于8
                     * cmd p[0]:USER_DEFINE_CMD
                     * voice data: p[0]:VOICE_CMD
                     */

                    if(p[0] == USER_DEFINE_CMD) {
                        //printf("u");
                        p_km_data->cmd = USER_DEFINE_CMD;
                        rf_km_buf.dma_len = 27;
                        rf_rx_timeout_us = 406;
                        memcpy((u8 *)&p_km_data->km_dat[0], &p[1], 20);
                    } else if (p[0] == VOICE_CMD) {
                        //printf("v");
                        p_km_data->cmd = VOICE_CMD;
                        rf_km_buf.dma_len = 39;
                        rf_rx_timeout_us = 454;
                        memcpy((u8 *)&p_km_data->km_dat[0], &p[1], 32);
                    } else if (p[0] == KB_CMD){
                        //printf("send kb data\r\n");
                        //kb data
                        p_km_data->cmd = KB_CMD;
                        rf_km_buf.dma_len = 13;
                        rf_rx_timeout_us = 350;
                        memcpy((u8 *)&p_km_data->km_dat[0], &p[1], 6);
                    } else {
                        //mouse data
                        //printf("m");
                        p_km_data->cmd = MOUSE_CMD;
                        rf_km_buf.dma_len = 13;
                        rf_rx_timeout_us = 350;
                        memcpy((u8 *)&p_km_data->km_dat[0], p, 6);
                    }
                    rf_km_buf.rf_len = rf_km_buf.dma_len - 1;
					p_km_data->seq_no++;
				}
            }
        }
#if D24G_OTA_ENABLE_AAA
        else if(device_status == STATE_OTA)
        {
            if(d24g_ota_status)
            {
                ptr = (u8 *)&rf_ota_buf;

                set_data_access_code(flash_dev_info.dongle_id);
                rf_set_power_level_index(user_cfg.tx_power);

                if(mouse_send_need_f == 0)
                {
                    u8 *p =  my_fifo_get (&fifo_km);
                    if(p)
                    {
                        mouse_send_need_f = 3;
						u8 *tmp = (u8 *)&p[0];
                        p_ota_data->pno_no = tmp[0];
						memcpy((u8 *)&p_ota_data->report_id, &tmp[1], D24G_OTA_LENGTH);
                    }
                }
            }
        }
#endif

		if(mouse_send_need_f) //send
		{
			rf_state=RF_TX_START_STATUS;
			rf_set_tx_rx_off();//must add
			device_ack_received = 0;
            rf_stx_to_rx(ptr, rf_rx_timeout_us);
			reg_rf_irq_status = 0xffff;
		}

	}
	else if(rf_state==RF_RX_END_STATUS)
	{
        irq_device_rx();
		check_rf_complet_status();
	}
	else if(rf_state==RF_RX_TIMEOUT_STATUS)
	{
		check_rf_complet_status();
	}
}


void ui_loop_24g(u32 enter_us)
{
	static u8 scan_interval=0;
	u8 need_run_flag=0;
	u32 wheel_prepare_tick;
	ms_data.wheel=0;
    scan_interval++;
	if(scan_interval>=(8000/enter_us))
	{
		scan_interval=0;
		need_run_flag=1;
	}
	if(need_run_flag)
	{
#if WHEEL_FUN_ENABLE_AAA
		 wheel_prepare_tick = mouse_wheel_prepare_tick();
#endif

#if BUTTON_FUN_ENABLE_AAA
		 has_new_key_event |= btn_get_value();
#endif

		idle_status_poll();
		device_led_process();
        if((connect_ok==0))
		{
#if BLT_APP_LED_ENABLE
            led_2p4_Adv_poll();
#endif
			adv_count_poll();
			my_fifo_reset(&fifo_km);
		}

		if(last_connect_ok!=connect_ok)
		{
			last_connect_ok=connect_ok;
			if(last_connect_ok)
			{
				led_24g_ConnectedStatus();
			}
		}
	}

	mouse_task_when_rf();

	if(need_run_flag)
	{
#if WHEEL_FUN_ENABLE_AAA
		 has_new_key_event |= wheel_get_value(wheel_prepare_tick);
#endif
	}

	if ((device_status == STATE_NORMAL)) {

            if (need_pop_internet) {
                if(need_win_R_internet){
					five_sec_win_r_internet();
				}else{
					five_sec_pop_internet();
				}
            }

			if (has_new_key_event) 
			{
				has_new_key_event = 0;
				reset_idle_status();
				my_fifo_push(&fifo_km, &ms_data.btn, sizeof(mouse_data_t));
			}
			else if((idle_count<IDLE_TIME_S)||ms_data.btn) 
			{
                u8 *p=my_fifo_get(&fifo_km);
				if(p==0)
				{
					my_fifo_push(&fifo_km, &ms_data.btn, sizeof(mouse_data_t));
				}
			}
	 }
}

void pm_poll()
{
	u32 wake_src=0;
	u32 interval_us=0;
	need_suspend_flag=0;

// TODO:check
#if BLE_AUDIO_ENABLE
    if (ui_mic_enable)
    {
        return;
    }
#endif

	if(rf_state==RF_IDLE_STATUS)
	{
		if(device_status <= STATE_PAIRING)
		{
		#if D24G_ADV_ENTER_DEEPSLEEP_AFTER_TIME_OUT_ENABLE_AAA
			if(adv_count>=D24G_ADV_TIMER_OUT)//30s
	   		{
	   		    printf("enter_deep_aaa---4\r\n");
		   		enter_deep_aaa();
	   		}
		 #endif
		 	wake_src=PM_WAKEUP_TIMER;
		 	interval_us=8000;
		    need_suspend_flag = 1;
		}
		else
		{
			if(my_fifo_get(&fifo_km)==0)
			{
                #if D24G_OTA_ENABLE_AAA
                if(d24g_ota_status)
                {
                    #if D24G_CONNECT_ENTER_DEEPSLEEP_AFTER_TIME_OUT_ENABLE_AAA
                    if(idle_count>=D24G_CONNECT_TIME_OUT) {
                        enter_deep_aaa();
                    }
                    #endif
                }
                else
                #endif
				if((idle_count<IDLE_TIME_S))
				{
					if(report_rate>=4)
					{
						need_suspend_flag=1;
				 		wake_src=PM_WAKEUP_TIMER;
		 				interval_us=report_rate*1000;
						if (suspend_wake_up_enable)
				        {

				         
						#if WHEEL_FUN_ENABLE_AAA
				            wheel_set_wakeup_level_suspend(0);
						#endif
						#if BUTTON_FUN_ENABLE_AAA
				            btn_set_wakeup_level_suspend(0);
						#endif
						#if SENSOR_FUN_ENABLE_AAA
				            sensor_set_wakeup_level_suspend(0);
						#endif
							suspend_wake_up_enable = 0;
				        }
					}
					
				}
				else
				{
					#if D24G_CONNECT_ENTER_DEEPSLEEP_AFTER_TIME_OUT_ENABLE_AAA
						if(idle_count>=D24G_CONNECT_TIME_OUT)
						{
                            printf("enter_deep_aaa---3\r\n");
							enter_deep_aaa();
						}
					#endif
					#if WHEEL_FUN_ENABLE_AAA
						wheel_set_wakeup_level_suspend(1);		//wake_time ==100ms 拢要gpio 饣�
					#endif

					#if BUTTON_FUN_ENABLE_AAA
				        btn_set_wakeup_level_suspend(1);
					#endif

					#if SENSOR_FUN_ENABLE_AAA
				        sensor_set_wakeup_level_suspend(1);
					#endif
                    need_suspend_flag=1;
                    suspend_wake_up_enable=1;
                    wake_src=PM_WAKEUP_TIMER|PM_WAKEUP_PAD;
                    interval_us=100000;//for dongle						
				}
			}
			if(connect_ok==0)
			{
				if(idle_count>5)
				{
				    printf("idle_count %d\r\n", idle_count);
					if(btn_value==0)
					{
                        printf("enter_deep_aaa---1\r\n");
						enter_deep_aaa();
					}
				}
			}
			
		}

		if(need_suspend_flag)
		{
			cpu_sleep_wakeup(SUSPEND_MODE, wake_src, (wakeup_next_tick+ interval_us*CLOCK_16M_SYS_TIMER_CLK_1US));
			wakeup_next_tick=clock_time();
			rf_drv_private_2m_init();
		}
		
	}
	
}

void d24_main_loop()
{
	u32 temp = 0;
	static  u32 tick_loop = 0;

	if(device_status <= STATE_PAIRING)
	{
		temp=8000;
	}
	else
	{
		#if 1
		if(my_fifo_number(&fifo_km)>6)
		{
			temp=report_rate*1000+2000;
		}
		else if(my_fifo_number(&fifo_km)>4)//(TX_FIFO_NUM_AAA/2)
		{
			temp=report_rate*1000+1000;
		}
		else
		#endif
		{
			temp=report_rate*1000;
		}
	}

#if D24G_OTA_ENABLE_AAA
    if(d24g_ota_status)
    {
        if(clock_time_exceed(tick_loop, 1000))
        {
            tick_loop = clock_time();
            u8 *p = my_fifo_get(&fifo_km);
            if(p == 0) {
                //主动请求OTA数据
				p_ota_ack_data.pno_no = 0;
				my_fifo_push(&fifo_km, &p_ota_ack_data.pno_no, 25);
            }
            adv_count_poll();
        }
    }
    else
#endif
	if(need_suspend_flag)
	{
		ui_loop_24g(8000);
		tick_loop=clock_time()|1;
	}
	else if(clock_time_exceed(tick_loop,temp))
	{
		tick_loop+=(temp)*CLOCK_16M_SYS_TIMER_CLK_1US;
		wakeup_next_tick=clock_time()|1;	
		 ui_loop_24g(temp);
	}

	d24g_rf_loop();

	pm_poll();
}
