
#ifndef VERTEX_ITERATOR_HPP
#define VERTEX_ITERATOR_HPP

#include <boost/descriptors.hpp>

/**
 * A simple vertex iterator for any underlying store.
 */
template <typename Container>
class basic_vertex_iterator
{
public:
    typedef typename Container::iterator iterator;
    typedef typename Container::value_type vertex_type;
    typedef typename descriptor_traits<Container>::descriptor_type vertex_descriptor;

    typedef typename iterator::iterator_category iterator_category;
    typedef typename iterator::difference_type difference_type;
    typedef vertex_descriptor value_type;
    typedef vertex_descriptor reference;
    typedef vertex_descriptor pointer;

    inline basic_vertex_iterator()
        : cont(0), iter()
    { }

    inline basic_vertex_iterator(Container& c, iterator x)
        : cont(c), iter(x)
    { }

    // Assignment and increment
    inline basic_vertex_iterator& operator+=(difference_type n)
    { iter += n; return *this; }

    inline basic_vertex_iterator& operator-=(difference_type n)
    { iter -= n; return *this; }

    inline basic_vertex_iterator& operator++()
    { ++iter; return *this; }

    inline basic_vertex_iterator& operator--()
    { --iter; return *this; }

    inline basic_vertex_iterator operator+(difference_type n) const
    { return indexed_basic_vertex_iterator(cont, iter + n); }

    inline basic_vertex_iterator operator-(difference_type n) const
    { return indexed_basic_vertex_iterator(cont, iter -n); }

    inline difference_type operator-(basic_vertex_iterator const& x) const
    { return iter - x.iter; }

    inline reference operator*()
    { return make_descriptor(*cont, iter); }

    inline bool operator==(basic_vertex_iterator const& x) const
    { return iter == x.iter; }

    inline bool operator!=(basic_vertex_iterator const& x) const
    { return iter != x.iter; }

private:
    Container*  cont;
    iterator    iter;
};

#endif
