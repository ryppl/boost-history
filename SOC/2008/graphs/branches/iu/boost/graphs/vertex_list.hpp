
#ifndef VERTEX_LIST_HPP
#define VERTEX_LIST_HPP

#include <list>

#include "descriptor.hpp"
#include "simple_vertex_iterator.hpp"

// Forward declarations
template <typename V, template <typename> class A> class vertex_list_elem;
template <typename V, typename A> class vertex_list_impl;

template <template <typename> class Allocator>
struct basic_vertex_list
{
    typedef void* key_type;
    typedef void* descriptor_type;

    template <typename Vertex>
    struct store
    {
        typedef vertex_list_elem<Vertex, Allocator> stored_vertex;
        typedef vertex_list_impl<stored_vertex, Allocator<stored_vertex> > type;
    };
};

struct vertex_list : basic_vertex_list<std::allocator> { };

template <typename Vertex, template <typename> class Alloc>
class vertex_list_elem
    : public Vertex
{
    typedef vertex_list_elem<Vertex, Alloc> this_type;
public:
    typedef typename std::list<this_type, Alloc<this_type> >::iterator iterator;

    inline vertex_list_elem(typename Vertex::vertex_properties const& vp)
        : Vertex(vp)
        , iter()
    { }

    iterator iter;
};

/**
 * The basic_vertex_list provides a list-based implementation of vertex storage
 * for an adjacency list. List-based storage is best for graphs with
 * unidentified vertices and requirements for fast vertex addition and deletion.
 *
 * Adding vertices to a list does not invalidate any vertex or edge descriptors.
 * Removing vertices will invalidate descriptors referencing the removed
 * vertex. All insertions and removals occur in constant time. However, getting
 * the number of vertices is linear.
 *
 * FIXME: Track the size of the list, so that num_vertices() is constant.
 */
template <typename Vertex, typename Allocator>
class vertex_list_impl
{
    typedef std::list<Vertex, Allocator> vertex_store;
public:
    typedef void* vertex_descriptor;
    typedef Vertex vertex_type;
    typedef typename Vertex::vertex_properties vertex_properties;
    typedef typename vertex_store::size_type size_type;
    typedef simple_vertex_iterator<vertex_store> iterator;
    typedef std::pair<iterator, iterator> range;

    // Constructors
    vertex_list_impl();

    // Add/remove vertices.
    vertex_descriptor add();
    vertex_descriptor add(vertex_properties const& vp);

    // Remove vertices.
    void remove(vertex_descriptor v);

    // Number of vertices.
    size_type size() const;

    // Vertex iteration.
    range vertices() const;
    iterator begin() const;
    iterator end() const;

    // Vertex accessors.
    vertex_type& vertex(vertex_descriptor);
    vertex_type const& vertex(vertex_descriptor) const;
    vertex_properties& properties(vertex_descriptor);
    vertex_properties const& properties(vertex_descriptor) const;

private:
    vertex_store _verts;
};

#define BOOST_GRAPHS_VL_PARAMS \
    typename V, typename A

/**
 * Construct an empty vertex list.
 */
template <BOOST_GRAPHS_VL_PARAMS>
vertex_list_impl<V,A>::vertex_list_impl()
    : _verts()
{ }

/**
 * Add a vertex to the list with no or default properties.
 *
 * @complexity O(1)
 */
template <BOOST_GRAPHS_VL_PARAMS>
typename vertex_list_impl<V,A>::vertex_descriptor
vertex_list_impl<V,A>::add()
{
    return add(vertex_properties());
}

/**
 * Add a vertex to the store with the given properties. If not specified, the
 * vertex is created with default properties. Note that vertices are not mapped
 * or ordered so multiple, equivalent vertices can be added to the graph.
 * This adds the vertex to the end of the list.
 *
 * @complexity O(1)
 */
template <BOOST_GRAPHS_VL_PARAMS>
typename vertex_list_impl<V,A>::vertex_descriptor
vertex_list_impl<V,A>::add(vertex_properties const& vp)
{
    typename vertex_store::iterator i = _verts.insert(_verts.end(), vertex_type(vp));
    i->iter = i;
    return &(*i);
}

/**
 * Return the number of vertices in the store.
 */
template <BOOST_GRAPHS_VL_PARAMS>
typename vertex_list_impl<V,A>::size_type
vertex_list_impl<V,A>::size() const
{
    return _verts.size();
}

#if 0

/**
 * Remove a vertex from the store.
 *
 * Removing a vertex will invalidate all vertex and edge descriptors.
 *
 * @complexity O(|V|)
 */
template <typename V, template <typename> class A>
void
vertex_list_impl<V,A>::remove_vertex(vertex_descriptor v)
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
typename vertex_list_impl<V,A>::vertices_size_type
vertex_list_impl<V,A>::num_vertices() const
{
    return _verts.size();
}

/**
 * Return the iterator range for the graph.
 */
template <typename V, template <typename> class A>
std::pair<
    typename vertex_list_impl<V,A>::vertex_iterator,
    typename vertex_list_impl<V,A>::vertex_iterator
>
vertex_list_impl<V,A>::vertices() const
{
    return std::make_pair(_verts.begin(), _verts.end());
}

/**
 * Return the iterator for the begining of the vertices.
 */
template <typename V, template <typename> class A>
typename vertex_list_impl<V,A>::vertex_iterator
vertex_list_impl<V,A>::begin_vertices() const
{
    return _verts.begin();
}

/**
 * Return the iterator for the begining of the vertices.
 */
template <typename V, template <typename> class A>
typename vertex_list_impl<V,A>::vertex_iterator
vertex_list_impl<V,A>::end_vertices() const
{
    return _verts.end();
}

/**
 * Get access to the given vertex.
 */
template <typename V, template <typename> class A>
typename vertex_list_impl<V,A>::vertex_type&
vertex_list_impl<V,A>::vertex(vertex_descriptor v)
{
    return *static_cast<vertex_type*>(v.desc);
}

/**
 * Get access to the given vertex.
 */
template <typename V, template <typename> class A>
typename vertex_list_impl<V,A>::vertex_type const&
vertex_list_impl<V,A>::vertex(vertex_descriptor v) const
{
    return *static_cast<vertex_type*>(v.desc);
}

/**
 * Access the properties ofthe given vertex.
 */
template <typename V, template <typename> class A>
typename vertex_list_impl<V,A>::vertex_properties&
vertex_list_impl<V,A>::properties(vertex_descriptor v)
{
    return *vertex(v);
}

/**
 * Access the properties ofthe given vertex.
 */
template <typename V, template <typename> class A>
typename vertex_list_impl<V,A>::vertex_properties const&
vertex_list_impl<V,A>::properties(vertex_descriptor v) const
{
    return *vertex(v);
}

#endif

#endif

