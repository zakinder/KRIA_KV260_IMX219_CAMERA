
#include <math.h>
#include <xil_printf.h>
#include <xil_types.h>
#include <xparameters.h>
#include <xstatus.h>
#include <xv_gamma_lut.h>

#include "../config.h"

XV_gamma_lut gamma_inst;

void gamma_calc(float gamma_val);

u16 gamma_reg[1024];
int gamma_lut_init() {
	gamma_calc(1.2);
	XV_gamma_lut_Initialize(&gamma_inst, XPAR_PS_VIDEO_RX_VIDEO_V_GAMMA_LUT_0_DEVICE_ID);
	XV_gamma_lut_Set_HwReg_width(&gamma_inst, VIDEO_COLUMNS);
	XV_gamma_lut_Set_HwReg_height(&gamma_inst, VIDEO_ROWS);
	XV_gamma_lut_Set_HwReg_video_format(&gamma_inst, 0x00);
	XV_gamma_lut_Write_HwReg_gamma_lut_0_Bytes(&gamma_inst, 0,(int *) gamma_reg, 512);
	XV_gamma_lut_Write_HwReg_gamma_lut_1_Bytes(&gamma_inst, 0,(int *) gamma_reg, 512);
	XV_gamma_lut_Write_HwReg_gamma_lut_2_Bytes(&gamma_inst, 0,(int *) gamma_reg, 512);
	XV_gamma_lut_Start(&gamma_inst);
	XV_gamma_lut_EnableAutoRestart(&gamma_inst);
	//xil_printf("Gamma correction LUT initialized\r\n");
	return XST_SUCCESS;
}

void gamma_calc(float gamma_val)
{
	int i;
	for(i = 0; i<256; i++){
		gamma_reg[i] = (pow((i / 256.0), (1/gamma_val)) * 256.0);
	}
}
