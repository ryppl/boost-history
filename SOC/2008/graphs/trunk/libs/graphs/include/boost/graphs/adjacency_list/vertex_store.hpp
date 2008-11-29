
#ifndef BOOST_GRAPH_ADJLIST_VERTEX_STORE_TRAITS_HPP
#define BOOST_GRAPH_ADJLIST_VERTEX_STORE_TRAITS_HPP


#include <boost/concept_check.hpp>

#include <boost/mpl/bool.hpp>

#include <boost/containers.hpp>
#include <boost/descriptors.hpp>

#include <boost/graphs/label.hpp>

// Define a common descriptor kind for these containers.
struct vertex_descriptor_kind { };

// Include type selectors for the different vertex store implementations
#include <boost/graphs/adjacency_list/vs/vector.hpp>
#include <boost/graphs/adjacency_list/vs/list.hpp>
#include <boost/graphs/adjacency_list/vs/set.hpp>
#include <boost/graphs/adjacency_list/vs/map.hpp>

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
// - Descriptor insert      (Store, Vertex)
// - Descriptor insert      (Store, Key, Vertex)
// - Result     try_insert  (Store, Vertex)
// - Result     try_insert  (Store, Key, Vertex)
// - Iterator   find        (Store, Label)
// - void       remove      (Store, Descriptor)
// - bool       empty       (Store)
// - Size       size        (Store)

namespace boost { namespace graphs {

// Specializations for label traits, etc.

// concept_map HasLabel<...> for most vertex types (vector, list, map).
//@{
template <typename Edges, typename Label>
struct label_traits<std::pair<Edges, Label>>
{ typedef Label label_type; };

template <typename Edges, typename Label>
Label& label(std::pair<Edges, Label>& v)
{ return v.second; }

template <typename Edges, typename Label>
Label const& label(std::pair<Edges, Label> const& v)
{ return v.second; }
//@}

/** @internal @name HasLabel<VertexSet>
 * This concept map removes the const-ness of the label when accessed as an
 * l-value. This permits the ability to modify parts of labels that are not
 * actually involved in a comparison. To be fair, it also allows the ability to
 * modify the parts that are so-involved so care must be taken. In general,
 * this does permit a more natural syntax for vertex-set graphs.
 */
//@{
template <typename Edges, typename Label>
struct label_traits<std::pair<Label const, Edges>>
{ typedef Label label_type; };

template <typename Edges, typename Label>
Label& label(std::pair<Label const, Edges>& v)
{ return const_cast<Label&>(v.first); }

template <typename Edges, typename Label>
Label const& label(std::pair<Label const, Edges> const& v)
{ return v.first; }
//@}

namespace adjacency_list {

/**
 * The incidence traits class provides a means of accessing type information
 * about the incidence store of a vertex.
 */
template <typename Vertex>
struct vertex_traits
{
    typedef typename label_traits<Vertex>::label_type vertex_label;
    typedef typename Vertex::vertex_edges vertex_edges;
};

// Specialization for any vertex type implemented as a pair - which most are
// implemented this way.
template <typename Edges, typename Label>
struct vertex_traits<std::pair<Edges, Label>>
{
    typedef Label vertex_label;
    typedef Edges vertex_edges;
};

// Specialization for vertex_set, which is actually a map.
template <typename Edges, typename Label>
struct vertex_traits<std::pair<Label const, Edges>>
{
    typedef Label vertex_label;
    typedef Edges vertex_edges;
};

/**
 * The vertex_store_traits relates associated types with a kind of vertex store.
 * Mostly, this is useful in cases where we have to get the vertex type from
 * simple and pair associative containers.
 */
template <typename Store>
struct vertex_store_traits
{
    typedef typename descriptor_traits<Store, vertex_descriptor_kind>::descriptor_type
        vertex_descriptor;
    typedef typename Store::size_type vertices_size_type;

