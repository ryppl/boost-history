
#ifndef BOOST_GRAPHS_ADJACENCY_LIST_VALUE_EDGE_ITERATOR_HPP
#define BOOST_GRAPHS_ADJACENCY_LIST_VALUE_EDGE_ITERATOR_HPP

namespace boost {
namespace graphs {
namespace adj_list {

/**
 * The value edge iterator provides a edge iterator unique associative
 * containers and sequences that don't invalidate memory on insertions
 * (lists).
 */
template <typename Store>
class value_edge_iterator
{
    typedef typename Store::const_iterator iterator;
public:
    typedef typename Store::value_type edge_type;
    typedef typename edge_type::descriptor_type edge_descriptor;

    typedef edge_descriptor value_type;
    typedef edge_descriptor reference;
    typedef edge_descriptor pointer;

    value_edge_iterator()
        : iter()
    { }

    value_edge_iterator(value_edge_iterator const& x)
        : iter(x.iter)
    { }

    value_edge_iterator(iterator const& x)
        : iter(x)
    { }

    value_edge_iterator& operator++()
    {
        ++iter;
        return *this;
    }

    reference operator*()
    { return &const_cast<edge_type&>(*iter); }

    bool operator==(value_edge_iterator const& x) const
    { return iter == x.iter; }

    bool operator!=(value_edge_iterator const& x) const
    { return iter != x.iter; }

    iterator iter;
};

} /* namespace adj_list */
} /* namespace graphs */
} /* namespace boost */

#endif
