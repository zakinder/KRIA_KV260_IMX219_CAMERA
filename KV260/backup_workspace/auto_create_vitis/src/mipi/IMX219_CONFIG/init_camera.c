#include <xiicps.h>
#include <xil_printf.h>
#include <xil_types.h>
#include <xstatus.h>

#include "imx219.h"
#include "I2c_transections.h"

XIicPs iic_cam;
#define IIC_DEVICEID        XPAR_XIICPS_0_DEVICE_ID
#define IIC_SCLK_RATE		100000
#define SW_IIC_ADDR         0x74
u8 SendBuffer [10];
int init_camera()
{
	XIicPs_Config *iic_conf;
	int Status;
    print("Init Camera\n\r");
	iic_conf = XIicPs_LookupConfig(IIC_DEVICEID);
	XIicPs_CfgInitialize(&iic_cam,iic_conf,iic_conf->BaseAddress);
	XIicPs_SetSClk(&iic_cam, IIC_SCLK_RATE);
    i2c_init(&iic_cam, IIC_DEVICEID,IIC_SCLK_RATE);
    SendBuffer[0]= 0x04;
    Status = XIicPs_MasterSendPolled(&iic_cam, SendBuffer, 1, SW_IIC_ADDR);
  	if (Status != XST_SUCCESS) {
  		print("I2C write error\n\r");
  		return XST_FAILURE;
  	}
  	sensor_init(&iic_cam);
    return 0;
}




