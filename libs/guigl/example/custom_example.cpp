/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


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

int main()
{
    window test_window1(( _label = "custom example", _size=size_type(300,300) ));
    
    test_window1 << new widget::custom((
        _size=size_type(300,300),
        _draw_prologue=draw_stuff
    ));

    application::run();
}
