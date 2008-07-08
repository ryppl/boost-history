
#ifndef VERTEX_DESCRIPTOR_HPP
#define VERTEX_DESCRIPTOR_HPP

#include <iosfwd>

#include <boost/functional/hash.hpp>

namespace detail
{
    inline std::size_t invalid_value(std::size_t)
    { return std::size_t(-1); }

    void* invalid_value(void*)
    { return 0; }
}

/**
 * The basic vertex descriptor is a wrapper around an opaque reference to a
 * vertex. This class is provided to get around overload problems for functions
 * taking properties or keys when the types of those objects are the same as
 * the underlying descriptor.
 */
template <typename D>
class basic_vertex_descriptor
{
public:
    inline basic_vertex_descriptor()
        : _d(detail::invalid_value(D()))
    { }

    inline basic_vertex_descriptor(basic_vertex_descriptor const& x)
        : _d(x._d)
    { }

    inline basic_vertex_descriptor(D const& d)
        : _d(d)
    { }

    // Assignment copy.
    inline basic_vertex_descriptor& operator=(basic_vertex_descriptor const& d)
    {
        _d = d._d;
        return *this;
    }

    // value-type assignment.
    inline basic_vertex_descriptor& operator=(D d)
    {
        _d = d;
        return *this;
    }

    /** Get the underlying descriptor value. */
    inline D get() const
    { return _d; }

    /** Returns true if the descriptor is valid. */
    inline bool valid() const
    { return _d != detail::invalid_value(D()); }

private:
    D _d;
};

template <typename D>
bool
operator<(basic_vertex_descriptor<D> const& a, basic_vertex_descriptor<D> const& b)
{ return a.get() < b.get(); }

template <typename D>
inline bool
operator==(basic_vertex_descriptor<D> const& a, basic_vertex_descriptor<D> const& b)
{ return a.get() == b.get(); }

template <typename D>
inline bool
operator!=(basic_vertex_descriptor<D> const& a, basic_vertex_descriptor<D> const& b)
{ return a.get() != b.get(); }

template <typename D>
std::ostream& operator<<(std::ostream& os, const basic_vertex_descriptor<D>& v)
{ return os << v.get(); }

/** Compute the hash value of the descriptor. */
template <typename D>
std::size_t hash_value(basic_vertex_descriptor<D> const& x)
{ return boost::hash_value(x.get()); }

#endif

