
#ifndef BOOST_GRAPHS_ADJACENCY_LIST_VERTEX_MAP_HPP
#define BOOST_GRAPHS_ADJACENCY_LIST_VERTEX_MAP_HPP

#include <map>

#include <boost/graphs/adjacency_list/descriptor.hpp>
#include <boost/graphs/adjacency_list/vs/mapped_vertex_iterator.hpp>

namespace boost {
namespace graphs {
namespace adj_list {

// Forward declarations
template <typename V, typename K, template <typename> class C, template <typename> class A> class vertex_map_elem;
template <typename V, typename K, typename C, typename A> class vertex_map_impl;

template <typename Key, template <typename> class Compare, template <typename> class Allocator>
struct basic_vertex_map
{
    typedef basic_vertex_descriptor<void*> descriptor_type;

    template <typename Vertex>
    struct store
    {
        typedef vertex_map_elem<Vertex, Key, Compare, Allocator> stored_vertex;
        typedef std::pair<const Key, stored_vertex> stored_value;
        typedef vertex_map_impl<stored_vertex, Key, Compare<Key>, Allocator<stored_value> > type;
    };
};

template <typename Key, template <typename> class Compare = std::less>
struct vertex_map : basic_vertex_map<Key, Compare, std::allocator> { };

// Extend the notion of a vertex for set storage so that we can store each
// vertex's iterator with current vertex. This is used to provide constant
// time access to the correct position in the underliying store.
template <
        typename Vertex,
        typename Key,
        template <typename> class Compare,
        template <typename> class Alloc
    >
class vertex_map_elem
    : public Vertex
{
    typedef vertex_map_elem<Vertex, Key, Compare, Alloc> this_type;
public:
    typedef typename std::map<
            Key, this_type, Compare<Key>, Alloc< std::pair<Key, this_type> >
        >::iterator iterator;

    inline vertex_map_elem(typename Vertex::vertex_properties const& vp)
        : Vertex(vp)
        , iter()
    { }

    iterator iter;
};

/**
 * The vertex_map_impl provides a list-based implementation of vertex storage
 * for an adjacency list. List-based storage is best for graphs with
 * unidentified vertices and requirements for fast vertex addition and deletion.
 *
 * This class requires that the graph's vertex properties be less than
 * comparable since the ordering of vertices in the set is based on that
 * implementation of the ordering. Note that the vertex type provides a built
 * in ordering using operator<() that delegates the call to the properties.
 *
 * Adding vertices to a list does not invalidate any vertex or edge descriptors.
 * Removing vertices will invalidate descriptors referencing the removed
 * vertex. All insertions and removals occur in constant time. However, getting
 * the number of vertices is linear.
 *
 * @require LessThanComparable<Vertex::properties_type>
 */
template <typename Vertex, typename Key, typename Compare, typename Allocator>
class vertex_map_impl
{
    typedef std::map<Key, Vertex, Compare, Allocator> vertex_store;
public:
    typedef Key key_type;
    typedef Vertex vertex_type;
    typedef typename Vertex::vertex_properties vertex_properties;
    typedef typename Vertex::vertex_descriptor vertex_descriptor;
    typedef typename vertex_store::size_type vertices_size_type;
    typedef mapped_vertex_iterator<vertex_store> vertex_iterator;

    // Constructors
    vertex_map_impl();

    // Add or insert a vertex.
    vertex_descriptor add_vertex(key_type const& k, vertex_properties const& vp);
    std::pair<vertex_descriptor, bool> insert_vertex(key_type const& k, vertex_properties const& vp);

    // Remove a vertex.
    void remove_vertex(vertex_descriptor v);

    // Find a vertex
    vertex_descriptor find_vertex(key_type const& k) const;

    // Number of vertices.
    vertices_size_type num_vertices() const;

    // Vertex iteration.
    std::pair<vertex_iterator, vertex_iterator> vertices() const;
    vertex_iterator begin_vertices() const;
    vertex_iterator end_vertices() const;

    // Vertex accessors.
    vertex_type& vertex(vertex_descriptor v);
    vertex_type const& vertex(vertex_descriptor v) const;
    vertex_descriptor descriptor(vertex_type const& v) const;
    key_type const& key(vertex_descriptor v) const;

