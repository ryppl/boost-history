
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

    node_descriptor()
        : value()
    { }

    node_descriptor(descriptor_type const& x)
        : value(x)
    { }

    inline bool is_null()
    { return value == descriptor_type(); }

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
    //@}

    descriptor_type value;
};

template <typename Container>
inline bool
is_null(node_descriptor<Container> const d)
{ return d.value == typename node_descriptor<Container>::descriptor_type(); }

#endif
