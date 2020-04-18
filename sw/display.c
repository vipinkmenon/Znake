/*
 * imageProcess.c
 *
 *  Created on: Apr 13, 2020
 *      Author: VIPIN
 */

#include "display.h"

#include "font.h"



/*****************************************************************************/
/**
 * This function copies the buffer data from image buffer to video buffer
 *
 * @param	displayHSize is Horizontal size of video in pixels
 * @param   displayVSize is Vertical size of video in pixels
 * @param	imageHSize is Horizontal size of image in pixels
 * @param   imageVSize is Vertical size of image in pixels
 * @param   hOffset is horizontal position in the video frame where image should be displayed
 * @param   vOffset is vertical position in the video frame where image should be displayed
 * @param   imagePointer pointer to the image buffer
 * @return
 * 		-  0 if successfully copied
 * 		- -1 if copying failed
 *****************************************************************************/
int drawImage(u32 displayHSize,u32 displayVSize,u32 imageHSize,u32 imageVSize,u32 hOffset, u32 vOffset,int numColors,char *imagePointer,char *videoFramePointer){
	Xil_DCacheInvalidateRange((u32)imagePointer,(imageHSize*imageVSize));
	for(int i=0;i<displayVSize;i++){
		for(int j=0;j<displayHSize;j++){
			if(i<vOffset || i >= vOffset+imageVSize){
				videoFramePointer[(i*displayHSize*3)+(j*3)]   = 0x00;
				videoFramePointer[(i*displayHSize*3)+(j*3)+1] = 0x00;
				videoFramePointer[(i*displayHSize*3)+(j*3)+2] = 0x00;
			}
			else if(j<hOffset || j >= hOffset+imageHSize){
				videoFramePointer[(i*displayHSize*3)+(j*3)]   = 0x00;
				videoFramePointer[(i*displayHSize*3)+(j*3)+1] = 0x00;
				videoFramePointer[(i*displayHSize*3)+(j*3)+2] = 0x00;
			}
			else {
				if(numColors==1){
					videoFramePointer[(i*displayHSize*3)+j*3]     = *imagePointer/16;
					videoFramePointer[(i*displayHSize*3)+(j*3)+1] = *imagePointer/16;
					videoFramePointer[(i*displayHSize*3)+(j*3)+2] = *imagePointer/16;
					imagePointer++;
				}
				else if(numColors==3){
					videoFramePointer[(i*displayHSize*3)+j*3]     = *imagePointer/16;
					videoFramePointer[(i*displayHSize*3)+(j*3)+1] = *(imagePointer++)/16;
					videoFramePointer[(i*displayHSize*3)+(j*3)+2] = *(imagePointer++)/16;
					imagePointer++;
				}
			}
		}
	}
	Xil_DCacheFlush();
	return 0;
}


int drawFrame(u32 displayHSize,u32 displayVSize,u32 gridSize,u32 frameVOffset,u32 frameHOffset,u32 framewidth,u32 color,char* videoFramePointer){
	//Top line
	u32 vOffset = frameVOffset*gridSize;
	u32 hOffset = frameHOffset*gridSize;
	u32 width = framewidth*gridSize;
	for(int i=vOffset;i<vOffset+width;i++){
		for(int j=hOffset;j<displayHSize-hOffset;j++){
			videoFramePointer[(i*displayHSize*3)+(j*3)]   = color&0xff;
			videoFramePointer[(i*displayHSize*3)+(j*3)+1] = (color&0x00ff00)>>8;
			videoFramePointer[(i*displayHSize*3)+(j*3)+2] = (color&0xff0000)>>16;
		}
	}
   //Bottom line
	for(int i=(displayVSize-vOffset-width);i<displayVSize-vOffset;i++){
		for(int j=hOffset;j<displayHSize-hOffset;j++){
			videoFramePointer[(i*displayHSize*3)+(j*3)]   = color&0xff;
			videoFramePointer[(i*displayHSize*3)+(j*3)+1] = (color&0x00ff00)>>8;
			videoFramePointer[(i*displayHSize*3)+(j*3)+2] = (color&0xff0000)>>16;
		}
	}
   //Left line
	for(int i=width+vOffset;i<displayVSize-width-vOffset;i++){
		for(int j=hOffset;j<hOffset+width;j++){
			videoFramePointer[(i*displayHSize*3)+(j*3)]   = color&0xff;
			videoFramePointer[(i*displayHSize*3)+(j*3)+1] = (color&0x00ff00)>>8;
			videoFramePointer[(i*displayHSize*3)+(j*3)+2] = (color&0xff0000)>>16;
		}
	}
	//Right line
	for(int i=width+vOffset;i<displayVSize-width-vOffset;i++){
		for(int j=displayHSize-width-hOffset;j<displayHSize-hOffset;j++){
			videoFramePointer[(i*displayHSize*3)+(j*3)]   = color&0xff;
			videoFramePointer[(i*displayHSize*3)+(j*3)+1] = (color&0x00ff00)>>8;
			videoFramePointer[(i*displayHSize*3)+(j*3)+2] = (color&0xff0000)>>16;
		}
	}
	Xil_DCacheFlush();
	return 0;
}

