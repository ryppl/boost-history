// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_FN_ACCESS_CONTROL_IMPL_HPP_INCLUDED
#define BOOST_IDL_FN_ACCESS_CONTROL_IMPL_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/detail/workaround.hpp>
#include <boost/config.hpp>

#if BOOST_WORKAROUND(BOOST_MSVC, >= 1310)
# pragma warning(push)
# pragma warning(disable:4224) // parameter previouisly defined as type
#endif

#include <boost/interfaces/detail/config/has_advice.hpp>
#include <boost/interfaces/detail/invoker.hpp>
#include <boost/interfaces/detail/preprocessor/fn_arg_list.hpp>
#include <boost/interfaces/detail/preprocessor/param_name_list.hpp>
#include <boost/preprocessor/array/size.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/stringize.hpp>

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
  BOOST_PP_IIF( BOOST_IDL_HAS_ADVICE(), \
                BOOST_IDL_FN_IMPL_HAS_ADVICE, \
                BOOST_IDL_FN_IMPL_NO_ADVICE ) (name, result, args) \
  /**/
#define BOOST_IDL_FN_IMPL_NO_ADVICE(name, result, args) \
  template<typename Category, typename Dummy2_> \
  struct BOOST_IDL_FN_IMPL_NAME(name) { \
    typedef result signature(BOOST_IDL_NAMED_PARAM_LIST(args)); \
    typedef ::boost::interfaces::detail::null_invoker< \
              DDD_, OOO_, signature \
            > invoker_type; \
    static result execute( const DDD_* self \
                           BOOST_IDL_COMMA_IF_ARGS(args) \
                           BOOST_IDL_NAMED_PARAM_LIST(args) ) \
    { \
      return invoker_type::execute( self  \
                                    BOOST_IDL_COMMA_IF_ARGS(args) \
                                    BOOST_IDL_FN_ARG_LIST(args) ); \
    } \
  }; \
  /**/
#define BOOST_IDL_FN_IMPL_HAS_ADVICE(name, result, args) \
  template<typename Category_, typename Dummy2_> \
  struct BOOST_IDL_FN_IMPL_NAME(name) { \
    typedef result signature(BOOST_IDL_NAMED_PARAM_LIST(args)); \
    struct names { \
      static const char* function_name() { return BOOST_PP_STRINGIZE(name); } \
      static const char** param_names() \
      { \
        BOOST_IDL_PARAM_NAME_LIST(args) \
        return names; \
      } \
    }; \
    typedef ::boost::interfaces::detail::invoker< \
              DDD_, OOO_, signature, names, \
              typename BBB_::interface_advice \
            > invoker_type; \
    static result execute( const DDD_* self \
                           BOOST_IDL_COMMA_IF_ARGS(args) \
                           BOOST_IDL_NAMED_PARAM_LIST(args) ) \
    { \
      return invoker_type::execute( self  \
                                    BOOST_IDL_COMMA_IF_ARGS(args) \
                                    BOOST_IDL_FN_ARG_LIST(args) ); \
    } \
  }; \
  /**/

#if BOOST_WORKAROUND(BOOST_MSVC, >= 1310)
# pragma warning(pop)
#endif

#endif // #ifndef BOOST_IDL_FN_ACCESS_CONTROL_IMPL_HPP_INCLUDED
