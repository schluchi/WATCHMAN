/*
 * time_hm.c
 *
 *  Created on: 9 nov. 2018
 *      Author: Anthony
 */

#include "time_hm.h"

time_cplt offset_time = {
	.year = 2000,
	.month = 1,
	.day = 1,
	.hour = 0,
	.minute = 0,
	.second = 0,
	.milisecond = 0
	};

uint64_t offset_timer = 0;

int day_per_month[13] = {(int)NULL, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

void gettime_hm(time_cplt* t){
	XTime time;
	XTime_GetTime(&time);

	time = time - offset_timer;

	time = (time / (COUNTS_PER_SECOND/1000)) + offset_time.milisecond;
	t->milisecond = time % 1000;
	time -= t->milisecond;

	time = (time / 1000) + offset_time.second;
	t->second = time % 60;
	time -= t->second;

	time = (time / 60) + offset_time.minute;
	t->minute = time % 60;
	time -= t->minute;

	time = (time / 60) + offset_time.hour;
	t->hour = time % 24;
	time -= t->hour;

	time = time / 24;
	t->day = offset_time.day;
	t->month = offset_time.month;
	t->year = offset_time.year;

	if(isALeapYear(t->year)) day_per_month[2] = 29;
	else day_per_month[2] = 28;

	while(time > 0){
		t->day++;
		if(t->day > day_per_month[t->month]){
			t->day = 1;
			t->month++;
			if(t->month > 12){
				t->month = 1;
				t->year++;
				if(isALeapYear(t->year)) day_per_month[2] = 29;
				else day_per_month[2] = 28;
			}
		}
		time--;
	}
}

void settime_hm(time_cplt* t){
	offset_time.milisecond = t->milisecond % 1000;
	offset_time.second = t->second % 60;
	offset_time.minute = t->minute % 60;
	offset_time.hour = t->hour % 24;

	offset_time.year = t->year;
	if((t->month > 0) && (t->month <= 12)) offset_time.month = t->month;
	if(isALeapYear(offset_time.year)) day_per_month[2] = 29;
	else day_per_month[2] = 28;
	if((t->day > 0) && (t->day <= day_per_month[t->month])) offset_time.day = t->day;

	XTime_GetTime(&offset_timer);
}

bool isALeapYear(int year){
	bool ret;
	/*
	 * https://en.wikipedia.org/wiki/Leap_year
	 * if (year is not divisible by 4) then (it is a common year)
	 * else if (year is not divisible by 100) then (it is a leap year)
	 * else if (year is not divisible by 400) then (it is a common year)
	 * else (it is a leap year)
	 */
	if((year % 4) != 0) ret = false;
	else{
		if((year % 100) != 0) ret = true;
		else{
			if((year % 400) != 0) ret = false;
			else ret = true;
		}
	}
	return ret;
}




