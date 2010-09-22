#ifndef OFX_DISPLAYLIST_H
#define OFX_DISPLAYLIST_H

#include "ofMain.h"

class ofxDisplayList{
public:
    ofxDisplayList();
    void begin();
    void end();
    void draw();

    int id;

};

#endif
