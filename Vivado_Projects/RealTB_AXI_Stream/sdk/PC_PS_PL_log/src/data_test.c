/*
 * data_testp.c
 *
 *  Created on: Jul 30, 2018
 *      Author: Jose Idlabs
 */

#include "data_test.h"

uint16_t data[CHANNEL][FRAME];

uint16_t made_frame(char stream[], uint16_t length){
	memset(stream, 0, length);
	char type;
	uint16_t count = 0;
	stream[count++] = 0x55;
	stream[count++] = 0xAA;
	count+=2; // location reserved for frame's length
	for(char i=0; i<CHANNEL; i++){
		stream[count++] = i;  // Channel
		type = rand() % 3; // Frame's type (0-2)
		stream[count++] = type;
		switch(type){
		case 0:		// Payload is 0
			break;
		case 1:		// Payload is 4 -> pedestal
			for(int k=0; k<4; k++) stream[count++] = (char)(rand() % 256);
			break;
		case 2:		// Payload is 256 -> Full wave
		default:
			for(int k=0; k<FRAME*2; k++) stream[count++] = (char)(rand() % 256);
			break;
		}
	}
	stream[count++] = 0x33;
	stream[count++] = 0xCC;
	stream[2] = (char)(count >> 8); // Frame's length
	stream[3] = (char)count;
	return count;
}
