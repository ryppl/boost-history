/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include <boost/guigl/widget/labeled_button.hpp>
#include <boost/guigl/view/impl/colored.hpp>
#include <boost/guigl/view/impl/labeled.hpp>

namespace boost { namespace guigl { namespace widget {

BOOST_GUIGL_WIDGET_DRAW_IMPL(labeled_button)

bool labeled_button::on_event(const event_type &event_info)
{
    return base_type::on_event(event_info);
}

}}}