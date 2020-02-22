--------------------------------------------------------------------------------
-- Company:     University of Verona
-- Engineer:    Mirco De Marchi
-- 
-- Create Date: 11.12.2019 12:02:48
-- Design Name: Top Level
-- Module Name: tl - behavioral
-- Project Name: First Assignment
-- Target Devices: PYNQ
-- Tool Versions: 2019.2
-- Description: Top Level of VHDL and Verilog floating point Multiplier IEEE754
-- 
-- Dependencies: null
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
-- Package definition
package tl_pack is

    -- Size of inputs and output
    constant SIZE               : INTEGER := 32;
    constant SIZE_OF_MANTISSA   : INTEGER := 23;
    constant SIZE_OF_EXPONENT   : INTEGER := 8;

    -- States name
    constant ST_0       : INTEGER := 0;
    constant ST_1       : INTEGER := 1;
    constant ST_2       : INTEGER := 2;
    constant ST_3       : INTEGER := 3;
    constant ST_4       : INTEGER := 4;
    constant ST_5       : INTEGER := 5;
    subtype TYPE_STATE is INTEGER range ST_0 to ST_5;
end tl_pack;

use WORK.tl_pack.ALL;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_BIT.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

--------------------------------------------------------------------------------
-- Model tl
-- op1, op2: floating points operands serialized;
-- op[31]       -> sign
-- op[30:23]    -> exp
-- op[22:0]     -> mantissa
-- res: floating point result of multiplication serialized;
-- in_rdy, out_rdy: handshake protocol of size 2; 
entity tl is
    Port ( op1          : in  STD_LOGIC_VECTOR(SIZE-1 downto 0);
           op2          : in  STD_LOGIC_VECTOR(SIZE-1 downto 0);
           in_rdy       : in  STD_LOGIC_VECTOR(1 downto 0);
           res          : out STD_LOGIC_VECTOR(SIZE-1 downto 0);
           res_rdy      : out STD_LOGIC_VECTOR(1 downto 0);
           clk          : in  STD_LOGIC;
           rst          : in  STD_LOGIC );
end entity tl;

--------------------------------------------------------------------------------
-- Architecture behavioral of model multiplier754_vhdl
architecture behavioral of tl is
    signal tmp          : STD_LOGIC_VECTOR(SIZE-1 downto 0);
    
    signal STATE        : TYPE_STATE := ST_0;
    signal NEXT_STATE   : TYPE_STATE := ST_0;

    -- Module signals
    signal res_vhdl     : STD_LOGIC_VECTOR(SIZE-1 downto 0);
    signal res_vhdl_rdy : STD_LOGIC;

    signal res_verilog     : STD_LOGIC_VECTOR(SIZE-1 downto 0);
    signal res_verilog_rdy : STD_LOGIC;

    -- Components
    component multiplierIEEE754_verilog is
        Port ( op1          : in  STD_LOGIC_VECTOR(SIZE-1 downto 0);
               op2          : in  STD_LOGIC_VECTOR(SIZE-1 downto 0);
               in_rdy       : in  STD_LOGIC;
               res          : out STD_LOGIC_VECTOR(SIZE-1 downto 0);
               res_rdy      : out STD_LOGIC;
               clk          : in  STD_LOGIC;
               rst          : in  STD_LOGIC );
    end component multiplierIEEE754_verilog;

    component multiplierIEEE754_vhdl is
        Port ( op1          : in  STD_LOGIC_VECTOR(SIZE-1 downto 0);
               op2          : in  STD_LOGIC_VECTOR(SIZE-1 downto 0);
               in_rdy       : in  STD_LOGIC;
               res          : out STD_LOGIC_VECTOR(SIZE-1 downto 0);
               res_rdy      : out STD_LOGIC;
               clk          : in  STD_LOGIC;
               rst          : in  STD_LOGIC );
    end component multiplierIEEE754_vhdl;
begin

    multiplier_verilog : multiplierIEEE754_verilog 
        Port map (clk       => clk,
                  rst       => rst,
                  op1       => op1,
                  op2       => op2,
                  in_rdy    => in_rdy(0),
                  res_rdy   => res_verilog_rdy,
                  res       => res_verilog);

    multiplier_vhdl : multiplierIEEE754_vhdl
        Port map (clk       => clk,
                  rst       => rst,
                  op1       => op1,
                  op2       => op2,
                  in_rdy    => in_rdy(0),
                  res_rdy   => res_vhdl_rdy,
                  res       => res_vhdl);

    ----------------------------------------------------------------------------
    -- FSM
    process (clk, rst) is
    begin
        if rst = '1' then 
            NEXT_STATE <= ST_0;

        elsif clk'EVENT and clk = '1' then
            case STATE is
                when ST_0 => 
                    NEXT_STATE <= ST_1;
                when ST_1 => 
                    if res_vhdl_rdy = '0' and res_verilog_rdy = '1' then 
                        NEXT_STATE <= ST_2;
                    elsif res_vhdl_rdy = '1' and res_verilog_rdy = '0' then 
                        NEXT_STATE <= ST_3;
                    elsif res_vhdl_rdy = '1' and res_verilog_rdy = '1' then 
                        NEXT_STATE <= ST_4;
                    else
                        NEXT_STATE <= ST_1;
                    end if;
                when ST_2 => 
                    NEXT_STATE <= ST_1;
                when ST_3 => 
                    NEXT_STATE <= ST_1;
                when ST_4 => 
                    NEXT_STATE <= ST_5;
                when ST_5 => 
                    NEXT_STATE <= ST_1;
                when others => 
                    NEXT_STATE <= ST_0;
            end case;
        end if;
    end process;
    
    ----------------------------------------------------------------------------
    -- DATAPATH
    process (NEXT_STATE) is
    begin
        STATE <= NEXT_STATE;
        case NEXT_STATE is
            when ST_0 => 
                tmp     <= (others => '0');
                res     <= (others => '0');
                res_rdy <= "00";
            when ST_1 => 
                -- do nothing
            when ST_2 => 
                res_rdy <= "01";
                res     <= res_verilog;
            when ST_3 => 
                res_rdy <= "10";
                res     <= res_vhdl;
            when ST_4 => 
                res_rdy <= "01";
                res     <= res_verilog;
                tmp     <= res_vhdl;
            when ST_5 => 
                res_rdy <= "10";
                res     <= tmp;
            when others => 
                -- do nothing
        end case;
    end process;

end architecture;
