/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include <boost/guigl/application.hpp>
#include <boost/guigl/platform/glut.hpp>


namespace boost { namespace guigl {

namespace {
    void idle_func()
    {
        application::on_idle()();
    }
}

void application::run()
{
    glutIdleFunc(idle_func);
    glutMainLoop();
}

boost::signal<void()> &application::on_idle()
{
    static boost::signal<void()> signal;
    return signal;
}

}}