/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include <boost/fusion/include/mpl.hpp>
#include <boost/dataflow/blueprint/framework.hpp>
#include <boost/dataflow/vivid/factory_window.hpp>
#include <boost/dataflow/vivid/network_window.hpp>
#include <boost/guigl/application.hpp>
#include <boost/graph/connected_components.hpp>
#include "../generic/graph_framework.hpp"


namespace df = boost::dataflow;
typedef df::blueprint::framework<df::ugraph::framework> graph_blueprint_framework;

void connected_components(const df::blueprint::framework_context<graph_blueprint_framework> &bfo)
{
    std::vector<int> component(num_vertices(bfo.object()));
    int num = connected_components(bfo.object(), &component[0]);

    std::cout << num << std::endl;
}

int main()
{
    using namespace boost::guigl::keywords;
    boost::guigl::window controls((_label="Controls", _size=boost::guigl::size_type(300,50) ));
    boost::guigl::widget::labeled_button *connected_components_button;
    controls << (connected_components_button = new boost::guigl::widget::labeled_button((_label="Components", _size(300,50), _background(0,0,1) )) );

    df::vivid::factory_window<graph_blueprint_framework> factory_window;
    df::vivid::network_window<graph_blueprint_framework> network_window;
    factory_window.set_network_window(network_window);
    
    df::blueprint::factory<graph_blueprint_framework> factory;
    
    factory.add_port<df::ugraph::vertex>("vertex");
    network_window.framework_context().register_operation<df::ugraph::vertex, df::ugraph::vertex, df::operations::connect>();

    factory_window.set_factory(factory);
    
    
    connected_components_button->on_click.connect(boost::bind(&connected_components, boost::ref(network_window.framework_context())));
    boost::guigl::window::redraw(*connected_components_button);
    
    boost::guigl::application::run();
    
    
    return 0;
}
