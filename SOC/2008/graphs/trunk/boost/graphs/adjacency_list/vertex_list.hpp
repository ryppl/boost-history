
#ifndef BOOST_GRAPHS_ADJLIST_VERTEX_LIST_HPP
#define BOOST_GRAPHS_ADJLIST_VERTEX_LIST_HPP

#include <list>

#include <boost/none.hpp>
#include <boost/descriptors.hpp>
#include <boost/graphs/adjacency_list/vertex_iterator.hpp>

namespace boost { namespace graphs { namespace adjacency_list {

// Forward declarations
template <typename, typename> class vertices_list;

/**
 * This metafunction defines the basic elements of a vertex list.
 */
template <template <typename> class Alloc = std::allocator>
struct vertex_list
{
    typedef unused key_type;

    typedef std::list<int, Alloc<int>> dummy;
    typedef typename descriptor_traits<dummy>::descriptor_type vertex_descriptor;

    template <typename Vertex>
    struct store
    {
        typedef vertices_list<Vertex, Alloc<Vertex>> type;
    };
};

/**
 * The implementation of the vertex list.
 *
 * @param Vertex The type of stored vertex.
 * @param Alloc The allocator for stored vertices.
 */
template <typename Vertex, typename Alloc>
class vertices_list
{
public:
    typedef std::list<Vertex, Alloc> store_type;
    typedef typename store_type::size_type size_type;
    typedef typename store_type::iterator iterator;
    typedef typename store_type::const_iterator const_iterator;

    typedef Vertex vertex_type;
    typedef typename Vertex::vertex_label vertex_label;

    typedef typename descriptor_traits<store_type>::descriptor_type vertex_descriptor;

    typedef basic_vertex_iterator<store_type> vertex_iterator;
    typedef std::pair<vertex_iterator, vertex_iterator> vertex_range;

    // Constructors
    inline vertices_list()
        : _verts(), _size(0)
    { }

    /** @name Add Vertex
     * Add a vertex to the store with the given label (or none). Return
     * a descriptor to the vertex that was added to the vector.
     */
    //@{
    inline vertex_descriptor add()
    { return add(vertex_label()); }

    inline vertex_descriptor add(vertex_label const& vp)
    {
        ++_size;
        iterator i = insert(_verts, vertex_type(vp));
        return make_descriptor(_verts, i);
    }
    //@}

    /** Find the vertex with the given label. */
    inline vertex_descriptor find(vertex_label const& vp)
    {
        iterator i = std::find_if(_verts.begin(), _verts.end(), find_label(vp));
        return make_descriptor(_verts, i);
    }

    /** @name Remove Vertex
     * Remove the given vertex from the vertex store. This will probably break
     * if the descriptor is invalid.
     */
    //@{
    inline void remove(vertex_descriptor v)
    {
        erase(_verts, make_iterator(_verts, v));
        --_size;
    }

    inline void remove(vertex_label const& vp)
    { remove(find(vp)); }
    //@}

    /** Return the number of vertices in the set. */
    inline size_type size() const
    { return _size; }

    /** @name Vertex Iterators
     * There are two sets of iterators here: those that provide iterators that
     * dereference as descriptors and those that dereference as vertices.
     */
    //@{
    inline vertex_iterator begin_vertices() const
    { return vertex_iterator(_verts, _verts.begin()); }

    inline vertex_iterator end_vertices() const
    { return vertex_iterator(_verts, _verts.end()); }

    inline vertex_range vertices() const
    { return std::make_pair(begin_vertices(), end_vertices()); }

    inline iterator begin() const
    { return _verts.begin(); }

    inline iterator end() const
    { return _verts.end(); }
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
    vertex_label& label(vertex_descriptor v)
    { return vertex(v).label(); }

    vertex_label const& label(vertex_descriptor v) const
    { return vertex(v).label(); }
    //@}

private:
    mutable store_type  _verts;
    size_type           _size;
};

} } } /* namespace boost::graphs::adjacency_list */

#endif

