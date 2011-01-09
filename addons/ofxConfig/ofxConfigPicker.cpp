/*
 *  ofxPicker.cpp
 *  ofxConfigExample
 *
 *  Created by Roger on 08/09/10.
 *  Copyright 2010 Studio Avante. All rights reserved.
 *
 */

#include "ofxConfigPicker.h"

ofxConfigPicker::ofxConfigPicker(ofxConfigGUI *_cfg, int _id, int x, int y, int sz)
{
	// Save picker info
	cfg = _cfg;
	id = _id;
	clamp = clamp;
	startX = x;
	startY = y;
	size = sz;
	radius = ceil(sz / 2.0f);
	boundRadius = (radius + PICKER_BOUND_PIXELS);
	colorOut = cfg->defaultColorOut;
	colorOver = cfg->defaultColorOver;
	colorPicked = cfg->defaultColorPicked;
	status = PICKER_OUT;
	bHidden = false;
	if (id >= 0)
		strcpy(label, cfg->getName(id));

#ifdef CINDER
	mFont = Font( "Helvetica", 14 );
#endif
}

//
// Test if mouse is over a picker
bool ofxConfigPicker::isMouseOver(int x, int y)
{
	if (bHidden)
		return false;
	return ( x >= boX1 && x <= boX2 && y >= boY1 && y <= boY2 );
}

/////////////////
//
// DRAW
//
// Get color for picker
ofColor ofxConfigPicker::getCurrentColor()
{
	ofColor c;
	// Set color
	if (status == PICKER_PICKED)
		c = colorPicked;
	else if (status == PICKER_OVER)
		c = colorOver;
	else //if (status == PICKER_OUT)
		c = colorOut;
#ifndef CINDER
	c.a = (255.0f * mAlpha);
#endif
	return c;
}
//
// DRAW square picker
void ofxConfigPicker::drawPicker(int x, int y, bool fill)
{
	// square bounds
	float x1 = x - radius;
	float x2 = x + radius;
	float y1 = y - radius;
	float y2 = y + radius;
	
	// draw quad
	cfg->drawQuad(x1, y1, x2, y2, this->getCurrentColor(), fill);
}

//////////////////////////////////
//
// OPENGL
//
// Draw some Text
void ofxConfigPicker::drawLabel(int x, int y, bool line)
{
	// No label
	if (strlen(label) == 0)
		return;
	// Setup
	int xx, yy;
	if (line)
	{
		int w = strlen(label) * 8;
		xx = x - radius - w - PICKER_LABEL_SPACER;
		yy = y + PICKER_TEXT_HEIGHT/2;
	}
	else
	{
		xx = x - radius;
		yy = y - radius - PICKER_LABEL_SPACER/2;
	}
	// Draw!
	this->drawText(xx, yy, label);
}
//
// Draw some Text
void ofxConfigPicker::drawValue(int x, int y, int width, bool line, const char *val)
{
// Not for Cinder (bad performance)
//#ifndef CINDER
	int xx, yy;
	// Picker name
	if (line)
	{
		xx = x + width + radius + PICKER_LABEL_SPACER/2;
		yy = y + PICKER_TEXT_HEIGHT/2;
	}
	else
	{
		int w = strlen(val) * 8;
		xx = x + radius + width - w;
		yy = y - radius - PICKER_LABEL_SPACER/2;
	}
	this->drawText(xx, yy, val);
//#endif
}
//
// Draw some Text
void ofxConfigPicker::drawText(int x, int y, const char *text)
{
	if (!cfg->shoudDrawText())
		return;
	// Draw picker
	// 1st pass = shadow, 2nd pass = color
	for (int n = 0 ; n < 2 ; n++)
	{
		int off = ( n==0 ? 1 : 0);

		// Set color
		ofColor color = ( n==0 ? SHADOW_COLOR : this->getCurrentColor() );
		cfg->setColor(color);
		
#ifdef CINDER
		y -= 4;
		// No shadow for Cinder (bad performance)
		if (n == 1)
			gl::drawString(text, Vec2f(x+off, y+off), ColorA( color.r/255.0f, color.g/255.0f, color.b/255.0f, mAlpha ), mFont );
#else
		ofDrawBitmapString(text, x+off, y+off);
#endif
	}
}



