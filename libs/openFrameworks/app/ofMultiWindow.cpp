/*
 *  ofMultiWindow.cpp
 *  openFrameworksLib
 *
 *  Created by Roger Sodre on 27/09/10.
 *
 */

#include "ofAppGlutWindow.h"

// current window from ofAppRunner.cpp
extern ofAppGlutWindow * window;

// globals
ofAppGlutWindow				*windows[OF_MAX_WINDOWS];
int							windowCount = 0;


//------------------------------------------------------------
// null callbacks
void null_cb_void(void) {}
void null_cb_int2(int a, int b) {}
void null_cb_int4(int a, int b, int c, int d) {}

//------------------------------------------------------------
// setup callback: main window call for everybody
// not really a callback, but main window is still the king
void setup_cb_all(void)
{
	for ( int n = 0 ; n < windowCount ; n++ )
		windows[n]->ofAppPtr->setup();
}

//------------------------------------------------------------
// idle callback: main window call for everybody
glut_cb_void get_idle_cb(int _ix) {
	if ( _ix == 0 )
		return idle_cb_all;
	else
		return null_cb_void;
}
void idle_cb_all(void)
{
	for ( int n = 0 ; n < windowCount ; n++ )
		windows[n]->idle_cb();
}

//------------------------------------------------------------
// display callback: main window call for everybody
glut_cb_void get_display_cb(int _ix) {
	if ( _ix == 0 )
		return display_cb_all;
	else
		return null_cb_void;
}
void display_cb_all(void)
{
	for ( int n = 0 ; n < windowCount ; n++ )
		windows[n]->display();
}

//------------------------------------------------------------
// resize callback: every window's got his own
glut_cb_int2 get_resize_cb(int _ix) {
	switch (_ix)
	{
		case 0:
			return resize_cb_0;
		case 1:
			return resize_cb_1;
		case 2:
			return resize_cb_2;
		default:
			return null_cb_int2;
	}
}
void resize_cb_0(int w, int h) { windows[0]->resize_cb(w, h); }
void resize_cb_1(int w, int h) { windows[1]->resize_cb(w, h); }
void resize_cb_2(int w, int h) { windows[2]->resize_cb(w, h); }

//------------------------------------------------------------
// mouse callback: every window's got his own
glut_cb_int4 get_mouse_cb(int _ix) {
	switch (_ix)
	{
		case 0:
			return mouse_cb_0;
		case 1:
			return mouse_cb_1;
		case 2:
			return mouse_cb_2;
		default:
			return null_cb_int4;
	}
}
void mouse_cb_0(int button, int state, int x, int y) { windows[0]->mouse_cb(button, state, x, y); }
void mouse_cb_1(int button, int state, int x, int y) { windows[1]->mouse_cb(button, state, x, y); }
void mouse_cb_2(int button, int state, int x, int y) { windows[2]->mouse_cb(button, state, x, y); }

//------------------------------------------------------------
// motion callback: every window's got his own
glut_cb_int2 get_motion_cb(int _ix) {
	switch (_ix)
	{
		case 0:
			return motion_cb_0;
		case 1:
			return motion_cb_1;
		case 2:
			return motion_cb_2;
		default:
			return null_cb_int2;
	}
}
void motion_cb_0(int x, int y) { windows[0]->motion_cb(x, y); }
void motion_cb_1(int x, int y) { windows[1]->motion_cb(x, y); }
void motion_cb_2(int x, int y) { windows[2]->motion_cb(x, y); }

//------------------------------------------------------------
// passive_motion callback: every window's got his own
glut_cb_int2 get_passive_motion_cb(int _ix) {
	switch (_ix)
	{
		case 0:
			return passive_motion_cb_0;
		case 1:
			return passive_motion_cb_1;
		case 2:
			return passive_motion_cb_2;
		default:
			return null_cb_int2;
	}
}
void passive_motion_cb_0(int x, int y) { windows[0]->passive_motion_cb(x, y); }
void passive_motion_cb_1(int x, int y) { windows[1]->passive_motion_cb(x, y); }
void passive_motion_cb_2(int x, int y) { windows[2]->passive_motion_cb(x, y); }


//------------------------------------------------------------
// Keyboard callbacks: Main window only
void keyboard_cb_main(unsigned char key, int x, int y) { windows[0]->keyboard_cb(key,x,y); }
void keyboard_up_cb_main(unsigned char key, int x, int y) { windows[0]->keyboard_up_cb(key,x,y); }
void special_key_cb_main(int key, int x, int y) { windows[0]->special_key_cb(key,x,y); }
void special_key_up_cb_main(int key, int x, int y) { windows[0]->special_key_up_cb(key,x,y); }


