
#ifndef BOOST_GRAPHS_ADJACENCY_LIST_INDEXED_EDGE_ITERATOR_HPP
#define BOOST_GRAPHS_ADJACENCY_LIST_INDEXED_EDGE_ITERATOR_HPP

#include <algorithm>

namespace boost {
namespace graphs {
namespace adj_list {

/**
 * The indexed edge iterator provides an edge iterator for stores whose
 * descriptors cannot be pointers (i.e., vectors).
 */
template <typename Store>
class indexed_edge_iterator
{
    typedef typename Store::const_iterator iterator;
public:
    typedef typename Store::value_type edge_type;
    typedef typename edge_type::descriptor_type edge_descriptor;

    typedef edge_descriptor value_type;
    typedef edge_descriptor reference;
    typedef edge_descriptor pointer;

    indexed_edge_iterator()
        : store(0)
        , iter()
    { }

    indexed_edge_iterator(indexed_edge_iterator const& x)
        : store(x.store)
        , iter(x.iter)
    { }

    indexed_edge_iterator(Store const& s, iterator const& x)
        : store(&s)
        , iter(x)
    { }

    indexed_edge_iterator& operator++()
    {
        ++iter;
        return *this;
    }

    // The returned descriptor is simply the distance from the beginning of
    // the underlying store to the end.
    reference operator*()
    { return std::distance(store->begin(), iter); }

    bool operator==(indexed_edge_iterator const& x) const
    { return (store == x.store) && (iter == x.iter); }

    bool operator!=(indexed_edge_iterator const& x) const
    { return (store == x.store) && (iter != x.iter); }

    Store const* store;
    iterator iter;
};

} /* namespace adj_list */
} /* namespace graphs */
} /* namespace boost */

#endif
