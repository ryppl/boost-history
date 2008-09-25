/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include "two_buttons.hpp"
#include <boost/guigl/view/impl/static_compound.hpp>
#include <boost/guigl/view/impl/positioned.hpp>


void two_buttons::draw()
{
    draw_prologue();
    draw_epilogue();
}