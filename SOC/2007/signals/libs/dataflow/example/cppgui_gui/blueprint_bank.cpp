// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "BlueprintBank.hpp"
#include "BlueprintWindow.hpp"
#include <fltk/Button.h>

namespace boost { namespace dataflow { namespace fltk_gui {

void BlueprintBank::button_callback(fltk::Widget* widget, void*b)
{
    BlueprintBank *bank= static_cast<BlueprintBank *>(b);
    bank->m_blueprint->add_component(bank->m_bank[widget->label()], widget->label());
}
    
void BlueprintBank::rearrange()
{
    clear();
    int y = 0;
    for(blueprint::component_bank::iterator it= m_bank.begin(); it!=m_bank.end(); it++)
    {
        fltk::Button *button = new fltk::Button(0, y, 100, 100, it->c_str());
        y += 100;
        button->callback((fltk::Callback *)&button_callback, this);
        add(button);
    }
}

} } }
