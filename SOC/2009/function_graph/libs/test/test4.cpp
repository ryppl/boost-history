/**
 * Testing a function that has a range
 *
 * Functions checklist:
 *  [X] source(e, g)
 *  [X] target(e, g)
 *  [X] out_edges(u, g)
 *  [X] out_degree(u, g)
 *  [X] in_edges(v, g)
 *  [X] in_degree(v, g)
 *  [ ] degree(v, g)
 *  [ ] adjacent_vertices(v, g)
 *  [X] vertices(g)
 *  [X] num_vertives(g)
 *  [X] edegs(g)
 *  [X] num_edges(g)
 */

#include <iostream>
#include <boost/function.hpp>
#include <functional>
#include <vector>
#include "function_graph.hpp"
#include <boost/range.hpp>
#include <cassert>
#include <boost/assert.hpp>
#include <boost/graph/graph_traits.hpp>

#define META_ASSERT(x) BOOST_ASSERT(x::value)

namespace t3 {
// print an edge
template<typename Graph>
void print_edge(typename Graph::edge_descriptor const& e, Graph const& g)
{
    std::cout << "\nEdge:\n" << "  Source: " << e.source << "\n";
    std::cout << "  Target: " << e.target << "\n";
}
template<typename Graph>
void print_edge_alt(typename Graph::edge_descriptor const& e, Graph const& g)
{
    std::cout << "\n(" << e.source << ", " << e.target << ")";
}
} // namespace

