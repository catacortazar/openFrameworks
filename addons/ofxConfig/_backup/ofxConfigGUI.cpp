/*
 *  ofxConfigGUI.cpp
 *
 *  Created by Roger Sodre on 08/04/10.
 *  Copyright 2010 Studio Avante. All rights reserved.
 *
 */

#include "ofxConfigGUI.h"
#include <sys/time.h>

//
// CUSTOM CONFIG CLASS
#ifdef CINDER
ofxConfigGUI::ofxConfigGUI(App *app) : ofxConfig()
#else
ofxConfigGUI::ofxConfigGUI() : ofxConfig()
#endif
{
	// Setup mouse events
#ifdef CINDER
	mApp = app;
	mCbMouseDown = mApp->registerMouseDown( this, &ofxConfigGUI::mousePressed );
	mCbMouseMove = mApp->registerMouseMove( this, &ofxConfigGUI::mouseMoved );	
	mCbMouseDrag = mApp->registerMouseDrag( this, &ofxConfigGUI::mouseDragged );	
	mCbMouseUp   = mApp->registerMouseUp(	this, &ofxConfigGUI::mouseReleased );	
	// Default font
	mFont = Font( "Helvetica", 14 );
#else
	ofAddListener(ofEvents.mousePressed,	this, &ofxConfigGUI::mousePressed);
	ofAddListener(ofEvents.mouseMoved,		this, &ofxConfigGUI::mouseMoved);
	ofAddListener(ofEvents.mouseDragged,	this, &ofxConfigGUI::mouseDragged);
	ofAddListener(ofEvents.mouseReleased,	this, &ofxConfigGUI::mouseReleased);
#endif
	
	// Automatic draw
#ifdef CFG_CATCH_LOOP_EVENTS
	ofAddListener(ofEvents.draw,			this, &ofxConfigGUI::drawGUI);
#endif
	
	// init params
	defaultColorOut = PICKER_COLOR_OUT;
	defaultColorOver = PICKER_COLOR_OVER;
	defaultColorPicked = PICKER_COLOR_PICKED;
	lastX = lastY = 0;
	bHidden = false;
	bAutoHide = false;
	bMouseDown = false;
	millisLast = getMillisNow();
}
ofxConfigGUI::~ofxConfigGUI()
{
	// Setup mouse events
#ifdef CINDER
	mApp->unregisterMouseDown( mCbMouseDown );
	mApp->unregisterMouseMove( mCbMouseMove );	
	mApp->unregisterMouseDrag( mCbMouseDrag );	
	mApp->unregisterMouseUp  ( mCbMouseUp );
#endif
}

//
// main Loop
void ofxConfigGUI::draw()
{
#ifndef CFG_CATCH_LOOP_EVENTS
	this->drawGUI();
#endif
}

//
// show/hide GUI
void ofxConfigGUI::show()
{
	bHidden = false;
}
void ofxConfigGUI::hide()
{
	bHidden = true;
}

//
// auto hide when inactive
void ofxConfigGUI::setAutoHide(bool b)
{
	bAutoHide = !bAutoHide;
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
	//printf("mouse moved [%d/%d]\n",x,y);
	ofxConfigParam *param;
	pickerInfo *picker;
	for (int id = 0 ; id < params.size() ; id++ )
	{
		// Get param/picker
		if ( (param = params[id]) == NULL )
			continue;
		if ( (picker = (pickerInfo*) param->picker) == NULL )
			continue;
		// Picker is OFF?
		if (picker->status == PICKER_OFF)
			continue;
		// mouse is over picker?
		if (isMouseOverPicker(param, x, y))
			picker->status = PICKER_OVER;
		else
			picker->status = PICKER_OUT;
	}
	return false;
}
bool ofxConfigGUI::processMousePressed(int x, int y)
{
	//printf("mouse pressed [%d/%d]\n",x,y);
	bMouseDown = true;
	ofxConfigParam *param;
	pickerInfo *picker;
	for (int id = 0 ; id < params.size() ; id++ )
	{
		// Get param/picker
		if ( (param = params[id]) == NULL )
			continue;
		if ( (picker = (pickerInfo*) param->picker) == NULL )
			continue;
		// Picker is ON?
		if (isMouseOverPicker(param, x, y))
		{
			picker->status = PICKER_PICKED;
			break;	// Pick just ONE!
		}
	}
	// Remember position
	lastX = x;
	lastY = y;
	// Check pos
	return this->processMouseDragged(x, y);
}
bool ofxConfigGUI::processMouseDragged(int x, int y)
{
	bool handled = false;
	ofxConfigParam *param;
	pickerInfo *picker;
	int dx, dy;
	float min, max;
	for (int id = 0 ; id < params.size() ; id++ )
	{
		// Get param/picker
		if ( (param = params[id]) == NULL )
			continue;
		if ( (picker = (pickerInfo*) param->picker) == NULL )
			continue;
		// Picker is ON?
		if (picker->status == PICKER_PICKED)
		{
			// Absolute picker
			if (picker->type == PICKER_ABSOLUTE)
			{
				// Move X
				dx = x-lastX;
				// ignore movement outside limits
				min = this->getMinX(id);
				max = this->getMaxX(id);
				if (picker->clamp && lastX < min)
					dx -= (min-lastX);
				else if (picker->clamp && lastX > max)
					dx += (lastX-max);
				this->addX(id, dx, picker->clamp);
				// Move Y
				dy = y-lastY;
				// ignore movement outside limits
				min = this->getMinY(id);
				max = this->getMaxY(id);
				if (picker->clamp && lastY < min)
					dy -= (min-lastY);
				else if (picker->clamp && lastY > max)
					dy += (lastY-max);
				this->addY(id, dy, picker->clamp);
			}
			// Slider
			else
			{
				if (x <= picker->startX)
					this->setProg(id, 0.0);
				else if (x >= picker->startX+picker->width)
					this->setProg(id, 1.0);
				else
					this->setProg(id, ((x-picker->startX)/(float)picker->width));
			}
			// feito!
			handled = true;
			break;
		}
	}
	// Remember position
	lastX = x;
	lastY = y;
	//return handled;
	return false;
}
bool ofxConfigGUI::processMouseReleased(int x, int y)
{
	bMouseDown = false;
	millisLast = getMillisNow();
	return this->processMouseMoved(x, y);
}















