#ifndef INC_GLV_SLIDERS_H
#define INC_GLV_SLIDERS_H

/*	Graphics Library of Views (GLV) - GUI Building Toolkit
	See COPYRIGHT file for authors and license information */

#include "glv_core.h"

namespace glv {


// LJP: The constructor for widgets takes a Rect as an argument rather than
// individual l,t,w,h components since it's more flexible for layout and 
// only requires writing one constructor.

/// Icon function type
typedef void (* iconFunc)(float l, float t, float r, float b);

template <int N>
class ButtonBase : public View{
public:

	ButtonBase(const Rect& r=Rect(20), bool toggles=true, iconFunc on=draw::rect, iconFunc off=0, space_t iconInset=2)
	:	View(r), mIconOff(off), mIconOn(on), mIconInset(iconInset), mToggles(toggles)
	{
		valueMin();
	}

	
	ButtonBase& iconInset(space_t v){ mIconInset = v; return *this; }

	/// Set button's inactive icon.
	ButtonBase& iconOff(iconFunc fnc){ mIconOff = fnc; return *this; }
	
	/// Set button's active icon.
	ButtonBase& iconOn(iconFunc fnc){ mIconOn = fnc; return *this; }
	
	ButtonBase& toggles(bool v){ mToggles = v; return *this; }
	
	ButtonBase& value(bool v, int i){ mVals[i] = v; return *this; }
	
	ButtonBase& valueMin(){ memset(mVals, 0, sizeof(bool) * size()); return *this; }
	
	
	space_t iconInset() const { return mIconInset; }
	
	bool toggles() const { return mToggles; }
	
	/// Get value of button
	bool value(int i=0) const { return mVals[i]; }
	
	static int size(){ return N; }


protected:
	iconFunc mIconOff, mIconOn;	// state icons
	space_t mIconInset;			// num pixels to inset icon
	bool mVals[N];				// values
	bool mToggles;				// whether button stays on or off when clicked once
};




/// Button widget
class Button : public ButtonBase<1>{
public:

	using ButtonBase<1>::iconInset;
	using ButtonBase<1>::toggles;
	using ButtonBase<1>::value;
	
	/// Constructor
	Button(const Rect& r=Rect(20), bool toggles=true, iconFunc on=draw::rect, iconFunc off=0);
	
	virtual void onDraw();
	virtual bool onEvent(Event::t e, GLV& glv);
};



template <int Nx, int Ny=1>
class Buttons : public ButtonBase<Nx * Ny>{
public:

	using ButtonBase<Nx * Ny>::iconInset;
	using ButtonBase<Nx * Ny>::toggles;
	using ButtonBase<Nx * Ny>::value;
	using ButtonBase<Nx * Ny>::valueMin;
	using ButtonBase<Nx * Ny>::w;
	using ButtonBase<Nx * Ny>::h;
	using ButtonBase<Nx * Ny>::colors;

	/// Constructor
	Buttons(const Rect& r=Rect(20), bool mutExc=false, iconFunc on=draw::rect, iconFunc off=0);
	
	int lastClicked() const;

	virtual void onDraw();
	virtual bool onEvent(Event::t e, GLV& glv);

protected:

	using ButtonBase<Nx * Ny>::mIconOn;
	using ButtonBase<Nx * Ny>::mIconOff;

	int cx, cy;
	
	float rNx(){ return 1./Nx; }
	float rNy(){ return 1./Ny; }
	int index(int x, int y) const { return x + y*Nx; }
	float cellW(){ return w * rNx(); }
	float cellH(){ return h * rNy(); }
	float iconW(){ return cellW() - 2*iconInset() + 0.5; }
	float iconH(){ return cellH() - 2*iconInset() + 0.5; }
};



/// Abstract multidimensional slider

/// The foreground color determines the color of the slider.
///
template <int Dim>
class SliderBase : public View{
public:

	/// Constructor
	SliderBase(const Rect& r);
	
	float value(int dim=0) const;			///< Returns a slider value
	
