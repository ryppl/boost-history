
#ifndef BOOST_GRAPH_ADJLIST_VERTEX_STORE_TRAITS_HPP
#define BOOST_GRAPH_ADJLIST_VERTEX_STORE_TRAITS_HPP

#include <boost/concept_check.hpp>

#include <boost/containers.hpp>
#include <boost/descriptors.hpp>

#include <boost/graphs/label.hpp>

// The Vertex Store defines the generic interface for working with a vertex
// set for both directed and undirected adjacency lists. Its primary goal is
// to provide generic interfaces that abstract common operations on these
// sets. Important kinds of types are:
// - Store - the selected container. Must model Container.
// - Descriptor - a descriptor into the store
// - Result - pair<Descriptor, bool>
// - Size - the size type of the container
// - Vertex - a type of vertex. Can be labelled.
// - Label - a vertex label
// - Key - a key maapping to a vertex.
//
// Supported operations are:
// - Descriptor vs_add_vertex       (Store, Vertex)
// - Descriptor vs_add_vertex       (Store, Key, Vertex)
// - Result     vs_try_add_vertex   (Store, Vertex)
// - Result     vs_try_add_vertex   (Store, Key, Vertex)
// - Iterator   vs_find_vertex      (Store, Label)
// - void       vs_remove_vertex    (Store, Iterator)
// - bool       vs_empty            (Store)
// - Size       vs_size             (Store)

namespace boost { namespace graphs { namespace adjacency_list {

/**
 * The vertex_store_traits relates associated types with a kind of vertex store.
 * Mostly, this is useful in cases where we have to get the vertex type from
 * simple and pair associative containers.
 */
template <typename Store>
struct vertex_store_traits
{
    typedef typename Store::value_type vertex_type;
};

namespace vs {

namespace detail {

    // Iterate and compare for sequences.
    template <typename Store, typename Label>
    inline typename Store::iterator
    dispatch_find(Store& store, Label const& l, sequence_tag)
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
    dispatch_find(Store& store, Label const& l, associative_container_tag)
    { return store.find(l); }


    // Try to add the vertex to an associative container. This is a simple
    // delegation to the container.
    template <typename Store, typename Vertex>
    inline std::pair<typename Store::iterator, bool>
    dispatch_try_insert(Store& store, Vertex&& v, associative_container_tag)
    { return store.insert(v); }

    // Try to add the vertex to a sequential container. Here, we want to search
    // for the vertex (linear time operation) and either insert at the end
    // or return an existing iterator.
    template <typename Store, typename Vertex>
    inline std::pair<typename Store::iterator, bool>
    dispatch_try_insert(Store& store, Vertex&& v, sequence_tag)
    {
        typename Store::iterator i = dispatch_find(store, label(v), sequence_tag());
        return std::make_pair(i, i == store.end());
    }

    // Explicitly remove the ability use this function, by failing a concept
    // check when it is instantiated.
    template <typename Store>
    void dispatch_remove(Store&, typename Store::iterator, vector_tag)
    { function_requires(Integer<Store>()); }

    template <typename Store, typename Tag>
    void dispatch_remove(Store& store, typename Store::iterator i, Tag)
    { store.erase(i); }

} /* namespace detail */

/** @name Add Vertex
 * Add a vertex to the given store, returning an iterator to the new vertex
 * vertex. The semantics and performance of this function depend on the kind
 * of vertex store.
 */
//@{
/** Insert a vertex into the container. */
template <typename Store, typename Vertex>
inline typename descriptor_traits<Store>::descriptor_type
insert(Store& store, Vertex&& v)
{ return make_descriptor(store, container_insert(store, v)); }

/** Insert a vertex into the container, mapped to the given key. */
template <typename Store, typename Key, typename Vertex>
inline typename descriptor_traits<Store>::descriptor_type
insert(Store& store, Key const& k, Vertex&& v)
{ return make_descriptor(store, container_insert(store, std::make_pair(k, v))); }
//@}

/** @name Try to Add Vertex
 * Try to add a vertex to the container, failing to add if a vertex already
 * exists. This variant is only appropriate for labelled, unique vertices and
 * associative vertex stores. However, it can be adapted to sequences in
 * linear time.
 */
//@{
template <typename Store, typename Vertex>
inline std::pair<typename descriptor_traits<Store>::descriptor_type, bool>
try_insert(Store& store, Vertex&& v)
{
    std::pair<typename Store::iterator, bool> x =
        detail::dispatch_try_insert(store, v, container_category(store));
    return std::make_pair(make_descriptor(store, x.first), x.second);
}

// There are no mapped linear sequences, so we can just rely on the fact that
// this is a pair associative container. In fact, this actually requires that
// Store model the UniqueAssociativeContainer concept.
template <typename Store, typename Key, typename Vertex>
inline std::pair<typename descriptor_traits<Store>::descriptor_type, bool>
try_insert(Store& store, Key const& k, Vertex&& v)
{
    std::pair<typename Store::iterator, bool> x =
        store.insert(std::make_pair(k, v));
    return std::make_pair(make_descriptor(store, x.first), x.second);
}
//@}

/** @name Find Vertex
 * Return an iterator to the vertex that matches the given label. This is
 * only applicable to labelled vertex types. The returned iterator will be
 * equivalent to store.end() if no such vertex is found.
 *
 * If the store is a pair associative container (e.g., map), then this function
 * will find the vertex mapped to the given key. Otherwise, it tries to find
 * the vertex with the given label.
 */
//@{
template <typename Store, typename LabelOrKey>
inline typename descriptor_traits<Store>::descriptor_type
find(Store& store, LabelOrKey const& lk)
{ return make_descriptor(store, detail::dispatch_find(store, lk, container_category(store))); }

// Does not return a const iterator!
template <typename Store, typename LabelOrKey>
inline typename descriptor_traits<Store>::descriptor_type
find(Store const& store, LabelOrKey const& lk)
{ return find(const_cast<Store&>(store), lk, container_category(store)); }
//@}

/**
 * @name Remove Vertex
 * Remove the vertex from the store. This will only remove the vertex from the
 * store, it cannot operate on edges.
 */
//@{
template <typename Store>
void remove(Store& store, typename descriptor_traits<Store>::descriptor_type d)
{ detail::dispatch_remove(store, make_iterator(store, d), container_category(store)); }
//@}

/** Return the number of elements in the vertex store. */
template <typename Store>
typename Store::size_type size(Store const& store)
{ return store.size(); }

/** Return true if the store is empty. */
template <typename Store>
bool empty(Store const& store)
{ return store.empty(); }

/** Clear the vertex store. */
template <typename Store>
void clear(Store& store)
{ store.clear(); }

} /* namespace vs */

} } } /* namespace boost::graphs::adjacency_list */

// Include type selectors for the different vertex store implementations
#include <boost/graphs/adjacency_list/vs/vector.hpp>
#include <boost/graphs/adjacency_list/vs/list.hpp>
#include <boost/graphs/adjacency_list/vs/set.hpp>
#include <boost/graphs/adjacency_list/vs/map.hpp>

#endif
