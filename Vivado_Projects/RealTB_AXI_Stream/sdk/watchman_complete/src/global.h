/*
 * global.h
 *
 *  Created on: 16 nov. 2018
 *      Author: Anthony
 */

#ifndef SRC_GLOBAL_H_
#define SRC_GLOBAL_H_

#include <stdbool.h>
#include <inttypes.h>
#include "xaxidma.h"
#include "axis_peripheral.h"

int init_global_var(void);
void cleanup_global_var(void);

#endif /* SRC_GLOBAL_H_ */