	void value(float val, int dim);			///< Sets a slider value
	void valueAdd(float val, int dim);		///< Add to a slider value
	void valueMax();						///< Max all slider values
	void valueMid();						///< Center all slider values
	
	static int dimensions(){ return Dim; }	///< Returns number of dimensions of slider

protected:
	float mAcc[Dim], mVal[Dim];
	//float mSens;
	
	void clip(float & v){ v<0.f ? v=0.f : v>1.f ? v=1.f : 0; }
	void clipAccs(){ for(int i=0; i<Dim; ++i) clip(mAcc[i]); }
	bool validDim(int dim) const { return (dim < Dim) && (dim >= 0); }
	float sens(const GLV& g){ return (g.mouse.left() && g.mouse.right()) ? 0.25 : 1; }
};



/// A 1-D slider
class Slider : public SliderBase<1>{
public:

	/// Constructor
	Slider(const Rect& r=Rect(200,20), float val=0);

	virtual void onDraw();
	virtual bool onEvent(Event::t e, GLV & glv);
};



/// A 2-D slider
class Slider2D : public SliderBase<2>{
public:

	/// Constructor
	Slider2D(const Rect& r, float valX=0, float valY=0, space_t knobSize=12);

	space_t knobSize;	///< Size of slider knob
	
	virtual void onDraw();
	virtual bool onEvent(Event::t e, GLV & glv);
	
	static void drawKnob(const Slider2D& s);
	
};



/// A multi-parameter slider grid

/// The cells of the grid represent all the pair permutations of parameters.
/// The parameter values run from 0 to N, going left-to-right along the x-axis
/// and bottom-to-top along th y-axis. Cells along the diagonal control the
/// parameters individually.
template <int Dim>
class SliderGrid : public SliderBase<Dim>{
public:
	using SliderBase<Dim>::colors;
	using SliderBase<Dim>::w;
	using SliderBase<Dim>::h;
	using SliderBase<Dim>::value;
	using SliderBase<Dim>::clipAccs;
	using SliderBase<Dim>::sens;

	/// Constructor
	SliderGrid(const Rect& r, space_t knobSize=4);

	space_t knobSize; ///< Knob size

