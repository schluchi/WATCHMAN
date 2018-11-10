/*Copyright (c) 2015, EmbeddedCentric.com
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

//////////////////////////////////////////////////////////////////////////////////
// Company: EmbeddedCentric.com
// Author:  Ali Aljaani
// Create Date:    08/21/2015
// Description: Zynq SoC Training-lab7
//////////////////////////////////////////////////////////////////////////////////

// Include Files
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

#include "interrupt.h"
#include "xscuwdt.h"
#include "xil_printf.h"

extern bool run_flag;
//----------------------------------------------------
// MAIN FUNCTION
//----------------------------------------------------
int main (void)
{
	bool init_flag = true;
	xil_printf("\r\n\r\nWatchdog test\r\n");
	xil_printf("-----START-----\r\n");

	//----------------------------------------------------
	// SETUP THE TIMER
	//----------------------------------------------------
	if(init_interrupts() == XST_SUCCESS){
		xil_printf("Interrupts initialization pass!\r\n");
		run_flag = true;
	}
	else{
		xil_printf("Interrupts initialization failed!\r\n");
		init_flag = false;
	}

	if(init_flag){
		while(run_flag);
	}

	cleanup_interrupts();
	// Never reached on normal execution
	xil_printf("------END------\r\n");
	return (0);
}



