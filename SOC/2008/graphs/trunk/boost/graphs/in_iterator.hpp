
#ifndef IN_ITERATOR_HPP
#define IN_ITERATOR_HPP

/**
 * The in edge iterator is a wrapper around out store iterators that, when
 * dereferenced will return an edge descriptor. The iterator category is taken
 * from the base iterator. The in edge iterator also needs to know the type
 * of the out edge iterator since in edges contain placeheld references to
 * out edges.
 */
template <typename Edge>
class basic_in_iterator
{
    typedef typename Edge::in_iterator base_iterator;
    typedef typename Edge::out_iterator out_iterator;
    typedef typename out_iterator::value_type out_tuple;
public:
    typedef typename boost::tuples::element<0, out_tuple>::type vertex_descriptor;
    typedef typename boost::tuples::element<1, out_tuple>::type edge_properties;

    // This is a little misleading. This iterator can be either bidi or random.
    // Clearly, we're going to be constraining members using some concept stuff
    // when it becomes available.
    typedef typename base_iterator::iterator_category iterator_category;
    typedef typename base_iterator::difference_type difference_type;

    typedef Edge value_type;
    typedef value_type reference;
    typedef value_type pointer;

    inline basic_in_iterator()
        : _base(), _iter()
    { }

    inline basic_in_iterator(basic_in_iterator const& x)
        : _base(x._base), _iter(x._iter)
    { }

    inline basic_in_iterator(vertex_descriptor v, base_iterator const& x)
        : _base(v), _iter(x)
    { }

    /**
     * Extended iterator functionality. Return the source vertex of the
     * iterator. This is the vertex for which the iterator was originally
     * created.
     */
    vertex_descriptor source() const
    { return _base; }

    /**
     * Extended iterator functionality. Return the opposite vertex of the
     * edge indicated by the iterator. This is mostly provided for use by
     * the adjacency iterator.
     */
    vertex_descriptor opposite() const
    { return _iter->first; }

    inline basic_in_iterator& operator++()
    { ++_iter; return *this; }

    inline basic_in_iterator& operator--()
    { --_iter; return *this; }

    // Iterators are equivalent if they reference the same edge.
    inline bool operator==(basic_in_iterator const& x) const
    { return **this == *x; }

    inline bool operator!=(basic_in_iterator const& x) const
    { return !this->operator==(x); }

    inline reference operator*() const
    {
        return reference(_iter->first, _iter->second.template get<out_iterator>());
    }

private:
    vertex_descriptor   _base;
    base_iterator       _iter;
};

#endif
