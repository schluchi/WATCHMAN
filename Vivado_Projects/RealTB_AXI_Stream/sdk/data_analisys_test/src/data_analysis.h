/*
 * features_extraction.h
 *
 *  Created on: 28 nov. 2018
 *      Author: Anthony
 */

#include <math.h>

#ifndef SRC_FEATURES_EXTRACTION_H_
#define SRC_FEATURES_EXTRACTION_H_

#include "xtime_l.h"

typedef struct features_ext_st{
	int amplitude;
	float time;
} features_ext;

typedef struct coordinates_st{
	float x;
	float y;
} coordinates;

int16_t capacitor_discharge(int16_t Vc, int64_t t1, int64_t t2, int32_t wdo_id, int ch, int sample);
int16_t substract_pedestal(int16_t v, int16_t vped, int32_t wdo_id, int ch, int sample);
int16_t correct_data(int16_t mes, int64_t t1, int64_t t2, int16_t vped, int32_t wdo_id, int ch, int sample);
void extract_features(int vped, int* data, int length, features_ext* features, XTime* tInt);

#endif /* SRC_FEATURES_EXTRACTION_H_ */
