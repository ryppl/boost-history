
#ifndef BOOST_GRAPHS_ADJACENCY_LIST_INDEXED_VERTEX_ITERATOR_HPP
#define BOOST_GRAPHS_ADJACENCY_LIST_INDEXED_VERTEX_ITERATOR_HPP

#include <algorithm>

namespace boost {
namespace graphs {
namespace adj_list {

/**
 * The indexed vertex iterator provides a vertex iterator for stores whose
 * descriptors cannot be pointers (i.e., vectors).
 */
template <typename Store>
class indexed_vertex_iterator
{
    typedef typename Store::const_iterator iterator;
public:
    typedef typename Store::value_type vertex_type;
    typedef typename vertex_type::descriptor_type vertex_descriptor;

    typedef typename iterator::iterator_category iterator_category;
    typedef typename iterator::difference_type difference_type;
    typedef vertex_descriptor value_type;
    typedef vertex_descriptor reference;
    typedef vertex_descriptor pointer;

    inline indexed_vertex_iterator()
        : store(0)
        , iter()
    { }

    inline indexed_vertex_iterator(indexed_vertex_iterator const& x)
        : store(x.store)
        , iter(x.iter)
    { }

    inline indexed_vertex_iterator(Store const& s, iterator const& x)
        : store(&s)
        , iter(x)
    { }

    inline indexed_vertex_iterator& operator++()
    {
        ++iter;
        return *this;
    }

    inline indexed_vertex_iterator& operator=(indexed_vertex_iterator const& x)
    {
        indexed_vertex_iterator t(x);
        swap(t);
        return *this;
    }

    inline indexed_vertex_iterator& operator+=(difference_type n)
    {
        iter += n;
        return *this;
    }

    inline indexed_vertex_iterator& operator-=(difference_type n)
    {
        iter -= n;
        return *this;
    }

    // Support addition and subtraction as per random access iterators
    inline indexed_vertex_iterator operator+(difference_type n) const
    { return iter + n; }

    inline indexed_vertex_iterator operator-(difference_type n) const
    { return iter - n; }

    inline difference_type operator-(indexed_vertex_iterator const& x) const
    { return iter - x.iter; }

    // The returned descriptor is simply the distance from the beginning of
    // the underlying store to the end.
    inline reference operator*()
    { return std::distance(store->begin(), iter); }

    inline bool operator==(indexed_vertex_iterator const& x) const
    { return (store == x.store) && (iter == x.iter); }

    inline bool operator!=(indexed_vertex_iterator const& x) const
    { return (store == x.store) && (iter != x.iter); }

    inline void swap(indexed_vertex_iterator& x)
    {
        std::swap(store, x.store);
        std::swap(iter, x.iter);
    }

    Store const* store;
    iterator iter;
};

} /* namespace adj_list */
} /* namespace graphs */
} /* namespace boost */

#endif
