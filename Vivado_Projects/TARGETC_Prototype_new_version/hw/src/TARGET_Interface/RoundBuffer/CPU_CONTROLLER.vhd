-- -----------------------------------------------------------
--!@FILE: 	CPU_CONTROLLER.vhd
--!@AUTHOR: Jonathan Hendriks
--!@DATE: 	21st of January 2019
-- -----------------------------------------------------------
--!@DESCRIPTION:
--! CPU_CONTROLLER is the supervision over the round buffer. It
--! looks after two modes :
--! - Normal Mode, which is the user can ask for FSTWINDOW and
--!   NBRWINDOW both parameter set by the PS side thourgh AXI-Lite.
--! - Trigger Mode, which is used to catch any trigger on the Raw
--! trigger inputs.
-- -----------------------------------------------------------

-- Libraries
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
use work.WindowCPU_PKG.all;
use work.TARGETC_pkg.all;

--Entity
entity CPU_CONTROLLER is
	Generic(
		-- NBR of windows menaing the number of CPUs
		NBRWINDOWS : integer := 16
	);
	Port (
	nrst : 			in	STD_Logic;

	--Time Input
	ClockBus:		in T_ClockBus;
	Timecounter:	in std_logic_vector(63 downto 0);

	-- Raw trigger from external board
	trigger : 		in std_logic_vector(3 downto 0);

	-- Interface to WindowCPU and WindowStore
	CPUBus:			out	std_logic_vector(10 downto 0);
	CPUTime:		out	std_logic_vector(63 downto 0);
	TriggerInfo :	out std_logic_vector(11 downto 0);

	-- Storage pins of TargetC
	WR_RS_S:		out std_logic_vector(1 downto 0);
	WR_CS_S:		out std_logic_vector(5 downto 0);

	-- Control Signals
	CtrlBus_IxSL:	in 	T_CtrlBus_IxSL;

	-- Address of current window and the old one
	ValidData:		out std_logic;
	CurAddr:		out	std_logic_vector(7 downto 0);
	CurAddrBit:		out std_logic_vector(NBRWINDOWS-1 downto 0);
	OldAddr:		out	std_logic_vector(7 downto 0);
	OldAddrBit:		out std_logic_vector(NBRWINDOWS-1 downto 0);

	-- Next and Prev address from Hamming code
	NextAddr_in : 	in std_logic_vector(7 downto 0);
	PrevAddr_in :	in std_logic_vector(7 downto 0);
	NextValid_in : 	in std_logic;
	PrevValid_in :	in std_logic;

    -- FIFO IN for Digiting
    DIG_Full	: out	std_logic;
    DIG_DataIn	: in	std_logic_vector(8 downto 0);
    DIG_WriteEn	: in	std_logic
	);

end CPU_CONTROLLER;

