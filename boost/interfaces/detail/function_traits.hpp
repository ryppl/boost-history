// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_FUNCTION_TRAITS_HPP_INCLUDED
#define BOOST_IDL_FUNCTION_TRAITS_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/config.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/preprocessor/iteration/local.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>

#ifndef BOOST_IDL_FUNCTION_TRAITS_MAX_ARITY
# define BOOST_IDL_FUNCTION_TRAITS_MAX_ARITY 10
#endif

namespace boost { namespace interfaces { namespace detail {

template<typename T>
struct function_info;

#define BOOST_PP_LOCAL_MACRO(n) \
    template< typename Result BOOST_PP_COMMA_IF(n) \
              BOOST_PP_ENUM_PARAMS(n, typename Arg)> \
    struct function_info<Result(BOOST_PP_ENUM_PARAMS(n, Arg))> { \
        static const int                                   arity = n; \
        typedef Result                                     result; \
        typedef mpl::vector<BOOST_PP_ENUM_PARAMS(n, Arg)>  arguments; \
        typedef Result (*add_void_pointer) \
            (void* BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, Arg)); \
    }; \
    /**/
#define BOOST_PP_LOCAL_LIMITS (0, BOOST_IDL_FUNCTION_TRAITS_MAX_ARITY)
#include BOOST_PP_LOCAL_ITERATE()

template<typename T>
struct function_arity
    : mpl::int_<function_info<T>::arity>
    { };

template<typename T>
struct function_result {
    typedef typename function_info<T>::result type; 
};

template<typename T>
struct function_arguments {
    typedef typename function_info<T>::arguments type; 
};

template<typename T, typename N>
struct nth_function_argument 
    : mpl::at<typename function_arguments<T>::type, N>
    { };

template<typename T>
struct add_void_pointer {
    typedef typename function_info<T>::add_void_pointer type; 
};

} } } // End namespaces detail, interfaces, boost.

#endif // #ifndef BOOST_IDL_FUNCTION_TRAITS_HPP_INCLUDED
