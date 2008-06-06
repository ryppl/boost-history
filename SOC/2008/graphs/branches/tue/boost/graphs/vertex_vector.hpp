
#ifndef VERTEX_VECTOR_HPP
#define VERTEX_VECTOR_HPP

#include <vector>

#include "indexed_vertex_iterator.hpp"

// Forward declarations
template <typename V, typename A> struct vertex_vector_impl;

template <template <typename> class Allocator>
struct basic_vertex_vector
{
    typedef void* key_type;
    typedef std::size_t descriptor_type;

    template <typename Vertex>
    struct store
    {
        typedef vertex_vector_impl<Vertex, Allocator<Vertex> > type;
    };
};

struct vertex_vector : basic_vertex_vector<std::allocator> { };


/**
 * The vertex_vector template implements veretex storage for adjacency lists
 * as a vector. This essentially provides a heavily constrained interface
 * to the underlying vector. Note that many operations normally supported by
 * a vector are not allowed with graphs.
 *
 * Adding vertices does not invalidate descriptors, but may result in the
 * reallocation of the underlying store. However, removing vertices could
 * corrupt the entire graph (since indices are adjusted). As a result, this
 * store type does not provide remove operations.
 */
template <typename Vertex, typename Allocator>
class vertex_vector_impl
{
    typedef std::vector<Vertex, Allocator> vertex_store;
public:
    typedef std::size_t vertex_descriptor;

    typedef Vertex vertex_type;
    typedef typename Vertex::vertex_properties vertex_properties;
    typedef typename vertex_store::size_type size_type;
    typedef indexed_vertex_iterator<vertex_store> iterator;
    typedef std::pair<iterator, iterator> range;

    // Constructors
    vertex_vector_impl();

    // Add/remove vertex.
    vertex_descriptor add();
    vertex_descriptor add(vertex_properties const& vp);

    // Number of vertices.
    size_type size() const;

    // Vertex iteration.
    range vertices() const;
    iterator begin() const;
    iterator end() const;

    // Vertex/vertex property accessors.
    vertex_type& vertex(vertex_descriptor v);
    vertex_type const& vertex(vertex_descriptor v) const;
    vertex_properties& properties(vertex_descriptor);
    vertex_properties const& properties(vertex_descriptor) const;

private:
    vertex_store _verts;
};

#define BOOST_GRAPHS_VV_PARAMS \
    typename V, typename A

template <BOOST_GRAPHS_VV_PARAMS>
vertex_vector_impl<V,A>::vertex_vector_impl()
    : _verts()
{ }

/**
 * Add a vertex to the store with no or default properties, returning the
 * the descriptor to the added vertex. Adding a vertex does not invalidate
 * any vertices or edges.
 *
 * @complexity O(1) amortized
 */
template <BOOST_GRAPHS_VV_PARAMS>
typename vertex_vector_impl<V,A>::vertex_descriptor
vertex_vector_impl<V,A>::add()
{
    return add(vertex_properties());
}

/**
 * Add a vertex to the store with the given properties. Adding a vertex does
 * not invalidate any other descriptors.
 *
 * @complexity O(1) amortized
 */
template <BOOST_GRAPHS_VV_PARAMS>
typename vertex_vector_impl<V,A>::vertex_descriptor
vertex_vector_impl<V,A>::add(vertex_properties const& vp)
{
    // Just push the vertex to the back and return its index.
    _verts.push_back(vertex_type(vp));
    return _verts.size() - 1;
}

/**
 * Return the number of vertices in the store.
 */
template <BOOST_GRAPHS_VV_PARAMS>
typename vertex_vector_impl<V,A>::size_type
vertex_vector_impl<V,A>::size() const
{
    return _verts.size();
}

/**
 * Get access to the underlying vertex.
 */
template <BOOST_GRAPHS_VV_PARAMS>
typename vertex_vector_impl<V,A>::vertex_type&
vertex_vector_impl<V,A>::vertex(vertex_descriptor v)
{
    return _verts[v];
}

/**
 * Get access to the underlying vertex.
 */
template <BOOST_GRAPHS_VV_PARAMS>
typename vertex_vector_impl<V,A>::vertex_type const&
vertex_vector_impl<V,A>::vertex(vertex_descriptor v) const
{
    return _verts[v];
}

#undef BOOST_GRAPHS_VV_PARAMS

#if 0

/**
 * Return the number of vertices in the vertex store.
 *
 * @complexity constant
 */
template <typename V, template <typename> class A>
typename vertex_vector_impl<V,A>::vertices_size_type
vertex_vector_impl<V,A>::num_vertices() const
{
    return _verts.size();
}

/**
 * Return the iterator range for the graph.
 */
template <typename V, template <typename> class A>
std::pair<
    typename vertex_vector_impl<V,A>::vertex_iterator,
    typename vertex_vector_impl<V,A>::vertex_iterator
>
vertex_vector_impl<V,A>::vertices() const
{
    return std::make_pair(begin_vertices(), end_vertices());
}

/**
 * Get an iterator to the beginning of the vertices.
 */
template <typename V, template <typename> class A>
typename vertex_vector_impl<V,A>::vertex_iterator
vertex_vector_impl<V,A>::begin_vertices() const
{
    return vertex_iterator(_verts, _verts.begin());
}

/**
 * Get an iterator past the end of the vertices.
 */
template <typename V, template <typename> class A>
typename vertex_vector_impl<V,A>::vertex_iterator
vertex_vector_impl<V,A>::end_vertices() const
{
    return vertex_iterator(_verts, _verts.end());
}


/**
 * Get access to the properties of the given vertex.
 */
template <typename V, template <typename> class A>
typename vertex_vector_impl<V,A>::vertex_properties&
vertex_vector_impl<V,A>::properties(vertex_descriptor v)
{
    return *vertex(v);
}

/**
 * Get access to the properties of the given vertex.
 */
template <typename V, template <typename> class A>
typename vertex_vector_impl<V,A>::vertex_properties const&
vertex_vector_impl<V,A>::properties(vertex_descriptor v) const
{
    return *vertex(v);
}

#endif

#endif
