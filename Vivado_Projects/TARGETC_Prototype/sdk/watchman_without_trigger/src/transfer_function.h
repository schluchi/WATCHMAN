/*
 * transfer_function.h
 *
 *  Created on: 7 janv. 2019
 *      Author: antho
 */

#ifndef SRC_TRANSFER_FUNCTION_H_
#define SRC_TRANSFER_FUNCTION_H_

#include "xstatus.h"
#include "data_analysis.h"
#include "xil_types.h"
#include "axis_peripheral.h"
#include "TARGETC_RegisterMap.h"
#include "iic_DAC_LTC2657.h"
#include "udp_peripheral.h"

int send_data_transfer_fct(void);


#endif /* SRC_TRANSFER_FUNCTION_H_ */
