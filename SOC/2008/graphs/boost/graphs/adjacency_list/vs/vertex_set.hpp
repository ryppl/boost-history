
#ifndef BOOST_GRAPHS_ADJACENCY_LIST_VERTEX_SET_HPP
#define BOOST_GRAPHS_ADJACENCY_LIST_VERTEX_SET_HPP

#include <set>
#include <tr1/unordered_map>

#include <boost/graphs/properties.hpp>
#include <boost/graphs/adjacency_list/vs/value_vertex_iterator.hpp>

namespace boost {
namespace graphs {
namespace adj_list {

namespace detail {
    // Extend the notion of a vertex for set storage so that we can store each
    // vertex's iterator with current vertex. This is used to provide constant
    // time access to the correct position in the underliying store.
    template <
            typename Vertex,
            template <typename> class Compare,
            template <typename> class Alloc>
    class basic_vertex_set_node
        : public Vertex
    {
    public:
        typedef basic_vertex_set_node<Vertex, Compare, Alloc> this_type;
        typedef typename Vertex::properties_type properties_type;

        typedef typename std::set<
                this_type, Compare<this_type>, Alloc<this_type>
            >::iterator iterator;

        inline basic_vertex_set_node(properties_type const& vp)
            : Vertex(vp)
            , iter()
        { }

        iterator iter;
    };

} /* namespace detail */

/**
 * The basic_vertex_set provides a list-based implementation of vertex storage
 * for an adjacency list. List-based storage is best for graphs with
 * unidentified vertices and requirements for fast vertex addition and deletion.
 *
 * This class requires that the graph's vertex properties be less than
 * comparable since the ordering of vertices in the set is based on that
 * implementation of the ordering. Note that the vertex type provides a built
 * in ordering using operator<() that delegates the call to the properties.
 *
 * Adding vertices to a list does not invalidate any vertex or edge descriptors.
 * Removing vertices will invalidate descriptors referencing the removed
 * vertex. All insertions and removals occur in constant time. However, getting
 * the number of vertices is linear.
 *
 * @require LessThanComparable<Vertex::properties_type>
 */
template <
        typename Vertex,
        template <typename> class Compare = std::less,
        template <typename> class Alloc = std::allocator
    >
class basic_vertex_set
{
public:
    typedef detail::basic_vertex_set_node<Vertex, Compare, Alloc> vertex_type;
    typedef typename vertex_type::descriptor_type vertex_descriptor;
    typedef typename vertex_type::properties_type vertex_properties;

    typedef std::set<
            vertex_type, Compare<vertex_type>, Alloc<vertex_type>
        > vertex_store;
    typedef value_vertex_iterator<vertex_store> vertex_iterator;
    typedef typename vertex_store::size_type vertices_size_type;

    // This is kind of hack, but we need some way of communicating a preference
    // of the favored underlying property store to higher level features.
    // FIXME: Clearly, this should go away during conceptization.
    typedef hashed_property_map_tag vertex_property_map_category;

    // Constructors
    basic_vertex_set();

    // Add vertices. Note that you can't add without properties.
    vertex_descriptor add_vertex(vertex_properties const& vp);
    std::pair<vertex_descriptor, bool> insert_vertex(vertex_properties const& vp);

    // Remove vertices.
    void remove_vertex(vertex_descriptor v);
    void remove_vertex(vertex_properties const& v);

    // Find a vertex based on its properties.
    vertex_descriptor find_vertex(vertex_properties const& vp) const;

    // Number of vertices.
    vertices_size_type num_vertices() const;

