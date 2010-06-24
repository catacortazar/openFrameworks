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
//

#include "ofxConfig.h"

#define CAPTURE_FORMAT	"/Users/Roger/Desktop/CAPTURE/capture_%06d.jpg"

#define VERBOSE_REC	1


ofxConfig::ofxConfig()
{
	// init rec
	isRecording = isPlaying = false;
	recFrame = 0;
	strcpy(saveTime,"using defaults");
	
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
	
	// Listen to picker movements
	ofAddListener(ofEvents.mousePressed, this, &ofxConfig::mousePressed);
	ofAddListener(ofEvents.mouseMoved, this, &ofxConfig::mouseMoved);
	ofAddListener(ofEvents.mouseDragged, this, &ofxConfig::mouseDragged);
	ofAddListener(ofEvents.mouseReleased, this, &ofxConfig::mouseReleased);
	ofAddListener(ofEvents.draw, this, &ofxConfig::drawPickers);
	
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

/////////////////////////////////////////////////////////////////////////
//
// MIDI
//
#ifdef CFG_USE_MIDI
void ofxConfig::setMidi(int id, short note)
{
	// Color: One note for all channels
	if (params[id]->type == TYPE_COLOR)
	{
		params[id]->vec[0].midiNote = note;
		params[id]->vec[1].midiNote = note;
		params[id]->vec[2].midiNote = note;
	}
	else
		params[id]->value.midiNote = note;
	
	// Can I push a value to MIDI device? I don't think so...
	//midiOut->stimulate(midiChannel, note);
	//midiOut->sendControlChange(midiChannel, note, 0);
}
void ofxConfig::setMidiLimits(int id, short note, float vmin, float vmax)
{
	params[id]->value.midiNote = note;
	params[id]->value.setLimits(vmin, vmax);
}
void ofxConfig::setMidiMulti(int id, short note0, short note1, short note2)
{
	params[id]->vec[0].midiNote = note0;
	params[id]->vec[1].midiNote = note1;
	params[id]->vec[2].midiNote = note2;
}
//
// ofxMidi virtual callback
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
		for (int v = 0 ; v < 3 ; v++)
		{
			if (p->vec[v].midiNote == note)
			{
				p->vec[v].setProg(prog);
				printf(" >> CFG id[%d/%d] = [%.3f] (%s)\n",n,v,p->vec[v].get(),p->name);
				return;
			}
		}
	}
	printf(" >> unassigned\n");
}
#endif

/////////////////////////////////////////////////////////////////////////
//
// PRIVATE
//

// store new param
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
// Is it a number?
bool ofxConfig::isNumber(int id)
{
	switch (params[id]->type)
	{
		case TYPE_FLOAT:
		case TYPE_DOUBLE:
		case TYPE_INTEGER:
		case TYPE_LONG:
		case TYPE_BYTE:
		case TYPE_COLOR:
		case TYPE_VECTOR:
			return true;
			break;
	}
	return false;
}
// Is it a vector?
bool ofxConfig::isVector(int id)
{
	switch (params[id]->type)
	{
		case TYPE_COLOR:
		case TYPE_VECTOR:
			return true;
			break;
	}
	return false;
}


