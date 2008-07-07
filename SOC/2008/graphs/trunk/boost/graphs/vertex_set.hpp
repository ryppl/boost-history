
#ifndef VERTEX_SET_HPP
#define VERTEX_SET_HPP

#include <set>

#include <boost/none.hpp>
#include <boost/descriptors.hpp>
#include <boost/graphs/utility.hpp>

#include "vertex_iterator.hpp"

// Forward declarations
template <typename, typename, typename> class vertex_set_impl;

/**
 * This metafunction defines the implementation requirements of a set of
 * vertices. This function allos the parameterization of vertex comparability
 * by allowing the caller to specify the comparator.
 *
 * The Compare parameter must be a unary template class. This lets us specify
 * arbitrary function objects by name without having to explicitly instantiate
 * them.
 *
 * The Alloc parameter is also a unary template class. In this case, the caller
 * does not actually know what the final instantiated type is going to be.
 *
 * @param Compare A unary template class that compares vertex properties.
 * @param Alloc A unary template class that allocates vertices.
 */
template <
    template <typename> class Compare = std::less,
    template <typename> class Alloc = std::allocator>
struct vertex_set
{
    typedef unused key_type;

    typedef std::set<int, Compare<int>, Alloc<int>> dummy;
    typedef typename descriptor_traits<dummy>::descriptor_type descriptor_type;

    // This metafunction generates the underlying vertex store implementation.
    template <typename Vertex>
    struct store
    {
        typedef vertex_set_impl<
                Vertex,
                property_comparator<Compare<typename Vertex::vertex_properties>>,
                Alloc<Vertex>
            > type;
    };
};


/**
 * Implementation of the vertex set. This requires that vertices (actually
 * their properties) are less-than comparable.
 *
 * @param Vertex The type of vertex stored by the set.
 * @param Compare An ordering of vertices (should delegate to properties).
 * @param Allocator The allocator for stored vertices.
 */
template <typename Vertex, typename Compare, typename Allocator>
class vertex_set_impl
{
public:
    typedef std::set<Vertex, Compare, Allocator> store_type;
    typedef typename store_type::size_type size_type;
    typedef typename store_type::iterator iterator;
    typedef typename store_type::const_iterator const_iterator;

    typedef Vertex vertex_type;
    typedef typename Vertex::vertex_properties vertex_properties;

    typedef typename descriptor_traits<store_type>::descriptor_type vertex_descriptor;

    typedef simple_vertex_iterator<store_type> vertex_iterator;
    typedef std::pair<vertex_iterator, vertex_iterator> vertex_range;

    inline vertex_set_impl()
        : _verts()
    { }

    /**
     * Add a vertex to the store with the given properties (or none). Return
     * a descriptor to the vertex that was added to the vector.
     */
    inline vertex_descriptor add(vertex_properties const& vp)
    { return make_descriptor(_verts, insert(_verts, vertex_type(vp))); }

    /** Find the vertex with the given properties. */
    inline vertex_descriptor find(vertex_properties const& vp) const
    { return make_descriptor(_verts, _verts.find(vp)); }

    /** @name Remove Vertex
     * Remove the given vertex or the one identified by the given properties.
     */
    //@{
    inline void remove(vertex_descriptor v)
    {
        iterator i = make_iterator(_verts, v);
        erase(_verts, make_iterator(_verts, v));
    }

    inline void remove(vertex_properties const& v)
    { remove(find(v)); }
    //@}

    /** Return the number of vertices in the set. */
    inline size_type size() const
    { return _verts.size(); }

    /** @name Vertex Iteration */
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
    { return const_cast<vertex_type&>(*make_iterator(_verts, v)); }

    vertex_type const& vertex(vertex_descriptor v) const
    { return *make_iterator(_verts, v); }
    //@}

    /** @name Property Accessors */
    //@{
    vertex_properties& properties(vertex_descriptor v)
    { return vertex(v).properties(); }

    vertex_properties const& properties(vertex_descriptor v) const
    { return vertex(v).properties(); }
    //@{

private:
    mutable store_type _verts;
};

#endif
