
#ifndef BOOST_GRAPHS_ADJACENCY_LIST_VERTEX_LIST_HPP
#define BOOST_GRAPHS_ADJACENCY_LIST_VERTEX_LIST_HPP

#include <list>

#include <boost/graphs/adjacency_list/vs/simple_vertex_iterator.hpp>

namespace boost {
namespace graphs {
namespace adj_list {

namespace detail {
    // Extend the notion of a vertex for list storage so that we can store each
    // vertex's iterator the vertex. Basically, this is used to provide constant
    // time access to correct vertex in the container. I can't think of a better
    // way to to do this.
    template <typename Vertex, template <typename> class Alloc>
    struct basic_vertex_list_node
        : Vertex
    {
        typedef basic_vertex_list_node<Vertex, Alloc> this_type;
        typedef typename Vertex::properties_type properties_type;
        typedef typename std::list<
                this_type, Alloc<this_type>
            >::iterator iterator;

        inline basic_vertex_list_node(properties_type const& vp)
            : Vertex(vp)
            , iter()
        { }

        iterator iter;
    };
} /* namespace detail */

/**
 * The basic_vertex_list provides a list-based implementation of vertex storage
 * for an adjacency list. List-based storage is best for graphs with
 * unidentified vertices and requirements for fast vertex addition and deletion.
 *
 * Adding vertices to a list does not invalidate any vertex or edge descriptors.
 * Removing vertices will invalidate descriptors referencing the removed
 * vertex. All insertions and removals occur in constant time. However, getting
 * the number of vertices is linear.
 */
template <
        typename Vertex,
        template <typename> class Alloc = std::allocator
    >
class basic_vertex_list
{
public:
    typedef detail::basic_vertex_list_node<Vertex, Alloc> vertex_type;
    typedef typename vertex_type::descriptor_type vertex_descriptor;
    typedef typename vertex_type::properties_type vertex_properties;

    typedef std::list<vertex_type, Alloc<vertex_type> > vertex_store;
    typedef simple_vertex_iterator<vertex_store> vertex_iterator;
    typedef typename vertex_store::size_type vertices_size_type;

    // FIXME: Clearly, this should go away during conceptization.
    typedef hashed_property_map_tag vertex_property_map_category;

    basic_vertex_list()
        : _verts()
    { }

    // Add/remove vertices.
    vertex_descriptor add_vertex();
    vertex_descriptor add_vertex(vertex_properties const& vp);

    // Remove vertices.
    void remove_vertex(vertex_descriptor v);

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
 * Create a default specialization of the basic vertex list.
 */
template <typename Vertex>
struct vertex_list : basic_vertex_list<Vertex> { };

/**
 * Add a vertex to the store with no or default properties.
 *
 * @complexity O(1)
 */
template <typename V, template <typename> class A>
typename basic_vertex_list<V,A>::vertex_descriptor
basic_vertex_list<V,A>::add_vertex()
{
    return add_vertex(vertex_properties());
}


/**
 * Add a vertex to the store with the given properties. If not specified, the
 * vertex is created with default properties. Note that vertices are not mapped
 * or ordered so multiple, equivalent vertices can be added to the graph.
 * This adds the vertex to the end of the list.
 *
 * @complexity O(1)
 */
template <typename V, template <typename> class A>
typename basic_vertex_list<V,A>::vertex_descriptor
basic_vertex_list<V,A>::add_vertex(vertex_properties const& vp)
{
    typename vertex_store::iterator i = _verts.insert(_verts.end(), vertex_type(vp));
    i->iter = i;
    return &(*i);
}

/**
 * Remove a vertex from the store.
 *
 * Removing a vertex will invalidate all vertex and edge descriptors.
 *
 * @complexity O(|V|)
 */
template <typename V, template <typename> class A>
void
basic_vertex_list<V,A>::remove_vertex(vertex_descriptor v)
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
template <typename V, template <typename> class A>
typename basic_vertex_list<V,A>::vertices_size_type
basic_vertex_list<V,A>::num_vertices() const
{
    return _verts.size();
}

/**
 * Return the iterator range for the graph.
 */
template <typename V, template <typename> class A>
std::pair<
    typename basic_vertex_list<V,A>::vertex_iterator,
    typename basic_vertex_list<V,A>::vertex_iterator
>
basic_vertex_list<V,A>::vertices() const
{
    return std::make_pair(_verts.begin(), _verts.end());
}

/**
 * Return the iterator for the begining of the vertices.
 */
template <typename V, template <typename> class A>
typename basic_vertex_list<V,A>::vertex_iterator
basic_vertex_list<V,A>::begin_vertices() const
{
    return _verts.begin();
}

/**
 * Return the iterator for the begining of the vertices.
 */
template <typename V, template <typename> class A>
typename basic_vertex_list<V,A>::vertex_iterator
basic_vertex_list<V,A>::end_vertices() const
{
    return _verts.end();
}

/**
 * Get access to the given vertex.
 */
template <typename V, template <typename> class A>
typename basic_vertex_list<V,A>::vertex_type&
basic_vertex_list<V,A>::vertex(vertex_descriptor v)
{
    return *static_cast<vertex_type*>(v.desc);
}

/**
 * Get access to the given vertex.
 */
template <typename V, template <typename> class A>
typename basic_vertex_list<V,A>::vertex_type const&
basic_vertex_list<V,A>::vertex(vertex_descriptor v) const
{
    return *static_cast<vertex_type*>(v.desc);
}

/**
 * Access the properties ofthe given vertex.
 */
template <typename V, template <typename> class A>
typename basic_vertex_list<V,A>::vertex_properties&
basic_vertex_list<V,A>::properties(vertex_descriptor v)
{
    return *vertex(v);
}

/**
 * Access the properties ofthe given vertex.
 */
template <typename V, template <typename> class A>
typename basic_vertex_list<V,A>::vertex_properties const&
basic_vertex_list<V,A>::properties(vertex_descriptor v) const
{
    return *vertex(v);
}

} /* namespace adj_list */
} /* namesapce graphs */
} /* namespace boost */

#endif
