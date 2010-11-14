#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	height = 0;
	ofSetCircleResolution(50);
	ofBackground(200,200,200);
	ofSetWindowTitle("color operations");
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
	float dy = 25;
	float x, y, val;
	ofColor c1, c2, c3;
	
	//////////////////////////////////
	//
	// TEST COLOR OPERATORS
	//
	dy = 25;
	
	//
	// Add
	x = ofGetWidth()*0.1;
	y = ofGetWidth()*0.1;
	// ofcolor + ofColor
	y += dy;
	c1 = ofColor(0);
	c2 = ofColor(100);
	c3 = c1 + c2;
	this->drawColorOperator(x, y, c1, c2, c3, "+");
	// ofcolor + ofColor
	y += dy;
	c1 = ofColor(100);
	c2 = ofColor(155);
	c3 = c1 + c2;
	this->drawColorOperator(x, y, c1, c2, c3, "+");
	// ofcolor + ofColor
	y += dy;
	c1 = ofColor(0);
	c2 = ofColor(255);
	c3 = c1 + c2;
	this->drawColorOperator(x, y, c1, c2, c3, "+");
	// ofcolor + ofColor
	y += dy;
	c1 = ofColor(255,0,0);
	c2 = ofColor(0,255,0);
	c3 = c1 + c2;
	this->drawColorOperator(x, y, c1, c2, c3, "+");
	// ofcolor + ofColor
	y += dy;
	c1 = ofColor(255,0,0);
	c2 = ofColor(0,0,255);
	c3 = c1 + c2;
	this->drawColorOperator(x, y, c1, c2, c3, "+");
	// ofcolor += ofColor
	y += dy;
	c1 = ofColor(0,255,0);
	c2 = ofColor(0,0,255);
	c3 = c1;
	c3 += c2;
	this->drawColorOperator(x, y, c1, c2, c3, "+");
	// ofcolor += ofColor
	y += dy;
	c1 = ofColor(255,0,0);
	c2 = ofColor(255,0,0);
	c3 = c1;
	c3 += c2;
	this->drawColorOperator(x, y, c1, c2, c3, "+");
	// ofcolor += ofColor
	y += dy;
	c1 = ofColor(255,255,0);
	c2 = ofColor(0,0,255);
	c3 = c1;
	c3 += c2;
	this->drawColorOperator(x, y, c1, c2, c3, "+");
	// ofcolor += ofColor
	y += dy;
	c1 = ofColor(0,255,255);
	c2 = ofColor(255,0,0);
	c3 = c1;
	c3 += c2;
	this->drawColorOperator(x, y, c1, c2, c3, "+");
	// ofcolor += ofColor
	y += dy;
	c1 = ofColor(255,0,255);
	c2 = ofColor(0,255,0);
	c3 = c1;
	c3 += c2;
	this->drawColorOperator(x, y, c1, c2, c3, "+");
	// ofcolor + float
	y += dy;
	c1 = ofColor(0);
	val = 100;
	c3 = c1 + val;
	this->drawColorOperator(x, y, c1, val, c3, "+");
	// ofcolor + float
	y += dy;
	c1 = ofColor(0);
	val = 255;
	c3 = c1 + val;
	this->drawColorOperator(x, y, c1, val, c3, "+");
	// ofColor + float
	y += dy;
	c1 = ofColor(255,0,0);
	val = 100;
	c3 = c1 + val;
	this->drawColorOperator(x, y, c1, val, c3, "+");
	// ofColor += float
	y += dy;
	c1 = ofColor(255,0,0);
	val = 255;
	c3 = c1;
	c3 += val;
	this->drawColorOperator(x, y, c1, val, c3, "+");
	
	//
	// Sub
	// 
	x = ofGetWidth()*0.3;
	y = ofGetWidth()*0.1;
	// ofColor - ofColor
	y += dy;
	c1 = ofColor(255);
	c2 = ofColor(100);
	c3 = c1 - c2;
	this->drawColorOperator(x, y, c1, c2, c3, "-");
	// ofColor - ofColor
	y += dy;
	c1 = ofColor(100);
	c2 = ofColor(100);
	c3 = c1 - c2;
	this->drawColorOperator(x, y, c1, c2, c3, "-");
	// ofColor - ofColor
	y += dy;
	c1 = ofColor(255);
	c2 = ofColor(255);
	c3 = c1 - c2;
	this->drawColorOperator(x, y, c1, c2, c3, "-");
	// ofColor - ofColor
	y += dy;
	c1 = ofColor(255,255,255);
	c2 = ofColor(255,0,0);
	c3 = c1 - c2;
	this->drawColorOperator(x, y, c1, c2, c3, "-");
	// ofColor - ofColor
	y += dy;
	c1 = ofColor(255,255,255);
	c2 = ofColor(0,255,0);
	c3 = c1 - c2;
	this->drawColorOperator(x, y, c1, c2, c3, "-");
	// ofColor -= ofColor
	y += dy;
	c1 = ofColor(255,255,255);
	c2 = ofColor(0,0,255);
	c3 = c1;
	c3 -= c2;
	this->drawColorOperator(x, y, c1, c2, c3, "-");
	// ofColor - ofColor
	y += dy;
	c1 = ofColor(255,0,0);
	c2 = ofColor(255,0,0);
	c3 = c1 - c2;
	this->drawColorOperator(x, y, c1, c2, c3, "-");
	// ofColor - ofColor
	y += dy;
	c1 = ofColor(255,255,0);
	c2 = ofColor(0,255,0);
	c3 = c1 - c2;
	this->drawColorOperator(x, y, c1, c2, c3, "-");
	// ofColor - ofColor
	y += dy;
	c1 = ofColor(255,255,0);
	c2 = ofColor(255,0,0);
	c3 = c1 - c2;
	this->drawColorOperator(x, y, c1, c2, c3, "-");
	// ofColor - ofColor
	y += dy;
	c1 = ofColor(0,255,255);
	c2 = ofColor(0,255,0);
	c3 = c1 - c2;
	this->drawColorOperator(x, y, c1, c2, c3, "-");
	// ofColor - ofColor
	y += dy;
	c1 = ofColor(0,255,255);
	c2 = ofColor(0,0,255);
	c3 = c1 - c2;
	this->drawColorOperator(x, y, c1, c2, c3, "-");
	// ofColor - ofColor
	y += dy;
	c1 = ofColor(255,0,255);
	c2 = ofColor(255,0,0);
	c3 = c1 - c2;
	this->drawColorOperator(x, y, c1, c2, c3, "-");
	// ofColor - ofColor
	y += dy;
	c1 = ofColor(255,0,255);
	c2 = ofColor(0,0,255);
	c3 = c1 - c2;
	this->drawColorOperator(x, y, c1, c2, c3, "-");
	// ofColor - float
	y += dy;
	c1 = ofColor(255);
	val = 200;
	c3 = c1 - val;
	this->drawColorOperator(x, y, c1, val, c3, "-");
	// ofColor - float
	y += dy;
	c1 = ofColor(255,255,255);
	val = 255;
	c3 = c1 - val;
	this->drawColorOperator(x, y, c1, val, c3, "-");
	// ofColor - float
	y += dy;
	c1 = ofColor(255,0,0);
	val = 100;
	c3 = c1 - val;
	this->drawColorOperator(x, y, c1, val, c3, "-");
	// ofColor -= float
	y += dy;
	c1 = ofColor(255,0,0);
	val = 255;
	c3 = c1;
	c3 -= val;
	this->drawColorOperator(x, y, c1, val, c3, "-");
	
	//
	// Multiplication
	// see ofTypes.h for explanation
	// or: http://www.digitalartform.com/imageArithmetic.htm
	x = ofGetWidth()*0.5;
	y = ofGetWidth()*0.1;
	// ofColor * ofColor
	// multiplying a color by BLACK will make anything black (0% of it)
	y += dy;
	c1 = ofColor(100);
	c2 = ofColor(0);
	c3 = c1 * c2;
	this->drawColorOperator(x, y, c1, c2, c3, "*");
	// ofColor * ofColor
	y += dy;
	c1 = ofColor(100);
	c2 = ofColor(100);
	c3 = c1 * c2;
	this->drawColorOperator(x, y, c1, c2, c3, "*");
	// ofColor * ofColor
	// multiplying a color by WHITE will return the original color (100% of it)
	y += dy;
	c1 = ofColor(100);
	c2 = ofColor(255);
	c3 = c1 * c2;
	this->drawColorOperator(x, y, c1, c2, c3, "*");
	// ofColor * ofColor
	y += dy;
	c1 = ofColor(255,0,0);
	c2 = ofColor(0,0,0);
	c3 = c1 * c2;
	this->drawColorOperator(x, y, c1, c2, c3, "*");
	// ofColor * ofColor
	// multiplying a color by 1.0 will return the original color (100% of it)
	y += dy;
	c1 = ofColor(255,0,0);
	c2 = ofColor(255,255,255);
	c3 = c1 * c2;
	this->drawColorOperator(x, y, c1, c2, c3, "*");
	// ofColor * ofColor
	// multiply RED by GREEN will give us black, because the red channel of GREEN will cancel RED, and the green channel of RED will cancel GREEN
	y += dy;
	c1 = ofColor(255,0,0);
	c2 = ofColor(0,255,0);
	c3 = c1 * c2;
	this->drawColorOperator(x, y, c1, c2, c3, "*");
	// ofColor *= ofColor
	y += dy;
	c1 = ofColor(255,255,0);
	c2 = ofColor(100,100,0);
	c3 = c1;
	c3 *= c2;
	this->drawColorOperator(x, y, c1, c2, c3, "*");
	// ofColor * float
	y += dy;
	c1 = ofColor(255,0,0);
	val = 0.0;
	c3 = c1 * val;
	this->drawColorOperator(x, y, c1, val, c3, "*");
	// ofColor * float
	y += dy;
	c1 = ofColor(255,0,0);
	val = 0.5;
	c3 = c1 * val;
	this->drawColorOperator(x, y, c1, val, c3, "*");
	// ofColor * float
	y += dy;
	c1 = ofColor(255,0,0);
	val = 1.0;
	c3 = c1;
	c3 *= val;
	this->drawColorOperator(x, y, c1, val, c3, "*");
	// ofColor * float
	y += dy;
	c1 = ofColor(255,0,0);
	val = 2.0;
	c3 = c1;
	c3 *= val;
	this->drawColorOperator(x, y, c1, val, c3, "*");
	
	//
	// Division
	// 
	x = ofGetWidth()*0.7;
	y = ofGetWidth()*0.1;
	// ofColor / ofColor
	// Dividing anything by WHITE will return the original color
	y += dy;
	c1 = ofColor(100);
	c2 = ofColor(255);
	c3 = c1 / c2;
	this->drawColorOperator(x, y, c1, c2, c3, "/");
	// ofColor / ofColor
	y += dy;
	c1 = ofColor(100);
	c2 = ofColor(100);
	c3 = c1 / c2;
	this->drawColorOperator(x, y, c1, c2, c3, "/");
	// ofColor /= ofColor
	// Dividing anything by BLACK is impossible (division by zero = core dump!), so will return the original color
	y += dy;
	c1 = ofColor(100);
	c2 = ofColor(0);
	c3 /= c1 / c2;
	this->drawColorOperator(x, y, c1, c2, c3, "/");
	// ofColor / ofColor
	// Dividing anything by WHITE will return the original color
	y += dy;
	c1 = ofColor(255,0,0);
	c2 = ofColor(255);
	c3 = c1 / c2;
	this->drawColorOperator(x, y, c1, c2, c3, "/");
	// ofColor /= ofColor
	// Dividing anything by BLACK is impossible (division by zero = core dump!), so will return the original color
	y += dy;
	c1 = ofColor(255,0,0);
	c2 = ofColor(0);
	c3 /= c1 / c2;
	this->drawColorOperator(x, y, c1, c2, c3, "/");
	// ofColor / ofColor
	y += dy;
	c1 = ofColor(255,0,0);
	c2 = ofColor(0,255,0);
	c3 = c1 / c2;
	this->drawColorOperator(x, y, c1, c2, c3, "/");
	// ofColor / ofColor
	y += dy;
	c1 = ofColor(255,255,0);
	c2 = ofColor(0,255,255);
	c3 = c1 / c2;
	this->drawColorOperator(x, y, c1, c2, c3, "/");
	// ofColor / float
	y += dy;
	c1 = ofColor(255,0,0);
	val = 0.0;
	c3 = c1 / val;
	this->drawColorOperator(x, y, c1, val, c3, "/");
	// ofColor / float
	y += dy;
	c1 = ofColor(255,0,0);
	val = 0.5;
	c3 = c1 / val;
	this->drawColorOperator(x, y, c1, val, c3, "/");
	// ofColor / float
	y += dy;
	c1 = ofColor(255,0,0);
	val = 1.0;
	c3 = c1;
	c3 /= val;
	this->drawColorOperator(x, y, c1, val, c3, "/");
	// ofColor / float
	y += dy;
	c1 = ofColor(255,0,0);
	val = 2.0;
	c3 = c1;
	c3 /= val;
	this->drawColorOperator(x, y, c1, val, c3, "/");
	// ofColor / float
	y += dy;
	c1 = ofColor(255,0,0);
	val = 5.0;
	c3 = c1;
	c3 /= val;
	this->drawColorOperator(x, y, c1, val, c3, "/");
}


