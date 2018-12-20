// Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2018.2 (win64) Build 2258646 Thu Jun 14 20:03:12 MDT 2018
// Date        : Tue Dec 18 15:35:01 2018
// Host        : Anthony-THINK running 64-bit Service Pack 1  (build 7601)
// Command     : write_verilog -force -mode synth_stub
//               C:/Users/Anthony/Documents/Hawaii/WATCHMAN/TARGETC_Prototype/hw/bd/base_zynq/ip/base_zynq_FifoManagerV4_0_0/base_zynq_FifoManagerV4_0_0_stub.v
// Design      : base_zynq_FifoManagerV4_0_0
// Purpose     : Stub declaration of top-level module interface
// Device      : xc7z010clg400-1
// --------------------------------------------------------------------------------

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// The synthesis directives are for Synopsys Synplify support to prevent IO buffer insertion.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
(* x_core_info = "FifoManagerV4,Vivado 2018.2" *)
module base_zynq_FifoManagerV4_0_0(nRST, CLK, PRECvalid, FIFOresponse, WDOTime, 
  DIGTime, Trigger, WDONBR, CH0, CH1, CH2, CH3, CH4, CH5, CH6, CH7, CH8, CH9, CH10, CH11, CH12, CH13, CH14, CH15, 
  FIFOvalid, ready_i, PSBUSY_i, DataOut)
/* synthesis syn_black_box black_box_pad_pin="nRST,CLK,PRECvalid,FIFOresponse,WDOTime[63:0],DIGTime[63:0],Trigger[31:0],WDONBR[8:0],CH0[11:0],CH1[11:0],CH2[11:0],CH3[11:0],CH4[11:0],CH5[11:0],CH6[11:0],CH7[11:0],CH8[11:0],CH9[11:0],CH10[11:0],CH11[11:0],CH12[11:0],CH13[11:0],CH14[11:0],CH15[11:0],FIFOvalid,ready_i,PSBUSY_i,DataOut[31:0]" */;
  input nRST;
  input CLK;
  input PRECvalid;
  output FIFOresponse;
  input [63:0]WDOTime;
  input [63:0]DIGTime;
  input [31:0]Trigger;
  input [8:0]WDONBR;
  input [11:0]CH0;
  input [11:0]CH1;
  input [11:0]CH2;
  input [11:0]CH3;
  input [11:0]CH4;
  input [11:0]CH5;
  input [11:0]CH6;
  input [11:0]CH7;
  input [11:0]CH8;
  input [11:0]CH9;
  input [11:0]CH10;
  input [11:0]CH11;
  input [11:0]CH12;
  input [11:0]CH13;
  input [11:0]CH14;
  input [11:0]CH15;
  output FIFOvalid;
  input ready_i;
  input PSBUSY_i;
  output [31:0]DataOut;
endmodule
