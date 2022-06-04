#include "imx219.h"

#include "xiicps.h"
#include "xparameters.h"
#include "sleep.h"
#include <xil_printf.h>

#include "../config.h"
#include "I2c_transections.h"
#define IIC_CAM_ADDR  		0x10
#define IIC_IMX477_ADDR  	0x64



struct reginfo cfg_702p_60fps[] =
{
    {0x30EB, 0x05},
    {0x30EB, 0x0C},
    {0x300A, 0xFF},
    {0x300B, 0xFF},
    {0x30EB, 0x05},
    {0x30EB, 0x09},
    {0x0114, 0x01}, // CSI_LANE_MODE = 2-lane
    {0x0128, 0x00}, // DPHY_CTRL = auto mode
    {0x012A, 0x13}, // EXCLK_FREQ[15:8]
    {0x012B, 0x34}, // EXCLK_FREQ[7:0] = 4916 MHz
    {0x0160, 0x04}, // FRM_LENGTH_A[15:8]
    {0x0161, 0x60}, // FRM_LENGTH_A[7:0] = 1120
    {0x0162, 0x0D}, // LINE_LENGTH_A[15:8]
    {0x0163, 0x78}, // LINE_LENGTH_A[7:0] = 3448
    {0x0164, 0x01}, // XADD_STA_A[11:8]
    {0x0165, 0x58}, // XADD_STA_A[7:0] = X top left = 344
    {0x0166, 0x0B}, // XADD_END_A[11:8]
    {0x0167, 0x77}, // XADD_END_A[7:0] = X bottom right = 2935
    {0x0168, 0x01}, // YADD_STA_A[11:8]
    {0x0169, 0xF0}, // YADD_STA_A[7:0] = Y top left = 496
    {0x016A, 0x07}, // YADD_END_A[11:8]
    {0x016B, 0xAF}, // YADD_END_A[7:0] = Y bottom right = 1967
    {0x016C, 0x05}, // x_output_size[11:8]
    {0x016D, 0x10}, // x_output_size[7:0] = 1296
    {0x016E, 0x02}, // y_output_size[11:8]
    {0x016F, 0xE0}, // y_output_size[7:0] = 736
    {0x0170, 0x01}, // X_ODD_INC_A
    {0x0171, 0x01}, // Y_ODD_INC_A
    {0x0174, 0x01}, // BINNING_MODE_H_A = x2-binning
    {0x0175, 0x01}, // BINNING_MODE_V_A = x2-binning
    // {0x0174, 0x00}, // BINNING_MODE_H_A = no-binning
    // {0x0175, 0x00}, // BINNING_MODE_V_A = no-binning
    {0x0176, 0x01}, // BINNING_CAL_MODE_H_A
    {0x0177, 0x01}, // BINNING_CAL_MODE_V_A
    {0x018C, 0x0A}, // CSI_DATA_FORMAT_A[15:8]
    {0x018D, 0x0A}, // CSI_DATA_FORMAT_A[7:0]
    {0x0301, 0x05},
    {0x0303, 0x01},
    {0x0304, 0x02},
    {0x0305, 0x02},
    {0x0309, 0x0A}, // OPPXCK_DIV
    {0x030B, 0x01}, // OPSYCK_DIV
    {0x0306, 0x00}, // PLL_VT_MPY[10:8]
    //{0x0307, 0x2E}, // PLL_VT_MPY[7:0] = 46
    {0x0307, 0x17}, // PLL_VT_MPY[7:0] = 23
    //{0x0307, 0x0F}, // PLL_VT_MPY[7:0] = 15
    {0x030C, 0x00}, // PLL_OP_MPY[10:8]
    //{0x030D, 0x5C}, // PLL_OP_MPY[7:0] = 92
    {0x030D, 0x2E}, // PLL_OP_MPY[7:0] = 46
    //{0x030D, 0x1E}, // PLL_OP_MPY[7:0] = 30
    {0x455E, 0x00},
    {0x471E, 0x4B},
    {0x4767, 0x0F},
    {0x4750, 0x14},
    {0x4540, 0x00},
    {0x47B4, 0x14},
    {0x4713, 0x30},
    {0x478B, 0x10},
    {0x478F, 0x10},
    {0x4793, 0x10},
    {0x4797, 0x0E},
    {0x479B, 0x0E},
    //{0x0601, 0x02}, // Test pattern = Color bar
//    {0x0601, 0x00}, // Test pattern = Normal work
//    {0x0620, 0x00}, // TP_WINDOW_X_OFFSET[11:8]
//    {0x0621, 0x00}, // TP_WINDOW_X_OFFSET[7:0]
//    {0x0621, 0x00}, // TP_WINDOW_Y_OFFSET[11:8]
//    {0x0623, 0x00}, // TP_WINDOW_Y_OFFSET[7:0]
//    {0x0624, 0x05}, // TP_WINDOW_WIDTH[11:8]
//    {0x0625, 0x00}, // TP_WINDOW_WIDTH[7:0] = 1280
//    {0x0626, 0x02}, // TP_WINDOW_HEIGHT[11:8]
//    {0x0627, 0xD0}, // TP_WINDOW_HEIGHT[7:0] = 720
    {0x0100, 0x01}, /* mode select streaming on */
	{SEQUENCE_END, 0x00}
};



