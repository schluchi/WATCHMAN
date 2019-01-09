/*
 * features_extraction.h
 *
 *  Created on: 28 nov. 2018
 *      Author: Anthony
 */


#ifndef SRC_FEATURES_EXTRACTION_H_
#define SRC_FEATURES_EXTRACTION_H_

#include <math.h>
#include <stdbool.h>
#include "xtime_l.h"
#include "axis_peripheral.h"

#define SIZE_DATA_ARRAY 	518 // 32samples * 16ch = 512 + header = 518
#define SIZE_DATA_ARRAY_BYT	SIZE_DATA_ARRAY*4 // -> int32_t is 4 bytes

#define CHANNEL 		16
#define SAMPLE 			32
#define MAX_WINDOW		4

#define TRIG_SHIFT		0
#define LAST_SHIFT		4
#define	TOO_LONG_SHIFT	8
#define MASK_INFO		0xF

#define VPED			1638 // 2.5V <=> 2047 -> 2V <=> 1637.6

typedef union time_union{
	float time_fl;
	int time_t;
}time_un;

typedef struct features_ext_st{
	int amplitude;
	time_un time;
} features_ext;

typedef struct coordinates_st{
	float x;
	float y;
} coordinates;

typedef struct data_axi_st{
	uint64_t wdo_time;
	uint64_t dig_time;
	uint32_t info;
	uint32_t wdo_id;
	uint32_t data[16][32];
}data_axi;

typedef union data_axi_union{
	struct data_axi_st data_struct;
	uint32_t data_array[SIZE_DATA_ARRAY];
}data_axi_un;

typedef struct data_list_st data_list;
struct data_list_st{
	data_axi_un data;
	data_list* previous;
	data_list* next;
};

int correct_data(uint16_t* data, int group, char nbr_wdo, uint32_t* info, data_list* tmp_first_element);
void extract_features(uint16_t* data, int length, features_ext* features);
#endif /* SRC_FEATURES_EXTRACTION_H_ */
