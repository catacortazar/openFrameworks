#pragma once

#include "ofMain.h"
#include "ofShader.h"
#include "ofTextureFBO.h"

class testApp : public ofBaseApp{
	
public:
	
	void setup();
	void update();
	void draw();
	
	void keyPressed  (int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);

	void drawLayer(int layer);

	ofPoint			screen;
	long			blendLayers[4];
	int				layer;
	float			ang;
	bool			paused;
	
	// Shaders
	ofShader		fxBlend;
	ofShader		fxBlur;
	
	// FBOs
	ofTexture		texLayer0;
	ofTexture		texLayer1;
	ofTexture		texLayer2;
	ofTextureFBO	texFinal;
};