////////////////////////////////////////////////////////////////////////////
//
// PICKER:ABSOLUTE
//
// for VECTOR types
// The position is the vector x/y
// Must take care with the current trans matrix (cfg->matrix*)
ofxPickerAbsolute::ofxPickerAbsolute(ofxConfigGUI *_cfg, int _id, bool _clamp, int sz) : ofxConfigPicker(_cfg, _id, 0, 0, sz)
{
	if (cfg->getType(id) != CFG_TYPE_VECTOR)
	{
		printf("ofxPickerAbsolute:: [%s] is not TYPE_VECTOR!!!",cfg->getName(id));
		return;
	}
	clamp = _clamp;
	// Update position based on current param value
	this->updatePos();
}
//
// Update position based on current param value
void ofxPickerAbsolute::updatePos()
{
	pickerX = cfg->matrixX + (int)(cfg->getX(id) * cfg->matrixScale);
	pickerY = cfg->matrixY + (int)(cfg->getY(id) * cfg->matrixScale);
	boX1 = pickerX - boundRadius;
	boX2 = pickerX + boundRadius;
	boY1 = pickerY - boundRadius;
	boY2 = pickerY + boundRadius;
}
//
// Mouse
void ofxPickerAbsolute::dragMouse(int x, int y)
{
	int dx, dy;
	float min, max;
	
	// Move X
	dx = x-mouseX;
	// ignore movement outside limits
	min = cfg->getMinX(id);
	max = cfg->getMaxX(id);
	if (clamp && mouseX < min)
		dx -= (min-mouseX);
	else if (clamp && mouseX > max)
		dx += (mouseX-max);
	dx = (int)(dx / cfg->matrixScale);
	cfg->addX(id, dx, clamp);
	
	// Move Y
	dy = y-mouseY;
	// ignore movement outside limits
	min = cfg->getMinY(id);
	max = cfg->getMaxY(id);
	if (clamp && mouseY < min)
		dy -= (min-mouseY);
	else if (clamp && mouseY > max)
		dy += (mouseY-max);
	dy = (int)(dy / cfg->matrixScale);
	cfg->addY(id, dy, clamp);
	
	// super
	this->ofxConfigPicker::dragMouse(x, y);
}
//
// DRAW
void ofxPickerAbsolute::draw(float alpha)
{
	if (bHidden)
		return;
	// update from cfg
	this->updatePos();
	// Draw the picker
	mAlpha = alpha;
	this->drawPicker(pickerX, pickerY);
}


////////////////////////////////////////////////////////////////////////////
//
// PICKER: SLIDER HORIZONTAL
//
ofxPickerSliderH::ofxPickerSliderH(ofxConfigGUI *_cfg, int _id, int x, int y, int w, bool l, int sz, bool c) : ofxConfigPicker(_cfg, _id, x, y, sz)
{
	width = w;
	line = l;
	// centralize
	if (c)
		startX -= (width/2);
	// Update position based on current param value
	this->updatePos();
}
//
// Update position based on current param value
void ofxPickerSliderH::updatePos()
{
	pickerX = startX + (width * cfg->getProg(id));
	pickerY = startY;
	boX1 = startX - boundRadius;
	boX2 = startX + width + boundRadius;
	boY1 = startY - boundRadius;
	boY2 = startY + boundRadius;
}
//
// Mouse
void ofxPickerSliderH::dragMouse(int x, int y)
{
	if (x <= startX)
		cfg->setProg(id, 0.0);
	else if (x >= startX+width)
		cfg->setProg(id, 1.0);
	else
		cfg->setProg(id, ((x-startX)/(float)width));
	
	// super
	this->ofxConfigPicker::dragMouse(x, y);
}
//
// DRAW
void ofxPickerSliderH::draw(float alpha)
{
	if (bHidden)
		return;

	mAlpha = alpha;

	float x1 = startX;
	float x2 = startX + width;
	float y1 = startY;
	float y2 = startY;
	
	// Slider line
	cfg->drawLine(x1, y1, x2, y2, this->getCurrentColor());
	
	// Picker name
	this->drawLabel(startX, startY, line);
	
	// Picker value
	this->drawValue(startX, startY, width, line, cfg->getString(id));
	
	// Draw the picker
	// update from cfg
	this->updatePos();
	this->drawPicker(pickerX, pickerY, true);
}


