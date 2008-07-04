
#ifndef INDEX_DESCRIPTOR_HPP
#define INDEX_DESCRIPTOR_HPP

#include <boost/functional/hash.hpp>

/**
 * The index_descriptor simply maintains the descriptor as an index into the
 * given offset.
 */
template <typename Index>
struct index_descriptor
{
    typedef Index descriptor_type;

    inline index_descriptor()
        : value(-1)
    { }

    inline index_descriptor(descriptor_type d)
        : value(d)
    { }

    inline bool is_null() const
    { return value == descriptor_type(-1); }

    /** @name Equality Comparable */
    //@{
    inline bool operator==(index_descriptor const& x)
    { return value == x.value; }

    inline bool operator!=(index_descriptor const& x)
    { return value != x.value; }
    //@}

    /** @name Less Than Comparable */
    //@{
    inline bool operator<(index_descriptor const& x)
    { return value < x.value; }

    inline bool operator>(index_descriptor const& x)
    { return value > x.value; }

    inline bool operator<=(index_descriptor const& x)
    { return value <= x.value; }

    inline bool operator>=(index_descriptor const& x)
    { return value >= x.value; }
    //@}

    descriptor_type value;
};

// A hash function for indexed descriptors.
template <typename Index>
std::size_t hash_value(index_descriptor<Index> const& x)
{
    using boost::hash_value;
    return hash_value(x.value);
}

#endif
