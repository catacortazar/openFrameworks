#ifndef _WINDOW_APP
#define _WINDOW_APP

#include "ofMain.h"

class windowApp : public ofWindowApp{
	
public:

	windowApp(int _n) : ofWindowApp() { number = _n; }
	
	void setup();
	void update();
	void draw();
	
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	
	void windowResized(int w, int h);
	
	int number;
};

#endif
