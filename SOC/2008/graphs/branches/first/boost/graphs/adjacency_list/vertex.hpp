
#ifndef BOOST_GRAPHS_ADJACENCY_LIST_VERTEX_HPP
#define BOOST_GRAPHS_ADJACENCY_LIST_VERTEX_HPP

namespace boost {
namespace graphs {
namespace adj_list {

/**
 * The vertex template is the base class of all vertices in the adjacency
 * list suite. This basically provides facilities for storing the vertex
 * properties.
 */
template <typename VertexProps>
struct vertex
{
    typedef VertexProps properties_type;

    vertex();
    vertex(properties_type const& vp);

    // Property accessors...
    properties_type* operator->();
    properties_type& operator*();
    properties_type const& operator*() const;

private:
    properties_type _props;
};

template <typename VP>
vertex<VP>::vertex()
    : _props()
{ }

template <typename VP>
vertex<VP>::vertex(properties_type const& vp)
    : _props(vp)
{ }

template <typename VP>
typename vertex<VP>::properties_type*
vertex<VP>::operator->()
{ return &_props; }

template <typename VP>
typename vertex<VP>::properties_type&
vertex<VP>::operator*()
{ return _props; }

template <typename VP>
typename vertex<VP>::properties_type const&
vertex<VP>::operator*() const
{ return _props; }

// Operator overloads

/**
 * By default, vertices are ordered by comparing their properties. Note that
 * this is only instantiated if it's used so you don't have to provide an
 * ordering over vertex properties if you're not using it.
 */
template <typename VP>
bool operator<(vertex<VP> const& u, vertex<VP> const& v)
{ return *u < *v; }

template <typename VP>
bool operator>(vertex<VP> const& u, vertex<VP> const& v)
{ return *v < *u; }


} /* namespace adj_list */
} /* namespace graphs */
} /* namespace boost */

#endif
