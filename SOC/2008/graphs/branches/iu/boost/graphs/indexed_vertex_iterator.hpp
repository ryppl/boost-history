
#ifndef INDEXED_VERTEX_ITERATOR
#define INDEXED_VERTEX_ITERATOR

#include <algorithm>

/**
 * The indexed vertex iterator provides a vertex iterator for stores whose
 * descriptors cannot be pointers (i.e., vectors). By virtue of the fact that
 * Store is required to be a vector of something, this is a random access
 * iterator.
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

    // Constructors
    inline indexed_vertex_iterator();
    inline indexed_vertex_iterator(indexed_vertex_iterator const& x);
    inline indexed_vertex_iterator(Store const& s, iterator const& x);

    // Assignment and increment
    inline indexed_vertex_iterator& operator=(indexed_vertex_iterator const& x);
    inline indexed_vertex_iterator& operator+=(difference_type n);
    inline indexed_vertex_iterator& operator-=(difference_type n);
    inline indexed_vertex_iterator& operator++();
    inline indexed_vertex_iterator& operator--();

    inline indexed_vertex_iterator operator+(difference_type n) const;
    inline indexed_vertex_iterator operator-(difference_type n) const;
    inline difference_type operator-(indexed_vertex_iterator const& x) const;

    inline reference operator*();

    inline bool operator==(indexed_vertex_iterator const& x) const;
    inline bool operator!=(indexed_vertex_iterator const& x) const;

private:
    Store const* store;
    iterator iter;
};

template <typename S>
indexed_vertex_iterator<S>::indexed_vertex_iterator()
    : store(0)
    , iter()
{ }

template <typename S>
indexed_vertex_iterator<S>::indexed_vertex_iterator(indexed_vertex_iterator const& x)
    : store(x.store)
    , iter(x.iter)
{ }

template <typename S>
indexed_vertex_iterator<S>::indexed_vertex_iterator(S const& s, iterator const& x)
    : store(&s)
    , iter(x)
{ }

template <typename S>
indexed_vertex_iterator<S>&
indexed_vertex_iterator<S>::operator=(indexed_vertex_iterator const& x)
{
    iter = x.iter;
    store = x.store;
    return *this;
}

template <typename S>
indexed_vertex_iterator<S>&
indexed_vertex_iterator<S>::operator+=(difference_type n)
{
    iter += n;
    return *this;
}

template <typename S>
indexed_vertex_iterator<S>&
indexed_vertex_iterator<S>::operator-=(difference_type n)
{
    iter -= n;
    return *this;
}

template <typename S>
indexed_vertex_iterator<S>&
indexed_vertex_iterator<S>::operator++()
{
    ++iter;
    return *this;
}

template <typename S>
indexed_vertex_iterator<S>&
indexed_vertex_iterator<S>::operator--()
{
    --iter;
    return *this;
}

template <typename S>
indexed_vertex_iterator<S>
indexed_vertex_iterator<S>::operator+(difference_type n) const
{
    return iter + n;
}

template <typename S>
indexed_vertex_iterator<S>
indexed_vertex_iterator<S>::operator-(difference_type n) const
{
    return iter - n;
}

template <typename S>
typename indexed_vertex_iterator<S>::difference_type
indexed_vertex_iterator<S>::operator-(indexed_vertex_iterator const& x) const
{
    return iter - x.iter;
}

template <typename S>
typename indexed_vertex_iterator<S>::reference
indexed_vertex_iterator<S>::operator*()
{
    // The returned descriptor is simply the distance from the beginning of
    // the underlying store to the end.
    return std::distance(store->begin(), iter);
}

template <typename S>
bool
indexed_vertex_iterator<S>::operator==(indexed_vertex_iterator const& x) const
{
     return (store == x.store) && (iter == x.iter);
}

template <typename S>
bool
indexed_vertex_iterator<S>::operator!=(indexed_vertex_iterator const& x) const
{
    return (store == x.store) && (iter != x.iter);
}

#endif
