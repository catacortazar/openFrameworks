/*
 *  ofxConfigAutomator.h
 *
 *  Created by Roger Sodre on 08/04/10.
 *  Copyright 2010 Studio Avante. All rights reserved.
 *
 */
#pragma once

#include "ofxConfig.h"


////////////////////////////////////////////////////////////////////////
//
// AUTOMATOR CLASS
//
class ofxConfigAutomator : public ofxConfig {
public:
	ofxConfigAutomator();
	~ofxConfigAutomator();
	
	//
	// REC / PLAY params
	bool isRecording;
	bool isPlaying;
	// Methods
	void recStart();
	void recAdd();
	void recStop();
	void recPlay();
	int getPlayFrame();

private:
	// REC
	vector<FLOAT_VEC> recBuffer;
	int			recFrame;
	int			playFrame;
	
};


