
#ifndef DESCRIPTORS_HPP
#define DESCRIPTORS_HPP

// Pull the container traits.
#include "containers.hpp"

// Some descriptors are built on blobs.
#include "blob.hpp"

// A default "responsibility" tag. Because descriptors are mostly container-
// neutral, debugging intermixed descriptors can be a painful and error-prone.
// We use responsibility tags to imbue descriptors with semantics.
struct basic_descriptor_kind { };

// Descriptor implementations.
#include "descriptors/node_descriptor.hpp"
#include "descriptors/index_descriptor.hpp"

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

// Mutator Stability Tags
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

// Kind of concepts
struct stable_mutators_tag { };
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
template <typename Container, typename Kind = basic_descriptor_kind>
struct descriptor_traits
{
    typedef Kind descriptor_kind;
    typedef typename Container::descriptor_type descriptor_type;
    typedef typename Container::mutator_stability mutator_stability;
};

/**
 * Given a container and a valid iterator into the container, return a
 * descriptor to the object being pointed at. The descriptor is guaranteed to
 * be at least as long as the iterator, generally longer. If the given iterator
 * is past the end of the container, then the returned descriptor is null.
 */
template <typename Container>
inline typename descriptor_traits<Container>::descriptor_type
make_descriptor(Container& c, typename Container::iterator i)
{
    typedef typename descriptor_traits<Container>::descriptor_type result_type;
    return i != c.end() ? result_type(c, i) : result_type();
}

/**
 * Given a container and a valid descriptor, return an iterator pointing to the
 * described object. The iterator will be valid as long as the descriptor has
 * not been invalidated (e.g., removing an item from a vector). If the given
 * descriptor is null, then the returned iterator is past the end of the
 * container.
 */
template <typename Container>
inline typename Container::iterator
make_iterator(Container& c, typename descriptor_traits<Container>::descriptor_type d)
{ return d ? d.get(c) : c.end(); }


/** Return the descriptor stability tag for the given container. */
template <typename Container>
inline typename descriptor_traits<Container>::mutator_stability
mutator_stability(Container const&)
{ return typename descriptor_traits<Container>::mutator_stability(); }

// Metafunctions

/**
 * Returns true if the cotnainer supports insert operations that do not
 * invalidate outstanding descriptors.
 */
template <typename Container>
struct has_insert_mutator
{
    // True if not convertible to unstable_insert_tag.
    static bool const value =
        !boost::is_convertible<
            typename descriptor_traits<Container>::descriptor_stability,
            unstable_insert_tag
        >::value;
};

/**
 * Returns true if the container supports remove operations that do not
 * invalidate outstanding descriptors.
 */
template <typename Container>
struct has_remove_mutator
{
    // True if not convertible to unstable_remove_tag.
    static bool const value =
        !boost::is_convertible<
            typename descriptor_traits<Container>::descriptor_stability,
            unstable_remove_tag
        >::value;
};

// Specializations

// Vector
template <typename T, typename Alloc, typename Kind>
struct descriptor_traits<std::vector<T, Alloc>, Kind>
{
    typedef Kind descriptor_kind;
    typedef index_descriptor<typename std::vector<T, Alloc>::size_type, Kind> descriptor_type;
    typedef unstable_remove_tag descriptor_stability;
};

// List
template <typename T, typename Alloc, typename Kind>
struct descriptor_traits<std::list<T, Alloc>, Kind>
{
    typedef Kind descriptor_kind;
    typedef node_descriptor<blob<sizeof(typename std::list<T, Alloc>::iterator)>, Kind> descriptor_type;
    typedef stable_mutators_tag descriptor_stability;
};

// TODO: Dequeue

// Set
template <typename T, typename Comp, typename Alloc, typename Kind>
struct descriptor_traits<std::set<T, Comp, Alloc>, Kind>
{
    typedef Kind descriptor_kind;
    typedef node_descriptor<blob<sizeof(typename std::set<T, Comp, Alloc>::iterator)>, Kind> descriptor_type;
    typedef stable_mutators_tag descriptor_stability;
};

// Multiset
template <typename T, typename Comp, typename Alloc, typename Kind>
struct descriptor_traits<std::multiset<T, Comp, Alloc>, Kind>
{
    typedef Kind descriptor_kind;
    typedef node_descriptor<blob<sizeof(typename std::multiset<T, Comp, Alloc>::iterator)>, Kind> descriptor_type;
    typedef stable_mutators_tag descriptor_stability;
};

// Map
template <typename Key, typename T, typename Comp, typename Alloc, typename Kind>
struct descriptor_traits<std::map<Key, T, Comp, Alloc>, Kind>
{
    typedef Kind descriptor_kind;
    typedef node_descriptor<blob<sizeof(typename std::map<Key, T, Comp, Alloc>::iterator)>, Kind> descriptor_type;
    typedef stable_mutators_tag descriptor_stability;
};

// Multimap
template <typename Key, typename T, typename Comp, typename Alloc, typename Kind>
struct descriptor_traits<std::multimap<Key, T, Comp, Alloc>, Kind>
{
    typedef Kind descriptor_kind;
    typedef node_descriptor<blob<sizeof(typename std::multimap<Key, T, Comp, Alloc>::iterator)>, Kind> descriptor_type;
    typedef stable_mutators_tag descriptor_stability;
};

// TODO: Unordered Set
// TODO: Unordered Multiset
// TODO: Unordered Map
// TODO: Unordered Multimap


#endif
