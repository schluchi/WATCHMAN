/**
 * @file 	global.h
 * @author	Anthony Schluchin
 * @date	28th November 2018
 * @version 0.0
 */

#ifndef SRC_GLOBAL_H_
#define SRC_GLOBAL_H_

#include <stdbool.h>
#include <inttypes.h>
#include "xaxidma.h"
#include "axis_peripheral.h"
#include "xparameters.h"
#include "TARGETC_RegisterMap.h"
#include "udp_peripheral.h"

/*** Function prototypes *********************************************/
int init_global_var(void);
void cleanup_global_var(void);

#endif /* SRC_GLOBAL_H_ */
