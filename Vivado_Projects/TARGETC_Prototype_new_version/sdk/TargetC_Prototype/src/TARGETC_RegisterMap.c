/**
 * @file 	TARGETC_RegisterMap.c
 * @author	Jonathan Hendriks
 * @date	14th November 2018
 * @version 0.0
 */
#include "TARGETC_RegisterMap.h"

/** **********************************************************
 * @brief 	Set the TargetC Registers using AXI Lite control
 *
 * @param	  void
 * @return	status
 ************************************************************* */
int SetTargetCRegisters(void){

	for(int i=0; i<64; i++){
		WriteRegister(TC_VDLYTUNE_REG + i,	0);
	}
//	*** TARGETC Register Initial Values
	WriteRegister(TC_SSTOUTFB_REG,		0x03a);

	WriteRegister(TC_SSPIN_LE_REG,		9);	//TEST with SSPIN
	WriteRegister(TC_SSPIN_TE_REG,		40);

	WriteRegister(TC_WR_STRB2_LE_REG,	54);
	WriteRegister(TC_WR_STRB2_TE_REG,	21);

	WriteRegister(TC_WR2_ADDR_LE_REG,	30);
	WriteRegister(TC_WR2_ADDR_TE_REG,	61);

	WriteRegister(TC_WR_STRB1_LE_REG,	15);
	WriteRegister(TC_WR_STRB1_TE_REG,	46);

	WriteRegister(TC_WR1_ADDR_LE_REG,	0);
	WriteRegister(TC_WR1_ADDR_TE_REG,	29);

	WriteRegister(TC_VQBUFF_REG,	1100);
	WriteRegister(TC_QBIAS_REG,		0);
	WriteRegister(TC_VTRIMT_REG,	0x4d8);

	WriteRegister(TC_VBIAS_REG,		0x4B0);

	WriteRegister(TC_VAPBUFF_REG,	0x3D9);
	//WriteRegister(TC_VADJP_REG,		0x480);
	WriteRegister(TC_VADJP_REG,		1020);

	WriteRegister(TC_VANBUFF_REG,	0x426);
	//WriteRegister(TC_VADJN_REG,		0x8BB);
	WriteRegister(TC_VADJN_REG,		2430);

	WriteRegister(TC_SBBIAS_REG,	0x78E);
	
	WriteRegister(TC_VDISCH_REG,	0);
	//WriteRegister(TC_ISEL_REG,		2600);
	WriteRegister(TC_ISEL_REG,	0x8fc);
	WriteRegister(TC_DBBIAS_REG,	0x690);

	// Initial Base Value
	WriteRegister(TC_CMPBIAS2_REG,	0x2D6);
	WriteRegister(TC_PUBIAS_REG,	0xBCA);
	WriteRegister(TC_CMPBIASIN_REG,	0x654);

	WriteRegister(TC_MISCDIG_REG,	0);		//nRD_EN, nWR1_Enable nWR2_Enable are set to
	WriteRegister(TC_MONTIMING_REG,	0);		//INIT MonTiming PASS disable

	xil_printf("DONE\r\n");
	xil_printf("\r\n");
}

/** **********************************************************
 * @brief 	Set the TargetC Registers using AXI Lite control
 *
 * @param	  void
 * @return	status
 ************************************************************* */
