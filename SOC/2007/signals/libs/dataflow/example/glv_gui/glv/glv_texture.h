#ifndef INC_GLV_TEXTURE_H
#define INC_GLV_TEXTURE_H

/*	Graphics Library of Views (GLV) - GUI Building Toolkit
	See COPYRIGHT file for authors and license information */

#include "glv_conf.h"

namespace glv{

class Texture2D{
public:
	Texture2D(GLsizei width, GLsizei height, bool doesLoad=false);
	virtual ~Texture2D();

	GLsizei width(){ return w; }
	GLsizei height(){ return h; }

	void bind(){ glBindTexture(GL_TEXTURE_2D, mID); }					// Bind self to current context.
	void load(GLsizei w, GLsizei h, const GLvoid * pixels = 0);	// Resizes texture on graphics card.
	void reload(){ load(w,h); }

private:
	GLuint mID;
	GLsizei w, h;
};


} //glv::

#endif
