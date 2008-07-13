/*	Graphics Library of Views (GLV) - GUI Building Toolkit
	See COPYRIGHT file for authors and license information */

#include <stdio.h>
#include "glv_core.h"
#include "glv_abstract_binding.h"

namespace glv{

void AbstractApplication::run(){
	//printf("GLV Application run()\n");
	GLV_PLATFORM_INIT;
}

void AbstractApplication::quit(){
	//printf("GLV Application quit()\n");
	//GLV::exit();
}



AbstractWindow::AbstractWindow(unsigned int w, unsigned int h, char * title, GLV * glv, double framerate)
:	glv(0), mFPS(framerate),
	mLabel(title), 
	w(w), h(h),
	mFullscreen(false), mVisible(true), mIsActive(true)
{
	if(glv) setGLV(*glv);
}

void AbstractWindow::setGLV(GLV & g){ 
	glv = &g;
	g.extent(w, h);
}

void AbstractWindow::fullscreen(bool on){
	if(on && !mFullscreen){
		wwin = w, hwin = h;
		mFullscreen = true;
		platformFullscreen();
	}
	else if(!on && mFullscreen){
		//resize(wwin, hwin);
		w = wwin; h = hwin;
		if(glv) glv->extent(w, h);
		mFullscreen = false;
		platformFullscreen();
	}
}

void AbstractWindow::fullscreenToggle(){ fullscreen(!mFullscreen); }

void AbstractWindow::hideCursor(bool hide){
	mHideCursor = hide;
	platformHideCursor(hide);
}

void AbstractWindow::resize(int width, int height){
	w = width; h = height;
	if(glv) glv->extent(w, h);
	platformResize(w, h);
}

bool AbstractWindow::shouldDraw(){ return glv && mIsActive && mVisible; }

void AbstractWindow::show(bool v){
	mVisible = v;
	platformShowHide();
}

} // glv::
