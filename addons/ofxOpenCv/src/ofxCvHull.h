/*
 * ofxCvHull.h
 * openFrameworks
 *
 * by rsodre
 *
 * Hull data
 *
 */


#ifndef OFX_CV_HULL_H
#define OFX_CV_HULL_H


#include "ofxCvConstants.h"

class ofxCvBlob;

typedef struct _ofxCvConvexity
	{
		// from CvConvexityDefect
		ofPoint		start;		// point of the contour where the defect begins
		ofPoint		end;		// point of the contour where the defect ends
		ofPoint		in;			// the farthest from the convex hull point within the defect
		float		depth;		// distance between the farthest point and the convex hull
		// new
		float		aperture;	// distance between start > end
		ofPoint		middle;		// point in the middle of start > end
		float		angle;		// Angle of in > middle
	} ofxCvConvexity;


class ofxCvHull {
	
public:
	
	// hull points
	vector <ofPoint>			pts;
	
	// convexity points
	vector <ofxCvConvexity>		conv;
	
	// Direction (from blob centroid to farthest hull point)
	// (think of an egg, the direction will always point to the egg's narrower end)
	int					farthest;		// Farthest point index
	ofPoint				dirPts[2];		// [0]=centroid [1]=farthest
	float				dirAngle;		// radians: 0 to 360, CCW
	float				dirDistance;	// centroid to farthest
	
	// draw elements
	bool				bDrawHull;
	bool				bDrawHullPoints;
	bool				bDrawConvexity;
	bool				bDrawConvexityPoints;
	bool				bDrawDirection;
	
	// Constructor
	ofxCvHull();
	
	// update hull
	void update(CvSeq* ptSeq, ofxCvBlob *blob);
	
	// Draw convex hulls
	void draw(float x = 0, float y = 0);
};


#endif