struct reginfo cfg_1080p_30fps[] =
{
		{0x30EB, 0x05},
		{0x30EB, 0x0C},
		{0x300A, 0xFF},
		{0x300B, 0xFF},
		{0x30EB, 0x05},
		{0x30EB, 0x09},
		{0x0114, 0x01},
		{0x0128, 0x00},
		{0x012A, 0x18},
		{0x012B, 0x00},
		{0x0160, 0x04},
		{0x0161, 0x59},
		{0x0162, 0x0D},
		{0x0163, 0x78},
		{0x0164, 0x02},
		{0x0165, 0xA8},
		{0x0166, 0x0A},
		{0x0167, 0x27},
		{0x0168, 0x02},
		{0x0169, 0xB4},
		{0x016A, 0x06},
		{0x016B, 0xEB},
		{0x016C, 0x07},
		{0x016D, 0x80},
		{0x016E, 0x04},
		{0x016F, 0x38},
		{0x0170, 0x01},
		{0x0171, 0x01},
		{0x0174, 0x00},
		{0x0175, 0x00},
		{0x018C, 0x0A},
		{0x018D, 0x0A},
		{0x0301, 0x05},
		{0x0303, 0x01},
		{0x0304, 0x03},
		{0x0305, 0x03},
		{0x0306, 0x00},
		{0x0307, 0x39},
		{0x0309, 0x0A},
		{0x030B, 0x01},
		{0x030C, 0x00},
		{0x030D, 0x72},
		{0x455E, 0x00},
		{0x471E, 0x4B},
		{0x4767, 0x0F},
		{0x4750, 0x14},
		{0x4540, 0x00},
		{0x47B4, 0x14},
		{0x4713, 0x30},
		{0x478B, 0x10},
		{0x478F, 0x10},
		{0x4793, 0x10},
		{0x4797, 0x0E},
		{0x479B, 0x0E},
		{0x0100, 0x01},
        
		{SEQUENCE_END, 0x00}
};

int ov5640_read(XIicPs *IicInstance,u16 addr,u8 *read_buf)
{
	*read_buf=i2c_reg16_read(IicInstance,IIC_CAM_ADDR,addr);
	return XST_SUCCESS;
}
int imx477_read(XIicPs *IicInstance,u16 addr,u8 *read_buf)
{
	*read_buf=i2c_reg16_read(IicInstance,100,addr);
	return XST_SUCCESS;
}
int scan_read(XIicPs *IicInstance,u16 addr,u8 *read_buf,u16 scan_addr)
{
	*read_buf=i2c_reg16_read(IicInstance,scan_addr,addr);
	return XST_SUCCESS;
}

int ov5640_write(XIicPs *IicInstance,u16 addr,u8 data)
{

	return i2c_reg16_write(IicInstance,IIC_CAM_ADDR,addr,data);
}
int imx477_write(XIicPs *IicInstance,u16 addr,u8 data)
{

	return i2c_reg16_write(IicInstance,100,addr,data);
}

int ov5640_soft_write(XIicPs *IicInstance,u16 addr,u8 data)
{

	return i2c_reg16_write(IicInstance,0x74,addr,data);
}

