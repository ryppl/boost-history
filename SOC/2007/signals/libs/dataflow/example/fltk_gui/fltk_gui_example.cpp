// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include "BlueprintWindow.hpp"
#include "BlueprintBank.hpp"
#include <fltk/run.h>
#include <fltk/Button.h>
#include <fltk/ValueOutput.h>

#include <boost/dataflow/signals/component/storage.hpp>
#include <boost/dataflow/signals/runtime_support.hpp>

#include <memory>

namespace blueprint=boost::dataflow::blueprint;
namespace signals=boost::signals;
namespace df=boost::dataflow;

class valuator : public boost::signals::filter<valuator, void(int), boost::mpl::vector<void(int)> >
{
public:
    valuator(const valuator &)
    {
        m_window.reset(new fltk::Window(200, 200, "Valuator output"));
        m_window->add(output = new fltk::ValueOutput(50, 50, 100, 100));
        m_window->show();
    }
    valuator()
    {
        m_window.reset(new fltk::Window(200, 200, "Valuator output"));
        m_window->add(output = new fltk::ValueOutput(50, 50, 100, 100));
        m_window->show();
    }
    void operator()(int x)
    {
        output->value(x);
    }
public:
    std::auto_ptr<fltk::Window> m_window;
    fltk::ValueOutput *output;
};

class example_bank : public blueprint::tag_component_bank<df::signals::tag>
{
public:
    example_bank()
    {
        add_component<signals::storage<void(int)> >("storage_int", 100);
        add_component<valuator>("valuator");
    }
};

int main()
{
    //fltk::Button::default_style->box_ = fltk::RSHADOW_BOX;
    //fltk::Button::default_style->buttonbox_ = fltk::RSHADOW_BOX;
    fltk::Button::default_style->buttoncolor_ = fltk::color(0, 0, 255);
    fltk::Window::default_style->color_ = fltk::GRAY75;
    using namespace boost::dataflow::fltk_gui;

    std::auto_ptr<df::fltk_gui::BlueprintWindow> window(new df::fltk_gui::BlueprintWindow(800, 600, "Blueprint"));
    std::auto_ptr<BlueprintBank> bank(new BlueprintBank(100, 600, "Components"));

    bank->set_bank(example_bank());
    bank->set_blueprint(*window);
    
    bank->show();    
    window->show();
    
    fltk::run();
}