int main()
{
    ////////
    // Data for graphh
    long arr[8] = {56233, 572, -7851, 956242, -2, 893437, 63, 4474443};
    std::vector<long> numbers(arr, arr + 8);

    ////////
    // Typedefs
    typedef std::pair<
                std::vector<long>::iterator,
                std::vector<long>::iterator
            > iterator_range;
    typedef boost::function<bool(long, long)> boolean_func;
    typedef boost::function<long(long, long)> weighted_func;
    typedef boost::function_graph<boolean_func, iterator_range> boolean_graph;
    typedef boost::function_graph<weighted_func, iterator_range> weighted_graph;
    typedef std::pair<
                boolean_graph::in_edge_iterator,
                boolean_graph::in_edge_iterator
            > in_edge_bool_range;
    typedef std::pair<
                weighted_graph::in_edge_iterator,
                weighted_graph::in_edge_iterator
            > in_edge_wght_range;
    typedef std::pair<
                boolean_graph::out_edge_iterator,
                boolean_graph::out_edge_iterator
            > out_edge_bool_range;
    typedef std::pair<
                weighted_graph::out_edge_iterator,
                weighted_graph::out_edge_iterator
            > out_edge_wght_range;
    typedef std::pair<
                boolean_graph::edge_iterator,
                boolean_graph::edge_iterator
            > edge_bool_range;
    typedef std::pair<
                weighted_graph::edge_iterator,
                weighted_graph::edge_iterator
            > edge_wght_range;
    typedef std::pair<
                boolean_graph::adjacency_iterator,
                boolean_graph::adjacency_iterator
            > adjacency_bool_range;
    typedef std::pair<
                weighted_graph::adjacency_iterator,
                weighted_graph::adjacency_iterator
            > adjacency_wght_range;



    ////////
    // Concept checking
    META_ASSERT(boost::is_directed_graph<boolean_graph>);
    META_ASSERT(boost::is_directed_graph<weighted_graph>);
    META_ASSERT(!boost::is_multigraph<boolean_graph>);
    META_ASSERT(!boost::is_multigraph<weighted_graph>);
    META_ASSERT(boost::is_incidence_graph<boolean_graph>);
    META_ASSERT(boost::is_incidence_graph<weighted_graph>);
    META_ASSERT(boost::is_bidirectional_graph<boolean_graph>);
    META_ASSERT(boost::is_bidirectional_graph<weighted_graph>);
    META_ASSERT(boost::is_vertex_list_graph<boolean_graph>);
    META_ASSERT(boost::is_vertex_list_graph<weighted_graph>);
    META_ASSERT(boost::is_edge_list_graph<boolean_graph>);
    META_ASSERT(boost::is_edge_list_graph<weighted_graph>);
    META_ASSERT(boost::is_directed_bidirectional_graph<boolean_graph>);
    META_ASSERT(boost::is_directed_bidirectional_graph<weighted_graph>);



    ////////
    // Create function graphs
    boolean_graph booleanGraph(std::less<long>(),
                               std::make_pair(numbers.begin(), numbers.end()));
    weighted_graph weightedGraph(std::minus<long>(),
                                std::make_pair(numbers.begin(), numbers.end()));



    ////////
    // Check vertices(g)
    iterator_range correctRange = std::make_pair(numbers.begin(),
                                                 numbers.end());
    assert(vertices(booleanGraph) == correctRange);
    assert(vertices(weightedGraph) == correctRange);



    ////////
    // Check num_vertices
    std::vector<long>::size_type numVertices = numbers.size();
    assert(num_vertices(booleanGraph) == numVertices);
    assert(num_vertices(weightedGraph) == numVertices);



    ////////
    // Check in edges
    std::vector<long>::iterator aVertex = ++numbers.begin(); // aVector = 572
    in_edge_bool_range in_edges_bool = boost::in_edges(*aVertex, booleanGraph);
    // Print all in_edges from booleanGraph to 572
    // In other words, print pairs of numbers that are less than 572
    unsigned int in_edges_count = 0;
    while(in_edges_bool.first != in_edges_bool.second)
    {
        t3::print_edge(*in_edges_bool.first, booleanGraph);
        ++in_edges_bool.first;
        ++in_edges_count;
    }
    assert(in_edges_count == in_degree(*aVertex, booleanGraph));

    in_edge_wght_range in_edges_wght = boost::in_edges(*aVertex, weightedGraph);
    // Print all in_edges from weightedGraph to 572
    // By the function, this prints in_edge - 572
    in_edges_count = 0;
    while(in_edges_wght.first != in_edges_wght.second)
    {
        std::cout << boost::source(*in_edges_wght.first, weightedGraph)
                  << " - 572 = " << (*in_edges_wght.first).result
                  << "\n";
        ++in_edges_wght.first;
        ++in_edges_count;
    }
    assert(in_edges_count == in_degree(*aVertex, weightedGraph));
    std::cout << "\n\n";


    ////////
    // Check out edges
    std::cout << "\n\n////Out Edges Check////\n\n";

    ++++++aVertex; // aVertex now holds -2
     out_edge_bool_range out_edges_bool = boost::out_edges(*aVertex,
                                                           booleanGraph);
    // Print all out_edges from booleanGraph to -2
    // In other words, print pairs of numbers that are not less than -2
    unsigned int out_edges_count = 0;
    while(out_edges_bool.first != out_edges_bool.second)
    {
        t3::print_edge(*out_edges_bool.first, booleanGraph);
        ++out_edges_bool.first;
        ++out_edges_count;
    }
    std::cerr << "\n\nout_edges_count : " << out_edges_count << "\n";
    assert(out_edges_count == out_degree(*aVertex, booleanGraph));

    out_edge_wght_range out_edges_wght = boost::out_edges(*aVertex,
                                                          weightedGraph);
    // Print all in_edges from weightedGraph to 572
    // By the function, this prints in_edge - 572
    out_edges_count = 0;
    while(out_edges_wght.first != out_edges_wght.second)
    {
        std::cout << "-2 - "
                  << boost::target(*out_edges_wght.first, weightedGraph)
                  << " = "
                  << (*out_edges_wght.first).result
                  << "\n";
        ++out_edges_wght.first;
        ++out_edges_count;
    }
    
    assert(out_edges_count == out_degree(*aVertex, weightedGraph));
    std::cout << "\n";



    ////////
    // Check edges
    std::cout << "\n\n////Edges Check////\n\n";

    edge_bool_range edges_bool = boost::edges(booleanGraph);
    unsigned int edges_count = 0;
    while(edges_bool.first != edges_bool.second)
    {
        t3::print_edge_alt(*edges_bool.first, booleanGraph);
        ++edges_bool.first;
        ++edges_count;  //TEMP: is 4
    }
    assert(edges_count == boost::num_edges(booleanGraph));

    edge_wght_range edges_wght = boost::edges(weightedGraph);
    edges_count = 0;
    while(edges_wght.first != edges_wght.second)
    {
        t3::print_edge_alt(*edges_wght.first, weightedGraph);
        ++edges_wght.first;
        ++edges_count;
    }
    // Since this is all pairings of all vertices, num_edges = 8*8 = 64
    assert(edges_count == num_edges(weightedGraph) && edges_count == 64);



    ////////
    // Check adjacent edges
    std::cout << "\n\n////Adjacency Check////\n\n";

    ++++aVertex;    // vertex is now 63
    adjacency_bool_range adjacent_bool =
                             boost::adjacent_vertices(*aVertex, booleanGraph);
    unsigned int adjacent_vertices_count = 0;
    while(adjacent_bool.first != adjacent_bool.second)
    {
        if(adjacent_bool.first.in_edge_check_)
            std::cout << "(" << *adjacent_bool.first << ", " << *aVertex << ")\n";
        else
            std::cout << "(" << *aVertex << ", " << *adjacent_bool.first << ")\n";
        ++adjacent_bool.first;
        ++adjacent_vertices_count;
    }
    assert(adjacent_vertices_count == degree(*aVertex, booleanGraph));

    return 0;
}