/////////////////////////////////////////////////////////////////////////
//
// INITTERS
//
// generic (float)
void ofxConfig::add(short id, char *name, float val)
{
	this->addFloat(id, name, val);
}
// specific
void ofxConfig::addFloat(short id, char *name, float val)
{
	this->pushParam(id, new ofxConfigParam(TYPE_FLOAT, name));
	this->set(id, val);
}
void ofxConfig::addDouble(short id, char *name, double val)
{
	this->pushParam(id, new ofxConfigParam(TYPE_DOUBLE, name));
	this->set(id, val);
}
void ofxConfig::addInt(short id, char *name, int val)
{
	this->pushParam(id, new ofxConfigParam(TYPE_INTEGER, name));
	this->set(id, val);
}
void ofxConfig::addLong(short id, char *name, long val)
{
	this->pushParam(id, new ofxConfigParam(TYPE_LONG, name));
	this->set(id, val);
}
void ofxConfig::addBool(short id, char *name, bool val)
{
	this->pushParam(id, new ofxConfigParam(TYPE_BOOLEAN, name));
	this->setLimits(id, 0.0f, 1.0f);
	this->set(id, val );
}
void ofxConfig::addByte(short id, char *name, unsigned char val)
{
	this->pushParam(id, new ofxConfigParam(TYPE_BYTE, name));
	this->setLimits(id, (float)0, (float)255);
	this->set(id, (float) val);
}
void ofxConfig::addString(short id, char *name, char *val)
{
	this->pushParam(id, new ofxConfigParam(TYPE_STRING, name));
	this->set(id, val);
}
void ofxConfig::addColor(short id, char *name, int hex)
{
	this->addColor(id, name, ofColor(hex));
}
void ofxConfig::addColor(short id, char *name, ofColor c)
{
	this->addColor(id, name, c.r, c.g, c.b);
}
void ofxConfig::addColor(short id, char *name, float r, float g, float b)
{
	this->pushParam(id, new ofxConfigParam(TYPE_COLOR, name));
	this->setLimitsR(id, 0.0f, 255.0f);
	this->setLimitsG(id, 0.0f, 255.0f);
	this->setLimitsB(id, 0.0f, 255.0f);
	this->set(id, r, g, b);
}
void ofxConfig::addVector(short id, char *name, ofPoint p)
{
	this->addVector(id, name, p.x, p.y, p.z);
}
void ofxConfig::addVector(short id, char *name, float x, float y, float z)
{
	this->pushParam(id, new ofxConfigParam(TYPE_VECTOR, name));
	this->setLimitsX(id, 0.0f, ofGetWidth());
	this->setLimitsY(id, 0.0f, ofGetHeight());
	this->set(id, x, y, z);
}


/////////////////////////////////////////////////////////////////////////
//
// SETTERS
//
// Set limits
void ofxConfig::setLimits(int id, int i, float vmin, float vmax)		// Private
{
	params[id]->vec[i].setLimits(vmin,vmax);
}
void ofxConfig::setLimits(int id, float vmin, float vmax) { this->setLimits(id,0,vmin,vmax); }
void ofxConfig::setLimitsR(int id, float vmin, float vmax) { this->setLimits(id,0,vmin,vmax); }
void ofxConfig::setLimitsG(int id, float vmin, float vmax) { this->setLimits(id,1,vmin,vmax); }
void ofxConfig::setLimitsB(int id, float vmin, float vmax) { this->setLimits(id,2,vmin,vmax); }
void ofxConfig::setLimitsX(int id, float vmin, float vmax) { this->setLimits(id,0,vmin,vmax); }
void ofxConfig::setLimitsY(int id, float vmin, float vmax) { this->setLimits(id,1,vmin,vmax); }
void ofxConfig::setLimitsZ(int id, float vmin, float vmax) { this->setLimits(id,2,vmin,vmax); }
// Preserve Prog: prog is priority
// Prog will be saved and read form file instead of value
void ofxConfig::preserveProg(int id, bool b)
{
	params[id]->vec[0].preserveProg = b;
	params[id]->vec[1].preserveProg = b;
	params[id]->vec[2].preserveProg = b;
}
// Set Prog
void ofxConfig::setProg(int id, int i, float p)		// Private
{
	params[id]->vec[i].setProg(p);
}
void ofxConfig::setProg(int id, float p) { this->setProg(id,0,p); }
void ofxConfig::setProgR(int id, float p) { this->setProg(id,0,p); }
void ofxConfig::setProgG(int id, float p) { this->setProg(id,1,p); }
void ofxConfig::setProgB(int id, float p) { this->setProg(id,2,p); }
void ofxConfig::setProgX(int id, float p) { this->setProg(id,0,p); }
void ofxConfig::setProgY(int id, float p) { this->setProg(id,1,p); }
void ofxConfig::setProgZ(int id, float p) { this->setProg(id,2,p); }
// generic (float)
// val1, val2 will be used only on TYPE_VECTOR and TYPE_COLOR
void ofxConfig::set(int id, int i, float val)		// Private
{
	ofxConfigParam *p = params[id];
	switch (p->type)
	{
		case TYPE_FLOAT:
		case TYPE_DOUBLE:
		case TYPE_INTEGER:
		case TYPE_LONG:
		case TYPE_BOOLEAN:
		case TYPE_BYTE:
		case TYPE_COLOR:
		case TYPE_VECTOR:
			p->vec[i].set(val);
			break;
		case TYPE_STRING:
			sprintf(p->string, "%.2f",val);
			break;
		default:
			printf("Config.set(int) ERROR invalid id[%d] type[%d] val[%d]\n",id,p->type,val);
			break;
	};
}
void ofxConfig::set(int id, float val) { this->set(id, 0, val); }
void ofxConfig::setR(int id, float val) { this->set(id, 0, val); }
void ofxConfig::setG(int id, float val) { this->set(id, 1, val); }
void ofxConfig::setB(int id, float val) { this->set(id, 2, val); }
void ofxConfig::setX(int id, float val) { this->set(id, 0, val); }
void ofxConfig::setY(int id, float val) { this->set(id, 1, val); }
void ofxConfig::setZ(int id, float val) { this->set(id, 2, val); }
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
void ofxConfig::set(int id, unsigned char val)
{
	this->set(id, (float)val);
}
void ofxConfig::set(int id, int i, char *val)		// Private
{
	if (params[id]->type == TYPE_STRING)	// Set string!
		strcpy(params[id]->string, val);
	else									// If not string, convert to float
		this->set(id, i, (float)atof(val));
}
void ofxConfig::set(int id, char *val)
{
	this->set(id, 0, val);
}
void ofxConfig::set(int id, ofColor c)
{
	this->set(id, c.r, c.g, c.b);
}
void ofxConfig::set(int id, ofPoint p)
{
	this->set(id, p.x, p.y, p.z);
}
void ofxConfig::set(int id, float val0, float val1, float val2)
{
	params[id]->vec[0].set(val0);
	params[id]->vec[1].set(val1);
	params[id]->vec[2].set(val2);
}


