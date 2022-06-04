// LAST TESTED : 09/08/2019
#ifndef __SYSTEM_CONFIG_DEFINES_H__
#define __SYSTEM_CONFIG_DEFINES_H__
#include "xparameters.h"

#define READ_WRITE_MUL_FACTOR 0x10
#define DEFAULT_HYPERTERMINAL_UART_ID 	XPAR_PS7_UART_1_DEVICE_ID
#define UART_BASEADDR        			XPAR_PSU_UART_1_BASEADDR
#define D5M_BASE        			    XPAR_PS_VIDEO_RX_VIDEO_VFP_0_VFPCONFIG_BASEADDR

/*****************************************************************************************************************/
#define D5M_mWriteReg(BaseAddress, RegOffset, Data) \
  	Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define D5M_mReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
/*****************************************************************************************************************/



/*****************************************************************************************************************/
#ifndef uartPRINTF
#define uartPRINTF    printf
#endif
#define uart_Black_Text() uartPRINTF("\x1B[30m")
#define uart_Red_Text() uartPRINTF("\x1B[31m")
#define uart_Green_Text() uartPRINTF("\x1B[32m")
#define uart_Yellow_Text() uartPRINTF("\x1B[33m")
#define uart_Blue_Text() uartPRINTF("\x1B[34m")
#define uart_Magenta_Text() uartPRINTF("\x1B[35m")
#define uart_Cyan_Text() uartPRINTF("\x1B[36m")
#define uart_White_Text() uartPRINTF("\x1B[37m")
#define uart_Default_Text() uartPRINTF("\x1B[39m")
#define print_prompt() uartPRINTF("\x1B[31m")
#define print_prompt_Default_Text() uartPRINTF("\x1B[39m")
#define uart_Black_Background() uartPRINTF("\x1B[40m")
#define uart_Red_Background() uartPRINTF("\x1B[41m")
#define uart_Green_Background() uartPRINTF("\x1B[42m")
#define uart_Yellow_Background() uartPRINTF("\x1B[43m")
#define uart_Blue_Background() uartPRINTF("\x1B[44m")
#define uart_Magenta_Background() uartPRINTF("\x1B[45m")
#define uart_Cyan_Background() uartPRINTF("\x1B[46m")
#define uart_White_Background() uartPRINTF("\x1B[47m")
#define uart_Default_Background() uartPRINTF("\x1B[49m")
#define uart_Cursor_Home() uartPRINTF("\x1B[H")
#define uart_Cursor_Set_XY_Position(X,Y) uartPRINTF("\x1B[%d;%dH",X,Y)
#define uart_Cursor_Show() uartPRINTF("\x1B[?25h")
#define uart_Cursor_Hide() uartPRINTF("\x1B[?25l")
#define uart_Cursor_Up(Rows) uartPRINTF("\x1B[%dA",Rows)
#define uart_Cursor_Down(Rows) uartPRINTF("\x1B[%dB",Rows)
#define uart_Cursor_Left(Columns) uartPRINTF("\x1B[%dC",Columns)
#define uart_Cursor_Right(Columns) uartPRINTF("\x1B[%dD",Columns)
#define uart_Clear_Line_To_End() uartPRINTF("\x1B[0K")
#define uart_Clear_Line_Up_To_Cursor() uartPRINTF("\x1B[1K")
#define uart_Clear_Line() uartPRINTF("\x1B[2K")
#define uart_Clear_Screen() uartPRINTF("\x1B[2J")
#define uart_Clear_Screen_To_End() uartPRINTF("\x1B[J")
#define uart_Clear_Screen_Up_To_Cursor() uartPRINTF("\x1B[1J")
#define uart_Text_Attributes_None() uartPRINTF("\x1B[0m")
#define uart_Text_Attributes_Bold() uartPRINTF("\x1B[1m")
#define uart_Text_Attributes_italic() uartPRINTF("\x1B[3m")
#define uart_Text_Attributes_underline() uartPRINTF("\x1B[4m")
#define uart_Text_Attributes_strike() uartPRINTF("\x1B[9m")
#define uart_Text_Attributes_Dim() uartPRINTF("\x1B[2m")
#define uart_Text_Attributes_Blink() uartPRINTF("\x1B[5m")
#define uart_Text_Attributes_Reverse() uartPRINTF("\x1B[7m")
#define uart_Text_Attributes_Hidden() uartPRINTF("\x1B[8m")
#define uart_Mode_Insert() uartPRINTF("\x1B[4h")
#define uart_Mode_Overwrite() uartPRINTF("\x1B[4l")
#define uart_Mode_Scrolling_Smooth() uartPRINTF("\x1B[?4h")
#define uart_Mode_Scrolling_Jump() uartPRINTF("\x1B[?4l")
#define uart_Editing_Insert_Lines(Lines) uartPRINTF("\x1B[%dL",Lines)
#define uart_Editing_Delete_Lines(Lines) uartPRINTF("\x1B[%dM",Lines)
#define uart_Editing_Delete_Characters(Characters) uartPRINTF("\x1B[%dP",Characters)
/*****************************************************************************************************************/
#define KEYPRESS_ARROW_UP       240
#define KEYPRESS_ARROW_DOWN     241
#define KEYPRESS_ARROW_LEFT     242
#define KEYPRESS_ARROW_RIGHT    243
#define KEYPRESS_END            244
/*****************************************************************************************************************/
#define menu_select             1
#define menuCheck               19
#define kernalCoef              0
/*****************************************************************************************************************/



#define kernalconfig            0x6769666E
#define clear                   0x10ECC0
#define mainmenu                0x4773053D
#define uart_0_baseaddr         0xe0000000


#define timex                   0x137C66
#define display1                0x429F579F
#define display2                0x429F57A0
#define display3                0x429F57A1
#define display5                0x429F57A3
#define display6                0x429F57A4
#define quit                    0x1EBAA
#define pl_testpattern          0x65747461
#define hdmi                    0x1C1FB
#define vga                     0x327F
#define xbright                 0xA8B548
#define blur                    0x1AE04
#define emboss                  0xAC6E0D
#define sharp                   0x134D38
#define hsv                     0x2D94
#define hsl                     0x2D8A
#define rgb                     0x30F0
#define rgbpoi                  0xBF59FF
#define sobel                   0x13686E
#define prewit                  0xBE08B2
#define prewitt                 0x76C5768
#define cgain                   0x10D7F4
#define menucall                0x47B89810
/************************************/

#define d5mgain                 0x60FE0B4
/************************************/
#define hsvpervalue             0xF43BADEF
#define yccpervalue             0x5E197EEF
#define lumvalue                0x4812C2CF
/************************************/
#define rgbdetect               0xEB640CB6
#define kernal                  0xB49AAE

#define quit_current_state      0x71



#define cmds_quit               0x95BA400A
#define cmds_space              0x20
#define cmds_uart               0x95BA4834
#define cmds_hdmitx             0x7CB0C08C
#define cmds_videoconfig        0x6F636F65
#define cmds_readfifo           0x66696664
#define cmds_fifomode           0x646F6D6F
#define cmds_fifostatus         0x5B28281D
#define cmds_gridpoint          0x148D411C
#define cmds_griddelta          0x148B46A1
#define sdcard                  0xC06994
#define wsd                     0x335E
#define rsd                     0x316A

/*****************************************************************************************************************/
#endif // __SYSTEM_CONFIG_DEFINES_H__
