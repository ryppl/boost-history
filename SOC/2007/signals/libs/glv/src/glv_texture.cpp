/*	Graphics Library of Views (GLV) - GUI Building Toolkit
	See COPYRIGHT file for authors and license information */

#include "glv_texture.h"

namespace glv{

Texture2D::Texture2D(GLsizei width, GLsizei height, bool doesLoad)
	: mID(0), w(width), h(height)
{
	if(doesLoad) load(w, h);
}

Texture2D::~Texture2D(){
	glDeleteTextures(1, &mID);
}

void Texture2D::load(GLsizei width, GLsizei height, const GLvoid * pixels){
	w = width;
	h = height;
	if(mID) glDeleteTextures(1, &mID);
	glGenTextures(1, &mID); //printf("%i\n", mID);
	glBindTexture(GL_TEXTURE_2D, mID);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

} // glv::
