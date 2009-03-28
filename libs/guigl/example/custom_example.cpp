/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#include "pch.hpp"

#include <boost/guigl/application.hpp>
#include <boost/guigl/window.hpp>
#include <boost/guigl/widget/custom.hpp>

#include <boost/guigl/platform/opengl.hpp>

#include <boost/guigl/gl.hpp>

using namespace boost::guigl;

void draw_stuff()
{
    using namespace boost::guigl::gl;

    color(1.0, 0.0, 0.0);
    rect(0.0, 0.0, 50.0, 50.0);
}

void draw_stuff2()
{
    glColor3d(1.0, 0.0, 0.0);
    glRectd(0.0, 0.0, 150.0, 150.0);
}

int main()
{
    window test_window1(( _label = "custom example", _size=size_type(300,300) ));
    
    test_window1 << new widget::custom((
        _size=size_type(300,150),
        _draw_prologue=draw_stuff
    ));

    widget::custom *set = new widget::custom((
        _size=size_type(300,150), _position=position_type(0, 150)
    ));
    
    set->set_draw_prologue(&draw_stuff2);
    
    test_window1 << set;

    application::run();
}
