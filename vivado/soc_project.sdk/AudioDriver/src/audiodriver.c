#include "xparameters.h"    // Contains macros that define addresses of different devices
#include "xil_printf.h"     // Xilinx's printf function
#include "xil_io.h"         // Contains functions for AXI I/O
#include "FILTER_IIR.h"
#include "audiodriver.h"

int main()
{
	print("\n\rTesting Filter IP\n\r");

	u32 high_pass = 0;
	u32 band_pass = 0;
	u32 low_pass = 0;

	FILTER_IIR_mWriteReg(0x43C00000, FILTER_IIR_S00_AXI_SLV_REG0_OFFSET, COEF_REG_0);
	FILTER_IIR_mWriteReg(0x43C00000, FILTER_IIR_S00_AXI_SLV_REG1_OFFSET, COEF_REG_1);
	FILTER_IIR_mWriteReg(0x43C00000, FILTER_IIR_S00_AXI_SLV_REG2_OFFSET, COEF_REG_2);
	FILTER_IIR_mWriteReg(0x43C00000, FILTER_IIR_S00_AXI_SLV_REG3_OFFSET, COEF_REG_3);
	FILTER_IIR_mWriteReg(0x43C00000, FILTER_IIR_S00_AXI_SLV_REG4_OFFSET, COEF_REG_4);
	FILTER_IIR_mWriteReg(0x43C00000, FILTER_IIR_S00_AXI_SLV_REG5_OFFSET, COEF_REG_5);
	FILTER_IIR_mWriteReg(0x43C00000, FILTER_IIR_S00_AXI_SLV_REG6_OFFSET, COEF_REG_6);
	FILTER_IIR_mWriteReg(0x43C00000, FILTER_IIR_S00_AXI_SLV_REG7_OFFSET, COEF_REG_7);
	FILTER_IIR_mWriteReg(0x43C00000, FILTER_IIR_S00_AXI_SLV_REG8_OFFSET, COEF_REG_8);
	FILTER_IIR_mWriteReg(0x43C00000, FILTER_IIR_S00_AXI_SLV_REG9_OFFSET, COEF_REG_9);
	FILTER_IIR_mWriteReg(0x43C00000, FILTER_IIR_S00_AXI_SLV_REG10_OFFSET, COEF_REG_10);
	FILTER_IIR_mWriteReg(0x43C00000, FILTER_IIR_S00_AXI_SLV_REG11_OFFSET, COEF_REG_11);
	FILTER_IIR_mWriteReg(0x43C00000, FILTER_IIR_S00_AXI_SLV_REG12_OFFSET, COEF_REG_12);
	FILTER_IIR_mWriteReg(0x43C00000, FILTER_IIR_S00_AXI_SLV_REG13_OFFSET, COEF_REG_13);
	FILTER_IIR_mWriteReg(0x43C00000, FILTER_IIR_S00_AXI_SLV_REG14_OFFSET, COEF_REG_14);
	// reset signal
	FILTER_IIR_mWriteReg(0x43C00000, FILTER_IIR_S00_AXI_SLV_REG15_OFFSET, 0);
	//  When it is '1', the filter is waiting for SAMPLE_TRIG to start filtering. Otherwise it's constantly filtering.
	FILTER_IIR_mWriteReg(0x43C00000, FILTER_IIR_S00_AXI_SLV_REG16_OFFSET, 1);
	// High pass - enabling 0, disabling 1
	FILTER_IIR_mWriteReg(0x43C00000, FILTER_IIR_S00_AXI_SLV_REG17_OFFSET, high_pass);
	// Band Pass
	FILTER_IIR_mWriteReg(0x43C00000, FILTER_IIR_S00_AXI_SLV_REG18_OFFSET, band_pass);
	// Low Pass
	FILTER_IIR_mWriteReg(0x43C00000, FILTER_IIR_S00_AXI_SLV_REG19_OFFSET, low_pass);
	xil_printf("High Pass: %s\n\r", (high_pass)? "On" : "Off");
	xil_printf("Band Pass: %s\n\r", (band_pass)? "On" : "Off");
	xil_printf("Low Pass: %s\n\r", (low_pass)? "On" : "Off");

	return 0;
}
