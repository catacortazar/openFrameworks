/*
 *  metaSource.cpp
 *  metaled
 *
 *  Created by Roger on 06/06/10.
 *  Copyright 2010 Studio Avante. All rights reserved.
 *
 */

#include "metaSource.h"
#include "ofx3D.h"
#include "globals.h"

metaSource::metaSource()
{
	sourceType = SOURCE_NONE;
	bLoaded = false;
	width = height = depth = 0.0;
	aspectRatio = 1.0;
}

///////////////////////////////////////////////////////////
//
// INIT
//

//-----------------------------------------------------
void metaSource::resize(int w, int h, int d)
{
	// save size
	width = w;
	height = h;
	depth = d;
	aspectRatio = ((float)width / (float)height);

	// Source > Grid
	sizeX = ((float)sourceWidth / (float)width);
	sizeY = ((float)sourceHeight / (float)height);
}

//-----------------------------------------------------
bool metaSource::openFile(const char *fileName)
{
	bool success = false;
	
	// Image file
	if (FreeImage_GetFileType(fileName) != FIF_UNKNOWN)
	{
		success = sourceImage.loadImage(fileName);
		if (success == false)
		{
			printf("metaSource:: ERROR loading image [%s]\n",fileName);
			return false;
		}
		// set source data
		sourceType = SOURCE_IMAGE;
		sourceDepth = sourceImage.bpp/8;
		sourceWidth = sourceImage.getWidth();
		sourceHeight = sourceImage.getHeight();
		sourcePixels = sourceImage.getPixels();
		printf("metaSource:: IMAGE ON!!\n");
	}
	// Video file
	else
	{
		success = sourceVideo.loadMovie(fileName);
		if (success == false)
		{
			printf("metaSource:: ERROR loading video [%s]\n",fileName);
			return false;
		}
		sourceVideo.play();
		// save source data
		sourceType = SOURCE_VIDEO;
		sourceDepth = 3;
		sourceWidth = sourceVideo.getWidth();
		sourceHeight = sourceVideo.getHeight();
		sourcePixels = sourceVideo.getPixels();
		printf("metaSource:: VIDEO ON!!\n");
	}
	
	// Save source data
	strcpy(sourceName, fileName);
	
	/// Reset Source
	this->newSource();
	
	// Ok!!!
	return true;
}

//-----------------------------------------------------
bool metaSource::openCamera(const char *camName)
{
	bool success = false;

	// Load camera
	if (sourceCamera.isGrabbing() == false)
	{
		//sourceCamera.listDevices();
		sourceCamera.setVerbose(true);
		if (sourceCamera.setDeviceByName(camName) == false)
		{
			printf("metaSource:: CAMERA not found [%s]\n",camName);
			return false;
		}
		if (sourceCamera.initGrabber(CAMERA_WIDTH,CAMERA_HEIGHT,true,false) == false)
		{
			printf("metaSource:: CAMERA unavailable [%s]\n",camName);
			return false;
		}
	}

	// Save Source data
	sourceType = SOURCE_CAMERA;
	sourceDepth = 3;
	sourceWidth = CAMERA_WIDTH;
	sourceHeight = CAMERA_HEIGHT;
	sourcePixels = sourceCamera.getPixels();
	strcpy(sourceName, camName);

	/// Reset Source
	this->newSource();
	
	// Ok!!!
	printf("metaSource:: CAMERA [%s] ON!!\n",camName);
	return true;
}

//-----------------------------------------------------
void metaSource::newSource()
{
	bLoaded = false;
	bNewFrame = false;
	bIsPaused = false;
	// Resize leds
	sizeX = ((float)sourceWidth / (float)width);
	sizeY = ((float)sourceHeight / (float)height);
}

///////////////////////////////////////////////////////////
//
// PLAYHEAD
//

bool metaSource::getFrame()
{
	// Paused?
	if (bIsPaused == true)
		return false;
	// Load new frame
	switch (sourceType) {
		case SOURCE_IMAGE:
			bNewFrame = (bLoaded == false ? true : false);
			break;
		case SOURCE_VIDEO:
			sourceVideo.idleMovie();
			bNewFrame = sourceVideo.isFrameNew();
			break;
		case SOURCE_CAMERA:
			sourceCamera.grabFrame();
			bNewFrame = sourceCamera.isFrameNew();
			break;
		default:
			bNewFrame = false;
			break;
	}
	// Increase
	if (bNewFrame)
		bLoaded = true;
	//printf("FRAME new[%d]\n",bNewFrame);
	return bNewFrame;
}

//-----------------------------------------------------
void metaSource::play()
{
	bIsPaused = false;
	if (sourceType == SOURCE_VIDEO)
		sourceVideo.play();
}
void metaSource::pause()
{
	bIsPaused = true;
	if (sourceType == SOURCE_VIDEO)
		sourceVideo.stop();
}
void metaSource::playPause()
{
	if (bIsPaused)
		this->play();
	else
		this->pause();
}






///////////////////////////////////////////////////////////
//
// GETTERS
//

//-----------------------------------------------------
ofColor metaSource::getColor(int x, int y, int z)
{
	return leds[x][y][z].color;
}
ofPoint metaSource::getPos(int x, int y, int z)
{
	return leds[x][y][z].pos;
}
float metaSource::getSize(int x, int y, int z)
{
	return leds[x][y][z].size;
}





///////////////////////////////////////////////////////////
//
// UPDATE
//

//-----------------------------------------------------
bool metaSource::update(float ledSize)
{
	// Load new frame
	if (this->getFrame() == false)
		return false;
	
	// Update LEDS
	for (int z = 0 ; z < depth ; z++)
	{
		for (int x = 0 ; x < width ; x++)
		{
			for (int y = 0 ; y < height ; y++)
			{
				// Color
				int i = INDEX_SOURCE(x,y);
				ofColor color;
				// Grayscale
				if (sourceDepth == 1)
					color = sourcePixels[i];
				// Color
				else
					color.set(sourcePixels[i],sourcePixels[i+1],sourcePixels[i+2]);
				leds[x][y][z].color = color;
				
				// Pos
				leds[x][y][z].pos.x = (x+0.5) * ledSize;
				leds[x][y][z].pos.y = (y+0.5) * ledSize;
				leds[x][y][z].pos.z = z * ledSize;
				leds[x][y][z].pos.z -= (1.0-color.getBrightness()) * cfg->get(LED_DEPTH);
				
				// Size
				leds[x][y][z].size = color.getBrightness() * ledSize;
			}
		}
	}
	// changed
	return true;
}



///////////////////////////////////////////////////////////
//
// DRAW
//

//-----------------------------------------------------
void metaSource::draw(int shape)
{
	for (int z = 0 ; z < depth ; z++)
		for (int x = 0 ; x < width ; x++)
			for (int y = 0 ; y < height ; y++)
				leds[x][y][z].draw(shape);
}



//-----------------------------------------------------
void Led::draw(int shape)
{
	// set color
	ofSetColor(color);
	
	// set pos
	if (shape == SHAPE_CUBE)
		ofx3DCube(pos.x, pos.y, pos.z, size);
	else if (shape == SHAPE_RECT)
		ofx3DSquare(pos.x, pos.y, pos.z, size);
	else if (shape == SHAPE_POINT || shape == SHAPE_SPRITE)
		glVertex3f(pos.x, pos.y, pos.z);
}









