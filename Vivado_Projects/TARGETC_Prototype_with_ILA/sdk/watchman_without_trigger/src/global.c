/*
 * global.c
 *
 *  Created on: 28 nov. 2018
 *      Author: Anthony
 */

#include "global.h"

/* Global variables */
struct netif *echo_netif;
volatile int count_ttcps_timer;
volatile int count_scu_timer;
volatile bool run_flag;
volatile bool stream_flag;
volatile bool flag_ttcps_timer;
volatile bool flag_scu_timer;
volatile bool flag_timefile;
XAxiDma AxiDmaInstance;
XScuWdt WdtScuInstance;
volatile bool flag_axidma_error;
volatile bool flag_axidma_rx_done;
int flag_axidma_rx[4];
int nbre_of_bytes;
data_list* first_element;
data_list* last_element;
volatile bool flag_assertion;
volatile bool flag_while_loop;
char* frame_buf_tmp;
char* frame_buf;
int* regptr;
uint16_t pedestal[512][16][32];

/****************************************************************************/
/**
* @brief	Initiate all the global variables declared in global.h file
*
* @param	None
*
* @return	None
*
* @note		-
*
****************************************************************************/
int init_global_var(void){
	int i;

	count_ttcps_timer = 0;
	count_scu_timer = 0;
	run_flag = true;
	stream_flag = false;
	nbre_of_bytes = 0;
	flag_ttcps_timer = false;
	flag_scu_timer = false;
	flag_timefile = false;
	first_element = (data_list *)malloc(sizeof(data_list));
	if(!first_element){
		xil_printf("malloc for first_element failed in function, %s!\r\n", __func__);
		return XST_FAILURE;
	}
	first_element->previous = NULL;
	first_element->next = NULL;
	last_element = first_element;
	flag_assertion = false;
	flag_while_loop = false;
	flag_axidma_error = false;
	flag_axidma_rx_done = false;
	for(i=0; i<4; i++) flag_axidma_rx[i] = 0;
	frame_buf_tmp = (char *)malloc(MAX_STREAM_SIZE);
	if(!frame_buf_tmp){
		xil_printf("malloc for frame_buf_tmp failed in function, %s!\r\n", __func__);
		return XST_FAILURE;
	}
	frame_buf = &frame_buf_tmp[BUF_HEADER_SIZE];
	regptr = XPAR_TARGETC_IP_PROTOTYPE_0_BASEADDR ; //XPAR_TARGETC_INTERFACE_IP_0_BASEADDR;
	for(i = TC_VDLYTUNE_REG; i<= TC_WL_DIV_REG; i++) regptr[i] = 0;
	return XST_SUCCESS;
}

/****************************************************************************/
/**
* @brief	Free memory from the mallocs done in function initt_global_var
*
* @param	None
*
* @return	None
*
* @note		-
*
****************************************************************************/
void cleanup_global_var(void){
	do{
		data_list* tmp = first_element->next;
		free(first_element);
		first_element = tmp;
	} while(first_element != NULL);
	free(frame_buf_tmp);
}

