#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	ofEnableSmoothing();
	
	font.loadFont("frabk.ttf", 128);

}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

	ofBackground(0);
	
	ofSetColor(255,255,255);
	font.drawString("main",0,ofGetHeight());
	
	ofSetColor(255,255,0);
	ofCircle(mouseX, mouseY, 15);

	ofSetColor(255,255,255);
	int x = 10;
	int y = 20;
	char str[64];
	sprintf(str,"screen [%d/%d]",ofGetScreenWidth(),ofGetScreenHeight());
	ofDrawBitmapString(str, x, y);
	y+=15;
	sprintf(str,"size   [%d/%d]",ofGetWidth(),ofGetHeight());
	ofDrawBitmapString(str, x, y);
	y+=15;
	sprintf(str,"pos    [%d/%d]",ofGetWindowPositionX(),ofGetWindowPositionY());
	ofDrawBitmapString(str, x, y);
	y+=15;
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

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

