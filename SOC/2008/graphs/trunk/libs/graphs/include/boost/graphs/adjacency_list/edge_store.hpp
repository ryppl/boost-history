
#ifndef BOOST_GRAPH_ADJLIST_EDGE_STORE_TRAITS_HPP
#define BOOST_GRAPH_ADJLIST_EDGE_STORE_TRAITS_HPP

#include <boost/containers.hpp>
#include <boost/descriptors.hpp>

#include <boost/graphs/label.hpp>
#include <boost/graphs/edge.hpp>

// Defines a kind of edge descriptor.
struct edge_descriptor_kind { };

#include <boost/graphs/adjacency_list/es/vector.hpp>
#include <boost/graphs/adjacency_list/es/list.hpp>
#include <boost/graphs/adjacency_list/es/set.hpp>

// The edge store interface defines generic operations on an edge store for
// undirected graphs.

// NOTE: Directed graphs do not use a global edge store. They use out and in
// edge stores and have a completely different interface.

namespace boost { namespace graphs {

/** @name Label Traits [edge_vector, edge_list] */
//@{
template <typename Vertex, typename Label>
struct label_traits<std::pair<std::pair<Vertex, Vertex>, Label>>
{
    typedef Label label_type;
};

template <typename Vertex, typename Label>
inline Label&
label(std::pair<std::pair<Vertex, Vertex>, Label>& edge)
{ return edge.second; }

template <typename Vertex, typename Label>
inline Label const&
label(std::pair<std::pair<Vertex, Vertex>, Label> const& edge)
{ return edge.second; }
//@}

/** @name Label Traits [edge_set] */
//@{
template <typename Vertex, typename Label>
struct label_traits<std::pair<std::pair<Vertex, Vertex> const, Label>>
{
    typedef Label label_type;
};

template <typename Vertex, typename Label>
inline Label&
label(std::pair<std::pair<Vertex, Vertex> const, Label>& edge)
{ return edge.second; }

template <typename Vertex, typename Label>
inline Label const&
label(std::pair<std::pair<Vertex, Vertex> const, Label> const& edge)
{ return edge.second; }
//@}

// NOTE: The difference between the specialization of edge traits over the
// edge types is quite subtle. Compare:
// std::pair<std::pair<Vertex, Vertex>, Label>  <- Vector and List
// std::pair<std::pair<Vertex, Vertex> const, Label>  <- Set
// Even worse, there's no functional difference between then.

/** @name Edge Traits [edge_vector, edge_list] */
//@{
template <typename Vertex, typename Label>
struct edge_traits<std::pair<std::pair<Vertex, Vertex>, Label>>
{
    typedef Vertex vertex_descriptor;
    typedef std::pair<Vertex, Vertex> edge_ends;
};

template <typename Vertex, typename Label>
inline Label const&
ends(std::pair<std::pair<Vertex, Vertex>, Label> const& edge)
{ return edge.first; }

template <typename Vertex, typename Label>
inline Vertex
first(std::pair<std::pair<Vertex, Vertex>, Label> const& edge)
{ return edge.first.first; }

template <typename Vertex, typename Label>
inline Vertex
second(std::pair<std::pair<Vertex, Vertex>, Label> const& edge)
{ return edge.first.second; }

template <typename Vertex, typename Label>
inline Vertex
oppposite(std::pair<std::pair<Vertex, Vertex>, Label> const& edge, Vertex which)
{ return which == first(edge) ? second(edge) : first(edge); }
//@}

/** @name Edge Traits [edge_set] */
//@{
template <typename Vertex, typename Label>
struct edge_traits<std::pair<std::pair<Vertex, Vertex> const, Label>>
{
    typedef Vertex vertex_descriptor;
    typedef std::pair<Vertex, Vertex> edge_ends;
};

template <typename Vertex, typename Label>
inline std::pair<Vertex, Vertex>
ends(std::pair<std::pair<Vertex, Vertex> const, Label> const& edge)
{ return edge.first; }

template <typename Vertex, typename Label>
inline Vertex
first(std::pair<std::pair<Vertex, Vertex> const, Label> const& edge)
{ return edge.first.first; }

template <typename Vertex, typename Label>
inline Vertex
second(std::pair<std::pair<Vertex, Vertex> const, Label> const& edge)
{ return edge.first.second; }

template <typename Vertex, typename Label>
inline Vertex
oppposite(std::pair<std::pair<Vertex, Vertex> const, Label> const& edge, Vertex which)
{ return which == first(edge) ? second(edge) : first(edge); }
//@}

namespace adjacency_list {

template <typename Store>
struct edge_store_traits
{
    typedef typename Store::size_type edges_size_type;
    typedef typename descriptor_traits<Store, edge_descriptor_kind>::descriptor_type
        edge_descriptor;

    typedef typename Store::value_type edge_type;
    typedef typename edge_traits<edge_type>::edge_ends edge_ends;
    typedef typename label_traits<edge_type>::label_type edge_label;

