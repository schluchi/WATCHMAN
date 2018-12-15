
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
