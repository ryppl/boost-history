
#ifndef BOOST_GRAPH_ADJLIST_VERTEX_STORE_TRAITS_HPP
#define BOOST_GRAPH_ADJLIST_VERTEX_STORE_TRAITS_HPP

#include <boost/containers.hpp>
#include <boost/descriptors.hpp>

#include <boost/graphs/label.hpp>
#include <boost/graphs/adjacency_list/vertex_iterator.hpp>

namespace boost { namespace graphs { namespace adjacency_list {

/**
 * The vertex store traits defines the basic types associate with a vertex set.
 */
template <typename VertexStore>
struct vertex_store_traits
{ };

namespace detail
{
    // Iterate and compare for sequences.
    template <typename Store, typename Label>
    inline typename Store::iterator
    dispatch_vs_find(Store& store, Label const& l, sequence_tag)
    {
        return std::find_if(
            store.begin(),
            store.end(),
            bind2nd(labelled_equal_to<typename Store::value_type>(), l));
    }

    // Iterate and compare for sequences.
    template <typename Store, typename Label>
    inline typename Store::const_iterator
    dispatch_vs_find(Store const& store, Label const& l, sequence_tag)
    {
        return std::find_if(
            store.begin(),
            store.end(),
            bind2nd(labelled_equal_to<typename Store::value_type>(), l));
    }

    // Associative containers already forward the label as the key, so we just
    // have to use the basic find command.
    template <typename Store, typename Label>
    inline typename Store::iterator
    dispatch_vs_find(Store& store, Label const& l, associative_container_tag)
    { return store.find(l); }

    template <typename Store, typename Label>
    inline typename Store::const_iterator
    dispatch_vs_find(Store const& store, Label const& l, associative_container_tag)
    { return store.find(l); }
}


/**
 * Add a vertex to the given store, returning an iterator to the new vertex
 * vertex. The semantics and performance of this function depend on the kind
 * of vertex store.
 */
template <typename Store, typename Vertex>
inline typename Store::iterator
vs_add_vertex(Store& store, Vertex&& v)
{ return container_insert(store, v); }

/** @name Find Vertex
 * Return an iterator to the vertex that matches the given label. This is
 * only applicable to labelled vertex types. The returned iterator will be
 * equivalent to store.end() if no such vertex is found.
 */
//@{

template <typename Store, typename Label>
inline typename Store::iterator
vs_find_vertex(Store& store, Label const& l)
{ return detail::dispatch_vs_find(store, l, container_category(store)); }

template <typename Store, typename Label>
inline typename Store::const_iterator
vs_find_vertex(Store const& store, Label const& l)
{ return detail::dispatch_vs_find(store, l, container_category(store)); }
//@}
// template <typename 

} } } /* namespace boost::graphs::adjacency_list */

#endif
