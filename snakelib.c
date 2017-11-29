
#include <stdio.h>
#include <avr/io.h>
#include <stdlib.h>

#include <util/delay.h>
#include "snakeLib.h"
#include "uart.h"



uint8_t snackPosition = -1;
uint8_t snakeHaveSnacklastTime = 0;
uint8_t snakeDir = 0;
uint8_t snakeBody[64];         //tvar hada
uint8_t snakeAuxBody[64];       //pomocna promenna
uint8_t snakeLength;
uint8_t i = 0;                  //snake iterator
uint8_t rndNum;

extern uint8_t comparator(uint8_t val1,uint8_t val2);

void snakeDisplay(uint8_t matrixNum){

    ledmatrix7219d88_resetmatrix(0);
    ledmatrix7219d88_setled(matrixNum,snackPosition,1);

    for(i = 0;i<snakeLength;i++){
        ledmatrix7219d88_setled(matrixNum,snakeBody[i],1);
    }
}


void snakeInit(){
    snakeLength = 3;    //pocatecni delka hada

    //pocatecni podoba hada
    snakeBody[0] = 33;
    snakeBody[1] = 25;
    snakeBody[2] = 17;

    makeSnack();
    makeSnack();

    ledmatrix7219d88_resetmatrix(0);

    snakeDisplay(0);
}


int8_t snakeMove(uint8_t dir){

    int8_t newHeadPosition;

    switch(dir){
        case 0:         //up
            newHeadPosition = snakeBody[0]-8;
            break;
        case 1:         //right
            newHeadPosition = snakeBody[0]+1;
            break;
        case 2:         //down
            newHeadPosition = snakeBody[0]+8;
            break;
        case 3:         //left
            newHeadPosition = snakeBody[0]-1;
            break;
    }


    if(didSnakeCrash(snakeBody[0],newHeadPosition)){
        return 2;
    }

    for(i=0;i<snakeLength;i++){             //copy snakeBody to snakeAuxBody
        snakeAuxBody[i]=snakeBody[i];
    }

    snakeBody[0] = newHeadPosition;         //make new snakeBody

    for(i=1;i<snakeLength;i++){
        snakeBody[i] = snakeAuxBody[i-1];
    }

    if(snakeHavingSnack(newHeadPosition)){        //TODO podminka jestli snedl jidlo
        snakeLength++;          //make snake longer after food
        snakeBody[snakeLength-1] = snakeAuxBody[snakeLength-2]; //add last piece of snake from auxBody

        snakeDisplay(0);
        return 1;
    }
    snakeDisplay(0);

    return 0;
}





uint8_t snakeHavingSnack(int8_t snakeNewheadPos){


    //if(snakeNewheadPos == snackPosition){
    if(comparator(snakeNewheadPos,snackPosition)){

        ledmatrix7219d88_setled(0,snackPosition,0);
        _delay_ms(100);
        makeSnack();
        return 1;
    }
    return 0;
}



uint8_t didSnakeCrash(int8_t snakeheadPos,int8_t snakeNewheadPos){



    //up and down boundaries
    if(snakeNewheadPos<0 || snakeNewheadPos>63){
        return 1;
    }

    //left boundary
    if((snakeheadPos%8 == 0)&&(snakeNewheadPos%8 == 7)){
        uart_puts("NARAZIL DO LEVA");
        blick();
        return 1;
    }


    //right boundary
    if((snakeheadPos%8 == 7)&&(snakeNewheadPos%8 == 0)){
        blick();
        return 1;
    }


    //snake crash to himself
    for(i=0;i<snakeLength-1;i++){
        if(snakeNewheadPos == snakeBody[i]){
            blick();
            return 1;
        }
    }
    return 0;
}

void makeSnack(){

    for(;;){
       rndNum = rand()%64;
       for(i=0;i<snakeLength;i++){
            if(rndNum==snakeBody[i]){
                rndNum = 64;
            }
       }
       if(rndNum != 64){
            break;
       }
    }
    snackPosition = rndNum;

}

void blick(){
    ledmatrix7219d88_setled(0,0,1);
    _delay_ms(100);
    ledmatrix7219d88_setled(0,0,0);
}




