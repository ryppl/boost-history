//[ cppgui_example

// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <glv.h>
#include <glv_binding_glut.h>

#include "blueprint_bank.hpp"
#include "blueprint_window.hpp"

#include <boost/dataflow/managed/fusion_component.hpp>
#include <boost/dataflow/managed/network.hpp>
#include <boost/dataflow/managed/runtime_support.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/dataflow/blueprint/component_bank.hpp>

namespace df=boost::dataflow;
namespace blueprint=boost::dataflow::blueprint;

using df::managed::fusion_component;

// source just provides an output port
class source : public fusion_component<bool>
{
public:
    source(df::managed::network &network)
        : fusion_component<bool>(network)
    {
//        m_producer_port.set(value);
    }
};

// adder has two input ports and an output port.
// invoking adds the two inputs into the output.
class not_operation : public fusion_component<bool>
{
public:
    not_operation(df::managed::network &network)
        : fusion_component<bool>(network)
    {}
    void invoke()
    {
        m_producer_port.set(!m_consumer_port.get());
    }
};



// A component_bank with some components
class example_bank : public blueprint::tag_component_bank<df::managed::tag>
{
public:
    example_bank()
    {
        add_component<source>("src", m_network);
        add_component<not_operation>("not", m_network);
    }
private:
    df::managed::network m_network;
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

