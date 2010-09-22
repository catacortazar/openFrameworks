/*
 *  ofxFFGLWindow.cpp
 *
 *  Created by Daniel Berio 2010
 */

#include "ofxFFGLWindow.h"
#include "ofMain.h"


ofFFGLWindow::ofFFGLWindow() 
{	
	windowPos.set(0, 0);
	windowSize.set(0, 0);
	screenSize.set(0, 0);
	
	nFrameCount				= 0;
	windowMode				= 0;
	timeNow, timeThen, fps	= 0.0f;
	
	frameRate				= 0;
}


void ofFFGLWindow::setupOpenGL(int w, int h, int screenMode)
{	
	//ofSetupGraphicDefaults();

	windowMode = screenMode;
	windowSize.set(w, h);
}

void ofFFGLWindow::initializeWindow()
{	
}

void  ofFFGLWindow::runAppViaInfiniteLoop(ofBaseApp * appPtr)
{
	appPtr->setup();
	appPtr->update();
}



void ofFFGLWindow::update()
{	
	ofGetAppPtr()->update();
}

void ofFFGLWindow::draw()
{
	
	ofGetAppPtr()->draw();
	
	
	// FPS calculation stolen from Memos code. Thanks Memo :)

	timeNow = ofGetElapsedTimef();
	if( (timeNow - timeThen) > 0)
	{
		fps = 1.0 / (timeNow-timeThen);
		frameRate *= 0.9f;
		frameRate += 0.1f*fps;
	}
	timeThen = timeNow;
	
	// increase the overall frame count
	nFrameCount++;			
}






void ofFFGLWindow::setWindowPosition(int x, int y)
{
	windowPos.set(x, y);
}

ofPoint	ofFFGLWindow::getWindowPosition() 
{
	return windowPos;
}

int	ofFFGLWindow::getWindowMode()
{
	return windowMode;
}

float ofFFGLWindow::getFrameRate()
{
	return frameRate;
}

int	ofFFGLWindow::getFrameNum()
{
	return nFrameCount;
}

void ofFFGLWindow::setFrameRate(float targetRate)
{	
}

void ofFFGLWindow::setWindowShape(int w, int h)
{
	windowSize.set(w, h);
}

ofPoint	ofFFGLWindow::getWindowSize()
{
	return windowSize;
}

ofPoint	ofFFGLWindow::getScreenSize()
{
	return screenSize;
}

