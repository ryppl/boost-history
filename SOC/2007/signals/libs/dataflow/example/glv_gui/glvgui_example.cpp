//[ cppgui_example

// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <glv.h>
#include <glv_binding_glut.h>

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
class output_valuator : public signals::filter<output_valuator, void(float), boost::mpl::vector<void(float)> >, public glv::Slider
{
public:
    typedef glv::View runtime_base_class_type;
    output_valuator()
        : glv::Slider(glv::Rect(100,100),0)
    {
        anchor(glv::Place::CL);
    }
    void operator()(float x)
    {
        value(x, 0);
        out(x);
    }
};

// A regular Dataflow.Signals component, receives void(int)
class input_valuator : public boost::signals::filter<input_valuator, void(float)>, public glv::Slider
{
public:
    typedef glv::View runtime_base_class_type;
    input_valuator()
        : glv::Slider(glv::Rect(100,100),0)
    {
        anchor(glv::Place::CL);
    }
public:
    bool onEvent(glv::Event::t e, glv::GLV& glv)
    {
        bool response = glv::Slider::onEvent(e, glv);
        out(value(0));
        return response;
    }
};

// A component_bank with some components
class example_bank : public blueprint::tag_component_bank<df::signals::tag>
{
public:
    example_bank()
    {
        // A storage initialized with a value of 0.5
        add_component<signals::storage<void(float)> >("sto", 0.5);
        // Output and input components
        add_component<output_valuator>("out");
        add_component<input_valuator>("in");
#ifndef BOOST_MSVC
        add_component<signals::function<void (int), int(int)> >
            ("x2", boost::function<int(int)>(boost::bind(std::multiplies<int>(), _1, 2)));
#endif
    }
};

int main()
{
    glv::GLV top;
	glv::Window win(640, 640, "GLV Blueprint GUI", &top);
    glv::Placer placer(top, glv::Direction::E, glv::Place::TL, 0, 0, 0);
    
    using namespace boost::dataflow::glv_gui;

    // create the blueprint and component windows
    blueprint_bank bank;
    blueprint_window window;

    // initialize the bank
    bank.set_bank(example_bank());
    bank.set_blueprint(window);
    
    placer << bank.view() << window;
    glv::Application::run();
}

//]