int SetTargetCRegisters_DLL(void){

	xil_printf("*** DLL Option ***\n\r");

	for(int i=0; i<64; i++){
		WriteRegister(TC_VDLYTUNE_REG + i,	0);
	}
//	*** TARGETC Register Initial Values
	WriteRegister(TC_SSTOUTFB_REG,		0x03a);

	WriteRegister(TC_SSPIN_LE_REG,		1);	//TEST with SSPIN
	WriteRegister(TC_SSPIN_TE_REG,		20);

	WriteRegister(TC_WR_STRB2_LE_REG,	55);
	WriteRegister(TC_WR_STRB2_TE_REG,	6);

	WriteRegister(TC_WR2_ADDR_LE_REG,	61);
	WriteRegister(TC_WR2_ADDR_TE_REG,	7);

	WriteRegister(TC_WR_STRB1_LE_REG,	25);
	WriteRegister(TC_WR_STRB1_TE_REG,	10);

	WriteRegister(TC_WR1_ADDR_LE_REG,	55);
	WriteRegister(TC_WR1_ADDR_TE_REG,	6);

	WriteRegister(TC_VQBUFF_REG,	1100);
	WriteRegister(TC_QBIAS_REG,		1500);
	WriteRegister(TC_VTRIMT_REG,	0x4d8);

	WriteRegister(TC_VBIAS_REG,		0x4B0);

	//WriteRegister(TC_VAPBUFF_REG,	0x400);//0x3D9);
	WriteRegister(TC_VAPBUFF_REG,	0x3D9);
	
	WriteRegister(TC_VADJP_REG,		0x480);
	//WriteRegister(TC_VADJP_REG,		767);

	WriteRegister(TC_VANBUFF_REG,	0);
	//WriteRegister(TC_VADJN_REG,		0x8BB);
	WriteRegister(TC_VADJN_REG,		2430);

	WriteRegister(TC_SBBIAS_REG,	0x78E);
	
	WriteRegister(TC_VDISCH_REG,	0);
	//WriteRegister(TC_ISEL_REG,		2600);
	WriteRegister(TC_ISEL_REG,	0x8fc);
	WriteRegister(TC_DBBIAS_REG,	0x690);

	// Initial Base Value
	WriteRegister(TC_CMPBIAS2_REG,	0x2D6);
	WriteRegister(TC_PUBIAS_REG,	0xBCA);
	WriteRegister(TC_CMPBIASIN_REG,	0x654);

	WriteRegister(TC_MISCDIG_REG,	0);		//nRD_EN, nWR1_Enable nWR2_Enable are set to
	WriteRegister(TC_MONTIMING_REG,	0);		//INIT MonTiming PASS disable

	xil_printf("DONE\r\n");
	xil_printf("\r\n");
}

int GetTargetCStatus(){
	xil_printf(">> STATUS:\t ");
	decToHexa(regptr[TC_STATUS_REG] );
	//xil_printf(" :");
#ifdef VERBOSE
	if(regptr[TC_STATUS_REG] & BUSY_MASK)	xil_printf("\t BUSY_MASK");
	if(regptr[TC_STATUS_REG] & LOCKED_MASK)	xil_printf("\t LOCKED_MASK");
	if(regptr[TC_STATUS_REG] & STORAGE_MASK)	xil_printf("\t STORAGE_MASK");
	if(regptr[TC_STATUS_REG] & SSVALID_MASK)	xil_printf("\t SSVALID_MASK");

	//if(regptr[TC_STATUS_REG] & RESET_MASK)	xil_printf("\t RESET_MASK");

#endif
	xil_printf("\r\n");
}

int GetTargetCControl(){
	xil_printf(">> CONTROL:\t ");
	decToHexa(regptr[TC_CONTROL_REG] );
	//xil_printf(" :");

#ifdef VERBOSE
	if(regptr[TC_CONTROL_REG] & WRITE_MASK)	xil_printf("\t WRITE_MASK");
	if(regptr[TC_CONTROL_REG] & PCLK_MASK)	xil_printf("\t PCLK_MASK");
	if(regptr[TC_CONTROL_REG] & SCLK_MASK)	xil_printf("\t SCLK_MASK");
	if(regptr[TC_CONTROL_REG] & SIN_MASK)	xil_printf("\t SIN_MASK");

	if(regptr[TC_CONTROL_REG] & RAMP_MASK)	xil_printf("\t RAMP_MASK");
	if(regptr[TC_CONTROL_REG] & REGCLR_MASK)	xil_printf("\t REGCLR_MASK");
	if(regptr[TC_CONTROL_REG] & SS_INCR_MASK)	xil_printf("\t SS_INCR_MASK");
	if(regptr[TC_CONTROL_REG] & SS_TPG_MASK)	xil_printf("\t SS_TPG_MASK");

	if(regptr[TC_CONTROL_REG] & SS_RESET_MASK)	xil_printf("\t SS_RESET_MASK");
	if(regptr[TC_CONTROL_REG] & RDAD_MASK)	xil_printf("\t RDAD_MASK");
	if(regptr[TC_CONTROL_REG] & STARTSTORAGE_MASK)	xil_printf("\t STARTSTORAGE_MASK");
	if(regptr[TC_CONTROL_REG] & SSACK_MASK)	xil_printf("\t SSACK_MASK");

	if(regptr[TC_CONTROL_REG] & SWRESET_MASK)	xil_printf("\t SWRESET_MASK");
#endif
	xil_printf("\r\n");
}

