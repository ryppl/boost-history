/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__VIEW__IMPL__SOLID_BACKGROUND_HPP
#define BOOST__GUIGL__VIEW__IMPL__SOLID_BACKGROUND_HPP

#include <boost/guigl/view/solid_background.hpp>
#include <boost/guigl/platform/opengl.hpp>


namespace boost { namespace guigl { namespace view {

//BOOST_GUIGL_DRAW_IMPL(solid_background)

template<typename BaseView>
inline void solid_background<BaseView>::draw_prologue()
{
    BaseView::draw_prologue();
    glColor3d(m_background_color[0], m_background_color[1], m_background_color[2]);
    glRectd(0.0, 0.0, solid_background::size().x, solid_background::size().y);
}

}}}

#endif // BOOST__GUIGL__VIEW__IMPL__SOLID_BACKGROUND_HPP