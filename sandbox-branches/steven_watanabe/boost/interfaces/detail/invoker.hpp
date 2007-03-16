// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_INVOKER_HPP_INCLUDED
#define BOOST_IDL_INVOKER_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/config.hpp>             // BOOST_MSVC.
#include <boost/detail/workaround.hpp>

#if BOOST_WORKAROUND(BOOST_MSVC, == 1310)
# pragma warning(push)
# pragma warning(disable:4224) // parameter previouisly defined as type
#endif

#include <exception>
#include <boost/interfaces/access.hpp>
#include <boost/interfaces/advice_categories.hpp>
#include <boost/interfaces/detail/argument_tuple.hpp>
#include <boost/interfaces/detail/finalizer.hpp>
#include <boost/interfaces/detail/function_traits.hpp>
#include <boost/interfaces/detail/return_void.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/less.hpp>
#include <boost/mpl/long.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/iteration/local.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_void.hpp>

namespace boost { namespace interfaces { namespace detail {

//------------------Declarataion of invoker-----------------------------------//

//
// Template Parameters:
//   Interface - The interface whose member function is to be invoked.
//   Offset - The offset within the function table of the function to be 
//      invoked.
//   Signature - The signature of the function to be invoked, represented as a
//      function type.
//   Advice - A Model of the concept Advice.
//    
template< typename Interface, typename Offset, typename Signature,
          typename Category = 
              typename advice_category<
                  typename Interface::interface_metadata::base_type
              >::type >
struct invoker;

//------------------Declarations of helper templates--------------------------//

template< typename Interface, typename Offset, typename Signature,
          int Arity = function_arity<Signature>::value >
struct null_invoker;

template< typename Interface, typename Offset, typename Signature,
          int Arity = function_arity<Signature>::value >
struct basic_invoker;

template< typename Interface, typename Offset, typename Signature,
          int Arity = function_arity<Signature>::value >
struct transforming_invoker;

//------------------Partial specializations of invoker------------------------//

template<typename Interface, typename Offset, typename Signature>
struct invoker<Interface, Offset, Signature, null_advice_tag>
    : null_invoker<Interface, Offset, Signature>
    { };

//template<typename Interface, typename Offset, typename Signature>
//struct invoker<Interface, Offset, Signature, basic_advice_tag>
//    : basic_invoker<Interface, Offset, Signature>
//    { };
//
//template<typename Interface, typename Offset, typename Signature>
//struct invoker<Interface, Offset, Signature, transforming_advice_tag>
//    : transforming_invoker<Interface, Offset, Signature>
//    { };

//----------------------------------------------------------------------------//

//
// Macro: BOOST_IDL_INVOKER_ARGS(arity)
// Expands to: x0, ..., xm, with m = arity - 1.
//     
#define BOOST_IDL_INVOKER_ARGS(arity) BOOST_PP_ENUM_PARAMS(arity, x) 

//
// Macro: BOOST_IDL_INVOKER_PARAMS(signature, arity)
// Parameters: 
//    signature - an identifier denoting a function type
//    arity - the arity of the function type
// Expands to: An argument list of the form 
//
//      , typename nth_function_argument<Signature, 0>::type x0
//      , ... 
//      , typename nth_function_argument<Signature, m>::type xm
//
//    where m is arity - 1.
//     
#define BOOST_IDL_INVOKER_PARAMS_IMPL(z, n, signature) \
    , typename nth_function_argument< signature, mpl::long_<n> >::type \
      BOOST_PP_CAT(x, n) \
    /**/
#define BOOST_IDL_INVOKER_PARAMS(signature, arity) \
    BOOST_PP_REPEAT(arity, BOOST_IDL_INVOKER_PARAMS_IMPL, signature) \
    /**/
                    
//------Local iteration defining specializations of null_invoker--------------//

#define BOOST_PP_LOCAL_MACRO(n) \
    template<typename Interface, typename Offset, typename Signature> \
    struct null_invoker<Interface, Offset, Signature, n> { \
      typedef typename Interface::interface_metadata::derived_type derived; \
      typedef typename add_void_pointer<Signature>::type  function_type; \
      static typename function_result<Signature>::type \
      invoke( const void* pv \
              BOOST_IDL_INVOKER_PARAMS(Signature, n) ) \
      { \
        const derived* self = static_cast<derived*>(const_cast<void*>(pv)); \
        const void* pointer = access::get_interface_pointer(*self); \
        const function_type* f = \
            reinterpret_cast<const function_type*>( \
                access::get_interface_table(*self) + Offset::value \
            ); \
        return (*f)( const_cast<void*>(pointer) BOOST_PP_COMMA_IF(n) \
                     BOOST_IDL_INVOKER_ARGS(n) ); \
      } \
    }; \
    /**/
#define BOOST_PP_LOCAL_LIMITS (0, BOOST_IDL_FUNCTION_TRAITS_MAX_ARITY)
#include BOOST_PP_LOCAL_ITERATE()
#undef BOOST_PP_LOCAL_MACRO
          
//------Local iteration defining specializations of basic_invoker-------------//

#define BOOST_PP_LOCAL_MACRO(n) \
    template< typename Interface, typename Offset, typename Signature, \
              typename Names, typename Advice > \
    struct basic_invoker<Interface, Offset, Signature, Names, Advice, n> { \
      typedef typename add_void_pointer<Signature>::type  function_type; \
      typedef typename function_result<Signature>::type   result_type; \
      typedef typename argument_tuple<Signature>::type    tuple_type; \
      typedef ::boost::interfaces::detail::finalizer<Advice, tuple_type> finalizer_type; \
      typedef typename finalizer_type::after_type         after_type; \
      typedef typename finalizer_type::finally_type       finally_type; \
      static const char* function_name() { return Names::function_name(); } \
      static const char** param_names() { return Names::param_names(); } \
      static result_type execute( const void* pv \
                                  BOOST_IDL_INVOKER_PARAMS(Signature, n) ) \
      { \
        const Interface* self = \
            static_cast<Interface*>(const_cast<void*>(pv)); \
        const Advice*    advice        = static_cast<const Advice*>(self); \
        after_type       after         = &Advice::after; \
        finally_type     finally       = &Advice::finally; \
        tuple_type       tuple         = tuple_type(BOOST_IDL_INVOKER_ARGS(n)); \
        finalizer_type   finalizer( advice, after, finally, tuple, \
                                   function_name(), n, param_names() ); \
        const void* pointer = \
          access::get_interface_pointer(*self); \
        const function_type* f = \
            reinterpret_cast<const function_type*>( \
                access::get_interface_table(*self) + Offset::value \
            ); \
        advice->before(tuple, function_name(), n, param_names()); \
        if ( is_void<result_type>::value && \
             advice->cancel(tuple, function_name(), n, param_names()) ) \
        { return return_void<result_type>(); } \
        try { \
            return (*f)( const_cast<void*>(pointer) \
                         BOOST_PP_COMMA_IF(n) \
                         BOOST_IDL_INVOKER_ARGS(n) ); \
        } catch (std::exception& e) { \
            finalizer.set_error(); \
            advice->error(e, tuple, function_name(), n, param_names()); \
            throw e; \
        } \
      } \
    }; \
    /**/
#define BOOST_PP_LOCAL_LIMITS (0, BOOST_IDL_FUNCTION_TRAITS_MAX_ARITY)
//#include BOOST_PP_LOCAL_ITERATE()
#undef BOOST_PP_LOCAL_MACRO
          
//------Local iteration defining specializations of transforming_invoker------//

#define BOOST_PP_LOCAL_MACRO(n) \
    template< typename Interface, typename Offset, typename Signature, \
              typename Names, typename Advice > \
    struct transforming_invoker<Interface, Offset, Signature, Names, Advice, n> { \
      typedef typename add_void_pointer<Signature>::type  function_type; \
      typedef typename function_result<Signature>::type   result_type; \
      typedef typename argument_tuple<Signature>::type    tuple_type; \
      typedef ::boost::interfaces::detail::finalizer<Advice, tuple_type> finalizer_type; \
      typedef typename finalizer_type::after_type         after_type; \
      typedef typename finalizer_type::finally_type       finally_type; \
      static const char* function_name() { return Names::function_name(); } \
      static const char** param_names() { return Names::param_names(); } \
      static result_type execute( const void* pv \
                                  BOOST_IDL_INVOKER_PARAMS(Signature, n) ) \
      { \
        const Interface* self = \
            static_cast<Interface*>(const_cast<void*>(pv)); \
        const Advice*    advice        = static_cast<const Advice*>(self); \
        after_type       after         = &Advice::after; \
        finally_type     finally       = &Advice::finally; \
        tuple_type       tuple         = tuple_type(BOOST_IDL_INVOKER_ARGS(n)); \
        finalizer_type   finalizer( advice, after, finally, tuple, \
                                   function_name(), n, param_names() ); \
        const void* pointer = \
          access::get_interface_pointer(*self); \
        const function_type* f = \
            reinterpret_cast<const function_type*>( \
                access::get_interface_table(*self) + Offset::value \
            ); \
        advice->before(tuple, function_name(), n, param_names()); \
        if ( is_void<result_type>::value && \
             advice->cancel(tuple, function_name(), n, param_names()) ) \
        { return return_void<result_type>(); } \
        try { \
            return advice->transform( \
                       (*f)( const_cast<void*>(pointer) \
                             BOOST_PP_COMMA_IF(n) \
                             BOOST_IDL_INVOKER_ARGS(n) ), \
                       tuple, function_name(), n, param_names() ); \
        } catch (std::exception& e) { \
            finalizer.set_error(); \
            advice->error(e, tuple, function_name(), n, param_names()); \
            throw e; \
        } \
      } \
    }; \
    /**/
#define BOOST_PP_LOCAL_LIMITS (0, BOOST_IDL_FUNCTION_TRAITS_MAX_ARITY)
//#include BOOST_PP_LOCAL_ITERATE()
#undef BOOST_PP_LOCAL_MACRO

} } } // End namespaces detail, interfaces, boost.

#endif // #ifndef BOOST_IDL_INVOKER_HPP_INCLUDED
