//[ cppgui_example

// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <glv.h>
#include <glv_pimpl_binding.h>

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

// the input/output component (link between GUI and dataflow network)
class io : public fusion_component<bool>, public glv::Button
{
public:
    typedef glv::View runtime_base_class_type;
    io(df::managed::network &network)
        : fusion_component<bool>(network)
        , glv::Button(glv::Rect(100,100),true)
    {
        anchor(glv::Place::CL);
    }
    void invoke()
    {
        if(port<0>().connected())
        {
            value(port<0>().get());
            port<1>().set(port<0>().get());
        }
    }
    bool onEvent(glv::Event::t e, glv::GLV& glv)
    {
        bool response = glv::Button::onEvent(e, glv);
        port<1>().set(value());
        return response;
    }
};

// logical NOT component.
class not_operation : public fusion_component<bool>
{
public:
    not_operation(df::managed::network &network)
        : fusion_component<bool>(network)
    {}
    void invoke()
    {
        if(port<0>().connected())
            port<1>().set(!port<0>().get());
    }
};

// logical AND component.
class and_operation : public fusion_component<boost::mpl::vector<bool,bool>,bool>
{
public:
    typedef fusion_component<boost::mpl::vector<bool,bool>,bool> base_type;
    
    and_operation(df::managed::network &network)
        : base_type(network)
    {}
    void invoke()
    {
        if(port<0>().connected())
        {
            port<2>().set(port<0>().get() && port<1>().get());
        }
    }
};

// logical OR component.
class or_operation : public fusion_component<boost::mpl::vector<bool,bool>,bool>
{
public:
    typedef fusion_component<boost::mpl::vector<bool,bool>,bool> base_type;
    
    or_operation(df::managed::network &network)
        : base_type(network)
    {}
    void invoke()
    {
        if(port<0>().connected())
        {
            port<2>().set(port<0>().get() || port<1>().get());
        }
    }
};

// This component requests a network update.
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
        add_component<io>("io", boost::ref(m_network));
        add_component<not_operation>("not", boost::ref(m_network));
        add_component<and_operation>("and", boost::ref(m_network));
        add_component<or_operation>("or", boost::ref(m_network));
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

