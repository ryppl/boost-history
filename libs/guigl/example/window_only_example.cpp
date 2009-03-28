/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#include "pch.hpp"

//[ window_only_example_includes
#include <boost/guigl/application.hpp>
#include <boost/guigl/window.hpp>
//]

//[ window_only_example_using
    using namespace boost::guigl;
//]

//[ window_only_example_window
window test_window1(( _label = "window example 1", _size=size_type(300,300) ));
//]

//[ window_only_example_application
int main()
{
    application::run();

    return 0;
}
//]
