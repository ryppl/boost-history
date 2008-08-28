/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__VIEW__IMPL__WINDOW_HPP
#define BOOST__GUIGL__VIEW__IMPL__WINDOW_HPP


#include <boost/guigl/view/window.hpp>
#include <boost/guigl/platform/opengl.hpp>


namespace boost { namespace guigl { namespace view {

//BOOST_GUIGL_DRAW_IMPL(window)

template<typename BaseView>
inline void window<BaseView>::draw_prologue()
{
    BaseView::draw_prologue();

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glOrtho (0, window::size().x, window::size().y, 0, 0, 1);
    glMatrixMode (GL_MODELVIEW);
    glDisable(GL_DEPTH_TEST);
    glClearColor(m_background_color[0], m_background_color[1], m_background_color[2], 0);
    glClear(GL_COLOR_BUFFER_BIT);
}

template<typename BaseView>
inline void window<BaseView>::draw_epilogue()
{
    BaseView::draw_epilogue();
}

}}}

#endif // BOOST__GUIGL__VIEW__IMPL__WINDOW_HPP