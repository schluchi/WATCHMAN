#include "iic_DAC_LTC2657.h"


int DAC_LTC2657_initialize(void){

	int Status = XST_SUCCESS;
	
	XIic_Config *ConfigPtr;	/* Pointer to configuration data */
	
	char WriteBuffer[3];
	
	ConfigPtr = XIic_LookupConfig(IIC_DEVICE_ID);
	if (ConfigPtr == NULL) {
		xil_printf("In %s: DAC Look up config failed...\r\n",
		__func__);
		return XST_FAILURE;
	}

	Status = XIic_CfgInitialize(&Iic, ConfigPtr,
					ConfigPtr->BaseAddress);
	if (Status != XST_SUCCESS) {
		xil_printf("In %s: DAC Cfg initialization failed...\r\n",
		__func__);
		return XST_FAILURE;
	}
	
	// No interrupt like it is only write only
	
	XIic_Start(&Iic);
	Status = XIic_SetAddress(&Iic, XII_ADDR_TO_SEND_TYPE, IIC_SLAVE_ADDRESS);
	if (Status != XST_SUCCESS) {
		xil_printf("In %s: DAC setting address failed...\r\n",
		__func__);
		return XST_FAILURE;
	}
	/*
	int XIic_MasterSend	(	XIic* 	InstancePtr, u8* 	TxMsgPtr, int 	ByteCount)
	Function to be called for accessing the IIC device
	*/
		
	return Status;	
}

int DAC_LTC2657_SetChannelVoltage(int channel, float voltage){

	int Status;
	char WriteBuffer[3];
		
	Status = XIic_SetAddress(&Iic, XII_ADDR_TO_SEND_TYPE, IIC_SLAVE_ADDRESS);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	
	//Channel
	switch(channel){
		case DAC_GRP_0:		xil_printf("DAC_GRP_0:%d\t",channel);	break;
		case DAC_GRP_1:		xil_printf("DAC_GRP_1:%d\t",channel);	break;
		case DAC_GRP_2:		xil_printf("DAC_GRP_2:%d\t",channel);	break;
		case DAC_GRP_3:		xil_printf("DAC_GRP_3:%d\t",channel);	break;
		case CHANNEL_E:		xil_printf("Channel E:%d\t",channel);	break;
		case CHANNEL_F:		xil_printf("Channel F:%d\t",channel);	break;
		case CHANNEL_G:		xil_printf("Channel G:%d\t",channel);	break;
		case DAC_VPED:		xil_printf("DAC_VPED:%d\t",channel);	break;
		case CHANNEL_ALL:	xil_printf("Channel ALL:%d\t",channel);	break;
		default :		
			return XST_FAILURE;
			break;	
	}
	
		//Voltage 
	int intvolt = (int)(65536.0 * voltage / 2.5);	
	printf("%lf V\r\n",voltage);
	
	
	WriteBuffer[0] = 0x30 | channel;	
	WriteBuffer[1] = intvolt>>8;	//0b10000000; // MSB First
	WriteBuffer[2] = intvolt & 0x00FF;	//0b00000000; // LSB Last

	int i;
	for(i=0; i <5 ; i++){
		Status = XIic_MasterSend(&Iic,WriteBuffer,4);
		if(Status == XST_SUCCESS){
			break;
		}
	}
	if(i ==5) return XST_FAILURE;
		
	sleep(1);	//Time for the DAC to ouput and stabilize the voltages
	return Status;	

}

