
#ifndef VERTEX_DESCRIPTOR_HPP
#define VERTEX_DESCRIPTOR_HPP

#include <iosfwd>

#include <boost/functional/hash.hpp>

// Important notes about descriptors
//
// A descriptor is basically an opaque reference to an object. It's kind of
// like an iterator that can't be moved or dereferenced. It's kind of like a
// flyweight, but you can't implicitly cast it as the shared object. In short,
// you can't use descriptors without their graph. In general, we'd like the
// descriptor types to be as trivial as possible. One of the most important
// facts about descriptors is that, by themselves, they have absolutely no
// semantics. They cannot be used (in general) be used to access the vertices
// or edges that they represent.
//
// The most important thing to understand about descriptors is that they attempt
// to model the most "consistent" reference into a storage mechanism. By the
// term "consistent", we mean a means of accessing elements that allow us to
// actually build a graph without invalidating memory or iterators. For example,
// we can't use pointers as descriptors into vectors since vectors occasionally
// reallocate all their memory (oops), but indices work just fine.
//
// Descriptors must be completely independent of the actual type of vertices
// and edges. Two problems arise if we don't do this. First, we end up with
// weird cyclic type dependencies that can probably be unrolled using some
// funky lazy template evaluation, but that's generally beyond me. Second,
// this would

// Build trivial wrappers around the underlying descriptor types. This allows
// us to differentiate descriptors based on these types rather than their
// simple descriptor types.

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
    inline bool is_valid() const
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

