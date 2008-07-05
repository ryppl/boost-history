
#ifndef VERTEX_LIST_HPP
#define VERTEX_LIST_HPP

#include <list>

#include "descriptor.hpp"

// Forward declarations
template <typename V, template <typename> class A> class vertex_list_elem;
template <typename V, typename A> class vertex_list_impl;

/**
 * This metafunctiopn defines the basic elements of a vertex list.
 *
 * FIXME: We can probably rebuild the descriptor as an iterator into the
 * underlying store by defining it as a buffer the same size as a fake store
 * iterator and then re-casting the buffered data as an iterator whenever we
 * need it. That's a bit hacky, but it gets rid of redundant storage.
 */
template <template <typename> class Allocator>
struct basic_vertex_list
{
    typedef none key_type;
    typedef void* descriptor_type;

    template <typename Vertex>
    struct store
    {
        typedef vertex_list_elem<Vertex, Allocator> stored_vertex;
        typedef vertex_list_impl<stored_vertex, Allocator<stored_vertex> > type;
    };
};

/**
 * The default vertex list uses the standard allocator.
 */
struct vertex_list : basic_vertex_list<std::allocator> { };

/**
 * Pad the vertex type with an iterator back into the list. See above for a
 * way around this.
 */
template <typename Vertex, template <typename> class Alloc>
class vertex_list_elem
    : public Vertex
{
    typedef vertex_list_elem<Vertex, Alloc> this_type;
public:
    typedef typename std::list<this_type, Alloc<this_type> >::iterator iterator;

    inline vertex_list_elem(typename Vertex::vertex_properties const& vp)
        : Vertex(vp)
        , iter()
    { }

    iterator iter;
};

/**
 *
 * FIXME: Track the size of the list, so that num_vertices() is constant.
 */
template <typename Vertex, typename Allocator>
class vertex_list_impl
{
    typedef std::list<Vertex, Allocator> vertex_store;
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
    vertex_list_impl();

    // Add/remove vertices.
    vertex_descriptor add();
    vertex_descriptor add(vertex_properties const& vp);

    // Remove vertices.
    void remove(vertex_descriptor v);

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

#define BOOST_GRAPH_VL_PARAMS \
    typename V, typename A

/**
 * Construct an empty vertex list.
 */
template <BOOST_GRAPH_VL_PARAMS>
vertex_list_impl<V,A>::vertex_list_impl()
    : _verts()
{ }

/**
 * Add a vertex to the list with no or default properties.
 *
 * @complexity O(1)
 */
template <BOOST_GRAPH_VL_PARAMS>
typename vertex_list_impl<V,A>::vertex_descriptor
vertex_list_impl<V,A>::add()
{
    return add(vertex_properties());
}

/**
 * Add a vertex to the store with the given properties. If not specified, the
 * vertex is created with default properties. Note that vertices are not mapped
 * or ordered so multiple, equivalent vertices can be added to the graph.
 * This adds the vertex to the end of the list.
 *
 * @complexity O(1)
 */
template <BOOST_GRAPH_VL_PARAMS>
typename vertex_list_impl<V,A>::vertex_descriptor
vertex_list_impl<V,A>::add(vertex_properties const& vp)
{
    typename vertex_store::iterator i = _verts.insert(_verts.end(), vertex_type(vp));
    i->iter = i;
    return &(*i);
}

/**
 * Remove a vertex from the set. Removing a vertex will invalidate all vertex
 * descriptors and iterators to the removed vertex.
 *
 * @complexity O(1)
 */
template <BOOST_GRAPH_VL_PARAMS>
void
vertex_list_impl<V,A>::remove(vertex_descriptor v)
{
    vertex_type* vp = static_cast<vertex_type*>(v);
    _verts.erase(vp->iter);
}

/**
 * Return an iterator range over the vertices in this graph.
 */
template <BOOST_GRAPH_VL_PARAMS>
typename vertex_list_impl<V,A>::vertex_range
vertex_list_impl<V,A>::vertices() const
{
    return std::make_pair(begin_vertices(), end_vertices());
}

/**
 * Return an iterator to the first vertex in the list.
 */
template <BOOST_GRAPH_VL_PARAMS>
typename vertex_list_impl<V,A>::vertex_iterator
vertex_list_impl<V,A>::begin_vertices() const
{
    return _verts.begin();
}

/**
 * Return an iterator past the end of the vertices in the list.
 */
template <BOOST_GRAPH_VL_PARAMS>
typename vertex_list_impl<V,A>::vertex_iterator
vertex_list_impl<V,A>::end_vertices() const
{
    return _verts.end();
}

/**
 * Return the number of vertices in the store.
 */
template <BOOST_GRAPH_VL_PARAMS>
typename vertex_list_impl<V,A>::size_type
vertex_list_impl<V,A>::size() const
{
    return _verts.size();
}

/**
 * Get access to the given vertex.
 */
template <BOOST_GRAPH_VL_PARAMS>
typename vertex_list_impl<V,A>::vertex_type&
vertex_list_impl<V,A>::vertex(vertex_descriptor v)
{
    return *static_cast<vertex_type*>(v);
}

/**
 * Get access to the given vertex.
 */
template <BOOST_GRAPH_VL_PARAMS>
typename vertex_list_impl<V,A>::vertex_type const&
vertex_list_impl<V,A>::vertex(vertex_descriptor v) const
{
    return *static_cast<vertex_type*>(v);
}

/**
 * Get the properties ofthe given vertex.
 */
template <BOOST_GRAPH_VL_PARAMS>
typename vertex_list_impl<V,A>::vertex_properties&
vertex_list_impl<V,A>::properties(vertex_descriptor v)
{
    return vertex(v).properties();
}

#undef BOOST_GRAPH_VL_PARAMS

#if 0

/**
 * Return the number of vertices in the vertex store.
 *
 * @complexity O(V)
 *
 * @todo I'm not sure about the interface I'd like to build for list storage.
 * If we don't include a lot of splice-style operations, then it's not a big
 * deal to manage the size of this list internally.
 */
template <typename V, template <typename> class A>
typename vertex_list_impl<V,A>::vertices_size_type
vertex_list_impl<V,A>::num_vertices() const
{
    return _verts.size();
}

/**
 * Return the iterator range for the graph.
 */
template <typename V, template <typename> class A>
std::pair<
    typename vertex_list_impl<V,A>::vertex_iterator,
    typename vertex_list_impl<V,A>::vertex_iterator
>
vertex_list_impl<V,A>::vertices() const
{
    return std::make_pair(_verts.begin(), _verts.end());
}

/**
 * Return the iterator for the begining of the vertices.
 */
template <typename V, template <typename> class A>
typename vertex_list_impl<V,A>::vertex_iterator
vertex_list_impl<V,A>::begin_vertices() const
{
    return _verts.begin();
}

/**
 * Return the iterator for the begining of the vertices.
 */
template <typename V, template <typename> class A>
typename vertex_list_impl<V,A>::vertex_iterator
vertex_list_impl<V,A>::end_vertices() const
{
    return _verts.end();
}


/**
 * Access the properties ofthe given vertex.
 */
template <typename V, template <typename> class A>
typename vertex_list_impl<V,A>::vertex_properties const&
vertex_list_impl<V,A>::properties(vertex_descriptor v) const
{
    return *vertex(v);
}

#endif

#endif

