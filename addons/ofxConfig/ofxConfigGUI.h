/*
 *  ofxConfigGUI.h
 *
 *  Created by Roger Sodre on 08/04/10.
 *  Copyright 2010 Studio Avante. All rights reserved.
 *
 */
#pragma once

#include "ofxConfig.h"

#ifdef CINDER
#include "cinder/Color.h"
#include "cinder/gl/Texture.h"
#include "cinder/Text.h"
#endif

// Size in pixels
#define PICKER_DEFAULT_SIZE		11

// Default colors
#define PICKER_COLOR_OUT		ofColor(160,160,160)
#define PICKER_COLOR_OVER		ofColor(255,255,255)
#define PICKER_COLOR_PICKED		ofColor(255,255,0)
#define SHADOW_COLOR			(ofColor(32.0f,32.0f,32.0f))

// Auto hide timeout (millis)
#define MILLIS_AUTO_HIDE		2000
// Fade-out after timeout (millis)
// Must enable blend:
// glEnable(GL_BLEND);
// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#define MILLIS_AUTO_FADE		1000

//
// Picker statuses
enum enumstatus
{
	PICKER_OFF,	
	PICKER_OUT,
	PICKER_OVER,
	PICKER_PICKED
};

class ofxConfigPicker;



//
// Config Clas
class ofxConfigGUI : public ofxConfig {
public:
#ifdef CINDER
	ofxConfigGUI(App *app, bool automatic=false);
#else
	ofxConfigGUI(bool automatic=true);
#endif
	~ofxConfigGUI();

	// main loop
#ifdef CFG_CATCH_LOOP_EVENTS
	void draw(ofEventArgs &e);
#endif
	virtual void draw();
	void setMatrix(int x, int y, float s);

	virtual void setColors(ofColor out, ofColor over, ofColor pick) { defaultColorOut = out; defaultColorOver = over; defaultColorPicked = pick; };
	virtual void setAutoHide(bool b) { bAutoHide = b; };
	virtual void show() { bHidden = false; };
	virtual void hide() { bHidden = true; };
	virtual void setAutoDraw(bool b) { bAutoDraw = b; };
	virtual void setDrawText(bool b) { bDrawText = b; };
	virtual bool shoudDrawText() { return bDrawText; }; 
	
	//
	// MAKE GUI
	void setPicker(int id, bool clamp=false, int sz=PICKER_DEFAULT_SIZE);
	void setSliderH(int id, int x, int y, int w, bool l=false, int sz=PICKER_DEFAULT_SIZE, bool c=false);
	void setSliderH(int id, int ix, int x, int y, int w, bool l=false, int sz=PICKER_DEFAULT_SIZE, bool c=false);
	void setSliderV(int id, int x, int y, int w, bool flip=false, int sz=PICKER_DEFAULT_SIZE, bool c=false);
	void setButton(int id, int x, int y, bool l=false, int sz=PICKER_DEFAULT_SIZE);
	void setRadio(int id, int x, int y, bool l=false, int sz=PICKER_DEFAULT_SIZE, bool c=false);
	// setters
	void setPos(int id, int x, int y);
	void updatePos(int id);
	void show(int id);
	void hide(int id);
	// standalone buttons
	void addSaveButton(const char *f, int x, int y, bool l=false, int sz=PICKER_DEFAULT_SIZE);
	void addLoadButton(const char *f, int x, int y, bool l=false, int sz=PICKER_DEFAULT_SIZE);
	
	// update pickers
	void setLabel(int id, const char *label);
	
	
	// public because of CINDER_COCOA_TOUCH
	bool processMouseMoved(int x, int y);
	bool processMouseDragged(int x, int y);
	bool processMousePressed(int x, int y);
	bool processMouseReleased(int x, int y);
	
	// misc
	virtual int readFile(const char *filename);
	
	// Drawers
	static void setColor(ofColor c);
	static void drawLine(int x1, int y1, int x2, int y2, const ofColor &color);
	static void drawQuad(int x1, int y1, int x2, int y2, const ofColor &color, bool fill=false);

	// Defaule colors
	ofColor defaultColorOut;
	ofColor defaultColorOver;
	ofColor defaultColorPicked;
	// Current screen
	int matrixX;
	int matrixY;
	float matrixScale;
	
private:
	
#ifdef CINDER
	ci::app::App	*mApp;
	ci::CallbackId	mCbMouseDown, mCbMouseMove, mCbMouseDrag, mCbMouseUp;
#endif
	// The Pickers!!
	vector<ofxConfigPicker*> pickers;
	// Auto fade-out
	bool bAutoDraw;
	bool bHidden;
	bool bAutoHide;
	float mAlpha;
	bool bMouseDown, bMouseOver;
	bool bDrawText;
	unsigned int millisLast;
	
	// misc
	ofxConfigPicker* getPicker(int id);
	unsigned int getMillisNow();

	//
	// Mouse Events
	// Thanks memo!
	// http://www.openframeworks.cc/forum/viewtopic.php?f=9&t=1825
#ifdef CINDER
	bool mouseMoved(MouseEvent e);
	bool mouseDragged(MouseEvent e);
	bool mousePressed(MouseEvent e);
	bool mouseReleased(MouseEvent e);
#else
	void mouseMoved(ofMouseEventArgs &e);
	void mouseDragged(ofMouseEventArgs &e);
	void mousePressed(ofMouseEventArgs &e);
	void mouseReleased(ofMouseEventArgs &e);
#endif
	
	// Draw functions
	virtual void drawGUI();
};



