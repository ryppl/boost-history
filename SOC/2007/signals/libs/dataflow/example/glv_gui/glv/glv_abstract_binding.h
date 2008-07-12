#ifndef INC_GLV_ABSTRACT_BINDING_H
#define INC_GLV_ABSTRACT_BINDING_H

/*	Graphics Library of Views (GLV) - GUI Building Toolkit
	See COPYRIGHT file for authors and license information */

namespace glv{

class GLV;


class AbstractWindow{

public:
	AbstractWindow(unsigned int width=800, unsigned int height=600, char * title="GLV Window", GLV * glv=0, double framerate=40.0);
	virtual ~AbstractWindow(){}

	void fullscreen(bool on);
	void fullscreenToggle();
	void resize(int w, int h);
	void show(bool v);
	void hideCursor(bool hide=true);

	void setGLV(GLV & g);

	double fps(){ return mFPS; }
	unsigned int height(){ return h; }
	unsigned int width(){ return w; }

	GLV * glv;
	
protected:	
	double mFPS;
	char * mLabel;
	unsigned int w, h, wwin, hwin;
	bool mFullscreen;
	bool mVisible;
	bool mIsActive;
	bool mHideCursor;
	
	bool shouldDraw();	// if the GLV views should be drawn

	// These virtuals should be overridden for the specific bindings
	virtual void platformFullscreen(){}
	virtual void platformHideCursor(bool hide){}
	virtual void platformResize(int width, int height){}	// platform specific resize stuff
	virtual void platformShowHide(){}						// platform specific show/hide stuff
};



class AbstractApplication{
public:

	static void			run();
	static void			quit();
//	static void			windowNotify(Notifier * sender, void * userdata);

protected:
	static AbstractWindow *focusedWindow;
};


} // glv::

#endif

