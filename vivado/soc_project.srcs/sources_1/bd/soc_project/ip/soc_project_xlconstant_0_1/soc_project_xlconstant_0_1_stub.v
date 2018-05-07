// Copyright 1986-2017 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2017.3 (lin64) Build 2018833 Wed Oct  4 19:58:07 MDT 2017
// Date        : Mon May  7 17:08:08 2018
// Host        : lx25 running 64-bit SUSE Linux Enterprise Desktop 12 SP2
// Command     : write_verilog -force -mode synth_stub
//               /home/adadek/SoC_Design/vivado/soc_project.srcs/sources_1/bd/soc_project/ip/soc_project_xlconstant_0_1/soc_project_xlconstant_0_1_stub.v
// Design      : soc_project_xlconstant_0_1
// Purpose     : Stub declaration of top-level module interface
// Device      : xc7z020clg484-1
// --------------------------------------------------------------------------------

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// The synthesis directives are for Synopsys Synplify support to prevent IO buffer insertion.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
(* X_CORE_INFO = "xlconstant_v1_1_3_xlconstant,Vivado 2017.3" *)
module soc_project_xlconstant_0_1(dout)
/* synthesis syn_black_box black_box_pad_pin="dout[0:0]" */;
  output [0:0]dout;
endmodule
