/**
 * @file 	iic_DAC_LTC2657.h
 * @author	Anthony Schluchin
 * @date	15th December 2018
 * @version 0.0
 */

#ifndef IIC_DAC_LTC2657_H   /* prevent circular inclusions */
#define IIC_DAC_LTC2657_H   /* by using protection macros */

#include "xparameters.h"
#include "xil_cache.h"
#include "xscugic.h"
#include "xil_printf.h"
#include "xtime_l.h"
#include "xiic.h"

#include "global.h"

/*** DEFINES *********************************************************/
#define IIC_DEVICE_ID		XPAR_AXI_IIC_0_DEVICE_ID	//Xparameters Base Address

#define IIC_SLAVE_ADDRESS	0x10	// ADC i2c address

#define CHANNEL_A	0x00
#define CHANNEL_B	0x01
#define CHANNEL_C	0x02
#define CHANNEL_D	0x03
#define CHANNEL_E	0x04
#define CHANNEL_F	0x05
#define CHANNEL_G	0x06
#define CHANNEL_H	0x07
#define CHANNEL_ALL	0x0F

#define DAC_GRP_0	CHANNEL_A
#define DAC_GRP_1	CHANNEL_B
#define DAC_GRP_2	CHANNEL_C
#define DAC_GRP_3	CHANNEL_D
#define DAC_VPED	CHANNEL_H




/*** Function prototypes *********************************************/
int DAC_LTC2657_initialize();
int DAC_LTC2657_SetChannelVoltage(int channel, float voltage);

#endif

