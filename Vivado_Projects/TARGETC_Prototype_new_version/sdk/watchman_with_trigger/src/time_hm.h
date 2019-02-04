/**
 * @file 	time_hm.h
 * @author	Anthony Schluchin
 * @date	9th November 2018
 * @version 0.0
 */

#ifndef SRC_TIME_HM_H_
#define SRC_TIME_HM_H_

#include <stdbool.h>
#include "xtime_l.h"

/*** Type definition *************************************************/
// Structure to get or set the time
typedef struct time_cplt_st{
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
	int milisecond;
} time_cplt;

/*** Function prototypes *********************************************/
void gettime_hm(time_cplt* t);
void settime_hm(time_cplt* t);
bool isALeapYear(int year);
void addtime(void);
void stringtotime(char* time_str, time_cplt* time);

#endif /* SRC_TIME_HM_H_ */
