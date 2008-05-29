
#ifndef BOOST_GRAPHS_ADJACENCY_LIST_MAPPED_VERTEX_ITERATOR_HPP
#define BOOST_GRAPHS_ADJACENCY_LIST_MAPPED_VERTEX_ITERATOR_HPP

namespace boost {
namespace graphs {
namespace adj_list {

/**
 * The mapped vertex iterator provides a vertex iterator pair associative
 * storage containers. This essintially wraps a store iterator, providing
 * the ability to dererence to descriptors.
 */
template <typename Store>
class mapped_vertex_iterator
{
    typedef typename Store::const_iterator iterator;
public:
    typedef typename Store::mapped_type vertex_type;
    typedef typename vertex_type::descriptor_type vertex_descriptor;

    typedef vertex_descriptor value_type;
    typedef vertex_descriptor reference;
    typedef vertex_descriptor pointer;

    inline mapped_vertex_iterator()
        : iter()
    { }

    inline mapped_vertex_iterator(mapped_vertex_iterator const& x)
        : iter(x.iter)
    { }

    inline mapped_vertex_iterator(iterator const& x)
        : iter(x)
    { }

    inline mapped_vertex_iterator& operator++()
    {
        ++iter;
        return *this;
    }

    inline reference operator*()
    { return &const_cast<vertex_type&>(iter->second); }

    inline bool operator==(mapped_vertex_iterator const& x) const
    { return iter == x.iter; }

    inline bool operator!=(mapped_vertex_iterator const& x) const
    { return iter != x.iter; }

    iterator iter;
};

} /* namespace adj_list */
} /* namespace graph */
} /* namespace boost */

#endif
