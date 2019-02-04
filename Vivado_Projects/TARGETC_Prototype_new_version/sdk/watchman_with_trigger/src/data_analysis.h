/**
 * @file 	features_extraction.h
 * @author	Anthony Schluchin
 * @date	28th November 2018
 * @version 0.0
 */


#ifndef SRC_FEATURES_EXTRACTION_H_
#define SRC_FEATURES_EXTRACTION_H_

#include <math.h>
#include <stdbool.h>
#include "xtime_l.h"
#include "axis_peripheral.h"

/*** DEFINES *********************************************************/
#define SIZE_DATA_ARRAY 	518 // 32samples * 16ch = 512 + header = 518
#define SIZE_DATA_ARRAY_BYT	SIZE_DATA_ARRAY*4 // -> int32_t is 4 bytes

#define CHANNEL 		16
#define SAMPLE 			32
#define MAX_WINDOW		4

#define TRIG_SHIFT		0
#define LAST_SHIFT		4
#define	TOO_LONG_SHIFT	8
#define MASK_INFO		0xF

#define VPED_DIGITAL	1024//1638 // 2.5V <=> 2047 -> 2V <=> 1637.6
#define VPED_ANALOG		1.25//2.0
#define THRESHOLD_CMP	1.75	// threshold voltage for the comparator
#define THRESHOLD_PULSE	500		// threshold used in the correct_data fct to choose the right gain stage

/*** Type definition *************************************************/
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
    uint64_t wdo_time;	// Timestampe of the window
    uint64_t PL_spare; 	// Spare bits for the developement
    uint32_t info; 		// Information about the window
    uint32_t wdo_id; 	// ID of the window (0 to 511)
    uint32_t data[16][32];// Payload
}data_axi;

typedef union data_axi_union{
    struct data_axi_st data_struct;		// Structure used by the uC
    uint32_t data_array[SIZE_DATA_ARRAY];	// Pointer to pass to the DMA
}data_axi_un;

typedef struct data_list_st data_list;
struct data_list_st{
    data_axi_un data;	// Current element
    data_list* previous;	// Pointer on the previous element
    data_list* next;	// Pointer on the next element
};

/*** Function prototypes *********************************************/
int correct_data(uint16_t* data, int pmt, char nbr_wdo, uint32_t* info, data_list* tmp_first_element);
void extract_features(uint16_t* data, int length, features_ext* features);

#endif /* SRC_FEATURES_EXTRACTION_H_ */
