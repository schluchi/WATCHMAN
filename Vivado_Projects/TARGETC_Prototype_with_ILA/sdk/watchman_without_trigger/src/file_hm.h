/*
 * log_file.h
 *
 *  Created on: 13 nov. 2018
 *      Author: Anthony
 */

#ifndef SRC_FILE_HM_H_
#define SRC_FILE_HM_H_

#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "ff.h"
#include "time_hm.h"

FRESULT mount_sd_card(void);
FRESULT create_logfile(void);
FRESULT write_logfile(char *tmp_text, uint length);
FRESULT create_wdtfile(void);
FRESULT write_wdtfile(void);
FRESULT create_timefile(void);
FRESULT update_timefile(void);

#endif /* SRC_FILE_HM_H_ */
