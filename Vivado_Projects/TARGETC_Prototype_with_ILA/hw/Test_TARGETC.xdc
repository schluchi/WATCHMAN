
set_property DONT_TOUCH true [get_cells base_zynq_i/TARGETC_0/U0/TC_Control_inst/axi_rdata*]
set_property DONT_TOUCH true [get_cells base_zynq_i/TARGETC_0/U0/TC_SerialRegCtrl_inst/axi_rdata*]
set_property DONT_TOUCH true [get_cells base_zynq_i/FifoManager_0/U0/FSM_sequential_fifo_wr_stm*]



set_property ALLOW_COMBINATORIAL_LOOPS true [get_nets base_zynq_i/TARGETC_0/U0/TC_Control_inst/axi_rdata*]
set_property ALLOW_COMBINATORIAL_LOOPS true [get_nets base_zynq_i/TARGETC_0/U0/TC_Control_inst/data0*]
set_property ALLOW_COMBINATORIAL_LOOPS true [get_nets base_zynq_i/TARGETC_0/U0/TC_SerialRegCtrl_inst/axi_rdata*]
set_property ALLOW_COMBINATORIAL_LOOPS true [get_nets base_zynq_i/FifoManager_0/U0/FSM_sequential_fifo_wr_stm*]


set_property PACKAGE_PIN G20 [get_ports {PB1}];  			# "G20.JX2_LVDS_16_N.JX2.69.PB1"



set_property PULLUP true [get_ports SDA]
set_property PULLUP true [get_ports SCL]
# ----------------------------------------------------------------------------
# FMC Expansion Connector - Bank 34
# ----------------------------------------------------------------------------

set_property PACKAGE_PIN Y14 [get_ports BB3]
set_property PACKAGE_PIN U14 [get_ports BB4]
set_property PACKAGE_PIN U20 [get_ports {BB6}];     #{LA08_N}];  # "U20.JX1_LVDS_14_N.JX1.55.LA08_N"

set_property PACKAGE_PIN P19 [get_ports {HCMPB}];           #{LA00_CC_N}];  # "P19.JX1_LVDS_12_N.JX1.49.LA00_CC_N"
set_property PACKAGE_PIN N18 [get_ports {HCMPA}];           #{LA00_CC_P}];  # "N18.JX1_LVDS_12_P.JX1.47.LA00_CC_P"
set_property PACKAGE_PIN P20 [get_ports {HCMPC}];           #{LA01_CC_N}];  # "P20.JX1_LVDS_13_N.JX1.50.LA01_CC_N"
set_property PACKAGE_PIN N20 [get_ports {HCMPD}];           #{LA01_CC_P}];  # "N20.JX1_LVDS_13_P.JX1.48.LA01_CC_P"
set_property PULLUP true [get_ports {HCMPB}];
set_property PULLUP true [get_ports {HCMPA}];
set_property PULLUP true [get_ports {HCMPC}];
set_property PULLUP true [get_ports {HCMPD}];

set_property PACKAGE_PIN Y16 [get_ports MONTIMING_P]
set_property PACKAGE_PIN Y17 [get_ports MONTIMING_N]


set_property PACKAGE_PIN U17 [get_ports SS_LD_DIR]
set_property PACKAGE_PIN T16 [get_ports SS_LD_SIN]
set_property PACKAGE_PIN W15 [get_ports REGCLR]


set_property PACKAGE_PIN W14 [get_ports {TRIGA}];           #{LA04_P}];  # "W14.JX1_LVDS_7_P.JX1.30.LA04_P"
set_property PACKAGE_PIN U15 [get_ports {TRIGB}];   #{LA07_N}];  # "U15.JX1_LVDS_10_N.JX1.43.LA07_N"

set_property PACKAGE_PIN W20 [get_ports DO_5]


set_property PACKAGE_PIN Y19 [get_ports DO_8]
set_property PACKAGE_PIN Y18 [get_ports DO_7]
set_property PACKAGE_PIN W16 [get_ports DO_1]
set_property PACKAGE_PIN V16 [get_ports RAMP]
set_property PACKAGE_PIN R17 [get_ports SS_RESET]
set_property PACKAGE_PIN R16 [get_ports {TRIGC}];       #{LA12_P}];  # "R16.JX1_LVDS_18_P.JX1.67.LA12_P"
set_property PACKAGE_PIN R18 [get_ports DO_10]
set_property PACKAGE_PIN T17 [get_ports DO_6]
set_property PACKAGE_PIN V18 [get_ports DO_13]
set_property PACKAGE_PIN V17 [get_ports DO_12]
#set_property PACKAGE_PIN W19 [get_ports DOE]
set_property PACKAGE_PIN W18 [get_ports DO_2]
set_property PACKAGE_PIN P18 [get_ports DO_3]
set_property PACKAGE_PIN N17 [get_ports DONE]


