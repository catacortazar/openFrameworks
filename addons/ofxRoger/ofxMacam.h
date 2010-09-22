/*
 *  ofxMacam.cpp
 *	Access fake macam driver shared memory
 *
 *  Created by Roger on 18/08/10.
 *  Copyright 2010 Studio Avante. All rights reserved.
 *
 */
#pragma once

#include "ofMain.h"
#include "SharedImageClient.h"


class ofxMacam {
public:
	
	ofxMacam();
	~ofxMacam();

	void drawColor(ofColor c, bool force=false);
	void drawScreen(bool force=false);
	
private:

	SharedImageClient	*shm;
	ofImage				imgBuffer;
	
};











