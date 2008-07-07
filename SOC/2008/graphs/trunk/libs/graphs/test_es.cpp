
#include <iostream>

#include <boost/next_prior.hpp>
#include <boost/graphs/edge_vector.hpp>
#include <boost/graphs/edge_list.hpp>
#include <boost/graphs/edge_set.hpp>

#include "typestr.hpp"
#include "properties_traits.hpp"
#include "incidence_traits.hpp"

using namespace std;
using namespace boost;

// These are things that we can't do much about... Note that we can actually
// generate all of these types prior to the definition of the edge store - it
// seems a little odd since IncDesc is actually IN the edge store, but its
// not instantiated until a little later.
typedef int VertexProps;
typedef int EdgeProps;
typedef index_descriptor<size_t> VertexDesc;

template <typename Props, typename Incs, typename Vertex, typename Edge>
void add_edge(Props& props, Incs& incs, Vertex v, Edge const& ep, associative_container_tag)
{
    typedef typename Props::property_descriptor PropDesc;
    typedef typename Incs::incidence_descriptor IncDesc;

    Vertex u = VertexDesc(0);

    // This is a little different protocol.. First, try to insert the edge
    // into the incidence list, but lave the property slot "empty".
    IncDesc i = incs.add(v);
    if(incs.valid(i)) {
        PropDesc p = props.add(ep);
        incs.bind(i, p);
        props.bind(p, make_pair(u, v));
        cout << "  * added: " << u << ", " << v << endl;
    }
    else {
        cout << "  * did not add: " << u << ", " << v << endl;
    }
}

// These are basically multigraphs (i.e., multiple edges allowed).
template <typename Props, typename Incs, typename Vertex, typename Edge>
void add_edge(Props& props, Incs& incs, Vertex v, Edge const& ep, sequence_tag)
{
    typedef typename Props::property_descriptor PropDesc;
    typedef typename Incs::incidence_descriptor IncDesc;

    Vertex u = VertexDesc(0);

    // Pretty simple... add the vertex and create the incident pair, and then
    // bind both the implicit (0) vertex and v to the property.
    size_t m = props.size(), n = incs.size();
    PropDesc p = props.add(ep);
    IncDesc i = incs.add(v, p);
    props.bind(p, make_pair(u, v));

    cout << "  * added: " << u << ", " << v << endl;
    BOOST_ASSERT(props.size() == m + 1);
    BOOST_ASSERT(incs.size() == n + 1);
}

// Add an edge with the given properties to the implicit vertex.
template <typename Props, typename Incs, typename Vertex, typename Edge>
void add_edge(Props& props, Incs& incs, Vertex v, Edge const& ep)
{
    add_edge(props, incs, v, ep, incidence_category(incs));
}

// Test the addition of edges as if we were adding them to an implicit vertex 0.
template <typename Props, typename Incs>
void add_edges(Props& props, Incs& incs)
{
    BOOST_ASSERT(props.empty());
    BOOST_ASSERT(incs.empty());

    size_t const N = 5;
    for(size_t i = 0; i < N; ++i) {
        add_edge(props, incs, VertexDesc(i + 1), i * i);
    }

    // Just for good measure, add antoher that duplicates the first.
    add_edge(props, incs, VertexDesc(1), 1);
}

template <typename EdgeStore>
void
undirected()
{
    cout << "--- " << typestr<EdgeStore>() << " ---" << endl;

    // Instantiate data structures related to the storage of edges and their
    // properties.
    typedef typename EdgeStore::template property_store<EdgeProps, VertexDesc>::type PropStore;
    typedef typename EdgeStore::template incidence_store<VertexDesc>::type IncStore;

    PropStore props;
    IncStore incs;

    cout << "  * " << typestr(properties_category(props)) << endl;
    cout << "  * " << typestr(incidence_category(incs)) << endl;

    add_edges(props, incs);
}

int main()
{
    undirected<edge_vector<>>();
    undirected<edge_list<>>();
    undirected<edge_set<>>();

   return 0;
}