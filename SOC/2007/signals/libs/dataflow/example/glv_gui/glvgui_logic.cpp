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
class source : public fusion_component<bool>, public glv::Button
{
public:
    typedef glv::View runtime_base_class_type;
    source(df::managed::network &network)
        : fusion_component<bool>(network)
        , glv::Button(glv::Rect(100,100),true)
    {
        anchor(glv::Place::CL);
//        m_producer_port.set(value);
    }
    void invoke()
    {
        if(m_consumer_port.connected())
        {
            value(m_consumer_port.get(), 0);
            m_producer_port.set(m_consumer_port.get());
        }
    }
    bool onEvent(glv::Event::t e, glv::GLV& glv)
    {
        bool response = glv::Button::onEvent(e, glv);
        std::cout <<"preval: " << value(0) << std::endl;
        m_producer_port.set(value(0));
        return response;
    }
};

// adder has two input ports and an output port.
// invoking adds the two inputs into the output.
class not_operation : public fusion_component<bool>, public glv::Button
{
public:
    typedef glv::View runtime_base_class_type;
    not_operation(df::managed::network &network)
        : fusion_component<bool>(network)
        , glv::Button(glv::Rect(100,100),true)
    {
        anchor(glv::Place::CL);
    }
    void invoke()
    {
        if(m_consumer_port.connected())
        {
            m_producer_port.set(!m_consumer_port.get());
            value(m_producer_port.get(), 0);
        }
    }
};

// adder has two input ports and an output port.
// invoking adds the two inputs into the output.
class update_network : public fusion_component<bool>
{
public:
    update_network(df::managed::network &network)
        : fusion_component<bool>(network)
    {}
    void invoke()
    {
        network_context().update();
    }
};


// A component_bank with some components
class example_bank : public blueprint::tag_component_bank<df::managed::tag>
{
public:
    example_bank()
    {
        add_component<source>("src", boost::ref(m_network));
        add_component<not_operation>("not", boost::ref(m_network));
        add_component<update_network>("upd", boost::ref(m_network));
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

