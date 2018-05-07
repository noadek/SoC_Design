-- Copyright 1986-2017 Xilinx, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2017.3 (lin64) Build 2018833 Wed Oct  4 19:58:07 MDT 2017
-- Date        : Mon May  7 17:08:08 2018
-- Host        : lx25 running 64-bit SUSE Linux Enterprise Desktop 12 SP2
-- Command     : write_vhdl -force -mode synth_stub
--               /home/adadek/SoC_Design/vivado/soc_project.srcs/sources_1/bd/soc_project/ip/soc_project_xlconstant_0_1/soc_project_xlconstant_0_1_stub.vhdl
-- Design      : soc_project_xlconstant_0_1
-- Purpose     : Stub declaration of top-level module interface
-- Device      : xc7z020clg484-1
-- --------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity soc_project_xlconstant_0_1 is
  Port ( 
    dout : out STD_LOGIC_VECTOR ( 0 to 0 )
  );

end soc_project_xlconstant_0_1;

architecture stub of soc_project_xlconstant_0_1 is
attribute syn_black_box : boolean;
attribute black_box_pad_pin : string;
attribute syn_black_box of stub : architecture is true;
attribute black_box_pad_pin of stub : architecture is "dout[0:0]";
attribute X_CORE_INFO : string;
attribute X_CORE_INFO of stub : architecture is "xlconstant_v1_1_3_xlconstant,Vivado 2017.3";
begin
end;
