
#ifndef VERTEX_SET_HPP
#define VERTEX_SET_HPP

#include <set>

// Forward declarations
template <typename, template <typename> class, template <typename> class> class vertex_set_elem;
template <typename, typename, typename> class vertex_set_impl;

/**
 * This metafunction defines the implementation requirements of a set of
 * verties.
 */
template <template <typename> class Compare, template <typename> class Alloc>
struct basic_vertex_set
{
    typedef none key_type;
    typedef void* descriptor_type;

    template <typename Vertex>
    struct store
    {
        typedef vertex_set_elem<Vertex, Compare, Alloc> stored_vertex;
        typedef Compare<stored_vertex> comp_type;
        typedef Alloc<stored_vertex> alloc_type;
        typedef vertex_set_impl<stored_vertex, comp_type, alloc_type> type;
    };
};

/**
 * The most common vertex set allows parameterization over the comparator used
 * to sort vertices and uses the standard omnibus allocator.
 */
template <template <typename> class Compare = std::less>
struct vertex_set : basic_vertex_set<Compare, std::allocator> { };

/**
 * Extend the notion of a vertex for set storage so that we can store each
 * vertex's iterator with current vertex. This is used to provide constant
 * time access to the correct position in the underliying store.
 */
template <typename Vertex,
    template <typename> class Compare,
    template <typename> class Alloc>
class vertex_set_elem
    : public Vertex
{
    typedef vertex_set_elem<Vertex, Compare, Alloc> this_type;
public:
    typedef typename std::set<
            this_type, Compare<this_type>, Alloc<this_type>
        >::iterator iterator;

    inline vertex_set_elem(typename Vertex::vertex_properties const& vp)
        : Vertex(vp)
        , iter()
    { }

    inline bool operator<(this_type const& x) const
    { return Vertex::operator<(static_cast<Vertex const&>(x)); }

    iterator iter;
};

/**
 * Implementation of the vertex set. This requires that vertices (actually
 * their properties) are less-than comparable.
 */
template <typename Vertex, typename Compare, typename Allocator>
class vertex_set_impl
{
    typedef std::set<Vertex, Compare, Allocator> vertex_store;
public:
    typedef void* vertex_descriptor;

    typedef Vertex vertex_type;
    typedef typename Vertex::vertex_properties vertex_properties;
    typedef typename vertex_store::size_type size_type;
    typedef typename vertex_store::iterator iterator;
    typedef typename vertex_store::const_iterator const_iterator;


    typedef simple_vertex_iterator<vertex_store> vertex_iterator;
    typedef std::pair<vertex_iterator, vertex_iterator> vertex_range;

    // Constructors
    vertex_set_impl();

    // Add vertices. Note that you can't add without properties.
    vertex_descriptor add(vertex_properties const& vp);

    // Remove vertices.
    void remove(vertex_descriptor v);

    // Find a vertex based on its properties.
    vertex_descriptor find(vertex_properties const& vp) const;

    // Number of vertices.
    size_type size() const;

    // Vertex iteration.
    vertex_range vertices() const;
    vertex_iterator begin_vertices() const;
    vertex_iterator end_vertices() const;

    // Vertex accessors.
    vertex_type& vertex(vertex_descriptor);
    vertex_type const& vertex(vertex_descriptor) const;
    vertex_properties& properties(vertex_descriptor);
    vertex_properties const& properties(vertex_descriptor) const;

private:
    vertex_store _verts;
};


#define BOOST_GRAPH_VS_PARAMS \
    typename V, typename C, typename A

template <BOOST_GRAPH_VS_PARAMS>
vertex_set_impl<V,C,A>::vertex_set_impl()
    : _verts()
{ }

/**
 * Add the vertex with the given properties. If there is already a vertex with
 * these properties, then this returns the descriptor of the existing vertex
 * and does not insert a new vertex.
 *
 * @complexity O(lg(V))
 */
template <BOOST_GRAPH_VS_PARAMS>
typename vertex_set_impl<V,C,A>::vertex_descriptor
vertex_set_impl<V,C,A>::add(vertex_properties const& vp)
{
    // Try to insert the vertex.
    vertex_descriptor ret;
    std::pair<iterator, bool> ins = _verts.insert(vertex_type(vp));
    if(ins.second) {
        vertex_type& v = const_cast<vertex_type&>(*(ins.first));
        v.iter = ins.first;
        ret = &v;
    }
    else {
        ret = &const_cast<vertex_type&>(*ins.first);
    }
    return ret;
}

/**
 * Remove a vertex from the set. Removing a vertex will invalidate all
 * descriptors and iterators to the vertex being removed.
 *
 * @complexity O(log(V))
 */
template <BOOST_GRAPH_VS_PARAMS>
void
vertex_set_impl<V,C,A>::remove(vertex_descriptor v)
{
    vertex_type* vp = static_cast<vertex_type*>(v);
    _verts.erase(vp->iter);
}

/**
 * Return an iterator range over the vertices in this graph.
 */
template <BOOST_GRAPH_VS_PARAMS>
typename vertex_set_impl<V,C,A>::vertex_range
vertex_set_impl<V,C,A>::vertices() const
{
    return std::make_pair(begin_vertices(), end_vertices());
}

/**
 * Return an iterator to the first vertex in the set.
 */
template <BOOST_GRAPH_VS_PARAMS>
typename vertex_set_impl<V,C,A>::vertex_iterator
vertex_set_impl<V,C,A>::begin_vertices() const
{
    return _verts.begin();
}

/**
 * Return an iterator past the end of the vertices in the set.
 */
