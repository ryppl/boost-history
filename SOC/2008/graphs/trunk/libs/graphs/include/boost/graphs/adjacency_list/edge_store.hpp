
#ifndef BOOST_GRAPH_ADJLIST_EDGE_STORE_TRAITS_HPP
#define BOOST_GRAPH_ADJLIST_EDGE_STORE_TRAITS_HPP

#include <boost/concept_check.hpp>

#include <boost/containers.hpp>
#include <boost/descriptors.hpp>

#include <boost/graphs/label.hpp>
#include <boost/graphs/edge.hpp>

// Include specializations for the label and edge interfaces.
#include <boost/graphs/adjacency_list/es/sequence.hpp>
#include <boost/graphs/adjacency_list/es/association.hpp>

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
    typedef typename Store::value_type edge_type;
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
        if(!(e.first < e.second)) swap(e.first, e.second);
        return container_insert(store, make_edge(store, e, l));
    }
}

/** Insert . */
template <typename Store, typename Ends, typename Label>
inline typename descriptor_traits<Store>::descriptor_type
insert(Store& store, Ends e, Label&& l)
{ return make_descriptor(store, detail::dispatch_insert(store, e, l, container_category(store))); }

/** Return the number of edges in the edge store. */
template <typename Store>
typename Store::size_type size(Store const& store)
{ return store.size(); }

/** @name Edge Label
 * Return a lable for the given edge.
 */
//@{
template <typename Store>
inline typename label_traits<typename edge_store_traits<Store>::edge_type>::label_type&
label(Store& store, typename descriptor_traits<Store>::descriptor_type d)
{ return graphs::label(*make_iterator(store, d)); }

template <typename Store>
inline typename label_traits<typename edge_store_traits<Store>::edge_type>::label_type const&
label(Store const& store, typename descriptor_traits<Store>::descriptor_type d)
{ return graphs::label(*make_iterator(store, d)); }
//@}

} /* namespace es */

} } } /* namespace boost::graphs::adjacency_list */

// Type selectors
#include <boost/graphs/adjacency_list/es/vector.hpp>
#include <boost/graphs/adjacency_list/es/set.hpp>

#endif