# ----------------------------------------------------------------------------
# FMC Expansion Connector - Bank 35
# ----------------------------------------------------------------------------


set_property PACKAGE_PIN H17 [get_ports BB2]
set_property PACKAGE_PIN H16 [get_ports DO_11]
set_property PACKAGE_PIN H18 [get_ports RDAD_CLK]
set_property PACKAGE_PIN J18 [get_ports RDAD_SIN]
set_property PACKAGE_PIN D18 [get_ports DO_14]
set_property PACKAGE_PIN E17 [get_ports BB1]
set_property PACKAGE_PIN D20 [get_ports SS_INCR]
set_property PACKAGE_PIN D19 [get_ports DO_4]
set_property PACKAGE_PIN E19 [get_ports BB5]
set_property PACKAGE_PIN E18 [get_ports DO_15]
set_property PACKAGE_PIN F17 [get_ports DO_16]
set_property PACKAGE_PIN F16 [get_ports DO_9]
set_property PACKAGE_PIN L20 [get_ports WR_CS_S4]
set_property PACKAGE_PIN L19 [get_ports RDAD_DIR]
set_property PACKAGE_PIN M18 [get_ports {TRIGD}];           #{LA25_N}];  # "M18.JX2_LVDS_8_N.JX2.43.LA25_N"
set_property PACKAGE_PIN M17 [get_ports SAMPLESEL_ANY]
set_property PACKAGE_PIN J19 [get_ports WR_CS_S2]
set_property PACKAGE_PIN K19 [get_ports WR_CS_S3]
set_property PACKAGE_PIN G18 [get_ports WR_CS_S0]
set_property PACKAGE_PIN G17 [get_ports WR_CS_S1]
set_property PACKAGE_PIN F20 [get_ports GCC_RESET]
set_property PACKAGE_PIN F19 [get_ports WR_CS_S5]
set_property PACKAGE_PIN H20 [get_ports WR_RS_S0]
set_property PACKAGE_PIN J20 [get_ports WR_RS_S1]
set_property PACKAGE_PIN G15 [get_ports SDA]
set_property PACKAGE_PIN H15 [get_ports SCL]
set_property PACKAGE_PIN N16 [get_ports PCLK]
set_property PACKAGE_PIN N15 [get_ports SHOUT]
set_property PACKAGE_PIN L15 [get_ports SIN]
set_property PACKAGE_PIN L14 [get_ports SCLK]



# ----------------------------------------------------------------------------
# IOSTANDARD Constraints
#
# Note that these IOSTANDARD constraints are applied to all IOs currently
# assigned within an I/O bank.  If these IOSTANDARD constraints are
# evaluated prior to other PACKAGE_PIN constraints being applied, then
# the IOSTANDARD specified will likely not be applied properly to those
# pins.  Therefore, bank wide IOSTANDARD constraints should be placed
# within the XDC file in a location that is evaluated AFTER all
# PACKAGE_PIN constraints within the target bank have been evaluated.
#
# Un-comment one or more of the following IOSTANDARD constraints according to
# the bank pin assignments that are required within a design.
# ----------------------------------------------------------------------------

# Set the bank voltage for IO Bank 34 to 3.3V by default.
# set_property IOSTANDARD LVCMOS33 [get_ports -of_objects [get_iobanks 34]];

# Set the bank voltage for IO Bank 35 to 3.3V by default.
# set_property IOSTANDARD LVCMOS33 [get_ports -of_objects [get_iobanks 35]];

# Set the bank voltage for IO Bank 13 to 3.3V by default. (I/O bank available on Z7020 device only)
# set_property IOSTANDARD LVCMOS33 [get_ports -of_objects [get_iobanks 13]];

