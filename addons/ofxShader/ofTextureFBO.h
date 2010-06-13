
#pragma once

#include "ofMain.h"

#include <iostream>


class ofTextureFBO : public ofTexture {
public:

	~ofTextureFBO();

	void allocate(int w, int h, int internalGlDataType);
	void allocate(int w, int h, int internalGlDataType, bool bUseARBExtention);
	
	void begin();
	void end();
	
protected:
	
	void allocateFBO();
	bool checkFBO();
	
	GLuint		fbo;			// FBO
	GLuint		depthBuffer;	// depth render buffer

};