    typedef typename Store::value_type vertex_type;
    typedef typename vertex_traits<vertex_type>::vertex_label vertex_label;
    typedef typename vertex_traits<vertex_type>::vertex_edges vertex_edges;
};

// Specialization for vertex map. This will not match for vertex set since the
// vertex type of a that selector is not a pair.
template <typename Key, typename Edges, typename Label, typename Compare, typename Alloc>
struct vertex_store_traits<std::map<Key, std::pair<Edges, Label>, Compare, Alloc>>
{
    typedef std::map<Key, std::pair<Edges, Label>, Compare, Alloc> base_type;

    typedef typename descriptor_traits<base_type, vertex_descriptor_kind>::descriptor_type
        vertex_descriptor;
    typedef typename base_type::size_type vertices_size_type;

    typedef typename base_type::mapped_type vertex_type;
    typedef typename vertex_traits<vertex_type>::vertex_label vertex_label;
    typedef typename vertex_traits<vertex_type>::vertex_edges vertex_edges;
};

namespace vs {

namespace detail {

    /** @internal @name Make Vertex
     * For some reason (possbily a GCC bug), we have to do a better job
     * specializing the make_vertex functions. No idea why... Either way, this
     * looks a bit hacky.
     */
    //@{
    template <typename Vertex, typename Alloc, typename Label>
    inline Vertex
    make_vertex(std::vector<Vertex, Alloc> const&, Label&& l)
    { return Vertex(typename vertex_traits<Vertex>::vertex_edges(), l); }

    template <typename Vertex, typename Alloc, typename Label>
    inline Vertex
    make_vertex(counted_list<Vertex, Alloc> const&, Label&& l)
    { return Vertex(typename vertex_traits<Vertex>::vertex_edges(), l); }

    // Overload for vertex_set
    template <typename Label, typename Edges, typename Compare, typename Alloc>
    inline std::pair<Label const, Edges>
    make_vertex(std::map<Label, Edges, Compare, Alloc> const&, Label const& l)
    { return std::make_pair(l, Edges()); }

    // Overload for vertex_map
    template <typename Key, typename Edges, typename Label, typename Compare, typename Alloc>
    inline std::pair<Edges, Label>
    make_vertex(std::map<Key, std::pair<Edges, Label>, Compare, Alloc> const&, Label&& l)
    { return std::make_pair(l, Edges()); }
    //@}

    /** @internal @name Get Vertex
     * Get the part of a stored vertex that represents just the vertex - the
     * label and edges. Generally, this is the same as dereferencing an,
     * iterator, but differs for graphs with mapped vertices.
     *
     * There may be some weirdness with the iterators and the container since
     * the container is passed as const in some cases, but the iterator is not
     * a const_iterator.
     */
    //@{
    // Overload for non-mapped vertices.
    template <typename Store>
    inline typename Store::value_type& get_vertex(Store&, typename Store::iterator i)
    { return *i; }

    template <typename Store>
    inline typename Store::value_type const& get_vertex(Store const&, typename Store::iterator i)
    { return *i; }

    // Overload(s) for vertex_map
    template <typename Key, typename Edges, typename Label, typename Compare, typename Alloc>
    inline std::pair<Edges, Label>&
    get_vertex(std::map<Key, std::pair<Edges, Label>, Compare, Alloc>&,
               typename std::map<Key, std::pair<Edges, Label>, Compare, Alloc>::iterator i)
    { return i->second; }

    template <typename Key, typename Edges, typename Label, typename Compare, typename Alloc>
    inline std::pair<Edges, Label> const&
    get_vertex(std::map<Key, std::pair<Edges, Label>, Compare, Alloc> const&,
               typename std::map<Key, std::pair<Edges, Label>, Compare, Alloc>::iterator i)
    { return i->second; }
    //@}


