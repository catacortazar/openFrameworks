/*
 *  ofxConfigAutomator.cpp
 *
 *  Created by Roger Sodre on 08/04/10.
 *  Copyright 2010 Studio Avante. All rights reserved.
 *
 */

////////////////////////////////////////////////////////////////////////
//
// CONFIG AUTOMATOR
//
//  Config class v1.0
//  Manage configuration parameters + save as XML
//  by Roger Sodre
//
//  Install:
//
//
//  Usage:
//
//
//  TODO:
//
//

#include "ofxConfigAutomator.h"

#define VERBOSE_REC	1

#define CAPTURE_FORMAT	"/Users/Roger/Desktop/CAPTURE/capture_%06d.jpg"



ofxConfigAutomator::ofxConfigAutomator() : ofxConfig()
{
	// init rec
	isRecording = isPlaying = false;
	recFrame = 0;
	
}
ofxConfigAutomator::~ofxConfigAutomator()
{
}




/////////////////////////////////////////////////////////////////////////
//
// REC / PLAY config parameters
//
// TODO: INCORPORAR VECTORS & COLORS!!!!!!
//
// Start to record config params
void ofxConfigAutomator::recStart()
{
	if (isRecording == false)
	{
		// erase buffer
		for (int n = 0 ; n < recBuffer.size() ; n++)
			recBuffer[n].clear();
		recBuffer.clear();
		// new buffer
		isRecording = true;
		isPlaying = false;
		recFrame = 0;
		playFrame = -1;
	}
	else
	{
		this->recStop();
		if (VERBOSE_REC)
			printf("RECORDED [%d] frames\n",recFrame);
		//this->recPlay();
	}
}
// Stop recording config params
void ofxConfigAutomator::recStop()
{
	isRecording = false;
}
// Start to play config params
void ofxConfigAutomator::recPlay()
{
	// start to play if not rec
	if (isRecording == true)
		return;
	// start to play...
	isPlaying = true;
	playFrame = -1;
}
// IF RECORDING: Add corrent config params to buffer
// IF PLAYING: Get next config params from buffer
void ofxConfigAutomator::recAdd()
{
	// Save Frame
	if (isRecording == true)
	{
		// debug
		if (VERBOSE_REC)
			printf("REC FRAME [%d] rate[%.2f] secs[%.2f]\n",recFrame,ofGetFrameRate(),((recFrame+1)/ofGetFrameRate()));
		// save params
		FLOAT_VEC values;
		for (int n = 0 ; n < params.size() ; n++ )
		{
			if (params[n] == NULL)
				continue;
			values.push_back(this->get(n));
		}
		recBuffer.push_back(values);
		recFrame++;
	}
	// Play Frame
	else if (isPlaying == true)
	{
		// Retrieve params
		playFrame++;
		FLOAT_VEC values = recBuffer[playFrame];
		for (int n = 0 ; n < params.size() ; n++ )
		{
			if (params[n] == NULL)
				continue;
			this->set(n, values[n]);
		}
		// debug
		if (VERBOSE_REC)
			printf("PLAY FRAME [%d] rate[%.2f] [%.2f]%%\n",playFrame,ofGetFrameRate(),((playFrame+1)*100.0/(float)recFrame));
		// end play?
		if (playFrame >= (recFrame-1))
			isPlaying = false;
	}
}
int ofxConfigAutomator::getPlayFrame()
{
	return playFrame;
}





