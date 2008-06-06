
#ifndef BOOST_GRAPHS_ADJACENCY_LIST_VERTEX_MAP_HPP
#define BOOST_GRAPHS_ADJACENCY_LIST_VERTEX_MAP_HPP

#include <map>
#include <string>

#include <boost/graphs/adjacency_list/vs/mapped_vertex_iterator.hpp>

namespace boost {
namespace graphs {
namespace adj_list {

namespace detail {
    // Extend the notion of a vertex for set storage so that we can store each
    // vertex's iterator with current vertex. This is used to provide constant
    // time access to the correct position in the underliying store.
    template <
            typename Vertex,
            typename Key,
            template <typename> class Compare,
            template <typename> class Alloc
        >
    class basic_vertex_map_node
        : public Vertex
    {
    public:
        typedef basic_vertex_map_node<Vertex, Key, Compare, Alloc> this_type;
        typedef typename Vertex::properties_type properties_type;
        typedef typename std::map<
                Key, this_type, Compare<Key>, Alloc< std::pair<Key, this_type> >
            >::iterator iterator;

        inline basic_vertex_map_node(properties_type const& vp)
            : Vertex(vp)
            , iter()
        { }

        iterator iter;
    };
} /* namespace detail */

/**
 * The basic_vertex_map provides a list-based implementation of vertex storage
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
        typename Key,
        template <typename> class Compare = std::less,
        template <typename> class Alloc = std::allocator
    >
class basic_vertex_map
{
public:
    typedef detail::basic_vertex_map_node<Vertex, Key, Compare, Alloc> vertex_type;
    typedef typename vertex_type::descriptor_type vertex_descriptor;
    typedef typename vertex_type::properties_type vertex_properties;

    typedef Key key_type;

    typedef std::map<
            key_type, vertex_type, Compare<key_type>, Alloc< std::pair<key_type, vertex_type> >
        > vertex_store;
    typedef mapped_vertex_iterator<vertex_store> vertex_iterator;
    typedef typename vertex_store::size_type vertices_size_type;

    // FIXME: Clearly, this should go away during conceptization.
    typedef hashed_property_map_tag vertex_property_map_category;

    // Constructors
    basic_vertex_map();

    // Add or insert a vertex.
    vertex_descriptor add_vertex(key_type const& k, vertex_properties const& vp);
    std::pair<vertex_descriptor, bool> insert_vertex(key_type const& k, vertex_properties const& vp);

    // Remove a vertex.
    void remove_vertex(vertex_descriptor v);

    // Find a vertex
    vertex_descriptor find_vertex(key_type const& k) const;

    // Number of vertices.
    vertices_size_type num_vertices() const;

    // Vertex iteration.
    std::pair<vertex_iterator, vertex_iterator> vertices() const;
    vertex_iterator begin_vertices() const;
    vertex_iterator end_vertices() const;

    // Vertex accessors.
    vertex_type& vertex(vertex_descriptor v);
    vertex_type const& vertex(vertex_descriptor v) const;
    vertex_descriptor descriptor(vertex_type const& v) const;
    key_type const& key(vertex_descriptor v) const;

    // Property accessors.
    vertex_properties& properties(vertex_descriptor v);
    vertex_properties const& properties(vertex_descriptor v) const;


private:
    vertex_store _verts;
};

// There isn't really a single trivial specialization of the mapped vertex
// store since the key is a required parameter. Basically, using this type
// requires the programmer to build different specializations of this basic
// vertex store.

/**
 * Provide a specialization for keyed to strings.
 */
template <typename Vertex>
struct string_to_vertex_map : basic_vertex_map<Vertex, std::string> { };

/**
 * Provide a specialization for integers.
 */
template <typename Vertex>
struct int_to_vertex_map : basic_vertex_map<Vertex, int> { };


/**
 * The default constructor creates an empty vertex set.
 */
template <typename V, typename K, template <typename> class C, template <typename> class A>
basic_vertex_map<V,K,C,A>::basic_vertex_map()
    : _verts()
{ }

/**
 * Add a vertex to the store with the key and properties. If the key is mapped
 * to a vertex, do nothing. Return a descriptor to the existing or new vertex.
 *
 * @complexity O(log(V))
 */
template <typename V, typename K, template <typename> class C, template <typename> class A>
typename basic_vertex_map<V,K,C,A>::vertex_descriptor
basic_vertex_map<V,K,C,A>::add_vertex(const K& k, vertex_properties const& vp)
{
    return insert_vertex(k, vp).first;
}

/**
 * Add a vertex to the store with the given properties. If not specified, the
 * vertex is created with default properties and return a descriptor to the
 * inserted vertex. If the vertex exists, the second element of the returned
 * pair is false.
 *
 * @complexity O(log(V))
 */
template <typename V, typename K, template <typename> class C, template <typename> class A>
std::pair<typename basic_vertex_map<V,K,C,A>::vertex_descriptor, bool>
basic_vertex_map<V,K,C,A>::insert_vertex(key_type const& k, vertex_properties const& vp)
{
    std::pair<vertex_descriptor, bool> ret;
    std::pair<typename vertex_store::iterator, bool> ins =
            _verts.insert(make_pair(k, vertex_type(vp)));
    if(ins.second) {
        // Yikes... so dirty. Normally, we can't modify an object via its
        // iterator since that would indicate a change to the key. However,
        // the key is based on the properties of the vertex, not the part of
        // the object that we're going to change.
        vertex_type& v = ins.first->second;
        v.iter = ins.first;
        ret.first = &v;
    }
    else {
        ret.first = &(ins.first->second);
    }
    ret.second = ins.second;

    return ret;
}

/**
 * Find a vertex given a key. If the key does not exist, return an invalid
 * descriptor.
 */
template <typename V, typename K, template <typename> class C, template <typename> class A>
typename basic_vertex_map<V,K,C,A>::vertex_descriptor
basic_vertex_map<V,K,C,A>::find_vertex(key_type const& k) const
{
    vertex_descriptor ret;
    typename vertex_store::const_iterator iter = _verts.find(k);
    if(iter != _verts.end()) {
        // Because the function is const, the resulting vertex should also be
        // const. Unfortunately, this isn't really going to work for me.
        vertex_type& v = const_cast<vertex_type&>(iter->second);
        ret = &v;
    }
    return ret;
}

/**
 * Remove a vertex from the store.
 *
 * Removing a vertex will invalidate all vertex and edge descriptors.
 *
 * @complexity O(|V|)
 */
template <typename V, typename K, template <typename> class C, template <typename> class A>
void
basic_vertex_map<V,K,C,A>::remove_vertex(vertex_descriptor v)
{
    vertex_type* vp = static_cast<vertex_type*>(v);
    _verts.erase(vp->iter);
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
template <typename V, typename K, template <typename> class C, template <typename> class A>
typename basic_vertex_map<V,K,C,A>::vertices_size_type
basic_vertex_map<V,K,C,A>::num_vertices() const
{
    return _verts.size();
}

/**
 * Return the iterator range for the graph.
 */
template <typename V, typename K, template <typename> class C, template <typename> class A>
std::pair<
    typename basic_vertex_map<V,K,C,A>::vertex_iterator,
    typename basic_vertex_map<V,K,C,A>::vertex_iterator
>
basic_vertex_map<V,K,C,A>::vertices() const
{
    return std::make_pair(_verts.begin(), _verts.end());
}

/**
 * Get a vertex iterator to the beginning iterator of the vertices.
 */
template <typename V, typename K, template <typename> class C, template <typename> class A>
typename basic_vertex_map<V,K,C,A>::vertex_iterator
basic_vertex_map<V,K,C,A>::begin_vertices() const
{ return _verts.begin(); }

/**
 * Get a vertex iterator to an iterator past the end of the vertices.
 */
template <typename V, typename K, template <typename> class C, template <typename> class A>
typename basic_vertex_map<V,K,C,A>::vertex_iterator
basic_vertex_map<V,K,C,A>::end_vertices() const
{ return _verts.end(); }

/**
 * Get access to the given vertex.
 */
template <typename V, typename K, template <typename> class C, template <typename> class A>
typename basic_vertex_map<V,K,C,A>::vertex_type&
basic_vertex_map<V,K,C,A>::vertex(vertex_descriptor v)
{
    return *static_cast<vertex_type*>(v.desc);
}

/**
 * Get access to the given vertex.
 */
template <typename V, typename K, template <typename> class C, template <typename> class A>
typename basic_vertex_map<V,K,C,A>::vertex_type const&
basic_vertex_map<V,K,C,A>::vertex(vertex_descriptor v) const
{
    return *static_cast<vertex_type*>(v.desc);
}

/**
 * Return a descriptor for the given vertex.
 */
template <typename V, typename K, template <typename> class C, template <typename> class A>
typename basic_vertex_map<V,K,C,A>::vertex_descriptor
basic_vertex_map<V,K,C,A>::descriptor(vertex_type const& v) const
{
    return &const_cast<vertex_type&>(v);
}

/**
 * Get the key of a vertex descriptor.
 */
template <typename V, typename K, template <typename> class C, template <typename> class A>
typename basic_vertex_map<V,K,C,A>::key_type const&
basic_vertex_map<V,K,C,A>::key(vertex_descriptor v) const
{
    vertex_type& vert = *static_cast<vertex_type*>(v.desc);
    return vert.iter->first;
}


/**
 * Get the properties of the given vertex.
 */
template <typename V, typename K, template <typename> class C, template <typename> class A>
typename basic_vertex_map<V,K,C,A>::vertex_properties&
basic_vertex_map<V,K,C,A>::properties(vertex_descriptor v)
{
    return *vertex(v);
}

/**
 * Get the properties of the given vertex.
 */
template <typename V, typename K, template <typename> class C, template <typename> class A>
typename basic_vertex_map<V,K,C,A>::vertex_properties const&
basic_vertex_map<V,K,C,A>::properties(vertex_descriptor v) const
{
    return *vertex(v);
}

} /* namespace adj_list */
} /* namesapce graphs */
} /* namespace boost */

#endif
