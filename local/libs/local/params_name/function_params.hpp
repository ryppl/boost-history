
#ifndef BOOST_LOCAL_FUNCTION_PARAMS_HPP_
#define BOOST_LOCAL_FUNCTION_PARAMS_HPP_

#include "aux_/function/params.hpp"
#include <boost/config.hpp>

// Pass a parenthesized params seq `()()...` on C++ but if variadics (C99
// preprocessor only) you can also pass a variable length tuple `..., ...`.
#if defined(BOOST_NO_VARIADIC_MACROS)
#   define BOOST_LOCAL_FUNCTION_PARAMS(parenthesized_params) \
        BOOST_LOCAL_AUX_FUNCTION_PARAMS(parenthesized_params) 
#else
#   include "aux_/preprocessor/va/to_seq.hpp"
#   define BOOST_LOCAL_FUNCTION_PARAMS(...) \
        BOOST_LOCAL_AUX_FUNCTION_PARAMS( \
                BOOST_LOCAL_AUX_PP_VA_TO_SEQ(__VA_ARGS__))
#endif

#endif // #include guard

