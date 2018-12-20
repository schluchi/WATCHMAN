-- Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2018.2 (lin64) Build 2258646 Thu Jun 14 20:02:38 MDT 2018
-- Date        : Wed Nov  7 12:53:31 2018
-- Host        : jonathan-Latitude-E7450 running 64-bit Linux Mint 18.1 Serena
-- Command     : write_vhdl -force -mode synth_stub
--               /home/jonathan/VivadoProjects/00_WATCHMANN/TARGETC_Prototype/hw/bd/base_zynq/ip/base_zynq_xlconcat_1_0/base_zynq_xlconcat_1_0_stub.vhdl
-- Design      : base_zynq_xlconcat_1_0
-- Purpose     : Stub declaration of top-level module interface
-- Device      : xc7z010clg400-1
-- --------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity base_zynq_xlconcat_1_0 is
  Port ( 
    In0 : in STD_LOGIC_VECTOR ( 0 to 0 );
    In1 : in STD_LOGIC_VECTOR ( 0 to 0 );
    dout : out STD_LOGIC_VECTOR ( 1 downto 0 )
  );

end base_zynq_xlconcat_1_0;

architecture stub of base_zynq_xlconcat_1_0 is
attribute syn_black_box : boolean;
attribute black_box_pad_pin : string;
attribute syn_black_box of stub : architecture is true;
attribute black_box_pad_pin of stub : architecture is "In0[0:0],In1[0:0],dout[1:0]";
attribute X_CORE_INFO : string;
attribute X_CORE_INFO of stub : architecture is "xlconcat_v2_1_1_xlconcat,Vivado 2018.2";
begin
end;
