// Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2018.2 (win64) Build 2258646 Thu Jun 14 20:03:12 MDT 2018
// Date        : Fri Dec 14 16:57:26 2018
// Host        : Anthony-THINK running 64-bit Service Pack 1  (build 7601)
// Command     : write_verilog -force -mode synth_stub
//               C:/Users/Anthony/Documents/Hawaii/WATCHMAN/Vivado_Projects/TARGETC_Prototype/hw/bd/base_zynq/ip/base_zynq_axistream_0_0/base_zynq_axistream_0_0_stub.v
// Design      : base_zynq_axistream_0_0
// Purpose     : Stub declaration of top-level module interface
// Device      : xc7z010clg400-1
// --------------------------------------------------------------------------------

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// The synthesis directives are for Synopsys Synplify support to prevent IO buffer insertion.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
(* x_core_info = "axistream,Vivado 2018.2" *)
module base_zynq_axistream_0_0(TestStream, FIFOvalid, FIFOdata, StreamReady, 
  M_AXIS_ACLK, M_AXIS_ARESETN, M_AXIS_TVALID, M_AXIS_TDATA, M_AXIS_TSTRB, M_AXIS_TLAST, 
  M_AXIS_TREADY)
/* synthesis syn_black_box black_box_pad_pin="TestStream,FIFOvalid,FIFOdata[31:0],StreamReady,M_AXIS_ACLK,M_AXIS_ARESETN,M_AXIS_TVALID,M_AXIS_TDATA[31:0],M_AXIS_TSTRB[3:0],M_AXIS_TLAST,M_AXIS_TREADY" */;
  input TestStream;
  input FIFOvalid;
  input [31:0]FIFOdata;
  output StreamReady;
  input M_AXIS_ACLK;
  input M_AXIS_ARESETN;
  output M_AXIS_TVALID;
  output [31:0]M_AXIS_TDATA;
  output [3:0]M_AXIS_TSTRB;
  output M_AXIS_TLAST;
  input M_AXIS_TREADY;
endmodule