    // Vertex iteration.
    std::pair<vertex_iterator, vertex_iterator> vertices() const;
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


/**
 * The default implementation of a vertex set is given here. Specialized
 * versions of the basic vertex set can be generated using inheritance like
 * this. This version fixes the default comparator and allocator of the vertex
 * set to the common defaults.
 */
template <typename Vertex>
struct vertex_set : basic_vertex_set<Vertex> { };

// Some examples of specialized (curried) variants of the basic vertex set.
// These provide specific orderings. Note that the "less set" is actually the
// same as the default vertex set.
template <typename Vertex>
struct min_vertex_set : public basic_vertex_set<Vertex, std::less> { };

template <typename Vertex>
struct max_vertex_set : public basic_vertex_set<Vertex, std::greater> { };


#define BOOST_GRAPH_VS_PARAMS \
    typename V, template <typename> class C, template <typename> class A

/**
 * The default constructor creates an empty vertex set.
 */
template <BOOST_GRAPH_VS_PARAMS>
basic_vertex_set<V,C,A>::basic_vertex_set()
    : _verts()
{ }

/**
 * Add a vertex to the store with the given properties. If not specified, the
 * vertex is created with default properties. Note that vertices are not mapped
 * or ordered so multiple, equivalent vertices can be added to the graph.
 *
 * @complexity O(log(V))
 */
template <BOOST_GRAPH_VS_PARAMS>
typename basic_vertex_set<V,C,A>::vertex_descriptor
basic_vertex_set<V,C,A>::add_vertex(vertex_properties const& vp)
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
std::pair<typename basic_vertex_set<V,C,A>::vertex_descriptor, bool>
basic_vertex_set<V,C,A>::insert_vertex(vertex_properties const& vp)
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
 * Remove a vertex from the store.
 *
 * @complexity O(log(V))
 */
template <BOOST_GRAPH_VS_PARAMS>
void
basic_vertex_set<V,C,A>::remove_vertex(vertex_descriptor v)
{
    vertex_type* vp = static_cast<vertex_type*>(v.desc);
    _verts.erase(vp->iter);
}

/**
 * Remove the vertex identified by the given properties from the store.
 *
 * @complexity O(log(V))
 */
template <BOOST_GRAPH_VS_PARAMS>
void
basic_vertex_set<V,C,A>::remove_vertex(vertex_properties const& vp)
{
    remove_vertex(find(vp));
}

/**
 * Find a vertex by its properties.
 *
 * @complexity O(log(V))
 */
template <BOOST_GRAPH_VS_PARAMS>
typename basic_vertex_set<V,C,A>::vertex_descriptor
basic_vertex_set<V,C,A>::find_vertex(vertex_properties const& vp) const
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
typename basic_vertex_set<V,C,A>::vertices_size_type
basic_vertex_set<V,C,A>::num_vertices() const
{
    return _verts.size();
}

/**
 * Return the iterator range for the graph.
 */
template <BOOST_GRAPH_VS_PARAMS>
std::pair<
    typename basic_vertex_set<V,C,A>::vertex_iterator,
    typename basic_vertex_set<V,C,A>::vertex_iterator
>
basic_vertex_set<V,C,A>::vertices() const
{
    return std::make_pair(_verts.begin(), _verts.end());
}

/**
 * Get a vertex iterator to the beginning iterator of the vertices.
 */
template <BOOST_GRAPH_VS_PARAMS>
typename basic_vertex_set<V,C,A>::vertex_iterator
basic_vertex_set<V,C,A>::begin_vertices() const
{
    return _verts.begin();
}

/**
 * Get a vertex iterator to an iterator past the end of the vertices.
 */
template <BOOST_GRAPH_VS_PARAMS>
typename basic_vertex_set<V,C,A>::vertex_iterator
basic_vertex_set<V,C,A>::end_vertices() const
{
    return _verts.end();
}

/**
 * Get access to the given vertex.
 */
template <BOOST_GRAPH_VS_PARAMS>
typename basic_vertex_set<V,C,A>::vertex_type&
basic_vertex_set<V,C,A>::vertex(vertex_descriptor v)
{
    return *static_cast<vertex_type*>(v.desc);
}

/**
 * Get access to the given vertex.
 */
template <BOOST_GRAPH_VS_PARAMS>
typename basic_vertex_set<V,C,A>::vertex_type const&
basic_vertex_set<V,C,A>::vertex(vertex_descriptor v) const
{
    return *static_cast<vertex_type*>(v.desc);
}

/**
 * Access the properties of the given vertex.
 */
template <BOOST_GRAPH_VS_PARAMS>
typename basic_vertex_set<V,C,A>::vertex_properties&
basic_vertex_set<V,C,A>::properties(vertex_descriptor v)
{
    return *vertex(v);
}

/**
 * Access the properties of the given vertex.
 */
template <BOOST_GRAPH_VS_PARAMS>
typename basic_vertex_set<V,C,A>::vertex_properties const&
basic_vertex_set<V,C,A>::properties(vertex_descriptor v) const
{
    return *vertex(v);
}

} /* namespace adj_list */

} /* namesapce graphs */
} /* namespace boost */

#endif