    typedef typename edge_traits<edge_type>::vertex_descriptor vertex_descriptor;
};

namespace es {

namespace detail {
    template <typename Store, typename Ends, typename Label>
    inline typename edge_store_traits<Store>::edge_type
    make_edge(Store const&, Ends e, Label&& l)
    { return typename edge_store_traits<Store>::edge_type(e, l); }

    // This is one of the few non-generic components of the library. We're
    // fixing the ordering of vertices within the end pair as strictly less.
    // Since descriptors are required to define this operation, we're in good
    // shape.
    template <typename Vertex>
    inline typename std::pair<Vertex, Vertex> order_verts(Vertex& u, Vertex& v)
    { return (u < v) ? std::make_pair(u, v) : std::make_pair(v, u); }

    // Just push the edge into the sequence. Since these types support a
    // multigraph, the ordering of endpoints is essentially irrelevant. This
    // always inserts so the returned pair is always true.
    template <typename Store, typename Vertex, typename Label>
    inline std::pair<typename Store::iterator, bool>
    dispatch_insert(Store& store, Vertex u, Vertex v, Label&& l, sequence_tag)
    {
        typename edge_store_traits<Store>::edge_type e =
            make_edge(store, std::make_pair(u, v), l);
        return std::make_pair(store.insert(store.end(), e), true);
    }

    // Unique associative containers may fail the insertion, returning false
    // as the second.
    template <typename Store, typename Edge>
    inline std::pair<typename Store::iterator, bool>
    assoc_insert(Store& store, Edge&& e, unique_associative_container_tag)
    { return store.insert(e); }

    // Pair associative containers always permit insertion.
    template<typename Store, typename Edge>
    inline std::pair<typename Store::iterator, bool>
    assoc_insert(Store& store, Edge&& e, multiple_associative_container_tag)
    { return std::make_pair(store.insert(e), true); }

    // This matches all associative containers, but we have to dispatch
    // separately for unique and multiple.
    template <typename Store, typename Vertex, typename Label>
    inline std::pair<typename Store::iterator, bool>
    dispatch_insert(Store& store, Vertex u, Vertex v, Label&& l, associative_container_tag)
    {
        typename edge_store_traits<Store>::edge_type e =
            make_edge(store, order_verts(u, v), l);
        return assoc_insert(store, std::move(e), container_category(store));
    }
} /* namespace detail */

/**
 * Insert an edge into the graph. If the edge set is uniquely associative, then
 * this may not insert the edge, and will return a descriptor to the exsiting
 * edge.
 */
template <typename Store, typename Vertex, typename Label>
inline std::pair<typename edge_store_traits<Store>::edge_descriptor, bool>
insert(Store& store, Vertex u, Vertex v, Label&& l)
{
    typedef typename Store::iterator Iterator;
    typedef typename edge_store_traits<Store>::edge_descriptor Edge;
    std::pair<Iterator, bool> x =
        detail::dispatch_insert(store, u, v, l, container_category(store));
    Edge e = make_descriptor(store, x.first, edge_descriptor_kind());
    return std::make_pair(e, x.second);
}

/** @name Find Edge
 * Find an edge with the given endpoints, and return a descriptor to the first
 * such edge found.
 */
//@{
template <typename Store, typename Ends>
inline typename descriptor_traits<Store>::descriptor_type
find(Store& store, Ends e)
{
    // Search the incidene list of the smaller vertex for an edge who's endpoint
    // is given as the other.
    // if(ies::size(e.first) < ies::size(e.second)
}

template <typename Store, typename Ends>
inline typename descriptor_traits<Store>::descriptor_type
find(Store const& store, Ends e)
{ return find(const_cast<Store&>(store), e); }
//@}

/** Return the number of edges in the edge store. */
template <typename Store>
typename edge_store_traits<Store>::edges_size_type size(Store const& store)
{ return store.size(); }

/** Return true if the global edge set is empty. */
template <typename Store>
bool empty(Store const& store)
{ return store.empty(); }

/** @name Edge Label
 * Return a lable for the given edge.
 */
//@{
template <typename Store>
inline typename edge_store_traits<Store>::edge_label&
label(Store& store, typename descriptor_traits<Store>::descriptor_type d)
{ return graphs::label(*make_iterator(store, d)); }

template <typename Store>
inline typename edge_store_traits<Store>::edge_label const&
label(Store const& store, typename descriptor_traits<Store>::descriptor_type d)
{ return graphs::label(*make_iterator(store, d)); }
//@}

/** @name Endpoints
 * Return a pair of endpoints for the edge referenced by the given descriptor.
 * Note that Edge endpoints are always passed and returned by value.
 */
//@{
template <typename Store>
inline typename edge_store_traits<Store>::edge_ends
ends(Store& store, typename descriptor_traits<Store>::descriptor_type d)
{
    graphs::ends(*make_iterator(store, d));
}
//@}

} /* namespace es */

} } } /* namespace boost::graphs::adjacency_list */

#endif
