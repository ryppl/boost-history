
#ifndef VERTEX_MAP_HPP
#define VERTEX_MAP_HPP

#include <map>

// Forward declarations
template <typename, typename, typename, template <typename> class> class vertex_map_elem;
template <typename, typename, typename, typename> class vertex_map_impl;

/**
 * This metafunction defines the implementation requirements of a mapping of
 * vertices to the keys that describe them. This is function takes three
 * parameters: the type of key, a comparator template, and vertex allocator.
 *
 * The Compare parameter must be a unary template class rather than a fully
 * instantiated type. This makes it easier to pass arbitrary template functions
 * (e.g., less and greater) instead of instantiated types.
 *
 * The Alloc parameter is also a unary template class. In this case, the caller
 * does not know the final type of the allocator object. It us ultimately
 * instantiated as an allocator of key/vertex pairs.
 *
 * @param Key The key type of the vertex map can be any LessThanComparable type.
 * @param Compare A unary template class that implements a comparison of keys.
 * @param Alloc An allocator for key/value pairs of the underliyng map.
 */
template <
        typename Key,
        template <typename> class Compare,
        template <typename> class Alloc>
struct basic_vertex_map
{
    typedef Key key_type;
    typedef void* descriptor_type;

    template <typename Vertex>
    struct store
    {
        // Build the key comparator (note: independent of vertex!)
        typedef Compare<key_type> compare_type;

        // Build the stored vertex type.
        typedef vertex_map_elem<Vertex, Key, compare_type, Alloc> stored_vertex;

        // Build the allocator
        typedef Alloc<stored_vertex> allocator_type;

        // Build the underlying store
        typedef vertex_map_impl<stored_vertex, key_type, compare_type, allocator_type> type;
    };
};

/**
 * The most common vertex set allows parameterization over the comparator used
 * to sort vertices and uses the standard omnibus allocator.
 */
template <typename Key, template <typename> class Compare = std::less>
struct vertex_map : basic_vertex_map<Key, Compare, std::allocator> { };

/**
 * Extend the notion of a vertex for map storage so that we can store each
 * vertex's iterator with current vertex. This is used to provide constant
 * time access to the correct position in the underliying store.
 */
template <
    typename Vertex,
    typename Key,
    typename Compare,
    template <typename> class Alloc>
class vertex_map_elem
    : public Vertex
{
    typedef vertex_map_elem<Vertex, Key, Compare, Alloc> this_type;
public:
    typedef typename std::map<Key, this_type, Compare, Alloc<this_type> >::iterator iterator;

    inline vertex_map_elem(typename Vertex::vertex_properties const& vp)
        : Vertex(vp)
        , iter()
    { }

    iterator iter;
};

/**
 * Implementation of the vertex set. This requires that vertices (actually
 * their properties) are less-than comparable.
 *
 * @param Vertex The vertex type stored by the class.
 * @param Key The type of key mapping to vertices.
 * @param Compare An ordering over keys.
 * @param Alloc The allocator for Key/Vertex pairs.
 */
template <typename Vertex, typename Key, typename Compare, typename Allocator>
class vertex_map_impl
{
    typedef std::map<Key, Vertex, Compare, Allocator> vertex_store;
public:
    typedef void* vertex_descriptor;

    typedef Vertex vertex_type;
    typedef typename Vertex::vertex_properties vertex_properties;
    typedef typename vertex_store::size_type size_type;
    typedef typename vertex_store::iterator iterator;
    typedef typename vertex_store::const_iterator const_iterator;

    typedef Key key_type;


    typedef simple_vertex_iterator<vertex_store> vertex_iterator;
    typedef std::pair<vertex_iterator, vertex_iterator> vertex_range;

    // Constructors
    vertex_map_impl();

    vertex_descriptor add(key_type const& k, vertex_properties const& vp);
    vertex_descriptor find(key_type const& vp) const;
    void remove(vertex_descriptor);
    void remove(key_type const&);

    key_type const& key(vertex_descriptor) const;

    /** Return the number of vertices in the map. */
    inline size_type size() const
    { return _verts.size(); }

    /** @name Vertex Iteration */
    //@{
    inline vertex_iterator begin_vertices() const
    { return _verts.begin(); }

    inline vertex_iterator end_vertices() const
    { return _verts.end(); }

    vertex_range vertices() const
    { return std::make_pair(begin_vertices(), end_vertices()); }
    //@}

    /** @name Vertex Accessors */
    //@{
    vertex_type& vertex(vertex_descriptor v)
    { return *static_cast<vertex_type*>(v); }

    vertex_type const& vertex(vertex_descriptor v) const
    { return *static_cast<vertex_type*>(v); }
    //@}

    /** @name Property Accessors */
    //@{
    vertex_properties& properties(vertex_descriptor v)
    { return vertex(v).properties(); }

    vertex_properties const& properties(vertex_descriptor v) const
    { return vertex(v).properties(); }
    //@}

private:
    vertex_store _verts;
};

template <typename V, typename K, typename C, typename A>
vertex_map_impl<V,K,C,A>::vertex_map_impl()
    : _verts()
{ }

/**
 * Add the vertex with the given properties. If there is already a vertex with
 * these properties, then this returns the descriptor of the existing vertex
 * and does not insert a new vertex.
 *
 * @complexity O(lg(V))
 */
template <typename V, typename K, typename C, typename A>
typename vertex_map_impl<V,K,C,A>::vertex_descriptor
vertex_map_impl<V,K,C,A>::add(key_type const& k, vertex_properties const& vp)
{
    // Try to insert the vertex into the map.
    vertex_descriptor ret;
    std::pair<iterator, bool> ins = _verts.insert(std::make_pair(k, vertex_type(vp)));
    if(ins.second) {
        vertex_type& v = ins.first->second;
        v.iter = ins.first;
        ret = &v;
    }
    else {
        ret = &ins.first->second;
    }
    return ret;
}

/**
 * Find the vertex in the map with the given key.
 *
 * @complexity O(log(V))
 */
template <typename V, typename K, typename C, typename A>
typename vertex_map_impl<V,K,C,A>::vertex_descriptor
vertex_map_impl<V,K,C,A>::find(key_type const& k) const
{
    return &const_cast<vertex_type&>(_verts.find(k)->second);
}

/**
 * Remove a vertex from the map. Removing a vertex will invalidate all
 * descriptors and iterators to the vertex being removed.
 *
 * @complexity O(log(V))
 */
template <typename V, typename K, typename C, typename A>
void
vertex_map_impl<V,K,C,A>::remove(vertex_descriptor v)
{
    _verts.erase(vertex(v).iter);
}

/**
 * Remove the vertex with the given key from the map. This operation finds
 * the vertex before removing it.
 *
 * @complexity O(log(V))
 */
template <typename V, typename K, typename C, typename A>
void
vertex_map_impl<V,K,C,A>::remove(key_type const& k)
{
    remove(find(k));
}

/**
 * Return the key for the given vertex.
 */
template <typename V, typename K, typename C, typename A>
typename vertex_map_impl<V,K,C,A>::key_type const&
vertex_map_impl<V,K,C,A>::key(vertex_descriptor v) const
{
    return vertex(v).iter->first;
}

#endif
