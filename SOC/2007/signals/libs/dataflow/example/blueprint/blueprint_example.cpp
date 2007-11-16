//[ blueprint_example

// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/blueprint/network.hpp>
#include <boost/dataflow/signals/component/storage.hpp>
#include <boost/dataflow/signals/runtime_support.hpp>

#include <boost/graph/copy.hpp>
#include <boost/graph/connected_components.hpp>

#include <iostream>

namespace blueprint=boost::dataflow::blueprint;
namespace df=boost::dataflow;

using namespace boost;

int main()
{
    typedef blueprint::network<df::signals::mechanism> network_type;
    
    // Start with the network.
    network_type network;

    // Add the components.
    network_type::component_type
        source = network.add_component<signals::storage<void(int)> >(100),
        sink = network.add_component<signals::storage<void(int)> >(0),
        source_float = network.add_component<signals::storage<void(float)> >(100.1),
        sink_float = network.add_component<signals::storage<void(float)> >(0);
    
    // Print some runtime info
    std::cout << "source has " << network[source].num_ports() << " ports. " << std::endl;
    for (int i=0; i<network[source].num_ports(); i++)
        std::cout << "  port " << i << " is a "
            << (network.get_port(source, i)->category()) << std::endl;
    std::cout << std::endl;

    std::cout
        << "source port 0 is "
        << (network.are_connectable(source, 0, sink, 1) ? "" : "not ")
        << "connectable to sink port 1" << std::endl;

    std::cout
        << "source port 1 is "
        << (network.are_connectable(source, 1, sink, 0) ? "" : "not ")
        << "connectable to sink port 0" << std::endl;
    
    std::cout
        << "source port 0 is "
        << (network.are_connectable(source, 0, sink, 0) ? "" : "not ")
        << "connectable to sink port 0" << std::endl;

    std::cout
        << "source port 0 is "
        << (network.are_connectable(source, 0, sink_float, 1) ? "" : "not ")
        << "connectable to sink_float port 1" << std::endl;

    // Make some connections
    std::cout << std::endl << "Blueprint connecting source to sink, source_float to sink_float..." << std::endl;
    network.add_connection(source, 0, sink, 1);
    network.add_connection(source_float, 0, sink_float, 1);
    
    // Do some analysis on the connectivity graph
    typedef boost::adjacency_list<
        boost::vecS, boost::vecS, boost::undirectedS> ugraph_type;
        
    ugraph_type g;
    copy_graph(network.graph(), g);
    std::vector<int> component(num_vertices(g));
    std::cout << std::endl << "Using BGL to analyze the network... There are "
        << connected_components(g, &component[0])
        << " independent parts of the network." << std::endl << std::endl;
    
    // Connect the underlying components
    std::cout << "Making the connections..." << std::endl;    
    network.connect();
    
    // Invoke the sources
    std::cout << "Invoking the sources..." << std::endl;
    network[source].invoke();
    network[source_float].invoke();

    // Output the values at the sinks:
    std::cout << "sink now has:"
        << ((blueprint::component_t<df::signals::mechanism, signals::storage<void(int)> > &)
            network[sink]).get().at<0>() << std::endl;
    std::cout << "sink_float now has:"
        << ((blueprint::component_t<df::signals::mechanism, signals::storage<void(float)> > &)
        network[sink_float]).get().at<0>() << std::endl;

}

//]