
#ifndef DESCRIPTOR_HPP
#define DESCRIPTOR_HPP

#include <vector>
#include <list>
#include <set>
#include <map>
#include <iterator>
#include <algorithm>

#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/functional/hash.hpp>

// The descriptor library provides an iterator-like reference into containers.
// Unlike iterators, however, descriptors do not iterate and, more importantly,
// are not easily invalidated. Descriptors are only invalidated when the object
// being described are removed from the container. Like iterators, descriptors
// can also be dereferenced, but are also testable for invalidity (unless they
// are invalidated). The cost of additional stability comes in the size of
// the descriptor. They tend to be a couple bytes larger than iterators or
// pointers.

// Descriptors should be const-neutral. Unfortunately, this is easier said than
// done. Because const and non-const iterators cannot be converted to each
// other, descriptors can only wrap iterators (or something similar). Therefore,
// any class exposing a descriptor to a nested container will probably need to
// declare the container as a mutable member. The other method is to const-cast
// the this object getting iterators to the object.

// Descriptors are also equality comparable, less than comparable, and hashable.

// Hash values are provided over a unique identifier "given" to each element in
// the container. The type of this value depends on the implementation of the
// descriptor, but these values are guaranteed to be hashable.

// Comparisons are also based on this unique identifier. Comparing two 
// descriptors is not the same as comparing the objects that they describe.

// Include implementations and their specializations.
#include "descriptor/common.hpp"
#include "descriptor/vector.hpp"

template <typename Container>
struct descriptor
    : descriptor_impl<Container>
{
    /** Create a null descriptor. */
    descriptor()
        : descriptor_impl<Container>()
    { }

    /** Create a descriptor over the given iterator. */
    descriptor(Container& c, typename Container::iterator i)
        : descriptor_impl<Container>(c, i)
    { }
};

template <typename C>
inline std::size_t
hash_value(descriptor<C> const& d)
{
    using boost::hash_value;
    return hash_value(d.index()); 
}

#endif
