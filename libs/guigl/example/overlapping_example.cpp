/*=================================---------------------------------------------
    Copyright 2009 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#include "pch.hpp"

#include <boost/guigl/application.hpp>
#include <boost/guigl/window.hpp>

#include <boost/guigl/widget/labeled_button.hpp>

using namespace boost::guigl;

window example_window(( _label = "overlapping example", _size=size_type(300,300) ));

int main()
{    
    example_window << new widget::labeled_button(( _label = "Bottom", _position=position_type(0,0), _size=size_type(200,200) ));
    example_window << new widget::labeled_button(( _label = "Top", _position=position_type(100,100), _size=size_type(200,200) ));    
    
    application::run();

    return 0;
}
