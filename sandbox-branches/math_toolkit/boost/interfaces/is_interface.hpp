// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_IS_INTERFACE_HPP_INCLUDED
#define BOOST_IDL_IS_INTERFACE_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/config.hpp> // BOOST_MSVC
#include <boost/detail/workaround.hpp>

#if BOOST_WORKAROUND(BOOST_MSVC, >= 1310)
# pragma warning(push)
# pragma warning(disable:4675)
#endif

#include <boost/mpl/bool.hpp>
#include <boost/type_traits/detail/yes_no_type.hpp>
#include <boost/type_traits/remove_reference.hpp>

namespace boost { namespace interfaces {

namespace detail {

type_traits::no_type is_interface_helper(...);

template<typename T>
struct is_interface_impl {
    typedef typename remove_reference<T>::type obj_type;
    static obj_type* ptr;
    static const bool value = 
        sizeof(is_interface_helper(ptr,ptr)) == 
        sizeof(type_traits::yes_type);
};

} // End namespace detail.

template<typename T>
struct is_interface 
    : mpl::bool_< detail::is_interface_impl<T>::value >
    { };

} } // End namespaces interfaces, boost.

#if BOOST_WORKAROUND(BOOST_MSVC, >= 1310)
# pragma warning(pop)
#endif

#endif // #ifndef BOOST_IDL_IS_INTERFACE_HPP_INCLUDED
