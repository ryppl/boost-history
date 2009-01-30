/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__VIEW__IMPL__POSITIONED_HPP
#define BOOST__GUIGL__VIEW__IMPL__POSITIONED_HPP

#include <boost/guigl/view/positioned.hpp>
#include <boost/guigl/platform/opengl.hpp>


namespace boost { namespace guigl { namespace view {

//BOOST_GUIGL_DRAW_IMPL(positioned)

template<typename BaseView>
inline void positioned<BaseView>::draw_prologue()
{
    BaseView::draw_prologue();
    glPushMatrix();
    glTranslated(m_position.x, m_position.y, 0);
}
    
template<typename BaseView>
inline void positioned<BaseView>::draw_epilogue()
{
    glPopMatrix();
    BaseView::draw_epilogue();
}

}}}

#endif // BOOST__GUIGL__VIEW__POSITIONED_HPP
