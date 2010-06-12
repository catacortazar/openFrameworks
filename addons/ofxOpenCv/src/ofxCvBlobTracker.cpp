
#include "ofxCvBlobTracker.h"

//----------------------------------------
ofxCvBlobTracker::ofxCvBlobTracker( int sz ) {
	sizeMax = sz;
}

//----------------------------------------
void ofxCvBlobTracker::push(BLOBS b)
{
	// remove old
	if ( blobChain.size() >= sizeMax )
		blobChain.erase( blobChain.begin() );
	// align blobs with last frame
	if (blobChain.size() > 0 && b.size() > 0)
		this->align( &b );
	// push new
	blobChain.push_back( b );
	// update size
	size = blobChain.size();
}

// Get all blobs stored
// at: 0 = latest/cuurent frame, 1 = previous frame, ...
//----------------------------------------
BLOBS ofxCvBlobTracker::getBlobs( int at )
{
	return blobChain.at( this->getChainIndex(at) );
}
int ofxCvBlobTracker::getChainIndex( int at )
{
	int i = blobChain.size() - 1 - at;
	return ( i < 0 ? 0 : i );
}

// Get blob index by ID
// Return -1 if not found
//----------------------------------------
int ofxCvBlobTracker::getBlobIndex( int id, int at )
{
	BLOBS bbs = this->getBlobs( at );
	// find matching id
	for ( int ix = 0 ; ix < bbs.size() ; ix++ )
		if ( bbs[ix].id == id)
			bbs[ix];
	// id not found
	return -1;
}


// Get a blob by its index
// The index must be valid, or else you might get a crash
//----------------------------------------
ofxCvBlob ofxCvBlobTracker::getBlob( int ix, int at )
{
	BLOBS bbs = this->getBlobs( at );
	return bbs[ix];
}



// align current blobs with last frame ones
// this must be done BEFORE pushing the BLOBS
// It will compare the new blobs with the last frame
// and give scores based on its size and position
// Then the new blobs will be aligned with previous ones by that score
//----------------------------------------
void ofxCvBlobTracker::align( BLOBS *b0 )
{
	// get last blob
	BLOBS b1 = this->getBlobs(0);
	
	//printf("ALIGN BLOBS  b0[%d] b1[%d]\n",(*b0).size(),b1.size());
	
	// calc scores
	float scores[b1.size()][(*b0).size()];	// OLD blob scores
	float scoreMax[b1.size()];				// OLD blobs highest scores
	int scoreMaxIx[b1.size()];				// OLD blobs highest scores' NEW id
	int order[b1.size()];					// OLD blobs order (highest to lowest score)
	float max, diff;
	// for each OLD blob...
	for ( int n1 = 0 ; n1 < b1.size() ; n1++ )
	{
		// reset OLD blob max score / order
		scoreMax[n1] = -1.0;
		order[n1] = n1;
		// ...calc the score to each NEW blob
		for ( int n0 = 0 ; n0 < (*b0).size() ; n0++ )
		{
			// reset score
			scores[n1][n0] = 0.0;
			// center proximity (0.0 - 1.0)
			max = 100.0;
			diff = ofDist((*b0)[n0].centroid.x, (*b0)[n0].centroid.y, b1[n1].centroid.x, b1[n1].centroid.y);
			scores[n1][n0] += ( diff > max ? 0.0 : ( 1.0 - (diff / max)) ) * 2.0;
			// area (0.0 -1.0)
			max = b1[n1].perim * 2.0;
			diff = fabs(b1[n1].perim - (*b0)[n0].perim);
			scores[n1][n0] += ( diff > max ? 0.0 : ( 1.0 - (diff / max)) );
			// new high score?
			if (scores[n1][n0] > scoreMax[n1])
			{
				scoreMaxIx[n1] = n0;
				scoreMax[n1] = scores[n1][n0];
			}
		}
	}
	//for ( int n1 = 0 ; n1 < b1.size() ; n1++ )
	//	printf("ALIGN ORIG   b1[%d] b0[%d]=[%.3f] id[%d]\n",n1,scoreMaxIx[n1],scoreMax[n1],b1[n1].id);
	
	// Sort blobs (highest to lowest score)
	for ( int n1 = b1.size()-1 ; n1 > 0 ; n1-- )
	{
		for ( int n = n1 ; n > 0 ; n-- )
		{
			if ( scoreMax[order[n]] < scoreMax[order[n-1]] )
			{
				int i = order[n];
				order[n] = order[n-1];
				order[n-1] = i;
			}
		}
	}
	//for ( int n1 = 0 ; n1 < b1.size() ; n1++ )
	//	printf("ALIGN SORT   b1[%d] b0[%d]=[%.3f] id[%d]\n",n1,scoreMaxIx[n1],scoreMax[n1],b1[n1].id);
	
	// reset current frame ids
	for ( int n0 = 0 ; n0 < (*b0).size() ; n0++ )
		(*b0)[n0].id = -1;
	
	// find current highest OLD blob with valid id
	for ( int n = 0 ; n < b1.size() ; n++ )
	{
		int n1 = order[n];
		// no score? no one to pass the id to :(
		if (scoreMax[n1] == 0.0)
			continue;
		// assign id
		int n0 = scoreMaxIx[n1];
		//printf("ALIGN GIVE > b0[%d] = b1[%d] = id[%d]\n",n0,n1,b1[n1].id);
		(*b0)[n0].id = b1[n1].id;
	}
	
	// create new ids if needed
	for ( int n0 = 0 ; n0 < (*b0).size() ; n0++ )
		if ((*b0)[n0].id == -1)
			(*b0)[n0].id = newId( (*b0) );
}

// create new blob id
//----------------------------------------
int ofxCvBlobTracker::newId( BLOBS b )
{
	int id = 0;
	for ( int n = 0 ; n < b.size() ; n++ )
	{
		if ( b[n].id == id)
		{
			id++;		// try new next id
			n = -1;		// restart
			continue;
		}
	}
	return id;
}


