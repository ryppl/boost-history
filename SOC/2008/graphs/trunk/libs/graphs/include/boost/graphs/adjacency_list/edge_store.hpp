
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

// Include specializations for the label and edge interfaces.
// TODO: Migrate code from these files to here.
#include <boost/graphs/adjacency_list/es/seq_edge.hpp>
#include <boost/graphs/adjacency_list/es/assoc_edge.hpp>

// The edge store interface defines generic operations on an edge store for
// undirected graphs.

// NOTE: Directed graphs do not use a global edge store. They use out and in
// edge stores and have a completely different interface.

// TODO: There is a significant overlap between the sequence selectors for
// vertex and edge stores. These could easily be integrated into a kind of
// general purpose selector mechanism. For now, they're separate.

namespace boost { namespace graphs { namespace adjacency_list {

template <typename Store>
struct edge_store_traits
{
    typedef typename descriptor_traits<Store, edge_descriptor_kind>::descriptor_kind
        edge_descriptor;

    typedef typename Store::value_type edge_type;
    typedef typename edge_traits<edge_type>::edge_ends edge_ends;
    typedef typename label_traits<edge_type>::label_type edge_label;
};

namespace es {

namespace detail {
    template <typename Store, typename Ends, typename Label>
    inline typename edge_store_traits<Store>::edge_type
    make_edge(Store const&, Ends e, Label&& l)
    { return typename edge_store_traits<Store>::edge_type(e, l); }

    template <typename Store, typename Ends, typename Label>
    inline typename Store::iterator
    dispatch_insert(Store& store, Ends e, Label&& l, sequence_tag)
    { return store.insert(store.end(), make_edge(store, e, l)); }

    // This re-orders the endpoints before inerting in order to guarantee a
    // canonical ordering edges, and preserving uniqueness, if required.
    // TODO: Is there a way to convert Comp<pair<VD,VD>> to Comp<VD>? Yeah, but
    // it's kind of nasty and uses template template parameters.
    template <typename Store, typename Ends, typename Label>
    inline typename Store::iterator
    dispatch_insert(Store& store, Ends e, Label&& l, pair_associative_container_tag)
    {
        // NOTE: Ends must be Pair<VD,VD>. For some reason, writing the expression
        // e.first < e.second tries to open a new template and gives an error
        // about constant expressions.
        typedef typename Ends::first_type VertexDesc;
        std::less<typename Ends::first_type> comp;
        if(!comp(e.first, e.second)) {
            swap(e.first, e.second);
        }
        return container_insert(store, make_edge(store, e, l));
    }
}

/** Insert an edge into the graph. */
template <typename Store, typename Ends, typename Label>
inline typename descriptor_traits<Store>::descriptor_type
insert(Store& store, Ends e, Label&& l)
{
    typedef typename Store::iterator Iterator;
    typedef typename descriptor_traits<Store>::descriptor_type Descriptor;

    // Start by inserting the edge globally. Could be O(1), O(lgE).
    Iterator i = detail::dispatch_insert(store, e, l, container_category(store));
    Descriptor d = make_descriptor(store, i);

    // Insert the edge into the incidence lists of each vertex.
    // TODO: implement me!
    // ies::insert(e.first, d);
    // ies::insert(e.second, d);
    return d;
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
typename Store::size_type size(Store const& store)
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
