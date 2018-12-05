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
int *PtrData;
volatile bool stream_flag;
volatile bool flag_ttcps_timer;
volatile bool flag_scu_timer;
volatile bool flag_timefile;
XAxiDma AxiDmaInstance;
XScuWdt WdtScuInstance;
char axidma_error;
char axidma_rx_done;
int nbre_of_bytes;
data_list* list;
volatile bool flag_assertion;
volatile bool flag_while_loop;

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
void init_global_var(void){
	count_ttcps_timer = 0;
	count_scu_timer = 0;
	run_flag = true;
	stream_flag = false;
	PtrData = (int *)malloc(NBR_DATA);
	nbre_of_bytes = 0;
	flag_ttcps_timer = false;
	flag_scu_timer = false;
	flag_timefile = false;
	list = malloc(sizeof(struct data_list_st));
	list->next = NULL;
	flag_assertion = false;
	flag_while_loop = false;
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
	free(PtrData);
	do{
		data_list* tmp = list->next;
		free(list);
		list = tmp;
	} while(list != NULL);
}