    // Iterate and compare for sequences.
    template <typename Store, typename Label>
    inline typename Store::iterator
    dispatch_find(Store& store, Label const& l, sequence_tag)
    {
        return std::find_if(
            store.begin(),
            store.end(),
            has_label_equal_to(l));
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
    void dispatch_erase(Store&, typename Store::iterator, vector_tag)
    { BOOST_CONCEPT_ASSERT((Integer<Store>)); }

    template <typename Store, typename Tag>
    void dispatch_erase(Store& store, typename Store::iterator i, Tag)
    { store.erase(i); }

} /* namespace detail */

/** @name Add Vertex
 * Add a vertex to the given store, returning an iterator to the new vertex
 * vertex. New vertices are created without an empty incidence list - they
 * have no neighbors.
 */
//@{
/** Insert a vertex into the container. */
template <typename Store>
inline typename vertex_store_traits<Store>::vertex_descriptor
insert(Store& store, typename vertex_store_traits<Store>::vertex_label&& l)
{
    typedef typename Store::iterator Iterator;
    Iterator i = container_insert(store, detail::make_vertex(store, l));
    return make_descriptor(store, i, vertex_descriptor_kind());
}

/** Insert a vertex into the container, mapped to the given key. */
template <typename Store>
inline typename vertex_store_traits<Store>::vertex_descriptor
insert(Store& store,
       typename Store::key_type const& k,
       typename vertex_store_traits<Store>::vertex_label&& l)
{
    // TODO: This is a bit hacky w.r.t. the generic make_vertex function
    // in that it doesn't call it - but it probably should. For some reason,
    // the make_vertex that I think should match, doesn't.
    typedef typename Store::iterator Iterator;
    typedef typename vertex_store_traits<Store>::vertex_type Vertex;
    typedef typename vertex_store_traits<Store>::vertex_edges Edges;
    Iterator i = container_insert(store, std::make_pair(k, Vertex(Edges(), l)));
    return make_descriptor(store, i, vertex_descriptor_kind());
}
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
inline typename vertex_store_traits<Store>::vertex_descriptor
find(Store& store, LabelOrKey const& lk)
{
    typename Store::iterator i = detail::dispatch_find(store, lk, container_category(store));
    return make_descriptor(store, i, vertex_descriptor_kind());
}

// Does not return a const iterator!
template <typename Store, typename LabelOrKey>
inline typename vertex_store_traits<Store>::vertex_descriptor
find(Store const& store, LabelOrKey const& lk)
{ return vs::find(const_cast<Store&>(store), lk); }
//@}

/**
 * @name Erase Vertex
 * Remove the vertex from the store. This will only remove the vertex from the
 * store, it does not remove any incident edges.
 */
//@{
template <typename Store>
inline void
erase(Store& store, typename vertex_store_traits<Store>::vertex_descriptor d)
{ detail::dispatch_erase(store, make_iterator(store, d), container_category(store)); }
//@}

/** @name Vertex Object */
//@{
template <typename Store>
inline typename vertex_store_traits<Store>::vertex_type&
vertex(Store& store, typename vertex_store_traits<Store>::vertex_descriptor v)
{ return detail::get_vertex(store, make_iterator(store, v)); }

template <typename Store>
inline typename vertex_store_traits<Store>::vertex_type const&
vertex(Store const& store, typename vertex_store_traits<Store>::vertex_descriptor v)
{ return detail::get_vertex(store, make_iterator(store, v)); }
//@}

/** @name Vertex Label
 *
 * @todo There's a problem with vertex_sets that causes the compiler to select
 * a non-const return type and then causes an error because we can't convert
 * the const label to the non-const label. One solution would be to provide an
 * internal overload that casted out the const - this isn't a terribly bad idea
 * since its easily possible that the only part of the label required to be
 * immutable is the part involved in the comparison and sorting.
 */
//@{
template <typename Store>
inline typename vertex_store_traits<Store>::vertex_label&
label(Store& store, typename vertex_store_traits<Store>::vertex_descriptor v)
{ return graphs::label(vertex(store, v)); }

template <typename Store>
inline typename vertex_store_traits<Store>::vertex_label const&
label(Store const& store, typename vertex_store_traits<Store>::vertex_descriptor v)
{ return graphs::label(vertex(store, v)); }
//@}

/** Return the number of elements in the vertex store. */
template <typename Store>
inline typename vertex_store_traits<Store>::vertices_size_type
size(Store const& store)
{ return store.size(); }

/** Return true if the store is empty. */
template <typename Store>
inline bool empty(Store const& store)
{ return store.empty(); }

/** Clear the vertex store. */
template <typename Store>
inline void clear(Store& store)
{ store.clear(); }

} /* namespace vs */

} } } /* namespace boost::graphs::adjacency_list */

#endif