/////////////////////////////////////////////////////////////////////////
//
// GETTERS
//
char* ofxConfig::getName(int id)
{
	return params[id]->name;
}
short ofxConfig::getType(int id)
{
	return params[id]->type;
}
// Get minimum value
float ofxConfig::getMin(int id, int i)		// Private
{
	return params[id]->vec[i].getMin();
}
float ofxConfig::getMin(int id) { return this->getMin(id, 0); }
float ofxConfig::getMinR(int id) { return this->getMin(id, 0); }
float ofxConfig::getMinG(int id) { return this->getMin(id, 1); }
float ofxConfig::getMinB(int id) { return this->getMin(id, 2); }
float ofxConfig::getMinX(int id) { return this->getMin(id, 0); }
float ofxConfig::getMinY(int id) { return this->getMin(id, 1); }
float ofxConfig::getMinZ(int id) { return this->getMin(id, 2); }
// Get maximum value
float ofxConfig::getMax(int id, int i)		// Private
{
	return params[id]->vec[i].getMax();
}
float ofxConfig::getMax(int id) { return this->getMax(id, 0); }
float ofxConfig::getMaxR(int id) { return this->getMax(id, 0); }
float ofxConfig::getMaxG(int id) { return this->getMax(id, 1); }
float ofxConfig::getMaxB(int id) { return this->getMax(id, 2); }
float ofxConfig::getMaxX(int id) { return this->getMax(id, 0); }
float ofxConfig::getMaxY(int id) { return this->getMax(id, 1); }
float ofxConfig::getMaxZ(int id) { return this->getMax(id, 2); }
// Get progress (min=0.0 .. max=1.0)
float ofxConfig::getProg(int id, int i)		// Private
{
	return params[id]->vec[i].getProg();
}
float ofxConfig::getProg(int id) { return this->getProg(id, 0); }
float ofxConfig::getProgR(int id) { return this->getProg(id, 0); }
float ofxConfig::getProgG(int id) { return this->getProg(id, 1); }
float ofxConfig::getProgB(int id) { return this->getProg(id, 2); }
float ofxConfig::getProgX(int id) { return this->getProg(id, 0); }
float ofxConfig::getProgY(int id) { return this->getProg(id, 1); }
float ofxConfig::getProgZ(int id) { return this->getProg(id, 2); }
// Get generic value (float)
float ofxConfig::get(int id, int i)		// Private
{
	ofxConfigParam *p = params[id];
	switch (p->type)
	{
		case TYPE_FLOAT:
		case TYPE_DOUBLE:
		case TYPE_INTEGER:
		case TYPE_LONG:
		case TYPE_BOOLEAN:
		case TYPE_BYTE:
		case TYPE_COLOR:
		case TYPE_VECTOR:
			return p->vec[i].get();
			break;
		case TYPE_STRING:
			return atof(p->string);
			break;
		default:
			printf("Config.get(int) ERROR invalid id[%d] type[%d] val[%d]\n",id,p->type);
			break;
	};
}
float ofxConfig::get(int id) { return this->get(id, 0); }
float ofxConfig::getR(int id) { return this->get(id, 0); }
float ofxConfig::getG(int id) { return this->get(id, 1); }
float ofxConfig::getB(int id) { return this->get(id, 2); }
float ofxConfig::getX(int id) { return this->get(id, 0); }
float ofxConfig::getY(int id) { return this->get(id, 1); }
float ofxConfig::getZ(int id) { return this->get(id, 2); }
//
// specific
float ofxConfig::getFloat(int id)
{
	return (float) (params[id]->value.get());
}
double ofxConfig::getDouble(int id)
{
	return (double) (params[id]->value.get());
}
short ofxConfig::getShort(int id)
{
	return (short) (params[id]->value.get());
}
int ofxConfig::getInt(int id)
{
	return (int) (params[id]->value.get());
}
long ofxConfig::getLong(int id)
{
	return (long) (params[id]->value.get());
}
bool ofxConfig::getBool(int id)
{
	return ( params[id]->value.get() == 0.0 ? false : true );
}
unsigned char ofxConfig::getByte(int id)
{
	return (unsigned char) (params[id]->value.get());
}
void ofxConfig::getString(int id, char *str)
{
	strcpy(str, this->getString(id,0));
}
char* ofxConfig::getString(int id)
{
	return this->getString(id,0);
}
char* ofxConfig::getString(int id, int i)		// Private
{
	// If not string, copy value to string
	if (params[id]->type != TYPE_STRING)
		sprintf(params[id]->string,"%f",this->get(id,i));
	// Return string
	return (char*) (params[id]->string);
}
ofColor ofxConfig::getColor(int id)
{
	ofxConfigParam *p = params[id];
	return ofColor(p->vec[0].get(), p->vec[1].get(), p->vec[2].get());
}
ofPoint ofxConfig::getVector(int id)
{
	ofxConfigParam *p = params[id];
	return ofPoint(p->vec[0].get(), p->vec[1].get(), p->vec[2].get());
}


