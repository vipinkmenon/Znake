/*
 * vdmaTest.c
 *
 *  Created on: Apr 9, 2020
 *      Author: VIPIN
 */
#include "xparameters.h"
#include "znake.h"
#include "video.h"

char Buffer[FrameSize]; //Video Frame
XScuGic Intc; //Interrupt Controller Instance


int main(){
	znake myZnake;
	u8 preyX;
	u8 preyY;
	u16 pos;
	u16 preyPos;
	u8 xPos;
	u8 yPos;
	u8 hxPos;
	u8 hyPos;
	u32 score;
	u32 delayValue;
	u32 snakeSize;
	initIntrController(XPAR_PS7_SCUGIC_0_DEVICE_ID,&Intc);
	initVideo(XPAR_AXI_VDMA_0_DEVICE_ID,Buffer,&Intc);
	initZnake(&myZnake,XPAR_SNAKETRACKER_0_S00_AXI_BASEADDR,XPAR_TIMER_0_S00_AXI_BASEADDR,XPAR_AXI_GPIO_0_BASEADDR,Buffer);
	while(1){
		showWelcome(&myZnake);
		score = 0;
		snakeSize = 3;
		updateSnakeSize(&myZnake,snakeSize);
		drawFrame(HSize,VSize,gridSize,frameVOffset,frameHOffset,borderWidth,frameColor,Buffer);//Draw the Frame
		printString(HSize,VSize,"Score:0",ScorePos,10,5,whiteColor,Buffer);//Show the initial Score
		delayValue = initDelayValue;
		hxPos = HSize/(2*gridSize);
		hyPos = VSize/(2*gridSize);
		pos = hxPos|(hyPos<<8);
		updateSnakePosition(&myZnake,pos); //Set the initial position of the snake as the centre of screen
		loadTimer(&myZnake,delayValue);
		startEventTimer(&myZnake); //Start the even timer
		resetSnakeTracker(&myZnake);
		preyPos=getPreyPosition(&myZnake);
		preyX = preyPos&0xff;
		preyY = (preyPos&0xff00)>>8;
		//drawPrey(&myZnake,preyX,preyY);
		while(1){
			restartSnakeTracker(&myZnake); //reset the internal read pointer
			pos = getSnakeSegment(&myZnake); //Get the snake head position
			hxPos = pos&0xff;
			hyPos = (pos&0xff00)>>8;
			if(checkGameOver(hxPos,hyPos)){//check whether head hit the wall
				break;
			}
			drawSquare(hxPos,hyPos,gridSize,HSize,Buffer,redColor); //draw the head
			for(int i=1;i<snakeSize;i++){
				pos = getSnakeSegment(&myZnake); //draw the body
				xPos = pos&0xff;
				yPos = (pos&0xff00)>>8;
				drawSquare(xPos,yPos,gridSize,HSize,Buffer,snakeColor);
			}
			if(checkPreyInBody(&myZnake)){//check whether prey in the body
				preyPos=getPreyPosition(&myZnake);
				preyX = preyPos&0xff;
				preyY = (preyPos&0xff00)>>8;
				//drawPrey(&myZnake,preyX,preyY);
				clearPreyInBody(&myZnake); //Clear the bit in the snake tracker
			}
			if(headHitBody(&myZnake)){ //Check whether head of snake hit the body
				break;
			}
			pos = getSnakeSegment(&myZnake);//Get the tail position
			xPos = pos&0xff;
			yPos = (pos&0xff00)>>8;
			if((hxPos==preyX) && (hyPos==preyY)){//Check whether snake swallowed the prey
				drawSquare(xPos,yPos,gridSize,HSize,Buffer,snakeColor);
				snakeSize++;                                     //Increase snake size
				updateSnakeSize(&myZnake,snakeSize);             //Update the snake size in snake tracker
				preyPos=getPreyPosition(&myZnake);
				preyX = preyPos&0xff;
				preyY = (preyPos&0xff00)>>8;
				//drawPrey(&myZnake,preyX,preyY);
				score++;                                         //Increment the score
				updateScore(&myZnake,score);                     //Update the score on screen
				if(score%scoreIncrementSize == 0){               //Once snake catches 10 preys double the speed
					delayValue /= speedIncrementFactor;
					loadTimer(&myZnake,delayValue);              //Load the new value to the event timer
				}
			}
			else{
				#ifdef whiteMode
					drawSquare(xPos,yPos,gridSize,HSize,Buffer,whiteColor); //remove the previous tail
				#else
					drawSquare(xPos,yPos,gridSize,HSize,Buffer,blackColor);
				#endif
			}
			pos = getSnakeSegment(&myZnake); //additional cell to solve the issue of move and button between 2 reads
			xPos = pos&0xff;
			yPos = (pos&0xff00)>>8;
			#ifdef whiteMode
				drawSquare(xPos,yPos,gridSize,HSize,Buffer,whiteColor);
			#else
				drawSquare(xPos,yPos,gridSize,HSize,Buffer,blackColor);
			#endif
			drawPrey(&myZnake,preyX,preyY);
			checkPause(&myZnake);
		}
		showGameOver(&myZnake);
		waitRestart(&myZnake);
	}
}
