/*
 * features_extraction.c
 *
 *  Created on: 28 nov. 2018
 *      Author: Anthony
 */

#include "features_extraction.h"

void extract_features(int vped, int* data, int length, features_ext* features, XTime* tInt){
	int i, sample, amplitude;
	float amp_start, a, b;
	coordinates p1, p2;

	/* Get the minimum amplitude of the pulse */
	amplitude = vped;
	for(i=0; i<length; i++){
		if(data[i] < amplitude){
			amplitude = data[i];
			sample = i;
		}
	}
	XTime_GetTime(tInt);
	features->amplitude = amplitude;

	/* Get the moment where the curve was at 20% of the difference between vped and the minimum amplitude */
	amp_start = vped - 0.2*(vped - features->amplitude);

	i=sample;
	while((float)data[i] < amp_start) i--;
	p1.x = (float)i;	// extrapolate the line between the two point surrounding the amplitude we're looking for
	p1.y = (float)data[i];
	i++;
	p2.x = (float)i;
	p2.y = (float)data[i];
	a = (p2.y - p1.y)/(p2.x - p1.x);
	b = p1.y - a*p1.x;
	features->time = (amp_start - b)/a;
}

