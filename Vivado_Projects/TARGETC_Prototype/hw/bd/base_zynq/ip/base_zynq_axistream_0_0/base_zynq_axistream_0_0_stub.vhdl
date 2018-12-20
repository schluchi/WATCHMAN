-- Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2018.2 (win64) Build 2258646 Thu Jun 14 20:03:12 MDT 2018
-- Date        : Wed Dec 19 14:51:21 2018
-- Host        : Anthony-THINK running 64-bit Service Pack 1  (build 7601)
-- Command     : write_vhdl -force -mode synth_stub
--               C:/Users/Anthony/Hawaii/WATCHMAN/Vivado_Projects/TARGETC_Prototype/hw/bd/base_zynq/ip/base_zynq_axistream_0_0/base_zynq_axistream_0_0_stub.vhdl
-- Design      : base_zynq_axistream_0_0
-- Purpose     : Stub declaration of top-level module interface
-- Device      : xc7z010clg400-1
-- --------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity base_zynq_axistream_0_0 is
  Port ( 
    TestStream : in STD_LOGIC;
    FIFOvalid : in STD_LOGIC;
    FIFOdata : in STD_LOGIC_VECTOR ( 31 downto 0 );
    StreamReady : out STD_LOGIC;
    M_AXIS_ACLK : in STD_LOGIC;
    M_AXIS_ARESETN : in STD_LOGIC;
    M_AXIS_TVALID : out STD_LOGIC;
    M_AXIS_TDATA : out STD_LOGIC_VECTOR ( 31 downto 0 );
    M_AXIS_TSTRB : out STD_LOGIC_VECTOR ( 3 downto 0 );
    M_AXIS_TLAST : out STD_LOGIC;
    M_AXIS_TREADY : in STD_LOGIC
  );

end base_zynq_axistream_0_0;

architecture stub of base_zynq_axistream_0_0 is
attribute syn_black_box : boolean;
attribute black_box_pad_pin : string;
attribute syn_black_box of stub : architecture is true;
attribute black_box_pad_pin of stub : architecture is "TestStream,FIFOvalid,FIFOdata[31:0],StreamReady,M_AXIS_ACLK,M_AXIS_ARESETN,M_AXIS_TVALID,M_AXIS_TDATA[31:0],M_AXIS_TSTRB[3:0],M_AXIS_TLAST,M_AXIS_TREADY";
attribute x_core_info : string;
attribute x_core_info of stub : architecture is "axistream,Vivado 2018.2";
begin
end;
