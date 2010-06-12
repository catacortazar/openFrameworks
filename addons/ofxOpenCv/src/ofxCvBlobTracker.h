/*
 * ofxCvBlobTracker.h
 * openFrameworks
 *
 * Blob tracker
 * 
 * Basically it will save al the blobs and track them between frames
 * It's useful when you need to know what's happening to blobs
 *
 * We push all the current blobs to the tracker each frame
 * The tracker will align the new blobs with the last frame based on size and position
 * Each blob has an int called "id" that will be maintained from frame to frame
 * So to keep track of the blobs you call them their ID.
 * PS: Maybe there is an OpenCV solution for that?
 * 
 */

#ifndef OFX_CV_BLOB_TRACKER_H
#define OFX_CV_BLOB_TRACKER_H

#include "ofxCvConstants.h"
#include "ofxCvBlob.h"

#define CHAIN_SIZE		20

#define BLOBS			vector<ofxCvBlob>

class ofxCvBlobTracker {
	
public:
	
	int				sizeMax;
	int				size;

	ofxCvBlobTracker( int sz=CHAIN_SIZE );
	
	
	void push(BLOBS b);
	
	BLOBS getBlobs( int at=0 );
	
	int getBlobIndex( int id, int at=0 );
	
	ofxCvBlob getBlob( int ix, int at=0 );


private:
	
	vector<BLOBS>	blobChain;

	int getChainIndex( int at=0 );
	
	void align( BLOBS *b0 );
	
	int newId( BLOBS b );

};

#endif
