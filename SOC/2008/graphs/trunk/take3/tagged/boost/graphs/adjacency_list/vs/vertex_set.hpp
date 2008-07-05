
#ifndef BOOST_GRAPHS_ADJACENCY_LIST_VERTEX_SET_HPP
#define BOOST_GRAPHS_ADJACENCY_LIST_VERTEX_SET_HPP

#include <set>
#include <tr1/unordered_map>

#include <boost/graphs/adjacency_list/descriptor.hpp>
#include <boost/graphs/adjacency_list/vs/simple_vertex_iterator.hpp>

namespace boost {
namespace graphs {
namespace adj_list {

// Forward declarations
template <typename V, template <typename> class C, template <typename> class A> class vertex_set_elem;
template <typename V, typename C, typename A> class vertex_set_impl;

template <template <typename> class Compare, template <typename> class Allocator>
struct basic_vertex_set
{
    typedef basic_vertex_descriptor<void*> descriptor_type;

    template <typename Vertex>
    struct store
    {
        typedef vertex_set_elem<Vertex, Compare, Allocator> stored_vertex;
        typedef vertex_set_impl<stored_vertex, Compare<stored_vertex>, Allocator<stored_vertex> > type;
    };
};

template <template <typename> class Compare = std::less>
struct vertex_set : basic_vertex_set<Compare, std::allocator> { };

// Extend the notion of a vertex for set storage so that we can store each
// vertex's iterator with current vertex. This is used to provide constant
// time access to the correct position in the underliying store.
template <typename Vertex,
        template <typename> class Compare,
        template <typename> class Alloc>
class vertex_set_elem
    : public Vertex
{
    typedef vertex_set_elem<Vertex, Compare, Alloc> this_type;
public:
    typedef typename std::set<
            this_type, Compare<this_type>, Alloc<this_type>
        >::iterator iterator;

    inline vertex_set_elem(typename Vertex::vertex_properties const& vp)
        : Vertex(vp)
        , iter()
    { }

    iterator iter;
};

/**
 * The vertex_set_impl provides a list-based implementation of vertex storage
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
template <typename Vertex, typename Compare, typename Allocator>
class vertex_set_impl
{
    typedef std::set<Vertex, Compare, Allocator> vertex_store;
public:
    typedef Vertex vertex_type;
    typedef typename Vertex::vertex_properties vertex_properties;
    typedef typename Vertex::vertex_descriptor vertex_descriptor;
    typedef typename vertex_store::size_type vertices_size_type;
    typedef simple_vertex_iterator<vertex_store> vertex_iterator;
    typedef std::pair<vertex_iterator, vertex_iterator> vertex_range;

    // Constructors
    vertex_set_impl();

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


#define BOOST_GRAPHS_VS_PARAMS \
    typename V, typename C, typename A

template <BOOST_GRAPHS_VS_PARAMS>
vertex_set_impl<V,C,A>::vertex_set_impl()
    : _verts()
{ }

#undef BOOST_GRAPHS_VS_PARAMS

#if 0

/**
 * Add a vertex to the store with the given properties. If not specified, the
 * vertex is created with default properties. Note that vertices are not mapped
 * or ordered so multiple, equivalent vertices can be added to the graph.
 *
 * @complexity O(log(V))
 */
template <BOOST_GRAPH_VS_PARAMS>
typename vertex_set_impl<V,C,A>::vertex_descriptor
vertex_set_impl<V,C,A>::add_vertex(vertex_properties const& vp)
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
std::pair<typename vertex_set_impl<V,C,A>::vertex_descriptor, bool>
vertex_set_impl<V,C,A>::insert_vertex(vertex_properties const& vp)
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
vertex_set_impl<V,C,A>::remove_vertex(vertex_descriptor v)
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
vertex_set_impl<V,C,A>::remove_vertex(vertex_properties const& vp)
{
    remove_vertex(find(vp));
}

/**
 * Find a vertex by its properties.
 *
 * @complexity O(log(V))
 */
template <BOOST_GRAPH_VS_PARAMS>
typename vertex_set_impl<V,C,A>::vertex_descriptor
vertex_set_impl<V,C,A>::find_vertex(vertex_properties const& vp) const
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
typename vertex_set_impl<V,C,A>::vertices_size_type
vertex_set_impl<V,C,A>::num_vertices() const
{
    return _verts.size();
}

/**
 * Return the iterator range for the graph.
 */
template <BOOST_GRAPH_VS_PARAMS>
std::pair<
    typename vertex_set_impl<V,C,A>::vertex_iterator,
    typename vertex_set_impl<V,C,A>::vertex_iterator
>
vertex_set_impl<V,C,A>::vertices() const
{
    return std::make_pair(_verts.begin(), _verts.end());
}

/**
 * Get a vertex iterator to the beginning iterator of the vertices.
 */
template <BOOST_GRAPH_VS_PARAMS>
typename vertex_set_impl<V,C,A>::vertex_iterator
vertex_set_impl<V,C,A>::begin_vertices() const
{
    return _verts.begin();
}

/**
 * Get a vertex iterator to an iterator past the end of the vertices.
 */
template <BOOST_GRAPH_VS_PARAMS>
typename vertex_set_impl<V,C,A>::vertex_iterator
vertex_set_impl<V,C,A>::end_vertices() const
{
    return _verts.end();
}

/**
 * Get access to the given vertex.
 */
template <BOOST_GRAPH_VS_PARAMS>
typename vertex_set_impl<V,C,A>::vertex_type&
vertex_set_impl<V,C,A>::vertex(vertex_descriptor v)
{
    return *static_cast<vertex_type*>(v.desc);
}

/**
 * Get access to the given vertex.
 */
template <BOOST_GRAPH_VS_PARAMS>
typename vertex_set_impl<V,C,A>::vertex_type const&
vertex_set_impl<V,C,A>::vertex(vertex_descriptor v) const
{
    return *static_cast<vertex_type*>(v.desc);
}

/**
 * Access the properties of the given vertex.
 */
template <BOOST_GRAPH_VS_PARAMS>
typename vertex_set_impl<V,C,A>::vertex_properties&
vertex_set_impl<V,C,A>::properties(vertex_descriptor v)
{
    return *vertex(v);
}

/**
 * Access the properties of the given vertex.
 */
template <BOOST_GRAPH_VS_PARAMS>
typename vertex_set_impl<V,C,A>::vertex_properties const&
vertex_set_impl<V,C,A>::properties(vertex_descriptor v) const
{
    return *vertex(v);
}

#endif

} /* namespace adj_list */
} /* namesapce graphs */
} /* namespace boost */

#endif
