
#ifndef VERTEX_MAP_HPP
#define VERTEX_MAP_HPP

#include <map>

#include <boost/descriptors.hpp>
#include <boost/graphs/adjacency_list/vertex_iterator.hpp>

// Forward declarations
template <typename, typename, typename, typename> class vertices_map;

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
        template <typename> class Compare = std::less,
        template <typename> class Alloc = std::allocator>
struct vertex_map
{
    typedef Key key_type;

    typedef std::map<Key, int, Compare<Key>, Alloc<std::pair<Key, int>>> dummy;
    typedef typename descriptor_traits<dummy>::descriptor_type vertex_descriptor;

    template <typename Vertex>
    struct store
    {
        typedef Alloc<std::pair<Key, Vertex>> allocator;
        typedef Compare<Key> compare;
        typedef vertices_map<Vertex, Key, compare, allocator> type;
    };
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
class vertices_map
{
public:
    typedef std::map<Key, Vertex, Compare, Allocator> store_type;
    typedef typename store_type::size_type size_type;
    typedef typename store_type::iterator iterator;
    typedef typename store_type::const_iterator const_iterator;

    typedef Key key_type;

    typedef Vertex vertex_type;
    typedef typename Vertex::vertex_properties vertex_properties;

    typedef typename descriptor_traits<store_type>::descriptor_type vertex_descriptor;

    typedef basic_vertex_iterator<store_type> vertex_iterator;
    typedef std::pair<vertex_iterator, vertex_iterator> vertex_range;

    inline vertices_map()
        : _verts()
    { }

    /**
     * Add a vertex to the store mapped to the given key, and having the given
     * properties.
     */
    inline vertex_descriptor add(key_type const& k, vertex_properties const& vp)
    { return make_descriptor(_verts, _verts.insert(std::make_pair(k, vp)).first); }

    /**
     * Find the vertex mapped to the given key and return a descriptor to it.
     */
    inline vertex_descriptor find(key_type const& k) const
    { return make_descriptor(_verts, _verts.find(k)); }

    /** @name Remove Vertex
     * Remove the vertex identified by the descriptor or the its properties.
     */
    //@{
    inline void remove(vertex_descriptor d)
    { _verts.erase(make_iterator(_verts, d)); }

    inline void remove(key_type const& k)
    { _verts.erase(_verts.find(k)); }
    //@}

    /** Return the number of vertices in the map. */
    inline size_type size() const
    { return _verts.size(); }

    /** @name Vertex Iteration */
    //@{
    inline vertex_iterator begin_vertices() const
    { return vertex_iterator(_verts, _verts.begin()); }

    inline vertex_iterator end_vertices() const
    { return vertex_iterator(_verts, _verts.end()); }

    vertex_range vertices() const
    { return std::make_pair(begin_vertices(), end_vertices()); }

    inline iterator begin() const
    { return _verts.begin(); }

    inline iterator end() const
    { return _verts.end(); }
    //@}

    /** @name Vertex and Key Accessors */
    //@{
    vertex_type& vertex(vertex_descriptor v)
    { return make_iterator(_verts, v)->second; }

    vertex_type const& vertex(vertex_descriptor v) const
    { return make_iterator(_verts, v)->second; }

    key_type const& key(vertex_descriptor d) const
    { return make_iterator(_verts, d)->first; }
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
