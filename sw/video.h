/*
 * video.h
 *
 *  Created on: Apr 17, 2020
 *      Author: VIPIN
 */

#ifndef SRC_VIDEO_H_
#define SRC_VIDEO_H_

#include "xil_types.h"
#include "xscugic.h"
#include "xaxivdma.h"

//These are hardware dependent definitions. Do not modify unless you modify it in hardware
#define HSize 1920
#define VSize 1080
#define FrameSize HSize*VSize*3


int initVideo(u32 deviceId,char *Buffer,XScuGic* Intc);
int SetupVideoIntrSystem(XAxiVdma *AxiVdmaPtr, u16 ReadIntrId, XScuGic *Intc);

#endif /* SRC_VIDEO_H_ */

