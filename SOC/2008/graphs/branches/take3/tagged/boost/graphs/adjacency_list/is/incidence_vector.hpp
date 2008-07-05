
#ifndef BOOST_GRAPHS_ADJACENCY_LIST_INCIDENCE_VECTOR_HPP
#define BOOST_GRAPHS_ADJACENCY_LIST_INCIDENCE_VECTOR_HPP

#include <vector>

#include <boost/graphs/adjacency_list/is/incidence_store.hpp>

namespace boost {
namespace graphs {
namespace adj_list {

// Forward declarations
template <typename ED, typename A> struct incidence_vector_impl;

template <template <typename> class Allocator>
struct basic_incidence_vector
{
    template <typename EdgeDesc>
    struct store
    {
        typedef incidence_vector_impl<EdgeDesc, Allocator<EdgeDesc> > type;
    };
};

struct incidence_vector : basic_incidence_vector<std::allocator> { };

template <typename EdgeDesc, typename Allocator>
struct incidence_vector_impl
{
};

#if 0

/**
 * The vertex edge vector provides vector-based storage for edges connected to
 * vertices. This type supports insertions in constant time, and find and
 * remove are supported in linear time. The remove operation will invalidate
 * vertex edge iterators (i.e., in, out, incident, adjacency).
 */
template <typename Edge>
class vertex_edge_vector
    : public vertex_edge_store< std::vector<Edge> >
{
    typedef vertex_edge_store< std::vector<Edge> > base_type;
public:
    typedef Edge edge_descriptor;
    typedef typename base_type::iterator iterator;
    typedef typename base_type::const_iterator const_iterator;
    typedef typename base_type::size_type size_type;

    // Constructors
    inline vertex_edge_vector();

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
vertex_edge_vector<E>::vertex_edge_vector()
    : base_type()
{ }

template <typename E>
void
vertex_edge_vector<E>::add(edge_descriptor e)
{
    this->_store.push_back(e);
}

template <typename E>
typename vertex_edge_vector<E>::iterator
vertex_edge_vector<E>::find(edge_descriptor e)
{
    return std::find(this->_store.begin(), this->_store.end(), e);
}

template <typename E>
typename vertex_edge_vector<E>::const_iterator
vertex_edge_vector<E>::find(edge_descriptor e) const
{
    return std::find(this->_store.begin(), this->_store.end(), e);
}

template <typename E>
void
vertex_edge_vector<E>::remove(edge_descriptor e)
{
    this->_store.erase(find(e));
}

template <typename E>
void
vertex_edge_vector<E>::clear()
{
    this->_store.clear();
}

template <typename E>
typename vertex_edge_vector<E>::size_type
vertex_edge_vector<E>::size() const
{
    return this->_store.size();
}

#endif

} /* namespace adj_list */
} /* namespace graphs */
} /* namespace boost */

#endif
