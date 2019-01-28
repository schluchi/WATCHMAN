-- Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2018.2 (lin64) Build 2258646 Thu Jun 14 20:02:38 MDT 2018
-- Date        : Fri Jan 18 08:09:58 2019
-- Host        : jonathan-Latitude-E7450 running 64-bit Linux Mint 18.1 Serena
-- Command     : write_vhdl -force -mode synth_stub
--               /home/jonathan/Desktop/LinktoTARGETC_Prototype/hw/bd/base_zynq/ip/base_zynq_iobuf_0_1/base_zynq_iobuf_0_1_stub.vhdl
-- Design      : base_zynq_iobuf_0_1
-- Purpose     : Stub declaration of top-level module interface
-- Device      : xc7z010clg400-1
-- --------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity base_zynq_iobuf_0_1 is
  Port ( 
    O : out STD_LOGIC;
    IO : inout STD_LOGIC;
    I : in STD_LOGIC;
    T : in STD_LOGIC
  );

end base_zynq_iobuf_0_1;

architecture stub of base_zynq_iobuf_0_1 is
attribute syn_black_box : boolean;
attribute black_box_pad_pin : string;
attribute syn_black_box of stub : architecture is true;
attribute black_box_pad_pin of stub : architecture is "O,IO,I,T";
attribute x_core_info : string;
attribute x_core_info of stub : architecture is "iobuf,Vivado 2018.2";
begin
end;
