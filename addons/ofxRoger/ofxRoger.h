/*
 *  ofxRoger.h
 *  MappingBuda
 *
 *  Created by Roger on 23/04/10.
 *  Copyright 2010 Studio Avante. All rights reserved.
 *
 */


void draw_quad_tex(float offX, float offY, float offZ, float sizeX, float sizeY);
void draw_quad(float offX, float offY, float offZ, float sizeX, float sizeY, int divs=1, bool textured=false);
void draw_box(float offX, float offY, float offZ, float sizeX, float sizeY, float sizeZ, int divs=1);

void draw_fps(bool filled=false);

// Capture
void capture_frame(int f);

// Charset Brasil
char* stringBR(char *str);
