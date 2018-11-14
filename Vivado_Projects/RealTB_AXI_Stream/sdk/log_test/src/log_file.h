/*
 * log_file.h
 *
 *  Created on: 13 nov. 2018
 *      Author: Anthony
 */

#ifndef SRC_LOG_FILE_H_
#define SRC_LOG_FILE_H_

#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "ff.h"
#include "time_hm.h"

FRESULT mount_sd_card(void);
FRESULT create_logfile(void);
FRESULT write_logfile(char *tmp_text, uint length);

#endif /* SRC_LOG_FILE_H_ */
