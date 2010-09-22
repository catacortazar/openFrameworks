/*
 *  ofxConfigGUI.cpp
 *
 *  Created by Roger Sodre on 08/04/10.
 *  Copyright 2010 Studio Avante. All rights reserved.
 *
 */

#include "ofxConfigGUI.h"
#include "ofxConfigPicker.h"
#include <sys/time.h>

//
// CUSTOM CONFIG CLASS
#ifdef CINDER
ofxConfigGUI::ofxConfigGUI(App *app, bool automatic) : ofxConfig()
#else
ofxConfigGUI::ofxConfigGUI(bool automatic) : ofxConfig()
#endif
{
	// Setup mouse events
#ifdef CINDER
	mApp = app;
	mCbMouseDown = mApp->registerMouseDown( this, &ofxConfigGUI::mousePressed );
	mCbMouseMove = mApp->registerMouseMove( this, &ofxConfigGUI::mouseMoved );	
	mCbMouseDrag = mApp->registerMouseDrag( this, &ofxConfigGUI::mouseDragged );	
	mCbMouseUp   = mApp->registerMouseUp(	this, &ofxConfigGUI::mouseReleased );	
#else
	ofAddListener(ofEvents.mousePressed,	this, &ofxConfigGUI::mousePressed);
	ofAddListener(ofEvents.mouseMoved,		this, &ofxConfigGUI::mouseMoved);
	ofAddListener(ofEvents.mouseDragged,	this, &ofxConfigGUI::mouseDragged);
	ofAddListener(ofEvents.mouseReleased,	this, &ofxConfigGUI::mouseReleased);
#endif
	
	// Automatic draw
#ifdef CFG_CATCH_LOOP_EVENTS
	bAutoDraw = automatic;
	if (bAutoDraw)
		ofAddListener(ofEvents.draw,			this, &ofxConfigGUI::draw);
#else
	bAutoDraw = false;
#endif
	
	// init
	defaultColorOut = PICKER_COLOR_OUT;
	defaultColorOver = PICKER_COLOR_OVER;
	defaultColorPicked = PICKER_COLOR_PICKED;
	bHidden = false;
	bAutoHide = false;
	bMouseDown = false;
	bMouseOver = false;
	bDrawText = true;
	matrixX = 0;
	matrixY = 0;
	matrixScale = 1.0f;
	millisLast = getMillisNow();
}
ofxConfigGUI::~ofxConfigGUI()
{
	// Release pickers
	for (int n = 0 ; n < pickers.size() ; n++ )
		delete pickers[n];
	// Setup mouse events
#ifdef CINDER
	mApp->unregisterMouseDown( mCbMouseDown );
	mApp->unregisterMouseMove( mCbMouseMove );	
	mApp->unregisterMouseDrag( mCbMouseDrag );	
	mApp->unregisterMouseUp  ( mCbMouseUp );
#endif
}

////////////////////////////////////////////////////////
//
// DRAW
//
// Draw automatically from Loop Event
#ifdef CFG_CATCH_LOOP_EVENTS
void ofxConfigGUI::draw(ofEventArgs &e)
{
	if (bAutoDraw)
		this->draw();
}
#endif
// main loop
void ofxConfigGUI::draw()
{
	//printf("mouse over[%d] down [%d]\n",bMouseOver,bMouseDown);
	// Hide / fade
	if (!bHidden)
	{
		mAlpha = 1.0;
		// fade away...
		if (bAutoHide && (!bMouseDown && !bMouseOver))
		{
			unsigned int diff = (unsigned int) (getMillisNow() - millisLast);
			// Hide!
			if ( diff >= (MILLIS_AUTO_HIDE+MILLIS_AUTO_FADE))
				bHidden = true;
			// Fade...
			else if ( diff  >= MILLIS_AUTO_HIDE)
				mAlpha -= ( (diff-MILLIS_AUTO_HIDE) / (float)MILLIS_AUTO_FADE );
			//printf("last[%u] now[%u] diff [%u] alpha[%.2f]\n",millisLast,getMillisNow(),diff,mAlpha);
		}
	}
	// no show!
	if (bHidden || mAlpha == 0.0)
		return;

	// cal virtual
	this->drawGUI();
}
// virtual
void ofxConfigGUI::drawGUI()
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// DRAW PICKERS
	for (int n = 0 ; n < pickers.size() ; n++ )
		pickers[n]->draw(mAlpha);
}
// set current matrix befor drawing
void ofxConfigGUI::setMatrix(int x, int y, float s)
{
	matrixX = x;
	matrixY = y;
	matrixScale = s;
	for (int n = 0 ; n < pickers.size() ; n++ )
		pickers[n]->updatePos();
}

