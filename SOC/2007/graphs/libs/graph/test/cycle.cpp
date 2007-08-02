// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <map>
#include <tr1/unordered_map>

#include <boost/graph/undirected_graph.hpp>
#include <boost/graph/directed_graph.hpp>
#include <boost/graph/tiernan_all_cycles.hpp>
#include <boost/graph/generators/cycle_graph.hpp>
#include <boost/graph/generators/prism_graph.hpp>
#include <boost/graph/generators/complete_graph.hpp>

using namespace std;
using namespace boost;

template <typename OStream>
struct cycle_printer
    : public cycle_visitor
{
    cycle_printer(OStream& os)
        : m_os(os)
    { }

    template <typename Path, typename Graph>
    void cycle(const Path& p, const Graph& g)
    {
        m_os << "cycle: ";
        typename Path::const_iterator i, end = p.end();
        for(i = p.begin(); i != end; ++i) {
            m_os << get(vertex_index, g, *i) << " ";
        }
        m_os << "\n";
    }

    OStream&    m_os;
};

template <typename SizeType>
struct cycle_counter
    : public cycle_visitor
{
    cycle_counter(SizeType& count)
        : m_count(count)
    { }

    template <typename Path, typename Graph>
    void cycle(const Path& p, const Graph& g)
    {
        ++m_count;
    }

    SizeType&   m_count;
};

template <typename Stream>
inline cycle_printer<Stream>
print_cycles(Stream& os)
{
    return cycle_printer<Stream>(os);
}

template <typename SizeType>
inline cycle_counter<SizeType>
count_cycles(SizeType& count)
{
    return cycle_counter<SizeType>(count);
}

template <typename Graph>
void build_graph(Graph& g)
{
    typedef typename Graph::vertex_descriptor Vertex;
    typedef typename Graph::edge_descriptor Edge;

    static const unsigned N = 5;
    vector<Vertex> v(N);

    // add some vertices
    for(size_t i = 0; i < N; ++i) {
        v[i] = add_vertex(g);
    }

    // add some edges (with weights)
    add_edge(v[0], v[1], g);
    add_edge(v[1], v[2], g);
    add_edge(v[2], v[0], g);

    add_edge(v[0], v[3], g);
    add_edge(v[3], v[4], g);
    add_edge(v[4], v[0], g);
};

template <typename Graph>
void test()
{
    Graph g;
    // build_graph(g);
    make_prism_graph(g, 3, 2);
    // make_complete_graph(g, 4, with_clockwise_cycle());

    size_t count = 0;
    tiernan_all_cycles(g, count_cycles(count));
    std::cout << "number of cycles: " << count << "\n";

    tiernan_all_cycles(g, print_cycles(cout));
}


int
main(int argc, char *argv[])
{
    typedef undirected_graph<> Graph;
    typedef directed_graph<> DiGraph;

    std::cout << "*** undirected ***\n";
    test<Graph>();

    std::cout << "*** directed ***\n";
    test<DiGraph>();
}
