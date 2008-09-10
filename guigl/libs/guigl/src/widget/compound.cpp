/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#include <boost/guigl/widget/compound.hpp>
#include <boost/guigl/view/impl/compound.hpp>
#include <boost/guigl/view/impl/positioned.hpp>
#include <boost/guigl/view/impl/solid_background.hpp>

namespace boost { namespace guigl { namespace widget {

void compound::draw()
{
    draw_prologue();
    draw_epilogue();
}

}}}
