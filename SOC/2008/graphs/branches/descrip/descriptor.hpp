
#ifndef DESCRIPTOR_HPP
#define DESCRIPTOR_HPP

// Pull the container traits.
#include "container.hpp"

// Some descriptors are built on blobs.
#include "blob.hpp"

// Descriptor implementations.
#include "descriptor/node.hpp"
#include "descriptor/index.hpp"

// Descriptors Take 2 (or 3 or 4).
//
// Note that we can't build descriptors on template names alone becaue we can't
// guarantee that arbitrary template parameters won't change the size of the
// iterator (e.g., comparators, allocators, or other policies). We should be
// able to say that the size of the iterator must NOT change with respect to
// the contained type.
//
// Basically, what I'm trying to say is that a descriptor type can be applied
// to every vector for which its value type is compatible - which is a really
// odd thing to think about.

// Operations on descriptors:
// This is the minimum interface supported by all containers that support
// descriptors. These operators are all constant (or at least constant with
// respect to the size of the container).
//
// --- get_descriptor(container, iterator) ---
// Given a container and a valid iterator, generate a descriptor to the value
// pointed at by the iterator.
//
// --- get_iterator(container, descriptor) ---
// Given a container and valid descriptor, return an iterator to the described
// value.

// Various Notes...
// What's the real tradeoff for using template template parameters versus just
// template parameters. For example:
// template <typename T, template <typename> class Alloc = std::allocator>
// template <typename T, typename Alloc = std::allocator<T>
//
// The first version decouples the specification of the allocator from the type
// that it will eventually be applied to. The downside is that it restricts the
// allocator to being parameterized over a single parameter. It's possible that
// the allocator is actually parameterized over several different policies or
// strategies. Of course, these policies can be reduced to a single template
// using inheritance and/or template aliases.

// Operator Stability Tags
// These tags define the stability of container's descriptors under insert and
// remove operations. Note that unstable operations should not be used in
// relational data structures (like graphs) since these operations will
// invalidate all outstanding descriptors. There are only two mutating
// operations worth describing: insert and remove. A container that is unstable
// for both operations has unstable mutators.
//
// Note that it is certainly possible for a container to be unstable for both
// insertions and removals. These data structures are generally constructed over
// a known set of objects and are generally immutable afterwards.
struct stable_descriptor_tag { };
struct unstable_insert_tag { };
struct unstable_remove_tag { };
struct unstable_mutators_tag : unstable_insert_tag, unstable_remove_tag { };

/**
 * This metafunction contains some basic traits related to descriptors of
 * containers. This structure must be specialized by each container. By default
 * the types and properties of this class reference nested types of the
 * container, which are basically doomed to fail.
 *
 * This inherits a specialized version of container traits from boost/pending.
 */
template <typename Container>
struct extended_container_traits
{
    typedef typename Container::descriptor_type descriptor_type;
    typedef typename Container::descriptor_stability descriptor_stability;
};


template <typename Container>
inline typename extended_container_traits<Container>::descriptor_type
make_descriptor(Container& c, typename Container::iterator i)
{ return typename extended_container_traits<Container>::descriptor_type(c, i); }

template <typename Container>
inline typename Container::iterator
make_iterator(Container& c, typename extended_container_traits<Container>::descriptor_type d)
{ return d.get(c); }

/** Return the descriptor stability tag for the given container. */
template <typename Container>
inline typename extended_container_traits<Container>::descriptor_stability
descriptor_stability(Container const&)
{ return typename extended_container_traits<Container>::descriptor_stability(); }


// Pull specializations and metafunctions.
#include "descriptor/functions.hpp"
#include "descriptor/vector.hpp"
#include "descriptor/list.hpp"
#include "descriptor/set.hpp"
#include "descriptor/multiset.hpp"
#include "descriptor/map.hpp"
#include "descriptor/multimap.hpp"

#endif
