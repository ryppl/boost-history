// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_NAMES_HPP_INCLUDED
#define BOOST_IDL_NAMES_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/call_traits.hpp>
#include <boost/interfaces/detail/config.hpp> // max arity
#include <boost/mpl/at.hpp>
#include <boost/mpl/aux_/template_arity.hpp>
#include <boost/mpl/size.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/type_traits/function_arity.hpp>
#include <boost/type_traits/function_arguments.hpp>
#include <boost/type_traits/function_result.hpp>

namespace boost { namespace interfaces {  

#define BOOST_IDL_MEM_FUN_NTH_PARAM(z, n, Sig) \
    BOOST_PP_COMMA_IF(n) \
    typename ::boost::nth_function_argument_c<Sig, n>::type \
    BOOST_PP_CAT(p, n) \
    /**/

#define BOOST_IDL_TEMPLATE_NTH_ARG(z, n, Seq) \
    BOOST_PP_COMMA_IF(n) typename ::boost::mpl::at_c<Seq, n>::type
    /**/

#define BOOST_IDL_MEM_FUN_IMPL(z, n, name) \
    template<typename Sig, typename Impl> \
    struct member_function<Sig, Impl, n> { \
      Ret name(BOOST_PP_REPEAT ## z(n, BOOST_IDL_MEM_FUN_NTH_PARAM, _)) \
      { \
        return Impl()( *this BOOST_PP_COMMA_IF(n) \
                       BOOST_PP_ENUM_PARAMS(n, p) ); \
      } \
    }; \
    /**/

#define BOOST_IDL_CONST_MEM_FUN_IMPL(z, n, name) \
    template<typename Sig, typename Impl> \
    struct const_member_function<Sig, Impl, n> { \
      Ret name(BOOST_PP_REPEAT ## z(n, BOOST_IDL_MEM_FUN_NTH_PARAM, _)) const \
      { \
        return Impl()( *this BOOST_PP_COMMA_IF(n) \
                       BOOST_PP_ENUM_PARAMS(n, p) ); \
      } \
    }; \
    /**/

#define BOOST_IDL_STATIC_MEM_FUN_IMPL(z, n, name) \
    template<typename Sig, typename Impl> \
    struct static_member_function<Sig, Impl, n> { \
      static Ret name(BOOST_PP_REPEAT ## z(n, BOOST_IDL_MEM_FUN_NTH_PARAM, _)) \
      { \
        return Impl()(BOOST_PP_ENUM_PARAMS(n, p)); \
      } \
    }; \
    /**/

#define BOOST_IDL_SYN_MEMBER_TEMPLATE_IMPL(z, n, name) \
    template<typename Predicate> \
    struct member_template<Predicate, n> { \
        template<BOOST_PP_ENUM_PARAM(n, typename P)> \
        struct name \
            : Predicate::template apply<BOOST_PP_ENUM_PARAM(n, P)>
            { }; \
    }; \
    /**/

#define BOOST_IDL_ACCESS_MEMBER_TEMPLATE_IMPL(z, n, name) \
    template<typename T, typename Params> \
    struct member_template<T, Params, n> { \
        typedef T::template \
                name<BOOST_PP_REPEAT ## z(n, BOOST_IDL_TEMPLATE_NTH_ARG, _)> \
                type; \
    }; \
    /**/

#define BOOST_IDL_NAME(name) \
    struct name { \
        struct synthesis { \
            template<typename T> \
            struct member_type { typedef T name; }; \
            template< typename Predicate, \
                      int Arity = \
                          mpl::aux::template_arity<Predicate>::type::value > \
            struct member_template; \
            template<typename T> \
            struct member_data { T name; }; \
            template< typename Sig, typename Impl, \
                      int arity = function_arity<Sig>::value > \
            struct member_function; \
            template< typename Sig, typename Impl, \
                      int arity = function_arity<Sig>::value > \
            struct const_member_function; \
            template< typename Sig, typename Impl, \
                      int Arity = function_arity<Sig>::value > \
            struct static_member_function; \
            BOOST_PP_REPEAT( BOOST_IDL_MEM_FUN_IMPL, \
                             BOOST_IDL_MAX_FUNCTION_ARITY, \
                             name ) \
            BOOST_PP_REPEAT( BOOST_IDL_CONST_MEM_FUN_IMPL, \
                             BOOST_IDL_MAX_FUNCTION_ARITY, \
                             name ) \
            BOOST_PP_REPEAT( BOOST_IDL_STATIC_MEM_FUN_IMPL, \
                             BOOST_IDL_MAX_FUNCTION_ARITY, \
                             name ) \
            BOOST_PP_REPEAT( BOOST_IDL_SYN_MEMBER_TEMPLATE_IMPL, \
                             BOOST_IDL_MAX_TEMPLATE_ARITY, \
                             name ) \
        }; \
        struct access { \
            template<typename T> \
            struct member_type { typedef typename T::name type; }; \
            template< typename T, typename Params, \
                      int Size = mpl::size<Params>::value > \
            struct member_template; \
            template<typename Ret, typename T> \
            typename call_traits<Ret>::reference \
            member_data(T& t) { return t.name; } \
            template<typename Ret, typename T> \
            typename call_traits<Ret>::const_reference \
            member_data(const T& t) { return t.name; } \
            template<typename Ret, typename T> \
            Ret pointer_to_member() \
            { return static_cast<Ret>(&T::name); } \
            BOOST_PP_REPEAT( BOOST_IDL_ACCESS_MEMBER_TEMPLATE_IMPL, \
                             BOOST_IDL_MAX_TEMPLATE_ARITY, \
                             name ) \
        }; \
        struct query { }; \
    }; \

} } // End namespaces interfaces, boost.

#endif // #ifndef BOOST_IDL_NAMES_HPP_INCLUDED
