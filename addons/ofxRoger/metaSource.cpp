/*
 *  metaSource.cpp
 *  metaled
 *
 *  Created by Roger on 06/06/10.
 *  Copyright 2010 Studio Avante. All rights reserved.
 *
 */

#include "metaSource.h"
#include "globals.h"

metaSource::metaSource()
{
	sourceType = SOURCE_NONE;
	bLoaded = false;
	metaPixels = NULL;
	metaWidth = 0;
	metaHeight = 0;
	metaAspectRatio = 1.0;
}

///////////////////////////////////////////////////////////
//
// INIT
//

//-----------------------------------------------------
void metaSource::resize(int w, int h)
{
	metaWidth = w;
	metaHeight = h;
	metaAspectRatio = ((float)metaWidth / (float)metaHeight);
	metaPixels = (ofColor*) realloc(metaPixels, (sizeof(ofColor)*w*h));
	// Resize leds
	sizeX = ((float)sourceWidth / (float)metaWidth);
	sizeY = ((float)sourceHeight / (float)metaHeight);
	// reload image
	if (sourceType == SOURCE_IMAGE)
		currentFrame = 0;
}

//-----------------------------------------------------
bool metaSource::open(int src)
{
	if (src == SOURCE_IMAGE)
		return this->openFile(FILE_SOURCE_IMAGE);
	else if (src == SOURCE_VIDEO)
		return this->openFile(FILE_SOURCE_VIDEO);
	else if (src == SOURCE_CAMERA)
		return this->openCamera("iSight");
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
	printf("metaSource:: CAMERA ON!!\n");
	return true;
}

//-----------------------------------------------------
void metaSource::newSource()
{
	bLoaded = true;
	bNewFrame = false;
	bIsPaused = false;
	currentFrame = 0;
	// Resize leds
	sizeX = ((float)sourceWidth / (float)metaWidth);
	sizeY = ((float)sourceHeight / (float)metaHeight);
}

///////////////////////////////////////////////////////////
//
// UPDATE
//

//-----------------------------------------------------
bool metaSource::update()
{
	if (bLoaded == false || bIsPaused == true)
		return false;
	
	/// Load new frame
	switch (sourceType) {
		case SOURCE_CAMERA:
			sourceCamera.grabFrame();
			bNewFrame = sourceCamera.isFrameNew();
			break;
		case SOURCE_VIDEO:
			sourceVideo.idleMovie();
			bNewFrame = sourceVideo.isFrameNew();
			break;
		case SOURCE_IMAGE:
			bNewFrame = (currentFrame == 0 ? true : false);
			break;
		default:
			bNewFrame = false;
			break;
	}
	
	// Update metaPixels
	if (bNewFrame)
	{
		// Increase Frame count
		currentFrame++;
		// update metaPixels
		for (int x = 0 ; x < metaWidth ; x++)
		{
			for (int y = 0 ; y < metaHeight ; y++)
			{
				int ixMeta = INDEX_META(x,y);
				int ixSource = INDEX_SOURCE(x,y);
				if (sourceDepth == 1)
					metaPixels[ixMeta].set(sourcePixels[ixSource]);		// Grayscale
				else
					metaPixels[ixMeta].set(sourcePixels[ixSource],sourcePixels[ixSource+1],sourcePixels[ixSource+2]);	// Color
			}
		}
	}
	//printf("FRAME new[%d]\n",bNewFrame);
	
	return bNewFrame;
}



///////////////////////////////////////////////////////////
//
// PLAY
//

//-----------------------------------------------------
ofColor metaSource::getColor(int x, int y)
{
	return metaPixels[INDEX_META(x,y)];
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









