
#include "typestr.hpp"

#include <iostream>

#include <boost/assert.hpp>
#include <boost/graphs/adjacency_list/undirected_graph.hpp>

#include "traits.hpp"
#include "gen/star.hpp"

using namespace std;
using namespace boost;
using namespace boost::graphs::adjacency_list;


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

    bool operator==(arc const& x) const     { return n == x.n; }

    int n;
};


namespace impl {
    template <typename Graph>
    typename Graph::vertex_descriptor do_add(Graph& g, int id, mpl::false_)
    { return add_vertex(g, node(id)); }

    template <typename Graph>
    typename Graph::vertex_descriptor do_add(Graph& g, int id, mpl::true_)
    { return add_vertex(g, id, node(id)); }
} /* namespace impl */

template <typename Graph>
typename Graph::vertex_descriptor add_vertex(Graph& g, int id)
{ return impl::do_add(g, id, has_mapped_vertices(g)); }


// Can't remove vertices... Don't do anything.
template <typename Graph, typename Vertex>
void test_remove_vert(Graph& g, Vertex v, mpl::false_)
{ }

template <typename Graph, typename Vertex>
void test_remove_vert(Graph& g, Vertex v, mpl::true_)
{
    remove_vertex(g, v);
    BOOST_ASSERT(num_vertices(g) == 2);
}

// TODO: Combine these test to make them more generic.
// Test vertices for simple vertex graphs
template <typename Graph>
void test_verts(Graph& g, mpl::false_)
{
    add_vertex(g, 1);
    add_vertex(g, 3);
    add_vertex(g, 5);
    BOOST_ASSERT(num_vertices(g) == 3);

    // Test the non-cosnt find
    typedef typename Graph::vertex_descriptor VertexDesc;
    VertexDesc v = vertex(g, node(3));
    BOOST_ASSERT(!v.is_null());
    BOOST_ASSERT(g[v] == node(3));

    test_remove_vert(g, v, can_remove_vertices(g));
}

// Test vertices for mapped vertex graphs
template <typename Graph>
void test_verts(Graph& g, mpl::true_)
{
    add_vertex(g, 1);
    add_vertex(g, 3);
    add_vertex(g, 5);
    BOOST_ASSERT(num_vertices(g) == 3);

    // Test the find also.
    typedef typename Graph::vertex_descriptor VertexDesc;
    VertexDesc v = vertex(g, 3);
    BOOST_ASSERT(!v.is_null());
    BOOST_ASSERT(g[v] == node(3));

    // Try to remove the vertex
    remove_vertex(g, v);
    BOOST_ASSERT(num_vertices(g) == 2);
}


template <typename Graph>
void test_edges(Graph& g)
{
    typedef typename Graph::vertex_descriptor Vertex;
    typedef typename Graph::edge_descriptor Edge;

    Vertex u = add_vertex(g, 100);
    Vertex v = add_vertex(g, 101);
    Vertex w = add_vertex(g, 102);
    Edge e1 = add_edge(g, u, v, arc(200));
    Edge e2 = add_edge(g, v, w, arc(201));
    Edge e3 = add_edge(g, w, u, arc(202));
    BOOST_ASSERT(num_edges(g) == 3);

    Edge t = edge(g, u, v);
    BOOST_ASSERT(!t.is_null());
    BOOST_ASSERT(g[t] == arc(200));

    remove_edge(g, t);
    BOOST_ASSERT(num_edges(g) == 2);

    add_edge(g, v, w);
    BOOST_ASSERT(num_edges(g) == 3);
    remove_edges(g, v, w);
    BOOST_ASSERT(num_edges(g) == 1);
}

template <typename Graph>
void test()
{
    typedef typename Graph::vertex_descriptor Vertex;
    Graph g;
    BOOST_ASSERT(num_vertices(g) == 0);

    test_verts(g, has_mapped_vertices(g));
    test_edges(g);

    Graph h;
    Vertex v = make_star(h, 5);
    BOOST_ASSERT(num_vertices(h) == 5);
    BOOST_ASSERT(num_edges(h) == 4);
    remove_edges(h, v);
    BOOST_ASSERT(num_edges(h) == 0);
}

int main()
{
    // Combinations of viable graph kinds
    typedef undirected_graph<node, arc, vertex_vector<>, edge_vector<>, incidence_vector<>> V_V_V;
    typedef undirected_graph<node, arc, vertex_list<>, edge_vector<>, incidence_vector<>> L_V_V;
    typedef undirected_graph<node, arc, vertex_set<>, edge_vector<>, incidence_vector<>> S_V_V;
    typedef undirected_graph<node, arc, vertex_map<int>, edge_vector<>, incidence_vector<>> M_V_V;
    typedef undirected_graph<node, arc, vertex_vector<>, edge_list<>, incidence_list<>> V_L_L;
    typedef undirected_graph<node, arc, vertex_vector<>, edge_list<>, incidence_set<>> V_L_S;

//    test<V_V_V>();
//     test<L_V_V>();
//     test<S_V_V>();
//     test<M_V_V>();
    test<V_L_L>();
//     test<V_L_S>();
}

