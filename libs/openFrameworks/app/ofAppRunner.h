#ifndef _OF_APP_RUNNER
#define _OF_APP_RUNNER


#include "ofMain.h"
#include "ofBaseApp.h"
#include "ofAppBaseWindow.h"
#include "ofAppGlutWindow.h"

void 		ofSetupOpenGL(int w, int h, int screenMode);	// sets up the opengl context!
void 		ofSetupOpenGL(ofAppBaseWindow * windowPtr, int w, int h, int screenMode);	// sets up the opengl context!
void		ofSetupWindow(ofWindowApp *app, int w, int h, int screenMode);	// sets up secondary opengl context!
void 		ofRunApp(ofBaseApp * OFSA = NULL);
ofBaseApp * ofGetAppPtr();

//-------------------------- GLUT Hack
void ofHideWindowBorders();


//-------------------------- time
float 		ofGetFrameRate();
int			ofGetFrameNum();
void 		ofSetFrameRate(int targetRate);
void		ofSleepMillis(int millis);
double		ofGetLastFrameTime();

//-------------------------- cursor
void 		ofHideCursor();
void 		ofShowCursor();
//-------------------------- window / screen
int 		ofGetWindowPositionX();
int 		ofGetWindowPositionY();
int 		ofGetScreenWidth();
int 		ofGetScreenHeight();
int			ofGetWindowMode();
int 		ofGetWidth();			// <-- should we call this ofGetWindowWidth?
int 		ofGetHeight();

void 		ofSetWindowPosition(int x, int y);
void 		ofSetWindowShape(int width, int height);
void 		ofSetWindowTitle(string title);
void		ofEnableSetupScreen();
void		ofDisableSetupScreen();
void		ofSetFullscreen(bool fullscreen);
void		ofToggleFullscreen();
//-------------------------- sync
void 		ofSetVerticalSync(bool bSync);

#endif
