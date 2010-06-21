/*
* ofxCvContourFinder.h
*
* Finds white blobs in binary images and identifies
* centroid, bounding box, area, length and polygonal contour
* The result is placed in a vector of ofxCvBlob objects.
*
*/

#ifndef OFX_CV_CONTOUR_FINDER
#define OFX_CV_CONTOUR_FINDER



#include "ofxCvConstants.h"
#include "ofxCvBlob.h"
#include "ofxCvGrayscaleImage.h"
#include <algorithm>

class ofxCvContourFinder : public ofBaseDraws {

  public:
  
    vector<ofxCvBlob>  blobs;
    int                nBlobs;    // DEPRECATED: use blobs.size() instead
      

    ofxCvContourFinder();
    virtual  ~ofxCvContourFinder();
    
	virtual float getWidth() { return _width; };    //set after first findContours call
	virtual float getHeight() { return _height; };  //set after first findContours call
    
    virtual int  findContours( ofxCvGrayscaleImage& input,
                               int minArea, int maxArea,
                               int nConsidered, bool bFindHoles,
                               bool bUseApproximation = true);
                               // approximation = don't do points for all points 
                               // of the contour, if the contour runs
                               // along a straight line, for example...
	
	void enableContourDrawing(unsigned char color,
							  unsigned char holeColor,
							  int maxLevel,
							  int thickness = 1,				// opencv default
							  int lineType = 8,					// opencv default
							  ofPoint offset = ofPoint(0,0));	// opencv default
	virtual void disableContourDrawing();
	virtual ofxCvGrayscaleImage getContourDrawing();

    virtual void  draw() { draw(0,0, _width, _height); };
    virtual void  draw( float x, float y ) { draw(x,y, _width, _height); };
    virtual void  draw( float x, float y, float w, float h );
	virtual void setAnchorPercent(float xPct, float yPct);
    virtual void setAnchorPoint(int x, int y);
	virtual void resetAnchor();      
    //virtual ofxCvBlob  getBlob(int num);



  protected:

    int  _width;
    int  _height;
    ofxCvGrayscaleImage     inputCopy;
    CvMemStorage*           contour_storage;
    CvMemStorage*           storage;
    CvMoments*              myMoments;
    vector<CvSeq*>          cvSeqBlobs;  //these will become blobs
    
	bool					bDrawContour;			// should draw contours on findContours() ?
    ofxCvGrayscaleImage     contourDrawing;			// the image drawn on the last call to findContours()
	CvScalar				contourColor;			// paint contour with this color
	CvScalar				contourHoleColor;		// paint holes with this color
	int						contourMaxLevel;		// ??
	int						contourThickness;		// CV_FILLED(-1) or the line thickness
	int						contourLineType;		// 8=aliased, 4=blocky
	CvPoint					contourOffset;			// something to do with ROI
	
    ofPoint  anchor;
    bool  bAnchorIsPct;      

    virtual void reset();

};



#endif