int ControlRegisterWrite(int mask, int actionID){

	//int* regptr = XPAR_TARGETC_0_TC_AXI_BASEADDR;
	if(actionID == ENABLE){
		regptr[TC_CONTROL_REG] = regptr[TC_CONTROL_REG] | mask;
	}
	else if (actionID == DISABLE){
		regptr[TC_CONTROL_REG] = regptr[TC_CONTROL_REG] & (0xFFFFFFFF^mask);
	}
	else if (actionID == INIT){
		regptr[TC_CONTROL_REG] =  0;
	}
	else{
		xil_printf("Error:\t%s\r\n\tNo valid argument {ENABLE/DISABLE}",__func__);
	}

	//DEBUG

#ifdef VERBOSE
	xil_printf(">> CONTROL:\t");

	if(regptr[TC_CONTROL_REG] & WRITE_MASK)	xil_printf("\t WRITE_MASK");
	if(regptr[TC_CONTROL_REG] & PCLK_MASK)	xil_printf("\t PCLK_MASK");
	if(regptr[TC_CONTROL_REG] & SCLK_MASK)	xil_printf("\t SCLK_MASK");
	if(regptr[TC_CONTROL_REG] & SIN_MASK)	xil_printf("\t SIN_MASK");

	if(regptr[TC_CONTROL_REG] & RAMP_MASK)	xil_printf("\t RAMP_MASK");
	if(regptr[TC_CONTROL_REG] & REGCLR_MASK)	xil_printf("\t REGCLR_MASK");
	if(regptr[TC_CONTROL_REG] & SS_INCR_MASK)	xil_printf("\t SS_INCR_MASK");
	if(regptr[TC_CONTROL_REG] & SS_TPG_MASK)	xil_printf("\t SS_TPG_MASK");

	if(regptr[TC_CONTROL_REG] & SS_RESET_MASK)	xil_printf("\t SS_RESET_MASK");
	if(regptr[TC_CONTROL_REG] & RDAD_MASK)	xil_printf("\t RDAD_MASK");
	if(regptr[TC_CONTROL_REG] & STARTSTORAGE_MASK)	xil_printf("\t STARTSTORAGE_MASK");
	if(regptr[TC_CONTROL_REG] & SSACK_MASK)	xil_printf("\t SSACK_MASK");

	if(regptr[TC_CONTROL_REG] & SWRESET_MASK)	xil_printf("\t SWRESET_MASK");
	xil_printf("\r\n");
#endif
}


int WriteRegister(int regID, int regData){

	ControlRegisterWrite(WRITE_MASK,DISABLE);

	regptr[regID] = regData;
	regptr[TC_ADDR_REG] = regID;

	ControlRegisterWrite(WRITE_MASK ,ENABLE);
	while(regptr[TC_STATUS_REG] & BUSY_MASK){
		usleep(100); //sleep 100ms
	}
	ControlRegisterWrite(WRITE_MASK,DISABLE);
}

