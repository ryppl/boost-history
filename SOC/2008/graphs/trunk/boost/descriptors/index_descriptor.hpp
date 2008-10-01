
#ifndef INDEX_DESCRIPTOR_HPP
#define INDEX_DESCRIPTOR_HPP

#include <boost/next_prior.hpp>
#include <boost/functional/hash.hpp>

/**
 * The index_descriptor simply maintains the descriptor as an index into the
 * given offset.
 */
template <typename Index, typename Kind = basic_descriptor_kind>
struct index_descriptor
{
    typedef index_descriptor<Index, Kind> this_type;
    typedef Index this_type::*unspecified_bool_type;

    typedef Kind descriptor_kind;
    typedef Index descriptor_type;

    inline index_descriptor()
        : value(-1)
    { }

    inline index_descriptor(descriptor_type d)
        : value(d)
    { }

    template <typename Container>
    inline index_descriptor(Container& c, typename Container::iterator i)
        : value(std::distance(c.begin(), i))
    { }

    /** Return true if the descriptor is null. */
    inline bool is_null() const
    { return value == descriptor_type(-1); }

    /** Cast to bool tests to see if the descritpor is null. */
    inline operator unspecified_bool_type() const
    { return is_null() ? 0 : &this_type::value; }

    /** @name Equality Comparable */
    //@{
    inline bool operator==(index_descriptor const& x) const
    { return value == x.value; }

    inline bool operator!=(index_descriptor const& x) const
    { return value != x.value; }
    //@}

    /** @name Less Than Comparable */
    //@{
    inline bool operator<(index_descriptor const& x) const
    { return value < x.value; }

    inline bool operator>(index_descriptor const& x) const
    { return value > x.value; }

    inline bool operator<=(index_descriptor const& x) const
    { return value <= x.value; }

    inline bool operator>=(index_descriptor const& x) const
    { return value >= x.value; }
    //@}

    inline void swap(index_descriptor& x)
    { std::swap(value, x); }

    template <typename Container>
    inline typename Container::iterator get(Container& c) const
    { return boost::next(c.begin(), value); }

    descriptor_type value;
};

// A hash function for indexed descriptors.
template <typename Index, typename Kind>
std::size_t hash_value(index_descriptor<Index, Kind> const& x)
{
    using boost::hash_value;
    return hash_value(x.value);
}

template <typename Index, typename Kind>
std::ostream& operator<<(std::ostream& os, index_descriptor<Index, Kind> const& x)
{ return os << x.value; }

#endif
