// Copyright 2008 Chris Goller, Jeff Flinn, Brook Milligan and Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/sequence_comparison/edit_distance.hpp>

#include <boost/graph/dag_shortest_paths.hpp>
#include <string>

int main(int, char* [])
{
    namespace scg = boost::sequence_comparison::graph;
    
    typedef scg::edit_distance<std::string::const_iterator,std::string::const_iterator> graph_type;
    typedef scg::weight_map<graph_type> weight_map_type;
    typedef scg::distance_map distance_map_type;
    
    std::string s1 ("ACGOT");
    std::string s2 ("ACCT");
    graph_type graph(s1.begin(), s1.end(), s2.begin(), s2.end());
    distance_map_type distance_map("Distance");
    
    boost::dag_shortest_paths(graph, graph.upper_left(),
        boost::weight_map(weight_map_type(graph))
        .distance_map(distance_map)
        .color_map(distance_map_type("Color"))
        .vertex_index_map(distance_map_type("Predecessor")));
        
    std::cout << "edit distance: " << get(distance_map, scg::vertex_descriptor(graph.a_size(),graph.b_size())) << std::endl;

    return 0;
};
