// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_DETAIL_CONSTANTS_HPP_INCLUDED
#define BOOST_IDL_DETAIL_CONSTANTS_HPP_INCLUDED

#include <boost/mpl/int.hpp>

namespace boost { namespace interfaces { 
    
namespace flags {

const int is_const = 1;
const int is_static = is_const << 1;
const int is_protected = is_static << 1;
const int is_fixed = is_protected << 1;
typedef mpl::int_<0> default_;

} // End namespace flags.

namespace offset {

typedef mpl::int_<3> initial;
typedef mpl::int_<0> default_;

} // End namespace offset.

} } // end namespace interfaces, boost.

#endif // #ifndef BOOST_IDL_DETAIL_CONSTANTS_HPP_INCLUDED
