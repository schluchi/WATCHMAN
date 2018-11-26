/*
 * global.h
 *
 *  Created on: 16 nov. 2018
 *      Author: Anthony
 */

#ifndef SRC_GLOBAL_H_
#define SRC_GLOBAL_H_

#include <stdbool.h>

struct netif *echo_netif;
volatile int count_ttcps_timer;
volatile int count_scu_timer;
volatile bool run_flag;
int *PtrData;
volatile bool stream_flag;
XAxiDma AxiDmaInstance;
char axidma_error, axidma_rx_done;
int nbre_of_bytes;

#endif /* SRC_GLOBAL_H_ */
