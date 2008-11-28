
#include <iostream>

#include <boost/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/graphs/adjacency_list/undirected_graph.hpp>

#include "typestr.hpp"

using namespace std;
using namespace boost;
using namespace boost::graphs::adjacency_list;

template <typename Graph>
struct _has_mapped_vertices
{ typedef mpl::false_ type; };

template <
    typename VL, typename EL,
    typename K, template <typename> class C, template <typename> class A,
    typename ES, typename IS>
struct _has_mapped_vertices<undirected_graph<VL, EL, vertex_map<K,C,A>, ES, IS>>
{ typedef mpl::true_ type; };

template <typename Graph>
typename _has_mapped_vertices<Graph>::type
has_mapped_vertices(Graph const& g)
{ return typename _has_mapped_vertices<Graph>::type(); }

struct node
{
    node() : n() { }
    node(int n) : n(n) { }

    bool operator==(node const& x) const    { return n == x.n; }
    bool operator<(node const& x) const     { return n < x.n; }

    int n;
};

struct arc
{
    arc() : n() { }
    arc(int n) : n(n) { }
    int n;
};

template <typename Graph>
void add_vertices(Graph& g, mpl::false_)
{
    add_vertex(g);  // This is really a test for unlabeled verts
    add_vertex(g, node(3));
    add_vertex(g, std::move(node(5)));
    BOOST_ASSERT(num_vertices(g) == 3);

    // Test the non-cosnt find
    {
        typedef typename Graph::vertex_descriptor VertexDesc;
        VertexDesc v = find_vertex(g, node(3));
        BOOST_ASSERT(!v.is_null());
        BOOST_ASSERT(g[v] == node(3));
    }

    // Test the const find.
    {
        Graph const& h = g;
        typedef typename Graph::vertex_descriptor VertexDesc;
        VertexDesc v = find_vertex(h, node(3));
        BOOST_ASSERT(!v.is_null());
        BOOST_ASSERT(h[v] == node(3));
    }
}

template <typename Graph>
void add_vertices(Graph& g, mpl::true_)
{
    add_vertex(g, "a", node());
    add_vertex(g, "b", node(3));
    add_vertex(g, "c", std::move(node(5)));
    BOOST_ASSERT(num_vertices(g) == 3);

    // Test the find also.
    typedef typename Graph::vertex_descriptor VertexDesc;
    VertexDesc v = find_vertex(g, "b");
    BOOST_ASSERT(!v.is_null());
    BOOST_ASSERT(g[v] == node(3));
}

template <typename Graph>
void test()
{
    Graph g;
    BOOST_ASSERT(num_vertices(g) == 0);

    add_vertices(g, has_mapped_vertices(g));
}

int main()
{
    typedef undirected_graph<node, arc, vertex_vector<>, edge_vector<>, incidence_vector<>> VVV;
    typedef undirected_graph<node, arc, vertex_list<>, edge_vector<>, incidence_vector<>> LVV;
    typedef undirected_graph<node, arc, vertex_set<>, edge_vector<>, incidence_vector<>> SVV;
    typedef undirected_graph<node, arc, vertex_map<string>, edge_vector<>, incidence_vector<>> MVV;

    test<VVV>();
    test<LVV>();
    test<SVV>();
    test<MVV>();
}

