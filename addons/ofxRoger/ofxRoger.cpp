/*
 *  ofxRoger.cpp
 *  MappingBuda
 *
 *  Created by Roger on 23/04/10.
 *  Copyright 2010 Studio Avante. All rights reserved.
 *
 */

#include "ofxRoger.h"
#include "ofx3DUtils.h"


////////////////////////////////////////////////////////////////
//
// 3D Geometry
// 
void draw_quad_tex(float offX, float offY, float offZ, float sizeX, float sizeY)
{
	draw_quad(offX, offY, sizeX, sizeY, 1, true);
}
void draw_quad(float offX, float offY, float offZ, float sizeX, float sizeY, int divs, bool textured)
{
    float px, py, pz;
    float szx = (sizeX / (float)divs);
    float szy = (sizeY / (float)divs);
    for (int x = 0 ; x < divs ; x++)
    {
		for (int y = 0 ; y < divs ; y++)
		{
			// calc
			px = offX + ((float)x / (divs+1.0)) * sizeX;
			py = offY + ((float)y / (divs+1.0)) * sizeY;
			pz = offZ;
			// draw
			glBegin(GL_TRIANGLE_STRIP);
			glVertex3f(px, py, 0.0);
			glVertex3f(px, py+szy, 0.0);
			glVertex3f(px+szx, py, 0.0);
			glVertex3f(px+szx, py+szy, 0.0);
			if (textured)
			{
				glTexCoord2f(0.0, 0.0);
				glTexCoord2f(0.0, 1.0);
				glTexCoord2f(1.0, 0.0);
				glTexCoord2f(1.0, 1.0);
			}
			glEnd();
		}
    }
}
// Draw box, centered
void draw_box(float offX, float offY, float offZ, float sizeX, float sizeY, float sizeZ, int divs)
{
    float px, py, pz;
    float szx = (sizeX / (float)divs);
    float szy = (sizeY / (float)divs);
    for (int x = 0 ; x < divs ; x++)
    {
		for (int y = 0 ; y < divs ; y++)
		{
			// calc
			px = offX + (szx/2.0) + ((float)x / divs) * sizeX;
			py = offY + (szy/2.0) + ((float)y / divs) * sizeY;
			pz = offZ;
			// draw
			ofxBox(px, py, pz, szx, szy, sizeZ);
		}
    }
}


//
// Draw FPS on screen
void draw_fps(bool filled)
{
	char str[20];
	sprintf(str, "fps [%.2f]", ofGetFrameRate());
	ofFill();
	ofSetColor(255,255,255,255);
	ofDrawBitmapString(str, 1.0, ofGetHeight()-5.0, 0.04);
	if (filled)
	{
		ofSetColor(0,0,0,255);
		ofRect(0.0f, ofGetHeight()-15.0, 0.02, 100.0, 15.0);
	}
}


////////////////////////////////////////////////////////////////////////////////
//
// CAPTURE FRAMES
//
//#define CAPTURE_FORMAT	"/Volumes/BROWNIE/@CAPTURE/capture_%06d.jpg"
#define CAPTURE_FORMAT	"/Users/Roger/Desktop/CAPTURE/capture_%06d.jpg"
void capture_frame(int f)
{
	// Capture screen
	char str[100];
	sprintf(str, CAPTURE_FORMAT, f);
	ofSaveScreen(str);
}