/////////////////////////
//
// MISC
//
//
// Find current poition
void ofxConfigGUI::calcPos(ofxConfigParam *param)
{
	pickerInfo *picker = (pickerInfo*) param->picker;
	// Picker type
	switch(picker->type)
	{
		case PICKER_ABSOLUTE:
			picker->posX = (int) param->vec[0].get();
			picker->posY = (int) param->vec[1].get();
			break;
		case PICKER_SLIDER:
			picker->posX = picker->startX + (picker->width * param->value.getProg());
			picker->posY = picker->startY;
			break;
	}
}
//
// Test if mouse is over a picker
bool ofxConfigGUI::isMouseOverPicker(ofxConfigParam *param, float x, float y)
{
	pickerInfo *picker = (pickerInfo*) param->picker;
	// get current position
	this->calcPos(param);
	// make bounds
	int x1, y1, x2, y2;
	if (picker->type == PICKER_ABSOLUTE)
	{
		x1 = picker->posX - picker->boundRadius;
		x2 = picker->posX + picker->boundRadius;
		y1 = picker->posY - picker->boundRadius;
		y2 = picker->posY + picker->boundRadius;
	}
	else
	{
		x1 = picker->startX - picker->boundRadius;
		x2 = picker->startX + picker->width + picker->boundRadius;
		y1 = picker->startY - picker->boundRadius;
		y2 = picker->startY + picker->boundRadius;
	}
	// Check pos > bounds
	return ( x >= x1 && x <= x2 && y >= y1 && y <= y2 );
}
//
// Get current time in millis
unsigned int ofxConfigGUI::getMillisNow()
{
    struct timeval now;
	gettimeofday(&now, NULL);
	return (unsigned int) (now.tv_sec * 1000 + now.tv_usec / 1000);
}


////////////////////////////////////////////////////////
//
// ADD GUI ELEMENTS
//
// Alloc a picker
pickerInfo* ofxConfigGUI::allocPicker(int id)
{
	ofxConfigParam *param = params[id];
	if (param->picker == NULL)
		param->picker = malloc(sizeof(pickerInfo));
	return (pickerInfo*) param->picker;
}

//
// PICKER: The value is the coordinate on the screen.
// Set a VECTOR as picker
void ofxConfigGUI::setPicker(int id, bool clamp, int sz)
{
	// alloc Picker
	pickerInfo *picker = this->allocPicker(id);
	
	if (params[id]->type != CFG_TYPE_VECTOR)
	{
		printf("Param [%s] is not TYPE_VECTOR become a picker!",params[id]->name);
		return;
	}
	// Save picker info
	picker->type = PICKER_ABSOLUTE;
	picker->clamp = clamp;
	picker->size = sz;
	picker->radius = ceil(sz / 2.0f);
	picker->boundRadius = ceil((sz * PICKER_BOUND_SCALE) / 2.0f);
	picker->colorOut = defaultColorOut;
	picker->colorOver = defaultColorOver;
	picker->colorPicked = defaultColorPicked;
	// Put picker to work
	picker->status = PICKER_OUT;
}

//
// SLIDER: Horizontal slider
void ofxConfigGUI::setSlider(int id, int x, int y, int w, const char *desc, int sz)
{
	// alloc Picker
	pickerInfo *picker = this->allocPicker(id);

	// Save picker info
	picker->type = PICKER_SLIDER;
	picker->clamp = true;
	picker->size = sz;
	picker->radius = ceil(sz / 2.0f);
	picker->boundRadius = ceil((sz * PICKER_BOUND_SCALE) / 2.0f);
	picker->colorOut = defaultColorOut;
	picker->colorOver = defaultColorOver;
	picker->colorPicked = defaultColorPicked;
	// set slider info
	picker->startX = x;
	picker->startY = y;
	picker->width = w;
	strcpy(picker->desc, desc);
	// Put picker to work
	picker->status = PICKER_OUT;
}

