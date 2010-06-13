#ifndef _OF_TYPES
#define _OF_TYPES

#include "ofConstants.h"

//----------------------------------------------------------
// ofPoint
//----------------------------------------------------------
class ofPoint {
  public:
	ofPoint(){ x = y = z = 0.0f; }
	virtual ~ofPoint(){}

	ofPoint( float _x, float _y, float _z=0.0f ) {
        x = _x;
        y = _y;
        z = _z;
    }

    ofPoint( const ofPoint & pnt){
        x = pnt.x;
        y = pnt.y;
        z = pnt.z;
    }

    void set(float _x, float _y, float _z = 0){
        x = _x;
        y = _y;
        z = _z;
    }
	
    void set(const ofPoint & pnt){
        x = pnt.x;
        y = pnt.y;
        z = pnt.z;
    }
	
	
	//------ Operators:

  	//Negative
    ofPoint operator-() const {
        return ofPoint( -x, -y, -z );
    }

    //equality
    bool operator==( const ofPoint& pnt ) {
        return (x == pnt.x) && (y == pnt.y) && (z == pnt.z);
    }

	//inequality
    bool operator!=( const ofPoint& pnt ) {
        return (x != pnt.x) || (y != pnt.y) || (z != pnt.z);
    }

	//Set
	ofPoint & operator=( const ofPoint& pnt ){
		x = pnt.x;
		y = pnt.y;
		z = pnt.z;
		return *this;
	}

	ofPoint & operator=( const float& val ){
		x = val;
		y = val;
		z = val;
		return *this;
	}

	// Add
    ofPoint operator+( const ofPoint& pnt ) const {
        return ofPoint( x+pnt.x, y+pnt.y, z+pnt.z );
    }

    ofPoint operator+( const float& val ) const {
        return ofPoint( x+val, y+val, z+val );
    }

	ofPoint & operator+=( const ofPoint& pnt ) {
        x+=pnt.x;
        y+=pnt.y;
        z+=pnt.z;
        return *this;
    }

	ofPoint & operator+=( const float & val ) {
        x+=val;
        y+=val;
        z+=val;
        return *this;
    }

	// Subtract
    ofPoint operator-(const ofPoint& pnt) const {
        return ofPoint( x-pnt.x, y-pnt.y, z-pnt.z );
    }

    ofPoint operator-(const float& val) const {
        return ofPoint( x-val, y-val, z-val);
    }

    ofPoint & operator-=( const ofPoint& pnt ) {
        x -= pnt.x;
        y -= pnt.y;
        z -= pnt.z;
        return *this;
    }

    ofPoint & operator-=( const float & val ) {
        x -= val;
        y -= val;
        z -= val;
        return *this;
    }

	// Multiply
    ofPoint operator*( const ofPoint& pnt ) const {
        return ofPoint( x*pnt.x, y*pnt.y, z*pnt.z );
    }

    ofPoint operator*(const float& val) const {
        return ofPoint( x*val, y*val, z*val);
    }

    ofPoint & operator*=( const ofPoint& pnt ) {
        x*=pnt.x;
        y*=pnt.y;
        z*=pnt.z;
        return *this;
    }

    ofPoint & operator*=( const float & val ) {
        x*=val;
        y*=val;
        z*=val;
        return *this;
    }


	// Divide
    ofPoint operator/( const ofPoint& pnt ) const {
        return ofPoint( pnt.x!=0 ? x/pnt.x : x , pnt.y!=0 ? y/pnt.y : y, pnt.z!=0 ? z/pnt.z : z );
    }

    ofPoint operator/( const float &val ) const {
		if( val != 0){
			return ofPoint( x/val, y/val, z/val );
		}
        return ofPoint(x, y, z );
    }

    ofPoint& operator/=( const ofPoint& pnt ) {
        pnt.x!=0 ? x/=pnt.x : x;
        pnt.y!=0 ? y/=pnt.y : y;
        pnt.z!=0 ? z/=pnt.z : z;
        return *this;
    }

    ofPoint& operator/=( const float &val ) {
		if( val != 0 ){
			x /= val;
			y /= val;
			z /= val;
		}
		return *this;
    }

