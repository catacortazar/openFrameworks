
#include "testApp.h"

enum enumLayers
{
	LAYER_0,
	LAYER_1,
	LAYER_2,
	LAYER_FINAL,
	LAYER_COUNT
};

//////////////////////////////////////////////
//
// CUBE VERTEXES
//
// up: v1 v3  down: v5 v7
//     v0 v2        v4 v6
const ofPoint cubeVertex[] = {
// up
ofPoint(-0.5f, -0.5f,  0.5f),	// 0
ofPoint(-0.5f,  0.5f,  0.5f),	// 1
ofPoint( 0.5f, -0.5f,  0.5f),	// 2
ofPoint( 0.5f,  0.5f,  0.5f),	// 3
// down
ofPoint(-0.5f, -0.5f, -0.5f),	// 4
ofPoint(-0.5f,  0.5f, -0.5f),	// 5
ofPoint( 0.5f, -0.5f, -0.5f),	// 6
ofPoint( 0.5f,  0.5f, -0.5f)	// 7
};
const int cubeIndex[] = {
0, 1, 2, 3,		// up
4, 5, 6, 7,		// down
5, 1, 4, 0,		// left
6, 2, 7, 3,		// right
4, 0, 6, 2,		// back
7, 3, 5, 1		// front
};

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetDataPathRoot("data/");
	
	// Init
	screen.set(ofGetWidth(), ofGetHeight());
	blendLayers[0] = 1;
	blendLayers[1] = 1;
	blendLayers[2] = 1;
	blendLayers[3] = 0;
	layer = LAYER_FINAL;
	paused = false;
	ang = 0.0f;
	
	//
	// Setup OpenGL
	ofSetBackgroundAuto(false);		// erase manually
	ofEnableSmoothing();
	ofDisableArbTex();
	ofDisableTextureEdgeHack();		// solves ofTexture minor scale/blur
	ofFill();
	
	//
	// Setup shaders
	fxBlend.loadShader("fxBlend");
	
	//
	// Setup textures
	texLayer0.allocate(screen.x, screen.y, GL_RGBA);
	texLayer1.allocate(screen.x, screen.y, GL_RGBA);
	texLayer2.allocate(screen.x, screen.y, GL_RGBA);
	texFinal.allocate(screen.x, screen.y, GL_RGBA);
	
}

//--------------------------------------------------------------
void testApp::update(){
	if (!paused)
	{
		ang += 0.01f;
		if (ang > TWO_PI)
			ang -= TWO_PI;
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	
	//
	// Render layers
	this->drawLayer(LAYER_0);
	texLayer0.loadScreenData(0, 0, screen.x, screen.y);
	this->drawLayer(LAYER_1);
	texLayer1.loadScreenData(0, 0, screen.x, screen.y);
	this->drawLayer(LAYER_2);
	texLayer2.loadScreenData(0, 0, screen.x, screen.y);
	
	//
	// Apply BLEND3
	fxBlend.setShaderActive(true);
	fxBlend.setSampler("tex0", 0, &texLayer0);					// sampler2D on texture unit 0
	fxBlend.setSampler("tex1", 1, &texLayer1);					// sampler2D on texture unit 1
	fxBlend.setSampler("tex2", 2, &texLayer2);					// sampler2D on texture unit 2
	fxBlend.setUniform("layers", (long*)&blendLayers, 4);		// ivec4
	fxBlend.apply(&texFinal);		// apply to FBO
	//fxBlend.apply();				// apply to screen then grab it
	//texFinal.loadScreenData(0, 0, screen.x, screen.y);
	fxBlend.setShaderActive(false);

	//
	// COMPOSE SCENE
	ofBackground(0,0,0);
	ofSetColor(255,255,255,255);
	ofFill();

	//
	// Draw current layer
	// PRESS any key to switch layer
	string desc = "none";
	switch (layer)
	{
		case LAYER_0:
			texLayer0.draw(0.0f, 0.0f);
			desc = "1";
			break;
		case LAYER_1:
			texLayer1.draw(0.0f, 0.0f);
			desc = "2";
			break;
		case LAYER_2:
			texLayer2.draw(0.0f, 0.0f);
			desc = "3";
			break;
		case LAYER_FINAL:
			texFinal.draw(0.0f, 0.0f);
			desc = "Final";
			break;
	}
	
	// draw desc
	char fps[10];
	sprintf(fps,"%.1f",ofGetFrameRate());
	desc = "Layer: " + desc + " (press K/L to switch, 1/2/3 to turn on/off, SPACE to pause) FPS: " + fps;
	ofDrawBitmapString(desc, 10.0f, screen.y-15.0f);
}


//--------------------------------------------------------------
void testApp::drawLayer(int layer){
	
	ofBackground(0,0,0);
	
	glPushMatrix();
	
	glTranslatef(screen.x/2.0f, screen.y/2.0f, 0.0f);

	float sz = screen.y*0.25f;
	
	switch (layer)
	{
		case LAYER_0:
			ofSetColor(255,0,0);
			glTranslatef(sz*sin(ang),sz*cos(ang),0.0f);
			glRotatef(ofRadToDeg(ang), 1.0, 1.0, 0.0);
			break;
		case LAYER_1:
			ofSetColor(0,255,0);
			glTranslatef(0.0f,sz*sin(ang+PI),sz*cos(ang+PI));
			glRotatef(ofRadToDeg(ang), 0.0, 1.0, 1.0);
			break;
		case LAYER_2:
			ofSetColor(0,0,255);
			glTranslatef(sz*sin(ang+HALF_PI),0.0f,sz*cos(ang+HALF_PI));
			glRotatef(ofRadToDeg(ang), 1.0, 0.0, 1.0);
			break;
	};
	
	glScalef(sz*1.5f,sz*1.5f,sz*1.5f);

	// Draw cube
	for (int f = 0 ; f < 6 ; f++)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for (int v = 0 ; v < 4 ; v++)
		{
			int i = cubeIndex[f*4+v];
			glVertex3f(cubeVertex[i].x, cubeVertex[i].y, cubeVertex[i].z);
		}
		glEnd();
	}
	
	glPopMatrix();
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){
	
	switch (key) {
		case '1':
			blendLayers[0] = !blendLayers[0];
			break;
		case '2':
			blendLayers[1] = !blendLayers[1];
			break;
		case '3':
			blendLayers[2] = !blendLayers[2];
			break;
		case ' ':
			paused = !paused;
			break;
		case 'k':
		case 'K':
			layer--;
			if (layer < 0)
				layer = LAYER_COUNT-1;
			break;
		case 'l':
		case 'L':
			layer++;
			if (layer == LAYER_COUNT)
				layer = 0;
			break;
	}
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

