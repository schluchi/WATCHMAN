-- Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2018.2 (win64) Build 2258646 Thu Jun 14 20:03:12 MDT 2018
-- Date        : Sun Jan 27 18:58:41 2019
-- Host        : LAPTOP-5FF9V25L running 64-bit major release  (build 9200)
-- Command     : write_vhdl -force -mode synth_stub
--               C:/Users/antho/Documents/WATCHMAN/Vivado_Projects/TARGETC_Prototype_new_version/hw/bd/base_zynq/ip/base_zynq_FifoManagerV4_0_0/base_zynq_FifoManagerV4_0_0_stub.vhdl
-- Design      : base_zynq_FifoManagerV4_0_0
-- Purpose     : Stub declaration of top-level module interface
-- Device      : xc7z010clg400-1
-- --------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity base_zynq_FifoManagerV4_0_0 is
  Port ( 
    nRST : in STD_LOGIC;
    CLK : in STD_LOGIC;
    PRECvalid : in STD_LOGIC;
    FIFOresponse : out STD_LOGIC;
    TestFIFO : in STD_LOGIC;
    FIFO_ReadEn : out STD_LOGIC;
    FIFO_Time : in STD_LOGIC_VECTOR ( 63 downto 0 );
    FIFO_WdoAddr : in STD_LOGIC_VECTOR ( 8 downto 0 );
    FIFO_TrigInfo : in STD_LOGIC_VECTOR ( 11 downto 0 );
    FIFO_Spare : in STD_LOGIC_VECTOR ( 9 downto 0 );
    FIFO_Empty : in STD_LOGIC;
    CH0 : in STD_LOGIC_VECTOR ( 11 downto 0 );
    CH1 : in STD_LOGIC_VECTOR ( 11 downto 0 );
    CH2 : in STD_LOGIC_VECTOR ( 11 downto 0 );
    CH3 : in STD_LOGIC_VECTOR ( 11 downto 0 );
    CH4 : in STD_LOGIC_VECTOR ( 11 downto 0 );
    CH5 : in STD_LOGIC_VECTOR ( 11 downto 0 );
    CH6 : in STD_LOGIC_VECTOR ( 11 downto 0 );
    CH7 : in STD_LOGIC_VECTOR ( 11 downto 0 );
    CH8 : in STD_LOGIC_VECTOR ( 11 downto 0 );
    CH9 : in STD_LOGIC_VECTOR ( 11 downto 0 );
    CH10 : in STD_LOGIC_VECTOR ( 11 downto 0 );
    CH11 : in STD_LOGIC_VECTOR ( 11 downto 0 );
    CH12 : in STD_LOGIC_VECTOR ( 11 downto 0 );
    CH13 : in STD_LOGIC_VECTOR ( 11 downto 0 );
    CH14 : in STD_LOGIC_VECTOR ( 11 downto 0 );
    CH15 : in STD_LOGIC_VECTOR ( 11 downto 0 );
    FIFOvalid : out STD_LOGIC;
    ready_i : in STD_LOGIC;
    PSBUSY_i : in STD_LOGIC;
    DataOut : out STD_LOGIC_VECTOR ( 31 downto 0 )
  );

end base_zynq_FifoManagerV4_0_0;

architecture stub of base_zynq_FifoManagerV4_0_0 is
attribute syn_black_box : boolean;
attribute black_box_pad_pin : string;
attribute syn_black_box of stub : architecture is true;
attribute black_box_pad_pin of stub : architecture is "nRST,CLK,PRECvalid,FIFOresponse,TestFIFO,FIFO_ReadEn,FIFO_Time[63:0],FIFO_WdoAddr[8:0],FIFO_TrigInfo[11:0],FIFO_Spare[9:0],FIFO_Empty,CH0[11:0],CH1[11:0],CH2[11:0],CH3[11:0],CH4[11:0],CH5[11:0],CH6[11:0],CH7[11:0],CH8[11:0],CH9[11:0],CH10[11:0],CH11[11:0],CH12[11:0],CH13[11:0],CH14[11:0],CH15[11:0],FIFOvalid,ready_i,PSBUSY_i,DataOut[31:0]";
attribute x_core_info : string;
attribute x_core_info of stub : architecture is "FifoManagerV4,Vivado 2018.2";
begin
end;
