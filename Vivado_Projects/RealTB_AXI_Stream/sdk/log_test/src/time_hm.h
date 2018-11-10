/*
 * time_hm.h
 *
 *  Created on: 9 nov. 2018
 *      Author: Anthony
 */

#ifndef SRC_TIME_HM_H_
#define SRC_TIME_HM_H_

#include <stdbool.h>
#include "xtime_l.h"

typedef struct time_cplt_st{
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
	int milisecond;
} time_cplt;

void gettime_hm(time_cplt* t);
void settime_hm(time_cplt* t);
bool isALeapYear(int year);

#endif /* SRC_TIME_HM_H_ */
