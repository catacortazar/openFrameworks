/*
 *  metaSource.h
 *  metaled
 *
 *  Created by Roger on 06/06/10.
 *  Copyright 2010 Studio Avante. All rights reserved.
 *
 */
#pragma once

#include "ofMain.h"
#include "globals.h"

#define INDEX_SOURCE(x,y)		(((((int)(y*sizeY))*sourceWidth)+((int)(x*sizeX)))*sourceDepth);
//#define INDEX_SOURCE(x,y)		(((((int)((y+0.5)*sizeY))*sourceWidth)+((int)((x+0.5)*sizeX)))*sourceDepth);

#define CAMERA_WIDTH			640
#define CAMERA_HEIGHT			480

///////////////////////////////////////////////////////
//
// A LED
//
class Led {
public:
	
	Led() {};
	//void update();
	void draw(int shape);
	
	ofColor		color;
	ofPoint		pos;
	float		size;
	float		bright;
	
protected:
	
};


///////////////////////////////////////////////////////
//
// METASOURCE
//
class metaSource {
public:
	
	metaSource();
	
	// init
	void resize(int w, int h, int d=1);
	bool openFile(const char *fileName);
	bool openCamera(const char *camName);
	// playhead
	void play();
	void pause();
	void playPause();
	// Getters
	ofColor getColor(int x, int y, int z=0);
	ofPoint getPos(int x, int y, int z=0);
	float getSize(int x, int y, int z=0);
	// update
	bool update(float ledScale);
	// draw
	void draw(int shape);
	
	// metaled
	int					width;
	int					height;
	int					depth;
	float				aspectRatio;
	// source > meta scale
	float				sizeX;
	float				sizeY;

	// Playhead
	bool				bIsPaused;
	bool				bNewFrame;

protected:

	// playhead
	void newSource();
	bool getFrame();

	// sources
	ofImage				sourceImage;
	ofVideoPlayer 		sourceVideo;
	ofVideoGrabber 		sourceCamera;
	int					sourceType;
	int					sourceDepth;			// bytes per pixel
	int					sourceWidth;
	int					sourceHeight;
	char				sourceName[256];
	bool				bLoaded;
	unsigned char		*sourcePixels;
	
	// Current frame
	Led					leds[GRID_X_MAX][GRID_Y_MAX][GRID_Z_MAX];


};


