// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_REMOVE_QUALIFIERS_HPP_INCLUDED
#define BOOST_IDL_REMOVE_QUALIFIERS_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/mpl/identity.hpp>

namespace boost { namespace interfaces {

template<typename T>
struct remove_qualifiers 
    : mpl::identity<T>
    { };

} } // End namespaces interfaces, boost.

#endif // #ifndef BOOST_IDL_REMOVE_QUALIFIERS_HPP_INCLUDED
