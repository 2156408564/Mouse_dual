/********************************************************************************************************
 * @file     AAA_24G_RF.c
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

#include "aaa_public_config.h"

#define  RF_TX_TO_RX_AUTO_MODE_ENABLE  1

#define  RF_FAST_TX_SETTING_ENABLE  0
#define  RF_FAST_RX_SETTING_ENABLE  0
#if(CLOCK_SYS_CLOCK_HZ == 32000000 || CLOCK_SYS_CLOCK_HZ == 24000000)
    #define			SHIFT_US		5
    #elif(CLOCK_SYS_CLOCK_HZ == 16000000 || CLOCK_SYS_CLOCK_HZ == 12000000)
    #define			SHIFT_US		4
    #elif(CLOCK_SYS_CLOCK_HZ == 8000000)
    #define			SHIFT_US		3
#else
   #define			SHIFT_US		6
#endif

task_when_rf_func p_task_when_rf = NULL;

#define  RX_FRAME_SIZE     64
volatile unsigned char  rx_packet[RX_FRAME_SIZE]  __attribute__((aligned(4)));
volatile unsigned char ack_payload[RX_FRAME_SIZE];
volatile unsigned int  rf_state = 0;

#if TEST_LOST_RATE
    volatile unsigned int  rf_rx_cnt = 0;
    volatile unsigned int  rf_rx_timeout_cnt = 0;
    volatile unsigned int  user_rx_timeout_cnt = 0;
    volatile unsigned int	user_rx_right_data_cnt = 0;
#endif
extern volatile unsigned int rf_rx_timeout_us;

 u8 chn_mask = 0x80;

#ifndef			CHANNEL_SLOT_TIME
    #define			CHANNEL_SLOT_TIME			8000
#endif

#ifndef		PKT_BUFF_SIZE
    #define		PKT_BUFF_SIZE		64
#endif



 u8			device_channel = 0;
 u8			ll_chn_sel;
 u8			ll_rssi;

 u16		ll_chn_tick;
 u32		ll_chn_rx_tick;
 u32		ll_chn_mask = LL_CHANNEL_SEARCH_FLAG;
 u32		ll_clock_time;
 int		device_packet_received;

 volatile int	device_ack_received = 0;

 int		device_sync = 0;
//-------------------- rf init------------------
#define FRE_OFFSET 	0
#define FRE_STEP 	5
#define MAX_RF_CHANNEL  16
const unsigned char rf_chn[MAX_RF_CHANNEL] =
{
    FRE_OFFSET + 5, FRE_OFFSET + 9, FRE_OFFSET + 13, FRE_OFFSET + 17,
    FRE_OFFSET + 22, FRE_OFFSET + 26, FRE_OFFSET + 30, FRE_OFFSET + 35,
    FRE_OFFSET + 40, FRE_OFFSET + 45, FRE_OFFSET + 50, FRE_OFFSET + 55,
    FRE_OFFSET + 60, FRE_OFFSET + 65, FRE_OFFSET + 70, FRE_OFFSET + 76,
};


volatile int rf_ca_cnt=0;
#if RF_FAST_RX_SETTING_ENABLE
_attribute_ram_code_sec_ static inline void rf_rx_fast_settle_init()
{
		
	{
		write_reg8(0x1284,(read_reg8(0x1284)&0x87)|0x37);
		//write_reg8(0x1284,(read_reg8(0x1284))|0x37);
			//rx timing sequence
		write_reg8(0x12b0,0x00);
		write_reg8(0x12b1,read_reg8(0x12b1)&0xfe);
		write_reg8(0x12b2,0x10);
		write_reg8(0x12b3,read_reg8(0x12b3)&0xfe);
		write_reg8(0x12b4,0x10);
		write_reg8(0x12b5,read_reg8(0x12b5)&0xfe);
		write_reg8(0x12b6,0x32);
		write_reg8(0x12b7,read_reg8(0x12b7)&0xfe);
		write_reg8(0x12b8,0x58);
		write_reg8(0x12b9,read_reg8(0x12b9)&0xfe);
		write_reg8(0x12ba,0x58 );
		write_reg8(0x12bb,0x00);
	}

}
_attribute_ram_code_sec_ static inline void rf_rx_fast_settle_en(void)
{
	write_reg8(0x1229,read_reg8(0x1229)|0x01);
}
_attribute_ram_code_sec_ void rf_rx_fast_settle_dis(void)
{
	write_reg8(0x1229,read_reg8(0x1229)&0xfe);
}
#endif

#if RF_FAST_TX_SETTING_ENABLE
_attribute_ram_code_sec_ static inline void rf_tx_fast_settle_init()
{

	{
		write_reg8(0x1284,(read_reg8(0x1284)&0xf8)|0x02);

		//write_reg8(0x1284,(read_reg8(0x1284))|0x02);

		
		write_reg8(0x12a4,0x00);
		write_reg8(0x12a5,read_reg8(0x12a5)&0xfe);
		write_reg8(0x12a6,0x10);
		write_reg8(0x12a7,read_reg8(0x12a7)&0xfe);
		write_reg8(0x12a8,0x62);
		write_reg8(0x12a9,read_reg8(0x12a9)&0xfe);
		write_reg8(0x12aa,0x64);
		write_reg8(0x12ab,read_reg8(0x12ab)&0xfe);
		write_reg8(0x12ac,0x6a);
		write_reg8(0x12ad,read_reg8(0x12ad)&0xfe);
		write_reg8(0x12ae,0x6a);
		write_reg8(0x12af,read_reg8(0x12af)&0xfe);
		write_reg8(0x12bc,0x62);
		write_reg8(0x12bd,read_reg8(0x12bd)&0xfe);
	}
}
_attribute_ram_code_sec_ static inline void rf_tx_fast_settle_en(void)
{
	write_reg8(0x1229,read_reg8(0x1229)|0x02);
}
_attribute_ram_code_sec_ void rf_tx_fast_settle_dis(void)
{
	write_reg8(0x1229,read_reg8(0x1229)&0xfd);
}
#endif

_attribute_ram_code_sec_ void rf_drv_private_2m_init()
{
	rf_private_2m_reg_init();
//// add end

    write_reg16(0xf06, 0);//rx wait time
    write_reg8(0x0f0c, 0x54); //rx settle time: Default 150us, minimum 85us(0x54+1)

    write_reg16(0xf0e, 0);//tx wait time
    write_reg8(0x0f04, 0x70);//tx settle time: Default 150us, minimum 113us(0x70+1)

    write_reg8(0xf10, 0);// wait time on NAK


#if 1//FREQUENY_HOPPING_1K	
		rf_ca_cnt=0;
	#if RF_FAST_RX_SETTING_ENABLE	//must fist  call rx fast settle init
			rf_rx_fast_settle_init();
			rf_rx_fast_settle_dis();
	#endif


	#if RF_FAST_TX_SETTING_ENABLE	
		rf_tx_fast_settle_init();
		rf_tx_fast_settle_dis();
	#endif
#endif

    rf_rx_buffer_set((u8 *)rx_packet, RX_FRAME_SIZE, 0);

    write_reg8(0x405, (read_reg8(0x405)&0xf8) |5); //access_byte_num[2:0]
    write_reg8(0x420, 35);
    rf_set_tx_rx_off();
    rf_rx_acc_code_enable(0x01);
    rf_tx_acc_code_select(0);
    irq_disable();
    irq_clr_src();
    rf_irq_src_allclr();
    irq_enable_type(FLD_IRQ_ZB_RT_EN); //enable RF irq
    rf_irq_disable(FLD_RF_IRQ_ALL);
    rf_irq_enable(FLD_RF_IRQ_TX | FLD_RF_IRQ_RX | FLD_RF_IRQ_RX_TIMEOUT|FLD_RF_IRQ_FIRST_TIMEOUT); //
	irq_enable();
}

_attribute_ram_code_ void rf_private_2m_reg_init()
{
    //tbl_rf_init
    write_reg8(0x12d2, 0x9b);//DCOC_DBG0
    write_reg8(0x12d3, 0x19); //DCOC_DBG1
    write_reg8(0x127b, 0x0e); //BYPASS_FILT_1
    write_reg8(0x1276, 0x50); //FREQ_CORR_CFG2_0
    write_reg8(0x1277, 0x73); //FREQ_CORR_CFG2_1

    //tbl_rf_private_2m
    write_reg8(0x1220, 0x04);//SC_CODE
    write_reg8(0x1221, 0x2a);//IF_FREQ
    write_reg8(0x1222, 0x43);//HPMC_EXP_DIFF_COUNT_L
    write_reg8(0x1223, 0x06);//HPMC_EXP_DIFF_COUNT_H
    write_reg8(0x1254, 0x0e); //AGC_THRSHLD1_2M_0
    write_reg8(0x1255, 0x09); //AGC_THRSHLD1_2M_1
    write_reg8(0x1256, 0x0c); //AGC_THRSHLD2_2M_0
    write_reg8(0x1257, 0x08); //AGC_THRSHLD2_2M_1
    write_reg8(0x1258, 0x09); //AGC_THRSHLD3_2M_0
    write_reg8(0x1259, 0x0f); //AGC_THRSHLD3_2M_1

    write_reg8(0x400, 0x1f);//tx mode
    write_reg8(0x402, 0x88);//preamble length
    write_reg8(0x404, 0xca);//nordic head
    write_reg8(0x421, 0xa1);//len_0_en
    write_reg8(0x430, 0x3e);//<1>hd_timestamp
    //AGC table
    write_reg8(0x460, 0x36);//grx_0
    write_reg8(0x461, 0x46);//grx_1
    write_reg8(0x462, 0x51);//grx_2
    write_reg8(0x463, 0x61);//grx_3
    write_reg8(0x464, 0x6d);//grx_4
    write_reg8(0x465, 0x78);//grx_5


	reg_dma_chn_en |= FLD_DMA_CHN_RF_RX  | FLD_DMA_CHN_RF_TX;

}


_attribute_ram_code_sec_  void my_rf_start_srx  (unsigned int tick,unsigned int rx_timeout)
{
    write_reg8 (0x800f00, 0x80);						// disable srx
	write_reg32 (0x800f28, rx_timeout-1);					// first timeout
	write_reg32(0x800f18, tick);
    write_reg8(0x800f16, read_reg8(0x800f16) | 0x04);	// Enable cmd_schedule mode
	write_reg16 (0x800f00, 0x3f86);						// srx
}
void my_rf_start_stx  (void* addr, unsigned int tick)
{
	write_reg32(0x800f18, tick);
    write_reg8(0x800f16, read_reg8(0x800f16) | 0x04);	// Enable cmd_schedule mode
	write_reg8 (0x800f00, 0x85);						// single TX
	write_reg16 (0x800c0c, (unsigned short)((unsigned int)addr));
}

static inline void rf_start_stxtorx(void *addr, unsigned int tick, unsigned int timeout_us)
{
    write_reg16(0x800f0a, timeout_us - 1);
    write_reg32(0x800f18, tick);
    write_reg8(0x800f16, read_reg8(0x800f16) | 0x04);	 // Enable cmd_schedule mode
    write_reg16(0x800c0c, (unsigned short)((unsigned int)addr));
    write_reg8(0x800f00, 0x87);	   // single tx2rx
}



void set_pair_access_code(u32 code)
{
    write_reg32(0x800408, ((code & 0xffffff00) | 0x71));
    write_reg8(0x80040c, code);
}
void set_data_access_code(u32 code)
{
    write_reg32(0x800408, ((code & 0xffffff00) | 0x77));
    write_reg8(0x80040c, code);
}


void rf_irq_src_allclr()
{
    reg_rf_irq_status = 0xffff;
}

u8  Read_Payload(u8 *payload)
{
    u8 *rx_pkt;
    u8 ret, i;

    rx_pkt = (u8 *) rx_packet ;
    rf_rx_buffer_set((u8 *)rx_packet, RX_FRAME_SIZE, 0);
    ret = rx_pkt[4] & 0x3f; //get length of rx paylaod

    for (i = 0; i < (ret + 5); i++)
    {
        payload[i] = rx_pkt[i];
    }
    return 1;
}

_attribute_ram_code_sec_ void irq_handle_private_2m()
{
	
	unsigned short rf_irq_src = rf_irq_src_get();
    if(rf_irq_src)
    {
		if (rf_irq_src & FLD_RF_IRQ_RX)
		{
			PIN_DEBUG_RF_RX_LEVEL(0);
			rf_irq_clr_src(FLD_RF_IRQ_RX);
			rf_state = RF_RX_END_STATUS;
		}

		if (rf_irq_src & FLD_RF_IRQ_TX)
		{
			PIN_DEBUG_RF_TX_LEVEL(0);
			rf_irq_clr_src(FLD_RF_IRQ_TX);
			rf_state = RF_RX_START_STATUS;
			
		#if (RF_TX_TO_RX_AUTO_MODE_ENABLE==0)			
			my_rf_start_srx(clock_time(), rf_rx_timeout_us);
		#endif
		}
#if (RF_TX_TO_RX_AUTO_MODE_ENABLE==1)


	 	if (rf_irq_src & FLD_RF_IRQ_RX_TIMEOUT)
		 {
		 	PIN_DEBUG_RF_RX_TIMEOUT_LEVEL(0);	
			rf_irq_clr_src(FLD_RF_IRQ_RX_TIMEOUT);
			rf_state = RF_RX_TIMEOUT_STATUS;
		}
#else
	 	if (rf_irq_src & FLD_RF_IRQ_FIRST_TIMEOUT)
		 {
		 	PIN_DEBUG_RF_RX_TIMEOUT_LEVEL(0);	
			rf_irq_clr_src(FLD_RF_IRQ_FIRST_TIMEOUT);
			rf_state = RF_RX_TIMEOUT_STATUS;
		}
#endif
		irq_clr_src();
    }
}


_attribute_ram_code_sec_  u8 get_next_channel_with_mask(u32 mask, u8 chn)
{
	PIN_DEBUG_RF_CHN_NEXT_TOGGLE;	
#if (HOPING_METHOD==1)
    u8 temp_chn;
	temp_chn=(chn+3)%15;
	return temp_chn;
#else
	int chn_high = (mask >> 4) & 0x0f;

	if (mask & LL_CHANNEL_SEARCH_FLAG) {
		return LL_NEXT_CHANNEL (chn);
	}
	else if (chn_high != chn) {
		ll_chn_sel = 1;
		return chn_high;
	}
	else {
		ll_chn_sel = 0;
		return mask & 0x0f;
	}
#endif

}

//rx_timeout_us  :Determined by the maximum packet time of ack
//must >=200us
_attribute_ram_code_sec_ void rf_stx_to_rx(u8 *p,u32 rx_timeout_us)
{
#if (RF_FAST_TX_SETTING_ENABLE||RF_FAST_TX_SETTING_ENABLE)
	if(rf_ca_cnt==0)
	{
		#if RF_FAST_TX_SETTING_ENABLE
			write_reg8(0x0f04, 0x70);//tx settle time: Default 150us, minimum 113us(0x70+1)
			rf_tx_fast_settle_dis();
		#endif
		
		#if RF_FAST_RX_SETTING_ENABLE
		
			write_reg8(0x0f0c, 0x54); //rx settle time: Default 150us, minimum 85us(0x54+1)
			rf_rx_fast_settle_dis();
		#endif

	}
	else if(rf_ca_cnt==1)
	{
		#if RF_FAST_TX_SETTING_ENABLE
			write_reg8(0x0f04, 53);//tx settle time: Default 150us, minimum 113us(0x70+1)
		
			rf_tx_fast_settle_en();
		#endif
	
		#if RF_FAST_RX_SETTING_ENABLE

			write_reg8(0x0f0c, 44); //rx settle time: Default 150us, minimum 85us(0x54+1)
		
			rf_rx_fast_settle_en();
		#endif
	}
		rf_ca_cnt=(rf_ca_cnt+1)%8;
#endif
		PIN_DEBUG_RF_TX_LEVEL(1);
		PIN_DEBUG_RF_RX_LEVEL(1);
		PIN_DEBUG_RF_RX_TIMEOUT_LEVEL(1);

    #if RF_TX_TO_RX_AUTO_MODE_ENABLE
		rf_trx_state_set(RF_MODE_AUTO, rf_chn[device_channel]);
		rf_start_stxtorx(p, ClockTime(), rx_timeout_us);
	#else
		rf_set_channel(rf_chn[device_channel], 0);
	  
    	my_rf_start_stx(p, ClockTime());
	#endif
}

extern u8  rf_rx_process(rf_packet_t *p_pkt);

_attribute_ram_code_sec_ void irq_device_rx(void)
{
	if(RF_NRF_ESB_PACKET_CRC_OK(rx_packet)&&RF_NRF_ESB_PACKET_LENGTH_OK(rx_packet))
	{
	    rf_packet_t *p = (rf_packet_t *)(rx_packet);
    if (rf_rx_process(p) /*&& ll_chn_tick != p->tick*/)
    {
	        //ll_chn_tick = p->tick;	

	       // device_sync = 1;
        device_ack_received = 1;
	      
	        //ll_chn_rx_tick = clock_time();

#if TEST_LOST_RATE
        user_rx_right_data_cnt++;
#endif
    }
	}
	rx_packet[0]=1;
}

