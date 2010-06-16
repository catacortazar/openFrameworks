/*
 *  ofxConfig.cpp
 *
 *  Created by Roger Sodre on 08/04/10.
 *  Copyright 2010 Studio Avante. All rights reserved.
 *
 */

////////////////////////////////////////////////////////////////////////
//
// BASE CONFIG CLASS
//
//  Config class v1.0
//  Manage configuration parameters + save as XML
//  by Roger Sodre
//
//  Install:
//
// - Add addons/ofxMidi
// - Add CoreMIDI.framework
//
//  Usage:
//
// - Inherit Config class
// - Create param enum
// - Initialize parameters
//
//  TODO:
//
// - Color type
//

#include "ofxConfig.h"

#define CAPTURE_FORMAT	"/Users/Roger/Desktop/CAPTURE/capture_%06d.jpg"

#define VERBOSE_REC	1


ofxConfig::ofxConfig()
{
	// init rec
	isRecording = isPlaying = false;
	recFrame = 0;
	strcpy(saveTime,"not loaded");
	
	// init midi
#ifdef CFG_USE_MIDI
	midiOut = new ofxMidiOut();
	midiIn = new ofxMidiIn();
	midiIn->addListener(this);
	// midi comm
	midiChannel = 1;
	midiPort = midiIn->findMyDevicePort();
	midiOut->openPort(midiPort);
	midiIn->openPort(midiPort);
#endif
	
	// init XML
	//xmlInOut = new XMLInOut(applet);
}
ofxConfig::~ofxConfig()
{
	// free params
	while ( params.size() > 0 )
	{
		ofxConfigParam* p = params.back();
		if (p != NULL)
			delete p;
		params.pop_back();
	}
#ifdef CFG_USE_MIDI
	// free midi
	delete midiIn;
	delete midiOut;
#endif
}

////////////////////////////////////////////////
//
// MIDI
//
void ofxConfig::setMidi(int id, short note, float vmin, float vmax)
{
	params[id]->setMidi(note, vmin, vmax);
	//midiOut->stimulate(midiChannel, note);
	//midiOut->sendControlChange(midiChannel, note, 0);
}
#ifdef CFG_USE_MIDI
// ofxMidi virtual
void ofxConfig::newMidiMessage(ofxMidiEventArgs& eventArgs)
{
	int channel = eventArgs.channel;
	int note = eventArgs.byteOne;
	int val = eventArgs.byteTwo;
	float prog = ((float)val / 127.0);
	printf("MIDI IN ch[%d] note[%d] = [%d] prog[%.2f]",channel,note,val,prog);
	
	for (int n = 0 ; n < params.size() ; n++)
	{
		if (params[n] == NULL)
			continue;
		ofxConfigParam *p = params.at(n);
		if (p->note == note)
		{
			p->prog = prog;
			float v = p->valMin + ((p->valMax - p->valMin) * prog);
			printf(" >> CFG id[%d] = [%.3f] (%s)\n",n,v,p->name);
			this->set(n, v);
			return;
		}
	}
	printf(" >> unassigned\n");
}
#endif


// push new param to vector
void ofxConfig::pushParam(int id, ofxConfigParam* p)
{
	// id is always the vector index
	// so we fill the vector with NULLs until it has <id> elements
	while (params.size() <= id)
		params.push_back(NULL);
	// replace param
	params.erase(params.begin()+id);
	params.insert(params.begin()+id, p);
}


