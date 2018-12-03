/*
 * features_extraction.h
 *
 *  Created on: 28 nov. 2018
 *      Author: Anthony
 */

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

void extract_features(int vped, int* data, int length, features_ext* features, XTime* tInt);

#endif /* SRC_FEATURES_EXTRACTION_H_ */
