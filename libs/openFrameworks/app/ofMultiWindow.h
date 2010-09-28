/*
 *  ofMultiWindow.h
 *  openFrameworksLib
 *
 *  Created by Roger on 27/09/10.
 *
 */
#pragma once

#include "ofBaseApp.h"

#define OF_MAX_WINDOWS			3

// globals
extern ofAppGlutWindow		*windows[OF_MAX_WINDOWS];
extern int					windowCount;

// secondary window pseudo-app
class ofWindowApp : public ofBaseApp {
public:
	ofWindowApp() : ofBaseApp()
	{
		ofAddListener(ofEvents.setup, this, &ofWindowApp::setup);
		//ofAddListener(ofEvents.update, this, &ofWindowApp::update);
		//ofAddListener(ofEvents.draw, this, &ofWindowApp::draw);
	}
	virtual ~ofWindowApp(){}
	
	virtual void setup(){}
	virtual void update(){}
	virtual void draw(){}
	
	virtual void mouseMoved( int x, int y ){}
	virtual void mouseDragged( int x, int y, int button ){}
	virtual void mousePressed( int x, int y, int button ){}
	virtual void mouseReleased(int x, int y, int button ){}
	
	virtual void windowResized(int w, int h){}
	
	// Current window pointer
	ofAppGlutWindow	*win;
	
private:
	void setup(ofEventArgs &e) { win->bind(); this->setup(); };
	//void update(ofEventArgs &e) { win->bind(); this->update(); };
	//void draw(ofEventArgs &e) { win->bind(); this->draw(); };
	
};


// Callback signatures
typedef void(*glut_cb_void)();
typedef void(*glut_cb_int2)(int,int);
typedef void(*glut_cb_int4)(int,int,int,int);

// Calbacks
void		null_cb_void(void);
void		null_cb_int2(int a, int b);
void		null_cb_int4(int a, int b, int c, int d);

glut_cb_void	get_idle_cb(int _ix);
void			idle_cb_0(void);

glut_cb_void	get_display_cb(int _ix);
void			display_cb_0(void);

glut_cb_int2	get_resize_cb(int _ix);
void			resize_cb_0(int w, int h);
void			resize_cb_1(int w, int h);
void			resize_cb_2(int w, int h);

glut_cb_int4	get_mouse_cb(int _ix);
void			mouse_cb_0(int button, int state, int x, int y);
void			mouse_cb_1(int button, int state, int x, int y);
void			mouse_cb_2(int button, int state, int x, int y);

glut_cb_int2	get_motion_cb(int _ix);
void			motion_cb_0(int x, int y);
void			motion_cb_1(int x, int y);
void			motion_cb_2(int x, int y);

glut_cb_int2	get_passive_motion_cb(int _ix);
void			passive_motion_cb_0(int x, int y);
void			passive_motion_cb_1(int x, int y);
void			passive_motion_cb_2(int x, int y);

void			keyboard_cb_main(unsigned char key, int x, int y);
void			keyboard_up_cb_main(unsigned char key, int x, int y);
void			special_key_cb_main(int key, int x, int y);
void			special_key_up_cb_main(int key, int x, int y);


