#include "ofxDisplayList.h"

ofxDisplayList::ofxDisplayList(){
    id = glGenLists(1);
}
void ofxDisplayList::begin() {
	glNewList(id, GL_COMPILE);
}
void ofxDisplayList::end(){
	glEndList();
}
void ofxDisplayList::draw(){
    glCallList(id);
}