////////////////////////////////////////////////////
//
// DRAW GUI
// 
// Draw automatically from Loop Event
#ifdef CFG_CATCH_LOOP_EVENTS
void ofxConfigGUI::drawGUI(ofEventArgs &e)
{
	this->drawGUI();
}
#endif
//
// Draw GUI
void ofxConfigGUI::drawGUI()
{
	// Hide / fade
	if (!bHidden)
	{
		mAlpha = 1.0;
		// Not touching
		if (!bMouseDown)
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
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	
	ofxConfigParam *param;
	pickerInfo *picker;
	//
	// PICKERS
	//
	for (int id = 0 ; id < params.size() ; id++ )
	{
		// Get param/picker
		if ( (param = params[id]) == NULL )
			continue;
		if ( (picker = (pickerInfo*) param->picker) == NULL )
			continue;
		// Hidden?
		if (picker->type != PICKER_ABSOLUTE && bHidden)
			continue;
		// Draw picker GUI
		switch(picker->type)
		{
			case PICKER_SLIDER:
				this->drawSlider(picker, this->getString(id));
				break;
		}
		// Draw picker
		if (mAlpha == 0.0)
			continue;
		this->calcPos(param);
		this->drawPicker(picker);
	}
}

//
// Draw square picker
void ofxConfigGUI::drawPicker(pickerInfo *picker)
{
	// picker bounds
	float x1 = picker->posX-picker->radius;
	float x2 = picker->posX+picker->radius;
	float y1 = picker->posY-picker->radius;
	float y2 = picker->posY+picker->radius;
	// Draw picker
	// 1st pass = shadow, 2nd pass = color
	for (int n = 0 ; n < 2 ; n++)
	{
		// Set color
		if (n==0)
			this->setColor(this->getColorShadow());
		else
			this->setColor(this->getColorPicker(picker));
#ifdef CINDER
		gl::drawSolidRect( Rectf(x1-n, y1-n, x2-n, y2-n));
#else
		glBegin(GL_LINE_LOOP);
		glVertex3f(x1-n, y1-n, 0.0f);
		glVertex3f(x2-n, y1-n, 0.0f);
		glVertex3f(x2-n, y2-n, 0.0f);
		glVertex3f(x1-n, y2-n, 0.0f);
		glEnd();
#endif
	}
}

//
// Draw square picker
void ofxConfigGUI::drawSlider(pickerInfo *picker, char *valstr)
{
	// Draw slider
	float x1 = picker->startX;
	float x2 = picker->startX+picker->width;
	float y1 = picker->startY;
	float y2 = picker->startY;
	// 1st pass = shadow, 2nd pass = color
	for (int n = 0 ; n < 2 ; n++)
	{
		// Set color
		ofColor color;
		if (n==0)
			color = this->getColorShadow();
		else
			color = this->getColorPicker(picker);
		this->setColor(color);
#ifdef CINDER
		gl::drawLine( Vec2f(x1-n, y1-n), Vec2f(x2-n, y2-n) );
#else
		glBegin(GL_LINES);
		glVertex3f(x1-n, y1-n, 0.0f);
		glVertex3f(x2-n, y2-n, 0.0f);
		glEnd();
#endif
		// Picker name
		float x = picker->startX;
		float y = picker->startY - picker->radius - 4;
#ifdef CINDER
		y -= 12;
		if (n == 1)
			gl::drawString( picker->desc, Vec2f(x-n,y-n), ColorA( color.r/255.0f, color.g/255.0f, color.b/255.0f, mAlpha ), mFont );
#else
		ofDrawBitmapString(picker->desc, x-n, y-n);
#endif
		// Picker value
		x = picker->startX + picker->width;
#ifdef CINDER
		// Not for Cinder (performance issue)
		//gl::drawStringRight( str, Vec2f(x-n,y-n), ColorA( color.r/255.0f, color.g/255.0f, color.b/255.0f, a ), mFont );
#else
		int w = strlen(valstr) * 8;
		ofDrawBitmapString(valstr, x-n-w, y-n);
#endif
	}
}




//
// Get color for picker
ofColor ofxConfigGUI::getColorPicker(pickerInfo *picker)
{
	// Set color
	if (picker->status == PICKER_PICKED)
		return picker->colorPicked;
	else if (picker->status == PICKER_OVER)
		 return picker->colorOver;
	else //if (picker->status == PICKER_OUT)
		return picker->colorOut;
}
// Get shadow color
ofColor ofxConfigGUI::getColorShadow()
{
	return ofColor( 32.0f, 32.0f, 32.0f );
}
// Set GL color
void ofxConfigGUI::setColor(ofColor c)
{
	glColor4f( c.r/255.0f, c.g/255.0f, c.b/255.0f, mAlpha );
}





