// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_NO_ACCESS_CONTROL_FN_IMPL_HPP_INCLUDED
#define BOOST_IDL_NO_ACCESS_CONTROL_FN_IMPL_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/detail/workaround.hpp>
#include <boost/config.hpp>

#if BOOST_WORKAROUND(BOOST_MSVC, == 1310)
# pragma warning(push)
# pragma warning(disable:4224) // parameter previouisly defined as type
#endif

#include <exception>
#include <boost/interfaces/advice_categories.hpp>
#include <boost/interfaces/detail/config/has_advice.hpp>
#include <boost/interfaces/detail/finalizer.hpp>
#include <boost/interfaces/detail/preprocessor/fn_arg_list.hpp>
#include <boost/interfaces/detail/preprocessor/member_data.hpp>
#include <boost/interfaces/detail/preprocessor/param_name_list.hpp>
#include <boost/interfaces/detail/preprocessor/param_type.hpp>
#include <boost/interfaces/detail/return_void.hpp>
#include <boost/preprocessor/array/elem.hpp>
#include <boost/preprocessor/array/size.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/facilities/identity.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/type_traits/add_reference.hpp>

//
// Macro: BOOST_IDL_COMMA_IF_ARGS(args)
// Parameters:
//     args - an array (arg0, ...., argn) of tuples consisting of 
//            tuples (type, name) if HAS_NAMED_PARAMS() is true and
//            typenames otherwise.
// Expands to: A comma if args has non-zero size, and nothing otherwise.
//        
#define BOOST_IDL_COMMA_IF_ARGS(args) \
    BOOST_PP_COMMA_IF(BOOST_PP_ARRAY_SIZE(args))
    /**/

//
// Macro: BOOST_IDL_TUPLE_TYPE(args)
// Parameters:
//     args - an array (arg0, ...., argn) of tuples consisting of 
//            tuples (type, name) if HAS_NAMED_PARAMS() is true and
//            typenames otherwise.
// Expands to: 
//
//     ::boost::tuple<
//        typename add_reference<BOOST_IDL_PARAM_TYPE(arg0)>::type,
//        ...,
//        typename add_reference<BOOST_IDL_PARAM_TYPE(argn)>::type
//     >
//
#define BOOST_IDL_TUPLE_TYPE(args) \
    ::boost::tuple< BOOST_PP_REPEAT( BOOST_PP_ARRAY_SIZE(args), \
                    BOOST_IDL_TUPLE_TYPE_I, args) > \
    /**/
#define BOOST_IDL_TUPLE_TYPE_I(z, n, args) \
    BOOST_PP_COMMA_IF(n) typename \
    ::boost::add_reference< \
        BOOST_IDL_PARAM_TYPE(BOOST_PP_ARRAY_ELEM(n, args)) \
    >::type \
    /**/
     
#define BOOST_IDL_FN_IMPL_NAME(name) \
    BOOST_PP_CAT(name, BOOST_PP_CAT(_function_impl_, __LINE__)) \
    /**/

//
// Macro: BOOST_IDL_FN_IMPL_CORE(name, result, args)
// Parameters:
//     name - the name of an interface member function
//     result - the result type of the named function
//     args - an array (arg0, ...., argn) of tuples consisting of 
//            tuples (type, name) if HAS_NAMED_PARAMS() is true and
//            typenames otherwise.
// Expands to: The definition of a template 
//
//     template<typename AdviceCategory, typename Dummy>
//     struct name_function_impl_xxx {
//         static result execute(const DDD_* ...);
//     };
//        
//
#define BOOST_IDL_FN_IMPL_CORE(name, result, args) \
  template<typename Category_, typename Dummy2_> \
  struct BOOST_IDL_FN_IMPL_NAME(name) { \
    typedef result (*function_type) (BOOST_IDL_AUGMENTED_PARAM_LIST(args)); \
    static result execute( const DDD_* self \
                           BOOST_IDL_COMMA_IF_ARGS(args) \
                           BOOST_IDL_NAMED_PARAM_LIST(args) ) \
    { \
      function_type f = \
        reinterpret_cast<function_type>((self->BOOST_IDL_MEMBER_TABLE)[OOO_::value]); \
      return f( const_cast<void*>(self->BOOST_IDL_MEMBER_PTR) \
                BOOST_IDL_COMMA_IF_ARGS(args) \
                BOOST_IDL_FN_ARG_LIST(args) ); \
    } \
  }; \
  BOOST_PP_IIF( BOOST_IDL_HAS_ADVICE(), \
                BOOST_IDL_FN_IMPL_HAS_ADVICE, \
                BOOST_IDL_FN_IMPL_NO_ADVICE ) (name, result, args) \
  /**/
