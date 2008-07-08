
#ifndef VERTEX_SET_HPP
#define VERTEX_SET_HPP

#include <set>

#include "vertex_descriptor.hpp"
#include "vertex_iterator.hpp"

// Forward declarations
template <typename, template <typename> class, template <typename> class> class vertex_set_elem;
template <typename, typename> class vertex_set_compare;
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
template <template <typename> class Compare, template <typename> class Alloc>
struct basic_vertex_set
{
    typedef unused key_type;
    typedef basic_vertex_descriptor<void*> descriptor_type;

    template <typename Vertex>
    struct store
    {
        // Build the stored vertex type.
        typedef vertex_set_elem<Vertex, Compare, Alloc> stored_vertex;

        // Build the vertex comparator.
        typedef Compare<typename stored_vertex::vertex_properties> vertex_compare;
        typedef vertex_set_compare<stored_vertex, vertex_compare> compare_type;

        // Build the allocator.
        typedef Alloc<stored_vertex> allocator_type;

        // Build the vertex set implementation.
        typedef vertex_set_impl<stored_vertex, compare_type, allocator_type> type;
    };
};

/**
 * The most common vertex set allows parameterization over the comparator used
 * to sort vertices and uses the standard omnibus allocator.
 */
template <template <typename> class Compare = std::less>
struct vertex_set : basic_vertex_set<Compare, std::allocator> { };

/**
 * Extend the notion of a vertex for set storage so that we can store each
 * vertex's iterator with current vertex. This is used to provide constant
 * time access to the correct position in the underliying store.
 */
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
 * A forwarding comparator for the vertex forwards the comparison to the given
 * comparator. This type is used internally to forward comparisons of vertices
 * to the property comparison provided by the edge set parameter.
 *
 * @param StoredVertex The type of vertex being compared
 * @param Compare An ordering over vertex properties.
 */
template <typename StoredVertex, typename Compare>
struct vertex_set_compare
{
    inline vertex_set_compare()
        : comp(Compare())
    { }

    inline vertex_set_compare(vertex_set_compare const& x)
        : comp(x.comp)
    { }

    inline bool operator()(StoredVertex const& a, StoredVertex const& b) const
    { return comp(a.properties(), b.properties()); }

    Compare comp;
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
    typedef std::set<Vertex, Compare, Allocator> vertex_store;
public:
    typedef basic_vertex_descriptor<void*> vertex_descriptor;

    typedef Vertex vertex_type;
    typedef typename Vertex::vertex_properties vertex_properties;
    typedef typename vertex_store::size_type size_type;
    typedef typename vertex_store::iterator iterator;
    typedef typename vertex_store::const_iterator const_iterator;


    typedef simple_vertex_iterator<vertex_store> vertex_iterator;
    typedef std::pair<vertex_iterator, vertex_iterator> vertex_range;

    // Constructors
    vertex_set_impl();

    // Add vertices. Note that you can't add without properties.
    vertex_descriptor add(vertex_properties const& vp);
    vertex_descriptor find(vertex_properties const& vp) const;
    void remove(vertex_descriptor v);
    void remove(vertex_properties const& v);

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
    { return *static_cast<vertex_type*>(v.get()); }

    vertex_type const& vertex(vertex_descriptor v) const
    { return *static_cast<vertex_type*>(v.get()); }
    //@}

    /** @name Property Accessors */
    //@{
    vertex_properties& properties(vertex_descriptor v)
    { return vertex(v).properties(); }

    vertex_properties const& properties(vertex_descriptor v) const
    { return vertex(v).properties(); }
    //@{

private:
    vertex_store _verts;
};


#define BOOST_GRAPH_VS_PARAMS \
    typename V, typename C, typename A

template <BOOST_GRAPH_VS_PARAMS>
vertex_set_impl<V,C,A>::vertex_set_impl()
    : _verts()
{ }

/**
 * Add the vertex with the given properties. If there is already a vertex with
 * these properties, then this returns the descriptor of the existing vertex
 * and does not insert a new vertex.
 *
 * @complexity O(lg(V))
 */
template <BOOST_GRAPH_VS_PARAMS>
typename vertex_set_impl<V,C,A>::vertex_descriptor
vertex_set_impl<V,C,A>::add(vertex_properties const& vp)
{
    // Try to insert the vertex.
    vertex_descriptor ret;
    std::pair<iterator, bool> ins = _verts.insert(vertex_type(vp));
    if(ins.second) {
        vertex_type& v = const_cast<vertex_type&>(*(ins.first));
        v.iter = ins.first;
        ret = &v;
    }
    else {
        ret = &const_cast<vertex_type&>(*ins.first);
    }
    return ret;
}

/**
 * Find the descriptor with the given properties.
 *
 * @complexity O(log(V))
 */
template <BOOST_GRAPH_VS_PARAMS>
typename vertex_set_impl<V,C,A>::vertex_descriptor
vertex_set_impl<V,C,A>::find(vertex_properties const& vp) const
{
    return &const_cast<vertex_type&>(*_verts.find(vp));
}

/**
 * Remove a vertex from the set. Removing a vertex will invalidate all
 * descriptors and iterators to the vertex being removed.
 *
 * @complexity O(log(V))
 */
template <BOOST_GRAPH_VS_PARAMS>
void
vertex_set_impl<V,C,A>::remove(vertex_descriptor v)
{
    _verts.erase(vertex(v).iter);
}

/**
 * Remove the vertex identified by the given properties from the set. This
 * finds the vertex before removing it.
 *
 * @complexity O(log(V))
 */
template <BOOST_GRAPH_VS_PARAMS>
void
vertex_set_impl<V,C,A>::remove(vertex_properties const& vp)
{
    remove(find(vp));
}

#undef BOOST_GRAPH_VS_PARAMS

#endif
