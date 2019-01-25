/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

#include "echo.h"

int regmap[REGMAP_SIZE];

struct udp_pcb *packet_pcb;
struct pbuf *packet;
struct pbuf *echo_buf;
volatile bool run_flag = true;
volatile bool stream_flag = false;

char return_buf[MAX_ARRAY_SIZE];
extern volatile int count_ttcps_timer;
extern volatile int count_scu_timer;

err_t transfer_data(char* frame, uint16_t length) {
	if(sizeof(frame) <= MAX_STREAM_SIZE){
		packet->payload = frame;
		packet->tot_len = length;
		packet->len = length;
		return udp_send(packet_pcb, packet);
	}
	else return ERR_BUF;
}

void udp_echo_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
	int length;
	err_t ret;
    if (p != NULL) {

    	//reset each buffer before each use
//    	memset(return_buf, 0, MAX_ARRAY_SIZE);
//    	return_buf[0] = 0x55;
//    	return_buf[1] = 0xAA;
//
//    	length = command_parser(p, regmap, return_buf);
//    	if(length > 0){
//    		//send data back
//			return_buf[length-2] = 0x33;
//			return_buf[length-1] = 0xCC;
//			p->payload = return_buf;
//			p->len = length;
//			p->tot_len = length;
//			ret = udp_sendto(pcb, p, addr, port);
//		}
//    	else xil_printf("ERROR with the command received\r\n");
    	char* payload = p->payload;
    	printf("data received =");
    	if(p->len > 0){
    		for(int i=0; i<(p->len-1); i++) printf(" %d |",payload[i]);
    		printf(" %d\r\n",payload[p->len-1]);
    		ret = udp_sendto(pcb, p, addr, port);
    	}
    	else xil_printf("ERROR with the command received\r\n");
    }
    	free(p);

}

int command_parser(struct pbuf *p, int regmap[], char* return_buf){
	char* payload = p->payload;
	uint16_t length = p->len;
	uint16_t start = 0;
	uint16_t end = 0;
	bool flag_start = false;
	bool flag_end = false;
	int i, k;

	while((start < (length-1)) && (flag_start == false)){
		if((payload[start] == 0x55) && (payload[start+1] == 0xAA)) flag_start = true;
		else start++;
	}

	while((end < (length-1)) && (flag_end == false)){
		if((payload[end] == 0x33) && (payload[end+1] == 0xCC)) flag_end = true;
		else end++;
	}

	if(flag_start && flag_end){
		return_buf[2] = payload[start+2];
		return_buf[3] = payload[start+3];
		switch(payload[start+2]){
			case 0: // cmd write all reg.
				if(start + 4 + 2*REGMAP_SIZE == end){
					k = 0;
					for(i = 4; i < (4 + 2*REGMAP_SIZE); i += 2){
						regmap[k] = payload[i]*256 + payload[i+1];
						k++;
					}
					xil_printf("Command write_all_reg received\r\n");
					return 6;
				}
				else return -1;
				break;
			case 1: // cmd read all reg
				if(start + 4 == end){
					k = 0;
					for(i = 4; i < (4 + 2*REGMAP_SIZE); i += 2){
						return_buf[i] = (char)(regmap[k] >> 8);
						return_buf[i+1] = (char)(regmap[k]);
						k++;
					}
					xil_printf("Command read_all_reg received\r\n");
					return (6 + 2*REGMAP_SIZE);
				}
				else return -1;
				break;
			case 2: // cmd ping
				if(start + 4 == end){
					xil_printf("Command ping received\r\n");
					return 6;
				}
				else return -1;
				break;
			case 3: // cmd start stream
				if(start + 4 == end){
					if(stream_flag){
						stream_flag = false;
						xil_printf("Command stop_stream received\r\n");
					}
					else{
						stream_flag = true;
						xil_printf("Command start_stream received\r\n");
						count_ttcps_timer = 0;
						count_scu_timer = 0;
					}
					return 6;
				}
				else return -1;
				break;
			case 4: // cmd stop uC
				if(start + 4 == end){
					xil_printf("Command stop_uC received\r\n");
					run_flag = false;
					return 6;
				}
				else return -1;
				break;
			default:
				return -1;

		}
	}
	else return -1;
}

void print_app_header()
{
	xil_printf("-----lwIP UDP echo server ------\n\r");
	xil_printf("UDP packets sent to port %d will be echoed back\n\r", PORT_ECHO);
}

int start_application(ip_addr_t pc_ipaddr)
{
	struct udp_pcb *pcb;
	err_t err;
	unsigned port = PORT_ECHO;

//	for(int i = 0; i<REGMAP_SIZE; i++){
//		regmap[i] = i;
//		printf("App: regmap[%d] = %d\n", i, regmap[i]);
//	}


	/* create new UDP PCB structure */
//	packet_pcb = setup_send_data(packet_pcb, pc_ipaddr, PORT_DATA);
//	packet = pbuf_alloc(PBUF_TRANSPORT,MAX_STREAM_SIZE,PBUF_RAM);
//	echo_buf = pbuf_alloc(PBUF_TRANSPORT,3*REGMAP_SIZE,PBUF_RAM);//5620,PBUF_RAM);

	/* create new TCP PCB structure */
	pcb = udp_new_ip_type(IPADDR_TYPE_ANY);
	if (!pcb) {
		xil_printf("Error creating PCB. Out of Memory\n\r");
		return -1;
	}

	/* bind to specified @port */
	err = udp_bind(pcb, IP_ANY_TYPE, port);
	if (err != ERR_OK) {
		xil_printf("Unable to bind to port %d: err = %d\n\r", port, err);
		return -2;
	}
	udp_recv(pcb, udp_echo_recv, NULL);

	xil_printf("UDP echo server started @ port %d\n\r", port);


	return 0;
}

void print_ip(char *msg, ip_addr_t *ip)
{
	print(msg);
	xil_printf("%d.%d.%d.%d\n\r", ip4_addr1(ip), ip4_addr2(ip),
			ip4_addr3(ip), ip4_addr4(ip));
}

void print_ip_settings(ip_addr_t *ip, ip_addr_t *mask, ip_addr_t *gw)
{

	print_ip("Board IP: ", ip);
	print_ip("Netmask : ", mask);
	print_ip("Gateway : ", gw);
}
