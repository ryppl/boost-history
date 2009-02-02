/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__VIEW__IMPL__COLORED_HPP
#define BOOST__GUIGL__VIEW__IMPL__COLORED_HPP

#include <boost/guigl/view/colored.hpp>
#include <boost/guigl/platform/opengl.hpp>


namespace boost { namespace guigl { namespace view {

template<typename BaseView>
inline void colored<BaseView>::draw_prologue()
{
    BaseView::draw_prologue();
    glColor3d(m_color[0], m_color[1], m_color[2]);
}

}}}

#endif // BOOST__GUIGL__VIEW__IMPL__COLORED_HPP
