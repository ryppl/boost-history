
#ifndef BOOST_GRAPHS_ADJACENCY_LIST_VERTEX_EDGE_STORE_HPP
#define BOOST_GRAPHS_ADJACENCY_LIST_VERTEX_EDGE_STORE_HPP

#include <set>

namespace boost {
namespace graphs {
namespace adj_list {

/**
 * The vertex store is a simple helper class that provides a number of common
 * functions for derived types.
 */
template <typename Store>
class vertex_edge_store
{
public:
    typedef typename Store::iterator iterator;
    typedef typename Store::const_iterator const_iterator;
    typedef typename Store::size_type size_type;

    // Constructors
    vertex_edge_store();

    // Iterator access.
    inline iterator begin();
    inline iterator end();

    inline const_iterator begin() const;
    inline const_iterator end() const;

protected:
    Store _store;
};

// Functions

template <typename S>
vertex_edge_store<S>::vertex_edge_store()
    : _store()
{ }

template <typename S>
typename vertex_edge_store<S>::iterator
vertex_edge_store<S>::begin()
{
    return _store.begin();
}

template <typename S>
typename vertex_edge_store<S>::iterator
vertex_edge_store<S>::end()
{
    return _store.end();
}

template <typename S>
typename vertex_edge_store<S>::const_iterator
vertex_edge_store<S>::begin() const
{
    return _store.begin();
}

template <typename S>
typename vertex_edge_store<S>::const_iterator
vertex_edge_store<S>::end() const
{
    return _store.end();
}

} /* namespace adj_list */
} /* namespace graphs */
} /* namespace boost */

#endif
