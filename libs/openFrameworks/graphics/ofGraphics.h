#ifndef OF_GEOM
#define OF_GEOM

#include "ofConstants.h"
#include "ofTypes.h"

#define  	CIRC_RESOLUTION		    22				// 22 pts for a circle...

// bg color
float * ofBgColorPtr();
void ofBackground(const ofColor & c);
void ofBackground(int hexColor, float _a=255.0f);
void ofBackground(int r, int g, int b, int a=255);

// user's access to settings (bgAuto, corner mode):
void 	ofSetBackgroundAuto(bool bManual);		// default is true
void 	ofSetRectMode(int mode);		// set the mode, either to OF_RECTMODE_CORNER or OF_RECTMODE_CENTER

//set whether OF uses ARB rectangular texture or the more traditonal GL_TEXTURE_2D
bool ofGetUsingArbTex();
void ofEnableArbTex();
void ofDisableArbTex();


bool ofGetUsingNormalizedTexCoords();
void ofEnableNormalizedTexCoords();
void ofDisableNormalizedTexCoords();


//***** add global functions to override texture settings
void ofSetTextureWrap(GLfloat wrapS = GL_CLAMP_TO_EDGE, GLfloat wrapT = GL_CLAMP_TO_EDGE);
bool ofGetUsingCustomTextureWrap();
void ofRestoreTextureWrap();

void ofSetMinMagFilters(GLfloat minFilter = GL_LINEAR, GLfloat maxFilter = GL_LINEAR);
bool ofGetUsingCustomMinMagFilters();
void ofRestoreMinMagFilters();
//*****



// OF's access to settings (bgAuto, origin, corner mode):
bool 	ofbClearBg();
int 	ofGetRectMode();				// default is OF_RECTMODE_CORNER

void ofSetCircleResolution(int res);  		// if there 22 is a problem, you can change it here
void ofSetCurveResolution(int res);

// geometry
void ofTriangle(const ofPoint & p1, const ofPoint & p2, const ofPoint & p3);
void ofTriangle(float x1,float y1,float x2,float y2,float x3, float y3);
void ofTriangle(float x1,float y1,float z1,float x2,float y2,float z2,float x3, float y3,float z3);

void ofCircle(const ofPoint & p, float radius);
void ofCircle(float x, float y, float radius);
void ofCircle(float x, float y, float z, float radius);

void ofEllipse(const ofPoint & p, float width, float height);
void ofEllipse(float x, float y, float width, float height);
void ofEllipse(float x, float y, float z, float width, float height);

void ofLine(const ofPoint & p1, const ofPoint & p2);
void ofLine(float x1,float y1,float x2,float y2);
void ofLine(float x1,float y1,float z1,float x2,float y2,float z2);

void ofRect(const ofRectangle & r);
void ofRect(const ofPoint & p,float w,float h);
void ofRect(float x1,float y1,float w,float h);
void ofRect(float x,float y,float z,float w,float h);

void ofCurve(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);
void ofBezier(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);


// drawing options
void ofNoFill();
void ofFill();
float ofGetFill();

void ofSetLineWidth(float lineWidth);

// color options
void ofSetColor(const ofColor & color);
void ofSetColor(const ofColor & color, int _a);
void ofSetColor(int r, int g, int b); // 0-255
void ofSetColor(int r, int g, int b, int a); // 0-255
void ofSetColor(int hexColor); // hex, like web 0xFF0033;

// transparency
void ofEnableAlphaBlending();  // this just turns on and off opengl blending, the common mode
void ofDisableAlphaBlending();

// smooth 
void ofEnableSmoothing();
void ofDisableSmoothing();

// drawing style - combines color, fill, blending and smoothing
ofStyle ofGetStyle();
void ofSetStyle(ofStyle style);
void ofPushStyle();
void ofPopStyle();

//our openGL wrappers
void ofPushMatrix();
void ofPopMatrix();
void ofTranslate(const ofPoint & p);
void ofTranslate(float x, float y, float z = 0);
void ofScale(float xAmnt, float yAmnt, float zAmnt = 1);
void ofRotate(float degrees, float vecX, float vecY, float vecZ);
void ofRotateX(float degrees);
void ofRotateY(float degrees);
void ofRotateZ(float degrees);
void ofRotate(float degrees);

// bitmapped type
// ** note, this uses glDrawPixels and may be S L 0 W on some graphics cards
void ofDrawBitmapString(string textString, const ofPoint & p);
void ofDrawBitmapString(string textString, float x, float y);
void ofDrawBitmapString(string textString, float x, float y, float z);

// screen coordinate things / default gl values
void ofSetupGraphicDefaults();
void ofSetupScreen();

void ofCurveVertex(float x, float y);
void ofBezierVertex(float x1, float y1, float x2, float y2, float x3, float y3);

// for polygons
void ofSetPolyMode(int mode);	
void ofBeginShape();
void ofVertex(float x, float y);
void ofEndShape(bool bClose = false);
void ofNextContour(bool bClose = false);  // for multi contour shapes!

// Check for OpenGL Errors
int ofCheckGLError(const char *cmd=NULL);

#endif
