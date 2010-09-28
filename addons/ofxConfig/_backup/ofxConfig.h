/*
 *  ofxConfig.h
 *
 *  Created by Roger Sodre on 08/04/10.
 *  Copyright 2010 Studio Avante. All rights reserved.
 *
 */
#pragma once

////////////////////////////////////////
//
// CUSTOM SETUP
//
//#define CINDER





////////////////////////////////////////
//
// CONFIG FOR CINDER
//
#ifdef CINDER
#include "cinder/Cinder.h"
#include "cinder/app/App.h"
#include "cinder/Vector.h"
#include "cinder/Utilities.h"
using namespace ci;
using namespace ci::app;
using namespace std;
// Cinder Touch
#ifndef CINDER_COCOA_TOUCH
#define CFG_USE_OFXOSC
#endif
// mimic OF types
typedef cinder::Vec3f ofPoint;
typedef cinder::Color8u ofColor;
//
// CONFIG FOR OPENFRAMEWORKS
//
#else
#include "ofMain.h"
#define CFG_USE_OFXMIDI
#define CFG_USE_OFXOSC
#define CFG_CATCH_LOOP_EVENTS
#endif

//
// Config OSC
#ifdef CFG_USE_OFXOSC
#ifdef CINDER
#include "OscListener.h"
#include "OscSender.h"
using namespace osc;
typedef osc::Sender ofxOscSender;
typedef osc::Listener ofxOscReceiver;
typedef osc::Message ofxOscMessage;
#else
#include "ofxOsc.h"
#endif
#endif
// OSC Defaults
#define DEFAULT_OSC_PORT		1122
#define DEFAULT_OSC_HOST		"localhost"

//
// MISC
#define	CFG_MAX_NAME_LEN		64
#define	CFG_MAX_DATA_LEN		256
#define FLOAT_VEC				vector<float>

//
// Types
enum enumConfigTypes
{
	CFG_TYPE_NONE = -1,	
	CFG_TYPE_FLOAT,
	CFG_TYPE_DOUBLE,
	CFG_TYPE_INTEGER,
	CFG_TYPE_LONG,
	CFG_TYPE_BOOLEAN,
	CFG_TYPE_BYTE,
	CFG_TYPE_STRING,
	CFG_TYPE_COLOR,
	CFG_TYPE_VECTOR,
	CFG_TYPE_COUNT
};

////////////////////////////////////////////////////////////////////////
//
// CONFIG VALUE
//
class ofxConfigValue {
public:
	bool	preserveProg;		// priority is prog
	short	midiNote;
	
	// Setters
	void set(float v)
	{
		value = v;
		if (min != max)		// avoid div by zero
			prog = (value-min) / (max-min);
		else
			prog = 0.0f;
	}
	void setProg(float p)
	{
		prog = p;
		value = min + ((max-min) * prog);
	}
	void setLimits(float vmin, float vmax)
	{
		min = vmin;
		max = vmax;
		if (preserveProg)
			this->setProg(prog);	// re-calc value
		else
			this->set(value);	// re-calc prog
	}
	
	// Getters
	float get() { return value; }
	float getProg() { return prog; }
	float getMin() { return min; }
	float getMax() { return max; }
	
	// Operations
	void invert()
	{
		this->set(max-value);
	}
	void sub(float v=1.0f, bool clamp=false)
	{
		this->set(value-v);
		if (clamp)
			this->clamp();
	}
	void add(float v=1.0f, bool clamp=false)
	{
		this->set(value+v);
		if (clamp)
			this->clamp();
	}
	void clamp()
	{
		if (value < min)
			this->set(min);
		else if (value > max)
			this->set(max);
	}
	
private:
	float	value;
	float	prog;
	float	min;
	float	max;
};

////////////////////////////////////////////////////////////////////////
//
// CONFIG PARAMETER
//
class ofxConfigParam {
public:
	short	type;
	char	name[CFG_MAX_NAME_LEN];
	// Values
	char	string[CFG_MAX_DATA_LEN];
    union {	// share same memory, so vector[0] = value
		struct {
			ofxConfigValue value, dummy[2];
		};
		ofxConfigValue vec[3];
    };
	// GUI
	void *picker;
	
