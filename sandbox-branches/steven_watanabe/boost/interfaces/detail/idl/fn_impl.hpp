// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_FN_IMPL_HPP_INCLUDED
#define BOOST_IDL_FN_IMPL_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/detail/workaround.hpp>
#include <boost/config.hpp>

#if BOOST_WORKAROUND(BOOST_MSVC, == 1310)
# pragma warning(push)
# pragma warning(disable:4224) // parameter previouisly defined as type
#endif

#include <cassert>
#include <utility>             // pair.
#include <boost/interfaces/advice_categories.hpp>
#include <boost/interfaces/detail/config/has_advice.hpp>
#include <boost/interfaces/detail/config/has_named_params.hpp>
#include <boost/interfaces/detail/constants.hpp>
#include <boost/interfaces/detail/fn_ptr.hpp>
#include <boost/interfaces/detail/invoker.hpp>
#include <boost/interfaces/detail/preprocessor/fn_arg_list.hpp>
#include <boost/interfaces/detail/preprocessor/param_name_list.hpp>
#include <boost/interfaces/access.hpp> // NO_ACCESS_CONTROL, PRIVATE.
#include <boost/mpl/int.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/control/expr_iif.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/static_assert.hpp>

#define BOOST_IDL_COMMA_IF_ARGS(args) \
    BOOST_PP_COMMA_IF(BOOST_PP_ARRAY_SIZE(args))
    /**/

namespace boost { namespace interfaces { namespace detail {

template<typename T>
struct dummy { };

} } } // End namespaces detail, interfaces, boost.

#ifndef BOOST_IDL_NO_IMPLEMENTS
# define BOOST_IDL_IMPLEMENTS(name, result, args, is_const_) \
    template<typename XXX_> \
    struct implements { \
        template< result (XXX_::*MemFun)(BOOST_IDL_PARAM_LIST(args)) \
                  BOOST_PP_EXPR_IIF(is_const_, const) > \
        struct holder { }; \
        typedef char (&yes) [1]; \
        typedef char (&no) [2]; \
        template<typename YYY_> \
        static no tester(...); \
        template<typename YYY_> \
        static yes tester( holder<&YYY_::name >* = 0 ); \
        static const bool value = sizeof(tester<XXX_>(0)) == sizeof(yes); \
        typedef ::boost::mpl::bool_<value> type; \
    }; \
   /**/
#else
# define BOOST_IDL_IMPLEMENTS(name, result, args, is_const_)
#endif

#define BOOST_IDL_FN_IMPL(name, result, args, is_const_) \
    ::boost::mpl::na > > { }; \
  BOOST_IDL_PRIVATE: \
  template<typename Dummy_> \
  struct tracker_idl_< ::boost::mpl::int_<__LINE__ - start_line_idl_>, Dummy_> { \
    typedef ::boost::interfaces::detail::fn_ptr fn_ptr; \
    template<typename XXX_> \
    struct function_holder { \
      static result function(BOOST_IDL_AUGMENTED_PARAM_LIST(args)) \
      { \
        return static_cast<XXX_*>(xxx_)->name(BOOST_IDL_FN_ARG_LIST(args)); \
      } \
    }; \
    struct type { \
      template< typename DDD_, typename OOO_, \
                typename FFF_, typename BBB_ > \
      struct apply { \
        struct type : BBB_ { \
          struct interface_metadata { \
            typedef DDD_ derived_type; \
            typedef OOO_ offset_type; \
            typedef FFF_ flags_type; \
            typedef BBB_ base_type; \
            static const int size = 1; \
            typedef ::boost::mpl::vector1< \
                        result(*BOOST_PP_EXPR_IIF(is_const_, const)) \
                        (BOOST_IDL_NAMED_PARAM_LIST(args)) \
                    > \
                    signatures; \
            BOOST_IDL_IMPLEMENTS(name, result, args, is_const_) \
            struct proxy { }; \
            template<typename XXX_> \
            static void initialize(fn_ptr* table) \
            { \
              table[OOO_::value] = \
                reinterpret_cast<fn_ptr>(&function_holder<XXX_>::function); \
            } \
            ::std::pair<const char*, fn_ptr> \
            lookup(int, fn_ptr* table) \
            { \
                return ::std::pair<const char*, fn_ptr> \
                    (BOOST_PP_STRINGIZE(name), table[OOO_::value]); \
            } \
          }; \
          using BBB_::name; \
          result name(BOOST_IDL_NAMED_PARAM_LIST(args)) \
            BOOST_PP_EXPR_IIF(is_const_, const) \
          { \
            typedef ::boost::interfaces::detail::invoker< \
                        type, OOO_, \
                        result(BOOST_IDL_NAMED_PARAM_LIST(args)) \
                    > invoker; \
            return invoker::invoke( this BOOST_IDL_COMMA_IF_ARGS(args) \
                                    BOOST_IDL_FN_ARG_LIST(args) ); \
          } \
        }; \
      };\
    }; \
  }; \
  friend struct tracker_idl_< ::boost::mpl::int_<__LINE__ - start_line_idl_>, int>; \
  struct BOOST_PP_CAT(dummy_idl_, __LINE__); \
  void name(BOOST_PP_CAT(dummy_idl_, __LINE__)*) { } \
  struct BOOST_PP_CAT(dummy_idl_, __LINE__) \
       : ::boost::interfaces::detail::dummy< \
           ::boost::interfaces::detail::dummy< \
  /**/

#if BOOST_WORKAROUND(BOOST_MSVC, == 1310)
# pragma warning(pop)
#endif

#endif // #ifndef BOOST_IDL_FN_IMPL_HPP_INCLUDED
