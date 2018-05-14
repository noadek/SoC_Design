#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

#include "xparameters.h"    // Contains macros that define addresses of different devices
#include "xil_printf.h"     // Xilinx's printf function
#include "xil_io.h"         // Contains functions for AXI I/O
#include "FILTER_IIR.h"
#include "Volume_Pregain.h"
#include "AXI_AUDIO.h"

#define FILTER_BASE XPAR_FILTER_IIR_0_S00_AXI_BASEADDR
#define VOLUME_BASE XPAR_VOLUME_PREGAIN_0_S00_AXI_BASEADDR
#define AXI_BASE XPAR_AXI_AUDIO_0_S00_AXI_BASEADDR

int main()
{
	print("\n\rTesting volume control with filter IP\n\r");
	/*u32 coef_reg0 = 0x00002CB6;
	u32 coef_reg1 = 0x0000596C;
	u32 coef_reg2 = 0x00002CB6;
	u32 coef_reg3 = 0x8097A63A;
	u32 coef_reg4 = 0x3F690C9D;
	u32 coef_reg5 = 0x074D9236;
	u32 coef_reg6 = 0x00000000;
	u32 coef_reg7 = 0xF8B26DCA;
	u32 coef_reg8 = 0x9464B81B;
	u32 coef_reg9 = 0x3164DB93;
	u32 coef_reg10 = 0x12BEC333;
	u32 coef_reg11 = 0xDA82799A;
	u32 coef_reg12 = 0x12BEC333;
	u32 coef_reg13 = 0x00000000;
	u32 coef_reg14 = 0x0AFB0CCC;
	u32 high_pass = 0;
	u32 band_pass = 0;
	u32 low_pass = 0;

	FILTER_IIR_mWriteReg(FILTER_BASE, FILTER_IIR_S00_AXI_SLV_REG0_OFFSET, coef_reg0);
	FILTER_IIR_mWriteReg(FILTER_BASE, FILTER_IIR_S00_AXI_SLV_REG1_OFFSET, coef_reg1);
	FILTER_IIR_mWriteReg(FILTER_BASE, FILTER_IIR_S00_AXI_SLV_REG2_OFFSET, coef_reg2);
	FILTER_IIR_mWriteReg(FILTER_BASE, FILTER_IIR_S00_AXI_SLV_REG3_OFFSET, coef_reg3);
	FILTER_IIR_mWriteReg(FILTER_BASE, FILTER_IIR_S00_AXI_SLV_REG4_OFFSET, coef_reg4);
	FILTER_IIR_mWriteReg(FILTER_BASE, FILTER_IIR_S00_AXI_SLV_REG5_OFFSET, coef_reg5);
	FILTER_IIR_mWriteReg(FILTER_BASE, FILTER_IIR_S00_AXI_SLV_REG6_OFFSET, coef_reg6);
	FILTER_IIR_mWriteReg(FILTER_BASE, FILTER_IIR_S00_AXI_SLV_REG7_OFFSET, coef_reg7);
	FILTER_IIR_mWriteReg(FILTER_BASE, FILTER_IIR_S00_AXI_SLV_REG8_OFFSET, coef_reg8);
	FILTER_IIR_mWriteReg(FILTER_BASE, FILTER_IIR_S00_AXI_SLV_REG9_OFFSET, coef_reg9);
	FILTER_IIR_mWriteReg(FILTER_BASE, FILTER_IIR_S00_AXI_SLV_REG10_OFFSET, coef_reg10);
	FILTER_IIR_mWriteReg(FILTER_BASE, FILTER_IIR_S00_AXI_SLV_REG11_OFFSET, coef_reg11);
	FILTER_IIR_mWriteReg(FILTER_BASE, FILTER_IIR_S00_AXI_SLV_REG12_OFFSET, coef_reg12);
	FILTER_IIR_mWriteReg(FILTER_BASE, FILTER_IIR_S00_AXI_SLV_REG13_OFFSET, coef_reg13);
	FILTER_IIR_mWriteReg(FILTER_BASE, FILTER_IIR_S00_AXI_SLV_REG14_OFFSET, coef_reg14);
	// reset signal
	FILTER_IIR_mWriteReg(FILTER_BASE, FILTER_IIR_S00_AXI_SLV_REG15_OFFSET, 0);
	//  When it is '1', the filter is waiting for SAMPLE_TRIG to start filtering. Otherwise it's constantly filtering.
	FILTER_IIR_mWriteReg(FILTER_BASE, FILTER_IIR_S00_AXI_SLV_REG16_OFFSET, 1);
	// High pass - enabling 0, disabling 1
	FILTER_IIR_mWriteReg(FILTER_BASE, FILTER_IIR_S00_AXI_SLV_REG17_OFFSET, high_pass);
	// Band Pass
	FILTER_IIR_mWriteReg(FILTER_BASE, FILTER_IIR_S00_AXI_SLV_REG18_OFFSET, band_pass);
	// Low Pass
	FILTER_IIR_mWriteReg(FILTER_BASE, FILTER_IIR_S00_AXI_SLV_REG19_OFFSET, low_pass);

	//Volume control
	VOLUME_PREGAIN_mWriteReg(VOLUME_BASE, VOLUME_PREGAIN_S00_AXI_SLV_REG0_OFFSET, 512);
	VOLUME_PREGAIN_mWriteReg(VOLUME_BASE, VOLUME_PREGAIN_S00_AXI_SLV_REG1_OFFSET, 512);

	xil_printf("High Pass: %s\n\r", (!high_pass)? "On" : "Off");
	xil_printf("Band Pass: %s\n\r", (!band_pass)? "On" : "Off");
	xil_printf("Low Pass: %s\n\r", (!low_pass)? "On" : "Off");*/

	while (1) {
		xil_printf("hello");
		short audioChunk;
		int fd = open("test.wav", O_RDONLY);
		int bytes_read = read(fd, &audioChunk, 2);
		if (bytes_read < 0) {
			break;
		}
		xil_printf("%d", (int) audioChunk);
		// sign extend 16 bit data and put in register
		AXI_AUDIO_mWriteReg(AXI_BASE, AXI_AUDIO_S00_AXI_SLV_REG0_OFFSET, (int) audioChunk);
	}

	return 0;
}
