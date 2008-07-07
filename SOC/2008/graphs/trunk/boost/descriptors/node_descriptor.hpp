
#ifndef VECTOR_DESCRIPTOR_HPP
#define VECTOR_DESCRIPTOR_HPP

/**
 * The node descriptor contains an iterator into the target container. The
 * container iterator is just a pattern for the actual iterator and has no
 * real type (it's opaque).
 */
template <typename Blob>
struct node_descriptor
{
    typedef Blob descriptor_type;

    inline node_descriptor()
        : value()
    { }

    inline node_descriptor(descriptor_type const& x)
        : value(x)
    { }

    template <typename Container>
    inline node_descriptor(Container&, typename Container::iterator i)
        : value(i)
    { }

    /** Return true if the descriptor is null. */
    inline bool is_null() const
    { return value == descriptor_type(); }

    /** Cast to bool tests to see if the descritpor is null. */
    inline operator bool() const
    { return !is_null(); }

    /** @name Equality Comparable */
    //@{
    inline bool operator==(node_descriptor const& x)
    { return value == x.value; }

    inline bool operator!=(node_descriptor const& x)
    { return value != x.value; }
    //@}

    /** @name Less Than Comparable */
    //@{
    inline bool operator<(node_descriptor const& x)
    { return value < x.value; }

    inline bool operator>(node_descriptor const& x)
    { return value > x.value; }

    inline bool operator<=(node_descriptor const& x)
    { return value <= x.value; }

    inline bool operator>=(node_descriptor const& x)
    { return value >= x.value; }
    //@}

    template <typename Container>
    inline typename Container::iterator get(Container& c) const
    { return value.get<typename Container::iterator>(); }

    descriptor_type value;
};

#endif
