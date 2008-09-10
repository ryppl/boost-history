/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__VIEW__IMPL__NAVIGABLE_HPP
#define BOOST__GUIGL__VIEW__IMPL__NAVIGABLE_HPP

#include <boost/guigl/view/impl/draggable.hpp>
#include <boost/guigl/window.hpp>

namespace boost { namespace guigl { namespace view {

template<typename BaseView>
void navigable<BaseView>::draw_prologue()
{
    base_type::draw_prologue();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glRotated(m_angle, 0, 1, 0);
	glTranslatef(0, 0, -500);
}

template<typename BaseView>
void navigable<BaseView>::draw_epilogue()
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    base_type::draw_epilogue();
}

template<typename BaseView>
void navigable<BaseView>::draggable_on_drag(const position_type &position)
{
    m_angle += position.x - base_type::drag_origin().x;
    base_type::set_drag_origin(position);
    guigl::window::redraw(*this);
}

}}}

#endif // BOOST__GUIGL__VIEW__IMPL__NAVIGABLE_HPP