template <BOOST_GRAPH_VS_PARAMS>
typename vertex_set_impl<V,C,A>::vertex_iterator
vertex_set_impl<V,C,A>::end_vertices() const
{
    return _verts.end();
}

/**
 * Return the number of vertices in this vertex set.
 */
template <BOOST_GRAPH_VS_PARAMS>
typename vertex_set_impl<V,C,A>::size_type
vertex_set_impl<V,C,A>::size() const
{
    return _verts.size();
}

/**
 * Get access to the given vertex.
 */
template <BOOST_GRAPH_VS_PARAMS>
typename vertex_set_impl<V,C,A>::vertex_type&
vertex_set_impl<V,C,A>::vertex(vertex_descriptor v)
{
    return *static_cast<vertex_type*>(v);
}

/**
 * Get access to the given vertex.
 */
template <BOOST_GRAPH_VS_PARAMS>
typename vertex_set_impl<V,C,A>::vertex_type const&
vertex_set_impl<V,C,A>::vertex(vertex_descriptor v) const
{
    return *static_cast<vertex_type*>(v);
}

/**
 * Get the properties of the given vertex.
 */
template <BOOST_GRAPH_VS_PARAMS>
typename vertex_set_impl<V,C,A>::vertex_properties&
vertex_set_impl<V,C,A>::properties(vertex_descriptor v)
{
    return vertex(v).properties();
}

#undef BOOST_GRAPH_VS_PARAMS

#if 0

/**
 * Add a vertex to the store with the given properties. If not specified, the
 * vertex is created with default properties. Note that vertices are not mapped
 * or ordered so multiple, equivalent vertices can be added to the graph.
 *
 * @complexity O(log(V))
 */
template <BOOST_GRAPH_VS_PARAMS>
typename vertex_set_impl<V,C,A>::vertex_descriptor
vertex_set_impl<V,C,A>::add_vertex(vertex_properties const& vp)
{
    return insert_vertex(vp).first;
}

/**
 * Add a vertex to the store with the given properties. If not specified, the
 * vertex is created with default properties and return a descriptor to the
 * inserted vertex. If the vertex exists, the second element of the returned
 * pair is false.
 *
 * @complexity O(log(V))
 */
template <BOOST_GRAPH_VS_PARAMS>
std::pair<typename vertex_set_impl<V,C,A>::vertex_descriptor, bool>
vertex_set_impl<V,C,A>::insert_vertex(vertex_properties const& vp)
{
    std::pair<vertex_descriptor, bool> ret;
    std::pair<typename vertex_store::iterator, bool> ins =
            _verts.insert(vertex_type(vp));
    if(ins.second) {
        // Yikes... so dirty. Normally, we can't modify an object via its
        // iterator since that would indicate a change to the key. However,
        // the key is based on the properties of the vertex, not the part of
        // the object that we're going to change.
        vertex_type& v = const_cast<vertex_type&>(*(ins.first));
        v.iter = ins.first;
        ret.first = &v;
    }
    else {
        ret.first = &const_cast<vertex_type&>(*ins.first);
    }
    ret.second = ins.second;

    return ret;
}

/**
 * Remove the vertex identified by the given properties from the store.
 *
 * @complexity O(log(V))
 */
template <BOOST_GRAPH_VS_PARAMS>
void
vertex_set_impl<V,C,A>::remove_vertex(vertex_properties const& vp)
{
    remove_vertex(find(vp));
}

/**
 * Find a vertex by its properties.
 *
 * @complexity O(log(V))
 */
template <BOOST_GRAPH_VS_PARAMS>
typename vertex_set_impl<V,C,A>::vertex_descriptor
vertex_set_impl<V,C,A>::find_vertex(vertex_properties const& vp) const
{
    // This is a little gross... We have to tempoararily construct an empty
    // vertex with the given properties in order for the find operations to
    // work.
    vertex_descriptor ret;
    typename vertex_store::iterator iter = _verts.find(vertex_type(vp));
    if(iter != _verts.end()) {
        ret = &const_cast<vertex_type&>(*iter);
    }
    return ret;
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
template <BOOST_GRAPH_VS_PARAMS>
typename vertex_set_impl<V,C,A>::vertices_size_type
vertex_set_impl<V,C,A>::num_vertices() const
{
    return _verts.size();
}

/**
 * Return the iterator range for the graph.
 */
template <BOOST_GRAPH_VS_PARAMS>
std::pair<
    typename vertex_set_impl<V,C,A>::vertex_iterator,
    typename vertex_set_impl<V,C,A>::vertex_iterator
>
vertex_set_impl<V,C,A>::vertices() const
{
    return std::make_pair(_verts.begin(), _verts.end());
}

/**
 * Get a vertex iterator to the beginning iterator of the vertices.
 */
template <BOOST_GRAPH_VS_PARAMS>
typename vertex_set_impl<V,C,A>::vertex_iterator
vertex_set_impl<V,C,A>::begin_vertices() const
{
    return _verts.begin();
}

/**
 * Get a vertex iterator to an iterator past the end of the vertices.
 */
template <BOOST_GRAPH_VS_PARAMS>
typename vertex_set_impl<V,C,A>::vertex_iterator
vertex_set_impl<V,C,A>::end_vertices() const
{
    return _verts.end();
}

/**
 * Access the properties of the given vertex.
 */
template <BOOST_GRAPH_VS_PARAMS>
typename vertex_set_impl<V,C,A>::vertex_properties const&
vertex_set_impl<V,C,A>::properties(vertex_descriptor v) const
{
    return *vertex(v);
}

#endif

#endif