	// union allows us to access the coordinates through
    // both an array 'v' and 'x', 'y', 'z' member varibles
    union  {
        struct {
            float x;
            float y;
            float z;
        };
        float v[3];
    };

};

//----------------------------------------------------------
// ofRectangle
//----------------------------------------------------------

class ofRectangle {
  public:
	ofRectangle(){ x = y = width = height = 0.0f; };
	virtual ~ofRectangle(){}

	ofRectangle(float _x, float _y, float _w, float _h){
		x = _x;
		y = _y;
		width = _w;
		height = _h;
	}
	ofRectangle(const ofRectangle & r){
		x = r.x;
		y = r.y;
		width = r.width;
		height = r.height;
	}

    void set(float _x, float _y, float _w, float _h){
		x = _x;
		y = _y;
		width = _w;
		height = _h;
    }
	
    void set(const ofRectangle & r){
		x = r.x;
		y = r.y;
		width = r.width;
		height = r.height;
    }
	
	//------ Operators:
	
	//Set
	ofRectangle & operator=( const ofRectangle& rect ){
		x = rect.x;
		y = rect.y;
		width = rect.width;
		height = rect.height;
		return *this;
	}
	
    //equality
    bool operator==( const ofRectangle& r ) {
        return (x == r.x) && (y == r.y) && (width == r.width) && (height == r.height);
    }
	
	//inequality
    bool operator!=( const ofRectangle& r ) {
        return (x != r.x) || (y != r.y) || (width != r.width) || (height != r.height);
    }

	float x;
	float y;
	float width;
	float height;
};



//----------------------------------------------------------
// ofVolume - representation of a 3D cuboid
//----------------------------------------------------------

class ofVolume {
public:
	ofVolume(){ x = y = z = width = height = depth = 0.0; };
	virtual ~ofVolume(){}

	ofVolume(float _x, float _y, float _z, float _w, float _h, float _d){
		x = _x;
		y = _y;
		z = _z;
		width = _w;
		height = _h;
		depth = _d;
	}
	
	ofVolume(float _x, float _y, float _z, float _sz){
		x = _x;
		y = _y;
		z = _z;
		width = _sz;
		height = _sz;
		depth = _sz;
	}
	
	ofVolume(const ofVolume& vol){
		x = vol.x;
		y = vol.y;
		z = vol.z;
		width = vol.width;
		height = vol.height;
		depth = vol.depth;
	}
	
	//Set
	void set(float _x, float _y, float _z, float _w, float _h, float _d){
		x = _x;
		y = _y;
		z = _z;
		width = _w;
		height = _h;
		depth = _d;
    }
	
	void set(float _x, float _y, float _z, float _sz){
		x = _x;
		y = _y;
		z = _z;
		width = _sz;
		height = _sz;
		depth = _sz;
    }
	
	void set(const ofVolume& vol){
		x = vol.x;
		y = vol.y;
		z = vol.z;
		width = vol.width;
		height = vol.height;
		depth = vol.depth;
	}

	//------ Operators:
	
	//Set
	ofVolume & operator=( const ofVolume& vol ){
		x = vol.x;
		y = vol.y;
		z = vol.z;
		width = vol.width;
		height = vol.height;
		depth = vol.depth;
		return *this;
	}
	
    //equality
    bool operator==( const ofVolume& vol ) {
        return (x == vol.x) && (y == vol.y) && (z == vol.z) && (width == vol.width) && (height == vol.height) && (depth == vol.depth);
    }
	
	//inequality
    bool operator!=( const ofVolume& vol ) {
        return (x != vol.x) || (y != vol.y) || (z != vol.z) || (width != vol.width) || (height != vol.height) || (depth != vol.depth);
    }
	
	float x;
	float y;
	float z;
	float width;
	float height;
	float depth;
	
};



//----------------------------------------------------------
// ofColor
//----------------------------------------------------------

class ofColor{
	public:
		ofColor(){
			r = 255.0f;
			g = 255.0f;
			b = 255.0f;
			a = 255.0f;
		}
		virtual ~ofColor(){}

    ofColor( float _r, float _g, float _b , float _a=255.0f ) {
        r = _r;
        g = _g;
        b = _b;
        a = _a;
    }
	