/////////////////////////////////////////////
//
// INITTERS
//
// generic (float)
void ofxConfig::add(short id, char *name, float val)
{
	this->addFloat(id, name, val);
}
//
// specific
void ofxConfig::addByte(short id, char *name, char val)
{
	this->pushParam(id, new ofxConfigParam(TYPE_BYTE, name));
	this->set(id, (float) val);
}
void ofxConfig::addShort(short id, char *name, short val)
{
	this->pushParam(id, new ofxConfigParam(TYPE_SHORT, name));
	this->set(id, (float) val);
}
void ofxConfig::addInt(short id, char *name, int val)
{
	this->pushParam(id, new ofxConfigParam(TYPE_INTEGER, name));
	this->set(id, (float) val);
}
void ofxConfig::addLong(short id, char *name, long val)
{
	this->pushParam(id, new ofxConfigParam(TYPE_LONG, name));
	this->set(id, (float) val);
}
void ofxConfig::addFloat(short id, char *name, float val)
{
	this->pushParam(id, new ofxConfigParam(TYPE_FLOAT, name));
	this->set(id, (float) val);
}
void ofxConfig::addDouble(short id, char *name, double val)
{
	this->pushParam(id, new ofxConfigParam(TYPE_DOUBLE, name));
	this->set(id, (float) val);
}
void ofxConfig::addBool(short id, char *name, bool val)
{
	this->pushParam(id, new ofxConfigParam(TYPE_BOOLEAN, name));
	this->set(id, (float) ( val == true ? 1.0 : 0.0 ) );
}
void ofxConfig::addString(short id, char *name, char *val)
{
	this->pushParam(id, new ofxConfigParam(TYPE_STRING, name));
	this->set(id, val);
}
/*
void ofxConfig::addColor(short id, char *name, color val)
{
}
*/


//
// SETTERS
//
// generic (float)
void ofxConfig::set(int id, float val)
{
	ofxConfigParam *p = params[id];
	switch (p->type)
	{
		case TYPE_FLOAT:
		case TYPE_DOUBLE:
		case TYPE_SHORT:
		case TYPE_INTEGER:
		case TYPE_LONG:
		case TYPE_BOOLEAN:
		case TYPE_BYTE:
			p->value = val;
			p->prog = ( (p->note >= 0) ? ((val-p->valMin) / (p->valMax-p->valMin)) : 1.0 );
			break;
		case TYPE_STRING:
			sprintf(p->string, "%.2f",val);
			p->prog = 1.0;
			break;
		case TYPE_COLOR:
			//	this->setColor(id, (color)val);
			//break;
		default:
			printf("Config.set(int) ERROR invalid id[%d] type[%d] val[%d]\n",id,p->type,val);
			break;
	};
}
//
// specific
void ofxConfig::set(int id, double val)
{
	this->set(id, (float)val);
}
void ofxConfig::set(int id, short val)
{
	this->set(id, (float)val);
}
void ofxConfig::set(int id, int val)
{
	this->set(id, (float)val);
}
void ofxConfig::set(int id, long val)
{
	this->set(id, (float)val);
}
void ofxConfig::set(int id, bool val)
{
	this->set(id, (float)( val ? 1.0f : 0.0f ));
}
void ofxConfig::set(int id, char val)
{
	this->set(id, (float)val);
}
void ofxConfig::set(int id, char *val)
{
	// If not string, set as float
	if (params[id]->type != TYPE_STRING)
		this->set(id, (float)atof(val));
	// Set string
	else
		strcpy(params[id]->string, val);
}
//
// Color
// color = int
/*
 void ofxConfig::set(int id, int r, int g, int b)
{
}
void ofxConfig::setColor(int id, color val)
{
}
*/

