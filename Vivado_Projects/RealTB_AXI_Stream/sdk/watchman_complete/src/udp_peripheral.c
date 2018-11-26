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

#include "udp_peripheral.h"

int regmap[REGMAP_SIZE];

struct udp_pcb *pcb_data;
struct udp_pcb *pcb_cmd;
struct pbuf *buf_data;
char return_buf[MAX_ARRAY_SIZE];

err_t transfer_data(char* frame, uint16_t length) {
	if(sizeof(frame) <= MAX_STREAM_SIZE){
		buf_data->payload = frame;
		buf_data->tot_len = length;
		buf_data->len = length;
		return udp_send(pcb_data, buf_data);
	}
	else return ERR_BUF;
}

void udp_cmd_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
	int length;
	err_t ret;
    if (p != NULL) {

    	//reset each buffer before each use
    	memset(return_buf, 0, MAX_ARRAY_SIZE);
    	return_buf[0] = 0x55;
    	return_buf[1] = 0xAA;

    	length = command_parser(p, regmap, return_buf);
    	if(length > 0){
    		//send data back
			return_buf[length-2] = 0x33;
			return_buf[length-1] = 0xCC;
			p->payload = return_buf;
			p->len = length;
			p->tot_len = length;
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
	time_cplt offset_time;

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
						nbre_of_bytes = 0;
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
			case 5: // set time
				if(start + 4 + 6 == end){
					offset_time.year = payload[4] + 2000;
					offset_time.month = payload[5];
					offset_time.day = payload[6];
					offset_time.hour = payload[7];
					offset_time.minute = payload[8];
					offset_time.second = payload[9];
					offset_time.milisecond = 0;
					settime_hm(&offset_time);
					xil_printf("Command settime received\r\n");
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

int setup_udp_settings(ip_addr_t pc_ipaddr)
{
	int ret = 0;
	/* Initialize the regmap for test */
	for(int i = 0; i<REGMAP_SIZE; i++) regmap[i] = i;


	/* create new UDP PCB structure for the data */
	ret += setup_pcb_data(pcb_data, pc_ipaddr, PORT_DATA);
	buf_data = pbuf_alloc(PBUF_TRANSPORT,MAX_STREAM_SIZE,PBUF_RAM);

	/* create new UDP PCB structure for the commands */
	ret += setup_pcb_cmd(pcb_cmd, PORT_CMD);

	xil_printf("UDP started @ port %d for data and @ port %d for commands\n\r", PORT_DATA, PORT_CMD);

	return ret;
}

int setup_pcb_data(struct udp_pcb *pcb, ip_addr_t pc_ipaddr, uint16_t port){

	err_t err;

	/* create new UDP PCB structure */
	pcb = udp_new_ip_type(IPADDR_TYPE_ANY);
	if (!pcb) {
		xil_printf("Error creating PCB. Out of Memory\n\r");
		return -1;
	}

	/* bind zynq to specified @port */
	err = udp_bind(pcb, IP_ANY_TYPE, port); //bind = port we are listenning to (zynq input port)
	if (err != ERR_OK) {
		xil_printf("Unable to bind port %d: err = %d\n\r", port, err);
		return -2;
	}

	/*connect zynq to pc @ip addr & port*/
	err = udp_connect(pcb, &pc_ipaddr, port); // connect = the input port of the PC
	if (err != ERR_OK) {
		xil_printf("Unable to bind to port %d: err = %d\n\r", port, err);
		return -2;
	}

	return 0;
}

int setup_pcb_cmd(struct udp_pcb *pcb, uint16_t port){
	err_t err;

	/* create new UDP PCB structure */
	pcb = udp_new_ip_type(IPADDR_TYPE_ANY);
	if (!pcb) {
		xil_printf("Error creating PCB. Out of Memory\n\r");
		return -1;
	}

	/* bind to specified @port */
	err = udp_bind(pcb, IP_ANY_TYPE, port);
	if (err != ERR_OK) {
		xil_printf("Unable to bind port %d: err = %d\n\r", port, err);
		return -2;
	}

	udp_recv(pcb, udp_cmd_recv, NULL);
	return 0;
}

void cleanup_udp(void){
	udp_disconnect(pcb_data);
	udp_remove(pcb_data);
	udp_remove(pcb_cmd);
}

void print_ip(char *msg, ip_addr_t *ip)
{
	printf("%s", msg);
	printf("%d.%d.%d.%d\n\r", ip4_addr1(ip), ip4_addr2(ip),
			ip4_addr3(ip), ip4_addr4(ip));
}

void print_ip_settings(ip_addr_t *ip, ip_addr_t *mask, ip_addr_t *gw)
{

	print_ip("Board IP: ", ip);
	print_ip("Netmask : ", mask);
	print_ip("Gateway : ", gw);
}
