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

#define INDEX_META(x,y)			(int)(((y)*metaWidth)+(x))
#define INDEX_SOURCE(x,y)		(((((int)(y*sizeY))*sourceWidth)+((int)(x*sizeX)))*sourceDepth);
//#define INDEX_SOURCE(x,y)		(((((int)((y+0.5)*sizeY))*sourceWidth)+((int)((x+0.5)*sizeX)))*sourceDepth);

#define CAMERA_WIDTH			640
#define CAMERA_HEIGHT			480


class metaSource {
	
public:
	
	metaSource();
	
	// init
	void resize(int w, int h);
	bool open(int src);
	bool openFile(const char *fileName);
	bool openCamera(const char *camName);
	// update
	bool update();
	// play
	void play();
	void pause();
	void playPause();
	ofColor getColor(int x, int y);
	
	// metaled
	int					metaWidth;
	int					metaHeight;
	float				metaAspectRatio;
	ofColor				*metaPixels;
	// source > meta scale
	float				sizeX;
	float				sizeY;
	
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
	
	// Playhead
	bool				bIsPaused;
	bool				bNewFrame;
	int					currentFrame;			// first = 1, ...

protected:

	void newSource();
	
};


