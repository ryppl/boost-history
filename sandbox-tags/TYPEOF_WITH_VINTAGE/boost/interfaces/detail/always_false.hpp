// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_DETAIL_ALWAYS_FALSE_HPP_INCLUDED
#define BOOST_IDL_DETAIL_ALWAYS_FALSE_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

namespace boost { namespace interfaces { namespace detail {

template<typename T>
struct always_false { enum { value = 0 }; };

} } } // End namespace detail, interfaces, boost.

#endif // #ifndef BOOST_IDL_DETAIL_ALWAYS_FALSE_HPP_INCLUDED
