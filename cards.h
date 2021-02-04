#include <stdio.h>
#include "tpm.h"
#include "MKL05Z4.h"
#include <stdlib.h>
#include "lcd1602.h"
#include "uart0.h"
#include "main.h"
#define BUTTON_1_POS 6
#define BUTTON_2_POS 7

typedef struct{
    int value;
    char hand[16];
    int P_n_of_cards;
    int P_has_ace; //boolean
		int bet;
		char name[4];
}Player;


typedef struct{
    int n_of_cards;
    char deck_letters[52];
    int deck_numbers[52];
    Player Player1;
    Player Player2;
    Player Dealer;
}Game;

int roll_card_number(int n);
void cleanup_deck(Game* Game, int card_number);
void roll_value(Game* Game, Player* Player,int card_number);
void roll_card(Game* Game,Player* Player);
void Player_turn(Game* Game,Player* Player);
void win_or_lose(Game* Game,Player* Player);
