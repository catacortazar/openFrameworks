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
//  Manage configuration parameters + save as text file
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
// - send string types on sendOsc()
//

#include "ofxConfig.h"
#include <exception>
#include <errno.h>

// comment to disable versosity
#define VERBOSE

ofxConfig::ofxConfig()
{
	// init
	saveTime = "using defaults";
	bCallback = false;
	currentFileName = this->getFullFilename("config.cfg");
	
	// init MIDI
	midiPort = 0;
	midiChannel = 1;
#ifdef CFG_USE_OFXMIDI
	midiOut = new ofxMidiOut();
	midiOut->setVerbose(false);
	midiIn = new ofxMidiIn();
	midiIn->addListener(this);
	midiIn->setVerbose(false);
	// midi comm
	midiPort = midiIn->findMyDevicePort();
	midiOut->openPort(midiPort);
	midiIn->openPort(midiPort);
#endif
	
	// init OSC
#ifdef CFG_USE_OFXOSC
	bOscSend = false;
	bOscReceive = false;
#ifdef CFG_CATCH_LOOP_EVENTS
	ofAddListener(ofEvents.update, this, &ofxConfig::oscCallback);
#endif
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
#ifdef CFG_USE_OFXMIDI
	// free midi
	delete midiIn;
	delete midiOut;
#endif
}

//
// Update state
//
void ofxConfig::update()
{
#ifdef CFG_USE_OFXOSC
#ifndef CFG_CATCH_LOOP_EVENTS
	this->oscCallback();
#endif
#endif
}


