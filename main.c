#include "MKL05Z4.h"

#include "uart0.h"

#include <stdio.h>

#include <string.h>

#include <stdlib.h>

#include "main.h"

#include "lcd1602.h"

#include <time.h>

#include "tpm.h"

#include "cards.h"

#define BUTTON_1_POS 6
#define BUTTON_2_POS 7

#define RED_LED_POS 8

char rx_buf_welcome[] = "\r Welcome to the game of blackjack! \r";


int D_turn = 1;
char str_staly[2];
char roll_numb[] = {
  "\r Rolled card number: \r"
};
uint8_t rx_buf_pos = 0;
char Too_Long[] = "Too long input.";
uint8_t rx_FULL = 1;

uint8_t too_long = 0;
uint8_t wfi = 1;
char temp;
char rx_buff2[16];

void delay_ms(int n) {
  volatile int k;
  volatile int j;
  for (k = 0; k < n; k++)
    for (j = 0; j < 3500; j++) {}
}

int main(void) {

  TPM0_Init_PWM();
  SIM -> SCGC5 |= SIM_SCGC5_PORTB_MASK;
  PORTB -> PCR[RED_LED_POS] |= PORT_PCR_MUX(1);
  PTB -> PDDR |= (1 << RED_LED_POS);

  UART0_Init();
  LCD1602_Init();

  SIM -> SCGC5 |= SIM_SCGC5_PORTB_MASK;
  PORTB -> PCR[BUTTON_1_POS] |= PORT_PCR_MUX(1);
  PORTB -> PCR[BUTTON_2_POS] |= PORT_PCR_MUX(1);


  PORTB -> PCR[BUTTON_1_POS] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
  PORTB -> PCR[BUTTON_2_POS] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;


  while (1) { // main loop
		
    Game Game = {
		52,
		{'2','2','2','2','3','3','3','3','4','4','4','4','5','5','5','5','6','6','6','6','7','7','7','7','8','8','8','8','9','9','9','9','1','1','1','1','J','J','J','J','Q','Q','Q','Q','K','K','K','K','A','A','A','A'},
		{2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,6,6,6,6,7,7,7,7,8,8,8,8,9,9,9,9,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,11,11,11,11},
		{0,"",0,0,0,{'1'}},
		{0,"",0,0,0,{'2'}},
		{0,"",0,0,0,{'D'}}
	};

 D_turn = 1;

    Uart_print(rx_buf_welcome);


    char rx_buf2[] = {
      "\r Please specify Player 1's bet\r"
    };

    Uart_print(rx_buf2);

    rx_FULL = 0; // allow interrupt 
    Uart_getdata();
    Game.Player1.bet = atoi(rx_buff2);

    Uart_print(rx_buff2);
    char rx_buf3[] = {
      "\r Please specify Player 2's bet\r"
    };
    Uart_print(rx_buf3);
    rx_FULL = 0; // allow interrupt 
    Uart_getdata();
    Game.Player2.bet = atoi(rx_buff2);

    Uart_print(rx_buff2);

    roll_card( & Game, & Game.Dealer);
    roll_card( & Game, & Game.Player1);
    roll_card( & Game, & Game.Player1);
    roll_card( & Game, & Game.Player2);
    roll_card( & Game, & Game.Player2);

    char rx_buf6[] = {
      "\r P2 value:\r"
    };

    LCD1602_SetCursor(0, 1);
    LCD1602_Print(Game.Player2.hand);

    char rx_buf5[] = {
      "\r Dealer value:\r"
    };
    char rx_buf4[] = {
      "\r Dealer cards: \r"
    };

    Uart_print(rx_buf4); //Dealer hand
    Uart_print(Game.Dealer.hand);

    Uart_print(rx_buf5); //Dealer values
    char str2[2];
    sprintf(str2, "%d", Game.Dealer.value);
    Uart_print(str2);

    char rx_buf7[] = {
      "\r P1 value: \r"
    };

    LCD1602_SetCursor(0, 0);
    LCD1602_Print(Game.Player1.hand);

    Uart_print(rx_buf7); //P1 values
    char str5[2];
    sprintf(str5, "%d", Game.Player1.value);
    Uart_print(str5);

    Uart_print(rx_buf6); //P1 values
    char str6[2];
    sprintf(str6, "%d", Game.Player2.value);
    Uart_print(str6);
		
    Player_turn( & Game, & Game.Player1);

		Player_turn( & Game, & Game.Player2);

    delay_ms(1046);
    while (D_turn) {
      roll_card( & Game, & Game.Dealer);
      if (Game.Dealer.value >= 17) {
        D_turn = 0;
      }
			if (Game.Dealer.value > 21 && Game.Dealer.P_has_ace==1) {
					Game.Dealer.value=Game.Dealer.value-10;
			}				
    }
		
    Uart_print(rx_buf4); //Dealer hand
    Uart_print(Game.Dealer.hand);

    Uart_print(rx_buf5); //Dealer values
    char str_d_k[2];
    sprintf(str_d_k, "%d", Game.Dealer.value);
    Uart_print(str_d_k);

		win_or_lose( & Game, & Game.Player1);
		win_or_lose( & Game, & Game.Player2);
		
    delay_ms(10465);
    LCD1602_ClearAll();
    //NVIC_SystemReset();

  } // main while loop
} // main
