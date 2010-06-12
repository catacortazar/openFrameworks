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


#ifndef OFX_CV_BLOB_H
#define OFX_CV_BLOB_H

#include "ofxCvConstants.h"
#include "ofxCvHull.h"


class ofxCvBlob {
	
public:
	
	float               area;
	float               length;
	ofRectangle         boundingRect;
	ofPoint             centroid;
	bool                hole;
	
	ofPoint             center;		// Rect center
	float               perim;		// Rect perimeter
	
	vector <ofPoint>    pts;    // the contour of the blob
	int                 nPts;   // number of pts;
	
	ofxCvHull			hull;
	int                 id;   // for history sorting
	
	//----------------------------------------
	ofxCvBlob();
	
	//----------------------------------------
	void update();
	
	//----------------------------------------
	void draw(float x = 0, float y = 0);
};


#endif