--Architecture
architecture Behavioral of CPU_CONTROLLER is

	component BlockDelay is
		generic(
			NBR : integer := 16
		);
		Port (
		nrst : 		in	STD_Logic;
		clk:		in std_logic;

		Scnt:		in	std_logic_vector(3 downto 0);

		D : 		in	std_logic;
		Q:			out	std_logic

		);

	end component BlockDelay;

	component TRIGGER_CONTROLLER is
		Generic(
			LONG_TRIGGER: integer := 32
		);
		Port (
		nrst : 			in	STD_Logic;
		clk:			in	std_Logic;

		trigger : 		in std_logic_vector(3 downto 0);

		-- Interface to WindowCPU
		TriggerInfo :		out std_logic_vector(11 downto 0)
		);
	end component;

	component aFifo is
    generic (
        DATA_WIDTH :integer := 8;
        ADDR_WIDTH :integer := 4
    );
    port (
    	rst :		in std_logic;
        -- Reading port.
        Data_out    :out std_logic_vector (DATA_WIDTH-1 downto 0);
        Empty_out   :out std_logic;
        ReadEn_in   :in  std_logic;
        RClk        :in  std_logic;
        -- Writing port.
        Data_in     :in  std_logic_vector (DATA_WIDTH-1 downto 0);
        Full_out    :out std_logic;
        WriteEn_in  :in  std_logic;
        WClk        :in  std_logic
    );
	end component aFifo;

	component LookupTable_LE is
		generic(
			MIN_LE_TIME : integer := 3	-- All times are x4 ns so 2 is equal to 8ns = 12 samples before rise of trigger
		);
		Port (
			Clk:		in 	std_logic;
			SCnt:		in 	std_logic_vector(3 downto 0);
			prevWdo :	out	std_logic
		);
	end component LookupTable_LE;

	-- component LookupTable_TE is
	-- 	generic(
	-- 		MIN_TE_TIME : integer := 1
	-- 	);
	-- 	Port (
	-- 		Clk:		in 	std_logic;
	-- 		SCnt:		in 	std_logic_vector(3 downto 0);
	-- 		nextWdo :	out	std_logic
	-- 	);
	-- end component LookupTable_TE;

	type storagestate is (
		IDLE,
		READY,
		RESPREADY,

		EVALUATE,
		MARK_WINDOW
	);
	signal storage_stm : storagestate := IDLE;

	type digstoragestate is (
		IDLE,
		READING,
		PREPARING,
		WRITING
	);
	signal digsto_stm : digstoragestate;

	signal WR_ADDR_s : std_logic_vector(7 downto 0);
	--signal busy_intl : std_logic;

	signal NextAddr_intl : std_logic_vector(7 downto 0);
	signal PrevAddr_intl : std_logic_vector(7 downto 0);
	signal OldAddr_intl : std_logic_vector(7 downto 0);
	--signal prev_OldAddr_intl : std_logic_vector(7 downto 0);
	signal Trig_OldAddr_intl : std_logic_vector(7 downto 0);
	signal Ctrl_OldAddr_intl : std_logic_vector(7 downto 0);

--	signal cmdbus_intl : std_logic_vector(2 downto 0);
--	signal cmdwdo_en_intl: std_logic_vector(NBRWINDOWS-1 downto 0);

	signal FstWindow512 : std_logic_vector(8 downto 0);
	signal CntWindow512 : std_logic_vector(8 downto 0);

	signal DIG_Empty_intl, STO_ReadEn : std_logic;
	signal DIG_DataOut_intl : std_logic_vector(8 downto 0);

	-- Optimization for LUT reduction
	signal Cmp_s : std_logic := '0';

	signal CTRL_CPUBUS : std_logic_vector(10 downto 0);
	signal TRIG_CPUBUS : std_logic_vector(10 downto 0);
	signal DIGI_CPUBUS : std_logic_vector(10 downto 0);

	signal D_wr1_en, Wr1_en_dly : std_logic;
	signal D_wr2_en, Wr2_en_dly : std_logic;
	signal Old_Wr_en : std_logic_vector(1 downto 0);
	signal prevTrigger : std_logic;

	signal TrigInfo_intl, TrigInfo_intl_dly, Old_TrigInfo : std_logic_vector(11 downto 0);
	signal Last_intl, Trig_intl : std_logic;
	signal TriggerRegDly : std_logic_vector(1 downto 0);

	signal valid, valid_1dly : std_logic;
	signal LE_intl, TE_intl : std_logic;
	signal prev_Timecounter:	std_logic_vector(63 downto 0);

	signal Ctrl_Busy_s : std_logic;
	-- -------------------------------------------------------------
	-- Constraints on Signals
	-- -------------------------------------------------------------
	attribute DONT_TOUCH : string;

	attribute DONT_TOUCH of Trig_OldAddr_intl : signal is "TRUE";
	attribute DONT_TOUCH of Ctrl_OldAddr_intl : signal is "TRUE";
	--attribute DONT_TOUCH of OldAddr_intl : signal is "TRUE";
	--attribute DONT_TOUCH of CntWindow512 : signal is "TRUE";

	--attribute DONT_TOUCH of CTRL_CPUBUS : signal is "TRUE";

