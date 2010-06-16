/*
 *  ofx3D.h
 *  ofxRoger
 *
 *  Created by Roger on 23/04/10.
 *  Copyright 2010 Studio Avante. All rights reserved.
 *
 */

//////////////////////////////////////////////
//
// CUBE
//
// vertex counts
#define RECT_VERTEX				4
#define RECT_BORDER_VERTEX		5
// cube data
#define CUBE_VERTEX				8
#define CUBE_FACES				6
#define CUBE_INDEX				(FACE_COUNT*RECT_VERTEX)
#define CUBE_BORDER_INDEX		(FACE_COUNT*RECT_BORDER_VERTEX)
#define CUBE_ELEMENTS			FACE_COUNT
// Cube strip
#define CUBE_STRIP_VERTEX		10
// Index of a vertex from face F and vertex V
#define CUBE_VERTEX_INDEX(f,v)	(((f)*RECT_VERTEX)+(v))

// Faces de um cubo
// Deve estar alinhado com cubeVertex[]
typedef enum {
	FACE_NONE = -1,
	FACE_UP,
	FACE_DOWN,
	FACE_LEFT,
	FACE_RIGHT,
	FACE_BACK,
	FACE_FRONT,
	FACE_COUNT
} enumCubeFaces;






//////////////////////////////////////////////
//
// FUNCTIONS
//

void ofx3DCube(float v[3], float scale=1.0);
void ofx3DCube(float x, float y, float z, float scale=1.0);
void ofx3DBox(float v[3], float w, float h, float d);
void ofx3DBox(float x, float y, float z, float w, float h, float d);

void ofx3DSquare(float v[3], float scale);
void ofx3DSquare(float x, float y, float z, float scale);
void ofx3DRect(float v[3], float w, float h);
void ofx3DRect(float x, float y, float z, float w, float h);




