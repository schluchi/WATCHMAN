-------------------------------------------------------------------------------
-- Title      : 
-------------------------------------------------------------------------------
-- File       : ClockGenerator.vhd
-- Author     : Kurtis Nishimura  <kurtisn@slac.stanford.edu>
-- Company    : SLAC National Accelerator Laboratory
-- Created    : 2014-06-06
-- Last update: 2014-06-06
-- Platform   : Vivado 2014.1
-- Standard   : VHDL'93/02
-------------------------------------------------------------------------------
-- Description: Register control for the IRSX
-------------------------------------------------------------------------------
-- Copyright (c) 2014 SLAC National Accelerator Laboratory
-------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;
use work.StdRtlPkg.all;

library unisim;
use unisim.vcomponents.all;

entity ClockGenerator is
	generic (
		TPD_G             : time := 1 ns;
--		BASE_CLK_PERIOD_G : real :=  7.861 -- in ns (PGP)
		BASE_CLK_PERIOD_G : real := 47.164 -- in ns (SST)
	);
   port (
      -- PGP clock in (127.216 MHz)
      pgpClk        : in  sl;
      pgpRst        : in  sl;
      -- SST clock in (127.216 MHz / 6 = 21.2027 MHz)
      sstClk        : in  sl;
      sstRst        : in  sl;
--      -- AXI clock out      (127.216 MHz / 6 * 48 / 6 = 169.6213 MHz)
      -- AXI clock out      (127.216 MHz / 6 * 60 / 7.5 = 169.6213 MHz)
      axiClk        : out sl;
      axiClkB        : out sl;
      axiClkRst     : in  sl;
--      -- IRSX GCC clock     (127.216 MHz / 6 * 48 / 4 = 254.432 MHz)
      -- IRSX GCC clock     (127.216 MHz / 6 * 60 / 5 = 254.432 MHz)
      irsxGccClk    : out sl;
      irsxGccClkRst : out sl;
--      -- IRSX HS data clock (127.216 MHz / 6 * 48 / 6 = 169.6213 MHz)
      -- IRSX HS data clock (127.216 MHz / 6 * 48 / 6 = 169.6213 MHz) now identical to axiClk
      irsxHsClk     : out sl;
      irsxHsClkRst  : out sl;
--      -- System clock       (127.216 MHz / 6 * 48 / 5 = 203.5456 MHz)
      -- System clock       (127.216 MHz / 6 * 60 / 25 = 50.88639 MHz *16/4) 
      sysClk        : out sl;
      sysClkRst     : out sl;
      -- Synchronization for IRSX blocks
      irsxSync      : out sl;
      -- Fast clock for trigger serial out (21.2027 * 48 / 8/5)
      -- Fast clock for trigger serial out (21.2027 * 60 / 4)
      pgp5XClk      : out sl;
      pgpX5Rst 	    : out sl;-- for output module reset
      -- Slow clock for trigger serial out (21.2027 * 60 / 20) = 1/2 pgpClk
      pgp2DClk      : out sl;
      pgp2DRst 	    : out sl;-- for output module reset

      -- Output status of MMCM
      pllLocked     : out sl
   ); 
end ClockGenerator;

architecture rtl of ClockGenerator is

   signal iAxiClkRaw : sl;
   signal iAxiClkBRaw : sl;
   signal iAxiClk    : sl;
   signal iAxiClkB    : sl;
   signal iHsClkRaw  : sl;
   signal iHsClk     : sl;
   signal iGccClkRaw : sl;
   signal iGccClk    : sl;
   signal iSysClkRaw : sl;
   signal iSysClk    : sl;
   signal iSysClkRst : sl;
   signal clkFbOut   : sl;
   signal clkFbOut2   : sl;
   signal clkFbIn    : sl;
   signal clkFbIn2    : sl;
   signal locked     : sl;
   signal locked2     : sl;
   signal powerUpReset : sl;
   signal powerUpReset2 : sl;
   signal axiOrPgpRst  : sl;
   signal sysClkDiv4  : sl;
   signal ipgp5XClk  : sl;
   signal ipgp5XClkRaw  : sl;
   signal ipgp2DClk  : sl;
   signal ipgp2DClkRaw  : sl;
   
begin

   -- Outputs to top level
   irsxGccClk <= iGccClk;