//
// GETTERS
//
// Is it a number?
bool ofxConfig::isNumber(int id)
{
	switch (params[id]->type)
	{
		case TYPE_FLOAT:
		case TYPE_DOUBLE:
		case TYPE_SHORT:
		case TYPE_INTEGER:
		case TYPE_LONG:
		case TYPE_BYTE:
			return true;
			break;
	}
	return false;
}
// Get minimum value
float ofxConfig::getMin(int id)
{
	return ( (params[id]->note >= 0) ? params[id]->valMin : this->get(id) );
}
// Get maximum value
float ofxConfig::getMax(int id)
{
	return ( (params[id]->note >= 0) ? params[id]->valMax : this->get(id) );
}
// Get progress (vmin .. vmax)
float ofxConfig::getProg(int id)
{
	return params[id]->prog;
}
// Get param name
char* ofxConfig::getName(int id)
{
	return params[id]->name;
}
// Get generic value (float)
float ofxConfig::get(int id)
{
	ofxConfigParam *p = params[id];
	switch (p->type)
	{
		case TYPE_FLOAT:
		case TYPE_DOUBLE:
		case TYPE_SHORT:
		case TYPE_INTEGER:
		case TYPE_LONG:
		case TYPE_BOOLEAN:
		case TYPE_BYTE:
			return p->value;
			break;
		case TYPE_STRING:
			return atof(p->string);
			break;
		case TYPE_COLOR:
			//	this->setColor(id, (color)val);
			//break;
		default:
			printf("Config.get(int) ERROR invalid id[%d] type[%d] val[%d]\n",id,p->type);
			break;
	};
}
//
// specific
float ofxConfig::getFloat(int id)
{
	return (float) (params[id]->value);
}
double ofxConfig::getDouble(int id)
{
	return (double) (params[id]->value);
}
short ofxConfig::getShort(int id)
{
	return (short) (params[id]->value);
}
int ofxConfig::getInt(int id)
{
	return (int) (params[id]->value);
}
long ofxConfig::getLong(int id)
{
	return (long) (params[id]->value);
}
bool ofxConfig::getBool(int id)
{
	return ( params[id]->value == 0.0 ? false : true );
}
char ofxConfig::getByte(int id)
{
	return (char) (params[id]->value);
}
char* ofxConfig::getString(int id)
{
	// If not string, copy value to string
	if (params[id]->type != TYPE_STRING)
		sprintf(params[id]->string,"%f",this->get(id));
	// Return string
	return (char*) (params[id]->string);
}
/*
color ofxConfig::getColor(int id)
{
	return array_color[params[id]->index];
}
*/

////////////////////////////////////////////////////////////////////////
//
// OPERATIONS
//
// Invert a Boolean value
void ofxConfig::invert(int id)
{
	ofxConfigParam *p = params[id];
	if (p->type == TYPE_BOOLEAN)
		p->value = !(p->value);
}
// Add/Sub a value
void ofxConfig::dec(int id, float v)
{
	this->inc(id, -v);
}
void ofxConfig::inc(int id, float v)
{
	if (this->isNumber(id))
		params[id]->value += v;
}
// Clamp a value
void ofxConfig::clamp(int id, float down, float up)
{
	this->clampDown(id, down);
	this->clampUp(id, up);
}
void ofxConfig::clampDown(int id, float low)
{
	if (this->isNumber(id))
		if (params[id]->value < low)
			params[id]->value = low;
}
void ofxConfig::clampUp(int id, float up)
{
	if (this->isNumber(id))
		if (params[id]->value > up)
			params[id]->value = up;
}




