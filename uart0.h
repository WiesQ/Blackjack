#include "MKL05Z4.h"

//----------------------------------------------
// SIM_SOPT2
//----------------------------------------------
// Clock source
//
#define CLK_DIS 					0x00
#define MCGFLLCLK 				0x01
#define OSCERCLK					0x02
#define MCGIRCLK					0x03
#define LF								0xa		// Enter
extern uint8_t rx_buf_pos;
extern char Too_Long[];
extern uint8_t rx_FULL;
extern uint8_t too_long;
extern uint8_t wfi;
extern char temp;
extern char rx_buff2[16];
extern char rx_buff3[16];
void UART0_Init(void);
void Uart_getdata(void);
void Uart_print(char a[]);
void UART0_IRQHandler(void); 

