
#ifndef BOOST_GRAPHS_ADJACENCY_LIST_VERTEX_VECTOR_HPP
#define BOOST_GRAPHS_ADJACENCY_LIST_VERTEX_VECTOR_HPP

#include <vector>

#include <boost/graphs/properties.hpp>
#include <boost/graphs/adjacency_list/vs/indexed_vertex_iterator.hpp>

namespace boost {
namespace graphs {
namespace adj_list {

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
template <
        typename Vertex,
        template <typename> class Alloc
    >
class basic_vertex_vector
{
public:
    typedef Vertex vertex_type;
    typedef typename vertex_type::descriptor_type vertex_descriptor;
    typedef typename vertex_type::properties_type vertex_properties;

    typedef std::vector<vertex_type, Alloc<vertex_type> > vertex_store;
    typedef indexed_vertex_iterator<vertex_store> vertex_iterator;
    typedef typename vertex_store::size_type vertices_size_type;

    // FIXME: This is old school.
    typedef indexed_property_map_tag vertex_property_map_category;

    // Constructors
    basic_vertex_vector();

    // Add/remove vertex.
    vertex_descriptor add_vertex();
    vertex_descriptor add_vertex(vertex_properties const& vp);

    // Number of vertices.
    vertices_size_type num_vertices() const;

    // Vertex iteration.
    std::pair<vertex_iterator, vertex_iterator> vertices() const;
    vertex_iterator begin_vertices() const;
    vertex_iterator end_vertices() const;

    // Vertex/vertex property accessors.
    vertex_type& vertex(vertex_descriptor v);
    vertex_type const& vertex(vertex_descriptor v) const;
    vertex_properties& properties(vertex_descriptor);
    vertex_properties const& properties(vertex_descriptor) const;

private:
    vertex_store _verts;
};

/**
 * Specialization of the storage traits to redefine the descriptor. Vectors for
 * any store type must use their indices as descriptors since the underlying
 * storage memory can be reallocated or shiftetd on inserts and removals.
 *
 * @todo For some reason, this doesn't work as a catch all for derived types.
 * How will this work if we use template aliases instead of inheritance? This
 * doesn't seem to be a problem for non-vectors since the default template
 * catches all aspects of it.
 */
template <typename Vertex, template <typename> class Alloc>
struct storage_traits< basic_vertex_vector<Vertex, Alloc> >
{
    typedef std::size_t descriptor_type;
};

/**
 * The default specialization of a vector store.
 */
template <typename Vertex>
struct vertex_vector : basic_vertex_vector<Vertex, std::allocator> { };

/**
 * Apparently, we have to provide a specific partial specialization for each
 * specialized storage. Is this right? Is it wrong? Is the compiler broken?
 */
template <typename Vertex>
struct storage_traits< vertex_vector<Vertex> >
{
    typedef std::size_t descriptor_type;
};

// Functions

template <typename V, template <typename> class A>
basic_vertex_vector<V,A>::basic_vertex_vector()
    : _verts()
{ }

/**
 * Add a vertex to the store with no or default properties, returning the
 * the descriptor to the added vertex. Adding a vertex does not invalidate
 * any vertices or edges.
 *
 * @complexity O(1) amortized
 */
template <typename V, template <typename> class A>
typename basic_vertex_vector<V,A>::vertex_descriptor
basic_vertex_vector<V,A>::add_vertex()
{
    return add_vertex(vertex_properties());
}

/**
 * Add a vertex to the store with the given properties. Adding a vertex does
 * not invalidate any other descriptors.
 *
 * @complexity O(1) amortized
 */
template <typename V, template <typename> class A>
typename basic_vertex_vector<V,A>::vertex_descriptor
basic_vertex_vector<V,A>::add_vertex(vertex_properties const& vp)
{
    // Basically, just push the vertex to the back and return its index.
    _verts.push_back(vertex_type(vp));
    return _verts.size() - 1;
}

/**
 * Return the number of vertices in the vertex store.
 *
 * @complexity constant
 */
template <typename V, template <typename> class A>
typename basic_vertex_vector<V,A>::vertices_size_type
basic_vertex_vector<V,A>::num_vertices() const
{
    return _verts.size();
}

/**
 * Return the iterator range for the graph.
 */
template <typename V, template <typename> class A>
std::pair<
    typename basic_vertex_vector<V,A>::vertex_iterator,
    typename basic_vertex_vector<V,A>::vertex_iterator
>
basic_vertex_vector<V,A>::vertices() const
{
    return std::make_pair(begin_vertices(), end_vertices());
}

/**
 * Get an iterator to the beginning of the vertices.
 */
template <typename V, template <typename> class A>
typename basic_vertex_vector<V,A>::vertex_iterator
basic_vertex_vector<V,A>::begin_vertices() const
{
    return vertex_iterator(_verts, _verts.begin());
}

/**
 * Get an iterator past the end of the vertices.
 */
template <typename V, template <typename> class A>
typename basic_vertex_vector<V,A>::vertex_iterator
basic_vertex_vector<V,A>::end_vertices() const
{
    return vertex_iterator(_verts, _verts.end());
}

/**
 * Get access to the underlying vertex.
 */
template <typename V, template <typename> class A>
typename basic_vertex_vector<V,A>::vertex_type&
basic_vertex_vector<V,A>::vertex(vertex_descriptor v)
{
    return _verts[v.desc];
}

/**
 * Get access to the underlying vertex.
 */
template <typename V, template <typename> class A>
typename basic_vertex_vector<V,A>::vertex_type const&
basic_vertex_vector<V,A>::vertex(vertex_descriptor v) const
{
    return _verts[v.desc];
}

/**
 * Get access to the properties of the given vertex.
 */
template <typename V, template <typename> class A>
typename basic_vertex_vector<V,A>::vertex_properties&
basic_vertex_vector<V,A>::properties(vertex_descriptor v)
{
    return *vertex(v);
}

/**
 * Get access to the properties of the given vertex.
 */
template <typename V, template <typename> class A>
typename basic_vertex_vector<V,A>::vertex_properties const&
basic_vertex_vector<V,A>::properties(vertex_descriptor v) const
{
    return *vertex(v);
}

} /* namespace adj_list */
} /* namesapce graphs */
} /* namespace boost */

#endif
