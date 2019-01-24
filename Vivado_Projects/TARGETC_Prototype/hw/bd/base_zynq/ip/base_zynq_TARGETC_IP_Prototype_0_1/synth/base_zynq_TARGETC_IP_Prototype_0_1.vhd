-- (c) Copyright 1995-2018 Xilinx, Inc. All rights reserved.
-- 
-- This file contains confidential and proprietary information
-- of Xilinx, Inc. and is protected under U.S. and
-- international copyright and other intellectual property
-- laws.
-- 
-- DISCLAIMER
-- This disclaimer is not a license and does not grant any
-- rights to the materials distributed herewith. Except as
-- otherwise provided in a valid license issued to you by
-- Xilinx, and to the maximum extent permitted by applicable
-- law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
-- WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
-- AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
-- BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
-- INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
-- (2) Xilinx shall not be liable (whether in contract or tort,
-- including negligence, or under any other theory of
-- liability) for any loss or damage of any kind or nature
-- related to, arising under or in connection with these
-- materials, including for any direct, or any indirect,
-- special, incidental, or consequential loss or damage
-- (including loss of data, profits, goodwill, or any type of
-- loss or damage suffered as a result of any action brought
-- by a third party) even if such damage or loss was
-- reasonably foreseeable or Xilinx had been advised of the
-- possibility of the same.
-- 
-- CRITICAL APPLICATIONS
-- Xilinx products are not designed or intended to be fail-
-- safe, or for use in any application requiring fail-safe
-- performance, such as life-support or safety devices or
-- systems, Class III medical devices, nuclear facilities,
-- applications related to the deployment of airbags, or any
-- other applications that could lead to death, personal
-- injury, or severe property or environmental damage
-- (individually and collectively, "Critical
-- Applications"). Customer assumes the sole risk and
-- liability of any use of Xilinx products in Critical
-- Applications, subject only to applicable laws and
-- regulations governing limitations on product liability.
-- 
-- THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
-- PART OF THIS FILE AT ALL TIMES.
-- 
-- DO NOT MODIFY THIS FILE.

-- IP VLNV: xilinx.com:module_ref:TARGETC_IP_Prototype:1.0
-- IP Revision: 1

LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.numeric_std.ALL;

ENTITY base_zynq_TARGETC_IP_Prototype_0_1 IS
  PORT (
    RefCLK_i1 : IN STD_LOGIC;
    RefCLK_i2 : IN STD_LOGIC;
    tc_axi_aclk : IN STD_LOGIC;
    tc_axi_aresetn : IN STD_LOGIC;
    tc_axi_awaddr : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    tc_axi_awprot : IN STD_LOGIC_VECTOR(2 DOWNTO 0);
    tc_axi_awvalid : IN STD_LOGIC;
    tc_axi_awready : OUT STD_LOGIC;
    tc_axi_wdata : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    tc_axi_wstrb : IN STD_LOGIC_VECTOR(3 DOWNTO 0);
    tc_axi_wvalid : IN STD_LOGIC;
    tc_axi_wready : OUT STD_LOGIC;
    tc_axi_bresp : OUT STD_LOGIC_VECTOR(1 DOWNTO 0);
    tc_axi_bvalid : OUT STD_LOGIC;
    tc_axi_bready : IN STD_LOGIC;
    tc_axi_araddr : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    tc_axi_arprot : IN STD_LOGIC_VECTOR(2 DOWNTO 0);
    tc_axi_arvalid : IN STD_LOGIC;
    tc_axi_arready : OUT STD_LOGIC;
    tc_axi_rdata : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    tc_axi_rresp : OUT STD_LOGIC_VECTOR(1 DOWNTO 0);
    tc_axi_rvalid : OUT STD_LOGIC;
    tc_axi_rready : IN STD_LOGIC;
    SIN : OUT STD_LOGIC;
    SCLK : OUT STD_LOGIC;
    PCLK : OUT STD_LOGIC;
    SHOUT : IN STD_LOGIC;
    HSCLK_P : OUT STD_LOGIC;
    HSCLK_N : OUT STD_LOGIC;
    WR_RS_S0 : OUT STD_LOGIC;
    WR_RS_S1 : OUT STD_LOGIC;
    WR_CS_S0 : OUT STD_LOGIC;
    WR_CS_S1 : OUT STD_LOGIC;
    WR_CS_S2 : OUT STD_LOGIC;
    WR_CS_S3 : OUT STD_LOGIC;
    WR_CS_S4 : OUT STD_LOGIC;
    WR_CS_S5 : OUT STD_LOGIC;
    GCC_RESET : OUT STD_LOGIC;
    WL_CLK_P : OUT STD_LOGIC;
    WL_CLK_N : OUT STD_LOGIC;
    RDAD_CLK : OUT STD_LOGIC;
    RDAD_SIN : OUT STD_LOGIC;
    RDAD_DIR : OUT STD_LOGIC;
    SAMPLESEL_ANY : OUT STD_LOGIC;
    DO : IN STD_LOGIC_VECTOR(15 DOWNTO 0);
    SS_INCR : OUT STD_LOGIC;
    DOE : OUT STD_LOGIC;
    DONE : IN STD_LOGIC;
    SS_RESET : OUT STD_LOGIC;
    REGCLR : OUT STD_LOGIC;
    SS_LD_SIN : OUT STD_LOGIC;
    SS_LD_DIR : OUT STD_LOGIC;
    RAMP : OUT STD_LOGIC;
    SSTIN_P : OUT STD_LOGIC;
    SSTIN_N : OUT STD_LOGIC;
    MONTIMING_P : IN STD_LOGIC;
    MONTIMING_N : IN STD_LOGIC;
    WDOTime : OUT STD_LOGIC_VECTOR(63 DOWNTO 0);
    DIGTime : OUT STD_LOGIC_VECTOR(63 DOWNTO 0);
    Trigger : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    WDONbr : OUT STD_LOGIC_VECTOR(8 DOWNTO 0);
    TestStream : OUT STD_LOGIC;
    PSBusy : OUT STD_LOGIC;
    FIFOresponse : IN STD_LOGIC;
    CH0 : OUT STD_LOGIC_VECTOR(11 DOWNTO 0);
    CH1 : OUT STD_LOGIC_VECTOR(11 DOWNTO 0);
    CH2 : OUT STD_LOGIC_VECTOR(11 DOWNTO 0);
    CH3 : OUT STD_LOGIC_VECTOR(11 DOWNTO 0);
    CH4 : OUT STD_LOGIC_VECTOR(11 DOWNTO 0);
    CH5 : OUT STD_LOGIC_VECTOR(11 DOWNTO 0);
    CH6 : OUT STD_LOGIC_VECTOR(11 DOWNTO 0);
    CH7 : OUT STD_LOGIC_VECTOR(11 DOWNTO 0);
    CH8 : OUT STD_LOGIC_VECTOR(11 DOWNTO 0);
    CH9 : OUT STD_LOGIC_VECTOR(11 DOWNTO 0);
    CH10 : OUT STD_LOGIC_VECTOR(11 DOWNTO 0);
    CH11 : OUT STD_LOGIC_VECTOR(11 DOWNTO 0);
    CH12 : OUT STD_LOGIC_VECTOR(11 DOWNTO 0);
    CH13 : OUT STD_LOGIC_VECTOR(11 DOWNTO 0);
    CH14 : OUT STD_LOGIC_VECTOR(11 DOWNTO 0);
    CH15 : OUT STD_LOGIC_VECTOR(11 DOWNTO 0);
    SSvalid : OUT STD_LOGIC;
    TrigA : IN STD_LOGIC;
    TrigB : IN STD_LOGIC;
    TrigC : IN STD_LOGIC;
    TrigD : IN STD_LOGIC;
    TrigA_intr : OUT STD_LOGIC;
    TrigB_intr : OUT STD_LOGIC;
    TrigC_intr : OUT STD_LOGIC;
    TrigD_intr : OUT STD_LOGIC;
    SSVALID_INTR : OUT STD_LOGIC;
    SSTIN : OUT STD_LOGIC;
    MONTIMING : OUT STD_LOGIC;
    RAMP_CNT : OUT STD_LOGIC
  );
END base_zynq_TARGETC_IP_Prototype_0_1;

ARCHITECTURE base_zynq_TARGETC_IP_Prototype_0_1_arch OF base_zynq_TARGETC_IP_Prototype_0_1 IS
  ATTRIBUTE DowngradeIPIdentifiedWarnings : STRING;
  ATTRIBUTE DowngradeIPIdentifiedWarnings OF base_zynq_TARGETC_IP_Prototype_0_1_arch: ARCHITECTURE IS "yes";
  COMPONENT TARGETC_IP_Prototype IS
    PORT (
      RefCLK_i1 : IN STD_LOGIC;
      RefCLK_i2 : IN STD_LOGIC;
      tc_axi_aclk : IN STD_LOGIC;
      tc_axi_aresetn : IN STD_LOGIC;
      tc_axi_awaddr : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
      tc_axi_awprot : IN STD_LOGIC_VECTOR(2 DOWNTO 0);
      tc_axi_awvalid : IN STD_LOGIC;
      tc_axi_awready : OUT STD_LOGIC;
      tc_axi_wdata : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
      tc_axi_wstrb : IN STD_LOGIC_VECTOR(3 DOWNTO 0);
      tc_axi_wvalid : IN STD_LOGIC;
      tc_axi_wready : OUT STD_LOGIC;
      tc_axi_bresp : OUT STD_LOGIC_VECTOR(1 DOWNTO 0);
      tc_axi_bvalid : OUT STD_LOGIC;
      tc_axi_bready : IN STD_LOGIC;
      tc_axi_araddr : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
      tc_axi_arprot : IN STD_LOGIC_VECTOR(2 DOWNTO 0);
      tc_axi_arvalid : IN STD_LOGIC;
      tc_axi_arready : OUT STD_LOGIC;
      tc_axi_rdata : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
      tc_axi_rresp : OUT STD_LOGIC_VECTOR(1 DOWNTO 0);
      tc_axi_rvalid : OUT STD_LOGIC;
      tc_axi_rready : IN STD_LOGIC;
      SIN : OUT STD_LOGIC;
      SCLK : OUT STD_LOGIC;
      PCLK : OUT STD_LOGIC;
      SHOUT : IN STD_LOGIC;
      HSCLK_P : OUT STD_LOGIC;
      HSCLK_N : OUT STD_LOGIC;
      WR_RS_S0 : OUT STD_LOGIC;
      WR_RS_S1 : OUT STD_LOGIC;
      WR_CS_S0 : OUT STD_LOGIC;
      WR_CS_S1 : OUT STD_LOGIC;
      WR_CS_S2 : OUT STD_LOGIC;
      WR_CS_S3 : OUT STD_LOGIC;
      WR_CS_S4 : OUT STD_LOGIC;
      WR_CS_S5 : OUT STD_LOGIC;
      GCC_RESET : OUT STD_LOGIC;
      WL_CLK_P : OUT STD_LOGIC;
      WL_CLK_N : OUT STD_LOGIC;
      RDAD_CLK : OUT STD_LOGIC;
      RDAD_SIN : OUT STD_LOGIC;
      RDAD_DIR : OUT STD_LOGIC;
      SAMPLESEL_ANY : OUT STD_LOGIC;
      DO : IN STD_LOGIC_VECTOR(15 DOWNTO 0);
      SS_INCR : OUT STD_LOGIC;
      DOE : OUT STD_LOGIC;
      DONE : IN STD_LOGIC;
      SS_RESET : OUT STD_LOGIC;
      REGCLR : OUT STD_LOGIC;
      SS_LD_SIN : OUT STD_LOGIC;
      SS_LD_DIR : OUT STD_LOGIC;
      RAMP : OUT STD_LOGIC;
      SSTIN_P : OUT STD_LOGIC;
      SSTIN_N : OUT STD_LOGIC;
      MONTIMING_P : IN STD_LOGIC;
      MONTIMING_N : IN STD_LOGIC;
      WDOTime : OUT STD_LOGIC_VECTOR(63 DOWNTO 0);
      DIGTime : OUT STD_LOGIC_VECTOR(63 DOWNTO 0);
      Trigger : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
      WDONbr : OUT STD_LOGIC_VECTOR(8 DOWNTO 0);
      TestStream : OUT STD_LOGIC;
      PSBusy : OUT STD_LOGIC;
      FIFOresponse : IN STD_LOGIC;
      CH0 : OUT STD_LOGIC_VECTOR(11 DOWNTO 0);
      CH1 : OUT STD_LOGIC_VECTOR(11 DOWNTO 0);
      CH2 : OUT STD_LOGIC_VECTOR(11 DOWNTO 0);
      CH3 : OUT STD_LOGIC_VECTOR(11 DOWNTO 0);
      CH4 : OUT STD_LOGIC_VECTOR(11 DOWNTO 0);
      CH5 : OUT STD_LOGIC_VECTOR(11 DOWNTO 0);
      CH6 : OUT STD_LOGIC_VECTOR(11 DOWNTO 0);
      CH7 : OUT STD_LOGIC_VECTOR(11 DOWNTO 0);
      CH8 : OUT STD_LOGIC_VECTOR(11 DOWNTO 0);
      CH9 : OUT STD_LOGIC_VECTOR(11 DOWNTO 0);
      CH10 : OUT STD_LOGIC_VECTOR(11 DOWNTO 0);
      CH11 : OUT STD_LOGIC_VECTOR(11 DOWNTO 0);
      CH12 : OUT STD_LOGIC_VECTOR(11 DOWNTO 0);
      CH13 : OUT STD_LOGIC_VECTOR(11 DOWNTO 0);
      CH14 : OUT STD_LOGIC_VECTOR(11 DOWNTO 0);
      CH15 : OUT STD_LOGIC_VECTOR(11 DOWNTO 0);
      SSvalid : OUT STD_LOGIC;
      TrigA : IN STD_LOGIC;
      TrigB : IN STD_LOGIC;
      TrigC : IN STD_LOGIC;
      TrigD : IN STD_LOGIC;
      TrigA_intr : OUT STD_LOGIC;
      TrigB_intr : OUT STD_LOGIC;
      TrigC_intr : OUT STD_LOGIC;
      TrigD_intr : OUT STD_LOGIC;
      SSVALID_INTR : OUT STD_LOGIC;
      SSTIN : OUT STD_LOGIC;
      MONTIMING : OUT STD_LOGIC;
      RAMP_CNT : OUT STD_LOGIC
    );
  END COMPONENT TARGETC_IP_Prototype;
  ATTRIBUTE X_CORE_INFO : STRING;
  ATTRIBUTE X_CORE_INFO OF base_zynq_TARGETC_IP_Prototype_0_1_arch: ARCHITECTURE IS "TARGETC_IP_Prototype,Vivado 2018.2";
  ATTRIBUTE CHECK_LICENSE_TYPE : STRING;
  ATTRIBUTE CHECK_LICENSE_TYPE OF base_zynq_TARGETC_IP_Prototype_0_1_arch : ARCHITECTURE IS "base_zynq_TARGETC_IP_Prototype_0_1,TARGETC_IP_Prototype,{}";
  ATTRIBUTE CORE_GENERATION_INFO : STRING;
  ATTRIBUTE CORE_GENERATION_INFO OF base_zynq_TARGETC_IP_Prototype_0_1_arch: ARCHITECTURE IS "base_zynq_TARGETC_IP_Prototype_0_1,TARGETC_IP_Prototype,{x_ipProduct=Vivado 2018.2,x_ipVendor=xilinx.com,x_ipLibrary=module_ref,x_ipName=TARGETC_IP_Prototype,x_ipVersion=1.0,x_ipCoreRevision=1,x_ipLanguage=VHDL,x_ipSimLanguage=MIXED}";
  ATTRIBUTE IP_DEFINITION_SOURCE : STRING;
  ATTRIBUTE IP_DEFINITION_SOURCE OF base_zynq_TARGETC_IP_Prototype_0_1_arch: ARCHITECTURE IS "module_ref";
  ATTRIBUTE X_INTERFACE_INFO : STRING;
  ATTRIBUTE X_INTERFACE_PARAMETER : STRING;
  ATTRIBUTE X_INTERFACE_PARAMETER OF SSVALID_INTR: SIGNAL IS "XIL_INTERFACENAME SSVALID_INTR, SENSITIVITY LEVEL_HIGH, PortWidth 1";
  ATTRIBUTE X_INTERFACE_INFO OF SSVALID_INTR: SIGNAL IS "xilinx.com:signal:interrupt:1.0 SSVALID_INTR INTERRUPT";
  ATTRIBUTE X_INTERFACE_PARAMETER OF TrigD_intr: SIGNAL IS "XIL_INTERFACENAME TrigD_intr, SENSITIVITY LEVEL_HIGH, PortWidth 1";
  ATTRIBUTE X_INTERFACE_INFO OF TrigD_intr: SIGNAL IS "xilinx.com:signal:interrupt:1.0 TrigD_intr INTERRUPT";
  ATTRIBUTE X_INTERFACE_PARAMETER OF TrigC_intr: SIGNAL IS "XIL_INTERFACENAME TrigC_intr, SENSITIVITY LEVEL_HIGH, PortWidth 1";
  ATTRIBUTE X_INTERFACE_INFO OF TrigC_intr: SIGNAL IS "xilinx.com:signal:interrupt:1.0 TrigC_intr INTERRUPT";
  ATTRIBUTE X_INTERFACE_PARAMETER OF TrigB_intr: SIGNAL IS "XIL_INTERFACENAME TrigB_intr, SENSITIVITY LEVEL_HIGH, PortWidth 1";
  ATTRIBUTE X_INTERFACE_INFO OF TrigB_intr: SIGNAL IS "xilinx.com:signal:interrupt:1.0 TrigB_intr INTERRUPT";
  ATTRIBUTE X_INTERFACE_PARAMETER OF TrigA_intr: SIGNAL IS "XIL_INTERFACENAME TrigA_intr, SENSITIVITY LEVEL_HIGH, PortWidth 1";
  ATTRIBUTE X_INTERFACE_INFO OF TrigA_intr: SIGNAL IS "xilinx.com:signal:interrupt:1.0 TrigA_intr INTERRUPT";
  ATTRIBUTE X_INTERFACE_PARAMETER OF SS_RESET: SIGNAL IS "XIL_INTERFACENAME SS_RESET, POLARITY ACTIVE_LOW";
  ATTRIBUTE X_INTERFACE_INFO OF SS_RESET: SIGNAL IS "xilinx.com:signal:reset:1.0 SS_RESET RST";
  ATTRIBUTE X_INTERFACE_PARAMETER OF RDAD_CLK: SIGNAL IS "XIL_INTERFACENAME RDAD_CLK, FREQ_HZ 100000000, PHASE 0.000, CLK_DOMAIN base_zynq_TARGETC_IP_Prototype_0_1_RDAD_CLK";
  ATTRIBUTE X_INTERFACE_INFO OF RDAD_CLK: SIGNAL IS "xilinx.com:signal:clock:1.0 RDAD_CLK CLK";
  ATTRIBUTE X_INTERFACE_PARAMETER OF WL_CLK_N: SIGNAL IS "XIL_INTERFACENAME WL_CLK_N, FREQ_HZ 100000000, PHASE 0.000, CLK_DOMAIN base_zynq_TARGETC_IP_Prototype_0_1_WL_CLK_N";
  ATTRIBUTE X_INTERFACE_INFO OF WL_CLK_N: SIGNAL IS "xilinx.com:signal:clock:1.0 WL_CLK_N CLK";
  ATTRIBUTE X_INTERFACE_PARAMETER OF WL_CLK_P: SIGNAL IS "XIL_INTERFACENAME WL_CLK_P, FREQ_HZ 100000000, PHASE 0.000, CLK_DOMAIN base_zynq_TARGETC_IP_Prototype_0_1_WL_CLK_P";
  ATTRIBUTE X_INTERFACE_INFO OF WL_CLK_P: SIGNAL IS "xilinx.com:signal:clock:1.0 WL_CLK_P CLK";
  ATTRIBUTE X_INTERFACE_PARAMETER OF GCC_RESET: SIGNAL IS "XIL_INTERFACENAME GCC_RESET, POLARITY ACTIVE_LOW";
  ATTRIBUTE X_INTERFACE_INFO OF GCC_RESET: SIGNAL IS "xilinx.com:signal:reset:1.0 GCC_RESET RST";
  ATTRIBUTE X_INTERFACE_INFO OF tc_axi_rready: SIGNAL IS "xilinx.com:interface:aximm:1.0 tc_axi RREADY";
  ATTRIBUTE X_INTERFACE_INFO OF tc_axi_rvalid: SIGNAL IS "xilinx.com:interface:aximm:1.0 tc_axi RVALID";
  ATTRIBUTE X_INTERFACE_INFO OF tc_axi_rresp: SIGNAL IS "xilinx.com:interface:aximm:1.0 tc_axi RRESP";
  ATTRIBUTE X_INTERFACE_INFO OF tc_axi_rdata: SIGNAL IS "xilinx.com:interface:aximm:1.0 tc_axi RDATA";
  ATTRIBUTE X_INTERFACE_INFO OF tc_axi_arready: SIGNAL IS "xilinx.com:interface:aximm:1.0 tc_axi ARREADY";
  ATTRIBUTE X_INTERFACE_INFO OF tc_axi_arvalid: SIGNAL IS "xilinx.com:interface:aximm:1.0 tc_axi ARVALID";
  ATTRIBUTE X_INTERFACE_INFO OF tc_axi_arprot: SIGNAL IS "xilinx.com:interface:aximm:1.0 tc_axi ARPROT";
  ATTRIBUTE X_INTERFACE_INFO OF tc_axi_araddr: SIGNAL IS "xilinx.com:interface:aximm:1.0 tc_axi ARADDR";
  ATTRIBUTE X_INTERFACE_INFO OF tc_axi_bready: SIGNAL IS "xilinx.com:interface:aximm:1.0 tc_axi BREADY";
  ATTRIBUTE X_INTERFACE_INFO OF tc_axi_bvalid: SIGNAL IS "xilinx.com:interface:aximm:1.0 tc_axi BVALID";
  ATTRIBUTE X_INTERFACE_INFO OF tc_axi_bresp: SIGNAL IS "xilinx.com:interface:aximm:1.0 tc_axi BRESP";
  ATTRIBUTE X_INTERFACE_INFO OF tc_axi_wready: SIGNAL IS "xilinx.com:interface:aximm:1.0 tc_axi WREADY";
  ATTRIBUTE X_INTERFACE_INFO OF tc_axi_wvalid: SIGNAL IS "xilinx.com:interface:aximm:1.0 tc_axi WVALID";
  ATTRIBUTE X_INTERFACE_INFO OF tc_axi_wstrb: SIGNAL IS "xilinx.com:interface:aximm:1.0 tc_axi WSTRB";
  ATTRIBUTE X_INTERFACE_INFO OF tc_axi_wdata: SIGNAL IS "xilinx.com:interface:aximm:1.0 tc_axi WDATA";
  ATTRIBUTE X_INTERFACE_INFO OF tc_axi_awready: SIGNAL IS "xilinx.com:interface:aximm:1.0 tc_axi AWREADY";
  ATTRIBUTE X_INTERFACE_INFO OF tc_axi_awvalid: SIGNAL IS "xilinx.com:interface:aximm:1.0 tc_axi AWVALID";
  ATTRIBUTE X_INTERFACE_INFO OF tc_axi_awprot: SIGNAL IS "xilinx.com:interface:aximm:1.0 tc_axi AWPROT";
  ATTRIBUTE X_INTERFACE_PARAMETER OF tc_axi_awaddr: SIGNAL IS "XIL_INTERFACENAME tc_axi, DATA_WIDTH 32, PROTOCOL AXI4LITE, FREQ_HZ 100000000, ID_WIDTH 0, ADDR_WIDTH 32, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 0, HAS_LOCK 0, HAS_PROT 1, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 1, HAS_BRESP 1, HAS_RRESP 1, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 1, NUM_WRITE_OUTSTANDING 1, MAX_BURST_LENGTH 1, PHASE 0.000, CLK_DOMAIN base_zynq_processing_system7_0_0_FCLK_CLK0, NUM_READ_THRE" & 
"ADS 1, NUM_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0";
  ATTRIBUTE X_INTERFACE_INFO OF tc_axi_awaddr: SIGNAL IS "xilinx.com:interface:aximm:1.0 tc_axi AWADDR";
  ATTRIBUTE X_INTERFACE_PARAMETER OF tc_axi_aresetn: SIGNAL IS "XIL_INTERFACENAME tc_axi_aresetn, POLARITY ACTIVE_LOW";
  ATTRIBUTE X_INTERFACE_INFO OF tc_axi_aresetn: SIGNAL IS "xilinx.com:signal:reset:1.0 tc_axi_aresetn RST";
  ATTRIBUTE X_INTERFACE_PARAMETER OF tc_axi_aclk: SIGNAL IS "XIL_INTERFACENAME tc_axi_aclk, ASSOCIATED_BUSIF tc_axi, ASSOCIATED_RESET tc_axi_aresetn, FREQ_HZ 100000000, PHASE 0.000, CLK_DOMAIN base_zynq_processing_system7_0_0_FCLK_CLK0";
  ATTRIBUTE X_INTERFACE_INFO OF tc_axi_aclk: SIGNAL IS "xilinx.com:signal:clock:1.0 tc_axi_aclk CLK";
BEGIN
  U0 : TARGETC_IP_Prototype
    PORT MAP (
      RefCLK_i1 => RefCLK_i1,
      RefCLK_i2 => RefCLK_i2,
      tc_axi_aclk => tc_axi_aclk,
      tc_axi_aresetn => tc_axi_aresetn,
      tc_axi_awaddr => tc_axi_awaddr,
      tc_axi_awprot => tc_axi_awprot,
      tc_axi_awvalid => tc_axi_awvalid,
      tc_axi_awready => tc_axi_awready,
      tc_axi_wdata => tc_axi_wdata,
      tc_axi_wstrb => tc_axi_wstrb,
      tc_axi_wvalid => tc_axi_wvalid,
      tc_axi_wready => tc_axi_wready,
      tc_axi_bresp => tc_axi_bresp,
      tc_axi_bvalid => tc_axi_bvalid,
      tc_axi_bready => tc_axi_bready,
      tc_axi_araddr => tc_axi_araddr,
      tc_axi_arprot => tc_axi_arprot,
      tc_axi_arvalid => tc_axi_arvalid,
      tc_axi_arready => tc_axi_arready,
      tc_axi_rdata => tc_axi_rdata,
      tc_axi_rresp => tc_axi_rresp,
      tc_axi_rvalid => tc_axi_rvalid,
      tc_axi_rready => tc_axi_rready,
      SIN => SIN,
      SCLK => SCLK,
      PCLK => PCLK,
      SHOUT => SHOUT,
      HSCLK_P => HSCLK_P,
      HSCLK_N => HSCLK_N,
      WR_RS_S0 => WR_RS_S0,
      WR_RS_S1 => WR_RS_S1,
      WR_CS_S0 => WR_CS_S0,
      WR_CS_S1 => WR_CS_S1,
      WR_CS_S2 => WR_CS_S2,
      WR_CS_S3 => WR_CS_S3,
      WR_CS_S4 => WR_CS_S4,
      WR_CS_S5 => WR_CS_S5,
      GCC_RESET => GCC_RESET,
      WL_CLK_P => WL_CLK_P,
      WL_CLK_N => WL_CLK_N,
      RDAD_CLK => RDAD_CLK,
      RDAD_SIN => RDAD_SIN,
      RDAD_DIR => RDAD_DIR,
      SAMPLESEL_ANY => SAMPLESEL_ANY,
      DO => DO,
      SS_INCR => SS_INCR,
      DOE => DOE,
      DONE => DONE,
      SS_RESET => SS_RESET,
      REGCLR => REGCLR,
      SS_LD_SIN => SS_LD_SIN,
      SS_LD_DIR => SS_LD_DIR,
      RAMP => RAMP,
      SSTIN_P => SSTIN_P,
      SSTIN_N => SSTIN_N,
      MONTIMING_P => MONTIMING_P,
      MONTIMING_N => MONTIMING_N,
      WDOTime => WDOTime,
      DIGTime => DIGTime,
      Trigger => Trigger,
      WDONbr => WDONbr,
      TestStream => TestStream,
      PSBusy => PSBusy,
      FIFOresponse => FIFOresponse,
      CH0 => CH0,
      CH1 => CH1,
      CH2 => CH2,
      CH3 => CH3,
      CH4 => CH4,
      CH5 => CH5,
      CH6 => CH6,
      CH7 => CH7,
      CH8 => CH8,
      CH9 => CH9,
      CH10 => CH10,
      CH11 => CH11,
      CH12 => CH12,
      CH13 => CH13,
      CH14 => CH14,
      CH15 => CH15,
      SSvalid => SSvalid,
      TrigA => TrigA,
      TrigB => TrigB,
      TrigC => TrigC,
      TrigD => TrigD,
      TrigA_intr => TrigA_intr,
      TrigB_intr => TrigB_intr,
      TrigC_intr => TrigC_intr,
      TrigD_intr => TrigD_intr,
      SSVALID_INTR => SSVALID_INTR,
      SSTIN => SSTIN,
      MONTIMING => MONTIMING,
      RAMP_CNT => RAMP_CNT
    );
END base_zynq_TARGETC_IP_Prototype_0_1_arch;