# define BOOST_IDL_FN_IMPL_NO_ADVICE(name, result, args)
# define BOOST_IDL_FN_IMPL_HAS_ADVICE(name, result, args) \
  template<typename Dummy2_> \
  struct BOOST_IDL_FN_IMPL_NAME(name) \
         < ::boost::interfaces::basic_advice_tag, Dummy2_ > \
  { \
    typedef result (*function_type) (BOOST_IDL_AUGMENTED_PARAM_LIST(args)); \
    typedef BOOST_IDL_TUPLE_TYPE(args) tuple_type; \
    typedef ::boost::interfaces::detail::finalizer \
            <BBB_, tuple_type>                      finalizer_type; \
    typedef typename finalizer_type::after_type     after_type; \
    typedef typename finalizer_type::finally_type   finally_type; \
    BOOST_STATIC_CONSTANT(int, arity = BOOST_PP_ARRAY_SIZE(args)); \
    static const char* function_name() { return BOOST_PP_STRINGIZE(name); } \
    static const char** param_names() \
    { \
        BOOST_IDL_PARAM_NAME_LIST(args) \
        return names; \
    } \
    static result \
    execute( const DDD_* self BOOST_IDL_COMMA_IF_ARGS(args) \
             BOOST_IDL_NAMED_PARAM_LIST(args) ) \
    { \
      const BBB_*     advice    = static_cast<const BBB_*>(self); \
      after_type      after     = &BBB_::after; \
      finally_type    finally   = &BBB_::finally; \
      tuple_type      tuple     = tuple_type(BOOST_IDL_FN_ARG_LIST(args)); \
      finalizer_type  finalizer( advice, after, finally, tuple, \
                                 function_name(), arity, param_names() ); \
      advice->before(tuple, function_name(), arity, param_names()); \
      if ( ::boost::is_void<result>::value && \
           advice->cancel(tuple, function_name(), arity, param_names()) ) \
      { return ::boost::interfaces::detail::return_void<result>(); } \
      try { \
        function_type f = \
          reinterpret_cast<function_type>((self->BOOST_IDL_MEMBER_TABLE)[OOO_::value]); \
        return f( const_cast<void*>(self->BOOST_IDL_MEMBER_PTR) \
                  BOOST_IDL_COMMA_IF_ARGS(args) \
                  BOOST_IDL_FN_ARG_LIST(args) ); \
      } catch (std::exception& e) { \
        finalizer.set_error(); \
        advice->error(e, tuple, function_name(), arity, param_names()); \
        throw e; \
      } \
    } \
  }; \
  template<typename Dummy2_> \
  struct BOOST_IDL_FN_IMPL_NAME(name) \
         < ::boost::interfaces::transforming_advice_tag, Dummy2_ > \
  { \
    typedef result (*function_type) (BOOST_IDL_AUGMENTED_PARAM_LIST(args)); \
    typedef BOOST_IDL_TUPLE_TYPE(args) tuple_type; \
    typedef ::boost::interfaces::detail::finalizer \
            <BBB_, tuple_type>                     finalizer_type; \
    typedef typename finalizer_type::after_type     after_type; \
    typedef typename finalizer_type::finally_type   finally_type; \
    BOOST_STATIC_CONSTANT(int, arity = BOOST_PP_ARRAY_SIZE(args)); \
    static const char* function_name() { return BOOST_PP_STRINGIZE(name); } \
    static const char** param_names() \
    { \
        BOOST_IDL_PARAM_NAME_LIST(args) \
        return names; \
    } \
    static result \
    execute( const DDD_* self BOOST_IDL_COMMA_IF_ARGS(args) \
             BOOST_IDL_NAMED_PARAM_LIST(args) ) \
    { \
      const BBB_*     advice    = static_cast<const BBB_*>(self); \
      after_type      after     = &BBB_::after; \
      finally_type    finally   = &BBB_::finally; \
      tuple_type      tuple     = tuple_type(BOOST_IDL_FN_ARG_LIST(args)); \
      finalizer_type  finalizer( advice, after, finally, tuple, \
                                 function_name(), arity, param_names() ); \
      advice->before(tuple, function_name(), arity, param_names()); \
      if ( ::boost::is_void<result>::value && \
           advice->cancel(tuple, function_name(), arity, param_names()) ) \
      { return ::boost::interfaces::detail::return_void<result>(); } \
      try { \
        function_type f = \
          reinterpret_cast<function_type>((self->BOOST_IDL_MEMBER_TABLE)[OOO_::value]); \
        return advice->transform( \
          f( const_cast<void*>(self->BOOST_IDL_MEMBER_PTR) \
             BOOST_IDL_COMMA_IF_ARGS(args) \
             BOOST_IDL_FN_ARG_LIST(args) ), \
          tuple, function_name(), arity, param_names() \
        ); \
      } catch (std::exception& e) { \
        finalizer.set_error(); \
        advice->error(e, tuple, function_name(), arity, param_names()); \
        throw e; \
      } \
    } \
  }; \
  /**/

#if BOOST_WORKAROUND(BOOST_MSVC, == 1310)
# pragma warning(pop)
#endif

#endif // #ifndef BOOST_IDL_NO_ACCESS_CONTROL_FN_IMPL_HPP_INCLUDED
