#include "menu_calls.h"
#include <stdio.h>
#include "../UART/uartio.h"
#include "config_defines.h"
void menu_calls(ON_OFF) {
    int menu_calls_enable = ON_OFF;
    unsigned int uart_io;
    u32 current_state = mainmenu;
    u32 k_number;
    u32 k_number_value;

//	 per_write_reg(REG0,1);
//	 usleep(1000000);
//	 per_write_reg(REG1,2000);
//	 usleep(1000000);
//	 per_write_reg(REG2,250);
//	 usleep(1000000);
//	 per_write_reg(REG3,250);
//	 usleep(1000000);
//	 per_write_reg(REG4,250);
//	 usleep(1000000);
//	 per_write_reg(REG5,2000);
//	 usleep(1000000);
//	 per_write_reg(REG6,250);
//	 usleep(1000000);
//	 per_write_reg(REG7,250);
//	 usleep(1000000);
//	 per_write_reg(REG8,250);
//	 usleep(1000000);
//	 per_write_reg(REG9,2000);
//	 usleep(1000000);


    while (menu_calls_enable == TRUE)
    {
        switch (current_state)
        {
        case mainmenu:
            cmds_menu();
            current_state = menu_select;
            break;
        case menu_select:
            uart_io = uart_prompt_io();
            if (uart_io == 0) {
                uart_Red_Text();
                printf("Unknown command entered %x\r\n",(unsigned) uart_io);
                printf("\r>>");
                uart_Default_Text();
                break;
            } else {
                uart_Default_Text();
                current_state = uart_io;
                break;
            }
            break;
        case clear:
            /*****************************************************************************************************************/
            menu_cls();
            current_state = mainmenu;break;
            /*****************************************************************************************************************/
        case kernal:
            /*****************************************************************************************************************/
            printf("Enter k_number\n");
            menu_print_prompt();
            k_number = uart_prompt_io();
            if (k_number == 0 || k_number == 4  || k_number == 8 || k_number == 12 || k_number == 16 || k_number == 20 || k_number == 24 || k_number == 28 || k_number == 32 || k_number == 36 || k_number == 40 || k_number == 44 || k_number == 48 || k_number == 52 || k_number == 56 || k_number == 60 || k_number == 64 || k_number == 68 || k_number == 72)
            {
                printf("Enter k_number_value\n");
                menu_print_prompt();
                k_number_value = uart_prompt_io();
                per_write_reg(k_number,k_number_value);
                current_state = kernal;break;
            }
            else if (k_number == cmds_quit)
            {
                printf("Entered Quit\n");
                current_state = mainmenu;break;
            }
            else
            {
                printf("Enter Wrong Register Value\n");
            	current_state = kernal;break;
            }
        case quit:
            menu_calls_enable = FALSE;
            break;
        case cmds_uart:
            uartvalue();
            current_state =uartcmd(mainmenu,cmds_uart);
            break;
        default:
            printf("\r\n");
            current_state = mainmenu;
            break;
        }
    }
    printf("Break\r\n");
    menu_calls_enable = TRUE;
}