/////////////////////////////////////////////////////////////////////////
//
// OPERATIONS
//
// Invert a value
void ofxConfig::invert(int id)
{
	if (this->isVector(id))
	{
		params[id]->vec[0].invert();
		params[id]->vec[1].invert();
		params[id]->vec[2].invert();
	}
	else
		params[id]->value.invert();
}
// invert one channel
void ofxConfig::invertR(int id) { params[id]->vec[0].invert(); }
void ofxConfig::invertG(int id) { params[id]->vec[1].invert(); }
void ofxConfig::invertB(int id) { params[id]->vec[2].invert(); }
void ofxConfig::invertX(int id) { params[id]->vec[0].invert(); }
void ofxConfig::invertY(int id) { params[id]->vec[1].invert(); }
void ofxConfig::invertZ(int id) { params[id]->vec[2].invert(); }
//
// Decrease a value
void ofxConfig::dec(int id, float val, bool clamp)
{
	if (this->isVector(id))
	{
		params[id]->vec[0].dec(val, clamp);
		params[id]->vec[1].dec(val, clamp);
		params[id]->vec[2].dec(val, clamp);
	}
	else
		params[id]->vec[0].dec(val, clamp);
}
void ofxConfig::decX(int id, float val, bool clamp) { params[id]->vec[0].dec(val, clamp); }
void ofxConfig::decY(int id, float val, bool clamp) { params[id]->vec[1].dec(val, clamp); }
void ofxConfig::decZ(int id, float val, bool clamp) { params[id]->vec[2].dec(val, clamp); }
void ofxConfig::decR(int id, float val, bool clamp) { params[id]->vec[0].dec(val, clamp); }
void ofxConfig::decG(int id, float val, bool clamp) { params[id]->vec[1].dec(val, clamp); }
void ofxConfig::decB(int id, float val, bool clamp) { params[id]->vec[2].dec(val, clamp); }
//
// Increase  a value
void ofxConfig::inc(int id, float val, bool clamp)
{
	if (this->isVector(id))
	{
		params[id]->vec[0].inc(val, clamp);
		params[id]->vec[1].inc(val, clamp);
		params[id]->vec[2].inc(val, clamp);
	}
	else
		params[id]->vec[0].inc(val, clamp);
}
void ofxConfig::incX(int id, float val, bool clamp) { params[id]->vec[0].inc(val, clamp); }
void ofxConfig::incY(int id, float val, bool clamp) { params[id]->vec[1].inc(val, clamp); }
void ofxConfig::incZ(int id, float val, bool clamp) { params[id]->vec[2].inc(val, clamp); }
void ofxConfig::incR(int id, float val, bool clamp) { params[id]->vec[0].inc(val, clamp); }
void ofxConfig::incG(int id, float val, bool clamp) { params[id]->vec[1].inc(val, clamp); }
void ofxConfig::incB(int id, float val, bool clamp) { params[id]->vec[2].inc(val, clamp); }




