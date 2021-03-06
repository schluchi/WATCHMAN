// Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2018.2 (lin64) Build 2258646 Thu Jun 14 20:02:38 MDT 2018
// Date        : Wed Nov  7 12:53:31 2018
// Host        : jonathan-Latitude-E7450 running 64-bit Linux Mint 18.1 Serena
// Command     : write_verilog -force -mode synth_stub
//               /home/jonathan/VivadoProjects/00_WATCHMANN/TARGETC_Prototype/hw/bd/base_zynq/ip/base_zynq_xlconcat_1_0/base_zynq_xlconcat_1_0_stub.v
// Design      : base_zynq_xlconcat_1_0
// Purpose     : Stub declaration of top-level module interface
// Device      : xc7z010clg400-1
// --------------------------------------------------------------------------------

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// The synthesis directives are for Synopsys Synplify support to prevent IO buffer insertion.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
(* X_CORE_INFO = "xlconcat_v2_1_1_xlconcat,Vivado 2018.2" *)
module base_zynq_xlconcat_1_0(In0, In1, dout)
/* synthesis syn_black_box black_box_pad_pin="In0[0:0],In1[0:0],dout[1:0]" */;
  input [0:0]In0;
  input [0:0]In1;
  output [1:0]dout;
endmodule