#IOSTANDARD
#BANK 34
set_property IOSTANDARD LVCMOS25 [get_ports {HCMPA}];       #{LA00_CC_N}];  # "P19.JX1_LVDS_12_N.JX1.49.LA00_CC_N"
set_property IOSTANDARD LVCMOS25 [get_ports {HCMPB}];       #{LA00_CC_P}];  # "N18.JX1_LVDS_12_P.JX1.47.LA00_CC_P"
set_property IOSTANDARD LVCMOS25 [get_ports {HCMPC}];       #{LA01_CC_N}];  # "P20.JX1_LVDS_13_N.JX1.50.LA01_CC_N"
set_property IOSTANDARD LVCMOS25 [get_ports {HCMPD}];       #{LA01_CC_P}];  # "N20.JX1_LVDS_13_P.JX1.48.LA01_CC_P"
set_property IOSTANDARD LVDS_25 [get_ports SSTIN_P]
set_property IOSTANDARD LVDS_25 [get_ports SSTIN_N]
set_property PACKAGE_PIN P14 [get_ports SSTIN_P]
set_property PACKAGE_PIN R14 [get_ports SSTIN_N]
set_property IOSTANDARD LVDS_25 [get_ports MONTIMING_P]
set_property IOSTANDARD LVDS_25 [get_ports MONTIMING_N]
set_property IOSTANDARD LVCMOS25 [get_ports {TRIGA}];           #{LA04_N}];  # "Y14.JX1_LVDS_7_N.JX1.32.LA04_N"
set_property IOSTANDARD LVCMOS25 [get_ports BB3]
set_property IOSTANDARD LVCMOS25 [get_ports SS_LD_SIN]
set_property IOSTANDARD LVCMOS25 [get_ports SS_LD_DIR]
set_property IOSTANDARD LVCMOS25 [get_ports REGCLR]
set_property IOSTANDARD LVCMOS25 [get_ports BB4]
set_property IOSTANDARD LVCMOS25 [get_ports {TRIGB}];           #{LA07_P}];  # "U14.JX1_LVDS_10_P.JX1.41.LA07_P"
set_property IOSTANDARD LVCMOS25 [get_ports {LDAC}];            #{LA08_N}];  # "U20.JX1_LVDS_14_N.JX1.55.LA08_N"
set_property IOSTANDARD LVCMOS25 [get_ports {BB6}];             #{LA08_P}];  # "T20.JX1_LVDS_14_P.JX1.53.LA08_P"
set_property IOSTANDARD LVCMOS25 [get_ports DO_5]
set_property IOSTANDARD LVCMOS25 [get_ports DO_7]
set_property IOSTANDARD LVCMOS25 [get_ports DO_8]
set_property IOSTANDARD LVCMOS25 [get_ports RAMP]
set_property IOSTANDARD LVCMOS25 [get_ports DO_1]
set_property IOSTANDARD LVCMOS25 [get_ports {TRIGC}];           #{LA12_N}];  # "R17.JX1_LVDS_18_N.JX1.69.LA12_N"
set_property IOSTANDARD LVCMOS25 [get_ports SS_RESET]
set_property IOSTANDARD LVCMOS25 [get_ports DO_6]
set_property IOSTANDARD LVCMOS25 [get_ports DO_10]
set_property IOSTANDARD LVCMOS25 [get_ports DO_12]
set_property IOSTANDARD LVCMOS25 [get_ports DO_13]
set_property IOSTANDARD LVCMOS25 [get_ports DO_2]
#set_property IOSTANDARD LVCMOS25 [get_ports DOE]
set_property IOSTANDARD LVCMOS25 [get_ports DONE]
set_property IOSTANDARD LVCMOS25 [get_ports DO_3]

