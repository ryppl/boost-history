
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
    typedef unused key_type;
    typedef void* descriptor_type;

    template <typename Vertex>
    struct store
    {
        typedef vertex_list_elem<Vertex, Allocator> stored_vertex;
        typedef Allocator<stored_vertex> allocator_type;
        typedef vertex_list_impl<stored_vertex, allocator_type > type;
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
    vertex_descriptor find(vertex_properties const& vp);
    void remove(vertex_descriptor v);
    void remove(vertex_properties const& vp);

    /** Return the number of vertices in the set. */
    size_type size() const
    { return _size; }

    /** @name Vertex Iterators */
    //@{
    vertex_iterator begin_vertices() const
    { return _verts.begin(); }

    vertex_iterator end_vertices() const
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

    /** @name Vertex Properties */
    //@{
    vertex_properties& properties(vertex_descriptor v)
    { return vertex(v).properties(); }

    vertex_properties const& properties(vertex_descriptor v) const
    { return vertex(v).properties(); }
    //@}

private:
    vertex_store _verts;
    size_type _size;
};

#define BOOST_GRAPH_VL_PARAMS \
    typename V, typename A

/**
 * Construct an empty vertex list.
 */
template <BOOST_GRAPH_VL_PARAMS>
vertex_list_impl<V,A>::vertex_list_impl()
    : _verts()
    , _size(0)
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
    ++_size;
    iterator i = _verts.insert(_verts.end(), vertex_type(vp));
    i->iter = i;
    return &(*i);
}

/**
 * Find the vertex with the given properties. If there are multiple vertices
 * with the given properties, only the first is returned.
 *
 * @complexity O(V)
 * @requires EqualityComparable<VertexProps>
 */
template <typename V, typename A>
typename vertex_list_impl<V,A>::vertex_descriptor
vertex_list_impl<V,A>::find(vertex_properties const& vp)
{
    return &const_cast<vertex_type&>(*std::find(_verts.begin(), _verts.end(), vp));
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
    --_size;
    vertex_type* vp = static_cast<vertex_type*>(v);
    _verts.erase(vp->iter);
}

/**
 * Remove the vertex with the given properties from the list. This searches
 * the list for the first element with the given vertex and erases it. If there
 * are multiple elements with these properties, only the first is removed.
 *
 * @complexity O(V)
 */
template <typename V, typename A>
void
vertex_list_impl<V,A>::remove(vertex_properties const& vp)
{
    remove(find(vp));
}

#undef BOOST_GRAPH_VL_PARAMS

#endif

