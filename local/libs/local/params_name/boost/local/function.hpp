
#ifndef BOOST_LOCAL_FUNCTION_HPP_
#define BOOST_LOCAL_FUNCTION_HPP_

#include "aux_/function/params.hpp"
#include <boost/preprocessor/facilities/empty.hpp>
#include <boost/config.hpp> // Handle variadic macros.

// Params.

// Pass a parenthesized params seq `()()...` on C++ but if variadics (C99
// preprocessor only) you can also pass a variable length tuple `..., ...`.
#if defined(BOOST_NO_VARIADIC_MACROS)
#   define BOOST_LOCAL_FUNCTION_PARAMS(parenthesized_params) \
        BOOST_LOCAL_AUX_FUNCTION_PARAMS(parenthesized_params, BOOST_PP_EMPTY()) 
#else
#   include "aux_/preprocessor/va/to_seq.hpp"
#   define BOOST_LOCAL_FUNCTION_PARAMS(...) \
        BOOST_LOCAL_AUX_FUNCTION_PARAMS( \
                BOOST_LOCAL_AUX_PP_VA_TO_SEQ(__VA_ARGS__), BOOST_PP_EMPTY())
#endif

#endif // #include guard

