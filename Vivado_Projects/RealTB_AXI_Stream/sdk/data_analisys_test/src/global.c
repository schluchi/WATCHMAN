/*
 * global.c
 *
 *  Created on: 7 déc. 2018
 *      Author: Anthony
 */

#include "global.h"

double period[512][16][32];
uint16_t pedestal[512][16][32];
data_list* first_element;
data_list* last_element;

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
	//printf("sizeof(data_axi_un) = %d | sizeof(data_axi) = %d\r\n", sizeof(data_axi_un), sizeof(data_axi));
	first_element = malloc(sizeof(data_list));
	if(!first_element){
		printf("malloc(%d) for first_element failed in function, %s!\r\n", sizeof(data_list), __func__);
		return 1;
	}
	first_element->next = NULL;
	last_element = first_element;
	return 0;
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
	} while(first_element != 0);
}
