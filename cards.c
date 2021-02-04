#include "cards.h"




int roll_card_number(int n){
    srand((uint16_t)TPM0->CNT);
		
		int card_n=(int)rand() % n;

    return card_n;
}

void cleanup_deck(Game* Game, int card_number){
    for(int i=0;i+card_number < Game->n_of_cards;i++){
        Game->deck_letters[card_number+i]=Game->deck_letters[card_number+1+i];
        Game->deck_numbers[card_number+i]=Game->deck_numbers[card_number+1+i];
    }
}

void Player_turn(Game* Game, Player* Player){

	int P_turn=1;
	while (P_turn) {
      if ((PTB -> PDIR & (1 << BUTTON_1_POS)) == 0) {
        /* Test if button S1  HIT PLAYER 1*/

        roll_card( Game, Player);

        
        
        char rx_buf_P_hit[27];
				sprintf(rx_buf_P_hit, "\r Player %s HIT ! Value: \r" ,Player->name);
        Uart_print(rx_buf_P_hit); //P1 values

        if (Player->value >= 21) {
					if(Player->P_has_ace==1){
						Player->value=Player->value-10;
						Player->P_has_ace=0;
					}
					else{
						P_turn = 0;
						char rx_buf_P_r_over[27];
						sprintf(rx_buf_P_r_over, "\r Player %s round over: \r" ,Player->name);
						Uart_print(rx_buf_P_r_over); //P1 values
					}
        }
				
        char temp_str[2];
        sprintf(temp_str, "%d", Player->value);
        Uart_print(temp_str);
				
        while ((PTB -> PDIR & (1 << BUTTON_1_POS)) == 0) /* Wait for release */
          delay_ms(100); /* Debouncing */
				LCD1602_SetCursor(0,0);
				LCD1602_Print(Game->Player1.hand);
				LCD1602_SetCursor(0,1);
				LCD1602_Print(Game->Player2.hand);

      } else if ((PTB -> PDIR & (1 << BUTTON_2_POS)) == 0) {
					/* Test if button S5 STAND PLAYER 1 -> FINISH TURN*/
					P_turn = 0;
					char rx_buf_P_r_over[27];
					sprintf(rx_buf_P_r_over, "\r Player %s round over: \r" ,Player->name);
					Uart_print(rx_buf_P_r_over); //P1 values
					while ((PTB -> PDIR & (1 << BUTTON_2_POS)) == 0) /* Wait for release */
          delay_ms(100);
      }
    }
	
}

void win_or_lose(Game* Game, Player* Player){

    if (Player->value > 21) {
			char P_lose[20];
			sprintf(P_lose, "\r Player %s lose %d \r" ,Player->name,Player->bet);
      Uart_print(P_lose);
      //P1 loses
    }
    if (Player->value == 21) {
      //P1 BLACKJACK
			char P[32];
			sprintf(P, "\r Player %s BLACKJACK!!! Win: %d \r" ,Player->name,Player->bet*2);
      Uart_print(P);
    }
    if (Player->value == Game->Dealer.value && Player->value <= 21) {
      //Player 1 Tie
			char P[35];
			sprintf(P, "\r Player %s tie. No loss of bet: %d \r" ,Player->name, Player->bet);
      Uart_print(P);
    }
    if (Player->value > Game->Dealer.value && Game->Dealer.value <= 21 && Player->value < 21) {
      //Player 1 wins
			char P[20];
			sprintf(P, "\r Player %s WINS: %d \r" ,Player->name, Player->bet*2);
      Uart_print(P);
    }
    if (Player->value < Game->Dealer.value && Game->Dealer.value <= 21 && Player->value < 21) {
			//Player 1 loses
			char P_lose[20];
			sprintf(P_lose, "\r Player %s lose %d \r" ,Player->name,Player->bet);
      Uart_print(P_lose);
    }
    if (Player->value < Game->Dealer.value && Game->Dealer.value > 21 && Player->value < 21) {
      //Player 1 wins
			char P[20];
			sprintf(P, "\r Player %s WINS: %d \r" ,Player->name, Player->bet*2);
      Uart_print(P);
    }
}

void roll_value(Game* Game, Player* Player,int card_number){
    if(Game->deck_numbers[card_number]==11){
        if(Player->value+Game->deck_numbers[card_number]>21){
            Player->value++;
        }
        else{
            Player->value=Player->value+Game->deck_numbers[card_number];
            Player->P_has_ace=1;
        }
    }
        else{
            Player->value=Player->value+Game->deck_numbers[card_number];
        }
}

void roll_card(Game* Game,Player* Player){
    int card_number=roll_card_number(Game->n_of_cards);
    roll_value(Game, Player, card_number);
    Player->hand[Player->P_n_of_cards]=Game->deck_letters[card_number];
    cleanup_deck(Game,card_number);
    Game->n_of_cards--;
    Player->P_n_of_cards++;
}