begin
	-- Lookup Table for the Leading Edge Signal to know if the previous window (OldAddr) must be taken into account
	LE_LUT_inst : 	LookupTable_LE
	generic map(
		MIN_LE_TIME => 3--MIN_LE_TIME
	)
	port map(
		clk	=> ClockBus.CLK250MHz,
		SCnt	=>  Timecounter(3 downto 0),
		prevWdo => LE_intl
	);

	-- TE_LUT_inst : 	LookupTable_TE
	-- generic map(
	-- 	MIN_TE_TIME => 2--MIN_TE_TIME
	-- )
	-- port map(
	-- 	clk	=> ClockBus.CLK250MHz,
	-- 	SCnt	=>  Timecounter(3 downto 0),
	-- 	nextWdo => TE_intl
	-- );

	-- Trigger controller analyze the raw trigger to give out last signals long signals etc from the 4 trigger inputs (TrigA, TrigB, TrigC and TrigD)
	TRIG_CONTROL_inst : TRIGGER_CONTROLLER
		Generic map(
			LONG_TRIGGER => 32
		)
		Port map(
		nrst 	=> nrst,
		clk		=> ClockBus.CLK250MHz,

		trigger => trigger,

		TriggerInfo => TrigInfo_intl
		);

	TriggerInfo <=	(others => '0')	when CtrlBus_IxSL.CPUMode = '0' else 	Old_TrigInfo;
	Trig_intl <= TrigInfo_intl(0) or TrigInfo_intl(1) or TrigInfo_intl(2) or TrigInfo_intl(3);

	-- Store and delay blocks for storage of trigger information
	GEN_DLY_TRIG : for I in 0 to 11 generate
		Dly_Trig : BlockDelay
			generic map(
				NBR => 16
			)
			port map(
				nrst 	=> nrst,
				clk		=> ClockBus.Clk250Mhz,

				SCnt	=> timecounter(3 downto 0),
				D		=> TrigInfo_intl(I),
				Q		=> TrigInfo_intl_dly(I)
			);
	end generate;

	-- Store and delay blocks for write 1 used in trigger mode
	Dly_WR1 : BlockDelay
		generic map(
			NBR => 16
		)
		port map(
			nrst 	=> nrst,
			clk		=> ClockBus.Clk250Mhz,

			SCnt	=> timecounter(3 downto 0),
			D		=> D_wr1_en,
			Q		=> wr1_en_dly
		);

	-- Store and delay blocks for write 2 used in trigger mode
	Dly_WR2 : BlockDelay
		generic map(
			NBR => 16
		)
		port map(
			nrst 	=> nrst,
			clk		=> ClockBus.Clk250Mhz,

			SCnt	=> timecounter(3 downto 0),
			D		=> D_wr2_en,
			Q		=> wr2_en_dly
		);

	-- Process for the write signals
	process(ClockBus.CLK250MHz,nrst)	-- Every 64 ns
	begin
		if nRST = '0' then
			prevTrigger <= '0';

			D_wr1_en <= '0';
			D_wr2_en <= '0';
		else
			if rising_edge(ClockBus.CLK250MHz) then
				prevTrigger <= Trig_intl;

				if prevTrigger = '1' or Trig_intl = '1' then
					if timecounter(3) = '1' then
						D_wr1_en <= '1';
					else
						D_wr2_en <= '1';
					end if;
				else
					D_wr1_en <= '0';
					D_wr2_en <= '0';
				end if;

			end if;
		end if;
	end process;

	-- Process of address change on rising_edge of SSTIN
	process(ClockBus.CLK250MHz,nrst)	-- Every 64 ns
	begin
		if nRST = '0' then
			valid <= '0';

			WR_ADDR_S <= (others => '0');

			-- Init the CPUs
			CurAddrBit <= (0 => '1', others => '0');

			OldAddr_intl <= x"FF";
			OldAddrBit <= (255 => '1', others => '0');

			Old_Wr_en <= (others => '1');
			Old_TrigInfo <= (others => '0');
			CPUTime <= (others=>'0');
		else
			if rising_edge(ClockBus.CLK250MHz) then

				valid_1dly <= not(valid);

				case timecounter(3 downto 0) is
					when "1111" => --Time 0
						valid <= '0';

						WR_ADDR_S 		<= NextAddr_intl;
						OldAddr_intl 	<= WR_ADDR_S;

						OldAddrBit <= (others => '0');
						OldAddrBit(to_integer(unsigned(WR_ADDR_S))) <= '1';
						--OldAddrBit <= (oldidx => '1', others => '0');

						CurAddrBit <= (others => '0');
						CurAddrBit(to_integer(unsigned(NextAddr_intl))) <= '1';
						--CurAddrBit <= (curidx => '1', others => '0');
						prev_Timecounter <= TimeCounter;
						CPUTime <= prev_Timecounter;

						Old_TrigInfo <= TrigInfo_intl_dly;
						Old_Wr_en	<= not(wr2_en_dly) & not(wr1_en_dly);

					when "0000" => --Time 1
						valid <= '1'; -- After this the data is correct, time to stabilize

					when others =>
				end case;

				if TriggerRegDly = "01" then
					Old_TrigInfo <= TrigInfo_intl_dly;
				end if;
			end if;
		end if;
	end process;

	-- Address update
	ValidData <= valid;
	CurAddr	<= WR_ADDR_S;
	OldAddr <= OldAddr_intl;

	--Update TARGET C pins
	WR_RS_S <= WR_ADDR_S(1 downto 0);
	WR_CS_S <= WR_ADDR_S(7 downto 2);



    -- Digitizing and Storage FIFO
	DIG_STO_AFIFO :  aFifo
    generic map(
        DATA_WIDTH => 9,
        ADDR_WIDTH => 4	--Maybe more ?
    )
    port map (
    	rst 	=> nrst,
        -- Reading port.
        Data_out    => DIG_DataOut_intl,
        Empty_out   => DIG_Empty_intl,
        ReadEn_in   => STO_ReadEn,
        RClk        => ClockBus.CLK250MHz,
        -- Writing port.
        Data_in     => DIG_DataIn,
        Full_out    => DIG_Full,
        WriteEn_in  => DIG_WriteEn,
        WClk        => ClockBus.WL_CLK
    );

	-- Process for Next and Prev Address
	process(ClockBus.CLK250MHz,nrst)
	begin
		if nrst = '0' then
			NextAddr_intl	<= x"01";
			PrevAddr_intl	<= x"FF";
		else
			if rising_edge(ClockBus.CLK250MHz) then
				if PrevValid_in = '1' then
					PrevAddr_intl <=	PrevAddr_in;
				end if;

				if NextValid_in = '1' then
					NextAddr_intl <=	NextAddr_in;
				end if;
			end if;
		end if;
	end process;

	-- Process for Trigger Information and Control
	process(ClockBus.CLK250MHz,nrst)
	begin
		if nrst='0' then
				TRIG_CPUBUS <= CMD_NOP & x"00";
				--Trig_OldAddr_intl <= OldAddr_intl;
				--Trig_intl_dly <= '0';
				TriggerRegDly <= (others => '0');
		else
			if rising_edge(ClockBus.CLK250MHz) then

				TriggerRegDly <= TriggerRegDly(0) & Trig_intl;
				--Trig_intl_dly <= Trig_intl;
				-- if valid = '1' then
				--
				-- 	Trig_OldAddr_intl <= OldAddr_intl;
				--
				-- 	if OldAddr_intl /= Trig_OldAddr_intl then
				if valid_1dly = '1' then
						--Send the command
						if Old_TrigInfo /= "000000000000" then
							case Old_Wr_en is
								when "00" =>
									TRIG_CPUBUS <= CMD_BOTH_MARKED & OldAddr_intl;
								when "01" =>
									TRIG_CPUBUS <= CMD_WR2_MARKED & OldAddr_intl;
								when "10" =>
									TRIG_CPUBUS <= CMD_WR1_MARKED & OldAddr_intl;
								when "11" =>
									--TRIG_CPUBUS <= CMD_NOP & OldAddr_intl;
									TRIG_CPUBUS <= DIGI_CPUBUS;
								when others =>
							end case;
						else
							--TRIG_CPUBUS <= CMD_NOP & x"00";
							TRIG_CPUBUS <= DIGI_CPUBUS;
						end if;
				else
					-- 	--TRIG_CPUBUS <= CMD_NOP & x"00";
					--TRIG_CPUBUS <= DIGI_CPUBUS;
						if LE_intl = '1' and TriggerRegDly = "01" then
							TRIG_CPUBUS <= CMD_WR2_MARKED & OldAddr_intl;
						else
							TRIG_CPUBUS <= DIGI_CPUBUS;
						end if;
					--end if;
				end if;
			end if;
		end if;
	end process;


	-- Minimal State Machine For Windows select and liberate CPU
	process(ClockBus.CLK250MHz, nRST)	-- Every 64 ns
	begin
	if nRST = '0' then
			storage_stm <= IDLE;

			FstWindow512 <= (others => '0');
			CntWindow512 <= (others => '0');

		--	busy_intl <= '0';
			CTRL_CPUBUS <= CMD_NOP & x"00";

			--Ctrl_OldAddr_intl <= OldAddr_intl;

		else
			if rising_edge(ClockBus.CLK250MHz) then
				-- if valid = '1' then
				--
				-- 	Ctrl_OldAddr_intl <= OldAddr_intl;

					-- Normal Storage Case from PS
					case storage_stm is
						when IDLE =>
							CTRL_CPUBUS <= DIGI_CPUBUS;
							Ctrl_Busy_s <=  '0';
							storage_stm <= READY;
					--		busy_intl <= '0';
						when READY =>
							CTRL_CPUBUS <= DIGI_CPUBUS;
					--		busy_intl <= '0';
							if(CtrlBus_IxSL.WindowStorage = '1') then
								-- First Window && Counter is on 512 windows (9bits)
								FstWindow512 	<= CtrlBus_IxSL.FSTWINDOW(8 downto 0);
								CntWindow512	<= CtrlBus_IxSL.NBRWINDOW(8 downto 0);

								Ctrl_Busy_s <=  '1';

								storage_stm <= RESPREADY;
							else
								storage_stm <= READY;
							end if;

						when RESPREADY =>
							CTRL_CPUBUS <= DIGI_CPUBUS;
							if(CtrlBus_IxSL.WindowStorage = '0') then
								--storage_stm <= STORAGE;
					--			busy_intl <= '1';
								storage_stm <= EVALUATE;

							else
					--			busy_intl <= '0';
								storage_stm <= RESPREADY;
							end if;

						when EVALUATE =>

							-- Waiting to have the next window
							--if NextAddr_intl = FstWindow512(8 downto 1) then
							if OldAddr_intl = FstWindow512(8 downto 1) and (valid_1dly = '1') then
								storage_stm <= MARK_WINDOW;

								--Ctrl_OldAddr_intl <= OldAddr_intl;

								if FstWindow512(0) = '0' then

									--Optimization for LUT reduction
									--if CntWindow512 = "000000001" then
									if Cmp_s = '1' then
										-- CTRL_CPUBUS.cmd <= CMD_WR1_MARKED;
										-- CTRL_CPUBUS.addr <= OldAddr_intl;
										--CTRL_CPUBUS <= CMD_WR1_MARKED & OldAddr_intl;
										CTRL_CPUBUS(10 downto 8) <= CMD_WR1_MARKED;
										CntWindow512 <= std_logic_vector(unsigned(CntWindow512) - 1);
									else
										-- CTRL_CPUBUS.cmd <= CMD_BOTH_MARKED;
										-- CTRL_CPUBUS.addr <= OldAddr_intl;

										--CTRL_CPUBUS <= CMD_BOTH_MARKED & OldAddr_intl;
										CTRL_CPUBUS(10 downto 8) <= CMD_BOTH_MARKED;
										CntWindow512 <= std_logic_vector(unsigned(CntWindow512) - 2);
									end if;
								else

									-- --Optimization for LUT reduction
									-- CTRL_CPUBUS.cmd <= CMD_WR2_MARKED;
									-- CTRL_CPUBUS.addr <= OldAddr_intl;
									--CTRL_CPUBUS <= CMD_WR2_MARKED & OldAddr_intl;
									CTRL_CPUBUS(10 downto 8) <= CMD_WR2_MARKED;
									CntWindow512 <= std_logic_vector(unsigned(CntWindow512) - 1);

								end if;

								CTRL_CPUBUS(7 downto 0) <= OldAddr_intl;

								storage_stm <= MARK_WINDOW;
							else
								CTRL_CPUBUS <= DIGI_CPUBUS;
								storage_stm <= EVALUATE;
							end if;

						when MARK_WINDOW =>
							--cmdwdo_en_intl <= (others => '0');

							if (to_integer(unsigned(CntWindow512)) /= 0) then

								--if OldAddr_intl /= Ctrl_OldAddr_intl then
								if (valid_1dly = '1') then
								--Ctrl_OldAddr_intl <= OldAddr_intl;

									if Cmp_s = '1' then
										-- CTRL_CPUBUS.cmd <= CMD_WR1_MARKED;
										-- CTRL_CPUBUS.addr <= OldAddr_intl;
										--CTRL_CPUBUS <= CMD_WR1_MARKED & OldAddr_intl;
										CTRL_CPUBUS(10 downto 8) <= CMD_WR1_MARKED;
										CntWindow512 <= std_logic_vector(unsigned(CntWindow512) - 1);
									else
										--CTRL_CPUBUS <= CMD_BOTH_MARKED & OldAddr_intl;
										CTRL_CPUBUS(10 downto 8) <= CMD_BOTH_MARKED;
										CntWindow512 <= std_logic_vector(unsigned(CntWindow512) - 2);
									end if;
									CTRL_CPUBUS(7 downto 0) <= OldAddr_intl;
								else
									--CTRL_CPUBUS <= CMD_NOP & x"00";
									CTRL_CPUBUS <= DIGI_CPUBUS;
								end if;
							else
								--CTRL_CPUBUS <= CMD_NOP & x"00";
								CTRL_CPUBUS <= DIGI_CPUBUS;
					--			busy_intl <= '0';
								storage_stm <= IDLE;
							end if;
						when others =>
					end case;
				--end if;
			end if;
		end if;
	end process;

	process(ClockBus.CLK250MHz, nRST)	-- Every 64 ns
	begin
		if nRST = '0' then
				digsto_stm <= IDLE;

				DIGI_CPUBUS <= CMD_NOP & x"00";
		else
			if rising_edge(ClockBus.CLK250MHz) then
			-- State Machine for Reading the windows digitized
				case digsto_stm is
					when IDLE =>
						DIGI_CPUBUS <= CMD_NOP & x"00";

						if DIG_Empty_intl = '0' then
							digsto_stm <= READING;
							STO_ReadEn <= '1';
						else
							digsto_stm <= IDLE;
							STO_ReadEn <= '0';
						end if;
					when READING =>

						digsto_stm <= PREPARING;
						STO_ReadEn <= '0';

					when PREPARING =>
						--TRIG_NOP_flg or CTRL_NOP_flg;


						if DIG_DataOut_intl(0) = '0' then
							DIGI_CPUBUS <= CMD_WR1_EN_DIS & DIG_DataOut_intl(8 downto 1);
						else
							DIGI_CPUBUS <= CMD_WR2_EN_DIS & DIG_DataOut_intl(8 downto 1);
						end if;
						digsto_stm <= WRITING;
					when  WRITING =>
						if ((DIGI_CPUBUS = TRIG_CPUBUS) and (CtrlBus_IxSL.CPUMode = '1')) or ((DIGI_CPUBUS = CTRL_CPUBUS) and (CtrlBus_IxSL.CPUMode = '0')) then
							digsto_stm <= IDLE;

							DIGI_CPUBUS <= CMD_NOP & x"00";
						else
							digsto_stm <= WRITING;
						end if;
					when others =>
						digsto_stm <= IDLE;
						STO_ReadEn <= '0';
				end case;
			end if;
		end if;
	end process;

--CPUMode
	-- CPUBus <=	TRIG_CPUBUS	when CtrlBus_IxSL.CPUMode = '1' else
	-- 			CTRL_CPUBUS;

	process(ClockBus.CLK250MHz, nRST)	-- Every 64 ns
	begin
	if nRST = '0' then

		CPUBus <= CMD_NOP & x"00";

		else
			if rising_edge(ClockBus.CLK250MHz) then
				if  CtrlBus_IxSL.CPUMode = '1' then
					CPUBus <= TRIG_CPUBUS;
				else
					CPUBus <= CTRL_CPUBUS;
				end if;
			end if;
		end if;
	end process;

	Cmp_s <= '1' when CntWindow512 = "000000001" else '0';

	--CtrlBus_OxSL.Ctrl_RB_Busy <= Ctrl_Busy_s;

end Behavioral;
