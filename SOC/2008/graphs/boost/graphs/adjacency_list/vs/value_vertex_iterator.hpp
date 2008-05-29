
#ifndef BOOST_GRAPHS_ADJACENCY_LIST_VALUE_VERTEX_ITERATOR_HPP
#define BOOST_GRAPHS_ADJACENCY_LIST_VALUE_VERTEX_ITERATOR_HPP

#include <iterator>

namespace boost {
namespace graphs {
namespace adj_list {

/**
 * The value vertex iterator provides a vertex iterator unique associative
 * containers and sequences that don't invalidate memory on insertions
 * (lists).
 *
 * Note that this is actually a random access iterator and can be used to
 * support trivial fill (memset) and copy (memcpy) operations.
 */
template <typename Store>
class value_vertex_iterator
{
    typedef typename Store::const_iterator iterator;
public:
    typedef typename Store::value_type vertex_type;
    typedef typename vertex_type::descriptor_type vertex_descriptor;

    typedef typename iterator::iterator_category iterator_category;
    typedef vertex_descriptor value_type;
    typedef vertex_descriptor reference;
    typedef vertex_descriptor pointer;
    typedef typename iterator::difference_type difference_type;

    inline value_vertex_iterator()
        : iter()
    { }

    inline value_vertex_iterator(value_vertex_iterator const& x)
        : iter(x.iter)
    { }

    inline value_vertex_iterator(iterator const& x)
        : iter(x)
    { }

    inline value_vertex_iterator& operator++()
    {
        ++iter;
        return *this;
    }

    inline value_vertex_iterator& operator--()
    {
        --iter;
        return *this;
    }

    inline reference operator*()
    { return &const_cast<vertex_type&>(*iter); }

    inline bool operator==(value_vertex_iterator const& x) const
    { return iter == x.iter; }

    inline bool operator!=(value_vertex_iterator const& x) const
    { return iter != x.iter; }

    iterator iter;
};

} /* namespace adj_list */
} /* namespace graphs */
} /* namespace boost */

#endif
