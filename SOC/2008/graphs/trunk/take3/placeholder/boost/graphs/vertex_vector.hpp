
#ifndef VERTEX_VECTOR_HPP
#define VERTEX_VECTOR_HPP

#include <vector>
#include <algorithm>

#include "placeholder.hpp"
#include "descriptor.hpp"
#include "vertex_descriptor.hpp"
#include "vertex_iterator.hpp"

// Forward declarations
template <typename V, typename A> struct vertex_vector_impl;

/**
 * The vertex vector stores vertices in a vector, allowing for fast inserts
 * and iteration, but slow finds and removals.
 *
 * The Alloc parameter is a unary template class responsible for allocating
 * the stored vertices. We use a template rather than a type because the caller
 * does not know the actual types being allocated.
 *
 * @param Alloc A unary template class that will allocate stored vertices.
 */
template <template <typename> class Alloc = std::allocator>
struct vertex_vector
{
    typedef unused key_type;
    typedef basic_vertex_descriptor<std::size_t> descriptor_type;

    // Generate some dummy information that we can use to generate fake
    // decriptors and other types. These types will be the same size as the
    // real descriptor and store tyes later.
    typedef std::vector<int, Alloc<int> > dummy_vector;
    typedef descriptor<dummy_vector> dummy_descriptor;

    // The store metafunction generates the type used to store vertices in
    // either a directed or undirected graph. This metafunction takes the
    // fully configured vertex type as a type parameter.
    template <typename Vertex>
    struct store
    {
        typedef Vertex stored_vertex;
        typedef Alloc<stored_vertex> allocator;
        typedef vertex_vector_impl<stored_vertex, allocator> type;
    };
};

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
    typedef basic_vertex_descriptor<std::size_t> vertex_descriptor;

    typedef Vertex vertex_type;
    typedef typename Vertex::vertex_properties vertex_properties;
    typedef typename vertex_store::size_type size_type;
    typedef typename vertex_store::iterator iterator;
    typedef typename vertex_store::const_iterator const_iterator;

    typedef indexed_vertex_iterator<vertex_store> vertex_iterator;
    typedef std::pair<vertex_iterator, vertex_iterator> vertex_range;

    // Constructors
    vertex_vector_impl();

    /** @name Add Vertex */
    //@{
    vertex_descriptor add();
    vertex_descriptor add(vertex_properties const&);
    //@}

    /** Return the vertex with the given properties */
    vertex_descriptor find(vertex_properties const&) const;

    /** Rerturn the number of vertices in the vector. */
    size_type size() const
    { return _verts.size(); }

    /** @name Vertex Iterators */
    //@{
    vertex_iterator begin_vertices() const
    { return vertex_iterator(_verts, _verts.begin()); }

    vertex_iterator end_vertices() const
    { return vertex_iterator(_verts, _verts.end()); }

    vertex_range vertices() const
    { return std::make_pair(begin_vertices(), end_vertices()); }
    //@}

    /** @name Vertex Accessors */
    //@{
    vertex_type& vertex(vertex_descriptor v)
    { return _verts[v.get()]; }

    vertex_type const& vertex(vertex_descriptor v) const
    { return _verts[v.get()]; }
    //@{

    /** @name Property Accessors */
    //@{
    vertex_properties& properties(vertex_descriptor v)
    { return vertex(v).properties(); }

    vertex_properties const& properties(vertex_descriptor v) const
    { return vertex(v).properties(); }
    //@{

private:
    vertex_store _verts;
};

template <typename V, typename A>
vertex_vector_impl<V,A>::vertex_vector_impl()
    : _verts()
{ }

/**
 * Add a vertex to the store with no or default properties, returning the
 * the descriptor to the added vertex. Adding a vertex does not invalidate
 * any vertices or edges.
 *
 * @complexity O(~1)
 */
template <typename V, typename A>
typename vertex_vector_impl<V,A>::vertex_descriptor
vertex_vector_impl<V,A>::add()
{
    return add(vertex_properties());
}

/**
 * Add a vertex to the store with the given properties. Adding a vertex does
 * not invalidate any other descriptors.
 *
 * @complexity O(1)
 */
template <typename V, typename A>
typename vertex_vector_impl<V,A>::vertex_descriptor
vertex_vector_impl<V,A>::add(vertex_properties const& vp)
{
    // Just push the vertex to the back and return its index.
    _verts.push_back(vertex_type(vp));
    return _verts.size() - 1;
}

/**
 * Find the vertex with the given properties.
 *
 * @complexity O(V)
 */
template <typename V, typename A>
typename vertex_vector_impl<V,A>::vertex_descriptor
vertex_vector_impl<V,A>::find(vertex_properties const& vp) const
{
    return std::distance(_verts.begin(),
                         std::find(_verts.begin(), _verts.end(), vp));
}

#endif
