#include <xaxivdma.h>
#include <xil_printf.h>
#include <xil_types.h>
#include <xparameters.h>

#include "config.h"
#include "DEMOSAIC/demosaic.h"
#include "DP_VIDEO/displayport.h"
#include "DP_VIDEO/video_modes.h"
#include "DP_VIDEO/xdpdma_video.h"
#include "GAMMA/gamma_lut.h"
#include "IMX219_CONFIG/config.h"
#include "MENU/menu_calls.h"
#include "PLATFORM/platform.h"
#include "TPG/tpg.h"
#include "UART/uartio.h"
#include "VTC/vtc.h"


XAxiVdma vdma;
XAxiVdma_DmaSetup vdmaDMA;
XAxiVdma_Config *vdmaConfig;
VideoMode video;

u32 frameBuf[DISPLAY_NUM_FRAMES][DEMO_MAX_FRAME];
u32 *pFrames[DISPLAY_NUM_FRAMES];

int main()
{
    video = VMODE_1280x720;
    init_platform();
    init_camera();
    per_write_reg(REG0,6);
    displayport_init();
    displayport_setup_interrupts();
    print("Configuration Complete\n\r");
    vtc_init(video);
    pFrames[0] = frameBuf[0];
    vdmaConfig = XAxiVdma_LookupConfig(XPAR_AXIVDMA_0_DEVICE_ID);
    XAxiVdma_CfgInitialize(&vdma, vdmaConfig, vdmaConfig->BaseAddress);
    vdmaDMA.FrameDelay              = 0;
    vdmaDMA.EnableCircularBuf       = 1;
    vdmaDMA.EnableSync              = 0;
    vdmaDMA.PointNum                = 0;
    vdmaDMA.EnableFrameCounter      = 0;
    vdmaDMA.VertSizeInput           = video.height;
    vdmaDMA.HoriSizeInput           = (video.width)*3;
    vdmaDMA.FixedFrameStoreAddr     = 0;
    vdmaDMA.FrameStoreStartAddr[0]  = (u32) pFrames[0];
    vdmaDMA.Stride                  = (video.width)*3;
    XAxiVdma_DmaConfig(&vdma, XAXIVDMA_WRITE, &(vdmaDMA));
    XAxiVdma_DmaSetBufferAddr(&vdma, XAXIVDMA_WRITE,vdmaDMA.FrameStoreStartAddr);
    XAxiVdma_DmaConfig(&vdma, XAXIVDMA_READ, &(vdmaDMA));
    XAxiVdma_DmaSetBufferAddr(&vdma, XAXIVDMA_READ,vdmaDMA.FrameStoreStartAddr);
    xil_printf("frame addr %x\n\r",vdmaDMA.FrameStoreStartAddr[0]);
    XAxiVdma_DmaStart(&vdma, XAXIVDMA_WRITE);
    XAxiVdma_StartParking(&vdma, 0, XAXIVDMA_WRITE);
    XAxiVdma_DmaStart(&vdma, XAXIVDMA_READ);
    XAxiVdma_StartParking(&vdma, 0, XAXIVDMA_READ);
    print("Configuration Complete\n\r");
    gamma_lut_init();
    tpg_init();
    demosaic_init();
    run_dppsu();
    print("Entire video pipeline activated\r\n");
     while(1){
         menu_calls(TRUE);
     }
    cleanup_platform();
    return 0;
}
