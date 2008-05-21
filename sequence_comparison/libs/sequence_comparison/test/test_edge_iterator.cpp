// Copyright 2008 Chris Goller, Jeff Flinn, Brook Milligan and Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/sequence_comparison/edit_distance.hpp>

#include <boost/graph/dag_shortest_paths.hpp>
#include <string>

#include <boost/test/included/test_exec_monitor.hpp>

namespace scg = boost::sequence_comparison::graph;

template<typename Graph>
void test_edge(const scg::edge_descriptor &e, Graph &graph, unsigned si, unsigned sj, unsigned ti, unsigned tj)
{
    BOOST_CHECK_EQUAL(source(e, graph), scg::vertex_descriptor(si, sj));
    BOOST_CHECK_EQUAL(target(e, graph), scg::vertex_descriptor(ti, tj));
}

int test_main(int, char* [])
{    
    typedef scg::edit_distance<std::string::const_iterator,std::string::const_iterator> graph_type;
    typedef scg::weight_map<graph_type> weight_map_type;
    typedef scg::distance_map distance_map_type;
    
    std::string s1 ("ACGT");
    std::string s2 ("ACCT");
    graph_type graph(s1.begin(), s1.end(), s2.begin(), s2.end());
    
    BOOST_CHECK_EQUAL(graph.a_size(),4);
    BOOST_CHECK_EQUAL(graph.b_size(),4);

    std::pair<scg::edge_iterator, scg::edge_iterator> edges = out_edges(scg::vertex_descriptor(0,0), graph);
    scg::edge_iterator it = edges.first;
    test_edge(*it++, graph, 0, 0, 1, 0);
    test_edge(*it++, graph, 0, 0, 0, 1);
    test_edge(*it++, graph, 0, 0, 1, 1);
    BOOST_CHECK((it == scg::edge_iterator(scg::vertex_descriptor(0,0), graph, scg::edge_iterator::end())));
    
    return 0;
};