////////////////////////////////////////////////////////////////////////////
//
// PICKER: SLIDER VERTICAL PICKER
//
ofxPickerSliderV::ofxPickerSliderV(ofxConfigGUI *_cfg, int _id, int x, int y, int h, bool f, int sz, bool c) : ofxConfigPicker(_cfg, _id, x, y, sz)
{
	height = h;
	flip = f;
	// centralize
	if (c)
		startX -= (height/2);
	// Update position based on current param value
	this->updatePos();
}
//
// Find current poition
void ofxPickerSliderV::updatePos()
{
	float p = cfg->getProg(id);
	pickerX = startX;
	pickerY = startY + (height * ( flip ? p : 1.0-p ) );
	
	boX1 = startX - boundRadius;
	boX2 = startX + boundRadius;
	boY1 = startY - boundRadius;
	boY2 = startY + height + boundRadius;

}
//
// Mouse
void ofxPickerSliderV::dragMouse(int x, int y)
{
	if (flip)
	{
		if (y <= startY)
			cfg->setProg(id, 0.0);
		else if (y >= startY+height)
			cfg->setProg(id, 1.0);
		else
			cfg->setProg(id, ((y-startY)/(float)height));
	}
	else
	{
		if (y <= startY)
			cfg->setProg(id, 1.0);
		else if (y >= startY+height)
			cfg->setProg(id, 0.0);
		else
			cfg->setProg(id, 1.0-((y-startY)/(float)height));
	}
	
	// super
	this->ofxConfigPicker::dragMouse(x, y);
}
//
// DRAW
void ofxPickerSliderV::draw(float alpha)
{
	if (bHidden)
		return;

	mAlpha = alpha;
	
	float x1 = startX;
	float x2 = startX;
	float y1 = startY;
	float y2 = startY + height;
	
	// Slider line
	cfg->drawLine(x1, y1, x2, y2, this->getCurrentColor());
	
	// update from cfg
	this->updatePos();
	// Draw the picker
	this->drawPicker(pickerX, pickerY, true);
}




////////////////////////////////////////////////////////////////////////////
//
// PICKER: BUTTON
//
ofxPickerButton::ofxPickerButton(ofxConfigGUI *_cfg, int _id, int x, int y, bool l, int sz) : ofxConfigPicker(_cfg, _id, x, y, sz)
{
	line = l;
	isDown = false;
	// Fixed position
	pickerX = (int) startX;
	pickerY = (int) startY;
	boX1 = pickerX - boundRadius;
	boX2 = pickerX + boundRadius;
	boY1 = pickerY - boundRadius;
	boY2 = pickerY + boundRadius;
}
//
// Release pick
bool ofxPickerButton::isMouseOver(int x, int y)
{
	bool over = ofxConfigPicker::isMouseOver(x, y);
	if (!over && isDown)
		isDown = false;
	return over;
}
//
// Mouse
void ofxPickerButton::pressMouse(int x, int y)
{
	this->ofxConfigPicker::setMouse(x, y);
	isDown = true;
	cfg->invert(id);
}
void ofxPickerButton::releaseMouse(int x, int y)
{
	isDown = false;
}
//
// DRAW
void ofxPickerButton::draw(float alpha)
{
	if (bHidden)
		return;

	mAlpha = alpha;
	
	int x, y, w;
	
	// Button name
	y = startY + PICKER_TEXT_HEIGHT/2;
	if (line)
	{
		w = strlen(label) * 8;
		x = startX - radius - w - PICKER_LABEL_SPACER;
	}
	else
	{
		x = startX + radius + PICKER_LABEL_SPACER;
	}
	this->drawText(x, y, label);
		
	// Draw the picker
	bool fill = false;
	if (id >= 0)
		fill = cfg->getBool(id);
	else
		fill = isDown;
	this->drawPicker(pickerX, pickerY, fill);
}

