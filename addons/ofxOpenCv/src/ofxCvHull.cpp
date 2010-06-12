
#include "ofxCvHull.h"
#include "ofxCvBlob.h"


//----------------------------------------
ofxCvHull::ofxCvHull() {
	farthest	= 0;
	dirAngle	= 0.0;
	dirDistance	= 0.0;
	bDrawHull = true;
	bDrawHullPoints = true;
	bDrawConvexity = true;
	bDrawConvexityPoints = true;
	bDrawDirection = true;
}


// Update Hull from blob points
//----------------------------------------
void ofxCvHull::update(CvSeq* ptSeq, ofxCvBlob *blob) {
	CvPoint cvpt;
	ofPoint ofpt;

	// get opencv hull
	CvSeq *hullSeq	= cvConvexHull2( ptSeq, 0, CV_CLOCKWISE, 0 );
	
	// Clear hull points
	pts.clear();
	
	// Save hull points
	for( int n = 0; n < hullSeq->total ; n++ )
	{
		cvpt = **CV_GET_SEQ_ELEM( CvPoint*, hullSeq, n );
		ofpt.set( (float)cvpt.x, (float)cvpt.y );
		//printf("HULL[%d] pt[%d] cvpt[%d/%d] ofpt[%.1f/%.1f]\n",i,j,cvpt.x,cvpt.y,ofpt.x,ofpt.y);
		pts.push_back( ofpt );
	}
	
	// Calc distances to blob centroid
	float dist[hullSeq->total];
	for( int n = 0; n < hullSeq->total ; n++ )
	{
		cvpt = **CV_GET_SEQ_ELEM( CvPoint*, hullSeq, n );
		dist[n] = ofDist( (float)cvpt.x, (float)cvpt.y, (float)blob->centroid.x, (float)blob->centroid.y);
	}
	// Find farthest point to blob centroid
	int farthest = 0;
	for ( int n = 1 ; n < hullSeq->total ; n++ )
	{
		if (dist[n] > dist[farthest])
			farthest = n;
	}
	farthest = farthest;
	
	// direction points
	dirPts[0] = blob->centroid;
	dirPts[1] = pts[farthest];
	// direction angle
	dirAngle = -atan2(dirPts[1].y-dirPts[0].y,dirPts[1].x-dirPts[0].x);
	if (dirAngle < 0.0)
		dirAngle += (PI*2.0);
	// direction distance to centroid
	dirDistance = ofDist(dirPts[0].x,dirPts[0].y,dirPts[1].x,dirPts[1].y);
	
	//
	// CONVEXITY
	//
	CvSeq *convSeq	= cvConvexityDefects(ptSeq, hullSeq);
	CvConvexityDefect	cvConv;
	ofxCvConvexity		ofConv;
	
	// Clear convs
	conv.clear();
	
	// Save convexity points
	for( int n = 0; n < convSeq->total ; n++ )
	{
		cvConv = *CV_GET_SEQ_ELEM( CvConvexityDefect, convSeq, n );
		ofConv.start	= ofPoint( (float)cvConv.start->x, (float)cvConv.start->y );
		ofConv.end		= ofPoint( (float)cvConv.end->x, (float)cvConv.end->y );
		ofConv.in		= ofPoint( (float)cvConv.depth_point->x, (float)cvConv.depth_point->y );
		ofConv.depth	= cvConv.depth;
		// Aperture (begin > end)
		ofPoint ap		= ofConv.end - ofConv.start;	// start > end
		ofConv.aperture	= sqrt( (ap.x*ap.x) + (ap.y*ap.y) );
		ofConv.middle	= ofConv.start + (ap / 2.0);
		// Direction / Angle between in-start and in-end
		ofConv.angle	= -atan2(ofConv.middle.y-ofConv.in.y,ofConv.middle.x-ofConv.in.x);
		if (ofConv.angle < 0.0)
			ofConv.angle += (PI*2.0);
		//printf("CONVEXITY[%d] pt[%d] cvpt[%.1f/%.1f]\n",i,n,ofConv.in.x,ofConv.in.y);
		conv.push_back( ofConv );
	}
	
	// Free opencv hull / convexity
	cvClearSeq(convSeq);
	cvClearSeq(hullSeq);
}


//----------------------------------------
void ofxCvHull::draw(float x, float y) {
	char str[10];

	ofNoFill();
	
	// convexity points
	if (bDrawConvexityPoints)
	{
		for (int i = 0; i < conv.size(); i++)
		{
			ofSetColor( 0x0000FF );
			ofCircle(x + conv[i].start.x, y + conv[i].start.y, 3.0);
			//ofDrawBitmapString("b", x + conv[i].start.x, y + conv[i].start.y);
			ofCircle(x + conv[i].end.x, y + conv[i].end.y, 3.0);
			//ofDrawBitmapString("e", x + conv[i].end.x, y + conv[i].end.y);
			ofSetColor( 0x00FFFF );
			ofCircle(x + conv[i].in.x, y + conv[i].in.y, 3.0);
			ofSetColor( 0xFFFF00 );
			ofCircle(x + conv[i].middle.x, y + conv[i].middle.y, 2.0);
			// direction angle
			//ofLine(x + conv[i].in.x, y + conv[i].in.y, x + conv[i].middle.x, y + conv[i].middle.y);
			//sprintf(str,"%.1fdg",ofRadToDeg(conv[i].angle));
			//ofDrawBitmapString(str, x + conv[i].in.x, y + conv[i].in.y);
		}
	}
	
	// convexity lines
	if (bDrawConvexity)
	{
		ofSetColor( 0x0077FF );
		for (int i = 0; i < conv.size(); i++)
		{
			ofLine(x + conv[i].in.x, y + conv[i].in.y, x + conv[i].start.x, y + conv[i].start.y);
			ofLine(x + conv[i].in.x, y + conv[i].in.y, x + conv[i].end.x, y + conv[i].end.y);
		}
	}
	
	// hull points
	if (bDrawHullPoints)
	{
		for (int i = 0; i < pts.size(); i++)
		{
			ofSetColor( i == farthest ? 0xFFFFFF : 0xFF0000 );
			ofCircle(x + pts[i].x, y + pts[i].y, 3.0);
		}
	}
	
	// hull lines
	if (bDrawHull)
	{
		ofSetColor(0x44FF44);
		ofBeginShape();
		for (int i = 0; i < pts.size(); i++)
			ofVertex(x + pts[i].x, y + pts[i].y);
		ofEndShape(true);
	}
	
	// direction
	if (bDrawDirection)
	{
		ofSetColor(0xFFFFFF);
		ofLine(x+dirPts[0].x, y+dirPts[0].y, x+dirPts[1].x, y+dirPts[1].y);
		// direction angle
		sprintf(str,"%.1fdg",ofRadToDeg(dirAngle));
		ofDrawBitmapString(str, x + dirPts[0].x, y + dirPts[0].y);
	}
}