	virtual void onDraw();
	virtual bool onEvent(Event::t e, GLV& glv);

protected:
	int cx, cy;
};



// Implementation ______________________________________________________________

template <int Nx, int Ny>
Buttons<Nx, Ny>::Buttons(const Rect& r, bool mutExc, iconFunc on, iconFunc off)
:	ButtonBase<Nx*Ny>(r, !mutExc, on, off), cx(0), cy(0)
{}


template <int Nx, int Ny>
int Buttons<Nx, Ny>::lastClicked() const{ return index(cx, cy); }


template <int Nx, int Ny>
void Buttons<Nx, Ny>::onDraw(){
	using namespace glv::draw;

	float wi = pix(w);
	float hi = pix(h);
	float wc = cellW();
	float hc = cellH();

	// draw the grid lines
	color(colors().border);
	lineWidth(1);
	begin(Lines);
		for(int x=1; x<Nx; ++x){
			vertex(pix(x * wc), 0); vertex(pix(x * wc), hi);
		}
		for(int y=1; y<Ny; ++y){
			vertex(0, pix(y * hc), 0); vertex(wi, pix(y * hc));
		}
	end();
	
	for(int x=0; x<Nx; ++x){
	
		float dx = x * wc + iconInset();
	
		for(int y=0; y<Ny; ++y){
		
			float dy = y * hc + iconInset();
		
			if(value(index(x,y))){
				color(colors().fore);
				if(mIconOn) mIconOn(pix(dx), pix(dy), pix(dx+iconW()), pix(dy+iconH()+0.5));
			}
			else{
				color(colors().fore);
				if(mIconOff) mIconOff(pix(dx), pix(dy), pix(dx+iconW()), pix(dy+iconH()+0.5));
			}
		}
	}
}

template <int Nx, int Ny>
bool Buttons<Nx, Ny>::onEvent(Event::t e, GLV& g){
	switch(e){		
	case Event::MouseDown:
		if(g.mouse.left()){
			cx = (int)((g.mouse.xRel() / w) * Nx);
			cy = (int)((g.mouse.yRel() / h) * Ny);
			cx < 0 ? cx=0 : cx>=Nx ? cx=Nx-1 : 0;
			cy < 0 ? cy=0 : cy>=Ny ? cy=Ny-1 : 0;
			
			if(!toggles()){ valueMin(); value(true, index(cx, cy)); }
			else value(!value(index(cx, cy)), index(cx, cy));
			return false;
		}
	default: break;
	}
	return true;
}



// SliderBase

template <int Dim>
SliderBase<Dim>::SliderBase(const Rect& r)
:	View(r)
{
	memset(mAcc, 0, sizeof(float) * Dim);
	memset(mVal, 0, sizeof(float) * Dim);
}

template <int Dim>
inline void SliderBase<Dim>::valueAdd(float add, int dim){
	if(!validDim(dim)) return;
	float acc = mAcc[dim] + add;
	mAcc[dim] = mVal[dim] = acc;
	clip(mVal[dim]);	// clip in [0, 1]
}

template <int Dim>
inline float SliderBase<Dim>::value(int dim) const{
	return validDim(dim) ? mVal[dim] : 0;
}

template <int Dim>
inline void SliderBase<Dim>::valueMid(){
	for(int i=0; i<Dim; ++i) mVal[i] = mAcc[i] = 0.5;
}

template <int Dim>
inline void SliderBase<Dim>::valueMax(){
	for(int i=0; i<Dim; ++i) mVal[i] = mAcc[i] = 1;
}

template <int Dim>
inline void SliderBase<Dim>::value(float value, int dim){
	if(validDim(dim)) mVal[dim] = mAcc[dim] = value;
}



template <int Dim>
SliderGrid<Dim>::SliderGrid(const Rect& r, space_t knobSize)
:	SliderBase<Dim>(r), knobSize(knobSize), cx(0), cy(0)
{
	//this->cropSelf = false;
	this->disable(CropSelf);
}

template <int Dim>
void SliderGrid<Dim>::onDraw(){
	using namespace glv::draw;

	float rDim = 1./Dim;
	color(colors().fore);
	lineWidth(1);
	frame(cx * w * rDim, (Dim-cy-1) * h * rDim, (cx+1) * w * rDim, (Dim-cy) * h * rDim);
	enable(LineStipple);
	lineStipple(1, 0xAAAA);
	grid(0,0,w,h,Dim,Dim, false);
	shape(Lines,0,h,w,0);
	disable(LineStipple);

	pointSize(knobSize);
	begin(Points);
	for(int i=0; i<Dim; ++i){
		float f = (i+value(i)) * rDim;
		float x = f*w;
		//float y = f*h;
		
		for(int j=0; j<Dim; ++j){
			vertex(x, (1.-(j+value(j)) * rDim) * h);
		}
	}
	end();
}

template <int Dim>
bool SliderGrid<Dim>::onEvent(Event::t e, GLV& g){

	switch(e){
	case Event::MouseDrag:
					valueAdd( g.mouse.dx()/w * sens(g) * Dim, cx);
		if(cx!=cy)	valueAdd(-g.mouse.dy()/h * sens(g) * Dim, cy);
		break;
		
	case Event::MouseDown:
		cx = (int)((g.mouse.xRel() / w) * Dim);
		cy = (int)((g.mouse.yRel() / h) * Dim);
		cy = (Dim-1) - cy;
		cx < 0 ? cx=0 : cx>=Dim ? cx=Dim-1 : 0;
		cy < 0 ? cy=0 : cy>=Dim ? cy=Dim-1 : 0;

		if(g.mouse.left() && !g.mouse.right()){
			float cw = w/Dim;
			float ch = h/Dim;
						value(      (g.mouse.xRel()/cw - cx), cx);
			if(cx!=cy)	value(1.f - (g.mouse.yRel()/ch - ((Dim-1)-cy)), cy);
		}
		break;
		
	case Event::MouseUp: clipAccs(); break;
	default: break;
	}
	return false;
}



} // glv::

#endif

