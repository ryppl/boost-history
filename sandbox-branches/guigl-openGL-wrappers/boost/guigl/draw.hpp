/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__GUIGL__DRAW_HPP
#define BOOST__GUIGL__DRAW_HPP


#include <boost/guigl/platform/opengl.hpp>
#include <boost/guigl/types.hpp>


namespace boost { namespace guigl {

namespace draw {

inline void vertex(const position_type &position)
{
    glVertex2d(position.x, position.y);
}

inline void color(const color_type &color)
{
    glColor3d(color[0], color[1], color[2]);
}

}

}}

#endif // BOOST__GUIGL__DRAW_HPP
