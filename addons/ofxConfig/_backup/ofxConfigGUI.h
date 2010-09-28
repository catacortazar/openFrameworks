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


#define PICKER_COLOR_OUT		ofColor(160,160,160)
#define PICKER_COLOR_OVER		ofColor(255,255,255)
#define PICKER_COLOR_PICKED		ofColor(255,255,0)

// Size in pixels
#define PICKER_SIZE				10

// Bound scale
#define PICKER_BOUND_SCALE		1.5f

// Auto hide timeout (millis)
#define MILLIS_AUTO_HIDE		2000
// Fade-out after timeout (millis)
// Must enable blend:
// glEnable(GL_BLEND);
// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#define MILLIS_AUTO_FADE		1000

//
// Picker types
enum enumtypes
{
	PICKER_ABSOLUTE,
	PICKER_SLIDER
};

//
// Picker statuses
enum enumstatus
{
	PICKER_OFF,	
	PICKER_OUT,
	PICKER_OVER,
	PICKER_PICKED
};

//
// Picker info
typedef struct _pickerInfo {
	int type;
	int size;
	int radius, boundRadius;
	bool clamp;
	ofColor colorOut;
	ofColor colorOver;
	ofColor colorPicked;
	// picker state
	int status;
	int posX, posY;
	// slider
	char desc[128];
	int startX, startY;
	int width;
} pickerInfo;


//
// Config Clas
class ofxConfigGUI : public ofxConfig {
public:
#ifdef CINDER
	ofxConfigGUI(App *app);
#else
	ofxConfigGUI();
#endif
	~ofxConfigGUI();
	
	virtual void draw();
	
	void setAutoHide(bool b);
	void show();
	void hide();

	// Here because of CINDER_COCOA_TOUCH
	bool processMouseMoved(int x, int y);
	bool processMouseDragged(int x, int y);
	bool processMousePressed(int x, int y);
	bool processMouseReleased(int x, int y);
	

	// Assign params to GUI
	void setPicker(int id, bool clamp=false, int sz=PICKER_SIZE);
	void setSlider(int id, int x, int y, int w, const char *desc, int sz=PICKER_SIZE);

private:
	
#ifdef CINDER
	ci::app::App	*mApp;
	ci::CallbackId	mCbMouseDown, mCbMouseMove, mCbMouseDrag, mCbMouseUp;
	Font mFont;
#endif
	// Defaule colors
	ofColor defaultColorOut;
	ofColor defaultColorOver;
	ofColor defaultColorPicked;
	// Mouse data
	int lastX, lastY;
	// Auto fade-out
	bool bHidden;
	bool bAutoHide;
	float mAlpha;
	bool bMouseDown;
	unsigned int millisLast;

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
	
	// Loop events
#ifdef CFG_CATCH_LOOP_EVENTS
	void drawGUI(ofEventArgs &e);
#endif
	
	// misc
	void calcPos(ofxConfigParam *param);
	bool isMouseOverPicker(ofxConfigParam *p, float x, float y);
	unsigned int getMillisNow();

	// picker creation
	pickerInfo* allocPicker(int id);

	// Draw functions
	void drawGUI();
	void drawPicker(pickerInfo *picker);
	void drawSlider(pickerInfo *picker, char *valstr);
	ofColor getColorShadow();
	ofColor getColorPicker(pickerInfo *picker);
	void setColor(ofColor c);

};



