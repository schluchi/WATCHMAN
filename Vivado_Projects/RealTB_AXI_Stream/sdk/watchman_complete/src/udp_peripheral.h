/*
 * udp_peripheral.h
 *
 *  Created on: 24 oct. 2018
 *      Author: Anthony
 */

#ifndef SRC_UDP_PERIPHERAL_H_
#define SRC_UDP_PERIPHERAL_H_

#include <stdio.h>
#include <string.h>
#include "data_test.h"
#include "lwip/err.h"
#include "lwip/udp.h"
#include "stdbool.h"
#include "time_hm.h"
#include "interrupt.h"

#if defined (__arm__) || defined (__aarch64__)
#include "xil_printf.h"
#endif

#define REGMAP_SIZE		50
#define MAX_ARRAY_SIZE	3*REGMAP_SIZE
#define PORT_CMD		7
#define PORT_DATA		8


err_t transfer_data(char* frame, uint16_t length);
void udp_cmd_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);
int command_parser(struct pbuf *p, char* return_buf);
int setup_udp_settings(ip_addr_t pc_ipaddr);
void cleanup_udp(void);
void print_ip(char *msg, ip_addr_t *ip);
void print_ip_settings(ip_addr_t *ip, ip_addr_t *mask, ip_addr_t *gw);
/* defined by each RAW mode application */
void tcp_fasttmr(void);
void tcp_slowtmr(void);
int setup_pcb_data(struct udp_pcb * pcb ,ip_addr_t pc_ipaddr, uint16_t port);
int setup_pcb_cmd(struct udp_pcb *pcb, uint16_t port);

#endif /* SRC_UDP_PERIPHERAL_H_ */
