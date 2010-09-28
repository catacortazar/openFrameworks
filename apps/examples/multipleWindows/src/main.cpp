#include "ofMain.h"
#include "ofAppGlutWindow.h"
#include "testApp.h"
#include "windowApp.h"

//========================================================================
int main( ){

    ofAppGlutWindow window;
	ofSetupOpenGL(&window, 400,200, OF_WINDOW);			// <-------- setup the GL context
	ofSetWindowPosition(0,0);
	//ofHideWindowBorders();	// not working yet for main window!

	// Additional windows
	ofSetupWindow(new windowApp( 0 ), 200,600, OF_WINDOW);
	ofSetWindowPosition(400,0);
	//ofHideWindowBorders();
	
	// Additional windows
	ofSetupWindow(new windowApp( 1 ), 500,200, OF_WINDOW);
	ofSetWindowPosition(600,100);
	ofHideWindowBorders();
	
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp());


}