////////////////////////////////////////////////////////////////////////
//
// REC / PLAY config parameters
//
// Start to record config params
void ofxConfig::recStart()
{
	if (isRecording == false)
	{
		// erase buffer
		for (int n = 0 ; n < recBuffer.size() ; n++)
			recBuffer[n].clear();
		recBuffer.clear();
		// new buffer
		isRecording = true;
		isPlaying = false;
		recFrame = 0;
		playFrame = -1;
	}
	else
	{
		this->recStop();
		if (VERBOSE_REC)
			printf("RECORDED [%d] frames\n",recFrame);
		//this->recPlay();
	}
}
// Stop recording config params
void ofxConfig::recStop()
{
	isRecording = false;
}
// Start to play config params
void ofxConfig::recPlay()
{
	// start to play if not rec
	if (isRecording == true)
		return;
	// start to play...
	isPlaying = true;
	playFrame = -1;
}
// IF RECORDING: Add corrent config params to buffer
// IF PLAYING: Get next config params from buffer
void ofxConfig::recAdd()
{
	// Save Frame
	if (isRecording == true)
	{
		// debug
		if (VERBOSE_REC)
			printf("REC FRAME [%d] rate[%.2f] secs[%.2f]\n",recFrame,ofGetFrameRate(),((recFrame+1)/ofGetFrameRate()));
		// save params
		FLOAT_VEC values;
		for (int n = 0 ; n < params.size() ; n++ )
		{
			if (params[n] == NULL)
				continue;
			values.push_back(this->get(n));
		}
		recBuffer.push_back(values);
		recFrame++;
	}
	// Play Frame
	else if (isPlaying == true)
	{
		// Retrieve params
		playFrame++;
		FLOAT_VEC values = recBuffer[playFrame];
		for (int n = 0 ; n < params.size() ; n++ )
		{
			if (params[n] == NULL)
				continue;
			this->set(n, values[n]);
		}
		// debug
		if (VERBOSE_REC)
			printf("PLAY FRAME [%d] rate[%.2f] [%.2f]%%\n",playFrame,ofGetFrameRate(),((playFrame+1)*100.0/(float)recFrame));
		// end play?
		if (playFrame >= (recFrame-1))
			isPlaying = false;
	}
}
int ofxConfig::getPlayFrame()
{
	return playFrame;
}








////////////////////////////////////////////////////////////////////////
//
// SAVE / READ config to file
//
// READ from file on data folder
// Return read params
int ofxConfig::readFile(const char *filename)
{
	// Open file
	char *fullFilename = (char*)ofToDataPath(filename).c_str();
	FILE *fp = fopen (fullFilename,"r");
	if (fp == NULL)
	{
		printf("ERROR reading config file [%s]\n",fullFilename);
		return 0;
	}
	
	// Read file
	char data[64];
	char key[32];
	char val[32];
	for (int n = 0 ; fgets(data,64,fp) != NULL ; n++)
	{
		// erase '\n';
		data[strlen(data)-1] = '\0';
		// find separator
		char *p = strchr(data,':');
		if (p == NULL)
			continue;
		// get val
		strcpy(val, p+1);
		// get key
		*p = '\0';
		strcpy(key, data);
		printf ("READ CGF: %s = %s\n",key,val);
		
		// Read params
		for (int n = 0 ; n < params.size() ; n++ )
		{
			if (params[n] == NULL)
				continue;
			if (!strcmp(key,params[n]->name))
			{
				this->set(n, val);
				break;
			}
		}
		// Is saved time?
		if (!strcmp(key,"SAVE_TIME"))
			strcpy(saveTime,val);
	}
	
	// Close file
	fclose (fp);
	printf("READ config [%s] OK!\n",fullFilename);
}
//
// SAVE to file on data folder
// Return saved params
int ofxConfig::saveFile(const char *filename)
{
	// Open file
	char *fullFilename = (char*)ofToDataPath(filename).c_str();
	FILE *fp = fopen (fullFilename,"w");
	if (fp == NULL)
	{
		printf("ERROR saving config [%s]\n",fullFilename);
		return 0;
	}
	
	// Save params
	char data[128];
	for (int n = 0 ; n < params.size() ; n++ )
	{
		if (params[n] == NULL)
			continue;
		sprintf(data,"%s:%s\n",this->getName(n),this->getString(n));
		fputs  (data,fp);
	}
	
	// generate save time
	time_t now;
	time ( &now );
	strcpy(saveTime, &(ctime(&now)[4]));
	sprintf(data,"SAVE_TIME:%s\n",saveTime);
	fputs(data,fp);
	
	// Close file
	fclose (fp);
	
	// Ok!
	printf("SAVE config [%s] OK!\n",fullFilename);
}
// Get param name
char* ofxConfig::getSaveTime()
{
	return saveTime;
}




