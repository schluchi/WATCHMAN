/*
 * global.c
 *
 *  Created on: 28 nov. 2018
 *      Author: Anthony
 */

#include "global.h"

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
	flag_timefile = false;
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
}

