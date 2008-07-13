//[ blueprint_example

// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>

#include <boost/dataflow/blueprint/component_bank.hpp>
#include <boost/dataflow/blueprint/network.hpp>
#include <boost/dataflow/blueprint/binary_operation_t.hpp>
#include <boost/dataflow/blueprint/port_t.hpp>

#include <boost/dataflow/signals/component/storage.hpp>
#include <boost/dataflow/signals/runtime_support.hpp>

#include <boost/graph/connected_components.hpp>

#include <iostream>

namespace blueprint=boost::dataflow::blueprint;
namespace df=boost::dataflow;

using namespace boost;

class example_bank : public blueprint::tag_component_bank<df::signals::tag>
{
public:
    example_bank()
    {
        add_component<signals::storage<void(int)> >("storage_int");
        add_component<signals::storage<void(float)> >("storage_float_100_1", 100.1f);
        add_component<signals::storage<void(float)> >("storage_float_0", 0.0f);
    }
};

class blueprint_example
{
    typedef blueprint::tag_network<df::signals::tag> network_type;

    // The network.
    network_type network;
    example_bank bank;

    // The components (graph indices).
    network_type::component_type source, sink, source_float, sink_float;

public:
    blueprint_example()
    {
        // Add the components to the network.
        source = network.add_component<signals::storage<void(int)> >(100),
        sink = network.add_component<signals::storage<void(int)> >(0),
        source_float = network.add_component(bank.make("storage_float_100_1"));
        sink_float = network.add_component(bank.make("storage_float_0"));
    }
    void run()
    {
        // Print some runtime port info
        std::cout << "Printing runtime info about all of the components:" << std::endl  << std::endl;
        print_port_info("source", source);
        print_port_info("sink", sink);
        print_port_info("source_float", source_float);
        print_port_info("sink_float", sink_float);
        
        // Print some runtime connectability info
        std::cout << std::endl << "Printing some runtime port connectability info:" << std::endl  << std::endl;
        print_connectability_info("source", source, 0, "sink", sink, 1);
        print_connectability_info("source", source, 1, "sink", sink, 0);
        print_connectability_info("source", source, 0, "sink", sink, 0);
        print_connectability_info("source", source, 0, "sink_float", sink_float, 1);
        std::cout << std::endl;
        
        // Make some connections
        std::cout << "Making blueprint connections: source to sink, source_float to sink_float..." << std::endl;
        network.add_connection(source, 0, sink, 1);
        network.add_connection(source_float, 0, sink_float, 1);
    
        // Do some analysis on the connectivity graph
        typedef boost::adjacency_list<
            boost::vecS, boost::vecS, boost::undirectedS> ugraph_type;
            
        ugraph_type g;
        copy_graph(network.graph(), g);
        std::vector<int> component(num_vertices(g));
        std::cout << std::endl << "Using BGL to analyze the network... BGL says there are "
            << connected_components(g, &component[0])
            << " independent parts of the network!" << std::endl << std::endl;
        
        // Copy the network
        std::cout << "Making a copy of the blueprint." << std::endl << std::endl;
        network_type network_copy(network);
        
        std::cout << "Testing the original network..." << std::endl << std::endl;
        test_network(network);
        std::cout << "Testing the network copy..." << std::endl << std::endl;
        test_network(network_copy);
    }
    void print_port_info(const char *c_name, network_type::component_type c)
    {
        std::cout << "\t" << c_name << " has " << network[c].num_ports() << " ports. " << std::endl;
        for (size_t i=0; i<network[c].num_ports(); i++)
        {
            blueprint::port &p = network.get_port(c, i);
            std::cout << "\t\tport " << i << " is a "
                << (p.traits().tag().name()) << " "
                << (p.traits().category().name())
                << (p.is_complemented_port() ? " (complemented_port)" : "");
            if (p.is_vector_port())
                std::cout << " (vector_port x " << p.as<blueprint::vector_port>().num_ports() << ")";
            std::cout
                << (network.get_port(c, i).is_keyed_port() ? " (keyed_port)" : "")
                << std::endl;
        }
        std::cout << std::endl;
    }
    void print_connectability_info(
        const char *src_name, network_type::component_type src, size_t src_port,
        const char *dst_name, network_type::component_type dst, size_t dst_port)
    {
        std::cout
            << src_name << " port " << src_port << " is "
            << (network.are_connectable(src, src_port, dst, dst_port) ? "" : "not ")
            << "connectable to " << dst_name << " port " << dst_port << std::endl;
    }

    template<typename T>
    void output_component_value(
        network_type &network, const char *c_name, typename network_type::component_type c)
    {
        blueprint::component_t<signals::storage<void(T)>, df::signals::tag> value;
        blueprint::extract(network[c].get_port(2), value.get_port(1));
 
        std::cout << "\t\tvalue at " << c_name << ":"
            << value.get().template at<0>()
            << std::endl;
    }
    void output_component_values(network_type &network)
    {
        std::cout << "\tValues at components:" << std::endl;    
        output_component_value<int>(network, "source", source);
        output_component_value<int>(network, "sink", sink);
        output_component_value<float>(network, "source_float", source_float);
        output_component_value<float>(network, "sink_float", sink_float);
        std::cout << std::endl;
    }

    void test_network(network_type &network)
    {
        // Output the values at the components:
        output_component_values(network);

        // Connect the underlying components
        std::cout << "\tConnecting the underlying components in the network..." << std::endl;    
        network.connect();
        
        // Invoke the sources
        std::cout << "\tInvoking the sources..." << std::endl;
        network[source].invoke();
        network[source_float].invoke();
        std::cout << std::endl;
        
        // Output the values at the sinks:
        output_component_values(network);
    }
};

int main()
{
    blueprint_example example;
    example.run();
    
    return 0;
}

//]