/////////////////////////////////////////////////////////////////////////
//
// MOUSE PICKERS
//
// Set a VECTOR as picker
void ofxConfig::setPicker(int id, bool clamp, float sz, ofColor outColor, ofColor overColor, ofColor pickedColor)
{
	ofxConfigParam *p = params[id];
	if (p->type != TYPE_VECTOR)
	{
		printf("Param [%s] is not TYPE_VECTOR become a picker!",p->name);
		return;
	}
	// Save picker info
	p->pickerClamp = clamp;
	p->pickerSize = sz;
	p->pickerColorOut = outColor;
	p->pickerColorOver = overColor;
	p->pickerColorPicked = pickedColor;
	// Put picker to work
	p->pickerStatus = PICKER_OUT;
}
//
// Test if mouse is over a picker
bool ofxConfig::isMouseOverPicker(ofxConfigParam *p, float x, float y)
{
	if (x >= (int)(p->vec[0].get()-(p->pickerSize)) && x <= (int)(p->vec[0].get()+(p->pickerSize)) && 
		y >= (int)(p->vec[1].get()-(p->pickerSize)) && y <= (int)(p->vec[1].get()+(p->pickerSize)) )
		return true;
	else
		return false;
}
//
// Mouse events
void ofxConfig::mouseMoved(ofMouseEventArgs &e)
{
	for (int id = 0 ; id < params.size() ; id++ )
	{
		ofxConfigParam *p = params[id];
		if (p == NULL)
			continue;
		// Picker is OFF?
		if (p->pickerStatus == PICKER_OFF)
			continue;
		// mouse is over picker?
		if (isMouseOverPicker(p, e.x, e.y))
			p->pickerStatus = PICKER_OVER;
		else
			p->pickerStatus = PICKER_OUT;
	}
}
void ofxConfig::mousePressed(ofMouseEventArgs &e)
{
	for (int id = 0 ; id < params.size() ; id++ )
	{
		ofxConfigParam *p = params[id];
		if (p == NULL)
			continue;
		// Picker is ON?
		if (isMouseOverPicker(p, e.x, e.y))
		{
			p->pickerStatus = PICKER_PICKED;
			break;	// Pick just ONE!
		}
	}
	// Remember position
	lastX = e.x;
	lastY = e.y;
}
void ofxConfig::mouseDragged(ofMouseEventArgs &e)
{
	int v;
	float min, max;
	for (int id = 0 ; id < params.size() ; id++ )
	{
		ofxConfigParam *p = params[id];
		if (p == NULL)
			continue;
		// Picker is ON?
		if (p->pickerStatus == PICKER_PICKED)
		{
			// Move X
			v = e.x-lastX;
			// ignore movement outside limits
			min = this->getMinX(id);
			max = this->getMaxX(id);
			if (p->pickerClamp && lastX < min)
				v -= (min-lastX);
			else if (p->pickerClamp && lastX > max)
				v += (lastX-max);
			this->incX(id, v);
			// Move Y
			v = e.y-lastY;
			// ignore movement outside limits
			min = this->getMinY(id);
			max = this->getMaxY(id);
			if (p->pickerClamp && lastY < min)
				v -= (min-lastY);
			else if (p->pickerClamp && lastY > max)
				v += (lastY-max);
			this->incY(id, v);
			// Clamp values to limits?
			if (p->pickerClamp)
			{
				p->vec[0].clamp();
				p->vec[1].clamp();
			}
		}
	}
	// Remember position
	lastX = e.x;
	lastY = e.y;
}
void ofxConfig::mouseReleased(ofMouseEventArgs &e)
{
	this->mouseMoved(e);
}
//
// Draw pickers!
void ofxConfig::drawPickers(ofEventArgs &e)
{
	for (int id = 0 ; id < params.size() ; id++ )
	{
		ofxConfigParam *p = params[id];
		if (p == NULL)
			continue;
		// Picker is ON?
		if (p->pickerStatus == PICKER_OFF)
			continue;
		// Set color
		if (p->pickerStatus == PICKER_OUT)
			ofSetColor(p->pickerColorOut);
		else if (p->pickerStatus == PICKER_OVER)
			ofSetColor(p->pickerColorOver);
		else if (p->pickerStatus == PICKER_PICKED)
			ofSetColor(p->pickerColorPicked);
		// Draw picker
		ofNoFill();
		ofRect(p->vec[0].get()-(p->pickerSize/2.0), p->vec[1].get()-(p->pickerSize/2.0), 
			   p->pickerSize, p->pickerSize);
	}
}




