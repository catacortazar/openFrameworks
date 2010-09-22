/*
 *  ofxRoger.cpp
 *  MappingBuda
 *
 *  Created by Roger on 23/04/10.
 *  Copyright 2010 Studio Avante. All rights reserved.
 *
 */

#include "ofMain.h"
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
	sprintf(str, "%.2f", ofGetFrameRate());
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




/////////////////////////////
//
// CHARSET BRASIL
//
// ESTE ARQUIVO E QUALQUER ARQUIVO COM LETRAS BRASILEIRAS DEVEM TER...
// File Encoding = Western (Mac OS Roman)
// (XCode: File > Get Info)
//
unsigned char charBR[][2] =
{
{'á', 225},	// 225	á
{'à', 224},	// 224	à
{'â', 226},	// 226	â
{'ã', 227},	// 227	ã
{'Á', 193},	// 193	Á
{'À', 192},	// 192	À
{'Â', 194},	// 194	Â
{'Ã', 195},	// 195	Ã
{'é', 233},	// 233	é
{'ê', 234},	// 234	ê
{'É', 201},	// 201	É
{'Ê', 202},	// 202	Ê
{'í', 237},	// 237	í
{'Í', 205},	// 205	Í
{'ó', 243},	// 243	ó
{'ô', 244},	// 244	ô
{'õ', 245},	// 245	õ
{'Ó', 211},	// 211	Ó
{'Ô', 212},	// 212	Ô
{'Õ', 213},	// 213	Õ
{'ú', 250},	// 250	ú
{'ü', 252},	// 252	ü
{'Ú', 218},	// 218	Ú
{'Ü', 220},	// 220	Ü
{'ç', 231},	// 231	ç
{'Ç', 199},	// 199	Ç
{'\0', 0}
};

// Retorna a string com os caracteres brasileiros corretos
char* stringBR(char *str)
{
	static char strBR[255];
	unsigned char c, cbr;
	int n, br;
	
	for (n = 0 ; n < strlen(str) ; n++)
	{
		c = str[n];
		// look for char
		for (br = 0 ; charBR[br][0] != '\0' ; br++)
		{
			cbr = charBR[br][0];
			if (c == cbr)
			{
				strBR[n] = charBR[br][1];
				break;
			}
		}
		// not found
		if (charBR[br][0] == '\0')
			strBR[n] = c;
	}
	strBR[n] = '\0';
	return strBR;
}






