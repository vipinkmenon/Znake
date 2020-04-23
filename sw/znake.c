/*
 * znake.c
 *
 *  Created on: Apr 17, 2020
 *      Author: VIPIN
 */


#include "znake.h"
#include <stdio.h>
#include "sleep.h"
#include <stdlib.h>

int initZnake(znake* myZnake, u32 snakeTrackBaseAddress,u32 timerBaseAddress, u32 gpioBaseAddress,char* videoBuffer){
	myZnake->Buffer = videoBuffer;
	myZnake->BaseAddress = snakeTrackBaseAddress;
	myZnake->EventTimerBaseAddress = timerBaseAddress;
	myZnake->GPIOBaseAddress = gpioBaseAddress;
	return 0;
}

int initIntrController(u32 deviceId, XScuGic *IntcInstancePtr){
	int Status;
	XScuGic_Config *IntcConfig;
	IntcConfig = XScuGic_LookupConfig(deviceId);
	Status =  XScuGic_CfgInitialize(IntcInstancePtr, IntcConfig, IntcConfig->CpuBaseAddress);
	if(Status != XST_SUCCESS){
		xil_printf("Interrupt controller initialization failed..");
		return -1;
	}

	Xil_ExceptionInit();
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,(Xil_ExceptionHandler)XScuGic_InterruptHandler,(void *)IntcInstancePtr);
	Xil_ExceptionEnable();

	return XST_SUCCESS;
}


void showGameOver(znake *myZnake){
	printString(HSize,VSize,(char *)"Game Over!!",80,VSize/3,20,redColor,myZnake->Buffer);
	printString(HSize,VSize,(char *)"Press Centre Button to Continue..",30,VSize/2+30,7,redColor,myZnake->Buffer);
}

void showWelcome(znake *myZnake){
	char count[10];
	#ifdef whiteMode
		memset(myZnake->Buffer,whiteColor,FrameSize);
	#else
		memset(myZnake->Buffer,blackColor,FrameSize);
	#endif
	char stringSize = 5;
	char zoom=20;
	printString(HSize,VSize,(char *)"Znake",(HSize-stringSize*8*zoom)/2,VSize/3,zoom,redColor,myZnake->Buffer);
	for(int i=3;i>0;i--){
		sprintf(count, "%d",i);
		printString(HSize,VSize,(char *)count,(HSize-8*zoom)/2,2*VSize/3,zoom,redColor,myZnake->Buffer);
		sleep(1);
	}
	#ifdef whiteMode
		memset(myZnake->Buffer,whiteColor,FrameSize);
	#else
		memset(myZnake->Buffer,blackColor,FrameSize);
	#endif
}


void updateScore(znake *myZnake,int score){
	char scoreString[10];
	sprintf(scoreString, "%d", score);
	printString(HSize,VSize,(char *)scoreString,ScorePos+(6*8*5),10,5,whiteColor,myZnake->Buffer);
}

int checkGameOver(u8 xPos, u8 yPos){
	//xil_printf("head x %0d head y %0d  %0d %d %d %d",xPos,yPos,frameHSize+borderWidth+frameHOffset,borderWidth+frameHOffset,frameVSize+borderWidth+frameVOffset,borderWidth+frameVOffset);
	if(xPos >= (frameHSize+borderWidth+frameHOffset) || xPos < (borderWidth+frameHOffset)){
		return 1;
	}
	if(yPos > (frameVSize+borderWidth+frameVOffset) || yPos < (borderWidth+frameVOffset)){
		return 1;
	}
	return 0;
}

void drawPrey(znake *myZnake,u8 *preyX,u8 *preyY ){
	u16 pos;
	srand(Xil_In32(myZnake->EventTimerBaseAddress));
	*preyX = rand()%(frameHSize)+frameHOffset+borderWidth;
	*preyY = rand()%(frameVSize)+frameVOffset+borderWidth;
	pos = *preyX|(*preyY<<8);
	Xil_Out16(myZnake->BaseAddress+4,pos);
	drawSquare(*preyX,*preyY,gridSize,HSize,myZnake->Buffer,preyColor);
}

void waitRestart(znake *myZnake){
	while(!Xil_In32(myZnake->GPIOBaseAddress)){
	}
}

void checkPause(znake *myZnake){
	u32 status;
	status = Xil_In32(myZnake->GPIOBaseAddress);
	if(status){
		stopEventTimer(myZnake);
		while(Xil_In32(myZnake->GPIOBaseAddress));  //Wait until button is released
		startEventTimer(myZnake);
	}
}

void updateSnakeSize(znake *myZnake,u32 snakeSize){
	Xil_Out32(myZnake->BaseAddress+16,snakeSize);
}

void updateSnakePosition(znake *myZnake, u16 pos){
	Xil_Out32(myZnake->BaseAddress+12,pos);
}


void resetSnakeTracker(znake *myZnake){
	Xil_Out32(myZnake->BaseAddress,0x2);
	Xil_Out32(myZnake->BaseAddress,0x0);
}

void restartSnakeTracker(znake *myZnake){
	Xil_Out32(myZnake->BaseAddress,0x1);
	Xil_Out32(myZnake->BaseAddress,0x0);
}

void startEventTimer(znake *myZnake){
	Xil_Out32(myZnake->EventTimerBaseAddress,1);
}

void stopEventTimer(znake *myZnake){
	Xil_Out32(myZnake->EventTimerBaseAddress,0);
}

int checkPreyInBody(znake *myZnake){
	return Xil_In32(myZnake->BaseAddress+8);
}

u16 getSnakeSegment(znake *myZnake){
	return Xil_In16(myZnake->BaseAddress+4);
}

void loadTimer(znake *myZnake,u32 timerValue){
	Xil_Out32(myZnake->EventTimerBaseAddress+4,timerValue);
}

int headHitBody(znake *myZnake){
	return Xil_In8(myZnake->BaseAddress);
}


void clearPreyInBody(znake *myZnake){
	Xil_Out32(myZnake->BaseAddress+8,0x00000000);
}
