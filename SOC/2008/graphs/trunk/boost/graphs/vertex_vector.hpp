
#ifndef VERTEX_VECTOR_HPP
#define VERTEX_VECTOR_HPP

#include <vector>
#include <algorithm>

#include <boost/none.hpp>
#include <boost/descriptors.hpp>
#include <boost/graphs/utility.hpp>

#include "vertex_iterator.hpp"

// Forward declarations
template <typename, typename> struct vertices_vector;

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

    typedef std::vector<int, Alloc<int>> dummy;
    typedef typename descriptor_traits<dummy>::descriptor_type descriptor_type;

    // The store metafunction generates the type used to store vertices in
    // either a directed or undirected graph. This metafunction takes the
    // fully configured vertex type as a type parameter.
    template <typename Vertex>
    struct store
    {
        typedef vertices_vector<Vertex, Alloc<Vertex>> type;
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
class vertices_vector
{
public:
    typedef std::vector<Vertex, Allocator> store_type;
    typedef typename store_type::size_type size_type;
    typedef typename store_type::iterator iterator;
    typedef typename store_type::const_iterator const_iterator;

    typedef Vertex vertex_type;
    typedef typename Vertex::vertex_properties vertex_properties;

    typedef typename descriptor_traits<store_type>::descriptor_type vertex_descriptor;

    typedef indexed_vertex_iterator<store_type> vertex_iterator;
    typedef std::pair<vertex_iterator, vertex_iterator> vertex_range;

    // Constructors
    inline vertices_vector()
        : _verts()
    { }

    /** @name Add Vertex
     * Add a vertex to the store with the given properties (or none). Return
     * a descriptor to the vertex that was added to the vector.
     */
    //@{
    inline vertex_descriptor add()
    { return add(vertex_properties()); }

    inline vertex_descriptor add(vertex_properties const& vp)
    {
        iterator i = insert(_verts, vertex_type(vp));
        return make_descriptor(_verts, i);
    }
    //@}

    /**
     * Return a descriptor to the vertex with the given properties. If you
     * end up calling this function, you're probably using the wrong graph
     * type.
     */
    vertex_descriptor find(vertex_properties const& vp) const
    {
        iterator i = std::find_if(_verts.begin(), _verts.end(), find_properties(vp));
        return make_descriptor(_verts, i);
    }

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
    { return *make_iterator(_verts, v); }

    vertex_type const& vertex(vertex_descriptor v) const
    { return *make_iterator(_verts, v); }
    //@}

    /** @name Property Accessors */
    //@{
    vertex_properties& properties(vertex_descriptor v)
    { return vertex(v).properties(); }

    vertex_properties const& properties(vertex_descriptor v) const
    { return vertex(v).properties(); }
    //@}

private:
    mutable store_type _verts;
};

#endif
