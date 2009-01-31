/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko, Andrey Torba
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include "two_buttons.hpp"
#include <boost/guigl/view/impl/static_compound.hpp>
#include <boost/guigl/view/impl/positioned.hpp>
#include <boost/guigl/types.hpp>
#include <boost/guigl/draw.hpp>
#include <boost/guigl/color.hpp>

void two_buttons::draw()
{
    draw_prologue();

    using namespace boost::guigl;
    using namespace boost::guigl::draw;

    color(black(0.3f));

    glLineWidth(2.5);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    {
      scoped_matrix m;

      rotate_z(20);
      translate_x(20);
      translate_y(20);
      //scale_y(4);

      polygon
        (0, 0)
        (0, size().y)
        (size().x, size().y)
        (size().x, 0);

      //rect(10);
    }

    color(yellow(0.5f));
    polygon
      (0, 0)
      (size().x, size().y)
      (size().x, 0);

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
