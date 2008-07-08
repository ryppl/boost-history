
#ifndef NONE_HPP
#define NONE_HPP

#include <boost/utility.hpp>

/** The canonical none type. */
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

#endif

