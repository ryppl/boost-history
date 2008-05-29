
#ifndef BOOST_GRAPHS_ADJACENCY_LIST_VERTEX_EDGE_SET_HPP
#define BOOST_GRAPHS_ADJACENCY_LIST_VERTEX_EDGE_SET_HPP

#include <set>

#include <boost/graphs/adjacency_list/ves/vertex_edge_store.hpp>

namespace boost {
namespace graphs {
namespace adj_list {

/**
 * The vertex edge set provides set-based storage for edges connected to
 * vertices. This type supports insertions, find, and removal in logarithmic
 * time. Vertex edge sets only work for simple graphs since they preclude the
 * possibility of parallel edges.
 */
template <typename Edge>
class vertex_edge_set
    : public vertex_edge_store< std::set<Edge> >
{
    typedef vertex_edge_store< std::set<Edge> > base_type;
public:
    typedef Edge edge_descriptor;
    typedef typename base_type::iterator iterator;
    typedef typename base_type::const_iterator const_iterator;
    typedef typename base_type::size_type size_type;

    // Constructors
    vertex_edge_set();

    // Add/Find/Remove interface.
    void add(edge_descriptor e);
    iterator find(edge_descriptor e);
    const_iterator find(edge_descriptor e) const;
    void remove(edge_descriptor e);
    void clear();
    size_type size() const;
};

// Functions

template <typename E>
vertex_edge_set<E>::vertex_edge_set()
    : base_type()
{ }

template <typename E>
void
vertex_edge_set<E>::add(edge_descriptor e)
{
    this->_store.insert(e);
}

template <typename E>
typename vertex_edge_set<E>::iterator
vertex_edge_set<E>::find(edge_descriptor e)
{
    return this->_store.find(e);
}

template <typename E>
typename vertex_edge_set<E>::const_iterator
vertex_edge_set<E>::find(edge_descriptor e) const
{
    return this->_store.find(e);
}

template <typename E>
void
vertex_edge_set<E>::remove(edge_descriptor e)
{
    this->_store.erase(e);
}

template <typename E>
void
vertex_edge_set<E>::clear()
{
    this->_store.clear();
}

template <typename E>
typename vertex_edge_set<E>::size_type
vertex_edge_set<E>::size() const
{
    return this->_store.size();
}


} /* namespace adj_list */
} /* namespace graphs */
} /* namespace boost */

#endif