#BANK 35
set_property IOSTANDARD LVCMOS25 [get_ports DO_11]
set_property IOSTANDARD LVCMOS25 [get_ports BB2]
set_property IOSTANDARD LVCMOS25 [get_ports RDAD_SIN]
set_property IOSTANDARD LVCMOS25 [get_ports RDAD_CLK]
set_property IOSTANDARD LVCMOS25 [get_ports BB1]
set_property IOSTANDARD LVCMOS25 [get_ports DO_14]
set_property IOSTANDARD LVCMOS25 [get_ports DO_4]
set_property IOSTANDARD LVCMOS25 [get_ports SS_INCR]
set_property IOSTANDARD LVCMOS25 [get_ports DO_15]
set_property IOSTANDARD LVCMOS25 [get_ports BB5]
set_property IOSTANDARD LVCMOS25 [get_ports DO_9]
set_property IOSTANDARD LVCMOS25 [get_ports DO_16]
set_property IOSTANDARD LVCMOS25 [get_ports RDAD_DIR]
set_property IOSTANDARD LVCMOS25 [get_ports WR_CS_S4]
set_property IOSTANDARD LVDS_25 [get_ports WL_CLK_P]
set_property IOSTANDARD LVDS_25 [get_ports WL_CLK_N]
set_property PACKAGE_PIN M19 [get_ports WL_CLK_P]
set_property PACKAGE_PIN M20 [get_ports WL_CLK_N]
set_property IOSTANDARD LVCMOS25 [get_ports SAMPLESEL_ANY]
set_property IOSTANDARD LVCMOS25 [get_ports {TRIGD}];           #{LA25_P}];  # "M17.JX2_LVDS_8_P.JX2.41.LA25_P"
set_property IOSTANDARD LVCMOS25 [get_ports WR_CS_S2]
set_property IOSTANDARD LVCMOS25 [get_ports WR_CS_S3]
set_property IOSTANDARD LVCMOS25 [get_ports WR_CS_S0]
set_property IOSTANDARD LVCMOS25 [get_ports WR_CS_S1]
set_property IOSTANDARD LVCMOS25 [get_ports WR_CS_S5]
set_property IOSTANDARD LVCMOS25 [get_ports GCC_RESET]
set_property IOSTANDARD LVCMOS25 [get_ports WR_RS_S1]
set_property IOSTANDARD LVCMOS25 [get_ports WR_RS_S0]
set_property IOSTANDARD LVDS_25 [get_ports HSCLK_P]
set_property IOSTANDARD LVDS_25 [get_ports HSCLK_N]
set_property PACKAGE_PIN K14 [get_ports HSCLK_P]
set_property PACKAGE_PIN J14 [get_ports HSCLK_N]
set_property IOSTANDARD LVCMOS25 [get_ports SCL]
set_property IOSTANDARD LVCMOS25 [get_ports SDA]
set_property IOSTANDARD LVCMOS25 [get_ports SHOUT]
set_property IOSTANDARD LVCMOS25 [get_ports PCLK]
set_property IOSTANDARD LVCMOS25 [get_ports SCLK]
set_property IOSTANDARD LVCMOS25 [get_ports SIN]

