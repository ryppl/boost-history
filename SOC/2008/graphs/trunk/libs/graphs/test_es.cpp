
#include <iostream>

#include <boost/next_prior.hpp>

#include <boost/graphs/edge_vector.hpp>
#include <boost/graphs/edge_list.hpp>
#include <boost/graphs/edge_set.hpp>

#include <boost/graphs/property_vector.hpp>
#include <boost/graphs/property_list.hpp>
#include <boost/graphs/incidence_vector.hpp>
#include <boost/graphs/incidence_list.hpp>
#include <boost/graphs/incidence_set.hpp>
#include <boost/graphs/out_vector.hpp>
#include <boost/graphs/out_list.hpp>
#include <boost/graphs/out_set.hpp>
#include <boost/graphs/in_vector.hpp>
#include <boost/graphs/in_list.hpp>
#include <boost/graphs/in_set.hpp>

#include "typestr.hpp"
#include "properties_traits.hpp"
#include "incidence_traits.hpp"
#include "out_edge_traits.hpp"
#include "in_edge_traits.hpp"

using namespace std;
using namespace boost;

// These are things that we can't do much about... Note that we can actually
// generate all of these types prior to the definition of the edge store - it
// seems a little odd since IncDesc is actually IN the edge store, but its
// not instantiated until a little later.
typedef int VertexProps;
typedef int EdgeProps;
typedef index_descriptor<size_t> VertexDesc;

// Add an edge with the given properties to the implicit vertex.
template <typename Props, typename Incs, typename Vertex, typename Edge>
void undirected_add_edge(Props& props, Incs& incs, Vertex v, Edge const& ep)
{
/*
    typedef typename Props::property_descriptor PropDesc;
    typedef typename Incs::incidence_descriptor IncDesc;
    typedef insertion_result<IncDesc> Result;

    size_t m = props.size();
    size_t n = incs.size();

    // Our global vertex.
    Vertex u = VertexDesc(0);

    // Insert algorithm: Try to stub out the edge locally first. If it succeeds,
    // then add the global property, and finally bind the incidence and property
    // descitpros into their respective "slots". Note that this is actually
    // faster than the
    Result r = incs.add(v);
    if(r.succeeded()) {
        PropDesc p = props.add(ep);
        incs.bind(r.value, p);
        props.bind(p, make_pair(u, v));

        BOOST_ASSERT(props.size() == m + 1);
        BOOST_ASSERT(incs.size() == n + 1);
        cout << "  * added " << u << "," << v << " -> " << props.properties(p) << endl;
    }
    else if(r.retained()) {
        PropDesc p = incs.properties(r.value);
        cout << "  * exists " << u << "," << v << " -> " << props.properties(p) << endl;
    }
    else {
        cout << "  * failed " << u << "," << v << endl;
    }
*/
}

template <typename Outs, typename Ins>
void directed_add_edge(Outs& outs, Ins& ins, VertexDesc v, EdgeProps const& ep)
{
    typedef typename Outs::out_descriptor OutDesc;
    typedef typename Ins::in_descriptor InDesc;
    typedef insertion_result<OutDesc> Result;

    size_t m = outs.size();

    // Here, we're adding edge (u,v). Note that outs is implicitly a member of
    // u and ins is a member of v.
    VertexDesc u(0);

    // Try to add the edge to the out list. If it succeeds then reciprocate the
    // insertion for the in edge set and bind them together.
    Result o = outs.add(v, ep);
    if(o.succeeded()) {
        Result i = ins.add(v, o.value);
        outs.bind(o.value, i.value);

        BOOST_ASSERT(outs.size() == m + 1);
        BOOST_ASSERT(ins.size() == 1);
        cout << "  * added " << u << "," << v << " -> " << outs.properties(o.value) << endl;
    }
    else if(o.retained()) {
        cout << "  * exists " << u << "," << v << " -> " << outs.properties(o.value) << endl;
    }
}

// Test the addition of edges as if we were adding them to an implicit vertex 0.
template <typename Props, typename Incs>
void undirected_add_edges(Props& props, Incs& incs)
{
    BOOST_ASSERT(props.empty());
    BOOST_ASSERT(incs.empty());

    // Add a bunch of implicit connections.
    size_t const N = 5;
    for(size_t i = 0; i < N; ++i) {
        undirected_add_edge(props, incs, VertexDesc(i + 1), 2 * (i + 1));
    }
    BOOST_ASSERT(props.size() == N);
    BOOST_ASSERT(incs.size() == N);

    // Just for good measure, add antoher that duplicates the first.
    undirected_add_edge(props, incs, VertexDesc(2), 1);
}

template <typename Outs, typename Ins>
void directed_add_edges(Outs& outs, Ins&)
{
    BOOST_ASSERT(outs.empty());

    // This is a little odd. The in edge set is actually specific to different
    // vertices, so construct new, empty sets on the fly.

    size_t const N = 5;
    for(size_t i = 0; i < N; ++i) {
        Ins ins;
        directed_add_edge(outs, ins, VertexDesc(i + 1), 2 * (i + 1));
    }

    // Just for good measure, add antoher that duplicates the first.
    Ins ins;
    directed_add_edge(outs, ins, VertexDesc(2), 1);
}

template <typename EdgeStore>
void
undirected()
{
    cout << "--- undirected " << typestr<EdgeStore>() << " ---" << endl;

    // Instantiate data structures related to the storage of edges and their
    // properties.
    typedef typename EdgeStore::template property_store<VertexDesc, EdgeProps>::type PropStore;
    typedef typename EdgeStore::template incidence_store<VertexDesc>::type IncStore;

    PropStore props;
    IncStore incs;

    cout << "  * " << typestr(properties_category(props)) << endl;
    cout << "  * " << typestr(incidence_category(incs)) << endl;

    cout << typestr<typename PropStore::vertex_descriptor>() << endl;
    cout << typestr<typename PropStore::incidence_descriptor>() << endl;


    undirected_add_edges(props, incs);
}

template <typename EdgeStore>
void
directed()
{
    cout << "--- directed " << typestr<EdgeStore>() << " ---" << endl;

    // Instantiate data structures related to the storage of edges and their
    // properties.
    typedef typename EdgeStore::template out_store<VertexDesc, EdgeProps>::type OutStore;
    typedef typename EdgeStore::template in_store<VertexDesc>::type InStore;

    OutStore outs;
    InStore ins;

    cout << "  * " << typestr(out_edge_category(outs)) << endl;
    cout << "  * " << typestr(in_edge_category(ins)) << endl;

    directed_add_edges(outs, ins);
}

int main()
{
    undirected<edge_vector<>>();
    undirected<edge_list<>>();
    undirected<edge_set<>>();

    directed<edge_vector<>>();
    directed<edge_list<>>();
    directed<edge_set<>>();

   return 0;
}