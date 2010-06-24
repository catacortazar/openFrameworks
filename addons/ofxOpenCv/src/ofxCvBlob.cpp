/*
 * ofxCvBlob.h
 * openFrameworks
 *
 * A blob is a homogenous patch represented by a polygonal contour.
 * Typically a blob tracker uses the contour to figure out the blob's
 * persistence and "upgrades" it with ids and other temporal
 * information.
 *
 */

#include "ofxCvBlob.h"

ofxCvBlob::ofxCvBlob() {
	area 		= 0.0f;
	length 		= 0.0f;
	hole 		= false;
	nPts        = 0;
	id			= 0;
}

//----------------------------------------
void ofxCvBlob::draw(float x, float y){
	ofNoFill();
	
	// Draw shape
	ofSetColor(0x00FFFF);
	ofBeginShape();
	for (int i = 0; i < nPts; i++){
		ofVertex(x + pts[i].x, y + pts[i].y);
	}
	ofEndShape(true);
	
	// Draw bounds
	ofSetColor(0xff0099);
	ofRect(x + (boundingRect.x), y + (boundingRect.y), boundingRect.width, boundingRect.height);

	// Draw center
	ofLine(x + (center.x-3.0), y + (center.y-3.0), x + (center.x+3.0), y + (center.y+3.0));
	ofLine(x + (center.x-3.0), y + (center.y+3.0), x + (center.x+3.0), y + (center.y-3.0));
	ofSetColor(0xffff00);
	ofLine(x + (centroid.x-3.0), y + (centroid.y-3.0), x + (centroid.x+3.0), y + (centroid.y+3.0));
	ofLine(x + (centroid.x-3.0), y + (centroid.y+3.0), x + (centroid.x+3.0), y + (centroid.y-3.0));
	
	// draw tracker id
	char str[8];
	sprintf(str,"id%i",id);
	ofSetColor(0xffffff);
	ofDrawBitmapString(str, x + (boundingRect.x), y + (boundingRect.y));
}


