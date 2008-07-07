
#ifndef NODE_DESCRIPTOR_HPP
#define NODE_DESCRIPTOR_HPP

#include <boost/functional/hash.hpp>

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

// A hash function for indexed descriptors.
template <typename Blob>
std::size_t hash_value(node_descriptor<Blob> const& x)
{
    using boost::hash_value;
    return hash_value(x.value);
}

template <typename Blob>
std::ostream& operator<<(std::ostream& os, node_descriptor<Blob> const& d)
{ return os << hash_value(d); }

#endif