////////////////////////////////////////////////////////
//
// ADD GUI ELEMENTS
//
// PICKER: The value is the coordinate on the screen.
// Set a VECTOR as picker
void ofxConfigGUI::setPicker(int id, bool clamp, int sz)
{
	if ( params[id] == NULL )
		return;
	// Create Picker
	ofxConfigPicker *picker = new ofxPickerAbsolute(this, id, clamp, sz);
	pickers.push_back(picker);
}

//
// SLIDER: Horizontal slider
void ofxConfigGUI::setSliderH(int id, int x, int y, int w, bool l, int sz, bool c)
{
	this->setSliderH(id, 0, x, y, w, l, sz, c);
}
void ofxConfigGUI::setSliderH(int id, int ix, int x, int y, int w, bool l, int sz, bool c)
{
	if ( params[id] == NULL )
		return;
	// Create Picker
	ofxConfigPicker *picker = new ofxPickerSliderH(this, id, x, y, w, l, sz, c);
	pickers.push_back(picker);
}

//
// SLIDER: Horizontal slider
void ofxConfigGUI::setSliderV(int id, int x, int y, int w, bool flip, int sz, bool c)
{
	if ( params[id] == NULL )
		return;
	// Create Picker
	ofxConfigPicker *picker = new ofxPickerSliderV(this, id, x, y, w, flip, sz, c);
	pickers.push_back(picker);
}

//
// BUTTON
void ofxConfigGUI::setButton(int id, int x, int y, bool l, int sz)
{
	if ( params[id] == NULL )
		return;
	// Create Picker
	ofxConfigPicker *picker = new ofxPickerButton(this, id, x, y, l, sz);
	pickers.push_back(picker);
}


//
// RADIO
void ofxConfigGUI::setRadio(int id, int x, int y, bool l, int sz, bool c)
{
	if ( params[id] == NULL )
		return;
	// Create Picker
	ofxConfigPicker *picker = new ofxPickerRadio(this, id, x, y, l, sz, c);
	pickers.push_back(picker);
}



///////////////////////////////////////////////////////
//
// STANDALONE
//
// SAVE Button
void ofxConfigGUI::addSaveButton(const char *f, int x, int y, bool l, int sz)
{
	ofxConfigPicker *picker = new ofxPickerSaveButton(this, f, x, y, l, sz);
	pickers.push_back(picker);
}
void ofxConfigGUI::addLoadButton(const char *f, int x, int y, bool l, int sz)
{
	ofxConfigPicker *picker = new ofxPickerLoadButton(this, f, x, y, l, sz);
	pickers.push_back(picker);
}


///////////////////////////////////////////////////////
//
// SETTERS
//
// SLIDER: Horizontal slider
void ofxConfigGUI::setLabel(int id, const char *label)
{
	ofxConfigPicker *picker;
	if ( (picker = this->getPicker(id)) == NULL )
		return;
	picker->setLabel(label);
}
// Start pos
void ofxConfigGUI::setPos(int id, int x, int y)
{
	ofxConfigPicker *picker;
	if ( (picker = this->getPicker(id)) == NULL )
		return;
	picker->setPos(x, y);
}
// Update pos
void ofxConfigGUI::updatePos(int id)
{
	ofxConfigPicker *picker;
	if ( (picker = this->getPicker(id)) == NULL )
		return;
	picker->updatePos();
}
// Show/Hide a picker
void ofxConfigGUI::show(int id)
{
	ofxConfigPicker *picker;
	if ( (picker = this->getPicker(id)) == NULL )
		return;
	picker->show();
}
void ofxConfigGUI::hide(int id)
{
	ofxConfigPicker *picker;
	if ( (picker = this->getPicker(id)) == NULL )
		return;
	picker->hide();
}