	ofxConfigParam(short t, const char *nm)
	{
		type = t;
		strcpy(name, nm);
		picker = NULL;
		// clean values here because union members cannot have constructors
		for (int n = 0 ; n < 3 ; n++)
		{
			vec[n].set(0.0f);
			vec[n].setLimits(0.0f,0.0f);
			vec[n].preserveProg = false;
			vec[n].midiNote = -1;
		}
	}
};

////////////////////////////////////////////////////////////////////////
//
// CONFIG CLASS
//
#ifdef CFG_USE_OFXMIDI
#include "ofxMidi.h"
class ofxConfig : public ofxMidiListener {
#else
	class ofxConfig {
#endif
	public:
		ofxConfig();
		~ofxConfig();
		
		virtual void update();			// Update state from OSC
		virtual void draw() {};			// Draw GUI
		
		char errmsg[256];
		
		// MIDI
		void setMidi(int id, short note);
		void setMidiLimits(int id, short note, float vmin, float vmax);
		void setMidiMulti(int id, short note0, short note1=-1, short note2=-1);
		
#ifdef CFG_USE_OFXOSC
		void setOscSend(bool b);		// Start to send OSC
		void setOscReceive(bool b);		// Start ro receive OSC
		void pushOsc();					// Seld all params by OSC
#endif
		
		// New parameters
		void add(short id, const char *name, float val);				// Generic = float
		void addFloat(short id, const char *name, float val);
		void addDouble(short id, const char *name, double val);
		void addInt(short id, const char *name, int val);
		void addLong(short id, const char *name, long val);
		void addBool(short id, const char *name, bool val);
		void addByte(short id, const char *name, unsigned char val);
		void addString(short id, const char *name, char *val);
#ifndef CINDER
		void addColor(short id, const char *name, int hex);
#endif
		void addColor(short id, const char *name, ofColor p);
		void addColor(short id, const char *name, float r, float g, float b);
		void addVector(short id, const char *name, ofPoint p);
		void addVector(short id, const char *name, float x, float y, float z=0.0f);
		
		// Setters
		void setLimits(int id, float vmin, float vmax);
		void setLimitsR(int id, float vmin, float vmax);
		void setLimitsG(int id, float vmin, float vmax);
		void setLimitsB(int id, float vmin, float vmax);
		void setLimitsX(int id, float vmin, float vmax);
		void setLimitsY(int id, float vmin, float vmax);
		void setLimitsZ(int id, float vmin, float vmax);
		// Setters
		void preserveProg(int id, bool p);
		void setProg(int id, float p);
		void setProgR(int id, float p);
		void setProgG(int id, float p);
		void setProgB(int id, float p);
		void setProgX(int id, float p);
		void setProgY(int id, float p);
		void setProgZ(int id, float p);
		void setProg(int id, float val0, float val1, float val2=0.0f);	// CFG_TYPE_VECTOR / CFG_TYPE_COLOR
		// Set values (generic = float)
		void set(int id, float val);				// CFG_TYPE_FLOAT (generic)
		void setR(int id, float val);
		void setG(int id, float val);
		void setB(int id, float val);
		void setX(int id, float val);
		void setY(int id, float val);
		void setZ(int id, float val);
		// Set values (specific)
		void set(int id, double val);						// CFG_TYPE_DOUBLE
		void set(int id, short val);						// no type, just cast
		void set(int id, int val);							// CFG_TYPE_INTEGER
		void set(int id, long val);							// CFG_TYPE_LONG
		void set(int id, bool val);							// CFG_TYPE_BOOLEAN
		void set(int id, unsigned char val);				// CFG_TYPE_BYTE
		void set(int id, const char *val, bool pp=false);	// CFG_TYPE_STRING, or convert to float
		void set(int id, ofColor c);						// CFG_TYPE_COLOR
		void set(int id, ofPoint p);						// CFG_TYPE_VECTOR
		void set(int id, float val0, float val1, float val2=0.0f);	// CFG_TYPE_VECTOR / CFG_TYPE_COLOR
		
