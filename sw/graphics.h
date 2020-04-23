#ifndef IMGPROCESS_H_   /* prevent circular inclusions */
#define IMGPROCESS_H_

#include "xil_types.h"
#include "xil_cache.h"
#include "xscugic.h"

#define blackColor 0x000000
#define whiteColor 0xffffff
#define redColor 0x0000ff
#define greenColor 0x00ff00
#define blueColor 0xff0000
#define brownColor 0x214365


//#define getSnakeHeadPosition Xil_In16(XPAR_SNAKETRACKER_0_S00_AXI_BASEADDR+4);

//Function for copying image from one buffer to another buffer
int drawImage(u32 displayHSize,u32 displayVSize,u32 imageHSize,u32 imageVSize,u32 hOffset, u32 vOffset,int numColors,char *imagePointer,char *videoFramePointer);
int drawFrame(u32 displayHSize,u32 displayVSize,u32 gridSize,u32 frameVOffset,u32 frameHOffset,u32 width,u32 color,char* videoFramePointer);
void printChar(u32 displayHSize,u32 displayVSize,char *charBitMap,u32 hOffset, u32 vOffset,u32 zoom,u32 color,char* videoFramePointer);
void printString(u32 displayHSize,u32 displayVSize,char *printString,u32 hOffset, u32 vOffset,u32 zoom,u32 color,char* videoFramePointer);
int drawSquare(u8 xPos, u8 yPos,u32 gridSize,u32 displayHSize,char* videoFramePointer,u32 color);
//void showGameOver();
#endif /* end of protection macro */
