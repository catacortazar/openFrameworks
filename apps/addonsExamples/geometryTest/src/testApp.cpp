#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	height = 0;
	ofSetCircleResolution(50);
	ofBackground(200,200,200);
	ofSetWindowTitle("geometry test");
	ofSetLineWidth(3);

	ofSetFrameRate(60); // if vertical sync is off, we can go a bit fast... this caps the framerate at 60fps.
}

//--------------------------------------------------------------
void testApp::update(){
	height = height + 0.04f;
	if (height > TWO_PI)
		height = 0;
	
}

//--------------------------------------------------------------
void testApp::draw(){
	float dx = (ofGetWidth() *0.1);
	float dy = (ofGetHeight() *0.1);
	float x, y, z, w, h, r, val;
	ofPoint p1, p2, p3;
	ofColor c, c1, c2, c3;

	//////////////////////////////////
	//
	// TEST GEOMETRY
	//
	ofFill();
	x = dx;
	y = dy;
	
	// ofLine
	ofSetColor(0, 0, 0);
	x = dx;
	y += dy;
	w = (dx*0.8);
	h = (dx*0.5);
	// x/y
	ofLine(x, y-h, x+w, y);
	// x/y/z
	x += dx*2;
	z = 50.0*sin(height);
	ofLine(x, y-h, z, x+w, y, z);
	// point
	x += dx*2;
	z = -50.0*sin(height);
	p1.set(x, y-h, z);
	p2.set(x+w, y, z);
	ofLine(p1, p2);
	
	// ofTriangle
	ofSetColor(255, 0, 0);
	x = dx;
	y += dy;
	// x/y
	ofTriangle(x, y, x+dx, y, x, y-dy);
	// x/y/z
	x += dx*2;
	z = 50.0*sin(height);
	ofTriangle(x, y, z, x+dx, y, z, x, y-dy, z);
	// point
	x += dx*2;
	z = -50.0*sin(height);
	p1.set(x, y, z);
	p2.set(x+dx, y, z);
	p3.set(x, y-dy, z);
	ofTriangle(p1, p2, p3);
	
	// ofCircle
	ofSetColor(0, 255, 0);
	x = dx;
	y += dy;
	r = (dx*0.3f);
	// x/y
	ofCircle(x, y, r);
	// x/y/z
	x += dx*2;
	z = 50.0*sin(height);
	ofCircle(x, y, z, r);
	// point
	x += dx*2;
	z = -50.0*sin(height);
	p1.set(x, y, z);
	ofCircle(p1, r);
	
	// ofEllipse
	ofSetColor(0, 0, 255);
	x = dx;
	y += dy;
	w = (dx*0.8);
	h = (dx*0.5);
	// x/y
	ofEllipse(x, y, w, h);
	// x/y/z
	x += dx*2;
	z = 50.0*sin(height);
	ofEllipse(x, y, z, w, h);
	// point
	x += dx*2;
	z = -50.0*sin(height);
	p1.set(x, y, z);
	ofEllipse(p1, w, h);
	
	// ofRect
	ofSetColor(0, 255, 255);
	x = dx;
	y += dy;
	w = (dx*0.8);
	h = (dx*0.5);
	// x/y
	ofRect(x, y, w, h);
	// x/y/z
	x += dx*2;
	z = 50.0*sin(height);
	ofRect(x, y, z, w, h);
	// point
	x += dx*2;
	z = -50.0*sin(height);
	p1.set(x, y, z);
	ofRect(p1, w, h);
	
	// text
	ofSetColor(0, 0, 0);
	x = dx;
	y += dy;
	// x/y
	ofDrawBitmapString("text1", x, y);
	// x/y/z
	x += dx*2;
	z = 50.0*sin(height);
	ofDrawBitmapString("text2", x, y, z);
	// point
	x += dx*2;
	z = -50.0*sin(height);
	p1.set(x, y, z);
	ofDrawBitmapString("text3", p1);
	
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){
}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}


//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}