//--------------------------------------------------------------
void testApp::drawColorOperator (int x, int y, ofColor c1, ofColor c2, ofColor c3, string op){
	int sz = 20;
	// color 1
	ofSetColor(c1);
	ofRect(x, y, sz, sz);
	x += sz*1.2;
	// operation
	ofSetColor(0);
	ofDrawBitmapString(op, x, y+sz*0.75);
	x += sz*0.5;
	// color 2
	ofSetColor(c2);
	ofRect(x, y, sz, sz);
	x += sz*1.2;
	// =
	ofSetColor(0);
	ofDrawBitmapString("=", x, y+sz*0.75);
	x += sz*0.5;
	// color 3
	ofSetColor(c3);
	ofRect(x, y, sz, sz);
}

//--------------------------------------------------------------
void testApp::drawColorOperator (int x, int y, ofColor c1, float val, ofColor c3, string op){
	int sz = 20;
	// color 1
	ofSetColor(c1);
	ofRect(x, y, sz, sz);
	x += sz*1.2;
	// operation
	ofSetColor(0);
	ofDrawBitmapString(op, x, y+sz*0.75);
	x += sz*0.5;
	// scalar
	char str[10];
	sprintf(str,"%.1f",val);
	ofSetColor(0);
	ofDrawBitmapString(string(str), x, y+sz*0.75);
	x += sz*2.2;
	// =
	ofSetColor(0);
	ofDrawBitmapString("=", x, y+sz*0.75);
	x += sz*0.5;
	// color 3
	ofSetColor(c3);
	ofRect(x, y, sz, sz);
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
