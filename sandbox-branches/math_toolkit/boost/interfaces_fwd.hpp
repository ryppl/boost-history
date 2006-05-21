// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_INTERFACES_FWD_HPP_INCLUDED
#define BOOST_IDL_INTERFACES_FWD_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <typeinfo>
#include <boost/mpl/limits/list.hpp>
#include <boost/preprocessor/arithmetic/add.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>

#define BOOST_IDL_CROSSCUT_ARITY() \
    BOOST_PP_ADD(BOOST_MPL_LIMIT_LIST_SIZE, 2) \
    /**/

namespace boost { namespace interfaces {

template<typename Subinterface, typename Superinterfaces>
class subinterface;

template<typename Interface>
class const_view;

template<typename Interface>
class fixed_view;

template<typename Interface>
class delegating;

template<BOOST_PP_ENUM_PARAMS(BOOST_IDL_CROSSCUT_ARITY(), typename P)>
class crosscut_interface;

template<typename Interface>
class manual_ptr;

template<typename Interface>
class unique_ptr;

template<typename Interface>
class shared_ptr;

template<typename Interface>
class unique_obj;

template<typename Interface>
class shared_obj;

template<typename T>
struct is_interface;

template<typename T, typename Interface>
struct implements;

template<typename Interface>
void swap(Interface&, Interface&);

template<typename Interface, typename Delegator>
fixed_view<Interface>* get_delegate(Delegator&);

template<typename Interface, typename Delegator, typename T>
void set_delegate(Delegator&, T*);

template<typename X, typename Interface>
X& extract(const Interface& i);

template<typename Interface>
const std::type_info& target_type(const Interface& i);

namespace detail {

template<typename Interface, typename InterfaceImpl, int Lines>
struct interface_base;

template<typename Interface, typename Offset, typename Signature, int Arity>
struct null_invoker;

template<typename Interface, typename Offset, typename Signature, int Arity>
struct basic_invoker;

template<typename Interface, typename Offset, typename Signature, int Arity>
struct transforming_invoker;

template<typename Subinterface, typename Superinterface>
struct is_base_and_derived_impl;

template<typename Interface>
struct interface_table_impl;

template<typename T>
class unique_obj_impl;

template<typename T>
class shared_obj_impl;

} // End namespace detail.

} } // End namespaces interfaces, boost.

#endif // #ifndef BOOST_IDL_INTERFACES_FWD_HPP_INCLUDED