/////////////////////////////////////////////////////////////////////////
//
// MIDI
//
void ofxConfig::setMidi(int id, short note)
{
	// Color: One note for all channels
	if (params[id]->type == CFG_TYPE_COLOR)
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
#ifdef CFG_USE_OFXMIDI
void ofxConfig::newMidiMessage(ofxMidiEventArgs& eventArgs)
{
	int channel = eventArgs.channel;
	int note = eventArgs.byteOne;
	int val = eventArgs.byteTwo;
	float prog = ((float)val / 127.0);
	this->parseMidiMessage(channel, note, prog);
}
#endif
//
// PARSE MIDI message
// used by OSC as well
void ofxConfig::parseMidiMessage(int channel, int note, float prog)
{
	printf("MIDI IN ch[%d] note[%d] = [%.2f]",channel,note,prog);
	for (int id = 0 ; id < params.size() ; id++)
	{
		if (params[id] == NULL)
			continue;
		ofxConfigParam *p = params.at(id);
		for (int i = 0 ; i < 3 ; i++)
		{
			if (p->vec[i].midiNote == note)
			{
				this->setProg(id, i, prog);
				printf(" >> CFG id[%d/%d] = [%.3f] (%s)\n",id,i,p->vec[i].get(),p->name.c_str());
				return;
			}
		}
	}
	printf(" >> unassigned\n");
}

//////////////////////////////////////////////////
//
// OSC
//
#ifdef CFG_USE_OFXOSC
//
// Enable/disable OSC
// Send every param change thu OSC to another app with ofxConfig
void ofxConfig::setOscSend(bool b, int port)
{
	bOscSend = b;
	bOscReceive = !b;
	this->switchOsc(port);
}
//
// Enable/disable OSC
// Receive params change from another app with ofxConfig
// Receive MIDI as OSC from MidiAsOsc.app
void ofxConfig::setOscReceive(bool b, int port)
{
	bOscReceive = b;
	bOscSend = !b;
	this->switchOsc(port);
}
// Turn OSC on/off
void ofxConfig::switchOsc(int port)
{
	try {
		if (bOscReceive)
		{
#ifndef CINDER
			oscSender.shutdown();
#endif
			oscReceiver.setup( port );
			sprintf(errmsg, "OSC RECEIVING PORT %d...",port);
		}
		else if (bOscSend)
		{
#ifndef CINDER
			oscReceiver.shutdown();
#endif
			oscSender.setup( DEFAULT_OSC_HOST, port );
			sprintf(errmsg, "OSC SENDING...");
		}
	} catch (exception &error) {
		sprintf(errmsg, "OSC FAILED!!!!");
		printf("OSC SETUP FAILED!!!\n");
	}
}
//
// Send params change to another app with ofxConfig
void ofxConfig::sendOsc(short id, short i)
{
	// OSC Receive Disabled?
	if (bOscSend == false)
		return;
	// Do not send string
	if (params[id]->type == CFG_TYPE_STRING)
		return;
	// Send OSC
	string addr = "/";
	addr += this->getName(id);
	ofxOscMessage m;
	m.setAddress( addr );
	m.addIntArg( (int)i );
	if (params[id]->vec[i].preserveProg)
	{
		m.addStringArg( "P" );
		m.addFloatArg( this->getProg(id, i) );
	}
	else
	{
		m.addStringArg( "V" );
		m.addFloatArg( this->get(id, i) );
	}
	oscSender.sendMessage( m );
#ifdef OSC_VERBOSE
	printf("OSC OUT >> id[%d/%d] = [%.3f] (%s)\n",id,i,this->getProg(id, i),this->getName(id));
#endif
}
//
// PUSH ALL PARAMS to another app with ofxConfig
void ofxConfig::pushOsc()
{
	for (int id = 0 ; id < params.size() ; id++)
	{
		if (params[id] == NULL)
			continue;
		for (int i = 0 ; i < 3 ; i++)
			sendOsc(id, i);
	}
}
//
// ofxOsc callback
// - Reads MIDI messages disguised as OSC forwarded from MidiAsOsc.app
// - Reads PARAMS sent from another app using odxConfig
#ifdef CFG_CATCH_LOOP_EVENTS
void ofxConfig::oscCallback(ofEventArgs &e)
{
	this->oscCallback();
}
#endif
void ofxConfig::oscCallback()
{
	// OSC Receive Disabled?
	if (bOscReceive == false)
		return;
#ifdef OSC_VERBOSE
	sprintf(errmsg, "OSC CALLBACK");
#endif
	// check for waiting messages
	while( oscReceiver.hasWaitingMessages() )
	{
		// get the next message
		ofxOscMessage m;
		oscReceiver.getNextMessage( &m );
		
#ifdef OSC_VERBOSE
		sprintf(errmsg, "OSC MSG [%s]", m.getAddress().c_str());
#endif
		// Receive from MisiAsOsc.app
		// Format:	/midi channel note value
		if ( m.getAddress() == "/midi" )
		{
			// both the arguments are int32's
			int channel	= m.getArgAsInt32( 0 );
			int note	= m.getArgAsInt32( 1 );
			float val	= m.getArgAsFloat( 2 );
#ifdef OSC_VERBOSE
			printf("OSC IN: /midi channel[%d] note[%d] value[%f]\n",channel,note,val);
#endif
			this->parseMidiMessage(channel, note, val);
		}
		// Receive from another ofxConfig
		// Format:	/PARAM_NAME ix P/V value
		else
		{
			const char *name = m.getAddress().substr(1).c_str();
			for (int id = 0 ; id < params.size() ; id++)
			{
				if (params[id] == NULL)
					continue;
				ofxConfigParam *p = params.at(id);
				if (name == p->name)
				{
					int i		= m.getArgAsInt32( 0 );
					string pv	= m.getArgAsString( 1 );
					float val	= m.getArgAsFloat( 2 );
					if (pv[0] == 'P')
						this->setProg(id, i, val);
					else // value
						this->set(id, i, val);
#ifdef OSC_VERBOSE
					printf("OSC IN >> id[%d/%d] = [%.3f] (%s)\n",id,i,p->vec[i].get(),p->name.c_str());
#endif
					sprintf(errmsg, "OSC TO %s=%.3f", p->name.c_str(), p->vec[i].get());
					return;
				}
			}
		}
	}
	
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
		case CFG_TYPE_FLOAT:
		case CFG_TYPE_DOUBLE:
		case CFG_TYPE_INTEGER:
		case CFG_TYPE_LONG:
		case CFG_TYPE_BYTE:
		case CFG_TYPE_COLOR:
		case CFG_TYPE_VECTOR:
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
		case CFG_TYPE_COLOR:
		case CFG_TYPE_VECTOR:
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
void ofxConfig::add(short id, const char *name, float val)
{
	this->addFloat(id, name, val);
}
// specific
void ofxConfig::addFloat(short id, const char *name, float val)
{
	this->pushParam(id, new ofxConfigParam(CFG_TYPE_FLOAT, name));
	this->set(id, val);
}
void ofxConfig::addDouble(short id, const char *name, double val)
{
	this->pushParam(id, new ofxConfigParam(CFG_TYPE_DOUBLE, name));
	this->set(id, val);
}
void ofxConfig::addInt(short id, const char *name, int val)
{
	this->pushParam(id, new ofxConfigParam(CFG_TYPE_INTEGER, name));
	this->set(id, val);
}
void ofxConfig::addLong(short id, const char *name, long val)
{
	this->pushParam(id, new ofxConfigParam(CFG_TYPE_LONG, name));
	this->set(id, val);
}
void ofxConfig::addBool(short id, const char *name, bool val)
{
	this->pushParam(id, new ofxConfigParam(CFG_TYPE_BOOLEAN, name));
	this->setLimits(id, 0.0f, 1.0f);
	this->set(id, val );
}
void ofxConfig::addByte(short id, const char *name, unsigned char val)
{
	this->pushParam(id, new ofxConfigParam(CFG_TYPE_BYTE, name));
	this->setLimits(id, (float)0, (float)255);
	this->set(id, (float) val);
}
void ofxConfig::addString(short id, const char *name, const char *val)
{
	this->pushParam(id, new ofxConfigParam(CFG_TYPE_STRING, name));
	this->set(id, val);
}
#ifndef CINDER
void ofxConfig::addColor(short id, const char *name, int hex)
{
	this->addColor(id, name, ofColor(hex));
}
#endif
void ofxConfig::addColor(short id, const char *name, ofColor c)
{
	this->addColor(id, name, c.r, c.g, c.b);
}
void ofxConfig::addColor(short id, const char *name, float r, float g, float b)
{
	this->pushParam(id, new ofxConfigParam(CFG_TYPE_COLOR, name));
	this->setLimitsR(id, 0.0f, 255.0f);
	this->setLimitsG(id, 0.0f, 255.0f);
	this->setLimitsB(id, 0.0f, 255.0f);
	this->set(id, r, g, b);
}
void ofxConfig::addVector(short id, const char *name, ofPoint p)
{
	this->addVector(id, name, p.x, p.y, p.z);
}
void ofxConfig::addVector(short id, const char *name, float x, float y, float z)
{
	this->pushParam(id, new ofxConfigParam(CFG_TYPE_VECTOR, name));
#ifdef CINDER
	this->setLimitsX(id, 0.0f, getWindowWidth());
	this->setLimitsY(id, 0.0f, getWindowHeight());
#else
	this->setLimitsX(id, 0.0f, ofGetWidth());
	this->setLimitsY(id, 0.0f, ofGetHeight());
#endif
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
//
// Called after any set
void ofxConfig::post_set(int id, int i)
{
	// Set as fresh
	freshness = true;
	// Call callback
	if (bCallback)
		this->callbackFunc(id, i);
	// Send OSC
#ifdef CFG_USE_OFXOSC
	this->sendOsc(id, i);
#endif
}
//
// PROG
//
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
	this->post_set(id, i);
}
void ofxConfig::setProg(int id, int i, const char *val)		// Private
{
	this->setProg(id, i, (float)atof(val));
}
void ofxConfig::setProg(int id, float p) { this->setProg(id,0,p); }
void ofxConfig::setProgR(int id, float p) { this->setProg(id,0,p); }
void ofxConfig::setProgG(int id, float p) { this->setProg(id,1,p); }
void ofxConfig::setProgB(int id, float p) { this->setProg(id,2,p); }
void ofxConfig::setProgX(int id, float p) { this->setProg(id,0,p); }
void ofxConfig::setProgY(int id, float p) { this->setProg(id,1,p); }
void ofxConfig::setProgZ(int id, float p) { this->setProg(id,2,p); }
void ofxConfig::setProg(int id, float val0, float val1, float val2)
{
	this->setProg(id,0,val0);
	this->setProg(id,1,val1);
	this->setProg(id,2,val2);
}
// generic (float)
// val1, val2 will be used only on CFG_TYPE_VECTOR and CFG_TYPE_COLOR
void ofxConfig::set(int id, int i, float val)		// Private
{
	ofxConfigParam *p = params[id];
	switch (p->type)
	{
		case CFG_TYPE_FLOAT:
		case CFG_TYPE_DOUBLE:
		case CFG_TYPE_INTEGER:
		case CFG_TYPE_LONG:
		case CFG_TYPE_BOOLEAN:
		case CFG_TYPE_BYTE:
		case CFG_TYPE_COLOR:
		case CFG_TYPE_VECTOR:
			p->vec[i].set(val);
			break;
		case CFG_TYPE_STRING:
		{
			std::ostringstream os;
			os << val;
			p->strval = os.str();
			break;
		}
		default:
			printf("Config.set(int) ERROR invalid id[%d] type[%d] val[%d]\n",id,p->type,(int)val);
			return;
			break;
	};
	this->post_set(id, i);
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
// If CFG_TYPE_STRING, just set
// Else, convert to float
// pp = Prog?
void ofxConfig::set(int id, const char *val, bool pp)
{
	// Set string!
	if (params[id]->type == CFG_TYPE_STRING)
		params[id]->strval = val;
	// Set vector values
	// val = "number,number,number"
	else if (this->isVector(id))
	{
		char *p1, *p2;
		char vals[3][16];
		memset(vals,0,sizeof(vals));
		// val1
		if ((p1 = strchr(val,',')) != NULL)
		{
			strncpy(vals[0],val,(p1-val));
			if ((p2 = strchr(p1+1,',')) != NULL)
			{
				strncpy(vals[1],p1+1,(p2-p1));
				strcpy(vals[2],p2+1);
			}
		}
		// set!
		if (pp)
			this->setProg(id, atof(vals[0]), atof(vals[1]), atof(vals[2]));
		else
			this->set(id, atof(vals[0]), atof(vals[1]), atof(vals[2]));
	}
	// defualt: float
	else if (pp)
		this->setProg(id, 0, val);
	else
		this->set(id, 0, val);
}
void ofxConfig::set(int id, int i, const char *val)		// Private
{
	this->set(id, i, (float)atof(val));
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
	this->set(id,0,val0);
	this->set(id,1,val1);
	this->set(id,2,val2);
}
//
// Vaule labels for radio buttons
// Works for CFG_TYPE_INTEGER
// TERMINATE WITH NULL
// example: this->setValueLabels(THE_SHAPE, "Sphere", "Cube", NULL);
void ofxConfig::setValueLabels(short id, const char *val0, ...)
{
	ofxConfigParam *param = params[id];
	va_list args;
	va_start (args, val0);
	char *p = (char*) val0;
	do {
		//printf("label %s\n",p);
		param->valueLabels.push_back( string(p) );
	} while ( p = va_arg (args, char *) );
	va_end (args);
}
// accept labels like this...
//	char labels[][64] = 
//	{
//		"640 x 480",
//		"1024 x 768",
//		"1280 x 1024",
//		"1920 x 1080",
//		NULL
//	};
void ofxConfig::setValueLabels(short id, const char labels[][64])
{
	for (int n = 0 ; labels[n][0] != '\0' ; n++)
		params[id]->valueLabels.push_back( string(labels[n]) );
}
// SETTER / GETER
void ofxConfig::setValueLabel(short id, int item, const char label[64])
{
	ofxConfigParam *param = params[id];
	if ( item < param->valueLabels.size())
		param->valueLabels[item].assign(label);
}
const char* ofxConfig::getValueLabel(short id, int item)
{
	ofxConfigParam *param = params[id];
	if ( item < param->valueLabels.size())
		return (char*) param->valueLabels[item].c_str();
	else
		return this->getString(id);
}


/////////////////////////////////////////////////////////////////////////
//
// OPERATIONS
//
// Invert a value
void ofxConfig::invert(int id, short i)		// Private
{
	params[id]->vec[i].invert();
	this->post_set(id, i);
}
void ofxConfig::invert(int id)
{
	if (this->isVector(id))
	{
		this->invert(id, 0);
		this->invert(id, 1);
		this->invert(id, 2);
	}
	else
		this->invert(id, 0);
}
// invert one channel
void ofxConfig::invertR(int id) { this->invert(id, 0); }
void ofxConfig::invertG(int id) { this->invert(id, 1); }
void ofxConfig::invertB(int id) { this->invert(id, 2); }
void ofxConfig::invertX(int id) { this->invert(id, 0); }
void ofxConfig::invertY(int id) { this->invert(id, 1); }
void ofxConfig::invertZ(int id) { this->invert(id, 2); }
//
// Subtract a value
void ofxConfig::sub(int id, int i, float val, bool clamp)	// Private
{
	params[id]->vec[i].sub(val, clamp);
	this->post_set(id, i);
}
void ofxConfig::sub(int id, float val, bool clamp)
{
	if (this->isVector(id))
	{
		this->sub(id, 0, val, clamp);
		this->sub(id, 1, val, clamp);
		this->sub(id, 2, val, clamp);
	}
	else
		this->sub(id, 0, val, clamp);
}
void ofxConfig::subX(int id, float val, bool clamp) { this->sub(id, 0, val, clamp); }
void ofxConfig::subY(int id, float val, bool clamp) { this->sub(id, 1, val, clamp); }
void ofxConfig::subZ(int id, float val, bool clamp) { this->sub(id, 2, val, clamp); }
void ofxConfig::subR(int id, float val, bool clamp) { this->sub(id, 0, val, clamp); }
void ofxConfig::subG(int id, float val, bool clamp) { this->sub(id, 1, val, clamp); }
void ofxConfig::subB(int id, float val, bool clamp) { this->sub(id, 2, val, clamp); }
// dec = sub(1.0)
void ofxConfig::dec(int id, bool clamp) { this->sub(id, 0, 1.0f, clamp); }
void ofxConfig::decX(int id, bool clamp) { this->sub(id, 0, 1.0f, clamp); }
void ofxConfig::decY(int id, bool clamp) { this->sub(id, 1, 1.0f, clamp); }
void ofxConfig::decZ(int id, bool clamp) { this->sub(id, 2, 1.0f, clamp); }
void ofxConfig::decR(int id, bool clamp) { this->sub(id, 0, 1.0f, clamp); }
void ofxConfig::decG(int id, bool clamp) { this->sub(id, 1, 1.0f, clamp); }
void ofxConfig::decB(int id, bool clamp) { this->sub(id, 2, 1.0f, clamp); }
//
// Add  a value
void ofxConfig::add(int id, int i, float val, bool clamp)	// Private
{
	params[id]->vec[i].add(val, clamp);
	this->post_set(id, i);
}
void ofxConfig::add(int id, float val, bool clamp)
{
	if (this->isVector(id))
	{
		this->add(id, 0, val, clamp);
		this->add(id, 1, val, clamp);
		this->add(id, 2, val, clamp);
	}
	else
		this->add(id, 0, val, clamp);
}
void ofxConfig::addX(int id, float val, bool clamp) { this->add(id, 0, val, clamp); }
void ofxConfig::addY(int id, float val, bool clamp) { this->add(id, 1, val, clamp); }
void ofxConfig::addZ(int id, float val, bool clamp) { this->add(id, 2, val, clamp); }
void ofxConfig::addR(int id, float val, bool clamp) { this->add(id, 0, val, clamp); }
void ofxConfig::addG(int id, float val, bool clamp) { this->add(id, 1, val, clamp); }
void ofxConfig::addB(int id, float val, bool clamp) { this->add(id, 2, val, clamp); }
// inc = add(1.0)
void ofxConfig::inc(int id, bool clamp) { this->add(id, 0, 1.0f, clamp); }
void ofxConfig::incX(int id, bool clamp) { this->add(id, 0, 1.0f, clamp); }
void ofxConfig::incY(int id, bool clamp) { this->add(id, 1, 1.0f, clamp); }
void ofxConfig::incZ(int id, bool clamp) { this->add(id, 2, 1.0f, clamp); }
void ofxConfig::incR(int id, bool clamp) { this->add(id, 0, 1.0f, clamp); }
void ofxConfig::incG(int id, bool clamp) { this->add(id, 1, 1.0f, clamp); }
void ofxConfig::incB(int id, bool clamp) { this->add(id, 2, 1.0f, clamp); }



/////////////////////////////////////////////////////////////////////////
//
// GETTERS
//
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
ofPoint ofxConfig::getProgVector(int id)
{
	return ofPoint(this->getProg(id, 0), this->getProg(id, 1), this->getProg(id, 2));
}
// Get generic value (float)
float ofxConfig::get(int id, int i)		// Private
{
	ofxConfigParam *p = params[id];
	switch (p->type)
	{
		case CFG_TYPE_FLOAT:
		case CFG_TYPE_DOUBLE:
		case CFG_TYPE_INTEGER:
		case CFG_TYPE_LONG:
		case CFG_TYPE_BOOLEAN:
		case CFG_TYPE_BYTE:
		case CFG_TYPE_COLOR:
		case CFG_TYPE_VECTOR:
			return p->vec[i].get();
			break;
		case CFG_TYPE_STRING:
			return atof(p->strval.c_str());
			break;
		default:
			printf("Config.get(int) ERROR invalid id[%d] type[%d]\n",id,p->type);
			return 0.0f;
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
void ofxConfig::getString(int id, char *str, bool raw)
{
	strcpy(str, this->getString(id));
}
const char* ofxConfig::getString(int id, bool raw)
{
	ofxConfigParam *param = params[id];
	// String type: no conversion
	if (param->type == CFG_TYPE_STRING)
		return (param->strval).c_str();
	// Init string
	bool pp = param->vec[0].preserveProg;
	param->strval = (pp ? "P" : "");
	// Convert to string...
	for (int i = 0 ; i < (this->isVector(id)?3:1) ; i++)
	{
		char val[16];
		if (i > 0)
			param->strval += ",";
		// prog value
		if (pp)
			sprintf(val,"%f",this->getProg(id,i));
		// normal value
		else
		{
			// Format value
			switch(param->type)
			{
				case CFG_TYPE_INTEGER:
					if (raw)
						sprintf(val,"%d",(int)this->get(id));
					else
					{
						int v = (int)this->get(id,i) - (int)this->getMin(id,i);
						if ( v < param->valueLabels.size())
							return (char*) param->valueLabels[v].c_str();
						else
							sprintf(val,"%d",(int)this->get(id));
					}
					break;
				case CFG_TYPE_COLOR:
					sprintf(val,"%d",(int)this->get(id,i));
					break;
				case CFG_TYPE_BYTE:
					if (raw)
						sprintf(val,"%d",(int)this->get(id,i));
					else
						sprintf(val,"%c",(int)this->get(id,i));
					break;
				case CFG_TYPE_LONG:
					sprintf(val,"%ld",(long)this->get(id,i));
					break;
				case CFG_TYPE_BOOLEAN:
					if (raw)
						sprintf(val,"%d",(this->get(id,i) == 0.0 ? 0 : 1));
					else
						sprintf(val,"%s",(this->get(id,i) == 0.0 ? "OFF" : "ON"));
					break;
				default:
					sprintf(val,"%f",this->get(id,i));
					break;
			}
		}
		param->strval += val;
	}
	// return string
	return (param->strval).c_str();
}
ofColor ofxConfig::getColor(int id)
{
	return ofColor(this->get(id,0), this->get(id,1),this->get(id,2));
}
ofPoint ofxConfig::getVector(int id)
{
	return ofPoint(this->get(id,0), this->get(id,1),this->get(id,2));
}
//
// Normalized getters
float ofxConfig::getDegrees(int id)
{
	float v = this->get(id);
	while (v < 0.0f)
		v += 360.0f;
	while (v >= 360.0f)
		v -= 360.0f;
	return v;
}
float ofxConfig::getRadians(int id)
{
	float v = this->get(id);
	while (v < 0.0f)
		v += TWO_PI;
	while (v >= TWO_PI)
		v -= TWO_PI;
	return v;
}



/////////////////////////////////////////////////////////////////////////
//
// SAVE / READ utils
//
// Sets default file and load it
int ofxConfig::setFile(const char *f, const char* path)
{
	currentFileName = this->getFullFilename(f);
}
int ofxConfig::useFile(const char *f, const char* path)
{
	this->setFile(f, path);
	return this->load();
}
//
// Get file name
string ofxConfig::getFullFilename(const char *f, const char* path)
{
	// Use current
	if (f == NULL)
		return currentFileName;
	
	// Make filename
	std::ostringstream os;
	// Cinder file
#ifdef CINDER
#ifdef CINDER_COCOA_TOUCH
	os << getHomeDirectory() << f;
#else
	if (path == NULL)
		os << getHomeDirectory() << "/Dev/Cinder/config/" << f;
	else
		os << getHomeDirectory() << path << f;
#endif
	// openFrameworks File
#else
	if (path == NULL)
	{
#ifdef RELEASE
		os << ofToDataPath(f);			// BUNDLED
#else
		os << "../../../data/" << f;
#endif // release
	}
	else
	{
		os << path << f;
	}
#endif // of
	return os.str();
}
// Get current save time
const char* ofxConfig::getSaveTime()
{
	return saveTime.c_str();
}




/////////////////////////////////////////////////////////////////////////
//
// SAVE / READ config to file
//
// SAVE to file on data folder
// Return saved params
int ofxConfig::readFile(const char *f)
{
	// Open file
	string fullFilename = this->getFullFilename(f);
	FILE *fp = fopen (fullFilename.c_str(),"r");
	if (fp == NULL)
	{
		printf("ERROR reading config file [%s] errno [%d/%s]\n",fullFilename.c_str(),errno,strerror(errno));
		sprintf(errmsg, "READ ERROR! [%s]", fullFilename.c_str());
		return 0;
	}
	
	// Read file
	int pid;
	bool pp;						// is prog?
	char *p;						// aux pointer
	char data[CFG_MAX_DATA_LEN];	// full line
	string key;						// param key
	string val;						// param value
	for (int n = 0 ; fgets(data, sizeof(data) ,fp) != NULL ; n++)
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
			val = string(p+2);
		else
			val = string(p+1);
		// get key
		*p = '\0';
		key = data;
#ifdef VERSOSE
		printf ("READ CFG: %s = %s",key.c_str(),val.c_str());
#endif
		
		// Read params
		for (pid = 0 ; pid < params.size() ; pid++ )
		{
			ofxConfigParam *param = params[pid];
			if (param == NULL)
				continue;
			if ( key == param->name )
			{
				this->preserveProg(pid, pp);
				this->set(pid, val.c_str(), pp);
				break;
			}
		}
#ifdef VERSOSE
		// finish output
		if (pid == params.size() && key != "SAVE_TIME")
			printf ("  !!!!!!! param do not exist!\n");
		else
			printf ("\n");
#endif
		
		// Is saved time?
		if (key == "SAVE_TIME")
			saveTime = val;
	}
	
	// Close file
	fclose (fp);
	printf("READ config [%s] OK!\n",fullFilename.c_str());
	sprintf(errmsg, "READ [%s]", fullFilename.c_str());
	
	// return param count
	return params.size();
}
//
// SAVE to file on data folder
// Return saved params
int ofxConfig::saveFile(const char *f)
{
	// Open file
	string fullFilename = this->getFullFilename(f);
	FILE *fp = fopen (fullFilename.c_str(),"w");
	if (fp == NULL)
	{
		printf("ERROR saving config [%s] errno [%d/%s]\n",fullFilename.c_str(),errno,strerror(errno));
		sprintf(errmsg, "SAVE ERROR!!! [%s]", fullFilename.c_str());
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
	fputs("#\tPARAM_PROG:P1.0\t\t(parameter with prog: from 0.0(vmin) to 1.0(vmax))\n",fp);
	fputs("#\tPARAM_VECTOR:150.0,150.0,0.0\t(vector: x,y,z)\n",fp);
	fputs("#\tPARAM_VECTOR:P0.5,0.5,0.0\t\t(vector with prog: x,y,z)\n",fp);
	fputs("#\tPARAM_COLOR:255,255,255\t\t\t(color: r,g,b)\n",fp);
	fputs("#\tPARAM_COLOR:P0.0,0.5,1.0\t\t(color with prog: r,g,b)\n",fp);
	fputs("#\n",fp);
	
	// generate save time
	char data[CFG_MAX_DATA_LEN];
	time_t now;
	time ( &now );
	saveTime = string(&(ctime(&now)[4]));
	sprintf(data,"SAVE_TIME:%s",saveTime.c_str());
	fputs(data,fp);
	
	// Save params
	for (int id = 0 ; id < params.size() ; id++ )
	{
		ofxConfigParam *param = params[id];
		if (param == NULL)
			continue;
		// make line (name+value)
		sprintf(data,"%s:%s\n",this->getName(id),this->getString(id, true));
		// save line
		fputs  (data,fp);
	}
	
	// Close file
	fclose (fp);
	
	// Ok!
	printf("SAVE config [%s] OK!\n",fullFilename.c_str());
	sprintf(errmsg, "SAVED [%s]", fullFilename.c_str());
	
	// return param count
	return params.size();
}






///////////////////////////////////////////////
//
// COCOA INTEGRATION
//
// Cocoa Slider
void ofxConfig::cocoaSetupSlider(int id, NSSlider *slider)
{
	[slider setMinValue:this->getMin(id)];
	[slider setMaxValue:this->getMax(id)];
	switch (params[id]->type)
	{
		case CFG_TYPE_FLOAT:
		case CFG_TYPE_DOUBLE:
			[slider setFloatValue:this->get(id)];
			break;
		case CFG_TYPE_INTEGER:
		case CFG_TYPE_LONG:
		case CFG_TYPE_BYTE:
			[slider setIntValue:this->getInt(id)];
			break;
	}

}
void ofxConfig::cocoaReadSlider(int id, NSSlider *slider)
{
	this->set(id, [slider floatValue]);
}
//
// Cocoa button
void ofxConfig::cocoaSetupButton(int id, NSButton *button)
{
	[button setState: ( this->getBool(id) ? NSOnState : NSOffState )];
}
void ofxConfig::cocoaReadButton(int id, NSButton *button)
{
	this->set(id, (bool)( [button state] == NSOnState ? true : false ));
}
//
// Cocoa Popup (combo box)
void ofxConfig::cocoaSetupPopup(int id, NSPopUpButton *pop)
{
	int v = this->getInt(id);
	int min = (int)this->getMin(id);
	int max = (int)this->getMax(id);
	[pop removeAllItems];
	for (int n = min ; n <= max ; n++)
	{
		int ix = (n - min);
		if ( ix < params[id]->valueLabels.size())
			[pop addItemWithTitle:STR2NSS(params[id]->valueLabels[ix].c_str())];
		else
			[pop addItemWithTitle:[NSString stringWithFormat:@"%d",n]];
		if (v == n)
			[pop selectItemAtIndex:ix];
		//printf("GUI POP id(%d) ix %d = %d, current %d\n",id,ix,n,v);
	}
}
void ofxConfig::cocoaReadPopup(int id, NSPopUpButton *pop)
{
	int v = (int)[pop indexOfSelectedItem] + (int)this->getMin(id);
	this->set(id, v);
}
//
// Cocoa Segmented
void ofxConfig::cocoaSetupSeg(int id, NSSegmentedControl *seg)
{
	int v = this->getInt(id);
	int min = (int)this->getMin(id);
	int max = (int)this->getMax(id);
	int count = (max-min+1);
	[seg setSegmentCount:count];
	float w = ( ([seg frame].size.width-10) / count );
	for (int n = min ; n <= max ; n++)
	{
		int ix = (n - min);
		[seg setWidth:w forSegment:ix];
		if ( ix < params[id]->valueLabels.size())
			[seg setLabel:STR2NSS(params[id]->valueLabels[ix].c_str()) forSegment:ix];
		else
			[seg setLabel:[NSString stringWithFormat:@"%d",n] forSegment:ix];
		if (v == n)
			[seg setSelectedSegment:ix];
		//printf("GUI SEG id(%d) ix %d = %d, current %d\n",id,ix,n,v);
	}
}
void ofxConfig::cocoaReadSeg(int id, NSSegmentedControl *seg)
{
	int v = (int)[seg selectedSegment] + (int)this->getMin(id);
	this->set(id, v);
}



