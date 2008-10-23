/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__VIEW__IMPL__THREE_DIMENSIONAL_HPP
#define BOOST__GUIGL__VIEW__IMPL__THREE_DIMENSIONAL_HPP

#include <boost/guigl/view/three_dimensional.hpp>
#include <boost/guigl/platform/opengl.hpp>


namespace boost { namespace guigl { namespace view {

template<typename BaseView>
inline void three_dimensional<BaseView>::draw_prologue()
{
    BaseView::draw_prologue();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	double xy_aspect = BaseView::size().x / BaseView::size().y;

	glLoadIdentity();
	glFrustum(xy_aspect*-0.5, xy_aspect*0.5, -0.5, 0.5, 0.5, 5000);
}
    
template<typename BaseView>
inline void three_dimensional<BaseView>::draw_epilogue()
{
    glPopMatrix();
    BaseView::draw_epilogue();
}

}}}

#endif // BOOST__GUIGL__VIEW__IMPL__THREE_DIMENSIONAL_HPP
