/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__VIEW__IMPL__LABELED_HPP
#define BOOST__GUIGL__VIEW__IMPL__LABELED_HPP

#include <boost/guigl/view/labeled.hpp>
#include <boost/guigl/platform/opengl.hpp>
#include <boost/guigl/platform/glut.hpp>


namespace boost { namespace guigl { namespace view {

template<typename BaseView>
inline void labeled<BaseView>::draw_prologue()
{
    BaseView::draw_prologue();

    glRasterPos3d((BaseView::size().x - 9*m_label.size())/2, BaseView::size().y-(BaseView::size().y-15)/2, 0);
    for (std::string::const_iterator it=m_label.begin(); it!=m_label.end(); it++)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *it);
}

}}}

#endif // BOOST__GUIGL__VIEW__IMPL__LABELED_HPP