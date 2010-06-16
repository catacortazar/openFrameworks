/*
 *  ofxConfig.h
 *
 *  Created by Roger Sodre on 08/04/10.
 *  Copyright 2010 Studio Avante. All rights reserved.
 *
 */
#pragma once

#include "ofMain.h"

// Comment/Uncomment to use MIDI
#define CFG_USE_MIDI			1

#define	CFG_MAX_STR_LEN			(64+1)
#define FLOAT_VEC				vector<float>

//
// Types
enum enumConfigTypes
{
	TYPE_NONE = -1,	
	TYPE_FLOAT,
	TYPE_DOUBLE,
	TYPE_SHORT,
	TYPE_INTEGER,
	TYPE_LONG,
	TYPE_BOOLEAN,
	TYPE_BYTE,
	TYPE_STRING,
	TYPE_COLOR,
	TYPE_COUNT
};

//
// Config Parameter
class ofxConfigParam {
public:
	// Values
	float	prog;
	float	value;
	char	string[CFG_MAX_STR_LEN];
	// Data type
	short	type;
	char	name[CFG_MAX_STR_LEN];
	// MIDI
	short	note;
	float	valMin;
	float	valMax;
	
	ofxConfigParam(short t, char *nm)
	{
		type = t;
		strcpy(name, nm);
		valMin = 0.0;
		valMax = 1.0;
		note = -1;
	}

	void ofxConfigParam::setMidi(short nt, float vmin, float vmax)
	{
		valMin = vmin;
		valMax = vmax;
		note = nt;
	}
};

//
// BASE CONFIG CLASS
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
	
	// MIDI connection
	void setMidi(int id, short note, float vmin=0.0, float vmax=1.0);
	
	// New parameters
	void add(short id, char *name, float val);
	void addFloat(short id, char *name, float val);
	void addDouble(short id, char *name, double val);
	void addShort(short id, char *name, short val);
	void addInt(short id, char *name, int val);
	void addLong(short id, char *name, long val);
	void addBool(short id, char *name, bool val);
	void addByte(short id, char *name, char val);
	void addString(short id, char *name, char *val);
	//void addColor(short id, char *name, color val);
	
	// Setters
	void set(int id, float val);
	void set(int id, double val);
	void set(int id, short val);
	void set(int id, int val);
	void set(int id, long val);
	void set(int id, bool val);
	void set(int id, char val);
	void set(int id, char *val);
	//void set(int id, int r, int g, int b);
	
	// Getters
	bool isNumber(int id);
	float getMin(int id);
	float getMax(int id);
	float getProg(int id);
	char* getName(int id);
	// get values
	float get(int id);
	float getFloat(int id);
	double getDouble(int id);
	short getShort(int id);
	int getInt(int id);
	long getLong(int id);
	bool getBool(int id);
	char getByte(int id);
	char* getString(int id);
	//color getColor(int id);
	
	//
	// Operations
	void invert(int id);
	void dec(int id, float v=1.0);
	void inc(int id, float v=1.0);
	void clamp(int id, float low, float high);
	void clampDown(int id, float low);
	void clampUp(int id, float high);
	
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
	
	// REC
	vector<FLOAT_VEC> recBuffer;
	int			recFrame;
	int			playFrame;
	
	//
	// Private methods
	void pushParam(int id, ofxConfigParam* p);

	// XML
	//proxml.XMLElement xmlFile = null;
	//XMLInOut xmlInOut = null;
};











