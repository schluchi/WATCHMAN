/*
 * global.h
 *
 *  Created on: 7 déc. 2018
 *      Author: Anthony
 */

#ifndef SRC_GLOBAL_H_
#define SRC_GLOBAL_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE_DATA_ARRAY 518 // (16*32+6)

#define TRIG_SHIFT		0
#define LAST_SHIFT		4
#define	TOO_LONG_SHIFT	8
#define MASK_INFO		0xF

typedef struct data_axi_st{
	uint64_t wdo_time;
	uint64_t dig_time;
	uint32_t info;
	uint32_t wdo_id;
	uint32_t data[16][32];
}data_axi;

typedef union data_axi_union{
	data_axi data_struct;
	uint32_t data_array[SIZE_DATA_ARRAY];
}data_axi_un;

typedef struct data_list_st data_list;
struct data_list_st{
	data_axi_un data;
	data_list* next;
};

int init_global_var(void);
void cleanup_global_var(void);

#endif /* SRC_GLOBAL_H_ */