		// Getters
		char* getName(int id);
		short getType(int id);
		// Get limits
		float getMin(int id);
		float getMinR(int id);
		float getMinG(int id);
		float getMinB(int id);
		float getMinX(int id);
		float getMinY(int id);
		float getMinZ(int id);
		float getMax(int id);
		float getMaxR(int id);
		float getMaxG(int id);
		float getMaxB(int id);
		float getMaxX(int id);
		float getMaxY(int id);
		float getMaxZ(int id);
		float getProg(int id);
		float getProgR(int id);
		float getProgG(int id);
		float getProgB(int id);
		float getProgX(int id);
		float getProgY(int id);
		float getProgZ(int id);
		ofPoint getProgVector(int id);
		// get values (generic = float)
		float get(int id);
		float getR(int id);
		float getG(int id);
		float getB(int id);
		float getX(int id);
		float getY(int id);
		float getZ(int id);
		// get values (specific
		float getFloat(int id);
		double getDouble(int id);
		short getShort(int id);
		int getInt(int id);
		long getLong(int id);
		bool getBool(int id);
		unsigned char getByte(int id);
		void getString(int id, char *str, bool raw=false);
		char* getString(int id, bool raw=false);
		ofColor getColor(int id);
		ofPoint getVector(int id);
		
		//
		// Operations
		void invert(int id);
		void invertR(int id);
		void invertG(int id);
		void invertB(int id);
		void invertX(int id);
		void invertY(int id);
		void invertZ(int id);
		void sub(int id, float val, bool clamp=false);
		void subX(int id, float val, bool clamp=false);
		void subY(int id, float val, bool clamp=false);
		void subZ(int id, float val, bool clamp=false);
		void subR(int id, float val, bool clamp=false);
		void subG(int id, float val, bool clamp=false);
		void subB(int id, float val, bool clamp=false);
		void add(int id, float val, bool clamp=false);
		void addX(int id, float val, bool clamp=false);
		void addY(int id, float val, bool clamp=false);
		void addZ(int id, float val, bool clamp=false);
		void addR(int id, float val, bool clamp=false);
		void addG(int id, float val, bool clamp=false);
		void addB(int id, float val, bool clamp=false);
		void dec(int id, bool clamp=false);
		void decX(int id, bool clamp=false);
		void decY(int id, bool clamp=false);
		void decZ(int id, bool clamp=false);
		void decR(int id, bool clamp=false);
		void decG(int id, bool clamp=false);
		void decB(int id, bool clamp=false);
		void inc(int id, bool clamp=false);
		void incX(int id, bool clamp=false);
		void incY(int id, bool clamp=false);
		void incZ(int id, bool clamp=false);
		void incR(int id, bool clamp=false);
		void incG(int id, bool clamp=false);
		void incB(int id, bool clamp=false);
		
		//
		// READ / SAVE to file
		void getFullFilename(const char *filename, char *fullFilename);
		int readFile(const char *filename);
		int saveFile(const char *filename);
		char* getSaveTime();
		//void xmlSave(char *xmlName);
		//void xmlLoad(char *xmlName);
		//void xmlParse(proxml.XMLElement xmlFile);
		
		
	protected:
		// Parameter index
		vector<ofxConfigParam*>	params;
		short		paramCount;
		char		saveTime[32];
		
		// MIDI
		int			midiChannel;
		int			midiPort;
#ifdef CFG_USE_OFXMIDI
		ofxMidiOut	*midiOut;
		ofxMidiIn	*midiIn;
		// ofxMidiListener Virtual
		virtual void newMidiMessage(ofxMidiEventArgs& eventArgs);
#endif
		// used by OSC as well
		void parseMidiMessage(int channel, int note, float prog);
		
		// OSC
#ifdef CFG_USE_OFXOSC
		bool			bOscSend;
		bool			bOscReceive;
		ofxOscSender	oscSender;
		ofxOscReceiver	oscReceiver;
		void switchOsc();					// Switch OSC SEND<>RECEIVE
		void sendOsc(short id, short i);	// Sends one message by OSC
		void oscCallback();
#ifdef CFG_CATCH_LOOP_EVENTS
		void oscCallback(ofEventArgs &e);
#endif
#endif
		
		
		// Methods
		void pushParam(int id, ofxConfigParam* p);
		bool isNumber(int id);
		bool isVector(int id);
		
		// Channel access
		// i=channel(RGB)/axis(XYZ)
		void setLimits(int id, int i, float vmin, float vmax);
		void setProg(int id, int i, float val);
		void setProg(int id, int i, const char *val);
		void set(int id, int i, float val);
		void set(int id, int i, const char *val);
		float getMin(int id, int i);
		float getMax(int id, int i);
		float getProg(int id, int i);
		float get(int id, int i);
		// Operators
		void invert(int id, short i);
		void sub(int id, int i, float val, bool clamp);
		void add(int id, int i, float val, bool clamp);
	};
	
	