/////////////////////////////////////////////////////////////////////////
//
// REC / PLAY config parameters
//
// TODO: INCORPORAR VECTORS & COLORS!!!!!!
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








/////////////////////////////////////////////////////////////////////////
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
	int id, i;
	bool pp;
	char *p;
	char data[64];
	char key[32];
	char val[CFG_MAX_STR_LEN];
	for (int n = 0 ; fgets(data,64,fp) != NULL ; n++)
	{
		// Comment?
		if (data[0] == '#')
			continue;
		// erase '\n';
		data[strlen(data)-1] = '\0';
		// find separator
		p = strchr(data,':');
		if (p == NULL)
			continue;
		// get val
		if ( (pp = (*(p+1)=='P')) == true)
			strcpy(val, p+2);
		else
			strcpy(val, p+1);
		// get key
		*p = '\0';
		strcpy(key, data);
		printf ("READ CGF: %s = %s",key,val);
		// is there an index in the key?
		if ( (p = strchr(key,'.')) == NULL)
			i = 0;
		else
		{
			*p = '\0';
			i = atoi(p+1);
		}
		
		// Read params
		for (id = 0 ; id < params.size() ; id++ )
		{
			ofxConfigParam *param = params[id];
			if (param == NULL)
				continue;
			if (!strcmp(key,param->name))
			{
				if (pp && param->type != TYPE_STRING)
					this->setProg(id, i, (float)atof(val));
				else
					this->set(id, i, val);
				break;
			}
		}
		// finish output
		if (id == params.size() && strcmp(key,"SAVE_TIME"))
			printf ("  !!!!!!! param do not exist!\n");
		else
			printf ("\n");
		
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
	char name[128];
	char data[128];

	// Open file
	char *fullFilename = (char*)ofToDataPath(filename).c_str();
	FILE *fp = fopen (fullFilename,"w");
	if (fp == NULL)
	{
		printf("ERROR saving config [%s]\n",fullFilename);
		return 0;
	}
	
	// Save comments
	fputs("#\n",fp);
	fputs("# ofxConfig file\n",fp);
	fputs("#\n",fp);
	fputs("# Format:\n",fp);
	fputs("#\tPARAM_NAME[.channel]:[P]value\n",fp);
	fputs("# Examples:\n",fp);
	fputs("#\tPARAM_VALUE:10.0\t(parameter with value)\n",fp);
	fputs("#\tPARAM_PROG:P1.0\t\t(parameter with prog, from 0.0(vmin) to 1.0(vmax))\n",fp);
	fputs("#\tPARAM_VECTOR.0:150.0\t(vector, channel 0 / X)\n",fp);
	fputs("#\tPARAM_COLOR.0:255.0\t(color, channel 0 / RED)\n",fp);
	fputs("#\tPARAM_COLOR.0:P0.5\t(color, channel 0 / RED, with prog)\n",fp);
	fputs("#\n",fp);
	// generate save time
	time_t now;
	time ( &now );
	strcpy(saveTime, &(ctime(&now)[4]));
	sprintf(data,"SAVE_TIME:%s",saveTime);
	fputs(data,fp);
	
	// Save params
	for (int id = 0 ; id < params.size() ; id++ )
	{
		ofxConfigParam *param = params[id];
		if (param == NULL)
			continue;
		for (int i = 0 ; i < (this->isVector(id)?3:1) ; i++)
		{
			if (this->isVector(id))
				sprintf(name, "%s.%d",this->getName(id),i);
			else
				strcpy(name, this->getName(id));
			if (param->vec[i].preserveProg && param->type != TYPE_STRING)
				sprintf(data,"%s:P%f\n",name,this->getProg(id,i));
			else
				sprintf(data,"%s:%s\n",name,this->getString(id,i));
			fputs  (data,fp);
		}
	}
	
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