int WriteReadBackRegister(int regID, int regData){

	//int* regptr = XPAR_TARGETC_0_TC_AXI_BASEADDR;

	//regptr[TC_CONTROL_REG] = 0;
	ControlRegisterWrite(WRITE_MASK,DISABLE);

	regptr[regID] = regData;
	regptr[TC_ADDR_REG] = regID;

	ControlRegisterWrite(WRITE_MASK ,ENABLE);
	//regptr[TC_CONTROL_REG] = 1 | RAMP_MASK;

	while(regptr[TC_STATUS_REG] & BUSY_MASK){
		usleep(100000); //sleep 100ms
	}
	ControlRegisterWrite(WRITE_MASK,DISABLE);

	xil_printf("W:\tReg: ");
	decToHexa(regID);
	xil_printf("\tData: ");
	decToHexa(regData);
	xil_printf("\r\n");

	//Dummy Write
	ControlRegisterWrite(WRITE_MASK ,ENABLE);
	while(regptr[TC_STATUS_REG] & BUSY_MASK){
		usleep(100000); //sleep 100ms
	}
	ControlRegisterWrite(WRITE_MASK ,DISABLE);

	//xil_printf(">>\tRB:\tReg:%d\tData:%d\r\n",regptr[TC_DATA_OUT_REG]>>12,regptr[TC_DATA_OUT_REG] & 0x00000FFF);
	xil_printf("RB:\t%d\t",regptr[TC_DATA_OUT_REG]);
	decToHexa(regptr[TC_DATA_OUT_REG]);
	xil_printf("\r\n");

}


int TestPatternGenerator(int tpg){

	//int* regptr = XPAR_TARGETC_0_TC_AXI_BASEADDR;
	//Charge TPG in register
	ControlRegisterWrite(SS_TPG_MASK,DISABLE);

	//WriteReadBackRegister(TC_TPG_REG, tpg);
	WriteRegister(TC_TPG_REG, tpg);

	TPG_intr_flg = 0;

	//TPG when SampleAny is '0'

	//Start the TPG conversion and readout
	ControlRegisterWrite(SS_INCR_MASK,ENABLE);
	ControlRegisterWrite(SS_INCR_MASK,DISABLE);
	//GetTargetCControl();
	//GetTargetCStatus();
	while(TPG_intr_flg == 0){
		sleep(1);
	}

	xil_printf("\r\nTPG:\t");
	decToHexa(tpg);
	xil_printf("\r\n");

	xil_printf("CH0:\t%d\t",regptr[TC_eDO_CH0_REG]);
	decToHexa(regptr[TC_eDO_CH0_REG]);
	xil_printf("\t");
	decToBin(regptr[TC_eDO_CH0_REG]);
	xil_printf("\r\n");

	//Reset Interrupt
	//ControlRegisterWrite(SS_RESET_MASK,ENABLE);
	TPG_intr_flg = 0;
	ControlRegisterWrite(SSACK_MASK,DISABLE);

	ControlRegisterWrite(SS_TPG_MASK,ENABLE);	// SAmple and not TPG
	//Wait on interrupt to be caught
}

void decToHexa(unsigned int n){
    // char array to store hexadecimal number
    char hexaDeciNum[100];

    // counter for hexadecimal number array
    int i = 0;
    while(n!=0)
    {
        // temporary variable to store remainder
        int temp  = 0;

        // storing remainder in temp variable.
        temp = n % 16;

        // check if temp < 10
        if(temp < 10)
        {
            hexaDeciNum[i] = temp + 48;
            i++;
        }
        else
        {
            hexaDeciNum[i] = temp + 55;
            i++;
        }

        n = n/16;
    }

    // printing hexadecimal number array in reverse order
    for(int j=i-1; j>=0; j--){
        xil_printf("%c",hexaDeciNum[j]);
    }

}

void decToBin(unsigned int n){
    // char array to store hexadecimal number
    char BinDeciNum[100];

    // counter for hexadecimal number array
    int i = 0;
    while(n!=0)
    {
        // temporary variable to store remainder
        int temp  = 0;

        // storing remainder in temp variable.
        temp = n % 2;

        // check if temp < 10
        if(temp < 10)
        {
            BinDeciNum[i] = temp + 48;
            i++;
        }
        else
        {
            BinDeciNum[i] = temp + 55;
            i++;
        }

        n = n/2;
    }

    // printing hexadecimal number array in reverse order
    for(int j=i-1; j>=0; j--)
        xil_printf("%c",BinDeciNum[j]);


}