/* write a array of registers  */
void sensor_write_array(XIicPs *IicInstance, struct reginfo *regarray)
{
	int i = 0;
	while (regarray[i].reg != SEQUENCE_END) {
		ov5640_write(IicInstance,regarray[i].reg,regarray[i].val);
		i++;
	}

}
void imx_477_sensor_write_array(XIicPs *IicInstance, struct reginfo *regarray)
{
	int i = 0;
	while (regarray[i].reg != SEQUENCE_END) {
		imx477_write(IicInstance,regarray[i].reg,regarray[i].val);
		i++;
	}

}


int sensor_init(XIicPs *IicInstance)
{
	u8 sensor_id[2] ;
	ov5640_read(IicInstance, 0x0000, &sensor_id[0]);
	ov5640_read(IicInstance, 0x0001, &sensor_id[1]);
	if (sensor_id[0] != 0x2 || sensor_id[1] != 0x19)
	{
		printf("Not ov5640 id, %x %x\n", sensor_id[0], sensor_id[1]);
	}
	else
	{
		printf("Got ov5640 id, %x %x\r\n", sensor_id[0], sensor_id[1]);
	}
	usleep(100000);
#if P1080 == 1
	//1080p 30fps
	sensor_write_array(IicInstance,cfg_1080p_30fps);
#else
	//720p 60fps
	sensor_write_array(IicInstance,cfg_702p_60fps);
#endif
	return 0;
}

int imx477_sensor_init(XIicPs *IicInstance)
{
	u8 sensor_id[2] ;
	scan_read(IicInstance, 0x0000, &sensor_id[0],99);
	scan_read(IicInstance, 0x0001, &sensor_id[1],99);
	if (sensor_id[0] != 4 || sensor_id[1] != 4)
	{
		printf("Not imx477 id, %x %x\n", sensor_id[0], sensor_id[1]);
	}
	else
	{
		printf("Got imx477 id, %x %x\r\n", sensor_id[0], sensor_id[1]);
	}
	usleep(1000000);
	scan_read(IicInstance, 0x0000, &sensor_id[0],100);
	scan_read(IicInstance, 0x0001, &sensor_id[1],100);
	if (sensor_id[0] != 4 || sensor_id[1] != 4)
	{
		printf("Not imx477 id, %x %x\n", sensor_id[0], sensor_id[1]);
	}
	else
	{
		printf("Got imx477 id, %x %x\r\n", sensor_id[0], sensor_id[1]);
	}
	usleep(1000000);
	scan_read(IicInstance, 0x0000, &sensor_id[0],101);
	scan_read(IicInstance, 0x0001, &sensor_id[1],101);
	if (sensor_id[0] != 4 || sensor_id[1] != 4)
	{
		printf("Not imx477 id, %x %x\n", sensor_id[0], sensor_id[1]);
	}
	else
	{
		printf("Got imx477 id, %x %x\r\n", sensor_id[0], sensor_id[1]);
	}

	usleep(1000000);
	imx477_read(IicInstance, 0x0000, &sensor_id[0]);
	imx477_read(IicInstance, 0x0001, &sensor_id[1]);

	if (sensor_id[0] != 4 || sensor_id[1] != 4)
	{
		printf("Not imx477 id, %x %x\n", sensor_id[0], sensor_id[1]);
	}
	else
	{
		printf("Got imx477 id, %x %x\r\n", sensor_id[0], sensor_id[1]);
	}

	usleep(1000000);

#if P1080 == 1
	//1080p 30fps
	imx_477_sensor_write_array(IicInstance,cfg_1080p_30fps);
#else
	//720p 60fps
	imx_477_sensor_write_array(IicInstance,cfg_702p_60fps);
#endif

	return 0;
}
int scan_sensor(XIicPs *IicInstance)
{
	for(int address = 0; address < 255; address++ )
	 {
		u8 sensor_id[2] ;
		scan_read(IicInstance, 0x0000, &sensor_id[0],address);
		scan_read(IicInstance, 0x0001, &sensor_id[1],address);

		if (sensor_id[0] != 0x4)
		{
			printf("Id @ address = %x is %x %x\n",address, sensor_id[0], sensor_id[1]);
		}
		else
		{
			printf("Id @ address ==== %x is %x %x\r\n",address, sensor_id[0], sensor_id[1]);
		}
	 }
	usleep(1000000);
	return 0;
}
