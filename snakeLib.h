#ifndef SNAKELIB_H_INCLUDED
#define SNAKELIB_H_INCLUDED

extern uint8_t didSnakeCrash(int8_t snakeheadPos,int8_t snakeNewheadPos);
extern int8_t snakeMove(uint8_t dir);
extern void snakeInit();
extern void snakeDisplay(uint8_t matrixNum);
extern uint8_t snakeHavingSnack(int8_t snakeNewheadPos);
extern void makeSnack();
extern void blick();


#endif // SNAKELIB_H_INCLUDED
