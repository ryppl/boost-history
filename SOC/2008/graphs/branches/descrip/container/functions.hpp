
#ifndef CONTAINER_FUNCTIONS_HPP
#define CONTAINER_FUNCTIONS_HPP

#include <boost/type_traits.hpp>

/**
 * Evaluates to true if the container contains unique elements.
 */
template <typename Container>
struct contains_unique_elements
{
    static bool const value =
        boost::is_convertible<
            typename container_traits<Container>::category,
            unique_associative_container_tag
        >::value;
};

#endif
