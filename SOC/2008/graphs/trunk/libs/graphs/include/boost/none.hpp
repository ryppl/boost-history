
#ifndef BOOST_NONE_HPP
#define BOOST_NONE_HPP

#include <boost/utility.hpp>

namespace boost {

/**
 * The canonical none type.
 * @todo This conflicts mightily with the standard boost none, and none_t. We
 * should not even be using this.
 */
struct none { };

/** Like none, but not. */
struct unused { };

// Traits for the none type
template <typename T>
struct is_none { BOOST_STATIC_CONSTANT(bool, value = false); };

template <>
struct is_none<none> { BOOST_STATIC_CONSTANT(bool, value = true); };

template <typename T>
struct is_not_none { BOOST_STATIC_CONSTANT(bool, value = !is_none<T>::value); };

template <>
struct is_not_none<none> { BOOST_STATIC_CONSTANT(bool, value = !is_none<none>::value); };

} /* namespace boost */

#endif

