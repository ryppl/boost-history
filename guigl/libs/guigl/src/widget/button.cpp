/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include <boost/guigl/window.hpp>
#include <boost/guigl/widget/button.hpp>
#include <boost/guigl/view/impl/active_colored.hpp>
#include <boost/guigl/view/impl/clickable.hpp>
#include <boost/guigl/view/impl/positioned.hpp>
#include <boost/guigl/view/impl/solid_background.hpp>

namespace boost { namespace guigl { namespace widget {

BOOST_GUIGL_WIDGET_DRAW_IMPL(button)

void button::draw_prologue()
{
    base_type::draw_prologue();
    if(button_down())
    {
        use_active_color();
        glRectd(0,0,size().x, size().y);
    }
}

void button::draw_epilogue()
{
    base_type::draw_epilogue();
}

void button::clickable_button_down(bool state)
{
    guigl::window::redraw(*this);
}

bool button::on_event(const event_type &event_info)
{
    return base_type::on_event(event_info);
}

}}}