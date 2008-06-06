
#ifndef BOOST_GRAPHS_ADJACENCY_LIST_MAPPED_VERTEX_ITERATOR_HPP
#define BOOST_GRAPHS_ADJACENCY_LIST_MAPPED_VERTEX_ITERATOR_HPP

namespace boost {
namespace graphs {
namespace adj_list {

/**
 * The mapped vertex iterator provides a vertex iterator pair associative
 * storage containers. This essintially wraps a store iterator, providing
 * the ability to dererence to descriptors. Because this iterator is taken
 * from pair associative structures, it is bidirectional but not random access.
 */
template <typename Store>
class mapped_vertex_iterator
{
    typedef typename Store::const_iterator iterator;
public:
    typedef typename Store::mapped_type vertex_type;
    typedef typename vertex_type::descriptor_type vertex_descriptor;

    typedef typename iterator::iterator_category iterator_category;
    typedef typename iterator::difference_type difference_type;
    typedef vertex_descriptor value_type;
    typedef vertex_descriptor reference;
    typedef vertex_descriptor pointer;

    inline mapped_vertex_iterator();
    inline mapped_vertex_iterator(mapped_vertex_iterator const& x);
    inline mapped_vertex_iterator(iterator const& x);

    inline mapped_vertex_iterator& operator=(mapped_vertex_iterator const& x);
    inline mapped_vertex_iterator& operator++();
    inline mapped_vertex_iterator& operator--();

    inline reference operator*();

    inline bool operator==(mapped_vertex_iterator const& x) const;
    inline bool operator!=(mapped_vertex_iterator const& x) const;

private:
    iterator iter;
};

template <typename S>
mapped_vertex_iterator<S>::mapped_vertex_iterator()
    : iter()
{ }

template <typename S>
mapped_vertex_iterator<S>::mapped_vertex_iterator(mapped_vertex_iterator const& x)
    : iter(x.iter)
{ }

template <typename S>
mapped_vertex_iterator<S>::mapped_vertex_iterator(iterator const& x)
    : iter(x)
{ }

template <typename S>
mapped_vertex_iterator<S>&
mapped_vertex_iterator<S>::operator=(mapped_vertex_iterator const& x)
{
    iter = x.iter;
    return *this;
}

template <typename S>
mapped_vertex_iterator<S>&
mapped_vertex_iterator<S>::operator++()
{
    ++iter;
    return *this;
}

template <typename S>
mapped_vertex_iterator<S>&
mapped_vertex_iterator<S>::operator--()
{
    --iter;
    return *this;
}

template <typename S>
typename mapped_vertex_iterator<S>::reference
mapped_vertex_iterator<S>::operator*()
{
    return &const_cast<vertex_type&>(iter->second);
}

template <typename S>
bool
mapped_vertex_iterator<S>::operator==(mapped_vertex_iterator const& x) const
{
    return iter == x.iter;
}

template <typename S>
bool
mapped_vertex_iterator<S>::operator!=(mapped_vertex_iterator const& x) const
{
    return iter != x.iter;
}

} /* namespace adj_list */
} /* namespace graph */
} /* namespace boost */

#endif
