//[ cppgui_example

// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <gui/gui.hpp>
#include <gui/frame_window.hpp>

#include "blueprint_bank.hpp"
#include "blueprint_window.hpp"

#include <boost/dataflow/signals/component/function.hpp>
#include <boost/dataflow/signals/component/storage.hpp>
#include <boost/dataflow/signals/runtime_support.hpp>
#include <boost/dataflow/blueprint/component_bank.hpp>

namespace blueprint=boost::dataflow::blueprint;
namespace signals=boost::signals;
namespace df=boost::dataflow;

// A regular Dataflow.Signals component, sends and receives void(int)
class output_valuator : public signals::filter<output_valuator, void(int), boost::mpl::vector<void(int)> >
{
public:
    output_valuator()
    {   valuator_(); }
    void operator()(int x)
    {
//        m_output->value(x);
        out(x);
    }
public:
    void valuator_()
    {
        // This component has a GUI element
//        m_output = new fltk::ValueOutput(20, 0, 60, 40);
    }
//    std::auto_ptr<fltk::Window> m_window;
//    fltk::ValueOutput *m_output;
};

// A regular Dataflow.Signals component, receives void(int)
class input_valuator : public boost::signals::filter<input_valuator, void(int)>
{
public:
    input_valuator()
    {
    }
public:
/*    static void callback(fltk::Widget *widget, input_valuator*iv)
    {
//        iv->out(iv->m_input->value());
    }*/
//    std::auto_ptr<fltk::Window> m_window;
//    fltk::ValueInput *m_input;
};

// A component_bank with some components
class example_bank : public blueprint::tag_component_bank<df::signals::tag>
{
public:
    example_bank()
    {
        // A storage initialized with a value of 100
        add_component<signals::storage<void(int)> >("storage", 100);
        // Output and input components
        add_component<output_valuator>("output");
        add_component<input_valuator>("input");
        // A filter that doubles
        add_component<signals::function<void (int), int(int)> >
            ("doubler", boost::function<int(int)>(boost::bind(std::multiplies<int>(), _1, 2)));
    }
};

int main()
{

    using namespace gui;

    // create the blueprint and component windows
    wnd<df::cppgui::blueprint_bank> bank = create<df::cppgui::blueprint_bank>(_title = "Component bank");
    wnd<df::cppgui::blueprint_window> window = create<df::cppgui::blueprint_window>(_title = "Blueprint");

    // initialize the bank
    bank->set_bank(example_bank());
    bank->set_blueprint(*window);

    gui::wait();
}

//]

