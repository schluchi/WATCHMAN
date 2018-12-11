/*
 * features_extraction.h
 *
 *  Created on: 28 nov. 2018
 *      Author: Anthony
 */


#ifndef SRC_FEATURES_EXTRACTION_H_
#define SRC_FEATURES_EXTRACTION_H_

#include <math.h>
#include "xtime_l.h"
#include "global.h"
#include <stdbool.h>

typedef struct features_ext_st{
	int amplitude;
	float time;
} features_ext;

typedef struct coordinates_st{
	float x;
	float y;
} coordinates;

int correct_data(uint16_t* data, int pmt, char nbr_wdo, uint32_t* info);
void extract_features_previous(int vped, int* data, int length, features_ext* features, XTime* tInt);
uint16_t capacitor_discharge(uint16_t Vc, int64_t t1, int64_t t2, int32_t wdo_id, int ch, int sample);
uint16_t substract_pedestal(uint16_t v, uint16_t vped, int32_t wdo_id, int ch, int sample);

#endif /* SRC_FEATURES_EXTRACTION_H_ */
