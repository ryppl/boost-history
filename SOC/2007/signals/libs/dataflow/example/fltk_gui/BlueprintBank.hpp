// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef BLUEPRINT_COMPONENT_BANK_HPP
#define BLUEPRINT_COMPONENT_BANK_HPP

#include <boost/dataflow/blueprint/component_bank.hpp>

#include <fltk/window.h>

namespace boost { namespace dataflow { namespace fltk_gui {

class BlueprintWindow;

class BlueprintBank : public fltk::Window
{
public:
    BlueprintBank(int w, int h, const char *label = 0)
        : fltk::Window(w, h, label)
    {}
    BlueprintBank(int x, int y, int w, int h, const char *label = 0, bool begin=false)
        : fltk::Window(x, y, w, h, label, begin)
    {}
    void set_bank(const blueprint::component_bank &bank)
    {
        m_bank = bank;
        rearrange();
    }
    void set_blueprint(BlueprintWindow &blueprint)
    {
        m_blueprint = &blueprint;
    }
    static void button_callback(fltk::Widget* widget, void*b);
private:
    void rearrange();
    BlueprintWindow * m_blueprint;
    blueprint::component_bank m_bank;
};

} } }

#endif
