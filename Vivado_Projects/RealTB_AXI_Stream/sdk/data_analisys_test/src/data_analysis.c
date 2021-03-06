/*
 * features_extraction.c
 *
 *  Created on: 28 nov. 2018
 *      Author: Anthony
 */

#include "data_analysis.h"

extern double period[512][16][32];
extern int16_t pedestal[512][16][32];
extern data_list* first_element;
extern data_list* last_element;


/****************************************************************************/
/**
* @brief	Correct the data received from the PL side (pedestal subtraction &
* 			transfer function correction)
*
* @param	data: pointer on array to return the data corrected
* 			pmt: PMT's ID (4 PMTs per ASIC, 16 ch per ASIC -> 4 ch per PMT)
* 			nbr_wdo: number of window for the pulse
* 			info: pointer to return info of pulse too long or not
*
* @return	ch: channel chose (gain stage chose)
*
* @note		-
*
****************************************************************************/
int correct_data(uint16_t* data, int pmt, char nbr_wdo, uint32_t* info){
	int ch = 4*pmt;
	int ch_last = ch+3;
	data_list* ptr;
	int wdo, sample;
	int index = 0;
	uint16_t vped = 1024; // because pedestal are null too
	uint16_t treshold = 500;
	bool gain_good = false;
	bool too_long = false;

	while(!gain_good){
		gain_good = true;
		ptr = first_element;
		index = 0;
		for(wdo=0; wdo<nbr_wdo; wdo++){
			// currently there is only the pedestal substraction, still need the transfert function correction
			for(sample=0; sample<32; sample++){
				data[index] = (uint16_t)ptr->data.data_struct.data[ch][sample] + vped - pedestal[ptr->data.data_struct.wdo_id][ch][sample];
				//printf("data = %d - %d = %d| sample = %d | index = %d | wdo = %d | wdo_id = %d | ch = %d\r\n", (uint16_t)ptr->data.data_struct.data[ch][sample], pedestal[ptr->data.data_struct.wdo_id][ch][sample], data[index], sample, index, wdo, ptr->data.data_struct.wdo_id, ch);
				if(data[index] < treshold){
					if(ch < ch_last){
						ch++;
						gain_good = false;
						sample = 32;
						wdo = nbr_wdo;
					}
					else too_long = true;
				}
				index++;
			}
			ptr = ptr->next;
		}
	}
	if(too_long) *info |= 0x1 << (TOO_LONG_SHIFT+pmt); // if all gain stage are saturated
	return ch;
}

void extract_features(uint16_t* data, int length, features_ext* features, XTime* tInt){
	int vped = 1024;
	int i, sample, amplitude;
	float amp_start, a, b;
	coordinates p1, p2;

	/* Get the minimum amplitude of the pulse */
	// if it takes too, maybe possible to do it in the function correct data
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

uint16_t capacitor_discharge(uint16_t Vc, int64_t t1, int64_t t2, int32_t wdo_id, int ch, int sample){
	double t = (double)(t2 - t1);
	uint16_t Vo;
	Vo = (uint16_t)((double)Vc / (exp(-t/period[wdo_id][ch][sample])));
	return Vo;
}

uint16_t substract_pedestal(uint16_t v, uint16_t vped, int32_t wdo_id, int ch, int sample){
	uint16_t ret;
	ret = v - pedestal[wdo_id][ch][sample] + vped;
	return ret;
}
