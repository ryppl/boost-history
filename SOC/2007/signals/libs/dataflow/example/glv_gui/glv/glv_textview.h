#ifndef INC_GLV_TEXTVIEW_H
#define INC_GLV_TEXTVIEW_H

/*	Graphics Library of Views (GLV) - GUI Building Toolkit
	See COPYRIGHT file for authors and license information */

#include <string>
#include <string.h>
#include "glv_core.h"

namespace glv{

/// Basic label widget
class Label : public View{
public:

	Label(const std::string str, bool vert);

	/// @param[in] str		Label string
	/// @param[in] l		Left position
	/// @param[in] t		Top position
	/// @param[in] vert		Whether to draw label vertically
	Label(const std::string str, space_t l=0, space_t t=0, bool vert=false);
	
	Label& label(const std::string& s);
	Label& size(float pixels);
	Label& vertical(bool v);

	virtual void onDraw();

protected:
	std::string mLabel;		// The label string
	float mSize;
	bool mVertical;
	
	void fitExtent();
	void rotateRect();		// swap between vertical and horizontal
};

} // glv::

#endif
