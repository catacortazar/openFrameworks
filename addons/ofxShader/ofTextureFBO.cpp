
#include "ofTextureFBO.h"

//----------------------------------------------------------
ofTextureFBO::~ofTextureFBO() {
	glDeleteFramebuffersEXT(1, &fbo);
	glDeleteRenderbuffersEXT(1, &depthBuffer);
}

//----------------------------------------------------------
void ofTextureFBO::allocate(int w, int h, int internalGlDataType){
	this->allocate(w, h, internalGlDataType, ofGetUsingArbTex());
}
void ofTextureFBO::allocate(int w, int h, int internalGlDataType, bool bUseARBExtention){
	ofTexture::allocate(w, h, internalGlDataType, bUseARBExtention);
	this->allocateFBO();
	texData.bFlipTexture = true;
}

//----------------------------------------------------------
void ofTextureFBO::allocateFBO(){
	// create & setup FBO
	glGenFramebuffersEXT(1, &fbo);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
	
	// Create the render buffer for depth
	glGenRenderbuffersEXT(1, &depthBuffer);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthBuffer);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, texData.tex_w, texData.tex_h);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthBuffer);
	printf("fbo[%d] depth[%d]\n", fbo, depthBuffer);
	
	// attach it to the FBO so we can render to it
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, texData.textureTarget, (GLuint)texData.textureID, 0);
	
	// Check new framebuffer
	if( this->checkFBO() == false ) {
		std::exit(1);
	}
	
	// back to screen framebuffer
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

// Start to render on the FBO
//----------------------------------------------------------
void ofTextureFBO::begin() {
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);	// bind the FBO to the screen so we can draw to it
	glPushAttrib(GL_VIEWPORT_BIT);
    glViewport(0,0,texData.tex_w,texData.tex_h);
}

// Go back to screen framebuffer
//----------------------------------------------------------
void ofTextureFBO::end() {
	glPopAttrib();
	glGenerateMipmapEXT(texData.textureTarget);		// Generate mipmaps manually
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);	// unbind the FBO
}


// Check for errors
//----------------------------------------------------------
bool ofTextureFBO::checkFBO()
{
	GLenum status = (GLenum) glCheckFramebufferStatusEXT( GL_FRAMEBUFFER_EXT );
	switch(status)
	{
		case GL_FRAMEBUFFER_COMPLETE_EXT:
			return true;
		case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
			cout<<"ofTextureFBO:: Failed w/ status ["<<status<<"] OpenGL framebuffer format not supported. "<<endl;
			return false;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
			cout<<"ofTextureFBO:: Failed w/ status ["<<status<<"] OpenGL framebuffer incomplete attachment. "<<endl;
			return false;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
			cout<<"ofTextureFBO:: Failed w/ status ["<<status<<"] OpenGL framebuffer missing attachment."<<endl;
			return false;
		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
			cout<<"ofTextureFBO:: Failed w/ status ["<<status<<"] OpenGL framebuffer attached images must have same dimensions."<<endl;
			return false;
		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
			cout<<"ofTextureFBO:: Failed w/ status ["<<status<<"] OpenGL framebuffer attached images must have same format."<<endl;
			return false;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
			cout<<"ofTextureFBO:: Failed w/ status ["<<status<<"] OpenGL framebuffer missing draw buffer."<<endl;
			return false;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
			cout<<"ofTextureFBO:: Failed w/ status ["<<status<<"] OpenGL framebuffer missing read buffer."<<endl;
			return false;
		default:
			cout<<"ofTextureFBO:: Failed w/ status ["<<status<<"] (generic). Maybe your graphics card doesnt support the framebuffer extension? If you are running osx prior to system 10.5, that could be the cause."<<endl;
			return false;
	}
	return true;
}

