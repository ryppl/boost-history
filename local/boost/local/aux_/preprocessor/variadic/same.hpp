
#ifndef BOOST_LOCAL_AUX_PP_VARIADIC_SAME_HPP_
#define BOOST_LOCAL_AUX_PP_VARIADIC_SAME_HPP_

#include <boost/config.hpp>

#if !defined(BOOST_NO_VARIADIC_MACROS) // If no variadics then no macros.

#define BOOST_LOCAL_AUX_PP_VARIADIC_SAME(...) __VA_ARGS__

#endif // BOOST_NO_VARIADIC_MACROS

#endif // #include guard