/*int printChar(u32 displayHSize,u32 displayVSize,char *charBitMap,u32 hOffset, u32 vOffset,u32 zoom,char* videoFramePointer ){
	for(int i=0;i<8*zoom;i++){
		for(int j=0;j<8*zoom;j++){
			videoFramePointer[((i+vOffset)*displayHSize*3)+((j+hOffset)*3)]   = *(charBitMap+(i/zoom)*8+(j/zoom));
			videoFramePointer[((i+vOffset)*displayHSize*3)+((j+hOffset)*3)+1] = *(charBitMap+(i/zoom)*8+(j/zoom));
			videoFramePointer[((i+vOffset)*displayHSize*3)+((j+hOffset)*3)+2] = *(charBitMap+(i/zoom)*8+(j/zoom));
			//xil_printf("%0x,%0x,%d,%d\n\r",*(charBitMap+(i/zoom)*(zoom*8)+(j/zoom)),(charBitMap+(i/zoom)*(zoom*8)+(j/zoom)),i,j);
		}
	}
	Xil_DCacheFlush();
	return 0;
}*/


void printString(u32 displayHSize,u32 displayVSize,char *printString,u32 hOffset, u32 vOffset,u32 zoom,u32 color,char* videoFramePointer){
	u32 charPos = hOffset;
	u32 charArrayPos;
	while(*printString != 0){
		charArrayPos = (*printString-32)*8;
		printChar(displayHSize,displayVSize,(char *)&fontBitMat[charArrayPos],charPos,vOffset,zoom,color,videoFramePointer);
		charPos += zoom*8;
		printString++;
	}
}


void printChar(u32 displayHSize,u32 displayVSize,char *charBitMap,u32 hOffset, u32 vOffset,u32 zoom,u32 color,char* videoFramePointer ){
	for(int i=0;i<8*zoom;i++){ //columnwise
		for(int j=0;j<8*zoom;j++){//rowwise
			videoFramePointer[((j+vOffset)*displayHSize*3)+((i+hOffset)*3)]   = ((*(charBitMap+(i/zoom))>>(j/zoom))&0x1)*(color&0xff);
			videoFramePointer[((j+vOffset)*displayHSize*3)+((i+hOffset)*3)+1] = ((*(charBitMap+(i/zoom))>>(j/zoom))&0x1)*(color&0x00ff00)>>8;
			videoFramePointer[((j+vOffset)*displayHSize*3)+((i+hOffset)*3)+2] = ((*(charBitMap+(i/zoom))>>(j/zoom))&0x1)*(color&0xff0000)>>16;
			//xil_printf("%d. i=%d j=%d %0d %0x %0x \n\r",i*8+j,i,j,((*(charBitMap+i))&0x01)>>j,*(charBitMap+i),(*(charBitMap+i)>>j)&0x1);
		}
	}
	Xil_DCacheFlush();
}




int drawSquare(u8 xPos, u8 yPos,u32 gridSize,u32 displayHSize,char* videoFramePointer,u32 color){
	for(int i=1;i<gridSize-1;i++){
		for(int j=1;j<gridSize-1;j++){
			videoFramePointer[((i+yPos*gridSize)*displayHSize*3)+((j+xPos*gridSize)*3)]   = color&0xff;
			videoFramePointer[((i+yPos*gridSize)*displayHSize*3)+((j+xPos*gridSize)*3)+1] = (color&0x00ff00)>>8;;
			videoFramePointer[((i+yPos*gridSize)*displayHSize*3)+((j+xPos*gridSize)*3)+2] = (color&0xff0000)>>16;;
			//xil_printf("%0x,%0x,%d,%d\n\r",*(charBitMap+(i/zoom)*(zoom*8)+(j/zoom)),(charBitMap+(i/zoom)*(zoom*8)+(j/zoom)),i,j);
		}
	}
	Xil_DCacheFlush();
	return 0;
}




