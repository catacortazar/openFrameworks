#include "windowApp.h"

//--------------------------------------------------------------
void windowApp::setup(){
	
	printf("windowApp: window [%d] setup()\n", number);

}

//--------------------------------------------------------------
void windowApp::update(){

	//printf("windowApp: window [%d] update()\n", number);

}

//--------------------------------------------------------------
void windowApp::draw(){
	
	//printf("windowApp: window [%d] draw()\n", number);

	if (number == 0)
		ofBackground(0,0,200);
	else
		ofBackground(0,100,0);
	
	ofSetColor(0,255,255);
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
	sprintf(str,"mouse  [%d/%d]",mouseX,mouseY);
	ofDrawBitmapString(str, x, y);
	y+=15;
}

//--------------------------------------------------------------
void windowApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void windowApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void windowApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void windowApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void windowApp::windowResized(int w, int h){
	
}

