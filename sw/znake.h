/*
 * znake.h
 *
 *  Created on: Apr 17, 2020
 *      Author: VIPIN
 */

#ifndef SRC_ZNAKE_H_
#define SRC_ZNAKE_H_

#include "display.h"
#include "video.h"

//These are hardware dependent definitions. Do not modify unless you modify it in hardware
#define clockFreq 100000000


//#define whiteMode

#define snakeSpeed 0.5 //Initial speed of snake in seconds
#define initDelayValue snakeSpeed*clockFreq

#define gridSize 40
#define borderWidth 1  //Thickness of frame margin now onwards everything in terms of number of gridSize
#define frameHSize 20
#define frameVSize 20
#define frameVOffset ((VSize/gridSize)-2*borderWidth-frameVSize)/2
#define frameHOffset ((HSize/gridSize)-2*borderWidth-frameHSize)/2

#define blackColor 0x000000
#define whiteColor 0xffffff
#define redColor 0x0000ff
#define greenColor 0x00ff00
#define blueColor 0xff0000
#define brownColor 0x214365
#define frameColor brownColor


#define ScorePos HSize-400

#ifdef whiteMode
	#define snakeColor blackColor
#else
	#define snakeColor whiteColor
#endif
	#define preyColor greenColor

typedef struct{
	char *Buffer;
	u32 BaseAddress;
    u32 EventTimerBaseAddress;
    u32 GPIOBaseAddress;
}znake;

int initZnake(znake* myZnake, u32 snakeTrackBaseAddress,u32 timerBaseAddress, u32 gpioBaseAddress,char* videoBuffer);
void showGameOver(znake *myZnake);
void showWelcome(znake *myZnake);
void updateScore(znake *myZnake,int score);
void drawPrey(znake *myZnake,u8 *preyX, u8 *preyY);
int checkGameOver(u8 xPos, u8 yPos);

void checkPause(znake *myZnake);
void updateSnakeSize(znake *myZnake,u32 snakeSize);
void updateSnakePosition(znake *myZnake, u16 pos);
void resetSnakeTracker(znake *myZnake);
void restartSnakeTracker(znake *myZnake);
void startEventTimer(znake *myZnake);
void stopEventTimer(znake *myZnake);
void waitRestart(znake *myZnake);
int checkPreyInBody(znake *myZnake);
u16 getSnakeSegment(znake *myZnake);
void loadTimer(znake *myZnake,u32 timerValue);
int headHitBody(znake *myZnake);
void clearPreyInBody(znake *myZnake);
#endif /* SRC_ZNAKE_H_ */