    ofColor( float _gray , float _a=255.0f ) {
        r = g = b = _gray;
        a = _a;
    }
	
	ofColor( const ofColor & color){
        r = color.r;
        g = color.g;
        b = color.b;
        a = color.a;
    }
	
	void set(float _r, float _g, float _b, float _a = 255.0f){
        r = _r;
        g = _g;
        b = _b;
        a = _a;
    }
	
    void set(float _gray, float _a = 255.0f){
        r = g = b = _gray;
        a = _a;
    }
	
    void set(const ofColor & color){
        r = color.r;
        g = color.g;
        b = color.b;
        a = color.a;
    }
	
	// clamp channels between 0.0 and 255.0
    ofColor clamp(){
		r = CLAMP(r, 0.0f, 255.f);
		g = CLAMP(g, 0.0f, 255.f);
		b = CLAMP(b, 0.0f, 255.f);
		a = CLAMP(a, 0.0f, 255.f);
		return *this;
    }
	
	// Return average brightnes (0.0 - 1.0), ignoring alpha
    float getBrightness(){
		return (((r+g+b)/3.0f)/255.0f);
    }
	
	
	//------ Operators:
	
	//Set
	ofColor & operator=( const ofColor& color ){
		r = color.r;
		g = color.g;
		b = color.b;
		a = color.a;
		return *this;
	}
	
	// set grayscale
	ofColor & operator=( const float& val ){
		r = val;
		g = val;
		b = val;
		a = 255.0f;
		return *this;
	}
	
    //equality
    bool operator==( const ofColor& color ) {
        return (r == color.r) && (g == color.g) && (b == color.b);
    }
	
	//inequality
    bool operator!=( const ofColor& color ) {
        return (r != color.r) || (g != color.g) || (b != color.b);
    }
	
	// Add
	// always clamp after to keep channels between 0.0 and 255.0
    ofColor operator+( const ofColor& color ) const {
        return ofColor( r+color.r, g+color.g, b+color.b ).clamp();
    }
	
    ofColor operator+( const float& val ) const {
        return ofColor( r+val, g+val, b+val ).clamp();
    }
	
	ofColor & operator+=( const ofColor& color ) {
        r += color.r;
        g += color.g;
        b += color.b;
		this->clamp();
        return *this;
    }
	
	ofColor & operator+=( const float & val ) {
        r += val;
        g += val;
        b += val;
		this->clamp();
        return *this;
    }
	
	// Subtract
	// always clamp after to keep channels between 0.0 and 255.0
    ofColor operator-(const ofColor& color) const {
        return ofColor( r-color.r, g-color.g, b-color.b ).clamp();
    }
	
    ofColor operator-(const float& val) const {
        return ofColor( r-val, g-val, b-val).clamp();
    }
	
    ofColor & operator-=( const ofColor& color ) {
        r -= color.r;
        g -= color.g;
        b -= color.b;
		this->clamp();
        return *this;
    }
	
    ofColor & operator-=( const float & val ) {
        r -= val;
        g -= val;
        b -= val;
		this->clamp();
        return *this;
    }
	
	//
	// Multiply
	// theory: http://www.digitalartform.com/imageArithmetic.htm
	// color multiplication is only useful when colors are represented as floats 0.0 to 1.0, not as bytes 0 to 255
	// in fact we are talking about percentages here
	// multiplying a color by 0.2 will give us 20% of that color
	// multiplying a color by 0.0 will make it black (0% of it)
	// multiplying a color by 1.0 will return the original color (100% of it)
	// multiplying a color by 2.0 makes no sense (there is no 200%), so we clamp the value to 1.0 (100%)
	// the same apply to each channel, so we must think about each channel separately
	// multiply RED by GREEN will give us black, because the red channel of GREEN will cancel RED, and the green channel of RED will cancel GREEN
    ofColor operator*( const ofColor& color ) const {
        return ofColor( r*(color.r/255.0f), g*(color.g/255.0f), b*(color.b/255.0f) );
    }
	
	// we clamp the value when multiplying by float (faster than clamping each channel)
    ofColor operator*(const float& val) const {
		float v = CLAMP(val,0.0,1.0);
        return ofColor( r*v, g*v, b*v).clamp();
    }
	
