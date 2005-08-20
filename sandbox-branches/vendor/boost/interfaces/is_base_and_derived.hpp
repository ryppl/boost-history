// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_EXTENDS_HPP_INCLUDED
#define BOOST_IDL_EXTENDS_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/interfaces/is_interface.hpp>
#include <boost/interfaces_fwd.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_base_and_derived.hpp>

namespace boost { namespace interfaces {

namespace detail {

template<typename Subinterface, typename Superinterface>
struct is_base_and_derived_impl
    : boost::is_base_and_derived< 
          typename Superinterface::interface_metadata::proxy,
          typename Subinterface::interface_metadata::proxy
      >
    { };

template<typename Interface>
struct is_base_and_derived_impl<Interface, Interface> : mpl::true_ { };

} // End namespace detail.

template<typename Subinterface, typename Superinterface>
struct is_base_and_derived 
    : mpl::and_< 
          is_interface<Subinterface>,
          is_interface<Superinterface>,
          detail::is_base_and_derived_impl<Subinterface, Superinterface>
      >
    { };

} } // End namespaces interfaces, boost.

#endif // #ifndef BOOST_IDL_EXTENDS_HPP_INCLUDED
