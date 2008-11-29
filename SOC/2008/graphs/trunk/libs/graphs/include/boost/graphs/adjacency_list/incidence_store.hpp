
#ifndef BOOST_GRAPHS_ADJLIST_INCIDENCE_STORE_HPP
#define BOOST_GRAPHS_ADJLIST_INCIDENCE_STORE_HPP

#include <vector>
#include <algorithm>

// Pull all of the incidence list type selectors
#include <boost/graphs/adjacency_list/incs/vector.hpp>
#include <boost/graphs/adjacency_list/incs/list.hpp>
#include <boost/graphs/adjacency_list/incs/set.hpp>

// The incidence store provides a means of selecting different kinds of storage
// mechanisms for the list of incident edges on undirected vertices. An
// incidence store simply contains the edge descriptors that are incident to
// a vertex - nothing more.

// The incidence store should be more appropriately called the adjacency store,
// but we're talking about incident edges, not adjacent vertices.

namespace boost { namespace graphs { namespace adjacency_list {

template <typename Store>
struct incidence_store_traits
{
    typedef typename Store::size_type incident_edges_size_type;

    // All incidence stores record a pair of descriptors (an adjacenct vertex
    // and an incident edge). We can extract these...
    typedef typename Store::value_type ai_pair;
    typedef typename ai_pair::first_type vertex_descriptor;
    typedef typename ai_pair::second_type edge_descriptor;
};

namespace incs {

namespace detail {

    template <typename Vertex>
    struct first_vertex_finder
    {
        Vertex v;

        first_vertex_finder(Vertex v) : v(v) { }

        template <typename Pair>
        bool operator()(Pair const& x) const
        { return x.first == v; }
    };

    template <typename Vertex>
    inline first_vertex_finder<Vertex> first_vertex(Vertex v)
    { return first_vertex_finder<Vertex>(v); }

    template <typename Store, typename Vertex>
    inline typename Store::iterator
    dispatch_find(Store& store, Vertex v, sequence_tag)
    { return std::find_if(store.begin(), store.end(), first_vertex(v)); }

    template <typename Store, typename Vertex>
    inline typename Store::iterator
    dispatch_find(Store& store, Vertex v, associative_container_tag)
    { return store.find(v); }

} /* namespace detail */

/** Insert an edge descriptor into the incidence store of the vertex. */
template <typename Store>
inline typename Store::iterator
insert(Store& store,
       typename incidence_store_traits<Store>::vertex_descriptor v,
       typename incidence_store_traits<Store>::edge_descriptor e)
{ return container_insert(store, std::make_pair(v, e)); }

/** @name Edge Object
 * Return the incident edge descriptor corresponding to the adjacent vertex.
 */
template <typename Store>
inline typename incidence_store_traits<Store>::edge_descriptor
find(Store& store, typename incidence_store_traits<Store>::vertex_descriptor v)
{
    typedef typename incidence_store_traits<Store>::edge_descriptor Edge;
    typename Store::iterator i = detail::dispatch_find(store, v, container_category(store));
    return i != store.end() ? i->second : Edge();
}

template <typename Store>
inline typename incidence_store_traits<Store>::edge_descriptor
find(Store const& store, typename incidence_store_traits<Store>::vertex_descriptor v)
{ return find(const_cast<Store&>(store), v); }

/** Return the size of an adjacency list for the given vertex, its degree. */
template <typename Store>
inline typename incidence_store_traits<Store>::incident_edges_size_type
size(Store const& store)
{ return store.size(); }

} /* namespace incs */

} } } /* namespace boost::graphs::adjacency_list */

#endif


