/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include <boost/guigl/widget/custom.hpp>
#include <boost/guigl/view/impl/positioned.hpp>

namespace boost { namespace guigl { namespace widget {

void custom::draw()
{
    draw_prologue();
    draw_epilogue();
}

bool custom::on_event(const event_type &event_info)
{
    return base_type::on_event(event_info);
}

}}}
