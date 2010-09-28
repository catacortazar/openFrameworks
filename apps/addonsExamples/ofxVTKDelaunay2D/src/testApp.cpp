
#include "testApp.h"
#include "vtkMath.h"

//--------------------------------------------------------------
void testApp::setup(){
	bPaused = false;
	bUpToDate = false;
	bInvertBoundary = true;
}

//--------------------------------------------------------------
void testApp::update(){

	if (bPaused && bUpToDate == true)
		return;
	bUpToDate = true;

	// empty mesh
	delaunay.reset();
	
	// Generate a 10 x 10 grid of points
	for(unsigned int x = 0; x < 10; x++)
    {
		for(unsigned int y = 0; y < 10; y++)
		{
			delaunay.addPoint(x + vtkMath::Random(-0.15f, 0.15f),
							  y + vtkMath::Random(-0.15f, 0.15f),
							  0.0f);
		}
	}
	
	// Define boundary
	int b[10] = { 22, 23, 24, 25, 35, 45, 44, 43, 42, 32 };
	// Counter-Clockwise: will make a hole
	if (bInvertBoundary)
		for (int n = 0 ; n < 10 ; n++)
			delaunay.setBoundaryPoint(b[n]);
	// Clockwise: will define a boundary
	else
		for (int n = 10-1 ; n >= 0 ; n--)
			delaunay.setBoundaryPoint(b[n]);

	// Find mesh
	delaunay.update();

}

//--------------------------------------------------------------
void testApp::draw(){

	ofBackground( 0x000000);
	ofSetColor( 0xFFFFFF );
	ofNoFill();
	
	// draw mesh
	glPushMatrix();
	glTranslatef(100.0, 100.0, 0.0);
	glScalef(60.0, 60.0, 60.0);
	delaunay.draw();
	glPopMatrix();
	
	// draw fps
	char str[255];
	sprintf(str, "press space to pause / 'i' to invert boundary / fps [%.2f]", ofGetFrameRate());
	ofDrawBitmapString(str, 1.0, ofGetHeight()-5.0);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	switch (key)
	{
		case ' ':
			bPaused = !bPaused;
			break;
		case 'i':
		case 'I':
			bInvertBoundary = !bInvertBoundary;
			bUpToDate = false;
			break;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

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

