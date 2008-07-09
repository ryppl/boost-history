
#ifndef OUT_ITERATOR_HPP
#define OUT_ITERATOR_HPP

/**
 * The out edge iterator is a wrapper around out store iterators that, when
 * dereferenced will return an edge descriptor. The properties of the iterator
 * are taken from the underlying store.
 */
template <typename Store, typename Edge>
class basic_out_iterator
{
    typedef Store store_type;
    typedef typename Store::iterator iterator;
    typedef typename iterator::value_type base_value_type;
    typedef typename base_value_type::first_type vertex_descriptor;
    typedef typename base_value_type::second_type edge_pair;
    typedef typename edge_pair::first_type edge_properties;
    typedef typename edge_pair::second_type in_descriptor;

    // This is a little misleading. This iterator can be either bidi or random.
    // Clearly, we're going to be constraining members using some concept stuff
    // when it becomes available.
    typedef typename iterator::iterator_category iterator_category;
    typedef typename iterator::difference_type difference_type;
    typedef Edge value_type;
    typedef value_type reference;
    typedef value_type pointer;

    inline basic_out_iterator()
        : src(), iter(), outs(0)
    { }

    inline basic_out_iterator(vertex_descriptor v, iterator x, store_type& store)
        : src(v), iter(x), outs(&store)
    { }

    /** Return the source vertex of the iterated edge. */
    vertex_descriptor source() const
    { return src; }

    /** Return the target vertex of the iterated edge. */
    vertex_descriptor target() const
    { return iter->first; }

    inline basic_out_iterator& operator++()
    { ++iter; return *this; }

    inline basic_out_iterator& operator--()
    { --iter; return *this; }

    /** @name Equality Comparable */
    //@{
    inline bool operator==(basic_out_iterator const& x) const
    { return iter == x.iter; }

    inline bool operator!=(basic_out_iterator const& x) const
    { return iter != x.iter; }
    //@}

    inline reference operator*() const
    { return Edge(source(), target(), outs->edge(iter)); }

    vertex_descriptor   src;
    iterator            iter;
    store_type*         outs;
};

#endif
