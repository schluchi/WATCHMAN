
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;
--use work.TARGETC_pkg.all;
use work.WindowCPU_pkg.all;

entity WindowCPU is
	generic(
		ADDRESS : integer := 0
	);
	Port (

	nrst : 			in	std_Logic;
--	SSTIN:			in std_logic;
	CLK:			in 	std_logic;

	--Window Part
	cmd :			in 	std_logic_vector(2 downto 0);
	cmd_en:			in 	std_logic;

	TrigInfo_IN:	in t_triggerinformation;
	TrigInfo_OUT:	out std_logic_vector(11 downto 0);

	wr1_en :		out std_logic;
	wr2_en :		out	std_logic;
	valid_o	:		out std_logic;

	CurAddr:		in 	std_logic_vector(7 downto 0);
	PREVBus_IN :	in	std_logic_vector(7 downto 0);
	PREVBus_OUT :	out	std_logic_vector(7 downto 0);
	NEXTBus_IN :	in	std_logic_vector(7 downto 0);
	NEXTBus_OUT :	out	std_logic_vector(7 downto 0)
	);

end WindowCPU;

architecture Behavioral of WindowCPU is


	signal wr1_en_intl : std_logic;
	signal wr2_en_intl : std_logic;

	signal wr1_mark_flg : std_logic;
	signal wr2_mark_flg : std_logic;

	signal wr1_flg : std_logic;
	signal wr2_flg : std_logic;

	signal PREVBus_intl : std_logic_vector(7 downto 0);
	signal NEXTBus_intl : std_logic_vector(7 downto 0);

	type WindowCPU_state_type is (
		IDLE,
		VALID,
		RESPVALID
	);

	type WindowCPU_trigger_state_type is (
		TRIG_IDLE,
		TRIG_SET,
		TRIG_RST
	);

	type state_t is (
		IDLE,
		READY,
		RESPREADY
	);
	signal trigger_stm : state_t := IDLE;

	signal TrigInfo_IN_last: t_triggerinformation;
	signal wr1_trig_last:	std_logic;
	signal wr2_trig_last:	std_logic;
	signal TrigInfo_marked : std_logic_vector(11 downto 0);
begin

	-- General Process
	process(CLK, nRST)
	begin
		if nrst = '0' then
			wr1_en_intl <= '1';
			wr2_en_intl <= '1';
			wr1_flg <= '0';
			wr2_flg <= '0';

			wr1_mark_flg <= '0';
			wr2_mark_flg <= '0';

			trigger_stm <= IDLE;
		else
			if rising_edge(clk) then

				-- Received Command from Control
				if cmd_en = '1' then
					case cmd is
						when CMD_WR1_EN =>
							wr1_en_intl <= '0';
							wr2_en_intl <= '1';
						when CMD_WR2_EN =>
							wr1_en_intl <= '1';
							wr2_en_intl <= '0';
						when CMD_BOTH_EN =>
							wr1_en_intl <= '0';
							wr2_en_intl <= '0';
						when CMD_WR1_EN_DIS =>
							wr1_flg <= '1';
						when CMD_WR2_EN_DIS =>
							wr2_flg <= '1';
						when CMD_WR1_MARKED =>
							wr1_mark_flg <= '1';
							TrigInfo_marked <= TrigInfo_IN.long & TrigInfo_IN.last & TrigInfo_IN.trig;
						when CMD_WR2_MARKED =>
							wr2_mark_flg <= '1';
							TrigInfo_marked <= TrigInfo_IN.long & TrigInfo_IN.last & TrigInfo_IN.trig;
						when CMD_BOTH_MARKED =>
							wr1_mark_flg <= '1';
							wr2_mark_flg <= '1';
							TrigInfo_marked <= TrigInfo_IN.long & TrigInfo_IN.last & TrigInfo_IN.trig;
						when others =>
					end case;
				end if;

				-- Marked STM
				if CurAddr = NEXTBus_IN then
					if (wr1_mark_flg = '1') then
						wr1_en_intl <= '0';
						wr1_mark_flg <= '0';
					end if;

					if (wr2_mark_flg = '1') then
						wr2_en_intl <= '0';
						wr2_mark_flg <= '0';
					end if;

					TrigInfo_OUT <= TrigInfo_marked;
				end if;

				-- Re-Entry STM
				if CurAddr /= std_logic_vector(to_unsigned(ADDRESS,8)) and CurAddr /= PREVBus_In and CurAddr /= NextBus_In then
					if (wr1_flg = '1') then
						wr1_en_intl <= '1';
						wr1_flg <= '0';
					end if;

					if (wr2_flg = '1') then
						wr2_en_intl <= '1';
						wr2_flg <= '0';
					end if;
				end if;

			end if;
		end if;
	end process;

	valid_o <= 	'1'	when wr1_en_intl = '0' or wr2_en_intl = '0' else '0';

	wr1_en	<= '0' when wr1_en_intl = '0' else '1';
	wr2_en	<= '0' when wr2_en_intl = '0' else '1';

	PREVBus_intl <= std_logic_vector(to_unsigned(ADDRESS,PREVBus_intl'length)) when ((wr1_en_intl='1') and (wr2_en_intl='1')) else PREVBus_In;

	NEXTBus_intl <= std_logic_vector(to_unsigned(ADDRESS,NEXTBus_intl'length)) when ((wr1_en_intl='1') and (wr2_en_intl='1')) else NEXTBus_In;

	PREVBus_Out <= PREVBus_intl;
	NEXTBus_Out <= NEXTBus_intl;
end Behavioral;
