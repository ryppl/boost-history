
#ifndef BOOST_GRAPHS_ADJACENCY_LIST_INDEXED_EDGE_ITERATOR_HPP
#define BOOST_GRAPHS_ADJACENCY_LIST_INDEXED_EDGE_ITERATOR_HPP

#include <algorithm>

namespace boost {
namespace graphs {
namespace adj_list {

/**
 * The indexed edge iterator provides an edge iterator for stores whose
 * descriptors cannot be pointers (i.e., vectors).
 *
 * Because the underlying store for this iterator is a vector, this is a random
 * access iterator.
 */
template <typename Store>
class indexed_edge_iterator
{
    typedef typename Store::const_iterator iterator;
public:
    typedef typename Store::value_type edge_type;
    typedef typename edge_type::descriptor_type edge_descriptor;

    typedef typename iterator::iterator_category iterator_category;
    typedef typename iterator::difference_type difference_type;
    typedef edge_descriptor value_type;
    typedef edge_descriptor reference;
    typedef edge_descriptor pointer;

    inline indexed_edge_iterator();
    inline indexed_edge_iterator(indexed_edge_iterator const& x);
    inline indexed_edge_iterator(Store const& s, iterator const& x);

    inline indexed_edge_iterator& operator=(indexed_edge_iterator const& x);
    inline indexed_edge_iterator& operator+=(difference_type n);
    inline indexed_edge_iterator& operator-=(difference_type n);
    inline indexed_edge_iterator& operator++();
    inline indexed_edge_iterator& operator--();

    inline indexed_edge_iterator operator+(difference_type n) const;
    inline indexed_edge_iterator operator-(difference_type n) const;
    inline difference_type operator-(indexed_edge_iterator const& x) const;

    inline reference operator*();

    inline bool operator==(indexed_edge_iterator const& x) const;
    inline bool operator!=(indexed_edge_iterator const& x) const;

private:
    Store const* store;
    iterator iter;
};

template <typename S>
indexed_edge_iterator<S>::indexed_edge_iterator()
    : store(0)
    , iter()
{ }

template <typename S>
indexed_edge_iterator<S>::indexed_edge_iterator(indexed_edge_iterator const& x)
    : store(x.store)
    , iter(x.iter)
{ }

template <typename S>
indexed_edge_iterator<S>::indexed_edge_iterator(S const& s, iterator const& x)
    : store(&s)
    , iter(x)
{ }

template <typename S>
indexed_edge_iterator<S>&
indexed_edge_iterator<S>::operator=(indexed_edge_iterator const& x)
{
    iter = x.iter;
    store = x.store;
    return &this;
}

template <typename S>
indexed_edge_iterator<S>&
indexed_edge_iterator<S>::operator+=(difference_type n)
{
    iter += n;
    return &this;
}

template <typename S>
indexed_edge_iterator<S>&
indexed_edge_iterator<S>::operator-=(difference_type n)
{
    iter -= n;
    return &this;
}

template <typename S>
indexed_edge_iterator<S>&
indexed_edge_iterator<S>::operator++()
{
    ++iter;
    return *this;
}

template <typename S>
indexed_edge_iterator<S>&
indexed_edge_iterator<S>::operator--()
{
    --iter;
    return *this;
}

template <typename S>
indexed_edge_iterator<S>
indexed_edge_iterator<S>::operator+(difference_type n) const
{
    return iter + n;
}

template <typename S>
indexed_edge_iterator<S>
indexed_edge_iterator<S>::operator-(difference_type n) const
{
    return iter - n;
}

template <typename S>
typename indexed_edge_iterator<S>::difference_type
indexed_edge_iterator<S>::operator-(indexed_edge_iterator const& x) const
{
    return iter - x;
}

template <typename S>
typename indexed_edge_iterator<S>::reference
indexed_edge_iterator<S>::operator*()
{
    // The returned descriptor is simply the distance from the beginning of
    // the underlying store to the end.
    return std::distance(store->begin(), iter);
}

template <typename S>
bool
indexed_edge_iterator<S>::operator==(indexed_edge_iterator const& x) const
{
    return (store == x.store) && (iter == x.iter);
}

template <typename S>
bool
indexed_edge_iterator<S>::operator!=(indexed_edge_iterator const& x) const
{
    return (store == x.store) && (iter != x.iter);
}

} /* namespace adj_list */
} /* namespace graphs */
} /* namespace boost */

#endif