set_property MARK_DEBUG true [get_nets {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[3]}]
set_property MARK_DEBUG true [get_nets {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[9]}]
set_property MARK_DEBUG true [get_nets {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[11]}]
set_property MARK_DEBUG true [get_nets {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[15]}]
set_property MARK_DEBUG true [get_nets {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[19]}]
set_property MARK_DEBUG true [get_nets {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[23]}]
set_property MARK_DEBUG true [get_nets {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[27]}]
set_property MARK_DEBUG true [get_nets {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[31]}]
set_property MARK_DEBUG true [get_nets {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[0]}]
set_property MARK_DEBUG true [get_nets {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[4]}]
set_property MARK_DEBUG true [get_nets {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[7]}]
set_property MARK_DEBUG true [get_nets {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[12]}]
set_property MARK_DEBUG true [get_nets {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[16]}]
set_property MARK_DEBUG true [get_nets {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[20]}]
set_property MARK_DEBUG true [get_nets {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[24]}]
set_property MARK_DEBUG true [get_nets {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[28]}]
set_property MARK_DEBUG true [get_nets {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[1]}]
set_property MARK_DEBUG true [get_nets {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[5]}]
set_property MARK_DEBUG true [get_nets {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[8]}]
set_property MARK_DEBUG true [get_nets {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[13]}]
set_property MARK_DEBUG true [get_nets {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[17]}]
set_property MARK_DEBUG true [get_nets {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[21]}]
set_property MARK_DEBUG true [get_nets {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[25]}]
set_property MARK_DEBUG true [get_nets {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[29]}]
set_property MARK_DEBUG true [get_nets {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[2]}]
set_property MARK_DEBUG true [get_nets {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[6]}]
set_property MARK_DEBUG true [get_nets {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[10]}]
set_property MARK_DEBUG true [get_nets {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[14]}]
set_property MARK_DEBUG true [get_nets {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[18]}]
set_property MARK_DEBUG true [get_nets {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[22]}]
set_property MARK_DEBUG true [get_nets {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[26]}]
set_property MARK_DEBUG true [get_nets {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[30]}]
set_property MARK_DEBUG true [get_nets base_zynq_i/axi_dma_0/m_axi_s2mm_aclk]
set_property MARK_DEBUG true [get_nets base_zynq_i/axi_dma_0/m_axi_s2mm_bready]
set_property MARK_DEBUG true [get_nets base_zynq_i/axi_dma_0/m_axi_s2mm_bvalid]
set_property MARK_DEBUG true [get_nets base_zynq_i/axi_dma_0/m_axi_s2mm_wlast]
set_property MARK_DEBUG true [get_nets base_zynq_i/axi_dma_0/m_axi_s2mm_wready]
set_property MARK_DEBUG true [get_nets base_zynq_i/axi_dma_0/m_axi_s2mm_wvalid]
create_debug_core u_ila_0 ila
set_property ALL_PROBE_SAME_MU true [get_debug_cores u_ila_0]
set_property ALL_PROBE_SAME_MU_CNT 1 [get_debug_cores u_ila_0]
set_property C_ADV_TRIGGER false [get_debug_cores u_ila_0]
set_property C_DATA_DEPTH 1024 [get_debug_cores u_ila_0]
set_property C_EN_STRG_QUAL false [get_debug_cores u_ila_0]
set_property C_INPUT_PIPE_STAGES 0 [get_debug_cores u_ila_0]
set_property C_TRIGIN_EN false [get_debug_cores u_ila_0]
set_property C_TRIGOUT_EN false [get_debug_cores u_ila_0]
set_property port_width 1 [get_debug_ports u_ila_0/clk]
connect_debug_port u_ila_0/clk [get_nets [list base_zynq_i/processing_system7_0/inst/FCLK_CLK0]]
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe0]
set_property port_width 32 [get_debug_ports u_ila_0/probe0]
connect_debug_port u_ila_0/probe0 [get_nets [list {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[0]} {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[1]} {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[2]} {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[3]} {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[4]} {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[5]} {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[6]} {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[7]} {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[8]} {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[9]} {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[10]} {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[11]} {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[12]} {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[13]} {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[14]} {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[15]} {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[16]} {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[17]} {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[18]} {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[19]} {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[20]} {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[21]} {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[22]} {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[23]} {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[24]} {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[25]} {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[26]} {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[27]} {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[28]} {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[29]} {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[30]} {base_zynq_i/axi_dma_0/m_axi_s2mm_wdata[31]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe1]
set_property port_width 1 [get_debug_ports u_ila_0/probe1]
connect_debug_port u_ila_0/probe1 [get_nets [list base_zynq_i/axi_dma_0/m_axi_s2mm_aclk]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe2]
set_property port_width 1 [get_debug_ports u_ila_0/probe2]
connect_debug_port u_ila_0/probe2 [get_nets [list base_zynq_i/axi_dma_0/m_axi_s2mm_bready]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe3]
set_property port_width 1 [get_debug_ports u_ila_0/probe3]
connect_debug_port u_ila_0/probe3 [get_nets [list base_zynq_i/axi_dma_0/m_axi_s2mm_bvalid]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe4]
set_property port_width 1 [get_debug_ports u_ila_0/probe4]
connect_debug_port u_ila_0/probe4 [get_nets [list base_zynq_i/axi_dma_0/m_axi_s2mm_wlast]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe5]
set_property port_width 1 [get_debug_ports u_ila_0/probe5]
connect_debug_port u_ila_0/probe5 [get_nets [list base_zynq_i/axi_dma_0/m_axi_s2mm_wready]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe6]
set_property port_width 1 [get_debug_ports u_ila_0/probe6]
connect_debug_port u_ila_0/probe6 [get_nets [list base_zynq_i/axi_dma_0/m_axi_s2mm_wvalid]]
set_property C_CLK_INPUT_FREQ_HZ 300000000 [get_debug_cores dbg_hub]
set_property C_ENABLE_CLK_DIVIDER false [get_debug_cores dbg_hub]
set_property C_USER_SCAN_CHAIN 1 [get_debug_cores dbg_hub]
connect_debug_port dbg_hub/clk [get_nets u_ila_0_FCLK_CLK0]
