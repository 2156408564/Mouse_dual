/********************************************************************************************************
 * @file     aaa_sensor.c
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
#if SENSOR_FUN_ENABLE_AAA




#define sif_spi_clk_low    gpio_write(PIN_SIF_SCL, 0)
#define sif_spi_clk_high   gpio_write(PIN_SIF_SCL, 1)

#define sif_spi_sda_low    gpio_write(PIN_SIF_SDA, 0)
#define sif_spi_sda_high   gpio_write(PIN_SIF_SDA, 1)
#define get_spi_sda_vaule  gpio_read(PIN_SIF_SDA)

#define sif_spi_sda_output_enable    gpio_set_output_en(PIN_SIF_SDA, 1)
#define sif_spi_sda_output_disable   gpio_set_output_en(PIN_SIF_SDA, 0)

#define sif_spi_sda_input_enable    gpio_set_input_en(PIN_SIF_SDA, 1)
#define sif_spi_sda_input_disable   gpio_set_input_en(PIN_SIF_SDA, 0)
#if SENSOR_CS_ENABLE

    #define sif_spi_cs_high	 gpio_write(PIN_SENSOR_CS,1)
    #define sif_spi_cs_low   gpio_write(PIN_SENSOR_CS,0)
    #define CS_DELAY        sleep_us(2)
#else
    #define sif_spi_cs_high
    #define sif_spi_cs_low
    #define CS_DELAY
#endif


//CLOCK_DLY_6_CYC   370khz
#define		DLY_200NS    CLOCK_DLY_6_CYC//sleep_us(5)//asm("tnop");asm("tnop")
#define		DLY_100NS    CLOCK_DLY_10_CYC//sleep_us(5)//asm("tnop")

#define		SENSOR_RECOVER_FAIL				1

////////////////////////////////////////////////////////////////////////////
// serial interface function
////////////////////////////////////////////////////////////////////////////


_attribute_data_retention_user u16	check_spi_counter;
_attribute_data_retention_user u8	check_resync_counter;
_attribute_data_retention_user u8	product_id1=0;
_attribute_data_retention_user u8	product_id2=0;
_attribute_data_retention_user u8	 product_id3=0;
//u8  reg_0d;
_attribute_data_retention_user  u8	sensor_type = 0xff;

_attribute_data_retention_user u8   mouse_cust_fct3065xy = 0;
_attribute_data_retention_user u8   dbg_sensor_cpi=0;

u8 dpi_max_level=3;


void sensor_set_wakeup_level_suspend(u8 enable)
{
#if 0
    if (gpio_read(PIN_SIF_MOTION))
    {
        cpu_set_gpio_wakeup(PIN_SIF_MOTION, 0, enable); //low wakeup suspend
    }
    else
    {
        cpu_set_gpio_wakeup(PIN_SIF_MOTION, 1, enable);
    }
#else
    cpu_set_gpio_wakeup(PIN_SIF_MOTION, 0, enable); //low wakeup suspend
#endif
}
void sensor_set_wakeup_level_deepsleep(u8 enable)
{

#if 0
    if (gpio_read(PIN_SIF_MOTION))
    {
        cpu_set_gpio_wakeup(PIN_SIF_MOTION, 0, enable); //low wakeup suspend

    }
    else
    {
        cpu_set_gpio_wakeup(PIN_SIF_MOTION, 1, enable);

    }
#else
    cpu_set_gpio_wakeup(PIN_SIF_MOTION, 0, enable); //low wakeup suspend
#endif


}

#if 0
void sensor_direct_check()
{
	gpio_write(PIN_SIF_SCL, 0);
	gpio_set_input_en(PIN_SIF_SCL, 1);
    gpio_set_output_en(PIN_SIF_SCL, 0);
    gpio_setup_up_down_resistor(PIN_SIF_SCL, PM_PIN_PULLDOWN_100K);
	sleep_us(10);
    if(gpio_read(PIN_SIF_SCL))
    {
		sensor_direct=SENSOR_DIRECTION_CLOCK_12;
}
	else
	{
		sensor_direct=SENSOR_DIRECTION_CLOCK_3;
	}
	sleep_us(10);
}
#endif
void sif_init(void)
{
    //init in mouse.h
#if 0
    gpio_set_input_en(PIN_SIF_SCL, 0);
    gpio_set_output_en(PIN_SIF_SCL, 1);
    gpio_setup_up_down_resistor(PIN_SIF_SCL, PM_PIN_PULLUP_1M);
    gpio_write(PIN_SIF_SCL, 1);

    gpio_set_input_en(PIN_SIF_SDA, 1);
    gpio_set_output_en(PIN_SIF_SDA, 1);
    gpio_setup_up_down_resistor(PIN_SIF_SDA, PM_PIN_PULLUP_10K);
	gpio_write(PIN_SIF_SDA, 1);


    gpio_set_input_en(PIN_SIF_MOTION, 1);
    gpio_set_output_en(PIN_SIF_MOTION, 0);
	gpio_write(PIN_SIF_MOTION, 1);
	gpio_setup_up_down_resistor(PIN_SIF_MOTION, PM_PIN_PULLUP_1M);
#if(BOARD_TYPE==QFN24_BOARD)
    gpio_setup_up_down_resistor(PIN_SIF_MOTION, PM_PIN_PULLUP_10K);

#elif(BOARD_TYPE==QFN32_BOARD)
    gpio_setup_up_down_resistor(PIN_SIF_MOTION, PM_PIN_PULLUP_1M);
#endif

    //sensor_set_wakeup_level_suspend();
#if SENSOR_CS_ENABLE
    gpio_set_input_en(PIN_SENSOR_CS, 0);
    gpio_set_output_en(PIN_SENSOR_CS, 1);
    gpio_setup_up_down_resistor(PIN_SENSOR_CS, PM_PIN_PULLUP_1M);
    gpio_write(PIN_SENSOR_CS, 1);
#endif
#else
#if 0
    if (deep_flag)
    {
        gpio_setup_up_down_resistor(PIN_SIF_SCL, PM_PIN_PULLUP_1M);
        gpio_setup_up_down_resistor(PIN_SIF_SDA, PM_PIN_PULLUP_10K);
        gpio_setup_up_down_resistor(PIN_SIF_MOTION, PM_PIN_PULLUP_1M);
    }
#endif
#endif
}
void sensor_gpio_powerDownConfig()
{
    gpio_setup_up_down_resistor(PIN_SIF_SCL, PM_PIN_PULLUP_1M);
    if (gpio_read(PIN_SIF_SDA))
    {
        gpio_setup_up_down_resistor(PIN_SIF_SDA, PM_PIN_PULLUP_1M);
    }
    else
    {
        gpio_setup_up_down_resistor(PIN_SIF_SDA, PM_PIN_PULLDOWN_100K);
    }

#if 0
    if (!gpio_read(PIN_SIF_MOTION))
    {
        gpio_setup_up_down_resistor(PIN_SIF_MOTION, PM_PIN_PULLDOWN_100K);
    }
#endif
}


_attribute_ram_code_ void sif_SendByte(u8 data)
{
    u8 i = 0;
    u8 buf = data;

    for (i = 0; i < 8; i++)
    {
        sif_spi_clk_low;
        gpio_write(PIN_SIF_SDA, buf & 0x80);
        DLY_100NS;


        sif_spi_clk_high;
        buf <<= 1;
        DLY_100NS;
    }
}

_attribute_ram_code_ u8 sif_ReadByte()
{
    u8 i = 0;
    u8 dat = 0;

    for (i = 0; i < 8; i++)
    {
        sif_spi_clk_low;
        dat <<= 1;
        DLY_100NS;

        sif_spi_clk_high;
        DLY_100NS;
        if (get_spi_sda_vaule)
        {
            dat |= 1;
        }
    }
    return dat;
}


_attribute_ram_code_ u8 I2C_PAN3204LL_ReadRegister(u8 cAddr)
{

    u8 dat = 0;


    //u8 r = irq_disable();
    sif_spi_sda_output_enable;
    sif_spi_sda_input_disable;
    sif_spi_cs_low;

    CS_DELAY;

    sif_SendByte(cAddr & 0x7f);

    sif_spi_sda_output_disable;// delay 5us
    sif_spi_sda_input_enable;  // delay 15us

    //WaitUs(5);       //this delay is necessary

    dat = sif_ReadByte();
	
    CS_DELAY;
    sif_spi_cs_high;

    //irq_restore(r);

    return dat;
}

_attribute_ram_code_  void I2C_PAN3204LL_WriteRegister(u8 cAddr, u8 cData)
{
   // u8 r = irq_disable();
    gpio_set_output_en(PIN_SIF_SDA, 1);
    gpio_set_input_en(PIN_SIF_SDA, 0);
    sif_spi_cs_low;
    CS_DELAY;
    sif_SendByte((cAddr | 0x80));
    //WaitUs(1);
    sif_SendByte(cData);

    CS_DELAY;
    sif_spi_cs_high;

    gpio_set_output_en(PIN_SIF_SDA, 0);
    gpio_set_input_en(PIN_SIF_SDA, 1);


   // irq_restore(r);

}


//----------------------------------------------------------------------
// FUNCTION NAME: Sensor3204LL_Optimization_Setting
//
// DESCRIPTION:
//      Do the optimization of the sensor PAN3204LL to consume less current.
//      Also, if need, set a better tracking performance on Critical surfaces.
//
//----------------------------------------------------------------------
void DownloadConfigTable(const unsigned char *ptbl, unsigned int len)
{
    unsigned int i, addr;

    for (i = 0; i < len; i += 2)
    {
        addr = ptbl[0];
        I2C_PAN3204LL_WriteRegister(addr, ptbl[1]);
        ptbl += 2;
    }
}



const unsigned char Config_3205_TJ3T[] = {
	0x09, 0x5A,
	0x0D, 0x0A,
	0x1B, 0x35,
    0x1D, 0xDB,

	0x28, 0xB4,
	0x29, 0x46,
	0x2A, 0x96,
	0x2B, 0x8C,
	0x2C, 0x6E,
	0x2D, 0x64,
	0x38, 0x5F,
	0x39, 0x0F,
	0x3A, 0x32,
	0x3B, 0x47,
	0x42, 0x10,

	0x43, 0x09,
	0x54, 0x2E,
	0x55, 0xF2,
	0x61, 0xF4,
	0x63, 0x70,
	0x75, 0x52,
	0x76, 0x41,
	0x77, 0xED,
	0x78, 0x23,
	0x79, 0x46,
	0x7A, 0xE5,
	0x7C, 0x48,

    0x7D, 0x80,
	0x7E, 0x77,

	0x7F, 0x01,
	0x0B, 0x00,

	0x7F, 0x00,
	0x09, 0x00,
};

const unsigned char Config_VT108[] = {		//KA8
	0x09,  0x5a,
	0x0d,  0x12,
	0x0e,  0xc5,
	0x09,  0x00,
};

const unsigned char Config_8650[] =
{
    0x09,  0x5a,
    0x06,  0xa0,
    //0x26,  0x34,
    0x09,  0x00,
};

const unsigned char Config_3212[] = {
	0x09,  0x5a,
	0x06,  0xa0,
	0x26,  0x34,
#if SENSOR_DATA_LENGTH_12_BIT_ENABLE
	0x19,  0x04, //new
#endif
	0x09,  0x00,
};

const unsigned char Config_YS8009[] = {
	0x06,  0x03,
	0x09,  0x5a,
	0x0b,  0x90,
	0x09,  0x69,
	0x0d,  0x48,
	0x0e,  0x9f,
	0x0f,  0xba,
	//0x16,  0xbd,
	0x17,  0x08,
	//0x22,  0x11,
	0x09,  0x00,
};
typedef struct
{
    const unsigned char *ptbl;
    unsigned int len;
} SENSOR_OPT;


void sensor_resync()
{
	gpio_write(PIN_SIF_SCL, 1);
    WaitUs(3);
    gpio_write(PIN_SIF_SCL, 0);

    if ((sensor_type==SENSOR_3065)||(sensor_type==SENSOR_3065XY))
    {
        WaitUs(10000);				//FCT3065-XY SCL Low 10ms, then high 512ms
        gpio_write(PIN_SIF_SCL, 1);
        WaitUs(40000);
    }
    else
    {
		WaitUs(5);
		gpio_write(PIN_SIF_SCL, 1);
		WaitUs(30000);
    }
}


u8 sensor_check_id()
{
	u8 reg0=0;
	reg0 = I2C_PAN3204LL_ReadRegister(REG_PAN3204LL_PRODUCT_ID1);
	if((reg0!=0x30)&&(reg0!=0x31)&&(reg0!=0x58)&&(reg0!=0x18))
	{
		sensor_resync();
		return 0;
	}
	return 1;
}

int OPTSensor_resync(u32 retry)
{
	//return 0;
	for(u16 i=0;i<retry;i++)
	{
	#if (MODULE_WATCHDOG_ENABLE)
        wd_clear();
	#endif
		if(sensor_check_id())
		{
			return 1;
		}
	}
	return 0;
}

static int sensor_type_identify()
{
	int sensor_type = SENSOR_3205_TJNT;
	if(product_id1==0x18)
	{
		return SENSOR_OM18;
	}
	else if((product_id1==0x58)&&(product_id2==0x59))
	{
		return SENSOR_KA8G2;
	}
	else if((product_id1==0x30))
	{
		if((product_id2)==0x02)
		{
			//mouse_3212_sensor=1;
			return SENSOR_PAW3212;
		}
		else if((product_id2&0xf0)==0x50)
		{
			if(product_id2==0x54)
			{
				return SENSOR_VT108;
			}
			else
			{
				return SENSOR_M8650A;
			}
		}
		else if((product_id2&0xf0)==0xd0)//
		{
			
			if(product_id2==0xd8)
			{
				if (I2C_PAN3204LL_ReadRegister(0x1B) == 0x1D)
	            {
	                return SENSOR_YS8009;
	            }
				
			}
			else if(product_id2==0xd1)
			{
				if (I2C_PAN3204LL_ReadRegister(0x49) == 0xa1)
	            {
	                return SENSOR_S201B;
	            }				
			}			
			return SENSOR_3205_TJ3T;
			
		}
	}
	else if((product_id1==0x31))
	{
		if((product_id2)==0x00)
		{
			return SENSOR_3065;
		}
		else if((product_id2)==0x70)
		{
			return SENSOR_3065XY;
		}
		
	}
	
	return  sensor_type;
}

void om18_Optimization_Setting()
{
	u8 var1,var2,var3;
	I2C_PAN3204LL_WriteRegister(0x09,0x5a);

	I2C_PAN3204LL_WriteRegister(0x62,0x63);
	I2C_PAN3204LL_WriteRegister(0x6e,0x63);
	I2C_PAN3204LL_WriteRegister(0x6f,0x63);
	I2C_PAN3204LL_WriteRegister(0x70,0x63);
	I2C_PAN3204LL_WriteRegister(0x71,0x63);
	I2C_PAN3204LL_WriteRegister(0x56,0x08);
	var1=I2C_PAN3204LL_ReadRegister(0x43)&0x0f;
	var2=I2C_PAN3204LL_ReadRegister(0x45)&0x7f;
	var3=I2C_PAN3204LL_ReadRegister(0x46)&0x7f;

	I2C_PAN3204LL_WriteRegister(0x43,var1);
	I2C_PAN3204LL_WriteRegister(0x45,var2);
	I2C_PAN3204LL_WriteRegister(0x46,var3);
	I2C_PAN3204LL_WriteRegister(0x56,0);
#if SENSOR_DATA_LENGTH_12_BIT_ENABLE
	I2C_PAN3204LL_WriteRegister(0x19,0x14); //14
#else
	I2C_PAN3204LL_WriteRegister(0x19,0x10);
#endif
	I2C_PAN3204LL_WriteRegister(0x09,0x00);
}

void KA8G2_Optimization_Setting()
{
	I2C_PAN3204LL_WriteRegister(0x09,0xA5);
	I2C_PAN3204LL_WriteRegister(0x46,0x34); // ???? spi ??????
	I2C_PAN3204LL_WriteRegister(0x19,0x00);
	I2C_PAN3204LL_WriteRegister(0x60,0x07);
	I2C_PAN3204LL_WriteRegister(0x69,0x04);
	I2C_PAN3204LL_WriteRegister(0x7D,0x20);
	I2C_PAN3204LL_WriteRegister(0x7E,0x00);
	I2C_PAN3204LL_WriteRegister(0x09,0x00);
	I2C_PAN3204LL_WriteRegister(0x0D,0x1B);
	I2C_PAN3204LL_WriteRegister(0x0E,0x1B);

	I2C_PAN3204LL_WriteRegister(0x6E,0x1c);  //disable 
	//I2C_PAN3204LL_WriteRegister(0x6E,0x14);// eanble inter Internal resistance

	I2C_PAN3204LL_WriteRegister(0x09,0x00);
}

//called when power on, or mouse waked up from deep-sleep
unsigned int OPTSensor_Init(unsigned int poweron)
{
	// Do the full chip reset.
	u8 reg6=0;
	// sensor_type = analog_read(DEEP_ANA_REG6);

	sif_init();
	if(OPTSensor_resync(33) == 0)	return 0;
	Sensor3204_Wakeup(sensor_type);
	reg6=I2C_PAN3204LL_ReadRegister(REG_PAN3204LL_CONFIGURATION)|0x80;
	I2C_PAN3204LL_WriteRegister(REG_PAN3204LL_CONFIGURATION, reg6);
	WaitUs(25000);

	//get sensor id
	product_id1 = I2C_PAN3204LL_ReadRegister(REG_PAN3204LL_PRODUCT_ID1);    //power-on must re-read product_id1, or it would make mistake
	product_id2 = I2C_PAN3204LL_ReadRegister(REG_PAN3204LL_PRODUCT_ID2);    
	// product_id3 = I2C_PAN3204LL_ReadRegister(0x60);				//
	sensor_type = sensor_type_identify();
	printf("---sensor_type=%d.\n",sensor_type);
	// if(poweron)
	{
		if(sensor_type==SENSOR_3205_TJNT)
		{
			//DownloadConfigTable(Config_3205,sizeof(Config_3205));
		}
		else if(sensor_type==SENSOR_3205_TJ3T)
		{
			DownloadConfigTable(Config_3205_TJ3T,sizeof(Config_3205_TJ3T));
		}
		else if(sensor_type==SENSOR_3065)
		{
			//DownloadConfigTable(Config_AN3205,sizeof(Config_AN3205));
		}
		else if(sensor_type==SENSOR_VT108)
		{
			DownloadConfigTable(Config_VT108,sizeof(Config_VT108));
		}
		else if(sensor_type==SENSOR_M8650A)
		{
            DownloadConfigTable(Config_8650, sizeof(Config_8650));
		}
		else if(sensor_type==SENSOR_OM18)
		{
			om18_Optimization_Setting();
		}
		else if(sensor_type==SENSOR_YS8009)
		{
			DownloadConfigTable(Config_YS8009, sizeof(Config_YS8009));
		}
		else if(sensor_type==SENSOR_KA8G2)
		{
			KA8G2_Optimization_Setting();
		}
		else if(sensor_type==SENSOR_PAW3212)
		{
			DownloadConfigTable(Config_3212, sizeof(Config_3212));
		}
		else if(sensor_type==SENSOR_S201B)
		{
			//SensorYS8008_Optimization_Setting();
		}
	}
	
	return sensor_type;
}

unsigned int OPTSensor_motion_report( u32 no_overflow )
{
	unsigned int optical_status = 0;
	unsigned int reg_x, reg_y, reg_X_Y_H;

	//sensor ph5205 no overflow read

	if(( sensor_type==SENSOR_PAW3212)||( sensor_type==SENSOR_OM18)||( sensor_type==SENSOR_KA8G2))
	{
		no_overflow=1;
	}
	
	ms_data.x = 0;
	ms_data.y = 0;
	
	if (OPTSensor_resync(33)==0)
	{
		return 0;
	}
	
 #if 0
    if (gpio_read(PIN_SIF_MOTION))
    {
        return 0;
    }
#endif

	optical_status = I2C_PAN3204LL_ReadRegister(REG_PAN3204LL_MOTION_STATUS);
	if((optical_status & MOTION_STATUS_MOT)  || (no_overflow && ((optical_status & MOTION_STATUS_DXOVF) || (optical_status & MOTION_STATUS_DYOVF))))
	{
	#if SENSOR_DATA_LENGTH_12_BIT_ENABLE
		if((sensor_type==SENSOR_OM18)||(sensor_type==SENSOR_PAW3212))
		{
			reg_X_Y_H = I2C_PAN3204LL_ReadRegister(REG_PAN3204LL_DELTA_XY_H);
			reg_x = I2C_PAN3204LL_ReadRegister(REG_PAN3204LL_DELTA_X)|((reg_X_Y_H&0xF0)<<4);  // DIRECTION_CLOCK_12 X
			reg_y = I2C_PAN3204LL_ReadRegister(REG_PAN3204LL_DELTA_Y)|((reg_X_Y_H&0x0F)<<8);  // DIRECTION_CLOCK_12 Y
		#if 0
			if(reg_x==0x800)
				reg_x = 0x801;
			if(reg_y==0x800)
				reg_y = 0x801;
			
			if( !no_overflow ){			
				if(optical_status & MOTION_STATUS_DXOVF)
					reg_x = ((signed char)reg_x>=0)? 0x801:0x7ff;   //x overflow, current<0,0x7f, current>0, 0xff

				if(optical_status & MOTION_STATUS_DYOVF)
					reg_y = ((signed char)reg_y>=0)? 0x801:0x7ff;   //y overflow, current<0,0x7f, current>0, 0xff
			}
		#endif
			if(reg_x&0x0800)
			{
				ms_data.x=0xf000|reg_x;
			}
			else
			{
				ms_data.x=reg_x;
			}

			if(reg_y&0x0800)
			{
				ms_data.y=0xf000|reg_y;
			}
			else
			{
				ms_data.y=reg_y;
			}
		}
		else
	#endif
		{
			reg_x = I2C_PAN3204LL_ReadRegister(REG_PAN3204LL_DELTA_X);  // DIRECTION_CLOCK_12 X
			reg_y = I2C_PAN3204LL_ReadRegister(REG_PAN3204LL_DELTA_Y);  // DIRECTION_CLOCK_12 Y

			if(reg_x==0x80)
				reg_x = 0x81;
			if(reg_y==0x80)
				reg_y = 0x81;

			if( !no_overflow ){			
				if(optical_status & MOTION_STATUS_DXOVF)
					reg_x = ((signed char)reg_x>=0)? 0x81:0x7f;   //x overflow, current<0,0x7f, current>0, 0xff

				if(optical_status & MOTION_STATUS_DYOVF)
					reg_y = ((signed char)reg_y>=0)? 0x81:0x7f;   //y overflow, current<0,0x7f, current>0, 0xff
			}

			if(reg_x&0x0080)
			{
				ms_data.x=0xff00|reg_x;
			}
			else
			{
				ms_data.x=reg_x;
			}
			
			if(reg_y&0x0080)
			{
				ms_data.y=0xff00|reg_y;
			}
			else
			{
				ms_data.y=reg_y;
			}
		}

		return 1;
	}
	else{
		return 0;
	}
 }


// Must clear SLP_ENH bit in the OPERATION_MODE register before using Power-Down mode.
void OPTSensor_Shutdown(void)
{
	u8 reg06=0;
	for(u8 i=0;i<8;i++)
	{  
		reg06=I2C_PAN3204LL_ReadRegister(REG_PAN3204LL_CONFIGURATION);
		I2C_PAN3204LL_WriteRegister(REG_PAN3204LL_CONFIGURATION,reg06|0x08);
       sleep_us(1000);
		reg06=I2C_PAN3204LL_ReadRegister(REG_PAN3204LL_CONFIGURATION);
		if(reg06&0x08)
		{
			break;
		}
		else
		{
			OPTSensor_resync(1);
		}
	}
	sensor_gpio_powerDownConfig();
			
}

void sensor3204_cpi_set(u8 dpi_val)
{
	for (u8 i = 0; i < 32; i++)
	{
		u8	reg_1 = 0, reg_2 = 0;
		reg_2 = (I2C_PAN3204LL_ReadRegister(REG_PAN3204LL_CONFIGURATION) & 0xf8) | (dpi_val & 0x07);
		I2C_PAN3204LL_WriteRegister(REG_PAN3204LL_CONFIGURATION, reg_2);
		reg_1 = I2C_PAN3204LL_ReadRegister(REG_PAN3204LL_CONFIGURATION);
		if (reg_1 == reg_2)
		{
			return;
		}
	}
}

const u8 KA8G2_dpi_tbl[] =
{
	17, //1000
	40, //2400
	67, //4000
};

void KA8G2_CPI_set(u8 dpi_val)
{
	for (u8 i = 0; i < 16; i++)
	{
		u8	reg_1 = 0, reg_2 = 0;
		I2C_PAN3204LL_WriteRegister(0x09,0xA5);
		I2C_PAN3204LL_WriteRegister(0x0d, dpi_val);
		I2C_PAN3204LL_WriteRegister(0x0e, dpi_val);
		reg_1 = I2C_PAN3204LL_ReadRegister(0x0d);
		reg_2 = I2C_PAN3204LL_ReadRegister(0x0e);
		I2C_PAN3204LL_WriteRegister(0x09,0x00);
		if ((reg_1 == reg_2)&&(reg_2 == dpi_val))
		{
		    return;
		}
	}
}

const u8 paw3212_dpi_tbl[] =
{
	26, 			// cpi_1000 =  26*38 = 1000, step = 38
	32, 			// cpi_1200 = 32*38 = 1216
	42, 			// cpi_1600 = 42*38 = 1596
	63, 			// cpi_2400 = 63*38 = 2394
	// cpi_3600 Counterfeiting
};
	
void paw3212_CPI_set(u8 dpi_val)
{
	for (u8 i = 0; i < 16; i++)
	{
		u8	reg_1 = 0, reg_2 = 0;
		I2C_PAN3204LL_WriteRegister(0x09,0x5A);
		I2C_PAN3204LL_WriteRegister(0x0d, dpi_val);
		I2C_PAN3204LL_WriteRegister(0x0e, dpi_val);
		reg_1 = I2C_PAN3204LL_ReadRegister(0x0d);
		reg_2 = I2C_PAN3204LL_ReadRegister(0x0e);
		I2C_PAN3204LL_WriteRegister(0x09,0x00);
			
		if ((reg_1 == reg_2)&&(reg_2 == dpi_val))
		{
		    return;
		}
	}
}

const u8 om18_dpi_tbl[] =
{
	// 22, 			// cpi_800 =  22*38 = 836, step = 38
	32, 			// cpi_1200 = 32*38 = 1216
	42, 			// cpi_1600 = 42*38 = 1596
	63, 			// cpi_2400 = 63*38 = 2394
	// cpi_3600 Counterfeiting
};
	
void om18_CPI_set(u8 dpi_val)
{
	for (u8 i = 0; i < 16; i++)
	{
		u8	reg_1 = 0, reg_2 = 0;

		I2C_PAN3204LL_WriteRegister(0x09,0x5a);

		I2C_PAN3204LL_WriteRegister(0x10, dpi_val);
		I2C_PAN3204LL_WriteRegister(0x11, dpi_val);
		reg_1 = I2C_PAN3204LL_ReadRegister(0x10);
		reg_2 = I2C_PAN3204LL_ReadRegister(0x11);
		I2C_PAN3204LL_WriteRegister(0x09,0x00);
		if ((reg_1 == reg_2)&&(reg_2 == dpi_val))
		{
			return;
		}
	}
}


_attribute_data_retention_user  u8 xy_multiple_flag = 0;
void sensor_dpi_set(u8 cpi_ctrl)
{
	if(sensor_type==SENSOR_3205_TJ3T)
	{
         //vaule 0:  600
        //vaule 1:  800
        //vaule 2:  1000
        //vaule 3:  1300
        //vaule 4:  1600
        if(cpi_ctrl == 3){
			cpi_ctrl = 2;
			xy_multiple_flag = MULTIPIPE_1_DOT_5;
			sensor3204_cpi_set(cpi_ctrl+2);
		}else if(cpi_ctrl == 4){
			cpi_ctrl = 2;
			xy_multiple_flag = MULTIPIPE_2_DOT_5;
			sensor3204_cpi_set(cpi_ctrl+2);
		}else{
			xy_multiple_flag = MULTIPIPE_1;
			sensor3204_cpi_set(cpi_ctrl+2);
		}
	}
	else if(sensor_type==SENSOR_3205_TJNT)
	{
        //vaule 0:  600
        //vaule 1:  800
        //vaule 2:  1000
        //vaule 3:  1300
        //vaule 4:  1600
        if(cpi_ctrl == 3){
			cpi_ctrl = 2;
			xy_multiple_flag = MULTIPIPE_1_DOT_5;
			sensor3204_cpi_set(cpi_ctrl+2);
		}else if(cpi_ctrl == 4){
			cpi_ctrl = 2;
			xy_multiple_flag = MULTIPIPE_2_DOT_5;
			sensor3204_cpi_set(cpi_ctrl+2);
		}else{
			xy_multiple_flag = MULTIPIPE_1;
			sensor3204_cpi_set(cpi_ctrl+2);
		}
	}
	else if(sensor_type==SENSOR_3065)
	{
         //vaule 0:  600
        //vaule 1:  800
        //vaule 2:  1000
        //vaule 3:  1300
        //vaule 4:  1600
		if(cpi_ctrl == 3){
			cpi_ctrl = 2;
			xy_multiple_flag = MULTIPIPE_1_DOT_5;
			sensor3204_cpi_set(cpi_ctrl + 2);
		}else if(cpi_ctrl == 4){
			cpi_ctrl = 2;
			xy_multiple_flag = MULTIPIPE_2_DOT_5;
			sensor3204_cpi_set(cpi_ctrl + 2);
		}else{
			xy_multiple_flag = MULTIPIPE_1;
			sensor3204_cpi_set(cpi_ctrl + 2);
		}
	}
	else if(sensor_type==SENSOR_3065XY)
	{
         //vaule 0:  800
        //vaule 1:  1000
        //vaule 2:  1300
        //vaule 3:  1600
		 if(cpi_ctrl == 3){//2400
			cpi_ctrl = 3;
			xy_multiple_flag = MULTIPIPE_1_DOT_5;
			sensor3204_cpi_set(cpi_ctrl);
		}else if(cpi_ctrl == 4){ //4000
			cpi_ctrl = 3;
			xy_multiple_flag = MULTIPIPE_2_DOT_5;
			sensor3204_cpi_set(cpi_ctrl);
		}else {
			xy_multiple_flag = MULTIPIPE_1;
			sensor3204_cpi_set(cpi_ctrl + 1);
		}
	}
	else if(sensor_type==SENSOR_VT108)
	{
		sensor3204_cpi_set(cpi_ctrl+4);
	}
	else if(sensor_type==SENSOR_M8650A)
	{
		sensor3204_cpi_set(cpi_ctrl);
	}
	else if(sensor_type==SENSOR_OM18)
	{
		om18_CPI_set(om18_dpi_tbl[cpi_ctrl]);
	}
	else if(sensor_type==SENSOR_YS8009)
	{
		sensor3204_cpi_set(cpi_ctrl+2);
	}
	else if(sensor_type==SENSOR_KA8G2)
	{
        if(cpi_ctrl > 2)
            cpi_ctrl = 2;

		KA8G2_CPI_set(KA8G2_dpi_tbl[cpi_ctrl]);
	}
	else if(sensor_type==SENSOR_PAW3212)
	{
		if(cpi_ctrl == 4){
			cpi_ctrl = 2;
			xy_multiple_flag = MULTIPIPE_2_DOT_5;
			paw3212_CPI_set(paw3212_dpi_tbl[cpi_ctrl]);
		}else{
			xy_multiple_flag = MULTIPIPE_1;
			paw3212_CPI_set(paw3212_dpi_tbl[cpi_ctrl]);
		}
	}
	else if(sensor_type==SENSOR_S201B)
	{
		sensor3204_cpi_set(cpi_ctrl+1);
	}
}

void btn_dpi_set()
{
	dpi_value++;
	
	if (dpi_value >= dpi_max_level)
	{
	   dpi_value = 0;
	}

	analog_write(USED_DPI_DEEP_ANA_REG7, dpi_value);

    sensor_dpi_set(dpi_value);

#if BLT_APP_LED_ENABLE
    dpi_led_set(dpi_value + 1);
#endif
}

int Sensor3204_Wakeup(u32 sensor)
{
    return 1;
}

void check_sensor_dircet(u8 sensor_dir)
{
    s16 temp;

    if (sensor_dir == SENSOR_DIRECTION_CLOCK_3)
    {
        temp = ms_data.y;
        ms_data.y = ms_data.x;
        ms_data.x = temp;
    }
    else if (sensor_dir == SENSOR_DIRECTION_CLOCK_6)
    {
        ms_data.x = -ms_data.x;
    }
    else if (sensor_dir == SENSOR_DIRECTION_CLOCK_9)
    {
        temp = ms_data.y;
        ms_data.y = -ms_data.x;
        ms_data.x = -temp;
    }

    else if (sensor_dir == SENSOR_DIRECTION_CLOCK_12)
    {
        ms_data.y = -ms_data.y;
    }
}

_attribute_data_retention_user int sm_sum_x=0;
_attribute_data_retention_user int sm_pre_x=0; 
_attribute_data_retention_user int sm_sum_y=0;
_attribute_data_retention_user int sm_pre_y=0;
void iir_smoother()
{
    sm_sum_x = sm_sum_x - sm_pre_x + ms_data.x;
    sm_pre_x = sm_sum_x / 2;
    ms_data.x = sm_pre_x;

    sm_sum_y = sm_sum_y - sm_pre_y + ms_data.y;
    sm_pre_y = sm_sum_y / 2;
    ms_data.y = sm_pre_y;
}

static inline void iir_smoother_clear(void)
{
    sm_sum_x = 0;
    sm_pre_x = 0;

    sm_sum_y = 0;
    sm_pre_y = 0;
}

#define sm_dyn_pth1  6
#define sm_dyn_pth2  4
u8 adaptive_smoother()
{
    _attribute_data_retention_user static u8 asm_flg = 0;
    _attribute_data_retention_user static u32 sm_last_smoother_tick = 0;

    //auto clear asm sum when no data for a long time
    if (asm_flg && clock_time_exceed(sm_last_smoother_tick, 100000))
    {
        asm_flg = 0;
        iir_smoother_clear();
    }
    if (!asm_flg)
    {
        if ((abs(ms_data.x) > sm_dyn_pth1) || (abs(ms_data.y) > sm_dyn_pth1))
        {
            asm_flg = 1;
            iir_smoother();
        }
        else
        {
            asm_flg = 0;
            iir_smoother_clear();
        }
    }
    else
    {
        if ((abs(ms_data.x) < sm_dyn_pth2) && (abs(ms_data.y) < sm_dyn_pth2))
        {
            asm_flg = 0;
            iir_smoother_clear();
        }
        else
        {
            asm_flg = 1;
            iir_smoother();
        }
    }

    if (asm_flg)
    {
        sm_last_smoother_tick = clock_time();
    }

    return asm_flg;
}


void Sensor_Concent_Init(void)
{
	OPTSensor_Init(1);
	 if (flash_dev_info.dpi == 0 || flash_dev_info.dpi == U8_MAX) {
		if (sensor_type == SENSOR_PAW3212) {
			flash_dev_info.dpi = 2;
		} else if (sensor_type == SENSOR_3065 || sensor_type == SENSOR_3065XY || sensor_type == SENSOR_3205_TJ3T || sensor_type == SENSOR_3205_TJNT ){
			flash_dev_info.dpi = 2;
		}else{
			flash_dev_info.dpi = 3;
		}
		save_dev_info_flash();
    }

    my_printf_aaa("sensor init flash_dev_info.dpi %d\r\n", flash_dev_info.dpi);
    sensor_dpi_set(flash_dev_info.dpi - 1);
}

#endif

