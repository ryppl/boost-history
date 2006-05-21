// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_DETAIL_IDL_END_HPP_INCLUDED
#define BOOST_IDL_DETAIL_IDL_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/interfaces_fwd.hpp>
#include <boost/interfaces/is_interface.hpp>
#include <boost/interfaces/access.hpp>
#include <boost/interfaces/detail/generate.hpp>
#include <boost/interfaces/detail/interface_base.hpp>
#include <boost/interfaces/detail/preprocessor/befriend_template.hpp>
#include <boost/interfaces/detail/preprocessor/interface_support.hpp>
#include <boost/interfaces/detail/preprocessor/template_args.hpp>
#include <boost/interfaces/detail/preprocessor/template_decl.hpp>
#include <boost/interfaces/detail/proxy.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/control/expr_if.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/type_traits/detail/yes_no_type.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>

#define BOOST_INTERFACE_END_IMPL(name, arity) \
        ::boost::mpl::na > > { }; \
        public: \
            struct index_idl_ { \
            template<typename N> \
            struct apply \
                : tracker_idl_< ::boost::mpl::int_<N::value>, int > \
                { }; \
            }; \
        }; \
    }; \
    BOOST_IDL_TEMPLATE_DECL(arity, XXX_) \
    class name \
        : public ::boost::interfaces::detail::interface_base< \
                     name BOOST_IDL_TEMPLATE_ARGS(arity, XXX_), \
                     BOOST_PP_CAT(name, _interface_impl_) \
                         BOOST_IDL_TEMPLATE_ARGS(arity, XXX_), \
                     __LINE__ - BOOST_PP_CAT(name, _interface_impl_) \
                     BOOST_IDL_TEMPLATE_ARGS(arity, XXX_)::start_line_idl_ \
                 >::type \
    { \
    private: \
        typedef BOOST_PP_CAT(name, _interface_impl_) \
                BOOST_IDL_TEMPLATE_ARGS(arity, XXX_) implementation_type; \
        typedef BOOST_PP_EXPR_IF(arity, typename) \
                ::boost::interfaces::detail::interface_base< \
                    name BOOST_IDL_TEMPLATE_ARGS(arity, XXX_), \
                    implementation_type, \
                    __LINE__ - implementation_type::start_line_idl_ \
                >::type base_idl_; \
        friend class ::boost::interfaces::access; \
    public: \
        BOOST_IDL_CTORS(name, XXX_, arity) \
        BOOST_IDL_CORE_INTERFACE_SUPPORT(name, arity) \
    }; \
  template< typename XXX_ BOOST_PP_COMMA_IF(arity) \
            BOOST_PP_ENUM_PARAMS(arity, typename XXX_) > \
  ::boost::type_traits::yes_type \
  is_interface_helper( name BOOST_IDL_TEMPLATE_ARGS(arity, XXX_)*, \
                       XXX_*, \
                       typename \
                       ::boost::enable_if< \
                           ::boost::is_same< \
                               XXX_, \
                               name BOOST_IDL_TEMPLATE_ARGS(arity, XXX_) \
                           > \
                       >::type* = 0 ); \
  /**/

#define BOOST_IDL_END(name) BOOST_INTERFACE_END_IMPL(name, 0) 

#define BOOST_IDL_END_TEMPLATE(name, arity) \
    BOOST_INTERFACE_END_IMPL(name, arity) \
    /**/

#define BOOST_IDL_END1(name) BOOST_IDL_END_TEMPLATE(name, 1)
#define BOOST_IDL_END2(name) BOOST_IDL_END_TEMPLATE(name, 2)
#define BOOST_IDL_END3(name) BOOST_IDL_END_TEMPLATE(name, 3)
#define BOOST_IDL_END4(name) BOOST_IDL_END_TEMPLATE(name, 4)
#define BOOST_IDL_END5(name) BOOST_IDL_END_TEMPLATE(name, 5)
#define BOOST_IDL_END6(name) BOOST_IDL_END_TEMPLATE(name, 6)
#define BOOST_IDL_END7(name) BOOST_IDL_END_TEMPLATE(name, 7)
#define BOOST_IDL_END8(name) BOOST_IDL_END_TEMPLATE(name, 8)
#define BOOST_IDL_END9(name) BOOST_IDL_END_TEMPLATE(name, 9)
#define BOOST_IDL_END10(name) BOOST_IDL_END_TEMPLATE(name, 10)

#endif // #ifndef BOOST_IDL_DETAIL_IDL_END_HPP_INCLUDED
