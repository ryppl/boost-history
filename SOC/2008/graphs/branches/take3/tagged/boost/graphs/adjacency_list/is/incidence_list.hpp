
#ifndef BOOST_GRAPHS_ADJACENCY_LIST_INCIDENCE_LIST_HPP
#define BOOST_GRAPHS_ADJACENCY_LIST_INCIDENCE_LIST_HPP

#include <list>
#include <algorithm>

#include <boost/graphs/adjacency_list/is/incidence_store.hpp>

namespace boost {
namespace graphs {
namespace adj_list {

/**
 * The vertex edge list provides list-based storage for edges connected to
 * vertices. This type supports insertions in constant time, and find and
 * remove in linear w.r.t. to degree of the vertex.
 *
 * Vertex edge lists work well with both simple and multigraphs, arguably
 * offering near-optimal performance in the average case.
 */
template <typename Edge>
class vertex_edge_list
    : public vertex_edge_store< std::list<Edge> >
{
    typedef vertex_edge_store< std::list<Edge> > base_type;
public:
    typedef Edge edge_descriptor;
    typedef typename base_type::iterator iterator;
    typedef typename base_type::const_iterator const_iterator;
    typedef typename base_type::size_type size_type;

    // Constructors
    inline vertex_edge_list();

    // Add/Find/Remove interface.
    inline void add(edge_descriptor e);
    inline iterator find(edge_descriptor e);
    inline const_iterator find(edge_descriptor e) const;
    inline void remove(edge_descriptor e);
    inline void clear();
    inline size_type size() const;
};

// Functions

template <typename E>
vertex_edge_list<E>::vertex_edge_list()
    : base_type()
{ }

template <typename E>
void
vertex_edge_list<E>::add(edge_descriptor e)
{
    this->_store.push_back(e);
}

template <typename E>
typename vertex_edge_list<E>::iterator
vertex_edge_list<E>::find(edge_descriptor e)
{
    return std::find(this->_store.begin(), this->_store.end(), e);
}

template <typename E>
typename vertex_edge_list<E>::const_iterator
vertex_edge_list<E>::find(edge_descriptor e) const
{
    return std::find(this->_store.begin(), this->_store.end(), e);
}

template <typename E>
void
vertex_edge_list<E>::remove(edge_descriptor e)
{
    this->_store.erase(find(e));
}

template <typename E>
void
vertex_edge_list<E>::clear()
{
    this->_store.clear();
}

template <typename E>
typename vertex_edge_list<E>::size_type
vertex_edge_list<E>::size() const
{
    return this->_store.size();
}

} /* namespace adj_list */
} /* namespace graphs */
} /* namespace boost */

#endif
