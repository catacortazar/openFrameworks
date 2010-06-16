/*
 *  ofx3Dcpp
 *  ofxRoger
 *
 *  Created by Roger on 23/04/10.
 *  Copyright 2010 Studio Avante. All rights reserved.
 *
 */

#include "ofx3D.h"
#include "ofMain.h"

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
//
// CUBE INDEX
// Must match enumCubeFaces
const int cubeIndex[] = {
0, 1, 2, 3,		// up
4, 5, 6, 7,		// down
5, 1, 4, 0,		// left
6, 2, 7, 3,		// right
4, 0, 6, 2,		// back
7, 3, 5, 1		// front
};
// Strip around cube in a single GL_TRIANGLE_STRIP 
const int cubeStripIndex[] = {
0, 4, 1, 5, 3, 7, 2, 6, 0, 4
};
//
// CUBE NORMALS
// Must match enumCubeFaces
const ofPoint cubeNormals[] = {
ofPoint( 0.0f,  0.0f,  1.0f),	// up
ofPoint( 0.0f,  0.0f, -1.0f),	// down
ofPoint(-1.0f,  0.0f,  0.0f),	// left
ofPoint( 0.0f,  1.0f,  0.0f),	// right
ofPoint( 0.0f, -1.0f,  0.0f),	// back
ofPoint( 0.0f,  1.0f,  0.0f),	// front
};

//
// Cube / Box
void ofx3DCube(float v[3], float scale)
{
	ofx3DBox(v[0], v[1], v[2], scale, scale, scale);
}
void ofx3DCube(float x, float y, float z, float scale)
{
	ofx3DBox(x, y, z, scale, scale, scale);
}
void ofx3DBox(float v[3], float w, float h, float d)
{
	ofx3DBox(v[0], v[1], v[2], w, h, d);
}
void ofx3DBox(float x, float y, float z, float w, float h, float d)
{
	// Strip-around
	if (false)
	{
		// Up + Down
		for (int f = 0 ; f < 2 ; f++)
		{
			glBegin(GL_TRIANGLE_STRIP);
			glNormal3fv(cubeNormals[f].v);
			for (int v = 0 ; v < RECT_VERTEX ; v++)
			{
				int i = cubeIndex[CUBE_VERTEX_INDEX(f,v)];
				glVertex3f(x+(cubeVertex[i].x*w), y+(cubeVertex[i].y*h), z+(cubeVertex[i].z*d));
			}
			glEnd();
		}
		// Strip
		glBegin(GL_TRIANGLE_STRIP);
		for (int v = 0 ; v < CUBE_STRIP_VERTEX ; v++)
		{
			int i = cubeStripIndex[v];
			glNormal3fv(cubeNormals[i].v);
			glVertex3f(x+(cubeVertex[i].x*w), y+(cubeVertex[i].y*h), z+(cubeVertex[i].z*d));
		}
		glEnd();
	}
	// Face-by-Face
	else
	{
		for (int f = 0 ; f < CUBE_FACES ; f++)
		{
			glBegin(GL_TRIANGLE_STRIP);
			glNormal3fv(cubeNormals[f].v);
			for (int v = 0 ; v < RECT_VERTEX ; v++)
			{
				int i = cubeIndex[CUBE_VERTEX_INDEX(f,v)];
				glVertex3f(x+(cubeVertex[i].x*w), y+(cubeVertex[i].y*h), z+(cubeVertex[i].z*d));
			}
			glEnd();
		}
	}
}




/////////////////////////////////////////////////////////
//
// RECT
// 
// v1 v3
// v0 v2
//
// Rect Vertex
const ofPoint rectVertex[] = {
ofPoint(0.0f, 0.0f, 0.0f),	// 0
ofPoint(0.0f, 1.0f, 0.0f),	// 1
ofPoint(1.0f, 0.0f, 0.0f),	// 2
ofPoint(1.0f, 1.0f, 0.0f)	// 3
};

//
// Rect
void ofx3DSquare(float v[3], float scale)
{
	ofx3DRect(v[0], v[1], v[2], scale, scale);
}
void ofx3DSquare(float x, float y, float z, float scale)
{
	ofx3DRect(x, y, z, scale, scale);
}
void ofx3DRect(float v[3], float w, float h)
{
	ofx3DRect(v[0], v[1], v[2], w, h);
}
void ofx3DRect(float x, float y, float z, float w, float h)
{
	glBegin(GL_TRIANGLE_STRIP);
	glNormal3fv(cubeNormals[0].v);
	for (int v = 0 ; v < RECT_VERTEX ; v++)
		glVertex3f(x+(rectVertex[v].x*w), y+(rectVertex[v].y*h), z);
	glEnd();
}





