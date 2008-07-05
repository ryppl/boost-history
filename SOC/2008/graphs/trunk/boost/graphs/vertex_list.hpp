
#ifndef VERTEX_LIST_HPP
#define VERTEX_LIST_HPP

#include <list>

#include <boost/descriptors.hpp>

#include "vertex_iterator.hpp"

// Forward declarations
template <typename V, template <typename> class A> class vertex_list_elem;
template <typename V, typename A> class vertex_list_impl;

/**
 * This metafunction defines the basic elements of a vertex list.
 */
template <template <typename> class Alloc = std::allocator>
struct vertex_list
{
    typedef unused key_type;

    typedef std::list<int, Alloc<int>> dummy;
    typedef typename descriptor_traits<dummy>::descriptor_type descriptor_type;

    template <typename Vertex>
    struct store
    {
        typedef vertex_list_impl<Vertex, Alloc<Vertex>> type;
    };
};

/**
 * The implementation of the vertex list.
 *
 * @param Vertex The type of stored vertex.
 * @param Alloc The allocator for stored vertices.
 */
template <typename Vertex, typename Alloc>
class vertex_list_impl
{
public:
    typedef std::list<Vertex, Alloc> store_type;
    typedef typename store_type::size_type size_type;
    typedef typename store_type::iterator iterator;
    typedef typename store_type::const_iterator const_iterator;

    typedef Vertex vertex_type;
    typedef typename Vertex::vertex_properties vertex_properties;

    typedef typename descriptor_traits<store_type>::descriptor_type vertex_descriptor;

    typedef simple_vertex_iterator<store_type> vertex_iterator;
    typedef std::pair<vertex_iterator, vertex_iterator> vertex_range;

    // Constructors
    inline vertex_list_impl()
        : _verts(), _size(0)
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
        ++_size;
        iterator i = insert(_verts, vertex_type(vp));
        return make_descriptor(_verts, i);
    }
    //@}

    /** Find the vertex with the given properties. */
    inline vertex_descriptor find(vertex_properties const& vp)
    {
        iterator i = std::find_if(_verts.begin(), _verts.end(), find_properties(vp));
        return make_descriptor(_verts, i);
    }

    /** @name Remove Vertex
     * Remove the given vertex from the vertex store. This will probably break
     * if the descriptor is invalid.
     */
    //@{
    inline void remove(vertex_descriptor v)
    {
        _verts.erase(make_iterator(_verts, v));
        --_size;
    }

    inline void remove(vertex_properties const& vp)
    { remove(find(vp)); }
    //@}

    /** Return the number of vertices in the set. */
    inline size_type size() const
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
    { return *make_iterator(_verts, v); }

    vertex_type const& vertex(vertex_descriptor v) const
    { return *make_iterator(_verts, v); }
    //@}

    /** @name Vertex Properties */
    //@{
    vertex_properties& properties(vertex_descriptor v)
    { return vertex(v).properties(); }

    vertex_properties const& properties(vertex_descriptor v) const
    { return vertex(v).properties(); }
    //@}

private:
    store_type  _verts;
    size_type   _size;
};


#endif

