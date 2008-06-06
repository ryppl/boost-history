
#ifndef SIMPLE_VERTEX_ITERATOR_HPP
#define SIMPLE_VERTEX_ITERATOR_HPP

#include <iterator>

/**
 * The value vertex iterator provides a vertex iterator unique associative
 * containers and sequences that don't invalidate memory on insertions
 * (lists).
 */
template <typename Store>
class simple_vertex_iterator
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

    inline simple_vertex_iterator();
    inline simple_vertex_iterator(simple_vertex_iterator const& x);
    inline simple_vertex_iterator(iterator const& x);

    inline simple_vertex_iterator& operator=(simple_vertex_iterator const& x);
    inline simple_vertex_iterator& operator++();
    inline simple_vertex_iterator& operator--();

    inline reference operator*();

    inline bool operator==(simple_vertex_iterator const& x) const;
    inline bool operator!=(simple_vertex_iterator const& x) const;

private:
    iterator iter;
};

template <typename S>
simple_vertex_iterator<S>::simple_vertex_iterator()
    : iter()
{ }

template <typename S>
simple_vertex_iterator<S>::simple_vertex_iterator(simple_vertex_iterator const& x)
    : iter(x.iter)
{ }

template <typename S>
simple_vertex_iterator<S>::simple_vertex_iterator(iterator const& x)
    : iter(x)
{ }

template <typename S>
simple_vertex_iterator<S>&
simple_vertex_iterator<S>::operator=(simple_vertex_iterator const& x)
{
    iter = x.iter;
    return *this;
}

template <typename S>
simple_vertex_iterator<S>&
simple_vertex_iterator<S>::operator++()
{
    ++iter;
    return *this;
}

template <typename S>
simple_vertex_iterator<S>&
simple_vertex_iterator<S>::operator--()
{
    --iter;
    return *this;
}

template <typename S>
typename simple_vertex_iterator<S>::reference
simple_vertex_iterator<S>::operator*()
{
    return &const_cast<vertex_type&>(*iter);
}

template <typename S>
bool
simple_vertex_iterator<S>::operator==(simple_vertex_iterator const& x) const
{
    return iter == x.iter;
}

template <typename S>
bool
simple_vertex_iterator<S>::operator!=(simple_vertex_iterator const& x) const
{
    return iter != x.iter;
}

#endif
