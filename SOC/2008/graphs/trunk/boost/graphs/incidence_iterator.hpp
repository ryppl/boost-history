
#ifndef INCIDENCE_ITERATOR_HPP
#define INCIDENCE_ITERATOR_HPP

/**
 * The incidence iterator is an abstraction over the incidence iterators of
 * a vertice's incidence store. Specifically when dereferenced, these iterators
 * will result in edge descriptors.
 */
template <typename Iter, typename Edge>
class incidence_iterator
{
public:
    typedef Iter iterator;
    typedef typename iterator::value_type base_value_type;
    typedef typename base_value_type::first_type vertex_descriptor;
    typedef typename base_value_type::second_type property_descriptor;

    // This is a little misleading. This iterator can be either bidi or random.
    // Clearly, we're going to be constraining members using some concept stuff
    // when it becomes available.
    typedef typename iterator::iterator_category iterator_category;
    typedef typename iterator::difference_type difference_type;

    typedef Edge value_type;
    typedef value_type reference;
    typedef value_type pointer;

    inline incidence_iterator()
        : base(), iter()
    { }

    inline incidence_iterator(vertex_descriptor v, iterator const& x)
        : base(v), iter(x)
    { }

    /**
     * Extended iterator functionality. Return the source vertex of the
     * iterator. This is the vertex for which the iterator was originally
     * created.
     */
    inline vertex_descriptor source() const
    { return base; }

    /**
     * Extended iterator functionality. Return the opposite vertex of the edge
     * indicated by the iterator. This is mostly provided for use by the
     * adjacency iterator.
     */
    inline vertex_descriptor opposite() const
    { return iter->first; }

    inline incidence_iterator& operator++()
    { ++iter; return *this; }

    inline incidence_iterator operator++(int)
    { incidence_iterator tmp(*this); ++iter; return tmp; }

    inline incidence_iterator& operator--()
    { --iter; return *this; }

    inline incidence_iterator operator--(int)
    { incidence_iterator tmp(*this); --iter; return tmp; }

    // Incidence iterators are equivalent if they have the same source and are
    // reference the same incident edge.
    inline bool operator==(incidence_iterator const& x) const
    { return (base == x.base) && (iter == x.iter); }

    inline bool operator!=(incidence_iterator const& x) const
    { return !this->operator==(x); }

    // This can access invalid memory if the iterator is at the end.
    inline reference operator*() const
    { return Edge(base, iter->first, iter->second); }

    vertex_descriptor   base;
    iterator            iter;
};


#endif
