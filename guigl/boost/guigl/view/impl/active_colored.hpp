/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__VIEW__IMPL__ACTIVE_COLORED_HPP
#define BOOST__GUIGL__VIEW__IMPL__ACTIVE_COLORED_HPP

#include <boost/guigl/view/active_colored.hpp>
#include <boost/guigl/platform/opengl.hpp>

namespace boost { namespace guigl { namespace view {

template<typename BaseView>
void active_colored<BaseView>::use_active_color()
{
    glColor3d(m_active_color[0],m_active_color[1],m_active_color[2]);
}

}}}

#endif // BOOST__GUIGL__VIEW__IMPL__COLORED_HPP