////////////////////////////////////////////////////////
//
// MOUSE EVENTS
//
#ifdef CINDER
bool ofxConfigGUI::mouseMoved(MouseEvent e)
{
	return this->processMouseMoved(e.getX(), e.getY());
}
bool ofxConfigGUI::mousePressed(MouseEvent e)
{
	return this->processMousePressed(e.getX(), e.getY());
}
bool ofxConfigGUI::mouseDragged(MouseEvent e)
{
	return this->processMouseDragged(e.getX(), e.getY());
}
bool ofxConfigGUI::mouseReleased(MouseEvent e)
{
	return this->processMouseReleased(e.getX(), e.getY());
}
#else
void ofxConfigGUI::mouseMoved(ofMouseEventArgs &e)
{
	this->processMouseMoved(e.x, e.y);
}
void ofxConfigGUI::mousePressed(ofMouseEventArgs &e)
{
	this->processMousePressed(e.x, e.y);
}
void ofxConfigGUI::mouseDragged(ofMouseEventArgs &e)
{
	this->processMouseDragged(e.x, e.y);
}
void ofxConfigGUI::mouseReleased(ofMouseEventArgs &e)
{
	this->processMouseReleased(e.x, e.y);
}
#endif
//
// COMMON
bool ofxConfigGUI::processMouseMoved(int x, int y)
{
	// focus detection
	bool wasOver = bMouseOver;
	bMouseOver = false;
	//printf("mouse moved [%d/%d]\n",x,y);
	for (int n = 0 ; n < pickers.size() ; n++ )
	{
		ofxConfigPicker *picker = pickers[n];
		// Picker is OFF?
		if (picker->getStatus() == PICKER_OFF)
			continue;
		// mouse is over picker?
		if (picker->isMouseOver(x, y))
		{
			bMouseOver = true;
			picker->setStatus(PICKER_OVER);
		}
		else
			picker->setStatus(PICKER_OUT);
	}
	// if went out of focus
	if (wasOver && !bMouseOver)
		millisLast = getMillisNow();
	return false;
}
bool ofxConfigGUI::processMousePressed(int x, int y)
{
	//printf("mouse press [%d/%d]\n",x,y);
	bMouseDown = true;
	ofxConfigPicker *picker;
	for (int n = 0 ; n < pickers.size() ; n++ )
	{
		picker = pickers[n];
		// Picker is ON?
		if (picker->isMouseOver(x, y))
		{
			//printf("  pick [%s]\n",picker->getLabel());
			picker->setStatus(PICKER_PICKED);
			picker->pressMouse(x, y);
			return this->processMouseDragged(x, y);
#ifndef CINDER_COCOA_TOUCH
			break;	// Pick just ONE!
#endif
		}
	}
	return false;
}
bool ofxConfigGUI::processMouseDragged(int x, int y)
{
	//printf("mouse dragg [%d/%d]\n",x,y);
	bool handled = false;
	for (int n = 0 ; n < pickers.size() ; n++ )
	{
		ofxConfigPicker *picker = pickers[n];
		// Picker is ON?
		if (picker->getStatus() == PICKER_PICKED)
		{
			//printf("  drag [%s]\n",picker->getLabel());
			picker->dragMouse(x, y);
			handled = true;
#ifndef CINDER_COCOA_TOUCH
			break;
#endif
		}
	}
	//return handled;
	return false;
}
bool ofxConfigGUI::processMouseReleased(int x, int y)
{
	// release pickers
	for (int n = 0 ; n < pickers.size() ; n++ )
	{
		if (pickers[n]->isMouseOver(x, y))
		{
			pickers[n]->setStatus(PICKER_OUT);
			pickers[n]->releaseMouse(x, y);
		}
	}
	// Release!
	bMouseDown = false;
	bMouseOver = false;
	millisLast = getMillisNow();
	
	return this->processMouseMoved(x, y);
}





////////////////////////////////////////////////////////
//
// MISC
//
// virtual
int ofxConfigGUI::readFile(const char *f)
{
	if ( ofxConfig::readFile(f) == 0 )
		return 0;
	// Update pickers
	for (int n = 0 ; n < pickers.size() ; n++ )
		pickers[n]->updatePos();
}

//
// Return the picker
ofxConfigPicker* ofxConfigGUI::getPicker(int id)
{
	ofxConfigPicker *picker;
	for (int n = 0 ; n < pickers.size() ; n++ )
	{
		if (pickers[n]->getId() == id)
			return pickers[n];
	}
	return NULL;
}

//
// Get current time in millis
unsigned int ofxConfigGUI::getMillisNow()
{
    struct timeval now;
	gettimeofday(&now, NULL);
	return (unsigned int) (now.tv_sec * 1000 + now.tv_usec / 1000);
}









