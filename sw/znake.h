/*
 * znake.h
 *
 *  Created on: Apr 17, 2020
 *      Author: VIPIN
 */

#ifndef SRC_ZNAKE_H_
#define SRC_ZNAKE_H_

#include "graphics.h"
#include "video.h"

//These are hardware dependent definitions. Do not modify unless you modify it in hardware
#define clockFreq 100000000


//#define whiteMode

#define snakeSpeed 0.5 //Initial speed of snake in seconds
#define initDelayValue snakeSpeed*clockFreq //Calculating the initial value for the event timer
#define scoreIncrementSize 10 //After how many preys size of snake should increment
#define speedIncrementFactor 2 //At what rate speed of snake should increase after catching scoreIncrementSize preys

#define gridSize 40    //Height/Width of grid
#define borderWidth 1  //Thickness of frame margin now onwards everything in terms of number of gridSize
#define frameHSize 20  //Height of frame
#define frameVSize 20  //Width of frame
#define frameVOffset ((VSize/gridSize)-2*borderWidth-frameVSize)/2 //Keep the frame at the middle of the display
#define frameHOffset ((HSize/gridSize)-2*borderWidth-frameHSize)/2

#define frameColor brownColor

#define ScorePos HSize-400 //Position for showing score. Interms of pixels

//Color of snake
#ifdef whiteMode
	#define snakeColor blackColor
#else
	#define snakeColor whiteColor
#endif

//Color of prey
#define preyColor greenColor

//Structure representing Znake
typedef struct{
	char *Buffer;
	u32 BaseAddress;
    u32 EventTimerBaseAddress;
    u32 GPIOBaseAddress;
}znake;

int initZnake(znake* myZnake, u32 snakeTrackBaseAddress,u32 timerBaseAddress, u32 gpioBaseAddress,char* videoBuffer);
int initIntrController(u32 deviceId,XScuGic *Intc);
void showWelcome(znake *myZnake);
void showGameOver(znake *myZnake);
void updateScore(znake *myZnake,int score);
void drawPrey(znake *myZnake,u8 xPos,u8 yPos);
u16 getPreyPosition(znake *myZnake);
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
