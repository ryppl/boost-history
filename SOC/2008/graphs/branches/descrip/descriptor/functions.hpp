
#ifndef DESCRIPTOR_FUNCTIONS_HPP
#define DESCRIPTOR_FUNCTIONS_HPP

#include <boost/type_traits.hpp>

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
            typename extended_container_traits<Container>::descriptor_stability,
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
            typename extended_container_traits<Container>::descriptor_stability,
            unstable_remove_tag
        >::value;
};

#endif
