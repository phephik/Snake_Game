//max7219 output example

#define F_CPU 8000000
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "max7219.h"
#include "ledmatrix7219d88.h"
#include "lcd.h"
#include "uart.h"



void startGame();
void moveWithSnake(uint8_t direction);
void gameOver();
void LCD_game_over();


uint8_t direction = 2;
uint8_t hasLost = 0;
uint8_t score = 0;
uint8_t iterator = 0;


int main(void) {

    TCCR1B |= (1 << WGM12)|(1 << CS11);  // set up timer with prescaler = 8 and CTC mode
    TCNT1 = 0;                          // initialize counter
    OCR1A = 24999;                      // initialize compare value
    TIMSK|=(1<<OCIE1A);

    sei();

    PORTD |= (1<<PD7) + (1<<PD6) + (1<<PD1) + (1<<PD0);     //pull up rezistor

    uart_init(UART_BAUD_SELECT(9600,F_CPU));

    // Initialize display
    max7219_init();
    lcd_init(LCD_DISP_ON);
	ledmatrix7219d88_init();    //inicializace displaje
	startGame();

	while(1){

	}
}

void startGame(){
    //inicialization of game var
    hasLost = 0;
    score = 0;
    LCD_playing(score);

    //snake inicialization
    snakeInit();

    while(1){
        _delay_ms(1200);
        moveWithSnake(direction);

        if(hasLost == 1){
            gameOver();
            return;
        }
    }
}

void moveWithSnake(uint8_t direction){      // input direction, output score

    switch(snakeMove(direction)){
        case 2:
            hasLost = 1;
            break;
        case 0:
            hasLost = 0;
            break;
        case 1:
            hasLost = 0;
            score++;
            LCD_playing(score);
            break;
    }
}

void LCD_game_over(){
    lcd_clrscr();
    lcd_gotoxy(0,0);
    lcd_puts("***SNAKE GAME***");
    lcd_gotoxy(0,2);
    lcd_puts("Press reset to ");
    lcd_gotoxy(0,3);
    lcd_puts("start a new game");

}

void LCD_playing(uint8_t score){
    lcd_clrscr();
    lcd_gotoxy(0,0);
    lcd_puts("***SNAKE GAME***");
    lcd_gotoxy(0,2);
    lcd_puts("Score:  ");
    lcd_gotoxy(10,2);
    char snum[5];
    itoa(score, snum, 10);
    lcd_puts(snum);
}

void LCD_gameover(uint8_t score){
    lcd_clrscr();
    lcd_gotoxy(0,0);
    lcd_puts("***GAME OVER***");
    lcd_gotoxy(0,2);
    lcd_puts("Score:  ");
    lcd_gotoxy(10,2);
    char snum[5];
    int sscore;
    itoa(score, sscore, 10);
    lcd_puts(sscore);
    lcd_gotoxy(0,3);
    lcd_puts("  Press reset");
}

ISR (TIMER1_COMPA_vect){

    if (bit_is_clear(PIND, PD7)){             // LEFT BUTTON
        direction = 3;
    }
    else if (bit_is_clear(PIND, PD6)){        // UP BUTTON
        direction = 2;
    }
    else if (bit_is_clear(PIND, PD1)){        // DOWN BUTTON
        direction = 0;
    }
    else if (bit_is_clear(PIND, PD0)){       // RIGHT BUTTON
        direction = 1;
    }
}


void gameOver(){
    LCD_gameover(score);
    for(iterator = 0;iterator<64;iterator++){
        ledmatrix7219d88_setled(0,iterator,1);
        _delay_ms(50);
    }
    ledmatrix7219d88_resetmatrix(0);
}


