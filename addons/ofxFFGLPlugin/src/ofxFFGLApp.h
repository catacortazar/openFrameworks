/*
 *  ofxFFGLApp.h
 *  FFGLExample
 *
 *  Created by ensta on 2/4/10.
 *  Copyright 2010 Daniel Berio. All rights reserved.
 *
 *  http://www.enist.org
 */

#ifndef _OFXFFGLAPP
#define _OFXFFGLAPP

#include "ofMain.h"
#include "ofxFFGLParameter.h"

#define MAX_INPUT_TEXTURES 3

class ofFFGLApp : public ofBaseApp {

	public:
        ofFFGLApp();
		virtual ~ofFFGLApp();
		
		/// override this to do something when a parameter is changed
		virtual void onParameterChanged( ofFFGLParameter * param ) {}
		
		vector < ofFFGLParameter * > parameters;
		
		int	getNumParameters() const { return parameters.size(); }
		
		ofFFGLParameter * getParameter( int index ) { if( index < parameters.size() ) return parameters[index]; else return 0; }
		//ofFFGLParameter * getParameterByName( const char * name );
		
		// These functions add a parameter to the plugin, pass the address of the variable that will be automatically updated.
		// We could add normal FFGL parameter handling but this way even though it is more limited, it makes life much easier
		// Parameters MUST be added in the constructor or FFGL will ignore them.
		
		void addFloatParameter( const char * name, float * address, float min = 0.0f, float max = 1.0f );
		void addBoolParameter( const char * name, bool * address );
		void addStringParameter( const char * name, char * address );
		
		ofTexture * inputTextures[MAX_INPUT_TEXTURES];
		
		
};

#endif
