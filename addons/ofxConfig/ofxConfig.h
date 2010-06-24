/*
 *  ofxConfig.h
 *
 *  Created by Roger Sodre on 08/04/10.
 *  Copyright 2010 Studio Avante. All rights reserved.
 *
 */
#ifndef _OFX_CONFIG_H
#define _OFX_CONFIG_H

#include "ofMain.h"

// Comment/Uncomment to use MIDI
#define CFG_USE_MIDI			1

#define	CFG_MAX_STR_LEN			(128+1)
#define FLOAT_VEC				vector<float>

#define PICKER_COLOR_OUT		ofColor(100,100,100)
#define PICKER_COLOR_OVER		ofColor(200,200,200)
#define PICKER_COLOR_PICKED		ofColor(255,255,0)

//
// Types
enum enumConfigTypes
{
	TYPE_NONE = -1,	
	TYPE_FLOAT,
	TYPE_DOUBLE,
	TYPE_INTEGER,
	TYPE_LONG,
	TYPE_BOOLEAN,
	TYPE_BYTE,
	TYPE_STRING,
	TYPE_COLOR,
	TYPE_VECTOR,
	TYPE_COUNT
};

//
// Picker statuses
enum enumPickerStatus
{
	PICKER_OFF,	
	PICKER_OUT,
	PICKER_OVER,
	PICKER_PICKED
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
	void dec(float v=1.0f, bool clamp=false)
	{
		this->set(value-v);
		if (clamp && value < min)
			this->set(min);
	}
	void inc(float v=1.0f, bool clamp=false)
	{
		this->set(value+v);
		if (clamp && value > max)
			this->set(max);
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
	char	name[CFG_MAX_STR_LEN];
	// Picker
	int pickerStatus;
	float pickerSize;
	bool pickerClamp;
	ofColor pickerColorOut;
	ofColor pickerColorOver;
	ofColor pickerColorPicked;
	// Values
	char	string[CFG_MAX_STR_LEN];
    union {	// share same memory, so vector[0] = value
		struct {
			ofxConfigValue value, dummy[2];
		};
		ofxConfigValue vec[3];
    };
	
	ofxConfigParam(short t, char *nm)
	{
		type = t;
		strcpy(name, nm);
		pickerStatus = PICKER_OFF;
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
#ifdef CFG_USE_MIDI
#include "ofxMidi.h"
class ofxConfig : public ofxMidiListener {
#else
class ofxConfig {
#endif
public:
	ofxConfig();
	~ofxConfig();
	
#ifdef CFG_USE_MIDI
	// MIDI
	void setMidi(int id, short note);
	void setMidiLimits(int id, short note, float vmin, float vmax);
	void setMidiMulti(int id, short note0, short note1=-1, short note2=-1);
#endif
	
	// New parameters
	void add(short id, char *name, float val);				// Generic = float
	void addFloat(short id, char *name, float val);
	void addDouble(short id, char *name, double val);
	void addInt(short id, char *name, int val);
	void addLong(short id, char *name, long val);
	void addBool(short id, char *name, bool val);
	void addByte(short id, char *name, unsigned char val);
	void addString(short id, char *name, char *val);
	void addColor(short id, char *name, int hex);
	void addColor(short id, char *name, ofColor p);
	void addColor(short id, char *name, float r, float g, float b);
	void addVector(short id, char *name, ofPoint p);
	void addVector(short id, char *name, float x, float y, float z=0.0f);

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
	// Set values (generic = float)
	void set(int id, float val);			// TYPE_FLOAT (generic)
	void setR(int id, float val);
	void setG(int id, float val);
	void setB(int id, float val);
	void setX(int id, float val);
	void setY(int id, float val);
	void setZ(int id, float val);
	// Set values (specific)
	void set(int id, double val);			// TYPE_DOUBLE
	void set(int id, short val);			// TYPE_SHORT
	void set(int id, int val);				// TYPE_INT
	void set(int id, long val);				// TYPE_LOG
	void set(int id, bool val);				// TYPE_BOOL
	void set(int id, unsigned char val);	// TYPE_BYTE
	void set(int id, char *val);			// TYPE_STRING
	void set(int id, ofColor c);			// TYPE_COLOR
	void set(int id, ofPoint p);			// TYPE_VECTOR
	void set(int id, float val0, float val1, float val2=0.0f);	// TYPE_VECTOR / TYPE_COLOR
	
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
	void getString(int id, char *str);
	char* getString(int id);
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
	void dec(int id, float val=1.0, bool clamp=false);
	void decX(int id, float val=1.0, bool clamp=false);
	void decY(int id, float val=1.0, bool clamp=false);
	void decZ(int id, float val=1.0, bool clamp=false);
	void decR(int id, float val=1.0, bool clamp=false);
	void decG(int id, float val=1.0, bool clamp=false);
	void decB(int id, float val=1.0, bool clamp=false);
	void inc(int id, float val=1.0, bool clamp=false);
	void incX(int id, float val=1.0, bool clamp=false);
	void incY(int id, float val=1.0, bool clamp=false);
	void incZ(int id, float val=1.0, bool clamp=false);
	void incR(int id, float val=1.0, bool clamp=false);
	void incG(int id, float val=1.0, bool clamp=false);
	void incB(int id, float val=1.0, bool clamp=false);
	
	//
	// Mouse Pickers
	// Thanks memo!
	// http://www.openframeworks.cc/forum/viewtopic.php?f=9&t=1825
	void setPicker(int id, bool clamp=false, float sz=7.0, ofColor outColor=PICKER_COLOR_OUT, ofColor overColor=PICKER_COLOR_OVER, ofColor pickedColor=PICKER_COLOR_PICKED);
	bool isMouseOverPicker(ofxConfigParam *p, float x, float y);
	void mouseMoved(ofMouseEventArgs &e);
	void mouseDragged(ofMouseEventArgs &e);
	void mousePressed(ofMouseEventArgs &e);
	void mouseReleased(ofMouseEventArgs &e);
	void drawPickers(ofEventArgs &e);
	
	//
	// REC / PLAY params
	bool isRecording;
	bool isPlaying;
	// Methods
	void recStart();
	void recAdd();
	void recStop();
	void recPlay();
	int getPlayFrame();

	//
	// READ / SAVE to file
	int readFile(const char *filename);
	int saveFile(const char *filename);
	char* getSaveTime();
	//void xmlSave(char *xmlName);
	//void xmlLoad(char *xmlName);
	//void xmlParse(proxml.XMLElement xmlFile);
	
	
private:
	// Parameter index
	vector<ofxConfigParam*>	params;
	short		paramCount;
	char		saveTime[32];
	
#ifdef CFG_USE_MIDI
	// MIDI
	ofxMidiOut	*midiOut;
	ofxMidiIn	*midiIn;
	int			midiChannel;
	int			midiPort;
	// ofxMidiListener Virtual
	virtual void newMidiMessage(ofxMidiEventArgs& eventArgs);
#endif
	
	// Picker data
	int lastX, lastY;
	
	// REC
	vector<FLOAT_VEC> recBuffer;
	int			recFrame;
	int			playFrame;
	
	// Methods
	void pushParam(int id, ofxConfigParam* p);
	bool isNumber(int id);
	bool isVector(int id);

	// Channel access
	// i=channel(RGB)/axis(XYZ)
	void setLimits(int id, int i, float vmin, float vmax);
	void setProg(int id, int i, float val);
	void set(int id, int i, float val);
	void set(int id, int i, char *val);
	float getMin(int id, int i);
	float getMax(int id, int i);
	float getProg(int id, int i);
	float get(int id, int i);
	char* getString(int id, int i);
	
};


#endif