    ofColor & operator*=( const ofColor& color ) {
        r *= (color.r/255.0f);
        g *= (color.g/255.0f);
        b *= (color.b/255.0f);
        return *this;
    }
	
    ofColor & operator*=( const float & val ) {
		float v = CLAMP(val,0.0,1.0);
        r *= v;
        g *= v;
        b *= v;
        return *this;
    }
	
	// Divide
	// Same concept of multiplication
	// Dividing anything by WHITE will return the original color
	// Dividing anything by BLACK is impossible (division by zero = core dump!), so will return the original color
	// The same apply for each channel
    ofColor operator/( const ofColor& color ) const {
        return ofColor( color.r!=0.0f ? r/(color.r/255.0f) : r , color.g!=0.0f ? g/(color.g/255.0f) : g, color.b!=0.0f ? b/(color.b/255.0f) : b );
    }
	
	// we clamp the color when divided by float
    ofColor operator/( const float &val ) const {
		if( val != 0.0f)
			return ofColor( r/val, g/val, b/val ).clamp();
		else
			return *this;
    }
	
    ofColor& operator/=( const ofColor& color ) {
        if (color.r!=0.0f) r /= (color.r/255.0f);
        if (color.g!=0.0f) g /= (color.g/255.0f);
        if (color.b!=0.0f) b /= (color.b/255.0f);
        return *this;
    }
	
    ofColor& operator/=( const float &val ) {
		if( val != 0.0f ){
			r /= val;
			g /= val;
			b /= val;
			clamp();
			return *this;
		}
		else
			return *this;
    }
	
	// ROGER
	// union allows us to access the coordinates through
    // both an array 'v' and 'r', 'g', 'b', 'a' member varibles
    union  {
        struct {
			float r, g, b, a;
        };
        float v[4];
    };
	
};

//----------------------------------------------------------
// ofStyle
//----------------------------------------------------------

class ofStyle{
	public:
		ofStyle(){
			bFill				= true;
			blending			= false;
			smoothing			= false;
			circleResolution	= 20;
			lineWidth			= 1.0;
			polyMode			= OF_POLY_WINDING_ODD;
			rectMode			= OF_RECTMODE_CORNER;
		}

		virtual ~ofStyle(){}

		ofColor color;
		int polyMode;
		int rectMode;
		bool bFill;
		bool blending;
		bool smoothing;
		int circleResolution;
		float lineWidth;
};




//----------------------------------------------------------
// ofBaseDraws
//----------------------------------------------------------

class ofBaseDraws{
public:
	virtual ~ofBaseDraws(){}
	virtual void draw(float x,float y)=0;
	virtual void draw(float x,float y,float w, float h)=0;
	virtual float getHeight()=0;
	virtual float getWidth()=0;

	virtual void setAnchorPercent(float xPct, float yPct){};
    virtual void setAnchorPoint(float x, float y){};
	virtual void resetAnchor(){};

};

//----------------------------------------------------------
// ofBaseUpdates
//----------------------------------------------------------

class ofBaseUpdates{
public:
	virtual ~ofBaseUpdates(){}
	virtual void update()=0;
};


//----------------------------------------------------------
// ofBaseHasTexture
//----------------------------------------------------------
class ofTexture;

class ofBaseHasTexture{
public:
	virtual ~ofBaseHasTexture(){}
	virtual ofTexture & getTextureReference()=0;
	virtual void setUseTexture(bool bUseTex)=0;
};

//----------------------------------------------------------
// ofBaseHasPixels
//----------------------------------------------------------
class ofBaseHasPixels{
public:
	virtual ~ofBaseHasPixels(){}
	virtual unsigned char * getPixels()=0;
};

//----------------------------------------------------------
// ofBaseImage
//----------------------------------------------------------
class ofBaseImage: public ofBaseDraws, public ofBaseHasTexture, public ofBaseHasPixels{
public:

};

//----------------------------------------------------------
// ofBaseVideo
//----------------------------------------------------------
class ofBaseVideo: public ofBaseImage, public ofBaseUpdates{
public:
	virtual ~ofBaseVideo(){}
	virtual unsigned char * getPixels()=0;
	virtual bool isFrameNew()=0;
	virtual void close()=0;
};
#endif
