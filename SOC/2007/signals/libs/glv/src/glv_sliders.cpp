/*	Graphics Library of Views (GLV) - GUI Building Toolkit
	See COPYRIGHT file for authors and license information */

#include "glv_sliders.h"
#include "glv_draw.h"

namespace glv{

// Button

Button::Button(const Rect& r, bool toggles, iconFunc on, iconFunc off)
:	ButtonBase<1>(r, toggles, on, off, 3)
{}
	
void Button::onDraw(){
	using namespace glv::draw;

	draw::lineWidth(2);

	float v1 = pix(iconInset());
	float vr = pix(w - iconInset() + 0.5);	// add 0.5 for rect, 0 o.w.; not sure why?
	float vb = pix(h - iconInset() + 0.5);

	if(value()){
		color(colors().fore);
		if(mIconOn) mIconOn(v1, v1, vr, vb);
	}
	else{
		color(colors().fore);
		if(mIconOff) mIconOff(v1, v1, vr, vb);
	}
}

bool Button::onEvent(Event::t e, GLV& g){
	switch(e){
		case Event::MouseDown:
			if(g.mouse.left()){
				value(toggles() ? !value() : true, 0);
				notify();
			}
			break;
			
		case Event::MouseUp:
			if(g.mouse.button() == Mouse::Left){
				if(!toggles()){ value(false, 0); notify(); }
			}
			break;
		default: break;
	}
	return false;
}



// Slider

Slider::Slider(const Rect& r, float val)
:	SliderBase<1>(r)
{
	value(val, 0);
}


void Slider::onDraw(){
	using namespace glv::draw;
	draw::color(colors().fore);
	if(isVertical())	rect(1, pix(h * (1.f - value())), pix(w), pix(h));
	else				rect(1, 1, pix(w * value()), pix(h));
}

bool Slider::onEvent(Event::t e, GLV& g){

	switch(e){
		case Event::MouseDrag:
			if(isVertical()) valueAdd(-g.mouse.dy()/h * sens(g), 0);
			else             valueAdd( g.mouse.dx()/w * sens(g), 0);
			notify();
			break;
			
		case Event::MouseDown:
			if(g.mouse.left() && !g.mouse.right()){
				if(isVertical()) value(1.f - g.mouse.yRel() / h, 0);
				else             value(      g.mouse.xRel() / w, 0);
			}
			notify();
			break;
			
		case Event::MouseUp: clipAccs(); break;
			
		case Event::KeyDown:
			switch(g.keyboard.key()){
			default: return true;
			}
		default: break;
			
	}
	return false;
}


// Slider2D

Slider2D::Slider2D(const Rect& r, float valX, float valY, space_t knobSize)
:	SliderBase<2>(r), knobSize(knobSize)
{
	value(valX, 0);
	value(valY, 1);
}

bool Slider2D::onEvent(Event::t e, GLV& g){

	switch(e){
		case Event::MouseDrag:
			valueAdd( g.mouse.dx()/w * sens(g), 0);
			valueAdd(-g.mouse.dy()/h * sens(g), 1); 		
			notify();
			break;
			
		case Event::MouseDown:
			if(g.mouse.left() && !g.mouse.right()){
				value(      g.mouse.xRel() / w, 0);
				value(1.f - g.mouse.yRel() / h, 1);
			}
			notify();
			break;
			
		case Event::MouseUp: clipAccs(); break;
			
		case Event::KeyDown:
			switch(g.keyboard.key()){
				case Key::Left:		valueAdd(-1. / w, 0); notify(); break;
				case Key::Right:	valueAdd( 1. / w, 0); notify(); break;
				case Key::Up:		valueAdd( 1. / h, 1); notify(); break;
				case Key::Down:		valueAdd(-1. / h, 1); notify(); break;
				default: return true;
			}
			break;
		default: break;
	}
	return false;
}


void Slider2D::drawKnob(const Slider2D& s){
	using namespace glv::draw;
	float sz = s.knobSize;	// size of indicator block
	float sz2 = sz * 0.5f;
	float posX = sz2 + (s.w - sz) * s.value(0);
	float posY = sz2 + (s.h - sz) * (1.f - s.value(1));
	
	color(s.colors().fore);
	rect(pix(posX - sz2), pix(posY - sz2), pix(posX + sz2), pix(posY + sz2));
}

/*
static void drawQuad(const Slider2D& s){
	using namespace glv::draw;
	float posX = s.w * s.value(0);
	float posY = s.h * (1.f - s.value(1));
	
	color(s.colors().fore);
	
	begin(Quads);
		vertex(0.f, posY);
		vertex(posX, s.h);
		vertex(s.w, posY);
		vertex(posX, 0.f);
	end();
	
	// clock hand
	//shape(Lines, posX, posY, s.w*0.5, s.h*0.5);
}
*/

void Slider2D::onDraw(){

	drawKnob(*this);
	//drawQuad(*this);
	
//	float hh = h * 0.5f;
//	float hw = w * 0.5f;

	// Web
//	glBegin(GL_LINES);
//		glVertex2f(posX, posY); glVertex2f(hw, h);
//		glVertex2f(posX, posY); glVertex2f(w, hh);
//		glVertex2f(posX, posY); glVertex2f(hw, 0.f);
//		glVertex2f(posX, posY); glVertex2f(0.f, hh);
//
//		glVertex2f(posX, posY); glVertex2f(0.f, h);
//		glVertex2f(posX, posY); glVertex2f(w, h);
//		glVertex2f(posX, posY); glVertex2f(w, 0.f);
//		glVertex2f(posX, posY); glVertex2f(0.f, 0.f);
//
////		glVertex2f(0.f, posY); glVertex2f(w, posY);
////		glVertex2f(posX, 0.f); glVertex2f(posX, h);
//	glEnd();
	
	//glColor4f(glvColor4(bgColor));
	//glRectf(posX, h, posX+1, 0.f);
	//glRectf(0.f, posY, w, posY+1);

//	if((posX > 6.f) && (posY < (h - 6.f))) return;

	// fade color in
	//s->sliderColor->glColor3(1.f - ((posX < h - posY) ? posX : h - posY) * 0.16667f);

	// double diagonal indicators
//	glBegin(GL_LINES);
//		glVertex2f(posX, posY); glVertex2f(w, hh);
//		glVertex2f(posX, posY); glVertex2f(hw, 0.f);
//	glEnd();
	
	// x
//	glBegin(GL_LINES);
//		float d = 6.f;
//		glVertex2f(posX - d, posY - d);
//		glVertex2f(posX + d, posY + d);
//		glVertex2f(posX, posY);
//		glVertex2f(posX + d, posY - d);
//	glEnd();
	
	// overline
//	glBegin(GL_LINES);
//		float d = 6.f;
//		glVertex2f(0.f, h - d);
//		glVertex2f(posX, h - d);
//		glVertex2f(d, h);
//		glVertex2f(d, posY);
//	glEnd();
}





} // end namespace glv
