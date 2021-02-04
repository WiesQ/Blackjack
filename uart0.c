#include "uart0.h"

void UART0_Init(void)
{
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;							// Enable clock for UART0
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;							// Enable clock for Port B
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(MCGFLLCLK);		        // Clock MCGFLLCLK=41943040Hz (CLOCK_SETUP=0)
	PORTB->PCR[1] = PORT_PCR_MUX(2);							//PTB1=TX_D
	PORTB->PCR[2] = PORT_PCR_MUX(2);							//PTB2=RX_D
	
	UART0->C2 &= ~(UART0_C2_TE_MASK | UART0_C2_RE_MASK );		// Disable TX & RX
	UART0->BDH = 0;
	UART0->BDL =91;		// For CLOCK_SETUP=0 BR=28800	BDL=91	:	CLOCK_SETUP=1	BR=230400		BDL=13
	UART0->C4 &= ~UART0_C4_OSR_MASK;
	UART0->C4 |= UART0_C4_OSR(15);	// For CLOCK_SETUP=0 BR=28800	OSR=15	:	CLOCK_SETUP=1	BR=230400		OSR=15
	UART0->C5 |= UART0_C5_BOTHEDGE_MASK;	// Both edge sampling
	UART0->C2 |= UART0_C2_RIE_MASK;		// Enable IRQ
	UART0->C2 |= (UART0_C2_TE_MASK | UART0_C2_RE_MASK);		// Enable TX & RX
	NVIC_EnableIRQ(UART0_IRQn);
	NVIC_ClearPendingIRQ(UART0_IRQn);
}
void Uart_getdata(){
	wfi = 1;
		while(wfi==1){
			
		if(rx_FULL)		// Is data ready?
		{
			if(too_long)
			{
				for(int i=0;Too_Long[i]!=0;i++)	// Too long
					{
						while(!(UART0->S1 & UART0_S1_TDRE_MASK));	// Is TX ready?
						UART0->D = Too_Long[i];
					}
					while(!(UART0->S1 & UART0_S1_TDRE_MASK));	// Is RX ready
					UART0->D = 0xa;		// Next line
					too_long=0;
			}
			else
			{
			wfi=0;
			rx_buf_pos=0;
			}
				
}
}
}
void Uart_print(char a[]){
					for(int i=0;a[i]!=0;i++)   // spit out P2Balance
			{
				while(!(UART0->S1 & UART0_S1_TDRE_MASK));
				UART0->D = a[i];

			}
						//rx_FULL=0;	// allow interrupt 
			rx_buf_pos=0;
}
void UART0_IRQHandler()
{
	if(UART0->S1 & UART0_S1_RDRF_MASK)
	{
		
		temp=UART0->D;	// Read data s flagi RDRF
		if(!rx_FULL)
		{
			if(temp!=LF)
			{
				if(!too_long)	// Timeout, ignore rest
				{
					rx_buff2[rx_buf_pos] = temp;	// Take bytes
					rx_buf_pos++;
					if(rx_buf_pos==16)
						too_long=1;		// 
				}
				
			}
			else
			{
				if(!too_long)	// If too long, abandon
					rx_buff2[rx_buf_pos] = 0;
				rx_FULL=1;
			}
		}
	NVIC_EnableIRQ(UART0_IRQn);
	}
}



	
