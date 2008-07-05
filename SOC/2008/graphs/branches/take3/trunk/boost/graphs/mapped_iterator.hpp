
#ifndef MAPPED_ITERATOR_HPP
#define MAPPED_ITERATOR_HPP

/**
 * A map iterator adapter that returns references to the mapped edge.
 */
template <typename Iter>
struct mapped_iterator
{
    typedef typename Iter::iterator_category iterator_category;
    typedef typename Iter::difference_type difference_type;

    // Deconstruct and reconstruct the value type. Yikes.
    typedef typename Iter::value_type::second_type value_type;
    typedef value_type& reference;
    typedef value_type* pointer;

    inline mapped_iterator()
        : iter()
    { }

    inline mapped_iterator(Iter x)
        : iter(x)
    { }

    inline mapped_iterator& operator++()
    { ++iter; return *this; }

    inline mapped_iterator& operator--()
    { --iter; return *this; }

    inline bool operator==(mapped_iterator const& x) const
    { return iter == x.iter; }

    inline bool operator!=(mapped_iterator const& x) const
    { return !operator==(x); }

    inline reference operator*()
    { return iter->second; }

    inline const reference operator*() const
    { return iter->second; }

    inline pointer operator->()
    { return &iter->second; }

    inline const pointer operator->() const
    { return &iter->second; }

    Iter iter;
};

#endif
