/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include "two_buttons.hpp"
#include <boost/guigl/view/impl/static_compound.hpp>
#include <boost/guigl/view/impl/positioned.hpp>
#include <boost/guigl/types.hpp>
#include <boost/guigl/draw.hpp>


void two_buttons::draw()
{
    draw_prologue();

    using namespace boost::guigl;
    using namespace boost::guigl::draw;

    color(black((float)0.3));

    glLineWidth(2.5);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
    line_loop
      (0, 0)
      (size().x, 0)
      (size().x, size().y)
      (0, size().y);

    draw_epilogue();
}

bool two_buttons::on_event(const boost::guigl::event_type &event_info)
{
    return base_type::on_event(event_info);
}
