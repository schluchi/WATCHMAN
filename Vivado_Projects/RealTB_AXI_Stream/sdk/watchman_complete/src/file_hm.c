/*
 * log_file.c
 *
 *  Created on: 13 nov. 2018
 *      Author: Anthony
 */

#include "file_hm.h"

char *Path = "0:/";  //  string pointer to the logical drive number
static FATFS FS_instance; // File System instance
static char *log_filename = "log.txt"; // pointer to the log file name
static FIL log_file;	// File instance
static char *wdt_filename = "wdt.txt"; // pointer to the wdt file name
static FIL wdt_file;	// File instance
static char *time_filename = "time.txt"; // pointer to the time file name
static FIL time_file;	// File instance

/****************************************************************************/
/**
* @brief	Mount the SD card
*
* @param	None
*
* @return	FRESULT: see enumeration in ff.h and possibility on note's website
*
* @note		http://elm-chan.org/fsw/ff/00index_e.html for informations abourt
* 			ff.c (Generic FAT Filesystem Module)
*
****************************************************************************/
FRESULT mount_sd_card(void)
{
	FRESULT result = f_mount(&FS_instance,Path, 1);
	if (result != FR_OK) printf("SD card mount failed, result = %d\r\n", result);

	return result;
}

/****************************************************************************/
/**
* @brief	Create the log file to save the log message
*
* @param	None
*
* @return	FRESULT: see enumeration in ff.h and possibility on note's website
*
* @note		http://elm-chan.org/fsw/ff/00index_e.html for informations abourt
* 			ff.c (Generic FAT Filesystem Module)
*
****************************************************************************/
FRESULT create_logfile(void)
{
	FRESULT result;
	result = f_open(&log_file, log_filename, FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
	if(result != FR_OK) printf("Create log file failed during f_open, result = %d\r\n", result);
	else {
		result = f_close(&log_file);
		if(result != FR_OK) printf("Create log file failed during f_close, result = %d\r\n", result);
	}
	return result;
}

/****************************************************************************/
/**
* @brief	Write a log message in the log file with the date and hour
*
* @param	tmp_text: text to be written
* 			length: size of tmp_text (in bytes)
*
* @return	FRESULT: see enumeration in ff.h and possibility on note's website
*
* @note		http://elm-chan.org/fsw/ff/00index_e.html for informations abourt
* 			ff.c (Generic FAT Filesystem Module)
*
****************************************************************************/
FRESULT write_logfile(char *tmp_text, uint length)
{
	static int file_index = 0;
	FRESULT result;
	uint nbr_byte = 0;
	char text[100];
	time_cplt time;

	gettime_hm(&time);
	length += 26;
	if(length < 100){
		sprintf((char *)text, "%02d.%02d.%04d @ %02d:%02d:%02d : %s\r\n",  time.day, time.month, time.year, time.hour, time.minute, time.second, tmp_text);
		result = f_open(&log_file, log_filename,FA_WRITE);
		if(result != FR_OK) printf("Write log file failed during f_open, result = %d\r\n", result);
		else {
			result = f_lseek(&log_file,file_index);
			if(result != FR_OK) printf("Write log file failed during f_lseek, result = %d\r\n", result);
			else{
				result = f_write(&log_file, (const void*)text, length, &nbr_byte);
				if(result != FR_OK) printf("Write log file failed during f_write, result = %d\r\n", result);
				else{
					if(nbr_byte < length){
						result = FR_INT_ERR;
						printf("Write log file failed, disk full\r\n");
					}
					else{
						result = f_close(&log_file);
						if(result != FR_OK) printf("Write log file failed during f_close, result = %d\r\n", result);
					}
					file_index += nbr_byte;
				}
			}
		}
	}
	else{
		result = FR_INVALID_PARAMETER;
		printf("Write log file failed, text too long\r\n");
	}
	return result;
}

/****************************************************************************/
/**
* @brief	Create the wdt file to save the the time when the wdt occurs
*
* @param	None
*
* @return	FRESULT: see enumeration in ff.h and possibility on note's website
*
* @note		http://elm-chan.org/fsw/ff/00index_e.html for informations abourt
* 			ff.c (Generic FAT Filesystem Module)
*
****************************************************************************/
FRESULT create_wdtfile(void)
{
	FRESULT result;
	result = f_open(&wdt_file, wdt_filename, FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
	if(result != FR_OK) printf("Create wdt file failed during f_open, result = %d\r\n", result);
	else {
		result = f_close(&wdt_file);
		if(result != FR_OK) printf("Create wdt file failed during f_close, result = %d\r\n", result);
	}
	return result;
}

/****************************************************************************/
/**
* @brief	Write a message in the wdt file to indicate when the wdt occurs
*
* @param	None
*
* @return	FRESULT: see enumeration in ff.h and possibility on note's website
*
* @note		http://elm-chan.org/fsw/ff/00index_e.html for informations abourt
* 			ff.c (Generic FAT Filesystem Module)
*
****************************************************************************/
FRESULT write_wdtfile(void)
{
	int file_index = 0;
	FRESULT result;
	uint length = 0;
	uint nbr_byte = 0;
	char text[100];
	char time_str[100];
	time_cplt time;

	result = f_open(&time_file, time_filename,FA_READ);
	if(result != FR_OK) printf("Write wdt file failed during f_open of time_file, result = %d\r\n", result);
	else{
		length = 21;
		result = f_read(&time_file, time_str, length, &nbr_byte);
		time_str[21] = '\0'; // end of line
		if(result != FR_OK) printf("Write wdt file failed during f_read of time_file, result = %d\r\n", result);
		else{
			if(nbr_byte < length){
				result = FR_INT_ERR;
				printf("Write wdt file failed, nbr of byte read not enough\r\n");
			}
			else{
				result = f_close(&time_file);
				if(result != FR_OK) printf("Write wdt file failed during f_close of time_file, result = %d\r\n", result);
				else{
					sprintf((char *)text, "%s : watchdog timer occurs!\r\n", time_str);
					stringtotime(time_str, &time);
					settime_hm(&time);
					addtime(); // compensate caused by the reboot

					result = f_open(&wdt_file, wdt_filename,FA_WRITE);
					if(result != FR_OK) printf("Write wdt file failed during f_open, result = %d\r\n", result);
					else {
						file_index = file_size(&wdt_file);
						result = f_lseek(&wdt_file,file_index);
						if(result != FR_OK) printf("Write wdt file failed during f_lseek, result = %d\r\n", result);
						else{
							length = 48;
							result = f_write(&wdt_file, (const void*)text, length, &nbr_byte);
							printf("write_wdtfile: %s", text);
							if(result != FR_OK) printf("Write wdt file failed during f_write, result = %d\r\n", result);
							else{
								if(nbr_byte < length){
									result = FR_INT_ERR;
									printf("Write wdt file failed, disk full\r\n");
								}
								else{
									result = f_close(&wdt_file);
									if(result != FR_OK) printf("Write wdt file failed during f_close, result = %d\r\n", result);
								}
							}
						}
					}
				}
			}
		}
	}

	return result;
}

/****************************************************************************/
/**
* @brief	Create the time file to save the time
*
* @param	None
*
* @return	FRESULT: see enumeration in ff.h and possibility on note's website
*
* @note		http://elm-chan.org/fsw/ff/00index_e.html for informations abourt
* 			ff.c (Generic FAT Filesystem Module)
*
****************************************************************************/
FRESULT create_timefile(void)
{
	FRESULT result;
	result = f_open(&time_file, time_filename, FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
	if(result != FR_OK) printf("Create time file failed during f_open, result = %d\r\n", result);
	else {
		result = f_close(&time_file);
		if(result != FR_OK) printf("Create time file failed during f_close, result = %d\r\n", result);
	}
	return result;
}

/****************************************************************************/
/**
* @brief	Write the time in the time file to know when the wdt occurs
*
* @param	None
*
* @return	FRESULT: see enumeration in ff.h and possibility on note's website
*
* @note		http://elm-chan.org/fsw/ff/00index_e.html for informations abourt
* 			ff.c (Generic FAT Filesystem Module)
*
****************************************************************************/
FRESULT update_timefile(void)
{
	FRESULT result;
	uint length = 21;
	uint nbr_byte = 0;
	int file_index = 0;
	char text[100];
	time_cplt time;

	gettime_hm(&time);

	sprintf((char *)text, "%02d.%02d.%04d @ %02d:%02d:%02d",  time.day, time.month, time.year, time.hour, time.minute, time.second);
	printf("update_timefile: %s\r\n", text);
	result = f_open(&time_file, time_filename,FA_WRITE);
	if(result != FR_OK) printf("Write time file failed during f_open, result = %d\r\n", result);
	else {
		result = f_lseek(&time_file,file_index);
		if(result != FR_OK) printf("Write time file failed during f_lseek, result = %d\r\n", result);
		else{
			result = f_write(&time_file, (const void*)text, length, &nbr_byte);
			if(result != FR_OK) printf("Write time file failed during f_write, result = %d\r\n", result);
			else{
				if(nbr_byte < length){
					result = FR_INT_ERR;
					printf("Write time file failed, disk full\r\n");
				}
				else{
					result = f_close(&time_file);
					if(result != FR_OK) printf("Write time file failed during f_close, result = %d\r\n", result);
				}
			}
		}
	}

	return result;
}
