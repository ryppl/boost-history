
#ifndef BOOST_LOCAL_AUX_PP_IS_VARIADIC_HPP_
#define BOOST_LOCAL_AUX_PP_IS_VARIADIC_HPP_

#include <boost/config.hpp>

#if !defined(BOOST_NO_VARIADIC_MACROS) // If no variadics then no macros.

#include "size.hpp"
#include "eat.hpp"
#include <boost/preprocessor/logical/not.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/comparison/greater.hpp>
#include <boost/preprocessor/facilities/is_unary.hpp>

// PRIVATE //

#define BOOST_LOCAL_AUX_PP_IS_VARIADIC_NOT_UNARY_(...) \
    BOOST_PP_NOT(BOOST_PP_IS_UNARY(__VA_ARGS__))

// PUBLIC //

#define BOOST_LOCAL_AUX_PP_IS_VARIADIC(...) \
    BOOST_PP_IF(BOOST_PP_GREATER( /* IIF does not expand on MSVC */ \
            BOOST_LOCAL_AUX_PP_VARIADIC_SIZE(__VA_ARGS__), 1), \
        1 BOOST_LOCAL_AUX_PP_VARIADIC_EAT \
    , \
        BOOST_LOCAL_AUX_PP_IS_VARIADIC_NOT_UNARY_ \
    )(__VA_ARGS__)

#endif // BOOST_NO_VARIADIC_MACROS

#endif // #include guard