////////////////////////////////////////////////////////////////////////////
//
// PICKER: SAVE BUTTON
//
ofxPickerSaveButton::ofxPickerSaveButton(ofxConfigGUI *_cfg, const char *f, int x, int y, bool l, int sz) : ofxPickerButton(_cfg, -1, x, y, l, sz)
{
	this->setLabel("SAVE");
	file = string(f);
}
//
// Mouse
void ofxPickerSaveButton::pressMouse(int x, int y)
{
	this->ofxConfigPicker::setMouse(x, y);
	cfg->saveFile(file.c_str());
	isDown = true;
}
///////////////////////
//
// PICKER: LOAD BUTTON
//
ofxPickerLoadButton::ofxPickerLoadButton(ofxConfigGUI *_cfg, const char *f, int x, int y, bool l, int sz) : ofxPickerButton(_cfg, -1, x, y, l, sz)
{
	this->setLabel("LOAD");
	file = string(f);
}
//
// Mouse
void ofxPickerLoadButton::pressMouse(int x, int y)
{
	this->ofxConfigPicker::setMouse(x, y);
	cfg->readFile(file.c_str());
	isDown = true;
}




////////////////////////////////////////////////////////////////////////////
//
// PICKER: RADIO
//
ofxPickerRadio::ofxPickerRadio(ofxConfigGUI *_cfg, int _id, int x, int y, bool l, int sz, bool c) : ofxConfigPicker(_cfg, _id, x, y, sz)
{
	line = l;
	optionOver = -1;
	optionSelected = -1;
	// centralize
	int min = (int) cfg->getMin(id);
	int max = (int) cfg->getMax(id);
	int qt = (max-min);
	int off = size + 5;
	width = (qt * off);
	if (c)
		startX -= (width/2);
	// Fixed positions
	for (int n = 0 ; n <= qt ; n++ )
	{
		radioOption o;
		o.pickerX = startX + (n * off);
		o.pickerY = startY;
		o.bo1.x = o.pickerX - boundRadius;
		o.bo2.x = o.pickerX + boundRadius;
		o.bo1.y = o.pickerY - boundRadius;
		o.bo2.y = o.pickerY + boundRadius;
		o.value = (min + n);
		options.push_back(o);
		// Selected?
		if (cfg->getInt(id) == o.value)
			optionSelected = n;
	}
}
//
// Test if mouse is over a picker
bool ofxPickerRadio::isMouseOver(int x, int y)
{
	if (bHidden)
		return false;
	for ( int n = 0 ; n < options.size() ; n++ )
	{
		radioOption o = options[n];
		if ( x >= o.bo1.x && x <= o.bo2.x && y >= o.bo1.y && y <= o.bo2.y )
		{
			optionOver = n;
			return true;
		}
	}
	optionOver = -1;
	return false;
}
//
// Mouse
void ofxPickerRadio::pressMouse(int x, int y)
{
	this->ofxConfigPicker::setMouse(x, y);
	radioOption o = options[optionOver];
	optionSelected = optionOver;
	cfg->set(id, o.value);
}
//
// DRAW
void ofxPickerRadio::draw(float alpha)
{
	if (bHidden)
		return;

	mAlpha = alpha;
	
	// Button name
	this->drawLabel(startX, startY, line);
	// Current value
	if (optionOver >= 0)
		this->drawValue(startX, startY, width, line, cfg->getValueLabel(id, optionOver));
	else if (optionSelected >= 0)
		this->drawValue(startX, startY, width, line, cfg->getString(id));
	
	// Draw the pickers
	int v = cfg->getInt(id);
	for ( int n = 0 ; n < options.size() ; n++ )
	{
		radioOption o = options[n];
		this->drawPicker(o.pickerX, o.pickerY, (o.value == v));
	}
}




