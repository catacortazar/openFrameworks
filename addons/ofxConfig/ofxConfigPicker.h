/*
 *  ofxPicker.h
 *  ofxConfig
 *
 *  Created by Roger on 08/09/10.
 *  Copyright 2010 Studio Avante. All rights reserved.
 *
 */
#pragma once

#include "ofxConfigGUI.h"

#define PICKER_BOUND_PIXELS		1
#define PICKER_LABEL_SPACER		8
#define PICKER_TEXT_HEIGHT		8

//
// Picker types
enum enumPickerStyle
{
	STYLE_STACKED,
	STYLE_HORIZONTAL
};



//
// BASE PICKER
//
class ofxConfigPicker {
public:
	ofxConfigPicker(ofxConfigGUI *_cfg, int _id, int x, int y, int sz);
	~ofxConfigPicker() {};
	
	// Setters
	void setColors(ofColor out, ofColor over, ofColor pick) { colorOut = out; colorOver = over; colorPicked = pick; };
	void setStatus(int s) { status = s; };
	void setLabel(const char *l) { strcpy(label, l); };
	void setMouse(int x, int y) { mouseX = x; mouseY = y; };
	void setPos(int x, int y) { startX = x; startY = y; this->updatePos(); } ;
	void show() { bHidden = false; };
	void hide() { bHidden = true; };

	// Getters
	int getId() { return id; };
	int getStatus() { return status; };
	const char* getLabel() { return label; };

	// Virtuals
	virtual bool isMouseOver(int x, int y);
	virtual void updatePos() {};
	virtual void draw(float alpha) {};
	// mouse events
	virtual void pressMouse(int x, int y) { this->setMouse(x, y); };
	virtual void dragMouse(int x, int y) { this->setMouse(x, y); this->updatePos(); };
	virtual void releaseMouse(int x, int y) {};

protected:
	// Param reference
	ofxConfigGUI *cfg;					// the cfg
	int id;							// param id
	
	// picker data
	int size;
	int radius, boundRadius;
	bool clamp;
	ofColor colorOut;
	ofColor colorOver;
	ofColor colorPicked;
	char label[128];
	
	// picker state
	int status;
	int mouseX, mouseY;				// last known mouse position
	int startX, startY;				// current position of th epicker
	int pickerX, pickerY;			// current position of th epicker
	int boX1, boX2, boY1, boY2;		// picker bounds
	float mAlpha;
	bool bHidden;
	
#ifdef CINDER
	Font mFont;
#endif

	// OpenGL
	void drawLabel(int x, int y, bool line);
	void drawValue(int x, int y, int width, bool line, const char *val);
	void drawText(int x, int y, const char *text);
	
	// Draw
	ofColor getCurrentColor();
	void drawPicker(int x, int y, bool fill=false);
	
};


//////////////////////////////////
//
// PICKER: ABSOLUTE (Vector)
//
class ofxPickerAbsolute : public ofxConfigPicker {
public:
	ofxPickerAbsolute(ofxConfigGUI *_cfg, int _id, bool _clamp, int sz);
	~ofxPickerAbsolute() {};
	void updatePos();
	void dragMouse(int x, int y);
	void draw(float alpha);
};


//////////////////////////////////
//
// PICKER: SLIDER HORIZONTAL (any)
//
class ofxPickerSliderH : public ofxConfigPicker {
public:
	ofxPickerSliderH(ofxConfigGUI *_cfg, int _id, int x, int y, int w, bool l, int sz, bool c);
	~ofxPickerSliderH() {};
	void updatePos();
	void dragMouse(int x, int y);
	void draw(float alpha);

private:
	int width;				// slider line width
	bool line;				// draw single line
};


//////////////////////////////////
//
// PICKER: SLIDER VERTICAL PICKER (any)
//
class ofxPickerSliderV : public ofxConfigPicker {
public:
	ofxPickerSliderV(ofxConfigGUI *_cfg, int _id, int x, int y, int h, bool f, int sz, bool c);
	~ofxPickerSliderV() {};
	void updatePos();
	void dragMouse(int x, int y);
	void draw(float alpha);
	
private:
	int height;
	bool flip;				// flip up/down;
};


//////////////////////////////////
//
// PICKER: BUTTON (Boolean)
//
class ofxPickerButton : public ofxConfigPicker {
public:
	ofxPickerButton(ofxConfigGUI *_cfg, int _id, int x, int y, bool l, int sz);
	~ofxPickerButton() {};
	bool isMouseOver(int x, int y);
	void pressMouse(int x, int y);
	void releaseMouse(int x, int y);
	void draw(float alpha);
	
protected:
	bool line;				// draw single line
	bool isDown;
};
// SAVE BUTTON
class ofxPickerSaveButton : public ofxPickerButton {
public:
	ofxPickerSaveButton(ofxConfigGUI *_cfg, const char *f, int x, int y, bool l, int sz);
	~ofxPickerSaveButton() {};
	void pressMouse(int x, int y);
private:
	string file;
};
// LOAD BUTTON
class ofxPickerLoadButton : public ofxPickerButton {
public:
	ofxPickerLoadButton(ofxConfigGUI *_cfg, const char *f, int x, int y, bool l, int sz);
	~ofxPickerLoadButton() {};
	void pressMouse(int x, int y);
private:
	string file;
};


//////////////////////////////////
//
// PICKER: RADIO (Integer)
//
typedef struct _radioOption {
	int pickerX, pickerY;			// current position on screen
	ofPoint bo1, bo2;				// picker bounds
	int value;						// value of this option
} radioOption;
class ofxPickerRadio : public ofxConfigPicker {
public:
	ofxPickerRadio(ofxConfigGUI *_cfg, int _id, int x, int y, bool l, int sz, bool c);
	~ofxPickerRadio() {};
	bool isMouseOver(int x, int y);
	void pressMouse(int x, int y);
	void draw(float alpha);
	
private:
	int width;
	bool line;						// draw single line
	vector<radioOption> options;	// The options bounds
	int optionOver;					// option INDEX where mouse is over
	int optionSelected;				// option INDEX where mouse is over
};


