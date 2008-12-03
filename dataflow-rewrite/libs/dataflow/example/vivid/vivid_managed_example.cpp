/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include <boost/dataflow/blueprint/framework.hpp>
#include <boost/dataflow/vivid/factory_window.hpp>
#include <boost/dataflow/vivid/network_window.hpp>
#include <boost/guigl/application.hpp>
#include <boost/guigl/widget/button.hpp>

#include <boost/dataflow/managed/io_component.hpp>
#include <boost/dataflow/managed/network.hpp>

#include <boost/assign/std/vector.hpp>

namespace df=boost::dataflow;
namespace blueprint=boost::dataflow::blueprint;
namespace guigl=boost::guigl;

using df::managed::io_component;

typedef df::blueprint::framework<df::managed::framework> managed_blueprint_framework;

// the input/output component (link between GUI and dataflow network)
class io : public io_component<bool>, public guigl::widget::button
{
public:
    typedef guigl::widget::button runtime_base_class_type;
    io(df::managed::network &network)
        : io_component<bool>(network)
        , guigl::widget::button(guigl::_size(100,100))
    {
//        anchor(glv::Place::CL);
    }
    void invoke()
    {
        if(port<0>().connected())
        {
            //value(port<0>().get());
            port<1>().set(port<0>().get());
        }
    }
/*    bool onEvent(glv::Event::t e, glv::GLV& glv)
    {
        bool response = glv::Button::onEvent(e, glv);
        port<1>().set(value());
        return response;
    }*/
};

// logical NOT component.
class not_operation : public io_component<bool>
{
public:
    not_operation(df::managed::network &network)
        : io_component<bool>(network)
    {}
    void invoke()
    {
        if(port<0>().connected())
            port<1>().set(!port<0>().get());
    }
};

// logical AND component.
class and_operation : public io_component<boost::mpl::vector<bool,bool>,bool>
{
public:
    typedef io_component<boost::mpl::vector<bool,bool>,bool> base_type;
    
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
class or_operation : public io_component<boost::mpl::vector<bool,bool>,bool>
{
public:
    typedef io_component<boost::mpl::vector<bool,bool>,bool> base_type;
    
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
class update_network : public io_component<bool>
{
public:
    update_network(df::managed::network &network)
        : io_component<bool>(network)
    {}
    void invoke()
    {
        network_context().update();
    }
};


// A component_bank with some components
class example_factory : public df::blueprint::factory<managed_blueprint_framework>
{
public:
    example_factory()
    {
/*        add_component<io>("io", boost::ref(m_network));
        add_component<not_operation>("not", boost::ref(m_network));
        add_component<and_operation>("and", boost::ref(m_network));
        add_component<or_operation>("or", boost::ref(m_network));
        add_component<update_network>("upd", boost::ref(m_network));*/
    }
private:
    df::managed::network m_network;
};

int main()
{
    using namespace boost::guigl::keywords;
    boost::guigl::window controls((_label="Controls", _size=boost::guigl::size_type(300,50) ));

    df::vivid::factory_window<managed_blueprint_framework> factory_window;
    df::vivid::network_window<managed_blueprint_framework> network_window;
    factory_window.set_network_window(network_window);
    
    example_factory factory;
    
    factory_window.set_factory(factory);
    
    boost::guigl::application::run();
}


