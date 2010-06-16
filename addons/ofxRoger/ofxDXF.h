/*
 *  ofxDXF.h
 *  Digitallique
 *
 *  Created by Roger on 22/05/10.
 *  Copyright 2010 Studio Avante. All rights reserved.
 *
 */
#pragma once

#include "ofMain.h"
#include "Tri.h"
#include <iostream>
#include <fstream>

class ofxDXF {
public:
	
	ofxDXF();
	~ofxDXF();
	
	bool feedTris(vector<Tri> *t, ofPoint *r=NULL);
	bool feedJoints(vector<ofPoint> *p, ofPoint *r=NULL);
	
	bool exportDXF(const char *filename, const char *comm=NULL);
	bool exportPython(const char *filename, const char *comm=NULL);
	
private:
	
	ofstream		stream;				// C++ file stream
	char			comment[64];		// Comments
	
	char			creationDate[32];	// Current date/time
	ofPoint			min, max;			// min / max points
	ofVolume		bounds;				// Bounds
	vector<Tri>		tris;				// parsed tris
	vector<ofPoint>	joints;				// parsed joints

	// Feeding
	void checkBounds(ofPoint *p);

	// Exporting
	void makeDXF();
	void makePython();

};











