/*
 *  ofxMacam.cpp
 *	Access fake macam driver shared memory
 *
 *  Created by Roger on 18/08/10.
 *  Copyright 2010 Studio Avante. All rights reserved.
 *
 */

#include "ofxMacam.h"

ofxMacam::ofxMacam()
{
	// Alloc local image buffer
	imgBuffer.allocate(SHM_IMAGE_WIDTH, SHM_IMAGE_HEIGHT, OF_IMAGE_COLOR);

	// Verify shm
	shm = new SharedImageClient();
	if (shm == NULL)
		printf("SHARED MEM: shm NULL!!!\n");
	else if (shm->getStatus() != SHM_OK)
		printf("SHARED MEM: shm error [%s]\n",shm->getStatusDesc());
}
ofxMacam::~ofxMacam()
{
	free(shm);
}

////////////////////////////////////////
//
// DRAW
//

//
// Fill shred memory with color
void ofxMacam::drawColor(ofColor c, bool force)
{
	// Fill buffer
	imgBuffer.fill(c);
	// write to shm
	shm->write(imgBuffer.getPixels());
}

//
// Send current screen to shared memory
void ofxMacam::drawScreen(bool force)
{
	// Fill buffer
	imgBuffer.grabScreen(0, 0, SHM_IMAGE_WIDTH, SHM_IMAGE_HEIGHT);
	// write to shm
	shm->write(imgBuffer.getPixels());
}



