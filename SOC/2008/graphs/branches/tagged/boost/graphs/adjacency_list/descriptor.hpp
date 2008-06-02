
#ifndef BOOST_GRAPHS_ADJACENCY_LIST_DESCRIPTOR_HPP
#define BOOST_GRAPHS_ADJACENCY_LIST_DESCRIPTOR_HPP

#include <boost/functional/hash.hpp>

namespace boost {
namespace graphs {
namespace adj_list {

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


// Convenience functions for building a default descriptor depending on the
// underlying descriptor type.
inline std::size_t default_descriptor(std::size_t)
{ return std::size_t(-1); }

inline void* default_descriptor(void*)
{ return 0; }

template <typename D>
struct descriptor
{
    inline descriptor()
        : desc(default_descriptor(D()))
    { }

    inline descriptor(D d)
        : desc(d)
    { }

    // Assignment copy.
    inline descriptor& operator=(descriptor const& d)
    {
        desc = d.desc;
        return *this;
    }

    // value-type assignment.
    inline descriptor& operator=(D d)
    {
        desc = d;
        return *this;
    }

    // Just to make the access a little prettier.
    inline D get() const
    { return desc; }

    // Provides an implicit cast to bool. Returns true if the descriptor
    // is not the same as its default value.
    inline bool is_valid() const
    { return desc != descriptor().desc; }

    inline bool operator<(descriptor const& d) const
    { return desc < d.desc; }

    inline bool operator==(descriptor const& d) const
    { return desc == d.desc; }

    inline bool operator!=(descriptor const& d) const
    { return desc != d.desc; }

    D desc;
};

// Hash value specializatins for the descriptor types.
template <typename D>
inline std::size_t hash_value(descriptor<D> const& d)
{
    return hash_value(d.get());
}

// Apparently, there's not a built-in hash for void ptrs.
template <>
inline std::size_t hash_value(descriptor<void*> const& d)
{
    return reinterpret_cast<std::size_t>(d.get());
}

/**
 * Wraps a vertex descriptor. Valid template argument are either size_t or
 * void*.
 */
template <typename D>
struct vertex_desc
    : public descriptor<D>
{
    inline vertex_desc()
        : descriptor<D>()
    { }

    inline vertex_desc(D d)
        : descriptor<D>(d)
    { }
};

/**
 * Wraps an edge descriptor. Valid template arguments are either size_t or
 * void*.
 */
template <typename D>
struct edge_desc
    : public descriptor<D>
{
    inline edge_desc()
        : descriptor<D>()
    { }

    inline edge_desc(D d)
        : descriptor<D>(d)
    { }
};


} /* namespace adj_list */
} /* namespace graphs */
} /* namespace boost */

#endif
