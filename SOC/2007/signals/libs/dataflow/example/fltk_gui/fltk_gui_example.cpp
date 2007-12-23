// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <fltk/Window.h>
#include <fltk/run.h>
#include <boost/dataflow/blueprint/network.hpp>

#include <memory>

class BlueprintWindow : public fltk::Window
{
    BlueprintWindow(int width, int height, const char *label = 0)
        : fltk::Window(width, height, label)
    {}
};

class BlueprintComponent : public fltk::Group
{
    
};

int main()
{
    std::auto_ptr<fltk::Window> window(new fltk::Window(100, 100));
    
    window->show();
    
    fltk::run();
}