--   irsxHsClk  <= iHsClk;
   irsxHsClk  <= iAxiClk; -- shared (same f)
   axiClk     <= iAxiClk;
   axiClkB     <= iAxiClkB;
   sysClk     <= iSysClk;
   sysClkRst  <= iSysClkRst;
   pgp5XClk<= ipgp5XClk;
   pgp2DClk<= ipgp2DClk;
--   pllLocked  <= locked; 
   pllLocked  <= locked and locked2; -- also locked2?

   -- SST sync signal is generated by rising edge of SST
   U_SstSyncEdge : entity work.SynchronizerEdge
      generic map (
         TPD_G          => 1 ns,
         RST_POLARITY_G => '1',
         OUT_POLARITY_G => '1',
         RST_ASYNC_G    => false,
--         BYPASS_SYNC_G  => false, 
         BYPASS_SYNC_G  => true, -- to test if this solves clock instability
         STAGES_G       => 3,
         INIT_G         => "0"
      )
      port map (
         clk         => iAxiClk,
         rst         => axiClkRst,
         dataIn      => sstClk,
         dataOut     => open,
         risingEdge  => irsxSync,
         fallingEdge => open
      );

--   -- Synchronize sstClkRst to other blocks
--   U_RstSyncSst : entity work.RstSync
--      generic map (
--         TPD_G           => TPD_G,
--         IN_POLARITY_G   => '1',
--         OUT_POLARITY_G  => '1',
--         BYPASS_SYNC_G   => false,
--         RELEASE_DELAY_G => 3
--      )
--      port map (
--         clk      => sstClk,
--         asyncRst => pgpRst, -- axiClkRst is a power on reset (or'ed with a usrRst which is tied to '0') that is clocked by AxiClk (which is derived from sstClk in the MMCM now, so watch out!)
--         syncRst  => iSstClkRst
--   );

	U_PowerUpResetMMCM : entity work.PwrUpRst
		generic map (
			--DURATION_G => 16962133 -- ~100 ms
			DURATION_G => 2120267 -- ~100 ms
		) port map (
--			arst   => pgpRst or sstRst, -- pllNotLocked counts up continuously at the sstClk rate
--			arst   => pgpRst, -- works
--			arst   => sstRst, -- b2l timeout error
			clk    => sstClk,
--			clk    => pgpClk,
			rstOut => powerUpReset
		);

   axiOrPgpRst <= axiClkRst or pgpRst;

   -- Synchronize irscGccClkRst to other blocks
   U_RstSyncGcc : entity work.RstSync
      generic map (
         TPD_G           => TPD_G,
         IN_POLARITY_G   => '1',
         OUT_POLARITY_G  => '1',
         BYPASS_SYNC_G   => false,
         RELEASE_DELAY_G => 3
      )
      port map (
         clk      => iGccClk,
         asyncRst => axiOrPgpRst,
         syncRst  => irsxGccClkRst
   );

   -- Synchronize axiClkRst to other blocks
   U_RstSyncSys : entity work.RstSync
      generic map (
         TPD_G           => TPD_G,
         IN_POLARITY_G   => '1',
         OUT_POLARITY_G  => '1',
         BYPASS_SYNC_G   => false,
         RELEASE_DELAY_G => 3
      )
      port map (
         clk      => iSysClk,
         asyncRst => axiOrPgpRst,
--         syncRst  => sysClkRst
         syncRst  => iSysClkRst
   );
   irsxHsClkRst <= axiClkRst;
   -- Synchronize axiClkRst to other blocks
   U_RstSyncPgp5x : entity work.RstSync
      generic map (
         TPD_G           => TPD_G,
         IN_POLARITY_G   => '1',
         OUT_POLARITY_G  => '1',
         BYPASS_SYNC_G   => false,
         RELEASE_DELAY_G => 3
      )
      port map (
         clk      => ipgp5XClk,
         asyncRst => axiOrPgpRst,
         syncRst  => pgpX5Rst
   );

   -- Synchronize pgp2DClk to other blocks
   U_RstSyncPgp2d : entity work.RstSync
      generic map (
         TPD_G           => TPD_G,
         IN_POLARITY_G   => '1',
         OUT_POLARITY_G  => '1',
         BYPASS_SYNC_G   => false,
         RELEASE_DELAY_G => 3
      )
      port map (
         clk      => ipgp2DClk,
         asyncRst => axiOrPgpRst,
         syncRst  => pgp2DRst
   );

   -- MMCM to generate base and fast clocks
   mmcm_adv_inst : MMCME2_ADV
   generic map(
      BANDWIDTH            => "LOW",
      CLKOUT4_CASCADE      => false,
      COMPENSATION         => "ZHOLD",
      STARTUP_WAIT         => false,
      DIVCLK_DIVIDE        => 1,
      --DIVCLK_DIVIDE        => 6,
      CLKFBOUT_MULT_F      => 60.0,
      CLKFBOUT_PHASE       => 0.000,
      CLKFBOUT_USE_FINE_PS => false,
      CLKOUT0_DIVIDE_F     => 7.5, -- 169
--      CLKOUT0_PHASE        => 0.000, --LM : original - see if 90 degrees solve problems with violations
      CLKOUT0_PHASE        => 90.000,
      CLKOUT0_DUTY_CYCLE   => 0.500,
      CLKOUT0_USE_FINE_PS  => false,
      CLKOUT1_DIVIDE       => 5, -- 254
      CLKOUT1_PHASE        => 0.000,
      CLKOUT1_DUTY_CYCLE   => 0.500,
      CLKOUT1_USE_FINE_PS  => false,
      CLKOUT2_DIVIDE       => 25, -- 51
      CLKOUT2_PHASE        => 0.000,
      CLKOUT2_DUTY_CYCLE   => 0.500,
      CLKOUT2_USE_FINE_PS  => false,
      CLKOUT3_DIVIDE       => 6,
      CLKOUT3_PHASE        => 0.000,
      CLKOUT3_DUTY_CYCLE   => 0.500,
      CLKOUT3_USE_FINE_PS  => false,
--      CLKOUT4_DIVIDE       => 2,  -- Axix5
      CLKOUT4_DIVIDE       => 4,  -- Axix2.5
      CLKOUT4_PHASE        => 0.000,
      CLKOUT4_DUTY_CYCLE   => 0.500,
      CLKOUT4_USE_FINE_PS  => false,
      CLKOUT5_DIVIDE       => 20,  -- Axix/2
      CLKOUT5_PHASE        => 0.000,
      CLKOUT5_DUTY_CYCLE   => 0.500,
      CLKOUT5_USE_FINE_PS  => false,
      CLKIN1_PERIOD        => BASE_CLK_PERIOD_G,
      REF_JITTER1          => 0.006)
   port map(
      -- Output clocks
      CLKFBOUT     => clkFbOut,
      CLKFBOUTB    => open,
      CLKOUT0      => iAxiClkRaw,
      CLKOUT0B     => iAxiClkBRaw,
      CLKOUT1      => iGccClkRaw,
      CLKOUT1B     => open,
      CLKOUT2      => sysClkDiv4,
      CLKOUT2B     => open,
      CLKOUT3      => open,
      CLKOUT3B     => open,
      CLKOUT4      => ipgp5XClkRaw,
      CLKOUT5      => ipgp2DClkRaw,
      CLKOUT6      => open,
      -- Input clock control
      CLKFBIN      => clkFbIn,
      CLKIN1       => sstClk,
      --CLKIN1       => pgpClk,
      CLKIN2       => '0',
      -- Tied to always select the primary input clock
      CLKINSEL     => '1',
      -- Ports for dynamic reconfiguration
      DADDR        => (others => '0'),
      DCLK         => '0',
      DEN          => '0',
      DI           => (others => '0'),
      DO           => open,
      DRDY         => open,
      DWE          => '0',
      -- Ports for dynamic phase shift
      PSCLK        => '0',
      PSEN         => '0',
      PSINCDEC     => '0',
      PSDONE       => open,
      -- Other control and status signals
      LOCKED       => locked,
      CLKINSTOPPED => open,
      CLKFBSTOPPED => open,
      PWRDWN       => '0',
      RST          => powerUpReset);

   BUFG_FB : BUFG
      port map (
         I => clkFbOut,
         O => clkFbIn);       
--   BUFG_FB2 : BUFG
--      port map (
--         I => clkFbOut2,
--         O => clkFbIn2);       
clkFbIn2<= clkFbOut2;  -- according to an error in connecting 2 MMCM - clkbfin should be connected directly to clkfbout
   BUFG_OUT0 : BUFG
      port map (
         I => iAxiClkRaw,
         O => iAxiClk); 
   BUFG_OUT0B : BUFG
      port map (
         I => iAxiClkBRaw,
         O => iAxiClkB); 
   BUFG_OUT1 : BUFG
      port map (
         I => iGccClkRaw,
         O => iGccClk);
   -- This clock is for IDELAYCTRL (203.5 MHz)
   BUFG_OUT2 : BUFG
      port map (
         I => iSysClkRaw,
         O => iSysClk);   
--   BUFG_OUT3 : BUFG
--      port map (
--         I => iHsClkRaw,
--         O => iHsClk);
   BUFG_OUT3 : BUFG
      port map (
         I => ipgp5XClkRaw,
         O => ipgp5XClk);

   BUFG_OUT4 : BUFG
      port map (
         I => ipgp2DClkRaw,
         O => ipgp2DClk);

   -- MMCM to generate 200MHz
   mmcm_adv_inst2 : MMCME2_ADV
   generic map(
      BANDWIDTH            => "LOW",
      CLKOUT4_CASCADE      => false,
      COMPENSATION         => "ZHOLD",
      STARTUP_WAIT         => false,
      DIVCLK_DIVIDE        => 1,
      --DIVCLK_DIVIDE        => 6,
      CLKFBOUT_MULT_F      => 16.0,
      CLKFBOUT_PHASE       => 0.000,
      CLKFBOUT_USE_FINE_PS => false,
      CLKOUT0_DIVIDE_F     => 4.0,
      CLKOUT0_PHASE        => 0.000,
      CLKOUT0_DUTY_CYCLE   => 0.500,
      CLKOUT0_USE_FINE_PS  => false,
      CLKOUT1_DIVIDE       => 4,
      CLKOUT1_PHASE        => 0.000,
      CLKOUT1_DUTY_CYCLE   => 0.500,
      CLKOUT1_USE_FINE_PS  => false,
      CLKOUT2_DIVIDE       => 5,
      CLKOUT2_PHASE        => 0.000,
      CLKOUT2_DUTY_CYCLE   => 0.500,
      CLKOUT2_USE_FINE_PS  => false,
      CLKOUT3_DIVIDE       => 6,
      CLKOUT3_PHASE        => 0.000,
      CLKOUT3_DUTY_CYCLE   => 0.500,
      CLKOUT3_USE_FINE_PS  => false,
      CLKOUT4_DIVIDE       => 6, 
      CLKOUT4_PHASE        => 0.000,
      CLKOUT4_DUTY_CYCLE   => 0.500,
      CLKOUT4_USE_FINE_PS  => false,
      CLKIN1_PERIOD        => BASE_CLK_PERIOD_G*25.0/60.0,
      REF_JITTER1          => 0.006)
   port map(
      -- Output clocks
      CLKFBOUT     => clkFbOut2,
      CLKFBOUTB    => open,
      CLKOUT0      => iSysClkRaw,
      CLKOUT0B     => open,
      CLKOUT1      => open,
      CLKOUT1B     => open,
      CLKOUT2      => open,
      CLKOUT2B     => open,
      CLKOUT3      => open,
      CLKOUT3B     => open,
      CLKOUT4      => open,
      CLKOUT5      => open,
      CLKOUT6      => open,
      -- Input clock control
      CLKFBIN      => clkFbIn2,
      CLKIN1       => sysClkDiv4,
      --CLKIN1       => pgpClk,
      CLKIN2       => '0',
      -- Tied to always select the primary input clock
      CLKINSEL     => '1',
      -- Ports for dynamic reconfiguration
      DADDR        => (others => '0'),
      DCLK         => '0',
      DEN          => '0',
      DI           => (others => '0'),
      DO           => open,
      DRDY         => open,
      DWE          => '0',
      -- Ports for dynamic phase shift
      PSCLK        => '0',
      PSEN         => '0',
      PSINCDEC     => '0',
      PSDONE       => open,
      -- Other control and status signals
      LOCKED       => locked2,
      CLKINSTOPPED => open,
      CLKFBSTOPPED => open,
      PWRDWN       => '0',
      RST          => powerUpReset2);

powerUpReset2<= powerUpReset; -- Use same reset for both PLLs

end rtl;