    // Property accessors.
    vertex_properties& properties(vertex_descriptor v);
    vertex_properties const& properties(vertex_descriptor v) const;


private:
    vertex_store _verts;
};

#define BOOST_GRAPHS_VM_PARAMS \
    typename V, typename K, typename C, typename A

template <BOOST_GRAPHS_VM_PARAMS>
vertex_map_impl<V,K,C,A>::vertex_map_impl()
    : _verts()
{ }

#undef BOOST_GRAPHS_VM_PARAMS

#if 0

/**
 * Add a vertex to the store with the key and properties. If the key is mapped
 * to a vertex, do nothing. Return a descriptor to the existing or new vertex.
 *
 * @complexity O(log(V))
 */
template <typename V, typename K, template <typename> class C, template <typename> class A>
typename vertex_map_impl<V,K,C,A>::vertex_descriptor
vertex_map_impl<V,K,C,A>::add_vertex(const K& k, vertex_properties const& vp)
{
    return insert_vertex(k, vp).first;
}

/**
 * Add a vertex to the store with the given properties. If not specified, the
 * vertex is created with default properties and return a descriptor to the
 * inserted vertex. If the vertex exists, the second element of the returned
 * pair is false.
 *
 * @complexity O(log(V))
 */
template <typename V, typename K, template <typename> class C, template <typename> class A>
std::pair<typename vertex_map_impl<V,K,C,A>::vertex_descriptor, bool>
vertex_map_impl<V,K,C,A>::insert_vertex(key_type const& k, vertex_properties const& vp)
{
    std::pair<vertex_descriptor, bool> ret;
    std::pair<typename vertex_store::iterator, bool> ins =
            _verts.insert(make_pair(k, vertex_type(vp)));
    if(ins.second) {
        // Yikes... so dirty. Normally, we can't modify an object via its
        // iterator since that would indicate a change to the key. However,
        // the key is based on the properties of the vertex, not the part of
        // the object that we're going to change.
        vertex_type& v = ins.first->second;
        v.iter = ins.first;
        ret.first = &v;
    }
    else {
        ret.first = &(ins.first->second);
    }
    ret.second = ins.second;

    return ret;
}

/**
 * Find a vertex given a key. If the key does not exist, return an invalid
 * descriptor.
 */
template <typename V, typename K, template <typename> class C, template <typename> class A>
typename vertex_map_impl<V,K,C,A>::vertex_descriptor
vertex_map_impl<V,K,C,A>::find_vertex(key_type const& k) const
{
    vertex_descriptor ret;
    typename vertex_store::const_iterator iter = _verts.find(k);
    if(iter != _verts.end()) {
        // Because the function is const, the resulting vertex should also be
        // const. Unfortunately, this isn't really going to work for me.
        vertex_type& v = const_cast<vertex_type&>(iter->second);
        ret = &v;
    }
    return ret;
}

/**
 * Remove a vertex from the store.
 *
 * Removing a vertex will invalidate all vertex and edge descriptors.
 *
 * @complexity O(|V|)
 */
template <typename V, typename K, template <typename> class C, template <typename> class A>
void
vertex_map_impl<V,K,C,A>::remove_vertex(vertex_descriptor v)
{
    vertex_type* vp = static_cast<vertex_type*>(v);
    _verts.erase(vp->iter);
}

/**
 * Return the number of vertices in the vertex store.
 *
 * @complexity O(V)
 *
 * @todo I'm not sure about the interface I'd like to build for list storage.
 * If we don't include a lot of splice-style operations, then it's not a big
 * deal to manage the size of this list internally.
 */
template <typename V, typename K, template <typename> class C, template <typename> class A>
typename vertex_map_impl<V,K,C,A>::vertices_size_type
vertex_map_impl<V,K,C,A>::num_vertices() const
{
    return _verts.size();
}

/**
 * Return the iterator range for the graph.
 */
template <typename V, typename K, template <typename> class C, template <typename> class A>
std::pair<
    typename vertex_map_impl<V,K,C,A>::vertex_iterator,
    typename vertex_map_impl<V,K,C,A>::vertex_iterator
>
vertex_map_impl<V,K,C,A>::vertices() const
{
    return std::make_pair(_verts.begin(), _verts.end());
}

/**
 * Get a vertex iterator to the beginning iterator of the vertices.
 */
template <typename V, typename K, template <typename> class C, template <typename> class A>
typename vertex_map_impl<V,K,C,A>::vertex_iterator
vertex_map_impl<V,K,C,A>::begin_vertices() const
{ return _verts.begin(); }

/**
 * Get a vertex iterator to an iterator past the end of the vertices.
 */
template <typename V, typename K, template <typename> class C, template <typename> class A>
typename vertex_map_impl<V,K,C,A>::vertex_iterator
vertex_map_impl<V,K,C,A>::end_vertices() const
{ return _verts.end(); }

/**
 * Get access to the given vertex.
 */
template <typename V, typename K, template <typename> class C, template <typename> class A>
typename vertex_map_impl<V,K,C,A>::vertex_type&
vertex_map_impl<V,K,C,A>::vertex(vertex_descriptor v)
{
    return *static_cast<vertex_type*>(v.desc);
}

/**
 * Get access to the given vertex.
 */
template <typename V, typename K, template <typename> class C, template <typename> class A>
typename vertex_map_impl<V,K,C,A>::vertex_type const&
vertex_map_impl<V,K,C,A>::vertex(vertex_descriptor v) const
{
    return *static_cast<vertex_type*>(v.desc);
}

/**
 * Return a descriptor for the given vertex.
 */
template <typename V, typename K, template <typename> class C, template <typename> class A>
typename vertex_map_impl<V,K,C,A>::vertex_descriptor
vertex_map_impl<V,K,C,A>::descriptor(vertex_type const& v) const
{
    return &const_cast<vertex_type&>(v);
}

/**
 * Get the key of a vertex descriptor.
 */
template <typename V, typename K, template <typename> class C, template <typename> class A>
typename vertex_map_impl<V,K,C,A>::key_type const&
vertex_map_impl<V,K,C,A>::key(vertex_descriptor v) const
{
    vertex_type& vert = *static_cast<vertex_type*>(v.desc);
    return vert.iter->first;
}


/**
 * Get the properties of the given vertex.
 */
template <typename V, typename K, template <typename> class C, template <typename> class A>
typename vertex_map_impl<V,K,C,A>::vertex_properties&
vertex_map_impl<V,K,C,A>::properties(vertex_descriptor v)
{
    return *vertex(v);
}

/**
 * Get the properties of the given vertex.
 */
template <typename V, typename K, template <typename> class C, template <typename> class A>
typename vertex_map_impl<V,K,C,A>::vertex_properties const&
vertex_map_impl<V,K,C,A>::properties(vertex_descriptor v) const
{
    return *vertex(v);
}

#endif

} /* namespace adj_list */
} /* namesapce graphs */
} /* namespace boost */

#endif
